// Run_Rbt_Work_Buffer.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Rbt_Work_Buffer.h"

#include "ComizoaPublic.h"
#include "CtlBd_Function.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Library.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Work_Buffer
CRun_Rbt_Work_Buffer Run_Rbt_Work_Buffer;
CRun_Rbt_Work_Buffer::CRun_Rbt_Work_Buffer()
{
	Thread_Variable_Initial();
}

CRun_Rbt_Work_Buffer::~CRun_Rbt_Work_Buffer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Work_Buffer message handlers
void CRun_Rbt_Work_Buffer::Thread_Run()
{
	// **************************************************************************
    // 쓰레드 강제 리턴 조건 검사한다 (System Lock)                              
    // **************************************************************************
	if(st_handler.mn_system_lock != FALSE)  return;	// 현재 시스템 Lock 상태인 경우 리턴 
	// **************************************************************************

	
	switch(st_work.mn_run_status)
	{
	case dINIT:
		Run_Init();
		break;

	case dRUN:
		Run_Worker_Buffer1();
		Run_Worker_Buffer2();
		Run_Worker_Buffer3();
		Run_Worker_Buffer4();
		Run_HS1_OnFix();
		Run_HS2_OnFix();
		Run_HS3_OnFix();
		Run_HS4_OnFix();
		break;

	case dREINSTATE :
		Run_Reinstate();
		break;
	
	default:
		if(st_work.mn_run_status == dSTOP)
		{
			OnWorkBufferRbt_FinalPos();//20121126
		}


		//dSTOP, dJAM, dLOTEND, dWARNING, dLOCK, dMAINT (장비가 동작하지 않는다면!)
		if(st_handler.mn_menu_num == 501)  return;   //XYZ Robot motor 화면이면..	
		
/*		COMI.Set_MotStop(1, M_WORK_BUFFER_1);
		COMI.Set_MotStop(1, M_WORK_BUFFER_2);
		COMI.Set_MotStop(1, M_WORK_BUFFER_3);
		COMI.Set_MotStop(1, M_WORK_BUFFER_4);
		CTL_Lib.mn_single_motmove_step[M_WORK_BUFFER_1] = 0;
		CTL_Lib.mn_single_motmove_step[M_WORK_BUFFER_2] = 0;
		CTL_Lib.mn_single_motmove_step[M_WORK_BUFFER_3] = 0;
		CTL_Lib.mn_single_motmove_step[M_WORK_BUFFER_4] = 0;
*/
		InitStep = 0;
		ReinstateStep = 0;
		break;
	}
}

void CRun_Rbt_Work_Buffer::Thread_Variable_Initial()
{
	int i,j;
	InitStep = 0;
	RunBuff1Step = 0;
	RunBuff2Step = 0;
	RunBuff3Step = 0;
	RunBuff4Step = 0;
	HS1Step = 0;
	HS2Step = 0;
	HS3Step = 0;
	HS4Step = 0;

	for (i = 0; i < 4; i++)
	{
		st_modulemap.WorkBuffer[D_EXIST][i][LDMODULE_SITE]	 = DVC_NO;
		st_modulemap.WorkBuffer[D_EXIST][i][LEFTSINK_SITE]	 = DVC_NO;
		st_modulemap.WorkBuffer[D_EXIST][i][RIGHTSINK_SITE] = DVC_NO;
		st_modulemap.WorkBuffer[D_EXIST][i][LDCLIP_SITE]	 = DVC_NO;
		st_modulemap.WorkBuffer[D_EXIST][i][SORTER_SITE]	 = DVC_NO;

		for (j = 0; j < 2; j++)
		{
			st_sync.n_module_site_status[LDMODULE_SITE][i][j] 	 = CTL_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][i][j] 	 = CTL_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][i][j] 	 = CTL_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][i][j] 	 = CTL_NO;
			st_sync.n_module_site_status[SORTER_SITE][i][j] 	 = CTL_NO;
		}
	}

	mn_retry = 0;

	ReinstateStep = 0;
	st_work.mn_WorkBufferRobot_Reinstatement_Ok = NO;
	st_work.mn_WorkBufferRobot_Reinstatement_Ready = NO;


}

void CRun_Rbt_Work_Buffer::Run_Init()
{
	int nRet_1,nRet_2;
	int i;

	if(st_handler.mn_init_state[INIT_WORK_BUFFER] != CTL_NO) return;

	switch(InitStep)
	{
	case 0:
		if(COMI.mn_simulation_mode == 1)
		{
			InitStep = 10;
		}
		else
		{
			if(st_handler.mn_init_state[INIT_RBT_MODULE] == CTL_READY &&
				st_handler.mn_init_state[INIT_RBT_LEFT_SINK] == CTL_READY &&
				st_handler.mn_init_state[INIT_RBT_RIGHT_SINK] == CTL_READY &&
				st_handler.mn_init_state[INIT_RBT_CLIP_MODULE] == CTL_READY &&
				st_handler.mn_init_state[INIT_RBT_SORTER] == CTL_READY)
			{
				mn_retry = 0;
				InitStep = 10;
			}
		}
		break;

	case 10:
		FAS_IO.set_out_bit(st_io.o_buf1_m_clamp_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_buf1_hs_clamp_onoff, IO_OFF);

		for (i = 0; i < 4; i++)
		{
			FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff + (i*100), IO_OFF);
		}

		l_Unitil_WaitTime[0] = GetCurrentTime();
		InitStep = 20;
		break;

	case 20:		
		l_Unitil_WaitTime[1] = GetCurrentTime();
		l_Unitil_WaitTime[2] = l_Unitil_WaitTime[1] - l_Unitil_WaitTime[0];
		if(l_Unitil_WaitTime[2] < 500) break;
		else if(l_Unitil_WaitTime[2] < 0) l_Unitil_WaitTime[0] = GetCurrentTime();
		nRet_1 = FAS_IO.get_in_bit(st_io.i_buf1_m_clamp_on_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_buf1_m_clamp_off_chk, IO_ON);
		if (nRet_1 == IO_OFF && nRet_2 == IO_ON || COMI.mn_simulation_mode == 1) // 20130509
		{
			mn_retry = 0;
			InitStep = 21;
		}
		else
		{//190104 1 19 "Work Buffer_1 로봇 모듈 있음 체크 에러."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"190104");
				CTL_Lib.Alarm_Error_Occurrence(2699, CTL_dWARNING, mc_alarmcode);
				InitStep = 10;
			}
			else
			{
				InitStep = 10;
			}
		}
		break;

	case 21:
		FAS_IO.set_out_bit(st_io.o_buf2_m_clamp_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
		l_Unitil_WaitTime[0] = GetCurrentTime();
		InitStep = 30;
		break;
		
	case 30:
		l_Unitil_WaitTime[1] = GetCurrentTime();
		l_Unitil_WaitTime[2] = l_Unitil_WaitTime[1] - l_Unitil_WaitTime[0];
		if(l_Unitil_WaitTime[2] < 500) break;
		else if(l_Unitil_WaitTime[2] < 0) l_Unitil_WaitTime[0] = GetCurrentTime();
		nRet_1 = FAS_IO.get_in_bit(st_io.i_buf2_m_clamp_on_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_buf2_m_clamp_off_chk, IO_ON);
		if (nRet_1 == IO_OFF && nRet_2 == IO_ON || COMI.mn_simulation_mode == 1) // 20130509
		{
			mn_retry = 0;
			InitStep = 31;
		}
		else
		{//200104 1 19 "Work Buffer_2 로봇 모듈 있음 체크 에러."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"200104");
				CTL_Lib.Alarm_Error_Occurrence(2698, CTL_dWARNING, mc_alarmcode);
			}
			else
			{
				InitStep = 21;
			}
		}
		break;

	case 31:
		FAS_IO.set_out_bit(st_io.o_buf3_m_clamp_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
		l_Unitil_WaitTime[0] = GetCurrentTime();
		InitStep = 40;
		break;		
		
	case 40:
		l_Unitil_WaitTime[1] = GetCurrentTime();
		l_Unitil_WaitTime[2] = l_Unitil_WaitTime[1] - l_Unitil_WaitTime[0];
		if(l_Unitil_WaitTime[2] < 500) break;
		else if(l_Unitil_WaitTime[2] < 0) l_Unitil_WaitTime[0] = GetCurrentTime();
		nRet_1 = FAS_IO.get_in_bit(st_io.i_buf3_m_clamp_on_chk,	IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_buf3_m_clamp_off_chk, IO_ON);
		if (nRet_1 == IO_OFF && nRet_2 == IO_ON || COMI.mn_simulation_mode == 1) // 20130509
		{
			mn_retry = 0;
			InitStep = 41;
		}
		else
		{//210104 1 19 "Work Buffer_3 로봇 모듈 있음 체크 에러."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;				
				sprintf(mc_alarmcode,"210104");
				CTL_Lib.Alarm_Error_Occurrence(2697, CTL_dWARNING, mc_alarmcode);
				InitStep = 31;
			}
			else
			{
				InitStep = 31;
			}
		}
		break;

	case 41:
		FAS_IO.set_out_bit(st_io.o_buf4_m_clamp_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
		l_Unitil_WaitTime[0] = GetCurrentTime();
		InitStep = 50;
		break;		

	case 50:
		l_Unitil_WaitTime[1] = GetCurrentTime();
		l_Unitil_WaitTime[2] = l_Unitil_WaitTime[1] - l_Unitil_WaitTime[0];
		if(l_Unitil_WaitTime[2] < 500) break;
		else if(l_Unitil_WaitTime[2] < 0) l_Unitil_WaitTime[0] = GetCurrentTime();
		nRet_1 = FAS_IO.get_in_bit(st_io.i_buf4_m_clamp_on_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_buf4_m_clamp_off_chk, IO_ON);
		if (nRet_1 == IO_OFF && nRet_2 == IO_ON || COMI.mn_simulation_mode == 1) // 20130509
		{
			mn_retry = 0;
			InitStep = 100;
		}
		else
		{//220104 1 19 "Work Buffer_4 로봇 모듈 있음 체크 에러."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"220104");
				CTL_Lib.Alarm_Error_Occurrence(2696, CTL_dWARNING, mc_alarmcode);
				InitStep = 41;
			}
			else
			{
				InitStep = 41;
			}
		}
		break;


	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			InitStep = 200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			InitStep = 100;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			InitStep = 100;
			CTL_Lib.Alarm_Error_Occurrence(2600, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;
		
	case 200:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			InitStep = 300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			InitStep = 200;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			InitStep = 200;
			CTL_Lib.Alarm_Error_Occurrence(2601, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			InitStep = 400;
		}
		else if(nRet_1 == BD_RETRY)
		{
			InitStep = 300;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			InitStep = 300;
			CTL_Lib.Alarm_Error_Occurrence(2602, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;
		
	case 400:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			InitStep = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			InitStep = 400;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			InitStep = 400;
			CTL_Lib.Alarm_Error_Occurrence(2603, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1000:
		nRet_1 = COMI.HomeCheck_Mot(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 1100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2604, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1100:
		nRet_1 = COMI.HomeCheck_Mot(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].mn_homecheck_method, MOT_TIMEOUT);

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
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2605, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1200:
		nRet_1 = COMI.HomeCheck_Mot(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 1300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1200;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2607, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;
		
	case 1300:
		nRet_1 = COMI.HomeCheck_Mot(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 2000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1300;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2608, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2000:
		InitStep = 0;
		st_handler.mn_init_state[INIT_WORK_BUFFER] = CTL_YES;
		break;
	
	}
}

void CRun_Rbt_Work_Buffer::Run_Worker_Buffer1()
{
	int nRet_1,nRet_2;
	double dCurrPos;
	Func.ThreadFunctionStepTrace(33, RunBuff1Step);
	switch(RunBuff1Step)
	{
	case -1:
		Thread_Variable_Initial();
		RunBuff1Step = 0;
		RunBuff2Step = 0;
		RunBuff3Step = 0;
		RunBuff4Step = 0;
		break;

	case 0:
		RunBuff1Step = 100;
		break;

	case 10:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff1Step = 20;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 10;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunBuff1Step = 10;
			CTL_Lib.Alarm_Error_Occurrence(2670, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 20:
		nRet_1 = COMI.HomeCheck_Mot(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			RunBuff1Step = 100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			RunBuff1Step = 20;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2671, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
// 			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][0] = CTL_FREE;
			RunBuff1Step = 1000;
			}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 100;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunBuff1Step = 100;
			CTL_Lib.Alarm_Error_Occurrence(2610, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;		

	case 1000:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_1][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][0] = CTL_LOCK;
				RunBuff1Step = 2200;

				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff1Step = 1100;
					st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 1;
					l_Work1_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LDMODULE_SITE] WORK BUFFER1 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][0] = CTL_LOCK;
				RunBuff1Step = 3000;
			
				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff1Step = 1200;
					st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 2;
					l_Work1_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LEFTSINK_SITE] WORK BUFFER1 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][0] = CTL_LOCK;
				RunBuff1Step = 4000;

				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff1Step = 1300;
					st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 3;
					l_Work1_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[RIGHTSINK_SITE] WORK BUFFER1 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_1][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1][0] = CTL_LOCK;
				RunBuff1Step = 5000;
				
				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff1Step = 1400;
					st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 4;
					l_Work1_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LDCLIP_SITE] WORK BUFFER1 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_1][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][0] = CTL_LOCK;
				RunBuff1Step = 6000;
				
				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff1Step = 1500;
					st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 5;
					l_Work1_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[SORTER_SITE] WORK BUFFER1 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		break;

	case 1100:
		l_Work1_WaitTime[1] = GetCurrentTime();
		l_Work1_WaitTime[2] = l_Work1_WaitTime[1] - l_Work1_WaitTime[0];
		if(l_Work1_WaitTime[2] < 0)
		{
			l_Work1_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work1_WaitTime[2] > 1000)
		{
			RunBuff1Step = 2200;

		}
		break;

	case 1200:
		l_Work1_WaitTime[1] = GetCurrentTime();
		l_Work1_WaitTime[2] = l_Work1_WaitTime[1] - l_Work1_WaitTime[0];
		if(l_Work1_WaitTime[2] < 0)
		{
			l_Work1_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work1_WaitTime[2] > 1000)
		{
			RunBuff1Step = 3000;
		}
		break;

	case 1300:
		l_Work1_WaitTime[1] = GetCurrentTime();
		l_Work1_WaitTime[2] = l_Work1_WaitTime[1] - l_Work1_WaitTime[0];
		if(l_Work1_WaitTime[2] < 0)
		{
			l_Work1_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work1_WaitTime[2] > 1000)
		{
			RunBuff1Step = 4000;
		}
		break;

	case 1400:
		l_Work1_WaitTime[1] = GetCurrentTime();
		l_Work1_WaitTime[2] = l_Work1_WaitTime[1] - l_Work1_WaitTime[0];
		if(l_Work1_WaitTime[2] < 0)
		{
			l_Work1_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work1_WaitTime[2] > 1000)
		{
			RunBuff1Step = 5000;
		}
		break;

	case 1500:
		l_Work1_WaitTime[1] = GetCurrentTime();
		l_Work1_WaitTime[2] = l_Work1_WaitTime[1] - l_Work1_WaitTime[0];
		if(l_Work1_WaitTime[2] < 0)
		{
			l_Work1_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work1_WaitTime[2] > 1000)
		{
			RunBuff1Step = 6000;
		}
		break;


