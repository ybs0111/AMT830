// Dialog_Manual_Vision.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Manual_Vision.h"
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
// CDialog_Manual_Vision dialog


CDialog_Manual_Vision::CDialog_Manual_Vision(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Manual_Vision::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CDialog_Manual_Vision)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_n_move_mode = 0;
}

void CDialog_Manual_Vision::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Manual_Vision)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BTN_STOP, m_btn_stop);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btn_right);
	DDX_Control(pDX, IDC_BTN_HOME, m_btn_home);
	DDX_Control(pDX, IDC_BTN_LEFT, m_btn_left);
	DDX_Control(pDX, IDC_GROUP_VISION, m_group_vision);
	DDX_Control(pDX, IDC_GROUP_MOTOR, m_group_motor);
	DDX_Control(pDX, IDC_GROUP_VISION_RBT_MOVE, m_group_vision_rbt_move);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_1, m_msg_Vision_1);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_2, m_msg_Vision_2);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_3, m_msg_Vision_3);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_4, m_msg_Vision_4);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_5, m_msg_Vision_5);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_6, m_msg_Vision_6);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_7, m_msg_Vision_7);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_8, m_msg_Vision_8);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_9, m_msg_Vision_9);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_10, m_msg_Vision_10);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_11, m_msg_Vision_11);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_12, m_msg_Vision_12);
	DDX_Control(pDX, IDC_MSG_VISION_SOL_13, m_msg_Vision_13);
	DDX_Control(pDX, IDC_BTN_GO_VISION_POS, m_btn_go_vision_pos);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_1, m_btn_Vision_sol_1);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_2, m_btn_Vision_sol_2);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_3, m_btn_Vision_sol_3);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_4, m_btn_Vision_sol_4);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_5, m_btn_Vision_sol_5);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_6, m_btn_Vision_sol_6);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_7, m_btn_Vision_sol_7);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_8, m_btn_Vision_sol_8);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_9, m_btn_Vision_sol_9);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_10, m_btn_Vision_sol_10);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_11, m_btn_Vision_sol_11);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_12, m_btn_Vision_sol_12);
	DDX_Control(pDX, IDC_BTN_VISION_SOL_13, m_btn_Vision_sol_13);
	DDX_Control(pDX, IDC_CMB_MOTOR_POS, m_cmb_motor_pos);
	DDX_Control(pDX, IDC_CMB_VISION_POS, m_cmb_vision_pos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Manual_Vision, CDialog)
	//{{AFX_MSG_MAP(CDialog_Manual_Vision)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_1, OnBtnVisionSol1)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_2, OnBtnVisionSol2)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_3, OnBtnVisionSol3)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_4, OnBtnVisionSol4)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_5, OnBtnVisionSol5)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_6, OnBtnVisionSol6)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_7, OnBtnVisionSol7)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_8, OnBtnVisionSol8)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_9, OnBtnVisionSol9)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_10, OnBtnVisionSol10)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_12, OnBtnVisionSol12)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_11, OnBtnVisionSol11)
	ON_BN_CLICKED(IDC_BTN_GO_VISION_POS, OnBtnGoVisionPos)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_HOME, OnBtnHome)
	ON_CBN_SELCHANGE(IDC_CMB_MOTOR_POS, OnSelchangeCmbMotorPos)
	ON_BN_CLICKED(IDC_BTN_VISION_SOL_13, OnBtnVisionSol13)
	//}}AFX_MSG_MAP
/////////////////////////////20121006
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
	ON_MESSAGE(WMU_CHILD_LBUTTON_DOWN, OnUserLButtonDown)
	ON_MESSAGE(WMU_CHILD_LBUTTON_UP, OnUserLButtonUp)
/////////////////////////////

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Vision message handlers
BOOL CDialog_Manual_Vision::OnInitDialog() 
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
		m_n_axis = M_LABEL_ATTACH_Z;
	}
	else if(nMotorPos == 1)
	{
		m_n_axis = M_VISION1_Y;
	}
	else if(nMotorPos == 2)
	{
		m_n_axis = M_VISION1_Z;
	}
	else if(nMotorPos == 3)
	{
		m_n_axis = M_VISION2_Y;
	}
	else if(nMotorPos == 4)
	{
		m_n_axis = M_VISION2_Z;
	}

	if (COMI.mn_motorbd_init_end)
	{
		SetTimer(TM_STATUS_CHECK, 100, NULL);
		SetTimer(TM_POSITION_CHECK, 100, NULL);
	}

	SetTimer(TMR_VISION_IO_MONITOR_CMD, 50, NULL);		// IO ���� �б� Ÿ�̸� ����
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Manual_Vision::Init_Grid_Info()
{
	int   i, j;
	CString str_tmp;

	m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);
	
	m_p_grid.GridReset(m_grid_motor_info);
	// �빮�� 
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
				m_p_grid.GridCellText(m_grid_motor_info, 1, 1, "�� ML");
				
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
				m_p_grid.GridCellText(m_grid_motor_info, 1, 5, "�� PL");
				
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
				m_p_grid.GridCellText(m_grid_motor_info, 1, 4, "�� PL");
				
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
			m_p_grid.GridCellText(m_grid_motor_info, 1, 1, "�� ML");
			
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
			m_p_grid.GridCellText(m_grid_motor_info, 1, 5, "�� PL");
			
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

void CDialog_Manual_Vision::Init_GroupBox_Set()
{
	CSxLogFont ls_Logo(15,FW_SEMIBOLD,false,"���� ���");

	m_group_vision_rbt_move.SetFont(ls_Logo);;
	m_group_vision_rbt_move.SetCatptionTextColor(RGB(145,25,0));
	m_group_vision_rbt_move.SetFontBold(TRUE);

	m_group_motor.SetFont(ls_Logo);
	m_group_motor.SetCatptionTextColor(RGB(145,25,0));
	m_group_motor.SetFontBold(TRUE);

	m_group_vision.SetFont(ls_Logo);
	m_group_vision.SetCatptionTextColor(RGB(145,25,0));
	m_group_vision.SetFontBold(TRUE);

}

