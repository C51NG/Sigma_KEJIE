// ParaCfgView.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ParaCfgView.h"
#include "ADWaveView.h"
#include "ADDigitView.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParaCfgView
///////////////////////////////
int glOffset=0;
int glBufferID=0;
extern int gl_InputRange[12];
int gl_MLength = 0;
int gl_NLength = 0;
extern BOOL gl_bEnough;

static UINT IDC_COMBO_VoltRange[] ={
	IDC_COMBO_VoltRange0, IDC_COMBO_VoltRange1,
	IDC_COMBO_VoltRange2, IDC_COMBO_VoltRange3,
	IDC_COMBO_VoltRange4, IDC_COMBO_VoltRange5,
	IDC_COMBO_VoltRange6, IDC_COMBO_VoltRange7,
	IDC_COMBO_VoltRange8,IDC_COMBO_VoltRange9,
	IDC_COMBO_VoltRange10,IDC_COMBO_VoltRange11,

};
static UINT IDC_COMBO_Gains[] ={
	IDC_COMBO_Gains0, IDC_COMBO_Gains1,
	IDC_COMBO_Gains2, IDC_COMBO_Gains3,
	IDC_COMBO_Gains4, IDC_COMBO_Gains5,
	IDC_COMBO_Gains6, IDC_COMBO_Gains7,
	IDC_COMBO_Gains8,IDC_COMBO_Gains9,
	IDC_COMBO_Gains10,IDC_COMBO_Gains11,

};


static UINT IDC_CHECK_CH[] ={
	IDC_CHECK_CH0, IDC_CHECK_CH1,
	IDC_CHECK_CH2, IDC_CHECK_CH3,
	IDC_CHECK_CH4, IDC_CHECK_CH5,
	IDC_CHECK_CH6, IDC_CHECK_CH7,
	IDC_CHECK_CH8, IDC_CHECK_CH9,
	IDC_CHECK_CH10, IDC_CHECK_CH11,
};

#define m_pCOMBO_VoltRange(nNum)	((CComboBox*)GetDlgItem(IDC_COMBO_VoltRange[nNum]))	
#define m_pCOMBO_Gains(nNum)	((CComboBox*)GetDlgItem(IDC_COMBO_Gains[nNum]))	
#define m_pCheck_CH(nNum)	((CButton*)GetDlgItem(IDC_CHECK_CH[nNum]))	

#define TIMER_TRIGER	3

extern CSysApp theApp;
///////////////////////////////
IMPLEMENT_DYNCREATE(CParaCfgView, CFormView)
CParaCfgView::CParaCfgView()
: CFormView(CParaCfgView::IDD)
{
	CountItem = 4;
	m_bInitial = FALSE;
	//{{AFX_DATA_INIT(CParaCfgView)
	m_TrigLevelVolt = 0;
	m_nChannelDataBit = 0;
	//}}AFX_DATA_INIT
}

CParaCfgView::~CParaCfgView()
{
}

void CParaCfgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParaCfgView)
	DDX_Control(pDX, IDC_CHECK_bClockOutput, m_Check_bClockOutput);
	DDX_Control(pDX, IDC_EDIT_TrigLevelVolt, m_Edit_TrigLevelVolt);
	DDX_Control(pDX, IDC_COMBO_ClockSource, m_Combo_ClockSource);
	//DDX_Control(pDX, IDC_COMBO_VoltRange0, m_Combo_VoltRange0);
	DDX_Control(pDX, IDC_COMBO_Frequence, m_Combo_Frequency);
	DDX_Control(pDX, IDC_COMBO_OutTriggerDir, m_Combo_TriggerDir);
	DDX_Control(pDX, IDC_COMBO_TriggerSource, m_Combo_TriggerSource);
	DDX_Control(pDX, IDC_STATIC_Offset, m_Static_Offset);
	DDX_Control(pDX, IDC_SPIN_BufferID, m_Spin_BufferID);
	DDX_Control(pDX, IDC_SLIDER_Offset, m_Slider_Offset);
	DDX_Control(pDX, IDC_EDIT_BufferID, m_Edit_BufferID);
	DDX_Control(pDX, IDC_CHECK_Auto, m_Check_Auto);
	DDX_Control(pDX, IDC_COMBO_TriggerMode, m_ComBo_TriggerMode);
	DDX_Control(pDX, IDC_EDIT_TrigWindow, m_Edit_TrigWindow);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CParaCfgView, CFormView)
