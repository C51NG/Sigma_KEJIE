// Sys.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Sys.h"
#include "MainFrm.h"
#include "ADFrm.h"
#include "ADDoc.h"
#include "ADDigitView.h"
#include "ADWaveView.h"
#include "ADHistFrm.h"
#include "ADHistDoc.h"
#include "ADHistDigitView.h"
#include "DlgCalibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
#include "ADStatusView.h"
#include "ParaCfgView.h"
extern CADStatusView* pADStatusView;
extern CParaCfgView* pParaCfgView;
// CSysApp
BEGIN_MESSAGE_MAP(CSysApp, CWinApp)
	//{{AFX_MSG_MAP(CSysApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_UPDATE_COMMAND_UI(IDM_CloseFile, OnUpdateCloseFile)
	ON_UPDATE_COMMAND_UI(IDM_DataFileNew, OnUpdateDataFileNew)
	ON_COMMAND(IDM_CloseFile, OnCloseFile)
	ON_COMMAND(IDM_DataFileNew, OnDataFileNew)
	ON_COMMAND(IDM_StartDeviceAD, OnStartDeviceAD)
	ON_COMMAND(IDM_StopDeviceAD, OnStopDeviceAD)
	ON_UPDATE_COMMAND_UI(IDM_StopDeviceAD, OnUpdateStopDeviceAD)
	ON_COMMAND(IDM_Open_AD, OnOpenAD)
	ON_UPDATE_COMMAND_UI(IDM_Open_AD, OnUpdateOpenAD)
	ON_COMMAND(IDM_Open_DIO, OnOpenDIO)
	ON_COMMAND(IDM_HistoryData, OnHistoryData)
	ON_UPDATE_COMMAND_UI(IDM_HistoryData, OnUpdateHistoryData)
	ON_UPDATE_COMMAND_UI(IDM_Open_DA, OnUpdateOpenDA)
	ON_UPDATE_COMMAND_UI(IDM_StartDeviceAD, OnUpdateStartDeviceAD)
	ON_UPDATE_COMMAND_UI(IDM_MDataSave, OnUpdateDataSave)
	ON_UPDATE_COMMAND_UI(IDM_MDigitalShow, OnUpdateDigitalShow)
	ON_UPDATE_COMMAND_UI(IDM_MGraphicShow, OnUpdateGraphicShow)
	ON_COMMAND(IDM_MDataSave, OnMDataSave)
	ON_COMMAND(IDM_MDigitalShow, OnMDigitalShow)
	ON_COMMAND(IDM_MGraphicShow, OnMGraphicShow)
	ON_COMMAND(IDM_ListDeviceDlg, OnListDeviceDlg)
	ON_COMMAND(IDM_ADCalibration, OnADCalibration)
	ON_UPDATE_COMMAND_UI(IDM_ADCalibration, OnUpdateADCalibration)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysApp construction

CSysApp::CSysApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bCreateADFrm = FALSE;
	m_bCreateDAFrm = FALSE;
	m_ADFrame = NULL;
	m_bDeviceCount = FALSE;

	//536870912
//	WORD tBuff[1024];

/*
 	ADBuffer[0][0] = new WORD[MAX_RAM_SIZE];
 	ADBuffer[0][1] = new WORD[MAX_RAM_SIZE];
 	ADBuffer[1][0] = new WORD[MAX_RAM_SIZE];
	ADBuffer[1][1] = new WORD[MAX_RAM_SIZE];
	ADBuffer[2][0] = new WORD[MAX_RAM_SIZE];
	ADBuffer[2][1] = new WORD[MAX_RAM_SIZE];
	ADBuffer[3][0] = new WORD[MAX_RAM_SIZE];
 	ADBuffer[3][1] = new WORD[MAX_RAM_SIZE];
	*/


	memset(ADBuffer, 0, sizeof(ADBuffer));

