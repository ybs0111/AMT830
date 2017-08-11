// Dialog_Manual_Clip_Clamp.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Manual_Clip_Clamp.h"
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
// CDialog_Manual_Clip_Clamp dialog


CDialog_Manual_Clip_Clamp::CDialog_Manual_Clip_Clamp(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Manual_Clip_Clamp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Manual_Clip_Clamp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_n_move_mode = 0;
}


void CDialog_Manual_Clip_Clamp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Manual_Clip_Clamp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BTN_STOP, m_btn_stop);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btn_right);
	DDX_Control(pDX, IDC_BTN_HOME, m_btn_home);
	DDX_Control(pDX, IDC_BTN_LEFT, m_btn_left);
	DDX_Control(pDX, IDC_GROUP_CLIP, m_group_clip);
	DDX_Control(pDX, IDC_GROUP_BUFFER_RBT_MOVE, m_group_buffer_rbt_move);
	DDX_Control(pDX, IDC_GROUP_BUFFER, m_group_buffer);
	DDX_Control(pDX, IDC_GROUP_MOTOR, m_group_motor);
	DDX_Control(pDX, IDC_GROUP_CLIP_RBT_MOVE, m_group_clip_rbt_move);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_1, m_msg_clip_1);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_2, m_msg_clip_2);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_3, m_msg_clip_3);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_4, m_msg_clip_4);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_5, m_msg_clip_5);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_6, m_msg_clip_6);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_7, m_msg_clip_7);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_8, m_msg_clip_8);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_9, m_msg_clip_9);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_10, m_msg_clip_10);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_11, m_msg_clip_11);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_12, m_msg_clip_12);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_13, m_msg_clip_13);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_14, m_msg_clip_14);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_15, m_msg_clip_15);
	DDX_Control(pDX, IDC_MSG_CLIP_SOL_16, m_msg_clip_16);
	DDX_Control(pDX, IDC_MSG_BUFFER4, m_msg_buffer4);
	DDX_Control(pDX, IDC_MSG_BUFFER3, m_msg_buffer3);
	DDX_Control(pDX, IDC_MSG_BUFFER2, m_msg_buffer2);
	DDX_Control(pDX, IDC_MSG_BUFFER1, m_msg_buffer1);
	DDX_Control(pDX, IDC_MSG_BUFFER_MODULE, m_msg_buffer_module);
	DDX_Control(pDX, IDC_MSG_BUFFER_HS, m_msg_buffer_hs);
	DDX_Control(pDX, IDC_BTN_BUFFER_MODULE_SOL4, m_btn_buffer_module_sol4);
	DDX_Control(pDX, IDC_BTN_BUFFER_MODULE_SOL3, m_btn_buffer_module_sol3);
	DDX_Control(pDX, IDC_BTN_BUFFER_MODULE_SOL2, m_btn_buffer_module_sol2);
	DDX_Control(pDX, IDC_BTN_BUFFER_MODULE_SOL1, m_btn_buffer_module_sol1);
	DDX_Control(pDX, IDC_BTN_BUFFER_HS_SOL1, m_btn_buffer_hs_sol1);
	DDX_Control(pDX, IDC_BTN_BUFFER_HS_SOL2, m_btn_buffer_hs_sol2);
	DDX_Control(pDX, IDC_BTN_BUFFER_HS_SOL3, m_btn_buffer_hs_sol3);
	DDX_Control(pDX, IDC_BTN_BUFFER_HS_SOL4, m_btn_buffer_hs_sol4);
	DDX_Control(pDX, IDC_BTN_GO_BUFFER_POS, m_btn_go_buffer_pos);
	DDX_Control(pDX, IDC_BTN_GO_CLIP_POS, m_btn_go_clip_pos);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_1, m_btn_clip_sol_1);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_2, m_btn_clip_sol_2);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_3, m_btn_clip_sol_3);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_4, m_btn_clip_sol_4);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_5, m_btn_clip_sol_5);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_6, m_btn_clip_sol_6);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_7, m_btn_clip_sol_7);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_8, m_btn_clip_sol_8);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_9, m_btn_clip_sol_9);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_10, m_btn_clip_sol_10);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_11, m_btn_clip_sol_11);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_12, m_btn_clip_sol_12);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_13, m_btn_clip_sol_13);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_14, m_btn_clip_sol_14);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_15, m_btn_clip_sol_15);
	DDX_Control(pDX, IDC_BTN_CLIP_SOL_16, m_btn_clip_sol_16);
	DDX_Control(pDX, IDC_CMB_MOTOR_POS, m_cmb_motor_pos);
	DDX_Control(pDX, IDC_CMB_BUFFER_POS, m_cmb_buffer_pos);
	DDX_Control(pDX, IDC_CMB_CLIP_POS, m_cmb_clip_pos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Manual_Clip_Clamp, CDialog)
	//{{AFX_MSG_MAP(CDialog_Manual_Clip_Clamp)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_1, OnBtnClipSol1)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_2, OnBtnClipSol2)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_3, OnBtnClipSol3)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_4, OnBtnClipSol4)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_5, OnBtnClipSol5)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_6, OnBtnClipSol6)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_7, OnBtnClipSol7)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_8, OnBtnClipSol8)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_9, OnBtnClipSol9)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_10, OnBtnClipSol10)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_11, OnBtnClipSol11)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_12, OnBtnClipSol12)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_13, OnBtnClipSol13)
	ON_BN_CLICKED(IDC_BTN_BUFFER_MODULE_SOL1, OnBtnBufferModuleSol1)
	ON_BN_CLICKED(IDC_BTN_BUFFER_MODULE_SOL2, OnBtnBufferModuleSol2)
	ON_BN_CLICKED(IDC_BTN_BUFFER_MODULE_SOL3, OnBtnBufferModuleSol3)
	ON_BN_CLICKED(IDC_BTN_BUFFER_MODULE_SOL4, OnBtnBufferModuleSol4)
	ON_BN_CLICKED(IDC_BTN_BUFFER_HS_SOL1, OnBtnBufferHsSol1)
	ON_BN_CLICKED(IDC_BTN_BUFFER_HS_SOL2, OnBtnBufferHsSol2)
	ON_BN_CLICKED(IDC_BTN_BUFFER_HS_SOL3, OnBtnBufferHsSol3)
	ON_BN_CLICKED(IDC_BTN_BUFFER_HS_SOL4, OnBtnBufferHsSol4)
	ON_BN_CLICKED(IDC_BTN_GO_BUFFER_POS, OnBtnGoBufferPos)
	ON_BN_CLICKED(IDC_BTN_GO_CLIP_POS, OnBtnGoClipPos)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_CBN_SELCHANGE(IDC_CMB_MOTOR_POS, OnSelchangeCmbMotorPos)
	ON_BN_CLICKED(IDC_BTN_HOME, OnBtnHome)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_14, OnBtnClipSol14)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_15, OnBtnClipSol15)
	ON_BN_CLICKED(IDC_BTN_CLIP_SOL_16, OnBtnClipSol16)
	//}}AFX_MSG_MAP
/////////////////////////////20121006
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
	ON_MESSAGE(WMU_CHILD_LBUTTON_DOWN, OnUserLButtonDown)
	ON_MESSAGE(WMU_CHILD_LBUTTON_UP, OnUserLButtonUp)
