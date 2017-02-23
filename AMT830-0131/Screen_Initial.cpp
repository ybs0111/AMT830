// Screen_Initial.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Initial.h"

// ******************************************************************************
// 대화 상자 클래스 추가                                                         
// ******************************************************************************
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"
// ******************************************************************************

#include "FastechPublic_IO.h"
#include "CtlBd_Library.h"

#include "Run_Rbt_Load_Clip.h"
#include "Run_Rbt_Load_LeftHeat.h"
#include "Run_Rbt_Load_Module.h"
#include "Run_Rbt_Load_RightHeat.h"
#include "Run_Rbt_Sorter.h"
#include "Run_Rbt_VisionLabel.h"
#include "Run_Rbt_Work_Buffer.h"
#include "Run_Stacker_Load_Clip.h"
#include "Run_Stacker_Load_LeftHeat.h"
#include "Run_Stacker_Load_Module.h"
#include "Run_Stacker_Load_RightHeat.h"
#include "Run_Stacker_Sorter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial

IMPLEMENT_DYNCREATE(CScreen_Initial, CFormView)

CScreen_Initial::CScreen_Initial()
	: CFormView(CScreen_Initial::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Initial)
	//}}AFX_DATA_INIT
}

CScreen_Initial::~CScreen_Initial()
{
}

void CScreen_Initial::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Initial)
	DDX_Control(pDX, IDC_RADIO_RBT_BUFFER, m_radio_buffer);
	DDX_Control(pDX, IDC_RADIO_RBT_VISION, m_radio_rbt_vision);
	DDX_Control(pDX, IDC_RADIO_TRAY_SUPPLY, m_radio_ld_stacker);
	DDX_Control(pDX, IDC_RADIO_REAR_RBT_HS, m_radio_rbt_rear_hs);
	DDX_Control(pDX, IDC_RADIO_RBT_SORTER, m_radio_rbt_sorter);
	DDX_Control(pDX, IDC_RADIO_RBT_LDMODULE, m_radio_rbt_ldmodule);
	DDX_Control(pDX, IDC_RADIO_CLIP_RBT, m_radio_rbt_clip);
	DDX_Control(pDX, IDC_RADIO_FRONT_RBT_HS, m_radio_rbt_front_hs);
	DDX_Control(pDX, IDC_RADIO_TRAY_UNLOADER, m_radio_unload_stacker);
	DDX_Control(pDX, IDC_RADIO_TRAY_TRANSFER, m_radio_hs_stacker);
	DDX_Control(pDX, IDC_RADIO_SORT_ROBOT, m_radio_rear_hs_stacker);
	DDX_Control(pDX, IDC_RADIO_AIR_BLOW, m_radio_clip_stacker);
	DDX_Control(pDX, IDC_RADIO_MOT_BD, m_radio_mot_bd);
	DDX_Control(pDX, IDC_RADIO_IO_BD, m_radio_io_bd);
	DDX_Control(pDX, IDC_GIF_INITIALIZING, m_gif_initializing);
	DDX_Control(pDX, IDC_BTN_INIT_SKIP, m_btn_init_skip);
	DDX_Control(pDX, IDC_BTN_INIT_RETRY, m_btn_init_retry);
	DDX_Control(pDX, IDC_MSG_INITIAL, m_msg_initial);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScreen_Initial, CFormView)
	//{{AFX_MSG_MAP(CScreen_Initial)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_INIT_SKIP, OnBtnInitSkip)
	ON_BN_CLICKED(IDC_BTN_INIT_RETRY, OnBtnInitRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial diagnostics

#ifdef _DEBUG
void CScreen_Initial::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Initial::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial message handlers

void CScreen_Initial::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다                                             
	// **************************************************************************
	mp_init_font = NULL;
	mp_init_font = new CFont;
	//	mp_init_font->CreateFont(35,20,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"MS Sans Serif");
	mp_init_font->CreateFont(35,20,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial Black Italic");
	// **************************************************************************
	All_Stop = 0 ;  // 쓰레드 상태 플래그 설정: 쓰레드 동작 시작
	st_handler.mn_motor_init_check = CTL_READY;
	
	OnInitial_Progress_Set();
	OnInitial_Picture_Set();
	OnInitial_Label_Set();
	OnInitial_RadioButton_Set();
	OnInitial_Change_Status(0);
	OnInitial_Step_Clear();
	OnInitial_Initial_Ready();
	
	st_handler.mn_initiailze_end_flag = CTL_NO; //2010.1013 
}

void CScreen_Initial::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다                                                 
	// **************************************************************************
	delete mp_init_font;
	mp_init_font = NULL;
	// **************************************************************************

	CFormView::OnDestroy();
}


