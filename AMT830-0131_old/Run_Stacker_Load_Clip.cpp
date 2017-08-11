// Run_Stacker_Load_Clip.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Stacker_Load_Clip.h"
#include "Cmmsdk.h"
#include "ComizoaPublic.h"
#include "Public_Function.h"
#include "CtlBd_Function.h"
#include "Variable.h"
#include "FastechPublic_IO.h"
#include "Run_Rbt_Load_Clip.h"

#include "CtlBd_Library.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Load_Clip
CRun_Stacker_Load_Clip Run_Stacker_Load_Clip;
CRun_Stacker_Load_Clip::CRun_Stacker_Load_Clip()
{
	Thread_Variable_Initial();
}

CRun_Stacker_Load_Clip::~CRun_Stacker_Load_Clip()
{
}
/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Load_Clip message handlers
void CRun_Stacker_Load_Clip::Thread_Run()
{
	
	// **************************************************************************
    // 쓰레드 강제 리턴 조건 검사한다 (System Lock)                              
    // **************************************************************************
	if(st_handler.mn_system_lock != FALSE)  return;	// 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************
	
	switch(st_work.mn_run_status)
	{

	case dINIT:
	case dLOTREADY://2014.0519
		Run_Init();
		break;

	case dRUN:
		Run_Move();//90
		Run_Transfer();//91
		Run_LoadClip();//92
		Run_UnloadClip();//93
		Run_LoadTube();	 //20130204	//94
		break;

	case dREINSTATE :
		break;
	
	default:
		//dSTOP, dJAM, dLOTEND, dWARNING, dLOCK, dMAINT (장비가 동작하지 않는다면!)
		if(st_handler.mn_menu_num == 501)  return;   //XYZ Robot motor 화면이면..		
		InitStep = 0;
		ReinstateStep = 0;
		break;
	}
}

void CRun_Stacker_Load_Clip::Run_Init()
{
	int nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED, nRet_3 = RET_PROCEED;

	if(st_handler.mn_init_state[INIT_CLIP_STACKER] != CTL_NO) return;

	switch(InitStep)
	{
	case 0:
		InitStep = 10;
		break;

	case 10:
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_OFF || FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_ON)
		{
			if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_ON)
			{
				FAS_IO.set_out_bit(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
				FAS_IO.set_out_bit(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
				InitStep = 100;
			}
			else
			{
				InitStep = 100;
			}
		}
		else
		{
			InitStep = 100;
		}
		break;		

	case 100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON)// || nRet_4 == IO_ON)
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
			CTL_Lib.Alarm_Error_Occurrence(1500, st_work.mn_run_status, mc_alarmcode);			
		}
		else
		{
			InitStep = 1000;
		}
		break;

	case 1000:
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			InitStep = 2000;
			break;
		}
		if(FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_fwd_chk, IO_ON) == IO_OFF ||
			FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_bwd_chk, IO_OFF) == IO_ON)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_OFF);
			InitStep = 1100;
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_OFF)
			{//일단 Up시킨다
				if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON &&
					FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_dn_chk, IO_ON) == IO_ON)
				{
					InitStep = 1300;
				}
				else
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
					FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
					InitStep = 1200;
				}
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
				InitStep = 1200;
			}
		}
		break;

	case 1010:
		FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_OFF);
		InitStep = 1100;
		break;

	case 1100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_ld_tube_s_latch_fwdbwd_onoff, st_io.i_ld_tube_s_latch_fwd_chk, st_io.i_ld_tube_s_latch_bwd_chk,
			IO_ON, st_wait.n_on_wait_time[54], st_wait.n_limit_wait_time[54]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180100 0 18 "로드 튜브 stacker latch On 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180100");
				CTL_Lib.Alarm_Error_Occurrence(1501, CTL_dWARNING, mc_alarmcode);
				InitStep = 1010;
			}
			else
			{
				InitStep = 1010;
			}
		}		
		break;

	case 1110:		
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
		InitStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_long_updn_onoff, st_io.i_tube_s_m_plate_long_dn_chk, st_io.i_tube_s_m_plate_long_up_chk,
			IO_ON, st_wait.n_on_wait_time[58], st_wait.n_limit_wait_time[58]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_dn_chk, st_io.i_tube_s_m_plate_short_up_chk,
			IO_ON, st_wait.n_on_wait_time[57], st_wait.n_limit_wait_time[57]);
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			InitStep = 1300;
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
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로드 튜브 stacker plate long down 체크 에러가 발생했습니다.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"180401");
				}
				else
				{
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로드 튜브 stacker plate short down 체크 에러가 발생했습니다.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"180301");
				}
				CTL_Lib.Alarm_Error_Occurrence(1502, CTL_dWARNING, mc_alarmcode);
				InitStep = 1110;
			}
			else
			{
				InitStep = 1110;
			}
		}
		break;

	case 1300:
		if(FAS_IO.get_in_bit(st_io.i_tube_push_fwd_chk, IO_ON) == IO_ON ||
			FAS_IO.get_in_bit(st_io.i_tube_push_bwd_chk, IO_OFF) == IO_OFF)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			InitStep = 1400;
		}
		else
		{
			//if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_ON &&
			//	FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_OFF)
			//{//일단 Up시킨다
				if(FAS_IO.get_in_bit(st_io.i_uld_tube_s_dn_chk, IO_ON) == IO_ON &&
					FAS_IO.get_in_bit(st_io.i_uld_tube_s_up_chk, IO_OFF) == IO_OFF)
				{
					InitStep = 2000;
				}
				else
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
					InitStep = 1500;
				}
			//}
		}
		break;

	case 1310:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		InitStep = 1400;
		break;

	case 1400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_bwd_chk, st_io.i_tube_push_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[66], st_wait.n_limit_wait_time[66]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180100 0 18 "로드 튜브 stacker latch On 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{//180900 0 18 "튜브 Pusher forward 체크 에러가 발생했습니다."
			//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 언로드 튜브 stacker latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180901");
				CTL_Lib.Alarm_Error_Occurrence(1503, CTL_dWARNING, mc_alarmcode);
				InitStep = 1310;
			}
			else
			{
				InitStep = 1310;
			}
		}		
		break;

	case 1410:		
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
		InitStep = 1500;
		break;

	case 1500:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_dn_chk, st_io.i_uld_tube_s_up_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180602 0 18 "언로드 튜브 stacker up 에러가 발생했습니다."
		 //180603 0 18 "언로드 튜브 stacker down 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 언로드 튜브 stacker down 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180603");
//				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
//				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 	
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1504, st_work.mn_run_status, mc_alarmcode);
				InitStep = 1410;
			}
			else
			{
				InitStep = 1410;
			}
		}
		break;

	case 2000:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
			st_handler.n_plate_fwdbwd_onoff = YES;
			InitStep = 2100;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			InitStep = 2020;
		}
		break;

	case 2010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		InitStep = 2020;
		break;

	case 2020:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1548, CTL_dWARNING, mc_alarmcode);
				InitStep = 2010;
			}
			else
			{
				InitStep = 2010;
			}
		}
		break;
		
	case 2100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_bwd_onoff, st_io.i_tube_s_m_plate_fwd_chk, st_io.i_tube_s_m_plate_bwd_chk,
			IO_ON, st_wait.n_on_wait_time[56], st_wait.n_limit_wait_time[56]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 3000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180201 0 18 "로드 튜브 stacker plate backward 에러가 발생했습니다."
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로드 튜브 stacker plate backward 에러가 발생했습니다.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
			sprintf(mc_alarmcode,"180201");
//			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
//			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 	
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1505, st_work.mn_run_status, mc_alarmcode);
			InitStep = 2000;
		}
		break;


	case 3000:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF);
		if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
			InitStep = 3200;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			InitStep = 3100;
		}
		break;

	case 3010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
		InitStep = 3100;
		break;

	case 3100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_bwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON,st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 3000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				st_work.mn_run_status = CTL_dWARNING;
			    CTL_Lib.Alarm_Error_Occurrence(1506, st_work.mn_run_status, mc_alarmcode);
				InitStep = 3010;
			}
			else
			{
				InitStep = 3010;
			}
		}
		break;

	case 3110:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		InitStep = 3200;
		break;

	case 3200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
		if(nRet_1 = RET_GOOD)
		{
			InitStep = 3300;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180703 0 18 "로테이터 캡 제거 Off 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 캡 제거 Off 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180703");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1507, st_work.mn_run_status, mc_alarmcode);
				InitStep =3110;
			}
			else
			{
				InitStep = 3110;
			}
		}
		break;

	case 3300:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_up_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_dn_chk, IO_ON);
		if (nRet_1 == IO_OFF && nRet_2 == IO_ON)
		{
			InitStep = 4000;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			InitStep = 3400;
		}
		break;

	case 3310:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		InitStep = 3400;
		break;

	case 3400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 3300;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Clip buffer down 체크 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180801");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1508, st_work.mn_run_status, mc_alarmcode);
				InitStep =3110;
			}
			else
			{
				InitStep = 3110;
			}
		}
		break;

		
	case 4000:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_OFF);
		if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{
			InitStep = 4200;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, IO_OFF);
			InitStep = 4100;
		}
		break;

	case 4010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, IO_OFF);
		InitStep = 4100;
		break;

	case 4100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_0_onoff, st_io.i_tube_rotator_0_chk, st_io.i_tube_rotator_90_chk,
			IO_ON, st_wait.n_on_wait_time[59], st_wait.n_limit_wait_time[59]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 4000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180706 0 18 "로테이터 튜브 0도 에러가 발생했습니다."
			 //180707 0 18 "로테이터 튜브 90도 에러가 발생했습니다."
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 0도 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180706");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1509, st_work.mn_run_status, mc_alarmcode);
				InitStep = 4010;
			}
			else
			{
				InitStep = 4010;
			}
		}
		break;	

	case 4200:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		InitStep = 4300;
		break;

	case 4300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 4400;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
			 //180705 0 18 "로테이터 클램프 Off 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 클램프 Off 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
			    sprintf(mc_alarmcode,"180705");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1510, st_work.mn_run_status, mc_alarmcode);
				InitStep = 3410;
			}
			else
			{
				InitStep = 3410;
			}
		}
		break;

	case 4400:
		InitStep = 4500;
		break;
		
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF);
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF);

		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{
			InitStep = 4500;
		}
		else
		{//180804 0 18 "Clip buffer clip#1을 제거 해 주세요."
		 //180805 0 18 "Clip buffer clip#2을 제거 해 주세요."
			if(nRet_1 == IO_ON)
			{
				sprintf(mc_alarmcode,"180804");
			}
			else
			{
				sprintf(mc_alarmcode,"180805");
			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1511, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 4500:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_ON)
		{
			InitStep = 4510;
		}
		else
		{
			InitStep = 5000;
		}
		break;

	case 4510:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			nRet_1 = OnLoadTransferMove();
			if(nRet_1 == RET_GOOD)
			{
				InitStep = 4600;
			}
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			InitStep = 4530;
		}
		break;

	case 4520:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		InitStep = 4530;
		break;
		
	case 4530:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 4510;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1540, CTL_dWARNING, mc_alarmcode);
				InitStep = 4520;
			}
			else
			{
				InitStep = 4520;
			}
		}
		break;

	case 4600:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_ON);
		if(st_basic.n_mode_device == 1 && (nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON))
		{
			if(nRet_1 == IO_ON)
			{//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180106");
			}
			else if(nRet_2 == IO_ON)
			{//180610 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180610");
			}
			else
			{//180509 0 18 "로드 튜브 rotator에 튜브가 체크되지 않습니다."
				sprintf(mc_alarmcode,"180509");
			}
			CTL_Lib.Alarm_Error_Occurrence(1541, CTL_dWARNING, mc_alarmcode);
		}
		else
		{
			InitStep = 4700;
		}
		break;

	case 4700:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_ON);
		InitStep = 4710;
		break;

	case 4710:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_up_chk, st_io.i_uldclip_tube_dn_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			InitStep = 4720;
		}
		else if (nRet_1 != RET_PROCEED)
		{//900910 0 90 "ULD Clip tube Cover up down Error."
//			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Clip Cover Error.");
//			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(mc_alarmcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(1542, CTL_dWARNING, mc_alarmcode);
		}
		break;

	case 4720:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 500)
		{
			FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
			InitStep = 4730;
		}
		break;

	case 4730:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
		InitStep = 4740;
		break;

	case 4740:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_dn_chk, st_io.i_uldclip_tube_up_chk, 
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 4750;
		}
		else if (nRet_1 != RET_PROCEED)
		{//900910 0 90 "ULD Clip tube Cover up down Error."
//			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Clip Cover Error.");
//			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(mc_alarmcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(1543, CTL_dWARNING, mc_alarmcode);
			InitStep = 4730;
		}
		break;

	case 4750:
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
			CTL_Lib.Alarm_Error_Occurrence(1544, st_work.mn_run_status, mc_alarmcode);			
		}
		else
		{
			InitStep = 4800;
		}
		break;

	case 4800:
		nRet_1 = CheckTransferBeforeMove();
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 4900;
		}
		break;

	case 4900:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
			st_handler.n_plate_fwdbwd_onoff = YES;
			InitStep = 4910;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			InitStep = 4902;
		}
		break;

	case 4901:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		InitStep = 4902;
		break;

	case 4902:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 4900;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1547, CTL_dWARNING, mc_alarmcode);
				InitStep = 4901;
			}
			else
			{
				InitStep = 4901;
			}
		}
		break;

		
	case 4910:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_bwd_onoff, st_io.i_tube_s_m_plate_fwd_chk, st_io.i_tube_s_m_plate_bwd_chk,
			IO_ON, st_wait.n_on_wait_time[56], st_wait.n_limit_wait_time[56]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 5000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180201 0 18 "로드 튜브 stacker plate backward 에러가 발생했습니다."
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로드 튜브 stacker plate backward 에러가 발생했습니다.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
			sprintf(mc_alarmcode,"180201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1597, st_work.mn_run_status, mc_alarmcode);
			InitStep = 4900;
		}
		break;
		
	case 5000:
		st_handler.mn_init_state[INIT_CLIP_STACKER] = CTL_YES;
		InitStep = 0;
		break;
	
	}
}

