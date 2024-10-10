// OScopeCtrl.cpp : implementation file//

#include "stdafx.h"
#include "math.h"
#include "sys.h"
#include "ParaCfgView.h"
#include "ADScopeCtrl.h"
#include "ADFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif
//////////////////////////////////////////////////
#define MAX_OFFSET 8192
int gl_moveY = 0;
extern int gl_MLength;
extern int gl_NLength;
extern CParaCfgView* pParaCfgView;
extern USB2881_PARA_AD ADPara;
extern int gl_InputRange[12];

/////////////////////////////////////////////////////////////////////////////
// CADScopeCtrl
CADScopeCtrl::CADScopeCtrl()
{
 	m_dPreviousPosition =   0.0 ;	
 	m_nYDecimals = 3 ;
	for(int j=0; j<MAX_CHANNEL_COUNT; j++)
	{
		m_dLowerLimit[j] = -1000.0 ;
		m_dUpperLimit[j] =  1000.0 ;
	}
	m_dRange      =  double(m_dUpperLimit - m_dLowerLimit);    
	m_nShiftPixels     = 4 ;
	m_nHalfShiftPixels = m_nShiftPixels/2 ;                      
	m_nPlotShiftPixels = m_nShiftPixels + m_nHalfShiftPixels ;   
	
 	m_crBackColor  = RGB(  0,   0,   0) ;  
	m_crGridColor  = RGB(  0, 255, 255) ;  
	m_crPlotColor  = RGB(255, 128, 0) ; 
	m_clPen[0] = RGB(255, 0, 0);
	m_clPen[1] = RGB(255, 255, 0);
	m_clPen[2] = RGB(0, 255, 0);
	m_clPen[3] = RGB(255, 0, 255);
	m_clPen[4] = RGB(255, 0, 0);
	m_clPen[5] = RGB(255, 255, 0);
	m_clPen[6] = RGB(0, 255, 0);
	m_clPen[7] = RGB(255, 0, 255);
	m_clPen[8] = RGB(255, 0, 0);
	m_clPen[9] = RGB(255, 255, 0);
	m_clPen[10] = RGB(0, 255, 0);
	m_clPen[11] = RGB(255, 0, 255);

	m_crLineAColor = RGB(255, 192, 255);  // A�ߵ���ɫ
    m_crLineBColor = RGB(192, 255, 255);  // B�ߵ���ɫ
    m_crLineVColor = RGB(255, 255, 128);  // C�ߵ���ɫ

	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor) ;
	m_brushBack.CreateSolidBrush(m_crBackColor) ;
	
	m_strXUnitsString.Format(L"Samples") ;  
	m_strYUnitsString.Format(L"Y units") ;  
	
	m_pbitmapOldGrid = NULL ;
	m_pbitmapOldPlot = NULL ;
	m_nChannelCount = gl_nChannelCount; // ȱʡʱ��ͨ����Ϊ4
	m_bDrawPoly = FALSE; 
	m_bAllChannel = TRUE;
	m_bInitialed = FALSE;
	m_nPtCount = 0; 
	tempCount = m_nChannelCount;
	m_OldMoveX1 = m_CurMoveX1 = 0;
	m_OldMoveX2 = m_CurMoveX2 = 0;
	m_CurMoveY = 0;
	m_CurVoltageY = 0;
	m_OldVoltageY = 0;
	m_nLineIndex = 0;
	m_iDataLength = 0;
	for(int i=0; i<MAX_CHANNEL_COUNT; i++)
		m_ChannelPen[i].CreatePen(PS_SOLID, 1, m_clPen[i]);

}  // CADScopeCtrl

/////////////////////////////////////////////////////////////////////////////

CADScopeCtrl::~CADScopeCtrl()
{
	if (m_pbitmapOldGrid != NULL)
		m_dcGrid.SelectObject(m_pbitmapOldGrid);
	if (m_pbitmapOldPlot != NULL)
		m_dcPlot.SelectObject(m_pbitmapOldPlot);

} // ~CADScopeCtrl


BEGIN_MESSAGE_MAP(CADScopeCtrl, CWnd)
//{{AFX_MSG_MAP(CADScopeCtrl)
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_CREATE()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CADScopeCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
BOOL CADScopeCtrl::Create(DWORD dwStyle, const RECT& rect, 
                         CWnd* pParentWnd, HMENU nID) 
{
	BOOL result ;
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) ;
		// otherwise we need to register a new class
	result = CWnd::CreateEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 
		className, NULL, dwStyle, 
		rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
		pParentWnd->GetSafeHwnd(), (HMENU)nID) ;
	//	if (result != 0)
	//		InvalidateCtrl() ;
	
	return result ;
	
} // Create

/////////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetRange(double dLower, double dUpper, int nChannel)
{ // ���ô�ֱ�����������̵Ĵ�С
	ASSERT(dUpper > dLower) ;

	m_dLowerLimit[nChannel]  = dLower ;
	m_dUpperLimit[nChannel]  = dUpper ;
	m_nYDecimals      = 2 ;
	m_dRange          = double(m_dUpperLimit[nChannel] - m_dLowerLimit[nChannel]);
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange ; 
	
	InvalidateCtrl() ;
	
}  // SetRange


