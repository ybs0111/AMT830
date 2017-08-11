// Dialog_Work_Wait_Time.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Work_Wait_Time.h"

#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyPad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Wait_Time dialog


CDialog_Work_Wait_Time::CDialog_Work_Wait_Time(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Work_Wait_Time::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Work_Wait_Time)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Work_Wait_Time::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Work_Wait_Time)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BTN_WAIT_APPLY, m_btn_wait_apply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Work_Wait_Time, CDialog)
	//{{AFX_MSG_MAP(CDialog_Work_Wait_Time)
		// NOTE: the ClassWizard will add message map macros here
	ON_WM_DESTROY()
	//ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_WAIT_APPLY, OnBtnWaitApply)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK, OnCellClick)
	ON_MESSAGE(WM_WAITTIME_APPLY, OnWaitTime_Info_Apply)  // DATA를 APPLY 시키기 위한 사용자 정의 메시지 선언
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Wait_Time message handlers

BOOL CDialog_Work_Wait_Time::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// ************************************************************************** 
	// 컨트롤에 설정할 폰트 생성한다                                              
	// ************************************************************************** 
	mp_wait_time_font = NULL;
	mp_wait_time_font = new CFont;
	mp_wait_time_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// ************************************************************************** 
	OnWaitTime_Data_Display();

	mn_enable = TRUE;
	st_handler.cwnd_waittime = this;  // Wait Time 화면에 대한 핸들 정보 설정 
	return TRUE;

}

void CDialog_Work_Wait_Time::OnDestroy() 
{
	int mn_response;  // 대화 상자에 대한 리턴 값 저장 변수
	int Ret;

	CDialog_Select select_dlg;

	//프로그램을 빠져나가면서 Save 하지 않고, Form이 Destroy되는것이라면 Setting을 포기한다는 말이다.
	//비교도 하지 말자.
	if (st_handler.b_program_exit == false)
	{
		Ret = OnWaitTime_Data_Comparison();

		if (Ret == RET_ERROR)
		{
			st_msg.mstr_confirm_msg = _T("Changed Data! Apply?");
			
			mn_response = select_dlg.DoModal();
			
			if (mn_response == IDOK)
			{
				OnWaitTime_Data_Apply();

				OnWaitTime_LogFile_Create();
			}
			else if (mn_response == IDCANCEL)
			{
				OnWaitTime_Data_Recovery();

				OnWaitTime_Data_Display();

				OnWaitTime_Data_Apply();
			}	
		}
	}

	// ************************************************************************** 
	// 생성한 폰트 정보 삭제한다                                                  
	// ************************************************************************** 
	delete mp_wait_time_font;
	mp_wait_time_font = NULL;
	// ************************************************************************** 

	st_handler.cwnd_waittime = NULL;	// Wait Time 화면에 대한 핸들 정보 저장 변수 초기화 
	
	CDialog::OnDestroy();
}

void CDialog_Work_Wait_Time::OnWaitTime_Data_Set()
{
	int i;

	for(i=0; i<mn_wait_time_cnt; i++)
	{
		mn_on_wait[i][1]		= st_wait.n_on_wait_time[i];
		mn_off_wait[i][1]		= st_wait.n_off_wait_time[i];
		mn_limit[i][1]			= st_wait.n_limit_wait_time[i];
	}
	
	OnWaitTime_Data_BackUp();
}

void CDialog_Work_Wait_Time::OnWaitTime_Data_Recovery()
{
	int i;
	
	for (i = 0; i < mn_wait_time_cnt; i ++)
	{
		mn_on_wait[i][1] = mn_on_wait[i][0];
		mn_off_wait[i][1] = mn_off_wait[i][0];
		mn_limit[i][1] = mn_limit[i][0];
	}
}

void CDialog_Work_Wait_Time::OnWaitTime_Data_Display()
{
	OnWaitTime_Init_Time();
}

