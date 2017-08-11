// Dialog_Motor_Axis_2.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Motor_Axis_2.h"
#include "Public_Function.h"
#include "Dialog_KeyPad.h"
#include "Cmmsdk.h"

#include "CtlBd_Function.h"
#include "CtlBd_Library.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Axis_2 dialog
#define TM_STATUS_CHECK		100
#define TM_POSITION_CHECK	200
#define TM_HOME_CHECK		300
#define TM_MOVE_CHECK		400
#define AXIS_NUM			1
CDialog_Motor_Axis_2::CDialog_Motor_Axis_2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Motor_Axis_2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Motor_Axis_2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_n_move_mode = 0;
	COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_EN, cmFALSE);
	COMI.Set_Motor_IO_Property(M_HS_F_STACKER_2, cmSD_EN, cmFALSE);
	COMI.Set_Motor_IO_Property(M_HS_B_STACKER_2, cmSD_EN, cmFALSE);
	COMI.Set_Motor_IO_Property(M_ULDM_STACKER_2, cmSD_EN, cmFALSE);
}


void CDialog_Motor_Axis_2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Motor_Axis_2)
	DDX_Control(pDX, IDC_BTN_APPLY, m_btn_apply);
	DDX_Control(pDX, IDC_GROUP_AXIS_INFO, m_group_axis_info);
	DDX_Control(pDX, IDC_GROUP_MOTOR, m_group_motor);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btn_right);
	DDX_Control(pDX, IDC_BTN_LEFT, m_btn_left);
	DDX_Control(pDX, IDC_GROUP_MOVE, m_group_move);
	DDX_Control(pDX, IDC_BTN_HOME, m_btn_home);
	DDX_Control(pDX, IDC_BTN_EMG, m_btn_emg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Motor_Axis_2, CDialog)
	//{{AFX_MSG_MAP(CDialog_Motor_Axis_2)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_EMG, OnBtnEmg)
	ON_BN_CLICKED(IDC_BTN_HOME, OnBtnHome)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
	ON_MESSAGE(WMU_CHILD_LBUTTON_DOWN, OnUserLButtonDown)
	ON_MESSAGE(WMU_CHILD_LBUTTON_UP, OnUserLButtonUp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Axis_2 message handlers
/*
void CDialog_Motor_Axis_2::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int nResponse;

	switch(nIDEvent)
	{
		case TM_STATUS_CHECK:
			MotorStatusCheck();
			break;

		case TM_POSITION_CHECK:
			MotorPositionCheck();
			break;

		case TM_HOME_CHECK:
			nResponse = MotorHomingExcution();		// 모터 HOMING 처리 함수 //
			
			if(nResponse == CTLBD_RET_GOOD)
			{
				KillTimer(TM_HOME_CHECK) ;  // 타이머 삭제 //
				if(st_handler.mn_home_state[m_n_axis] != TRUE)
				{
					st_handler.mn_home_state[m_n_axis] = TRUE;
				}
				
				m_n_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화 //
				
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================

				OnButtonControl(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
			}
			else if(nResponse == CTLBD_RET_ERROR)
			{
				KillTimer(TM_HOME_CHECK) ;  // 타이머 삭제 //
				
				m_n_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화 //
				
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================
				OnButtonControl(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
			}
			break;

		case TM_MOVE_CHECK:
			nResponse = MotorMovingExcution();		// 모터 HOMING 처리 함수 //
			
			if(nResponse == CTLBD_RET_GOOD)
			{
				KillTimer(TM_MOVE_CHECK) ;  // 타이머 삭제 //
		
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================
				
				OnButtonControl(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
				if(m_n_position_move == CTL_YES)
				{
					m_n_position_move = CTL_NO;
					MotorCompletion(m_n_cmd_row, m_n_cmd_col);
				}
				m_n_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화 //
			}
			else if(nResponse == CTLBD_RET_ERROR)
			{
				KillTimer(TM_MOVE_CHECK) ;  // 타이머 삭제 //
				
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================
				
				OnButtonControl(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
				if(m_n_position_move == CTL_YES)
				{
					m_n_position_move = CTL_NO;
					MotorCompletion(m_n_cmd_row, m_n_cmd_col);
				}
				m_n_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화 //
			}
			break;
	}
	CDialog::OnTimer(nIDEvent);
}
*/
BOOL CDialog_Motor_Axis_2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	
	m_d_data = 10;
	Data_Init();
	Init_Grid_Pos();
	Init_Grid_Info();
	Init_Label();
	Init_Group();
	Init_Digit();
	Init_Radio();
	Init_Button();

	if(COMI.mn_motorbd_init_end)
	{
		SetTimer(TM_STATUS_CHECK, 100, NULL);
		SetTimer(TM_POSITION_CHECK, 100, NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Motor_Axis_2::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_motor.SetFont(main_font);
	m_group_motor.SetCatptionTextColor(RGB(145,25,0));
	m_group_motor.SetFontBold(TRUE);
	m_group_motor.SetWindowText(m_str_axis_name);
		
	m_group_move.SetFont(main_font);
	m_group_move.SetCatptionTextColor(RGB(145,25,0));
	m_group_move.SetFontBold(TRUE);
	m_group_move.SetWindowText("Move");

	m_group_axis_info.SetFont(main_font);
	m_group_axis_info.SetCatptionTextColor(RGB(145,25,0));
	m_group_axis_info.SetFontBold(TRUE);
	m_group_axis_info.SetWindowText("Position Information");
}

void CDialog_Motor_Axis_2::Init_Button()
{
	short	shBtnColor = 30;
	CString strTmp;

	if (m_str_axis_name == "Loader_Tray_Transfer_Z")
	{
		m_btn_left.SetIcon(IDI_UP);
	}
///////////////////////// 20120716
	else if (m_str_axis_name == "Vision1_Y")// || m_str_axis_name == "Loader_Transfer_X")
	{
		m_btn_left.SetIcon(IDI_UP);
	}
	else if (m_str_axis_name == "W_Buffer_2")// || m_str_axis_name == "Loader_Transfer_X")
	{
		m_btn_left.SetIcon(IDI_LEFT);
	}
	else if (m_str_axis_name == "M_Clamp_Rbt_Glipper")// || m_str_axis_name == "Loader_Transfer_X")
	{
		m_btn_left.SetIcon(IDI_WIDTH);
	}

/////////////////////////

	else
	{
		m_btn_left.SetIcon(IDI_DN);
	}
	m_btn_left.SetTag(IDC_BTN_LEFT);
	m_btn_left.SetFont(m_p_font);
//	m_btn_left.SetIcon(IDI_DN);
	m_btn_left.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_left.SetButtonColor(1);
	
	if (m_str_axis_name == "Loader_Tray_Transfer_Z")
	{
		m_btn_right.SetIcon(IDI_DN);
	}
///////////////////////// 20120716
	else if (m_str_axis_name == "Vision1_Y")// || m_str_axis_name == "Loader_Transfer_X")
	{
		m_btn_right.SetIcon(IDI_DN);
	}
	else if (m_str_axis_name == "W_Buffer_2")// || m_str_axis_name == "Loader_Transfer_X")
	{
		m_btn_right.SetIcon(IDI_RIGHT);
	}
	else if (m_str_axis_name == "M_Clamp_Rbt_Glipper")// || m_str_axis_name == "Loader_Transfer_X")
	{
		m_btn_right.SetIcon(IDI_UNWIDTH);
	}

/////////////////////////

	else
	{
		m_btn_right.SetIcon(IDI_UP);
	}
	m_btn_right.SetTag(IDC_BTN_RIGHT);
	m_btn_right.SetFont(m_p_font);
//	m_btn_right.SetIcon(IDI_UP);
	m_btn_right.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_right.SetButtonColor(1);
	
	m_btn_home.SetTag(IDC_BTN_HOME);
	m_btn_home.SetFont(m_p_font);
	m_btn_home.SetIcon(IDI_HOME);
	m_btn_home.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_home.SetButtonColor(1);
	
	m_btn_emg.SetFont(m_p_font);
	m_btn_emg.SetIcon(IDI_EMG_STOP);
	m_btn_emg.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_emg.SetButtonColor(1);
	strTmp.Format("%s Emergency Step", m_str_axis_name);
	m_btn_emg.SetTooltipText(strTmp);
}

void CDialog_Motor_Axis_2::Init_Label()
{

}

void CDialog_Motor_Axis_2::Init_Radio()
{

}

void CDialog_Motor_Axis_2::Init_Digit()
{

}

void CDialog_Motor_Axis_2::Init_Grid_Pos()
{
	int   i, j;
	CString str_tmp;
	m_grid_motor_pos = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_POS);

	m_p_grid.GridReset(m_grid_motor_pos);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_motor_pos, TRUE);
	
	m_p_grid.GridRowHeader(m_grid_motor_pos, FALSE);
	m_p_grid.GridColHeader(m_grid_motor_pos, TRUE);
	m_p_grid.GridHorizontal(m_grid_motor_pos, FALSE);
	m_p_grid.GridVertical(m_grid_motor_pos, TRUE);
	m_p_grid.GridAutoSize(m_grid_motor_pos, FALSE);
	m_p_grid.GridAutoSize(m_grid_motor_pos, FALSE);
	m_p_grid.GridCellRows(m_grid_motor_pos, m_n_position_num);
	m_p_grid.GridCellCols(m_grid_motor_pos, 4);
	
	for(i=0; i<m_n_position_num+1; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_motor_pos, i, 30);
		for(j=0; j<4; j++)
		{
			
			switch(j)
			{
			case 0:
				m_p_grid.GridCellWidth_L(m_grid_motor_pos, j+1, 25);
				break;
			case 1:
				m_p_grid.GridCellWidth_L(m_grid_motor_pos, j+1, 15);
				break;
			case 2:
				m_p_grid.GridCellWidth_L(m_grid_motor_pos, j+1, 11);
				break;
			case 3:
				m_p_grid.GridCellWidth_L(m_grid_motor_pos, j+1, 11);
				break;
			}
			m_p_grid.GridCellControlStatic(m_grid_motor_pos, i, j+1);
		}
	}
	
	m_p_grid.GridCellFont(m_grid_motor_pos, 0, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_pos, 0, 1, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_motor_pos, 0, 1, "-");
	
	m_p_grid.GridCellFont(m_grid_motor_pos, 0, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_pos, 0, 2, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_motor_pos, 0, 2, "Positon");
	
	m_p_grid.GridCellFont(m_grid_motor_pos, 0, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_pos, 0, 3, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_motor_pos, 0, 3, "Move");
	
	m_p_grid.GridCellFont(m_grid_motor_pos, 0, 4, "MS Sans Serif", 15);
	m_p_grid.GridCellColor(m_grid_motor_pos, 0, 4, BLACK_C, YELLOW_C);
	m_p_grid.GridCellText(m_grid_motor_pos, 0, 4, "Read");
	
	for(i=0; i<m_n_position_num; i++)
	{
		m_p_grid.GridCellColor(m_grid_motor_pos, i+1, 1, BLACK_C, YELLOW_C);
		m_p_grid.GridCellText(m_grid_motor_pos, i+1, 1, m_str_position_name[i]);
		
		m_p_grid.GridCellFont(m_grid_motor_pos, i+1, 2, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_motor_pos, i+1, 2, GREEN_L, BLACK_C);
		str_tmp.Format("%.3f", m_d_position[i][1]);
		m_p_grid.GridCellText(m_grid_motor_pos, i+1, 2, str_tmp);
		
		m_p_grid.GridCellFont(m_grid_motor_pos, i+1, 3, "MS Sans Serif", 15);
		m_p_grid.GridCellColor(m_grid_motor_pos, i+1, 3, TEXT_BC, BLUE_C);
		m_p_grid.GridCellText(m_grid_motor_pos, i+1, 3, "Go.");
		
		m_p_grid.GridCellFont(m_grid_motor_pos, i+1, 4, "MS Sans Serif", 15);
		m_p_grid.GridCellColor(m_grid_motor_pos, i+1, 4, TEXT_BC, RED_C);
		m_p_grid.GridCellText(m_grid_motor_pos, i+1, 4, "Read.");
	}
}