//	case 2000:
//		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
//		if(nRet_1 == BD_GOOD)
//		{
//			RunBuff1Step = 2200;
//		}
//		else if(nRet_1 == BD_RETRY)
//		{
//			RunBuff1Step = 2000;
//		}
//		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
//		{
//			RunBuff1Step = 2000;
//			st_work.mn_run_status = CTL_dWARNING;
//			CTL_Lib.Alarm_Error_Occurrence(2611, CTL_dWARNING, COMI.mc_alarmcode);
//		}
//		break;

	case 2200:
		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][1] = CTL_READY;
		RunBuff1Step = 2300;
		break;

	case 2300:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf1_m_clamp_onoff, IO_ON);
			RunBuff1Step = 2400;
		}
//		else
//		{
//			RunBuff1Step = 2200;
//		}
		break;

	case 2310:
		FAS_IO.OnCylinderAction(st_io.o_buf1_m_clamp_onoff, IO_ON);
		RunBuff1Step = 2400;
		break;

	case 2400:		
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf1_m_clamp_onoff, st_io.i_buf1_m_clamp_off_chk, st_io.i_buf1_m_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[72], st_wait.n_limit_wait_time[72]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff1Step = 2500;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550804 1 55 "Module Buffer#1 UnClamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"550804");
				CTL_Lib.Alarm_Error_Occurrence(2612, CTL_dWARNING, mc_alarmcode);
				RunBuff1Step = 2310;
			}
			else
			{
				RunBuff1Step = 2310;
			}
		}
		break;

	case 2500:
		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][1] = CTL_POSSIBLE;
		RunBuff1Step = 2600;
		break;

	case 2600:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][1] == CTL_COMPLETE)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf1_m_clamp_onoff, IO_OFF);
			RunBuff1Step = 2700;
		}
		break;

	case 2610:
		FAS_IO.OnCylinderAction(st_io.o_buf1_m_clamp_onoff, IO_OFF);
		RunBuff1Step = 2700;
		break;

	case 2700:
		if(st_basic.n_mode_device != 1 || (FAS_IO.get_in_bit(st_io.i_buf1_m_clamp_off_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_buf1_m_clamp_on_chk, IO_ON) == IO_ON))
		{
			RunBuff1Step = 2800;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf1_m_clamp_onoff, st_io.i_buf1_m_clamp_on_chk, st_io.i_buf1_m_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[72], st_wait.n_limit_wait_time[72]);
			nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_buf1_m_clamp_onoff, st_io.i_buf1_m_clamp_off_chk, st_io.i_buf1_m_clamp_on_chk,
				IO_ON, st_wait.n_on_wait_time[72], st_wait.n_limit_wait_time[72]);
			if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
			{
				RunBuff1Step = 2800;
			}
			else if (nRet_1 != RET_PROCEED)
			{//550800 1 55 "Module Buffer#1 Clamp 에러"
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550800");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2613, st_work.mn_run_status, mc_alarmcode);
					RunBuff1Step = 2610;
				}
				else
				{
					RunBuff1Step = 2610;
				}
			}
		}
		break;

	case 2800:
 //		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][1] = CTL_BUSY;
		RunBuff1Step = 2900;
		break;

	case 2900:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][1] == CTL_CHANGE)
		{
			RunBuff1Step = 2910;
		}
		break;

	case 2910:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDLEFTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
// 			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][1] = CTL_NO;
			FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff, IO_OFF);
			RunBuff1Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 2910;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff1Step = 2910;
			CTL_Lib.Alarm_Error_Occurrence(2613, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_1);
		if(dCurrPos > st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDLEFTHS] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDLEFTHS] + 0.05)
		{
			RunBuff1Step = 3100;
		}
		else
		{
			RunBuff1Step = 3005;
		}
		break;

	case 3005:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDLEFTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff, IO_OFF);
			RunBuff1Step = 3100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff1Step = 3000;
			CTL_Lib.Alarm_Error_Occurrence(2614, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3010:
		FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff, IO_OFF);
		RunBuff1Step = 3100;
		break;

	case 3100:
		nRet_1= FAS_IO.OnCylinderActionCheck(st_io.o_buf1_hs_clamp_onoff, st_io.i_buf1_hs_clamp_on_chk, st_io.i_buf1_hs_clamp_off_chk, 
			IO_ON, st_wait.n_on_wait_time[73], st_wait.n_limit_wait_time[73]);
		if(nRet_1 == RET_GOOD || st_basic.n_mode_device != 1)
		{
			RunBuff1Step = 3200;
		}
		else if(nRet_1 != RET_PROCEED)
		{//550810 1 55 "Heat Sink Buffer#1 Clamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"550810");
				CTL_Lib.Alarm_Error_Occurrence(2615, CTL_dWARNING, mc_alarmcode);
				RunBuff1Step = 3010;
			}
			else
			{
				RunBuff1Step = 3010;
			}
		}
		break;

	case 3200:
		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][1] = CTL_READY;
		RunBuff1Step = 3300;
		break;

	case 3300:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff, IO_OFF);
			RunBuff1Step = 3400;
		}
//		else
//		{
//			RunBuff1Step = 3300;
//		}
		break;

	case 3310:
		FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff, IO_OFF);
		RunBuff1Step = 3400;
		break;

	case 3400:
		if(FAS_IO.get_in_bit(st_io.i_buf1_hs_clamp_off_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_buf1_hs_clamp_on_chk, IO_ON) == IO_ON)
		{
			RunBuff1Step = 3500;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf1_hs_clamp_onoff, st_io.i_buf1_hs_clamp_on_chk, st_io.i_buf1_hs_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[73], st_wait.n_limit_wait_time[73]);
			if(nRet_1 == RET_GOOD)
			{
				RunBuff1Step = 3500;
			}
			else if (nRet_1 != RET_PROCEED)
			{//550810 1 55 "Heat Sink Buffer#1 Clamp 에러"
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550810");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2616, st_work.mn_run_status, mc_alarmcode);
					RunBuff1Step = 3310;
				}
				else
				{
					RunBuff1Step = 3310;
				}
			}
			
		}
		break;

	case 3500:
		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][1] = CTL_POSSIBLE;
		RunBuff1Step = 3600;
		break;

	case 3600:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][1] == CTL_COMPLETE)
		{
			RunBuff1Step = 3800;
		}
		break;

	case 3800:
// 		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][1] = CTL_BUSY;
		RunBuff1Step = 3900;
		break;

	case 3900:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][1] == CTL_CHANGE)
		{
			RunBuff1Step = 3910;
		}
		break;

	case 3910:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDRIGHTTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][1] = CTL_NO;
			RunBuff1Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 3910;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff1Step = 3910;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2616, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_1);
		if(dCurrPos > st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDRIGHTTHS] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDRIGHTTHS] + 0.05)
		{
			RunBuff1Step = 4200;
		}
		else
		{
			RunBuff1Step = 4100;
		}
		break;

	case 4100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDRIGHTTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff1Step = 4200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 4000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff1Step = 4000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2617, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4200:
		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][1] = CTL_READY;
		RunBuff1Step = 4300;
		break;

	case 4300:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff, IO_OFF);
			RunBuff1Step = 4400;
		}
		break;

	case 4310:
		FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff, IO_OFF);
		RunBuff1Step = 4400;
		break;

	case 4400:		
		if(FAS_IO.get_in_bit(st_io.i_buf1_hs_clamp_off_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_buf1_hs_clamp_on_chk, IO_ON) == IO_ON)
		{
			RunBuff1Step = 4500;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf1_hs_clamp_onoff, st_io.i_buf1_hs_clamp_on_chk, st_io.i_buf1_hs_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[73], st_wait.n_limit_wait_time[73]);
			if(nRet_1 == RET_GOOD)
			{
				RunBuff1Step = 4500;
			}
			else if (nRet_1 != RET_PROCEED)
			{//550810 1 55 "Heat Sink Buffer#1 Clamp 에러"
				mn_retry++;
				if (mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550810");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2618, st_work.mn_run_status, mc_alarmcode);
					RunBuff1Step = 4310;
				}
				else
				{
					RunBuff1Step = 4310;
				}
			}
		}
		break;

	case 4500:
		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][1] = CTL_POSSIBLE;
		RunBuff1Step = 4600;
		break;

	case 4600:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][1] == CTL_COMPLETE)
		{
			FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff, IO_OFF);
			RunBuff1Step = 4700;
		}
		break;

	case 4700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf1_hs_clamp_onoff, st_io.i_buf1_hs_clamp_on_chk, st_io.i_buf1_hs_clamp_off_chk,
			IO_ON, st_wait.n_on_wait_time[73], st_wait.n_limit_wait_time[73]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff1Step = 4800;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550814 1 55 "Heat Sink Buffer#1 UnClamp 에러"
			sprintf(mc_alarmcode,"550814");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2619, st_work.mn_run_status, mc_alarmcode);
			RunBuff1Step = 4600;
		}
		break;

	case 4800:
// 		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][1] = CTL_BUSY;
		RunBuff1Step = 4900;
		break;

	case 4900:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][1] == CTL_CHANGE)
		{
			RunBuff1Step = 4920;
			//RunBuff1Step = 4910;
		}
		break;

	case 4910:
		if(st_basic.n_mode_device !=0)
		{
			if(FAS_IO.get_in_bit(st_io.i_buf1_m_clamp_off_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_buf1_m_clamp_on_chk, IO_ON) == IO_ON)
			{
				RunBuff1Step = 4920;
			}
			else
			{//550800 1 55 "Module Buffer#1 Clamp 에러"
				sprintf(mc_alarmcode,"550800");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2625, st_work.mn_run_status, mc_alarmcode);
				RunBuff1Step = 4910;
			}
		}
		else
		{
			RunBuff1Step = 4920;
		}
		break;

	case 4920:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDCLIP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1][0] = CTL_FREE;  // 20130416*/
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][1] = CTL_NO;
			st_handler.n_hs1_fix = CTL_YES;
			RunBuff1Step = 1000;
//			RunBuff1Step = 4930;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 4920;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff1Step = 4920;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2626, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4930:
		FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff, IO_ON);//2012,0821
		l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
		RunBuff1Step = 4940;
		break;

	case 4940:
		l_ClipWaitTime[WORK_BUFFER_1][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_1][2] = l_ClipWaitTime[WORK_BUFFER_1][1] - l_ClipWaitTime[WORK_BUFFER_1][0];
		if(l_ClipWaitTime[WORK_BUFFER_1][2] < 0) l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_1][2] > 800)
		{
			FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
			RunBuff1Step = 4950;
		}
		break;

	case 4950:
		l_ClipWaitTime[WORK_BUFFER_1][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_1][2] = l_ClipWaitTime[WORK_BUFFER_1][1] - l_ClipWaitTime[WORK_BUFFER_1][0];
		if(l_ClipWaitTime[WORK_BUFFER_1][2] < 0) l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_1][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff, IO_ON);//2012,0821
			l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
			RunBuff1Step = 4960;
		}
		break;

	case 4960:
		l_ClipWaitTime[WORK_BUFFER_1][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_1][2] = l_ClipWaitTime[WORK_BUFFER_1][1] - l_ClipWaitTime[WORK_BUFFER_1][0];
		if(l_ClipWaitTime[WORK_BUFFER_1][2] < 0) l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_1][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
			RunBuff1Step = 4970;
		}
		break;

	case 4970:
		l_ClipWaitTime[WORK_BUFFER_1][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_1][2] = l_ClipWaitTime[WORK_BUFFER_1][1] - l_ClipWaitTime[WORK_BUFFER_1][0];
		if(l_ClipWaitTime[WORK_BUFFER_1][2] < 0) l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_1][2] > 200)
		{
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][1] = CTL_NO;
			RunBuff1Step = 1000;
		}
		break;

	case 5000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_1);
		if(dCurrPos > st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDCLIP] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDCLIP] + 0.05)
		{
			RunBuff1Step = 5200;
		}
		else
		{
			RunBuff1Step = 5100;
		}
		break;

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDCLIP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff1Step = 5200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 5000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff1Step = 5000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2620, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 5200:
		if(st_handler.n_hs1_fix == CTL_NO)
		{
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1][1] = CTL_READY;
			RunBuff1Step = 5300;
		}
		break;

	case 5300:
		if(st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1][1] == CTL_CHANGE)
		{
//			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1][1] = CTL_NO;
			RunBuff1Step = 5400;
		}
		break;

	case 5400:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_ULDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_1] = NO;
