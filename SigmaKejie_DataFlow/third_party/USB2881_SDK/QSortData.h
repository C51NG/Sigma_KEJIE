// QSortData.h: interface for the CQSortData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QSORTDATA_H__05802C51_F215_4B0C_A99D_F1D37D869F96__INCLUDED_)
#define AFX_QSORTDATA_H__05802C51_F215_4B0C_A99D_F1D37D869F96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAX_BUFFER 8192 //�����������

typedef struct _SORTCOUNT{ 
	int Value;
	int CountNum;
}SORT_COUNT, *pSORT_COUNT; 

class CQSortData  
{

public:
	CQSortData();
public:
	void SetSortDir(int nDir = 0);
	int GetSortDir();
	void Sort(SHORT *data,int left,int right);	
	int GetDataTypes();
	int GetMidDataCount();
	int GetMidData();
	int GetMinData();
	int GetMaxData();
	virtual ~CQSortData();

private:
	BOOL m_bNewData;
	int m_DataMax; // �����
	int m_DataMin; // ��С��
	int m_DataTypes; // ��������	
	int m_SortDir; // ������(�������)
	int nBufferSize; // �趯̬������ڴ��С
	SORT_COUNT m_DataMiddle; // �м���
	pSORT_COUNT  Result; // _SORTCOUNT�ṹ��ָ��, ָ������������
	SHORT * Buffer;
private:
 	void SortAsc(SHORT *data, int left, int right);	// ��������
	void SortDsc(SHORT *data, int left, int right); // ��������
	void ProcessData(void);
};

#endif // !defined(AFX_QSORTDATA_H__05802C51_F215_4B0C_A99D_F1D37D869F96__INCLUDED_)