/////////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetXUnits(CString string)
{
	m_strXUnitsString = string ;
	
	InvalidateCtrl() ;
	
}  // SetXUnits

/////////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetYUnits(CString string)
{
	m_strYUnitsString = string ;
	
	InvalidateCtrl() ;
	
}  // SetYUnits

/////////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetGridColor(COLORREF color)
{
	m_crGridColor = color ;
	
	InvalidateCtrl() ;
	
}  // SetGridColor


/////////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetPlotColor(COLORREF color)
{
	m_crPlotColor = color ;
	
	m_penPlot.DeleteObject() ;
	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor) ;
	
	InvalidateCtrl() ;
	
}  // SetPlotColor


/////////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetBackgroundColor(COLORREF color)
{
	m_crBackColor = color ;
	
	m_brushBack.DeleteObject() ;
	m_brushBack.CreateSolidBrush(m_crBackColor) ;
	
	InvalidateCtrl() ;
	
}  // SetBackgroundColor

/////////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::InvalidateCtrl()
{

	CClientDC dc(this);
	if (m_dcGrid.GetSafeHdc() == NULL)
	{
		m_dcGrid.CreateCompatibleDC(&dc) ;	
		m_bitmapGrid.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
		m_pbitmapOldGrid = m_dcGrid.SelectObject(&m_bitmapGrid) ;
	}
	if(m_dcPlot.GetSafeHdc() == NULL)
	{
		m_dcPlot.CreateCompatibleDC(&dc) ;
		m_bitmapPlot.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
		m_pbitmapOldPlot = m_dcPlot.SelectObject(&m_bitmapPlot) ;
	}
	if(m_PenLineA.m_hObject == NULL)
			m_PenLineA.CreatePen(PS_DASHDOT, 1, m_crLineAColor); // ����A�ߵĻ���
	if(m_PenLineB.m_hObject == NULL)
			m_PenLineB.CreatePen(PS_DASHDOT, 1, m_crLineBColor); // ����B�ߵĻ���
	if(m_PenLineV.m_hObject == NULL)
			m_PenLineV.CreatePen(PS_DASHDOT, 1, m_crLineVColor); // ����V�ߵĻ���
	for(int i=0; i<MAX_CHANNEL_COUNT; i++)
	{
		if(m_polyPen[i].m_hObject == NULL)
		m_polyPen[i].CreatePen(PS_SOLID, 1, m_clPen[i]);
	}
	
	m_nChannelCount = gl_nChannelCount; // ȱʡʱ��ͨ����Ϊ4
	if(!gl_bTileWave) m_nChannelCount = 1; // ����ǵ�����ʾ���õ�ͨ����ʾ�ķ�ʽ

// ��û������
	
// 	if (m_bAllChannel)
// 	{
// 		for(int j=0; j<MAX_CHANNEL_COUNT; j++)
// 		{
// 			switch(ADPara.InputRange[j])
// 			{	
// 			case USB2881_INPUT_N10000_P10000mV: // -1000mV - +1000mV
// 				m_dLowerLimit[j] = -10000;
// 				m_dUpperLimit[j] = 10000;
// 				break;
// 			case USB2881_INPUT_N5000_P5000mV:	// -5000mV - +5000mV
// 				m_dLowerLimit[j] = -5000;
// 				m_dUpperLimit[j] = 5000;
// 				break;
// 			}
// 		}
// 	}

	if(!gl_bTileWave) this->m_nChannelCount = 1;
	DrawBkGnd();  // ������
	DrawPoly();  
	ProcessData();
	m_bInitialed = TRUE;
} // InvalidateCtrl

/////////////////////////////////////////////////////////////////////////////
double CADScopeCtrl::AppendPoint(double dNewPoint)
{
	
	double dPrevious ;
	
	dPrevious = m_dCurrentPosition ;
	m_dCurrentPosition = dNewPoint ;
	DrawPoint() ;
	
	Invalidate() ;
	
	return dPrevious ;
	
} // AppendPoint

////////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::OnPaint() 
{ // �����m_nClientWidth, m_ClientHeightҲ��ȡ����m_rectClient�Ĵ�С
	CPaintDC dc(this) ;  // device context for painting
	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // bitmap originally found in CMemDC
	memDC.CreateCompatibleDC(&dc) ;
	memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;
	CString str;
	
	if (memDC.GetSafeHdc() != NULL)
	{
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&m_dcGrid, 0, 0, SRCCOPY) ;
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&m_dcPlot, 0, 0, SRCPAINT) ;  //SRCPAINT
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&m_dcMove, 0, 0, SRCPAINT) ;  //SRCPAINT		
		dc.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&memDC, 0, 0, SRCCOPY);       //SRCCOPY) ;
		
	}
	memDC.SelectObject(oldBitmap) ;
	
	int	nDrawMode;
	dc.SetBkMode(TRANSPARENT);		                // ���ñ���Ϊ͸��
	nDrawMode = dc.SetROP2(R2_XORPEN);              // ���û���ģʽ
	
	dc.SelectObject(&m_PenLineA);                   // ѡ����A�Ļ���
	dc.MoveTo(m_OldMoveX1/*+100*/, m_rectPlot.top);
	dc.LineTo(m_OldMoveX1, m_rectPlot.Height()+10); // ����A
	dc.SetTextColor(RGB(255, 255, 0));
	dc.TextOut(m_OldMoveX1, m_rectPlot.Height()+13, L"������");
	
	dc.SelectObject(&m_PenLineB);				    // ѡ������B�Ļ���
	dc.MoveTo(m_OldMoveX2, m_rectPlot.top);
	dc.LineTo(m_OldMoveX2, m_rectPlot.Height()+10); // ����B	
	if(!m_bAllChannel)
	{
		dc.SelectObject(&m_PenLineV);				// ѡ������V�Ļ���
		dc.MoveTo(m_rectPlot.left, m_OldVoltageY);
		dc.LineTo(m_rectPlot.right, m_OldVoltageY);	// ����V
	}
	dc.SetROP2(nDrawMode);
} // OnPaint

