#if !defined(AFX_HistWaveView_H__D442B984_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
#define AFX_HistWaveView_H__D442B984_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistWaveView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView view

class CADHistWaveView : public CScrollView
{
protected:
	CADHistWaveView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADHistWaveView)

// Attributes
public:
	void ShowWave(CDC *pDC); //绘制波形
	int m_center;
	int m_Index;
	int  m_ScreenVolume;//设置屏幕显示的量程
	int  m_TriggerVolt;//触发电压
	int m_MiddleLsb;
	CRect m_rcClient;
		 
	
	// Operations
public:
	BOOL m_Draw;
	CPoint m_oldPoint;
	BOOL m_bCapture;
	int m_nWidth;
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADHistWaveView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CADHistWaveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CADHistWaveView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);		 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HistWaveView_H__D442B984_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
