// Run_Rbt_Sorter.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Rbt_Sorter.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Function.h"
#include "ComizoaPublic.h"
#include "CtlBd_Library.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SORTER_PICKER_SITE		0
#define UNLOAD_PICKER_SITE		1
/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Sorter
CRun_Rbt_Sorter Run_Rbt_Sorter;
CRun_Rbt_Sorter::CRun_Rbt_Sorter()
{
	Thread_Variable_Initial();
}

CRun_Rbt_Sorter::~CRun_Rbt_Sorter()
{
}
/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Sorter message handlers
void CRun_Rbt_Sorter::Thread_Run()
{
	// **************************************************************************
    // 쓰레드 강제 리턴 조건 검사한다 (System Lock)                              
    // **************************************************************************
	if(st_handler.mn_system_lock != FALSE)  return;	// 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************

	switch(st_work.mn_run_status)
	{
	case dINIT:
	case dLOTREADY://20140402
		Run_Init();
		break;

	case dRUN:
		Run_Sorter_Move();
		Run_Unload_Move();
		break;

	case dREINSTATE :
		Run_Sorter_Reinstate();   // 20130416
		Run_Unload_Reinstate();	  // 20130416
		break;
	
	default:
		if(st_work.mn_run_status == dSTOP)
		{
			OnSorterRbt_FinalPos();//20121126
			OnUnloaderRbt_FinalPos();//20121126
		}

		//dSTOP, dJAM, dLOTEND, dWARNING, dLOCK, dMAINT (장비가 동작하지 않는다면!)
		if(st_handler.mn_menu_num == 501)  return;   //XYZ Robot motor 화면이면..		

		COMI.Set_MotStop(0, M_RBT_SORTER_Z);
		COMI.Set_MotStop(0, M_RBT_SORTER_X);
		COMI.Set_MotStop(0, M_RBT_SORTER_Y);
		CTL_Lib.mn_single_motmove_step[M_RBT_SORTER_Z] = 0;
		CTL_Lib.mn_single_motmove_step[M_RBT_SORTER_X] = 0;
		CTL_Lib.mn_single_motmove_step[M_RBT_SORTER_Y] = 0;
		InitStep = 0;
		ReinstateStep = 0;
		ReinstateStep1 = 0;//20121126
		break;
	}

}


void CRun_Rbt_Sorter::Run_Init()
{
	int nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	
	if(st_handler.mn_init_state[INIT_RBT_SORTER] != CTL_NO) return;

	if(alarm.n_area_ready_alarm[1] == TRUE)
	{
//		COMI.Set_MotStop(0, M_RBT_SORTER_Z);
//		COMI.Set_MotStop(0, M_RBT_SORTER_X);
//		COMI.Set_MotStop(0, M_RBT_SORTER_Y);
//		CTL_Lib.mn_single_motmove_step[M_RBT_SORTER_Z] = 0;
//		CTL_Lib.mn_single_motmove_step[M_RBT_SORTER_X] = 0;
//		CTL_Lib.mn_single_motmove_step[M_RBT_SORTER_Y] = 0;
		COMI.Set_MotStop(0, M_RBT_UNLOAD_Z);
		COMI.Set_MotStop(0, M_RBT_UNLOAD_X);
		COMI.Set_MotStop(0, M_RBT_UNLOAD_Y);
		CTL_Lib.mn_single_motmove_step[M_RBT_UNLOAD_Z] = 0;
		CTL_Lib.mn_single_motmove_step[M_RBT_UNLOAD_X] = 0;
		CTL_Lib.mn_single_motmove_step[M_RBT_UNLOAD_Y] = 0;
		return;
	}

	switch(InitStep)
	{
	case 0:
		ActionPicker[0][D_EXIST] = YES;
		ActionPicker[1][D_EXIST] = YES;	
		Set_FingerOnOff(UNLOADER_SITE, IO_OFF, ActionPicker);
		Set_FingerOnOff(SORTER_SITE, IO_OFF, ActionPicker);
		InitStep = 10;
		break;

	case 10:
		nRet_1 = Get_FingerOnOff(SORTER_SITE, IO_OFF, ActionPicker);

		if(nRet_1 == RET_GOOD)
		{
			InitStep = 20;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2493, st_work.mn_run_status, mc_alarmcode);
			InitStep = 10;
		}
		break;

	case 20:
		nRet_1 = Get_FingerOnOff(UNLOADER_SITE, IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 30;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2497, st_work.mn_run_status, mc_alarmcode);
			InitStep = 20;
		}
		break;		

	case 30:
		Set_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		Set_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);
		InitStep = 100;
		break;

	case 100:
		nRet_1 = Get_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 110;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2400, st_work.mn_run_status,mc_alarmcode);
		}
		break;

	case 110:
		nRet_1 = Get_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1000;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2401, st_work.mn_run_status,mc_alarmcode);
		}
		break;

	
	case 1000:
//		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dvc_chk, IO_OFF);
//		nRet_2 = FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dvc_chk, IO_OFF);
//		nRet_1 = nRet_2 = IO_OFF;
//		if(nRet_1 == IO_ON || nRet_2 == IO_ON)
//		{	//440300 1 44 "Label Picker Module On Check Error.[PS0400]."
//			//440301 1 44 "Sorter Picker Module On Check Error.[PS0400]."
//			if(nRet_2 == IO_ON)
//			{
//				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Picker Rbt checked dvc.");
//				alarm.mstr_pcode = _T("440300");
//			}
//			else
//			{
//				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Sorter Rbt checked dvc.");
//				alarm.mstr_pcode = _T("440301");
//			}
//			if(st_handler.cwnd_list != NULL)
//			{
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
//
//			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
//			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
//			st_work.mn_run_status = dWARNING;
//			CtlBdFunc.Alarm_Error_Occurrence(alarm.mn_count_mode, alarm.mn_type_mode, st_work.mn_run_status, alarm.mstr_pcode);
//		}
//		else
//		{
			InitStep = 1100;
//		}
		break;

	case 1100:
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 1200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1100;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2402, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1200:
		nRet_1 = COMI.HomeCheck_Mot(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			l_until_wait_time[1][0] = GetCurrentTime();
			InitStep = 1210;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1200;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2403, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1210:
		l_until_wait_time[1][1] = GetCurrentTime();
		l_until_wait_time[1][2] = l_until_wait_time[1][1] - l_until_wait_time[1][0];
		if(l_until_wait_time[1][2] < PICKERSTATUS_CHECK_TIME) break;

		if(FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dvc_chk, IO_ON) == IO_OFF)
		{
			//550505 1 55 "Sorter Picker에 모듈이 감지 됩니다. 제거해 주세요."

			sprintf(mc_alarmcode, "550505"); //피커가 벌렸는데 센서가 ON 되었다..
			CTL_Lib.Alarm_Error_Occurrence(2404, CTL_dWARNING, mc_alarmcode);
		}
		else
		{
			InitStep = 1220;	
		}
		break;

	case 1220:
		if(FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dvc_chk, IO_ON) == IO_OFF)
		{
			//550605 1 55 "Unload Picker에 모듈이 감지 됩니다. 제거해 주세요."
			sprintf(mc_alarmcode, "550605"); //피커가 벌렸는데 센서가 ON 되었다..
			CTL_Lib.Alarm_Error_Occurrence(2405, CTL_dWARNING, mc_alarmcode);
		}
		else
		{
			InitStep = 1300;	 // 현재 핑거 체크한는데 문제가 있어 벌리면 안된다 
		}
		break;
		
	case 1300:
		nRet_1 = COMI.HomeCheck_Mot(M_RBT_SORTER_X, st_motor[M_RBT_SORTER_X].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 1400;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1300;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2404, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1400:
		nRet_1 = COMI.HomeCheck_Mot(M_RBT_SORTER_Y, st_motor[M_RBT_SORTER_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 1500;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1300;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2405, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
		
	case 1500:
		nRet_1 = COMI.HomeCheck_Mot(M_RBT_UNLOAD_X, st_motor[M_RBT_UNLOAD_X].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 1600;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1500;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2406, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
		
	case 1600:
		nRet_1 = COMI.HomeCheck_Mot(M_RBT_UNLOAD_Y, st_motor[M_RBT_UNLOAD_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 3000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1600;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2407, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;


	case 3000:
		st_handler.mn_init_state[INIT_RBT_SORTER] = CTL_YES;
		InitStep = 0;
		break;

	}
}


void CRun_Rbt_Sorter::OnSorterRbt_FinalPos()//20121126
{
	int i = 0;
	if(st_work.mn_reinstatement_mode[SORTER_SITE] == 0) //stop시 최초 한번만 기록하기 위함
	{
		st_work.md_reinstatement_Pos[0][M_RBT_SORTER_X] = COMI.md_cmdpos_backup[M_RBT_SORTER_X];
		st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Y] = COMI.md_cmdpos_backup[M_RBT_SORTER_Y];
		st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Z] = COMI.md_cmdpos_backup[M_RBT_SORTER_Z];

// 		for ( i = 0; i < 8 ; i++ )
// 		{
// 			if( st_work.md_reinstatement_Pos[0][M_RBT_SORTER_X] >= ( st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY] - 0.5) &&
// 				st_work.md_reinstatement_Pos[0][M_RBT_SORTER_X] >= ( st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY] + 0.5 ) )
// 			{
// 			}
// 		}

		st_work.mn_PickerUpDnStatusBackup[SORTER_SITE][0] = FAS_IO.get_out_bit(st_io.o_sorter_picker_rbt_updn_onoff, IO_OFF);
		st_work.mn_FingerStatusBackup[SORTER_SITE][0] = FAS_IO.get_out_bit(st_io.o_sorter_picker_rbt_gripper_onoff, IO_OFF);
		
		st_work.mn_reinstatement_mode[SORTER_SITE] = 1;	//최종 위치를 기록 했다
	}
}

void CRun_Rbt_Sorter::Robot_Sorter_BackMovePosValCheck()//20121126
{
	//여기서 이전에 동작했던 위치를 다시 계산해야한다.
	st_work.md_reinstatement_Pos[1][M_RBT_SORTER_X] =  st_work.md_reinstatement_Pos[0][M_RBT_SORTER_X];
	st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Y] =  st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Y];
	st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Z] =  st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Z];

}

void CRun_Rbt_Sorter::Run_Sorter_Reinstate()//20121126
{
	int Ret1 = 0, Ret2 = 0, i = 0, n_response = 0;
	int nRet1 = 0, nRet2 = 0;

	if (st_work.mn_SorterRobot_Reinstatement_Ok == TRUE) return;//20121126
	
	switch (ReinstateStep)
	{
	case 0:
		ReinstateStep = 1000;
		break;

	case 1000://현재 위치 확인
		dSorterCurPos[0] = COMI.Get_MotCurrentPos(M_RBT_SORTER_X);
		dSorterCurPos[1] = COMI.Get_MotCurrentPos(M_RBT_SORTER_Y);
		dSorterCurPos[2] = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
	
		for (i = 0; i < 3; i++)
		{
			mn_change[SORTER_SITE][i] = 0;
		}

		//변경된 위치값 출력
		if (dSorterCurPos[0] > st_work.md_reinstatement_Pos[0][M_RBT_SORTER_X] + COMI.mn_allow_value[M_RBT_SORTER_X]
			|| dSorterCurPos[0] < st_work.md_reinstatement_Pos[0][M_RBT_SORTER_X] - COMI.mn_allow_value[M_RBT_SORTER_X])
		{
			mn_change[SORTER_SITE][0]++;
			sprintf(st_msg.c_abnormal_msg, "Sorter Robot X POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_SORTER_X], (long)dSorterCurPos[0], (long)COMI.mn_allow_value[M_RBT_SORTER_X]);
		}
		if (dSorterCurPos[1] > st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Y] + COMI.mn_allow_value[M_RBT_SORTER_Y]
			|| dSorterCurPos[1] < st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Y] - COMI.mn_allow_value[M_RBT_SORTER_Y])
		{
			mn_change[SORTER_SITE][1]++;
			sprintf(st_msg.c_abnormal_msg, "Sorter Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Y], (long)dSorterCurPos[1], (long)COMI.mn_allow_value[M_RBT_SORTER_Y]);
		}
		if (dSorterCurPos[2] > st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Z] + COMI.mn_allow_value[M_RBT_SORTER_Z]
			|| dSorterCurPos[2] < st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Z] - COMI.mn_allow_value[M_RBT_SORTER_Z])
		{
			mn_change[SORTER_SITE][2]++;
			sprintf(st_msg.c_abnormal_msg, "Sorter Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Z], (long)dSorterCurPos[2], (long)COMI.mn_allow_value[M_RBT_SORTER_Z]);
		}

		if(mn_change[SORTER_SITE][0] > 0 || mn_change[SORTER_SITE][1] > 0 || mn_change[SORTER_SITE][2] > 0) //rbt y
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
		if (backuptime[2] >= 100)
		{
			ReinstateStep = 50000;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 2000:
		Robot_Sorter_BackMovePosValCheck();
	
		COMI.Set_MotPower(M_RBT_SORTER_X, TRUE);
		COMI.Set_MotPower(M_RBT_SORTER_Y, TRUE);
		COMI.Set_MotPower(M_RBT_SORTER_Z, TRUE);

		ReinstateStep = 3000;
		break;


	case 3000:
		if(FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dvc_chk, IO_OFF) == IO_OFF)
		{
		}
		else
		{
			FAS_IO.set_out_bit(st_io.o_sorter_picker_rbt_gripper_onoff, IO_ON);
		}
		ReinstateStep = 3010;	
		break;

// 		st_work.mn_PickerUpDnStatusBackup[SORTER_SITE][0] = FAS_IO.get_out_bit(st_io.o_sorter_picker_rbt_updn_onoff, IO_OFF);
// 		st_work.mn_FingerStatusBackup[SORTER_SITE][0] = FAS_IO.get_out_bit(st_io.o_sorter_picker_rbt_gripper_onoff, IO_OFF);

	case 3010:
		Set_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		ReinstateStep = 3015;
		break;
		
	case 3015:
		nRet1 = Get_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		if (nRet1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			ReinstateStep = 3020;
		}
		else if (nRet1 == RET_ERROR)
		{

			ReinstateStep = 20000;
		}
		break;

	case 3020:
		nRet1 =  CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3100;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2447, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;


	case 3100:
		nRet1 = CTL_Lib.Single_Move(M_RBT_SORTER_X, st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3110;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2432, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3110:
		nRet1 = CTL_Lib.Single_Move(M_RBT_SORTER_Y, st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_SAFETY], st_basic.nRunSpeed);
		if(nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3120;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5024, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3120:
		st_work.mn_SorterRobot_Reinstatement_Ready = YES;
		ReinstateStep = 3130;
		break;

	case 3130:	
		if(st_work.mn_WorkBufferRobot_Reinstatement_Ok == YES)
		{
			ReinstateStep = 3200;
		}
		break;

	case 3200:
		nRet1 = CTL_Lib.Single_Move(M_RBT_SORTER_Y, st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Y], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3300;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(5024, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;		

	case 3300:
		nRet1 = COMI.Check_SingleMove(M_RBT_SORTER_Y, st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Y]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3400;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5024, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3400:
		nRet1 = CTL_Lib.Single_Move(M_RBT_SORTER_X, st_work.md_reinstatement_Pos[1][M_RBT_SORTER_X], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3500;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(2432, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;
	
	case 3500:
		nRet1 = COMI.Check_SingleMove(M_RBT_SORTER_X, st_work.md_reinstatement_Pos[1][M_RBT_SORTER_X]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3600;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2432, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;		

	case 3600:
		nRet1 = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Z], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3700;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(2447, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;
	
	case 3700:
		nRet1 = COMI.Check_SingleMove(M_RBT_SORTER_Z, st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Z]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3800;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2447, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;		

// 	st_work.mn_PickerUpDnStatusBackup[SORTER_SITE][0] = FAS_IO.get_out_bit(st_io.o_sorter_picker_rbt_updn_onoff, IO_OFF);
	case 3800:
		if(st_work.mn_PickerUpDnStatusBackup[SORTER_SITE][0] == IO_ON)
		{
			Set_PickerUpDown(SORTER_SITE, IO_ON, ActionPicker);
			ReinstateStep = 3810;		
		}
		else
		{
			ReinstateStep = 3900;		
		}
		break;
		
	case 3810:
		nRet1 = Get_PickerUpDown(SORTER_SITE, IO_ON, ActionPicker);
		if (nRet1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			ReinstateStep = 3900;
		}
		else if (nRet1 == RET_ERROR)
		{

			ReinstateStep = 3810;
		}
		break;


	case 3900:
		if(st_work.mn_FingerStatusBackup[SORTER_SITE][0] == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_sorter_picker_rbt_gripper_onoff, IO_ON);
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_sorter_picker_rbt_gripper_onoff, IO_OFF);
		}

		ReinstateStep = 50000;		
		break;

	case 20000:
		st_handler.n_sync_reinstate = NO;
		ReinstateStep = 0;
		break;

	case 50000:
		st_work.mn_SorterRobot_Reinstatement_Ready = YES;
		st_work.mn_SorterRobot_Reinstatement_Ok = YES;//20121126
		ReinstateStep = 0;
		break;
	}	
}

void CRun_Rbt_Sorter::OnUnloaderRbt_FinalPos()//20121126
{
	int i = 0;

	if(st_work.mn_reinstatement_mode[UNLOADER_SITE] == 0) //stop시 최초 한번만 기록하기 위함
	{
		st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_X] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_X);
		st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Y] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Y);
		st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Z] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);

		st_work.mn_PickerUpDnStatusBackup[UNLOADER_SITE][0] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_updn_onoff, IO_OFF);
		st_work.mn_FingerStatusBackup[UNLOADER_SITE][0] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_gripper_onoff, IO_OFF);

		st_work.mn_reinstatement_mode[UNLOADER_SITE] = 1;	//최종 위치를 기록 했다
	}
}


void CRun_Rbt_Sorter::Robot_Unloader_BackMovePosValCheck()//20121126
{
	//여기서 이전에 동작했던 위치를 다시 계산해야한다.
	st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_X] =  st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_X];
	st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Y] =  st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Y];
	st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Z] =  st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Z];

}

void CRun_Rbt_Sorter::Run_Unload_Reinstate()//20121126
{
	int Ret1 = 0, Ret2 = 0, i = 0, n_response = 0;
	int nRet1 = 0, nRet2 = 0;

	if (st_work.mn_UnloadRobot_Reinstatement_Ok == TRUE) return;//20121126
	
	switch (ReinstateStep1)
	{
	case 0:
		ReinstateStep1 = 1000;
		break;

	case 1000://현재 위치 확인
		dUldCurPos[0] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_X);
		dUldCurPos[1] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Y);
		dUldCurPos[2] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);
	
		for (i = 0; i < 3; i++)
		{
			mn_change[UNLOADER_SITE][i] = 0;
		}

		//변경된 위치값 출력
		if (dUldCurPos[0] > st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_X] + COMI.mn_allow_value[M_RBT_UNLOAD_X]
			|| dUldCurPos[0] < st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_X] - COMI.mn_allow_value[M_RBT_UNLOAD_X])
		{
			mn_change[UNLOADER_SITE][0]++;
			sprintf(st_msg.c_abnormal_msg, "Unloader Robot X POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_X], (long)dUldCurPos[0], (long)COMI.mn_allow_value[M_RBT_UNLOAD_X]);
		}
		else if (dUldCurPos[1] > st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Y] + COMI.mn_allow_value[M_RBT_UNLOAD_Y]
			|| dUldCurPos[1] < st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Y] - COMI.mn_allow_value[M_RBT_UNLOAD_Y])
		{
			mn_change[UNLOADER_SITE][1]++;
			sprintf(st_msg.c_abnormal_msg, "Unloader Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Y], (long)dUldCurPos[1], (long)COMI.mn_allow_value[M_RBT_UNLOAD_Y]);
		}
		else if (dUldCurPos[2] > st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Z] + COMI.mn_allow_value[M_RBT_UNLOAD_Z]
			|| dUldCurPos[2] < st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Z] - COMI.mn_allow_value[M_RBT_UNLOAD_Z])
		{
			mn_change[UNLOADER_SITE][2]++;
			sprintf(st_msg.c_abnormal_msg, "Unloader Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Z], (long)dUldCurPos[2], (long)COMI.mn_allow_value[M_RBT_UNLOAD_Z]);
		}


		if(mn_change[UNLOADER_SITE][0] > 0 || mn_change[UNLOADER_SITE][1] > 0 || mn_change[UNLOADER_SITE][2] > 0) //rbt y
		{
			ReinstateStep1 = 2000;
		}
		else
		{
			ReinstateStep1 = 1100;			 
		}

		break;

	case 1100:
		backuptime[0] = GetCurrentTime();
		ReinstateStep1 = 1200;
		break;

	case 1200:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if (backuptime[2] >= 100)
		{
			ReinstateStep1 = 50000;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 2000:
		Robot_Unloader_BackMovePosValCheck();
	
		COMI.Set_MotPower(M_RBT_UNLOAD_X, TRUE);
		COMI.Set_MotPower(M_RBT_UNLOAD_Y, TRUE);
		COMI.Set_MotPower(M_RBT_UNLOAD_Z, TRUE);
	
		ReinstateStep1 = 3000;
		break;

	case 3000:
		if(FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dvc_chk, IO_OFF) == IO_OFF)
		{
		}
		else
		{
			FAS_IO.set_out_bit(st_io.o_uld_picker_rbt_gripper_onoff, IO_ON);
		}
		ReinstateStep1 = 3010;	
		break;


// 		st_work.mn_PickerUpDnStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_updn_onoff, IO_OFF);
// 		st_work.mn_FingerStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_gripper_onoff, IO_OFF);
	case 3010:
		Set_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);
		ReinstateStep1 = 3015;	
		break;

	case 3015:
		nRet1 = Get_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);
		if (nRet1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			ReinstateStep1 = 3020;
		}
		else if (nRet1 == RET_ERROR)
		{
			ReinstateStep1 = 3015;
		}
		break;

	case 3020:
		nRet1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep1 = 3100;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2498, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep1 = 20000;
		}
		break;

	case 3100:
		nRet1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_X, st_motor[M_RBT_UNLOAD_X].md_pos[X_SORT_SAFETY], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep1 = 3110;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2448, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep1 = 20000;
		}
		break;

	case 3110:
		nRet1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_Y, st_motor[M_RBT_UNLOAD_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep1 = 3120;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2419, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep1 = 20000;
		}
		break;

	case 3120:
		st_work.mn_UnloadRobot_Reinstatement_Ready = YES;
		ReinstateStep1 = 3130;
		break;
		
	case 3130:	
		if(st_work.mn_WorkBufferRobot_Reinstatement_Ok == YES)
		{
			ReinstateStep1 = 3200;
		}
		break;

	case 3200:
		nRet1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_Y, st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Y], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep1 = 3300;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(2419, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep1 = 20000;
		}
		break;		

	case 3300:
		nRet1 = COMI.Check_SingleMove(M_RBT_UNLOAD_Y, st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Y]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep1 = 3400;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2419, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep1 = 20000;
		}
		break;

	case 3400:
		nRet1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_X, st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_X], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep1 = 3500;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(2448, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep1 = 20000;
		}
		break;
	
	case 3500:
		nRet1 = COMI.Check_SingleMove(M_RBT_UNLOAD_X, st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_X]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep1 = 3600;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2448, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep1 = 20000;
		}
		break;		

	case 3600:
		nRet1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Z], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep1 = 3700;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(2498, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep1 = 20000;
		}
		break;
	
	case 3700:
		nRet1 = COMI.Check_SingleMove(M_RBT_UNLOAD_Z, st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Z]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep1 = 3800;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2498, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep1 = 20000;
		}
		break;		

