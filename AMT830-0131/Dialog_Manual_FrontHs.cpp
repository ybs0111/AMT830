// Dialog_Manual_FrontHs.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Manual_FrontHs.h"
#include "Public_Function.h"
#include "Cmmsdk.h"//20121006
#include "CtlBd_Function.h"
#include "CtlBd_Library.h"
#include "FastechPublic_IO.h"
#include "Dialog_Message.h"
#include "Dialog_KeyPad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TM_STATUS_CHECK		100
#define TM_POSITION_CHECK	200
#define TM_HOME_CHECK		300
#define TM_MOVE_CHECK		400

#define AXIS_NUM			0

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_FrontHs dialog


CDialog_Manual_FrontHs::CDialog_Manual_FrontHs(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Manual_FrontHs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Manual_FrontHs)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_n_move_mode = 0;

}


void CDialog_Manual_FrontHs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Manual_FrontHs)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BTN_STOP, m_btn_stop);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btn_right);
	DDX_Control(pDX, IDC_BTN_HOME, m_btn_home);
	DDX_Control(pDX, IDC_BTN_LEFT, m_btn_left);
	DDX_Control(pDX, IDC_GROUP_MOVE, m_group_motormove);
	DDX_Control(pDX, IDC_GROUP_MOTOR, m_group_motor);
	DDX_Control(pDX, IDC_CMB_MOTOR_POS, m_cmb_motor_pos);
	DDX_Control(pDX, IDC_BTN_GUIDE2_ONFF, m_btn_guide2_onoff);
	DDX_Control(pDX, IDC_MSG_BUFFER4, m_msg_buffer4);
	DDX_Control(pDX, IDC_MSG_BUFFER3, m_msg_buffer3);
	DDX_Control(pDX, IDC_MSG_BUFFER2, m_msg_buffer2);
	DDX_Control(pDX, IDC_MSG_BUFFER1, m_msg_buffer1);
	DDX_Control(pDX, IDC_MSG_BUFFER_HS, m_msg_buffer_hs);
	DDX_Control(pDX, IDC_IO_GUIDE2_ONOFF, m_io_guide2_onoff);
	DDX_Control(pDX, IDC_IO_CLAMP2_ONOFF, m_io_clamp2_onoff);
	DDX_Control(pDX, IDC_IO_CLAMP1_ONOFF, m_io_clamp1_onoff);
	DDX_Control(pDX, IDC_CMB_TRAY_POS, m_cmb_tray_pos);
	DDX_Control(pDX, IDC_CMB_F_HS_POS, m_cmb_f_hs_pos);
	DDX_Control(pDX, IDC_CMB_BUFFER_POS, m_cmb_buffer_pos);
	DDX_Control(pDX, IDC_BTN_CLAMP2_ONOFF, m_btn_clamp2_onoff);
	DDX_Control(pDX, IDC_BTN_CLAMP1_ONOFF, m_btn_clamp1_onoff);
	DDX_Control(pDX, IDC_BTN_GO_TRAY_POS, m_btn_go_tray_pos);
	DDX_Control(pDX, IDC_BTN_GO_STEP_RBT_MOVE, m_btn_go_step_rbt_move);
	DDX_Control(pDX, IDC_BTN_GO_BUFFER_POS, m_btn_go_buffer_pos);
	DDX_Control(pDX, IDC_BTN_FINGER_SOL_ALL, m_btn_finger_sol_all);
	DDX_Control(pDX, IDC_BTN_FINGER_SOL_2, m_btn_finger_sol2);
	DDX_Control(pDX, IDC_BTN_FINGER_SOL_1, m_btn_finger_sol1);
	DDX_Control(pDX, IDC_BTN_PICKER_SOL_ALL, m_btn_picker_sol_all);
	DDX_Control(pDX, IDC_BTN_PICKER_SOL_2, m_btn_picker_sol2);
	DDX_Control(pDX, IDC_BTN_PICKER_SOL_1, m_btn_picker_sol1);
	DDX_Control(pDX, IDC_MSG_FINGER_SOL, m_msg_finger_sol);
	DDX_Control(pDX, IDC_MSG_PICKER_SOL, m_msg_picker_sol);
	DDX_Control(pDX, IDC_MSG_PICKER_2, m_msg_picker2);
	DDX_Control(pDX, IDC_MSG_PICKER_1, m_msg_picker1);
	DDX_Control(pDX, IDC_GROUP_BUFFER_RBT_MOVE, m_group_buffer_rbt_move);
	DDX_Control(pDX, IDC_GROUP_BUFFER, m_group_buffer);
	DDX_Control(pDX, IDC_GROUP_TRAY_POS_CLAMP_GUIDE, m_group_tray_pos_clamp_guide);
	DDX_Control(pDX, IDC_GROUP_RBT_STEP_MOVE, m_group_step_rbt_move);
	DDX_Control(pDX, IDC_GROUP_RBT_MOVE, m_group_picker_rbt_move);
	DDX_Control(pDX, IDC_GROUP_PICKER, m_group_picker);
	DDX_Control(pDX, IDC_BTN_BUFFER_FRONTHS_SOL1, m_btn_buffer_fronths_sol1);
	DDX_Control(pDX, IDC_BTN_BUFFER_FRONTHS_SOL2, m_btn_buffer_fronths_sol2);
	DDX_Control(pDX, IDC_BTN_BUFFER_FRONTHS_SOL3, m_btn_buffer_fronths_sol3);
	DDX_Control(pDX, IDC_BTN_BUFFER_FRONTHS_SOL4, m_btn_buffer_fronths_sol4);
	DDX_Control(pDX, IDC_BTN_GO_F_HS_POS, m_btn_go_f_hs_pos);


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Manual_FrontHs, CDialog)
	//{{AFX_MSG_MAP(CDialog_Manual_FrontHs)
	ON_BN_CLICKED(IDC_BTN_GO_TRAY_POS, OnBtnGoTrayPos)
	ON_BN_CLICKED(IDC_BTN_GO_BUFFER_POS, OnBtnGoBufferPos)
	ON_BN_CLICKED(IDC_BTN_CLAMP1_ONOFF, OnBtnClamp1Onoff)
	ON_BN_CLICKED(IDC_BTN_CLAMP2_ONOFF, OnBtnClamp2Onoff)
	ON_BN_CLICKED(IDC_BTN_GUIDE2_ONFF, OnBtnGuide2Onff)
	ON_BN_CLICKED(IDC_BTN_PICKER_SOL_1, OnBtnPickerSol1)
	ON_BN_CLICKED(IDC_BTN_PICKER_SOL_2, OnBtnPickerSol2)
	ON_BN_CLICKED(IDC_BTN_PICKER_SOL_ALL, OnBtnPickerSolAll)
	ON_BN_CLICKED(IDC_BTN_FINGER_SOL_1, OnBtnFingerSol1)
	ON_BN_CLICKED(IDC_BTN_FINGER_SOL_2, OnBtnFingerSol2)
	ON_BN_CLICKED(IDC_BTN_FINGER_SOL_ALL, OnBtnFingerSolAll)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_HOME, OnBtnHome)
	ON_BN_CLICKED(IDC_BTN_GO_STEP_RBT_MOVE, OnBtnGoStepRbtMove)
	ON_CBN_SELCHANGE(IDC_CMB_MOTOR_POS, OnSelchangeCmbMotorPos)
	ON_BN_CLICKED(IDC_BTN_BUFFER_FRONTHS_SOL1, OnBtnBufferFronthsSol1)
	ON_BN_CLICKED(IDC_BTN_BUFFER_FRONTHS_SOL2, OnBtnBufferFronthsSol2)
	ON_BN_CLICKED(IDC_BTN_BUFFER_FRONTHS_SOL3, OnBtnBufferFronthsSol3)
	ON_BN_CLICKED(IDC_BTN_BUFFER_FRONTHS_SOL4, OnBtnBufferFronthsSol4)
	ON_BN_CLICKED(IDC_BTN_GO_F_HS_POS, OnBtnGoFHsPos)
	//}}AFX_MSG_MAP
/////////////////////////////20121006
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
	ON_MESSAGE(WMU_CHILD_LBUTTON_DOWN, OnUserLButtonDown)
	ON_MESSAGE(WMU_CHILD_LBUTTON_UP, OnUserLButtonUp)