void CDialog_Manual_Vision::Init_Label()
{
	int i;
	CString str_line;
	
	m_cmb_motor_pos.SetFont(m_motor_font);
	m_cmb_motor_pos.PostMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM) 20);
	
	for (i = 0; i < 5; i++)
	{
		if(i == 0)
		{
			str_line.Format("%02d_L_Attach_Z", i + 1);
		}
		else if(i == 1)
		{
			str_line.Format("%02d_Vision1_Y", i + 1);
		}
		else if(i == 2)
		{
			str_line.Format("%02d_Vision1_Z", i + 1);
		}
		else if(i == 3)
		{
			str_line.Format("%02d_Vision2_Y", i + 1);
		}
		else if(i == 4)
		{
			str_line.Format("%02d_Vision2_Z", i + 1);
		}
		m_cmb_motor_pos.AddString(str_line);
	}

	m_cmb_motor_pos.SetCurSel(0);


	m_cmb_vision_pos.SetFont(m_motor_font);
	m_cmb_vision_pos.PostMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM) 20);
	
	for (i = 0; i < 24; i++)
	{
		if(i == 0)
		{
			str_line.Format("L_Attach_Z_Safety");
		}
		else if(i == 1)
		{
			str_line.Format("L_Attach_Z_Load BCR");
		}
		else if(i == 2)
		{
			str_line.Format("L_Attach_Z_Vision1");
		}
		else if(i == 3)
		{
			str_line.Format("L_Attach_Z_Vision2");
		}
		else if(i == 4)
		{
			str_line.Format("Vision1_Y_Ready");
		}
		else if(i == 5)
		{
			str_line.Format("Vision1_Y_Load Module Pos");
		}
		else if(i == 6)
		{
			str_line.Format("Vision1_Y_Load BCR Pos");
		}
		else if(i == 7)
		{
			str_line.Format("Vision1_Y_Vision Check");
		}
		else if(i == 8)
		{
			str_line.Format("Vision1_Y_Unload Module Pos");
		}
		else if(i == 9)
		{
			str_line.Format("Vision1_Z_Ready");
		}
		else if(i == 10)
		{
			str_line.Format("Vision1_Z_Load Module Pos");
		}
		else if(i == 11)
		{
			str_line.Format("Vision1_Z_Load BCR Pos");
		}
		else if(i == 12)
		{
			str_line.Format("Vision1_Z_Vision Check");
		}
		else if(i == 13)
		{
			str_line.Format("Vision1_Z_Unload Module Pos");
		}
		else if(i == 14)
		{
			str_line.Format("Vision2_Y_Ready");
		}
		else if(i == 15)
		{
			str_line.Format("Vision2_Y_Load Module Pos");
		}
		else if(i == 16)
		{
			str_line.Format("Vision2_Y_Load BCR Pos");
		}
		else if(i == 17)
		{
			str_line.Format("Vision2_Y_Vision Check");
		}
		else if(i == 18)
		{
			str_line.Format("Vision2_Y_Unload Module Pos");
		}
		else if(i == 19)
		{
			str_line.Format("Vision2_Z_Ready");
		}
		else if(i == 20)
		{
			str_line.Format("Vision2_Z_Load Module Pos");
		}
		else if(i == 21)
		{
			str_line.Format("Vision2_Z_Load BCR Pos");
		}
		else if(i == 22)
		{
			str_line.Format("Vision2_Z_Vision Check");
		}
		else if(i == 23)
		{
			str_line.Format("Vision2_Z_Unload Module Pos");
		}
		m_cmb_vision_pos.AddString(str_line);
	}
	m_cmb_vision_pos.SetCurSel(0);

	m_msg_Vision_1.SetFont(m_p_font);
	m_msg_Vision_1.SetWindowText(_T("Label Attach Vacuum On/Off"));
	m_msg_Vision_1.SetCenterText();
	m_msg_Vision_1.SetColor(RGB(0,0,255));
	m_msg_Vision_1.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_1.SetTextColor(RGB(255,255,255));

	m_msg_Vision_2.SetFont(m_p_font);
	m_msg_Vision_2.SetWindowText(_T("Vision Buffer1 Rotate On/Off"));
	m_msg_Vision_2.SetCenterText();
	m_msg_Vision_2.SetColor(RGB(0,0,255));
	m_msg_Vision_2.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_2.SetTextColor(RGB(255,255,255));

	m_msg_Vision_3.SetFont(m_p_font);
 	m_msg_Vision_3.SetWindowText(_T("Label Attach Eject On/Off"));
	m_msg_Vision_3.SetCenterText();
	m_msg_Vision_3.SetColor(RGB(0,0,255));
	m_msg_Vision_3.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_3.SetTextColor(RGB(255,255,255));

	m_msg_Vision_4.SetFont(m_p_font);
 	m_msg_Vision_4.SetWindowText(_T("Vision Buffer1 Clamp On/Off"));
	m_msg_Vision_4.SetCenterText();
	m_msg_Vision_4.SetColor(RGB(0,0,255));
	m_msg_Vision_4.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_4.SetTextColor(RGB(255,255,255));

	m_msg_Vision_5.SetFont(m_p_font);
 	m_msg_Vision_5.SetWindowText(_T("Vision1 BCR Reset On/Off"));
	m_msg_Vision_5.SetCenterText();
	m_msg_Vision_5.SetColor(RGB(0,0,255));
	m_msg_Vision_5.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_5.SetTextColor(RGB(255,255,255));

	m_msg_Vision_6.SetFont(m_p_font);
 	m_msg_Vision_6.SetWindowText(_T("Ionizer Air On/Off"));
	m_msg_Vision_6.SetCenterText();
	m_msg_Vision_6.SetColor(RGB(0,0,255));
	m_msg_Vision_6.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_6.SetTextColor(RGB(255,255,255));

	m_msg_Vision_7.SetFont(m_p_font);
 	m_msg_Vision_7.SetWindowText(_T("Label Attach Unit For/Back On/Off"));
	m_msg_Vision_7.SetCenterText();
	m_msg_Vision_7.SetColor(RGB(0,0,255));
	m_msg_Vision_7.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_7.SetTextColor(RGB(255,255,255));

	m_msg_Vision_8.SetFont(m_p_font);
 	m_msg_Vision_8.SetWindowText(_T("Vision Buffer2 Rotate On/Off"));
	m_msg_Vision_8.SetCenterText();
	m_msg_Vision_8.SetColor(RGB(0,0,255));
	m_msg_Vision_8.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_8.SetTextColor(RGB(255,255,255));

	m_msg_Vision_9.SetFont(m_p_font);
 	m_msg_Vision_9.SetWindowText(_T("Vision2 HS Reset On/Off"));
	m_msg_Vision_9.SetCenterText();
	m_msg_Vision_9.SetColor(RGB(0,0,255));
	m_msg_Vision_9.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_9.SetTextColor(RGB(255,255,255));

	m_msg_Vision_10.SetFont(m_p_font);
 	m_msg_Vision_10.SetWindowText(_T("Vision1 BCR Trigger On/Off"));
	m_msg_Vision_10.SetCenterText();
	m_msg_Vision_10.SetColor(RGB(0,0,255));
	m_msg_Vision_10.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_10.SetTextColor(RGB(255,255,255));

	m_msg_Vision_11.SetFont(m_p_font);
 	m_msg_Vision_11.SetWindowText(_T("Vision2 HS Trigger On/Off"));
	m_msg_Vision_11.SetCenterText();
	m_msg_Vision_11.SetColor(RGB(0,0,255));
	m_msg_Vision_11.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_11.SetTextColor(RGB(255,255,255));

	m_msg_Vision_12.SetFont(m_p_font);
	m_msg_Vision_12.SetWindowText(_T("Vision Buffer2 Clamp On/Off"));
	m_msg_Vision_12.SetCenterText();
	m_msg_Vision_12.SetColor(RGB(0,0,255));
	m_msg_Vision_12.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_12.SetTextColor(RGB(255,255,255));

	m_msg_Vision_13.SetFont(m_p_font);
	m_msg_Vision_13.SetWindowText(_T("VLPD Barcode On/Off"));
	m_msg_Vision_13.SetCenterText();
	m_msg_Vision_13.SetColor(RGB(0,0,255));
	m_msg_Vision_13.SetGradientColor(RGB(0,0,0));
	m_msg_Vision_13.SetTextColor(RGB(255,255,255));

}

void CDialog_Manual_Vision::Init_Data()
{
 	int i;

	mn_cmd_no = 0;
	mn_motor_no = 0;
	md_mot_start_pos = 0;

	for (i = 0; i < 13; i++)
	{
		mn_sol[i] = 0;
	}

//////////////////////////////////////////////////// 20121006
	m_n_axis			= st_motor_info.n_part_axis[7][AXIS_NUM];
	
	m_str_axis_name = st_motor_info.str_part_axis_name[7][AXIS_NUM];
	m_n_minus_el	= st_motor_info.n_axis_minus_el[7][AXIS_NUM];
	m_n_plus_el		= st_motor_info.n_axis_plus_el[7][AXIS_NUM];
	m_n_home		= st_motor_info.n_axis_home[7][AXIS_NUM];
	m_n_sd			= st_motor_info.n_axis_sd[7][AXIS_NUM];
	m_n_direction	= st_motor_info.n_axis_direction[7][AXIS_NUM];
	m_n_account		= st_motor_info.n_axis_account[7][AXIS_NUM];


////////////////////////////////////////////////////	
}

