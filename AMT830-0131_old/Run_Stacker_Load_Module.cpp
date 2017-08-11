// Run_Stacker_Load_Module.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Stacker_Load_Module.h"
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
/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Load_Module
CRun_Stacker_Load_Module Run_Stacker_Load_Module;
CRun_Stacker_Load_Module::CRun_Stacker_Load_Module()
{
	Thread_Variable_Initial();
}

CRun_Stacker_Load_Module::~CRun_Stacker_Load_Module()
{
}
/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Load_Module message handlers
void CRun_Stacker_Load_Module::Thread_Run()
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
		Run_Transfer();
		Run_Stacker1Move();
		Run_Stacker2Move();
		Run_Moving_Stacker();
		break;

	case dREINSTATE :
		break;
	
	default:
		//dSTOP, dJAM, dLOTEND, dWARNING, dLOCK, dMAINT (장비가 동작하지 않는다면!)

		if(st_handler.mn_menu_num == 501)  return;   //XYZ Robot motor 화면이면..		

		COMI.Set_MotStop(0, M_LDM_STACKER_1);
		CTL_Lib.mn_single_motmove_step[M_LDM_STACKER_1] = 0;
		COMI.Set_MotStop(0, M_LDM_STACKER_2);
		CTL_Lib.mn_single_motmove_step[M_LDM_STACKER_2] = 0;

		ReadyPosStep[0] = 0;
		ReadyPosStep[1] = 0;
		InitStep = 0;
		ReinstateStep = 0;
		break;
	}
}

void CRun_Stacker_Load_Module::OnLdModuleStacker_FinalPos()//20121126
{
	if(st_work.mn_reinstatement_mode[M_LDM_STACKER_1] == 0) //stop시 최초 한번만 기록하기 위함
	{
		st_work.d_reinstate_pos[0][M_LDM_STACKER_1] = COMI.md_cmdpos_backup[M_LDM_STACKER_1];
		st_work.d_reinstate_pos[0][M_LDM_STACKER_2] = COMI.md_cmdpos_backup[M_LDM_STACKER_2];
		st_work.d_reinstate_pos[0][M_LDM_STACKER_MOVE] = COMI.md_cmdpos_backup[M_LDM_STACKER_MOVE];

		st_work.mn_TrayClampStatusBackup[LDMODULE_SITE][1] = FAS_IO.get_out_bit(st_io.o_m_stacker2_tray_clamp_onoff, IO_OFF);
		
		st_work.mn_reinstatement_mode[M_LDM_STACKER_1] = 1;	//최종 위치를 기록 했다
	}
}

void CRun_Stacker_Load_Module::Thread_Variable_Initial()
{
	int i;
	RunS1Step = 0;
	RunS2Step = 0;
	RunMStep = 0;
	InitStep = 0;
	TrayBackStep = 0;
	TrayMoveStep = 0;
	RunTransStep = 0;
	mn_Moving_stacker = NO;
	mn_stacker1_init_chk = NO;
	mn_stacker2_init_chk = NO;
	st_handler.n_Init_Stacker = CTL_READY;

	ReadyPosStep[0] = ReadyPosStep[1] = 0;
	MotorMoveStep[0] = MotorMoveStep[1] = 0;
	mn_stacker_move_flag = CTL_NO;
	mn_stacker_Up = CTL_NO;

	st_sync.mn_ld_module_tray_supply[0] = st_sync.mn_ld_module_tray_supply[1] = CTL_CLEAR;
	mn_stacker_updn_cyliner[0] = mn_stacker_updn_cyliner[1] = CYLINDER_OFF;

	st_sync.n_lotend_module_ldtray_site = CTL_NO;
	st_sync.n_lotend_module_uldtray_site = CTL_NO;
	st_sync.n_lotend_module_movingtray_site = CTL_NO;
	st_sync.mn_ld_module_tray_change[0] = st_sync.mn_ld_module_tray_change[1] = CTL_NO;			// 교체할 트레이가 없다

	mn_LeakM_LotEnd[0] = mn_LeakM_LotEnd[1] = mn_LeakM_LotEnd[2] = mn_LeakM_LotEnd[3] = NO;
	for(i = 0; i < 4; i++)
	{
		st_sync.n_lot_reready[i] = NO;
	}
}

void CRun_Stacker_Load_Module::Run_Init()
{
	int i,nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	double dCurrentPos[8] = {0,};
	int ratio = 50, num = 0;

	if(st_handler.mn_init_state[INIT_LD_STACKER] != CTL_NO) return;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_LDM_STACKER_1);
		CTL_Lib.mn_single_motmove_step[M_LDM_STACKER_1] = 0;
		COMI.Set_MotStop(0, M_LDM_STACKER_2);
		CTL_Lib.mn_single_motmove_step[M_LDM_STACKER_2] = 0;
		COMI.Set_MotStop(0, M_LDM_STACKER_MOVE);
		CTL_Lib.mn_single_motmove_step[M_LDM_STACKER_MOVE] = 0;
		ReadyPosStep[0] = ReadyPosStep[1] = 0;
		COMI.mn_home_step[M_LDM_STACKER_1] = 0;
		COMI.mn_home_step[M_LDM_STACKER_2] = 0;
		COMI.mn_home_step[M_LDM_STACKER_MOVE] = 0;
		return;
	}
	dCurrentPos[0] = COMI.Get_MotCurrentPos(M_LDM_STACKER_1);
	dCurrentPos[1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_2);
	dCurrentPos[2] = COMI.Get_MotCurrentPos(M_LDM_STACKER_MOVE);

	switch(InitStep)
	{
	case 0:
		initial_smove = NO;
		if(st_handler.nBootFlagCheck == CTL_YES || 1)
		{//홈밍을 조심할 것
			dCurrentPos[0] = COMI.Get_MotCurrentPos(M_LDM_STACKER_2);
			dCurrentPos[1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_MOVE);
			dCurrentPos[2] = COMI.Get_MotCurrentPos(M_HS_F_STACKER_2);
			dCurrentPos[3] = COMI.Get_MotCurrentPos(M_HS_F_STACKER_MOVE);
			dCurrentPos[4] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_2);
			dCurrentPos[5] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_MOVE);
			dCurrentPos[6] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_1);
			dCurrentPos[7] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_MOVE);
			if(dCurrentPos[0] >= 0.0 && dCurrentPos[0] < 0.1 &&
				dCurrentPos[1] >= 0.0 && dCurrentPos[0] < 0.1 &&
				dCurrentPos[2] >= 0.0 && dCurrentPos[0] < 0.1 &&
				dCurrentPos[3] >= 0.0 && dCurrentPos[0] < 0.1 &&
				dCurrentPos[4] >= 0.0 && dCurrentPos[0] < 0.1 &&
				dCurrentPos[5] >= 0.0 && dCurrentPos[0] < 0.1 &&
				dCurrentPos[6] >= 0.0 && dCurrentPos[0] < 0.1 &&
				dCurrentPos[7] >= 0.0 && dCurrentPos[0] < 0.1)
			{
				st_handler.n_Init_Stacker = CTL_YES;
				initial_smove = YES;
				mn_retry = 0;
				InitStep = 1;
			}
			else
			{
				st_handler.n_Init_Stacker = CTL_NO;
				mn_retry = 0;
				InitStep = 5;
			}
		}
		break;

	case 1:
		if(COMI.Get_MotIOSensor(M_LDM_STACKER_MOVE, MOT_SENS_ELP) == CTLBD_RET_GOOD ||
			COMI.Get_MotIOSensor(M_LDM_STACKER_MOVE, MOT_SENS_ELM) == CTLBD_RET_GOOD)
		{
			num++;
		}
		if(COMI.Get_MotIOSensor(M_HS_F_STACKER_MOVE, MOT_SENS_ELP) == CTLBD_RET_GOOD ||
			COMI.Get_MotIOSensor(M_HS_F_STACKER_MOVE, MOT_SENS_ELM) == CTLBD_RET_GOOD)
		{
			num++;
		}
		if(COMI.Get_MotIOSensor(M_HS_B_STACKER_MOVE, MOT_SENS_ELP) == CTLBD_RET_GOOD ||
			COMI.Get_MotIOSensor(M_HS_B_STACKER_MOVE, MOT_SENS_ELM) == CTLBD_RET_GOOD)
		{
			num++;
		}
		if(COMI.Get_MotIOSensor(M_ULDM_STACKER_MOVE, MOT_SENS_ELP) == CTLBD_RET_GOOD ||
			COMI.Get_MotIOSensor(M_ULDM_STACKER_MOVE, MOT_SENS_ELM) == CTLBD_RET_GOOD)
		{
			num++;
		}
		if(num >= 4)
		{
			InitStep = 5;
		}
		else
		{//993000 0 99 "FeedBack 값이 0입니다. 각 stacker move를 + 또는 - 리밋으로 이동해 주세요."
			sprintf(mc_jamcode,"993000");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1000, st_work.mn_run_status, mc_jamcode);
			COMI.Set_MotPower(M_LDM_STACKER_MOVE, CTL_NO);
			COMI.Set_MotPower(M_HS_F_STACKER_MOVE, CTL_NO);
			COMI.Set_MotPower(M_HS_B_STACKER_MOVE, CTL_NO);
			COMI.Set_MotPower(M_ULDM_STACKER_MOVE, CTL_NO);
		}
		break;

	case 5:		
		if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		{//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"000202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1000, st_work.mn_run_status, mc_jamcode);
		}
		else if(FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF) == IO_ON)
		{//010202 0 01 "Stacker2 레일에 트레이가 있습니다.
			sprintf(mc_jamcode,"010202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1001, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			InitStep = 10;
		}
		break;		

	case 10:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			InitStep = 20;
		}
		break;

	case 20:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			InitStep = 30;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//010208 0 01 "Stocker2 Latch Clamp Tray Off 에러."
				sprintf(mc_jamcode, "010208");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1002, st_work.mn_run_status, mc_jamcode);
				InitStep = 10;
			}
			else
			{
				InitStep = 10;
			}
		}
		break;

	case 30:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			InitStep = 40;
		}
		break;

	case 40:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			InitStep = 50;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{	
			//000203 0 00 "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"000203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1003, st_work.mn_run_status, mc_jamcode);
		}
		break;
		
	case 50:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(1, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			InitStep = 60;
		}
		break;

	case 60:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(1, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			InitStep = 1000;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{	
			//000203 0 00 "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"000203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1004, st_work.mn_run_status, mc_jamcode);
		}
		break;
		
	case 1000:
		InitStep = 1100;
		break;
		
		if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		{//000100 0 00 "Stacker1에 트레이가 존재합니다.
			sprintf(mc_jamcode,"000100");
			CTL_Lib.Alarm_Error_Occurrence(1005, st_work.mn_run_status, mc_jamcode);
		}
		else if(FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF) == IO_ON)
		{//010100 0 00 "Stacker2에 트레이가 존재합니다.
			sprintf(mc_jamcode,"010100");
			CTL_Lib.Alarm_Error_Occurrence(1006, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			InitStep = 1100;
		}
		break;

	case 1100:
		InitStep = 1300;
		break;
		FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_ON);
		InitStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker_guide_clamp_onoff, st_io.i_m_stacker_guide_unclamp_off_chk, st_io.i_m_stacker_guide_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[9], st_wait.n_limit_wait_time[9]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//000102 0 00 "Stacker1이 guide clamp가 On이 되지 않습니다."
			sprintf(mc_jamcode,"000102");
			CTL_Lib.Alarm_Error_Occurrence(1007, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 1300:
		InitStep = 1500;
		break;
		FAS_IO.OnCylinderAction(st_io.o_m_stacker_tray_pusher_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_m_stacker_tray_pusher_bwd_onoff, IO_ON);
		InitStep = 1400;
		break;

	case 1400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker_tray_pusher_bwd_onoff, st_io.i_m_stacker_tray_pusher_bwd_chk, st_io.i_m_stacker_tray_pusher_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[3], st_wait.n_limit_wait_time[3]);
		nRet_1 = RET_GOOD;
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//000105 0 00 "stacker 트레이 pusher backward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"000105");
			CTL_Lib.Alarm_Error_Occurrence(1008, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 1500:
		FAS_IO.OnCylinderAction(st_io.o_m_stacker1_rail_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_m_stacker1_rail_bwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_rail_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_rail_bwd_onoff, IO_ON);
		InitStep = 1600;
		break;

	case 1600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker1_rail_bwd_onoff, st_io.i_m_stacker1_rail_bwd_chk, st_io.i_m_stacker1_rail_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[6], st_wait.n_limit_wait_time[6]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker2_rail_bwd_onoff, st_io.i_m_stacker2_rail_bwd_chk, st_io.i_m_stacker2_rail_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[8], st_wait.n_limit_wait_time[8]);
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			InitStep = 1700;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//000108 0 00 "stacker1 레일 forward가 동작이 되지 않습니다."
		 //000109 0 00 "stacker1 레일 backward가 동작이 되지 않습니다."
			if(nRet_1 != RET_PROCEED) sprintf(mc_jamcode, "000108");
			else                      sprintf(mc_jamcode, "010109");
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
			CTL_Lib.Alarm_Error_Occurrence(1009, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 1700:
		//FAS_IO.OnCylinderAction(st_io.o_m_stacker1_up_onoff, IO_OFF);
		//FAS_IO.OnCylinderAction(st_io.o_m_stacker1_dn_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_up_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_m_stacker2_dn_onoff, IO_ON);
		InitStep = 1800;
		break;

	case 1800:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker1_dn_onoff, st_io.i_m_stacker1_dn_chk, st_io.i_m_stacker1_up_chk,
			IO_ON, st_wait.n_on_wait_time[1], st_wait.n_limit_wait_time[1]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker2_dn_onoff, st_io.i_m_stacker2_dn_chk, st_io.i_m_stacker2_up_chk,
			IO_ON, st_wait.n_on_wait_time[1], st_wait.n_limit_wait_time[1]);
		nRet_1 = RET_GOOD;
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			InitStep = 1900;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//000108 0 00 "stacker1 업이 동작을 하지 않습니다."
		 //000109 0 00 "stacker1 다운이 동작을 하지 않습니다."
			if(nRet_1 != RET_PROCEED) sprintf(mc_jamcode, "000109");
			else                      sprintf(mc_jamcode, "010109");
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
			CTL_Lib.Alarm_Error_Occurrence(1010, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 1900:
		if(initial_smove == YES)
		{
			if(COMI.Get_MotIOSensor(M_LDM_STACKER_MOVE, MOT_SENS_ELP) == CTLBD_RET_GOOD)
			{
				InitStep = 2300;
			}
			else if(COMI.Get_MotIOSensor(M_LDM_STACKER_MOVE, MOT_SENS_ELM) == CTLBD_RET_GOOD)
			{
				InitStep = 2100;
			}
		}
		else
		{
			InitStep = 2000;
		}
		break;
	
	// 
	case 2000:
		if(dCurrentPos[2] < st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS])//stacker move 
		{
			InitStep = 2010;
		}
		else if(dCurrentPos[2] > st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS] &&
			dCurrentPos[2] < st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS])
		{
			InitStep = 2010;
		}
		else //st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS];
		{
			InitStep = 2300;
		}
		break;

	case 2010:
		if(dCurrentPos[1] <= st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN])
		{
			InitStep = 2100;
		}
		else if(dCurrentPos[1] > st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN] && 
			dCurrentPos[1] < st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_UP] - st_motor[M_LDM_STACKER_2].mn_allow)
		{
			InitStep = 2300;
		}
		else //dCurrentPos[1] > st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT]
		{
			InitStep = 2100;
		}
		break;

	case 2100:
		nRet_1 = COMI.HomeCheck_Mot(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 2200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2100;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1011, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2200:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS], st_basic.nManualSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_retry = 0;
			init_stacker_move = YES;
			InitStep = 2300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2200;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1011, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;


	// 모듈 스테커 1번 홈을 진행한다. 
	case 2300:
		if(st_work.mn_run_status == dLOTREADY)//2014.0415
		{
			nRet_1 = COMI.Start_SingleMove(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_SAFETY], st_basic.nRunSpeed);
			if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
			{
				init_stacker_1 = YES;
				InitStep = 2350;
			}
			else if (nRet_1 == CTLBD_RET_RETRY)
			{
				InitStep = 2300;
			}
			else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
			{
				mn_retry++;
				if(mn_retry > 5)
				{
					mn_retry = 0;
					st_work.mn_run_status = dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1011, st_work.mn_run_status, COMI.mc_alarmcode);
				}
			}
		}
		else
		{
			nRet_1 = COMI.HomeCheck_Mot(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].mn_homecheck_method, MOT_TIMEOUT);
			if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
			{
				init_stacker_1 = YES;
				InitStep = 2400;
			}
			else if (nRet_1 == CTLBD_RET_RETRY)
			{
				InitStep = 2300;
			}
			else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
			{
				mn_retry++;
				if(mn_retry > 5)
				{
					mn_retry = 0;
					st_work.mn_run_status = dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1011, st_work.mn_run_status, COMI.mc_alarmcode);
					COMI.mn_home_step[M_LDM_STACKER_1] = 0;
				}
				else if(mn_retry == 3)
				{
					COMI.mn_home_step[M_LDM_STACKER_1] = 0;
				}
			}
		}
		break;

	////2014.0415
	case 2350:
			nRet_1 = COMI.Check_SingleMove(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_SAFETY]);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				InitStep = 2400;
			}
			else if(nRet_1 == CTLBD_RET_ERROR)
			{
				InitStep = 2300;
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1011, st_work.mn_run_status, COMI.mc_alarmcode);
			}
		break;
	////

	// 모듈 스테커 2번 홈을 진행한다. 
	case 2400:
		nRet_1 = COMI.HomeCheck_Mot(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);
		if(nRet_1 == CTLBD_RET_GOOD)			
		{
			init_stacker_2 = YES;
			InitStep = 2500;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2400;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1012, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;



	// Home 완료 플레그 확인 [ 전체 home 상태 확인 ]
	case 2500:
		if(init_stacker_move == YES && init_stacker_1 == YES && init_stacker_2 == YES)
		{
			InitStep = 2600;
		}
		else
		{
			InitStep = 2100;
		}	
		break;

	// 트레이 정보 초기화 진행 
	case 2600:
		for (i = 0; i < st_traybuffer[LDMODULE_SITE].i_loader_row; i++)
		{
			st_modulemap.LoadTray[0][i] = DVC_NO;
			st_modulemap.LoadTray[1][i] = DVC_NO;
		}
		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_LOADER_PICKER);			// 피커 정보 
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_LOADER_TRAY);			// 피커 정보 
		}
		InitStep = 3000;
		break;
		
	case 3000:
		if(st_handler.n_lotend_ready == CTL_READY)
		{
			if(st_handler.mn_init_state[INIT_LEFTHS_STACKER] == CTL_YES && st_handler.mn_init_state[INIT_RIGHTHS_STACKER] == CTL_YES &&
				st_handler.mn_init_state[INIT_ULD_STACKER] == CTL_YES)
			{
				st_handler.mn_init_state[INIT_LD_STACKER] = CTL_YES;
				st_handler.n_lotend_ready = CTL_NO;
				//994001 0 99 "자재및 모듈을 투입해 주세요. Lot 수량이 맞지 않습니다."
				sprintf(mc_jamcode,"994001");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1013, st_work.mn_run_status, mc_jamcode);
				if (st_handler.cwnd_main != NULL)
				{
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOT_RESTART, 0);
				}

				InitStep = 0;
			}
		}
		else
		{
			st_handler.mn_init_state[INIT_LD_STACKER] = CTL_YES;
			InitStep = 0;
		}
		break;		
		
	}
}

