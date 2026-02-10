# CAN USB Driver Toolkits<=>CAN USB 驱动程序工具包

**English** 

## Project Overview

> This repository contains specialized CAN USB driver toolkits for different embedded platforms. Each toolkit includes pre-compiled kernel modules and source code for various kernel versions.

### Directory Structure Quick Guide

```bash
gs_usb_drives/
├── README.md                    # This file
├── gs_usb_sppe_Orin_NX/         # For NVIDIA Jetson Orin series boards
│   ├── README_en.md            # English documentation
│   ├── README_zh-CN.md         # Chinese documentation
│   ├── gs_usb_5.10/            # Kernel 5.10 version
│   ├── gs_usb_5.15/            # Kernel 5.15 version
│   └── gs_usb_6.8/             # Kernel 6.8 version
└── gs_usb_sppe_RK_board/       # For Rockchip RK3588 series boards
    ├── README_en.md            # English documentation
    ├── README_zh-CN.md         # Chinese documentation
    ├── gs_usb_5.1/             # Kernel 5.1 version
    ├── gs_usb_6.1/             # Kernel 6.1 version
    └── gs_usb_6.8/             # Kernel 6.8 version
```

## Platform Selection

### For NVIDIA Jetson Series Boards
- Navigate to: `gs_usb_sppe_Orin_NX/`

- Compatible with: Jetson Orin Nano, Jetson Orin NX, and similar Orin series boards

-  Each kernel directory contains tested and verified modules

### For Rockchip RK3588 Series Boards
- Navigate to: gs_usb_sppe_RK_board/

- Compatible with: DAMIAO-RK development board and other RK3588-based boards

- Each kernel directory contains tested and verified modules

### Quality Assurance
**Note**: All modules in these directories (except where specially marked as "untested" in the documentation) have been tested and verified to work properly. These are production-ready drivers.

## Troubleshooting Guide
### Before Submitting Issues:
1. Check Physical Connections First

- Verify USB-CAN adapter is properly connected

- Ensure power supply is adequate

- Check cable integrity

- Confirm device detection with lsusb

2. Verify Basic Functionality

- Follow the step-by-step instructions in the respective README

- Ensure you're using the correct kernel version folder

### When Creating Gitee Issues:
1. Search Existing Issues First

- Check if your problem has already been reported

- Review closed issues for solutions

2. Create New Issue Only If:

- No existing issue matches your problem

- Existing issue has no responses/solutions

- Your situation is genuinely different

### Issue Reporting Requirements:

- Include your board model and kernel version

- Provide exact error messages

- Describe steps you've already taken

- Mention any troubleshooting attempted

### Quick Start Commands
```bash
# Identify your platform and kernel version
uname -r  # Check kernel version

# Example: For Jetson Orin with kernel 5.15
cd gs_usb_sppe_Orin_NX/gs_usb_5.15/

# Example: For RK3588 with kernel 6.1
cd gs_usb_sppe_RK_board/gs_usb_6.1/
```
## Support

1. Most common issues are resolved by:

Using the correct folder for your kernel version

- Ensuring proper physical connections

- Following the documentation exactly

- If problems persist after thorough checking, please submit a detailed issue on Gitee with all relevant information.

---

**Attention: Always start with physical connection verification. Many reported "driver issues" are actually hardware connection problems.**


## 目概述

> 本仓库包含针对不同嵌入式平台的专用CAN USB驱动程序工具包。每个工具包包含预编译的内核模块和适用于不同内核版本的源代码。

### 目录结构快速指南

```bash
gs_usb_drives/
├── README.md                    # 本文件
├── gs_usb_sppe_Orin_NX/         # 适用于 NVIDIA Jetson Orin 系列板卡
│   ├── README_en.md            # 英文文档
│   ├── README_zh-CN.md         # 中文文档
│   ├── gs_usb_5.10/            # 内核 5.10 版本
│   ├── gs_usb_5.15/            # 内核 5.15 版本
│   └── gs_usb_6.8/             # 内核 6.8 版本
└── gs_usb_sppe_RK_board/       # 适用于 Rockchip RK3588 系列板卡
    ├── README_en.md            # 英文文档
    ├── README_zh-CN.md         # 中文文档
    ├── gs_usb_5.1/             # 内核 5.1 版本
    ├── gs_usb_6.1/             # 内核 6.1 版本
    └── gs_usb_6.8/             # 内核 6.8 版本
```

## 平台选择

### 适用于 NVIDIA Jetson 系列板卡
- 进入目录： `gs_usb_sppe_Orin_NX/`

- 兼容：Jetson Orin Nano、Jetson Orin NX 及类似 Orin 系列板卡

- 每个内核目录包含经过测试验证的模块

### 适用于 Rockchip RK3588 系列板卡
- 进入目录： gs_usb_sppe_RK_board/

- 兼容：DAMIAO-RK 开发板及其他基于 RK3588 的板卡

- 每个内核目录包含经过测试验证的模块

## 质量保证
> **注意**： 这些目录中的所有模块（除文档中特别标记为"未测试"的）均已通过测试验证可以正常工作。这些是生产就绪的驱动程序。

## 故障排除指南
### 提交问题前：
1. 首先检查物理连接

- 确认USB-CAN适配器正确连接

- 确保电源供应充足

- 检查电缆完整性

- 使用 lsusb 确认设备检测

2. 验证基本功能

- 按照相应README中的逐步说明操作

- 确保使用正确的内核版本文件夹

### 创建Gitee议题时：
1. 首先搜索现有议题

- 检查您的问题是否已报告

- 查看已关闭议题中的解决方案

2. 仅在以下情况创建新议题：

- 没有现有议题匹配您的问题

- 现有议题没有回复/解决方案

- 您的情况确实不同

### 议题报告要求：
- 包含您的板卡型号和内核版本

- 提供确切的错误信息

- 描述您已采取的步骤

- 提及任何已尝试的故障排除

### 快速开始命令
```bash
# 识别您的平台和内核版本
uname -r  # 检查内核版本

# 示例：Jetson Orin，内核 5.15
cd gs_usb_sppe_Orin_NX/gs_usb_5.15/

# 示例：RK3588，内核 6.1
cd gs_usb_sppe_RK_board/gs_usb_6.1/
```
## 支持
大多数常见问题可通过以下方式解决：

- 使用适合您内核版本的文件夹

- 确保正确的物理连接

- 严格按照文档操作

如果经过彻底检查后问题仍然存在，请在Gitee上提交包含所有相关信息的详细议题。

---

**注意： 始终从物理连接验证开始。许多报告的"驱动程序问题"实际上是硬件连接问题。**
