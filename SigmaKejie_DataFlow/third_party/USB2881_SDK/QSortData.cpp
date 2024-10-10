// QSortData.cpp: implementation of the CQSortData class.
//////////////////////////////////////////////////////////////////////
//*******************************************************************
//
// QSortData���ʹ��˵����
// �ܹ���SHORTָ��ָ���һ�������Ƚ���ָ�����ȵ�����,����м���(���ָ���������)
// ԭ�ͣ�CQSortData::Sort(SHORT *data, int left, int right)
//
//*******************************************************************
#include "stdafx.h"
#include "QSortData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CQSortData::CQSortData()
{
	m_SortDir = 0;	 // ����
	m_DataMiddle.Value = 0; 
	m_DataMiddle.CountNum = 0;
	m_DataTypes = 1;
	m_bNewData = FALSE; 
}

CQSortData::~CQSortData()
{
	
}

int CQSortData::GetMaxData() // ȡ�����
{
	if(m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMax;
}

int CQSortData::GetMinData() // ȡ��С��
{
	if(m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMin;
}

int CQSortData::GetMidData() // ȡ�м���
{
	if(m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMiddle.Value;
}
int CQSortData::GetMidDataCount()
{
	if(m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMiddle.CountNum;
}

int CQSortData::GetDataTypes() // ���ֵ���������
{	
	if(m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return  m_DataTypes;
}

int CQSortData::GetSortDir() // ȡ��������(�������)
{
	return m_SortDir;
}

void CQSortData::SetSortDir(int nDir) // ����������(ȱʡΪ =0����)
{
	m_SortDir = nDir;
}

//#################################################################
// ��dataָ��ָ��Ļ��������±�Ϊleft��right��Ԫ�ؽ��� ��������
void CQSortData::SortAsc(SHORT *data, int left, int right) 
{
	register int i,j;  
	SHORT m,t;     
	i=left;j=right;
	m=data[(int)(left+right)/2];  
	
	do{
		while(data[i]<m && i<right)
			i++;
		while(m<data[j] && j>left)
			j--;
		if(i <= j) { //����i, j ��ֵ
			t = data[i];
			data[i] = data[j];
			data[j] = t;
			i++;j--;
		}
	}while(i<=j);
	if(left<j)  SortAsc(data,left,j);   
	if(i<right) SortAsc(data,i,right);
}

//#################################################################
// ��dataָ��ָ��Ļ��������±�Ϊleft��right��Ԫ�ؽ���  ��������
void CQSortData::SortDsc(SHORT *data, int left, int right) 
{
	register int i,j;  
	SHORT m,t;     
	i=left;j=right;
	m=data[(int)(left+right)/2];  
	
	do{
		while(data[i]>m && i<right)
			i++;
		while(m>data[j] && j>left)
			j--;
		if(i<=j) { 
			t=data[i];
			data[i]=data[j];
			data[j]=t;
			i++;j--;
		}
	}while(i<=j);
	if(left<j)  SortDsc(data,left,j);   
	if(i<right) SortDsc(data,i,right);
}

//############################################################
// 
void CQSortData::Sort(SHORT *data, int left, int right)
{
	
	if(!m_SortDir)
	{
		SortAsc(data, left, right); // ��������(ʡȱ)
		m_DataMin = data[left];
		m_DataMax = data[right];
	}
	else
	{
		SortDsc(data, left, right); //��������
		m_DataMin = data[right];
		m_DataMax = data[left];
	}
	m_bNewData = TRUE;
	nBufferSize = right - left;
	Buffer = data;  // ��ָ��data��ֵ���ݸ�Bufferָ��   
}

//#################################################################
// ��Result�����в����м���������Ƶ���������� 
void CQSortData::ProcessData(void)
{
	Result = new SORT_COUNT[nBufferSize]; // ��̬������ʴ�С���ڴ�
	Result[0].Value = Buffer[0];
	Result[0].CountNum = 0;
	m_DataMiddle.CountNum = 0;
	m_DataMiddle.Value = Result[0].Value;
	int k = 0;
	for (int i=0; i<nBufferSize; i++) 
	{ 
		if (Buffer[i] == Result[k].Value)
		{
			Result[k].CountNum++; // ͳ�Ƶڸ������ֵĴ���, ������Result[]��
			
		}
		else
		{ 
			if(Result[k].CountNum > m_DataMiddle.CountNum ) 
			{
				m_DataMiddle.CountNum = Result[k].CountNum ;	  
				m_DataMiddle.Value = Result[k].Value ;	 
			}
			
			k++;
			Result[k].Value = Buffer[i];
			Result[k].CountNum = 1;  
			m_DataTypes++;
		}		
	}
	
	if(Result[k].CountNum > m_DataMiddle.CountNum )
	{
		m_DataMiddle.CountNum = Result[k].CountNum ;	  
		m_DataMiddle.Value = Result[k].Value ;	  
	}
	//���û���м���
	if(m_DataMiddle.CountNum == 1)
	{
		AfxMessageBox(L"û���м���");
		m_DataMiddle.CountNum = 0;
	}

	delete [] Result; // �ͷŶ�̬������ڴ�
	Result = NULL;
}