void CRun_Stacker_Load_Clip::Thread_Variable_Initial()
{
	InitStep = 0;
	RunStep = 0;
	PushStep = 0;
	RotatorStep = 0;
	RunTransStep = 0;
	ClipOutStep = 0;
	mn_retry = 0;
	RunInsertStep = 0;
	mn_change_clip = NO;
	mn_Tuberetry = 0;
	Change_step = 0;
	MoveTransferStep = 0;
	MoveStep = 0;
	RunLdClipStep = 0;
	RunUldClipStep = 0;
	RunLdTubeStep = 0; //20130204
	RunUldTubeStep = 0;//20130204

	st_sync.n_lotend_clip_ldstacker_site = CTL_NO;
}

void CRun_Stacker_Load_Clip::Run_Move()
{
	int i, nRet_1, nRet_2, nRet_3, nRet_4;
	Func.ThreadFunctionStepTrace(37, RunStep);
	switch(RunStep)
	{
	case -1:
		Thread_Variable_Initial();
		RunStep = 0;
		RunTransStep = 0;
		break;

	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_clip_ldstacker_site == CTL_NO)
			{
				RunStep = 10;
			}
		}
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
					RunStep = 20;
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
					RunStep = 20;
					break;
				}
				else                            sprintf(mc_alarmcode,"180510");
			}
			alarm.mn_alarm_assign_section = 1800;
			CTL_Lib.Alarm_Error_Occurrence(1520, CTL_dWARNING, mc_alarmcode);			
		}
		else
		{
			RunStep = 20;
		}
		break;
		
	case 20:
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_fwd_chk, IO_ON) == IO_ON || FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_bwd_chk, IO_OFF) == IO_OFF ||
			FAS_IO.get_in_bit(st_io.i_tube_push_bwd_chk, IO_OFF) == IO_OFF || FAS_IO.get_in_bit(st_io.i_tube_push_fwd_chk, IO_ON) == IO_ON)
		{//현재의 FWDBWD의 Pusher 위치가 올바르지 않다 롵ㅔ이터를 들어야 한다
			RunStep = 30;
		}
		else
		{
			RunStep = 100;
		}
		break;

	case 30:

		nRet_1 = OnSetRotatorUpdn(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 40;
		}
		break;
	case 40:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_OFF) == IO_OFF && FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk,IO_ON) == IO_ON)
		{
			RunStep = 100;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
			RunStep = 50;
		}
		break;

	case 50:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		RunStep = 60;
		break;

	case 60:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 100;
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
				CTL_Lib.Alarm_Error_Occurrence(1522, st_work.mn_run_status, mc_alarmcode);			
				RunStep = 50;
			}
			else
			{
				RunStep = 50;
			}
		}
		break;

	case 100:
		nRet_1 = CheckTransferBeforeMove();
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 110;
		}
		break;

	case 110:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			if(FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_fwd_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_bwd_chk, IO_OFF) == IO_OFF)
			{
				RunStep = 200;
			}
			else
			{
				PushStep = 0;
				SetMovePusherCyliner(FALSE);
				RunStep = 130;
			}
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			RunStep = 112;
		}
		break;

	case 111:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		RunStep = 112;
		break;

	case 112:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 110;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1596, CTL_dWARNING, mc_alarmcode);
				RunStep = 111;
			}
			else
			{
				RunStep = 113;
			}
		}
		break;

	case 113:
		if(FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_fwd_chk, IO_ON) == IO_ON &&
		FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_bwd_chk, IO_OFF) == IO_OFF)
		{
			RunStep = 200;
		}
		else
		{
			PushStep = 0;
			SetMovePusherCyliner(FALSE);
			RunStep = 130;
		}
		break;
	
	case 120:
		SetMovePusherCyliner(FALSE);
		RunStep = 130;
		break;

	case 130:
		nRet_1 = GetMovePusherCyliner(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 200;
		}
		break;

		//Clip check
	case 200:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON || 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			RunStep = 220;
		}
		else
		{
			RunStep = 230;
		}
		break;

	case 220:
		nRet_1 = OnSetRotatorUpdn(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 230;
		}
		break;
		
	case 230:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk,IO_ON) == IO_ON)
		{
			RunStep = 500;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
			RunStep = 240;
		}
		break;

	case 240:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
		RunStep = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.o_tube_rotator_unclamp_onoff, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 500;
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
				CTL_Lib.Alarm_Error_Occurrence(1523, CTL_Lib.mn_run_status, mc_alarmcode);
				RunStep = 240;
			}
			else
			{
				RunStep = 240;
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
			CTL_Lib.Alarm_Error_Occurrence(1524, CTL_dWARNING, mc_alarmcode);
			RunStep = 500;
		}
		else
		{
			mn_LdTube = mn_LdRotateTube = FALSE;
			RunStep = 1000;
		}
		break;

		//동작 시작
	case 1000:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_clip_ldstacker_site == CTL_NO)
			{
				RunStep = 1100;
			}
		}
		break;

	case 1100:
		if(st_sync.n_lotend_righths_ldtray_site == CTL_YES && st_sync.n_lotend_clip_ldtray_site == CTL_YES)
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_s_empty_chk, IO_OFF);
			if(nRet_1 == IO_ON)
			{
				st_sync.n_ld_clip_tray_supply = CTL_NOTREADY;
				RunStep = 10000;
				break;
			}
		}
		st_sync.n_ld_clip_tray_supply = CTL_FREE;
		RunStep = 1200;
		break;

	case 1200:
		if(st_sync.n_ld_clip_tray_supply == CTL_REQ)
		{
			st_sync.n_ld_clip_tray_supply = CTL_READY;
			RunStep = 1300;
		}
		break;

	case 1300:
		st_handler.n_loadClip = YES;
		RunStep = 2910;
		break;

		nRet_1 = CheckTransferBeforeMove();
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 1400;
		}
		break;

	case 1400://하나빼기
		nRet_1 = OnLoadClipTransferStackerUpDn();
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 1500;
		}
		break;

	case 1500:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = IO_ON;
		if(nRet_1 == IO_ON)
		{
			RunStep = 1600;
		}
		else
		{
			//180108 0 18 "로드 튜브에 튜브가 체크되지 않았습니다."
			sprintf(mc_alarmcode,"180108");
			st_work.mn_run_status = CTL_dWARNING;			
			CTL_Lib.Alarm_Error_Occurrence(1525, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 1600:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_OFF)
		{				
			RunStep = 2000;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1526, st_work.mn_run_status, mc_alarmcode);	
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
				RunStep = 2200;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
				RunStep = 2100;
			}
		}
		break;

	case 2010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		RunStep = 2100;
		break;

	case 2100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 2200;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
			 //180705 0 18 "로테이터 클램프 Off 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 클램프 Off 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180705");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1527, st_work.mn_run_status, mc_alarmcode);
				RunStep = 2010;
			}
			else
			{
				RunStep = 2010;
			}
		}
		break;

	case 2200:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1) // Cap제거 사용 모드.
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_on_chk, IO_OFF) == IO_OFF && 
				FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_off_chk, IO_ON) == IO_ON)
			{
				RunStep = 2300;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
				RunStep = 2220;
			}
		}
		else
		{
			RunStep = 2300;
		}
		break;

	case 2210:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF); // 20140220
		RunStep = 2220;
		break;

	case 2220:
		// 20140220
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 2230;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180505 0 18 "로드 튜브 cap 제거 Off 체크 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180505");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1528, st_work.mn_run_status, mc_alarmcode);
			RunStep = 2210;
		}
		break;

	case 2230:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_OFF)
		{				
			RunStep = 2300;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1529, st_work.mn_run_status, mc_alarmcode);	
		}
		break;

	case 2300:
		nRet_1 = CheckTransferBeforeMove();
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			RunStep = 2310;
		}
		break;

	case 2310:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			nRet_1 = OnLoadTransferMove();
			if(nRet_1 == RET_GOOD)
			{
				l_Until_WaitTime[0] = GetCurrentTime();
				RunStep = 2320;
			}
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			RunStep = 2312;
		}
		break;

	case 2311:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		RunStep = 2312;
		break;
		
	case 2312:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 2311;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1540, CTL_dWARNING, mc_alarmcode);
				RunStep = 2311;
			}
			else
			{
				RunStep = 2313;
			}
		}
		break;

	case 2313:
		nRet_1 = OnLoadTransferMove();
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			RunStep = 2320;
		}
		break;
		
	case 2320:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] < 1000) break;
		nRet_1 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF);
		if(st_basic.n_mode_device != 1) nRet_1 = IO_ON;
		if(nRet_1 == IO_ON)
		{				
			RunStep = 2400;
		}
		else
		{//180509 0 18 "로드 튜브 rotator에 튜브가 체크되지 않습니다."
			sprintf(mc_alarmcode,"180509");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1530, st_work.mn_run_status, mc_alarmcode);	
		}
		break;

		
	case 2400:
		nRet_1 = OnFixLoadTube();//튜브 고정
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			mn_LdTube = TRUE;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
			RunStep = 2500;
		}
		break;

	case 2410:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
		RunStep = 2500;
		break;

	case 2500:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_clamp_chk, st_io.i_tube_rotator_unclamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 2600;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
//			if (mn_retry > 0)
//			{
//				RunStep = 2600;
//				break;
//			}
			if (mn_retry > 3)
			{//180502 0 18 "로드 튜브 rotator clamp 체크 에러가 발생했습니다."
			 //180503 0 18 "로드 튜브 rotator unclamp 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180503");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1531, st_work.mn_run_status, mc_alarmcode);
				RunStep = 2410;
			}
			else
			{
				 RunStep = 2410;
			}
		}
		break;

//	case 2600://제거요망		
//		nRet_1 = OnSetRotatorUpdn(FALSE);
//		if(nRet_1 == RET_GOOD)
//		{
//			mn_retry = 0;
//			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
//			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
//			RunStep = 2700;
//		}
//		break;

	case 2600:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk, IO_OFF) == IO_OFF)
		{
			mn_retry = 0;
			mn_Tuberetry = 0;
			
			if(st_basic.n_mode_cap_remove == 1)
			{
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_ON); // 20140220
			}
			else
			{
			}
			RunStep = 2720;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
			RunStep = 2700;
		}
		break;
		
	case 2610:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
		RunStep = 2700;
		break;

	case 2700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_clamp_chk, st_io.i_tube_rotator_unclamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 2600;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;	
			if (mn_retry > 0)
			{
				mn_retry = 0;
				mn_Tuberetry = 0;
				// 20140220
				if(st_basic.n_mode_cap_remove == 1)
				{
					FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_ON);
				}
				else
				{
				}
				RunStep = 2720;
				break;
			}
			if (mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180502");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1532, st_work.mn_run_status, mc_alarmcode);	
				RunStep = 2610;
			}
			else
			{
				RunStep = 2610;
			}
		}
		break;

	case 2710:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_ON); // 20140220
		RunStep = 2720;
		break;

	case 2720:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_on_chk, st_io.i_tube_rotator_cap_remove_off_chk,
				IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
			if(nRet_1 == RET_GOOD)
			{
				mn_Tuberetry++;
				if(mn_Tuberetry > 3)
				{
					mn_Tuberetry = 0;
					RunStep = 2800;
				}
				else
				{
					FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
					RunStep = 2722;
				}
			}
			else if(nRet_1 != RET_PROCEED)
			{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
					sprintf(mc_alarmcode,"180902");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1533, st_work.mn_run_status, mc_alarmcode);	
					RunStep = 2730;
				}
				else
				{
					FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
					RunStep = 2725;
				}
			}
		}
		else
		{
			RunStep = 2800;
		}
		break;

	case 2721:
		// 20140220
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		RunStep = 2722;
		break;

	case 2722:
		// 20140220
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			RunStep = 2710;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
				sprintf(mc_alarmcode,"180902");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1539, st_work.mn_run_status, mc_alarmcode);	
				RunStep = 2722;
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
				RunStep = 2722;
			}
		}
		break;
		
	case 2725:
		// 20140220
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 2710;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			RunStep = 2710;
		}
		break;


	case 2730:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_ON)
		{				
			RunStep = 2320;
		}
		else
		{
			RunStep = 1300;
		}
		break;
		

	case 2800:
		nRet_1 = OnSetRotatorUpdn(TRUE);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			// 20140220
			if(st_basic.n_mode_cap_remove == 1)
			{
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
			}
			else
			{
			}
			RunStep = 2820;
		}
		break;

	case 2810:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF); // 20140220
		RunStep = 2820;
		break;

	case 2820:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
				IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
			if(nRet_1 == RET_GOOD)
			{
				RunStep = 2900;
			}
			else if(nRet_1 != RET_PROCEED)
			{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
				mn_retry++;
				if(mn_retry > 3)
				{
					sprintf(mc_alarmcode,"180902");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1534, st_work.mn_run_status, mc_alarmcode);
					RunStep = 2810;
				}
				else
				{
					RunStep = 2810;
				}
			}
		}
		else
		{
			RunStep = 2900;
		}
		break;

	case 2900:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = IO_OFF;
		if (nRet_1 == IO_OFF)
		{
			mn_LdTube = FALSE;
			mn_LdRotateTube = TRUE;
			RunStep = 2910;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1535, st_work.mn_run_status, mc_alarmcode);	
		}
		break;

	case 2910:
		if(st_handler.n_loadClip == CTL_NO && st_handler.n_loadCenterChk == CTL_NO)
		{
			if(st_basic.n_mode_device == YES && st_basic.n_mode_bcr == YES)
			{
				if(st_handler.mb_bcr_read == FALSE)//2014,0218
				{
					FAS_IO.set_out_bit(st_io.o_left_clip_bcr_onoff, IO_ON);//2014,0218 off->on
					FAS_IO.set_out_bit(st_io.o_right_clip_bcr_onoff, IO_ON);//2014,0218 off->on
					l_UntilWaitTime[0] = GetCurrentTime();
					st_handler.mb_bcr_read = TRUE;
					Func.BarcodeScanStep = 0;
					RunStep = 2920;
				}
			}
			else
			{
				RunStep = 3000;
			}
		}
		break;

	case 2920:
		if(st_basic.n_mode_bcr != YES)
		{
			RunStep = 3000;
			break;
		}//Read BCR#1
		l_UntilWaitTime[1] = GetCurrentTime();
		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
		if(l_UntilWaitTime[2] < 0) l_UntilWaitTime[0] = GetCurrentTime();
		if(l_UntilWaitTime[2] < 1000) break;
		if(Func.Robot_BarcodeScanning(HEATSINK_RIGHT_PORT) == RET_GOOD)
		{
//			st_handler.mstr_bcr_id3 = st_msg.mstr_recive;
//			if(st_handler.cwnd_main != NULL)
//			{			
//				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, 5);			// 피커 정보 
//			}
//			FAS_IO.set_out_bit(st_io.o_left_clip_bcr_onoff, IO_ON);//2014,0218
			Func.BarcodeScanStep = 0;
			l_UntilWaitTime[0] = GetCurrentTime();
		}	RunStep = 2930;
		break;

	case 2930://2014,0218