void CDialog_Manual_Vision::Init_Button()
{
	short	shBtnColor = 30;
	CString strTmp;
	int nMotorPos = m_cmb_motor_pos.GetCurSel();

	if(nMotorPos == 0 || nMotorPos == 1)
	{
		m_btn_left.SetIcon(IDI_UP);
	}
	else if(nMotorPos == 2 || nMotorPos == 3  || nMotorPos == 4)
	{
		m_btn_left.SetIcon(IDI_DN);
	}
// 	else
// 	{
// 		m_btn_left.SetIcon(IDI_LEFT);
// 	}	

//	m_btn_left.SetIcon(IDI_DN);
	m_btn_left.SetTag(IDC_BTN_LEFT);
	m_btn_left.SetFont(m_motor_font);
	m_btn_left.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_left.SetButtonColor(1);

	if(nMotorPos == 0 || nMotorPos == 1)
	{
		m_btn_right.SetIcon(IDI_DN);
	}
	else if(nMotorPos == 2 || nMotorPos == 3  || nMotorPos == 4)
	{
		m_btn_right.SetIcon(IDI_UP);
	}
// 	else
// 	{
// 		m_btn_right.SetIcon(IDI_RIGHT);
// 	}	
	
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
	
	m_btn_Vision_sol_1.SetFont(m_motor_font);
	m_btn_Vision_sol_1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_1.SetButtonColor(1);

	m_btn_Vision_sol_2.SetFont(m_motor_font);
	m_btn_Vision_sol_2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_2.SetButtonColor(1);

	m_btn_Vision_sol_3.SetFont(m_motor_font);
	m_btn_Vision_sol_3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_3.SetButtonColor(1);

	m_btn_Vision_sol_4.SetFont(m_motor_font);
	m_btn_Vision_sol_4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_4.SetButtonColor(1);

	m_btn_Vision_sol_5.SetFont(m_motor_font);
	m_btn_Vision_sol_5.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_5.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_5.SetButtonColor(1);

	m_btn_Vision_sol_6.SetFont(m_motor_font);
	m_btn_Vision_sol_6.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_6.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_6.SetButtonColor(1);

	m_btn_Vision_sol_7.SetFont(m_motor_font);
	m_btn_Vision_sol_7.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_7.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_7.SetButtonColor(1);

	m_btn_Vision_sol_8.SetFont(m_motor_font);
	m_btn_Vision_sol_8.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_8.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_8.SetButtonColor(1);

	m_btn_Vision_sol_9.SetFont(m_motor_font);
	m_btn_Vision_sol_9.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_9.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_9.SetButtonColor(1);

	m_btn_Vision_sol_10.SetFont(m_motor_font);
	m_btn_Vision_sol_10.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_10.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_10.SetButtonColor(1);

	m_btn_Vision_sol_11.SetFont(m_motor_font);
	m_btn_Vision_sol_11.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_11.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_11.SetButtonColor(1);

	m_btn_Vision_sol_12.SetFont(m_motor_font);
	m_btn_Vision_sol_12.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_12.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_12.SetButtonColor(1);

	m_btn_Vision_sol_13.SetFont(m_motor_font);
	m_btn_Vision_sol_13.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_btn_Vision_sol_13.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Vision_sol_13.SetButtonColor(1);

}

void CDialog_Manual_Vision::OnRead_IO_Check()
{
	if(FAS_IO.get_in_bit(st_io.o_label_attach_vacuum_onoff, IO_ON) == IO_ON)
	{
		mn_sol[0] = TRUE;
	}
	else
	{
		mn_sol[0] = FALSE;		
	}	


	if(FAS_IO.get_in_bit(st_io.o_vis_buff1_rotator_onoff, IO_ON) == IO_ON)
	{
		mn_sol[1] = TRUE;
	}
	else
	{
		mn_sol[1] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_label_attach_eject_onoff, IO_ON) == IO_ON)
	{
		mn_sol[2] = TRUE;
	}
	else
	{
		mn_sol[2] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_vis_buff1_clamp_onoff, IO_ON) == IO_ON)
	{
		mn_sol[3] = TRUE;
	}
	else
	{
		mn_sol[3] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_vis_reset1_onoff, IO_ON) == IO_ON)
	{
		mn_sol[4] = TRUE;
	}
	else
	{
		mn_sol[4] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_ionizer_onoff, IO_ON) == IO_ON)
	{
		mn_sol[5] = TRUE;
	}
	else
	{
		mn_sol[5] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_label_attach_unit_fwd_onoff, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.o_label_attach_unit_bwd_onoff, IO_OFF) == IO_OFF)
	{
		mn_sol[6] = TRUE;
	}
	else
	{
		mn_sol[6] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_vis_buff2_rotator_onoff, IO_ON) == IO_ON)
	{
		mn_sol[7] = TRUE;
	}
	else
	{
		mn_sol[7] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_vis_trigger1_onoff, IO_ON) == IO_ON)
	{
		mn_sol[9] = TRUE;
	}
	else
	{
		mn_sol[9] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_vis_buff2_clamp_onoff, IO_ON) == IO_ON)
	{
		mn_sol[11] = TRUE;
	}
	else
	{
		mn_sol[11] = FALSE;		
	}	

	if(FAS_IO.get_in_bit(st_io.o_vlp_bcr_clamp_onoff, IO_ON) == IO_ON)
	{
		mn_sol[12] = TRUE;
	}
	else
	{
		mn_sol[12] = FALSE;		
	}	

}

