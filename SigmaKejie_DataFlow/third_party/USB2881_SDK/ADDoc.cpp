// ADDoc.cpp : implementation of the CADDoc class
//

#include "stdafx.h"
#include "Sys.h"
#include "ADDoc.h"
#include "ADDigitView.h"
#include "ADWaveView.h"
#include "ADStatusView.h"
#include "ParaCfgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#ifdef USB2881_CreateSystemEvent
#define USB2881_CreateSystemEvent USB2881_TCreateSystemEvent 
#endif
////////////////////////////////////////////////////////////////////////
// WORD ADBuffer[MAX_CHANNEL_COUNT][MAX_SEGMENT][MAX_RAM_SIZE]; // �������
BOOL gl_bAutoCollect = FALSE;
int CurrentIndex; // AD���ݴ����̵߳�ǰ������������
int ReadIndex;    // AD���ݲɼ��̵߳�ǰ������������
BOOL gl_bDeviceADRun=FALSE;
USB2881_PARA_AD ADPara;
USB2881_PARA_AD OldADPara;
USB2881_STATUS_AD gl_ADStatus;
//BOOL bCheckOverflow=FALSE; // �Ƿ���FIFO�����־
int  gl_nSampleMode = NPT_MODE;            // �ɼ�ģʽ(1,��ѯ�� 2, �жϣ� 3�� DMA)
UINT ReadDataThread(PVOID hWnd);//�����߳�
UINT ProccessDataThread(PVOID hWnd);  // ���������߳�

BOOL m_bFirstCreateDeviceFile;
int  m_ProcessGraphicMode;//Ϊ1,������ʾ
BOOL m_AnalyzeCheck;//���ƾ��ȷ���
int  gl_ScreenVolume;//������Ļ��ʾ������
int  m_TriggerVolt;//������ѹ
int gl_MiddleLsb[MAX_CHANNEL_COUNT];
float m_AnalyzeAllCount;
UINT m_OverLimitCount;
int m_ProcessMoveVolt;//Ϊ1ʱ,ƽ�Ƶ�ѹ
BOOL bCreateDevice;
BOOL m_bProgress;  // �Ƿ���½�����
int nProcessMode;  // ���ݴ���ʽ 1��������ʾ  2��������ʾ  3�����ݴ���
CEdit* pCountEdit;
CEdit* pRangeEdit;
CEdit* pRatioEdit;
CADDigitView* m_pDigitView;
CADWaveView* m_pWaveView;
CADStatusView* pADStatusView = NULL;
CParaCfgView* pParaCfgView = NULL;
HANDLE hEvent;  // �ɼ��߳�������̵߳�ͬ���ź�
int m_DigitalDataShowMode; // ���ִ�����ʾģʽ
int DeviceID;   // �ó���ʹ�õ��豸ID
HANDLE hFileObject[12];

extern BOOL gl_bSegMentFlag[MAX_SEGMENT];
extern LONG gl_lSegSaveSize[MAX_SEGMENT];

int gl_nChannelCount = MAX_CHANNEL_COUNT;

BOOL m_bCloseFile;
HANDLE hExitEvent;
float Center, PerY;
CRect ClientRect;
float middle1;
BOOL bDrawing=FALSE;

ULONGLONG FileLenghtWords;
int nChannelArrayClient[12];// ͨ����
int gl_CurrentbufNum = 0; // ��ǰ�Ļ���������
int DisplayChannel[12];// ���ڱ�־��ͨ���Ƿ���ʾ
float OutAnalogValve; // ��ʾģ���ⴥ����ȫ�ֱ���
ULONG gl_nSampleLenth = MAX_RAM_SIZE;   // ��������
int  gl_InputRange[12];    // ��ͨ�����õĵ�ѹ���̷�Χ
float gl_PerLsbVoltM[12]; // ��λLSB�ĵ�ѹֵ
CString g_strFileFullName[12];
ULONG gl_StartAddr;
//PWORD ADBuffer[MAX_CHANNEL_COUNT][MAX_SEGMENT]; // �������
USHORT ADBuffer[MAX_CHANNEL_COUNT][MAX_SEGMENT][MAX_RAM_SIZE]; // �������

//##############################################
//ʹ�ö�ý�嶨ʱ����ÿ��50֡�����µ��ٶ�ˢ����Ļ
#include "stdio.h"
#include "MmSystem.h"

