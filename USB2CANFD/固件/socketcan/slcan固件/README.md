# SLCAN 固件使用说明

## 概述

- 本目录是 USB2CANFD 的 `slcan` 固件入口，负责帮你判断刷写、切换和验证该看什么。
- 具体刷机命令、切回出厂固件、Linux 侧验证流程都在 [FLASHING.md](FLASHING.md)。
- 如果你只是想知道下一步看哪个文件，本页先回答，不需要直接在固件文件里盲找。

## 文档 / 资源

- [FLASHING.md](FLASHING.md) - 刷写、切换和验证主流程
- [dm_usb2fdcan_slcan_1002.enc](dm_usb2fdcan_slcan_1002.enc) - 当前 `slcan` 固件包
- [../../出厂固件/](../../出厂固件/) - 切回出厂固件时需要回看的目录
- [img/](img/) - 流程截图
- [GitHub / Gitee 覆盖范围说明](https://gitee.com/kit-miao/damiao/blob/master/docs/repository/mirror-scope.md) - 双仓覆盖范围说明

## 快速开始

- 想刷到 `slcan` 固件：先看 [FLASHING.md](FLASHING.md) 的刷写步骤。
- 想切回出厂固件：先看 [FLASHING.md](FLASHING.md) 的切换说明，再回到 [../../出厂固件/](../../出厂固件/) 取对应固件包。
- 想验证刷写是否成功：按 [FLASHING.md](FLASHING.md) 里的串口、`candump` 和 `cansend` 检查步骤执行。
- 如果 GitHub 镜像里只有入口层或文件不全：先看 [GitHub / Gitee 覆盖范围说明](https://gitee.com/kit-miao/damiao/blob/master/docs/repository/mirror-scope.md)，再回 Gitee 继续。

## 状态

- ZH: 主版
- EN: Translation pending
- TBD: 刷机与切换说明保留在 `FLASHING.md`

