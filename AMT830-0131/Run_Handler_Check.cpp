// Run_Handler_Check.cpp: implementation of the CRun_Handler_Check class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Run_Handler_Check.h"

#include "Variable.h"				// 전역 변수 정의 Class
#include "Public_Function.h"		// Motor 및 Cylinder 동작 Class
#include "FastechPublic_IO.h"
#include "CtlBd_Library.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRun_Handler_Check Run_Handler_Check;
CRun_Handler_Check Run_Handler_Check_Manual;

CRun_Handler_Check::CRun_Handler_Check()
{
	StartStep = 0;
	StopStep = 0;
	LoaderStep = 0;
	AlaemClearStep = 0;
	BuzzerStep = 0;
	LdClipCoverStep = 0;
	UldClipCoverStep = 0;
	RejectTrayStep = 0;
	
	MachineResetStep = 0;
	
	DoorStep = 0;
	AreaStep = 0;
	
	n_emo_chk = FALSE;
	n_air_chk = FALSE;

	n_light_curtain_chk = CTL_NO;
	i_reject_tray_change_flag = FALSE;

	n_front_key = -1;
	n_rear_key = -1;


}

CRun_Handler_Check::~CRun_Handler_Check()
{

}

void CRun_Handler_Check::ButtonCheck_Start()
{
	int i;
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return; // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************
	if (st_handler.mn_switch_lock != FALSE)	return;	// 모달 창이 떠 있으면 실행하지 않는다.
	if (st_work.mn_run_status == dRUN)              return;//20121122

	switch (StartStep)
	{
	case 0:
		// **********************************************************************
		// RUN 키 I/O 입력이 On 상태이고 상태가 STOP 상태였는지 검사             
		// -> 다시 말해, STOP 상태에서만 RUN 키 입력이 동작한다                  
		// **********************************************************************
		if (FAS_IO.Get_In_Bit(st_io.i_start_switch_chk) == TRUE && st_work.mn_run_status == dSTOP)
		{
			// 만일 Stop Button이 같이 눌렸다면 동작되지 않는다.
			if (FAS_IO.Get_In_Bit(st_io.i_stop_switch_chk) == TRUE)
			{
				break;
			}

			StartStep = 100;
		}
		break;

	case 100:
		// **********************************************************************
		// START 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함        
		// **********************************************************************
		SwitchWaitTime[0] = GetCurrentTime();
		StartStep = 200;
		break;

	case 200: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

		if(SwitchWaitTime[2] > 50 && FAS_IO.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			StartStep = 300;
		}
		else if (FAS_IO.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)
		{
			StartStep = 0;
		}
		break;

	case 300:
		// **********************************************************************
		// RUN 키 I/O 입력이 Off 되었는지 검사                                   
		// -> 알람 화면 출력 상태이면 알람 화면 종료 -> 메인 화면으로 전환       
		// **********************************************************************
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];

		if(SwitchWaitTime[2] > 10)
		{
			if (FAS_IO.Get_In_Bit(st_io.i_start_switch_chk) == FALSE)
			{
				StartStep = 600;
			}
			else
			{
				StartStep = 0;
			}
		}
		// **********************************************************************
		break;

	case 600:
		// **********************************************************************
		//  RUN할수 있는 조건인지 검사한다. 이상이 없어야 RUN이 된다.            
		// **********************************************************************
		if (st_handler.mn_motor_init_check != CTL_YES)	// 모터 초기화를 하지 않았다면 동작하지 않는다. 2K4/11/23/ViboX
		{
			Func.OnSet_IO_Port_Sound(IO_ON);
			sprintf(st_msg.c_abnormal_msg, "[MOTOR INIT CHECK] Motor Initial..., become Run.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(cJamcode,"909002");		// Alarm 화면을 띄워준다. 2K4/11/21/ViboX
			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정
			st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX
			StartStep = 0;
			return;
		}

// 		// JAM이 리셋 되었는지 확인하고 RUN을 시킨다.
		if (alarm.mn_reset_status != CTL_NO)
		{
			sprintf(st_msg.c_abnormal_msg, "[JAM CONFIRM] If do to do Run, do first Jam Reset.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			StartStep = 0;
			break;
		}

		// Door가 열려있으면 RUN이 되지 않는다.
		if (Func.DoorOpenCheckSpot() == RET_ERROR && st_handler.mn_safety == CTL_YES)	
		{
			Func.OnSet_IO_Port_Sound(IO_ON);

			sprintf(st_msg.c_abnormal_msg, "[DOOR CHECK] Door Open being done Run become.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);			
			st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX

			StartStep = 0;
			break;
		}
	
		// Air 감지 센서를 확인한다.
		if (FAS_IO.get_in_bit(st_io.i_main_air_chk, IO_ON) == IO_OFF)
		{
			//900200 0 90 "Air Supply Check Error."
			sprintf(cJamcode,"900200");    // 발생한 알람 코드 설정 //
			st_work.mn_run_status = dWARNING;  // 장비 동작 상태 설정 //
			CTL_Lib.Alarm_Error_Occurrence(5500, st_work.mn_run_status, cJamcode);
			break;
		}	

		// **********************************************************************
		// RUN 키 I/O 입력이 Off 되었는지 검사                                   
		// -> 알람 화면 출력 상태이면 알람 화면 종료 -> 메인 화면으로 전환       
		// **********************************************************************
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);		// 메인 프레임으로 메시지 전송하여 알람 화면 종료 요청 
	
		if (st_handler.mn_menu_num != 101)
		{
			::SendMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
		}

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
			
			
			StartStep = 620;
			break;
		}
		
// 		st_basic.mn_mode_run = CTL_YES;
// 		if (st_basic.mn_mode_run == CTL_NO)		// OFFLINE이면 상태를 알리자.
// 		{
// 			st_handler.mstrSelectMessage = "OFFLINE 상태입니다. RUN 하시겠습니까?";
// 			
// 			if (st_handler.mn_language == LANGUAGE_ENGLISH)
// 			{
// 				st_handler.mstrSelectMessage = "NOW OFFLINE. ARE YOU RUN?";
// 			}
// 			st_handler.mnSelectMessage = 0;
// 			
// 			::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1001, 0);
// 						
// 			StartStep = 610;
// 			break;
// 		}
		
		StartStep = 650;
		break;

	case 610:
		if (st_handler.mnSelectMessage == 1)					// YES
		{
			if (st_alarm.mstr_cur_state == "0")	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_OFF);
			else                                FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_OFF);

			StartStep = 650;
		}
		else if (st_handler.mnSelectMessage == 2)			// NO
		{
			if (st_alarm.mstr_cur_state == "0")	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_OFF);
			else                                FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_OFF);

			StartStep = 0;
		}
		break;

	case 620:
		if (st_handler.mnSelectMessage == 1)					// YES
		{
			if (st_alarm.mstr_cur_state == "0")	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_OFF);
			else                                FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_OFF);

			StartStep = 0;
		}
		else if (st_handler.mnSelectMessage == 2)			// NO
		{
			if (st_alarm.mstr_cur_state == "0")	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_OFF);
			else                                FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_OFF);
			StartStep = 0;
		}
		break;

	case 650:
		if(st_handler.n_plate_fwdbwd_onoff == YES)
		{
			FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON);
			FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
		}
		else
		{
			FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
		}
		StartStep = 700;
		SwitchWaitTime[0] = GetCurrentTime();
		break;


	case 700:
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		if(SwitchWaitTime[2] < 500) break; 
		else if(SwitchWaitTime[2] < 0) SwitchWaitTime[0] = GetCurrentTime();
		
		
		///////////////////// 20121122
		//if (FAS_IO.get_in_bit(st_io.i_auto_mode_switch_chk, IO_OFF) == IO_OFF)
		if (FAS_IO.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF) == IO_OFF)
		//	FAS_IO.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
		//if(st_handler.mn_manual == YES)
		{
			sprintf(st_msg.c_abnormal_msg, "[MODE CHECK] Auto로 방향키를 돌려주세요.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "[MODE CHECK] Auto, please return to the keys");
			}

			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);			
			StartStep = 0;
			break;
		}
		/////////////////////
		
		
		if(st_work.mn_run_status != dSTOP) break;

		///////////////////////////////////////////20121122
 		st_work.mn_run_status = dREINSTATE;
		st_handler.n_sync_reinstate = READY;
		
		st_work.mn_LdModuleRobot_Reinstatement_Ok = NO;
		st_work.mn_LeftHsRobot_Reinstatement_Ok = NO;
		st_work.mn_RightHsRobot_Reinstatement_Ok = NO;
		st_work.mn_ClipRobot_Reinstatement_Ok = NO;   //20130416
		st_work.mn_SorterRobot_Reinstatement_Ok = NO;
		st_work.mn_UnloadRobot_Reinstatement_Ok = NO;
		st_work.mn_WorkBufferRobot_Reinstatement_Ok = NO;

		st_work.mn_LdModuleRobot_Reinstatement_Ready = NO;
		st_work.mn_LeftHsRobot_Reinstatement_Ready = NO;
		st_work.mn_RightHsRobot_Reinstatement_Ready = NO;
		st_work.mn_ClipRobot_Reinstatement_Ready = NO;
		st_work.mn_SorterRobot_Reinstatement_Ready = NO;
		st_work.mn_UnloadRobot_Reinstatement_Ready = NO;
		st_work.mn_WorkBufferRobot_Reinstatement_Ready = NO;

		if(st_handler.cwnd_title != NULL) st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.mn_run_status);		

		StartStep = 750;//900;//750;
		break;

	case 750:
		st_msg.mstr_event_msg[0] = "Recovery Mode.......";
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
		StartStep = 800;
		break;
		
	case 800:
		if(st_work.mn_LdModuleRobot_Reinstatement_Ok   == YES && st_work.mn_LeftHsRobot_Reinstatement_Ok == YES && 
		   st_work.mn_RightHsRobot_Reinstatement_Ok    == YES && st_work.mn_ClipRobot_Reinstatement_Ok == YES && // 20130416
		   st_work.mn_SorterRobot_Reinstatement_Ok     == YES && st_work.mn_UnloadRobot_Reinstatement_Ok == YES && // 20130416
		   st_work.mn_WorkBufferRobot_Reinstatement_Ok == YES)
		{
			st_handler.n_sync_reinstate = YES;
		}
		if(st_handler.n_sync_reinstate == YES)	//복귀동작 완료!
		{
			for (i = 0; i < 40; i++)
			{
				st_work.mn_reinstatement_mode[i] = 0;
			}
			StartStep = 900;
		}
		else if(st_handler.n_sync_reinstate == NO)	//복귀동작 실패!
		{
			st_work.mn_run_status = dSTOP;
			if(st_handler.cwnd_title != NULL)
			{
				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.mn_run_status);
			}		
			StartStep = 850;
		}
		else
		{
			if(st_work.mn_run_status != dREINSTATE)	//복귀 동작이 성공도, 실패도 아닌데 복귀동작 상태가 아니라면..
			{
				st_work.mn_run_status = dSTOP;
				if(st_handler.cwnd_title != NULL)
				{
					st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.mn_run_status);
				}
				StartStep = 0;
			}
		}
		break;

	case 850:
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
		StartStep = 0;
		break;
		
	case 900:
		///////////////////////////////////////////
		Func.OnSet_IO_Port_Run();									// 장비 상태 : 동작 상태인 경우 I/O 출력 내보내는 함수
		if(st_work.n_jamcode_flag == YES)
		{
			st_work.n_jamcode_flag	= NO;
			st_work.t_jamcode_end	= CTime::GetCurrentTime();
			::PostMessage(st_handler.hWnd, WM_WORK_END, MAIN_MYSQL_ALARM_WRITE, 0);
		}

		n_emo_chk = FALSE;
		n_air_chk = FALSE;
		alarm.n_area_alarm = FALSE;
		alarm.n_emo_alarm = FALSE;
		
		n_light_curtain_chk = CTL_NO;

		// 20131210
		for(i = 0; i < 9; i++)
		{
			st_work.mn_reinstatement_mode[i] = 0;
		}

		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);
		::PostMessage(st_handler.hWnd, WM_WORK_END, MAIN_CAP_REMOVE_CHECK, 0);

		StartStep = 0;
		break;


	default :
		break;
	}
}

