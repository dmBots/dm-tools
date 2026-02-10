# gs_usb_sppe_RK_board User Manual

> - The term 'sppe' stands for 'special purpose'. While not a strictly formal designation, it indicates that this driver toolkit is specifically designed for Rockchip RK3588 series boards, such as the DAMIAO-RK development board.
> - Each subdirectory contains only gs_usb.c, Makefile, compat_6_xx.h, and a pre-compiled gs_usb.ko file. You may need to compile the module yourself if necessary, replacing the existing .ko file for better compatibility.

### File Structure
- The primary files are gs_usb.c, Makefile, optional header files like compat_5_xx.h, and the pre-generated gs_usb.ko file
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


### Compiling the Module
- Navigate to the appropriate folder based on your kernel version 
- If you're unsure about module compatibility, you can attempt recompilation. If you've verified compatibility multiple times, you can directly load the kernel module
- gs_usb configurations vary across kernel versions, so modules are not cross-compatible between different kernel versions or platforms (e.g., NVIDIA Jetson series boards)
- 你可以参考下表来选择内核版本

|
System Version  |内核Kernel Version版本|
|---            |---            |
|ubuntu 20.04   |Kernel 5.1 (no corresponding firmware available)|
|ubuntu 22.04   |Kernel 6.1     |
|ubuntu 24.03   |Kernel 6.x(untested)| 

```bash
# Using version 6.1 as an example; other versions follow the same process
# Navigate to the module directory
cd gs_usb_sppe_RK_board/gs_usb_6.1/

# Compile the module (usually not necessary)
make -j4
```
> Compilation will generate files such as 'gs_usb.ko'.

### Applying the Module
- Loading the module
```bash
# Temporary loading
## Unload any existing module
sudo modprobe -r gs_usb

## Load all dependency modules
sudo modprobe can
sudo modprobe can-dev
sudo modprobe usbcore

## Attempt to load the gs_usb module from the folder
sudo insmod gs_usb.ko


# Permanent loading (requires successful temporary loading verification)
sudo cp gs_usb.ko /lib/modules/$(uname -r)/kernel/drivers/net/can/usb/
sudo depmod -a
sudo modprobe gs_usb
```
- Unloading the module (usually not required)
```bash
# Unload in dependency order
sudo modprobe -r gs_usb

# Complete module removal
## First remove the module
sudo modprobe -r gs_usb
## Delete the module file
sudo rm /lib/modules/$(uname -r)/kernel/drivers/net/can/usb/gs_usb.ko
## Update module dependencies
sudo depmod -a
## Optional: Prevent automatic module loading
echo "blacklist gs_usb" | sudo tee /etc/modprobe.d/blacklist-gs_usb.conf
```


### Verifying gs_usb Module Functionality

```bash
# Check if the CAN module exists (if not visible, try replugging the device and executing the command again to confirm device mounting)
lsusb


# Check if CAN devices exist (if multiple CAN device numbers appear, try replugging and executing the command to identify the correct device number)
sudo ip link 


# Configure the CAN module - arbitration phase bitrate, sample point position, data phase bitrate, enable CANFD mode (use actual CAN device number: can0, can1, can2, or can3)
sudo ip link set can0 type can bitrate 1000000 sample-point 0.875 dbitrate 5000000 fd on


# Verify that CAN0 device configuration is applied
sudo ip link show can0
```


> - At this point, your configuration work is largely complete. If you encounter any issues during use, please submit an issue on Gitee. We will address it as soon as possible. Please monitor the progress of your issue and avoid duplicate submissions (when similar issues have been raised previously). 
> - Thank you for using DaMiao Technology products. We wish you success in your work and happiness in your life!