// 	st_work.mn_PickerUpDnStatusBackup[UNLOADER_SITE][0] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_updn_onoff, IO_OFF);
	case 3800:
		if(st_work.mn_PickerUpDnStatusBackup[UNLOADER_SITE][0] == IO_ON)
		{
			Set_PickerUpDown(UNLOADER_SITE, IO_ON, ActionPicker);
			ReinstateStep1 = 3810;		
		}
		else
		{
			ReinstateStep1 = 3900;		
		}
		break;

		
	case 3810:
		nRet1 = Get_PickerUpDown(UNLOADER_SITE, IO_ON, ActionPicker);
		if (nRet1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			ReinstateStep1 = 3900;
		}
		else if (nRet1 == RET_ERROR)
		{

			ReinstateStep1 = 3810;
		}
		break;


	case 3900:
		if(st_work.mn_FingerStatusBackup[UNLOADER_SITE][0] == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_uld_picker_rbt_gripper_onoff, IO_ON);
		}
		else
		{
			FAS_IO.OnCylinderAction(st_io.o_uld_picker_rbt_gripper_onoff, IO_OFF);
		}

		ReinstateStep1 = 50000;		
		break;


	case 20000:
		st_handler.n_sync_reinstate = NO;
		ReinstateStep1 = 0;
		break;

	case 50000:
		st_work.mn_UnloadRobot_Reinstatement_Ready = YES;
		st_work.mn_UnloadRobot_Reinstatement_Ok = YES;//20121126
		ReinstateStep1 = 0;
		break;
	}	
}

void CRun_Rbt_Sorter::Thread_Variable_Initial()
{
	Module_Place_Step = 0;
	msp_AxisNum[0] = M_RBT_SORTER_X;        //현재의 IndexNum에서 사용하는 모터 실제 번호를 가진다 
	msp_AxisNum[1] = M_RBT_SORTER_Y;  
	msp_AxisNum[2] = M_RBT_SORTER_Z;  

	mlp_AxisNum[0] = M_RBT_UNLOAD_X;        //현재의 IndexNum에서 사용하는 모터 실제 번호를 가진다 
	mlp_AxisNum[1] = M_RBT_UNLOAD_Y;  
	mlp_AxisNum[2] = M_RBT_UNLOAD_Z;  

	st_traybuffer[UNLOADER_SITE].i_unloader_row = 25;

	st_modulemap.nSortRobotPicker[D_EXIST]   = DVC_NO;
	st_modulemap.nSortRobotPicker[D_BIN]     = BIN_CLEAR;
	st_modulemap.nSortRobotPicker[D_RE_CNT]  = T_NOTTEST;
	st_modulemap.nSortRobotPicker[D_SCR_BIN] = BIN_CLEAR;

	st_modulemap.nUldRobotPicker[D_EXIST]	= DVC_NO;
	st_modulemap.nUldRobotPicker[D_BIN]     = BIN_CLEAR;
	st_modulemap.nUldRobotPicker[D_RE_CNT]	= T_NOTTEST;			// Test Count Reset
	st_modulemap.nUldRobotPicker[D_SCR_BIN]	= BIN_CLEAR;			// Screen Bin		

	ReinstateStep = 0;
	ReinstateStep1 = 0;
	st_work.mn_SorterRobot_Reinstatement_Ok = NO;
	st_work.mn_SorterRobot_Reinstatement_Ready = NO;
	st_work.mn_UnloadRobot_Reinstatement_Ok =NO;
	st_work.mn_UnloadRobot_Reinstatement_Ready = NO;

	st_sync.n_module_buffer_action_site[SORTER_SITE][0] = -1; // 20130416
	st_sync.n_module_buffer_action_site[SORTER_SITE][1] = -1; // 20130416

	//2015.0129
	st_work.mn_output_dvc = 0;
	mn_hs_cont_err = 0;
	mn_hs_tot_err = 0;
}

//Label Picker rbt & Sorter //Vision로 이동
void CRun_Rbt_Sorter::Run_Sorter_Move()
{
	int nRet_1, nRet_3; // i;
	double dCurr, dCurrPos[2];
	double dSpeedRatio[3] = {st_basic.nRunSpeed,st_basic.nRunSpeed,st_basic.nRunSpeed};
	long motor[2];
	CString strTmp;//2014.0722 ysj

//	if(alarm.n_area_ready_alarm == TRUE)
//	{
//		COMI.Set_MotStop(0, M_RBT_SORTER_Z);
//		COMI.Set_MotStop(0, M_RBT_SORTER_X);
//		COMI.Set_MotStop(0, M_RBT_SORTER_Y);
//		CTL_Lib.mn_single_motmove_step[M_RBT_SORTER_Z] = 0;
//		CTL_Lib.mn_single_motmove_step[M_RBT_SORTER_X] = 0;
//		CTL_Lib.mn_single_motmove_step[M_RBT_SORTER_Y] = 0;
//		return;
//	}
	motor[0] = M_RBT_SORTER_X;
	motor[1] = M_RBT_SORTER_Y;

	st_linearmot[SORTER_MOT_MAP].l_AxisCnt = 2;
	st_linearmot[SORTER_MOT_MAP].lp_AxisNum[0] = motor[0];
	st_linearmot[SORTER_MOT_MAP].lp_AxisNum[1] = motor[1];

	Func.ThreadFunctionStepTrace(24, RunSortStep);
	switch(RunSortStep)
	{
	case -1:
		Thread_Variable_Initial();
		RunSortStep = 0;
		RunUldStep = 0;
		break;

	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_sorter_site == CTL_NO)
			{
				RunSortStep = 100;
			}
		}
		break;

	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunSortStep = 300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunSortStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunSortStep = 100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2410, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 300:
		nRet_1 = SorterRobot_AutoMove_Safety_Zone(1,0);
		if(nRet_1 == RET_GOOD)
		{
			RunSortStep = 1000;
		}
		break;


	case 1000:
		if(COMI.mn_simulation_mode == 1)
		{
			st_ani.nSelectMotPos[M_RBT_SORTER_Y] = 0;
		}

		if(st_sync.n_lotend_modulebuffer_site == CTL_YES)
		{
			RunSortStep = 10000;
			break;
		}
		//st_sync.n_module_ldrbt_sorter_req[0] = CTL_REQ; //Clip Buffer 로딩 자재 요청 

		mn_pre_move_flag[0] = CTL_NO;
		RunSortStep = 1050;//1100;

		break;

	case 1050: // 20130416
 		if(st_work.mn_SorterRobot_Reinstatement_Ok == YES || st_work.n_multilot_start == CTL_NO)
 		{
 			nRet_3 = Func.ULDFindNode();
 			if(nRet_3 != -1)
 			{
				//2015.0131
				if(st_picker.n_work_buffer_enable[nRet_3] ==1)
 					Func.MDInsertNode(nRet_3);
//				st_basic.n_uld_node = nRet_3;//2014.0818
 			}

 			Func.ULDDelete_Node();
 			HeadVal = nRet_3;
			////2014.0722 ysj
//			strTmp.Format("Run_Rbt_Sorter - ULDFindNode : %d", HeadVal);
//			Func.On_LogFile_Add(99, strTmp);
			////
 		}
	
		RunSortStep = 1100;
		break;

	case 1100:
		st_sync.n_module_buffer_action_site[SORTER_SITE][0] = -1;
 		if(st_sync.n_module_site_status[LDMODULE_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[LEFTSINK_SITE][HeadVal][0] == BUFF_DVC_LOAD && 
			st_sync.n_module_site_status[RIGHTSINK_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[LDCLIP_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[SORTER_SITE][HeadVal][0] == BUFF_EMPTY)
		{
				st_sync.n_module_buffer_action_site[SORTER_SITE][0] = HeadVal;
				st_sync.n_module_buffer_action_site[SORTER_SITE][1] = HeadVal;
				st_sync.n_module_ldrbt_to_work[SORTER_SITE][HeadVal][0] = CTL_REQ;
		}
		
		//미리이동한다

		if(st_sync.n_module_buffer_action_site[SORTER_SITE][0] == HeadVal)
		{
			mn_NextPos = HeadVal;
			if(mn_NextPos >= 4 || mn_NextPos == -1)
				mn_NextPos = WORK_BUFFER_1;
			mn_NextPos = Func.OnRetNextBufferPos(mn_NextPos);
			mn_pre_move_flag[0] = CTL_YES;
			RunSortStep = 1200;	

			if(COMI.mn_simulation_mode == 1)
			{
				st_ani.nSelectMotPos[M_RBT_SORTER_Y] = st_sync.n_module_buffer_action_site[SORTER_SITE][1] + 1;
				l_ReinStateWaitTime[0] = GetCurrentTime();

				RunSortStep = 1150;
			}

		}
		break;

	case 1150:
		if(COMI.mn_simulation_mode == 1)
		{
			l_ReinStateWaitTime[1] = GetCurrentTime();
			l_ReinStateWaitTime[2] = l_ReinStateWaitTime[1] - l_ReinStateWaitTime[0];

			st_ani.nSelectMotPos[M_RBT_SORTER_Z] = 2;

			if(l_ReinStateWaitTime[2] < 0)
			{
				l_ReinStateWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(l_ReinStateWaitTime[2] > 2000)
			{
				st_ani.nSelectMotPos[M_RBT_SORTER_Z] = 0;

				RunSortStep = 1200;
			}
		}
		break;

	case 1200:
		dCurr = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(dCurr > st_motor[M_RBT_SORTER_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow && COMI.mn_simulation_mode != 1) // 20130509
		{
			RunSortStep = 1210; //Z축을 안전 위치로 든다 
		}
		else
		{
			ActionPicker[0][D_EXIST] = YES;
			RunSortStep = 1300;
		}
		break; 

	case 1210:
		ActionPicker[0][D_EXIST] = YES;
//		Set_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);   //2012,0930
		nRet_1 = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunSortStep = 1200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunSortStep = 1210;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2411, CTL_dWARNING, COMI.mc_alarmcode);
			RunSortStep = 1210;
		}
		break;

	case 1220:
		ActionPicker[0][D_EXIST] = YES;
		Set_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		RunSortStep = 1230;
		break;		

	case 1230:
		nRet_1 = Get_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			RunSortStep = 1300;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2412, st_work.mn_run_status,mc_alarmcode);
				RunSortStep = 1220;
			}
			else
			{
				RunSortStep = 1220;
			}
		}
		break;

	case 1300:
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		
		nRet_1 = Func.PickerSorterStatusCheck(IO_OFF,  st_modulemap.nSortRobotPicker, mResult); // 피커 정보와 실제 센서 상태를 체크 
		
		if(nRet_1 == RET_ERROR)
		{
			if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME*2))//자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[0][2] < 0)
				{
					l_until_wait_time[0][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
					break;
				}			
			}
			CTL_Lib.Alarm_Error_Occurrence(2413, CTL_dWARNING, alarm.mc_code);
			RunSortStep = 1220;
		}
		else
		{
			RunSortStep = 1400;
		}
		break;

	case 1400:
		dCurrPos[0] = st_motor[M_RBT_SORTER_X].md_pos[X_SORT_WORK_PICK1 + mn_NextPos];
		dCurrPos[1] = st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_WORK_PICK1 + mn_NextPos];

		nRet_1 = CTL_Lib.Linear_Move(SORTER_MOT_MAP, 2, msp_AxisNum, dCurrPos, dSpeedRatio);
		if(nRet_1 == BD_GOOD)
		{
			mn_pre_move_flag[0] = CTL_NO;
			l_ReinStateWaitTime[0] = GetCurrentTime();
			RunSortStep = 1500;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunSortStep = 1400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2414, CTL_dWARNING, COMI.mc_alarmcode);
			RunSortStep = 1400;
		}
		break;

	case 1500:
//////////////////////////////////////////// 추가 위치
		st_sync.n_module_buffer_action_site[SORTER_SITE][0] = -1;
 		if(st_sync.n_module_site_status[LDMODULE_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[LEFTSINK_SITE][HeadVal][0] == BUFF_DVC_LOAD && 
			st_sync.n_module_site_status[RIGHTSINK_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[LDCLIP_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[SORTER_SITE][HeadVal][0] == BUFF_EMPTY)
		{
				st_sync.n_module_buffer_action_site[SORTER_SITE][0] = HeadVal;
				st_sync.n_module_buffer_action_site[SORTER_SITE][1] = HeadVal;
				st_sync.n_module_ldrbt_to_work[SORTER_SITE][HeadVal][0] = CTL_REQ;
				RunSortStep = 2000;
				break;
		}

		if(HeadVal == -1)
		{
			RunSortStep = 1050;
		}

		if((st_work.n_loadlot_count[LDMODULE_SITE] >= st_work.nMdlInputCount[0][0]) && st_work.n_lotend == CTL_YES &&
			Func.ChkForLotEnd(SORTER_SITE) == RET_ERROR)
		{
			RunSortStep = 10000;
		}
		break;

	case 2000:
		if(st_sync.n_module_buffer_action_site[SORTER_SITE][1] == -1)
		{
			RunSortStep = 1500;
		}
		else
		{
			RunSortStep = 2100;
		}
		break;

	case 2100:
		//2012,0930
		ActionPicker[0][D_EXIST] = YES;
		RunSortStep = 2300;
		break;

		ActionPicker[0][D_EXIST] = YES;
		if(FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dn_chk, IO_OFF) == IO_ON || FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_up_chk, IO_ON) == IO_OFF)
		{
			mn_retry = 0;
			Set_FingerOnOff(SORTER_SITE, IO_OFF, ActionPicker);
			Set_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
			RunSortStep = 2200;
		}
		else
		{
			RunSortStep = 2300;
		}
		break;

	case 2110:
		Set_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		RunSortStep = 2200;
		break;

	case 2200:
		nRet_1 = Get_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			RunSortStep = 1400;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2415, CTL_dWARNING, mc_alarmcode);
				RunSortStep = 2110;
			}
			else
			{
				RunSortStep = 2110;
			}
		}
		break;

	case 2300:
		dCurr = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(dCurr > st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			RunSortStep = 2400;
		}
		else
		{
			RunSortStep = 2500;
		}
		break;

	case 2400:
		nRet_1 = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunSortStep = 2500;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunSortStep = 2400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunSortStep = 2400;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2416, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2500 :
		dCurr = COMI.Get_MotCurrentPos(M_RBT_SORTER_X);
		if(dCurr > st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY] + st_motor[M_RBT_SORTER_X].mn_allow)
		{
			RunSortStep = 2600;
		}
		else
		{
			RunSortStep = 3000;
		}
		break;

		
	case 2600:
		nRet_1 = SorterRobot_AutoMove_Safety_Zone(1,0);
		if(nRet_1 == RET_GOOD)
		{
			RunSortStep = 3000;
		}
		break;
		

	case 3000:
		nRet_1 = OnPickupModule(st_sync.n_module_buffer_action_site[SORTER_SITE][1] , 0);
		if(nRet_1 == RET_GOOD)
		{
			RunSortStep = 3100;
		}
		break;

	case 3100://2014,1210
		st_sync.n_ldrbt_visionrbt_req[0] = CTL_REQ;
		RunSortStep = 3200;

		//vision buffer Z축이 LD위치에 올라오고 받을 준비가 되었을때 Top/Btm Vision case 3400:에서 보냄
// 		if(st_sync.n_vision_buffer_req[SITE_SORTER][0] == CTL_REQ)
// 		{
// 			st_sync.n_vision_buffer_req[SITE_SORTER][0] = CTL_READY;
// 			st_sync.n_ldrbt_visionrbt_req[0] = CTL_REQ;
// 			RunSortStep = 3200;
// 		}
// 		else if(st_sync.n_vision_buffer_req[SITE_SORTER][1] == CTL_REQ)
// 		{
// 			st_sync.n_vision_buffer_req[SITE_SORTER][1] = CTL_READY;
// 			st_sync.n_ldrbt_visionrbt_req[0] = CTL_REQ;
// 			RunSortStep = 3200;
// 		}
		break;

	case 3200:
		///////////////////////// 20121114
		dCurr = COMI.Get_MotCurrentPos(M_VISION1_Y);
	//	if(dCurr > st_motor[M_VISION1_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION1_Y].mn_allow)

		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 0 && st_sync.n_ldrbt_visionrbt_req[0] == CTL_READY &&
		   st_sync.n_ldrbt_visionrbt_req[1] == VISION_1_SITE)//2014,1209 //2014,1210 1 -> 0
		{
			Func.ThreadFunctionStepTrace(90, st_sync.n_visionrbt_workend_flag[VISION_1_SITE]);
			Func.ThreadFunctionStepTrace(91, st_sync.n_ldrbt_visionrbt_req[0]);
			Func.ThreadFunctionStepTrace(92, st_sync.n_ldrbt_visionrbt_req[1]);
			
			nRet_1 = SorterRobot_AutoMove_Safety_Zone(0,0);
			if(nRet_1 == RET_GOOD)
			{
				md_Robot_X_Pos = st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY];
				md_Robot_Y_Pos = st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER];
				st_work.m_nSorterCurRetry = 0;
				RunSortStep = 4000;
			
			}
		///////////////////////// 20121114	
		}
		else if(st_sync.n_visionrbt_workend_flag[VISION_2_SITE] == 0 && st_sync.n_ldrbt_visionrbt_req[0] == CTL_READY &&
		   st_sync.n_ldrbt_visionrbt_req[1] == VISION_2_SITE) //2014,1209 //2014,1210 // if(dCurr > st_motor[M_VISION2_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION2_Y].mn_allow)
		{
			Func.ThreadFunctionStepTrace(93, st_sync.n_visionrbt_workend_flag[VISION_2_SITE]);
			Func.ThreadFunctionStepTrace(94, st_sync.n_ldrbt_visionrbt_req[0]);
			Func.ThreadFunctionStepTrace(95, st_sync.n_ldrbt_visionrbt_req[1]);
			nRet_1 = SorterRobot_AutoMove_Safety_Zone(0,0);
			if(nRet_1 == RET_GOOD)
			{
				md_Robot_X_Pos = st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY];
				md_Robot_Y_Pos = st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER2 - 4];
				st_work.m_nSorterCurRetry = 0;
				RunSortStep = 4000;
			}			
		}
		/////////////////////////
		break;

	case 4000:
		nRet_1 = OnPlaceModule(SORTER_SITE, md_Robot_X_Pos, md_Robot_Y_Pos, 0);
		if(nRet_1 == CTLBD_RET_RETRY)
		{
			//RunSortStep = 4200; //다시 처음 부터 시작 한다 
		}
		else if(nRet_1 == RET_GOOD)
		{
			RunSortStep = 4100;
		}
		break;


	case 4100:
		dCurr = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(dCurr < st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			RunSortStep = 4200;
		}
		else
		{
			RunSortStep = 4110;
		}
		break;

	case 4110:
		nRet_1 = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunSortStep = 4100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunSortStep = 4110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2417, CTL_dWARNING, COMI.mc_alarmcode);
			RunSortStep = 4110;
		}
		break;


	case 4200:
		nRet_1 = SorterRobot_AutoMove_Safety_Zone(1,0);
		if(nRet_1 == RET_GOOD)
		{
			RunSortStep = 1000;
		}
		break;		
	
	case 10000:
		st_sync.n_lotend_sorter_site = CTL_YES;
		RunSortStep = 0;
		break;
	}
}

