// Run_Rbt_VisionLabel.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Rbt_VisionLabel.h"
#include "ComizoaPublic.h"
#include "CtlBd_Function.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Library.h"
#include "Cmmsdk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_VisionLabel
CRun_Rbt_VisionLabel Run_Rbt_VisionLabel;
CRun_Rbt_VisionLabel::CRun_Rbt_VisionLabel()
{
	Thread_Variable_Initial();
}

CRun_Rbt_VisionLabel::~CRun_Rbt_VisionLabel()
{
}
/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_VisionLabel message handlers
void CRun_Rbt_VisionLabel::Thread_Run()
{
	// **************************************************************************
    // 쓰레드 강제 리턴 조건 검사한다 (System Lock)                              
    // **************************************************************************
	if(st_handler.mn_system_lock != FALSE)  return;	// 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************



	switch(st_work.mn_run_status)
	{
	case dINIT:
	case dLOTREADY:
		Run_Init();
		break;

	case dRUN:
		Run_Label();
		Run_Module_Vision_Top();
		Run_Module_Vision_Btm();
		break;

	case dREINSTATE:
		break;
	
	default:
		//dSTOP, dJAM, dLOTEND, dWARNING, dLOCK, dMAINT (장비가 동작하지 않는다면!)
		
 		if(st_handler.mn_menu_num == 501)  return;   //XYZ Robot motor 화면이면..		
		InitStep = 0;
		ReinstateStep = 0;
		break;
	}
}