void CDialog_Manual_Vision::OnSiteRbt_Button_Status()//20121006
{
	short	shBtnColor = 30;
	
	if (mn_sol[0] < 0 || mn_sol[0] > 1) mn_sol[0] = TRUE;
	m_btn_Vision_sol_1.SetCheck(mn_sol[0]);
	if (mn_sol[0] == TRUE)
	{
		m_btn_Vision_sol_1.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_1, "Off");
		m_btn_Vision_sol_1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_1.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_1.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_1, "On");
		m_btn_Vision_sol_1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_1.SetButtonColor(1);
	}

	if (mn_sol[1] < 0 || mn_sol[1] > 1) mn_sol[1] = TRUE;
	m_btn_Vision_sol_2.SetCheck(mn_sol[1]);
	if (mn_sol[1] == TRUE)
	{
		m_btn_Vision_sol_2.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_2, "Off");
		m_btn_Vision_sol_2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_2.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_2.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_2, "On");
		m_btn_Vision_sol_2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_2.SetButtonColor(1);
	}

	if (mn_sol[2] < 0 || mn_sol[2] > 1) mn_sol[2] = TRUE;
	m_btn_Vision_sol_3.SetCheck(mn_sol[2]);
	if (mn_sol[2] == TRUE)
	{
		m_btn_Vision_sol_3.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_3, "Off");
		m_btn_Vision_sol_3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_3.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_3.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_3, "On");
		m_btn_Vision_sol_3.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_3.SetButtonColor(1);
	}

	if (mn_sol[3] < 0 || mn_sol[3] > 1) mn_sol[3] = TRUE;
	m_btn_Vision_sol_4.SetCheck(mn_sol[3]);
	if (mn_sol[3] == TRUE)
	{
		m_btn_Vision_sol_4.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_4, "On");
		m_btn_Vision_sol_4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_4.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_4.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_4, "Off");
		m_btn_Vision_sol_4.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_4.SetButtonColor(1);
	}

	if (mn_sol[4] < 0 || mn_sol[4] > 1) mn_sol[4] = TRUE;
	m_btn_Vision_sol_5.SetCheck(mn_sol[4]);
	if (mn_sol[4] == TRUE)
	{
		m_btn_Vision_sol_5.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_5, "Off");
		m_btn_Vision_sol_5.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_5.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_5.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_5.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_5, "On");
		m_btn_Vision_sol_5.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_5.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_5.SetButtonColor(1);
	}

	if (mn_sol[5] < 0 || mn_sol[5] > 1) mn_sol[5] = TRUE;
	m_btn_Vision_sol_6.SetCheck(mn_sol[5]);
	if (mn_sol[5] == TRUE)
	{
		m_btn_Vision_sol_6.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_6, "Off");
		m_btn_Vision_sol_6.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_6.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_6.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_6.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_6, "On");
		m_btn_Vision_sol_6.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_6.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_6.SetButtonColor(1);
	}

	if (mn_sol[6] < 0 || mn_sol[6] > 1) mn_sol[6] = TRUE;
	m_btn_Vision_sol_7.SetCheck(mn_sol[6]);
	if (mn_sol[6] == TRUE)
	{
		m_btn_Vision_sol_7.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_7, "Off");
		m_btn_Vision_sol_7.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_7.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_7.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_7.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_7, "On");
		m_btn_Vision_sol_7.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_7.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_7.SetButtonColor(1);
	}

	if (mn_sol[7] < 0 || mn_sol[7] > 1) mn_sol[7] = TRUE;
	m_btn_Vision_sol_8.SetCheck(mn_sol[7]);
	if (mn_sol[7] == TRUE)
	{
		m_btn_Vision_sol_8.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_8, "Off");
		m_btn_Vision_sol_8.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_8.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_8.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_8.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_8, "On");
		m_btn_Vision_sol_8.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_8.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_8.SetButtonColor(1);
	}

	if (mn_sol[8] < 0 || mn_sol[8] > 1) mn_sol[8] = TRUE;
	m_btn_Vision_sol_9.SetCheck(mn_sol[6]);
	if (mn_sol[8] == TRUE)
	{
		m_btn_Vision_sol_9.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_9, "Off");
		m_btn_Vision_sol_9.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_9.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_9.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_9.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_9, "On");
		m_btn_Vision_sol_9.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_9.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_9.SetButtonColor(1);
	}

	if (mn_sol[9] < 0 || mn_sol[9] > 1) mn_sol[9] = TRUE;
	m_btn_Vision_sol_10.SetCheck(mn_sol[9]);
	if (mn_sol[9] == TRUE)
	{
		m_btn_Vision_sol_10.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_10, "Off");
		m_btn_Vision_sol_10.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_10.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_10.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_10.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_10, "On");
		m_btn_Vision_sol_10.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_10.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_10.SetButtonColor(1);
	}

	if (mn_sol[10] < 0 || mn_sol[10] > 1) mn_sol[10] = TRUE;
	m_btn_Vision_sol_11.SetCheck(mn_sol[10]);
	if (mn_sol[10] == TRUE)
	{
		m_btn_Vision_sol_11.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_11, "Off");
		m_btn_Vision_sol_11.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_11.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_11.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_11.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_11, "On");
		m_btn_Vision_sol_11.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_11.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_11.SetButtonColor(1);
	}

	if (mn_sol[11] < 0 || mn_sol[11] > 1) mn_sol[11] = TRUE;
	m_btn_Vision_sol_12.SetCheck(mn_sol[11]);
	if (mn_sol[11] == TRUE)
	{
		m_btn_Vision_sol_12.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_12, "On");
		m_btn_Vision_sol_12.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_12.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_12.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_12.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_12, "Off");
		m_btn_Vision_sol_12.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_12.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_12.SetButtonColor(1);
	}

	if (mn_sol[12] < 0 || mn_sol[12] > 1) mn_sol[12] = TRUE;
	m_btn_Vision_sol_13.SetCheck(mn_sol[12]);
	if (mn_sol[12] == TRUE)
	{
		m_btn_Vision_sol_13.SetCheck(TRUE);
		SetDlgItemText(IDC_BTN_VISION_SOL_13, "On");
		m_btn_Vision_sol_13.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_13.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_13.SetButtonColor(1);
	}
	else																					// Off���̸�
	{
		m_btn_Vision_sol_13.SetCheck(FALSE);
		SetDlgItemText(IDC_BTN_VISION_SOL_13, "Off");
		m_btn_Vision_sol_13.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_btn_Vision_sol_13.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_btn_Vision_sol_13.SetButtonColor(1);
	}
}

void CDialog_Manual_Vision::OnSite_Controls_Enable(BOOL iFlag)
{
	m_btn_go_vision_pos.EnableWindow(iFlag);
	m_btn_Vision_sol_1.EnableWindow(iFlag);
	m_btn_Vision_sol_2.EnableWindow(iFlag);
	m_btn_Vision_sol_3.EnableWindow(iFlag);
	m_btn_Vision_sol_4.EnableWindow(iFlag);
	m_btn_Vision_sol_5.EnableWindow(iFlag);
	m_btn_Vision_sol_6.EnableWindow(iFlag);
	m_btn_Vision_sol_7.EnableWindow(iFlag);
	m_btn_Vision_sol_8.EnableWindow(iFlag);
	m_btn_Vision_sol_9.EnableWindow(iFlag);
	m_btn_Vision_sol_10.EnableWindow(iFlag);
	m_btn_Vision_sol_11.EnableWindow(iFlag);
	m_btn_Vision_sol_12.EnableWindow(iFlag);
	m_btn_Vision_sol_13.EnableWindow(iFlag);

}

void CDialog_Manual_Vision::OnSite_Step_Clear()
{
	mn_stop_req = FALSE;  // ESTOP ��û �÷��� �ʱ�ȭ

	mn_cmd_no = 0;        // ���� ��� ��ȣ ���� ���� �ʱ�ȭ

	mn_home_step = 0;     // ���� HOMING ó�� ���� ���� ���� �ʱ�ȭ
	mn_move_step = 0;     // ���� �̵� ó�� ���� ���� ���� �ʱ�ȭ
}


