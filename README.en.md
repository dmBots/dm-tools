# DM-Tools

#### Description
The DM debugging tool supports DM-USB2CAN series CAN cards and can run on Windows.
The DM-USB2FDCAN host computer software supports DM-USB2FDCAN and DM-USB2CAN series CAN cards, and can run on both Windows and Linux.

#### Copyright and Third-Party Agreements
1. **Software Copyright**
    This software is closed-source, and its copyright is owned by [Damiao Technology Co., Ltd.].

2. **Third-Party Component Declaration**
    During the development of this software, the Qt library following the GNU Lesser General Public License Version 3 (LGPLv3) has been used, specifically including: QtCore, QtGui, QtSerialPort, QtCore5Compat, and QtCharts.

    Copyright (C) [2025] The Qt Company Ltd. All rights reserved. Qt and the Qt logo are trademarks of The Qt Company Ltd.

    In accordance with the provisions of the LGPLv3, you have the right to obtain the source code of the Qt library used in this software, which can be downloaded from the official Qt website: https://download.qt.io/archive/qt/ (please select the corresponding source code package according to Qt 6.5.8, the actual version used in this software).

    This software calls the Qt library through dynamic linking. Users can directly replace the relevant Qt DLL files in the program directory to use a modified or adapted version of the Qt library, which complies with the requirements of the LGPLv3.

#### Instructions
   **[DM-USB2FDCAN-x86_64.AppImage]linux host computer software**
1. **Configure user permission groups**
   ```markdown
   # View device information
   lsusb
   # Create udev rule files
   echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="34b7", ATTR{idProduct}=="6877", MODE="0666", GROUP="plugdev"
   SUBSYSTEM=="usb_device", ATTR{idVendor}=="34b7", ATTR{idProduct}=="6877", MODE="0666", GROUP="plugdev"' | sudo tee /etc/udev/rules.d/99-dm-fdcan.rules > /dev/null
   # Reload udev rules
   sudo udevadm control --reload-rules
   sudo udevadm trigger
   # Add the current user to the plugdev group
   sudo usermod -aG plugdev $USER
   ```
2. **Run**
   ##### Installed AppImage
   ```markdown
   # Run with root privileges
   sudo ./DM-USB2FDCAN-x86_64.AppImage
   ```
   ##### Uninstalled AppImage (cannot run .AppImage using sudo)
   ```markdown
   # View serial port device name
   ls /dev/tyyACM*
   # Set access permissions for a specific device individually (taking the serial port device ttyACM0 as an example; modify according to the actual device name)
   sudo chmod 666 /dev/ttyACM0
   # Or grant access permissions to all devices of the same type at once
   sudo chmod 666 /dev/ttyACM*
   # Set executable permissions
   sudo chmod +x DM-USB2FDCAN-x86_64.AppImage
   # Run
   ./DM-USB2FDCAN-x86_64.AppImage
   ```

#### Related Resources

1.  User Manual:[DM-USB2FDCAN User Manual.pdf](https://gitee.com/kit-miao/dm-usb2-fdcan/blob/master/%E4%B8%8A%E4%BD%8D%E6%9C%BA/DM-USB2FDCAN使用手册.pdf)
2.  Frequently Asked Questions:[DAMIAO Motor FAQ](https://gl1po2nscb.feishu.cn/wiki/NGhYwis06iKQqTkUwa6ckRaSnld)
3.  Contact Support:[DAMIAO Forum](https://bbs.dmbot.cn/tags)

#### DAMIAO Technology Navigation Materials

1. [Customer Support Center](https://gl1po2nscb.feishu.cn/wiki/MZ32w0qnnizTpOkNvAZcJ9SlnXb)
2. [DAMIAO Product Materials Gitee Master Link](https://gitee.com/kit-miao/damiao)
3. [DAMIAO Product Materials Github Master Link](https://github.com/dmBots/DAMIAO-Motor)
4. [To facilitate downloading materials for all our esteemed customers, our company has specially set up a Feishu link, which allows direct download without login](https://gl1po2nscb.feishu.cn/drive/folder/RJL7fFT4ll9PDSdvM6Pc5vntnPw)
