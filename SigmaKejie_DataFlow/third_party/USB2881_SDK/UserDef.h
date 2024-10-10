#include "USB2881.h"  // 包含驱动程序接口
#include "ADThread.h"
//#############################################################
// 定义用于控制数据位数及量程的常数
#define LSB_COUNT  65536.0          // LSB码数量
#define LSB_HALF   32768            // 当取偏移码时，其原点位置
#define VOLT_RANGE 10000.0         // 量程（mV）(注意必须带小数)

#define MAX_LSB 0xFFFF         


// 定义其他常量
#define DeviceName "USB2881"       // 定义设备名
#define MAX_FREQUENCE     20000000 // 最高频率Hz
#define MAX_CHANNEL_COUNT 12
#define MAX_SEGMENT       128
#define MAX_RAM_SIZE 16384 // 每通道RAM的最大容量(点或字)

#define MAX_SEGMENT_DMA    32


//#############################################################
// Header define
#define PCI_BUS     0x01
#define USB_BUS     0x02
#define ISA_BUS     0x03
#define PC104_BUS   0x04
#define PXI_BUS     0x05
#define PCIE_BUS     0x06

#define NPT_MODE    0x00 // 非空采集
#define HALF_MODE	0x01 // 半满采集
#define INT_MODE	0x02 // 中断采集
#define DMA_MODE	0x03 // DMA采集

#define AD_FILE_TYPE 0x00  // 存储AD数据的文件
#define DA_FILE_TYPE 0x01  // 存储DA数据的文件
#define DIO_FILE_TYPE 0x02 // 存储DIO数据的文件

#define DEFAULT_FILE_TYPE  AD_FILE_TYPE
#define DEFAULT_BUS_TYPE   USB_BUS    // 设备类型(PXI)
#define DEFAULT_DEVICE_NUM 0x2881     // 设备编号(8532)

#define DEFAULT_VOLTBOTTOM_RANGE -50000
#define DEFAULT_VOLTTOP_RANGE     5000

#define DEFAULT_DATA_WIDTH 0x16     // 设备的精度(16位)

#define HEAD_END_FLAG  0xAA55AA55

#define DATA_MODE 1 // 数字显示
#define WAVE_MODE 2 // 波形显示
#define SAVE_MODE 3 // 存盘处理

// 数字窗口显示模式
#define DEC_DATAMODE   0 // 十进制显示
#define HEX_DATAMODE   1 // 十六进制显示
#define VOLT_DATAMODE  2 // 电压值显示

typedef struct _FILE_HEADER
{
	LONG HeadSizeBytes;		// 文件头信息长度
	LONG FileType;  
	// 该设备数据文件共有的成员
	LONG BusType;				// 设备总线类型(DEFAULT_BUS_TYPE)
	LONG DeviceNum;				// 该设备的编号(DEFAULT_DEVICE_NUM)
	LONG VoltBottomRange;       // 量程下限(mV)
	LONG VoltTopRange;	        // 量程上限(mV)	
	
	LONG ChannelCount;          // 通道总数
	LONG DataWidth;             // 设备的精度(分辩率)
	LONG bXorHighBit;           // 是否高位求反(为1则求反)     
	USB2881_PARA_AD ADPara;     // 保存硬件参数
	USB2881_STATUS_AD ADStatus;     // 保存硬件参数
	LONG CrystalFreq;           // 晶振频率
	LONG ChannelNum;            // 通道号
	LONG HeadEndFlag;			// 头信息结束位
} FILE_HEADER, *PFILE_HEADER;


//#############################################################
// external declare
 extern USHORT ADBuffer[MAX_CHANNEL_COUNT][MAX_SEGMENT][MAX_RAM_SIZE]; // 缓冲队列
//extern PWORD ADBuffer[MAX_CHANNEL_COUNT][MAX_SEGMENT]; // 缓冲队列
extern USB2881_PARA_AD ADPara;
extern USB2881_PARA_AD OldADPara;
extern USB2881_STATUS_AD pADStatus;
extern int  gl_ScreenVolume;
extern int  m_TriggerVolt;
extern int WavePointX;
extern int CurrentIndex;
extern HANDLE hDevice;
extern CEdit* pCountEdit;
extern BOOL m_BounceStop;
extern BOOL m_AnalyzeCheck;
extern BOOL m_bCreateDevice;
extern int glOffset;
extern int glBufferID;
extern int m_BounceCount;
extern float m_AnalyzeAllCount;
extern UINT m_OverLimitCount;
extern BOOL m_bProgress;
extern float Center, PerY;
extern CRect ClientRect;
extern float middle1;
extern BOOL bDrawing;
extern int nChannelArrayClient[12];
extern int DisplayChannel[12];
extern BOOL m_bFirstCreateDeviceFile;
extern int  m_ProcessDigitalMode;
extern int  m_ProcessGraphicMode;
extern int  m_AnalyzeChannel;
extern int m_UpperLimit;
extern int m_LowerLimit;
extern CEdit* pRangeEdit;
extern CEdit* pRatioEdit;
extern int m_ProcessMoveVolt;
extern int nProcessMode;
extern BOOL bBunch;
extern int m_DigitalDataShowMode;
extern SHORT InUserRegion[MAX_SEGMENT][16384];
extern BOOL gl_bAutoCollect;
extern float OutAnalogValve;
extern int ReadIndex;    
extern HANDLE hFileObject[12];
extern HANDLE hEvent;  
extern HANDLE hExitEvent;
extern BOOL gl_bDeviceADRun;
extern BOOL bCreateDevice;
extern ULONGLONG FileLenghtWords;
extern BOOL gl_Event;
extern BOOL gl_bCollected;
extern BOOL m_bCloseFile;
extern int gl_MiddleLsb[MAX_CHANNEL_COUNT];
extern int gl_nSampleMode;
extern BOOL gl_bTileWave;
extern ULONG gl_nSampleLenth;
extern float gl_PerLsbVoltM[12];
extern LONG gl_nTriggerPos;
extern ULONG gl_StartAddr;
extern int gl_nChannelCount;
extern BOOL	gl_bSmlThenSize;

extern ULONG gl_ReadSizeWords;
#define MsgBox AfxMessageBox
//CString strMsg;
extern CString g_strFileFullName[12];
extern USB2881_STATUS_AD gl_ADStatus;
extern ULONG gl_ChannelReadSize[12];


extern ULONG gl_ChannelArrary[MAX_CHANNEL_COUNT];