//{{AFX_MSG_MAP(CParaCfgView)
ON_CBN_SELCHANGE(IDC_COMBO_TriggerSource, OnSelchangeCOMBOTriggerSource)	
ON_WM_KILLFOCUS()
ON_CBN_SELCHANGE(IDC_COMBO_OutTriggerDir, OnSelchangeCOMBOOutTriggerDir)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Offset, OnCustomdrawSLIDEROffset)
ON_EN_CHANGE(IDC_EDIT_BufferID, OnChangeEDITBufferID)
ON_BN_CLICKED(IDC_CHECK_Auto, OnCHECKAuto)
	ON_CBN_SELCHANGE(IDC_COMBO_ClockSource, OnSelchangeCOMBOClockSource)
	ON_EN_CHANGE(IDC_EDIT_TrigLevelVolt, OnChangeEDITTrigLevelVolt)
	ON_CBN_SELCHANGE(IDC_COMBO_Frequence, OnSelchangeCOMBOFrequence)
	ON_CBN_EDITCHANGE(IDC_COMBO_Frequence, OnEditchangeCOMBOFrequence)
	ON_BN_CLICKED(IDC_CHECK_bClockOutput, OnCHECKbClockOutput)
    ON_CBN_SELCHANGE(IDC_COMBO_TriggerMode, OnSelchangeCOMBOTriggerMode)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange0, OnSelchangeCOMBOVoltRange)
	ON_BN_CLICKED(IDC_CHECK_CH0, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH1, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH2, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH3, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH4, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH5, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH6, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH7, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH8, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH9, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH10, OnCheckCh)
	ON_BN_CLICKED(IDC_CHECK_CH11, OnCheckCh)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange1, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange2, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange3, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange4, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange5, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange6, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange7, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange8, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange9, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange10, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange11, OnSelchangeCOMBOVoltRange)
    ON_CBN_SELCHANGE(IDC_COMBO_Gains0, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains1, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains2, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains3, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains4, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains5, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains6, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains7, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains8, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains9, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains10, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains11, OnSelchangeCOMBOGains)
	ON_EN_CHANGE(IDC_EDIT_TrigWindow, OnChangeEDITTrigWindow)
	
	
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_bEnough, &CParaCfgView::OnBnClickedCheckbenough)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParaCfgView diagnostics

#ifdef _DEBUG
void CParaCfgView::AssertValid() const
{
	CFormView::AssertValid();
}

void CParaCfgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CParaCfgView message handlers

