#if !defined(AFX_HistDataDoc_H__D442B985_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
#define AFX_HistDataDoc_H__D442B985_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistDataDoc.h : header file
//
#include "Sys.h"
#include "ADHistFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc document

class CADHistDoc : public CDocument
{
protected:
	CADHistDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADHistDoc)

public:
   	HWND m_hWndDigit;        // ���ִ��ھ��
	HWND m_hWndWave;         // ���δ��ھ��

// Attributes
public:
	int m_nDigitWidth ;
	CFile m_File;
	CString m_FileName;
	DWORD m_FileLength;
	FILE_HEADER m_Header;  // �����ļ�ͷ��Ϣ
	int m_HeaderSize;
    int  m_ChannelCount;
    LONG m_Offset;        // �ļ�ƫ��(��)
	WORD m_ADBuffer[65536];
	int m_Rate;            // �ط��ٶ�
    ULONG m_ScreenOffset;  // ��Ļλ��
	HANDLE m_hFileObject;
	int m_nMouseMoveX;
public:
	void ReadData(void);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADHistDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	UINT m_ReadDataSize;
	int m_nDigitLine;
	virtual ~CADHistDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CADHistDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HistDataDoc_H__D442B985_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