void CScreen_Initial::OnTimer(UINT nIDEvent) 
{
	int mn_chk_run;			// 초기화 진행 함수 리턴 플래그 설정 변수
	int n_response;
	
	CDialog_Message  msg_dlg;

	if(nIDEvent == TMR_INIT)  
	{
		if (st_handler.mn_menu_lock != TRUE)  st_handler.mn_menu_lock = TRUE;			// 메뉴 사용 불가능 상태 플래그 설정 

		// **********************************************************************
		// 초기화 작업을 진행한다.                                               
		// **********************************************************************
		mn_chk_run = OnInitial_Init_Excution(); // 초기화 작업 진행 함수

		if (mn_chk_run == RET_GOOD)
		{
			KillTimer(TMR_INIT) ;  // 초기화 진행 타이머 삭제 

			 
			sprintf(st_msg.c_normal_msg, "[초 기 화] 장비 초기화 완료");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_normal_msg, "Initialization complete equipment!!");
			}


			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); //james 2010.1004 8); 

			// ******************************************************************
			// 현재 장비 상태 (STOP) 타이틀 바에 출력한다.                       
			// -> 초기화 후에 장비를 정지시킬 경우 이 부분 주석 해제             
			// ******************************************************************
			if (st_work.mn_run_status != dSTOP)
			{
				alarm.mn_reset_status = CTL_NO;
				
				Func.OnSet_IO_Port_Stop(); // 장비 상태 : 정지 상태인 경우 I/O 출력 내보내는 함수
				if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, 2, dSTOP);
			}
			// ******************************************************************

			if (st_handler.mn_initial != TRUE)  st_handler.mn_initial = TRUE;			// 초기화 작업 완료 플래그 설정 
			if (st_handler.mn_menu_lock != FALSE)  st_handler.mn_menu_lock = FALSE;	// 메뉴 사용 가능 상태 플래그 설정
			
			// 초기화가 정상적으로 끝났슴을 알려준다.
			FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_buzzer_off_switch_lamp, IO_OFF);

			Func.OnSet_IO_Port_Stop(); 
			st_msg.str_fallacy_msg = _T("Initialization completion");			
			n_response = msg_dlg.DoModal();

			::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 화면 전환 
			st_handler.mn_initiailze_end_flag = CTL_YES; //2010.1013 
			if (st_handler.mn_initial != TRUE)  st_handler.mn_initial = TRUE;			// 초기화 작업 완료 플래그 설정 
		}
		else if (mn_chk_run == RET_ERROR)
		{
			st_handler.mn_initiailze_end_flag = CTL_NO; //2010.1013 

			KillTimer(TMR_INIT) ;  // 초기화 진행 타이머 삭제 

			sprintf(st_msg.c_abnormal_msg, "[초 기 화] 장비 초기화 실패");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "[Initialize] Initialization failed equipment");
			}


			st_handler.mn_manual = YES;			// 시작은 메뉴얼 모드이다.
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			// ******************************************************************
			// 현재 장비 상태 (STOP) 타이틀 바에 출력한다.                       
			// -> 초기화 후에 장비를 정지시킬 경우 이 부분 주석 해제             
			// ******************************************************************
//			if (st_work.mn_run_status != dSTOP)
//			{
//				Func.OnSet_IO_Port_Stop(); // 장비 상태 : 정지 상태인 경우 I/O 출력 내보내는 함수
//				if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, 2, dSTOP);
//			}
			// ******************************************************************

			OnInitial_Controls_Enable(TRUE); // 초기화 화면에 대한 버튼 컨트롤 Enabled/Disabled 설정 함수

			if (st_handler.mn_initial != FALSE)		st_handler.mn_initial = FALSE;	// 초기화 작업 완료 여부 초기화 
			if (st_handler.mn_menu_lock != FALSE)	st_handler.mn_menu_lock = FALSE; // 메뉴 사용 가능 상태 플래그 설정 
		}
		// ******************************************************************************
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Initial::OnInitial_Label_Set()
{
	m_msg_initial.SetFont(mp_init_font);
	m_msg_initial.SetWindowText(_T("initializing~!! Wait a moment~"));
	m_msg_initial.SetCenterText();
	m_msg_initial.SetColor(RGB(0,0,0));
	m_msg_initial.SetGradientColor(RGB(0,0,255));
	m_msg_initial.SetTextColor(RGB(255,255,255));
}

void CScreen_Initial::OnInitial_Picture_Set()
{
	if (m_gif_initializing.Load(MAKEINTRESOURCE(IDR_GIF_INITIALIZING),_T("GIF")))
		m_gif_initializing.Draw();
}