void CRun_Rbt_VisionLabel::Run_Init()
{
	int i, nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	int nRet_3 = RET_PROCEED, nRet_4 = RET_PROCEED;
	
	if(st_handler.mn_init_state[INIT_LAVEL_VISION] != CTL_NO) return;

	switch(InitStep)
	{ 
	case 0:
		InitStep = 10;
		break;

	case 10:
		for(i = 0; i < 2; i++)
		{
			set_dvc_clamp(i, IO_OFF);
			set_dvc_rotator(i, IO_OFF); //자재를 받을 수 있는 정위치 
		}
		InitStep = 20;
		break;

	case 20:
		nRet_1 = get_dvc_clamp(VISION_1_SITE, IO_OFF, st_wait.n_on_wait_time[92], st_wait.n_limit_wait_time[92]);
		if(nRet_1 == CTL_GOOD || nRet_1 == CTL_ERROR)
		{
			//560100 1 56 "Vision#1의 Module Clamp 에러."
			nRet_1 = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_on_chk[VISION_1_SITE], IO_ON);
			nRet_2 = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_off_chk[VISION_1_SITE], IO_OFF);
			if(COMI.mn_simulation_mode == 1) // 20130509
			{
				InitStep = 30;
			}
			else
			{
				if(nRet_1 == IO_ON || nRet_2 == IO_ON)
				{
					sprintf(mc_alarmcode,"560200");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2500, st_work.mn_run_status, mc_alarmcode);
				}
				else
				{
					InitStep = 30;
				}
			}
		}
		break;

	case 30:
		nRet_1 = get_dvc_clamp(VISION_2_SITE, IO_OFF, st_wait.n_on_wait_time[94], st_wait.n_limit_wait_time[94]);
		if(nRet_1 == CTL_GOOD || nRet_1 == CTL_ERROR)
		{			
			nRet_1 = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_on_chk[VISION_2_SITE], IO_ON);
			nRet_2 = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_off_chk[VISION_2_SITE], IO_OFF);
			if(COMI.mn_simulation_mode == 1)
			{
				InitStep = 1000;
			}
			else
			{
				if(nRet_1 == IO_ON || nRet_2 == IO_ON)
				{
					//560102 1 56 "Vision#2의 Module Clamp 에러."
					sprintf(mc_alarmcode,"560102");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2501, st_work.mn_run_status, mc_alarmcode);
				}
				else
				{
					InitStep = 1000;
				}			
			}
		}
		break;
		
	case 1000:
		nRet_1 = COMI.HomeCheck_Mot(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 2000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2500, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2000:		
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_unit_fwd_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_label_attach_unit_bwd_chk, IO_ON);
		if(nRet_1 == IO_OFF || nRet_2 == IO_ON)
		{	//340101 0 34 "Label unit backward 체크 에러"
			InitStep = 2200;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
			InitStep = 2200;
		}
		break;

	case 2010:
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
		InitStep = 2200;
		break;

	case 2200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.i_label_attach_unit_bwd_chk, st_io.i_label_attach_unit_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[88], st_wait.n_limit_wait_time[88]);
		
		if(nRet_1 == RET_GOOD)
		{
			st_handler.i_vision1_init = NO;
			st_handler.i_vision2_init = NO;
			InitStep = 2300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//340101 0 34 "Label unit backward 체크 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Label unit backward 체크 에러.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"340101");
				CTL_Lib.Alarm_Error_Occurrence(2501, CTL_dWARNING, mc_alarmcode);
				InitStep = 2010;
			}
			else
			{
				InitStep = 2010;
			}
		}
		break;

	case 2300:
		FAS_IO.set_out_bit(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_ON);
		l_PickerWaitTime[0] = GetCurrentTime();
		InitStep = 2310;
		break;

	case 2310:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] > 500)	//20120725
		{
			InitStep = 2320;
		}
		break;

	case 2320:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
		if(nRet_1 == IO_ON)
		{
			//550704 1 55 "Label Picker에 Lable을 제거해 주세요."
			//sprintf(mc_alarmcode,"550704");
			//st_work.mn_run_status = CTL_dWARNING;
			//CTL_Lib.Alarm_Error_Occurrence(2591, CTL_dWARNING, mc_alarmcode);
			InitStep = 2330;
		}
		else
		{
			InitStep = 3000;
		}
		break;

	case 2330:
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_PICK_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			InitStep = 2340;
		}
		else if (nRet_1 == BD_RETRY)
		{
			InitStep= 2330;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(2590, CTL_dWARNING, COMI.mc_alarmcode);
			InitStep = 2330;
		}
		break;

	case 2340:
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_ON);
		l_PickerWaitTime[0] = GetCurrentTime();
		InitStep = 2350;
		break;

	case 2350:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] > 500)
		{
			InitStep = 2360;
		}
		break;

	case 2360:
		nRet_1 = COMI.HomeCheck_Mot(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 2370;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2360;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2500, st_work.mn_run_status, COMI.mc_alarmcode);
			InitStep = 2360;
		}
		break;

	case 2370:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
		
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
			InitStep = 3000;
		}
		else
		{
			if(nRet_1 == IO_ON)
			{
				//550704 1 55 "Label Picker에 Lable을 제거해 주세요."
				sprintf(mc_alarmcode,"550704");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2597, CTL_dWARNING, mc_alarmcode);
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
				InitStep = 3000;
			}
		}
		break;
		
	case 3000:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_vision1_safety_up_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_vision1_safety_dn_chk, IO_ON);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_vision2_safety_up_chk, IO_ON);
		nRet_4 = FAS_IO.get_in_bit(st_io.i_vision2_safety_dn_chk, IO_ON);

		if(st_handler.i_vision2_init == YES)
		{
			InitStep = 3010;
			break;
		}
		else if(st_handler.i_vision1_init == YES)
		{
			InitStep = 3500;
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
					InitStep = 3700;
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
					InitStep = 3200;
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
					InitStep = 3500;
					break;
				}
				if(cmmSxVMoveStart(M_VISION1_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
				}
			}
		}
		else if ((nRet_3 == IO_ON && nRet_4 == IO_ON) && (nRet_1 == IO_OFF && nRet_2 == IO_OFF))
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION2_Z, cmSMODE_KEEP, st_motor[M_VISION2_Z].md_spd_jog, st_motor[M_VISION2_Z].md_spd_acc, st_motor[M_VISION2_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION2_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION2_Z, FALSE, TRUE);
					InitStep = 3010;
					break;
				}
				if(cmmSxVMoveStart(M_VISION2_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION2_Z, FALSE, TRUE);
				}
			}
		}
		else if ((nRet_1 == IO_OFF && nRet_2 == IO_ON) && (nRet_3 == IO_OFF && nRet_4 == IO_ON))
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION1_Z, cmSMODE_KEEP, st_motor[M_VISION1_Z].md_spd_jog, st_motor[M_VISION1_Z].md_spd_acc, st_motor[M_VISION1_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION1_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
					InitStep = 3500;
					break;
				}
				if(cmmSxVMoveStart(M_VISION1_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
				}
			}
		}
		else if(COMI.mn_simulation_mode == 1) // 20130509
		{
			InitStep = 3500;
		}
		else//2015.0126
		{//560900 0 56 "Vision # 1 ,Vision # 2 position error. Please move to the standby position"
			sprintf(mc_alarmcode,"560900");
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2592, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 3010:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION1_Y, st_motor[M_VISION1_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 3100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 3010;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2502, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3100:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION1_Z, st_motor[M_VISION1_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 4100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 3100;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2503, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;


	case 3200:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION1_Z, st_motor[M_VISION1_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 3300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 3200;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2504, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3300:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION1_Y, st_motor[M_VISION1_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 4100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 3300;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2505, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3500:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION2_Y, st_motor[M_VISION2_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 3600;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 3500;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2506, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3600:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION2_Z, st_motor[M_VISION2_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 4000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 3600;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2507, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3700:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION2_Z, st_motor[M_VISION2_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 3800;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 3700;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2508, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3800:
		nRet_1 = COMI.HomeCheck_Mot(M_VISION2_Y, st_motor[M_VISION2_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 4000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 3800;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2509, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4000:
		st_handler.i_vision2_init = YES;
		InitStep = 4500;
		break;

	case 4100:
		st_handler.i_vision1_init = YES;
		InitStep = 4500;
		break;

	case 4500:
		if(st_handler.i_vision1_init == YES && st_handler.i_vision2_init == YES)
		{
			InitStep = 5000;
		}
		else
		{
			if(st_handler.i_vision1_init == YES) InitStep = 3500;
			else								 InitStep = 3000;
		}
		break;

//	case 3000:
//		InitStep = 5000; //2012.0701
//		break;
//
//		st_handler.mn_init_state[INIT_LAVEL_VISION] = CTL_YES;
//		InitStep = 0; 
//		break;

	//////////////////////////////////////////////////////////////
	// 2012.0701 james
	// 초기화는 V1, V2모두 정상적으로 끝났다는 것을 가정 한다 
	//////////////////////////////////////////////////////////////
	case 5000:
	///////////////////////////////////////////////////////////////////////////////////
	// SITE_V1를 SITE_LDRBT_POS까지 이동하여 로딩 로봇이 자재를 놓을 수 있도록 한다 
	///////////////////////////////////////////////////////////////////////////////////  
		nRet_1 = CTL_Lib.Single_Move(M_VISION1_Z, st_motor[M_VISION1_Z].md_pos[VISION_Z_DOWN_READY], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			InitStep = 5100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 5000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2510, st_work.mn_run_status, COMI.mc_alarmcode);
			InitStep = 5000;
		} 
		break;

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(M_VISION1_Y, st_motor[M_VISION1_Y].md_pos[VISION_Y_READY_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			InitStep = 5200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 5000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2511, st_work.mn_run_status, COMI.mc_alarmcode);
			InitStep = 5100;
		} 
		break;

	case 5200:
		 st_sync.n_visionrbt_working_site[VISION_1_SITE] = SITE_READY_POS;
		 InitStep = 6000;
		 break;

	case 6000:
	///////////////////////////////////////////////////////////////////////////////////
	// SITE_V2를 SITE_ULDRBT_POS까지  이동하여 로딩 로봇이 자재를 놓을 수 있도록 한다 
	/////////////////////////////////////////////////////////////////////////////////// 
		nRet_1 = CTL_Lib.Single_Move(M_VISION2_Y, st_motor[M_VISION2_Y].md_pos[SITE_ULDRBT_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			InitStep = 6100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 6000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2512, st_work.mn_run_status, COMI.mc_alarmcode);
			InitStep = 6100;
		} 
		break;

	case 6100:
		nRet_1 = CTL_Lib.Single_Move(M_VISION2_Z, st_motor[M_VISION2_Z].md_pos[VISION_Z_ULDMODULE], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			InitStep = 6200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 6000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2513, st_work.mn_run_status, COMI.mc_alarmcode);
			InitStep = 6100;
		} 
		break;

	case 6200:
		 st_sync.n_visionrbt_working_site[VISION_2_SITE] = SITE_ULDRBT_POS;
		 InitStep = 7000;
		 break;

	case 7000:
		st_handler.mn_init_state[INIT_LAVEL_VISION] = CTL_YES;
		InitStep = 0; 
		break;

	}
}

void CRun_Rbt_VisionLabel::OnVisionRbt_FinalPos()//20121126
{
	int i = 0;
	if(st_work.mn_reinstatement_mode[VISION_SITE] == 0) //stop시 최초 한번만 기록하기 위함
	{
		st_work.md_reinstatement_Pos[0][M_LABEL_ATTACH_Z] = COMI.md_cmdpos_backup[M_LABEL_ATTACH_Z];
		st_work.md_reinstatement_Pos[0][M_VISION1_Y] = COMI.md_cmdpos_backup[M_VISION1_Y];
		st_work.md_reinstatement_Pos[0][M_VISION1_Z] = COMI.md_cmdpos_backup[M_VISION1_Z];
		st_work.md_reinstatement_Pos[0][M_VISION2_Y] = COMI.md_cmdpos_backup[M_VISION2_Y];
		st_work.md_reinstatement_Pos[0][M_VISION2_Z] = COMI.md_cmdpos_backup[M_VISION2_Z];

		for (i = 0; i < 2; i++)
		{
			st_work.mn_VisClampStatusBackup[VISION_1_SITE + i][0] = FAS_IO.get_out_bit(st_io.o_vis_buff1_clamp_onoff + (i*2), IO_OFF);
			st_work.mn_VisRotateStatusBackup[VISION_1_SITE + i][0] = FAS_IO.get_out_bit(st_io.o_vis_buff1_rotator_onoff + (i*2), IO_OFF);
		}
		st_work.mn_VisVlpClampStatusBackup[VISION_SITE][0]  = FAS_IO.get_out_bit(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);
		st_work.mn_VisAttVacuumStatusBackup[VISION_SITE][0] = FAS_IO.get_out_bit(st_io.o_label_attach_vacuum_onoff, IO_OFF);
		st_work.mn_VisAttEjectStatusBackup[VISION_SITE][0] = FAS_IO.get_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
		st_work.mn_VisAttUnitFwdStatusBackup[VISION_SITE][0] = FAS_IO.get_out_bit(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
		st_work.mn_VisAttUnitBwdStatusBackup[VISION_SITE][0] = FAS_IO.get_out_bit(st_io.o_label_attach_unit_bwd_onoff, IO_OFF);
		
		st_work.mn_reinstatement_mode[VISION_SITE] = 1;	//최종 위치를 기록 했다
	}
}

void CRun_Rbt_VisionLabel::Robot_BackMovePosValCheck()//20121126
{
	//여기서 이전에 동작했던 위치를 다시 계산해야한다.
	st_work.md_reinstatement_Pos[1][M_LABEL_ATTACH_Z] =  st_work.md_reinstatement_Pos[0][M_LABEL_ATTACH_Z];
	st_work.md_reinstatement_Pos[1][M_VISION1_Y] =  st_work.md_reinstatement_Pos[0][M_VISION1_Y];
	st_work.md_reinstatement_Pos[1][M_VISION1_Z] =  st_work.md_reinstatement_Pos[0][M_VISION1_Z];
	st_work.md_reinstatement_Pos[1][M_VISION2_Y] =  st_work.md_reinstatement_Pos[0][M_VISION2_Y];
	st_work.md_reinstatement_Pos[1][M_VISION2_Z] =  st_work.md_reinstatement_Pos[0][M_VISION2_Z];
}

void CRun_Rbt_VisionLabel::Run_Reinstate()
{
	int Ret1 = 0, Ret2 = 0, i = 0, n_response = 0;
	int nRet1 = 0, nRet2 = 0;


	if (st_work.mn_VisionRobot_Reinstatement_Ok == TRUE) return;//20121126
	
	switch (ReinstateStep)
	{
	case 0:
		ReinstateStep = 1000;
		break;

	case 1000://현재 위치 확인
		dVisionCurPos[0] = COMI.Get_MotCurrentPos(M_LABEL_ATTACH_Z);
		dVisionCurPos[1] = COMI.Get_MotCurrentPos(M_VISION1_Y);
		dVisionCurPos[2] = COMI.Get_MotCurrentPos(M_VISION1_Z);
		dVisionCurPos[3] = COMI.Get_MotCurrentPos(M_VISION2_Y);
		dVisionCurPos[4] = COMI.Get_MotCurrentPos(M_VISION2_Z);

		for (i = 0; i < 5; i++)
		{
			mn_change[VISION_SITE][i] = 0;
		}
	
		//변경된 위치값 출력

		if (dVisionCurPos[0] > st_work.md_reinstatement_Pos[0][M_LABEL_ATTACH_Z] + COMI.mn_allow_value[M_LABEL_ATTACH_Z]
			|| dVisionCurPos[0] < st_work.md_reinstatement_Pos[0][M_LABEL_ATTACH_Z] - COMI.mn_allow_value[M_LABEL_ATTACH_Z])
		{
			mn_change[VISION_SITE][0]++;
			sprintf(st_msg.c_abnormal_msg, "Label Attach Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_LABEL_ATTACH_Z], (long)dVisionCurPos[0], (long)COMI.mn_allow_value[M_LABEL_ATTACH_Z]);
		}
		else if (dVisionCurPos[1] > st_work.md_reinstatement_Pos[0][M_VISION1_Y] + COMI.mn_allow_value[M_VISION1_Y]
			|| dVisionCurPos[1] < st_work.md_reinstatement_Pos[0][M_VISION1_Y] - COMI.mn_allow_value[M_VISION1_Y])
		{
			mn_change[VISION_SITE][1]++;
			sprintf(st_msg.c_abnormal_msg, "Vision#1 Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_VISION1_Y], (long)dVisionCurPos[1], (long)COMI.mn_allow_value[M_VISION1_Y]);
		}
		else if (dVisionCurPos[2] > st_work.md_reinstatement_Pos[0][M_VISION1_Z] + COMI.mn_allow_value[M_VISION1_Z]
			|| dVisionCurPos[2] < st_work.md_reinstatement_Pos[0][M_VISION1_Z] - COMI.mn_allow_value[M_VISION1_Z])
		{
			mn_change[VISION_SITE][2]++;
			sprintf(st_msg.c_abnormal_msg, "Vision#1 Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_VISION1_Z], (long)dVisionCurPos[2], (long)COMI.mn_allow_value[M_VISION1_Z]);
		}
		else if (dVisionCurPos[3] > st_work.md_reinstatement_Pos[0][M_VISION2_Y] + COMI.mn_allow_value[M_VISION2_Y]
			|| dVisionCurPos[3] < st_work.md_reinstatement_Pos[0][M_VISION2_Y] - COMI.mn_allow_value[M_VISION2_Y])
		{
			mn_change[VISION_SITE][3]++;
			sprintf(st_msg.c_abnormal_msg, "Vision#2 Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_VISION2_Y], (long)dVisionCurPos[3], (long)COMI.mn_allow_value[M_VISION2_Y]);
		}
		else if (dVisionCurPos[4] > st_work.md_reinstatement_Pos[0][M_VISION2_Z] + COMI.mn_allow_value[M_VISION2_Z]
			|| dVisionCurPos[4] < st_work.md_reinstatement_Pos[0][M_VISION2_Z] - COMI.mn_allow_value[M_VISION2_Z])
		{
			mn_change[VISION_SITE][4]++;
			sprintf(st_msg.c_abnormal_msg, "Vision#2 Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_VISION2_Z], (long)dVisionCurPos[4], (long)COMI.mn_allow_value[M_VISION2_Z]);
		}

		st_msg.mstr_event_msg[0] = "Recovery Mode.......";
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);

		if(mn_change[VISION_SITE][0] > 0 || mn_change[VISION_SITE][1] > 0 || mn_change[VISION_SITE][2] > 0 ||
		   mn_change[VISION_SITE][3] > 0 || mn_change[VISION_SITE][4] > 0) //rbt y
		{
			ReinstateStep = 2000;
		}
		else
		{
			ReinstateStep = 1100;			 
		}

		break;

	case 1100:
		backuptime[0] = GetCurrentTime();
		ReinstateStep = 1200;
		break;

	case 1200:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if (backuptime[2] >= 500)
		{
			ReinstateStep = 50000;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 2000:
		Robot_BackMovePosValCheck();
		
		COMI.Set_MotPower(M_LABEL_ATTACH_Z, TRUE);
		COMI.Set_MotPower(M_VISION1_Y, TRUE);
		COMI.Set_MotPower(M_VISION1_Z, TRUE);
		COMI.Set_MotPower(M_VISION2_Y, TRUE);
		COMI.Set_MotPower(M_VISION2_Z, TRUE);

	
		if(mn_change[VISION_SITE][0] > 0 || mn_change[VISION_SITE][1] > 0 || mn_change[VISION_SITE][2] > 0 ||
		   mn_change[VISION_SITE][3] > 0 || mn_change[VISION_SITE][4] > 0) //rbt y
		{
			ReinstateStep = 2100;
		}
		else
		{
			ReinstateStep = 1100;
		}
		break;

	case 2100:	
// 		if(st_work.mn_LdModuleRobot_Reinstatement_Ready == YES && st_work.mn_LdModuleStacker_Reinstatement_Ready     == YES && 
// 		   st_work.mn_LeftHsRobot_Reinstatement_Ready   == YES && st_work.mn_LeftHsStacker_Reinstatement_Ready       == YES && 
// 		   st_work.mn_RightHsRobot_Reinstatement_Ready  == YES && st_work.mn_RightHsRobotStacker_Reinstatement_Ready == YES && 
// 		   st_work.mn_ClipRobot_Reinstatement_Ready     == YES && st_work.mn_SorterRobot_Reinstatement_Ready         == YES && 
// 		   st_work.mn_UnloadRobot_Reinstatement_Ready   == YES && st_work.mn_UnloadStacker_Reinstatement_Ready       == YES && 
// 		   st_work.mn_VisionRobot_Reinstatement_Ready   == YES && st_work.mn_WorkBufferRobot_Reinstatement_Ready     == YES)
		if(st_work.mn_SorterRobot_Reinstatement_Ready == YES && st_work.mn_UnloadRobot_Reinstatement_Ready   == YES)
		{
			ReinstateStep = 3000;
		}
		break;


	case 3000:
		nRet1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_SAFETY], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3010;
		}
		else if (nRet1 != CTLBD_RET_PROCEED)
		{
			// 000100 1 A "Front Shifter Z Safety Pos Move Error."
			CTL_Lib.Alarm_Error_Occurrence(2520, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3010:
		for(i = 0; i < 2; i++)
		{
			set_dvc_clamp(i, IO_OFF);
			set_dvc_rotator(i, IO_OFF); //자재를 받을 수 있는 정위치 
		}
		ReinstateStep = 3020;
		break;
		
	case 3020:
		Ret1 = get_dvc_clamp(VISION_1_SITE, IO_OFF, st_wait.n_on_wait_time[92], st_wait.n_limit_wait_time[92]);
		if(Ret1 == CTL_GOOD || Ret1 == CTL_ERROR)
		{	
			nRet1 = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_on_chk[VISION_1_SITE], IO_ON);
			nRet2 = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_off_chk[VISION_1_SITE], IO_OFF);
			if(nRet1 == IO_ON || nRet2 == IO_ON)
			{
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_vis_buffer_clamp_onoff[VISION_1_SITE], IO_ON);

			}
			ReinstateStep = 3030;	
		}
		break;
		
	case 3030:
		Ret1 = get_dvc_clamp(VISION_2_SITE, IO_OFF, st_wait.n_on_wait_time[94], st_wait.n_limit_wait_time[94]);
		if(Ret1 == CTL_GOOD || Ret1 == CTL_ERROR)
		{	
			nRet1 = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_on_chk[VISION_2_SITE], IO_ON);
			nRet2 = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_off_chk[VISION_2_SITE], IO_OFF);
			if(nRet1 == IO_ON || nRet2 == IO_ON)
			{
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_vis_buffer_clamp_onoff[VISION_2_SITE], IO_ON);

			}
			ReinstateStep = 3040;	
		}
		break;

	case 3040:
		FAS_IO.OnCylinderAction(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);
		ReinstateStep = 3050;
		break;
		
	case 3050:
		nRet1 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_off_chk, st_io.i_vlp_bcr_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
		nRet1 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_off_chk,
			IO_ON, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
		if(nRet1 == RET_GOOD)
		{
			ReinstateStep = 3070;
		}
		else if(nRet1 != RET_PROCEED)
		{
			mn_visionretry++;
			if(mn_visionretry > 3)
			{
				mn_visionretry = 0;
				sprintf(mc_alarmcode,"550201");
				CTL_Lib.Alarm_Error_Occurrence(2529, CTL_dWARNING, mc_alarmcode);
				ReinstateStep = 3040;
			}
			else
			{
				ReinstateStep = 3040;
			}
		}
		break;

	case 3060:
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
		ReinstateStep = 3070;
		break;
		
	case 3070:
		nRet1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.i_label_attach_unit_bwd_chk, st_io.i_label_attach_unit_fwd_chk,
		IO_ON, st_wait.n_on_wait_time[88], st_wait.n_limit_wait_time[88]);
		if(nRet1 == RET_GOOD)
		{
			ReinstateStep = 3100;
		}
		else if(nRet1 != RET_PROCEED)
		{
			mn_visionretry++;
			if(mn_visionretry > 3)
			{
				mn_visionretry = 0;
				sprintf(mc_alarmcode,"340101");
				CTL_Lib.Alarm_Error_Occurrence(2501, CTL_dWARNING, mc_alarmcode);
				ReinstateStep = 3060;
			}
			else
			{
				ReinstateStep = 3060;
			}
		}
		break;

	case 3100:
		Ret1 = FAS_IO.get_in_bit(st_io.i_vision1_safety_up_chk, IO_ON);
		Ret2 = FAS_IO.get_in_bit(st_io.i_vision1_safety_dn_chk, IO_ON);
		nRet1 = FAS_IO.get_in_bit(st_io.i_vision2_safety_up_chk, IO_ON);
		nRet1 = FAS_IO.get_in_bit(st_io.i_vision2_safety_dn_chk, IO_ON);

		if(st_handler.i_vision2_init == YES)
		{
			ReinstateStep = 3110;
			break;
		}
		else if(st_handler.i_vision1_init == YES)
		{
			ReinstateStep = 3130;
			break;
		}

		if(Ret1 == IO_ON && Ret2 == IO_OFF)
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION1_Z, cmSMODE_KEEP, st_motor[M_VISION1_Z].md_spd_jog, st_motor[M_VISION1_Z].md_spd_acc, st_motor[M_VISION1_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION1_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
					ReinstateStep = 3150;
					break;
				}
				if(cmmSxVMoveStart(M_VISION1_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
				}
			}
		}
		else if(nRet1 == IO_ON && nRet2 == IO_OFF)
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION2_Z, cmSMODE_KEEP, st_motor[M_VISION2_Z].md_spd_jog, st_motor[M_VISION2_Z].md_spd_acc, st_motor[M_VISION2_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION2_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION2_Z, FALSE, TRUE);
					ReinstateStep = 3170;
					break;
				}
				if(cmmSxVMoveStart(M_VISION2_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION2_Z, FALSE, TRUE);
				}
			}
		}
		else if ((Ret1 == IO_ON && Ret2 == IO_ON) && (nRet1 == IO_OFF && nRet2 == IO_OFF))
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION1_Z, cmSMODE_KEEP, st_motor[M_VISION1_Z].md_spd_jog, st_motor[M_VISION1_Z].md_spd_acc, st_motor[M_VISION1_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION1_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
					ReinstateStep = 3130;
					break;
				}
				if(cmmSxVMoveStart(M_VISION1_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
				}
			}
		}
		else if ((nRet1 == IO_ON && nRet2 == IO_ON) && (Ret1 == IO_OFF && Ret2 == IO_OFF))
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION2_Z, cmSMODE_KEEP, st_motor[M_VISION2_Z].md_spd_jog, st_motor[M_VISION2_Z].md_spd_acc, st_motor[M_VISION2_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION2_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION2_Z, FALSE, TRUE);
					ReinstateStep = 3110;
					break;
				}
				if(cmmSxVMoveStart(M_VISION2_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION2_Z, FALSE, TRUE);
				}
			}
		}
		else if ((Ret1 == IO_OFF && Ret2 == IO_ON) && (nRet1 == IO_OFF && nRet2 == IO_ON))
		{
			COMI.Set_MotSpeed(MOT_SPD_JOG, M_VISION1_Z, cmSMODE_KEEP, st_motor[M_VISION1_Z].md_spd_jog, st_motor[M_VISION1_Z].md_spd_acc, st_motor[M_VISION1_Z].md_spd_dec);		// 축의 속도를 변경한다.
			while (1)
			{
				if(COMI.Get_MotIOSensor(M_VISION1_Z, MOT_SENS_ELP) == CTLBD_RET_GOOD)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
					ReinstateStep = 3130;
					break;
				}
				if(cmmSxVMoveStart(M_VISION1_Z, PLUS) != cmERR_NONE)
				{
					cmmSxStop(M_VISION1_Z, FALSE, TRUE);
				}
			}
		}
		break;

	case 3110:
		nRet1 = COMI.HomeCheck_Mot(M_VISION1_Y, st_motor[M_VISION1_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ReinstateStep = 3120;
		}
		else if (nRet1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3110;
		}
		else if ((nRet1 != CTLBD_RET_GOOD && nRet1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(2502, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3120:
		nRet1 = COMI.HomeCheck_Mot(M_VISION1_Z, st_motor[M_VISION1_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ReinstateStep = 3300;
		}
		else if (nRet1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3120;
		}
		else if ((nRet1 != CTLBD_RET_GOOD && nRet1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(2503, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3130:
		nRet1 = COMI.HomeCheck_Mot(M_VISION2_Y, st_motor[M_VISION2_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ReinstateStep = 3140;
		}
		else if (nRet1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3130;
		}
		else if ((nRet1 != CTLBD_RET_GOOD && nRet1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(2506, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3140:
		nRet1 = COMI.HomeCheck_Mot(M_VISION2_Z, st_motor[M_VISION2_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ReinstateStep = 3200;
		}
		else if (nRet1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3140;
		}
		else if ((nRet1 != CTLBD_RET_GOOD && nRet1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(2507, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3150:
		nRet1 = COMI.HomeCheck_Mot(M_VISION2_Z, st_motor[M_VISION2_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ReinstateStep = 3160;
		}
		else if (nRet1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3150;
		}
		else if ((nRet1 != CTLBD_RET_GOOD && nRet1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(2508, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3160:
		nRet1 = COMI.HomeCheck_Mot(M_VISION2_Y, st_motor[M_VISION2_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ReinstateStep = 3200;
		}
		else if (nRet1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3160;
		}
		else if ((nRet1 != CTLBD_RET_GOOD && nRet1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(2509, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3170:
		nRet1 = COMI.HomeCheck_Mot(M_VISION1_Z, st_motor[M_VISION1_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ReinstateStep = 3180;
		}
		else if (nRet1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3170;
		}
		else if ((nRet1 != CTLBD_RET_GOOD && nRet1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(2504, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3180:
		nRet1 = COMI.HomeCheck_Mot(M_VISION1_Y, st_motor[M_VISION1_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ReinstateStep = 3300;
		}
		else if (nRet1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3180;
		}
		else if ((nRet1 != CTLBD_RET_GOOD && nRet1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(2505, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3200://4000:
		st_handler.i_vision2_init = YES;
		ReinstateStep = 3400;
		break;

	case 3300://4100:
		st_handler.i_vision1_init = YES;
		ReinstateStep = 3400;
		break;

	case 3400:
		st_work.mn_VisionRobot_Reinstatement_Ready = YES;
		ReinstateStep = 3500;
		break;

	case 3500:
	// 	st_work.md_reinstatement_Pos[1][M_VISION1_Z] =  st_work.md_reinstatement_Pos[0][M_VISION1_Z];
	// 	dVisionCurPos[2] = COMI.Get_MotCurrentPos(M_VISION1_Z);
		dVisionCurPos[2] = COMI.Get_MotCurrentPos(M_VISION1_Z);
		if(dVisionCurPos[2] <= st_work.md_reinstatement_Pos[1][M_VISION1_Z] - COMI.mn_allow_value[M_VISION1_Z])
		{
			nRet1 = CTL_Lib.Single_Move(M_VISION1_Z, st_work.md_reinstatement_Pos[1][M_VISION1_Z], st_basic.nManualSpeed);
			
			if (nRet1 == CTLBD_RET_GOOD)
			{
				ReinstateStep = 3510;
			}
			else if (nRet1 == CTLBD_RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(2510, CTL_dWARNING, COMI.mc_alarmcode);
			}
		}
		else
		{
			ReinstateStep = 3600;			
		}
		break;

	case 3510:
		nRet1 = COMI.Check_SingleMove(M_VISION1_Z, st_work.md_reinstatement_Pos[1][M_VISION1_Z]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3600;
		}		
		else if(Ret1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3510;
		}
		else if (nRet1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(2510, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3600:
		// 	st_work.md_reinstatement_Pos[1][M_VISION2_Z] =  st_work.md_reinstatement_Pos[0][M_VISION2_Z];
		// 	dVisionCurPos[4] = COMI.Get_MotCurrentPos(M_VISION2_Z);
		dVisionCurPos[4] = COMI.Get_MotCurrentPos(M_VISION2_Z);
		if(dVisionCurPos[4] <= st_work.md_reinstatement_Pos[1][M_VISION2_Z] - COMI.mn_allow_value[M_VISION2_Z])
		{
			nRet1 = CTL_Lib.Single_Move(M_VISION2_Z, st_work.md_reinstatement_Pos[1][M_VISION2_Z], st_basic.nManualSpeed);
			
			if (nRet1 == CTLBD_RET_GOOD)
			{
				ReinstateStep = 3610;
			}
			else if (nRet1 == CTLBD_RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(2513, CTL_dWARNING, COMI.mc_alarmcode);
			}
		}
		else
		{
			ReinstateStep = 3700;			
		}
		break;

	case 3610:
		nRet1 = COMI.Check_SingleMove(M_VISION2_Z, st_work.md_reinstatement_Pos[1][M_VISION2_Z]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3700;
		}		
		else if(Ret1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3610;
		}
		else if (nRet1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(2513, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3700:
		// 	st_work.md_reinstatement_Pos[1][M_VISION1_Y] =  st_work.md_reinstatement_Pos[0][M_VISION1_Y];
		// 	dVisionCurPos[1] = COMI.Get_MotCurrentPos(M_VISION1_Y);
		dVisionCurPos[1] = COMI.Get_MotCurrentPos(M_VISION1_Y);
		if(dVisionCurPos[1] <= st_work.md_reinstatement_Pos[1][M_VISION1_Y] - COMI.mn_allow_value[M_VISION1_Y])
		{
			nRet1 = CTL_Lib.Single_Move(M_VISION1_Y, st_work.md_reinstatement_Pos[1][M_VISION1_Y], st_basic.nManualSpeed);
			
			if (nRet1 == CTLBD_RET_GOOD)
			{
				ReinstateStep = 3710;
			}
			else if (nRet1 == CTLBD_RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(2511, CTL_dWARNING, COMI.mc_alarmcode);
			}
		}
		else
		{
			ReinstateStep = 3800;			
		}
		break;

	case 3710:
		nRet1 = COMI.Check_SingleMove(M_VISION1_Y, st_work.md_reinstatement_Pos[1][M_VISION1_Y]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3800;
		}		
		else if(Ret1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3710;
		}
		else if (nRet1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(2511, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3800:
		// 	st_work.md_reinstatement_Pos[1][M_VISION2_Y] =  st_work.md_reinstatement_Pos[0][M_VISION2_Y];
		// 	dVisionCurPos[3] = COMI.Get_MotCurrentPos(M_VISION2_Y);
		dVisionCurPos[3] = COMI.Get_MotCurrentPos(M_VISION2_Y);
		if(dVisionCurPos[3] <= st_work.md_reinstatement_Pos[1][M_VISION2_Y] - COMI.mn_allow_value[M_VISION2_Y])
		{
			nRet1 = CTL_Lib.Single_Move(M_VISION2_Y, st_work.md_reinstatement_Pos[1][M_VISION2_Y], st_basic.nManualSpeed);
			
			if (nRet1 == CTLBD_RET_GOOD)
			{
				ReinstateStep = 3810;
			}
			else if (nRet1 == CTLBD_RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(2512, CTL_dWARNING, COMI.mc_alarmcode);
			}
		}
		else
		{
			ReinstateStep = 3900;			
		}
		break;

	case 3810:
		nRet1 = COMI.Check_SingleMove(M_VISION2_Y, st_work.md_reinstatement_Pos[1][M_VISION2_Y]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3900;
		}		
		else if(Ret1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 3810;
		}
		else if (nRet1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(2512, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3900:
		// st_work.mn_VisAttUnitFwdStatusBackup[VISION_SITE][0] = FAS_IO.get_out_bit(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
		// st_work.mn_VisAttUnitBwdStatusBackup[VISION_SITE][0] = FAS_IO.get_out_bit(st_io.o_label_attach_unit_bwd_onoff, IO_OFF);
		// st_work.mn_VisVlpClampStatusBackup[VISION_SITE][0]  = FAS_IO.get_out_bit(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);
		ReinstateStep = 3910;
		break;

	case 3910:
		nRet1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.i_label_attach_unit_fwd_chk, st_io.i_label_attach_unit_bwd_chk,
			IO_ON, st_wait.n_on_wait_time[88], st_wait.n_limit_wait_time[88]);
		if(st_work.n_bcrNojob == 2 || st_work.n_bcrNojob == 3)//G1
		{
			nRet2 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_on_chk, st_io.i_vlp_bcr_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
			nRet2 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_off_chk,
				IO_OFF, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
			nRet2 = IO_ON;
		}
		else
		{
			nRet2 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_off_chk, st_io.i_vlp_bcr_clamp_on_chk,
				IO_ON, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
			nRet2 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_on_chk,
				IO_OFF, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
		}
		
		if(nRet1 == RET_GOOD && nRet2 == RET_GOOD)
		{
			ReinstateStep = 4000;
		}
		else if(nRet1 != RET_PROCEED && nRet2 != RET_PROCEED)
		{//550200 0 55 "Label unit backward"
		 //550201 0 55 "VLP Label unit backward"
			mn_visionretry++;
			if(mn_visionretry > 3)
			{
				mn_visionretry = 0;
				if(nRet1 != RET_PROCEED)
				{
					sprintf(mc_alarmcode,"550200");
				}
				else
				{
					sprintf(mc_alarmcode,"550201");
				}
				CTL_Lib.Alarm_Error_Occurrence(2525, CTL_dWARNING, mc_alarmcode);
				ReinstateStep = 3900;
			}
			else
			{
				ReinstateStep = 3900;
			}
		}
		break;
		
	case 4000:
		// for (i = 0; i < 2; i++)
		// {
		// 	st_work.mn_VisClampStatusBackup[VISION_1_SITE + i][0] = FAS_IO.get_out_bit(st_io.o_vis_buff1_clamp_onoff + (i*2), IO_OFF);
		// 	st_work.mn_VisRotateStatusBackup[VISION_1_SITE + i][0] = FAS_IO.get_out_bit(st_io.o_vis_buff1_rotator_onoff + (i*2), IO_OFF);
		// }
		for (i = 0; i < 2; i++)
		{
			if(st_work.mn_VisClampStatusBackup[VISION_1_SITE + i][0] == IO_ON)
			{
				FAS_IO.OnCylinderAction(st_io.o_vis_buff1_clamp_onoff + (i*2), IO_ON);
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_vis_buff1_clamp_onoff + (i*2), IO_OFF);
			}

			if(st_work.mn_VisRotateStatusBackup[VISION_1_SITE + i][0] == IO_ON)
			{
				FAS_IO.OnCylinderAction(st_io.o_vis_buff1_rotator_onoff + (i*2), IO_ON);
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_vis_buff1_rotator_onoff + (i*2), IO_OFF);
			}
		}
		ReinstateStep = 4100;
		break;


	case 4100:
		// 	st_work.md_reinstatement_Pos[1][M_LABEL_ATTACH_Z] =  st_work.md_reinstatement_Pos[0][M_LABEL_ATTACH_Z];
		// 	dVisionCurPos[0] = COMI.Get_MotCurrentPos(M_LABEL_ATTACH_Z);
		dVisionCurPos[0] = COMI.Get_MotCurrentPos(M_LABEL_ATTACH_Z);
		if(dVisionCurPos[0] <= st_work.md_reinstatement_Pos[1][M_LABEL_ATTACH_Z] - COMI.mn_allow_value[M_LABEL_ATTACH_Z])
		{
			nRet1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_work.md_reinstatement_Pos[1][M_LABEL_ATTACH_Z], st_basic.nManualSpeed);
			
			if (nRet1 == CTLBD_RET_GOOD)
			{
				ReinstateStep = 4110;
			}
			else if (nRet1 == CTLBD_RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(2526, CTL_dWARNING, COMI.mc_alarmcode);
			}
		}
		else
		{
			ReinstateStep = 4200;			
		}
		break;

	case 4110:
		nRet1 = COMI.Check_SingleMove(M_LABEL_ATTACH_Z, st_work.md_reinstatement_Pos[1][M_LABEL_ATTACH_Z]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4200;
		}		
		else if(Ret1 == CTLBD_RET_RETRY)
		{
			ReinstateStep = 4110;
		}
		else if (nRet1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(2526, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 4200:
		// st_work.mn_VisAttVacuumStatusBackup[VISION_SITE][0] = FAS_IO.get_out_bit(st_io.o_label_attach_vacuum_onoff, IO_OFF);
		// st_work.mn_VisAttEjectStatusBackup[VISION_SITE][0] = FAS_IO.get_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
		if(st_work.mn_VisAttVacuumStatusBackup[VISION_SITE][0] == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_label_attach_vacuum_onoff, IO_ON);
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_label_attach_vacuum_onoff, IO_OFF);
		}

		if(st_work.mn_VisAttEjectStatusBackup[VISION_SITE][0] == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_label_attach_eject_onoff, IO_ON);
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_label_attach_eject_onoff, IO_OFF);
		}
		ReinstateStep = 50000;
		break;


	case 50000:
		st_work.mn_VisionRobot_Reinstatement_Ready = YES;
		st_work.mn_VisionRobot_Reinstatement_Ok = YES;//20121126
		ReinstateStep = 0;
		break;

	}
}

void CRun_Rbt_VisionLabel::Thread_Variable_Initial()
{
	int i = 0;
	mn_retry = 0;
	Run_labelStep = 0;
	Run_Vision1 = 0;
	Run_Vision2 = 0;
	m_nLabelNum = 0;
	m_nVisChkNum = -1;
	m_nPick_Label = -1;
	mn_attach_err = NO;
	mn_retry = 0;
	md_offset=  0.0;
//	m_nOrder = -1;
//	m_nOutPrint = 0;
	st_work.mn_Barcode_OutPrint = -1;
	
	for (i = 0; i < 100; i++)
	{
		mn_vis_result[i] = 0;
	}

	m_nZPos[TOPBUFFER] = m_nZPos[BTMBUFFER] = NO;
	m_buffPos[TOPBUFFER] = BUFFPOS_NONE;
	m_buffPos[BTMBUFFER] = BUFFPOS_NONE;  // 초기값
	m_buffStatus[TOPBUFFER] = sDONE;
	m_buffStatus[BTMBUFFER] = sDONE;  // 초기값

    st_sync.n_ldrbt_visionrbt_req[1] = VISION_SITE_CLEAR;

	st_sync.n_visionrbt_workend_flag[VISION_1_SITE] = CTL_NO;//20140215
	st_sync.n_visionrbt_workend_flag[VISION_2_SITE] = CTL_NO;//20140215

	//2015.0126
	st_sync.n_ldrbt_visionrbt_req[0] = -1;
	st_sync.n_ldrbt_visionrbt_req[1] = -1;

	////2014,0303
	st_sync.n_visionrbt_load_flag[VISION_1_SITE] = CTL_NO;
	st_sync.n_visionrbt_load_flag[VISION_2_SITE] = CTL_NO;

	st_sync.n_visionrbt_unload_flag[VISION_1_SITE] = CTL_NO;
	st_sync.n_visionrbt_unload_flag[VISION_2_SITE] = CTL_NO;
	////
}

void CRun_Rbt_VisionLabel::Run_Label()
{
	int i,nRet_1,nRet_2;
	double dCurr;

	////2014,0301
	if(st_barcode.n_barcode_hs_paper_out_error == 1)//라벨용지 교체 
	{
		Run_labelStep = 0;
	}
	////
	////2014,0411
	if(st_handler.n_vision_reset == YES)
	{
		st_handler.n_vision_reset = NO;
		for (i = 0; i < m_inspect; i++)
		{
			m_bOutPrint[i] = 0;
		}
		//m_nOutPrint = m_inspect = 0;
		st_work.mn_Barcode_OutPrint = m_inspect = 0;//20140214

		Run_labelStep = 0;
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_normal_msg,"Vision Reset");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
	////


	Func.ThreadFunctionStepTrace(30, Run_labelStep);

	switch(Run_labelStep)
	{
	case -1:
		Thread_Variable_Initial();
		Run_labelStep = 0;
		mn_VisionStep[TOPBUFFER] = 0;
		mn_VisionStep[BTMBUFFER] = 0;
		break;

	case 0:
// 		FAS_IO.set_out_bit(st_io.o_ionizer_onoff, IO_ON);//20121205
		mn_singleprint = 0;
		mn_emptyprint = 0;
		mn_labelprint = 0;
		Run_labelStep = 10;
		break;

	case 10:
		dCurr = COMI.Get_MotCurrentPos(M_LABEL_ATTACH_Z);
		if(dCurr > st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_SAFETY] + st_motor[M_LABEL_ATTACH_Z].mn_allow)
		{
			if(COMI.mn_simulation_mode == 1) // 20130509
			{
				Run_labelStep = 50;
				break;
			}

			Run_labelStep = 20;
		}
		else
		{
			Run_labelStep = 50;
		}
		break;

	case 20:
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			Run_labelStep = 10;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			Run_labelStep = 20;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2520, st_work.mn_run_status, COMI.mc_alarmcode);
			Run_labelStep = 20;
		}
		break;

	case 50:
//		if(FAS_IO.get_in_bit(st_io.i_label_attach_unit_fwd_chk, IO_OFF) == IO_OFF &&
//			FAS_IO.get_in_bit(st_io.i_label_attach_unit_bwd_chk, IO_ON) == IO_ON)
//		{
//			Run_labelStep = 70;
//		}
//		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
//			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
//			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
			Run_labelStep = 60;
		}
		break;

	case 55:
		FAS_IO.set_out_bit(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);

//		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
//		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
		Run_labelStep = 60;
		break;

	case 60:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.o_label_attach_unit_bwd_onoff, st_io.i_label_attach_unit_fwd_chk,
					IO_ON, st_wait.n_on_wait_time[88], st_wait.n_limit_wait_time[88]);
		if(nRet_1 == RET_GOOD)
		{
			Run_labelStep = 91;//100;//80;	//20120707
		}
		else if(nRet_1 != RET_PROCEED)
		{//550702 1 55 "Label Attach Forward 에러."
		 //550703 1 55 "Label Attach Backward 에러."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"550703");
				CTL_Lib.Alarm_Error_Occurrence(2521, CTL_dWARNING, mc_alarmcode);
			}
			else
			{
				Run_labelStep = 55;
			}
		}
		break;
		
	case 70:
		if (st_picker.n_vision_eable[0] == TRUE)
		{
			str_bcr = "^XA~PS^XZ";
			
			if(Func.OutLptPort(str_bcr) == CTLBD_RET_GOOD) //Printer  reset
			{
				Run_labelStep = 80;
			}
			else
			{
				//560708 1 56 '바코드 프린터기를 확인해 주세요"
				sprintf(mc_alarmcode,"560708");
				CTL_Lib.Alarm_Error_Occurrence(2522, CTL_dWARNING, mc_alarmcode);
			}
		}
		else
		{
			Run_labelStep = 80;			
		}

		break;

	case 80:
		FAS_IO.set_out_bit(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_ON);
		l_PickerWaitTime[0] = GetCurrentTime();
		Run_labelStep = 85;
		break;

	case 85:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] > 500)
		{
			mn_retry = 0;
			Run_labelStep = 86;
		}
		else if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		break;

	case 86:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
		if(nRet_1 == IO_ON)
		{
			//550704 1 55 "Label Picker에 Lable을 제거해 주세요."
			// 20140224
			//2017.0731
			//sprintf(mc_alarmcode,"550704");
			//st_work.mn_run_status = CTL_dWARNING;
			//CTL_Lib.Alarm_Error_Occurrence(3523, CTL_dWARNING, mc_alarmcode);
			Run_labelStep = 87;
		}
		else
		{
			Run_labelStep = 90;
		}
		break;

	case 87:
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_PICK_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Run_labelStep = 88;
		}
		else if (nRet_1 == BD_RETRY)
		{
			Run_labelStep= 87;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(2524, CTL_dWARNING, COMI.mc_alarmcode);
			Run_labelStep = 87;
		}
		break;

	case 88:
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_ON);
		l_PickerWaitTime[0] = GetCurrentTime();
		Run_labelStep = 89;
		break;

	case 89:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] > 500)
		{
			// 20140224 // 제거 동작을 한 후 Vacuum 체크 했을 때도 Label이 걸리게 되면.
			nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
			
			if(nRet_1 == IO_ON)
			{
				//550704 1 55 "Label Picker에 Lable을 제거해 주세요."
				// 20140224
				sprintf(mc_alarmcode,"550704");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(6523, CTL_dWARNING, mc_alarmcode);
				Run_labelStep = 87;
			}
			else
			{
				Run_labelStep = 90;
			}
		}
		break;

	case 90:
		nRet_1 = COMI.HomeCheck_Mot(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			Run_labelStep = 91;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			Run_labelStep = 90;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2525, st_work.mn_run_status, COMI.mc_alarmcode);
			Run_labelStep = 90;
		}
		break;

	case 91:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
		if(nRet_1 == IO_ON && COMI.mn_simulation_mode != 1) // 20130509
		{
			mn_retry++;
			if(mn_retry > 2)
			{
				mn_retry = 0;
				//550704 1 55 "Label Picker에 Lable을 제거해 주세요."
				sprintf(mc_alarmcode,"550704");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2526, CTL_dWARNING, mc_alarmcode);
			}
			else
			{
				Run_labelStep = 86;
			}
		}
		else
		{
			FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
			Run_labelStep = 92;
		}
		break;

	case 92:

		if(st_sync.n_lotend_module_vision[VISION_1_SITE] == CTL_YES &&
			st_sync.n_lotend_module_vision[VISION_2_SITE] == CTL_YES)
		{
			break;
		}


		if (st_picker.n_vision_eable[0] == TRUE)
		{
			if(st_handler.mn_jobComplete == YES)
			{
				m_nPick = -1;
				m_inspect = -1;
				Run_labelStep = 93; // 20140224
			}
			else
			{
				if( st_work.mn_lot_start == CTL_YES )
				{
					//992002 0 99 "BCR Job선택을 하지 못했습니다."
					sprintf(mc_alarmcode,"992002");
					CTL_Lib.Alarm_Error_Occurrence(2527, CTL_dWARNING, mc_alarmcode);
				}
			}	
		}
		else
		{
			Run_labelStep = 99;			
		}
		break;

	// 20140224
	case 93:
//		Func.Multi_Paper_Print(st_basic.mn_label_bin_print); // 빈 Label 여러장 출력.
		Func.Single_Paper_Print(); // 빈 Label 여러장 출력.
		l_PickerWaitTime[0] = GetCurrentTime();
		Run_labelStep = 94;
		break;

	case 94:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] > 500)
		{
			mn_singleprint++;
			if( mn_singleprint < st_basic.mn_label_bin_print )
				Run_labelStep = 93;
			else
				Run_labelStep = 95;
		}
		else if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		break;
		
	case 95:
		if (st_picker.n_label_enable == TRUE)//20120912
		{
			if (st_picker.n_vision_eable[0] == TRUE)//20120912
			{
				if( st_work.mn_Barcode_OutPrint == -1 ||
					(st_work.mn_Barcode_OutPrint != 0 && st_work.mn_Barcode_OutPrint % 10 == 0) )
				{	
					//mn_printnum = m_nOutPrint % 10;	
					mn_printnum = st_work.mn_Barcode_OutPrint % 10;//20140214
					nRet_1 = Func.Barcode_Create(mn_printnum, 1);	//20120725
					if(nRet_1 == CTLBD_RET_GOOD)
					{
						l_PickerWaitTime[0] = GetCurrentTime();
						Run_labelStep = 96;	
					}
					else
					{
						st_work.mn_Barcode_OutPrint++;
						Run_labelStep = 97;
					}
				}
				else
				{
					//mn_printnum = m_nOutPrint % 10;	
					mn_printnum = st_work.mn_Barcode_OutPrint % 10;//20140214
					nRet_1 = Func.Barcode_Create(mn_printnum, 1);	//20120725
					if(nRet_1 == CTLBD_RET_GOOD)
					{
						l_PickerWaitTime[0] = GetCurrentTime();
						Run_labelStep = 96;	
					}
					else
					{
						st_work.mn_Barcode_OutPrint++;
						Run_labelStep = 97;
					}
				}
				
			}
			else
			{
				Run_labelStep = 99;
			}
		}
/*		else  //20140401
		{
			Func.Single_Paper_Print();
			Run_labelStep = 99;	
		}		
*/
		break;

	case 96:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] > 500)
		{
			if (st_barcode.n_Prt_Comp_Reply == 0) //20120727 
			{
				if(st_barcode.b_print_complete == TRUE)
				{
					st_barcode.b_print_complete = FALSE;
					mn_labelprint++;
				}
				else
				{
					mn_labelprint++;
				}
			}
			st_work.mn_Barcode_OutPrint++;//20140214
			if(st_work.mn_Barcode_OutPrint % 10 == 0)
			{
				Run_labelStep = 97;
			}
			else
			{
				Run_labelStep = 98;
			}
		}
		else if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}

		break;


	case 97:
		nRet_1 = OnBcrRev_Excution();
		if(nRet_1 == RET_GOOD)
		{
			Run_labelStep = 98;
		}
		else if(nRet_1 == RET_ERROR)
		{
//			Run_labelStep = 97;
		}
		break;
		
	case 98:
		//if(m_nOutPrint > 13) 

		// 20140224
		if(st_work.mn_Barcode_OutPrint > 30 && m_inspect < 0)  
		{
			if (st_barcode.n_Prt_Comp_Reply == 0) //20120727 
			{
				if( mn_labelprint > 25)
				{
					st_work.mn_Barcode_OutPrint = 14;
					Run_labelStep = 99;
				}
				else
				{
					sprintf(mc_alarmcode, "560602");
					CTL_Lib.Alarm_Error_Occurrence(5109, CTL_dWARNING, mc_alarmcode);
					Run_labelStep = 0;
					st_work.mn_Barcode_OutPrint = -1;
				}
			}
			else
			{
				st_work.mn_Barcode_OutPrint = 14;
				Run_labelStep = 99;
			}
		}
		else if(m_inspect >= 0)
		{
			Run_labelStep = 99;
		}
		else
		{
			Run_labelStep = 95;
		}
		break;
		
	case 99: // 트리거 신호 무한 반복 .
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			Run_labelStep = 100;
			break;
		}

		if (st_picker.n_vision_eable[0] == TRUE)//20120912
		{
			nRet_1 = Func.BCR_Trigger();//0907
			if(nRet_1 == RET_GOOD)
			//if(nRet_1 == RET_GOOD || nRet_1 == RET_ERROR)
			{
				//비전 명령 
				m_inspect++;
				if(m_inspect < 0) m_inspect = 0;
				m_bOutPrint[m_inspect] = BIN_GOOD;
				if(m_inspect >= 6)
				{
					Run_labelStep = 100;
				}
				else
				{
					Run_labelStep = 95;
				}
			}
			else if(nRet_1 == RET_DOOROPEN)
			{
				//비전 명령 
				m_inspect++;
				if(m_inspect < 0) m_inspect = 0;
				m_bOutPrint[m_inspect] = BIN_REJECT;
				if(m_inspect >= 6)
				{
					Run_labelStep = 100;
				}
				else
				{
					Run_labelStep = 95;
				}
			}
			else if(nRet_1 == -1)
			{
				m_inspect++;
				if(m_inspect < 0) m_inspect = 0;
				m_bOutPrint[m_inspect] = BIN_REJECT;
				if(m_inspect >= 6)
				{
					Run_labelStep = 100;
				}
				else
				{
					Run_labelStep = 95;
				}
			}
			else if(nRet_1 == RET_ERROR)
			{//991002 0 99 "BCR 비젼이 ready 상태가 아닙니다."
				sprintf(mc_alarmcode,"991002");
				st_work.mn_run_status = CTL_dWARNING;
			}
		}
		else
		{
			m_inspect++;
			if(m_inspect < 0) m_inspect = 0;
			m_bOutPrint[m_inspect] = BIN_GOOD;
			if (st_basic.n_mode_device == 0)
			{
				Run_labelStep = 100;
				m_inspect = 0;
			}
			else
			{
				if(m_inspect >= 6)
				{
					Run_labelStep = 107;
				}
				else
				{
					Run_labelStep = 95;
				}
			}
		}
		break;

//////////////////////////////////////20120707		
	case 100:
		if (st_basic.n_mode_device == 0)//20120912 
		{
			Run_labelStep = 104;
		}
		else
		{
			if (st_picker.n_vision_eable[0] == TRUE)//20120912
			{
				if (st_picker.n_label_enable == TRUE)//20120912
				{
					//일단 한장만 출력한다
					
					//m_nOrder = m_nOutPrint % 10;
					m_nOrder = st_work.mn_Barcode_OutPrint % 10;//20140214
					nRet_1 = Func.Barcode_Create(m_nOrder, 1);	//20120725
					if(nRet_1 == CTLBD_RET_GOOD)
					{
						Run_labelStep = 101;
					}
					else
					{
						Run_labelStep = 102;
					}
					
			//		Run_labelStep = 2000;	//20120725
				}
/*				else //20140401
				{
					Func.Single_Paper_Print();
					Run_labelStep = 104;	
				}
*/
			}
			else
			{
				Func.Single_Paper_Print();
				Run_labelStep = 104;	
			}
		}
		break;

	case 101:
//		m_nOutPrint++;
//		if(m_nOutPrint % 10 == 0)
		st_work.mn_Barcode_OutPrint++;//20140214
		if(st_work.mn_Barcode_OutPrint % 10 == 0)
		{			
			Run_labelStep = 102;
		}
		else
		{
			Run_labelStep = 103;
		}
		break;

	case 102:
		nRet_1 = OnBcrRev_Excution();
		if(nRet_1 == RET_GOOD)
		{
			Run_labelStep = 103;
		}
		else if(nRet_1 == RET_ERROR)
		{
//			Run_labelStep = 102;
		}
		break;
		
	case 103:

		if(m_inspect >= 6)
		{
			Run_labelStep = 104;
		}
		else
		{
			Run_labelStep = 100;
		}
		break;
		
	case 104:
		l_PickerWaitTime[0] = GetCurrentTime();
		Run_labelStep = 105;
		break;

	case 105:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] > 500)//3000)	//20120725
		{
			Run_labelStep = 106;
		}
		break;

	case 106:
		//한장 출력 완료 확인 
		if (st_barcode.n_Prt_Comp_Reply == 0) //20120727 
		{
			if(st_barcode.b_print_complete == TRUE)
			{
				st_barcode.b_print_complete = FALSE;
				Run_labelStep = 109;
			}
			else
			{
				mn_retry++;
				if(mn_retry > 5)
				{
					mn_retry = 0;
					if (st_basic.n_mode_device != 0)
					{
						//2015.0130
						//if(st_handler.cwnd_list != NULL)
						//{
						//	sprintf(st_msg.c_abnormal_msg, "바코드 출력 완료 신호 못 받음.");
						//	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						//	{
						//		sprintf(st_msg.c_abnormal_msg, "Complete signal output Error of the bar code");
						//	}
//
//							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
//						}					
					}
					Run_labelStep = 109;
				}
				else
				{
					Run_labelStep = 106;
				}
			}

		}
		else
		{
				mn_retry = 0;
				Run_labelStep = 109;
		}
		break;

	case 107:
		Func.Label_Printer_Status();//2014,0301
		l_PickerWaitTime[0] = GetCurrentTime();
		Run_labelStep = 109;
		break;

/*	case 108:	
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] > 500)
		{
			if(st_barcode.n_barcode_hs_paper_out_error == 1)//라벨용지 교체 
			{//995002 0 99 "Label Printer Paper Out Error."
				st_work.mn_Barcode_OutPrint = 0;
				sprintf(mc_alarmcode,"995002");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2528, st_work.mn_run_status, mc_alarmcode);
				Run_labelStep = 0;
			}
			else
			{
				Run_labelStep = 109;
			}
		}
		break;
*/
	case 109:
		if(st_basic.n_mode_device != 1)
		{
			Run_labelStep = 2000;
		}
		else
		{
			if (st_picker.n_vision_eable[0] == TRUE)//20120912
			{
				Run_labelStep = 1000;
			}
			else
			{
				md_offset = 0;
				m_inspect++;
				m_bOutPrint[m_inspect] = BIN_GOOD;
				Run_labelStep = 2000;
			}
		}
		break;

	case 1000:
		nRet_1 = Func.BCR_Trigger();//0907
		if(nRet_1 == RET_GOOD)
		//if(nRet_1 == RET_GOOD || nRet_1 == RET_ERROR)
		{
			//비전 명령 
			md_offset = 0;
			Run_labelStep = 2000;
			m_inspect++;
			m_bOutPrint[m_inspect] = BIN_GOOD;

		}
		else if(nRet_1 == RET_DOOROPEN)
		{
			//비전 명령 
			md_offset = 0;
			Run_labelStep = 2000;
			m_inspect++;
			m_bOutPrint[m_inspect] = BIN_REJECT;
		}
		else if(nRet_1 == -1)
		{
			//비전 명령 
			md_offset = 0;
			Run_labelStep = 2000;
			m_inspect++;
			m_bOutPrint[m_inspect] = BIN_REJECT;
		}
		else if(nRet_1 == RET_ERROR)
		{//560700 1 56 "BCR Trigger1 테스트 에러!! - Trigger가 Ready 상태가 아님."
			mn_retry++;
			if (mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"560700");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2528, st_work.mn_run_status, mc_alarmcode);
				Run_labelStep = 107;
			}
			else
			{
				break;					
			}
		}

		break;
//////////////////////////////////////
	case 2000:
		m_nPick++;
		if(m_nPick < 0) m_nPick = 0;
		if(m_bOutPrint[m_nPick] != RET_GOOD && st_basic.n_mode_device == 1)
		{			
			if(st_handler.n_vision_reset == YES)
			{
				st_handler.n_vision_reset = NO;
				for (i = 0; i < m_inspect; i++)
				{
					m_bOutPrint[i] = 0;
				}
				//m_nOutPrint = m_inspect = 0;
				st_work.mn_Barcode_OutPrint = m_inspect = 0;//20140214
				Run_labelStep = 0;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg,"Vision Reset...Again Print");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
				}
			}
			else
			{
				Run_labelStep = 100;
			}
		}
		else
		{
			Run_labelStep = 2010;
		}
		break;

	case 2010://2012,0918
		if (st_basic.n_mode_device == 0)//20120912 
		{
			m_dTargetLabel = st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_PICK_POS] + md_offset;
			Run_labelStep = 2100;
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_print_label_chk, IO_ON) == IO_ON)
			{
				m_dTargetLabel = st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_PICK_POS] + md_offset;
				Run_labelStep = 2100;
			}
//			else if(st_barcode.n_barcode_hs_paper_out_error == 1)//라벨용지 교체 
//			{//995002 0 99 "Label Printer Paper Out Error."
//				st_work.mn_Barcode_OutPrint = 0;
//				sprintf(mc_alarmcode,"995002");
//				st_work.mn_run_status = CTL_dWARNING;
//				CTL_Lib.Alarm_Error_Occurrence(2528, st_work.mn_run_status, mc_alarmcode);
//				Run_labelStep = 0;
//			}
			else
			{//992003 0 99 "BCR가 센서에 검출되지 않습니다."
				sprintf(mc_alarmcode,"992003");//2014,0303
//				st_work.mn_run_status = CTL_dWARNING;
//				CTL_Lib.Alarm_Error_Occurrence(2529, st_work.mn_run_status, mc_alarmcode);
				Run_labelStep = 100;
			}
		}
		break;

	case 2100:
		if(st_handler.n_vision_reset == YES)
		{
			st_handler.n_vision_reset = NO;
			for (i = 0; i < m_inspect; i++)
			{
				m_bOutPrint[i] = 0;
			}
			//m_nOutPrint = m_inspect = 0;
			st_work.mn_Barcode_OutPrint = m_inspect = 0;//20140214

			Run_labelStep = 0;
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg,"Vision Reset...Again Print");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
		}
		else
		{
			Run_labelStep = 2110;
		}
		break;

	case 2110:
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, m_dTargetLabel, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_ON);
			Run_labelStep = 2200;
			//Run_labelStep = 2300;	//20120725
		}
		else if (nRet_1 == BD_RETRY)
		{
			Run_labelStep= 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(2522, CTL_dWARNING, COMI.mc_alarmcode);
			Run_labelStep = 2100;
		}
		break;

	case 2200:
//		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_ON);
		l_PickerWaitTime[0] = GetCurrentTime();
		Run_labelStep = 2210;
		break;

	case 2210:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] > 200)//700)	//20120725
		{
			Run_labelStep = 2220;
		}
		break;

	case 2220:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
		if (st_basic.n_mode_device == 0)//20120912 
		{
			Run_labelStep = 2300;
		}
		else
		{
	//		nRet_1 = IO_ON;
			if(nRet_1 == IO_ON)
			{
				//if(FAS_IO.get_in_bit(st_io.i_print_label_chk, IO_ON) != IO_ON)
				//{
					Run_labelStep = 2300;
				//}
				//else
				//{//992004 0 99 "BCR이 아직 남아 있습니다."
				//	sprintf(mc_alarmcode,"992004");
				//	st_work.mn_run_status = CTL_dWARNING;
				//	CTL_Lib.Alarm_Error_Occurrence(2597, st_work.mn_run_status, mc_alarmcode);
				//}
			}
			else
			{
				dCurr = COMI.Get_MotCurrentPos(M_LABEL_ATTACH_Z);
				if(dCurr >= st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_PICK_POS] + 0.4)
				{
					mn_attach_err = YES;
					Run_labelStep = 2300;
				}
				else
				{
					md_offset += 0.2;
					Run_labelStep = 2010;
					mn_attach_err = YES;
					Run_labelStep = 2300;
				}
			}
		}
		break;

	case 2300:
		md_offset = 0.0;
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, 0.0, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_attach_err = NO;
			nRet_2 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
			if(nRet_2 == IO_ON)
			{
				Run_labelStep = 2310;
			}
			else
			{
				//550705 0 55 "Lable Vacuum not detedted.[PS3001]"
				sprintf(mc_alarmcode,"550705");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(7523, CTL_dWARNING, mc_alarmcode);
				Run_labelStep = 2305;
			}
		}
		else if (nRet_1 == BD_RETRY)
		{
			Run_labelStep= 2300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2524, st_work.mn_run_status, COMI.mc_alarmcode);
			Run_labelStep = 2300;
		}
		break;

	case 2305:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
		if(nRet_1 == IO_ON)//550704 0 55 "Please remove the Lable Label Picker.[PS3001]"
		{
			sprintf(mc_alarmcode,"550704");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(3923, CTL_dWARNING, mc_alarmcode);
			Run_labelStep = 2305;
		}
		else
		{
			Run_labelStep = 100;
		}
		break;

	case 2310:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
		if (st_basic.n_mode_device == 0)//20120912 
		{
			Run_labelStep = 2400;
		}
		else
		{
			if(nRet_1 == IO_ON)
			{
	//			if(st_sync.n_visionrbt_label_req[0] == CTL_REQ)
	//			{
					//n_visionrbt_label_req[0] = CTL_READY;
					Run_labelStep = 2400;
	//			}
			}
			else
			{
				Run_labelStep = 100;
				Run_labelStep = 2300;//2015,0130
			}
		}
		break;

	case 2400:
		md_offset = 0.0;
		mn_retry = 0;
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_OFF);
		if(st_work.n_bcrNojob == 2 || st_work.n_bcrNojob == 3)//G1
		{
			FAS_IO.OnCylinderAction(st_io.o_vlp_bcr_clamp_onoff, IO_ON);
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);
		}
		Run_labelStep = 2600;
		break;

	case 2500:
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_OFF);
		if(st_work.n_bcrNojob == 2 || st_work.n_bcrNojob == 3)//G1
		{
			FAS_IO.OnCylinderAction(st_io.o_vlp_bcr_clamp_onoff, IO_ON);
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);
		}
		Run_labelStep = 2600;
		break;

	case 2600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.i_label_attach_unit_fwd_chk, st_io.i_label_attach_unit_bwd_chk,
			IO_ON, st_wait.n_on_wait_time[88], st_wait.n_limit_wait_time[88]);
		if(st_work.n_bcrNojob == 2 || st_work.n_bcrNojob == 3)//G1
		{
			nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_on_chk, st_io.i_vlp_bcr_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
			nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_off_chk,
				IO_OFF, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
			nRet_2 = IO_ON;
		}
		else
		{
			nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_off_chk, st_io.i_vlp_bcr_clamp_on_chk,
				IO_ON, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
			nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_on_chk,
				IO_OFF, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
		}
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			Run_labelStep = 3000;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//550200 0 55 "Label unit backward"
		 //550201 0 55 "VLP Label unit backward"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(nRet_1 != RET_PROCEED)
				{
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"Label unit backward 체크 에러.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"550200");
				}
				else
				{
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"VLP Label unit 체크 에러.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"550201");
				}
				CTL_Lib.Alarm_Error_Occurrence(2525, CTL_dWARNING, mc_alarmcode);
				Run_labelStep = 2500;
			}
			else
			{
				Run_labelStep = 2500;
			}
		}
		break;

	case 3000:
		if(st_sync.n_lotend_module_vision[VISION_1_SITE] == CTL_YES &&
			st_sync.n_lotend_module_vision[VISION_2_SITE] == CTL_YES)
		{
			Run_labelStep = 0;
		}
		else
		{
			if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_1_SITE)
			{
				if(st_sync.n_visionrbt_label_req[0] == CTL_REQ)
				{
					//n_visionrbt_label_req[0] = CTL_READY;
					m_dTargetLabel = st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_POS1] + md_offset;
					Run_labelStep = 3010;
				}
			}
			else
			{
				if(st_sync.n_visionrbt_label_req[0] == CTL_REQ)
				{
					//n_visionrbt_label_req[0] = CTL_READY;
					m_dTargetLabel = st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_POS2] + md_offset;
					Run_labelStep = 3010;
				}
			}
		}

		break;

	case 3010://2015.0131
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
		if (st_basic.n_mode_device == 0)//20120912 
		{
			Run_labelStep = 3100;
		}
		else
		{
			if(nRet_1 == IO_ON)
			{
				Run_labelStep = 3100;
			}
			else
			{
				Run_labelStep = 3020;
			}
		}
		break;

	case 3020:
		md_offset = 0.0;
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, 0.0, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
			mn_retry = 0;
			Run_labelStep = 3040;
		}
		else if (nRet_1 == BD_RETRY)
		{
			Run_labelStep= 3020;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2524, st_work.mn_run_status, COMI.mc_alarmcode);
			Run_labelStep = 3020;
		}
		break;

	case 3030:
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
		Run_labelStep = 3040;
		break;

	case 3040:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.i_label_attach_unit_bwd_chk, st_io.i_label_attach_unit_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[88], st_wait.n_limit_wait_time[88]);
		
		if(nRet_1 == RET_GOOD)
		{
			Run_labelStep = 2300;

		}
		else if(nRet_1 != RET_PROCEED)
		{//340101 0 34 "Label unit backward 체크 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"340101");
				CTL_Lib.Alarm_Error_Occurrence(2501, CTL_dWARNING, mc_alarmcode);
				Run_labelStep = 3030;
			}
			else
			{
				Run_labelStep = 3030;
			}
		}
		break;

	case 3100:
		if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_1_SITE)
		{
			nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_POS1], st_basic.nRunSpeed);
		}
		else
		{
			nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_POS2], st_basic.nRunSpeed);
		}

		if(nRet_1 == BD_GOOD)
		{
			Run_labelStep = 3200;
			/////////////////////////////////////////////////////////////// 20130124
			FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_ON);
			Run_labelStep = 3400;
			///////////////////////////////////////////////////////////////
		}
		else if (nRet_1 == BD_RETRY)
		{
			Run_labelStep= 3100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2526, st_work.mn_run_status, COMI.mc_alarmcode);
			Run_labelStep = 3100;
		}
		break;

	case 3200:
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_ON);
		l_PickerWaitTime[0] = GetCurrentTime();
		Run_labelStep = 3300;
		break;

	case 3300:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] > 100)
		{
			Run_labelStep = 3400;
		}
		break;
		
	case 3400:
		nRet_1 = CTL_Lib.Single_Move(M_LABEL_ATTACH_Z, st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_ATTACH_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
			if(st_basic.n_mode_device != 1) 
			{
				Run_labelStep = 3600;
			}
			else
			{
				l_PickerWaitTime[0] = GetCurrentTime();
				Run_labelStep = 3410;

				/////////////////////////// 20130124
				FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_ON);
				Run_labelStep = 3500;
				///////////////////////////
			}
		}
		else if (nRet_1 == BD_RETRY)
		{
			Run_labelStep= 3400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2527, st_work.mn_run_status, COMI.mc_alarmcode);
			Run_labelStep = 3400;
		}
		break;

	case 3410:
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_ON);
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] > 500)
		{
			Run_labelStep = 3500;
		}
		break;


	case 3500:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			mn_retry = 0;
			FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_ON);
			Run_labelStep = 3510;
		}
		else
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//550701 1 55 "Label Attach Place 에러."
				sprintf(mc_alarmcode, "550701");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2528, st_work.mn_run_status, mc_alarmcode);
				Run_labelStep = 3000;
			}
			else
			{
				md_offset += 0.5;
				Run_labelStep = 3000;
			}
		}
		break;

	case 3510:
