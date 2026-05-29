#include "linkx.h"
#include "network_adapter_manager.h"
#include "soem.h"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <string_view>
#include <thread>

namespace
{
using Clock = std::chrono::steady_clock;

constexpr int kDefaultRunSeconds = 10;
constexpr std::uint32_t kSyncShiftNs = 1 * 1000;
constexpr int kDefaultPdoLoopSleepUs = 300;
constexpr int kDefaultFramesPerCycle = 4;
constexpr DWORD kPdoThreadStopTimeoutMs = 5000;
constexpr std::size_t kDemoPayloadSize = 8;
// The demo uses an 8-byte payload so a receiver can identify looped-back frames
// by origin channel, sequence number, and marker bytes.
constexpr std::uint32_t kMaxFramesPerCycle =
    static_cast<std::uint32_t>(LINKX_PACKET_DATA_SIZE / (sizeof(can_frame_head_t) + kDemoPayloadSize));

// Process image shared with SOEM. ecx_config_map_group() maps slave PDOs into
// this buffer, and LinkxDevice copies its packet structs to/from the mapped PDOs.
std::uint8_t IOmap[4 * 1024] = {};
ecx_contextt ctx = {};
LinkxDevice linkx;
int expected_wkc = 0;

// Counters are atomic because this file originally evolved from threaded test
// code. The current demo loop runs on the main thread, but relaxed atomics keep
// the stats helpers simple and ready for future background polling.
std::atomic_uint32_t pdo_queued_frame_count{0};
std::atomic_uint32_t pdo_acked_frame_count{0};
std::atomic_uint32_t rx_frame_count{0};
std::atomic_uint32_t pdo_queued_channel_count[LINKX_CAN_CHANNEL_NUM] = {};
std::atomic_uint32_t pdo_acked_channel_count[LINKX_CAN_CHANNEL_NUM] = {};
std::atomic_uint32_t rx_channel_count[LINKX_CAN_CHANNEL_NUM] = {};
std::atomic_uint32_t rx_origin_channel_count[LINKX_CAN_CHANNEL_NUM] = {};
std::atomic_uint32_t rx_missing_count[LINKX_CAN_CHANNEL_NUM] = {};
std::atomic_uint32_t rx_out_of_order_count[LINKX_CAN_CHANNEL_NUM] = {};
std::atomic_uint32_t wkc_ok_count{0};
std::atomic_uint32_t wkc_bad_count{0};
std::atomic_int last_wkc{0};
std::atomic_uint32_t parse_error_count{0};
std::atomic_uint32_t rx_bad_channel_count{0};
std::atomic_uint32_t rx_invalid_payload_count{0};
std::atomic_uint32_t tx_append_fail_count{0};
std::atomic_uint32_t rx_nonempty_pdo_count{0};
std::atomic_uint32_t rx_empty_pdo_count{0};
std::atomic_uint32_t first_rx_print_count{0};
std::atomic_bool pdo_stop_requested{false};
std::atomic_bool pdo_thread_finished{false};

std::uint32_t expected_rx_sequence[LINKX_CAN_CHANNEL_NUM] = {};
bool rx_sequence_seen[LINKX_CAN_CHANNEL_NUM] = {};

struct PdoThreadConfig
{
    std::uint32_t frames_per_cycle = kDefaultFramesPerCycle;
    int pdo_period_us = kDefaultPdoLoopSleepUs;
    int run_seconds = kDefaultRunSeconds;
    Clock::time_point run_start = Clock::now();
};

void wait_until_next_cycle(Clock::time_point* next_cycle, std::chrono::microseconds period);

// Called for every received frame in input_packet. It validates the demo payload
// pattern and tracks missing/out-of-order sequences per original TX channel.
void count_rx_frame(const can_frame_t* frame, void*)
{
    rx_frame_count.fetch_add(1, std::memory_order_relaxed);
    if (frame->head.channel < LINKX_CAN_CHANNEL_NUM)
    {
        rx_channel_count[frame->head.channel].fetch_add(1, std::memory_order_relaxed);
    }
    else
    {
        rx_bad_channel_count.fetch_add(1, std::memory_order_relaxed);
    }

    const std::uint8_t payload_size = linkx_dlc_to_payload_size(frame->head.dlc);
    if (payload_size >= kDemoPayloadSize &&
        frame->payload[5] == 0xAA &&
        frame->payload[6] == 0x55 &&
        frame->payload[7] == 0x5A)
    {
        const std::uint8_t origin_channel = frame->payload[0];
        const std::uint32_t sequence =
            static_cast<std::uint32_t>(frame->payload[1]) |
            (static_cast<std::uint32_t>(frame->payload[2]) << 8) |
            (static_cast<std::uint32_t>(frame->payload[3]) << 16) |
            (static_cast<std::uint32_t>(frame->payload[4]) << 24);

        if (origin_channel < LINKX_CAN_CHANNEL_NUM)
        {
            rx_origin_channel_count[origin_channel].fetch_add(1, std::memory_order_relaxed);
            if (!rx_sequence_seen[origin_channel])
            {
                if (sequence > 0)
                {
                    rx_missing_count[origin_channel].fetch_add(sequence, std::memory_order_relaxed);
                }
                rx_sequence_seen[origin_channel] = true;
                expected_rx_sequence[origin_channel] = sequence + 1;
            }
            else if (sequence == expected_rx_sequence[origin_channel])
            {
                ++expected_rx_sequence[origin_channel];
            }
            else if (sequence > expected_rx_sequence[origin_channel])
            {
                rx_missing_count[origin_channel].fetch_add(sequence - expected_rx_sequence[origin_channel],
                                                           std::memory_order_relaxed);
                expected_rx_sequence[origin_channel] = sequence + 1;
            }
            else
            {
                rx_out_of_order_count[origin_channel].fetch_add(1, std::memory_order_relaxed);
            }
        }
        else
        {
            rx_invalid_payload_count.fetch_add(1, std::memory_order_relaxed);
        }
    }
    else
    {
        rx_invalid_payload_count.fetch_add(1, std::memory_order_relaxed);
    }

    const std::uint32_t print_index = first_rx_print_count.fetch_add(1, std::memory_order_relaxed);
    if (print_index < 16)
    {
        std::printf("rx sample[%u]: channel=%u can_id=0x%X dlc=%u payload_len=%u data=",
                    print_index + 1,
                    frame->head.channel,
                    frame->head.can_id,
                    frame->head.dlc,
                    payload_size);
        for (std::uint8_t i = 0; i < payload_size; ++i)
        {
            std::printf("%02X ", frame->payload[i]);
        }
        std::printf("\n");
    }
}

void run_pdo_cycle(std::uint32_t frames_per_cycle)
{
    static std::uint32_t sequence[LINKX_CAN_CHANNEL_NUM] = {};
    static std::uint8_t channel = 0;
    std::uint32_t queued_this_cycle = 0;
    std::uint32_t queued_channel_this_cycle[LINKX_CAN_CHANNEL_NUM] = {};

    linkx.clear_output_packet();
    for (std::uint32_t i = 0; i < frames_per_cycle; ++i)
    {
        const std::uint32_t current_sequence = sequence[channel]++;
        // Payload format:
        //   byte0    origin CAN channel
        //   byte1-4  little-endian sequence number
        //   byte5-7  marker used to distinguish demo frames from other traffic
        const std::uint8_t payload[kDemoPayloadSize] = {
            channel,
            static_cast<std::uint8_t>(current_sequence & 0xFF),
            static_cast<std::uint8_t>((current_sequence >> 8) & 0xFF),
            static_cast<std::uint8_t>((current_sequence >> 16) & 0xFF),
            static_cast<std::uint8_t>((current_sequence >> 24) & 0xFF),
            0xAA,
            0x55,
            0x5A,
        };

        if (linkx.append_output_can_frame(channel, channel + 1, true, true, false, false, payload, sizeof(payload)))
        {
            pdo_queued_frame_count.fetch_add(1, std::memory_order_relaxed);
            pdo_queued_channel_count[channel].fetch_add(1, std::memory_order_relaxed);
            ++queued_this_cycle;
            ++queued_channel_this_cycle[channel];
        }
        else
        {
            tx_append_fail_count.fetch_add(1, std::memory_order_relaxed);
            break;
        }

        channel = static_cast<std::uint8_t>((channel + 1) % LINKX_CAN_CHANNEL_NUM);
    }

    // One EtherCAT process-data exchange sends the just-built output packet and
    // refreshes input_packet with frames received by the LinkX module.
    const int wkc = linkx.exchange_processdata(EC_TIMEOUTRET);
    last_wkc.store(wkc, std::memory_order_relaxed);
    if (wkc >= expected_wkc)
    {
        wkc_ok_count.fetch_add(1, std::memory_order_relaxed);
        pdo_acked_frame_count.fetch_add(queued_this_cycle, std::memory_order_relaxed);
        for (std::uint8_t ch = 0; ch < LINKX_CAN_CHANNEL_NUM; ++ch)
        {
            pdo_acked_channel_count[ch].fetch_add(queued_channel_this_cycle[ch], std::memory_order_relaxed);
        }
    }
    else
    {
        wkc_bad_count.fetch_add(1, std::memory_order_relaxed);
    }

    if (wkc < expected_wkc)
    {
        return;
    }

    if (linkx.input_packet().nbytes &&
        !linkx.foreach_input_frame(count_rx_frame, nullptr))
    {
        parse_error_count.fetch_add(1, std::memory_order_relaxed);
    }

    if (linkx.input_packet().nbytes)
    {
        rx_nonempty_pdo_count.fetch_add(1, std::memory_order_relaxed);
    }
    else
    {
        rx_empty_pdo_count.fetch_add(1, std::memory_order_relaxed);
    }
}

OSAL_THREAD_FUNC_RT pdo_rt_thread(void* param)
{
    const auto* config = static_cast<const PdoThreadConfig*>(param);
    const auto period = std::chrono::microseconds(config->pdo_period_us);
    Clock::time_point next_cycle = Clock::now();
    const Clock::time_point deadline =
        config->run_seconds > 0
            ? config->run_start + std::chrono::seconds(config->run_seconds)
            : (Clock::time_point::max)();

    while (!pdo_stop_requested.load(std::memory_order_relaxed))
    {
        if (Clock::now() >= deadline)
        {
            break;
        }

        run_pdo_cycle(config->frames_per_cycle);
        wait_until_next_cycle(&next_cycle, period);
    }

    pdo_thread_finished.store(true, std::memory_order_release);
}

bool start_pdo_rt_thread(OSAL_THREAD_HANDLE* thread_handle, PdoThreadConfig* config)
{
    pdo_stop_requested.store(false, std::memory_order_release);
    pdo_thread_finished.store(false, std::memory_order_release);
    *thread_handle = nullptr;
    if (osal_thread_create_rt(thread_handle,
                              128000,
                              reinterpret_cast<void*>(pdo_rt_thread),
                              config) != 0)
    {
        return true;
    }

    if (*thread_handle == nullptr)
    {
        return false;
    }

    // SOEM 2.0 win32 osal_thread_create_rt can create the thread but report
    // failure because it passes the handle storage address to SetThreadPriority.
    // Keep using the SOEM thread API, then set priority on the actual HANDLE.
    if (!SetThreadPriority(*thread_handle, THREAD_PRIORITY_TIME_CRITICAL))
    {
        std::printf("SOEM RT thread was created, but SetThreadPriority failed: %lu\n", GetLastError());
    }
    return true;
}

bool wait_pdo_rt_thread(OSAL_THREAD_HANDLE thread_handle)
{
    if (thread_handle == nullptr)
    {
        return true;
    }

    const DWORD wait_result = WaitForSingleObject(thread_handle, kPdoThreadStopTimeoutMs);
    CloseHandle(thread_handle);
    return wait_result == WAIT_OBJECT_0;
}

void print_usage(const char* exe)
{
    std::printf("Usage:\n");
    std::printf("  %s --list\n", exe);
    std::printf("  %s <adapter_index_or_name> [run_seconds] [slave_id] [frames_per_cycle] [pdo_period_us]\n", exe);
    std::printf("\n");
    std::printf("Example:\n");
    std::printf("  %s 1 10 1 4 300\n", exe);
    std::printf("  %s USB 10 1 4 300\n", exe);
    std::printf("  frames_per_cycle range: 1..%u for 8-byte CAN/CANFD payloads\n", kMaxFramesPerCycle);
}

bool parse_int(const char* text, int* value)
{
    if (text == nullptr || value == nullptr)
    {
        return false;
    }

    char* end = nullptr;
    const long parsed = std::strtol(text, &end, 10);
    if (end == text || *end != '\0')
    {
        return false;
    }

    *value = static_cast<int>(parsed);
    return true;
}

char ascii_lower(char c)
{
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

bool contains_case_insensitive(std::string_view text, std::string_view pattern)
{
    if (pattern.empty())
    {
        return true;
    }

    if (pattern.size() > text.size())
    {
        return false;
    }

    for (std::size_t start = 0; start <= text.size() - pattern.size(); ++start)
    {
        bool matched = true;
        for (std::size_t offset = 0; offset < pattern.size(); ++offset)
        {
            if (ascii_lower(text[start + offset]) != ascii_lower(pattern[offset]))
            {
                matched = false;
                break;
            }
        }

        if (matched)
        {
            return true;
        }
    }

    return false;
}

bool select_adapter_by_arg(NetworkAdapterManager& adapter_manager, const char* arg, int* adapter_index)
{
    int parsed_index = -1;
    if (parse_int(arg, &parsed_index))
    {
        if (parsed_index < 0 || !adapter_manager.select_adapter(static_cast<std::size_t>(parsed_index)))
        {
            return false;
        }

        *adapter_index = parsed_index;
        return true;
    }

    const std::string_view needle(arg != nullptr ? arg : "");
    const auto& adapters = adapter_manager.adapters();
    // Besides numeric indexes, accept a substring of the adapter name or
    // description so users can run the demo with words like "Intel" or "USB".
    for (std::size_t i = 0; i < adapters.size(); ++i)
    {
        if (contains_case_insensitive(adapters[i].name, needle) ||
            contains_case_insensitive(adapters[i].description, needle))
        {
            if (!adapter_manager.select_adapter(i))
            {
                return false;
            }

            *adapter_index = static_cast<int>(i);
            return true;
        }
    }

    return false;
}

void wait_until_next_cycle(Clock::time_point* next_cycle, std::chrono::microseconds period)
{
    *next_cycle += period;
    const Clock::time_point now = Clock::now();
    if (now >= *next_cycle)
    {
        *next_cycle = now;
        return;
    }

    while (Clock::now() < *next_cycle)
    {
        // Busy wait keeps the PDO cycle tighter than Sleep() on a stock Windows
        // timer. For production code, consider a real-time thread/timer policy.
    }
}

void print_stats(int second, Clock::time_point run_start)
{
    const std::uint32_t pdo_queued_frames = pdo_queued_frame_count.load(std::memory_order_relaxed);
    const std::uint32_t pdo_acked_frames = pdo_acked_frame_count.load(std::memory_order_relaxed);
    const std::uint32_t rx_frames = rx_frame_count.load(std::memory_order_relaxed);
    const std::uint32_t wkc_ok = wkc_ok_count.load(std::memory_order_relaxed);
    const std::uint32_t wkc_bad = wkc_bad_count.load(std::memory_order_relaxed);
    const std::uint32_t cycles = wkc_ok + wkc_bad;
    const double elapsed_seconds =
        std::chrono::duration<double>(Clock::now() - run_start).count();
    const double cycle_rate = elapsed_seconds > 0.0 ? static_cast<double>(cycles) / elapsed_seconds : 0.0;
    std::printf("[%ds elapsed=%.3fs cycles=%u rate=%.1f/s] pdo_queued=%u pdo_acked=%u rx_frames=%u ack_rx_delta=%u wkc_ok=%u wkc_bad=%u last_wkc=%d parse_errors=%u\n",
                second,
                elapsed_seconds,
                cycles,
                cycle_rate,
                pdo_queued_frames,
                pdo_acked_frames,
                rx_frames,
                pdo_acked_frames >= rx_frames ? pdo_acked_frames - rx_frames : 0,
                wkc_ok,
                wkc_bad,
                last_wkc.load(std::memory_order_relaxed),
                parse_error_count.load(std::memory_order_relaxed));
    std::printf("     pdo input nonempty=%u empty=%u\n",
                rx_nonempty_pdo_count.load(std::memory_order_relaxed),
                rx_empty_pdo_count.load(std::memory_order_relaxed));
    std::printf("     queued ch0=%u ch1=%u ch2=%u ch3=%u append_fail=%u\n",
                pdo_queued_channel_count[0].load(std::memory_order_relaxed),
                pdo_queued_channel_count[1].load(std::memory_order_relaxed),
                pdo_queued_channel_count[2].load(std::memory_order_relaxed),
                pdo_queued_channel_count[3].load(std::memory_order_relaxed),
                tx_append_fail_count.load(std::memory_order_relaxed));
    std::printf("     acked  ch0=%u ch1=%u ch2=%u ch3=%u expected_wkc=%d\n",
                pdo_acked_channel_count[0].load(std::memory_order_relaxed),
                pdo_acked_channel_count[1].load(std::memory_order_relaxed),
                pdo_acked_channel_count[2].load(std::memory_order_relaxed),
                pdo_acked_channel_count[3].load(std::memory_order_relaxed),
                expected_wkc);
    std::printf("     rx physical ch0=%u ch1=%u ch2=%u ch3=%u bad=%u invalid_payload=%u\n",
                rx_channel_count[0].load(std::memory_order_relaxed),
                rx_channel_count[1].load(std::memory_order_relaxed),
                rx_channel_count[2].load(std::memory_order_relaxed),
                rx_channel_count[3].load(std::memory_order_relaxed),
                rx_bad_channel_count.load(std::memory_order_relaxed),
                rx_invalid_payload_count.load(std::memory_order_relaxed));
    std::printf("     rx origin   ch0=%u ch1=%u ch2=%u ch3=%u | missing ch0=%u ch1=%u ch2=%u ch3=%u | out_of_order=%u\n",
                rx_origin_channel_count[0].load(std::memory_order_relaxed),
                rx_origin_channel_count[1].load(std::memory_order_relaxed),
                rx_origin_channel_count[2].load(std::memory_order_relaxed),
                rx_origin_channel_count[3].load(std::memory_order_relaxed),
                rx_missing_count[0].load(std::memory_order_relaxed),
                rx_missing_count[1].load(std::memory_order_relaxed),
                rx_missing_count[2].load(std::memory_order_relaxed),
                rx_missing_count[3].load(std::memory_order_relaxed),
                rx_out_of_order_count[0].load(std::memory_order_relaxed) +
                    rx_out_of_order_count[1].load(std::memory_order_relaxed) +
                    rx_out_of_order_count[2].load(std::memory_order_relaxed) +
                    rx_out_of_order_count[3].load(std::memory_order_relaxed));
}

void print_dc_status(int slave_id, const char* stage)
{
    const auto& slave = ctx.slavelist[slave_id];
    const auto& group = ctx.grouplist[0];
    std::printf("DC status [%s]: slave_hasdc=%u group_hasdc=%u DCactive=%u DCcycle=%d ns DCshift=%d ns pdelay=%d ns DCtime=%lld\n",
                stage,
                static_cast<unsigned>(slave.hasdc),
                static_cast<unsigned>(group.hasdc),
                static_cast<unsigned>(slave.DCactive),
                slave.DCcycle,
                slave.DCshift,
                slave.pdelay,
                static_cast<long long>(ctx.DCtime));
}
}

int main(int argc, char** argv)
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::printf("DM-LinkX ECAT hardware PDO demo\n");