//		if(st_basic.n_mode_bcr != YES)
//		{
//			RunStep = 3000;
//			break;
//		}//Read BCR#1
//		//Read BCR#2
//		l_UntilWaitTime[1] = GetCurrentTime();
//		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
//		if(l_UntilWaitTime[2] < 0) l_UntilWaitTime[0] = GetCurrentTime();
//		if(l_UntilWaitTime[2] < 1000) break;
//		if(Func.Robot_BarcodeScanning(HEATSINK_RIGHT_PORT) == RET_GOOD)
//		{
////			st_handler.mstr_bcr_id3_1 = st_msg.mstr_recive;
////			if(st_handler.cwnd_main != NULL)
////			{			
////				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, 9);			// 피커 정보 
////			}
////			FAS_IO.set_out_bit(st_io.o_right_clip_bcr_onoff, IO_OFF);//2014,0218
//			st_handler.mb_bcr_read = FALSE;
//			Func.BarcodeScanStep = 0;
//			l_UntilWaitTime[0] = GetCurrentTime();
//			RunStep = 2940;
//		}
		RunStep = 2940;
		break;

	case 2940:
		if(st_basic.n_mode_bcr != YES)
		{
			RunStep = 3000;
			break;
		}//Read BCR#1
		l_UntilWaitTime[1] = GetCurrentTime();
		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
		if(l_UntilWaitTime[2] < 0) l_UntilWaitTime[0] = GetCurrentTime();
		if(l_UntilWaitTime[2] < 1000) break;
		if(Func.Robot_BarcodeScanning(CLIP_PORT) == RET_GOOD)
		{
//			st_handler.mstr_bcr_id4 = st_msg.mstr_recive;
//			if(st_handler.cwnd_main != NULL)
//			{			
//				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, 6);			// 피커 정보 
//			}
//			FAS_IO.set_out_bit(st_io.o_right_clip_bcr_onoff, IO_ON);//2014,0218
			l_UntilWaitTime[0] = GetCurrentTime();
		}	RunStep = 2950;
		break;
		
	case 2950://2014,0218
//		if(st_basic.n_mode_bcr != YES)
//		{
//			RunStep = 3000;
//			break;
//		}//Read BCR#1
//		l_UntilWaitTime[1] = GetCurrentTime();
//		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
//		if(l_UntilWaitTime[2] < 0) l_UntilWaitTime[0] = GetCurrentTime();
//		if(l_UntilWaitTime[2] < 1000) break;
//		if(Func.Robot_BarcodeScanning(CLIP_PORT) == RET_GOOD)
//		{
////			st_handler.mstr_bcr_id4_1 = st_msg.mstr_recive;
////			if(st_handler.cwnd_main != NULL)
////			{			
////				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, 10);			// 피커 정보 
////			}
////			FAS_IO.set_out_bit(st_io.o_left_clip_bcr_onoff, IO_OFF);//2014,0218
////			FAS_IO.set_out_bit(st_io.o_right_clip_bcr_onoff, IO_OFF);//2014,0218
//		}	RunStep = 3000;
		RunStep = 3000;
		break;

	case 3000://바코드체크	
		st_sync.n_ld_clip_tray_supply = CTL_LOCK;
		for(i = 0 ; i < st_traybuffer[LDCLIP_SITE].i_loader_row ; i++)
		{
			st_modulemap.ClipTray[0][i] = DVC_YES;
			st_modulemap.ClipTray[1][i] = DVC_YES;
		}
		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_CLIP1_TRAY);			// 피커 정보 
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_CLIP2_TRAY);			// 피커 정보 
		}
		RunStep = 3100;
		break;

	case 3100:
		if(st_sync.n_lotend_clip_ldrbt == CTL_YES)
		{
			RunStep = 10000;
		}
		else if(st_sync.n_ld_clip_tray_supply == CTL_CHANGE)
		{
			for(i = 0 ; i < st_traybuffer[LDCLIP_SITE].i_loader_row ; i++)
			{
				st_modulemap.ClipTray[0][i] = DVC_NO;
				st_modulemap.ClipTray[1][i] = DVC_NO;
			}
			if(st_handler.cwnd_main != NULL)
			{			
				st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_CLIP1_TRAY);			// 피커 정보 
				st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_CLIP2_TRAY);			// 피커 정보 
			}
			RunStep = 3200;
		}
		break;

	case 3200:
		nRet_1 = ChangeLoadTray();
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 2320;
			RunStep = 2910;
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1536, st_work.mn_run_status, mc_alarmcode);			
		}
		else
		{
			RunStep = 10010;
		}
		break;

	case 10010:
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_up_chk, IO_OFF) == IO_OFF && FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_dn_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_up_chk, IO_OFF) == IO_OFF && FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
		{
			SetMovePusherCyliner(TRUE);
			RunStep = 10030;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
			RunStep = 10020;
		}
		break;

	case 10015:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
		RunStep = 10020;
		break;

	case 10020:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_long_updn_onoff, st_io.i_tube_s_m_plate_long_dn_chk, st_io.i_tube_s_m_plate_long_up_chk,
			IO_ON, st_wait.n_on_wait_time[58], st_wait.n_limit_wait_time[58]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_dn_chk, st_io.i_tube_s_m_plate_short_up_chk,
			IO_ON, st_wait.n_on_wait_time[57], st_wait.n_limit_wait_time[57]);
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			RunStep = 10010;
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
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate long up 체크 에러가 발생했습니다.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"180400");
				}
				else
				{
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate short up 체크 에러가 발생했습니다.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"180300");
				}
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1537, st_work.mn_run_status, mc_alarmcode);
				RunStep = 10015;
			}
			else
			{
				RunStep = 10015;
			}
		}
		break;

	case 10030:
		nRet_1 = GetMovePusherCyliner(TRUE);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 10100;
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
			CTL_Lib.Alarm_Error_Occurrence(1538, st_work.mn_run_status, mc_alarmcode);			
		}
		else
		{
			RunStep = 10200;
		}
		break;
		
	case 10200:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			RunStep = 10300;
		}
		else
		{//180709 0 18 "로테이터 튜브 90도 상태가 아닙니다."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1805, st_work.mn_run_status, mc_alarmcode);	
		}
		break;

	case 10300:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_OFF)
		{		
			RunStep = 10310;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1807, st_work.mn_run_status, mc_alarmcode);	
		}
		break;

	case 10310:
		if(st_sync.n_lotend_clip_ldtray_site == YES)
		{
			mn_retry = 0;
			RunStep = 10330;
		}
		break;

	case 10330:
		if(st_basic.n_mode_use_bin == YES)
		{
			RunStep = 10340;
		}
		else
		{
			RunStep = 10400;
		}
		break;

	case 10340:
		st_sync.i_remove_clip_bin = CTL_REQ;
		RunStep = 10350;
		break;

	case 10350:
		if(st_sync.i_remove_clip_bin == CTL_NO)
		{
			RunStep = 10360;
		}
		break;

	case 10360:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_ON);
		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{
			RunStep = 10385;
		}
		else
		{
			if(st_basic.n_mode_use_bin == YES)
			{
				RunStep = 10370;
			}
			else
			{
				RunStep = 10500;
			}
		}
		break;

	case 10370:
		st_sync.i_pickup_clip = CTL_REQ;
		RunStep = 10380;
		break;

	case 10380:
		if(st_sync.i_pickup_clip == CTL_NO)
		{
			RunStep = 10381;
		}
		break;

	case 10381:
		st_sync.i_remove_clip_bin = CTL_REQ;
		RunStep = 10382;
		break;

	case 10382:
		if(st_sync.i_remove_clip_bin == CTL_NO)
		{
			RunStep = 10385;
		}
		break;

	case 10385:
		st_sync.i_load_clip = CTL_REQ;
		RunStep = 10390;
		break;

	case 10390:
		if(st_sync.i_load_clip == CTL_NO)
		{
			RunStep = 10400;
		}
		break;

	case 10400:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_ON);
		if(nRet_1 == IO_OFF && nRet_2 == IO_ON)
		{
			RunStep = 10500;
		}
		else
		{
			mn_retry = 0;
			RunStep = 10410;
		}
		mn_retry = 0;
		RunStep = 10410;
		break;

	case 10410:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
		RunStep = 10420;
		break;

	case 10420:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 10430;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180701 0 18 "로테이터 튜브 latch Off 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180701");
				CTL_Lib.Alarm_Error_Occurrence(1598, CTL_dWARNING, mc_alarmcode);
				RunStep = 10410;
			}
			else
			{
				RunStep = 10410;
			}
		}
		break;

	case 10430:
		if(st_basic.n_mode_use_bin == YES)
		{
			st_sync.i_unload_clip = CTL_REQ;
			RunStep = 10440;
		}
		else
		{
			RunStep = 10500;
		}
		break;

	case 10440:
		if(st_sync.i_unload_clip == CTL_NO)
		{
			RunStep = 10450;
		}
		break;

	case 10450:
		st_sync.i_pickup_clip = CTL_REQ;
		RunStep = 10460;
		break;

	case 10460:
		if(st_sync.i_pickup_clip == CTL_NO)
		{
			RunStep = 10470;
		}
		break;

	case 10470:
		st_sync.i_remove_clip_bin = CTL_REQ;
		RunStep = 10480;
		break;

	case 10480:
		if(st_sync.i_remove_clip_bin == CTL_NO)
		{
			RunStep = 10500;
		}
		break;		

	case 10500:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_on_chk, IO_OFF) == IO_OFF &&
				FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_off_chk, IO_ON) == IO_ON)
			{
				mn_retry = 0;
				RunStep = 10503;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
				RunStep = 10502;
				break;
			}
		}
		else
		{
			RunStep = 10503;
		}
		break;

	case 10501:
		// 20140220
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		RunStep = 10502;
		break;

	case 10502:
		// 20140220
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 10500;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180703 0 18 "로테이터 캡 제거 Off 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180703");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1589, st_work.mn_run_status, mc_alarmcode);
				RunStep = 10501;
			}
			else
			{
				RunStep = 10501;
			}
		}
		break;

	case 10503:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		RunStep = 10504;
		break;

	case 10504:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_bwd_chk, st_io.i_tube_push_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[66], st_wait.n_limit_wait_time[66]);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, IO_ON);
			RunStep = 10506;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180901");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1588, st_work.mn_run_status, mc_alarmcode);
				RunStep = 10503;
			}
			else
			{
				RunStep = 10503;
			}
		}
		break;

	case 10505:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, IO_ON);
		RunStep = 10506;
		break;

	case 10506:		
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_0_onoff, st_io.i_tube_rotator_0_chk, st_io.i_tube_rotator_90_chk,
			IO_ON, st_wait.n_on_wait_time[59], st_wait.n_limit_wait_time[59]);
		if(nRet_1 == RET_GOOD)
		{
			l_UntilWaitTime[0] = GetCurrentTime();
			RunStep = 10510;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180706 0 18 "로테이터 튜브 0도 에러가 발생했습니다."
		//180707 0 18 "로테이터 튜브 90도 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				sprintf(mc_alarmcode,"180706");
				mn_retry = 0;
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1587, st_work.mn_run_status, mc_alarmcode);			
				RunStep = 10505;
			}
			else
			{
				RunStep = 10505;
			}
		}
		break;
		
	
