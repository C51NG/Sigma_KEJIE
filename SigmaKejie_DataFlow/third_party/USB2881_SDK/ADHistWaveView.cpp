// HistWaveView.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ADHistFrm.h"
#include "ADHistWaveView.h"
#include "ADHistDigitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView
IMPLEMENT_DYNCREATE(CADHistWaveView, CScrollView)

CADHistWaveView::CADHistWaveView()
{
	m_Index=0;
	m_ScreenVolume=(int)VOLT_RANGE;  // 定义量程为10V	
	m_MiddleLsb = 0;   // 偏移0个LSB
	m_oldPoint.x=-1;	
	m_bCapture=FALSE;
	m_Draw=FALSE;
}

CADHistWaveView::~CADHistWaveView()
{
}

BEGIN_MESSAGE_MAP(CADHistWaveView, CScrollView)
	//{{AFX_MSG_MAP(CADHistWaveView)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()	
	ON_WM_LBUTTONDBLCLK()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView drawing

void CADHistWaveView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument(); 
	m_nWidth=sizeTotal.cx = (int)(pDoc->m_ReadDataSize/1/*pDoc->m_ChannelCount*/)-1/*pDoc->m_ChannelCount*/;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	pDoc->m_hWndWave=m_hWnd; 

}
void CADHistWaveView::OnSize(UINT nType, int cx, int cy)
{
	if(this->GetSafeHwnd())
		GetClientRect(m_rcClient);	
	//CWnd::OnSize(nType, cx, cy);
	CScrollView::OnSize(nType, cx, cy);

}

void CADHistWaveView::OnDraw(CDC* pDC)
{
	ShowWave(pDC);
	CRect rect;
	int nDrawMode = pDC->SetROP2(R2_NOTXORPEN);
	pDC->MoveTo(m_oldPoint.x,0);
	pDC->LineTo(m_oldPoint.x ,m_rcClient.Height());//涂去上次画的线
	pDC->SetROP2(nDrawMode);
}

/////////////////////////////////////////////////////////////////////////////
// CShowWaveView message handlers
void CADHistWaveView::ShowWave(CDC *pDC)//绘制波形
{
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	
	CPen Pen;
	CPen CenterPen;
	CPen BackGroundPen;
	CPen* OldPen;

	Pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	CenterPen.CreatePen(PS_DOT,1,RGB(255,255,0));
	BackGroundPen.CreatePen(PS_DOT,1,RGB(0,255,255));

	GetClientRect(m_rcClient);
	OnPrepareDC(pDC);
	pDC->DPtoLP(&m_rcClient);

	int Height = m_rcClient.Height()/10;
	OldPen = pDC->SelectObject(&BackGroundPen);
	
	for(int h=0 ;h<m_rcClient.bottom;h = h+Height)
	{
		pDC->MoveTo(0,h);
		pDC->LineTo(m_rcClient.right,h);
	}
	for(int k=0; k < m_rcClient.right; k = k+Height)
	{
		pDC->MoveTo(k,0);
		pDC->LineTo(k,m_rcClient.bottom);
	}
	m_center = m_rcClient.Height()/2;
	int Offset = m_rcClient.left;  // 保存窗口最左边的位置

	float	middle1 = (float)((((m_ScreenVolume)/VOLT_RANGE)*LSB_COUNT)/(PerY));//每像素对应的码值
	
	pDC->SelectObject(&CenterPen);
	pDC->MoveTo(m_rcClient.left, m_rcClient.CenterPoint().y);
	pDC->LineTo(m_rcClient.right, m_rcClient.CenterPoint().y);
	
	pDC->SelectObject(&Pen);

	int y1, y2;	
 	int iHead = 0; // 真正开始处
// 	if(pDoc->m_Header.ADStatus.bAheadTrig)
// 	{
// 		iHead = 0;
// 	}
	int iDataLength = pDoc->m_ReadDataSize - 1;
	for(int i = m_rcClient.left - 3; i < m_rcClient.right + 2 && i < iDataLength; i++)
 	{
		if(i < iHead)
			continue;		
		y1 = (int)((m_center)-(int)((((((pDoc->m_ADBuffer[i]^0x8000)&MAX_LSB)-LSB_HALF-m_MiddleLsb)))
			*m_rcClient.Height())/ LSB_COUNT);
		y2 = (int)((m_center)-(int)((((((pDoc->m_ADBuffer[i+1]^0x8000)&MAX_LSB)-LSB_HALF-m_MiddleLsb)))
			*m_rcClient.Height())/LSB_COUNT);
		pDC->MoveTo(i, y1); 
		pDC->LineTo(i + 1, y2);
	}		
	
	pDC->SelectObject(OldPen);   // 恢复原笔
}


