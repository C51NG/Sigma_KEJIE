// HistDigitView.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ADHistDoc.h"
#include "ADHistDigitView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView

IMPLEMENT_DYNCREATE(CADHistDigitView, CListView)

CADHistDigitView::CADHistDigitView()
{
	nChooseMode=1;
}

CADHistDigitView::~CADHistDigitView()
{
}


BEGIN_MESSAGE_MAP(CADHistDigitView, CListView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CADHistDigitView)
	ON_COMMAND(IDM_SixteenShow, OnSixteenShow)
	ON_UPDATE_COMMAND_UI(IDM_SixteenShow, OnUpdateSixteenShow)
	ON_COMMAND(IDM_DigitalVoltShowMode, OnDigitalVoltShowMode)
	ON_UPDATE_COMMAND_UI(IDM_DigitalVoltShowMode, OnUpdateDigitalVoltShowMode)
	ON_COMMAND(IDM_TenShow, OnTenShow)
	ON_UPDATE_COMMAND_UI(IDM_TenShow, OnUpdateTenShow)
	ON_COMMAND(IDM_REFURBISH, OnRefurbish)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView drawing

void CADHistDigitView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	
}
/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView diagnostics

#ifdef _DEBUG
void CADHistDigitView::AssertValid() const
{
	CListView::AssertValid();
}

void CADHistDigitView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
CADHistDoc* CADHistDigitView::GetDocument()// non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADHistDoc)));
	return (CADHistDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView message handlers

void CADHistDigitView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class

	DWORD type;
	type=LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |
			LVS_EX_HEADERDRAGDROP | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_SUBITEMIMAGES;	
	// 改变列表视图的风格，在这里为显示格线
	GetListCtrl().SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,LPARAM(type));	

	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	pDoc->m_hWndDigit=m_hWnd; 

	CString str;
	CListCtrl& List=GetListCtrl();
	
	int ChannelCount = 1;  //32
	long VoltTopRange, VoltBottomRange;
	VoltTopRange = pDoc->m_Header.VoltTopRange;       // 电压的最大值
	VoltBottomRange = pDoc->m_Header.VoltBottomRange; // 电压的最小值
	m_PerLsbVolt = (float)((VoltTopRange - VoltBottomRange) / LSB_COUNT);

	CADHistFrm* pFrame=(CADHistFrm*)GetParentFrame();	
	CListBox* pList=(CListBox*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_FILESINFO));
	switch(pDoc->m_Header.BusType)
	{//1:PCI, 2:USB, 3:ISA, 4:PC104
		case PCI_BUS:
			str="BusType:PCI";
			break;
		case USB_BUS:
			str="BusType:USB";
			break;
		case ISA_BUS:
			str="BusType:ISA";
			break;
		case PC104_BUS:
			str="BusType:PC104";
			break;
		case PXI_BUS:
			str="BusType:PXI";
			break;
		case PCIE_BUS:
			str="BusType:PCIE";
			break;
	}//板卡类型
	pList->AddString (str);
	str.Format(L"DeviceNum:%x",pDoc->m_Header.DeviceNum );
	pList->AddString (str);
	str.Format(L"HeadSizeBytes:%d",pDoc->m_Header.HeadSizeBytes );
	pList->AddString(str);	

	str.Format(L"分辨率:%x位", pDoc->m_Header.DataWidth);
	pList->AddString(str);
	str.Format(L"电压量程:%d～%dmV",pDoc->m_Header.VoltBottomRange,pDoc->m_Header.VoltTopRange);
	pList->AddString (str);//电压量程
	str.Format(L"采样频率:%d", pDoc->m_Header.ADPara.Frequency);
	pList->AddString (str);//采样频率
	str.Format(L"触发源:%d",pDoc->m_Header.ADPara.TriggerSource);
	pList->AddString (str);//触发源
	str.Format(L"触发方向:%d",pDoc->m_Header.ADPara.TriggerDir);
	pList->AddString (str);


	CEdit* pEditOffset=(CEdit*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	str.Format(L"%d", pDoc->m_Offset);
	pEditOffset->SetWindowText(str);  // 显示文件偏移

	CEdit* pLength=(CEdit*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_EDIT_FileLength));	
	str.Format(L"%d", pDoc->m_FileLength);
	pLength->SetWindowText(str);  // 显示文件长度（字）

	
	CSliderCtrl* pSliderOffset=(CSliderCtrl*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	

	// 之所以除以通道数，是求出每通道点数
	pSliderOffset->SetRange(0, ((pDoc->m_FileLength-pDoc->m_ReadDataSize)/1));  // pDoc->m_ChannelCount设置滑动条范围（字）
	pSliderOffset->SetPageSize(1000);

	CEdit* pEditPerChannel=(CEdit*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_EDIT_PerChannel));	
    str.Format(L"%d", pDoc->m_FileLength/1); //pDoc->m_ChannelCount
	pEditPerChannel->SetWindowText(str);  // 显示每通道点数

	CComboBox* pRate=(CComboBox*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_COMBO_Rate));	
	pRate->SetCurSel(0);

	CStatic* pLabel=(CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_CardType);
	pLabel->SetWindowText(pDoc->m_File.GetFilePath()); 

	List.InsertColumn(0, L"Index" ,LVCFMT_LEFT,42,-1);  // 加入第一列
	ChannelCount=1/*pDoc->m_ChannelCount*/;	
	int Row=0, Col=0;
	str.Format(L"CH%d", pDoc->m_Header.ChannelNum);
	List.InsertColumn(1, str ,LVCFMT_LEFT,55,-1);

	for(Row=0; Row<(int)(pDoc->m_ReadDataSize/1/*pDoc->m_ChannelCount*/); Row++)
	{
		str.Format(L"%d", Row);
		List.InsertItem(Row,str);  // 添加行号,应一个紧接一个插入	
	} 
	str.Format(L"数据回放:%s",pDoc->m_File.GetFileName());
	pDoc->SetTitle(str);
	SetListView();

}

