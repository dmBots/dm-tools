/* compat_5_10.h */
#pragma once
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,15,0)

/* ========= CAN RX offload ========= */
#define can_rx_offload_queue_timestamp(offload, skb, ts) \
        can_rx_offload_queue_tail(offload, skb)

// #define can_rx_offload_get_echo_skb_queue_timestamp(offload, echo_id, ts, skb) \
//         // can_rx_offload_get_echo_skb_queue_tail(offload, echo_id, skb)    /*原始值*/
//         can_rx_offload_get_echo_skb(offload, echo_id, skb)                  /*新增值*/

#define can_rx_offload_get_echo_skb_queue_timestamp(offload, echo_id, ts, skb) \
        can_rx_offload_get_echo_skb(offload, echo_id, skb)

#define can_rx_offload_irq_finish(offload) do { } while (0)

/* ========= CAN FD ========= */
#define can_fd_len2dlc(len) can_len2dlc(len)
#define can_fd_dlc2len(dlc) can_dlc2len(dlc)

/* ========= CAN CC (不存在) ========= */
#define can_get_cc_dlc(cf, ctrlmode) ((cf)->can_dlc)
#define can_frame_set_cc_len(cf, dlc, ctrlmode) do { } while (0)
#define CAN_CTRLMODE_CC_LEN8_DLC 0

/* ========= CAN echo ========= */
#define can_dev_dropped_skb(netdev, skb) 0

/* ========= ethtool ========= */
struct kernel_ethtool_ts_info;

static inline int can_ethtool_op_get_ts_info_hwts(
        struct net_device *dev,
        struct kernel_ethtool_ts_info *info)
{
        return ethtool_op_get_ts_info(dev, (void *)info);
}

/* ========= CAN ioctl ========= */
#define can_eth_ioctl_hwts(netdev, ifr, cmd) (-EOPNOTSUPP)

#endif