void CDialog_Motor_Axis_2::Data_Apply()
{
	int i;
	
	for(i=0; i<m_n_position_num; i++)
	{
		st_motor[m_n_axis].md_pos[i]		= m_d_position[i][1];
	}
}

void CDialog_Motor_Axis_2::Data_Backup()
{
	int i;
	
	for(i=0; i<m_n_position_num; i++)
	{
		m_d_position[i][0]	= m_d_position[i][1];
	}
}

int CDialog_Motor_Axis_2::Data_Comparison()
{
	int i;
	int Ret = CTLBD_RET_GOOD;
	
	for(i=0; i<m_n_position_num; i++)
	{
		if(m_d_position[i][0] != m_d_position[i][1])
		{
			return CTLBD_RET_ERROR;
		}
	}
	
	return CTLBD_RET_GOOD;
}

void CDialog_Motor_Axis_2::Data_HistoryLog()
{
	int i;
	CString str_msg;
	
	for(i=0; i<m_n_position_num; i++)
	{
		if(m_d_position[i][0] != m_d_position[i][1])
		{
			str_msg.Format( "[%s] Axis Change %.3f -> %.3f", m_str_axis_name, m_d_position[i][0], m_d_position[i][1]);
			Func.On_LogFile_Add(0, str_msg);
		}
	}
}

void CDialog_Motor_Axis_2::Data_Recovery()
{
	int i;
	
	for(i=0; i<m_n_position_num; i++)
	{
		m_d_position[i][1]	= m_d_position[i][0];
	}

}

void CDialog_Motor_Axis_2::Data_Init()
{
	int i;
	
	m_n_axis			= st_motor_info.n_part_axis[m_n_part][AXIS_NUM];
	m_n_position_num	= st_motor_info.n_axis_pos_num[m_n_part][AXIS_NUM];
	
	for(i=0; i<m_n_position_num; i++)
	{
		m_str_position_name[i]	= st_motor_info.str_axis_pos_name[m_n_part][AXIS_NUM][i];
		m_str_position_bmp[i]	= st_motor_info.str_axis_pos_info[m_n_part][AXIS_NUM][i];
	}
	
	m_str_axis_name = st_motor_info.str_part_axis_name[m_n_part][AXIS_NUM];
	m_n_minus_el	= st_motor_info.n_axis_minus_el[m_n_part][AXIS_NUM];
	m_n_plus_el		= st_motor_info.n_axis_plus_el[m_n_part][AXIS_NUM];
	m_n_home		= st_motor_info.n_axis_home[m_n_part][AXIS_NUM];
	m_n_sd			= st_motor_info.n_axis_sd[m_n_part][AXIS_NUM];
	m_n_direction	= st_motor_info.n_axis_direction[m_n_part][AXIS_NUM];
	m_n_account		= st_motor_info.n_axis_account[m_n_part][AXIS_NUM];
	
	for(i=0; i<m_n_position_num; i++)
	{
		m_d_position[i][1]	= st_motor[m_n_axis].md_pos[i];
	}
	
	Data_Backup();
}