/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView diagnostics

#ifdef _DEBUG
void CADHistWaveView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CADHistWaveView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView message handlers

void CADHistWaveView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// 显示十字光标
	HCURSOR m_WaveCursor;
	m_WaveCursor = ::AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	::SetCursor(m_WaveCursor);
	/////////////////////////////////////////////////////////////////////
	CADHistFrm* pFrame=(CADHistFrm*)GetParentFrame();
	CEdit* pPos=(CEdit*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	if(m_bCapture==TRUE)
	{
		int nDrawMode=dc.SetROP2(R2_NOTXORPEN);
		dc.MoveTo(m_oldPoint.x,0);
		dc.LineTo(m_oldPoint.x ,m_rcClient.Height());//涂去上次画的线
		m_oldPoint=point;
		dc.MoveTo(point.x,0);
		dc.LineTo(point.x,m_rcClient.Height());//画新线
		dc.SetROP2(nDrawMode);
	}
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument(); 
	CADHistDigitView* pView=(CADHistDigitView*)CWnd::FromHandle(pDoc->m_hWndDigit);
	CString str;
	pDoc->m_nMouseMoveX = point.x;
	pDoc->m_ScreenOffset=pDoc->m_Offset+point.x;
//		TRACE("pDoc->m_ScreenOffset=pDoc->m_Offset + %d\n", point.x);
	str.Format(L"%d", pDoc->m_ScreenOffset);
	pPos->SetWindowText(str);		
	CScrollView::OnMouseMove(nFlags, point);
}


void CADHistWaveView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDC* pDC=GetDC();
	OnPrepareDC(pDC);
	pDC->DPtoLP(&point);

	int		nDrawMode;
	m_bCapture=TRUE;
	SetCapture();
	nDrawMode=pDC->SetROP2(R2_NOTXORPEN);
	pDC->MoveTo(m_oldPoint.x,0);
	pDC->LineTo(m_oldPoint.x ,m_rcClient.Height());//涂去上次画的线
	pDC->MoveTo(point.x,0);
	pDC->LineTo(point.x,m_rcClient.Height());//画新线
	pDC->SetROP2(nDrawMode);
	m_oldPoint=point;
	
	this->ReleaseDC(pDC);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CADHistWaveView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDC* pDC=GetDC();
	OnPrepareDC(pDC);
	pDC->DPtoLP(&point);
	CRect rect;
	int nDrawMode;	
	m_bCapture=FALSE;
	ReleaseCapture();
	nDrawMode=pDC->SetROP2(R2_NOTXORPEN);
	pDC->MoveTo(m_oldPoint.x,0);
	pDC->LineTo(m_oldPoint.x,m_rcClient.Height());//涂去上次画的线
	pDC->MoveTo(point.x,0);
	pDC->LineTo(point.x,m_rcClient.Height());//画新线
	pDC->SetROP2(nDrawMode);
	m_oldPoint=point;
	ReleaseDC(pDC);//释放CDC
	
	CScrollView::OnLButtonUp(nFlags, point);
}

