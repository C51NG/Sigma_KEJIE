// QSortData.cpp: implementation of the CQSortData class.
//////////////////////////////////////////////////////////////////////
//*******************************************************************
//
// QSortData类的使用说明：
// 能够对SHORT指针指向的一组数据先进行指定长度的排序,求出中间数(出现概率最大的数)
// 原型：CQSortData::Sort(SHORT *data, int left, int right)
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
	m_SortDir = 0;	 // 升序
	m_DataMiddle.Value = 0; 
	m_DataMiddle.CountNum = 0;
	m_DataTypes = 1;
	m_bNewData = FALSE; 
}

CQSortData::~CQSortData()
{
	
}

int CQSortData::GetMaxData() // 取最大数
{
	if(m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMax;
}

int CQSortData::GetMinData() // 取最小数
{
	if(m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMin;
}

int CQSortData::GetMidData() // 取中间数
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

int CQSortData::GetDataTypes() // 出现的数据种数
{	
	if(m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return  m_DataTypes;
}

int CQSortData::GetSortDir() // 取得排序方向(升序或降序)
{
	return m_SortDir;
}

void CQSortData::SetSortDir(int nDir) // 设置排序方向(缺省为 =0升序)
{
	m_SortDir = nDir;
}

//#################################################################
// 对data指针指向的缓冲区从下标为left到right的元素进行 升序排序
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
		if(i <= j) { //交换i, j 的值
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
// 对data指针指向的缓冲区从下标为left到right的元素进行  降序排序
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
		SortAsc(data, left, right); // 升序排序(省缺)
		m_DataMin = data[left];
		m_DataMax = data[right];
	}
	else
	{
		SortDsc(data, left, right); //降序排序
		m_DataMin = data[right];
		m_DataMax = data[left];
	}
	m_bNewData = TRUE;
	nBufferSize = right - left;
	Buffer = data;  // 把指针data的值传递给Buffer指针   
}

//#################################################################
// 从Result数组中查找中间数（出现频率最大的数） 
void CQSortData::ProcessData(void)
{
	Result = new SORT_COUNT[nBufferSize]; // 动态分配合适大小的内存
	Result[0].Value = Buffer[0];
	Result[0].CountNum = 0;
	m_DataMiddle.CountNum = 0;
	m_DataMiddle.Value = Result[0].Value;
	int k = 0;
	for (int i=0; i<nBufferSize; i++) 
	{ 
		if (Buffer[i] == Result[k].Value)
		{
			Result[k].CountNum++; // 统计第个数出现的次数, 保存在Result[]中
			
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
	//如果没有中间数
	if(m_DataMiddle.CountNum == 1)
	{
		AfxMessageBox(L"没有中间数");
		m_DataMiddle.CountNum = 0;
	}

	delete [] Result; // 释放动态分配的内存
	Result = NULL;
}


