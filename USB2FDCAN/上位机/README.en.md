# DM-USB2FDCAN

#### Description
The DM-USB2FDCAN host computer software supports DM-USB2FDCAN series CAN cards and can run on both Windows and Linux operating systems.

#### Version Update Notes
1. **[v2.0.3.1] - 2025.9.4**
   ##### Bug Fixes:
   - Fix the remote frame ID length limitation
   - Fix the Y-axis display anomaly of the waveform saving control

2. **[v2.0.3.0] - 2025.9.2**
   ##### Major Updates:
   - Optimize serial port switching speed
   - Optimize adaptive adjustment of the Y-axis scale for the waveform control

   ##### Bug Fixes:
   - Fix abnormal software crash issue
   - Using CAN to read and write parameters in the enabled state no longer results in power loss

3. **[v2.0.2.9] - 2025.8.25**
   ##### Major Updates:
   - Restore the old upper computer style for the debugging and parameter configuration interface
   - Modify the display format of the control block diagram to "table + text box + image"
   - Add multi-curve saving function for the waveform control

   ##### Bug Fixes:
   - Update the default CAN ID to 0x01 and display the default ID simultaneously

4. **[v2.0.2.8] - 2025.8.21**
   ##### Major Updates:
   - Add serial port reception settings (toggle for display, configurable display base: binary/decimal/hexadecimal)
   - Significantly adjust UI layout:
   - Move part of serial port configurations to the "Device Configuration" module
   - Add a "Serial Port Queue Send" button
   - Hide the "Calibration" tab by default on initial launch

   ##### Bug Fixes:
   - Fill the gap of missing FDCAN transmission length (48 bytes)

5. **[v2.0.2.7] - 2025.8.19**
   ##### Major Updates:
   - Add arbitrary scaling function for the main interface
   - Add auto-switch function for the control mode tab in the debugging interface (switches based on the motor mode read after parameter reading)
   - Add sampling point display function in the FDCAN device configuration interface
   - Remove the frame parsing function

   ##### Bug Fixes:
   - Fix speed mapping restriction outside MIT mode
   - Fix white window issue of toolTip
   - Fix misalignment issue of the "Calibration" tab

6. **[v2.0.2.7-trial] - 2025.8.14**
   ##### Major Updates:
   - Added the function of arbitrary scaling for the main interface. Currently, this is only a trial version and will be optimized in subsequent updates.

7. **[v2.0.2.6] - 2025.8.13**
   ##### Major Updates:
   - Added the serial port list sending function, which can be called up by pressing F3. It allows convenient customization of the serial port sending list and sending interval.
   - Added the local storage function for runtime crash logs. However, this function takes effect only after decompressing the executable file, and the logs are stored in the "logs" folder.
   ##### Bug Fixes:
   - Enabled the input box formatting function when sending data in serial port Hex format to correctly restrict input characters.
   - Fixed the issue where the selection would be incorrectly reset under formatted input, causing the Ctrl combination keys to fail.
   - Fixed the issue where the reduction ratio (Gr) could not be imported correctly during parameter import.

8. **[v2.0.2.5] - 2025.8.7**
   ##### Major Updates:
   - The logic for scanning devices on the bus has been rewritten to identify existing devices on the bus more accurately.
   - Additionally, the function of broadcasting read/write IDs in the debugging interface has been retained, and a secondary confirmation dialog has been added to prevent misoperations.
   ##### Bug Fixes:
   - Fixed the issue where changing the CAN frame transmission interval caused firmware upgrade failures. 
   - Fixed the problem of reading and writing parameter card frames when using USB2CAN devices.

#### Copyright and Third-Party Agreements
1. **Software Copyright**
    This software is closed-source, and its copyright is owned by [Damiao Technology Co., Ltd.].

2. **Third-Party Component Declaration**
    During the development of this software, the Qt library following the GNU Lesser General Public License Version 3 (LGPLv3) has been used, specifically including: QtCore, QtGui, QtSerialPort, QtCore5Compat, and QtCharts.

    Copyright (C) [2025] The Qt Company Ltd. All rights reserved. Qt and the Qt logo are trademarks of The Qt Company Ltd.

    In accordance with the provisions of the LGPLv3, you have the right to obtain the source code of the Qt library used in this software, which can be downloaded from the official Qt website: https://download.qt.io/archive/qt/ (please select the corresponding source code package according to Qt 6.5.8, the actual version used in this software).

    This software calls the Qt library through dynamic linking. Users can directly replace the relevant Qt DLL files in the program directory to use a modified or adapted version of the Qt library, which complies with the requirements of the LGPLv3.

#### Instructions
   **[DMTool-x86_64.AppImage]linux host computer software**
1. **Configure user permission groups (Only required when using USB2FDCAN)**
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
   sudo ./DMTool-x86_64.AppImage
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
   sudo chmod +x DMTool-x86_64.AppImage
   # Run
   ./DMTool-x86_64.AppImage
   ```

#### Related Resources

1.  User Manual:[DM-USB2FDCAN User Manual.pdf](https://gitee.com/kit-miao/dm-tools/blob/master/USB2FDCAN/%E8%BE%BE%E5%A6%99%E7%A7%91%E6%8A%80-USB%E8%BD%ACCANFD%E6%A8%A1%E5%9D%97%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E%E4%B9%A6V1.0(2).pdf)
2.  Frequently Asked Questions:[DAMIAO Motor FAQ](https://gl1po2nscb.feishu.cn/wiki/NGhYwis06iKQqTkUwa6ckRaSnld)
3.  Contact Support:[DAMIAO Forum](https://bbs.dmbot.cn/tags)

#### DAMIAO Technology Navigation Materials

1. [Customer Support Center](https://gl1po2nscb.feishu.cn/wiki/MZ32w0qnnizTpOkNvAZcJ9SlnXb)
2. [DAMIAO Product Materials Gitee Master Link](https://gitee.com/kit-miao/damiao)
3. [DAMIAO Product Materials Github Master Link](https://github.com/dmBots/DAMIAO-Motor)
4. [To facilitate downloading materials for all our esteemed customers, our company has specially set up a Feishu link, which allows direct download without login](https://gl1po2nscb.feishu.cn/drive/folder/RJL7fFT4ll9PDSdvM6Pc5vntnPw)