void CRun_Handler_Check::ButtonCheck_Stop()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************

	switch (StopStep)
	{
	case 0:
		// **********************************************************************
		// STOP 키 I/O 입력이 On 상태이고 현재 STOP 상태가 아닌지 검사           
		// **********************************************************************
		if (FAS_IO.Get_In_Bit(st_io.i_stop_switch_chk) == TRUE && st_work.mn_run_status != dSTOP)
		{
			// 만일 Start Button이 같이 눌렸다면 동작되지 않는다.
			if (FAS_IO.Get_In_Bit(st_io.i_start_switch_chk) == TRUE)
			{
				break;
			}

			StartStep = 0;
			StopStep = 100;
		}
		else if (FAS_IO.Get_In_Bit(st_io.i_stop_switch_chk) == TRUE)
		{
			StopStep = 100;
		}
		break;

	case 100:
		// **********************************************************************
		// START 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함        
		// **********************************************************************
		SwitchWaitTime[0] = GetCurrentTime();
		StopStep = 200;
		break;
		
	case 200: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 50 && FAS_IO.Get_In_Bit(st_io.i_stop_switch_chk) == FALSE)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			StopStep = 300;
		}
		else if (FAS_IO.Get_In_Bit(st_io.i_stop_switch_chk) == FALSE)
		{
			StopStep = 0;
		}
		break;
		
	case 300:
		// **********************************************************************
		// RUN 키 I/O 입력이 Off 되었는지 검사                                   
		// -> 알람 화면 출력 상태이면 알람 화면 종료 -> 메인 화면으로 전환       
		// **********************************************************************
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 10)
		{
			if (FAS_IO.Get_In_Bit(st_io.i_stop_switch_chk) == FALSE)
			{
				StopStep = 600; 
			}
			else
			{
				StopStep = 0;
			}
		}
		// **********************************************************************
		break;
	
	case 600:
		// **********************************************************************
		// STOP 키 I/O 입력이 Off 되었는지 검사                                  
		// **********************************************************************
		if (FAS_IO.Get_In_Bit(st_io.i_stop_switch_chk) == FALSE)
		{
			StopStep = 0; 

			if (st_work.mn_run_status == dINIT)				// 초기화 중에 Stop 버튼이 눌리면... 2K5/02/23/ViboX
			{
				st_handler.mn_initial_error = TRUE;
				
				alarm.mstr_code = _T("900003");		// Alarm 화면을 띄워준다. 2K5/03/10/ViboX
				sprintf(alarm.mc_code, "900003");
				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정
				st_work.mn_run_status = dWARNING;	// dJAM,dWARNING 
				break;
			}

			if (st_handler.mn_menu_num != 101)
			{
				::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);  // 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
			}

			Func.OnSet_IO_Port_Stop();
			alarm.mn_reset_status = CTL_NO;

		}
		break;

	default : 
		break;
	}
}

