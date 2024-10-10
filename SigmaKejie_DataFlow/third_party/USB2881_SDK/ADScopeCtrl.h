// OScopeCtrl.h : header file
//

#ifndef __OScopeCtrl_H__
#define __OScopeCtrl_H__
/////////////////////////////////////////////////////////////////////////////
// CADScopeCtrl window

class CADScopeCtrl : public CWnd
{
	// Construction
public:
	CADScopeCtrl();
	
	// Attributes
public:
	double AppendPoint(double dNewPoint);
	void SetRange(double dLower, double dUpper, int nDecimalPlaces=1);
	void SetXUnits(CString string);
	void SetYUnits(CString string);
	void SetGridColor(COLORREF color);
	void SetPlotColor(COLORREF color);
	void SetBackgroundColor(COLORREF color);
	void InvalidateCtrl();
	
	void Reset();
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADScopeCtrl)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, HMENU nID=NULL);
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	void DrawAllChannelText(CDC *pDC);
	void DrawSingleCHText(CDC *pDC, int nChannelNum);
	CPen m_polyPen[12];
	CPen m_ChannelPen[12];
	int FindTrigger(int nChannel, int nValue);
	void SetStatusBar();
	void AppendPoly(int BufferID, int  Offset);
	int		m_nPtCount;
	int		tempCount;
	int		m_nChannelCount;
	int		m_nChannelNum;
	//int		PerY;

	int		m_BufferID;
	int		m_Offset;
	float	PixelOfLsb;
//	int		 m_nCenter[32];
	POINT	 pointxy[12][8192]; // ����ͨ����ʾ�ĵ�����
	CPoint	 pointTemp[4096];  // ������ŵ�ͨ����ʾ�ĵ�����
	SHORT	*m_pBuffer;
	COLORREF m_clPen[12]; 
	int		 HeightMid[64] ;
	int		 MouseOffset;		 // ����ڻ�ͼ����ƫ��λ��
	int	     m_OldMoveX1;		 // �������ƶ����ߵ�λ��
	int		 m_OldMoveX2;
	int		 m_OldMoveY;
	int		 m_CurMoveX1;
	int		 m_CurMoveX2;
	int		 m_CurMoveY;
    COLORREF m_crLineAColor;  // A�ߵ���ɫ
	COLORREF m_crLineBColor;  // B�ߵ���ɫ
	COLORREF m_crLineVColor;  // C�ߵ���ɫ 
	
	CPen     m_PenLineA;	  // A�ߵĻ���
	CPen     m_PenLineB;	  // B�ߵĻ���
	CPen     m_PenLineV;      // V�ߵĻ���
	
	int m_CurVoltageY;
	int m_OldVoltageY;
	float m_VolOffset;
	int m_nLineIndex;
	BOOL m_bAllChannel;
	
	int m_nShiftPixels;           
	int m_nYDecimals;
	
	CString m_strXUnitsString;
	CString m_strYUnitsString;
	
	COLORREF m_crBackColor;        
	COLORREF m_crGridColor;        
	COLORREF m_crPlotColor;        
	
	double m_dCurrentPosition;   
	double m_dPreviousPosition;  

	LONG  m_iDataLength;
	int		 m_nChannel;	
	
	virtual ~CADScopeCtrl();
private:
	UINT m_nLoop;
	void DrawBkGnd(); // ��������
	void DrawPoint();
	void DrawPoly();
	void ProcessData();

protected:
	//{{AFX_MSG(CADScopeCtrl)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
		int m_nHalfShiftPixels;
	int		m_nPlotShiftPixels;
	int		m_nClientHeight;
	int		m_nClientWidth;
	int		m_nPlotHeight;
	int		m_nPlotWidth;  // 
	BOOL	m_bDrawPoly;
	BOOL	m_bInitialed;
	double  m_dLowerLimit[MAX_CHANNEL_COUNT];        // lower bounds
	double  m_dUpperLimit[MAX_CHANNEL_COUNT];        // upper bounds
	double  m_dRange;
	double  m_dVerticalFactor;
	
	CRect   m_rectClient;
	CRect   m_rectPlot;
	CPen    m_penPlot;
	CBrush  m_brushBack;
	
	CDC     m_dcGrid;
	CDC     m_dcPlot;
	CDC	    m_dcMove;
	CBitmap *m_pbitmapOldGrid;
	CBitmap *m_pbitmapOldPlot;
	CBitmap m_bitmapGrid;
	CBitmap m_bitmapPlot;
	
	
};

/////////////////////////////////////////////////////////////////////////////
#endif