void CDialog_Motor_Axis_2::MotorStatusCheck()
{
m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

	if(m_n_home == 0)
	{
		if (m_n_sd == 1)
		{
			if(m_n_minus_el == 1)
			{
				if(COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELM) == CTLBD_RET_GOOD)
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, RED_C, WHITE_C);
				}
				else
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
				}
			}
			
			if(COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_SD) == CTLBD_RET_GOOD)
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, RED_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, BLACK_L, WHITE_C);
			}
			
			if(m_n_plus_el == 1)
			{
				if(COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 5, RED_C, WHITE_C);
				}
				else
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 5, BLACK_L, WHITE_C);
				}
			}
		}
		else
		{
			if(m_n_minus_el == 1)
			{
				if(COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELM) == CTLBD_RET_GOOD)
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, RED_C, WHITE_C);
				}
				else
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
				}
			}
			
			if(m_n_plus_el == 1)
			{
				if(COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 4, RED_C, WHITE_C);
				}
				else
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 4, BLACK_L, WHITE_C);
				}
			}
		}
	}
	else
	{
		if(m_n_minus_el == 1)
		{
			if(COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELM) == CTLBD_RET_GOOD)
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, RED_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
			}
		}
		
		if(COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_HOME) == CTLBD_RET_GOOD)
		{
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, RED_C, WHITE_C);
		}
		else
		{
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, BLACK_L, WHITE_C);
		}
		
		if(m_n_plus_el == 1)
		{
			if(COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELP) == CTLBD_RET_GOOD)
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 5, RED_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 5, BLACK_L, WHITE_C);
			}
		}
	}

	if(COMI.Get_MotPower(m_n_axis) == CTLBD_RET_GOOD)
	{
		m_p_grid.GridCellColor(m_grid_motor_info, 4, 1, RED_C, WHITE_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_motor_info, 4, 1, BLACK_L, WHITE_C);
	}

	if(COMI.Get_MotAlarmStatus(m_n_axis) == CTLBD_RET_GOOD)
	{
		m_p_grid.GridCellColor(m_grid_motor_info, 6, 1, BLACK_L, WHITE_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_motor_info, 6, 1, RED_C, WHITE_C);
	}
}

void CDialog_Motor_Axis_2::MotorPositionCheck()
{
//	m_dgt_current_pos.SetVal(COMI.Get_MotCurrentPos(st_part.n_motor_axis[MOTOR_PART]));
	CString str_tmp;

	m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

	str_tmp.Format("%.3f", COMI.Get_MotCurrentPos(m_n_axis));
	m_p_grid.GridCellText(m_grid_motor_info, 8, 3, str_tmp);
}

void CDialog_Motor_Axis_2::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CDialog_Message msg_dlg;

	CString strTmp, strChange, str_file;
	CString str_tmp, str_data;
	double value;
	int nResponse;
	double dCurr1, dCurr2;

	CString strText;
	
	CDialog_KeyPad	pad_dlg;
	
	int n_response;

	if(st_handler.mn_menu_lock) return;

	if(wParam == IDC_CUSTOM_MOTOR_POS)
	{
		if(lpcc->Row == 0) return;

		m_grid_motor_pos = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_POS);
		
		m_n_cmd_row	= lpcc->Row;
		m_n_cmd_col	= lpcc->Col;

		switch(lpcc->Col)
		{
		case 1:
			str_file.Format("C:\\AMT830\\Bmp\\%s", m_str_position_bmp[lpcc->Row-1]);
			m_p_bmp_view.Load(str_file);
			
			Invalidate(FALSE);
			break;

		case 2:				
			strText = m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col - 1);
			st_msg.mstr_keypad_msg.Format("[%s Motor] %s set", m_str_axis_name, strText);

			strTmp = m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col);
			st_msg.mstr_keypad_val = strTmp;
			
			st_msg.mstr_pad_high_limit.Format("%3.2f", float(COMI.md_limit_position[m_n_axis][1]));
			st_msg.mstr_pad_low_limit.Format("%3.2f", float(COMI.md_limit_position[m_n_axis][0]));
			
			st_msg.mn_dot_use = CTL_YES;
			st_msg.mn_pad_dialog = CTL_YES;
			
			n_response = pad_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				// 20140120
				/////////////20121107
//				if (strText == "Grip Clamp Module Pos")
//				{
//					if(st_handler.cwnd_list != NULL)
//					{  // 리스트 바 화면 존재 //
//						sprintf(st_msg.c_normal_msg, "고정 데이타이므로 변경이 안됩니다.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청 //
//					}
//				}
//				else
				/////////////
				{
					strTmp = st_msg.mstr_keypad_val;
					m_d_position[lpcc->Row-1][1] = atof(strTmp);
					m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col, strTmp);

					strTmp.Format("[Motor] (%s)_Axis [%.3f]Position Change", m_str_axis_name, m_d_position[lpcc->Row-1][1]);
					Func.On_LogFile_Add(99, strTmp);
				}
			}
			else if (n_response == IDCANCEL)
			{
				
			}
			break;
		case 3:
			if(COMI.mn_motorbd_init_end != TRUE)  
			{
				st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
				
				nResponse = msg_dlg.DoModal();
				if(nResponse == IDOK) return ;
			}

			///////////////////////////////////////// 20121114
			if (m_str_axis_name == "Vision1_Y")
			{
				dCurr1 = COMI.Get_MotCurrentPos(M_VISION1_Z);
				dCurr2 = COMI.Get_MotCurrentPos(M_VISION2_Z);
				if(dCurr1 <= st_motor[M_VISION1_Z].md_pos[Z_VISION_SAFETY] && dCurr2 <= st_motor[M_VISION2_Z].md_pos[Z_VISION_SAFETY])
				{//Z축 1번이 Ready이고, 2번이 Ready일 때 
					st_msg.str_fallacy_msg = _T("Vision Buffer1, 2번 Z축 모두 Ready 위치입니다. 한 축을 Up 하세요");	
					if(st_handler.mn_language == LANGUAGE_ENGLISH)
					{
						st_msg.str_fallacy_msg = _T("Vision Buffer1, 2 times, all the Z-axis position of Ready. Up on the one axis");
					}
					
					n_response = msg_dlg.DoModal();
					return;	
				}//Z축 1번이 Ready가 아니고, 2번도 Ready가 아닐 때 
				else if(dCurr1 > st_motor[M_VISION1_Z].md_pos[Z_VISION_SAFETY] && dCurr2 > st_motor[M_VISION2_Z].md_pos[Z_VISION_SAFETY])
				{
					st_msg.str_fallacy_msg = _T("Vision Buffer1, 2번 Z축 모두 Up 위치입니다. 한 축을 Down 하세요");
					if(st_handler.mn_language == LANGUAGE_ENGLISH)
					{
						st_msg.str_fallacy_msg = _T("Vision Buffer1, 2 times, all the Z-axis position of Up. Down on the one axis");
					}
					
					n_response = msg_dlg.DoModal();
					return;	
				}
				
			}
			/////////////////////////////////////////

			m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col, "ing...");
			m_p_grid.GridCellColor(m_grid_motor_pos, lpcc->Row, lpcc->Col, NOR_C, BLACK_C);

			m_d_mot_goto_pos = m_d_position[lpcc->Row-1][1];
			nResponse = Func.OnMotor_Go_Check(m_n_axis, m_d_mot_goto_pos);

			if(nResponse != IDOK)
			{
				MotorCompletion(m_n_cmd_row, m_n_cmd_col);
				return;
			}
			
			if(m_d_mot_goto_pos <= COMI.md_limit_position[m_n_axis][0])
			{
				MotorCompletion(m_n_cmd_row, m_n_cmd_col);
				return;
			}

			if(st_handler.mn_menu_lock != TRUE)
			{
				st_handler.mn_menu_lock = TRUE;
			}
			// =============================================================================
			
			MotorMoveStepClear();				// 모터 동작 처리 스텝 초기화 함수
			
			m_n_cmd_motor_no	= m_n_axis;					// 모터 번호 설정
			m_n_cmd_no			= POS_MOVING;	
			m_d_mot_goto_spd	= st_basic.nManualSpeed;
		
			m_str_cmd_motor		= m_str_axis_name;
			m_n_position_move	= CTL_YES;

			strTmp.Format("[Motor] (%s)_Axis [%.3f]Position Change", m_str_axis_name, m_d_position[lpcc->Row-1][1]);
			Func.On_LogFile_Add(99, strTmp);
			Func.On_LogFile_Add(0, strTmp);

			SetTimer(TM_MOVE_CHECK, 10, NULL);		// 모터 동작 제어 타이머
			break;
		case 4:
			if(COMI.mn_motorbd_init_end != TRUE)  
			{
				st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
				
				nResponse = msg_dlg.DoModal();
				if(nResponse == IDOK) return ;
			}
			///////////// 20121107
			strText = m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, lpcc->Col - 3);