void CScreen_Initial::OnInitial_Progress_Set()
{
	CSuperProgressCtrl::RegisterClass();
	
	m_ctrlProgress.Create(this, 4, 250, LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITIAL_STR_1)), 0);
	m_ctrlProgress.SetFillStyle(SP_FILL_HORZGRAD);
	m_ctrlProgress.SetColours(RGB(255,0,0), RGB(0,0,0));
	m_ctrlProgress.SetRange(0, 50); //20120502 유성준 - 50 -> 30 변경
	m_ctrlProgress.SetStep(1);

	m_ctrlProgress1.Create(this, 4, 400, LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INITIAL_STR_2)), 0);
	m_ctrlProgress1.SetFillStyle(SP_FILL_HORZGRAD);
	m_ctrlProgress1.SetColours(RGB(0,0,255), RGB(0,0,0));
	m_ctrlProgress1.SetRange(50, 100);
	m_ctrlProgress1.SetStep(1);
}

void CScreen_Initial::OnInitial_RadioButton_Set()
{
	m_radio_io_bd.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_mot_bd.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);

	m_radio_ld_stacker.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_hs_stacker.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_clip_stacker.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_rear_hs_stacker.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_unload_stacker.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_rbt_front_hs.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_rbt_clip.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_rbt_ldmodule.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_rbt_sorter.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_rbt_rear_hs.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_rbt_vision.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
	m_radio_buffer.SetIcons(IDI_GRAY_LED_ICON, IDI_RED_LED_ICON);
}

void CScreen_Initial::OnInitial_Controls_Enable(int n_init)
{
	if (n_init == TRUE)  m_btn_init_retry.ShowWindow(SW_SHOW);  
	else                 m_btn_init_retry.ShowWindow(SW_HIDE);
}

void CScreen_Initial::OnInitial_Change_Status(int n_status)
{
	switch(n_status)
	{
	case 0:  // 이니셜 초기화인 경우 
		m_radio_io_bd.SetLedState(FALSE);
		m_radio_mot_bd.SetLedState(FALSE);
		m_radio_ld_stacker.SetLedState(FALSE);
		m_radio_hs_stacker.SetLedState(FALSE);
		m_radio_clip_stacker.SetLedState(FALSE);
		m_radio_rear_hs_stacker.SetLedState(FALSE);
		m_radio_unload_stacker.SetLedState(FALSE);
		m_radio_ld_stacker.SetLedState(FALSE);
		m_radio_rbt_rear_hs.SetLedState(FALSE);
		m_radio_rbt_sorter.SetLedState(FALSE);
		m_radio_rbt_ldmodule.SetLedState(FALSE);
		m_radio_rbt_clip.SetLedState(FALSE);
		m_radio_buffer.SetLedState(FALSE);
		break;

	case 1:  // I/O 보드 초기화 
		m_radio_io_bd.SetLedState(TRUE);
		break;

	case 2:  // 모터 모드 초기화
		m_radio_mot_bd.SetLedState(TRUE);
		break;

	case 3:  // Barcode 초기화
		m_radio_ld_stacker.SetLedState(TRUE);
		break;

	case 4:  // Front Buffer 초기화
		m_radio_hs_stacker.SetLedState(TRUE);
		break;

	case 5:  // Rear Buffer 초기화 
		m_radio_rear_hs_stacker.SetLedState(TRUE);
		break;

	case 6:  // Front Shifter 초기화
		m_radio_clip_stacker.SetLedState(TRUE);
		break;

	case 7:  // Rear Shifter 초기화
		m_radio_unload_stacker.SetLedState(TRUE);
		break;

	case 8:
		m_radio_rbt_ldmodule.SetLedState(TRUE);
		break;

	case 9:
		m_radio_rbt_front_hs.SetLedState(TRUE);
		break;

	case 10:
		m_radio_rbt_rear_hs.SetLedState(TRUE);
		break;

	case 11:
		m_radio_rbt_clip.SetLedState(TRUE);
		break;
	
	case 12:
		m_radio_rbt_sorter.SetLedState(TRUE);
		break;
	
	case 13:
		m_radio_rbt_vision.SetLedState(TRUE);
		break;

	case 14:
		m_radio_buffer.SetLedState(TRUE);
		break;

	default :
		break;
	}
}

void CScreen_Initial::OnInitial_Step_Clear()
{
	int i = 0, j = 0, k = 0;
	
	ml_init_step = 0;			// 초기화 진행 스텝 정보 저장 변수 초기화 
	mn_motor_init_count = 0;
	
	for (i = 0; i < MAXSITE; i++)
	{
		st_handler.mn_init_state[i] = CTL_NO;
	}
}