/////////////////////////////
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_FrontHs message handlers
BOOL CDialog_Manual_FrontHs::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int nMotorPos = 0;
	
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(20,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	m_load_combo_font = NULL;
	m_load_combo_font = new CFont;
	m_load_combo_font->CreateFont(55, 0, 0, 0, 300, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Arial");

//////////////////////////////////////////////////// 20121006
	m_motor_font = NULL;
	m_motor_font = new CFont;
	m_motor_font->CreateFont(20,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	m_d_data = 10;
////////////////////////////////////////////////////

	Init_GroupBox_Set();
	Init_Label();
	Init_Data();
	Init_Grid_Info();//20121006

	Init_Button();
	OnRead_IO_Check();
	OnSite_IO_Button_Status();
	OnSiteRbt_Button_Status();

	m_cmb_motor_pos.SetCurSel(nMotorPos);// .Init_Tab(this, m_n_axis);

	if(nMotorPos == 0)
	{
		m_n_axis = M_HS_F_RBT_Y;
	}
	else if(nMotorPos == 1)
	{
		m_n_axis = M_HS_F_RBT_Z;
	}
	else if(nMotorPos == 2)
	{
		m_n_axis = M_HS_F_STACKER_1;
	}
	else if(nMotorPos == 3)
	{
		m_n_axis = M_HS_F_STACKER_2;
	}
	else if(nMotorPos == 4)
	{
		m_n_axis = M_HS_F_STACKER_MOVE;
	}
	else
	{
		m_n_axis = M_WORK_BUFFER_1 + nMotorPos - 5;
	}	
	if (COMI.mn_motorbd_init_end)
	{
		SetTimer(TM_STATUS_CHECK, 100, NULL);
		SetTimer(TM_POSITION_CHECK, 100, NULL);
	}

	SetTimer(TMR_FRONTHS_IO_MONITOR_CMD, 50, NULL);		// IO 정보 읽기 타이머 생성
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////// 20121006
void CDialog_Manual_FrontHs::Init_Grid_Info()
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

	if (m_n_home == 0)
	{
		if (m_n_sd == 1)
		{
			if (m_n_minus_el == 1)
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
		if (m_n_minus_el == 1)
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
////////////////////////////////////////////////////

void CDialog_Manual_FrontHs::Init_GroupBox_Set()
{
	CSxLogFont ls_Logo(15,FW_SEMIBOLD,false,"맑은 고딕");

	m_group_buffer_rbt_move.SetFont(ls_Logo);;
	m_group_buffer_rbt_move.SetCatptionTextColor(RGB(145,25,0));
	m_group_buffer_rbt_move.SetFontBold(TRUE);

	m_group_buffer.SetFont(ls_Logo);
	m_group_buffer.SetCatptionTextColor(RGB(145,25,0));
	m_group_buffer.SetFontBold(TRUE);

	m_group_tray_pos_clamp_guide.SetFont(ls_Logo);
	m_group_tray_pos_clamp_guide.SetCatptionTextColor(RGB(145,25,0));
	m_group_tray_pos_clamp_guide.SetFontBold(TRUE);

	m_group_step_rbt_move.SetFont(ls_Logo);
	m_group_step_rbt_move.SetCatptionTextColor(RGB(145,25,0));
	m_group_step_rbt_move.SetFontBold(TRUE);

	m_group_picker_rbt_move.SetFont(ls_Logo);
	m_group_picker_rbt_move.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker_rbt_move.SetFontBold(TRUE);
	m_group_picker.SetFont(ls_Logo);
	m_group_picker.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker.SetFontBold(TRUE);

	m_group_motor.SetFont(ls_Logo);
	m_group_motor.SetCatptionTextColor(RGB(145,25,0));
	m_group_motor.SetFontBold(TRUE);

	m_group_motormove.SetFont(ls_Logo);
	m_group_motormove.SetCatptionTextColor(RGB(145,25,0));
	m_group_motormove.SetFontBold(TRUE);
}

void CDialog_Manual_FrontHs::Init_Label()
{
	int i;
	CString str_line;
	
	m_cmb_tray_pos.SetFont(m_load_combo_font);
	m_cmb_tray_pos.PostMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM) 60);
	
	for (i = 0; i < st_traybuffer[LEFTSINK_SITE].i_loader_row; i++)
	{
		str_line.Format("%02d", i + 1);		
		m_cmb_tray_pos.AddString(str_line);
	}
	m_cmb_tray_pos.SetCurSel(0);

	m_cmb_buffer_pos.SetFont(m_load_combo_font);
	m_cmb_buffer_pos.PostMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM) 60);
	
	for (i = 0; i < 4; i++)
	{
		str_line.Format("%02d", i + 1);		
		m_cmb_buffer_pos.AddString(str_line);
	}
	m_cmb_buffer_pos.SetCurSel(0);

	m_cmb_motor_pos.SetFont(m_p_font);
	m_cmb_motor_pos.PostMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM) 20);
	
	for (i = 0; i < 9; i++)
	{
		if(i == 0)
		{
			str_line.Format("%02d_FRONTHS_Y", i + 1);
		}
		else if(i == 1)
		{
			str_line.Format("%02d_FRONTHS_Z", i + 1);
		}
		else if(i == 2)
		{
			str_line.Format("%02d_STACKER1", i + 1);
		}
		else if(i == 3)
		{
			str_line.Format("%02d_STACKER2", i + 1);
		}
		else if(i == 4)
		{
			str_line.Format("%02d_STACKER_MOVE", i + 1);
		}
		else
		{
			str_line.Format("BUFFER_%02d", i - 4);
		}
		m_cmb_motor_pos.AddString(str_line);
	}

	m_cmb_motor_pos.SetCurSel(0);


	m_cmb_f_hs_pos.SetFont(m_p_font);
	m_cmb_f_hs_pos.PostMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM) 20);
	
	for (i = 0; i < 29; i++)
	{
		if(i == 0)
		{
			str_line.Format("Y_Safety");
		}
		else if(i == 1)
		{
			str_line.Format("Y_Heat Sink Start");
		}
		else if(i == 2)
		{
			str_line.Format("Y_Heat Sink End");
		}
		else if(i == 3 || i == 4 || i == 5 || i == 6)
		{
			str_line.Format("Y_WB%01d", i - 2);				
		}
		else if(i == 7)
		{
			str_line.Format("Z_Safety");
		}
		else if(i == 8)
		{
			str_line.Format("Z_Heat Sink Pick");
		}
		else if(i == 9 || i == 10 || i == 11 || i == 12)
		{
			str_line.Format("Z_WB%01d", i - 8);				
		}		
		else if(i == 13)
		{
			str_line.Format("Z_PCB Direction");
		}
		else if(i == 14)
		{
			str_line.Format("Stacker1_Safety");
		}
		else if(i == 15)
		{
			str_line.Format("Stacker1_Down Pos");
		}
		else if(i == 16)
		{
			str_line.Format("Stacker1_Up Pos");
		}
		else if(i == 17)
		{
			str_line.Format("Stacker1_Up Offset");
		}
		else if(i == 18)
		{
			str_line.Format("Stacker1_Down Offset");
		}
		else if(i == 19)
		{
			str_line.Format("Stacker1_P Limit");
		}
		else if(i == 20)
		{
			str_line.Format("Stacker2_Safety");
		}
		else if(i == 21)
		{
			str_line.Format("Stacker2_Down Pos");
		}
		else if(i == 22)
		{
			str_line.Format("Stacker2_Up Pos");
		}
		else if(i == 23)
		{
			str_line.Format("Stacker2_Up Offset");
		}
		else if(i == 24)
		{
			str_line.Format("Stacker2_Down Offset");
		}
		else if(i == 25)
		{
			str_line.Format("Stacker2_P Limit");
		}
		else if(i == 26)
		{
			str_line.Format("Stacker_Move_Safety");
		}
		else if(i == 27)
		{
			str_line.Format("Stacker_Move_Load Pos");
		}
		else if(i == 28)
		{
			str_line.Format("Stacker_Move_Unload Pos");
		}

		m_cmb_f_hs_pos.AddString(str_line);
	}

	m_cmb_f_hs_pos.SetCurSel(0);


	m_msg_picker_sol.SetFont(m_p_font);
	m_msg_picker_sol.SetWindowText(_T("Picker sol"));
	m_msg_picker_sol.SetCenterText();
	m_msg_picker_sol.SetColor(RGB(0,0,255));
	m_msg_picker_sol.SetGradientColor(RGB(0,0,0));
	m_msg_picker_sol.SetTextColor(RGB(255,255,255));

	m_msg_finger_sol.SetFont(m_p_font);
	m_msg_finger_sol.SetWindowText(_T("Finger sol"));
	m_msg_finger_sol.SetCenterText();
	m_msg_finger_sol.SetColor(RGB(0,0,255));
	m_msg_finger_sol.SetGradientColor(RGB(0,0,0));
	m_msg_finger_sol.SetTextColor(RGB(255,255,255));

	m_msg_picker1.SetFont(m_p_font);
	m_msg_picker1.SetWindowText(_T("#1"));
	m_msg_picker1.SetCenterText();
	m_msg_picker1.SetColor(RGB(0,0,255));
	m_msg_picker1.SetGradientColor(RGB(0,0,0));
	m_msg_picker1.SetTextColor(RGB(255,255,255));

	m_msg_picker2.SetFont(m_p_font);
	m_msg_picker2.SetWindowText(_T("#2"));
	m_msg_picker2.SetCenterText();
	m_msg_picker2.SetColor(RGB(0,0,255));
	m_msg_picker2.SetGradientColor(RGB(0,0,0));
	m_msg_picker2.SetTextColor(RGB(255,255,255));

	m_msg_buffer_hs.SetFont(m_p_font);
	m_msg_buffer_hs.SetWindowText(_T("hs sol"));
	m_msg_buffer_hs.SetCenterText();
	m_msg_buffer_hs.SetColor(RGB(0,0,255));
	m_msg_buffer_hs.SetGradientColor(RGB(0,0,0));
	m_msg_buffer_hs.SetTextColor(RGB(255,255,255));

	m_msg_buffer1.SetFont(m_p_font);
	m_msg_buffer1.SetWindowText(_T("buffer#1"));
	m_msg_buffer1.SetCenterText();
	m_msg_buffer1.SetColor(RGB(0,0,255));
	m_msg_buffer1.SetGradientColor(RGB(0,0,0));
	m_msg_buffer1.SetTextColor(RGB(255,255,255));

	m_msg_buffer2.SetFont(m_p_font);
	m_msg_buffer2.SetWindowText(_T("buffer#2"));
	m_msg_buffer2.SetCenterText();
	m_msg_buffer2.SetColor(RGB(0,0,255));
	m_msg_buffer2.SetGradientColor(RGB(0,0,0));
	m_msg_buffer2.SetTextColor(RGB(255,255,255));

	m_msg_buffer3.SetFont(m_p_font);
	m_msg_buffer3.SetWindowText(_T("buffer#3"));
	m_msg_buffer3.SetCenterText();
	m_msg_buffer3.SetColor(RGB(0,0,255));
	m_msg_buffer3.SetGradientColor(RGB(0,0,0));
	m_msg_buffer3.SetTextColor(RGB(255,255,255));

	m_msg_buffer4.SetFont(m_p_font);
	m_msg_buffer4.SetWindowText(_T("buffer#4"));
	m_msg_buffer4.SetCenterText();
	m_msg_buffer4.SetColor(RGB(0,0,255));
	m_msg_buffer4.SetGradientColor(RGB(0,0,0));
	m_msg_buffer4.SetTextColor(RGB(255,255,255));
}

void CDialog_Manual_FrontHs::Init_Data()
{
 	int i;

	mn_cmd_no = 0;
	mn_motor_no = 0;
	md_mot_start_pos = 0;
	mn_io_clamp1_onoff = 0;
	mn_io_clamp2_onoff = 0;
	mn_io_guide2_onoff = 0;

	for (i = 0; i < 2; i++)
	{
		mn_picker[i] = 0;
		mn_finger[i] = 0;
		mn_fronths_buffer[i] = 0;
		mn_fronths_buffer[i + 2] = 0;
	}

//////////////////////////////////////////////////// 20121006
	m_n_axis			= st_motor_info.n_part_axis[1][AXIS_NUM];
		
	m_str_axis_name = st_motor_info.str_part_axis_name[1][AXIS_NUM];
	m_n_minus_el	= st_motor_info.n_axis_minus_el[1][AXIS_NUM];
	m_n_plus_el		= st_motor_info.n_axis_plus_el[1][AXIS_NUM];
	m_n_home		= st_motor_info.n_axis_home[1][AXIS_NUM];
	m_n_sd			= st_motor_info.n_axis_sd[1][AXIS_NUM];
	m_n_direction	= st_motor_info.n_axis_direction[1][AXIS_NUM];
	m_n_account		= st_motor_info.n_axis_account[1][AXIS_NUM];
////////////////////////////////////////////////////	
}