void CParaCfgView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	CSysApp *pApp = (CSysApp *)AfxGetApp();
	CADDoc *pDoc = (CADDoc *)pApp->m_pADDoc;
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	pDoc->m_hWndADCfg = this->m_hWnd;
	pApp->m_hParaCfgView = this->m_hWnd;

	// 初始化用户界面视觉参数
	CString str;
	OutAnalogValve = 2000;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_bEnough);
	pBtn->SetCheck(1);


	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir);
	m_Static_Offset.SetWindowText(L"0");
	m_Spin_BufferID.SetRange(0, MAX_SEGMENT-1);	
	m_Spin_BufferID.SetBuddy(&m_Edit_BufferID);
	m_Spin_BufferID.SetPos(0);

	m_Slider_Offset.SetRange(0, MAX_RAM_SIZE/gl_nChannelCount-1); // 初始化缓冲偏移范围
	
	OldADPara = ADPara;
	CString string;
	int CurSelIndex = 0;
	string.Format(L"%dKHz", (ADPara.Frequency/1000));
	CurSelIndex = m_Combo_Frequency.FindString(-1, string);
	if(CurSelIndex==-1)
		CurSelIndex = 1;

	m_Combo_Frequency.SetCurSel(CurSelIndex); 
	OnSelchangeCOMBOFrequence();

	// 初始化采样长度
	


	// 初始化状态栏的TimeDiv框(与采样频率相对应)
	float fFrequency, fTimePixel, fTimeDiv;
	m_Combo_Frequency.GetWindowText(str);
	
	fFrequency = (float)_wtof(str.Left(5)); // 取得频率大小(MHz)
	fTimePixel = float(1000.0 / fFrequency); // 每两个像素点之间的时间(nS)
    fTimeDiv = fTimePixel*50;
	if(fTimeDiv < 1000.0)
		str.Format(L"%7.2f nS/Div", fTimeDiv);
	if(fTimeDiv>=1000.0 && fTimeDiv<1000000.0)
		str.Format(L"%7.2f uS/Div", fTimeDiv/1000);
	if(fTimeDiv>=1000000.0 && fTimeDiv<1000000000.0)
		str.Format(L"%7.2f mS/Div", fTimeDiv/1000000);
	if(fTimeDiv >= 1000000000.0)
		str.Format(L"%7.2f S/Div", fTimeDiv/1000000000);
	CStatic *pStatic = (CStatic *)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
    pStatic->SetWindowText(str);
	
	// 初始化电压量程大小为 5000毫伏
	
	for (int index=0; index<MAX_CHANNEL_COUNT; index++)
	{
		m_pCOMBO_VoltRange(index)->SetCurSel(ADPara.InputRange[index]);
		m_pCOMBO_Gains(index)->SetCurSel(ADPara.Gains[index]);
		if (ADPara.bChannelArray[index])
		{	
			m_pCheck_CH(index)->SetCheck(1);
		}
		else
		{
			m_pCOMBO_VoltRange(index)->EnableWindow(FALSE);
		    m_pCOMBO_Gains(index)->EnableWindow(FALSE);
		}
	}

	m_Combo_TriggerSource.SetCurSel(ADPara.TriggerSource);	// 初始化外触源

	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir);		// 初始化触发方向

	m_TrigLevelVolt = ADPara.TrigLevelVolt;		// 触发电平
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TrigLevelVolt); // 触发电平码值
	m_Combo_ClockSource.SetCurSel(ADPara.ClockSource);	// 初始化时钟源
	m_Check_bClockOutput.SetCheck(ADPara.bClockOutput);
    m_ComBo_TriggerMode.SetCurSel(ADPara.TriggerMode);
	str.Format(_T("%d") , ADPara.TrigWindow);
	m_Edit_TrigWindow.SetWindowText(str);


	for (int nChannel = 0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{
		switch(ADPara.InputRange[nChannel])
		{
			case USB2881_INPUT_N10000_P10000mV:
				gl_InputRange[nChannel] = 20000;
				gl_PerLsbVoltM[nChannel] = (float)(20000.0 / LSB_COUNT);
				pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, nChannel);
				break;
			case USB2881_INPUT_N5000_P5000mV:
				gl_InputRange[nChannel] = 10000;
				gl_PerLsbVoltM[nChannel] = (float)(10000.0 / LSB_COUNT);
				pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, nChannel);
				break;
			default:
				break;
		}
	}

	str.Format(L"%d", ADPara.TrigLevelVolt);
	m_Edit_TrigLevelVolt.SetWindowText(str);
	m_Check_Auto.SetCheck(TRUE); // 自动采集
	OnSelchangeCOMBOTriggerSource();
	gl_bAutoCollect = TRUE;
	m_bInitial = TRUE;
	OnCheckCh();
}

void CParaCfgView::OnSelchangeCOMBOTriggerSource() 
{
	// TODO: Add your control notification handler code here
	ADPara.TriggerSource = m_Combo_TriggerSource.GetCurSel();


}