//		nRet_1 = OnSetRotatorUpdn(FALSE);
//		if(nRet_1 == RET_GOOD)
//		{
//			mn_retry = 0;
//			l_UntilWaitTime[0] = GetCurrentTime();
//			RunStep = 10510;
//		}
//		break;

	case 10510:
		l_UntilWaitTime[1] = GetCurrentTime();
		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
		if(l_UntilWaitTime[2] < 0) l_UntilWaitTime[0] = GetCurrentTime();
		if(l_UntilWaitTime[2] < 2000) break;
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
		RunStep = 10520;
		break;

	case 10520:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 10530;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180701");
				CTL_Lib.Alarm_Error_Occurrence(1592, CTL_dWARNING, mc_alarmcode);
				RunStep = 10510;
			}
			else
			{
				RunStep = 10510;
			}
		}
		break;
		
	case 10530:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_OFF) == IO_OFF)
		{
			mn_retry = 0;
			RunStep = 10600;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
			RunStep = 10550;
		}
		break;

	case 10540:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		RunStep = 10550;
		break;

	case 10550:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			RunStep = 10600;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
			 //180705 0 18 "로테이터 클램프 Off 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 클램프 Off 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180705");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1539, st_work.mn_run_status, mc_alarmcode);
				RunStep = 10540;
			}
			else
			{
				RunStep = 10540;
			}
		}
		break;

	case 10600:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		RunStep = 10610;
		break;

	case 10610:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 10700;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1592, CTL_dWARNING, mc_alarmcode);
				RunStep = 10600;
			}
			else
			{
				RunStep = 10700;
			}
		}
		break;

	case 10700:
		nRet_1 = OnLoadTransferMove();
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			l_UntilWaitTime[0] = GetCurrentTime();
			RunStep = 10800;
		}
		break;
		
	case 10800:
		l_UntilWaitTime[1] = GetCurrentTime();
		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
		if(l_UntilWaitTime[2] < 0) l_UntilWaitTime[0] = GetCurrentTime();
		if(l_UntilWaitTime[2] < 1000) break;
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
			CTL_Lib.Alarm_Error_Occurrence(1541, CTL_dWARNING, mc_alarmcode);			
		}
		else
		{
			RunStep = 10810;//10900;//20120927
		}
		break;

	case 10810:
	///////////////// 20120927
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_ON);
		RunStep = 10820;
		break;

	case 10820:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_up_chk, st_io.i_uld_tube_s_dn_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			RunStep = 10830;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Tube Short Up Error.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			RunStep = 10810;
		}
		break;

	case 10830:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 300)
		{
			RunStep = 10911;//10900;
		}
		break;
////////////////////////

	case 10900:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_ON);
		RunStep = 10910;
		break;

	case 10910:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_up_chk, st_io.i_uldclip_tube_dn_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			RunStep = 10911;//10920;//20120927
		}
		else if (nRet_1 != RET_PROCEED)
		{//900902 0 90 "ULD Clip tube Cover up down Error."
//			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Clip Cover Error.");
//			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(mc_alarmcode,"900902");
			CTL_Lib.Alarm_Error_Occurrence(1542, CTL_dWARNING, mc_alarmcode);
			RunStep = 10900;
		}
		break;

////////////////// 20120927
	case 10911:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);//20120927
		RunStep = 10912;
		break;

	case 10912:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_dn_chk, st_io.i_uld_tube_s_up_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			RunStep = 10940;//10920;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Tube Short Dn Error.");
			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			RunStep = 10911;
		}
		break;

//////////////////

	case 10920:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 500)
		{
			FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
			RunStep = 10930;
		}
		break;

	case 10925:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
		RunStep = 10930;
		break;

	case 10930:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_dn_chk, st_io.i_uldclip_tube_up_chk, 
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			RunStep = 10940;
		}
		else if (nRet_1 != RET_PROCEED)
		{//900910 0 90 "ULD Clip tube Cover up down Error."
//			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Clip Cover Error.");
//			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(mc_alarmcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, mc_alarmcode);
			RunStep = 10925;
		}
		break;

	case 10940:		
		st_sync.n_lotend_clip_ldstacker_site = YES;
		RunStep = 1000;
		break;
	}				
}

////////////////////////////////////////////////// 20130204
void CRun_Stacker_Load_Clip::Run_LoadTube()
{
	int nRet_1 = IO_OFF, nRet_2 = IO_OFF, nRet_3 = IO_OFF;
	Func.ThreadFunctionStepTrace(97, RunLdTubeStep);
	switch(RunLdTubeStep)
	{
	case 0:
		if(st_handler.n_loadClip == CTL_YES && st_handler.n_loadCenterChk == CTL_NO)
		{
			RunLdTubeStep = 1000;
		}
		else if(st_handler.n_unloadCenterChk == CTL_YES && st_handler.n_loadCenterChk == CTL_NO)
		{
			RunLdTubeStep = 1010;
		}
		break;

	case 1000:
		nRet_1 = CheckTransferBeforeMove();
		if(nRet_1 == RET_GOOD)
		{
//			st_handler.n_loadCenterChk = CTL_YES;
			RunLdTubeStep = 1100;
		}
		break;

	case 1010:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON || 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			RunLdTubeStep = 1020;
		}
		else
		{
			RunLdTubeStep = 1020;
		}
		break;

	case 1020:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON)
		{
			//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
			//180608 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
			if(nRet_1 == IO_ON)	sprintf(mc_alarmcode,"180106");
			else				sprintf(mc_alarmcode,"180608");
			CTL_Lib.Alarm_Error_Occurrence(1524, CTL_dWARNING, mc_alarmcode);
			RunLdTubeStep = 1020;
		}
		else
		{
			RunLdTubeStep = 1100;
		}
		break;
		
	case 1100://하나빼기
		nRet_1 = OnLoadClipTransferStackerUpDn();
		if(nRet_1 == RET_GOOD)
		{
			RunLdTubeStep = 1200;
		}
		break;

	case 1200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = IO_ON;
		if(nRet_1 == IO_ON)
		{
//			RunLdTubeStep = 1300;
			RunLdTubeStep = 0;
			st_handler.n_loadCenterChk = CTL_YES;
			st_handler.n_loadClip = NO;
		}
		else
		{
			//180108 0 18 "로드 튜브에 튜브가 체크되지 않았습니다."
			sprintf(mc_alarmcode,"180108");
			st_work.mn_run_status = CTL_dWARNING;			
			CTL_Lib.Alarm_Error_Occurrence(1525, st_work.mn_run_status, mc_alarmcode);
		}
		break;
	}
}

void CRun_Stacker_Load_Clip::Run_UnloadTube()
{
	
}
//////////////////////////////////////////////////

void CRun_Stacker_Load_Clip::Run_LoadClip()
{
	int nRet_1 = IO_OFF, nRet_2 = IO_OFF, nRet_3 = IO_OFF;
	Func.ThreadFunctionStepTrace(89, RunLdClipStep);
	switch(RunLdClipStep)
	{
//	case 0:
//		if(st_handler.n_loadClip == CTL_YES)
//		{
//			RunLdClipStep = 1000;
//		}
//		else if(st_handler.n_unloadCenterChk == CTL_YES)
//		{
//			RunLdClipStep = 1010;
//		}
//		break;
//
//	case 1000:
//		nRet_1 = CheckTransferBeforeMove();
//		if(nRet_1 == RET_GOOD)
//		{
//			st_handler.n_loadCenterChk = CTL_YES;
//			RunLdClipStep = 1100;
//		}
//		break;
//
//	case 1010:
//		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON || 
//			FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
//		{
//			RunLdClipStep = 1020;
//		}
//		else
//		{
//			RunLdClipStep = 1020;
//		}
//		break;
//
//	case 1020:
//		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
//		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
//		if(nRet_1 == IO_ON || nRet_2 == IO_ON)
//		{
//			//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
//			//180608 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
//			if(nRet_1 == IO_ON)	sprintf(mc_alarmcode,"180106");
//			else				sprintf(mc_alarmcode,"180608");
//			CTL_Lib.Alarm_Error_Occurrence(1524, CTL_dWARNING, mc_alarmcode);
//			RunLdClipStep = 1020;
//		}
//		else
//		{
//			RunLdClipStep = 1100;
//		}
//		break;
//		
//	case 1100://하나빼기
//		nRet_1 = OnLoadClipTransferStackerUpDn();
//		if(nRet_1 == RET_GOOD)
//		{
//			RunLdClipStep = 1200;
//		}
//		break;
//
//	case 1200:
//		nRet_1 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_ON);
//		if(st_basic.n_mode_device != 1) nRet_1 = IO_ON;
//		if(nRet_1 == IO_ON)
//		{
//			RunLdClipStep = 1300;
//		}
//		else
//		{
//			//180108 0 18 "로드 튜브에 튜브가 체크되지 않았습니다."
//			sprintf(mc_alarmcode,"180108");
//			st_work.mn_run_status = CTL_dWARNING;			
//			CTL_Lib.Alarm_Error_Occurrence(1525, st_work.mn_run_status, mc_alarmcode);
//		}
//		break;
//
//	case 1300:
//		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_OFF)
//		{				
//			RunLdClipStep = 1400;
//		}
//		else
//		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
//			sprintf(mc_alarmcode,"180508");
//			st_work.mn_run_status = CTL_dWARNING;
//			CTL_Lib.Alarm_Error_Occurrence(1526, st_work.mn_run_status, mc_alarmcode);	
//		}
//		break;

//	case 1400:
	case 0:
		if(st_handler.n_loadCenterChk == CTL_YES)//CTL_READY)
		{
			RunLdClipStep = 2000;
		}
		else if(st_handler.n_unloadCenterChk == CTL_YES)
		{
			RunLdClipStep = 2100;
		}
		break;

	case 2000:
		nRet_1 = CheckTransferBeforeMove();
		if(nRet_1 == RET_GOOD)
		{
			mn_Ldretry = 0;
			RunLdClipStep = 2310;
		}
		break;

	case 2100:
		nRet_1 = CheckTransferBeforeMove(YES);
		if(nRet_1 == RET_GOOD)
		{
			mn_Ldretry = 0;
			RunLdClipStep = 4000;
		}
		break;

	case 2310:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			RunLdClipStep = 2313;
		}
		else
		{
			mn_Ldretry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			RunLdClipStep = 2312;
		}
		break;

	case 2311:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		RunLdClipStep = 2312;
		break;
		
	case 2312:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			RunLdClipStep = 2313;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Ldretry++;
			if(mn_Ldretry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_Ldretry = 0;
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1540, CTL_dWARNING, mc_alarmcode);
				RunLdClipStep = 2311;
			}
			else
			{
				RunLdClipStep = 2313;
			}
		}
		break;

	case 2313:
		nRet_1 = OnLoadTransferMove();
		if(nRet_1 == RET_GOOD)
		{
			l_Until_Ld_WaitTime[0] = GetCurrentTime();
			RunLdClipStep = 2320;
		}
		break;
		
	case 2320:
		l_Until_Ld_WaitTime[1] = GetCurrentTime();
		l_Until_Ld_WaitTime[2] = l_Until_Ld_WaitTime[1] - l_Until_Ld_WaitTime[0];
		if(l_Until_Ld_WaitTime[2] < 0) l_Until_Ld_WaitTime[0] = GetCurrentTime();
		if(l_Until_Ld_WaitTime[2] < 1000) break;
		nRet_1 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF);
		if(st_basic.n_mode_device != 1) nRet_1 = IO_ON;
		if(nRet_1 == IO_ON)
		{				
			RunLdClipStep = 2400;
		}
		else
		{//180509 0 18 "로드 튜브 rotator에 튜브가 체크되지 않습니다."
			sprintf(mc_alarmcode,"180509");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1530, st_work.mn_run_status, mc_alarmcode);	
		}
		break;
		
	case 2400:
		nRet_1 = OnFixLoadTube();//튜브 고정
		if(nRet_1 == RET_GOOD)
		{
			mn_Ldretry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
			RunLdClipStep = 2500;
		}
		break;

	case 2410:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
		RunLdClipStep = 2500;
		break;

	case 2500:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_clamp_chk, st_io.i_tube_rotator_unclamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			RunLdClipStep = 2600;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Ldretry++;
			if (mn_Ldretry > 3)
			{//180502 0 18 "로드 튜브 rotator clamp 체크 에러가 발생했습니다."
			 //180503 0 18 "로드 튜브 rotator unclamp 체크 에러가 발생했습니다."
				mn_Ldretry = 0;
				sprintf(mc_alarmcode,"180503");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1531, st_work.mn_run_status, mc_alarmcode);
				RunLdClipStep = 2410;
			}
			else
			{
				 RunLdClipStep = 2410;
			}
		}
		break;

	case 2600:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk, IO_OFF) == IO_OFF)
		{
			mn_Ldretry = 0;
			mn_Tuberetry = 0;
			if(st_basic.n_mode_cap_remove == 1)
			{
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_ON); // 20140220
			}
			else
			{
			}
			RunLdClipStep = 2720;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
			RunLdClipStep = 2700;
		}
		break;
		
	case 2610:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
		RunLdClipStep = 2700;
		break;

	case 2700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_clamp_chk, st_io.i_tube_rotator_unclamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			RunLdClipStep = 2600;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Ldretry++;
			if (mn_Ldretry > 3)
			{
				mn_Ldretry = 0;
				sprintf(mc_alarmcode,"180502");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1532, st_work.mn_run_status, mc_alarmcode);	
				RunLdClipStep = 2610;
			}
			else
			{
				RunLdClipStep = 2610;
			}
		}
		break;

	case 2710:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_ON); // 20140220
		RunLdClipStep = 2720;
		break;

	case 2720:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_on_chk, st_io.i_tube_rotator_cap_remove_off_chk,
				IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
			if(nRet_1 == RET_GOOD)
			{
				mn_Tuberetry = 0;
				RunLdClipStep = 2800;
			}
			else if(nRet_1 != RET_PROCEED)
			{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
				mn_Ldretry++;
				if(mn_Ldretry > 6)
				{
					mn_Ldretry = 0;
					FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
					sprintf(mc_alarmcode,"180902");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1533, st_work.mn_run_status, mc_alarmcode);	
					RunLdClipStep = 2730;
				}
				else
				{
					FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
					RunLdClipStep = 2725;
				}
			}
		}
		else
		{
			RunLdClipStep = 2800;
		}
		break;

	case 2721:
		// 20140220
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		RunLdClipStep = 2722;
		break;

	case 2722:
		// 20140220
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
		if(nRet_1 == RET_GOOD)
		{
			mn_Ldretry = 0;
			RunLdClipStep = 2710;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
			mn_Ldretry++;
			if(mn_Ldretry > 3)
			{
				mn_Ldretry = 0;
				FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
				sprintf(mc_alarmcode,"180902");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1539, st_work.mn_run_status, mc_alarmcode);	
				RunLdClipStep = 2722;
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
				RunLdClipStep = 2722;
			}
		}
		break;
		
	case 2725:
		// 20140220
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
		if(nRet_1 == RET_GOOD)
		{
			RunLdClipStep = 2710;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			RunLdClipStep = 2710;
		}
		break;


	case 2730:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_ON)
		{				
			RunLdClipStep = 2320;
		}
