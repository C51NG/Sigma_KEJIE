#include "stdAfx.h"
#include "Sys.h"
#include "ADThread.h"
#include "ADDigitView.h"
#include "math.h"

CWinThread* m_ReadThread;  // 子线程句柄
BOOL        gl_bTerminateThr = TRUE; // 线程是否删除
BOOL		gl_bSmlThenSize = FALSE;

BOOL gl_bEnough = TRUE;


//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------
UINT ReadDataThread(PVOID hWnd);
//########################################################################
// 启动数据采集线程
BOOL MyStartDeviceAD(HANDLE hDevice)
{
	if(!gl_bTerminateThr)				// 上一次线程还没有删除
	{
		AfxMessageBox(L"上一次的线程还没有中止");
		return FALSE; 
	}
	
	
		for (int i=0; i<MAX_CHANNEL_COUNT; i++)
		{
			for (int j=0; j<2; j++)
			{
				for (int k=0; k<MAX_RAM_SIZE; k++)
				{
					ADBuffer[i][j][k]= 0;
				}
			}
		}
		

		m_ReadThread = AfxBeginThread(ReadDataThread,NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		m_ReadThread->m_bAutoDelete = true; // 自动删除线程
		gl_bDeviceADRun = TRUE;				// 告之采集线程可以工作了
		m_ReadThread->ResumeThread();		// 启动采集线程ReadDataThread_NotEmpty

        return TRUE;
		
}

WORD IsolateData[1024*2]; // 隔离数据的缓冲
BOOL bWriteCnt = 0;
//###############################################################
USB2881_PARA_AD gl_OldADPara = ADPara;
BOOL IsChangePara()
{
	BOOL bRet;
	if(memcmp(&gl_OldADPara, &ADPara, sizeof(ADPara))==0)
		bRet = FALSE;
	else
		bRet = TRUE;
	gl_OldADPara = ADPara;
	return bRet;
}


BOOL gl_bSegMentFlag[MAX_SEGMENT];
LONG gl_lSegSaveSize[MAX_SEGMENT];

ULONG gl_ChannelReadSize[] = 
{
	16384, 16384, 16128, 16384, 15360, 16128, 16128, 16384, 16128, 15360, 14080, 16128
};
// 读取数据 线程函数
UINT ReadDataThread(PVOID hWnd)  
{
	PWORD Buffer;
	Buffer = new WORD[MAX_RAM_SIZE];
	//WORD Buffer[MAX_RAM_SIZE];
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc; 
	ULONG ReadOffsetWords = 0;
	LONG RetReadSizeWords;
	int nADChannel = 0;

	HANDLE hDevice = pApp->m_hDevice;
	ULONG SampleCnt = 1;		// 采集次数
	ULONG SampleIndex;
	ULONG RemainSizeWords = 0;
	LONG nRetSizeWords;
	ULONG nIndex = 0;
	DWORD dTmp;
	
	LONG lBufferID = 0;
	BOOL bSuccess = FALSE;
	BOOL bErrorDlg = FALSE;

	memset(gl_bSegMentFlag, 0 ,sizeof(gl_bSegMentFlag));
	memset(gl_lSegSaveSize, 0 ,sizeof(gl_lSegSaveSize));

	glBufferID = 0;
	gl_ReadSizeWords = gl_ChannelReadSize[gl_nChannelCount-1];
	

		memset(ADBuffer, 0, sizeof(ADBuffer));
		if(gl_bAutoCollect)			// 是手动采集的话，显示完了就停止采集了
		{	
			SampleCnt = 0xFFFFFFFF;
		}

		USB2881_StopDeviceAD(hDevice);
		USB2881_ReleaseDeviceAD(hDevice); // 释放AD

		if(!USB2881_InitDeviceAD(hDevice, &ADPara)) // 初始化设备
		{
			AfxMessageBox(L"不明的初始化错误.....", MB_ICONERROR);		
			goto ExitReadThread;
		}


		USB2881_StartDeviceAD(hDevice);  // 初始化后，启动设备工作 

		gl_bTerminateThr = FALSE ;
		gl_bSmlThenSize = FALSE;

		for(SampleIndex = 0; SampleIndex<SampleCnt; SampleIndex++)// 循环采集AD数据
		{
NextRead:
			if(!gl_bDeviceADRun)
				goto ExitReadThread;

			RetReadSizeWords = USB2881_ReadDeviceAD(hDevice, Buffer, gl_ReadSizeWords, &nRetSizeWords,gl_bEnough);
			if(RetReadSizeWords == -1)
			{
				AfxMessageBox(L"读数据出错!");
				goto ExitReadThread;
			}	
			if (nRetSizeWords == 0)
			{
				//Sleep(1);
				//continue;
				goto NextRead;
			}
				
            bSuccess = TRUE;


			for (nIndex=0; nIndex<nRetSizeWords/gl_nChannelCount; nIndex++)
			{
				for (int nCH=0; nCH<gl_nChannelCount; nCH++)
				{					
					ADBuffer[nCH][lBufferID][nIndex] = Buffer[gl_nChannelCount*nIndex + nCH];					
				}
			}		
			glBufferID = lBufferID;		
			if (gl_bSegMentFlag[lBufferID] && bErrorDlg == FALSE && nProcessMode == SAVE_MODE)
			{
				bErrorDlg = TRUE;
				AfxMessageBox(L"存盘溢出,可使用VC下的SaveFile程序存盘!");
			}
			gl_bSegMentFlag[lBufferID] = TRUE;
			gl_lSegSaveSize[lBufferID] = nRetSizeWords/gl_nChannelCount*sizeof(USHORT);

// 			if (nProcessMode == SAVE_MODE)
// 			{		
// 				for (nADChannel = 0; nADChannel<gl_nChannelCount; nADChannel++)
// 				{
// 					WriteFile(hFileObject[nADChannel], ADBuffer[nADChannel][lBufferID], nRetSizeWords/gl_nChannelCount*sizeof(USHORT), &dTmp, NULL);
// 
// 					FileLenghtWords += nRetSizeWords;	// 存盘文件大小增加
// 				}				
// 			}

			lBufferID++;
			if (lBufferID >= MAX_SEGMENT)
				lBufferID = 0;

			SetEvent(hEvent);					// 发送事件，告诉绘制窗口线程，该批数据已采集完毕



	}
ExitReadThread:
		USB2881_StopDeviceAD(hDevice);
		//USB2881_StopDeviceAD(hDevice);
		USB2881_ReleaseDeviceAD(hDevice); // 释放AD
	SetEvent(hExitEvent);	// 发出设备成功退出消息，让关闭设备的线程得此消息
	gl_bTerminateThr = TRUE;
	gl_bCollected = TRUE;	// 完成了一次采集
	
	if (!gl_bAutoCollect && bSuccess)
	{
		pApp->m_pADDoc->StopDeviceAD();
	}	
	
 	delete Buffer;

	return TRUE;  
}

BOOL MyStopDeviceAD(HANDLE hDevice)
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	
	gl_bDeviceADRun = FALSE;
	
	WaitForSingleObject(m_ReadThread->m_hThread, INFINITE);
	bCreateDevice = FALSE;	
	
	return TRUE;
}
 