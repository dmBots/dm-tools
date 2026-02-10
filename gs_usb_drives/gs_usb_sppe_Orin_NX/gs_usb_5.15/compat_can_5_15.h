#pragma once

#include <linux/version.h>
#include <linux/can/dev.h>
#include <linux/ethtool.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(6,1,0)

/* =========================================================
 * RX OFFLOAD — Jetson 5.15 最终降级
 * ========================================================= */

/*
 * 6.x:
 *   can_rx_offload_queue_timestamp()
 *
 * Jetson 5.15-tegra:
 *   rx-offload API 与 mainline 不兼容
 *   → 直接退化到 netif_rx()
 */
static inline int
can_rx_offload_queue_timestamp(struct can_rx_offload *offload,
                               struct sk_buff *skb,
                               ktime_t ts)
{
    skb->tstamp = ts;
    netif_rx(skb);
    return 0;
}

/*
 * Jetson 5.15:
 *   rx-offload echo skb 不支持
 *   → 关闭 echo 回收
 */
static inline unsigned int
can_rx_offload_get_echo_skb_queue_timestamp(
        struct can_rx_offload *offload,
        unsigned int idx,
        u32 ts,
        struct sk_buff **skb)
{
    if (skb)
        *skb = NULL;
    return 0;
}

static inline unsigned int
can_rx_offload_get_echo_skb_queue_tail(
        struct can_rx_offload *offload,
        unsigned int idx,
        struct sk_buff **skb)
{
    if (skb)
        *skb = NULL;
    return 0;
}

/* =========================================================
 * DROP 统计
 * ========================================================= */

static inline bool
can_dev_dropped_skb(struct net_device *dev, struct sk_buff *skb)
{
    return false;
}

/* =========================================================
 * HW TIMESTAMP / ETHTOOL
 * ========================================================= */

static inline int
can_eth_ioctl_hwts(struct net_device *dev,
                   struct ifreq *ifr,
                   int cmd)
{
    return -EOPNOTSUPP;
}

static inline int
can_ethtool_op_get_ts_info_hwts(struct net_device *dev,
                               struct ethtool_ts_info *info)
{
    return ethtool_op_get_ts_info(dev, info);
}

#endif /* < 6.1 */