void CDialog_Manual_Vision::OnTimer(UINT nIDEvent) 
{
	int n_response,n_check;
//	int nRet[5];

	CDialog_Message msg_dlg;

	if (nIDEvent == TMR_VISION_MOT_CMD) 
	{
		// **************************************************************************
		// ��� ���� �ʱ�ȭ �۾� �Ϸ� ���� �˻��Ѵ�                                  
		// **************************************************************************
		if (st_handler.mn_mot_brd_initial != TRUE)  
		{
			KillTimer(TMR_VISION_MOT_CMD) ;  // Ÿ�̸� ����
			mn_cmd_no = 0;				// ���� ��� ��ȣ ���� ���� �ʱ�ȭ

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
			n_check = OnSite_Homing_Excution();		// ���� HOMING ó�� �Լ�

			if(n_check == CTLBD_RET_GOOD)
			{
				KillTimer(TMR_VISION_MOT_CMD) ;  // Ÿ�̸� ���� //
				if(st_handler.mn_home_state[mn_motor_no] != TRUE)
				{
					st_handler.mn_home_state[mn_motor_no] = TRUE;
				}				
				mn_cmd_no = 0;  // ���� ��� ��ȣ ���� ���� �ʱ�ȭ //				
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================

				OnSite_Controls_Enable(TRUE);  // ��� ��Ʈ�� ȭ�� ��� ���� �Լ� //
			}
			else if(n_check == CTLBD_RET_ERROR)
			{
				KillTimer(TMR_VISION_MOT_CMD) ;  // Ÿ�̸� ���� //
				
				mn_cmd_no = 0;  // ���� ��� ��ȣ ���� ���� �ʱ�ȭ //
				
				// ==============================================================================
				// �޴� ��� �����ϵ��� �÷��� �����Ѵ�
				// -> st_handler.i_menu_lock   : �޴� ��� ���� ���� �÷���
				//  : ���� �������� �޴� ��ȯ �κп��� �� �÷��� �˻��ϰ� �ȴ�
				// ==============================================================================
				if(st_handler.mn_menu_lock != FALSE)
				{
					st_handler.mn_menu_lock = FALSE;
				}
				// ==============================================================================
				OnSite_Controls_Enable(TRUE);  // ��� ��Ʈ�� ȭ�� ��� ���� �Լ� //
			}
		}
		else if (mn_cmd_no == MOT_MOVE)
		{
			n_check = OnSite_Move_Excution();			// ���� �̵� ��� ó�� �Լ�
			if (n_check == CTLBD_RET_GOOD)   
			{
				KillTimer(TMR_VISION_MOT_CMD) ;  // Ÿ�̸� ����
				mn_cmd_no = 0;  // ���� ��� ��ȣ ���� ���� �ʱ�ȭ

				// ******************************************************************
				if (st_handler.mn_menu_lock != FALSE)
					st_handler.mn_menu_lock = FALSE;
				// ******************************************************************

				OnSite_Controls_Enable(TRUE);  // ��� ��Ʈ�� ȭ�� ��� ���� �Լ�
			}
			else if (n_check == CTLBD_RET_ERROR) 
			{
				KillTimer(TMR_VISION_MOT_CMD) ;  // Ÿ�̸� ����

				mn_cmd_no = 0;  // ���� ��� ��ȣ ���� ���� �ʱ�ȭ

				if (st_handler.mn_menu_lock != FALSE)
					st_handler.mn_menu_lock = FALSE;
				// ******************************************************************

				OnSite_Controls_Enable(TRUE);  // ��� ��Ʈ�� ȭ�� ��� ���� �Լ�
			}	
		}
		else 
		{
			KillTimer(TMR_VISION_MOT_CMD) ;  // Ÿ�̸� ����
		}


	}
// 	else if (nIDEvent == TMR_Vision_IO_MONITOR_CMD) 
//	{
//		OnSite_IO_Status_Read();
//	}
	else if (nIDEvent == TMR_VISION_STEPMOT_CMD) 
	{
		Run_Init();
		if (st_handler.mn_init_state[INIT_RBT_MODULE] == CTL_YES)
		{
			KillTimer(TMR_VISION_STEPMOT_CMD);
			OnSite_Controls_Enable(TRUE);  // ��� ��Ʈ�� ȭ�� ��� ���� �Լ�			
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

void CDialog_Manual_Vision::OnBtnVisionSol1() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_1.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_ON);
		mn_sol[0] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_OFF);
		mn_sol[0] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Vision::OnBtnVisionSol2() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_2.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_vis_buff1_rotator_onoff, IO_ON);
		mn_sol[1] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_vis_buff1_rotator_onoff, IO_OFF);
		mn_sol[1] = FALSE;
	}	
	OnSiteRbt_Button_Status();		

}

void CDialog_Manual_Vision::OnBtnVisionSol3() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_3.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_ON);
		mn_sol[2] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
		mn_sol[2] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
	
}

void CDialog_Manual_Vision::OnBtnVisionSol4() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_4.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_vis_buff1_clamp_onoff, IO_ON);
		mn_sol[3] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_vis_buff1_clamp_onoff, IO_OFF);
		mn_sol[3] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
	
}


void CDialog_Manual_Vision::OnBtnVisionSol5() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_5.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_vis_reset1_onoff, IO_ON);
		mn_sol[4] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_vis_reset1_onoff, IO_OFF);
		mn_sol[4] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
	
}

void CDialog_Manual_Vision::OnBtnVisionSol6() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_6.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_ionizer_onoff, IO_ON);
		mn_sol[5] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_ionizer_onoff, IO_OFF);
		mn_sol[5] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
	
}

void CDialog_Manual_Vision::OnBtnVisionSol7() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_7.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_label_attach_unit_fwd_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_label_attach_unit_bwd_onoff, IO_OFF);
		mn_sol[6] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
		mn_sol[6] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
	
}

void CDialog_Manual_Vision::OnBtnVisionSol8() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_8.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_vis_buff2_rotator_onoff, IO_ON);
		mn_sol[7] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_vis_buff2_rotator_onoff, IO_OFF);
		mn_sol[7] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
	
}

void CDialog_Manual_Vision::OnBtnVisionSol9() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_9.GetCheck() == 1)
	{
		Func.OnSet_Vision_send(HEATSINK_TRIGGER_RESET);
		mn_sol[8] = TRUE;
	}
	else
	{
		Func.OnSet_Vision_send(HEATSINK_TRIGGER_RESET);
		mn_sol[8] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
}

void CDialog_Manual_Vision::OnBtnVisionSol10() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_9.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_vis_trigger1_onoff, IO_ON);
		mn_sol[9] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_vis_trigger1_onoff, IO_OFF);
		mn_sol[9] = FALSE;
	}	
	OnSiteRbt_Button_Status();			
}

void CDialog_Manual_Vision::OnBtnVisionSol11() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_11.GetCheck() == 1)
	{
		Func.OnSet_Vision_send(HEATSINK_TRIGGER);
		mn_sol[10] = TRUE;
	}
	else
	{
		Func.OnSet_Vision_send(HEATSINK_TRIGGER);
		mn_sol[10] = FALSE;
	}	
	OnSiteRbt_Button_Status();			
	
}

void CDialog_Manual_Vision::OnBtnVisionSol12() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_12.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_vis_buff2_clamp_onoff, IO_ON);
		mn_sol[11] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_vis_buff2_clamp_onoff, IO_OFF);
		mn_sol[11] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
	
}

void CDialog_Manual_Vision::OnBtnVisionSol13() 
{
	// TODO: Add your control notification handler code here
	if (m_btn_Vision_sol_13.GetCheck() == 1)
	{
		FAS_IO.set_out_bit(st_io.o_vlp_bcr_clamp_onoff, IO_ON);
		mn_sol[12] = TRUE;
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);
		mn_sol[12] = FALSE;
	}	
	OnSiteRbt_Button_Status();		
	
}

BOOL CDialog_Manual_Vision::PreTranslateMessage(MSG* pMsg) 
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


void CDialog_Manual_Vision::OnBtnGoVisionPos() 
{
	// TODO: Add your control notification handler code here
	int n_tray_y;
	int mn_FirstPicker_Y_Pos = 0;
	
	////////////////////// 20121107
	int	nRet;	
	int n_response;	
	CDialog_Message  msg_dlg;
	////////////////////// 
	n_tray_y = m_cmb_vision_pos.GetCurSel();
	
	if (n_tray_y < 0)	
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg,"[Vision RBT] can not move.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� �Ϸ� ��� ��û
		}
		return;
	}
	
	OnSite_Controls_Enable(FALSE);  // ��� ��Ʈ�� ȭ�� ��� ���� �Լ�
	
	if (st_handler.mn_menu_lock != TRUE)
		st_handler.mn_menu_lock = TRUE;
	// **************************************************************************
	
	OnSite_Step_Clear();				// ���� ���� ó�� ���� �ʱ�ȭ �Լ�
	// **************************************************************************
	// ���� �̵� ��ġ �����Ѵ�                                                   
	// **************************************************************************

	////////////////////// 20121107
	if(n_tray_y == 0 || n_tray_y == 1 || n_tray_y == 2 || n_tray_y == 3)
	{
	}
	else
	{
		nRet = COMI.Check_SingleMove(M_LABEL_ATTACH_Z, Z_LABEL_ATTACH_SAFETY);
		if (nRet != CTLBD_RET_GOOD)
		{
			st_msg.str_fallacy_msg = _T("Z���� Safety ��ġ�� �ƴմϴ�.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH)
			{
				st_msg.str_fallacy_msg = _T("Safety is not the position of the Z-axis");
			}
			
			n_response = msg_dlg.DoModal();
			OnSite_Controls_Enable(TRUE);  // ��� ��Ʈ�� ȭ�� ��� ���� �Լ�
			st_handler.mn_menu_lock = FALSE;
			return;

		}
		else
		{
		}
	}
	//////////////////////

	mn_cmd_no = MOT_MOVE;					// ���� ��� ��ȣ ���� [���� �̵� ���]
	if(n_tray_y == 0 || n_tray_y == 1 || n_tray_y == 2 || n_tray_y == 3)	mn_motor_no = M_LABEL_ATTACH_Z;
	if(n_tray_y == 4 || n_tray_y == 5 || n_tray_y == 6 || n_tray_y == 7 || n_tray_y == 8) mn_motor_no = M_VISION1_Y;
	if(n_tray_y == 9 || n_tray_y == 10 || n_tray_y == 11 || n_tray_y == 12 || n_tray_y == 13) mn_motor_no = M_VISION1_Z;
	if(n_tray_y == 14 || n_tray_y == 15 || n_tray_y == 16 || n_tray_y == 17 || n_tray_y == 18) mn_motor_no = M_VISION2_Y;
	if(n_tray_y == 19 || n_tray_y == 20 || n_tray_y == 21 || n_tray_y == 22 || n_tray_y == 23) mn_motor_no = M_VISION2_Z;

	
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
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];	
	}
	else if(n_tray_y == 5)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 6) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];
	}
	else if(n_tray_y == 7) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[3];
	}
	else if(n_tray_y == 8) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[4];
	}
	else if(n_tray_y == 9)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];	
	}
	else if(n_tray_y == 10)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 11)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];	
	}
	else if(n_tray_y == 12)
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[3];	
	}
	else if(n_tray_y == 13) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[4];
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
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[0];	
	}
	else if(n_tray_y == 20) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[1];	
	}
	else if(n_tray_y == 21) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[2];	
	}
	else if(n_tray_y == 22) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[3];	
	}
	else if(n_tray_y == 23) 
	{
		md_mot_start_pos = st_motor[mn_motor_no].md_pos[4];	
	}

	SetTimer(TMR_VISION_MOT_CMD, 10, NULL);		// ���� ���� ���� Ÿ�̸�
	// **************************************************************************		
}

