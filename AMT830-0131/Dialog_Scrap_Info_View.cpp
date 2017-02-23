// Dialog_Scrap_Info_View.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Scrap_Info_View.h"

#include "Dialog_Select.h"
#include "Dialog_Keyboard.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"

#include "io.h" 
#include "GridControlAlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Scrap_Info_View dialog


CDialog_Scrap_Info_View::CDialog_Scrap_Info_View(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Scrap_Info_View::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Scrap_Info_View)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "scrap info view Box";
}


void CDialog_Scrap_Info_View::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Scrap_Info_View)
	DDX_Control(pDX, IDC_BTN_SCRAP_INFO, m_btn_scrap_info);
	DDX_Control(pDX, IDC_BTN_YES, m_btn_yes);
	DDX_Control(pDX, IDC_BTN_NO, m_btn_no);
	DDX_Control(pDX, IDC_GROUP_SORTER_SCRAP_LIST, m_group_sorter_scrap_list);
	DDX_Control(pDX, IDC_GROUP_SORTER_SCRAP_INFO, m_group_sorter_scrap_info);
	DDX_Control(pDX, IDC_GROUP_QUESTION, m_group_question);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Scrap_Info_View, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Scrap_Info_View)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_YES, OnBtnYes)
	ON_BN_CLICKED(IDC_BTN_NO, OnBtnNo)
	ON_BN_CLICKED(IDC_BTN_SCRAP_INFO, OnBtnScrapInfo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK, OnCellClick)
	ON_MESSAGE(WM_SCRAP_DSP, OnScrap_Info_Display)  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Scrap_Info_View message handlers

BOOL CDialog_Scrap_Info_View::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// ************************************************************************** 
	// 컨트롤에 설정할 폰트 생성한다                                              
	// ************************************************************************** 
	mp_scrap_font = NULL;
	mp_scrap_font = new CFont;
	mp_scrap_font->CreateFont(15,0,0,0,0,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	
	mn_data_send_to_host = CTL_NO;
	(GetDlgItem(IDOK))->EnableWindow(false);

	OnScrapInfo_GroupBox_Set();
	OnScrapInfo_Init_List();
	OnScrapInfo_Init_Info();
	OnScrapInfo_Button_Set();

	OnScrap_Display_Info();
	OnScrap_Display_List();

//	mn_edit = CTL_NO;
	mn_timer_act = CTL_NO;
	mn_ec_server_step = 0;

	st_handler.cwnd_scrap_info = this;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Scrap_Info_View::OnDestroy() 
{
	st_handler.cwnd_scrap_info = NULL;

	delete mp_scrap_font;
	mp_scrap_font = NULL;

	delete mp_scrap_ok;      // OK 버튼에 설정된 정보 삭제 

	StandardDialog::OnDestroy();
}

void CDialog_Scrap_Info_View::OnTimer(UINT nIDEvent) 
{
	int nRet;

	if (nIDEvent == TMR_SCRAP_INFO)
	{
		nRet = OnScrapinfo_Server_Send();
		
		if (nRet == CTLBD_RET_GOOD)
		{
			mn_timer_act = CTL_NO;
			m_btn_yes.EnableWindow(true);
			m_btn_no.EnableWindow(true);
			m_btn_scrap_info.EnableWindow(true);
			(GetDlgItem(IDOK))->EnableWindow(true);
			KillTimer(TMR_SCRAP_INFO);
			OnOK();
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			mn_timer_act = CTL_NO;
			m_btn_yes.EnableWindow(true);
			m_btn_no.EnableWindow(true);
			m_btn_scrap_info.EnableWindow(true);
			(GetDlgItem(IDOK))->EnableWindow(true);
			KillTimer(TMR_SCRAP_INFO);
		}
		///////////////////////////////////////////////
		else if (nIDEvent == TMR_YESBTN_SCRAP_INFO) // 20130121
		{
			nRet = OnScrapInfo_YesBtn_Server_Send();
			
			if (nRet == RET_GOOD)
			{
				mn_timer_act = CTL_NO;
				m_btn_yes.EnableWindow(true);
				m_btn_no.EnableWindow(true);
				m_btn_scrap_info.EnableWindow(true);
				(GetDlgItem(IDOK))->EnableWindow(true);
				KillTimer(TMR_YESBTN_SCRAP_INFO); 
			//	OnOK(); // 20130121
			}
			else if (nRet == RET_ERROR)
			{
				mn_timer_act = CTL_NO;
				m_btn_yes.EnableWindow(true);
				m_btn_no.EnableWindow(true);
				m_btn_scrap_info.EnableWindow(true);
				(GetDlgItem(IDOK))->EnableWindow(true);
				KillTimer(TMR_YESBTN_SCRAP_INFO);
			}
		}
	///////////////////////////////////////////////

	}
	
	StandardDialog::OnTimer(nIDEvent);
}

void CDialog_Scrap_Info_View::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	
	Grid = NULL;
	delete Grid;
}

