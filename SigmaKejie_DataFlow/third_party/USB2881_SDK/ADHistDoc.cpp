// HistDataDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ADHistDoc.h"
#include "ADHistDigitView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc

IMPLEMENT_DYNCREATE(CADHistDoc, CDocument)

CADHistDoc::CADHistDoc()
{
	m_Offset=0;
	m_Rate=1;	
	m_nMouseMoveX = 0;
}

BOOL CADHistDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CADHistDoc::~CADHistDoc()
{
}


BEGIN_MESSAGE_MAP(CADHistDoc, CDocument)
	//{{AFX_MSG_MAP(CADHistDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc diagnostics
#ifdef _DEBUG
void CADHistDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADHistDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc serialization

void CADHistDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc commands

BOOL CADHistDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	m_ChannelCount=1;
	m_HeaderSize = sizeof(m_Header);
	//m_hFileObject = USB2881_CreateFileObject(NULL, lpszPathName, USB2881_modeRead);
	m_hFileObject = CreateFile(lpszPathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,	FILE_ATTRIBUTE_NORMAL, NULL);

	if(m_hFileObject==INVALID_HANDLE_VALUE) return FALSE;

	//USB2881_ReadFile(m_hFileObject, (WORD*)&m_Header, 0, m_HeaderSize);
	DWORD dRet;
	ReadFile(m_hFileObject, (WORD*)&m_Header, m_HeaderSize, &dRet, NULL);
	//m_FileLength = (ULONG)((USB2881_GetFileLength(m_hFileObject) - m_HeaderSize)/2);
	m_FileLength = (ULONG)((GetFileSize(m_hFileObject, NULL) - m_HeaderSize)/2);
	if(m_FileLength>8192)
		m_ReadDataSize = 8192; // 如果数据文件长度大于8192点，则以8192点为段读取
	else
		m_ReadDataSize = m_FileLength; // 如果数据文件长度小于8192点，则以文件实际大小读取

	//USB2881_ReadFile(m_hFileObject, m_ADBuffer, m_HeaderSize, m_ReadDataSize*2);

	SetFilePointer(m_hFileObject, m_HeaderSize, 0, FILE_BEGIN);
	ReadFile(m_hFileObject, m_ADBuffer, m_ReadDataSize*2, &dRet, NULL);

	return TRUE;
}


void CADHistDoc:: ReadData(void)
{
	//m_File.Seek(sizeof(m_Header)+(m_Offset*2), CFile::begin);
	//m_File.Read(m_ADBuffer, m_ReadDataSize*2);

	//USB2881_ReadFile(m_hFileObject, m_ADBuffer, m_HeaderSize+(m_Offset*2), m_ReadDataSize*2);

	DWORD dRet;
	SetFilePointer(m_hFileObject, m_HeaderSize+(m_Offset*2), 0, FILE_BEGIN);
	ReadFile(m_hFileObject, m_ADBuffer, m_ReadDataSize*2, &dRet, NULL);
}

void CADHistDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	//USB2881_ReleaseFile(m_hFileObject);
	CloseHandle(m_hFileObject);
	CDocument::OnCloseDocument();
}