// 	for (int i=0; i<MAX_CHANNEL_COUNT; i++)
// 	{
// 		for (int j=0; j<MAX_SEGMENT; j++)
// 		{
// 			for (int k=0; k<MAX_RAM_SIZE; k++)
// 			{
// 				ADBuffer[i][j][k]= 0;
// 			}
// 		}
// 	}
// 	
// 	ADBuffer[1][1] = new WORD[536870912];
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSysApp object

CSysApp theApp;
ULONG gl_ReadSizeWords;	// 读入的数据长度

/////////////////////////////////////////////////////////////////////////////
// CSysApp initialization

BOOL CSysApp::InitInstance()
{
	WCHAR szMutex[256];
	BOOL bCreate=FALSE;
	//HANDLE hDevice;
	m_hDevice = INVALID_HANDLE_VALUE; // 在该应用程序使用的全局设备对象未被创建时，应置初值，视为设备无效
	m_CurrentDeviceID=0;   // 指定当前设备的ID标示符
	/////
// 	int DeviceCount;
// 	hDevice = USB2881_CreateDevice(0); // 创建设备对象，保存在App中，可供其他任何子窗体或子功能使用
// 	if(hDevice!=INVALID_HANDLE_VALUE) // 如果设备句柄有效
// 	{
// 		//DeviceCount = USB2881_GetDeviceCurrentID(hDevice;&DeviceLgcID);
// 		USB2881_ReleaseDevice(hDevice);	
// 	}
// 	else
// 	{
// 		DeviceCount = 0; // 如果没有创建成功，则认为设备数量为0
// 	}
// 
// 	for(int i=0; i<DeviceCount; i++)
// 	{
// 		swprintf_s(szMutex, L"USB2881-%d", i);
// 		
// 		// 创建互斥对象
// 		m_hMutex=::CreateMutex(NULL, NULL, szMutex);  // m_pszExeName为本程序的执行名
// 		if(GetLastError()==ERROR_ALREADY_EXISTS)  // 第二次创建应用程序
// 		{
// 			bCreate=FALSE;
// 			continue;  // 如果已经创建，则继续下一个设备的应用程序创建
// 		}
// 		else
// 		{ 	
// 			bCreate=TRUE;
// 			m_CurrentDeviceID=i;
// 			break;
// 		}
// 		
// 	}
// 
// 	if(DeviceCount!=0)
// 	{
// 		if(bCreate==FALSE)  // 当该实例不能被创建时
// 		{
// 			AfxMessageBox(L"对不起，您的所有设备已被相应程序管理，您不能再创建新实例...",MB_ICONWARNING,0);
// 			return FALSE; 
// 		}
// 		m_hDevice = USB2881_CreateDevice(m_CurrentDeviceID);
// 	}

	for(int i=0; i<16; i++)
	{
		swprintf_s(szMutex, L"USB2881-%d", i);

		// 创建互斥对象
		m_hMutex=::CreateMutex(NULL, NULL, szMutex);  // m_pszExeName为本程序的执行名
		if(GetLastError()==ERROR_ALREADY_EXISTS)  // 第二次创建应用程序
		{
			bCreate=FALSE;
			continue;  // 如果已经创建，则继续下一个设备的应用程序创建
		}
		else
		{ 	
			bCreate=TRUE;
			m_CurrentDeviceID=i;
			break;
		}

	}
	m_hDevice = USB2881_CreateDevice(m_CurrentDeviceID);
	

	///////////////////////////////////////////////
	// 判断用户的显示器模式是否为1024*768
	int Len=GetSystemMetrics(SM_CXSCREEN);  // 取得屏幕宽度
	if(Len<1024) // 如果屏幕宽度大小1024，则
	{
		if(AfxMessageBox(L"请最好使用1024*768或以上的显示器分辨率，继续吗？",MB_ICONWARNING|MB_YESNO,0)==IDNO)	
		{
			ExitInstance();
			return FALSE;
		}
	}
	
///////////////////////////////////////////////
	
// 应用程序正常创建代码：
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

// #ifdef _AFXDLL
// 	Enable3dControls();			// Call this when using MFC in a shared DLL
// #else
// 	Enable3dControlsStatic();	// Call this when linking to MFC statically
// #endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	
	// 添加AD采集模板
	pADTemplate = new CMultiDocTemplate(
		IDR_AD,
		RUNTIME_CLASS(CADDoc),
		RUNTIME_CLASS(CADFrame), // custom MDI child frame
		RUNTIME_CLASS(CADDigitView));
	AddDocTemplate(pADTemplate);
	
	// 添加历史数据回放模板
	pHistDataTemplate = new CMultiDocTemplate(
		IDR_HistoryData,
		RUNTIME_CLASS(CADHistDoc),
		RUNTIME_CLASS(CADHistFrm), // custom MDI child frame
		RUNTIME_CLASS(CADHistDigitView));
	AddDocTemplate(pHistDataTemplate);

    // 添加开关量模板
	pDIOTemplate = new CMultiDocTemplate(
		IDR_DIO,
		RUNTIME_CLASS(CDIODoc),
		RUNTIME_CLASS(CDIOFrm), // custom MDI child frame
		RUNTIME_CLASS(CDIOView));
	AddDocTemplate(pDIOTemplate);

	// Enable DDE Execute open
	// 让WINDOWS登记该程序的缺省数据文件名扩展名
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	
	
	m_pMainWnd = pMainFrame;
	
	
	// The main window has been initialized, so show and update it.
	// pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);  // 使主窗口最大化
	
	pMainFrame->UpdateWindow();
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();  // 支持拖放功能

	::SetProp(m_pMainWnd->GetSafeHwnd(), m_pszExeName, (HANDLE)1);	

	CString MainFrmName; WCHAR str[100];
	swprintf_s(str, L"USB2881-%d ", m_CurrentDeviceID);
	MainFrmName = pMainFrame->GetTitle();
	MainFrmName = str+MainFrmName;
	pMainFrame->SetTitle(MainFrmName);

	OnOpenAD();		// 打开AD设备
	m_ADFrame->BringWindowToTop();		// 将AD模板置于屏幕顶端

	// 取得操作系统版本
	DWORD	dwVersion = GetVersion();
	// Get the Windows version.
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	// Get the build number.
	if ((dwVersion < 0x80000000) || (dwWindowsMajorVersion < 4))              // Windows NT
		gl_nSampleMode = DMA_MODE;		// DMA方式
	else                                     // Windows Me/98/95
		gl_nSampleMode = NPT_MODE;		// 查询方式

	
		gl_nSampleMode = NPT_MODE;		// 查询方式
	// CG: This line inserted by 'Tip of the Day' component.
 

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSysApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSysApp message handlers



void CSysApp::OnUpdateOpenAD(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(this->m_bCreateADFrm==FALSE?1:0);
}

void CSysApp::OnOpenAD(void)
{
	if (m_ADFrame != NULL)
	{
		m_ADFrame->BringWindowToTop();
		return;
	}
	BeginWaitCursor();  // 开始漏斗鼠标
	CDocument* pDoc;
	pDoc=pADTemplate->CreateNewDocument();  // 创建新文档
	m_pADDoc=(CADDoc*)pDoc;  // 保存文档对象
	// 创建该文档对象的子帧窗口
	m_ADFrame=(CADFrame*)pADTemplate->CreateNewFrame(pDoc, NULL);
	// 设置默认的文档标题
	// 创建文档实例
	// 初始化FRAME子帧窗口，且触发FRAME的OnUpdate函数
	pADTemplate->InitialUpdateFrame(m_ADFrame, pDoc);
	pDoc->SetTitle(L"AD数采测试");

	if(pADStatusView != NULL)
	{
		pDoc->AddView ((CView*)pADStatusView);
	}
	if(pParaCfgView != NULL)
	{
		pDoc->AddView((CView*)pParaCfgView);
	}

	EndWaitCursor(); // 停止漏斗鼠标
}

void CSysApp::OnHistoryData() 
{
	// TODO: Add your command handler code here
	CString strNewFileName;	
	// 弹出文件对话框
	if (!(DoPromptFileName(strNewFileName, IDS_ProccessHistoryData,
		OFN_HIDEREADONLY , TRUE, NULL))) // OFN_CREATEPROMPT:当文件不存在时，是否要求新建
		return;
	
	CFile file;
	CFileStatus status;
	if (!file.GetStatus(strNewFileName, status))
	{
		strNewFileName=strNewFileName+" 不存在!";
		AfxMessageBox(strNewFileName,MB_ICONSTOP, 0);
		return; // 如果文件不存
	}
	
	CFile File;
	FILE_HEADER m_Header;

	File.Open(strNewFileName, CFile::modeRead);
	File.Seek(0,CFile::begin);
	File.Read((WORD*)&m_Header,sizeof(m_Header)); // 文件信息由HistDigitView类将其置在HistDataFrm的对话框条上OnInitialUpdate
	File.Close();

	CADHistDoc* pDoc;	
	BeginWaitCursor();  // 开始漏斗鼠标
	pDoc=(CADHistDoc*)pHistDataTemplate->OpenDocumentFile(strNewFileName); // 先传递文件名到pDoc的消息函数OnOpenDocument
	EndWaitCursor();   // 停止漏斗鼠标

}

void CSysApp::OnUpdateHistoryData(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}



void CSysApp::OnUpdateDataFileNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((!gl_bDeviceADRun)&&m_bFirstCreateDeviceFile);	
}