int CDialog_Work_Wait_Time::OnWaitTime_Data_Comparison()
{
	int Ret = CTLBD_RET_GOOD;
	int i;
	
	for (i = 0; i < mn_wait_time_cnt; i ++)
	{
		if (mn_on_wait[i][0] != mn_on_wait[i][1])			Ret = CTLBD_RET_ERROR;
		if (mn_off_wait[i][0] != mn_off_wait[i][1])			Ret = CTLBD_RET_ERROR;
		if (mn_limit[i][0] != mn_limit[i][1])				Ret = CTLBD_RET_ERROR;
	}

	return Ret;
}

void CDialog_Work_Wait_Time::OnWaitTime_Data_BackUp()
{
	int i;

	for (i = 0; i < mn_wait_time_cnt; i ++)
	{
		mn_on_wait[i][0] = mn_on_wait[i][1];
		mn_off_wait[i][0] = mn_off_wait[i][1];
		mn_limit[i][0] = mn_limit[i][1];
	}
}

void CDialog_Work_Wait_Time::OnWaitTime_Data_Apply()
{
	int i;
//	int num;

	for (i = 0; i < mn_wait_time_cnt; i++)
	{
		st_wait.n_on_wait_time[i] = mn_on_wait[i][1];
		st_wait.n_off_wait_time[i] = mn_off_wait[i][1];
		st_wait.n_limit_wait_time[i] = mn_limit[i][1];
	}

// 	num = 0;
// 	st_wait.i_picker_updn_w[0]						= mn_on_wait[num][1];
// 	st_wait.i_picker_updn_w[1]						= mn_off_wait[num][1];
// 	st_wait.i_picker_l								= mn_limit[num][1];
// 	
// 	num = 1;
// 	st_wait.i_finger_onoff_w[0]						= mn_on_wait[num][1];
// 	st_wait.i_finger_onoff_w[1]						= mn_off_wait[num][1];
// 	st_wait.i_finger_l								= mn_limit[num][1];
// 	
// 	num = 2;
// 	st_wait.i_module_clamp_w[0]						= mn_on_wait[num][1];
// 	st_wait.i_module_clamp_w[1]						= mn_off_wait[num][1];
// 	st_wait.i_module_l								= mn_limit[num][1];
// 	
// 	num = 3;
// 	st_wait.i_stacker_guide_w[0]					= mn_on_wait[num][1];
// 	st_wait.i_stacker_guide_w[1]					= mn_off_wait[num][1];
// 	st_wait.i_stacker_guide_l						= mn_limit[num][1];
// 	
// 	num = 4;
// 	st_wait.i_stacker_f_b_w[0]						= mn_on_wait[num][1];
// 	st_wait.i_stacker_f_b_w[1]						= mn_off_wait[num][1];
// 	st_wait.i_stacker_f_b_l							= mn_limit[num][1];
// 	
// 	num = 5;
// 	st_wait.i_stacker_rail_push_back_w[0]			= mn_on_wait[num][1];
// 	st_wait.i_stacker_rail_push_back_w[1]			= mn_off_wait[num][1];
// 	st_wait.i_stacker_rail_push_back_l				= mn_limit[num][1];
// 	
// 	num = 6;
// 	st_wait.i_stacker_updn_w[0]						= mn_on_wait[num][1];
// 	st_wait.i_stacker_updn_w[1]						= mn_off_wait[num][1];
// 	st_wait.i_stacker_updn_l						= mn_limit[num][1];
// 	
// 	num = 7;
// 	st_wait.i_hs_stacker_guide_w[0]					= mn_on_wait[num][1];
// 	st_wait.i_hs_stacker_guide_w[1]					= mn_off_wait[num][1];
// 	st_wait.i_hs_stacker_guide_l					= mn_limit[num][1];
// 	
// 	num = 8;;
// 	st_wait.i_stacker_clip_w[0]						= mn_on_wait[num][1];
// 	st_wait.i_stacker_clip_w[1]						= mn_off_wait[num][1];
// 	st_wait.i_stacker_clip_l						= mn_limit[num][1];
// 	
// 	num = 9;
// 	st_wait.i_label_onoff_w[0]						= mn_on_wait[num][1];
// 	st_wait.i_label_onoff_w[1]						= mn_off_wait[num][1];
// 	st_wait.i_label_l								= mn_limit[num][1];
// 	
// 	num = 10;
// 	st_wait.i_label_vacuum							= mn_limit[num][1];
// 	
// 	num = 11;
// 	st_wait.i_label_eject							= mn_limit[num][1];


}

