// Sys.h : main header file for the SYS application
//
#include "ADDoc.h"
#include "ADHistDoc.h"
#include "ADFrm.h"

//#include "ADDigitListFrame.h"




#if !defined(AFX_SYS_H__D485DEFD_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
#define AFX_SYS_H__D485DEFD_85AB_11D1_B878_A68EB300DD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSysApp:
// See Sys.cpp for the implementation of this class
//
class CDIOFrm;
class CSysApp : public CWinApp
{
public:
	CSysApp();

public:
	// 以下三个变量用于控制子窗体的唯一性
	BOOL m_bCreateADFrm;  
	BOOL m_bCreateDAFrm;

	HANDLE m_hDevice;  // InitInstance中被创建, 在ExitInstance被释放
	BOOL m_bCreateDevice;
	HWND m_hParaCfgView;

public:
	CADFrame* m_ADFrame;  // 保存数据采集子帧窗口
	CMultiDocTemplate* pADTemplate;
	CMultiDocTemplate* pHistDataTemplate;
	CMultiDocTemplate* pDIOTemplate;
	//CMultiDocTemplate* m_pDIOTemplate;

	CADDoc* m_pADDigitListDoc;
	CADDoc* m_pADDoc;
	CDIOFrm* m_pDIOFrm;

	HANDLE m_hMutex;
public:
	int m_CurrentDeviceID;  // 记录当前设备ID号
	BOOL m_bDeviceCount;	// 判断当前系统是否有多USB2881设备
public:
   
    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSysApp)
	afx_msg void OnAppAbout();
	afx_msg void OnUpdateCloseFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDataFileNew(CCmdUI* pCmdUI);
	afx_msg void OnCloseFile();
	afx_msg void OnCollectData();
	afx_msg void OnDataFileNew();
	afx_msg void OnReleaseDevice();
	afx_msg void OnStartDeviceAD();
	afx_msg void OnStopDeviceAD();
	afx_msg void OnUpdateStopDeviceAD(CCmdUI* pCmdUI);
	afx_msg void OnOpenAD();
	afx_msg void OnUpdateOpenAD(CCmdUI* pCmdUI);
	afx_msg void OnOpenDIO();
	afx_msg void OnHistoryData();
	afx_msg void OnUpdateHistoryData(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOpenDA(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStartDeviceAD(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDataSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDigitalShow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraphicShow(CCmdUI* pCmdUI);
	afx_msg void OnMDataSave();
	afx_msg void OnMDigitalShow();
	afx_msg void OnMGraphicShow();
	afx_msg void OnListDeviceDlg();
	afx_msg void OnADCalibration();
	afx_msg void OnUpdateADCalibration(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
private:
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYS_H__D485DEFD_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