void CRun_Handler_Check::ButtonCheck_BuzzerOff()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************
	
	switch(BuzzerStep)
	{
	case 0:
		// **********************************************************************
		//  Reset 키 I/O 입력이 On 상태인지 확인                                 
		// **********************************************************************
		if (FAS_IO.Get_In_Bit(st_io.i_buzzer_switch_chk) == TRUE)
		{
			BuzzerStep = 10;
		}
		break;
		
	case 10:
		// **********************************************************************
		//  Reset 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함       
		// **********************************************************************
		SwitchWaitTime[0] = GetCurrentTime();
		BuzzerStep = 20;
		break;
		
	case 20: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 50 && FAS_IO.Get_In_Bit(st_io.i_buzzer_switch_chk) == FALSE)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			BuzzerStep = 30;
		}
		else if (FAS_IO.Get_In_Bit(st_io.i_buzzer_switch_chk) == FALSE)
		{
			BuzzerStep = 0;
		}
		break;
		
	case 30: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 10)
		{
			if (FAS_IO.Get_In_Bit(st_io.i_buzzer_switch_chk) == FALSE)
			{
				BuzzerStep = 40;
			}
			else
			{
				BuzzerStep = 0;
			}
		}
		// **********************************************************************
		break;
		
	case 40:
		// **********************************************************************
		// Sound 키 I/O 입력이 Off 되었는지 검사                                 
		// **********************************************************************
		if (FAS_IO.Get_In_Bit(st_io.i_buzzer_switch_chk) == FALSE)
		{
//			st_msg.mstr_normal_msg = _T("[SOUND] OFF.");
			sprintf(st_msg.c_normal_msg, "[SOUND] OFF.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			
			Func.OnSet_IO_Port_Sound(IO_OFF);
			
			BuzzerStep = 0;
		}
		break;
		
	default : 
		break;
	}
}