void CDialog_Scrap_Info_View::GridControl(UINT nID, int type, int row, int col, int pos)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	SS_CELLTYPE CellType;
	SS_DATEFORMAT dateFormat = { TRUE, '/', IDF_DDMMYY, FALSE };
	SS_TIMEFORMAT tmFormat;
	CString strList;
	strList = "combo1\tcombo2\tcombo3\tcombo4\tcombo5\tcombo6";
	double a = 0.5;

	switch (type)
	{
		case DATE:			
			Grid->SetTypeDate(&CellType, 0 , &dateFormat, NULL, NULL);
			break;

		case EDIT:
			Grid->SetTypeEdit(&CellType,ES_CENTER,60,SS_CHRSET_CHR,SS_CASE_NOCASE);
			break;

		case COMBO:
			// SetValue(col, row, data); data = ""0"(1선택) data = "1"(2선택) data = "2"(3선택)
			Grid->SetTypeComboBox(&CellType, 0, strList);
			break;

		case BUTTON:
			if (pos == 0)
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Go", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE, SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			else
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Read", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE,SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			break;

		case STATIC:
			Grid->SetTypeStaticText(&CellType, SS_TEXT_CENTER | SS_TEXT_VCENTER);
			break;

		case TIME:
			tmFormat.b24Hour=TRUE;
			tmFormat.bSpin=FALSE;
			tmFormat.bSeconds=FALSE;
			tmFormat.cSeparator=':';
			Grid->SetTypeTime(&CellType, 0, &tmFormat, NULL, NULL);
			break;

		case PERCENT:
			Grid->SetTypePercentEx(&CellType,ES_LEFTALIGN,2,.01,100,".",0,0,0,0,1.001);
			break;

		case CHECK1:
			Grid->SetTypeCheckBox(&CellType,BS_CENTER, "", 
				"CheckUp", BT_BITMAP,
				"CheckDown", BT_BITMAP,
				"CheckFocusUp", BT_BITMAP,
				"CheckFocusDown", BT_BITMAP,
				"CheckDisabledUp", BT_BITMAP,
				"CheckDisabledDown", BT_BITMAP);
			break;

		case NUMBER:
			Grid->SetTypeNumberEx(&CellType,0,pos, 0, 10000,0,".",",",0,0,1,0,1.001);
			break;
	}

	Grid->SetCellType(col,row,&CellType);

	Grid = NULL;
	delete Grid;
}

void CDialog_Scrap_Info_View::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CDialog_Scrap_Info_View::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CDialog_Scrap_Info_View::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}

void CDialog_Scrap_Info_View::OnScrapInfo_Init_List()
{
	TSpread *Grid;
	CString str_pos;
	int row = st_work.strMdlReject.GetCount() + 1;
	int col = 2;
	int i, j;
	CString str_tmp[100];
	
	Grid = (TSpread*)GetDlgItem(IDC_CST_SORTER_SCRAP_LIST);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, TRUE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 20);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 230);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 100);
			}
			
			if (i == 1)
			{
				GridColor(IDC_CST_SORTER_SCRAP_LIST, i, j, ORANGE_C, WHITE_C);
			}
			else
			{
				GridColor(IDC_CST_SORTER_SCRAP_LIST, i, j, WHITE_C, BLACK_C);
			}
			
			GridControl(IDC_CST_SORTER_SCRAP_LIST, STATIC, i, j, 0);
			GridFont(IDC_CST_SORTER_SCRAP_LIST, i, j, 15);
		}
	}
	
	GridData(IDC_CST_SORTER_SCRAP_LIST, 1, 1, "Module ID");
	GridData(IDC_CST_SORTER_SCRAP_LIST, 1, 2, "Fail code");
	
	int cnt = st_work.strMdlReject.GetCount();
	
	for (i = 0; i < cnt; i++)
	{
		str_tmp[i] = st_work.strMdlReject.GetAt(st_work.strMdlReject.FindIndex(i));
	}

	for (i = 2; i < row + 1; i++)
	{
		GridData(IDC_CST_SORTER_SCRAP_LIST, i, 1, str_tmp[i]);
	}

	for (i = 0; i < cnt; i++)
	{
		str_tmp[i] = st_work.strMdlRejectCode.GetAt(st_work.strMdlRejectCode.FindIndex(i));
	}
	
	for (i = 2; i < row + 1; i++)
	{
		GridData(IDC_CST_SORTER_SCRAP_LIST, i, 2, str_tmp[i]);
	}
	Grid = NULL;
	delete Grid;
}

