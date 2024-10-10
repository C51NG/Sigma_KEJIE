// ADDoc.cpp : implementation of the CADDoc class
//

#include "stdafx.h"
#include "Sys.h"
#include "ADDoc.h"
#include "ADDigitView.h"
#include "ADWaveView.h"
#include "ADStatusView.h"
#include "ParaCfgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#ifdef USB2881_CreateSystemEvent
#define USB2881_CreateSystemEvent USB2881_TCreateSystemEvent 
#endif
////////////////////////////////////////////////////////////////////////
// WORD ADBuffer[MAX_CHANNEL_COUNT][MAX_SEGMENT][MAX_RAM_SIZE]; // 缓冲队列
BOOL gl_bAutoCollect = FALSE;
int CurrentIndex; // AD数据处理线程当前缓冲区索引号
int ReadIndex;    // AD数据采集线程当前缓冲区索引号
BOOL gl_bDeviceADRun=FALSE;
USB2881_PARA_AD ADPara;
USB2881_PARA_AD OldADPara;
USB2881_STATUS_AD gl_ADStatus;
//BOOL bCheckOverflow=FALSE; // 是否检测FIFO溢出标志
int  gl_nSampleMode = NPT_MODE;            // 采集模式(1,查询， 2, 中断， 3， DMA)
UINT ReadDataThread(PVOID hWnd);//数据线程
UINT ProccessDataThread(PVOID hWnd);  // 绘制数据线程

BOOL m_bFirstCreateDeviceFile;
int  m_ProcessGraphicMode;//为1,波形显示
BOOL m_AnalyzeCheck;//控制精度分析
int  gl_ScreenVolume;//设置屏幕显示的量程
int  m_TriggerVolt;//触发电压
int gl_MiddleLsb[MAX_CHANNEL_COUNT];
float m_AnalyzeAllCount;
UINT m_OverLimitCount;
int m_ProcessMoveVolt;//为1时,平移电压
BOOL bCreateDevice;
BOOL m_bProgress;  // 是否更新进度条
int nProcessMode;  // 数据处理方式 1：数字显示  2：波形显示  3：数据存盘
CEdit* pCountEdit;
CEdit* pRangeEdit;
CEdit* pRatioEdit;
CADDigitView* m_pDigitView;
CADWaveView* m_pWaveView;
CADStatusView* pADStatusView = NULL;
CParaCfgView* pParaCfgView = NULL;
HANDLE hEvent;  // 采集线程与绘制线程的同步信号
int m_DigitalDataShowMode; // 数字窗口显示模式
int DeviceID;   // 该程序使用的设备ID
HANDLE hFileObject[12];

extern BOOL gl_bSegMentFlag[MAX_SEGMENT];
extern LONG gl_lSegSaveSize[MAX_SEGMENT];

int gl_nChannelCount = MAX_CHANNEL_COUNT;

BOOL m_bCloseFile;
HANDLE hExitEvent;
float Center, PerY;
CRect ClientRect;
float middle1;
BOOL bDrawing=FALSE;

ULONGLONG FileLenghtWords;
int nChannelArrayClient[12];// 通道号
int gl_CurrentbufNum = 0; // 当前的缓冲索引号
int DisplayChannel[12];// 用于标志各通道是否显示
float OutAnalogValve; // 表示模拟外触发的全局变量
ULONG gl_nSampleLenth = MAX_RAM_SIZE;   // 采样长度
int  gl_InputRange[12];    // 各通道设置的电压量程范围
float gl_PerLsbVoltM[12]; // 单位LSB的电压值
CString g_strFileFullName[12];
ULONG gl_StartAddr;
//PWORD ADBuffer[MAX_CHANNEL_COUNT][MAX_SEGMENT]; // 缓冲队列
USHORT ADBuffer[MAX_CHANNEL_COUNT][MAX_SEGMENT][MAX_RAM_SIZE]; // 缓冲队列

//##############################################
//使用多媒体定时器以每秒50帧或以下的速度刷新屏幕
#include "stdio.h"
#include "MmSystem.h"

void PASCAL DrawWindowProc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);
#define ONE_MILLI_SECOND 1
#define TWO_SECOND 2000
// 定 义 时 钟 分 辨 率， 以 毫 秒 为 单 位 ---- 
#define TIMER_ACCURACY 1
UINT wAccuracy;  // 定 义 分 辨 率
UINT TimerID; // 定 义 定 时 器 句 柄 
ULONG gl_ChannelArrary[MAX_CHANNEL_COUNT];

