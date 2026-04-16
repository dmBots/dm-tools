# USB2CANFD SDK / C++ / Windows 平台流程

## 环境与默认行为
- 原 README 的测试环境为 Windows。
- 默认示例会把 `can_id=0x01`、`mst_id=0x11` 的 DM4310 设为速度模式并运行，电机波特率为 `5M`。
- 原说明强调：5M 波特率下多电机接入时，末端需要增加 `120` 欧终端电阻。

## MSYS2 与依赖安装
- 原 README 使用 MSYS2 作为 Unix-like 构建环境：
  - 安装地址示例：`E:\\msys64`
  - 更新系统：`pacman -Syu`
  - 安装工具链和 CMake：

```shell
pacman -S --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake
```

  - 安装依赖：

```shell
pacman -Syu
pacman -S pkg-config
pacman -S --needed mingw-w64-x86_64-libusb
pacman -S --needed mingw-w64-x86_64-boost
```

## CLion 工具链配置
- 原 README 使用 CLion 示例配置：
  - `cmake.exe`
  - `mingw32-make.exe`
  - `gcc.exe`
  - `g++.exe`
  - `gdb.exe`
- 配置完成后，用 CLion 打开工程并完成 CMake 预编译，再编译 `Motor` 和 `dm_main`。

## 运行默认示例
- 通过 `dev_sn.cpp` 获取设备 SN。
- 把 SN 写回 `main.cpp`。
- 重新编译后运行 `main.cpp`。
- 原说明是：运行后电机会亮绿灯并旋转。

## 进阶控制
- 原 README 给出的是 Windows 下 5M 波特率、1kHz 同时控制 6 个 DM4310 的流程。
- 需要修改：
  - 全局 `Motor_Control` 对象
  - `main.cpp` 中的 `can_id / mst_id`
  - `nom_baud / dat_baud`
  - `init_data`
  - 设备 SN
- 原 README 额外强调：Windows 下当电机数量超过 3 个时，最好开启多个发送线程分别控制。
- 示例做法是在线程函数中控制 `id4 ~ id6`，主循环控制 `id1 ~ id3`。
- 反馈读取仍通过 `Get_Position()`、`Get_Velocity()`、`Get_tau()`、`getTimeInterval()`。
- 1kHz 循环仍建议使用 `std::this_thread::sleep_until(...)`。

## 回调说明
- 原 README 说明 `damiao.cpp` 中的 `Motor_Control::canframeCallback(...)` 用于解析接收到的 CAN 报文。
- 该函数由 `usb_class` 内部线程调用，不应在外部主动调用。