//		FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
//		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_ON);
		l_PickerWaitTime[0] = GetCurrentTime();
		Run_labelStep = 3520;
		break;

	case 3520:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] < 200) break;
		nRet_1 = FAS_IO.get_in_bit(st_io.i_label_attach_vacuum_on_chk, IO_ON);
		
//		nRet_1 = IO_OFF;//임시
		if(nRet_1 == IO_ON)
		{
			//550704 1 55 "Label Picker에 Lable을 제거해 주세요."
			sprintf(mc_alarmcode,"550704");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2592, CTL_dWARNING, mc_alarmcode);
		}
		else
		{
			FAS_IO.set_out_bit(st_io.o_label_attach_eject_onoff, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_OFF);
			Run_labelStep = 3600;
		}
		break;

	case 3600:
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_label_attach_unit_bwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_vlp_bcr_clamp_onoff, IO_OFF);

		Run_labelStep = 3700;
		break;

	case 3700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_label_attach_unit_fwd_onoff, st_io.i_label_attach_unit_bwd_chk, st_io.i_label_attach_unit_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[88], st_wait.n_limit_wait_time[88]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_off_chk, st_io.i_vlp_bcr_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_vlp_bcr_clamp_onoff, st_io.i_vlp_bcr_clamp_off_chk,
			IO_ON, st_wait.n_on_wait_time[90], st_wait.n_limit_wait_time[90]);

