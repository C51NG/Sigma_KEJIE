#include "stdAfx.h"
#include "Sys.h"
#include "ADThread.h"
#include "ADDigitView.h"
#include "math.h"

CWinThread* m_ReadThread;  // ���߳̾��
BOOL        gl_bTerminateThr = TRUE; // �߳��Ƿ�ɾ��
BOOL		gl_bSmlThenSize = FALSE;

BOOL gl_bEnough = TRUE;


//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------
UINT ReadDataThread(PVOID hWnd);
//########################################################################
// �������ݲɼ��߳�
BOOL MyStartDeviceAD(HANDLE hDevice)
{
	if(!gl_bTerminateThr)				// ��һ���̻߳�û��ɾ��
	{
		AfxMessageBox(L"��һ�ε��̻߳�û����ֹ");
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
		m_ReadThread->m_bAutoDelete = true; // �Զ�ɾ���߳�
		gl_bDeviceADRun = TRUE;				// ��֮�ɼ��߳̿��Թ�����
		m_ReadThread->ResumeThread();		// �����ɼ��߳�ReadDataThread_NotEmpty

        return TRUE;
		
}

WORD IsolateData[1024*2]; // �������ݵĻ���
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
// ��ȡ���� �̺߳���
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
	ULONG SampleCnt = 1;		// �ɼ�����
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
		if(gl_bAutoCollect)			// ���ֶ��ɼ��Ļ�����ʾ���˾�ֹͣ�ɼ���
		{	
			SampleCnt = 0xFFFFFFFF;
		}

		USB2881_StopDeviceAD(hDevice);
		USB2881_ReleaseDeviceAD(hDevice); // �ͷ�AD

		if(!USB2881_InitDeviceAD(hDevice, &ADPara)) // ��ʼ���豸
		{
			AfxMessageBox(L"�����ĳ�ʼ������.....", MB_ICONERROR);		
			goto ExitReadThread;
		}


		USB2881_StartDeviceAD(hDevice);  // ��ʼ���������豸���� 

		gl_bTerminateThr = FALSE ;
		gl_bSmlThenSize = FALSE;

		for(SampleIndex = 0; SampleIndex<SampleCnt; SampleIndex++)// ѭ���ɼ�AD����
		{
NextRead:
			if(!gl_bDeviceADRun)
				goto ExitReadThread;

			RetReadSizeWords = USB2881_ReadDeviceAD(hDevice, Buffer, gl_ReadSizeWords, &nRetSizeWords,gl_bEnough);
			if(RetReadSizeWords == -1)
			{
				AfxMessageBox(L"�����ݳ���!");
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
				AfxMessageBox(L"�������,��ʹ��VC�µ�SaveFile�������!");
			}
			gl_bSegMentFlag[lBufferID] = TRUE;
			gl_lSegSaveSize[lBufferID] = nRetSizeWords/gl_nChannelCount*sizeof(USHORT);

// 			if (nProcessMode == SAVE_MODE)
// 			{		
// 				for (nADChannel = 0; nADChannel<gl_nChannelCount; nADChannel++)
// 				{
// 					WriteFile(hFileObject[nADChannel], ADBuffer[nADChannel][lBufferID], nRetSizeWords/gl_nChannelCount*sizeof(USHORT), &dTmp, NULL);
// 
// 					FileLenghtWords += nRetSizeWords;	// �����ļ���С����
// 				}				
// 			}

			lBufferID++;
			if (lBufferID >= MAX_SEGMENT)
				lBufferID = 0;

			SetEvent(hEvent);					// �����¼������߻��ƴ����̣߳����������Ѳɼ����



	}
ExitReadThread:
		USB2881_StopDeviceAD(hDevice);
		//USB2881_StopDeviceAD(hDevice);
		USB2881_ReleaseDeviceAD(hDevice); // �ͷ�AD
	SetEvent(hExitEvent);	// �����豸�ɹ��˳���Ϣ���ùر��豸���̵߳ô���Ϣ
	gl_bTerminateThr = TRUE;
	gl_bCollected = TRUE;	// �����һ�βɼ�
	
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
 