//		else
//		{
//			RunLdClipStep = 1300;
//		}
		break;
		

	case 2800:
		nRet_1 = OnSetRotatorUpdn(TRUE);
		if(nRet_1 == RET_GOOD)
		{
			mn_Ldretry = 0;
			if(st_basic.n_mode_cap_remove == 1)
			{
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF); // 20140220
			}
			else
			{
			}
			RunLdClipStep = 2820;
		}
		break;

	case 2810:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF); // 20140220
		RunLdClipStep = 2820;
		break;

	case 2820:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
				IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
			if(nRet_1 == RET_GOOD)
			{
				RunLdClipStep = 2900;
				l_Until_Ld_WaitTime[0] = GetCurrentTime();
			}
			else if(nRet_1 != RET_PROCEED)
			{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
				mn_Ldretry++;
				if(mn_Ldretry > 3)
				{
					mn_Ldretry = 0;
					sprintf(mc_alarmcode,"180902");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1534, st_work.mn_run_status, mc_alarmcode);
					RunLdClipStep = 2810;
				}
				else
				{
					RunLdClipStep = 2810;
				}
			}
		}
		else
		{
			RunLdClipStep = 2900;
		}
		break;

	case 2900:
		l_Until_Ld_WaitTime[1] = GetCurrentTime();
		l_Until_Ld_WaitTime[2] = l_Until_Ld_WaitTime[1] - l_Until_Ld_WaitTime[0];
		if(l_Until_Ld_WaitTime[2] < 0) l_Until_Ld_WaitTime[0] = GetCurrentTime();
		if(l_Until_Ld_WaitTime[2] < 1000) break;

		nRet_1 = FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = IO_OFF;
		if (nRet_1 == IO_OFF)
		{
			mn_LdRotateTube = TRUE;
			RunLdClipStep = 3000;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1535, st_work.mn_run_status, mc_alarmcode);	
		}
		break;

	case 3000:
		st_handler.n_loadClip = CTL_NO;
		st_handler.n_loadCenterChk = CTL_NO;
		RunLdClipStep = 0;
		break;

	case 4000:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		RunLdClipStep = 4100;
		break;

	case 4100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			RunLdClipStep = 4200;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Ldretry++;
			if(mn_Ldretry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_Ldretry = 0;
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1590, CTL_dWARNING, mc_alarmcode);
				RunLdClipStep = 4000;
			}
			else
			{
				RunLdClipStep = 4000;
			}
		}
		break;

	case 4200:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			RunLdClipStep = 4300;
		}
		else
		{//180708 0 18 "로테이터 튜브 0도 상태가 아닙니다."
			sprintf(mc_alarmcode,"180708");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1805, st_work.mn_run_status, mc_alarmcode);	
		}
		break;

	case 4300:
		nRet_1 = OnSetRotatorUpdn(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			mn_Ldretry = 0;
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_OFF) == IO_OFF)
			{
				RunLdClipStep = 4400;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
				RunLdClipStep = 4400;
			}
		}
		break;
		
	case 4310:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		RunLdClipStep = 4400;
		break;

	case 4400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			RunLdClipStep = 4500;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Ldretry++;
			if(mn_Ldretry > 3)
			{
			 //180705 0 18 "로테이터 클램프 Off 에러가 발생했습니다."
				mn_Ldretry = 0;
				sprintf(mc_alarmcode,"180705");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1539, st_work.mn_run_status, mc_alarmcode);
				RunLdClipStep = 4310;
			}
			else
			{
				RunLdClipStep = 4310;
			}
		}
		break;

	case 4500:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			MoveTransferStep = 0;
			RunLdClipStep = 4600;
		}
		else
		{
			mn_Ldretry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			RunLdClipStep = 4520;
		}
		break;

	case 4510:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		RunLdClipStep = 4520;
		break;

	case 4520:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			RunLdClipStep = 4500;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Ldretry++;
			if(mn_Ldretry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_Ldretry = 0;
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1540, CTL_dWARNING, mc_alarmcode);
				RunLdClipStep = 4510;
			}
			else
			{
				MoveTransferStep = 0;
				l_Until_Ld_WaitTime[0] = GetCurrentTime();
				RunLdClipStep = 4530;
			}
		}
		break;

	case 4530:
		l_Until_Ld_WaitTime[1] = GetCurrentTime();
		l_Until_Ld_WaitTime[2] = l_Until_Ld_WaitTime[1] - l_Until_Ld_WaitTime[0];
		if(l_Until_Ld_WaitTime[2] < 0) l_Until_Ld_WaitTime[0] = GetCurrentTime();
		if(l_Until_Ld_WaitTime[2] < 500) break;
		RunLdClipStep = 4600;
		break;

	case 4600:
		nRet_1 = OnLoadTransferMove();
		if(nRet_1 == RET_GOOD)
		{
			l_Until_Ld_WaitTime[0] = GetCurrentTime();
			RunLdClipStep = 4700;
		}
		break;

	case 4700:
		l_Until_Ld_WaitTime[1] = GetCurrentTime();
		l_Until_Ld_WaitTime[2] = l_Until_Ld_WaitTime[1] - l_Until_Ld_WaitTime[0];
		if(l_Until_Ld_WaitTime[2] < 0) l_Until_Ld_WaitTime[1] = GetCurrentTime();
		if(l_Until_Ld_WaitTime[2] < 2000) break;
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
				alarm.mstr_pcode = _T("180509");
			}
			CTL_Lib.Alarm_Error_Occurrence(1541, CTL_dWARNING, mc_alarmcode);			
		}
		else
		{
			RunLdClipStep = 5000;
		}
		break;

	case 5000:
		if(st_handler.n_unloadCenterChk == CTL_YES)
		{
			st_handler.n_unloadCenterChk = CTL_READY;
			RunLdClipStep = 5100;
			///////////////////////////////////////////// 20130206
			l_Until_Ld_WaitTime[0] = GetCurrentTime();
			RunLdClipStep = 2320;
			///////////////////////////////////////////// 
		}
		break;

	case 5100:
		if(st_handler.n_loadClip == CTL_YES)
		{
// 			l_Until_Ld_WaitTime[0] = GetCurrentTime();
// 			RunLdClipStep = 2320;
		}
		break;
	}
}

void CRun_Stacker_Load_Clip::Run_UnloadClip()
{
	int nRet_1 = IO_OFF, nRet_2 = IO_OFF;
	Func.ThreadFunctionStepTrace(88, RunUldClipStep);
	switch(RunUldClipStep)
	{
	case 0:
		if(st_handler.n_loadCenterChk == CTL_YES && st_handler.n_unloadCenterChk == CTL_READY)
		{
			l_Until_Uld_WaitTime[0] = GetCurrentTime();
			RunUldClipStep = 1000;
		}
		else if(st_handler.n_unloadClip == CTL_YES)
		{
			RunUldClipStep = 5000;
			st_handler.n_unloadCenterChk = CTL_YES;
		}
		break;

	case 900:
		l_Until_Uld_WaitTime[0] = GetCurrentTime();
		RunUldClipStep = 1000;
		break;

	case 1000:
		l_Until_Uld_WaitTime[1] = GetCurrentTime();
		l_Until_Uld_WaitTime[2] = l_Until_Uld_WaitTime[1] - l_Until_Uld_WaitTime[0];
		if(l_Until_Uld_WaitTime[2] < 0) l_Until_Uld_WaitTime[0] = GetCurrentTime();
		nRet_1 = FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF);
		if(nRet_1 == IO_ON && nRet_2 == IO_OFF)
		{
			RunUldClipStep = 2000;
		}
		else if(l_Until_Uld_WaitTime[2] > 20000)
		{
			sprintf(mc_alarmcode,"180700");
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
			CTL_Lib.Alarm_Error_Occurrence(1570, st_work.mn_run_status, mc_alarmcode);
			RunUldClipStep = 900;
		}
		break;

	case 2000:
		nRet_1 = OnSetRotatorUpdn(FALSE);
		if(nRet_1 == RET_GOOD)
		{
			mn_Uldretry = 0;
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_unclamp_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_tube_rotator_clamp_chk, IO_OFF) == IO_OFF)
			{
				RunUldClipStep = 2200;
			}
			else
			{
				mn_Uldretry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
				RunUldClipStep = 2100;
			}
		}
		break;

	case 2010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		RunUldClipStep = 2100;
		break;

	case 2100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			RunUldClipStep = 2200;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Uldretry++;
			if(mn_Uldretry > 3)
			{
			 //180705 0 18 "로테이터 클램프 Off 에러가 발생했습니다."
				mn_Uldretry = 0;
				sprintf(mc_alarmcode,"180705");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1527, st_work.mn_run_status, mc_alarmcode);
				RunUldClipStep = 2010;
			}
			else
			{
				RunUldClipStep = 2010;
			}
		}
		break;

	case 2200:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_on_chk, IO_OFF) == IO_OFF && 
				FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_off_chk, IO_ON) == IO_ON)
			{
				RunUldClipStep = 2300;
			}
			else
			{
				mn_Uldretry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
				RunUldClipStep = 2220;
			}
		}
		else
		{
			RunUldClipStep = 2300;
		}
		break;

	case 2210:
		// 20140220
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		RunUldClipStep = 2220;
		break;

	case 2220:
		// 20140220
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
		if(nRet_1 == RET_GOOD)
		{
			RunUldClipStep = 2230;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180505 0 18 "로드 튜브 cap 제거 Off 체크 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180505");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1528, st_work.mn_run_status, mc_alarmcode);
			RunUldClipStep = 2210;
		}
		break;

	case 2230:
		if(FAS_IO.get_in_bit(st_io.i_rotate_tube_chk, IO_OFF) == IO_OFF)
		{				
			RunUldClipStep = 2300;
		}
		else
		{//180508 0 18 "로드 튜브 rotator에 튜브가 체크되었습니다. 제거해 주세요."
			sprintf(mc_alarmcode,"180508");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1529, st_work.mn_run_status, mc_alarmcode);	
		}
		break;	

	case 2300:
		st_handler.n_loadCenterChk = CTL_READY;	
		RunUldClipStep = 0;
		break;

	case 5000:
//		st_handler.n_unloadCenterChk = CTL_YES;
		RunUldClipStep = 5100;
		break;

	case 5100:
		if(st_handler.n_unloadCenterChk == CTL_READY)
		{
			st_handler.n_loadClip = CTL_YES;
			RunUldClipStep = 5200;
		}
		break;

	case 5200:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_ON);
		RunUldClipStep = 5300;
		break;

	case 5300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_up_chk, st_io.i_uld_tube_s_dn_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_Uld_WaitTime[0] = GetCurrentTime();
			RunUldClipStep = 5400;
		}
		else if (nRet_1 != RET_PROCEED)
		{//180602 0 18 "언로드 튜브 stacker up 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180602");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1568, st_work.mn_run_status, mc_alarmcode);
			RunUldClipStep = 5200;
		}
		break;

	case 5400:
		l_Until_Uld_WaitTime[1] = GetCurrentTime();
		l_Until_Uld_WaitTime[2] = l_Until_Uld_WaitTime[1] - l_Until_Uld_WaitTime[0];
		if(l_Until_Uld_WaitTime[2] < 0) l_Until_Uld_WaitTime[0] = GetCurrentTime();
		if(l_Until_Uld_WaitTime[2] > 300)
		{
			RunUldClipStep = 5700;//5500;
		}
		break;

	case 5500:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_ON);
		RunUldClipStep = 5600;
		break;

	case 5600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_up_chk, st_io.i_uldclip_tube_dn_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			RunUldClipStep = 5700;//3500;//20120927
		}
		else if (nRet_1 != RET_PROCEED)
		{//900902 0 90 "ULD Clip tube Cover up down Error."
			sprintf(mc_alarmcode,"900902");
			CTL_Lib.Alarm_Error_Occurrence(1595, CTL_dWARNING, mc_alarmcode);
			RunUldClipStep = 5500;
		}
		break;