//		nRet_1 = RET_GOOD;//임시
//		nRet_2 = RET_GOOD;//임시
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			Run_labelStep = 4000;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//550200 0 55 "Label unit backward"
		 //550201 0 55 "VLP Label unit backward"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				if(nRet_1 != RET_PROCEED)
				{
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"Label unit backward 체크 에러.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"550200");
				}
				else
				{
//					if(st_handler.cwnd_list != NULL)
//					{
//						sprintf(st_msg.c_abnormal_msg,"VLP Label unit 체크 에러.");
//						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//					}
					sprintf(mc_alarmcode,"550201");
				}
				CTL_Lib.Alarm_Error_Occurrence(2529, CTL_dWARNING, mc_alarmcode);
				Run_labelStep = 3600;
			}
			else
			{
				Run_labelStep = 3600;
			}
		}
		break;
		
	case 4000:
		st_sync.n_visionrbt_label_req[0] = CTL_READY; 
		Run_labelStep = 100;
		break;
	
	}
}

//Vision
//int n_vision_attach_req[2];	//[0]=req, [1]:
//int n_vision_buffer_req[2][2];
//int nUpperBuffer_LoadingZone_Priority;
//int nUpperBuffer_LabelingZone_Priority;
//int nUpperBuffer_UnloadingZone_Priority;
void CRun_Rbt_VisionLabel::Run_Module_Vision_Top() //VISION 1 -> Rear Robot 으로 이것부터 진행한다 
{
	int nMotor_Y = M_VISION1_Y, nMotor_Z = M_VISION1_Z, nSite = VISION_1_SITE;
	int nRet_1=0, nRet_2=0, nFlag=0;

	Func.ThreadFunctionStepTrace(31, mn_VisionStep[nSite]);
	switch(mn_VisionStep[nSite])
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_module_vision[nSite] == CTL_YES) break;
			if(st_sync.n_lotend_module_vision[nSite] == CTL_NO)
			{
				st_modulemap.VisionBuffer[D_EXIST][TOPBUFFER] = DVC_EMPTY;
				
			}

			mn_VisionStep[nSite] = 100;
		}
		break;

	/////////////////////////////////////////////////////////
	// //데이터를 클리어 한다
	/////////////////////////////////////////////////////////
	case 100: //데이터를 클리어 한다   // 20140304
		st_sync.n_visionrbt_workend_flag[nSite] = CTL_NO;	 //vision 1 작업 정보 클리어  
		mn_VisionStep[nSite] = 1000;
		break;

	/////////////////////////////////////////////////////////
	// loading robot가 놓는 자재를 받는다  
	/////////////////////////////////////////////////////////
	case 1000://2014,1209
		if(st_sync.n_lotend_sorter_site == CTL_YES && st_modulemap.VisionBuffer[D_EXIST][TOPBUFFER] == DVC_NO) //lot end가 시작되었으면 
		{
			mn_VisionStep[nSite] = 10000;
		}	
		else
		{
		//if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == CTL_REQ && st_sync.n_visionrbt_workend_flag[VISION_2_SITE] == CTL_REQ)
// 		if(st_sync.n_visionrbt_workend_flag[VISION_2_SITE] == CTL_YES) //vision 2 사이트 작업시작 플레그 
// 		{//vision2의 해당 위치의 작업이 모두 완료 되었으니 다음 작업을 진행하자  

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//현재 작업중인 위치를 다음으로 이동하기 위해 백업한다, 백업한 위치정보는 안전 및 다음 위치 이동에 활용한다 	
			//st_sync.n_visionrbt_past_working_site[nSite]는 바로전 작업위치 이다 
			//st_sync.n_visionrbt_working_site[nSite]는 현재 위치이다 
			//st_sync.n_visionrbt_next_working_site[nSite]는 다음에 작업할 위치정보 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite]; //바로전에 작업한 위치 정보를 백업/저장 한다 
			st_sync.n_visionrbt_next_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite]; //다음 작업할 위치 정보를 확인하기 위해 임시로 위치값을 저장한다 