void CRun_Stacker_Load_Module::Run_Transfer()
{
	int nRet = 0, nRet_1 = VARIABLE_INIT;
	int i=0,j=0;

	Func.ThreadFunctionStepTrace(50, RunTransStep);

	switch(RunTransStep)
	{
	case -1:
		Thread_Variable_Initial();
		RunTransStep = 0;
		RunS1Step = 0;
		RunS2Step = 0;
		RunMStep = 0;
		break;

	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_module_ldrbt == CTL_YES)
			{
				st_sync.n_lotend_module_ldtray_site = CTL_YES;
				mn_LeakM_LotEnd[1] = NO;
			}
			if(mn_LeakM_LotEnd[1] == NO && st_sync.n_lotend_module_ldtray_site == CTL_NO)
			{				
				RunTransStep = 100;
			}
		}
		break;

	case 100:
		if(st_sync.n_lotend_module_ldtray_site == CTL_YES) return;
		//Stacker#1로더 트레이가 준비됐다
		//먼저 Stacker Move가 안으로 들어가 대기한다.
		if(st_sync.mn_ld_module_tray_supply[0] == CTL_READY)
		{
			RunTransStep = 900;
			RunTransStep = 1000;
		}
		else if (st_sync.mn_ld_module_tray_supply[0] == CTL_NOTREADY)		// 텅 비었을 경우에...
		{			
			RunTransStep = 10000;
		}
		break;

		//먼저 Stacker#2로 이동하여 기다린다.
	case 900:
		if(mn_stacker_move_flag == CTL_NO)
		{
			nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS], st_basic.nRunSpeed);
			if(nRet_1 == BD_GOOD)
			{
				mn_stacker_move_flag = CTL_YES;
				RunTransStep = 1000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				RunTransStep = 2300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				st_work.mn_run_status = dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1016, st_work.mn_run_status, COMI.mc_alarmcode);
			}
		}
		else
		{
			RunTransStep = 1000;
		}
		break;
		
	case 1000:
		//트레이를 올려놓고 기다린다.
		nRet_1 = LD_Module_TrayBack();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 1100;
		}
		break;

	case 1100:
		if(st_sync.mn_ld_module_tray_supply[0] == CTL_READY)//SD 대기위치에 있으면
		{
			st_sync.mn_ld_module_tray_supply[0] = CTL_CHANGE;
			RunTransStep = 1200;
		}
		else if (st_sync.mn_ld_module_tray_supply[0] == CTL_NOTREADY)		// 텅 비었을 경우에...
		{			
			RunTransStep = 10000;
		}
		break;

	case 1200:		
		if(st_sync.mn_ld_module_tray_supply[0] == CTL_LOCK)// 교체 완료
		{
			RunTransStep = 1300;
		}
		else if(st_sync.mn_ld_module_tray_supply[0] == CTL_NOTREADY)
		{
			RunTransStep = 10000;
		}
		break;

	case 1300://stacker up일 경우 대비해서 변수
		if (FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON) == IO_ON || 
			mn_stacker_updn_cyliner[M_STACKER_1] == CYLINDER_ON)
		{
			//for (i = 0; i < st_traybuffer[LDMODULE_SITE].i_loader_col; i++)
			//{
				for (j = 0; j < st_traybuffer[LDMODULE_SITE].i_loader_row; j++)
				{
					st_modulemap.LoadTray[0][j] = DVC_YES; 
				}
			//}
			st_sync.mn_ld_module_tray_change[0] = CTL_YES;
			RunTransStep = 2000;
		}
		else
		{
			//000101 0 00 "Stacker1에 트레이가 존재하지 않습니다."
			sprintf(mc_jamcode,"000101");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1021, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 2000:		
		if(st_sync.mn_ld_module_tray_supply[1] == CTL_READY)//교체 완료 또는 Tray 기다림
		{
			RunTransStep = 2100;
			RunTransStep = 2110;
		}
		break;

	case 2100:
		if(st_sync.n_lotend_module_ldstacker_site == CTL_NO)
		{
			RunTransStep = 2110;
		}
		else
		{
			if(st_sync.mn_ld_module_tray_supply[0] == CTL_NOTREADY)
			{
				RunTransStep = 100;
			}
		}
		break;

	case 2110:
		nRet_1 = LD_Module_TrayMove();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 2200;
		}
		break;

	case 2200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_ON);			
		if (st_basic.n_mode_device != 1)		nRet_1 = IO_ON;
		
		if(nRet_1 == IO_ON)
		{
			//for (i = 0; i < st_traybuffer[LDMODULE_SITE].i_loader_col; i++)
			//{
				for (j = 0; j < st_traybuffer[LDMODULE_SITE].i_loader_row; j++)
				{
					st_modulemap.LoadTray[1][j] = st_modulemap.LoadTray[0][j]; 
					st_modulemap.LoadTray[0][j] = DVC_NO;
				}
			//}
			st_sync.mn_ld_module_tray_change[0] = CTL_NO;
			st_sync.mn_ld_module_tray_change[1] = CTL_YES;  // 교체할 트레이가 있다
			if(st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_LOADER_TRAY);			// 피커 정보 
			}
			RunTransStep = 3000;
		}
		else
		{
			//010101 0 01 "Stacker2에 트레이가 존재지 않습니다."
			sprintf(mc_jamcode,"010101");
			CTL_Lib.Alarm_Error_Occurrence(1022, CTL_dWARNING, mc_jamcode);
		}
		break;

	case 3000:
		st_sync.mn_ld_module_tray_supply[1] = CTL_LOCK;
		RunTransStep = 3100;
		break;

	case 3100:
		if(st_sync.mn_ld_module_tray_supply[1] == CTL_FREE)
		{
			RunTransStep = 100;			
		}
		break;		
		