void CScreen_Initial::OnInitial_Initial_Ready()
{
	int n_response;					// 대화 상자에 대한 리턴 값 저장 변수
	CDialog_Select select_dlg;
	char chr_data[25];	
	CString str_lock_check;
	int nFlag, i, nRet=0;
	
	st_handler.mn_initial_error = CTL_READY;

	// Lock상태 였는지 확인을 한다.
	:: GetPrivateProfileString("Password", "SysLockStatus", "NO", chr_data, 20, st_path.mstr_basic);
	str_lock_check = chr_data;

	if (str_lock_check == "YES")			// Lock상태에서 강제 종료한것이라면...
	{
		// **************************************************************************
		// Lock 화면으로 전환하여 장비 사용을 막는다.
		// **************************************************************************
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, FORM_NAME, 2);  // 메인 프레임에 SYSTEM LOCK 화면 출력 요청
		// **************************************************************************
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_normal_msg = "System Locking.";
			sprintf(st_msg.c_normal_msg, "System Locking.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
		return;
	}
	
	// **************************************************************************
	// 현재 장비 상태 (INIT) 타이틀 바에 출력한다.                               
	// **************************************************************************
	if (st_work.mn_run_status != dINIT)
	{
		Func.OnSet_IO_Port_Init(); // 장비 상태 : 동작 상태인 경우 I/O 출력 내보내는 함수
		if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.mn_run_status);
	}
	// **************************************************************************
	
	if (st_handler.mn_motor_init_check == CTL_READY)			// 모터 초기화를 할것인지 말것인지 결정한다. 2K4/11/16/ViboX
	{
		st_msg.mstr_confirm_msg = _T("Handler Motor Init?");

		n_response = select_dlg.DoModal();
		
		if (n_response == IDOK)
		{
			st_handler.mn_motor_init_check = CTL_YES;
		}
		else if (n_response == IDCANCEL)
		{
			st_handler.mn_motor_init_check = CTL_NO;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////
	// 복구 모드를 확인 한다.
	////////////////////////////////////////////////////////////////////////////////////
	:: GetPrivateProfileString("SaveMode", "RecoveryMode", "0", chr_data, 20, st_path.mstr_basic);
	st_basic.l_handler_recovery_flag[0] = long(atoi(chr_data));

	//[0]=>복구모드 저장 모드(0:저장되지 않은 상태, 1:복구모드가 저장 상태(이때만 초기화시 복구여부를 물어볼 수  있다) 
	//[1]=>초기화시 복구모드 선택 유/무 확인 (0:복구모드 선택안함, 1:복구모드 선택)
	if (st_basic.l_handler_recovery_flag[0] == CTL_YES)			// 기존 상태로 복구 할것인지 말것인지 결정한다. 2K9/08/25/ViboX
	{
		st_msg.mstr_confirm_msg = _T("Handler Recovery Mode(YES), Are You sure ?");
		
		n_response = select_dlg.DoModal();
		
		if (n_response == IDOK)
		{		 
			nFlag = CTL_NO;
			for (i = 0; i < HANDLER_BACKUP_DATA_RW; i++)
 			{
 				n_response = Func.Handler_Recovery_Data(0, i, DATA_READ);
 				
 				if (n_response == RET_ERROR)
 				{
					if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
					{
//						st_msg.mstr_abnormal_msg.Format("[ERROR_READ] Handler Recovery Mode SKIP[%02d]. Handler ALL Initialize. ", i);
						sprintf(st_msg.c_abnormal_msg, "[ERROR_READ] Handler Recovery Mode SKIP[%02d]. Handler ALL Initialize. ", i);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}
					nFlag = CTL_YES;
 				}
 			}

			if(nFlag == CTL_YES)
			{
				st_basic.l_handler_recovery_flag[1] = CTL_NO;		
				if (st_handler.cwnd_list != NULL)
				{
//					st_msg.mstr_abnormal_msg.Format("[skip] recovery data save-init[read]");
					sprintf(st_msg.c_abnormal_msg, "[skip] recovery data save-init[read]");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_msg.mstr_normal_msg.Format("[ok]recovery data save-init[read]");
					sprintf(st_msg.c_normal_msg, "[ok]recovery data save-init[read]");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
		}
		else if (n_response == IDCANCEL)
		{
			st_basic.l_handler_recovery_flag[1] = CTL_NO;
			if (st_handler.cwnd_list != NULL) 
			{
//				st_msg.mstr_normal_msg.Format("[Init_Menu_SKIP]recovery data save-init[read]");
				sprintf(st_msg.c_normal_msg, "[Init_Menu_SKIP]recovery data save-init[read]");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
	}

	SetTimer(TMR_INIT, 250, NULL);  // 초기화 작업 진행 타이머 생성
}

int CScreen_Initial::OnInitial_Init_Excution()
{
	int Ret;
	int i, j = 0;
//	long motor[2];
	CString str_macaddr;

	CDialog_Message msg_dlg;

	mn_init_flag = RET_PROCEED;

	if (st_work.mn_run_status != dINIT)  return mn_init_flag;				// 현재 장비 상태가 RUN 아니면 리턴한다.

	switch (ml_init_step)
	{
	case 0 :
		if(Func.DoorOpenCheckSpot() == CTLBD_RET_GOOD)
		{
			ml_init_step = 1;
		}
		else
		{
			st_work.mn_run_status  = dWARNING;
			return CTLBD_RET_ERROR;
		}
		mn_pos_step = 1;
		ml_init_step = 1;
		break;

	case 1:
		if (st_handler.mn_manual == CTL_YES)
		{	
			ml_init_step = 5;
			mn_pos_step = 9;
			break;
		}

		if (st_handler.mn_motor_init_check == CTL_YES)
		{
			if (FAS_IO.Get_In_Bit(st_io.i_mc1_chk) == CTL_NO || FAS_IO.Get_In_Bit(st_io.i_mc2_chk) == CTL_NO )
			{
				st_handler.mstrSelectMessage = "먼저 RESET 버튼을 눌러 주세요.";
				
				if (st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_handler.mstrSelectMessage = "RESET BUTTON CHECK...";
				}
				st_handler.mnSelectMessage = 0;
				
				::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1001, 0);
				
				if (st_alarm.mstr_cur_state == "0")	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_ON);
				else                                FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_ON);
				
				mn_pos_step = 2;
				ml_init_step = 1;
				break;
			}
		}
		// **************************************************************************
		// 화면 설정 데이터 로딩 부분                                                
		// **************************************************************************
 		mcls_m_basic.OnBasic_Data_Load();				// 베이직 셋팅 정보 전역 변수에 로딩하는 함수
// 		mcls_m_basic.OnTraySet_Data_Load();
 		mcls_m_basic.OnMaintenance_Data_Load();			// Tower Lamp 셋팅 정보 전역 변수에 로딩하는 함수
 		mcls_m_basic.OnWaitTime_Data_Load();			// Wait Time 셋팅 정보 전역 변수에 로딩하는 함수

// 
// 		mcls_m_basic.OnLoaderRobot_Teach_Data_Load();
// 		mcls_m_basic.OnLoaderSite_Teach_Data_Load();
// 		mcls_m_basic.OnTestSite_Teach_Data_Load();
// 		mcls_m_basic.OnUnloadFront_Teach_Data_Load();
// 		mcls_m_basic.OnUnloadRear_Teach_Data_Load(); //james 2010,0707 추가 누락되어 있음 
// 		mcls_m_basic.OnUnloadSite_Teach_Data_Load();
 		mcls_m_basic.OnInterface_Data_Load();        //james 2010,0707 추가 누락되어 있음 
// 		mcls_m_basic.OnLoad_Motor_Mode();			 //james 2010,0707 추가 누락되어 있음
 		mcls_m_basic.OnSkip_Data_Load();	//20120525
		mcls_m_basic.OnBasic_Label_Data_Load();	//20120531
		mcls_m_basic.OnBarcode_Label_Data_Load();//20120604
		mcls_m_basic.OnBasic_Part_Number_Load();

		// **************************************************************************

		if (st_handler.cwnd_title != NULL)   
		{
// 			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, INTERFACE_MODE, st_basic.n_mode_interface);	// Interface Mode
// 			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, RETEST_MODE, st_basic.n_mode_retest);			// Retest Mode
// 			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, st_basic.n_mode_device);			// Device Mode
// 			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, SOCKETOFF_MODE, 0);							// Socket Off Mode
// 			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_NAME, 0);								// Device Name
// 			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, TAMS_ONLINE, st_var.n_tams_online);		//james 2010.0922 
		}

		if (st_handler.cwnd_list != NULL)
		{
/*			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);*/
		}
		mn_pos_step = 9;
				
		ml_init_step = 5;
		break;

	case 2:
		if (st_handler.mnSelectMessage == 1)					// YES
		{
			if (st_alarm.mstr_cur_state == "0")	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_OFF);
			else                                FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_OFF);
			ml_init_step = 0;
		}
		else if (st_handler.mnSelectMessage == 2)			// NO
		{
			if (st_alarm.mstr_cur_state == "0")	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_OFF);
			else                                FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_OFF);
			ml_init_step = 0;
		}
		break;

	case 5 :
		Ret = st_handler.mn_io_board_initial;		// I/O 보드 초기화 여부 검사한다

		if (Ret != TRUE)  
		{
			sprintf(cJamcode, "900002");
			CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);

			ml_init_step = 0;						// 초기화 진행 스텝 정보 초기화 
			mn_init_flag = RET_ERROR;				// 초기화 작업 에러 플래그 설정
		}
		else
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_normal_msg = _T("I/O Board Initialized...");
				sprintf(st_msg.c_normal_msg, "I/O Board Initialized...");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			Func.OnSet_IO_Out_Port_Clear();

			mn_pos_step += 8;
	
			OnInitial_Change_Status(1);				// I/O보드 초기화 완료.

			ml_init_step = 10;
		}
		break;

	case 10 :
		Ret = st_handler.mn_mot_brd_initial;
		
		if (Ret != TRUE)
		{
			alarm.mstr_code = _T("900001");			// 현재 발생한 알람 코드 정보 설정 
			sprintf(alarm.mc_code, "9000001");
			alarm.mn_count_mode = 0;					// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
			alarm.mn_type_mode = eWARNING;			// 현재 발생한 알람 상태 플래그 설정 
			st_work.mn_run_status = dWARNING;		// dJAM,dWARNING 
			
			ml_init_step = 0;						// 초기화 진행 스텝 정보 초기화 
			mn_init_flag = RET_ERROR;				// 초기화 작업 에러 플래그 설정 
			
	 		return RET_ERROR;
		}
		else
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_normal_msg = _T("Motor Board Open Success!");
				sprintf(st_msg.c_normal_msg, "Motor Board Open Success!");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			Func.OnMot_Speed_Setting();					// MOTOR 보드가 정상적으로 OPEN이 된 후에 속도를 세팅한다. 2K5/03/24/ViboX

			mn_pos_step += 8;			
			
		/*	motor[0] = M_RBT_SORTER_X;
			motor[1] = M_RBT_SORTER_Y;

			st_linearmot[SORTER_MOT_MAP].l_AxisCnt = 2;
			st_linearmot[SORTER_MOT_MAP].lp_AxisNum[0] = motor[0];
			st_linearmot[SORTER_MOT_MAP].lp_AxisNum[1] = motor[1];

			COMI.Create_LmIndexMapAxis(SORTER_MOT_MAP, 2, motor);

			motor[0] = M_RBT_UNLOAD_X;
			motor[1] = M_RBT_UNLOAD_Y;

			st_linearmot[UNLOAD_MOT_MAP].l_AxisCnt = 2;
			st_linearmot[UNLOAD_MOT_MAP].lp_AxisNum[0] = motor[0];
			st_linearmot[UNLOAD_MOT_MAP].lp_AxisNum[1] = motor[1];
			
			COMI.Create_LmIndexMapAxis(UNLOAD_MOT_MAP, 2, motor);*/

			
			OnInitial_Change_Status(2);					// MOTOR 보드 초기화 완료.
			

			ml_init_step = 15;
		}
		break;

	case 15:
		for (i = 0; i < MAXSITE; i++)
		{
			st_handler.mn_init_state[i] = CTL_NO;
		}

		if (st_handler.mn_motor_init_check == CTL_YES)			// 모터 초기화를 할것인지 말것인지 결정한다. 2K4/11/16/ViboX
		{
			if (st_handler.mn_manual == CTL_NO)
			{
				ml_init_step = 20;
			}
			else if (st_handler.mn_manual == CTL_YES)
			{	
				if (m_thread[4] != NULL)
				{
					ml_init_step = 30;
				}
				else
				{
					ml_init_step = 20;
				}
			}
		}
		else if (st_handler.mn_motor_init_check == CTL_NO)
		{
			ml_init_step = 100;
		}
		break;

	case 20 :
		// **********************************************************************
		// Tray Supply 동작 제어 쓰레드 생성한다                                 
		// **********************************************************************
 		m_thread[0]=AfxBeginThread(OnThread_Stacker_Load_Module, this);
 		if (m_thread[0] != NULL) 	
 			hThrHandle[0] = m_thread[0]->m_hThread;
		// **********************************************************************
 		m_thread[1]=AfxBeginThread(OnThread_Stacker_Load_LeftHeat, this);
 		if (m_thread[1] != NULL) 	
 			hThrHandle[1] = m_thread[1]->m_hThread;
		// **********************************************************************
 		m_thread[2]=AfxBeginThread(OnThread_Stacker_Load_RightHeat, this);
 		if (m_thread[2] != NULL) 	
 			hThrHandle[2] = m_thread[2]->m_hThread;
		// **********************************************************************
 		m_thread[3]=AfxBeginThread(OnThread_Stacker_Load_Clip, this);
 		if (m_thread[3] != NULL) 	
 			hThrHandle[3] = m_thread[3]->m_hThread;
		// **********************************************************************
 		m_thread[4]=AfxBeginThread(OnThread_Stacker_Sorter, this);
 		if (m_thread[4] != NULL) 	
 			hThrHandle[4] = m_thread[4]->m_hThread;
		// **********************************************************************
 		m_thread[5]=AfxBeginThread(OnThread_Rbt_Load_Module, this);
 		if (m_thread[5] != NULL) 	
 			hThrHandle[5] = m_thread[5]->m_hThread;
		// **********************************************************************
 		m_thread[6]=AfxBeginThread(OnThread_Rbt_Load_LeftHeat, this);
 		if (m_thread[6] != NULL) 	
 			hThrHandle[6] = m_thread[6]->m_hThread;
		// **********************************************************************
 		m_thread[7]=AfxBeginThread(OnThread_Rbt_Load_RightHeat, this);
 		if (m_thread[7] != NULL) 	
 			hThrHandle[7] = m_thread[7]->m_hThread;
		// **********************************************************************
 		m_thread[8]=AfxBeginThread(OnThread_Rbt_Load_Clip, this);
 		if (m_thread[8] != NULL) 	
 			hThrHandle[8] = m_thread[8]->m_hThread;
		// **********************************************************************
 		m_thread[9]=AfxBeginThread(OnThread_Rbt_VisionLabel, this);
 		if (m_thread[9] != NULL) 	
 			hThrHandle[9] = m_thread[9]->m_hThread;
		// **********************************************************************
 		m_thread[9]=AfxBeginThread(OnThread_Rbt_Sorter, this);
 		if (m_thread[9] != NULL) 	
 			hThrHandle[9] = m_thread[9]->m_hThread;
		// **********************************************************************
 		m_thread[10]=AfxBeginThread(OnThread_Work_Buffer, this);
 		if (m_thread[10] != NULL) 	
 			hThrHandle[10] = m_thread[10]->m_hThread;
		// **********************************************************************

		ml_init_step = 30;
		break;

	case 25 :
 		Run_Rbt_Load_Clip.InitStep = 0;
 		Run_Rbt_Load_LeftHeat.InitStep = 0;
 		Run_Rbt_Load_Module.InitStep = 0;
 		Run_Rbt_Load_RightHeat.InitStep = 0;
 		Run_Rbt_Sorter.InitStep = 0;
 		Run_Rbt_VisionLabel.InitStep = 0;

 		Run_Rbt_Work_Buffer.InitStep = 0;
 		Run_Stacker_Load_Clip.InitStep = 0;
 		Run_Stacker_Load_LeftHeat.InitStep = 0;
 		Run_Stacker_Load_Module.InitStep = 0;
 		Run_Rbt_VisionLabel.InitStep = 0;
		Run_Stacker_Load_RightHeat.InitStep = 0;
		Run_Stacker_Sorter.InitStep = 0;
		ml_init_step = 30;
		break;

	case 30 :
		if (st_handler.mn_init_state[INIT_LD_STACKER] == CTL_YES)
		{
			mn_pos_step += 5;			
			OnInitial_Change_Status(3);			
			st_handler.mn_init_state[INIT_LD_STACKER] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_LEFTHS_STACKER] == CTL_YES)
		{
			mn_pos_step += 5;			
			OnInitial_Change_Status(4);			
			st_handler.mn_init_state[INIT_LEFTHS_STACKER] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_RIGHTHS_STACKER] == CTL_YES)
		{
			mn_pos_step += 5;			
			OnInitial_Change_Status(5);			
			st_handler.mn_init_state[INIT_RIGHTHS_STACKER] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_CLIP_STACKER] == CTL_YES)
		{
			mn_pos_step += 5;
			OnInitial_Change_Status(6);			
			st_handler.mn_init_state[INIT_CLIP_STACKER] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_ULD_STACKER] == CTL_YES)
		{
			mn_pos_step += 5;
			OnInitial_Change_Status(7);			
			st_handler.mn_init_state[INIT_ULD_STACKER] = CTL_READY;
		}

		if (st_handler.mn_init_state[INIT_RBT_MODULE] == CTL_YES)
		{
			mn_pos_step += 5;
			OnInitial_Change_Status(8);			
			st_handler.mn_init_state[INIT_RBT_MODULE] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_RBT_LEFT_SINK] == CTL_YES)
		{
			mn_pos_step += 5;
			OnInitial_Change_Status(9);			
			st_handler.mn_init_state[INIT_RBT_LEFT_SINK] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_RBT_RIGHT_SINK] == CTL_YES)
		{
			mn_pos_step += 5;
			OnInitial_Change_Status(10);			
			st_handler.mn_init_state[INIT_RBT_RIGHT_SINK] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_RBT_CLIP_MODULE] == CTL_YES)
		{
			mn_pos_step += 5;
			OnInitial_Change_Status(11);			
			st_handler.mn_init_state[INIT_RBT_CLIP_MODULE] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_RBT_SORTER] == CTL_YES)
		{
			mn_pos_step += 5;
			OnInitial_Change_Status(12);			
			st_handler.mn_init_state[INIT_RBT_SORTER] = CTL_READY;
		}
		

		if (st_handler.mn_init_state[INIT_LAVEL_VISION] == CTL_YES)
		{
			mn_pos_step += 10;
			OnInitial_Change_Status(13);			
			st_handler.mn_init_state[INIT_LAVEL_VISION] = CTL_READY;
		}
		
		if (st_handler.mn_init_state[INIT_WORK_BUFFER] == CTL_YES)
		{
			mn_pos_step += 10;
			OnInitial_Change_Status(14);			
			st_handler.mn_init_state[INIT_WORK_BUFFER] = CTL_READY;
		}		
		//st_handler.mn_init_state[INIT_CLIP_STACKER] = CTL_READY;
		st_handler.mn_init_state[INIT_LD_CLIP_TRANSFER] = CTL_READY;
		for (i = 0; i < MAXSITE; i++)
		{
			if (st_handler.mn_init_state[i] == CTL_READY)
			{
				j++;
			}
		}

		// 전부 홈체크가 끝났으면 초기화 끝~~~~ ^_^
		if (j == MAXSITE)
		{
			for (i = 0; i < MAXSITE; i++)
			{
				st_handler.mn_init_state[i] = CTL_YES;
			}

			ml_init_step = 100;
 		}

