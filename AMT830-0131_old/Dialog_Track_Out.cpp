// Dialog_Track_Out.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Track_Out.h"

#include "Dialog_Select.h"
#include "Dialog_Keyboard.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"
#include "Dialog_Add_Fail.h"

#include "io.h" 
#include "GridControlAlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Track_Out dialog


CDialog_Track_Out::CDialog_Track_Out(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Track_Out::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Track_Out)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "track out view Box";
}


void CDialog_Track_Out::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Track_Out)
	DDX_Control(pDX, IDC_CHK_EDIT, m_chk_edit);
	DDX_Control(pDX, IDC_GROUP_SORTER_SCRAP_INFO, m_group_sorter_scrap_info);
	DDX_Control(pDX, IDC_GROUP_SERVER_SCRAP_LIST, m_group_server_scrap_info);
// 	DDX_Control(pDX, IDC_GROUP_ROUTER_SORTER_SCRAP_INFO, m_group_router_sorter_scrap_info);
// 	DDX_Control(pDX, IDC_GROUP_PCT_SORTER_SCRAP_INFO, m_group_pct_sorter_scrap_info);
// 	DDX_Control(pDX, IDC_GROUP_FCT_HANDLER_SCRAP_INFO, m_group_fct_handler_scrap_info);
	DDX_Control(pDX, IDC_BTN_TRACK_OUT, m_btn_track_out);
	DDX_Control(pDX, IDC_BTN_ADD_FAIL, m_btn_add_fail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Track_Out, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Track_Out)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_TRACK_OUT, OnBtnTrackOut)
	ON_BN_CLICKED(IDC_BTN_ADD_FAIL, OnBtnAddFail)
//	ON_BN_CLICKED(IDC_CHK_EDIT, OnChkEdit)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK, OnCellClick)
	ON_MESSAGE(WM_TRACKOUT_DSP, OnTrackOut_Info_Display)  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Track_Out message handlers

BOOL CDialog_Track_Out::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// ************************************************************************** 
	// 컨트롤에 설정할 폰트 생성한다                                              
	// ************************************************************************** 
	mp_trackout_font = NULL;
	mp_trackout_font = new CFont;
	mp_trackout_font->CreateFont(15,0,0,0,0,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

 	OnTrackOut_GroupBox_Set();
 	OnTrackOut_Button_Set();
	OnTrackOut_Display_Renew();
	
	mn_edit = CTL_NO;
	mn_timer_act = CTL_NO;
	mn_ec_server_step = 0;
	mn_scrap_info_step = 0;
	mn_delete_scrap_step = 0;
	mn_track_out = CTL_NO;
	mn_add_fail = CTL_NO;

// 	OnTrackOut_Disp_Edit();

	st_work.mstr_lot_no_check = st_handler.mstr_lot_name[0];

	SetTimer(TMR_TSCRAP_INFO, 10, NULL);
	
	st_handler.cwnd_track_out = this;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Track_Out::OnDestroy() 
{
	st_handler.cwnd_track_out = NULL;
	
	delete mp_trackout_font;
	mp_trackout_font = NULL;
	
	delete mp_trackout_ok;      // OK 버튼에 설정된 정보 삭제 
	KillTimer(TMR_TRACK_OUT);

	StandardDialog::OnDestroy();
}

void CDialog_Track_Out::OnTimer(UINT nIDEvent) 
{
	int nRet;
	
	if (nIDEvent == TMR_TRACK_OUT)
	{
		nRet = OnTrackOut_Server_Send();
		
		if (nRet == CTLBD_RET_GOOD)
		{
			mn_timer_act = CTL_NO;
			st_sync.mn_add_fail = CTL_NO;
			m_btn_add_fail.EnableWindow(true);
			m_btn_track_out.EnableWindow(true);
			(GetDlgItem(IDOK))->EnableWindow(true);
			OnTrackOut_Display_Renew();
			KillTimer(TMR_TRACK_OUT);
			OnOK();
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			mn_timer_act = CTL_NO;
			st_sync.mn_add_fail = CTL_NO;
			m_btn_add_fail.EnableWindow(true);
			m_btn_track_out.EnableWindow(true);
			(GetDlgItem(IDOK))->EnableWindow(true);
			KillTimer(TMR_TRACK_OUT);
		}
	}
	else if (nIDEvent == TMR_TSCRAP_INFO)
	{
		nRet = OnTrackOut_Server_Send_Scrap_Info();
		
		if (nRet == CTLBD_RET_GOOD)
		{
			mn_timer_act = CTL_NO;
			st_sync.mn_add_fail = CTL_NO;
			m_btn_add_fail.EnableWindow(true);
			m_btn_track_out.EnableWindow(true);
			(GetDlgItem(IDOK))->EnableWindow(true);
			OnTrackOut_Display_Renew();
			KillTimer(TMR_TSCRAP_INFO);

			if (st_work.n_lotdisply_qty != st_work.n_scrap_info_fail_qty + st_work.nMdlPassCount[1][0])
			{
				//m_btn_track_out.EnableWindow(false);
			}
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			mn_timer_act = CTL_NO;
			st_sync.mn_add_fail = CTL_NO;
			m_btn_add_fail.EnableWindow(true);
			m_btn_track_out.EnableWindow(false);
			(GetDlgItem(IDOK))->EnableWindow(true);
			KillTimer(TMR_TSCRAP_INFO);
		}
	}
	else if (nIDEvent == TMR_TDEL_SCRAP)
	{
		nRet = OnTrackOut_Server_Send_Delete_Scrap();
		
		if (nRet == CTLBD_RET_GOOD)
		{
			mn_timer_act = CTL_NO;
			st_sync.mn_add_fail = CTL_NO;
			m_btn_add_fail.EnableWindow(true);
			m_btn_track_out.EnableWindow(true);
			(GetDlgItem(IDOK))->EnableWindow(true);
			OnTrackOut_Display_Renew();
			KillTimer(TMR_TDEL_SCRAP);
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			mn_timer_act = CTL_NO;
			st_sync.mn_add_fail = CTL_NO;
			m_btn_add_fail.EnableWindow(true);
			m_btn_track_out.EnableWindow(true);
			(GetDlgItem(IDOK))->EnableWindow(true);
			KillTimer(TMR_TDEL_SCRAP);
		}
	}
	
	StandardDialog::OnTimer(nIDEvent);
}

void CDialog_Track_Out::OnBtnTrackOut() 
{
	int mn_response = 0;  // 대화 상자에 대한 리턴 값 저장 변수
	int Ret = 0;
	CString strTemp;
	
	CDialog_Select select_dlg;
	
	strTemp.Format("Track Out? [LOT:%s] PASS:%d, FAIL:%d", st_handler.mstr_lot_name[0], st_work.nMdlPassCount[1][0], st_work.strMdlReject.GetCount());

	st_msg.mstr_confirm_msg = _T(strTemp);
	
	mn_response = select_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		if (mn_timer_act == CTL_NO)
		{
			mn_track_out = CTL_YES;
			mn_add_fail = CTL_NO;
			
			mn_timer_act = CTL_YES;
			st_sync.mstr_track_out = "READY";
			m_btn_add_fail.EnableWindow(false);
			m_btn_track_out.EnableWindow(false);
			(GetDlgItem(IDOK))->EnableWindow(false);
			SetTimer(TMR_TRACK_OUT, 10, NULL);
		}
	}
	else if (mn_response == IDCANCEL)
	{

	}	
}

