# DM_DeviceSDK / C&C++ 使用说明

## 支持设备
- `DEV_USB2CANFD`：单通道 USB-CANFD 设备
- `DEV_USB2CANFD_DUAL`：双通道 USB-CANFD 设备
- `DEV_ECAT2CANFD`：EtherCAT 转 CANFD 设备

## 依赖与构建环境
- Windows：使用 Visual Studio，链接 `.lib` 并包含 `pub_user.h`
- Linux：使用 GCC / G++，链接动态库并包含头文件
- 需要把 SDK 的头文件路径和库路径加入工程配置

## 典型接入流程

### 1. 创建设备句柄

```c++
damiao_handle* handle = damiao_handle_create(DEV_USB2CANFD_DUAL);
```

### 2. 查找并打开设备

```c++
int device_cnt = damiao_handle_find_devices(handle);
device_handle* dev_list[16];
int handle_cnt = 0;
damiao_handle_get_devices(handle, dev_list, &handle_cnt);
device_open(dev_list[0]);
```

### 3. 获取版本和序列号

```c++
char buf[255];
device_get_version(dev_list[0], buf, sizeof(buf));
device_get_serial_number(dev_list[0], buf, sizeof(buf));
```

### 4. 配置通道波特率并打开通道

```c++
device_channel_set_baud_with_sp(dev_list[0], 0, true, 1000000, 5000000, 0.75f, 0.75f);
device_open_channel(dev_list[0], 0);
```

- 原说明明确支持继续配置双通道设备的第二个通道。

### 5. 注册接收 / 发送回调

```c++
void rec_callback(usb_rx_frame_t* frame) {}
void sent_callback(usb_rx_frame_t* frame) {}

device_hook_to_rec(dev_list[0], rec_callback);
device_hook_to_sent(dev_list[0], sent_callback);
```

### 6. 发送 CAN / CANFD 报文
- 简单场景优先使用 `device_channel_send_fast(...)`
- 需要 ID 自增、发送步进等高级控制时，使用 `device_channel_send_advanced(...)`
- 原 README 特别提醒：发送前必须保证设备已经接入有效 CAN 总线

### 7. 释放资源

```c++
device_close(dev_list[0]);
damiao_handle_destroy(handle);
```

## 关键结构与回调
- `usb_tx_frame_t`：发送帧，包含 CAN ID、帧类型、通道、发送次数和 `payload`
- `usb_rx_frame_t`：接收帧，包含时间戳、方向、ACK、通道和接收数据
- 回调类型：
  - `dev_rec_callback`
  - `dev_sent_callback`
  - `dev_err_callback`
- 原 README 说明所有回调都在 SDK 内部线程中执行，接入时要注意线程安全

## 使用注意事项
- 通道编号从 `0` 开始
- 标准帧 ID 范围：`0x000 ~ 0x7FF`
- 扩展帧 ID 范围：`0x00000000 ~ 0x1FFFFFFF`
- CAN 2.0 最长 `8` 字节，CAN FD 最长 `64` 字节
- 高级发送接口可通过 `interval` 和 `send_times` 控制发送节奏
- 退出前务必调用 `device_close()` 和 `damiao_handle_destroy()`

## 示例与支持
- `example/` 中保留了原 README 提到的完整初始化与发送示例
- `UPDATE.md` 保留版本更新记录
- 问题反馈时，原 README 建议提供：
  - SDK 版本
  - 设备型号和序列号
  - 操作系统版本
  - 编译环境
  - 复现步骤
  - 日志 / 截图
  - 最小复现代码