void PASCAL DrawWindowProc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);
#define ONE_MILLI_SECOND 1
#define TWO_SECOND 2000
// �� �� ʱ �� �� �� �ʣ� �� �� �� Ϊ �� λ ---- 
#define TIMER_ACCURACY 1
UINT wAccuracy;  // �� �� �� �� ��
UINT TimerID; // �� �� �� ʱ �� �� �� 
ULONG gl_ChannelArrary[MAX_CHANNEL_COUNT];

////////////////////////////////////////////////////////////////////////////
// CADDoc

IMPLEMENT_DYNCREATE(CADDoc, CDocument)

BEGIN_MESSAGE_MAP(CADDoc, CDocument)
//{{AFX_MSG_MAP(CADDoc)
ON_COMMAND(IDM_TileWave, OnTileWave)
ON_COMMAND(IDM_SuperPoseWave, OnSuperPoseWave)
ON_UPDATE_COMMAND_UI(IDM_SuperPoseWave, OnUpdateSuperPoseWave)
ON_UPDATE_COMMAND_UI(IDM_TileWave, OnUpdateTileWave)
	//}}AFX_MSG_MAP

	/*ON_COMMAND(IDM_HALF_MODE, &CADDoc::OnHalfMode)
	ON_UPDATE_COMMAND_UI(IDM_HALF_MODE, &CADDoc::OnUpdateHalfMode)*/


END_MESSAGE_MAP()
///////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CADDoc construction/destruction

CADDoc::CADDoc()
{
	// TODO: add one-time construction code here
	m_bProgress=FALSE; // һ��ʼ�����½�����
	nProcessMode = DATA_MODE;    // Ĭ�Ϸ�ʽ��������ʾ
	m_TriggerVolt=(int)VOLT_RANGE;	// �ô�����ƽ�ĳ�ֵΪ���������
	m_bFirstCreateDeviceFile=TRUE;  // һ��ʼ��Ϊ��һ�δ����ļ�����
	m_bCloseFile=TRUE; // û�д��ļ�ʱ����Ϊ�ļ����ر�	
	gl_nChannelCount=0;
	USB2881_LoadParaAD(NULL, &ADPara);  // ����ϵͳ����

	gl_nChannelCount = MAX_CHANNEL_COUNT;




	int a =0;
	int b = sizeof(int);

}

CADDoc::~CADDoc()
{
}

BOOL CADDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CADDoc serialization

void CADDoc::Serialize(CArchive& ar)
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
// CADDoc diagnostics

#ifdef _DEBUG
void CADDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

BOOL gl_bTerminateDrawThr = TRUE;
void CADDoc::StartDeviceAD() 
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	////////////////////////////////////////////////////////////////////////
	if(pApp->m_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(L"�Բ������豸����...",MB_ICONERROR);
		return;
	}
	if(!gl_bTerminateDrawThr)
	{
		AfxMessageBox(L" �߳�û����ֹ");
		return;
	}
	FileLenghtWords = 256; // �ļ����ȳ�ʼ��
	m_DrawWindowThread = AfxBeginThread(ProccessDataThread,NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_DrawWindowThread->m_bAutoDelete = TRUE; // �Զ�ɾ��    
	
	// �������ݲɼ��봰�ڻ���ͬ���¼�����
	hEvent = USB2881_CreateSystemEvent();
	hExitEvent = USB2881_CreateSystemEvent();
	bCollectData=TRUE;  // �����豸����󣬿��Խ������ݲɼ�
	for(int i=0;i<MAX_CHANNEL_COUNT;i++)
		gl_MiddleLsb[i]=0;

	
	
	if(!m_bFirstCreateDeviceFile) // ����������ļ�����
	{
		for (int Index=0; Index<gl_nChannelCount; Index++)
		{
			header[Index].HeadSizeBytes = sizeof(FILE_HEADER);   // ͷ��Ϣ��С
			header[Index].FileType = DEFAULT_FILE_TYPE;
			header[Index].BusType = DEFAULT_BUS_TYPE;        // ���豸��������
			header[Index].DeviceNum = DEFAULT_DEVICE_NUM;	 // �豸ID��
			header[Index].ChannelCount = 1;                 // ÿͨ�����Ƕ����ģ�����Ϊ1
			header[Index].DataWidth = DEFAULT_DATA_WIDTH;   // 12λ
			header[Index].bXorHighBit = 0;                  // ��λ������
			
			// ��û������
		
		
					switch(ADPara.InputRange[Index])	// ��ѹ����
					{		
					case USB2881_INPUT_N10000_P10000mV: // ��1000����
						header[Index].VoltTopRange = 10000;
						header[Index].VoltBottomRange = -10000;
						break;
					case USB2881_INPUT_N5000_P5000mV: // ��5000����
						header[Index].VoltTopRange = 5000;
						header[Index].VoltBottomRange = -5000;
						break;
					}
		
		
			header[Index].ADPara = ADPara;
			header[Index].CrystalFreq = 80000000; // ����Ƶ��(80 MHz)
			header[Index].HeadEndFlag = HEAD_END_FLAG;
			header[Index].ChannelNum = gl_ChannelArrary[Index]; // ͨ����
			header[Index].ADStatus = gl_ADStatus;
			
			// д�ļ�ͷ

			DWORD dTmp;
			if (!WriteFile(hFileObject[Index], (WORD*)&header[Index], header[Index].HeadSizeBytes,
				&dTmp, NULL))
			{
				AfxMessageBox(L"error");
			}
		}
	}
	
	// ����ͨ����������������ͼ������Χ
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(m_hWndDigit)); 
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // ȡ����֡���ھ��
	
	CProgressCtrl* pProgress=(CProgressCtrl*)(pShow->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));	
	pProgress->SetPos(0);  // �ͷ��豸ʱ���ý�����Ϊ��ֵ
	

	// �����豸
	if(!MyStartDeviceAD(pApp->m_hDevice)) // �����豸,��ʼ�ɼ�����
	{
		AfxMessageBox(L"�豸����ʧ��");
		return;
	}

	gl_bDeviceADRun = TRUE;
	if(pParaCfgView != NULL)
	{
		pParaCfgView->EnableWindows(FALSE); // ���豸��ʼ����ʱ����ֹ����Ӳ������
		pParaCfgView->StartAD();
	}
	m_DrawWindowThread->ResumeThread(); // �������ƴ����߳�
}

