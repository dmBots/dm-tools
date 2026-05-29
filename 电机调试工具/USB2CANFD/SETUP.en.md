# DM-USB2FDCAN

[中文](./SETUP.md) | [EN](./SETUP.en.md)

#### Introduction

The DM-USB2FDCAN host software supports DM-USB2FDCAN series CAN cards and can run on both Windows and Linux.

#### Instructions

   **[DMTool-x86_64.AppImage] Linux host software**

1. **Configure user permission groups (required only when using USB2FDCAN)**

<html>
<table style="margin-left: auto; margin-right: auto; border-collapse: collapse; width: 100%; max-width: 1200px;">
  <tr style="background: linear-gradient(90deg, #e95420, #1793d1); color: white; font-weight: bold;">
    <td style="padding: 12px; text-align: center; width: 50%;">Debian/Ubuntu</td>
    <td style="padding: 12px; text-align: center; width: 50%;">Arch Linux</td>
  </tr>
  
  <!-- Device information row -->
  <tr style="border-bottom: 1px solid #eee;">
    <td style="padding: 15px; vertical-align: top; background: #f9f9f9;">
      <div style="color: #e95420; font-weight: bold; margin-bottom: 8px;">View device information</div>
      <pre style="background: #2d2d2d; color: #f8f8f2; padding: 12px; border-radius: 6px; overflow-x: auto; margin: 8px 0; font-size: 13px; line-height: 1.4;">
lsusb
eg.
Bus 001 Device 069: ID 34b7:6877 DaMiao-Tech DM-USB2FDCAN
VID 34b7, PID 6877</pre>
    </td>
    <td style="padding: 15px; vertical-align: top; background: #f9f9f9;">
      <div style="color: #1793d1; font-weight: bold; margin-bottom: 8px;">View device information</div>
      <pre style="background: #2d2d2d; color: #f8f8f2; padding: 12px; border-radius: 6px; overflow-x: auto; margin: 8px 0; font-size: 13px; line-height: 1.4;">
lsusb
eg.
Bus 001 Device 069: ID 34b7:6877 DaMiao-Tech DM-USB2FDCAN
VID 34b7, PID 6877</pre>
    </td>
  </tr>
  
  <!-- Notice row -->
  <tr style="border-bottom: 1px solid #eee;">
    <td colspan="2" style="padding: 15px; text-align: center; background: #fff3cd; color: #856404; font-weight: bold;">
      ⚠️ Notice: The PID and VID are different for the single-channel module and the dual-channel module!!!
    </td>
  </tr>
  
  <!-- Create udev rule file row -->
  <tr style="border-bottom: 1px solid #eee;">
    <td style="padding: 15px; vertical-align: top;">
      <div style="color: #e95420; font-weight: bold; margin-bottom: 8px;">Create udev rule file</div>
      <pre style="background: #2d2d2d; color: #f8f8f2; padding: 12px; border-radius: 6px; overflow-x: auto; margin: 8px 0; font-size: 12px; line-height: 1.3;">
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="34b7", ATTR{idProduct}=="6877", MODE="0666", GROUP="plugdev"
SUBSYSTEM=="usb_device", ATTR{idVendor}=="34b7", ATTR{idProduct}=="6877", MODE="0666", GROUP="plugdev"' | sudo tee /etc/udev/rules.d/99-dm-fdcan.rules > /dev/null</pre>
    </td>
    <td style="padding: 15px; vertical-align: top;">
      <div style="color: #1793d1; font-weight: bold; margin-bottom: 8px;">Create udev rule file</div>
      <pre style="background: #2d2d2d; color: #f8f8f2; padding: 12px; border-radius: 6px; overflow-x: auto; margin: 8px 0; font-size: 12px; line-height: 1.3;">
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="34b7", ATTR{idProduct}=="6877", MODE="0666", GROUP="uucp", TAG+="uaccess"
SUBSYSTEM=="usb_device", ATTR{idVendor}=="34b7", ATTR{idProduct}=="6877", MODE="0666", GROUP="uucp", TAG+="uaccess"' | sudo tee /etc/udev/rules.d/99-dm-fdcan.rules > /dev/null</pre>
      <div style="background: #f8d7da; color: #721c24; padding: 8px; border-radius: 4px; margin-bottom: 8px; font-size: 13px;">
        ⚠️ Notice: Arch users need to run arch_lib_install.sh manually to download dependencies.
      </div>
    </td>
  </tr>
  
  <!-- Reload udev rules row -->
  <tr style="border-bottom: 1px solid #eee;">
    <td style="padding: 15px; vertical-align: top;">
      <div style="color: #e95420; font-weight: bold; margin-bottom: 8px;">Reload udev rules</div>
      <pre style="background: #2d2d2d; color: #f8f8f2; padding: 12px; border-radius: 6px; overflow-x: auto; margin: 8px 0; font-size: 13px; line-height: 1.4;">