void CParaCfgView::EnableWindows(BOOL Flag)
{

	for (int index=0; index<MAX_CHANNEL_COUNT; index++)
	{
		m_pCheck_CH(index)->EnableWindow(Flag);

		if (ADPara.bChannelArray[index])
		{
			m_pCOMBO_VoltRange(index)->EnableWindow(Flag);
		    m_pCOMBO_Gains(index)->EnableWindow(Flag);
		}
		else
		{
			m_pCOMBO_VoltRange(index)->EnableWindow(FALSE);
		    m_pCOMBO_Gains(index)->EnableWindow(FALSE);
		}
	}

	m_ComBo_TriggerMode.EnableWindow(Flag);	
	m_Combo_Frequency.EnableWindow(Flag);
	m_Combo_TriggerSource.EnableWindow(Flag);
	m_Combo_TriggerDir.EnableWindow(Flag);
	//m_Combo_VoltRange0.EnableWindow(Flag);
	m_Edit_TrigLevelVolt.EnableWindow(Flag);
	m_Combo_ClockSource.EnableWindow(Flag);
	m_Check_bClockOutput.EnableWindow(Flag);
	m_Edit_TrigWindow.EnableWindow(Flag);

	if(nProcessMode == SAVE_MODE) // 如果是存盘操作
	{
		m_Edit_BufferID.EnableWindow(Flag);
		m_Spin_BufferID.EnableWindow(Flag);
	}
}


void CParaCfgView::OnKillFocus(CWnd* pNewWnd) 
{
	CFormView::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here	
}

void CParaCfgView::OnSelchangeCOMBOOutTriggerDir() 
{
	// TODO: Add your control notification handler code here
	ADPara.TriggerDir = m_Combo_TriggerDir.GetCurSel();
}


void CParaCfgView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)  
{
	// TODO: Add your specialized code here and/or call the base class
	if(lHint==1 || lHint==2) return;
	CView::OnUpdate(pSender, lHint, pHint);
}

void CParaCfgView::OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	// TODO: Add your control notification handler code here
	if(m_bInitial)
	{ 
		CSliderCtrl* pSliderOffset=(CSliderCtrl*)(GetDlgItem(IDC_SLIDER_Offset));	
		int Pos = pSliderOffset->GetPos();  // 取得滑动条位置
		pSliderOffset->SetPos(Pos);
		
		glOffset = Pos;	//gloffset 标志在缓冲区中的偏移
		CString str;
		str.Format(L"%d", glOffset);
		CStatic* pStatic = (CStatic*)(GetDlgItem(IDC_STATIC_Offset));
		pStatic->SetWindowText(str);
		CSysApp* pApp = (CSysApp*)AfxGetApp();	
		CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndWave));
		CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndDigit));
		
		pWaveView->OnDrawPolyLine(); // 更新波形视图
		pDigitView->RedrawWindow();  // 更新数字视图
		
//		CADFrame* pADFrame = (CADFrame*)pWaveView->GetParentFrame();
//		CEdit* pEditOffset=(CEdit*)(pADFrame->m_wndShowStatus).GetDlgItem(IDC_EDIT_Offset); 
//		int Count = glOffset + WavePointX;  // 偏移总点数
//		str.Format(L"%d", Count);
		
		*pResult = 0;
	}
}

BOOL gl_bCollected = FALSE; //  是否已经进行过一次采集
void CParaCfgView::OnChangeEDITBufferID() 
{
	CString string;
	CEdit* pEditBufferID = (CEdit*)(GetDlgItem(IDC_EDIT_BufferID));	
	pEditBufferID->GetWindowText(string); 
	
	glBufferID = wcstol(string,NULL,10); // 改变ADBuffer的缓冲段号
	if(glBufferID<0 || glBufferID>32) 
	{	
		glBufferID = 0;
		pEditBufferID->SetWindowText(L"0");
	}
	CSysApp* pApp=(CSysApp*)AfxGetApp();	
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndWave));
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndDigit));
	CADDoc *pADDoc = (CADDoc *)pApp->m_pADDoc;
	if(m_bInitial)
	{
		pDigitView->RedrawWindow();  // 更新数字视图
		pWaveView->OnDrawPolyLine(); // 更新波形视图
	}
	
}