void CDialog_Manual_FrontHs::Init_Button()
{
	short	shBtnColor = 30;
	CString strTmp;
	int nMotorPos = m_cmb_motor_pos.GetCurSel();
	
	if(nMotorPos == 0 || nMotorPos == 2 || nMotorPos == 3 || nMotorPos == 4)
	{
		m_btn_left.SetIcon(IDI_DN);
	}
	else if(nMotorPos == 1)
	{
		m_btn_left.SetIcon(IDI_UP);
	}
	else
	{
		m_btn_left.SetIcon(IDI_LEFT);
	}	

//	m_btn_left.SetIcon(IDI_DN);
	m_btn_left.SetTag(IDC_BTN_LEFT);
	m_btn_left.SetFont(m_p_font);
	m_btn_left.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_left.SetButtonColor(1);

	if(nMotorPos == 0 || nMotorPos == 2 || nMotorPos == 3 || nMotorPos == 4)
	{
		m_btn_right.SetIcon(IDI_UP);
	}
	else if(nMotorPos == 1)
	{
		m_btn_right.SetIcon(IDI_DN);
	}
	else
	{
		m_btn_right.SetIcon(IDI_RIGHT);
	}	

//	m_btn_right.SetIcon(IDI_UP);
	m_btn_right.SetTag(IDC_BTN_RIGHT);
	m_btn_right.SetFont(m_p_font);
	m_btn_right.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_right.SetButtonColor(1);

	m_btn_home.SetTag(IDC_BTN_HOME);
	m_btn_home.SetFont(m_p_font);
	m_btn_home.SetIcon(IDI_HOME);
	m_btn_home.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_home.SetButtonColor(1);

	m_btn_picker_sol1.SetFont(m_p_font);
	m_btn_picker_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_picker_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_picker_sol1.SetButtonColor(1);

	m_btn_picker_sol2.SetFont(m_p_font);
	m_btn_picker_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_picker_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_picker_sol2.SetButtonColor(1);

	m_btn_finger_sol1.SetFont(m_p_font);
	m_btn_finger_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_finger_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_finger_sol1.SetButtonColor(1);

	m_btn_finger_sol2.SetFont(m_p_font);
	m_btn_finger_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_finger_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_finger_sol2.SetButtonColor(1);

	m_btn_buffer_fronths_sol1.SetFont(m_p_font);
	m_btn_buffer_fronths_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_fronths_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_fronths_sol1.SetButtonColor(1);

	m_btn_buffer_fronths_sol2.SetFont(m_p_font);
	m_btn_buffer_fronths_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_fronths_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_fronths_sol2.SetButtonColor(1);

	m_btn_buffer_fronths_sol3.SetFont(m_p_font);
	m_btn_buffer_fronths_sol3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_fronths_sol3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_fronths_sol3.SetButtonColor(1);

	m_btn_buffer_fronths_sol4.SetFont(m_p_font);
	m_btn_buffer_fronths_sol4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_fronths_sol4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_fronths_sol4.SetButtonColor(1);

}

void CDialog_Manual_FrontHs::OnBtnPickerSol1() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_picker_sol1.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[0], IO_ON);
		mn_picker[0] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[0], IO_OFF);
		mn_picker[0] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_FrontHs::OnBtnPickerSol2() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_picker_sol2.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[1], IO_ON);
		mn_picker[1] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[1], IO_OFF);
		mn_picker[1] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
	
}

void CDialog_Manual_FrontHs::OnBtnPickerSolAll() 
{
	// TODO: Add your control notification handler code here
	int i;
	if (m_btn_picker_sol_all.GetCheck() == 0)//1번 기준
	{
		for (i = 0; i < PICKER_NUM; i++)
		{
			FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[i], IO_OFF); 
			mn_picker[i] = FALSE;
		}		
	}
	else
	{
		for (i = 0; i < PICKER_NUM; i++)
		{
			FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[i], IO_ON); 
			mn_picker[i] = TRUE;
		}		
	}
	
	OnSiteRbt_Button_Status();
	
}

void CDialog_Manual_FrontHs::OnBtnFingerSol1() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_finger_sol1.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_glipper_onoff[0], IO_ON);
		mn_finger[0] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_glipper_onoff[0], IO_OFF);
		mn_finger[0] = FALSE;
	}	
	OnSiteRbt_Button_Status();				
}

void CDialog_Manual_FrontHs::OnBtnFingerSol2() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_finger_sol2.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_glipper_onoff[1], IO_ON);
		mn_finger[1] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_glipper_onoff[1], IO_OFF);
		mn_finger[1] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
	
}

void CDialog_Manual_FrontHs::OnBtnFingerSolAll() 
{
	// TODO: Add your control notification handler code here
	int i;
	if (m_btn_finger_sol_all.GetCheck() == 0)//1번 기준
	{
		for (i = 0; i < PICKER_NUM; i++)
		{
			FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_glipper_onoff[i], IO_OFF); 
			mn_finger[i] = FALSE;
		}		
	}
	else
	{
		for (i = 0; i < PICKER_NUM; i++)
		{
			FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_glipper_onoff[i], IO_ON); 
			mn_finger[i] = TRUE;
		}		
	}
	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_FrontHs::OnBtnBufferFronthsSol1()
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_fronths_sol1.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf1_hs_clamp_onoff, IO_ON);
		mn_fronths_buffer[0] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf1_hs_clamp_onoff, IO_OFF);
		mn_fronths_buffer[0] = FALSE;
	}	
	OnSiteRbt_Button_Status();
	
}

void CDialog_Manual_FrontHs::OnBtnBufferFronthsSol2()
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_fronths_sol2.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf2_hs_clamp_onoff, IO_ON);
		mn_fronths_buffer[1] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
		mn_fronths_buffer[1] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_FrontHs::OnBtnBufferFronthsSol3()
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_fronths_sol3.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf3_hs_clamp_onoff, IO_ON);
		mn_fronths_buffer[2] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
		mn_fronths_buffer[2] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_FrontHs::OnBtnBufferFronthsSol4()
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_fronths_sol4.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf4_hs_clamp_onoff, IO_ON);
		mn_fronths_buffer[3] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
		mn_fronths_buffer[3] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_FrontHs::OnRead_IO_Check()
{
	int i;

	for(i=0; i<2; i++)
	{
		if(FAS_IO.get_in_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[i], IO_ON) == IO_ON)
		{
			mn_picker[i] = TRUE;
		}
		else
		{
			mn_picker[i] = FALSE;		
		}	
	}

	for(i=0; i<2; i++)
	{
		if(FAS_IO.get_in_bit(st_io.o_hs_fwd_rbt_glipper_onoff[i], IO_ON) == IO_ON)
		{
			mn_finger[i] = TRUE;
		}
		else
		{
			mn_finger[i] = FALSE;		
		}	
	}

	for(i=0; i<4; i++)
	{
		if(FAS_IO.get_in_bit(st_io.o_buf1_hs_clamp_onoff + (i*100), IO_ON) == IO_ON)
		{
			mn_fronths_buffer[i] = TRUE;
		}
		else
		{
			mn_fronths_buffer[i] = FALSE;		
		}	
	}

}

void CDialog_Manual_FrontHs::OnSiteRbt_Button_Status()//20121006
{
	short	shBtnColor = 30;
	
	if (mn_picker[0] < 0 || mn_picker[0] > 1) mn_picker[0] = TRUE;
	m_btn_picker_sol1.SetCheck(mn_picker[0]);
	if (mn_picker[0] == TRUE)
	{
		m_btn_picker_sol1.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_PICKER_SOL_1, "Up");
		m_btn_picker_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_picker_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_picker_sol1.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_picker_sol1.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_PICKER_SOL_1, "Down");
		m_btn_picker_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_picker_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_picker_sol1.SetButtonColor(1);

	}

	if (mn_picker[1] < 0 || mn_picker[1] > 1) mn_picker[1] = TRUE;
	m_btn_picker_sol2.SetCheck(mn_picker[1]);
	if (mn_picker[1] == TRUE)
	{
		m_btn_picker_sol2.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_PICKER_SOL_2, "Up");
		m_btn_picker_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_picker_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_picker_sol2.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_picker_sol2.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_PICKER_SOL_2, "Down");
		m_btn_picker_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_picker_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_picker_sol2.SetButtonColor(1);

	}

	if (mn_finger[0] < 0 || mn_finger[0] > 1) mn_finger[0] = TRUE;
	m_btn_finger_sol1.SetCheck(mn_finger[0]);
	if (mn_finger[0] == TRUE)
	{
		m_btn_finger_sol1.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_FINGER_SOL_1, "On");
		m_btn_finger_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_finger_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_finger_sol1.SetButtonColor(1);

	}	
	else
	{
		m_btn_finger_sol1.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_FINGER_SOL_1, "Off");
		m_btn_finger_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_finger_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_finger_sol1.SetButtonColor(1);

	}

	if (mn_finger[1] < 0 || mn_finger[1] > 1) mn_finger[1] = TRUE;
	m_btn_finger_sol2.SetCheck(mn_finger[1]);
	if (mn_finger[1] == TRUE)
	{
		m_btn_finger_sol2.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_FINGER_SOL_2, "On");
		m_btn_finger_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_finger_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_finger_sol2.SetButtonColor(1);
	}	
	else
	{
		m_btn_finger_sol2.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_FINGER_SOL_2, "Off");
		m_btn_finger_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_finger_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_finger_sol2.SetButtonColor(1);
	}

	if (mn_fronths_buffer[0] < 0 || mn_fronths_buffer[0] > 1) mn_fronths_buffer[0] = TRUE;
	m_btn_buffer_fronths_sol1.SetCheck(mn_fronths_buffer[0]);
	if (mn_fronths_buffer[0] == TRUE)
	{
		m_btn_buffer_fronths_sol1.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_FRONTHS_SOL1, "On");
		m_btn_buffer_fronths_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_fronths_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_fronths_sol1.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_fronths_sol1.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_FRONTHS_SOL1, "Off");
		m_btn_buffer_fronths_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_fronths_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_fronths_sol1.SetButtonColor(1);

	}

	if (mn_fronths_buffer[1] < 0 || mn_fronths_buffer[1] > 1) mn_fronths_buffer[1] = TRUE;
	m_btn_buffer_fronths_sol2.SetCheck(mn_fronths_buffer[1]);
	if (mn_fronths_buffer[1] == TRUE)
	{
		m_btn_buffer_fronths_sol2.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_FRONTHS_SOL2, "On");
		m_btn_buffer_fronths_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_fronths_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_fronths_sol2.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_fronths_sol2.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_FRONTHS_SOL2, "Off");
		m_btn_buffer_fronths_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_fronths_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_fronths_sol2.SetButtonColor(1);

	}

	if (mn_fronths_buffer[2] < 0 || mn_fronths_buffer[2] > 1) mn_fronths_buffer[2] = TRUE;
	m_btn_buffer_fronths_sol3.SetCheck(mn_fronths_buffer[2]);
	if (mn_fronths_buffer[2] == TRUE)
	{
		m_btn_buffer_fronths_sol3.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_FRONTHS_SOL3, "On");
		m_btn_buffer_fronths_sol3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_fronths_sol3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_fronths_sol3.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_fronths_sol3.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_FRONTHS_SOL3, "Off");
		m_btn_buffer_fronths_sol3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_fronths_sol3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_fronths_sol3.SetButtonColor(1);

	}

	if (mn_fronths_buffer[3] < 0 || mn_fronths_buffer[3] > 1) mn_fronths_buffer[3] = TRUE;
	m_btn_buffer_fronths_sol4.SetCheck(mn_fronths_buffer[3]);
	if (mn_fronths_buffer[3] == TRUE)
	{
		m_btn_buffer_fronths_sol4.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_FRONTHS_SOL4, "On");
		m_btn_buffer_fronths_sol4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_fronths_sol4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_fronths_sol4.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_fronths_sol4.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_FRONTHS_SOL4, "Off");
		m_btn_buffer_fronths_sol4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_fronths_sol4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_fronths_sol4.SetButtonColor(1);

	}
}

