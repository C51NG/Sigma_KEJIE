// ADStatusView.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"

#include "ADFrm.h"
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

/////////////////////////////////////////////////////////////////////////////
// CADStatusView

IMPLEMENT_DYNCREATE(CADStatusView, CFormView)

CADStatusView::CADStatusView()
	: CFormView(CADStatusView::IDD)
{
	//{{AFX_DATA_INIT(CADStatusView)
	//}}AFX_DATA_INIT
//	bFirstChangeTrigge=TRUE;  // 当某些类对象没有准备就绪时，不能操作CEdit框
}

CADStatusView::~CADStatusView()
{
}	

void CADStatusView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADStatusView)
	DDX_Control(pDX, IDC_Combo_ScreenVolume, m_nScreenVolume);
	DDX_Control(pDX, IDC_Combo_MoveVolt, m_nMoveVolt);
	DDX_Control(pDX, IDC_EDIT_AnalyzeCount, m_nAnalyzeCount);
	DDX_Control(pDX, IDC_EDIT_OverRange, m_nOverRange);
	DDX_Control(pDX, IDC_EDIT_OverRatio, m_nOverRatio);
	DDX_Control(pDX, IDC_EDIT_TriggerVolt, m_nTringgerVolt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADStatusView, CFormView)
	//{{AFX_MSG_MAP(CADStatusView)
	ON_EN_CHANGE(IDC_EDIT_TriggerVolt, OnChangeEDITTriggerVolt)
	ON_BN_CLICKED(IDM_GraphicShow, OnGraphicShow)
	ON_BN_CLICKED(IDM_DigitalShow, OnDigitalShow)
	ON_BN_CLICKED(IDM_DataSave, OnDataSave)
	ON_CBN_SELCHANGE(IDC_Combo_ScreenVolume, OnSelchangeComboScreenVolume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADStatusView diagnostics

#ifdef _DEBUG
void CADStatusView::AssertValid() const
{
	CFormView::AssertValid();
}

void CADStatusView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADStatusView message handlers

void CADStatusView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CString str;
	// TODO: Add your specialized code here and/or call the base class
	str.Format(L"%d", gl_ScreenVolume);
	
	str.Format(L"%d", m_TriggerVolt);
	m_nTringgerVolt.SetWindowText(str);

	m_nScreenVolume.SetCurSel(13);


	m_nMoveVolt.SetWindowText(L"0");
	
	m_nAnalyzeCount.SetWindowText(L"0");
	
	m_nOverRange.SetWindowText(L"0");//要返回的
	
	m_nOverRatio.SetWindowText(L"0");//要返回的
	if(gl_bTileWave)
	{
		CButton* pButtonTile = (CButton*)GetDlgItem(IDM_TileWave);
		pButtonTile->SetCheck(1);
	}
	else
	{
		CButton* pButtonPose = (CButton*)GetDlgItem(IDM_SuperPoseWave);
		pButtonPose->SetCheck(1);
	}
	SetDlgCheck();     
}

 


void CADStatusView::OnChangeEDITTriggerVolt() //改变触发电压的值
{
	// TODO: Add your control notification handler code here
	CString str;
	m_nTringgerVolt.GetWindowText(str);
	m_TriggerVolt=wcstol(str,NULL,10);
	// 当重新设置触发电平时，重置超限数和统计总数
	m_OverLimitCount=0; 
	m_AnalyzeAllCount=0;	
}

void CADStatusView::SetDlgCheck()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDM_DigitalShow);
	pCheck->SetCheck(nProcessMode == DATA_MODE ? BST_CHECKED:BST_UNCHECKED);
	pCheck->EnableWindow(m_bFirstCreateDeviceFile);

	pCheck=(CButton*)GetDlgItem(IDM_GraphicShow);
	pCheck->SetCheck(nProcessMode == WAVE_MODE ? BST_CHECKED:BST_UNCHECKED);
	pCheck->EnableWindow(m_bFirstCreateDeviceFile);


	pCheck=(CButton*)GetDlgItem(IDM_DataSave);
	pCheck->SetCheck(nProcessMode == SAVE_MODE ? BST_CHECKED:BST_UNCHECKED);
	pCheck->EnableWindow(!m_bFirstCreateDeviceFile);    
}

 

void CADStatusView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if(lHint==1||lHint==2) return;
	CView::OnUpdate(pSender, lHint, pHint);
}

void CADStatusView::OnDigitalShow() 
{//数字方式显示
	// TODO: Add your control notification handler code here
	nProcessMode = DATA_MODE;
	SetDlgCheck();
}
void CADStatusView::OnGraphicShow() 
{//波形显示方式
	// TODO: Add your control notification handler code here

	nProcessMode = WAVE_MODE;
	SetDlgCheck();
}
void CADStatusView::OnDataSave() 
{//存盘方式显示
	// TODO: Add your control notification handler code here
	nProcessMode = SAVE_MODE;	
	SetDlgCheck();

}


void CADStatusView::OnSelchangeComboScreenVolume() 
{
	// TODO: Add your control notification handler code here
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc; 
	
	CString str;
	m_nScreenVolume.GetWindowText(str);
	gl_ScreenVolume=wcstol(str,NULL,10);
	
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	
	pWaveView->OnDrawPolyLine();
	if(pWaveView->m_ADScopeCtrl.m_bAllChannel == FALSE) //单通道显示时才能测电压
	{
		pWaveView->m_ADScopeCtrl.SetRange(-gl_ScreenVolume/2.0, gl_ScreenVolume/2.0, pWaveView->m_ADScopeCtrl.m_nChannelNum);
	}
}