//			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][1] = CTL_READY;
			RunBuff1Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 5400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff1Step = 5400;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2621, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 6000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_1);
		if(dCurrPos > st_motor[M_WORK_BUFFER_1].md_pos[WORK_ULDMODULE] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_1].md_pos[WORK_ULDMODULE] + 0.05)
		{
			RunBuff1Step = 6100;
		}
		else
		{
			RunBuff1Step = 6010;
		}
		break;

	case 6010:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_ULDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_1] = NO;
			RunBuff1Step = 6100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 6000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff1Step = 6000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2622, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 6100:	
//		st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][1] = CTL_READY;
		RunBuff1Step = 6200;
		break;

	case 6200:
		if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf1_m_clamp_onoff, IO_ON);
			RunBuff1Step = 6300;
		}
		break;
		
	case 6210:
		FAS_IO.OnCylinderAction(st_io.o_buf1_m_clamp_onoff, IO_ON);
		RunBuff1Step = 6300;
		break;

	case 6300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf1_m_clamp_onoff, st_io.i_buf1_m_clamp_off_chk, st_io.i_buf1_m_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[72], st_wait.n_limit_wait_time[72]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff1Step = 6400;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550810 1 55 "Heat Sink Buffer#1 Clamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				sprintf(mc_alarmcode,"550810");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2623, st_work.mn_run_status, mc_alarmcode);
				RunBuff1Step = 6210;
			}
			else
			{
				RunBuff1Step = 6210;
			}
		}
		break;

	case 6400:
		st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][1] = CTL_POSSIBLE;
		RunBuff1Step = 6500;
		break;

	case 6500:
		if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][1] == CTL_CHANGE)
		{
			//st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][0] = CTL_FREE;
			//st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][1] = CTL_NO;
			RunBuff1Step = 6600;
		}
		break;

	case 6600:
//		if(st_sync.i_WorkClamp_Clip[WORK_BUFFER_1] == NO && st_sync.i_WorkClamp_Clip[WORK_BUFFER_2] == NO)
//		{
////			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_1] = YES;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][1] = CTL_NO;
			FAS_IO.set_out_bit(st_io.o_buf1_m_clamp_onoff, IO_ON);
			FAS_IO.set_out_bit(st_io.o_buf1_hs_clamp_onoff, IO_OFF);

			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1][LDMODULE_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1][LEFTSINK_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1][RIGHTSINK_SITE] = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1][LDCLIP_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1][SORTER_SITE]	 = DVC_NO;

			st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_1][0] 	 = CTL_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1][0] 	 = CTL_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1][0] 	 = CTL_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_1][0] 	 = CTL_NO;
			st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_1][0] 	 = CTL_NO;

			st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_1][1] 	 = CTL_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1][1] 	 = CTL_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1][1] 	 = CTL_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_1][1] 	 = CTL_NO;
			st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_1][1] 	 = CTL_NO;

			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][0] = CTL_NO;

			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][1] = CTL_NO;

			RunBuff1Step = 6700;
//		}
		break;

	case 6700:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_1] = NO;
			RunBuff1Step = 100;
			if(COMI.mn_simulation_mode == 1)
			{
				st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 1;
			}
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff1Step = 6700;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunBuff1Step = 6700;
			CTL_Lib.Alarm_Error_Occurrence(2624, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;		
	}
}

void CRun_Rbt_Work_Buffer::Run_Worker_Buffer2()
{
	int nRet_1,nRet_2;
	double dCurrPos;
	Func.ThreadFunctionStepTrace(34, RunBuff2Step);
	switch(RunBuff2Step)
	{
	case 0:
		RunBuff2Step = 100;
		break;

	case 10:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff2Step = 20;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 10;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunBuff2Step = 10;
			CTL_Lib.Alarm_Error_Occurrence(2672, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 20:
		nRet_1 = COMI.HomeCheck_Mot(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			RunBuff2Step = 100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			RunBuff2Step = 20;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2673, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
		
	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
// 			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][0] = CTL_FREE;
			RunBuff2Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff2Step = 100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2630, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;	

	case 1000:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_2][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][0] = CTL_LOCK;
				RunBuff2Step = 2200;				

				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff2Step = 1100;
					st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 1;
					l_Work2_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LDMODULE_SITE] WORK BUFFER2 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_2][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][0] = CTL_LOCK;
				RunBuff2Step = 3000;

				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff2Step = 1200;
					st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 2;
					l_Work2_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LEFTSINK_SITE] WORK BUFFER2 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_2][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][0] = CTL_LOCK;
				RunBuff2Step = 4000;

				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff2Step = 1300;
					st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 3;
					l_Work2_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[RIGHTSINK_SITE] WORK BUFFER2 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_2][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_2][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_2][0] = CTL_LOCK;
				RunBuff2Step = 5000;

				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff2Step = 1400;
					st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 4;
					l_Work2_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LDCLIP_SITE] WORK BUFFER2 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_2][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][0] = CTL_LOCK;
				RunBuff2Step = 6000;
	
				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff2Step = 1500;
					st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 5;
					l_Work2_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[SORTER_SITE] WORK BUFFER2 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		break;

	case 1100:
		l_Work2_WaitTime[1] = GetCurrentTime();
		l_Work2_WaitTime[2] = l_Work2_WaitTime[1] - l_Work2_WaitTime[0];
		if(l_Work2_WaitTime[2] < 0)
		{
			l_Work2_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work2_WaitTime[2] > 1000)
		{
			RunBuff2Step = 2200;
		}
		break;
		
	case 1200:
		l_Work2_WaitTime[1] = GetCurrentTime();
		l_Work2_WaitTime[2] = l_Work2_WaitTime[1] - l_Work2_WaitTime[0];
		if(l_Work2_WaitTime[2] < 0)
		{
			l_Work2_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work2_WaitTime[2] > 1000)
		{
			RunBuff2Step = 3000;
		}
		break;
		
	case 1300:
		l_Work2_WaitTime[1] = GetCurrentTime();
		l_Work2_WaitTime[2] = l_Work2_WaitTime[1] - l_Work2_WaitTime[0];
		if(l_Work2_WaitTime[2] < 0)
		{
			l_Work2_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work2_WaitTime[2] > 1000)
		{
			RunBuff2Step = 4000;
		}
		break;
		
	case 1400:
		l_Work2_WaitTime[1] = GetCurrentTime();
		l_Work2_WaitTime[2] = l_Work2_WaitTime[1] - l_Work2_WaitTime[0];
		if(l_Work2_WaitTime[2] < 0)
		{
			l_Work2_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work2_WaitTime[2] > 1000)
		{
			RunBuff2Step = 5000;
		}
		break;
		
	case 1500:
		l_Work2_WaitTime[1] = GetCurrentTime();
		l_Work2_WaitTime[2] = l_Work2_WaitTime[1] - l_Work2_WaitTime[0];
		if(l_Work2_WaitTime[2] < 0)
		{
			l_Work2_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work2_WaitTime[2] > 1000)
		{
			RunBuff2Step = 6000;
		}
		break;

//	case 2000:
//		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
//		if(nRet_1 == BD_GOOD)
//		{
//			RunBuff2Step = 2200;
//			l_Unitil_WaitTime[0] = GetCurrentTime();
//
//		}
//		else if(nRet_1 == BD_RETRY)
//		{
//			RunBuff2Step = 2000;
//		}
//		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
//		{
//			RunBuff2Step = 2000;
//			st_work.mn_run_status = CTL_dWARNING;
//			CTL_Lib.Alarm_Error_Occurrence(2631, st_work.mn_run_status, COMI.mc_alarmcode);
//		}
//		break;

	case 2200://20120816
//		l_Unitil_WaitTime[1] = GetCurrentTime();
//		l_Unitil_WaitTime[2] = l_Unitil_WaitTime[1] - l_Unitil_WaitTime[0];
//		if(l_Unitil_WaitTime[2] > PICKERSTATUS_CHECK_TIME*5) //자재를 체크할 수 있는 안정 시간을 준다  
//		{
		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][1] = CTL_READY;
		RunBuff2Step = 2300;
//		}	
//		else if(l_Unitil_WaitTime[2] < 0)
//		{
//			l_Unitil_WaitTime[0] = GetCurrentTime();
//		}
//		break;

	case 2300:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][1] == CTL_DOREADY)
		{			
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf2_m_clamp_onoff, IO_ON);
			RunBuff2Step = 2400;
		}
//		else
//		{
//			RunBuff2Step = 2200;
//		}
		break;

	case 2310:
		FAS_IO.OnCylinderAction(st_io.o_buf2_m_clamp_onoff, IO_ON);
		RunBuff2Step = 2400;
		break;

	case 2400:		
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf2_m_clamp_onoff, st_io.i_buf2_m_clamp_off_chk, st_io.i_buf2_m_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[76], st_wait.n_limit_wait_time[76]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff2Step = 2500;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550805 1 55 "Module Buffer#2 UnClamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"550805");
				CTL_Lib.Alarm_Error_Occurrence(2632, CTL_dWARNING, mc_alarmcode);
				RunBuff2Step = 2310;
			}
			else
			{
				RunBuff2Step = 2310;
			}
		}
		break;

	case 2500:
		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][1] = CTL_POSSIBLE;
		RunBuff2Step = 2600;
		break;

	case 2600:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][1] == CTL_COMPLETE)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf2_m_clamp_onoff, IO_OFF);
			RunBuff2Step = 2700;
		}
		break;

	case 2610:
		FAS_IO.OnCylinderAction(st_io.o_buf2_m_clamp_onoff, IO_OFF);
		RunBuff2Step = 2700;
		break;

	case 2700:
		if(st_basic.n_mode_device != 1 || (FAS_IO.get_in_bit(st_io.i_buf2_m_clamp_off_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_buf2_m_clamp_on_chk, IO_ON) == IO_ON))
		{
			RunBuff2Step = 2800;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf2_m_clamp_onoff, st_io.i_buf2_m_clamp_on_chk, st_io.i_buf2_m_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[76], st_wait.n_limit_wait_time[76]);
			nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_buf2_m_clamp_onoff, st_io.i_buf2_m_clamp_off_chk, st_io.i_buf2_m_clamp_on_chk,
				IO_ON, st_wait.n_on_wait_time[76], st_wait.n_limit_wait_time[76]);
			if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
			{
				RunBuff2Step = 2800;
			}
			else if (nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
			{//550801 1 55 "Module Buffer#2 Clamp 에러"
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550801");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2633, st_work.mn_run_status, mc_alarmcode);
					RunBuff2Step = 2610;
				}
				else
				{
					RunBuff2Step = 2610;
				}
			}
		}
		break;

	case 2800:
// 		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][1] = CTL_BUSY;
		RunBuff2Step = 2900;
		break;

	case 2900:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][1] == CTL_CHANGE)
		{
			RunBuff2Step = 2910;
		}
		break;

	case 2910:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDLEFTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
// 			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][1] = CTL_NO;
			FAS_IO.OnCylinderAction(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
			RunBuff2Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 2910;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff2Step = 2910;
			CTL_Lib.Alarm_Error_Occurrence(2633, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_2);
		if(dCurrPos > st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDLEFTHS] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDLEFTHS] + 0.05)
		{
			RunBuff2Step = 3100;
		}
		else
		{
			RunBuff2Step = 3005;
		}
		break;

	case 3005:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDLEFTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
			RunBuff2Step = 3100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff2Step = 3000;
			CTL_Lib.Alarm_Error_Occurrence(2634, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3010:
		FAS_IO.OnCylinderAction(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
		RunBuff2Step = 3100;
		break;

	case 3100:
		nRet_1= FAS_IO.OnCylinderActionCheck(st_io.o_buf2_hs_clamp_onoff, st_io.i_buf2_hs_clamp_on_chk, st_io.i_buf2_hs_clamp_off_chk, 
			IO_ON, st_wait.n_on_wait_time[77], st_wait.n_limit_wait_time[77]);
		if(nRet_1 == RET_GOOD || st_basic.n_mode_device != 1)
		{
			RunBuff2Step = 3200;
		}
		else if(nRet_1 != RET_PROCEED)
		{//550811 1 55 "Heat Sink Buffer#1 Clamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"550811");
				CTL_Lib.Alarm_Error_Occurrence(2635, CTL_dWARNING, mc_alarmcode);
				RunBuff2Step = 3010;
			}
			else
			{
				RunBuff2Step = 3010;
			}
		}
		break;
	
	case 3200:
		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][1] = CTL_READY;
		RunBuff2Step = 3300;
		break;

	case 3300:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
			RunBuff2Step = 3400;
		}
//		else
//		{
//			RunBuff1Step = 3200;
//		}
		break;

	case 3310:
		FAS_IO.OnCylinderAction(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
		RunBuff2Step = 3400;
		break;

	case 3400:		
		if(FAS_IO.get_in_bit(st_io.i_buf2_hs_clamp_off_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_buf2_hs_clamp_on_chk, IO_ON) == IO_ON)
		{
			RunBuff2Step = 3500;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf2_hs_clamp_onoff, st_io.i_buf2_hs_clamp_off_chk, st_io.i_buf2_hs_clamp_on_chk,
				IO_ON, st_wait.n_on_wait_time[77], st_wait.n_limit_wait_time[77]);
			if(nRet_1 == RET_GOOD)
			{
				RunBuff2Step = 3500;
			}
			else if (nRet_1 != RET_PROCEED)
			{//550811 1 55 "Heat Sink Buffer#2 Clamp 에러"
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550811");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2636, st_work.mn_run_status, mc_alarmcode);
					RunBuff2Step = 3310;
				}
				else
				{
					RunBuff2Step = 3310;
				}
			}
		}
		break;

	case 3500:
		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][1] = CTL_POSSIBLE;
		RunBuff2Step = 3600;
		break;

	case 3600:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][1] == CTL_COMPLETE)
		{
			RunBuff2Step = 3800;
		}
		break;

	case 3800:
