#include <cstdio>
#include <thread>

#include "pub_user.h"

//发送帧回传回调函数
void sent_callback(usb_rx_frame_t* frame)
{

    printf("sent callback , packet id:%x\n",frame->head.can_id);
}

//接收帧回传回调函数
void rec_callback(usb_rx_frame_t* frame)
{

    printf("rec callback , packet id:%x\n",frame->head.can_id);
}

int main()
{
    //初始化模块句柄
    damiao_handle* handle=damiao_handle_create(DEV_USB2CANFD_DUAL);

    //打印sdk版本信息
    damiao_print_version(handle);

    //查找对应类型模块的设备数量
    int device_cnt=damiao_handle_find_devices(handle);

    if (device_cnt==0)
    {
        printf("no device found !\n");
        damiao_handle_destroy(handle);
        return -1;
    }

    int handle_cnt=0;

    device_handle* dev_list[16];
    //获取设备信息
    damiao_handle_get_devices(handle,dev_list,&handle_cnt);

    //打开设备
    if (device_open(dev_list[0]))
    {
        printf("device opened !\n");
    }
    else
    {
        printf("open device failed !\n");
        damiao_handle_destroy(handle);
        return -1;
    }

    char strBuf[255]={0};
    //获取模块版本信息
    device_get_version(dev_list[0],strBuf,sizeof(strBuf));
    printf("device version:%s\n",strBuf);
    //获取序列号信息
    device_get_serial_number(dev_list[0],strBuf,sizeof(strBuf));
    printf("device sn:%s\n",strBuf);


    //设置通道波特率
    device_channel_set_baud_with_sp(dev_list[0],0,true,1000000,5000000,0.75f,0.75f);
    device_baud_t baud={0};
    //获取通道波特率
    if (device_channel_get_baudrate(dev_list[0],0,&baud))
    {
        printf("===can channel baudrate info===\n");
        printf("can_baud:%d  \n",baud.can_baudrate);
        printf("canfd_baud:%d  \n",baud.canfd_baudrate);
        printf("can_sp:%f  \n",baud.can_sp);
        printf("canfd_sp:%f  \n",baud.canfd_sp);

    }
    
    device_channel_set_baud_with_sp(dev_list[0],1,true,1000000,5000000,0.75f,0.75f);

    //开启can通道
    device_open_channel(dev_list[0],0);
    device_open_channel(dev_list[0],1);

    uint8_t payload[8]={1,2,3,4,5,6,7,8};

    //发送钩子函数注册
    device_hook_to_sent(dev_list[0],sent_callback);
    //接收钩子函数注册
    device_hook_to_rec(dev_list[0],rec_callback);

    uint8_t count=0;

    while (count<100)
    {
        //此下注释代码请确保模块处于某个CAN总线中使用
        // device_channel_send_fast(dev_list[0],0,0x123123,1,true,true,true,8,payload);
        // device_channel_send_fast(dev_list[0],1,0x123123,1,true,true,true,8,payload);
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // payload[0]++;
        // count++;
    }

    //关闭can通道
    device_close_channel(dev_list[0],0);
    device_close_channel(dev_list[0],1);

    //关闭设备
    device_close(dev_list[0]);

    //销毁模块句柄
    damiao_handle_destroy(handle);

    return 0;
}