//			st_sync.n_visionrbt_workend_flag[VISION_1_SITE] = CTL_NO;

			mn_VisionStep[nSite] = 1100;
		}	 
		break;

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 다음에 작업할 위치를 셋팅하여 작업을 시작하다 
	//////////////////////////////////////////////////////////////////////////////////////////////
	case 1100:
		if(st_sync.n_visionrbt_working_site[nSite] >= SITE_MAX_POS - 1 || st_sync.n_visionrbt_working_site[nSite] < 0) //0,1,2,3,4 까지의 위치 
		{//st_sync.n_visionrbt_next_working_site[nSite] >= SITE_MAX_POS - 1(//4)는 마지막 위치(4: SITE_ULDRBT_POS)작업까지 완료한 상태로, 대기위치(0:SITE_READY_POS)로 셋팅	
		 //st_sync.n_visionrbt_next_working_site[nSite] < 0 는 나오면 정상적인 상황에서는 나오면 안됨. 
			st_sync.n_visionrbt_next_working_site[nSite] = 0; //다음에 작업하 위치정보 
		}
		else
		{
			//////////////////////////////////////
			//다음에 작업할 위치 정보이다 정보를 알 수 있다 
			//////////////////////////////////////
			st_sync.n_visionrbt_next_working_site[nSite]++; 
		}

		
		mn_VisionStep[nSite] = 1200; 
		break;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//상태편 사이트도 위치계산등 작업이 완료된 상태, 동시에 작업하기 위해 위치계산도 같이 되어야 한다 
	//그러나 효율적인 동작을 위해 같은 이동할 위치가 같은 위치 또는 충돌 위치가 아니면 발호 이동 하기위한 조건을 찾는다 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 1200: //실제 이동할 위치 정보가 임시로 셋팅된 상태이다 , 이때 상태편(다른 비젼) 사이트와 위치를 비교하여 안전 조건을 확인한다 
		nFlag=0; 
		if(st_sync.n_visionrbt_next_working_site[nSite] == st_sync.n_visionrbt_working_site[VISION_2_SITE])
		{//앞으로 작업할 위치가 같은 위치이니 다 끝나고 이동하기를 기다려야 한다 
			nFlag++;
		}
		else if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_LDRBT_POS || st_sync.n_visionrbt_next_working_site[nSite] == SITE_LABEL_POS)
		{//SITE_LDRBT_POS & SITE_LABEL_POS 위치는 거의 같은 위치로 상태편 로봇이 이 위치에서 작업중이면 충돌이 발생하니 다 끝나고 다른 위치로 이동 하기를 기다려야 한다 
			if(st_sync.n_visionrbt_working_site[VISION_2_SITE] == SITE_LDRBT_POS) //SITE_LDRBT_POS, SITE_LABEL_POS는 거의 같은 위치로 이 위치로 이동할려면 상태편은 이 위치에 없어야 한다 
			{
				nFlag++;
			}
			else if(st_sync.n_visionrbt_working_site[VISION_2_SITE] == SITE_LABEL_POS) //SITE_LDRBT_POS, SITE_LABEL_POS는 거의 같은 위치로 이 위치로 이동할려면 상태편은 이 위치에 없어야 한다 
			{
				nFlag++;
			}
		}

		if(nFlag > 0) //앞으로 작업할 위치가 상대편 로봇이 작업중인 위치와 같으면 충돌 위치에 있으니 기다린다 
		{
			if(st_sync.n_lotend_sorter_site == CTL_YES && st_modulemap.VisionBuffer[D_EXIST][TOPBUFFER] == DVC_NO) //lot end가 시작되었으면 
			{
				mn_VisionStep[nSite] = 10000;
			}
			
			break;
		}	
		else
		{
			//바로 이동하여도 상태편과 충돌 공간이 아니니 다음 위치로 이동하여 작업이 가능하다.
			mn_VisionStep[nSite] = 1300; //모든 안전 조건 확인 완료 
		}
		break;

	case 1300:
		if(st_sync.n_visionrbt_next_working_site[nSite] > SITE_MAX_POS - 1)
		{
			//////////////////////////////////////
			//정상적이면 이 조건은 나오지 않음 
			//////////////////////////////////////
			//560000 1 56 "Vision#1 위치이동 에러. 대기 위치로 이동해 주세요"
			sprintf(mc_alarmcode,"560000");
			CTL_Lib.Alarm_Error_Occurrence(2530, CTL_dWARNING, mc_alarmcode);
			
			//AfxMessageBox("Vision 1 Robot site work  >= SITE_MAX_POS exist check error");
			break;

			st_sync.n_visionrbt_next_working_site[nSite] = 0;
			//mn_VisionStep[nSite] = 2000; //대기 위치로 이동 SITE_READY_POS
		}
		else
		{
			//////////////////////////////////////
			//다음에 작업할 정보를 알 수 있다 
			//st_sync.n_visionrbt_next_working_site[nSite]위치로 이동하고 이동 후 st_sync.n_visionrbt_working_site[nSite]의 값을 현재위치로 셋팅한다 
			//////////////////////////////////////
			if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_READY_POS) //0 //대기 위치로 이동 //SITE_READY_POS
			{
				mn_VisionStep[nSite] = 2000; //0 대기 위치로 이동 //SITE_READY_POS
			}
			else if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_LDRBT_POS)//1) //SITE_LDRBT_POS
			{
				////2014,0303
				//st_sync.n_visionrbt_load_flag[nSite] = CTL_YES;//2014,1209
				////
				mn_VisionStep[nSite] = 3000; //1
			}
			else if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_LABEL_POS) //2) //SITE_LABEL_POS
			{ //이 위치 부분은 1,2 SITE_LDRBT_POS 작업 후 바로 SITE_LABEL_POS로  이동하여 작업후 st_sync.n_visionrbt_workend_flag[VISION_1_SITE] = CTL_YES할 것으로 
			  //  이 조건은 들어오지 않는다  
				mn_VisionStep[nSite] = 4000; //2
			}
			else if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_VISION_POS) //3) //SITE_VISION_POS
			{
				mn_VisionStep[nSite] = 5000; //3
			}
			else if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_ULDRBT_POS) //4) //SITE_ULDRBT_POS
			{
				////2014,0303
				st_sync.n_visionrbt_unload_flag[nSite] = CTL_YES;
				////
				mn_VisionStep[nSite] = 6000; //4
			}
			else
			{
				//AfxMessageBox("Vision Robot next working site Error");
				//560001 1 56 "Vision#1의 다음 이동 사이트 에러"
				sprintf(mc_alarmcode,"560001");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2531, st_work.mn_run_status, mc_alarmcode);
			}
		}
		break;


	/////////////////////////////////////////////////////////
	// SITE_READY_POS 로 이동한다  
	/////////////////////////////////////////////////////////
	case 2000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560002 1 56 "Vision#1의 READY POS가 아닙니다."
			sprintf(mc_alarmcode,"560002");
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
			CTL_Lib.Alarm_Error_Occurrence(2532, st_work.mn_run_status, mc_alarmcode);

			strTmp.Format("Top_Vision : %d, Position : %s, Vision#1 is not READY POS.", nSite, VisionRobot_Current_Position(st_sync.n_visionrbt_past_working_site[nSite]));
			Func.On_LogFile_Add(99, strTmp);
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
			mn_VisionStep[nSite] = 2020;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 2010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2533, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 2010;
		} 
		break;

	case 2020:
		nRet_1 = COMI.Check_SingleMove(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_DOWN_READY]);
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
			mn_VisionStep[nSite] = 2100;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2534, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 2100;
		}
		break;

	case 2200:
		st_sync.n_visionrbt_working_site[nSite] = 0; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];
		//st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		st_sync.n_visionrbt_workend_flag[nSite] = CTL_NO; //2014,1210

		mn_VisionStep[nSite] = 2300;
		break;

	case 2300:
		mn_VisionStep[nSite] = 1000;
		break;

		/*
if(st_sync.n_ldrbt_visionrbt_req[0] == CTL_REQ)
		{
			if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_SITE_CLEAR) //어느 사이트도 사용하지 않고 있다 
			{
				st_sync.n_ldrbt_visionrbt_req[1] = VISION_1_SITE;
				Run_Vision1 = 1100;
			}		
		}
		break;

  nRet_1 = COMI.Check_MotPosRange(nMotor_Y, st_motor[nMotor_Y].d_pos[VISION_Y_LDRBT_POS], COMI.mn_allow_value[nMotor_Y]);
		if (nRet_1 == BD_GOOD) //X축이 작업 가능한 위치에 와 있는지 확인한다 
		{
			Run_Vision1 = 1110; //buffer motor Y축을 작업 가능한 위치로 이동 시킨다 
			break;
		}
*/
	


	/////////////////////////////////////////////////////////
	// Ready POS - > loading robot가 놓는 자재를 받는다  
	/////////////////////////////////////////////////////////
	case 3000:
		mn_VisionStep[nSite] = 3010;
		break;

		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560003 1 56 "Vision#1의 자재 LOADING POS가 아닙니다."
			sprintf(mc_alarmcode,"560003");
			CTL_Lib.Alarm_Error_Occurrence(2535, CTL_dWARNING, mc_alarmcode);
			break;
		}
		else
		{
			mn_VisionStep[nSite] = 3010;
		}
		break;

	case 3010:
		//혹시 모르니 실린더는 미리 동작 시키고 이동한다 
		set_dvc_clamp(nSite, IO_ON); //open 
		set_dvc_rotator(nSite, IO_OFF); //자재를 받을 수 있는 정위치 
		mn_VisionStep[nSite] = 3100; 
		break;

	case 3100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_LDRBT_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			st_sync.n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
			st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];
			mn_VisionStep[nSite] = 3200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 3100;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(2596, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 3100;
		}
		break;

	case 3200:	
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_LDMODULE], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 3400;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 3200;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(2537, CTL_dWARNING, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 3200;
		} 
		break;

	case 3300:
		set_dvc_clamp(nSite, IO_ON); //open 
		set_dvc_rotator(nSite, IO_OFF); //자재를 받을 수 있는 정위치 
		mn_VisionStep[nSite] = 3400;
		break;

	case 3400:
		nRet_1 = get_dvc_clamp(nSite, IO_ON, st_wait.n_on_wait_time[92], st_wait.n_limit_wait_time[92]);
		nRet_1 = FAS_IO.get_out_bit(st_io.o_vis_buffer_clamp_onoff[nSite], IO_ON);
		nRet_2 = get_dvc_rotator(nSite, IO_OFF, st_wait.n_on_wait_time[91], st_wait.n_limit_wait_time[91]);
		if((nRet_1 == CTL_GOOD && nRet_2 == CTL_GOOD) || st_basic.n_mode_device != 1)
		{
// 			st_sync.n_vision_buffer_req[SITE_SORTER][nSite] = CTL_REQ;//2014,1210
			mn_VisionStep[nSite] = 3500;
		}
		else if(nRet_1 == CTL_ERROR || nRet_2 == CTL_ERROR)
		{//560100 1 56 "Vision#1의 Module Clamp 에러."
		//560101 1 56 "Vision#1의 Rotator 에러."
			if(nRet_1 != CTL_GOOD) sprintf(mc_alarmcode,"560200");
			else                   sprintf(mc_alarmcode,"560101");
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
			CTL_Lib.Alarm_Error_Occurrence(2538, st_work.mn_run_status, mc_alarmcode);
			mn_VisionStep[nSite] = 3300;
		}
		break;

	case 3500:
		//st_sync.n_ldrbt_visionrbt_req[0] = CTL_REQ;
		if(st_sync.n_ldrbt_visionrbt_req[0] == CTL_REQ)
		{
			//if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_SITE_CLEAR) //어느 사이트도 사용하지 않고 있다 
			//{
				//st_sync.n_ldrbt_visionrbt_req[1] = VISION_1_SITE;
				mn_VisionStep[nSite]  = 3600;
			//}		
		}
		else if(st_sync.n_lotend_sorter_site == CTL_YES && st_modulemap.VisionBuffer[D_EXIST][TOPBUFFER] == DVC_NO) //lot end가 시작되었으면 
		{
			mn_VisionStep[nSite] = 10000;
		}
		break;

	case 3600: //자재를 로보트에서 받을 준비가 되었다 
		st_sync.n_ldrbt_visionrbt_req[0] = CTL_READY;
		st_sync.n_ldrbt_visionrbt_req[1] = nSite;
		mn_VisionStep[nSite] = 3700;
		break;

	case 3700: //
		//st_sync.n_ldrbt_visionrbt_req[0] = CTL_DOREADY;
		if(st_sync.n_ldrbt_visionrbt_req[0] == CTL_DOREADY)
		{//로보트가 자재를 놓았다 
			mn_VisionStep[nSite] = 3800;
		}
		break;

	case 3800:
		set_dvc_clamp(nSite, IO_OFF);
		mn_VisionStep[nSite] = 3810;
		break;

	case 3810:
		nRet_1 = get_dvc_clamp(nSite, IO_OFF, st_wait.n_on_wait_time[92], st_wait.n_limit_wait_time[92]);
		if(nRet_1 == CTL_GOOD || st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 3900;
		}
		else if(nRet_1 == CTL_ERROR)
		{//560100 1 56 "Vision#1의 Module Clamp 에러."
			sprintf(mc_alarmcode,"560200");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2539, st_work.mn_run_status, mc_alarmcode);
			mn_VisionStep[nSite] = 3800;
		}
		break;

	case 3900:
		//자재 정상적으로 집었 는지 확인한다 

		st_sync.n_ldrbt_visionrbt_req[0] = CTL_COMPLETE;
		mn_VisionStep[nSite] = 3910;
		break;

	case 3910:
		//st_sync.n_ldrbt_visionrbt_req[0] = CTL_FREE;
		if(st_sync.n_ldrbt_visionrbt_req[0] == CTL_FREE)
		{
			mn_VisionStep[nSite] = 3920;			
//			st_sync.n_visionrbt_label_req[0] = CTL_REQ; 
//			st_sync.n_visionrbt_label_req[1] = VISION_1_SITE; 
//			st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
//			mn_VisionStep[nSite] = 4000;
		}
		break;

	case 3920:
		//2012,0807
//		if(FAS_IO.get_in_bit(st_io.i_vis_buff1_clamp_off_chk, IO_ON) == IO_ON)
//		{
			st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
			mn_VisionStep[nSite] = 4000;