void CRun_Rbt_Sorter::Run_Unload_Move()
{
	int i,nRet_1, nRet_2;
	int	flag,nCnt=0;

	double dCurr;
	long motor[2];
	if(alarm.n_area_ready_alarm[1] == TRUE)
	{
		COMI.Set_MotStop(0, M_RBT_UNLOAD_Z);
		COMI.Set_MotStop(0, M_RBT_UNLOAD_X);
		COMI.Set_MotStop(0, M_RBT_UNLOAD_Y);
		CTL_Lib.mn_single_motmove_step[M_RBT_UNLOAD_Z] = 0;
		CTL_Lib.mn_single_motmove_step[M_RBT_UNLOAD_X] = 0;
		CTL_Lib.mn_single_motmove_step[M_RBT_UNLOAD_Y] = 0;
		return;
	}

	motor[0] = M_RBT_UNLOAD_X;
	motor[1] = M_RBT_UNLOAD_Y;

	st_linearmot[UNLOAD_MOT_MAP].l_AxisCnt = 2;
	st_linearmot[UNLOAD_MOT_MAP].lp_AxisNum[0] = motor[0];
	st_linearmot[UNLOAD_MOT_MAP].lp_AxisNum[1] = motor[1];
	
	Func.ThreadFunctionStepTrace(25, RunUldStep); // 20140304
	switch(RunUldStep)
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_unload_site == CTL_NO)
			{
				RunUldStep = 100;
			}
		}
		break;
		
	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunUldStep = 300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunUldStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunUldStep = 100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2418, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_Y, st_motor[M_RBT_UNLOAD_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunUldStep = 500;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			RunUldStep = 300;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunUldStep = 300;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2419, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 500:
		if(st_sync.mn_uld_module_sortertray_supply[1] == CTL_FREE)
		{
			st_sync.n_module_uldrbt_tray_req[0] = CTL_REQ;
			RunUldStep = 1000;
		}
		break;

	case 1000:
		if(COMI.mn_simulation_mode == 1)
		{
			st_ani.nSelectMotPos[M_RBT_UNLOAD_Y] = 0;
			st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 2;
		}

		if(st_sync.n_lotend_module_vision[0] == CTL_YES && st_sync.n_lotend_module_vision[1] == CTL_YES)
		{
			RunUldStep = 10000;
			break;
		}	
		//st_sync.n_vision_buffer_req[SITE_ULDBUFFER][0] = CTL_REQ;
		mn_pre_move_flag[1] = CTL_NO;
		RunUldStep = 1010;
		break;

	case 1010:	
		ActionPicker[1][D_EXIST] = YES;
		if(FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dn_chk, IO_OFF) == IO_ON || FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_up_chk, IO_ON) == IO_OFF)
		{
			mn_uldretry = 0;
			Set_FingerOnOff(UNLOADER_SITE, IO_OFF, ActionPicker);
			Set_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);
			RunUldStep = 1030;
		}
		else
		{
			RunUldStep = 1050;
		}
		break;

	case 1020:
		Set_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);
		RunUldStep = 1030;
		break;

	case 1030:
		nRet_1 = Get_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			RunUldStep = 1050;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			mn_uldretry++;
			if(mn_uldretry > 3)
			{
				mn_uldretry = 0;
				st_work.mn_run_status = dWARNING;
				RunUldStep = 1020;
			}
			else
			{
				RunUldStep = 1020;
			}
		}
		break;

	case 1050:
		dCurr = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(dCurr > st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			RunUldStep = 1060;
		}
		else
		{
			RunUldStep = 1080;
		}
		break;

	case 1060:
		nRet_1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunUldStep = 1080;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunUldStep = 1060;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2420, CTL_dWARNING, COMI.mc_alarmcode);
			RunUldStep = 1060;
		}
		break;

	case 1080:
		dCurr = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_X);
		if(dCurr > st_motor[M_RBT_UNLOAD_X].md_pos[X_SORT_SAFETY] + st_motor[M_RBT_UNLOAD_X].mn_allow)
		{
			RunUldStep = 1090;
		}
		else
		{
			RunUldStep = 1100;
		}
		RunUldStep = 1100;
		break;
		
	case 1090:
		nRet_1 = UldRobot_AutoMove_Safety_Zone(1,0);
		if(nRet_1 == RET_GOOD)
		{
			RunUldStep = 1100;
		}
		break;
		
	case 1100:
		if(st_sync.n_lotend_module_vision[0] == CTL_YES && st_sync.n_lotend_module_vision[1] == CTL_YES)
		{
			UldModule_Pick_Step = 0;
			RunUldStep = 10000;
			break;
		}
		else if(st_work.n_lotend != CTL_YES && st_handler.n_lotend_ready == 5 &&  
			(st_work.nMdlPassCount[0][0] + st_work.nMdlRejectCount[0][0]) >= st_work.n_loadlot_count[LDMODULE_SITE])
		{
			st_handler.n_lotend_ready = 6;
			break;
		}
		else if(st_handler.n_more_uld_tray == CTL_NO && st_sync.mn_uld_module_sortertray_supply[0] == CTL_NOTREADY)
		{
			for(i=0 ; i<st_traybuffer[UNLOADER_SITE].i_loader_row; i++)
			{
				if(st_modulemap.UnloadTray[1][i] == DVC_NO) //디바이스가 있으면 
				{
					nCnt++;	
				}
			}
			if(nCnt <=0)
			{
				st_handler.n_more_uld_tray = CTL_YES;
				break;
			}
		}
		nRet_1 = OnUldPickupModule(0);
		if(nRet_1 == RET_NOWHERE)
		{
			RunUldStep = 1000;
		}
		else if(nRet_1 == RET_GOOD)
		{
			RunUldStep = 1200;
		}
		break;

	case 1200:
		dCurr = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);
		if(dCurr < st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_RBT_UNLOAD_Z].mn_allow || COMI.mn_simulation_mode == 1) // 20130509
		{
			RunUldStep = 1400;
		}
		else
		{
			RunUldStep = 1300;
		}
		break;
		
	case 1300:
		nRet_1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunUldStep = 1200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunUldStep = 1300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunUldStep = 1300;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2421, CTL_Lib.mn_run_status, COMI.mc_alarmcode);
		}
		break;


	case 1400:	
		if(st_modulemap.nUldRobotPicker[D_BIN] == BIN_GOODBIN)
		{
			//nRet_1 = UldRobot_AutoMove_Safety_Zone(1,0);
			//if(nRet_1 == RET_GOOD)
			//{
			//	st_sync.n_module_uldrbt_tray_req[0] = CTL_REQ; //로딩 자재 요청 
				RunUldStep = 1500;
			//}
		}
		else
		{
			//st_sync.n_module_uldrbt_tray_req[0] = CTL_REQ; //로딩 자재 요청 
			RunUldStep = 1500;
		}
		break;

	case 1500:
		if(st_modulemap.nUldRobotPicker[D_BIN] == BIN_GOODBIN)
		{
			RunUldStep = 2000;
		}
		else
		{
			RunUldStep = 3010;
		}
		//RunUldStep = 1500;

		//0831 Qual VLPDMM
		//RunUldStep = 2000;
		//st_modulemap.nUldRobotPicker[D_BIN] = BIN_GOODBIN;
		break;

	case 2000:
		flag = CTL_NO;
		mn_bcr_goodbin_cnt = 0; 
		mn_bcr_rejectbin_cnt = 0;

		if(st_modulemap.nUldRobotPicker[D_EXIST] == DVC_YES) //자재를 집고 있고
		{
			if(st_modulemap.nUldRobotPicker[D_BIN] == BIN_GOODBIN)
			{
				mn_bcr_goodbin_cnt++;
			}				
			else if(st_modulemap.nUldRobotPicker[D_BIN] == BIN_REJECTBIN)
			{
				mn_bcr_rejectbin_cnt++;
			}
			else
			{
				st_modulemap.nUldRobotPicker[D_BIN] = BIN_REJECTBIN;
				mn_bcr_rejectbin_cnt++;
			}
			flag = CTL_YES;
		}

		if(flag == CTL_NO)
		{
			RunUldStep = 3100; 
			break;
		}

		if(mn_bcr_rejectbin_cnt > 0 ) //리젝 자재를 모두 리젝 버퍼에 언로딩  가능하면 언로딩 한다 
		{
			if(st_sync.n_rejectbin_tray_change_flag == CTL_YES) //이미 리젝 트레이에 놓던 중이면 계속 놓는다. 다 찰때까지 
			{
				RunUldStep = 2200;  //리젝 트레이에 리젝 자재를 처리한다 				
				
				if(COMI.mn_simulation_mode == 1)
				{
					l_ReinStateWaitTime[0] = GetCurrentTime();
					st_ani.nSelectMotPos[M_RBT_UNLOAD_Y] = 2;
					st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 0;

					RunUldStep = 2030;
				}

			}
			else
			{ 
				st_sync.n_rejectbin_tray_change_flag = CTL_YES; //리젝 트레이 교체중 임을 알리는 플레그  

				RunUldStep = 2200; 
				
				if(COMI.mn_simulation_mode == 1)
				{
					l_ReinStateWaitTime[0] = GetCurrentTime();
					st_ani.nSelectMotPos[M_RBT_UNLOAD_Y] = 2;
					st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 0;

					RunUldStep = 2030;
				}

			}
		}
		else if(mn_bcr_goodbin_cnt > 0) //로딩 사이트가 현재 바코드가 끝난 로딩 자재를 놓을 수 있는 조건이면 놓느다 
		{
			RunUldStep = 2100;  
			
			if(COMI.mn_simulation_mode == 1)
			{
				l_ReinStateWaitTime[0] = GetCurrentTime();
				st_ani.nSelectMotPos[M_RBT_UNLOAD_Y] = 2;
				st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 0;

				RunUldStep = 2050;
			}

		}		
		break;

	case 2030:
		if(COMI.mn_simulation_mode == 1)
		{
			l_ReinStateWaitTime[1] = GetCurrentTime();
			l_ReinStateWaitTime[2] = l_ReinStateWaitTime[1] - l_ReinStateWaitTime[0];

			st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 2;

			if(l_ReinStateWaitTime[2] < 0)
			{
				l_ReinStateWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(l_ReinStateWaitTime[2] > 1000)
			{
				st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 0;
				RunUldStep = 2200;
			}
		}
		break;

	case 2050:
		if(COMI.mn_simulation_mode == 1)
		{
			l_ReinStateWaitTime[1] = GetCurrentTime();
			l_ReinStateWaitTime[2] = l_ReinStateWaitTime[1] - l_ReinStateWaitTime[0];

			st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 2;

			if(l_ReinStateWaitTime[2] < 0)
			{
				l_ReinStateWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(l_ReinStateWaitTime[2] > 1000)
			{
				st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 0;
				RunUldStep = 2100;
			}
		}
		break;

	case 2100:
		nRet_1 = Find_TrayInfo_Count(UNLOADER_SITE, CTL_NO, -1);// 디바이스가 있으면 CTL_YES, 없으면 CTL_NO 임 
		if(st_sync.n_module_uldrbt_tray_req[0] == CTL_READY) //아직 트레이가 준비되지 않았다 
		{		
			if(nRet_1 <= 0)//굿 트레이에 자재를 놓을 공간이 없다 
			{//트레이를 중 교체를 요청한다 
				st_sync.n_module_uldrbt_tray_req[0] = CTL_REQ;
				st_sync.n_module_uldrbt_tray_req[1] = BIN_GOODBIN;
				RunUldStep = 2110;//2016.0524
			}
			else if(nRet_1 > 0) //굿 트레이에 놓을 공간이 있다 
			{
				RunUldStep = 3000;
			}
			//2016.0524
// 			if(nRet_1 > 0) //굿 트레이에 놓을 공간이 있다 
// 			{
// 				RunUldStep = 3000;
// 			}
		}
		else if(st_sync.n_module_uldrbt_tray_req[0] == CTL_REQ) //트레이 교체중인 상태 
		{
			nRet_1 = Find_Picker_Count(CTL_NO, CTL_NO, BIN_NOTUESD);

			if(nRet_1 > 0 )//굿 버퍼의 빈 공간이 피커의 굿 자재보다 크면 굿 버퍼에 놓는다 
			{
				RunUldStep = 3000;
			}
		}
		break;

	case 2110://2016.0524
		if( st_sync.mn_uld_module_sortertray_supply[1] == CTL_FREE )//교체 완료
		{
			nRet_1 = Find_TrayInfo_Count(UNLOADER_SITE, CTL_NO, -1);// 디바이스가 있으면 CTL_YES, 없으면 CTL_NO 임 
			if(st_sync.n_module_uldrbt_tray_req[0] == CTL_READY)
			{		
				if(nRet_1 > 0) //굿 트레이에 놓을 공간이 있다 
				{
					RunUldStep = 3000;
				}
			}
		}
		break;

	case 2200:
		flag = CTL_NO;
		for (i = 0; i < st_traybuffer[REJECT_SITE].i_loader_row; i++)
		{
			if(st_modulemap.RejectTray[i] == DVC_NO)
			{
				flag = CTL_YES;
			}
		}

		if(flag == CTL_NO) //빈 공간이 없다, 리젝 트레이가 꽉찬 상태 
		{
			RunUldStep = 2300;
		}
		else
		{
			RunUldStep = 3010;
		}				 
		break;

	case 2300:
		// **********************************************************************
		//  Reject Tray 교체 알람.
		// **********************************************************************
		//900503 0 90 "Reject Tray Full Error."
		alarm.n_rejecttrayfull = YES;
		sprintf(mc_alarmcode,"900503");
		st_work.mn_run_status = CTL_dWARNING;
		st_sync.n_RejectTrayChange = CTL_READY;
		CTL_Lib.Alarm_Error_Occurrence(2422, st_work.mn_run_status, mc_alarmcode);
		RunUldStep = 2400;
		break;
		
	case 2400:
		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_REJECT_TRAY, 0);
		}
		// 20140220 
		if(st_sync.n_RejectTrayChange == CTL_YES)
		{
			RunUldStep = 2500;
		}
		else if(st_sync.n_RejectTrayChange == CTL_NO)
		{
			RunUldStep = 2400;
		}
		break;

	case 2500:		
		if(st_sync.n_RejectTrayChange == CTL_YES)
		{
			for (i = 0; i < st_traybuffer[REJECT_SITE].i_loader_row; i++)
			{
				st_modulemap.RejectTray[i] = DVC_NO;
			}
			RunUldStep = 3010;
		}
		else if(st_sync.n_RejectTrayChange == CTL_NO)
		{
			RunUldStep = 2300;
		}
		break;

	case 3000:
		nRet_1 = OnUldPlaceModuleTray(BIN_GOODBIN, 0);
		if(nRet_1 == RET_GOOD)
		{
			nRet_2 = Find_TrayInfo_Count(UNLOADER_SITE, CTL_NO, -1); //리젝트레이 전체의 빈 공간을 확인한다 
			if(nRet_2 == 0)
			{
				//리젝 트레이는 굿 트레이로 교체한다 
				st_sync.n_module_uldrbt_tray_req[0] = CTL_REQ;
				st_sync.n_module_uldrbt_tray_req[1] = BIN_GOODBIN; //로더 트레이 요청 

				st_sync.n_rejectbin_tray_change_flag = CTL_NO; //더이상 리젝 트레이에 놓을 공간이 없으니 리젝 트레이 작업을 완료한다 
			}
			if(st_handler.cwnd_main != NULL)
			{
				//st_work.nMdlPassCount,[0][0] = mn_bcr_goodbin_cnt;
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);
			}


			//2015.0129
			st_work.mn_output_dvc++;
			mn_hs_cont_err = 0;
			RunUldStep = 2000;

		}
		else if(nRet_1 == RET_PICKERNOTFIND)
		{
			nRet_2 = Find_TrayInfo_Count(UNLOADER_SITE, CTL_NO, -1); //리젝트레이 전체의 빈 공간을 확인한다 
			if(nRet_2 == 0)
			{
				//리젝 트레이는 굿 트레이로 교체한다 
				st_sync.n_module_uldrbt_tray_req[0] = CTL_REQ;
				st_sync.n_module_uldrbt_tray_req[1] = BIN_GOODBIN; //로더 트레이 요청 

				st_sync.n_rejectbin_tray_change_flag = CTL_NO; //더이상 리젝 트레이에 놓을 공간이 없으니 리젝 트레이 작업을 완료한다 
			}
			RunUldStep = 2000;
		}
		else if(nRet_1 == RET_NOWHERE)//놓을곳이없다
		{
			nRet_2 = Find_TrayInfo_Count(UNLOADER_SITE, CTL_NO, -1); //리젝트레이 전체의 빈 공간을 확인한다 
			if(nRet_2 == 0)
			{
				//리젝 트레이는 굿 트레이로 교체한다 
				st_sync.n_module_uldrbt_tray_req[0] = CTL_REQ;
				st_sync.n_module_uldrbt_tray_req[1] = BIN_GOODBIN; //로더 트레이 요청 

				st_sync.n_rejectbin_tray_change_flag = CTL_NO; //더이상 리젝 트레이에 놓을 공간이 없으니 리젝 트레이 작업을 완료한다 
			}
			RunUldStep = 2000;
		}
		break;

	case 3010:
		nRet_1 = OnUldPlaceModuleTray(BIN_REJECTBIN, 0);
		if(nRet_1 == RET_GOOD)
		{
			nRet_2 = Find_TrayInfo_Count(REJECT_SITE, CTL_NO, -1); //리젝트레이 전체의 빈 공간을 확인한다 
			if(nRet_2 == 0)
			{
				st_sync.n_rejectbin_tray_change_flag = CTL_NO; //더이상 리젝 트레이에 놓을 공간이 없으니 리젝 트레이 작업을 완료한다 
			}
			if(st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);
			}
			//2015.0129			
			st_work.mn_output_dvc++;

			mn_hs_cont_err++;
			mn_hs_tot_err++;

			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[VISION ERROR]Total HS error = %d, HS Continue error = %d", st_work.mn_output_dvc, mn_hs_cont_err);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}

			if(st_basic.n_mode_use_vis_cont_err == CTL_YES)
			{
				//560901 0 56 "Vision pempin continuoes error."
				//560902 0 56 "Vision pempin total error."
				if( mn_hs_cont_err >= st_basic.n_vis_cont_err)
				{
					st_work.mn_run_status = CTL_dWARNING;
					sprintf(mc_alarmcode,"560901");
					CTL_Lib.Alarm_Error_Occurrence(2423, st_work.mn_run_status, mc_alarmcode);
					RunUldStep = 3020;
					mn_hs_cont_err = 0;
					break;
				}
				else if( mn_hs_tot_err >= st_basic.n_vis_tot_err)
				{
					st_work.mn_run_status = CTL_dWARNING;
					sprintf(mc_alarmcode,"560902");
					mn_hs_tot_err = 0;
					CTL_Lib.Alarm_Error_Occurrence(2423, st_work.mn_run_status, mc_alarmcode);
					RunUldStep = 3020;
					break;
				}
			}
			RunUldStep = 2000;

		}
		else if(nRet_1 == RET_PICKERNOTFIND)
		{
			nRet_2 = Find_TrayInfo_Count(REJECT_SITE, CTL_NO, -1); //리젝트레이 전체의 빈 공간을 확인한다  
			if(nRet_2 == 0 )//리젝 트레이에 자재를 놓을 빈 공간이 없다 
			{
				st_sync.n_rejectbin_tray_change_flag = CTL_NO; //더이상 리젝 트레이에 놓을 공간이 없으니 리젝 트레이 작업을 완료한다 
			}
			RunUldStep = 2000;
		}
		else if(nRet_1 == RET_NOWHERE)//놓을곳이없다
		{
			nRet_2 = Find_TrayInfo_Count(REJECT_SITE, CTL_NO, -1); //리젝트레이 전체의 빈 공간을 확인한다 
			if(nRet_2 == 0)//리젝 트레이에 자재를 놓을 빈 공간이 없다
			{
				st_sync.n_rejectbin_tray_change_flag = CTL_NO; //더이상 리젝 트레이에 놓을 공간이 없으니 리젝 트레이 작업을 완료한다 
			}
			RunUldStep = 2000;
		}
		break;

	case 3020:
			if(st_basic.n_mode_use_vis_cont_err == CTL_YES)
			{
				//560901 0 56 "Vision pempin continuoes error."
				//560902 0 56 "Vision pempin total error."
				if( mn_hs_cont_err >= st_basic.n_vis_cont_err)
				{
					st_work.mn_run_status = CTL_dWARNING;
					sprintf(mc_alarmcode,"560901");
					CTL_Lib.Alarm_Error_Occurrence(2423, st_work.mn_run_status, mc_alarmcode);
					mn_hs_cont_err = 0;
					RunUldStep = 3020;
					break;
				}
				else if( st_work.nMdlRejectCount[0][0] >= st_basic.n_vis_tot_err )
				{
					st_work.mn_run_status = CTL_dWARNING;
					sprintf(mc_alarmcode,"560902");
					CTL_Lib.Alarm_Error_Occurrence(2423, st_work.mn_run_status, mc_alarmcode);
					RunUldStep = 3020;
					break;
				}
			}
			RunUldStep = 2000;
			break;

	case 3100:
		dCurr = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);
		if(dCurr < st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_RBT_UNLOAD_Z].mn_allow)
		{
			RunUldStep = 4000;
		}
		else
		{
			RunUldStep = 3200;
		}
		break;

	case 3200:
		nRet_1 = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunUldStep = 4000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunUldStep = 3100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunUldStep = 3100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2423, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
		
	case 4000:
		//nRet_1 = UldRobot_AutoMove_Safety_Zone(1,0);
		//if(nRet_1 == RET_GOOD)
		//{
		if(	st_work.i_sort_good_cnt + st_work.i_sort_reject_cnt == 1)
		{
			//560800 1 56 "최초 양상된 Lot의 바코드가 올바른지 확인해 주세요."
				sprintf(mc_alarmcode,"560800");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2523, CTL_dWARNING, mc_alarmcode);
		}
		////2014,0411
		st_work.b_Barcode_Create_Btn_Use = FALSE;
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_EQP_LOTEND, 0);
		}
		////
		RunUldStep = 1000;
		//}
		break;
	
	case 10000:
		st_sync.n_lotend_unload_site = CTL_YES;
		Func.InitNode();
		RunUldStep = 0;
		break;
	}

}

void CRun_Rbt_Sorter::Set_PickerUpDown(int nSite, int OnOff, int PickerInfo[2][D_INFOSIZE])
{
	if(nSite == SORTER_SITE)
	{
		if(PickerInfo[0][D_EXIST] == YES)
		{
			FAS_IO.set_out_bit(st_io.o_sorter_picker_rbt_updn_onoff, OnOff);
		}
		else
		{
			if(st_basic.n_mode_use_sorter_picker == 0)
			{
				FAS_IO.set_out_bit(st_io.o_sorter_picker_rbt_updn_onoff, !OnOff);			
			}
		}
		b_PickerFlag[0] = FALSE;
		l_PickerWaitTime[0][0] = GetCurrentTime();
	}
	else//UNLOADER_SITE
	{
		if(PickerInfo[1][D_EXIST] == YES)
		{
			FAS_IO.set_out_bit(st_io.o_uld_picker_rbt_updn_onoff, OnOff);
		}
		else
		{
			FAS_IO.set_out_bit(st_io.o_uld_picker_rbt_updn_onoff, !OnOff);
		}
		b_PickerFlag[1] = FALSE;
		l_PickerWaitTime[1][0] = GetCurrentTime();
	}
}