//	case 2000:
//		if(st_sync.n_module_ldrbt_tray_req[0] == CTL_REQ)
//		{
//			st_sync.mn_ld_module_tray_supply[1] = CTL_CHANGE;
//			RunTransStep = 2100;
//		}
//		else
//		{//120100 0 12 "Module RBT 동작 상태가 다릅니다."
//			alarm.mstr_code = "000105";
//			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
//			alarm.mn_alarm_assign_section = 1105;
//						//CTL_Lib.Alarm_Error_Occurrence(alarm.mn_count_mode, st_work.mn_run_status, alarm.mstr_code);
//			if(st_handler.cwnd_main != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"[Module TrayTransfer] tray_req[0] = %d.", st_sync.n_module_ldrbt_tray_req[0]);
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
//			}
//			RunTransStep = 0;
//		}
//		break;
//
//	case 2100:
//		if(st_sync.mn_ld_module_tray_supply[1] == CTL_LOCK)
//		{
//			RunTransStep = 2000;
//		}
//		break;
//
//	case 2000:
//
//
//	case 2200:
//		st_sync.mn_ld_module_tray_change = NO;
//		st_sync.mn_ld_module_tray_supply[1] = CTL_FREE;
//		RunTransStep = 3000;
//		break;
//
//	case 3000:
//		if(st_sync.mn_ld_module_tray_supply[1] == CTL_LOCK)
//		{			
//			st_traybuffer[LDMODULE_SITE].i_loader_row = 0;
//			st_sync.n_module_ldrbt_tray_req[2] = st_traybuffer[LDMODULE_SITE].i_loader_row;
//			RunTransStep = 100;
//		}
//		break;

	case 10000:
		nRet_1 = LD_Module_TrayBack();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 11000;
//			if(st_work.n_loadlot_count[LDMODULE_SITE] >= st_work.nMdlInputCount[0][0])
//			{
//				RunTransStep = 11000;
//			}
//			else//제품 요구시
//			{
//				//st_sync.mn_ld_module_tray_supply[0] = CTL_REQ;
//				RunTransStep = 100;
//			}
		}
		break;

	case 11000:
		if(st_sync.n_lotend_module_ldrbt != CTL_YES)
		{
			st_sync.n_lotend_module_ldtray_site = CTL_NO;		// 일단 LOT END를 시키자.
			mn_LeakM_LotEnd[1] = YES;
			RunTransStep = 0;
		}
		else
		{
			st_sync.n_lotend_module_ldtray_site = CTL_YES;		// 일단 LOT END를 시키자.
			RunTransStep = 100;
		}
		break;
	}
}

void CRun_Stacker_Load_Module::Run_Stacker1Move()
{
	// 동작 방법.
	// 1. Tray Transfer에서 Tray를 바꿔 달라고 할때까지 대기.
	// 2. 교체 요청이 오면 Tray 유무 확인.
	// 3. 있으면 Tray 교체
	// 4. 없으면 없다고 보고.

	int nRet_1, nRet_2, nRet_3;
	int mn_MotorNum, mn_StackerPos;

	mn_MotorNum = M_LDM_STACKER_1;
	mn_StackerPos = M_STACKER_1;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_LDM_STACKER_1);
		CTL_Lib.mn_single_motmove_step[M_LDM_STACKER_1] = 0;
		ReadyPosStep[mn_StackerPos] = 0;
		COMI.mn_home_step[M_LDM_STACKER_1] = 0;
		return;
	}

	Func.ThreadFunctionStepTrace(51, RunS1Step);

	switch(RunS1Step)
	{
	case 0:		
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_work.nMdlInputCount[0][0] <= 0)
			{//991001 0 99 "작업할 Lot 개수를 입력하지 않았습니다."
				sprintf(mc_jamcode, "991001");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1089, st_work.mn_run_status, mc_jamcode);	
			}
			if(st_sync.n_lotend_module_ldrbt == CTL_YES)
			{
				st_sync.n_lotend_module_ldstacker_site = CTL_YES;
				mn_LeakM_LotEnd[0] = NO;
			}
			if(mn_LeakM_LotEnd[0] == NO && st_sync.n_lotend_module_ldstacker_site == CTL_NO)
			{
				RunS1Step = 10;
			}
		}
		else //if(st_work.mn_lot_start != CTL_YES)
		{//991000 0 99 "Lot이 Start가 되지 않았습니다."
			sprintf(mc_jamcode, "991000");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1029, st_work.mn_run_status, mc_jamcode);	
		}

		break;

	case 10:
		//000500 0 01 "Load Module Slide에 트레이가 존재하지 않습니다."
		//000501 0 01 "Load Module Slide에 트레이를 제거해 주세요."
		//000502 0 01 "Unload Module Slide에 트레이가 존재하지 않습니다."
		//000503 0 01 "Unload Module Slide에 트레이를 제거해 주세요."
		//nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_tray_chk, IO_ON);
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_m_uld_slide_tray_chk, IO_OFF);
		if(nRet_1 == IO_OFF || nRet_2 == IO_ON || nRet_3 == IO_ON)
		{
			if(nRet_1 == IO_OFF)
			{//000101 0 00 "Stacker1에 트레이가 존재하지 않습니다."
				sprintf(mc_jamcode, "000500"); 
			}
			else if(nRet_2 == IO_ON)
			{//010100 0 01 "Stacker2에 트레이가 존재합니다.제거해 주세요."
				sprintf(mc_jamcode, "010100"); 
			}
			else if(nRet_3 == IO_ON)
			{//000503 0 01 "Unload Module Slide에 트레이를 제거해 주세요."
				sprintf(mc_jamcode, "000503"); 
			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1030, st_work.mn_run_status, mc_jamcode);	
		}
		else
		{
			RunS1Step = 20;
		}
		break;

	case 20:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_ON) 
		{
			//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
			sprintf(mc_jamcode, "000202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1201, st_work.mn_run_status, mc_jamcode);	
		}
		else
		{
			RunS1Step = 30;
		}
		break;

	case 30:
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			RunS1Step = 100;
			break;
		}

		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_onoff_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_m_uld_slide_onoff_chk, IO_OFF);
		if(nRet_1 == IO_ON && nRet_2 == IO_ON)
		{
			mn_retry1 = 0;
			RunS1Step = 100;
		}
		else
		{//000802 0 01 "Load Module Slide가 Open되어 있습니다. Close 해 주세요."
		 //000804 0 01 "Unload Module Slide가 Open되어 있습니다. Close 해 주세요."
			if(nRet_1 == IO_OFF)	sprintf(mc_jamcode, "000802");
			else				sprintf(mc_jamcode, "000804");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1030, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 100:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS1Step = 110;
		}
		break;

	case 110:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			RunS1Step = 200;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry1++;
			if(mn_retry1 > 3)
			{
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode, "000204");			
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1031,  st_work.mn_run_status, mc_jamcode);
				RunS1Step = 30;
			}
			else
			{
				RunS1Step = 30;
			}
		}
		break;

	case 200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
		
		if (nRet_1 == IO_OFF)
		{
			RunS1Step = 300;
		}
		else
		{
			//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
			sprintf(mc_jamcode, "000202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1032, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 300:
		mn_stacker1_init_chk = YES;
		RunS1Step = 1000;
		break;

		st_sync.n_module_guide_tray_stacker[0] = CTL_REQ;
		RunS1Step = 400;
		break;

	case 400:
		if(st_sync.n_module_guide_tray_stacker[1] == CTL_LOCK)
		{
			st_sync.n_module_guide_tray_stacker[0] = CTL_CLEAR;
			mn_stacker1_init_chk = YES;
			RunS1Step = 1000;
		}
		break;

	case 1000:
		nRet_1 = OnModuleStackerReadyPos(0,1); //SD 대기
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			st_sync.mn_ld_module_tray_supply[0] = CTL_READY;
			RunS1Step = 1100;
		}
		else if(nRet_1 == CTL_FULL)
		{
			//000302 1 01 "Loader Stocker 1 Tray Full 에러."
			sprintf(mc_jamcode, "000302"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1035, st_work.mn_run_status, mc_jamcode);	
			RunS1Step = 1000;
		}
		else if(nRet_1 == CTLBD_RET_SKIP) //스태커에 트레이가 없을때 
		{
			st_sync.n_lotend_module_ldstacker_site = CTL_YES;
			RunS1Step = 1100;
		}
		break;


	case 1100:
		if(mn_LeakM_LotEnd[1] == YES || st_sync.n_lotend_module_uldstacker_site == CTL_YES || (COMI.mn_simulation_mode == 1 &&(st_work.nMdlInputCount[0][0] == st_work.nMdlPassCount[0][0])))
		{
			//일단 초기화 후 제품을 요구하느지 체크 한다
			 RunS1Step = 10000;
		}
		else if(st_sync.mn_ld_module_tray_supply[0] == CTL_CHANGE)//stacker2 move 
		{
			RunS1Step = 1200;
		}
		break;

	case 1200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_ON)
		{
			//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"000202");
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			CTL_Lib.Alarm_Error_Occurrence(1036, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			RunS1Step = 1300;
		}
		break;

	case 1300:
		if ((FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_fwd_chk + (mn_StackerPos*3), IO_OFF) == IO_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_bwd_chk + (mn_StackerPos*3), IO_ON) == IO_ON))	// Off 상태.
		{
			ReadyPosStep[mn_StackerPos] = 0;
			RunS1Step = 1500;
		}
		else
		{
			mn_retry1 = 0;
			nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
			{
				RunS1Step = 1400;
			}
		}
		break;

	case 1310:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS1Step = 1400;
		}
		break;

	case 1400:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			ReadyPosStep[mn_StackerPos] = 0;
			RunS1Step = 1500;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry1++;
			if(mn_retry1 > 3)
			{
				mn_retry1 = 0;
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"000204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1037, st_work.mn_run_status, mc_jamcode);
				RunS1Step = 1310;
			}
			else
			{
				RunS1Step = 1310;
			}
		}
		break;

	case 1500:
		nRet_1 = OnModuleStackerReadyPos(0, 0); //트레이 하나 Up한다
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 1600;
		}
		else if (nRet_1 == CTLBD_RET_SKIP)
		{
			RunS1Step = 1100;
		}
		break;

	case 1600:
		nRet_1 = COMI.Get_MotIOSensor(mn_MotorNum, MOT_SENS_SD);
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 1700;	
		}
		else
		{
			RunS1Step = 1700;
		}
		break;	

	case 1700:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			RunS1Step = 1800;//20121019
		}
		break;

	case 1800:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			ReadyPosStep[mn_StackerPos] = 0;
			RunS1Step = 2000;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{	
			//000203 0 00 "Stacker1 레일푸셔 forward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"000203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1038, st_work.mn_run_status, mc_jamcode);
			RunS1Step = 1700;
		}
		break;

	case 2000:
		nRet_1 = OnModuleStackerReadyPos(0, 2); //트레 하나 Dn한다
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 2100;
		}
		else if (nRet_1 == CTLBD_RET_SKIP)
		{
			RunS1Step = 1100;
		}
		break;

	case 2100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON);
		
		if (nRet_1 == IO_ON || st_basic.n_mode_device == 2)	// 트레이가 있다
		{
			RunS1Step = 3000;
		}
		else
		{
			//000201 0 00 "Stacker1 레일에 트레이가 존재하지 않습니다."
			sprintf(mc_jamcode, "000201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1039, st_work.mn_run_status, mc_jamcode);
			RunS1Step = 2200;
		}
		break;

	case 2200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk, IO_ON);
		
		if (nRet_1 == IO_ON || st_basic.n_mode_device == 2)	// 트레이가 있다
		{
			RunS1Step = 3000;
		}
		else
		{
			RunS1Step = 1200;
		}
		break;
		

	case 3000:
		st_sync.mn_ld_module_tray_supply[mn_StackerPos] = CTL_LOCK;		// 교체 끝났다고 설정.
		//트레이 정보 
		RunS1Step = 1000;
		break;

	case 10000:
		if(FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_OFF) == IO_ON)
		{
			//000100 0 00 "Stacker1에 트레이가 존재합니다.제거해 주세요."
			sprintf(mc_jamcode, "000100"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1329, st_work.mn_run_status, mc_jamcode);	
		}
		else
		{
			RunS1Step = 10010;
		}
		break;

	case 10010:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT], st_basic.nRunSpeed);
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 10100;
		}
		else if (nRet_1 == CTLBD_RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1040, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 10100:
		if(st_sync.n_lotend_module_ldrbt != CTL_YES)
		{
			st_sync.n_lotend_lefths_ldstacker_site = CTL_NO;
			mn_LeakM_LotEnd[0] = YES;
			RunS1Step = 0;
		}
		else
		{
			st_sync.n_lotend_module_ldstacker_site = CTL_YES;
			RunS1Step = 0;
		}
		break;		

	}


}

