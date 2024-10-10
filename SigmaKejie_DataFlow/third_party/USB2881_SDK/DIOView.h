#if !defined(AFX_IOVIEW_H__B8A8D101_1478_11D5_B883_D6511767BB0F__INCLUDED_)
#define AFX_IOVIEW_H__B8A8D101_1478_11D5_B883_D6511767BB0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IOView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDIOView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDIOView : public CFormView
{
protected:
	CDIOView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDIOView)
public:
	int DeviceID;
	HANDLE hDevice;
	BOOL bCreateDevice;
	BOOL Status[16];
	BYTE  m_OldDI[8];
	BOOL Flag;
	void SetSwitchStatus(void);
	void SetSwitchOut(void);

// Form Data
public:
	//{{AFX_DATA(CDIOView)
	enum { IDD = CG_IDD_DIALOG_IO };
	CButton	m_Button_In15;
	CButton	m_Button_In14;
	CButton	m_Button_In13;
	CButton	m_Button_In12;
	CButton	m_Button_In11;
	CButton	m_Button_In10;
	CButton	m_Button_In9;
	CButton	m_Button_In8;
	CButton	m_Check_DO15;
	CButton	m_Check_DO14;
	CButton	m_Check_DO13;
	CButton	m_Check_DO12;
	CButton	m_Check_DO11;
	CButton	m_Check_DO10;
	CButton	m_Check_DO9;
	CButton	m_Check_DO8;
	CButton	m_Button_In7;
	CButton	m_Button_In6;
	CButton	m_Button_In5;
	CButton	m_Button_In4;
	CButton	m_Button_In3;
	CButton	m_Button_In2;
	CButton	m_Button_In1;
	CButton	m_Button_In0;
	CButton	m_Check_DO7;
	CButton	m_Check_DO6;
	CButton	m_Check_DO5;
	CButton	m_Check_DO4;
	CButton	m_Check_DO3;
	CButton	m_Check_DO2;
	CButton	m_Check_DO1;
	CButton	m_Check_DO0;
	BOOL	m_bDO0;
	BOOL	m_bDO1;
	BOOL	m_bDO2;
	BOOL	m_bDO3;
	BOOL	m_bDO4;
	BOOL	m_bDO5;
	BOOL	m_bDO6;
	BOOL	m_bDO7;
	BOOL	m_bDO8;
	BOOL	m_bDO9;
	BOOL	m_bDO10;
	BOOL	m_bDO11;
	BOOL	m_bDO12;
	BOOL	m_bDO13;
	BOOL	m_bDO14;
	BOOL	m_bDO15;
	//}}AFX_DATA

// Attributes
public:
	inline CButton* m_CHECK_AutoRead()
	{return (CButton*)GetDlgItem(IDC_CHECK_AutoRead);}
	inline CButton* m_BUTTON_Read()
	{return (CButton*)GetDlgItem(IDC_BUTTON_Read);}
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIOView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDIOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDIOView)
	afx_msg void OnCheckDo0();
	afx_msg void OnCheckDo1();
	afx_msg void OnCheckDo2();
	afx_msg void OnCheckDo3();
	afx_msg void OnCheckDo4();
	afx_msg void OnCheckDo5();
	afx_msg void OnCheckDo6();
	afx_msg void OnCheckDo7();
	afx_msg void OnCheckDo8();
	afx_msg void OnCheckDo9();
	afx_msg void OnCheckDo10();
	afx_msg void OnCheckDo11();
	afx_msg void OnCheckDo12();
	afx_msg void OnCheckDo13();
	afx_msg void OnCheckDo14();
	afx_msg void OnCheckDo15();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnCHECKAutoRead();
	afx_msg void OnBUTTONRead();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_IOVIEW_H__B8A8D101_1478_11D5_B883_D6511767BB0F__INCLUDED_)