/////////////////////////////

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Clip_Clamp message handlers
BOOL CDialog_Manual_Clip_Clamp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int nMotorPos = 0;
	
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(12,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

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
	OnSiteRbt_Button_Status();

	m_cmb_motor_pos.SetCurSel(nMotorPos);// .Init_Tab(this, m_n_axis);

	if(nMotorPos == 0)
	{
		m_n_axis = M_CLIP_BUFFER;
	}
	else if(nMotorPos == 1)
	{
		m_n_axis = M_M_CLAMP_RBT_GRIPPER;
	}
	else if(nMotorPos == 2)
	{
		m_n_axis = M_M_CLAMP_RBT_Y;
	}
	else if(nMotorPos == 3)
	{
		m_n_axis = M_M_CLAMP_RBT_Z;
	}
	else if(nMotorPos == 4)
	{
		m_n_axis = M_CLIP_INSERT_Y;
	}
	else if(nMotorPos == 5)
	{
		m_n_axis = M_CLIP_INSERT_Z;
	}
	else if(nMotorPos == 6)
	{
		m_n_axis = M_M_CLIP_CLAMP;
	}	
	else
	{
		m_n_axis = M_WORK_BUFFER_1 + nMotorPos - 7;
	}
	
	if (COMI.mn_motorbd_init_end)
	{
		SetTimer(TM_STATUS_CHECK, 100, NULL);
		SetTimer(TM_POSITION_CHECK, 100, NULL);
	}

	SetTimer(TMR_CLIP_IO_MONITOR_CMD, 50, NULL);		// IO 정보 읽기 타이머 생성
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//////////////////////////////////////////////////// 20121006
void CDialog_Manual_Clip_Clamp::Init_Grid_Info()
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

void CDialog_Manual_Clip_Clamp::Init_GroupBox_Set()
{
	CSxLogFont ls_Logo(15,FW_SEMIBOLD,false,"맑은 고딕");

	m_group_buffer_rbt_move.SetFont(ls_Logo);;
	m_group_buffer_rbt_move.SetCatptionTextColor(RGB(145,25,0));
	m_group_buffer_rbt_move.SetFontBold(TRUE);

	m_group_buffer.SetFont(ls_Logo);
	m_group_buffer.SetCatptionTextColor(RGB(145,25,0));
	m_group_buffer.SetFontBold(TRUE);

	m_group_motor.SetFont(ls_Logo);
	m_group_motor.SetCatptionTextColor(RGB(145,25,0));
	m_group_motor.SetFontBold(TRUE);

	m_group_clip.SetFont(ls_Logo);
	m_group_clip.SetCatptionTextColor(RGB(145,25,0));
	m_group_clip.SetFontBold(TRUE);

	m_group_clip_rbt_move.SetFont(ls_Logo);
	m_group_clip_rbt_move.SetCatptionTextColor(RGB(145,25,0));
	m_group_clip_rbt_move.SetFontBold(TRUE);
}

void CDialog_Manual_Clip_Clamp::Init_Label()
{
	int i;
	CString str_line;
	
	m_cmb_buffer_pos.SetFont(m_load_combo_font);
	m_cmb_buffer_pos.PostMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM) 60);
	
	for (i = 0; i < 4; i++)
	{
		str_line.Format("%02d", i + 1);		
		m_cmb_buffer_pos.AddString(str_line);
	}
	m_cmb_buffer_pos.SetCurSel(0);

	m_cmb_motor_pos.SetFont(m_motor_font);
	m_cmb_motor_pos.PostMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM) 20);
	
	for (i = 0; i < 11; i++)
	{
		if(i == 0)
		{
			str_line.Format("%02d_CLIP_BUFFER", i + 1);
		}
		else if(i == 1)
		{
			str_line.Format("%02d_CLAMP_RBT_GLIPPER", i + 1);
		}
		else if(i == 2)
		{
			str_line.Format("%02d_CLAMP_RBT_Y", i + 1);
		}
		else if(i == 3)
		{
			str_line.Format("%02d_CLAMP_RBT_Z", i + 1);
		}
		else if(i == 4)
		{
			str_line.Format("%02d_CLIP_INSERT_Y", i + 1);
		}
		else if(i == 5)
		{
			str_line.Format("%02d_CLIP_INSERT_Z", i + 1);
		}
		else if(i == 6)
		{
			str_line.Format("%02d_CLIP_CLAMP", i + 1);
		}
		else
		{
			str_line.Format("BUFFER_%02d", i - 6);
		}
		m_cmb_motor_pos.AddString(str_line);
	}

	m_cmb_motor_pos.SetCurSel(0);


	m_cmb_clip_pos.SetFont(m_motor_font);
	m_cmb_clip_pos.PostMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM) 20);
	
	for (i = 0; i < 26; i++)
	{
		if(i == 0)
		{
			str_line.Format("Clip_Buffer_Safety");
		}
		else if(i == 1)
		{
			str_line.Format("Clip_Buffer_Load Pos");
		}
		else if(i == 2)
		{
			str_line.Format("Clip_Buffer_Unload Pos");
		}
		else if(i == 3)
		{
			str_line.Format("Clamp_Rbt_Glipper_Safety");
		}
		else if(i == 4)
		{
			str_line.Format("Clamp_Rbt_Glipper_Ready Pos");
		}
		else if(i == 5)
		{
			str_line.Format("Clamp_Rbt_Glipper_Clamp Pos");
		}
		else if(i == 6)
		{
			str_line.Format("Clamp_Rbt_Y_Safety");
		}
		else if(i == 7)
		{
			str_line.Format("Clamp_Rbt_Y_WB1 Pos");
		}
		else if(i == 8)
		{
			str_line.Format("Clamp_Rbt_Y_WB2 Pos");
		}
		else if(i == 9)
		{
			str_line.Format("Clamp_Rbt_Y_WB3 Pos");
		}
		else if(i == 10)
		{
			str_line.Format("Clamp_Rbt_Y_WB4 Pos");
		}
		else if(i == 11)
		{
			str_line.Format("Clamp_Rbt_Z_Safety");
		}
		else if(i == 12)
		{
			str_line.Format("Clamp_Rbt_Z_Work Pos");
		}
		else if(i == 13)
		{
			str_line.Format("Clip_Insert_Y_Safety");
		}
		else if(i == 14)
		{
			str_line.Format("Clip_Insert_Y_Load Pos");
		}
		else if(i == 15)
		{
			str_line.Format("Clip_Insert_Y_WB1 Pos");
		}
		else if(i == 16)
		{
			str_line.Format("Clip_Insert_Y_WB2 Pos");
		}
		else if(i == 17)
		{
			str_line.Format("Clip_Insert_Y_WB3 Pos");
		}
		else if(i == 18)
		{
			str_line.Format("Clip_Insert_Y_WB4 Pos");
		}
		else if(i == 19)
		{
			str_line.Format("Clip_Insert_Y_Clip Remove Pos");
		}
		else if(i == 20)
		{
			str_line.Format("Clip_Insert_Z_Safety");
		}
		else if(i == 21)
		{
			str_line.Format("Clip_Insert_Z_Load Pos");
		}
		else if(i == 22)
		{
			str_line.Format("Clip_Insert_Z_Unload Pos");
		}
		else if(i == 23)
		{
			str_line.Format("Clip_Clamp_Clamp Pos");
		}
		else if(i == 24)
		{
			str_line.Format("Clip_Clamp_Unclamp Pos");
		}
		else if(i == 25)
		{
			str_line.Format("Clip_Clamp_Midclamp Pos");
		}
		m_cmb_clip_pos.AddString(str_line);
	}
	m_cmb_clip_pos.SetCurSel(0);

	m_msg_buffer_module.SetFont(m_p_font);
	m_msg_buffer_module.SetWindowText(_T("module sol"));
	m_msg_buffer_module.SetCenterText();
	m_msg_buffer_module.SetColor(RGB(0,0,255));
	m_msg_buffer_module.SetGradientColor(RGB(0,0,0));
	m_msg_buffer_module.SetTextColor(RGB(255,255,255));

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

	m_msg_clip_1.SetFont(m_p_font);
	m_msg_clip_1.SetWindowText(_T("LD Tube Up/down"));
	m_msg_clip_1.SetCenterText();
	m_msg_clip_1.SetColor(RGB(0,0,255));
	m_msg_clip_1.SetGradientColor(RGB(0,0,0));
	m_msg_clip_1.SetTextColor(RGB(255,255,255));

	m_msg_clip_2.SetFont(m_p_font);
	m_msg_clip_2.SetWindowText(_T("ULD Tube Up/down"));
	m_msg_clip_2.SetCenterText();
	m_msg_clip_2.SetColor(RGB(0,0,255));
	m_msg_clip_2.SetGradientColor(RGB(0,0,0));
	m_msg_clip_2.SetTextColor(RGB(255,255,255));

	m_msg_clip_3.SetFont(m_p_font);
	m_msg_clip_3.SetWindowText(_T("LD Tube Pusher On/Off"));
	m_msg_clip_3.SetCenterText();
	m_msg_clip_3.SetColor(RGB(0,0,255));
	m_msg_clip_3.SetGradientColor(RGB(0,0,0));
	m_msg_clip_3.SetTextColor(RGB(255,255,255));

	m_msg_clip_4.SetFont(m_p_font);
	m_msg_clip_4.SetWindowText(_T("ULD Tube Pusher On/Off"));
	m_msg_clip_4.SetCenterText();
	m_msg_clip_4.SetColor(RGB(0,0,255));
	m_msg_clip_4.SetGradientColor(RGB(0,0,0));
	m_msg_clip_4.SetTextColor(RGB(255,255,255));

	m_msg_clip_5.SetFont(m_p_font);
	m_msg_clip_5.SetWindowText(_T("Tube Move For/Back On/Off"));
	m_msg_clip_5.SetCenterText();
	m_msg_clip_5.SetColor(RGB(0,0,255));
	m_msg_clip_5.SetGradientColor(RGB(0,0,0));
	m_msg_clip_5.SetTextColor(RGB(255,255,255));

	m_msg_clip_6.SetFont(m_p_font);
	m_msg_clip_6.SetWindowText(_T("Clip Buffer Up/Down"));
	m_msg_clip_6.SetCenterText();
	m_msg_clip_6.SetColor(RGB(0,0,255));
	m_msg_clip_6.SetGradientColor(RGB(0,0,0));
	m_msg_clip_6.SetTextColor(RGB(255,255,255));

	m_msg_clip_7.SetFont(m_p_font);
	m_msg_clip_7.SetWindowText(_T("LD Plate Short Up/down"));
	m_msg_clip_7.SetCenterText();
	m_msg_clip_7.SetColor(RGB(0,0,255));
	m_msg_clip_7.SetGradientColor(RGB(0,0,0));
	m_msg_clip_7.SetTextColor(RGB(255,255,255));

	m_msg_clip_8.SetFont(m_p_font);
	m_msg_clip_8.SetWindowText(_T("ULD Plate Short Up/down"));
	m_msg_clip_8.SetCenterText();
	m_msg_clip_8.SetColor(RGB(0,0,255));
	m_msg_clip_8.SetGradientColor(RGB(0,0,0));
	m_msg_clip_8.SetTextColor(RGB(255,255,255));

	m_msg_clip_9.SetFont(m_p_font);
	m_msg_clip_9.SetWindowText(_T("Tube Rotator 0(90) On/Off"));
	m_msg_clip_9.SetCenterText();
	m_msg_clip_9.SetColor(RGB(0,0,255));
	m_msg_clip_9.SetGradientColor(RGB(0,0,0));
	m_msg_clip_9.SetTextColor(RGB(255,255,255));

	m_msg_clip_10.SetFont(m_p_font);
	m_msg_clip_10.SetWindowText(_T("Tube (Un)clamp On/Off"));
	m_msg_clip_10.SetCenterText();
	m_msg_clip_10.SetColor(RGB(0,0,255));
	m_msg_clip_10.SetGradientColor(RGB(0,0,0));
	m_msg_clip_10.SetTextColor(RGB(255,255,255));

	m_msg_clip_11.SetFont(m_p_font);
	m_msg_clip_11.SetWindowText(_T("Tube Cap Remove On/Off"));
	m_msg_clip_11.SetCenterText();
	m_msg_clip_11.SetColor(RGB(0,0,255));
	m_msg_clip_11.SetGradientColor(RGB(0,0,0));
	m_msg_clip_11.SetTextColor(RGB(255,255,255));

	m_msg_clip_12.SetFont(m_p_font);
	m_msg_clip_12.SetWindowText(_T("Tube Latch For/Back On/Off"));
	m_msg_clip_12.SetCenterText();
	m_msg_clip_12.SetColor(RGB(0,0,255));
	m_msg_clip_12.SetGradientColor(RGB(0,0,0));
	m_msg_clip_12.SetTextColor(RGB(255,255,255));

	m_msg_clip_13.SetFont(m_p_font);
	m_msg_clip_13.SetWindowText(_T("LD Plate Long Up/down"));
	m_msg_clip_13.SetCenterText();
	m_msg_clip_13.SetColor(RGB(0,0,255));
	m_msg_clip_13.SetGradientColor(RGB(0,0,0));
	m_msg_clip_13.SetTextColor(RGB(255,255,255));

	m_msg_clip_14.SetFont(m_p_font);
	m_msg_clip_14.SetWindowText(_T("Clip Remove Up/Down"));
	m_msg_clip_14.SetCenterText();
	m_msg_clip_14.SetColor(RGB(0,0,255));
	m_msg_clip_14.SetGradientColor(RGB(0,0,0));
	m_msg_clip_14.SetTextColor(RGB(255,255,255));

	m_msg_clip_15.SetFont(m_p_font);
	m_msg_clip_15.SetWindowText(_T("Clip Insert Rbt Up/Down"));
	m_msg_clip_15.SetCenterText();
	m_msg_clip_15.SetColor(RGB(0,0,255));
	m_msg_clip_15.SetGradientColor(RGB(0,0,0));
	m_msg_clip_15.SetTextColor(RGB(255,255,255));

	
	m_msg_clip_16.SetFont(m_p_font);
	m_msg_clip_16.SetWindowText(_T("Clip Remove Blower On/Off"));
	m_msg_clip_16.SetCenterText();
	m_msg_clip_16.SetColor(RGB(0,0,255));
	m_msg_clip_16.SetGradientColor(RGB(0,0,0));
	m_msg_clip_16.SetTextColor(RGB(255,255,255));	

}