void CRun_Stacker_Load_Module::Run_Stacker2Move()
{
	// 동작 방법.
	// 1. Tray Transfer에서 Tray를 바꿔 달라고 할때까지 대기.
	// 2. 교체 요청이 오면 Tray 유무 확인.
	// 3. 있으면 Tray 교체
	// 4. 없으면 없다고 보고.

	int nRet_1, nRet_2;
	int mn_MotorNum, mn_StackerPos;

	mn_MotorNum = M_LDM_STACKER_2;
	mn_StackerPos = M_STACKER_2;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_LDM_STACKER_2);
		CTL_Lib.mn_single_motmove_step[M_LDM_STACKER_2] = 0;
		ReadyPosStep[mn_StackerPos] = 0;
		COMI.mn_home_step[M_LDM_STACKER_2] = 0;
		return;
	}
	Func.ThreadFunctionStepTrace(52, RunS2Step);

	switch(RunS2Step)
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(mn_LeakM_LotEnd[2] == NO && st_sync.n_lotend_module_uldstacker_site == CTL_NO)
			{
				RunS2Step = 10;
			}
		}
		break;

	case 10:
		nRet_2 = FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_chk, IO_OFF);
		if(nRet_2 == IO_ON)
		{
			//010100 0 00 "Stacker2에 트레이가 존재합니다."
			sprintf(mc_jamcode, "010100");
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1050, st_work.mn_run_status, mc_jamcode);			
		}
		else
		{
			RunS2Step = 20;
		}
		break;

	case 20:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_ON)
		{
			//010202 0 01 "Stacker2 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"010202");
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1051, st_work.mn_run_status, mc_jamcode);			
		}
		else
		{
			//if(mn_stacker_move_flag != CTL_YES) break;
			mn_retry2 = 0;
			Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			RunS2Step = 40;
		}
		break;

	case 30:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS2Step = 40;//60;
		}
		break;

	case 40:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			mn_retry2 = 0;
			Set_TrayLatchClinder(CYLINDER_OFF);
			RunS2Step = 60;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				//010204 0 01 "Stacker2 레일푸셔 backward가 동작이 되지 않습니다."
				mn_retry2 = 0;
				sprintf(mc_jamcode, "010204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1052, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 30;
			}
			else
			{
				RunS2Step = 30;
			}
		}
		break;

	case 50:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 60;
		}
		break;

	case 60:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			RunS2Step = 70;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//010208 0 01 "Stocker2 Latch Clamp Tray Off 에러."
				sprintf(mc_jamcode, "010208");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1053, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 50;
			}
			else
			{
				RunS2Step = 50;
			}
		}
		break;

	case 70:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		
		if (nRet_1 == IO_OFF)	// 트레이가 없으면...
		{
			mn_retry2 = 0;
			RunS2Step = 90;
		}
		else
		{
			//010202 0 01 "Stacker2 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"010202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1054, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 80:
		Set_StackerUpDnClinder(mn_StackerPos, CYLINDER_OFF);
		RunS2Step = 90;

	case 90:
		nRet_1 = Get_StackerUpDnClinder(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_stacker2_init_chk = YES;
			mn_stacker_updn_cyliner[mn_StackerPos] = CYLINDER_OFF;
			RunS2Step = 100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//000205 0 00 "Stacker1 레일 Up 센서 에러."
				sprintf(mc_jamcode,"010205");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1055, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 80;
			}
			else
			{
				RunS2Step = 80;
			}
		}
		break;

	case 100:
		if(mn_Moving_stacker == YES)
		{
			st_sync.mn_ld_module_tray_change[0] = CTL_YES;
			RunS2Step = 200;
		}
		break;

	case 200:
		Set_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
		RunS2Step = 300;

	case 300:
		nRet_1 = Get_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_stacker_updn_cyliner[mn_StackerPos] = CYLINDER_ON;//항상 UP
			RunS2Step = 900;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//000206 0 00 "Stacker1 레일 Down 센서 에러."
				sprintf(mc_jamcode,"000206");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1055, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 200;
			}
			else
			{
				RunS2Step = 200;
			}
		}
		break;

	case 900:
		//SD가 감지 않되고 대기위치로 이동 P_STOCKER_UP
		//트레이를 받기 위해 올리는 위치 P_STOCKER_P_LIMIT
		/*nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunS2Step = 1000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunS2Step = 900;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1078, st_work.mn_run_status, COMI.mc_alarmcode);
		}*/
		nRet_1 = OnModuleStackerReadyPos(1,1); //SD 대기
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS2Step = 1000;
		}
		else if(nRet_1 == CTL_FULL)
		{
			//010302 1 01 "Loader Stocker 2 Tray Full 에러."
			sprintf(mc_jamcode, "010302"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1078, st_work.mn_run_status, mc_jamcode);			
		}
		break;

	case 1000:
		if (st_sync.mn_ld_module_tray_change[0] == CTL_YES || 
			(st_sync.mn_ld_module_tray_supply[1] == CTL_FREE && mn_LeakM_LotEnd[1] == YES))
		{
			RunS2Step = 1100;
		}
		else
		{
			if (st_sync.n_lotend_module_ldrbt == CTL_YES)
			{
				RunS2Step = 1100;
			}
		}
		break;

	case 1100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		
		if (nRet_1 == IO_OFF)//IO_ON)//20121113
		{
			RunS2Step = 2000;
		}
		else// if (nRet == IO_ON)
		{
			mn_retry2 = 0;
			Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			RunS2Step = 1200;
		}
		break;

	case 1110:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS2Step = 1200;
		}
		break;

	case 1200:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			RunS2Step = 1300;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//000204 0 00 "Stacker1 레일푸셔 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"000204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1056, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 1110;
			}
			else
			{
				RunS2Step = 1110;
			}
		}
		break;	

	case 1300:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_OFF) == IO_OFF)
		{
			RunS2Step = 2000;
		}
		else
		{
			mn_retry2 = 0;
			nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				RunS2Step = 1400;
			}
		}
		break;

	case 1310:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 1400;
		}
		break;

	case 1400:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			RunS2Step = 2000;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//010201 0 01 "Stocker2 Latch Clamp Tray Off 에러."
				sprintf(mc_jamcode,"010201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1057, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 1310;
			}
			else
			{
				RunS2Step = 1310;
			}
		}
		break;

	case 2000:
		nRet_1 = OnModuleStackerReadyPos(1,1); //SD 대기
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_retry2 = 0;
			Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_ON);
			RunS2Step = 2200;
		}
		else if(nRet_1 == CTL_FULL)
		{
			//010302 1 01 "Loader Stocker 2 Tray Full 에러."
			sprintf(mc_jamcode, "010302"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1058, st_work.mn_run_status, mc_jamcode);			
			RunS2Step = 2000;
		}
		break;

	case 2100:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS2Step = 2200;
		}
		break;

	case 2200:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_ON);
		if (nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 2300;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//010203 0 01 "Stacker2 레일푸셔 forward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"010203");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1059, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 2100;
			}
			else
			{
				RunS2Step = 2100;
			}
		}
		break;

	case 2300:
		if((FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_OFF)) == CTL_ON && 
			(FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_ON) == CTL_OFF) || COMI.mn_simulation_mode == 1) // 20130509
		{
			RunS2Step = 3000;			
		}
		else
		{
			mn_retry2 = 0;
			Set_TrayLatchClinder(CYLINDER_OFF);
			RunS2Step = 2500;
		}
		break;

	case 2400:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 2500;
		}
		break;

	case 2500:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			RunS2Step = 2300;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//010208 0 01 "Stocker2 Latch Clamp Tray Off 에러."
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1060, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 2400;
			}
			else
			{
				RunS2Step = 2400;
			}
		}
		break;

	case 3000:
		st_sync.mn_ld_module_tray_supply[1] = CTL_READY; //준비됨.
		RunS2Step = 3100;
		break;

	case 3100:
		if(st_sync.mn_ld_module_tray_supply[1] == CTL_LOCK)
		{
			RunS2Step = 3200;
		}
		//else if(st_sync.n_lotend_module_ldrbt == CTL_YES)
		else if(st_sync.mn_ld_module_tray_supply[0] == CTL_NOTREADY || (st_work.nMdlInputCount[0][0] == st_work.nMdlPassCount[0][0]) && COMI.mn_simulation_mode == 1) // Tray가 없으면 stacker2 Down // 20130509
		{
			RunS2Step = 10000;
		}
		break;

	case 3200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_ON);

		if (nRet_1 == IO_ON || st_basic.n_mode_device == 2)	// 트레이가 있으면...
		{
			mn_retry2 = 0;
			Set_TrayLatchClinder(CYLINDER_ON);
			RunS2Step = 3500;
		}
		else
		{
			if(st_sync.mn_ld_module_tray_change[1] == NO)
			{
				RunS2Step = 2000;
			}
			else
			{
				//010201 0 01 "Stacker2 레일에 트레이가 존재하지 않습니다."
				sprintf(mc_jamcode, "010201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1061, st_work.mn_run_status, mc_jamcode);
			}
		}
		break;
		
	case 3300:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 3500;
		}
		break;

	case 3500:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_ON);
		if (nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 3600;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				//010207 0 01 "Stocker2 Latch Clamp Tray On 에러."
				sprintf(mc_jamcode, "010207");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1062, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 3300;
			}
			else
			{
				RunS2Step = 3300;
			}
		}
		break;

	case 3600:
		st_sync.mn_ld_module_tray_supply[1] = CTL_FREE; //교체 완료
		RunS2Step = 3700;
		break;

	case 3700:
		if(st_sync.n_module_ldrbt_tray_req[0] == CTL_REQ || st_sync.n_lotend_module_ldrbt == CTL_YES)
		{
			st_sync.n_module_ldrbt_tray_req[0] = CTL_READY;
			st_sync.n_module_ldrbt_tray_req[1] = BIN_LDBUFFERBIN;
			RunS2Step = 4000;
		}
		break;

	case 4000://한장 빼기
		if(st_sync.n_module_ldrbt_tray_req[0] == CTL_REQ || st_sync.n_lotend_module_ldrbt == CTL_YES)
		{
			st_sync.mn_ld_module_tray_supply[1] = CTL_CHANGE;
			RunS2Step = 4100;
		}
		break;

	case 4100:
		if(st_sync.mn_ld_module_tray_supply[1] != CTL_CHANGE) return;

		if (FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_off_chk, IO_OFF) == IO_OFF)	// On 상태.
		{
			nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
			{
				mn_retry2 = 0;
				RunS2Step = 4300;
			}
		}
		else
		{
			nRet_1 = Set_TrayLatchClinder(CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				mn_retry2 = 0;
				RunS2Step = 4200;
			}
		}
		break;

	case 4110:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 4200;
		}
		break;

	case 4200:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_ON);
		if (nRet_1 == CYLINDER_OFF)
		{
			nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
			{
				mn_retry2 = 0;
				RunS2Step = 4300;
			}
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				//010207 0 01 "Stocker2 Latch Clamp Tray On 에러."
				sprintf(mc_jamcode, "010207");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1063, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 4110;
			}
			else
			{
				RunS2Step = 4110;
			}
		}
		break;
		
	case 4210:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS2Step = 4300;
		}
		break;

	case 4300:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			ReadyPosStep[mn_StackerPos] = 0;
			RunS2Step = 4400;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//010204 0 01 "Stacker2 레일푸셔 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode, "010204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1064, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 4210;
			}
			else
			{
				RunS2Step = 4210;
			}
		}
		break;

	case 4400:
		if (FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_fwd_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_bwd_chk, IO_ON) == IO_ON)	// On 상태.
		{
			RunS2Step = 4420;
		}
		else
		{
			mn_retry2 = 0;
			RunS2Step = 4210;
		}
		break;

	case 4410:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 4500;
		}
		break;
		
	case 4420:
		nRet_1 = OnModuleStackerReadyPos(1, 0); //트레이 하나 Up한다
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_retry2 = 0;
			Set_TrayLatchClinder(CYLINDER_OFF);
			RunS2Step = 4500;
		}
		break;

	case 4500:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			RunS2Step = 4600;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//010208 0 01 "Stocker2 Latch Clamp Tray Off 에러."
				sprintf(mc_jamcode, "010208");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1065, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 4410;
			}
			else
			{
				RunS2Step = 4410;
			}
		}
		break;

	case 4600:
		nRet_1 = OnModuleStackerReadyPos(1, 2); //트레이 하나 Dn한다
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_retry2 = 0;
			RunS2Step = 4700;
		}
		//else if (nRet_1 == CTLBD_RET_SKIP)
		//{
		//	RunS2Step = 1000;
		//}
		break;

	case 4700:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_OFF);
		
		if (nRet_1 == IO_OFF || st_basic.n_mode_device == 2)	// 트레이가 없다
		{
			RunS2Step = 5000;
		}
		else
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//000201 0 00 "Stacker1 레일에 트레이가 존재하지 않습니다."
				sprintf(mc_jamcode, "000201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1066, st_work.mn_run_status, mc_jamcode);
			}
		}
		break;

	case 5000:
		st_sync.mn_ld_module_tray_supply[mn_StackerPos] = CTL_FREE;		// 교체 끝났다고 설정.
		//트레이 정보 
		RunS2Step = 1000;
		break;

		
	case 10000:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		
		if (nRet_1 == BD_GOOD)
		{
			mn_retry2 = 0;
			RunS2Step = 10010;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunS2Step = 10000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//010010
			sprintf(mc_jamcode,"010010");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1067, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 10010:
		mn_retry2 = 0;
		FAS_IO.set_out_bit(st_io.o_m_stacker2_up_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_m_stacker2_dn_onoff, IO_ON);
		RunS2Step = 10100;
		break;

	case 10100:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS2Step = 10110;
		}
		break;

	case 10110:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			mn_retry2 = 0;
			RunS2Step = 10200;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//010204 0 01 "Stacker2 레일푸셔 backward가 동작이 되지 않습니다."
				//010206 0 01 "Stacker2 레일 Down 센서 에러."
				sprintf(mc_jamcode, "010204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1068, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 10100;
			}
			else
			{
				RunS2Step = 10100;
			}
		}
		break;

	case 10200:
		nRet_2 = Set_StackerUpDnClinder(mn_StackerPos, CYLINDER_OFF);
		if(nRet_2 == CYLINDER_ON)
		{
			RunS2Step = 10210;
		}
		break;

	case 10210:
		nRet_2 = Get_StackerUpDnClinder(mn_StackerPos, CYLINDER_OFF);
		if (nRet_2 == CYLINDER_OFF)
		{
			RunS2Step = 10300;
		}
		else if (nRet_2 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//010204 0 01 "Stacker2 레일푸셔 backward가 동작이 되지 않습니다."
				//010206 0 01 "Stacker2 레일 Down 센서 에러."
				sprintf(mc_jamcode, "010206");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1069, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 10200;
			}
			else
			{
				RunS2Step = 10200;
			}
		}
		break;		

	case 10300:		