////////////////////////////////////////////////////////////////////////////
// CADDoc

IMPLEMENT_DYNCREATE(CADDoc, CDocument)

BEGIN_MESSAGE_MAP(CADDoc, CDocument)
//{{AFX_MSG_MAP(CADDoc)
ON_COMMAND(IDM_TileWave, OnTileWave)
ON_COMMAND(IDM_SuperPoseWave, OnSuperPoseWave)
ON_UPDATE_COMMAND_UI(IDM_SuperPoseWave, OnUpdateSuperPoseWave)
ON_UPDATE_COMMAND_UI(IDM_TileWave, OnUpdateTileWave)
	//}}AFX_MSG_MAP

	/*ON_COMMAND(IDM_HALF_MODE, &CADDoc::OnHalfMode)
	ON_UPDATE_COMMAND_UI(IDM_HALF_MODE, &CADDoc::OnUpdateHalfMode)*/


END_MESSAGE_MAP()
///////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CADDoc construction/destruction

CADDoc::CADDoc()
{
	// TODO: add one-time construction code here
	m_bProgress=FALSE; // 一开始不更新进度条
	nProcessMode = DATA_MODE;    // 默认方式：数字显示
	m_TriggerVolt=(int)VOLT_RANGE;	// 置触发电平的初值为其最大量程
	m_bFirstCreateDeviceFile=TRUE;  // 一开始，为第一次创建文件对象
	m_bCloseFile=TRUE; // 没有打开文件时，认为文件被关闭	
	gl_nChannelCount=0;
	USB2881_LoadParaAD(NULL, &ADPara);  // 读入系统参数

	gl_nChannelCount = MAX_CHANNEL_COUNT;




	int a =0;
	int b = sizeof(int);

}

CADDoc::~CADDoc()
{
}

BOOL CADDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CADDoc serialization

void CADDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CADDoc diagnostics

#ifdef _DEBUG
void CADDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

BOOL gl_bTerminateDrawThr = TRUE;
void CADDoc::StartDeviceAD() 
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	////////////////////////////////////////////////////////////////////////
	if(pApp->m_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(L"对不起，无设备存在...",MB_ICONERROR);
		return;
	}
	if(!gl_bTerminateDrawThr)
	{
		AfxMessageBox(L" 线程没有中止");
		return;
	}
	FileLenghtWords = 256; // 文件长度初始化
	m_DrawWindowThread = AfxBeginThread(ProccessDataThread,NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_DrawWindowThread->m_bAutoDelete = TRUE; // 自动删除    
	
	// 创建数据采集与窗口绘制同步事件对象
	hEvent = USB2881_CreateSystemEvent();
	hExitEvent = USB2881_CreateSystemEvent();
	bCollectData=TRUE;  // 创建设备对象后，可以进行数据采集
	for(int i=0;i<MAX_CHANNEL_COUNT;i++)
		gl_MiddleLsb[i]=0;

	
	
	if(!m_bFirstCreateDeviceFile) // 如果创建了文件对象
	{
		for (int Index=0; Index<gl_nChannelCount; Index++)
		{
			header[Index].HeadSizeBytes = sizeof(FILE_HEADER);   // 头信息大小
			header[Index].FileType = DEFAULT_FILE_TYPE;
			header[Index].BusType = DEFAULT_BUS_TYPE;        // 该设备总线类型
			header[Index].DeviceNum = DEFAULT_DEVICE_NUM;	 // 设备ID号
			header[Index].ChannelCount = 1;                 // 每通道都是独立的，所以为1
			header[Index].DataWidth = DEFAULT_DATA_WIDTH;   // 12位
			header[Index].bXorHighBit = 0;                  // 高位不用求反
			
			// 还没有设置
		
		
					switch(ADPara.InputRange[Index])	// 电压量程
					{		
					case USB2881_INPUT_N10000_P10000mV: // ±1000毫伏
						header[Index].VoltTopRange = 10000;
						header[Index].VoltBottomRange = -10000;
						break;
					case USB2881_INPUT_N5000_P5000mV: // ±5000毫伏
						header[Index].VoltTopRange = 5000;
						header[Index].VoltBottomRange = -5000;
						break;
					}
		
		
			header[Index].ADPara = ADPara;
			header[Index].CrystalFreq = 80000000; // 晶振频率(80 MHz)
			header[Index].HeadEndFlag = HEAD_END_FLAG;
			header[Index].ChannelNum = gl_ChannelArrary[Index]; // 通道号
			header[Index].ADStatus = gl_ADStatus;
			
			// 写文件头

			DWORD dTmp;
			if (!WriteFile(hFileObject[Index], (WORD*)&header[Index], header[Index].HeadSizeBytes,
				&dTmp, NULL))
			{
				AfxMessageBox(L"error");
			}
		}
	}
	
	// 根据通道数，重新设置视图滚动范围
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(m_hWndDigit)); 
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // 取得子帧窗口句柄
	
	CProgressCtrl* pProgress=(CProgressCtrl*)(pShow->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));	
	pProgress->SetPos(0);  // 释放设备时，置进度条为起值
	

	// 启动设备
	if(!MyStartDeviceAD(pApp->m_hDevice)) // 启动设备,开始采集数据
	{
		AfxMessageBox(L"设备启动失败");
		return;
	}

	gl_bDeviceADRun = TRUE;
	if(pParaCfgView != NULL)
	{
		pParaCfgView->EnableWindows(FALSE); // 在设备开始运行时，禁止操作硬件参数
		pParaCfgView->StartAD();
	}
	m_DrawWindowThread->ResumeThread(); // 启动绘制窗口线程
}