void CDialog_Track_Out::OnBtnAddFail() 
{
	int mn_response = 0;
	
	CDialog_Add_Fail		addfail_dlg;
	
	mn_response = addfail_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		SetTimer(TMR_TSCRAP_INFO, 10, NULL);
	}
}

void CDialog_Track_Out::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	
	Grid = NULL;
	delete Grid;
}

void CDialog_Track_Out::GridControl(UINT nID, int type, int row, int col, int pos)
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

void CDialog_Track_Out::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CDialog_Track_Out::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CDialog_Track_Out::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}

void CDialog_Track_Out::OnTrackOut_Button_Set()
{
	mp_trackout_ok = CreateOkButton(IDOK, this);
}

void CDialog_Track_Out::OnTrackOut_GroupBox_Set()
{
	CSxLogFont trackout_font(16, FW_BOLD, false, "Bitstream Vera Sans Mono");

	m_group_sorter_scrap_info.SetFont(trackout_font);
	m_group_sorter_scrap_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_sorter_scrap_info.SetFontBold(TRUE);

	m_group_server_scrap_info.SetFont(trackout_font);
	m_group_server_scrap_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_server_scrap_info.SetFontBold(TRUE);

// 	m_group_router_sorter_scrap_info.SetFont(trackout_font);
// 	m_group_router_sorter_scrap_info.SetCatptionTextColor(RGB(145, 25, 0));
// 	m_group_router_sorter_scrap_info.SetFontBold(TRUE);
// 
// 	m_group_pct_sorter_scrap_info.SetFont(trackout_font);
// 	m_group_pct_sorter_scrap_info.SetCatptionTextColor(RGB(145, 25, 0));
// 	m_group_pct_sorter_scrap_info.SetFontBold(TRUE);
// 
// 	m_group_fct_handler_scrap_info.SetFont(trackout_font);
// 	m_group_fct_handler_scrap_info.SetCatptionTextColor(RGB(145, 25, 0));
// 	m_group_fct_handler_scrap_info.SetFontBold(TRUE);
}

void CDialog_Track_Out::OnCellClick(WPARAM wParam, LPARAM lParam)
{
// 	if (mn_edit != CTL_YES)	return;
	
	int nId = -1;
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	
	switch (wParam)
	{
	case IDC_CST_ROUTER_SORTER_SCRAP_INFO:
		nId = 0;
		break;
		
	case IDC_CST_FCT_HANDLER_SCRAP_INFO:
		nId = 1;
		break;

	case IDC_CST_PCT_SORTER_SCRAP_INFO:
		nId = 2;
		break;

	case IDC_CST_SORTER_SCRAP_INFO: // 20130128
		nId = 3;
		break;
		
	case IDC_CST_SERVER_SCRAP_INFO: // 20130128
		nId = 4;
		break;

	}
	
	if (nId > -1)
	{
		OnChangeValue(nId, lpcc->Row, lpcc->Col);
	}
}