//#####################################################################
// �������ĵ�(λͼ���ƶ�������̬)
void CADScopeCtrl::DrawPoint()
{
	int currX, prevX, currY, prevY ;
	
	CPen *oldPen ;
	CRect rectCleanUp ;
	
	if (m_dcPlot.GetSafeHdc() != NULL)
	{
		m_dcPlot.BitBlt(m_rectPlot.left, m_rectPlot.top+1, 
			m_nPlotWidth, m_nPlotHeight, &m_dcPlot, 
			m_rectPlot.left+1/*2/*m_nShiftPixels*/, m_rectPlot.top+1, 
			SRCCOPY) ;
		
		
		rectCleanUp = m_rectPlot ;
		rectCleanUp.left  = rectCleanUp.right - 1;
		
		m_dcPlot.FillRect(rectCleanUp, &m_brushBack) ;
		
		oldPen = m_dcPlot.SelectObject(&m_penPlot) ;
		
		prevX = m_rectPlot.right-2;//m_nPlotShiftPixels ;

		prevY = m_rectPlot.bottom - (long)m_dPreviousPosition-10;
		m_dcPlot.MoveTo (prevX, prevY) ;
		
		currX = m_rectPlot.right-1;//m_nHalfShiftPixels ;
		currY = m_rectPlot.bottom -(long)m_dCurrentPosition-10;
		m_dcPlot.LineTo (currX, currY);
		
		m_dcPlot.SelectObject(oldPen) ;
		
		m_dPreviousPosition = m_dCurrentPosition ;
		
	}
	
} // end DrawPoint
/////////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy) ;
	if(m_bInitialed) 
	{ 
		GetClientRect(m_rectClient) ; // ��ȡ��ǰ�Ŀͻ�����С
		
		m_nClientHeight = m_rectClient.Height() ; 
		m_nClientWidth  = m_rectClient.Width() ;
		
		m_rectPlot.left   = 60 ;  
		m_rectPlot.top    = 10 ;
		m_rectPlot.right  = m_rectClient.right-10 ;
		m_rectPlot.bottom = m_rectClient.bottom-25 ;
		
		m_nPlotHeight = m_rectPlot.Height() ;
		m_nPlotWidth  = m_rectPlot.Width() ;
		
		m_dVerticalFactor = (double)m_nPlotHeight / m_dRange ; 	
		gl_moveY = m_rectPlot.top-5;
		InvalidateCtrl(); // ���»���
		
		m_OldVoltageY = m_CurVoltageY = m_rectPlot.top + 20;
		m_OldMoveX1   = m_CurMoveX1 = m_rectPlot.left /*+ ADPara.M_Length*/;	// 20;
		m_OldMoveX2   = m_CurMoveX2 = m_rectPlot.right - 20;
		m_OldVoltageY = m_CurVoltageY = m_rectPlot.top + 20;

	}
} // OnSize


/////////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::Reset()
{
}
////////////////////////////////////////////////
	//InvalidateCtrl() ;/////////////////////////////
int CADScopeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{ 
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_rectClient = CRect(0, 0, 1024, 1000);		// 768
	m_nClientHeight = m_rectClient.Height();
	m_nClientWidth  = m_rectClient.Width();
	
	m_rectPlot.left   = 20 ;
	m_rectPlot.top    = 10 ;
	m_rectPlot.right  = m_rectClient.right-10 ;
	m_rectPlot.bottom = m_rectClient.bottom-25 ;
	
	m_nPlotHeight = m_rectPlot.Height() ;
	m_nPlotWidth  = m_rectPlot.Width() ;
	
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange ; 

	return 0;
}