//			if (strText == "Grip Clamp Module Pos")
//			{
//				if(st_handler.cwnd_list != NULL)
//				{  // 리스트 바 화면 존재 //
//					sprintf(st_msg.c_normal_msg, "고정 데이타이므로 변경이 안됩니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청 //
//				}
//			}
//			else
			/////////////
			{
				lpcc->Col = 2;
				value = COMI.Get_MotCurrentPos(m_n_axis);  // FASTECH 모터 특정 축의 커맨트 위치 리턴 함수 //
				
				nResponse = Func.On_Encoder_Check(value);

				if(nResponse == CTL_YES)
				{
					m_d_position[lpcc->Row-1][1] = value;
					strTmp.Format("%.3f", m_d_position[lpcc->Row-1][1]);
					m_p_grid.GridCellText(m_grid_motor_pos, lpcc->Row, 2, strTmp);	
				}

				strTmp.Format("[Motor] (%s)_Axis [%.3f]Position Change", m_str_axis_name, m_d_position[lpcc->Row-1][1]);
				Func.On_LogFile_Add(99, strTmp);

				MotorCompletion(m_n_cmd_row, m_n_cmd_col);
			}
			break;
		}
	}
	else
	{
		m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

		switch(lpcc->Row)
		{
		case 3:
		case 4:
			switch(lpcc->Col)
			{
			case 3:
			case 4:
				if(COMI.mn_motorbd_init_end != TRUE)  
				{
					st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
					
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK) return ;
				}
				strTmp.Format("[Motor] (%s)_Axis Power Off", m_str_axis_name);
				Func.On_LogFile_Add(99, strTmp);
				
				COMI.Set_MotPower(m_n_axis, FALSE);
				break;
				
			case 5:
			case 6:
				if(COMI.mn_motorbd_init_end != TRUE)  
				{
					st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
					
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK) return ;
				}
				strTmp.Format("[Motor] (%s)_Axis Power Off", m_str_axis_name);
				Func.On_LogFile_Add(99, strTmp);
				
				COMI.Set_MotPower(m_n_axis, TRUE);
				break;
			}
			break;
			
			case 5:
			case 6:
				if(lpcc->Col == 3 || lpcc->Col == 4 || lpcc->Col == 5 || lpcc->Col == 6)
				{
					if(COMI.mn_motorbd_init_end != TRUE)  
					{
						st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
						
						nResponse = msg_dlg.DoModal();
						if(nResponse == IDOK) return ;
					}
					
					strTmp.Format("[Motor] (%s)_Axis Alarm Clear", m_str_axis_name);
					Func.On_LogFile_Add(99, strTmp);
					
					COMI.Set_MotAlarmClear(m_n_axis);
				}
				break;
				
			case 8:
				switch(lpcc->Col)
				{
				case 1:
					if(m_n_move_mode != 0)
					{
						m_n_move_mode = 0;
						
						m_p_grid.GridCellColor(m_grid_motor_info, 8, 1, RED_C, BLACK_C);
						m_p_grid.GridCellColor(m_grid_motor_info, 8, 2, BLACK_L, YELLOW_C);
					}
					break;
					
				case 2:
					if(m_n_move_mode != 1)
					{
						m_n_move_mode = 1;
						
						m_p_grid.GridCellColor(m_grid_motor_info, 8, 1, BLACK_L, YELLOW_C);
						m_p_grid.GridCellColor(m_grid_motor_info, 8, 2, RED_C, BLACK_C);
					}
					break;
					
				case 5:
				case 6:
					strTmp.Format("%.3f", m_d_data);
					st_msg.mstr_keypad_msg.Format("[Jog Data]  set");
					st_msg.mstr_keypad_val = strTmp;
					
					st_msg.mstr_pad_high_limit.Format("10000.0");
					st_msg.mstr_pad_low_limit.Format("0.001");
					
					st_msg.mn_dot_use = CTL_YES;
					st_msg.mn_pad_dialog = CTL_YES;
					
					n_response = pad_dlg.DoModal();
					
					if (n_response == IDOK)
					{
						strTmp = st_msg.mstr_keypad_val;
						m_d_data = atof(strTmp);
						m_p_grid.GridCellText(m_grid_motor_info, 8, 5, strTmp);
					}
					else if (n_response == IDCANCEL)
					{
						
					}
					break;
					
				}
				break;
		}
	}
}

void CDialog_Motor_Axis_2::MotorMoveStepClear()
{
	m_b_stop_req = FALSE;	// ESTOP 요청 플래그 초기화
	
	m_n_cmd_no = 0;			// 실행 명령 번호 저장 변수 초기화
	m_n_cmd_motor_no = -1;	// 모터 번호 저장 변수 초기화
	m_str_cmd_motor = _T("Motor NULL");
	
	m_n_home_step = 0;		// 모터 HOMING 처리 스텝 저장 변수 초기화
	m_n_move_step = 0;		// 모터 이동 처리 스텝 저장 변수 초기화
}

void CDialog_Motor_Axis_2::MotorCompletion(int nrow, int ncol)
{
	m_grid_motor_pos = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_POS);
	
	if(ncol == 3)
	{
		m_p_grid.GridCellText(m_grid_motor_pos, nrow, ncol, "Go.");
		m_p_grid.GridCellColor(m_grid_motor_pos, nrow, ncol, TEXT_BC, BLUE_C);
	}
	else
	{
		m_p_grid.GridCellText(m_grid_motor_pos, nrow, ncol, "Read.");
		m_p_grid.GridCellColor(m_grid_motor_pos, nrow, ncol, TEXT_BC, RED_C);
	}
}

int CDialog_Motor_Axis_2::MotorHomingExcution()
{
	int n_home_flag = CTLBD_RET_PROCEED;
	int nRet;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다
	// =============================================================================
	if(m_b_stop_req){
		if(m_n_home_step < 100){
			m_n_home_step = 100;
		}
	}
	// =============================================================================
	if(FAS_IO.get_in_bit(st_io.i_stacker_light_curtain_chk, IO_ON) == IO_ON ||
		FAS_IO.get_in_bit(st_io.i_vision_light_curtain_chk, IO_ON) == IO_ON)
	{
		COMI.Set_MotStop(0, m_n_cmd_motor_no);
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Error] Area sensor is detected. Motor stop. ReWork Please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		COMI.mn_home_step[m_n_cmd_motor_no] = 0;
		m_n_home_step = 100;
		return n_home_flag;
	}