void CDialog_Manual_Clip_Clamp::Init_Data()
{
 	int i;

	mn_cmd_no = 0;
	mn_motor_no = 0;
	md_mot_start_pos = 0;

	for (i = 0; i < 2; i++)
	{
		mn_module_buffer[i] = 0;
		mn_module_buffer[i + 2] = 0;
		mn_hs_buffer[i] = 0;
		mn_hs_buffer[i + 2] = 0;
	}

	for (i = 0; i < 16; i++)
	{
		mn_sol[i] = 0;
	}

//////////////////////////////////////////////////// 20121006
	m_n_axis			= st_motor_info.n_part_axis[3][AXIS_NUM];
	
	m_str_axis_name = st_motor_info.str_part_axis_name[3][AXIS_NUM];
	m_n_minus_el	= st_motor_info.n_axis_minus_el[3][AXIS_NUM];
	m_n_plus_el		= st_motor_info.n_axis_plus_el[3][AXIS_NUM];
	m_n_home		= st_motor_info.n_axis_home[3][AXIS_NUM];
	m_n_sd			= st_motor_info.n_axis_sd[3][AXIS_NUM];
	m_n_direction	= st_motor_info.n_axis_direction[3][AXIS_NUM];
	m_n_account		= st_motor_info.n_axis_account[3][AXIS_NUM];


////////////////////////////////////////////////////	
}

void CDialog_Manual_Clip_Clamp::Init_Button()
{
	short	shBtnColor = 30;
	CString strTmp;
	int nMotorPos = m_cmb_motor_pos.GetCurSel();

	if(nMotorPos == 0 || nMotorPos == 3 || nMotorPos == 5)
	{
		m_btn_left.SetIcon(IDI_UP);
	}
	else if(nMotorPos == 1)
	{
		m_btn_left.SetIcon(IDI_WIDTH);
	}
	else if(nMotorPos == 2 || nMotorPos == 4)
	{
		m_btn_left.SetIcon(IDI_DN);
	}
	else if(nMotorPos == 6)
	{
		m_btn_left.SetIcon(IDI_UNWIDTH);
	}
	else
	{
		m_btn_left.SetIcon(IDI_LEFT);
	}	

//	m_btn_left.SetIcon(IDI_DN);
	m_btn_left.SetTag(IDC_BTN_LEFT);
	m_btn_left.SetFont(m_motor_font);
	m_btn_left.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_left.SetButtonColor(1);

	if(nMotorPos == 0 || nMotorPos == 3 || nMotorPos == 5)
	{
		m_btn_right.SetIcon(IDI_DN);
	}
	else if(nMotorPos == 1)
	{
		m_btn_right.SetIcon(IDI_UNWIDTH);
	}
	else if(nMotorPos == 2 || nMotorPos == 4)
	{
		m_btn_right.SetIcon(IDI_UP);
	}
	else if(nMotorPos == 6)
	{
		m_btn_right.SetIcon(IDI_WIDTH);
	}
	else
	{
		m_btn_right.SetIcon(IDI_RIGHT);
	}	
	
//	m_btn_right.SetIcon(IDI_UP);
	m_btn_right.SetTag(IDC_BTN_RIGHT);
	m_btn_right.SetFont(m_motor_font);
	m_btn_right.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_right.SetButtonColor(1);
	
	m_btn_home.SetTag(IDC_BTN_HOME);
	m_btn_home.SetFont(m_motor_font);
	m_btn_home.SetIcon(IDI_HOME);
	m_btn_home.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_home.SetButtonColor(1);
	
	
	m_btn_buffer_module_sol1.SetFont(m_motor_font);
	m_btn_buffer_module_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_module_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_module_sol1.SetButtonColor(1);
	
	m_btn_buffer_module_sol2.SetFont(m_motor_font);
	m_btn_buffer_module_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_module_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_module_sol2.SetButtonColor(1);
	
	m_btn_buffer_module_sol3.SetFont(m_motor_font);
	m_btn_buffer_module_sol3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_module_sol3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_module_sol3.SetButtonColor(1);
	
	m_btn_buffer_module_sol4.SetFont(m_motor_font);
	m_btn_buffer_module_sol4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_module_sol4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_module_sol4.SetButtonColor(1);
	
	m_btn_buffer_hs_sol1.SetFont(m_motor_font);
	m_btn_buffer_hs_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_hs_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_hs_sol1.SetButtonColor(1);

	m_btn_buffer_hs_sol2.SetFont(m_motor_font);
	m_btn_buffer_hs_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_hs_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_hs_sol2.SetButtonColor(1);

	m_btn_buffer_hs_sol3.SetFont(m_motor_font);
	m_btn_buffer_hs_sol3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_hs_sol3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_hs_sol3.SetButtonColor(1);

	m_btn_buffer_hs_sol4.SetFont(m_motor_font);
	m_btn_buffer_hs_sol4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_buffer_hs_sol4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_buffer_hs_sol4.SetButtonColor(1);

	m_btn_clip_sol_1.SetFont(m_motor_font);
	m_btn_clip_sol_1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_1.SetButtonColor(1);

	m_btn_clip_sol_2.SetFont(m_motor_font);
	m_btn_clip_sol_2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_2.SetButtonColor(1);

	m_btn_clip_sol_3.SetFont(m_motor_font);
	m_btn_clip_sol_3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_3.SetButtonColor(1);

	m_btn_clip_sol_4.SetFont(m_motor_font);
	m_btn_clip_sol_4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_4.SetButtonColor(1);

	m_btn_clip_sol_5.SetFont(m_motor_font);
	m_btn_clip_sol_5.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_5.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_5.SetButtonColor(1);

	m_btn_clip_sol_6.SetFont(m_motor_font);
	m_btn_clip_sol_6.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_6.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_6.SetButtonColor(1);

	m_btn_clip_sol_7.SetFont(m_motor_font);
	m_btn_clip_sol_7.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_7.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_7.SetButtonColor(1);

	m_btn_clip_sol_8.SetFont(m_motor_font);
	m_btn_clip_sol_8.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_8.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_8.SetButtonColor(1);

	m_btn_clip_sol_9.SetFont(m_motor_font);
	m_btn_clip_sol_9.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_9.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_9.SetButtonColor(1);

	m_btn_clip_sol_10.SetFont(m_motor_font);
	m_btn_clip_sol_10.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_10.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_10.SetButtonColor(1);

	m_btn_clip_sol_11.SetFont(m_motor_font);
	m_btn_clip_sol_11.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_11.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_11.SetButtonColor(1);

	m_btn_clip_sol_12.SetFont(m_motor_font);
	m_btn_clip_sol_12.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_12.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_12.SetButtonColor(1);

	m_btn_clip_sol_13.SetFont(m_motor_font);
	m_btn_clip_sol_13.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_13.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_13.SetButtonColor(1);

	m_btn_clip_sol_14.SetFont(m_motor_font);
	m_btn_clip_sol_14.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_14.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_14.SetButtonColor(1);

	m_btn_clip_sol_15.SetFont(m_motor_font);
	m_btn_clip_sol_15.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_15.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_15.SetButtonColor(1);

	m_btn_clip_sol_16.SetFont(m_motor_font);
	m_btn_clip_sol_16.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_clip_sol_16.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_clip_sol_16.SetButtonColor(1);

}

