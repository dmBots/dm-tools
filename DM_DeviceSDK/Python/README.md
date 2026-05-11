# DM-DeviceSDK-Python CAN 使用示例

基于DMCan C++后端的Python CAN/CAN FD通信示例程序

## 环境要求

### Python版本
- Python 3.13 及以上版本

### SDK版本
- C++后端版本：1.1.0 及以上

### 操作系统
- Windows 10/11 (x64)

### 硬件支持
- USB2CANFD 系列设备（单路/双路/LinkX4c模块）

## 目录结构

project/
├── dlls/ # SDK C++后端动态库文件夹
│ ├── libdm_device.dll # DMCan核心库
│ └── ... # 其他依赖库
├── libusb.dll # USB通信库
├── demo.py # 示例程序（本文件）
└── README.md # 本说明文档

## 安装步骤

### 1. 准备依赖库

#### 添加 libusb 库
将 `libusb.dll` 复制到程序运行目录（与demo脚本同级目录）

```bash
# 示例
copy libusb.dll ./
```

#### 添加 SDK C++ 后端

1. 在程序运行目录创建 `dlls` 文件夹
2. 将SDK C++后端的所有动态库文件（.dll）复制到 `dlls` 文件夹中

```bash
# 示例
mkdir dlls
copy C:\path\to\sdk\*.dll dlls\
```

### 2. 安装设备驱动

确保USB2CANFD设备驱动已正确安装：

- 连接设备到USB端口
- 安装相应的驱动程序（参考硬件手册）
- 在设备管理器中确认设备识别正常

## 快速开始

### 运行示例程序

```bash
python demo.py
```



### 程序说明

示例程序演示了以下功能：

1. **设备初始化**
   - 创建DMCan上下文
   - 打印SDK版本信息
   - 查找并显示所有可用设备
2. **设备操作**
   - 打开设备
   - 获取设备版本信息
   - 启用CAN通道（支持多通道）
3. **波特率配置**
   - 获取当前通道波特率信息
   - 支持修改CAN/CAN FD波特率
   - 支持配置采样点位置
4. **数据收发**
   - 注册接收回调函数
   - 注册发送回调函数
   - 循环发送CAN FD帧

## API 说明

### 主要类

#### DmCanContext

设备上下文管理类

```python
context = DmCanContext()
context.print_version()              # 打印SDK版本
context.find_devices()               # 查找所有设备
context.find_devices(device_type)    # 查找指定类型设备
context.show_all_devices()          # 显示所有设备信息
device = context.get_device(index)   # 获取指定设备
```



#### DmCanDevice

设备操作类

```python
device.open()                        # 打开设备
device.print_version()              # 打印设备版本
device.enable_channel(ch, enable)    # 启用/禁用通道
device.get_channel_baudrate(ch)      # 获取通道波特率信息
device.set_channel_baudrate(ch, info) # 设置通道波特率
device.send_can(ch, id, dlc, data, is_canfd, ext, rtr, ack) # 发送CAN帧
device.hook_recv_callback(callback)  # 注册接收回调
device.hook_sent_callback(callback)  # 注册发送回调
```



### 回调函数

```python
def recv_frame(dev_handle: DmCanDevice, rx_frame: usb_rx_frame):
    """接收帧回调"""
    rx_frame.show()

def send_frame(dev_handle: DmCanDevice, tx_frame: usb_rx_frame):
    """发送帧回调"""
    tx_frame.show()
```



### 设备类型

```python
from dmcan import dmcan_device_type

# 可用设备类型
dmcan_device_type.USB2CANFD_SINGLE    # 单路设备
dmcan_device_type.USB2CANFD_DUAL      # 双路设备
dmcan_device_type.LINKX4C             # LinkX4c模块
```



## 配置说明

### CAN FD帧结构

```python
frame.show()  # 输出格式示例：
# ID:S00000002 TS:3750364204560 CH:0 FD RX DLC:8(8) RTR:0 ESI:0 BRS:1 ACK:0 DATA:[11 22 33 44 55 66 77 88]
```



输出字段说明：

- `ID`: CAN报文ID（S:标准帧, E:扩展帧）
- `TS`: 时间戳
- `CH`: 通道号
- `FD/CA`: 帧类型（CAN FD / Classic CAN）
- `TX/RX`: 方向（发送/接收）
- `DLC`: 数据长度代码
- `RTR`: 远程传输请求
- `ESI`: 错误状态指示
- `BRS`: 比特率切换
- `ACK`: 应答位
- `DATA`: 数据载荷（十六进制）

### 波特率配置

python

```
info = device.get_channel_baudrate(0)
info.canfd = True              # 启用CAN FD
info.can_baudrate = 500000     # 仲裁域波特率 500kbps
info.canfd_baudrate = 2000000  # 数据域波特率 2Mbps
info.can_sp = 0.75             # 仲裁域采样点 75%
info.canfd_sp = 0.80           # 数据域采样点 80%
device.set_channel_baudrate(0, info)
```



## 常见问题

### 1. 找不到设备

- 检查设备是否正确连接
- 确认驱动安装正确
- 验证 `libusb.dll` 是否在运行目录

### 2. DLL加载失败

- 确认 `dlls` 文件夹包含所有必需的DLL文件
- 检查C++后端版本是否为1.1.0+
- 验证Python版本为3.13+

### 3. 权限问题

- 以管理员权限运行程序
- 检查USB端口权限设置

## 技术支持

- SDK文档：参考官方技术手册
- 驱动程序：联系硬件供应商
- 示例程序：本项目提供基础使用示例

## 注意事项

1. 使用前请确认C++后端版本兼容性
2. 多通道设备需要分别启用各通道
3. 发送CAN FD帧时注意配置正确的DLC值
4. 接收回调函数应快速返回，避免阻塞数据接收
5. 程序退出时确保正确关闭设备连接