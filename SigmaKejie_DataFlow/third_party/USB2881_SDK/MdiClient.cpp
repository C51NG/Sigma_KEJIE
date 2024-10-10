// MdiClient.cpp:implementation file
//
/////////////////////////////////////////////////////////////////////////////
// This class does subclass the MDI-CLIENT window.
// Subclassing means that all messages are first routed to this class, then 
// to the original window (in this case the MDI-CLIENT).
// We need this to get notifications of the creation and deletion of the 
// MDI child frames (contain views).
/////////////////////////////////////////////////////////////////////////////
//
// Copyright ?1998 Written by Dieter Fauth 
//		mailto:fauthd@zvw.de 
//  
// This code may be used in compiled form in any way you desire. This    
// file may be redistributed unmodified by any means PROVIDING it is     
// not sold for profit without the authors written consent, and     
// providing that this notice and the authors name and all copyright     
// notices remains intact. If the source code in this file is used in     
// any  commercial application then a statement along the lines of     
// "Portions Copyright ?1999 Dieter Fauth" must be included in     
// the startup banner, "About" box or printed documentation. An email     
// letting me know that you are using it would be nice as well. That's     
// not much to ask considering the amount of work that went into this.    
//    
// This file is provided "as is" with no expressed or implied warranty.    
// The author accepts no liability for any damage/loss of business that    
// this product may cause.    
//  
// ==========================================================================  
// HISTORY:	  
// ==========================================================================  
//			1.00	08 May 1999	- Initial release.  
// ==========================================================================  
//  
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabCtrlBarDoc.h"
#include "MdiClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiClient

CMdiClient::CMdiClient(): m_sizeClient(0, 0)
{
	m_crBkColor = GetSysColor(COLOR_DESKTOP);
	m_pWndTabs = NULL;
}


CMdiClient::~CMdiClient()
{
}


BEGIN_MESSAGE_MAP(CMdiClient, CWnd)
	//{{AFX_MSG_MAP(CMdiClient)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_MESSAGE(WM_MDICREATE,OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY,OnMDIDestroy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CMdiClient::AddHandle(HWND hWnd)
{
	ASSERT(m_pWndTabs != NULL);
	// ����б�
	m_pWndTabs->AddHandle(hWnd);
}

