// ADWaveView.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ADWaveView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////
#include "ADStatusView.h"
#include "ParaCfgView.h"
extern CADStatusView* pADStatusView;
extern CParaCfgView* pParaCfgView;
///////////////////////
static CPoint pointxy[32][8192];
int WavePointX;	//记录鼠标X位置
BOOL bMeasureFreq=FALSE;  // 是否计算频率
int StartX, EndX;    // 测频率
COLORREF pColor[]=
{
	
	    RGB(176,15,172),
		RGB(170,21,33),
		RGB(22,158,169),
		RGB(4,187,109),
		
		RGB(176,15,172),
		RGB(170,21,33),
		RGB(22,158,169),
		RGB(4,187,109),
		
		RGB(176,15,172),
		RGB(170,21,33),
		RGB(22,158,169),
		RGB(4,187,109),
		
		RGB(176,15,172),
		RGB(170,21,33),
		RGB(22,158,169),
		RGB(4,187,109),

		
		
};

IMPLEMENT_DYNCREATE(CADWaveView, CScrollView)

CADWaveView::CADWaveView()
{
	m_Draw=FALSE;
	
	Offset=0;
	brush.CreateSolidBrush (RGB(255,255,255));
	CurrentProgress=0;  // 置采集进度条起值为0
	m_ScaleH_Width = 40;
	m_ScaleV_Width = 30;
	m_oldPoint.x =m_oldPoint.y =-1;
}

CADWaveView::~CADWaveView()
{
}


BEGIN_MESSAGE_MAP(CADWaveView, CScrollView)
//{{AFX_MSG_MAP(CADWaveView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_SHOWWAVE, DrawPolyLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADWaveView drawing
BOOL bCreate = FALSE;
void CADWaveView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
	CADDoc* pDoc = (CADDoc*)GetDocument();
	pDoc->m_hWndWave=m_hWnd;   // 取得视图句柄，以便在DOC中的线程使用
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	const SIZE conPSize={200,1};
	const SIZE conLSize={20,1};
	sizeTotal.cx = 0; 
	sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	for(int i=0;i<16;i++)
		m_Pen[i].CreatePen(PS_SOLID,1,pColor[i]);
	////////////////////////////////////////////////////////////////////////
	//创建绘图窗体
	CRect rect;
	GetClientRect(rect);
 	m_ADScopeCtrl.Create(WS_CHILD | WS_VISIBLE, CRect(10, 0, 400, 600), this);
	m_ADScopeCtrl.SetXUnits("X");
	m_ADScopeCtrl.SetYUnits("Y");
	m_ADScopeCtrl.SetRange(-5, 5, 1);
}

void CADWaveView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CADWaveView diagnostics

#ifdef _DEBUG
void CADWaveView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CADWaveView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CADDoc* CADWaveView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADDoc)));
	return (CADDoc*)m_pDocument;
}
#endif //_DEBUG

void CADWaveView::AnalyzeAccurate()//精度分析
{
	int m_TemporyVolt;
	CString StrAllCount;
	CString StrOverCount;
	CString StrOverRatio;
	int m_TriggerLsb;
	m_TriggerLsb=(int)(m_TriggerVolt*(LSB_COUNT/VOLT_RANGE));
	for(ULONG i=0;i<gl_ReadSizeWords;i++)
	{
		m_TemporyVolt=(int)((ADBuffer[0][CurrentIndex][i])-gl_MiddleLsb[i%MAX_CHANNEL_COUNT]);
		if(m_TemporyVolt>m_TriggerLsb)
		{
			m_OverLimitCount=m_OverLimitCount+1;	 
		}
		if(m_TemporyVolt<(-m_TriggerLsb))
		{
			m_OverLimitCount=m_OverLimitCount+1;	
		}
	}
	m_AnalyzeAllCount=m_AnalyzeAllCount+gl_ReadSizeWords;
	StrAllCount.Format(L"%3.2e",m_AnalyzeAllCount);
	StrOverCount.Format(L"%d",m_OverLimitCount);
	StrOverRatio.Format(L"%4.4f",m_OverLimitCount*100/m_AnalyzeAllCount);
	StrOverRatio=StrOverRatio+"%";
	pADStatusView->m_nAnalyzeCount.SetWindowText(StrAllCount);
	pADStatusView->m_nOverRange.SetWindowText(StrOverCount);
	pADStatusView->m_nOverRatio.SetWindowText(StrOverRatio);
}

void CADWaveView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
} 

BOOL CADWaveView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CBrush brush;
	CRect rect;
	GetClientRect(rect);
	brush.CreateSolidBrush(RGB(0, 0, 0));
	pDC->FillRect(rect, &brush);
	return TRUE;
// 	return CScrollView::OnEraseBkgnd(pDC);
}

void CADWaveView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	GetClientRect(&ClientRect);	 
	PerY=ClientRect.Height()*1.0/gl_nChannelCount; 
	middle1=(float)((((gl_ScreenVolume)/VOLT_RANGE)*LSB_COUNT)/(PerY-5));//每像素对应的码值    
	CRect rect;
	GetClientRect(rect);
	if(::IsWindow(m_ADScopeCtrl))//bCreate)
	{
		m_ADScopeCtrl.SetWindowPos(NULL, 0, 0, cx, cy, SWP_DRAWFRAME);
		this->OnDrawPolyLine();	
	}
}

LRESULT CADWaveView::DrawPolyLine(WPARAM wParam, LPARAM lParam)
{
	if(m_bProgress && gl_bDeviceADRun) // 刷新ADDigitView视图时，更新进度条的状态
	{
		CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // 取得子帧窗口句柄
		pProgress=(CProgressCtrl*)(pShow->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
		pProgress->SetPos(CurrentProgress);
		CurrentProgress++;
		if(CurrentProgress>50) CurrentProgress=0;
		m_bProgress=FALSE;
	}

	OnDrawPolyLine();	// 开始显示图形
	return 1;
}

void CADWaveView::OnDrawPolyLine()
{	
	m_ADScopeCtrl.AppendPoly(glBufferID, glOffset);
	
}

 

 
