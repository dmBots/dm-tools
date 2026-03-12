//
// Created by 93094 on 2026/3/12.
//

#ifndef LINKX_SOEM_DEMO_LINKX_H
#define LINKX_SOEM_DEMO_LINKX_H

#include <stdint.h>
#include <stdbool.h>

#define LINKX_CAN_CHANNEL_NUM 4

#pragma pack(push, 1)
typedef struct
{
    uint8_t channel;
    uint8_t enable_canfd;
    uint8_t nominal_prescaler;
    uint8_t nominal_seg1;
    uint8_t nominal_seg2;
    uint8_t nominal_sjw;
    uint8_t data_prescaler;
    uint8_t data_seg1;
    uint8_t data_seg2;
    uint8_t data_sjw;

}can_baudrate_setting_t;

typedef struct
{
    uint16_t ext:1;
    uint16_t rtr:1;
    uint16_t canfd:1;
    uint16_t brs:1;
    uint16_t reserved:4;
    uint16_t dlen:8;
}can_pdo_param_t;

typedef struct
{
    uint32_t can_id;
    can_pdo_param_t params;
    uint32_t data_u32[16];
}can_rx_pdo_t;

typedef struct
{
    uint32_t can_id;
    can_pdo_param_t params;
    uint64_t timestamp;
    uint32_t data_u32[16];

}can_tx_pdo_t;


#pragma pack(pop)

typedef struct
{
    uint32_t slave_id;

    can_baudrate_setting_t channel_baudrates[LINKX_CAN_CHANNEL_NUM];

    can_tx_pdo_t tx_pdos[LINKX_CAN_CHANNEL_NUM];
    can_rx_pdo_t rx_pdos[LINKX_CAN_CHANNEL_NUM];

    struct ecx_context* master;
    struct ec_slave* slave;

}linkx_t;


void linkx_init(linkx_t* linkx, uint32_t slave_id, struct ecx_context* master);
bool linkx_start(linkx_t* linkx);
bool linkx_stop(linkx_t* linkx);
bool linkx_read_baudrate(linkx_t* linkx,uint8_t channel);
bool linkx_write_baudrate(linkx_t* linkx,uint8_t channel);
bool linkx_switch_can_channel(linkx_t* linkx,uint8_t channel,bool enable);
void linkx_recv_pdos(linkx_t* linkx);
void linkx_send_pdos(linkx_t* linkx);
bool linkx_send_can(linkx_t* linkx,uint8_t channel,uint32_t canid,bool canfd,bool brs,bool ext,bool rtr,uint8_t dlen,uint32_t* data);
can_tx_pdo_t* linkx_recv_can(linkx_t* linkx,uint8_t channel);
char* linkx_get_error_string(linkx_t* linkx);
#endif //LINKX_SOEM_DEMO_LINKX_H