void CDialog_Manual_FrontHs::OnSite_Controls_Enable(BOOL iFlag)
{
	m_btn_go_tray_pos.EnableWindow(iFlag);
	m_btn_go_f_hs_pos.EnableWindow(iFlag);
	m_btn_go_buffer_pos.EnableWindow(iFlag);
	m_btn_go_step_rbt_move.EnableWindow(iFlag);
	m_btn_picker_sol1.EnableWindow(iFlag);
	m_btn_picker_sol2.EnableWindow(iFlag);
	m_btn_picker_sol_all.EnableWindow(iFlag);
	m_btn_finger_sol1.EnableWindow(iFlag);
	m_btn_finger_sol2.EnableWindow(iFlag);
	m_btn_finger_sol_all.EnableWindow(iFlag);
	m_btn_clamp1_onoff.EnableWindow(iFlag);
	m_btn_clamp2_onoff.EnableWindow(iFlag);
	m_btn_guide2_onoff.EnableWindow(iFlag);
	m_btn_buffer_fronths_sol1.EnableWindow(iFlag);
	m_btn_buffer_fronths_sol2.EnableWindow(iFlag);
	m_btn_buffer_fronths_sol3.EnableWindow(iFlag);
	m_btn_buffer_fronths_sol4.EnableWindow(iFlag);
}

void CDialog_Manual_FrontHs::OnSite_Step_Clear()
{
	mn_stop_req = FALSE;  // ESTOP 요청 플래그 초기화

	mn_cmd_no = 0;        // 실행 명령 번호 저장 변수 초기화

	mn_home_step = 0;     // 모터 HOMING 처리 스텝 저장 변수 초기화
	mn_move_step = 0;     // 모터 이동 처리 스텝 저장 변수 초기화
}

void CDialog_Manual_FrontHs::OnBtnGoTrayPos() 
{
	// TODO: Add your control notification handler code here
	int x, n_tray_y;
	int mn_FirstPicker_Y_Pos = 0;
	////////////////////// 20121107
	int	nRet;	
	int n_response;	
	CDialog_Message  msg_dlg;
	////////////////////// 
	
	n_tray_y = m_cmb_tray_pos.GetCurSel();
	
	if (n_tray_y < 0)	
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg,"[Frontward Heat Sink RBT] can not move.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
		}
		return;
	}
	
	OnSite_Controls_Enable(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
	
	if (st_handler.mn_menu_lock != TRUE)
		st_handler.mn_menu_lock = TRUE;
	// **************************************************************************
	
	OnSite_Step_Clear();				// 모터 동작 처리 스텝 초기화 함수
	
	// **************************************************************************
	// 모터 이동 위치 설정한다                                                   
	// **************************************************************************

	////////////////////// 20121107
	nRet = COMI.Check_SingleMove(M_HS_F_RBT_Z, Z_LD_SAFETY_UP);
	if (nRet != CTLBD_RET_GOOD)
	{
		st_msg.str_fallacy_msg = _T("Z축이 Safety 위치가 아닙니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("Safety is not the position of the Z-axis");
		}
		
		n_response = msg_dlg.DoModal();
		OnSite_Controls_Enable(TRUE);  // 모든 컨트롤 화면 출력 제어 함수
		st_handler.mn_menu_lock = FALSE;
		return;
	}
	else
	{
	}
	//////////////////////

	
	mn_cmd_no = MOT_MOVE;					// 실행 명령 번호 설정 [모터 이동 명령]
	mn_motor_no = M_HS_F_RBT_Y;

	for(x=0 ; x<PICKER_NUM; x++) 
	{
		if(st_modulemap.nLefthsRobotPicker[D_EXIST][x] == DVC_NO && st_picker.n_heat_leftldrbt_enable[x] == YES)
		{
			mn_FirstPicker_Y_Pos = x;
			break;
		}
	}
	md_mot_start_pos = Func.XYRobot_MovePosValCheck(LEFTSINK_SITE, 0, mn_FirstPicker_Y_Pos, n_tray_y); // 트레이에서 찾은 위치로 레일을 이동시킨다
	
	SetTimer(TMR_FRONTHSMOT_CMD, 10, NULL);		// 모터 동작 제어 타이머
	// **************************************************************************		
}

void CDialog_Manual_FrontHs::OnBtnGoBufferPos() 
{
	// TODO: Add your control notification handler code here
	int n_tray_y;
	int mn_FirstPicker_Y_Pos = 0;
	////////////////////// 20121107
	int	nRet, nRet1, nRet2, nRet3, nRet4;	
	int n_response;	
	CDialog_Message  msg_dlg;
	////////////////////// 
	
	n_tray_y = m_cmb_buffer_pos.GetCurSel();
	
	if (n_tray_y < 0)	
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg,"[Buffer RBT] can not move.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
		}
		return;
	}
	
	OnSite_Controls_Enable(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
	
	if (st_handler.mn_menu_lock != TRUE)
		st_handler.mn_menu_lock = TRUE;
	// **************************************************************************
	
	OnSite_Step_Clear();				// 모터 동작 처리 스텝 초기화 함수
	
	// **************************************************************************
	// 모터 이동 위치 설정한다                                                   
	// **************************************************************************

	////////////////////// 20121107
	nRet = COMI.Check_SingleMove(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP]);
	nRet1 = COMI.Check_SingleMove(M_HS_B_RBT_Z, Z_LD_SAFETY_UP);
	nRet2 = COMI.Check_SingleMove(M_HS_F_RBT_Z, Z_LD_SAFETY_UP);
	nRet3 = COMI.Check_SingleMove(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP]);
	nRet4 = COMI.Check_SingleMove(M_RBT_SORTER_Z, Z_SORT_SAFETY_UP);

	if (nRet == CTLBD_RET_GOOD && nRet1 == CTLBD_RET_GOOD && nRet2 == CTLBD_RET_GOOD && 
		nRet3 == CTLBD_RET_GOOD && nRet1 == CTLBD_RET_GOOD)
	{
	}
	else
	{
		st_msg.str_fallacy_msg = _T("모듈 && Front/Back HS && 모듈 Clamp && Sorter 로봇 Z축이 Safety 위치가 아닙니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("Modules && Front / Back HS && module Safety Clamp && Sorter Z axis position is not a robot");
		}
		
		n_response = msg_dlg.DoModal();
		OnSite_Controls_Enable(TRUE);  // 모든 컨트롤 화면 출력 제어 함수
		st_handler.mn_menu_lock = FALSE;
		return;
	}
	//////////////////////

	mn_cmd_no = MOT_MOVE;					// 실행 명령 번호 설정 [모터 이동 명령]
	mn_motor_no = M_WORK_BUFFER_1 + n_tray_y;

	md_mot_start_pos = st_motor[mn_motor_no].md_pos[WORK_LDLEFTHS];
	
	SetTimer(TMR_FRONTHSMOT_CMD, 10, NULL);		// 모터 동작 제어 타이머
	// **************************************************************************			
}

void CDialog_Manual_FrontHs::OnSite_RadioButton_Set()
{
	m_io_clamp1_onoff.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_io_clamp2_onoff.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_io_guide2_onoff.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
}

void CDialog_Manual_FrontHs::OnSite_IO_Status_Read()
{
	if (FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_fwd_chk, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_bwd_chk, IO_OFF) == IO_OFF)		mn_io_clamp1_onoff = TRUE;
	else																			mn_io_clamp1_onoff = FALSE;

	if (FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_rail_fwd_chk, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_rail_bwd_chk, IO_OFF) == IO_OFF)		mn_io_clamp2_onoff = TRUE;
	else																			mn_io_clamp2_onoff = FALSE;

	if (FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON &&
		FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_tray_clamp_off_chk, IO_OFF) == IO_OFF)	mn_io_guide2_onoff = TRUE;
	else																			mn_io_guide2_onoff = FALSE;

}

void CDialog_Manual_FrontHs::OnSite_IO_Button_Status()
{
	// **************************************************************************
	// IO 센서 입력 상태 정보 저장 변수
	// **************************************************************************
	m_io_clamp1_onoff.SetLedState(mn_io_clamp1_onoff);
	m_io_clamp2_onoff.SetLedState(mn_io_clamp2_onoff);
	m_io_guide2_onoff.SetLedState(mn_io_guide2_onoff);
}

void CDialog_Manual_FrontHs::OnBtnClamp1Onoff() 
{
	// TODO: Add your control notification handler code here
	int nRet_1;

	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_tray_chk, IO_OFF);
	if(nRet_1 == IO_ON && mn_io_clamp1_onoff == TRUE)
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg,"Stacker1 레일에 트레이가 있습니다.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker1");
			}

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
		}
		return;	
	}
	if(mn_io_clamp1_onoff == TRUE)
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker1_rail_fwd_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker1_rail_bwd_onoff, IO_ON);
		SetDlgItemText(IDC_BTN_CLAMP1_ONOFF, "On");
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker1_rail_fwd_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker1_rail_bwd_onoff, IO_OFF);
		SetDlgItemText(IDC_BTN_CLAMP1_ONOFF, "Off");
	}
	
}

void CDialog_Manual_FrontHs::OnBtnClamp2Onoff() 
{
	// TODO: Add your control notification handler code here
	int nRet_1;

	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_rail_tray_chk, IO_OFF);
	if(nRet_1 == IO_ON && mn_io_clamp2_onoff == TRUE)
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg,"Stacker2 레일에 트레이가 있습니다.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker2");
			}

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
		}
		return;	
	}

	if(mn_io_clamp2_onoff == TRUE)
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_rail_fwd_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_rail_bwd_onoff, IO_ON);
		SetDlgItemText(IDC_BTN_CLAMP2_ONOFF, "On");
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_rail_fwd_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_rail_bwd_onoff, IO_OFF);
		SetDlgItemText(IDC_BTN_CLAMP2_ONOFF, "Off");
	}		
}

void CDialog_Manual_FrontHs::OnBtnGuide2Onff() 
{
	// TODO: Add your control notification handler code here
	int nRet_1;

	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_rail_tray_chk, IO_OFF);
	if(nRet_1 == IO_ON && mn_io_clamp2_onoff == TRUE)
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg,"Stacker2 레일에 트레이가 있습니다.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker2");
			}

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
		}
		return;	
	}
	if(mn_io_guide2_onoff == TRUE)
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_tray_clamp_onoff, IO_OFF);
		SetDlgItemText(IDC_BTN_GUIDE2_ONFF, "On");
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_tray_clamp_onoff, IO_ON);
		SetDlgItemText(IDC_BTN_GUIDE2_ONFF, "Off");
	}	
}