//		nRet_1 = COMI.HomeCheck_Mot(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);		
//		if (nRet_1 == CTLBD_RET_GOOD)
//		{
//			RunS2Step = 10400;
//		}
//		else if (nRet_1 == CTLBD_RET_ERROR)
//		{
//			//010300 0 00 "Stacker2 Motor 홈 체크 에러."
//			sprintf(mc_jamcode,"010300");
//			st_work.mn_run_status = CTL_dWARNING;
//			CTL_Lib.Alarm_Error_Occurrence(1070, st_work.mn_run_status, mc_jamcode);
//		}
		RunS2Step = 10400;
		break;

	case 10400:
		if(st_sync.n_lotend_module_ldrbt != CTL_YES)
		{
			st_sync.n_lotend_module_uldstacker_site = CTL_NO;
			mn_LeakM_LotEnd[2] = YES;
			RunS2Step = 0;
		}
		else
		{
			st_sync.n_lotend_module_uldstacker_site = CTL_YES;	
			RunS2Step = 0;
		}
		break;
	}
}

void CRun_Stacker_Load_Module::Run_Moving_Stacker()
{
	int nRet_1;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_LDM_STACKER_MOVE);
		CTL_Lib.mn_single_motmove_step[M_LDM_STACKER_MOVE] = 0;
		COMI.mn_home_step[M_LDM_STACKER_MOVE] = 0;
		return;
	}

	Func.ThreadFunctionStepTrace(53, RunMStep);

	switch(RunMStep)
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(mn_LeakM_LotEnd[3] == NO && st_sync.n_lotend_module_movingtray_site == CTL_NO)
			{
				mn_ms_retry = 0;
				RunMStep = 100;
			}
		}		
		break;

	case 100:
		RunMStep = 1000;
		break;
		if(st_sync.n_module_guide_tray_stacker[0] == CTL_REQ)
		{
			RunMStep = 110;
		}
		break;

	case 110:
		RunMStep = 300;
		break;
		FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_OFF);
		RunMStep = 200;
		break;

	case 200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker_guide_clamp_onoff, st_io.i_m_stacker_guide_clamp_on_chk, st_io.i_m_stacker_guide_unclamp_off_chk,
			IO_ON, st_wait.n_on_wait_time[9], st_wait.n_limit_wait_time[9]);
		if(nRet_1 == RET_GOOD)
		{
			mn_ms_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_ON);
			RunMStep = 300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//000400 0 00 "Module stacker1 guide clamp가 On이 되지 않습니다."
		 //000401 0 00 "Module Stacker1 guide clamp가 Off가 되지 않습니다."
			mn_ms_retry++;
			if(mn_ms_retry > 3)
			{
				mn_ms_retry = 0;
				sprintf(mc_jamcode, "000400");			
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1076, st_work.mn_run_status, mc_jamcode);
				RunMStep = 110;
			}
			else
			{
				RunMStep = 110;
			}
		}
		break;

	case 210:
		st_sync.n_module_guide_tray_stacker[1] = CTL_LOCK;
		RunMStep = 1000;
		break;
		
		FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_m_stacker_guide_unclamp_onoff, IO_ON);
		RunMStep = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_m_stacker_guide_clamp_onoff, st_io.i_m_stacker_guide_unclamp_off_chk, st_io.i_m_stacker_guide_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[9], st_wait.n_limit_wait_time[9]);
		if(nRet_1 == RET_GOOD)
		{
			st_sync.n_module_guide_tray_stacker[1] = CTL_LOCK;
			RunMStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//000400 0 00 "Module stacker1 guide clamp가 On이 되지 않습니다."
		 //000401 0 00 "Module Stacker1 guide clamp가 Off가 되지 않습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_jamcode, "000401");			
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1077, st_work.mn_run_status, mc_jamcode);
				RunMStep = 210;
			}
			else
			{
				RunMStep = 210;
			}
		}
		break;
		
	case 1000:
		if(mn_stacker1_init_chk == YES && mn_stacker2_init_chk == YES)
		{
			RunMStep = 1100;
		}
		break;

	case 1100:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_Moving_stacker = YES;//Unloader위치로 이동 완료
			RunMStep = 2000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunMStep = 1100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1078, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2000:
		if(st_sync.n_lotend_module_ldrbt == CTL_YES && st_sync.n_lotend_module_uldstacker_site == CTL_YES && 
			st_sync.n_lotend_module_ldstacker_site == CTL_YES)
		{
			RunMStep = 2100;
		}
		else if(mn_LeakM_LotEnd[0] == YES && mn_LeakM_LotEnd[1] == YES && mn_LeakM_LotEnd[2] == YES)
		{
			RunMStep = 2100;
		}
		break;

	case 2100:
		nRet_1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_Moving_stacker = YES;//Unloader위치로 이동 완료
			RunMStep = 3000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunMStep = 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1079, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
		
	case 3000:
		if(FAS_IO.get_in_bit(st_io.i_m_uld_slide_tray_chk, IO_ON) == IO_ON)
		{
			RunMStep = 4000;
		}
		else
		{//000800 0 01 "Module Slide에 트레이가 존재하지 않습니다."
			sprintf(mc_jamcode, _T("000800"));
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1080, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 4000:
// 		if(mn_LeakM_LotEnd[0] == YES && mn_LeakM_LotEnd[1] == YES && mn_LeakM_LotEnd[2] == YES)
// 		{
// 			st_sync.n_lotend_module_movingtray_site = CTL_NO; //전체 다시 시작
// 			 mn_LeakM_LotEnd[3] = YES;
// 			 st_sync.n_lot_reready[0] = CTL_REQ;
// 			 st_handler.n_lotend_ready = 1;
// 			 mn_Moving_stacker = NO;
// 			 RunMStep = 5000;
// 		}
// 		else
// 		{
// 			mn_Moving_stacker = NO;
// 			st_sync.n_lotend_module_movingtray_site = CTL_YES;
// 			RunMStep = 0;
// 		}
		//2017.0731
		if(mn_LeakM_LotEnd[0] == YES && mn_LeakM_LotEnd[1] == YES && mn_LeakM_LotEnd[2] == YES)
		{
			mn_LeakM_LotEnd[3] = YES;
			mn_Moving_stacker = NO;
			RunMStep = 5000;
		}
		else
		{
			mn_Moving_stacker = NO;
			RunMStep = 0;
		}
		st_sync.n_module_guide_tray_stacker[1] = CTL_CLEAR;
		break;

	case 5000:
// 		if(st_sync.n_lot_reready[0] == CTL_READY)
// 		{
// 			if(FAS_IO.get_in_bit(st_io.i_m_uld_slide_tray_chk, IO_OFF) == IO_OFF)
// 			{
// 				InitStep = 0;
// 				st_handler.mn_init_state[INIT_LD_STACKER] = CTL_NO;
// 				RunMStep = 5200;
// 			}
// 			else
// 			{
// 				//994009 0 99 "Module Blank(빈)자재를 제거해 주세요."
// 				sprintf(mc_jamcode, _T("994009")); 
// 		//		st_work.mn_run_status = CTL_dWARNING;
// 				CTL_Lib.Alarm_Error_Occurrence(1081, CTL_dWARNING, mc_jamcode);
// 				RunMStep = 5000;
// 			}
// 		}
// 		/////////////////// 20130611
// 		else if(st_sync.n_lot_reready[0] == CTL_REQ)
// 		{
// 			if(FAS_IO.get_in_bit(st_io.i_m_uld_slide_tray_chk, IO_ON) == IO_ON)
// 			{
// 				st_sync.n_lot_reready[0] = CTL_READY;
// 			}
// 		}
		//2017.0731
		if(FAS_IO.get_in_bit(st_io.i_m_uld_slide_tray_chk, IO_ON) == IO_ON)
		{
			//994009 0 99 "Module Blank(빈)자재를 제거해 주세요."
			sprintf(mc_jamcode, _T("994009")); 
			//		st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1081, CTL_dWARNING, mc_jamcode);
			RunMStep = 5000;
		}
		else //if(FAS_IO.get_in_bit(st_io.i_m_uld_slide_tray_chk, IO_OFF) == IO_OFF)
		{
			InitStep = 0;
			st_handler.mn_init_state[INIT_LD_STACKER] = CTL_NO;
			RunMStep = 5200;
		}
		break;

	case 5100:
		RunMStep = 5000;
		break;

	case 5200:
// 		Run_Init();
// 		if(st_handler.mn_init_state[INIT_LD_STACKER] == CTL_YES)
// 		{
// 			st_sync.n_lot_reready[0] = CTL_LOCK;
// 			RunMStep = 5300;
// 		}
		//2017.0731
		Run_Init();
		if(st_handler.mn_init_state[INIT_LD_STACKER] == CTL_YES)
		{
			RunMStep = 5300;
		}
		break;

	case 5300:
// 		if(st_sync.n_lot_reready[0] == CTL_CLEAR)
// 		{
// 			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_ON);
// 			if(nRet_1 == IO_OFF)
// 			{
// 				//994001 0 99 "자재및 모듈을 투입해 주세요. Lot 수량이 맞지 않습니다."
// 				sprintf(mc_jamcode, "994001"); 
// 				st_work.mn_run_status = CTL_dWARNING;
// 				CTL_Lib.Alarm_Error_Occurrence(1082, st_work.mn_run_status, mc_jamcode);
// 			}
// 			else
// 			{
// 				RunMStep = 5400;
// 			}
// 		}
// 		/////////////////// 20130611
// 		else if(st_sync.n_lot_reready[0] != CTL_CLEAR && st_handler.mn_init_state[INIT_LD_STACKER] == CTL_YES)
// 		{
// 			if(FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_OFF) == IO_OFF)
// 			{
// 				st_sync.n_lot_reready[0] = CTL_CLEAR;
// 			}
// 		}
		//2017.0731
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_ON);
		if(nRet_1 == IO_OFF)
		{
			//994001 0 99 "자재및 모듈을 투입해 주세요. Lot 수량이 맞지 않습니다."
			sprintf(mc_jamcode, "994001"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1082, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			RunMStep = 5400;
		}
		break;

	case 5400:
		st_sync.mn_ld_module_tray_supply[0] = st_sync.mn_ld_module_tray_supply[1] = CTL_CLEAR;
		mn_LeakM_LotEnd[0] = mn_LeakM_LotEnd[1] = mn_LeakM_LotEnd[2] = mn_LeakM_LotEnd[3] = NO;
		RunMStep = 0;
		break;
	}
}

void CRun_Stacker_Load_Module::Robot_BackMovePosValCheck()//20121126
{
	//여기서 이전에 동작했던 위치를 다시 계산해야한다.
		st_work.md_reinstatement_Pos[1][M_LDM_STACKER_1] =  st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1];
		st_work.md_reinstatement_Pos[1][M_LDM_STACKER_2] =  st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2];
		st_work.md_reinstatement_Pos[1][M_LDM_STACKER_MOVE] =  st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE];
}