void CRun_Handler_Check::ButtonCheck_Alarm_Clear()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************
	
	switch(AlaemClearStep)
	{
	case 0:
		// **********************************************************************
		//  Reset 키 I/O 입력이 On 상태인지 확인                                 
		// **********************************************************************
		if (FAS_IO.Get_In_Bit(st_io.i_alarm_reset_switch_chk) == TRUE)
		{
			AlaemClearStep = 10;
		}
		break;
		
	case 10:
		// **********************************************************************
		//  Reset 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함       
		// **********************************************************************
		SwitchWaitTime[0] = GetCurrentTime();
		AlaemClearStep = 20;
		break;
		
	case 20: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 50 && FAS_IO.Get_In_Bit(st_io.i_alarm_reset_switch_chk) == FALSE)
		{
			SwitchWaitTime[0] = GetCurrentTime();
			AlaemClearStep = 30;
		}
		else if (FAS_IO.Get_In_Bit(st_io.i_alarm_reset_switch_chk) == FALSE)
		{
			AlaemClearStep = 0;
		}
		break;
		
	case 30: 
		SwitchWaitTime[1] = GetCurrentTime();
		SwitchWaitTime[2] = SwitchWaitTime[1] - SwitchWaitTime[0];
		
		if(SwitchWaitTime[2] > 10)
		{
			if (FAS_IO.Get_In_Bit(st_io.i_alarm_reset_switch_chk) == FALSE)
			{
				AlaemClearStep = 40;
			}
			else
			{
				AlaemClearStep = 0;
			}
		}
		break;
		
	case 40:
		// **********************************************************************
		// Reset 키 I/O 입력이 Off 되었는지 검사                                 
		// **********************************************************************
		if (FAS_IO.Get_In_Bit(st_io.i_alarm_reset_switch_chk) == FALSE)
		{
			AlaemClearStep = 110;
		}
		break;
		
	case 110:
		alarm.mn_reset_status = CTL_NO;
		sprintf(st_msg.c_normal_msg, "[ALARM CLEAR] JAM RESET.");
		if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		
		if (st_work.mn_run_status != dRUN)	Func.OnSet_IO_Port_Stop();
		FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_OFF);	// Reset Switch에 Lamp를 Off 시킨다.
		st_handler.mn_alarm_screen = FALSE;  // 현재 알람 화면이 출력되지 않았다고 해당 플래그 설정

		AlaemClearStep = 0;
		break;
		
	default : 
		break;
	}
}