void CDialog_Manual_FrontHs::OnTimer(UINT nIDEvent) 
{
	int n_response,n_check;
//	int nRet[5];

	CDialog_Message msg_dlg;

	if (nIDEvent == TMR_FRONTHSMOT_CMD) 
	{
		// **************************************************************************
		// 모션 보드 초기화 작업 완료 유무 검사한다                                  
		// **************************************************************************
		if (st_handler.mn_mot_brd_initial != TRUE)  
		{
			KillTimer(TMR_FRONTHSMOT_CMD) ;  // 타이머 삭제
			mn_cmd_no = 0;				// 실행 명령 번호 저장 변수 초기화

			st_other.str_fallacy_msg = _T("Initialize first motion board...error...");
			n_response = msg_dlg.DoModal();

			if (n_response == IDOK) 
			{
				n_check = CTLBD_RET_ERROR;
			}
			else
			{
				n_check = CTLBD_RET_ERROR;
			}
		}
		// **************************************************************************

		if (mn_cmd_no == MOT_HOME)
		{
			n_check = OnSite_Homing_Excution();		// 모터 HOMING 처리 함수

			if(n_check == CTLBD_RET_GOOD)
			{
				KillTimer(TMR_FRONTHSMOT_CMD) ;  // 타이머 삭제 //
				if(st_handler.mn_home_state[mn_motor_no] != TRUE)
				{
					st_handler.mn_home_state[mn_motor_no] = TRUE;
				}				
				mn_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화 //				
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================

				OnSite_Controls_Enable(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
			}
			else if(n_check == CTLBD_RET_ERROR)
			{
				KillTimer(TMR_FRONTHSMOT_CMD) ;  // 타이머 삭제 //
				
				mn_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화 //
				
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
				OnSite_Controls_Enable(TRUE);  // 모든 컨트롤 화면 출력 제어 함수 //
			}
		}
		else if (mn_cmd_no == MOT_MOVE)
		{
			n_check = OnSite_Move_Excution();			// 모터 이동 명령 처리 함수
			if (n_check == CTLBD_RET_GOOD)   
			{
				KillTimer(TMR_FRONTHSMOT_CMD) ;  // 타이머 삭제
				mn_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화

				// ******************************************************************
				if (st_handler.mn_menu_lock != FALSE)
					st_handler.mn_menu_lock = FALSE;
				// ******************************************************************

				OnSite_Controls_Enable(TRUE);  // 모든 컨트롤 화면 출력 제어 함수
			}
			else if (n_check == CTLBD_RET_ERROR) 
			{
				KillTimer(TMR_FRONTHSMOT_CMD) ;  // 타이머 삭제

				mn_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화

				if (st_handler.mn_menu_lock != FALSE)
					st_handler.mn_menu_lock = FALSE;
				// ******************************************************************

				OnSite_Controls_Enable(TRUE);  // 모든 컨트롤 화면 출력 제어 함수
			}	
		}
		else 
		{
			KillTimer(TMR_FRONTHSMOT_CMD) ;  // 타이머 삭제
		}


	}
	else if (nIDEvent == TMR_FRONTHS_IO_MONITOR_CMD) 
	{
		OnSite_IO_Status_Read();
	}
	else if (nIDEvent == TMR_FRONTHSSTEPMOT_CMD) 
	{
		Run_Init();
		if (st_handler.mn_init_state[INIT_RBT_LEFT_SINK] == CTL_YES)
		{
			KillTimer(TMR_FRONTHSSTEPMOT_CMD);
			OnSite_Controls_Enable(TRUE);  // 모든 컨트롤 화면 출력 제어 함수			
		}
	}
	else if (nIDEvent == TM_STATUS_CHECK) 
	{
		MotorStatusCheck();
	}
	else if (nIDEvent == TM_POSITION_CHECK) 
	{
		MotorPositionCheck();
	}


	CDialog::OnTimer(nIDEvent);
}

void CDialog_Manual_FrontHs::Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i;

	for (i = 0; i < PICKER_NUM; i++)
	{
		if(PickerInfo[D_EXIST][i] == YES)
		{
			FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[i], OnOff);
		}
		//else
		//{
		//	FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[i], !OnOff);
		//}
		b_PickerFlag[i] = FALSE;
		l_PickerWaitTime[i][0] = GetCurrentTime();
	}
}

int CDialog_Manual_FrontHs::Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i, FuncRet = RET_PROCEED;
	int RetVal[PICKER_NUM] = {0,};
	char jamcode[10] = {0,};
	CString str;
	
	if(st_basic.n_mode_device != CTL_YES)
	{
	}

	for(i = 0 ; i < PICKER_NUM ; i++)
	{
		RetVal[i] = NO;

		if(OnOff == IO_ON) //Picker ON (Down이라면..)
		{
			if(PickerInfo[D_EXIST][i] == YES) //동작 시킨 피커면!
			{
				if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[i], IO_ON) == IO_ON
					&& FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_up_chk[i], IO_OFF) == IO_OFF)
				{
					b_PickerFlag[i] = TRUE;
					l_PickerWaitTime[i][0] = GetCurrentTime();   
				}
				else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[i], IO_ON) == IO_ON
					&& FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_up_chk[i], IO_OFF) == IO_OFF)
				{
					l_PickerWaitTime[i][1] = GetCurrentTime();  
					l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
					
					if(l_PickerWaitTime[i][2] < 0)
					{
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[42] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_PickerWaitTime[i][1] = GetCurrentTime();  
					l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
					if(l_PickerWaitTime[i][2] < 0)
					{
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					
					if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[42] + (i*2))
					{
						//420200 1 40 "Heat Sink Picker #1 Down Check Error.[PS0308]."					
						sprintf(jamcode, "42020%d", i);
						CTL_Lib.Alarm_Error_Occurrence(2220, CTL_dWARNING, jamcode);

						FuncRet = RET_ERROR;
					}
				}
			}
			else RetVal[i] = YES;
		}
		else //Picker OFF (UP이라면..)
		{
			if(PickerInfo[D_EXIST][i] == YES) //동작 시킨 피커면!
			{
				if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[i], IO_OFF) == IO_OFF
					&& FAS_IO.Get_In_Bit(st_io.i_hs_fwd_picker_up_chk[i]) == IO_ON)
				{
					b_PickerFlag[i] = TRUE;
					l_PickerWaitTime[i][0] = GetCurrentTime();   
				}
				else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[i], IO_OFF) == IO_OFF
					&& FAS_IO.Get_In_Bit(st_io.i_hs_fwd_picker_up_chk[i]) == IO_ON)
				{
					l_PickerWaitTime[i][1] = GetCurrentTime();  
					l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
					
					if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[42] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_PickerWaitTime[i][1] = GetCurrentTime();  
					l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
					if(l_PickerWaitTime[i][2] < 0)
					{
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					
					if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[42] + (i*2))
					{
						if(st_handler.cwnd_list != NULL)
						{
							str = _T("[UPICKER Error] Picker up sensor check error.");
							sprintf(st_msg.c_normal_msg,"%s", str);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
							Func.On_LogFile_Add(0, str);
							Func.On_LogFile_Add(99, str);
						}
						//420100 1 40 "Heat Sink Picker #1 Up Check Error.[PS0300]."				
						sprintf(jamcode, "42010%d", i);
						CTL_Lib.Alarm_Error_Occurrence(2221, CTL_dWARNING, jamcode);

						FuncRet = RET_ERROR;
					}
				}
			}
			else RetVal[i] = YES;
		}
	}
	
	if(RetVal[0] == YES && RetVal[1] == YES) 
	{
		FuncRet = RET_GOOD;
	}

	return FuncRet;
}

void CDialog_Manual_FrontHs::Set_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i;

	for (i = 0; i < PICKER_NUM; i++)
	{
		if(FingerInfo[D_EXIST][i] == YES)
		{
			FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_glipper_onoff[i], OnOff);
		}
		//else
		//{
		//	FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_glipper_onoff[i], !OnOff);
		//}
		b_FingerFlag[i] = false;
		l_FingerWaitTime[i][0] = GetCurrentTime();
	}
}

int CDialog_Manual_FrontHs::Get_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i, FuncRet = RET_PROCEED;
	int RetVal[PICKER_NUM] = {0,};
	char jamcode[10] = {0,};
	 
	for(i = 0 ; i < PICKER_NUM ; i++)
	{
		RetVal[i] = NO;

		if(OnOff == IO_ON) //Finger ON (place라면..)
		{
			if(FingerInfo[D_EXIST][i] == YES) //동작 시킨 핑거면!
			{
				//어짜피 센서가 없으므로 일정 시간 뒤에 동작 완료로 본다!!
				if(b_FingerFlag[i] == FALSE)
				{
					b_FingerFlag[i] = TRUE;
					l_FingerWaitTime[i][0] = GetCurrentTime();   
				}				
				else if(b_FingerFlag[i] == TRUE)
				{
					l_FingerWaitTime[i][1] = GetCurrentTime();  
					l_FingerWaitTime[i][2] = l_FingerWaitTime[i][1] - l_FingerWaitTime[i][0] ;
					
					if(l_FingerWaitTime[i][2] > st_wait.n_on_wait_time[42] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_FingerWaitTime[i][1] = GetCurrentTime();  
					l_FingerWaitTime[i][2] = l_FingerWaitTime[i][1] - l_FingerWaitTime[i][0];
					if(l_FingerWaitTime[i][2] < 0)
					{
						l_FingerWaitTime[i][0] = GetCurrentTime();
					}
					
					if(l_FingerWaitTime[i][2] > st_wait.n_limit_wait_time[42] + (i*2))
					{
						//420500 1 40 "Heat Sink Picker #1 Module On Check Error.[PS0400]."
						sprintf(jamcode, "42050%d", i);
						alarm.mn_count_mode = 0;	
						alarm.mn_type_mode = eWARNING; 
						alarm.mn_alarm_assign_section = 188;

						FuncRet = RET_ERROR;
					}
				}
			}
			else RetVal[i] = YES;
		}
		else //Finger OFF (Pick이라면..)
		{
			if(FingerInfo[D_EXIST][i] == YES)
			{
				//어짜피 센서가 없으므로 일정 시간 뒤에 동작 완료로 본다!!
				if(b_FingerFlag[i] == FALSE)
				{
					b_FingerFlag[i] = TRUE;
					l_FingerWaitTime[i][0] = GetCurrentTime();   
				}				
				else if(b_FingerFlag[i] == TRUE)
				{
					l_FingerWaitTime[i][1] = GetCurrentTime();  
					l_FingerWaitTime[i][2] = l_FingerWaitTime[i][1] - l_FingerWaitTime[i][0];
					
					if(l_FingerWaitTime[i][2] > st_wait.n_on_wait_time[42] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_FingerWaitTime[i][1] = GetCurrentTime();  
					l_FingerWaitTime[i][2] = l_FingerWaitTime[i][1] - l_FingerWaitTime[i][0];
					if(l_FingerWaitTime[i][2] < 0)
					{
						l_FingerWaitTime[i][0] = GetCurrentTime();
					}
					
					if(l_FingerWaitTime[i][2] > st_wait.n_limit_wait_time[42] + (i*2))
					{
						//410600 1 40 "Heat Sink Picker #1 Module Off Check Error.[PS0400]."				
						sprintf(jamcode, "41060%d", i);
						alarm.mn_count_mode = 0;	
						alarm.mn_type_mode = eWARNING; 
						alarm.mn_alarm_assign_section = 188;

						FuncRet = RET_ERROR;
					}
				}
			}
			else RetVal[i] = YES;
		}
	}
	
	if(RetVal[0] == YES && RetVal[1] == YES) 
	{
		FuncRet = RET_GOOD;
	}

	return FuncRet;
}
void CDialog_Manual_FrontHs::Run_Init()
{
	int i,nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	
	if(st_handler.mn_init_state[INIT_RBT_LEFT_SINK] != CTL_NO) return;

	switch(InitStep)
	{
	case 0:
		for (i = 0; i < PICKER_NUM; i++)
		{
			ActionPicker[D_EXIST][i] = YES;
		}
		Set_FingerOnOff(IO_OFF, ActionPicker);
		InitStep = 10;
		break;

	case 10:
		nRet_1 = Get_FingerOnOff(IO_OFF, ActionPicker);

		if(nRet_1 == RET_GOOD)
		{
			InitStep = 100;
		}
		else if(nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			InitStep = 10;
		}
		break;

	case 100:
		Set_PickerUpDown(IO_OFF, ActionPicker);
		InitStep = 200;
		break;

	case 200:
		// *************************************************************
		//  UP 시킨 실린더를 CHECK한다.                                 
		// *************************************************************
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);

		if (nRet_1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			InitStep = 1000;
		}
		else if (nRet_1 == RET_ERROR)
		{
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Do not Picker Up.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}

		}
		break;

	case 1000:		//z axis부터 homecheck 하자!!
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_HS_F_RBT_Z, st_motor[M_HS_F_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			l_until_wait_time[0] = GetCurrentTime();
			InitStep = 1100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2101, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;


	case 1100:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		if(l_until_wait_time[2] < 0)	l_until_wait_time[0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
		if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) 	break;

		nRet_1 = RET_GOOD;
		for (i = 0; i < PICKER_NUM; i++)
		{
			if(ActionPicker[D_EXIST][i] == DVC_YES && st_picker.n_heat_leftldrbt_enable[i] == YES)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_fwd_dvc_chk[i], IO_ON) == IO_OFF)
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Frontward Heat Sink Picker #1 Module On Check Error.");
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					nRet_1 = RET_ERROR;
					break;
				}
			}
		}

		if(nRet_1 == RET_GOOD)
		{
			InitStep = 2000;	
		}
		break;
		
	case 2000:
		nRet_1 = COMI.HomeCheck_Mot(M_HS_F_RBT_Y, st_motor[M_HS_F_RBT_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 3000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(COMI.mc_alarmcode,"%s", alarm.mstr_code);
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2102, st_work.mn_run_status, COMI.mc_alarmcode);
			}

		}
		break;


	case 3000:
		st_handler.mn_init_state[INIT_RBT_LEFT_SINK] = CTL_YES;
		InitStep = 0;
		break;		
	}
}
BOOL CDialog_Manual_FrontHs::PreTranslateMessage(MSG* pMsg) 
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
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialog_Manual_FrontHs::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	mn_stop_req = TRUE;
}

