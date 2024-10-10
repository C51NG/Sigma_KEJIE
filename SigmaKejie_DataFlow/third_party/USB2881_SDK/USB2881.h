#ifndef _USB2881_DEVICE_
#define _USB2881_DEVICE_

#include<windows.h>

#define USB2881_MAX_AI_CHANNELS	12 // 本卡最多支持12路模拟量单端输入通道

//***********************************************************
// 用于AD采集的参数结构	
typedef struct _USB2881_PARA_AD
{
	LONG bChannelArray[USB2881_MAX_AI_CHANNELS];	// 采样通道选择阵列，分别控制6个通道，=TRUE表示该通道采样，否则不采样	
	LONG InputRange[USB2881_MAX_AI_CHANNELS];		// 模拟量输入量程选择(前六个通道量程必须一致，后六个通道量程必须一致)
	LONG Gains[USB2881_MAX_AI_CHANNELS];			// 增益控制
	LONG Frequency;         // 采集频率,单位为Hz, [1000, 250000]
	LONG TriggerMode;		// 触发模式选择
	LONG TriggerSource;		// 触发源选择
	LONG TriggerDir;		// 触发方向选择(正向/负向触发)
	LONG TrigLevelVolt;		// 触发电平(±10000mV)
	LONG TrigWindow;		// 触发灵敏度设置uS,[50,1638]
	LONG ClockSource;		// 时钟源选择(内/外时钟源)
	LONG bClockOutput;      // 允许时钟输出到CLKOUT,=TRUE:允许时钟输出, =FALSE:禁止时钟输出
} USB2881_PARA_AD, *PUSB2881_PARA_AD;

//***********************************************************
// AD参数USB2881_PARA_AD中的Gains[x]使用的硬件增益选项
const long USB2881_GAINS_1MULT			= 0x00; // 1倍增益
const long USB2881_GAINS_2MULT			= 0x01; // 2倍增益
const long USB2881_GAINS_4MULT			= 0x02; // 4倍增益
const long USB2881_GAINS_8MULT			= 0x03; // 8倍增益

//***********************************************************
// AD硬件参数USB2881_PARA_AD中的InputRange量程所使用的选项
const long USB2881_INPUT_N10000_P10000mV= 0x00; // ±10000mV
const long USB2881_INPUT_N5000_P5000mV	= 0x01; // ±5000mV

//***********************************************************
// AD硬件参数USB2881_PARA_AD中的TriggerMode成员变量所使用触发模式选项
const long USB2881_TRIGMODE_EDGE		= 0x00; // 边沿触发
const long USB2881_TRIGMODE_PULSE		= 0x01; // 电平触发

//***********************************************************
// AD硬件参数USB2881_PARA_AD中的TriggerSource触发源信号所使用的选项
const long USB2881_TRIGMODE_SOFT		= 0x00; // 软件触发
const long USB2881_TRIGSRC_DTR			= 0x01; // 选择DTR作为触发源
const long USB2881_TRIGSRC_ATR			= 0x02; // 选择ATR作为触发源
const long USB2881_TRIGSRC_TRIGGER		= 0x03; // Trigger信号触发（用于多卡同步）

//***********************************************************
// AD硬件参数USB2881_PARA_AD中的TriggerDir触发方向所使用的选项
const long USB2881_TRIGDIR_NEGATIVE		= 0x00; // 负向触发(低电平/下降沿触发)
const long USB2881_TRIGDIR_POSITIVE		= 0x01; // 正向触发(高电平/上升沿触发)
const long USB2881_TRIGDIR_POSIT_NEGAT	= 0x02; // 正负向触发(高/低电平或上升/下降沿触发)

//***********************************************************
// AD硬件参数USB2881_PARA_AD中的ClockSource时钟源所使用的选项
const long USB2881_CLOCKSRC_IN			= 0x00; // 内部时钟定时触发
const long USB2881_CLOCKSRC_OUT			= 0x01; // 外部时钟定时触发(使用CN1上的CLKIN信号输入)

//*************************************************************************************
// 用于AD采样的实际硬件参数
typedef struct _USB2881_STATUS_AD     
{
	LONG bADEanble;	// AD是否已经使能，=TRUE:表示已使能，=FALSE:表示未使能
	LONG bTrigger;  // AD是否被触发，=TRUE:表示已被触发，=FALSE:表示未被触发
	LONG bHalf;		// 采集数据是否已达半满，=TRUE:表示已半满，=FALSE:表示未半满
} USB2881_STATUS_AD, *PUSB2881_STATUS_AD;