BOOL gl_Event = FALSE;
//###################  ���������߳� ################################################3### 
UINT ProccessDataThread(PVOID hWnd)  
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADFrame *pFrm=pApp->m_ADFrame ;
	CADDoc* pDoc=pApp->m_pADDoc; 
	// ���߳���ȡ����ͼָ��
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CString str;
	BOOL bFirst=TRUE;    
	ULONG WroteMB=0;  
	
	pDoc->m_Wrote8KWCounter=0;
	pDoc->m_WroteMB=0;
	CurrentIndex=0;
	LONG lTime = 0;
	LONG lBufferID = 0;
	LONG nADChannel;
	DWORD dTmp;
	int i=0;
	while(gl_bDeviceADRun)  // ѭ������AD����
	{ 

		do{

			if(!gl_bDeviceADRun)
				goto ExitProcDataThread;
			Sleep(1);
		} while(gl_bSegMentFlag[lBufferID] == FALSE);
	//} while(WaitForSingleObject(hEvent, 100) != WAIT_OBJECT_0);
		gl_bTerminateDrawThr = FALSE;
		gl_Event = TRUE;
		switch(nProcessMode)  // ���ݴ���
		{
		case DATA_MODE: // ���ִ���

			if (GetTickCount()-lTime>30)
			{
				::SendMessage(pDoc->m_hWndDigit, WM_SHOWDIGIT, NULL, NULL);	
				lTime = GetTickCount();
			}	

			while (gl_bSegMentFlag[lBufferID])
			{
				gl_bSegMentFlag[lBufferID] = FALSE;
				lBufferID++;
				if (lBufferID >= MAX_SEGMENT)
					lBufferID = 0;
			}
			m_bProgress=TRUE; // ʹOnDraw�����ܸ��½�����	
			break; 
		case WAVE_MODE:  // ���δ���
			if (GetTickCount()-lTime>30)
			{
				::SendMessage(pDoc->m_hWndWave,WM_SHOWWAVE, NULL, NULL);
				lTime = GetTickCount();
			}	
			while (gl_bSegMentFlag[lBufferID])
			{
				gl_bSegMentFlag[lBufferID] = FALSE;
				lBufferID++;
				if (lBufferID >= MAX_SEGMENT)
					lBufferID = 0;
			}
			m_bProgress=TRUE; // ʹOnDraw�����ܸ��½�����
			break;
		case SAVE_MODE: // ���ݴ��̴���	
			while (gl_bSegMentFlag[lBufferID])
			{
				for (nADChannel = 0; nADChannel<gl_nChannelCount; nADChannel++)
				{
					WriteFile(hFileObject[nADChannel], ADBuffer[nADChannel][lBufferID], gl_lSegSaveSize[lBufferID], &dTmp, NULL);
				}		
				FileLenghtWords = FileLenghtWords+gl_lSegSaveSize[lBufferID]*gl_nChannelCount;	// �����ļ���С����
				gl_bSegMentFlag[lBufferID] = FALSE;
				lBufferID++;
				if (lBufferID >= MAX_SEGMENT)
					lBufferID = 0;
			}
			WroteMB=(WroteMB+32)%1024;
			pFrm->m_Progress()->SetPos((int)WroteMB);					
			str.Format(L"%d", (FileLenghtWords/1024));
			i=str.GetLength()-3;
			while(i>0)
			{
				str.Insert(i, ',');
				i-=3;
			}
			str+=" Kb";
			pFrm->m_FileLenght()->SetWindowText(str);
			break;
		}//end switch  

	}//end while