void CDialog_Manual_Clip_Clamp::OnRead_IO_Check()
{
	int i;

	if(FAS_IO.get_in_bit(st_io.o_ldclip_tube_updn_onoff, IO_ON) == IO_ON)
	{
		mn_sol[0] = TRUE;
	}
	else
	{
		mn_sol[0] = FALSE;		
	}	


	if(FAS_IO.get_in_bit(st_io.o_uldclip_tube_updn_onoff, IO_ON) == IO_ON)
	{
		mn_sol[1] = TRUE;
	}
	else
	{
		mn_sol[1] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_ON) == IO_ON)
	{
		mn_sol[2] = TRUE;
	}
	else
	{
		mn_sol[2] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_ON) == IO_ON)
	{
		mn_sol[3] = TRUE;
	}
	else
	{
		mn_sol[3] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF) == IO_OFF)
	{
		mn_sol[4] = TRUE;
	}
	else
	{
		mn_sol[4] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_clip_buffer_updn_onoff, IO_ON) == IO_ON)
	{
		mn_sol[5] = TRUE;
	}
	else
	{
		mn_sol[5] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_tube_s_m_plate_short_updn_onoff, IO_ON) == IO_ON)
	{
		mn_sol[6] = TRUE;
	}
	else
	{
		mn_sol[6] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_uld_tube_s_updn_onoff, IO_ON) == IO_ON)
	{
		mn_sol[7] = TRUE;
	}
	else
	{
		mn_sol[7] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_tube_rotator_90_onoff, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.o_tube_rotator_0_onoff, IO_OFF) == IO_OFF)
	{
		mn_sol[8] = TRUE;
	}
	else
	{
		mn_sol[8] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_tube_rotator_clamp_onoff, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.o_tube_rotator_unclamp_onoff, IO_OFF) == IO_OFF)
	{
		mn_sol[9] = TRUE;
	}
	else
	{
		mn_sol[9] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_ON) == IO_ON)
	{
		mn_sol[10] = TRUE;
	}
	else
	{
		mn_sol[10] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF)
	{
		mn_sol[11] = TRUE;
	}
	else
	{
		mn_sol[11] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_tube_s_m_plate_long_updn_onoff, IO_ON) == IO_ON)
	{
		mn_sol[12] = TRUE;
	}
	else
	{
		mn_sol[12] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_clip_remove_up_onoff, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.o_clip_remove_down_onoff, IO_OFF) == IO_OFF)
	{
		mn_sol[13] = TRUE;
	}
	else
	{
		mn_sol[13] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_clip_rbt_updn_onoff, IO_ON) == IO_ON)
	{
		mn_sol[14] = TRUE;
	}
	else
	{
		mn_sol[14] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_clip_remove_blower_on, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.o_clip_remove_blower_off, IO_OFF) == IO_OFF)
	{
		mn_sol[15] = TRUE;
	}
	else
	{
		mn_sol[15] = FALSE;		
	}	

	for(i=0; i<4; i++)
	{
		if(FAS_IO.get_in_bit(st_io.o_buf1_m_clamp_onoff + (i*100), IO_ON) == IO_ON)
		{
			mn_module_buffer[i] = TRUE;
		}
		else
		{
			mn_module_buffer[i] = FALSE;		
		}	
	}


	for(i=0; i<4; i++)
	{
		if(FAS_IO.get_in_bit(st_io.o_buf1_hs_clamp_onoff + (i*100), IO_ON) == IO_ON)
		{
			mn_hs_buffer[i] = TRUE;
		}
		else
		{
			mn_hs_buffer[i] = FALSE;		
		}	
	}

}