//	if(alarm.n_area_ready_alarm[0] == TRUE || alarm.n_area_ready_alarm[1] == TRUE)
//	{
//		COMI.Set_MotStop(1, m_n_cmd_motor_no);
//		COMI.mn_home_step[m_n_cmd_motor_no] = 0;
//		return n_home_flag;
//	}

	switch(m_n_home_step)
	{
		case 0 :
			nRet = COMI.HomeCheck_Mot(m_n_cmd_motor_no, st_motor[m_n_cmd_motor_no].mn_homecheck_method, MOT_TIMEOUT);

			if (nRet == CTLBD_RET_GOOD)				// 정상적으로 Home Check가 끝났을 경우.
			{
				if (m_b_stop_req != FALSE)
					m_b_stop_req = FALSE;		// ESTOP 요청 플래그 초기화
				
				m_n_home_step = 0; 
				n_home_flag = CTLBD_RET_GOOD;
			}
			else if (nRet == CTLBD_RET_ERROR)		// Home Check에 실패 했을 경우.
			{
				if (m_b_stop_req != FALSE)
					m_b_stop_req = FALSE;		// ESTOP 요청 플래그 초기화
				
				m_n_home_step = 0; 
				n_home_flag = CTLBD_RET_ERROR;
			}
			else if (nRet == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					//st_msg.mstr_abnormal_msg = _T("[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					sprintf(st_msg.c_abnormal_msg, "[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_n_home_step = 0; 
				n_home_flag = CTLBD_RET_ERROR;
			}
			break;

		case 100 :
			// =============================================================================
			// 특정 축 긴급 정지 이벤트로 모터 정지한다
			// =============================================================================
			nRet = COMI.Set_MotStop(0, m_n_cmd_motor_no);

			if (nRet == CTLBD_RET_GOOD)
			{
				m_n_home_step = 110;
			}
			else 
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					//st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Though canceled transfer of motor, error happened.");
					sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Though canceled transfer of motor, error happened.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				
				m_n_home_step = 120;
			}
			break;

		case 110 :
			if(st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재 //
			//	st_msg.mstr_normal_msg = _T("[" + m_str_cmd_motor + "] Motor shut down during transfer.");
				sprintf(st_msg.c_normal_msg, "[" + m_str_cmd_motor + "] Motor shut down during transfer.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청 //
			}

			//if(m_nstop_req != FALSE){
			if(m_b_stop_req){
				m_b_stop_req = FALSE;		// ESTOP 요청 플래그 초기화
			}

			m_n_home_step = 0;
			n_home_flag = CTLBD_RET_GOOD;
			break;

		case 120 :
			if(st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재 //
			//	st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
			}

			//if(m_nstop_req != FALSE){
			if(m_b_stop_req)
			{
				m_b_stop_req = FALSE;	// ESTOP 요청 플래그 초기화 //
			}

			st_motor[m_n_cmd_motor_no].mn_home_step = 0;
			m_n_home_step = 0; 
			n_home_flag = CTLBD_RET_ERROR;
			break;

		case 130 :
			if(st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재 //
				//st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
			}

			//if(m_nstop_req != FALSE){
			if(m_b_stop_req)
			{
				m_b_stop_req = FALSE;	// ESTOP 요청 플래그 초기화 //
			}

			st_motor[m_n_cmd_motor_no].mn_home_step = 0;
			m_n_home_step = 0; 
			n_home_flag = CTLBD_RET_ERROR;
			break;
	}

	return  n_home_flag;
}

int CDialog_Motor_Axis_2::MotorMovingExcution()
{
	int n_move_flag = CTLBD_RET_PROCEED;
	int nRet;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// =============================================================================
	if(m_b_stop_req)
	{
		if(m_n_move_step < 100)
		{
			m_n_move_step = 100;
		}
	}
	// =============================================================================
	if(FAS_IO.get_in_bit(st_io.i_stacker_light_curtain_chk, IO_ON) == IO_ON ||
		FAS_IO.get_in_bit(st_io.i_vision_light_curtain_chk, IO_ON) == IO_ON)
	{
		COMI.Set_MotStop(0, m_n_cmd_motor_no);
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Error] Area sensor is detected. Motor stop. ReWork Please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		m_n_move_step = 100;
		return n_move_flag;
	}
	//	if(alarm.n_area_ready_alarm[0] == TRUE || alarm.n_area_ready_alarm[1] == TRUE)