void CDialog_Scrap_Info_View::OnScrapInfo_Init_Info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 4;//7;
	int col = 2;
	int i, j;
// 	CString str_tmp[7] = {"Lot no", "Pass", "BTO", "HTO", "NTO", "FAIL", "SCRAP TOTAL"};
	CString str_tmp[4] = {"Lot no", "Pass", "FAIL", "SCRAP TOTAL"}; // 20130121
	
	Grid = (TSpread*)GetDlgItem(IDC_CST_SORTER_SCRAP_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++)
		{
// 			Grid -> SetRowHeightInPixels(i, 20);
// 			
// 			if (j == 1)
// 			{
// 				Grid -> SetColWidthInPixels(j, 140);
// 			}
// 			else
// 			{
// 				Grid -> SetColWidthInPixels(j, 180);
// 			}
			Grid -> SetRowHeightInPixels(i, 35);   // 20130121
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 150); // 20130121
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 170); // 20130121
			}
			
			if (j == 1)
			{
				GridColor(IDC_CST_SORTER_SCRAP_INFO, i, j, PINK_D, BLACK_C);
			}
			else
			{
				if (i == 1)
				{
					GridColor(IDC_CST_SORTER_SCRAP_INFO, i, j, YELLOW_C, RED_C);
				}
				else if (i == 2)
				{
					GridColor(IDC_CST_SORTER_SCRAP_INFO, i, j, GREEN_C, BLACK_C);
				}
				else
				{
					GridColor(IDC_CST_SORTER_SCRAP_INFO, i, j, RED_C, YELLOW_C);
				}
			}
			
			GridControl(IDC_CST_SORTER_SCRAP_INFO, STATIC, i, j, 0);
			GridFont(IDC_CST_SORTER_SCRAP_INFO, i, j, 15);
		}
	}
	
	for (i = 1; i < row + 1; i++)
	{
		GridData(IDC_CST_SORTER_SCRAP_INFO, i, 1, str_tmp[i - 1]);
	}
	Grid = NULL;
	delete Grid;
}

void CDialog_Scrap_Info_View::OnScrapInfo_Button_Set()
{
	mp_scrap_ok = CreateOkButton(IDOK, this);
}

void CDialog_Scrap_Info_View::OnScrap_Display_Info()
{
	CString strTemp;

// 	GridData(IDC_CST_SORTER_SCRAP_INFO, 1, 2, st_handler.mstr_lot_name[0]);
// 
// 	strTemp.Format("%d", st_work.nMdlPassCount[1][0]);
// 	GridData(IDC_CST_SORTER_SCRAP_INFO, 2, 2, strTemp);
// 
// 	strTemp.Format("%d", st_work.nMdlBtoCount[1][0]);
// 	GridData(IDC_CST_SORTER_SCRAP_INFO, 3, 2, strTemp);
// 
// 	strTemp.Format("%d", st_work.nMdlHtoCount[1][0]);
// 	GridData(IDC_CST_SORTER_SCRAP_INFO, 4, 2, strTemp);
// 
// 	strTemp.Format("%d", st_work.nMdlNtoCount[1][0]);
// 	GridData(IDC_CST_SORTER_SCRAP_INFO, 5, 2, strTemp);
// 
// 	strTemp.Format("%d", st_work.nMdlRejectCount[1][0]);
// 	GridData(IDC_CST_SORTER_SCRAP_INFO, 6, 2, strTemp);
	GridData(IDC_CST_SORTER_SCRAP_INFO, 1, 2, st_NW.mstr_lot_id[1]); // 20130121
	
	strTemp.Format("%d", st_work.nMdlPassCount[0][0]);   // 20130121
	GridData(IDC_CST_SORTER_SCRAP_INFO, 2, 2, strTemp);
	
	strTemp.Format("%d", st_work.nMdlRejectCount[0][0]); // 20130121
	GridData(IDC_CST_SORTER_SCRAP_INFO, 3, 2, strTemp);
	
	strTemp.Format("%d", st_work.nMdlScrapTotal[0][0]); // 20130121
	GridData(IDC_CST_SORTER_SCRAP_INFO, 4, 2, strTemp);

}