void CRun_Handler_Check::LightCurtainCheck()
{
	int nRet1, nRet2, nRet[4]={0,};

// 	if (st_work.mn_run_status == dRUN)              return;
	if (COMI.mn_simulation_mode == 1)				return; // 20130509
	if (st_work.mn_run_status == dRUN || st_work.mn_run_status == dINIT || st_work.mn_run_status == dLOTREADY)//20140213 st_work.mn_run_status == dLOTREADY 추가
	{
	}
	else
	{
		return;
	}

	if (n_light_curtain_chk == CTL_YES)				return;

	nRet1 = FAS_IO.get_in_bit(st_io.i_stacker_light_curtain_chk, IO_ON);
	nRet2 = FAS_IO.get_in_bit(st_io.i_vision_light_curtain_chk, IO_ON);

	if((nRet1 == IO_ON || nRet2 == IO_ON) && alarm.n_area_alarm != TRUE)
	{
		if(i_area_check_flag == NO)
		{
			i_area_check_flag = YES;
			Area_CheckTime[0] = GetCurrentTime();
		}
		else
		{
			Area_CheckTime[1] = GetCurrentTime();
			if(Area_CheckTime[1] < Area_CheckTime[0])
			{
				i_area_check_flag = NO;
			}
			else
			{
				Area_CheckTime[2] = Area_CheckTime[1] - Area_CheckTime[0];
				if(Area_CheckTime[2] < 0) Area_CheckTime[0] = GetCurrentTime();
				if(Area_CheckTime[2] > 10 && (alarm.n_area_ready_alarm[0] == FALSE && alarm.n_area_ready_alarm[1] == FALSE))
				{
					if(nRet1 == IO_ON)	alarm.n_area_ready_alarm[0] = TRUE;
					else                alarm.n_area_ready_alarm[1] = TRUE;
					alarm.n_area_alarm = TRUE;
					Func.OnSet_IO_Port_Sound(1, 1);
				}
				if(Area_CheckTime[2] > 10000)
				{//20120825
					//900900 0 90 "Area 센서 감지 에러."
					sprintf(cJamcode,"900900");		// Alarm 화면을 띄워준다. 2K4/11/21/ViboX
					st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX
					CTL_Lib.Alarm_Error_Occurrence(5510, st_work.mn_run_status, cJamcode);
				}
			}
		}
	}
	else
	{
		i_area_check_flag = NO;
		if(alarm.n_area_ready_alarm[0] == TRUE || alarm.n_area_ready_alarm[1] == TRUE)// && alarm.n_area_alarm != TRUE)
		{
			if(alarm.n_area_alarm == TRUE)
			{
				Area_CheckTime[0] = GetCurrentTime();
				alarm.n_area_alarm = FALSE;
			}
			else
			{
				Area_CheckTime[1] = GetCurrentTime();
				Area_CheckTime[2] = Area_CheckTime[1] - Area_CheckTime[0];
				if(Area_CheckTime[2] < 0)
				{
					Area_CheckTime[0] = GetCurrentTime();
					Area_CheckTime[2] = GetCurrentTime();
				}
				if(Area_CheckTime[2] > 800)
				{
					Func.OnSet_IO_Port_Sound(0, 0);
					alarm.n_area_ready_alarm[0] = alarm.n_area_ready_alarm[1] = FALSE;
				}
			}
		}
	}

	nRet[0] = FAS_IO.get_in_bit(st_io.i_emo1_chk, IO_OFF); 
	nRet[1] = FAS_IO.get_in_bit(st_io.i_emo2_chk, IO_OFF); 
	nRet[2] = FAS_IO.get_in_bit(st_io.i_emo3_chk, IO_OFF); 
	nRet[3] = FAS_IO.get_in_bit(st_io.i_emo4_chk, IO_OFF); 

	if(st_work.mn_run_status != dWARNING && (nRet[0] == IO_ON || nRet[1] == IO_ON || nRet[2] == IO_ON || nRet[3] == IO_ON)&& 
		alarm.n_emo_alarm != TRUE)
	{
		if(i_emo_check_flag == NO)
		{
			i_emo_check_flag = YES;			// 첫번째 EMO가 감지됐다.
			EMO_CheckTime[0] = GetCurrentTime();
		}
		else
		{
			EMO_CheckTime[1] = GetCurrentTime();

			if(EMO_CheckTime[1] < EMO_CheckTime[0])		// 위에 안거치고 온거다. 다시 올라가던가.
			{
				i_emo_check_flag = NO;
			}
			else
			{
				EMO_CheckTime[2] = EMO_CheckTime[1] - EMO_CheckTime[0];

				if(EMO_CheckTime[2] > 500)		// 0.5초동안 감지되면..
				{
					if(st_work.mn_run_status == dINIT)
					{
						if(st_handler.cwnd_list != NULL)
						{
							sprintf(st_msg.c_abnormal_msg,"[Initialize Error] EMO is not check.");
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
						}
						st_handler.mn_initial_error = TRUE;
					}
					FAS_IO.set_out_bit(st_io.o_main_air, IO_OFF);
					i_emo_check_flag = NO;

					//900200 0 90 "EMO Error."
					if     (nRet[0] == FALSE) sprintf(cJamcode,"900201");		// 발생한 알람 코드 설정
					else if(nRet[1] == FALSE) sprintf(cJamcode,"900202");		// 발생한 알람 코드 설정
					else if(nRet[2] == FALSE) sprintf(cJamcode,"900203");		// 발생한 알람 코드 설정
					else                      sprintf(cJamcode,"900204");		// 발생한 알람 코드 설정
					st_work.mn_run_status = dWARNING;	// 장비 동작 상태 설정
					CTL_Lib.Alarm_Error_Occurrence(9997, st_work.mn_run_status, cJamcode);
					alarm.n_emo_alarm = TRUE;
					alarm.mn_alarm_assign_section = 63;
				}
			}
		}
	}
	else
	{
		i_emo_check_flag = NO;
	}


}

