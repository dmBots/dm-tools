# DM-USB2CANFD

中文 | [EN](./README.en.md) | GitHub 英文展示仓：`dmBots/dmBot`

## 概述

- 本目录是 USB2CANFD 模块的总入口，适合先判断你是要下载工具、跑上位机、切换固件还是接 SDK。
- 具体安装、运行和权限配置已经迁移到 [SETUP.md](SETUP.md)，本页只负责帮你选路。
- 如果你正在对照 GitHub 镜像找资料，先看 [../../../docs/repository/mirror-scope.md](../../../docs/repository/mirror-scope.md)。

## 文档 / 资源

- [SETUP.md](SETUP.md) - 模块安装、权限、连通性检查和基础运行准备
- [上位机/README.md](上位机/README.md) - 上位机入口；先在这里判断该下载哪个安装包
- [固件/socketcan/slcan固件/README.md](固件/socketcan/slcan固件/README.md) - `slcan` 固件入口；刷写、切换和验证都从这里继续
- [SDK/README.md](SDK/README.md) - SDK 入口；先区分旧版资料、语言、平台与兼容说明
- [达妙科技-USB转CANFD模块使用说明书V1.0.pdf](达妙科技-USB转CANFD模块使用说明书V1.0.pdf) - 模块说明书
- [外壳模型/](外壳模型/) - 外壳模型资料
- [网友分享-开源外壳/](网友分享-开源外壳/) - 开源外壳参考
- [../../../docs/repository/mirror-scope.md](../../../docs/repository/mirror-scope.md) - GitHub / Gitee 覆盖范围说明

## 快速开始

- 想把模块先跑通：先看 [SETUP.md](SETUP.md)。
- 想运行上位机：先看 [上位机/README.md](上位机/README.md)，再跳到 `SETUP.md`。
- 想刷写、切换或验证 `slcan` 固件：先看 [固件/socketcan/slcan固件/README.md](固件/socketcan/slcan固件/README.md)，再按 `FLASHING.md` 执行。
- 想接入 SDK：先看 [SDK/README.md](SDK/README.md)。如果你实际上需要更通用的 USB 设备 SDK 路线，再转到 `DM_DeviceSDK`。
- 想确认 GitHub 上有没有完整资料：先看 [../../../docs/repository/mirror-scope.md](../../../docs/repository/mirror-scope.md)，再决定是否回到 Gitee。

## 状态

- ZH: 主版
- EN: `README.en.md` 可用
- TBD: 版本更新与兼容说明保留在 `SETUP.md` 和 `SDK/UPDATE.md`
