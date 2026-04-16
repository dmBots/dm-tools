# USB2CANFD SDK / Python 使用说明

## 环境与默认行为
- 原 README 测试环境为 Python 3.8 和 Python 3.10。
- 默认示例会把 `can_id=0x01`、`mst_id=0x11` 的 DM4310 设为速度模式并运行，电机波特率为 `5M`。
- 原说明强调：5M 波特率下多电机接入时，末端需要增加 `120` 欧终端电阻。

## 安装与目录准备
- 安装依赖：

```shell
pip3 install pyusb
```

- 创建工作目录：

```shell
mkdir -p ~/catkin_ws
cd ~/catkin_ws
```

- 把 `u2canfd` 工程放到工作目录下。

## 权限与 SN 获取
- Linux 需要配置 udev 权限；原 README 明确说明 macOS 和 Windows 不需要这一步：

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

- 运行 `dev_sn.py` 获取设备序列号：

```shell
cd ~/catkin_ws/u2canfd
python3 dev_sn.py
```

- 把输出的 `Serial_Number` 写回 `damiao.py`。

## 运行默认示例

```shell
cd ~/catkin_ws/u2canfd
python3 damiao.py
```

- 原 README 说明是：运行后电机会亮绿灯并旋转。

## 进阶控制
- 原示例给出的是 5M 波特率、1kHz 下同时控制 9 个电机的流程。
- 需要在主脚本中修改：
  - `canid1 ~ canid9`
  - `mstid1 ~ mstid9`
  - `init_data1`
  - `Motor_Control(1000000, 5000000, "SN", init_data1)`
- 控制命令通过 `control_mit(...)` 发送。
- 反馈读取通过 `Get_Position()`、`Get_Velocity()`、`Get_tau()`、`getTimeInterval()`。
- 原 README 额外提醒：在 Windows 或 macOS 上同时控制多个电机时，建议使用多个发送线程。
- 1kHz 控制循环保留原 README 推荐的节奏控制方式：

```python
while running.is_set():
    desired_duration = 0.001
```

## 回调说明
- 原 README 说明 `damiao.py` 中的 `canframeCallback(...)` 用于解析接收到的 CAN 报文。
- 该函数由 `usb_class` 内部线程调用，不应在外部主动调用。
- 原文还保留了 `can_value_type` / `can_head_type` 的结构定义，可继续对照源码查看。