void CParaCfgView::OnCHECKAuto() // 自动采集设置
{
	if(m_Check_Auto.GetCheck())	
		gl_bAutoCollect = TRUE;
	else 
		gl_bAutoCollect = FALSE;
}



void CParaCfgView::OnSelchangeCOMBOClockSource() 
{
	// TODO: Add your control notification handler code here
	ADPara.ClockSource = m_Combo_ClockSource.GetCurSel();
	if(ADPara.ClockSource == USB2881_CLOCKSRC_IN)
	{
		m_Combo_Frequency.EnableWindow(TRUE);
	}
	else
	{
		m_Combo_Frequency.EnableWindow(FALSE);
	}
}

void CParaCfgView::OnChangeEDITTrigLevelVolt() 
{
	// TODO: Add your control notification handler code here

	CString str;
	this->m_Edit_TrigLevelVolt.GetWindowText(str);
	LONG TrigLevelVolt = (LONG)wcstol(str, NULL, 10);
	
	//LONG VoltTopRange, VoltBottomRange;
	//
	//// 还没有设置
	//for (int nCH=0;nCH<MAX_CHANNEL_COUNT;nCH++)
	//{
 //    switch(ADPara.InputRange[nCH])	// 电压量程
	//{		
	//case USB2881_INPUT_N10000_P10000mV: // ±1000毫伏
	//	VoltTopRange = 10000;
	//	VoltBottomRange = -10000;
	//	break;
	//case USB2881_INPUT_N5000_P5000mV: // ±5000毫伏
	//	VoltTopRange = 5000;
	//	VoltBottomRange = -5000;
	//	break;
	//}	
	//}

	if(TrigLevelVolt<-10000/*VoltBottomRange*/ || TrigLevelVolt>10000/*VoltTopRange*/)
	{
		str.Format(L"触发电平的范围为：%dmV～%dmV", -10000/*VoltBottomRange*/,10000/*VoltTopRange*/);
		MessageBox(str, L"Information", MB_ICONWARNING | MB_OK);
		if(ADPara.TrigLevelVolt<-10000/*VoltBottomRange*/ || ADPara.TrigLevelVolt>10000/*VoltTopRange*/)
		{
			ADPara.TrigLevelVolt =10000/*VoltTopRange*/; 
		}
		str.Format(L"%ld", ADPara.TrigLevelVolt);
		this->m_Edit_TrigLevelVolt.SetWindowText(str);
	}
	else
	{
		ADPara.TrigLevelVolt = TrigLevelVolt;
	}
		
}

void CParaCfgView::OnSelchangeCOMBOFrequence() 
{
	// TODO: Add your control notification handler code here
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CString str;
	double FrequencyMHz;
	m_Combo_Frequency.SetCurSel(m_Combo_Frequency.GetCurSel());
	m_Combo_Frequency.GetWindowText(str);
	FrequencyMHz = _wtof(str);    // 保存分频数
	ADPara.Frequency = (LONG)(FrequencyMHz * 1000);

	float fFrequency = (float)_wtof(str.Left(8));  // 取得频率值(MHz)
	float fTimePixel = float(1000.0 / fFrequency); // 求每两点间的周期(nS)
    float fTimeDiv = fTimePixel*50; // 求每大格的周期(每大格为50个象素,且每个象素即代表一个AD点)

	if(fTimeDiv < 1000.0)
		str.Format(L"%7.2f nS/Div", fTimeDiv);
	if(fTimeDiv>=1000.0 && fTimeDiv<1000000.0)
		str.Format(L"%7.2f uS/Div", fTimeDiv/1000);
	if(fTimeDiv>=1000000.0 && fTimeDiv<1000000000.0)
		str.Format(L"%7.2f mS/Div", fTimeDiv/1000000);
	if(fTimeDiv >= 1000000000.0)
		str.Format(L"%7.2f S/Div", fTimeDiv/1000000000);
	// 设置状态栏的方格时间
	CStatic *pStatic = (CStatic *)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
    pStatic->SetWindowText(str);	
}