BOOL gl_Event = FALSE;
//###################  绘制数据线程 ################################################3### 
UINT ProccessDataThread(PVOID hWnd)  
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADFrame *pFrm=pApp->m_ADFrame ;
	CADDoc* pDoc=pApp->m_pADDoc; 
	// 在线程中取得视图指针
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CString str;
	BOOL bFirst=TRUE;    
	ULONG WroteMB=0;  
	
	pDoc->m_Wrote8KWCounter=0;
	pDoc->m_WroteMB=0;
	CurrentIndex=0;
	LONG lTime = 0;
	LONG lBufferID = 0;
	LONG nADChannel;
	DWORD dTmp;
	int i=0;
	while(gl_bDeviceADRun)  // 循环处理AD数据
	{ 

		do{

			if(!gl_bDeviceADRun)
				goto ExitProcDataThread;
			Sleep(1);
		} while(gl_bSegMentFlag[lBufferID] == FALSE);
	//} while(WaitForSingleObject(hEvent, 100) != WAIT_OBJECT_0);
		gl_bTerminateDrawThr = FALSE;
		gl_Event = TRUE;
		switch(nProcessMode)  // 数据处理
		{
		case DATA_MODE: // 数字处理

			if (GetTickCount()-lTime>30)
			{
				::SendMessage(pDoc->m_hWndDigit, WM_SHOWDIGIT, NULL, NULL);	
				lTime = GetTickCount();
			}	

			while (gl_bSegMentFlag[lBufferID])
			{
				gl_bSegMentFlag[lBufferID] = FALSE;
				lBufferID++;
				if (lBufferID >= MAX_SEGMENT)
					lBufferID = 0;
			}
			m_bProgress=TRUE; // 使OnDraw函数能更新进度条	
			break; 
		case WAVE_MODE:  // 波形处理
			if (GetTickCount()-lTime>30)
			{
				::SendMessage(pDoc->m_hWndWave,WM_SHOWWAVE, NULL, NULL);
				lTime = GetTickCount();
			}	
			while (gl_bSegMentFlag[lBufferID])
			{
				gl_bSegMentFlag[lBufferID] = FALSE;
				lBufferID++;
				if (lBufferID >= MAX_SEGMENT)
					lBufferID = 0;
			}
			m_bProgress=TRUE; // 使OnDraw函数能更新进度条
			break;
		case SAVE_MODE: // 数据存盘处理	
			while (gl_bSegMentFlag[lBufferID])
			{
				for (nADChannel = 0; nADChannel<gl_nChannelCount; nADChannel++)
				{
					WriteFile(hFileObject[nADChannel], ADBuffer[nADChannel][lBufferID], gl_lSegSaveSize[lBufferID], &dTmp, NULL);
				}		
				FileLenghtWords = FileLenghtWords+gl_lSegSaveSize[lBufferID]*gl_nChannelCount;	// 存盘文件大小增加
				gl_bSegMentFlag[lBufferID] = FALSE;
				lBufferID++;
				if (lBufferID >= MAX_SEGMENT)
					lBufferID = 0;
			}
			WroteMB=(WroteMB+32)%1024;
			pFrm->m_Progress()->SetPos((int)WroteMB);					
			str.Format(L"%d", (FileLenghtWords/1024));
			i=str.GetLength()-3;
			while(i>0)
			{
				str.Insert(i, ',');
				i-=3;
			}
			str+=" Kb";
			pFrm->m_FileLenght()->SetWindowText(str);
			break;
		}//end switch  

	}//end while