void CRun_Stacker_Load_Module::Run_Reinstate()//20121126
{
	int Ret1 = 0, Ret2 = 0, i = 0, n_response = 0;
	int FuncRet = RET_PROCEED;
	int nRet1 = 0, nRet2 = 0;
	double dPosition[10];

	if (st_work.mn_LdModuleStacker_Reinstatement_Ok == TRUE) return;//20121126
	
	if (st_work.mn_reinstatement_mode[LDMODULE_SITE] == 0) // 완전 초기화 후만 관리함 
	{	// 현재 위치 백업 
		st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1] =  COMI.md_cmdpos_backup[M_LDM_STACKER_1];
		st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2] =  COMI.md_cmdpos_backup[M_LDM_STACKER_2];
		st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] =  COMI.md_cmdpos_backup[M_LDM_STACKER_MOVE];

		st_work.mn_TrayClampStatusBackup[LDMODULE_SITE][1] = FAS_IO.get_out_bit(st_io.o_m_stacker2_tray_clamp_onoff, IO_OFF);
		st_work.mn_reinstatement_mode[LDMODULE_SITE] = 1; //한번 데이터를 저장한다 
		ReinstateStep = 0;

	}	 

	switch (ReinstateStep)
	{
	case 0:
		ReinstateStep = 1000;
		break;

	case 1000://현재 위치 확인
		dPosition[0] = COMI.Get_MotCurrentPos(M_LDM_STACKER_1);
		dPosition[1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_2);
		dPosition[2] = COMI.Get_MotCurrentPos(M_LDM_STACKER_MOVE);

		for (i = 0; i < 3; i++)
		{
			mn_change[LDMODULE_SITE][i] = 0;
		}
	
		//변경된 위치값 출력
		if (dPosition[0] >  st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1] + COMI.mn_allow_value[M_LDM_STACKER_1]
			|| dPosition[0] < st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1] - COMI.mn_allow_value[M_LDM_STACKER_1])
		{
			mn_change[LDMODULE_SITE][0]++;
			sprintf(st_msg.c_abnormal_msg, "Load Stacker#1 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1], (long)dPosition[0], (long)COMI.mn_allow_value[M_LDM_STACKER_1]);
		}
		if (dPosition[1] > st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2] + COMI.mn_allow_value[M_LDM_STACKER_2]
			|| dPosition[1] < st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2] - COMI.mn_allow_value[M_LDM_STACKER_2])
		{
			mn_change[LDMODULE_SITE][1]++;
			sprintf(st_msg.c_abnormal_msg, "Load Stacker#2 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2], (long)dPosition[1], (long)COMI.mn_allow_value[M_LDM_STACKER_2]);
		}
		if (dPosition[2] > st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] + COMI.mn_allow_value[M_LDM_STACKER_MOVE]
			|| dPosition[2] < st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] - COMI.mn_allow_value[M_LDM_STACKER_MOVE])
		{
			mn_change[LDMODULE_SITE][2]++;
			sprintf(st_msg.c_abnormal_msg, "Load Stacker Move POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE], (long)dPosition[2], (long)COMI.mn_allow_value[M_LDM_STACKER_MOVE]);
		}

		st_msg.mstr_event_msg[0] = "Recovery Mode.......";
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);

		st_work.mn_reinstatement_mode[LDMODULE_SITE] = 3; //초기화 정보를 완료했다, 클리어 

		if(mn_change[LDMODULE_SITE][0] > 0 || mn_change[LDMODULE_SITE][1] > 0) //stacker 1,2
		{
			ReinstateStep = 2000;
		}
		else if(mn_change[LDMODULE_SITE][2] > 0) // tray move
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
		
		COMI.Set_MotPower(M_LDM_STACKER_1, TRUE);
		COMI.Set_MotPower(M_LDM_STACKER_2, TRUE);
		COMI.Set_MotPower(M_LDM_STACKER_MOVE, TRUE);
	
		mn_check[0] = VARIABLE_INIT;
		mn_check[1] = VARIABLE_INIT;

		if(mn_change[LDMODULE_SITE][0] > 0 || mn_change[LDMODULE_SITE][1] > 0) //stacker 1,2
		{
			ReinstateStep = 2010;
		}
		else if(mn_change[LDMODULE_SITE][2] > 0)// tray move
		{
			ReinstateStep = 2100;
		}
		else
		{
			ReinstateStep = 1100;
		}
		break;

	case 2010: //stacker move 축 확인

		if (st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] + COMI.mn_allow_value[M_LDM_STACKER_MOVE] < st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS]
		|| st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] - COMI.mn_allow_value[M_LDM_STACKER_MOVE] < st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS])
		{
			ReinstateStep = 2100;
		}
		else
		{
			ReinstateStep = 2020;
		}
		break;

	case 2020: //stacker move 축 확인

		if (st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] + COMI.mn_allow_value[M_LDM_STACKER_MOVE] >= st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS]
		|| st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] - COMI.mn_allow_value[M_LDM_STACKER_MOVE] >= st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS])
		{
			ReinstateStep = 2100;
		}
		else
		{
			ReinstateStep = 2030;
		}
		break;

	case 2030://stacker 높이 강제고정 1 : 500, 2 : 600
		if (st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1] + COMI.mn_allow_value[M_LDM_STACKER_1] >= 500   
			|| st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1] - COMI.mn_allow_value[M_LDM_STACKER_1] >= 500)
		{
			ReinstateStep = 2050;
			
		}
		else
		{
			ReinstateStep = 2040;
		}
		break;
	case 2040:
		if (st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1] + COMI.mn_allow_value[M_LDM_STACKER_1] <= st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DOWN]   
			|| st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1] - COMI.mn_allow_value[M_LDM_STACKER_1] <= st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DOWN])
		{
			ReinstateStep = 2050;
			
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1086, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 0;
		}
		break;
		
	case 2050://stacker 높이 강제고정 1 : 500, 2 : 600
		if (st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2] + COMI.mn_allow_value[M_LDM_STACKER_2] > 600   
			|| st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2] - COMI.mn_allow_value[M_LDM_STACKER_2] > 600)
		{
			ReinstateStep = 2070;
			
		}
		else
		{
			ReinstateStep = 2060;
		}
		break;
	
	case 2060:
		if (st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2] + COMI.mn_allow_value[M_LDM_STACKER_2] <= st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN]   
			|| st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2] - COMI.mn_allow_value[M_LDM_STACKER_2] <= st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN])
		{
			ReinstateStep = 2070;
			
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1087, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 0;
		}
		break;

	case 2070:
		nRet1 = COMI.HomeCheck_Mot(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet1 == BD_GOOD)
		{
			ReinstateStep = 2100;
		}		
		else if (nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5003, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;
		

	case 2100:
		nRet1 = COMI.HomeCheck_Mot(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 2110;
		}		
		else if (nRet1 != CTLBD_RET_PROCEED)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5001, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2110:
		nRet1 = COMI.HomeCheck_Mot(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet1 == BD_GOOD)
		{
			ReinstateStep = 2120;
		}		
		else if (nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5002, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2120:
		nRet1 = COMI.HomeCheck_Mot(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet1 == BD_GOOD)
		{
			ReinstateStep = 2130;
		}		
		else if (nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5003, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2130:
		if((st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] > (st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS] + st_motor[M_LDM_STACKER_MOVE].mn_allow)) ||
			(st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] < (st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] - st_motor[M_LDM_STACKER_MOVE].mn_allow)))
		{
			ReinstateStep = 2140;
		}
		else
		{
			ReinstateStep = 2150;
		}
		break;

	case 2140:
		nRet1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[st_work.mn_back_site[M_LDM_STACKER_MOVE]], st_basic.nManualSpeed);
		if(nRet1 == BD_GOOD)
		{
			ReinstateStep = 2200;
		}
		else if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			ReinstateStep = 2140;
		}
		break;

	case 2150:
		nRet1 = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE], st_basic.nManualSpeed);
		if(nRet1 == BD_GOOD)
		{
			ReinstateStep = 2200;
		}
		else if(nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			ReinstateStep = 2140;
		}
		break;

	case 2200:
		nRet1 = CTL_Lib.Single_Move(M_LDM_STACKER_1, st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1], MOT_TIMEOUT);		
		if (nRet1 == BD_GOOD)
		{
			ReinstateStep = 2300;
		}		
		else if (nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5002, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2300:
		nRet1 = CTL_Lib.Single_Move(M_LDM_STACKER_2, st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2], MOT_TIMEOUT);		
		if (nRet1 == BD_GOOD)
		{
			ReinstateStep = 50000;
			mn_change[LDMODULE_SITE][0] = 0;
			mn_change[LDMODULE_SITE][1] = 0;
			mn_change[LDMODULE_SITE][2] = 0;
		}		
		else if (nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5002, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

// 	case 3510:
// 		backuptime[0] = GetCurrentTime();
// 		ReinstateStep = 3600;
// 		break;
// 		
// 	case 3600:
// 		backuptime[1] = GetCurrentTime();
// 		backuptime[2] = backuptime[1] - backuptime[0];
// 		if (backuptime[2] >= 500)
// 		{
// 			ReinstateStep = 10000;
// 		}
// 		else if(backuptime[2] < 0)
// 		{
// 			backuptime[0] = GetCurrentTime();
// 		}
// 		break;
// 
// 	case 4000:
// 		//일단 Buffer의 위치를 측정할 필요가 있다.
// 		//움직였기에 이 구간에 들어온다
// 		//if()
// 		FAS_IO.set_out_bit(st_work.mn_ModuleClampStatusBackup[st_work.mn_back_site[M_M_RBT_Y] - Y_LD_WORK_PLACE1], IO_OFF);
// 		backuptime[0] = GetCurrentTime();
// 		ReinstateStep = 4100;
// 		break;
// 
// 	case 4100:
// 		backuptime[1] = GetCurrentTime();
// 		backuptime[2] = backuptime[1] - backuptime[0];
// 		if(backuptime[2] < 0) backuptime[0] = GetCurrentTime();
// 		if(backuptime[2] < 1000) break;
// 		
// 		if(FAS_IO.get_in_bit(st_work.mn_ModuleClampStatusBackup[st_work.mn_back_site[M_M_RBT_Y] - Y_LD_WORK_PLACE1], IO_OFF) == IO_OFF)
// 		{
// 			ReinstateStep = 3100;
// 		}
// 		else
// 		{
// 			if(backuptime[2] > 5000)
// 			{
// 				sprintf(mc_jamcode,"55080%d", st_work.mn_back_site[M_M_RBT_Y] - Y_LD_WORK_PLACE1);
// 				CTL_Lib.Alarm_Error_Occurrence(5026, CTL_dWARNING, mc_jamcode);
// 			}
// 		}
// 		break;
// 
// 	case 10000:
// 		for (i = 0; i < PICKER_NUM; i++)
// 		{
// 			FAS_IO.set_out_bit(st_io.o_ld_module_picker_updn[i], st_work.mn_PickerUpDnStatusBackup[LDMODULE_SITE][i]);
// 		}
// 		backuptime[0] = GetCurrentTime();
// 		ReinstateStep = 10100;
// 		break;
// 
// 	case 10100:
// 		backuptime[1] = GetCurrentTime();
// 		backuptime[2] = backuptime[1] - backuptime[0];
// 		if (backuptime[2] >= 1500)
// 		{
// 			ReinstateStep = 10200;
// 		}
// 		else if(backuptime[2] < 0)
// 		{
// 			backuptime[0] = GetCurrentTime();
// 		}
// 		break;
// 
// 	case 10200:
// 		for (i = 0; i < PICKER_NUM; i++)
// 		{
// 			FAS_IO.set_out_bit(st_io.o_ld_module_glipper_onoff[i], st_work.mn_FingerStatusBackup[LDMODULE_SITE][i]);
// 		}
// 		backuptime[0] = GetCurrentTime();
// 		ReinstateStep = 10300;
// 		break;
// 
// 	case 10300:
// 		backuptime[1] = GetCurrentTime();
// 		backuptime[2] = backuptime[1] - backuptime[0];
// 		if (backuptime[2] >= 1000)
// 		{
// 			ReinstateStep = 50000;
// 		}
// 		else if(backuptime[2] < 0)
// 		{
// 			backuptime[0] = GetCurrentTime();
// 		}
// 		break;

	case 50000:
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);

		ReinstateStep = 0;
// 		st_handler.mn_reinstatement = CTL_NO;			// 복구 동작이 아니다.
 		st_work.mn_reinstatement_mode[LDMODULE_SITE] = 0;	// 초기화 정보를 완료했다, 클리어 
		st_work.mn_LdModuleStacker_Reinstatement_Ok = TRUE;//20121126
		FuncRet = RET_GOOD;
		break;
	}
}

int CRun_Stacker_Load_Module::LD_Module_TrayBack()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet_1;

	Func.ThreadFunctionStepTrace(54, TrayBackStep);
	switch(TrayBackStep)
	{
	case 0:
		TrayBackStep = 1000;
		break;

	case 1000:
		if (FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_up_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_dn_chk, IO_OFF) == IO_OFF)
		{
			TrayBackStep = 1100;
		}
		else
		{
			mn_retry = 0;
			Set_PusherUpDnCylinderOnOff(CYLINDER_OFF);
			TrayBackStep = 1020;
		}
		break;

	case 1010:
		nRet_1 = Set_PusherUpDnCylinderOnOff(CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			TrayBackStep = 1020;
		}
		break;

	case 1020:
		nRet_1 = Get_PusherUpDnCylinderOnOff(CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			TrayBackStep = 1100;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//000108 0 00 "stacker 트레이 pusher clamp 업이 동작을 하지 않습니다."
				sprintf(mc_jamcode,"000108");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1070, st_work.mn_run_status, mc_jamcode);
				TrayBackStep = 1010;			
			}
			else
			{
				TrayBackStep = 1010;
			}
		}
		break;

	case 1100:
		if ((FAS_IO.Get_In_Bit(st_io.i_m_stacker_tray_pusher_fwd_chk) == IO_ON) || 
			(FAS_IO.Get_In_Bit(st_io.i_m_stacker_tray_pusher_bwd_chk) == IO_OFF))
		{
			mn_retry = 0;
			Set_PusherCylinderOnOff(CYLINDER_OFF);
			TrayBackStep = 1110;
		}
		else
		{
			TrayBackStep = 2000;
		}
		break;

	case 1110:
		nRet_1 = Set_PusherCylinderOnOff(CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			TrayBackStep = 1120;
		}
		break;

	case 1120:
		nRet_1 = Get_PusherCylinderOnOff(CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			TrayBackStep = 2000;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//000105 0 00 "stacker 트레이 pusher backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"000105");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1071, st_work.mn_run_status, mc_jamcode);
				TrayBackStep = 1110;
			}
			else
			{
				TrayBackStep = 1110;
			}			
		}
		break;

	case 2000:
		nFuncRet = CTLBD_RET_GOOD;
		TrayBackStep = 0;
		break;
	}
	return nFuncRet;
}

