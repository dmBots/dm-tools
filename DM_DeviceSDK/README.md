# DM_DeviceSDK

中文 | [EN](./README.en.md) | GitHub 英文展示仓：`dmBots/dmBot`

## 概述

- 本目录是达妙 USB 类设备通用 SDK 的总入口。
- 如果你是要把 USB2CANFD、USB2CANFD_Dual、LinkX4C等设备接到自己的软件里，先从这里判断是否该走通用 SDK，而不是旧版 USB2CANFD 专用 SDK。
- 当前公开的高频入口是 [C&C++/README.md](C&C++/README.md)；GitHub 镜像覆盖差异请看 [../../../docs/repository/mirror-scope.md](../../../docs/repository/mirror-scope.md)。

## 文档 / 资源

- [C&C++/README.md](C&C++/README.md) - C / C++ SDK 主入口；先在这里看快速接入、示例和库文件位置
- [../USB2CANFD/SDK/README.md](../USB2CANFD/SDK/README.md) - 如果你只做 USB2CANFD 专用接入，可对比这个专用 SDK 入口
- [../README.md](../README.md) - 返回 `dm-tools` 总入口
- [../../../docs/repository/mirror-scope.md](../../../docs/repository/mirror-scope.md) - 查看 GitHub / Gitee 覆盖范围差异

## 快速开始

- 想做通用 C / C++ 接入：先看 [C&C++/README.md](C&C++/README.md)，再跳到 `USAGE.md`。
- 想找示例、头文件、动态库和版本记录：先看 [C&C++/README.md](C&C++/README.md)，它会再把你分到 `example/`、`lib/` 和 `UPDATE.md`。
- 想做更新的通用 USB 设备接入：优先走本目录；如果你只做 USB2CANFD 旧版专用接入，再看 [../USB2CANFD/SDK/README.md](../USB2CANFD/SDK/README.md)。
- 想知道 GitHub 有没有同等覆盖：先看 [../../../docs/repository/mirror-scope.md](../../../docs/repository/mirror-scope.md)。

## 状态

- ZH: 主版入口
- EN: `README.en.md` 可用
- TBD: 其他语言分支后续按实际资料补齐
