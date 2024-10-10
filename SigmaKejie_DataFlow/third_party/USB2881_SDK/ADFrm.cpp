// ADFrm.cpp : implementation of the CADFrame class
//

#include "stdafx.h"
#include "Sys.h"

#include "MainFrm.h"
#include "ADFrm.h"
#include "ADDoc.h"
#include "ADDigitView.h"
#include "ADWaveView.h"
#include "QSortData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////
#include "ADStatusView.h"
#include "ParaCfgView.h"
extern CADStatusView* pADStatusView;
extern CParaCfgView* pParaCfgView;
////////////////////////////////////////////////////////////////////////////
// CADFrame

IMPLEMENT_DYNCREATE(CADFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CADFrame, CMDIChildWnd)
ON_COMMAND_EX(CG_ID_VIEW_DLGBAR, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_DLGBAR, OnUpdateControlBarMenu)
ON_COMMAND_EX(CG_ID_VIEW_SHOWSTATUS1, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SHOWSTATUS1, OnUpdateControlBarMenu)
ON_COMMAND_EX(CG_ID_VIEW_SHOWSTATUS, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SHOWSTATUS, OnUpdateControlBarMenu)
ON_WM_CREATE()
ON_COMMAND_EX(CG_ID_VIEW_SETUPDLG, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SETUPDLG, OnUpdateControlBarMenu)
//{{AFX_MSG_MAP(CADFrame)
ON_WM_CLOSE()
ON_WM_CANCELMODE()
ON_WM_LBUTTONUP()
ON_COMMAND(IDM_History_Digit, OnHistoryDigit)
ON_COMMAND(IDM_History_Wave, OnHistoryWave)
ON_UPDATE_COMMAND_UI(IDM_History_Digit, OnUpdateHistoryDigit)
ON_UPDATE_COMMAND_UI(IDM_History_Wave, OnUpdateHistoryWave)
ON_COMMAND(IDM_MoveVolt, OnMoveVolt)
ON_WM_DESTROY()
ON_WM_CREATE()
ON_UPDATE_COMMAND_UI(IDM_MoveVolt, OnUpdateMoveVolt)
ON_BN_CLICKED(IDC_CHECK_CheckOverflow, OnCHECKCheckOverflow)
ON_COMMAND(IDM_ShowCfgAnalBar, OnShowCfgAnalBar)
ON_UPDATE_COMMAND_UI(IDM_ShowCfgAnalBar, OnUpdateShowCfgAnalBar)
ON_COMMAND(IDM_ShowStatusBar, OnShowStatusBar)
ON_UPDATE_COMMAND_UI(IDM_ShowStatusBar, OnUpdateShowStatusBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADFrame construction/destruction

CADFrame::CADFrame()
{
	// TODO: add member initialization code here
	//	m_ProcessDigitalMode=1;
	m_ProcessGraphicMode = 1; 
	m_AnalyzeCheck = false;	
	m_bShowCfgAnalBar = true;
	m_bShowStatusBar = true;
	m_ProcessMoveVolt = 1;
	gl_ReadSizeWords = 0;
	//gl_ScreenVolume = (int)VOLT_RANGE;
	gl_ScreenVolume=20000;
	
}

CADFrame::~CADFrame()
{
	if(m_pWndSTCBar != NULL)
	{
		delete m_pWndSTCBar;
		m_pWndSTCBar = NULL;
	}
	pParaCfgView = NULL;
	pADStatusView = NULL;

}



/////////////////////////////////////////////////////////////////////////////
// CADFrame diagnostics

#ifdef _DEBUG
void CADFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CADFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADFrame message handlers

BOOL CADFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 1, 2))  // 创建1行2列分割
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	
	// add the first splitter pane - the default view in column 0
	// 创建第一个格子，在0列中使用默认的视图（由文档模板决定）
	
	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CADDigitView), CSize(350, 50), pContext))//CADDigitView
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	} 
	
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CADWaveView), CSize(0, 470), pContext))//
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	return TRUE;	
}


