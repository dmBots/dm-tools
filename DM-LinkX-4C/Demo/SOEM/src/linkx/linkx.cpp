#include "linkx.h"
#include "soem.h"

#include <cstring>

namespace
{
// Subindex 0x0B is the command latch in the LinkX baud-rate objects. After the
// parameter bytes are written/read, writing 1 tells the module to apply/fetch
// the selected channel configuration.
constexpr std::uint8_t kBaudrateConfigSubindex = 0x0B;
constexpr int kSdoTimeout = 5000;

// CANFD DLC encoding table. DLC 0..8 are linear; 9..15 map to the larger CANFD
// payload sizes.
constexpr std::uint8_t kDlcPayloadSizeTable[] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 12, 16, 20, 24, 32, 48, 64,
};

bool is_valid_linkx(const linkx_t* linkx)
{
    return linkx != nullptr && linkx->master != nullptr && linkx->slave != nullptr;
}

bool is_valid_channel(std::uint8_t channel)
{
    return channel < LINKX_CAN_CHANNEL_NUM;
}

bool write_u8(linkx_t* linkx, std::uint16_t index, std::uint8_t subindex, std::uint8_t value)
{
    if (!is_valid_linkx(linkx))
    {
        return false;
    }

    return ecx_SDOwrite(linkx->master, linkx->slave_id, index, subindex, false,
                        sizeof(value), &value, kSdoTimeout) > 0;
}

bool read_u8(linkx_t* linkx, std::uint16_t index, std::uint8_t subindex, std::uint8_t* value)
{
    if (!is_valid_linkx(linkx) || value == nullptr)
    {
        return false;
    }

    int read_size = sizeof(*value);
    return ecx_SDOread(linkx->master, linkx->slave_id, index, subindex, false,
                       &read_size, value, kSdoTimeout) > 0 && read_size > 0;
}

bool linkx_set_state(linkx_t* linkx, ec_state request_state)
{
    if (!is_valid_linkx(linkx))
    {
        return false;
    }

    linkx->slave->state = request_state;
    ecx_writestate(linkx->master, linkx->slave_id);

    // SOEM state transitions are asynchronous. Wait until the slave reports the
    // requested state so callers can fail fast when the device refuses OP/SAFE_OP.
    const std::uint16_t state = ecx_statecheck(linkx->master, linkx->slave_id, request_state, EC_TIMEOUTSTATE * 2);
    return state == request_state;
}

bool has_output_pdo_buffer(const linkx_t* linkx)
{
    return is_valid_linkx(linkx) && linkx->slave->outputs != nullptr &&
           linkx->slave->Obytes >= sizeof(packet_buf_t);
}

bool has_input_pdo_buffer(const linkx_t* linkx)
{
    return is_valid_linkx(linkx) && linkx->slave->inputs != nullptr &&
           linkx->slave->Ibytes >= sizeof(packet_buf_t);
}
}

std::uint8_t linkx_dlc_to_payload_size(std::uint8_t dlc)
{
    if (dlc > LINKX_CAN_MAX_DLC)
    {
        return 0;
    }

    return kDlcPayloadSizeTable[dlc];
}

bool linkx_payload_size_to_dlc(std::uint8_t payload_size, std::uint8_t* dlc)
{
    if (dlc == nullptr)
    {
        return false;
    }

    for (std::uint8_t i = 0; i <= LINKX_CAN_MAX_DLC; ++i)
    {
        if (kDlcPayloadSizeTable[i] == payload_size)
        {
            *dlc = i;
            return true;
        }
    }

    return false;
}

bool linkx_make_can_frame(can_frame_t* frame,
                          std::uint8_t channel,
                          std::uint32_t can_id,
                          bool canfd,
                          bool brs,
                          bool ext,
                          bool rtr,
                          const std::uint8_t* payload,
                          std::uint8_t payload_size,
                          std::uint32_t timestamp,
                          bool esi)
{
    if (frame == nullptr || !is_valid_channel(channel) || can_id > LINKX_CAN_ID_MASK)
    {
        return false;
    }

    std::uint8_t dlc = 0;
    if (!linkx_payload_size_to_dlc(payload_size, &dlc))
    {
        return false;
    }

    if (payload_size > 0 && payload == nullptr)
    {
        return false;
    }

    std::memset(frame, 0, sizeof(*frame));
    frame->head.can_id = can_id;
    frame->head.esi = esi ? 1 : 0;
    frame->head.rtr = rtr ? 1 : 0;
    frame->head.ext = ext ? 1 : 0;
    frame->head.brs = brs ? 1 : 0;
    frame->head.canfd = canfd ? 1 : 0;
    frame->head.dlc = dlc;
    frame->head.channel = channel;
    frame->head.timestamp = timestamp;

    if (payload_size > 0)
    {
        std::memcpy(frame->payload, payload, payload_size);
    }

    return true;
}