void CDialog_Manual_Clip_Clamp::OnSiteRbt_Button_Status()//20121006
{
	short	shBtnColor = 30;
	
	if (mn_sol[0] < 0 || mn_sol[0] > 1) mn_sol[0] = TRUE;
	m_btn_clip_sol_1.SetCheck(mn_sol[0]);
	if (mn_sol[0] == TRUE)
	{
		m_btn_clip_sol_1.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_1, "Down");
		m_btn_clip_sol_1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_1.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_1.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_1, "Up");
		m_btn_clip_sol_1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_1.SetButtonColor(1);
	}

	if (mn_sol[1] < 0 || mn_sol[1] > 1) mn_sol[1] = TRUE;
	m_btn_clip_sol_2.SetCheck(mn_sol[1]);
	if (mn_sol[1] == TRUE)
	{
		m_btn_clip_sol_2.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_2, "Down");
		m_btn_clip_sol_2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_2.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_2.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_2, "Up");
		m_btn_clip_sol_2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_2.SetButtonColor(1);
	}

	if (mn_sol[2] < 0 || mn_sol[2] > 1) mn_sol[2] = TRUE;
	m_btn_clip_sol_3.SetCheck(mn_sol[2]);
	if (mn_sol[2] == TRUE)
	{
		m_btn_clip_sol_3.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_3, "On");
		m_btn_clip_sol_3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_3.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_3.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_3, "Off");
		m_btn_clip_sol_3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_3.SetButtonColor(1);
	}

	if (mn_sol[3] < 0 || mn_sol[3] > 1) mn_sol[3] = TRUE;
	m_btn_clip_sol_4.SetCheck(mn_sol[3]);
	if (mn_sol[3] == TRUE)
	{
		m_btn_clip_sol_4.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_4, "Off");
		m_btn_clip_sol_4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_4.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_4.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_4, "On");
		m_btn_clip_sol_4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_4.SetButtonColor(1);
	}

	if (mn_sol[4] < 0 || mn_sol[4] > 1) mn_sol[4] = TRUE;
	m_btn_clip_sol_5.SetCheck(mn_sol[4]);
	if (mn_sol[4] == TRUE)
	{
		m_btn_clip_sol_5.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_5, "Off");
		m_btn_clip_sol_5.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_5.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_5.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_5.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_5, "On");
		m_btn_clip_sol_5.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_5.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_5.SetButtonColor(1);
	}

	if (mn_sol[5] < 0 || mn_sol[5] > 1) mn_sol[5] = TRUE;
	m_btn_clip_sol_6.SetCheck(mn_sol[5]);
	if (mn_sol[5] == TRUE)
	{
		m_btn_clip_sol_6.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_6, "Down");
		m_btn_clip_sol_6.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_6.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_6.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_6.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_6, "Up");
		m_btn_clip_sol_6.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_6.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_6.SetButtonColor(1);
	}

	if (mn_sol[6] < 0 || mn_sol[6] > 1) mn_sol[6] = TRUE;
	m_btn_clip_sol_7.SetCheck(mn_sol[6]);
	if (mn_sol[6] == TRUE)
	{
		m_btn_clip_sol_7.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_7, "Down");
		m_btn_clip_sol_7.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_7.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_7.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_7.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_7, "Up");
		m_btn_clip_sol_7.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_7.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_7.SetButtonColor(1);
	}

	if (mn_sol[7] < 0 || mn_sol[7] > 1) mn_sol[7] = TRUE;
	m_btn_clip_sol_8.SetCheck(mn_sol[7]);
	if (mn_sol[7] == TRUE)
	{
		m_btn_clip_sol_8.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_8, "Down");
		m_btn_clip_sol_8.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_8.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_8.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_8.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_8, "Up");
		m_btn_clip_sol_8.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_8.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_8.SetButtonColor(1);
	}

	if (mn_sol[8] < 0 || mn_sol[8] > 1) mn_sol[8] = TRUE;
	m_btn_clip_sol_9.SetCheck(mn_sol[6]);
	if (mn_sol[8] == TRUE)
	{
		m_btn_clip_sol_9.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_9, "0도");
		m_btn_clip_sol_9.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_9.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_9.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_9.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_9, "90도");
		m_btn_clip_sol_9.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_9.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_9.SetButtonColor(1);
	}

	if (mn_sol[9] < 0 || mn_sol[9] > 1) mn_sol[9] = TRUE;
	m_btn_clip_sol_10.SetCheck(mn_sol[9]);
	if (mn_sol[9] == TRUE)
	{
		m_btn_clip_sol_10.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_10, "Off");
		m_btn_clip_sol_10.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_10.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_10.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_10.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_10, "On");
		m_btn_clip_sol_10.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_10.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_10.SetButtonColor(1);
	}

	if (mn_sol[10] < 0 || mn_sol[10] > 1) mn_sol[10] = TRUE;
	m_btn_clip_sol_11.SetCheck(mn_sol[10]);
	if (mn_sol[10] == TRUE)
	{
		m_btn_clip_sol_11.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_11, "Off");
		m_btn_clip_sol_11.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_11.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_11.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_11.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_11, "On");
		m_btn_clip_sol_11.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_11.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_11.SetButtonColor(1);
	}

	if (mn_sol[11] < 0 || mn_sol[11] > 1) mn_sol[11] = TRUE;
	m_btn_clip_sol_12.SetCheck(mn_sol[11]);
	if (mn_sol[11] == TRUE)
	{
		m_btn_clip_sol_12.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_12, "Off");
		m_btn_clip_sol_12.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_12.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_12.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_12.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_12, "On");
		m_btn_clip_sol_12.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_12.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_12.SetButtonColor(1);
	}

	if (mn_sol[12] < 0 || mn_sol[12] > 1) mn_sol[12] = TRUE;
	m_btn_clip_sol_13.SetCheck(mn_sol[12]);
	if (mn_sol[12] == TRUE)
	{
		m_btn_clip_sol_13.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_13, "Down");
		m_btn_clip_sol_13.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_13.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_13.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_13.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_13, "Up");
		m_btn_clip_sol_13.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_13.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_13.SetButtonColor(1);
	}

	if (mn_sol[13] < 0 || mn_sol[13] > 1) mn_sol[13] = TRUE;
	m_btn_clip_sol_14.SetCheck(mn_sol[13]);
	if (mn_sol[13] == TRUE)
	{
		m_btn_clip_sol_14.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_14, "Down");
		m_btn_clip_sol_14.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_14.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_14.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_14.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_14, "Up");
		m_btn_clip_sol_14.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_14.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_14.SetButtonColor(1);
	}

	if (mn_sol[14] < 0 || mn_sol[14] > 1) mn_sol[14] = TRUE;
	m_btn_clip_sol_15.SetCheck(mn_sol[14]);
	if (mn_sol[14] == TRUE)
	{
		m_btn_clip_sol_15.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_15, "Up");
		m_btn_clip_sol_15.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_15.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_15.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_15.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_15, "Down");
		m_btn_clip_sol_15.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_15.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_15.SetButtonColor(1);
	}

	if (mn_sol[15] < 0 || mn_sol[15] > 1) mn_sol[15] = TRUE;
	m_btn_clip_sol_16.SetCheck(mn_sol[15]);
	if (mn_sol[15] == TRUE)
	{
		m_btn_clip_sol_16.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_16, "Off");
		m_btn_clip_sol_16.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_16.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_16.SetButtonColor(1);
	}
	else																					// Off중이면
	{
		m_btn_clip_sol_16.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_CLIP_SOL_16, "On");
		m_btn_clip_sol_16.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_clip_sol_16.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_clip_sol_16.SetButtonColor(1);
	}

	
	if (mn_module_buffer[0] < 0 || mn_module_buffer[0] > 1) mn_module_buffer[0] = TRUE;
	m_btn_buffer_module_sol1.SetCheck(mn_module_buffer[0]);
	if (mn_module_buffer[0] == TRUE)
	{
		m_btn_buffer_module_sol1.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_MODULE_SOL1, "On");
		m_btn_buffer_module_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_module_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_module_sol1.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_module_sol1.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_MODULE_SOL1, "Off");
		m_btn_buffer_module_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_module_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_module_sol1.SetButtonColor(1);

	}

	if (mn_module_buffer[1] < 0 || mn_module_buffer[1] > 1) mn_module_buffer[1] = TRUE;
	m_btn_buffer_module_sol2.SetCheck(mn_module_buffer[1]);
	if (mn_module_buffer[1] == TRUE)
	{
		m_btn_buffer_module_sol2.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_MODULE_SOL2, "On");
		m_btn_buffer_module_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_module_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_module_sol2.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_module_sol2.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_MODULE_SOL2, "Off");
		m_btn_buffer_module_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_module_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_module_sol2.SetButtonColor(1);

	}

	if (mn_module_buffer[2] < 0 || mn_module_buffer[2] > 1) mn_module_buffer[2] = TRUE;
	m_btn_buffer_module_sol3.SetCheck(mn_module_buffer[2]);
	if (mn_module_buffer[2] == TRUE)
	{
		m_btn_buffer_module_sol3.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_MODULE_SOL3, "On");
		m_btn_buffer_module_sol3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_module_sol3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_module_sol3.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_module_sol3.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_MODULE_SOL3, "Off");
		m_btn_buffer_module_sol3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_module_sol3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_module_sol3.SetButtonColor(1);

	}

	if (mn_module_buffer[3] < 0 || mn_module_buffer[3] > 1) mn_module_buffer[3] = TRUE;
	m_btn_buffer_module_sol4.SetCheck(mn_module_buffer[3]);
	if (mn_module_buffer[3] == TRUE)
	{
		m_btn_buffer_module_sol4.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_MODULE_SOL4, "On");
		m_btn_buffer_module_sol4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_module_sol4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_module_sol4.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_module_sol4.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_MODULE_SOL4, "Off");
		m_btn_buffer_module_sol4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_module_sol4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_module_sol4.SetButtonColor(1);

	}

	if (mn_hs_buffer[0] < 0 || mn_hs_buffer[0] > 1) mn_hs_buffer[0] = TRUE;
	m_btn_buffer_hs_sol1.SetCheck(mn_hs_buffer[0]);
	if (mn_hs_buffer[0] == TRUE)
	{
		m_btn_buffer_hs_sol1.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_HS_SOL1, "On");
		m_btn_buffer_hs_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_hs_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_hs_sol1.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_hs_sol1.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_HS_SOL1, "Off");
		m_btn_buffer_hs_sol1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_hs_sol1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_hs_sol1.SetButtonColor(1);

	}

	if (mn_hs_buffer[1] < 0 || mn_hs_buffer[1] > 1) mn_hs_buffer[1] = TRUE;
	m_btn_buffer_hs_sol2.SetCheck(mn_hs_buffer[1]);
	if (mn_hs_buffer[1] == TRUE)
	{
		m_btn_buffer_hs_sol2.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_HS_SOL2, "On");
		m_btn_buffer_hs_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_hs_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_hs_sol2.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_hs_sol2.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_HS_SOL2, "Off");
		m_btn_buffer_hs_sol2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_hs_sol2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_hs_sol2.SetButtonColor(1);

	}

	if (mn_hs_buffer[2] < 0 || mn_hs_buffer[2] > 1) mn_hs_buffer[2] = TRUE;
	m_btn_buffer_hs_sol3.SetCheck(mn_hs_buffer[2]);
	if (mn_hs_buffer[2] == TRUE)
	{
		m_btn_buffer_hs_sol3.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_HS_SOL3, "On");
		m_btn_buffer_hs_sol3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_hs_sol3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_hs_sol3.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_hs_sol3.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_HS_SOL3, "Off");
		m_btn_buffer_hs_sol3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_hs_sol3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_hs_sol3.SetButtonColor(1);

	}

	if (mn_hs_buffer[3] < 0 || mn_hs_buffer[3] > 1) mn_hs_buffer[3] = TRUE;
	m_btn_buffer_hs_sol4.SetCheck(mn_hs_buffer[3]);
	if (mn_hs_buffer[3] == TRUE)
	{
		m_btn_buffer_hs_sol4.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_BUFFER_HS_SOL4, "On");
		m_btn_buffer_hs_sol4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_hs_sol4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_hs_sol4.SetButtonColor(1);

	}
	else
	{
		m_btn_buffer_hs_sol4.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_BUFFER_HS_SOL4, "Off");
		m_btn_buffer_hs_sol4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_buffer_hs_sol4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_buffer_hs_sol4.SetButtonColor(1);

	}

}

