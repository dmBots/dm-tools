#include <iostream>
#include <thread>

#include "../user/dmcan.h"


struct dmcan_device_handle* u2canfd_dev=nullptr,* u2canfd_dual_dev=nullptr;

void recv_frame_handle(dmcan_device_handle* handle, usb_rx_frame_t* frame)
{

    if (handle==u2canfd_dev)
    {
        printf("device0 recv frame: ch:%d can_id=0x%X, dlc=%d, data=", frame->head.channel,frame->head.can_id, frame->head.dlc);
        uint8_t dlen=dmcan_utils_get_dlc_from_len(frame->head.dlc);
        for (size_t i = 0; i < dlen; ++i)
        {
            printf("%02X ", frame->payload[i]);
        }
        printf("\n");

    }
    else if (handle==u2canfd_dual_dev)
    {
        printf("device1 recv frame: ch:%d can_id=0x%X, dlc=%d, data=", frame->head.channel,frame->head.can_id, frame->head.dlc);
        uint8_t dlen=dmcan_utils_get_dlc_from_len(frame->head.dlc);
        for (size_t i = 0; i < dlen; ++i)
        {
            printf("%02X ", frame->payload[i]);
        }
        printf("\n");

    }
}

void sent_frame_handle(dmcan_device_handle* handle, usb_rx_frame_t* frame)
{
    if (handle==u2canfd_dev)
    {
        printf("device0 sent frame: ch:%d can_id=0x%X, dlc=%d, data=", frame->head.channel,frame->head.can_id, frame->head.dlc);
        uint8_t dlen=dmcan_utils_get_dlc_from_len(frame->head.dlc);
        for (size_t i = 0; i < dlen; ++i)
        {
            printf("%02X ", frame->payload[i]);
        }
        printf("\n");

    }
    else if (handle==u2canfd_dual_dev)
    {
        printf("device1 sent frame: ch:%d can_id=0x%X, dlc=%d, data=", frame->head.channel,frame->head.can_id, frame->head.dlc);
        uint8_t dlen=dmcan_utils_get_dlc_from_len(frame->head.dlc);
        for (size_t i = 0; i < dlen; ++i)
        {
            printf("%02X ", frame->payload[i]);
        }
        printf("\n");

    }

}

int main()
{
    //创建SDK上下文
    dmcan_context* ctx=nullptr;
    dmcan_context_create(&ctx);

    if (!ctx)
    {
        std::cout << "dmcan_context_create failed" << std::endl;
        return -1;
    }

    //打印SDK版本号
    dmcan_print_version(ctx);

    //查找设备数量
    int dev_cnt= dmcan_find_devices(ctx);
    std::cout << "device found cnt:" <<dev_cnt<< std::endl;

    //显示设备列表
    dmcan_show_all_devices(ctx);

    //获取0号设备
    dmcan_device_get(ctx,&u2canfd_dev,0);    //USB2CANFD

    //获取1号设备
    dmcan_device_get(ctx,&u2canfd_dual_dev,1);    //USB2CANFD_DUAL

    //打开设备
    bool dev0_open_ret=dmcan_device_open(u2canfd_dev);
    bool dev1_open_ret=dmcan_device_open(u2canfd_dual_dev);

    if (!dev0_open_ret)
    {
        std::cout << "index 0 device open failed" << std::endl;
    }

    if (!dev1_open_ret)
    {
        std::cout << "index 0 device open failed" << std::endl;
    }


    //配置设备0并测试发送 USB2CANFD
    if (dev0_open_ret)
    {
        dmcan_device_print_version(u2canfd_dev); //打印设备版本信息
        dmcan_device_enable_channel(u2canfd_dev,0); //打开can通道

        //获取设备通道波特率信息
        dmcan_channel_can_info_t can_info={0};
        bool ret=dmcan_device_get_channel_baudrate(u2canfd_dev,0,&can_info);
        if (ret)
        {
            can_info.canfd=true;
            can_info.can_baudrate=500000;
            can_info.canfd_baudrate=2000000;
            can_info.can_sp=0.75;
            can_info.canfd_sp=0.75;

            ret=dmcan_device_set_channel_baudrate(u2canfd_dev,0,can_info); //设置波特率参数 也可以通过dmcan_ch_can_config_t结构体设置更详细的波特率参数
            if (ret)
                std::cout << "index 0 device set channel baudrate successfully" << std::endl;
        }

        dmcan_device_hook_sent_callback(u2canfd_dev,sent_frame_handle); //注册发送回调
        dmcan_device_hook_recv_callback(u2canfd_dev,recv_frame_handle); //注册接收回调

        //单包发送
        dmcan_device_send_can(u2canfd_dev,0,0x123,true,false,false,true,8,(uint8_t*)"abcdefgh");

        //队列发送
        // dmcan_device_fill_can_queue(u2canfd_dev,0,0x123,true,false,false,true,8,(uint8_t*)"abcdefgh");
        // dmcan_device_can_queue_send(u2canfd_dev);

        dmcan_device_close(u2canfd_dev); //可以单独关闭设备  也可以通过dmcan_context_destroy()销毁上下文时自动关闭设备
    }



    //配置设备1并测试发送 USB2CANFD_DUAL
    if (dev1_open_ret)
    {
        dmcan_device_print_version(u2canfd_dual_dev); //打印设备版本信息
        dmcan_device_enable_channel(u2canfd_dual_dev,0);  //打开can通道0
        dmcan_device_enable_channel(u2canfd_dual_dev,1);  //打开can通道1
        //获取设备通道波特率信息
        dmcan_channel_can_info_t can_info={0};
        bool ret=dmcan_device_get_channel_baudrate(u2canfd_dual_dev,0,&can_info);
        if (ret)
        {
            can_info.canfd=true;
            can_info.can_baudrate=500000;
            can_info.canfd_baudrate=2000000;
            can_info.can_sp=0.75;
            can_info.canfd_sp=0.75;

            ret=dmcan_device_set_channel_baudrate(u2canfd_dual_dev,0,can_info); //设置波特率参数 也可以通过dmcan_ch_can_config_t结构体设置更详细的波特率参数
            if (ret)
                std::cout << "index 0 device set channel baudrate successfully" << std::endl;
        }

        dmcan_device_hook_sent_callback(u2canfd_dual_dev,sent_frame_handle); //注册发送回调
        dmcan_device_hook_recv_callback(u2canfd_dual_dev,recv_frame_handle); //注册接收回调


        //单包发送
        dmcan_device_send_can(u2canfd_dual_dev,0,0x123,true,false,false,true,8,(uint8_t*)"abcdefgh");

        //队列发送
        // dmcan_device_fill_can_queue(u2canfd_dual_dev,0,0x123,true,false,false,true,8,(uint8_t*)"abcdefgh");
        // dmcan_device_can_queue_send(u2canfd_dual_dev);

        dmcan_device_close(u2canfd_dual_dev); //可以单独关闭设备  也可以通过dmcan_context_destroy()销毁上下文时自动关闭设备
    }

    dmcan_context_destroy(ctx); //销毁SDK上下文  会自动关闭设备

    return 0;

}