// 		if (j == 1)
// 		{
// 			for (i = 0; i < 1; i++)
// 			{
// 				st_handler.mn_init_state[i] = CTL_YES;
// 			}
// 			
// 			ml_init_step = 100;
// 		}
		break;

	case 100 :
		mn_pos_step = 100;

		mn_motor_init_count = 0;
		ml_init_step = 0; 
		mn_init_flag = RET_GOOD;
		break;
	}

	if (mn_pos_step < 51)
	{
		m_ctrlProgress.SetPos(mn_pos_step);
	}
	else if (mn_pos_step > 49)
	{
		if (mn_pos_step > 100)	mn_pos_step = 100;
		m_ctrlProgress.SetPos(50);
		m_ctrlProgress1.SetPos(mn_pos_step);
	}
	
	//초기화중 문제가 생겼다~
	if (st_handler.mn_initial_error == TRUE)
	{
		mn_pos_step = 0;
		m_ctrlProgress.SetPos(0);
		m_ctrlProgress1.SetPos(50);
		
		ml_init_step = 0;			// 초기화 진행 스텝 정보 초기화 
		mn_init_flag = RET_ERROR;	// 초기화 작업 에러 플래그 설정 
	}

	return mn_init_flag;
}

void CScreen_Initial::OnBtnInitSkip() 
{
	KillTimer(TMR_INIT);  // 초기화 진행 타이머 삭제 
	
//	st_msg.mstr_normal_msg = _T("[initialization] A APCT initialization Skip.");
	sprintf(st_msg.c_normal_msg, "[initialization] AMT830 initialization Skip.");
	if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
	
	// ******************************************************************
	// 현재 장비 상태 (STOP) 타이틀 바에 출력한다.                       
	// -> 초기화 후에 장비를 정지시킬 경우 이 부분 주석 해제             
	// ******************************************************************
	if (st_work.mn_run_status != dSTOP)
	{
		alarm.mn_reset_status = CTL_NO;
		FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_OFF);	// Reset Switch에 Lamp를 Off 시킨다.
		
		Func.OnSet_IO_Port_Stop(); // 장비 상태: 정지 상태인 경우 I/O 출력 내보내는 함수
		if(st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, dSTOP);
	}
	// ******************************************************************
	
	if (st_handler.mn_initial != TRUE)  st_handler.mn_initial = TRUE;			// 초기화 작업 완료 플래그 설정 
	if (st_handler.mn_menu_lock != FALSE)  st_handler.mn_menu_lock = FALSE;	// 메뉴 사용 가능 상태 플래그 설정
	
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 화면 전환 
}

void CScreen_Initial::OnBtnInitRetry() 
{
	int n_response;  // 대화 상자에 대한 리턴 값 저장 변수
	
	CDialog_Select select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Initialization work progress");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		if (st_handler.mn_initial != FALSE)  st_handler.mn_initial = FALSE;  // 초기화 작업 완료 여부 초기화 
		
		st_handler.mn_manual = CTL_YES;
		mn_pos_step = 0;					// 프로그레서 위치 정보 초기화
		
		OnInitial_Controls_Enable(FALSE);	// 초기화 화면에 대한 버튼 컨트롤 Enabled/Disabled 설정 함수
		
		OnInitial_Change_Status(0);			// 초기화 화면 이니셜 진행 버튼 상태 변경 함수
		
		OnInitial_Step_Clear();				// 초기화 작업: Step 데이터 저장 변수 초기화 함수
		
		OnInitial_Initial_Ready();			// 초기화 준비 작업 설정 함수
	}
	else if (n_response == IDCANCEL)
	{
		
	}	
}