void CSysApp::OnCloseFile() 
{
	// TODO: Add your command handler code here
	m_pADDoc->OnCloseFile();
}

void CSysApp::OnDataFileNew() 
{
	// TODO: Add your command handler code here
	m_pADDoc->OnDataFileNew();
}

void CSysApp::OnUpdateOpenDA(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void CSysApp::OnStartDeviceAD() 
{
	// TODO: Add your command handler code here
	m_pADDoc->StartDeviceAD();
}

void CSysApp::OnUpdateStartDeviceAD(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(gl_bDeviceADRun==FALSE?1:0);	
}

void CSysApp::OnStopDeviceAD() 
{
	// TODO: Add your command handler code here
	m_pADDoc->StopDeviceAD();
}

void CSysApp::OnUpdateStopDeviceAD(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(gl_bDeviceADRun==TRUE?1:0);		
}

void CSysApp::OnUpdateCloseFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (FALSE); 
	if((nProcessMode == SAVE_MODE) && (bCreateDevice))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(!m_bCloseFile);	
}

void CSysApp::OnUpdateDataSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
	if(m_bFirstCreateDeviceFile)
		pCmdUI->Enable(FALSE);	
	pCmdUI->SetCheck(nProcessMode == SAVE_MODE ? 1:0);	
}