void CDialog_Scrap_Info_View::OnScrapInfo_GroupBox_Set()
{
	CSxLogFont scrap_font(11, FW_THIN, false, "Bitstream Vera Sans Mono");
	
	m_group_question.SetFont(scrap_font);
	m_group_question.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_question.SetFontBold(TRUE);

	m_group_sorter_scrap_info.SetFont(scrap_font);
	m_group_sorter_scrap_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_sorter_scrap_info.SetFontBold(TRUE);

	m_group_sorter_scrap_list.SetFont(scrap_font);
	m_group_sorter_scrap_list.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_sorter_scrap_list.SetFontBold(TRUE);
}

void CDialog_Scrap_Info_View::OnBtnYes() 
{
	if (mn_data_send_to_host == CTL_NO)
	{
		if (mn_timer_act == CTL_NO)
		{
			mn_timer_act = CTL_YES;
			st_sync.mstr_scrap_info = "READY";
			m_btn_no.EnableWindow(false);
			m_btn_scrap_info.EnableWindow(false);
			(GetDlgItem(IDOK))->EnableWindow(false);
			SetTimer(TMR_SCRAP_INFO, 10, NULL);
		}
	}
}
//s0hl000202
void CDialog_Scrap_Info_View::OnBtnNo() 
{
// 	sprintf(st_buffer_tray[BT_REJECT_BUFFER].cSerialNumber[0][0], "32C6997A");
// 	sprintf(st_buffer_tray[BT_REJECT_BUFFER].cSerialNumber[0][1], "32C6937F");
// 
// 	sprintf(st_buffer_tray[BT_REJECT_BUFFER].cTestResult[0][0], "BTO");
// 	sprintf(st_buffer_tray[BT_REJECT_BUFFER].cTestResult[0][1], "BTO");
// 
// 	for (int i=0;i<2;i++)
// 	{
// // 		st_work.strMdlReject.AddHead(st_buffer_tray[BT_REJECT_BUFFER].cSerialNumber[0][i]);
// // 		st_work.strMdlRejectCode.AddHead(st_buffer_tray[BT_REJECT_BUFFER].cTestResult[0][i]);
// 	}

// 	int cnt = st_work.strMdlReject.GetCount();
// 
// 	mn_edit = CTL_YES;
}

void CDialog_Scrap_Info_View::OnCellClick(WPARAM wParam, LPARAM lParam)
{
//	if (mn_edit != CTL_YES)	return;

	int nId = -1;
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	
	switch (wParam)
	{
	case IDC_CST_SORTER_SCRAP_LIST:
		nId = 0;
		break;
		
	case IDC_CST_SORTER_SCRAP_INFO:
		nId = 1;
		break;
	}
	
	if (nId > -1)
	{
		OnChangeValue(nId, lpcc->Row, lpcc->Col);
	}
}

