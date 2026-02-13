# gs_usb_sppe_RK_board使用说明

> - ‘sppe’ 是单词‘special purpose’，当然这不是一个严谨的做法，不过表明了这个驱动工具包只能适用于Rockchip RK3588系列板卡，如 DAMIAO-RK 开发板
> - 每个子文件夹中只包含`gs_usb.c` `Makefile` `compat_6_xx.h`以及已生成的`gs_usb.ko`文件，必要时可能需要您自行编译一次，来替换掉现在的.ko文件实现更好的适配

### 文件结构
- 最主要的文件是gs_usb.c、Makefile、或者还有compat_5_xx.h 之类的头文件 以及 已经生成的gs_usb.ko文件
``` bash
gs_usb_sppe_RK_board
├── gs_usb_5.10
│   ├── compat_5_10.h
│   ├── gs_usb.c
│   ├── gs_usb.ko
│   ├── Makefile
│   └── ......
├── gs_usb_6.1
│   ├── compat_can_5_15.h
│   ├── gs_usb.c
│   ├── gs_usb.ko
│   ├── Makefile
│   └── ......
├── gs_usb_6.x
│   ├── gs_usb.c
│   ├── gs_usb.ko
│   ├── Makefile
│   └── ......
└── README_ZH.md

3 directories, 39 files
```


### 编译模块
- 进入文件夹后根据你的内核版本选择 
- 如果你不确定模块是否可用的话你可以尝试重新编译，如果你验证过很多次则可以直接加载内核
- 不同内核版本的gs_usb配置不同，因此不可以跨内核、跨平台使用（如：Jetson系列板卡）
- 你可以参考下表来选择内核版本

|系统版本|内核版本|
|---    |---    |
|ubuntu 20.04|Kernel 5.1(暂无相应固件)|
|ubuntu 22.04|Kernel 6.1|
|ubuntu 24.03|Kernel 6.x(暂未验证) |

```bash
# 以6.1版本为例，其他版本同理
# 进入模块文件夹
cd gs_usb_sppe_RK_board/gs_usb_6.1/

# 编译文件（通常不需要这步操作）
make -j4
```
> 编译会生成'gs_usb.ko' 等文件

### 应用模块
- 加载模块
```bash
# 临时加载
## 卸载已有的模块
sudo modprobe -r gs_usb

## 加载全部依赖模块
sudo modprobe can
sudo modprobe can-dev
sudo modprobe usbcore

## 尝试加载文件夹里的gs_usb模块
sudo insmod gs_usb.ko


# 永久加载（需要您测试通过临时加载的gs_usb）
sudo cp gs_usb.ko /lib/modules/$(uname -r)/kernel/drivers/net/can/usb/
sudo depmod -a
sudo modprobe gs_usb
```
- 卸载模块(通常不需要操作)
```bash
# 按依赖顺序卸载
sudo modprobe -r gs_usb

# 彻底卸载模块
## 先移除模块
sudo modprobe -r gs_usb
## 删除模块文件
sudo rm /lib/modules/$(uname -r)/kernel/drivers/net/can/usb/gs_usb.ko
## 更新模块依赖
sudo depmod -a
## 可选：禁止模块自动加载
echo "blacklist gs_usb" | sudo tee /etc/modprobe.d/blacklist-gs_usb.conf
```


### 验证gs_usb模块是否可用

```bash
# 检查CAN模块是否存在（如果不好看出来需要插拔一次,并且分别执行下面命令一次，来确认设备是否挂载）
lsusb


# 检查CAN设备是否存在，（若果有多个CAN设备号，可能需要插拔一次,并且分别执行下面命令一次来判断设备号）
sudo ip link 


# 配置CAN模块 仲裁阶段波特率 采样点位置 数据阶段波特率 启用CANFD模式（请根据实际的can设备号来决定是can0还是can1、2、3）
sudo ip link set can0 type can bitrate 1000000 sample-point 0.875 dbitrate 5000000 fd on


# 再试检查CAN0设备的配置是否生效
sudo ip link can0
```


> - 致此，您的配置工作大致已完成，如果您在使用中遇到问题，欢迎在gitee提交议题，我们会第一时间为您处理，请留意您的议题处理进度，也请不要重复提交议题（之前有人提出相似问题时）
> - 感谢您使用达妙科技产品，祝您生活、工作愉快！