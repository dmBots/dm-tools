# USB2CANFD SDK

中文主版

## 概述

- 本目录是 USB2CANFD SDK 的入口页，负责先帮你判断应该走哪条 SDK 路线。
- 当前这里保留的是 USB2CANFD 旧版 SDK 分支；如果你要做更通用的 USB 类设备开发，优先去 [../../DM_DeviceSDK/README.md](../../DM_DeviceSDK/README.md)。
- 旧版 SDK 的兼容提醒和更新说明已整理到 [UPDATE.md](UPDATE.md)。

## 文档 / 资源

- [UPDATE.md](UPDATE.md) - 旧版 SDK 兼容与更新说明
- [../README.md](../README.md) - 返回 USB2CANFD 总入口
- [旧版/C++/arm/README.md](旧版/C++/arm/README.md) - 旧版 C++ SDK 的 arm 平台入口
- [旧版/C++/ubuntu/README.md](旧版/C++/ubuntu/README.md) - 旧版 C++ SDK 的 Ubuntu 平台入口
- [旧版/C++/win/README.md](旧版/C++/win/README.md) - 旧版 C++ SDK 的 Windows 平台入口
- [旧版/Python/README.md](旧版/Python/README.md) - 旧版 Python SDK 入口
- [../../DM_DeviceSDK/README.md](../../DM_DeviceSDK/README.md) - 通用 USB 类设备 SDK 入口
- [../../../../docs/repository/mirror-scope.md](../../../../docs/repository/mirror-scope.md) - 查看双仓覆盖范围说明

## 快速开始

- 想先确认旧版 SDK 是否适合当前固件版本：先看 [UPDATE.md](UPDATE.md)。
- 想做 C++ 接入：按目标平台进入 `旧版/C++/arm`、`ubuntu` 或 `win`，再看对应 `PLATFORM.md`。
- 想做 Python 接入：先看 [旧版/Python/README.md](旧版/Python/README.md)，再按 `USAGE.md` 安装依赖和运行。
- 想做更新的通用 USB 设备接入：改看 [../../DM_DeviceSDK/README.md](../../DM_DeviceSDK/README.md)。
- 想确认 GitHub 上是否也有对应资料：先看 [../../../../docs/repository/mirror-scope.md](../../../../docs/repository/mirror-scope.md)。

## 状态

- ZH: 主版入口
- EN: Translation pending
- TBD: 新版 SDK 适配状态以后续公开资料为准