void CADHistWaveView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	int nDrawMode;
	CClientDC dc(this);
	CPoint point,point1;
	GetCursorPos(&point);
	point1=m_oldPoint;	
	OnPrepareDC(&dc);
	CSize size(point1.x,point1.y );	
	dc.LPtoDP (&point1);
	CADHistDoc* pDoc=(CADHistDoc*)GetDocument();
	switch(nChar)
	{//用键盘移动鼠标既标线
		case VK_RIGHT:
			nDrawMode=dc.SetROP2(R2_NOTXORPEN);
			dc.MoveTo(m_oldPoint.x,0);
			dc.LineTo(m_oldPoint.x ,m_rcClient.Height());//涂去上次画的线
			m_oldPoint.x++;
			dc.MoveTo(m_oldPoint.x,0);
			dc.LineTo(m_oldPoint.x,m_rcClient.Height());//画线
			dc.SetROP2(nDrawMode);

			SetCursorPos((++point1.x+pDoc->m_nDigitWidth) ,point.y  );
			break;
		case VK_LEFT:
			nDrawMode=dc.SetROP2(R2_NOTXORPEN);
			dc.MoveTo(m_oldPoint.x,0);
			dc.LineTo(m_oldPoint.x ,m_rcClient.Height());//涂去上次画的线
			m_oldPoint.x--;
			dc.MoveTo(m_oldPoint.x,0);
			dc.LineTo(m_oldPoint.x,m_rcClient.Height());//画线
			dc.SetROP2(nDrawMode);

			SetCursorPos((--point1.x+pDoc->m_nDigitWidth),point.y );
			break;
// 		case VK_PAGE_DOWN:

	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL bFirstPosition = TRUE;
void CADHistWaveView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CADHistFrm* pFrame = (CADHistFrm*)GetParentFrame();
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	CButton* pRadioA = (CButton*)pFrame->m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionA);
	CButton* pRadioB = (CButton*)pFrame->m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionB);
	CStatic* pStaticA = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionA);
	CStatic* pStaticB = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionB);
	CStatic* pDistanceAB = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_DistanceAB);
	CStatic* pStaticFre = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_Frequency);
	CString str;
	long nHistFrequency;
	float fCalFrequency = 0;
	int nPositionA, nPositionB;
	int nCurPosition = pDoc->m_Offset + pDoc->m_nMouseMoveX; // pDoc->m_nMouseMoveX为鼠标移动的x坐标取代鼠标按下的坐标point.x
	str.Format(L"%d", nCurPosition);
	pRadioA->SetCheck(bFirstPosition==TRUE ? 1:0);
	pRadioB->SetCheck(bFirstPosition==TRUE ? 0:1);

	if(pRadioA->GetCheck())
	{
		pStaticA->SetWindowText(str);
	}
	if(pRadioB->GetCheck())
	{
		pStaticB->SetWindowText(str);
	}
	int nDistanceAB = 0;
	nHistFrequency = pDoc->m_Header.ADPara.Frequency; // 从文件头中取得采样频率

	pStaticA->GetWindowText(str);
	nPositionA = wcstol(str, NULL, 10);
	pStaticB->GetWindowText(str);
	nPositionB = wcstol(str, NULL, 10);
	nDistanceAB = abs(nPositionA - nPositionB);
	str.Format(L"%d", nDistanceAB);
	pDistanceAB->SetWindowText(str);
	if(nDistanceAB)
	{
		
		fCalFrequency = (float)((nHistFrequency)/nDistanceAB);
		if(fCalFrequency < 1000)
			str.Format(L"%7.2f Hz", fCalFrequency);
		if(fCalFrequency > 1000 && fCalFrequency < 1000000)
			str.Format(L"%7.2f KHz", fCalFrequency / 1000);
		if(fCalFrequency > 1000000)
			str.Format(L"%7.2f MHz", fCalFrequency / 1000000);		
		pStaticFre->SetWindowText(str);
	}
	bFirstPosition = !bFirstPosition;	
	CScrollView::OnLButtonDblClk(nFlags, point);
}


void CADHistWaveView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	CADHistDigitView* pView = (CADHistDigitView*)CView::FromHandle(pDoc->m_hWndDigit);
	
	static int nLastPos = 0;
	pView->SetScrollPos(SB_VERT, nPos);
	CListCtrl& listctrl = pView->GetListCtrl();
	listctrl.Scroll(CSize(0, nPos - nLastPos));
	nLastPos = nPos;

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}