void CDialog_Manual_Vision::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	mn_stop_req = TRUE;	
	
}

void CDialog_Manual_Vision::OnBtnHome() 
{
	// TODO: Add your control notification handler code here
	CString strTmp;
	int nMotorPos = 0;

	OnSite_Controls_Enable(FALSE);  // ��� ��Ʈ�� ȭ�� ��� ���� �Լ�

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
		mn_motor_no = M_LABEL_ATTACH_Z;
	}
	else if(nMotorPos == 1)
	{
		mn_motor_no = M_VISION1_Y;
	}
	else if(nMotorPos == 2)
	{
		mn_motor_no = M_VISION1_Z;
	}
	else if(nMotorPos == 3)
	{
		mn_motor_no = M_VISION2_Y;
	}
	else if(nMotorPos == 4)
	{
		mn_motor_no = M_VISION2_Z;
	}


	// ==============================================================================
	// -> HOMING �۾� ó�� �Լ��� CPublic_Function Ŭ������ ����                 
	// ==============================================================================
	st_handler.mn_home_state[mn_motor_no] = CTL_READY;	// ���� HOMING �Ϸ� �÷���
	// ==============================================================================
	
	st_motor[mn_motor_no].mn_home_step	= 0;
	COMI.mn_home_step[mn_motor_no] = 0;

	strTmp.Format("[Motor] (%d)_Axis Home Move", mn_motor_no);
	Func.On_LogFile_Add(99, strTmp);
	
	SetTimer(TMR_VISION_MOT_CMD, 100, NULL);			//		
}

void CDialog_Manual_Vision::OnSelchangeCmbMotorPos() 
{
	// TODO: Add your control notification handler code here
	int nMotorPos = m_cmb_motor_pos.GetCurSel();
	
	if(nMotorPos == 0)
	{
		m_n_axis = M_LABEL_ATTACH_Z;
	}
	else if(nMotorPos == 1)
	{
		m_n_axis = M_VISION1_Y;
	}
	else if(nMotorPos == 2)
	{
		m_n_axis = M_VISION1_Z;
	}
	else if(nMotorPos == 3)
	{
		m_n_axis = M_VISION2_Y;
	}
	else if(nMotorPos == 4)
	{
		m_n_axis = M_VISION2_Z;
	}
	
	Init_Button();		
}

/////////////////////////////////////////// 20121006
void CDialog_Manual_Vision::OnCellClick(WPARAM wParam, LPARAM lParam)
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
		m_n_axis = M_LABEL_ATTACH_Z;
	}
	else if(nMotorPos == 1)
	{
		m_n_axis = M_VISION1_Y;
	}
	else if(nMotorPos == 2)
	{
		m_n_axis = M_VISION1_Z;
	}
	else if(nMotorPos == 3)
	{
		m_n_axis = M_VISION2_Y;
	}
	else if(nMotorPos == 4)
	{
		m_n_axis = M_VISION2_Z;
	}

	
	if (nMotorPos < 0)	
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Module] �̵� ���� ���õ��� �ʾҽ��ϴ�.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "[Manual Module] was not selected Axis");
			}

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� �Ϸ� ��� ��û
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
LONG CDialog_Manual_Vision::OnUserLButtonDown(WPARAM wParam, LPARAM lParam)
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

	// Velocity-Move(Jog) ����϶��� WM_LBUTTONDOWN �̺�Ʈ�� ó���Ѵ�. //
	// Rel/Abs Position Mode������ WM_LBUTTONUP �̺�Ʈ���� �̼� ����� //
	// ó���Ѵ�.                                                       //
	CButtonST *pButton = (CButtonST *)lParam;

	nRet = pButton->GetTag();

	if (pButton->GetTag() == IDC_BTN_LEFT)
	{ // "Jog (-)" ��ư�� ���� ���...
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
			
			COMI.Set_MotSpeed(MOT_SPD_JOG, m_n_axis, cmSMODE_KEEP, st_motor[m_n_axis].md_spd_jog, st_motor[m_n_axis].md_spd_acc, st_motor[m_n_axis].md_spd_dec);		// ���� �ӵ��� �����Ѵ�.
			
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
			// ���� �̵����� ���� üũ �߰� �ؾߵ�......
			OnButtonControl(FALSE);							// ��ư ��Ȱ��ȭ
			MotorMoveStepClear();							// �������� �Լ� ���� �ʱ�ȭ
			
			mn_motor_no = m_n_cmd_motor_no	= m_n_axis;					// ���� ��ȣ ����
			mn_cmd_no = MOT_MOVE;					// ���� ��� ��ȣ ���� [���� �̵� ���]
//			m_n_cmd_no			= MOVING;							// ���� ��� ��ȣ ���� [���� �̵� ���]
			m_d_mot_goto_spd	= (st_motor[m_n_cmd_motor_no].md_spd_jog / COMI.md_spd_vel[m_n_cmd_motor_no][0]) * 100;
			m_str_cmd_motor		= m_str_axis_name;
			
			dmot_curr_pos		= COMI.Get_MotCurrentPos(m_n_axis);
			
//			m_d_mot_goto_pos	= dmot_curr_pos - m_d_data;
			md_mot_start_pos	= dmot_curr_pos - m_d_data;
			strTmp.Format("[Motor] (%s)_Axis (%.3f) - Data Move", m_str_axis_name, md_mot_start_pos);
			Func.On_LogFile_Add(99, strTmp);
			
			SetTimer(TMR_VISION_MOT_CMD, 10, NULL);
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
			
			OnButtonControl(FALSE);						// ��ư ��Ȱ��ȭ
			
			strTmp.Format("[Motor] (%s)_Axis + Jog Move", m_str_axis_name);
			Func.On_LogFile_Add(99, strTmp);
			
			COMI.Set_MotSpeed(MOT_SPD_JOG, m_n_axis, cmSMODE_KEEP, st_motor[m_n_axis].md_spd_jog, st_motor[m_n_axis].md_spd_acc, st_motor[m_n_axis].md_spd_dec);		// ���� �ӵ��� �����Ѵ�.
			
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
			// ���� �̵����� ���� üũ �߰� �ؾߵ�......				
			OnButtonControl(FALSE);						// ��ư ��Ȱ��ȭ
			MotorMoveStepClear();							// �������� �Լ� ���� �ʱ�ȭ

			mn_motor_no = m_n_cmd_motor_no	= m_n_axis;					// ���� ��ȣ ����
			mn_cmd_no = MOT_MOVE;					// ���� ��� ��ȣ ���� [���� �̵� ���]
//			m_n_cmd_no			= MOVING;							// ���� ��� ��ȣ ���� [���� �̵� ���]
			m_d_mot_goto_spd	= (st_motor[m_n_cmd_motor_no].md_spd_jog / COMI.md_spd_vel[m_n_cmd_motor_no][0]) * 100;
			m_str_cmd_motor		= m_str_axis_name;
			
			dmot_curr_pos		= COMI.Get_MotCurrentPos(m_n_axis);
			
//			m_d_mot_goto_pos	= dmot_curr_pos + m_d_data;
			md_mot_start_pos	= dmot_curr_pos + m_d_data;			
			strTmp.Format("[Motor] (%s)_Axis (%.3f) + Data Move", m_str_axis_name, md_mot_start_pos);
			Func.On_LogFile_Add(99, strTmp);
			
			SetTimer(TMR_VISION_MOT_CMD, 10, NULL);
		}
	}

	return 0;
}

