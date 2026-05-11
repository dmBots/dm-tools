import time

from dmcan import DmCanContext, dmcan_channel_can_info, DmCanDevice, usb_rx_frame, dmcan_device_type


def recv_frame(dev_handle:DmCanDevice,rx_frame:usb_rx_frame):
    rx_frame.show()

def send_frame(dev_handle:DmCanDevice,rx_frame:usb_rx_frame):
    rx_frame.show()

# 创建设备上下文
context= DmCanContext()
# 打印SDK后端版本
context.print_version()
# 查找所有设备 默认查找所有设备 包括 单路 双路 linkx4c模块
context.find_devices()
# 可以使用查找指定模块类型的函数重载
# context.find_devices(dmcan_device_type.USB2CANFD_DUAL)

# 显示所有查找的设备
context.show_all_devices()
# 获取设备
device=context.get_device(0)
# 打开设备
device.open()
# 打印设备版本信息
device.print_version()
# 开启通道 这里使用的是双路模块演示 故开启两个通道
device.enable_channel(0,True)
device.enable_channel(1,True)

# 获取设备通道波特率信息
info=device.get_channel_baudrate(0)

# 修改波特率 只做使用演示 实际不修改
# info.canfd=True
# info.can_baudrate=500000
# info.canfd_baudrate=2000000
# info.can_sp=0.75
# info.canfd_sp=0.80
# device.set_channel_baudrate(0,info)

# 注册接收回调
device.hook_recv_callback(recv_frame)
# 注册发送回调
device.hook_sent_callback(send_frame)

while True:
    # 发送canfd帧
    device.send_can(0, 0x1, 8, bytes([0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88]), True, False, False, True)
    device.send_can(1, 0x2, 8, bytes([0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88]), True, False, False, True)
    time.sleep(1)

