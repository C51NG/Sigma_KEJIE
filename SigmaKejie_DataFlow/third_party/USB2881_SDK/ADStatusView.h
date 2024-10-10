#if !defined(AFX_ADStatusView_H__E4B4B7A2_D13C_11D6_B1D2_AB386BFF0020__INCLUDED_)
#define AFX_ADStatusView_H__E4B4B7A2_D13C_11D6_B1D2_AB386BFF0020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ADStatusView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CADStatusView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CADStatusView : public CFormView
{
protected:
	CADStatusView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADStatusView)

// Form Data
public:
	//{{AFX_DATA(CADStatusView)
	enum { IDD = CG_IDD_Capability };
	CComboBox	m_nScreenVolume;
	CComboBox	m_nMoveVolt;
	CEdit	m_nAnalyzeCount;
	CEdit	m_nOverRange;
	CEdit	m_nOverRatio;
	CEdit	m_nTringgerVolt; 
	//}}AFX_DATA

// Attributes
public:
	void SetDlgCheck();

	BOOL bFirstChangeTrigge;  // 首次改变触发电平
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADStatusView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CADStatusView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CADStatusView)
	afx_msg void OnChangeEDITTriggerVolt();
	afx_msg void OnGraphicShow();
	afx_msg void OnDigitalShow();
	afx_msg void OnDataSave();
	afx_msg void OnSelchangeComboScreenVolume();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADStatusView_H__E4B4B7A2_D13C_11D6_B1D2_AB386BFF0020__INCLUDED_)