int CDialog_Manual_FrontHs::OnSite_Move_Excution()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet_1 = CTLBD_RET_PROCEED, n_chk;	
	
	// **************************************************************************
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// **************************************************************************
	if (mn_stop_req == TRUE)
	{
		if (mn_move_step < 100)
			mn_move_step = 100;
	}
	// **************************************************************************
	
	switch(mn_move_step)
	{
	case 0 :
		// **********************************************************************
		// 모터 특정 위치로 이동시킨다. (이동위치 세팅~)                         
		// **********************************************************************
		n_chk = CTL_Lib.Single_Move(mn_motor_no, md_mot_start_pos, st_basic.nManualSpeed);
		if (n_chk == CTLBD_RET_GOOD)
		{
			mn_move_step = 10;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_move_step = 0; 
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			mn_move_step = 130;
		}
		// **********************************************************************
		break;

	case 10 :
		n_chk = COMI.Check_SingleMove(mn_motor_no, md_mot_start_pos);

		if (n_chk == CTLBD_RET_GOOD)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_normal_msg, "[FRONT HS SITE] Motor completed transfer.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
			}

			if (mn_stop_req != FALSE)	mn_stop_req = FALSE;
			mn_move_step = 0;
			nFuncRet = CTLBD_RET_GOOD;
		}
		else if (n_chk == CTLBD_RET_ERROR)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				//st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Motor failed to move.");
				sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Motor failed to move.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
		}
		else if (n_chk == CTLBD_RET_SAFETY)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				//st_msg.mstr_abnormal_msg = _T("[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
				sprintf(st_msg.c_abnormal_msg, "[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
		}	
		break;

	case 100 :
		// **********************************************************************
		// 특정 축 긴급 정지 이벤트로 모터 정지한다                              
		// **********************************************************************
		n_chk = COMI.Set_MotStop(0, mn_motor_no);
		
		if (n_chk == TRUE)
			mn_move_step = 110;
		else 
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				st_other.str_abnormal_msg = _T("[FRONT HS SITE] Though canceled transfer of motor, error happened.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			mn_move_step = 120;
		}
		// **********************************************************************
		
		break;
	case 110 :
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_op_msg = _T("[FRONT HS SITE] Motor shut down during transfer.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청
		}

		if (mn_stop_req != FALSE)
			mn_stop_req = FALSE;  // ESTOP 요청 플래그 초기화

		mn_move_step = 0;
		nFuncRet = CTLBD_RET_GOOD;
		break;

	case 120 :
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_abnormal_msg = _T("[FRONT HS SITE] Transfer command of motor failed.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}

		if (mn_stop_req != FALSE)
			mn_stop_req = FALSE;  // ESTOP 요청 플래그 초기화

		mn_move_step = 0; 
		nFuncRet = CTLBD_RET_ERROR;
		break;

	case 130 :
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_abnormal_msg = _T("[FRONT HS SITE] Transfer command of motor failed.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}

		if (mn_stop_req != FALSE)
			mn_stop_req = FALSE;  // ESTOP 요청 플래그 초기화

		mn_move_step = 0; 
		nFuncRet = CTLBD_RET_ERROR;
		break;
	}

	return nFuncRet;
}

void CDialog_Manual_FrontHs::OnBtnHome() 
{
	CString strTmp;
	int nMotorPos = 0;

	OnSite_Controls_Enable(FALSE);  // 모든 컨트롤 화면 출력 제어 함수

	if (st_handler.mn_menu_lock != TRUE)
	{
		st_handler.mn_menu_lock = TRUE;
	}
	// ==============================================================================
	
	
	OnSite_Step_Clear();
	
	mn_cmd_no = HOMING;

	nMotorPos = m_cmb_motor_pos.GetCurSel();

	if(nMotorPos == 0)
	{
		mn_motor_no = M_HS_F_RBT_Y;
	}
	else if(nMotorPos == 1)
	{
		mn_motor_no = M_HS_F_RBT_Z;
	}
	else if(nMotorPos == 2)
	{
		mn_motor_no = M_HS_F_STACKER_1;
	}
	else if(nMotorPos == 3)
	{
		mn_motor_no = M_HS_F_STACKER_2;
	}
	else if(nMotorPos == 4)
	{
		mn_motor_no = M_HS_F_STACKER_MOVE;
	}
	else
	{
		mn_motor_no = M_WORK_BUFFER_1 + nMotorPos - 5;
	}	

	// ==============================================================================
	// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
	// ==============================================================================
	st_handler.mn_home_state[mn_motor_no] = CTL_READY;	// 모터 HOMING 완료 플래그
	// ==============================================================================
	
	st_motor[mn_motor_no].mn_home_step	= 0;
	COMI.mn_home_step[mn_motor_no] = 0;

	strTmp.Format("[Motor] (%d)_Axis Home Move", mn_motor_no);
	Func.On_LogFile_Add(99, strTmp);
	
	SetTimer(TMR_FRONTHSMOT_CMD, 100, NULL);			// 모터 동작 제어 타이머	
}

// void CDialog_Manual_FrontHs::OnBtnHome() 
// {
// 	CString strTmp;
// 
// 	OnButtonControl(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
// 	
// 	// ==============================================================================
// 	// 메뉴 사용 불가능하도록 플래그 설정한다                                    
// 	// -> 현재 모터 동작 중에 다른 메뉴 모터 동작시키면 오동작할 수 있기 때문이다
// 	// -> st_handler.i_menu_lock   : 메뉴 사용 가능 상태 플래그                  
// 	//  : 메인 프레임의 메뉴 전환 부분에서 이 플래그 검사하게 된다               
// 	// ==============================================================================
// 	if (st_handler.mn_menu_lock != TRUE)
// 	{
// 		st_handler.mn_menu_lock = TRUE;
// 	}
// 	// ==============================================================================
// 	
// 	// ==============================================================================
// 	// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
// 	// ==============================================================================
// 	st_handler.mn_home_state[m_n_axis] = CTL_READY;	// 모터 HOMING 완료 플래그
// 	// ==============================================================================
// 	
// 	MotorMoveStepClear();							// 모터제어 함수 스텝 초기화
// 	
// 	m_n_cmd_motor_no						= m_n_axis;					// 모터 번호 설정
// 	m_n_cmd_no								= HOMING;							// 실행 명령 번호 설정 [모터 이동 명령]
// 	m_str_cmd_motor							= m_str_axis_name;
// 	
// 	strTmp.Format("[Motor] (%s)_Axis Home Move", m_str_axis_name);
// 	Func.On_LogFile_Add(99, strTmp);
// 
// 	st_motor[m_n_cmd_motor_no].mn_home_step	= 0;
// 	COMI.mn_home_step[m_n_cmd_motor_no] = 0;
// 	
// 	SetTimer(TM_HOME_CHECK, 100, NULL);			// 모터 동작 제어 타이머
//}

