// Run_Worker_Buffer_1.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Worker_Buffer_1.h"
#include "ComizoaPublic.h"
#include "CtlBd_Function.h"
#include "FastechPublic_IO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Worker_Buffer_1
CRun_Worker_Buffer_1	Run_Worker_Buffer_1;
CRun_Worker_Buffer_1::CRun_Worker_Buffer_1()
{
}

CRun_Worker_Buffer_1::~CRun_Worker_Buffer_1()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Worker_Buffer_1 message handlers
void CRun_Worker_Buffer_1::Thread_Run()
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
		break;

	case dREINSTATE :
		Run_Reinstate();
		break;
	
	default:
		//dSTOP, dJAM, dLOTEND, dWARNING, dLOCK, dMAINT (장비가 동작하지 않는다면!)
		//OnLdModuleRbt_FinalPos(); // 현재 위치를 저장한다.
		if(st_handler.mn_menu_num == 501)  return;   //XYZ Robot motor 화면이면..		

		InitStep = 0;
		ReinstateStep = 0;
		break;
	}
}

void CRun_Worker_Buffer_1::Thread_Variable_Initial()
{
	InitStep = 0;
	RunStep = 0;
	ReinstateStep = 0;
}


void CRun_Worker_Buffer_1::Run_Init()
{
	int nRet_1 = CTLBD_RET_PROCEED, nRet_2 = CTLBD_RET_PROCEED;
	int nRet_3 = CTLBD_RET_PROCEED, nRet_4 = CTLBD_RET_PROCEED;

	if(st_handler.mn_init_state[INIT_WORK_BUFFER] != CTL_NO) return;

	switch(InitStep) 
	{
	case 0:
		InitStep = 1000;
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
// 			st_work.mn_run_status = dWARNING;
// 			CtlBdFunc.Alarm_Error_Occurrence(alarm.mn_count_mode, alarm.mn_type_mode, st_work.mn_run_status, alarm.mstr_pcode);
			CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1100:
		FAS_IO.OnCylinderAction(st_io.o_buf1_m_clamp_onoff, IO_ON);
		 FAS_IO.OnCylinderAction(st_io.o_buf1_hs_clamp_onoff, IO_ON);
		InitStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_buf1_m_clamp_onoff, st_io.i_buf1_m_clamp_on_chk, st_io.i_buf1_m_clamp_off_chk, 
			IO_ON, st_wait.n_on_wait_time[72], st_wait.n_limit_wait_time[72]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_buf1_hs_clamp_onoff, st_io.i_buf1_hs_clamp_on_chk, st_io.i_buf1_hs_clamp_off_chk, 
			IO_ON, st_wait.n_on_wait_time[72], st_wait.n_limit_wait_time[72]);
		if (nRet_1 == CTLBD_RET_GOOD && nRet_2 == CTLBD_RET_GOOD)
		{	//190105 1 19 "Work Buffer_1 로봇 모듈 Off 체크 에러".
			//190205 1 19 "Work Buffer_1 로봇 H/S Off 체크 에러".
			if(FAS_IO.get_in_bit(st_io.i_buf1_m_clamp_on_chk, IO_OFF) == IO_ON)
			{
				alarm.mstr_code = _T("190105");
				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정
				st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX
			}
			else if(FAS_IO.get_in_bit(st_io.i_buf1_hs_clamp_on_chk, IO_OFF) == IO_ON)
			{
				alarm.mstr_code = _T("190205");
				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정
				st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX
			}
			else
			{
				InitStep = 3000;
			}
		}
		else if(nRet_1 != CTLBD_RET_PROCEED && nRet_2 != CTLBD_RET_PROCEED)
		{	//190102 1 19 "Work Buffer_1 로봇 모듈 클램프 On 체크 에러".
			//190202 1 19 "Work Buffer_1 로봇 H/S 클램프 On 체크 에러".
			if(nRet_1 != CTLBD_RET_PROCEED) alarm.mstr_code = _T("190102");
			else							alarm.mstr_code = _T("190202");
			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정
			st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX
		}
		break;

	case 3000:
		st_handler.mn_init_state[INIT_WORK_BUFFER] = CTL_YES;
		InitStep = 0;
		break;
	}
}

void CRun_Worker_Buffer_1::Run_Move()
{
}

void CRun_Worker_Buffer_1::Run_Reinstate()
{
}
