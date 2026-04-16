# USB2CANFD SDK

## 概述

- 本目录是 USB2CANFD SDK 的入口页，负责先帮你判断应该走哪条 SDK 路线。
- 当前这里保留的是 USB2CANFD 旧版 SDK 分支和兼容说明；如果你要做更通用的 USB 类设备开发，优先去 [../../DM_DeviceSDK/README.md](../../DM_DeviceSDK/README.md)。
- 旧版 SDK 对部分新固件版本存在兼容限制，因此本页先讲版本判断，再分到具体语言和平台。

## 文档 / 资源

- [旧版/C++/arm/README.md](旧版/C++/arm/README.md) - 旧版 C++ SDK 的 arm 平台入口
- [旧版/C++/ubuntu/README.md](旧版/C++/ubuntu/README.md) - 旧版 C++ SDK 的 Ubuntu 平台入口
- [旧版/C++/win/README.md](旧版/C++/win/README.md) - 旧版 C++ SDK 的 Windows 平台入口
- [旧版/Python/README.md](旧版/Python/README.md) - 旧版 Python SDK 入口
- [../../DM_DeviceSDK/README.md](../../DM_DeviceSDK/README.md) - 通用 USB 类设备 SDK 入口
- [../README.md](../README.md) - 返回 USB2CANFD 总入口
- [../../../../docs/mirror-scope.md](../../../../docs/mirror-scope.md) - 查看双仓覆盖范围说明

## 快速开始

- 想做 C++ 接入：先按目标平台进入 `旧版/C++/arm`、`ubuntu` 或 `win`，再看对应 `PLATFORM.md`。
- 想做 Python 接入：先看 [旧版/Python/README.md](旧版/Python/README.md)，再按 `USAGE.md` 安装依赖和运行。
- 想区分旧版资料、平台分支和示例位置：先看本页；本目录下全部 `旧版/` 内容都是兼容保留资料，平台细节在各平台目录里。
- 想做更新的通用 USB 设备接入：改看 [../../DM_DeviceSDK/README.md](../../DM_DeviceSDK/README.md)。
- 想确认 GitHub 上是否也有对应资料：先看 [../../../../docs/mirror-scope.md](../../../../docs/mirror-scope.md)。

## 状态

- ZH: 主版入口
- EN: Translation pending
- TBD: 新版 SDK 适配状态以后续公开资料为准

## 旧版兼容说明

- 旧版 SDK 不支持模块固件新版本。
- 单路 USB2CANFD：`1004` 版本起请谨慎核对兼容性。
- 双路 USB2CANFD：`1000` 版本起请谨慎核对兼容性。
- 新的 SDK 正在逐步适配上线，后续以实际公开资料为准。