sudo udevadm control --reload-rules
sudo udevadm trigger</pre>
    </td>
    <td style="padding: 15px; vertical-align: top;">
      <div style="color: #1793d1; font-weight: bold; margin-bottom: 8px;">Reload udev rules</div>
      <pre style="background: #2d2d2d; color: #f8f8f2; padding: 12px; border-radius: 6px; overflow-x: auto; margin: 8px 0; font-size: 13px; line-height: 1.4;">
sudo udevadm control --reload-rules
sudo udevadm trigger</pre>
    </td>
  </tr>
  
  <!-- Add user to group row -->
  <tr>
    <td style="padding: 15px; vertical-align: top;">
      <div style="color: #e95420; font-weight: bold; margin-bottom: 8px;">Add the current user to the plugdev group</div>
      <pre style="background: #2d2d2d; color: #f8f8f2; padding: 12px; border-radius: 6px; overflow-x: auto; margin: 8px 0; font-size: 13px; line-height: 1.4;">
sudo usermod -aG plugdev $USER</pre>
    </td>
    <td style="padding: 15px; vertical-align: top;">
      <div style="color: #1793d1; font-weight: bold; margin-bottom: 8px;">Add the current user to the uucp group</div>
      <pre style="background: #2d2d2d; color: #f8f8f2; padding: 12px; border-radius: 6px; overflow-x: auto; margin: 8px 0; font-size: 13px; line-height: 1.4;">
sudo usermod -aG uucp $USER</pre>
    </td>
  </tr>
</table>
</html>

2. **Run**

  ##### Installed AppImage

   ```markdown
   # Run with root privileges
   sudo ./DMTool-x86_64.AppImage
   ```

   ##### AppImage not installed (cannot run .AppImage with sudo)


   ```markdown
   # View the serial device name
   ls /dev/tyyACM*
   # Set access permission for a specific device only (ttyACM0 is used here as an example; modify it according to the actual device name)
   sudo chmod 666 /dev/ttyACM0
   # Or grant access permission to all similar devices at once
   sudo chmod 666 /dev/ttyACM*
   # Set executable permission
   sudo chmod +x DMTool-x86_64.AppImage
   # Run
   ./DMTool-x86_64.AppImage
   ```

#### Version Update Notes

- **[V2.1.6.7] - 2025.5.29**
  ##### Major Updates:

  - Added mouse wheel zoom, mouse drag, point auto-snapping, slope calculation, and history data display features to the waveform control

- **[V2.1.6.6] - 2025.5.27**
  ##### Major Updates:

  - Removed the read ID and set ID buttons from the debug interface

  ##### Bug Fixes:

  - Fixed waveform control lag and excessive memory usage
  - Fixed missing 125K baud rate setting for USB2CAN modules

- **[V2.1.6.5] - 2025.5.20**
  ##### Bug Fixes:

  - Fixed incorrect binding issue between dual USB2CAN modules

- **[V2.1.6.3] - 2025.5.18**
  ##### Major Updates:

  - Adapted FDCAN module for opening and upgrading special SLCAN and GSUSB firmware; the bottom-left status bar now displays the special firmware name

  ##### Bug Fixes:

  - Fixed FDCAN module firmware upgrade issues
  - Fixed abnormal behavior after multiple calibrations of the output axis

