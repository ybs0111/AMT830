// Dialog_Part_Manual.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Part_Manual.h"
#include "Cmmsdk.h"
#include "CtlBd_Function.h"
#include "FastechPublic_IO.h"
#include "ComizoaPublic.h"
#include "Public_Function.h"
#include "CtlBd_Library.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int M_STACKER_1 = 0;
const int M_STACKER_2 = 1;
const int M_STACKER_MOVE = 2; // 20130213
///////////////////////////// 20120718
const int HSL_STACKER_1 = 0;
const int HSL_STACKER_2 = 1;
const int HSR_STACKER_1 = 0;
const int HSR_STACKER_2 = 1;
const int U_STACKER_1 = 0;
const int U_STACKER_2 = 1;
/////////////////////////////
#define TMR_CLIPTEST_CHECK		768
#define TMR_STACKER_CHECK		769
#define TMR_VISION_CHECK		770
#define TMR_STACKER_CHECK2		771//20120718 HS Left
#define TMR_STACKER_CHECK3		772//20120718 HS Right
#define TMR_STACKER_CHECK4		773//20120718 Unloader
#define TMR_STACKER_CHECK5		774//20120723 Unloader
#define TMR_MODULEMENEAL_CHECK  775//20130213 ModuleMenual 
#define TMR_STACKER1_CHECK      776
#define TMR_MODULERBT_CHECK		777//20130225 MDRbt
#define TMR_CLIPINIT_CHECK		778//20130320

/////////////////////////////////////////////////////////////////////////////
// CDialog_Part_Manual dialog


CDialog_Part_Manual::CDialog_Part_Manual(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Part_Manual::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Part_Manual)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Func.m_p_manual = NULL;
}


void CDialog_Part_Manual::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Part_Manual)
	DDX_Control(pDX, IDC_BTN_VISION_END, m_btn_vision_end);
	DDX_Control(pDX, IDC_BTN_VISION_RUN, m_btn_vision_run);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_12, m_btn_io_move_12);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_11, m_btn_io_move_11);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_10, m_btn_io_move_10);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_9, m_btn_io_move_9);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_8, m_btn_io_move_8);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_7, m_btn_io_move_7);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_6, m_btn_io_move_6);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_5, m_btn_io_move_5);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_4, m_btn_io_move_4);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_3, m_btn_io_move_3);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_2, m_btn_io_move_2);
	DDX_Control(pDX, IDC_BTN_IO_MOVE_1, m_btn_io_move_1);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_12, m_btn_motor_move_12);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_11, m_btn_motor_move_11);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_10, m_btn_motor_move_10);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_9, m_btn_motor_move_9);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_8, m_btn_motor_move_8);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_7, m_btn_motor_move_7);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_6, m_btn_motor_move_6);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_5, m_btn_motor_move_5);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_4, m_btn_motor_move_4);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_3, m_btn_motor_move_3);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_2, m_btn_motor_move_2);
	DDX_Control(pDX, IDC_BTN_MOTOR_MOVE_1, m_btn_motor_move_1);
	DDX_Control(pDX, IDC_GROUP_MOTOR, m_group_motor);
	DDX_Control(pDX, IDC_GROUP_IO, m_group_io);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Part_Manual, CDialog)
	//{{AFX_MSG_MAP(CDialog_Part_Manual)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_1, OnBtnMotorMove1)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_2, OnBtnMotorMove2)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_3, OnBtnMotorMove3)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_4, OnBtnMotorMove4)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_5, OnBtnMotorMove5)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_1, OnBtnMotorMove1)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_2, OnBtnMotorMove2)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_3, OnBtnMotorMove3)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_4, OnBtnMotorMove4)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_5, OnBtnMotorMove5)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_6, OnBtnMotorMove6)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_7, OnBtnMotorMove7)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_8, OnBtnMotorMove8)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_9, OnBtnMotorMove9)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_10, OnBtnMotorMove10)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_11, OnBtnMotorMove11)
	ON_BN_CLICKED(IDC_BTN_MOTOR_MOVE_12, OnBtnMotorMove12)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_1, OnBtnIoMove1)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_2, OnBtnIoMove2)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_3, OnBtnIoMove3)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_4, OnBtnIoMove4)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_5, OnBtnIoMove5)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_6, OnBtnIoMove6)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_7, OnBtnIoMove7)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_8, OnBtnIoMove8)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_9, OnBtnIoMove9)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_10, OnBtnIoMove10)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_11, OnBtnIoMove11)
	ON_BN_CLICKED(IDC_BTN_IO_MOVE_12, OnBtnIoMove12)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_VISION_RUN, OnBtnVisionRun)
	ON_BN_CLICKED(IDC_BTN_VISION_END, OnBtnVisionEnd)
	ON_BN_CLICKED(IDC_BTN_CLIPTEST_RUN, OnBtnCliptestRun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Part_Manual message handlers

BOOL CDialog_Part_Manual::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rectArea;
	
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(16,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	
	GetClientRect(rectArea);
	MoveWindow(m_cp_coord.x, m_cp_coord.y, rectArea.Width(), rectArea.Height(), TRUE);

	OnPManual_GroupBox_Set();
	OnPManual_Button_Set();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Part_Manual::OnDestroy() 
{
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}

	KillTimer(TMR_CLIPTEST_CHECK);
	KillTimer(TMR_STACKER_CHECK);
	KillTimer(TMR_VISION_CHECK);
	CDialog::OnDestroy();
}

void CDialog_Part_Manual::OnBtnExit() 
{
	m_b_stop_req = TRUE;
	DestroyWindow();
}

BOOL CDialog_Part_Manual::Create()
{
	return CDialog::Create( CDialog_Part_Manual::IDD );
}

void CDialog_Part_Manual::PostNcDestroy() 
{
	delete this;
	Func.m_p_manual = NULL;
	
	CDialog::PostNcDestroy();
}

void CDialog_Part_Manual::OnPManual_GroupBox_Set()
{
	CSxLogFont manual_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_motor.SetFont(manual_font);
	m_group_motor.SetCatptionTextColor(RGB(145,25,0));
	m_group_motor.SetFontBold(TRUE);

	m_group_io.SetFont(manual_font);
	m_group_io.SetCatptionTextColor(RGB(145,25,0));
	m_group_io.SetFontBold(TRUE);
}

void CDialog_Part_Manual::OnPManual_Button_Set()
{
//	switch (m_n_part)
//	{
//	case M_LD_TRAY_ELEVATOR:
//		m_btn_motor_move_1.SetWindowText("Goto Top");
//		break;
//	
//	case M_TRAY_TRANSFER:
//		m_btn_motor_move_1.SetWindowText("Goto AirBlow Box");
//		break;
//	
//	case M_ROBOT_X:
//		m_btn_motor_move_1.SetWindowText("Goto Front Buffer");
//
//		break;
//	
//	case M_ROBOT_Y:
//		m_btn_motor_move_1.SetWindowText("Goto Front Buffer");
//		break;
//	
//	case M_ROBOT_Z:
//		m_btn_motor_move_1.SetWindowText("Goto Buffer Pick");
//		break;
//	
//	case M_ROBOT_PITCH:
//		m_btn_motor_move_1.SetWindowText("Goto Work");
//		break;
//	
//	case M_ULD_TRAY_ELEVATOR:
//		m_btn_motor_move_1.SetWindowText("Goto Base");
//		break;
//	}
}

void CDialog_Part_Manual::OnTimer(UINT nIDEvent) 
{
	int nResponse, nRet[5];
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TMR_STACKER_CHECK)
	{
		nResponse = StackerMovingHome();
//		nResponse = StackerMovingExcution();
		if(nResponse == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_STACKER_CHECK);
		}
		else if (nResponse != CTLBD_RET_PROCEED)
		{
			KillTimer(TMR_STACKER_CHECK);
		}
	}
///////////////////// 20120718
	else if(nIDEvent == TMR_STACKER_CHECK2)
	{
		nResponse = StackerMovingExcution2();
		if(nResponse == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_STACKER_CHECK2);
		}
		else if (nResponse != CTLBD_RET_PROCEED)
		{
			KillTimer(TMR_STACKER_CHECK2);
		}
	}
///////////////////// 20120719
	else if(nIDEvent == TMR_STACKER_CHECK3)
	{
		nResponse = StackerMovingExcution3();
		if(nResponse == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_STACKER_CHECK3);
		}
		else if (nResponse != CTLBD_RET_PROCEED)
		{
			KillTimer(TMR_STACKER_CHECK3);
		}
	}
///////////////////// 20120721
	else if(nIDEvent == TMR_STACKER_CHECK4)
	{
 		nResponse = StackerMovingExcution4();
 		if(nResponse == CTLBD_RET_GOOD)
 		{
 			KillTimer(TMR_STACKER_CHECK4);
 		}
 		else if (nResponse != CTLBD_RET_PROCEED)
 		{
 			KillTimer(TMR_STACKER_CHECK4);
 		}
	}

///////////////////// 20120723
	else if(nIDEvent == TMR_STACKER_CHECK5)
	{
		InitRightStacker();
		nRet[1] = Stacker1Move();
		nRet[2] = Stacker2Move();
		if(mn_Error_stop == YES)
		{
			m_b_stop_req = YES;
			KillTimer(TMR_STACKER_CHECK5);
			EnableButton(TRUE);
		}
		if(nRet[1] == RET_ERROR || nRet[2] == RET_ERROR)//|| nRet[3] == RET_ERROR || nRet[4] == RET_ERROR)
		{
			m_b_stop_req = YES;
		}
	}
///////////////////// 
	
	
	else if(nIDEvent == TMR_CLIPTEST_CHECK)
	{
		nRet[0] = InitClip();
		nRet[1] = LoadClipMove();
		nRet[2] = LoadClipTransfer();
		nRet[3] = ClipInsertMove();
		nRet[4] = ClipBufferMove();
		if(mn_Error_stop == YES)
		{
			m_b_stop_req = YES;
			KillTimer(TMR_CLIPTEST_CHECK);
			EnableButton(TRUE);
		}
		if(nRet[0] == RET_ERROR || nRet[1] == RET_ERROR || nRet[2] == RET_ERROR || nRet[3] == RET_ERROR || nRet[4] == RET_ERROR)
		{
			m_b_stop_req = YES;
		}
	}
	else if (nIDEvent == TMR_VISION_CHECK)
	{
		nRet[0] = InitVision();
		nRet[1] = Label_Test();
		nRet[2] = Module_Vision_Top();
		nRet[3] = Module_Vision_Btm();
		if(mn_Error_stop == YES)
		{
			m_b_stop_req = YES;
			KillTimer(TMR_VISION_CHECK);
			EnableButton(TRUE);
		}
		if(nRet[0] == RET_ERROR || nRet[1] == RET_ERROR || nRet[2] == RET_ERROR || nRet[3] == RET_ERROR)
		{
			m_b_stop_req = YES;
		}
	}	

/////////////////////////////////////////// 20130213
	else if(nIDEvent == TMR_MODULEMENEAL_CHECK)
	{
		nResponse = ModuleStackerMenualMoving();
		if(nResponse == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_MODULEMENEAL_CHECK);
		}
		else if (nResponse != CTLBD_RET_PROCEED)
		{
			KillTimer(TMR_MODULEMENEAL_CHECK);
		}
	}

	else if(nIDEvent == TMR_STACKER1_CHECK)
	{
		nResponse = Stacker1MovingCheck();
		
		if(nResponse == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_STACKER1_CHECK);
		}
		else if (nResponse != CTLBD_RET_PROCEED)
		{
			KillTimer(TMR_STACKER1_CHECK);
		}
	}

 	else if(nIDEvent == TMR_MODULERBT_CHECK)
 	{
 		nResponse = Module_Rbt_Home();
 		if(nResponse == CTLBD_RET_GOOD)
 		{
 			KillTimer(TMR_MODULERBT_CHECK);
 		}
 		else if(nResponse != CTLBD_RET_PROCEED)
 		{
 			KillTimer(TMR_MODULERBT_CHECK);
 		}
 	}
//////////////////////////////////////////////////////////////////////////
	else if(nIDEvent == TMR_CLIPINIT_CHECK)    // 20130320
	{
		nResponse = ClipPlaceInit();
		if(nResponse == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_CLIPINIT_CHECK);
		}
		else if(nResponse != CTLBD_RET_PROCEED)
		{
			KillTimer(TMR_CLIPINIT_CHECK);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

int CDialog_Part_Manual::StackerMovingExcution()//SD체크
{
	int n_move_flag = CTLBD_RET_PROCEED;
	int nRet_1, nRet_2;

	mn_MotorNum = m_n_motor_no;
	mn_StackerPos = M_STACKER_1;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// =============================================================================
	if (m_b_stop_req)
	{
		m_n_move_stacker_step = 3000;
	}
	// =============================================================================

	switch(m_n_move_stacker_step)
	{
		case 0 :
//			if(mn_MotorNum == M_LDM_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_ON); // 임시 20130213
//			if(nRet_1 == IO_OFF)
//			{				
//				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
//				{				
//					//000101 0 00 "Stacker1에 트레이가 존재하지 않습니다."
//					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재하지 않습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
//				}
//				n_move_flag = CTLBD_RET_ERROR;
//				m_n_move_stacker_step = 20;
//			}
//			else
//			{
				m_n_move_stacker_step = 20;
//			}
			break;

		case 20:
			if(mn_MotorNum == M_LDM_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 있습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker1");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_move_stacker_step = 25;
			}
			break;

		case 25:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_onoff_chk, IO_OFF);
			nRet_2 = FAS_IO.get_in_bit(st_io.i_m_uld_slide_onoff_chk, IO_OFF);
			if(nRet_1 == IO_ON && nRet_2 == IO_ON)
			{
				m_n_move_stacker_step = 30;
			}
			else
			{//000802 0 01 "Load Module Slide가 Open되어 있습니다. Close 해 주세요."
			 //000804 0 01 "Unload Module Slide가 Open되어 있습니다. Close 해 주세요."
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;


		case 30:
			nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step = 100;
			}
			break;

		case 100:
			nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_n_move_stacker_step = 200;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not backward");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 200:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
			
			if (nRet_1 == IO_OFF)
			{
//				FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_ON);
//				FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_OFF);
				m_n_move_stacker_step = 1000;
			}
			else// if (nRet == IO_OFF)
			{
				m_n_move_stacker_step = 2000;
			}
			break;
		
		case 210:
			FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_OFF);
			m_n_move_stacker_step = 300;
			break;

		case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker_guide_clamp_onoff, st_io.i_m_stacker_guide_clamp_on_chk, st_io.i_m_stacker_guide_unclamp_off_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_ON);
				m_n_move_stacker_step = 400;
			}
//			else 
//			{
//				m_n_move_stacker_step = 210;
//			}
			break;

		case 310:
			FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_ON);
			m_n_move_stacker_step = 400;
			break;

		case 400:
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker_guide_clamp_onoff, st_io.i_m_stacker_guide_unclamp_off_chk, st_io.i_m_stacker_guide_clamp_on_chk,
				IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step = 1000;
			}
			else if(nRet_1 != RET_PROCEED)
			{
				m_n_move_stacker_step = 310;
			}
			break;


		case 1000:
			nRet_1 = OnModuleStackerReadyPos(mn_StackerPos,1); //SD 대기
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step = 1100;
			}
			else if(nRet_1 == CTL_FULL)
			{
				//000302 1 01 "Loader Stocker 1 Tray Full 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Module Stocker 1 Tray Full error.");

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				n_move_flag = CTLBD_RET_ERROR;
			}
			else if(nRet_1 == CTLBD_RET_SKIP) //스태커에 트레이가 없을때 
			{//000301 0 00 "Loader Stocker 1에 트레이 없음 에러"
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Module Stocker 1에 트레이 없음 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "No Tray Module Stocker 1 on error");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;


		case 1100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000100 0 00 "Stacker1에 트레이가 존재합니다.제거해 주세요."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재합니다.제거해 주세요.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 the tray is present., Please remove");
					}
					

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_move_stacker_step = 1300;
			}
			break;

		case 1300:
			if ((FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_fwd_chk + (mn_StackerPos), IO_OFF) == IO_OFF) && 
				(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_bwd_chk + (mn_StackerPos), IO_ON) == IO_ON))	// Off 상태.
			{
				m_n_move_stacker_step = 1500;
			}
			else
			{
				nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
				if(nRet_1 == CYLINDER_ON)
				{
					m_n_move_stacker_step = 1400;
				}
			}
			break;

		case 1310:
			nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step = 1400;
			}
			break;

			
		case 1400:
			nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_n_move_stacker_step = 1500;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if(mn_StackerPos == 0)
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not backward");
					}

				}
				else 
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward");
					}

				}
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
				m_n_move_stacker_step = 1310;
			}
			break;

		case 1500:
			nRet_1 = OnModuleStackerReadyPos(mn_StackerPos, 0); //트레이 하나 Up한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step = 1600;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_n_move_stacker_step = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 1600:
			nRet_1 = COMI.Get_MotIOSensor(mn_MotorNum, MOT_SENS_SD);
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
//				st_sync.mn_ld_module_tray_supply[mn_StackerPos] = CTL_FREE;		// 교체 끝났다고 설정.
//				m_n_move_stacker_step = 300;	
				m_n_move_stacker_step = 1700;	
			}
			/////////////// 20120719
			else if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON) == IO_ON && nRet_1 == CTLBD_RET_ERROR)
			{
				m_n_move_stacker_step = 1700;
			}
    		///////////////

			else
			{
				m_n_move_stacker_step = 1100;
			}
			break;	

		case 1700:
			nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
			if(nRet_1 == CTL_GOOD)
			{
				m_n_move_stacker_step = 1800;
			}
			break;

		case 1800:
			nRet_1 = Get_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step = 2000;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{	
				//000203 0 00 "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."			
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 forward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not forward");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;	
				m_n_move_stacker_step = 1700;
			}
			break;


		case 2000:
			nRet_1 = OnModuleStackerReadyPos(mn_StackerPos, 2); //트레 하나 Dn한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step = 2100;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_n_move_stacker_step = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 2100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON);
			
			if (nRet_1 == IO_ON)	// 트레이가 있다
			{
				m_n_move_stacker_step = 3000;
			}
			else
			{
				//000201 0 00 "Stacker1 레일에 트레이가 존재하지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail does not exist in the tray");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_n_move_stacker_step = 2100;
			}
			break;

		case 3000:
			m_n_move_stacker_step = 0;
			n_move_flag = CTLBD_RET_GOOD;
			break;
	}


	return  n_move_flag;
}

void CDialog_Part_Manual::MoveStepClear() 
{
	m_n_move_stacker_step = 0;
	ReadyPosStep = 0;
	m_b_stop_req = FALSE;
	m_b_run_req = FALSE;
	///////////////////////////// Module Steacker Test 20130213

	m_n_Stacker1 = 0;
	m_n_Stacker2 = 0;
	m_n_Stacker_Move = 0;
	m_n_motor_no11 = 0;
	m_n_motor_no12 = 0;
	m_n_MotorMove_Step = 0;
	m_n_StackerPos1 = 0;
	m_n_StackerPos2 = 0;
	m_n_Stackerstep = 0;
	st_handler.n_Init_Stacker = CTL_READY;
	m_n_Stacker1step = 0;
	m_n_StackerMoveStep = 0; 
	mn_MDRbt_step = 0;
	
	///////////////////////////// HS Left Stacker 20120718
	m_n_move_stacker_step2 = 0;
	ReadyPosStep2 = 0;
	///////////////////////////// HS Right Stacker 20120719
	m_n_move_stacker_step3 = 0;
	ReadyPosStep3 = 0;
	///////////////////////////// ULD Stacker 20120721
	m_n_move_stacker_step4 = 0;
	ReadyPosStep4 = 0;
	///////////////////////////// ULD Stacker 20120724
	m_move_stacker_step1 = 0;
	m_move_stacker_step2 = 0;

	///////////////////////////// MD Robot 20130225

	m_n_Rbt_Y = 0;
	m_n_Rbt_Z = 0;
	mn_Stacker_MovingStep = 0;
	mn_TrayBackStep = 0;
	mn_TrayMoveStep = 0;
	mn_S2Step = 0;
	mn_S2MoveStep = 0;
	TrayPlaceStep = 0;
	RunStep = 0;
	Tray_Pick_Step = 0;
	i_PartialPickCnt = 0;
	PosPlus = 0;

	///////////////////////////// 20130320

	ClipStep = 0;
	ClipRbtStep = 0;

	/////////////////////////////

	//VisionTest
	mn_labelStep = 0;
	mn_VisionStep[0] = mn_VisionStep[1] = 0;
	mn_InitStep = 0;
	mn_Visinitial = FALSE;
	md_offset = 0.0;
	mn_attach_err = NO;
	mn_Error_stop = NO;

	//Clip Test
	mn_Clipinitial = FALSE;
	InsertStep = 0;
	BufferStep = 0;
	ClipPickStep = 0;
	ClipPlaceStep = 0;
	ClipBufferStep = 0;
	LoadClipStep = 0;
	ClipMoveStep = 0;
	PushStep = 0;
	RotatorStep = 0;
	FixStep = 0;
	Change_step = 0;
	ClipMoveStep = 0;
	mn_Tuberetry = 0;
	mn_Clipretry = 0;
	ClipInitStep = 0;
	ClipStackerStep = 0;
	RunTransStep = 0;
	TransUpDnStep = 0;
	TransferMoveStep = 0;
	mn_Clip1_init = FALSE;
	mn_Clip2_init = FALSE;

	n_lotend_righths_ldtray_site = NO;
	n_lotend_clip_ldtray_site = NO;
}
void CDialog_Part_Manual::OnBtnMotorMove1() 
{//Load Stacker
	MoveStepClear();

	m_n_motor_no = M_LDM_STACKER_1;				// 모터 번호 설정
	st_motor[m_n_motor_no].mn_home_step	= 0;
	
	// ==============================================================================
	// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
	// ==============================================================================
	st_handler.mn_home_state[m_n_motor_no] = CTL_READY;	// 모터 HOMING 완료 플래그
	// ==============================================================================
	//Func.On_LogFile_Add(99, strTmp);	
	SetTimer(TMR_STACKER_CHECK, 100, NULL);			// 모터 동작 제어 타이머

}

void CDialog_Part_Manual::OnBtnMotorMove2() 
{//Unload#1 Staker
	MoveStepClear();

	m_n_motor_no = M_ULDM_STACKER_1;				// 모터 번호 설정
	st_motor[m_n_motor_no].mn_home_step	= 0;
	
	// ==============================================================================
	// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
	// ==============================================================================
	st_handler.mn_home_state[m_n_motor_no] = CTL_READY;	// 모터 HOMING 완료 플래그
	// ==============================================================================
	//Func.On_LogFile_Add(99, strTmp);	
	SetTimer(TMR_STACKER_CHECK4, 100, NULL);			// 	
}

void CDialog_Part_Manual::OnBtnMotorMove3() 
{
	
	CDialog_Message msg_dlg;
	int mn_response;
	if(m_b_run_req == YES)
	{
		st_msg.str_fallacy_msg = _T("Clip 테스트중입니다. 다시 시작할까요?");
		mn_response = msg_dlg.DoModal();
		if (mn_response != IDOK) return;
	}
	else
	{
		st_msg.str_fallacy_msg = _T("Clip 테스트 하시겠습니까?");
		mn_response = msg_dlg.DoModal();
		if (mn_response != IDOK) return;
	}

	EnableButton(FALSE);
	MoveStepClear();
	
	m_b_run_req = YES;
	m_btn_vision_run.EnableWindow(FALSE);
	SetTimer(TMR_CLIPTEST_CHECK, 100, NULL);			
}

void CDialog_Part_Manual::OnBtnMotorMove4() //HS Left Stacker
{
	MoveStepClear();

	m_n_motor_no = M_LDM_STACKER_1;				// 모터 번호 설정
	st_motor[m_n_motor_no].mn_home_step	= 0;
	
	// ==============================================================================
	// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
	// ==============================================================================
	st_handler.mn_home_state[m_n_motor_no] = CTL_READY;	// 모터 HOMING 완료 플래그
	// ==============================================================================
	//Func.On_LogFile_Add(99, strTmp);	
	SetTimer(TMR_STACKER1_CHECK, 100, NULL);			// 모터 동작 제어 타이머
	
//HS Left Stacker
// 	// TODO: Add your control notification handler code here
// 	MoveStepClear();
// 
// 	m_n_motor_no2 = M_HS_F_STACKER_1;				// 모터 번호 설정
// 	st_motor[m_n_motor_no2].mn_home_step	= 0;
// 	
// 	// ==============================================================================
// 	// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
// 	// ==============================================================================
// 	st_handler.mn_home_state[m_n_motor_no2] = CTL_READY;	// 모터 HOMING 완료 플래그
// 	// ==============================================================================
// 	//Func.On_LogFile_Add(99, strTmp);	
// 	SetTimer(TMR_STACKER_CHECK2, 100, NULL);			// 모터 동작 제어 타이머
	
}

void CDialog_Part_Manual::OnBtnMotorMove5() 
{
///////////////////////////////////////////////////////////////////////////////// 20130225
	MoveStepClear();

	m_n_Rbt_Y = M_M_RBT_Y;
	m_n_Rbt_Z = M_M_RBT_Z;

	st_motor[m_n_Rbt_Y].mn_home_step	= 0;
	st_motor[m_n_Rbt_Z].mn_home_step	= 0;

	st_handler.mn_home_state[m_n_Rbt_Y] = CTL_READY;	// 모터 HOMING 완료 플래그
	st_handler.mn_home_state[m_n_Rbt_Z] = CTL_READY;	// 모터 HOMING 완료 플래그

	SetTimer(TMR_MODULERBT_CHECK, 100, NULL);
///////////////////////////////////////////////////////////////////////////////////
//HS Right Stacker
// 	// TODO: Add your control notification handler code here
// 	MoveStepClear();
// 
// 	m_n_motor_no3 = M_HS_B_STACKER_1;				// 모터 번호 설정
// 	st_motor[m_n_motor_no3].mn_home_step	= 0;
// 	
// 	// ==============================================================================
// 	// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
// 	// ==============================================================================
// 	st_handler.mn_home_state[m_n_motor_no3] = CTL_READY;	// 모터 HOMING 완료 플래그
// 	// ==============================================================================
// 	//Func.On_LogFile_Add(99, strTmp);	
// 	SetTimer(TMR_STACKER_CHECK3, 100, NULL);			// 모터 동작 제어 타이머
	
}

void CDialog_Part_Manual::OnBtnMotorMove6() 
{
	MoveStepClear();

	SetTimer(TMR_CLIPINIT_CHECK, 100, NULL);			// 모터 동작 제어 타이머
}

void CDialog_Part_Manual::OnBtnMotorMove7() 
{//Right Stacker Run
// 	// TODO: Add your control notification handler code here
// 	MoveStepClear();
// 
// 	COMI.Set_MotStop(0, M_HS_B_STACKER_1);
// 	CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_1] = 0;
// 	COMI.Set_MotStop(0, M_HS_B_STACKER_2);
// 	CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_2] = 0;
// 	COMI.Set_MotStop(0, M_HS_B_STACKER_MOVE);
// 	CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_MOVE] = 0;
// 	COMI.mn_home_step[M_HS_B_STACKER_1] = 0;
// 	COMI.mn_home_step[M_HS_B_STACKER_2] = 0;
// 	COMI.mn_home_step[M_HS_B_STACKER_MOVE] = 0;
// 
// 	SetTimer(TMR_STACKER_CHECK5, 100, NULL);			// 모터 동작 제어 타이머
	
}

void CDialog_Part_Manual::OnBtnMotorMove8() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnMotorMove9() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnMotorMove10() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnMotorMove11() 
{
//	MoveStepClear();
//
//	m_n_motor_no	 = M_LDM_STACKER_1;				// 모터 번호 설정
//	m_n_motor_no11	 = M_LDM_STACKER_2;             // 모터 번호 설정 
//	m_n_motor_no12	 = M_LDM_STACKER_MOVE;          // 모터 번호 설정
//
//	st_motor[m_n_Stacker1].mn_home_step	= 0;
//	st_motor[m_n_Stacker2].mn_home_step	= 0;
//	st_motor[m_n_Stacker_Move].mn_home_step	= 0;	
//	// ==============================================================================
//	// -> HOMING 작업 처리 함수는 CPublic_Function 클래스에 존재                 
//	// ==============================================================================
//	st_handler.mn_home_state[m_n_Stacker1] = CTL_READY;	// 모터 HOMING 완료 플래그
//	st_handler.mn_home_state[m_n_Stacker2] = CTL_READY;	// 모터 HOMING 완료 플래그
//	st_handler.mn_home_state[m_n_Stacker_Move] = CTL_READY;	// 모터 HOMING 완료 플래그
//	// ==============================================================================
//	//Func.On_LogFile_Add(99, strTmp);	
//	SetTimer(TMR_MODULEMENEAL_CHECK, 100, NULL);			// 모터 동작 제어 타이머

}

void CDialog_Part_Manual::EnableButton(int iEnable)
{
// 	m_btn_motor_move_1.EnableWindow(iEnable);
// 	m_btn_motor_move_2.EnableWindow(iEnable);
	m_btn_motor_move_3.EnableWindow(iEnable);
	m_btn_motor_move_12.EnableWindow(iEnable);
}

void CDialog_Part_Manual::OnBtnMotorMove12() 
{
	CDialog_Message msg_dlg;
	int mn_response;
	if(m_b_run_req == YES)
	{
		st_msg.str_fallacy_msg = _T("비젼이 테스트중입니다. 다시 시작할까요?");
		mn_response = msg_dlg.DoModal();
		if (mn_response != IDOK) return;
	}
	else
	{
		st_msg.str_fallacy_msg = _T("비젼을 테스트 하시겠습니까?");
		mn_response = msg_dlg.DoModal();
		if (mn_response != IDOK) return;
	}

	EnableButton(FALSE);
	MoveStepClear();
	
	m_b_run_req = YES;
	m_btn_vision_run.EnableWindow(FALSE);
	SetTimer(TMR_VISION_CHECK, 100, NULL);			// 		
	
}

void CDialog_Part_Manual::OnBtnIoMove1() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove2() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove3() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove4() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove5() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove6() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove7() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove8() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove9() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove10() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove11() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Part_Manual::OnBtnIoMove12() 
{
}


void CDialog_Part_Manual::OnBtnStop() 
{	
	m_b_stop_req = TRUE;
	m_btn_vision_run.EnableWindow(TRUE);	
}


int CDialog_Part_Manual::OnModuleStackerReadyPos(int n_StackerPos, int nMode)
{
	int nRet_1 = VARIABLE_INIT;
	int nRet_2 = VARIABLE_INIT;
	int nFuncRet = CTLBD_RET_PROCEED;
	double dNowPos;
	int nMotorNum, nMotorNum1, nMotorNum2;


	if(n_StackerPos == M_STACKER_1)
	{
		nMotorNum = M_LDM_STACKER_1;
	}
//////////////////////////////////////////////////////// 20130213

	else if(n_StackerPos == M_STACKER_2) 
	{
		nMotorNum = M_LDM_STACKER_2;
	}
	
////////////////////////////////////////////////////////

	else
	{
		nMotorNum = M_LDM_STACKER_MOVE;
	}
	
	if(m_n_motor_no == M_LDM_STACKER_1) 
		nMotorNum = M_LDM_STACKER_1;
	else 
		nMotorNum = M_ULDM_STACKER_1;

//////////////////////////////////////////////////////// 20130213

	if(m_n_motor_no11 == M_LDM_STACKER_2) 
		nMotorNum1 = M_LDM_STACKER_2;
	else 
		nMotorNum1 = M_ULDM_STACKER_2;

	if(m_n_motor_no12 == M_LDM_STACKER_MOVE) 
		nMotorNum2 = M_LDM_STACKER_MOVE;
	else 
		nMotorNum2 = M_ULDM_STACKER_MOVE;

////////////////////////////////////////////////////////
	if(FAS_IO.get_in_bit(st_io.i_stacker_light_curtain_chk, IO_ON) == IO_ON )
	{
		COMI.Set_MotStop(0, nMotorNum);
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Error] Area sensor is detected. Motor stop. ReWork Please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		CTL_Lib.mn_single_motmove_step[nMotorNum] = 0;
		return nFuncRet;
	}
//	if(alarm.n_area_ready_alarm[0] == TRUE)
//	{
//		COMI.Set_MotStop(0, nMotorNum);
//		CTL_Lib.mn_single_motmove_step[nMotorNum] = 0;
//		return nFuncRet;
//	}

	switch(ReadyPosStep)
	{
	case 0:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
		if(nRet_1 != CTLBD_RET_GOOD)
		{
			ReadyPosStep = 1000;
		}
		else
		{
			ReadyPosStep = 100;
		}
		break;

	case 100:
		COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmTRUE);
		nRet_1 =  COMI.Start_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep = 200;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		else if(nRet_1 == CTLBD_RET_SAFETY)	
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		break;

	case 200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
		if(nRet_1 != CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, nMotorNum);
			ReadyPosStep = 300;
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_UP]);
			if(nRet_1 == CTLBD_RET_GOOD)// 정상 동작을 했다면...
			{
				if (nMode == 0)
				{
					nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
					nRet_2 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk + (n_StackerPos * 3), IO_OFF);
					if(nRet_1 != CTLBD_RET_GOOD || nRet_2 == IO_ON)
					{
						if(nRet_1 == CTLBD_RET_GOOD)
						{
							ReadyPosStep = 2000;//1000; //20120719
						}
						else
						{
							ReadyPosStep = 5000;
						}					
					}
					else
					{
						ReadyPosStep = 2000;//1000; //20120719
					}
				}
				else if (nMode == 2)// SD 감지후에 DOWN하는 모드
				{
					ReadyPosStep = 300;
				}
				else//SD 감지거나, UP_LIMIT까지만 동작하는 모드이다.
				{
					ReadyPosStep = 5000;
				}
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
			{
				ReadyPosStep = 100;
			}
		}
		break;

	case 300:
		if (nMode == 0)//한장 올린다
		{
			dNowPos = COMI.Get_MotCurrentPos(nMotorNum);
			if(nMotorNum == M_LDM_STACKER_2)
			{
				m_dTargetPos = dNowPos + st_motor[nMotorNum].md_pos[P_STOCKER_UP_OFFSET] - (st_motor[nMotorNum].mn_allow * 30);
			}
			else
			{
				m_dTargetPos = dNowPos + st_motor[nMotorNum].md_pos[P_STOCKER_UP]; // 20130213 위치이동거리
			}
			if(m_dTargetPos > st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT])
			{
				m_dTargetPos = st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT];
				m_b_p_limit_check = TRUE;//20120719
			}
			ReadyPosStep = 400;
		}
		else if (nMode == 2)//한장만큼 내린다.
		{
			dNowPos = COMI.Get_MotCurrentPos(nMotorNum);			
			m_dTargetPos = dNowPos - st_motor[nMotorNum].md_pos[P_STOCKER_SAFETY];
			
			if (m_dTargetPos < st_motor[nMotorNum].md_pos[P_STOCKER_DOWN])
				m_dTargetPos = st_motor[nMotorNum].md_pos[P_STOCKER_DOWN];
			
			ReadyPosStep = 400;
		}
		else//SD감지 대기한다
		{
			ReadyPosStep = 5000;
			ReadyPosStep = 1000;
		}
		break;

	case 400:
		nRet_2 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
		COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum, m_dTargetPos, st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			if(nMode == 0)//UP
			{
			//////////////////////////////////// 20120719
				if (m_b_p_limit_check == TRUE)
				{
					m_b_p_limit_check = FALSE;
					ReadyPosStep = 2000;
				}
			////////////////////////////////////

				else if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON) == IO_ON)
				{
					ReadyPosStep = 5000;
				}
			//////////////////////////////////// 20120719
				else if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF) == IO_OFF && nRet_2 == CTLBD_RET_GOOD)
				{
					ReadyPosStep = 5000;
				}
			////////////////////////////////////
				else
				{
					ReadyPosStep = 2000;
				}
			}
			else
			{
				ReadyPosStep = 5000;
			}

		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep = 400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			ReadyPosStep = 400;
		}
		break;

	case 1000:
		COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
		nRet_1 = COMI.Start_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ReadyPosStep = 1100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		break;

	case 1100:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
		if(nRet_1 != BD_GOOD)
		{
			COMI.Set_MotStop(0, nMotorNum);
			
			if (nMode == 1)//대기
			{
				ReadyPosStep = 5000;
			}
			else
			{
				ReadyPosStep = 100;											// SD센서에 감지가 되지 않으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
			}
			
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_DOWN]);
			if (nRet_1 == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
			{
				ReadyPosStep = 1200;
//				ReadyPosStep = 2000;
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
//				ReadyPosStep = 1000;
			}
			else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
			{
				ReadyPosStep = 5000;
			}
		}
		break;

	case 1200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD); 
		if(nRet_1 != CTLBD_RET_GOOD) //이미 감지하고 있으면 트레이가 FULL까지 있다는 이야기임 
		{
			if(nMotorNum == M_LDM_STACKER_2)
			{
				if(FAS_IO.get_in_bit(st_io.i_m_stacker2_up_chk + (n_StackerPos), IO_OFF) == IO_ON ||
					FAS_IO.get_in_bit(st_io.i_m_stacker2_dn_chk + (n_StackerPos), IO_ON) == IO_OFF)
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_m_stacker1_up_onoff + (n_StackerPos), IO_ON);
					FAS_IO.OnCylinderAction(st_io.o_m_stacker1_dn_onoff + (n_StackerPos), IO_OFF);
					ReadyPosStep = 1300;
				}
				else
				{
					nFuncRet = CTL_FULL;
					ReadyPosStep = 0;
				}
			}
			else
			{
				nFuncRet = CTL_FULL;
				ReadyPosStep = 0;		
			}
		}
		else
		{//Full sensor는 감지되지 않았으니 다시확인한다 
			ReadyPosStep = 1210;		
		}
		break;

	case 1210:
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_up_onoff + (n_StackerPos), IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_dn_onoff + (n_StackerPos), IO_OFF);
		ReadyPosStep = 1300;
		break;

	case 1300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker2_up_onoff + (n_StackerPos), st_io.i_m_stacker1_up_chk + (n_StackerPos),
			                                  st_io.i_m_stacker2_dn_chk + (n_StackerPos), IO_ON, st_wait.i_stacker_updn_w[0], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD)
		{
			ReadyPosStep = 100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)				
			{
				mn_retry = 0;
				//010206 0 01 "Stacker2 레일 Down 센서 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "stacker2 레일 Down 센서 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Down stacker2 rail sensor error");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				ReadyPosStep = 1210;
			}
			else
			{
				ReadyPosStep = 1210;
			}
		}
		break;		


	case 2000://tray는 있으나, UP Pos까지 갔는데 SD가 감지 않된다, stacker를 Up한 후 SD센서를 본다
		if(FAS_IO.get_in_bit(st_io.i_m_stacker_1_updn_pos_chk + (n_StackerPos), IO_OFF) == IO_OFF)
		{//실린더 up한다	
			 if ((FAS_IO.get_in_bit(st_io.i_m_stacker1_up_chk + (n_StackerPos), IO_ON) == IO_ON) && 
				(FAS_IO.get_in_bit(st_io.i_m_stacker1_dn_chk + (n_StackerPos), IO_OFF) == IO_OFF))
			 {
				ReadyPosStep = 2300;
			 }
			 else
			 {
				ReadyPosStep = 2100;
			 }			
		}
		else
		{
//			if ((FAS_IO.get_in_bit(st_io.i_m_stacker_1_up_chk + (n_StackerPos * 4), IO_ON) == IO_ON) && 
//			 (FAS_IO.get_in_bit(st_io.i_m_stacker1_dn_chk + (n_StackerPos * 4), IO_OFF) == IO_OFF))
//			{
//				if(nMode == 0)
//				{
//					ReadyPosStep = 5000;
//				}
//				else
//				{
//					ReadyPosStep = 2010;
//				}
//			}
//			else
//			{
				ReadyPosStep = 2010;
//			}	

		}
		break;

	case 2010:
		COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed); //20130213 StackerDown 위치
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep = 2020;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			ReadyPosStep = 2000;
		}		
		break;

	case 2020:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker_1_updn_pos_chk + (n_StackerPos), IO_OFF) == IO_OFF)
		{//실린더 up한다	
			 if ((FAS_IO.get_in_bit(st_io.i_m_stacker1_up_chk + (n_StackerPos), IO_ON) == IO_ON) && 
				(FAS_IO.get_in_bit(st_io.i_m_stacker1_dn_chk + (n_StackerPos), IO_OFF) == IO_OFF))
			 {
				ReadyPosStep = 2300;
			 }
			 else
			 {
				ReadyPosStep = 2100;
			 }			
		}
		else
		{
			ReadyPosStep = 2300;
		}
		break;

	case 2100:
		nRet_1 = Set_StackerUpDnClinder(n_StackerPos, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep = 2200;
		}
		break;

	case 2200:
		nRet_1 = Get_StackerUpDnClinder(n_StackerPos, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			ReadyPosStep = 100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000205 0 00 "Stacker1 레일 Up 센서 에러."
			if(n_StackerPos == 0)			sprintf(st_msg.c_abnormal_msg, "Stacker1 레일 Up 센서 에러.");
			else                            sprintf(st_msg.c_abnormal_msg, "Stacker2 레일 Up 센서 에러.");
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{				
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
			ReadyPosStep = 2100;
		}
		break;

	case 2300:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_tray_chk + (n_StackerPos), IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			ReadyPosStep = 2400;
		}
		else// if(nRet_1 == IO_OFF)
		{
			ReadyPosStep = 100;
		}
		break;

	case 2400:
		nRet_1 = Set_StackerUpDnClinder(n_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep = 2500;
		}
		break;

	case 2500:
		nRet_1 = Get_StackerUpDnClinder(n_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			ReadyPosStep = 2600;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000206 0 00 "Stacker1 레일 애주 센서 에러."
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "Stacker1 레일 센서 에러.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "Down stacker1 rail sensor error");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
			ReadyPosStep = 2400;
		}
		break;

	case 2600:
		nFuncRet = CTLBD_RET_SKIP;
		ReadyPosStep = 0;
		break;

	case 5000:
		nFuncRet = CTLBD_RET_GOOD;
		ReadyPosStep = 0;
		break;	
	}

	return nFuncRet;


}

int CDialog_Part_Manual::Set_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff == CYLINDER_OFF)
	{
	}

	FAS_IO.set_out_bit(st_io.o_m_stacker1_rail_fwd_onoff + (nStacker*2), n_onoff);
	FAS_IO.set_out_bit(st_io.o_m_stacker1_rail_bwd_onoff + (nStacker*2), !n_onoff);
	mb_pushercylinder_flag[nStacker] = FALSE;
	ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
	return nFuncRet;
}

int CDialog_Part_Manual::Get_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff)
{
	int nFuncRet = -1;
	
	ml_pushercylinder_Time[nStacker][1] = GetCurrentTime();
	
	if (mb_pushercylinder_flag[nStacker] != TRUE)
	{
		ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
		mb_pushercylinder_flag[nStacker] = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_fwd_chk + (nStacker*3), IO_ON) == IO_ON) && 
		(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_bwd_chk + (nStacker*3), IO_OFF) == IO_OFF))	// On 상태.
	{
		ml_pushercylinder_Time[nStacker][2] = ml_pushercylinder_Time[nStacker][1] - ml_pushercylinder_Time[nStacker][0];
		
		if (ml_pushercylinder_Time[nStacker][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_pushercylinder_Time[nStacker][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker] = FALSE;
			}
		}
		else if (ml_pushercylinder_Time[nStacker][2] > st_wait.i_stacker_f_b_w[1])
		{
			nFuncRet = CYLINDER_ON;
			mb_pushercylinder_flag[nStacker] = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_fwd_chk + (nStacker*3), IO_OFF) == IO_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_bwd_chk + (nStacker*3), IO_ON) == IO_ON))	// Off 상태.
	{
		ml_pushercylinder_Time[nStacker][2] = ml_pushercylinder_Time[nStacker][1] - ml_pushercylinder_Time[nStacker][0];
		
		if (ml_pushercylinder_Time[nStacker][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_pushercylinder_Time[nStacker][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker] = FALSE;
			}
		}
		else if (ml_pushercylinder_Time[nStacker][2] > st_wait.i_stacker_f_b_w[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_pushercylinder_flag[nStacker] = FALSE;
		}
	}
	else 
	{
		ml_pushercylinder_Time[nStacker][2] = ml_pushercylinder_Time[nStacker][1] - ml_pushercylinder_Time[nStacker][0];
		
		if (ml_pushercylinder_Time[nStacker][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_pushercylinder_Time[nStacker][2] > st_wait.i_stacker_f_b_l)
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_pushercylinder_flag[nStacker] = FALSE;
		}
	}
	
	return nFuncRet;
}

int CDialog_Part_Manual::InitVision()
{
	int nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	int nRet_3 = RET_PROCEED, nRet_4 = RET_PROCEED;

	int mn_Init_flag = RET_PROCEED;
	
	if(mn_Visinitial != FALSE) return mn_Init_flag;
	if(m_b_stop_req == YES) return mn_Init_flag;


	switch(mn_InitStep)
	{
	case 0:
		mn_InitStep = 1000;
		break;

	case 1000:
		nRet_1 = COMI.HomeCheck_Mot(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_InitStep = 2000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			mn_Init_flag = RET_ERROR;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8003, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2000:		
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_unit_fwd_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_label_attach_unit_bwd_chk, IO_ON);
		if(nRet_1 == IO_OFF || nRet_2 == IO_ON)
		{	//340101 0 34 "Label unit backward 체크 에러"
			mn_InitStep = 2200;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
			mn_InitStep = 2100;
		}
		break;

	case 2010:
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
		mn_InitStep = 2100;
		break;

	case 2200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.i_label_attach_unit_bwd_chk, st_io.i_label_attach_unit_fwd_chk,
			IO_ON, st_wait.i_label_onoff_w[1], st_wait.i_label_l);
		
		if(nRet_1 == RET_GOOD)
		{
			mn_vision1_init = NO;
			mn_vision2_init = NO;
			mn_InitStep = 3000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//340101 0 34 "Label unit backward 체크 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"Label unit backward 체크 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Check Label unit backward error");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(cAlarmCose,"340101");
				CTL_Lib.Alarm_Error_Occurrence(8004, CTL_dWARNING, cAlarmCose);
				mn_Init_flag = RET_ERROR;
				mn_InitStep = 2010;
			}
			else
			{
				mn_InitStep = 2010;
			}
		}
		break;

	case 3000:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_vision1_safety_up_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_vision1_safety_dn_chk, IO_ON);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_vision2_safety_up_chk, IO_ON);
		nRet_4 = FAS_IO.get_in_bit(st_io.i_vision2_safety_dn_chk, IO_ON);

		if(mn_vision2_init == YES)
		{
			mn_InitStep = 3010;
			break;
		}
		else if(mn_vision1_init == YES)
		{
			mn_InitStep = 3500;
			break;
		}
		if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION1_Z, cmSMODE_KEEP, st_motor[M_VISION1_Z].md_spd_jog, st_motor[M_VISION1_Z].md_spd_acc, st_motor[M_VISION1_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION1_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
					mn_InitStep = 3700;
					break;
				}
				if(cmmSxVMoveStart(M_VISION1_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
				}
			}
		}
		else if(nRet_3 == IO_ON && nRet_4 == IO_OFF)
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION2_Z, cmSMODE_KEEP, st_motor[M_VISION2_Z].md_spd_jog, st_motor[M_VISION2_Z].md_spd_acc, st_motor[M_VISION2_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION2_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION2_Z, FALSE, TRUE);
					mn_InitStep = 3200;
					break;
				}
				if(cmmSxVMoveStart(M_VISION2_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION2_Z, FALSE, TRUE);
				}
			}
		}
		else if ((nRet_1 == IO_ON && nRet_2 == IO_ON) && (nRet_3 == IO_OFF && nRet_4 == IO_OFF))
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION1_Z, cmSMODE_KEEP, st_motor[M_VISION1_Z].md_spd_jog, st_motor[M_VISION1_Z].md_spd_acc, st_motor[M_VISION1_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION1_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
					mn_InitStep = 3500;
					break;
				}
				if(cmmSxVMoveStart(M_VISION1_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
					mn_Init_flag = RET_ERROR;
				}
			}
		}
		else if ((nRet_3 == IO_ON && nRet_3 == IO_ON) && (nRet_1 == IO_OFF && nRet_2 == IO_OFF))
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION2_Z, cmSMODE_KEEP, st_motor[M_VISION2_Z].md_spd_jog, st_motor[M_VISION2_Z].md_spd_acc, st_motor[M_VISION2_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION2_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION2_Z, FALSE, TRUE);
					mn_InitStep = 3010;
					break;
				}
				if(cmmSxVMoveStart(M_VISION2_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION2_Z, FALSE, TRUE);
				}
			}
		}		
		break;

	case 3010:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION1_Y, st_motor[M_VISION1_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_InitStep = 3100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 3010;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8005, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3100:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION1_Z, st_motor[M_VISION1_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_InitStep = 4100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 3100;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8006, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3200:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION1_Z, st_motor[M_VISION1_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_InitStep = 3300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 3200;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8007, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3300:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION1_Y, st_motor[M_VISION1_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_InitStep = 4100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 3300;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8008, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3500:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION2_Y, st_motor[M_VISION2_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_InitStep = 3600;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 3500;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8009, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3600:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION2_Z, st_motor[M_VISION2_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_InitStep = 4000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 3600;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8010, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3700:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION2_Z, st_motor[M_VISION2_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_InitStep = 3800;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 3700;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8011, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3800:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION2_Y, st_motor[M_VISION2_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_InitStep = 4000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 3800;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8012, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 4000:
		mn_vision2_init = YES;
		mn_InitStep = 4500;
		break;

	case 4100:
		mn_vision1_init = YES;
		mn_InitStep = 4500;
		break;

	case 4500:
		if(mn_vision1_init == YES && mn_vision2_init == YES)
		{
			mn_InitStep = 5000;
		}
		else
		{
			if(mn_vision1_init == YES) mn_InitStep = 3500;
			else					   mn_InitStep = 3000;
		}
		break;

	case 5000:
	///////////////////////////////////////////////////////////////////////////////////
	// SITE_V1를 SITE_LDRBT_POS까지 이동하여 로딩 로봇이 자재를 놓을 수 있도록 한다 
	///////////////////////////////////////////////////////////////////////////////////  
		nRet_1 = CTL_Lib.Single_Move(M_VISION1_Z, st_motor[M_VISION1_Z].md_pos[VISION_Z_DOWN_READY], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_InitStep = 5100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 5000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8013, CTL_dWARNING, COMI.mc_alarmcode);
			mn_InitStep = 5100;
		} 
		break;

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(M_VISION1_Y, st_motor[M_VISION1_Y].md_pos[VISION_Y_READY_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_InitStep = 5200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 5000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8014, CTL_dWARNING, COMI.mc_alarmcode);
			mn_InitStep = 5100;
		} 
		break;

	case 5200:
		 n_visionrbt_working_site[VISION_1_SITE] = SITE_READY_POS;
		 mn_InitStep = 6000;
		 break;

	case 6000:
	///////////////////////////////////////////////////////////////////////////////////
	// SITE_V2를 SITE_ULDRBT_POS까지  이동하여 로딩 로봇이 자재를 놓을 수 있도록 한다 
	/////////////////////////////////////////////////////////////////////////////////// 
		nRet_1 = CTL_Lib.Single_Move(M_VISION2_Y, st_motor[M_VISION2_Y].md_pos[SITE_ULDRBT_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_InitStep = 6100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 6000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8015, CTL_dWARNING, COMI.mc_alarmcode);
			mn_InitStep = 6100;
		} 
		break;

	case 6100:
		nRet_1 = CTL_Lib.Single_Move(M_VISION2_Z, st_motor[M_VISION2_Z].md_pos[VISION_Z_ULDMODULE], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_InitStep = 6200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_InitStep = 6000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Init_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8016, CTL_dWARNING, COMI.mc_alarmcode);
			mn_InitStep = 6100;
		} 
		break;

	case 6200:
		 n_visionrbt_working_site[VISION_2_SITE] = SITE_ULDRBT_POS;
		 mn_InitStep = 7000;
		 break;

	case 7000:
		mn_Visinitial = CTL_YES;
		mn_InitStep = 0; 
		break;

	}
	return mn_Init_flag;
}

int CDialog_Part_Manual::Label_Test()
{
	int nRet_1;
	int mn_lavel_flag = RET_PROCEED;
	double dCurr;

	if(mn_Visinitial != CTL_YES)  return mn_lavel_flag;
	if(m_b_stop_req == YES) return mn_lavel_flag;

	switch(mn_labelStep)
	{
	case 0:
		mn_labelStep = 10;
		break;

	case 10:
		dCurr = COMI.Get_MotCurrentPos(M_LABEL_ATTACH_Z);
		if(dCurr > st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_SAFETY] + st_motor[M_LABEL_ATTACH_Z].mn_allow)
		{
			mn_labelStep = 20;
		}
		else
		{
			mn_labelStep = 80;
		}
		break;

	case 20:
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_labelStep = 10;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			mn_labelStep = 20;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_lavel_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8020, CTL_dWARNING, COMI.mc_alarmcode);
			mn_labelStep = 20;
		}
		break;

	case 80:
		if(FAS_IO.get_in_bit(st_io.i_label_attach_unit_fwd_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_label_attach_unit_bwd_onoff, IO_ON) == IO_ON)
		{
			mn_labelStep = 100;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.i_label_attach_unit_fwd_chk, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
			mn_labelStep = 90;
		}
		break;

	case 85:
		FAS_IO.OnCylinderAction(st_io.i_label_attach_unit_fwd_chk, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
		mn_labelStep = 90;
		break;

	case 90:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.o_label_attach_unit_bwd_onoff, st_io.i_label_attach_unit_fwd_chk,
					IO_ON, st_wait.i_label_onoff_w[0], st_wait.i_label_l);
		if(nRet_1 == RET_GOOD)
		{
			mn_labelStep = 100;//80;	//20120707
		}
		else if(nRet_1 != RET_PROCEED)
		{//550702 1 55 "Label Attach Forward 에러."
		 //550703 1 55 "Label Attach Backward 에러."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_lavel_flag = RET_ERROR;
				sprintf(cAlarmCose,"550703");
				CTL_Lib.Alarm_Error_Occurrence(8021, CTL_dWARNING, cAlarmCose);
			}
			else
			{
				mn_labelStep = 85;
			}
		}
		break;		
		
//////////////////////////////////////20120707		
	case 100:
		//일단 한장만 출력한다
			m_dTargetLabel = 0;
			nRet_1 = Func.Barcode_Create(0, 0);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				mn_labelStep = 101;
			}
			else
			{
				mn_labelStep = 101;
			}
			//Func.Barcode_Create();	
			
		break;

	case 101:
		//한장 출력 완료 확인 
		if (st_barcode.b_print_complete == TRUE)
		{
			st_barcode.b_print_complete = FALSE;
			mn_labelStep = 102;
		}
		else
		{
			mn_labelStep = 102;
		}
		break;

	case 102:
		nRet_1 = Func.BCR_Trigger();
		if(nRet_1 == RET_GOOD || nRet_1 == RET_DOOROPEN)
		{
			//비전 명령 
			mn_labelStep = 2000;
		}
		break;
//////////////////////////////////////
	case 2000:
		m_dTargetLabel = st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_PICK_POS] + md_offset;
		mn_labelStep = 2100;
		break;

	case 2100:
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, m_dTargetLabel, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_labelStep = 2200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_labelStep= 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			mn_lavel_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8022, CTL_dWARNING, COMI.mc_alarmcode);
			mn_labelStep = 2100;
		}
		break;

	case 2200:
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_ON);
		l_PickerWaitTime[0] = GetCurrentTime();
		mn_labelStep = 2210;
		break;

	case 2210:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] > 1000)
		{
			mn_labelStep = 2220; 
		}
		break;

	case 2220:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
		if(nRet_1 == IO_ON || st_basic.n_mode_device != 1)
		{
			mn_labelStep = 2300;
		}
		else
		{
			dCurr = COMI.Get_MotCurrentPos(M_LABEL_ATTACH_Z);
			if(dCurr > st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_PICK_POS] + 2)
			{
				mn_attach_err = YES;
				mn_labelStep = 2300;
			}
			else
			{
				md_offset += 0.2;
				mn_labelStep = 2100;
			}
		}
		break;

	case 2300:
		md_offset = 0.0;
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			if(mn_attach_err == YES)//error
			{//550700 1 55 "Label Attach Pick 에러."
				mn_attach_err = NO;
				sprintf(cAlarmCose, "550700");
				CTL_Lib.Alarm_Error_Occurrence(8023, CTL_dWARNING, cAlarmCose);
				mn_labelStep = 2010;
			}
			else
			{
				mn_labelStep = 2310;
			}
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_labelStep= 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			mn_lavel_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8024, CTL_dWARNING, COMI.mc_alarmcode);
			mn_labelStep = 2100;
		}
		break;

	case 2310:
		if(n_visionrbt_label_req[0] == CTL_REQ)
		{
			//n_visionrbt_label_req[0] = CTL_READY;
			mn_labelStep = 2400;
		}
		break;

	case 2400:
		md_offset = 0.0;
		mn_retry = 0;
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_OFF);
		mn_labelStep = 2600;
		break;

	case 2500:
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_OFF);
		mn_labelStep = 2600;
		break;

	case 2600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.i_label_attach_unit_fwd_chk, st_io.i_label_attach_unit_bwd_chk,
			IO_ON, st_wait.i_label_onoff_w[1], st_wait.i_label_l);
		if(nRet_1 == RET_GOOD)
		{
			mn_labelStep = 3000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//340101 0 34 "Label unit backward 체크 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"Label unit backward 체크 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Check Label unit backward error");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(cAlarmCose,"340101");
				CTL_Lib.Alarm_Error_Occurrence(8025, CTL_dWARNING, cAlarmCose);
				mn_labelStep = 2500;
			}
			else
			{
				mn_labelStep = 2500;
			}
		}
		break;

	case 3000:
		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 0)
		{
			m_dTargetLabel = st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_POS1] + md_offset;
		}
		else
		{
			m_dTargetLabel = st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_POS2] + md_offset;			
		}
		mn_labelStep = 3100;
		break;

	case 3100:
		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 0)
		{
			nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_POS1], st_basic.nRunSpeed);
		}
		else
		{
			nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_POS2], st_basic.nRunSpeed);
		}

		if(nRet_1 == BD_GOOD)
		{
			mn_labelStep = 3200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_labelStep= 3100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			mn_lavel_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8026, CTL_dWARNING, COMI.mc_alarmcode);
			mn_labelStep = 3100;
		}
		break;

	case 3200:
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_ON);
		l_PickerWaitTime[0] = GetCurrentTime();
		mn_labelStep = 3300;
		break;

	case 3300:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] > 500)
		{
			mn_labelStep = 3400;
		}
		break;
		
	case 3400:
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
			mn_labelStep = 3500;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_labelStep= 3400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			mn_lavel_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8027, CTL_dWARNING, COMI.mc_alarmcode);
			mn_labelStep = 3400;
		}
		break;

	case 3500:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			mn_retry = 0;
			mn_labelStep = 3600;
		}
		else
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//550701 1 55 "Label Attach Place 에러."
				sprintf(cAlarmCose, "550701");
				alarm.mn_alarm_assign_section = 3506;
				CTL_Lib.Alarm_Error_Occurrence(8028, CTL_dWARNING, cAlarmCose);
				mn_labelStep = 3010;
			}
			else
			{
				md_offset += 0.5;
				mn_labelStep = 3010;
			}
		}
		break;

	case 3600:
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
		mn_labelStep = 3700;
		break;

	case 3700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.i_label_attach_unit_bwd_chk, st_io.i_label_attach_unit_fwd_chk,
			IO_ON, st_wait.i_label_onoff_w[1], st_wait.i_label_l);
		if(nRet_1 == RET_GOOD)
		{
			mn_labelStep = 4000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//340100 0 34 "Label unit forward 체크 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"Label unit forward 체크 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Check Label unit forward error");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(cAlarmCose,"340100");
				mn_lavel_flag = RET_ERROR;
				CTL_Lib.Alarm_Error_Occurrence(8029, CTL_dWARNING, cAlarmCose);
				mn_labelStep = 3600;
			}
			else
			{
				mn_labelStep = 3600;
			}
		}
		break;

	case 4000:
		n_visionrbt_label_req[0] = CTL_READY;
		mn_labelStep = 100;
		break;
	
	}

	return mn_lavel_flag;
}

int CDialog_Part_Manual::Module_Vision_Top() //VISION 1 -> Rear Robot 으로 이것부터 진행한다 
{
	int nMotor_Y = M_VISION1_Y, nMotor_Z = M_VISION1_Z, nSite = VISION_1_SITE;
	int nRet_1=0, nRet_2=0, nFlag=0;
	int mn_Vision1_flag = RET_PROCEED;

	if(mn_Visinitial != YES) return mn_Vision1_flag;
	if(m_b_stop_req == YES) return mn_Vision1_flag;

	switch(mn_VisionStep[nSite])
	{
	case 0:
		mn_VisionStep[nSite] = 100;
		break;

	case 100: //데이터를 클리어 한다 
		n_visionrbt_workend_flag[nSite] = CTL_NO;	 //vision 1 작업 정보 클리어  
		mn_VisionStep[nSite] = 1000;
		break;

	case 1000:
		if(1 || n_visionrbt_workend_flag[VISION_2_SITE] == CTL_YES) //vision 2 사이트 작업시작 플레그 
		{//vision2의 해당 위치의 작업이 모두 완료 되었으니 다음 작업을 진행하자  

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//현재 작업중인 위치를 다음으로 이동하기 위해 백업한다, 백업한 위치정보는 안전 및 다음 위치 이동에 활용한다 	
			//st_sync.n_visionrbt_past_working_site[nSite]는 바로전 작업위치 이다 
			//st_sync.n_visionrbt_working_site[nSite]는 현재 위치이다 
			//st_sync.n_visionrbt_next_working_site[nSite]는 다음에 작업할 위치정보 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite]; //바로전에 작업한 위치 정보를 백업/저장 한다 
			n_visionrbt_next_working_site[nSite] = n_visionrbt_working_site[nSite]; //다음 작업할 위치 정보를 확인하기 위해 임시로 위치값을 저장한다 

			n_visionrbt_workend_flag[VISION_2_SITE] = CTL_NO;

			mn_VisionStep[nSite] = 1100;
		}	 
		break;

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 다음에 작업할 위치를 셋팅하여 작업을 시작하다 
	//////////////////////////////////////////////////////////////////////////////////////////////
	case 1100:
		if(n_visionrbt_working_site[nSite] >= SITE_MAX_POS - 1 || n_visionrbt_working_site[nSite] < 0) //0,1,2,3,4 까지의 위치 
		{
			n_visionrbt_next_working_site[nSite] = 0; //다음에 작업하 위치정보 
		}
		else
		{
			n_visionrbt_next_working_site[nSite]++; 
		}

		
		mn_VisionStep[nSite] = 1200; 
		break;

	case 1200: //실제 이동할 위치 정보가 임시로 셋팅된 상태이다 , 이때 상태편(다른 비젼) 사이트와 위치를 비교하여 안전 조건을 확인한다 
		nFlag=0; 
		if(n_visionrbt_next_working_site[nSite] == n_visionrbt_working_site[VISION_2_SITE])
		{//앞으로 작업할 위치가 같은 위치이니 다 끝나고 이동하기를 기다려야 한다 
			nFlag++;
		}
		else if(n_visionrbt_next_working_site[nSite] == SITE_LDRBT_POS || n_visionrbt_next_working_site[nSite] == SITE_LABEL_POS)
		{
			if(n_visionrbt_working_site[VISION_2_SITE] == SITE_LDRBT_POS) //SITE_LDRBT_POS, SITE_LABEL_POS는 거의 같은 위치로 이 위치로 이동할려면 상태편은 이 위치에 없어야 한다 
			{
				nFlag++;
			}
			else if(n_visionrbt_working_site[VISION_2_SITE] == SITE_LABEL_POS) //SITE_LDRBT_POS, SITE_LABEL_POS는 거의 같은 위치로 이 위치로 이동할려면 상태편은 이 위치에 없어야 한다 
			{
				nFlag++;
			}
		}

		if(nFlag > 0) //앞으로 작업할 위치가 상대편 로봇이 작업중인 위치와 같으면 충돌 위치에 있으니 기다린다 
		{			
			break;
		}	
		else
		{
			mn_VisionStep[nSite] = 1300; //모든 안전 조건 확인 완료 
		}
		break;

	case 1300:
		if(n_visionrbt_next_working_site[nSite] > SITE_MAX_POS - 1)
		{
			mn_Vision1_flag = RET_ERROR;
			sprintf(cAlarmCose,"560000");
			CTL_Lib.Alarm_Error_Occurrence(8030, CTL_dWARNING, cAlarmCose);			
			break;
		}
		else
		{
			if(n_visionrbt_next_working_site[nSite] == SITE_READY_POS) //0 //대기 위치로 이동 //SITE_READY_POS
			{
				mn_VisionStep[nSite] = 2000; //0 대기 위치로 이동 //SITE_READY_POS
			}
			else if(n_visionrbt_next_working_site[nSite] == SITE_LDRBT_POS)//1) //SITE_LDRBT_POS
			{
				mn_VisionStep[nSite] = 3000; //1
			}
			else if(n_visionrbt_next_working_site[nSite] == SITE_LABEL_POS) //2) //SITE_LABEL_POS
			{
				mn_VisionStep[nSite] = 4000; //2
			}
			else if(n_visionrbt_next_working_site[nSite] == SITE_VISION_POS) //3) //SITE_VISION_POS
			{
				mn_VisionStep[nSite] = 5000; //3
			}
			else if(n_visionrbt_next_working_site[nSite] == SITE_ULDRBT_POS) //4) //SITE_ULDRBT_POS
			{
				mn_VisionStep[nSite] = 6000; //4
			}
			else
			{
				//560001 1 56 "Vision#1의 다음 이동 사이트 에러"
				sprintf(cAlarmCose,"560001");
				CTL_Lib.Alarm_Error_Occurrence(8031, CTL_dWARNING, cAlarmCose);
			}
		}
		break;

	/////////////////////////////////////////////////////////
	// SITE_READY_POS 로 이동한다  
	/////////////////////////////////////////////////////////
	case 2000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560002 1 56 "Vision#1의 READY POS가 아닙니다."
			sprintf(cAlarmCose,"560002");
			CTL_Lib.Alarm_Error_Occurrence(8032, CTL_dWARNING, cAlarmCose);
			mn_Vision1_flag = RET_ERROR;
			break;
		}
		else
		{
			mn_VisionStep[nSite] = 2010;
		}
		break;

	case 2010:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_DOWN_READY], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 2020;//2100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 2010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision1_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8033, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 2010;
		} 
		break;

	case 2020:
		nRet_1 = COMI.Check_SingleMove(M_VISION2_Z, st_motor[M_VISION2_Z].md_pos[VISION_Z_LABEL]);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 2100;
		}
		break;
		

	case 2100:
//		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_READY_POS], st_basic.nRunSpeed);
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_LDRBT_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 2200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 2010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision1_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8034, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 2010;
		}
		break;

	case 2200:
		n_visionrbt_working_site[nSite] = 0; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 

		mn_VisionStep[nSite] = 2300;
		break;

	case 2300:
		mn_VisionStep[nSite] = 1000;
		break;


	/////////////////////////////////////////////////////////
	// Ready POS - > loading robot가 놓는 자재를 받는다  
	/////////////////////////////////////////////////////////
	case 3000:
		mn_VisionStep[nSite] = 3010;
		break;

		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560003 1 56 "Vision#1의 자재 LOADING POS가 아닙니다."
			sprintf(cAlarmCose,"560003");
			CTL_Lib.Alarm_Error_Occurrence(8035, CTL_dWARNING, cAlarmCose);
			mn_Vision1_flag = RET_ERROR;
			break;
		}
		else
		{
			mn_VisionStep[nSite] = 3010;
		}
		break;

	case 3010:
		mn_VisionStep[nSite] = 3100; 
		break;

	case 3100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_LDRBT_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
			n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];
			mn_VisionStep[nSite] = 3200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 3100;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision1_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8036, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 3100;
		}
		break;

	case 3200:	
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_LDMODULE], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 3300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 3200;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(8037, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 3200;
		} 
		break;

	case 3300:
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		mn_VisionStep[nSite] = 4000;
		break;

	///////////////////////////////////////////////////////////////////
	// Label 붙이는 작업을 한다 //SITE_LDRBT_POS - > SITE_LABEL_POS
	///////////////////////////////////////////////////////////////////
	case 4000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560004 1 56 "Vision#1의 LABEL POS가 아닙니다."
			sprintf(cAlarmCose,"560004");
			CTL_Lib.Alarm_Error_Occurrence(8038, CTL_dWARNING, cAlarmCose);
			mn_Vision1_flag = RET_ERROR;
			break;
		}
		else
		{
			set_dvc_rotator(nSite, IO_ON);
			mn_VisionStep[nSite] = 4100;
		}
		break;

	case 4100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_LABEL], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 4200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 4100;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision1_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8039, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 4100;
		} 
		break;

	case 4200:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_LABEL_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 4300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 4200;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision1_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8040, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 4200;
		} 
		break;

	case 4300:
		n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];		
		mn_VisionStep[nSite] = 4400;
		break;


	case 4400: //라벨을 붙을수 있는 상태인지 확인 
		nRet_1 = get_dvc_rotator(nSite, IO_ON, st_wait.i_vision_ratator_w[0], st_wait.i_vision_ratator_l);
		if(nRet_1 == CTL_GOOD)
		{
			mn_VisionStep[nSite] = 4500;
		}
		else if(nRet_1 == CTL_ERROR)
		{//560100 1 56 "Vision#1의 Module Clamp 에러."
			sprintf(cAlarmCose,"560100");
			CTL_Lib.Alarm_Error_Occurrence(8041, CTL_dWARNING, cAlarmCose);
			mn_Vision1_flag = RET_ERROR;
		}
		break;

	case 4500:		
		n_visionrbt_label_req[0] = CTL_REQ; 
		n_visionrbt_label_req[1] = VISION_1_SITE; 
		mn_VisionStep[nSite] = 4600;
		break;

	case 4600:
		if(n_visionrbt_label_req[0] == CTL_READY) //라벨 로봇이 자재를 붙여 놓았다 
		{
			n_visionrbt_label_req[0] = CTL_FREE;
			mn_VisionStep[nSite] = 4700;
		}
		break;

	case 4700:
		set_dvc_rotator(nSite, IO_OFF);
		mn_VisionStep[nSite]  = 4800;
		break;

	case 4800:
		nRet_1 = get_dvc_rotator(nSite, IO_OFF, st_wait.i_vision_ratator_w[1], st_wait.i_vision_ratator_l);
		if(nRet_1 == CTL_GOOD)
		{
			mn_VisionStep[nSite] = 4900;
		}
		else if(nRet_1 == CTL_ERROR)
		{//560100 1 56 "Vision#1의 Module Clamp 에러."
			sprintf(cAlarmCose,"560100");
			CTL_Lib.Alarm_Error_Occurrence(8042, CTL_dWARNING, cAlarmCose);
			mn_Vision1_flag = RET_ERROR;
		}
		break;

	case 4900:
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료
		mn_VisionStep[nSite] = 1000;
		break;

	////////////////////////////////////////////////////////////////
	// VISION TEST을 한다 //SITE_LABEL_POS - > SITE_VISION_POS
	////////////////////////////////////////////////////////////////
	case 5000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560005 1 56 "Vision#1의 VISION Check POS가 아닙니다."
			sprintf(cAlarmCose,"560005");
			CTL_Lib.Alarm_Error_Occurrence(8043, CTL_dWARNING, cAlarmCose);
			mn_Vision1_flag = RET_ERROR;
			break;
		}
		else 
		{
			mn_VisionStep[nSite] = 5010;
		}
		break;

	case 5010:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_VISION], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 5100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 5010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision1_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8044, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 5010;
		} 
		break; 

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_VISION_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 5200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 5000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision1_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8045, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 5000;
		} 
		break; 

	case 5200:
		n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];
		mn_VisionStep[nSite] = 5300;
		break;

	//////////////////////////////////////////////////////////////////
	//vision test를 요청한다 
	//////////////////////////////////////////////////////////////////
	case 5300: //
		n_visionrbt_visiontest_req[nSite] = CTL_REQ;
		mn_VisionStep[nSite] = 5310;
		break;

/////////////////////////////////////// 20120707
	case 5310:	
		if (n_visionrbt_visiontest_req[nSite] == CTL_REQ)
		{
			nRet_1 = Func.HS_Trigger();
			if(nRet_1 == RET_GOOD)
			{
				//비전 명령 
				mn_VisionStep[nSite] = 5400;
			}
		}
		break;

///////////////////////////////////////

	case 5400: //
		n_visionrbt_visiontest_req[nSite] = CTL_READY;

		if(n_visionrbt_visiontest_req[nSite] == CTL_READY)
		{
			////////////////////////////////////////////////////////
			//vision test를 완료한 상태 , 결과를 받으면 된다 
			////////////////////////////////////////////////////////

			mn_VisionStep[nSite] = 5500;
		}
		break;

	case 5500:
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		mn_VisionStep[nSite] = 1000;
		break;

	///////////////////////////////////////////////////////////////////
	// ULD Robot 작업을 한다 //SITE_VISION_POS - > SITE_ULDRBT_POS
	///////////////////////////////////////////////////////////////////
	case 6000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560006 1 56 "Vision#1의 UNLOAD POS가 아닙니다."
			sprintf(cAlarmCose,"560006");
			CTL_Lib.Alarm_Error_Occurrence(8046, CTL_dWARNING, cAlarmCose);
			mn_Vision1_flag = RET_ERROR;
			break;
		}
		else
		{
			mn_VisionStep[nSite] = 6010;
		}
		break;

	case 6010:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_ULDMODULE], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 6100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 6010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision1_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8047, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 6010;
		} 
		break; 

	case 6100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_ULDRBT_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 6200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 6010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision1_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8048, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 6010;
		} 
		break; 

	case 6200:
		n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		mn_VisionStep[nSite] = 1000;
		break;

	case 7000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560002 1 56 "Vision#1의 READY POS가 아닙니다."
			sprintf(cAlarmCose,"560002");
			CTL_Lib.Alarm_Error_Occurrence(8049, CTL_dWARNING, cAlarmCose);
			mn_Vision1_flag = RET_ERROR;
			break;
		}

		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_DOWN_READY], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 7100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 7000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(8050, CTL_dWARNING, COMI.mc_alarmcode);
			mn_Vision1_flag = RET_ERROR;
			mn_VisionStep[nSite] = 7000;
		} 
		break; 

	case 7100:		
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[SITE_READY_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 7200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 7000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(8051, CTL_dWARNING, COMI.mc_alarmcode);
			mn_Vision1_flag = RET_ERROR;
			mn_VisionStep[nSite] = 7000;
		} 
		break; 

	case 7200:
		n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];
		mn_VisionStep[nSite] = 1000;
		break;
	}

	return mn_Vision1_flag;
}


int CDialog_Part_Manual::Module_Vision_Btm() //VISION 2 -> Front robot 
{
	int nMotor_Y = M_VISION2_Y, nMotor_Z = M_VISION2_Z, nSite = VISION_2_SITE;
	int nRet_1=0, nRet_2=0, nFlag=0;
	int mn_Vision2_flag = RET_PROCEED;

	if(mn_Visinitial != YES) return mn_Vision2_flag;
	if(m_b_stop_req == YES) return mn_Vision2_flag;

	switch(mn_VisionStep[nSite])
	{
	case 0:
		mn_VisionStep[nSite] = 100;
		break;

	case 100: //데이터를 클리어 한다 
		n_visionrbt_workend_flag[nSite] = CTL_NO;	 //vision 2 작업 정보 클리어  
		mn_VisionStep[nSite] = 1000;
		break;

	/////////////////////////////////////////////////////////
	case 1000:
		if(1 || n_visionrbt_workend_flag[VISION_1_SITE] == CTL_YES) //vision 2 사이트 작업시작 플레그 
		{//vision2의 해당 위치의 작업이 모두 완료 되었으니 다음 작업을 진행하자  

			n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite]; //바로전에 작업한 위치 정보를 백업/저장 한다 
			n_visionrbt_next_working_site[nSite] = n_visionrbt_working_site[nSite]; //다음 작업할 위치 정보를 확인하기 위해 임시로 위치값을 저장한다 

			n_visionrbt_workend_flag[VISION_1_SITE] = CTL_NO;
			mn_VisionStep[nSite] = 1100;
		}	 
		break;

	case 1100:
		if(n_visionrbt_working_site[nSite] >= SITE_MAX_POS - 1 || n_visionrbt_working_site[nSite] < 0) //0,1,2,3,4 까지의 위치 
		{
			n_visionrbt_next_working_site[nSite] = 0; //다음에 작업하 위치정보 
		}
		else
		{
			//////////////////////////////////////
			//다음에 작업할 위치 정보이다 정보를 알 수 있다 
			//////////////////////////////////////
			n_visionrbt_next_working_site[nSite]++; 
		}

		
		mn_VisionStep[nSite] = 1200; 
		break;

	case 1200: //실제 이동할 위치 정보가 임시로 셋팅된 상태이다 , 이때 상태편(다른 비젼) 사이트와 위치를 비교하여 안전 조건을 확인한다 
		nFlag=0; 
		if(n_visionrbt_next_working_site[nSite] == n_visionrbt_working_site[VISION_1_SITE])
		{//앞으로 작업할 위치가 같은 위치이니 다 끝나고 이동하기를 기다려야 한다 
			nFlag++;
		}
		else if(n_visionrbt_next_working_site[nSite] == SITE_LDRBT_POS || n_visionrbt_next_working_site[nSite] == SITE_LABEL_POS)
		{
			if(n_visionrbt_working_site[VISION_1_SITE] == SITE_LDRBT_POS) //SITE_LDRBT_POS, SITE_LABEL_POS는 거의 같은 위치로 이 위치로 이동할려면 상태편은 이 위치에 없어야 한다 
			{
				nFlag++;
			}
			else if(n_visionrbt_working_site[VISION_1_SITE] == SITE_LABEL_POS) //SITE_LDRBT_POS, SITE_LABEL_POS는 거의 같은 위치로 이 위치로 이동할려면 상태편은 이 위치에 없어야 한다 
			{
				nFlag++;
			}
		}

		if(nFlag > 0) //앞으로 작업할 위치가 상대편 로봇이 작업중인 위치와 같으면 충돌 위치에 있으니 기다린다 
		{			
			break;
		}	
		else
		{
			//바로 이동하여도 상태편과 충돌 공간이 아니니 다음 위치로 이동하여 작업이 가능하다.
			mn_VisionStep[nSite] = 1300; //모든 안전 조건 확인 완료 
		}
		break;

	case 1300:
		if(n_visionrbt_next_working_site[nSite] > SITE_MAX_POS - 1)
		{
			//////////////////////////////////////
			//정상적이면 이 조건은 나오지 않음 
			//////////////////////////////////////
			//560100 1 56 "Vision#2 위치이동 에러. 대기 위치로 이동해 주세요"
			sprintf(cAlarmCose,"560100");
			CTL_Lib.Alarm_Error_Occurrence(8052, CTL_dWARNING, cAlarmCose);
			mn_Vision2_flag = RET_ERROR;
			break;
		}
		else
		{
			if(n_visionrbt_next_working_site[nSite] == SITE_READY_POS) //0 //대기 위치로 이동 //SITE_READY_POS
			{
				mn_VisionStep[nSite] = 2000; //0 대기 위치로 이동 //SITE_READY_POS
			}
			else if(n_visionrbt_next_working_site[nSite] == SITE_LDRBT_POS)//1) //SITE_LDRBT_POS
			{
				mn_VisionStep[nSite] = 3000; //1
			}
			else if(n_visionrbt_next_working_site[nSite] == SITE_LABEL_POS) //2) //SITE_LABEL_POS
			{ //이 위치 부분은 1,2 SITE_LDRBT_POS 작업 후 바로 SITE_LABEL_POS로  이동하여 작업후 st_sync.n_visionrbt_workend_flag[VISION_1_SITE] = CTL_YES할 것으로 
			  //  이 조건은 들어오지 않는다  
				mn_VisionStep[nSite] = 4000; //2
			}
			else if(n_visionrbt_next_working_site[nSite] == SITE_VISION_POS) //3) //SITE_VISION_POS
			{
				mn_VisionStep[nSite] = 5000; //3
			}
			else if(n_visionrbt_next_working_site[nSite] == SITE_ULDRBT_POS) //4) //SITE_ULDRBT_POS
			{
				mn_VisionStep[nSite] = 6000; //4
			}
			else
			{
				sprintf(cAlarmCose,"560201");
				mn_Vision2_flag = RET_ERROR;
				CTL_Lib.Alarm_Error_Occurrence(8053, CTL_dWARNING, cAlarmCose);
			}
		}
		break;



	/////////////////////////////////////////////////////////
	// SITE_READY_POS 로 이동한다  
	/////////////////////////////////////////////////////////
	case 2000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560102 1 56 "Vision#2의 READY POS가 아닙니다."
			sprintf(cAlarmCose,"560102");
			CTL_Lib.Alarm_Error_Occurrence(8054, CTL_dWARNING, cAlarmCose);
			mn_Vision2_flag = RET_ERROR;
			break;
		}
		else
		{
			mn_VisionStep[nSite] = 2010;
		}
		break;

	case 2010:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_DOWN_READY], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 2020;//2100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 2010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision2_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8055, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 2010;
		} 
		break;

	case 2020:
		nRet_1 = COMI.Check_SingleMove(M_VISION1_Z, st_motor[M_VISION1_Z].md_pos[VISION_Z_LABEL]);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 2100;
		}
		break;
		
	case 2100:
//		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_READY_POS], st_basic.nRunSpeed);
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_LDRBT_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 2200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 2010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision2_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8056, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 2010;
		}
		break;

	case 2200:
		n_visionrbt_working_site[nSite] = 0; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 

		mn_VisionStep[nSite] = 2300;
		break;

	case 2300:
		mn_VisionStep[nSite] = 1000;
		break;
		
	/////////////////////////////////////////////////////////
	// Ready POS - > loading robot가 놓는 자재를 받는다  
	/////////////////////////////////////////////////////////
	case 3000:
		mn_VisionStep[nSite] = 3010;
		break;

		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560503 1 56 "Vision#2의 자재 LOADING POS가 아닙니다."
			sprintf(cAlarmCose,"560503");
			CTL_Lib.Alarm_Error_Occurrence(8057, CTL_dWARNING, cAlarmCose);
			mn_Vision2_flag = RET_ERROR;
			break;
		}
		else
		{
			mn_VisionStep[nSite] = 3010;
		}
		break;

	case 3010:
		mn_VisionStep[nSite] = 3100;
		break;

	case 3100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_LDRBT_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
			n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];		
			mn_VisionStep[nSite] = 3200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 3100;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision2_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8058, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 3100;
		}
		break;

	case 3200:		 
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_LDMODULE], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 3300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 3200;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision2_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8059, CTL_dWARNING, cAlarmCose);
			mn_VisionStep[nSite] = 3200;
		} 
		break;

	case 3300:
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		mn_VisionStep[nSite] = 4000;		
		break;

	///////////////////////////////////////////////////////////////////
	// Label 붙이는 작업을 한다 //SITE_LDRBT_POS - > SITE_LABEL_POS
	///////////////////////////////////////////////////////////////////
	case 4000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560504 1 56 "Vision#2의 LABEL POS가 아닙니다."
			sprintf(cAlarmCose,"560504");
			CTL_Lib.Alarm_Error_Occurrence(8060, CTL_dWARNING, cAlarmCose);
			break;
		}
		else
		{
			set_dvc_rotator(nSite, IO_ON);
			mn_VisionStep[nSite] = 4100;
		}
		break;

	case 4100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_LABEL], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 4200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 4100;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision2_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8061, CTL_dWARNING, cAlarmCose);
			mn_VisionStep[nSite] = 4100;
		} 
		break;

	case 4200:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_LABEL_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 4300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 4200;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision2_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8062, CTL_dWARNING, cAlarmCose);
			mn_VisionStep[nSite] = 4200;
		} 
		break;

	case 4300:
		n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];
		
		mn_VisionStep[nSite] = 4400;
		break;


	case 4400: //라벨을 붙을수 있는 상태인지 확인 
		nRet_1 = get_dvc_rotator(nSite, IO_ON, st_wait.i_vision_ratator_w[0], st_wait.i_vision_ratator_l);
		if(nRet_1 == CTL_GOOD || st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 4500;
		}
		else if(nRet_1 == CTL_ERROR)
		{////560600 1 56 "Vision#2의 Module Clamp 에러."
			sprintf(cAlarmCose,"560600");
			CTL_Lib.Alarm_Error_Occurrence(8063, CTL_dWARNING, cAlarmCose);
			mn_Vision2_flag = RET_ERROR;
		}
		break;

	case 4500:
		n_visionrbt_label_req[0] = CTL_REQ; 
		n_visionrbt_label_req[1] = VISION_2_SITE;
		mn_VisionStep[nSite] = 4600;
		break;

	case 4600:
		if(n_visionrbt_label_req[0] == CTL_READY) //라벨 로봇이 자재를 붙여 놓았다 
		{
			mn_VisionStep[nSite] = 4700;
		}
		break;

	case 4700:
		set_dvc_rotator(nSite, IO_OFF);
		mn_VisionStep[nSite] = 4800;
		break;

	case 4800:
		nRet_1 = get_dvc_rotator(nSite, IO_OFF, st_wait.i_vision_ratator_w[1], st_wait.i_vision_ratator_l);
		if(nRet_1 == CTL_GOOD || st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 4900;
		}
		else if(nRet_1 == CTL_ERROR)
		{//560600 1 56 "Vision#2의 Module Clamp 에러."
			sprintf(cAlarmCose,"560600");
			CTL_Lib.Alarm_Error_Occurrence(8064, CTL_dWARNING, mc_alarmcode);
			mn_Vision2_flag = RET_ERROR;
		}
		break;

	case 4900:
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료
		mn_VisionStep[nSite] = 1000;
		break;

	////////////////////////////////////////////////////////////////
	// VISION TEST을 한다 //SITE_LABEL_POS - > SITE_VISION_POS
	////////////////////////////////////////////////////////////////
	case 5000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560505 1 56 "Vision#2의 VISION Check POS가 아닙니다."
			sprintf(cAlarmCose,"560505");
			CTL_Lib.Alarm_Error_Occurrence(8065, CTL_dWARNING, cAlarmCose);
			mn_Vision2_flag = RET_ERROR;
			break;
		}
		else 
		{
			mn_VisionStep[nSite] = 5010;
		}
		break;

	case 5010:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_VISION], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 5100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 5010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision2_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8066, CTL_dWARNING ,COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 5010;
		} 
		break; 

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_VISION_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 5200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 5000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision2_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8067, CTL_dWARNING ,COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 5000;
		} 
		break; 

	case 5200:
		n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		mn_VisionStep[nSite] = 5300;
		break;

	//////////////////////////////////////////////////////////////////
	//vision test를 요청한다 
	//////////////////////////////////////////////////////////////////
	case 5300: //
		n_visionrbt_visiontest_req[nSite] = CTL_REQ;
		n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];
		mn_VisionStep[nSite] = 5310;
		break;

/////////////////////////////////////// 20120707
	case 5310:	
		if (n_visionrbt_visiontest_req[nSite] == CTL_REQ)
		{
			nRet_1 = Func.HS_Trigger();
			if(nRet_1 == RET_GOOD)
			{
				//비전 명령 
				mn_VisionStep[nSite] = 5400;
			}
		}
		break;

///////////////////////////////////////

	case 5400: //
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		mn_VisionStep[nSite] = 1000;
		break;

	///////////////////////////////////////////////////////////////////
	// ULD Robot 작업을 한다 //SITE_VISION_POS - > SITE_ULDRBT_POS
	///////////////////////////////////////////////////////////////////
	case 6000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560506 1 56 "Vision#2의 UNLOAD POS가 아닙니다."
			sprintf(cAlarmCose,"560506");
			CTL_Lib.Alarm_Error_Occurrence(8068, CTL_dWARNING, cAlarmCose);
			mn_Vision2_flag = RET_ERROR;
			break;
		}
		else
		{
			mn_VisionStep[nSite] = 6010;
		}
		break;

	case 6010:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_ULDMODULE], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 6100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 6010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision2_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8069, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 6010;
		} 
		break; 

	case 6100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_ULDRBT_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 6200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 6010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			mn_Vision2_flag = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8070, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 6010;
		} 
		break; 

	case 6200:
		n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];

		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		mn_VisionStep[nSite] = 1000;
		break;

	case 7000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560502 1 56 "Vision#1의 READY POS가 아닙니다."
			sprintf(cAlarmCose,"560502");
			CTL_Lib.Alarm_Error_Occurrence(8071, CTL_dWARNING, cAlarmCose);
			mn_Vision2_flag = RET_ERROR;
			break;
		}

		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_DOWN_READY], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 7100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 7000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(8072, CTL_dWARNING, COMI.mc_alarmcode);
			mn_Vision2_flag = RET_ERROR;
			mn_VisionStep[nSite] = 7000;
		} 
		break; 

	case 7100:		
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[SITE_READY_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 7200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 7000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(8073, CTL_dWARNING, COMI.mc_alarmcode);
			mn_Vision2_flag = RET_ERROR;
			mn_VisionStep[nSite] = 7000;
		} 
		break; 

	case 7200:
		n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다
		n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		n_visionrbt_past_working_site[nSite] = n_visionrbt_working_site[nSite];
		mn_VisionStep[nSite] = 1000;
		break;

	}
	return mn_Vision2_flag;
}

void CDialog_Part_Manual::OnBtnVisionRun() 
{
	if(m_b_stop_req == TRUE)
	{
		m_b_stop_req = FALSE;
		m_b_run_req = TRUE;
	}
	
}

int CDialog_Part_Manual::Find_VisionRobot_Current_Position(int n_site, int n_check_pos)  //로보트의 현재 위치를 확인한다 
{
	int nRet_1, nRet_2, nFlag = CTL_ERROR;
//	int n_Ocheck_pos;

	switch(n_check_pos)
	{
	case SITE_READY_POS: //0
	case SITE_LDRBT_POS: //1
	case SITE_LABEL_POS: //2
	case SITE_VISION_POS: //3
	case SITE_ULDRBT_POS: //4
		if(n_site == VISION_1_SITE)
		{
			nRet_1 = COMI.Check_MotPosRange(M_VISION1_Y, st_motor[M_VISION1_Y].md_pos[n_check_pos], COMI.mn_allow_value[M_VISION1_Y]); 
			nRet_2 = COMI.Check_MotPosRange(M_VISION1_Z, st_motor[M_VISION1_Z].md_pos[n_check_pos], COMI.mn_allow_value[M_VISION1_Z]);

			//n_Ocheck_pos = st_sync.n_visionrbt_working_site[VISION_2_SITE];
			//nRet_3 = COMI.Check_MotPosRange(M_VISION2_Y, st_motor[M_VISION2_Y].md_pos[n_Ocheck_pos], COMI.mn_allow_value[M_VISION2_Y]); 
			//nRet_4 = COMI.Check_MotPosRange(M_VISION2_Z, st_motor[M_VISION2_Z].md_pos[n_Ocheck_pos], COMI.mn_allow_value[M_VISION2_Z]);

		}
		else
		{
			nRet_1 = COMI.Check_MotPosRange(M_VISION2_Y, st_motor[M_VISION2_Y].md_pos[n_check_pos], COMI.mn_allow_value[M_VISION2_Y]); 
			nRet_2 = COMI.Check_MotPosRange(M_VISION2_Z, st_motor[M_VISION2_Z].md_pos[n_check_pos], COMI.mn_allow_value[M_VISION2_Z]);
			//n_Ocheck_pos = st_sync.n_visionrbt_working_site[M_VISION1_Y];
			//nRet_3 = COMI.Check_MotPosRange(M_VISION1_Y, st_motor[M_VISION1_Y].md_pos[n_Ocheck_pos], COMI.mn_allow_value[M_VISION1_Y]); 
			//nRet_4 = COMI.Check_MotPosRange(M_VISION1_Z, st_motor[M_VISION1_Z].md_pos[n_Ocheck_pos], COMI.mn_allow_value[M_VISION1_Z]);
		}
		break;
	}


	if(nRet_1 == BD_GOOD && nRet_2 == BD_GOOD)// && nRet_3 == BD_GOOD && nRet_4 == BD_GOOD)
	{
		nFlag = CTL_GOOD;
	}
	else
	{//안전을 위해 현재 모터가 정 위치에 있지 않으니 에러 메세지를 출력하여 작업자가 정위치로 이동하게 한다 
		//sprintf(mc_alarmcode, "0200%02d", n_check_pos); 
		//alarm.mstr_code = "020020";	
		alarm.mn_count_mode = 0;	alarm.mn_type_mode = CTL_dWARNING; //함수 밖에서 장비 스톱처리 st_work.n_run_status = CTL_dWARNING;
		nFlag = CTL_ERROR;

	} 
 
	return nFlag;
}


int CDialog_Part_Manual::set_dvc_rotator(int n_site, int n_onoff)
{
	 if(n_site == VISION_1_SITE) //0
	 {
		FAS_IO.set_out_bit(st_io.o_vis_buffer_rotator_onoff[n_site], n_onoff);
	 }
	 else //if(n_site == VISION_2_SITE) //1
	 {
		FAS_IO.set_out_bit(st_io.o_vis_buffer_rotator_onoff[n_site], n_onoff);
	 }
	mb_dvc_rotator_flag[n_site] = TRUE;
	ml_dvc_rotator_wait[n_site][0] = GetCurrentTime();
	return TRUE;
}

int CDialog_Part_Manual::get_dvc_rotator(int n_site, int n_onoff, int n_wait, int n_limit)
{
	int nFunRet=CTL_PROCEED, nRetIO[4]={0,};

	ml_dvc_rotator_wait[n_site][1] = GetCurrentTime();
	ml_dvc_rotator_wait[n_site][2] = ml_dvc_rotator_wait[n_site][1] - ml_dvc_rotator_wait[n_site][0];
	if(ml_dvc_rotator_wait[n_site][2] < 0) ml_dvc_rotator_wait[n_site][0] = GetCurrentTime();
 
	if(n_onoff == IO_ON) //
	{
		nRetIO[0] = FAS_IO.get_in_bit(st_io.i_vis_buff_rotator_on_chk[n_site], IO_ON);
		nRetIO[1] = FAS_IO.get_in_bit(st_io.i_vis_buff_rotator_off_chk[n_site], IO_OFF);
	
		if(mb_dvc_rotator_flag[n_site] == TRUE &&  nRetIO[0] == IO_ON && nRetIO[1] == IO_OFF)
		{
			ml_dvc_rotator_wait[n_site][0] = GetCurrentTime();
			mb_dvc_rotator_flag[n_site] = FALSE;
		}
		else if(mb_dvc_rotator_flag[n_site] == FALSE && nRetIO[0] == IO_ON && nRetIO[1] == IO_OFF)
		{
			if(ml_dvc_rotator_wait[n_site][2] > st_wait.i_vision_ratator_w[0])
			{
				nFunRet = CTL_GOOD;
			}			
		}
		else 
		{
			if(ml_dvc_rotator_wait[n_site][2] > st_wait.i_vision_ratator_l)
			{//560103 1 56 "ROBOT PICKER ROTATOR 90 DEGREE CHECK ERROR -[PS0501]."
				sprintf(mc_alarmcode, "560103"); 
				alarm.mn_count_mode = 0;	alarm.mn_type_mode = CTL_dWARNING; //함수 밖에서 장비 스톱처리 st_work.n_run_status = CTL_dWARNING;
				nFunRet = CTL_ERROR;
			}
		}		
	}
	else //if(n_onoff == IO_OFF) //  
	{
		nRetIO[0] = FAS_IO.get_in_bit(st_io.i_vis_buff_rotator_on_chk[n_site], IO_OFF);
		nRetIO[1] = FAS_IO.get_in_bit(st_io.i_vis_buff_rotator_off_chk[n_site], IO_ON);

		if(mb_dvc_rotator_flag[n_site] == TRUE &&  nRetIO[0] == IO_OFF && nRetIO[1] == IO_ON)
		{
			ml_dvc_rotator_wait[n_site][0] = GetCurrentTime();
			mb_dvc_rotator_flag[n_site] = FALSE;
		}
		else if(mb_dvc_rotator_flag[n_site] == FALSE && nRetIO[0] == IO_OFF && nRetIO[1] == IO_ON)
		{
			if(ml_dvc_rotator_wait[n_site][2] > st_wait.i_vision_ratator_w[1])
			{
				nFunRet = CTL_GOOD;
			}			
		}
		else 
		{
			if(ml_dvc_rotator_wait[n_site][2] > st_wait.i_vision_ratator_l)
			{//560102 1 56 "ROBOT PICKER ROTATOR 0 DEGREE CHECK ERROR -[PS0500]."
				sprintf(mc_alarmcode, "560102"); 
				//alarm.mstr_code = "020020";	
				alarm.mn_count_mode = 0;	alarm.mn_type_mode = CTL_dWARNING; //함수 밖에서 장비 스톱처리 st_work.n_run_status = CTL_dWARNING;
				nFunRet = CTL_ERROR;
			}
		}

	}
	return nFunRet;
} 

void CDialog_Part_Manual::OnBtnVisionEnd() 
{
	mn_Error_stop = YES;	
}

int CDialog_Part_Manual::InitClip()
{//init
	int nRet_1,nRet_2;
	int nFuncRet = RET_PROCEED;

	nRet_1 = InitClip_1();
	nRet_2 = InitClip_2();
	if(mn_Clip1_init == TRUE && mn_Clip2_init == TRUE)
	{
		mn_Clipinitial = TRUE;
	}

	return nFuncRet;
}

int CDialog_Part_Manual::InitClip_1()
{
	int nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	int nFuncRet = RET_PROCEED;

	if(mn_Clip1_init != FALSE) return nFuncRet;
	if(m_b_stop_req == YES) return nFuncRet;
	
	switch(ClipInitStep)
	{
	case 0:
		ClipInitStep = 10;
		break;

	case 10:
		if(FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_OFF) == IO_ON ||
			FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON) == IO_OFF)
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
			ClipInitStep = 100;
		}
		else
		{
			ClipInitStep = 1000;
		}
		break;

	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.i_picker_updn_w[1], st_wait.i_picker_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipInitStep = 900;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			//260100 0 26 "Clip Insert Picker up 에러."
			mn_Clipretry = 0;
			sprintf(mc_alarmcode,"260100");
			CTL_Lib.Alarm_Error_Occurrence(8074, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
			ClipInitStep = 10;
		}
		break;

	case 900:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 1000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 900;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			nFuncRet = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8075, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1000:
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 1100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			nFuncRet = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8076, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1100:
		ActionPicker[D_EXIST] = YES;
		Set_PickerUpDown(IO_OFF, ActionPicker);
		ClipInitStep = 1200;
		break;

	case 1200:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		if (nRet_1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			ClipInitStep = 2000;
		}
		else if (nRet_1 == CTLBD_RET_ERROR)
		{
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Clip Picker Wouldn't Picker Up.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}//260100 0 26 "Clip Insert Picker up 에러."
			sprintf(mc_alarmcode,"260100");
			CTL_Lib.Alarm_Error_Occurrence(8077, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 2000:
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 2100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(8078, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 2100:
		ClipInitStep = 2200;
		break;

	case 2200:
		ClipInitStep = 2300;
		break;

	case 2300:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		ClipInitStep = 2400;
		break;

	case 2400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if (nRet_1 == RET_GOOD)
		{
			ClipInitStep = 2500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180801");
			CTL_Lib.Alarm_Error_Occurrence(8080, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 2500:
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 2600;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2500;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(8081, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 2600:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 2700;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2600;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(8082, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 2700:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 2800;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2700;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(8083, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 2800:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 3000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2800;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(8084, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 3000:
		mn_Clip1_init = TRUE;
		nFuncRet = RET_GOOD;
		ClipInitStep = 0;
		break;
	}

	return nFuncRet;
}

int CDialog_Part_Manual::InitClip_2()
{
	int nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED, nRet_3 = RET_PROCEED;
	int nFuncRet = RET_PROCEED;

	if(mn_Clip2_init != FALSE) return nFuncRet;

	switch(ClipStackerStep)
	{
	case 0:
		ClipStackerStep = 100;
		break;

	case 100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON)
		{
			if(nRet_1 == IO_ON)
			{//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180106");
			}
			else if(nRet_2 == IO_ON)
			{//180608 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180608");
			}
			else
			{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180508");
			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8090, st_work.mn_run_status, mc_alarmcode);			
			nFuncRet = RET_ERROR;
			ClipStackerStep = 10;
		}
		else
		{
			ClipStackerStep = 1000;
		}
		break;

	case 1000:
		if(FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_fwd_chk, IO_ON) == IO_OFF ||
			FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_bwd_chk, IO_OFF) == IO_ON)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_OFF);
			ClipStackerStep = 1100;
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_OFF)
			{//일단 Up시킨다
				if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON &&
					FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_dn_chk, IO_ON) == IO_ON)
				{
					ClipStackerStep = 1300;
				}
				else
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
					FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
					ClipStackerStep = 1200;
				}
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
				ClipStackerStep = 1200;
			}
		}
		break;

	case 1010:
		FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_OFF);
		ClipStackerStep = 1100;
		break;

	case 1100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_ld_tube_s_latch_fwdbwd_onoff, st_io.i_ld_tube_s_latch_fwd_chk, st_io.i_ld_tube_s_latch_bwd_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipStackerStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180100 0 18 "로드 튜브 stacker latch On 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로드 튜브 stacker latch On 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] loading error has occurred stacker latch On the tube");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180100");
				CTL_Lib.Alarm_Error_Occurrence(8091, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipStackerStep = 1010;
			}
			else
			{
				ClipStackerStep = 1010;
			}
		}		
		break;

	case 1110:		
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
		ClipStackerStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_long_updn_onoff, st_io.i_tube_s_m_plate_long_dn_chk, st_io.i_tube_s_m_plate_long_up_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[1], st_wait.i_hs_stacker_guide_l);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_dn_chk, st_io.i_tube_s_m_plate_short_up_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[1], st_wait.i_hs_stacker_guide_l);
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			ClipStackerStep = 1300;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//180301 0 18 "로드 튜브 stacker plate short down 체크 에러가 발생했습니다."
	 	 //180401 0 18 "로드 튜브 stacker plate long down 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(nRet_1 == RET_ERROR)
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로드 튜브 stacker plate long down 체크 에러가 발생했습니다.");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_abnormal_msg, "[Initialize Error] Check loading stacker plate long down tube error occurred");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					sprintf(mc_alarmcode,"180401");
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로드 튜브 stacker plate short down 체크 에러가 발생했습니다.");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_abnormal_msg, "[Initialize Error] Check loading stacker plate short down tube error occurred");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					sprintf(mc_alarmcode,"180301");
				}
				CTL_Lib.Alarm_Error_Occurrence(8092, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipStackerStep = 1110;
			}
			else
			{
				ClipStackerStep = 1110;
			}
		}
		break;

	case 1300:
		if(FAS_IO.get_in_bit(st_io.i_tube_push_fwd_chk, IO_ON) == IO_ON ||
			FAS_IO.get_in_bit(st_io.i_tube_push_bwd_chk, IO_OFF) == IO_OFF)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			ClipStackerStep = 1400;
		}
		else
		{
			//if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_ON &&
			//	FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_OFF)
			//{//일단 Up시킨다
				if(FAS_IO.get_in_bit(st_io.i_uld_tube_s_dn_chk, IO_ON) == IO_ON &&
					FAS_IO.get_in_bit(st_io.i_uld_tube_s_up_chk, IO_OFF) == IO_OFF)
				{
					ClipStackerStep = 2000;
				}
				else
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
					ClipStackerStep = 1500;
				}
			//}
		}
		break;

	case 1310:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		ClipStackerStep = 1400;
		break;

	case 1400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_bwd_chk, st_io.i_tube_push_fwd_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipStackerStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180100 0 18 "로드 튜브 stacker latch On 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{//180900 0 18 "튜브 Pusher forward 체크 에러가 발생했습니다."
			//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 언로드 튜브 stacker latch On 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] unload error occurred stacker latch On the tube");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180901");
				CTL_Lib.Alarm_Error_Occurrence(8093, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipStackerStep = 1310;
			}
			else
			{
				ClipStackerStep = 1310;
			}
		}		
		break;

	case 1410:		
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
		ClipStackerStep = 1500;
		break;

	case 1500:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_dn_chk, st_io.i_uld_tube_s_up_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[1], st_wait.i_hs_stacker_guide_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipStackerStep = 1300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180602 0 18 "언로드 튜브 stacker up 에러가 발생했습니다."
		 //180603 0 18 "언로드 튜브 stacker down 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 언로드 튜브 stacker down 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] An error has occurred unload stacker down tube");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180603");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8094, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipStackerStep = 1410;
			}
			else
			{
				ClipStackerStep = 1410;
			}
		}
		break;

	case 2000:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
		ClipStackerStep = 2100;
		break;

	case 2100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_bwd_onoff, st_io.i_tube_s_m_plate_bwd_chk, st_io.i_tube_s_m_plate_fwd_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipStackerStep = 3000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180201 0 18 "로드 튜브 stacker plate backward 에러가 발생했습니다."
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로드 튜브 stacker plate backward 에러가 발생했습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[Initialize Error] Road tube stacker plate backward error occurred");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			sprintf(mc_alarmcode,"180201");
			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 	
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8095, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
			ClipStackerStep = 2000;
		}
		break;


	case 3000:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF);
		if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
			ClipStackerStep = 3200;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			ClipStackerStep = 3100;
		}
		break;

	case 3010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
		ClipStackerStep = 3100;
		break;

	case 3100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_bwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipStackerStep = 3000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] An error has occurred rotator tube latch On");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180700");
				st_work.mn_run_status = CTL_dWARNING;
			    CTL_Lib.Alarm_Error_Occurrence(8096, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipStackerStep = 3010;
			}
			else
			{
				ClipStackerStep = 3010;
			}
		}
		break;

	case 3110:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		ClipStackerStep = 3200;
		break;

	case 3200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 = RET_GOOD)
		{
			ClipStackerStep = 3300;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180703 0 18 "로테이터 캡 제거 Off 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 캡 제거 Off 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] An error has occurred Off rotator cap removed");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180703");
				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
				CTL_Lib.Alarm_Error_Occurrence(8097, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipStackerStep =3110;
			}
			else
			{
				ClipStackerStep = 3110;
			}
		}
		break;

	case 3300:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_up_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_dn_chk, IO_ON);
		if (nRet_1 == IO_OFF && nRet_2 == IO_ON)
		{
			ClipStackerStep = 4000;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			ClipStackerStep = 3400;
		}
		break;

	case 3310:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		ClipStackerStep = 3400;
		break;

	case 3400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipStackerStep = 3300;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Clip buffer down 체크 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] Clip buffer down check error has occurred");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180801");
				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
				CTL_Lib.Alarm_Error_Occurrence(8098, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipStackerStep =3110;
			}
			else
			{
				ClipStackerStep = 3110;
			}
		}
		break;

		
	case 4000:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_OFF);
		if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{
			ClipStackerStep = 4200;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, IO_OFF);
			ClipStackerStep = 4100;
		}
		break;

	case 4010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, IO_OFF);
		ClipStackerStep = 4100;
		break;

	case 4100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_0_onoff, st_io.i_tube_rotator_0_chk, st_io.i_tube_rotator_90_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipStackerStep = 4000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180706 0 18 "로테이터 튜브 0도 에러가 발생했습니다."
			 //180707 0 18 "로테이터 튜브 90도 에러가 발생했습니다."
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 0도 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] An error has occurred rotator tube 0 degree");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180706");
				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8099, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipStackerStep = 4010;
			}
			else
			{
				ClipStackerStep = 4010;
			}
		}
		break;	

	case 4200:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		ClipStackerStep = 4300;
		break;

	case 4300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipStackerStep = 4400;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
			 //180705 0 18 "로테이터 클램프 Off 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 클램프 Off 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] An error has occurred Rotator Clamp Off");
					}
					

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			    sprintf(mc_alarmcode,"180705");
				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8100, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipStackerStep = 3410;
			}
			else
			{
				ClipStackerStep = 3410;
			}
		}
		break;

	case 4400:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF);
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF);

		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{
			ClipStackerStep = 5000;
		}
		else
		{//180804 0 18 "Clip buffer clip#1을 제거 해 주세요."
		 //180805 0 18 "Clip buffer clip#2을 제거 해 주세요."
			if(nRet_1 == IO_ON)
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Error] Clip buffer clip#1을 제거 해 주세요.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Error] Clip buffer clip # 1, please remove");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180804");
			}
			else
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Error] Clip buffer clip#2을 제거 해 주세요.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Error] Clip buffer clip # 2, please remove");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180805");
			}
			st_work.mn_run_status = CTL_dWARNING;
			nFuncRet = RET_ERROR;
			CTL_Lib.Alarm_Error_Occurrence(8101, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 5000:
		mn_Clip2_init = TRUE;
		nFuncRet = RET_GOOD;
		ClipStackerStep = 0;
		break;
	
	}
	return nFuncRet;
}

int CDialog_Part_Manual::LoadClipMove()
{
	int i, nRet_1, nRet_2, nRet_3, nRet_4;
	int nFuncRet = RET_PROCEED;

	if(mn_Clipinitial != CTL_YES)  return nFuncRet;
	if(m_b_stop_req == YES) return nFuncRet;

	switch(LoadClipStep)
	{
	case 0:
		LoadClipStep = 10;
		break;

	case 10:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_tube_s_full_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		nRet_4 = FAS_IO.get_in_bit(st_io.i_ld_tube_s_empty_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON || nRet_4 == IO_OFF)
		{
			if(nRet_1 == IO_ON)
			{//180607 0 18 "언로드 튜브 stacker full 체크 에러가 발생했습니다."
				if(st_basic.n_mode_device != 1)
				{
					LoadClipStep = 20;
					break;
				}
				else                            sprintf(mc_alarmcode,"180607");
			}
			else if(nRet_2 == IO_ON)
			{//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180106");
			}
			else if(nRet_3 == IO_ON)
			{//180608 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180608");
			}
			else
			{//180510 0 18 "로드 튜브 stacker empty 체크 에러가 발생했습니다."
				if(st_basic.n_mode_device != 1)
				{
					LoadClipStep = 20;
					break;
				}
				else                            alarm.mstr_pcode = _T("180510");
			}
			CTL_Lib.Alarm_Error_Occurrence(8102, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		else
		{
			LoadClipStep = 20;
		}
		break;
		
	case 20:
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_ON || FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_OFF ||
			FAS_IO.get_in_bit(st_io.i_tube_push_bwd_chk, IO_OFF) == IO_OFF || FAS_IO.get_in_bit(st_io.i_tube_push_fwd_chk, IO_ON) == IO_ON)
		{//현재의 FWDBWD의 Pusher 위치가 올바르지 않다 롵ㅔ이터를 들어야 한다
			LoadClipStep = 30;
		}
		else
		{
			LoadClipStep = 100;
		}
		break;

	case 30:
		nRet_1 = OnSetRotatorUpdn(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 40;
		}
		break;

	case 40:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_OFF) == IO_OFF && FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk,IO_ON) == IO_ON)
		{
			LoadClipStep = 100;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
			LoadClipStep = 50;
		}
		break;

	case 50:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		LoadClipStep = 60;
		break;

	case 60:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 100;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180502 0 18 "로드 튜브 rotator clamp 체크 에러가 발생했습니다."
		//180503 0 18 "로드 튜브 rotator unclamp 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180503");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8103, st_work.mn_run_status, mc_alarmcode);	
				nFuncRet = RET_ERROR;
				LoadClipStep = 50;
			}
			else
			{
				LoadClipStep = 50;
			}
		}
		break;

	case 100:
		nRet_1 = CheckTransferBeforeMove();
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 110;
		}
		break;

	case 110:
		if(FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_fwd_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_bwd_chk, IO_OFF) == IO_OFF)
		{
			LoadClipStep = 200;
		}
		else
		{
			PushStep = 0;
			SetMovePusherCyliner(FALSE);
			LoadClipStep = 130;
		}		
		break;

	case 120:
		SetMovePusherCyliner(FALSE);
		LoadClipStep = 130;
		break;

	case 130:
		nRet_1 = GetMovePusherCyliner(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 200;
		}
		break;

		//Clip check
	case 200:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON || 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			LoadClipStep = 220;
		}
		else
		{
			LoadClipStep = 230;
		}
		break;

	case 220:
		nRet_1 = OnSetRotatorUpdn(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 230;
		}
		break;
		
	case 230:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk,IO_ON) == IO_ON)
		{
			LoadClipStep = 500;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
			LoadClipStep = 240;
		}
		break;

	case 240:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
		LoadClipStep = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.o_tube_rotator_unclamp_onoff, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180502 0 18 "로드 튜브 rotator clamp 체크 에러가 발생했습니다."
		//180503 0 18 "로드 튜브 rotator unclamp 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode, "180503");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8104, CTL_Lib.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				LoadClipStep = 240;
			}
			else
			{
				LoadClipStep = 240;
			}
		}
		break;

	case 500:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON)
		{
			//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
			//180608 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
			if(nRet_1 == IO_ON)	sprintf(mc_alarmcode,"180106");
			else				sprintf(mc_alarmcode,"180608");
			CTL_Lib.Alarm_Error_Occurrence(8105, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
			LoadClipStep = 500;
		}
		else
		{
			LoadClipStep = 1000;
		}
		break;

		//동작 시작
	case 1000:
		LoadClipStep = 1100;
		break;

	case 1100:
		if(n_lotend_righths_ldtray_site == CTL_YES && n_lotend_clip_ldtray_site == CTL_YES)
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_s_empty_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				n_ld_clip_tray_supply = CTL_NOTREADY;
				LoadClipStep = 10000;
				break;
			}
		}
		n_ld_clip_tray_supply = CTL_FREE;
		LoadClipStep = 1200;
		break;

	case 1200:
		if(n_ld_clip_tray_supply == CTL_REQ)
		{
			n_ld_clip_tray_supply = CTL_READY;
			LoadClipStep = 1300;
		}
		break;

	case 1300:
		nRet_1 = CheckTransferBeforeMove();
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 1400;
		}
		break;

	case 1400://하나빼기
		nRet_1 = OnLoadClipTransferStackerUpDn();
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 1500;
		}
		break;

	case 1500:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = IO_ON;
		if(nRet_1 == IO_ON)
		{
			LoadClipStep = 1600;
		}
		else
		{
			//180108 0 18 "로드 튜브에 튜브가 체크되지 않았습니다."
			sprintf(mc_alarmcode,"180108");
			st_work.mn_run_status = CTL_dWARNING;			
			CTL_Lib.Alarm_Error_Occurrence(8106, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;


	case 1600:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_OFF)
		{				
			LoadClipStep = 2000;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8107, st_work.mn_run_status, mc_alarmcode);	
			nFuncRet = RET_ERROR;
		}
		break;

	case 2000:
		nRet_1 = OnSetRotatorUpdn(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_OFF) == IO_OFF)
			{
				LoadClipStep = 2200;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
				LoadClipStep = 2100;
			}
		}
		else if(nRet_1 == RET_ERROR)
		{
			nFuncRet = RET_ERROR;
		}
		break;

	case 2010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		LoadClipStep = 2100;
		break;

	case 2100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 2200;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
			 //180705 0 18 "로테이터 클램프 Off 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 클램프 Off 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Error] An error has occurred Rotator Clamp Off");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180705");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8108, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				LoadClipStep = 2010;
			}
			else
			{
				LoadClipStep = 2010;
			}
		}
		break;

	case 2200:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_on_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_off_chk, IO_ON) == IO_ON)
		{
			LoadClipStep = 2300;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
			LoadClipStep = 2220;
		}
		break;

	case 2210:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		LoadClipStep = 2220;
		break;

	case 2220:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 2230;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180505 0 18 "로드 튜브 cap 제거 Off 체크 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180505");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8109, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
			LoadClipStep = 2210;
		}
		break;

	case 2230:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_OFF)
		{				
			LoadClipStep = 2300;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8110, st_work.mn_run_status, mc_alarmcode);	
			nFuncRet = RET_ERROR;
		}
		break;

	case 2300:
		nRet_1 = CheckTransferBeforeMove();
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			LoadClipStep = 2310;
		}
		break;

	case 2310:
		nRet_1 = OnLoadTransferMove();
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 2320;
		}
		else if(nRet_1 == RET_ERROR)
		{
			nFuncRet = RET_ERROR;
		}
		break;

	case 2320:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF);
		if(st_basic.n_mode_device != 1) nRet_1 = IO_ON;
		if(nRet_1 == IO_ON)
		{				
			LoadClipStep = 2400;
		}
		else
		{//180509 0 18 "로드 튜브 rotator에 튜브가 체크되지 않습니다."
			sprintf(mc_alarmcode,"180509");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8111, st_work.mn_run_status, mc_alarmcode);	
			nFuncRet = RET_ERROR;
		}
		break;

		
	case 2400:
		nRet_1 = OnFixLoadTube();//튜브 고정
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
			LoadClipStep = 2500;
		}
		break;

	case 2410:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
		LoadClipStep = 2500;
		break;

	case 2500:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_clamp_chk, st_io.i_tube_rotator_unclamp_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 2600;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if (mn_retry > 3)
			{//180502 0 18 "로드 튜브 rotator clamp 체크 에러가 발생했습니다."
			 //180503 0 18 "로드 튜브 rotator unclamp 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180503");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8112, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				LoadClipStep = 2410;
			}
			else
			{
				LoadClipStep = 2410;
			}
		}
		break;

	case 2600:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk, IO_OFF) == IO_OFF)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_ON);
			LoadClipStep = 2720;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
			LoadClipStep = 2700;
		}
		break;
		
	case 2610:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
		LoadClipStep = 2700;
		break;

	case 2700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_clamp_chk, st_io.i_tube_rotator_unclamp_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 2600;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if (mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180502");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8113, st_work.mn_run_status, mc_alarmcode);	
				nFuncRet = RET_ERROR;
				LoadClipStep = 2610;
			}
			else
			{
				LoadClipStep = 2610;
			}
		}
		break;

	case 2710:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_ON);
		LoadClipStep = 2720;
		break;

	case 2720:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_on_chk, st_io.i_tube_rotator_cap_remove_off_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 2800;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180902");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8114, st_work.mn_run_status, mc_alarmcode);	
				nFuncRet = RET_ERROR;
				LoadClipStep = 2710;
			}
			else
			{
				LoadClipStep = 2710;
			}
		}
		break;
		

	case 2800:
		nRet_1 = OnSetRotatorUpdn(TRUE);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
			LoadClipStep = 2820;
		}
		else if(nRet_1 == RET_ERROR)
		{
			nFuncRet = RET_ERROR;
		}
		break;

	case 2810:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		LoadClipStep = 2820;
		break;

	case 2820:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 2900;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				sprintf(mc_alarmcode,"180902");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8115, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				LoadClipStep = 2810;
			}
			else
			{
				LoadClipStep = 2810;
			}
		}
		break;

	case 2900:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = IO_OFF;
		if (nRet_1 == IO_OFF)
		{
			LoadClipStep = 3000;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8116, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 3000://바코드체크	
		n_ld_clip_tray_supply = CTL_LOCK;
		for(i = 0 ; i < st_traybuffer[LDCLIP_SITE].i_loader_row ; i++)
		{
			ClipTray[0][i] = DVC_YES;
			ClipTray[1][i] = DVC_YES;
		}
		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_CLIP1_TRAY);			// 피커 정보 
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_CLIP2_TRAY);			// 피커 정보 
		}
		LoadClipStep = 3100;
		break;

	case 3100:
		if(n_lotend_clip_ldrbt == CTL_YES)
		{
			LoadClipStep = 10000;
		}
		else if(n_ld_clip_tray_supply == CTL_CHANGE)
		{
			for(i = 0 ; i < st_traybuffer[LDCLIP_SITE].i_loader_row ; i++)
			{
				ClipTray[0][i] = DVC_NO;
				ClipTray[1][i] = DVC_NO;
			}
			if(st_handler.cwnd_main != NULL)
			{			
				st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_CLIP1_TRAY);			// 피커 정보 
				st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_CLIP2_TRAY);			// 피커 정보 
			}
			LoadClipStep = 3200;
		}
		break;

	case 3200:
		nRet_1 = ChangeLoadTray();
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 2320;
		}
		else if(nRet_1 == RET_ERROR)
		{
			nFuncRet = RET_ERROR;
		}
		break;

	case 10000:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON)
		{
			if(nRet_1 == IO_ON)
			{//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180106");
			}
			else if(nRet_2 == IO_ON)
			{//180608 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180608");
			}
			else
			{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180508");
			}
			alarm.mn_alarm_assign_section = 1800;
			CTL_Lib.Alarm_Error_Occurrence(8117, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		else
		{
			LoadClipStep = 10010;
		}
		break;

	case 10010:
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_up_chk, IO_OFF) == IO_OFF && FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_dn_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_up_chk, IO_OFF) == IO_OFF && FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
		{
			SetMovePusherCyliner(TRUE);
			LoadClipStep = 10030;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
			LoadClipStep = 10020;
		}
		break;

	case 10015:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
		LoadClipStep = 10020;
		break;

	case 10020:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_long_updn_onoff, st_io.i_tube_s_m_plate_long_dn_chk, st_io.i_tube_s_m_plate_long_up_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_dn_chk, st_io.i_tube_s_m_plate_short_up_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			LoadClipStep = 10010;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//180300 0 18 "로드 튜브 stacker plate short up 체크 에러가 발생했습니다."
	 	 //180400 0 18 "로드 튜브 stacker plate long up 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(nRet_1 != RET_PROCEED)
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate long up 체크 에러가 발생했습니다.");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_abnormal_msg, "[Error] Check loading stacker plate long up tubes An error has occurred");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					sprintf(mc_alarmcode,"180400");
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate short up 체크 에러가 발생했습니다.");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_abnormal_msg, "[Error] Check loading stacker plate short up tubes An error has occurred");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					sprintf(mc_alarmcode,"180300");
				}
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8118, st_work.mn_run_status, COMI.mc_alarmcode);
				nFuncRet = RET_ERROR;
				LoadClipStep = 10015;
			}
			else
			{
				LoadClipStep = 10015;
			}
		}
		break;

	case 10030:
		nRet_1 = GetMovePusherCyliner(TRUE);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 10100;
		}
		break;
					
	case 10100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON)
		{
			if(nRet_1 == IO_ON)
			{//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180106");
			}
			else if(nRet_2 == IO_ON)
			{//180608 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180608");
			}
			else
			{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180508");
			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8119, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		else
		{
			LoadClipStep = 10200;
		}
		break;
		
	case 10200:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			LoadClipStep = 10300;
		}
		else
		{//180709 0 18 "로테이터 튜브 90도 상태가 아닙니다."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8120, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 10300:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_OFF)
		{
			mn_retry = 0;
			LoadClipStep = 10310;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8121, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 10310:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
		LoadClipStep = 10320;
		break;

	case 10320:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_bwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			LoadClipStep = 10330;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] An error has occurred rotator tube latch On");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(8122, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				LoadClipStep = 10310;
			}
			else
			{
				LoadClipStep = 10310;
			}
		}
		break;

	case 10330:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		LoadClipStep = 10340;
		break;

	case 10340:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 10400;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] An error has occurred rotator tube latch On");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(8122, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				LoadClipStep = 10330;
			}
			else
			{
				LoadClipStep = 10330;
			}
		}
		break;

	case 10400:
		nRet_1 = OnSetRotatorUpdn(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_OFF) == IO_OFF)
			{
				LoadClipStep = 10600;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
				LoadClipStep = 10500;
			}
		}
		break;

	case 10410:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		LoadClipStep = 10500;
		break;

	case 10500:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 10600;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
			 //180705 0 18 "로테이터 클램프 Off 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 클램프 Off 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Error] An error has occurred Rotator Clamp Off");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180705");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8121, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				LoadClipStep = 10410;
			}
			else
			{
				LoadClipStep = 10410;
			}
		}
		break;

	case 10600:		
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			LoadClipStep = 10700;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			LoadClipStep = 10620;
		}
		break;

	case 10610:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		LoadClipStep = 10620;
		break;
		
	case 10620:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 10600;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] An error has occurred rotator tube latch On");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(8122, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				LoadClipStep = 10610;
			}
			else
			{
				LoadClipStep = 10610;
			}
		}
		break;

	case 10700:	
		nRet_1 = OnLoadTransferMove();
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			LoadClipStep = 10800;
		}
		break;

	case 10800:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF);
		if(st_basic.n_mode_device == 1 && (nRet_1 == IO_ON || nRet_2 == IO_OFF || nRet_3 == IO_ON))
		{
			if(nRet_1 == IO_ON)
			{//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180106");
			}
			else if(nRet_2 == IO_OFF)
			{//180608 0 18 "언로드 튜브에 튜브가 체크되지 않았습니다."
				sprintf(mc_alarmcode,"180608");
			}
			else
			{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180508");
			}
			CTL_Lib.Alarm_Error_Occurrence(8123, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		else
		{
			LoadClipStep = 10900;
		}
		break;

	case 10900:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_ON);
		LoadClipStep = 10910;
		break;

	case 10910:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_up_chk, st_io.i_uldclip_tube_dn_chk,
			IO_ON, st_wait.i_stacker_updn_w[0], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			LoadClipStep = 10920;
		}
		else if (nRet_1 != RET_PROCEED)
		{//900901 0 90 "ULD Clip tube Cover up down Error."
			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Clip Cover Error.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(mc_alarmcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(8124, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
			LoadClipStep = 10900;
		}
		break;

	case 10920:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 500)
		{
			FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
			LoadClipStep = 10930;
		}
		break;

	case 10925:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
		LoadClipStep = 10930;
		break;

	case 10930:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_dn_chk, st_io.i_uldclip_tube_up_chk, 
			IO_ON, st_wait.i_stacker_updn_w[0], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD)
		{
			LoadClipStep = 10940;
		}
		else if (nRet_1 != RET_PROCEED)
		{//900901 0 90 "ULD Clip tube Cover up down Error."
			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Clip Cover Error.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(mc_alarmcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(8125, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
			LoadClipStep = 10925;
		}
		break;

	case 10940:		
		st_sync.n_lotend_clip_ldstacker_site = YES;
		LoadClipStep = 1000;
		break;
	}
	return nFuncRet;
}

int CDialog_Part_Manual::LoadClipTransfer()
{
	int nRet_1,nRet_2;
	int nFuncRet = RET_PROCEED;
	
	if(mn_Clipinitial != CTL_YES)  return nFuncRet;
	if(m_b_stop_req == YES) return nFuncRet;
	
	switch(RunTransStep)
	{
	case 0:
		RunTransStep = 100;
		break;

	case 100:
		if(n_lotend_clip_ldtray_site == CTL_YES) return nFuncRet;
		//if tube is exist, ready
		if(FAS_IO.get_in_bit(st_io.i_ld_tube_s_empty_chk, IO_ON) == IO_ON || st_basic.n_mode_device != 1)
		{
			if(n_ld_clip_tray_supply == CTL_FREE)// CTL_FREE)
			{
				RunTransStep = 1000;
			}
		}
		else
		{
			if (n_ld_clip_tray_supply == CTL_NOTREADY)		// 텅 비었을 경우에...
			{			
				RunTransStep = 10000;
			}
		}
		break;

	case 1000:
		if(n_module_clip_req[0] == CTL_REQ)
		{
			n_module_clip_req[0] = CTL_LOCK;
			RunTransStep = 1100;
		}
		else if(n_lotend_clip_ldrbt == CTL_YES)
		{
			RunTransStep = 10000;
		}
		break;

	case 1100:
		if(n_ld_clip_tray_supply == CTL_FREE)
		{
			n_ld_clip_tray_supply = CTL_REQ;
			mn_UnloadTubeNum = 0; //튜브 개수 세팅
			mn_change_clip = NO;
			RunTransStep = 1200;
		}
		else if(n_ld_clip_tray_supply == CTL_LOCK)//준비됨
		{
			RunTransStep = 1200;
		}
		break;

	case 1200:
		if(n_ld_clip_tray_supply == CTL_LOCK)
		{
			RunTransStep = 1300;
		}
		break;

	case 1300:
		RunTransStep = 1400;
		break;

	case 1400:
		if(n_module_ldrbt_clipbuffer_req[0] == CTL_READY)
		{
			mn_retry = 0;
			RunTransStep = 1510;
		}
		break;

	case 1510:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, IO_OFF) == IO_OFF)
		{
			RunTransStep = 1700;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
			RunTransStep = 1600;
		}
		break;

	case 1520:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
		RunTransStep = 1600;
		break;

	case 1600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			RunTransStep = 1700;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180101 0 18 "로드 튜브 stacker latch Off 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode, "180101");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8126, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				RunTransStep = 1520;
			}
			else
			{
				RunTransStep = 1520;
			}
		}
		break;

	case 1700:
		mn_retry = 0;
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		RunTransStep = 1800;
		break;

	case 1710:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		RunTransStep = 1800;
		break;

	case 1800:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
			RunTransStep = 1820;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180101 0 18 "로드 튜브 stacker latch Off 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180101");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8127, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				RunTransStep = 1710;
			}
			else
			{
				RunTransStep = 1710;
			}
		}
		break;

	case 1810:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
		RunTransStep = 1820;
		break;


	case 1820:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			RunTransStep = 1830;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180101 0 18 "로드 튜브 stacker latch Off 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180101");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8128, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				RunTransStep = 1710;
			}
			else
			{
				RunTransStep = 1710;
			}
		}
		break;

	case 1830:
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		l_Until_WaitTime[0] = GetCurrentTime();
		RunTransStep = 1840;
		break;

	case 1840:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 500)
		{
			RunTransStep = 1900;
		}
		break;

	case 1900:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_ON);	
		
		//FAS_IO.set_out_bit(st_io.o_tube_rotator_0_onoff, IO_OFF);
		//FAS_IO.set_out_bit(st_io.o_tube_rotator_90_onoff, IO_ON);
		//FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);

		//RunTransStep = 1901;
		//break;
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = nRet_2 = IO_ON;
		if(nRet_1 == IO_OFF || nRet_2 == IO_OFF)
		{//180802 0 18 "Clip buffer clip#1 Clip이 체크가 되지 않습니다."
		 //180803 0 18 "Clip buffer clip#2 Clip이 체크가 되지 않습니다."
			mn_UnloadTubeNum++;
			if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
			{
				mn_Tuberetry++;
				if(mn_Tuberetry > 2)// && mn_UnloadTubeNum > st_basic.mn_lotset_clip)
				{
					/*
					if(nRet_1 == IO_OFF) sprintf(mc_alarmcode,"180802");
					else                 sprintf(mc_alarmcode,"180803"); 
					alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
					alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
					alarm.mn_alarm_assign_section = 1843;
					CTL_Lib.Alarm_Error_Occurrence(1851, st_work.mn_run_status, mc_alarmcode);*/
					mn_Tuberetry = 0;
					mn_change_clip = YES;
					st_sync.n_module_clip_req[0] = CTL_CHANGE;
					RunTransStep = 3000;
				}
				else
				{
					mn_retry = 0;
					RunTransStep = 1930;
				}
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);
				if(nRet_1 == IO_OFF) sprintf(mc_alarmcode,"180802");
				else                 sprintf(mc_alarmcode,"180803"); 
				CTL_Lib.Alarm_Error_Occurrence(8129, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				RunTransStep = 1910;
			}
		}
		else
		{
					mn_Tuberetry = 0;
					mn_change_clip = YES;
					st_sync.n_module_clip_req[0] = CTL_CHANGE;
					RunTransStep = 3000;

//			mn_UnloadTubeNum++;
//			n_module_clip_req[0] = CTL_FREE;
//			RunTransStep = 2000;
		}
		break;

	case 1901:
		FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_ON);
		RunTransStep = 1902;
		break;

	case 1902:
		FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		RunTransStep = 1904;
		break;

	case 1904:
		FAS_IO.set_out_bit(st_io.o_tube_rotator_0_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_90_onoff, IO_ON);
		RunTransStep = 1900;
		break;

	case 1910:
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);
		l_Until_WaitTime[0] = GetCurrentTime();
		RunTransStep = 1920;
		break;

	case 1920:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 500)
		{
			RunTransStep = 1900;
		}
		break;

	case 1930:
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);
		l_Until_WaitTime[0] = GetCurrentTime();
		RunTransStep = 1940;
		break;

	case 1940:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 500)
		{
			RunTransStep = 1520;
		}
		break;

	case 2000:
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);

		ClipTray[0][mn_UnloadTubeNum] = DVC_NO; //트레이에 디바이스가 없음을 셋
		ClipTray[1][mn_UnloadTubeNum] = DVC_NO; //트레이에 디바이스가 없음을 셋
		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_CLIP1_TRAY);			// 피커 정보 
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_CLIP2_TRAY);			// 피커 정보 
		}

		RunTransStep = 3000;
		break;


	case 3000:
		if(mn_change_clip == YES)
		{
			mn_change_clip = NO;
			n_ld_clip_tray_supply = CTL_CHANGE;
			RunTransStep = 3100;
		}
		else
		{
			n_module_ldrbt_clipbuffer_req[0] = CTL_CHANGE;
			RunTransStep = 1000;
		}
		break;

	case 3100:
		n_module_clip_req[0] = CTL_CHANGE;
		RunTransStep = 1200;
		break;

		
	case 10000:
		n_lotend_clip_ldtray_site = YES;
		RunTransStep = 0;
		break;
		
	}

	return nFuncRet;
}


int CDialog_Part_Manual::ChangeLoadTray()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2,nRet_3,nRet_4;

	switch(Change_step)
	{
	case 0:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_tube_s_full_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		nRet_4 = FAS_IO.get_in_bit(st_io.i_ld_tube_s_empty_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON || nRet_4 == IO_OFF)
		{
			if(nRet_1 == IO_ON)
			{//180607 0 18 "언로드 튜브 stacker full 체크 에러가 발생했습니다."
				if(st_basic.n_mode_device != 1)
				{
					Change_step = 20;
					break;
				}
				else                            sprintf(mc_alarmcode,"180607");
			}
			else if(nRet_2 == IO_ON)
			{//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180106");
			}
			else if(nRet_3 == IO_ON)
			{//180608 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180608");
			}
			else
			{//180510 0 18 "로드 튜브 stacker empty 체크 에러가 발생했습니다."
				if(st_basic.n_mode_device != 1)
				{
					Change_step = 20;
					break;
				}
				else                            sprintf(mc_alarmcode,"180510");
			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8130, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		else
		{
			Change_step = 100;
		}
		break;

	case 100://트레이plate가 들어가 있는지 체크
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_OFF)
		{
			Change_step = 1000;
		}
		else
		{
			Change_step = 200;
		}
		break;

	case 200:
		nRet_1 = CheckTransferBeforeMove(YES);
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 300;
		}
		break;

	case 300:
		if(FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_fwd_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_bwd_chk, IO_OFF) == IO_OFF)
		{
			Change_step = 1000;
		}
		else
		{
			PushStep = 0;
			SetMovePusherCyliner(FALSE);
			Change_step = 320;
		}		
		break;

	case 310:
		SetMovePusherCyliner(FALSE);
		Change_step = 320;
		break;

	case 320:
		nRet_1 = GetMovePusherCyliner(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 1000;
		}
		break;

	case 400:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			Change_step = 500;
		}
		else
		{//180501 0 18 "로드 튜브 rotator 90도 체크 에러가 발생했습니다."
			//sprintf(mc_alarmcode,"180501");
			//st_work.mn_run_status = CTL_dWARNING;
			//CTL_Lib.Alarm_Error_Occurrence(8301, st_work.mn_run_status, mc_alarmcode);
			//nFuncRet = RET_ERROR;
			Change_step = 410;
		}
		break;

	case 410:
		nRet_1 = OnSetRotatorUpdn(TRUE);
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 500;
		}
		else if(nRet_1 == RET_ERROR)
		{
			nFuncRet = RET_ERROR;
		}
		break;

	case 500:
		Change_step = 1000;
		break;

	case 1000:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			Change_step = 1200;
		}
		else
		{
			Change_step = 1100;
		}
		break;

	case 1100:
		nRet_1 = OnSetRotatorUpdn(TRUE);
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 1200;
		}
		break;

	case 1200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON)
		{
			//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
			//180608 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
			if(nRet_1 == IO_ON)	sprintf(mc_alarmcode,"180106");
			else				sprintf(mc_alarmcode,"180608");
			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			CTL_Lib.Alarm_Error_Occurrence(8131, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		else
		{
			Change_step = 2000;
		}
		break;

	case 2000:
		nRet_1 = OnLoadClipTransferStackerUpDn();
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 2100;
		}
		break;

	case 2100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = IO_ON;
		if(nRet_1 == IO_ON)
		{
			Change_step = 2200;
		}
		else
		{
			//180108 0 18 "로드 튜브에 튜브가 체크되지 않았습니다."
			sprintf(mc_alarmcode,"180108");
			st_work.mn_run_status = CTL_dWARNING;			
			CTL_Lib.Alarm_Error_Occurrence(8132, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 2200:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_OFF)
		{				
			Change_step = 2300;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8133, st_work.mn_run_status, mc_alarmcode);	
			nFuncRet = RET_ERROR;
		}
		break;

	case 2300:	
		//nRet_1 = CheckTransferBeforeMove();
		//if(nRet_1 == RET_GOOD)
		//{
			mn_retry = 0;
			Change_step = 2310;
		//}
		//else if(nRet_1 == RET_ERROR)
		//{
		//	nFuncRet = RET_ERROR;
		//}
		break;

	case 2310:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			Change_step = 2400;
		}
		else
		{//180709 0 18 "로테이터 튜브 90도 상태가 아닙니다."
			//sprintf(mc_alarmcode,"180709");
			//st_work.mn_run_status = CTL_dWARNING;
			//CTL_Lib.Alarm_Error_Occurrence(8134, st_work.mn_run_status, mc_alarmcode);
			//nFuncRet = RET_ERROR;
			Change_step = 2500;
		}
		break;

	case 2400:
		nRet_1 = OnSetRotatorUpdn(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_OFF) == IO_OFF)
			{
				Change_step = 2600;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
				Change_step = 2500;
			}
		}
		break;
		
	case 2500:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		Change_step = 2600;
		break;

	case 2600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 3000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
			 //180705 0 18 "로테이터 클램프 Off 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 클램프 Off 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Error] An error has occurred Rotator Clamp Off");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180705");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8135, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				Change_step = 2500;
			}
			else
			{
				Change_step = 2500;
			}
		}
		break;

	case 3000:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			Change_step = 3100;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			Change_step = 3010;
		}
		break;

	case 3010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		Change_step = 3020;
		break;


	case 3020:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 3000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Error] An error has occurred rotator tube latch On");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(8136, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				Change_step = 3010;
			}
			else
			{
				Change_step = 3010;
			}
		}
		break;

	case 3100:
		nRet_1 = OnLoadTransferMove();
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 3200;
		}
		else if(nRet_1 == RET_ERROR)
		{
			nFuncRet = RET_ERROR;
		}
		break;

	case 3200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_ON);
		if(st_basic.n_mode_device == 1 && (nRet_1 == IO_ON || nRet_2 == IO_OFF || nRet_3 == IO_OFF))
		{
			if(nRet_1 == IO_ON)
			{//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180106");
			}
			else if(nRet_2 == IO_OFF)
			{//180608 0 18 "언로드 튜브에 튜브가 체크되지 않았습니다."
				sprintf(mc_alarmcode,"180608");
			}
			else
			{//180509 0 18 "로드 튜브 rotator에 튜브가 체크되지 않습니다."
				sprintf(mc_alarmcode,"180509");
			}
			CTL_Lib.Alarm_Error_Occurrence(8137, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		else
		{
			Change_step = 3300;
		}
		break;

	case 3300:
	///////////////// 20120927
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_ON);
		Change_step = 3310;
		break;

	case 3310:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_up_chk, st_io.i_uld_tube_s_dn_chk,
			IO_ON, st_wait.i_stacker_updn_w[0], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			Change_step = 3320;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Tube Short Up Error.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			Change_step = 3300;
		}
		break;

	case 3320:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 300)
		{
			Change_step = 3330;
		}
		break;

	case 3330:
	/////////////////
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_ON);
		Change_step = 3400;
		break;

	case 3400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_up_chk, st_io.i_uldclip_tube_dn_chk,
			IO_ON, st_wait.i_stacker_updn_w[0], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			Change_step = 3410;//3500;//20120927
		}
		else if (nRet_1 != RET_PROCEED)
		{//900901 0 90 "ULD Clip tube Cover up down Error."
			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Clip Cover Error.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(mc_alarmcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(8138, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
			Change_step = 3330;//3300;//20120927
		}
		break;

////////////////// 20120927
	case 3410:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);//20120927
		Change_step = 3420;
		break;

	case 3420:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_dn_chk, st_io.i_uld_tube_s_up_chk,
			IO_ON, st_wait.i_stacker_updn_w[0], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			Change_step = 3500;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Tube Short Dn Error.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			Change_step = 3410;
		}
		break;

//////////////////
	case 3500:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 500)
		{
			FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
			Change_step = 3600;
		}
		break;

	case 3510:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
		Change_step = 3600;
		break;

	case 3600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_dn_chk, st_io.i_uldclip_tube_up_chk, 
			IO_ON, st_wait.i_stacker_updn_w[0], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 4000;
		}
		else if (nRet_1 != RET_PROCEED)
		{//900901 0 90 "ULD Clip tube Cover up down Error."
			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Clip Cover Error.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(mc_alarmcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(8139, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
			Change_step = 3510;
		}
		break;

	case 4000:
//		mn_retry = 0;
//		RunStep = 2310;
		nFuncRet = RET_GOOD;
		Change_step = 0;
		break;
	}

	return nFuncRet;
}

int CDialog_Part_Manual::OnLoadTransferMove()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1;
	switch(TransferMoveStep)
	{
	case 0:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_OFF) == IO_OFF)
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_up_chk, IO_OFF) == IO_OFF &&
				FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
			{
				TransferMoveStep = 100;
			}
			else
			{//180301 0 18 "로드 튜브 stacker plate short down 체크 에러가 발생했습니다."
				sprintf(mc_alarmcode,"180301");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8140, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
			}
		}
		else
		{//180707 0 18 "로테이터 튜브 90도 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180707");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8141, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 100:
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_up_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
		{
			FAS_IO.set_out_bit(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			TransferMoveStep = 200;
		}
		else
		{//180403 0 18 "로드 튜브 stacker plate long down 위치에 있지 않습니다."
			sprintf(mc_alarmcode,"180403");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8142, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 200:
		if(FAS_IO.get_in_bit(st_io.i_uld_tube_s_up_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_uld_tube_s_dn_chk, IO_ON) == IO_ON)
		{
			TransferMoveStep = 300;
		}
		else
		{//180605 0 18 "언로드 튜브 stacker down 위치에 있지 않습니다."
			sprintf(mc_alarmcode,"180605");
			st_work.mn_run_status = CTL_dWARNING;			
			CTL_Lib.Alarm_Error_Occurrence(8143, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 300:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			mn_retry = 0;
			TransferMoveStep = 1000;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			TransferMoveStep = 400;
		}
		break;

	case 310:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		TransferMoveStep = 400;
		break;

	case 400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			TransferMoveStep = 300;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Error] An error has occurred rotator tube latch On");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180700");
				st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
				CTL_Lib.Alarm_Error_Occurrence(8277, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				TransferMoveStep = 310;
			}
			else
			{
				TransferMoveStep = 310;
			}
		}
		break;


	case 900:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		TransferMoveStep = 1000;
		break;

	case 1000:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_bwd_onoff, st_io.i_tube_s_m_plate_bwd_chk, st_io.i_tube_s_m_plate_fwd_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			TransferMoveStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180201 0 18 "로드 튜브 stacker plate backward 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8144, st_work.mn_run_status, mc_alarmcode);
				FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON);
				FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
				nFuncRet = RET_ERROR;
				TransferMoveStep = 900;
			}
			else
			{
				TransferMoveStep = 900;
			}
		}
		break;
		
	case 2000:
		TransferMoveStep = 0;
		nFuncRet = RET_GOOD;
		break;
	}

	return nFuncRet;
}

int CDialog_Part_Manual::OnLoadClipTransferStackerUpDn()//하나빼기
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2;
	
	switch(TransUpDnStep)
	{
	case 0:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_s_empty_chk, IO_OFF);
		if(st_basic.n_mode_device == 0 || st_basic.n_mode_device == 2) nRet_1 = IO_ON;
		if(nRet_1 == IO_ON)
		{
			if(n_lotend_righths_ldtray_site == CTL_YES && n_lotend_clip_ldtray_site == CTL_YES)
			{
				nFuncRet = RET_GOOD;
				break;
			}
		}
		else
		{//180107 0 18 "로드 스태커에 튜브가 존재하지 않습니다. 튜브를 넣어 주세요."
			sprintf(mc_alarmcode,"180107");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8145, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
			break;
		}
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_OFF)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_ON);
			TransUpDnStep = 100;
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_up_chk, IO_OFF) == IO_OFF && FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_dn_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_up_chk, IO_OFF) == IO_OFF && FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
			{
				SetMovePusherCyliner(TRUE);
				TransUpDnStep = 5;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
				TransUpDnStep = 2;
			}
		}
		break;
		
	case 1:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
		TransUpDnStep = 2;
		break;

	case 2:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_long_updn_onoff, st_io.i_tube_s_m_plate_long_dn_chk, st_io.i_tube_s_m_plate_long_up_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_dn_chk, st_io.i_tube_s_m_plate_short_up_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			TransUpDnStep = 0;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//180300 0 18 "로드 튜브 stacker plate short up 체크 에러가 발생했습니다."
	 	 //180400 0 18 "로드 튜브 stacker plate long up 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(nRet_1 != RET_PROCEED)
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate long up 체크 에러가 발생했습니다.");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_abnormal_msg, "[Error] Check loading stacker plate long up tubes An error has occurred");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					sprintf(mc_alarmcode,"180400");
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate short up 체크 에러가 발생했습니다.");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_abnormal_msg, "[Error] Check loading stacker plate short up tubes An error has occurred");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					sprintf(mc_alarmcode,"180300");
				}
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8146, st_work.mn_run_status, COMI.mc_alarmcode);
				nFuncRet = RET_ERROR;
				TransUpDnStep = 1;
			}
			else
			{
				TransUpDnStep = 1;
			}
		}
		break;
		
	case 5:
		nRet_1 = GetMovePusherCyliner(TRUE);
		if(nRet_1 == RET_GOOD)
		{
			TransUpDnStep = 0;
		}
		break;

	case 10:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_ON);
		TransUpDnStep = 100;
		break;
		
	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_long_updn_onoff, st_io.i_tube_s_m_plate_long_up_chk, st_io.i_tube_s_m_plate_long_dn_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_up_chk, st_io.i_tube_s_m_plate_short_dn_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_ON);
			TransUpDnStep = 200;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//180300 0 18 "로드 튜브 stacker plate short up 체크 에러가 발생했습니다."
	 	 //180400 0 18 "로드 튜브 stacker plate long up 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(nRet_1 != RET_PROCEED)
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate long up 체크 에러가 발생했습니다.");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_abnormal_msg, "[Error] Check loading stacker plate long up tubes An error has occurred");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					sprintf(mc_alarmcode,"180400");
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate short up 체크 에러가 발생했습니다.");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_abnormal_msg, "[Error] Check loading stacker plate short up tubes An error has occurred");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					sprintf(mc_alarmcode,"180300");
				}
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8147, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				TransUpDnStep = 10;
			}
			else
			{
				TransUpDnStep = 10;
			}
		}
		break;

	case 110:
		FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_ON);
		TransUpDnStep = 200;
		break;

	case 200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_ld_tube_s_latch_fwdbwd_onoff, st_io.i_ld_tube_s_latch_bwd_chk, st_io.i_ld_tube_s_latch_fwd_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
			TransUpDnStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180101 0 18 "로드 튜브 stacker latch Off 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180101");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8148, st_work.mn_run_status, COMI.mc_alarmcode);
				nFuncRet = RET_ERROR;
				TransUpDnStep = 110;
			}
			else
			{
				TransUpDnStep = 110;
			}
		}
		break;

	case 300:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
		TransUpDnStep = 1000;
		break;

	case 1000:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_dn_chk, st_io.i_tube_s_m_plate_short_up_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_OFF);
			TransUpDnStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180301 0 18 "로드 튜브 stacker plate short down 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180301");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8149, st_work.mn_run_status, COMI.mc_alarmcode);
				nFuncRet = RET_ERROR;
				TransUpDnStep = 300;
			}
			else
			{
				TransUpDnStep = 300;
			}
		}
		break;

	case 1100:
		FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_OFF);
		TransUpDnStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_ld_tube_s_latch_fwdbwd_onoff, st_io.i_ld_tube_s_latch_fwd_chk, st_io.i_ld_tube_s_latch_bwd_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
			TransUpDnStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180100 0 18 "로드 튜브 stacker latch On 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180100");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8150, st_work.mn_run_status, COMI.mc_alarmcode);
				nFuncRet = RET_ERROR;
				TransUpDnStep = 1100;
			}
			else
			{
				TransUpDnStep = 1100;
			}
		}
		break;

	case 1300:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
		TransUpDnStep = 2000;
		break;

	case 2000:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_long_updn_onoff, st_io.i_tube_s_m_plate_long_dn_chk, st_io.i_tube_s_m_plate_long_up_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			TransUpDnStep = 3000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180401 0 18 "로드 튜브 stacker plate long down 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180401");
				CTL_Lib.Alarm_Error_Occurrence(8151, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				TransUpDnStep = 1300;
			}
			else
			{
				TransUpDnStep = 1300;
			}
		}
		break;

	case 3000:
		nFuncRet = RET_GOOD;
		TransUpDnStep = 0;
		break;
	}

	return nFuncRet;
}

int CDialog_Part_Manual::OnFixLoadTube()
{
	int nRet_1, nFuncRet = RET_PROCEED;
	switch(FixStep)
	{
	case 0:
		mn_retry = 0;
		FixStep = 100;
		break;

	case 100:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_ON);
		FixStep = 1000;
		break;

	case 1000:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_fwd_chk, st_io.i_tube_push_bwd_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			//FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			FixStep = 1100;
			//FixStep = 2000;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180900 0 18 "튜브 Pusher forward 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180900");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8152, st_work.mn_run_status, mc_alarmcode);	
				nFuncRet = RET_ERROR;
				FixStep = 100;
			}
			else
			{
				FixStep = 100;
			}
		}
		break;

	case 1010:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		FixStep = 1100;
		break;

	case 1100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_fwd_chk, st_io.i_ld_tube_s_latch_bwd_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			FixStep = 2000;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180901");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8153, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				FixStep = 1010;
			}
			else
			{
				FixStep = 1010;
			}
		}
		break;

	case 2000:
		nFuncRet = RET_GOOD;
		FixStep = 0;
		break;
	}
	return nFuncRet;
}

int CDialog_Part_Manual::OnSetRotatorUpdn(BOOL bUpDn)
{
	int nRet_1, nFuncRet = RET_PROCEED;
	switch(RotatorStep)
	{
	case 0:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_on_chk, IO_OFF) == bUpDn &&
			FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_off_chk, IO_ON) == !bUpDn)
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, bUpDn) == !bUpDn &&
				FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, bUpDn) == bUpDn)
			{
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, !bUpDn);
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, bUpDn);
				RotatorStep = 100;
				break;
			}
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, bUpDn);
			RotatorStep = 10;
			break;
		}

		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, !bUpDn) == !bUpDn &&
			FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, bUpDn) == bUpDn)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			RotatorStep = 200;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, !bUpDn);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, bUpDn);
			RotatorStep = 100;
		}
		break;

	case 5:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, bUpDn);
		RotatorStep = 10;
		break;

	case 10:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_on_chk, st_io.i_tube_rotator_cap_remove_off_chk,
			bUpDn, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			RotatorStep = 0;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180703 0 18 "로테이터 캡 제거 Off 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 캡 제거 Off 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] An error has occurred Off rotator cap removed");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180703");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8154, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				RotatorStep = 5;
			}
			else
			{
				RotatorStep = 5;
			}
		}
		break;

	case 90:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, !bUpDn);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, bUpDn);
		RotatorStep = 100;
		break;

	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			!bUpDn, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, bUpDn);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, !bUpDn);
			RotatorStep = 200;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180506 0 18 "로드 튜브 rotator latch On 에러가 발생했습니다."
         //180507 0 18 "로드 튜브 rotator latch Off 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180506");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8155, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				RotatorStep = 90;
			}
			else
			{
				RotatorStep = 90;
			}
		}
		break;

	case 110:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		RotatorStep = 200;
		break;

	case 200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_bwd_chk, st_io.i_tube_push_fwd_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, bUpDn);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, !bUpDn);
			RotatorStep = 300;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180901");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8156, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				RotatorStep = 110;
			}
			else
			{
				RotatorStep = 110;
			}
		}
		break;

	case 210:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, bUpDn);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, !bUpDn);
		RotatorStep = 300;
		break;

	case 300:		
		if(bUpDn == TRUE)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_90_onoff, st_io.i_tube_rotator_90_chk, st_io.i_tube_rotator_0_chk,
				IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_0_onoff, st_io.i_tube_rotator_0_chk, st_io.i_tube_rotator_90_chk,
				IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		}
		if(nRet_1 == RET_GOOD)
		{
			RotatorStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180706 0 18 "로테이터 튜브 0도 에러가 발생했습니다."
		//180707 0 18 "로테이터 튜브 90도 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				if(bUpDn == TRUE)	sprintf(mc_alarmcode,"180707");
				else				sprintf(mc_alarmcode,"180706");
				mn_retry = 0;
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8157, st_work.mn_run_status, mc_alarmcode);	
				nFuncRet = RET_ERROR;
				RotatorStep = 210;
			}
			else
			{
				RotatorStep = 210;
			}
		}
		break;

	case 1000:
		nFuncRet = RET_GOOD;
		RotatorStep = 0;
		break;
	}

	return nFuncRet;
}

void CDialog_Part_Manual::SetMovePusherCyliner(BOOL bFwdMove)
{
	if(bFwdMove == TRUE)
	{
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
		//FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
	}
	else
	{
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
		//FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_ON);
	}
	mn_retry = 0;
}

int CDialog_Part_Manual::GetMovePusherCyliner(BOOL bFwdMove)
{
	int nFuncRet = RET_PROCEED;
	int nRet_1, nRet_2;

	switch(PushStep)
	{
	case 0:
		if(bFwdMove == TRUE)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_fwd_onoff, st_io.i_tube_s_m_plate_fwd_chk,
				st_io.o_tube_s_m_plate_bwd_onoff, IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
//			nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_bwd_chk,
//				st_io.i_tube_push_fwd_chk, IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
			nRet_2 = RET_GOOD;
			if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
			{
				nFuncRet = RET_GOOD;
			}
			else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
			{
				mn_retry++;
				if(mn_retry > 3)
				{////180200 0 18 "로드 튜브 stacker plate forward 에러가 발생했습니다."
					mn_retry = 0;
					sprintf(mc_alarmcode,"180200");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(8158, st_work.mn_run_status, mc_alarmcode);
					nFuncRet = RET_ERROR;
					PushStep = 10;
				}
				else
				{
					PushStep = 10;
				}
			}
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_fwd_onoff, st_io.i_tube_s_m_plate_bwd_chk,
				st_io.o_tube_s_m_plate_fwd_onoff, IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
//			nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_fwd_chk,
//				st_io.i_tube_push_bwd_chk, IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
			nRet_2 = RET_GOOD;
			if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
			{
				nFuncRet = RET_GOOD;
			}
			else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
			{
				mn_retry++;
				if(mn_retry > 3)
				{//180201 0 18 "로드 튜브 stacker plate backward 에러가 발생했습니다."
					mn_retry = 0;
					sprintf(mc_alarmcode,"180201");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(8159, st_work.mn_run_status, mc_alarmcode);
					nFuncRet = RET_ERROR;
					PushStep = 10;
				}
				else
				{
					PushStep = 10;
				}
			}			
		}
		break;

	case 10:
		if(bFwdMove == TRUE)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
//			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
//			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_ON);
		}
		PushStep = 0;
		break;
	}
	return nFuncRet;
}


int CDialog_Part_Manual::CheckTransferBeforeMove(int nChange)
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_3;
	switch(ClipMoveStep)
	{
	case 0:
		if(nChange == NO)
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_OFF) == IO_OFF)
			{
				if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_up_chk, IO_OFF) == IO_OFF &&
					FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
				{
					ClipMoveStep = 200;
				}
				else
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
					ClipMoveStep = 100;
				}
			}
			else
			{//180707 0 18 "로테이터 튜브 90도 에러가 발생했습니다."
				sprintf(mc_alarmcode,"180707");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8160, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
			}
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_up_chk, IO_OFF) == IO_OFF &&
				FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
			{
				ClipMoveStep = 200;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
				ClipMoveStep = 100;
			}
		}
		break;

	case 10:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
		ClipMoveStep = 100;
		break;

	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_dn_chk,
			st_io.i_tube_s_m_plate_short_up_chk, IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipMoveStep = 200;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180301 0 18 "로드 튜브 stacker plate short down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180301");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8161, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
			}
			else
			{
				ClipMoveStep = 10; 
			}
		}
		break;

	case 200:
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_up_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_dn_chk, IO_ON) == IO_ON)
		{
			ClipMoveStep = 1000;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
			ClipMoveStep = 300;
		}
		break;

	case 210:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
		ClipMoveStep = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_long_updn_onoff, st_io.i_tube_s_m_plate_long_dn_chk,
			st_io.i_tube_s_m_plate_long_up_chk, IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipMoveStep = 200;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180401 0 18 "로드 튜브 stacker plate long down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180401");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8162, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
			}
			else
			{
				ClipMoveStep = 210;
			}
		}
		break;

	case 1000:
		if(FAS_IO.get_in_bit(st_io.i_uld_tube_s_up_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_uld_tube_s_dn_chk, IO_ON) == IO_ON)
		{
			ClipMoveStep = 1200;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
			ClipMoveStep = 1100;
		}
		break;
		
	case 1010:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
		ClipMoveStep = 1100;
		break;

	case 1100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_dn_chk,
			st_io.i_uld_tube_s_up_chk, IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			mn_retry = 0;
			ClipMoveStep = 1200;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180603 0 18 "언로드 튜브 stacker down 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180603");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8163, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
			}
			else
			{
				ClipMoveStep = 1010;
			}
		}
		break;

	case 1110:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		ClipMoveStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_bwd_chk,
			st_io.i_tube_push_fwd_chk, IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipMoveStep = 1300;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180900 0 18 "튜브 Pusher forward 체크 에러가 발생했습니다."
			//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180901");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(8164, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
			}
			else
			{
				ClipMoveStep = 1110;
			}
		}
		break;

	case 1300:
		nRet_3 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		if(nRet_3 == IO_ON)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_ON);
			ClipMoveStep = 1400;
		}
		else
		{
			ClipMoveStep = 2000;
		}
		break;

	case 1310:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_ON);
		ClipMoveStep = 1400;
		break;

	case 1400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_up_chk, st_io.i_uld_tube_s_dn_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			ClipMoveStep = 1500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180602 0 18 "언로드 튜브 stacker up 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180602");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8165, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 1500:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_Until_WaitTime[2] > 500)
		{
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
			ClipMoveStep = 1600;
		}
		break;

	case 1510:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
		ClipMoveStep = 1600;
		break;

	case 1600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_dn_chk, st_io.i_uld_tube_s_up_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipMoveStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180603 0 18 "언로드 튜브 stacker down 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180603");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(8166, st_work.mn_run_status, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

		
	case 2000:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			ClipMoveStep = 3000;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			ClipMoveStep = 2100;
		}
		break;

	case 2010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		ClipMoveStep = 2100;
		break;

	case 2100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipMoveStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Initialize Error] An error has occurred rotator tube latch On");
					}
					

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"180700");
				st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
				CTL_Lib.Alarm_Error_Occurrence(8167, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipMoveStep = 2010;
			}
			else
			{
				ClipMoveStep = 2010;
			}
		}
		break;

	case 3000:
		ClipMoveStep = 0;
		nFuncRet = RET_GOOD;
		break;
	}

	return nFuncRet;
}

int CDialog_Part_Manual::ClipInsertMove()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1;
	double dCurrPos,md_ClipAbsPos;

	if(mn_Clipinitial != CTL_YES)  return nFuncRet;
	if(m_b_stop_req == YES) return nFuncRet;

	switch(InsertStep)
	{
	case 0:
		n_module_ldrbt_clipbuffer_req[0] = CTL_NO;
		InsertStep = 100;
		break;

	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			InsertStep = 200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			InsertStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(8168, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
			InsertStep = 100;
		}
		break;
		

	case 200:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			InsertStep = 300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			InsertStep = 200;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(8169, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
			InsertStep = 200;
		}
		break;

	case 210:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		InsertStep = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if (nRet_1 == RET_GOOD)
		{
			InsertStep = 400;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(8170, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				InsertStep = 210;
			}
			else
			{
				InsertStep = 210;
			}
		}
		break;

	case 400:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].md_pos[LD_CLIP_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			InsertStep = 500;
		}
		else if(nRet_1 == BD_RETRY)
		{
			InsertStep = 600;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			InsertStep = 400;
			CTL_Lib.Alarm_Error_Occurrence(8171, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;
	
	case 500:
		if(FAS_IO.get_in_bit(st_io.i_clip_buff_dn_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_clip_buff_up_chk, IO_OFF) == IO_OFF)
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			InsertStep = 600;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			InsertStep = 600;
		}
		break;

	case 510:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		InsertStep = 600;
		break;

	case 600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if (nRet_1 == RET_GOOD)
		{
			n_module_ldrbt_clipbuffer_req[0] = CTL_REQ; //Clip Buffer 로딩 자재 요청 
			InsertStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(8172, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				InsertStep = 510;
			}
			else
			{
				InsertStep = 510;
			}
		}
		break;

	case 1000:
		InsertStep = 1100;
		break;

	case 1100:
		InsertStep = 1200;
		break;

	case 1200:
		md_ClipAbsPos = Func.XYRobot_MovePosValCheck(LDCLIP_SITE, 0, LD_CLIP_POS);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, md_ClipAbsPos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			InsertStep = 1210;
		}
		else if(nRet_1 == BD_RETRY)
		{
			InsertStep = 1200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(8173, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
			InsertStep = 1200;
		}
		break;

	case 1210:
		if(n_module_ldrbt_clipbuffer_req[0] == CTL_FREE)
		{
			InsertStep = 2000;
		}
		break;

	case 2000:
		dCurrPos = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
		if(dCurrPos > st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_POS] - st_motor[M_CLIP_INSERT_Y].mn_allow &&
			dCurrPos < st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_POS] + st_motor[M_CLIP_INSERT_Y].mn_allow)
		{
			InsertStep = 2100;
		}
		break;

	case 2100:
		nRet_1 = OnPickupClip(LD_CLIP_POS);
		if(nRet_1 == RET_GOOD)
		{
			InsertStep = 3000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			nFuncRet = RET_ERROR;
		}
		break;

	case 3000:
		nRet_1 = OnPlaceClip(Y_CLIP_REMOVE);
		if(nRet_1 == RET_GOOD)
		{
			n_module_ldrbt_clipbuffer_req[0] = CTL_REQ; //Clip Buffer 로딩 자재 요청 
			InsertStep = 1000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			nFuncRet = RET_ERROR;
		}
		break;

	}

	return nFuncRet;
}

int CDialog_Part_Manual::ClipBufferMove()
{
	int nRet_1,nRet_2;
	int nFuncRet = RET_PROCEED;

	if(mn_Clipinitial != CTL_YES)  return nFuncRet;
	if(m_b_stop_req == YES) return nFuncRet;

	switch(BufferStep)
	{
	case -1:
		n_module_clip_req[0] = CTL_NO;
		BufferStep = 100;
		break;

	case 0:
		BufferStep = 100;
		break;

	case 100:
		if(n_module_ldrbt_clipbuffer_req[0] == CTL_REQ)
		{
			n_module_clip_req[0] = CTL_REQ;
			BufferStep = 1000;
		}
		break;

	case 1000:
		if(n_ld_clip_tray_supply != CTL_LOCK) return nFuncRet;
		nRet_1 = LdUldClipBufferPos(LD_CLIP_POS);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
			BufferStep = 1100;
		}
		break;

	case 1010:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		BufferStep = 1100;
		break;

	case 1100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			BufferStep = 1200;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(8174, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				BufferStep = 1010;
			}
			else
			{
				BufferStep = 1010;
			}
		}
		break;

	case 1200:
		n_module_ldrbt_clipbuffer_req[0] = CTL_READY;
		BufferStep = 2000;
		break;

	case 2000:
		if (n_module_clip_req[0] == CTL_FREE)
		{
			//CTL_LOCK이 동시 접근 체크 요망
			if(n_module_ldrbt_clipbuffer_req[0] == CTL_CHANGE)
			{
				BufferStep = 2100;
			}
		}
		else if(n_module_clip_req[0] == CTL_CHANGE)
		{
			BufferStep = 2010;
		}
		break;

	case 2010:
		BufferStep = 1000;
		break;

	case 2100:
		nRet_1 = LdUldClipBufferPos(ULD_CLIP_POS);
		if(nRet_1 == RET_GOOD)
		{
			n_module_ldrbt_clipbuffer_req[0] = CTL_LOCK;
			BufferStep = 2200;
		}
		else if(nRet_1 == RET_ERROR)
		{
			nFuncRet = RET_ERROR;
		}
		break;

	case 2200:		
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = nRet_2 = IO_ON;
		if(nRet_1 == IO_OFF || nRet_2 == IO_OFF)
		{//180802 0 18 "Clip buffer clip#1 Clip이 체크가 되지 않습니다."
		 //180803 0 18 "Clip buffer clip#2 Clip이 체크가 되지 않습니다."
			if(nRet_1 == IO_OFF) sprintf(mc_alarmcode,"180802");
			else                 sprintf(mc_alarmcode,"180803"); 
			CTL_Lib.Alarm_Error_Occurrence(8175, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		else
		{
			BufferStep = 2300;
		}
		break;

	case 2300:
		if(n_module_ldrbt_clipbuffer_req[0] == CTL_LOCK)
		{
			n_module_ldrbt_clipbuffer_req[0] = CTL_FREE;
			BufferStep = 3000;
		}
		break;

	case 3000:
		BufferStep = 100;
		break;

	}

	return nFuncRet;
}


int CDialog_Part_Manual::LdUldClipBufferPos(int nPos)
{// nPos : 0 safety 1 : ld 2 : uld  
	int nRet_1,nFuncRet = RET_PROCEED;

	switch(ClipBufferStep)
	{
	case 0:
		if(FAS_IO.get_in_bit(st_io.i_clip_buff_up_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_clip_buff_dn_chk, IO_ON) == IO_ON)
		{
			ClipBufferStep = 1000;
		}
		else
		{
			mn_Clipretry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			ClipBufferStep = 100;
		}
		break;

	case 10:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		ClipBufferStep = 100;
		break;

	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipBufferStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(8176, CTL_dWARNING, mc_alarmcode);
				FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);
				nFuncRet = RET_ERROR;
				ClipBufferStep = 10;
			}
			else
			{
				ClipBufferStep = 10;
			}
		}
		break;

	case 1000:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].md_pos[nPos], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipBufferStep = 2000;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipBufferStep = 1000;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			ClipBufferStep = 1000;
			CTL_Lib.Alarm_Error_Occurrence(8177, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 2000:
		ClipBufferStep = 0;
		nFuncRet = RET_GOOD;
		break;
	}

	return nFuncRet;
}

int CDialog_Part_Manual::OnPlaceClip(int nPos)
{
	int nFuncRet = RET_PROCEED;
	double dCurr[2];
	int nRet_1;

	dCurr[0] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
	dCurr[1] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);

	switch(ClipPlaceStep)
	{
	case 0:
		if (nPos == WORK_BUFFER_1 || nPos == WORK_BUFFER_2 || nPos == WORK_BUFFER_3 || nPos == WORK_BUFFER_4
			|| nPos == Y_CLIP_REMOVE)
		{
			if(dCurr[0] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Z].mn_allow)
			{
				ClipPlaceStep = 100;
			}
			else
			{
				if(FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_OFF ||
					FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_ON)
				{
					mn_Clipretry = 0;
					FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
					ClipPlaceStep = 10;
				}
				else if(dCurr[1] < st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_CLAMP_POS] - st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow || 
					dCurr[1] > st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_CLAMP_POS] + st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow)
				{
					ClipPlaceStep = 300;
				}
				else
				{
					ClipPlaceStep = 1000;
				}
			}
		}
		else
		{
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[PLACECLIP Error] POs = %d Error",nPos);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			//260100 0 26 "Clip Insert Picker up 에러."
			//260101 0 26 "Clip Insert Picker down 에러."
			sprintf(mc_alarmcode,"260100");
			CTL_Lib.Alarm_Error_Occurrence(8178, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
			ClipPlaceStep = 0;
		}
		break;

	case 5:
		FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		ClipPlaceStep = 10;
		break;

	case 10:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.i_picker_updn_w[1], st_wait.i_picker_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipPlaceStep = 0;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{//260100 0 26 "Clip Insert Picker up 에러."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"260100");
				CTL_Lib.Alarm_Error_Occurrence(8179, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipPlaceStep = 5;
			}
			else
			{
				ClipPlaceStep = 5;
			}
		}
		break;		

	case 100:
		FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[LD_CLIP_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 100;
			CTL_Lib.Alarm_Error_Occurrence(8180, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;

		}
		break;

	case 200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.i_picker_updn_w[1], st_wait.i_picker_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipPlaceStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{//260100 0 26 "Clip Insert Picker up 에러."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"260100");
				CTL_Lib.Alarm_Error_Occurrence(8181, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipPlaceStep = 100;
			}
			else
			{
				ClipPlaceStep = 100;
			}
		}
		break;
		
	case 300:
		if(dCurr[1] > st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[1] - st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow &&
			dCurr[1] < st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[1] + st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow)
		{
			ClipPlaceStep = 1000;
		}
		else
		{
			nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_CLAMP_POS], st_basic.nRunSpeed);
			if(nRet_1 == BD_GOOD)
			{
				ClipPlaceStep = 0;
			}
			else if(nRet_1 == BD_RETRY)
			{
				ClipPlaceStep = 300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				ClipPlaceStep = 300;
				CTL_Lib.Alarm_Error_Occurrence(8182, CTL_dWARNING, COMI.mc_alarmcode);
				nFuncRet = RET_ERROR;
			}
		}
		break;

	case 1000://20121018
		if (nPos == 6)
		{
			nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y , st_motor[M_CLIP_INSERT_Y].md_pos[nPos], st_basic.nRunSpeed);
		}
		else
		{
			nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y , st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE1+nPos], st_basic.nRunSpeed);
		}
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 1600;//1200; 20121018
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 1000;
			CTL_Lib.Alarm_Error_Occurrence(8183, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 1200://2 Work Buffer Clip down POS 0701
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_WORK_PLACE1+nPos], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 1600;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 1200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 1200;
			CTL_Lib.Alarm_Error_Occurrence(8184, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 1300:
		ActionPicker[D_EXIST] = YES;
		Set_PickerUpDown(IO_ON, ActionPicker);
		ClipPlaceStep = 1400;
		break;

	case 1400:
		nRet_1 = Get_PickerUpDown(IO_ON, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPlaceStep = 1500;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			ClipPlaceStep = 1300;
			st_work.mn_run_status = dWARNING;
		}
		break;

	case 1500:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] <  PICKERSTATUS_CHECK_TIME )break;
		ClipPlaceStep = 1600;
		break;

	case 1600:
		nRet_1 = COMI.Start_SingleMove(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPlaceStep = 1610;			
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 1600;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 1600;
			CTL_Lib.Alarm_Error_Occurrence(8185, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;	

	case 1610:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME*8) break;
		///////////////////////// 20121018
		FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_off, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_on, IO_ON);	
		/////////////////////////
		l_PickerWaitTime[0] = GetCurrentTime();
		ClipPlaceStep = 1700;

	case 1700:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME*5) break;
		ClipPlaceStep = 1800;
		break;

	case 1800:
		ActionPicker[D_EXIST] = YES;
		Set_PickerUpDown(IO_OFF, ActionPicker);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 1900;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 2100;
			CTL_Lib.Alarm_Error_Occurrence(8186, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 1900:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPlaceStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
		}
		break;		

	case 2000:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0) l_PickerWaitTime[0] = GetCurrentTime();
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME) break;

		//st_modulemap.WorkBuffer[D_EXIST][nPos][LDCLIP_SITE] = DVC_YES; //WORK_BUFFER_1 2 3 4
		//st_modulemap.WorkBuffer[D_BIN][nPos][LDCLIP_SITE] = BIN_LDBUFFERBIN;		// BIN_LOADTRAYBIN
		//st_modulemap.WorkBuffer[D_RE_CNT][nPos][LDCLIP_SITE] = T_NOTTEST;			// Test Count Reset
		//st_modulemap.WorkBuffer[D_SCR_BIN][nPos][LDCLIP_SITE] = BIN_CLEAR;			// Screen Bin

		//st_modulemap.nClipRobotPicker[D_EXIST]   = DVC_NO;
		//st_modulemap.nClipRobotPicker[D_BIN]     = BIN_CLEAR;		// BIN_LOADTRAYBIN
		//st_modulemap.nClipRobotPicker[D_RE_CNT]  = T_NOTTEST;		// Test Count Reset
		//st_modulemap.nClipRobotPicker[D_SCR_BIN] = BIN_CLEAR;		// Screen Bin
		//if(st_sync.n_module_site_status[LDCLIP_SITE][nPos][0] != BUFF_DVC_LOAD)
		//{
	//		st_sync.n_module_site_status[LDCLIP_SITE][nPos][0] = BUFF_DVC_LOAD;
	//	}

	//	if(st_handler.cwnd_main != NULL)
	//	{			
	//		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_CLIP1_PICKER);			// 피커 정보 
	//	}
		ClipPlaceStep = 3000;
		break;

		
	case 3000:
		
		nFuncRet = RET_GOOD;
		ClipPlaceStep = 0;
		///////////////////////// 20121018
		FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_off, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_on, IO_OFF);
		/////////////////////////

	}

	return nFuncRet;
		
}

int CDialog_Part_Manual::OnPickupClip(int nPos)
{
	int nFuncRet = RET_PROCEED;
	double dCurr[3];
	int nRet_1,nRet_2;

	dCurr[0] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
	dCurr[1] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
	dCurr[2] = COMI.Get_MotCurrentPos(M_M_CLIP_CLAMP);
	switch(ClipPickStep)
	{
	case 0:
		if (nPos == LD_CLIP_POS || nPos == ULD_CLIP_POS || nPos == LD_CLIP_SAFETY)
		{
			if(dCurr[0] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Z].mn_allow)
			{
				ClipPickStep = 0;
				ClipPickStep = 100;
			}
			else
			{
				if(dCurr[2] < st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP] + st_motor[M_M_CLIP_CLAMP].mn_allow)
				{
					if(FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_OFF ||
						FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_ON)
					{
						mn_Clipretry = 0;
						FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
						ClipPickStep = 10;
					}
					else
					{
						ClipPickStep = 1000;
					}
				}
				else
				{
					ClipPickStep = 300;
				}

			}
		}
		else
		{
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[PICKUPCLIP Error] POs = %d Error",nPos);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			//260105 0 26 "Clip Pickup Position이 아닙니다."
			sprintf(mc_alarmcode,"260105");
			CTL_Lib.Alarm_Error_Occurrence(8187, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
			ClipPickStep = 0;
		}
		break;
		
	case 5:
		FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			ClipPickStep = 10;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ClipPickStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(8188, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
			ClipPickStep = 100;
		}
		break;
		
	case 10:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.i_picker_updn_w[1], st_wait.i_picker_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipPickStep = 0;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{//260100 0 26 "Clip Insert Picker up 에러."
			 //180904 0 18 "Clamp rbt unclamp 에러가 발생했습니다."
				if(nRet_1 != RET_PROCEED) sprintf(mc_alarmcode,"260100");
				else					  sprintf(mc_alarmcode,"180904");
				mn_Clipretry = 0;
				CTL_Lib.Alarm_Error_Occurrence(8189, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipPickStep = 5;
			}
			else
			{
				ClipPickStep = 5;
			}
		}
		break;
		

	case 100:
//		if(FAS_IO.get_in_bit(st_io.i_clip_rbt_clamp_chk, IO_OFF) == IO_OFF &&
//			FAS_IO.get_in_bit(st_io.i_clip_rbt_unclamp_chk, IO_ON) == IO_ON)
//		{
			ClipPickStep = 200;
//		}
//		else
//		{
//				sprintf(mc_alarmcode,"180904");
//				mn_Clipretry = 0;
//				CTL_Lib.Alarm_Error_Occurrence(8190, CTL_dWARNING, mc_alarmcode);
//				nFuncRet = RET_ERROR;
//				ClipPickStep = 5;
//		}
		break;

	case 200:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPickStep = 210;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPickStep = 200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPickStep = 100;
			CTL_Lib.Alarm_Error_Occurrence(8191, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 210:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_picker_rbt_updn_onoff, st_io.i_uld_picker_rbt_up_chk, st_io.i_uld_stacker1_dn_chk,
			IO_ON, st_wait.i_picker_updn_w[1], st_wait.i_picker_l);
//		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_unclamp_onoff, st_io.i_clip_rbt_unclamp_chk, st_io.i_clip_rbt_clamp_chk,
//			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		nRet_2 = RET_GOOD;
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			ClipPickStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED && nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{//180903 0 18 "Clamp rbt clamp 에러가 발생했습니다."
				mn_Clipretry = 0;
				if(nRet_1 != RET_PROCEED) sprintf(mc_alarmcode,"260100");
				else					  sprintf(mc_alarmcode,"180903");
				CTL_Lib.Alarm_Error_Occurrence(8192, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipPickStep = 100;
			}
			else
			{
				ClipPickStep = 100;
			}
		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPickStep = 0;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPickStep = 300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPickStep = 300;
			CTL_Lib.Alarm_Error_Occurrence(8193, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;
		
	case 1000:		//Y_LD_SAFETY+LD_CLIP_POS = Y_LD_TRAY_START ->LD_CLIP_POS 로 이동
		mn_Clipretry = 0;
		//FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);//2014,0304
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y , st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPickStep = 1200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPickStep = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPickStep = 1000;
			CTL_Lib.Alarm_Error_Occurrence(8194, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;

	case 1100:
		//FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);//2014,0304
		ClipPickStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
			IO_ON, st_wait.i_stacker_clip_w[0], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipPickStep = 1300;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 4)
			{////180800 0 18 "Clip buffer Up 체크 에러가 발생했습니다."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"180800");
				CTL_Lib.Alarm_Error_Occurrence(8195, CTL_dWARNING, mc_alarmcode);
				nFuncRet = RET_ERROR;
				ClipPickStep = 1100;
			}
			else
			{
				ClipPickStep = 1100;
			}
		}
		break;

	case 1300:		
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z , st_motor[M_CLIP_INSERT_Z].md_pos[LD_CLIP_POS], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipPickStep = 2000;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipPickStep = 1200;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			ClipPickStep = 1200;
			CTL_Lib.Alarm_Error_Occurrence(8196, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;	

	case 2000:
		if(FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_ON)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPickStep = 2100;
		}
		else
		{
			ActionPicker[D_EXIST] = YES;
			Set_PickerUpDown(IO_OFF, ActionPicker);
			ClipPickStep = 2010;
		}
		break;

	case 2010:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPickStep = 2100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
		}
		break;

	case 2100:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME) break;
		mn_Clipretry = 0;
		ClipPickStep = 2200;
		break;

	case 2200://20120720 -3추가
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_MIDCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPickStep = 2300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 2200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 2200;
			CTL_Lib.Alarm_Error_Occurrence(8197, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;	

	case 2300:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME*5) break;

		ClipPickStep = 2400;
		break;

	case 2400:
		nRet_1 =CTL_Lib.Single_Move(M_CLIP_INSERT_Z , st_motor[M_CLIP_INSERT_Z].md_pos[LD_CLIP_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipPickStep = 2500;//20120720
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipPickStep = 2400;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			ClipPickStep = 2400;
			CTL_Lib.Alarm_Error_Occurrence(8198, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;
////////////////////////////////////////////////// 20120720
	case 2410:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP , (st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_CLAMP] - 3), st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPickStep = 2420;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 2410;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 2410;
			CTL_Lib.Alarm_Error_Occurrence(8197, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
		}
		break;	

	case 2420:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME) break;

		ClipPickStep = 2500;
		break;
//////////////////////////////////////////////////
		
	case 2500:
		//180804 0 18 "Clip buffer clip#1에 Clip이 존재합니다. 제거 해 주세요."
		//180805 0 18 "Clip buffer clip#2에 Clip이 존재합니다. 제거 해 주세요."
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF);

		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{
			ClipPickStep = 3000;
		}
		else
		{//180804 0 18 "Clip buffer clip#1을 제거 해 주세요."
		 //180805 0 18 "Clip buffer clip#2을 제거 해 주세요."
		//260200 0 26 "Clip Insert Picker Clip이 존재하지 않습니다. 체크해 주세요."
			if(nRet_1 == IO_ON)
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Error] Clip buffer clip#1을 제거 해 주세요.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Error] Clip buffer clip # 1, please remove");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"260200");
			}
			else
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg,"[Error] Clip buffer clip#2을 제거 해 주세요.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "[Error] Clip buffer clip # 2, please remove");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(mc_alarmcode,"260200");
			}
			mn_Clipretry = 0;
			CTL_Lib.Alarm_Error_Occurrence(8199, CTL_dWARNING, mc_alarmcode);
			nFuncRet = RET_ERROR;
			ClipPickStep = 0;
		}
		break;

	case 3000:
		nClipRobotPicker[D_EXIST]   = DVC_YES;
		nClipRobotPicker[D_BIN]     = BIN_LDBUFFERBIN;
		nClipRobotPicker[D_RE_CNT]  = T_NOTTEST;
		nClipRobotPicker[D_SCR_BIN] = BIN_LDBUFFERBIN;
		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_CLIP1_PICKER);			// 피커 정보 
		}
		nFuncRet = RET_GOOD;
		ClipPickStep = 0;
		break;
	}

	return nFuncRet;
}

void CDialog_Part_Manual::Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE])
{
	if(PickerInfo[D_EXIST] == YES)
	{
		FAS_IO.set_out_bit(st_io.o_clip_rbt_updn_onoff, OnOff);
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_clip_rbt_updn_onoff, !OnOff);
	}
	b_PickerFlag = FALSE;
	l_PickerWaitTime[0] = GetCurrentTime();
}

int CDialog_Part_Manual::Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE])
{
	int FuncRet = RET_PROCEED;
	int RetVal = NO;
	char jamcode[10] = {0,};
	CString str;
	 

	RetVal = NO;

	if(OnOff == IO_ON) //Picker ON (Down이라면..)
	{
		if(PickerInfo[D_EXIST] == YES) //동작 시킨 피커면!
		{
			if(b_PickerFlag == FALSE && FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON) == IO_ON
				&& FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_OFF) == IO_OFF)
			{
				b_PickerFlag = TRUE;
				l_PickerWaitTime[0] = GetCurrentTime();   
			}
			else if(b_PickerFlag == TRUE && FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON) == IO_ON
				&& FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_OFF) == IO_OFF)
			{
				l_PickerWaitTime[1] = GetCurrentTime();  
				l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
				
				if(l_PickerWaitTime[2] < 0)
				{
					l_PickerWaitTime[0] = GetCurrentTime();
				}
				if(l_PickerWaitTime[2] > st_wait.i_picker_updn_w[1])
				{
					RetVal = YES;
				}
			}
			else
			{
				l_PickerWaitTime[1] = GetCurrentTime();  
				l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
				if(l_PickerWaitTime[2] < 0)
				{
					l_PickerWaitTime[0] = GetCurrentTime();
				}
				
				if(l_PickerWaitTime[2] > st_wait.i_picker_l)
				{
//					260100 0 26 "Clip Insert Picker up 에러."
//					260101 0 26 "Clip Insert Picker down 에러."
					sprintf(mc_alarmcode,"260101"); 
					alarm.mn_count_mode = 0;	
					alarm.mn_type_mode = eWARNING; 
					alarm.mn_alarm_assign_section = 186;
					CTL_Lib.Alarm_Error_Occurrence(8200, CTL_dWARNING, mc_alarmcode);

					FuncRet = RET_ERROR;
				}
			}
		}
		else RetVal = YES;
	}
	else //Picker OFF (UP이라면..)
	{
		if(PickerInfo[D_EXIST] == YES) //동작 시킨 피커면!
		{
			if(b_PickerFlag == FALSE && FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_OFF
				&& FAS_IO.Get_In_Bit(st_io.i_clip_rbt_up_chk) == IO_ON)
			{
				b_PickerFlag = TRUE;
				l_PickerWaitTime[0] = GetCurrentTime();   
			}
			else if(b_PickerFlag == TRUE && FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_OFF
				&& FAS_IO.Get_In_Bit(st_io.i_clip_rbt_up_chk) == IO_ON)
			{
				l_PickerWaitTime[1] = GetCurrentTime();  
				l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
				
				if(l_PickerWaitTime[2] > st_wait.i_picker_updn_w[0])
				{
					RetVal = YES;
				}
			}
			else
			{
				l_PickerWaitTime[1] = GetCurrentTime();  
				l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
				if(l_PickerWaitTime[2] < 0)
				{
					l_PickerWaitTime[0] = GetCurrentTime();
				}
				
				if(l_PickerWaitTime[2] > st_wait.i_picker_l)
				{
					if(st_handler.cwnd_list != NULL)
					{
						str = _T("[UPICKER Error] Picker up sensor check error.");
						sprintf(st_msg.c_normal_msg,"%s", str);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
						Func.On_LogFile_Add(0, str);
						Func.On_LogFile_Add(99, str);
					}
//					260100 0 26 "Clip Insert Picker up 에러."
//					260101 0 26 "Clip Insert Picker down 에러."
					alarm.mstr_pcode = _T("260100"); 
					sprintf(mc_alarmcode,"260100"); 
					alarm.mn_count_mode = 0;	
					alarm.mn_type_mode = eWARNING; 
					//st_handler.i_run_status = dWARNING;

					alarm.mn_alarm_assign_section = 187;
					CTL_Lib.Alarm_Error_Occurrence(8201, CTL_dWARNING, mc_alarmcode);

					FuncRet = RET_ERROR;
				}
			}
		}
		else RetVal = YES;
	}
	
	if(RetVal == YES) 
	{
		FuncRet = RET_GOOD;
	}

	return FuncRet;
}

void CDialog_Part_Manual::OnBtnCliptestRun() 
{
	if(m_b_stop_req == TRUE)
	{
		m_b_stop_req = FALSE;
		m_b_run_req = TRUE;
	}	
}

/////////////////////////// 20120718
int CDialog_Part_Manual::StackerMovingExcution2()//SD체크
{
	int n_move_flag = CTLBD_RET_PROCEED;
	int nRet_1;

	mn_MotorNum2 = m_n_motor_no2;
	mn_StackerPos2 = HSL_STACKER_1;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// =============================================================================
	if (m_b_stop_req)
	{
		m_n_move_stacker_step2 = 3000;
	}
	// =============================================================================

	switch(m_n_move_stacker_step2)
	{
		case 0 :
			if(mn_MotorNum2 == M_HS_F_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_tray_chk, IO_ON);
			if(nRet_1 == IO_OFF)
			{				
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{				
					//000101 0 00 "Stacker1에 트레이가 존재하지 않습니다."
					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Does not exist on the tray Stacker1");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
//				n_move_flag = CTLBD_RET_ERROR;
				m_n_move_stacker_step2 = 20;
			}
			else
			{
				m_n_move_stacker_step2 = 20;
			}
			break;

		case 20:
			if(mn_MotorNum2 == M_HS_F_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 있습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker1");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_move_stacker_step2 = 30;
			}
			break;

		case 30:
			nRet_1 = Set_RailFwdbwdCylinderOnOff2(mn_StackerPos2, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step2 = 100;
			}
			break;

		case 100:
			nRet_1 = Get_RailFwdbwdCylinderOnOff2(mn_StackerPos2, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_n_move_stacker_step2 = 200;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 200:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_tray_chk, IO_OFF);
			
			if (nRet_1 == IO_OFF)
			{
				FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker_guide_clamp_onoff, IO_ON);
				FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker_guide_unclamp_onoff, IO_OFF);
				m_n_move_stacker_step2 = 300;
			}
			else// if (nRet == IO_OFF)
			{
				m_n_move_stacker_step2 = 2000;
			}
			break;
		
		case 210:
			FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker_guide_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker_guide_unclamp_onoff, IO_OFF);
			m_n_move_stacker_step2 = 300;
			break;

		case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_fwd_stacker_guide_clamp_onoff, st_io.i_hs_fwd_stacker_guide_clamp_on_chk, st_io.i_hs_fwd_stacker_guide_unclamp_off_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker_guide_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker_guide_unclamp_onoff, IO_ON);
				m_n_move_stacker_step2 = 400;
			}
//			else 
//			{
//				m_n_move_stacker_step2 = 210;
//			}
			break;

		case 310:
			FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker_guide_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker_guide_unclamp_onoff, IO_ON);
			m_n_move_stacker_step2 = 400;
			break;

		case 400:
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_fwd_stacker_guide_clamp_onoff, st_io.i_hs_fwd_stacker_guide_unclamp_off_chk, st_io.i_hs_fwd_stacker_guide_clamp_on_chk,
				IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step2 = 1000;
			}
			else if(nRet_1 != RET_PROCEED)
			{
				m_n_move_stacker_step2 = 310;
			}
			break;


		case 1000:
			nRet_1 = OnLefthsStackerReadyPos(mn_StackerPos2,1); //SD 대기
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step2 = 1100;
			}
			else if(nRet_1 == CTL_FULL)
			{
				//000302 1 01 "Loader Stocker 1 Tray Full 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "HS Left Stocker 1 Tray Full error.");

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				n_move_flag = CTLBD_RET_ERROR;
			}
			else if(nRet_1 == CTLBD_RET_SKIP) //스태커에 트레이가 없을때 
			{//000301 0 00 "Loader Stocker 1에 트레이 없음 에러"
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "HS Left Stocker 1에 트레이 없음 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "No tray error on HS Left Stocker 1");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;


		case 1100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000100 0 00 "Stacker1에 트레이가 존재합니다.제거해 주세요."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재합니다.제거해 주세요.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 the tray is present, Please remove");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_move_stacker_step2 = 1300;
			}
			break;

		case 1300:
			if ((FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_fwd_chk + (mn_StackerPos2), IO_OFF) == IO_OFF) && 
				(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_bwd_chk + (mn_StackerPos2), IO_ON) == IO_ON))	// Off 상태.
			{
				m_n_move_stacker_step2 = 1500;
			}
			else
			{
				nRet_1 = Set_RailFwdbwdCylinderOnOff2(mn_StackerPos2, CYLINDER_OFF);
				if(nRet_1 == CYLINDER_ON)
				{
					m_n_move_stacker_step2 = 1400;
				}
			}
			break;

		case 1310:
			nRet_1 = Set_RailFwdbwdCylinderOnOff2(mn_StackerPos2, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step2 = 1400;
			}
			break;

			
		case 1400:
			nRet_1 = Get_RailFwdbwdCylinderOnOff2(mn_StackerPos2, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_n_move_stacker_step2 = 1500;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if(mn_StackerPos2 == 0)
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not backward");
					}

				}
				else 
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward");
					}

				}
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
				m_n_move_stacker_step2 = 1310;
			}
			break;

		case 1500:
			nRet_1 = OnLefthsStackerReadyPos(mn_StackerPos2, 0); //트레이 하나 Up한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step2 = 1600;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_n_move_stacker_step2 = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 1600:
			nRet_1 = COMI.Get_MotIOSensor(mn_MotorNum2, MOT_SENS_SD);
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
	//			st_sync.mn_ld_module_tray_supply[mn_StackerPos] = CTL_FREE;		// 교체 끝났다고 설정.
//				m_n_move_stacker_step2 = 300;	
				m_n_move_stacker_step2 = 1700;	
			}
			/////////////// 20120719
			else if(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_tray_chk, IO_ON) == IO_ON && nRet_1 == CTLBD_RET_ERROR)
			{
				m_n_move_stacker_step2 = 1700;
			}
    		///////////////

			else
			{
				m_n_move_stacker_step2 = 1100;
			}
			break;	

		case 1700:
			nRet_1 = Set_RailFwdbwdCylinderOnOff2(0, CYLINDER_ON);
			if(nRet_1 == CTL_GOOD)
			{
				m_n_move_stacker_step2 = 1800;
			}
			break;

		case 1800:
			nRet_1 = Get_RailFwdbwdCylinderOnOff2(0, CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step2 = 2000;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{	
				//000203 0 00 "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."			
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 forward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not forward");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;	
				m_n_move_stacker_step2 = 1700;
			}
			break;


		case 2000:
			nRet_1 = OnLefthsStackerReadyPos(mn_StackerPos2, 2); //트레 하나 Dn한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step2 = 2100;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_n_move_stacker_step2 = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 2100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_tray_chk, IO_ON);
			
			if (nRet_1 == IO_ON)	// 트레이가 있다
			{
				m_n_move_stacker_step2 = 3000;
			}
			else
			{
				//000201 0 00 "Stacker1 레일에 트레이가 존재하지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail does not exist in the tray");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_n_move_stacker_step2 = 2100;
			}
			break;

		case 3000:
			m_n_move_stacker_step2 = 0;
			n_move_flag = CTLBD_RET_GOOD;
			break;
	}


	return  n_move_flag;
}

int CDialog_Part_Manual::OnLefthsStackerReadyPos(int n_StackerPos2, int nMode2)
{
	int nRet_1 = VARIABLE_INIT;
	int nRet_2 = VARIABLE_INIT;
	int nFuncRet = CTLBD_RET_PROCEED;
	double dNowPos2;
	int nMotorNum2;


	if(n_StackerPos2 == HSL_STACKER_1)
	{
		nMotorNum2 = M_HS_F_STACKER_1;
	}
	else
	{
		nMotorNum2 = M_HS_F_STACKER_2;
	}

	if(m_n_motor_no2 == M_HS_F_STACKER_1) nMotorNum2 = M_HS_F_STACKER_1;

	if(FAS_IO.get_in_bit(st_io.i_stacker_light_curtain_chk, IO_ON) == IO_ON )
	{
		COMI.Set_MotStop(0, nMotorNum2);
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Error] Area sensor is detected. Motor stop. ReWork Please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		CTL_Lib.mn_single_motmove_step[nMotorNum2] = 0;
		return nFuncRet;
	}

//	if(alarm.n_area_ready_alarm[0] == TRUE)
//	{
//		COMI.Set_MotStop(0, nMotorNum2);
//		CTL_Lib.mn_single_motmove_step[nMotorNum2] = 0;
//		return nFuncRet;
//	}

	switch(ReadyPosStep2)
	{
	case 0:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum2, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep2 = 1000;
		}
		else
		{
			ReadyPosStep2 = 100;
		}
		break;

	case 100:
		COMI.Set_Motor_IO_Property(nMotorNum2, cmSD_EN, cmTRUE);
		nRet_1 =  COMI.Start_SingleMove(nMotorNum2, st_motor[nMotorNum2].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep2 = 200;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		else if(nRet_1 == CTLBD_RET_SAFETY)	
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		break;

	case 200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum2, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, nMotorNum2);
			ReadyPosStep2 = 300;
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum2, st_motor[nMotorNum2].md_pos[P_STOCKER_UP]);
			if(nRet_1 == CTLBD_RET_GOOD)// 정상 동작을 했다면...
			{
				if (nMode2 == 0)
				{
					nRet_1 = COMI.Get_MotIOSensor(nMotorNum2, MOT_SENS_SD);
					nRet_2 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_tray_chk + (n_StackerPos2 * 3), IO_OFF);
					if(nRet_1 == CTLBD_RET_GOOD || nRet_2 == IO_ON)
					{
						if(nRet_1 == CTLBD_RET_GOOD)
						{
							ReadyPosStep2 = 2000;//1000; //20120719
						}
						else
						{
							ReadyPosStep2 = 5000;
						}					
					}
					else
					{
						ReadyPosStep2 = 2000;//1000; //20120719
					}
				}
				else if (nMode2 == 2)// SD 감지후에 DOWN하는 모드
				{
					ReadyPosStep2 = 300;
				}
				else//SD 감지거나, UP_LIMIT까지만 동작하는 모드이다.
				{
					ReadyPosStep2 = 5000;
				}
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
			{
				ReadyPosStep2 = 100;
			}
		}
		break;

	case 300:
		if (nMode2 == 0)//한장 올린다
		{
			dNowPos2 = COMI.Get_MotCurrentPos(nMotorNum2);
			if(nMotorNum2 == M_HS_F_STACKER_2)
			{
				m_dTargetPos2 = dNowPos2 + st_motor[nMotorNum2].md_pos[P_STOCKER_UP_OFFSET] - (st_motor[nMotorNum2].mn_allow * 30);
			}
			else
			{
				m_dTargetPos2 = dNowPos2 + st_motor[nMotorNum2].md_pos[P_STOCKER_UP_OFFSET];
			}
			if(m_dTargetPos2 > st_motor[nMotorNum2].md_pos[P_STOCKER_P_LIMIT])
			{
				m_dTargetPos2 = st_motor[nMotorNum2].md_pos[P_STOCKER_P_LIMIT];
				m_b_p_limit_check2 = TRUE;//20120719
			}
			ReadyPosStep2 = 400;
		}
		else if (nMode2 == 2)//한장만큼 내린다.
		{
			dNowPos2 = COMI.Get_MotCurrentPos(nMotorNum2);			
			m_dTargetPos2 = dNowPos2 - st_motor[nMotorNum2].md_pos[P_STOCKER_DN_OFFSET];
			
			if (m_dTargetPos2 < st_motor[nMotorNum2].md_pos[P_STOCKER_DOWN])		m_dTargetPos2 = st_motor[nMotorNum2].md_pos[P_STOCKER_DOWN];
			
			ReadyPosStep2 = 400;
		}
		else//SD감지 대기한다
		{
			ReadyPosStep2 = 5000;
			ReadyPosStep2 = 1000;
		}
		break;

	case 400:
		nRet_2 = COMI.Get_MotIOSensor(nMotorNum2, MOT_SENS_SD);
		COMI.Set_Motor_IO_Property(nMotorNum2, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum2, m_dTargetPos2, st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			if(nMode2 == 0)//UP
			{
			//////////////////////////////////// 20120719
				if (m_b_p_limit_check2 == TRUE)
				{
					m_b_p_limit_check2 = FALSE;
					ReadyPosStep2 = 2000;
				}
			////////////////////////////////////

				else if(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_tray_chk, IO_ON) == IO_ON)
				{
					ReadyPosStep2 = 5000;
				}
			//////////////////////////////////// 20120719
				else if(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_tray_chk, IO_OFF) == IO_OFF && nRet_2 == CTLBD_RET_GOOD)
				{
					ReadyPosStep2 = 5000;
				}
			////////////////////////////////////
				else
				{
					ReadyPosStep2 = 2000;
				}
			}
			else
			{
				ReadyPosStep2 = 5000;
			}

		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep2 = 400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			ReadyPosStep2 = 400;
		}
		break;

	case 1000:
		COMI.Set_Motor_IO_Property(nMotorNum2, cmSD_EN, cmFALSE);
		nRet_1 = COMI.Start_SingleMove(nMotorNum2, st_motor[nMotorNum2].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ReadyPosStep2 = 1100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep2 = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		break;

	case 1100:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum2, MOT_SENS_SD);
		if(nRet_1 != BD_GOOD)
		{
			COMI.Set_MotStop(0, nMotorNum2);
			
			if (nMode2 == 1)//대기
			{
				ReadyPosStep2 = 5000;
			}
			else
			{
				ReadyPosStep2 = 100;											// SD센서에 감지가 되지 않으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
			}
			
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum2, st_motor[nMotorNum2].md_pos[P_STOCKER_DOWN]);
			if (nRet_1 == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
			{
				ReadyPosStep2 = 1200;
//				ReadyPosStep2 = 2000;
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
//				ReadyPosStep2 = 1000;
			}
			else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
			{
				ReadyPosStep2 = 5000;
			}
		}
		break;

	case 1200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum2, MOT_SENS_SD); 
		if(nRet_1 == CTLBD_RET_GOOD) //이미 감지하고 있으면 트레이가 FULL까지 있다는 이야기임 
		{
			if(nMotorNum2 == M_HS_F_STACKER_2)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_up_chk + (n_StackerPos2), IO_OFF) == IO_ON ||
					FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_dn_chk + (n_StackerPos2), IO_ON) == IO_OFF)
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker1_up_onoff + (n_StackerPos2), IO_ON);
					FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker1_dn_onoff + (n_StackerPos2), IO_OFF);
					ReadyPosStep2 = 1300;
				}
				else
				{
					nFuncRet = CTL_FULL;
					ReadyPosStep2 = 0;
				}
			}
			else
			{
				nFuncRet = CTL_FULL;
				ReadyPosStep2 = 0;		
			}
		}
		else
		{//Full sensor는 감지되지 않았으니 다시확인한다 
			ReadyPosStep2 = 1210;		
		}
		break;

	case 1210:
//		FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker2_up_onoff + (n_StackerPos2), IO_ON);
//		FAS_IO.OnCylinderAction(st_io.o_hs_fwd_stacker2_dn_onoff + (n_StackerPos2), IO_OFF);
		ReadyPosStep2 = 1300;
		break;

	case 1300:
//		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_fwd_stacker2_up_onoff + (n_StackerPos2), st_io.i_hs_fwd_stacker1_up_chk + (n_StackerPos2),
//			                                  st_io.i_hs_fwd_stacker2_dn_chk + (n_StackerPos2), IO_ON, st_wait.i_stacker_updn_w[0], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD)
		{
			ReadyPosStep2 = 100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)				
			{
				mn_retry = 0;
				//010206 0 01 "Stacker2 레일 Down 센서 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "stacker2 레일 Down 센서 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Down stacker2 rail sensor error");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				ReadyPosStep2 = 1210;
			}
			else
			{
				ReadyPosStep2 = 1210;
			}
		}
		break;		


	case 2000://tray는 있으나, UP Pos까지 갔는데 SD가 감지 않된다, stacker를 Up한 후 SD센서를 본다
		if(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_updn_pos_chk + (n_StackerPos2), IO_OFF) == IO_OFF)
		{//실린더 up한다	
			 if ((FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_up_chk + (n_StackerPos2), IO_ON) == IO_ON) && 
				(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_dn_chk + (n_StackerPos2), IO_OFF) == IO_OFF))
			 {
				ReadyPosStep2 = 2300;
			 }
			 else
			 {
				ReadyPosStep2 = 2100;
			 }			
		}
		else
		{
//			if ((FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_up_chk + (n_StackerPos2 * 4), IO_ON) == IO_ON) && 
//			 (FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_dn_chk + (n_StackerPos2 * 4), IO_OFF) == IO_OFF))
//			{
//				if(nMode2 == 0)
//				{
//					ReadyPosStep2 = 5000;
//				}
//				else
//				{
//					ReadyPosStep2 = 2010;
//				}
//			}
//			else
//			{
				ReadyPosStep2 = 2010;
//			}	

		}
		break;

	case 2010:
		COMI.Set_Motor_IO_Property(nMotorNum2, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum2, st_motor[nMotorNum2].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep2 = 2020;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep2 = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			ReadyPosStep2 = 2000;
		}		
		break;

	case 2020:
		if(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_updn_pos_chk + (n_StackerPos2), IO_OFF) == IO_OFF)
		{//실린더 up한다	
			 if ((FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_up_chk + (n_StackerPos2), IO_ON) == IO_ON) && 
				(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_dn_chk + (n_StackerPos2), IO_OFF) == IO_OFF))
			 {
				ReadyPosStep2 = 2300;
			 }
			 else
			 {
				ReadyPosStep2 = 2100;
			 }			
		}
		else
		{
			ReadyPosStep2 = 2300;
		}
		break;

	case 2100:
		nRet_1 = Set_StackerUpDnClinder2(n_StackerPos2, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep2 = 2200;
		}
		break;

	case 2200:
		nRet_1 = Get_StackerUpDnClinder2(n_StackerPos2, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			ReadyPosStep2 = 100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000205 0 00 "Stacker1 레일 Up 센서 에러."
			if(n_StackerPos2 == 0)			sprintf(st_msg.c_abnormal_msg, "Stacker1 레일 Up 센서 에러.");
			else                            sprintf(st_msg.c_abnormal_msg, "Stacker2 레일 Up 센서 에러.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				if(n_StackerPos2 == 0)			sprintf(st_msg.c_abnormal_msg, "Up Stacker1 rail sensor error.");
				else                            sprintf(st_msg.c_abnormal_msg, "Up Stacker2 rail sensor error.");
			}

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{				
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
			ReadyPosStep2 = 2100;
		}
		break;

	case 2300:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_tray_chk + (n_StackerPos2), IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			ReadyPosStep2 = 2400;
		}
		else// if(nRet_1 == IO_OFF)
		{
			ReadyPosStep2 = 100;
		}
		break;

	case 2400:
		nRet_1 = Set_StackerUpDnClinder2(n_StackerPos2, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep2 = 2500;
		}
		break;

	case 2500:
		nRet_1 = Get_StackerUpDnClinder2(n_StackerPos2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			ReadyPosStep2 = 2600;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000206 0 00 "Stacker1 레일 애주 센서 에러."
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "Stacker1 레일 센서 에러.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "Down Stacker1 rail sensor error.");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
			ReadyPosStep2 = 2400;
		}
		break;

	case 2600:
		nFuncRet = CTLBD_RET_SKIP;
		ReadyPosStep2 = 0;
		break;

	case 5000:
		nFuncRet = CTLBD_RET_GOOD;
		ReadyPosStep2 = 0;
		break;	
	}

	return nFuncRet;

}

int CDialog_Part_Manual::Set_RailFwdbwdCylinderOnOff2(int nStacker2, int n_onoff2)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff2 == CYLINDER_OFF)
	{
	}

	FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker1_rail_fwd_onoff + (nStacker2*2), n_onoff2);
	FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker1_rail_bwd_onoff + (nStacker2*2), !n_onoff2);
	mb_pushercylinder_flag[nStacker2] = FALSE;
	ml_pushercylinder_Time[nStacker2][0] = GetCurrentTime();
	return nFuncRet;
	
}
int CDialog_Part_Manual::Get_RailFwdbwdCylinderOnOff2(int nStacker2, int n_onoff2)
{
	int nFuncRet = -1;
	
	ml_pushercylinder_Time[nStacker2][1] = GetCurrentTime();
	
	if (mb_pushercylinder_flag[nStacker2] != TRUE)
	{
		ml_pushercylinder_Time[nStacker2][0] = GetCurrentTime();
		mb_pushercylinder_flag[nStacker2] = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_fwd_chk + (nStacker2*3), IO_ON) == IO_ON) && 
		(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_bwd_chk + (nStacker2*3), IO_OFF) == IO_OFF))	// On 상태.
	{
		ml_pushercylinder_Time[nStacker2][2] = ml_pushercylinder_Time[nStacker2][1] - ml_pushercylinder_Time[nStacker2][0];
		
		if (ml_pushercylinder_Time[nStacker2][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker2][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff2 == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_pushercylinder_Time[nStacker2][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker2] = FALSE;
			}
		}
		else if (ml_pushercylinder_Time[nStacker2][2] > st_wait.i_stacker_f_b_w[1])
		{
			nFuncRet = CYLINDER_ON;
			mb_pushercylinder_flag[nStacker2] = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_fwd_chk + (nStacker2*3), IO_OFF) == IO_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_rail_bwd_chk + (nStacker2*3), IO_ON) == IO_ON))	// Off 상태.
	{
		ml_pushercylinder_Time[nStacker2][2] = ml_pushercylinder_Time[nStacker2][1] - ml_pushercylinder_Time[nStacker2][0];
		
		if (ml_pushercylinder_Time[nStacker2][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker2][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff2 == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_pushercylinder_Time[nStacker2][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker2] = FALSE;
			}
		}
		else if (ml_pushercylinder_Time[nStacker2][2] > st_wait.i_stacker_f_b_w[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_pushercylinder_flag[nStacker2] = FALSE;
		}
	}
	else
	{
		ml_pushercylinder_Time[nStacker2][2] = ml_pushercylinder_Time[nStacker2][1] - ml_pushercylinder_Time[nStacker2][0];
		
		if (ml_pushercylinder_Time[nStacker2][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker2][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_pushercylinder_Time[nStacker2][2] > st_wait.i_stacker_f_b_l)
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_pushercylinder_flag[nStacker2] = FALSE;
		}
	}
	
	return nFuncRet;
}

int CDialog_Part_Manual::Set_StackerUpDnClinder2(int nStacker2, int n_onoff2)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff2 == CYLINDER_OFF)
	{
		if(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_tray_chk + (nStacker2 * 4), IO_OFF) == IO_ON)
		{
			sprintf(st_msg.c_abnormal_msg, "Left Heat Sink#l stacker에 트레이가 존재합니다.제거해 주세요.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "Left Heat Sink # l stacker tray to exist., Please remove.");
			}

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			nFuncRet = CYLINDER_ERROR;
		}
	}
	FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker1_up_onoff + (nStacker2 * 2), n_onoff2);
	FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker1_dn_onoff + (nStacker2 * 2), !n_onoff2);
	mb_updncylinder_flag[nStacker2] = FALSE;
	ml_undncylinder_Time[nStacker2][0] = GetCurrentTime();

	return nFuncRet;
}

int CDialog_Part_Manual::Get_StackerUpDnClinder2(int nStacker2, int n_onoff2)
{
	int nFuncRet = -1;
	
	ml_undncylinder_Time[nStacker2][1] = GetCurrentTime();
	
	if (mb_updncylinder_flag[nStacker2] != TRUE)
	{
		ml_undncylinder_Time[nStacker2][0] = GetCurrentTime();
		mb_updncylinder_flag[nStacker2] = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_up_chk + (nStacker2 * 4), IO_ON) == CTL_ON) && 
		(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_dn_chk + (nStacker2 * 4), IO_OFF) == CTL_OFF))	// On 상태.
	{
		ml_undncylinder_Time[nStacker2][2] = ml_undncylinder_Time[nStacker2][1] - ml_undncylinder_Time[nStacker2][0];
		
		if (ml_undncylinder_Time[nStacker2][2] < 0)		
		{
			ml_undncylinder_Time[nStacker2][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff2 == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_undncylinder_Time[nStacker2][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_updncylinder_flag[nStacker2] = FALSE;
			}
		}
		else if (ml_undncylinder_Time[nStacker2][2] > st_wait.i_stacker_f_b_w[1])
		{
			nFuncRet = CYLINDER_ON;
			mb_updncylinder_flag[nStacker2] = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_up_chk + (nStacker2 * 4), IO_OFF) == CTL_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_dn_chk + (nStacker2 * 4), IO_ON) == CTL_ON))	// Off 상태.
	{
		ml_undncylinder_Time[nStacker2][2] = ml_undncylinder_Time[nStacker2][1] - ml_undncylinder_Time[nStacker2][0];
		
		if (ml_undncylinder_Time[nStacker2][2] < 0)		
		{
			ml_undncylinder_Time[nStacker2][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff2 == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_undncylinder_Time[nStacker2][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_updncylinder_flag[nStacker2] = FALSE;
			}
		}
		else if (ml_undncylinder_Time[nStacker2][2] > st_wait.i_stacker_f_b_w[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_updncylinder_flag[nStacker2]= FALSE;
		}
	}
	else
	{
		ml_undncylinder_Time[nStacker2][2] = ml_undncylinder_Time[nStacker2][1] - ml_undncylinder_Time[nStacker2][0];
		
		if (ml_undncylinder_Time[nStacker2][2] < 0)		
		{
			ml_undncylinder_Time[nStacker2][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_undncylinder_Time[nStacker2][2] > st_wait.i_stacker_f_b_l)
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_updncylinder_flag[nStacker2] = FALSE;
		}
	}
	
	return nFuncRet;

}


/////////////////////////// 20120719
int CDialog_Part_Manual::StackerMovingExcution3()//SD체크
{
	int n_move_flag = CTLBD_RET_PROCEED;
	int nRet_1;

	mn_MotorNum3 = m_n_motor_no3;
	mn_StackerPos3 = HSR_STACKER_1;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// =============================================================================
	if (m_b_stop_req)
	{
		m_n_move_stacker_step3 = 3000;
	}
	// =============================================================================

	switch(m_n_move_stacker_step3)
	{
		case 0 :
			if(mn_MotorNum3 == M_HS_B_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_tray_chk, IO_ON);
			if(nRet_1 == IO_OFF)
			{				
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{				
					//000101 0 00 "Stacker1에 트레이가 존재하지 않습니다."
					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Does not exist on the tray Stacker1.");
					}
					

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
//				n_move_flag = CTLBD_RET_ERROR;
				m_n_move_stacker_step3 = 20;
			}
			else
			{
				m_n_move_stacker_step3 = 20;
			}
			break;

		case 20:
			if(mn_MotorNum3 == M_HS_B_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 있습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker1.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_move_stacker_step3 = 30;
			}
			break;

		case 30:
			nRet_1 = Set_RailFwdbwdCylinderOnOff3(mn_StackerPos3, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step3 = 100;
			}
			break;

		case 100:
			nRet_1 = Get_RailFwdbwdCylinderOnOff3(mn_StackerPos3, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_n_move_stacker_step3 = 200;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 200:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF);
			
			if (nRet_1 == IO_OFF)
			{
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_ON);
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_OFF);
				m_n_move_stacker_step3 = 300;
			}
			else// if (nRet == IO_OFF)
			{
				m_n_move_stacker_step3 = 2000;
			}
			break;
		
		case 210:
			FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_OFF);
			m_n_move_stacker_step3 = 300;
			break;

		case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_guide_clamp_onoff, st_io.i_hs_bwd_stacker_guide_clamp_on_chk, st_io.i_hs_bwd_stacker_guide_unclamp_off_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_ON);
				m_n_move_stacker_step3 = 400;
			}
//			else 
//			{
//				m_n_move_stacker_step3 = 210;
//			}
			break;

		case 310:
			FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_ON);
			m_n_move_stacker_step3 = 400;
			break;

		case 400:
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_guide_clamp_onoff, st_io.i_hs_bwd_stacker_guide_unclamp_off_chk, st_io.i_hs_bwd_stacker_guide_clamp_on_chk,
				IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step3 = 1000;
			}
			else if(nRet_1 != RET_PROCEED)
			{
				m_n_move_stacker_step3 = 310;
			}
			break;


		case 1000:
			nRet_1 = OnRighthsStackerReadyPos(mn_StackerPos3,1); //SD 대기
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step3 = 1100;
			}
			else if(nRet_1 == CTL_FULL)
			{
				//000302 1 01 "Loader Stocker 1 Tray Full 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "HS Right Stacker 1 Tray Full error.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				n_move_flag = CTLBD_RET_ERROR;
			}
			else if(nRet_1 == CTLBD_RET_SKIP) //스태커에 트레이가 없을때 
			{//000301 0 00 "Loader Stocker 1에 트레이 없음 에러"
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "HS Right Stacker 1에 트레이 없음 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "No tray error on HS Right Stacker 1.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;


		case 1100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000100 0 00 "Stacker1에 트레이가 존재합니다.제거해 주세요."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재합니다.제거해 주세요.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 the tray is present., Please remove.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_move_stacker_step3 = 1300;
			}
			break;

		case 1300:
			if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_fwd_chk + (mn_StackerPos3), IO_OFF) == IO_OFF) && 
				(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_bwd_chk + (mn_StackerPos3), IO_ON) == IO_ON))	// Off 상태.
			{
				m_n_move_stacker_step3 = 1500;
			}
			else
			{
				nRet_1 = Set_RailFwdbwdCylinderOnOff3(mn_StackerPos3, CYLINDER_OFF);
				if(nRet_1 == CYLINDER_ON)
				{
					m_n_move_stacker_step3 = 1400;
				}
			}
			break;

		case 1310:
			nRet_1 = Set_RailFwdbwdCylinderOnOff3(mn_StackerPos3, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step3 = 1400;
			}
			break;

			
		case 1400:
			nRet_1 = Get_RailFwdbwdCylinderOnOff3(mn_StackerPos3, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_n_move_stacker_step3 = 1500;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if(mn_StackerPos3 == 0)
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not backward.");
					}

				}
				else 
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward.");
					}

				}
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
				m_n_move_stacker_step3 = 1310;
			}
			break;

		case 1500:
			nRet_1 = OnRighthsStackerReadyPos(mn_StackerPos3, 0); //트레이 하나 Up한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step3 = 1600;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_n_move_stacker_step3 = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 1600:
			nRet_1 = COMI.Get_MotIOSensor(mn_MotorNum3, MOT_SENS_SD);
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
	//			st_sync.mn_ld_module_tray_supply[mn_StackerPos] = CTL_FREE;		// 교체 끝났다고 설정.
//				m_n_move_stacker_step3 = 300;	
				m_n_move_stacker_step3 = 1700;	
			}
			/////////////// 20120719
			else if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_ON) == IO_ON && nRet_1 == CTLBD_RET_ERROR)
			{
				m_n_move_stacker_step3 = 1700;
			}
    		///////////////

			else
			{
				m_n_move_stacker_step3 = 1100;
			}
			break;	

		case 1700:
			nRet_1 = Set_RailFwdbwdCylinderOnOff3(0, CYLINDER_ON);
			if(nRet_1 == CTL_GOOD)
			{
				m_n_move_stacker_step3 = 1800;
			}
			break;

		case 1800:
			nRet_1 = Get_RailFwdbwdCylinderOnOff3(0, CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step3 = 2000;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{	
				//000203 0 00 "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."			
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s","Stacker1 레일푸셔 forward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "%s","Stacker1 rail pusher does not forward the operation.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;	
				m_n_move_stacker_step3 = 1700;
			}
			break;


		case 2000:
			nRet_1 = OnRighthsStackerReadyPos(mn_StackerPos3, 2); //트레 하나 Dn한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step3 = 2100;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_n_move_stacker_step3 = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 2100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_ON);
			
			if (nRet_1 == IO_ON)	// 트레이가 있다
			{
				m_n_move_stacker_step3 = 3000;
			}
			else
			{
				//000201 0 00 "Stacker1 레일에 트레이가 존재하지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail does not exist in the tray.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_n_move_stacker_step3 = 2100;
			}
			break;

		case 3000:
			m_n_move_stacker_step3 = 0;
			n_move_flag = CTLBD_RET_GOOD;
			break;
	}


	return  n_move_flag;
}

int CDialog_Part_Manual::OnRighthsStackerReadyPos(int n_StackerPos3, int nMode3)
{
	int nRet_1 = VARIABLE_INIT;
	int nRet_2 = VARIABLE_INIT;
	int nFuncRet = CTLBD_RET_PROCEED;
	double dNowPos3;
	int nMotorNum3;


	if(n_StackerPos3 == HSL_STACKER_1)
	{
		nMotorNum3 = M_HS_B_STACKER_1;
	}
	else
	{
		nMotorNum3 = M_HS_B_STACKER_2;
	}

	if(m_n_motor_no3 == M_HS_B_STACKER_1) nMotorNum3 = M_HS_B_STACKER_1;

	if(FAS_IO.get_in_bit(st_io.i_stacker_light_curtain_chk, IO_ON) == IO_ON )
	{
		COMI.Set_MotStop(0, nMotorNum3);
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Error] Area sensor is detected. Motor stop. ReWork Please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		CTL_Lib.mn_single_motmove_step[nMotorNum3] = 0;
		return nFuncRet;
	}

//	if(alarm.n_area_ready_alarm[0] == TRUE)
//	{
//		COMI.Set_MotStop(0, nMotorNum3);
//		CTL_Lib.mn_single_motmove_step[nMotorNum3] = 0;
//		return nFuncRet;
//	}

	switch(ReadyPosStep3)
	{
	case 0:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum3, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep3 = 1000;
		}
		else
		{
			ReadyPosStep3 = 100;
		}
		break;

	case 100:
		COMI.Set_Motor_IO_Property(nMotorNum3, cmSD_EN, cmTRUE);
		nRet_1 =  COMI.Start_SingleMove(nMotorNum3, st_motor[nMotorNum3].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep3 = 200;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		else if(nRet_1 == CTLBD_RET_SAFETY)	
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		break;

	case 200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum3, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, nMotorNum3);
			ReadyPosStep3 = 300;
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum3, st_motor[nMotorNum3].md_pos[P_STOCKER_UP]);
			if(nRet_1 == CTLBD_RET_GOOD)// 정상 동작을 했다면...
			{
				if (nMode3 == 0)
				{
					nRet_1 = COMI.Get_MotIOSensor(nMotorNum3, MOT_SENS_SD);
					nRet_2 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk + (n_StackerPos3 * 3), IO_OFF);
					if(nRet_1 == CTLBD_RET_GOOD || nRet_2 == IO_ON)
					{
						if(nRet_1 == CTLBD_RET_GOOD)
						{
							ReadyPosStep3 = 2000;//1000; //20120719
						}
						else
						{
							ReadyPosStep3 = 5000;
						}					
					}
					else
					{
						ReadyPosStep3 = 2000;//1000; //20120719
					}
				}
				else if (nMode3 == 2)// SD 감지후에 DOWN하는 모드
				{
					ReadyPosStep3 = 300;
				}
				else//SD 감지거나, UP_LIMIT까지만 동작하는 모드이다.
				{
					ReadyPosStep3 = 5000;
				}
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
			{
				ReadyPosStep3 = 100;
			}
		}
		break;

	case 300:
		if (nMode3 == 0)//한장 올린다
		{
			dNowPos3 = COMI.Get_MotCurrentPos(nMotorNum3);
			if(nMotorNum3 == M_HS_B_STACKER_2)
			{
				m_dTargetPos3 = dNowPos3 + st_motor[nMotorNum3].md_pos[P_STOCKER_UP_OFFSET] - (st_motor[nMotorNum3].mn_allow * 30);
			}
			else
			{
				m_dTargetPos3 = dNowPos3 + st_motor[nMotorNum3].md_pos[P_STOCKER_UP_OFFSET];
			}
			if(m_dTargetPos3 > st_motor[nMotorNum3].md_pos[P_STOCKER_P_LIMIT])
			{
				m_dTargetPos3 = st_motor[nMotorNum3].md_pos[P_STOCKER_P_LIMIT];
				m_b_p_limit_check3 = TRUE;//20120719
			}
			ReadyPosStep3 = 400;
		}
		else if (nMode3 == 2)//한장만큼 내린다.
		{
			dNowPos3 = COMI.Get_MotCurrentPos(nMotorNum3);			
			m_dTargetPos3 = dNowPos3 - st_motor[nMotorNum3].md_pos[P_STOCKER_DN_OFFSET];
			
			if (m_dTargetPos3 < st_motor[nMotorNum3].md_pos[P_STOCKER_DOWN])		m_dTargetPos3 = st_motor[nMotorNum3].md_pos[P_STOCKER_DOWN];
			
			ReadyPosStep3 = 400;
		}
		else//SD감지 대기한다
		{
//			ReadyPosStep3 = 5000;
			ReadyPosStep3 = 1000;
		}
		break;

	case 400:
		nRet_2 = COMI.Get_MotIOSensor(nMotorNum3, MOT_SENS_SD);
		COMI.Set_Motor_IO_Property(nMotorNum3, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum3, m_dTargetPos3, st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			if(nMode3 == 0)//UP
			{
			//////////////////////////////////// 20120719
				if (m_b_p_limit_check3 == TRUE)
				{
					m_b_p_limit_check3 = FALSE;
					ReadyPosStep3 = 2000;
				}
			////////////////////////////////////

				else if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_ON) == IO_ON)
				{
					ReadyPosStep3 = 5000;
				}
			//////////////////////////////////// 20120719
				else if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF) == IO_OFF && nRet_2 == CTLBD_RET_GOOD)
				{
					ReadyPosStep3 = 5000;
				}
			////////////////////////////////////
				else
				{
					ReadyPosStep3 = 2000;
				}
			}
			else
			{
				ReadyPosStep3 = 5000;
			}

		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep3 = 400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			ReadyPosStep3 = 400;
		}
		break;

	case 1000:
		COMI.Set_Motor_IO_Property(nMotorNum3, cmSD_EN, cmFALSE);
		nRet_1 = COMI.Start_SingleMove(nMotorNum3, st_motor[nMotorNum3].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ReadyPosStep3 = 1100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep3 = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		break;

	case 1100:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum3, MOT_SENS_SD);
		if(nRet_1 != BD_GOOD)
		{
			COMI.Set_MotStop(0, nMotorNum3);
			
			if (nMode3 == 1)//대기
			{
				ReadyPosStep3 = 5000;
			}
			else
			{
				ReadyPosStep3 = 100;											// SD센서에 감지가 되지 않으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
			}
			
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum3, st_motor[nMotorNum3].md_pos[P_STOCKER_DOWN]);
			if (nRet_1 == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
			{
				ReadyPosStep3 = 1200;
//				ReadyPosStep3 = 2000;
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
//				ReadyPosStep3 = 1000;
			}
			else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
			{
				ReadyPosStep3 = 5000;
			}
		}
		break;

	case 1200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum3, MOT_SENS_SD); 
		if(nRet_1 == CTLBD_RET_GOOD) //이미 감지하고 있으면 트레이가 FULL까지 있다는 이야기임 
		{
			if(nMotorNum3 == M_HS_B_STACKER_2)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_up_chk + (n_StackerPos3), IO_OFF) == IO_ON ||
					FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_dn_chk + (n_StackerPos3), IO_ON) == IO_OFF)
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker1_up_onoff + (n_StackerPos3), IO_ON);
					FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker1_dn_onoff + (n_StackerPos3), IO_OFF);
					ReadyPosStep3 = 1300;
				}
				else
				{
					nFuncRet = CTL_FULL;
					ReadyPosStep3 = 0;
				}
			}
			else
			{
				nFuncRet = CTL_FULL;
				ReadyPosStep3 = 0;		
			}
		}
		else
		{//Full sensor는 감지되지 않았으니 다시확인한다 
			ReadyPosStep3 = 1210;		
		}
		break;

	case 1210:
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker2_up_onoff + (n_StackerPos3), IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker2_dn_onoff + (n_StackerPos3), IO_OFF);
		ReadyPosStep3 = 1300;
		break;

	case 1300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker2_up_onoff + (n_StackerPos3), st_io.i_hs_bwd_stacker1_up_chk + (n_StackerPos3),
			                                  st_io.i_hs_bwd_stacker2_dn_chk + (n_StackerPos3), IO_ON, st_wait.i_stacker_updn_w[0], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD)
		{
			ReadyPosStep3 = 100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)				
			{
				mn_retry = 0;
				//010206 0 01 "Stacker2 레일 Down 센서 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "stacker2 레일 Down 센서 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Down stacker2 rail sensor error.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				ReadyPosStep3 = 1210;
			}
			else
			{
				ReadyPosStep3 = 1210;
			}
		}
		break;		


	case 2000://tray는 있으나, UP Pos까지 갔는데 SD가 감지 않된다, stacker를 Up한 후 SD센서를 본다
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_updn_pos_chk + (n_StackerPos3), IO_OFF) == IO_OFF)
		{//실린더 up한다	
			 if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_up_chk + (n_StackerPos3), IO_ON) == IO_ON) && 
				(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_dn_chk + (n_StackerPos3), IO_OFF) == IO_OFF))
			 {
				ReadyPosStep3 = 2300;
			 }
			 else
			 {
				ReadyPosStep3 = 2100;
			 }			
		}
		else
		{
//			if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_up_chk + (n_StackerPos3 * 4), IO_ON) == IO_ON) && 
//			 (FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_dn_chk + (n_StackerPos3 * 4), IO_OFF) == IO_OFF))
//			{
//				if(nMode3 == 0)
//				{
//					ReadyPosStep3 = 5000;
//				}
//				else
//				{
//					ReadyPosStep3 = 2010;
//				}
//			}
//			else
//			{
				ReadyPosStep3 = 2010;
//			}	

		}
		break;

	case 2010:
		COMI.Set_Motor_IO_Property(nMotorNum3, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum3, st_motor[nMotorNum3].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep3 = 2020;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep3 = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			ReadyPosStep3 = 2000;
		}		
		break;

	case 2020:
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_updn_pos_chk + (n_StackerPos3), IO_OFF) == IO_OFF)
		{//실린더 up한다	
			 if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_up_chk + (n_StackerPos3), IO_ON) == IO_ON) && 
				(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_dn_chk + (n_StackerPos3), IO_OFF) == IO_OFF))
			 {
				ReadyPosStep3 = 2300;
			 }
			 else
			 {
				ReadyPosStep3 = 2100;
			 }			
		}
		else
		{
			ReadyPosStep3 = 2300;
		}
		break;

	case 2100:
		nRet_1 = Set_StackerUpDnClinder3(n_StackerPos3, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep3 = 2200;
		}
		break;

	case 2200:
		nRet_1 = Get_StackerUpDnClinder3(n_StackerPos3, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			ReadyPosStep3 = 100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000205 0 00 "Stacker1 레일 Up 센서 에러."
			if(n_StackerPos3 == 0)			sprintf(st_msg.c_abnormal_msg, "Stacker1 레일 Up 센서 에러.");
			else                            sprintf(st_msg.c_abnormal_msg, "Stacker2 레일 Up 센서 에러.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				if(n_StackerPos3 == 0)			sprintf(st_msg.c_abnormal_msg, "Up Stacker1 rail sensor error.");
				else                            sprintf(st_msg.c_abnormal_msg, "Up Stacker2 rail sensor error.");
			}

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{				
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
			ReadyPosStep3 = 2100;
		}
		break;

	case 2300:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_tray_chk + (n_StackerPos3), IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			ReadyPosStep3 = 2400;
		}
		else// if(nRet_1 == IO_OFF)
		{
			ReadyPosStep3 = 100;
		}
		break;

	case 2400:
		nRet_1 = Set_StackerUpDnClinder3(n_StackerPos3, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep3 = 2500;
		}
		break;

	case 2500:
		nRet_1 = Get_StackerUpDnClinder3(n_StackerPos3, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			ReadyPosStep3 = 2600;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000206 0 00 "Stacker1 레일 애주 센서 에러."
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "Stacker1 레일 센서 에러.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 rail sensor error.");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
			ReadyPosStep3 = 2400;
		}
		break;

	case 2600:
		nFuncRet = CTLBD_RET_SKIP;
		ReadyPosStep3 = 0;
		break;

	case 5000:
		nFuncRet = CTLBD_RET_GOOD;
		ReadyPosStep3 = 0;
		break;	
	}

	return nFuncRet;

}

int CDialog_Part_Manual::Set_RailFwdbwdCylinderOnOff3(int nStacker3, int n_onoff3)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff3 == CYLINDER_OFF)
	{
	}

	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker1_rail_fwd_onoff + (nStacker3*2), n_onoff3);
	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker1_rail_bwd_onoff + (nStacker3*2), !n_onoff3);
	mb_pushercylinder_flag[nStacker3] = FALSE;
	ml_pushercylinder_Time[nStacker3][0] = GetCurrentTime();
	return nFuncRet;
	
}
int CDialog_Part_Manual::Get_RailFwdbwdCylinderOnOff3(int nStacker3, int n_onoff3)
{
	int nFuncRet = -1;
	
	ml_pushercylinder_Time[nStacker3][1] = GetCurrentTime();
	
	if (mb_pushercylinder_flag[nStacker3] != TRUE)
	{
		ml_pushercylinder_Time[nStacker3][0] = GetCurrentTime();
		mb_pushercylinder_flag[nStacker3] = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_fwd_chk + (nStacker3*3), IO_ON) == IO_ON) && 
		(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_bwd_chk + (nStacker3*3), IO_OFF) == IO_OFF))	// On 상태.
	{
		ml_pushercylinder_Time[nStacker3][2] = ml_pushercylinder_Time[nStacker3][1] - ml_pushercylinder_Time[nStacker3][0];
		
		if (ml_pushercylinder_Time[nStacker3][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker3][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff3 == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_pushercylinder_Time[nStacker3][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker3] = FALSE;
			}
		}
		else if (ml_pushercylinder_Time[nStacker3][2] > st_wait.i_stacker_f_b_w[1])
		{
			nFuncRet = CYLINDER_ON;
			mb_pushercylinder_flag[nStacker3] = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_fwd_chk + (nStacker3*3), IO_OFF) == IO_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_bwd_chk + (nStacker3*3), IO_ON) == IO_ON))	// Off 상태.
	{
		ml_pushercylinder_Time[nStacker3][2] = ml_pushercylinder_Time[nStacker3][1] - ml_pushercylinder_Time[nStacker3][0];
		
		if (ml_pushercylinder_Time[nStacker3][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker3][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff3 == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_pushercylinder_Time[nStacker3][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker3] = FALSE;
			}
		}
		else if (ml_pushercylinder_Time[nStacker3][2] > st_wait.i_stacker_f_b_w[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_pushercylinder_flag[nStacker3] = FALSE;
		}
	}
	else
	{
		ml_pushercylinder_Time[nStacker3][2] = ml_pushercylinder_Time[nStacker3][1] - ml_pushercylinder_Time[nStacker3][0];
		
		if (ml_pushercylinder_Time[nStacker3][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker3][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_pushercylinder_Time[nStacker3][2] > st_wait.i_stacker_f_b_l)
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_pushercylinder_flag[nStacker3] = FALSE;
		}
	}
	
	return nFuncRet;
}

int CDialog_Part_Manual::Set_StackerUpDnClinder3(int nStacker3, int n_onoff3)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff3 == CYLINDER_OFF)
	{
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_tray_chk + (nStacker3 * 4), IO_OFF) == IO_ON)
		{
			sprintf(st_msg.c_abnormal_msg, "Right Heat Sink#l stacker에 트레이가 존재합니다.제거해 주세요.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "Right Heat Sink # l stacker tray to exist., Please remove.");
			}

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			nFuncRet = CYLINDER_ERROR;
		}
	}
	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker1_up_onoff + (nStacker3 * 2), n_onoff3);
	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker1_dn_onoff + (nStacker3 * 2), !n_onoff3);
	mb_updncylinder_flag[nStacker3] = FALSE;
	ml_undncylinder_Time[nStacker3][0] = GetCurrentTime();

	return nFuncRet;
}

int CDialog_Part_Manual::Get_StackerUpDnClinder3(int nStacker3, int n_onoff3)
{
	int nFuncRet = -1;
	
	ml_undncylinder_Time[nStacker3][1] = GetCurrentTime();
	
	if (mb_updncylinder_flag[nStacker3] != TRUE)
	{
		ml_undncylinder_Time[nStacker3][0] = GetCurrentTime();
		mb_updncylinder_flag[nStacker3] = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_up_chk + (nStacker3 * 4), IO_ON) == CTL_ON) && 
		(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_dn_chk + (nStacker3 * 4), IO_OFF) == CTL_OFF))	// On 상태.
	{
		ml_undncylinder_Time[nStacker3][2] = ml_undncylinder_Time[nStacker3][1] - ml_undncylinder_Time[nStacker3][0];
		
		if (ml_undncylinder_Time[nStacker3][2] < 0)		
		{
			ml_undncylinder_Time[nStacker3][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff3 == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_undncylinder_Time[nStacker3][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_updncylinder_flag[nStacker3] = FALSE;
			}
		}
		else if (ml_undncylinder_Time[nStacker3][2] > st_wait.i_stacker_f_b_w[1])
		{
			nFuncRet = CYLINDER_ON;
			mb_updncylinder_flag[nStacker3] = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_up_chk + (nStacker3 * 4), IO_OFF) == CTL_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_dn_chk + (nStacker3 * 4), IO_ON) == CTL_ON))	// Off 상태.
	{
		ml_undncylinder_Time[nStacker3][2] = ml_undncylinder_Time[nStacker3][1] - ml_undncylinder_Time[nStacker3][0];
		
		if (ml_undncylinder_Time[nStacker3][2] < 0)		
		{
			ml_undncylinder_Time[nStacker3][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff3 == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_undncylinder_Time[nStacker3][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_updncylinder_flag[nStacker3] = FALSE;
			}
		}
		else if (ml_undncylinder_Time[nStacker3][2] > st_wait.i_stacker_f_b_w[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_updncylinder_flag[nStacker3]= FALSE;
		}
	}
	else
	{
		ml_undncylinder_Time[nStacker3][2] = ml_undncylinder_Time[nStacker3][1] - ml_undncylinder_Time[nStacker3][0];
		
		if (ml_undncylinder_Time[nStacker3][2] < 0)		
		{
			ml_undncylinder_Time[nStacker3][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_undncylinder_Time[nStacker3][2] > st_wait.i_stacker_f_b_l)
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_updncylinder_flag[nStacker3] = FALSE;
		}
	}
	
	return nFuncRet;

}

///////////////////////////

/////////////////////////// 20120721
int CDialog_Part_Manual::StackerMovingExcution4()//SD체크
{
	int n_move_flag = CTLBD_RET_PROCEED;
	int nRet_1;

	mn_MotorNum4 = m_n_motor_no4;
	mn_StackerPos4 = U_STACKER_1;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// =============================================================================
	if (m_b_stop_req)
	{
		m_n_move_stacker_step4 = 3000;
	}
	// =============================================================================

	switch(m_n_move_stacker_step4)
	{
		case 0 :
			if(mn_MotorNum4 == M_ULDM_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_tray_chk, IO_ON);
			if(nRet_1 == IO_OFF)
			{				
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{				
					//000101 0 00 "Stacker1에 트레이가 존재하지 않습니다."
					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Does not exist on the tray Stacker1.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_move_stacker_step4 = 30;
			}
			break;

		case 20:
			if(mn_MotorNum4 == M_ULDM_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 있습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker1.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_move_stacker_step4 = 30;
			}
			break;

		case 30:
			nRet_1 = Set_RailFwdbwdCylinderOnOff4(mn_StackerPos4, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step4 = 100;
			}
			break;

		case 100:
			nRet_1 = Get_RailFwdbwdCylinderOnOff4(mn_StackerPos4, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_n_move_stacker_step4 = 200;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 200:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF);
			
			if (nRet_1 == IO_OFF)
			{
// 				FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_clamp_onoff, IO_ON);
// 				FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_unclamp_onoff, IO_OFF);
				m_n_move_stacker_step4 = 300;
			}
			else// if (nRet == IO_OFF)
			{
//				m_n_move_stacker_step4 = 2000;
				sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 있습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker1.");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;
		
		case 300:
			nRet_1 = CTL_Lib.Single_Move(mn_StackerPos4, st_motor[mn_StackerPos4].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step4 = 900;
			}
			else if(nRet_1 == CTLBD_RET_ERROR)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 900:
			Set_StackerUpDnClinder(mn_StackerPos4, CYLINDER_ON);
			m_n_move_stacker_step4 = 910;
			break;
		case 910:
			nRet_1 = Get_StackerUpDnClinder(mn_StackerPos4, CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				mn_stacker_updn_cyliner[mn_StackerPos4] = CYLINDER_ON;//항상 UP
				m_n_move_stacker_step4 = 1000;
			}
			else if(nRet_1 == CYLINDER_ERROR)
			{
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 Up/Down Error.");
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					n_move_flag = CTLBD_RET_ERROR;
				
				}
				else
				{
					m_n_move_stacker_step4 = 900;
				}
			}
			break;


		case 1000:
			nRet_1 = OnULDStackerReadyPos(mn_StackerPos4,1); //SD 대기
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step4 = 1100;
			}
			else if(nRet_1 == CTL_FULL)
			{
				//000302 1 01 "Loader Stocker 1 Tray Full 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "ULD Stocker 1 Tray Full error.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				n_move_flag = CTLBD_RET_ERROR;
			}
			else if(nRet_1 == CTLBD_RET_SKIP) //스태커에 트레이가 없을때 
			{//000301 0 00 "Loader Stocker 1에 트레이 없음 에러"
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "ULD Stocker 1에 트레이 없음 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "No tray error on ULD Stocker 1.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;


		case 1100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000100 0 00 "Stacker1에 트레이가 존재합니다.제거해 주세요."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재합니다.제거해 주세요.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 the tray is present., Please remove.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_move_stacker_step4 = 1300;
			}
			break;

		case 1300:
			if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_fwd_chk + (mn_StackerPos4), IO_OFF) == IO_OFF) && 
				(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_bwd_chk + (mn_StackerPos4), IO_ON) == IO_ON))	// Off 상태.
			{
				m_n_move_stacker_step4 = 1500;
			}
			else
			{
				nRet_1 = Set_RailFwdbwdCylinderOnOff4(mn_StackerPos4, CYLINDER_OFF);
				if(nRet_1 == CYLINDER_ON)
				{
					m_n_move_stacker_step4 = 1400;
				}
			}
			break;

		case 1310:
			nRet_1 = Set_RailFwdbwdCylinderOnOff4(mn_StackerPos4, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step4 = 1400;
			}
			break;

			
		case 1400:
			nRet_1 = Get_RailFwdbwdCylinderOnOff4(mn_StackerPos4, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_n_move_stacker_step4 = 1500;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if(mn_StackerPos4 == 0)
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not backward.");
					}

				}
				else 
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward.");
					}

				}
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
				m_n_move_stacker_step4 = 1310;
			}
			break;

		case 1500:
			nRet_1 = OnULDStackerReadyPos(mn_StackerPos4, 0); //트레이 하나 Up한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step4 = 1600;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_n_move_stacker_step4 = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 1600:
			nRet_1 = COMI.Get_MotIOSensor(mn_MotorNum4, MOT_SENS_SD);
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
	//			st_sync.mn_ld_module_tray_supply[mn_StackerPos] = CTL_FREE;		// 교체 끝났다고 설정.
//				m_n_move_stacker_step4 = 300;	
				m_n_move_stacker_step4 = 1700;	
			}
			/////////////// 20120719
			else if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_ON) == IO_ON && nRet_1 == CTLBD_RET_ERROR)
			{
				m_n_move_stacker_step4 = 1700;
			}
    		///////////////

			else
			{
				m_n_move_stacker_step4 = 1100;
			}
			break;	

		case 1700:
			nRet_1 = Set_RailFwdbwdCylinderOnOff4(0, CYLINDER_ON);
			if(nRet_1 == CTL_GOOD)
			{
				m_n_move_stacker_step4 = 1800;
			}
			break;

		case 1800:
			nRet_1 = Get_RailFwdbwdCylinderOnOff4(0, CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_move_stacker_step4 = 2000;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{	
				//000203 0 00 "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."			
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 forward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not forward.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;	
				m_n_move_stacker_step4 = 1700;
			}
			break;


		case 2000:
			nRet_1 = OnULDStackerReadyPos(mn_StackerPos4, 2); //트레 하나 Dn한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_move_stacker_step4 = 2100;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_n_move_stacker_step4 = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 2100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_ON);
			
			if (nRet_1 == IO_ON)	// 트레이가 있다
			{
				m_n_move_stacker_step4 = 3000;
			}
			else
			{
				//000201 0 00 "Stacker1 레일에 트레이가 존재하지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail does not exist in the tray.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_n_move_stacker_step4 = 2100;
			}
			break;

		case 3000:
			m_n_move_stacker_step4 = 0;
			n_move_flag = CTLBD_RET_GOOD;
			break;
	}


	return  n_move_flag;
}

int CDialog_Part_Manual::OnULDStackerReadyPos(int n_StackerPos4, int nMode4)
{
	int nRet_1 = VARIABLE_INIT;
	int nRet_2 = VARIABLE_INIT;
	int nFuncRet = CTLBD_RET_PROCEED;
	double dNowPos4;
	int nMotorNum4;


	if(n_StackerPos4 == U_STACKER_1)
	{
		nMotorNum4 = M_ULDM_STACKER_1;
	}
	else
	{
		nMotorNum4 = M_ULDM_STACKER_2;
	}

	if(m_n_motor_no4 == M_ULDM_STACKER_1) nMotorNum4 = M_ULDM_STACKER_1;

	if(FAS_IO.get_in_bit(st_io.i_stacker_light_curtain_chk, IO_ON) == IO_ON )
	{
		COMI.Set_MotStop(0, nMotorNum4);
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Error] Area sensor is detected. Motor stop. ReWork Please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		CTL_Lib.mn_single_motmove_step[nMotorNum4] = 0;
		return nFuncRet;
	}

//	if(alarm.n_area_ready_alarm[0] == TRUE)
//	{
//		COMI.Set_MotStop(0, nMotorNum4);
//		CTL_Lib.mn_single_motmove_step[nMotorNum4] = 0;
//		return nFuncRet;
//	}

	switch(ReadyPosStep4)
	{
	case 0:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum4, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep4 = 1000;
		}
		else
		{
			ReadyPosStep4 = 100;
		}
		break;

	case 100:
		COMI.Set_Motor_IO_Property(nMotorNum4, cmSD_EN, cmTRUE);
		nRet_1 =  COMI.Start_SingleMove(nMotorNum4, st_motor[nMotorNum4].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep4 = 200;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		else if(nRet_1 == CTLBD_RET_SAFETY)	
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		break;

	case 200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum4, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, nMotorNum4);
			ReadyPosStep4 = 300;
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum4, st_motor[nMotorNum4].md_pos[P_STOCKER_UP]);
			if(nRet_1 == CTLBD_RET_GOOD)// 정상 동작을 했다면...
			{
				if (nMode4 == 0)
				{
					nRet_1 = COMI.Get_MotIOSensor(nMotorNum4, MOT_SENS_SD);
					nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF);
					if(nRet_1 == CTLBD_RET_GOOD || nRet_2 == IO_ON)
					{
						if(nRet_1 == CTLBD_RET_GOOD)
						{
							COMI.Set_Motor_IO_Property(nMotorNum4, cmSD_EN, cmFALSE);
							ReadyPosStep4 = 1000; //20120719
						}
						else
						{
							ReadyPosStep4 = 5000;
						}					
					}
					else
					{
						COMI.Set_Motor_IO_Property(nMotorNum4, cmSD_EN, cmFALSE);
						ReadyPosStep4 = 2000;//1000; //20120719
					}
				}
				else if (nMode4 == 2)// SD 감지후에 DOWN하는 모드
				{
					ReadyPosStep4 = 300;
				}
				else//SD 감지거나, UP_LIMIT까지만 동작하는 모드이다.
				{
					ReadyPosStep4 = 5000;
				}
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
			{
				ReadyPosStep4 = 100;
			}
		}
		break;

	case 300:
		if (nMode4 == 0)//한장 올린다
		{
			dNowPos4 = COMI.Get_MotCurrentPos(nMotorNum4);
			if(nMotorNum4 == M_ULDM_STACKER_2)
			{
				m_dTargetPos4 = dNowPos4 + st_motor[nMotorNum4].md_pos[P_STOCKER_UP_OFFSET] - (st_motor[nMotorNum4].mn_allow * 30);
			}
			else
			{
				m_dTargetPos4 = dNowPos4 + st_motor[nMotorNum4].md_pos[P_STOCKER_UP_OFFSET];
			}
			if(m_dTargetPos4 > st_motor[nMotorNum4].md_pos[P_STOCKER_P_LIMIT])
			{
				m_dTargetPos4 = st_motor[nMotorNum4].md_pos[P_STOCKER_P_LIMIT];
				m_b_p_limit_check4 = TRUE;//20120719
			}
			ReadyPosStep4 = 400;
		}
		else if (nMode4 == 2)//한장만큼 내린다.
		{
			dNowPos4 = COMI.Get_MotCurrentPos(nMotorNum4);			
			m_dTargetPos4 = dNowPos4 - st_motor[nMotorNum4].md_pos[P_STOCKER_DN_OFFSET];
			
			if (m_dTargetPos4 < st_motor[nMotorNum4].md_pos[P_STOCKER_DOWN])		m_dTargetPos4 = st_motor[nMotorNum4].md_pos[P_STOCKER_DOWN];
			
			ReadyPosStep4 = 400;
		}
		else//SD감지 대기한다
		{
			COMI.Set_Motor_IO_Property(nMotorNum4, cmSD_EN, cmFALSE);
//			ReadyPosStep4 = 5000;
			ReadyPosStep4 = 1000;
		}
		break;

	case 400:
		nRet_2 = COMI.Get_MotIOSensor(nMotorNum4, MOT_SENS_SD);
		COMI.Set_Motor_IO_Property(nMotorNum4, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum4, m_dTargetPos4, st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			if(nMode4 == 0)//UP
			{
			//////////////////////////////////// 20120719
				if (m_b_p_limit_check4 == TRUE)
				{
					m_b_p_limit_check4 = FALSE;
					ReadyPosStep4 = 2000;
				}
			////////////////////////////////////

				else if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_ON) == IO_ON)
				{
					ReadyPosStep4 = 5000;
				}
			//////////////////////////////////// 20120719
				else if((FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF) == IO_OFF) && nRet_2 == CTLBD_RET_GOOD)
				{
					ReadyPosStep4 = 5000;
				}
			////////////////////////////////////
				else
				{
					ReadyPosStep4 = 2000;
				}
			}
			else
			{
				ReadyPosStep4 = 5000;
			}

		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep4 = 400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			ReadyPosStep4 = 400;
		}
		break;

	case 1000:
		COMI.Set_Motor_IO_Property(nMotorNum4, cmSD_EN, cmFALSE);
		nRet_1 = COMI.Start_SingleMove(nMotorNum4, st_motor[nMotorNum4].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ReadyPosStep4 = 1100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep4 = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		break;

	case 1100:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum4, MOT_SENS_SD);
		if(nRet_1 != BD_GOOD)
		{
			COMI.Set_MotStop(0, nMotorNum4);
			
			if (nMode4 == 1)//대기
			{
				ReadyPosStep4 = 5000;
			}
			else
			{
				ReadyPosStep4 = 100;											// SD센서에 감지가 되지 않으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
			}
			
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum4, st_motor[nMotorNum4].md_pos[P_STOCKER_DOWN]);
			if (nRet_1 == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
			{
				ReadyPosStep4 = 1200;
//				ReadyPosStep4 = 2000;
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
//				ReadyPosStep4 = 1000;
			}
// 			else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
// 			{
// 				ReadyPosStep4 = 5000;
// 			}
		}
		break;

	case 1200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum4, MOT_SENS_SD); 
		if(nRet_1 == CTLBD_RET_GOOD) //이미 감지하고 있으면 트레이가 FULL까지 있다는 이야기임 
		{
			if(nMotorNum4 == M_ULDM_STACKER_2)
			{
				if(FAS_IO.get_in_bit(st_io.i_uld_stacker2_up_chk + (n_StackerPos4), IO_OFF) == IO_ON ||
					FAS_IO.get_in_bit(st_io.i_uld_stacker2_dn_chk + (n_StackerPos4), IO_ON) == IO_OFF)
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_uld_stacker1_up_onoff + (n_StackerPos4), IO_ON);
					FAS_IO.OnCylinderAction(st_io.o_uld_stacker1_dn_onoff + (n_StackerPos4), IO_OFF);
					ReadyPosStep4 = 1300;
				}
				else
				{
					nFuncRet = CTL_FULL;
					ReadyPosStep4 = 0;
				}
			}
			else
			{
				nFuncRet = CTL_FULL;
				ReadyPosStep4 = 0;		
			}
		}
		else
		{//Full sensor는 감지되지 않았으니 다시확인한다 
			ReadyPosStep4 = 1210;
			ReadyPosStep4 = 2010;
		}
		break;

	case 1210:
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker2_up_onoff + (n_StackerPos4), IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker2_dn_onoff + (n_StackerPos4), IO_OFF);
		ReadyPosStep4 = 1300;
		break;

	case 1300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_stacker2_up_onoff + (n_StackerPos4), st_io.i_uld_stacker1_up_chk + (n_StackerPos4),
			                                  st_io.i_uld_stacker2_dn_chk + (n_StackerPos4), IO_ON, st_wait.i_stacker_updn_w[0], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD)
		{
			ReadyPosStep4 = 100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)				
			{
				mn_retry = 0;
				//010206 0 01 "Stacker2 레일 Down 센서 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "stacker2 레일 Down 센서 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Down stacker2 rail sensor error.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				ReadyPosStep4 = 1210;
			}
			else
			{
				ReadyPosStep4 = 1210;
			}
		}
		break;		


	case 2000://tray는 있으나, UP Pos까지 갔는데 SD가 감지 않된다, stacker를 Up한 후 SD센서를 본다
		if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_updn_pos_chk + (n_StackerPos4), IO_OFF) == IO_OFF)
		{//실린더 up한다	
			 if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk + (n_StackerPos4), IO_ON) == IO_ON) && 
				(FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk + (n_StackerPos4), IO_OFF) == IO_OFF))
			 {
				ReadyPosStep4 = 2300;
			 }
			 else
			 {
				ReadyPosStep4 = 2100;
			 }			
		}
		else
		{
//			if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk + (n_StackerPos4 * 4), IO_ON) == IO_ON) && 
//			 (FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk + (n_StackerPos4 * 4), IO_OFF) == IO_OFF))
//			{
//				if(nMode4 == 0)
//				{
//					ReadyPosStep4 = 5000;
//				}
//				else
//				{
//					ReadyPosStep4 = 2010;
//				}
//			}
//			else
//			{
				ReadyPosStep4 = 2010;
//			}	

		}
		break;

	case 2010:
		COMI.Set_Motor_IO_Property(nMotorNum4, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum4, st_motor[nMotorNum4].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep4 = 2020;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep4 = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s",COMI.mc_alarmcode);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			ReadyPosStep4 = 2000;
		}		
		break;

	case 2020:
// 		if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_updn_pos_chk + (n_StackerPos4), IO_OFF) == IO_OFF)
// 		{//실린더 up한다	
// 			 if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk + (n_StackerPos4), IO_ON) == IO_ON) && 
// 				(FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk + (n_StackerPos4), IO_OFF) == IO_OFF))
// 			 {
// 				ReadyPosStep4 = 2300;
// 			 }
// 			 else
// 			 {
// 				ReadyPosStep4 = 2100;
// 			 }			
// 		}
// 		else
// 		{
// 			ReadyPosStep4 = 2300;
// 		}
		if(nMotorNum4 == M_ULDM_STACKER_2)
		{
			ReadyPosStep4 = 2600;
		}
		else
		{
			ReadyPosStep4 = 2400;		
		}
		break;

	case 2100:
		nRet_1 = Set_StackerUpDnClinder4(n_StackerPos4, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep4 = 2200;
		}
		break;

	case 2200:
		nRet_1 = Get_StackerUpDnClinder4(n_StackerPos4, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			ReadyPosStep4 = 100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000205 0 00 "Stacker1 레일 Up 센서 에러."
			if(n_StackerPos4 == 0)			sprintf(st_msg.c_abnormal_msg, "Stacker1 레일 Up 센서 에러.");
			else                            sprintf(st_msg.c_abnormal_msg, "Stacker2 레일 Up 센서 에러.");

			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				if(n_StackerPos4 == 0)			sprintf(st_msg.c_abnormal_msg, "Up Stacker1 rail sensor error.");
				else                            sprintf(st_msg.c_abnormal_msg, "Up Stacker2 rail sensor error.");
			}

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{				
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
			ReadyPosStep4 = 2100;
		}
		break;

	case 2300:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_tray_chk + (n_StackerPos4), IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			ReadyPosStep4 = 2400;
		}
		else// if(nRet_1 == IO_OFF)
		{
			ReadyPosStep4 = 100;
		}
		break;

	case 2400:
		nRet_1 = Set_StackerUpDnClinder4(n_StackerPos4, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep4 = 2500;
		}
		break;

	case 2500:
		nRet_1 = Get_StackerUpDnClinder4(n_StackerPos4, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			ReadyPosStep4 = 2510;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000206 0 00 "Stacker1 레일 애주 센서 에러."
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "Stacker1 레일 센서 에러.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 rail sensor error.");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
			ReadyPosStep4 = 2400;
		}
		break;


	case 2510:
		if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_tray_chk, IO_ON) == IO_ON) ReadyPosStep4 = 2520;
		else                                                                 ReadyPosStep4 = 2600;
		break;

	case 2520:
		nRet_1 = Set_StackerUpDnClinder4(n_StackerPos4, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep4 = 2530;
		}
		break;

	case 2530:
		nRet_1 = Get_StackerUpDnClinder4(n_StackerPos4, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			ReadyPosStep4 = 100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000205 0 00 "Stacker1 레일 Up 센서 에러."
			if(n_StackerPos4 == 0)			sprintf(st_msg.c_abnormal_msg, "Stacker1 레일 Up 센서 에러.");
			else                            sprintf(st_msg.c_abnormal_msg, "Stacker2 레일 Up 센서 에러.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				if(n_StackerPos4 == 0)			sprintf(st_msg.c_abnormal_msg, "Up Stacker1 rail sensor error.");
				else                            sprintf(st_msg.c_abnormal_msg, "Up Stacker2 rail sensor error.");
			}

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{				
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			nFuncRet = CTLBD_RET_ERROR;
			ReadyPosStep4 = 2400;
		}
		break;		
		

	case 2600:
		nFuncRet = CTLBD_RET_SKIP;
		ReadyPosStep4 = 0;
		break;

	case 5000:
		nFuncRet = CTLBD_RET_GOOD;
		ReadyPosStep4 = 0;
		break;	
	}

	return nFuncRet;

}

int CDialog_Part_Manual::Set_RailFwdbwdCylinderOnOff4(int nStacker4, int n_onoff4)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff4 == CYLINDER_OFF)
	{
	}

	FAS_IO.set_out_bit(st_io.o_uld_stacker1_rail_fwd_onoff + (nStacker4*2), n_onoff4);
	FAS_IO.set_out_bit(st_io.o_uld_stacker1_rail_bwd_onoff + (nStacker4*2), !n_onoff4);
	mb_pushercylinder_flag[nStacker4] = FALSE;
	ml_pushercylinder_Time[nStacker4][0] = GetCurrentTime();
	return nFuncRet;
	
}
int CDialog_Part_Manual::Get_RailFwdbwdCylinderOnOff4(int nStacker4, int n_onoff4)
{
	int nFuncRet = -1;
	
	ml_pushercylinder_Time[nStacker4][1] = GetCurrentTime();
	
	if (mb_pushercylinder_flag[nStacker4] != TRUE)
	{
		ml_pushercylinder_Time[nStacker4][0] = GetCurrentTime();
		mb_pushercylinder_flag[nStacker4] = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_fwd_chk + (nStacker4*3), IO_ON) == IO_ON) && 
		(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_bwd_chk + (nStacker4*3), IO_OFF) == IO_OFF))	// On 상태.
	{
		ml_pushercylinder_Time[nStacker4][2] = ml_pushercylinder_Time[nStacker4][1] - ml_pushercylinder_Time[nStacker4][0];
		
		if (ml_pushercylinder_Time[nStacker4][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker4][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff4 == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_pushercylinder_Time[nStacker4][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker4] = FALSE;
			}
		}
		else if (ml_pushercylinder_Time[nStacker4][2] > st_wait.i_stacker_f_b_w[1])
		{
			nFuncRet = CYLINDER_ON;
			mb_pushercylinder_flag[nStacker4] = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_fwd_chk + (nStacker4*3), IO_OFF) == IO_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_bwd_chk + (nStacker4*3), IO_ON) == IO_ON))	// Off 상태.
	{
		ml_pushercylinder_Time[nStacker4][2] = ml_pushercylinder_Time[nStacker4][1] - ml_pushercylinder_Time[nStacker4][0];
		
		if (ml_pushercylinder_Time[nStacker4][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker4][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff4 == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_pushercylinder_Time[nStacker4][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker4] = FALSE;
			}
		}
		else if (ml_pushercylinder_Time[nStacker4][2] > st_wait.i_stacker_f_b_w[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_pushercylinder_flag[nStacker4] = FALSE;
		}
	}
	else
	{
		ml_pushercylinder_Time[nStacker4][2] = ml_pushercylinder_Time[nStacker4][1] - ml_pushercylinder_Time[nStacker4][0];
		
		if (ml_pushercylinder_Time[nStacker4][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker4][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_pushercylinder_Time[nStacker4][2] > st_wait.i_stacker_f_b_l)
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_pushercylinder_flag[nStacker4] = FALSE;
		}
	}
	
	return nFuncRet;
}

int CDialog_Part_Manual::Set_StackerUpDnClinder4(int nStacker4, int n_onoff4)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff4 == CYLINDER_OFF)
	{
		if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_tray_chk + (nStacker4 * 4), IO_OFF) == IO_ON)
		{
			sprintf(st_msg.c_abnormal_msg, "ULD Sink#l stacker에 트레이가 존재합니다.제거해 주세요.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "ULD Sink # l stacker tray to exist, Please remove.");
			}

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			nFuncRet = CYLINDER_ERROR;
		}
	}
	FAS_IO.set_out_bit(st_io.o_uld_stacker1_up_onoff + (nStacker4 * 2), n_onoff4);
	FAS_IO.set_out_bit(st_io.o_uld_stacker1_dn_onoff + (nStacker4 * 2), !n_onoff4);
	mb_updncylinder_flag[nStacker4] = FALSE;
	ml_undncylinder_Time[nStacker4][0] = GetCurrentTime();

	return nFuncRet;
}

int CDialog_Part_Manual::Get_StackerUpDnClinder4(int nStacker4, int n_onoff4)
{
	int nFuncRet = -1;
	
	ml_undncylinder_Time[nStacker4][1] = GetCurrentTime();
	
	if (mb_updncylinder_flag[nStacker4] != TRUE)
	{
		ml_undncylinder_Time[nStacker4][0] = GetCurrentTime();
		mb_updncylinder_flag[nStacker4] = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk + (nStacker4 * 4), IO_ON) == CTL_ON) && 
		(FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk + (nStacker4 * 4), IO_OFF) == CTL_OFF))	// On 상태.
	{
		ml_undncylinder_Time[nStacker4][2] = ml_undncylinder_Time[nStacker4][1] - ml_undncylinder_Time[nStacker4][0];
		
		if (ml_undncylinder_Time[nStacker4][2] < 0)		
		{
			ml_undncylinder_Time[nStacker4][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff4 == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_undncylinder_Time[nStacker4][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_updncylinder_flag[nStacker4] = FALSE;
			}
		}
		else if (ml_undncylinder_Time[nStacker4][2] > st_wait.i_stacker_f_b_w[1])
		{
			nFuncRet = CYLINDER_ON;
			mb_updncylinder_flag[nStacker4] = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk + (nStacker4 * 4), IO_OFF) == CTL_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk + (nStacker4 * 4), IO_ON) == CTL_ON))	// Off 상태.
	{
		ml_undncylinder_Time[nStacker4][2] = ml_undncylinder_Time[nStacker4][1] - ml_undncylinder_Time[nStacker4][0];
		
		if (ml_undncylinder_Time[nStacker4][2] < 0)		
		{
			ml_undncylinder_Time[nStacker4][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff4 == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_undncylinder_Time[nStacker4][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_updncylinder_flag[nStacker4] = FALSE;
			}
		}
		else if (ml_undncylinder_Time[nStacker4][2] > st_wait.i_stacker_f_b_w[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_updncylinder_flag[nStacker4]= FALSE;
		}
	}
	else
	{
		ml_undncylinder_Time[nStacker4][2] = ml_undncylinder_Time[nStacker4][1] - ml_undncylinder_Time[nStacker4][0];
		
		if (ml_undncylinder_Time[nStacker4][2] < 0)		
		{
			ml_undncylinder_Time[nStacker4][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_undncylinder_Time[nStacker4][2] > st_wait.i_stacker_f_b_l)
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_updncylinder_flag[nStacker4] = FALSE;
		}
	}
	
	return nFuncRet;

}

/////////////////////////// 20120724
int CDialog_Part_Manual::Set_M_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff == CYLINDER_OFF)
	{
	}

	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker1_rail_fwd_onoff + (nStacker*2), n_onoff);
	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker1_rail_bwd_onoff + (nStacker*2), !n_onoff);
	mb_pushercylinder_flag[nStacker] = FALSE;
	ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();

	return nFuncRet;
}

int CDialog_Part_Manual::Get_M_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff)
{
	int nFuncRet = -1;
	
	ml_pushercylinder_Time[nStacker][1] = GetCurrentTime();
	
	if (mb_pushercylinder_flag[nStacker] != TRUE)
	{
		ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
		mb_pushercylinder_flag[nStacker] = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_fwd_chk + (nStacker*3), IO_ON) == IO_ON) && 
		(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_bwd_chk + (nStacker*3), IO_OFF) == IO_OFF))	// On 상태.
	{
		ml_pushercylinder_Time[nStacker][2] = ml_pushercylinder_Time[nStacker][1] - ml_pushercylinder_Time[nStacker][0];
		
		if (ml_pushercylinder_Time[nStacker][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_pushercylinder_Time[nStacker][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker] = FALSE;
			}
		}
		else if (ml_pushercylinder_Time[nStacker][2] > st_wait.i_stacker_f_b_w[1])
		{
			nFuncRet = CYLINDER_ON;
			mb_pushercylinder_flag[nStacker] = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_fwd_chk + (nStacker*3), IO_OFF) == IO_OFF) &&
		(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_bwd_chk + (nStacker*3), IO_ON) == IO_ON))	// Off 상태.
	{
		ml_pushercylinder_Time[nStacker][2] = ml_pushercylinder_Time[nStacker][1] - ml_pushercylinder_Time[nStacker][0];
		
		if (ml_pushercylinder_Time[nStacker][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_pushercylinder_Time[nStacker][2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker] = FALSE;
			}
		}
		else if (ml_pushercylinder_Time[nStacker][2] > st_wait.i_stacker_f_b_w[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_pushercylinder_flag[nStacker] = FALSE;
		}
	}
	else
	{
		ml_pushercylinder_Time[nStacker][2] = ml_pushercylinder_Time[nStacker][1] - ml_pushercylinder_Time[nStacker][0];
		
		if (ml_pushercylinder_Time[nStacker][2] < 0)		
		{
			ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_pushercylinder_Time[nStacker][2] > st_wait.i_stacker_f_b_l)
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_pushercylinder_flag[nStacker] = FALSE;
		}
	}
	
	return nFuncRet;
}
int CDialog_Part_Manual::Set_M_RailClampOnOff(int n_onoff)
{
	int nFuncRet = CTL_GOOD;

	FAS_IO.set_out_bit(st_io.o_hs_bwd_clamp_onoff, n_onoff);
	mb_m_clampcylinder_flag = FALSE;
	ml_m_clampcylinder_Time[0] = GetCurrentTime();

	return nFuncRet;	
}

int CDialog_Part_Manual::Get_M_RailClampOnOff(int n_onoff)
{
	int nFuncRet = -1;
	
	ml_m_clampcylinder_Time[1] = GetCurrentTime();
	
	if (mb_m_clampcylinder_flag != TRUE)
	{
		ml_m_clampcylinder_Time[0] = GetCurrentTime();
		mb_m_clampcylinder_flag = TRUE;
	}
	else if (FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_off_chk, IO_OFF) == IO_OFF && 
		FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_on_chk, IO_ON) == IO_ON)	// On 상태.
	{
		ml_m_clampcylinder_Time[2] = ml_m_clampcylinder_Time[1] - ml_m_clampcylinder_Time[0];
		
		if (ml_m_clampcylinder_Time[2] < 0)		
		{
			ml_m_clampcylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_m_clampcylinder_Time[2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_m_clampcylinder_flag = FALSE;
			}
		}
		else if (ml_m_clampcylinder_Time[2] > st_wait.i_stacker_f_b_w[1])
		{
			nFuncRet = CYLINDER_ON;
			mb_m_clampcylinder_flag = FALSE;
		}
	}
	else if (FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_off_chk, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_on_chk, IO_OFF) == IO_OFF)	// Off 상태.
	{
		ml_m_clampcylinder_Time[2] = ml_m_clampcylinder_Time[1] - ml_m_clampcylinder_Time[0];
		
		if (ml_m_clampcylinder_Time[2] < 0)		
		{
			ml_m_clampcylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_m_clampcylinder_Time[2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_m_clampcylinder_flag = FALSE;
			}
		}
		else if (ml_m_clampcylinder_Time[2] > st_wait.i_stacker_f_b_w[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_m_clampcylinder_flag = FALSE;
		}
	}
	else
	{
		ml_m_clampcylinder_Time[2] = ml_m_clampcylinder_Time[1] - ml_m_clampcylinder_Time[0];
		
		if (ml_m_clampcylinder_Time[2] < 0)		
		{
			ml_m_clampcylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_m_clampcylinder_Time[2] > st_wait.i_stacker_f_b_l)
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_m_clampcylinder_flag = FALSE;
		}
	}
	
	return nFuncRet;
}
int CDialog_Part_Manual::Set_M_TrayLatchClinder(int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker2_tray_clamp_onoff, n_onoff);
	mb_m_latchcylinder_flag = FALSE;
	ml_m_latchcylinder_Time[0] = GetCurrentTime();

	return nFuncRet;
	
}
int CDialog_Part_Manual::Get_M_TrayLatchClinder(int n_onoff)
{
	int nFuncRet = -1;
	
	ml_m_latchcylinder_Time[1] = GetCurrentTime();
	
	if (mb_m_latchcylinder_flag != TRUE)
	{
		ml_m_latchcylinder_Time[0] = GetCurrentTime();
		mb_m_latchcylinder_flag = TRUE;
	}
	else if (FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_ON) == CTL_ON && 
		FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_off_chk, IO_OFF) == CTL_OFF)	// On 상태.
	{
		ml_m_latchcylinder_Time[2] = ml_m_latchcylinder_Time[1] - ml_m_latchcylinder_Time[0];
		
		if (ml_m_latchcylinder_Time[2] < 0)		
		{
			ml_m_latchcylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_m_latchcylinder_Time[2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_m_latchcylinder_flag = FALSE;
			}
		}
		else if (ml_m_latchcylinder_Time[2] > st_wait.i_stacker_f_b_w[1])
		{
			nFuncRet = CYLINDER_ON;
			mb_m_latchcylinder_flag = FALSE;
		}
	}
	else if (FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_OFF) == CTL_OFF && 
		FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_off_chk, IO_ON) == CTL_ON)	// Off 상태.
	{
		ml_m_latchcylinder_Time[2] = ml_m_latchcylinder_Time[1] - ml_m_latchcylinder_Time[0];
		
		if (ml_m_latchcylinder_Time[2] < 0)		
		{
			ml_m_latchcylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_m_latchcylinder_Time[2] > st_wait.i_stacker_f_b_l)
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_m_latchcylinder_flag = FALSE;
			}
		}
		else if (ml_m_latchcylinder_Time[2] > st_wait.i_stacker_f_b_w[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_m_latchcylinder_flag= FALSE;
		}
	}
	else
	{
		ml_m_latchcylinder_Time[2] = ml_m_latchcylinder_Time[1] - ml_m_latchcylinder_Time[0];
		
		if (ml_m_latchcylinder_Time[2] < 0)		
		{
			ml_m_latchcylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_m_latchcylinder_Time[2] > st_wait.i_stacker_f_b_l)
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_m_latchcylinder_flag = FALSE;
		}
	}
	
	return nFuncRet;
}
void CDialog_Part_Manual::InitRightStacker()
{
	int nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	int ratio = 50;

	if(st_handler.mn_init_state[INIT_RIGHTHS_STACKER] != CTL_NO) return;


	if(FAS_IO.get_in_bit(st_io.i_stacker_light_curtain_chk, IO_ON) == IO_ON  || m_b_stop_req == TRUE)
	{
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg,"[Manual Error] Area sensor is detected. Motor stop. ReWork Please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		COMI.Set_MotStop(0, M_HS_B_STACKER_1);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_1] = 0;
		COMI.Set_MotStop(0, M_HS_B_STACKER_2);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_2] = 0;
		COMI.Set_MotStop(0, M_HS_B_STACKER_MOVE);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_MOVE] = 0;
		COMI.mn_home_step[M_HS_B_STACKER_1] = 0;
		COMI.mn_home_step[M_HS_B_STACKER_2] = 0;
		COMI.mn_home_step[M_HS_B_STACKER_MOVE] = 0;
		
		RightInitStep = 3000;
		return;
	}
//	if(alarm.n_area_ready_alarm[0] == TRUE || m_b_stop_req == TRUE)
//	{
//		COMI.Set_MotStop(0, M_HS_B_STACKER_1);
//		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_1] = 0;
//		COMI.Set_MotStop(0, M_HS_B_STACKER_2);
//		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_2] = 0;
//		COMI.Set_MotStop(0, M_HS_B_STACKER_MOVE);
//		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_MOVE] = 0;
//		COMI.mn_home_step[M_HS_B_STACKER_1] = 0;
//		COMI.mn_home_step[M_HS_B_STACKER_2] = 0;
//		COMI.mn_home_step[M_HS_B_STACKER_MOVE] = 0;
//		
//		RightInitStep = 3000;
//		return;
//	}

	switch(RightInitStep)
	{
	case 0:
		mn_retry = 0;
		RightInitStep = 5;
		break;

	case 5:
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		{//040100 0 04 "Heat Sink#2 stacker에 트레이가 존재합니다."
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Heat Sink#l stacker에 트레이가 존재합니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[Initialize Error] Heat Sink # l stacker is on the tray.");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			sprintf(mc_alarmcode,"040100");
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1400, st_work.mn_run_status, mc_alarmcode);
		}
		else if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF) == IO_ON)
		{//040101 0 04 "Heat Sink#2 stacker에 트레이가 존재지 않습니다."
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Heat Sink#2 stacker에 트레이가 존재합니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[Initialize Error] Heat Sink # 2 stacker is on the tray.");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			sprintf(mc_alarmcode,"040101");
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1401, st_work.mn_run_status, mc_alarmcode);
		}
		else
		{
			RightInitStep = 10;
		}
		break;		

	case 10:
		nRet_1 = Set_M_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			RightInitStep = 20;
		}
		break;

	case 20:
		nRet_1 = Get_M_TrayLatchClinder(CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			RightInitStep = 30;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//070204 0 07 "Right Heat Sink#2 Stacker 레일 backward가 동작이 되지 않습니다."
				sprintf(mc_alarmcode, "070204");
// 				alarm.mn_alarm_assign_section = 20; alarm.mn_count_mode = 0;
				st_work.mn_run_status = CTL_dWARNING;
 				CTL_Lib.Alarm_Error_Occurrence(1402, st_work.mn_run_status, mc_alarmcode);
				RightInitStep = 10;
			}
			else
			{
				RightInitStep = 10;
			}
		}
		break;

	case 30:
		nRet_1 = Set_M_RailClampOnOff(CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			RightInitStep = 35;
		}
		break;

	case 35:
		nRet_1 = Get_M_RailClampOnOff(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			RightInitStep = 40;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			Set_M_RailClampOnOff(CYLINDER_OFF);
//			060500 0 03 "Right Heat Sink#l Stacker 레일 Clamp Fwd가 동작이 되지 않습니다."
//			060501 0 03 "Right Heat Sink#l Stacker 레일 Clamp Bwd가 동작이 되지 않습니다."
			sprintf(mc_alarmcode,"060500");
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1491, st_work.mn_run_status, mc_alarmcode);
			RightInitStep = 30;
		}
		break;	

	case 40:
		nRet_1 = Set_M_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			RightInitStep = 45;
		}
		break;

	case 45:
		nRet_1 = Get_M_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			RightInitStep = 50;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{	
			//060203 0 06 "Right Heat Sink#l Stacker 레일 forward가 동작이 되지 않습니다."
			sprintf(mc_alarmcode,"070204");
 			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
// 			alarm.mn_alarm_assign_section = 21;
 			CTL_Lib.Alarm_Error_Occurrence(1403, st_work.mn_run_status, mc_alarmcode);
		}
		break;
		
	case 50:
		nRet_1 = Set_M_RailFwdbwdCylinderOnOff(1, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			RightInitStep = 60;
		}
		break;

	case 60:
		nRet_1 = Get_M_RailFwdbwdCylinderOnOff(1, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			RightInitStep = 1000;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{	
			//070204 0 07 "Right Heat Sink#2 Stacker 레일 backward가 동작이 되지 않습니다."
			sprintf(mc_alarmcode,"070204");
 			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
// 			alarm.mn_alarm_assign_section = 22;
 			CTL_Lib.Alarm_Error_Occurrence(1404, st_work.mn_run_status, mc_alarmcode);
		}
		break;
		
	case 1000:
		RightInitStep = 1100;
		break;
		
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		{//040100 0 04 "Heat Sink#2 stacker에 트레이가 존재합니다."
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Heat Sink#l stacker에 트레이가 존재합니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[Initialize Error] Heat Sink # l stacker is on the tray.");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			sprintf(mc_alarmcode,"040100");
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1405, st_work.mn_run_status, mc_alarmcode);
		}
		else if(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_rail_tray_chk, IO_OFF) == IO_ON)
		{//040101 0 04 "Heat Sink#2 stacker에 트레이가 존재지 않습니다."
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Heat Sink#2 stacker에 트레이가 존재합니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[Initialize Error] Heat Sink # 2 stacker is on the tray.");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			sprintf(mc_alarmcode,"040110");
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1406, st_work.mn_run_status, mc_alarmcode);
		}
		else
		{
			RightInitStep = 1100;
		}
		break;

	case 1100:
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_ON);
		RightInitStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_guide_clamp_onoff, st_io.i_hs_bwd_stacker_guide_unclamp_off_chk, st_io.i_hs_bwd_stacker_guide_clamp_on_chk,
			IO_ON, st_wait.i_stacker_guide_w[0], st_wait.i_stacker_guide_l);
		if(nRet_1 == RET_GOOD)
		{
			RightInitStep = 1300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//060102 0 06 "Right Heat Sink#l stacker이 guide clamp가 On이 되지 않습니다."
			sprintf(mc_alarmcode,"060102");
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1407, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 1300:
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_tray_pusher_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_tray_pusher_bwd_onoff, IO_OFF);
		RightInitStep = 1400;
		break;

	case 1400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_tray_pusher_bwd_onoff, st_io.i_hs_bwd_stacker_tray_pusher_fwd_chk, st_io.i_hs_bwd_stacker_tray_pusher_bwd_chk,
			IO_ON, st_wait.i_stacker_f_b_w[1], st_wait.i_stacker_f_b_l);
		nRet_1 = RET_GOOD;
		if(nRet_1 == RET_GOOD)
		{
			RightInitStep = 1500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//060105 0 06 "Right Heat Sink#l stacker 트레이 pusher backward가 동작이 되지 않습니다."
			sprintf(mc_alarmcode,"060105");
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1408, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 1500:
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker1_rail_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker1_rail_bwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker2_rail_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker2_rail_bwd_onoff, IO_ON);
		RightInitStep = 1600;
		break;

	case 1600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker1_rail_bwd_onoff, st_io.i_hs_bwd_stacker1_rail_bwd_chk, st_io.i_hs_bwd_stacker1_rail_fwd_chk,
			IO_ON, st_wait.i_stacker_rail_push_back_w[1], st_wait.i_stacker_rail_push_back_l);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_fwd_stacker2_rail_bwd_onoff, st_io.i_hs_bwd_stacker2_rail_bwd_chk, st_io.i_hs_bwd_stacker2_rail_fwd_chk,
			IO_ON, st_wait.i_stacker_rail_push_back_w[1], st_wait.i_stacker_rail_push_back_l);
		nRet_1 = RET_GOOD;
		nRet_2 = RET_GOOD;
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			RightInitStep = 1700;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//060106 0 06 "Right Heat Sink#l stacker 레일 forward가 동작이 되지 않습니다."
         //060107 0 06 "Right Heat Sink#l stacker 레일 backward가 동작이 되지 않습니다."
			if(nRet_1 != RET_PROCEED) sprintf(mc_alarmcode,"060107");
			else                      sprintf(mc_alarmcode,"070107");
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1409, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 1700:
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker1_up_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker1_dn_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker2_up_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker2_dn_onoff, IO_ON);
		RightInitStep = 1800;
		break;

	case 1800:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker1_dn_onoff, st_io.i_hs_bwd_stacker1_dn_chk, st_io.o_hs_bwd_stacker1_up_onoff,
			IO_ON, st_wait.i_stacker_updn_w[1], st_wait.i_stacker_updn_l);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker2_dn_onoff, st_io.i_hs_bwd_stacker2_dn_chk, st_io.o_hs_bwd_stacker2_up_onoff,
			IO_ON, st_wait.i_stacker_updn_w[1], st_wait.i_stacker_updn_l);
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			RightInitStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//060108 0 06 "Right Heat Sink#l stacker 업이 동작을 하지 않습니다."
		 //060109 0 06 "Right Heat Sink#l stacker 다운이 동작을 하지 않습니다."
			if(nRet_1 != RET_PROCEED) sprintf(mc_alarmcode,"060109");
			else                      sprintf(mc_alarmcode,"070109");
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1410, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 2000:
		nRet_1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_1, st_motor[M_HS_B_STACKER_1].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			RightInitStep = 2100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			RightInitStep = 2000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1411, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2100:
		nRet_1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_2, st_motor[M_HS_B_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			RightInitStep = 2200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			RightInitStep = 2100;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1412, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2200:
		nRet_1 = CTLBD_RET_GOOD;
		//nRet_1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_MOVE, st_motor[M_HS_B_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			RightInitStep = 2500;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			RightInitStep = 2200;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
// 			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			st_work.mn_run_status = CTL_dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1413, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2300:
		nRet_1 = CTLBD_RET_GOOD;
		//nRet_1 = CTL_Lib.Single_Move(M_HS_B_STACKER_MOVE, st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS], ratio);
		if(nRet_1 == CTLBD_RET_GOOD)//BD_GOOD)//20140213
		{
			RightInitStep = 2500;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)//BD_RETRY)//20140213
		{
			RightInitStep = 2300;
		}
		else if (nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_SAFETY)//BD_ERROR || nRet_1 == BD_SAFETY)//20140213
		{
 			st_work.mn_run_status = dWARNING;
 			CTL_Lib.Alarm_Error_Occurrence(1414, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2500:
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_ON);
		RightInitStep = 2600;
		break;

	case 2600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, st_io.i_hs_bwd_stacker_guide_unclamp_off_chk, st_io.i_hs_bwd_stacker_guide_clamp_on_chk,
			IO_ON, st_wait.i_stacker_guide_w[0], st_wait.i_stacker_guide_l);
		if(nRet_1 == RET_GOOD)
		{
			RightInitStep = 3000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//060103 0 06 "Right Heat Sink#l stacker이 guide clamp가 Off가 되지 않습니다."
			sprintf(mc_alarmcode,"060103");
// 			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
 			CTL_Lib.Alarm_Error_Occurrence(1415,  st_work.mn_run_status, mc_alarmcode);			
		}
		break;
		
	case 3000:
		st_handler.mn_init_state[INIT_RIGHTHS_STACKER] = CTL_YES;
		RightInitStep = 0;
		break;		
		
	}
		
}
int CDialog_Part_Manual::Stacker1Move()
{
	int n_move_flag = CTLBD_RET_PROCEED;
	int nRet_1;

	m_MotorNum = m_motor_no;
	m_StackerPos = HSR_STACKER_1;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// =============================================================================
	if (m_b_stop_req)
	{
		m_move_stacker_step1 = 3000;
	}
	// =============================================================================

	switch(m_move_stacker_step1)
	{
		case 0 :
			if(m_MotorNum == M_HS_B_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_tray_chk, IO_ON);
			if(nRet_1 == IO_OFF)
			{				
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{				
					//000101 0 00 "Stacker1에 트레이가 존재하지 않습니다."
					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Does not exist on the tray Stacker1.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
//				n_move_flag = CTLBD_RET_ERROR;
				m_move_stacker_step1 = 20;
			}
			else
			{
				m_move_stacker_step1 = 20;
			}
			break;

		case 20:
			if(m_MotorNum == M_HS_B_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 있습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker1.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_move_stacker_step1 = 30;
			}
			break;

		case 30:
			nRet_1 = Set_M_RailFwdbwdCylinderOnOff(m_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step1 = 100;
			}
			break;

		case 100:
			nRet_1 = Get_M_RailFwdbwdCylinderOnOff(m_StackerPos, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_move_stacker_step1 = 200;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 200:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF);
			
			if (nRet_1 == IO_OFF)
			{
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_ON);
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_OFF);
				m_move_stacker_step1 = 300;
			}
			else// if (nRet == IO_OFF)
			{
				m_move_stacker_step1 = 2000;
			}
			break;
		
		case 210:
			FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_OFF);
			m_move_stacker_step1 = 300;
			break;

		case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_guide_clamp_onoff, st_io.i_hs_bwd_stacker_guide_clamp_on_chk, st_io.i_hs_bwd_stacker_guide_unclamp_off_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_ON);
				m_move_stacker_step1 = 400;
			}
//			else 
//			{
//				m_move_stacker_step1 = 210;
//			}
			break;

		case 310:
			FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_ON);
			m_move_stacker_step1 = 400;
			break;

		case 400:
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_guide_clamp_onoff, st_io.i_hs_bwd_stacker_guide_unclamp_off_chk, st_io.i_hs_bwd_stacker_guide_clamp_on_chk,
				IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				m_move_stacker_step1 = 1000;
			}
			else if(nRet_1 != RET_PROCEED)
			{
				m_move_stacker_step1 = 310;
			}
			break;


		case 1000:
			nRet_1 = OnRighthsStackerReadyPos(m_StackerPos,1); //SD 대기
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_move_stacker_step1 = 1100;
			}
			else if(nRet_1 == CTL_FULL)
			{
				//000302 1 01 "Loader Stocker 1 Tray Full 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "HS Right Stocker 1 Tray Full error.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				n_move_flag = CTLBD_RET_ERROR;
			}
			else if(nRet_1 == CTLBD_RET_SKIP) //스태커에 트레이가 없을때 
			{//000301 0 00 "Loader Stocker 1에 트레이 없음 에러"
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "HS Right Stocker 1에 트레이 없음 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "No tray error on HS Right Stocker 1.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;


		case 1100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000100 0 00 "Stacker1에 트레이가 존재합니다.제거해 주세요."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재합니다.제거해 주세요.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 the tray is present, Please remove.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_move_stacker_step1 = 1300;
			}
			break;

		case 1300:
			if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_fwd_chk + (m_StackerPos), IO_OFF) == IO_OFF) && 
				(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_bwd_chk + (m_StackerPos), IO_ON) == IO_ON))	// Off 상태.
			{
				m_move_stacker_step1 = 1500;
			}
			else
			{
				nRet_1 = Set_M_RailFwdbwdCylinderOnOff(m_StackerPos, CYLINDER_OFF);
				if(nRet_1 == CYLINDER_ON)
				{
					m_move_stacker_step1 = 1400;
				}
			}
			break;

		case 1310:
			nRet_1 = Set_M_RailFwdbwdCylinderOnOff(m_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step1 = 1400;
			}
			break;

			
		case 1400:
			nRet_1 = Get_M_RailFwdbwdCylinderOnOff(m_StackerPos, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_move_stacker_step1 = 1500;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if(m_StackerPos == 0)
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not backward.");
					}
					
				}
				else 
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward.");
					}

				}
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
				m_move_stacker_step1 = 1310;
			}
			break;

		case 1500:
			nRet_1 = OnRighthsStackerReadyPos(m_StackerPos, 0); //트레이 하나 Up한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_move_stacker_step1 = 1600;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_move_stacker_step1 = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 1600:
			nRet_1 = COMI.Get_MotIOSensor(m_MotorNum, MOT_SENS_SD);
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
	//			st_sync.mn_ld_module_tray_supply[mn_StackerPos] = CTL_FREE;		// 교체 끝났다고 설정.
//				m_move_stacker_step1 = 300;	
				m_move_stacker_step1 = 1700;	
			}
			/////////////// 20120719
			else if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_ON) == IO_ON && nRet_1 == CTLBD_RET_ERROR)
			{
				m_move_stacker_step1 = 1700;
			}
    		///////////////

			else
			{
				m_move_stacker_step1 = 1100;
			}
			break;	

		case 1700:
			nRet_1 = Set_M_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
			if(nRet_1 == CTL_GOOD)
			{
				m_move_stacker_step1 = 1800;
			}
			break;

		case 1800:
			nRet_1 = Get_M_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step1 = 2000;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{	
				//000203 0 00 "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."			
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 forward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not forkward.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;	
				m_move_stacker_step1 = 1700;
			}
			break;


		case 2000:
			nRet_1 = OnRighthsStackerReadyPos(m_StackerPos, 2); //트레 하나 Dn한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_move_stacker_step1 = 2100;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_move_stacker_step1 = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 2100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_ON);
			
			if (nRet_1 == IO_ON)	// 트레이가 있다
			{
				m_move_stacker_step1 = 3000;
			}
			else
			{
				//000201 0 00 "Stacker1 레일에 트레이가 존재하지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail does not exist in the tray.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_move_stacker_step1 = 2100;
			}
			break;

		case 3000:
			m_move_stacker_step1 = 0;
			n_move_flag = CTLBD_RET_GOOD;
			break;
	}


	return  n_move_flag;	
}
int CDialog_Part_Manual::Stacker2Move()
{
	int n_move_flag = CTLBD_RET_PROCEED;
	int nRet_1;

	m_MotorNum = m_motor_no;
	m_StackerPos = HSR_STACKER_2;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// =============================================================================
	if (m_b_stop_req)
	{
		m_move_stacker_step2 = 3000;
	}
	// =============================================================================

	switch(m_move_stacker_step2)
	{
		case 0 :
			if(m_MotorNum == M_HS_B_STACKER_2)	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_chk, IO_ON);
			if(nRet_1 == IO_OFF)
			{				
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{				
					//000101 0 00 "Stacker2에 트레이가 존재하지 않습니다."
					sprintf(st_msg.c_abnormal_msg, "Stacker2에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail does not exist in the tray.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
//				n_move_flag = CTLBD_RET_ERROR;
				m_move_stacker_step2 = 20;
			}
			else
			{
				m_move_stacker_step2 = 20;
			}
			break;

		case 20:
			if(m_MotorNum == M_HS_B_STACKER_2)	nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000202 0 00 "Stacker2 레일에 트레이가 있습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일에 트레이가 있습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker2.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_move_stacker_step2 = 30;
			}
			break;

		case 30:
			nRet_1 = Set_M_RailFwdbwdCylinderOnOff(m_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step2 = 40;
			}
			break;

		case 40:
			nRet_1 = Get_M_RailFwdbwdCylinderOnOff(m_StackerPos, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				Set_M_TrayLatchClinder(CYLINDER_OFF);
				m_move_stacker_step2 = 60;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker2 레일푸셔 backward가 동작이 되지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_move_stacker_step2 = 30;
			}

			break;

		case 50:
			nRet_1 = Set_M_TrayLatchClinder(CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step2 = 60;
			}
			break;

		case 60:
			nRet_1 = Get_M_TrayLatchClinder(CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				st_sync.n_ld_righths_tray_supply[1] = CTL_FREE;
				m_move_stacker_step2 = 70;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					//070208 0 07 "Right Heat Sink#2 Latch Clamp Tray Off 에러."
	// 				sprintf(mc_jamcode, "070208");
	// 				alarm.mn_alarm_assign_section = 1743; alarm.mn_count_mode = 0;
	// 				st_work.mn_run_status = CTL_dWARNING;
	// 				CTL_Lib.Alarm_Error_Occurrence(1443, st_work.mn_run_status, mc_jamcode);
					m_move_stacker_step2 = 50;
				}
				else
				{
					m_move_stacker_step2 = 50;
				}
			}
			break;
			
		case 70:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF);
			
			if (nRet_1 == IO_OFF)	// 트레이가 없으면...
			{
				mn_retry = 0;
				Set_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
				m_move_stacker_step2 = 90;
			}
			else
			{
				//070202 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 있습니다."
	// 			sprintf(mc_jamcode,"070202");
	// 			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
	// 			alarm.mn_alarm_assign_section = 1744;
	// 			CTL_Lib.Alarm_Error_Occurrence(1444, st_work.mn_run_status, mc_jamcode);
			}
			break;

		case 80:
			nRet_1 = Set_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
			if(nRet_1 == CTL_GOOD)
			{
				m_move_stacker_step2 = 90;
			}
			break;

		case 90:
			nRet_1 = Get_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
// 				mn_stacker_updn_cyliner[mn_StackerPos] = CTL_YES;
				m_move_stacker_step2 = 100;
			}
			else if(nRet_1 == CYLINDER_ERROR)
			{
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					//070205 0 07 "Right Heat Sink#2 Stacker 레일 Up 센서 에러."
	// 				sprintf(mc_jamcode,"070205");
	// 				alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
	// 				alarm.mn_alarm_assign_section = 1745;
	// 				CTL_Lib.Alarm_Error_Occurrence(1445, st_work.mn_run_status, mc_jamcode);
					m_move_stacker_step2 = 80;
				}
				else
				{
					m_move_stacker_step2 = 80;
				}
			}
			break;
		
		case 100:
			if (st_sync.mn_ld_righths_tray_change[0] == CTL_YES)		// 로드 트레이를 올려 놓을 꺼라고 요청.
			{
				m_move_stacker_step2 = 200;
			}
			else
			{
				if(st_sync.n_lotend_righths_ldrbt == YES)
				{
					m_move_stacker_step2 = 2000;
				}
				else if (st_sync.n_lotend_righths_ldtray_site == CTL_YES)
				{
					st_sync.n_lotend_righths_uldstacker_site = CTL_READY;
					m_move_stacker_step2 = 200;
				}
			}
			break;		
			
		case 200:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF);
			
			if (nRet_1 == IO_OFF)
			{
				if (st_sync.n_lotend_righths_uldstacker_site == CTL_READY)		// Lot End 동작이면..
				{
					m_move_stacker_step2 = 2000;
				}
				else
				{
					m_move_stacker_step2 = 1000;
				}
			}
			else// if (nRet_1 == IO_ON)
			{
				mn_retry = 0;
				Set_M_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
				m_move_stacker_step2 = 300;
			}
			
			break;
		
		case 210:
			nRet_1 = Set_M_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step2 = 300;
			}
			break;

		case 300:
			nRet_1 = Get_M_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_move_stacker_step2 = 400;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					//070204 0 07 "Right Heat Sink#2 Stacker 레일 backward가 동작이 되지 않습니다."
// 					sprintf(mc_jamcode,"070204");
// 					alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
// 					alarm.mn_alarm_assign_section = 1746;
// 					CTL_Lib.Alarm_Error_Occurrence(1446, st_work.mn_run_status, mc_jamcode);
					m_move_stacker_step2 = 210;
				}
				else
				{
					m_move_stacker_step2 = 210;
				}
			}
			break;

		case 400:
			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_off_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_OFF) == IO_OFF)
			{
				m_move_stacker_step2 = 600;
			}
			else
			{
				mn_retry = 0;
				nRet_1 = Set_M_TrayLatchClinder(CYLINDER_OFF);
				if(nRet_1 == CYLINDER_ON)
				{
					m_move_stacker_step2 = 500;
				}
			}
			break;

		case 410:
			nRet_1 = Set_M_TrayLatchClinder(CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step2 = 500;
			}
			break;

		case 500:
			nRet_1 = Get_M_TrayLatchClinder(CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_move_stacker_step2 = 600;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				mn_retry++;
				if(mn_retry > 3)
				{//070201 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 존재하지 않습니다."
					mn_retry = 0;
	// 				sprintf(mc_jamcode,"070201");
	// 				alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
	// 				alarm.mn_alarm_assign_section = 1747;
	// 				CTL_Lib.Alarm_Error_Occurrence(1447, st_work.mn_run_status, mc_jamcode);
					m_move_stacker_step2 = 410;
				}
				else
				{
					m_move_stacker_step2 = 410;
				}
			}
			break;

		case 600:
			if (st_sync.n_lotend_righths_uldstacker_site == CTL_READY)		// Lot End 동작이면..
			{
				m_move_stacker_step2 = 2000;
			}
			else
			{
				m_move_stacker_step2 = 1000;
			}
			break;

		case 1000:
			nRet_1 = OnRighthsStackerReadyPos(m_StackerPos,1); //SD 대기
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_move_stacker_step2 = 1200;
			}
			else if(nRet_1 == CTL_FULL)
			{
				//000302 1 01 "Loader Stocker 2 Tray Full 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "HS Right Stocker 2 Tray Full error.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				n_move_flag = CTLBD_RET_ERROR;
			}
			else if(nRet_1 == CTLBD_RET_SKIP) //스태커에 트레이가 없을때 
			{//000301 0 00 "Loader Stocker 2에 트레이 없음 에러"
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "HS Right Stocker 2에 트레이 없음 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "No tray error on HS Right Stocker 2.");
					}
					

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;


		case 1100:
			nRet_1 = Set_M_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step2 = 1200;
			}
			break;

		case 1200:
			nRet_1 = Get_M_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_ON);
			if (nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step2 = 1300;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				mn_retry++;
				if(mn_retry > 3)
				{//070203 0 06 "Right Heat Sink#2 Stacker 레일 forward가 동작이 되지 않습니다."
					mn_retry = 0;
	// 				sprintf(mc_jamcode,"070203");
	// 				alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
	// 				alarm.mn_alarm_assign_section = 1749;
	// 				CTL_Lib.Alarm_Error_Occurrence(1449, st_work.mn_run_status, mc_jamcode);
					m_move_stacker_step2 = 1100;
				}
				else
				{
					m_move_stacker_step2 = 1100;
				}
			}
			break;

		case 1300:
			if((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_off_chk, IO_ON)) == IO_ON && 
				(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_OFF) == IO_OFF))
			{
				m_move_stacker_step2 = 2000;			
			}
			else
			{
				mn_retry = 0;
				Set_M_TrayLatchClinder(CYLINDER_OFF);
				m_move_stacker_step2 = 1500;
			}
			break;

			
		case 1400:
			nRet_1 = Set_M_TrayLatchClinder(CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_move_stacker_step2 = 1500;
			}

			break;

		case 1500:
			nRet_1 = Get_M_TrayLatchClinder(CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_move_stacker_step2 = 1300;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					//070207 0 07 "Right Heat Sink#2 Latch Clamp Tray On 에러."
					//070208 0 07 "Right Heat Sink#2 Latch Clamp Tray Off 에러."				
// 					sprintf(mc_jamcode,"070208");
// 					alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
// 					alarm.mn_alarm_assign_section = 1750;
// 					CTL_Lib.Alarm_Error_Occurrence(1450, st_work.mn_run_status, mc_jamcode);
					m_move_stacker_step2 = 1400;
				}
				else
				{
					m_move_stacker_step2 = 1400;
				}
			}
			break;


		case 2000:
			st_sync.n_ld_righths_tray_supply[1] = CTL_READY; //준비됨.
			m_move_stacker_step2 = 2100;
			break;

		case 2100:
			if(st_sync.n_ld_righths_tray_supply[1] == CTL_LOCK)
			{
				m_move_stacker_step2 = 2200;
			}
			break;

		case 2200:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_ON);

			if (nRet_1 == IO_ON || st_basic.n_mode_device == 2)	// 트레이가 있으면...		
			{
				mn_retry = 0;
				Set_M_TrayLatchClinder(CYLINDER_ON);
				m_move_stacker_step2 = 2500;
			}
			else
			{
				//070201 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 존재하지 않습니다."
				sprintf(mc_alarmcode,"070201");
	// 			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
	// 			alarm.mn_alarm_assign_section = 1751;
	// 			CTL_Lib.Alarm_Error_Occurrence(1451, st_work.mn_run_status, mc_alarmcode);
			}
			break;

			
		case 2300:
			nRet_1 = Set_M_TrayLatchClinder(CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step2 = 2500;
			}
			break;

		case 2500:
			nRet_1 = Get_M_TrayLatchClinder(CYLINDER_ON);
			if (nRet_1 == CYLINDER_ON)
			{
				m_move_stacker_step2 = 2600;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					//070207 0 07 "Right Heat Sink#2 Latch Clamp Tray On 에러."
					sprintf(mc_alarmcode,"070207");
	// 				alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
	// 				alarm.mn_alarm_assign_section = 1752;
	// 				CTL_Lib.Alarm_Error_Occurrence(1452, st_work.mn_run_status, mc_alarmcode);
					m_move_stacker_step2 = 2300;
				}
				else
				{
					m_move_stacker_step2 = 2300;
				}
			}
			break;

			case 2600:
				st_sync.n_ld_righths_tray_supply[1] = CTL_FREE; //교체 환료
				m_move_stacker_step2 = 2700;
				break;

			case 2700:
				if(st_sync.n_righths_ldrbt_tray_req[0] == CTL_REQ)
				{
					st_sync.n_righths_ldrbt_tray_req[0] = CTL_READY;
					st_sync.n_righths_ldrbt_tray_req[1] = BIN_LDBUFFERBIN;
					m_move_stacker_step2 = 3000;
				}
				break;

			case 3000://한장 빼기
				if(st_sync.n_righths_ldrbt_tray_req[0] == CTL_REQ)
				{
					st_sync.n_ld_righths_tray_supply[1] = CTL_CHANGE;
					m_move_stacker_step2 = 3100;
				}
				else if(st_sync.n_lotend_righths_ldrbt == YES)
				{
					st_sync.n_righths_ldrbt_tray_req[0] = CTL_REQ;
				}
				break;

			case 3100:
// 				if(st_sync.n_ld_righths_tray_supply[1] != CTL_CHANGE) return;

				if (FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON && 
				FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_off_chk, IO_OFF) == IO_OFF)	// On 상태.
				{
					nRet_1 = Set_M_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
					if(nRet_1 == CYLINDER_ON)
					{
						mn_retry = 0;
						m_move_stacker_step2 = 3300;
					}
					}
				else
				{
					nRet_1 = Set_M_TrayLatchClinder(CYLINDER_ON);
					if(nRet_1 == CYLINDER_ON)
					{
						mn_retry = 0;
						m_move_stacker_step2 = 3200;
					}
				}
				break;
				
			case 3110:
				nRet_1 = Set_M_TrayLatchClinder(CYLINDER_ON);
				if(nRet_1 == CYLINDER_ON)
				{
					m_move_stacker_step2 = 3200;
				}
				break;

			case 3200:
				nRet_1 = Get_M_TrayLatchClinder(CYLINDER_ON);
				if (nRet_1 == CYLINDER_OFF)
				{
					nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
					if(nRet_1 == CYLINDER_ON)
					{
						mn_retry = 0;
						m_move_stacker_step2 = 3300;
					}
				}
				else if (nRet_1 == CYLINDER_ERROR)
				{
					mn_retry++;
					if(mn_retry > 3)
					{
						//070207 0 07 "Right Heat Sink#2 Latch Clamp Tray On 에러."
						sprintf(mc_alarmcode, "070207");
	// 					alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
	// 					alarm.mn_alarm_assign_section = 1753;
	// 					CTL_Lib.Alarm_Error_Occurrence(1453, st_work.mn_run_status, mc_alarmcode);
						m_move_stacker_step2 = 3110;
					}
					else
					{
						m_move_stacker_step2 = 3110;
					}
				}
				break;

			case 3210:
				nRet_1 = Set_M_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
				if(nRet_1 == CYLINDER_ON)
				{
					m_move_stacker_step2 = 3300;
				}
				break;

			case 3300:
				nRet_1 = Get_M_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
				if (nRet_1 == CYLINDER_OFF)
				{
//					ReadyPosStep[mn_StackerPos] = 0;
					m_move_stacker_step2 = 3400;
				}
				else if (nRet_1 == CYLINDER_ERROR)
				{
					mn_retry++;
					if(mn_retry > 3)
					{
						mn_retry = 0;
						//070204 0 07 "Right Heat Sink#2 Stacker 레일 backward가 동작이 되지 않습니다."
						sprintf(mc_alarmcode, "070204");
	// 					alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
	// 					alarm.mn_alarm_assign_section = 1754;
	// 					CTL_Lib.Alarm_Error_Occurrence(1454, st_work.mn_run_status, mc_alarmcode);
						m_move_stacker_step2 = 3210;
					}
					else
					{
						m_move_stacker_step2 = 3210;
					}
				}
				break;

			case 3400:
				if (FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_fwd_chk, IO_OFF) == IO_OFF && 
					FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_bwd_chk, IO_ON) == IO_ON)	// On 상태.
				{
					m_move_stacker_step2 = 3420;
				}
				else
				{
					mn_retry = 0;
					m_move_stacker_step2 = 3210;
				}
				break;

			case 3410:
				nRet_1 = Set_M_TrayLatchClinder(CYLINDER_OFF);
				if(nRet_1 == CYLINDER_ON)
				{
					m_move_stacker_step2 = 3500;
				}
				break;

			case 3420:
				nRet_1 = OnRighthsStackerReadyPos(mn_StackerPos, 0); //트레이 하나 Up한다
				if(nRet_1 == CTLBD_RET_GOOD)
				{
					mn_retry = 0;
					Set_M_TrayLatchClinder(CYLINDER_OFF);
					m_move_stacker_step2 = 3500;
				}
				break;

			case 3500:
				nRet_1 = Get_M_TrayLatchClinder(CYLINDER_OFF);
				if (nRet_1 == CYLINDER_OFF)
				{
					m_move_stacker_step2 = 3600;
				}
				else if (nRet_1 == CYLINDER_ERROR)
				{
					mn_retry++;
					if(mn_retry > 3)
					{
						mn_retry = 0;
						//070208 0 07 "Right Heat Sink#2 Latch Clamp Tray Off 에러."
						sprintf(mc_alarmcode, "070208");
	// 					alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
	// 					alarm.mn_alarm_assign_section = 1755; 
	// 					CTL_Lib.Alarm_Error_Occurrence(1455, st_work.mn_run_status, mc_alarmcode);
						m_move_stacker_step2 = 3410;
					}
					else
					{
						m_move_stacker_step2 = 3410;
					}
				}
				break;
				
			case 3600:
				nRet_1 = OnRighthsStackerReadyPos(mn_StackerPos, 2); //트레이 하나 Dn한다
				
				if (nRet_1 == CTLBD_RET_GOOD)
				{
					m_move_stacker_step2 = 3700;
				}
				break;

			case 3700:
				nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF);
				
				if (nRet_1 == IO_OFF || st_basic.n_mode_device == 2)	// 트레이가 없다
				{
					m_move_stacker_step2 = 4000;
				}
				else
				{
					//070201 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 존재하지 않습니다."
					sprintf(mc_alarmcode, "070201");
					st_work.mn_run_status = CTL_dWARNING;
	// 				CTL_Lib.Alarm_Error_Occurrence(1456, st_work.mn_run_status, mc_alarmcode);
				}
				break;

			case 4000:
				st_sync.n_ld_righths_tray_supply[mn_StackerPos] = CTL_FREE;		// 교체 끝났다고 설정.
				//트레이 정보 
				m_move_stacker_step2 = 100;
				break;

			case 10000:
				nRet_1 = CTL_Lib.Single_Move(M_HS_B_STACKER_2, 5, st_basic.nRunSpeed);
				
				if (nRet_1 == CTLBD_RET_GOOD)
				{
					m_move_stacker_step2 = 10010;
				}
				else if (nRet_1 == CTLBD_RET_ERROR)
				{
					//040010 0 04 "Left Heat Sink#2 stacker Motor 위치 이동 에러."
					sprintf(mc_alarmcode, "070201");
	// 				alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
	// 				alarm.mn_alarm_assign_section = 1757;
	// 				CTL_Lib.Alarm_Error_Occurrence(1457, st_work.mn_run_status, mc_alarmcode);
				}
				break;

			case 10010:
				FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker2_up_onoff, IO_OFF);
				FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker2_dn_onoff, IO_ON);
				m_move_stacker_step2 = 10100;
				break;

			case 10100:
				nRet_1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_2, 0, MOT_TIMEOUT);
				
				if (nRet_1 == CTLBD_RET_GOOD)
				{
					m_move_stacker_step2 = 10200;
				}
				else if (nRet_1 == CTLBD_RET_ERROR)
				{
					//070300 0 07 "Right Heat Sink#2 Stacker Motor 홈 체크 에러."
	 				sprintf(mc_alarmcode, "070300");
		// 			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
		// 			alarm.mn_alarm_assign_section = 1758;
		// 			CTL_Lib.Alarm_Error_Occurrence(1458, st_work.mn_run_status, mc_alarmcode);
				}
				break;

			case 10200:
				m_move_stacker_step2 = 0;
				n_move_flag = CTLBD_RET_GOOD;
				break;

	}



	return  n_move_flag;	
}

int CDialog_Part_Manual::Set_StackerUpDnClinder(int nStacker, int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff == CYLINDER_OFF)
	{
//		if(FAS_IO.get_in_bit(st_io.i_m_stacker1_tray_chk + (nStacker * 2), IO_ON) == IO_ON)
//		{
//			//000100 0 00 "Stacker1에 트레이가 존재합니다.제거해 주세요."
//			if(nStacker == 0)	sprintf(mc_jamcode,"000100");
//			else                sprintf(mc_jamcode,"010100");
//			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
//			alarm.mn_alarm_assign_section = 1004;
//			CTL_Lib.Alarm_Error_Occurrence(1082, st_work.mn_run_status, mc_jamcode);
//			nFuncRet = CYLINDER_ERROR;
//		}
	}
	FAS_IO.set_out_bit(st_io.o_m_stacker1_up_onoff + (nStacker * 2), n_onoff);
	FAS_IO.set_out_bit(st_io.o_m_stacker1_dn_onoff + (nStacker * 2), !n_onoff);
	mb_updncylinder_flag[nStacker] = FALSE;
	ml_undncylinder_Time[nStacker][0] = GetCurrentTime();

	return nFuncRet;
}

int CDialog_Part_Manual::Get_StackerUpDnClinder(int nStacker, int n_onoff)
{
	int nFuncRet = -1;
	
	ml_undncylinder_Time[nStacker][1] = GetCurrentTime();
	
	if (mb_updncylinder_flag[nStacker] != TRUE)
	{
		ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
		mb_updncylinder_flag[nStacker] = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_m_stacker1_up_chk + (nStacker * 4), IO_ON) == CTL_ON) && 
		(FAS_IO.get_in_bit(st_io.i_m_stacker1_dn_chk + (nStacker * 4), IO_OFF) == CTL_OFF))	// On 상태.
	{
		ml_undncylinder_Time[nStacker][2] = ml_undncylinder_Time[nStacker][1] - ml_undncylinder_Time[nStacker][0];
		
		if (ml_undncylinder_Time[nStacker][2] < 0)		
		{
			ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_undncylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[0])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_updncylinder_flag[nStacker] = FALSE;
			}
		}
		else if (ml_undncylinder_Time[nStacker][2] > st_wait.n_on_wait_time[0])
		{
			nFuncRet = CYLINDER_ON;
			mb_updncylinder_flag[nStacker] = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_m_stacker1_up_chk + (nStacker * 4), IO_OFF) == CTL_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_m_stacker1_dn_chk + (nStacker * 4), IO_ON) == CTL_ON))	// Off 상태.
	{
		ml_undncylinder_Time[nStacker][2] = ml_undncylinder_Time[nStacker][1] - ml_undncylinder_Time[nStacker][0];
		
		if (ml_undncylinder_Time[nStacker][2] < 0)		
		{
			ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_undncylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[0])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_updncylinder_flag[nStacker] = FALSE;
			}
		}
		else if (ml_undncylinder_Time[nStacker][2] > st_wait.n_on_wait_time[0])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_updncylinder_flag[nStacker]= FALSE;
		}
	}
	else
	{
		ml_undncylinder_Time[nStacker][2] = ml_undncylinder_Time[nStacker][1] - ml_undncylinder_Time[nStacker][0];
		
		if (ml_undncylinder_Time[nStacker][2] < 0)		
		{
			ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_undncylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[0])
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_updncylinder_flag[nStacker] = FALSE;
		}
	}
	
	return nFuncRet;
}

int CDialog_Part_Manual::ModuleStackerMenualMoving()
{
	int n_move_flag = CTLBD_RET_PROCEED;
	int nRet_1, nRet_2;

	m_n_motor_no   = m_n_Stacker1;
	m_n_motor_no11   = m_n_Stacker2;
	m_n_motor_no12   = m_n_Stacker_Move;

	m_n_StackerPos1 = M_STACKER_1;
	m_n_StackerPos2 = M_STACKER_2;
	m_n_Stacker_Move = M_STACKER_MOVE;

	// =============================================================================
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// =============================================================================
	if (m_b_stop_req)
	{
		m_n_MotorMove_Step = 3000;
	}
	// =============================================================================

	switch(m_n_MotorMove_Step)
	{
		case 0 :
//			if(mn_MotorNum == M_LDM_STACKER_1)	nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_ON); // 임시 20130213
//			if(nRet_1 == IO_OFF)
//			{				
//				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
//				{				
//					//000101 0 00 "Stacker1에 트레이가 존재하지 않습니다."
//					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재하지 않습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
//				}
//				n_move_flag = CTLBD_RET_ERROR;
//				m_n_move_stacker_step = 20;
//			}
//			else
//			{
				m_n_MotorMove_Step = 20;
//			}
			break;

		case 20:
			if(m_n_motor_no == M_LDM_STACKER_1)
				nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 있습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "There are rails on tray Stacker1.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_MotorMove_Step = 25;
			}
			break;

		case 25:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_onoff_chk, IO_OFF);
			nRet_2 = FAS_IO.get_in_bit(st_io.i_m_uld_slide_onoff_chk, IO_OFF);
			if(nRet_1 == IO_ON && nRet_2 == IO_ON)
			{
				m_n_MotorMove_Step = 30;
			}
			else
			{//000802 0 01 "Load Module Slide가 Open되어 있습니다. Close 해 주세요."
			 //000804 0 01 "Unload Module Slide가 Open되어 있습니다. Close 해 주세요."
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 30:
			nRet_1 = Set_RailFwdbwdCylinderOnOff(m_n_StackerPos1, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_MotorMove_Step = 100;
			}
			break;

		case 100:
			nRet_1 = Get_RailFwdbwdCylinderOnOff(m_n_StackerPos1, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_n_MotorMove_Step = 200;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not backward.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 200:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
			
			if (nRet_1 == IO_OFF)
			{
//				FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_ON);
//				FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_OFF);
				m_n_MotorMove_Step = 1000;
			}
			else// if (nRet == IO_OFF)
			{
				m_n_MotorMove_Step = 2000;
			}
			break;
		
		case 210:
			FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_OFF);
			m_n_MotorMove_Step = 300;
			break;

		case 300:
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker_guide_clamp_onoff,
			st_io.i_m_stacker_guide_clamp_on_chk, st_io.i_m_stacker_guide_unclamp_off_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_ON);
				m_n_MotorMove_Step = 400;
			}
//			else 
//			{
//				m_n_move_stacker_step = 210;
//			}
			break;

		case 310:
			FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_ON);
			m_n_MotorMove_Step = 400;
			break;

		case 400:
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker_guide_clamp_onoff,
			st_io.i_m_stacker_guide_unclamp_off_chk, st_io.i_m_stacker_guide_clamp_on_chk,
			IO_ON, st_wait.i_hs_stacker_guide_w[0], st_wait.i_hs_stacker_guide_l);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_MotorMove_Step = 1000;
			}
			else if(nRet_1 != RET_PROCEED)
			{
				m_n_MotorMove_Step = 310;
			}
			break;


		case 1000:
			nRet_1 = OnModuleStackerReadyPos(m_n_StackerPos1,1); //SD 대기
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_MotorMove_Step = 1100;
			}
			else if(nRet_1 == CTL_FULL)
			{
				//000302 1 01 "Loader Stocker 1 Tray Full 에러."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Module Stocker 1 Tray Full error러.");

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				n_move_flag = CTLBD_RET_ERROR;
			}
			else if(nRet_1 == CTLBD_RET_SKIP) //스태커에 트레이가 없을때 
			{//000301 0 00 "Loader Stocker 1에 트레이 없음 에러"
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Module Stocker 1에 트레이 없음 에러.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "No Tray Module Stocker 1 on error.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;


		case 1100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				//000100 0 00 "Stacker1에 트레이가 존재합니다.제거해 주세요."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1에 트레이가 존재합니다.제거해 주세요.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 the tray is present., Please remove.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_n_MotorMove_Step = 1300;
			}
			break;

		case 1300:
			if ((FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_fwd_chk + (m_n_StackerPos1), IO_OFF) == IO_OFF) && 
				(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_bwd_chk + (m_n_StackerPos1), IO_ON) == IO_ON))	// Off 상태.
			{
				m_n_MotorMove_Step = 1500;
			}
			else
			{
				nRet_1 = Set_RailFwdbwdCylinderOnOff(m_n_StackerPos1, CYLINDER_OFF);
				if(nRet_1 == CYLINDER_ON)
				{
					m_n_MotorMove_Step = 1400;
				}
			}
			break;

		case 1310:
			nRet_1 = Set_RailFwdbwdCylinderOnOff(m_n_StackerPos1, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_MotorMove_Step = 1400;
			}
			break;

			
		case 1400:
			nRet_1 = Get_RailFwdbwdCylinderOnOff(m_n_StackerPos1, CYLINDER_OFF);
			if (nRet_1 == CYLINDER_OFF)
			{
				m_n_MotorMove_Step = 1500;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				if(m_n_StackerPos1 == 0)
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not backward.");
					}

				}
				else 
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker2 레일푸셔 backward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker2 rail pusher is operating is not backward.");
					}

				}
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;
				m_n_MotorMove_Step = 1310;
			}
			break;

		case 1500:
			nRet_1 = OnModuleStackerReadyPos(m_n_StackerPos1, 0); //트레이 하나 Up한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_MotorMove_Step = 1600;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_n_MotorMove_Step = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 1600:
			nRet_1 = COMI.Get_MotIOSensor(m_n_motor_no, MOT_SENS_SD);
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
//				st_sync.mn_ld_module_tray_supply[mn_StackerPos] = CTL_FREE;		// 교체 끝났다고 설정.
//				m_n_move_stacker_step = 300;	
				m_n_MotorMove_Step = 1700;	
			}
			/////////////// 20120719
			else if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON) == IO_ON && nRet_1 == CTLBD_RET_ERROR)
			{
				m_n_MotorMove_Step = 1700;
			}
    		///////////////

			else
			{
				m_n_MotorMove_Step = 1100;
			}
			break;	

		case 1700:
			nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
			if(nRet_1 == CTL_GOOD)
			{
				m_n_MotorMove_Step = 1800;
			}
			break;

		case 1800:
			nRet_1 = Get_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_MotorMove_Step = 2000;
			}
			else if (nRet_1 == CYLINDER_ERROR)
			{	
				//000203 0 00 "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."			
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일푸셔 forward가 동작이 되지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail pusher is operating is not forward.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				n_move_flag = CTLBD_RET_ERROR;	
				m_n_MotorMove_Step = 1700;
			}
			break;


		case 2000:
			nRet_1 = OnModuleStackerReadyPos(m_n_StackerPos1, 2); //트레이 하나 Dn한다
			
			if (nRet_1 == CTLBD_RET_GOOD)
			{
				m_n_MotorMove_Step = 2100;
			}
			else if (nRet_1 == CTLBD_RET_SKIP)
			{
				m_n_MotorMove_Step = 0;
				n_move_flag = CTLBD_RET_ERROR;
			}
			break;

		case 2100:
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON);
			
			if (nRet_1 == IO_ON)	// 트레이가 있다
			{
				m_n_MotorMove_Step = 3000;
			}
			else
			{
				//000201 0 00 "Stacker1 레일에 트레이가 존재하지 않습니다."
				if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "Stacker1 레일에 트레이가 존재하지 않습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "Stacker1 rail does not exist in the tray.");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}
				m_n_MotorMove_Step = 2100;
			}
			break;

		case 3000:
			m_n_MotorMove_Step = 0;
			n_move_flag = CTLBD_RET_GOOD;
			break;
	}

	return  n_move_flag;
}

int CDialog_Part_Manual::StackerMovingHome()
{
	int nRet_1 = CTLBD_RET_PROCEED;
	int mn_Init_flag = CTLBD_RET_PROCEED;
	int num = 0;
	double dCurrentPos[3] = {0,};

//	if(st_handler.mn_init_state[INIT_LD_STACKER] != CTL_NO)
//		 return CTLBD_RET_PROCEED; // 초기화가 완료되었으면 Return

	dCurrentPos[0] = COMI.Get_MotCurrentPos(M_LDM_STACKER_1);
	dCurrentPos[1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_2);
	dCurrentPos[2] = COMI.Get_MotCurrentPos(M_LDM_STACKER_MOVE);
	
	switch(m_n_Stackerstep)
	{
	case 0:
		init_stackermove = NO;
		if(st_handler.nBootFlagCheck == CTL_YES || 1)
		{
			dCurrentPos[0] = COMI.Get_MotCurrentPos(M_LDM_STACKER_2);
			dCurrentPos[1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_MOVE);
			
			if(dCurrentPos[0] >= 0.0 && dCurrentPos[0] < 0.1 &&
				dCurrentPos[1] >= 180.0 && dCurrentPos[0] < 0.1) // 20130226 Stacker Move의 Uld값이 179이므로
			{
				st_handler.n_Init_Stacker = CTL_YES;
				init_stackermove = YES;
				mn_retry = 0;
				m_n_Stackerstep = 50;
			}
			else
			{
				st_handler.n_Init_Stacker = CTL_NO;
				mn_retry = 0;
				m_n_Stackerstep = 100;
			}
		}
		break;
//////////////////////////////////////////////////////////////////////////////////////// StackerMove 위치확인		
	case 50:
		if(COMI.Get_MotIOSensor(M_LDM_STACKER_MOVE, MOT_SENS_ELP) == CTLBD_RET_GOOD ||
		   COMI.Get_MotIOSensor(M_LDM_STACKER_MOVE, MOT_SENS_ELM) == CTLBD_RET_GOOD)
		{
			num++;
		}
		if(num >= 1)
		{
			init_stackermove = NO;
			mn_retry = 0;
			m_n_Stackerstep = 100;
		}
		else
		{
			sprintf(mc_jamcode,"993000");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1000, st_work.mn_run_status, mc_jamcode);
			COMI.Set_MotPower(M_LDM_STACKER_MOVE, CTL_NO);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
		break;
///////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////// Stacker 1 Tray 존재유무 확인
	case 100:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker1_up_chk, IO_OFF) == IO_ON) // stacker1 tray check
		{
			sprintf(mc_jamcode,"000100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1082, st_work.mn_run_status, mc_jamcode);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
//		else if(FAS_IO.get_in_bit(st_io.i_m_stacker2_up_chk, IO_OFF) == IO_ON) // stacker2 tray check 
//		{
//			sprintf(mc_jamcode,"010100");
//			st_work.mn_run_status = CTL_dWARNING;
//			CTL_Lib.Alarm_Error_Occurrence(1083, st_work.mn_run_status, mc_jamcode);
//		}
		else
		{
			m_n_Stackerstep = 200;
		}
		break;
////////////////////////////////////////////////////////////////////////////////// Stacker2 Tray Clamp Open, 확인		
	case 200:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);  // stacker2 tray clamp open
		if(nRet_1 == CYLINDER_ON)
		{
			m_n_Stackerstep = 300;
		}
		break;		
	case 300:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);  // stacker2 tray clamp open 확인
		if (nRet_1 == CYLINDER_OFF)
		{
			m_n_Stackerstep = 400;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++; 
			if(mn_retry > 3) // 3번이상 에러가 났을경우 알람 "Stocker2 Latch Clamp Tray Off 에러."
			{
				mn_retry = 0;
				sprintf(mc_jamcode, "010208");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1002, st_work.mn_run_status, mc_jamcode);
				mn_Init_flag = CTLBD_RET_ERROR;
				m_n_Stackerstep = 0;
			}
			else
			{
				m_n_Stackerstep = 200;
			}
		}
		break;
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////// Stacker1,2 Rail Open, 확인
	case 400:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);  // Stacker1 Rail Open
		if(nRet_1 == CTL_GOOD)
		{
			m_n_Stackerstep = 500;
		}
		break;
		
	case 500:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);  // Stacker1 Rail Open 확인
		if(nRet_1 == CYLINDER_OFF)
		{
			m_n_Stackerstep = 600;
		}
		else if (nRet_1 == CYLINDER_ERROR) // Cylinder 에러가 났을 경우 알람. "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."
		{	
			sprintf(mc_jamcode,"000203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1003, st_work.mn_run_status, mc_jamcode);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
		break;

	case 600:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(1, CYLINDER_OFF);  // Stacker2 Rail Open
		if(nRet_1 == CTL_GOOD)
		{
			m_n_Stackerstep = 700;
		}
		break;
		
	case 700:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(1, CYLINDER_OFF);  // Stacker2 Rail Open 확인
		if(nRet_1 == CYLINDER_OFF)
		{
			m_n_Stackerstep = 750;
		}
		else if (nRet_1 == CYLINDER_ERROR) // Cylinder 에러가 났을 경우 알람. "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."
		{	
			sprintf(mc_jamcode,"000203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1003, st_work.mn_run_status, mc_jamcode);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
		break;
	
	case 750:
		nRet_1 = Set_PusherUpDnCylinderOnOff(CYLINDER_OFF);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stackerstep = 800;
		}
		break;

	case 800:
		nRet_1 = Get_PusherUpDnCylinderOnOff(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			m_n_Stackerstep = 850;
		}
		else if (nRet_1 == CYLINDER_ERROR) // Cylinder 에러가 났을 경우 알람. "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."
		{	
			sprintf(mc_jamcode,"000108");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1070, st_work.mn_run_status, mc_jamcode);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
		break;

	case 850:
		nRet_1 = Set_PusherCylinderOnOff(CYLINDER_OFF);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stackerstep = 900;
		}
		break;

	case 900:
		nRet_1 = Get_PusherCylinderOnOff(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			m_n_Stackerstep = 1000;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"000108");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1070, st_work.mn_run_status, mc_jamcode);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////// Stacker2 Cylinder Down, check		
	case 1000:
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_up_onoff, IO_OFF); // Stacker2의 Cylinder Up Off
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_dn_onoff, IO_ON);  // Stacker2의 Cylinder Down On
		m_n_Stackerstep = 1100;
		break;

	case 1100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker2_dn_onoff, st_io.i_m_stacker2_dn_chk, st_io.i_m_stacker2_up_chk,
		IO_ON, st_wait.n_on_wait_time[1], st_wait.n_limit_wait_time[1]);
		if(nRet_1 == RET_GOOD)
		{
			m_n_Stackerstep = 1200;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			if(nRet_1 != RET_PROCEED)  // Stacker2 다운이 동작하지 않습니다.
				sprintf(mc_jamcode, "010109");
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
			CTL_Lib.Alarm_Error_Occurrence(1010, st_work.mn_run_status, mc_jamcode);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
		break;
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////		
////////////////////////////////////////////////////////////////////////////// Stacker Move 안전위치 확인	
	case 1200:
		if(init_stackermove == YES)
		{
			if(COMI.Get_MotIOSensor(M_LDM_STACKER_MOVE, MOT_SENS_ELP) == CTLBD_RET_GOOD)
			{
				m_n_Stackerstep = 1300;
			}
			else if(COMI.Get_MotIOSensor(M_LDM_STACKER_MOVE, MOT_SENS_ELM) == CTLBD_RET_GOOD)
			{
				m_n_Stackerstep = 1230;
			}
		}
		else
		{
			m_n_Stackerstep = 1210;			// Stacker Move가 초기화가 되지 않았으면 
		}
		break;

	case 1210:
		//Stacker Move의 위치가 Stacker Move의 STFETY위치보다 작으면
		if(dCurrentPos[2] < st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS])
		{
			m_n_Stackerstep = 1220;
		}
		//Stacker Move의 위치가 Stacker Move의 STFETY위치보다 크거나 UNLOADER 위치보다 작으면
		else if(dCurrentPos[2] > st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS] &&
			dCurrentPos[2] < st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS])
		{
			m_n_Stackerstep = 1220;
		}
		else
		{
			m_n_Stackerstep = 1300;
		}
		break;
		
	case 1220:
		// Stacker2의 위치가 Stacker2의 P_STOCKER_DOWN 위치와 같거나 작으면
		if(dCurrentPos[1] <= st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN])
		{
			m_n_Stackerstep = 1230;
		}
		// Stacker2의 위치가 Stacker2의 P_STOCKER_DOWN 위치보다 크거나 P_STOCKER_UP에서 허용 오차 위치를 뺀 값보다 작으면
		else if(dCurrentPos[1] > st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN] && 
			dCurrentPos[1] < st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_UP] - st_motor[M_LDM_STACKER_2].mn_allow)
		{
			m_n_Stackerstep = 1300;
		}
		else
		{
			m_n_Stackerstep = 1230;
		}
		break;

	case 1230:
		// Stacker Move HomeCheck 시작
		nRet_1 = COMI.HomeCheck_Mot(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			m_n_Stackerstep = 1240;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stackerstep = 1230;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1011, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
		break;

	case 1240:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS], st_basic.nManualSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			init_stackermove = YES;
			m_n_Stackerstep = 1300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stackerstep = 1240;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1011, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
		break;

	// 모듈 스테커 1번 홈을 진행한다. 
	case 1300:
		nRet_1 = COMI.HomeCheck_Mot(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].mn_homecheck_method, MOT_TIMEOUT);
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			init_stacker1 = YES;
			m_n_Stackerstep = 1400;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stackerstep = 1300;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1011, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
		break;

	// 모듈 스테커 2번 홈을 진행한다. 
	case 1400:
		nRet_1 = COMI.HomeCheck_Mot(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);
		if(nRet_1 == CTLBD_RET_GOOD)			
		{
			init_stacker2 = YES;
			m_n_Stackerstep = 1500;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stackerstep = 1400;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1012, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_Init_flag = CTLBD_RET_ERROR;
			m_n_Stackerstep = 0;
		}
		break;

	// Home 완료 플레그 확인 [ 전체 home 상태 확인 ]
	case 1500:
		if(init_stackermove == YES && init_stacker1 == YES && init_stacker2 == YES)
		{
//			sprintf(mc_jamcode,"994008");
//			st_work.mn_run_status = CTL_dWARNING;
//			CTL_Lib.Alarm_Error_Occurrence(1695, st_work.mn_run_status, mc_jamcode);
			mn_Init_flag = CTLBD_RET_GOOD;
		}
		else
		{
			m_n_Stackerstep = 1230;
		}	
		break;
	}
	return mn_Init_flag;
}

int CDialog_Part_Manual::Set_TrayLatchClinder(int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	FAS_IO.set_out_bit(st_io.o_m_stacker2_tray_clamp_onoff, n_onoff);
	mb_latchcylinder_flag = FALSE;
	ml_latchcylinder_Time[0] = GetCurrentTime();

	return nFuncRet;	
}


int CDialog_Part_Manual::Get_TrayLatchClinder(int n_onoff)
{
	int nFuncRet = -1;
	
	ml_latchcylinder_Time[1] = GetCurrentTime();
	
	if (mb_latchcylinder_flag != TRUE)
	{
		ml_latchcylinder_Time[0] = GetCurrentTime();
		mb_latchcylinder_flag = TRUE;
	}
	else if (FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_OFF) == IO_OFF)	// On 상태.
	{
		ml_latchcylinder_Time[2] = ml_latchcylinder_Time[1] - ml_latchcylinder_Time[0];
		
		if (ml_latchcylinder_Time[2] < 0)		
		{
			ml_latchcylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_latchcylinder_Time[2] > st_wait.n_limit_wait_time[9])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_latchcylinder_flag = FALSE;
			}
		}
		else if (ml_latchcylinder_Time[2] > st_wait.n_on_wait_time[9])
		{
			nFuncRet = CYLINDER_ON;
			mb_latchcylinder_flag = FALSE;
		}
	}
	else if (FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_ON) == IO_OFF && 
		FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_OFF) == IO_ON)	// Off 상태.
	{
		ml_latchcylinder_Time[2] = ml_latchcylinder_Time[1] - ml_latchcylinder_Time[0];
		
		if (ml_latchcylinder_Time[2] < 0)		
		{
			ml_latchcylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_latchcylinder_Time[2] > st_wait.n_limit_wait_time[9])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_latchcylinder_flag = FALSE;
			}
		}
		else if (ml_latchcylinder_Time[2] > st_wait.n_on_wait_time[9])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_latchcylinder_flag= FALSE;
		}
	}
	else
	{
		ml_latchcylinder_Time[2] = ml_latchcylinder_Time[1] - ml_latchcylinder_Time[0];
		
		if (ml_latchcylinder_Time[2] < 0)		
		{
			ml_latchcylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_latchcylinder_Time[2] > st_wait.n_limit_wait_time[9])
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_latchcylinder_flag = FALSE;
		}
	}
	
	return nFuncRet;
}

int CDialog_Part_Manual::Set_PusherUpDnCylinderOnOff(int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	
	FAS_IO.set_out_bit(st_io.o_m_stacker_tray_pusher_clamp_updn, n_onoff);
	mb_pusherupdncylinder_flag = FALSE;
	ml_pusherupdncylinder_Time[0] = GetCurrentTime();
	
	return nFuncRet;	
}

int CDialog_Part_Manual::Get_PusherUpDnCylinderOnOff(int n_onoff)
{
	int nFuncRet = -1;
	
	ml_pusherupdncylinder_Time[1] = GetCurrentTime();
	
	if (mb_pusherupdncylinder_flag != TRUE)
	{
		ml_pusherupdncylinder_Time[0] = GetCurrentTime();
		mb_pusherupdncylinder_flag = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_up_chk, IO_OFF) == IO_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_dn_chk, IO_ON) == IO_ON))	// On 상태.
	{
		ml_pusherupdncylinder_Time[2] = ml_pusherupdncylinder_Time[1] - ml_pusherupdncylinder_Time[0];
		
		if (ml_pusherupdncylinder_Time[2] < 0)		
		{
			ml_pusherupdncylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_pusherupdncylinder_Time[2] > st_wait.n_limit_wait_time[4])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pusherupdncylinder_flag = FALSE;
			}
		}
		else if (ml_pusherupdncylinder_Time[2] > st_wait.n_on_wait_time[4])
		{
			nFuncRet = CYLINDER_ON;
			mb_pusherupdncylinder_flag = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_up_chk, IO_ON) == IO_ON) && 
		(FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_dn_chk, IO_OFF) == IO_OFF))	// Off 상태.
	{
		ml_pusherupdncylinder_Time[2] = ml_pusherupdncylinder_Time[1] - ml_pusherupdncylinder_Time[0];
		
		if (ml_pusherupdncylinder_Time[2] < 0)		
		{
			ml_pusherupdncylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_pusherupdncylinder_Time[2] > st_wait.n_limit_wait_time[4])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pusherupdncylinder_flag = FALSE;
			}
		}
		else if (ml_pusherupdncylinder_Time[2] > st_wait.n_on_wait_time[4])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_pusherupdncylinder_flag = FALSE;
		}
	}
	else
	{
		ml_pusherupdncylinder_Time[2] = ml_pusherupdncylinder_Time[1] - ml_pusherupdncylinder_Time[0];
		
		if (ml_pusherupdncylinder_Time[2] < 0)		
		{
			ml_pusherupdncylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_pusherupdncylinder_Time[2] > st_wait.n_limit_wait_time[4])
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_pusherupdncylinder_flag = FALSE;
		}
	}
	
	return nFuncRet;
}

int CDialog_Part_Manual::Stacker1MoveMethod(int nMode)
{
	int nRet_1 = CTLBD_RET_PROCEED, nRet_2 = CTLBD_RET_PROCEED;
	int mn_stacker_flag = CTLBD_RET_PROCEED;

	mn_stacker_updn_cyliner[M_LDM_STACKER_1] = CYLINDER_ON;

	switch(m_n_StackerMoveStep)
	{
	case 0:
		nRet_1 = COMI.Get_MotIOSensor(M_LDM_STACKER_1, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_EN, cmFALSE);
			m_n_StackerMoveStep = 1000;
		}
		else 
		{
			m_n_StackerMoveStep = 100;
		}
		break;

	case 100:
		m_CurrentPos[M_LDM_STACKER_1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_1);
		if(m_CurrentPos[M_LDM_STACKER_1] > st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT] - st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DN_OFFSET])
		{
			COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_EN, cmFALSE);
			m_n_StackerMoveStep = 1000;
		}
		else
		{
			m_n_StackerMoveStep = 200;
		}
		break;

	case 200:
		COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_EN, cmTRUE);
		nRet_1 = COMI.Start_SingleMove(M_LDM_STACKER_1,st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_StackerMoveStep = 300;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1083, st_work.mn_run_status, COMI.mc_alarmcode);
			m_n_StackerMoveStep = 0;
		}
		else if(nRet_1 == CTLBD_RET_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1084, st_work.mn_run_status, COMI.mc_alarmcode);
			m_n_StackerMoveStep = 0;
		}
		break;

	case 300:
		nRet_1 = COMI.Get_MotIOSensor(M_LDM_STACKER_1, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, M_LDM_STACKER_1);
			m_n_StackerMoveStep = 400;
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_UP]);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				if(nMode == 0)
				{
					nRet_1 = COMI.Get_MotIOSensor(M_LDM_STACKER_1, MOT_SENS_SD);
					nRet_2 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
					
					if(nRet_1 != CTLBD_RET_GOOD || nRet_2 == IO_ON)
					{
						if(nRet_1 == CTLBD_RET_GOOD)
						{
							COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_EN, cmFALSE);
							m_n_StackerMoveStep = 1000;
						}
						else
						{
							m_n_StackerMoveStep = 5000;
						}
					}
					else
					{
						COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_EN, cmFALSE);
						m_n_StackerMoveStep = 2000;
					}
				}
				else if(nMode == 2)
				{
					m_n_StackerMoveStep = 400;
				}
				else
				{
					m_n_StackerMoveStep = 5000;
				}
			}
			else if(nRet_1 == CTLBD_RET_ERROR)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1085, st_work.mn_run_status, COMI.mc_alarmcode);
				m_n_StackerMoveStep = 200;
			}
			else if(nRet_1 == CTLBD_RET_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1085, st_work.mn_run_status, COMI.mc_alarmcode);
				m_n_StackerMoveStep = 200;
			}
		}
		break;

	case 400:
		if(nMode == 0) // Stacker1 Up
		{
			m_CurrentPos[M_LDM_STACKER_1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_1);
			m_TargetPos[M_LDM_STACKER_1] = m_CurrentPos[M_LDM_STACKER_1] + st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_UP_OFFSET];
			if(m_TargetPos[M_LDM_STACKER_1] > st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT])
			{
				m_TargetPos[M_LDM_STACKER_1] = st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT];
			}
			m_n_StackerMoveStep = 500;
		}
		else if(nMode == 2) // Stacker1 Down
		{
			m_CurrentPos[M_LDM_STACKER_1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_1);
			m_TargetPos[M_LDM_STACKER_1] = m_CurrentPos[M_LDM_STACKER_1] - st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DN_OFFSET];
			if(m_TargetPos[M_LDM_STACKER_1] < st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DOWN])
			{
				m_TargetPos[M_LDM_STACKER_1] = st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DOWN];
			}
			m_n_StackerMoveStep = 500;
		}
		else
		{	
			COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_EN, cmFALSE);
			m_n_StackerMoveStep = 5000;
		}
		break;

	case 500:
		COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_1, m_TargetPos[M_LDM_STACKER_1], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			if(nMode == 0)
			{
				if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON) == IO_ON)
				{
					m_n_StackerMoveStep = 5000;
				}
				else
				{
					m_n_StackerMoveStep = 2000;
				}
			}
			else
			{
				m_n_StackerMoveStep = 5000;
			}
		}
		else if(nRet_1 == BD_RETRY)
		{
			m_n_StackerMoveStep = 500;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1087, st_work.mn_run_status, COMI.mc_alarmcode);
			m_n_StackerMoveStep = 500;
		}
		break;

	case 1000:
		COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_EN, cmFALSE);
		nRet_1 = COMI.Start_SingleMove(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_StackerMoveStep = 1100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_StackerMoveStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1088, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_stacker_flag = CTLBD_RET_ERROR;
		}
		break;

	case 1100:
		m_CurrentPos[M_LDM_STACKER_1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_1);
		if(m_CurrentPos[M_LDM_STACKER_1] > st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT] - st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_UP_OFFSET]  + st_motor[M_LDM_STACKER_1].mn_allow)
			break;
	
		nRet_1 = COMI.Get_MotIOSensor(M_LDM_STACKER_1, MOT_SENS_SD); //2012,0923 FULL SENSOR
		if(nRet_1 != CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, M_LDM_STACKER_1);
			if (nMode == 1)//대기
			{
				m_n_StackerMoveStep = 5000;
			}
			else if(nMode == 0)
			{
				if(mn_stacker_updn_cyliner[M_LDM_STACKER_1] == CYLINDER_ON)
				{
					m_n_StackerMoveStep = 200;
				}
				else
				{
					m_n_StackerMoveStep = 2000;
				}
			}
			else
			{
				m_n_StackerMoveStep = 200;											// SD센서에 감지가 되지 않으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
			}
			
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DOWN]);
			if (nRet_1 == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
			{
				m_n_StackerMoveStep = 1200;
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1089, st_work.mn_run_status, COMI.mc_alarmcode);
				m_n_StackerMoveStep = 1000;
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1090, st_work.mn_run_status, COMI.mc_alarmcode);
				m_n_StackerMoveStep = 1000;
			}
		}
		break;

	case 1200:
		nRet_1 = COMI.Get_MotIOSensor(M_LDM_STACKER_1, MOT_SENS_SD); 
		if(nRet_1 == CTLBD_RET_GOOD) //이미 감지하고 있으면 트레이가 FULL까지 있다는 이야기임 
		{
			m_n_StackerMoveStep = 0;
		}
		else
		{//Full sensor는 감지되지 않았으니 다시확인한다 
			m_n_StackerMoveStep = 2000;		
		}
		break;

	case 2000:
		m_n_StackerMoveStep = 2010;
		break;

	case 2010:
		COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_StackerMoveStep = 2020;
		}
		else if (nRet_1 == BD_RETRY)
		{
			m_n_StackerMoveStep = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1092, st_work.mn_run_status, COMI.mc_alarmcode);
			m_n_StackerMoveStep = 2000;
		}
		break;

	case 2020:
		m_n_StackerMoveStep = 2300;
		break;

//	case 2100:     // 20130228
//		FAS_IO.OnCylinderAction(st_io.o_m_stacker1_up_onoff, IO_ON);   // Stacker1의 Cylinder
//		FAS_IO.OnCylinderAction(st_io.o_m_stacker1_dn_onoff, IO_OFF);  // Stacker1의 Cylinder 
//		m_n_StackerMoveStep = 2200;
//		break;
//
//	case 2200:    // 20130228
//		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker1_up_onoff, st_io.i_m_stacker1_up_chk,
//			st_io.i_m_stacker1_dn_chk, IO_ON, st_wait.n_on_wait_time[1], st_wait.n_limit_wait_time[1]);
//		if(nRet_1 == CTLBD_RET_GOOD)
//		{
//			m_n_StackerMoveStep = 100;
//		}
//		else if(nRet_1 == CTLBD_RET_ERROR)
//		{				 // Stacker2 다운이 동작하지 않습니다.
//			sprintf(mc_jamcode, "000305");
//			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
//			CTL_Lib.Alarm_Error_Occurrence(1093, st_work.mn_run_status, mc_jamcode);
//			m_n_StackerMoveStep = 2100;
//		}
////		nRet_1 = Get_StackerUpDnClinder(M_LDM_STACKER_1, CYLINDER_ON);
//		if(nRet_1 == CYLINDER_ON)
//		{
//			mn_stacker_updn_cyliner[M_LDM_STACKER_1] = CYLINDER_ON;
//			m_n_StackerMoveStep = 100;
//		}
//		else if(nRet_1 == CYLINDER_ERROR)
//		{
//			if(M_LDM_STACKER_1 == 0)
//				sprintf(mc_jamcode,"000205");
//			else
//				sprintf(mc_jamcode,"010205");
//			st_work.mn_run_status = CTL_dWARNING;
//			CTL_Lib.Alarm_Error_Occurrence(1093, st_work.mn_run_status, mc_jamcode);
//			m_n_StackerMoveStep = 2100;
//		}
		break;

	case 2300:
		if(M_LDM_STACKER_1 == 0)
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_OFF);
		}
		else
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_chk, IO_OFF);
		}
		if(nRet_1 == IO_OFF)
		{
			m_n_StackerMoveStep = 2400;
		}
		else
		{
			m_n_StackerMoveStep = 100;
		}
		break;

	case 2400:		
		st_sync.mn_ld_module_tray_supply[M_LDM_STACKER_1] = CTL_NOTREADY;		// 트레이가 없음.
		mn_stacker_flag = CTLBD_RET_SKIP;
		m_n_StackerMoveStep = 0;
		break;

	case 5000:
		mn_stacker_flag = CTLBD_RET_GOOD;
		m_n_StackerMoveStep = 0;
		break;	
	}
	return mn_stacker_flag;
}

int CDialog_Part_Manual::Stacker1MovingCheck() // nMode, 0 = Up, 1 = 대기, 2 = Down
{
	int nRet_1 = CTLBD_RET_PROCEED, nRet_2 = CTLBD_RET_PROCEED, nRet_3 = CTLBD_RET_PROCEED; // nRet_1 = Stacker1, nRet_2 = Stacker2, nRet_3 = Pusher Move,Back
	int Val_1 = 0, nVal_2 = 0, i = 0;
	int mn_stacker_flag = CTLBD_RET_PROCEED;
	
	switch(m_n_Stacker1step)
	{		
	case 0:
		
		if(FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_ON) == IO_OFF) // stacker1 tray check
		{
			sprintf(mc_jamcode, "000500");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1030, st_work.mn_run_status, mc_jamcode);
			mn_stacker_flag = CTLBD_RET_ERROR;
			m_n_Stacker1step = 0;
		}
		else if(FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_chk, IO_OFF) == IO_ON) // stacker2 tray check
		{
			sprintf(mc_jamcode, "010100"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1030, st_work.mn_run_status, mc_jamcode);
			mn_stacker_flag = CTLBD_RET_ERROR;
			m_n_Stacker1step = 0;
		}
		else if(init_stacker1 == YES && init_stacker2 == YES && init_picker == YES)  // stacker1,stacker2의 초기화가 진행이 되었는지.
		{
			m_n_Stacker1step = 100;
		}
		else
		{
			COMI.Set_MotStop(0,M_LDM_STACKER_MOVE);
			mn_stacker_flag = CTLBD_RET_ERROR;
		}
		break;
		
	case 100:
		////////////////////////////////////// stacker move를 uld 위치로 이동
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS], st_basic.nRunSpeed);
		if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF) == IO_ON) // stacker1 tray check
		{
			sprintf(mc_jamcode,"000202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1000, st_work.mn_run_status, mc_jamcode);
			mn_stacker_flag = CTLBD_RET_ERROR;
			m_n_Stacker1step = 100;
		}
		else if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 120;
		}
		else if(nRet_1 == CTLBD_RET_PROCEED)
		{
			m_n_Stacker1step = 100;
		}
		else
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1078, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 120:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);  // Stacker1 Rail Open
		if(nRet_1 == CTL_GOOD)
		{
			m_n_Stacker1step = 140;
		}
		break;
		
	case 140:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);  // Stacker1 Rail Open 확인
		if(nRet_1 == CYLINDER_OFF)
		{
			m_n_Stacker1step = 150;
		}
		else if (nRet_1 == CYLINDER_ERROR) // Cylinder 에러가 났을 경우 알람. "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."
		{	
			sprintf(mc_jamcode,"000203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1003, st_work.mn_run_status, mc_jamcode);
			mn_stacker_flag = CTLBD_RET_ERROR;
			m_n_Stacker1step = 120;
		}
		break;
		
	case 150:
		nRet_2 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		if(nRet_2 == IO_ON)
		{
			sprintf(mc_jamcode,"010202");
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1051, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
			m_n_Stacker1step = 200;
		}
		break;
		
	case 200:
		if(FAS_IO.get_in_bit(st_io.i_m_ld_slide_onoff_chk, IO_ON) == IO_OFF) // Stacker1 Slide Close Check
		{
			sprintf(mc_jamcode,"000802");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1030, st_work.mn_run_status, mc_jamcode);
			mn_stacker_flag = CTLBD_RET_ERROR;
			m_n_Stacker1step = 200;
		}
		else
		{
			m_n_Stacker1step = 250;
		}
		break;

	case 250:
		nRet_2 = Get_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_2 == CYLINDER_OFF)
		{
			Set_TrayLatchClinder(CYLINDER_OFF);
			m_n_Stacker1step = 350;
		}
		else if(nRet_2 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010204");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1052, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 300;
		}
		break;

	case 300:
		nRet_2 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 250;
		}
		break;

	case 350:
		nRet_2 = Get_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_2 == CYLINDER_OFF)
		{
			m_n_Stacker1step = 450;
		}
		else if(nRet_2 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010208");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1053, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 400;
		}
		else
		{
			m_n_Stacker1step = 400;
		}
		break;
		
	case 400:
		nRet_2 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 450;
		}
		break;

	case 450:
		nRet_2 = FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_chk, IO_OFF);
		if(nRet_2 == IO_OFF)
		{
			m_n_Stacker1step = 550;
		}
		else
		{
			sprintf(mc_jamcode,"010202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1054, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 500:
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_up_onoff, IO_OFF);   // Stacker2의 Cylinder Up OFF
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_dn_onoff, IO_ON);  // Stacker2의 Cylinder Down ON
		m_n_Stacker1step = 550;
		break;

	case 550:
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker2_dn_onoff, st_io.i_m_stacker2_dn_chk,
			st_io.i_m_stacker2_up_chk, IO_ON, st_wait.n_on_wait_time[1], st_wait.n_limit_wait_time[1]);
		if(nRet_2 == RET_GOOD)
		{
			m_n_Stacker1step = 600;
		}
		else if(nRet_2 != RET_PROCEED)
		{				 // Stacker2 다운이 동작하지 않습니다.
			sprintf(mc_jamcode, "000109");
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
			CTL_Lib.Alarm_Error_Occurrence(1010, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 500;
		}
		break;

	case 600:
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_up_onoff, IO_ON);   // Stacker2의 Cylinder Up ON
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_dn_onoff, IO_OFF);  // Stacker2의 Cylinder Down OFF
		m_n_Stacker1step = 650;
		break;

	case 650:
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker2_up_onoff, st_io.i_m_stacker2_up_chk,
			st_io.i_m_stacker2_dn_chk, IO_ON, st_wait.n_on_wait_time[1], st_wait.n_limit_wait_time[1]);
		if(nRet_2 == RET_GOOD)
		{
			m_n_Stacker1step = 700;
		}
		else if(nRet_2 != RET_PROCEED)
		{			  // Stacker2 다운이 동작하지 않습니다.
			sprintf(mc_jamcode, "010109");
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
			CTL_Lib.Alarm_Error_Occurrence(1010, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 600;
		}
		break;

	case 700:
		nRet_2 = Module_Stacker2_MoveMethod(1); //SD대기
		if(nRet_2 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 750;
		}
		else if(nRet_2 == CTL_FULL)
		{
			sprintf(mc_jamcode, "010302"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1078, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 750:
		nRet_2 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		if(nRet_2 == IO_OFF)
		{
			m_n_Stacker1step = 800;
		}
		else
		{
			Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2,CYLINDER_OFF);
			m_n_Stacker1step = 850;
		}
		break;
		
	case 800:
		nRet_2 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2,CYLINDER_OFF);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 850;
		}
		break;
	
	case 850:
		nRet_2 = Get_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2,CYLINDER_OFF);
		if(nRet_2 == CYLINDER_OFF)
		{
			m_n_Stacker1step = 900;
		}
		else if(nRet_2 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"000204");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1056, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 800;
		}
		else
		{
			m_n_Stacker1step = 900;
		}
		break;
	
	case 900:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_OFF) == IO_OFF)
		{
			m_n_Stacker1step = 1050;
		}
		else
		{
			nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_Stacker1step = 1000;
			}
		}
		break;

	case 950:
		nRet_2 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 1000;
		}
		break;

	case 1000:
		nRet_2 = Get_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_2 == CYLINDER_OFF)
		{
			m_n_Stacker1step = 1050;
		}
		else if(nRet_2 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"010201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1057, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 950;
		}
		else
		{
			m_n_Stacker1step = 950;
		}
		break;

	case 1050:
		nRet_2 = Module_Stacker2_MoveMethod(1);
		if(nRet_2 == CTLBD_RET_GOOD)
		{
			Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_ON);
			m_n_Stacker1step = 1150;
		}
		else if(nRet_2 == CTL_FULL)
		{
			sprintf(mc_jamcode, "010302"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1058, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 1050;
		}
		break;

	case 1100:
		nRet_2 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2,CYLINDER_ON);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 1150;
		}
		break;

	case 1150:
		nRet_2 = Get_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2,CYLINDER_ON);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 2000;
		}
		else if(nRet_2 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"010203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1059, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 1100;
		}
		else
		{
			m_n_Stacker1step = 2000;
		}
		break;

	case 2000:
		nRet_1 = Stacker1MoveMethod(1);

		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 2100;
		}
		else if(nRet_1 == CTLBD_RET_SKIP)
		{
			m_n_Stacker1step = 2100;
		}
//		else
//		{
//			sprintf(mc_jamcode, "000302"); 
//			st_work.mn_run_status = CTL_dWARNING;
//			CTL_Lib.Alarm_Error_Occurrence(1035, st_work.mn_run_status, mc_jamcode);	
//			m_n_Stacker1step = 1000;
//		}
		break;

	case 2100:
//		if(st_sync.mn_ld_module_tray_supply[0] == CTL_CHANGE)
//		{
			m_n_Stacker1step = 2200;
/*		}*/
		break;
		
	case 2200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_ON)
		{
			sprintf(mc_jamcode,"000202");
			st_work.mn_run_status = CTL_dWARNING;	
			CTL_Lib.Alarm_Error_Occurrence(1036, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			m_n_Stacker1step = 2300;
		}
		break;

	case 2300:
		if ((FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_fwd_chk, IO_OFF) == IO_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_bwd_chk, IO_ON) == IO_ON))	// Off 상태.
		{
			m_n_StackerMoveStep = 0;
			m_n_Stacker1step = 2500;
		}
		else
		{
			nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				m_n_Stacker1step = 2400;
			}
		}
		break;

	case 2350:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			m_n_Stacker1step = 2400;
		}
		break;

	case 2400:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			m_n_StackerMoveStep = 0;
			m_n_Stacker1step = 2500;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"000204");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1037, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 2350;
		}
		break;

	case 2500: // Stacker1 up
		nRet_1 = Stacker1MoveMethod(0); //트레이 하나 Up한다
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 2600;
		}
		else if (nRet_1 == CTLBD_RET_SKIP)
		{
			m_n_Stacker1step = 2100;
		}
		break;

	case 2600:
		nRet_1 = COMI.Get_MotIOSensor(0, MOT_SENS_SD);
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 2700;	
		}
		else
		{
			m_n_Stacker1step = 2700;
		}
		break;	

	case 2700:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			m_n_Stacker1step = 2800;
		}
		break;

	case 2800:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			m_n_StackerMoveStep = 0;
			m_n_Stacker1step = 2900;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{	
			sprintf(mc_jamcode,"000203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1038, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 2700;
		}
		break;

	case 2900:
		nRet_1 = Stacker1MoveMethod(2); //트레 하나 Dn한다
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 3000;
		}
		else if (nRet_1 == CTLBD_RET_SKIP)
		{
			m_n_Stacker1step = 2100;
		}
		break;

	case 3000:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON);
		
		if (nRet_1 == IO_ON)
		{
			m_n_Stacker1step = 3300;
		}
		else
		{
			sprintf(mc_jamcode, "000201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1039, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 3200;
		}
		break;

	case 3200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON);
		if(nRet_1 == IO_ON)
		{
			m_n_Stacker1step = 3300;
		}
		else
		{
			m_n_Stacker1step = 2200;
		}
		break;

	case 3300:
		nRet_3 = Module_TrayMove();  // Pusher Move
		if(nRet_3 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 3350;
		}
//		m_n_Stacker1step = 10000;//1800;
//		st_sync.mn_ld_module_tray_supply[M_LDM_STACKER_1] = CTL_LOCK; // 교체완료설정
		break;

	case 3350:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		{
			sprintf(mc_jamcode, "000100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1329, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			m_n_Stacker1step = 3400;
		}
		break;
		
	case 3400:
		nRet_3 = Module_TrayBack(); // Pusher Back
		if(nRet_3 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 3450;
		}
		break;

	case 3450:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_ON);
		if(nRet_1 == IO_ON)
		{
			m_n_Stacker1step = 3500;
		}
		else
		{
			sprintf(mc_jamcode, "010101");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1075, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 3500:
		if((FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_OFF)) == CTL_ON &&
			(FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_ON) == CTL_OFF))
		{
			m_n_Stacker1step = 3700;
		}
		else
		{
			Set_TrayLatchClinder(CYLINDER_OFF);
			m_n_Stacker1step = 3600;
		}
		break;

	case 3550:
		nRet_2 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 3600;
		}
		break;

	case 3600:
		nRet_2 = Get_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_2 == CYLINDER_OFF)
		{
			m_n_Stacker1step = 3500;
		}
		else if(nRet_2 == CYLINDER_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1060, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 3550;
		}
		break;

	case 3700:
		nRet_2 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_ON);
		if(nRet_2 == IO_ON)
		{
			Set_TrayLatchClinder(CYLINDER_ON);
			m_n_Stacker1step = 3800;
		}
		else
		{
			sprintf(mc_jamcode, "010201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1061, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 3800:
		nRet_2 = Set_TrayLatchClinder(CYLINDER_ON);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 3900;
		}
		break;

	case 3900:
		nRet_2 = Get_TrayLatchClinder(CYLINDER_ON);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 4000;
		}
		else if(nRet_2 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010207");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1062, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 3800;
		}
		break;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Robot 동작 추가 시점.
////////////////////////////////////////////////////////////////////////////////////////////////////		

	case 4000:
		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 4100;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 4000;
		}
		break;

	case 4100:
		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Y, st_motor[M_M_RBT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 4200;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 4100;
		}
		break;

	case 4150:
		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 4200;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 4000;
		}
		break;


	case 4200:
		md_AbsPos[0] = Func.XYRobot_MovePosValCheck(LDMODULE_SITE,0,0,PosPlus); // Module Robot Tray start위치
		nRet_1 = COMI.Start_SingleMove(M_M_RBT_Y, md_AbsPos[0], st_basic.nRunSpeed);; // 이동
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 4300;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 4200;
		}
		break;

	case 4300:
		nRet_1 = COMI.Check_SingleMove(M_M_RBT_Y, md_AbsPos[0]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 4600;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 4300;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1106, st_work.mn_run_status, COMI.mc_alarmcode);
			m_n_Stacker1step = 4200;
		}
		break;

	case 4600:
		Set_FingerOnOff(IO_ON, PickerAction);
		m_n_Stacker1step = 4650;
		break;

	case 4650:
		nRet_1 = Get_FingerOnOff(IO_ON, PickerAction);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			m_n_Stacker1step = 4700;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			l_until_wait_time[0] = GetCurrentTime();
			m_n_Stacker1step = 4600;
		}
		break;

	case 4700:
		Set_PickerUpDown(IO_ON, PickerAction);
		nRet_1 =  CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_TRAY_PICK] - st_motor[M_M_RBT_Z].mn_allow, st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 4750;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 4700;
		}
		else if(nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1992, st_work.mn_run_status, COMI.mc_alarmcode);
			m_n_Stacker1step = 4700;
		}
		break;
	
	case 4750:
		nRet_1 = Get_PickerUpDown(IO_ON, ActionPicker);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 4800;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			st_work.mn_run_status = dWARNING;
			m_n_Stacker1step = 4700;
		}
		break;

	case 4800:
		Set_FingerOnOff(IO_OFF, PickerAction);
		m_n_Stacker1step = 4850;
		break;

	case 4850:
		nRet_1 = Get_FingerOnOff(IO_OFF, PickerAction);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			m_n_Stacker1step = 4900;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			l_until_wait_time[0] = GetCurrentTime();
			m_n_Stacker1step = 4800;
		}
		break;

	case 4900:
		Set_PickerUpDown(IO_OFF, PickerAction);
		nRet_1 =  CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_TRAY_PICK] - st_motor[M_M_RBT_Z].mn_allow, st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 5000;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 4900;
		}
		else if(nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1992, st_work.mn_run_status, COMI.mc_alarmcode);
			m_n_Stacker1step = 4900;
		}
		break;
	
	case 5000:
		nRet_1 = Get_PickerUpDown(IO_OFF, PickerAction);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 5100;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			st_work.mn_run_status = dWARNING;
			m_n_Stacker1step = 4900;
		}
		break;

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_DIRECTION_CHECK], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 5150;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 5100;
		}
		else if(nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_SAFETY)
		{
			m_n_Stacker1step = 5100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1113, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 5150:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_module_dvc_chk[i], IO_OFF);
		if(nRet_1 == CTLBD_RET_GOOD && PosPlus == 24)
		{
			PosPlus = 0;
			m_n_Stacker1step = 5200;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			m_n_Stacker1step = 5150;
		}
		else if(PosPlus != 24)
		{
			PosPlus++;
			m_n_Stacker1step = 4150;
		}
		break;

	case 5200:
		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 7500;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 5150;
		}
		else if(nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_SAFETY)
		{
			m_n_Stacker1step = 5150;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1113, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 7500:
		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Y, st_motor[M_M_RBT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 8000;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 7500;
		}
		else if(nRet_1 == CTLBD_RET_ERROR || nRet_1 == CTLBD_RET_SAFETY)
		{
			m_n_Stacker1step = 7500;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1113, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Robot 동작 완료 시점
//////////////////////////////////////////////////////////////////////////////////////////////////
	case 8000:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_OFF) == IO_OFF)
		{
			nRet_2 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
			if(nRet_2 == CYLINDER_ON)
			{
				m_n_Stacker1step = 8300;
			}
		}
		else
		{
			nRet_2 = Set_TrayLatchClinder(CYLINDER_ON);
			if(nRet_2 == CYLINDER_ON)
			{
				m_n_Stacker1step = 8200;
			}
		}
		break;


	case 8100:
		nRet_2 = Set_TrayLatchClinder(CYLINDER_ON);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 8200;
		}
		break;

	case 8200:
		nRet_2 = Get_TrayLatchClinder(CYLINDER_ON);
		if(nRet_2 == CYLINDER_OFF)
		{
			nRet_2 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
			if(nRet_2 == CYLINDER_ON)
			{
				m_n_Stacker1step = 8300;
			}
		}
		else if(nRet_2 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010207");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1063, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 8100;
		}
		break;

	case 8250:
		nRet_2 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 8300;
		}
		break;

	case 8300:
		nRet_2 = Get_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_2 == CYLINDER_OFF)
		{
			m_n_Stacker1step = 8400;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010204");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1064, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 8250;
		}
		break;

	case 8400:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_fwd_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_bwd_chk, IO_ON) == IO_ON)
		{
			m_n_Stacker1step = 8500;
		}
		else
		{
			m_n_Stacker1step = 8450;
		}
		break;
		
	case 8450:
		nRet_2 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_2 == CYLINDER_ON)
		{
			m_n_Stacker1step = 8600;
		}
		break;

	case 8500:
		nRet_2 = Module_Stacker2_MoveMethod(0);
		if(nRet_2 == CTLBD_RET_GOOD)
		{
			Set_TrayLatchClinder(CYLINDER_OFF);
			m_n_Stacker1step = 8600;
		}
		break;
		
	case 8600:
		nRet_2 = Get_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_2 == CYLINDER_OFF)
		{
			m_n_Stacker1step = 8700;
		}
		else if(nRet_2 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010208");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1065, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 8450;
		}
		break;
		
	case 8700:
		nRet_2 = Module_Stacker2_MoveMethod(2); // stacker2 대기 위치
		if(nRet_2 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 8800;
		}
		break;

	case 8800:
		nRet_2 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		if(nRet_2 == IO_OFF)
		{
			m_n_Stacker1step = 8850;
		}
		else
		{
			sprintf(mc_jamcode, "000201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1066, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 8850:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker1_up_chk, IO_OFF) == IO_ON) // stacker1 tray check
		{
			m_n_Stacker1step = 700;
		}
		else
		{
			m_n_Stacker1step = 8900;
		}
		break;
		
	case 8900:
		nRet_2 = CTL_Lib.Single_Move(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_2 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 9000;
		}
		else if(nRet_2 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 8900;
		}
		else if(nRet_2 == CTLBD_RET_ERROR || nRet_2 == CTLBD_RET_SAFETY)
		{
			sprintf(mc_jamcode,"010010");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1067, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 9000:
		FAS_IO.set_out_bit(st_io.o_m_stacker2_up_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_m_stacker2_dn_onoff, IO_ON);
		m_n_Stacker1step = 9100;
		break;

	case 9100:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_1, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			m_n_Stacker1step = 9200;
		}
		break;

	case 9200:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(M_LDM_STACKER_1, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			m_n_Stacker1step = 9300;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010204");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1068, st_work.mn_run_status, mc_jamcode);
			m_n_Stacker1step = 9100;
		}
		break;

	case 9300:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 9400;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			m_n_Stacker1step = 9300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1079, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 9400:
		if(FAS_IO.get_in_bit(st_io.i_m_uld_slide_tray_chk, IO_ON) == IO_ON)
		{
			m_n_Stacker1step = 10000;
		}
		else
		{
			sprintf(mc_jamcode, _T("000800"));
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1080, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 10000:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_n_Stacker1step = 11000;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1040, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 11000:
//		sprintf(mc_jamcode,"999000");
//		st_work.mn_run_status = CTL_dWARNING;
//		CTL_Lib.Alarm_Error_Occurrence(1620, st_work.mn_run_status, mc_jamcode);
		m_n_Stacker1step = 0;
		init_stacker1, init_stacker2, init_picker = NO;
		mn_stacker_flag = CTLBD_RET_GOOD;
		break;
	}
	return mn_stacker_flag;
}

 int CDialog_Part_Manual::Module_Rbt_Home()
 {
  	int i, nRet_1 = CTLBD_RET_PROCEED, nRet_2 = CTLBD_RET_PROCEED;
	int mn_Rbt_flag = CTLBD_RET_PROCEED;

	switch(mn_MDRbt_step)
	{
	case 0:
		for(i = 0; i<PICKER_NUM; i++)
		{
			PickerAction[D_EXIST][i] = YES;
		}
		Set_FingerOnOff(IO_OFF, PickerAction);
		mn_MDRbt_step = 100;
		break;

	case 100:
		nRet_1 = Get_FingerOnOff(IO_OFF, PickerAction);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_MDRbt_step = 200;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			mn_MDRbt_step = 100;
		}
		break;

	case 200:
		Set_PickerUpDown(IO_OFF, PickerAction);
		mn_MDRbt_step = 300;
		break;

	case 300:
		nRet_1 = Get_PickerUpDown(IO_OFF, PickerAction);

		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_MDRbt_step = 400;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			CTL_Lib.Alarm_Error_Occurrence(1100, st_work.mn_run_status, mc_alarmcode);
			mn_Rbt_flag = CTLBD_RET_ERROR;
		}
		break;
		
	case 400:
		nRet_1 = COMI.HomeCheck_Mot(M_M_RBT_Z, st_motor[M_M_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);
		
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			mn_MDRbt_step = 500;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			mn_MDRbt_step = 400;
		}
		else if(nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1101, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_Rbt_flag = CTLBD_RET_ERROR;
		}
		break;

	case 500:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		if(l_until_wait_time[2] < 0)
			l_until_wait_time[0] = GetCurrentTime();
		if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME)
			break;

		nRet_1 = CTLBD_RET_GOOD;
		for(i = 0; i<PICKER_NUM; i++)
		{
			if(PickerAction[D_EXIST][i] == DVC_YES && st_picker.n_module_ldrbt_enable[i] == YES)
			{
				if(FAS_IO.get_in_bit(st_io.i_ld_module_dvc_chk[i], IO_ON) == IO_OFF)
				{
					sprintf(mc_alarmcode, "40050%d",  i); //피커가 벌렸는데 센서가 ON 되었다..
					CTL_Lib.Alarm_Error_Occurrence(1102, CTL_dWARNING, mc_alarmcode);
					nRet_1 = RET_ERROR;
					break;
				}
			}
		}
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_MDRbt_step = 600;
		}
		break;

	case 600:
		nRet_1 = COMI.HomeCheck_Mot(M_M_RBT_Y, st_motor[M_M_RBT_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_MDRbt_step = 700;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			mn_MDRbt_step = 600;
		}
		else if(nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"%s", alarm.mstr_code);
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1102, st_work.mn_run_status, mc_alarmcode);
			mn_Rbt_flag = CTLBD_RET_ERROR;
		}
		break;

	case 700:
		mn_MDRbt_step = 0;
		init_picker = YES;
		mn_Rbt_flag = CTLBD_RET_GOOD;
		break;
	}
	return mn_Rbt_flag;
 }

void CDialog_Part_Manual::Module_Stacker_Move()
{
	int nRet_1;
	
	switch(mn_Stacker_MovingStep)
	{
	case 0:
		if(init_stacker1 == YES && init_stacker2 == YES)
		{
			mn_Stacker_MovingStep = 100;
		}
		else
		{
			COMI.Set_MotStop(0,M_LDM_STACKER_MOVE);
		}
		break;

	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_Stacker_MovingStep = 200;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			mn_Stacker_MovingStep = 100;
		}
		else
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1078, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 200: // Stacker2의 동작이 완료가 되었으면 Stacker Move가 동작
		mn_Stacker_MovingStep = 300;
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS],st_basic.nRunSpeed);
		if(nRet_1 = CTLBD_RET_GOOD)
		{
			mn_Stacker_MovingStep = 400;
		}
		else if(nRet_1 = CTLBD_RET_RETRY)
		{
			mn_Stacker_MovingStep = 300;
		}
		else
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1079, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 400:
		if(FAS_IO.get_in_bit(st_io.i_m_uld_slide_tray_chk, IO_ON) == IO_ON)
		{
			mn_Stacker_MovingStep = 0;
		}
		else 
		{
			sprintf(mc_jamcode, _T("000800"));
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1080, st_work.mn_run_status, mc_jamcode);
		}
		break;
	}
}

int CDialog_Part_Manual::Module_TrayBack() // Pusher의 Up/Down 실린더 
{
	int mn_TrayBack_flag = CTLBD_RET_PROCEED;
	int nRet_1;

	switch(mn_TrayBackStep)
	{
	case 0:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_up_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_dn_chk, IO_OFF) == IO_OFF)
		{
			mn_TrayBackStep = 100;
		}
		else
		{
			Set_PusherUpDnCylinderOnOff(CYLINDER_OFF);
			mn_TrayBackStep = 50;
		}
		break;

	case 10:
		nRet_1 = Set_PusherUpDnCylinderOnOff(CYLINDER_OFF);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_TrayBackStep = 50;
		}
		break;

	case 50:
		nRet_1 = Get_PusherUpDnCylinderOnOff(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_TrayBackStep = 100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"000108");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1070, st_work.mn_run_status, mc_jamcode);
			mn_TrayBackStep = 10;
		}
		break;

	case 100:
		if((FAS_IO.Get_In_Bit(st_io.i_m_stacker_tray_pusher_fwd_chk) == IO_ON) ||
			(FAS_IO.Get_In_Bit(st_io.i_m_stacker_tray_pusher_bwd_chk)) == IO_OFF)
		{
			Set_PusherCylinderOnOff(CYLINDER_OFF);
			mn_TrayBackStep = 110;
		}
		else
		{
			mn_TrayBackStep = 200;
		}
		break;

	case 110:
		nRet_1 = Set_PusherCylinderOnOff(CYLINDER_OFF);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_TrayBackStep = 120;
		}
		break;

	case 120:
		nRet_1 = Get_PusherCylinderOnOff(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_TrayBackStep = 200;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"000108");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1070, st_work.mn_run_status, mc_jamcode);
			mn_TrayBackStep = 110;
		}
		break;

	case 200:
		mn_TrayBack_flag = CTLBD_RET_GOOD;
		mn_TrayBackStep = 0;
		break;
	}
	return mn_TrayBack_flag;
}

int CDialog_Part_Manual::Module_TrayMove()
{
	int mn_TrayMove_flag = CTLBD_RET_PROCEED;
	int nRet_1;

	switch(mn_TrayMoveStep)
	{
	case 0:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_up_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_dn_chk, IO_OFF) == IO_OFF)
		{
			Set_PusherUpDnCylinderOnOff(CYLINDER_ON);
			mn_TrayMoveStep = 100;
		}
		else 
		{
			mn_TrayMoveStep = 300;
		}
		break;

	case 100:
		nRet_1 = Set_PusherUpDnCylinderOnOff(CYLINDER_ON);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_TrayMoveStep = 200;
		}
		break;

	case 200:
		nRet_1 = Get_PusherUpDnCylinderOnOff(CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_TrayMoveStep = 0;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			mn_TrayMoveStep = 100;
			sprintf(mc_jamcode,"000108");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1072, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 300:
		if((FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_fwd_chk, IO_OFF) == IO_OFF) &&
			(FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_bwd_chk, IO_ON) == IO_ON))
		{
			Set_PusherCylinderOnOff(CYLINDER_ON);
			mn_TrayMoveStep = 500;
		}
		else
		{
			sprintf(mc_jamcode,"000107");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1073, st_work.mn_run_status, mc_jamcode);
		}
		mn_TrayMoveStep = 400;
		break;

	case 400:
		nRet_1 = Set_PusherCylinderOnOff(CYLINDER_ON);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_TrayMoveStep = 500;
		}
		break;

	case 500:
		nRet_1 = Get_PusherCylinderOnOff(CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_TrayMoveStep = 0;
			mn_TrayMove_flag = CTLBD_RET_GOOD;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"000104");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1074, st_work.mn_run_status, mc_jamcode);
			mn_TrayMoveStep = 400;
		}
		break;
	}
	return mn_TrayMove_flag;
}

int CDialog_Part_Manual::Set_PusherCylinderOnOff(int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff == CYLINDER_OFF)
	{
		if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		{
			//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"000202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1080, st_work.mn_run_status, mc_jamcode);
			nFuncRet = CYLINDER_ERROR;
		}
	}

	FAS_IO.set_out_bit(st_io.o_m_stacker_tray_pusher_fwd_onoff, n_onoff);
	FAS_IO.set_out_bit(st_io.o_m_stacker_tray_pusher_bwd_onoff, !n_onoff);
	mb_cylinder_flag = FALSE;
	ml_cylinder_Time[0] = GetCurrentTime();

	return nFuncRet;
	
}

int CDialog_Part_Manual::Get_PusherCylinderOnOff(int n_onoff)
{
	int nFuncRet = -1;
	
	ml_cylinder_Time[1] = GetCurrentTime();
	
	if (mb_cylinder_flag != TRUE)
	{
		ml_cylinder_Time[0] = GetCurrentTime();
		mb_cylinder_flag = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_fwd_chk, IO_ON) == CTL_ON) && 
		(FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_bwd_chk, IO_OFF) == CTL_OFF))	// On 상태.
	{
		ml_cylinder_Time[2] = ml_cylinder_Time[1] - ml_cylinder_Time[0];
		
		if (ml_cylinder_Time[2] < 0)		
		{
			ml_cylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_cylinder_Time[2] > st_wait.n_limit_wait_time[2])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_cylinder_flag = FALSE;
			}
		}
		else if (ml_cylinder_Time[2] > st_wait.n_on_wait_time[2])
		{
			nFuncRet = CYLINDER_ON;
			mb_cylinder_flag = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_fwd_chk, IO_OFF) == CTL_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_bwd_chk, IO_ON) == CTL_ON))	// Off 상태.
	{
		ml_cylinder_Time[2] = ml_cylinder_Time[1] - ml_cylinder_Time[0];
		
		if (ml_cylinder_Time[2] < 0)		
		{
			ml_cylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_cylinder_Time[2] > st_wait.n_limit_wait_time[2])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_cylinder_flag = FALSE;
			}
		}
		else if (ml_cylinder_Time[2] > st_wait.n_on_wait_time[2])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_cylinder_flag = FALSE;
		}
	}
	else
	{
		ml_cylinder_Time[2] = ml_cylinder_Time[1] - ml_cylinder_Time[0];
		
		if (ml_cylinder_Time[2] < 0)		
		{
			ml_cylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_cylinder_Time[2] > st_wait.n_limit_wait_time[2])
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_cylinder_flag = FALSE;
		}
	}
	
	return nFuncRet;
}

int CDialog_Part_Manual::Module_Stacker2_MoveMethod(int nMode)
{
	int nRet_1 = CTLBD_RET_PROCEED, nRet_2 = CTLBD_RET_PROCEED;
	int mn_stacker_flag = CTLBD_RET_PROCEED;

	if(FAS_IO.get_in_bit(st_io.i_m_stacker2_up_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_m_stacker2_dn_chk, IO_OFF) == IO_OFF)
	{
		mn_stacker_updn_cyliner[M_LDM_STACKER_2] = CYLINDER_ON;
	}
	else
	{
		mn_stacker_updn_cyliner[M_LDM_STACKER_2] = CYLINDER_OFF;
	}

	switch(mn_S2MoveStep)
	{
	case 0:
		nRet_1 = COMI.Get_MotIOSensor(M_LDM_STACKER_2, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_EN, cmFALSE);
			mn_S2MoveStep = 1000;
		}
		else
		{
			mn_S2MoveStep = 100;
		}
		break;

	case 100:
		m_CurrentPos[M_LDM_STACKER_2] = COMI.Get_MotCurrentPos(M_LDM_STACKER_2);
		if(m_CurrentPos[M_LDM_STACKER_2] > st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_P_LIMIT] - st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DN_OFFSET])
		{
			COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_EN, cmFALSE);
			mn_S2MoveStep = 1000;
		}
		else
		{
			mn_S2MoveStep = 200;
		}
		break;

	case 200:
		COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_EN, cmTRUE);
		nRet_1 = COMI.Start_SingleMove(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_S2MoveStep = 300;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1083, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_S2MoveStep = 0;
		}
		break;

	case 300:
		nRet_1 = COMI.Get_MotIOSensor(M_LDM_STACKER_2, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, M_LDM_STACKER_2);
			if(mn_stacker_updn_cyliner[M_LDM_STACKER_2] == CYLINDER_OFF)
			{
				mn_S2MoveStep = 2000;
			}
			else
			{
				mn_S2MoveStep = 400;
			}
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_UP]);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				if(nMode == 0)
				{
					if(mn_stacker_updn_cyliner[M_LDM_STACKER_2] == CYLINDER_ON)
					{
						mn_S2MoveStep = 5000;
					}
					else
					{
						COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_EN, cmFALSE);
						mn_S2MoveStep = 2000;
					}
				}
				else if(nMode == 2)
				{
					mn_S2MoveStep = 400;
				}
				else
				{
					mn_S2MoveStep = 5000;
				}
			}
			else if(nRet_1 == CTLBD_RET_ERROR)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1085, st_work.mn_run_status, COMI.mc_alarmcode);
				mn_S2MoveStep = 200;
			}
			else if(nRet_1 == CTLBD_RET_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1085, st_work.mn_run_status, COMI.mc_alarmcode);
				mn_S2MoveStep = 200;
			}
		}
		break;

	case 400:
		if(nMode == 0) // Stacker1 Up
		{
			m_CurrentPos[M_LDM_STACKER_2] = COMI.Get_MotCurrentPos(M_LDM_STACKER_2);
			m_TargetPos[M_LDM_STACKER_2] = m_CurrentPos[M_LDM_STACKER_2] + st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_UP_OFFSET];
			if(m_TargetPos[M_LDM_STACKER_2] > st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_P_LIMIT])
			{
				m_TargetPos[M_LDM_STACKER_2] = st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_P_LIMIT];
			}
			mn_S2MoveStep = 500;
		}
		else if(nMode == 2) // Stacker1 Down
		{
			m_CurrentPos[M_LDM_STACKER_2] = COMI.Get_MotCurrentPos(M_LDM_STACKER_2);
			m_TargetPos[M_LDM_STACKER_2] = m_CurrentPos[M_LDM_STACKER_2] - st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DN_OFFSET*3];
			if(m_TargetPos[M_LDM_STACKER_2] < st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN])
			{
				m_TargetPos[M_LDM_STACKER_2] = st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN];
			}
			mn_S2MoveStep = 500;
		}
		else
		{	
			COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_EN, cmFALSE);
			mn_S2MoveStep = 5000;
		}
		break;

	case 500:
		COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_2, m_TargetPos[M_LDM_STACKER_2], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			if(nMode == 0)
			{
				if(FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_ON) == IO_ON)
				{
					mn_S2MoveStep = 5000;
				}
				else
				{
					mn_S2MoveStep = 2000;
				}
			}
			else
			{
				mn_S2MoveStep = 5000;
			}
		}
		else if(nRet_1 == BD_RETRY)
		{
			mn_S2MoveStep = 500;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1087, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_S2MoveStep = 500;
		}
		break;

	case 1000:
		COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_EN, cmFALSE);
		nRet_1 = COMI.Start_SingleMove(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_S2MoveStep = 1100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_S2MoveStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1088, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_stacker_flag = CTLBD_RET_ERROR;
		}
		break;

	case 1100:
		m_CurrentPos[M_LDM_STACKER_2] = COMI.Get_MotCurrentPos(M_LDM_STACKER_2);
		if(m_CurrentPos[M_LDM_STACKER_2] > st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_P_LIMIT] - st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_UP_OFFSET]  + st_motor[M_LDM_STACKER_2].mn_allow)
			break;
	
		nRet_1 = COMI.Get_MotIOSensor(M_LDM_STACKER_2, MOT_SENS_SD); //2012,0923 FULL SENSOR
		if(nRet_1 != CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, M_LDM_STACKER_2);
			if (nMode == 1)//대기
			{
				mn_S2MoveStep = 5000;
			}
			else if(nMode == 0)
			{
				if(mn_stacker_updn_cyliner[M_LDM_STACKER_2] == CYLINDER_ON)
				{
					mn_S2MoveStep = 200;
				}
				else
				{
					mn_S2MoveStep = 2000;
				}
			}
			else
			{
				mn_S2MoveStep = 200;											// SD센서에 감지가 되지 않으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
			}
			
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN]);
			if (nRet_1 == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
			{
				mn_S2MoveStep = 1200;
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1089, st_work.mn_run_status, COMI.mc_alarmcode);
				mn_S2MoveStep = 1000;
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1090, st_work.mn_run_status, COMI.mc_alarmcode);
				mn_S2MoveStep = 1000;
			}
		}
		break;

	case 1200:
		nRet_1 = COMI.Get_MotIOSensor(M_LDM_STACKER_2, MOT_SENS_SD); 
		if(nRet_1 == CTLBD_RET_GOOD) //이미 감지하고 있으면 트레이가 FULL까지 있다는 이야기임 
		{
			mn_S2MoveStep = 0;
		}
		else
		{//Full sensor는 감지되지 않았으니 다시확인한다 
			mn_S2MoveStep = 2000;		
		}
		break;

	case 2000:
		mn_S2MoveStep = 2010;
		break;

	case 2010:
		COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_S2MoveStep = 2020;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_S2MoveStep = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1092, st_work.mn_run_status, COMI.mc_alarmcode);
			m_n_StackerMoveStep = 2000;
		}
		break;

	case 2020:
		if(mn_stacker_updn_cyliner[M_LDM_STACKER_2] == CYLINDER_ON)
		{
			mn_S2MoveStep = 2300;
		}
		else
		{
			mn_S2MoveStep = 2100;
		}
		break;

	case 2100:
		nRet_1 = Set_StackerUpDnClinder(M_LDM_STACKER_2, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			mn_S2MoveStep = 2200;
		}
		break;

	case 2200:
		nRet_1 = Get_StackerUpDnClinder(M_LDM_STACKER_2, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_stacker_updn_cyliner[M_LDM_STACKER_2] = CYLINDER_ON;
			mn_S2MoveStep = 100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			if(M_LDM_STACKER_2 == 0)
				sprintf(mc_jamcode,"000205");
			else
				sprintf(mc_jamcode,"010205");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1093, st_work.mn_run_status, mc_jamcode);
			mn_S2MoveStep = 2100;
		}
		break;

	case 2300:
		if(M_LDM_STACKER_2 == 0)
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_OFF);
		}
		else
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_chk, IO_OFF);
		}
		if(nRet_1 == IO_OFF)
		{
			mn_S2MoveStep = 2400;
		}
		else
		{
			mn_S2MoveStep = 100;
		}
		break;

	case 2400:		
		st_sync.mn_ld_module_tray_supply[M_LDM_STACKER_2] = CTL_NOTREADY;		// 트레이가 없음.
		mn_stacker_flag = CTLBD_RET_SKIP;
		mn_S2MoveStep = 0;
		break;

	case 5000:
		mn_stacker_flag = CTLBD_RET_GOOD;
		mn_S2MoveStep = 0;
		break;	
	}
	return mn_stacker_flag;	
}

void CDialog_Part_Manual::Module_Stacker2_Move()
{
	int nRet_1;

	switch(mn_S2Step)
	{
	case 0:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_chk, IO_OFF);
		if(nRet_1 == IO_ON)
		{
			sprintf(mc_jamcode, "010100");
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1050, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			mn_S2Step = 100;
		}
		break;

	case 100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_ON)
		{
			sprintf(mc_jamcode,"010202");
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1051, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
			mn_S2Step = 200;
		}
		break;

	case 150:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 200;
		}
		break;

	case 200:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			Set_TrayLatchClinder(CYLINDER_OFF);
			mn_S2Step = 300;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010204");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1052, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 150;
		}
		else
		{
			mn_S2Step = 150;
		}
		break;

	case 250:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 300;
		}
		break;

	case 300:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_S2Step = 400;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010208");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1053, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 250;
		}
		else
		{
			mn_S2Step = 250;
		}
		break;

	case 400:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			mn_S2Step = 500;
		}
		else
		{
			sprintf(mc_jamcode,"010202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1054, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 450:
		Set_StackerUpDnClinder(M_LDM_STACKER_2, CYLINDER_OFF);
		mn_S2Step = 500;
		break;

	case 500:
		nRet_1 = Get_StackerUpDnClinder(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_S2Step = 600;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"010205");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1055, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 450;
		}
		else
		{
			mn_S2Step = 450;
		}
		break;

	case 600:
		Set_StackerUpDnClinder(M_LDM_STACKER_2, CYLINDER_ON);
		mn_S2Step = 700;
		break;

	case 700:
		nRet_1 = Get_StackerUpDnClinder(M_LDM_STACKER_2, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 800;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"000206");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1055, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 600;
		}
		else
		{
			mn_S2Step = 600;
		}
		break;

	case 800:
		nRet_1 = Module_Stacker2_MoveMethod(1); //SD대기
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			nRet_1 = 900;
		}
		else if(nRet_1 == CTL_FULL)
		{
			sprintf(mc_jamcode, "010302"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1078, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 900:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			mn_S2Step = 1500;
		}
		else
		{
			Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
			mn_S2Step = 1000;
		}
		break;
		
	case 950:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 1000;
		}
		break;
	
	case 1000:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_S2Step = 1100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"000204");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1056, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 950;
		}
		else
		{
			mn_S2Step = 950;
		}
		break;
	
	case 1100:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_OFF) == IO_OFF)
		{
			mn_S2Step = 1500;
		}
		else
		{
			nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				mn_S2Step = 1200;
			}
		}
		break;

	case 1150:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 1200;
		}
		break;

	case 1200:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_S2Step = 1500;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"010201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1057, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 1150;
		}
		else
		{
			mn_S2Step = 1150;
		}
		break;

	case 1500:
		nRet_1 = Module_Stacker2_MoveMethod(1);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_ON);
			mn_S2Step = 1700;
		}
		else if(nRet_1 == CTL_FULL)
		{
			sprintf(mc_jamcode, "010302"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1058, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 1500;
		}
		break;

	case 1600:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 1700;
		}
		break;

	case 1700:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 1800;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode,"010203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1059, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 1600;
		}
		else
		{
			mn_S2Step = 1600;
		}
		break;

	case 1800:
		if((FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_OFF)) == CTL_ON &&
			(FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_ON) == CTL_OFF))
		{
			mn_S2Step = 2500;
		}
		else
		{
			Set_TrayLatchClinder(CYLINDER_OFF);
			mn_S2Step = 2000;
		}
		break;

	case 1900:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 2000;
		}
		break;

	case 2000:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_S2Step = 1800;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1060, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 1900;
		}
		else
		{
			mn_S2Step = 1900;
		}
		break;
/////////////////////////////////////////////////////////////////////////////////////// 447
	case 2500:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_ON);
		if(nRet_1 == IO_ON)
		{
			Set_TrayLatchClinder(CYLINDER_ON);
			mn_S2Step = 2700;
		}
		else
		{
			sprintf(mc_jamcode, "010201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1061, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 2600:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 2700;
		}
		break;

	case 2700:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 2800;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010207");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1062, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 2600;
		}
		else
		{
			mn_S2Step = 2600;
		}
		break;

	case 2800:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_OFF) == IO_OFF)
		{
			nRet_1 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				mn_S2Step = 3000;
			}
		}
		else
		{
			nRet_1 = Set_TrayLatchClinder(CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				mn_S2Step = 2900;
			}
		}
		break;

	case 2850:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 2900;
		}
		break;

	case 2900:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_ON);
		if(nRet_1 == CYLINDER_OFF)
		{
			nRet_1 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				mn_S2Step = 3000;
			}
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010207");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1063, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 2850;
		}
		else
		{
			mn_S2Step = 2850;
		}
		break;

	case 2950:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 3000;
		}
		break;

	case 3000:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_S2Step = 3100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010204");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1064, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 2950;
		}
		else
		{
			mn_S2Step = 2950;
		}
		break;

	case 3100:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_fwd_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_bwd_chk, IO_ON) == IO_ON)
		{
			mn_S2Step = 3300;
		}
		else
		{
			mn_S2Step = 3200;
		}
		break;

	case 3200:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 3400;
		}
		break;

	case 3300:
		nRet_1 = Module_Stacker2_MoveMethod(0);
		if(nRet_1 = CTLBD_RET_GOOD)
		{
			Set_TrayLatchClinder(CYLINDER_OFF);
			mn_S2Step = 3400;
		}
		break;

	case 3400:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_S2Step = 3500;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010208");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1065, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 3200;
		}
		else
		{
			mn_S2Step = 3200;
		}
		break;
		
	case 3500:
		nRet_1 = Module_Stacker2_MoveMethod(2);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_S2Step = 3600;
		}
		break;

	case 3600:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			mn_S2Step = 3700;
		}
		else
		{
			sprintf(mc_jamcode, "000201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1066, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 3700:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);

		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_S2Step = 3800;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			mn_S2Step = 3700;
		}
		else
		{
			sprintf(mc_jamcode,"010010");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1067, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 3800:
		FAS_IO.set_out_bit(st_io.o_m_stacker2_up_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_m_stacker2_dn_onoff, IO_ON);
		mn_S2Step = 3900;
		break;

	case 3900:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 4000;
		}
		break;

	case 4000:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_S2Step = 4100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010204");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1068, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 3900;
		}
		else
		{
			mn_S2Step = 3900;
		}
		break;

	case 4100:
		nRet_1 = Set_StackerUpDnClinder(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_S2Step = 4200;
		}
		break;

	case 4200:
		nRet_1 = Get_StackerUpDnClinder(M_LDM_STACKER_2, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_S2Step = 0;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			sprintf(mc_jamcode, "010206");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1069, st_work.mn_run_status, mc_jamcode);
			mn_S2Step = 4100;
		}
		else
		{
			mn_S2Step = 4100;
		}
		break;
	}
}

void CDialog_Part_Manual::Set_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i;

	for (i = 0; i < PICKER_NUM; i++)
	{
		if(FingerInfo[D_EXIST][i] == YES)
		{
			if(PosPlus != 24)
			{
				FAS_IO.set_out_bit(st_io.o_ld_module_glipper_onoff[i], OnOff);
			}
			else if(PosPlus == 24)
			{
				FAS_IO.set_out_bit(st_io.o_ld_module_rbt_glipper1_onoff, OnOff);
			}
		}
		//else
		//{
		//	FAS_IO.set_out_bit(st_io.o_ld_module_glipper_onoff[i], !OnOff);
		//}
		b_FingerFlag[i] = false;
		l_FingWaitTime[i][0] = GetCurrentTime();
	}
}

int CDialog_Part_Manual::Get_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM])
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
					l_FingWaitTime[i][0] = GetCurrentTime();   
				}				
				else if(b_FingerFlag[i] == TRUE)
				{
					l_FingWaitTime[i][1] = GetCurrentTime();  
					l_FingWaitTime[i][2] = l_FingWaitTime[i][1] - l_FingWaitTime[i][0] ;
					
					if(l_FingWaitTime[i][2] > st_wait.n_on_wait_time[39] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_FingWaitTime[i][1] = GetCurrentTime();  
					l_FingWaitTime[i][2] = l_FingWaitTime[i][1] - l_FingWaitTime[i][0];
					if(l_FingWaitTime[i][2] < 0)
					{
						l_FingWaitTime[i][0] = GetCurrentTime();
					}
					
					if(l_FingWaitTime[i][2] > st_wait.n_limit_wait_time[39] + (i*2))
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
					l_FingWaitTime[i][0] = GetCurrentTime();   
				}				
				else if(b_FingerFlag[i] == TRUE)
				{
					l_FingWaitTime[i][1] = GetCurrentTime();  
					l_FingWaitTime[i][2] = l_FingWaitTime[i][1] - l_FingWaitTime[i][0];
					
					if(l_FingWaitTime[i][2] > st_wait.n_on_wait_time[39] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_FingWaitTime[i][1] = GetCurrentTime();  
					l_FingWaitTime[i][2] = l_FingWaitTime[i][1] - l_FingWaitTime[i][0];
					if(l_FingWaitTime[i][2] < 0)
					{
						l_FingWaitTime[i][0] = GetCurrentTime();
					}
					
					if(l_FingWaitTime[i][2] > st_wait.n_limit_wait_time[39] + (i*2))
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

void CDialog_Part_Manual::Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i;

	for (i = 0; i < PICKER_NUM; i++)
	{
		if(PickerInfo[D_EXIST][i] == YES)
		{
			if(PosPlus != 24)
			{
				FAS_IO.set_out_bit(st_io.o_ld_module_picker_updn[i], OnOff);
			}
			else if(PosPlus == 24)
			{
				FAS_IO.set_out_bit(st_io.o_ld_module_rbt_picker1_updn, OnOff);
			}
		}
		//else
		//{
		//	FAS_IO.set_out_bit(st_io.o_ld_module_picker_updn[i], !OnOff);
		//}
		b_PickFlag[i] = FALSE;
		l_PickWaitTime[i][0] = GetCurrentTime();
	}
}

int CDialog_Part_Manual::Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i, FuncRet = RET_PROCEED;
	int RetVal[PICKER_NUM] = {0,};
	char jamcode[10] = {0,};
	CString str;
	
	if(st_basic.n_mode_device == 0 || st_basic.n_mode_device == 2) // without mode
	{
		for(i = 0 ; i < PICKER_NUM ; i++) 
		{
			RetVal[i] = YES;
		}
		return RET_GOOD;
	}

	for(i = 0 ; i < PICKER_NUM ; i++)
	{
		RetVal[i] = NO;

		if(OnOff == IO_ON) //Picker ON (Down이라면..)
		{
			if(PickerInfo[D_EXIST][i] == YES) //동작 시킨 피커면!
			{
				if(b_PickFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_ON) == IO_ON
					&& FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[i], IO_OFF) == IO_OFF)
				{
					b_PickFlag[i] = TRUE;
					l_PickWaitTime[i][0] = GetCurrentTime();   
				}
				else if(b_PickFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_ON) == IO_ON
					&& FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[i], IO_OFF) == IO_OFF)
				{
					l_PickWaitTime[i][1] = GetCurrentTime();  
					l_PickWaitTime[i][2] = l_PickWaitTime[i][1] - l_PickWaitTime[i][0];
					
					if(l_PickWaitTime[i][2] < 0)
					{
						l_PickWaitTime[i][0] = GetCurrentTime();
					}
					if(l_PickWaitTime[i][2] > st_wait.n_on_wait_time[38] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_PickWaitTime[i][1] = GetCurrentTime();  
					l_PickWaitTime[i][2] = l_PickWaitTime[i][1] - l_PickWaitTime[i][0];
					if(l_PickWaitTime[i][2] < 0)
					{
						l_PickWaitTime[i][0] = GetCurrentTime();
					}
					
					if(l_PickWaitTime[i][2] > st_wait.n_limit_wait_time[38] + (i*2))
					{
						//400200 1 40 "Module Picker #1 Down Check Error."						
						sprintf(COMI.mc_alarmcode, "40020%d", i);
						sprintf(mc_alarmcode, "40020%d", i);						
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
				if(b_PickFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_OFF) == IO_OFF
					&& FAS_IO.Get_In_Bit(st_io.i_ld_module_picker_up_chk[i]) == IO_ON)
				{
					b_PickFlag[i] = TRUE;
					l_PickWaitTime[i][0] = GetCurrentTime();   
				}
				else if(b_PickFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_OFF) == IO_OFF
					&& FAS_IO.Get_In_Bit(st_io.i_ld_module_picker_up_chk[i]) == IO_ON)
				{
					l_PickWaitTime[i][1] = GetCurrentTime();  
					l_PickWaitTime[i][2] = l_PickWaitTime[i][1] - l_PickWaitTime[i][0];
					
					if(l_PickWaitTime[i][2] > st_wait.n_on_wait_time[38] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_PickWaitTime[i][1] = GetCurrentTime();  
					l_PickWaitTime[i][2] = l_PickWaitTime[i][1] - l_PickWaitTime[i][0];
					if(l_PickWaitTime[i][2] < 0)
					{
						l_PickWaitTime[i][0] = GetCurrentTime();
					}
					
					if(l_PickWaitTime[i][2] > st_wait.n_limit_wait_time[38] + (i*2))
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
						sprintf(mc_alarmcode,  "40010%d", i);					
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

int CDialog_Part_Manual::Robot_TrayDVCPick(int n_bin, int n_tray_y)
{
	int nRet_1, i=0, y=0, num, FuncRet, ActionFlag = NO, nPick = 0;
	

	FuncRet = RET_PROCEED;

	switch(Tray_Pick_Step)
	{
	case 0:
		mn_FirstPicker_X_Pos = -1;
		i_DVCRepickCnt = 0;

		l_until_wait_time[0] = GetCurrentTime();
		Tray_Pick_Step = 1000;
		break;

	case 1000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_M_RBT_Z);
		if(md_CurrentPos > st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_RBT_Z].mn_allow)
		{
			Tray_Pick_Step = 1100; //Z축을 안전 위치로 든다 
		}
		else
		{
			Tray_Pick_Step = 2000;
		}
		break; 

	case 1100:
 		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		
		if(nRet_1 == BD_GOOD)
		{
			Tray_Pick_Step = 2000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 1100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 1100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1141, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2000:	// 현재 피커의 상태를 파악하고 , 문제가 되면 로보트 이동전 피커의 상태를 체크한다 .
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerModuleStatusCheck(PICKERSTATUS, st_modulemap.nModuleRobotPicker, mResult);
		
		if(nRet_1 == RET_GOOD)
		{
			Tray_Pick_Step = 3000;
		}
		else if(nRet_1 == RET_ERROR)	// 피커 상태 에러 
		{
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME)
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				}
				break;
			}
			st_work.mn_run_status = CTL_dWARNING;
		}
		break;

	//트레이 정보 및 피커 정보를 비교하여 로보트가 이동할 위치를 찾는다 
	case 3000:
		mn_FirstPicker_Y_Pos = -1; mn_FirstTray_X_Pos = -1; mn_FirstTray_Y_Pos = -1;
		PickerAction[D_EXIST][i] = YES;
		Tray_Pick_Step = 3100;
		break;	

	case 3100: //로보트 피치 모터를 트레이에서 집을 수 있도록 이동한다 
		md_CurrentPos = COMI.Get_MotCurrentPos(M_M_RBT_Z);
		if(md_CurrentPos > st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_RBT_Z].mn_allow)
		{
			Tray_Pick_Step = 3110; //Z축을 안전 위치로 든다 
		}
		else
		{
			Tray_Pick_Step = 3200;
		}
		break; 

	case 3110:
 		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		
		if(nRet_1 == BD_GOOD)
		{
			Tray_Pick_Step = 3200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 3110;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 3110;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1143, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3200:
	//로보트를 작업 할 위치로 이동한다 
		//2012,0905
		md_Robot_Pos = Func.XYRobot_MovePosValCheck(LDMODULE_SITE, 0, st_work.i_FirstPickerNum[LDMODULE_SITE], mCurrent_RowPos[LDMODULE_SITE]);
		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Y, md_Robot_Pos, st_basic.nRunSpeed);
	
		if(nRet_1 == BD_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Tray_Pick_Step = 3300;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			Tray_Pick_Step = 3200;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Tray_Pick_Step = 3200;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1144, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
	
	case 3300: // 가능한 피커를 내리고 피킹하는 동작 
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerModuleStatusCheck(PICKERSTATUS, st_modulemap.nModuleRobotPicker, mResult); // 피커 정보와 실제 센서 상태를 체크 
		
		if(nRet_1 == RET_ERROR)
		{
			//2010,0531
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				}
				break;
			}
			st_work.mn_run_status = dWARNING;
			break;
		}

		Tray_Pick_Step = 4000;		
		break;

	case 4000: // moving after finger open
		Set_FingerOnOff(IO_ON, PickerAction);
		Tray_Pick_Step = 4010;
		break;

	case 4010:
		nRet_1 = Get_FingerOnOff(IO_ON, PickerAction);

		if(nRet_1 == RET_GOOD)
		{
			Tray_Pick_Step = 4100;
		}
		else if(nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			Tray_Pick_Step = 4000;
		}
		break;		

	case 4100:
		nRet_1 = Robot_CurrentPos_Check(md_Robot_Pos, -1);
		if(nRet_1 == CTL_YES) //현재 로보트 위치가 목표 위치가 아니다
		{
			Tray_Pick_Step = 0; //다시 처음 부터 시작 한다 
		}
		else
		{
			Tray_Pick_Step = 4110;
		}
		break;

	case 4110:
		Set_PickerUpDown(IO_ON, PickerAction);

 		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_TRAY_PICK], st_basic.nRunSpeed);
		
		if(nRet_1 == BD_GOOD)
		{
			Tray_Pick_Step = 4200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 4110;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 4110;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(0, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4200: //picker down action check
		nRet_1 = Get_PickerUpDown(IO_ON, PickerAction);

		if(nRet_1 == RET_GOOD)
		{
			Tray_Pick_Step = 4300;
		}
		else if(nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1145, st_work.mn_run_status, COMI.mc_alarmcode);
			Tray_Pick_Step = 4100;
		}
		break;

	case 4300: //Pick 디바이스 클램프 
		Set_FingerOnOff(IO_OFF, PickerAction);
		Tray_Pick_Step = 4310;
		break;

	case 4310:
		nRet_1 = Get_FingerOnOff(IO_OFF, PickerAction);
		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Tray_Pick_Step = 4400;
		}
		else if(nRet_1 == RET_ERROR)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Tray_Pick_Step = 4300;
		}
		break;	

	case 4400: //디바이스 존재유무 체크 
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerModuleStatusCheck(IO_ON, PickerAction, mResult); // 피커 정보와 실제 센서 상태를 체크 

		if(nRet_1 == RET_GOOD)
		{
			Tray_Pick_Step = 4500;	
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(l_until_wait_time[2] < (PICKERSTATUS_CHECK_TIME*2)) //자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				}
				break;
			}

			for(i = 0 ; i < PICKER_NUM ; i++)
			{
				mTempPicker[D_EXIST][i] = NO;

				if(PickerAction[D_EXIST][i] == YES && mResult[i] == NO)
				{	
					//문제 발생시 에러시 해당 포트만 오픈 시키자 
					mTempPicker[D_EXIST][i] = YES;
				}
			}
 			Tray_Pick_Step = 4500;	 // 현재 핑거 체크한는데 문제가 있어 벌리면 안된다 
		}
		break;
 
	case 4500: // 모터 업 and picker up
		Set_PickerUpDown(IO_OFF, PickerAction);
		if(st_basic.n_mode_device == 1 && st_basic.n_mode_m_direction == YES)
		{
			Tray_Pick_Step = 4510;
		}
		else
		{
 			nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		
			if(nRet_1 == BD_GOOD)
			{
				Tray_Pick_Step = 4600;
			}
			else if (nRet_1 == BD_RETRY)
			{
				Tray_Pick_Step = 4500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				Tray_Pick_Step = 4500;
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(alarm.mn_count_mode, st_work.mn_run_status, COMI.mc_alarmcode);
			}
		}
		break;

	case 4510:
 		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_DIRECTION_CHECK], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Tray_Pick_Step = 4520;
		}
		else if (nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 4510;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 4510;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(alarm.mn_count_mode, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4520:
		nRet_1 = Get_PickerUpDown(IO_OFF, PickerAction);

		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Tray_Pick_Step = 4530;
		}
		else if(nRet_1 == RET_ERROR)
		{
			Tray_Pick_Step = 4500;
			st_work.mn_run_status = dWARNING;
		}
		break;

	case 4530:
		if(st_basic.n_mode_device == 1 &&st_basic.n_mode_m_direction == YES)
		{
			if(FAS_IO.get_in_bit(st_io.i_ld_module_rbt_dvc_direction_chk, IO_ON) == IO_ON)
			{
				Tray_Pick_Step = 4540;
			}
			else
			{//400700 1 40 "Module Direction Error"
				l_until_wait_time[1] = GetCurrentTime();
				l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
				if(l_until_wait_time[2] < 0) l_until_wait_time[0] = GetCurrentTime();

				nRet_1 = Func.PickerModuleStatusCheck(IO_ON, PickerAction, mResult); // 피커 정보와 실제 센서 상태를 체크 
				
				if(nRet_1 == RET_ERROR) //
				{
					if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //자재를 체크할 수 있는 안정 시간을 준다  
					{						 //값이 - 값이면 시간 초기화하여 다시 체크한다 
						break;
					}
					Tray_Pick_Step = 4540;
				}
				else
				{
					if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME)	break;
					Tray_Pick_Step = 4535;
				}
			}
		}
		else
		{
			Tray_Pick_Step = 4540;
		}
		break;

	case 4535:
		nRet_1 = Robot_ModuleDVCTrayPlace(mCurrent_RowPos[LDMODULE_SITE], PickerAction);
		if(nRet_1 == RET_GOOD)
		{
			sprintf(mc_alarmcode, "400700");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(alarm.mn_count_mode, st_work.mn_run_status, mc_alarmcode);
			Tray_Pick_Step = 0;
		}
		break;

	case 4540:
 		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
	
		if(nRet_1 == BD_GOOD)
		{
			Tray_Pick_Step = 4600;
		}
		else if (nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 4500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 4500;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(alarm.mn_count_mode, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;


	case 4600: //picker up action check
		nRet_1 = Get_PickerUpDown(IO_OFF, PickerAction);

		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Tray_Pick_Step = 5000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			Tray_Pick_Step = 4500;
			st_work.mn_run_status = dWARNING;
		}
		break;

	//////////////////////////////////////////////////////////
	// 작업한 피커 및 트레이 정보를 셋팅한다 
	//////////////////////////////////////////////////////////
	case 5000:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		
		nRet_1 = Func.PickerModuleStatusCheck(IO_ON, PickerAction, mResult); // 피커 정보와 실제 센서 상태를 체크 
		
		if(nRet_1 == RET_ERROR) //
		{
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[2] < 0) l_until_wait_time[0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				break;
			}
		}
		
		for(i = 0 ; i < PICKER_NUM ; i++) 
		{
			if(PickerAction[D_EXIST][i] == YES && mResult[i] == NO)
			{	
				//문제 발생시 에러시 해당 포트만 오픈 시키자 
				mTempPicker[D_EXIST][i] = YES;
			}
			if(PickerAction[D_EXIST][i] == YES && mResult[i] == YES) // 디바이스가 있는 피커만 체크
			{
				num = i * PICKING_GAP;
				
				//st_modulemap.LoadTray[1][mn_FirstTray_Y_Pos][mn_FirstTray_X_Pos + (num - (mn_FirstPicker_X_Pos * PICKING_GAP))] = NO; //트레이에 디바이스가 없음을 셋
				st_modulemap.LoadTray[1][mn_FirstTray_Y_Pos + (num - (mn_FirstPicker_Y_Pos * PICKING_GAP))] = NO; //트레이에 디바이스가 없음을 셋

				st_modulemap.nModuleRobotPicker[D_EXIST][i] = DVC_YES;
				st_modulemap.nModuleRobotPicker[D_BIN][i] = BIN_LDBUFFERBIN;		// BIN_LOADTRAYBIN
				st_modulemap.nModuleRobotPicker[D_RE_CNT][i] = T_NOTTEST;			// Test Count Reset
				st_modulemap.nModuleRobotPicker[D_SCR_BIN][i] = BIN_CLEAR;			// Screen Bin
				
//				if(st_basic.n_mode_barcode == TRUE)
//				{
//					//st_modulemap.BufferLeft[st_modulemap.CurrentRows[BUFFERTRAY1] + (num - (st_work.i_FirstPickerNum[BUFFERTRAY1] * PICKING_GAP))] = st_modulemap.strBcr_DeviceInRobotPicker[i];
//					st_modulemap.strBcr_DeviceInRobotPicker[i] = _T("NO CODE");
//				}

				PickerAction[0][i] = NO;  
				mResult[i] = NO;
			}

		}

		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_LOADER_PICKER);			// 피커 정보 
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_LOADER_TRAY);			// 피커 정보 
		}
		num = Func.Find_Picker_Count(LDMODULE_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
		if(st_work.nMdlInputCount[0][0] <= st_work.n_loadlot_count[LDMODULE_SITE] + num)
		{
			nRet_1 = RET_GOOD;
		}
		if(nRet_1 == RET_GOOD)
		{
			Tray_Pick_Step = 6000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			i_DVCRepickCnt++;

			if(i_DVCRepickCnt >= st_basic.mn_Dvc_Repick_Cnt) //20120730
			{
				i_DVCRepickCnt = 0;		//피킹 재동작의 카운트는 초기화 되는게 맞음!
				i_PartialPickCnt++;

				//if(i_PartialPickCnt >= 5)
				if(Func.Find_TrayInfo_Count(LDMODULE_SITE, CTL_YES, -1) <= 0)
				{	
					i_PartialPickCnt = 0;
					Func.OnLoadModule_Tray_Clear();			// Loader Tray에 Module이 없슴을 화면에 보여준다.
					FuncRet = RET_NOWHERE;				// Tray에 Device가 없다고 강제 세팅하고, 다음 Tray를 요청한다.
					Tray_Pick_Step = 0;
					break;
				}			
				else
				{	
					//해당 트레이 정보를 클리어 시키고 계속 진행. 
					for(i = 0 ; i < PICKER_NUM ; i++) 
					{
						num = i * PICKING_GAP;
						
						if(PickerAction[0][i] == YES && mResult[i] == NO) // 동작했는데 디바이스가 없는 피커만 체크
						{
							if(mTempPicker[D_EXIST][i] == YES)
							{
								//st_modulemap.LoadTray[1][mn_FirstTray_Y_Pos][mn_FirstTray_X_Pos + (num - (mn_FirstPicker_X_Pos * PICKING_GAP))]  = NO; //트레이에 디바이스가 없음을 셋
								st_modulemap.LoadTray[1][mn_FirstTray_Y_Pos + (num - (mn_FirstPicker_Y_Pos * PICKING_GAP))]  = NO; //트레이에 디바이스가 없음을 셋
							}
						}
					}
					
					if(st_handler.cwnd_main != NULL)
					{
						st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOADINFO, 0);  //로더 정보 
					}

					Tray_Pick_Step = 1000;
				}
			}
			else 
			{
				Tray_Pick_Step = 1000;
			}
		}
		break;

	case 6000: //정상적으로 모두 잡았으면 
		nRet_1 = Func.Find_Picker_Count(LDMODULE_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); //피커에는 디바이스 몇개가 없냐? 

		//자재를 한번이라도 집은 뒤부터 loader off가 되도록 하자.. (로더/언로더 카세트 및 관련 동작 flow를 맞추기 위함)
		if(st_work.i_robot_device_loading != TRUE)
		{
			st_work.i_robot_device_loading = TRUE;
		}

		Tray_Pick_Step = 0;
		FuncRet = RET_GOOD;			
		break;
	}

	return FuncRet;
}

int CDialog_Part_Manual::Robot_CurrentPos_Check(double y_pos, double z_pos)
{
	double dCurPos[4]; //x,y,z,p
	int nFlag = CTL_NO;

	dCurPos[0] = COMI.Get_MotCurrentPos(M_M_RBT_Y);
	dCurPos[1] = COMI.Get_MotCurrentPos(M_M_RBT_Z);

	if ((y_pos < ((double)dCurPos[0] - (double)(st_motor[M_M_RBT_Y].mn_allow))) 
		|| (y_pos > ((double)dCurPos[0] + (double)(st_motor[M_M_RBT_Y].mn_allow))))
	{
		nFlag = CTL_YES;
	}
//	else if (z_pos != -1 && (z_pos < ((double)dCurPos[2] - (double)(st_motor[M_M_RBT_Z].mn_allow))) 
//		|| (z_pos > ((double)dCurPos[2] + (double)(st_motor[M_M_RBT_Z].mn_allow))))
//	{
//		nFlag = CTL_YES;
//	}
	

	return nFlag;
	//nFlag => YES:위치가 맞지않다, NO:현재 위치와 목표우치가 일치한다 
}

int CDialog_Part_Manual::Robot_ModuleDVCTrayPlace(int nRow, int ActionPicker[D_INFOSIZE][PICKER_NUM])
{
	int nRet_1;
	int nFuncRet = RET_PROCEED;
	double md_CurrentPos;

	switch(TrayPlaceStep)
	{
	case 0:
		l_until_wait_time[0] = GetCurrentTime();
		TrayPlaceStep = 1000;
		break;

	case 1000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_M_RBT_Z);
		if(md_CurrentPos > st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_RBT_Z].mn_allow)
		{
			TrayPlaceStep = 1100; //Z축을 안전 위치로 든다 
		}
		else
		{
			TrayPlaceStep = 2000;
		}
		break; 

	case 1100:
 		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);		
		if(nRet_1 == BD_GOOD)
		{
			TrayPlaceStep = 2000;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == BD_RETRY)
		{
			TrayPlaceStep = 1100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1990, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlaceStep = 1100;
		}
		break;

	case 2000:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerModuleStatusCheck(PICKERSTATUS, ActionPicker, mResult);
		
		if(nRet_1 == RET_GOOD)
		{
			TrayPlaceStep = 3000;
		}
		else if(nRet_1 == RET_ERROR)	// 피커 상태 에러 
		{	
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME)
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				}
				break;
			}

			st_work.mn_run_status = dWARNING;
		}
		break;	

	case 3000:
		md_Robot_TrayPlacePos = Func.XYRobot_MovePosValCheck(LDMODULE_SITE, 0, 0, nRow);
		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Y, md_Robot_TrayPlacePos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			TrayPlaceStep = 3100;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == BD_RETRY)
		{
			TrayPlaceStep = 3000;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1991, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlaceStep = 3000;
		}
		break;

	case 3100:
		Set_PickerUpDown(IO_ON, ActionPicker);
		nRet_1 =  CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_TRAY_PICK] - st_motor[M_M_RBT_Z].mn_allow, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			TrayPlaceStep = 3200;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == BD_RETRY)
		{
			TrayPlaceStep = 3100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1992, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlaceStep = 3100;
		}
		break;

	case 3200:
		nRet_1 = Get_PickerUpDown(IO_ON, ActionPicker);
		if (nRet_1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			l_until_wait_time[0] = GetCurrentTime();
			TrayPlaceStep = 3300;
		}
		else if (nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			CTL_Lib.Alarm_Error_Occurrence(1993, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 3300:
		Set_FingerOnOff(IO_ON, ActionPicker);
		TrayPlaceStep = 3400;
		break;

	case 3400:
		nRet_1 = Get_FingerOnOff(IO_ON, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			TrayPlaceStep = 3500;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(1994, CTL_dWARNING, alarm.mc_code);
		}
		break;

	case 3500:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		if(l_until_wait_time[2] < 0) break;
		if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) break;
		Set_PickerUpDown(IO_OFF, ActionPicker);
		nRet_1 =  CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			TrayPlaceStep = 3600;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == BD_RETRY)
		{
			TrayPlaceStep = 3500;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1995, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlaceStep = 3500;
		}
		break;

	case 3510:
		Set_PickerUpDown(IO_OFF, ActionPicker);
		l_until_wait_time[0] = GetCurrentTime();
		TrayPlaceStep = 3600;
		break;

	case 3600:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		if (nRet_1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			l_until_wait_time[0] = GetCurrentTime();
			TrayPlaceStep = 3700;
		}
		else if (nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			CTL_Lib.Alarm_Error_Occurrence(1996, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 3700:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		
		nRet_1 = Func.PickerModuleStatusCheck(IO_OFF, ActionPicker, mResult); // 피커 정보와 실제 센서 상태를 체크 
		
		if(nRet_1 == RET_ERROR)
		{
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
					break;
				}
				CTL_Lib.Alarm_Error_Occurrence(1997, CTL_dWARNING, alarm.mc_code);
			}
			TrayPlaceStep = 3510;
		}
		else
		{
			TrayPlaceStep = 4000;
		}
		break;

	case 4000:
		nFuncRet = RET_GOOD;
		TrayPlaceStep = 0;
		break;
	}

	return nFuncRet;

}

int CDialog_Part_Manual::ClipStackerInit()
{
	int nRet_1 = CTLBD_RET_PROCEED;
	int nRet_2 = CTLBD_RET_PROCEED;
	int nRet_3 = CTLBD_RET_PROCEED;
	int ClipStackerFlag = CTLBD_RET_PROCEED;

	switch(ClipStep)
	{
	case 0:
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_OFF ||
			FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_ON)
		{
			if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_ON)
			{
				FAS_IO.set_out_bit(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
				FAS_IO.set_out_bit(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
				ClipStep = 100;
			}
			else
			{
				ClipStep = 100;
			}
		}
		else
		{
			ClipStep = 100;
		}
		break;

	case 100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF); // "로드 튜브에 튜브 체크
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF); 
		nRet_3 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON)
		{
			if(nRet_1 == IO_ON)
			{	
				sprintf(mc_alarmcode,"180106");
			}
			else if(nRet_2 == IO_ON)//언로드 튜브에 튜브 체크
			{
				sprintf(mc_alarmcode,"180608");
			} 
			else					//로드 튜브 rotator에 튜브 체크
			{
				sprintf(mc_alarmcode,"180508");
			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1500, st_work.mn_run_status, mc_alarmcode);			
		}
		else
		{
			ClipStep = 200;
		}
		break;

	case 200:
		if(FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_fwd_chk, IO_ON) == IO_OFF || // tube latch가 bwd 상태가 아니면
			FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_bwd_chk, IO_OFF) == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_OFF);  // 실린더 off  Latch bwd상태로.
			ClipStep = 300;
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_ON &&  // tube plate fwd on, bwd off 체크
				FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_OFF)
			{
				if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON && // tube plate short dn, long dn On 체크
					FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_dn_chk, IO_ON) == IO_ON)
				{
					ClipStep = 500;
				}
				else
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF); // 실린더 long updn Io_Off 
					FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF); // 실린더 short updn Io_Off
					ClipStep = 400;
				}
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);// 실린더 long updn Io_Off 
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);// 실린더 short updn Io_Off
				ClipStep = 400;
			}
		}
		break;

	case 250:
		FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_OFF); // 실린더 off  Latch bwd상태로.
		ClipStep = 300;
		break;
		
	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_ld_tube_s_latch_fwdbwd_onoff, st_io.i_ld_tube_s_latch_fwd_chk, st_io.i_ld_tube_s_latch_bwd_chk,
			IO_ON, st_wait.n_on_wait_time[54], st_wait.n_limit_wait_time[54]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 200;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)	//"로드 튜브 stacker latch On 에러가 발생."
		{	
			sprintf(mc_alarmcode,"180100");
			CTL_Lib.Alarm_Error_Occurrence(1501, CTL_dWARNING, mc_alarmcode);
			ClipStep = 250;
		}
		else
		{
			ClipStep = 250;
		}
		break;

	case 350:		
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);// 실린더 long updn Io_Off 
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);// 실린더 short updn Io_Off
		ClipStep = 400;
		break;
		
	case 400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_long_updn_onoff, st_io.i_tube_s_m_plate_long_dn_chk, st_io.i_tube_s_m_plate_long_up_chk,
			IO_ON, st_wait.n_on_wait_time[58], st_wait.n_limit_wait_time[58]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_dn_chk, st_io.i_tube_s_m_plate_short_up_chk,
			IO_ON, st_wait.n_on_wait_time[57], st_wait.n_limit_wait_time[57]);
		if(nRet_1 == CTLBD_RET_GOOD && nRet_2 == CTLBD_RET_GOOD)
		{
			ClipStep = 500;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED && nRet_2 != CTLBD_RET_PROCEED)
		{
			if(nRet_1 == RET_ERROR)
			{	
				sprintf(mc_alarmcode,"180401"); // 로드 튜브 stacker plate long down 체크 에러
			}
			else
			{
				sprintf(mc_alarmcode,"180301");	// 로드 튜브 stacker plate short down 체크 에러
				CTL_Lib.Alarm_Error_Occurrence(1502, CTL_dWARNING, mc_alarmcode);
				ClipStep = 350;
			}
		}
		break;

	case 500:
		if(FAS_IO.get_in_bit(st_io.i_tube_push_fwd_chk, IO_ON) == IO_ON || //  tube push fwd Io on, bwd Io off
			FAS_IO.get_in_bit(st_io.i_tube_push_bwd_chk, IO_OFF) == IO_OFF)
		{
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF); //pusher fwdbwd onoff Io_off
			ClipStep = 600;
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_uld_tube_s_dn_chk, IO_ON) == IO_ON &&  // 언로더 tube stacker dn Io_on, up Io_off
				FAS_IO.get_in_bit(st_io.i_uld_tube_s_up_chk, IO_OFF) == IO_OFF)
			{
				ClipStep = 700;
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF); // 언로더 tube stacker updn_onoff Io_off
				ClipStep = 1500;
			}
		}
		break;

	case 550:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF); // 언로더 tube stacker updn_onoff Io_off
		ClipStep = 500;
		break;
		
	case 600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_bwd_chk, st_io.i_tube_push_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[66], st_wait.n_limit_wait_time[66]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 200;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180901");//튜브 Pusher backward 체크 에러
			CTL_Lib.Alarm_Error_Occurrence(1503, CTL_dWARNING, mc_alarmcode);
			ClipStep = 550;
		}
		else
		{
			ClipStep = 550;
		}
		break;

	case 700:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON) // tube rotator latch bwd Io_off, fwd Io_on
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON); // tube stacker plate fwd onoff Io_on
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF); // tube stacker plate bwd onoff Io_off
			ClipStep = 900;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON); // 실린더 tube rotator latch fwd Io_on
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF); // 실린더 tube rotator latch bwd Io_off
			ClipStep = 800; 
		}
		break;

	case 750:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON); // 실린더 tube rotator_latch fwd Io_on
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF); // 실린더 tube rotator_latch bwd Io_off
		ClipStep = 800;
		break;
		
	case 800:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 700;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180700"); //로테이터 튜브 latch On 에러
			CTL_Lib.Alarm_Error_Occurrence(1548, CTL_dWARNING, mc_alarmcode);
			ClipStep = 750;
		}
		else
		{
			ClipStep = 750;
		}
		break;

	case 900:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_bwd_onoff, st_io.i_tube_s_m_plate_fwd_chk, st_io.i_tube_s_m_plate_bwd_chk,
			IO_ON, st_wait.n_on_wait_time[56], st_wait.n_limit_wait_time[56]);
		if(nRet_1 == RET_GOOD)
		{
			ClipStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180201");//로드 튜브 stacker plate backward 에러
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1505, st_work.mn_run_status, mc_alarmcode);
			ClipStep = 700;
		}
		break;

	case 1000:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, IO_ON); // tube rotator latch fwd onoff Io_on 확인
		nRet_2 = FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF); // tube rotator latch bwd onoff Io_off 확인
		if(nRet_1 == IO_ON && nRet_2 == IO_OFF)  // latch fwd가 Io_on 이고 latch bwd가 Io_off 이면
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF); // 실린더 cap 제거 onoff Io_off
			ClipStep = 1200;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF); // 실린더 latch bwd onoff Io_off
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON); // 실린더 latch fwd onoff Io_on
			ClipStep = 1100;
		}
		break;

	case 1050:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON); // 실린더 latch bwd onoff Io_on
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF); // 실린더 latch fwd onoff Io_off
		ClipStep = 1100;
		break;
		
	case 1100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_bwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON,st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 1000;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180700");//로테이터 튜브 latch On 에러
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1506, st_work.mn_run_status, mc_alarmcode);
			ClipStep = 1050;
		}
		else
		{
			ClipStep = 1050;
		}
		break;

	case 1150:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF); // tube rotator cap 제거 onoff Io_off
		ClipStep = 1200;
		break;
		
	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
		if(nRet_1 = CTLBD_RET_GOOD)
		{
			ClipStep = 1300;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180703"); //로테이터 캡 제거 Off 에러
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1507, st_work.mn_run_status, mc_alarmcode);
			ClipStep = 1150;
		}
		else
		{
			ClipStep = 1150;
		}
		break;

	case 1300:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_up_chk, IO_OFF); // clip buffer up chk Io_off
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_dn_chk, IO_ON);  // clip buffer dn chk Io_on
		if (nRet_1 == IO_OFF && nRet_2 == IO_ON) // 클립 버퍼 up 체크 Io가 off이고 클립 버퍼 dn 체크 Io가 on이면.
		{
			ClipStep = 1500;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF); // 실린더 클립 버퍼 updn onoff Io_off
			ClipStep = 1400;
		}
		break;
	
	case 1350:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF); // 실린더 클립 버퍼 updn onoff Io_off
		ClipStep = 1400;
		break;
		
	case 1400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 1300;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180801");//Clip buffer down 체크 에러
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1508, st_work.mn_run_status, mc_alarmcode);
			ClipStep = 1350;
		}
		else
		{
			ClipStep = 1350;
		}
		break;

	case 1500:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_ON); // 튜브 rotator 0도 chk Io_on
		nRet_2 = FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_OFF); // 튜브 rotator 90도 chk Io_off
		if(nRet_1 == IO_ON && nRet_2 == IO_OFF) // 튜브 rotator가 0도 chk가 on이고 90도가 off이면
		{
			ClipStep = 1700;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, IO_ON); // 실린더 튜브 rotator 0도 onoff Io_on
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, IO_OFF); // 실린더 튜브 rotator 90도 onoff Io_off
			ClipStep = 1600;
		}
		break;

	case 1550:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, IO_ON); // 실린더 튜브 rotator 0도 onoff Io_on
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, IO_OFF); // 실린더 튜브 rotator 90도 onoff Io_off
		ClipStep = 1600;
		break;
		
	case 1600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_0_onoff, st_io.i_tube_rotator_0_chk, st_io.i_tube_rotator_90_chk,
			IO_ON, st_wait.n_on_wait_time[59], st_wait.n_limit_wait_time[59]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 1500;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180706");//"로테이터 튜브 0도 에러
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1509, st_work.mn_run_status, mc_alarmcode);
			ClipStep = 1550;
		}
		else
		{
			ClipStep = 1550;
		}
		break;	

	case 1700:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF); // 실린더 tube rotator clamp onoff Io_off
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON); // 실린더 tube rotator unclamp onoff Io_on
		ClipStep = 1800;
		break;

	case 1800:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 1900;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180705");//로테이터 클램프 Off 에러
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1510, st_work.mn_run_status, mc_alarmcode);
			ClipStep = 1700;
		}
		else
		{
			ClipStep = 1700;
		}
		break;
		
	case 1900:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_ON) // rotate tube chk Io_on
		{
			ClipStep = 2000;
		}
		else
		{
			ClipStep = 5000;
		}
		break;

	case 2000:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)  // 튜브 rotator latch bwd onoff Io_off, fwd onoff Io_on이면
		{
			nRet_1 = OnLoadTransferMove(); //
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				ClipStep = 2200;
			}
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON); // latch fwd onoff Io_on
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF); // latch bwd onoff Io_off
			ClipStep = 2100;
		}
		break;

	case 2050:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON); // latch fwd onoff Io_on
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF); // latch bwd onoff Io_off
		ClipStep = 2100;
		break;
		
	case 2100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 2050;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180700"); //로테이터 튜브 latch On 에러
			CTL_Lib.Alarm_Error_Occurrence(1540, CTL_dWARNING, mc_alarmcode);
			ClipStep = 2050;
		}
		else
		{
			ClipStep = 2050;
		}
		break;

	case 2200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);    // 로더 튜브 체크 Io_off
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);   // 언로더 튜브 체크 Io_off
		nRet_3 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_ON); // rotate 튜브 체크 Io_on
		if(st_basic.n_mode_device == 1 && (nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON))
		{
			if(nRet_1 == IO_ON)
			{
				sprintf(mc_alarmcode,"180106");//로드 튜브에 튜브가 체크
			}
			else if(nRet_2 == IO_ON)
			{
				sprintf(mc_alarmcode,"180610");//언로드 튜브에 튜브가 체크
			}
			else
			{
				sprintf(mc_alarmcode,"180509");//로드 튜브 rotator에 튜브가 체크 X
			}
			CTL_Lib.Alarm_Error_Occurrence(1541, CTL_dWARNING, mc_alarmcode);
		}
		else
		{
			ClipStep = 2300;
		}
		break;
		
	case 2300:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_ON); // 언로드클립 튜브 updn onoff Io_on
		ClipStep = 2400;
		break;
		
	case 2400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_up_chk, st_io.i_uldclip_tube_dn_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			ClipStep = 2500;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"900901"); // ULD Clip tube Cover up down Error.
			CTL_Lib.Alarm_Error_Occurrence(1542, CTL_dWARNING, mc_alarmcode);
		}
		break;

	case 2500:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 500)
		{
			FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF); // 언로더클립 튜브 updn onoff Io_off
			ClipStep = 2600;
		}
		break;

	case 2600:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF); // 언로더클립 튜브 updn onoff Io_off
		ClipStep = 2700;
		break;

	case 2700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_dn_chk, st_io.i_uldclip_tube_up_chk, 
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 2800;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(1543, CTL_dWARNING, mc_alarmcode);
			ClipStep = 2600;
		}
		break;

	case 2800:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF); // 로더 튜브 체크 Io_off
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF); // 언로더 튜브 체크 Io_off
		nRet_3 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF); // rotate 튜브 체크 Io_off
		if(nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON) // 로더튜브체크가 On이거나 언로더뷰트체크가 On이거나 Rotate튜브체크가 on이면
		{
			if(nRet_1 == IO_ON)
			{
				sprintf(mc_alarmcode,"180106");//로드 튜브에 튜브가 체크
			}
			else if(nRet_2 == IO_ON)
			{
				sprintf(mc_alarmcode,"180608");//언로드 튜브에 튜브가 체크
			}
			else
			{
				sprintf(mc_alarmcode,"180508");//로드 튜브 rotator에 튜브가 체크
			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1544, st_work.mn_run_status, mc_alarmcode);			
		}
		else
		{
			ClipStep = 2900;
		}
		break;

	case 2900:
		nRet_1 = CheckTransferBeforeMove();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 3000;
		}
		break;

	case 3000:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON) // 튜브 rotator bwd onoff Io_off, fwd onoff Io_on
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON); // 실린더 튜브 스태커 plate fwd onoff Io_on
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF); // 실린더 튜브 스태커 plate fwd onoff Io_off
			ClipStep = 3300;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON); // 실린더 튜브 rotator latch fwd onoff Io_on
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF); // 실린더 튜브 rotator latch bwd onoff Io_off
			ClipStep = 3100;
		}
		break;

	case 3100:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON); // 실린더 튜브 rotator latch fwd onoff Io_on
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF); // 실린더 튜브 rotator latch bwd onoff Io_off
		ClipStep = 3200;
		break;

	case 3200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 3000;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180700");//로테이터 튜브 latch On 에러
			CTL_Lib.Alarm_Error_Occurrence(1547, CTL_dWARNING, mc_alarmcode);
			ClipStep = 3100;
		}
		else
		{
			ClipStep = 3100;
		}
		break;

	case 3300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_bwd_onoff, st_io.i_tube_s_m_plate_fwd_chk, st_io.i_tube_s_m_plate_bwd_chk,
			IO_ON, st_wait.n_on_wait_time[56], st_wait.n_limit_wait_time[56]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipStep = 5000;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180201");//로드 튜브 stacker plate backward 에러
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1597, st_work.mn_run_status, mc_alarmcode);
			ClipStep = 3000;
		}
		break;

	case 5000:
		ClipStep = 0;
		break;
		}

	return ClipStackerFlag;
}

int CDialog_Part_Manual::ClipRbtInit()
{
	int nRet_1 = CTLBD_RET_PROCEED;
	int nRet_2 = CTLBD_RET_PROCEED;
	int ClipRbtFlag = CTLBD_RET_PROCEED;

	switch(ClipRbtFlag)
	{
	case 0:
		if(FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_OFF) == IO_ON ||
			FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON) == IO_OFF) // 클립 로봇 up chk Io_on이거나 클립 로봇 dn chk Io_off 이면
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF); // 실린더 클립 로봇 업다운 Io_off (다운)
			ClipRbtStep = 100;
		}
		else
		{
			ClipRbtStep = 200;
		}
		break;

	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipRbtStep = 200;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"260100");//Clip Insert Picker up 에러
			CTL_Lib.Alarm_Error_Occurrence(2300, CTL_dWARNING, mc_alarmcode);
			ClipRbtStep = 0;
		}
		break;
		
	case 200:
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].mn_homecheck_method, MOT_TIMEOUT); // 클립버퍼 홈체크 
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipRbtStep = 300;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipRbtStep = 100;
		}
		else if((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(2302, CTL_dWARNING, COMI.mc_alarmcode);	
		}
		break;

	case 300:
		ActionPicker[D_EXIST] = YES;
		Set_PickerUpDown(IO_OFF, ActionPicker);
		ClipRbtStep = 400;
		break;

	case 400:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		nRet_1 = CTLBD_RET_GOOD;
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			ClipRbtStep = 500;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			sprintf(mc_alarmcode,"260100");
			CTL_Lib.Alarm_Error_Occurrence(2303, CTL_dWARNING, mc_alarmcode);
		}
		break;

	case 500:
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].mn_homecheck_method, MOT_TIMEOUT); // 클립 Insert Z축 홈체크
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipRbtStep = 600;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipRbtStep = 500;
		}
		else if((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2304, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 600:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF); // 실린더 클립버퍼 업다운 onoff Io_off (다운)
		ClipRbtStep = 700;
		break;

	case 700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipRbtStep = 800;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			sprintf(mc_alarmcode,"180801");//Clip buffer down 체크 에러
			CTL_Lib.Alarm_Error_Occurrence(2305, CTL_dWARNING, mc_alarmcode);		
		}
		break;

	case 800:
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].mn_homecheck_method, MOT_TIMEOUT); // 클립 Insert Y축 홈체크
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipRbtStep = 900;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipRbtStep = 800;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2306, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 900:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].mn_homecheck_method, MOT_TIMEOUT); // 클립 Clamp 로봇 그리퍼 홈체크	
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipRbtStep = 1000;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipRbtStep = 900;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2307, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1000:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].mn_homecheck_method, MOT_TIMEOUT); // Clamp 로봇 Z축 홈체크
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipRbtStep = 1100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipRbtStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2308, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1100:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].mn_homecheck_method, MOT_TIMEOUT); // Clamp 로봇 Y축 홈체크
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipRbtStep = 1200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipRbtStep = 1100;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);	
		}
		break;

	case 1200:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE1], st_basic.nManualSpeed); // Clamp 로봇 Y축 Clamp Work Place1으로 이동.	
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipRbtStep = 1300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipRbtStep = 1200;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1300:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].mn_homecheck_method, MOT_TIMEOUT); // 클립 Clamp 홈체크		
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipRbtStep = 1400;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipRbtStep = 1300;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1400:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF); // 클립 버퍼 클립1 chk Io_off 확인
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF); // 클립 버퍼 클립2 chk Io_off 확인
		if(nRet_1 == IO_ON || nRet_2 == IO_ON) // 클립버퍼 클립1이나 클립2가 Io_on이면
		{
			ClipRbtStep = 1500;
		}
		else
		{
			ClipRbtStep = 1800;
		}
		break;

	case 1500:
//		nRet_1 = UnloadClipMove();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipRbtStep = 1600;
		}
		break;
		
	case 1600:
//		nRet_1 = PickUpClipForBin();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipRbtStep = 1700;
		}
		break;

	case 1700:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF);
		
		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{
			ClipRbtStep = 1800;
		}
		else
		{
			if(nRet_1 == IO_ON)
			{
				sprintf(mc_alarmcode,"180804");//Clip buffer clip#1을 제거
			}
			else
			{
				sprintf(mc_alarmcode,"180805");//Clip buffer clip#2을 제거
			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, st_work.mn_run_status, mc_alarmcode);
			ClipRbtStep = 1400;
		}
		break;

	case 1800:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_PICKPOS], st_basic.nManualSpeed); // 클립 Insert Y축을 Y_Clip_pickpos로 이동
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipRbtStep = 3000;
			ClipRbtStep = 1900;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipRbtStep = 1200;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1900:
//		nRet_1 = Run_RemoveClip();
		if(nRet_1 == RET_GOOD)
		{
			ClipRbtStep = 3000;
		}
		break;

	case 3000:
		ClipRbtStep = 0;
		break;
	}

	return ClipRbtFlag;
}


int CDialog_Part_Manual::ClipPlaceInit()
{
	int nRet_1 = CTLBD_RET_PROCEED;
	int nRet_2 = CTLBD_RET_PROCEED;
	int ClipFlag = CTLBD_RET_PROCEED;

	nRet_1 = ClipStackerInit();
	nRet_2 = ClipRbtInit();

	if(nRet_1 == CTLBD_RET_GOOD && nRet_2 == CTLBD_RET_GOOD)
	{
		ClipFlag = CTLBD_RET_GOOD;
	}
	else
	{
		ClipFlag = CTLBD_RET_ERROR;
	}

	return ClipFlag;
}
