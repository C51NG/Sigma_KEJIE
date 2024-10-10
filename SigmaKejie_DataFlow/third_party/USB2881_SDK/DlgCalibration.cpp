// DlgCalibration.cpp : implementation file
//

#include "stdafx.h"
#include "sys.h"
#include "DlgCalibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibration dialog



CDlgCalibration::CDlgCalibration(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalibration::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCalibration)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCalibration)
	DDX_Control(pDX, IDC_PROGRESS_Calibration, m_ProgressCalibration);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCalibration, CDialog)
	//{{AFX_MSG_MAP(CDlgCalibration)
	ON_BN_CLICKED(IDC_BUTTON_Calibration, OnBUTTONCalibration)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibration message handlers
extern CSysApp theApp;
BOOL gl__bCalibration = FALSE;

BOOL CDlgCalibration::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ProgressCalibration.SetRange(0, 100);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCalibration::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	m_lProgress++;
	if (m_lProgress>100)
		m_lProgress = 0;
	m_ProgressCalibration.SetPos(m_lProgress);

	if (!gl__bCalibration)
		StopCalibration();
		
		
	CDialog::OnTimer(nIDEvent);
}

void CDlgCalibration::OnBUTTONCalibration() 
{
	// TODO: Add your control notification handler code here
	if (theApp.m_hDevice != INVALID_HANDLE_VALUE)
	{
		int iRet = AfxMessageBox(L"Confirm there is no external links",MB_OKCANCEL);
		if (iRet == 2)
		{
			return;
		}
		CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_Calibration);
		m_lProgress =0;		
		gl__bCalibration = TRUE;
		SetTimer(1, 100, NULL);
		pStatic->SetWindowText(L"Calibration over a long time, please be patient");
		GetDlgItem(IDC_BUTTON_Calibration)->EnableWindow(FALSE);		
		
		AfxBeginThread(ReadDataThread, &m_hWnd, THREAD_PRIORITY_HIGHEST);		
	}
	else
	{
		AfxMessageBox(L"The device does not exist",MB_OK, MB_ICONWARNING);
	}
}

UINT CDlgCalibration::ReadDataThread(PVOID hWnd)
{	

	USB2881_ADCalibration(theApp.m_hDevice);
	
	gl__bCalibration = FALSE;

	TRACE("END ADCalibrationThread");
	return 1;
}

void CDlgCalibration::StopCalibration()
{
	GetDlgItem(IDC_BUTTON_Calibration)->EnableWindow(TRUE);
	KillTimer(1);
	m_ProgressCalibration.SetPos(100);
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_Calibration);
	pStatic->SetWindowText(L"Complete the calibration");
	AfxMessageBox(L"Complete the calibration");
	
}