void CRun_Handler_Check::ButtonCheck_Manual()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************
	if(st_handler.mn_manual == YES) return;

	switch(ManualStep)
	{
		case 0:
			// **********************************************************************
			//  Auto 키 I/O 입력이 On 상태인지 확인                                 
			// **********************************************************************
			if (FAS_IO.get_in_bit(st_io.i_manual_mode_switch_chk, IO_OFF) == IO_ON)
			{
				Func.OnSet_Door_Open();
				manual_wait_time[0] = GetTickCount();
				ManualStep = 100;
			}
			break;
			
		case 100:
			if (FAS_IO.get_in_bit(st_io.i_manual_mode_switch_chk, IO_OFF) == IO_ON)
			{
				manual_wait_time[1] = GetCurrentTime();
				manual_wait_time[2] = manual_wait_time[1] - manual_wait_time[0];

				if(manual_wait_time[2] < 0)
				{
					manual_wait_time[0] = GetTickCount();
					break;
				}

				if(manual_wait_time[2] > 500)
				{
					manual_wait_time[0] = GetTickCount();
					ManualStep = 200;
				}
			}
			else
			{
				ManualStep = 0;
			}
			break;
			
		case 200:
			if(FAS_IO.get_in_bit(st_io.i_auto_mode_switch_chk, IO_OFF) == IO_OFF)
			{
				ManualStep = 300;
			}
			else
			{
				manual_wait_time[1] = GetCurrentTime();
				manual_wait_time[2] = manual_wait_time[1] - manual_wait_time[0];

				if(manual_wait_time[2] < 0)
				{
					manual_wait_time[0] = GetTickCount();
					break;
				}

				if(manual_wait_time[2] > 10000)
				{
					ManualStep = 0;
				}
			}
			break;
			
		case 300:
			// **********************************************************************
			//  Auto 키 I/O 입력이 Off 되었는지 검사                                 
			// **********************************************************************
			if (FAS_IO.get_in_bit(st_io.i_auto_mode_switch_chk, IO_OFF) == IO_OFF)
			{
				st_other.str_op_msg = _T("[Manual] Enable");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				
				st_handler.mn_manual = YES;

				if (st_handler.mn_menu_num != 101)
				{
//					::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
				}
				
				ManualStep = 0;
			}
			break;
			
		default : 
			break;
	}
}

void CRun_Handler_Check::ButtonCheck_Auto()
{
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************
	if(st_handler.mn_initial != TRUE) return;
	if(st_handler.mn_manual == NO) return;

	int i,nRet;

	switch(AutoStep)
	{
		case 0:
			// **********************************************************************
			//  Auto 키 I/O 입력이 On 상태인지 확인                                 
			// **********************************************************************
			if (FAS_IO.get_in_bit(st_io.i_auto_mode_switch_chk, IO_OFF) == IO_ON)
			{
				Func.OnSet_Door_Close();
				auto_wait_time[0] = GetTickCount();
				AutoStep = 100;
			}
			break;
			
		case 100: 
			if (FAS_IO.get_in_bit(st_io.i_auto_mode_switch_chk, IO_OFF) == IO_ON)
			{
				auto_wait_time[1] = GetCurrentTime();
				auto_wait_time[2] = auto_wait_time[1] - auto_wait_time[0];
				
				if(auto_wait_time[2] < 0)
				{
					auto_wait_time[0] = GetTickCount();
					break;
				}
			
				if(auto_wait_time[2] > 30)
				{
					nRet = Func.DoorOpenCheckSpot();
					if(nRet == CTLBD_RET_ERROR)
					{
						for (i = 0; i < MOT_MAXMOTOR; i++)
						{
							COMI.Set_MotStop(0, i);
						}
						st_work.mn_run_status = dWARNING;  // 장비 동작 상태 설정 //
						alarm.mn_alarm_assign_section = 7;
						AutoStep = 0;
						return;
					}
					auto_wait_time[0] = GetTickCount();
					AutoStep = 200;
				}
			}
			else
			{
				AutoStep = 0;
			}
			break;
			
		case 200:
			if (FAS_IO.get_in_bit(st_io.i_manual_mode_switch_chk, IO_OFF) == IO_OFF)//IO_ON) 20121122
			{
				AutoStep = 300;
			}
			else
			{
				auto_wait_time[1] = GetTickCount();;
				auto_wait_time[2] = auto_wait_time[1] - auto_wait_time[0];
			
				if(auto_wait_time[2] < 0)
				{
					auto_wait_time[0] = GetTickCount();
					break;
				}

				if (auto_wait_time[2] > 10)
				{
					AutoStep = 0;
				}
			}
			// **********************************************************************
			break;
			
		case 300:
			// **********************************************************************
			//  Auto 키 I/O 입력이 Off 되었는지 검사                                 
			// **********************************************************************
			if (FAS_IO.get_in_bit(st_io.i_auto_mode_switch_chk, IO_OFF) == IO_OFF)
			{
				st_other.str_op_msg = _T("[AUTO] Enable");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				
				st_handler.mn_manual = NO;

				if (st_handler.mn_menu_num != 101)
				{
					::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
				}
				
				AutoStep = 0;
			}
			break;
			
		default : 
			break;
	}
}