void CADHistDigitView::SetListView(void)
{
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	CListCtrl& List=GetListCtrl();
	UINT Col=0;
	int Row;
	List.ShowWindow(SW_HIDE);
//	List.SetRedraw(FALSE);
	for(Row=0; Row<(int)(pDoc->m_ReadDataSize); Row++)
	{		
		swprintf_s(str,L"%d", pDoc->m_Offset+Row);
		List.SetItemText(Row, 0, str);  // 更新Index列
		for(Col=0; Col</*(UINT)1*/pDoc->m_ChannelCount; Col++)  
		{
			
			switch(nChooseMode)
			{
				case 1://十进制显示
					swprintf_s(str,L"%d", (pDoc->m_ADBuffer[Row*1+Col]^0x8000)&MAX_LSB);
					break;
				case 2://十六进制显示
					swprintf_s(str,L"%5x", (pDoc->m_ADBuffer[Row*1+Col])&MAX_LSB);	
					break;
				case 3:
					swprintf_s(str,L"%.2f", (float)((((pDoc->m_ADBuffer[Row*1+Col]^0x8000)&MAX_LSB)-LSB_HALF)*m_PerLsbVolt));	
					break;
			}
			List.SetItemText(Row,Col+1, str);// 添加各行内容
		} 
	} 
	List.SetRedraw(TRUE);
	List.ShowWindow(SW_SHOW);

}


BOOL CADHistDigitView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~(LVS_LIST | LVS_ICON | LVS_SMALLICON);
	cs.style |= LVS_REPORT;
	cs.style |= LVS_SINGLESEL;
	return CListView::PreCreateWindow(cs);
}

void CADHistDigitView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CListView::OnUpdate(pSender, lHint, pHint);
	
}

#include "resource.h"
void CADHistDigitView::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_HisMouseR));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

void CADHistDigitView::OnSixteenShow() 
{
	// TODO: Add your command handler code here
	//AfxMessageBox(L"十六进制");	
	nChooseMode=2;
	SetListView();//刷新列表
}

void CADHistDigitView::OnUpdateSixteenShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nChooseMode==2);
}

void CADHistDigitView::OnDigitalVoltShowMode() 
{
	// TODO: Add your command handler code here
	//AfxMessageBox(L"电压显示");
	nChooseMode=3;
	SetListView();//刷新列表
}

void CADHistDigitView::OnUpdateDigitalVoltShowMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nChooseMode==3);
}

void CADHistDigitView::OnTenShow() 
{
	// TODO: Add your command handler code here
	//AfxMessageBox(L"十进制显示");
	nChooseMode=1;//刷新列表
	SetListView();//刷新列表
}

void CADHistDigitView::OnUpdateTenShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nChooseMode==1);
}

void CADHistDigitView::OnRefurbish() //刷新列表
{
	// TODO: Add your command handler code here
	SetListView();//刷新列表

}

void CADHistDigitView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	CADHistDoc* pDoc=(CADHistDoc*)GetDocument();
	CRect rect;	
	GetWindowRect(&rect);	
	pDoc->m_nDigitWidth=rect.right+7;
	//pDoc->m_nDigitLine=(int)((cy/13.5)-1);	
	// TODO: Add your message handler code here	
}

void CADHistDigitView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default


	CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CADHistDigitView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CADHistDigitView::ProcessADLSB()
{
	
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	LONG lData = 0;
	WORD ADData = 0;
	LONGLONG lADBuffer = 0;
	int iData = 0;
	int iDataLength = (int)(pDoc->m_ReadDataSize);
	for(int iIndex=0;iIndex<iDataLength;iIndex++)
	{
		lADBuffer += ((pDoc->m_ADBuffer[iIndex]) & MAX_LSB);
	}
	lADBuffer /= iDataLength;
	
	//求均方根	
	
	for( int Row=0; Row<iDataLength; Row++)
	{
		for(int Col=0; Col<1; Col++)  
		{	
			ADData = ((pDoc->m_ADBuffer[Row*1+Col])&MAX_LSB);
			iData = ADData -(WORD)lADBuffer;
			lData = lData + (iData)*(iData);
		}
	}
	m_dSqrtValue = sqrtf((float)(lData/iDataLength)); 
}
