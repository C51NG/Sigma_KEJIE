// DIOFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CDIOFrm::~CDIOFrm()
{    
}


// CDIOFrm

IMPLEMENT_DYNCREATE(CDIOFrm, CMDIChildWnd)

CDIOFrm::CDIOFrm()
{
}
BEGIN_MESSAGE_MAP(CDIOFrm, CMDIChildWnd)
//{{AFX_MSG_MAP(CDIOFrm)
ON_WM_DESTROY()
ON_WM_CREATE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIOFrm message handlers


void CDIOFrm::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	CSysApp* pApp = (CSysApp*)AfxGetApp();   
	pApp->m_pDIOFrm = NULL;
}

int CDIOFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CSysApp* pApp = (CSysApp*)AfxGetApp();   
	pApp->m_pDIOFrm = this;
	
	return 0;
}