void CDialog_Track_Out::OnChangeValue(int nid, int nrow, int ncol)
{
	TSpread *Grid; 
	CString strTmp;
	char chvalue[100];
	CString strText;
	CString strText2;
	int n_response = 0;
	int nPos = -1;
	int nReadVal = 0;

	char chText[100]; // 20130128

	CString str_temp;	// 저장할 정보 임시 저장 변수 

	CDialog_Select select_dlg;
	CDialog_KeyPad pad_dlg; // 20130128

	
	if (nid == 0)
	{
		if (nrow == 0)		return;

		Grid = (TSpread*)GetDlgItem(IDC_CST_ROUTER_SORTER_SCRAP_INFO);
		
		Grid->GetValue(2, nrow, chvalue);
		strTmp.Format("%s", chvalue);
		mstr_del_mdl_id = strTmp;

		Grid->GetValue(3, nrow, chvalue);
		strTmp.Format("%s", chvalue);
		mstr_del_mdl_code = strTmp;

		strText.Format("Do you want delete? [Module ID : %s, Scrap Code : %s]", mstr_del_mdl_id, mstr_del_mdl_code);

		st_msg.mstr_confirm_msg = _T(strText);
		
		n_response = select_dlg.DoModal();
		
		if (n_response == IDOK)
		{
			SetTimer(TMR_TDEL_SCRAP, 10, NULL);
		}
	}
	else if (nid == 1)
	{
		if (nrow == 0)		return;

		Grid = (TSpread*)GetDlgItem(IDC_CST_FCT_HANDLER_SCRAP_INFO);
		
		Grid->GetValue(2, nrow, chvalue);
		strTmp.Format("%s", chvalue);
		mstr_del_mdl_id = strTmp;
		
		Grid->GetValue(3, nrow, chvalue);
		strTmp.Format("%s", chvalue);
		mstr_del_mdl_code = strTmp;
		
		strText.Format("Do you want delete? [Module ID : %s, Scrap Code : %s]", mstr_del_mdl_id, mstr_del_mdl_code);
		
		st_msg.mstr_confirm_msg = _T(strText);
		
		n_response = select_dlg.DoModal();
		
		if (n_response == IDOK)
		{
			SetTimer(TMR_TDEL_SCRAP, 10, NULL);
		}
	}
	else if (nid == 2)
	{
		if (nrow == 0)		return;
		
		Grid = (TSpread*)GetDlgItem(IDC_CST_PCT_SORTER_SCRAP_INFO);
		
		Grid->GetValue(2, nrow, chvalue);
		strTmp.Format("%s", chvalue);
		mstr_del_mdl_id = strTmp;
		
		Grid->GetValue(3, nrow, chvalue);
		strTmp.Format("%s", chvalue);
		mstr_del_mdl_code = strTmp;
		
		strText.Format("Do you want delete? [Module ID : %s, Scrap Code : %s]", mstr_del_mdl_id, mstr_del_mdl_code);
		
		st_msg.mstr_confirm_msg = _T(strText);
		
		n_response = select_dlg.DoModal();
		
		if (n_response == IDOK)
		{
			SetTimer(TMR_TDEL_SCRAP, 10, NULL);
		}
	}
	else if (nid == 3) // 20130128
	{
		if (ncol == 1)		return;
// 		else if(nrow == 1)   return; // 20130121
// 		else if(nrow == 2)   return; // 20130121
// 		else if(nrow == 3)   return; // 20130121
		
		
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
	}
	else if (nid == 4) // 20130128
	{
		if (ncol == 1)		return;
// 		else if(nrow == 1)   return; // 20130121
// 		else if(nrow == 2)   return; // 20130121
// 		else if(nrow == 3)   return; // 20130121
		
		
		Grid = (TSpread*)GetDlgItem(IDC_CST_SERVER_SCRAP_INFO);
		
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
	}
}

LRESULT CDialog_Track_Out::OnTrackOut_Info_Display(WPARAM wParam,LPARAM lParam) 
{
	int mn_state = wParam;
	
// 	OnScrap_Display_Info();
// 	OnScrap_Display_List();
	
	return 0;
}

void CDialog_Track_Out::OnTrackOut_Init_Sorter_Scrap_Info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 4;//7;
	int col = 2;
	int i, j;
//	CString str_tmp[7] = {"Lot no", "Pass", "BTO", "HTO", "NTO", "FAIL", "SCRAP TOTAL"};
	CString str_tmp[4] = {"Lot no", "Pass", "FAIL", "SCRAP TOTAL"};  // 20130128
	
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
//			Grid -> SetRowHeightInPixels(i, 20);
			Grid -> SetRowHeightInPixels(i, 34);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 140);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 180);
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

void CDialog_Track_Out::OnTrackOut_Init_Server_Scrap_Info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 4;
	int col = 2;
	int i, j;
	CString str_tmp[4] = {"Lot no", "Lot QTY", "Pass QTY", "Scrap Total"};
	
	Grid = (TSpread*)GetDlgItem(IDC_CST_SERVER_SCRAP_INFO);
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
			Grid -> SetRowHeightInPixels(i, 34);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 140);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 180);
			}
			
			if (j == 1)
			{
				GridColor(IDC_CST_SERVER_SCRAP_INFO, i, j, PINK_D, BLACK_C);
			}
			else
			{
				if (i == 1)
				{
					GridColor(IDC_CST_SERVER_SCRAP_INFO, i, j, YELLOW_C, RED_C);
				}
				else if (i == 2)
				{
					GridColor(IDC_CST_SERVER_SCRAP_INFO, i, j, GREEN_C, BLACK_C);
				}
				else if (i == 3)
				{
					GridColor(IDC_CST_SERVER_SCRAP_INFO, i, j, SKY_C, BLACK_C);
				}
				else
				{
					GridColor(IDC_CST_SERVER_SCRAP_INFO, i, j, RED_C, YELLOW_C);
				}
			}
			
			GridControl(IDC_CST_SERVER_SCRAP_INFO, STATIC, i, j, 0);
			GridFont(IDC_CST_SERVER_SCRAP_INFO, i, j, 15);
		}
	}
	
	for (i = 1; i < row + 1; i++)
	{
		GridData(IDC_CST_SERVER_SCRAP_INFO, i, 1, str_tmp[i - 1]);
	}
	Grid = NULL;
	delete Grid;
}