ExitProcDataThread:
	gl_bTerminateDrawThr = TRUE; // 线程中止
	return TRUE;
}
//##########################################################################################

void CADDoc::Public_ReleaseDevice(void) // 供MDI窗口调用
{
	StopDeviceAD();
}

void CADDoc::StopDeviceAD() 
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
//	gl_bDeviceADRun = FALSE; // 停止子线程进行连续数据采集	
	
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // 取得子帧窗口句柄
	if(pParaCfgView != NULL)
	{
		pParaCfgView->EnableWindows(TRUE);	

	}
	
	if(!MyStopDeviceAD(pApp->m_hDevice)) // 关闭AD设备
	{
		AfxMessageBox(L"关闭AD设备失败!");
		return;
	}

	WaitForSingleObject(hExitEvent, 100);  // 等待采集线程退出
	USB2881_ReleaseSystemEvent(hEvent);	   // 释放消息
	// 终止采集线程
	CProgressCtrl* pProgress=(CProgressCtrl*)(pShow->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
	pProgress->SetPos(0);  // 释放设备时，置进度条为起值
	
	// 在设备对象释放后，允许操作硬件参数
	/*if(pParaCfgView != NULL)
	{
		pParaCfgView->EnableWindows(TRUE);	

	}*/
	bCreateDevice = FALSE;     // 释放设备后，视设备未创建
	
	if(nProcessMode == SAVE_MODE)  // 如果数据处理方式为存盘
	{
		
		OnCloseFile();//释放文件
		nProcessMode = DATA_MODE; // 置成数字方式	
		if(pADStatusView != NULL)
		{
			CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
			pDataSave->EnableWindow(FALSE);  // 使存盘方式单选框有效

		}
	}


	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();	
	pWaveView->OnDrawPolyLine();
	/*CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(m_hWndDigit));
	pDigitView->Invalidate(); */
	
}


void CADDoc::OnDataFileNew() 
{
	// TODO: Add your command handler code here
	CString strNewFileName;
	CString TempFileName;
	CSysApp* pApp=(CSysApp*)AfxGetApp();
	if (!(pApp->DoPromptFileName(strNewFileName, IDS_NEW_FILE,
		OFN_HIDEREADONLY | OFN_CREATEPROMPT, TRUE, NULL)))
		return;
	if(!m_bFirstCreateDeviceFile)
	{  // 如果不是第一次创建文件对象,则释放老文件对象
		for (int nCH=0; nCH<gl_nChannelCount; nCH++)
		{			

			if ( ! CloseHandle( hFileObject[nCH] ) )
			{
				AfxMessageBox(L"释放文件对象失败...",MB_ICONSTOP,0);
			}
		}
	} 
	
	// 创建新文件对象

		int FileNameLength=strNewFileName.GetLength();
		TempFileName=strNewFileName.Left(FileNameLength-4);
	for (int nCH=0; nCH<gl_nChannelCount; nCH++)
	{		
		CString str;
		str.Format(L"-%d.usb", gl_ChannelArrary[nCH]);
		strNewFileName = TempFileName + str;
		hFileObject[nCH] =CreateFile(strNewFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);

		if ( hFileObject[nCH] == INVALID_HANDLE_VALUE )		
		{
			AfxMessageBox(L"初始化文件对象失败...",MB_ICONSTOP,0);
			return;
		}	
		// 保存打开的文件名称
		g_strFileFullName[nCH] = strNewFileName;

	}


	
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // 取得子帧窗口句柄
	pShow->ShowHideDlgSave ();	//显示存盘栏
	pShow->ShowHideStatus(FALSE);//隐藏状态栏
	if(pADStatusView != NULL)
	{
		CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
		pDataSave->EnableWindow(TRUE);  // 使存盘方式单选框有效
	}

	ULARGE_INTEGER  uLarge;
	GetDiskFreeSpaceEx(strNewFileName.Left(3),NULL,NULL,&uLarge);  // 取得文件所在磁盘可用空间

	ULONGLONG DiskFreeBytes=uLarge.QuadPart;	
	m_RemainMB=(ULONG)(DiskFreeBytes/(1024*1024));  // 求出有多少M字	
	CString str;
	
	if(m_RemainMB<2) 
	{
		AfxMessageBox(L"硬盘剩余空间不足2M，存盘操作不能进行...",MB_ICONSTOP,0);
		return;
	}
	str.Format(L"%d Mb",m_RemainMB);
	pShow->m_DiskFree()->SetWindowText (str);// 置当前硬盘可用空间MB
	
	pShow->m_Progress()->SetRange32(0,1024);// 设置进程范围
	pShow->m_Progress()->SetPos(0);
	
	/////////////
	pShow->m_FileName()->SetWindowText(strNewFileName);//显示存盘文件名
	m_bFirstCreateDeviceFile=FALSE;  // 若成功文件对象，则置标志	
	
	m_bCloseFile=FALSE; // 有文件没有关闭,可以操作《关闭当前文件》菜单
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
	nProcessMode = SAVE_MODE; // 当打开新文件时，意味着要进行存盘处理
	
	pADStatusView->SetDlgCheck();
}