void linkx_packet_clear(packet_buf_t* packet)
{
    if (packet != nullptr)
    {
        std::memset(packet, 0, sizeof(*packet));
    }
}

bool linkx_packet_append_frame(packet_buf_t* packet, const can_frame_t* frame)
{
    if (packet == nullptr || frame == nullptr || frame->head.dlc > LINKX_CAN_MAX_DLC ||
        !is_valid_channel(frame->head.channel))
    {
        return false;
    }

    const std::uint8_t payload_size = linkx_dlc_to_payload_size(frame->head.dlc);
    const std::size_t frame_size = sizeof(can_frame_head_t) + payload_size;
    // nbytes is a packed byte cursor in data_buf. Reject frames that would cross
    // the fixed PDO packet payload.
    if (packet->nbytes + frame_size > LINKX_PACKET_DATA_SIZE)
    {
        return false;
    }

    std::uint8_t* const write_ptr = packet->data_buf + packet->nbytes;
    std::memcpy(write_ptr, &frame->head, sizeof(can_frame_head_t));
    if (payload_size > 0)
    {
        std::memcpy(write_ptr + sizeof(can_frame_head_t), frame->payload, payload_size);
    }

    packet->nbytes = static_cast<std::uint16_t>(packet->nbytes + frame_size);
    return true;
}

bool linkx_packet_read_frame(const packet_buf_t* packet, std::uint16_t* offset, can_frame_t* frame)
{
    if (packet == nullptr || offset == nullptr || frame == nullptr || packet->nbytes > LINKX_PACKET_DATA_SIZE)
    {
        return false;
    }

    if (*offset >= packet->nbytes)
    {
        return false;
    }

    if (*offset + sizeof(can_frame_head_t) > packet->nbytes)
    {
        return false;
    }

    can_frame_head_t head{};
    std::memcpy(&head, packet->data_buf + *offset, sizeof(head));
    if (head.dlc > LINKX_CAN_MAX_DLC || !is_valid_channel(head.channel))
    {
        return false;
    }

    const std::uint8_t payload_size = linkx_dlc_to_payload_size(head.dlc);
    const std::uint16_t next_offset = static_cast<std::uint16_t>(*offset + sizeof(can_frame_head_t) + payload_size);
    // The device may produce a partially updated packet if PDO mapping or timing
    // is wrong; this guard prevents walking beyond nbytes while parsing.
    if (next_offset > packet->nbytes)
    {
        return false;
    }

    std::memset(frame, 0, sizeof(*frame));
    frame->head = head;
    if (payload_size > 0)
    {
        std::memcpy(frame->payload, packet->data_buf + *offset + sizeof(can_frame_head_t), payload_size);
    }

    *offset = next_offset;
    return true;
}

bool linkx_foreach_packet_frame(const packet_buf_t* packet, linkx_can_frame_callback_t callback, void* user_data)
{
    if (packet == nullptr || callback == nullptr || packet->nbytes > LINKX_PACKET_DATA_SIZE)
    {
        return false;
    }

    std::uint16_t offset = 0;
    while (offset < packet->nbytes)
    {
        can_frame_t frame{};
        if (!linkx_packet_read_frame(packet, &offset, &frame))
        {
            return false;
        }
        callback(&frame, user_data);
    }

    return true;
}

void linkx_init(linkx_t* linkx, std::uint32_t slave_id, struct ecx_context* master)
{
    if (linkx == nullptr || master == nullptr)
    {
        return;
    }

    *linkx = {};
    linkx->slave_id = slave_id;
    linkx->master = master;
    linkx->slave = &master->slavelist[slave_id];
}

bool linkx_start(linkx_t* linkx)
{
    return linkx_set_state(linkx, EC_STATE_OPERATIONAL);
}

bool linkx_stop(linkx_t* linkx)
{
    return linkx_set_state(linkx, EC_STATE_SAFE_OP);
}

bool linkx_set_can_channel_enabled(linkx_t* linkx, std::uint8_t channel, bool enable)
{
    if (!is_valid_channel(channel))
    {
        return false;
    }

    return write_u8(linkx, LINKX_OD_CAN_CHANNEL_ENABLED, static_cast<std::uint8_t>(channel + 1), enable ? 1 : 0);
}

