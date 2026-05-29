#ifndef LINKX_SOEM_DEMO_LINKX_H
#define LINKX_SOEM_DEMO_LINKX_H

#include <cstddef>
#include <cstdint>

#define LINKX_CAN_CHANNEL_NUM 4

// DM-LinkX object dictionary indexes used by this wrapper.
// TxPDO buffers are slave-to-master CAN receive data; RxPDO buffers are
// master-to-slave CAN transmit data. The 0x800x objects are configured by SDO.
constexpr std::uint16_t LINKX_OD_CAN_RECV_TXPDO_BUFFER0 = 0x6001;
constexpr std::uint16_t LINKX_OD_CAN_RECV_TXPDO_BUFFER1 = 0x6002;
constexpr std::uint16_t LINKX_OD_CAN_SEND_RXPDO_BUFFER0 = 0x7001;
constexpr std::uint16_t LINKX_OD_CAN_SEND_RXPDO_BUFFER1 = 0x7002;
constexpr std::uint16_t LINKX_OD_CAN_CHANNEL_ENABLED = 0x8001;
constexpr std::uint16_t LINKX_OD_CAN_BAUDRATE_WRITE = 0x8002;
constexpr std::uint16_t LINKX_OD_CAN_BAUDRATE_READ = 0x8003;

// One EtherCAT PDO packet contains a 16-bit byte count followed by serialized
// CAN/CANFD frames. Each frame stores a compact 10-byte header and only the
// payload bytes implied by its DLC.
constexpr std::size_t LINKX_PDO_BUFFER_SIZE = 255;
constexpr std::size_t LINKX_PACKET_SIZE = LINKX_PDO_BUFFER_SIZE * 2;
constexpr std::size_t LINKX_PACKET_DATA_SIZE = LINKX_PACKET_SIZE - sizeof(std::uint16_t);
constexpr std::size_t LINKX_CAN_MAX_PAYLOAD_SIZE = 64;
constexpr std::uint8_t LINKX_CAN_MAX_DLC = 15;
constexpr std::uint32_t LINKX_CAN_ID_MASK = 0x1FFFFFFF;

#pragma pack(push, 1)
// Baud-rate parameters are exchanged one byte at a time through SDO objects
// 0x8002/0x8003. Keep this layout stable because the field order is the wire
// format expected by the LinkX slave.
struct can_baudrate_setting_t
{
    std::uint8_t channel;
    std::uint8_t enable_canfd;
    std::uint8_t nominal_prescaler;
    std::uint8_t nominal_seg1;
    std::uint8_t nominal_seg2;
    std::uint8_t nominal_sjw;
    std::uint8_t data_prescaler;
    std::uint8_t data_seg1;
    std::uint8_t data_seg2;
    std::uint8_t data_sjw;
};

// Compact CAN/CANFD frame header stored inside a PDO packet. The bit-field
// layout mirrors the LinkX module protocol, so the struct is packed.
struct can_frame_head_t
{
    std::uint32_t can_id : 29;
    std::uint32_t esi : 1;
    std::uint32_t rtr : 1;
    std::uint32_t ext : 1;

    std::uint8_t brs : 1;
    std::uint8_t canfd : 1;
    std::uint8_t reserved : 2;
    std::uint8_t dlc : 4;
    std::uint8_t channel;
    std::uint32_t timestamp;
};

// In memory this reserves the maximum CANFD payload. When serialized into a
// packet only linkx_dlc_to_payload_size(head.dlc) bytes are copied.
struct can_frame_t
{
    can_frame_head_t head;
    std::uint8_t payload[LINKX_CAN_MAX_PAYLOAD_SIZE];
};

// PDO packet buffer. nbytes is the number of valid bytes in data_buf, not the
// size of the whole struct.
struct packet_buf_t
{
    std::uint16_t nbytes;
    std::uint8_t data_buf[LINKX_PACKET_DATA_SIZE];
};
#pragma pack(pop)

static_assert(sizeof(can_baudrate_setting_t) == 10);
static_assert(sizeof(can_frame_head_t) == 10);
static_assert(sizeof(can_frame_t) == sizeof(can_frame_head_t) + LINKX_CAN_MAX_PAYLOAD_SIZE);
static_assert(sizeof(packet_buf_t) == LINKX_PACKET_SIZE);

// Plain C handle for one LinkX EtherCAT slave. It borrows the SOEM context and
// slave entry; the caller owns SOEM initialization and adapter lifetime.
struct linkx_t
{
    std::uint32_t slave_id = 0;
    can_baudrate_setting_t channel_baudrates[LINKX_CAN_CHANNEL_NUM]{};
    packet_buf_t output_packet{};
    packet_buf_t input_packet{};
    struct ecx_context* master = nullptr;
    struct ec_slave* slave = nullptr;
};

using linkx_can_frame_callback_t = void (*)(const can_frame_t* frame, void* user_data);