    NetworkAdapterManager adapter_manager(ctx);
    adapter_manager.scan();

    if (argc < 2)
    {
        adapter_manager.show_adapters(std::cout);
        print_usage(argv[0]);
        return 0;
    }

    if (std::string_view(argv[1]) == "--list")
    {
        adapter_manager.show_adapters(std::cout);
        return 0;
    }

    int adapter_index = -1;
    int run_seconds = kDefaultRunSeconds;
    int slave_id = 1;
    int frames_per_cycle = kDefaultFramesPerCycle;
    int pdo_period_us = kDefaultPdoLoopSleepUs;
    if (argc >= 5 &&
        (!parse_int(argv[4], &frames_per_cycle) ||
         frames_per_cycle <= 0 ||
         static_cast<std::uint32_t>(frames_per_cycle) > kMaxFramesPerCycle))
    {
        std::printf("Invalid frames_per_cycle: %s. Valid range: 1..%u\n", argv[4], kMaxFramesPerCycle);
        return 1;
    }

    if (argc >= 6 &&
        (!parse_int(argv[5], &pdo_period_us) ||
         pdo_period_us <= 0 ||
         pdo_period_us > 1000000))
    {
        std::printf("Invalid pdo_period_us: %s. Valid range: 1..1000000\n", argv[5]);
        return 1;
    }