void CDialog_Track_Out::OnTrackOut_Init_Router_Sorter_Scrap_Info()
{
// 	TSpread *Grid;
// 	CString str_pos;
// 	int row = st_work.strMdlRejectRouterSorter.GetCount();
// 	int col = 3;
// 	int i, j;
// 	CString str_tmp[100], strTemp;
// 	
// 	Grid = (TSpread*)GetDlgItem(IDC_CST_ROUTER_SORTER_SCRAP_INFO);
// 	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
// 	Grid -> SetBool(SSB_SHOWCOLHEADERS, TRUE);
// 	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
// 	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
// 	Grid -> SetBool(SSB_VERTSCROLLBAR, TRUE);
// 	Grid -> SetMaxRows(row);
// 	Grid -> SetMaxCols(col);
// 	
// 	for (i = 0; i <= row; i++)
// 	{
// 		for (j = 1; j <= col; j++)
// 		{
// 			Grid -> SetRowHeightInPixels(i, 20);
// 			
// 			if (j == 1)
// 			{
// 				Grid -> SetColWidthInPixels(j, 30);
// 			}
// 			else if (j == 2)
// 			{
// 				Grid -> SetColWidthInPixels(j, 170);
// 			}
// 			else
// 			{
// 				Grid -> SetColWidthInPixels(j, 90);
// 			}
// 			
// 			if (i == 0)
// 			{
// 				GridColor(IDC_CST_ROUTER_SORTER_SCRAP_INFO, i, j, ORANGE_C, WHITE_C);
// 			}
// 			else
// 			{
// 				GridColor(IDC_CST_ROUTER_SORTER_SCRAP_INFO, i, j, WHITE_C, BLACK_C);
// 			}
// 			
// 			GridControl(IDC_CST_ROUTER_SORTER_SCRAP_INFO, STATIC, i, j, 0);
// 			GridFont(IDC_CST_ROUTER_SORTER_SCRAP_INFO, i, j, 15);
// 		}
// 	}
// 	
// 	GridData(IDC_CST_ROUTER_SORTER_SCRAP_INFO, 0, 1, "No.");
// 	GridData(IDC_CST_ROUTER_SORTER_SCRAP_INFO, 0, 2, "Module ID");
// 	GridData(IDC_CST_ROUTER_SORTER_SCRAP_INFO, 0, 3, "Fail code");
// 	
// 	int cnt = st_work.strMdlRejectRouterSorter.GetCount();
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		strTemp.Format("%d", i + 1);
// 		GridData(IDC_CST_ROUTER_SORTER_SCRAP_INFO, i + 1, 1, strTemp);
// 		str_tmp[i] = st_work.strMdlRejectRouterSorter.GetAt(st_work.strMdlRejectRouterSorter.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_ROUTER_SORTER_SCRAP_INFO, i + 1, 2, str_tmp[i]);
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		str_tmp[i] = st_work.strMdlRejectCodeRouterSorter.GetAt(st_work.strMdlRejectCodeRouterSorter.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_ROUTER_SORTER_SCRAP_INFO, i + 1, 3, str_tmp[i]);
// 	}
// 	Grid = NULL;
// 	delete Grid;
}

void CDialog_Track_Out::OnTrackOut_Init_FCT_Handler_Scrap_Info()
{
// 	TSpread *Grid;
// 	CString str_pos;
// 	int row = st_work.strMdlRejectFCT.GetCount();
// 	int col = 3;
// 	int i, j;
// 	CString str_tmp[100], strTemp;
// 	
// 	Grid = (TSpread*)GetDlgItem(IDC_CST_FCT_HANDLER_SCRAP_INFO);
// 	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
// 	Grid -> SetBool(SSB_SHOWCOLHEADERS, TRUE);
// 	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
// 	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
// 	Grid -> SetBool(SSB_VERTSCROLLBAR, TRUE);
// 	Grid -> SetMaxRows(row);
// 	Grid -> SetMaxCols(col);
// 	
// 	for (i = 0; i <= row; i++)
// 	{
// 		for (j = 1; j <= col; j++)
// 		{
// 			Grid -> SetRowHeightInPixels(i, 20);
// 			
// 			if (j == 1)
// 			{
// 				Grid -> SetColWidthInPixels(j, 30);
// 			}
// 			else if (j == 2)
// 			{
// 				Grid -> SetColWidthInPixels(j, 170);
// 			}
// 			else
// 			{
// 				Grid -> SetColWidthInPixels(j, 90);
// 			}
// 			
// 			if (i == 0)
// 			{
// 				GridColor(IDC_CST_FCT_HANDLER_SCRAP_INFO, i, j, ORANGE_C, WHITE_C);
// 			}
// 			else
// 			{
// 				GridColor(IDC_CST_FCT_HANDLER_SCRAP_INFO, i, j, WHITE_C, BLACK_C);
// 			}
// 			
// 			GridControl(IDC_CST_FCT_HANDLER_SCRAP_INFO, STATIC, i, j, 0);
// 			GridFont(IDC_CST_FCT_HANDLER_SCRAP_INFO, i, j, 15);
// 		}
// 	}
// 	
// 	GridData(IDC_CST_FCT_HANDLER_SCRAP_INFO, 0, 1, "No.");
// 	GridData(IDC_CST_FCT_HANDLER_SCRAP_INFO, 0, 2, "Module ID");
// 	GridData(IDC_CST_FCT_HANDLER_SCRAP_INFO, 0, 3, "Fail code");
// 	
// 	int cnt = st_work.strMdlRejectFCT.GetCount();
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		strTemp.Format("%d", i + 1);
// 		GridData(IDC_CST_FCT_HANDLER_SCRAP_INFO, i + 1, 1, strTemp);
// 		str_tmp[i] = st_work.strMdlRejectFCT.GetAt(st_work.strMdlRejectFCT.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_FCT_HANDLER_SCRAP_INFO, i + 1, 2, str_tmp[i]);
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		str_tmp[i] = st_work.strMdlRejectCodeFCT.GetAt(st_work.strMdlRejectCodeFCT.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_FCT_HANDLER_SCRAP_INFO, i + 1, 3, str_tmp[i]);
// 	}
// 	Grid = NULL;
// 	delete Grid;
}