int CRun_Stacker_Load_Module::LD_Module_TrayMove()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet_1;

	Func.ThreadFunctionStepTrace(55, TrayMoveStep);
	switch(TrayMoveStep)
	{
	case 0:
//		if (FAS_IO.get_in_bit(st_io.i_m_stacker1_tray_chk, IO_ON) == IO_ON)
//		{
			TrayMoveStep = 1000;
//		}
//		else
//		{
//			//000101 0 00 "Stacker1에 트레이가 존재하지 않습니다."
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Stacker1에 트레이가 존재하지 않습니다.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
//			alarm.mstr_code = _T("000101");
//			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
//			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
//						//CTL_Lib.Alarm_Error_Occurrence(alarm.mn_count_mode, st_work.mn_run_status, alarm.mstr_code);
//		}
		break;

	case 1000:
		if(FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_up_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_clamp_dn_chk, IO_OFF) == IO_OFF && COMI.mn_simulation_mode != 1) // 20130509
		{
			mn_retry = 0;
			Set_PusherUpDnCylinderOnOff(CYLINDER_ON);
			TrayMoveStep = 1100;
		}
		else
		{
			TrayMoveStep = 2000;
		}
		break;

	case 1100:
		nRet_1 = Set_PusherUpDnCylinderOnOff(CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			TrayMoveStep = 1200;
		}
		break;

	case 1200:
		nRet_1 = Get_PusherUpDnCylinderOnOff(CYLINDER_ON);
		if (nRet_1 == CYLINDER_ON)
		{
			TrayMoveStep = 1000;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				TrayMoveStep = 1100;			
				//000108 0 00 "stacker 트레이 pusher clamp 업이 동작을 하지 않습니다."
				sprintf(mc_jamcode,"000108");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1072, st_work.mn_run_status, mc_jamcode);
			}
			else
			{
				TrayMoveStep = 1100;			
			}
		}
		break;

	case 2000:
		if ((FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_fwd_chk, IO_OFF) == IO_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_m_stacker_tray_pusher_bwd_chk, IO_ON) == IO_ON))
		{
			mn_retry = 0;
			Set_PusherCylinderOnOff(CYLINDER_ON);
			TrayMoveStep = 2200;
		}
		else
		{
			//000107 0 00 "stacker 트레이 pusher backward 상태가 아닙니다."
			sprintf(mc_jamcode,"000107");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1073, st_work.mn_run_status, mc_jamcode);
		}
		TrayMoveStep = 2100;
		break;

	case 2100:
		nRet_1 = Set_PusherCylinderOnOff(CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			TrayMoveStep = 2200;
		}
		break;

	case 2200:
		nRet_1 = Get_PusherCylinderOnOff(CYLINDER_ON);
		if (nRet_1 == CYLINDER_ON)
		{
			TrayMoveStep = 2300;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//000104 0 00 "stacker 트레이 pusher forward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"000104");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1074, st_work.mn_run_status, mc_jamcode);
				TrayMoveStep = 2100;			
			}
			else
			{
				TrayMoveStep = 2100;
			}
		}
		break;

	case 2300:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_rail_tray_chk, IO_ON);
			
		if (st_basic.n_mode_device == 2)		nRet_1 = IO_ON;
		
		if(nRet_1 == IO_ON)
		{
			TrayMoveStep = 3000;
		}
		else
		{
			//010101 0 01 "Stacker2에 트레이가 존재지 않습니다."
			sprintf(mc_jamcode, "010101");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1075, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 3000:
		nFuncRet = CTLBD_RET_GOOD;
		TrayMoveStep = 0;
		break;
	}

	return nFuncRet;
}


int CRun_Stacker_Load_Module::Set_PusherCylinderOnOff(int n_onoff)
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

int CRun_Stacker_Load_Module::Get_PusherCylinderOnOff(int n_onoff)
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

int CRun_Stacker_Load_Module::Set_PusherUpDnCylinderOnOff(int n_onoff)
{
	int nFuncRet = CTL_GOOD;

	FAS_IO.set_out_bit(st_io.o_m_stacker_tray_pusher_clamp_updn, n_onoff);
	mb_pusherupdncylinder_flag = FALSE;
	ml_pusherupdncylinder_Time[0] = GetCurrentTime();

	return nFuncRet;
	
}

int CRun_Stacker_Load_Module::Get_PusherUpDnCylinderOnOff(int n_onoff)
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

int CRun_Stacker_Load_Module::Set_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff == CYLINDER_OFF)
	{
		/*if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk + (nStacker*3), IO_OFF) == IO_ON)
		{
			//000202 0 00 "Stacker1 레일에 트레이가 있습니다."
			//010202 0 01 "Stacker2 레일에 트레이가 있습니다."
			if(nStacker == 0)		sprintf(mc_jamcode, "000202");
			else                    sprintf(mc_jamcode, "010202");
			alarm.mn_count_mode = 0; st_work.mn_run_status = CTL_dWARNING;
			alarm.mn_alarm_assign_section = 1004;
			CTL_Lib.Alarm_Error_Occurrence(1081, st_work.mn_run_status, mc_jamcode);
			nFuncRet = CYLINDER_ERROR;
		}*/
	}

	FAS_IO.set_out_bit(st_io.o_m_stacker1_rail_fwd_onoff + (nStacker*2), n_onoff);
	FAS_IO.set_out_bit(st_io.o_m_stacker1_rail_bwd_onoff + (nStacker*2), !n_onoff);
	mb_pushercylinder_flag[nStacker] = FALSE;
	ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();

	return nFuncRet;
	
}

int CRun_Stacker_Load_Module::Get_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff)
{
	int nFuncRet = -1;

	if(COMI.mn_simulation_mode == 1) // 20130509
	{
		nFuncRet = n_onoff;
	}
	else
	{
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
				if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[5] + (nStacker*2))
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_pushercylinder_flag[nStacker] = FALSE;
				}
			}
			else if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_on_wait_time[5] + (nStacker*2))
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
				if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[5] + (nStacker*2))
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_pushercylinder_flag[nStacker] = FALSE;
				}
			}
			else if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_on_wait_time[5] + (nStacker*2))
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
			
			if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[5] + (nStacker*2))
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker] = FALSE;
			}
		}
	}
	
	return nFuncRet;
}

int CRun_Stacker_Load_Module::Set_StackerUpDnClinder(int nStacker, int n_onoff)
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

int CRun_Stacker_Load_Module::Get_StackerUpDnClinder(int nStacker, int n_onoff)
{
	int nFuncRet = -1;

	if(COMI.mn_simulation_mode == 1) // 20130509
	{
		nFuncRet = n_onoff;
	}
	else
	{
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
	}
	
	return nFuncRet;
}

int CRun_Stacker_Load_Module::Set_TrayLatchClinder(int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	FAS_IO.set_out_bit(st_io.o_m_stacker2_tray_clamp_onoff, n_onoff);
	mb_latchcylinder_flag = FALSE;
	ml_latchcylinder_Time[0] = GetCurrentTime();

	return nFuncRet;
	
}

int CRun_Stacker_Load_Module::Get_TrayLatchClinder(int n_onoff)
{
	int nFuncRet = -1;

	if(COMI.mn_simulation_mode == 1) // 201305099
	{
		nFuncRet = n_onoff;
	}
	else
	{
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
			
			if (ml_latchcylinder_Time[2] <= 0)		
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
			
			if (ml_latchcylinder_Time[2] <= 0)		
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
	}
	
	return nFuncRet;
}

//Loader에만 Stacker가 있고 UNloader에는 Stacker가 없다
//일단 현재 위치에서 Stacker가 UP할 수 있는지 체크하고 Up해서 움직인다.
int CRun_Stacker_Load_Module::OnModuleStackerReadyPos(int n_StackerPos, int nMode)
{
	int nRet_1 = VARIABLE_INIT, nRet_2 = VARIABLE_INIT;
	int nStackerUpDn = 0;
	int nFuncRet = CTLBD_RET_PROCEED;
	int nMotorNum;

	if(n_StackerPos == M_STACKER_1)
	{
		nMotorNum = M_LDM_STACKER_1;
		mn_stacker_updn_cyliner[n_StackerPos] = CYLINDER_ON;
		m_nMotorNum[n_StackerPos] = nMotorNum;
	}
	else
	{
		if(FAS_IO.get_in_bit(st_io.i_m_stacker2_up_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_m_stacker2_dn_chk, IO_OFF) == IO_OFF)
		{
			mn_stacker_updn_cyliner[n_StackerPos] = CYLINDER_ON;
		}
		else
		{
			mn_stacker_updn_cyliner[n_StackerPos] = CYLINDER_OFF;
		}
		nMotorNum = M_LDM_STACKER_2;
		m_nMotorNum[n_StackerPos] = nMotorNum;
	}

	if(st_basic.n_mode_device == 2)
	{
		return CTLBD_RET_GOOD;
	}

	//2016.0526
	if(n_StackerPos == M_STACKER_1)
	{
		Func.ThreadFunctionStepTrace(56, ReadyPosStep[n_StackerPos]);
	}
	else
	{
		Func.ThreadFunctionStepTrace(57, ReadyPosStep[n_StackerPos]);
	}

	//1.일단 현재 위치가 P_STOCKER_P_LIMIT인지 체크 한다
	//2.stacker2는 stacker가 UP인지 체크한다
	//3.SD가 감지인지 아닌지 체크한다.
	//4. SD가 체크이면 P_STOCKER_DOWN까지 내려간다.


	switch(ReadyPosStep[n_StackerPos])
	{
	case 0:		
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
			ReadyPosStep[n_StackerPos] = 1000;
		}
		else
		{
			////////////////////////20121113
			m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);
			if(m_dNowPos[n_StackerPos] > st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT] - st_motor[nMotorNum].md_pos[P_STOCKER_DN_OFFSET])
			{
				COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
				ReadyPosStep[n_StackerPos] = 1000;
			}
			else
			{
				ReadyPosStep[n_StackerPos] = 100;
			}

//			ReadyPosStep[n_StackerPos] = 100;
			////////////////////////
		}
		break;

	case 100:
		COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmTRUE);
		nRet_1 = COMI.Start_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep[n_StackerPos] = 200;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1083, st_work.mn_run_status, COMI.mc_alarmcode);
			ReadyPosStep[n_StackerPos] = 0;
		}
		else if(nRet_1 == CTLBD_RET_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1084, st_work.mn_run_status, COMI.mc_alarmcode);
			ReadyPosStep[n_StackerPos] = 0;
		}
		break;

	case 200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
// 			ml_moveWaitTime[1] = GetCurrentTime();
// 			ml_moveWaitTime[2] = ml_moveWaitTime[1] - ml_moveWaitTime[0];
			COMI.Set_MotStop(0, nMotorNum);

// 			if (ml_moveWaitTime[2] < 1000)
// 			{
// 				break;
// 			}
// 			
// 			dNowPos = COMI.Get_MotCurrentPos(nMotorNum);
// 			
			if(n_StackerPos == M_STACKER_1)
			{
				ReadyPosStep[n_StackerPos] = 300;
			}
			else
			{
				if(mn_stacker_updn_cyliner[n_StackerPos] == CYLINDER_OFF)
				{
					ReadyPosStep[n_StackerPos] = 2000;
				}
				else 
				{
					ReadyPosStep[n_StackerPos] = 300;
				}
			}
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_UP]);
			if(nRet_1 == CTLBD_RET_GOOD)// 정상 동작을 했다면...
			{
				if (nMode == 0)
				{
					if(n_StackerPos == M_STACKER_2)
					{
						if(mn_stacker_updn_cyliner[n_StackerPos] == CYLINDER_ON) ReadyPosStep[n_StackerPos] = 5000;
						else
						{
							COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
							ReadyPosStep[n_StackerPos] = 2000;
						}
					}
					else
					{
						nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
						nRet_2 = FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk + (n_StackerPos * 3), IO_OFF);
// 						if(nRet_1 == IO_ON || nRet_2 == IO_ON)//20121113
// 						{
// 							if(nRet_1 == IO_ON)
// 							{
// 								COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
// 								ReadyPosStep[n_StackerPos] = 1000;
// 							}
// 							else
// 							{
// 								ReadyPosStep[n_StackerPos] = 5000;
// 							}					
// 						}
						if(nRet_1 != CTLBD_RET_GOOD || nRet_2 == IO_ON)//20121113
						{
							if(nRet_1 == CTLBD_RET_GOOD)
							{
								COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
								ReadyPosStep[n_StackerPos] = 1000;
							}
							else
							{
								ReadyPosStep[n_StackerPos] = 5000;
							}					
						}
						else
						{
							COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
							ReadyPosStep[n_StackerPos] = 2000;
						}
					}
				}
				else if (nMode == 2)// SD 감지후에 DOWN하는 모드
				{
					ReadyPosStep[n_StackerPos] = 300;
				}
				else//SD 감지거나, UP_LIMIT까지만 동작하는 모드이다.
				{
					ReadyPosStep[n_StackerPos] = 5000;
				}
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1085, st_work.mn_run_status, COMI.mc_alarmcode);
				ReadyPosStep[n_StackerPos] = 100;
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1086, st_work.mn_run_status, COMI.mc_alarmcode);
				ReadyPosStep[n_StackerPos] = 100;
			}