void CRun_Handler_Check::HandlerCheck_RejectTrayChange()
{
	// Reject Tray Full Error일때만 동작한다.
	if(alarm.n_rejecttrayfull == NO)		return;

	switch(RejectTrayStep)
	{
	case 0 :
		// **********************************************************************
		// Tray 존재 유무 및 교체 플래그 검사
		// **********************************************************************
		if(FAS_IO.get_in_bit(st_io.i_reject_tray_chk, IO_OFF) == IO_OFF && i_reject_tray_change_flag == FALSE)
		{
			// ********************************************************************
			// Tray 1.5초 이상 감지되지 않으면 제거된 것으로 인정
			// -> 존재 유무 1.5초 동안 검사하는 이유는 감지 센서 불안정한 경우 대비
			// ********************************************************************
			RejectTray_CheckTime[1] = GetCurrentTime();
			RejectTray_CheckTime[2] = RejectTray_CheckTime[1] - RejectTray_CheckTime[0];
			
			if(RejectTray_CheckTime[2] > 800)
			{
				i_reject_tray_change_flag = TRUE;

				if(st_handler.cwnd_main != NULL)
				{
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);		// Reject Tray 정보 
				}

				RejectTray_CheckTime[0] = GetCurrentTime();								// Tray 교체 시작 시간 설정
			}
		}
		else if(FAS_IO.get_in_bit(st_io.i_reject_tray_chk, IO_ON) == IO_ON && i_reject_tray_change_flag == TRUE)
		{
			// ******************************************************************
			// Tray 1.5초 이상 감지되면 공급된 것으로 인정
			// ******************************************************************
			RejectTray_CheckTime[1] = GetCurrentTime();
			RejectTray_CheckTime[2] = RejectTray_CheckTime[1] - RejectTray_CheckTime[0];
			
			if(RejectTray_CheckTime[2] > 1000)
			{
				Func.OnReject_Tray_Reset();

				alarm.n_rejecttrayfull = NO;
				i_reject_tray_change_flag = FALSE;

				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg,"[Reject Tray] Reset!!");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}

				RejectTray_CheckTime[0] = GetCurrentTime();  // Tray 제거 검사 시작 시간 설정
			}
		}
		else if(FAS_IO.get_in_bit(st_io.i_reject_tray_chk, IO_ON) == IO_ON && i_reject_tray_change_flag == FALSE)
		{
			RejectTray_CheckTime[0] = GetCurrentTime();  // Tray 제거 검사 시작 시간 설정
		}
		break;

	default :
		break;
	}
}

void CRun_Handler_Check::CheckLdClipCoverOnOff()
{
	int nRet_1,nRet_2;
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return; // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************

	if (st_handler.mn_switch_lock != FALSE)	return;	// 모달 창이 떠 있으면 실행하지 않는다.

	if(st_handler.mn_menu_num != 101)   return;

	nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_fwd_chk, IO_OFF);
	nRet_2 = FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_bwd_chk, IO_ON);

	if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
	{
		if(FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF) == IO_ON)
		{
			FAS_IO.set_out_bit(st_io.o_ldclip_tube_updn_onoff, IO_OFF);
		}
	}

	switch (LdClipCoverStep)
	{
	case 0:
		// **********************************************************************
		// RUN 키 I/O 입력이 On 상태이고 상태가 STOP 상태였는지 검사             
		// -> 다시 말해, STOP 상태에서만 RUN 키 입력이 동작한다                  
		// **********************************************************************
		if (FAS_IO.get_in_bit(st_io.i_ld_tube_s_cover_on_chk, IO_OFF) == IO_OFF && st_work.mn_run_status == dSTOP)
		{
			if (FAS_IO.get_in_bit(st_io.i_ld_tube_s_cover_on_chk, IO_ON) == IO_ON)
			{
				break;
			}

			LdClipCoverStep = 100;
		}
		break;

	case 100:
		// **********************************************************************
		// START 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함        
		// **********************************************************************
		LdCoverWaitTime[0] = GetCurrentTime();
		LdClipCoverStep = 200;
		break;

	case 200: 
		LdCoverWaitTime[1] = GetCurrentTime();
		LdCoverWaitTime[2] = LdCoverWaitTime[1] - LdCoverWaitTime[0];

		if(LdCoverWaitTime[2] > 50 && FAS_IO.get_in_bit(st_io.i_ld_tube_s_cover_on_chk, IO_OFF) == IO_OFF)
		{
			LdClipCoverStep = 300;
		}
		else if (FAS_IO.get_in_bit(st_io.i_ld_tube_s_cover_on_chk, IO_ON) == IO_ON)
		{
			LdClipCoverStep = 0;
		}
		break;

	case 300:
		FAS_IO.OnCylinderAction(st_io.o_ldclip_tube_updn_onoff, IO_ON);
		LdClipCoverStep = 310;
		break;

	case 310:
		if (FAS_IO.get_in_bit(st_io.i_ld_tube_s_cover_on_chk, IO_ON) == IO_ON && st_work.mn_run_status == dSTOP)
		{
			LdClipCoverStep = 400;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_ldclip_tube_updn_onoff, st_io.i_ldclip_tube_up_chk, st_io.i_ldclip_tube_dn_chk,
				IO_ON, st_wait.n_on_wait_time[67], st_wait.n_limit_wait_time[67]);
			if(nRet_1 == RET_GOOD)
			{
				LdClipCoverStep = 400;
			}
 			else if (nRet_1 != RET_PROCEED)
			{//900901 0 90 "LD Clip tube Cover up down Error."
				sprintf(cJamcode,"900901");
				CTL_Lib.Alarm_Error_Occurrence(9950, CTL_dWARNING, cJamcode);
				LdClipCoverStep = 300;
			}
		}
		break;

	case 400:
		if (FAS_IO.get_in_bit(st_io.i_ld_tube_s_cover_on_chk, IO_ON) == IO_ON && st_work.mn_run_status == dSTOP)
		{
			LdClipCoverStep = 410;
		}
		break;

	case 410:
		LdCoverWaitTime[0] = GetCurrentTime();
		LdClipCoverStep = 500;
		break;

	case 500: 
		LdCoverWaitTime[1] = GetCurrentTime();
		LdCoverWaitTime[2] = LdCoverWaitTime[1] - LdCoverWaitTime[0];

		if(LdCoverWaitTime[2] > 50 && FAS_IO.get_in_bit(st_io.i_ld_tube_s_cover_on_chk, IO_ON) == IO_ON)
		{
			LdClipCoverStep = 600;
		}
		else
		{
			LdClipCoverStep = 500;
		}
		break;

	case 600:
		FAS_IO.OnCylinderAction(st_io.o_ldclip_tube_updn_onoff, IO_OFF);
		LdClipCoverStep = 610;
		break;

	case 610:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_ldclip_tube_updn_onoff, st_io.i_ldclip_tube_up_chk, st_io.i_ldclip_tube_dn_chk,
			IO_OFF, st_wait.n_on_wait_time[67], st_wait.n_limit_wait_time[67]);
		if(nRet_1 == RET_GOOD)
		{
			LdClipCoverStep = 700;
		}
		else if (nRet_1 != RET_PROCEED)
		{//900901 0 90 "LD Clip tube Cover up down Error."
			sprintf(cJamcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(9951, CTL_dWARNING, cJamcode);
			LdClipCoverStep = 600;
		}
		break;

	case 700:
		LdClipCoverStep = 0;
		break;
	}
}