// ****************************************************************************** 
//  화면 정보 갱신 요청 메시지                                                    
// ****************************************************************************** 
LRESULT CDialog_Work_Wait_Time::OnWaitTime_Info_Apply(WPARAM wParam, LPARAM lParam)  
{
	OnWaitTime_Data_Apply();		// 화면 셋팅 정보 전역 변수에 전송 함수
	
	OnWaitTime_LogFile_Create();
	
	OnWaitTime_Data_BackUp();
	
	return 0;
}

void CDialog_Work_Wait_Time::OnWaitTime_LogFile_Create()
{
	CString str_msg;
	CString str_data;
	CString str_old_data;
	CString str_io_name;
	int i;
	
	for (i = 0; i < mn_wait_time_cnt; i++)
	{
		str_io_name = mstr_wait_time_name[i];

		if (mn_on_wait[i][1] != mn_on_wait[i][0])
		{
			str_data.Format("%d", mn_on_wait[i][1]);
			str_old_data.Format("%d", mn_on_wait[i][0]);
			str_msg.Format("[Time Set] %s On Wait Time was changed by %s To %s", str_io_name, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}

		if (mn_off_wait[i][1] != mn_off_wait[i][0])
		{
			str_data.Format("%d", mn_off_wait[i][1]);
			str_old_data.Format("%d", mn_off_wait[i][0]);
			str_msg.Format("[Time Set] %s Off Wait Time was changed by %s To %s", str_io_name, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}

		if (mn_limit[i][1] != mn_limit[i][0])
		{
			str_data.Format("%d", mn_limit[i][1]);
			str_old_data.Format("%d", mn_limit[i][0]);
			str_msg.Format("[Time Set] %s Limit Time was changed by %s To %s", str_io_name, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
	}
}

void CDialog_Work_Wait_Time::OnWaitTime_Init_Time()
{
	int i, j;
	CString str_tmp;
	
	m_grid_time = (TSpread*)GetDlgItem(IDC_CUSTOM_TIME);
	
	mp_grid.GridFileOpen(m_grid_time, st_path.mstr_wait_time_map, "sheet1");
	
	mn_wait_time_cnt = 0;
	mn_wait_time_cnt = atoi(mp_grid.GridCellText(m_grid_time, 1, 3));
	
	if(mn_wait_time_cnt == 0) return;
	
	for(i=0; i<mn_wait_time_cnt; i++)
	{
		mn_on_used[i]				= atoi(mp_grid.GridCellText(m_grid_time, i+4, 4));
		mn_off_used[i]				= atoi(mp_grid.GridCellText(m_grid_time, i+4, 5));
		mn_limit_used[i]			= atoi(mp_grid.GridCellText(m_grid_time, i+4, 6));
		mstr_wait_time_name[i]		= mp_grid.GridCellText(m_grid_time, i+4, 2);
	}
	
	OnWaitTime_Data_Set();
	
	mp_grid.GridReset(m_grid_time);
	// 대문자 
	mp_grid.GridCellSelectDisplay(m_grid_time, TRUE);
	mp_grid.GridRowHeader(m_grid_time, FALSE);
	mp_grid.GridColHeader(m_grid_time, TRUE);
	mp_grid.GridHorizontal(m_grid_time, FALSE);
	mp_grid.GridVertical(m_grid_time, TRUE);
	mp_grid.GridAutoSize(m_grid_time, FALSE);
	
	mp_grid.GridCellRows(m_grid_time, mn_wait_time_cnt);
	mp_grid.GridCellCols(m_grid_time, 5);


	for(i=0; i<mn_wait_time_cnt+1; i++)
	{
		mp_grid.GridCellHeight_L(m_grid_time, i, (double)30);
		
		if(i == 0)
		{
			for(j=0; j<5; j++)
			{
				switch(j)
				{
					case 0:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, (double)5);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, "No.");
						break;

					case 1:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, (double)40);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, "Name");
						break;

					case 2:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, (double)20);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, "On Time (ms)");
						break;

					case 3:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, (double)20);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, "Off Time (ms)");
						break;

					case 4:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, (double)20);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, "Limit Time (ms)");
						break;
				}

				mp_grid.GridCellControlStatic(m_grid_time, i, j+1);
			}
		}
		else
		{
			for(j=0; j<5; j++)
			{
				switch(j)
				{
					case 0:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, 5);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 14);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						str_tmp.Format("%d", i);
						mp_grid.GridCellText(m_grid_time, i, j+1, str_tmp);
						break;

					case 1:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, 40);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 14);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLACK_L, YELLOW_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, mstr_wait_time_name[i-1]);
						break;

					case 2:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, 20);
						
						if(mn_on_used[i-1] == CTL_YES)
						{
							mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
							mp_grid.GridCellColor(m_grid_time, i, j+1, GREEN_L, BLACK_C);
							str_tmp.Format("%d", mn_on_wait[i-1][1]);
							mp_grid.GridCellText(m_grid_time, i, j+1, str_tmp);
						}
						else
						{
							mp_grid.GridCellColor(m_grid_time, i, j+1, DSERBLE_BC, YELLOW_C);
							mp_grid.GridCellText(m_grid_time, i, j+1, "-");
						}
						break;

					case 3:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, 20);

						if(mn_off_used[i-1] == CTL_YES)
						{
							mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
							mp_grid.GridCellColor(m_grid_time, i, j+1, RED_L, BLACK_C);
							str_tmp.Format("%d", mn_off_wait[i-1][1]);
							mp_grid.GridCellText(m_grid_time, i, j+1, str_tmp);
						}
						else
						{
							mp_grid.GridCellColor(m_grid_time, i, j+1, DSERBLE_BC, YELLOW_C);
							mp_grid.GridCellText(m_grid_time, i, j+1, "-");
						}
						break;

					case 4:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, 20);

						if(mn_limit_used[i-1] == CTL_YES)
						{
							mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
							mp_grid.GridCellColor(m_grid_time, i, j+1, YELLOW_L, BLACK_C);
							str_tmp.Format("%d", mn_limit[i-1][1]);
							mp_grid.GridCellText(m_grid_time, i, j+1, str_tmp);
						}
						else
						{
							mp_grid.GridCellColor(m_grid_time, i, j+1, DSERBLE_BC, YELLOW_C);
							mp_grid.GridCellText(m_grid_time, i, j+1, "-");
						}
						break;
				}
	
				mp_grid.GridCellControlStatic(m_grid_time, i, j+1);
			}
		}
	}
}