//########################################################################
// ����������������
void CADScopeCtrl::DrawBkGnd()  
{ 
	int nCharacters=0 ;
	CPen *oldPen ;
	CPen solidPen(PS_SOLID, 0, m_crGridColor) ;
	CFont axisFont, yUnitFont, *oldFont ;
	CString strTemp ;
	//---------------------------------------------------------------------------
	m_dcGrid.SetBkColor (m_crBackColor) ;	
	// fill the grid background
	m_dcGrid.FillRect(m_rectClient, &m_brushBack) ;	
	// ���������ִ���ռ�õ��ַ����
	nCharacters = abs((int)log10(fabs(m_dUpperLimit[0]))) ;
	nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit[0]))));
	
	nCharacters = nCharacters + 4 + m_nYDecimals ;  
	//	m_rectPlot.left = m_rectClient.left + 6*(nCharacters) ;  
	m_nPlotWidth    = m_rectPlot.Width() ;
	//----------------------------------------------------------------------------------
	// �����ܵĿ��	
	oldPen = m_dcGrid.SelectObject (&solidPen) ;  // �����ܵķ���
	m_dcGrid.MoveTo (m_rectPlot.left, m_rectPlot.top) ;
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.top) ;
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.bottom+1) ;
	m_dcGrid.LineTo (m_rectPlot.left, m_rectPlot.bottom+1) ;
	m_dcGrid.LineTo (m_rectPlot.left, m_rectPlot.top) ;
	m_dcGrid.SelectObject (oldPen) ;  
	COLORREF m_Grid = RGB(200, 200, 200);
	int k;
	for( k=50; k<m_rectPlot.Width(); k+= 50) // �ڸ�50�����ػ�һ����ֱ����
	{
		for(int j=5; j<m_rectPlot.Height(); j+=5)
		{
			m_dcGrid.SetPixelV(CPoint(m_rectPlot.left + k, m_rectPlot.top + j), m_Grid);
		}
	}
	
	for(k=30; k<m_rectPlot.Height(); k+= 30) // �ڸ�30�����ػ�һ��ˮƽ�������
	{
		for(int j=10; j<m_rectPlot.Width(); j+=10)
		{
			m_dcGrid.SetPixelV(CPoint(m_rectPlot.left + j, m_rectPlot.top + k), m_Grid);
		}
	}	
	//��ÿ��ͨ���ķֽ���(ˮƽ��)
	int nGridPix = 0; // Y����������ߵĿ̶�
	float hight = (float)(m_rectPlot.Height()*1.0 / m_nChannelCount); // ÿͨ����Y���

	int i = 0;
	if(m_bAllChannel) // ��ʾ����ͨ��
	{
		if(gl_bTileWave)
		{
			int j;
			for( j=1; j<m_nChannelCount; j++) // ��m_nChannelCount-1��������
			{
				nGridPix = m_rectPlot.top + (int)(m_rectPlot.Height() * j) / m_nChannelCount;
				for(i=m_rectPlot.left; i<m_rectPlot.right; i+=2)//ÿ��2�����ػ�1��
				{
					m_dcGrid.SetPixel(i, nGridPix, m_crGridColor); 
				}				
			}
			for(j=0; j<m_nChannelCount; j++) // ��ÿͨ��������
			{
				for(i=m_rectPlot.left; i<m_rectPlot.right; i+=3)//ÿ��3�����ػ�1��
				{
					m_dcGrid.SetPixel(i, (int)(m_rectPlot.top + (hight * j) + hight/2.0), RGB(255, 255, 255)); 
				}
			}
		}
		else
		{
			for(i=m_rectPlot.left; i<m_rectPlot.right; i+=2)//ÿ��4�����ػ�1��
				m_dcGrid.SetPixel(i, m_rectPlot.top+(int)(m_rectPlot.Height()/2), RGB(255, 255, 255));
		}
		
	}
	else // �����м仭һ��������
	{
		for(i=m_rectPlot.left+1; i<m_rectPlot.right; i+=2)//ÿ��4�����ػ�1��
			m_dcGrid.SetPixel(i, m_rectPlot.top+(int)(m_rectPlot.Height()/2), RGB(255, 255, 255));
		
	}
	//--------------------------------------------------------------------------------
	axisFont.CreateFont (14, 0, 0, 0, 300,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, 
		DEFAULT_PITCH|FF_SWISS, L"Arial") ;
	yUnitFont.CreateFont (14, 0, 900, 0, 300,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, 
		DEFAULT_PITCH|FF_SWISS, L"Arial") ;
	
	oldFont = m_dcGrid.SelectObject(&axisFont);
	m_dcGrid.SetTextAlign(TA_RIGHT|TA_TOP);		 // ���뷽ʽ
	
	int PerY = (int)(m_rectPlot.Height() / tempCount);
	int StartY = m_rectPlot.top;
	//----------------------------------------------------------------------
	CString strChannel;
	int nIndex = 0;

	if (m_bAllChannel)
	{
		if(gl_bTileWave)	// ƽ����ʾ
		{
			DrawAllChannelText(&m_dcGrid);
		}
		else	// ������ʾ
		{
			DrawSingleCHText(&m_dcGrid, 0xFFFF/*0x0000*/);
		}
	}
	else
	{
		DrawSingleCHText(&m_dcGrid, m_nChannelNum);
	}

	// x min
	m_dcGrid.SetTextAlign (TA_LEFT|TA_TOP) ;
	m_dcGrid.SetTextColor(m_crGridColor);

	// x max
	m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP) ;
	strTemp.Format(L"%d", m_nPlotWidth/m_nShiftPixels) ; 

	// x units
	m_dcGrid.SetTextAlign (TA_CENTER|TA_TOP) ;

	// restore the font
	m_dcGrid.SelectObject(oldFont) ;
	
	// y units
	oldFont = m_dcGrid.SelectObject(&yUnitFont) ;
	m_dcGrid.SetTextAlign (TA_CENTER|TA_BASELINE) ;
	m_dcGrid.SelectObject(oldFont) ;
	//����������ɫ
	m_dcGrid.SetTextColor(RGB(255, 0, 0));
	
	InvalidateRect(m_rectClient) ; // ˢ������
	
}