void CDialog_Scrap_Info_View::OnChangeValue(int nid, int nrow, int ncol)
{
	TSpread *Grid; 
	CString strTmp;
	char chvalue[100];
	char chText[100];
	CString strText;
	CString strText2;
	int n_response;
	int nPos = -1;
	int nReadVal;

	CDialog_Keyboard keyboard_dlg;
	CDialog_KeyPad pad_dlg;
	
	CString str_temp;	// 저장할 정보 임시 저장 변수 

	if (nid == 0)
	{
		if (nrow == 1)		return;

		Grid = (TSpread*)GetDlgItem(IDC_CST_SORTER_SCRAP_LIST);
		
		Grid->GetValue(ncol, nrow, chvalue);
		strTmp.Format("%s", chvalue);
		
		st_msg.mstr_typing_msg = strTmp;  // 키보드 상자 출력 데이터 설정
		
		n_response = keyboard_dlg.DoModal();
		
		if (n_response == IDOK)
		{
			strTmp = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정
			
			strTmp.MakeUpper();
			strTmp.TrimLeft(' ');               
			strTmp.TrimRight(' ');
			
			Grid->SetValue(ncol, nrow, strTmp);
			
			if (ncol == 1)				// 시리얼 번호 및 유무임.
			{
				if (strTmp == "")
				{
				}
				else
				{
				}
			}
			else if (ncol == 2)
			{
				if (strTmp == "")
				{
				}
				else
				{
				}
			}
		}
		else if (n_response == IDCANCEL)
		{
			
		}
	}
	else if (nid == 1)
	{
		if (ncol == 1)		return;
		else if(nrow == 1)   return; // 20130121
		else if(nrow == 2)   return; // 20130121
		else if(nrow == 3)   return; // 20130121

		Grid = (TSpread*)GetDlgItem(IDC_CST_SORTER_SCRAP_INFO);
		
		Grid->GetValue(ncol, nrow, chvalue);

		if (nrow != 1)
		{
			nReadVal = atoi(chvalue);
			strTmp.Format("%d", nReadVal);

			Grid->GetValue(1, nrow, chText);
			strText2 = chText;  // 라벨 정보 읽기
			
			st_msg.mstr_keypad_msg.Format("[Scrap Info] %s set", strText2);
			st_msg.mstr_keypad_val = strTmp;
			st_msg.mstr_pad_high_limit.Format("9999");
			st_msg.mstr_pad_low_limit.Format("0");
			st_msg.mn_dot_use    = CTL_NO;
			st_msg.mn_pad_dialog = CTL_YES;
			
			n_response = pad_dlg.DoModal();
			
			if (n_response == IDOK)
			{					
				strTmp = st_msg.mstr_keypad_val;

	//			m_sBasicSetting[nrow-1][1] = strTmp;
				
				Grid->SetValue(ncol, nrow, strTmp);
			}
			else if (n_response == IDCANCEL)
			{
				
			}
		}
		else
		{
			strTmp.Format("%s", chvalue);
			
			st_msg.mstr_typing_msg = strTmp;  // 키보드 상자 출력 데이터 설정
			
			n_response = keyboard_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				strTmp = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정
				
				strTmp.MakeUpper();
				strTmp.TrimLeft(' ');               
				strTmp.TrimRight(' ');
				
				Grid->SetValue(ncol, nrow, strTmp);
				
				if (strTmp == "")
				{
				}
				else
				{
				}
			}
			else if (n_response == IDCANCEL)
			{
				
			}
		}
	}
}