//		}
//		else
//		{//450500 1 44 "Vision#1 Module check error."
//			sprintf(mc_alarmcode,"450500");
//			alarm.mn_count_mode = 0; st_work.mn_run_status = CTL_dWARNING; alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
//			CTL_Lib.Alarm_Error_Occurrence(2591, st_work.mn_run_status, mc_alarmcode);
//		}
		break;

	///////////////////////////////////////////////////////////////////
	// Label 붙이는 작업을 한다 //SITE_LDRBT_POS - > SITE_LABEL_POS
	///////////////////////////////////////////////////////////////////
	case 4000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560004 1 56 "Vision#1의 LABEL POS가 아닙니다."
			sprintf(mc_alarmcode,"560004");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2540, st_work.mn_run_status, mc_alarmcode);
			strTmp.Format("Top_Vision : %d, Position : %s, Vision#1 is not Label POS.", nSite, VisionRobot_Current_Position(st_sync.n_visionrbt_past_working_site[nSite]));
			Func.On_LogFile_Add(99, strTmp);
			break;
		}
		else
		{
			set_dvc_rotator(nSite, IO_ON);
			mn_VisionStep[nSite] = 4100;
//			mn_VisionStep[nSite] = 4010;//20130116
//			l_PickerWaitTime[0] = GetCurrentTime();
		}
		break;

	case 4010:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] > 1000)	//20120725
		{
			mn_VisionStep[nSite] = 4100;
		}
		break;

	case 4100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_LABEL], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 4110;
			mn_VisionStep[nSite] = 4200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 4100;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2541, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 4100;
		} 
		break;

	case 4110:
		nRet_1 = COMI.Check_SingleMove(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_LABEL]);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 4200;
		}
		break;

	case 4200:
		nRet_2 = get_dvc_rotator(nSite, IO_ON, st_wait.n_on_wait_time[91], st_wait.n_limit_wait_time[91]);
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_LABEL_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD && nRet_2 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 4300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY || nRet_2 == CTL_PROCEED )
		{
			mn_VisionStep[nSite] = 4200;
		}
		else if (nRet_1 == CTLBD_RET_ERROR || nRet_2 == CTL_ERROR)
		{
			if(nRet_1 == CTLBD_RET_ERROR)
			{
//				st_work.mn_run_status = CTL_dWARNING;
//				CTL_Lib.Alarm_Error_Occurrence(2542, st_work.mn_run_status, COMI.mc_alarmcode);
				mn_VisionStep[nSite] = 4200;
				COMI.Set_MotStop( 0, nMotor_Y);//2016.0523
				CTL_Lib.mn_single_motmove_step[nMotor_Y] = 0;
			}
			else
			{
				sprintf(mc_alarmcode,"560201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2542, st_work.mn_run_status, mc_alarmcode);
				COMI.Set_MotStop( 0, nMotor_Y);
				CTL_Lib.mn_single_motmove_step[nMotor_Y] = 0;
			}
		} 
		break;

	case 4300:
		st_sync.n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];
		
		mn_VisionStep[nSite] = 4500;
		break;


	case 4400: //라벨을 붙을수 있는 상태인지 확인 
		nRet_1 = get_dvc_rotator(nSite, IO_ON, st_wait.n_on_wait_time[91], st_wait.n_limit_wait_time[91]);
		if(nRet_1 == CTL_GOOD || st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 4500;
		}
		else if(nRet_1 == CTL_ERROR)
		{//560100 1 56 "Vision#1의 Module Clamp 에러."
			sprintf(mc_alarmcode,"560100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2543, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 4500:		
		st_sync.n_visionrbt_label_req[0] = CTL_REQ; 
		st_sync.n_visionrbt_label_req[1] = VISION_1_SITE; 
		mn_VisionStep[nSite] = 4600;
		break;

	case 4600:
		//20131210
		if(st_sync.n_visionrbt_label_req[0] == CTL_READY || st_picker.n_label_enable == FALSE) //라벨 로봇이 자재를 붙여 놓았다 
		{
			st_sync.n_visionrbt_label_req[0] = CTL_FREE;
			mn_VisionStep[nSite] = 4700;
		}
		break;

	case 4700:
		set_dvc_rotator(nSite, IO_OFF);
		mn_VisionStep[nSite]  = 4800;
		mn_VisionStep[nSite]  = 4900;
		break;

	case 4800:
		nRet_1 = get_dvc_rotator(nSite, IO_OFF, st_wait.n_on_wait_time[91], st_wait.n_limit_wait_time[91]);
		if(nRet_1 == CTL_GOOD|| st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 4900;
		}
		else if(nRet_1 == CTL_ERROR)
		{//560100 1 56 "Vision#1의 Module Clamp 에러."
			sprintf(mc_alarmcode,"560100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2544, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 4900:
		st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료
		mn_VisionStep[nSite] = 1000;
		break;

	////////////////////////////////////////////////////////////////
	// VISION TEST을 한다 //SITE_LABEL_POS - > SITE_VISION_POS
	////////////////////////////////////////////////////////////////
	case 5000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560005 1 56 "Vision#1의 VISION Check POS가 아닙니다."
			sprintf(mc_alarmcode,"560005");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2545, st_work.mn_run_status, mc_alarmcode);
			strTmp.Format("Top_Vision : %d, Position : %s, Vision#1 is not VISION Check POS.", nSite, st_sync.n_visionrbt_past_working_site[nSite]);
			Func.On_LogFile_Add(99, strTmp);

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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2546, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 5010;
		} 
		break; 

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_VISION_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 5110;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 5000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			//st_work.mn_run_status = CTL_dWARNING;
			//CTL_Lib.Alarm_Error_Occurrence(2547, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 5000;
		} 
		break; 

	case 5105:
		set_dvc_rotator(nSite, IO_OFF);
		mn_VisionStep[nSite]  = 5110;
		break;

	case 5110:
		nRet_1 = get_dvc_rotator(nSite, IO_OFF, st_wait.n_on_wait_time[91], st_wait.n_limit_wait_time[91]);
		if(nRet_1 == CTL_GOOD|| st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 5200;
		}
		else if(nRet_1 == CTL_ERROR)
		{//560100 1 56 "Vision#1의 Module Clamp 에러."
			sprintf(mc_alarmcode,"560100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2544, st_work.mn_run_status, mc_alarmcode);
			mn_VisionStep[nSite]  = 5105;
		}
		break;

	case 5200:
		st_sync.n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];
		mn_VisionStep[nSite] = 5300;
		break;

	//////////////////////////////////////////////////////////////////
	//vision test를 요청한다 
	//////////////////////////////////////////////////////////////////
	case 5300: //
		st_sync.n_visionrbt_visiontest_req[nSite] = CTL_REQ;
		mn_VisionStep[nSite] = 5310;//5400;//20120725
		break;

/////////////////////////////////////// 20120725
	case 5310:	
		if (st_sync.n_visionrbt_visiontest_req[nSite] == CTL_REQ)
		{
			mn_VisionStep[nSite] = 5320;
		}
		break;

	case 5320:
//		st_modulemap.VisionBuffer[D_BIN][TOPBUFFER] = BIN_GOODBIN;
//		mn_VisionStep[nSite] = 5400;
//		break;
		
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			mn_VisionStep[nSite] = 5330;
			break;
		}

		if (st_picker.n_vision_eable[1] == TRUE)//20120912
		{
			nRet_1 = Func.HS_Trigger();
			if(nRet_1 == RET_GOOD)
			{
				NetworkingTime[0] = GetCurrentTime();
				mn_VisionStep[nSite] = 5330;
			}
			else if(nRet_1 == RET_ERROR)
			{//991003 0 99 "Heat Sink 비젼이 ready 상태기 아닙니다."
				sprintf(mc_alarmcode,"991003");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2598, st_work.mn_run_status, mc_alarmcode);
			}
		}
		else
		{
			st_modulemap.VisionBuffer[D_BIN][TOPBUFFER] = BIN_GOODBIN;
			mn_VisionStep[nSite] = 5400;
		}

		break;

	case 5330:
		NetworkingTime[1] = GetCurrentTime();
		NetworkingTime[2] = NetworkingTime[1] - NetworkingTime[0];
		if(NetworkingTime[2] < 0) NetworkingTime[0] = GetCurrentTime();
		if(NetworkingTime[2] < 30) break;
		//비전 명령
		if(FAS_IO.get_in_bit(st_io.i_vision_complste2, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_vision_passfail2, IO_OFF) == IO_OFF)
		{
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 !! - SUCCESS 상태!!");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
//			}
			st_modulemap.VisionBuffer[D_BIN][TOPBUFFER] = BIN_GOODBIN;
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_vision_complste2, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_vision_passfail2, IO_OFF) == IO_OFF)
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 !! - SUCCESS2 상태!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "HS Trigger test! - SUCCESS2 state!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				st_modulemap.VisionBuffer[D_BIN][TOPBUFFER] = BIN_GOODBIN;
			}
			else
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 !! - FAIL 상태!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "HS Trigger test! - FAIL state!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				st_modulemap.VisionBuffer[D_BIN][TOPBUFFER] = BIN_REJECTBIN;
			}
		}
		mn_VisionStep[nSite] = 5400;
		break;

///////////////////////////////////////
	case 5400: //
		st_sync.n_visionrbt_visiontest_req[nSite] = CTL_READY;

		if(st_sync.n_visionrbt_visiontest_req[nSite] == CTL_READY)
		{
			////////////////////////////////////////////////////////
			//vision test를 완료한 상태 , 결과를 받으면 된다 
			////////////////////////////////////////////////////////

			mn_VisionStep[nSite] = 5500;
		}
		break;

	case 5500:
		st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		mn_VisionStep[nSite] = 1000;
		break;

	///////////////////////////////////////////////////////////////////
	// ULD Robot 작업을 한다 //SITE_VISION_POS - > SITE_ULDRBT_POS
	///////////////////////////////////////////////////////////////////
	case 6000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560006 1 56 "Vision#1의 UNLOAD POS가 아닙니다."
			sprintf(mc_alarmcode,"560006");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2548, st_work.mn_run_status, mc_alarmcode);
			strTmp.Format("Top_Vision : %d, Position : %s, Vision#1 is not UNLOAD POS.", nSite, VisionRobot_Current_Position(st_sync.n_visionrbt_past_working_site[nSite]));
			Func.On_LogFile_Add(99, strTmp);
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2549, st_work.mn_run_status, COMI.mc_alarmcode);
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2550, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 6010;
		} 
		break; 

	case 6200:
		st_sync.n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];

		mn_VisionStep[nSite] = 6300;
		break;

	///////////////////////////////////////////////////////////////////
	// unload robot가 자재를 집어 가도록 요청한다 
	///////////////////////////////////////////////////////////////////
	case 6300:
		st_sync.n_visionrbt_uldrbt_req[0] = CTL_REQ;
		st_sync.n_visionrbt_uldrbt_req[1] = nSite;
		mn_VisionStep[nSite] = 6400;
		break;

	case 6400: //
		//st_sync.n_visionrbt_uldrbt_req[0] = CTL_READY;
		if(st_sync.n_visionrbt_uldrbt_req[0] == CTL_READY)
		{
			//언로딩 로봇이 자재를 잡아 가기위해  잡은 상태 
			mn_retry = 0;
			mn_VisionStep[nSite] = 6500;
		}
		break;

	case 6500: //벌린다 
		set_dvc_clamp(nSite, IO_ON);
		mn_VisionStep[nSite] = 6510;
		break;

	case 6510://벌린다 
		nRet_1 = get_dvc_clamp(nSite, IO_ON, st_wait.n_on_wait_time[92], st_wait.n_limit_wait_time[92]);
		nRet_1 = FAS_IO.get_out_bit(st_io.o_vis_buffer_clamp_onoff[nSite], IO_ON);
		if(nRet_1 == CTL_GOOD || st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 6600;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"560100");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2551, st_work.mn_run_status, mc_alarmcode);
				mn_VisionStep[nSite] = 6500;
			}
			else
			{
				mn_VisionStep[nSite] = 6500;
			}
		}
		break;

	case 6600://
		st_sync.n_visionrbt_uldrbt_req[0] = CTL_COMPLETE;
		st_sync.n_visionrbt_uldrbt_req[1] = nSite;
		mn_VisionStep[nSite] = 6700;
		break;

	case 6700:
		//st_sync.n_visionrbt_uldrbt_req[0] = CTL_FREE;
		if(st_sync.n_visionrbt_uldrbt_req[0] == CTL_FREE)
		{
			st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
			st_modulemap.VisionBuffer[D_EXIST][TOPBUFFER] = DVC_NO;
			mn_VisionStep[nSite] = 1000;
		}
		break;

	/////////////////////////////////////////////////////////////////////
	// 작업 대기 위치로 이동 한다 //SITE_ULDRBT_POS -> SITE_READY_POS
	/////////////////////////////////////////////////////////////////////
	case 7000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560002 1 56 "Vision#1의 READY POS가 아닙니다."
			sprintf(mc_alarmcode,"560002");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2552, st_work.mn_run_status, mc_alarmcode);
			strTmp.Format("Top_Vision : %d, Position : %s, Vision#1 is not READY POS.", nSite, VisionRobot_Current_Position(st_sync.n_visionrbt_past_working_site[nSite]));
			Func.On_LogFile_Add(99, strTmp);
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2553, st_work.mn_run_status, COMI.mc_alarmcode);
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2554, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 7000;
		} 
		break; 

	case 7200:

		st_sync.n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다
		st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];
		mn_VisionStep[nSite] = 1000;
		break;

	case 10000:
		st_sync.n_lotend_module_vision[nSite] = CTL_YES;
		mn_VisionStep[nSite] = 11000;
		break;

	case 11000:
		if( st_work.n_lotend == CTL_NO && st_sync.n_lotend_module_vision[TOPBUFFER] == CTL_NO &&
			st_sync.n_lotend_module_vision[BTMBUFFER] == CTL_NO )
		{
			mn_VisionStep[nSite] = 0;
		}
		break;

	}
}


void CRun_Rbt_VisionLabel::Run_Module_Vision_Btm() //VISION 2 -> Front robot 
{
	int nMotor_Y = M_VISION2_Y, nMotor_Z = M_VISION2_Z, nSite = VISION_2_SITE;
	int nRet_1=0, nRet_2=0, nFlag=0;

	Func.ThreadFunctionStepTrace(32, mn_VisionStep[nSite]);
	switch(mn_VisionStep[nSite])
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_module_vision[nSite] == CTL_YES) break;
			if(st_sync.n_lotend_module_vision[nSite] == CTL_NO)
			{
				st_modulemap.VisionBuffer[D_EXIST][BTMBUFFER] = DVC_EMPTY;
				
			}

			mn_VisionStep[nSite] = 100;
		}
		break;

	/////////////////////////////////////////////////////////
	// //데이터를 클리어 한다
	/////////////////////////////////////////////////////////
	case 100: //데이터를 클리어 한다   // 20140303
		//st_sync.n_visionrbt_working_site[nSite] = SITE_LDRBT_POS; //대기 위치로 최기화 한다.
		st_sync.n_visionrbt_workend_flag[nSite] = CTL_NO;	 //vision 2 작업 정보 클리어  
		mn_VisionStep[nSite] = 1000;

		break;

	/////////////////////////////////////////////////////////
	// loading robot가 놓는 자재를 받는다  
	/////////////////////////////////////////////////////////
	case 1000://2014,1209
		if(st_sync.n_lotend_sorter_site == CTL_YES && st_modulemap.VisionBuffer[D_EXIST][BTMBUFFER] == DVC_NO) //lot end가 시작되었으면 
		{
			mn_VisionStep[nSite] = 10000;
		}	
		else
		{
		//if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == CTL_REQ && st_sync.n_visionrbt_workend_flag[VISION_2_SITE] == CTL_REQ)
// 		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == CTL_YES) //vision 2 사이트 작업시작 플레그 
// 		{//vision2의 해당 위치의 작업이 모두 완료 되었으니 다음 작업을 진행하자  

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//현재 작업중인 위치를 다음으로 이동하기 위해 백업한다, 백업한 위치정보는 안전 및 다음 위치 이동에 활용한다 	
			//st_sync.n_visionrbt_past_working_site[nSite]는 바로전 작업위치 이다 
			//st_sync.n_visionrbt_working_site[nSite]는 현재 위치이다 
			//st_sync.n_visionrbt_next_working_site[nSite]는 다음에 작업할 위치정보 
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite]; //바로전에 작업한 위치 정보를 백업/저장 한다 
			st_sync.n_visionrbt_next_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite]; //다음 작업할 위치 정보를 확인하기 위해 임시로 위치값을 저장한다 