void CDialog_Track_Out::OnTrackOut_Init_PCT_Sorter_Scrap_Info()
{
// 	TSpread *Grid;
// 	CString str_pos;
// 	int row = st_work.strMdlRejectPCTSorter.GetCount();
// 	int col = 3;
// 	int i, j;
// 	CString str_tmp[100], strTemp;
// 	
// 	Grid = (TSpread*)GetDlgItem(IDC_CST_PCT_SORTER_SCRAP_INFO);
// 	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
// 	Grid -> SetBool(SSB_SHOWCOLHEADERS, TRUE);
// 	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
// 	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
// 	Grid -> SetBool(SSB_VERTSCROLLBAR, TRUE);
// 	Grid -> SetMaxRows(row);
// 	Grid -> SetMaxCols(col);
// 	
// 	for (i = 0; i <= row; i++)
// 	{
// 		for (j = 1; j <= col; j++)
// 		{
// 			Grid -> SetRowHeightInPixels(i, 20);
// 			
// 			if (j == 1)
// 			{
// 				Grid -> SetColWidthInPixels(j, 30);
// 			}
// 			else if (j == 2)
// 			{
// 				Grid -> SetColWidthInPixels(j, 170);
// 			}
// 			else
// 			{
// 				Grid -> SetColWidthInPixels(j, 90);
// 			}
// 			
// 			if (i == 0)
// 			{
// 				GridColor(IDC_CST_PCT_SORTER_SCRAP_INFO, i, j, ORANGE_C, WHITE_C);
// 			}
// 			else
// 			{
// 				GridColor(IDC_CST_PCT_SORTER_SCRAP_INFO, i, j, WHITE_C, BLACK_C);
// 			}
// 			
// 			GridControl(IDC_CST_PCT_SORTER_SCRAP_INFO, STATIC, i, j, 0);
// 			GridFont(IDC_CST_PCT_SORTER_SCRAP_INFO, i, j, 15);
// 		}
// 	}
// 	
// 	GridData(IDC_CST_PCT_SORTER_SCRAP_INFO, 0, 1, "No.");
// 	GridData(IDC_CST_PCT_SORTER_SCRAP_INFO, 0, 2, "Module ID");
// 	GridData(IDC_CST_PCT_SORTER_SCRAP_INFO, 0, 3, "Fail code");
// 	
// 	int cnt = st_work.strMdlRejectPCTSorter.GetCount();
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		strTemp.Format("%d", i + 1);
// 		GridData(IDC_CST_PCT_SORTER_SCRAP_INFO, i + 1, 1, strTemp);
// 		str_tmp[i] = st_work.strMdlRejectPCTSorter.GetAt(st_work.strMdlRejectPCTSorter.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_PCT_SORTER_SCRAP_INFO, i + 1, 2, str_tmp[i]);
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		str_tmp[i] = st_work.strMdlRejectCodePCTSorter.GetAt(st_work.strMdlRejectCodePCTSorter.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_PCT_SORTER_SCRAP_INFO, i + 1, 3, str_tmp[i]);
// 	}
// 	Grid = NULL;
// 	delete Grid;
}

void CDialog_Track_Out::OnTrackOut_Disp_Sorter_Scrap_Info()
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
// 	strTemp.Format("%d", st_work.nMdlFailCount[1][0]);
// 	GridData(IDC_CST_SORTER_SCRAP_INFO, 6, 2, strTemp);
// 
// 	strTemp.Format("%d", st_work.strMdlReject.GetCount());
// 	GridData(IDC_CST_SORTER_SCRAP_INFO, 7, 2, strTemp);

	GridData(IDC_CST_SORTER_SCRAP_INFO, 1, 2, st_NW.mstr_lot_id[1]); // 20130128
	
	strTemp.Format("%d", st_work.nMdlPassCount[0][0]);               // 20130128
	GridData(IDC_CST_SORTER_SCRAP_INFO, 2, 2, strTemp);
	
	strTemp.Format("%d", st_work.nMdlRejectCount[0][0]);				 // 20130128 확인해야함
	GridData(IDC_CST_SORTER_SCRAP_INFO, 3, 2, strTemp);              
	
	strTemp.Format("%d", st_work.strMdlReject.GetCount());           // 20130128 확인해야함
	GridData(IDC_CST_SORTER_SCRAP_INFO, 4, 2, strTemp);
}

