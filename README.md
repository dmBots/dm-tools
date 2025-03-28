# DM-Tools

#### 介绍
上位机工具

[DMTool] 是一个闭源软件，本软件使用了 LGPLv3 版本的 Qt 库进行开发。具体使用的 Qt 库包括：QtCore、QtGui、QtSerialPort、QtCore5Compat 和 QtCharts。

Copyright (C) [2025] The Qt Company Ltd. All rights reserved.
Qt 和 Qt logo 是 The Qt Company Ltd. 的商标。

本软件依据 LGPLv3 协议使用 Qt 库，如果你需要获取 Qt 库的源代码，可根据 LGPLv3 协议规定的方式进行获取。同时，本软件提供了重新链接 Qt 库的机制，以符合 LGPLv3 协议要求。

#### 软件架构
软件架构说明


#### 安装教程

1.  xxxx
2.  xxxx
3.  xxxx

#### 使用说明

[DMTool-x86_64.AppImage]linux上位机

如果安装了AppImage
1.  >>sudo ./DMTool-x86_64.AppImage 即可运行
如果没有安装AppImage（无法使用sudo运行.AppImage）
1.  >>ls /dev/tyyACM* 查看串口设备名称
2.  >>sudo chmod 666 /dev/ttyACM0 设置设备访问权限，这里的ttyACM0是串口设备名称，根据实际情况进行修改
3.  >>sudo chmod +x DMTool-x86_64.AppImage 设置可执行权限
4.  >>./DMTool-x86_64.AppImage 运行

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