int CDialog_Scrap_Info_View::OnScrapinfo_Server_Send()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	CString strTemp, strMsgTemp, strHeader, strBody;
	CString strEvent[10];
	char chvalue[100];

	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CST_SORTER_SCRAP_INFO);

	CDialog_Message  msg_dlg;

	Func.ThreadFunctionStepTrace(1, mn_ec_server_step);

	switch (mn_ec_server_step)
	{
	case 0:
		if (st_work.mn_client_ready[NETWORK_ECSERVER] == CTL_YES)
		{
			Grid->GetValue(2, 1, chvalue);
			strTemp = chvalue;

			st_handler.mstr_lot_name[0] = strTemp;

			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
			{
				mn_ec_server_step  = 200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_ECSERVER, 0);
				}
				
				ml_wait_time[0] = GetCurrentTime();
				mn_ec_server_step  = 100;
			}
		}
		break;
		
	case 100:
		if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
		{
			mn_ec_server_step  = 200;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				nFuncRet = CTLBD_RET_ERROR;
				st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
				mn_ec_server_step  = 0;
			}
		}
		break;
		
	case 200:	
		st_work.mstr_client_send_msg[NETWORK_ECSERVER] = Func.OnNetworkDataMake(NW_ONLINE_END);

		st_NW.mn_invaild_function_flag = CTL_NO;
		st_work.mn_client_rpy[NETWORK_ECSERVER] = CTL_YES;
		st_sync.mstr_online_end = "READY";
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->SendMessage(CLIENT_SEND, NETWORK_ECSERVER, 0);
		}
		
		ml_retry_wait[0] = GetCurrentTime();
		mn_ec_server_step  = 300;
		break;
		
	case 300:						// Buffer ready에 대한 Reply가 오면 Buffer를 다음 장비로 넣는다.
		if (st_NW.mn_invaild_function_flag == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_ec_server_step  = 1500;
		}
		else if (st_sync.mstr_online_end == "PASS")
		{
			mn_data_send_to_host = CTL_YES;
			(GetDlgItem(IDOK))->EnableWindow(true);
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_ec_server_step  = 400;
		}
		else if (st_sync.mstr_online_end == "FAIL")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_ec_server_step  = 400;
		}
		else
		{
			ml_retry_wait[1] = GetCurrentTime();
			ml_retry_wait[2] = ml_retry_wait[1] - ml_retry_wait[0];
			
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			
			if (ml_retry_wait[2] > st_basic.mn_network_wait_time * 1000)		// 분으로 세팅하기 때문에 X 60... msec이기 때문에 X 1000
			{
				mn_ec_server_step  = 0;
			}
		}
		break;
		
	case 400:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_ec_server_step  = 1000;
		break;

	case 1000:
		if (st_work.mn_client_ready[NETWORK_ECSERVER] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
			{
				mn_ec_server_step  = 1200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_ECSERVER, 0);
				}
				
				ml_wait_time[0] = GetCurrentTime();
				mn_ec_server_step  = 1100;
			}
		}
		break;
		
	case 1100:
		if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
		{
			mn_ec_server_step  = 1200;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				nFuncRet = CTLBD_RET_ERROR;
				st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
				mn_ec_server_step  = 1000;
			}
		}
		break;
		
	case 1200:	
		st_work.mstr_client_send_msg[NETWORK_ECSERVER] = Func.OnNetworkDataMake(NW_SCRAP_INFO);
		
		st_work.mn_client_rpy[NETWORK_ECSERVER] = CTL_YES;
		st_NW.mn_scrap_info_flag = CTL_READY;
		st_sync.mstr_scrap_info = "READY";
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->SendMessage(CLIENT_SEND, NETWORK_ECSERVER, 0);
		}
		
		ml_retry_wait[0] = GetCurrentTime();
		mn_ec_server_step  = 1300;
		break;
		
	case 1300:						// Buffer ready에 대한 Reply가 오면 Buffer를 다음 장비로 넣는다.
		if (st_sync.mstr_scrap_info == "PASS")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_ec_server_step  = 1400;
		}
		else if (st_sync.mstr_scrap_info == "FAIL")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_ec_server_step  = 1500;
		}
		else
		{
			ml_retry_wait[1] = GetCurrentTime();
			ml_retry_wait[2] = ml_retry_wait[1] - ml_retry_wait[0];
			
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			
			if (ml_retry_wait[2] > st_basic.mn_network_wait_time * 1000)		// 분으로 세팅하기 때문에 X 60... msec이기 때문에 X 1000
			{
				mn_ec_server_step  = 1000;
			}
		}
		break;

	case 1400:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_ec_server_step  = 0;
		nFuncRet = CTLBD_RET_GOOD;
		break;

	case 1500:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_ec_server_step  = 0;
		nFuncRet = CTLBD_RET_ERROR;
		break;
	}

	return nFuncRet;
}

LRESULT CDialog_Scrap_Info_View::OnScrap_Info_Display(WPARAM wParam,LPARAM lParam) 
{
	int mn_state = wParam;
		
	OnScrap_Display_Info();
	OnScrap_Display_List();

	return 0;
}

void CDialog_Scrap_Info_View::OnScrap_Display_List()
{
	int i;
	int cnt = st_work.strMdlReject.GetCount();
	CString str_tmp[100];
	
	for (i = 0; i < cnt; i++)
	{
		str_tmp[i] = st_work.strMdlReject.GetAt(st_work.strMdlReject.FindIndex(i));
	}
	
	for (i = 2; i < cnt + 2; i++)
	{
		GridData(IDC_CST_SORTER_SCRAP_LIST, i, 1, str_tmp[i - 2]);
	}
	
	for (i = 0; i < cnt; i++)
	{
		str_tmp[i] = st_work.strMdlRejectCode.GetAt(st_work.strMdlRejectCode.FindIndex(i));
	}
	
	for (i = 2; i < cnt + 2; i++)
	{
		GridData(IDC_CST_SORTER_SCRAP_LIST, i, 2, str_tmp[i - 2]);
	}

	if (st_work.strMdlReject.GetCount() == 0)
	{
		(GetDlgItem(IDOK))->EnableWindow(true);
	}
}

