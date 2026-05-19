# DM_Device_SDK 示例程序

## 概述

本示例程序演示了如何使用 **DM_Device_SDK** 对 达妙CANFD设备（USB2CANFD 、USB2CANFD_DUAL以及LinkX4C）进行初始化、配置、数据收发及回调处理等操作。该程序可同时管理多个设备，并展示单包发送与接收功能。

## 硬件支持

- USB2CANFD
- USB2CANFD_DUAL
- LinkX4C

## 功能说明

1. **SDK 上下文初始化**
   - 调用 `dmcan_context_create` 创建上下文。
   - 使用 `dmcan_print_version` 打印 SDK 版本信息。

2. **设备查找与列表显示**
   - `dmcan_find_devices` 查找已连接设备数量。
   - `dmcan_show_all_devices` 显示所有设备信息。

3. **设备获取与打开**
   - 通过 `dmcan_device_get` 按索引获取设备句柄。
   - 使用 `dmcan_device_open` 打开设备。

4. **设备信息打印**
   - 调用 `dmcan_device_print_version` 打印指定设备的版本信息。

5. **通道使能与波特率配置**
   - 使用 `dmcan_device_enable_channel` 使能 CAN 通道。
   - 获取当前波特率配置：`dmcan_device_get_channel_baudrate`。
   - 设置新的波特率（支持 CAN 及 CAN FD）：`dmcan_device_set_channel_baudrate`。

6. **回调注册**
   - 注册发送回调：`dmcan_device_hook_sent_callback`
   - 注册接收回调：`dmcan_device_hook_recv_callback`
   - 回调函数中会打印 CAN 帧的通道、ID、DLC 及数据内容。

7. **数据发送**
   - 单包发送示例：`dmcan_device_send_can`
   - （注释部分）队列发送示例：
     - `dmcan_device_fill_can_queue`
     - `dmcan_device_can_queue_send`

8. **资源释放**
   - 单独关闭设备：`dmcan_device_close`
   - 销毁上下文并自动关闭所有设备：`dmcan_context_destroy`

## 编译与运行

### 前置条件

- 已安装 DM_Device_SDK 及相关驱动。
- 确保 `dmcan.h` 头文件路径正确，库文件可在链接阶段找到。