////////////////// 20120927
	case 5700:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);//20120927
		RunUldClipStep = 5800;
		break;

	case 5800:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_dn_chk, st_io.i_uld_tube_s_up_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_Uld_WaitTime[0] = GetCurrentTime();
			RunUldClipStep = 6100;//5900;
		}
		else if (nRet_1 != RET_PROCEED)
		{//180603 0 18 "언로드 튜브 stacker down 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180603");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1569, st_work.mn_run_status, mc_alarmcode);
			RunUldClipStep = 5700;
		}
		break;
//////////////////

	case 5900:
		l_Until_Uld_WaitTime[1] = GetCurrentTime();
		l_Until_Uld_WaitTime[2] = l_Until_Uld_WaitTime[1] - l_Until_Uld_WaitTime[0];
		if(l_Until_Uld_WaitTime[2] < 0) l_Until_Uld_WaitTime[0] = GetCurrentTime();
		if(l_Until_Uld_WaitTime[2] > 500)
		{
			FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
			RunUldClipStep = 6000;
		}
		break;

	case 5910:
		FAS_IO.OnCylinderAction(st_io.o_uldclip_tube_updn_onoff, IO_OFF);
		RunUldClipStep = 6000;
		break;

	case 6000:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uldclip_tube_updn_onoff, st_io.i_uldclip_tube_dn_chk, st_io.i_uldclip_tube_up_chk, 
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			RunUldClipStep = 6100;
		}
		else if (nRet_1 != RET_PROCEED)
		{//900910 0 90 "ULD Clip tube Cover up down Error."
			sprintf(mc_alarmcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(1596, CTL_dWARNING, mc_alarmcode);
			RunUldClipStep = 5910;
		}
		break;

	case 6100:
		st_handler.n_unloadClip = CTL_NO;
		st_handler.n_unloadCenterChk = CTL_NO;
		RunUldClipStep = 0;
		break;
		
	}
}

void CRun_Stacker_Load_Clip::Run_Transfer()
{
	int nRet_1,nRet_2;

	if(st_sync.n_lotend_clip_ldrbt == CTL_YES)
	{
		if(RunTransStep >= 1510 && RunTransStep < 2000)
		{
			RunTransStep = 2000;
		}
	}

	Func.ThreadFunctionStepTrace(38, RunTransStep);
	switch(RunTransStep)
	{
	case -1:
		RunTransStep = 1000;
		break;

	case 0:
		RunTransStep = 100;
		break;

	case 100:
		if(st_sync.n_lotend_clip_ldtray_site == CTL_YES) return;
		//if tube is exist, ready
		if(FAS_IO.get_in_bit(st_io.i_ld_tube_s_empty_chk, IO_ON) == IO_ON || st_basic.n_mode_device != 1)
		{
			if(st_sync.n_ld_clip_tray_supply == CTL_FREE)// CTL_FREE)
			{
				RunTransStep = 1000;
			}
		}
		else
		{
			if (st_sync.n_ld_clip_tray_supply == CTL_NOTREADY)		// 텅 비었을 경우에...
			{			
				RunTransStep = 10000;
			}
		}
		break;

	case 1000:
		if(st_sync.n_module_clip_req[0] == CTL_REQ)
		{
			st_sync.n_module_clip_req[0] = CTL_LOCK;
			RunTransStep = 1100;
		}
		else if(st_sync.n_lotend_clip_ldrbt == CTL_YES)
		{
			RunTransStep = 10000;
		}
		break;

	case 1100:
		if(st_sync.n_ld_clip_tray_supply == CTL_FREE)
		{
			st_sync.n_ld_clip_tray_supply = CTL_REQ;
			mn_UnloadTubeNum = 0; //튜브 개수 세팅
			mn_change_clip = NO;
			RunTransStep = 1200;
		}
		else if(st_sync.n_ld_clip_tray_supply == CTL_LOCK)//준비됨
		{
			RunTransStep = 1200;
		}
		break;

	case 1200:
		if(st_sync.n_ld_clip_tray_supply == CTL_LOCK)
		{
			RunTransStep = 1300;
		}
		break;

	case 1300:
		RunTransStep = 1400;
		break;

	case 1400:
		if(st_sync.n_module_ldrbt_clipbuffer_req[0] == CTL_READY)
		{
			mn_retry = 0;
			//st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_LOCK;
			RunTransStep = 1510;
		}
		if(st_sync.n_lotend_clip_ldrbt == CTL_YES)
		{
			RunTransStep = 2000;
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
			FAS_IO.Set_Out_Bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);
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
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			//FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
			l_UntilWaitTime[0] = GetCurrentTime();	
			RunTransStep = 1700;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180701 0 18 "로테이터 튜브 latch Off 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode, "180701");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1542, st_work.mn_run_status, mc_alarmcode);
				RunTransStep = 1520;
			}
			else
			{
				RunTransStep = 1520;
			}
		}
		break;

	case 1700:
		l_UntilWaitTime[1] = GetCurrentTime();
		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
		if(l_UntilWaitTime[2] < 0) l_UntilWaitTime[0] = GetCurrentTime();
		if(l_UntilWaitTime[2] < 500) break;
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
//			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_OFF);
//			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_ON);
			RunTransStep = 1820;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180700");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1544, st_work.mn_run_status, mc_alarmcode);
				RunTransStep = 1710;
			}
			else
			{
				RunTransStep = 1820;
			}
		}
		break;

	case 1810:
		RunTransStep = 1820;
		break;


	case 1820:
		mn_retry = 0;
		RunTransStep = 1900;
		break;

	case 1900:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_ON); // 1851
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = nRet_2 = IO_ON;
		if(nRet_1 == IO_OFF || nRet_2 == IO_OFF)
		{//180802 0 18 "Clip buffer clip#1 Clip이 체크가 되지 않습니다."
		 //180803 0 18 "Clip buffer clip#2 Clip이 체크가 되지 않습니다."
			mn_UnloadTubeNum++;
			mn_Tuberetry++;
			if(nRet_1 == IO_OFF && nRet_2 == IO_OFF && mn_Tuberetry > 3)// && mn_UnloadTubeNum > st_basic.mn_lotset_clip) //20121105
			{
				/*
				if(nRet_1 == IO_OFF) sprintf(mc_alarmcode,"180802");
				else                 sprintf(mc_alarmcode,"180803"); 
				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
				alarm.mn_alarm_assign_section = 1843;
				CTL_Lib.Alarm_Error_Occurrence(1851, st_work.mn_run_status, mc_alarmcode);*/
				mn_change_clip = YES;
				st_sync.n_module_clip_req[0] = CTL_CHANGE;
				RunTransStep = 3000;
			}
			else
			{
				if(nRet_1 == IO_ON || nRet_2 == IO_ON)
				{
					mn_retry++;
					if(mn_retry > 3)
					{
						mn_retry = 0;
						FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);
						if(nRet_1 == IO_OFF) sprintf(mc_alarmcode,"180802");
						else                 sprintf(mc_alarmcode,"180803"); 
//						CTL_Lib.Alarm_Error_Occurrence(1851, CTL_dWARNING, mc_alarmcode);
						st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_CAP_REMOVE_CHECK, 0);
						RunTransStep = 1960;
					}
				}
				else
				{
					RunTransStep = 1910;
				}
			}
		}
		else
		{
			mn_UnloadTubeNum++;
			if(st_basic.n_mode_device != 1 && mn_UnloadTubeNum > st_basic.mn_lotset_clip)
			{
				mn_change_clip = YES;
				st_sync.n_module_clip_req[0] = CTL_CHANGE;
				RunTransStep = 3000;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);
				RunTransStep = 1906;
			}
		}
		break;

	case 1905:
//		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		RunTransStep = 1906;
		break;

	case 1906:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
			IO_OFF, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			st_sync.n_module_clip_req[0] = CTL_FREE;
			RunTransStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{////180800 0 18 "Clip buffer Up 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180800");
				CTL_Lib.Alarm_Error_Occurrence(1889, CTL_dWARNING, mc_alarmcode);
				RunTransStep = 1905;
			}
			else
			{
				RunTransStep = 1905;
			}
		}
		break;

	case 1910:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = nRet_2 = IO_ON;
		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{
			//모터 이동 후 실린더 업하는 수동 동작 추가 후 처리
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			RunTransStep = 1920;
		}
		else
		{
			FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);
			l_Until_WaitTime[0] = GetCurrentTime();
			RunTransStep = 1950;
		}
		break;

	case 1915:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		RunTransStep = 1920;
		break;

	case 1920:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			RunTransStep = 1930;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] Clip buffer down 체크 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180801");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1890, st_work.mn_run_status, mc_alarmcode);
				RunTransStep = 1915;
			}
			else
			{
				RunTransStep = 1915;
			}
		}
		break;

	case 1930:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].md_pos[LD_CLIP_POS], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 1940;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			RunTransStep = 1930;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(1891, CTL_dWARNING, COMI.mc_alarmcode);
			RunTransStep = 1930;
		}
		break;

	case 1940:
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);
		l_Until_WaitTime[0] = GetCurrentTime();
		RunTransStep = 1945;
		break;

	case 1945:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 500)
		{
			RunTransStep = 1100;
		}
		break;

	case 1950:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 500)
		{
			mn_retry = 0;
			RunTransStep = 1900;
		}
		break;

	case 1960:
		if(st_sync.n_module_ldrbt_clamp_req[0] == CTL_NO ||
			st_sync.n_module_ldrbt_clamp_req[0] == CTL_READY || 
			st_sync.n_module_ldrbt_clamp_req[0] == CTL_NOTREADY)
		{
			RunTransStep = 1961;
		}
		break;

	case 1961:
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		l_Until_WaitTime[0] = GetCurrentTime();
		RunTransStep = 1962;
		break;

	case 1962:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 200)
		{
			RunTransStep = 1963;
		}
		break;

	case 1963:
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);
		l_Until_WaitTime[0] = GetCurrentTime();
		RunTransStep = 1964;
		break;

	case 1964:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] > 200)
		{
			RunTransStep = 1965;
		}
		break;

	case 1965:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_ON); // 1851
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = nRet_2 = IO_ON;
		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{
			RunTransStep = 1905;
		}
		else
		{
			RunTransStep = 1970;
		}
		break;
		
		
	case 1970:
		//2016.0525
		//if( st_sync.n_module_ldrbt_clamp_req[0] == CTL_READY ) break;

		if(Run_Rbt_Load_Clip.Run_RemoveClip() == RET_GOOD)
		{
			RunTransStep = 1975;
		}
		break;

	case 1975:
		if(st_basic.n_curr_clamp_clip_bin >= st_basic.n_clamp_clip_bin)
		{//995000 0 99 "Clamp에서 Clip 제거 Bin 통을 제거해 주세요."
			sprintf(mc_alarmcode,"995000"); 
			CTL_Lib.Alarm_Error_Occurrence(1892, CTL_dWARNING, mc_alarmcode);
		}
//		else if(st_basic.n_curr_clip_bin >= st_basic.n_clip_bin)
//		{//995001 0 99 "Clip 제거 Bin 통을 제거해 주세요."
//			sprintf(mc_alarmcode,"995001"); 
//			CTL_Lib.Alarm_Error_Occurrence(1893, CTL_dWARNING, mc_alarmcode);
//		}
		else
		{
			RunTransStep = 1980;
		}
		break;

	case 1980:
		if(Run_Rbt_Load_Clip.PickUpClipForBin() == RET_GOOD)
		{
			RunTransStep = 1990;
		}
		else if(Run_Rbt_Load_Clip.PickUpClipForBin() == RET_ERROR)//2015.0131
		{
			RunTransStep = 1995;
		}
		break;

	case 1990:
		if(Run_Rbt_Load_Clip.Run_RemoveClip() == RET_GOOD)
		{
			RunTransStep = 1995;
		}
		break;
		
	case 1995:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON)
		{//180804 0 18 "Clip buffer clip#1을 제거 해 주세요."
		 //180805 0 18 "Clip buffer clip#2을 제거 해 주세요."
			if(nRet_1 == IO_ON)
			{
				sprintf(mc_alarmcode,"180804"); 
			}
			else
			{
				sprintf(mc_alarmcode,"180805"); 
			}
			CTL_Lib.Alarm_Error_Occurrence(2894, CTL_dWARNING, mc_alarmcode);
			break;
		}


		if(st_basic.n_curr_clamp_clip_bin >= st_basic.n_clamp_clip_bin)
		{//995000 0 99 "Clamp에서 Clip 제거 Bin 통을 제거해 주세요."
			sprintf(mc_alarmcode,"995000"); 
			CTL_Lib.Alarm_Error_Occurrence(1894, CTL_dWARNING, mc_alarmcode);
		}