int CRun_Rbt_Sorter::Get_PickerUpDown(int nSite,int OnOff, int PickerInfo[2][D_INFOSIZE])
{
	int FuncRet = RET_PROCEED;
	int RetVal[2] = {0,};
	char jamcode[10] = {0,};
	CString str;
	 

	RetVal[0] = RetVal[1] = NO;

	if (COMI.mn_simulation_mode == 1) // 20130509
	{
		FuncRet = RET_GOOD;
	}
	else
	{
		if(nSite == SORTER_SITE)
		{
			if(OnOff == IO_ON) //Picker ON (Down이라면..)
			{
				if(PickerInfo[0][D_EXIST] == YES) //동작 시킨 피커면!
				{
					if(b_PickerFlag[0] == FALSE && FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dn_chk, IO_ON) == IO_ON
						&& FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_up_chk, IO_OFF) == IO_OFF)
					{
						b_PickerFlag[0] = TRUE;
						l_PickerWaitTime[0][0] = GetCurrentTime();   
					}
					else if(b_PickerFlag[0] == TRUE && FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dn_chk, IO_ON) == IO_ON
						&& FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_up_chk, IO_OFF) == IO_OFF)
					{
						l_PickerWaitTime[0][1] = GetCurrentTime();  
						l_PickerWaitTime[0][2] = l_PickerWaitTime[0][1] - l_PickerWaitTime[0][0];
						
						if(l_PickerWaitTime[0][2] < 0)
						{
							l_PickerWaitTime[0][0] = GetCurrentTime();
						}
						if(l_PickerWaitTime[0][2] > st_wait.n_on_wait_time[52])
						{
							RetVal[0] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[0][1] = GetCurrentTime();  
						l_PickerWaitTime[0][2] = l_PickerWaitTime[0][1] - l_PickerWaitTime[0][0];
						if(l_PickerWaitTime[0][2] < 0)
						{
							l_PickerWaitTime[0][0] = GetCurrentTime();
						}
						
						if(l_PickerWaitTime[0][2] > st_wait.n_limit_wait_time[52])
						{
							if(st_handler.cwnd_list != NULL)
							{
								str = _T("[UPICKER Error] Sorter Picker down check error.");
								sprintf(st_msg.c_normal_msg,"%s", str);
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
								Func.On_LogFile_Add(0, str);
								Func.On_LogFile_Add(99, str);
							}
	//						550500 0 26 "Sorter Picker up 에러."
	//						550501 0 26 "Sorter Picker down 에러."
							alarm.mstr_pcode = _T("550501");
							sprintf(mc_alarmcode,"550501");
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							alarm.mn_alarm_assign_section = 186;

							FuncRet = RET_ERROR;
						}
					}
				}
				else RetVal[0] = YES;
			}
			else //Picker OFF (UP이라면..)
			{
				if(PickerInfo[0][D_EXIST] == YES) //동작 시킨 피커면!
				{
					if(b_PickerFlag[0] == FALSE && FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dn_chk, IO_OFF) == IO_OFF
						&& FAS_IO.Get_In_Bit(st_io.i_sorter_picker_rbt_up_chk) == IO_ON)
					{
						b_PickerFlag[0] = TRUE;
						l_PickerWaitTime[0][0] = GetCurrentTime();   
					}
					else if(b_PickerFlag[0] == TRUE && FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dn_chk, IO_OFF) == IO_OFF
						&& FAS_IO.Get_In_Bit(st_io.i_sorter_picker_rbt_up_chk) == IO_ON)
					{
						l_PickerWaitTime[0][1] = GetCurrentTime();  
						l_PickerWaitTime[0][2] = l_PickerWaitTime[0][1] - l_PickerWaitTime[0][0];
						
						if(l_PickerWaitTime[0][2] > st_wait.n_on_wait_time[52])
						{
							RetVal[0] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[0][1] = GetCurrentTime();  
						l_PickerWaitTime[0][2] = l_PickerWaitTime[0][1] - l_PickerWaitTime[0][0];
						if(l_PickerWaitTime[0][2] < 0)
						{
							l_PickerWaitTime[0][0] = GetCurrentTime();
						}
						
						if(l_PickerWaitTime[0][2] > st_wait.n_limit_wait_time[52])
						{
							if(st_handler.cwnd_list != NULL)
							{
								str = _T("[UPICKER Error] Sorter Picker up check error.");
								sprintf(st_msg.c_normal_msg,"%s", str);
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
								Func.On_LogFile_Add(0, str);
								Func.On_LogFile_Add(99, str);
							}
	//						550500 0 26 "Sorter Picker up 에러."
	//						550501 0 26 "Sorter Picker down 에러."
							alarm.mstr_pcode = _T("550500"); 
							sprintf(mc_alarmcode,"550500");
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							//st_work.mn_run_status = dWARNING;

							alarm.mn_alarm_assign_section = 187;

							FuncRet = RET_ERROR;
						}
					}
				}
				else RetVal[0] = YES;
			}
			
			if(RetVal[0] == YES) 
			{
				FuncRet = RET_GOOD;
			}
		}
		else
		{
			if(OnOff == IO_ON) //Picker ON (Down이라면..)
			{
				if(PickerInfo[1][D_EXIST] == YES) //동작 시킨 피커면!
				{
					if(b_PickerFlag[1] == FALSE && FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dn_chk, IO_ON) == IO_ON
						&& FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_up_chk, IO_OFF) == IO_OFF)
					{
						b_PickerFlag[1] = TRUE;
						l_PickerWaitTime[1][0] = GetCurrentTime();   
					}
					else if(b_PickerFlag[1] == TRUE && FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dn_chk, IO_ON) == IO_ON
						&& FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_up_chk, IO_OFF) == IO_OFF)
					{
						l_PickerWaitTime[1][1] = GetCurrentTime();  
						l_PickerWaitTime[1][2] = l_PickerWaitTime[1][1] - l_PickerWaitTime[1][0];
						
						if(l_PickerWaitTime[1][2] < 0)
						{
							l_PickerWaitTime[1][0] = GetCurrentTime();
						}
						if(l_PickerWaitTime[1][2] > st_wait.n_on_wait_time[50])
						{
							RetVal[1] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[1][1] = GetCurrentTime();  
						l_PickerWaitTime[1][2] = l_PickerWaitTime[1][1] - l_PickerWaitTime[1][0];
						if(l_PickerWaitTime[1][2] < 0)
						{
							l_PickerWaitTime[1][0] = GetCurrentTime();
						}
						
						if(l_PickerWaitTime[1][2] > st_wait.n_limit_wait_time[50])
						{
							if(st_handler.cwnd_list != NULL)
							{
								str = _T("[UPICKER Error] Unload down sensor check error.");
								sprintf(st_msg.c_normal_msg,"%s", str);
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
								Func.On_LogFile_Add(0, str);
								Func.On_LogFile_Add(99, str);
							}
	//						550600 0 26 "Unload Picker up 에러."
	//						550601 0 26 "Unload Picker down 에러."
							alarm.mstr_pcode = _T("550601"); 
							sprintf(mc_alarmcode,"550601");
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							alarm.mn_alarm_assign_section = 186;

							FuncRet = RET_ERROR;
						}
					}
				}
				else RetVal[1] = YES;
			}
			else //Picker OFF (UP이라면..)
			{
				if(PickerInfo[1][D_EXIST] == YES) //동작 시킨 피커면!
				{
					if(b_PickerFlag[1]== FALSE && FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dn_chk, IO_OFF) == IO_OFF
						&& FAS_IO.Get_In_Bit(st_io.i_uld_picker_rbt_up_chk) == IO_ON)
					{
						b_PickerFlag[1] = TRUE;
						l_PickerWaitTime[1][0] = GetCurrentTime();   
					}
					else if(b_PickerFlag[1] == TRUE && FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dn_chk, IO_OFF) == IO_OFF
						&& FAS_IO.Get_In_Bit(st_io.i_uld_picker_rbt_up_chk) == IO_ON)
					{
						l_PickerWaitTime[1][1] = GetCurrentTime();  
						l_PickerWaitTime[1][2] = l_PickerWaitTime[1][1] - l_PickerWaitTime[1][0];
						
						if(l_PickerWaitTime[1][2] > st_wait.n_on_wait_time[50])
						{
							RetVal[1] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[1][1] = GetCurrentTime();  
						l_PickerWaitTime[1][2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
						if(l_PickerWaitTime[1][2] < 0)
						{
							l_PickerWaitTime[1][0] = GetCurrentTime();
						}
						
						if(l_PickerWaitTime[1][2] > st_wait.n_limit_wait_time[50])
						{
							if(st_handler.cwnd_list != NULL)
							{
								str = _T("[UPICKER Error] Unload up sensor check error.");
								sprintf(st_msg.c_normal_msg,"%s", str);
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
								Func.On_LogFile_Add(0, str);
								Func.On_LogFile_Add(99, str);
							}
	//						550600 0 26 "Unload Picker up 에러."
	//						550601 0 26 "Unload Picker down 에러."
							alarm.mstr_pcode = _T("550600"); 
							sprintf(mc_alarmcode,"550600");
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							//st_work.mn_run_status = dWARNING;

							alarm.mn_alarm_assign_section = 187;

							FuncRet = RET_ERROR;
						}
					}
				}
				else RetVal[1] = YES;
			}	

		}

		if(nSite == SORTER_SITE)
		{
			if(RetVal[0] == YES) 
			{
				FuncRet = RET_GOOD;
			}
		}
		else
		{
			if(RetVal[1] == YES) 
			{
				FuncRet = RET_GOOD;
			}
		}
	}

	return FuncRet;
}

void CRun_Rbt_Sorter::Set_FingerOnOff(int nSite, int OnOff, int FingerInfo[2][D_INFOSIZE])
{
//	int i;

//	int nPortNum[PICKER_NUM];
	//int iPickerData;

// ******************************************************************************
// 다중 I/O 동시 출력 함수 [최대 8 PORT 동시 제어]                               
// -> int nSite     : 출력할 I/O PORT 부위                                       
//  : 차후에 I/O PORT 출력 제약 조건 검사 시 필요하다                            
// -> int nModuleNo : 출력할 I/O PORT 존재 모듈 번호                             
// -> int nOffset   : I/O BIT TYPE                                               
//  : TYPE_LBYTE [0x2] : 16 BIT 중 상위 8BIT 읽어 리턴한다                       
//  : TYPE_HBYTE [0x3] : 16 BIT 중 하위 8BIT 읽어 리턴한다                       
// -> int *npBit    : 상태 변경할 I/O PORT 번호                                  
// -> int *npOnOff  : 출력 I/O PORT 상태 [0:OFF    1:ON]                         
// -> int nBitCount : 상태 변경할 I/O PORT 갯수                                  
//  : [npBit/npOnOff] 배열 크리보다는 작던지 같은 값일 것이다                    
// ******************************************************************************
		//nPortNum[i] = i + 8;

//	if(FingerInfo[D_EXIST] == YES)	//동작 시킬 피커를 재확인
//	{
//		iPickerData = YES;
//	}
//	else
//	{
//		iPickerData = NO;
//	}

	if(nSite == SORTER_SITE)
	{
		FAS_IO.set_out_bit(st_io.o_sorter_picker_rbt_gripper_onoff, OnOff);
		b_FingerFlag[0] = FALSE;
		l_FingerWaitTime[0][0] = GetCurrentTime();   
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_uld_picker_rbt_gripper_onoff, OnOff);
		b_FingerFlag[1] = FALSE;
		l_FingerWaitTime[1][0] = GetCurrentTime();   
	}


}

int CRun_Rbt_Sorter::Get_FingerOnOff(int nSite, int OnOff, int FingerInfo[2][D_INFOSIZE])
{
	int FuncRet;
	int RetVal[2] = {0,};
	 
	FuncRet = RET_PROCEED;

//	if(st_basic.n_mode_device != CTL_YES)
//	{
//		return RET_GOOD;
//	}
//
	if (COMI.mn_simulation_mode == 1) // 20130509
	{
		return RET_GOOD;
	}
	else
	{
		RetVal[0] = RetVal[1] = NO;
		if(nSite == SORTER_SITE)
		{

			if(OnOff == IO_ON) //Finger ON (Down이라면..)
			{
				if(FingerInfo[0][D_EXIST] == YES) //동작 시킨 피커면!
				{
					if(b_FingerFlag[0] == FALSE && FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dvc_chk, IO_OFF) == IO_OFF)
					{
						b_FingerFlag[0] = TRUE;
						l_FingerWaitTime[0][0] = GetCurrentTime();   
					}
					else if(b_FingerFlag[0] == TRUE && FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dvc_chk, IO_OFF) == IO_OFF)
					{
						l_FingerWaitTime[0][1] = GetCurrentTime();  
						l_FingerWaitTime[0][2] = l_FingerWaitTime[0][1] - l_FingerWaitTime[0][0];
						
						if(l_FingerWaitTime[0][2] > st_wait.n_on_wait_time[53])
						{
							RetVal[0] = YES;
						}
					}
					else
					{
						l_FingerWaitTime[0][1] = GetCurrentTime();  
						l_FingerWaitTime[0][2] = l_FingerWaitTime[0][1] - l_FingerWaitTime[0][0];
						
						if(l_FingerWaitTime[0][2] > st_wait.n_limit_wait_time[53])
						{
							if(st_handler.cwnd_list != NULL)
							{
								sprintf(st_msg.c_abnormal_msg,"[Finger Error] Sorter Finger on sensor check error.");
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
								//Func.On_LogFile_Add(0, st_other.str_abnormal_msg);
								//Func.On_LogFile_Add(99, st_other.str_abnormal_msg);
							}
	//						550502 1 55 "Sorter Finger On Check Error."
	//						550503 1 55 "Sorter Finger Off Check Error."
							alarm.mstr_pcode = _T("550502");
							sprintf(mc_alarmcode,"550502");
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							//st_work.mn_run_status = dWARNING;

							alarm.mn_alarm_assign_section = 188;

							FuncRet = RET_ERROR;
						}
					}
				}
				else RetVal[0] = YES;
			}
			else
			{
				if(FingerInfo[0][D_EXIST] == YES) //동작 시킨 피커면!
				{
					if(b_FingerFlag[0] == FALSE && FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dvc_chk, IO_OFF) == IO_OFF)
					{
						b_FingerFlag[0] = TRUE;
						l_FingerWaitTime[0][0] = GetCurrentTime();   
					}
					if(b_FingerFlag[0] == FALSE && FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dvc_chk, IO_ON) == IO_ON)
					{
						b_FingerFlag[0] = TRUE;
						l_FingerWaitTime[0][0] = GetCurrentTime();   
					}
					else if(b_FingerFlag[0] == TRUE && FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dvc_chk, IO_OFF) == IO_OFF)
					{
						l_FingerWaitTime[0][1] = GetCurrentTime();  
						l_FingerWaitTime[0][2] = l_FingerWaitTime[0][1] - l_FingerWaitTime[0][0];
						
						if(l_FingerWaitTime[0][2] > st_wait.n_on_wait_time[53])
						{
							RetVal[0] = YES;
						}
					}
					else if(b_FingerFlag[0] == TRUE && FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dvc_chk, IO_ON) == IO_ON)
					{
						l_FingerWaitTime[0][1] = GetCurrentTime();  
						l_FingerWaitTime[0][2] = l_FingerWaitTime[0][1] - l_FingerWaitTime[0][0];
						
						if(l_FingerWaitTime[0][2] > st_wait.n_on_wait_time[53])
						{
							RetVal[0] = YES;
						}
					}
					else
					{
						l_FingerWaitTime[0][1] = GetCurrentTime();  
						l_FingerWaitTime[0][2] = l_FingerWaitTime[0][1] - l_FingerWaitTime[0][0];
						
						if(l_FingerWaitTime[0][2] > st_wait.n_limit_wait_time[53])
						{
							if(st_handler.cwnd_list != NULL)
							{
								sprintf(st_msg.c_abnormal_msg,"[Finger Error] Sorter Finger off sensor check error.");
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
								//Func.On_LogFile_Add(0, st_other.str_abnormal_msg);
								//Func.On_LogFile_Add(99, st_other.str_abnormal_msg);
							}
	//						550502 1 55 "Sorter Finger On Check Error."
	//						550503 1 55 "Sorter Finger Off Check Error."
							alarm.mstr_pcode = _T("550503");
							sprintf(mc_alarmcode,"550503");
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							//st_work.mn_run_status = dWARNING;

							alarm.mn_alarm_assign_section = 189;

							FuncRet = RET_ERROR;
						}
					}
				}
				else RetVal[0] = YES;
			}
		}
		else
		{
			if(OnOff == IO_ON) //Finger ON (Down이라면..)
			{
				if(FingerInfo[1][D_EXIST] == YES) //동작 시킨 피커면!
				{
					if(b_FingerFlag[1] == FALSE && FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dvc_chk, IO_OFF) == IO_OFF)
					{
						b_FingerFlag[1] = TRUE;
						l_FingerWaitTime[1][0] = GetCurrentTime();   
					}
					else if(b_FingerFlag[1] == TRUE && FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dvc_chk, IO_OFF) == IO_OFF)
					{
						l_FingerWaitTime[1][1] = GetCurrentTime();  
						l_FingerWaitTime[1][2] = l_FingerWaitTime[1][1] - l_FingerWaitTime[1][0];
						
						if(l_FingerWaitTime[1][2] > st_wait.n_on_wait_time[51])
						{
							RetVal[1] = YES;
						}
					}
					else
					{
						l_FingerWaitTime[1][1] = GetCurrentTime();  
						l_FingerWaitTime[1][2] = l_FingerWaitTime[1][1] - l_FingerWaitTime[1][0];
						
						if(l_FingerWaitTime[1][2] > st_wait.n_limit_wait_time[51])
						{
							if(st_handler.cwnd_list != NULL)
							{
								sprintf(st_msg.c_abnormal_msg,"[Finger Error] Unload Finger on sensor check error.");
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
								//Func.On_LogFile_Add(0, st_other.str_abnormal_msg);
								//Func.On_LogFile_Add(99, st_other.str_abnormal_msg);
							}
	//						550602 1 55 "Sorter Finger On Check Error."
	//						550603 1 55 "Sorter Finger Off Check Error."
							alarm.mstr_pcode = _T("550602");
							sprintf(mc_alarmcode,"550602");
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							alarm.mn_alarm_assign_section = 188;
							FuncRet = RET_ERROR;
						}
					}
				}
				else RetVal[1] = YES;
			}
			else
			{
				if(FingerInfo[1][D_EXIST] == YES) //동작 시킨 피커면!
				{
					if(b_FingerFlag[1] == FALSE && FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dvc_chk, IO_OFF) == IO_OFF)
					{
						b_FingerFlag[1] = TRUE;
						l_FingerWaitTime[1][0] = GetCurrentTime();   
					}
					if(b_FingerFlag[1] == FALSE && FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dvc_chk, IO_ON) == IO_ON)
					{
						b_FingerFlag[1] = TRUE;
						l_FingerWaitTime[1][0] = GetCurrentTime();   
					}
					else if(b_FingerFlag[1] == TRUE && FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dvc_chk, IO_OFF) == IO_OFF)
					{
						l_FingerWaitTime[1][1] = GetCurrentTime();  
						l_FingerWaitTime[1][2] = l_FingerWaitTime[1][1] - l_FingerWaitTime[1][0];
						
						if(l_FingerWaitTime[1][2] > st_wait.n_on_wait_time[51])
						{
							RetVal[1] = YES;
						}
					}
					else if(b_FingerFlag[1] == TRUE && FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dvc_chk, IO_ON) == IO_ON)
					{
						l_FingerWaitTime[1][1] = GetCurrentTime();  
						l_FingerWaitTime[1][2] = l_FingerWaitTime[1][1] - l_FingerWaitTime[1][0];
						
						if(l_FingerWaitTime[1][2] > st_wait.n_on_wait_time[51])
						{
							RetVal[1] = YES;
						}
					}
					else
					{
						l_FingerWaitTime[1][1] = GetCurrentTime();  
						l_FingerWaitTime[1][2] = l_FingerWaitTime[1][1] - l_FingerWaitTime[1][0];
						
						if(l_FingerWaitTime[1][2] > st_wait.n_limit_wait_time[51])
						{
							if(st_handler.cwnd_list != NULL)
							{
								sprintf(st_msg.c_abnormal_msg,"[Finger Error] Unload Finger off sensor check error.");
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
								//Func.On_LogFile_Add(0, st_other.str_abnormal_msg);
								//Func.On_LogFile_Add(99, st_other.str_abnormal_msg);
							}
	//						550602 1 55 "Sorter Finger On Check Error."
	//						550603 1 55 "Sorter Finger Off Check Error."
							alarm.mstr_pcode = _T("550603");
							sprintf(mc_alarmcode,"550603");
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							alarm.mn_alarm_assign_section = 189;
							FuncRet = RET_ERROR;
						}
					}
				}
				else RetVal[1] = YES;
			}
		}
		
		if(nSite == SORTER_SITE)
		{
			if(RetVal[0]) 
			{
				FuncRet = RET_GOOD;
			}
		}
		else
		{
			if(RetVal[1]) 
			{
				FuncRet = RET_GOOD;
			}
		}
	}

	return FuncRet;
}


int CRun_Rbt_Sorter::SorterRobot_AutoMove_Safety_Zone(int n_mode, int n_flag)
{
	int Ret=0, Ret_1=0, FuncRet = RET_PROCEED, i=0;
	double dCurrentPos =0;
	double dSpeedRatio[3] = {st_basic.nRunSpeed,st_basic.nRunSpeed,st_basic.nRunSpeed};

	//n_mode == 0 => 로보트가 BCR로 들어 가는 동작 할 예정 
	//n_mode == 1 => 로보트가 BCR이외의 작업을 동작 할 예정

	//n_flag == Not Used

	switch(SafetyZone_Step)
	{
	case 0:
		l_until_wait_time[0][0] = GetCurrentTime(); 
		SafetyZone_Step = 1000;
		break;

	case 1000:
		dCurrentPos = COMI.Get_MotCurrentPos(M_RBT_SORTER_X);
		if(dCurrentPos > st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY] + st_motor[M_RBT_SORTER_X].mn_allow)	 
		{ //위험 위치에 있는 상태 
			if(n_mode == 0)  //로보트가 BCR로 들어 가는 동작 할 예정
			{
				SafetyZone_Step = 2000;	//이미 들어가 있는 상태로 어느 위치에 있는지 확인한다 
			}
			else //if(n_mode == 1) //로보트가 BCR 이외의 작업을 동작 할 예정
			{
				SafetyZone_Step = 3000;	//X축을 BCR를 빠져 나가니 안전 위치까지 이동 시킨다 
			} 
		}
		else
		{//X축이 BCR를 벗어나 안전 위치에 있는 상태이다   
			if(n_mode == 0)  //로보트가 BCR로 들어 가는 동작 할 예정
			{
				SafetyZone_Step = 4000;	//X축을 BCR 대기 위치까지 이동 시킨다 
			}
			else //if(n_mode == 1) //로보트가 BCR이외의 작업을 동작 할 예정
			{
				SafetyZone_Step = 5000; //이미 X축이 안전 위치에 있다 
			} 
		}
		break;

	//////////////////////////////////////////////////////////////////
	//이미 Test site 안에 있는 상태로 어느 PH사이트에 있는지 확인한다 
	//////////////////////////////////////////////////////////////////
	case 2000: 
		dCurrentPos = COMI.Get_MotCurrentPos(M_RBT_SORTER_X);
		if(dCurrentPos >= st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY] + st_motor[M_RBT_SORTER_X].mn_allow)
		{
			SafetyZone_Step = 5000;//현재 목표로 가야할 위치에 들어와 있는 상태로 이미 목표 위치에 도착에 있다 
		}
		else
		{
			SafetyZone_Step = 2100;//Test site 안에 있지만 현재 위치는 목표 위치와 틀리다, 목표 위치를 찾아 가자 
		}
		break;

	///////////////////////////////////////////////////////////////////
	//이동할 사이트가 아니고 위험 우치에 있으면 , 빠진 후 가야할 위치의 안전 위치까지 간다 
	///////////////////////////////////////////////////////////////////
	case 2100://Y축을 안전 위치로 이동 시킨다 
		Ret = CTL_Lib.Single_Move(M_RBT_SORTER_X, st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY], st_basic.nRunSpeed);
		if(Ret == BD_GOOD)
		{			
			SafetyZone_Step = 2200;
		}
		else if(Ret == BD_RETRY)
		{
			SafetyZone_Step = 2100;
		}
		else if(Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2430, st_work.mn_run_status, COMI.mc_alarmcode);
			SafetyZone_Step = 2100;
		}
		break;


	case 2200: //X축을 이동 하려는 사이트로 이동 시킨다 
		///////////////////////// 20121114
		dCurrentPos = COMI.Get_MotCurrentPos(M_VISION1_Y);
