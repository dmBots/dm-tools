# DM-DeviceSDK-CSharp CAN 使用示例

本项目提供了基于 `DM_DeviceSDK_CSharp` 包的 CAN 设备控制示例，支持 USB2CANFD（单路/双路）及 Link4C 模块。

## 环境要求

- .NET 运行时 **8.0 及以上**（.NET 8+）
- 已安装 NuGet 包 `DM_DeviceSDK_CSharp`

安装命令：

```bash
dotnet add package DM_DeviceSDK_CSharp
```

### 依赖说明

> **重要：** 本 NuGet 包底层依赖 C++ 后端 SDK（DM DeviceSDK C++），**要求 C++ 后端 SDK 版本为 1.1.0 及以上**。请确保系统中已安装对应版本的 C++ SDK/驱动，否则可能导致部分功能不可用或运行异常。

## 功能演示

示例代码展示了以下操作流程：

1. 创建上下文并打印 SDK 版本
2. 查找所有可用 CAN 设备
3. 打开指定设备并查看设备信息
4. 使能 CAN 通道并配置波特率（支持 CAN/CAN FD）
5. 发送与接收 CAN 帧，通过事件回调打印帧内容

## 示例代码

```c#
using DM_DeviceSDK_CSharp.core;

// 创建上下文
DmCanContext context = new DmCanContext();

// 打印 SDK 版本
context.PrintVersion();

// 查找所有可用设备（包括 USB2CANFD 单/双路模块、Link4C 模块）
context.FindAllDevice();

// 也可使用带类型的查找，例如：
// context.FindAllDevice(dmcan_device_type.USB2CANFD);

// 打印所有模块信息
context.ShowAllDevices();

// 获取指定索引的设备
var device = context.GetDevice(0);
if (device == null)
    return;

// 打开设备
device.Open();

// 打印设备版本
device.PrintVersion();

// 打印设备详细信息（PID、VID、SN、类型等）
Console.WriteLine(device.ToString());

// 开启两个通道
device.EnableChannel(0, true);
device.EnableChannel(1, true);

// 获取通道波特率简略信息
dmcan_ch_can_info channel_info = default;
device.GetChannelBaudInfo(0, ref channel_info);

// 获取通道波特率详细配置
dmcan_ch_can_config channel_config = default;
device.GetChannelBaudConfig(0, ref channel_config);

// 修改通道波特率（使用简略信息结构）
channel_info.canfd = true;
channel_info.can_baudrate = 500000;        // CAN 波特率（经典模式）或仲裁域波特率（CAN FD）
channel_info.canfd_baudrate = 2000000;     // 加速域波特率（仅 CAN FD）
channel_info.can_sp = 0.75f;
channel_info.canfd_sp = 0.75f;
device.SetChannelBaudByInfo(0, channel_info);

// 也可使用详细配置结构修改波特率：
// device.SetChannelBaudByConfig(0, channel_config);

// 订阅发送/接收事件
device.ReceiveEchoEvent += OnReceive;
device.SendEchoEvent += OnSend;

void OnSend(CanRxEventArgs args)
{
    Console.WriteLine($"发送帧: {args.Frame}");
}

void OnReceive(CanRxEventArgs args)
{
    Console.WriteLine($"接收帧: {args.Frame}");
}

// 循环发送 CAN 帧
while (true)
{
    device.Send(0, 0x111, true, false, false, true, 8, new byte[] { 1, 2, 3, 4, 5, 6, 7, 8 });
    Thread.Sleep(1000);
}
```



## 主要 API 说明

| 方法 / 属性                               | 说明                                           |
| :---------------------------------------- | :--------------------------------------------- |
| `DmCanContext.FindAllDevice()`            | 扫描所有连接的 DM CAN 设备。                   |
| `context.GetDevice(index)`                | 按索引获取设备对象。                           |
| `device.Open()`                           | 打开设备。                                     |
| `device.EnableChannel(ch, enable)`        | 使能或关闭指定通道。                           |
| `device.GetChannelBaudInfo(ch, ref info)` | 获取通道的简化波特率信息。                     |
| `device.SetChannelBaudByInfo(ch, info)`   | 使用简化信息设置通道波特率。                   |
| `device.Send(...)`                        | 发送 CAN 帧，可指定 ID、远程帧、扩展帧等属性。 |
| `device.ReceiveEchoEvent`                 | 接收回调事件，返回接收到的 CAN 帧。            |
| `device.SendEchoEvent`                    | 发送回调事件，返回已发送的 CAN 帧。            |

## 设备支持类型

- USB2CANFD（单路/双路）
- Link4C 模块

## 注意事项

- 运行时要求 **.NET 8.0 及以上版本**，请确保开发与部署环境满足此要求。
- **C++ 后端 SDK 版本必须为 1.1.0 或更高版本**，否则本 NuGet 包可能无法正常工作。
- 请确保设备驱动已正确安装，且设备已被系统识别。
- 波特率配置须与总线其他节点匹配，否则会导致通信失败。
- 在处理接收事件时应尽快返回，避免阻塞内部接收线程。