LONG CDialog_Manual_Vision::OnUserLButtonUp(WPARAM wParam, LPARAM lParam)
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

	// Velocity-Move(Jog) ����϶��� WM_LBUTTONDOWN �̺�Ʈ�� ó���Ѵ�. //
	// Rel/Abs Position Mode������ WM_LBUTTONUP �̺�Ʈ���� �̼� ����� //
	// ó���Ѵ�.                                                       //
	CButtonST *pButton = (CButtonST *)lParam;

	nRet = pButton->GetTag();

	if (pButton->GetTag() == IDC_BTN_LEFT)
	{ // "Jog (-)" ��ư�� ���� ���...
		if (m_n_move_mode == 0)//20120716 0->1
		{
			strTmp.Format("[Motor] (%s)_Axis - Jog Stop", m_str_axis_name);
			Func.On_LogFile_Add(99, strTmp);
			
			cmmSxStop(m_n_axis, FALSE, TRUE);  // ���� �̵� ���� �Լ� 
			
			OnButtonControl(TRUE);	
		}
	}
	else if (pButton->GetTag() == IDC_BTN_RIGHT)
	{ 
		if (m_n_move_mode == 0)//20120716 0->1
		{
			strTmp.Format("[Motor] (%s)_Axis + Jog Stop", m_str_axis_name);
			Func.On_LogFile_Add(99, strTmp);
			
			cmmSxStop(m_n_axis, FALSE, TRUE);  // ���� �̵� ���� �Լ� 
			
			OnButtonControl(TRUE);	
		}
	}
	return 0;
}

// void CDialog_Manual_Module::OnBtnHome() 
// {
// 	CString strTmp;
// 
// 	OnButtonControl(FALSE);  // ��� ��Ʈ�� ȭ�� ��� ���� �Լ�
// 	
// 	// ==============================================================================
// 	// �޴� ��� �Ұ����ϵ��� �÷��� �����Ѵ�                                    
// 	// -> ���� ���� ���� �߿� �ٸ� �޴� ���� ���۽�Ű�� �������� �� �ֱ� �����̴�
// 	// -> st_handler.i_menu_lock   : �޴� ��� ���� ���� �÷���                  
// 	//  : ���� �������� �޴� ��ȯ �κп��� �� �÷��� �˻��ϰ� �ȴ�               
// 	// ==============================================================================
// 	if (st_handler.mn_menu_lock != TRUE)
// 	{
// 		st_handler.mn_menu_lock = TRUE;
// 	}
// 	// ==============================================================================
// 	
// 	// ==============================================================================
// 	// -> HOMING �۾� ó�� �Լ��� CPublic_Function Ŭ������ ����                 
// 	// ==============================================================================
// 	st_handler.mn_home_state[m_n_axis] = CTL_READY;	// ���� HOMING �Ϸ� �÷���
// 	// ==============================================================================
// 	
// 	MotorMoveStepClear();							// �������� �Լ� ���� �ʱ�ȭ
// 	
// 	m_n_cmd_motor_no						= m_n_axis;					// ���� ��ȣ ����
// 	m_n_cmd_no								= HOMING;							// ���� ��� ��ȣ ���� [���� �̵� ���]
// 	m_str_cmd_motor							= m_str_axis_name;
// 	
// 	strTmp.Format("[Motor] (%s)_Axis Home Move", m_str_axis_name);
// 	Func.On_LogFile_Add(99, strTmp);
// 
// 	st_motor[m_n_cmd_motor_no].mn_home_step	= 0;
// 	COMI.mn_home_step[m_n_cmd_motor_no] = 0;
// 	
// 	SetTimer(TM_HOME_CHECK, 100, NULL);			// ���� ���� ���� Ÿ�̸�
//}

void CDialog_Manual_Vision::MotorMoveStepClear()
{
 	mn_stop_req = FALSE;	// ESTOP ��û �÷��� �ʱ�ȭ
	
	m_n_cmd_no = 0;			// ���� ��� ��ȣ ���� ���� �ʱ�ȭ
	m_n_cmd_motor_no = -1;	// ���� ��ȣ ���� ���� �ʱ�ȭ
	m_str_cmd_motor = _T("Motor NULL");
	
 	mn_home_step = 0;		// ���� HOMING ó�� ���� ���� ���� �ʱ�ȭ
 	mn_move_step = 0;		// ���� �̵� ó�� ���� ���� ���� �ʱ�ȭ
}

///////////////////////////////////////////

void CDialog_Manual_Vision::MotorStatusCheck()
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

void CDialog_Manual_Vision::MotorPositionCheck()
{
//	m_dgt_current_pos.SetVal(COMI.Get_MotCurrentPos(st_part.n_motor_axis[MOTOR_PART]));
	CString str_tmp;

	m_grid_motor_info = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_INFO);

	str_tmp.Format("%.3f", COMI.Get_MotCurrentPos(m_n_axis));
	m_p_grid.GridCellText(m_grid_motor_info, 8, 3, str_tmp);
}

void CDialog_Manual_Vision::Run_Init()
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
		//  UP ��Ų �Ǹ����� CHECK�Ѵ�.                                 
		// *************************************************************
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);

		if (nRet_1 == RET_GOOD)			// ���������� Up�� ���� ���.
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

	case 1000:		//z axis���� homecheck ����!!
		// *************************************************************
		//  Ȩüũ�� �Ѵ�.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_M_RBT_Z, st_motor[M_M_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// ���������� Home Check�� ������ ���.
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
		if(l_until_wait_time[2] < 0)	l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
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

		if(nRet_1 == CTLBD_RET_GOOD)			// ���������� Home Check�� ������ ���.
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

