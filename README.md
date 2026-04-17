# DM-Tools

中文 | [EN](./README.en.md) | [GitHub English](https://github.com/dmBots/dmBot/blob/master/8.Tools%20and%20Host%20Software/dm-tools/README.md)

## 概述

- 这里是客户查找 USB2CANFD 工具、上位机、固件、SDK 和常用驱动资料的统一入口。
- 如果你只是想尽快连上设备、跑通上位机、切换固件或接 SDK，优先从 `USB2CANFD` 和 `DM_DeviceSDK` 这两个入口判断。
- 如果你需要确认 GitHub 镜像是否也覆盖了某个工具分支，请先看 [../../docs/repository/mirror-scope.md](../../docs/repository/mirror-scope.md)。

## 文档 / 资源

- [USB2CANFD/README.md](USB2CANFD/README.md) - 单路 USB2CANFD 主入口；下载工具、上位机、固件和 SDK 都先从这里判断
- [USB2CANFD/上位机/README.md](USB2CANFD/上位机/README.md) - 上位机入口；先看这里再决定下载哪个安装包
- [USB2CANFD/固件/socketcan/slcan固件/README.md](USB2CANFD/固件/socketcan/slcan固件/README.md) - `slcan` 固件入口；刷写、切换和验证都从这里开始
- [USB2CANFD/SDK/README.md](USB2CANFD/SDK/README.md) - USB2CANFD 专用 SDK 入口；先在这里区分旧版资料、语言和平台
- [DM_DeviceSDK/README.md](DM_DeviceSDK/README.md) - 通用 USB 类设备 SDK 入口；适合 USB2CANFD、双路模块、ECAT2CANFD 等通用二次开发
- [USB2CANFD_Dual/README.md](USB2CANFD_Dual/README.md) - 双路 USB2CANFD 资料入口
- [gs_usb_drives/README.md](gs_usb_drives/README.md) - Linux 驱动与适配支持入口
- [../../docs/repository/mirror-scope.md](../../docs/repository/mirror-scope.md) - GitHub / Gitee 覆盖范围说明

## 快速开始

- 想下载并使用 USB2CANFD 工具：先看 [USB2CANFD/README.md](USB2CANFD/README.md)，它会继续把你分到 `SETUP.md`、上位机、固件或 SDK。
- 想运行上位机：先看 [USB2CANFD/上位机/README.md](USB2CANFD/上位机/README.md)，这里会告诉你 Windows / Linux / macOS 分别下载哪个包，以及下一步该看 `SETUP.md`。
- 想刷写、切换或验证 `slcan` 固件：先看 [USB2CANFD/固件/socketcan/slcan固件/README.md](USB2CANFD/固件/socketcan/slcan固件/README.md)，再按 `FLASHING.md` 执行。
- 想接入 SDK：如果你做的是 USB2CANFD 专用接入，先看 [USB2CANFD/SDK/README.md](USB2CANFD/SDK/README.md)；如果你做的是更通用的 USB 设备接入，先看 [DM_DeviceSDK/README.md](DM_DeviceSDK/README.md)。
- 想区分 SDK 示例、平台分支和旧版资料：USB2CANFD 的 SDK README 会先区分语言、平台和旧版兼容；`DM_DeviceSDK` 会把你导向通用 C / C++ SDK、示例和库文件。
- 想确认当前仓库有没有你要的内容：如果本页已经列出对应入口，就按对应 README 继续；如果本页没有，先看 [../../docs/repository/mirror-scope.md](../../docs/repository/mirror-scope.md) 判断它是不是当前仍以 Gitee 为准。
- 在 GitHub 上没找到完整资料：先看 [../../docs/repository/mirror-scope.md](../../docs/repository/mirror-scope.md)，再切到 Gitee 中文主仓继续。

## 状态

- ZH: 主版入口
- EN: Translation pending
- TBD: 更低频的工具分支按各自目录维护

