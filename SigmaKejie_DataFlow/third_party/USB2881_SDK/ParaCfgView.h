#if !defined(AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_)
#define AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParaCfgView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParaCfgView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ADFrm.h"

class CParaCfgView : public CFormView
{
protected:
	CParaCfgView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CParaCfgView)

public:
	CADFrame* m_pADFrm;
// Form Data
public:
	//{{AFX_DATA(CParaCfgView)
	enum { IDD = CG_IDD_ADCHANNEL };
	CComboBox	m_Combo_ChannelCount;
	CEdit	m_Edit_TrigInterva;
	CButton	m_Check_bClockOutput;
	CEdit	m_Edit_TrigLevelVolt;
	CComboBox	m_Combo_ClockSource;
	//CComboBox	m_Combo_VoltRange0;
	CComboBox	m_ComBo_TriggerMode;
	CComboBox	m_Combo_SampleLenth;
	CComboBox	m_Combo_Frequency;	
	CComboBox	m_Combo_TriggerDir;
	CComboBox	m_Combo_TriggerSource;
	CStatic		m_Static_Offset;
	CSpinButtonCtrl	m_Spin_BufferID;
	CSliderCtrl	m_Slider_Offset;
	CEdit		m_Edit_BufferID;
	CButton		m_Check_Auto;
	LONG		m_TrigLevelVolt;
	CEdit	m_Edit_TrigWindow;
	//}}AFX_DATA

// Attributes
public:

	long Array[80];
	int CountItem;
	BOOL m_bInitial;
	SCROLLINFO si;
	long cyChar;
	int iVertPos;
	int m_nBits;
	int    m_nChannelDataBit;

// Operations
public:

	void StartAD();

	
	void EnableWindows(BOOL Flag=TRUE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaCfgView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CParaCfgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CParaCfgView)
	afx_msg void OnSelchangeCOMBOTriggerSource();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSelchangeCOMBOOutTriggerDir();
	afx_msg void OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEDITBufferID();
	afx_msg void OnCHECKAuto();
	afx_msg void OnSelchangeCOMBOClockSource();
	afx_msg void OnChangeEDITTrigLevelVolt();
	afx_msg void OnSelchangeCOMBOFrequence();
	afx_msg void OnEditchangeCOMBOFrequence();
	afx_msg void OnCHECKbClockOutput();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSelchangeCOMBOVoltRange();
	afx_msg void OnSelchangeCOMBOGains();
	afx_msg void OnCheckCh();
	afx_msg void OnSelchangeCOMBOTriggerMode();
	afx_msg void OnChangeEDITTrigWindow();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckbenough();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_)