void CDialog_Manual_Clip_Clamp::OnSite_Controls_Enable(BOOL iFlag)
{
	m_btn_go_buffer_pos.EnableWindow(iFlag);
	m_btn_go_clip_pos.EnableWindow(iFlag);
	m_btn_clip_sol_1.EnableWindow(iFlag);
	m_btn_clip_sol_2.EnableWindow(iFlag);
	m_btn_clip_sol_3.EnableWindow(iFlag);
	m_btn_clip_sol_4.EnableWindow(iFlag);
	m_btn_clip_sol_5.EnableWindow(iFlag);
	m_btn_clip_sol_6.EnableWindow(iFlag);
	m_btn_clip_sol_7.EnableWindow(iFlag);
	m_btn_clip_sol_8.EnableWindow(iFlag);
	m_btn_clip_sol_9.EnableWindow(iFlag);
	m_btn_clip_sol_10.EnableWindow(iFlag);
	m_btn_clip_sol_11.EnableWindow(iFlag);
	m_btn_clip_sol_12.EnableWindow(iFlag);
	m_btn_clip_sol_13.EnableWindow(iFlag);
	m_btn_clip_sol_14.EnableWindow(iFlag);
	m_btn_clip_sol_15.EnableWindow(iFlag);
	m_btn_clip_sol_16.EnableWindow(iFlag);
	m_btn_buffer_module_sol1.EnableWindow(iFlag);
	m_btn_buffer_module_sol2.EnableWindow(iFlag);
	m_btn_buffer_module_sol3.EnableWindow(iFlag);
	m_btn_buffer_module_sol4.EnableWindow(iFlag);
	m_btn_buffer_hs_sol1.EnableWindow(iFlag);
	m_btn_buffer_hs_sol2.EnableWindow(iFlag);
	m_btn_buffer_hs_sol3.EnableWindow(iFlag);
	m_btn_buffer_hs_sol4.EnableWindow(iFlag);
}

void CDialog_Manual_Clip_Clamp::OnSite_Step_Clear()
{
	mn_stop_req = FALSE;  // ESTOP 요청 플래그 초기화

	mn_cmd_no = 0;        // 실행 명령 번호 저장 변수 초기화

	mn_home_step = 0;     // 모터 HOMING 처리 스텝 저장 변수 초기화
	mn_move_step = 0;     // 모터 이동 처리 스텝 저장 변수 초기화
}

void CDialog_Manual_Clip_Clamp::OnTimer(UINT nIDEvent) 
{
	int n_response,n_check;
//	int nRet[5];

	CDialog_Message msg_dlg;

	if (nIDEvent == TMR_CLIPMOT_CMD) 
	{
		// **************************************************************************
		// 모션 보드 초기화 작업 완료 유무 검사한다                                  
		// **************************************************************************
		if (st_handler.mn_mot_brd_initial != TRUE)  
		{
			KillTimer(TMR_CLIPMOT_CMD) ;  // 타이머 삭제
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
				KillTimer(TMR_CLIPMOT_CMD) ;  // 타이머 삭제 //
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
				KillTimer(TMR_CLIPMOT_CMD) ;  // 타이머 삭제 //
				
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
				KillTimer(TMR_CLIPMOT_CMD) ;  // 타이머 삭제
				mn_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화

				// ******************************************************************
				if (st_handler.mn_menu_lock != FALSE)
					st_handler.mn_menu_lock = FALSE;
				// ******************************************************************

				OnSite_Controls_Enable(TRUE);  // 모든 컨트롤 화면 출력 제어 함수
			}
			else if (n_check == CTLBD_RET_ERROR) 
			{
				KillTimer(TMR_CLIPMOT_CMD) ;  // 타이머 삭제

				mn_cmd_no = 0;  // 실행 명령 번호 저장 변수 초기화

				if (st_handler.mn_menu_lock != FALSE)
					st_handler.mn_menu_lock = FALSE;
				// ******************************************************************

				OnSite_Controls_Enable(TRUE);  // 모든 컨트롤 화면 출력 제어 함수
			}	
		}
		else 
		{
			KillTimer(TMR_CLIPMOT_CMD) ;  // 타이머 삭제
		}


	}
// 	else if (nIDEvent == TMR_CLIP_IO_MONITOR_CMD) 
//	{
//		OnSite_IO_Status_Read();
//	}
	else if (nIDEvent == TMR_CLIP_STEPMOT_CMD) 
	{
		Run_Init();
		if (st_handler.mn_init_state[INIT_RBT_MODULE] == CTL_YES)
		{
			KillTimer(TMR_CLIP_STEPMOT_CMD);
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

void CDialog_Manual_Clip_Clamp::OnBtnClipSol1() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_1.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_ldclip_tube_updn_onoff, IO_ON);
		mn_sol[0] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_ldclip_tube_updn_onoff, IO_OFF);
		mn_sol[0] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol2() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_2.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_uldclip_tube_updn_onoff, IO_ON);
		mn_sol[1] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
		mn_sol[1] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol3() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_3.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_ON);
		mn_sol[2] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_OFF);
		mn_sol[2] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol4() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_4.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_ON);
		mn_sol[3] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		mn_sol[3] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol5() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_5.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
		mn_sol[4] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
		mn_sol[4] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol6() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_6.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);
		mn_sol[5] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		mn_sol[5] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol7() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_7.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_short_updn_onoff, IO_ON);
		mn_sol[6] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
		mn_sol[6] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol8() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_8.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_uld_tube_s_updn_onoff, IO_ON);
		mn_sol[7] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
		mn_sol[7] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol9() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_9.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_tube_rotator_90_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_0_onoff, IO_OFF);
		mn_sol[8] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_tube_rotator_90_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_0_onoff, IO_ON);
		mn_sol[8] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol10() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_10.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_tube_rotator_clamp_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
		mn_sol[9] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		mn_sol[9] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol11() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_11.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_ON);
		mn_sol[10] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		mn_sol[10] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol12() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_12.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		mn_sol[11] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
		mn_sol[11] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol13() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_13.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_long_updn_onoff, IO_ON);
		mn_sol[12] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
		mn_sol[12] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol14() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_14.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_clip_remove_up_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_clip_remove_down_onoff, IO_OFF);
		mn_sol[13] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_clip_remove_up_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_clip_remove_down_onoff, IO_ON);
		mn_sol[13] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol15() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_15.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_clip_rbt_updn_onoff, IO_ON);
		mn_sol[14] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		mn_sol[14] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Clip_Clamp::OnBtnClipSol16() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_clip_sol_16.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_clip_remove_blower_on, IO_ON);
		FAS_IO.set_out_bit(st_io.o_clip_remove_blower_off, IO_OFF);
		mn_sol[15] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_clip_remove_blower_on, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_clip_remove_blower_off, IO_ON);
		mn_sol[15] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Clip_Clamp::OnBtnBufferModuleSol1() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_module_sol1.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf1_m_clamp_onoff, IO_ON);
		mn_module_buffer[0] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf1_m_clamp_onoff, IO_OFF);
		mn_module_buffer[0] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
}

