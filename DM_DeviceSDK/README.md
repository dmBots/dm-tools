## 达妙USB2CANFD类模块二次开发SDK

#### 1.简介

​	这是达妙USB类设备通用二次开发SDK，支持开发的设备有USB2CANFD模块、USB2CANFD_Dual模块、ECAT2CANFD模块。目前支持开发的语言有C/C++、C#，后续会加入Python版本。

#### 2.引用声明

​	SDK的开发均采用了libusb库([LGPL-2.1 license](https://github.com/libusb/libusb#LGPL-2.1-1-ov-file))[libusb/libusb: A cross-platform library to access USB devices](https://github.com/libusb/libusb)

​	SDK开发严格遵守LGPL2.1协议，未修改libusb库源代码，未采用静态链接的方式引入libusb，不属于libusb衍生物，属于闭源软件