//		if(dCurrentPos > st_motor[M_VISION1_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION1_Y].mn_allow)
		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 0)
		{
		/////////////////////////
			Ret = CTL_Lib.Single_Move(M_RBT_SORTER_Y, st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER], st_basic.nRunSpeed);
			
			if(Ret == BD_GOOD)
			{			
				SafetyZone_Step = 5000;
			}
			else if(Ret == BD_RETRY)
			{
				SafetyZone_Step = 2200;
			}
			else if(Ret == BD_ERROR || Ret == BD_SAFETY)
			{
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2431, st_work.mn_run_status, COMI.mc_alarmcode);
				SafetyZone_Step = 2200;
			}
		///////////////////////// 20121114	
		}
		else// if(dCurrentPos > st_motor[M_VISION2_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION2_Y].mn_allow)
		{
			Ret = CTL_Lib.Single_Move(M_RBT_SORTER_Y, st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER2 - 4], st_basic.nRunSpeed);

			if(Ret == BD_GOOD)
			{			
				SafetyZone_Step = 5000;
			}
			else if(Ret == BD_RETRY)
			{
				SafetyZone_Step = 2200;
			}
			else if(Ret == BD_ERROR || Ret == BD_SAFETY)
			{
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2431, st_work.mn_run_status, COMI.mc_alarmcode);
				SafetyZone_Step = 2200;
			}		
		}
		/////////////////////////
		break;

	//////////////////////////////////////////////////////////////////
	//BCR 위험 영역안에서 BCR사이트 밖으로 나가는 루틴
	//////////////////////////////////////////////////////////////////
	case 3000: //안전 위치로 X축을 이동 시킨다 
		Ret = CTL_Lib.Single_Move(M_RBT_SORTER_X, st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY], st_basic.nRunSpeed);		
		if(Ret == BD_GOOD)
		{			
			SafetyZone_Step = 5000;
		}
		else if(Ret == BD_RETRY)
		{
			SafetyZone_Step = 3000;
		}
		else if(Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2432, st_work.mn_run_status, COMI.mc_alarmcode);
			SafetyZone_Step = 3000;
		}
		break;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//이미 안전 위치에 있으니 목표하는 해당 BCR 사이트 대기 위치까지 바로 들어가 작업 할 수 있도록 안전 위치까지 이동한다  
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 4000:
		///////////////////////// 20121114
		dCurrentPos = COMI.Get_MotCurrentPos(M_VISION1_Y);
//		if(dCurrentPos > st_motor[M_VISION1_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION1_Y].mn_allow)
		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 0)
		{
		/////////////////////////	
			dAbsPos[0] = st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY];
			dAbsPos[1] = st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER];
						
				 
			Ret = CTL_Lib.Linear_Move(SORTER_MOT_MAP, 2, msp_AxisNum, dAbsPos, dSpeedRatio);
			if(Ret == BD_GOOD)
			{			
				SafetyZone_Step = 4100;
			}
			else if(Ret == BD_RETRY)
			{
				SafetyZone_Step = 4000;
			}
			else if (Ret == BD_ERROR || Ret == BD_SAFETY)
			{
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2433, CTL_dWARNING, COMI.mc_alarmcode);
				SafetyZone_Step = 4000;
			}
		///////////////////////// 20121114
		}
		else// if(dCurrentPos > st_motor[M_VISION2_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION2_Y].mn_allow)
		{
			dAbsPos[0] = st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY];
			dAbsPos[1] = st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER2 - 4];
			
			
			Ret = CTL_Lib.Linear_Move(SORTER_MOT_MAP, 2, msp_AxisNum, dAbsPos, dSpeedRatio);
			if(Ret == BD_GOOD)
			{			
				SafetyZone_Step = 4100;
			}
			else if(Ret == BD_RETRY)
			{
				SafetyZone_Step = 4000;
			}
			else if (Ret == BD_ERROR || Ret == BD_SAFETY)
			{
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2433, CTL_dWARNING, COMI.mc_alarmcode);
				SafetyZone_Step = 4000;
			}
		}
		/////////////////////////
		break;

	case 4100: // 동작완료 및 위치 체크 
		l_until_wait_time[0][0] = GetCurrentTime();
		SafetyZone_Step = 5000;
		break; 

	/////////////////////////////////////////////////////
	// PH 안전위치 또는 PH 작업 영역에 존재 할 수 있다 //
	/////////////////////////////////////////////////////
	case 5000: 
		SafetyZone_Step = 0;
		FuncRet = RET_GOOD;
		break;
	}

	return FuncRet;
}


int CRun_Rbt_Sorter::OnPickupModule(int n_pos, int n_flag)
{
	int Ret = 0, Ret1 = 0, y=0, FuncRet, ActionFlag = NO;
	double dSpeedRatio[3] = {st_basic.nRunSpeed,st_basic.nRunSpeed,st_basic.nRunSpeed};

	FuncRet = RET_PROCEED;

	Func.ThreadFunctionStepTrace(26, Module_Pick_Step);
	switch(Module_Pick_Step)
	{
	case 0:
		if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][n_pos][1] != CTL_READY) //아직 트레이가 준비되지 않았다 
		{
			if(st_modulemap.nSortRobotPicker[D_EXIST] == DVC_YES)
			{
				break;
			}
			RunSortStep = 1500;
			break;
		}

 		i_DVCSortRepickCnt = 0;

		l_until_wait_time[0][0] = GetCurrentTime();
		Module_Pick_Step = 1000;
		break;

	case 10: //X축을 안전 위치 이상으로 빼줌 
		Ret = SorterRobot_AutoMove_Safety_Zone(1, 0);
		if(Ret == RET_GOOD)
		{
			Module_Pick_Step = 20;
		}
		break;

	case 20:
		dAbsPos[0] = st_motor[M_RBT_SORTER_X].md_pos[X_SORT_WORK_PICK1 + n_pos];
		dAbsPos[1] = st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_WORK_PICK1 + n_pos];
		Ret = COMI.Start_LinearMove(SORTER_MOT_MAP, dAbsPos, dSpeedRatio);
		if(Ret == CTLBD_RET_GOOD)
		{
			Module_Pick_Step = 21;
		}
		else if(Ret == CTLBD_RET_RETRY)
		{
			Module_Pick_Step = 20;
		}		
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			Module_Pick_Step = 20;
		}
		break;

	case 21:
		Ret = COMI.Check_LinearMove(SORTER_MOT_MAP, dAbsPos);

		if(Ret == CTLBD_RET_GOOD)
		{
			Module_Pick_Step = 30;			
		}
		else if(Ret == CTLBD_RET_RETRY)
		{
			Module_Pick_Step = 20;
		}
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			Module_Pick_Step = 20;
		}
		break;

	case 30://트레이가 준비 되기를 기다린다 
		if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][n_pos][1] != CTL_READY)
		{
			Module_Pick_Step = 0;
		}
		break;

	case 1000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(md_CurrentPos > st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			Module_Pick_Step = 1100; //Z축을 안전 위치로 든다 
		}
		else
		{
			Module_Pick_Step = 2000;
		}
		break; 

	case 1100:
 		Ret = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP], st_basic.nRunSpeed);
		
		if(Ret == BD_GOOD)
		{
			l_until_wait_time[0][0] = GetCurrentTime();
			Module_Pick_Step = 2000;
		}
		else if(Ret == BD_RETRY)
		{
			Module_Pick_Step = 1100;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			Module_Pick_Step = 1100;
		}
		break;

	case 2000:
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		Ret = Func.PickerSorterStatusCheck(PICKERSTATUS, st_modulemap.nSortRobotPicker, mResult);
		
		if(Ret == RET_GOOD)
		{
			Module_Pick_Step = 3000;
		}
		else if(Ret == RET_ERROR)	// 피커 상태 에러 
		{	
			if(l_until_wait_time[0][2] < 0)
			{
				l_until_wait_time[0][0] = GetCurrentTime();
				break;
			}
			if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME*5))
			{
				if(l_until_wait_time[0][2] < 0) l_until_wait_time[0][0] = GetCurrentTime();
				break;
			}

			st_work.mn_run_status = dWARNING;
		}
		break;

	//로보트가 이동할 위치를 찾는다 
	case 3000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(md_CurrentPos > st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			Module_Pick_Step = 3110; //Z축을 안전 위치로 든다 
		}
		else
		{
			Module_Pick_Step = 3200;
		}
		break; 

	case 3110:
 		Ret = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP], st_basic.nRunSpeed);
		
		if(Ret == BD_GOOD)
		{
			Module_Pick_Step = 3200;
		}
		else if(Ret == BD_RETRY)
		{
			Module_Pick_Step = 3110;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2440, st_work.mn_run_status, COMI.mc_alarmcode);
			Module_Pick_Step = 3110;
		}
		break; 

	case 3200:
		Ret = SorterRobot_AutoMove_Safety_Zone(1, 0); //해당 사이트로 이동하여 X,Y 로보트가 동작 가능하게 한다 
		if(Ret == RET_GOOD)
		{
			Module_Pick_Step = 3300;
		}
		break;

	//로보트를 작업 할 위치로 이동한다 
	case 3300:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(md_CurrentPos > st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			Module_Pick_Step = 3000;
		}
		else
		{
			Module_Pick_Step = 3400;
		}
		break;

	case 3400:
		md_SortAbsPos[0] = st_motor[M_RBT_SORTER_X].md_pos[X_SORT_WORK_PICK1 + n_pos];
		md_SortAbsPos[1] = st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_WORK_PICK1 + n_pos];

		Ret = CTL_Lib.Linear_Move(SORTER_MOT_MAP, 2, msp_AxisNum, md_SortAbsPos, dSpeedRatio);
		if(Ret == BD_GOOD)
		{			
			l_until_wait_time[0][0] = GetCurrentTime();
			Module_Pick_Step = 3500;
		}
		else if(Ret == BD_RETRY)
		{
			Module_Pick_Step = 3300;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2441, CTL_dWARNING, COMI.mc_alarmcode);
			Module_Pick_Step = 3300;
		}
		break;

	
	case 3500: // 가능한 피커를 내리고 피킹하는 동작 
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		Ret = Func.PickerSorterStatusCheck(PICKERSTATUS, st_modulemap.nSortRobotPicker, mResult); // 피커 정보와 실제 센서 상태를 체크 
		
		if(Ret == RET_ERROR)
		{
			if(l_until_wait_time[0][2] < 0)
			{
				l_until_wait_time[0][0] = GetCurrentTime();
				break;
			}
			if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME*5)) //자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[0][2] < 0) l_until_wait_time[0][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				break;
			}
			st_work.mn_run_status = dWARNING;
			break;
		}
		Module_Pick_Step = 4000;		
		break;

	case 4000: // moving after finger open
		ActionPicker[0][D_EXIST] = YES;
		Set_FingerOnOff(SORTER_SITE, IO_ON, ActionPicker);
		Module_Pick_Step = 4010;
		break;

	case 4010:
		Ret = Get_FingerOnOff(SORTER_SITE, IO_ON, ActionPicker);

		if(Ret == RET_GOOD)
		{
			Module_Pick_Step = 4100;
		}
		else if(Ret != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2493, st_work.mn_run_status, mc_alarmcode);
			Module_Pick_Step = 4000;
		}
		break;		

	case 4100:	//피커 및 Z axis down action
		Ret = Robot_CurrentPos_Check(SORTER_SITE, md_SortAbsPos[0], md_SortAbsPos[1]);
		if(Ret == CTL_YES) //현재 로보트 위치가 목표 우치가 아니다
		{
			Module_Pick_Step = 0; //다시 처음 부터 시작 한다 
		}
		else
		{
			Module_Pick_Step = 4110;
		}
		break;

	case 4110:
		Set_PickerUpDown(SORTER_SITE, IO_ON, ActionPicker);

 		Ret = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_BUFFER_PLACE1 + n_pos], st_basic.nRunSpeed);
		
		if(Ret == BD_GOOD)
		{
			Module_Pick_Step = 4200;
		}
		else if(Ret == BD_RETRY)
		{
			Module_Pick_Step = 4110;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2442, st_work.mn_run_status, COMI.mc_alarmcode);
			Module_Pick_Step = 4110;
		}
		break;


	case 4200: //picker down action check
		Ret = Get_PickerUpDown(SORTER_SITE, IO_ON, ActionPicker);

		if(Ret == RET_GOOD)
		{
			Module_Pick_Step = 4300;
		}
		else if(Ret == RET_ERROR)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2443, st_work.mn_run_status, mc_alarmcode);
			Module_Pick_Step = 4100;
		}
		break;

	case 4300: //Pick 디바이스 클램프 
		Set_FingerOnOff(SORTER_SITE, IO_OFF, ActionPicker);
		Module_Pick_Step = 4310;
		break;

	case 4310:
		Ret = Get_FingerOnOff(SORTER_SITE,IO_OFF, ActionPicker);
		if(Ret == RET_GOOD)
		{
			l_until_wait_time[0][0] = GetCurrentTime();
			Module_Pick_Step = 4400;
		}
		else if(Ret != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2494, st_work.mn_run_status, mc_alarmcode);
			Module_Pick_Step = 4300;
		}
		break;	

	case 4400: //디바이스 존재유무 체크 
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		TempPicker[D_EXIST] = ActionPicker[0][D_EXIST];
		Ret = Func.PickerSorterStatusCheck(IO_ON, TempPicker, mResult); // 피커 정보와 실제 센서 상태를 체크 

		if(Ret == RET_GOOD)
		{
			Module_Pick_Step = 4500;	
		}
		else if(Ret == RET_ERROR)
		{
			if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME*5)) //자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[0][2] < 0)	l_until_wait_time[0][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				break;
			}
			mTempPicker[D_EXIST] = NO;

			if(TempPicker[D_EXIST] == YES && mResult[0] == NO)
			{	
				//문제 발생시 에러시 해당 포트만 오픈 시키자 
				mTempPicker[D_EXIST] = YES;
			}
 			Module_Pick_Step = 4500;	 // 현재 핑거 체크한는데 문제가 있어 벌리면 안된다 
		}
		break;
 
	case 4500: // 모터 업 and picker up
		if(i_DVCSortRepickCnt > 0)
		{
			Module_Pick_Step = 4600;
		}
		else
		{
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][n_pos][1] = CTL_DOREADY;
			Module_Pick_Step = 4600;
		}
		break;

	case 4600:
		if(i_DVCSortRepickCnt > 0)
		{
			Module_Pick_Step = 4700;
		}
		else if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][n_pos][1] == CTL_POSSIBLE)
		{
			Module_Pick_Step = 4700;
		}
		break;

	case 4700:
		//2012,0930
//		Set_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);		
 		Ret = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP], st_basic.nRunSpeed);
		if(Ret == BD_GOOD)
		{
			Module_Pick_Step = 4800;
		}
		else if(Ret == BD_RETRY)
		{
			Module_Pick_Step = 4700;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2443, st_work.mn_run_status, COMI.mc_alarmcode);
			Module_Pick_Step = 4700;
		}
		break;

	case 4800:// motor up M_RBT_SORTER_Z
		//2012,0930
		l_until_wait_time[0][0] = GetCurrentTime();
		Module_Pick_Step = 5000;
		break;

		Ret = Get_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);

		if(Ret == RET_GOOD)
		{
			l_until_wait_time[0][0] = GetCurrentTime();
			Module_Pick_Step = 5000;
		}
		else if(Ret == RET_ERROR)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2444, st_work.mn_run_status, mc_alarmcode);
			Module_Pick_Step = 4700;
		}
		break;

	//////////////////////////////////////////////////////////
	// 작업한 피커 및 트레이 정보를 셋팅한다 
	//////////////////////////////////////////////////////////
	case 5000:
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		
		Ret = Func.PickerSorterStatusCheck(IO_ON, ActionPicker[0], mResult); // 피커 정보와 실제 센서 상태를 체크 
//		mResult = YES;
		
		if(Ret == RET_ERROR)
		{
			if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME*2))
			{
				if(l_until_wait_time[0][2] < 0)		l_until_wait_time[0][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				break;
			}
		}
		
		if(ActionPicker[0][D_EXIST] == YES && mResult[0] == NO)
		{	
			//문제 발생시 에러시 해당 포트만 오픈 시키자 
			mTempPicker[D_EXIST] = YES;
		}
		if(ActionPicker[0][D_EXIST] == YES && mResult[0] == YES) // 디바이스가 있는 피커만 체크
		{
	
			st_modulemap.WorkBuffer[D_EXIST][n_pos][LDMODULE_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][n_pos][LEFTSINK_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][n_pos][RIGHTSINK_SITE] = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][n_pos][LDCLIP_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][n_pos][SORTER_SITE]	 = DVC_NO;

			st_sync.n_module_site_status[LDMODULE_SITE][n_pos][0] 	 = DVC_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][n_pos][0] 	 = DVC_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][n_pos][0] 	 = DVC_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][n_pos][0] 	 = DVC_NO;
			st_sync.n_module_site_status[SORTER_SITE][n_pos][0] 	 = DVC_NO;

			st_sync.n_module_site_status[LDMODULE_SITE][n_pos][1] 	 = DVC_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][n_pos][1] 	 = DVC_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][n_pos][1] 	 = DVC_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][n_pos][1] 	 = DVC_NO;
			st_sync.n_module_site_status[SORTER_SITE][n_pos][1] 	 = DVC_NO;

			st_modulemap.nSortRobotPicker[D_EXIST] = DVC_YES;
			st_modulemap.nSortRobotPicker[D_BIN] = BIN_LDBUFFERBIN;		// BIN_LOADTRAYBIN
			st_modulemap.nSortRobotPicker[D_RE_CNT] = T_NOTTEST;			// Test Count Reset
			st_modulemap.nSortRobotPicker[D_SCR_BIN] = BIN_CLEAR;			// Screen Bin			

			ActionPicker[0][D_EXIST] = NO;  
			mResult[0] = NO;

			st_sync.n_module_site_status[LDMODULE_SITE][n_pos][0] = DVC_NO;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][n_pos][1] = CTL_CHANGE;
		}

		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_SORTER_PICKER);			// 피커 정보 
		}

		if(Ret == RET_GOOD)
		{
			Module_Pick_Step = 6000;
		}
		else if(Ret == RET_ERROR)
		{
			i_DVCSortRepickCnt++;

			if(i_DVCSortRepickCnt >= 3)
			{ 
				i_DVCSortRepickCnt = 1;		//피킹 재동작의 카운트는 초기화 되는게 맞음!

			
					if(ActionPicker[0][D_EXIST] == YES && mResult == NO) // 동작했는데 디바이스가 없는 피커만 체크
					{
						if(mTempPicker[D_EXIST] == YES)
						{
							st_modulemap.WorkBuffer[D_EXIST][n_pos][LDMODULE_SITE]	 = DVC_NO;
							st_modulemap.WorkBuffer[D_EXIST][n_pos][LEFTSINK_SITE]	 = DVC_NO;
							st_modulemap.WorkBuffer[D_EXIST][n_pos][RIGHTSINK_SITE] = DVC_NO;
							st_modulemap.WorkBuffer[D_EXIST][n_pos][LDCLIP_SITE]	 = DVC_NO;
							st_modulemap.WorkBuffer[D_EXIST][n_pos][SORTER_SITE]	 = DVC_NO;
						}
					}
				
				if(st_handler.cwnd_main != NULL)
				{
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BUFFER_INFO, SORTER_SITE);  //로더 정보 
				}
				
				Module_Pick_Step = 1000;
			}
			else 
			{
				Module_Pick_Step = 1000;
			}
		}
		break;

	case 6000: //정상적으로 모두 잡았으면 
		//자재를 한번이라도 집은 뒤부터 loader off가 되도록 하자.. (로더/언로더 카세트 및 관련 동작 flow를 맞추기 위함)
		if(st_work.i_robot_device_loading != TRUE)
		{
			st_work.i_robot_device_loading = TRUE;
		}
		Module_Pick_Step = 0;
		FuncRet = RET_GOOD;			
		break;
	}

	return FuncRet;
}

int CRun_Rbt_Sorter::OnPlaceModule(int n_Site,double d_XPos, double d_YPos, int n_flag)
{
	int Ret = 0, Ret_1=0, Ret_2=0, y=0, FuncRet, ActionFlag = NO;
	//int nFirstPicker_X_Pos = -1, nFirstTray_X_Pos = -1;
	int nRet_1;
	double dSpeedRatio[3] = {st_basic.nRunSpeed,st_basic.nRunSpeed,st_basic.nRunSpeed};

	FuncRet = RET_PROCEED;

	Func.ThreadFunctionStepTrace(27, Module_Place_Step);
	switch(Module_Place_Step)
	{
	case 0:
 		i_DVCSortReplaceCnt = 0;
		l_until_wait_time[0][0] = GetCurrentTime();
		Module_Place_Step = 1000;
		break;

	case 1000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(md_CurrentPos > st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			Module_Place_Step = 1100; //Z축을 안전 위치로 든다 
		}
		else
		{
			Module_Place_Step = 2000;
		}
		break; 

	case 1100:
  		Ret = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP], st_basic.nRunSpeed);
		
		if(Ret == BD_GOOD)
		{
			Module_Place_Step = 1000;
		}
		else if(Ret == BD_RETRY)
		{
			Module_Place_Step = 1100;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2444, CTL_dWARNING, COMI.mc_alarmcode);
			Module_Place_Step = 1100;
		}
		break;

	case 2000:	// 현재 피커의 상태를 파악하고 , 문제가 되면 로보트 이동전 피커의 상태를 체크한다 . 
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		Ret = Func.PickerSorterStatusCheck(PICKERSTATUS, st_modulemap.nSortRobotPicker, mResult);
		
		if(Ret == RET_GOOD)
		{
			Module_Place_Step = 3000;
		}
		else if(Ret == RET_ERROR)	// 피커 상태 에러 
		{	
			if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME*5))
			{
				if(l_until_wait_time[0][2] < 0)
				{
					l_until_wait_time[0][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
					break;
				}
				break;
			}
			st_work.mn_run_status = dWARNING;
		}
		break;

	//트레이 정보 및 피커 정보를 비교하여 로보트가 이동할 위치를 찾는다 
	case 3000:
		nRet_1 = Robot_CurrentPos_Check(SORTER_SITE, d_XPos,d_YPos);
		if(nRet_1 == CTL_YES)
		{
			////2014,1210
			st_work.m_nSorterCurRetry++;
			if(st_work.m_nSorterCurRetry > 2)
			{
				st_work.m_nSorterCurRetry = 0;
				sprintf(mc_alarmcode,"560910");
				st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
				CTL_Lib.Alarm_Error_Occurrence(2538, st_work.mn_run_status, mc_alarmcode);

			}
			////
			else
			{
				Module_Place_Step = 0;
				FuncRet = CTLBD_RET_RETRY;
			}
		}
		else
		{
			st_work.m_nSorterCurRetry = 0;//2014,1210
			Module_Place_Step = 3100;
		}
		break;

	case 3100:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(md_CurrentPos > st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			Module_Place_Step = 1100; //Z축을 안전 위치로 든다 
			break; 
		}

		///////////////////////// 20121114
		md_CurrentPos = COMI.Get_MotCurrentPos(M_VISION1_Y);