// 		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][1] = CTL_BUSY;
		RunBuff2Step = 3900;
		break;

	case 3900:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][1] == CTL_CHANGE)
		{
			RunBuff2Step = 3910;
		}
		break;

	case 3910:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDRIGHTTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][1] = CTL_NO;
			RunBuff2Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 3910;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff2Step = 3910;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2636, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_2);
		if(dCurrPos > st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDRIGHTTHS] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDRIGHTTHS] + 0.05)
		{
			RunBuff2Step = 4200;
		}
		else
		{
			RunBuff2Step = 4100;
		}
		break;

	case 4100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDRIGHTTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff2Step = 4200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 4000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff2Step = 4000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2637, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4200:
		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][1] = CTL_READY;
		RunBuff2Step = 4300;
		break;

	case 4300:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
			RunBuff2Step = 4400;
		}
//		else
//		{
//			RunBuff2Step = 4200;
//		}
		break;

	case 4310:
		FAS_IO.OnCylinderAction(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
		RunBuff2Step = 4400;
		break;

	case 4400:		
		if(FAS_IO.get_in_bit(st_io.i_buf2_hs_clamp_off_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_buf2_hs_clamp_on_chk, IO_ON) == IO_ON)
		{
			RunBuff2Step = 4500;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf2_hs_clamp_onoff, st_io.i_buf2_hs_clamp_on_chk, st_io.i_buf2_hs_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[77], st_wait.n_limit_wait_time[77]);
			if(nRet_1 == RET_GOOD)
			{
				RunBuff2Step = 4500;
			}
			else if (nRet_1 != RET_PROCEED)
			{//550811 1 55 "Heat Sink Buffer#2 Clamp 에러"
				mn_retry++;
				if (mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550811");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2638, st_work.mn_run_status, mc_alarmcode);
					RunBuff2Step = 4310;
				}
				else
				{
					RunBuff2Step = 4310;
				}
			}
		}
		break;

	case 4500:
		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][1] = CTL_POSSIBLE;
		RunBuff2Step = 4600;
		break;

	case 4600:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][1] == CTL_COMPLETE)
		{
			FAS_IO.OnCylinderAction(st_io.o_buf2_hs_clamp_onoff, IO_OFF);
			RunBuff2Step = 4700;
		}
		break;

	case 4700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf2_m_clamp_onoff, st_io.i_buf2_hs_clamp_on_chk, st_io.i_buf2_hs_clamp_off_chk,
			IO_ON, st_wait.n_on_wait_time[76], st_wait.n_limit_wait_time[76]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff2Step = 4800;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550815 1 55 "Heat Sink Buffer#2 UnClamp 에러"
			sprintf(mc_alarmcode,"550815");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2639, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 4800:
// 		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][1] = CTL_BUSY;
		RunBuff2Step = 4900;
		break;

	case 4900:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][1] == CTL_CHANGE)
		{
//			RunBuff2Step = 1000;
			RunBuff2Step = 4910;
		}
		break;

	case 4910:
		if(st_basic.n_mode_device !=0)
		{
			if(FAS_IO.get_in_bit(st_io.i_buf2_m_clamp_off_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_buf2_m_clamp_on_chk, IO_ON) == IO_ON)
			{
				RunBuff2Step = 4920;
			}
			else
			{//550801 1 55 "Module Buffer#2 Clamp 에러"
				sprintf(mc_alarmcode,"550801");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2635, st_work.mn_run_status, mc_alarmcode);
				RunBuff2Step = 4910;
			}
		}
		else
		{
			RunBuff2Step = 4920;
		}
		break;

	case 4920:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDCLIP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_2][0] = CTL_FREE; // 20130416
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][1] = CTL_NO;
			st_handler.n_hs2_fix = CTL_YES;
			RunBuff2Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 4920;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff2Step = 4920;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2636, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4930:
		FAS_IO.set_out_bit(st_io.o_buf2_f_hs_onoff, IO_ON);//2012,0821
		l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
		RunBuff2Step = 4940;
		break;

	case 4940:
		l_ClipWaitTime[WORK_BUFFER_2][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_2][2] = l_ClipWaitTime[WORK_BUFFER_2][1] - l_ClipWaitTime[WORK_BUFFER_2][0];
		if(l_ClipWaitTime[WORK_BUFFER_2][2] < 0) l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_2][2] > 800)
		{
			FAS_IO.set_out_bit(st_io.o_buf2_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
			RunBuff2Step = 4950;
		}
		break;

	case 4950:
		l_ClipWaitTime[WORK_BUFFER_2][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_2][2] = l_ClipWaitTime[WORK_BUFFER_2][1] - l_ClipWaitTime[WORK_BUFFER_2][0];
		if(l_ClipWaitTime[WORK_BUFFER_2][2] < 0) l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_2][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf2_f_hs_onoff, IO_ON);//2012,0821
			l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
			RunBuff2Step = 4960;
		}
		break;

	case 4960:
		l_ClipWaitTime[WORK_BUFFER_2][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_2][2] = l_ClipWaitTime[WORK_BUFFER_2][1] - l_ClipWaitTime[WORK_BUFFER_2][0];
		if(l_ClipWaitTime[WORK_BUFFER_2][2] < 0) l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_2][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf2_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
			RunBuff2Step = 4970;
		}
		break;

	case 4970:
		l_ClipWaitTime[WORK_BUFFER_2][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_2][2] = l_ClipWaitTime[WORK_BUFFER_2][1] - l_ClipWaitTime[WORK_BUFFER_2][0];
		if(l_ClipWaitTime[WORK_BUFFER_2][2] < 0) l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_2][2] > 200)
		{
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_2][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][1] = CTL_NO;
			RunBuff2Step = 1000;
		}
		break;

	case 5000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_2);
		if(dCurrPos > st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDCLIP] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDCLIP] + 0.05)
		{
			RunBuff2Step = 5200;
		}
		else
		{
			RunBuff2Step = 5100;
		}
		break;
		
	case 5100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDCLIP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff2Step = 5200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 5000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff2Step = 5000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2640, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 5200:
		if(st_handler.n_hs2_fix == CTL_NO)
		{
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_2][1] = CTL_READY;
			RunBuff2Step = 5300;
		}
		break;

	case 5300:
		if(st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_2][1] == CTL_CHANGE)
		{
//			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_2][1] = CTL_NO;
			RunBuff2Step = 5400;
		}
		break;

	case 5400:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_ULDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_2] = NO;
//			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][1] = CTL_READY;
			RunBuff2Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 5400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff2Step = 5400;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2641, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 6000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_2);
		if(dCurrPos > st_motor[M_WORK_BUFFER_2].md_pos[WORK_ULDMODULE] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_2].md_pos[WORK_ULDMODULE] + 0.05)
		{
			RunBuff2Step = 6100;
		}
		else
		{
			RunBuff2Step = 6010;
		}
		break;

	case 6010:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_ULDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_2] = NO;
			RunBuff2Step = 6100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 6000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff2Step = 6000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2642, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 6100:
//		st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][1] = CTL_READY;
		RunBuff2Step = 6200;
		break;

	case 6200:
		if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf2_m_clamp_onoff, IO_ON);
			RunBuff2Step = 6300;
		}
		break;
		
	case 6210:
		FAS_IO.OnCylinderAction(st_io.o_buf2_m_clamp_onoff, IO_ON);
		RunBuff2Step = 6300;
		break;

	case 6300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf2_m_clamp_onoff, st_io.i_buf2_m_clamp_off_chk, st_io.i_buf2_m_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[76], st_wait.n_limit_wait_time[76]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff2Step = 6400;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550810 1 55 "Heat Sink Buffer#1 Clamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				sprintf(mc_alarmcode,"550811");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2643, st_work.mn_run_status, mc_alarmcode);
				RunBuff2Step = 6210;
			}
			else
			{
				RunBuff2Step = 6210;
			}
		}
		break;

	case 6400:
		st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][1] = CTL_POSSIBLE;
		RunBuff2Step = 6500;
		break;

	case 6500:
		if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][1] == CTL_CHANGE)
		{
			//st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][0] = CTL_FREE;
			//st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][1] = CTL_NO;
			RunBuff2Step = 6600;
		}
		break;

	case 6600:
//		if(st_sync.i_WorkClamp_Clip[WORK_BUFFER_1] == NO && st_sync.i_WorkClamp_Clip[WORK_BUFFER_2] == NO && st_sync.i_WorkClamp_Clip[WORK_BUFFER_3] == NO)
//		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_2] = YES;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][1] = CTL_NO;
			FAS_IO.set_out_bit(st_io.o_buf2_m_clamp_onoff, IO_ON);
			FAS_IO.set_out_bit(st_io.o_buf2_hs_clamp_onoff, IO_OFF);

			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_2][LDMODULE_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_2][LEFTSINK_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_2][RIGHTSINK_SITE] = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_2][LDCLIP_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_2][SORTER_SITE]	 = DVC_NO;

			st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_2][0] 	 = CTL_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_2][0] 	 = CTL_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_2][0] 	 = CTL_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_2][0] 	 = CTL_NO;
			st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_2][0] 	 = CTL_NO;

			st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_2][1] 	 = CTL_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_2][1] 	 = CTL_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_2][1] 	 = CTL_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_2][1] 	 = CTL_NO;
			st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_2][1] 	 = CTL_NO;

			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_2][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][0] = CTL_NO;

			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_2][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_2][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_2][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_2][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_2][1] = CTL_NO;
			RunBuff2Step = 6700;
//		}
		break;

	case 6700:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_2] = NO;
			if(COMI.mn_simulation_mode == 1)
			{
				st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 1;
			}

			RunBuff2Step = 100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff2Step = 6700;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunBuff2Step = 6700;
			CTL_Lib.Alarm_Error_Occurrence(2644, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;	
	}
}

void CRun_Rbt_Work_Buffer::Run_Worker_Buffer3()
{
	int nRet_1,nRet_2;
	double dCurrPos;
	Func.ThreadFunctionStepTrace(35, RunBuff3Step);
	switch(RunBuff3Step)
	{
	case 0:
		RunBuff3Step = 100;
		break;

	case 10:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff3Step = 20;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 10;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunBuff3Step = 10;
			CTL_Lib.Alarm_Error_Occurrence(2674, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 20:
		nRet_1 = COMI.HomeCheck_Mot(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			RunBuff3Step = 100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			RunBuff3Step = 20;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2675, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
	
	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
// 			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][0] = CTL_FREE;
			RunBuff3Step = 1000;

		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff3Step = 100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2650, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;		

	case 1000:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_3][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][0] = CTL_LOCK;
				RunBuff3Step = 2200;

				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff3Step = 1100;
					st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 1;
					l_Work3_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LDMODULE_SITE] WORK BUFFER3 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_3][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][0] = CTL_LOCK;
				RunBuff3Step = 3000;
	
				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff3Step = 1200;
					st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 2;
					l_Work3_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LEFTSINK_SITE] WORK BUFFER3 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_3][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][0] = CTL_LOCK;
				RunBuff3Step = 4000;
			
				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff3Step = 1300;
					st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 3;
					l_Work3_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[RIGHTSINK_SITE] WORK BUFFER3 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_3][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_3][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_3][0] = CTL_LOCK;
				RunBuff3Step = 5000;
				
				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff3Step = 1400;
					st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 4;
					l_Work3_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LDCLIP_SITE] WORK BUFFER3 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_3][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][0] = CTL_LOCK;
				RunBuff3Step = 6000;
			
				if(COMI.mn_simulation_mode == 1)
				{
					RunBuff3Step = 1500;
					st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 5;
					l_Work3_WaitTime[0] = GetCurrentTime();
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[SORTER_SITE] WORK BUFFER1 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		break;


	case 1100:
		l_Work3_WaitTime[1] = GetCurrentTime();
		l_Work3_WaitTime[2] = l_Work3_WaitTime[1] - l_Work3_WaitTime[0];
		if(l_Work3_WaitTime[2] < 0)
		{
			l_Work3_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work3_WaitTime[2] > 1000)
		{
			RunBuff3Step = 2200;
		}
		break;
		
	case 1200:
		l_Work3_WaitTime[1] = GetCurrentTime();
		l_Work3_WaitTime[2] = l_Work3_WaitTime[1] - l_Work3_WaitTime[0];
		if(l_Work3_WaitTime[2] < 0)
		{
			l_Work3_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work3_WaitTime[2] > 1000)
		{
			RunBuff3Step = 3000;	
		}
		break;
		
	case 1300:
		l_Work3_WaitTime[1] = GetCurrentTime();
		l_Work3_WaitTime[2] = l_Work3_WaitTime[1] - l_Work3_WaitTime[0];
		if(l_Work3_WaitTime[2] < 0)
		{
			l_Work3_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work3_WaitTime[2] > 1000)
		{
			RunBuff3Step = 4000;			
		}
		break;
		
	case 1400:
		l_Work3_WaitTime[1] = GetCurrentTime();
		l_Work3_WaitTime[2] = l_Work3_WaitTime[1] - l_Work3_WaitTime[0];
		if(l_Work3_WaitTime[2] < 0)
		{
			l_Work3_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work3_WaitTime[2] > 1000)
		{
			RunBuff3Step = 5000;			
		}
		break;
		
	case 1500:
		l_Work3_WaitTime[1] = GetCurrentTime();
		l_Work3_WaitTime[2] = l_Work3_WaitTime[1] - l_Work3_WaitTime[0];
		if(l_Work3_WaitTime[2] < 0)
		{
			l_Work3_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work3_WaitTime[2] > 1000)
		{
			RunBuff3Step = 6000;			
		}
		break;

//	case 2000:
//		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
//		if(nRet_1 == BD_GOOD)
//		{
//			RunBuff3Step = 2200;
//		}
//		else if(nRet_1 == BD_RETRY)
//		{
//			RunBuff3Step = 2000;
//		}
//		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
//		{
//			RunBuff3Step = 2000;
//			st_work.mn_run_status = CTL_dWARNING;
//			CTL_Lib.Alarm_Error_Occurrence(2651, CTL_dWARNING, COMI.mc_alarmcode);
//		}
//		break;


	case 2200:
		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][1] = CTL_READY;
		RunBuff3Step = 2300;
		break;

	case 2300:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf3_m_clamp_onoff, IO_ON);
			RunBuff3Step = 2400;
		}