//***********************************************************
// 驱动函数接口
#ifndef _USB2881_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## 常规通用函数 #################################
	HANDLE DEVAPI FAR PASCAL USB2881_CreateDevice(int DeviceLgcID = 0); // 创建设备对象(该函数使用系统内逻辑设备ID）
	HANDLE DEVAPI FAR PASCAL USB2881_CreateDeviceEx(int DevicePhysID); // 使用物理ID创建设备对象
	BOOL DEVAPI FAR PASCAL USB2881_GetDeviceCurrentID(HANDLE hDevice, PLONG DevicePhysID); // 取得当前设备的物理ID号
	BOOL DEVAPI FAR PASCAL USB2881_SetDevicePhysID(HANDLE hDevice, LONG DevicePhysID); // 设置当前设备的物理ID号,物理ID[0~255],需重新上电
	BOOL DEVAPI FAR PASCAL USB2881_ResetDevice(HANDLE hDevice);		 // 复位整个USB设备
    BOOL DEVAPI FAR PASCAL USB2881_ReleaseDevice(HANDLE hDevice);    // 设备句柄

	//####################### AD数据读取函数 #################################
	BOOL DEVAPI FAR PASCAL USB2881_ADCalibration(				// AD自动校准函数
									HANDLE hDevice);			// 设备对象句柄,它由CreateDevice函数创建
	
    BOOL DEVAPI FAR PASCAL USB2881_InitDeviceAD(				// 初始化设备,当返回TRUE后,设备即刻开始传输.
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建
									PUSB2881_PARA_AD pADPara);  // 硬件参数, 它仅在此函数中决定硬件状态	

	BOOL DEVAPI FAR PASCAL USB2881_StartDeviceAD(				// 在初始化之后，启动设备
									HANDLE hDevice);			// 设备对象句柄

	BOOL DEVAPI FAR PASCAL USB2881_StopDeviceAD(				// 在启动设备之后，暂停设备
									HANDLE hDevice);			// 设备对象句柄

    BOOL DEVAPI FAR PASCAL USB2881_ReadDeviceAD(				// 初始化设备后，即可用此函数读取设备上的AD数据
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建
									USHORT ADBuffer[],			// 将用于接受数据的用户缓冲区
									LONG nReadSizeWords,		// 读取AD数据的长度(字)
									PLONG nRetSizeWords,		// 实际返回数据的长度(字),
									BOOL bEnoughRtn = TRUE,		// TRUE：不够读取点数返回0；FALSE:不够读取点数返回实际点数
									PLONG nSurplusWords = NULL);// 返回FIFO剩余点数


    BOOL DEVAPI FAR PASCAL USB2881_ReleaseDeviceAD( HANDLE hDevice); // 停止AD采集，释放AD对象所占资源

   	//################# AD的硬件参数操作函数 ########################	
	BOOL DEVAPI FAR PASCAL USB2881_SaveParaAD(HANDLE hDevice, PUSB2881_PARA_AD pADPara);  
    BOOL DEVAPI FAR PASCAL USB2881_LoadParaAD(HANDLE hDevice, PUSB2881_PARA_AD pADPara);
    BOOL DEVAPI FAR PASCAL USB2881_ResetParaAD(HANDLE hDevice, PUSB2881_PARA_AD pADPara); // 将AD采样参数恢复至出厂默认值

	//####################### 数字I/O输入输出函数 #################################
	BOOL DEVAPI FAR PASCAL USB2881_GetDeviceDI(				// 取得开关量状态     
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建								        
									BYTE bDISts[16]);			// 开关输入状态(注意: 必须定义为16个字节元素的数组)

	BOOL DEVAPI FAR PASCAL USB2881_SetDeviceDO(				// 输出开关量状态
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建								        
									BYTE bDOSts[16]);			// 开关输出状态(注意: 必须定义为16个字节元素的数组)

	//############################################################################
	BOOL DEVAPI FAR PASCAL USB2881_GetDevVersion(				// 获取设备固件及程序版本
									HANDLE hDevice,				// 设备对象句柄,它由CreateDevice函数创建
									PULONG pulFmwVersion,		// 固件版本
									PULONG pulDriverVersion);	// 驱动版本


	//############################ 线程操作函数 ################################
	HANDLE DEVAPI FAR PASCAL USB2881_CreateSystemEvent(void); 	// 创建内核系统事件对象
	BOOL DEVAPI FAR PASCAL USB2881_ReleaseSystemEvent(HANDLE hEvent); // 释放内核事件对象

#ifdef __cplusplus
}
#endif

// 自动包含驱动函数导入库
#ifndef _USB2881_DRIVER_
#ifndef _WIN64
#pragma comment(lib, "USB2881_32.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with USB2881_32.dll...")
#pragma message("======== Successfully linked with USB2881_32.dll")
#else
#pragma comment(lib, "USB2881_64.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with USB2881_64.dll...")
#pragma message("======== Successfully linked with USB2881_64.dll")
#endif

#endif

#endif; // _USB2881_DEVICE_