void CSysApp::OnUpdateDigitalShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nProcessMode == DATA_MODE ? 1:0);
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
    CSysApp *pApp=(CSysApp *)AfxGetApp();

    CADDoc* pDoc=pApp->m_pADDoc; 
    // 在线程中取得视图指针
    CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	if(nProcessMode == SAVE_MODE) // 如果为存盘方式
	{
		pDigitView->EnableWindow(FALSE);
	}
	else
	{
		pDigitView->EnableWindow(TRUE);
	}		
}

void CSysApp::OnUpdateGraphicShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
	pCmdUI->SetCheck(nProcessMode == WAVE_MODE ? 1:0);
	CSysApp *pApp=(CSysApp *)AfxGetApp();
    CADDoc* pDoc=pApp->m_pADDoc; 
    // 在线程中取得视图指针

    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	
	if(nProcessMode == SAVE_MODE) // 如果为存盘方式
	{
		pWaveView->EnableWindow(FALSE);
	}
	else
	{
		pWaveView->EnableWindow(TRUE);
	}		
}

void CSysApp::OnMDataSave() 
{
	// TODO: Add your command handler code here
	nProcessMode = SAVE_MODE;
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
}

void CSysApp::OnMDigitalShow() 
{
	// TODO: Add your command handler code here
	nProcessMode = DATA_MODE;
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
}