//		else 
//		{
//			RunBuff3Step = 2200;
//		}
		break;

	case 2310:
		FAS_IO.OnCylinderAction(st_io.o_buf3_m_clamp_onoff, IO_ON);
		RunBuff3Step = 2400;
		break;


	case 2400:		
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf3_m_clamp_onoff, st_io.i_buf3_m_clamp_off_chk, st_io.i_buf3_m_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[80], st_wait.n_limit_wait_time[80]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff3Step = 2500;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550806 1 55 "Module Buffer#3 UnClamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"550806");
				CTL_Lib.Alarm_Error_Occurrence(2652, CTL_dWARNING, mc_alarmcode);
				RunBuff3Step = 2310;
			}
			else
			{
				RunBuff3Step = 2310;
			}
		}
		break;

	case 2500:
		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][1] = CTL_POSSIBLE;
		RunBuff3Step = 2600;
		break;

	case 2600:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][1] == CTL_COMPLETE)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf3_m_clamp_onoff, IO_OFF);
			RunBuff3Step = 2700;
		}
		break;

	case 2610:
		FAS_IO.OnCylinderAction(st_io.o_buf3_m_clamp_onoff, IO_OFF);
		RunBuff3Step = 2700;
		break;

	case 2700:
		if(st_basic.n_mode_device != 1 || (FAS_IO.get_in_bit(st_io.i_buf3_m_clamp_off_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_buf3_m_clamp_on_chk, IO_ON) == IO_ON))
		{
			RunBuff3Step = 2800;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf3_m_clamp_onoff, st_io.i_buf3_m_clamp_on_chk, st_io.i_buf3_m_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[80], st_wait.n_limit_wait_time[80]);
			nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_buf3_m_clamp_onoff, st_io.i_buf3_m_clamp_off_chk, st_io.i_buf3_m_clamp_on_chk,
				IO_ON, st_wait.n_on_wait_time[80], st_wait.n_limit_wait_time[80]);
			if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
			{
				RunBuff3Step = 2800;
			}
			else if (nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
			{//550802 1 55 "Module Buffer#3 Clamp 에러"
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550802");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2653, st_work.mn_run_status, mc_alarmcode);
					RunBuff3Step = 2610;
				}
				else
				{
					RunBuff3Step = 2610;
				}
			}
		}
		break;

	case 2800:
// 		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][1] = CTL_BUSY;
		RunBuff3Step = 2900;
		break;

	case 2900:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][1] == CTL_CHANGE)
		{
			RunBuff3Step = 2910;
		}
		break;

	case 2910:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDLEFTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
// 			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][1] = CTL_NO;
			FAS_IO.OnCylinderAction(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
			RunBuff3Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 2910;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff3Step = 2910;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2653, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_3);
		if(dCurrPos > st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDLEFTHS] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDLEFTHS] + 0.05)
		{
			RunBuff3Step = 3100;
		}
		else
		{
			RunBuff3Step = 3005;
		}
		break;

	case 3005:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDLEFTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
			RunBuff3Step = 3100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff3Step = 3000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2654, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3010:
		FAS_IO.OnCylinderAction(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
		RunBuff3Step = 3100;
		break;

	case 3100:
		nRet_1= FAS_IO.OnCylinderActionCheck(st_io.o_buf3_hs_clamp_onoff, st_io.i_buf3_hs_clamp_on_chk, st_io.i_buf3_hs_clamp_off_chk, 
			IO_ON, st_wait.n_on_wait_time[81], st_wait.n_limit_wait_time[81]);
		if(nRet_1 == RET_GOOD || st_basic.n_mode_device != 1)
		{
			RunBuff3Step = 3200;
		}
		else if(nRet_1 != RET_PROCEED)
		{//550812 1 55 "Heat Sink Buffer#1 Clamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"550812");
				CTL_Lib.Alarm_Error_Occurrence(2655, CTL_dWARNING, mc_alarmcode);
				RunBuff3Step = 3010;
			}
			else
			{
				RunBuff3Step = 3010;
			}
		}
		break;

	case 3200:
		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][1] = CTL_READY;
		RunBuff3Step = 3300;
		break;

	case 3300:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
			RunBuff3Step = 3400;
		}
//		else
//		{
//			RunBuff3Step = 3200;
//		}
		break;

	case 3310:
		FAS_IO.OnCylinderAction(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
		RunBuff3Step = 3400;
		break;

	case 3400:		
		if(FAS_IO.get_in_bit(st_io.i_buf3_hs_clamp_off_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_buf3_hs_clamp_on_chk, IO_ON) == IO_ON)
		{
			RunBuff3Step = 3500;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf3_hs_clamp_onoff, st_io.i_buf3_hs_clamp_on_chk, st_io.i_buf3_hs_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[81], st_wait.n_limit_wait_time[81]);
			if(nRet_1 == RET_GOOD)
			{
				RunBuff3Step = 3500;
			}
			else if (nRet_1 != RET_PROCEED)
			{//550812 1 55 "Heat Sink Buffer#3 Clamp 에러"
				mn_retry++;
				if(mn_retry > 3)
				{
					sprintf(mc_alarmcode,"550812");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2656, st_work.mn_run_status, mc_alarmcode);
					RunBuff3Step = 3310;
				}
				else
				{
					RunBuff3Step = 3310;
				}
			}
		}
		break;

	case 3500:
		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][1] = CTL_POSSIBLE;
		RunBuff3Step = 3600;
		break;

	case 3600:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][1] == CTL_COMPLETE)
		{
			RunBuff3Step = 3800;
		}
		break;

	case 3800:
// 		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][1] = CTL_BUSY;
		RunBuff3Step = 3900;
		break;

	case 3900:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][1] == CTL_CHANGE)
		{
			RunBuff3Step = 3910;
		}
		break;

	case 3910:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDRIGHTTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][1] = CTL_NO;
			RunBuff3Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 3910;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff3Step = 3910;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2657, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_3);
		if(dCurrPos > st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDRIGHTTHS] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDRIGHTTHS] + 0.05)
		{
			RunBuff3Step = 4200;
		}
		else
		{
			RunBuff3Step = 4100;
		}
		break;

	case 4100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDRIGHTTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff3Step = 4200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 4000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff3Step = 4000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2657, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4200:
		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][1] = CTL_READY;
		RunBuff3Step = 4300;
		break;

	case 4300:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
			RunBuff3Step = 4400;
		}
//		else 
//		{
//			RunBuff3Step = 4200;
//		}
		break;

	case 4310:
		FAS_IO.OnCylinderAction(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
		RunBuff3Step = 4400;
		break;

	case 4400:		
		if(FAS_IO.get_in_bit(st_io.i_buf3_hs_clamp_off_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_buf3_hs_clamp_on_chk, IO_ON) == IO_ON)
		{
			RunBuff3Step = 4500;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf3_hs_clamp_onoff, st_io.i_buf3_hs_clamp_on_chk, st_io.i_buf3_hs_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[81], st_wait.n_limit_wait_time[81]);
			if(nRet_1 == RET_GOOD)
			{
				RunBuff3Step = 4500;
			}
			else if (nRet_1 != RET_PROCEED)
			{//550822 1 55 "Heat Sink Buffer#3 Clamp 에러"
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550812");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2658, st_work.mn_run_status, mc_alarmcode);
					RunBuff3Step = 4310;
				}
				else
				{
					RunBuff3Step = 4310;
				}
			}
		}
		break;

	case 4500:
		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][1] = CTL_POSSIBLE;
		RunBuff3Step = 4600;
		break;

	case 4600:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][1] == CTL_COMPLETE)
		{
			FAS_IO.OnCylinderAction(st_io.o_buf3_hs_clamp_onoff, IO_OFF);
			RunBuff3Step = 4700;
		}
		break;

	case 4700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf3_m_clamp_onoff, st_io.i_buf3_hs_clamp_on_chk, st_io.i_buf3_hs_clamp_off_chk,
			IO_ON, st_wait.n_on_wait_time[80], st_wait.n_limit_wait_time[80]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff3Step = 4800;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550814 1 55 "Heat Sink Buffer#1 UnClamp 에러"
			sprintf(mc_alarmcode,"550814");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2659, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 4800:
// 		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][1] = CTL_BUSY;
		RunBuff3Step = 4900;
		break;

	case 4900:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][1] == CTL_CHANGE)
		{
//			RunBuff3Step = 1000;
			RunBuff3Step = 4920;
		}
		break;

	case 4910:
		if(st_basic.n_mode_device !=0)
		{
			if(FAS_IO.get_in_bit(st_io.i_buf3_m_clamp_off_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_buf3_m_clamp_on_chk, IO_ON) == IO_ON)
			{
				RunBuff3Step = 4920;
			}
			else
			{//550802 1 55 "Module Buffer#3 Clamp 에러"
				sprintf(mc_alarmcode,"550802");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2645, st_work.mn_run_status, mc_alarmcode);
				RunBuff3Step = 4910;
			}
		}
		else
		{
			RunBuff3Step = 4920;
		}

		break;


	case 4920:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDCLIP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_3][0] = CTL_FREE; // 20130416
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][1] = CTL_NO;
			st_handler.n_hs3_fix = CTL_YES;
			RunBuff3Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 4920;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff3Step = 4920;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2636, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4930:
		FAS_IO.set_out_bit(st_io.o_buf3_f_hs_onoff, IO_ON);//2012,0821
		l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
		RunBuff3Step = 4940;
		break;

	case 4940:
		l_ClipWaitTime[WORK_BUFFER_3][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_3][2] = l_ClipWaitTime[WORK_BUFFER_3][1] - l_ClipWaitTime[WORK_BUFFER_3][0];
		if(l_ClipWaitTime[WORK_BUFFER_3][2] < 0) l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_3][2] > 800)
		{
			FAS_IO.set_out_bit(st_io.o_buf3_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
			RunBuff3Step = 4950;
		}
		break;

	case 4950:
		l_ClipWaitTime[WORK_BUFFER_3][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_3][2] = l_ClipWaitTime[WORK_BUFFER_3][1] - l_ClipWaitTime[WORK_BUFFER_3][0];
		if(l_ClipWaitTime[WORK_BUFFER_3][2] < 0) l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_3][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf3_f_hs_onoff, IO_ON);//2012,0821
			l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
			RunBuff3Step = 4960;
		}
		break;

	case 4960:
		l_ClipWaitTime[WORK_BUFFER_3][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_3][2] = l_ClipWaitTime[WORK_BUFFER_3][1] - l_ClipWaitTime[WORK_BUFFER_3][0];
		if(l_ClipWaitTime[WORK_BUFFER_3][2] < 0) l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_3][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf3_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
			RunBuff3Step = 4970;
		}
		break;

	case 4970:
		l_ClipWaitTime[WORK_BUFFER_3][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_3][2] = l_ClipWaitTime[WORK_BUFFER_3][1] - l_ClipWaitTime[WORK_BUFFER_3][0];
		if(l_ClipWaitTime[WORK_BUFFER_3][2] < 0) l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_3][2] > 200)
		{
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_3][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][1] = CTL_NO;
			RunBuff3Step = 1000;
		}
		break;

	case 5000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_3);
		if(dCurrPos > st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDCLIP] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDCLIP] + 0.05)
		{
			RunBuff3Step = 5200;
		}
		else
		{
			RunBuff3Step = 5100;
		}
		break;

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDCLIP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff3Step = 5200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 5100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff3Step = 5100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2660, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 5200:
		if(st_handler.n_hs3_fix == CTL_NO)
		{
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_3][1] = CTL_READY;
			RunBuff3Step = 5300;
		}
		break;

	case 5300:
		if(st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_3][1] == CTL_CHANGE)
		{
//			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_3][1] = CTL_NO;
			RunBuff3Step = 5400;
		}
		break;

	case 5400:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_ULDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_3] = NO;
//			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][1] = CTL_READY;
			RunBuff3Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 5400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff3Step = 5400;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2661, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
	
	case 6000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_3);
		if(dCurrPos > st_motor[M_WORK_BUFFER_3].md_pos[WORK_ULDMODULE] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_3].md_pos[WORK_ULDMODULE] + 0.05)
		{
			RunBuff3Step = 6100;
		}
		else
		{
			RunBuff3Step = 6010;
		}
		break;

	case 6010:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_ULDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_3] = NO;
			RunBuff3Step = 6100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 6000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff3Step = 6000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2662, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 6100:
//		st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][1] = CTL_READY;
		RunBuff3Step = 6200;
		break;

	case 6200:
		if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf3_m_clamp_onoff, IO_ON);
			RunBuff3Step = 6300;
		}
		break;
		
	case 6210:
		FAS_IO.OnCylinderAction(st_io.o_buf3_m_clamp_onoff, IO_ON);
		RunBuff3Step = 6300;
		break;

	case 6300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf3_m_clamp_onoff, st_io.i_buf3_m_clamp_off_chk, st_io.i_buf3_m_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[80], st_wait.n_limit_wait_time[80]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff3Step = 6400;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550810 1 55 "Heat Sink Buffer#1 Clamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				sprintf(mc_alarmcode,"550812");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2663, st_work.mn_run_status, mc_alarmcode);
				RunBuff3Step = 6210;
			}
			else
			{
				RunBuff3Step = 6210;
			}
		}
		break;

	case 6400:
		st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][1] = CTL_POSSIBLE;
		RunBuff3Step = 6500;
		break;

	case 6500:
		if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][1] == CTL_CHANGE)
		{
			//st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][0] = CTL_FREE;
			//st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][1] = CTL_NO;
			RunBuff3Step = 6600;
		}
		break;
		
	case 6600:
//		if(st_sync.i_WorkClamp_Clip[WORK_BUFFER_2] == NO && st_sync.i_WorkClamp_Clip[WORK_BUFFER_3] == NO && st_sync.i_WorkClamp_Clip[WORK_BUFFER_4] == NO)
//		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_3] = YES;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][1] = CTL_NO;
			FAS_IO.set_out_bit(st_io.o_buf3_m_clamp_onoff, IO_ON);
			FAS_IO.set_out_bit(st_io.o_buf3_hs_clamp_onoff, IO_OFF);

			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_3][LDMODULE_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_3][LEFTSINK_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_3][RIGHTSINK_SITE] = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_3][LDCLIP_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_3][SORTER_SITE]	 = DVC_NO;

			st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_3][0] 	 = CTL_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_3][0] 	 = CTL_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_3][0] 	 = CTL_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_3][0] 	 = CTL_NO;
			st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_3][0] 	 = CTL_NO;

			st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_3][1] 	 = CTL_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_3][1] 	 = CTL_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_3][1] 	 = CTL_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_3][1] 	 = CTL_NO;
			st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_3][1] 	 = CTL_NO;

			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_3][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][0] = CTL_NO;

			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_3][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_3][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_3][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_3][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_3][1] = CTL_NO;
			RunBuff3Step = 6700;
//		}
		break;

	case 6700:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_2] = NO;
			if(COMI.mn_simulation_mode == 1)
			{
				st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 1;
			}
			
			RunBuff3Step = 100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff3Step = 6700;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunBuff3Step = 6700;
			CTL_Lib.Alarm_Error_Occurrence(2664, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;	
	}
}

void CRun_Rbt_Work_Buffer::Run_Worker_Buffer4()
{
	int nRet_1,nRet_2;
	double dCurrPos;
	Func.ThreadFunctionStepTrace(36, RunBuff4Step);
	switch(RunBuff4Step)
	{
	case 0:
		RunBuff4Step = 100;
		break;

	case 10:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff4Step = 20;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff4Step = 10;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunBuff4Step = 10;
			CTL_Lib.Alarm_Error_Occurrence(2676, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 20:
		nRet_1 = COMI.HomeCheck_Mot(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			RunBuff4Step = 100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			RunBuff4Step = 20;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2677, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
	
	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
// 			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][0] = CTL_FREE;
			RunBuff4Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff4Step = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff4Step = 100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2670, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1000:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_4][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][0] = CTL_LOCK;
				RunBuff4Step = 2200;

				if(COMI.mn_simulation_mode == 1)
				{
					st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 1;
					l_Work4_WaitTime[0] = GetCurrentTime();
					RunBuff4Step = 1100;
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LDMODULE_SITE] WORK BUFFER1 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_4][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][0] = CTL_LOCK;
				RunBuff4Step = 3000;

				if(COMI.mn_simulation_mode == 1)
				{
					st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 2;
					l_Work4_WaitTime[0] = GetCurrentTime();
					RunBuff4Step = 1200;
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LEFTSINK_SITE] WORK BUFFER4 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_4][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][0] = CTL_LOCK;
				RunBuff4Step = 4000;

				if(COMI.mn_simulation_mode == 1)
				{
					st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 3;
					l_Work4_WaitTime[0] = GetCurrentTime();
					RunBuff4Step = 1300;
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[RIGHTSINK_SITE] WORK BUFFER4 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_4][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_4][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_4][0] = CTL_LOCK;				
				RunBuff4Step = 5000;

				if(COMI.mn_simulation_mode == 1)
				{
					st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 4;
					l_Work4_WaitTime[0] = GetCurrentTime();
					RunBuff4Step = 1400;
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[LDCLIP_SITE] WORK BUFFER4 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		else if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][0] == CTL_REQ)
		{
			if(st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_4][0] == BUFF_EMPTY)
			{
				st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][0] = CTL_LOCK;
				RunBuff4Step = 6000;

				if(COMI.mn_simulation_mode == 1)
				{
					st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 5;
					l_Work4_WaitTime[0] = GetCurrentTime();
					RunBuff4Step = 1500;
				}
			}
			else
			{
				sprintf(st_msg.c_abnormal_msg, "[SORTER_SITE] WORK BUFFER4 not empty.");
				if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
		}
		break;

	case 1100:
		l_Work4_WaitTime[1] = GetCurrentTime();
		l_Work4_WaitTime[2] = l_Work4_WaitTime[1] - l_Work4_WaitTime[0];
		if(l_Work4_WaitTime[2] < 0)
		{
			l_Work4_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work4_WaitTime[2] > 1000)
		{
			RunBuff4Step = 2200;
		}
		break;
		
	case 1200:
		l_Work4_WaitTime[1] = GetCurrentTime();
		l_Work4_WaitTime[2] = l_Work4_WaitTime[1] - l_Work4_WaitTime[0];
		if(l_Work4_WaitTime[2] < 0)
		{
			l_Work4_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work4_WaitTime[2] > 1000)
		{
			RunBuff4Step = 3000;
		}
		break;
		
	case 1300:
		l_Work4_WaitTime[1] = GetCurrentTime();
		l_Work4_WaitTime[2] = l_Work4_WaitTime[1] - l_Work4_WaitTime[0];
		if(l_Work4_WaitTime[2] < 0)
		{
			l_Work4_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work4_WaitTime[2] > 1000)
		{
			RunBuff4Step = 4000;
		}
		break;
		
	case 1400:
		l_Work4_WaitTime[1] = GetCurrentTime();
		l_Work4_WaitTime[2] = l_Work4_WaitTime[1] - l_Work4_WaitTime[0];
		if(l_Work4_WaitTime[2] < 0)
		{
			l_Work4_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work4_WaitTime[2] > 1000)
		{
			RunBuff4Step = 5000;
		}
		break;
		
	case 1500:
		l_Work4_WaitTime[1] = GetCurrentTime();
		l_Work4_WaitTime[2] = l_Work4_WaitTime[1] - l_Work4_WaitTime[0];
		if(l_Work4_WaitTime[2] < 0)
		{
			l_Work4_WaitTime[0] = GetCurrentTime();
			break;
		}
		else if(l_Work4_WaitTime[2] > 1000)
		{
			RunBuff4Step = 6000;
		}
		break;

//	case 2000:
//		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
//		if(nRet_1 == BD_GOOD)
//		{
//			RunBuff4Step = 2200;
//		}
//		else if(nRet_1 == BD_RETRY)
//		{
//			RunBuff4Step = 2000;
//		}
//		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
//		{
//			RunBuff4Step = 2000;
//			st_work.mn_run_status = CTL_dWARNING;
//			CTL_Lib.Alarm_Error_Occurrence(2671, st_work.mn_run_status, COMI.mc_alarmcode);
//		}
//		break;

	case 2200:
		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][1] = CTL_READY;
		RunBuff4Step = 2300;
		break;

	case 2300:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf4_m_clamp_onoff, IO_ON);
			RunBuff4Step = 2400;
		}
//		else
//		{
//			RunBuff4Step = 2200;
//		}
		break;

	case 2310:
		FAS_IO.OnCylinderAction(st_io.o_buf4_m_clamp_onoff, IO_ON);
		RunBuff4Step = 2400;
		break;

	case 2400:		
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf4_m_clamp_onoff, st_io.i_buf4_m_clamp_off_chk, st_io.i_buf4_m_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[84], st_wait.n_limit_wait_time[84]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff4Step = 2500;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550807 1 55 "Module Buffer#4 UnClamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				sprintf(mc_alarmcode,"550807");
				CTL_Lib.Alarm_Error_Occurrence(2672, CTL_dWARNING, mc_alarmcode);
				RunBuff4Step = 2310;
			}
			else
			{
				RunBuff4Step = 2310;
			}
		}
		break;

	case 2500:
		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][1] = CTL_POSSIBLE;
		RunBuff4Step = 2600;
		break;

	case 2600:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][1] == CTL_COMPLETE)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf4_m_clamp_onoff, IO_OFF);
			RunBuff4Step = 2700;
		}
		break;

	case 2610:
		FAS_IO.OnCylinderAction(st_io.o_buf4_m_clamp_onoff, IO_OFF);
		RunBuff4Step = 2700;
		break;

	case 2700:
		if(st_basic.n_mode_device != 1 || (FAS_IO.get_in_bit(st_io.i_buf4_m_clamp_off_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_buf4_m_clamp_on_chk, IO_ON) == IO_ON))
		{
			RunBuff4Step = 2800;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf4_m_clamp_onoff, st_io.i_buf4_m_clamp_on_chk, st_io.i_buf4_m_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[84], st_wait.n_limit_wait_time[84]);
			nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_buf4_m_clamp_onoff, st_io.i_buf4_m_clamp_off_chk, st_io.i_buf4_m_clamp_on_chk,
				IO_ON, st_wait.n_on_wait_time[84], st_wait.n_limit_wait_time[84]);
			if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
			{
				RunBuff4Step = 2800;
			}
			else if (nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
			{//550803 1 55 "Module Buffer#4 Clamp 에러"
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550803");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2673, st_work.mn_run_status, mc_alarmcode);
					RunBuff4Step = 2610;
				}
				else
				{
					RunBuff4Step = 2610;
				}
			}
		}
		break;

	case 2800:
// 		st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][1] = CTL_BUSY;
		RunBuff4Step = 2900;
		break;

	case 2900:
		if(st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][1] == CTL_CHANGE)
		{
			RunBuff4Step = 2910;
		}
		break;

	case 2910:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDLEFTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
// 			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][1] = CTL_NO;
			FAS_IO.OnCylinderAction(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
			RunBuff4Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff4Step = 2910;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff4Step = 2910;
			CTL_Lib.Alarm_Error_Occurrence(2673, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_4);
		if(dCurrPos > st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDLEFTHS] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDLEFTHS] + 0.05)
		{
			RunBuff4Step = 3100;
		}
		else
		{
			RunBuff4Step = 3005;
		}
		break;

	case 3005:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDLEFTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
			RunBuff4Step = 3100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff4Step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff4Step = 3000;
			CTL_Lib.Alarm_Error_Occurrence(2674, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3010:
		FAS_IO.OnCylinderAction(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
		RunBuff4Step = 3100;
		break;

	case 3100:
		nRet_1= FAS_IO.OnCylinderActionCheck(st_io.o_buf4_hs_clamp_onoff, st_io.i_buf4_hs_clamp_on_chk, st_io.i_buf4_hs_clamp_off_chk, 
			IO_ON, st_wait.n_on_wait_time[85], st_wait.n_limit_wait_time[85]);
		if(nRet_1 == RET_GOOD || st_basic.n_mode_device != 1)
		{
			RunBuff4Step = 3200;
		}
		else if(nRet_1 != RET_PROCEED)
		{//550813 1 55 "Heat Sink Buffer#4 Clamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"550813");
				CTL_Lib.Alarm_Error_Occurrence(2675, CTL_dWARNING, mc_alarmcode);
				RunBuff4Step = 3010;
			}
			else
			{
				RunBuff4Step = 3010;
			}
		}
		break;
		
	case 3200:
		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][1] = CTL_READY;
		RunBuff4Step = 3300;
		break;

	case 3300:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
			RunBuff4Step = 3400;
		}
//		else
//		{
//			RunBuff4Step = 3200;
//		}
		break;

	case 3310:
		FAS_IO.OnCylinderAction(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
		RunBuff4Step = 3400;
		break;

	case 3400:
		if(FAS_IO.get_in_bit(st_io.i_buf4_hs_clamp_off_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_buf4_hs_clamp_on_chk, IO_ON) == IO_ON)
		{
			RunBuff4Step = 3500;
		}
		else
		{		
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf4_hs_clamp_onoff, st_io.i_buf4_hs_clamp_off_chk, st_io.i_buf4_hs_clamp_on_chk,
				IO_ON, st_wait.n_on_wait_time[85], st_wait.n_limit_wait_time[85]);
			if(nRet_1 == RET_GOOD)
			{
				RunBuff4Step = 3500;
			}
			else if (nRet_1 != RET_PROCEED)
			{//550813 1 55 "Heat Sink Buffer#4 Clamp 에러"
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550813");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2676, st_work.mn_run_status, mc_alarmcode);
					RunBuff4Step = 3310;
				}
				else
				{
					RunBuff4Step = 3310;
				}
			}
		}
		break;

	case 3500:
		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][1] = CTL_POSSIBLE;
		RunBuff4Step = 3600;
		break;

	case 3600:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][1] == CTL_COMPLETE)
		{
			RunBuff4Step = 3800;
		}
		break;

	case 3800:
// 		st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][1] = CTL_BUSY;
		RunBuff4Step = 3900;
		break;

	case 3900:
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][1] == CTL_CHANGE)
		{
			RunBuff4Step = 3910;
		}
		break;

	case 3910:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDRIGHTTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][1] = CTL_NO;
			RunBuff4Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff4Step = 3910;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff4Step = 3910;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2677, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_4);
		if(dCurrPos > st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDRIGHTTHS] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDRIGHTTHS] + 0.05)
		{
			RunBuff4Step = 4200;
		}
		else
		{
			RunBuff4Step = 4100;
		}
		break;

	case 4100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDRIGHTTHS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunBuff4Step = 4200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff4Step = 4000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff4Step = 4000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2678, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4200:
		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][1] = CTL_READY;
		RunBuff4Step = 4300;
		break;

	case 4300:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
			RunBuff4Step = 4400;
		}