int CDialog_Manual_Vision::OnSite_Move_Excution()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet_1 = CTLBD_RET_PROCEED, n_chk;	
	
	// **************************************************************************
	// ���� ���� �� ESTOP ��û �߻��ߴ��� �˻��Ѵ�                               
	// -> ESTOP ��û �߻��� ��� ��� ������ ���� �����                         
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
		// ���� Ư�� ��ġ�� �̵���Ų��. (�̵���ġ ����~)                         
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
			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_normal_msg, "[MODULE SITE] Motor completed transfer.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
			}

			if (mn_stop_req != FALSE)	mn_stop_req = FALSE;
			mn_move_step = 0;
			nFuncRet = CTLBD_RET_GOOD;
		}
		else if (n_chk == CTLBD_RET_ERROR)
		{
			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			{
				//st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Motor failed to move.");
				sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Motor failed to move.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			nFuncRet = CTLBD_RET_ERROR;
		}
		else if (n_chk == CTLBD_RET_SAFETY)
		{
			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			{
				//st_msg.mstr_abnormal_msg = _T("[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
				sprintf(st_msg.c_abnormal_msg, "[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			nFuncRet = CTLBD_RET_ERROR;
		}
		break;

	case 100 :
		// **********************************************************************
		// Ư�� �� ��� ���� �̺�Ʈ�� ���� �����Ѵ�                              
		// **********************************************************************
		n_chk = COMI.Set_MotStop(0, mn_motor_no);
		
		if (n_chk == TRUE)
			mn_move_step = 110;
		else 
		{
			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			{
				st_other.str_abnormal_msg = _T("[MODULE SITE] Though canceled transfer of motor, error happened.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
			}
			mn_move_step = 120;
		}
		// **********************************************************************
		
		break;
	case 110 :
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_other.str_op_msg = _T("[LOADER SITE] Motor shut down during transfer.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
		}

		if (mn_stop_req != FALSE)
			mn_stop_req = FALSE;  // ESTOP ��û �÷��� �ʱ�ȭ

		mn_move_step = 0;
		nFuncRet = CTLBD_RET_GOOD;
		break;

	case 120 :
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_other.str_abnormal_msg = _T("[LOADER SITE] Transfer command of motor failed.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}

		if (mn_stop_req != FALSE)
			mn_stop_req = FALSE;  // ESTOP ��û �÷��� �ʱ�ȭ

		mn_move_step = 0; 
		nFuncRet = CTLBD_RET_ERROR;
		break;

	case 130 :
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_other.str_abnormal_msg = _T("[MODULE SITE] Transfer command of motor failed.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}

		if (mn_stop_req != FALSE)
			mn_stop_req = FALSE;  // ESTOP ��û �÷��� �ʱ�ȭ

		mn_move_step = 0; 
		nFuncRet = CTLBD_RET_ERROR;
		break;
	}

	return nFuncRet;
}

int CDialog_Manual_Vision::OnSite_Homing_Excution()
{
	int n_home_flag = CTLBD_RET_PROCEED;
	int nRet;

	// =============================================================================
	// ���� ���� �� ESTOP ��û �߻��ߴ��� �˻��Ѵ�
	// -> ESTOP ��û �߻��� ��� ��� ������ ���� �����
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

			if (nRet == CTLBD_RET_GOOD)				// ���������� Home Check�� ������ ���.
			{
				if (mn_stop_req != FALSE)
					mn_stop_req = FALSE;		// ESTOP ��û �÷��� �ʱ�ȭ
				
				mn_home_step = 0; 
				n_home_flag = CTLBD_RET_GOOD;
			}
			else if (nRet == CTLBD_RET_ERROR)		// Home Check�� ���� ���� ���.
			{
				if (mn_stop_req != FALSE)
					mn_stop_req = FALSE;		// ESTOP ��û �÷��� �ʱ�ȭ
				
				mn_home_step = 0; 
				n_home_flag = CTLBD_RET_ERROR;
			}
			else if (nRet == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
				{
					//st_msg.mstr_abnormal_msg = _T("[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					sprintf(st_msg.c_abnormal_msg, "[SAFETY] Do not move being thought that is not safe at relevant motor difference.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}
				mn_home_step = 0; 
				n_home_flag = CTLBD_RET_ERROR;
			}
			break;

		case 100 :
			// =============================================================================
			// Ư�� �� ��� ���� �̺�Ʈ�� ���� �����Ѵ�
			// =============================================================================
			nRet = COMI.Set_MotStop(0, mn_motor_no);

			if (nRet == CTLBD_RET_GOOD)
			{
				mn_home_step = 110;
			}
			else 
			{
				if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
				{
					//st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Though canceled transfer of motor, error happened.");
					sprintf(st_msg.c_abnormal_msg, "[ + mn_motor_no + ] Though canceled transfer of motor, error happened.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
				}
				
				mn_home_step = 120;
			}
			break;

		case 110 :
			if (st_handler.cwnd_list != NULL)
			{  // ����Ʈ �� ȭ�� ���� //
			//	st_msg.mstr_normal_msg = _T("[" + m_str_cmd_motor + "] Motor shut down during transfer.");
				sprintf(st_msg.c_normal_msg, "[ + mn_motor_no + ] Motor shut down during transfer.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û //
			}

			//if (m_nstop_req != FALSE){
			if (mn_stop_req){
				mn_stop_req = FALSE;		// ESTOP ��û �÷��� �ʱ�ȭ
			}

			mn_home_step = 0;
			n_home_flag = CTLBD_RET_GOOD;
			break;

		case 120 :
			if (st_handler.cwnd_list != NULL)
			{  // ����Ʈ �� ȭ�� ���� //

				//st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				sprintf(st_msg.c_abnormal_msg, "[ + mn_motor_no + ] Transfer command of motor failed.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û //
			}

			//if (m_nstop_req != FALSE){
			if (mn_stop_req)
			{
				mn_stop_req = FALSE;	// ESTOP ��û �÷��� �ʱ�ȭ //
			}

			st_motor[mn_motor_no].mn_home_step = 0;
			mn_home_step = 0; 
			n_home_flag = CTLBD_RET_ERROR;
			break;

		case 130 :
			if (st_handler.cwnd_list != NULL)
			{  // ����Ʈ �� ȭ�� ���� //
				//st_msg.mstr_abnormal_msg = _T("[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				sprintf(st_msg.c_abnormal_msg, "[" + m_str_cmd_motor + "] Transfer command of motor failed.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û //
			}

			//if (m_nstop_req != FALSE){
			if (mn_stop_req)
			{
				mn_stop_req = FALSE;	// ESTOP ��û �÷��� �ʱ�ȭ //
			}

			st_motor[mn_motor_no].mn_home_step = 0;
			mn_home_step = 0; 
			n_home_flag = CTLBD_RET_ERROR;
			break;
	}

	return  n_home_flag;
}

void CDialog_Manual_Vision::Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
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

int CDialog_Manual_Vision::Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
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

		if(OnOff == IO_ON) //Picker ON (Down�̶��..)
		{
			if(PickerInfo[D_EXIST][i] == YES) //���� ��Ų ��Ŀ��!
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
					
					if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[39] + (i*2))
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
		else //Picker OFF (UP�̶��..)
		{
			if(PickerInfo[D_EXIST][i] == YES) //���� ��Ų ��Ŀ��!
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
					
					if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[39] + (i*2))
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
					
					if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[39] + (i*2))
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

void CDialog_Manual_Vision::Set_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM])
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

int CDialog_Manual_Vision::Get_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i, FuncRet = RET_PROCEED;
	int RetVal[PICKER_NUM] = {0,};
	char jamcode[10] = {0,};
	 
	for(i = 0 ; i < PICKER_NUM ; i++)
	{
		RetVal[i] = NO;

		if(OnOff == IO_ON) //Finger ON (place���..)
		{
			if(FingerInfo[D_EXIST][i] == YES) //���� ��Ų �ΰŸ�!
			{
				//��¥�� ������ �����Ƿ� ���� �ð� �ڿ� ���� �Ϸ�� ����!!
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
		else //Finger OFF (Pick�̶��..)
		{
			if(FingerInfo[D_EXIST][i] == YES)
			{
				//��¥�� ������ �����Ƿ� ���� �ð� �ڿ� ���� �Ϸ�� ����!!
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

void CDialog_Manual_Vision::OnButtonControl(BOOL b_flag)
{
	
}



BOOL CDialog_Manual_Vision::DestroyWindow() 
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