void CADFrame::ActivateFrame(int nCmdShow) 
{
	nCmdShow = SW_SHOWMAXIMIZED; // 使子帧窗口最大化
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

int CADFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// 创建右侧CControlBar窗体
	m_pWndSTCBar = new CCfgTabCtrlBar;
	m_pWndSTCBar->Create(this, CSize(260, 100), IDW_PROP_BAR);
	m_pWndSTCBar->SetBarStyle(CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);//CBRS_SIZE_DYNAMIC);
	
	m_pWndSTCBar->EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(m_pWndSTCBar, AFX_IDW_DOCKBAR_RIGHT);
	{ // 这个视图没有问题
		pParaCfgView=(CParaCfgView*)m_pWndSTCBar->AddView(L"参数配置", RUNTIME_CLASS(CParaCfgView/*CParaCfgView*/));//CParaCfgView
		pParaCfgView->m_pADFrm = this;
	}
	{// 这个视图有有问题
		pADStatusView=(CADStatusView*)m_pWndSTCBar->AddView(L"性能分析", RUNTIME_CLASS(CADStatusView));//CADStatusView
	}	

	OldADPara = ADPara;
	// TODO: Add a menu item that will toggle the visibility of the
	{
		// Initialize dialog bar m_wndShowStatus
		if (!m_wndShowStatus.Create(this, CG_IDD_Status,
			CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
			CG_IDD_Status))
		{
			TRACE0("Failed to create dialog bar m_wndShowStatus\n");
			return -1;		// fail to create
		}
		
		m_wndShowStatus.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndShowStatus);
		
		CProgressCtrl* pT=(CProgressCtrl*)m_wndShowStatus.GetDlgItem(IDC_PROGRESS);
        pT->SetRange(0, 50);
		
		if (!m_wndDlgSave.Create(this, CG_IDD_DIALOG_Save,
			CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
			CG_IDD_DIALOG_Save))
		{
			TRACE0("Failed to create dialog bar m_wndShowStatus\n");
			return -1;		// fail to create
		}
		m_wndDlgSave.ShowWindow(SW_HIDE);
		m_wndDlgSave.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndDlgSave);
	}
	CEdit* pFreqEdit=(CEdit*)(this->m_wndShowStatus).GetDlgItem(IDC_EDIT_MeasureFreq); 
	pFreqEdit->SetWindowText(L"拖鼠标测频");
	
	m_ToolTip.Create(this,TTS_ALWAYSTIP|WS_CHILD|WS_VISIBLE);
	if(pParaCfgView != NULL)
	{
		CComboBox * pFrequency =(CComboBox *)pParaCfgView->GetDlgItem(IDC_COMBO_Frequence);
		CComboBox * pTriggerSource =(CComboBox *)pParaCfgView->GetDlgItem(IDC_COMBO_TriggerSource);
		CComboBox * pTriggerDir =(CComboBox *)pParaCfgView->GetDlgItem(IDC_COMBO_OutTriggerDir);
		
		CEdit * pBufferID =(CEdit *)pParaCfgView->GetDlgItem(IDC_EDIT_BufferID);
		CSliderCtrl* pSLIDER_Offset =(CSliderCtrl*)pParaCfgView->GetDlgItem(IDC_SLIDER_Offset);
		m_ToolTip.AddTool(pFrequency,L"设定频率");
		m_ToolTip.AddTool(pTriggerSource,L"内部触发或外部触发");
		m_ToolTip.AddTool(pTriggerDir,L"触发方向");
		m_ToolTip.AddTool(pBufferID,L"在2个缓冲区中选择缓冲区");
		m_ToolTip.AddTool(pSLIDER_Offset,L"在缓冲区中选择偏移");
	}
	return 0;
}

void CADFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnClose();
}

void CADFrame::OnCancelMode() 
{
	CMDIChildWnd::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CADFrame::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}


void CADFrame::OnHistoryDigit() 
{
	// TODO: Add your command handler code here
	STARTUPINFO StartupInfo;
	memset(&StartupInfo,0,sizeof(STARTUPINFO));
	StartupInfo.cb=sizeof(STARTUPINFO);
	
	PROCESS_INFORMATION ProcessInfo;
	
	::CreateProcess
		(L"\\User\\SysTest\\Release\\SysTest.exe",
		L" Digit",  // 注意：第一个字符必须为空格
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInfo);
}

void CADFrame::OnHistoryWave() 
{
	// TODO: Add your command handler code here
	STARTUPINFO StartupInfo;
	memset(&StartupInfo,0,sizeof(STARTUPINFO));
	StartupInfo.cb=sizeof(STARTUPINFO);
	
	PROCESS_INFORMATION ProcessInfo;
	
	::CreateProcess
		(L"\\User\\SysTest\\Release\\SysTest.exe",
		L" Wave",  // 注意：第一个字符必须为空格
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInfo);
	
}

void CADFrame::OnUpdateHistoryDigit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);
}

void CADFrame::OnUpdateHistoryWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);
}