void CParaCfgView::OnEditchangeCOMBOFrequence() 
{
	// TODO: Add your control notification handler code here
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CString str;
	double FrequencyMHz;
	m_Combo_Frequency.GetWindowText(str);
	FrequencyMHz = _wtof(str);    // 保存分频数
	ADPara.Frequency = (LONG)(FrequencyMHz * 1000);

	float fFrequency = (float)_wtof(str.Left(8));  // 取得频率值(MHz)
	float fTimePixel = float(1000.0 / fFrequency); // 求每两点间的周期(nS)
    float fTimeDiv = fTimePixel*50; // 求每大格的周期(每大格为50个象素,且每个象素即代表一个AD点)

	if(fTimeDiv < 1000.0)
		str.Format(L"%7.2f nS/Div", fTimeDiv);
	if(fTimeDiv>=1000.0 && fTimeDiv<1000000.0)
		str.Format(L"%7.2f uS/Div", fTimeDiv/1000);
	if(fTimeDiv>=1000000.0 && fTimeDiv<1000000000.0)
		str.Format(L"%7.2f mS/Div", fTimeDiv/1000000);
	if(fTimeDiv >= 1000000000.0)
		str.Format(L"%7.2f S/Div", fTimeDiv/1000000000);
	// 设置状态栏的方格时间
	CStatic *pStatic = (CStatic *)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
    pStatic->SetWindowText(str);
}


void CParaCfgView::OnCHECKbClockOutput() 
{
	// TODO: Add your control notification handler code here
	ADPara.bClockOutput = m_Check_bClockOutput.GetCheck();
}


void CParaCfgView::StartAD()
{
	
 	if (/*m_Check_TimerTrig.GetCheck() &&*/ ADPara.TriggerSource == USB2881_TRIGMODE_SOFT)
 	{
 		
 		CString str;
 		//m_Edit_TrigInterva.GetWindowText(str);	
 		SetTimer(TIMER_TRIGER,wcstol(str, NULL, 10), NULL);
 		
 	}
}

void CParaCfgView::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nIDEvent == TIMER_TRIGER)
	{
		
		if (gl_bDeviceADRun)
		{
			//USB2881_SetDeviceTrigAD(theApp.m_hDevice);
		}
		else
		{
			KillTimer(TIMER_TRIGER);
		}
		
	}

	CFormView::OnTimer(nIDEvent);
}

void CParaCfgView::OnSelchangeCOMBOVoltRange() 
{
	// TODO: Add your control notification handler code here
	CSysApp *pApp=(CSysApp*)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc;
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave));
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit));

	const MSG* pMsg = GetCurrentMessage();
	int nChannel;
	for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{
		
		if (pMsg->lParam == (LPARAM)m_pCOMBO_VoltRange(nChannel)->m_hWnd)
		{
			//ADPara.InputRange[nChannel] = m_pCOMBO_VoltRange(nChannel)->GetCurSel();
			if (nChannel<6)
			{
				for (int nCH=0; nCH<6; nCH++)
				{
					ADPara.InputRange[nCH] = m_pCOMBO_VoltRange(nChannel)->GetCurSel();
					m_pCOMBO_VoltRange(nCH)->SetCurSel(ADPara.InputRange[nCH]);
				}
			}
			else
			{
				for (int nCH=6; nCH<12; nCH++)
				{
					ADPara.InputRange[nCH] = m_pCOMBO_VoltRange(nChannel)->GetCurSel();
					m_pCOMBO_VoltRange(nCH)->SetCurSel(ADPara.InputRange[nCH]);
				}
			}
			break;
		}
	}

	
	for ( nChannel = 0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{
		switch(ADPara.InputRange[nChannel])
		{
		case USB2881_INPUT_N10000_P10000mV:
			//gl_ScreenVolume = 20000;
			gl_InputRange[nChannel]= 20000;
			gl_PerLsbVoltM[nChannel] = (float)(20000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, nChannel);
			break;
		case USB2881_INPUT_N5000_P5000mV:
			//gl_ScreenVolume = 10000;
			gl_InputRange[nChannel] = 10000;
			gl_PerLsbVoltM[nChannel] = (float)(10000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, nChannel);
			break;
		default:
			break;
		}
	}
	pDigitView->Invalidate(); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
}
void CParaCfgView::OnSelchangeCOMBOGains() 
{
	// TODO: Add your control notification handler code here
	CSysApp *pApp=(CSysApp*)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc;
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave));
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit));

	const MSG* pMsg = GetCurrentMessage();
	int nChannel;
	for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{

		if (pMsg->lParam == (LPARAM)m_pCOMBO_Gains(nChannel)->m_hWnd)
		{
			ADPara.Gains[nChannel] = m_pCOMBO_Gains(nChannel)->GetCurSel();
			break;
		}
	}


	
	pDigitView->Invalidate(); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
}