void CDialog_Track_Out::OnTrackOut_Disp_Server_Scrap_Info()
{
	CString strTemp;
	
// 	GridData(IDC_CST_SERVER_SCRAP_INFO, 1, 2, st_handler.mstr_lot_name[0]);
// 	
// 	strTemp.Format("%d", st_work.n_lotdisply_qty);
// 	GridData(IDC_CST_SERVER_SCRAP_INFO, 2, 2, strTemp);
// 
// 	strTemp.Format("%d", st_work.nMdlPassCount[1][0]);
// 	GridData(IDC_CST_SERVER_SCRAP_INFO, 3, 2, strTemp);
// 	
// 	strTemp.Format("%d", st_work.strMdlReject.GetCount());
// 	GridData(IDC_CST_SERVER_SCRAP_INFO, 4, 2, strTemp);

	GridData(IDC_CST_SERVER_SCRAP_INFO, 1, 2, st_NW.mstr_lot_id[1]); // 20130128
	
	strTemp.Format("%d", st_work.nMdlInputCount[0][0]);
	GridData(IDC_CST_SERVER_SCRAP_INFO, 2, 2, strTemp);

	strTemp.Format("%d", st_work.nMdlPassCount[0][0]);
	GridData(IDC_CST_SERVER_SCRAP_INFO, 3, 2, strTemp);
	
	strTemp.Format("%d", st_work.strMdlReject.GetCount());  // 20130128 확인해야함
	GridData(IDC_CST_SERVER_SCRAP_INFO, 4, 2, strTemp);
}

void CDialog_Track_Out::OnTrackOut_Disp_Router_Sorter_Scrap_Info()
{
// 	int i;
// 	int cnt = st_work.strMdlRejectRouterSorter.GetCount();
// 	CString str_tmp[100];
// 	CString strTemp;
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		str_tmp[i] = st_work.strMdlRejectRouterSorter.GetAt(st_work.strMdlRejectRouterSorter.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_ROUTER_SORTER_SCRAP_INFO, i + 1, 2, str_tmp[i]);
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		str_tmp[i] = st_work.strMdlRejectCodeRouterSorter.GetAt(st_work.strMdlRejectCodeRouterSorter.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_ROUTER_SORTER_SCRAP_INFO, i + 1, 3, str_tmp[i]);
// 	}
// 
// 	strTemp.Format("Router sorter [ %d ]", cnt);
// 	m_group_router_sorter_scrap_info.SetText(strTemp);
}

void CDialog_Track_Out::OnTrackOut_Disp_FCT_Handler_Scrap_Info()
{
// 	int i;
// 	int cnt = st_work.strMdlRejectFCT.GetCount();
// 	CString str_tmp[100];
// 	CString strTemp;
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		str_tmp[i] = st_work.strMdlRejectFCT.GetAt(st_work.strMdlRejectFCT.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_FCT_HANDLER_SCRAP_INFO, i + 1, 2, str_tmp[i]);
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		str_tmp[i] = st_work.strMdlRejectCodeFCT.GetAt(st_work.strMdlRejectCodeFCT.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_FCT_HANDLER_SCRAP_INFO, i + 1, 3, str_tmp[i]);
// 	}
// 
// 	strTemp.Format("FCT Handler [ %d ]", cnt);
// 	m_group_fct_handler_scrap_info.SetText(strTemp);
}

void CDialog_Track_Out::OnTrackOut_Disp_PCT_Sorter_Scrap_Info()
{
// 	int i;
// 	int cnt = st_work.strMdlRejectPCTSorter.GetCount();
// 	CString str_tmp[100];
// 	CString strTemp;
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		str_tmp[i] = st_work.strMdlRejectPCTSorter.GetAt(st_work.strMdlRejectPCTSorter.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_PCT_SORTER_SCRAP_INFO, i + 1, 2, str_tmp[i]);
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		str_tmp[i] = st_work.strMdlRejectCodePCTSorter.GetAt(st_work.strMdlRejectCodePCTSorter.FindIndex(i));
// 	}
// 	
// 	for (i = 0; i < cnt; i++)
// 	{
// 		GridData(IDC_CST_PCT_SORTER_SCRAP_INFO, i + 1, 3, str_tmp[i]);
// 	}
// 
// 	strTemp.Format("PCT Sorter [ %d ]", cnt);
// 	m_group_pct_sorter_scrap_info.SetText(strTemp);
}