void CADDoc::OnCloseFile() 
{
	// TODO: Add your command handler code here
	DWORD dTmp;
	for(int nChannel=0; nChannel<gl_nChannelCount; nChannel++)
	{
		header[nChannel].HeadSizeBytes = sizeof(FILE_HEADER);
		header[nChannel].ADStatus = gl_ADStatus; // gl_ADStatus.bTrigFlag;


		SetFilePointer(hFileObject[nChannel], 0, 0, FILE_BEGIN);
		if (!WriteFile(hFileObject[nChannel], &header[nChannel], sizeof(FILE_HEADER), &dTmp, NULL))
		{
			AfxMessageBox(L"error");
		}
		CloseHandle( hFileObject[nChannel]);
	}

	CString strTitle;
	strTitle = GetTitle(); // 为防止SetPathName改变文档的Title，则提前保存
	// 添加到 曾打开的文件列表中





	m_bCloseFile=TRUE;  // 文件被关闭
	nProcessMode = DATA_MODE; // 当关闭新文件时，意味着要不能再进行存盘处理，因此置成数字方式
	m_bFirstCreateDeviceFile=TRUE; // 当新文件被关闭后，意味着以后新文件还是第一次被创建 
	///////
	CADFrame* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrame ; // 取得子帧窗口句柄
	CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);  // 使存盘方式单选框有效
	
	CString str;
	str.Format(L"%d", 0);
	pADFrm->m_DiskFree()->SetWindowText(str);

	if (gl_bAutoCollect)
	{		
		SetPathName(g_strFileFullName[1]);
		SetPathName(g_strFileFullName[0]); 
		SetTitle(strTitle); // 恢复文档的Title
		pADFrm->ShowHideDlgSave (FALSE); // 隐藏存盘栏
		pADFrm->ShowHideStatus();//显示状态栏
	}
	     

	pADFrm->m_Progress()->SetRange32(0,1024);  // 设置进程范围
	pADFrm->m_Progress()->SetPos(0);	
	// 一兆进度
	pADStatusView->SetDlgCheck();	
}


void CADDoc::OnTileWave() 
{
	// TODO: Add your command handler code here
	gl_bTileWave = TRUE;
	CButton* pButtonTile = (CButton*)pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(1);
	CButton* pButtonPose = (CButton*)pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(0);
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();	
	pWaveView->OnDrawPolyLine();
}

void CADDoc::OnSuperPoseWave() 
{
	// TODO: Add your command handler code here
	gl_bTileWave = FALSE;
	CButton* pButtonTile = (CButton*)pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(0);
	CButton* pButtonPose = (CButton*)pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(1);
    CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	pWaveView->OnDrawPolyLine();
	
}

void CADDoc::OnUpdateSuperPoseWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_bTileWave==TRUE ? 0:1);
}

void CADDoc::OnUpdateTileWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_bTileWave==TRUE ? 1:0);
	
}