ExitProcDataThread:
	gl_bTerminateDrawThr = TRUE; // �߳���ֹ
	return TRUE;
}
//##########################################################################################

void CADDoc::Public_ReleaseDevice(void) // ��MDI���ڵ���
{
	StopDeviceAD();
}

void CADDoc::StopDeviceAD() 
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
//	gl_bDeviceADRun = FALSE; // ֹͣ���߳̽����������ݲɼ�	
	
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // ȡ����֡���ھ��
	if(pParaCfgView != NULL)
	{
		pParaCfgView->EnableWindows(TRUE);	

	}
	
	if(!MyStopDeviceAD(pApp->m_hDevice)) // �ر�AD�豸
	{
		AfxMessageBox(L"�ر�AD�豸ʧ��!");
		return;
	}

	WaitForSingleObject(hExitEvent, 100);  // �ȴ��ɼ��߳��˳�
	USB2881_ReleaseSystemEvent(hEvent);	   // �ͷ���Ϣ
	// ��ֹ�ɼ��߳�
	CProgressCtrl* pProgress=(CProgressCtrl*)(pShow->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
	pProgress->SetPos(0);  // �ͷ��豸ʱ���ý�����Ϊ��ֵ
	
	// ���豸�����ͷź��������Ӳ������
	/*if(pParaCfgView != NULL)
	{
		pParaCfgView->EnableWindows(TRUE);	

	}*/
	bCreateDevice = FALSE;     // �ͷ��豸�����豸δ����
	
	if(nProcessMode == SAVE_MODE)  // ������ݴ���ʽΪ����
	{
		
		OnCloseFile();//�ͷ��ļ�
		nProcessMode = DATA_MODE; // �ó����ַ�ʽ	
		if(pADStatusView != NULL)
		{
			CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
			pDataSave->EnableWindow(FALSE);  // ʹ���̷�ʽ��ѡ����Ч

		}
	}


	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();	
	pWaveView->OnDrawPolyLine();
	/*CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(m_hWndDigit));
	pDigitView->Invalidate(); */
	
}


void CADDoc::OnDataFileNew() 
{
	// TODO: Add your command handler code here
	CString strNewFileName;
	CString TempFileName;
	CSysApp* pApp=(CSysApp*)AfxGetApp();
	if (!(pApp->DoPromptFileName(strNewFileName, IDS_NEW_FILE,
		OFN_HIDEREADONLY | OFN_CREATEPROMPT, TRUE, NULL)))
		return;
	if(!m_bFirstCreateDeviceFile)
	{  // ������ǵ�һ�δ����ļ�����,���ͷ����ļ�����
		for (int nCH=0; nCH<gl_nChannelCount; nCH++)
		{			

			if ( ! CloseHandle( hFileObject[nCH] ) )
			{
				AfxMessageBox(L"�ͷ��ļ�����ʧ��...",MB_ICONSTOP,0);
			}
		}
	} 
	
	// �������ļ�����

		int FileNameLength=strNewFileName.GetLength();
		TempFileName=strNewFileName.Left(FileNameLength-4);
	for (int nCH=0; nCH<gl_nChannelCount; nCH++)
	{		
		CString str;
		str.Format(L"-%d.usb", gl_ChannelArrary[nCH]);
		strNewFileName = TempFileName + str;
		hFileObject[nCH] =CreateFile(strNewFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);

		if ( hFileObject[nCH] == INVALID_HANDLE_VALUE )		
		{
			AfxMessageBox(L"��ʼ���ļ�����ʧ��...",MB_ICONSTOP,0);
			return;
		}	
		// ����򿪵��ļ�����
		g_strFileFullName[nCH] = strNewFileName;

	}


	
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // ȡ����֡���ھ��
	pShow->ShowHideDlgSave ();	//��ʾ������
	pShow->ShowHideStatus(FALSE);//����״̬��
	if(pADStatusView != NULL)
	{
		CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
		pDataSave->EnableWindow(TRUE);  // ʹ���̷�ʽ��ѡ����Ч
	}

	ULARGE_INTEGER  uLarge;
	GetDiskFreeSpaceEx(strNewFileName.Left(3),NULL,NULL,&uLarge);  // ȡ���ļ����ڴ��̿��ÿռ�

	ULONGLONG DiskFreeBytes=uLarge.QuadPart;	
	m_RemainMB=(ULONG)(DiskFreeBytes/(1024*1024));  // ����ж���M��	
	CString str;
	
	if(m_RemainMB<2) 
	{
		AfxMessageBox(L"Ӳ��ʣ��ռ䲻��2M�����̲������ܽ���...",MB_ICONSTOP,0);
		return;
	}
	str.Format(L"%d Mb",m_RemainMB);
	pShow->m_DiskFree()->SetWindowText (str);// �õ�ǰӲ�̿��ÿռ�MB
	
	pShow->m_Progress()->SetRange32(0,1024);// ���ý��̷�Χ
	pShow->m_Progress()->SetPos(0);
	
	/////////////
	pShow->m_FileName()->SetWindowText(strNewFileName);//��ʾ�����ļ���
	m_bFirstCreateDeviceFile=FALSE;  // ���ɹ��ļ��������ñ�־	
	
	m_bCloseFile=FALSE; // ���ļ�û�йر�,���Բ������رյ�ǰ�ļ����˵�
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
	nProcessMode = SAVE_MODE; // �������ļ�ʱ����ζ��Ҫ���д��̴���
	
	pADStatusView->SetDlgCheck();
}

