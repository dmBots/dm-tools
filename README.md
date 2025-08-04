# DM-Tools

#### 介绍
DM调试工具支持DM-USB2CAN系列CAN卡，支持在windows下运行
DM-USB2FDCAN上位机软件支持DM-USB2FDCAN和DM-USB2CAN系列CAN卡，支持在windows和linux下运行

#### 版权与第三方协议
1. **软件版权**  
   本软件为闭源软件，版权归 [达妙科技有限公司] 所有。

2. **第三方组件声明**  
   本软件开发过程中使用了遵循 GNU Lesser General Public License Version 3（LGPLv3）协议的 Qt 库，具体包括：QtCore、QtGui、QtSerialPort、QtCore5Compat 及 QtCharts。  

   Copyright (C) [2025] The Qt Company Ltd. 保留所有权利。Qt 及 Qt logo 均为 The Qt Company Ltd. 的商标。  

   根据 LGPLv3 协议规定，您有权获取本软件所使用的 Qt 库源代码，可通过 Qt 官方网站下载：[https://download.qt.io/archive/qt/](https://download.qt.io/archive/qt/)（请根据本软件实际使用的 Qt6.5.8 版本选择对应的源代码包）。  

   本软件采用动态链接方式调用 Qt 库，用户可直接替换程序目录中相关的 Qt DLL 文件，以使用经修改或适配后的 Qt 库版本，此机制符合 LGPLv3 协议要求。  

#### 使用说明
   **[DM-USB2FDCAN-x86_64.AppImage]linux上位机**
1. **配置用户权限组**
   ```markdown
   # 查看设备信息
   lsusb
   # 创建 udev 规则文件
   echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="34b7", ATTR{idProduct}=="6877", MODE="0666", GROUP="plugdev"
   SUBSYSTEM=="usb_device", ATTR{idVendor}=="34b7", ATTR{idProduct}=="6877", MODE="0666", GROUP="plugdev"' | sudo tee /etc/udev/rules.d/99-dm-fdcan.rules > /dev/null
   # 重新加载 udev 规则
   sudo udevadm control --reload-rules
   sudo udevadm trigger
   # 将当前用户加入 plugdev 组
   sudo usermod -aG plugdev $USER
   ```
2. **运行**
   ##### 已安装AppImage
   ```markdown
   # root权限运行运行
   sudo ./DM-USB2FDCAN-x86_64.AppImage
   ```
   ##### 未安装AppImage（无法使用sudo运行.AppImage）
   ```markdown
   # 查看串口设备名称
   ls /dev/tyyACM*
   # 单独设置特定设备的访问权限（此处以串口设备ttyACM0为例，请根据实际设备名称修改）
   sudo chmod 666 /dev/ttyACM0
   # 或一次性授予同类设备的访问权限
   sudo chmod 666 /dev/ttyACM*
   # 设置可执行权限
   sudo chmod +x DM-USB2FDCAN-x86_64.AppImage
   # 运行
   ./DM-USB2FDCAN-x86_64.AppImage
   ```

#### 相关资源

1.  项目使用手册：[DM-USB2FDCAN使用手册.pdf](https://gitee.com/kit-miao/dm-usb2-fdcan/blob/master/%E4%B8%8A%E4%BD%8D%E6%9C%BA/README.md)
2.  常见问题：[达妙电机FAQ](https://gl1po2nscb.feishu.cn/wiki/NGhYwis06iKQqTkUwa6ckRaSnld)
3.  联系支持：[达妙论坛](https://bbs.dmbot.cn/tags)

#### 达妙科技导航资料

1. [客户帮助中心](https://gl1po2nscb.feishu.cn/wiki/MZ32w0qnnizTpOkNvAZcJ9SlnXb)
2. [达妙产品资料Gitee总链接](https://gitee.com/kit-miao/damiao)
3. [达妙产品资料Github总链接](https://github.com/dmBots/DAMIAO-Motor)
4. [为方便各位上帝下载资料，敝司特意开通飞书连接，免登录直接下载链接](https://gl1po2nscb.feishu.cn/drive/folder/RJL7fFT4ll9PDSdvM6Pc5vntnPw)