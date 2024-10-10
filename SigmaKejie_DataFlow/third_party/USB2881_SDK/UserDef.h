#include "USB2881.h"  // ������������ӿ�
#include "ADThread.h"
//#############################################################
// �������ڿ�������λ�������̵ĳ���
#define LSB_COUNT  65536.0          // LSB������
#define LSB_HALF   32768            // ��ȡƫ����ʱ����ԭ��λ��
#define VOLT_RANGE 10000.0         // ���̣�mV��(ע������С��)

#define MAX_LSB 0xFFFF         


// ������������
#define DeviceName "USB2881"       // �����豸��
#define MAX_FREQUENCE     20000000 // ���Ƶ��Hz
#define MAX_CHANNEL_COUNT 12
#define MAX_SEGMENT       128
#define MAX_RAM_SIZE 16384 // ÿͨ��RAM���������(�����)

#define MAX_SEGMENT_DMA    32


//#############################################################
// Header define
#define PCI_BUS     0x01
#define USB_BUS     0x02
#define ISA_BUS     0x03
#define PC104_BUS   0x04
#define PXI_BUS     0x05
#define PCIE_BUS     0x06

#define NPT_MODE    0x00 // �ǿղɼ�
#define HALF_MODE	0x01 // �����ɼ�
#define INT_MODE	0x02 // �жϲɼ�
#define DMA_MODE	0x03 // DMA�ɼ�

#define AD_FILE_TYPE 0x00  // �洢AD���ݵ��ļ�
#define DA_FILE_TYPE 0x01  // �洢DA���ݵ��ļ�
#define DIO_FILE_TYPE 0x02 // �洢DIO���ݵ��ļ�

#define DEFAULT_FILE_TYPE  AD_FILE_TYPE
#define DEFAULT_BUS_TYPE   USB_BUS    // �豸����(PXI)
#define DEFAULT_DEVICE_NUM 0x2881     // �豸���(8532)

#define DEFAULT_VOLTBOTTOM_RANGE -50000
#define DEFAULT_VOLTTOP_RANGE     5000

#define DEFAULT_DATA_WIDTH 0x16     // �豸�ľ���(16λ)

#define HEAD_END_FLAG  0xAA55AA55

#define DATA_MODE 1 // ������ʾ
#define WAVE_MODE 2 // ������ʾ
#define SAVE_MODE 3 // ���̴���

// ���ִ�����ʾģʽ
#define DEC_DATAMODE   0 // ʮ������ʾ
#define HEX_DATAMODE   1 // ʮ��������ʾ
#define VOLT_DATAMODE  2 // ��ѹֵ��ʾ

typedef struct _FILE_HEADER
{
	LONG HeadSizeBytes;		// �ļ�ͷ��Ϣ����
	LONG FileType;  
	// ���豸�����ļ����еĳ�Ա
	LONG BusType;				// �豸��������(DEFAULT_BUS_TYPE)
	LONG DeviceNum;				// ���豸�ı��(DEFAULT_DEVICE_NUM)
	LONG VoltBottomRange;       // ��������(mV)
	LONG VoltTopRange;	        // ��������(mV)	
	
	LONG ChannelCount;          // ͨ������
	LONG DataWidth;             // �豸�ľ���(�ֱ���)
	LONG bXorHighBit;           // �Ƿ��λ��(Ϊ1����)     
	USB2881_PARA_AD ADPara;     // ����Ӳ������
	USB2881_STATUS_AD ADStatus;     // ����Ӳ������
	LONG CrystalFreq;           // ����Ƶ��
	LONG ChannelNum;            // ͨ����
	LONG HeadEndFlag;			// ͷ��Ϣ����λ
} FILE_HEADER, *PFILE_HEADER;


//#############################################################
// external declare
 extern USHORT ADBuffer[MAX_CHANNEL_COUNT][MAX_SEGMENT][MAX_RAM_SIZE]; // �������
//extern PWORD ADBuffer[MAX_CHANNEL_COUNT][MAX_SEGMENT]; // �������
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