void CSysApp::OnMGraphicShow() 
{
	// TODO: Add your command handler code here
	nProcessMode = WAVE_MODE;
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
}

CDocument* CSysApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString str;
	CFile file;
	CFileStatus status;
	if (!file.GetStatus(lpszFileName, status))
	{
		str=lpszFileName;
		str=str+" 不存在!";
		AfxMessageBox(lpszFileName,MB_ICONSTOP, 0);
		return NULL;  // 如果文件不存
	}
	
	CFile File;
	FILE_HEADER m_Header;
	File.Open(lpszFileName, CFile::modeRead);
	File.Seek(0, CFile::begin);
	File.Read((WORD*)&m_Header, sizeof(m_Header));  // 文件信息由HistDigitView类将其置在HistDataFrame的对话框条上OnInitialUpdate
	File.Close();
	if(m_Header.DeviceNum != DEFAULT_DEVICE_NUM) 
	{
		if(AfxMessageBox(L"对不起，该文件不能解析数据文件, 是否继续",MB_YESNO, 0)==IDNO)
		return NULL;
		//bFile = FALSE;
	}	

	CADHistDoc* pDoc;	
	BeginWaitCursor();  // 开始漏斗鼠标
	pDoc=(CADHistDoc*)pHistDataTemplate->OpenDocumentFile(lpszFileName);  // 先传递文件名到pDoc的消息函数OnOpenDocument
	str = lpszFileName;
	//str="历史数据："+ str;
	str="历史数据：";
	pDoc->SetTitle(str);  // 置文档新标题
	EndWaitCursor(); // 停止漏斗鼠标
	return pDoc;
	//return CWinApp::OpenDocumentFile(lpszFileName);
}

void CSysApp::OnListDeviceDlg() 
{
	// TODO: Add your command handler code here
	//USB2881_ListDeviceDlg(m_hDevice); // 列表该类设备
}




int CSysApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	 	ReleaseMutex(m_hMutex);
 	if(m_hDevice != INVALID_HANDLE_VALUE) 
 	{
		USB2881_ReleaseDevice(m_hDevice);
 		m_hDevice = INVALID_HANDLE_VALUE;
 	}

/*	
 	delete ADBuffer[0][0];
 	delete ADBuffer[0][1];
 	delete ADBuffer[1][0];
	delete ADBuffer[1][1];
	delete ADBuffer[2][0];
	delete ADBuffer[2][1];
	delete ADBuffer[3][0];
	delete ADBuffer[3][1];
	*/

	return CWinApp::ExitInstance();
}

void CSysApp::OnADCalibration() 
{
	// TODO: Add your command handler code here

	if (m_hDevice != INVALID_HANDLE_VALUE)
	{
		CDlgCalibration dlg;
		dlg.DoModal();
	}
}

void CSysApp::OnUpdateADCalibration(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!gl_bDeviceADRun);
}


void CSysApp::OnOpenDIO()
{
	if (NULL == m_pDIOFrm)
	{
		BeginWaitCursor();  // 开始漏斗鼠标
		CDocument* pDoc;
		pDoc = pDIOTemplate->OpenDocumentFile(NULL);  // 创建新文档	
		pDoc->SetTitle(_T("开关量测试"));
		EndWaitCursor(); // 停止漏斗鼠标
	}
	else
	{
		m_pDIOFrm->BringWindowToTop();
	}
}
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString str;
	ULONG ulFmwVer, ulDriverVer;
	if (theApp.m_hDevice != INVALID_HANDLE_VALUE)
	{
		USB2881_GetDevVersion(theApp.m_hDevice, &ulFmwVer, &ulDriverVer);
		str.Format(L"驱动版本 %d", ulDriverVer);
		GetDlgItem(IDC_STATIC_DriverVer)->SetWindowText(str);

		str.Format(L"固件版本 %d", ulFmwVer);
		GetDlgItem(IDC_STATIC_FmwVer)->SetWindowText(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