//########################################################################
// ��PolyLine��

extern BOOL gl_LBtnDown;
BOOL gl_bOnly = FALSE;   // �����ƶ�������Ψһ��
void CADScopeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HCURSOR m_hCurSor;
	m_hCurSor = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	::SetCursor(m_hCurSor);
	int nOldPositionX, nOldPositionY;
	CString str;

	///////////////////////////////////////////////////////////////////////////////
	CClientDC dc(this);
	int nMode;
	dc.SetBkMode(TRANSPARENT);         // ͸������
	nMode = dc.SetROP2(R2_XORPEN);	   // ������ģʽ	
	if(m_rectPlot.PtInRect(point))
	{
		// ͨ������λ���ж�����һ���߻�ý���
		if(abs(point.x - m_OldMoveX1) < 5) 
		{
			m_hCurSor = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
			::SetCursor(m_hCurSor); 
			if(gl_LBtnDown && !gl_bOnly)
			{
				m_nLineIndex = 1;
				gl_bOnly = TRUE;
			}
			
		}
		if(abs(point.x - m_OldMoveX2) < 5) 
		{
			m_hCurSor = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
			::SetCursor(m_hCurSor);
			if(gl_LBtnDown && !gl_bOnly)
			{
				m_nLineIndex = 2;
				gl_bOnly = TRUE;
			}
			
		}	
		if (!m_bAllChannel && abs(point.y - m_OldVoltageY) < 5) 
		{
			m_hCurSor = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
			::SetCursor(m_hCurSor);
			if(gl_LBtnDown && !gl_bOnly)
			{
				m_nLineIndex = 3;
				gl_bOnly = TRUE;
			}
		}
		
		if(gl_LBtnDown) 
		{
			if(point.x < m_rectPlot.left)	nOldPositionX = m_rectPlot.left;
			if(point.x > m_rectPlot.right)	nOldPositionX = m_rectPlot.right;
			if(point.x >= m_rectPlot.left && point.x <= m_rectPlot.right)
			{
				nOldPositionX = point.x;
			} 
			
			if(point.y > m_rectPlot.bottom) nOldPositionY = m_rectPlot.bottom;
			if(point.y < m_rectPlot.top)	nOldPositionY = m_rectPlot.top;
			if(point.y >= m_rectPlot.top && point.y <= m_rectPlot.bottom)
			{
				nOldPositionY = point.y;
			}
			
			switch(m_nLineIndex)
			{
			case 1: // ѡ�������1
				dc.SelectObject(&m_PenLineA);
				dc.MoveTo(m_OldMoveX1 /*+ 100*/, m_rectPlot.top);//������һ�λ�����A
				dc.LineTo(m_OldMoveX1, m_rectPlot.Height()+10);	// ����A
				dc.SetTextColor(RGB(0, 0, 0));
				dc.TextOut(m_OldMoveX1, m_rectPlot.Height()+13, L"������");
				m_OldMoveX1 = point.x;
				dc.MoveTo(point.x, m_rectPlot.top);
				dc.LineTo(point.x, m_rectPlot.Height()+10);
				dc.SetTextColor(RGB(255, 255, 0));
				dc.TextOut(point.x, m_rectPlot.Height()+13, L"������");


				break;
			case 2: // ѡ�������2
				dc.SelectObject(&m_PenLineB);
				dc.MoveTo(m_OldMoveX2, m_rectPlot.top);//������һ�λ�����B
				dc.LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	// ����B
				m_OldMoveX2 = point.x;
				dc.MoveTo(point.x, m_rectPlot.top);
				dc.LineTo(point.x, m_rectPlot.Height()+10);
				break;
			case 3: // ѡ�������3
				dc.SelectObject(&m_PenLineV);
				dc.MoveTo(m_rectPlot.left,  m_OldVoltageY);//������һ�λ�����V
				dc.LineTo(m_rectPlot.right, m_OldVoltageY);	// ����V
				m_OldVoltageY = point.y;
				dc.MoveTo(m_rectPlot.left, point.y);
				dc.LineTo(m_rectPlot.right, point.y);
				break;
			}
		}
	}
	dc.SetROP2(nMode);
	MouseOffset = point.x;	
	SetStatusBar(); // ����״̬����Ƶ�ʵ�
	
	CWnd::OnMouseMove(nFlags, point);
}
void CADScopeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bDrawPoly == TRUE && point.x > m_rectPlot.left && gl_bTileWave) // �������ڱ߿���ʱ
	{
		// ͨ���ж����˫��ʱ��λ�ã���������ʾ��һ·��ͨ��		
		if(m_bAllChannel) // ��ʾ��ͨ��
		{
			CString str;
			tempCount = 1;
			for(int i=0; i<gl_nChannelCount; i++) //�ж���ʾ˫��λ��
			{		
				if(abs(point.y - HeightMid[i]) < m_rectPlot.Height()/(2*m_nChannelCount))
				{	
					m_nChannelNum = i; // Ҫ��ʾ�ĵ�ͨ����

					CString str;
					str.Format(L"%d", gl_ChannelArrary[i]);
					this->SetYUnits("��ͨ��-"+str);
					break;
				}
			}
		

			SetRange(-gl_ScreenVolume/2.0, gl_ScreenVolume/2.0, m_nChannelNum);
		}
		else // ��ʾ����ͨ��
		{	
			for(int j=0; j<MAX_CHANNEL_COUNT; j++)
			{
				switch(ADPara.InputRange[j])
				{	
				case USB2881_INPUT_N10000_P10000mV: // -1000mV - +1000mV
					m_dLowerLimit[j] = -10000;
					m_dUpperLimit[j] = 10000;
					break;
				case USB2881_INPUT_N5000_P5000mV:	// -5000mV - +5000mV
					m_dLowerLimit[j] = -5000;
					m_dUpperLimit[j] = 5000;
					break;
				}
			}
			tempCount = m_nChannelCount;				
			CString str;
			str.Format(L"�ܹ�-%d-ͨ�� ", m_nChannelCount); 
			this->SetYUnits(str);
		}
		m_bAllChannel = !m_bAllChannel; 
		this->InvalidateCtrl();
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}