void CDialog_Scrap_Info_View::OnBtnScrapInfo() 
{
	CString strTemp;
	char chvalue[100];

	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CST_SORTER_SCRAP_INFO);

	if (mn_timer_act == CTL_NO)
	{
		if (st_handler.mstr_lot_name[0] == "")
		{
			Grid->GetValue(2, 1, chvalue);
			strTemp = chvalue;
			
			st_handler.mstr_lot_name[0] = strTemp;
		}
		
		mn_ec_server_step  = 400;
		mn_timer_act = CTL_YES;
		st_sync.mstr_scrap_info = "READY";
		m_btn_no.EnableWindow(false);
		m_btn_yes.EnableWindow(false);
		(GetDlgItem(IDOK))->EnableWindow(false);
		SetTimer(TMR_SCRAP_INFO, 10, NULL);
	}
}


void CDialog_Scrap_Info_View::OnOK() 
{
	// TODO: Add extra validation here
	
	StandardDialog::OnOK();
}

int CDialog_Scrap_Info_View::OnScrapInfo_YesBtn_Server_Send() // 20130121
{
	int FuncRet = RET_PROCEED;
	int n_response;							// 대화 상자 리턴 플래그
	CDialog_Message msg_dlg;
	
	switch (n_scrapinfo_step)
	{
	case 0:
		// **********************************************************************
		//  서버와의 통신전에 접속 상태를 확인한다.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect == NO)		// 접속중이 아니면 접속을 시도한다.
		{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_7);
			NetworkConnectTime[0] = GetCurrentTime();
			n_scrapinfo_step = 10;
		}
		else													// 접속중이면...
		{
			n_scrapinfo_step = 100;
		}
		break;
		
	case 10:
		// ********************************************************************** 
		//  접속을 확인한다.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != YES)
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];
			
			if (NetworkConnectTime[2] > 5000)	// 5초 동안 연결이 되지 않으면...
			{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_7);
				n_scrapinfo_step = 5000;  
				st_msg.str_fallacy_msg = _T("[SERVER] 연결이 되지 않습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[SERVER] The connection will not be");
				}

				n_response = msg_dlg.DoModal();
			}
			if(n_response == IDOK)
			{

			}
		}
		else											// 연결이 되었다.
		{
			n_scrapinfo_step = 100;
		}
		break;
		
	case 100:
		// **********************************************************************
		//  시작을 알린다.
		// **********************************************************************
		sprintf(st_client[NETWORK_CLIENT_1].ch_send,"%s",Func.OnNetworkDataMake(NEW_LOT_END));
		
		//if (Ret == RET_GOOD)
		//{
		::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, NETWORK_CLIENT_7);	// 메인 프레임으로 메시지 전송
		NetworkConnectTime[0] = GetCurrentTime();
		n_scrapinfo_step = 200;
		//}
		break;
		
	case 200:
		// **********************************************************************
		//  완료가 될때까지 대기.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != NO)	// 작업이 끝나지 않았다면...
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];
			
			if (NetworkConnectTime[2] > 5000)	// 5초 동안 들어오지 않는다면...
			{
				n_scrapinfo_step = 5000;
				st_msg.str_fallacy_msg = _T("[SERVER] 5초간 답변이 오지 않았습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[SERVER] Answer did not come 5 seconds");
				}

				n_response = msg_dlg.DoModal();
				
				if (n_response == IDOK)
				{
					
				}
			}

		}
		else if (st_client[NETWORK_CLIENT_1].n_connect == NO)	// 작업이 끝났다면...
		{
			n_scrapinfo_step = 500;
		}	
	case 500:
		// 다 정상이니까, 테스터에게 새로운 LOT임을 알려준다.
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
		} 
		FuncRet = RET_GOOD;
		n_scrapinfo_step = 0;
		break;
		
	case 5000:
		FuncRet = RET_ERROR;
		n_scrapinfo_step = 0;
		break;

	default:
		break;
	}
	return FuncRet;
}