//		if(md_CurrentPos > st_motor[M_VISION1_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION1_Y].mn_allow)
		////2014,0303
		//if(st_sync.n_visionrbt_load_flag[VISION_1_SITE] == 1)
//		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 1)//2014,1209
		if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_1_SITE)//2014,1209
		////
		{
		/////////////////////////
			dAbsPos[0] = st_motor[M_RBT_SORTER_X].md_pos[X_SORT_VISION_BUFFER];
			dAbsPos[1] = st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER];
			
			nRet_1 = CTL_Lib.Linear_Move(SORTER_MOT_MAP, 2, msp_AxisNum, dAbsPos, dSpeedRatio);
			if(nRet_1 == BD_GOOD)
			{
				//st_sync.n_visionrbt_load_flag[VISION_1_SITE] = 0;//2014,0303
				st_sync.n_visionrbt_workend_flag[VISION_1_SITE] = 0;//2014,1209
				l_until_wait_time[0][0] = GetCurrentTime();
				Module_Place_Step = 3300;
			}
			else if(nRet_1 == BD_RETRY)
			{
				Module_Place_Step = 3100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				Module_Place_Step = 3100;
				CTL_Lib.Alarm_Error_Occurrence(2445, CTL_dWARNING, COMI.mc_alarmcode);
			}
		///////////////////////// 20121114
		}
		////2014,0303
		//else if(st_sync.n_visionrbt_load_flag[VISION_2_SITE] == 1)
//		else if(st_sync.n_visionrbt_workend_flag[VISION_2_SITE] == 1)//2014,1209
		else if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_2_SITE)//2014,1209
		//else// if(md_CurrentPos > st_motor[M_VISION2_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION2_Y].mn_allow)
		{
			dAbsPos[0] = st_motor[M_RBT_SORTER_X].md_pos[X_SORT_VISION_BUFFER2 - 2];
			dAbsPos[1] = st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER2 - 4];
			
			nRet_1 = CTL_Lib.Linear_Move(SORTER_MOT_MAP, 2, msp_AxisNum, dAbsPos, dSpeedRatio);
			if(nRet_1 == BD_GOOD)
			{
				//st_sync.n_visionrbt_load_flag[VISION_2_SITE] = 0;//2014,0303
				st_sync.n_visionrbt_workend_flag[VISION_2_SITE] = 0;//2014,1209
				l_until_wait_time[0][0] = GetCurrentTime();
				Module_Place_Step = 3300;
			}
			else if(nRet_1 == BD_RETRY)
			{
				Module_Place_Step = 3100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				Module_Place_Step = 3100;
				CTL_Lib.Alarm_Error_Occurrence(2445, CTL_dWARNING, COMI.mc_alarmcode);
			}
		}
		/////////////////////////	
		break;

	case 3300:  // 가능한 피커를 내리고 피킹하는 동작 
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		nRet_1 = Func.PickerSorterStatusCheck(PICKERSTATUS, st_modulemap.nSortRobotPicker, mResult); // 피커 정보와 실제 센서 상태를 체크 
		
		if(nRet_1 == RET_ERROR)
		{	
			//2010,0531
			if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME))
			{
				if(l_until_wait_time[0][2] < 0)
				{
					l_until_wait_time[0][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
					break;
				}
				break;
			}
			st_work.mn_run_status = dWARNING;
			break;
		}

		Module_Place_Step = 4000;
		break;
		
	case 4000:
		///////////////////////// 20121114
		md_CurrentPos = COMI.Get_MotCurrentPos(M_VISION1_Y);
//		if(md_CurrentPos > st_motor[M_VISION1_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION1_Y].mn_allow)
		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 0)
		{
		/////////////////////////	
			nRet_1 = Robot_CurrentPos_Check(SORTER_SITE, st_motor[M_RBT_SORTER_X].md_pos[X_SORT_VISION_BUFFER], st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER]);
			if(nRet_1 == CTL_YES) //현재 로보트 위치가 목표 우치가 아니다
			{
				Module_Place_Step = 0; //다시 처음 부터 시작 한다 
				break;
			}

			//if (st_sync.n_vision_buffer_req[SITE_SORTER][0] == CTL_READY)
			if(st_sync.n_ldrbt_visionrbt_req[0] == CTL_READY)
			{
				//if(st_sync.n_vision_buffer_req[SITE_SORTER][1] == SITE_UPPER_BUFFER)
//				if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_1_SITE)
//				{
					md_Robot_Z_Pos = st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_VISION_PLACE];
//				}
//				else
//				{
//					md_Robot_Z_Pos = st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_VISION_PLACE2];//20121114
//				}
				Module_Place_Step = 4010;
			}
		///////////////////////// 20121114
		}
		else if(st_sync.n_visionrbt_workend_flag[VISION_2_SITE] == 0)//2014,1209 // if(md_CurrentPos > st_motor[M_VISION2_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION2_Y].mn_allow)
		{
			nRet_1 = Robot_CurrentPos_Check(SORTER_SITE, st_motor[M_RBT_SORTER_X].md_pos[X_SORT_VISION_BUFFER2 - 2], st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER2 - 4]);
			if(nRet_1 == CTL_YES) //현재 로보트 위치가 목표 우치가 아니다
			{
				Module_Place_Step = 0; //다시 처음 부터 시작 한다 //2014,1209
				break;
			}

			//if (st_sync.n_vision_buffer_req[SITE_SORTER][0] == CTL_READY)
			if(st_sync.n_ldrbt_visionrbt_req[0] == CTL_READY)
			{
				//if(st_sync.n_vision_buffer_req[SITE_SORTER][1] == SITE_UPPER_BUFFER)
//				if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_1_SITE)
//				{
//					md_Robot_Z_Pos = st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_VISION_PLACE];
//				}
//				else
//				{
					md_Robot_Z_Pos = st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_VISION_PLACE2];
//				}
				Module_Place_Step = 4010;
			}
		}
		/////////////////////////	
		break;

	case 4010:
		ActionPicker[0][D_EXIST] = YES;
		Set_PickerUpDown(SORTER_SITE, IO_ON, ActionPicker);

		///////////////////////// 20121114
		md_CurrentPos = COMI.Get_MotCurrentPos(M_VISION1_Y);
//		if(md_CurrentPos > st_motor[M_VISION1_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION1_Y].mn_allow)
		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 0)
		{
		/////////////////////////	
			nRet_1 = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_VISION_PLACE], st_basic.nRunSpeed);		
			if(nRet_1 == BD_GOOD)
			{
				Module_Place_Step = 4200;
			}
			else if(nRet_1 == BD_RETRY)
			{
				Module_Place_Step = 4010;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(2446, CTL_dWARNING, COMI.mc_alarmcode);
				Module_Place_Step = 4010;
			}
		///////////////////////// 20121114
		}
		else if(st_sync.n_visionrbt_workend_flag[VISION_2_SITE] == 0) //2014,1209 // if(md_CurrentPos > st_motor[M_VISION2_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION2_Y].mn_allow)
		{
			nRet_1 = CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_VISION_PLACE2], st_basic.nRunSpeed);		
			if(nRet_1 == BD_GOOD)
			{
				Module_Place_Step = 4200;
			}
			else if(nRet_1 == BD_RETRY)
			{
				Module_Place_Step = 4010;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(2446, CTL_dWARNING, COMI.mc_alarmcode);
				Module_Place_Step = 4010;
			}
		}
		/////////////////////////	
		break;

	case 4200:
		nRet_1 = Get_PickerUpDown(SORTER_SITE, IO_ON, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0][0] = GetCurrentTime();
			Module_Place_Step = 4210;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2490, st_work.mn_run_status, mc_alarmcode);
			Module_Place_Step = 4010;
		}
		break;

	case 4210:
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		if(l_until_wait_time[0][2] < 0) l_until_wait_time[0][0] = GetCurrentTime();
		if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME)) break;
		Module_Place_Step = 4300;
		break;


	case 4300: //Pick 디바이스 벌러 놓는다 
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		if(l_until_wait_time[0][2] < 0) l_until_wait_time[0][0] = GetCurrentTime();
		if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME)) break;
		Set_FingerOnOff(SORTER_SITE, IO_ON, ActionPicker);
		Module_Place_Step = 4310;
		break;

	case 4310:
		nRet_1 = Get_FingerOnOff(SORTER_SITE, IO_ON, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			//st_sync.n_ldrbt_visionrbt_req[0] = CTL_DOREADY;
			l_until_wait_time[0][0] = GetCurrentTime();
			Module_Place_Step = 4400;
		}
		else if(nRet_1 == RET_ERROR)
		{
			l_until_wait_time[0][0] = GetCurrentTime();
			CTL_Lib.Alarm_Error_Occurrence(2495, CTL_dWARNING, mc_alarmcode);
			Module_Place_Step = 4300;
		}
		break;	

	case 4400: //디바이스 존재유무 체크 
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		if(l_until_wait_time[0][2] < PICKERSTATUS_CHECK_TIME) break;
		Ret = Func.PickerSorterStatusCheck(IO_OFF, ActionPicker[0], mResult); // 피커 정보와 실제 센서 상태를 체크 

		if(Ret == RET_GOOD)
		{
			Module_Place_Step = 4500;	
		}
		else if(Ret == RET_ERROR)
		{
			if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME*5)) //자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[0][2] < 0)
				{
					l_until_wait_time[0][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
					break;
				}
				break;
			}

			mTempPicker[D_EXIST] = NO;
			if(ActionPicker[0][D_EXIST] == YES && mResult[0] == NO)
			{	
				mTempPicker[D_EXIST] = YES;
			}
 			Module_Place_Step = 4500;	 // 현재 핑거 체크한는데 문제가 있어 벌리면 안된다 
		}
		break;
 
	case 4500: // 모터 업 and picker up
//2012,0930
//		Set_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		 
 		nRet_1 =  CTL_Lib.Single_Move(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP], st_basic.nRunSpeed);

		if(nRet_1 == BD_GOOD)
		{
			Module_Place_Step = 4700;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Module_Place_Step = 4500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2447, CTL_dWARNING, COMI.mc_alarmcode);
			Module_Place_Step = 4500;
		}
		break;

	case 4700: //picker up action check
		//2012,0930
		st_sync.n_ldrbt_visionrbt_req[0] = CTL_DOREADY;
		l_until_wait_time[0][0] = GetCurrentTime();
		Module_Place_Step = 4800;
		break;

		nRet_1 = Get_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);

		if(nRet_1 == RET_GOOD)
		{
			st_sync.n_ldrbt_visionrbt_req[0] = CTL_DOREADY;
			l_until_wait_time[0][0] = GetCurrentTime();
			Module_Place_Step = 4800;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2499, st_work.mn_run_status, mc_alarmcode);
			Module_Place_Step = 4500;
		}
		break;

	case 4800:
		if(st_sync.n_ldrbt_visionrbt_req[0] != CTL_COMPLETE) break;
		i_DVCSortReplaceCnt = 0;
		l_until_wait_time[0][0] = GetCurrentTime();
		Module_Place_Step = 5000;
		break;

	case 4900:
		l_until_wait_time[0][0] = GetCurrentTime();
		Module_Place_Step = 5000;
		break;


	//////////////////////////////////////////////////////////
	// 작업한 피커 및 트레이 정보를 셋팅한다 
	//////////////////////////////////////////////////////////
	case 5000:
		l_until_wait_time[0][1] = GetCurrentTime();
		l_until_wait_time[0][2] = l_until_wait_time[0][1] - l_until_wait_time[0][0];
		
		nRet_1 = Func.PickerSorterStatusCheck(IO_OFF, ActionPicker[0], mResult); // 피커 정보와 실제 센서 상태를 체크 
		
		if(nRet_1 == RET_ERROR)
		{
			if(l_until_wait_time[0][2] < (PICKERSTATUS_CHECK_TIME*5)) //자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[0][2] < 0)
				{
					l_until_wait_time[0][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
					break;
				}
				break;
			}

			//2016.0524
			i_DVCSortReplaceCnt++;
			
			if(i_DVCSortReplaceCnt >= 2)
			{
				i_DVCSortReplaceCnt = 0;		//피킹 재동작의 카운트는 초기화 되는게 맞음!				
				st_work.mn_run_status = dWARNING;				
				CTL_Lib.Alarm_Error_Occurrence(9260, CTL_dWARNING, alarm.mc_code);
				Module_Place_Step = 4900;
			}
			else 
			{
				Module_Place_Step = 4900;
			}
		}
		
		if(ActionPicker[0][D_EXIST] == YES && mResult[0] == YES) // 디바이스가 있는 피커만 체크
		{
			//if(st_sync.n_vision_buffer_req[SITE_SORTER][1] == SITE_UPPER_BUFFER)
			if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_1_SITE)
			{
				st_modulemap.VisionBuffer[D_EXIST][TOPBUFFER] = DVC_YES;
			}
			else
			{
				st_modulemap.VisionBuffer[D_EXIST][BTMBUFFER] = DVC_YES;
			}

			st_modulemap.nSortRobotPicker[D_EXIST]   = DVC_NO;
			st_modulemap.nSortRobotPicker[D_BIN]     = BIN_CLEAR;
			st_modulemap.nSortRobotPicker[D_RE_CNT]  = T_NOTTEST;
			st_modulemap.nSortRobotPicker[D_SCR_BIN] = BIN_CLEAR;

			//st_sync.n_vision_buffer_req[SITE_SORTER][0] = CTL_FREE;
			//st_sync.n_ldrbt_visionrbt_req[0] = CTL_FREE;

			ActionPicker[0][D_EXIST] = NO;  
			mResult[0] = NO;
		}

		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_SORTER_PICKER);			// 피커 정보 
		}

		if(nRet_1 == RET_GOOD)
		{
			st_sync.n_ldrbt_visionrbt_req[0] = CTL_FREE;
//			Func.ULDDelete_Node();
			Module_Place_Step = 6000;
		}
		else if(Ret == RET_ERROR)
		{
			i_DVCSortReplaceCnt++;

			if(i_DVCSortReplaceCnt >= 3)
			{
				i_DVCSortReplaceCnt = 0;		//피킹 재동작의 카운트는 초기화 되는게 맞음!				
				Module_Place_Step = 1000;
				Module_Place_Step = 4900;//2016.0524
				st_work.mn_run_status = dWARNING;
			}
			else 
			{
				Module_Place_Step = 1000;
				Module_Place_Step = 4900;
			}
		}
		break;

	case 6000:
		Module_Place_Step = 0;
		FuncRet = RET_GOOD;			
		break;
	}
	return FuncRet;
}

int CRun_Rbt_Sorter::Robot_CurrentPos_Check(int n_site, double x_pos, double y_pos)
{
	double dCurPos[4]; //x,y,z,p
	int nFlag = CTL_NO;

	if(COMI.mn_simulation_mode == 1) // 20130509
	{
		return CTL_NO;
	}

	if(n_site == SORTER_SITE)
	{
		dCurPos[0] = COMI.Get_MotCurrentPos(M_RBT_SORTER_X);
		dCurPos[1] = COMI.Get_MotCurrentPos(M_RBT_SORTER_Y);
		dCurPos[2] = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);

		if ((x_pos < ((double)dCurPos[0] - (double)(st_motor[M_RBT_SORTER_X].mn_allow))) 
			|| (x_pos > ((double)dCurPos[0] + (double)(st_motor[M_RBT_SORTER_X].mn_allow))))
		{
			nFlag = CTL_YES;
		}
		else if ((y_pos < ((double)dCurPos[1] - (double)(st_motor[M_RBT_SORTER_Y].mn_allow))) 
			|| (y_pos > ((double)dCurPos[1] + (double)(st_motor[M_RBT_SORTER_Y].mn_allow))))
		{
			nFlag = CTL_YES;
		}
	}
	else
	{
		dCurPos[0] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_X);
		dCurPos[1] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Y);
		dCurPos[2] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);

		if ((x_pos < ((double)dCurPos[0] - (double)(st_motor[M_RBT_UNLOAD_X].mn_allow))) 
			|| (x_pos > ((double)dCurPos[0] + (double)(st_motor[M_RBT_UNLOAD_X].mn_allow))))
		{
			nFlag = CTL_YES;
		}
		else if ((y_pos < ((double)dCurPos[1] - (double)(st_motor[M_RBT_UNLOAD_Y].mn_allow))) 
			|| (y_pos > ((double)dCurPos[1] + (double)(st_motor[M_RBT_UNLOAD_Y].mn_allow))))
		{
			nFlag = CTL_YES;
		}
	}

	return nFlag;
	//nFlag => YES:위치가 맞지않다, NO:현재 위치와 목표우치가 일치한다 

}

int CRun_Rbt_Sorter::UldRobot_AutoMove_Safety_Zone(int n_mode, int n_flag)
{
	int Ret=0, Ret_1=0, FuncRet = RET_PROCEED, i=0;
	double dCurrentPos =0;
	double dSpeedRatio[3] = {st_basic.nRunSpeed,st_basic.nRunSpeed,st_basic.nRunSpeed};

	//n_mode == 0 => 로보트가 BCR로 들어 가는 동작 할 예정 
	//n_mode == 1 => 로보트가 BCR이외의 작업을 동작 할 예정
	//n_flag == Not Used
	switch(UldSafetyZone_Step)
	{
	case 0:
		UldSafetyZone_Step = 1000;
		break;

	case 1000:
		dCurrentPos = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_X);
		if(dCurrentPos > st_motor[M_RBT_UNLOAD_X].md_pos[X_SORT_SAFETY] + st_motor[M_RBT_UNLOAD_X].mn_allow)	 
		{ //위험 위치에 있는 상태 
			if(n_mode == 0)  //로보트가 BCR로 들어 가는 동작 할 예정
			{
				UldSafetyZone_Step = 2000;	//이미 들어가 있는 상태로 어느 위치에 있는지 확인한다 
			}
			else //if(n_mode == 1) //로보트가 BCR 이외의 작업을 동작 할 예정
			{
				UldSafetyZone_Step = 3000;	//X축을 BCR를 빠져 나가니 안전 위치까지 이동 시킨다 
			} 
		}
		else
		{//X축이 BCR를 벗어나 안전 위치에 있는 상태이다   
			if(n_mode == 0)  //로보트가 BCR로 들어 가는 동작 할 예정
			{
				UldSafetyZone_Step = 4000;	//X축을 BCR 대기 위치까지 이동 시킨다 
			}
			else //if(n_mode == 1) //로보트가 BCR이외의 작업을 동작 할 예정
			{
				UldSafetyZone_Step = 5000; //이미 X축이 안전 위치에 있다 
			} 
		}
		break;

	//////////////////////////////////////////////////////////////////
	//이미 Test site 안에 있는 상태로 어느 PH사이트에 있는지 확인한다 
	//////////////////////////////////////////////////////////////////
	case 2000: 
		dCurrentPos = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_X);
		if(dCurrentPos >= st_motor[M_RBT_UNLOAD_X].md_pos[X_SORT_SAFETY] + st_motor[M_RBT_UNLOAD_X].mn_allow)
		{
			UldSafetyZone_Step = 5000;//현재 목표로 가야할 위치에 들어와 있는 상태로 이미 목표 위치에 도착에 있다 
		}
		else
		{
			UldSafetyZone_Step = 2100;//Test site 안에 있지만 현재 위치는 목표 위치와 틀리다, 목표 위치를 찾아 가자 
		}
		break;

	///////////////////////////////////////////////////////////////////
	//이동할 사이트가 아니고 위험 우치에 있으면 , 빠진 후 가야할 위치의 안전 위치까지 간다 
	///////////////////////////////////////////////////////////////////
	case 2100://Y축을 안전 위치로 이동 시킨다 
		Ret = CTL_Lib.Single_Move(M_RBT_UNLOAD_X, st_motor[M_RBT_UNLOAD_X].md_pos[X_SORT_SAFETY], st_basic.nRunSpeed);
		if(Ret == BD_GOOD)
		{			
			UldSafetyZone_Step = 2200;
		}
		else if(Ret == BD_RETRY)
		{
			UldSafetyZone_Step = 2100;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2448, st_work.mn_run_status, COMI.mc_alarmcode);
			UldSafetyZone_Step = 2100;
		}
		break;


	case 2200: //X축을 이동 하려는 사이트로 이동 시킨다 
		///////////////////////// 20121114
		md_CurrentPos = COMI.Get_MotCurrentPos(M_VISION1_Y);
//		if(md_CurrentPos > st_motor[M_VISION1_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION1_Y].mn_allow)
		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 0)
		{
		/////////////////////////		
			Ret = CTL_Lib.Single_Move(M_RBT_UNLOAD_Y, st_motor[M_RBT_UNLOAD_Y].md_pos[Y_SORT_VISION_BUFFER], st_basic.nRunSpeed);

			if(Ret == BD_GOOD)
			{			
				UldSafetyZone_Step = 5000;
			}
			else if(Ret == BD_RETRY)
			{
				UldSafetyZone_Step = 2200;
			}
			else if (Ret == BD_ERROR || Ret == BD_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2449, st_work.mn_run_status, COMI.mc_alarmcode);
				UldSafetyZone_Step = 2200;
			}
		///////////////////////// 20121114
		}
		else// if(md_CurrentPos > st_motor[M_VISION2_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION2_Y].mn_allow)
		{
			Ret = CTL_Lib.Single_Move(M_RBT_UNLOAD_Y, st_motor[M_RBT_UNLOAD_Y].md_pos[Y_SORT_VISION_BUFFER2 - 4], st_basic.nRunSpeed);

			if(Ret == BD_GOOD)
			{			
				UldSafetyZone_Step = 5000;
			}
			else if(Ret == BD_RETRY)
			{
				UldSafetyZone_Step = 2200;
			}
			else if (Ret == BD_ERROR || Ret == BD_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2449, st_work.mn_run_status, COMI.mc_alarmcode);
				UldSafetyZone_Step = 2200;
			}			
		}
		/////////////////////////
		break;


	//////////////////////////////////////////////////////////////////
	//BCR 위험 영역안에서 BCR사이트 밖으로 나가는 루틴
	//////////////////////////////////////////////////////////////////
	case 3000: //안전 위치로 X축을 이동 시킨다 
		Ret = CTL_Lib.Single_Move(M_RBT_UNLOAD_X, st_motor[M_RBT_UNLOAD_X].md_pos[X_SORT_SAFETY], st_basic.nRunSpeed);		
		if(Ret == BD_GOOD)
		{			
			UldSafetyZone_Step = 5000;
		}
		else if(Ret == BD_RETRY)
		{
			UldSafetyZone_Step = 3000;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2450, st_work.mn_run_status, COMI.mc_alarmcode);
			UldSafetyZone_Step = 3000;
		}
		break;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//이미 안전 위치에 있으니 목표하는 해당 BCR 사이트 대기 위치까지 바로 들어가 작업 할 수 있도록 안전 위치까지 이동한다  
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 4000:	
		///////////////////////// 20121114
		md_CurrentPos = COMI.Get_MotCurrentPos(M_VISION1_Y);