void CADDoc::OnCloseFile() 
{
	// TODO: Add your command handler code here
	DWORD dTmp;
	for(int nChannel=0; nChannel<gl_nChannelCount; nChannel++)
	{
		header[nChannel].HeadSizeBytes = sizeof(FILE_HEADER);
		header[nChannel].ADStatus = gl_ADStatus; // gl_ADStatus.bTrigFlag;


		SetFilePointer(hFileObject[nChannel], 0, 0, FILE_BEGIN);
		if (!WriteFile(hFileObject[nChannel], &header[nChannel], sizeof(FILE_HEADER), &dTmp, NULL))
		{
			AfxMessageBox(L"error");
		}
		CloseHandle( hFileObject[nChannel]);
	}

	CString strTitle;
	strTitle = GetTitle(); // Ϊ��ֹSetPathName�ı��ĵ���Title������ǰ����
	// ��ӵ� ���򿪵��ļ��б���





	m_bCloseFile=TRUE;  // �ļ����ر�
	nProcessMode = DATA_MODE; // ���ر����ļ�ʱ����ζ��Ҫ�����ٽ��д��̴�������ó����ַ�ʽ
	m_bFirstCreateDeviceFile=TRUE; // �����ļ����رպ���ζ���Ժ����ļ����ǵ�һ�α����� 
	///////
	CADFrame* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrame ; // ȡ����֡���ھ��
	CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);  // ʹ���̷�ʽ��ѡ����Ч
	
	CString str;
	str.Format(L"%d", 0);
	pADFrm->m_DiskFree()->SetWindowText(str);

	if (gl_bAutoCollect)
	{		
		SetPathName(g_strFileFullName[1]);
		SetPathName(g_strFileFullName[0]); 
		SetTitle(strTitle); // �ָ��ĵ���Title
		pADFrm->ShowHideDlgSave (FALSE); // ���ش�����
		pADFrm->ShowHideStatus();//��ʾ״̬��
	}
	     

	pADFrm->m_Progress()->SetRange32(0,1024);  // ���ý��̷�Χ
	pADFrm->m_Progress()->SetPos(0);	
	// һ�׽���
	pADStatusView->SetDlgCheck();	
}


void CADDoc::OnTileWave() 
{
	// TODO: Add your command handler code here
	gl_bTileWave = TRUE;
	CButton* pButtonTile = (CButton*)pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(1);
	CButton* pButtonPose = (CButton*)pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(0);
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();	
	pWaveView->OnDrawPolyLine();
}

void CADDoc::OnSuperPoseWave() 
{
	// TODO: Add your command handler code here
	gl_bTileWave = FALSE;
	CButton* pButtonTile = (CButton*)pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(0);
	CButton* pButtonPose = (CButton*)pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(1);
    CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	pWaveView->OnDrawPolyLine();
	
}

void CADDoc::OnUpdateSuperPoseWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_bTileWave==TRUE ? 0:1);
}

void CADDoc::OnUpdateTileWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_bTileWave==TRUE ? 1:0);
	
}






