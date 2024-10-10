// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Sys.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern USB2881_PARA_AD Para;
extern BOOL bCreateDevice;
extern BOOL gl_bDeviceADRun;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_ADDCHANNEL, ClickFunction)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR, 		// status line indicator
	ID_SHOWMOUSEVOLT,   //显示鼠标的指针的位置
	ID_SHOWMOUSEVOLT, 		// status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here	
}

CMainFrame::~CMainFrame()
{
}


void CMainFrame::ClickFunction(void)
{


}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(CreateTabs()==-1)
		return -1;


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers




void CMainFrame::SetMouseVolt(CString strText)//把鼠标的指针的位置显状态栏上
{
	int nIndex=m_wndStatusBar.CommandToIndex(ID_SHOWMOUSEVOLT);
	CWindowDC dc(&m_wndStatusBar);
	CSize sizeText=dc.GetTextExtent(strText);
	m_wndStatusBar.SetPaneInfo(nIndex,ID_SHOWMOUSEVOLT,SBPS_NORMAL,sizeText.cx);
	m_wndStatusBar.SetPaneText(2,strText);
	
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);	
}

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMDIFrameWnd::DestroyWindow();
}

void CMainFrame::OnDestroy() 
{
	CMDIFrameWnd::OnDestroy();	
	// TODO: Add your message handler code here	
}



void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
//	if(MessageBox(L"您真的要退出本系统吗？...", L"提醒您...", MB_YESNO|MB_ICONINFORMATION)==IDNO)
//		return;
	CMDIFrameWnd::OnClose();
}

BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CMDIFrameWnd::OnCreateClient(lpcs, pContext);
}
