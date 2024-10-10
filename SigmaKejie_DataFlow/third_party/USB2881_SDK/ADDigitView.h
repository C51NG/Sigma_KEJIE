// ADDigitView.h : interface of the CADDigitView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDIGITVIEW_H__D485DF07_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
#define AFX_ADDIGITVIEW_H__D485DF07_85AB_11D1_B878_A68EB300DD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CADDigitView : public CScrollView
{
protected: // create from serialization only
	CADDigitView();
	DECLARE_DYNCREATE(CADDigitView)

// Attributes 
public:
	CProgressCtrl* pProgress;
	int CurrentProgress;
// Operations
public:
	void ADDigit(CDC *pDC);
	CADDoc *GetDocument();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADDigitView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT ShowDigit(WPARAM wParam, LPARAM lParam);
	CDC memDC;
	CBitmap m_Bitmap;
	virtual ~CADDigitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CADDigitView)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnSixteenShow();
	afx_msg void OnUpdateSixteenShow(CCmdUI* pCmdUI);
	afx_msg void OnTenShow();
	afx_msg void OnUpdateTenShow(CCmdUI* pCmdUI);
	afx_msg void OnDigitalVoltShowMode();
	afx_msg void OnUpdateDigitalVoltShowMode(CCmdUI* pCmdUI);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
CADDoc *GetDocument();
#ifndef _DEBUG  // debug version in ADDigitView.cpp
inline CADDoc* CADDigitView::GetDocument()
   { return (CADDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDigitVIEW_H__D485DF07_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