void CRun_Handler_Check::CheckUldClipCoverOnOff()
{
	int nRet_1;
	// **************************************************************************
    // 화면 뷰 전환 불가능한 정보 검사한다.                                      
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return; // 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************

	if (st_handler.mn_switch_lock != FALSE)	return;	// 모달 창이 떠 있으면 실행하지 않는다.

	if(st_handler.mn_menu_num != 101)   return;
	switch (UldClipCoverStep)
	{
	case 0:
		// **********************************************************************
		// RUN 키 I/O 입력이 On 상태이고 상태가 STOP 상태였는지 검사             
		// -> 다시 말해, STOP 상태에서만 RUN 키 입력이 동작한다                  
		// **********************************************************************
		if (FAS_IO.get_in_bit(st_io.i_uld_tube_s_cover_on_chk, IO_OFF) == IO_OFF && st_work.mn_run_status == dSTOP)
		{
			if (FAS_IO.get_in_bit(st_io.i_uld_tube_s_cover_on_chk, IO_ON) == IO_ON)
			{
				break;
			}

			UldClipCoverStep = 100;
		}
		break;

	case 100:
		// **********************************************************************
		// START 키가 순간적으로 On/Off 되는 것을 방지하기 위하여 대기 함        
		// **********************************************************************
		UldCoverWaitTime[0] = GetCurrentTime();
		UldClipCoverStep = 200;
		break;

	case 200: 
		UldCoverWaitTime[1] = GetCurrentTime();
		UldCoverWaitTime[2] = UldCoverWaitTime[1] - UldCoverWaitTime[0];

		if(UldCoverWaitTime[2] > 50 && FAS_IO.get_in_bit(st_io.i_uld_tube_s_cover_on_chk, IO_OFF) == IO_OFF)
		{
			UldClipCoverStep = 300;
		}
		else if (FAS_IO.get_in_bit(st_io.i_uld_tube_s_cover_on_chk, IO_ON) == IO_ON)
		{
			UldClipCoverStep = 0;
		}
		break;

	case 300:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_ON);
		UldClipCoverStep = 310;
		break;

	case 310:
		if (FAS_IO.get_in_bit(st_io.i_uld_tube_s_cover_on_chk, IO_ON) == IO_ON && st_work.mn_run_status == dSTOP)
		{
			UldClipCoverStep = 400;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_up_chk, st_io.i_uldclip_tube_dn_chk,
				IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
			if(nRet_1 == RET_GOOD)
			{
				UldClipCoverStep = 400;
			}
 			else if (nRet_1 != RET_PROCEED)
			{//900902 0 90 "ULD Clip tube Cover up down Error."
				sprintf(cJamcode,"900902");
				CTL_Lib.Alarm_Error_Occurrence(9952, CTL_dWARNING, cJamcode);
				UldClipCoverStep = 300;
			}
		}
		break;

	case 400:
		if (FAS_IO.get_in_bit(st_io.i_uld_tube_s_cover_on_chk, IO_ON) == IO_ON && st_work.mn_run_status == dSTOP)
		{
			UldClipCoverStep = 410;
		}
		break;

	case 410:
		UldCoverWaitTime[0] = GetCurrentTime();
		UldClipCoverStep = 500;
		break;

	case 500: 
		UldCoverWaitTime[1] = GetCurrentTime();
		UldCoverWaitTime[2] = UldCoverWaitTime[1] - UldCoverWaitTime[0];

		if(UldCoverWaitTime[2] > 50 && FAS_IO.get_in_bit(st_io.i_uld_tube_s_cover_on_chk, IO_ON) == IO_ON)
		{
			UldClipCoverStep = 600;
		}
		else
		{
			UldClipCoverStep = 500;
		}
		break;

	case 600:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
		UldClipCoverStep = 610;
		break;

	case 610:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_up_chk, st_io.i_uldclip_tube_dn_chk,
			IO_OFF, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			UldClipCoverStep = 700;
		}
		else if (nRet_1 != RET_PROCEED)
		{//900902 0 90 "ULD Clip tube Cover up down Error."
			sprintf(cJamcode,"900902");
			CTL_Lib.Alarm_Error_Occurrence(9953, CTL_dWARNING, cJamcode);
			UldClipCoverStep = 600;
		}
		break;

	case 700:
		UldClipCoverStep = 0;
		break;
	}
}