//		if(md_CurrentPos > st_motor[M_VISION1_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION1_Y].mn_allow)
		if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 0)
		{
		/////////////////////////		
			dUldAbsPos[0] = st_motor[M_RBT_UNLOAD_X].md_pos[X_SORT_SAFETY];
			dUldAbsPos[1] = st_motor[M_RBT_UNLOAD_Y].md_pos[Y_SORT_VISION_BUFFER];	
				 
			Ret = CTL_Lib.Linear_Move(UNLOAD_MOT_MAP, 2, mlp_AxisNum, dUldAbsPos, dSpeedRatio);
			if(Ret == BD_GOOD)
			{			
				UldSafetyZone_Step = 5000;
			}
			else if(Ret == BD_RETRY)
			{
				UldSafetyZone_Step = 4000;
			}
			else if (Ret == BD_ERROR || Ret == BD_SAFETY)
			{
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2451, st_work.mn_run_status, COMI.mc_alarmcode);
				UldSafetyZone_Step = 4000;
			}
		///////////////////////// 20121114
		}
		else// if(md_CurrentPos <= st_motor[M_VISION2_Y].md_pos[Y_LDMODULE_POS] + st_motor[M_VISION2_Y].mn_allow)
		{
			dUldAbsPos[0] = st_motor[M_RBT_UNLOAD_X].md_pos[X_SORT_SAFETY];
			dUldAbsPos[1] = st_motor[M_RBT_UNLOAD_Y].md_pos[Y_SORT_VISION_BUFFER2 - 4];	
				 
			Ret = CTL_Lib.Linear_Move(UNLOAD_MOT_MAP, 2, mlp_AxisNum, dUldAbsPos, dSpeedRatio);
			if(Ret == BD_GOOD)
			{			
				UldSafetyZone_Step = 5000;
			}
			else if(Ret == BD_RETRY)
			{
				UldSafetyZone_Step = 4000;
			}
			else if (Ret == BD_ERROR || Ret == BD_SAFETY)
			{
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2451, st_work.mn_run_status, COMI.mc_alarmcode);
				UldSafetyZone_Step = 4000;
			}			
		}
		/////////////////////////
		break;

	/////////////////////////////////////////////////////
	// PH 안전위치 또는 PH 작업 영역에 존재 할 수 있다 //
	/////////////////////////////////////////////////////
	case 5000: 
		UldSafetyZone_Step = 0;
		FuncRet = RET_GOOD;
		break;
	}

	return FuncRet;
}

int CRun_Rbt_Sorter::OnUldPickupModule(int n_flag)
{
	int Ret = 0, y=0, FuncRet, ActionFlag = NO;
	double dSpeedRatio[3] = {st_basic.nRunSpeed,st_basic.nRunSpeed,st_basic.nRunSpeed};

	FuncRet = RET_PROCEED;

	Func.ThreadFunctionStepTrace(28, UldModule_Pick_Step);
	switch(UldModule_Pick_Step)
	{
	case 0:
		if(st_modulemap.nUldRobotPicker[D_EXIST] == DVC_YES)
		{			
			break;
		}
 		i_DVCUldRepickCnt = 0;
		l_until_wait_time[1][0] = GetCurrentTime();
		UldModule_Pick_Step = 1000;
		break;

	case 1000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);
		if(md_CurrentPos > st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_RBT_UNLOAD_Z].mn_allow)
		{
			UldModule_Pick_Step = 1100; //Z축을 안전 위치로 든다 
		}
		else
		{
			l_until_wait_time[1][0] = GetCurrentTime();
			UldModule_Pick_Step = 2000;
		}
		break; 

	case 1100:
		Ret =  CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		
		if(Ret == BD_GOOD)
		{
			l_until_wait_time[1][0] = GetCurrentTime();
			UldModule_Pick_Step = 2000;
		}
		else if(Ret == BD_RETRY)
		{
			UldModule_Pick_Step = 1100;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			UldModule_Pick_Step = 1100;
		}
		break;

	case 2000:
		l_until_wait_time[1][1] = GetCurrentTime();
		l_until_wait_time[1][2] = l_until_wait_time[1][1] - l_until_wait_time[1][0];
		Ret = Func.PickerUnloadStatusCheck(PICKERSTATUS, st_modulemap.nUldRobotPicker, mUldResult);
		
		if(Ret == RET_GOOD)
		{
			UldModule_Pick_Step = 3000;
		}
		else if(Ret == RET_ERROR)	// 피커 상태 에러 
		{	
			if(l_until_wait_time[1][2] < 0)
			{
				l_until_wait_time[1][0] = GetCurrentTime();
				break;
			}
			if(l_until_wait_time[1][2] < (PICKERSTATUS_CHECK_TIME*5))
			{
				if(l_until_wait_time[1][2] < 0) l_until_wait_time[1][0] = GetCurrentTime();
				break;
			}

			st_work.mn_run_status = dWARNING;
		}
		break;

	//로보트가 이동할 위치를 찾는다 
	case 3000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);
		if(md_CurrentPos > st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_RBT_UNLOAD_Z].mn_allow)
		{
			UldModule_Pick_Step = 3110; //Z축을 안전 위치로 든다 
		}
		else
		{
			UldModule_Pick_Step = 3200;
		}
		break; 

	case 3110:
 		Ret = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		
		if(Ret == BD_GOOD)
		{
			UldModule_Pick_Step = 3200;
		}
		else if(Ret == BD_RETRY)
		{
			UldModule_Pick_Step = 3110;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			UldModule_Pick_Step = 3110;
		}
		break;

	case 3200:
		//Ret = UldRobot_AutoMove_Safety_Zone(0, 0); //해당 사이트로 이동하여 X,Y 로보트가 동작 가능하게 한다 
		//if(Ret == RET_GOOD)
		//{
			UldModule_Pick_Step = 3300;
		//}
		break;

	//로보트를 작업 할 위치로 이동한다 
	case 3300:
		UldModule_Pick_Step = 3310;
		break;

	case 3310:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(md_CurrentPos > st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			UldModule_Pick_Step = 3110;
		}
		else
		{
			UldModule_Pick_Step = 3320;
		}
		break;

	case 3320:
		////2014,0303
		//if(st_sync.n_visionrbt_workend_flag[VISION_1_SITE] == 0)
		if(st_sync.n_visionrbt_unload_flag[VISION_1_SITE] == 1)
		////
		{
			md_UldAbsPos[0] = st_motor[M_RBT_UNLOAD_X].md_pos[3];
			md_UldAbsPos[1] = st_motor[M_RBT_UNLOAD_Y].md_pos[3];
			mn_uldz_pos = 0;
		}
		////2014,0303
		//else if(st_sync.n_visionrbt_workend_flag[VISION_2_SITE] == 0)
		else if(st_sync.n_visionrbt_unload_flag[VISION_2_SITE] == 1)
		////
		{
			md_UldAbsPos[0] = st_motor[M_RBT_UNLOAD_X].md_pos[6];
			md_UldAbsPos[1] = st_motor[M_RBT_UNLOAD_Y].md_pos[6];
			mn_uldz_pos = 1;
		}
		else
		{
			break;
		}

		Ret = CTL_Lib.Linear_Move(UNLOAD_MOT_MAP, 2, mlp_AxisNum, md_UldAbsPos, dSpeedRatio);
		if(Ret == BD_GOOD)
		{	
			////2014,0303
			if(st_sync.n_visionrbt_unload_flag[VISION_1_SITE] == 1)
			{
				st_sync.n_visionrbt_unload_flag[VISION_1_SITE] = 0;//2014,0303		
			}
			else if(st_sync.n_visionrbt_unload_flag[VISION_2_SITE] == 1)
			{
				st_sync.n_visionrbt_unload_flag[VISION_2_SITE] = 0;//2014,0303
			}
			////
			l_until_wait_time[1][0] = GetCurrentTime();
			UldModule_Pick_Step = 3500;
		}
		else if(Ret == BD_RETRY)
		{
			UldModule_Pick_Step = 3300;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2460, st_work.mn_run_status, COMI.mc_alarmcode);
			UldModule_Pick_Step = 3300;
		}
		break;

	case 3500: // 가능한 피커를 내리고 피킹하는 동작 
		l_until_wait_time[1][1] = GetCurrentTime();
		l_until_wait_time[1][2] = l_until_wait_time[1][1] - l_until_wait_time[1][0];
		Ret = Func.PickerUnloadStatusCheck(PICKERSTATUS, st_modulemap.nUldRobotPicker, mUldResult); // 피커 정보와 실제 센서 상태를 체크 
		
		if(Ret == RET_ERROR)
		{
			if(l_until_wait_time[1][2] < 0)
			{
				l_until_wait_time[1][0] = GetCurrentTime();
				break;
			}
			if(l_until_wait_time[1][2] < (PICKERSTATUS_CHECK_TIME*2))
			{
				if(l_until_wait_time[1][2] < 0) l_until_wait_time[1][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				break;
			}
			st_work.mn_run_status = dWARNING;
			break;
		}
		UldModule_Pick_Step = 4000;		
		break;

	case 4000: // moving after finger open
		ActionPicker[1][D_EXIST] = YES;
		Set_FingerOnOff(UNLOADER_SITE, IO_ON, ActionPicker);
		UldModule_Pick_Step = 4010;
		break;

	case 4010:
		Ret = Get_FingerOnOff(UNLOADER_SITE, IO_ON, ActionPicker);

		if(Ret == RET_GOOD)
		{
			UldModule_Pick_Step = 4100;
		}
		else if(Ret != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2497, st_work.mn_run_status, mc_alarmcode);
			UldModule_Pick_Step = 4000;
		}
		break;		

	case 4100:	//피커 및 Z axis down action
		Ret = Robot_CurrentPos_Check(UNLOADER_SITE, md_UldAbsPos[0], md_UldAbsPos[1]);
		if(Ret == CTL_YES) //현재 로보트 위치가 목표 우치가 아니다
		{
			UldModule_Pick_Step = 0; //다시 처음 부터 시작 한다 
		}
		else
		{
			mn_pre_move_flag[1] = CTL_NO;
			UldModule_Pick_Step = 4110;
		}
		break;

	case 4110:
		if (st_sync.n_visionrbt_uldrbt_req[0] != CTL_REQ) break;
		UldModule_Pick_Step = 4120;
		break;

	case 4120:
		Set_PickerUpDown(UNLOADER_SITE, IO_ON, ActionPicker);
		//2016.0826
// 		Ret = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[3], st_basic.nRunSpeed);
		if(mn_uldz_pos == 0)
		{
			Ret = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[3], st_basic.nRunSpeed);
		}
		else
		{
			Ret = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[6], st_basic.nRunSpeed);
		}
		if(Ret == BD_GOOD)
		{
			UldModule_Pick_Step = 4200;
		}
		else if(Ret == BD_RETRY)
		{
			UldModule_Pick_Step = 4120;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2461, st_work.mn_run_status, COMI.mc_alarmcode);
			UldModule_Pick_Step = 4120;
		}
		break;

	case 4200: //picker down action check
		Ret = Get_PickerUpDown(UNLOADER_SITE, IO_ON, ActionPicker);

		if(Ret == RET_GOOD)
		{
			UldModule_Pick_Step = 4300;
		}
		else if(Ret != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2462, st_work.mn_run_status, mc_alarmcode);
			UldModule_Pick_Step = 4120;
		}
		break;

	case 4300: //Pick 디바이스 클램프 
		Set_FingerOnOff(UNLOADER_SITE, IO_OFF, ActionPicker);
		UldModule_Pick_Step = 4310;
		break;

	case 4310:
		Ret = Get_FingerOnOff(UNLOADER_SITE,IO_OFF, ActionPicker);
		if(Ret == RET_GOOD)
		{
			l_until_wait_time[1][0] = GetCurrentTime();
			UldModule_Pick_Step = 4400;
		}
		else if(Ret != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2491, st_work.mn_run_status, mc_alarmcode);
			UldModule_Pick_Step = 4300;
		}
		break;	

	case 4400: //디바이스 존재유무 체크 
		l_until_wait_time[1][1] = GetCurrentTime();
		l_until_wait_time[1][2] = l_until_wait_time[1][1] - l_until_wait_time[1][0];
		if(l_until_wait_time[1][2] < PICKERSTATUS_CHECK_TIME) break;
		UldTempPicker[D_EXIST] = ActionPicker[1][D_EXIST];
		Ret = Func.PickerUnloadStatusCheck(IO_ON, UldTempPicker, mUldResult); // 피커 정보와 실제 센서 상태를 체크 

		if(Ret == RET_GOOD)
		{
			UldModule_Pick_Step = 4500;	
		}
		else if(Ret == RET_ERROR)
		{
			if(l_until_wait_time[1][2] < (PICKERSTATUS_CHECK_TIME*5))  
			{
				if(l_until_wait_time[1][2] < 0)
				{
					l_until_wait_time[1][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
					break;
				}
				break;
			}
			mUldTempPicker[D_EXIST] = NO;

			if(UldTempPicker[D_EXIST] == YES && mUldResult[0] == NO)
			{	
				//문제 발생시 에러시 해당 포트만 오픈 시키자 
				mTempPicker[D_EXIST] = YES;
			}
 			st_work.mn_run_status = CTL_dWARNING;
			UldModule_Pick_Step = 4300;	 // 현재 핑거 체크한는데 문제가 있어 벌리면 안된다 
		}
		break;

	case 4500:
		st_sync.n_visionrbt_uldrbt_req[0] = CTL_READY;
		l_until_wait_time[1][0] = GetCurrentTime();
		UldModule_Pick_Step = 4600;
		break;
 
	case 4600: // 모터 업 and picker up
		l_until_wait_time[1][1] = GetCurrentTime();
		l_until_wait_time[1][2] = l_until_wait_time[1][1] - l_until_wait_time[1][0];
		if(l_until_wait_time[1][2] < 0)	l_until_wait_time[1][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
		if(l_until_wait_time[1][2] < PICKERSTATUS_CHECK_TIME*2) break;
		if(st_sync.n_visionrbt_uldrbt_req[0] != CTL_COMPLETE) break;
		UldModule_Pick_Step = 4700;
		break;
		
	case 4700:		
		Set_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);		
 		Ret = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(Ret == BD_GOOD)
		{
			UldModule_Pick_Step = 4800;
		}
		else if(Ret == BD_RETRY)
		{
			UldModule_Pick_Step = 4700;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2498, st_work.mn_run_status, COMI.mc_alarmcode);
			UldModule_Pick_Step = 4700;
		}
		break;

	case 4800: //picker up action check
		Ret = Get_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);

		if(Ret == RET_GOOD)
		{
			Set_FingerOnOff(UNLOADER_SITE, IO_OFF, ActionPicker);
			UldModule_Pick_Step = 4900;
		}
		else if(Ret == RET_ERROR)
		{
			UldModule_Pick_Step = 4700;
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2499, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 4900:
		Ret = Get_FingerOnOff(UNLOADER_SITE,IO_OFF, ActionPicker);
		if(Ret == RET_GOOD)
		{
			l_until_wait_time[1][0] = GetCurrentTime();
			UldModule_Pick_Step = 5000;
		}
		else if(Ret != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2496, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	//////////////////////////////////////////////////////////
	// 작업한 피커 및 트레이 정보를 셋팅한다 
	//////////////////////////////////////////////////////////
	case 5000:
		l_until_wait_time[1][1] = GetCurrentTime();
		l_until_wait_time[1][2] = l_until_wait_time[1][1] - l_until_wait_time[1][0];
		
		Ret = Func.PickerUnloadStatusCheck(IO_ON, ActionPicker[1], mUldResult); // 피커 정보와 실제 센서 상태를 체크 
//		mUldResult[0] = YES;
		if(Ret == RET_ERROR)
		{
			if(l_until_wait_time[1][2] < (PICKERSTATUS_CHECK_TIME*5))
			{
				if(l_until_wait_time[1][2] < 0)
				{
					l_until_wait_time[1][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
					break;
				}
				break;
			}
			st_work.mn_run_status = CTL_dWARNING;
			break;
		}
		
		if(ActionPicker[1][D_EXIST] == YES && mUldResult[0] == NO)
		{	
			//문제 발생시 에러시 해당 포트만 오픈 시키자 
			mTempPicker[D_EXIST] = YES;
		}
		if(ActionPicker[1][D_EXIST] == YES && mUldResult[0] == YES) // 디바이스가 있는 피커만 체크
		{
//			st_sync.n_vision_buffer_req[SITE_ULDBUFFER][0] = CTL_CHANGE;
			//if (st_sync.n_vision_buffer_req[SITE_ULDBUFFER][1] == TOPBUFFER)
			if(st_sync.n_visionrbt_uldrbt_req[1] == TOPBUFFER)
			{
				st_modulemap.VisionBuffer[D_EXIST][SITE_UPPER_BUFFER] = DVC_NO;
				st_modulemap.nUldRobotPicker[D_BIN]	= st_modulemap.VisionBuffer[D_BIN][SITE_UPPER_BUFFER];
				st_modulemap.VisionBuffer[D_BIN][SITE_UPPER_BUFFER] = BIN_CLEAR;
				
			}
			else
			{
				st_modulemap.VisionBuffer[D_EXIST][SITE_LOWER_BUFFER] = DVC_NO;
				st_modulemap.nUldRobotPicker[D_BIN]	= st_modulemap.VisionBuffer[D_BIN][SITE_LOWER_BUFFER];
				st_modulemap.VisionBuffer[D_BIN][SITE_LOWER_BUFFER] = BIN_CLEAR;
			}
			
			st_modulemap.nUldRobotPicker[D_EXIST]	= DVC_YES;
			st_modulemap.nUldRobotPicker[D_RE_CNT]	= T_NOTTEST;			// Test Count Reset
			st_modulemap.nUldRobotPicker[D_SCR_BIN]	= BIN_CLEAR;			// Screen Bin		

			st_sync.n_visionrbt_uldrbt_req[0] = CTL_FREE;

			ActionPicker[1][D_EXIST] = NO;  
			mUldResult[0] = NO;
		}


		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_UNLOADER_PICKER);			// 피커 정보 
		}

		if(Ret == RET_GOOD)
		{
			UldModule_Pick_Step = 6000;
		}
		else if(Ret == RET_ERROR)
		{
			i_DVCUldRepickCnt++;

			if(i_DVCUldRepickCnt >= 3)
			{
				i_DVCUldRepickCnt = 0;			//피킹 재동작의 카운트는 초기화 되는게 맞음!

				FuncRet = RET_NOWHERE;		// Buffer에 Device가 없다고 강제 세팅하고, 다음 Buffer를 요청한다.
				st_work.mn_run_status = dWARNING;
				UldModule_Pick_Step = 0;
				break;
				
			}
			else 
			{
				UldModule_Pick_Step = 1000;
			}
			
			if(st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_UNLOADER_PICKER);  //로더 정보 
			}
				
		}
		break;

	case 6000:
		if(st_work.i_robot_device_loading != TRUE)
		{
			st_work.i_robot_device_loading = TRUE;
		}
		UldModule_Pick_Step = 0;
		FuncRet = RET_GOOD;			
		break;
	}

	return FuncRet;
}