//	{
//		COMI.Set_MotStop(1, m_n_cmd_motor_no);
//		m_n_move_step = 100;
//		return n_move_flag;
//	}

	switch(m_n_move_step)
	{
		case 0 :
			// =============================================================================
			// 모터 특정 위치로 이동시킨다. (이동위치 세팅~)                         
			// =============================================================================
			nRet = COMI.Start_SingleMove(m_n_cmd_motor_no, m_d_mot_goto_pos, (int)m_d_mot_goto_spd);
			

			if (nRet == CTLBD_RET_GOOD)
			{
				m_n_move_step = 10;
			}
			else if (nRet == CTLBD_RET_ERROR)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
				//	st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Motor failed to move.");
					sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Motor failed to move.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_n_move_step = 130; 
			}
			else if (nRet == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
				//	st_msg.mstr_abnormal_msg = _T("[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					sprintf(st_msg.c_abnormal_msg, "[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_n_move_step = 130;
			}
			break;
		case 10 :
			nRet = COMI.Check_SingleMove(m_n_cmd_motor_no, m_d_mot_goto_pos);

			if (nRet == CTLBD_RET_GOOD)
			{
				m_n_move_step = 20;
			}
			else if (nRet == CTLBD_RET_ERROR)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
				//	st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Motor failed to move.");
					sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Motor failed to move.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_n_move_step = 130; 
			}
			else if (nRet == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
				//	st_msg.mstr_abnormal_msg = _T("[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					sprintf(st_msg.c_abnormal_msg, "[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_n_move_step = 130;
			}
			break;

		case 20 :
			if (st_handler.cwnd_list != NULL)
			{
			//	st_msg.mstr_normal_msg = _T("[" + m_str_cmd_motor + "] Motor completed transfer.");
				sprintf(st_msg.c_normal_msg, "[" + m_str_cmd_motor + "] Motor completed transfer.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
			}
			
			if (m_b_stop_req)
			{
				m_b_stop_req = FALSE;  // ESTOP 요청 플래그 초기화
			}
			
			m_n_move_step = 0;
			n_move_flag = CTLBD_RET_GOOD;
			break;

		case 100 :
			// =============================================================================
			// 특정 축 긴급 정지 이벤트로 모터 정지한다                              
			// =============================================================================
			nRet = COMI.Set_MotStop(0, m_n_cmd_motor_no);
			
			if (nRet == CTLBD_RET_GOOD)
			{
				m_n_move_step = 110;
			}
			else 
			{
				if (st_handler.cwnd_list != NULL)
				{  // 리스트 바 화면 존재
				//	st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Though canceled transfer of motor, error happened.");
					sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Though canceled transfer of motor, error happened.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				
				m_n_move_step = 120;
			}
			// =============================================================================
			break;
		case 110 :
			if (st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재
			//	st_msg.mstr_normal_msg = _T("[" + m_str_cmd_motor + "] Motor shut down during transfer.");
				sprintf(st_msg.c_normal_msg, "[" + m_str_cmd_motor + "] Motor shut down during transfer.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
			}
			
			if (m_b_stop_req)
			{
				m_b_stop_req = FALSE;  // ESTOP 요청 플래그 초기화
			}
			
			m_n_move_step = 0;
			n_move_flag = CTLBD_RET_GOOD;
			break;

		case 120:
			if (st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재
			//	st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			
			if (m_b_stop_req)
			{
				m_b_stop_req = FALSE;  // ESTOP 요청 플래그 초기화
			}
			
			m_n_move_step = 0; 
			n_move_flag = CTLBD_RET_ERROR;
			break;
			
		case 130:
			if (st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재
		//		st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			
			if (m_b_stop_req)
			{
				m_b_stop_req = FALSE;  // ESTOP 요청 플래그 초기화
			}
			
			m_n_move_step = 0; 
			n_move_flag = CTLBD_RET_ERROR;
			break;
	}

	return  n_move_flag;
}

void CDialog_Motor_Axis_2::OnButtonControl(BOOL b_flag)
{
}


void CDialog_Motor_Axis_2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	CDC *pDC;

	// TODO: Add your message handler code here

	pDC = GetDlgItem(IDC_BMP_VIEW)->GetDC();
	GetDlgItem(IDC_BMP_VIEW)->GetWindowRect(rect);
	
	m_p_bmp_view.Show(pDC, CPoint(0, 0), CPoint(rect.Width(), rect.Height()), 0,0);
	
	ReleaseDC(pDC);
	// Do not call CDialog::OnPaint() for painting messages
}

void CDialog_Motor_Axis_2::Init_Grid_Info()
{
	int   i, j;
	CString str_tmp;

	m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);
	
	m_p_grid.GridReset(m_grid_motor_info);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_motor_info, TRUE);
	
	m_p_grid.GridRowHeader(m_grid_motor_info, FALSE);
	m_p_grid.GridColHeader(m_grid_motor_info, FALSE);
	m_p_grid.GridHorizontal(m_grid_motor_info, FALSE);
	m_p_grid.GridVertical(m_grid_motor_info, FALSE);
	m_p_grid.GridAutoSize(m_grid_motor_info, FALSE);
	m_p_grid.GridAutoSize(m_grid_motor_info, FALSE);
	m_p_grid.GridCellRows(m_grid_motor_info, 8);
	m_p_grid.GridCellCols(m_grid_motor_info, 6);
	
	for(i=0; i<8; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_motor_info, i+1, 20);
		for(j=0; j<6; j++)
		{
			m_p_grid.GridCellWidth_L(m_grid_motor_info, j+1, 9.5);
	
			m_p_grid.GridCellControlStatic(m_grid_motor_info, i+1, j+1);
		}
	}

	if(m_n_home == 0)
	{
		if (m_n_sd == 1)
		{
			if(m_n_minus_el == 1)
			{
				m_p_grid.GridCellMerge(m_grid_motor_info, 1, 1, 1, 2);
				m_p_grid.GridCellFont(m_grid_motor_info, 1, 1, "MS Sans Serif", 12);
				m_p_grid.GridCellColor(m_grid_motor_info, 1, 1, BLUE_D, WHITE_C);
				m_p_grid.GridCellText(m_grid_motor_info, 1, 1, "－ ML");
				
				m_p_grid.GridCellMerge(m_grid_motor_info, 2, 1, 1, 2);
				m_p_grid.GridCellFont(m_grid_motor_info, 2, 1, "MS Sans Serif", 12);
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
				m_p_grid.GridCellText(m_grid_motor_info, 2, 1, "");
			}
			
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 3, 1, 2);
			m_p_grid.GridCellFont(m_grid_motor_info, 1, 3, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 3, BLUE_D, WHITE_C);
			m_p_grid.GridCellText(m_grid_motor_info, 1, 3, "SD");
			
			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 3, 1, 2);
			m_p_grid.GridCellFont(m_grid_motor_info, 2, 3, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(m_grid_motor_info, 2, 3, "");
			
			if (m_n_plus_el == 1)
			{
				m_p_grid.GridCellMerge(m_grid_motor_info, 1, 5, 1, 2);
				m_p_grid.GridCellFont(m_grid_motor_info, 1, 5, "MS Sans Serif", 12);
				m_p_grid.GridCellColor(m_grid_motor_info, 1, 5, BLUE_D, WHITE_C);
				m_p_grid.GridCellText(m_grid_motor_info, 1, 5, "＋ PL");
				
				m_p_grid.GridCellMerge(m_grid_motor_info, 2, 5, 1, 2);
				m_p_grid.GridCellFont(m_grid_motor_info, 2, 5, "MS Sans Serif", 12);
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 5, BLACK_L, WHITE_C);
				m_p_grid.GridCellText(m_grid_motor_info, 2, 5, "");
			}
		}
		else
		{
			if (m_n_minus_el == 1)
			{
				m_p_grid.GridCellMerge(m_grid_motor_info, 1, 1, 1, 3);
				m_p_grid.GridCellFont(m_grid_motor_info, 1, 1, "MS Sans Serif", 12);
				m_p_grid.GridCellColor(m_grid_motor_info, 1, 1, BLUE_D, WHITE_C);
				m_p_grid.GridCellText(m_grid_motor_info, 1, 1, "- ML");
				
				m_p_grid.GridCellMerge(m_grid_motor_info, 2, 1, 1, 3);
				m_p_grid.GridCellFont(m_grid_motor_info, 2, 1, "MS Sans Serif", 12);
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
				m_p_grid.GridCellText(m_grid_motor_info, 2, 1, "");
			}
			
			if (m_n_plus_el == 1)
			{
				m_p_grid.GridCellMerge(m_grid_motor_info, 1, 4, 1, 3);
				m_p_grid.GridCellFont(m_grid_motor_info, 1, 4, "MS Sans Serif", 12);
				m_p_grid.GridCellColor(m_grid_motor_info, 1, 4, BLUE_D, WHITE_C);
				m_p_grid.GridCellText(m_grid_motor_info, 1, 4, "＋ PL");
				
				m_p_grid.GridCellMerge(m_grid_motor_info, 2, 4, 1, 3);
				m_p_grid.GridCellFont(m_grid_motor_info, 2, 4, "MS Sans Serif", 12);
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 4, BLACK_L, WHITE_C);
				m_p_grid.GridCellText(m_grid_motor_info, 2, 4, "");
			}
		}
	}
	else
	{
		if(m_n_minus_el == 1)
		{
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 1, 1, 2);
			m_p_grid.GridCellFont(m_grid_motor_info, 1, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 1, BLUE_D, WHITE_C);
			m_p_grid.GridCellText(m_grid_motor_info, 1, 1, "－ ML");
			
			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 1, 1, 2);
			m_p_grid.GridCellFont(m_grid_motor_info, 2, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(m_grid_motor_info, 2, 1, "");
		}
		
		m_p_grid.GridCellMerge(m_grid_motor_info, 1, 3, 1, 2);
		m_p_grid.GridCellFont(m_grid_motor_info, 1, 3, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(m_grid_motor_info, 1, 3, BLUE_D, WHITE_C);
		m_p_grid.GridCellText(m_grid_motor_info, 1, 3, "HOME");
		
		m_p_grid.GridCellMerge(m_grid_motor_info, 2, 3, 1, 2);
		m_p_grid.GridCellFont(m_grid_motor_info, 2, 3, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, BLACK_L, WHITE_C);
		m_p_grid.GridCellText(m_grid_motor_info, 2, 3, "");

		if(m_n_plus_el == 1)
		{
			m_p_grid.GridCellMerge(m_grid_motor_info, 1, 5, 1, 2);
			m_p_grid.GridCellFont(m_grid_motor_info, 1, 5, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 1, 5, BLUE_D, WHITE_C);
			m_p_grid.GridCellText(m_grid_motor_info, 1, 5, "＋ PL");
			
			m_p_grid.GridCellMerge(m_grid_motor_info, 2, 5, 1, 2);
			m_p_grid.GridCellFont(m_grid_motor_info, 2, 5, "MS Sans Serif", 12);
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 5, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(m_grid_motor_info, 2, 5, "");
		}
	}

	m_p_grid.GridCellMerge(m_grid_motor_info, 3, 1, 1, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 3, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 3, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_motor_info, 3, 1, "Motor Power");
	
	m_p_grid.GridCellMerge(m_grid_motor_info, 4, 1, 1, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 4, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 4, 1, BLACK_L, WHITE_C);
	m_p_grid.GridCellText(m_grid_motor_info, 4, 1, "");

	m_p_grid.GridCellMerge(m_grid_motor_info, 3, 3, 2, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 3, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 3, 3, TEXT_BC, RED_C);
	m_p_grid.GridCellText(m_grid_motor_info, 3, 3, "Power Off");

	m_p_grid.GridCellMerge(m_grid_motor_info, 3, 5, 2, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 3, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 3, 5, TEXT_BC, RED_C);
	m_p_grid.GridCellText(m_grid_motor_info, 3, 5, "Power On");


	m_p_grid.GridCellMerge(m_grid_motor_info, 5, 1, 1, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 5, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 5, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_motor_info, 5, 1, "Motor Alarm");
	
	m_p_grid.GridCellMerge(m_grid_motor_info, 6, 1, 1, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 6, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 6, 1, BLACK_L, WHITE_C);
	m_p_grid.GridCellText(m_grid_motor_info, 6, 1, "");

	m_p_grid.GridCellMerge(m_grid_motor_info, 5, 3, 2, 4);
	m_p_grid.GridCellFont(m_grid_motor_info, 5, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 5, 3, TEXT_BC, RED_C);
	m_p_grid.GridCellText(m_grid_motor_info, 5, 3, "Alarm Reset");

	m_p_grid.GridCellMerge(m_grid_motor_info, 7, 1, 1, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 7, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 7, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_motor_info, 7, 1, "Motor Move");
	
	m_p_grid.GridCellFont(m_grid_motor_info, 8, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 8, 1, RED_C, BLACK_C);
	m_p_grid.GridCellText(m_grid_motor_info, 8, 1, "Jog");

	m_p_grid.GridCellFont(m_grid_motor_info, 8, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 8, 2, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_motor_info, 8, 2, "Distance");

	m_p_grid.GridCellMerge(m_grid_motor_info, 7, 3, 1, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 7, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 7, 3, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_motor_info, 7, 3, "Current Position");
	
	m_p_grid.GridCellMerge(m_grid_motor_info, 8, 3, 1, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 8, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 8, 3, WHITE_C, BLACK_C);
	m_p_grid.GridCellText(m_grid_motor_info, 8, 3, "0");

	m_p_grid.GridCellMerge(m_grid_motor_info, 7, 5, 1, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 7, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 7, 5, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_motor_info, 7, 5, "Move Distance (mm)");
	
	m_p_grid.GridCellMerge(m_grid_motor_info, 8, 5, 1, 2);
	m_p_grid.GridCellFont(m_grid_motor_info, 8, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_motor_info, 8, 5, WHITE_C, BLACK_C);
	str_tmp.Format("%.2f", m_d_data);
	m_p_grid.GridCellText(m_grid_motor_info, 8, 5, str_tmp);
}
/*
BOOL CDialog_Motor_Axis_2::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Data_HistoryLog();
	Data_Apply();

	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}

	KillTimer(TM_STATUS_CHECK);
	KillTimer(TM_POSITION_CHECK);
	KillTimer(TM_HOME_CHECK);
	KillTimer(TM_MOVE_CHECK);

	return CDialog::DestroyWindow();
}
*/