    if (argc >= 3 && (!parse_int(argv[2], &run_seconds) || run_seconds < 0))
    {
        std::printf("Invalid run_seconds: %s\n", argv[2]);
        return 1;
    }

    if (argc >= 4 && (!parse_int(argv[3], &slave_id) || slave_id <= 0))
    {
        std::printf("Invalid slave_id: %s\n", argv[3]);
        return 1;
    }

    adapter_manager.show_adapters(std::cout);
    if (!select_adapter_by_arg(adapter_manager, argv[1], &adapter_index))
    {
        std::printf("Adapter '%s' was not found.\n", argv[1]);
        return 1;
    }

    std::printf("Opening adapter %d: %s\n", adapter_index, adapter_manager.selected_name().c_str());
    if (!adapter_manager.open_selected_adapter())
    {
        std::printf("Failed to open adapter.\n");
        return 1;
    }

    const int slave_count = ecx_config_init(&ctx);
    if (slave_count <= 0)
    {
        std::printf("No EtherCAT slaves found on this adapter.\n");
        return 1;
    }
    std::printf("Found %d EtherCAT slave(s).\n", slave_count);

    if (slave_id > slave_count)
    {
        std::printf("Slave id %d is out of range. Valid range: 1..%d\n", slave_id, slave_count);
        return 1;
    }