int CDialog_Track_Out::OnTrackOut_Server_Send()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	CString strTemp, strMsgTemp, strHeader, strBody;
	CString strEvent[10];
	char chvalue[100];

	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CST_SORTER_SCRAP_INFO);

	CDialog_Message  msg_dlg;

	Func.ThreadFunctionStepTrace(2, mn_ec_server_step);

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
		if (mn_track_out == CTL_YES)
		{
			st_NW.mn_invaild_function_flag = CTL_NO;
			st_sync.mstr_track_out_msg = "";
			st_sync.mstr_track_out = "READY";
			st_work.mstr_client_send_msg[NETWORK_ECSERVER] = Func.OnNetworkDataMake(NW_TRACK_OUT);
		}
		else if (mn_add_fail == CTL_YES)
		{
			st_NW.mn_invaild_function_flag = CTL_NO;
			st_sync.mstr_online_end = "READY";
			st_work.mstr_client_send_msg[NETWORK_ECSERVER] = Func.OnNetworkDataMake(NW_ONLINE_END);
		}

		st_work.mn_client_rpy[NETWORK_ECSERVER] = CTL_YES;
		
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
		else if (mn_track_out == CTL_YES)
		{
			if (st_sync.mstr_track_out == "PASS")
			{
				st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
				mn_ec_server_step  = 1400;
			}
			else if (st_sync.mstr_track_out == "FAIL")
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
					mn_ec_server_step  = 0;
				}
			}
		}
		else if (mn_add_fail == CTL_YES)
		{
			if (st_sync.mstr_online_end == "PASS")
			{
				st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
				mn_ec_server_step  = 400;
			}
			else if (st_sync.mstr_online_end == "FAIL")
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
					mn_ec_server_step  = 0;
				}
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
				mn_ec_server_step  = 0;
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

int CDialog_Track_Out::OnTrackOut_Server_Send_Scrap_Info()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	CString strTemp, strMsgTemp, strHeader, strBody;
	CString strEvent[10];

	Func.ThreadFunctionStepTrace(3, mn_scrap_info_step);

	switch (mn_scrap_info_step)
	{
	case 0:
		if (st_handler.mstr_lot_name[0] == "")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_scrap_info_step  = 1500;
			break;
		}

		if (st_work.mn_client_ready[NETWORK_ECSERVER] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
			{
				mn_scrap_info_step  = 200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_ECSERVER, 1);
				}
				
				ml_wait_time[0] = GetCurrentTime();
				mn_scrap_info_step  = 100;
			}
		}
		break;
		
	case 100:
		if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
		{
			mn_scrap_info_step  = 200;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				nFuncRet = CTLBD_RET_ERROR;
				st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
				mn_scrap_info_step  = 0;
			}
		}
		break;
		
	case 200:	
		st_work.mstr_client_send_msg[NETWORK_ECSERVER] = Func.OnNetworkDataMake(NW_LOT_DISPLAY);
		
		st_work.mn_client_rpy[NETWORK_ECSERVER] = CTL_YES;
		st_sync.mstr_lot_display = "READY";
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->SendMessage(CLIENT_SEND, NETWORK_ECSERVER, 0);
		}
		
		ml_retry_wait[0] = GetCurrentTime();
		mn_scrap_info_step  = 300;
		break;
		
	case 300:						// Buffer ready에 대한 Reply가 오면 Buffer를 다음 장비로 넣는다.
		if (st_sync.mstr_lot_display == "OK")
		{
			OnTrackOut_Display_Renew();
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_scrap_info_step  = 400;
		}
		else if (st_sync.mstr_lot_display == "NG")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_scrap_info_step  = 500;
		}
		else
		{
			ml_retry_wait[1] = GetCurrentTime();
			ml_retry_wait[2] = ml_retry_wait[1] - ml_retry_wait[0];
			
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			
			if (ml_retry_wait[2] > st_basic.mn_network_wait_time * 1000)		// 분으로 세팅하기 때문에 X 60... msec이기 때문에 X 1000
			{
				mn_scrap_info_step  = 0;
			}
		}
		break;

	case 400:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_scrap_info_step  = 1000;
		break;

	case 500:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_scrap_info_step  = 0;
		nFuncRet = CTLBD_RET_ERROR;
		break;

	case 1000:
		if (st_work.mn_client_ready[NETWORK_ECSERVER] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
			{
				mn_scrap_info_step  = 1200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_ECSERVER, 0);
				}
				
				ml_wait_time[0] = GetCurrentTime();
				mn_scrap_info_step  = 1100;
			}
		}
		break;
		
	case 1100:
		if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
		{
			mn_scrap_info_step  = 1200;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				nFuncRet = CTLBD_RET_ERROR;
				st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
				mn_scrap_info_step  = 1000;
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
		mn_scrap_info_step  = 1300;
		break;
		
	case 1300:						// Buffer ready에 대한 Reply가 오면 Buffer를 다음 장비로 넣는다.
		if (st_sync.mstr_scrap_info == "PASS")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_scrap_info_step  = 1400;
		}
		else if (st_sync.mstr_scrap_info == "FAIL")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_scrap_info_step  = 1500;
		}
		else
		{
			ml_retry_wait[1] = GetCurrentTime();
			ml_retry_wait[2] = ml_retry_wait[1] - ml_retry_wait[0];
			
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			
			if (ml_retry_wait[2] > st_basic.mn_network_wait_time * 1000)		// 분으로 세팅하기 때문에 X 60... msec이기 때문에 X 1000
			{
				mn_scrap_info_step  = 0;
			}
		}
		break;

	case 1400:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_scrap_info_step  = 0;
		nFuncRet = CTLBD_RET_GOOD;
		break;

	case 1500:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_scrap_info_step  = 0;
		nFuncRet = CTLBD_RET_ERROR;
		break;
	}

	return nFuncRet;
}