//			st_sync.n_visionrbt_workend_flag[VISION_2_SITE] = CTL_NO;

			mn_VisionStep[nSite] = 1100;
		}	 
		break;

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 다음에 작업할 위치를 셋팅하여 작업을 시작하다 
	//////////////////////////////////////////////////////////////////////////////////////////////
	case 1100:
		if(st_sync.n_visionrbt_working_site[nSite] >= SITE_MAX_POS - 1 || st_sync.n_visionrbt_working_site[nSite] < 0) //0,1,2,3,4 까지의 위치 
		{//st_sync.n_visionrbt_next_working_site[nSite] >= SITE_MAX_POS - 1(//4)는 마지막 위치(4: SITE_ULDRBT_POS)작업까지 완료한 상태로, 대기위치(0:SITE_READY_POS)로 셋팅	
		 //st_sync.n_visionrbt_next_working_site[nSite] < 0 는 나오면 정상적인 상황에서는 나오면 안됨. 
			st_sync.n_visionrbt_next_working_site[nSite] = 0; //다음에 작업하 위치정보 
		}
		else
		{
			//////////////////////////////////////
			//다음에 작업할 위치 정보이다 정보를 알 수 있다 
			//////////////////////////////////////
			st_sync.n_visionrbt_next_working_site[nSite]++; 
		}

		
		mn_VisionStep[nSite] = 1200; 
		break;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//상태편 사이트도 위치계산등 작업이 완료된 상태, 동시에 작업하기 위해 위치계산도 같이 되어야 한다 
	//그러나 효율적인 동작을 위해 같은 이동할 위치가 같은 위치 또는 충돌 위치가 아니면 발호 이동 하기위한 조건을 찾는다 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 1200: //실제 이동할 위치 정보가 임시로 셋팅된 상태이다 , 이때 상태편(다른 비젼) 사이트와 위치를 비교하여 안전 조건을 확인한다 
		nFlag=0; 

		if(st_sync.n_visionrbt_next_working_site[nSite] == st_sync.n_visionrbt_working_site[VISION_1_SITE])
		{//앞으로 작업할 위치가 같은 위치이니 다 끝나고 이동하기를 기다려야 한다 
			nFlag++;
		}
		else if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_LDRBT_POS || st_sync.n_visionrbt_next_working_site[nSite] == SITE_LABEL_POS)
		{//SITE_LDRBT_POS & SITE_LABEL_POS 위치는 거의 같은 위치로 상태편 로봇이 이 위치에서 작업중이면 충돌이 발생하니 다 끝나고 다른 위치로 이동 하기를 기다려야 한다 
			if(st_sync.n_visionrbt_working_site[VISION_1_SITE] == SITE_LDRBT_POS) //SITE_LDRBT_POS, SITE_LABEL_POS는 거의 같은 위치로 이 위치로 이동할려면 상태편은 이 위치에 없어야 한다 
			{
				nFlag++;
			}
			else if(st_sync.n_visionrbt_working_site[VISION_1_SITE] == SITE_LABEL_POS) //SITE_LDRBT_POS, SITE_LABEL_POS는 거의 같은 위치로 이 위치로 이동할려면 상태편은 이 위치에 없어야 한다 
			{
				nFlag++;
			}
		}

		if(nFlag > 0) //앞으로 작업할 위치가 상대편 로봇이 작업중인 위치와 같으면 충돌 위치에 있으니 기다린다 
		{			
			if(st_sync.n_lotend_sorter_site == CTL_YES && st_modulemap.VisionBuffer[D_EXIST][BTMBUFFER] == DVC_NO) //lot end가 시작되었으면 
			{
				mn_VisionStep[nSite] = 10000;
			}
			break;
		}	
		else
		{
			//바로 이동하여도 상태편과 충돌 공간이 아니니 다음 위치로 이동하여 작업이 가능하다.
			mn_VisionStep[nSite] = 1300; //모든 안전 조건 확인 완료 
			break;
		}
		break;

	case 1300:
		if(st_sync.n_visionrbt_next_working_site[nSite] > SITE_MAX_POS - 1)
		{
			//////////////////////////////////////
			//정상적이면 이 조건은 나오지 않음 
			//////////////////////////////////////
			//560100 1 56 "Vision#2 위치이동 에러. 대기 위치로 이동해 주세요"
			sprintf(mc_alarmcode,"560100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2555, st_work.mn_run_status, mc_alarmcode);
			//AfxMessageBox("Vision 1 Robot site work  >= SITE_MAX_POS exist check error");
			break;

			st_sync.n_visionrbt_next_working_site[nSite] = 0;
			//mn_VisionStep[nSite] = 2000; //대기 위치로 이동 SITE_READY_POS
		}
		else
		{
			//////////////////////////////////////
			//다음에 작업할 정보를 알 수 있다 
			//st_sync.n_visionrbt_next_working_site[nSite]위치로 이동하고 이동 후 st_sync.n_visionrbt_working_site[nSite]의 값을 현재위치로 셋팅한다 
			//////////////////////////////////////
			if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_READY_POS) //0 //대기 위치로 이동 //SITE_READY_POS
			{
				mn_VisionStep[nSite] = 2000; //0 대기 위치로 이동 //SITE_READY_POS
			}
			else if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_LDRBT_POS)//1) //SITE_LDRBT_POS
			{
				////2014,0303
				//st_sync.n_visionrbt_load_flag[nSite] = CTL_YES;//2014,1209
				////
				mn_VisionStep[nSite] = 3000; //1
			}
			else if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_LABEL_POS) //2) //SITE_LABEL_POS
			{ //이 위치 부분은 1,2 SITE_LDRBT_POS 작업 후 바로 SITE_LABEL_POS로  이동하여 작업후 st_sync.n_visionrbt_workend_flag[VISION_1_SITE] = CTL_YES할 것으로 
			  //  이 조건은 들어오지 않는다  
				mn_VisionStep[nSite] = 4000; //2
			}
			else if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_VISION_POS) //3) //SITE_VISION_POS
			{
				mn_VisionStep[nSite] = 5000; //3
			}
			else if(st_sync.n_visionrbt_next_working_site[nSite] == SITE_ULDRBT_POS) //4) //SITE_ULDRBT_POS
			{
				////2014,0303
				st_sync.n_visionrbt_unload_flag[nSite] = CTL_YES;
				////
				mn_VisionStep[nSite] = 6000; //4
			}
			else
			{
				//AfxMessageBox("Vision Robot next working site Error");
				//560201 1 56 "Vision#1의 다음 이동 사이트 에러"
				sprintf(mc_alarmcode,"560201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2556, st_work.mn_run_status, mc_alarmcode);
			}
		}
		break;



	/////////////////////////////////////////////////////////
	// SITE_READY_POS 로 이동한다  
	/////////////////////////////////////////////////////////
	case 2000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560502 1 56 "Vision#2의 READY POS가 아닙니다."
			sprintf(mc_alarmcode,"560502");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2557, st_work.mn_run_status, mc_alarmcode);
			strTmp.Format("Btm_Vision : %d, Position : %s, Vision#2 is not READY POS.", nSite, VisionRobot_Current_Position(st_sync.n_visionrbt_past_working_site[nSite]));
			Func.On_LogFile_Add(99, strTmp);
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
			mn_VisionStep[nSite] = 2020;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 2010;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2558, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 2010;
		} 
		break;

	case 2020:
		nRet_1 = COMI.Check_SingleMove(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_DOWN_READY]);
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
			mn_VisionStep[nSite] = 2100;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2559, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 2100;
		}
		break;

	case 2200:
		st_sync.n_visionrbt_working_site[nSite] = 0; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];
		//st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		st_sync.n_visionrbt_workend_flag[nSite] = CTL_NO; //2014,1210

		mn_VisionStep[nSite] = 2300;
		break;

	case 2300:
		mn_VisionStep[nSite] = 1000;
		break;
		
	/*	if(st_sync.n_vision_buffer_req[SITE_SORTER][0] == CTL_REQ)
		{
			st_sync.n_vision_buffer_req[SITE_SORTER][0] = CTL_READY;
			st_sync.n_vision_buffer_req[SITE_SORTER][1] = SITE_LOWER_BUFFER;
			mn_VisionStep[nSite] = 2300;
		}
		break;

	case 2300:
		if(st_sync.n_vision_buffer_req[SITE_SORTER][0] == CTL_FREE)
		{
			mn_VisionStep[nSite] = 1000;
		}
		break;*/

		/*
if(st_sync.n_ldrbt_visionrbt_req[0] == CTL_REQ)
		{
			if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_SITE_CLEAR) //어느 사이트도 사용하지 않고 있다 
			{
				st_sync.n_ldrbt_visionrbt_req[1] = VISION_1_SITE;
				Run_Vision1 = 1100;
			}		
		}
		break;

  nRet_1 = COMI.Check_MotPosRange(nMotor_Y, st_motor[nMotor_Y].d_pos[VISION_Y_LDRBT_POS], COMI.mn_allow_value[nMotor_Y]);
		if (nRet_1 == BD_GOOD) //X축이 작업 가능한 위치에 와 있는지 확인한다 
		{
			Run_Vision1 = 1110; //buffer motor Y축을 작업 가능한 위치로 이동 시킨다 
			break;
		}
*/
	
 


	/////////////////////////////////////////////////////////
	// Ready POS - > loading robot가 놓는 자재를 받는다  
	/////////////////////////////////////////////////////////
	case 3000:
		mn_VisionStep[nSite] = 3010;
		break;

		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560503 1 56 "Vision#2의 자재 LOADING POS가 아닙니다."
			sprintf(mc_alarmcode,"560503");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2560, st_work.mn_run_status, mc_alarmcode);
			break;
		}
		else
		{
			mn_VisionStep[nSite] = 3010;
		}
		break;

	case 3010:
		//혹시 모르니 실린더는 미리 동작 시키고 이동한다 
		set_dvc_clamp(nSite, IO_ON); //open 
		set_dvc_rotator(nSite, IO_OFF); //자재를 받을 수 있는 정위치 
		mn_VisionStep[nSite] = 3100; 
		break;

	case 3100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_LDRBT_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			st_sync.n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
			st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];		
			mn_VisionStep[nSite] = 3200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 3100;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2561, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 3100;
		}
		break;

	case 3200:		 
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_LDMODULE], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 3400;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 3200;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2562, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 3200;
		} 
		break;

	case 3300:
		set_dvc_clamp(nSite, IO_ON); //open 
		set_dvc_rotator(nSite, IO_OFF); //자재를 받을 수 있는 정위치 
		mn_VisionStep[nSite] = 3400;
		break;

	case 3400:
		nRet_1 = get_dvc_clamp(nSite, IO_ON, st_wait.n_on_wait_time[94], st_wait.n_limit_wait_time[94]);
		nRet_1 = FAS_IO.get_out_bit(st_io.o_vis_buffer_clamp_onoff[nSite], IO_ON);
		nRet_2 = get_dvc_rotator(nSite, IO_OFF, st_wait.n_on_wait_time[93], st_wait.n_limit_wait_time[93]);
		if((nRet_1 == CTL_GOOD && nRet_2 == CTL_GOOD) || st_basic.n_mode_device != 1)
		{
// 			st_sync.n_vision_buffer_req[SITE_SORTER][nSite] = CTL_REQ;//2014,1210
			mn_VisionStep[nSite] = 3500;
		}
		else if(nRet_1 == CTL_ERROR || nRet_2 == CTL_ERROR)
		{//560600 1 56 "Vision#2의 Module Clamp 에러."
		 //560601 1 56 "Vision#2의 Rotator 에러."
			if(nRet_1 != CTL_GOOD) sprintf(mc_alarmcode,"560600");
			else                   sprintf(mc_alarmcode,"560601");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2563, st_work.mn_run_status, mc_alarmcode);
			mn_VisionStep[nSite] = 3300;
		}
		break;



	case 3500:
		//st_sync.n_ldrbt_visionrbt_req[0] = CTL_REQ;
		if(st_sync.n_ldrbt_visionrbt_req[0] == CTL_REQ)
		{
			//if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_SITE_CLEAR) //어느 사이트도 사용하지 않고 있다 
			//{
				//st_sync.n_ldrbt_visionrbt_req[1] = VISION_1_SITE;
				mn_VisionStep[nSite]  = 3600;
			//}		
		}
		else if(st_sync.n_lotend_sorter_site == CTL_YES && st_modulemap.VisionBuffer[D_EXIST][BTMBUFFER] == DVC_NO) //lot end가 시작되었으면 
		{
			mn_VisionStep[nSite] = 10000;
		}

		break;

	case 3600: //자재를 로보트에서 받을 준비가 되었다 
		st_sync.n_ldrbt_visionrbt_req[0] = CTL_READY;
		st_sync.n_ldrbt_visionrbt_req[1] = nSite;
		mn_VisionStep[nSite] = 3700;
		break;

	case 3700: //
		//st_sync.n_ldrbt_visionrbt_req[0] = CTL_DOREADY;
		if(st_sync.n_ldrbt_visionrbt_req[0] == CTL_DOREADY)
		{//로보트가 자재를 놓았다 
			mn_VisionStep[nSite] = 3800;
		}
		break;

	case 3800:
		set_dvc_clamp(nSite, IO_OFF);
		mn_VisionStep[nSite] = 3810;
		break;

	case 3810:
		nRet_1 = get_dvc_clamp(nSite, IO_OFF, st_wait.n_on_wait_time[94], st_wait.n_limit_wait_time[94]);
		if(nRet_1 == CTL_GOOD || st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 3900;
		}
		else if(nRet_1 == CTL_ERROR)
		{//560600 1 56 "Vision#2의 Module Clamp 에러."
			sprintf(mc_alarmcode,"560600");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2564, st_work.mn_run_status, mc_alarmcode);
			mn_VisionStep[nSite] = 3800;
		}
		break;

	case 3900:
		//자재 정상적으로 집었 는지 확인한다
		st_sync.n_ldrbt_visionrbt_req[0] = CTL_COMPLETE;
		mn_VisionStep[nSite] = 3910;
		break;

	case 3910:
		//st_sync.n_ldrbt_visionrbt_req[0] = CTL_FREE;
		if(st_sync.n_ldrbt_visionrbt_req[0] == CTL_FREE)
		{
			mn_VisionStep[nSite] = 3920;
//			st_sync.n_visionrbt_label_req[0] = CTL_REQ; 
//			st_sync.n_visionrbt_label_req[1] = VISION_2_SITE;
//			st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
//			mn_VisionStep[nSite] = 4000;		
		}
		break;

	case 3920:
		//2012,0807
//		if(FAS_IO.get_in_bit(st_io.i_vis_buff2_clamp_off_chk, IO_ON) == IO_ON)
//		{
			st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
			mn_VisionStep[nSite] = 4000;
//		}
//		else
//		{//450501 1 44 "Vision#2 Module check error."
//			sprintf(mc_alarmcode,"450501");
//			alarm.mn_count_mode = 0; st_work.mn_run_status = CTL_dWARNING; alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
//			CTL_Lib.Alarm_Error_Occurrence(2592, st_work.mn_run_status, mc_alarmcode);
//		}
		break;
		
	///////////////////////////////////////////////////////////////////
	// Label 붙이는 작업을 한다 //SITE_LDRBT_POS - > SITE_LABEL_POS
	///////////////////////////////////////////////////////////////////
	case 4000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560504 1 56 "Vision#2의 LABEL POS가 아닙니다."
			sprintf(mc_alarmcode,"560504");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2565, st_work.mn_run_status, mc_alarmcode);
			strTmp.Format("Btm_Vision : %d, Position : %s, Vision#2 is not LABEL POS.", nSite, VisionRobot_Current_Position(st_sync.n_visionrbt_past_working_site[nSite]));
			Func.On_LogFile_Add(99, strTmp);
			break;
		}
		else
		{
			set_dvc_rotator(nSite, IO_ON);
			mn_VisionStep[nSite] = 4100;
//			mn_VisionStep[nSite] = 4010;//20130116
//			l_PickerWaitTime[0] = GetCurrentTime();

		}
		break;

	case 4010:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
		}
		if(l_PickerWaitTime[2] > 1000)	//20120725
		{
			mn_VisionStep[nSite] = 4100;
		}
		break;

	case 4100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_LABEL], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 4110;
			mn_VisionStep[nSite] = 4200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 4100;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2566, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 4100;
		} 
		break;

	case 4110:
		nRet_1 = COMI.Check_SingleMove(nMotor_Z, st_motor[nMotor_Z].md_pos[VISION_Z_LABEL]);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 4200;
		}
		break;

		
	case 4200:
		nRet_2 = get_dvc_rotator(nSite, IO_ON, st_wait.n_on_wait_time[93], st_wait.n_limit_wait_time[93]);
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_LABEL_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD && nRet_2 == CTL_GOOD)
		{
			mn_VisionStep[nSite] = 4300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY || nRet_2 == CTL_PROCEED)
		{
			mn_VisionStep[nSite] = 4200;
		}
		else if (nRet_1 == CTLBD_RET_ERROR || nRet_2 == CTL_ERROR)
		{
			if(nRet_1 == CTLBD_RET_ERROR)
			{
//				st_work.mn_run_status = CTL_dWARNING;
//				CTL_Lib.Alarm_Error_Occurrence(2567, st_work.mn_run_status, COMI.mc_alarmcode);
				mn_VisionStep[nSite] = 4200;
				COMI.Set_MotStop( 0, nMotor_Y);//2016.0523
				CTL_Lib.mn_single_motmove_step[nMotor_Y] = 0;
			}
			else
			{
				sprintf(mc_alarmcode,"560103");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2589, st_work.mn_run_status, mc_alarmcode);
				COMI.Set_MotStop( 0, nMotor_Y);//2016.0523
				CTL_Lib.mn_single_motmove_step[nMotor_Y] = 0;
			}

		} 
		break;

	case 4300:
		st_sync.n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];
		
		mn_VisionStep[nSite] = 4500;
		break;

	case 4400: //라벨을 붙을수 있는 상태인지 확인 
		nRet_1 = get_dvc_rotator(nSite, IO_ON, st_wait.n_on_wait_time[93], st_wait.n_limit_wait_time[93]);
		if(nRet_1 == CTL_GOOD || st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 4500;
		}
		else if(nRet_1 == CTL_ERROR)
		{////560600 1 56 "Vision#2의 Module Clamp 에러."
			sprintf(mc_alarmcode,"560600");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2568, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 4500:
		st_sync.n_visionrbt_label_req[0] = CTL_REQ; 
		st_sync.n_visionrbt_label_req[1] = VISION_2_SITE;
		mn_VisionStep[nSite] = 4600;
		break;

	case 4600:
		//20131210
		if(st_sync.n_visionrbt_label_req[0] == CTL_READY || st_picker.n_label_enable == FALSE) //라벨 로봇이 자재를 붙여 놓았다 
		{
			mn_VisionStep[nSite] = 4700;
		}
		break;

	case 4700:
		set_dvc_rotator(nSite, IO_OFF);
		mn_VisionStep[nSite] = 4800;
		mn_VisionStep[nSite] = 4900;
		break;

	case 4800:
		nRet_1 = get_dvc_rotator(nSite, IO_OFF, st_wait.n_on_wait_time[93], st_wait.n_limit_wait_time[93]);
		if(nRet_1 == CTL_GOOD || st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 4900;
		}
		else if(nRet_1 == CTL_ERROR)
		{//560600 1 56 "Vision#2의 Module Clamp 에러."
			sprintf(mc_alarmcode,"560600");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2569, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 4900:
		st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료
		mn_VisionStep[nSite] = 1000;
		break;

	////////////////////////////////////////////////////////////////
	// VISION TEST을 한다 //SITE_LABEL_POS - > SITE_VISION_POS
	////////////////////////////////////////////////////////////////
	case 5000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{//560505 1 56 "Vision#2의 VISION Check POS가 아닙니다."
			sprintf(mc_alarmcode,"560505");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2570, st_work.mn_run_status, mc_alarmcode);
			strTmp.Format("Btm_Vision : %d, Position : %s, Vision#2 is not VISION Check POS.", nSite, VisionRobot_Current_Position(st_sync.n_visionrbt_past_working_site[nSite]));
			Func.On_LogFile_Add(99, strTmp);
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2571, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 5010;
		} 
		break; 

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(nMotor_Y, st_motor[nMotor_Y].md_pos[VISION_Y_VISION_POS], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_VisionStep[nSite] = 5110;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			mn_VisionStep[nSite] = 5000;
		}
		else if (nRet_1 != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2572, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 5000;
		} 
		break; 


	case 5105:
		set_dvc_rotator(nSite, IO_OFF);
		mn_VisionStep[nSite]  = 5110;
		break;

	case 5110:
		nRet_1 = get_dvc_rotator(nSite, IO_OFF, st_wait.n_on_wait_time[93], st_wait.n_limit_wait_time[93]);
		if(nRet_1 == CTL_GOOD|| st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 5200;
		}
		else if(nRet_1 == CTL_ERROR)
		{//560100 1 56 "Vision#1의 Module Clamp 에러."
			sprintf(mc_alarmcode,"560100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2544, st_work.mn_run_status, mc_alarmcode);
			mn_VisionStep[nSite]  = 5105;
		}
		break;


	case 5200:
		st_sync.n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		mn_VisionStep[nSite] = 5300;
		break;

	//////////////////////////////////////////////////////////////////
	//vision test를 요청한다 
	//////////////////////////////////////////////////////////////////
	case 5300: //
		st_sync.n_visionrbt_visiontest_req[nSite] = CTL_REQ;
		st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];
		mn_VisionStep[nSite] =5310;// 5400;//20120725
		break;

/////////////////////////////////////// 20120725
	case 5310:	
		if (st_sync.n_visionrbt_visiontest_req[nSite] == CTL_REQ)
		{
			mn_VisionStep[nSite] = 5320;
		}
		break;

///////////////////////////////////////

	case 5320:
//		st_modulemap.VisionBuffer[D_BIN][TOPBUFFER] = BIN_GOODBIN;
//		mn_VisionStep[nSite] = 5400;
//		break;
		
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			mn_VisionStep[nSite] = 5330;
			break;
		}
		
		if (st_picker.n_vision_eable[1] == TRUE)//20120912
		{
			nRet_1 = Func.HS_Trigger();
			if(nRet_1 == RET_GOOD)// || st_basic.n_mode_device != 1)
			{
				NetworkingTime[0] = GetCurrentTime();
				mn_VisionStep[nSite] = 5330;
			}
			else if(nRet_1 == RET_ERROR)
			{//991003 0 99 "Heat Sink 비젼이 ready 상태기 아닙니다."
				sprintf(mc_alarmcode,"991003");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2599, st_work.mn_run_status, mc_alarmcode);
			}
		}
		else
		{
			st_modulemap.VisionBuffer[D_BIN][BTMBUFFER] = BIN_GOODBIN;
			mn_VisionStep[nSite] = 5400;
		}

		break;

	case 5330:
		//비전 명령 
		NetworkingTime[1] = GetCurrentTime();
		NetworkingTime[2] = NetworkingTime[1] - NetworkingTime[0];
		if(NetworkingTime[2] < 0) NetworkingTime[0] = GetCurrentTime();
		if(NetworkingTime[2] < 30) break;
		if(FAS_IO.get_in_bit(st_io.i_vision_complste2, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_vision_passfail2, IO_OFF) == IO_OFF)
		{
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 !! - SUCCESS 상태!!");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
//			}
			st_modulemap.VisionBuffer[D_BIN][BTMBUFFER] = BIN_GOODBIN;
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_vision_complste2, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_vision_passfail2, IO_OFF) == IO_OFF)
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 !! - SUCCESS2 상태!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "HS Trigger test! - SUCCESS2 state!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				st_modulemap.VisionBuffer[D_BIN][BTMBUFFER] = BIN_GOODBIN;
			}
			else
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 !! - FAIL 상태!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "HS Trigger test! - FAIL status!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				st_modulemap.VisionBuffer[D_BIN][BTMBUFFER] = BIN_REJECTBIN;
			}
		}
		mn_VisionStep[nSite] = 5400;
		break;

	case 5400: //
		st_sync.n_visionrbt_visiontest_req[nSite] = CTL_READY;
		if(st_sync.n_visionrbt_visiontest_req[nSite] == CTL_READY)
		{
			////////////////////////////////////////////////////////
			//vision test를 완료한 상태 , 결과를 받으면 된다 
			////////////////////////////////////////////////////////

			mn_VisionStep[nSite] = 5500;
		}
		break;

	case 5500:

		st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 

		mn_VisionStep[nSite] = 1000;
		break;

	///////////////////////////////////////////////////////////////////
	// ULD Robot 작업을 한다 //SITE_VISION_POS - > SITE_ULDRBT_POS
	///////////////////////////////////////////////////////////////////
	case 6000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560506 1 56 "Vision#2의 UNLOAD POS가 아닙니다."
			sprintf(mc_alarmcode,"560506");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2573, st_work.mn_run_status, mc_alarmcode);
			strTmp.Format("Btm_Vision : %d, Position : %s, Vision#2 is not UNLOAD POS.", nSite, VisionRobot_Current_Position(st_sync.n_visionrbt_past_working_site[nSite]));
			Func.On_LogFile_Add(99, strTmp);
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2573, st_work.mn_run_status, COMI.mc_alarmcode);
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2575, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 6010;
		} 
		break; 

	case 6200:
		st_sync.n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다 
		st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];

		mn_VisionStep[nSite] = 6300;
		break;

	///////////////////////////////////////////////////////////////////
	// unload robot가 자재를 집어 가도록 요청한다 
	///////////////////////////////////////////////////////////////////
	case 6300:
		st_sync.n_visionrbt_uldrbt_req[0] = CTL_REQ;
		st_sync.n_visionrbt_uldrbt_req[1] = nSite;
		mn_VisionStep[nSite] = 6400;
		break;

	case 6400: //
		if(st_sync.n_visionrbt_uldrbt_req[0] == CTL_READY)
		{
			//언로딩 로봇이 자재를 잡아 가기위해  잡은 상태 
			mn_retry = 0;
			mn_VisionStep[nSite] = 6500;
		}
		break;

	case 6500: //벌린다 
		set_dvc_clamp(nSite, IO_ON);
		mn_VisionStep[nSite] = 6510;
		break;

	case 6510://벌린다 
		nRet_1 = get_dvc_clamp(nSite, IO_ON, st_wait.n_on_wait_time[94], st_wait.n_limit_wait_time[94]);
		nRet_1 = FAS_IO.get_out_bit(st_io.o_vis_buffer_clamp_onoff[nSite], IO_ON);
		if(nRet_1 == CTL_GOOD || st_basic.n_mode_device != 1)
		{
			mn_VisionStep[nSite] = 6600;
		}
		else if(nRet_1 == CTL_ERROR)
		{
			mn_retry++;
			if (mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"560600");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2576, st_work.mn_run_status, mc_alarmcode);
				mn_VisionStep[nSite] = 6500;
			}
			else
			{
				mn_VisionStep[nSite] = 6500;
			}
		}
		break;

	case 6600://
		st_sync.n_visionrbt_uldrbt_req[0] = CTL_COMPLETE;
		st_sync.n_visionrbt_uldrbt_req[1] = nSite;
		mn_VisionStep[nSite] = 6700;
		break;

	case 6700:
		if(st_sync.n_visionrbt_uldrbt_req[0] == CTL_FREE)
		{
			st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
			st_modulemap.VisionBuffer[D_EXIST][BTMBUFFER] = DVC_NO;
			mn_VisionStep[nSite] = 1000;
		}
		break;

	/////////////////////////////////////////////////////////////////////
	// 작업 대기 위치로 이동 한다 //SITE_ULDRBT_POS -> SITE_READY_POS
	/////////////////////////////////////////////////////////////////////
	case 7000:
		nRet_1 = Find_VisionRobot_Current_Position(nSite, st_sync.n_visionrbt_past_working_site[nSite]); 
		if(nRet_1 == CTL_ERROR)
		{
			//560502 1 56 "READY POS is not of Vision # 2."
			sprintf(mc_alarmcode,"560502");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2577, st_work.mn_run_status, mc_alarmcode);
			strTmp.Format("Btm_Vision : %d, Position : %s, Vision#2 is not READY POS.", nSite, VisionRobot_Current_Position(st_sync.n_visionrbt_past_working_site[nSite]));
			Func.On_LogFile_Add(99, strTmp);
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2578, st_work.mn_run_status, COMI.mc_alarmcode);
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2579, st_work.mn_run_status, COMI.mc_alarmcode);
			mn_VisionStep[nSite] = 7000;
		} 
		break; 

	case 7200:

		st_sync.n_visionrbt_working_site[nSite]++; //현재 사이트의 위치정보를 증가하여 이동한 현재 위치를  셋팅한다
		st_sync.n_visionrbt_workend_flag[nSite] = CTL_YES; //해당 사이트 작업완료 
		st_sync.n_visionrbt_past_working_site[nSite] = st_sync.n_visionrbt_working_site[nSite];
		mn_VisionStep[nSite] = 1000;
		break;

	case 10000:
		st_sync.n_lotend_module_vision[nSite] = CTL_YES;
		mn_VisionStep[nSite] = 11000;
		break;

	case 11000:
		if( st_work.n_lotend == CTL_NO && st_sync.n_lotend_module_vision[TOPBUFFER] == CTL_NO &&
			st_sync.n_lotend_module_vision[BTMBUFFER] == CTL_NO )
		{
			mn_VisionStep[nSite] = 0;
		}
		break;
	}
}