bool linkx_set_all_can_channels_enabled(linkx_t* linkx, bool enable)
{
    for (std::uint8_t channel = 0; channel < LINKX_CAN_CHANNEL_NUM; ++channel)
    {
        if (!linkx_set_can_channel_enabled(linkx, channel, enable))
        {
            return false;
        }
    }

    return true;
}

bool linkx_switch_can_channel(linkx_t* linkx, std::uint8_t channel, bool enable)
{
    return linkx_set_can_channel_enabled(linkx, channel, enable);
}

bool linkx_read_baudrate(linkx_t* linkx, std::uint8_t channel)
{
    if (!is_valid_linkx(linkx) || !is_valid_channel(channel))
    {
        return false;
    }

    return linkx_read_baudrate(linkx, channel, &linkx->channel_baudrates[channel]);
}

bool linkx_read_baudrate(linkx_t* linkx, std::uint8_t channel, can_baudrate_setting_t* baudrate)
{
    if (!is_valid_linkx(linkx) || !is_valid_channel(channel) || baudrate == nullptr)
    {
        return false;
    }

    if (!linkx_set_state(linkx, EC_STATE_SAFE_OP))
    {
        return false;
    }

    if (!write_u8(linkx, LINKX_OD_CAN_BAUDRATE_READ, 0x01, channel))
    {
        return false;
    }

    if (!write_u8(linkx, LINKX_OD_CAN_BAUDRATE_READ, kBaudrateConfigSubindex, 1))
    {
        return false;
    }

    can_baudrate_setting_t read_setting{};
    std::uint8_t* const bytes = reinterpret_cast<std::uint8_t*>(&read_setting);
    // The LinkX firmware exposes the baud-rate structure as byte subindexes.
    // Keep this loop aligned with can_baudrate_setting_t.
    for (std::uint8_t subindex = 0x01; subindex <= 0x0A; ++subindex)
    {
        if (!read_u8(linkx, LINKX_OD_CAN_BAUDRATE_READ, subindex, bytes + subindex - 1))
        {
            return false;
        }
    }

    *baudrate = read_setting;
    linkx->channel_baudrates[channel] = read_setting;
    return true;
}

bool linkx_write_baudrate(linkx_t* linkx, std::uint8_t channel)
{
    if (!is_valid_linkx(linkx) || !is_valid_channel(channel))
    {
        return false;
    }

    return linkx_write_baudrate(linkx, channel, linkx->channel_baudrates[channel]);
}

bool linkx_write_baudrate(linkx_t* linkx, std::uint8_t channel, const can_baudrate_setting_t& baudrate)
{
    if (!is_valid_linkx(linkx) || !is_valid_channel(channel))
    {
        return false;
    }

    if (!linkx_set_state(linkx, EC_STATE_SAFE_OP))
    {
        return false;
    }

    can_baudrate_setting_t write_setting = baudrate;
    write_setting.channel = channel;
    const std::uint8_t* const bytes = reinterpret_cast<const std::uint8_t*>(&write_setting);
    for (std::uint8_t subindex = 0x01; subindex <= 0x0A; ++subindex)
    {
        if (!write_u8(linkx, LINKX_OD_CAN_BAUDRATE_WRITE, subindex, bytes[subindex - 1]))
        {
            return false;
        }
    }

    if (!write_u8(linkx, LINKX_OD_CAN_BAUDRATE_WRITE, kBaudrateConfigSubindex, 1))
    {
        return false;
    }

    linkx->channel_baudrates[channel] = write_setting;
    return true;
}

void linkx_clear_output_packet(linkx_t* linkx)
{
    if (linkx != nullptr)
    {
        linkx_packet_clear(&linkx->output_packet);
    }
}

bool linkx_append_output_frame(linkx_t* linkx, const can_frame_t* frame)
{
    if (linkx == nullptr)
    {
        return false;
    }

    return linkx_packet_append_frame(&linkx->output_packet, frame);
}

bool linkx_append_output_can_frame(linkx_t* linkx,
                                   std::uint8_t channel,
                                   std::uint32_t can_id,
                                   bool canfd,
                                   bool brs,
                                   bool ext,
                                   bool rtr,
                                   const std::uint8_t* payload,
                                   std::uint8_t payload_size,
                                   std::uint32_t timestamp,
                                   bool esi)
{
    can_frame_t frame{};
    return linkx_make_can_frame(&frame, channel, can_id, canfd, brs, ext, rtr, payload, payload_size, timestamp, esi) &&
           linkx_append_output_frame(linkx, &frame);
}