void CDialog_Manual_Clip_Clamp::OnBtnBufferModuleSol2() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_module_sol2.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf2_m_clamp_onoff, IO_ON);
		mn_module_buffer[1] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf2_m_clamp_onoff, IO_OFF);
		mn_module_buffer[1] = FALSE;
	}	
	OnSiteRbt_Button_Status();	
	
}

void CDialog_Manual_Clip_Clamp::OnBtnBufferModuleSol3() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_module_sol3.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf3_m_clamp_onoff, IO_ON);
		mn_module_buffer[2] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf3_m_clamp_onoff, IO_OFF);
		mn_module_buffer[2] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Clip_Clamp::OnBtnBufferModuleSol4() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_module_sol4.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf4_m_clamp_onoff, IO_ON);
		mn_module_buffer[3] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf4_m_clamp_onoff, IO_OFF);
		mn_module_buffer[3] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Clip_Clamp::OnBtnBufferHsSol1() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_hs_sol1.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf1_hs_clamp_onoff, IO_ON);
		mn_hs_buffer[0] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf1_hs_clamp_onoff, IO_OFF);
		mn_hs_buffer[0] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Clip_Clamp::OnBtnBufferHsSol2() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_hs_sol2.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf2_hs_clamp_onoff, IO_ON);
		mn_hs_buffer[1] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
		mn_hs_buffer[1] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Clip_Clamp::OnBtnBufferHsSol3() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_hs_sol3.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf3_hs_clamp_onoff, IO_ON);
		mn_hs_buffer[2] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
		mn_hs_buffer[2] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Clip_Clamp::OnBtnBufferHsSol4() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_buffer_hs_sol4.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_buf4_hs_clamp_onoff, IO_ON);
		mn_hs_buffer[3] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
		mn_hs_buffer[3] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Clip_Clamp::OnBtnGoBufferPos() 
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

	md_mot_start_pos = st_motor[mn_motor_no].md_pos[WORK_LDCLIP];
	
	SetTimer(TMR_CLIPMOT_CMD, 10, NULL);		// 모터 동작 제어 타이머
	// **************************************************************************		
}

void CDialog_Manual_Clip_Clamp::Run_Init()
{
	int i,nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	
	if(st_handler.mn_init_state[INIT_RBT_MODULE] != CTL_NO) return;

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
		nRet_1 = COMI.HomeCheck_Mot(M_M_RBT_Z, st_motor[M_M_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);

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
			CTL_Lib.Alarm_Error_Occurrence(1101, st_work.mn_run_status, COMI.mc_alarmcode);
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
			if(ActionPicker[D_EXIST][i] == DVC_YES && st_picker.n_module_ldrbt_enable[i] == YES)
			{
				if(FAS_IO.get_in_bit(st_io.i_ld_module_dvc_chk[i], IO_ON) == IO_OFF)
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Module Picker #1 Module On Check Error.");
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
		nRet_1 = COMI.HomeCheck_Mot(M_M_RBT_Y, st_motor[M_M_RBT_Y].mn_homecheck_method, MOT_TIMEOUT);

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
				CTL_Lib.Alarm_Error_Occurrence(1102, st_work.mn_run_status, COMI.mc_alarmcode);
			}

		}
		break;


	case 3000:
		st_handler.mn_init_state[INIT_RBT_MODULE] = CTL_YES;
		InitStep = 0;
		break;		
	}
}

BOOL CDialog_Manual_Clip_Clamp::PreTranslateMessage(MSG* pMsg) 
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

void CDialog_Manual_Clip_Clamp::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	mn_stop_req = TRUE;	
	
}

int CDialog_Manual_Clip_Clamp::OnSite_Move_Excution()
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
		n_chk = COMI.Start_SingleMove(mn_motor_no, md_mot_start_pos, st_basic.nManualSpeed);
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
				sprintf(st_msg.c_normal_msg, "[MODULE SITE] Motor completed transfer.");
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
				st_other.str_abnormal_msg = _T("[MODULE SITE] Though canceled transfer of motor, error happened.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			mn_move_step = 120;
		}
		// **********************************************************************
		
		break;
	case 110 :
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_op_msg = _T("[LOADER SITE] Motor shut down during transfer.");
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
			st_other.str_abnormal_msg = _T("[LOADER SITE] Transfer command of motor failed.");
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
			st_other.str_abnormal_msg = _T("[MODULE SITE] Transfer command of motor failed.");
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

void CDialog_Manual_Clip_Clamp::OnBtnHome() 
{
	// TODO: Add your control notification handler code here
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
		mn_motor_no = M_CLIP_BUFFER;
	}
	else if(nMotorPos == 1)
	{
		mn_motor_no = M_M_CLAMP_RBT_GRIPPER;
	}
	else if(nMotorPos == 2)
	{
		mn_motor_no = M_M_CLAMP_RBT_Y;
	}
	else if(nMotorPos == 3)
	{
		mn_motor_no = M_M_CLAMP_RBT_Z;
	}
	else if(nMotorPos == 4)
	{
		mn_motor_no = M_CLIP_INSERT_Y;
	}
	else if(nMotorPos == 5)
	{
		mn_motor_no = M_CLIP_INSERT_Z;
	}
	else if(nMotorPos == 6)
	{
		mn_motor_no = M_M_CLIP_CLAMP;
	}	
	else
	{
		mn_motor_no = M_WORK_BUFFER_1 + nMotorPos - 7;
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
	
	SetTimer(TMR_CLIPMOT_CMD, 100, NULL);			//	
}

int CDialog_Manual_Clip_Clamp::OnSite_Homing_Excution()
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

/////////////////////////////////////////// 20121006
void CDialog_Manual_Clip_Clamp::OnCellClick(WPARAM wParam, LPARAM lParam)
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
		m_n_axis = M_CLIP_BUFFER;
	}
	else if(nMotorPos == 1)
	{
		m_n_axis = M_M_CLAMP_RBT_GRIPPER;
	}
	else if(nMotorPos == 2)
	{
		m_n_axis = M_M_CLAMP_RBT_Y;
	}
	else if(nMotorPos == 3)
	{
		m_n_axis = M_M_CLAMP_RBT_Z;
	}
	else if(nMotorPos == 4)
	{
		m_n_axis = M_CLIP_INSERT_Y;
	}
	else if(nMotorPos == 5)
	{
		m_n_axis = M_CLIP_INSERT_Z;
	}
	else if(nMotorPos == 6)
	{
		m_n_axis = M_M_CLIP_CLAMP;
	}	
	else
	{
		m_n_axis = M_WORK_BUFFER_1 + nMotorPos - 7;
	}

	
	if (nMotorPos < 0)	
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Module] 이동 축이 선택되지 않았습니다.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "[Manual Module] is not selected Axis");
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

LONG CDialog_Manual_Clip_Clamp::OnUserLButtonDown(WPARAM wParam, LPARAM lParam)
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
			strTmp.Format("[Motor] (%s)_Axis (%.3f) - Data Move", m_str_axis_name, md_mot_start_pos);
			Func.On_LogFile_Add(99, strTmp);
			
			SetTimer(TMR_CLIPMOT_CMD, 10, NULL);
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
			strTmp.Format("[Motor] (%s)_Axis (%.3f) + Data Move", m_str_axis_name, md_mot_start_pos);
			Func.On_LogFile_Add(99, strTmp);
			
			SetTimer(TMR_CLIPMOT_CMD, 10, NULL);
		}
	}

	return 0;
}

LONG CDialog_Manual_Clip_Clamp::OnUserLButtonUp(WPARAM wParam, LPARAM lParam)
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

// void CDialog_Manual_Module::OnBtnHome() 
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

void CDialog_Manual_Clip_Clamp::MotorMoveStepClear()
{
 	mn_stop_req = FALSE;	// ESTOP 요청 플래그 초기화
	
	m_n_cmd_no = 0;			// 실행 명령 번호 저장 변수 초기화
	m_n_cmd_motor_no = -1;	// 모터 번호 저장 변수 초기화
	m_str_cmd_motor = _T("Motor NULL");
	
 	mn_home_step = 0;		// 모터 HOMING 처리 스텝 저장 변수 초기화
 	mn_move_step = 0;		// 모터 이동 처리 스텝 저장 변수 초기화
}

///////////////////////////////////////////

void CDialog_Manual_Clip_Clamp::MotorStatusCheck()
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

void CDialog_Manual_Clip_Clamp::MotorPositionCheck()
{
//	m_dgt_current_pos.SetVal(COMI.Get_MotCurrentPos(st_part.n_motor_axis[MOTOR_PART]));
	CString str_tmp;

	m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

	str_tmp.Format("%.3f", COMI.Get_MotCurrentPos(m_n_axis));
	m_p_grid.GridCellText(m_grid_motor_info, 8, 3, str_tmp);
}