//		else if(st_basic.n_curr_clip_bin >= st_basic.n_clip_bin)
//		{//995001 0 99 "Clip 제거 Bin 통을 제거해 주세요."
//			sprintf(mc_alarmcode,"995001"); 
//			CTL_Lib.Alarm_Error_Occurrence(1895, CTL_dWARNING, mc_alarmcode);
//		}
		else
		{
			RunTransStep = 1930;
		}
		break;


	case 2000:
		st_modulemap.ClipTray[0][mn_UnloadTubeNum] = DVC_NO; //트레이에 디바이스가 없음을 셋
		st_modulemap.ClipTray[1][mn_UnloadTubeNum] = DVC_NO; //트레이에 디바이스가 없음을 셋
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
			mn_UnloadTubeNum = 0;
			mn_change_clip = NO;
			st_sync.n_ld_clip_tray_supply = CTL_CHANGE;
			RunTransStep = 3100;
		}
		else
		{
			st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_CHANGE;
			RunTransStep = 1000;
		}
		break;

	case 3100:
		st_sync.n_module_clip_req[0] = CTL_CHANGE;
		RunTransStep = 1000;
		break;


		
	case 10000:
		st_sync.n_lotend_clip_ldtray_site = CTL_YES;
		RunTransStep = 0;
		break;
		
	}
}

//Rotoator를 UP한다
//Clamp는 따로 해야 한다
int CRun_Stacker_Load_Clip::OnSetRotatorUpdn(BOOL bUpDn)
{
	int nRet_1, nFuncRet = RET_PROCEED;
	switch(RotatorStep)
	{
		// 20140219
	case 0: 
		if(st_basic.n_mode_cap_remove == 1)
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_on_chk, IO_OFF) == bUpDn &&
				FAS_IO.get_in_bit(st_io.i_tube_rotator_cap_remove_off_chk, IO_ON) == !bUpDn || COMI.mn_simulation_mode == 1) // 20130509
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
				FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, bUpDn); // 20140220
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
		}
		else
		{
			//추가 위치 시퀀스 변경.
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			RotatorStep = 80;
			break;
		}
		break;

	case 5:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, bUpDn); // 20140220
		RotatorStep = 10;
		break;

	case 10:
		// 20140220
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_on_chk, st_io.i_tube_rotator_cap_remove_off_chk,
			bUpDn, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
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
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 캡 제거 Off 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180703");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1550, st_work.mn_run_status, mc_alarmcode);
				RotatorStep = 5;
			}
			else
			{
				RotatorStep = 5;
			}
		}
		break;

	case 80: // 20140220 
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			RotatorStep = 100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(8176, CTL_dWARNING, mc_alarmcode);
				FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);
				nFuncRet = RET_ERROR;
				RotatorStep = 0;
			}
			else
			{
				RotatorStep = 0;
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
			!bUpDn, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			RotatorStep = 200;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
         //180701 0 18 "로테이터 튜브 latch Off 에러가 발생했습니다."
			if(bUpDn == TRUE)
			{
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"180701");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1551, st_work.mn_run_status, mc_alarmcode);			
					RotatorStep = 90;
				}
				else
				{
					RotatorStep = 90;
				}
			}
			else
			{
				mn_retry = 0;
				RotatorStep = 200;
			}
		}
		break;

	case 110:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		RotatorStep = 200;
		break;

	case 200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_bwd_chk, st_io.i_tube_push_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[66], st_wait.n_limit_wait_time[66]);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			l_UntilWaitTime[0] = GetCurrentTime();
			RotatorStep = 210;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180901");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1552, st_work.mn_run_status, mc_alarmcode);
				RotatorStep = 110;
			}
			else
			{
				RotatorStep = 110;
			}
		}
		break;

	case 210:
		l_UntilWaitTime[1] = GetCurrentTime();
		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
		if(l_UntilWaitTime[2] < 0) l_UntilWaitTime[0] = GetCurrentTime();
		if(l_UntilWaitTime[2] < 200) break;
		RotatorStep = 220;
		break;

	case 220:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, bUpDn);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, !bUpDn);
		RotatorStep = 300;
		break;

	case 300:		
		if(bUpDn == TRUE)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_90_onoff, st_io.i_tube_rotator_90_chk, st_io.i_tube_rotator_0_chk,
				IO_ON, st_wait.n_on_wait_time[60], st_wait.n_limit_wait_time[60]);
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_0_onoff, st_io.i_tube_rotator_0_chk, st_io.i_tube_rotator_90_chk,
				IO_ON, st_wait.n_on_wait_time[59], st_wait.n_limit_wait_time[59]);
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
				CTL_Lib.Alarm_Error_Occurrence(1553, st_work.mn_run_status, mc_alarmcode);			
				RotatorStep = 220;
			}
			else
			{
				RotatorStep = 220;
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

void CRun_Stacker_Load_Clip::Run_Reinstate()
{
}

void CRun_Stacker_Load_Clip::SetMovePusherCyliner(BOOL bFwdMove)
{
	if(bFwdMove == TRUE)
	{
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
		//FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		st_handler.n_plate_fwdbwd_onoff = YES;
	}
	else
	{
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
		//FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_ON);
		st_handler.n_plate_fwdbwd_onoff = NO;
	}
	mn_retry = 0;
}

int CRun_Stacker_Load_Clip::GetMovePusherCyliner(BOOL bFwdMove)
{
	int nFuncRet = RET_PROCEED;
	int nRet_1, nRet_2;

	switch(PushStep)
	{
	case 0:
		if(bFwdMove == TRUE)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_fwd_onoff, st_io.i_tube_s_m_plate_fwd_chk,
				st_io.o_tube_s_m_plate_bwd_onoff, IO_ON, st_wait.n_on_wait_time[55], st_wait.n_limit_wait_time[55]);
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
					CTL_Lib.Alarm_Error_Occurrence(1561, st_work.mn_run_status, mc_alarmcode);			
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
				st_io.o_tube_s_m_plate_fwd_onoff, IO_ON, st_wait.n_on_wait_time[55], st_wait.n_limit_wait_time[55]);
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
					CTL_Lib.Alarm_Error_Occurrence(1562, st_work.mn_run_status, mc_alarmcode);		
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
			st_handler.n_plate_fwdbwd_onoff = YES;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
//			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_ON);
			st_handler.n_plate_fwdbwd_onoff = NO;
		}
		PushStep = 0;
		break;
	}
	return nFuncRet;
}

int CRun_Stacker_Load_Clip::CheckTransferBeforeMove(int nChange)
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_3;

	Func.ThreadFunctionStepTrace(84, MoveStep);
	switch(MoveStep)
	{
	case 0:
		if(nChange == NO)
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_OFF) == IO_OFF)
			{
				if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_up_chk, IO_OFF) == IO_OFF &&
					FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
				{
					MoveStep = 200;
				}
				else
				{
					mn_retry = 0;
					FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
					MoveStep = 100;
				}
			}
			else
			{//180707 0 18 "로테이터 튜브 90도 에러가 발생했습니다."
				sprintf(mc_alarmcode,"180707");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1563, st_work.mn_run_status, mc_alarmcode);			
			}
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_up_chk, IO_OFF) == IO_OFF &&
				FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
			{
				MoveStep = 200;
			}
			else
			{
				mn_retry = 0;
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
				MoveStep = 100;
			}
		}
		break;

	case 10:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_short_updn_onoff, IO_OFF);
		MoveStep = 100;
		break;

	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_dn_chk,
			st_io.i_tube_s_m_plate_short_up_chk, IO_ON, st_wait.n_on_wait_time[57], st_wait.n_limit_wait_time[57]);
		if(nRet_1 == RET_GOOD)
		{
			MoveStep = 200;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180301 0 18 "로드 튜브 stacker plate short down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180301");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1564, st_work.mn_run_status, mc_alarmcode);
			}
			else
			{
				MoveStep = 10; 
			}
		}
		break;

	case 200:
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_up_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_dn_chk, IO_ON) == IO_ON)
		{
			MoveStep = 1000;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
			MoveStep = 300;
		}
		break;

	case 210:
		FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
		MoveStep = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_long_updn_onoff, st_io.i_tube_s_m_plate_long_dn_chk,
			st_io.i_tube_s_m_plate_long_up_chk, IO_ON, st_wait.n_on_wait_time[58], st_wait.n_limit_wait_time[58]);
		if(nRet_1 == RET_GOOD)
		{
			MoveStep = 200;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180401 0 18 "로드 튜브 stacker plate long down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180401");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1565, st_work.mn_run_status, mc_alarmcode);
			}
			else
			{
				MoveStep = 210;
			}
		}
		break;

	case 1000:
		if(FAS_IO.get_in_bit(st_io.i_uld_tube_s_up_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_uld_tube_s_dn_chk, IO_ON) == IO_ON)
		{
			MoveStep = 1200;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
			MoveStep = 1100;
		}
		break;
		
	case 1010:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
		MoveStep = 1100;
		break;

	case 1100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_dn_chk,
			st_io.i_uld_tube_s_up_chk, IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			mn_retry = 0;
			MoveStep = 1200;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180603 0 18 "언로드 튜브 stacker down 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180603");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1566, st_work.mn_run_status, mc_alarmcode);
			}
			else
			{
				MoveStep = 1010;
			}
		}
		break;

	case 1110:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		MoveStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_bwd_chk,
			st_io.i_tube_push_fwd_chk, IO_ON, st_wait.n_on_wait_time[66], st_wait.n_limit_wait_time[66]);
		if(nRet_1 == RET_GOOD)
		{
			MoveStep = 1300;
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
				CTL_Lib.Alarm_Error_Occurrence(1567, st_work.mn_run_status, mc_alarmcode);
			}
			else
			{
				MoveStep = 1110;
			}
		}
		break;

	case 1300:
		nRet_3 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		if(nRet_3 == IO_ON)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_ON);
			MoveStep = 1400;
		}
		else
		{
			MoveStep = 2000;
		}
		break;

	case 1310:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_ON);
		MoveStep = 1400;
		break;

	case 1400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_up_chk, st_io.i_uld_tube_s_dn_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			MoveStep = 1500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180602 0 18 "언로드 튜브 stacker up 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180602");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1568, st_work.mn_run_status, mc_alarmcode);
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
			MoveStep = 1600;
		}
		break;

	case 1510:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);
		MoveStep = 1600;
		break;

	case 1600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_dn_chk, st_io.i_uld_tube_s_up_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			MoveStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180603 0 18 "언로드 튜브 stacker down 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180603");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1569, st_work.mn_run_status, mc_alarmcode);
		}
		break;

		
	case 2000:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			MoveStep = 3000;
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			MoveStep = 2100;
		}
		break;

	case 2010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		MoveStep = 2100;
		break;

	case 2100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			MoveStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
				CTL_Lib.Alarm_Error_Occurrence(1570, st_work.mn_run_status, mc_alarmcode);
				MoveStep = 2010;
			}
			else
			{
				MoveStep = 2010;//2015.0130
			}
		}
		break;

	case 3000:
		MoveStep = 0;
		nFuncRet = RET_GOOD;
		break;
	}

	return nFuncRet;
}

int CRun_Stacker_Load_Clip::OnLoadTransferMove()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2;
	
	Func.ThreadFunctionStepTrace(79, MoveTransferStep);
	switch(MoveTransferStep)
	{
	case 0:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_OFF) == IO_OFF)
		{
			if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_up_chk, IO_OFF) == IO_OFF &&
				FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
			{
				MoveTransferStep = 100;
			}
			else
			{//180301 0 18 "로드 튜브 stacker plate short down 체크 에러가 발생했습니다."
				sprintf(mc_alarmcode,"180301");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1571, st_work.mn_run_status, mc_alarmcode);
			}
		}
		else
		{//180707 0 18 "로테이터 튜브 90도 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180707");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1572, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 100:
		if(FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_long_up_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_tube_s_m_plate_short_dn_chk, IO_ON) == IO_ON)
		{
			FAS_IO.set_out_bit(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			MoveTransferStep = 200;
		}
		else
		{//180403 0 18 "로드 튜브 stacker plate long down 위치에 있지 않습니다."
			sprintf(mc_alarmcode,"180403");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1573, st_work.mn_run_status, mc_alarmcode);
			//FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);//2014.0519
		}
		break;

	case 200:
		if(FAS_IO.get_in_bit(st_io.i_uld_tube_s_up_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_uld_tube_s_dn_chk, IO_ON) == IO_ON)
		{
			MoveTransferStep = 300;
		}
		else
		{//180605 0 18 "언로드 튜브 stacker down 위치에 있지 않습니다."
			sprintf(mc_alarmcode,"180605");
			st_work.mn_run_status = CTL_dWARNING;			
			CTL_Lib.Alarm_Error_Occurrence(1574, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 300:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			st_handler.n_plate_fwdbwd_onoff = NO;
			mn_retry = 0;
			MoveTransferStep = 1000;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			MoveTransferStep = 400;
		}
		break;

	case 310:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		MoveTransferStep = 400;
		break;

	case 400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			MoveTransferStep = 300;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
				CTL_Lib.Alarm_Error_Occurrence(1575, st_work.mn_run_status, mc_alarmcode);
				nFuncRet = RET_ERROR;
				MoveTransferStep = 310;
			}
			else
			{
				MoveTransferStep = 300;
			}
		}
		break;

	case 900:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			st_handler.n_plate_fwdbwd_onoff = NO;
			MoveTransferStep = 1000;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			MoveTransferStep = 920;
		}
		break;

	case 910:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		MoveTransferStep = 920;
		break;

	case 920:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			MoveTransferStep = 900;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1576, CTL_dWARNING, mc_alarmcode);
				MoveTransferStep = 910;
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
				FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
				FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
				st_handler.n_plate_fwdbwd_onoff = NO;
				MoveTransferStep = 1000;
			}
		}
		break;
		
	case 1000:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_bwd_onoff, st_io.i_tube_s_m_plate_bwd_chk, st_io.i_tube_s_m_plate_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[56], st_wait.n_limit_wait_time[56]);