void CADFrame::OnMoveVolt() //平移电压的函数
{
	WORD m_DataMiddle; // 中间数原码
	ULONG nArrayCount=-1;
	ULONG m=0;
	WCHAR str[100];
	
	//m_ProcessMoveVolt=m_ProcessMoveVolt==0?1:0;
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc; 
	CString strn;
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	if(m_ProcessMoveVolt == 1) // 开始求中间值
	{
		m_ProcessMoveVolt = 0;
		if(gl_ReadSizeWords == 0)
		{
// 			AfxMessageBox(L"还没有开始采集数据!");
// 			return;
		}

		//ULONG lCount = (gl_ReadSizeWords%4096)/gl_nChannelCount;
		ULONG lCount = (gl_ReadSizeWords)/gl_nChannelCount;
		if (lCount == 0)
			lCount = MAX_RAM_SIZE/gl_nChannelCount;


		for(int nChannel=0; nChannel<gl_nChannelCount; nChannel++)
		{
			if (ADPara.bChannelArray[nChannel] == TRUE)
			{
				for(ULONG Index=0; Index<lCount; Index++)
				{
					DataAnalyze[nChannel][Index] = ((ADBuffer[nChannel][glBufferID][Index]^0x8000)&MAX_LSB);
				}
			}
		}
		/////////////////////////////////////////////			
		for(int k=0; k<gl_nChannelCount; k++)//每通道的数据各自进行排序
		{
			if (ADPara.bChannelArray[k] == TRUE)
			{
				m_QSortData.Sort(DataAnalyze[k], 0, lCount);
				m_DataMiddle = m_QSortData.GetMidData(); // 取得中间数		
				// m_DataMiddle是非偏移码，即原码，即0-LSB_COUNT中的任意值
				// gl_MiddleLsb 是偏移码值，即原码减去LSB_HALF后的值,即在原点上下浮动的值
				// gl_MiddleLsb[k]=(int)((m_DataMiddle)-LSB_HALF);
				gl_MiddleLsb[k]=(int)(m_DataMiddle - LSB_HALF);
	//			gl_MiddleLsb[k] = gl_MiddleLsb[k];
				
				CString StrMoveVolt;
				switch(m_DigitalDataShowMode)
				{
				case 0: // 十进制原码
					swprintf_s(str, L"CH%d %d Dec",k,m_DataMiddle-LSB_HALF);
					break;
				case 1: // 十六进制原码
					//StrMoveVolt.Format(L"%x Hec",m_DataMiddle.value);
					swprintf_s(str, L"CH%d %x Hec",k,m_DataMiddle);
					break;
				case 2: // 电压值
					//StrMoveVolt.Format(L"%5.2fmV",(m_DataMiddle.value-8192)*VOLT_RANGE/LSB_COUNT);
					swprintf_s(str, L"CH%d %5.2fmV",k,(m_DataMiddle-LSB_HALF)*gl_PerLsbVoltM[k]);
					break;
				}
				//pADStatus->m_nMoveVolt.SetWindowText(StrMoveVolt);
				if(pADStatusView != NULL)
					pADStatusView->m_nMoveVolt.AddString (str);
		}
			m_AnalyzeAllCount=0;
			m_OverLimitCount=0;
		}
		if(pADStatusView != NULL)
			pADStatusView->m_nMoveVolt.SetCurSel(0);
	}	
	else
	{
		m_ProcessMoveVolt = 1;
		for(int i=0;i<MAX_CHANNEL_COUNT;i++)
		{
			gl_MiddleLsb[i] = 0;// 如果不偏移, 置偏移值为0
			//gl_MiddleLsb[i] = 0;
		}
		if(pADStatusView != NULL)
		{
			while(pADStatusView->m_nMoveVolt.GetCount()>0)
				pADStatusView->m_nMoveVolt.DeleteString(0);
			CString StrMoveVolt;
			StrMoveVolt=" ";
			pADStatusView->m_nMoveVolt.AddString (StrMoveVolt);
		}
	}
	
	if(gl_bDeviceADRun==FALSE) // 如果设备没有运行，即静态分析
	{
		pDigitView->RedrawWindow();
		pWaveView->OnDrawPolyLine();
		return;
	}
	if(nProcessMode == DATA_MODE)  // 如果是波形方式，则只更新数字方式
	{
		pDigitView->RedrawWindow();
	}
	if(nProcessMode == WAVE_MODE) // 反之则然
	{
		pWaveView->OnDrawPolyLine();
	}
}
void CADFrame::OnUpdateMoveVolt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!m_ProcessMoveVolt);	
}

void CADFrame::OnDestroy() 
{
	
	// TODO: Add your message handler code here
	USB2881_SaveParaAD(NULL, &ADPara);
	if(gl_bDeviceADRun)//检查是否设备
	{
		CADDoc* pDoc=(CADDoc*)GetActiveDocument();
		pDoc->Public_ReleaseDevice();
	}    
	;
	CSysApp* pApp = (CSysApp*)AfxGetApp();
//	USB2881_ReleaseDevice(pApp->m_hDevice);
    pApp->m_bCreateADFrm = FALSE;	
	pApp->m_ADFrame = NULL;

	CMDIChildWnd::OnDestroy();	
}

void CADFrame::OnCHECKCheckOverflow() 
{
	// TODO: Add your control notification handler code here
	MessageBox(L"OnCHECKCheckOverflow");
}

void CADFrame::OnShowCfgAnalBar() 
{
	// TODO: Add your command handler code here
	m_bShowCfgAnalBar = !m_bShowCfgAnalBar;
	ShowControlBar(m_pWndSTCBar, m_bShowCfgAnalBar, false);
	
}

void CADFrame::OnUpdateShowCfgAnalBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bShowCfgAnalBar);
	
}

void CADFrame::OnShowStatusBar() 
{
	// TODO: Add your command handler code here
	m_bShowStatusBar = !m_bShowStatusBar;
	ShowControlBar(&m_wndShowStatus, m_bShowStatusBar, false);	
}

void CADFrame::OnUpdateShowStatusBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bShowStatusBar);
}

BOOL CADFrame::PreTranslateMessage(MSG* pMsg) 
{
	m_ToolTip.RelayEvent(pMsg);		
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

 