DWORD gl_nOldTicks = 0;
BOOL gl_LBtnDown = FALSE;
void CADScopeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	gl_LBtnDown = TRUE;
	// TODO: Add your message handler code here and/or call default	
	// �������˫���¼�
	DWORD nNewTick;
	nNewTick = GetTickCount();
	if((nNewTick - gl_nOldTicks)<250) //Beep(3000, 1);
	{
		LPARAM CursorPos;
		CursorPos = MAKELONG(point.x, point.y);
		// �������˫����Ϣ, ���������ĵ�ǰ����
		::SendMessage(this->m_hWnd, WM_LBUTTONDBLCLK,  nFlags, CursorPos); 
	}
	gl_nOldTicks = nNewTick;
	this->SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

//#####################################################################################
// ��ԭ��ֵ����(buffer������ԭ��), �ٴ���ɿ�����ʾ�ĵ�����
BOOL gl_bInitBuf = FALSE;
void CADScopeCtrl::AppendPoly(int BufferID, int  Offset)
{
	m_nChannelCount = gl_nChannelCount;
	m_BufferID = BufferID; // ����ID
	m_Offset = Offset; // ƫ��
	if(!gl_bInitBuf)
	{	
		DrawBkGnd(); // �ڻ���֮ǰ���Ȼ�����������	
		gl_bInitBuf = TRUE; // ����������ֻ��һ��
	}
	// ����ָ��ָ�������ת���ɶ�Ӧ�������
	ProcessData(); 
	// ��ʼ����	
	DrawPoly(); 
}

//###############################################################################
// ��������, �ѻ����е�����ת���ɿ�����ʾ�ĵ�����
BOOL gl_bTileWave = TRUE;
void CADScopeCtrl::ProcessData()  
{	
	float m_ScreenVolume = VOLT_RANGE; // ��ͨ����ʾʱ����Ļ�������Ǻ�����ѹֵһ�µ�
	float LsbOfPixel;

//	m_iDataLength = MAX_RAM_SIZE-glOffset;

//	m_iDataLength = MAX_RAM_SIZE/gl_nChannelCount-glOffset;
	if (gl_nChannelCount==0)
		gl_nChannelCount =1;
	m_iDataLength = gl_ReadSizeWords/gl_nChannelCount-glOffset;
	if (gl_bSmlThenSize)
	{
		m_iDataLength = (gl_ReadSizeWords%8192)/gl_nChannelCount-glOffset;
	}

	if (m_iDataLength>m_nPlotWidth)
	{
		m_iDataLength = m_nPlotWidth;
	}
	

	PerY = (float)(m_rectPlot.Height()*1.0 / m_nChannelCount); // ÿͨ����Y���

	LsbOfPixel=(float)(((m_ScreenVolume/VOLT_RANGE)*LSB_COUNT)/(PerY));//ÿ���ض�Ӧ����ֵ    
	
	Center = (float)((PerY/2.0)+m_rectPlot.top);



	int Channel, Index, StartX;
	int nCount=0;
  //--------------------------------------------------------
	for(Channel=0; Channel<gl_nChannelCount; Channel++)
	{
		StartX = m_rectPlot.left; // X�������ʼλ��
		if(gl_bTileWave) // ƽ����ʾ
		{
			pointxy[Channel][0].x = StartX;	
			pointxy[Channel][0].y = (int)(Center) - (int)(((ADBuffer[Channel][glBufferID][glOffset]^0x8000)&MAX_LSB)/LsbOfPixel);
			
			for(Index=0; Index<m_iDataLength; Index++) 
			{
				pointxy[Channel][Index].x = StartX + Index;
				pointxy[Channel][Index].y = (int)(Center) - (int)(((((ADBuffer[Channel][glBufferID][Index+glOffset]^0x8000)&MAX_LSB) - LSB_HALF) / LsbOfPixel));
			}
			HeightMid[Channel] = (int)Center; // ����ͨ���м�λ������
			
			Center += PerY;
		}
		else //������ʾ
		{
			//LsbOfPixel = (float)(((gl_ScreenVolume/VOLT_RANGE)*LSB_COUNT)/(m_rectPlot.Height()));//ÿ���ض�Ӧ����ֵ    
			LsbOfPixel = (float)(((m_ScreenVolume/VOLT_RANGE)*LSB_COUNT)/(m_rectPlot.Height()));//ÿ���ض�Ӧ����ֵ    
			int StartX = m_rectPlot.left+1;
			Center = (float)(m_rectPlot.Height() / 2.0) + m_rectPlot.top;
			for(int Index=0; Index<m_iDataLength; Index++)	
			{
				pointxy[Channel][Index].x = StartX + Index;	
				pointxy[Channel][Index].y = (int)(Center) - (int)(((((ADBuffer[Channel][m_BufferID][Index]^0x8000)&MAX_LSB) - LSB_HALF) / LsbOfPixel));			
			}
		}
			
	
	}	
	m_bDrawPoly = TRUE; 
	
}