//		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_s_m_plate_fwd_chk, st_io.i_tube_s_m_plate_bwd_chk,
//			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		nRet_2 = RET_GOOD;
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			MoveTransferStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180201 0 18 "로드 튜브 stacker plate backward 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1577, st_work.mn_run_status, mc_alarmcode);
				FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
				FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
				MoveTransferStep = 900;
			}
			else
			{
//				FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);//20120921
//				FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
				l_UntilWaitTime[0] = GetCurrentTime();
				MoveTransferStep = 1100;
			}
		}
		break;

	case 1100:
		l_UntilWaitTime[1] = GetCurrentTime();
		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
		if(l_UntilWaitTime[2] > 1000)
		{
			MoveTransferStep = 900;
		}
		else if(l_UntilWaitTime[2] < 0)
		{
			l_UntilWaitTime[0] = GetCurrentTime();
		}
		break;
		
	case 2000:
		MoveTransferStep = 0;
		nFuncRet = RET_GOOD;
		break;
	}

	return nFuncRet;
}

int CRun_Stacker_Load_Clip::OnLoadClipTransferStackerUpDn()//하나빼기
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
			if(st_sync.n_lotend_righths_ldtray_site == CTL_YES && st_sync.n_lotend_clip_ldtray_site == CTL_YES)
			{
				nFuncRet = RET_GOOD;
				break;
			}
		}
		else
		{//180107 0 18 "로드 스태커에 튜브가 존재하지 않습니다. 튜브를 넣어 주세요."
			sprintf(mc_alarmcode,"180107");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1580, st_work.mn_run_status, mc_alarmcode);
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
			//180202 0 18 "로드 튜브 stacker plate forward 위치에 있지 않습니다."
			//alarm.mstr_pcode = _T("180202");
			//alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
			//alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			//alarm.mn_alarm_assign_section = 1813;
			//st_work.mn_run_status = CTL_dWARNING;
			//CtlBdFunc.Alarm_Error_Occurrence(alarm.mn_count_mode, alarm.mn_type_mode, st_work.mn_run_status, alarm.mstr_pcode);
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
			IO_ON, st_wait.n_on_wait_time[58], st_wait.n_limit_wait_time[58]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_dn_chk, st_io.i_tube_s_m_plate_short_up_chk,
			IO_ON, st_wait.n_on_wait_time[57], st_wait.n_limit_wait_time[57]);
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
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate long up 체크 에러가 발생했습니다.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"180400");
				}
				else
				{
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate short up 체크 에러가 발생했습니다.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"180300");
				}
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1581, st_work.mn_run_status, COMI.mc_alarmcode);
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
			IO_ON, st_wait.n_on_wait_time[58], st_wait.n_limit_wait_time[58]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_s_m_plate_short_updn_onoff, st_io.i_tube_s_m_plate_short_up_chk, st_io.i_tube_s_m_plate_short_dn_chk,
			IO_ON, st_wait.n_on_wait_time[57], st_wait.n_limit_wait_time[57]);
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
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate long up 체크 에러가 발생했습니다.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"180400");
				}
				else
				{
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"[Error] 로드 튜브 stacker plate short up 체크 에러가 발생했습니다.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"180300");
				}
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1582, st_work.mn_run_status, mc_alarmcode);
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
			IO_ON, st_wait.n_on_wait_time[54], st_wait.n_limit_wait_time[54]);
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
				CTL_Lib.Alarm_Error_Occurrence(1583, st_work.mn_run_status, COMI.mc_alarmcode);
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
			IO_ON, st_wait.n_on_wait_time[57], st_wait.n_limit_wait_time[57]);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
////2014.0519
//			FAS_IO.OnCylinderAction(st_io.o_ld_tube_s_latch_fwdbwd_onoff, IO_OFF);
//			TransUpDnStep = 2000;
//			TransUpDnStep = 1200;
////
			TransUpDnStep = 1100;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180301 0 18 "로드 튜브 stacker plate short down 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180301");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1584, st_work.mn_run_status, COMI.mc_alarmcode);
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
			IO_ON, st_wait.n_on_wait_time[54], st_wait.n_limit_wait_time[54]);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
////2014.0519
//			FAS_IO.OnCylinderAction(st_io.o_tube_s_m_plate_long_updn_onoff, IO_OFF);
//			TransUpDnStep = 2000;
////
			TransUpDnStep = 1300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180100 0 18 "로드 튜브 stacker latch On 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180100");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1585, st_work.mn_run_status, COMI.mc_alarmcode);
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
			IO_ON, st_wait.n_on_wait_time[58], st_wait.n_limit_wait_time[58]);
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
				CTL_Lib.Alarm_Error_Occurrence(1586, CTL_dWARNING, mc_alarmcode);
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


int CRun_Stacker_Load_Clip::OnFixLoadTube()
{
	int nRet_1, nFuncRet = RET_PROCEED;
	Func.ThreadFunctionStepTrace(85, FixStep);
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
			IO_ON, st_wait.n_on_wait_time[66], st_wait.n_limit_wait_time[66]);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			FixStep = 1100;
			FixStep = 2000;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180900 0 18 "튜브 Pusher forward 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180900");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1587, st_work.mn_run_status, mc_alarmcode);	
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
			IO_ON, st_wait.n_on_wait_time[66], st_wait.n_limit_wait_time[66]);
		if(nRet_1 == RET_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
			FixStep = 2000;
		}
		else if (nRet_1 == RET_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180901");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1588, st_work.mn_run_status, mc_alarmcode);
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


int CRun_Stacker_Load_Clip::ChangeLoadTray()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2,nRet_3,nRet_4;

	Func.ThreadFunctionStepTrace(83, Change_step);
	switch(Change_step)
	{
	case 0:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_tube_s_full_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_ld_tube_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_uld_tube_chk, IO_OFF);
		nRet_4 = FAS_IO.get_in_bit(st_io.i_ld_tube_s_empty_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON)// || nRet_4 == IO_ON)
		{
			if(nRet_1 == IO_ON)
			{//180607 0 18 "언로드 튜브 stacker full 체크 에러가 발생했습니다."
				sprintf(mc_alarmcode,"180607");
			}
			else if(nRet_2 == IO_ON)
			{//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
				sprintf(mc_alarmcode,"180106");
			}
			else if(nRet_3 == IO_ON)
			{//180608 0 18 "언로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
// 				mn_retry++;
// 				if(mn_retry >2)
// 				{
// 					mn_retry = 0;
 					sprintf(mc_alarmcode,"180608");
// 				}
// 				else
// 				{
// 					Change_step = 3330;
// 				}
				
			}
			//else
			//{//180510 0 18 "로드 튜브 stacker empty 체크 에러가 발생했습니다."
			//	alarm.mstr_pcode = _T("180510");
			//}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1590, st_work.mn_run_status, mc_alarmcode);			
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
		if(FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_fwd_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_ld_tube_s_latch_bwd_chk, IO_ON) == IO_ON)
		{
			Change_step = 1000;
		}
		else
		{
			PushStep = 0;
			SetMovePusherCyliner(TRUE);
			Change_step = 320;
		}		
		break;

	case 310:
		SetMovePusherCyliner(TRUE);
		Change_step = 320;
		break;

	case 320:
		nRet_1 = GetMovePusherCyliner(TRUE);
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 1000;
		}
		break;

	case 1000:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON || 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			Change_step = 1100;
		}
		else
		{
			Change_step = 1100;
		}
		break;

	case 1100:
		st_handler.n_unloadClip = CTL_YES;
		Change_step = 3300;
		break;
		//nRet_1 = OnSetRotatorUpdn(FALSE);
		//if(nRet_1 == RET_GOOD)
		//{
			Change_step = 1200;
		//}
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
			CTL_Lib.Alarm_Error_Occurrence(1524, CTL_dWARNING, mc_alarmcode);
			Change_step = 1200;
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
			CTL_Lib.Alarm_Error_Occurrence(1525, st_work.mn_run_status, mc_alarmcode);
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
			CTL_Lib.Alarm_Error_Occurrence(1526, st_work.mn_run_status, mc_alarmcode);	
		}
		break;

	case 2300:	
		nRet_1 = CheckTransferBeforeMove(YES);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			Change_step = 2301;
		}
		break;

	case 2301:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		Change_step = 2302;
		break;


	case 2302:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 2310;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180700 0 18 "로테이터 튜브 latch On 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1590, CTL_dWARNING, mc_alarmcode);
				Change_step = 2301;
			}
			else
			{
				Change_step = 2310;
			}
		}
		break;

	case 2310:
		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_90_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_tube_rotator_0_chk, IO_OFF) == IO_OFF)
		{
			Change_step = 2400;
		}
		else
		{//180708 0 18 "로테이터 튜브 0도 상태가 아닙니다."
			sprintf(mc_alarmcode,"180708");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1805, st_work.mn_run_status, mc_alarmcode);	
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
				Change_step = 2600;
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
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
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
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 클램프 Off 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180705");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1539, st_work.mn_run_status, mc_alarmcode);
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
			MoveTransferStep = 0;
			Change_step = 3100;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
			Change_step = 3020;
		}
		break;

	case 3010:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);
		Change_step = 3020;
		break;


	case 3020:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_fwd_onoff, st_io.i_tube_rotator_latch_bwd_onoff,
			IO_ON, st_wait.n_on_wait_time[63], st_wait.n_limit_wait_time[63]);
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
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] 로테이터 튜브 latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180700");
				CTL_Lib.Alarm_Error_Occurrence(1540, CTL_dWARNING, mc_alarmcode);
				Change_step = 3010;
			}
			else
			{
				MoveTransferStep = 0;
				l_UntilWaitTime[0] = GetCurrentTime();
				Change_step = 3050;
			}
		}
		break;

	case 3050:
		l_UntilWaitTime[1] = GetCurrentTime();
		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
		if(l_UntilWaitTime[2] < 0) l_UntilWaitTime[0] = GetCurrentTime();
		if(l_UntilWaitTime[2] < 500) break;
		Change_step = 3100;
		break;

	case 3100:
//		if(FAS_IO.get_in_bit(st_io.i_tube_rotator_latch_bwd_onoff, IO_OFF) == IO_OFF &&
//			FAS_IO.get_in_bit(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON) == IO_ON)
//		{
			nRet_1 = OnLoadTransferMove();
			if(nRet_1 == RET_GOOD)
			{
				l_Until_WaitTime[0] = GetCurrentTime();
				Change_step = 3200;
			}
//		}
//		else
//		{
//			Change_step = 3000;
//		}
		break;

	case 3200:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0) l_Until_WaitTime[0] = GetCurrentTime();
		if(l_Until_WaitTime[2] < 2000) break;
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
				alarm.mstr_pcode = _T("180509");
			}
			CTL_Lib.Alarm_Error_Occurrence(1541, CTL_dWARNING, mc_alarmcode);			
		}
		else
		{
			Change_step = 3300;
		}
		break;

	///////////////// 20120927
	case 3300:
		if(st_handler.n_unloadClip == CTL_NO)
		{
			Change_step = 4000;
		}
		break;
// 		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_ON);
// 		Change_step = 3310;
// 		break;

	case 3310:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_up_chk, st_io.i_uld_tube_s_dn_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			Change_step = 3320;
		}
		else if (nRet_1 != RET_PROCEED)
		{//180602 0 18 "언로드 튜브 stacker up 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180602");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1568, st_work.mn_run_status, mc_alarmcode);
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
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			Change_step = 3410;//3500;//20120927
		}
		else if (nRet_1 != RET_PROCEED)
		{//900902 0 90 "ULD Clip tube Cover up down Error."
//			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Clip Cover Error.");
//			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(mc_alarmcode,"900902");
			CTL_Lib.Alarm_Error_Occurrence(1595, CTL_dWARNING, mc_alarmcode);
			Change_step = 3400;
		}
		break;

////////////////// 20120927
	case 3410:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_s_updn_onoff, IO_OFF);//20120927
		Change_step = 3420;
		break;

	case 3420:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_s_updn_onoff, st_io.i_uld_tube_s_dn_chk, st_io.i_uld_tube_s_up_chk,
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
			Change_step = 3500;
		}
		else if (nRet_1 != RET_PROCEED)
		{//180603 0 18 "언로드 튜브 stacker down 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180603");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1569, st_work.mn_run_status, mc_alarmcode);
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
			IO_ON, st_wait.n_on_wait_time[68], st_wait.n_limit_wait_time[68]);
		if(nRet_1 == RET_GOOD)
		{
			Change_step = 4000;
		}
		else if (nRet_1 != RET_PROCEED)
		{//900910 0 90 "ULD Clip tube Cover up down Error."
//			sprintf(st_msg.c_abnormal_msg, "[ULDClipCover] UnLoad Clip Cover Error.");
//			if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);

			sprintf(mc_alarmcode,"900901");
			CTL_Lib.Alarm_Error_Occurrence(1596, CTL_dWARNING, mc_alarmcode);
			Change_step = 3510;
		}
		break;

	case 4000:
//		mn_retry = 0;
//		Change_step = 2310;
		nFuncRet = RET_GOOD;
		Change_step = 0;
		break;
	}

	return nFuncRet;
}