- **[V2.1.6.2] - 2025.5.12**
  ##### Major Updates:

  - Optimized the waveform widget
  - Optimized serial-port refresh response speed
  - Added a prompt for failures caused by old FDCAN module firmware: please update the FDCAN module to the latest firmware (single-channel modules must be updated to 1004 or later)!
  - Removed the FDCAN module refresh button from the F2 device configuration page and replaced it with a restore factory button

  ##### Bug Fixes:

  - Fixed edge-jump issues in the waveform widget
  - Fixed software crashes when opening FDCAN modules with old firmware
  - Fixed the issue where hot-plugging could not correctly identify the FDCAN module in certain cases

- **[V2.1.6.1] - 2025.4.29**

  ##### Major Updates:

  - Added prompts for modifying pole pairs / reduction ratio through the serial port

  ##### Bug Fixes:

  - Fixed the invalid firmware-format issue caused by copying the firmware path

- **[V2.1.6.0] - 2025.4.17**
  ##### Major Updates:

  - Optimized the USB2CAN module user experience
  - Optimized the waveform widget
  - Optimized device scanning and device binding logic: opened devices are no longer displayed, and the selected serial port number is automatically bound to the libusb device port
  - The CAN analyzer now distinguishes and displays standard FDCAN frames and FDCAN accelerated frames
  - Added a host software manual link to the About page
  - Enabled modification of pole pairs and reduction ratio through the serial port

  ##### Bug Fixes:

  - Fixed brief stuttering when toggling the serial port on and off
  - Fixed issues related to bus channel utilization calculation
  - Fixed waveform delay, jagged behavior, and inability to adjust transmission interval when debugging with the USB2CAN module
  - Fixed the invalid-length prompt when sending zero-length frames in the CAN analyzer, removed the dynamic length limit on sent data, and prevented data from being cleared by mistake
  - Fixed the custom baud-rate configuration logic
  - Fixed serial output text to use a monospace font so serial print information is displayed correctly
  - Fixed the issue where CAN communication with the motor could not work at 3.2M when using FDCAN module firmware 1007

- **[V2.1.5.3] - 2025.12.15**
  ##### Bug Fixes:

  - Fixed incorrect error-frame counting

- **[v2.1.5.2] - 2025.12.12**

   ##### Major Updates

   - Adapted to new firmware for the `usb2canfd_dual` module and the single-channel `usb2fdcan` module (single-channel firmware requires v1.0.0.4 or later), and added error-frame counting

   ##### Bug Fixes:

   - Fixed incorrect index counting in the CAN analyzer
   - Fixed incorrect display of feedback-frame error codes (for example, 0x08 displayed as 0xf8)

- **[v2.1.4.6] - 2025.11.20**

  ##### Major Updates


  - Added support for big-endian/little-endian conversion of 1-byte and 2-byte data

  ##### Bug Fixes:

  - Fixed parameter-reading errors on the V2 driver board
  - Fixed the issue where HEX values in imported EDS files could not be parsed correctly
- **[v2.1.4.5] - 2025.11.18**

  ##### Bug Fixes:


  - Fixed feedback-frame parsing errors
- **[v2.1.4.4] - 2025.11.14**

  ##### Major Updates:


  - Added USB2CAN module baud-rate configuration to the device configuration page

  ##### Bug Fixes:

  - Fixed the issue where changing the frame ID in the CAN analyzer did not take effect immediately
- **[v2.1.4.2] - 2025.11.5**

  ##### Major Updates:


  - Supports gsusb firmware
  - CANOpen-related pages are hidden by default and are shown after CANOpen is checked in the settings page
  - Hardware timestamp sorting is disabled by default and can be enabled from the settings page
- **[v2.1.4.0] - 2025.11.3**

  ##### Major Updates:


  - Supports CANOpen (EDS dictionary import, SDO read/write, and graphical PDO mapping configuration)
  - Added a settings page (allows free configuration of the number of data points displayed by the waveform widget)

  ##### Bug Fixes:

  - Fixed the crash caused by configuring a custom baud rate when the FDCAN device was not opened
- **[v2.0.3.7] - 2025.10.10**

  ##### Bug Fixes:


  - Used hardware timestamps to sort data frames, fixing the issue where debug waveforms showed abnormal states due to out-of-order data