//##############################################################################
// ��������ĵ�
void CADScopeCtrl::DrawPoly()
{	
	m_dcPlot.SetBkColor (m_crBackColor) ;
	m_dcPlot.FillRect(m_rectClient, &m_brushBack) ;
	m_dcPlot.SetTextColor(RGB(255, 158, 0));
	CPen  *oldPen;	
	//---------------------------------------------------------------------------------
	if(m_bAllChannel) // ����ͨ����ʾ
	{
		for(int i = 0; i<gl_nChannelCount; i++) // ������ͨ���ĵ�
		{
			oldPen = m_dcPlot.SelectObject(&m_polyPen[gl_ChannelArrary[i]]);
			m_dcPlot.Polyline(pointxy[i], m_iDataLength);			
		}
		
	}
	else // ��ͨ����ʾ
	{	
		float LsbOfPixel;
		//--------------------------------------------------------------------------		
		//----------------------------------------------------------------------------		
		m_dcPlot.SelectObject(&m_polyPen[gl_ChannelArrary[m_nChannelNum]]);
		// LsbOfPixel��ֵ����ѡ��ĵ�ѹ����û�й�ϵ����ֻ���ѹ����������й�(gl_ScreenVolume <= VOLT_RANGE)
	    LsbOfPixel = (float)(((gl_ScreenVolume*LSB_COUNT/gl_InputRange[m_nChannelNum]))/(m_rectPlot.Height()));//ÿ���ض�Ӧ����ֵ    

		int StartX = m_rectPlot.left;
		Center = (float)(m_rectPlot.Height() / 2.0) + m_rectPlot.top;
		for(int Index=0; Index<m_iDataLength; Index++)	
		{
		//	ptOffset = m_pBuffer + m_nChannelNum*MAX_SEGMENT*MAX_OFFSET + m_BufferID*MAX_OFFSET + m_Offset; // ָ���ƫ����		
			pointTemp[Index].x = StartX + Index;
			pointTemp[Index].y = (int)(Center) - (int)(((((ADBuffer[m_nChannelNum][glBufferID][Index+glOffset]^0x8000)&MAX_LSB) - LSB_HALF - gl_MiddleLsb[m_nChannelNum]) / LsbOfPixel));
		}
		m_dcPlot.Polyline(pointTemp, m_iDataLength);
	}
	Invalidate();
}

void CADScopeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_nLineIndex = 0XFFFF;
	gl_bOnly = FALSE;
	gl_LBtnDown = FALSE; 
	::ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}
//#######################################################################


void CADScopeCtrl::SetStatusBar()
{
	float fTimeDiv;
	float fTimePixel;
	float fFrequency;
	CString strTimeDiv;
	CString strCountTimer;
	CString strFrequency;
	CString strVol;
	int SubstValue = abs(m_OldMoveX1 - m_OldMoveX2);
	CSysApp *pApp = (CSysApp *)AfxGetApp();
	CADFrame *pFrame = (CADFrame *)pApp->m_ADFrame;
	CEdit *pEditDiv = (CEdit *)pFrame->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv); 
	pEditDiv->GetWindowText(strTimeDiv);
	fTimeDiv = (float)_wtof(strTimeDiv);
	fTimePixel = fTimeDiv / 50; // ÿ��������֮���ʱ����(nS)
	// ��������֮���ʱ���

	strCountTimer.Format(L"%7.2f uS",fTimePixel * SubstValue);

	CEdit *pEditTime = (CEdit *)pFrame->m_wndShowStatus.GetDlgItem(IDC_EDIT_CountTime);
	pEditTime->SetWindowText(strCountTimer);
	// ����Ƶ�ʵĴ�С
   // fFrequency = (float)(1000000.0 / (fTimePixel * SubstValue));
    if (SubstValue==0)
    {
		SubstValue=1;

	}
	fFrequency =(float) (ADPara.Frequency*1.0 / SubstValue);

	if(fFrequency < 1000.0)
	{
		strFrequency.Format(L"%7.2f Hz", fFrequency);
	}
	if(fFrequency >= 1000.0 && fFrequency < 1000000.0)
	{
		strFrequency.Format(L"%7.2f KHz", fFrequency/1000.0);
	}
	if(fFrequency >= 1000000.0)
	{
		strFrequency.Format(L"%7.2f MHz", fFrequency/1000000.0);
	}
	
	CEdit *pEditFre = (CEdit *)pFrame->m_wndShowStatus.GetDlgItem(IDC_EDIT_MeasureFreq);
	pEditFre->SetWindowText(strFrequency);
	// ������ѹֵ��ѹֵ
 	float LsbOfPixel=(float)((gl_ScreenVolume*1.0)/(m_rectPlot.Height()));//ÿ���ض�Ӧ����ֵ    

	int Center = m_rectPlot.Height()/2 + m_rectPlot.top;
	int nOffset = Center - m_OldVoltageY;
	m_VolOffset = nOffset * LsbOfPixel;
	strVol.Format(L"%7.2f", m_VolOffset);
	CEdit *pEditVol = (CEdit *)pFrame->m_wndShowStatus.GetDlgItem(IDC_EDIT_Volt);
	pEditVol->SetWindowText(strVol);

	CEdit *pEditOffset = (CEdit *)pFrame->m_wndShowStatus.GetDlgItem(IDC_EDIT_Offset);
	CString strOffset;
	strOffset.Format(L"%d", MouseOffset - m_rectPlot.left + glOffset);
	if(MouseOffset >= m_rectPlot.left && MouseOffset <= m_rectPlot.right)
	{
		pEditOffset->SetWindowText(strOffset);
	}

}