int CDialog_Manual_FrontHs::OnSite_Homing_Excution()
{
	int n_home_flag = CTLBD_RET_PROCEED;
	int nRet;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다
	// =============================================================================
	if (mn_stop_req){
		if (mn_home_step < 100){
			mn_home_step = 100;
		}
	}
	// =============================================================================
	if(FAS_IO.get_in_bit(st_io.i_stacker_light_curtain_chk, IO_ON) == IO_ON ||
		FAS_IO.get_in_bit(st_io.i_vision_light_curtain_chk, IO_ON) == IO_ON)
	{
		COMI.Set_MotStop(0, mn_motor_no);
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Error] Area sensor is detected. Motor stop. ReWork Please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		COMI.mn_home_step[mn_motor_no] = 0;
		mn_home_step = 100;
		return n_home_flag;
	}

	switch(mn_home_step)
	{
		case 0 :
			nRet = COMI.HomeCheck_Mot(mn_motor_no, st_motor[mn_motor_no].mn_homecheck_method, MOT_TIMEOUT);

			if (nRet == CTLBD_RET_GOOD)				// 정상적으로 Home Check가 끝났을 경우.
			{
				if (mn_stop_req != FALSE)
					mn_stop_req = FALSE;		// ESTOP 요청 플래그 초기화
				
				mn_home_step = 0; 
				n_home_flag = CTLBD_RET_GOOD;
			}
			else if (nRet == CTLBD_RET_ERROR)		// Home Check에 실패 했을 경우.
			{
				if (mn_stop_req != FALSE)
					mn_stop_req = FALSE;		// ESTOP 요청 플래그 초기화
				
				mn_home_step = 0; 
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
				mn_home_step = 0; 
				n_home_flag = CTLBD_RET_ERROR;
			}
			break;

		case 100 :
			// =============================================================================
			// 특정 축 긴급 정지 이벤트로 모터 정지한다
			// =============================================================================
			nRet = COMI.Set_MotStop(0, mn_motor_no);

			if (nRet == CTLBD_RET_GOOD)
			{
				mn_home_step = 110;
			}
			else 
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					//st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Though canceled transfer of motor, error happened.");
					sprintf(st_msg.c_abnormal_msg, "[ + mn_motor_no + ] Though canceled transfer of motor, error happened.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				
				mn_home_step = 120;
			}
			break;

		case 110 :
			if (st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재 //
			//	st_msg.mstr_normal_msg = _T("[" + m_str_cmd_motor + "] Motor shut down during transfer.");
				sprintf(st_msg.c_normal_msg, "[ + mn_motor_no + ] Motor shut down during transfer.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 완료 출력 요청 //
			}

			//if (m_nstop_req != FALSE){
			if (mn_stop_req){
				mn_stop_req = FALSE;		// ESTOP 요청 플래그 초기화
			}

			mn_home_step = 0;
			n_home_flag = CTLBD_RET_GOOD;
			break;

		case 120 :
			if (st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재 //

				//st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				sprintf(st_msg.c_abnormal_msg, "[ + mn_motor_no + ] Transfer command of motor failed.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
			}

			//if (m_nstop_req != FALSE){
			if (mn_stop_req)
			{
				mn_stop_req = FALSE;	// ESTOP 요청 플래그 초기화 //
			}

			st_motor[mn_motor_no].mn_home_step = 0;
			mn_home_step = 0; 
			n_home_flag = CTLBD_RET_ERROR;
			break;

		case 130 :
			if (st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재 //
				//st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
			}

			//if (m_nstop_req != FALSE){
			if (mn_stop_req)
			{
				mn_stop_req = FALSE;	// ESTOP 요청 플래그 초기화 //
			}

			st_motor[mn_motor_no].mn_home_step = 0;
			mn_home_step = 0; 
			n_home_flag = CTLBD_RET_ERROR;
			break;
	}

	return  n_home_flag;
}

void CDialog_Manual_FrontHs::OnBtnGoStepRbtMove() 
{
	// TODO: Add your control notification handler code here
	CDialog_Message msg_dlg;
	int mn_response;

	st_msg.str_fallacy_msg = _T("Step Robot Move 테스트를 하시겠습니까?");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		st_msg.str_fallacy_msg = _T("Would you like to test Step Robot Move?");
	}

	mn_response = msg_dlg.DoModal();
	if (mn_response != IDOK) return;

	if (st_handler.mn_menu_lock != TRUE)
		st_handler.mn_menu_lock = TRUE;

	OnSite_Controls_Enable(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
	OnSite_Step_Clear();
	
	st_handler.mn_init_state[INIT_RBT_LEFT_SINK] = CTL_READY;
	InitStep = 0;

	SetTimer(TMR_FRONTHSSTEPMOT_CMD, 100, NULL);			// 모터 동작 제어 타이머		
}


/////////////////////////////////////////// 20121006
void CDialog_Manual_FrontHs::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CDialog_Message msg_dlg;

	CString strTmp, strChange, str_file;
	CString str_tmp, str_data;
	int nResponse;

	CString strText;
	
	CDialog_KeyPad	pad_dlg;
	
	int n_response;
	int nMotorPos = 0;

	if (st_handler.mn_menu_lock) return;

	nMotorPos = m_cmb_motor_pos.GetCurSel();

	if(nMotorPos == 0)
	{
		m_n_axis = M_HS_F_RBT_Y;
	}
	else if(nMotorPos == 1)
	{
		m_n_axis = M_HS_F_RBT_Z;
	}
	else if(nMotorPos == 2)
	{
		m_n_axis = M_HS_F_STACKER_1;
	}
	else if(nMotorPos == 3)
	{
		m_n_axis = M_HS_F_STACKER_2;
	}
	else if(nMotorPos == 4)
	{
		m_n_axis = M_HS_F_STACKER_MOVE;
	}
	else
	{
		m_n_axis = M_WORK_BUFFER_1 + nMotorPos - 5;
	}	
	
	if (nMotorPos < 0)	
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual FRONT HS] 이동 축이 선택되지 않았습니다.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "[Manual FRONT HS] is not selected Axis");
			}

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
		}
		return;
	}

		m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

		switch(lpcc->Row)
		{
		case 3:
		case 4:
			switch(lpcc->Col)
			{
			case 3:
			case 4:
				if (COMI.mn_motorbd_init_end != TRUE)  
				{
					st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
					
					nResponse = msg_dlg.DoModal();
					if (nResponse == IDOK) return ;
				}
				strTmp.Format("[Motor] (%s)_Axis Power Off", m_str_axis_name);
				Func.On_LogFile_Add(99, strTmp);
				
				COMI.Set_MotPower(m_n_axis, FALSE);
				break;
				
			case 5:
			case 6:
				if (COMI.mn_motorbd_init_end != TRUE)  
				{
					st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
					
					nResponse = msg_dlg.DoModal();
					if (nResponse == IDOK) return ;
				}
				strTmp.Format("[Motor] (%s)_Axis Power On", m_str_axis_name);
				Func.On_LogFile_Add(99, strTmp);
				
				COMI.Set_MotPower(m_n_axis, TRUE);
				break;
			}
			break;
			
			case 5:
			case 6:
				if (lpcc->Col == 3 || lpcc->Col == 4 || lpcc->Col == 5 || lpcc->Col == 6)
				{
					if (COMI.mn_motorbd_init_end != TRUE)  
					{
						st_msg.str_fallacy_msg = _T("First, you may initialize motion board.");
						
						nResponse = msg_dlg.DoModal();
						if (nResponse == IDOK) return ;
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
					if (m_n_move_mode != 0)
					{
						m_n_move_mode = 0;
						
						m_p_grid.GridCellColor(m_grid_motor_info, 8, 1, RED_C, BLACK_C);
						m_p_grid.GridCellColor(m_grid_motor_info, 8, 2, BLACK_L, YELLOW_C);
					}
					break;
					
				case 2:
					if (m_n_move_mode != 1)
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

LONG CDialog_Manual_FrontHs::OnUserLButtonDown(WPARAM wParam, LPARAM lParam)
{
	CDialog_Message msg_dlg;
	CString strTmp;
	int		nRet;
	double  dmot_curr_pos;
	
	nRet = Func.DoorOpenCheckSpot();
	
	if (nRet == CTLBD_RET_ERROR)
	{
		return 0;
	}

	// Velocity-Move(Jog) 모드일때만 WM_LBUTTONDOWN 이벤트를 처리한다. //
	// Rel/Abs Position Mode에서는 WM_LBUTTONUP 이벤트에서 이송 명령을 //
	// 처리한다.                                                       //
	CButtonST *pButton = (CButtonST *)lParam;

	nRet = pButton->GetTag();

	if (pButton->GetTag() == IDC_BTN_LEFT)
	{ // "Jog (-)" 버튼이 눌린 경우...
		if (m_n_move_mode == 0)//20120716 0->1
		{
			nRet = CTL_Lib.Motor_SafetyCheck(0, m_n_axis, 0);
			
			if (nRet == CTLBD_RET_ERROR)
			{
				return 0;
			}
			
			OnButtonControl(FALSE);	
			
			strTmp.Format("[Motor] (%s)_Axis - Jog Move", m_str_axis_name);
			Func.On_LogFile_Add(99, strTmp);
			
			COMI.Set_MotSpeed(MOT_SPD_JOG, m_n_axis, cmSMODE_KEEP, st_motor[m_n_axis].md_spd_jog, st_motor[m_n_axis].md_spd_acc, st_motor[m_n_axis].md_spd_dec);		// 축의 속도를 변경한다.
			
			if (cmmSxVMoveStart(m_n_axis, MINUS) != cmERR_NONE)
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
			OnButtonControl(FALSE);							// 버튼 비활성화
			MotorMoveStepClear();							// 모터제어 함수 스텝 초기화
			
			mn_motor_no = m_n_cmd_motor_no	= m_n_axis;					// 모터 번호 설정
			mn_cmd_no = MOT_MOVE;					// 실행 명령 번호 설정 [모터 이동 명령]
//			m_n_cmd_no			= MOVING;							// 실행 명령 번호 설정 [모터 이동 명령]
			m_d_mot_goto_spd	= (st_motor[m_n_cmd_motor_no].md_spd_jog / COMI.md_spd_vel[m_n_cmd_motor_no][0]) * 100;
			m_str_cmd_motor		= m_str_axis_name;
			
			dmot_curr_pos		= COMI.Get_MotCurrentPos(m_n_axis);
			
//			m_d_mot_goto_pos	= dmot_curr_pos - m_d_data;
			md_mot_start_pos	= dmot_curr_pos - m_d_data;
			
			strTmp.Format("[Motor] (%s)_Axis (%.3f) - Data Move", m_str_axis_name, m_d_mot_goto_pos);
			Func.On_LogFile_Add(99, strTmp);
			
			SetTimer(TMR_FRONTHSMOT_CMD, 10, NULL);
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT)
	{ 
		if (m_n_move_mode == 0)//20120716 0->1
		{
			nRet = CTL_Lib.Motor_SafetyCheck(0, m_n_axis, 0);
			
			if (nRet == CTLBD_RET_ERROR)
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
			
			mn_motor_no = m_n_cmd_motor_no	= m_n_axis;					// 모터 번호 설정
			mn_cmd_no = MOT_MOVE;					// 실행 명령 번호 설정 [모터 이동 명령]
//			m_n_cmd_no			= MOVING;							// 실행 명령 번호 설정 [모터 이동 명령]
			m_d_mot_goto_spd	= (st_motor[m_n_cmd_motor_no].md_spd_jog / COMI.md_spd_vel[m_n_cmd_motor_no][0]) * 100;
			m_str_cmd_motor		= m_str_axis_name;
			
			dmot_curr_pos		= COMI.Get_MotCurrentPos(m_n_axis);
			
//			m_d_mot_goto_pos	= dmot_curr_pos + m_d_data;
			md_mot_start_pos	= dmot_curr_pos + m_d_data;			
			
			strTmp.Format("[Motor] (%s)_Axis (%.3f) + Data Move", m_str_axis_name, m_d_mot_goto_pos);
			Func.On_LogFile_Add(99, strTmp);
			
			SetTimer(TMR_FRONTHSMOT_CMD, 10, NULL);
		}
	}

	return 0;
}

LONG CDialog_Manual_FrontHs::OnUserLButtonUp(WPARAM wParam, LPARAM lParam)
{	
	CDialog_Message msg_dlg;
	CString strTmp;
	int		nRet;

	nRet = Func.DoorOpenCheckSpot();
	
	if (nRet == CTLBD_RET_ERROR)
	{
 		st_msg.str_fallacy_msg = _T("If the Open Door, does not work.");
 		::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1000, 0);
		return 0;
	}

	// Velocity-Move(Jog) 모드일때만 WM_LBUTTONDOWN 이벤트를 처리한다. //
	// Rel/Abs Position Mode에서는 WM_LBUTTONUP 이벤트에서 이송 명령을 //
	// 처리한다.                                                       //
	CButtonST *pButton = (CButtonST *)lParam;

	nRet = pButton->GetTag();

	if (pButton->GetTag() == IDC_BTN_LEFT)
	{ // "Jog (-)" 버튼이 눌린 경우...
		if (m_n_move_mode == 0)//20120716 0->1
		{
			strTmp.Format("[Motor] (%s)_Axis - Jog Stop", m_str_axis_name);
			Func.On_LogFile_Add(99, strTmp);
			
			cmmSxStop(m_n_axis, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			OnButtonControl(TRUE);	
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT)
	{ 
		if (m_n_move_mode == 0)//20120716 0->1
		{
			strTmp.Format("[Motor] (%s)_Axis + Jog Stop", m_str_axis_name);
			Func.On_LogFile_Add(99, strTmp);
			
			cmmSxStop(m_n_axis, FALSE, TRUE);  // 모터 이동 정지 함수 
			
			OnButtonControl(TRUE);	
		}
	}
	return 0;
}


void CDialog_Manual_FrontHs::OnButtonControl(BOOL b_flag)
{

}

void CDialog_Manual_FrontHs::MotorMoveStepClear()
{
 	mn_stop_req = FALSE;	// ESTOP 요청 플래그 초기화
	
	m_n_cmd_no = 0;			// 실행 명령 번호 저장 변수 초기화
	m_n_cmd_motor_no = -1;	// 모터 번호 저장 변수 초기화
	m_str_cmd_motor = _T("Motor NULL");
	
 	mn_home_step = 0;		// 모터 HOMING 처리 스텝 저장 변수 초기화
 	mn_move_step = 0;		// 모터 이동 처리 스텝 저장 변수 초기화
}

///////////////////////////////////////////

void CDialog_Manual_FrontHs::MotorStatusCheck()
{
	m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

	if (m_n_home == 0)
	{
		if (m_n_sd == 1)
		{
			if (m_n_minus_el == 1)
			{
				if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELM) == CTLBD_RET_GOOD)
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, RED_C, WHITE_C);
				}
				else
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
				}
			}
			
			if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_SD) == CTLBD_RET_GOOD)
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, RED_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, BLACK_L, WHITE_C);
			}
			
			if (m_n_plus_el == 1)
			{
				if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELP) == CTLBD_RET_GOOD)
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
			if (m_n_minus_el == 1)
			{
				if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELM) == CTLBD_RET_GOOD)
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, RED_C, WHITE_C);
				}
				else
				{
					m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
				}
			}
			
			if (m_n_plus_el == 1)
			{
				if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELP) == CTLBD_RET_GOOD)
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
		if (m_n_minus_el == 1)
		{
			if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELM) == CTLBD_RET_GOOD)
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, RED_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 1, BLACK_L, WHITE_C);
			}
		}
		
		if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_HOME) == CTLBD_RET_GOOD)
		{
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, RED_C, WHITE_C);
		}
		else
		{
			m_p_grid.GridCellColor(m_grid_motor_info, 2, 3, BLACK_L, WHITE_C);
		}
		
		if (m_n_plus_el == 1)
		{
			if (COMI.Get_MotIOSensor(m_n_axis, MOT_SENS_ELP) == CTLBD_RET_GOOD)
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 5, RED_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(m_grid_motor_info, 2, 5, BLACK_L, WHITE_C);
			}
		}
	}

	if (COMI.Get_MotPower(m_n_axis) == CTLBD_RET_GOOD)
	{
		m_p_grid.GridCellColor(m_grid_motor_info, 4, 1, RED_C, WHITE_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_motor_info, 4, 1, BLACK_L, WHITE_C);
	}

	if (COMI.Get_MotAlarmStatus(m_n_axis) == CTLBD_RET_GOOD)
	{
		m_p_grid.GridCellColor(m_grid_motor_info, 6, 1, BLACK_L, WHITE_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_motor_info, 6, 1, RED_C, WHITE_C);
	}
}