//		else
//		{
//			RunBuff4Step = 4200;
//		}
		break;

	case 4310:
		FAS_IO.OnCylinderAction(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
		RunBuff4Step = 4400;
		break;

	case 4400:		
		if(FAS_IO.get_in_bit(st_io.i_buf4_hs_clamp_off_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_buf4_hs_clamp_on_chk, IO_ON) == IO_ON)
		{
			RunBuff4Step = 4500;
		}
		else
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf4_hs_clamp_onoff, st_io.i_buf4_hs_clamp_on_chk, st_io.i_buf4_hs_clamp_off_chk,
				IO_ON, st_wait.n_on_wait_time[85], st_wait.n_limit_wait_time[85]);
			if(nRet_1 == RET_GOOD)
			{
				RunBuff4Step = 4500;
			}
			else if (nRet_1 != RET_PROCEED)
			{//550822 1 55 "Heat Sink Buffer#3 Clamp 에러"
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"550812");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2679, st_work.mn_run_status, mc_alarmcode);
					RunBuff4Step = 4310;
				}
				else
				{
					RunBuff4Step = 4310;
				}
			}
		}
		break;

	case 4500:
		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][1] = CTL_POSSIBLE;
		RunBuff4Step = 4600;
		break;

	case 4600:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][1] == CTL_COMPLETE)
		{
			FAS_IO.OnCylinderAction(st_io.o_buf4_hs_clamp_onoff, IO_OFF);
			RunBuff4Step = 4700;
		}
		break;

	case 4700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf4_m_clamp_onoff, st_io.i_buf4_hs_clamp_on_chk, st_io.i_buf4_hs_clamp_off_chk,
			IO_ON, st_wait.n_on_wait_time[84], st_wait.n_limit_wait_time[84]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff4Step = 4800;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550817 1 55 "Heat Sink Buffer#7 UnClamp 에러"
			sprintf(mc_alarmcode,"550817");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2680, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 4800:
// 		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][1] = CTL_BUSY;
		RunBuff4Step = 4900;
		break;

	case 4900:
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][1] == CTL_CHANGE)
		{
//			RunBuff4Step = 1000;
			RunBuff4Step = 4920;
		}
		break;

	case 4910:
		if(st_basic.n_mode_device !=0)
		{
			if(FAS_IO.get_in_bit(st_io.i_buf4_m_clamp_off_chk, IO_ON) == IO_ON &&
				FAS_IO.get_in_bit(st_io.i_buf4_m_clamp_on_chk, IO_ON) == IO_ON)
			{
				RunBuff4Step = 4920;
			}
			else
			{//550803 1 55 "Module Buffer#4 Clamp 에러"
				sprintf(mc_alarmcode,"550803");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2645, st_work.mn_run_status, mc_alarmcode);
				RunBuff4Step = 4910;
			}
		}
		else
		{
			RunBuff4Step = 4920;
		}

		break;

	case 4920:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDCLIP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_4][0] = CTL_FREE; // 20130416
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][1] = CTL_NO;
			st_handler.n_hs4_fix = CTL_YES;
			RunBuff4Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff4Step = 4920;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff4Step = 4920;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2636, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4930:
		FAS_IO.set_out_bit(st_io.o_buf4_f_hs_onoff, IO_ON);//2012,0821
		l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
		RunBuff4Step = 4940;
		break;

	case 4940:
		l_ClipWaitTime[WORK_BUFFER_4][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_4][2] = l_ClipWaitTime[WORK_BUFFER_4][1] - l_ClipWaitTime[WORK_BUFFER_4][0];
		if(l_ClipWaitTime[WORK_BUFFER_4][2] < 0) l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_4][2] > 800)
		{
			FAS_IO.set_out_bit(st_io.o_buf4_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
			RunBuff4Step = 4950;
		}
		break;

	case 4950:
		l_ClipWaitTime[WORK_BUFFER_4][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_4][2] = l_ClipWaitTime[WORK_BUFFER_4][1] - l_ClipWaitTime[WORK_BUFFER_4][0];
		if(l_ClipWaitTime[WORK_BUFFER_4][2] < 0) l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_4][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf4_f_hs_onoff, IO_ON);//2012,0821
			l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
			RunBuff4Step = 4960;
		}
		break;

	case 4960:
		l_ClipWaitTime[WORK_BUFFER_4][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_4][2] = l_ClipWaitTime[WORK_BUFFER_4][1] - l_ClipWaitTime[WORK_BUFFER_4][0];
		if(l_ClipWaitTime[WORK_BUFFER_4][2] < 0) l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_4][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf4_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
			RunBuff4Step = 4970;
		}
		break;

	case 4970:
		l_ClipWaitTime[WORK_BUFFER_4][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_4][2] = l_ClipWaitTime[WORK_BUFFER_4][1] - l_ClipWaitTime[WORK_BUFFER_4][0];
		if(l_ClipWaitTime[WORK_BUFFER_4][2] < 0) l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_4][2] > 200)
		{
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_4][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][1] = CTL_NO;
			RunBuff4Step = 1000;
		}
		break;


	case 5000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_4);
		if(dCurrPos > st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDCLIP] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDCLIP] + 0.05)
		{
			RunBuff4Step = 5200;
		}
		else
		{
			RunBuff4Step = 5100;
		}
		break;

	case 5100:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDCLIP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunBuff4Step = 5200;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			RunBuff4Step = 5100;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunBuff4Step = 5100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2681, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 5200:
		if(st_handler.n_hs4_fix == CTL_NO)
		{
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_4][1] = CTL_READY;
			RunBuff4Step = 5300;
		}
		break;

	case 5300:
		if(st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_4][1] == CTL_CHANGE)
		{
//			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_4][1] = CTL_NO;
			RunBuff4Step = 5400;
		}
		break;

	case 5400:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_ULDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_4] = NO;
//			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][0] = CTL_FREE;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][1] = CTL_READY;
			RunBuff4Step = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff4Step = 5400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff4Step = 5400;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2682, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
	
	case 6000:
		dCurrPos = COMI.Get_MotCurrentPos(M_WORK_BUFFER_4);
		if(dCurrPos > st_motor[M_WORK_BUFFER_4].md_pos[WORK_ULDMODULE] - 0.05 &&
			dCurrPos < st_motor[M_WORK_BUFFER_4].md_pos[WORK_ULDMODULE] + 0.05)
		{
			RunBuff4Step = 6100;
		}
		else
		{
			RunBuff4Step = 6010;
		}
		break;

	case 6010:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_ULDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_4] = NO;
			RunBuff4Step = 6100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff4Step = 6000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff4Step = 6000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2683, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 6100:
//		st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][1] = CTL_READY;
		RunBuff4Step = 6200;
		break;

	case 6200:
		if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][1] == CTL_DOREADY)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_buf4_m_clamp_onoff, IO_ON);
			RunBuff4Step = 6300;
		}
		break;
		
	case 6210:
		FAS_IO.OnCylinderAction(st_io.o_buf4_m_clamp_onoff, IO_ON);
		RunBuff4Step = 6300;
		break;

	case 6300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf4_m_clamp_onoff, st_io.i_buf4_m_clamp_off_chk, st_io.i_buf4_m_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[84], st_wait.n_limit_wait_time[84]);
		if(nRet_1 == RET_GOOD)
		{
			RunBuff4Step = 6400;
		}
		else if (nRet_1 != RET_PROCEED)
		{//550813 1 55 "Heat Sink Buffer#1 Clamp 에러"
			mn_retry++;
			if(mn_retry > 3)
			{
				sprintf(mc_alarmcode,"550813");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2684, st_work.mn_run_status, mc_alarmcode);
				RunBuff4Step = 6210;
			}
			else
			{
				RunBuff4Step = 6210;
			}
		}
		break;

	case 6400:
		st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][1] = CTL_POSSIBLE;
		RunBuff4Step = 6500;
		break;

	case 6500:
		if(st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][1] == CTL_CHANGE)
		{
			//st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][0] = CTL_FREE;
			//st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][1] = CTL_NO;
			RunBuff4Step = 6600;
		}
		break;
		
	case 6600:
//		if(st_sync.i_WorkClamp_Clip[WORK_BUFFER_3] == NO)
//		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_4] = YES;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][1] = CTL_NO;
			FAS_IO.set_out_bit(st_io.o_buf4_m_clamp_onoff, IO_ON);
			FAS_IO.set_out_bit(st_io.o_buf4_hs_clamp_onoff, IO_OFF);

			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_4][LDMODULE_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_4][LEFTSINK_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_4][RIGHTSINK_SITE] = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_4][LDCLIP_SITE]	 = DVC_NO;
			st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_4][SORTER_SITE]	 = DVC_NO;

			st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_4][0] 	 = DVC_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_4][0] 	 = DVC_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_4][0] 	 = DVC_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_4][0] 	 = DVC_NO;
			st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_4][0] 	 = DVC_NO;

			st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_4][1] 	 = CTL_NO;
			st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_4][1] 	 = CTL_NO;
			st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_4][1] 	 = CTL_NO;
			st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_4][1] 	 = CTL_NO;
			st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_4][1] 	 = CTL_NO;

			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_4][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][0] = CTL_NO;

			st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_4][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_4][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_4][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_4][1] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_4][1] = CTL_NO;

			RunBuff4Step = 6700;
//		}
		break;

	case 6700:
		nRet_1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_LDMODULE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
//			st_sync.i_WorkBuffer_Clip[WORK_BUFFER_2] = NO;
			if(COMI.mn_simulation_mode == 1)
			{
				st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 1;
			}

			RunBuff4Step = 100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunBuff4Step = 6700;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunBuff4Step = 6700;
			CTL_Lib.Alarm_Error_Occurrence(2685, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;	
	}
}

void CRun_Rbt_Work_Buffer::OnWorkBufferRbt_FinalPos()//20121126
{
	int i;
	if(st_work.mn_reinstatement_mode[WORK_BUFFER_SITE] == 0) //stop시 최초 한번만 기록하기 위함
	{
		st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_1] = COMI.md_cmdpos_backup[M_WORK_BUFFER_1];
		st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_2] = COMI.md_cmdpos_backup[M_WORK_BUFFER_2];
		st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_3] = COMI.md_cmdpos_backup[M_WORK_BUFFER_3];
		st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_4] = COMI.md_cmdpos_backup[M_WORK_BUFFER_4];

		for (i = 0; i < 4; i++)
		{
			st_work.mn_BufVacuumStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_f_vacuum_onoff + (100*i), IO_OFF);
			st_work.mn_EjectStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_f_eject_onoff + (100*i), IO_OFF);
			st_work.mn_ModuleClampStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_m_clamp_onoff + (100*i), IO_OFF);
			st_work.mn_HsClampStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_hs_clamp_onoff + (100*i), IO_OFF);
		}
		st_work.mn_reinstatement_mode[WORK_BUFFER_SITE] = 1;	//최종 위치를 기록 했다

	}
}

void CRun_Rbt_Work_Buffer::Robot_BackMovePosValCheck()//20121126
{
	st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_1] = st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_1];
	st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_2] = st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_2];
	st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_3] = st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_3];
	st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_4] = st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_4];
}

