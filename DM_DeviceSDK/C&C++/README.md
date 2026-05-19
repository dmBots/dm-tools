# DM_Device_SDK 使用教程

## 目录

1. [概述](#概述)
2. [环境准备](#环境准备)
3. [SDK 基础概念](#sdk-基础概念)
4. [快速开始](#快速开始)
5. [API 详细说明](#api-详细说明)
6. [完整示例](#完整示例)
7. [常见问题](#常见问题)

---

## 概述

DM_Device_SDK 是用于控制 CAN/CAN FD 设备的软件开发工具包，支持以下设备型号：

| 设备类型 | 枚举值 | 说明 |
|---------|--------|------|
| USB2CANFD | `USB2CANFD` | 单通道 CAN FD 设备 |
| USB2CANFD_DUAL | `USB2CANFD_DUAL` | 双通道 CAN FD 设备 |
| LINKX4C | `LINKX4C` | 四通道 CAN 设备 |

**主要功能：**

- 设备发现与管理
- CAN 2.0 / CAN FD 数据收发
- 波特率灵活配置（支持标准参数与详细寄存器配置）
- 多种数据发送模式（单包发送、队列发送、定时发送）
- 回调机制（接收、发送、错误）

---

## 环境准备

### 支持的平台

- **Linux**（Ubuntu 22.04+）
- **Windows**（Windows 10/11）
- **MacOS**

## SDK 基础概念

### 核心数据结构

#### 1. 上下文（Context）

```C
struct dmcan_context;
```

- 所有操作的入口，管理设备列表
- 通过 `dmcan_context_create` 创建，`dmcan_context_destroy` 销毁

#### 2. 设备句柄（Device Handle）

```c
struct dmcan_device_handle;
```

- 代表一个物理设备
- 通过 `dmcan_device_get` 获取
- 所有设备操作都需要此句柄

#### 3. CAN 帧结构

```c
typedef struct {
    usb_rx_frame_head_t head;  // 帧头信息
    uint8_t payload[64];        // 数据负载（最大64字节）
} usb_rx_frame_t;

typedef struct {
    uint32_t can_id:29;    // CAN ID
    uint32_t esi:1;        // 错误状态指示
    uint32_t ext:1;        // 标准帧/扩展帧
    uint32_t rtr:1;        // 数据帧/远程帧
    uint64_t time_stamp;   // 时间戳
    uint8_t  channel;      // 通道号
    uint8_t  canfd:1;      // CAN/CAN FD
    uint8_t  dir:1;        // 发送/接收
    uint8_t  brs:1;        // 波特率切换
    uint8_t  ack:1;        // 应答标志
    uint8_t  dlc:4;        // 数据长度码
    uint16_t reserved;     // 保留
} usb_rx_frame_head_t;
```

#### 4. 波特率配置

**标准配置（推荐）：**

```c
typedef struct {
    uint8_t  channel;         // 通道号
    bool     canfd;           // 是否启用 CAN FD
    uint32_t can_baudrate;    // 仲裁域波特率
    uint32_t canfd_baudrate;  // 数据域波特率
    float    can_sp;          // 仲裁域采样点
    float    canfd_sp;        // 数据域采样点
} dmcan_channel_can_info_t;
```

**详细配置（高级用户）：**

```c
typedef struct {
    uint8_t channel;
    uint8_t can_fd;
    uint8_t can_seg1, can_seg2, can_sjw, can_prescaler;
    uint8_t canfd_seg1, canfd_seg2, canfd_sjw, canfd_prescaler;
} dmcan_ch_can_config_t;
```



### 回调函数类型

```c
typedef void (*dev_recv_callback)(dmcan_device_handle*, usb_rx_frame_t*);  // 接收回调
typedef void (*dev_sent_callback)(dmcan_device_handle*, usb_rx_frame_t*);  // 发送回调
typedef void (*dev_err_callback)(dmcan_device_handle*, usb_rx_frame_t*);   // 错误回调
```



------

## 快速开始

### 最小示例：发送并接收一帧数据

```c++
#include <iostream>
#include "dmcan.h"

// 全局设备句柄
dmcan_device_handle* g_device = nullptr;

// 接收回调
void on_recv(dmcan_device_handle* handle, usb_rx_frame_t* frame) {
    printf("收到帧: ID=0x%X, 数据长度=%d\n", 
           frame->head.can_id, 
           dmcan_utils_get_dlc_from_len(frame->head.dlc));
}

int main() {
    // 1. 创建上下文
    dmcan_context* ctx = nullptr;
    dmcan_context_create(&ctx);
    
    // 2. 查找设备
    int count = dmcan_find_devices(ctx);
    if (count == 0) {
        printf("未找到设备\n");
        return -1;
    }
    
    // 3. 获取并打开第一个设备
    dmcan_device_get(ctx, &g_device, 0);
    dmcan_device_open(g_device);
    
    // 4. 使能通道0
    dmcan_device_enable_channel(g_device, 0);
    
    // 5. 配置波特率 (500kbps CAN FD)
    dmcan_channel_can_info_t info = {0};
    dmcan_device_get_channel_baudrate(g_device, 0, &info);
    info.canfd = true;
    info.can_baudrate = 500000;
    info.canfd_baudrate = 2000000;
    dmcan_device_set_channel_baudrate(g_device, 0, info);
    
    // 6. 注册回调
    dmcan_device_hook_recv_callback(g_device, on_recv);
    
    // 7. 发送数据
    uint8_t data[] = "Hello CAN!";
    dmcan_device_send_can(g_device, 0, 0x123, true, false, false, true, 10, data);
    
    // 8. 等待接收
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // 9. 清理
    dmcan_context_destroy(ctx);
    return 0;
}
```



------

## API 详细说明

### 上下文管理

#### `dmcan_context_create`

```c
void dmcan_context_create(struct dmcan_context** ctx);
```

创建 SDK 上下文。**必须第一个调用**。



#### `dmcan_context_destroy`

```c
void dmcan_context_destroy(struct dmcan_context* ctx);
```

销毁上下文，自动关闭所有已打开的设备。



#### `dmcan_print_version`

```c
void dmcan_print_version(struct dmcan_context* ctx);
```

打印 SDK 版本号到标准输出。



------

### 设备发现

#### `dmcan_find_devices`

```c
int dmcan_find_devices(struct dmcan_context* ctx);
```

查找所有已连接的 DM 设备，返回设备数量。



#### `dmcan_find_devices_with_type`

```c
int dmcan_find_devices_with_type(struct dmcan_context* ctx, dmcan_device_type_t type);
```

查找指定类型的设备。



**示例：仅查找双通道设备**

```c++
int count = dmcan_find_devices_with_type(ctx, USB2CANFD_DUAL);
```



#### `dmcan_show_all_devices`

```c
void dmcan_show_all_devices(struct dmcan_context* ctx);
```

打印所有已发现设备的详细信息（序列号、类型等）。



------

### 设备管理

#### `dmcan_device_get`

```c
bool dmcan_device_get(struct dmcan_context* ctx, 
                      struct dmcan_device_handle** dev_handle, 
                      int index);
```

获取指定索引的设备句柄。

**参数：**

- `index`：设备索引（0 到 count-1）
- 返回 `true` 表示成功



#### `dmcan_device_open` / `dmcan_device_close`

```c
bool dmcan_device_open(struct dmcan_device_handle* dev_handle);
void dmcan_device_close(struct dmcan_device_handle* dev_handle);
```

打开/关闭设备。打开成功后才能进行数据收发。



#### `dmcan_device_print_version`

```c
void dmcan_device_print_version(struct dmcan_device_handle* dev_handle);
```

打印设备固件版本。



------

### 通道管理

#### `dmcan_device_enable_channel`

```c
bool dmcan_device_enable_channel(struct dmcan_device_handle* dev_handle, 
                                 uint8_t channel);
```

使能指定 CAN 通道。

**注意：**

- 单通道设备仅支持通道 0
- 双通道设备支持通道 0 和 1
- 四通道设备支持通道 0~3



#### `dmcan_device_disable_channel`

```c
bool dmcan_device_disable_channel(struct dmcan_device_handle* dev_handle, 
                                  uint8_t channel);
```

禁能指定通道。



------

### 波特率配置

#### 标准配置方式（推荐）

**获取当前配置：**

```c
bool dmcan_device_get_channel_baudrate(
    struct dmcan_device_handle* dev_handle,
    uint8_t channel,
    dmcan_channel_can_info_t* baud_info
);
```



**设置新配置：**

```c
bool dmcan_device_set_channel_baudrate(
    struct dmcan_device_handle* dev_handle,
    uint8_t channel,
    dmcan_channel_can_info_t baud_info
);
```



**常用波特率示例：**

```c
dmcan_channel_can_info_t info = {0};
info.channel = 0;

// CAN 2.0 @ 500kbps
info.canfd = false;
info.can_baudrate = 500000;
info.can_sp = 0.75;

// CAN FD @ 500k/2M
info.canfd = true;
info.can_baudrate = 500000;
info.canfd_baudrate = 2000000;
info.can_sp = 0.75;
info.canfd_sp = 0.75;
```



#### 详细配置方式（高级）

```c
bool dmcan_device_get_channel_baudrate_details(
    struct dmcan_device_handle* dev_handle,
    uint8_t channel,
    dmcan_ch_can_config_t* config
);

bool dmcan_device_set_channel_baudrate_details(
    struct dmcan_device_handle* dev_handle,
    uint8_t channel,
    dmcan_ch_can_config_t config
);
```



**示例：手动配置寄存器参数**

```c++
dmcan_ch_can_config_t config = {0};
config.channel = 0;
config.can_fd = 1;
config.can_prescaler = 4;
config.can_seg1 = 13;
config.can_seg2 = 2;
config.can_sjw = 1;
// ... 设置 CAN FD 参数
dmcan_device_set_channel_baudrate_details(dev, 0, config);
```



------

### 回调函数注册

#### 接收回调

```c
void dmcan_device_hook_recv_callback(
    struct dmcan_device_handle* dev_handle,
    dev_recv_callback callback
);
```

当接收到 CAN 帧时触发。回调在**接收线程**中执行，请勿进行耗时操作。



#### 发送回调

```c
void dmcan_device_hook_sent_callback(
    struct dmcan_device_handle* dev_handle,
    dev_sent_callback callback
);
```

当 CAN 帧发送完成时触发。



#### 错误回调

```c
void dmcan_device_hook_err_callback(
    struct dmcan_device_handle* dev_handle,
    dev_err_callback callback
);
```

当发生 CAN 错误时触发（如总线错误、仲裁丢失等）。



**完整回调示例：**

```c++
void on_recv(dmcan_device_handle* h, usb_rx_frame_t* f) {
    printf("[RX] CH%d ID=0x%X ", f->head.channel, f->head.can_id);
    if (f->head.canfd) printf("[FD] ");
    if (f->head.ext) printf("[EXT] ");
    printf("DLC=%d DATA=", dmcan_utils_get_dlc_from_len(f->head.dlc));
    for (int i = 0; i < dmcan_utils_get_dlc_from_len(f->head.dlc); i++)
        printf("%02X ", f->payload[i]);
    printf("\n");
}

void on_sent(dmcan_device_handle* h, usb_rx_frame_t* f) {
    printf("[TX] CH%d ID=0x%X\n", f->head.channel, f->head.can_id);
}

void on_error(dmcan_device_handle* h, usb_rx_frame_t* f) {
    printf("[ERR] CH%d\n", f->head.channel);
}
```



------

### 数据发送

#### 单包发送

```c
bool dmcan_device_send_can(
    struct dmcan_device_handle* dev_handle,
    uint8_t  ch,        // 通道号
    uint32_t can_id,    // CAN ID
    bool     canfd,     // 是否 CAN FD
    bool     ext,       // 是否扩展帧
    bool     rtr,       // 是否远程帧
    bool     brs,       // 是否波特率切换（CAN FD）
    uint8_t  dlen,      // 数据长度
    uint8_t* payload    // 数据指针
);
```



**发送示例：**

```c
// CAN 2.0 标准数据帧
uint8_t data1[] = {0x11, 0x22, 0x33, 0x44};
dmcan_device_send_can(dev, 0, 0x100, false, false, false, false, 4, data1);

// CAN FD 扩展帧
uint8_t data2[64] = {0};
dmcan_device_send_can(dev, 0, 0x1ABCDEF, true, true, false, true, 64, data2);

// 远程帧
dmcan_device_send_can(dev, 0, 0x200, false, false, true, false, 0, nullptr);
```



#### 定时/批量发送

```c
bool dmcan_device_send_can_details(
    struct dmcan_device_handle* dev_handle,
    uint8_t  ch,
    uint32_t interval_ms,   // 发送间隔（毫秒）
    uint16_t step_id,       // ID 步进值
    uint32_t stop_id,       // 停止 ID
    int      send_times,    // 发送次数
    uint32_t can_id,
    bool     canfd, bool ext, bool rtr, bool brs,
    bool     id_inc,        // 是否自动递增 ID
    bool     data_inc,      // 是否自动递增数据
    uint8_t  dlen,
    uint8_t* payload
);
```



**示例：每100ms发送一次，ID自动递增**

```c
uint8_t data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
dmcan_device_send_can_details(
    dev, 0,           // 通道0
    100,              // 100ms间隔
    1,                // ID每次+1
    0x110,            // 停止ID
    10,               // 发送10次
    0x100,            // 起始ID
    false,            // CAN 2.0
    false,            // 标准帧
    false, false,     // 非远程帧，不切换波特率
    true,             // ID递增
    false,            // 数据不递增
    8, data
);
```



#### 队列发送

```c
bool dmcan_device_fill_can_queue(
    struct dmcan_device_handle* dev_handle,
    uint8_t ch, uint32_t can_id,
    bool canfd, bool ext, bool rtr, bool brs,
    uint8_t dlen, uint8_t* payload
);

bool dmcan_device_can_queue_send(
    struct dmcan_device_handle* dev_handle
);
```



**批量发送示例：**

```c
// 填充队列
for (int i = 0; i < 100; i++) {
    uint8_t data[8] = {uint8_t(i), 0, 0, 0, 0, 0, 0, 0};
    dmcan_device_fill_can_queue(dev, 0, 0x100 + i, false, false, false, false, 8, data);
}

// 一次性发送所有队列数据
dmcan_device_can_queue_send(dev);
```



------

### 工具函数

#### `dmcan_utils_get_dlc_from_len`

```c
int dmcan_utils_get_dlc_from_len(int dlen);
```

将数据长度（字节数）转换为 DLC（数据长度码）。

**映射表：**

| 数据长度 | DLC  |
| :------- | :--- |
| 0-8      | 0-8  |
| 12       | 9    |
| 16       | 10   |
| 20       | 11   |
| 24       | 12   |
| 32       | 13   |
| 48       | 14   |
| 64       | 15   |



#### `dmcan_utils_get_len_from_dlc`

```c
int dmcan_utils_get_len_from_dlc(int dlc);
```

将 DLC 转换为实际数据长度。



------

## 完整示例

### 示例 1：双设备同时通信

```c++
#include <iostream>
#include <thread>
#include "dmcan.h"

dmcan_device_handle *dev0 = nullptr, *dev1 = nullptr;

void recv_callback(dmcan_device_handle* h, usb_rx_frame_t* f) {
    const char* name = (h == dev0) ? "设备0" : "设备1";
    printf("[%s] 通道%d 收到: ID=0x%X, 数据=", name, f->head.channel, f->head.can_id);
    int len = dmcan_utils_get_dlc_from_len(f->head.dlc);
    for (int i = 0; i < len; i++) printf("%02X ", f->payload[i]);
    printf("\n");
}

int main() {
    dmcan_context* ctx;
    dmcan_context_create(&ctx);
    dmcan_print_version(ctx);

    int cnt = dmcan_find_devices(ctx);
    printf("找到 %d 个设备\n", cnt);
    dmcan_show_all_devices(ctx);

    if (cnt >= 2) {
        dmcan_device_get(ctx, &dev0, 0);
        dmcan_device_get(ctx, &dev1, 1);
        
        dmcan_device_open(dev0);
        dmcan_device_open(dev1);
        
        // 配置设备0
        dmcan_device_enable_channel(dev0, 0);
        dmcan_channel_can_info_t info = {0};
        dmcan_device_get_channel_baudrate(dev0, 0, &info);
        info.canfd = true;
        info.can_baudrate = 500000;
        info.canfd_baudrate = 2000000;
        dmcan_device_set_channel_baudrate(dev0, 0, info);
        dmcan_device_hook_recv_callback(dev0, recv_callback);
        
        // 配置设备1（双通道）
        dmcan_device_enable_channel(dev1, 0);
        dmcan_device_enable_channel(dev1, 1);
        dmcan_device_set_channel_baudrate(dev1, 0, info);
        dmcan_device_set_channel_baudrate(dev1, 1, info);
        dmcan_device_hook_recv_callback(dev1, recv_callback);
        
        // 交叉发送测试
        uint8_t data[] = "Test";
        dmcan_device_send_can(dev0, 0, 0x100, true, false, false, true, 4, data);
        dmcan_device_send_can(dev1, 0, 0x200, true, false, false, true, 4, data);
        dmcan_device_send_can(dev1, 1, 0x300, true, false, false, true, 4, data);
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
    dmcan_context_destroy(ctx);
    return 0;
}
```



### 示例 2：高性能数据采集

```c++
#include <iostream>
#include <fstream>
#include <atomic>
#include "dmcan.h"

std::ofstream log_file("can_data.log");
std::atomic<uint64_t> frame_count{0};

void recv_handler(dmcan_device_handle* h, usb_rx_frame_t* f) {
    frame_count++;
    
    // 记录时间戳和关键信息
    log_file << f->head.time_stamp << ","
             << std::hex << f->head.can_id << ","
             << std::dec << (int)dmcan_utils_get_dlc_from_len(f->head.dlc) << "\n";
    
    // 每秒输出统计
    static auto last_time = std::chrono::steady_clock::now();
    static uint64_t last_count = 0;
    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - last_time).count() >= 1) {
        uint64_t fps = frame_count - last_count;
        printf("接收速率: %llu 帧/秒\n", fps);
        last_count = frame_count.load();
        last_time = now;
    }
}

int main() {
    dmcan_context* ctx;
    dmcan_context_create(&ctx);
    
    dmcan_device_handle* dev;
    dmcan_find_devices(ctx);
    dmcan_device_get(ctx, &dev, 0);
    dmcan_device_open(dev);
    
    // 配置高速 CAN FD
    dmcan_device_enable_channel(dev, 0);
    dmcan_channel_can_info_t info = {0};
    dmcan_device_get_channel_baudrate(dev, 0, &info);
    info.canfd = true;
    info.can_baudrate = 500000;
    info.canfd_baudrate = 5000000;  // 5Mbps 数据域
    info.canfd_sp = 0.75;
    dmcan_device_set_channel_baudrate(dev, 0, info);
    
    dmcan_device_hook_recv_callback(dev, recv_handler);
    
    printf("开始采集，按回车键停止...\n");
    getchar();
    
    printf("总共接收 %llu 帧\n", frame_count.load());
    log_file.close();
    dmcan_context_destroy(ctx);
    
    return 0;
}
```



------

## 常见问题

### Q1: 设备打开失败？

- 检查设备是否正确连接并安装驱动
- Linux 下需要 root 权限：`sudo ./program`
- Windows 下检查设备管理器是否有黄色感叹号

### Q2: 发送数据但收不到？

- 确认 CAN 总线两端波特率一致
- 检查是否启用终端电阻（通常120Ω）
- 确认 `dmcan_device_enable_channel` 已调用

### Q3: 接收数据有丢帧？

- 回调函数中避免耗时操作（如 printf 可能较慢）
- 使用缓冲队列异步处理
- 检查 USB 带宽是否充足

### Q4: DLC 和数据长度是什么关系？

- DLC (Data Length Code) 是 CAN 协议中的长度编码
- CAN 2.0: DLC 0-8 对应 0-8 字节
- CAN FD: 使用 `dmcan_utils_get_dlc_from_len()` 进行转换
- 64字节数据对应 DLC=15

### Q5: 如何多线程操作？

- 每个设备句柄线程安全，可多线程同时访问
- 但建议一个设备由一个线程管理
- 回调函数可能在不同线程中执行

### Q6: 如何获取更详细的错误信息？

- 注册错误回调：`dmcan_device_hook_err_callback`
- 查看错误帧的 `esi` 字段
- 检查返回值（大部分函数返回 `bool` 表示成功/失败）

------

## 技术支持

- **SDK 版本**：通过 `dmcan_print_version()` 查看
- **设备固件版本**：通过 `dmcan_device_print_version()` 查看
- **示例代码**：参考 SDK 包中的 `examples` 目录