void CDialog_Motor_Axis_2::OnTimer(UINT nIDEvent) 
{
	int nResponse;

	switch(nIDEvent)
	{
		case TM_STATUS_CHECK:
			MotorStatusCheck();
			break;

		case TM_POSITION_CHECK:
			MotorPositionCheck();
			break;

		case TM_HOME_CHECK:
			nResponse = MotorHomingExcution();		// 모터 HOMING 처리 함수 //
			
			if(nResponse == CTLBD_RET_GOOD)
			{
				KillTimer(TM_HOME_CHECK) ;  // 타이머 삭제 //
				if(st_handler.mn_home_state[m_n_axis] != TRUE)
				{
					st_handler.mn_home_state[m_n_axis] = TRUE;
				}
				
				m_n_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화 //
				
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================

				OnButtonControl(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
			}
			else if(nResponse == CTLBD_RET_ERROR)
			{
				KillTimer(TM_HOME_CHECK) ;  // 타이머 삭제 //
				
				m_n_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화 //
				
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================
				OnButtonControl(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
			}
			break;

		case TM_MOVE_CHECK:
			nResponse = MotorMovingExcution();		// 모터 HOMING 처리 함수 //
			
			if(nResponse == CTLBD_RET_GOOD)
			{
				KillTimer(TM_MOVE_CHECK) ;  // 타이머 삭제 //
		
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================
				
				OnButtonControl(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
				if(m_n_position_move == CTL_YES)
				{
					m_n_position_move = CTL_NO;
					MotorCompletion(m_n_cmd_row, m_n_cmd_col);
				}
				m_n_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화 //
			}
			else if(nResponse == CTLBD_RET_ERROR)
			{
				KillTimer(TM_MOVE_CHECK) ;  // 타이머 삭제 //
				
				// ==============================================================================
				// 메뉴 사용 가능하도록 플래그 설정한다
				// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그
				//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다
				// ==============================================================================
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================
				
				OnButtonControl(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
				if(m_n_position_move == CTL_YES)
				{
					m_n_position_move = CTL_NO;
					MotorCompletion(m_n_cmd_row, m_n_cmd_col);
				}
				m_n_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화 //
			}
			break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDialog_Motor_Axis_2::OnDestroy() 
{
	Data_HistoryLog();
	Data_Apply();
	
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}
	
	KillTimer(TM_STATUS_CHECK);
	KillTimer(TM_POSITION_CHECK);
	KillTimer(TM_HOME_CHECK);
	KillTimer(TM_MOVE_CHECK);

	CDialog::OnDestroy();
}

void CDialog_Motor_Axis_2::OnBtnEmg() 
{
	m_b_stop_req = TRUE;	// ESTOP 요청 플래그
}

// **********************************************************************
// OnUserLButtonDown() : 버튼의 WM_LBUTTONDOWN 이벤트 핸들러입니다.
// CButton 클래스는 WM_LBUTTONDOWN 이벤트를 지원하지 않으므로 CButtonST
// 이라는 클래스를 생성하여 WM_LBUTTONDOWN 이벤트를 받은후에 User-event로
// CSxMotionDlg 클래스에 전달하는 방식을 취하였습니다.
// **********************************************************************
LONG CDialog_Motor_Axis_2::OnUserLButtonDown(WPARAM wParam, LPARAM lParam)
{
	CDialog_Message msg_dlg;
	CString strTmp;
	int		nRet;
	double  dmot_curr_pos;
	
	if(FAS_IO.get_in_bit(st_io.i_stacker_light_curtain_chk, IO_ON) == IO_ON ||
		FAS_IO.get_in_bit(st_io.i_vision_light_curtain_chk, IO_ON) == IO_ON)
	{
		COMI.Set_MotStop(0, m_n_cmd_motor_no);
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Error] Area sensor is detected. Motor stop. ReWork Please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		COMI.mn_home_step[m_n_axis] = 0;
		CTL_Lib.mn_single_motmove_step[m_n_axis] = 0;
		return 0;
	}
//	if(alarm.n_area_ready_alarm[0] == TRUE)
//	{
//		COMI.Set_MotStop(1, m_n_axis);
//		CTL_Lib.mn_single_motmove_step[m_n_axis] = 0;
//		COMI.mn_home_step[m_n_axis] = 0;
//		return 0;
//	}

	nRet = Func.DoorOpenCheckSpot();
	
	if (nRet == CTLBD_RET_ERROR)
	{
// 		st_msg.str_fallacy_msg = _T("If the Open Door, does not work.");
// 		::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);
		return 0;
	}

	// Velocity-Move(Jog) 모드일때만 WM_LBUTTONDOWN 이벤트를 처리한다. //
	// Rel/Abs Position Mode에서는 WM_LBUTTONUP 이벤트에서 이송 명령을 //
	// 처리한다.                                                       //
	CButtonST *pButton = (CButtonST *)lParam;

	nRet = pButton->GetTag();

	if (pButton->GetTag() == IDC_BTN_LEFT)
	{ // "Jog (-)" 버튼이 눌린 경우...
		if(m_n_move_mode == 0)
		{
			nRet = CTL_Lib.Motor_SafetyCheck(0, m_n_axis, 0);
			
			if(nRet == CTLBD_RET_ERROR)
			{
				return 0;
			}
			
			OnButtonControl(FALSE);	
			
			strTmp.Format("[Motor] (%s)_Axis - Jog Move", m_str_axis_name);
			Func.On_LogFile_Add(99, strTmp);
			
			COMI.Set_MotSpeed(MOT_SPD_JOG, m_n_axis, cmSMODE_KEEP, st_motor[m_n_axis].md_spd_jog, st_motor[m_n_axis].md_spd_acc, st_motor[m_n_axis].md_spd_dec);		// 축의 속도를 변경한다.
			
			if(cmmSxVMoveStart(m_n_axis, MINUS) != cmERR_NONE)
			{ 
				cmmErrShowLast(GetSafeHwnd());
			}
			else
			{
				st_handler.mn_menu_lock = FALSE;
			}
		}
		else
		{
			// 차후 이동가능 여부 체크 추가 해야됨......
			OnButtonControl(FALSE);						// 버튼 비활성화
			MotorMoveStepClear();							// 모터제어 함수 스텝 초기화
			
			m_n_cmd_motor_no	= m_n_axis;					// 모터 번호 설정
			m_n_cmd_no			= MOVING;							// 실행 명령 번호 설정 [모터 이동 명령]
			m_d_mot_goto_spd	= (st_motor[m_n_cmd_motor_no].md_spd_jog / COMI.md_spd_vel[m_n_cmd_motor_no][0]) * 100;
			m_str_cmd_motor		= m_str_axis_name;
			
			dmot_curr_pos		= COMI.Get_MotCurrentPos(m_n_axis);
			
			m_d_mot_goto_pos	= dmot_curr_pos - m_d_data;
			
			strTmp.Format("[Motor] (%s)_Axis (%.3f) - Data Move", m_str_axis_name, m_d_mot_goto_pos);
			Func.On_LogFile_Add(99, strTmp);
			
			SetTimer(TM_MOVE_CHECK, 10, NULL);
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT)
	{ 
		if(m_n_move_mode == 0)
		{
			dmot_curr_pos = COMI.Get_MotCurrentPos(m_n_axis);
			nRet = CTL_Lib.Motor_SafetyCheck(0, m_n_axis, dmot_curr_pos+1);
			
			if(nRet == CTLBD_RET_ERROR)
			{
				return 0;
			}
			
			OnButtonControl(FALSE);						// 버튼 비활성화
			
			strTmp.Format("[Motor] (%s)_Axis + Jog Move", m_str_axis_name);
			Func.On_LogFile_Add(99, strTmp);
			
			COMI.Set_MotSpeed(MOT_SPD_JOG, m_n_axis, cmSMODE_KEEP, st_motor[m_n_axis].md_spd_jog, st_motor[m_n_axis].md_spd_acc, st_motor[m_n_axis].md_spd_dec);		// 축의 속도를 변경한다.
			
			if (cmmSxVMoveStart(m_n_axis, PLUS) != cmERR_NONE)
			{ 
				cmmErrShowLast(GetSafeHwnd());
			}
			else
			{
				st_handler.mn_menu_lock = FALSE;
			}
		}
		else
		{
			// 차후 이동가능 여부 체크 추가 해야됨......				
			OnButtonControl(FALSE);						// 버튼 비활성화
			MotorMoveStepClear();							// 모터제어 함수 스텝 초기화
			
			m_n_cmd_motor_no	= m_n_axis;					// 모터 번호 설정
			m_n_cmd_no			= MOVING;							// 실행 명령 번호 설정 [모터 이동 명령]
			m_d_mot_goto_spd	= (st_motor[m_n_cmd_motor_no].md_spd_jog / COMI.md_spd_vel[m_n_cmd_motor_no][0]) * 100;
			m_str_cmd_motor		= m_str_axis_name;
			
			dmot_curr_pos		= COMI.Get_MotCurrentPos(m_n_axis);
			
			m_d_mot_goto_pos	= dmot_curr_pos + m_d_data;
			
			strTmp.Format("[Motor] (%s)_Axis (%.3f) + Data Move", m_str_axis_name, m_d_mot_goto_pos);
			Func.On_LogFile_Add(99, strTmp);
			
			SetTimer(TM_MOVE_CHECK, 10, NULL);
		}
	}

	return 0;
}

LONG CDialog_Motor_Axis_2::OnUserLButtonUp(WPARAM wParam, LPARAM lParam)
{	
	CDialog_Message msg_dlg;
	CString strTmp;
	int		nRet;

	nRet = Func.DoorOpenCheckSpot();
	
	if (nRet == CTLBD_RET_ERROR)
	{
// 		st_msg.str_fallacy_msg = _T("If the Open Door, does not work.");
// 		::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);
		return 0;
	}

	// Velocity-Move(Jog) 모드일때만 WM_LBUTTONDOWN 이벤트를 처리한다. //
	// Rel/Abs Position Mode에서는 WM_LBUTTONUP 이벤트에서 이송 명령을 //
	// 처리한다.                                                       //
	CButtonST *pButton = (CButtonST *)lParam;

	nRet = pButton->GetTag();

	if (pButton->GetTag() == IDC_BTN_LEFT)
	{ // "Jog (-)" 버튼이 눌린 경우...
		if(m_n_move_mode == 0)
		{
			strTmp.Format("[Motor] (%s)_Axis - Jog Stop", m_str_axis_name);
			Func.On_LogFile_Add(99, strTmp);
			
			cmmSxStop(m_n_axis, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			OnButtonControl(TRUE);	
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT)
	{ 
		if(m_n_move_mode == 0)
		{
			strTmp.Format("[Motor] (%s)_Axis + Jog Stop", m_str_axis_name);
			Func.On_LogFile_Add(99, strTmp);
			
			cmmSxStop(m_n_axis, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			OnButtonControl(TRUE);	
		}
	}
	return 0;
}

void CDialog_Motor_Axis_2::OnBtnHome() 
{
	CString strTmp;
	
	OnButtonControl(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
	
	// ==============================================================================
	// 메뉴 사용 불가능하도록 플래그 설정한다                                    
	// -> 현재 모터 동작 중에 다른 메뉴 모터 동작시키면 오동작할 수 있기 때문이다
	// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그                  
	//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다               
	// ==============================================================================
	if(st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// ==============================================================================
	
	// ==============================================================================
	// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
	// ==============================================================================
	st_handler.mn_home_state[m_n_axis] = CTL_READY;	// 모터 HOMING 완료 플래그
	// ==============================================================================
	
	MotorMoveStepClear();							// 모터제어 함수 스텝 초기화
	
	m_n_cmd_motor_no						= m_n_axis;					// 모터 번호 설정
	m_n_cmd_no								= HOMING;							// 실행 명령 번호 설정 [모터 이동 명령]
	m_str_cmd_motor							= m_str_axis_name;
	st_motor[m_n_cmd_motor_no].mn_home_step	= 0;
	
	strTmp.Format("[Motor] (%s)_Axis Home Move", m_str_axis_name);
	Func.On_LogFile_Add(99, strTmp);
	
	SetTimer(TM_HOME_CHECK, 100, NULL);			// 모터 동작 제어 타이머
}

void CDialog_Motor_Axis_2::OnBtnApply() 
{
	Data_HistoryLog();
	Data_Apply();
}