void CRun_Rbt_Work_Buffer::Run_Reinstate()
{
	int Ret1 = 0, Ret2 = 0, i = 0, n_response = 0;
	int nRet1 = 0, nRet2 = 0;

	if (st_work.mn_WorkBufferRobot_Reinstatement_Ok == TRUE) return;//20121126

	switch (ReinstateStep)
	{
	case 0:
		ReinstateStep = 1000;
		break;

	case 1000://현재 위치 확인
		dWorkBufferCurPos[0] = COMI.Get_MotCurrentPos(M_WORK_BUFFER_1);
		dWorkBufferCurPos[1] = COMI.Get_MotCurrentPos(M_WORK_BUFFER_2);
		dWorkBufferCurPos[2] = COMI.Get_MotCurrentPos(M_WORK_BUFFER_3);
		dWorkBufferCurPos[3] = COMI.Get_MotCurrentPos(M_WORK_BUFFER_4);
	
		for (i = 0; i < 4; i++)
		{
			mn_change[WORK_BUFFER_SITE][i] = 0;
		}

		//변경된 위치값 출력
		if (dWorkBufferCurPos[0] > st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_1] + COMI.mn_allow_value[M_WORK_BUFFER_1]
			|| dWorkBufferCurPos[0] < st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_1] - COMI.mn_allow_value[M_WORK_BUFFER_1])
		{
			mn_change[WORK_BUFFER_SITE][0]++;
			sprintf(st_msg.c_abnormal_msg, "Work Buffer#1 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_1], (long)dWorkBufferCurPos[0], (long)COMI.mn_allow_value[M_WORK_BUFFER_1]);
		}
		else if (dWorkBufferCurPos[1] > st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_2] + COMI.mn_allow_value[M_WORK_BUFFER_2]
			|| dWorkBufferCurPos[1] < st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_2] - COMI.mn_allow_value[M_WORK_BUFFER_2])
		{
			mn_change[WORK_BUFFER_SITE][1]++;
			sprintf(st_msg.c_abnormal_msg, "Work Buffer#2 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_2], (long)dWorkBufferCurPos[1], (long)COMI.mn_allow_value[M_WORK_BUFFER_2]);
		}
		else if (dWorkBufferCurPos[2] > st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_3] + COMI.mn_allow_value[M_WORK_BUFFER_3]
			|| dWorkBufferCurPos[2] < st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_3] - COMI.mn_allow_value[M_WORK_BUFFER_3])
		{
			mn_change[WORK_BUFFER_SITE][2]++;
			sprintf(st_msg.c_abnormal_msg, "Work Buffer#3 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_3], (long)dWorkBufferCurPos[2], (long)COMI.mn_allow_value[M_WORK_BUFFER_3]);
		}
		else if (dWorkBufferCurPos[3] > st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_4] + COMI.mn_allow_value[M_WORK_BUFFER_4]
			|| dWorkBufferCurPos[3] < st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_4] - COMI.mn_allow_value[M_WORK_BUFFER_4])
		{
			mn_change[WORK_BUFFER_SITE][3]++;
			sprintf(st_msg.c_abnormal_msg, "Work Buffer#4 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_4], (long)dWorkBufferCurPos[3], (long)COMI.mn_allow_value[M_WORK_BUFFER_4]);
		}

		if(mn_change[WORK_BUFFER_SITE][0] > 0 || mn_change[WORK_BUFFER_SITE][1] > 0 || mn_change[WORK_BUFFER_SITE][2] > 0 || mn_change[WORK_BUFFER_SITE][3] > 0) //rbt y
		{
			ReinstateStep = 2000;
			//2016.0823
			ReinstateStep = 1100;//recovery skip
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
		Robot_BackMovePosValCheck();
	
		COMI.Set_MotPower(M_WORK_BUFFER_1, TRUE);
		COMI.Set_MotPower(M_WORK_BUFFER_2, TRUE);
		COMI.Set_MotPower(M_WORK_BUFFER_3, TRUE);
		COMI.Set_MotPower(M_WORK_BUFFER_4, TRUE);
	
		ReinstateStep = 2100;
		break;
	
	case 2100:
		if(st_work.mn_LdModuleRobot_Reinstatement_Ready == YES && st_work.mn_LeftHsRobot_Reinstatement_Ready   == YES &&
		   st_work.mn_RightHsRobot_Reinstatement_Ready  == YES && st_work.mn_ClipRobot_Reinstatement_Ready     == YES && // 20130416
		   st_work.mn_SorterRobot_Reinstatement_Ready   == YES)
		{
			ReinstateStep = 3000;
		}
		break;

	case 3000:
		nRet1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3020;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5028, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3020:
		nRet1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3040;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5030, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3040:
		nRet1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3060;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5032, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3060:
		nRet1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3080;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5034, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3080:
		st_work.mn_WorkBufferRobot_Reinstatement_Ready = YES;
		ReinstateStep = 3200;
		break;


	case 3200:
		nRet1 = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_1], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3300;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(5028, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;		

	case 3300:
		nRet1 = COMI.Check_SingleMove(M_WORK_BUFFER_1, st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_1]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3400;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5028, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3400:
		nRet1 = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_2], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3500;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(5030, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;		

	case 3500:
		nRet1 = COMI.Check_SingleMove(M_WORK_BUFFER_2, st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_2]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3600;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5030, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3600:
		nRet1 = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_3], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3700;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(5032, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;		

	case 3700:
		nRet1 = COMI.Check_SingleMove(M_WORK_BUFFER_3, st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_3]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3800;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5032, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3800:
		nRet1 = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_4], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3900;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(5034, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;		

	case 3900:
		nRet1 = COMI.Check_SingleMove(M_WORK_BUFFER_4, st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_4]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4000;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5034, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 4000:
		for (i = 0; i < 4; i++)
		{
			if(st_work.mn_BufVacuumStatusBackup[WORK_BUFFER_1 + i] == IO_ON)
			{
				FAS_IO.OnCylinderAction(st_io.o_buf1_f_vacuum_onoff + (100*i), IO_ON);
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_buf1_f_vacuum_onoff + (100*i), IO_OFF);
			}
			
			if(st_work.mn_EjectStatusBackup[WORK_BUFFER_1 + i] == IO_ON)
			{
				FAS_IO.OnCylinderAction(st_io.o_buf1_f_hs_onoff + (100*i), IO_ON);
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_buf1_f_hs_onoff + (100*i), IO_OFF);
			}
			
			if(st_work.mn_ModuleClampStatusBackup[WORK_BUFFER_1 + i] == IO_ON)
			{
				FAS_IO.OnCylinderAction(st_io.o_buf1_m_clamp_onoff + (100*i), IO_ON);
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_buf1_m_clamp_onoff + (100*i), IO_OFF);
			}
			
			if(st_work.mn_HsClampStatusBackup[WORK_BUFFER_1 + i] == IO_ON)
			{
				FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff + (100*i), IO_ON);
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff + (100*i), IO_OFF);
			}
		}
		ReinstateStep = 50000;		
		break;

	case 20000:
		st_handler.n_sync_reinstate = NO;
		ReinstateStep = 0;
		break;


	case 50000:
		st_work.mn_WorkBufferRobot_Reinstatement_Ready = YES;
		st_work.mn_WorkBufferRobot_Reinstatement_Ok = YES;//20121126
		ReinstateStep = 0;
		break;

	
	}
}

void CRun_Rbt_Work_Buffer::Run_HS1_OnFix()
{
	switch(HS1Step)
	{
	case 0:
		if(st_handler.n_hs1_fix == CTL_YES)
		{
			HS1Step = 1000;
		}
		break;

	case 1000:
		FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff, IO_ON);//2012,0821
		l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
		HS1Step = 1100;
		break;

	case 1100:
		l_ClipWaitTime[WORK_BUFFER_1][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_1][2] = l_ClipWaitTime[WORK_BUFFER_1][1] - l_ClipWaitTime[WORK_BUFFER_1][0];
		if(l_ClipWaitTime[WORK_BUFFER_1][2] < 0) l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_1][2] > 800)
		{
			FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
			HS1Step = 1200;
		}
		break;

	case 1200:
		l_ClipWaitTime[WORK_BUFFER_1][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_1][2] = l_ClipWaitTime[WORK_BUFFER_1][1] - l_ClipWaitTime[WORK_BUFFER_1][0];
		if(l_ClipWaitTime[WORK_BUFFER_1][2] < 0) l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_1][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff, IO_ON);//2012,0821
			l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
			HS1Step = 1300;
		}
		break;

	case 1300:
		l_ClipWaitTime[WORK_BUFFER_1][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_1][2] = l_ClipWaitTime[WORK_BUFFER_1][1] - l_ClipWaitTime[WORK_BUFFER_1][0];
		if(l_ClipWaitTime[WORK_BUFFER_1][2] < 0) l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_1][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
			HS1Step = 1400;
		}
		break;

	case 1400:
		l_ClipWaitTime[WORK_BUFFER_1][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_1][2] = l_ClipWaitTime[WORK_BUFFER_1][1] - l_ClipWaitTime[WORK_BUFFER_1][0];
		if(l_ClipWaitTime[WORK_BUFFER_1][2] < 0) l_ClipWaitTime[WORK_BUFFER_1][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_1][2] > 1000)
		{
			st_handler.n_hs1_fix = CTL_NO;
			HS1Step = 0;
		}
		break;
	}
}

void CRun_Rbt_Work_Buffer::Run_HS2_OnFix()
{
	switch(HS2Step)
	{
	case 0:
		if(st_handler.n_hs2_fix == CTL_YES)
		{
			HS2Step = 1000;
		}
		break;

	case 1000:
		FAS_IO.set_out_bit(st_io.o_buf2_f_hs_onoff, IO_ON);//2012,0821
		l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
		HS2Step = 1100;
		break;

	case 1100:
		l_ClipWaitTime[WORK_BUFFER_2][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_2][2] = l_ClipWaitTime[WORK_BUFFER_2][1] - l_ClipWaitTime[WORK_BUFFER_2][0];
		if(l_ClipWaitTime[WORK_BUFFER_2][2] < 0) l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_2][2] > 800)
		{
			FAS_IO.set_out_bit(st_io.o_buf2_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
			HS2Step = 1200;
		}
		break;

	case 1200:
		l_ClipWaitTime[WORK_BUFFER_2][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_2][2] = l_ClipWaitTime[WORK_BUFFER_2][1] - l_ClipWaitTime[WORK_BUFFER_2][0];
		if(l_ClipWaitTime[WORK_BUFFER_2][2] < 0) l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_2][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf2_f_hs_onoff, IO_ON);//2012,0821
			l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
			HS2Step = 1300;
		}
		break;

	case 1300:
		l_ClipWaitTime[WORK_BUFFER_2][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_2][2] = l_ClipWaitTime[WORK_BUFFER_2][1] - l_ClipWaitTime[WORK_BUFFER_2][0];
		if(l_ClipWaitTime[WORK_BUFFER_2][2] < 0) l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_2][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf2_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
			HS2Step = 1400;
		}
		break;

	case 1400:
		l_ClipWaitTime[WORK_BUFFER_2][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_2][2] = l_ClipWaitTime[WORK_BUFFER_2][1] - l_ClipWaitTime[WORK_BUFFER_2][0];
		if(l_ClipWaitTime[WORK_BUFFER_2][2] < 0) l_ClipWaitTime[WORK_BUFFER_2][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_2][2] > 1000)
		{
			st_handler.n_hs2_fix = CTL_NO;
			HS2Step = 0;
		}
		break;
	}
}

void CRun_Rbt_Work_Buffer::Run_HS3_OnFix()
{
	switch(HS3Step)
	{
	case 0:
		if(st_handler.n_hs3_fix == CTL_YES)
		{
			HS3Step = 1000;
		}
		break;

	case 1000:
		FAS_IO.set_out_bit(st_io.o_buf3_f_hs_onoff, IO_ON);//2012,0821
		l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
		HS3Step = 1100;
		break;

	case 1100:
		l_ClipWaitTime[WORK_BUFFER_3][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_3][2] = l_ClipWaitTime[WORK_BUFFER_3][1] - l_ClipWaitTime[WORK_BUFFER_3][0];
		if(l_ClipWaitTime[WORK_BUFFER_3][2] < 0) l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_3][2] > 800)
		{
			FAS_IO.set_out_bit(st_io.o_buf3_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
			HS3Step = 1200;
		}
		break;

	case 1200:
		l_ClipWaitTime[WORK_BUFFER_3][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_3][2] = l_ClipWaitTime[WORK_BUFFER_3][1] - l_ClipWaitTime[WORK_BUFFER_3][0];
		if(l_ClipWaitTime[WORK_BUFFER_3][2] < 0) l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_3][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf3_f_hs_onoff, IO_ON);//2012,0821
			l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
			HS3Step = 1300;
		}
		break;

	case 1300:
		l_ClipWaitTime[WORK_BUFFER_3][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_3][2] = l_ClipWaitTime[WORK_BUFFER_3][1] - l_ClipWaitTime[WORK_BUFFER_3][0];
		if(l_ClipWaitTime[WORK_BUFFER_3][2] < 0) l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_3][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf3_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
			HS3Step = 1400;
		}
		break;

	case 1400:
		l_ClipWaitTime[WORK_BUFFER_3][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_3][2] = l_ClipWaitTime[WORK_BUFFER_3][1] - l_ClipWaitTime[WORK_BUFFER_3][0];
		if(l_ClipWaitTime[WORK_BUFFER_3][2] < 0) l_ClipWaitTime[WORK_BUFFER_3][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_3][2] > 1000)
		{
			st_handler.n_hs3_fix = CTL_NO;
			HS3Step = 0;
		}
		break;
	}
}

void CRun_Rbt_Work_Buffer::Run_HS4_OnFix()
{
	switch(HS4Step)
	{
	case 0:
		if(st_handler.n_hs4_fix == CTL_YES)
		{
			HS4Step = 1000;
		}
		break;

	case 1000:
		FAS_IO.set_out_bit(st_io.o_buf4_f_hs_onoff, IO_ON);//2012,0821
		l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
		HS4Step = 1100;
		break;

	case 1100:
		l_ClipWaitTime[WORK_BUFFER_4][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_4][2] = l_ClipWaitTime[WORK_BUFFER_4][1] - l_ClipWaitTime[WORK_BUFFER_4][0];
		if(l_ClipWaitTime[WORK_BUFFER_4][2] < 0) l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_4][2] > 800)
		{
			FAS_IO.set_out_bit(st_io.o_buf4_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
			HS4Step = 1200;
		}
		break;

	case 1200:
		l_ClipWaitTime[WORK_BUFFER_4][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_4][2] = l_ClipWaitTime[WORK_BUFFER_4][1] - l_ClipWaitTime[WORK_BUFFER_4][0];
		if(l_ClipWaitTime[WORK_BUFFER_4][2] < 0) l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_4][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf4_f_hs_onoff, IO_ON);//2012,0821
			l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
			HS4Step = 1300;
		}
		break;

	case 1300:
		l_ClipWaitTime[WORK_BUFFER_4][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_4][2] = l_ClipWaitTime[WORK_BUFFER_4][1] - l_ClipWaitTime[WORK_BUFFER_4][0];
		if(l_ClipWaitTime[WORK_BUFFER_4][2] < 0) l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_4][2] > 700)
		{
			FAS_IO.set_out_bit(st_io.o_buf4_f_hs_onoff, IO_OFF);
			l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
			HS4Step = 1400;
		}
		break;

	case 1400:
		l_ClipWaitTime[WORK_BUFFER_4][1] = GetCurrentTime();
		l_ClipWaitTime[WORK_BUFFER_4][2] = l_ClipWaitTime[WORK_BUFFER_4][1] - l_ClipWaitTime[WORK_BUFFER_4][0];
		if(l_ClipWaitTime[WORK_BUFFER_4][2] < 0) l_ClipWaitTime[WORK_BUFFER_4][0] = GetCurrentTime();
		if(l_ClipWaitTime[WORK_BUFFER_4][2] > 1000)
		{
			st_handler.n_hs4_fix = CTL_NO;
			HS4Step = 0;
		}
		break;
	}
}