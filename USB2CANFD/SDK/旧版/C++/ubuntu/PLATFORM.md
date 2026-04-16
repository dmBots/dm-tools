# USB2CANFD SDK / C++ / Ubuntu 平台流程

## 环境与默认行为
- 原 README 的测试环境为 Ubuntu 20.04 / 22.04。
- 默认示例会把 `can_id=0x01`、`mst_id=0x11` 的 DM4310 设为速度模式并运行，电机波特率为 `5M`。
- 原说明强调：5M 波特率下多电机接入时，末端需要增加 `120` 欧终端电阻。

## 安装与编译
- 安装 `libusb`：

```shell
sudo apt update
sudo apt install libusb-1.0-0-dev
```

- 创建工作目录并放置 `u2canfd` 工程：

```shell
mkdir -p ~/catkin_ws
cd ~/catkin_ws
```

- 编译：

```shell
cd ~/catkin_ws/u2canfd
mkdir build
cd build
cmake ..
make
```

## 权限与 SN 获取
- 配置 udev 权限：

```shell
sudo nano /etc/udev/rules.d/99-usb.rules
```

```shell
SUBSYSTEM=="usb", ATTR{idVendor}=="34b7", ATTR{idProduct}=="6877", MODE="0666"
```

```shell
sudo udevadm control --reload-rules
sudo udevadm trigger
```

- 运行 `dev_sn` 获取设备序列号：

```shell
cd ~/catkin_ws/u2canfd/build
./dev_sn
```

- 把得到的 `Serial_Number` 写回 `main.cpp` 中的设备 SN 字段。

## 运行默认示例

```shell
cd ~/catkin_ws/u2canfd/build
make
./dm_main
```

## 进阶控制
- 原示例给出的是 5M 波特率、1kHz 下同时控制 9 个 DM4310 的流程。
- 需要在 `main.cpp` 中修改：
  - 多个 `can_id / mst_id`
  - `nom_baud = 1000000`
  - `dat_baud = 5000000`
  - `std::vector<damiao::DmActData> init_data`
  - `Motor_Control` 初始化时的设备 SN
- 控制命令通过 `control_mit(...)` 发送。
- 电机反馈通过 `Get_Position()`、`Get_Velocity()`、`Get_tau()`、`getTimeInterval()` 读取。
- 1kHz 控制循环沿用 `std::this_thread::sleep_until(...)`。

## 回调说明
- 原 README 说明 `damiao.cpp` 中的 `Motor_Control::canframeCallback(...)` 用于解析接收到的 CAN 报文。
- 该函数由 `usb_class` 内部线程调用，不应在外部主动调用。