void CDialog_Work_Wait_Time::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);

	Grid = NULL;
	delete Grid;
}

void CDialog_Work_Wait_Time::GridControl(UINT nID, int type, int row, int col, int pos)
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

void CDialog_Work_Wait_Time::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CDialog_Work_Wait_Time::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CDialog_Work_Wait_Time::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}

void CDialog_Work_Wait_Time::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	int nId = -1;
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	
	if (mn_enable != TRUE)					return;
	if (lpcc->Col == 1 || lpcc->Col == 2)	return;

	switch (wParam)
	{
	case IDC_CUSTOM_TIME:
		nId = 0;
		break;	
	}
	
	if (nId > -1)
	{
		if (lpcc->Col == 3)
		{
			if (mn_on_used[lpcc->Row-1] == CTL_NO) return;
		}
		else if (lpcc->Col == 4)
		{
			if (mn_off_used[lpcc->Row-1] == CTL_NO) return;
		}
		else if (lpcc->Col == 5)
		{
			if (mn_limit_used[lpcc->Row-1] == CTL_NO) return;
		}

		OnChangeValue(nId, lpcc->Row, lpcc->Col);
	}
}

void CDialog_Work_Wait_Time::OnChangeValue(int nid, int nrow, int ncol)
{
	TSpread *Grid; 
	CString strTmp;
	char chvalue[100];
	char chText[100];
	CString strText;
	CString strText2;
	int nValue, n_response, i;

	CDialog_KeyPad pad_dlg;
	
	CString str_temp;	// 저장할 정보 임시 저장 변수 

	if (nid == 0)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_TIME);
		
		Grid->GetValue(ncol, nrow, chvalue);
		nValue = atoi(chvalue);
		strTmp.Format("%d", nValue);
		
		Grid->GetValue(2, nrow, chText);
		strText = chText;

		Grid->GetValue(ncol, 0, chText);
		strText2 = chText;

		if (nrow == 1)		// 전체 선택.
		{
			st_msg.mstr_keypad_msg.Format("[TIME SET] All Contents, %s set", strText2);
		}
		else
		{
			st_msg.mstr_keypad_msg.Format("[TIME SET] %s, %s set", strText, strText2);
		}
		
		st_msg.mstr_keypad_val = strTmp;
		
		st_msg.mstr_pad_high_limit = "99999";
		st_msg.mstr_pad_low_limit = "0";
		
		st_msg.mn_dot_use = CTL_NO;
		pad_dlg.m_point = CTL_YES;
		
		n_response = pad_dlg.DoModal();
		
		if (n_response == IDOK)
		{
			strTmp = st_msg.mstr_keypad_val;
			
			if (ncol == 1)
			{
			}
			else if (ncol == 3)		// On Wait
			{
				if (nrow == 0)		// 전체 선택.
				{
					for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
					{	
						if (mn_on_used[i] == CTL_NO) continue;

						mn_on_wait[i][1] = atoi(strTmp);
						strTmp.Format("%d", mn_on_wait[i][1]);

						Grid->SetValue(ncol, i + 1, strTmp);
					}
					return;
				}
				else
				{
					mn_on_wait[nrow-1][1] = atoi(strTmp);
					strTmp.Format("%d", mn_on_wait[nrow-1][1]);
				}
			}
			else if (ncol == 4)		// Off Wait
			{
				if (nrow == 0)		// 전체 선택.
				{
					for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
					{	
						if (mn_off_used[i] == CTL_NO) continue;
						mn_off_wait[i][1] = atoi(strTmp);
						strTmp.Format("%d", mn_off_wait[i][1]);
						
						Grid->SetValue(ncol, i + 1, strTmp);
					}
					return;
				}
				else
				{
					mn_off_wait[nrow-1][1] = atoi(strTmp);
					strTmp.Format("%d", mn_off_wait[nrow-1][1]);
				}
			}
			else if (ncol == 5)		// Limit
			{
				if (nrow == 0)		// 전체 선택.
				{
					for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
					{	
						if (mn_limit_used[i] == CTL_NO) continue;
						mn_limit[i][1] = atoi(strTmp);
						strTmp.Format("%d", mn_limit[i][1]);
						
						Grid->SetValue(ncol, i + 1, strTmp);
					}
					return;
				}
				else
				{
					mn_limit[nrow-1][1] = atoi(strTmp);
					strTmp.Format("%d", mn_limit[nrow-1][1]);
				}
			}

			Grid->SetValue(ncol, nrow, strTmp);
		}
		else if (n_response == IDCANCEL)
		{
			
		}
	}
}

void CDialog_Work_Wait_Time::On_Controls_Enable(int nState)
{
	mn_enable = nState;
	m_btn_wait_apply.EnableWindow(nState);
}	

void CDialog_Work_Wait_Time::OnBtnWaitApply() 
{
	int n_response;  // 대화 상자 리턴 플래그
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Wait Time : Setting Data Apply?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnWaitTime_Data_Apply();			// 화면 셋팅 정보 전역 변수에 전송 함수
		
		OnWaitTime_LogFile_Create();
		
		OnWaitTime_Data_BackUp();

		mcls_waittime.OnWaitTime_Data_Save();			// 전역 변수에 저장된 Wait Time 셋팅 정보를 파일에 저장하는 함수
		mcls_waittime.OnWaitTime_Data_Load();			// 전역 변수에 저장된 Wait Time 셋팅 정보를 파일에 저장하는 함수

	}
	else if (n_response == IDCANCEL)
	{
		
	}
}