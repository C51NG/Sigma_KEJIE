// TabCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "TabCtrlDoc.h"
#include "PopupMenu.h"
#include "Resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlDoc

CTabCtrlDoc::CTabCtrlDoc()
{
	m_crSelColour     = RGB(0,0,255);
	m_crUnselColour   = RGB(50,50,50);
}

CTabCtrlDoc::~CTabCtrlDoc()
{
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();
}

BEGIN_MESSAGE_MAP(CTabCtrlDoc, CTabCtrl)
	//{{AFX_MSG_MAP(CTabCtrlDoc)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlDoc message handlers

int CTabCtrlDoc::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1) return -1;
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
	
	return 0;
}

void CTabCtrlDoc::PreSubclassWindow() 
{	
	CTabCtrl::PreSubclassWindow();
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

void CTabCtrlDoc::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	CRect rect = lpDrawItemStruct->rcItem;
	int nTabIndex = lpDrawItemStruct->itemID;
	if (nTabIndex < 0) return;
	BOOL bSelected = (nTabIndex == GetCurSel());

	WCHAR label[64];
	TC_ITEM tci;
	tci.mask = TCIF_TEXT|TCIF_IMAGE;
	tci.pszText = label;     
	tci.cchTextMax = 63;    	
	if (!GetItem(nTabIndex, &tci )) return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (!pDC) return;
	int nSavedDC = pDC->SaveDC();

	// For some bizarre reason the rcItem you get extends above the actual
	// drawing area. We have to workaround this "feature".
	rect.top += ::GetSystemMetrics(SM_CYEDGE);

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));

	// Draw image
	CImageList* pImageList = GetImageList();
	if (pImageList && tci.iImage >= 0) {

		rect.left += pDC->GetTextExtent(_T(" ")).cx;		// Margin

		// Get height of image so we 
		IMAGEINFO info;
		pImageList->GetImageInfo(tci.iImage, &info);
		CRect ImageRect(info.rcImage);
		int nYpos = rect.top;

		pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
		rect.left += ImageRect.Width();
	}

	if (bSelected) {
		pDC->SetTextColor(m_crSelColour);
		pDC->SelectObject(&m_SelFont);
		rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	} else {
		pDC->SetTextColor(m_crUnselColour);
		pDC->SelectObject(&m_UnselFont);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
	}

	pDC->RestoreDC(nSavedDC);
}

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlDoc operations

void CTabCtrlDoc::SetColours(COLORREF bSelColour, COLORREF bUnselColour)
{
	m_crSelColour = bSelColour;
	m_crUnselColour = bUnselColour;
	Invalidate();
}

void CTabCtrlDoc::SetFonts(CFont* pSelFont, CFont* pUnselFont)
{
	ASSERT(pSelFont && pUnselFont);

	LOGFONT lFont;
	int nSelHeight, nUnselHeight;

	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();

	pSelFont->GetLogFont(&lFont);
	m_SelFont.CreateFontIndirect(&lFont);
	nSelHeight = lFont.lfHeight;

	pUnselFont->GetLogFont(&lFont);
	m_UnselFont.CreateFontIndirect(&lFont);
	nUnselHeight = lFont.lfHeight;

	SetFont( (nSelHeight > nUnselHeight)? &m_SelFont : &m_UnselFont);
}


void CTabCtrlDoc::SetFonts(int nSelWeight,   BOOL bSelItalic,   BOOL bSelUnderline,
						  int nUnselWeight, BOOL bUnselItalic, BOOL bUnselUnderline)
{
	// Free any memory currently used by the fonts.
	m_SelFont.DeleteObject();
	m_UnselFont.DeleteObject();

	// Get the current font
	LOGFONT lFont;
	CFont *pFont = GetFont();
	if (pFont)
		pFont->GetLogFont(&lFont);
	else {
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
		lFont = ncm.lfMessageFont; 
	}

	// Create the "Selected" font
	lFont.lfWeight = nSelWeight;
	lFont.lfItalic = bSelItalic;
	lFont.lfUnderline = bSelUnderline;
	m_SelFont.CreateFontIndirect(&lFont);

	// Create the "Unselected" font
	lFont.lfWeight = nUnselWeight;
	lFont.lfItalic = bUnselItalic;
	lFont.lfUnderline = bUnselUnderline;
	m_UnselFont.CreateFontIndirect(&lFont);

	SetFont( (nSelWeight > nUnselWeight)? &m_SelFont : &m_UnselFont);
}

void CTabCtrlDoc::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		// �������CControlBar�ؼ��ϰ����Ҽ�ʱ���������˵�
	
	POINT  ptScreen = point;
	// ת��������굽��Ļ����
	ClientToScreen(&ptScreen);
	// �Զ����࣬��CMenu����
	CPopupMenu cMenu;
	// ��������ʽ�����˵�
	cMenu.CreatePopupMenu();
	// ��Ӳ˵��ID_VIEW_VIEWTAB�ǲ˵�����IDֵ
		
	// ��ӷָ���
	cMenu.AppendMenu(MF_STRING, ID_FILE_CLOSE, _T("�رձ�����"));
	this->GetCurSel();
	cMenu.LoadToolBarResource(IDR_TABVIEW_MENU);
	cMenu.RemapMenu(&cMenu);
	cMenu.EnableMenuItems(&cMenu, AfxGetMainWnd());
	cMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, ptScreen.x, 
		ptScreen.y, AfxGetMainWnd());	
	
	//*/
	//AfxMessageBox(L"����Ҽ�tab");
	//CMDIFrameWnd::OnRButtonDown(nFlags, point);
}

void CTabCtrlDoc::OnSetFocus(CWnd* pOldWnd) 
{
	CTabCtrl::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here
	
}