    const int io_size = ecx_config_map_group(&ctx, IOmap, 0);
    std::printf("IO map size: %d bytes\n", io_size);
    expected_wkc = ctx.grouplist[0].outputsWKC * 2 + ctx.grouplist[0].inputsWKC;
    std::printf("WKC: outputs=%u inputs=%u expected=%d\n",
                ctx.grouplist[0].outputsWKC,
                ctx.grouplist[0].inputsWKC,
                expected_wkc);

    ecx_statecheck(&ctx, 0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 2);
    ecx_send_processdata(&ctx);
    ecx_receive_processdata(&ctx, EC_TIMEOUTRET);

    linkx.init(static_cast<std::uint32_t>(slave_id), ctx);
    if (!linkx.set_all_can_channels_enabled(true))
    {
        std::printf("Failed to enable all CAN channels by SDO 0x8001.\n");
        return 1;
    }

    // Enable distributed clock Sync0 so the slave samples PDOs at the requested
    // period. The host loop below targets the same period in microseconds.
    const boolean dc_configured = ecx_configdc(&ctx);
    std::printf("ecx_configdc result=%u\n", static_cast<unsigned>(dc_configured));
    const std::uint32_t sync_cycle_ns = static_cast<std::uint32_t>(pdo_period_us) * 1000;
    print_dc_status(slave_id, "after ecx_configdc");
    ecx_dcsync0(&ctx, static_cast<std::uint16_t>(slave_id), true, sync_cycle_ns, kSyncShiftNs);
    print_dc_status(slave_id, "after ecx_dcsync0 on");

