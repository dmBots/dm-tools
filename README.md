# DM-Tools

中文 | [EN](./README.en.md) | GitHub 英文仓：`dmBots/dmBot`

## 概述

- 本目录提供 USB2CANFD 工具、上位机、固件、SDK 和常用驱动资料入口。
- 常用路径可优先从 `USB2CANFD` 和 `DM_DeviceSDK` 两个入口判断。
- 如需确认 GitHub 镜像是否覆盖某个工具分支，请参见 [GitHub / Gitee 覆盖范围说明](https://gitee.com/kit-miao/damiao/blob/master/docs/repository/mirror-scope.md)。

## 文档 / 资源

- [USB2CANFD/README.md](USB2CANFD/README.md) - 单路 USB2CANFD 主入口；下载工具、上位机、固件和 SDK 都先从这里判断
- [USB2CANFD/SETUP.md](USB2CANFD/SETUP.md) - 上位机与基础连接准备；先看这里再完成安装与连接
- [USB2CANFD/固件/socketcan/slcan固件/README.md](USB2CANFD/固件/socketcan/slcan固件/README.md) - `slcan` 固件入口；刷写、切换和验证都从这里开始
- [USB2CANFD/SDK/README.md](USB2CANFD/SDK/README.md) - USB2CANFD 专用 SDK 入口；先在这里区分旧版资料、语言和平台
- [DM Device SDK](https://gitee.com/kit-miao/dm-device-sdk) - 通用 USB 类设备 SDK 入口；适合 USB2CANFD、双路模块、ECAT2CANFD 等通用二次开发
- [USB2CANFD_Dual/README.md](USB2CANFD_Dual/README.md) - 双路 USB2CANFD 资料入口
- [gs_usb_drives/README.md](gs_usb_drives/README.md) - Linux 驱动与适配支持入口
- [GitHub / Gitee 覆盖范围说明](https://gitee.com/kit-miao/damiao/blob/master/docs/repository/mirror-scope.md) - 双仓覆盖范围说明

## 快速开始

- USB2CANFD 工具与基础连接请参见 [USB2CANFD/README.md](USB2CANFD/README.md)，后续按 `SETUP.md` 执行。
- 上位机与基础连接准备请参见 [USB2CANFD/SETUP.md](USB2CANFD/SETUP.md)，再按说明完成安装与连接。
- `slcan` 固件刷写、切换和验证请参见 [USB2CANFD/固件/socketcan/slcan固件/README.md](USB2CANFD/固件/socketcan/slcan固件/README.md)，后续按 `FLASHING.md` 执行。
- SDK 接入请根据用途选择 [USB2CANFD/SDK/README.md](USB2CANFD/SDK/README.md) 或 [DM Device SDK](https://gitee.com/kit-miao/dm-device-sdk)。
- SDK 示例、平台分支和旧版资料请以对应 SDK 目录 README 为准。
- 如需确认当前仓库是否覆盖对应内容，请参见 [GitHub / Gitee 覆盖范围说明](https://gitee.com/kit-miao/damiao/blob/master/docs/repository/mirror-scope.md)。

## 状态

- ZH: 主版入口
- EN: Translation pending
- TBD: 更低频的工具分支按各自目录维护

