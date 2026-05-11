
using DM_DeviceSDK_CSharp.core;


//创建上下文
DmCanContext context = new DmCanContext();

//打印SDK版本
context.PrintVersion();

//查找所有可用设备 包括 usb2canfd单路 双路模块  link4c模块
context.FindAllDevice();

//也可以使用带类型的查找
//context.FindAllDevice(dmcan_device_type.USB2CANFD);

//打印所有模块信息
context.ShowAllDevices();

//获取指定索引的设备
var device = context.GetDevice(0);

if (device == null)
    return;

//打开设备
device.Open();

//打印设备版本
device.PrintVersion();

//打印设备详细信息 包括 PID VID SN 类型等信息
Console.WriteLine(device.ToString());

//开启通道
device.EnableChannel(0,true);
device.EnableChannel(1,true);

dmcan_ch_can_info channel_info=default;
dmcan_ch_can_config channel_config = default;

//获取设备通道波特率的简略信息
device.GetChannelBaudInfo(0,ref channel_info);

//获取设备通道波特率的详细信息
device.GetChannelBaudConfig(0,ref channel_config);

//修改通道波特率 可以直接那简略配置进行修改
channel_info.canfd = true;
channel_info.can_baudrate = 500000; //经典模式下对应can波特率 canfd模式下对应仲裁域波特率
channel_info.canfd_baudrate = 2000000; //加速域波特率
channel_info.can_sp = 0.75f;
channel_info.canfd_sp = 0.75f;
device.SetChannelBaudByInfo(0,channel_info);
//类似的详细配置也可以进行一样的操作 配置完之后调用下面的函数
//device.SetChannelBaudByConfig(0,channel_config);


//订阅发送回调事件  以及 接收回调事件
device.ReceiveEchoEvent += OnReceive;
device.SendEchoEvent += OnSend;

void OnSend(CanRxEventArgs args)
{
    //将发送的帧打印出来
    Console.WriteLine(args.Frame.ToString());
}

void OnReceive(CanRxEventArgs args)
{
    //将发送的帧打印出来
    Console.WriteLine(args.Frame.ToString());
}


while (true) 
{
    //发送can帧 
    device.Send(0, 0x111, true, false, false, true, 8, new byte[] { 1, 2, 3, 4, 5, 6, 7, 8 });

    Thread.Sleep(1000);
}