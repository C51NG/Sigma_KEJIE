#if !defined(AFX_ADWAVEVIEW_H__D485DF11_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
#define AFX_ADWaveVIEW_H__D485DF11_85AB_11D1_B878_A68EB300DD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ADWaveView.h : header file
//
#include "ADScopeCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CADWaveView view

class CADWaveView : public CScrollView
{
protected:
	CADWaveView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADWaveView)
public:
	int m_ScaleH_Width;
	int m_ScaleV_Width;

// Attributes
public:
	//void ADWave(CDC *pDC);
	CADScopeCtrl m_ADScopeCtrl;
	CPen *OldPen;
//	CPen Pen;
	CBrush brush;
	long Offset;  // 记录在8192点中当前偏移位置

	CProgressCtrl* pProgress;
	int CurrentProgress;
	int CurrentX, CurrentY;  // 用于控制当前光标
	CPoint CurrentPoint;
	CADDoc *GetDocument();
// Operations
public:
	void OnDrawPolyLine();

	LRESULT DrawPolyLine(WPARAM wParam, LPARAM lParam);

	int * m_pBuffer;
	CPen m_Pen[16];
	BOOL m_bCapture;
	CPoint m_oldPoint;
	BOOL m_Draw;
	void AnalyzeAccurate();
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADWaveView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_ClassName;
	CSize sizeTotal;//设置波形显示视图的大小
	virtual ~CADWaveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CADWaveView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	friend  class CParaCfgView;
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ADDigitView.cpp
inline CADDoc* CADWaveView::GetDocument()
   { return (CADDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADWAVEVIEW_H__D485DF11_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
