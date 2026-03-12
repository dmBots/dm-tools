//
// Created by 93094 on 2026/3/12.
//

#include "linkx.h"
#include "soem.h"


void linkx_init(linkx_t* linkx, uint32_t slave_id, struct ecx_context* master)
{
    linkx->slave_id = slave_id;
    linkx->master = master;
    linkx->slave=&master->slavelist[slave_id];
}

static bool linkx_set_state(linkx_t* linkx,ec_state request_state)
{
    if (!linkx->master)
        return false;

    linkx->slave->state=request_state;
    ecx_writestate(linkx->master, linkx->slave_id);

    ecx_readstate(linkx->master);

    if (linkx->slave->state==request_state)
        return true;
    return false;
}

bool linkx_start(linkx_t* linkx)
{
    return linkx_set_state(linkx,EC_STATE_OPERATIONAL);
}

bool linkx_stop(linkx_t* linkx)
{
    return linkx_set_state(linkx,EC_STATE_SAFE_OP);
}

bool linkx_read_baudrate(linkx_t* linkx,uint8_t channel)
{
    if (channel>=LINKX_CAN_CHANNEL_NUM)
        return false;

    /* switch to safe op mode */
    if (!linkx_set_state(linkx,EC_STATE_SAFE_OP))
        return false;

    /* write channel & config_read */
    int config_read=1;
    ecx_SDOwrite(linkx->master, linkx->slave_id, 0x8003, 1, false, 1, &channel,300);
    ecx_SDOwrite(linkx->master, linkx->slave_id, 0x8003, 11, false, 1, &config_read,300);

    /* sdo read index 0x8003 ob */
    int read_size=0;
    uint8_t subitem_cnt=0;
    uint8_t read_bytes[sizeof(can_baudrate_setting_t)]={0};

    /* get subitem count */
    ecx_SDOread(linkx->master, linkx->slave_id, 0x8003, 0, false, &read_size, &subitem_cnt,300);

    if (read_size==0||subitem_cnt==0)
        return false;

    for (int i=1;i<subitem_cnt-1;i++)
    {
        ecx_SDOread(linkx->master, linkx->slave_id, 0x8003, i, false, &read_size, read_bytes+i-1,300);
        if (read_size==0)
            return false;
    }

    memcpy(&linkx->channel_baudrates[channel],read_bytes,sizeof(can_baudrate_setting_t));
    return true;
}

bool linkx_write_baudrate(linkx_t* linkx,uint8_t channel)
{
    if (channel>=LINKX_CAN_CHANNEL_NUM)
        return false;

    /* switch to safe op mode */
    if (!linkx_set_state(linkx,EC_STATE_SAFE_OP))
        return false;

    uint8_t* write_bytes=(uint8_t*)&linkx->channel_baudrates[channel];

    /* sdo read index 0x8003 ob */
    int read_size=0;
    uint8_t subitem_cnt=0;
    uint8_t read_bytes[sizeof(can_baudrate_setting_t)]={0};

    /* get subitem count */
    ecx_SDOread(linkx->master, linkx->slave_id, 0x8002, 0, false, &read_size, &subitem_cnt,300);
    if (read_size==0||subitem_cnt==0)
        return false;

    for (int i=0;i<subitem_cnt;i++)
    {
        ecx_SDOwrite(linkx->master, linkx->slave_id, 0x8002, i+1, false, 1, read_bytes+i,300);
    }

}

bool linkx_switch_can_channel(linkx_t* linkx,uint8_t channel,bool enable)
{
    if (channel>=LINKX_CAN_CHANNEL_NUM)
        return false;
    ecx_SDOwrite(linkx->master, linkx->slave_id, 0x8001, channel+1, false, 1,&enable,300);
}


void linkx_recv_pdos(linkx_t* linkx)
{
    /* receive pdos from slave  pData=master inputs*/
    for (int i=0;i<LINKX_CAN_CHANNEL_NUM;i++)
    {
        memcpy(&linkx->rx_pdos[i],linkx->slave->inputs+i*sizeof(can_rx_pdo_t),sizeof(can_rx_pdo_t));
    }
}


void linkx_send_pdos(linkx_t* linkx)
{
    /* transmit pdos to slave pData=master outputs*/
    for (int i=0;i<LINKX_CAN_CHANNEL_NUM;i++)
    {
        memcpy(linkx->slave->outputs+i*sizeof(can_tx_pdo_t),&linkx->tx_pdos[i],sizeof(can_tx_pdo_t));
    }
}

bool linkx_send_can(linkx_t* linkx,uint8_t channel,uint32_t canid,bool canfd,bool brs,bool ext,bool rtr,uint8_t dlen,uint32_t* data)
{
    if (channel>=LINKX_CAN_CHANNEL_NUM)
        return false;

    linkx->rx_pdos[channel].can_id=canid;
    linkx->rx_pdos[channel].params.canfd=canfd;
    linkx->rx_pdos[channel].params.brs=brs;
    linkx->rx_pdos[channel].params.ext=ext;
    linkx->rx_pdos[channel].params.rtr=rtr;
    linkx->rx_pdos[channel].params.dlen=dlen;
    memcpy(linkx->rx_pdos[channel].data_u32,data,dlen);
    return true;
}

can_tx_pdo_t* linkx_recv_can(linkx_t* linkx,uint8_t channel)
{
    if (channel>=LINKX_CAN_CHANNEL_NUM)
        return NULL;
    return &linkx->tx_pdos[channel];
}

char* linkx_get_error_string(linkx_t* linkx)
{
    if (!linkx->slave)
        return "slave is null";
    return ec_ALstatuscode2string(linkx->slave->ALstatuscode);
}