void CMdiClient::RemoveHandle(HWND hWnd)
{
	ASSERT(m_pWndTabs != NULL);
	m_pWndTabs->RemoveHandle(hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CMdiClient message handlers

LRESULT CMdiClient::OnMDICreate(WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = (HWND) DefWindowProc(WM_MDICREATE,  wParam, lParam);
	AddHandle(hWnd);
	return (LRESULT) hWnd;
}

LRESULT CMdiClient::OnMDIDestroy(WPARAM wParam, LPARAM lParam)
{
	RemoveHandle((HWND) wParam);
	return DefWindowProc(WM_MDIDESTROY,  wParam, lParam);
}



BOOL CMdiClient::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	// ���ı䴰�ڴ�С���ƶ����״δ���ʱ���˺��������ã����������ڵı���
	CRect rect;
	// ȡ�������ڿͻ�������
	GetClientRect(&rect);	
	
	// �������ǰɫ��Ϊ0, m_crBkColor�ڹ��캯���г�ʼ��
	if (m_crBkColor != 0)
	{

		CBrush NewBrush(m_crBkColor); 
		// ����ˢ�ӵ���ʼԭ��
		pDC->SetBrushOrg(0, 0);
		// ѡ����ˢ�ӣ��ҽ�ԭˢ��ת����CBrush��
		CBrush* pOldBrush = static_cast<CBrush*>(pDC->SelectObject(&NewBrush));  
		// �Կ�����ʽ���ƴ��ڱ���
		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY); 		
		pDC->SelectObject(pOldBrush);
		// ɾ����ˢ�Ӷ���
		NewBrush.DeleteObject();
	}

    CRect rcDataBox;
	CString strLogo = "����̩�Ƽ�";//szLogoString;
	CFont fontLogo;
	TEXTMETRIC tm;
	
	// ����һ��32λ������ڶ���32λ������ˣ��ٳ��Ե�����32λ�������ȡ��
	int fontSize = -MulDiv(18, pDC->GetDeviceCaps(LOGPIXELSY), 72);
	
	// ��������
	fontLogo.CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_ROMAN, _T("Times New Roman"));
	// ���ñ���ģʽΪ��͸����ʽ��OPAQUE��
	pDC->SetBkMode(OPAQUE);		
	
	// ѡ��������
	CFont* oldFont = pDC->SelectObject(&fontLogo);
	CRect st(0, 0, 0, 0);
	
	// ȡ�ø����ַ���strLogo�ľ�������
	CSize sz = pDC->GetTextExtent(strLogo, strLogo.GetLength());
	// ���ݵ�ǰ���壬�����ѡ����Ķ���
	pDC->GetTextMetrics(&tm);

	// Calculate the box size by subtracting the text width and height from the
	// window size.  Also subtract 20% of the average character size to keep the
	// logo from printing into the borders...

	// ȡ�ÿͻ���
	GetClientRect(&rcDataBox);
	
	rcDataBox.left = rcDataBox.right  - sz.cx - tm.tmAveCharWidth ;
	rcDataBox.top  = rcDataBox.bottom - sz.cy - st.bottom - tm.tmHeight ;
	
	CRect rcSave = rcDataBox;		
	// ���ñ���ģʽΪ͸����ʽ��TRANSPARENT��
	pDC->SetBkMode(TRANSPARENT); 
	rcSave = rcDataBox;
	
	// shift logo box right, and print black...
	rcDataBox.left   += tm.tmAveCharWidth / 5;
	// ��������ɫ����ɫ��
	COLORREF oldColor = pDC->SetTextColor(RGB(0, 0, 0));
	// ���������ֵĵײ�����
	pDC->DrawText(strLogo, strLogo.GetLength(), &rcDataBox, 
		DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	
	rcDataBox = rcSave;
	
	//����ƫ������λ��Ϊ��ǰ�����ȵ�25%
	rcDataBox.left -= tm.tmAveCharWidth /4;
	// ���������ֵ��в�����ɫΪ��ɫ
	pDC->SetTextColor(RGB(255, 255, 255));
	// �����в���
	pDC->DrawText(strLogo, strLogo.GetLength(), &rcDataBox, 
		DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	
	
	rcDataBox = rcSave;
	// ��������Ϊϵͳɫ
	pDC->SetTextColor(GetSysColor(COLOR_BTNFACE));
	// ��ԭλ�û����ϲ�����
	pDC->DrawText(strLogo, strLogo.GetLength(), &rcDataBox, 
		DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	
	// restore the original properties and release resources...
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);   
	pDC->SetBkMode(OPAQUE);	
	fontLogo.DeleteObject();
    // �ͷŹ���DC�ʹ���DC��ʹ����Ӧ�ó������ʹ�ã�����DC��˽��DC��Ӱ��	
	ReleaseDC(pDC);
	
    return TRUE;	
	//return CMDIFrameWnd::OnEraseBkgnd(pDC);
}
void CMdiClient::OnSize(UINT nType, int cx, int cy) 
{
	// ���ͻ�����仯ʱ�������á�������֡���ڱ����ʱ��Ҳ���������Ϣ
	CWnd::OnSize(nType, cx, cy);
	
	// ���Ӧ�ó����������򱣴������С���������ɷ���
    if ((m_sizeClient.cx == 0) && (m_sizeClient.cy == 0))
	{
        m_sizeClient.cx = cx;
        m_sizeClient.cy = cy;
		
        return ;
	}	
    // ����ͻ����ڴ�Сδ�����仯���򷵻�
    if ((m_sizeClient.cx == cx) && ( m_sizeClient.cy == cy))
    { 
        return;
    }	
	// ������ֵ
    m_sizeClient.cx = cx;
    m_sizeClient.cy = cy;
	// ǿ���ػ�
    RedrawWindow(NULL, NULL,
        RDW_INVALIDATE | RDW_ERASE | RDW_ERASENOW | RDW_ALLCHILDREN);    
	
    return;                
}