- **[v2.0.3.6] - 2025.9.30**

  ##### Major Updates:


  - Added periodic mode (periodic position, periodic speed, periodic torque)
  - The status bar displays the motor baud rate from the power-on print information
  - Waveform drawing in the debug interface is now always enabled
- **[v2.0.3.5] - 2025.9.23**

  ##### Major Updates:


  - Adjusted the waveform widget style
  - Improved motor baud-rate updates in the status bar
  - Added a data-field acceleration option to the CAN analyzer under FDCAN

  ##### Bug Fixes:

  - Fixed extra line breaks in power-on print information
  - Fixed the issue where CAN frames could not continue to be received normally after hot-plugging
  - Fixed incorrect upper and lower slider limits after switching RPM in MIT mode
  - Fixed data-mapping errors caused by range parameters on the debug page not being updated correctly after writing parameters
  - Fixed incorrect RPM conversion in PVT mode
- **[v2.0.3.4] - 2025.9.15**

  ##### Major Updates:


  - Added USB2FDCAN module and motor baud-rate prompts to the status bar
  - Added custom baud-rate configuration for the USB2FDCAN module

  ##### Bug Fixes:

  - Fixed ASCII/HEX conversion errors
  - Cleared endpoints after reception, fixing the issue where the lower-level device could become blocked
- **[v2.0.3.3] - 2025.9.10**

  ##### Major Updates:


  - Added special motor selection (H6215 hub motor, special calibration, and parameter calibration)
  - Allowed region selection and copying in the CAN analyzer

  ##### Bug Fixes:

  - Filtered non-printable characters in the serial data display box, fixing garbled printed data
- **[v2.0.3.2] - 2025.9.8**

  ##### Major Updates:


  - Added an About page

  ##### Bug Fixes:

  - Fixed the issue where CAN and serial data were not properly separated under the USB2CAN module
- **[v2.0.3.1] - 2025.9.4**

  ##### Bug Fixes:


  - Fixed the extended frame ID length limitation
  - Fixed abnormal Y-axis display in the waveform save widget
- **[v2.0.3.0] - 2025.9.2**

  ##### Major Updates:


  - Optimized serial port switch speed
  - Optimized adaptive adjustment of the Y-axis scale in the waveform widget

  ##### Bug Fixes:

  - Fixed abnormal software crashes
  - Reading and writing parameters with CAN in the enabled state no longer causes power loss
- **[v2.0.2.9] - 2025.8.25**

  ##### Major Updates:


  - Restored the old host-software style for the debug and parameter settings pages
  - Changed the control block diagram display format to table + text box + image
  - Added multi-curve save support to the waveform widget

  ##### Bug Fixes:

  - Changed the default CAN ID to 0x01 and displayed the default ID at the same time
- **[v2.0.2.8] - 2025.8.21**

  ##### Major Updates:


  - Added serial receive settings (whether to display and the display base)
  - Substantially adjusted the UI layout (moved part of the serial configuration into device configuration, added a serial queue send button, and hid the calibration tab by default)

  ##### Bug Fixes:

  - Filled in the missing FDCAN transmission length of 48
- **[v2.0.2.7] - 2025.8.19**

  ##### Major Updates:


  - Added arbitrary zoom support for the main interface.
  - Added a function that automatically switches the control-mode tab on the debug page according to the motor mode read after reading parameters.
  - Added a sampling-point display function to the FDCAN device configuration page.
  - Removed the frame parsing feature.

  ##### Bug Fixes:

  - Fixed speed mapping restrictions outside MIT mode
  - Fixed the white tooltip window issue
  - Fixed the calibration tab misalignment issue
- **[v2.0.2.7-trial] - 2025.8.14**

  ##### Major Updates:


  - Added arbitrary zoom support for the main interface. This is currently only a trial version and will be optimized later.
- **[v2.0.2.6] - 2025.8.13**

  ##### Major Updates:


  - Added serial-port list sending, opened with F3. It allows convenient customization of the serial send list and send interval.
  - Added local storage for runtime crash logs, but it only takes effect after the executable file is extracted. Logs are stored in the `logs` folder.

  ##### Bug Fixes:

  - Enabled input-box formatting when sending serial data in Hex format to correctly restrict input characters.
  - Fixed the issue where formatted input could incorrectly reset the selection, causing Ctrl key combinations to fail.
  - Fixed the issue where the reduction ratio `Gr` could not be imported correctly during parameter import.