void CDialog_Manual_Clip_Clamp::OnSelchangeCmbMotorPos() 
{
	// TODO: Add your control notification handler code here
	int nMotorPos = m_cmb_motor_pos.GetCurSel();
	
	if(nMotorPos == 0)
	{
		m_n_axis = M_CLIP_BUFFER;
	}
	else if(nMotorPos == 1)
	{
		m_n_axis = M_M_CLAMP_RBT_GRIPPER;
	}
	else if(nMotorPos == 2)
	{
		m_n_axis = M_M_CLAMP_RBT_Y;
	}
	else if(nMotorPos == 3)
	{
		m_n_axis = M_M_CLAMP_RBT_Z;
	}
	else if(nMotorPos == 4)
	{
		m_n_axis = M_CLIP_INSERT_Y;
	}
	else if(nMotorPos == 5)
	{
		m_n_axis = M_CLIP_INSERT_Z;
	}
	else if(nMotorPos == 6)
	{
		m_n_axis = M_M_CLIP_CLAMP;
	}	
	else
	{
		m_n_axis = M_WORK_BUFFER_1 + nMotorPos - 7;
	}
	
	Init_Button();	
}

void CDialog_Manual_Clip_Clamp::OnBtnGoClipPos() 
{
	// TODO: Add your control notification handler code here
	int n_tray_y;
	int mn_FirstPicker_Y_Pos = 0;
	
	n_tray_y = m_cmb_clip_pos.GetCurSel();
	
	if (n_tray_y < 0)	
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg,"[Clip RBT] can not move.");
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
	mn_cmd_no = MOT_MOVE;					// 실행 명령 번호 설정 [모터 이동 명령]
	if(n_tray_y == 0 || n_tray_y == 1 || n_tray_y == 2)	mn_motor_no = M_CLIP_BUFFER;
	if(n_tray_y == 3 || n_tray_y == 4 || n_tray_y == 5)	mn_motor_no = M_M_CLAMP_RBT_GRIPPER;
	if(n_tray_y == 6 || n_tray_y == 7 || n_tray_y == 8 || n_tray_y == 9 || n_tray_y == 10) mn_motor_no = M_M_CLAMP_RBT_Y;
	if(n_tray_y == 11 || n_tray_y == 12) mn_motor_no = M_M_CLAMP_RBT_Z;
	if(n_tray_y == 13 || n_tray_y == 14 || n_tray_y == 15 || n_tray_y == 16 || n_tray_y == 17 || n_tray_y == 18 || n_tray_y == 19)	mn_motor_no = M_CLIP_INSERT_Y;
	if(n_tray_y == 20 || n_tray_y == 21 || n_tray_y == 22)	mn_motor_no = M_CLIP_INSERT_Z;
	if(n_tray_y == 23 || n_tray_y == 24 || n_tray_y == 25)	mn_motor_no = M_M_CLIP_CLAMP;
	
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
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];	
	}
	else if(n_tray_y == 4)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 5) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];
	}
	else if(n_tray_y == 6) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];
	}
	else if(n_tray_y == 7) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];
	}
	else if(n_tray_y == 8)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];	
	}
	else if(n_tray_y == 9)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[3];	
	}
	else if(n_tray_y == 10)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[4];	
	}
	else if(n_tray_y == 11)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];	
	}
	else if(n_tray_y == 12) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];
	}
	else if(n_tray_y == 13) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];	
	}
	else if(n_tray_y == 14) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 15) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];	
	}
	else if(n_tray_y == 16) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[3];	
	}
	else if(n_tray_y == 17) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[4];	
	}
	else if(n_tray_y == 18) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[5];	
	}
	else if(n_tray_y == 19) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[6];	
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
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];	
	}
	else if(n_tray_y == 24) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 25) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];	
	}

	SetTimer(TMR_CLIPMOT_CMD, 10, NULL);		// 모터 동작 제어 타이머
	// **************************************************************************	
}

void CDialog_Manual_Clip_Clamp::Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i;

	for (i = 0; i < PICKER_NUM; i++)
	{
		if(PickerInfo[D_EXIST][i] == YES)
		{
			FAS_IO.set_out_bit(st_io.o_ld_module_picker_updn[i], OnOff);
		}
		//else
		//{
		//	FAS_IO.set_out_bit(st_io.o_ld_module_picker_updn[i], !OnOff);
		//}
		b_PickerFlag[i] = FALSE;
		l_PickerWaitTime[i][0] = GetCurrentTime();
	}
}

int CDialog_Manual_Clip_Clamp::Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
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
				if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_ON) == IO_ON
					&& FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[i], IO_OFF) == IO_OFF)
				{
					b_PickerFlag[i] = TRUE;
					l_PickerWaitTime[i][0] = GetCurrentTime();   
				}
				else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_ON) == IO_ON
					&& FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[i], IO_OFF) == IO_OFF)
				{
					l_PickerWaitTime[i][1] = GetCurrentTime();  
					l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
					
					if(l_PickerWaitTime[i][2] < 0)
					{
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[38] + (i*2))
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
					
					if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[38] + (i*2))
					{
						//400200 1 40 "Module Picker #1 Down Check Error."						
						sprintf(COMI.mc_alarmcode, "40020%d", i);
						sprintf(jamcode, "40020%d", i);						
						alarm.mstr_pcode = _T(jamcode); 
						alarm.mn_count_mode = 0;	
						alarm.mn_type_mode = eWARNING; 
						alarm.mn_alarm_assign_section = 186;

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
				if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_OFF) == IO_OFF
					&& FAS_IO.Get_In_Bit(st_io.i_ld_module_picker_up_chk[i]) == IO_ON)
				{
					b_PickerFlag[i] = TRUE;
					l_PickerWaitTime[i][0] = GetCurrentTime();   
				}
				else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_OFF) == IO_OFF
					&& FAS_IO.Get_In_Bit(st_io.i_ld_module_picker_up_chk[i]) == IO_ON)
				{
					l_PickerWaitTime[i][1] = GetCurrentTime();  
					l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
					
					if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[38] + (i*2))
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
					
					if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[38] + (i*2))
					{
						if(st_handler.cwnd_list != NULL)
						{
							str = _T("[UPICKER Error] Picker up sensor check error.");
							sprintf(st_msg.c_normal_msg,"%s", str);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
							Func.On_LogFile_Add(0, str);
							Func.On_LogFile_Add(99, str);
						}
						//400100 1 40 "Module Picker #1 Up Check Error."
						sprintf(COMI.mc_alarmcode, "40010%d", i);
						sprintf(jamcode,  "40010%d", i);					
						alarm.mstr_pcode = _T(jamcode); 
						alarm.mn_count_mode = 0;	
						alarm.mn_type_mode = eWARNING; 
						//st_handler.i_run_status = dWARNING;

						alarm.mn_alarm_assign_section = 187;

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
void CDialog_Manual_Clip_Clamp::Set_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i;

	for (i = 0; i < PICKER_NUM; i++)
	{
		if(FingerInfo[D_EXIST][i] == YES)
		{
			FAS_IO.set_out_bit(st_io.o_ld_module_glipper_onoff[i], OnOff);
		}
		//else
		//{
		//	FAS_IO.set_out_bit(st_io.o_ld_module_glipper_onoff[i], !OnOff);
		//}
		b_FingerFlag[i] = false;
		l_FingerWaitTime[i][0] = GetCurrentTime();
	}
}

int CDialog_Manual_Clip_Clamp::Get_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM])
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
					
					if(l_FingerWaitTime[i][2] > st_wait.n_on_wait_time[39] + (i*2))
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
					
					if(l_FingerWaitTime[i][2] > st_wait.n_limit_wait_time[39] + (i*2))
					{
						//400500 1 40 "Module Picker #1 Module On Check Error.[PS0400]."					
						sprintf(jamcode, "40050%d", i);
						alarm.mstr_pcode = _T(jamcode); 
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
					
					if(l_FingerWaitTime[i][2] > st_wait.n_on_wait_time[39] + (i*2))
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
					
					if(l_FingerWaitTime[i][2] > st_wait.n_limit_wait_time[39] + (i*2))
					{
						//400600 1 40 "Module Picker #1 Module Off Check Error.[PS0400]."				
						sprintf(jamcode, "40060%d", i);
						alarm.mstr_pcode = _T(jamcode); 
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

void CDialog_Manual_Clip_Clamp::OnButtonControl(BOOL b_flag)
{
	
}






BOOL CDialog_Manual_Clip_Clamp::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}
	if( m_load_combo_font != NULL)
	{
		delete m_load_combo_font;
		m_load_combo_font = NULL;
	}
	if( m_motor_font != NULL)
	{
		delete m_motor_font;
		m_motor_font = NULL;
	}	
	return CDialog::DestroyWindow();
}