int CRun_Rbt_VisionLabel::OnRetBufferTest(int nAxis, int nPos)
{
	int nRetPos = -1;
	if(nAxis == M_VISION1_Z || nAxis == M_VISION2_Z)
	{
		if(nPos == BCR_READY || nPos == LOAD_MODULE_READY || nPos == VISION_READY || nPos == ULD_MODULE_READY)
		{
			nRetPos = Z_VISION_SAFETY;
		}
		else if(nPos == BCR_POS)
		{
			nRetPos = Z_VISION_LABEL;
		}
		else if(nPos == LOAD_MODULE_POS)
		{
			nRetPos = Z_VISION_LDMODULE;
		}
		else if(nPos == VISION_CHK_POS)
		{
			nRetPos = Z_VISION_CHK;
		}
		else if(nPos == ULD_MODULE_POS)
		{
			nRetPos = Z_VISION_ULDMODULE;
		}
	}
	else if(nAxis == M_VISION1_Y || nAxis == M_VISION2_Y)
	{
		if(nPos == BCR_POS || nPos == BCR_READY)
		{
			nRetPos = Y_LABEL_POS;
		}
		else if(nPos == LOAD_MODULE_POS || nPos == LOAD_MODULE_READY)
		{
			nRetPos = Y_LDMODULE_POS;
		}
		else if(nPos == VISION_CHK_POS || nPos == VISION_READY)
		{
			nRetPos = Y_VISION_POS;
		}
		else if(nPos == ULD_MODULE_POS || nPos == ULD_MODULE_READY)
		{
			nRetPos = Y_ULDMODULE_POS;
		}
	}
	return nRetPos;
}

int CRun_Rbt_VisionLabel::OnChkVisionBuffer(int nPosBuff, int& zPos, int nTargetPos)
{
	int FuncRet = RET_PROCEED;

	if(nPosBuff == TOPBUFFER)
	{
		if(m_buffPos[TOPBUFFER] > m_buffPos[BTMBUFFER])
		{//
			if( nTargetPos > m_buffPos[BTMBUFFER])
			{
				FuncRet = RET_GOOD;
			}
			else if(nTargetPos < m_buffPos[BTMBUFFER])
			{
			}
			else if(nTargetPos == m_buffPos[BTMBUFFER])
			{
				FuncRet = RET_ERROR;
			}
		}
		else if(m_buffPos[TOPBUFFER] < m_buffPos[BTMBUFFER])
		{
			if( nTargetPos > m_buffPos[BTMBUFFER])
			{
				FuncRet = RET_GOOD;
			}
			else if(nTargetPos < m_buffPos[BTMBUFFER])
			{
			}
			else if(nTargetPos == m_buffPos[BTMBUFFER])
			{
				FuncRet = RET_ERROR;
			}
		}
		else if(m_buffPos[TOPBUFFER] == m_buffPos[BTMBUFFER] && m_buffPos[TOPBUFFER] == MBUFF_EMPTY)
		{
			FuncRet = RET_GOOD;
		}
	}
	else
	{
		if(m_buffPos[BTMBUFFER] > m_buffPos[TOPBUFFER])
		{//
			if( nTargetPos > m_buffPos[TOPBUFFER])
			{
				FuncRet = RET_ERROR;
			}
			else if(nTargetPos < m_buffPos[TOPBUFFER])
			{
			}
			else if(nTargetPos == m_buffPos[TOPBUFFER])
			{
				FuncRet = RET_ERROR;
			}
		}
		else if(m_buffPos[BTMBUFFER] < m_buffPos[TOPBUFFER])
		{
			if( nTargetPos > m_buffPos[TOPBUFFER])
			{
				FuncRet = RET_ERROR;
			}
			else if(nTargetPos < m_buffPos[TOPBUFFER])
			{
			}
			else if(nTargetPos == m_buffPos[TOPBUFFER])
			{
				FuncRet = RET_ERROR;
			}
		}
		else if(m_buffPos[TOPBUFFER] == m_buffPos[BTMBUFFER] && m_buffPos[BTMBUFFER] == MBUFF_EMPTY)
		{
			FuncRet = RET_GOOD;
		}
	}
	
	return FuncRet;
}



int CRun_Rbt_VisionLabel::set_dvc_clamp(int n_site, int n_onoff)
{
	//n_onoff ==IO_ON 이면 open, IO_OFF이면 close 상태이다 
	 if(n_site == VISION_1_SITE) //
	 {
		FAS_IO.set_out_bit(st_io.o_vis_buffer_clamp_onoff[n_site], n_onoff);
	 }
	 else //if(n_site == VISION_2_SITE) //1
	 {
		FAS_IO.set_out_bit(st_io.o_vis_buffer_clamp_onoff[n_site], n_onoff);
	 }
	mb_dvc_clamp_flag[n_site] = true;
	ml_dvc_clamp_wait[n_site][0] = GetCurrentTime();
	return true;
}

int CRun_Rbt_VisionLabel::set_dvc_rotator(int n_site, int n_onoff)
{
	 if(n_site == VISION_1_SITE) //0
	 {
		FAS_IO.set_out_bit(st_io.o_vis_buffer_rotator_onoff[n_site], n_onoff);
	 }
	 else //if(n_site == VISION_2_SITE) //1
	 {
		FAS_IO.set_out_bit(st_io.o_vis_buffer_rotator_onoff[n_site], n_onoff);
	 }
	mb_dvc_rotator_flag[n_site] = true;
	ml_dvc_rotator_wait[n_site][0] = GetCurrentTime();
	return true;
}

int CRun_Rbt_VisionLabel::get_dvc_clamp(int n_site, int n_onoff, int n_wait, int n_limit)
{//n_onoff ==IO_ON 이면 open, IO_OFF이면 close 상태이다 
	int nFunRet=CTL_PROCEED, nRetIO[4]={0,};

	ml_dvc_clamp_wait[n_site][1] = GetCurrentTime();
	ml_dvc_clamp_wait[n_site][2] = ml_dvc_clamp_wait[n_site][1] - ml_dvc_clamp_wait[n_site][0];
	if(ml_dvc_clamp_wait[n_site][2] < 0) ml_dvc_clamp_wait[n_site][0] = GetCurrentTime();
 
	if(n_onoff == IO_ON) //open
	{
		nRetIO[0] = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_on_chk[n_site], IO_OFF);
		nRetIO[1] = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_off_chk[n_site], IO_ON);

		if(mb_dvc_clamp_flag[n_site] == true &&  nRetIO[0] == IO_ON && nRetIO[1] == IO_OFF)
		{
			ml_dvc_clamp_wait[n_site][0] = GetCurrentTime();
			mb_dvc_clamp_flag[n_site] = false;
		}
		else if(mb_dvc_clamp_flag[n_site] == false && nRetIO[0] == IO_ON && nRetIO[1] == IO_OFF)
		{
			if(ml_dvc_clamp_wait[n_site][2] > st_wait.n_on_wait_time[92] + (n_site*2))
			{
				nFunRet = CTL_GOOD;
			}			
		}
		else 
		{
			if(ml_dvc_clamp_wait[n_site][2] > st_wait.n_limit_wait_time[92] + (n_site*2))
			{//560100 1 56 "ROBOT PICKER ROTATOR 0 DEGREE CHECK ERROR -[PS0500]."
				sprintf(mc_alarmcode, "560100"); 
				//alarm.mstr_code = "020020";	
				alarm.mn_count_mode = 0;	alarm.mn_type_mode = CTL_dWARNING; //함수 밖에서 장비 스톱처리 st_work.n_run_status = CTL_dWARNING;
				nFunRet = CTL_ERROR;
			}
		}
	}
	else //if(n_onoff == IO_OFF) // close 
	{
		nRetIO[0] = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_on_chk[n_site], IO_ON);
		nRetIO[1] = FAS_IO.get_in_bit(st_io.i_vis_buff_clamp_off_chk[n_site], IO_OFF);
	
		if(mb_dvc_clamp_flag[n_site] == true &&  nRetIO[0] == IO_OFF && nRetIO[1] == IO_ON)
		{
			ml_dvc_clamp_wait[n_site][0] = GetCurrentTime();
			mb_dvc_clamp_flag[n_site] = false;
		}
		else if(mb_dvc_clamp_flag[n_site] == false && nRetIO[0] == IO_OFF && nRetIO[1] == IO_ON)
		{
			if(ml_dvc_clamp_wait[n_site][2] > st_wait.n_on_wait_time[92] + (n_site*2))
			{
				nFunRet = CTL_GOOD;
			}			
		}
		else 
		{
			if(ml_dvc_clamp_wait[n_site][2] > st_wait.n_limit_wait_time[92] + (n_site*2))
			{//560101 1 56 "ROBOT PICKER ROTATOR 90 DEGREE CHECK ERROR -[PS0500]."
				sprintf(mc_alarmcode, "560101"); 
				alarm.mn_count_mode = 0;	alarm.mn_type_mode = CTL_dWARNING; //함수 밖에서 장비 스톱처리 st_work.n_run_status = CTL_dWARNING;
				nFunRet = CTL_ERROR;
			}
		}	
	}
	return nFunRet;

}

int CRun_Rbt_VisionLabel::get_dvc_rotator(int n_site, int n_onoff, int n_wait, int n_limit)
{
	int nFunRet=CTL_PROCEED, nRetIO[4]={0,};

	ml_dvc_rotator_wait[n_site][1] = GetCurrentTime();
	ml_dvc_rotator_wait[n_site][2] = ml_dvc_rotator_wait[n_site][1] - ml_dvc_rotator_wait[n_site][0];
	if(ml_dvc_rotator_wait[n_site][2] < 0) ml_dvc_rotator_wait[n_site][0] = GetCurrentTime();
 
	if(n_onoff == IO_ON) //
	{
		nRetIO[0] = FAS_IO.get_in_bit(st_io.i_vis_buff_rotator_on_chk[n_site], IO_ON);
		nRetIO[1] = FAS_IO.get_in_bit(st_io.i_vis_buff_rotator_off_chk[n_site], IO_OFF);
	
		if(mb_dvc_rotator_flag[n_site] == true &&  nRetIO[0] == IO_ON && nRetIO[1] == IO_OFF)
		{
			ml_dvc_rotator_wait[n_site][0] = GetCurrentTime();
			mb_dvc_rotator_flag[n_site] = false;
		}
		else if(mb_dvc_rotator_flag[n_site] == false && nRetIO[0] == IO_ON && nRetIO[1] == IO_OFF)
		{
			if(ml_dvc_rotator_wait[n_site][2] > st_wait.n_on_wait_time[91] + (n_site*2))
			{
				nFunRet = CTL_GOOD;
			}			
		}
		else 
		{
			if(ml_dvc_rotator_wait[n_site][2] > st_wait.n_limit_wait_time[91] + (n_site*2))
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

		if(mb_dvc_rotator_flag[n_site] == true &&  nRetIO[0] == IO_OFF && nRetIO[1] == IO_ON)
		{
			ml_dvc_rotator_wait[n_site][0] = GetCurrentTime();
			mb_dvc_rotator_flag[n_site] = false;
		}
		else if(mb_dvc_rotator_flag[n_site] == false && nRetIO[0] == IO_OFF && nRetIO[1] == IO_ON)
		{
			if(ml_dvc_rotator_wait[n_site][2] > st_wait.n_on_wait_time[91] + (n_site*2))
			{
				nFunRet = CTL_GOOD;
			}			
		}
		else 
		{
			if(ml_dvc_rotator_wait[n_site][2] > st_wait.n_limit_wait_time[91] + (n_site*2))
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



//2012.0629 
int CRun_Rbt_VisionLabel::Find_VisionRobot_Current_Position(int n_site, int n_check_pos)  //로보트의 현재 위치를 확인한다 
{
	int nRet_1, nRet_2, nFlag = CTL_ERROR;
//	int n_Ocheck_pos;

	switch(n_check_pos)
	{
	case SITE_READY_POS: //0
	case SITE_LDRBT_POS: //0
	case SITE_LABEL_POS: //0
	case SITE_VISION_POS: //0
	case SITE_ULDRBT_POS: //0
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




int CRun_Rbt_VisionLabel::OnBcrRev_Excution()
{
	int FuncRet = RET_PROCEED;
	CString str_serial,str_partname;

	switch (n_bcrlot_step)
	{
	case 0:
		// **********************************************************************
		//  서버와의 통신전에 접속 상태를 확인한다.
		// **********************************************************************
		if(COMI.mn_simulation_mode == 1) // 20130509
		{ 
			n_bcrlot_step = 10000;
			break;
		}

		if (st_client[NETWORK_CLIENT_1].n_connect == NO)		// 접속중이 아니면 접속을 시도한다.
		{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
			NetworkConnectTime[0] = GetCurrentTime();
			n_bcrlot_step = 10;
		}
		else													// 접속중이면...
		{
			n_bcrlot_step = 100;
		}
		break;

	case 10:
		// **********************************************************************
		//  접속을 확인한다.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != YES)
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 10000)	// 10초 동안 연결이 되지 않으면...
			{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
				
				n_bcrlot_step = 5000;		
				
				sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] 연결이 되지 않습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] is not connected");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
				//FuncRet = RET_ERROR;
			}
		}
		else											// 연결이 되었다.
		{
			n_bcrlot_step = 100;
		}
		break;

	case 100:
		// **********************************************************************
		//  시작을 알린다.
		// **********************************************************************
		{
			sprintf(st_client[NETWORK_CLIENT_1].ch_send,"%s",Func.OnNetworkDataMake(NW_LABEL_INFO));

			//if (Ret == RET_GOOD)
			//{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, NETWORK_CLIENT_1);	// 메인 프레임으로 메시지 전송
				NetworkConnectTime[0] = GetCurrentTime();
				n_bcrlot_step = 200;
			//}
		}
		break;

	case 200:
		// **********************************************************************
		//  완료가 될때까지 대기.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != NO)	// 작업이 끝나지 않았다면...
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 13000)	// 10초 동안 들어오지 않는다면...
			{
				n_bcrlot_step = 5000;

				sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] 10초간 답변이 오지 않았습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] Answer did not come 10 seconds");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
//				FuncRet = RET_ERROR;
				break;

			}
		}
		else if (st_client[NETWORK_CLIENT_1].n_connect == NO)	// 작업이 끝났다면...
		{
			n_bcrlot_step = 300;
		}	
		break;

	case 300:
		// **********************************************************************
		//  Reply를 분석한다.
		// **********************************************************************
		if(st_NW.mstr_result[1] == "PASS")
		{
			n_bcrlot_step = 400;
		}
		else								// 비정상이면...
		{
			n_bcrlot_step = 5000;

			if (st_NW.mstr_msg[1] == "")
			{
				sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER - LIST] 비정상 데이터가 전송되었습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER - LIST] data was sent to the abnormal");
				}

			}
			else
			{	
				st_NW.mstr_msg[1].TrimRight(' ');
				st_NW.mstr_msg[1].TrimLeft(' ');
				sprintf(st_msg.c_abnormal_msg, ("[LABEL SERVER ERROR] : " + st_NW.mstr_msg[1]));
				

			}			
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
			//FuncRet = RET_ERROR;

		}
		break;

	case 400:
		n_bcrlot_step = 500;
		break;

	case 500:
		// 다 정상이니까, 테스터에게 새로운 LOT임을 알려준다.
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
		}
		ml_untilwait_time[0] = GetCurrentTime();
		n_bcrlot_step = 600;
		
		break;

	case 600:
		ml_untilwait_time[1] = GetCurrentTime();
		ml_untilwait_time[2] = ml_untilwait_time[1] - ml_untilwait_time[0];
		if(st_handler.mn_receive == YES)
		{
			n_bcrlot_step = 10000;
		}
		if(ml_untilwait_time[2] > 5000)
		{
			n_bcrlot_step = 5000;
		}
		break;

	case 5000:
		FuncRet = RET_ERROR;
		n_bcrlot_step = 0;
		break;

	case 10000:
		FuncRet = RET_GOOD;
		n_bcrlot_step = 0;
		break;
	}

	return FuncRet;
}


CString CRun_Rbt_VisionLabel::VisionRobot_Current_Position( int n_check_pos)  //로보트의 현재 위치를 확인한다 
{
	CString str ="";

	if( n_check_pos < 0 ||  n_check_pos >= SITE_MAX_POS)
	{
		str = "NO_POSION";
		return str;
	}
	
	switch(n_check_pos)
	{
		case SITE_READY_POS: //0
			str = "SITE_READY_POS";
			break;
		case SITE_LDRBT_POS: //0
			str = "SITE_LDRBT_POS";
			break;;
		case SITE_LABEL_POS: //0
			str = "SITE_LABEL_POS";
			break;
		case SITE_VISION_POS: //0
			str = "SITE_VISION_POS";
			break;
		case SITE_ULDRBT_POS: //0
			str = "SITE_ULDRBT_POS";
			break;
	}

	return str;
}