void CDialog_Track_Out::OnTrackOut_Display_Renew()
{
	OnTrackOut_Init_Sorter_Scrap_Info();
	OnTrackOut_Init_Server_Scrap_Info();
// 	OnTrackOut_Init_Router_Sorter_Scrap_Info();
// 	OnTrackOut_Init_FCT_Handler_Scrap_Info();
// 	OnTrackOut_Init_PCT_Sorter_Scrap_Info();

	OnTrackOut_Disp_Sorter_Scrap_Info();
	OnTrackOut_Disp_Server_Scrap_Info();
// 	OnTrackOut_Disp_Router_Sorter_Scrap_Info();
// 	OnTrackOut_Disp_FCT_Handler_Scrap_Info();
// 	OnTrackOut_Disp_PCT_Sorter_Scrap_Info();
}

void CDialog_Track_Out::OnOK() 
{
	// TODO: Add extra validation here
	
	StandardDialog::OnOK();
}

void CDialog_Track_Out::OnChkEdit() 
{
// 	mn_edit = !mn_edit;
// 
// 	OnTrackOut_Disp_Edit();
}

void CDialog_Track_Out::OnTrackOut_Disp_Edit()
{
// 	m_chk_edit.SetCheck(mn_edit);
}

int CDialog_Track_Out::OnTrackOut_Server_Send_Delete_Scrap()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	CString strTemp, strMsgTemp, strHeader, strBody;
	CString strEvent[10];

	Func.ThreadFunctionStepTrace(4, mn_delete_scrap_step);

	switch (mn_delete_scrap_step)
	{
	case 0:
		if (st_work.mn_client_ready[NETWORK_ECSERVER] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
			{
				mn_delete_scrap_step  = 200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_ECSERVER, 1);
				}
				
				ml_wait_time[0] = GetCurrentTime();
				mn_delete_scrap_step  = 100;
			}
		}
		break;
		
	case 100:
		if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
		{
			mn_delete_scrap_step  = 200;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				nFuncRet = CTLBD_RET_ERROR;
				st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
				mn_delete_scrap_step  = 0;
			}
		}
		break;
		
	case 200:	
		st_work.mstr_delete_lot_no = st_handler.mstr_lot_name[0];
		st_work.mstr_delete_serial = mstr_del_mdl_id;
		st_work.mstr_client_send_msg[NETWORK_ECSERVER] = Func.OnNetworkDataMake(NW_DELETE_SCRAP);
		
		st_work.mn_client_rpy[NETWORK_ECSERVER] = CTL_YES;
		st_sync.mstr_delete_scrap = "READY";
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->SendMessage(CLIENT_SEND, NETWORK_ECSERVER, 0);
		}
		
		ml_retry_wait[0] = GetCurrentTime();
		mn_delete_scrap_step  = 300;
		break;
		
	case 300:
		if (st_sync.mstr_delete_scrap == "PASS")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_delete_scrap_step  = 400;
		}
		else if (st_sync.mstr_delete_scrap == "FAIL")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_delete_scrap_step  = 500;
		}
		else
		{
			ml_retry_wait[1] = GetCurrentTime();
			ml_retry_wait[2] = ml_retry_wait[1] - ml_retry_wait[0];
			
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			
			if (ml_retry_wait[2] > st_basic.mn_network_wait_time * 1000)		// 분으로 세팅하기 때문에 X 60... msec이기 때문에 X 1000
			{
				mn_delete_scrap_step  = 0;
			}
		}
		break;

	case 400:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_delete_scrap_step  = 1000;
		break;

	case 500:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_delete_scrap_step  = 0;
		nFuncRet = CTLBD_RET_ERROR;
		break;

	case 1000:
		if (st_work.mn_client_ready[NETWORK_ECSERVER] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
			{
				mn_delete_scrap_step  = 1200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_ECSERVER, 0);
				}
				
				ml_wait_time[0] = GetCurrentTime();
				mn_delete_scrap_step  = 1100;
			}
		}
		break;
		
	case 1100:
		if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
		{
			mn_delete_scrap_step  = 1200;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				nFuncRet = CTLBD_RET_ERROR;
				st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
				mn_delete_scrap_step  = 1000;
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
		mn_delete_scrap_step  = 1300;
		break;
		
	case 1300:						// Buffer ready에 대한 Reply가 오면 Buffer를 다음 장비로 넣는다.
		if (st_sync.mstr_scrap_info == "PASS")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_delete_scrap_step  = 1400;
		}
		else if (st_sync.mstr_scrap_info == "FAIL")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_delete_scrap_step  = 1500;
		}
		else
		{
			ml_retry_wait[1] = GetCurrentTime();
			ml_retry_wait[2] = ml_retry_wait[1] - ml_retry_wait[0];
			
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			
			if (ml_retry_wait[2] > st_basic.mn_network_wait_time * 1000)		// 분으로 세팅하기 때문에 X 60... msec이기 때문에 X 1000
			{
				mn_delete_scrap_step  = 0;
			}
		}
		break;

	case 1400:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_delete_scrap_step  = 0;
		nFuncRet = CTLBD_RET_GOOD;
		break;

	case 1500:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_delete_scrap_step  = 0;
		nFuncRet = CTLBD_RET_ERROR;
		break;
	}

	return nFuncRet;
}

BOOL CDialog_Track_Out::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
		
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	
	return StandardDialog::PreTranslateMessage(pMsg);
}