int CADScopeCtrl::FindTrigger(int nChannel, int nValue) // ��ֹ���ζ���
{
	int Offset=0;
	for(int Index=100; Index<1024; Index++) // �����м�ĵ㣬��ֹ���ζ���
	{
		int Temp = ADBuffer[nChannel][m_BufferID][Index]&MAX_LSB;
		if(Temp<(ADBuffer[nChannel][m_BufferID][Index+1]&MAX_LSB))
		{
			if(Temp>nValue && Temp<nValue+20)
				Offset = Index;	
		}
	}
	if(!gl_bDeviceADRun)
		Offset = 0;
	return Offset;
}

 
LRESULT CADScopeCtrl::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_pfnSuper != NULL)
		return ::CallWindowProc(m_pfnSuper, m_hWnd, nMsg, wParam, lParam);

	WNDPROC pfnWndProc;
	if ((pfnWndProc = *GetSuperWndProcAddr()) == NULL)
		return ::DefWindowProc(m_hWnd, nMsg, wParam, lParam);
	else
		return ::CallWindowProc(pfnWndProc, m_hWnd, nMsg, wParam, lParam);
}

void CADScopeCtrl::DrawAllChannelText(CDC *pDC)
{
	CString str;
	float hight = (float)(m_rectPlot.Height()*1.0 / m_nChannelCount); // ÿͨ����Y���
	int nCount=0;

	for (int nCH=0; nCH<MAX_CHANNEL_COUNT; nCH++)
	{
		if (ADPara.bChannelArray[nCH])
		{

			pDC->SetTextColor(m_clPen[nCH]); // �������ֵ���ɫ
			str.Format(L"%.*lf V", m_nYDecimals, m_dUpperLimit[nCH]/1000.0); // ����ѹֵ
			pDC->TextOut (m_rectPlot.left-4, (int)(m_rectPlot.top+hight*nCount+8), str); 

			pDC->SetTextAlign (TA_RIGHT|TA_BASELINE);
			str.Format(L"%.*lf V", m_nYDecimals, m_dLowerLimit[nCH]/1000.0); // ����ѹֵ
			pDC->TextOut (m_rectPlot.left-4, (int)(m_rectPlot.top+hight*(nCount+1)-5), str);

			str.Format(L"CH %d", nCH);
			pDC->TextOut(m_rectPlot.left-4, (int)(m_rectPlot.top+hight*nCount+hight/2 + 5), str);      
			nCount++;
		}
	}


}

void CADScopeCtrl::DrawSingleCHText(CDC *pDC, int nChannelNum)
{
	CString str;
	if(nChannelNum != 0xFFFF)
	{
		pDC->SetTextColor(m_clPen[gl_ChannelArrary[nChannelNum]]);
		str.Format(L"%.*lf mV", 0, m_dUpperLimit[nChannelNum]); // ����ѹֵ
		pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top, str); 
		pDC->SetTextAlign (TA_RIGHT|TA_BASELINE) ;
		str.Format(L"%.*lf mV", 0, m_dLowerLimit[nChannelNum]); // ����ѹֵ
		pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight, str);
	}
	else
	{
		pDC->SetTextColor(m_crGridColor);
		str.Format(L"%.*lf mV", 0, m_dUpperLimit[0]); // ����ѹֵ
		pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top, str); 
		pDC->SetTextAlign (TA_RIGHT|TA_BASELINE) ;
		str.Format(L"%.*lf mV", 0, m_dLowerLimit[0]); // ����ѹֵ
		pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight, str);
	}
	
	if(nChannelNum == 0xFFFF) // ����ǵ�����ʾʱ
		str = "CH";
	else
		str.Format(L"CH %d", gl_ChannelArrary[nChannelNum]); 
	// дͨ����
	pDC->TextOut(m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight/2+5, str);      

}