    if (!linkx.start())
    {
        std::printf("Failed to switch slave %d to OP: %s\n", slave_id, linkx_get_error_string(&linkx.raw()));
        return 1;
    }
    std::printf("Slave %d is OP. Running PDO loop with SOEM RT thread, period target=%u us, grouped CAN frames/cycle=%d.\n",
                slave_id, static_cast<unsigned>(pdo_period_us), frames_per_cycle);

    const Clock::time_point run_start = Clock::now();
    PdoThreadConfig pdo_thread_config;
    pdo_thread_config.frames_per_cycle = static_cast<std::uint32_t>(frames_per_cycle);
    pdo_thread_config.pdo_period_us = pdo_period_us;
    pdo_thread_config.run_seconds = run_seconds;
    pdo_thread_config.run_start = run_start;

    OSAL_THREAD_HANDLE pdo_thread_handle = nullptr;
    if (!start_pdo_rt_thread(&pdo_thread_handle, &pdo_thread_config))
    {
        std::printf("Failed to create SOEM RT PDO thread.\n");
        linkx.stop();
        ecx_dcsync0(&ctx, static_cast<std::uint16_t>(slave_id), false, sync_cycle_ns, kSyncShiftNs);
        adapter_manager.close();
        return 1;
    }

    if (run_seconds == 0)
    {
        std::printf("Running continuously. Stop the process to exit.\n");
        int second = 0;
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            print_stats(++second, run_start);
        }
    }

    for (int second = 1; second <= run_seconds; ++second)
    {
        std::this_thread::sleep_until(run_start + std::chrono::seconds(second));
        print_stats(second, run_start);
    }
    pdo_stop_requested.store(true, std::memory_order_release);
    if (!wait_pdo_rt_thread(pdo_thread_handle))
    {
        std::printf("PDO RT thread did not stop within %u ms.\n", kPdoThreadStopTimeoutMs);
        return 2;
    }

    linkx.stop();
    ecx_dcsync0(&ctx, static_cast<std::uint16_t>(slave_id), false, sync_cycle_ns, kSyncShiftNs);
    adapter_manager.close();

    std::printf("Demo stopped.\n");
    return 0;
}
