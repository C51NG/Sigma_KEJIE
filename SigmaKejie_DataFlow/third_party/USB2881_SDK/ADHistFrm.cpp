// HistDataFrm.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ADHistFrm.h"

#include "ADHistDigitView.h"
#include "ADHistWaveView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADHistFrm

IMPLEMENT_DYNCREATE(CADHistFrm, CMDIChildWnd)

CADHistFrm::CADHistFrm()
{
	
m_bStart=FALSE;
m_ShowMode=2;  // 波形回放（1为数字回放）
}

CADHistFrm::~CADHistFrm()
{
}


BEGIN_MESSAGE_MAP(CADHistFrm, CMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND_EX(CG_ID_VIEW_DIALOGBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_DIALOGBAR, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CADHistFrm)
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND(IDM_History_Start, OnHistoryStart)
	ON_UPDATE_COMMAND_UI(IDM_History_Start, OnUpdateHistoryStart)
	ON_COMMAND(IDM_History_Stop, OnHistoryStop)
	ON_UPDATE_COMMAND_UI(IDM_History_Stop, OnUpdateHistoryStop)
	ON_UPDATE_COMMAND_UI(IDM_History_Digit, OnUpdateHistoryDigit)
	ON_UPDATE_COMMAND_UI(IDM_History_Wave, OnUpdateHistoryWave)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Offset, OnCustomdrawSLIDEROffset)
	ON_CBN_SELCHANGE(IDC_COMBO_Rate, OnSelchangeCOMBORate)
	ON_EN_CHANGE(IDC_EDIT_Offset, OnChangeEDITOffset)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_REFRUSH, OnButtonRefrush)
	ON_BN_CLICKED(IDM_History_ProcessADLSB, OnHistoryProcessADLSB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistFrm message handlers

BOOL CADHistFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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
		pContext->m_pNewViewClass, CSize(250, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	} 
	
	if (!m_wndSplitter.CreateView(0, 1,
			RUNTIME_CLASS(CADHistWaveView), CSize(350, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	} 

	
    
	return TRUE;

	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

int CADHistFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	{
		// Initialize dialog bar m_wndSetupBar
		if (!m_wndSetupBar.Create(this, CG_IDD_HistoryDlg,
			CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
			CG_ID_VIEW_DIALOGBAR))
		{
			TRACE0("Failed to create dialog bar m_wndSetupBar\n");
			return -1;		// fail to create
		}

		m_wndSetupBar.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndSetupBar);

	CButton* pRadioA = (CButton*)m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionA);
	CButton* pRadioB = (CButton*)m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionB);
 	pRadioA->SetCheck(1);
	pRadioB->SetCheck(0);

	}		
	return 0;
}


void CADHistFrm::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}

void CADHistFrm::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CADHistDoc* pDoc=(CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
	pDoc->m_Offset += pDoc->m_Rate;
// 	if (pDoc->m_Offset+8192 >= pDoc->m_FileLength)
// 	{
// 		OnHistoryStop();
// 	}
	if (pDoc->m_Offset>=(LONG)(pDoc->m_FileLength-8192))
	{
		pDoc->m_Offset=(LONG)(pDoc->m_FileLength-8192);
		OnHistoryStop();
	}
	
	RedrawDataWindow();
	CMDIChildWnd::OnTimer(nIDEvent);
}

void CADHistFrm::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	

}

void CADHistFrm::OnHistoryStart() 
{
	// TODO: Add your command handler code here
	m_bStart=TRUE;
	SetTimer(1, 200, NULL);

}

void CADHistFrm::OnUpdateHistoryStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStart==FALSE?1:0);
}

void CADHistFrm::OnHistoryStop() 
{
	// TODO: Add your command handler code here
	m_bStart=FALSE;
	KillTimer(1);
}

void CADHistFrm::OnUpdateHistoryStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStart==TRUE?1:0);
}

 

void CADHistFrm::OnUpdateHistoryDigit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_ShowMode==1? 1:0);
}

 

void CADHistFrm::OnUpdateHistoryWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_ShowMode==2? 1:0);
}

void CADHistFrm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CADHistFrm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default


	CMDIChildWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CADHistFrm::OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc=(CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针

	CSliderCtrl* pSliderOffset=(CSliderCtrl*)(m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	
	int Pos=pSliderOffset->GetPos();  // 取得滑动条位置（即文件偏移（字））
	pSliderOffset->SetPos(Pos);
	pDoc->m_Offset=Pos;
	CString str;	
	CEdit* pEditOffset = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	CEdit* pEditScreenPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));
	str.Format(L"%d", Pos);
	pEditOffset->SetWindowText(str);  // 显示文件偏移
	str.Format(L"%d", Pos);
	pEditScreenPos->SetWindowText(str);   
	RedrawDataWindow();  // 重绘数据视图

	*pResult = 0;
}

void CADHistFrm::RedrawDataWindow(void)
{
	CString str;
	CADHistDoc* pDoc=(CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
	CADHistDigitView* pDigitView=(CADHistDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
    CADHistWaveView* pWaveView=(CADHistWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CEdit* pOffset=(CEdit*)m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset);

	CEdit* pPos=(CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));

	CSliderCtrl* pSliderOffset=(CSliderCtrl*)(m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	

	pSliderOffset->SetPos(pDoc->m_Offset);
	
	pDoc->m_ScreenOffset = pDoc->m_Offset;
    str.Format(L"%d", pDoc->m_ScreenOffset);
    pPos->SetWindowText(str);
	pDoc->ReadData();
	switch(m_ShowMode)
	{
	   case 1:
			pDigitView->SetListView();
			break;
	   case 2:
			pWaveView->RedrawWindow();			
			break;			
	}
}

void CADHistFrm::OnSelchangeCOMBORate() 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc=(CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
	CComboBox* pRate=(CComboBox*)(m_wndSetupBar.GetDlgItem(IDC_COMBO_Rate));	
	CString str;
	int Index=pRate->GetCurSel();  // 取得当前选择索引号
	pRate->GetLBText(Index,str);   // 根据选择，取得所选择项文本	
	pDoc->m_Rate=wcstol(str,NULL, 10);  // 将字符转换成数字
}

void CADHistFrm::OnChangeEDITOffset() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CMDIChildWnd::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}

void CADHistFrm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnClose();
}

 


void CADHistFrm::OnButtonRefrush() 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc=(CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
	CADHistDigitView* pView = (CADHistDigitView*)CView::FromHandle(pDoc->m_hWndDigit);
	pView->SetListView();
}

void CADHistFrm::OnHistoryProcessADLSB() 
{
	// TODO: Add your control notification handler code here
	
	
	CADHistDoc* pDoc=(CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
	CADHistDigitView* pView = (CADHistDigitView*)CView::FromHandle(pDoc->m_hWndDigit);
	pView->ProcessADLSB();
	CEdit* pEditSqrtValue = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_SqrtValue));
	CString str;
	str.Format(L"%6.2f",pView->m_dSqrtValue);	
	pEditSqrtValue->SetWindowText(str); 
}