void CParaCfgView::OnCheckCh() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	BOOL bFlag = FALSE;

	for (int nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{		
		if (pMsg->lParam == (LPARAM)m_pCheck_CH(nChannel)->m_hWnd)
		{
			ADPara.bChannelArray[nChannel] = m_pCheck_CH(nChannel)->GetCheck();
			if (m_pCheck_CH(nChannel)->GetCheck())
			{
				m_pCOMBO_VoltRange(nChannel)->EnableWindow(TRUE);
				m_pCOMBO_Gains(nChannel)->EnableWindow(TRUE);
			}
			else
			{
				m_pCOMBO_VoltRange(nChannel)->EnableWindow(FALSE);
				m_pCOMBO_Gains(nChannel)->EnableWindow(FALSE);
				for (int index=0; index<MAX_CHANNEL_COUNT; index++)
				{
					if (ADPara.bChannelArray[index])
						bFlag = TRUE;
				}
				if (!bFlag)
				{
					MsgBox(_T("请至少选择一个通道"));
					m_pCheck_CH(nChannel)->SetCheck(1);
					ADPara.bChannelArray[nChannel] = TRUE;
					m_pCOMBO_VoltRange(nChannel)->EnableWindow(TRUE);
					m_pCOMBO_Gains(nChannel)->EnableWindow(TRUE);
					
				}
			}
			break;
		}
	}
	INT uCHCount = 0;
	for (int nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{	
		if (ADPara.bChannelArray[nChannel])
		{
			uCHCount++;
		}
	}

	gl_nChannelCount = uCHCount;

	CADDoc* pDoc=theApp.m_pADDoc;
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave));
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit));

	pDigitView->Invalidate(); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	

	ULONG ulCount = 0;
	for (INT nCH=0; nCH<MAX_CHANNEL_COUNT; nCH++)
	{
		if (ADPara.bChannelArray[nCH] == TRUE)
		{
			gl_ChannelArrary[ulCount] = nCH;
			ulCount++;
		}
	}



}
void CParaCfgView::OnSelchangeCOMBOTriggerMode() 
{
	// TODO: Add your control notification handler code here


	ADPara.TriggerMode = m_ComBo_TriggerMode.GetCurSel();

}
void CParaCfgView::OnChangeEDITTrigWindow() 
{
	CString strTrigWindow;
	m_Edit_TrigWindow.GetWindowText(strTrigWindow);
	long TrigWindow = _tcstol(strTrigWindow, NULL, 10);
	if (TrigWindow > 0 && TrigWindow <=50)
	{
		ADPara.TrigWindow = TrigWindow;
	}
	else
	{
		AfxMessageBox(_T("输入错误"));
		ADPara.TrigWindow = 1;
		m_Edit_TrigWindow.SetWindowText(_T("1"));
	}

}








void CParaCfgView::OnBnClickedCheckbenough()
{
	// TODO: 在此添加控件通知处理程序代码

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_bEnough);
	gl_bEnough = pBtn->GetCheck();
}