// CAN/CANFD DLC helpers. Only valid CAN/CANFD payload sizes map back to a DLC.
std::uint8_t linkx_dlc_to_payload_size(std::uint8_t dlc);
bool linkx_payload_size_to_dlc(std::uint8_t payload_size, std::uint8_t* dlc);

// Build a validated frame in host memory. This does not touch EtherCAT; append
// it to output_packet and exchange process data to send it.
bool linkx_make_can_frame(can_frame_t* frame,
                          std::uint8_t channel,
                          std::uint32_t can_id,
                          bool canfd,
                          bool brs,
                          bool ext,
                          bool rtr,
                          const std::uint8_t* payload,
                          std::uint8_t payload_size,
                          std::uint32_t timestamp = 0,
                          bool esi = false);

// Local packet helpers used before/after PDO exchange.
void linkx_packet_clear(packet_buf_t* packet);
bool linkx_packet_append_frame(packet_buf_t* packet, const can_frame_t* frame);
bool linkx_packet_read_frame(const packet_buf_t* packet, std::uint16_t* offset, can_frame_t* frame);
bool linkx_foreach_packet_frame(const packet_buf_t* packet, linkx_can_frame_callback_t callback, void* user_data);

// Bind the LinkX handle to an already discovered SOEM slave.
void linkx_init(linkx_t* linkx, std::uint32_t slave_id, struct ecx_context* master);

// Move the slave between OP and SAFE_OP. Configuration SDOs should be done in
// SAFE_OP; cyclic PDO traffic runs in OP.
bool linkx_start(linkx_t* linkx);
bool linkx_stop(linkx_t* linkx);

// SDO configuration helpers. Channels are zero-based in this API.
bool linkx_set_can_channel_enabled(linkx_t* linkx, std::uint8_t channel, bool enable);
bool linkx_set_all_can_channels_enabled(linkx_t* linkx, bool enable);
bool linkx_switch_can_channel(linkx_t* linkx, std::uint8_t channel, bool enable);

// Baud-rate read/write helpers switch the slave to SAFE_OP. Call linkx_start()
// again before resuming cyclic PDO traffic.
bool linkx_read_baudrate(linkx_t* linkx, std::uint8_t channel);
bool linkx_read_baudrate(linkx_t* linkx, std::uint8_t channel, can_baudrate_setting_t* baudrate);
bool linkx_write_baudrate(linkx_t* linkx, std::uint8_t channel);
bool linkx_write_baudrate(linkx_t* linkx, std::uint8_t channel, const can_baudrate_setting_t& baudrate);

// PDO output/input helpers. Fill output_packet, exchange process data, then
// iterate input_packet to consume CAN frames received by the LinkX module.
void linkx_clear_output_packet(linkx_t* linkx);
bool linkx_append_output_frame(linkx_t* linkx, const can_frame_t* frame);
bool linkx_append_output_can_frame(linkx_t* linkx,
                                   std::uint8_t channel,
                                   std::uint32_t can_id,
                                   bool canfd,
                                   bool brs,
                                   bool ext,
                                   bool rtr,
                                   const std::uint8_t* payload,
                                   std::uint8_t payload_size,
                                   std::uint32_t timestamp = 0,
                                   bool esi = false);
bool linkx_write_output_packet(linkx_t* linkx);
bool linkx_read_input_packet(linkx_t* linkx);
bool linkx_foreach_input_frame(const linkx_t* linkx, linkx_can_frame_callback_t callback, void* user_data);
int linkx_exchange_processdata(linkx_t* linkx, int timeout);

const char* linkx_get_error_string(linkx_t* linkx);

// Small C++ convenience wrapper around linkx_t. It keeps the same lifetime
// rules as the C API: the SOEM context and adapter must outlive the device.
class LinkxDevice
{
public:
    void init(std::uint32_t slave_id, struct ecx_context& master);
    bool start();
    bool stop();

    bool set_can_channel_enabled(std::uint8_t channel, bool enable);
    bool set_all_can_channels_enabled(bool enable);

    bool read_baudrate(std::uint8_t channel, can_baudrate_setting_t& baudrate);
    bool write_baudrate(std::uint8_t channel, const can_baudrate_setting_t& baudrate);

    void clear_output_packet();
    bool append_output_frame(const can_frame_t& frame);
    bool append_output_can_frame(std::uint8_t channel,
                                 std::uint32_t can_id,
                                 bool canfd,
                                 bool brs,
                                 bool ext,
                                 bool rtr,
                                 const std::uint8_t* payload,
                                 std::uint8_t payload_size,
                                 std::uint32_t timestamp = 0,
                                 bool esi = false);
    bool write_output_packet();
    bool read_input_packet();
    bool foreach_input_frame(linkx_can_frame_callback_t callback, void* user_data) const;
    int exchange_processdata(int timeout);

    [[nodiscard]] const packet_buf_t& input_packet() const;
    [[nodiscard]] packet_buf_t& output_packet();
    [[nodiscard]] const linkx_t& raw() const;
    [[nodiscard]] linkx_t& raw();

private:
    linkx_t handle_{};
};

#endif // LINKX_SOEM_DEMO_LINKX_H