//			else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
//			{
//				ReadyPosStep[n_StackerPos] = 100;
//			}
		}
		break;

	case 300:
		if (nMode == 0)//한장 올린다
		{//SD가 감지된 상태에서 P_STOCKER_UP_OFFSET값만큼만 올린다.
			m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);
			m_dTargetPos[n_StackerPos] = m_dNowPos[n_StackerPos] + st_motor[nMotorNum].md_pos[P_STOCKER_UP_OFFSET];
			if(m_dTargetPos[n_StackerPos] > st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT])
			{
				m_dTargetPos[n_StackerPos] = st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT];
			}
			ReadyPosStep[n_StackerPos] = 400;
		}
		else if (nMode == 2)//한장만큼 내린다.
		{//SD가 감지된 상태에서 P_STOCKER_DN_OFFSET값만큼 내리거나  만 올린다.
			m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);			
			m_dTargetPos[n_StackerPos] = m_dNowPos[n_StackerPos] - st_motor[nMotorNum].md_pos[P_STOCKER_DN_OFFSET*3];
			
			if (m_dTargetPos[n_StackerPos] < st_motor[nMotorNum].md_pos[P_STOCKER_DOWN])
			{
				m_dTargetPos[n_StackerPos] = st_motor[nMotorNum].md_pos[P_STOCKER_DOWN];
			}
			
			ReadyPosStep[n_StackerPos] = 400;
		}
		else//SD감지 대기한다
		{
			COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
			ReadyPosStep[n_StackerPos] = 5000;
			ReadyPosStep[n_StackerPos] = 1000;
		}
		break;

	case 400:
		COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum, m_dTargetPos[n_StackerPos], st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			if(nMode == 0)//UP
			{
				if(FAS_IO.get_in_bit(st_io.i_m_stacker1_rail_tray_chk + (n_StackerPos * 3), IO_ON) == IO_ON)
				{
					ReadyPosStep[n_StackerPos] = 5000;
				}
				else
				{
					ReadyPosStep[n_StackerPos] = 2000;
				}
			}
			else
			{
				ReadyPosStep[n_StackerPos] = 5000;
			}
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep[n_StackerPos] = 400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1087, st_work.mn_run_status, COMI.mc_alarmcode);
			ReadyPosStep[n_StackerPos] = 400;
		}
		break;

	case 1000:
		COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
		nRet_1 = COMI.Start_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep[n_StackerPos] = 1100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ReadyPosStep[n_StackerPos] = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1088, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1100:
		m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);
		if(m_dNowPos[n_StackerPos] > st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT] - st_motor[nMotorNum].md_pos[P_STOCKER_UP_OFFSET]  + st_motor[nMotorNum].mn_allow) break;
	
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD); //2012,0923 FULL SENSOR

		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			nRet_1 = CTLBD_RET_PROCEED;
		}

		if(nRet_1 != CTLBD_RET_GOOD)
		{
// 			ml_moveWaitTime[1] = GetCurrentTime();
// 			ml_moveWaitTime[2] = ml_moveWaitTime[1] - ml_moveWaitTime[0];
			COMI.Set_MotStop(0, nMotorNum);
			
// 			if (ml_moveWaitTime[2] < 1000)
// 			{
// 				break;
// 			}
// 			
// 			m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);
// 			
// 			cmmStGetPosition(nMotorNum, cmCNT_FEED, &dNowPos);
// 			cmmStSetPosition(nMotorNum, cmCNT_COMM, dNowPos);
			if (nMode == 1)//대기
			{
				ReadyPosStep[n_StackerPos] = 5000;
			}
			else if(nMode == 0)
			{
				if(mn_stacker_updn_cyliner[n_StackerPos] == CYLINDER_ON) // 20130509
				{
					ReadyPosStep[n_StackerPos] = 100;
				}
				else
				{
					ReadyPosStep[n_StackerPos] = 2000;
				}
			}
			else
			{
				ReadyPosStep[n_StackerPos] = 100;											// SD센서에 감지가 되지 않으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
			}
			
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_DOWN]);
			if (nRet_1 == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
			{
				ReadyPosStep[n_StackerPos] = 1200;
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1089, st_work.mn_run_status, COMI.mc_alarmcode);
				ReadyPosStep[n_StackerPos] = 1000;
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1090, st_work.mn_run_status, COMI.mc_alarmcode);
				ReadyPosStep[n_StackerPos] = 1000;
			}
			//else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
			//{
			//	ReadyPosStep[n_StackerPos] = 5000;
			//}
		}
		break;

	case 1200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD); 
		if(nRet_1 == CTLBD_RET_GOOD) //이미 감지하고 있으면 트레이가 FULL까지 있다는 이야기임 
		{
			nFuncRet = CTL_FULL;
			ReadyPosStep[n_StackerPos] = 0;
		}
		else
		{//Full sensor는 감지되지 않았으니 다시확인한다 
			ReadyPosStep[n_StackerPos] = 2000;		
		}
		break;

	case 2000:
		/*if(mn_stacker_updn_cyliner[n_StackerPos] == CYLINDER_OFF)
		{
			ReadyPosStep[n_StackerPos] = 2010;
		}
		else
		{
			if(nMode == 0)
			{
				ReadyPosStep[n_StackerPos] = 2010;
			}
			else
			{
				ReadyPosStep[n_StackerPos] = 2010;
			}
		}*/
		ReadyPosStep[n_StackerPos] = 2010;
		break;

	case 2010:
		COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep[n_StackerPos] = 2020;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep[n_StackerPos] = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1092, st_work.mn_run_status, COMI.mc_alarmcode);
			ReadyPosStep[n_StackerPos] = 2000;
		}
		break;

	case 2020:
		if(n_StackerPos == M_STACKER_1)
		{
			ReadyPosStep[n_StackerPos] = 2300;
		}
		else
		{
			if (mn_stacker_updn_cyliner[n_StackerPos] == CYLINDER_ON)
			{
				ReadyPosStep[n_StackerPos] = 2300;
			}
			else
			{
				mn_retry = 0;
				ReadyPosStep[n_StackerPos] = 2100;
			}
		}
		break;

	case 2100:
		nRet_1 = Set_StackerUpDnClinder(n_StackerPos, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep[n_StackerPos] = 2200;
		}
		break;

	case 2200:
		nRet_1 = Get_StackerUpDnClinder(n_StackerPos, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_stacker_updn_cyliner[n_StackerPos] = CYLINDER_ON;
			ReadyPosStep[n_StackerPos] = 100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if (mn_retry > 3)
			{
				mn_retry = 0;
				//000205 0 00 "Stacker1 레일 Up 센서 에러."
				if(n_StackerPos == 0)			sprintf(mc_jamcode,"000205");
				else                            sprintf(mc_jamcode,"010205");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1093, st_work.mn_run_status, mc_jamcode);
				ReadyPosStep[n_StackerPos] = 2100;
			}
			else
			{
				ReadyPosStep[n_StackerPos] = 2100;
			}
		}
		break;

	case 2300:
		if(n_StackerPos ==0)
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk, IO_OFF);
		}
		else
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_chk, IO_OFF);
		}
		//nRet_1 = FAS_IO.get_in_bit(st_io.i_m_stacker1_tray_chk + (n_StackerPos * 4), IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			ReadyPosStep[n_StackerPos] = 2400;
		}
		else// if(nRet_1 == IO_OFF)
		{
			ReadyPosStep[n_StackerPos] = 100;
		}
		break;

	case 2400:		
		st_sync.mn_ld_module_tray_supply[n_StackerPos] = CTL_NOTREADY;		// 트레이가 없음.
		nFuncRet = CTLBD_RET_SKIP;
		ReadyPosStep[n_StackerPos] = 0;
		break;

	case 5000:
		nFuncRet = CTLBD_RET_GOOD;
		ReadyPosStep[n_StackerPos] = 0;
		break;	
	}

	return nFuncRet;

}

int CRun_Stacker_Load_Module::MotorMove(int n_StackerPos, int nMotor, double dPos, int nSpeed)
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet;
	
	switch (MotorMoveStep[n_StackerPos])
	{
	case 0:
		nRet = COMI.Start_SingleMove(nMotor, dPos, nSpeed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			MotorMoveStep[n_StackerPos] = 100;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			nFuncRet = CTLBD_RET_ERROR;
			MotorMoveStep[n_StackerPos] = 0;
		}
		else if (nRet == CTLBD_RET_SAFETY)
		{
			nFuncRet = CTLBD_RET_SAFETY;
			MotorMoveStep[n_StackerPos] = 0;
		}
		break;
		
	case 100:
		nRet = COMI.Check_SingleMove(nMotor, dPos);
		
		if (nRet == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
		{
			MotorMoveStep[n_StackerPos] = 0;
			nFuncRet = CTLBD_RET_GOOD;
		}
		else if (nRet == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
		{
			nFuncRet = CTLBD_RET_ERROR;
			MotorMoveStep[n_StackerPos] = 0;
		}
		else if (nRet == CTLBD_RET_SAFETY)
		{
			nFuncRet = CTLBD_RET_SAFETY;
			MotorMoveStep[n_StackerPos] = 0;
		}
		else if (nRet == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
		{
			MotorMoveStep[n_StackerPos] = 100;
		}
		break;
	}
	
	return nFuncRet;
}

int CRun_Stacker_Load_Module::Get_StackerTrayUseCheck(int nTime)
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nIoRet = FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_chk, IO_OFF);	// 엘레베이터 바닥 체크.

	if (nTime == 0)														// 즉시 확인이면...
	{
		if (nIoRet == IO_ON)
		{
			nFuncRet = IO_ON;	
			mb_StackerTrayCheck_Flag = FALSE;
		}
		else //if (nIoRet == IO_OFF)
		{
			nFuncRet = IO_OFF;	
			mb_StackerTrayCheck_Flag = FALSE;
		}
	}
	else
	{
		if (mb_StackerTrayCheck_Flag != TRUE)
		{
			ml_StackerTrayCheck_Wait[0][0] = GetCurrentTime();
			ml_StackerTrayCheck_Wait[1][0] = GetCurrentTime();
			ml_StackerTrayCheck_Wait[2][0] = GetCurrentTime();
			mb_StackerTrayCheck_Flag = TRUE;
		}
		else if (nIoRet == IO_ON)
		{
			ml_StackerTrayCheck_Wait[0][0] = GetCurrentTime();	// 다른쪽 보는 시간은 Reset
			ml_StackerTrayCheck_Wait[1][1] = GetCurrentTime();
			ml_StackerTrayCheck_Wait[1][2] = ml_StackerTrayCheck_Wait[1][1] - ml_StackerTrayCheck_Wait[1][0];
			
			if (ml_StackerTrayCheck_Wait[1][2] > 10)
			{
				nFuncRet = IO_ON;	
				mb_StackerTrayCheck_Flag = FALSE;
			}
		}
		else if (nIoRet == IO_OFF)
		{
			ml_StackerTrayCheck_Wait[1][0] = GetCurrentTime();	// 다른쪽 보는 시간은 Reset
			ml_StackerTrayCheck_Wait[0][1] = GetCurrentTime();
			ml_StackerTrayCheck_Wait[0][2] = ml_StackerTrayCheck_Wait[0][1] - ml_StackerTrayCheck_Wait[0][0];
			
			if (ml_StackerTrayCheck_Wait[0][2] > 10)
			{
				nFuncRet = IO_OFF;	
				mb_StackerTrayCheck_Flag = FALSE;
			}
		}
		
		if (mb_StackerTrayCheck_Flag == TRUE)
		{
			ml_StackerTrayCheck_Wait[2][1] = GetCurrentTime();
			ml_StackerTrayCheck_Wait[2][2] = ml_StackerTrayCheck_Wait[2][1] - ml_StackerTrayCheck_Wait[2][0];
			
			if (ml_StackerTrayCheck_Wait[2][2] > 1000)
			{
				nFuncRet = CTLBD_RET_ERROR;	
				mb_StackerTrayCheck_Flag = FALSE;
			}
		}
	}
	
	return nFuncRet;
}