bool linkx_write_output_packet(linkx_t* linkx)
{
    if (!has_output_pdo_buffer(linkx))
    {
        return false;
    }

    // SOEM maps slave outputs directly into the process image. Copy the whole
    // fixed-size packet so stale bytes from the previous cycle are cleared too.
    std::memcpy(linkx->slave->outputs, &linkx->output_packet, sizeof(packet_buf_t));
    return true;
}

bool linkx_read_input_packet(linkx_t* linkx)
{
    if (!has_input_pdo_buffer(linkx))
    {
        return false;
    }

    std::memcpy(&linkx->input_packet, linkx->slave->inputs, sizeof(packet_buf_t));
    return linkx->input_packet.nbytes <= LINKX_PACKET_DATA_SIZE;
}

bool linkx_foreach_input_frame(const linkx_t* linkx, linkx_can_frame_callback_t callback, void* user_data)
{
    if (linkx == nullptr)
    {
        return false;
    }

    return linkx_foreach_packet_frame(&linkx->input_packet, callback, user_data);
}

int linkx_exchange_processdata(linkx_t* linkx, int timeout)
{
    if (!is_valid_linkx(linkx) || !linkx_write_output_packet(linkx))
    {
        return EC_ERROR;
    }

    ecx_send_processdata(linkx->master);
    const int wkc = ecx_receive_processdata(linkx->master, timeout);
    if (!linkx_read_input_packet(linkx))
    {
        return EC_ERROR;
    }

    return wkc;
}

const char* linkx_get_error_string(linkx_t* linkx)
{
    if (linkx == nullptr || linkx->slave == nullptr)
    {
        return "slave is null";
    }
    return ec_ALstatuscode2string(linkx->slave->ALstatuscode);
}

void LinkxDevice::init(std::uint32_t slave_id, struct ecx_context& master)
{
    linkx_init(&handle_, slave_id, &master);
}

bool LinkxDevice::start()
{
    return linkx_start(&handle_);
}

bool LinkxDevice::stop()
{
    return linkx_stop(&handle_);
}

bool LinkxDevice::set_can_channel_enabled(std::uint8_t channel, bool enable)
{
    return linkx_set_can_channel_enabled(&handle_, channel, enable);
}

bool LinkxDevice::set_all_can_channels_enabled(bool enable)
{
    return linkx_set_all_can_channels_enabled(&handle_, enable);
}

bool LinkxDevice::read_baudrate(std::uint8_t channel, can_baudrate_setting_t& baudrate)
{
    return linkx_read_baudrate(&handle_, channel, &baudrate);
}

bool LinkxDevice::write_baudrate(std::uint8_t channel, const can_baudrate_setting_t& baudrate)
{
    return linkx_write_baudrate(&handle_, channel, baudrate);
}

void LinkxDevice::clear_output_packet()
{
    linkx_clear_output_packet(&handle_);
}

bool LinkxDevice::append_output_frame(const can_frame_t& frame)
{
    return linkx_append_output_frame(&handle_, &frame);
}

bool LinkxDevice::append_output_can_frame(std::uint8_t channel,
                                          std::uint32_t can_id,
                                          bool canfd,
                                          bool brs,
                                          bool ext,
                                          bool rtr,
                                          const std::uint8_t* payload,
                                          std::uint8_t payload_size,
                                          std::uint32_t timestamp,
                                          bool esi)
{
    return linkx_append_output_can_frame(&handle_, channel, can_id, canfd, brs, ext, rtr,
                                         payload, payload_size, timestamp, esi);
}

bool LinkxDevice::write_output_packet()
{
    return linkx_write_output_packet(&handle_);
}

bool LinkxDevice::read_input_packet()
{
    return linkx_read_input_packet(&handle_);
}

bool LinkxDevice::foreach_input_frame(linkx_can_frame_callback_t callback, void* user_data) const
{
    return linkx_foreach_input_frame(&handle_, callback, user_data);
}

int LinkxDevice::exchange_processdata(int timeout)
{
    return linkx_exchange_processdata(&handle_, timeout);
}

const packet_buf_t& LinkxDevice::input_packet() const
{
    return handle_.input_packet;
}

packet_buf_t& LinkxDevice::output_packet()
{
    return handle_.output_packet;
}

const linkx_t& LinkxDevice::raw() const
{
    return handle_;
}

linkx_t& LinkxDevice::raw()
{
    return handle_;
}