- **[v2.0.2.5] - 2025.8.7**

  ##### Major Updates:


  - Rewrote the logic for scanning devices on the bus, allowing more accurate identification of devices present on the bus;
  - At the same time, retained the broadcast read/write ID function on the debug page and added a secondary confirmation dialog to prevent accidental operations.

  ##### Bug Fixes:

  - Fixed the firmware upgrade failure caused by changing the CAN frame transmission interval;
  - Fixed the issue where reading and writing parameters with a USB2CAN device could cause frame stalls.

#### Related Resources

1. User Manual: [DM-USB2FDCAN User Manual.pdf](https://gitee.com/kit-miao/dm-tools/blob/master/USB2FDCAN/%E8%BE%BE%E5%A6%99%E7%A7%91%E6%8A%80-USB%E8%BD%ACCANFD%E6%A8%A1%E5%9D%97%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E%E4%B9%A6V1.0(2).pdf)
2. Frequently Asked Questions: [DAMIAO Motor FAQ](https://gl1po2nscb.feishu.cn/wiki/NGhYwis06iKQqTkUwa6ckRaSnld)
3. Contact Support: [DAMIAO Forum](https://bbs.dmbot.cn/tags)

#### DAMIAO Technology Navigation Resources

1. [Customer Help Center](https://gl1po2nscb.feishu.cn/wiki/MZ32w0qnnizTpOkNvAZcJ9SlnXb)
2. [DAMIAO Product Materials Gitee Master Link](https://gitee.com/kit-miao/damiao)
3. [DAMIAO Product Materials GitHub Master Link](https://github.com/dmBots/DAMIAO-Motor)
4. [For easier direct downloads, we have specially provided this Feishu link so materials can be downloaded without login](https://gl1po2nscb.feishu.cn/drive/folder/RJL7fFT4ll9PDSdvM6Pc5vntnPw)

#### Copyright and Third-Party Agreements

1. **Software Copyright** This software is closed-source software, and the copyright is owned by [DAMIAO Technology Co., Ltd.].
2. **Third-Party Component Statement**
The core program of this software is proprietary software. During development, open-source libraries governed by open-source licenses were used, specifically including:

Qt library: governed by the GNU Lesser General Public License version 2.1 (LGPLv2.1) or version 3 (LGPLv3). The modules used specifically include: QtCore, QtGui, QtSerialPort, QtCore5Compat, QSql, and QOpenGL.
Copyright (C) [2025] The Qt Company Ltd. All rights reserved. Qt and the Qt logo are trademarks of The Qt Company Ltd.
Qwt library: governed by the GNU Lesser General Public License version 2.1 (LGPLv2.1).
Copyright (C) 1991, 1999 Free Software Foundation, Inc. 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
libusb library: governed by the GNU Lesser General Public License version 2.1 (LGPLv2.1).
Copyright © 2012-2025 libusb. All rights reserved.

User rights and source code access
According to the LGPL licenses governing these libraries, you have the right to obtain, modify, and redistribute the source code of these open-source libraries. You can obtain the source code corresponding to the versions used by this software through the following official channels:
Qt library (v6.5.3) source code: please visit the Qt official archive directory at https://download.qt.io/archive/qt/ and enter the `6.5/` subdirectory to download the corresponding source package.
Qwt library (v6.3.0) source code: please visit the official Qwt project website at https://qwt.sourceforge.io/ or its source repository.
libusb library (v1.0.27) source code: please visit the official libusb project website at https://libusb.info/ or its source repository.

License compliance statement
This software uses dynamic linking to call all of the open-source libraries listed above. Users may directly replace the relevant dynamic library files in the program directory (such as DLL files on Windows) to use modified or adapted versions of those libraries. Although this software mixes libraries under LGPLv2.1 and LGPLv3, Section 3 of LGPLv3 allows LGPLv2.1 libraries to be lawfully used in this environment. This mechanism complies with the core LGPL-series requirement regarding user freedom.
