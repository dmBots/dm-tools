# DM-USB2FDCAN

#### 介绍
DM-USB2FDCAN上位机软件支持DM-USB2FDCAN系列CAN卡，支持在windows和linux下运行

#### 使用说明
   **[DMTool-x86_64.AppImage]linux上位机**
1. **配置用户权限组（仅使用USB2FDCAN需要）**
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
   # root权限运行
   sudo ./DMTool-x86_64.AppImage
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
   sudo chmod +x DMTool-x86_64.AppImage
   # 运行
   ./DMTool-x86_64.AppImage
   ```

#### 版本更新说明
1. **[v2.0.3.4] - 2025.9.15**
   ##### 主要更新：
   - 状态栏新增USB2FDCAN模块与电机波特率提示
   - 新增USB2FDCAN模块自定义波特率配置功能

   ##### 问题修复：
   - 修复ASCII/HEX转换错误
   - 接收后清空端点，修复了下位机堵死的问题

2. **[v2.0.3.3] - 2025.9.10**
   ##### 主要更新：
   - 新增特殊电机选择(H6215轮毂电机 特殊校准和参数标定)
   - 允许CAN分析仪选取区域复制

   ##### 问题修复：
   - 串口数据展示框对不可打印字符进行过滤，修复了打印数据乱码问题

3. **[v2.0.3.2] - 2025.9.8**
   ##### 主要更新：
   - 新增关于界面

   ##### 问题修复：
   - 修复USB2CAN模块下未正确分离CAN与串口数据的问题

4. **[v2.0.3.1] - 2025.9.4**
   ##### 问题修复：
   - 修复拓展帧ID长度限制
   - 修复波形保存控件纵坐标显示异常

5. **[v2.0.3.0] - 2025.9.2**
   ##### 主要更新：
   - 优化串口开关速度
   - 优化波形控件纵坐标刻度的自适应调整

   ##### 问题修复：
   - 修复了软件异常崩溃问题
   - 使能状态下使用CAN读写参数不再掉电

6. **[v2.0.2.9] - 2025.8.25**
   ##### 主要更新：
   - 调试与参数设置界面恢复老上位机样式
   - 控制框图展示形式修改为表格+文本框+图片形式
   - 补充波形控件多曲线保存功能

   ##### 问题修复：
   - 默认CAN ID修改为0x01，同时展示默认ID

7. **[v2.0.2.8] - 2025.8.21**
   ##### 主要更新：
   - 新增串口接收设置（显示与否、显示进制）
   - 大幅调整UI布局（将部分串口配置移到设备配置中，新增串口队列发送按键，初始隐藏校准标签页）

   ##### 问题修复：
   - 填补FDCAN发送长度48的缺失

8. **[v2.0.2.7] - 2025.8.19**
   ##### 主要更新：
   - 添加了主界面任意缩放功能。
   - 添加读参数后根据读取的电机模式自动切换调试界面的控制模式标签页的功能。
   - 添加FDCAN设备配置界面新增采样点展示功能。
   - 砍掉了帧解析功能。

   ##### 问题修复：
   - 修复MIT模式外的速度映射受限
   - 修复toolTip白窗
   - 修复了校准标签页错位问题

9. **[v2.0.2.7-trial] - 2025.8.14**
   ##### 主要更新：
   - 添加了主界面任意缩放功能。当前仅为试用版本，后续会进行优化。

10. **[v2.0.2.6] - 2025.8.13**
   ##### 主要更新：
   - 添加串口列表发送功能，F3调出。可便捷地自定义串口发送列表以及发送间隔。
   - 添加运行崩溃日志本地存储功能，但需要解压可执行文件后才能生效，日志存储在logs文件夹下。

   ##### 问题修复：
   - 串口Hex格式发送时启动输入框格式化功能，以正确限制输入字符。
   - 修复在格式化输入下会错误重置选区导致ctrl组合键失效的问题。
   - 修复参数导入无法正确导入减速比Gr的问题。

11. **[v2.0.2.5] - 2025.8.7**
   ##### 主要更新：
   - 重写扫描总线上设备逻辑，可以更准确的识别总线上所存在的设备；
   - 同时保留了调试界面下广播读写ID的功能，添加了二次确认对话框防止误操作。
   ##### 问题修复：
   - 修复了更改 CAN 帧发送间隔导致的固件升级失败问题；
   - 修复了使用 USB2CAN 设备读写参数卡帧问题。

#### 相关资源

1.  使用手册：[DM-USB2FDCAN使用手册.pdf](https://gitee.com/kit-miao/dm-tools/blob/master/USB2FDCAN/%E8%BE%BE%E5%A6%99%E7%A7%91%E6%8A%80-USB%E8%BD%ACCANFD%E6%A8%A1%E5%9D%97%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E%E4%B9%A6V1.0(2).pdf)
2.  常见问题：[达妙电机FAQ](https://gl1po2nscb.feishu.cn/wiki/NGhYwis06iKQqTkUwa6ckRaSnld)
3.  联系支持：[达妙论坛](https://bbs.dmbot.cn/tags)

#### 达妙科技导航资料

1. [客户帮助中心](https://gl1po2nscb.feishu.cn/wiki/MZ32w0qnnizTpOkNvAZcJ9SlnXb)
2. [达妙产品资料Gitee总链接](https://gitee.com/kit-miao/damiao)
3. [达妙产品资料Github总链接](https://github.com/dmBots/DAMIAO-Motor)
4. [为方便各位上帝下载资料，敝司特意开通飞书连接，免登录直接下载链接](https://gl1po2nscb.feishu.cn/drive/folder/RJL7fFT4ll9PDSdvM6Pc5vntnPw)

#### 版权与第三方协议
1. **软件版权**  
   本软件为闭源软件，版权归 [达妙科技有限公司] 所有。

2. **第三方组件声明**  
   本软件开发过程中使用了遵循 GNU Lesser General Public License Version 3（LGPLv3）协议的 Qt 库，具体包括：QtCore、QtGui、QtSerialPort、QtCore5Compat 及 QtCharts。  

   Copyright (C) [2025] The Qt Company Ltd. 保留所有权利。Qt 及 Qt logo 均为 The Qt Company Ltd. 的商标。  

   根据 LGPLv3 协议规定，您有权获取本软件所使用的 Qt 库源代码，可通过 Qt 官方网站下载：[https://download.qt.io/archive/qt/](https://download.qt.io/archive/qt/)（请根据本软件实际使用的 Qt6.5.8 版本选择对应的源代码包）。  

   本软件采用动态链接方式调用 Qt 库，用户可直接替换程序目录中相关的 Qt DLL 文件，以使用经修改或适配后的 Qt 库版本，此机制符合 LGPLv3 协议要求。  
   