///////////////////////////////////////////////////////////////////
//GOOD DVC Place, Reject DVC Place
///////////////////////////////////////////////////////////////////
int CRun_Rbt_Sorter::OnUldPlaceModuleTray(int n_bin, int n_tray_y)
{
	int Ret=0, Ret_1=0, Ret_2=0, x, y=0, FuncRet, ActionFlag = NO;
	double dSpeedRatio[3] = {st_basic.nRunSpeed,st_basic.nRunSpeed,st_basic.nRunSpeed};
	FuncRet = RET_PROCEED;

	Func.ThreadFunctionStepTrace(29, TrayPlace_Step);
	switch(TrayPlace_Step)
	{
	case 0:		 
		//mn_FirstPicker_X_Pos = -1;

		if(n_bin == BIN_GOODBIN)
		{
			if(st_sync.n_module_uldrbt_tray_req[0] != CTL_READY)
			{				
				if(st_modulemap.nUldRobotPicker[D_EXIST] == DVC_YES && st_modulemap.nUldRobotPicker[D_BIN] == n_bin) //디바이스를 잡고있지않은 피커번호 (집으려고 찾는거니까..)
				{
					mCurrent_RowPos = 0;//mn_FirstTray_Y_Pos - (mn_FirstPicker_X_Pos * PICKING_GAP);

					md_UldRobot_X_Pos = st_motor[M_RBT_UNLOAD_X].md_pos[X_ULD_TRAY_PLACE]; 
					md_UldRobot_Y_Pos = Func.XYRobot_MovePosValCheck(UNLOADER_SITE, 0, mCurrent_RowPos); //미리 피커를 대기시킨다  
					TrayPlace_Step = 10;
					break;
				}
			}
		}
		else //if(n_bin == BIN_REJECTBIN) //리젝 로딩 트레이에서 작업 
		{
			if(st_sync.n_module_uldrbt_tray_req[0] != CTL_READY)
			{
				if(st_modulemap.nUldRobotPicker[D_EXIST] == DVC_YES && st_modulemap.nUldRobotPicker[D_BIN] == n_bin) //디바이스를 잡고있지않은 피커번호 (집으려고 찾는거니까..)
				{
					mCurrent_RowPos = 0;//mn_FirstTray_X_Pos - (mn_FirstPicker_X_Pos * PICKING_GAP);

					md_UldRobot_X_Pos = st_motor[M_RBT_UNLOAD_X].md_pos[X_ULD_ETRAY_PLACE];  
					md_UldRobot_Y_Pos = Func.XYRobot_MovePosValCheck(REJECT_SITE, 0, mCurrent_RowPos); //미리 피커를 대기시킨다 
					TrayPlace_Step = 10;
					break;
				}
			}
		}

		i_DVCUldReplaceCnt = 0;
		l_until_wait_time[1][0] = GetCurrentTime();
		TrayPlace_Step = 1000;
		break;

	case 10:
		TrayPlace_Step = 20;
		break;

	case 20:		
		//dUldAbsPos[0] = md_UldRobot_X_Pos;
		//dUldAbsPos[1] = md_UldRobot_Y_Pos;
		//Ret = COMI.Start_LinearMove(UNLOAD_MOT_MAP, dUldAbsPos, dSpeedRatio);
		Ret = CTLBD_RET_GOOD;
	
		if(Ret == CTLBD_RET_GOOD)
		{
			TrayPlace_Step = 21;
		}
		else if(Ret == CTLBD_RET_RETRY)
		{
			TrayPlace_Step = 20;
		}
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			TrayPlace_Step = 20;
		}
		break;

	case 21:
		//Ret = COMI.Check_LinearMove(UNLOAD_MOT_MAP, dUldAbsPos);
		Ret = CTLBD_RET_GOOD;

		if(Ret == CTLBD_RET_GOOD)
		{
			TrayPlace_Step = 30;			
		}
		else if(Ret == CTLBD_RET_RETRY)
		{
			TrayPlace_Step = 20;
		}
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			TrayPlace_Step = 20;
		}
		break;

	case 30://트레이가 준비 되기를 기다린다 
		if(n_bin == BIN_GOODBIN)
		{
			//if(st_sync.n_module_uldrbt_tray_req[0] == CTL_READY)
			//{
				TrayPlace_Step = 0;	
			//}
		}
		else //if(n_bin == BIN_REJECTBIN) //리젝 로딩 트레이에서 작업 
		{
			TrayPlace_Step = 0;	
		}
		break;

	case 1000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);
		if(md_CurrentPos > st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_RBT_UNLOAD_Z].mn_allow)
		{
			TrayPlace_Step = 1100; //Z축을 안전 위치로 든다 
		}
		else
		{
			l_until_wait_time[1][0] = GetCurrentTime();
			TrayPlace_Step = 2000;
		}
		break; 

	case 1100:
 		Ret = COMI.Start_SingleMove(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		
		if(Ret == CTLBD_RET_GOOD)
		{
			TrayPlace_Step = 1110;
		}
		else if(Ret == CTLBD_RET_RETRY)
		{
			TrayPlace_Step = 1100;
		}
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			TrayPlace_Step = 1100;
		}
		break;

	case 1110:
		Ret = COMI.Check_SingleMove(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP]);
		if(Ret == CTLBD_RET_GOOD)
		{
			l_until_wait_time[1][0] = GetCurrentTime();
			TrayPlace_Step = 2000;
		}
		else if(Ret == CTLBD_RET_RETRY)
		{
			TrayPlace_Step = 1100;
		}
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			TrayPlace_Step = 1100;
		}
		break;	 

	case 2000:	// 현재 피커의 상태를 파악하고 , 문제가 되면 로보트 이동전 피커의 상태를 체크한다 .		
		l_until_wait_time[1][1] = GetCurrentTime();
		l_until_wait_time[1][2] = l_until_wait_time[1][1] - l_until_wait_time[1][0];
		Ret = Func.PickerUnloadStatusCheck(PICKERSTATUS, st_modulemap.nUldRobotPicker, mUldResult);
		
		if(Ret == RET_GOOD)
		{
			TrayPlace_Step = 3000;
		}
		else if(Ret == RET_ERROR)	// 피커 상태 에러 
		{	
			if(l_until_wait_time[1][2] < (PICKERSTATUS_CHECK_TIME*5))
			{
				if(l_until_wait_time[1][2] < 0)
				{
					l_until_wait_time[1][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
					break;
				}
				break;
			}
			st_work.mn_run_status = dWARNING;
		}
		break;

	//트레이 정보 및 피커 정보를 비교하여 로보트가 이동할 위치를 찾는다 
	case 3000:
		mn_FirstTray_Y_Pos = -1;
		//첫번째 집을 수 있는 피커 위치를 찾는다 
		if(st_modulemap.nUldRobotPicker[D_EXIST] == DVC_YES && st_modulemap.nUldRobotPicker[D_BIN] == n_bin)
		{
		}
		else
		{
			TrayPlace_Step = 0;
			FuncRet = RET_PICKERNOTFIND; //이곳에서 사용 할 수 있는 피커가 없다 
			break;
		}

		if(n_bin == BIN_GOODBIN) //굿 언로딩 트레이에서 작업
		{
			for(x=0 ; x<st_traybuffer[UNLOADER_SITE].i_unloader_row; x++)
			{
				if(st_modulemap.UnloadTray[1][x] == DVC_NO) //디바이스가 있으면 
				{
					mn_FirstTray_Y_Pos = x;
					break;
				}
			}
		}
		else //if(n_bin == BIN_REJECTBIN) //리젝 로딩 트레이에서 작업 
		{
			//첫번째 집을 수 있는 tray x 정보를 찾는다 
			for(x=0 ; x<st_traybuffer[REJECT_SITE].i_loader_row; x++)
			{
				if(st_modulemap.RejectTray[x] == DVC_NO) //디바이스가 있으면 
				{				
					mn_FirstTray_Y_Pos = x;
					break;
				}
			}
		}

		if(mn_FirstTray_Y_Pos == -1) 
		{
			TrayPlace_Step = 0;
			FuncRet = RET_NOWHERE;	//트레이에서 작업할 공간이 없다, 해당 줄은 더이상 작업할 디바이스가 없는 상태이다, 함수 밖에서 트레이를 Y 열 교체를 결정한다 
			break;
		}
	
		//로보트가 이동할 위치를 찾는다 
		if(n_bin == BIN_GOODBIN) //굿 언로딩 트레이에서 작업
		{
			mCurrent_RowPos = mn_FirstTray_Y_Pos;
		}
		else //if(n_bin == BIN_REJECTBIN) //리젝 로딩 트레이에서 작업 
		{
			mCurrent_RowPos = mn_FirstTray_Y_Pos;
		}

		////////////////////////////////////
		//동작 할 피커 정보를 셋팅한다 
		////////////////////////////////////
		ActionFlag = NO;
		ActionPicker[1][D_EXIST] = NO; // 피커 정보 클리어 하자.

		if(n_bin == BIN_GOODBIN) //굿 언로딩 트레이에서 작업
		{			
			if(st_modulemap.nUldRobotPicker[D_EXIST] == YES && st_modulemap.nUldRobotPicker[D_BIN] == n_bin) // 디바이스가 없는 피커만 체크
			{
				if(st_modulemap.UnloadTray[1][mn_FirstTray_Y_Pos] == DVC_NO)
				{
					ActionFlag = YES;
					ActionPicker[1][D_EXIST] = YES; // 동작 가능한 피커						
				}
			}
		}
		else //if(n_bin == BIN_REJECTBIN) //리젝 로딩 트레이에서 작업
		{
			if(st_modulemap.nUldRobotPicker[D_EXIST] == YES && st_modulemap.nUldRobotPicker[D_BIN] == n_bin) // 디바이스가 없는 피커만 체크
			{
				if(st_modulemap.RejectTray[mn_FirstTray_Y_Pos] == DVC_NO)
				{
					ActionFlag = YES;
					ActionPicker[1][D_EXIST] = YES; // 동작 가능한 피커
				}
			}
		}

		if(ActionFlag == YES) // 하나라도 동작 가능하면 다음으로 이동 
		{
			TrayPlace_Step = 3100;
		}
		else
		{	
			TrayPlace_Step = 0;
			FuncRet = RET_NOWHERE;
		}
		break;	

	case 3100: //로보트 피치 모터를 트레이에서 집을 수 있도록 이동한다 
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);
		if(md_CurrentPos > st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_RBT_UNLOAD_Z].mn_allow)
		{
			TrayPlace_Step = 3110;
		}
		else
		{
			TrayPlace_Step = 3200;
		}
		break; 

	case 3110:
 		Ret = COMI.Start_SingleMove(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		
		if(Ret == CTLBD_RET_GOOD)
		{
			TrayPlace_Step = 3111;
		}
		else if(Ret == CTLBD_RET_RETRY)
		{
			TrayPlace_Step = 3110;
		}
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2462, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlace_Step = 3110;
		}
		break;

	case 3111:
		Ret = COMI.Check_SingleMove(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP]);
		if(Ret == CTLBD_RET_GOOD)
		{
			TrayPlace_Step = 3200;
		}
		else if(Ret == CTLBD_RET_RETRY)
		{
			TrayPlace_Step = 3110;
		}
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2463, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlace_Step = 3110;
		}
		break;	 

	case 3200:
		TrayPlace_Step = 3300;
		break;

	case 3300:
		if(n_bin == BIN_GOODBIN)
		{
			TrayPlace_Step = 3400;
		}
		else
		{
			TrayPlace_Step = 3400;
		}
		break;

	//로보트를 작업 할 위치로 이동한다 
	case 3400:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);
		if(md_CurrentPos > st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_RBT_UNLOAD_Z].mn_allow && COMI.mn_simulation_mode != 1) // 20130509
		{
			TrayPlace_Step = 3100; //Z축을 안전 위치로 든다 
			break;
		}

		if(n_bin == BIN_GOODBIN)
		{
			md_UldRobot_X_Pos = st_motor[M_RBT_UNLOAD_X].md_pos[1];  //고정이니까...
			md_UldRobot_Y_Pos = Func.XYRobot_MovePosValCheck(UNLOADER_SITE, 0, mCurrent_RowPos);
		}
		else //if(n_bin == BIN_REJECTBIN) //리젝 로딩 트레이에서 작업
		{
			md_UldRobot_X_Pos = st_motor[M_RBT_UNLOAD_X].md_pos[4];  //고정이니까...
			md_UldRobot_Y_Pos = Func.XYRobot_MovePosValCheck(REJECT_SITE, 0,  mCurrent_RowPos);
		}
		dUldAbsPos[0] = md_UldRobot_X_Pos;
		dUldAbsPos[1] = md_UldRobot_Y_Pos;
		Ret = CTL_Lib.Linear_Move(UNLOAD_MOT_MAP, 2, mlp_AxisNum, dUldAbsPos, dSpeedRatio);
	
		if(Ret == BD_GOOD)
		{
			l_until_wait_time[1][0] = GetCurrentTime();
			TrayPlace_Step = 3500;
		}
		else if(Ret == BD_RETRY)
		{
			TrayPlace_Step = 3400;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2464, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlace_Step = 3400;
		}
		break;
	
	case 3500: // 가능한 피커를 내리고 피킹하는 동작 
		l_until_wait_time[1][1] = GetCurrentTime();
		l_until_wait_time[1][2] = l_until_wait_time[1][1] - l_until_wait_time[1][0];
		Ret = Func.PickerUnloadStatusCheck(PICKERSTATUS, st_modulemap.nUldRobotPicker, mUldResult); // 피커 정보와 실제 센서 상태를 체크 
		
		if(Ret == RET_ERROR)
		{	
			if(l_until_wait_time[1][2] < (PICKERSTATUS_CHECK_TIME*5)) //james 0121 자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[1][2] < 0)	l_until_wait_time[1][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				break;
			}
			st_work.mn_run_status = CTL_dWARNING;
			break;
		}

		TrayPlace_Step = 4000;		
		break;

	case 4000:	//피커 및 Z axis down action
		Ret = Robot_CurrentPos_Check(UNLOADER_SITE, dUldAbsPos[0], dUldAbsPos[1]);
		if(Ret == CTL_YES) //현재 로보트 위치가 목표 우치가 아니다
		{
			TrayPlace_Step = 0; //다시 처음 부터 시작 한다 
			break;
		}

		ActionPicker[1][D_EXIST] = YES;

		Set_PickerUpDown(UNLOADER_SITE, IO_ON, ActionPicker);

		if(n_bin == BIN_GOODBIN) //굿 언로딩 트레이에서 작업
		{
			Ret = COMI.Start_SingleMove(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[2], st_basic.nRunSpeed);
		}
		else //if(n_bin == BIN_REJECTBIN) //리젝 로딩 트레이에서 작업
		{
			Ret = COMI.Start_SingleMove(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[4], st_basic.nRunSpeed);
		}		
		
		if(Ret == CTLBD_RET_GOOD)
		{
			TrayPlace_Step = 4010;
		}
		else if(Ret == CTLBD_RET_RETRY)
		{
			TrayPlace_Step = 4000;
		}
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2465, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlace_Step = 4000;
		}
		break;

	case 4010:
		if(n_bin == BIN_GOODBIN) //굿 언로딩 트레이에서 작업
		{
			Ret = COMI.Check_SingleMove(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[2]);
		}
		else //if(n_bin == BIN_REJECTBIN) //리젝 로딩 트레이에서 작업
		{
			Ret = COMI.Check_SingleMove(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[4]);
		}
		if(Ret == CTLBD_RET_GOOD)
		{
			TrayPlace_Step = 4100;
		}
		else if(Ret == CTLBD_RET_RETRY)
		{
			TrayPlace_Step = 4000;
		}
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2466, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlace_Step = 4000;
		}
		break;

	case 4100: //picker down action check
		Ret = Get_PickerUpDown(UNLOADER_SITE, IO_ON, ActionPicker);

		if(Ret == RET_GOOD)
		{
			TrayPlace_Step = 4200;
		}
		else if(Ret == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2467, st_work.mn_run_status, mc_alarmcode);
			TrayPlace_Step = 4000;
		}
		break;


	case 4200: // moving after finger open //device place
		Set_FingerOnOff(UNLOADER_SITE, IO_ON, ActionPicker);
		TrayPlace_Step = 4210;
		break;

	case 4210:
		Ret = Get_FingerOnOff(UNLOADER_SITE, IO_ON, ActionPicker);
		if(Ret == RET_GOOD)
		{
			l_until_wait_time[1][0] = GetCurrentTime();
			TrayPlace_Step = 4300;
		}
		else if(Ret == RET_ERROR)
		{

			CTL_Lib.Alarm_Error_Occurrence(2492, CTL_dWARNING, mc_alarmcode);
			TrayPlace_Step = 4200;
		}
		break;	 

	case 4300: //디바이스 존재유무 체크 
		l_until_wait_time[1][1] = GetCurrentTime();
		l_until_wait_time[1][2] = l_until_wait_time[1][1] - l_until_wait_time[1][0];
		Ret = Func.PickerUnloadStatusCheck(IO_ON, ActionPicker[1], mResult); // 피커 정보와 실제 센서 상태를 체크 

		if(Ret == RET_GOOD)
		{
			TrayPlace_Step = 4400;	
		}
		else if(Ret == RET_ERROR)
		{
			if(l_until_wait_time[1][2] < (PICKERSTATUS_CHECK_TIME*5)) //james 0121 자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[1][2] < 0) l_until_wait_time[1][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				break;
			}

			mTempPicker[D_EXIST] = NO;

			if(ActionPicker[1][D_EXIST] == YES && mUldResult[0] == NO)
			{	
				//문제 발생시 에러시 해당 포트만 오픈 시키자 
				mTempPicker[D_EXIST] = YES;
			}
 			TrayPlace_Step = 4400;	 // 현재 핑거 체크한는데 문제가 있어 벌리면 안된다 
		}
		break;
 
	case 4400: // 모터 업 and picker up
		Set_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);
 		Ret = CTL_Lib.Single_Move(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);

		if(Ret == BD_GOOD)
		{
			TrayPlace_Step = 4700;
		}
		else if(Ret == BD_RETRY)
		{
			TrayPlace_Step = 4400; //ybs 4500;
		}
		else if (Ret == BD_ERROR || Ret == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2467, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlace_Step = 4400;//ybs 4500;
		}
		break;

	case 4700: //picker up action check
		Ret = Get_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);

		if(Ret == RET_GOOD)
		{
			l_until_wait_time[1][0] = GetCurrentTime();
			TrayPlace_Step = 5000;
		}
		else if(Ret != CTLBD_RET_PROCEED)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2468, st_work.mn_run_status, mc_alarmcode);
			TrayPlace_Step = 4400; //ybs 4500;
		}
		break;

	//////////////////////////////////////////////////////////
	// 작업한 피커 및 트레이 정보를 셋팅한다 
	//////////////////////////////////////////////////////////
	case 5000:
		l_until_wait_time[1][1] = GetCurrentTime();
		l_until_wait_time[1][2] = l_until_wait_time[1][1] - l_until_wait_time[1][0];
		
		Ret = Func.PickerUnloadStatusCheck(IO_OFF, ActionPicker[1], mUldResult); // 피커 정보와 실제 센서 상태를 체크 
//		mUldResult = YES;
		
		if(Ret == RET_ERROR) //james 2010,0121
		{
			if(l_until_wait_time[1][2] < (PICKERSTATUS_CHECK_TIME*5)) //james 0121 자재를 체크할 수 있는 안정 시간을 준다  
			{
				if(l_until_wait_time[1][2] < 0) l_until_wait_time[1][0] = GetCurrentTime(); //값이 - 값이면 시간 초기화하여 다시 체크한다 
				break;
			}
		}
		
		if(ActionPicker[1][D_EXIST] == YES && mUldResult[0] == YES) // 디바이스가 있는 피커만 체크
		{
			if(n_bin == BIN_GOODBIN) //굿 언로딩 트레이에서 작업
			{				
				st_modulemap.UnloadTray[1][mn_FirstTray_Y_Pos] = DVC_YES; //트레이에 디바이스가 없음을 셋

				st_modulemap.nUldRobotPicker[D_EXIST] = DVC_NO;
				st_modulemap.nUldRobotPicker[D_BIN] = BIN_CLEAR;
				st_modulemap.nUldRobotPicker[D_RE_CNT] = T_NOTTEST;
				st_modulemap.nUldRobotPicker[D_SCR_BIN] = BIN_CLEAR;	
				st_work.i_sort_good_cnt++;
				st_work.nMdlPassCount[0][0] = st_work.i_sort_good_cnt;
			}
			else //if(n_bin == BIN_REJECTBIN) //리젝 로딩 트레이에서 작업
			{
				//st_modulemap.UnloadTray[1][mn_FirstTray_Y_Pos] = DVC_YES; //트레이에 디바이스가 없음을 셋
				////////////////////////////////////////////////////////////
				//ybs 3.25
				st_modulemap.RejectTray[mn_FirstTray_Y_Pos] = DVC_YES;
				//st_modulemap.RejectTray[mn_FirstTray_Y_Pos] = st_modulemap.nUldRobotPicker[D_SCR_BIN];
				////////////////////////////////////////////////////////////
				
				st_modulemap.nUldRobotPicker[D_EXIST] = DVC_NO;
				st_modulemap.nUldRobotPicker[D_BIN] = BIN_CLEAR;
				st_modulemap.nUldRobotPicker[D_RE_CNT] = T_NOTTEST;
				st_modulemap.nUldRobotPicker[D_SCR_BIN] = BIN_CLEAR;	
				st_work.i_sort_reject_cnt++;
				st_work.nMdlRejectCount[0][0] = st_work.i_sort_reject_cnt;
			}			

			ActionPicker[1][D_EXIST] = NO;  
			mUldResult[0] = NO;
		}

		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_UNLOADER_PICKER);			// 피커 정보 
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_UNLOADER_TRAY);			// 피커 정보 
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_REJECT_TRAY);			// 피커 정보 
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);			// 로더 정보 
		}

		if(st_handler.cwnd_main != NULL)
		{
//			if(n_bin == BIN_GOODBIN) //굿 언로딩 트레이에서 작업
//			{
//				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);			// 로더 정보 
//			}
//			else if(n_bin == BIN_REJECTBIN) //리젝 로딩 트레이에서 작업
//			{
//				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);			// 로더 정보 
//			}
//			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, PICKER);	// 피커 정보 
//			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BININFO, 0);	// 피커 정보 
		}

		if(Ret == RET_GOOD)
		{
			TrayPlace_Step = 6000;
		}
		else if(Ret == RET_ERROR)
		{
			i_DVCUldReplaceCnt++;
			
			TrayPlace_Step = 1000;	
			st_work.mn_run_status = dWARNING;
		}
		break;

	case 6000: //정상적으로 모두 잡았으면 
		TrayPlace_Step = 0;
		FuncRet = RET_GOOD;			
		break;
	}

	return FuncRet;
}


int CRun_Rbt_Sorter::Find_TrayInfo_Count(int n_site, int n_dvcyesno, int n_tray_y_pos)
{
	int x=0, y=0, nFlag=CTL_NO, nCnt = 0;

	if(n_site == UNLOADER_SITE)
	{
		if(n_tray_y_pos != -1)
		{
			for(x=0 ; x<st_traybuffer[UNLOADER_SITE].i_loader_row; x++)
			{
				if(st_modulemap.UnloadTray[1][x] == n_dvcyesno) //디바이스가 있으면 
				{
					nCnt++;					 
				}
			}
		}		
		else //if(n_tray_y_pos == -1) //트레이 전체 정보를 구한다 
		{
			for(x=0 ; x<st_traybuffer[UNLOADER_SITE].i_loader_row; x++)
			{
				if(st_modulemap.UnloadTray[1][x] == n_dvcyesno) //디바이스가 있으면 
				{
					nCnt++;	
				}
			}
		}		
		
	}
	else if(n_site == REJECT_SITE) // 
	{
		if(n_tray_y_pos != -1)
		{
			for(x=0 ; x<st_traybuffer[UNLOADER_SITE].i_unloader_row; x++)
			{
				if(st_modulemap.RejectTray[x] == n_dvcyesno) //디바이스가 있으면 
				{
					nCnt++;
				}
			}
		}
		else //if(n_tray_y_pos == -1) //트레이 전체 정보를 구한다 
		{
			for(x=0 ; x<st_traybuffer[REJECT_SITE].i_unloader_row; x++)
			{
				if(st_modulemap.RejectTray[x] == n_dvcyesno) //디바이스가 있으면 
				{
					nCnt++;
				}
			}
		}		
	}

	return nCnt;
}

int CRun_Rbt_Sorter::Find_Picker_Count(int n_enable, int n_dvcyesno, int n_bin)
{
	int x=0, y=0, nCnt = 0;

//	for(i = 0 ; i < PICKER_NUM ; i++) //BUF1_SIZE 과 BUF2_SIZE는 같다고 본다 , 같아야 한다 
//	{
		if((st_modulemap.nUldRobotPicker[D_EXIST] == n_dvcyesno || n_dvcyesno == -1)
			&& (st_modulemap.nUldRobotPicker[D_BIN] == n_bin || n_bin == BIN_NOTUESD))
		{
			if(n_enable == CTL_YES) //picker enable check count
			{
//				if(st_modulemap.PickerEnable[i] == YES)
//				{
					nCnt++;
//				}
			}
			else
			{
				nCnt++;
			}
		}
//	}
	  
	return nCnt;	
}