void CDialog_Manual_FrontHs::MotorPositionCheck()
{
//	m_dgt_current_pos.SetVal(COMI.Get_MotCurrentPos(st_part.n_motor_axis[MOTOR_PART]));
	CString str_tmp;

	m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

	str_tmp.Format("%.3f", COMI.Get_MotCurrentPos(m_n_axis));
	m_p_grid.GridCellText(m_grid_motor_info, 8, 3, str_tmp);
}



void CDialog_Manual_FrontHs::OnSelchangeCmbMotorPos() 
{
	// TODO: Add your control notification handler code here
	int nMotorPos = m_cmb_motor_pos.GetCurSel();
	
	if(nMotorPos == 0)
	{
		m_n_axis = M_HS_F_RBT_Y;
	}
	else if(nMotorPos == 1)
	{
		m_n_axis = M_HS_F_RBT_Z;
	}
	else if(nMotorPos == 2)
	{
		m_n_axis = M_HS_F_STACKER_1;
	}
	else if(nMotorPos == 3)
	{
		m_n_axis = M_HS_F_STACKER_2;
	}
	else if(nMotorPos == 4)
	{
		m_n_axis = M_HS_F_STACKER_MOVE;
	}
	else
	{
		m_n_axis = M_WORK_BUFFER_1 + nMotorPos - 5;
	}	

	Init_Button();
	
}



void CDialog_Manual_FrontHs::OnBtnGoFHsPos() 
{
	// TODO: Add your control notification handler code here
	int n_tray_y;
	int mn_FirstPicker_Y_Pos = 0;
	////////////////////// 20121107
	int	nRet;	
	int n_response;	
	CDialog_Message  msg_dlg;
	////////////////////// 
	
	n_tray_y = m_cmb_f_hs_pos.GetCurSel();
	
	if (n_tray_y < 0)	
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg,"[F_HS RBT] can not move.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
		}
		return;
	}
	
	OnSite_Controls_Enable(FALSE);  // 모든 컨트롤 화면 출력 제어 함수
	
	if (st_handler.mn_menu_lock != TRUE)
		st_handler.mn_menu_lock = TRUE;
	// **************************************************************************
	
	OnSite_Step_Clear();				// 모터 동작 처리 스텝 초기화 함수
	
	// **************************************************************************
	// 모터 이동 위치 설정한다                                                   
	// **************************************************************************

	////////////////////// 20121107
	if(n_tray_y == 7 || n_tray_y == 8 || n_tray_y == 9 || n_tray_y == 10 ||
	   n_tray_y == 11 || n_tray_y == 12 || n_tray_y == 13 ||
	   n_tray_y == 14 || n_tray_y == 15 || n_tray_y == 16 || n_tray_y == 17 ||
	   n_tray_y == 18 || n_tray_y == 19 || n_tray_y == 20 || n_tray_y == 21 ||
	   n_tray_y == 22 || n_tray_y == 23 || n_tray_y == 24 || n_tray_y == 25 ||
	   n_tray_y == 26 || n_tray_y == 27 || n_tray_y == 28)
	{
	}
	else
	{
		nRet = COMI.Check_SingleMove(M_HS_F_RBT_Z, Z_LD_SAFETY_UP);
		if (nRet != CTLBD_RET_GOOD)
		{
			st_msg.str_fallacy_msg = _T("Z축이 Safety 위치가 아닙니다.");	
			if(st_handler.mn_language == LANGUAGE_ENGLISH)
			{
				st_msg.str_fallacy_msg = _T("Safety is not the position of the Z-axis");
			}
			
			n_response = msg_dlg.DoModal();
			OnSite_Controls_Enable(TRUE);  // 모든 컨트롤 화면 출력 제어 함수
			st_handler.mn_menu_lock = FALSE;
			return;
		}
		else
		{
		}
	}
	//////////////////////

	mn_cmd_no = MOT_MOVE;					// 실행 명령 번호 설정 [모터 이동 명령]
//	mn_motor_no = M_HS_F_RBT_Y;
	if(n_tray_y == 0 || n_tray_y == 1 || n_tray_y == 2 || n_tray_y == 3 ||
	   n_tray_y == 4 || n_tray_y == 5 || n_tray_y == 6)	mn_motor_no = M_HS_F_RBT_Y;
	if(n_tray_y == 7 || n_tray_y == 8 || n_tray_y == 9 || n_tray_y == 10 ||
	   n_tray_y == 11 || n_tray_y == 12 || n_tray_y == 13) mn_motor_no = M_HS_F_RBT_Z;

	if(n_tray_y == 14 || n_tray_y == 15 || n_tray_y == 16 || n_tray_y == 17 ||
	   n_tray_y == 18 || n_tray_y == 19) mn_motor_no = M_HS_F_STACKER_1;

	if(n_tray_y == 20 || n_tray_y == 21 || n_tray_y == 22 || n_tray_y == 23 ||
	   n_tray_y == 24 || n_tray_y == 25) mn_motor_no = M_HS_F_STACKER_2;

	if(n_tray_y == 26 || n_tray_y == 27 || n_tray_y == 28) mn_motor_no = M_HS_F_STACKER_MOVE;


	if(n_tray_y == 0) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];
	}
	else if(n_tray_y == 1)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 2)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];	
	}
	else if(n_tray_y == 3)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[3];	
	}
	else if(n_tray_y == 4)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[4];	
	}
	else if(n_tray_y == 5) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[5];
	}
	else if(n_tray_y == 6) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[6];
	}
	else if(n_tray_y == 7) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];
	}
	else if(n_tray_y == 8)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 9)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];	
	}
	else if(n_tray_y == 10)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[3];	
	}
	else if(n_tray_y == 11)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[4];	
	}
	else if(n_tray_y == 12) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[5];
	}
	else if(n_tray_y == 13) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[6];
	}
	else if(n_tray_y == 14) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];
	}
	else if(n_tray_y == 15)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 16)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];	
	}
	else if(n_tray_y == 17)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[3];	
	}
	else if(n_tray_y == 18)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[4];	
	}
	else if(n_tray_y == 19) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[5];
	}
	else if(n_tray_y == 20) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];
	}
	else if(n_tray_y == 21)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 22)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];	
	}
	else if(n_tray_y == 23)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[3];	
	}
	else if(n_tray_y == 24)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[4];	
	}
	else if(n_tray_y == 25) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[5];
	}
	else if(n_tray_y == 26) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];
	}
	else if(n_tray_y == 27)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 28)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];	
	}

	SetTimer(TMR_FRONTHSMOT_CMD, 10, NULL);		// 모터 동작 제어 타이머
	// **************************************************************************			
}
