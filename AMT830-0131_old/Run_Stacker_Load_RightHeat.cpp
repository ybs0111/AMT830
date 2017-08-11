st_sync.n_ld_righths_tray_supply[1]// Run_Stacker_Load_RightHeat.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Stacker_Load_RightHeat.h"
#include "Cmmsdk.h"
#include "ComizoaPublic.h"
#include "Public_Function.h"
#include "CtlBd_Function.h"
#include "Variable.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Library.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int M_STACKER_1 = 0;
const int M_STACKER_2 = 1;

/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Load_RightHeat
CRun_Stacker_Load_RightHeat Run_Stacker_Load_RightHeat;
CRun_Stacker_Load_RightHeat::CRun_Stacker_Load_RightHeat()
{
	Thread_Variable_Initial();
}

CRun_Stacker_Load_RightHeat::~CRun_Stacker_Load_RightHeat()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Load_RightHeat message handlers
void CRun_Stacker_Load_RightHeat::Thread_Run()
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

		COMI.Set_MotStop(0, M_HS_B_STACKER_1);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_1] = 0;
		COMI.Set_MotStop(0, M_HS_B_STACKER_2);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_2] = 0;
		ReadyPosStep[0] = ReadyPosStep[1]  = 0;
		InitStep = 0;
		ReinstateStep = 0;
		break;
	}
}

void CRun_Stacker_Load_RightHeat::OnHSRightStacker_FinalPos()//20121126
{
	if(st_work.mn_reinstatement_mode[M_HS_B_STACKER_1] == 0) //stop시 최초 한번만 기록하기 위함
	{
		st_work.d_reinstate_pos[0][M_HS_B_STACKER_1] = COMI.md_cmdpos_backup[M_HS_B_STACKER_1];
		st_work.d_reinstate_pos[0][M_HS_B_STACKER_2] = COMI.md_cmdpos_backup[M_HS_B_STACKER_2];
		st_work.d_reinstate_pos[0][M_HS_B_STACKER_MOVE] = COMI.md_cmdpos_backup[M_HS_B_STACKER_MOVE];

		st_work.mn_TrayClampStatusBackup[RIGHTSINK_SITE][1] = FAS_IO.get_out_bit(st_io.o_hs_bwd_stacker2_tray_clamp_onoff, IO_OFF);
		
		st_work.mn_reinstatement_mode[M_HS_B_STACKER_1] = 1;	//최종 위치를 기록 했다
	}
}

void CRun_Stacker_Load_RightHeat::Run_Init()
{
	int i, nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	double dCurrentPos[3] = {0,};
	int ratio = 50;

	if(st_handler.mn_init_state[INIT_RIGHTHS_STACKER] != CTL_NO) return;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_HS_B_STACKER_1);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_1] = 0;
		COMI.Set_MotStop(0, M_HS_B_STACKER_2);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_2] = 0;
		COMI.Set_MotStop(0, M_HS_B_STACKER_MOVE);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_MOVE] = 0;
		ReadyPosStep[0] = ReadyPosStep[1] = 0;
		COMI.mn_home_step[M_HS_B_STACKER_1] = 0;
		COMI.mn_home_step[M_HS_B_STACKER_2] = 0;
		COMI.mn_home_step[M_HS_B_STACKER_MOVE] = 0;
		return;
	}
	dCurrentPos[0] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_1);
	dCurrentPos[1] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_2);
	dCurrentPos[2] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_MOVE);

	switch(InitStep)
	{
	case 0:
		initial_smove = NO;
		if(st_handler.n_Init_Stacker == CTL_YES)
		{
			initial_smove = YES;
		}
		else if(st_handler.n_Init_Stacker == CTL_NO)
		{
			initial_smove = NO;
		}
		mn_retry = 0;
		InitStep = 5;
		
		break;

	case 5:
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		{//060202 0 06 "Right Heat Sink#l Stacker 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"060202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1400, st_work.mn_run_status, mc_jamcode);
		}
		else if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF) == IO_ON)
		{//070202 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"070202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1401, st_work.mn_run_status, mc_jamcode);
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
				//070204 0 07 "Right Heat Sink#2 Stacker 레일 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode, "070204");
				alarm.mn_alarm_assign_section = 20; alarm.mn_count_mode = 0;
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1402, st_work.mn_run_status, mc_jamcode);
				InitStep = 10;
			}
			else
			{
				InitStep = 10;
			}
		}
		break;

	case 30:
		nRet_1 = Set_RailClampOnOff(CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			InitStep = 35;
		}
		break;

	case 35:
		nRet_1 = Get_RailClampOnOff(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			InitStep = 40;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			Set_RailClampOnOff(CYLINDER_OFF);
//			060500 0 03 "Right Heat Sink#l Stacker 레일 Clamp Fwd가 동작이 되지 않습니다."
//			060501 0 03 "Right Heat Sink#l Stacker 레일 Clamp Bwd가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"060500");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1491, st_work.mn_run_status, mc_jamcode);
			InitStep = 30;
		}
		break;	

	case 40:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			InitStep = 45;
		}
		break;

	case 45:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(0, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			InitStep = 50;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{	
			//060203 0 06 "Right Heat Sink#l Stacker 레일 forward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"070204");
			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			alarm.mn_alarm_assign_section = 21;
			CTL_Lib.Alarm_Error_Occurrence(1403, st_work.mn_run_status, mc_jamcode);
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
			//070204 0 07 "Right Heat Sink#2 Stacker 레일 backward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"070204");
			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			alarm.mn_alarm_assign_section = 22;
			CTL_Lib.Alarm_Error_Occurrence(1404, st_work.mn_run_status, mc_jamcode);
		}
		break;
		
	case 1000:
		InitStep = 1100;
		break;
		
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		{//040100 0 04 "Heat Sink#2 stacker에 트레이가 존재합니다."
			sprintf(mc_jamcode,"040100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1405, st_work.mn_run_status, mc_jamcode);
		}
		else if(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_rail_tray_chk, IO_OFF) == IO_ON)
		{//040101 0 04 "Heat Sink#2 stacker에 트레이가 존재지 않습니다."
			sprintf(mc_jamcode,"040110");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1406, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			InitStep = 1100;
		}
		break;

	case 1100:
		InitStep = 1300;
		break;
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_ON);
		InitStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_guide_clamp_onoff, st_io.i_hs_bwd_stacker_guide_unclamp_off_chk, st_io.i_hs_bwd_stacker_guide_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[25], st_wait.n_limit_wait_time[25]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//060102 0 06 "Right Heat Sink#l stacker이 guide clamp가 On이 되지 않습니다."
			sprintf(mc_jamcode,"060102");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1407, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 1300:
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_tray_pusher_fwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_tray_pusher_bwd_onoff, IO_OFF);
		InitStep = 1400;
		break;

	case 1400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_tray_pusher_bwd_onoff, st_io.i_hs_bwd_stacker_tray_pusher_fwd_chk, st_io.i_hs_bwd_stacker_tray_pusher_bwd_chk,
			IO_ON, st_wait.n_on_wait_time[19], st_wait.n_limit_wait_time[19]);
		nRet_1 = RET_GOOD;
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//060105 0 06 "Right Heat Sink#l stacker 트레이 pusher backward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"060105");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1408, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 1500:
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker1_rail_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker1_rail_bwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker2_rail_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker2_rail_bwd_onoff, IO_ON);
		InitStep = 1600;
		break;

	case 1600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker1_rail_bwd_onoff, st_io.i_hs_bwd_stacker1_rail_bwd_chk, st_io.i_hs_bwd_stacker1_rail_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[22], st_wait.n_limit_wait_time[22]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_fwd_stacker2_rail_bwd_onoff, st_io.i_hs_bwd_stacker2_rail_bwd_chk, st_io.i_hs_bwd_stacker2_rail_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[24], st_wait.n_limit_wait_time[24]);
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			InitStep = 1700;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//060106 0 06 "Right Heat Sink#l stacker 레일 forward가 동작이 되지 않습니다."
         //060107 0 06 "Right Heat Sink#l stacker 레일 backward가 동작이 되지 않습니다."
			if(nRet_1 != RET_PROCEED) sprintf(mc_jamcode,"060107");
			else                      sprintf(mc_jamcode,"070107");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1409, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 1700:
		//FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker1_up_onoff, IO_OFF);
		//FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker1_dn_onoff, IO_ON);
		//FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker2_up_onoff, IO_OFF);
		//FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker2_dn_onoff, IO_ON);
		InitStep = 1900;
		break;

	case 1900:
		if(initial_smove == YES)
		{
			if(COMI.Get_MotIOSensor(M_HS_B_STACKER_MOVE, MOT_SENS_ELP) == CTLBD_RET_GOOD)
			{
				InitStep = 2300;
			}
			else if(COMI.Get_MotIOSensor(M_HS_B_STACKER_MOVE, MOT_SENS_ELM) == CTLBD_RET_GOOD)
			{
				InitStep = 2100;
			}
		}
		else
		{
			InitStep = 2000;
		}
		break;

	case 2000:
		if(dCurrentPos[2] < st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS])//stacker move
		{
			InitStep = 2010;
		}
		else if(dCurrentPos[2] > st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS] &&
			dCurrentPos[2] < st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS])
		{
			InitStep = 2010;
		}
		else //st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS];
		{
			InitStep = 2300;
		}
		break;

	case 2010:
		if(dCurrentPos[1] <= st_motor[M_HS_B_STACKER_2].md_pos[P_STOCKER_DOWN] - (st_motor[M_HS_B_STACKER_2].mn_allow * 5))
		{
			InitStep = 2100;
		}
		else if(dCurrentPos[1] > st_motor[M_HS_B_STACKER_2].md_pos[P_STOCKER_DOWN] - (st_motor[M_HS_B_STACKER_2].mn_allow * 10) && 
			dCurrentPos[1] < st_motor[M_HS_B_STACKER_2].md_pos[P_STOCKER_UP] - st_motor[M_HS_B_STACKER_2].mn_allow)
		{
			InitStep = 2300;
		}
		else //dCurrentPos[1] > st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT]
		{
			InitStep = 2100;
		}
		break;


	case 2100:
		nRet_1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_MOVE, st_motor[M_HS_B_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);

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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1411, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2200:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_STACKER_MOVE, st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS], st_basic.nManualSpeed);
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
			CTL_Lib.Alarm_Error_Occurrence(1413, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2300:
		if(st_work.mn_run_status == dLOTREADY)//2014.0415
		{
			nRet_1 = COMI.Start_SingleMove(M_HS_B_STACKER_1, st_motor[M_HS_B_STACKER_1].md_pos[P_STOCKER_SAFETY], st_basic.nRunSpeed);

			if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
			{
				init_stacker_1 = YES;
				InitStep = 2350;
			}
			else if (nRet_1 == CTLBD_RET_RETRY)
			{
				InitStep = 2200;
			}
			else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
			{
				mn_retry++;
				if(mn_retry > 5)
				{
					mn_retry = 0;
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1414, st_work.mn_run_status, COMI.mc_alarmcode);
				}
			}
		}
		else
		{
			nRet_1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_1, st_motor[M_HS_B_STACKER_1].mn_homecheck_method, MOT_TIMEOUT);

			if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
			{
				init_stacker_1 = YES;
				InitStep = 2400;
			}
			else if (nRet_1 == CTLBD_RET_RETRY)
			{
				InitStep = 2200;
			}
			else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
			{
				mn_retry++;
				if(mn_retry > 5)
				{
					mn_retry = 0;
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1414, st_work.mn_run_status, COMI.mc_alarmcode);
					COMI.mn_home_step[M_HS_B_STACKER_1] = 0;
				}
				else if(mn_retry == 3)
				{
					COMI.mn_home_step[M_HS_B_STACKER_1] = 0;
				}
			}
		}
		break;
	////2014.0415
	case 2350:
			nRet_1 = COMI.Check_SingleMove(M_HS_B_STACKER_1, st_motor[M_HS_B_STACKER_1].md_pos[P_STOCKER_SAFETY]);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				InitStep = 2400;
			}
			else if(nRet_1 == CTLBD_RET_ERROR)
			{
				InitStep = 2300;
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1414, st_work.mn_run_status, COMI.mc_alarmcode);
			}
		break;
	////
	case 2400:
		nRet_1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_2, st_motor[M_HS_B_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			init_stacker_2 = YES;
			InitStep = 2500;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2300;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1415, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

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

	case 2600:
		for (i = 0; i < st_traybuffer[RIGHTSINK_SITE].i_loader_row; i++)
		{
			st_modulemap.RighthsTray[0][i] = DVC_NO;
			st_modulemap.RighthsTray[1][i] = DVC_NO;
		}	
		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_HS_RIGHT_PICKER);			// 피커 정보 
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_HS_RIGHT_PICKER);			// 피커 정보 
		}
		InitStep = 3000;
		break;

	case 3000:
		st_handler.mn_init_state[INIT_RIGHTHS_STACKER] = CTL_YES;
		InitStep = 0;
		break;		
		
	}
}

void CRun_Stacker_Load_RightHeat::Thread_Variable_Initial()
{
	int i;

	mb_sdstop = FALSE;
	InitStep = 0;
	RunS1Step = 0;
	RunS2Step = 0;
	RunMStep = 0;
	TrayBackStep = 0;
	TrayMoveStep = 0;
	RunTransStep = 0;
	TraySeperateStep = 0;
	MotorMoveStep[0] = MotorMoveStep[1] = 0;
	mn_stacker_move_flag = CTL_NO;
	mn_Moving_stacker = NO;
	mn_stacker1_init_chk = NO;
	mn_stacker2_init_chk = NO;
	init_stacker_move = NO;
	init_stacker_1 = NO;
	init_stacker_2 = NO;

	for (i = 0; i < st_traybuffer[RIGHTSINK_SITE].i_loader_row; i++)
	{
		st_modulemap.RighthsTray[0][i] = DVC_NO;
		st_modulemap.RighthsTray[1][i] = DVC_NO;
	}	

	st_sync.n_ld_righths_tray_supply[0] = st_sync.n_ld_righths_tray_supply[1] = CTL_CLEAR;
	ReadyPosStep[0] = ReadyPosStep[1] = 0;
	mn_stacker_updn_cyliner[M_STACKER_1] = mn_stacker_updn_cyliner[M_STACKER_2] = CYLINDER_OFF;
	
	st_sync.n_lotend_righths_ldstacker_site = CTL_NO;
	st_sync.n_lotend_righths_uldstacker_site = CTL_NO;
	st_sync.n_lotend_righths_movingtray_site = CTL_NO;
	st_sync.mn_ld_righths_tray_change[0] = st_sync.mn_ld_righths_tray_change[1] = CTL_NO;

	mn_LeakM_LotEnd[0] = mn_LeakM_LotEnd[1] = mn_LeakM_LotEnd[2] = mn_LeakM_LotEnd[3] = NO;
}

void CRun_Stacker_Load_RightHeat::Run_Transfer()
{
	int nRet = 0, nRet_1 = VARIABLE_INIT;
	int i=0,j=0;

	Func.ThreadFunctionStepTrace(60, RunTransStep);
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
			if(st_sync.n_lotend_righths_ldrbt == CTL_YES)
			{
				st_sync.n_lotend_righths_ldtray_site = CTL_YES;
				mn_LeakM_LotEnd[1] = NO;
			}
			if(mn_LeakM_LotEnd[1] == NO && st_sync.n_lotend_righths_ldtray_site == CTL_NO)
			{
				RunTransStep = 100;
			}
		}
		break;

	case 100:
		if(st_sync.n_lotend_righths_ldtray_site == CTL_YES) return;
		//Stacker#1로더 트레이가 준비됐다
		//먼저 Stacker Move가 안으로 들어가 대기한다.
		if(st_sync.n_ld_righths_tray_supply[0] == CTL_READY)
		{
			RunTransStep = 900;
			RunTransStep = 1000;
		}
		else if (st_sync.n_ld_righths_tray_supply[0] == CTL_NOTREADY)		// 텅 비었을 경우에...
		{
			RunTransStep = 10000;
		}
		break;
		
	case 900:
		if(mn_stacker_move_flag == CTL_NO)
		{
			nRet_1 = CTLBD_RET_GOOD;
			//nRet_1 = CTL_Lib.Single_Move(M_HS_B_STACKER_MOVE, st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS], st_basic.nRunSpeed);
			if(nRet_1 == BD_GOOD)
			{
				mn_stacker_move_flag = CTL_YES;
				RunTransStep = 1000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				RunTransStep = 900;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1420, st_work.mn_run_status, COMI.mc_alarmcode);			
			}
		}
		else
		{
			RunTransStep = 1000;
		}
		break;
		
	case 1000:
		nRet_1 = LD_Righths_TrayBack();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 1100;
		}
		break;

	case 1100:
		if(st_sync.n_ld_righths_tray_supply[0] == CTL_READY)
		{
			st_sync.n_ld_righths_tray_supply[0] = CTL_CHANGE;
			RunTransStep = 1200;
		}
		else if (st_sync.n_ld_righths_tray_supply[0] == CTL_NOTREADY)		// 텅 비었을 경우에...
		{			
			RunTransStep = 10000;
		}
		break;

	case 1200:		
		if(st_sync.n_ld_righths_tray_supply[0] == CTL_LOCK)// 교체 완료
		{
			RunTransStep = 1300;
		}
		else if (st_sync.n_ld_righths_tray_supply[0] == CTL_NOTREADY)		// 텅 비었을 경우에...
		{			
			RunTransStep = 10000;
		}
		break;

	case 1300:
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_ON) == IO_ON ||
			mn_stacker_updn_cyliner[M_STACKER_1] == CYLINDER_ON)
		{
			//for (i = 0; i < st_traybuffer[RIGHTSINK_SITE].i_loader_col; i++)
			//{
				for (j = 0; j < st_traybuffer[RIGHTSINK_SITE].i_loader_row; j++)
				{
					st_modulemap.RighthsTray[0][j] = DVC_YES;
				}
			//}
			st_sync.mn_ld_righths_tray_change[0] = CTL_YES;
			RunTransStep = 2000;
		}
		else
		{
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Bwd Heat Sink Transfer] Stacker1에 트레이가 존재하지 않습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[Bwd Heat Sink Transfer] Stacker1 does not exist on the tray");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			sprintf(mc_jamcode,"030101");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1421, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 2000:
		if(st_sync.n_ld_righths_tray_supply[1] == CTL_READY)//교체 완료 또는 Tray 기다림
		{
			RunTransStep = 2050;
		}
		break;

	case 2050:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunTransStep = 2100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunTransStep = 2050;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunTransStep = 2050;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2211, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2100:
		nRet_1 = LD_Righths_TrayMove();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 2200;
		}
		break;

	case 2200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_ON);
		if (st_basic.n_mode_device == 0)		nRet_1 = IO_ON;

		if(nRet_1 == IO_ON)
		{
			//for (i = 0; i < st_traybuffer[RIGHTSINK_SITE].i_loader_col; i++)
			//{
				for (j = 0; j < st_traybuffer[RIGHTSINK_SITE].i_loader_row; j++)
				{
					st_modulemap.RighthsTray[1][j] = st_modulemap.RighthsTray[0][j]; 
					st_modulemap.RighthsTray[0][j] = NO;
				}
			//}
			if(st_handler.cwnd_main != NULL)
			{			
				st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_HS_RIGHT_TRAY);			// 피커 정보 
			}
			RunTransStep = 2300;//2014,0407
		}
		else
		{
			//030101
			sprintf(mc_jamcode,"030101");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1422, st_work.mn_run_status, mc_jamcode);
		}
		break;

	////2014,0407
	case 2300:
		Set_TrayLatchClinder(CYLINDER_ON);
		backuptime[0] = GetCurrentTime();
		RunTransStep = 2310;
		break;

	case 2310:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];

		if (backuptime[2] > 300)
		{
			RunTransStep = 2400;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	////
	case 2400:
		nRet_1 = LD_Righths_TrayBack();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 3000;
		}
		break;


	case 3000:
		st_sync.mn_ld_righths_tray_change[0] = CTL_NO;
		st_sync.mn_ld_righths_tray_change[1] = CTL_YES;  // 교체할 트레이가 있다

		st_sync.n_ld_righths_tray_supply[1] = CTL_LOCK;
		RunTransStep = 3100;
		break;

	case 3100:
		if(st_sync.n_ld_righths_tray_supply[1] == CTL_FREE)
		{
			RunTransStep = 100;			
		}
		break;

	case 10000:
		nRet_1 = LD_Righths_TrayBack();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 11000;
		}
		break;

	case 11000:
		if(st_sync.n_lotend_righths_ldrbt != CTL_YES)
		{
			st_sync.n_lotend_righths_ldtray_site = CTL_NO;		// 일단 LOT END를 시키자.
			mn_LeakM_LotEnd[1] = YES;
			RunTransStep = 0;
		}
		else
		{
			st_sync.n_lotend_righths_ldtray_site = CTL_YES;		// 일단 LOT END를 시키자.
			RunTransStep = 100;
		}
		break;

	}

}

void CRun_Stacker_Load_RightHeat::Run_Stacker1Move()
{
	int nRet_1, nRet_2, nRet_3, mn_MotorNum;

	mn_MotorNum = M_HS_B_STACKER_1;
	mn_StackerPos = M_STACKER_1;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_HS_B_STACKER_1);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_1] = 0;
		ReadyPosStep[mn_StackerPos] = 0;
		COMI.mn_home_step[M_HS_B_STACKER_1] = 0;
		return;
	}
	Func.ThreadFunctionStepTrace(61, RunS1Step);
	switch(RunS1Step)
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES)
		{
			if(st_sync.n_lotend_righths_ldrbt == CTL_YES)
			{
				st_sync.n_lotend_righths_ldstacker_site = CTL_YES;
				mn_LeakM_LotEnd[0] = NO;
			}
			if (mn_LeakM_LotEnd[0] == NO && st_sync.n_lotend_righths_ldstacker_site == CTL_NO)
			{
				RunS1Step = 10;
			}
		}
		break;

	case 10:
		//070600 0 01 "Load HS Back Slide에 트레이가 존재하지 않습니다."
		//070601 0 01 "Load HS Back Slide에 트레이를 제거해 주세요."
		//070602 0 01 "Unload HS Back Slide에 트레이가 존재하지 않습니다."
		//070603 0 01 "Unload HS Back Slide에 트레이를 제거해 주세요."
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_tray_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_hs_bwd_uld_slide_tray_chk, IO_OFF);
//		nRet_1 = IO_ON;
		if(nRet_1 == IO_OFF || nRet_2 == IO_ON || nRet_3 == IO_ON)
		{
			if(nRet_1 == IO_OFF)
			{//060101 0 06 "Right Heat Sink#l stacker에 트레이가 존재지 않습니다."
				sprintf(mc_jamcode, "070600"); 
			}
			else if(nRet_2 == IO_ON)
			{//070101 0 07 "Right Heat Sink#2 stacker에 트레이가 존재지 않습니다."
				sprintf(mc_jamcode, "070101"); 
			}
			else
			{//070801 0 01 "HS Back Slide에 트레이를 제거해 주세요."
				sprintf(mc_jamcode, "070603"); 
			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1423, st_work.mn_run_status, mc_jamcode);	
		}
		else
		{
			RunS1Step = 20;
		}
		break;

	case 20:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_ON);
		
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			RunS1Step = 30;
		}
		else
		{
			if(nRet_1 == IO_ON)
			{
				//060202 0 06 "Right Heat Sink#l Stacker 레일에 트레이가 있습니다."
				sprintf(mc_jamcode, "060202");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1424, st_work.mn_run_status, mc_jamcode);	
			}
			else
			{
				RunS1Step = 30;
			}
			break;
		}

	case 30:
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			RunS1Step = 40;
			break;
		}

		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_onoff_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_hs_bwd_uld_slide_onoff_chk, IO_OFF);
		if(nRet_1 == IO_ON && nRet_2 == IO_ON)
		{
			mn_retry1 = 0;
			RunS1Step = 40;
		}
		else
		{//070802 0 07 "Load HS BackSlide가 Open되어 있습니다. Close 해 주세요."
		 //070804 0 07 "Unload HS BackSlide가 Open되어 있습니다. Close 해 주세요."
			if(nRet_1 == IO_OFF)	sprintf(mc_jamcode, "070802");
			else				sprintf(mc_jamcode, "070804");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1030, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 40:
		nRet_1 = LD_Righths_Seperate_Tray(CYLINDER_OFF);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 100;
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
			//060204 0 06 "Right Heat Sink#l Stacker 레일 backward가 동작이 되지 않습니다."
			sprintf(mc_jamcode, "060204");			
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1425, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			RunS1Step = 300;
		}
		else
		{
			//060202 0 06 "Right Heat Sink#l Stacker 레일에 트레이가 있습니다."
			sprintf(mc_jamcode, "060202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1426, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 300:
		mn_stacker1_init_chk = YES;
		RunS1Step = 900;
		break;
		st_sync.n_righths_guide_tray_stacker[0] = CTL_REQ;
		RunS1Step = 400;
		break;

	case 400:
		if(st_sync.n_righths_guide_tray_stacker[1] == CTL_LOCK)
		{
			st_sync.n_righths_guide_tray_stacker[0] = CTL_CLEAR;
		}
		break;

	case 900:
		if(mn_Moving_stacker == YES)
		{
			RunS1Step = 1000;
		}
		break;

	case 1000:
		nRet_1 = OnRighthsStackerReadyPos(0,1);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			st_sync.n_ld_righths_tray_supply[0] = CTL_READY;
			RunS1Step = 1100;
		}
		else if(nRet_1 == CTL_FULL)
		{
			//060200 0 06 "Right Heat Sink#l Stacker Tray Full 에러"
			sprintf(mc_jamcode, "060200"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1429, st_work.mn_run_status, mc_jamcode);	
			RunS1Step = 1000;
		}
		else if(nRet_1 == CTLBD_RET_SKIP) //스태커에 트레이가 없을때 
		{
			st_sync.n_lotend_righths_ldtray_site = CTL_YES;
			RunS1Step = 1100;
		}
		break;

	case 1100:
		if(st_sync.n_lotend_righths_uldstacker_site == CTL_YES || mn_LeakM_LotEnd[1] == YES || (COMI.mn_simulation_mode == 1 &&(st_work.nMdlInputCount[0][0] == st_work.nMdlPassCount[0][0]))) // 20130509
		{
			RunS1Step = 10000;
		}
		else if(st_sync.n_ld_righths_tray_supply[0] == CTL_CHANGE)
		{
			RunS1Step = 1200;
		}
		break;

	case 1200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_ON)
		{
			//060202 0 06 "Right Heat Sink#l Stacker 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"060202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1430, st_work.mn_run_status, mc_jamcode);			
		}
		else
		{
			RunS1Step = 1210;
		}
		break;

	case 1210:
		if (FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_on_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_off_chk, IO_ON) == IO_ON)	// Off 상태.
		{
			RunS1Step = 1300;
		}
		else
		{
			nRet_1 = Set_RailClampOnOff(CYLINDER_OFF);
			if(nRet_1 == CTL_GOOD)
			{
				mn_retry1 = 0;
				RunS1Step = 1230;
			}
		}
		break;
		
	case 1220:
		nRet_1 = Set_RailClampOnOff(CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			RunS1Step = 1230;
		}
		break;

	case 1230:
		nRet_1 = Get_RailClampOnOff(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			RunS1Step = 1300;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry1++;
			if(mn_retry1 > 3)
			{
				mn_retry1 = 0;
				Set_RailClampOnOff(CYLINDER_OFF);
//				060500 0 03 "Right Heat Sink#l Stacker 레일 Clamp Fwd가 동작이 되지 않습니다."
//				060501 0 03 "Right Heat Sink#l Stacker 레일 Clamp Bwd가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"060501");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1492, st_work.mn_run_status, mc_jamcode);
				RunS1Step = 1220;
			}
			else
			{
				RunS1Step = 1220;
			}
		}
		break;	

	case 1300:
		if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_fwd_chk + (mn_StackerPos*3), IO_OFF) == IO_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_bwd_chk + (mn_StackerPos*3), IO_ON) == IO_ON))	// Off 상태.
		{
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
		if(nRet_1 == CYLINDER_OFF)
		{
			ReadyPosStep[mn_StackerPos] = 0;
			RunS1Step = 1500;
		}
		else if(nRet_1 == CYLINDER_OFF)
		{
			mn_retry1++;
			if(mn_retry1 > 3)
			{
				mn_retry1 = 0;
				//060204 0 06 "Right Heat Sink#l Stacker 레일 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"060204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1431, st_work.mn_run_status, mc_jamcode);
				RunS1Step = 1310;
			}
			else
			{
				RunS1Step = 1310;
			}
		}
		break;

	case 1500:
		nRet_1 = OnRighthsStackerReadyPos(mn_StackerPos, 0);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 1600;
		}
		else if(nRet_1 == CTLBD_RET_SKIP)
		{
			RunS1Step = 1100;
		}
		break;

	case 1600:
		nRet_1 = COMI.Get_MotIOSensor(mn_MotorNum, MOT_SENS_SD);
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 300;	
			RunS1Step = 1700;	
		}
		else
		{
			//nRet_2 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_ON);
			//if(nRet_2 == IO_OFF && mn_stacker_updn_cyliner[mn_StackerPos] == CYLINDER_ON)
			//{
			//	RunS1Step = 4000;
			//}
			//else//다시 시도한다
			//{
				RunS1Step = 1700;
			//}
		}
		break;			

	case 1700:
		nRet_1 = LD_Righths_Seperate_Tray(CYLINDER_ON);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 2000;
		}
		break;

	case 2000:
		nRet_1 = OnRighthsStackerReadyPos(0, 2); //트레 하나 Dn한다
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 2100;
		}
		else if (nRet_1 == CTLBD_RET_SKIP)
		{
			//if(st_sync.n_lotend_lefths_ldtray_site == CTL_YES)
			//{
				RunS1Step = 1100;
			//}
			//else
			//{
			//	RunS1Step = 2000;
			//}
		}
		break;

	case 2100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_ON);
		if (nRet_1 == IO_ON || st_basic.n_mode_device == 2)	// 트레이가 있다
		{
			RunS1Step = 2200;
		}
		else
		{//060201 0 06 "Right Heat Sink#l Stacker 레일에 트레이가 존재하지 않습니다."
                                                                                                   		//	RunS1Step = 2110;//20120728
			sprintf(mc_jamcode,"060201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1498, st_work.mn_run_status, mc_jamcode);
			RunS1Step = 2110;
		}
		break;

	case 2110:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_ON);
		if (nRet_1 == IO_ON || st_basic.n_mode_device == 2)	// 트레이가 있다
		{
			RunS1Step = 2200;
		}
		else
		{
			RunS1Step = 1200;
		}
		break;


/*	case 2110:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_STACKER_1, 5, st_basic.nRunSpeed);
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			sprintf(mc_jamcode,"060201");
			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1434, st_work.mn_run_status, mc_jamcode);
			RunS1Step = 1500;
		}
		else if (nRet_1 == CTLBD_RET_ERROR)
		{
			//060010 0 06 "Right Heat Sink#l stacker Motor 위치 이동 에러."
			sprintf(mc_jamcode,"060010");
			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
			alarm.mn_type_mode = eWARNING;			// 현재 발생한 알람 상태 플래그 설정 
			alarm.mn_alarm_assign_section = 1721;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1499, st_work.mn_run_status, mc_jamcode);	
		}
		break;*/


	case 2200:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			RunS1Step = 2300;
		}
		break;

	case 2300:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(0, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			ml_until_waittime[0] = GetCurrentTime();
			RunS1Step = 2310;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			//060203 0 06 "Right Heat Sink#l Stacker 레일 forward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"060203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1432, st_work.mn_run_status, mc_jamcode);
			RunS1Step = 2200;
		}
		break;

	case 2310:
		ml_until_waittime[1] = GetCurrentTime();
		ml_until_waittime[2] = ml_until_waittime[1] - ml_until_waittime[0];
		if(ml_until_waittime[2] > 500)
		{
			mn_retry1 = 0;
			RunS1Step = 2400;
		}
		else if(ml_until_waittime[2] < 0)
		{
			ml_until_waittime[0] = GetCurrentTime();
		}
		break;


	case 2400:
		nRet_1 = LD_Righths_Seperate_Tray(CYLINDER_OFF);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 3000;
		}
		break;

	case 3000:
		st_sync.n_ld_righths_tray_supply[mn_StackerPos] = CTL_LOCK;		// 교체 끝났다고 설정.
		//트레이 정보 
		RunS1Step = 1000;
		break;

		
	case 10000:
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_tray_chk, IO_OFF) == IO_ON)
		{
			//070601 0 07 "Load HS Back Slide에 트레이를 제거해 주세요."
			sprintf(mc_jamcode, "070601"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1323, st_work.mn_run_status, mc_jamcode);	
		}
		else
		{
			RunS1Step = 10010;
		}
		break;

	case 10010:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_STACKER_1, st_motor[M_HS_B_STACKER_1].md_pos[P_STOCKER_P_LIMIT], st_basic.nRunSpeed);
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 10200;
		}
		else if (nRet_1 == CTLBD_RET_ERROR)
		{
			//060010 0 06 "Right Heat Sink#l stacker Motor 위치 이동 에러."
			sprintf(mc_jamcode,"060010");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1435, st_work.mn_run_status, mc_jamcode);	
		}
		break;
		
	case 10100:
		if(st_sync.n_lotend_lefths_ldrbt == CTL_YES)
		{
			RunS1Step = 10200;
		}
		else if (st_sync.mn_ld_module_tray_supply[0] == CTL_REQ)//일단, 나중에 생산 lot이 적을 경우 추가하는 경우도 있을 수 있겠다. feedback으로 위치 확인 필요.
		{
//			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_tray_chk, IO_OFF);
//			
//			if(nRet_1 == IO_OFF)
//			{//990104 0 99 "Lot 트레이를 공급해 주세요."
//				//st_sync.n_lotend_module_ldstacker_site = CTL_YES;
//				sprintf(mc_jamcode,"990104");
//				st_work.mn_run_status = CTL_dWARNING;
//				CTL_Lib.Alarm_Error_Occurrence(1090, st_work.mn_run_status, mc_jamcode);
//			}
//			else//다시 시도한다
//			{
//				RunS1Step = 1000;
//			}
		}
		break;
		
//	case 10100:
//		nRet_1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_1, 0, MOT_TIMEOUT);
//		
//		if (nRet_1 == CTLBD_RET_GOOD)
//		{
//			RunS1Step = 10200;
//		}
//		else if (nRet_1 == CTLBD_RET_ERROR)
//		{
//			//060300 0 06 "Right Heat Sink#l Stacker Motor 홈 체크 에러."
//			sprintf(mc_jamcode,"060300");
//			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
//			alarm.mn_alarm_assign_section = 1722;
//			CTL_Lib.Alarm_Error_Occurrence(1436, st_work.mn_run_status, mc_jamcode);	
//		}
//		break;

	case 10200:
		if(st_sync.n_lotend_righths_ldrbt != CTL_YES)
		{
			st_sync.n_lotend_righths_ldstacker_site = CTL_NO;
			mn_LeakM_LotEnd[0] = YES;
			RunS1Step = 0;
		}
		else
		{
			st_sync.n_lotend_righths_ldstacker_site = CTL_YES;
			RunS1Step = 0;
		}
		break;	
	}	
}

void CRun_Stacker_Load_RightHeat::Run_Stacker2Move()
{
	int nRet_1, nRet_2;
	int mn_MotorNum, mn_StackerPos;

	mn_MotorNum = M_HS_B_STACKER_2;
	mn_StackerPos = M_STACKER_2;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_HS_B_STACKER_2);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_2] = 0;
		ReadyPosStep[mn_StackerPos] = 0;
		COMI.mn_home_step[M_HS_B_STACKER_2] = 0;
		return;
	}

	Func.ThreadFunctionStepTrace(62, RunS2Step);

	switch(RunS2Step)
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(mn_LeakM_LotEnd[2] == NO && st_sync.n_lotend_righths_uldstacker_site == CTL_NO)
			{
				RunS2Step = 10;
			}
		}
		break;

	case 10:
		nRet_2 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_chk, IO_OFF);
		if(nRet_2 == IO_ON)
		{
			//070100 0 07 "Right Heat Sink#2 stacker에 트레이가 존재합니다.제거해 주세요."
			sprintf(mc_jamcode,"070100");
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1440, st_work.mn_run_status, mc_jamcode);			
		}
		else
		{
			RunS2Step = 20;
		}
		break;

	case 20:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_ON)
		{
			//070202 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"070202");
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1441, st_work.mn_run_status, mc_jamcode);			
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
			RunS2Step = 40;
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
				mn_retry2 = 0;
				//070204 0 07 "Right Heat Sink#2 Stacker 레일 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"070204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1442, st_work.mn_run_status, mc_jamcode);			
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
			st_sync.n_ld_righths_tray_supply[1] = CTL_FREE;
			RunS2Step = 70;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//070208 0 07 "Right Heat Sink#2 Latch Clamp Tray Off 에러."
				sprintf(mc_jamcode, "070208");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1443, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 50;
			}
			else
			{
				RunS2Step = 50;
			}
		}
		break;

	case 70:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF);
		
		if (nRet_1 == IO_OFF)	// 트레이가 없으면...
		{
			mn_retry2 = 0;
			//Set_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
			RunS2Step = 90;
		}
		else
		{
			//070202 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"070202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1444, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 80:
		//nRet_1 = Set_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
		//if(nRet_1 == CTL_GOOD)
		//{
			RunS2Step = 90;
		//}
		break;

	case 90:
		//nRet_1 = Get_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
		//if(nRet_1 == CYLINDER_ON)
		//{
			mn_stacker2_init_chk = YES;
			RunS2Step = 100;
		//}
		/*else if(nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//070205 0 07 "Right Heat Sink#2 Stacker 레일 Up 센서 에러."
				sprintf(mc_jamcode,"070205");
				alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
				alarm.mn_alarm_assign_section = 1745;
				CTL_Lib.Alarm_Error_Occurrence(1445, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 80;
			}
			else
			{
				RunS2Step = 80;
			}
		}*/
		break;
		
	case 100:
		if(mn_Moving_stacker == YES)
		{
			st_sync.mn_ld_righths_tray_change[0] = CTL_YES;
			RunS2Step = 900;
		}
		break;

	case 900:
		//SD가 감지 않되고 대기위치로 이동 P_STOCKER_UP
		//트레이를 받기 위해 올리는 위치 P_STOCKER_P_LIMIT
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_STACKER_2, st_motor[M_HS_B_STACKER_2].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);
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
		}
		break;

	case 1000:
		if (st_sync.mn_ld_righths_tray_change[0] == CTL_YES ||
			(st_sync.n_ld_righths_tray_supply[1] == CTL_FREE && mn_LeakM_LotEnd[1] == YES))		// 로드 트레이를 올려 놓을 꺼라고 요청.
		{
			RunS2Step = 1100;
		}
		else
		{
			if(st_sync.n_lotend_righths_ldrbt == YES)
			{
				RunS2Step = 1100;
			}
		}
		break;

	case 1100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF);
		if (nRet_1 == IO_OFF)
		{
			RunS2Step = 2000;
		}
		else// if (nRet_1 == IO_ON)
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
				//070204 0 07 "Right Heat Sink#2 Stacker 레일 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"070204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1446, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 1110;
			}
			else
			{
				RunS2Step = 1110;
			}
		}
		break;

	case 1300:
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_off_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_OFF) == IO_OFF)
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
			{//070201 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 존재하지 않습니다."
				mn_retry2 = 0;
				sprintf(mc_jamcode,"070201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1447, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 1310;
			}
			else
			{
				RunS2Step = 1310;
			}
		}
		break;

	case 2000:
		nRet_1 = OnRighthsStackerReadyPos(mn_StackerPos,1); //SD 대기		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_retry2 = 0;
			Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_ON);
			RunS2Step = 2200;
		}
		else if(nRet_1 == CTL_FULL)
		{
			//070200 0 07 "Right Heat Sink#2 Stacker Tray Full 에러"
			sprintf(mc_jamcode, "070200"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1448, st_work.mn_run_status, mc_jamcode);			
			RunS2Step = 1000;
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
			{//070203 0 06 "Right Heat Sink#2 Stacker 레일 forward가 동작이 되지 않습니다."
				mn_retry2 = 0;
				sprintf(mc_jamcode,"070203");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1449, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 2100;
			}
			else
			{
				RunS2Step = 2100;
			}
		}
		break;

	case 2300:
		if((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_off_chk, IO_ON)) == IO_ON && 
			(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_OFF) == IO_OFF))
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
				//070207 0 07 "Right Heat Sink#2 Latch Clamp Tray On 에러."
				//070208 0 07 "Right Heat Sink#2 Latch Clamp Tray Off 에러."				
				sprintf(mc_jamcode,"070208");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1450, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 2400;
			}
			else
			{
				RunS2Step = 2400;
			}
		}
		break;

	case 3000:
		st_sync.n_ld_righths_tray_supply[1] = CTL_READY; //준비됨.
		RunS2Step = 3100;
		break;

	case 3100:
		if(st_sync.n_ld_righths_tray_supply[1] == CTL_LOCK)
		{
			RunS2Step = 3200;
		}
		else if(st_sync.n_ld_righths_tray_supply[0] == CTL_NOTREADY || 
			(st_work.nMdlInputCount[0][0] == st_work.nMdlPassCount[0][0]) && COMI.mn_simulation_mode == 1) // 20130509
		{
			RunS2Step = 10000;
		}
		break;

	case 3200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_ON);

		if (nRet_1 == IO_ON || st_basic.n_mode_device == 2)	// 트레이가 있으면...		
		{
			mn_retry2 = 0;
			Set_TrayLatchClinder(CYLINDER_ON);
			RunS2Step = 3500;
		}
		else
		{
			//070201 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 존재하지 않습니다."
			sprintf(mc_jamcode,"070201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1451, st_work.mn_run_status, mc_jamcode);
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
			if(st_work.n_hsNojob == 3)//RDMM
			{
//				Set_TrayLatchClinder(CYLINDER_OFF);//2014,0403
			}
			RunS2Step = 3600;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//070207 0 07 "Right Heat Sink#2 Latch Clamp Tray On 에러."
				sprintf(mc_jamcode,"070207");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1452, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 3300;
			}
			else
			{
				RunS2Step = 3300;
			}
		}
		break;

	case 3600:
		st_sync.n_ld_righths_tray_supply[1] = CTL_FREE; //교체 환료
		RunS2Step = 3700;

		//2016.0518
		if( st_lamp.mn_rear_mode == CTL_YES )
		{
			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON)
			{
				FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker2_tray_clamp_onoff, IO_OFF);
			}
		}

		break;

	case 3700:
		if(st_sync.n_righths_ldrbt_tray_req[0] == CTL_REQ || st_sync.n_lotend_righths_ldrbt == CTL_YES)
		{
			st_sync.n_righths_ldrbt_tray_req[0] = CTL_READY;
			st_sync.n_righths_ldrbt_tray_req[1] = BIN_LDBUFFERBIN;
			RunS2Step = 3800;
		}
		break;

	case 3800:
//BCR_READ
		if(st_basic.n_mode_device == YES && st_basic.n_mode_bcr == YES)
		{
			if(st_handler.mb_bcr_read == FALSE)
			{
				FAS_IO.set_out_bit(st_io.o_back_hs_bcr_onoff, IO_ON);//2014,0218 off->on
				ml_until_waittime[0] = GetCurrentTime();
				st_handler.mb_bcr_read = TRUE;
				RunS2Step = 3900;
			}
		}
		else
		{
			RunS2Step = 4000;
		}
		break;

	case 3900:
		if(st_basic.n_mode_bcr != YES)
		{
			RunS2Step = 4000;
			break;
		}//Read BCR#1
		ml_until_waittime[1] = GetCurrentTime();
		ml_until_waittime[2] = ml_until_waittime[1] - ml_until_waittime[0];
		if(ml_until_waittime[2] < 0) ml_until_waittime[0] = GetCurrentTime();
		if(ml_until_waittime[2] < 1000) break;
		if(Func.Robot_BarcodeScanning(HEATSINK_LEFT_PORT) == RET_GOOD)
		{
//			st_handler.mstr_bcr_id2 = st_msg.mstr_recive;
//			if(st_handler.cwnd_main != NULL)
//			{			
//				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, 4);			// 피커 정보 
//			}
//			FAS_IO.set_out_bit(st_io.o_front_hs_bcr_onoff, IO_ON);//2014,0218
			ml_until_waittime[0] = GetCurrentTime();
			RunS2Step = 3910;
		}	
		break;

	case 3910://2014,0218
//		if(st_basic.n_mode_bcr != YES)
//		{
//			RunS2Step = 4000;
//			break;
//		}//Read BCR#1
//		//Read BCR#2
//		ml_until_waittime[1] = GetCurrentTime();
//		ml_until_waittime[2] = ml_until_waittime[1] - ml_until_waittime[0];
//		if(ml_until_waittime[2] < 0) ml_until_waittime[0] = GetCurrentTime();
//		if(ml_until_waittime[2] < 1000) break;
//		if(Func.Robot_BarcodeScanning(HEATSINK_LEFT_PORT) == RET_GOOD)
//		{
////			st_handler.mstr_bcr_id2_1 = st_msg.mstr_recive;
////			if(st_handler.cwnd_main != NULL)
////			{			
////				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, 8);			// 피커 정보 
////			}
//			Func.BarcodeScanStep = 0;
//			st_handler.mb_bcr_read = FALSE;
//			RunS2Step = 4000;
//		}
		RunS2Step = 4000;
		break;

	case 4000://한장 빼기
// 		if(st_sync.n_righths_ldrbt_tray_req[0] == CTL_REQ || st_sync.n_lotend_righths_ldrbt == CTL_YES)
// 		{
// 			st_sync.n_ld_righths_tray_supply[1] = CTL_CHANGE;
// 			RunS2Step = 4100;
// 		}
// 		else if(st_sync.n_lotend_righths_ldrbt == YES || st_handler.n_lotend_ready == 4)
// 		{
// 			st_sync.n_righths_ldrbt_tray_req[0] = CTL_REQ;
// 		}
		//2017.0731
		if(st_sync.n_righths_ldrbt_tray_req[0] == CTL_REQ || st_sync.n_lotend_righths_ldrbt == CTL_YES)
		{
			st_sync.n_ld_righths_tray_supply[1] = CTL_CHANGE;
			RunS2Step = 4100;
		}
		break;

	case 4100:
		if(st_sync.n_ld_righths_tray_supply[1] != CTL_CHANGE) return;

		if (FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_off_chk, IO_OFF) == IO_OFF)	// On 상태.
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
		if (nRet_1 == CYLINDER_ON)
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
				mn_retry2 = 0;
				//070207 0 07 "Right Heat Sink#2 Latch Clamp Tray On 에러."
				sprintf(mc_jamcode, "070207");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1453, st_work.mn_run_status, mc_jamcode);
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
				//070204 0 07 "Right Heat Sink#2 Stacker 레일 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode, "070204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1454, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 4210;
			}
			else
			{
				RunS2Step = 4210;
			}
		}
		break;

	case 4400:
		if (FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_fwd_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_bwd_chk, IO_ON) == IO_ON)	// On 상태.
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
		nRet_1 = OnRighthsStackerReadyPos(mn_StackerPos, 0); //트레이 하나 Up한다
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_retry2 = 0;
			Set_TrayLatchClinder(CYLINDER_OFF);
			RunS2Step = 4500;
		}
		else if(nRet_1 == CTLBD_RET_SKIP) //2016.0526
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF);
			
			if (nRet_1 == IO_OFF)// 트레이가 없다
			{
				//070201 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 존재하지 않습니다."
				sprintf(mc_jamcode, "070201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1478, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 4400;
			}
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
				//070208 0 07 "Right Heat Sink#2 Latch Clamp Tray Off 에러."
				sprintf(mc_jamcode, "070208");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1455, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 4410;
			}
			else
			{
				RunS2Step = 4410;
			}
		}
		break;
		
	case 4600:
		nRet_1 = OnRighthsStackerReadyPos(mn_StackerPos, 2); //트레이 하나 Dn한다
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS2Step = 4700;
		}
		break;

	case 4700:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_OFF);
		
		if (nRet_1 == IO_OFF || st_basic.n_mode_device == 2)	// 트레이가 없다
		{
			RunS2Step = 5000;
		}
		else
		{
			//070201 0 07 "Right Heat Sink#2 Stacker 레일에 트레이가 존재하지 않습니다."
			sprintf(mc_jamcode, "070201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1456, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 5000:
		st_sync.n_ld_righths_tray_supply[mn_StackerPos] = CTL_FREE;		// 교체 끝났다고 설정.
		//트레이 정보 
		RunS2Step = 1000;
		break;

	case 10000:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_STACKER_2, st_motor[M_HS_B_STACKER_2].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS2Step = 10100;
		}
		else if (nRet_1 == CTLBD_RET_ERROR)
		{
			//040010 0 04 "Left Heat Sink#2 stacker Motor 위치 이동 에러."
			sprintf(mc_jamcode, "070201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1457, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 10100:
//		nRet_1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_2, 0, MOT_TIMEOUT);
//		
//		if (nRet_1 == CTLBD_RET_GOOD)
//		{
//			RunS2Step = 10200;
//		}
//		else if (nRet_1 == CTLBD_RET_ERROR)
//		{
//			//070300 0 07 "Right Heat Sink#2 Stacker Motor 홈 체크 에러."
//			sprintf(mc_jamcode, "070300");
//			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
//			alarm.mn_alarm_assign_section = 1758;
//			CTL_Lib.Alarm_Error_Occurrence(1458, st_work.mn_run_status, mc_jamcode);
//		}
		RunS2Step = 10200;
		break;

	case 10200:
		if(st_sync.n_lotend_righths_ldrbt != CTL_YES)
		{
			st_sync.n_lotend_righths_uldstacker_site = CTL_NO;
			mn_LeakM_LotEnd[2] = YES;
			RunS2Step = 0;
		}
		else
		{
			st_sync.n_lotend_righths_uldstacker_site = CTL_YES;	
			RunS2Step = 0;
		}
		break;	


	}
}

void CRun_Stacker_Load_RightHeat::Run_Moving_Stacker()
{
	int nRet_1;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_HS_B_STACKER_MOVE);
		CTL_Lib.mn_single_motmove_step[M_HS_B_STACKER_MOVE] = 0;
		COMI.mn_home_step[M_HS_B_STACKER_MOVE] = 0;
		return;
	}

	Func.ThreadFunctionStepTrace(66, RunMStep);
	switch(RunMStep)
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(mn_LeakM_LotEnd[3] == NO && st_sync.n_lotend_righths_movingtray_site == CTL_NO)
			{
				mn_ms_retry = 0;
				RunMStep = 100;
			}
		}		
		break;

	case 100:
		//if(st_sync.n_righths_guide_tray_stacker[0] == CTL_REQ)
		//{
			RunMStep = 110;
		//}
		break;

	case 110:
		RunMStep = 1000;
		break;
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_OFF);
		RunMStep = 200;
		break;

	case 200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_guide_clamp_onoff, st_io.i_hs_bwd_stacker_guide_clamp_on_chk, st_io.i_hs_bwd_stacker_guide_unclamp_off_chk,
			IO_ON, st_wait.n_on_wait_time[25], st_wait.n_limit_wait_time[25]);
		if(nRet_1 == RET_GOOD)
		{
			mn_ms_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_OFF);
			FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_ON);
			RunMStep = 300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//070400 0 07 "Right Heat Sink#2 stacker이 guide clamp가 On이 되지 않습니다."
		 //070401 0 07 "Right Heat Sink#2 stacker이 guide clamp가 Off가 되지 않습니다."
			mn_ms_retry++;
			if(mn_ms_retry > 3)
			{
				mn_ms_retry = 0;
				sprintf(mc_jamcode, "070400");			
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1427,  st_work.mn_run_status, mc_jamcode);
				RunMStep = 110;
			}
			else
			{
				RunMStep = 110;
			}
		}
		break;

	case 210:
		st_sync.n_righths_guide_tray_stacker[1] = CTL_LOCK;
		RunMStep = 1000;
		break;

		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_hs_bwd_stacker_guide_unclamp_onoff, IO_ON);
		RunMStep = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_hs_bwd_stacker_guide_clamp_onoff, st_io.i_hs_bwd_stacker_guide_unclamp_off_chk, st_io.i_hs_bwd_stacker_guide_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[25], st_wait.n_limit_wait_time[25]);
		if(nRet_1 == RET_GOOD)
		{
			st_sync.n_righths_guide_tray_stacker[1] = CTL_LOCK;
			RunMStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//070400 0 07 "Right Heat Sink#2 stacker이 guide clamp가 On이 되지 않습니다."
		 //070401 0 07 "Right Heat Sink#2 stacker이 guide clamp가 Off가 되지 않습니다."
			mn_ms_retry++;
			if(mn_ms_retry > 3)
			{
				mn_ms_retry = 0;
				sprintf(mc_jamcode, "070401");			
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1428,  st_work.mn_run_status, mc_jamcode);
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
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_STACKER_MOVE, st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS], st_basic.nRunSpeed);
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
			CTL_Lib.Alarm_Error_Occurrence(1478, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2000:
		if(st_sync.n_lotend_righths_ldrbt == CTL_YES && st_sync.n_lotend_righths_ldstacker_site == CTL_YES && 
			st_sync.n_lotend_righths_uldstacker_site == CTL_YES)
		{
			RunMStep = 2100;
		}
		else if(mn_LeakM_LotEnd[0] == YES && mn_LeakM_LotEnd[1] == YES && mn_LeakM_LotEnd[2] == YES)
		{
			RunMStep = 2100;
			m_strTmp = "[RightHeat]mn_LeakM_LotEnd[0] == YES";
			Func.On_LogFile_Add(99, m_strTmp);
		}
		break;

	case 2100:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_STACKER_MOVE, st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunMStep = 3000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunMStep = 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1479, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3000:
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_uld_slide_tray_chk, IO_ON) == IO_ON)
		{
			RunMStep = 4000;
		}
		else
		{//030800 0 01 "HS Front Slide에 트레이가 존재하지 않습니다."
			sprintf(mc_jamcode, _T("030800"));
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1495, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 4000:
// 		if(mn_LeakM_LotEnd[0] == YES && mn_LeakM_LotEnd[1] == YES && mn_LeakM_LotEnd[2] == YES)
// 		{
// 			st_sync.n_lotend_righths_movingtray_site = CTL_NO;
// 			if(st_handler.n_lotend_ready == 4)
// 			{
// 				st_handler.n_lotend_ready = 5;
// 				st_sync.n_lot_reready[2] = CTL_REQ;
// 				if(st_handler.cwnd_list != NULL)//2016.0520
// 				{
// 					sprintf(st_msg.c_normal_msg, _T("[Stacker_Load_LeftHeat] st_handler.n_lotend_ready = 5"));
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 				}
// 			}
// 			//st_sync.n_lot_reready[2] = CTL_REQ;//2016.0520
// 			mn_Moving_stacker = NO;
// 			mn_LeakM_LotEnd[3] = YES;
// 			RunMStep = 5000;
// 		}
// 		else if(st_sync.n_lotend_righths_ldrbt == CTL_YES && st_sync.n_lotend_righths_ldstacker_site == CTL_YES && 
// 			st_sync.n_lotend_righths_uldstacker_site == CTL_YES)
// 		{
// 			st_sync.n_lotend_righths_movingtray_site = CTL_YES;
// 			mn_Moving_stacker = NO;
// 			RunMStep = 0;
// 		}
		//2017.0731
		if(mn_LeakM_LotEnd[0] == YES && mn_LeakM_LotEnd[1] == YES && mn_LeakM_LotEnd[2] == YES)
		{
			mn_Moving_stacker = NO;
			mn_LeakM_LotEnd[3] = YES;
			RunMStep = 5000;
		}
		else
		{
			mn_Moving_stacker = NO;
			RunMStep = 0;
		}
		break;

	case 5000:
// 		if(st_sync.n_lot_reready[2] == CTL_READY)
// 		{
// 			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_uld_slide_tray_chk, IO_OFF) == IO_OFF)
// 			{
// 				st_handler.mn_init_state[INIT_RIGHTHS_STACKER] = CTL_NO;
// 				InitStep = 0;
// 				RunMStep = 5200;
// 			}
// 			else
// 			{
// 				RunMStep = 5100;
// 			}
// 		}
// //		else if(st_work.n_lotend != CTL_YES && st_work.n_loadlot_count[LDMODULE_SITE] > st_work.n_loadlot_count[RIGHTSINK_SITE])
// 		else if(st_handler.n_more_righths == CTL_YES)
// 		{
// 			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_uld_slide_tray_chk, IO_OFF) == IO_OFF)
// 			{
// 				st_handler.mn_init_state[INIT_RIGHTHS_STACKER] = CTL_NO;
// 				InitStep = 0;
// 				RunMStep = 5200;
// 			}
// 			else
// 			{
// 				RunMStep = 5100;
// 			}
// 		}
// 		/////////////////// 20130611
// 		else if(st_sync.n_lot_reready[2] == CTL_CLEAR)
// 		{
// 			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_uld_slide_tray_chk, IO_ON) == IO_ON)
// 			{
// 				st_sync.n_lot_reready[2] = CTL_READY;
// 			}
// 		}
		//2017.0731
		if(st_handler.n_more_righths == CTL_YES)
		{
			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_uld_slide_tray_chk, IO_OFF) == IO_OFF)
			{
				st_handler.mn_init_state[INIT_RIGHTHS_STACKER] = CTL_NO;
				InitStep = 0;
				RunMStep = 5200;
			}
			else
			{
				RunMStep = 5100;
			}
		}
		else//lotend
		{
			RunMStep = 5400;
		}

		break;

	case 5100:
		//994004 0 99 "Back Heat Sink Blank(빈)자재를 제거해 주세요."
		sprintf(mc_jamcode, "994004"); 
		st_work.mn_run_status = CTL_dWARNING;
		CTL_Lib.Alarm_Error_Occurrence(1480, st_work.mn_run_status, mc_jamcode);
		RunMStep = 5000;
		break;


	case 5200:
// 		Run_Init();
// 		if(st_handler.mn_init_state[INIT_RIGHTHS_STACKER] == CTL_YES)
// 		{
// 			st_sync.n_lot_reready[2] = CTL_LOCK;
// 			RunMStep = 5300;
// 		}
		//2017.0731
		Run_Init();
		if(st_handler.mn_init_state[INIT_RIGHTHS_STACKER] == CTL_YES)
		{
			RunMStep = 5300;
		}
		break;

	case 5300:
// 		if(st_sync.n_lot_reready[2] == CTL_CLEAR)// || st_handler.n_lotend_ready != 3)
// 		{
// 			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_tray_chk, IO_ON);
// 			if(nRet_1 == IO_OFF)
// 			{
// 				//994005 0 99 "Back Heat Sink 자재를 투입해 주세요."
// 				sprintf(mc_jamcode, "994005"); 
// 				st_work.mn_run_status = CTL_dWARNING;
// 				CTL_Lib.Alarm_Error_Occurrence(1481, st_work.mn_run_status, mc_jamcode);
// 			}
// 			else
// 			{
// 				RunMStep = 5400;
// 			}
// 		}
// //		else if(st_work.n_lotend != CTL_YES && st_work.n_loadlot_count[LDMODULE_SITE] > st_work.n_loadlot_count[RIGHTSINK_SITE])
// 		else if(st_handler.n_more_righths == CTL_YES)
// 		{
// 			st_sync.n_lot_reready[2] = CTL_CLEAR;
// 			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_tray_chk, IO_ON);
// 			if(nRet_1 == IO_OFF)
// 			{
// 				//994005 0 99 "Back Heat Sink 자재를 투입해 주세요."
// 				sprintf(mc_jamcode, "994005"); 
// 				st_work.mn_run_status = CTL_dWARNING;
// 				CTL_Lib.Alarm_Error_Occurrence(1482, st_work.mn_run_status, mc_jamcode);
// 			}
// 			else
// 			{
// 				RunMStep = 5400;
// 			}
// 		}
// 		/////////////////// 20130611
// 		else if(st_sync.n_lot_reready[2] != CTL_CLEAR && st_handler.mn_init_state[INIT_RIGHTHS_STACKER] == CTL_YES)
// 		{
// 			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_tray_chk, IO_OFF) == IO_OFF)
// 			{
// 				st_sync.n_lot_reready[2] = CTL_CLEAR;
// 			}
// 		}
		//2017.0731
		if(st_handler.n_more_righths == CTL_YES)
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_tray_chk, IO_ON);
			if(nRet_1 == IO_OFF)
			{
				//994005 0 99 "Back Heat Sink 자재를 투입해 주세요."
				sprintf(mc_jamcode, "994005"); 
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1482, st_work.mn_run_status, mc_jamcode);
			}
			else
			{
				RunMStep = 5400;
			}
		}
		else if(st_handler.mn_init_state[INIT_RIGHTHS_STACKER] == CTL_YES)
		{
			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_tray_chk, IO_OFF) == IO_OFF)
			{
				st_sync.n_lot_reready[2] = CTL_CLEAR;
			}
		}

		break;

	case 5400:
		st_handler.n_more_righths = CTL_NO;
		st_sync.n_ld_righths_tray_supply[0] = CTL_CLEAR;
		mn_LeakM_LotEnd[0] = mn_LeakM_LotEnd[1] = mn_LeakM_LotEnd[2] = mn_LeakM_LotEnd[3] = NO;
		RunMStep = 0;
		break;
	}	
}

void CRun_Stacker_Load_RightHeat::Robot_BackMovePosValCheck()//20121126
{
	//여기서 이전에 동작했던 위치를 다시 계산해야한다.
		st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_1] =  st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1];
		st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_2] =  st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2];
		st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_MOVE] =  st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE];
}

void CRun_Stacker_Load_RightHeat::Run_Reinstate()//20121126
{
	int Ret1 = 0, Ret2 = 0, i = 0, n_response = 0;
	int FuncRet = RET_PROCEED;
	int nRet1 = 0, nRet2 = 0;
	double dPosition[10];

	if (st_work.mn_RightHsRobotStacker_Reinstatement_Ok == TRUE) return;//20121126
	
	if (st_work.mn_reinstatement_mode[RIGHTSINK_SITE] == 0) // 완전 초기화 후만 관리함 
	{	// 현재 위치 백업 
		st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1] =  COMI.md_cmdpos_backup[M_HS_B_STACKER_1];
		st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2] =  COMI.md_cmdpos_backup[M_HS_B_STACKER_2];
		st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] =  COMI.md_cmdpos_backup[M_HS_B_STACKER_MOVE];

		st_work.mn_TrayClampStatusBackup[RIGHTSINK_SITE][1] = FAS_IO.get_out_bit(st_io.o_hs_bwd_stacker2_tray_clamp_onoff, IO_OFF);
		st_work.mn_reinstatement_mode[RIGHTSINK_SITE] = 1; //한번 데이터를 저장한다 
		ReinstateStep = 0;

	}	 

	switch (ReinstateStep)
	{
	case 0:
		ReinstateStep = 1000;
		break;

	case 1000://현재 위치 확인
		dPosition[0] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_1);
		dPosition[1] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_2);
		dPosition[2] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_MOVE);

		for (i = 0; i < 3; i++)
		{
			mn_change[RIGHTSINK_SITE][i] = 0;
		}
	
		//변경된 위치값 출력
		if (dPosition[0] >  st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1] + COMI.mn_allow_value[M_HS_B_STACKER_1]
			|| dPosition[0] < st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1] - COMI.mn_allow_value[M_HS_B_STACKER_1])
		{
			mn_change[RIGHTSINK_SITE][0]++;
			sprintf(st_msg.c_abnormal_msg, "Right HS Stacker#1 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1], (long)dPosition[0], (long)COMI.mn_allow_value[M_HS_B_STACKER_1]);
		}
		if (dPosition[1] > st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2] + COMI.mn_allow_value[M_HS_B_STACKER_2]
			|| dPosition[1] < st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2] - COMI.mn_allow_value[M_HS_B_STACKER_2])
		{
			mn_change[RIGHTSINK_SITE][1]++;
			sprintf(st_msg.c_abnormal_msg, "Right HS Stacker#2 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2], (long)dPosition[1], (long)COMI.mn_allow_value[M_HS_B_STACKER_2]);
		}
		if (dPosition[2] > st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] + COMI.mn_allow_value[M_HS_B_STACKER_MOVE]
			|| dPosition[2] < st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] - COMI.mn_allow_value[M_HS_B_STACKER_MOVE])
		{
			mn_change[RIGHTSINK_SITE][2]++;
			sprintf(st_msg.c_abnormal_msg, "Right HS Stacker Move POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE], (long)dPosition[2], (long)COMI.mn_allow_value[M_HS_B_STACKER_MOVE]);
		}

		st_msg.mstr_event_msg[0] = "Recovery Mode.......";
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);

		st_work.mn_reinstatement_mode[RIGHTSINK_SITE] = 3; //초기화 정보를 완료했다, 클리어 

		if(mn_change[RIGHTSINK_SITE][0] > 0 || mn_change[RIGHTSINK_SITE][1] > 0) //stacker 1,2
		{
			ReinstateStep = 2000;
		}
		else if(mn_change[RIGHTSINK_SITE][2] > 0) // tray move
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
		
		COMI.Set_MotPower(M_HS_B_STACKER_1, TRUE);
		COMI.Set_MotPower(M_HS_B_STACKER_2, TRUE);
		COMI.Set_MotPower(M_HS_B_STACKER_MOVE, TRUE);
	
		mn_check[0] = VARIABLE_INIT;
		mn_check[1] = VARIABLE_INIT;

		if(mn_change[RIGHTSINK_SITE][0] > 0 || mn_change[RIGHTSINK_SITE][1] > 0) //stacker 1,2
		{
			ReinstateStep = 2010;
		}
		else if(mn_change[RIGHTSINK_SITE][2] > 0)// tray move
		{
			ReinstateStep = 2100;
		}
		else
		{
			ReinstateStep = 1100;
		}
		break;

	case 2010: //stacker move 축 확인

		if (st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] + COMI.mn_allow_value[M_HS_B_STACKER_MOVE] < st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS]
		|| st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] - COMI.mn_allow_value[M_HS_B_STACKER_MOVE] < st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS])
		{
			ReinstateStep = 2100;
		}
		else
		{
			ReinstateStep = 2020;
		}
		break;

	case 2020: //stacker move 축 확인

		if (st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] + COMI.mn_allow_value[M_HS_B_STACKER_MOVE] >= st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS]
		|| st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] - COMI.mn_allow_value[M_HS_B_STACKER_MOVE] >= st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS])
		{
			ReinstateStep = 2100;
		}
		else
		{
			ReinstateStep = 2030;
		}
		break;

	case 2030://stacker 높이 강제고정 1 : 500, 2 : 600
		if (st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1] + COMI.mn_allow_value[M_HS_B_STACKER_1] >= 500   
			|| st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1] - COMI.mn_allow_value[M_HS_B_STACKER_1] >= 500)
		{
			ReinstateStep = 2050;
			
		}
		else
		{
			ReinstateStep = 2040;
		}
		break;
	case 2040:
		if (st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1] + COMI.mn_allow_value[M_HS_B_STACKER_1] <= st_motor[M_HS_B_STACKER_1].md_pos[P_STOCKER_DOWN]   
			|| st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1] - COMI.mn_allow_value[M_HS_B_STACKER_1] <= st_motor[M_HS_B_STACKER_1].md_pos[P_STOCKER_DOWN])
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
		if (st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2] + COMI.mn_allow_value[M_HS_B_STACKER_2] > 600   
			|| st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2] - COMI.mn_allow_value[M_HS_B_STACKER_2] > 600)
		{
			ReinstateStep = 2070;
			
		}
		else
		{
			ReinstateStep = 2060;
		}
		break;
	
	case 2060:
		if (st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2] + COMI.mn_allow_value[M_HS_B_STACKER_2] <= st_motor[M_HS_B_STACKER_2].md_pos[P_STOCKER_DOWN]   
			|| st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2] - COMI.mn_allow_value[M_HS_B_STACKER_2] <= st_motor[M_HS_B_STACKER_2].md_pos[P_STOCKER_DOWN])
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
		nRet1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_MOVE, st_motor[M_HS_B_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);		
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
		nRet1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_1, st_motor[M_HS_B_STACKER_1].mn_homecheck_method, MOT_TIMEOUT);		
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
		nRet1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_2, st_motor[M_HS_B_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);		
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
		nRet1 = COMI.HomeCheck_Mot(M_HS_B_STACKER_MOVE, st_motor[M_HS_B_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);		
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
		if((st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] > (st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS] + st_motor[M_HS_B_STACKER_MOVE].mn_allow)) ||
			(st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] < (st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] - st_motor[M_HS_B_STACKER_MOVE].mn_allow)))
		{
			ReinstateStep = 2140;
		}
		else
		{
			ReinstateStep = 2150;
		}
		break;

	case 2140:
		nRet1 = CTL_Lib.Single_Move(M_HS_B_STACKER_MOVE, st_motor[M_HS_B_STACKER_MOVE].md_pos[st_work.mn_back_site[M_HS_B_STACKER_MOVE]], st_basic.nManualSpeed);
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
		nRet1 = CTL_Lib.Single_Move(M_HS_B_STACKER_MOVE, st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE], st_basic.nManualSpeed);
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
		nRet1 = CTL_Lib.Single_Move(M_HS_B_STACKER_1, st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1], MOT_TIMEOUT);		
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
		nRet1 = CTL_Lib.Single_Move(M_HS_B_STACKER_2, st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2], MOT_TIMEOUT);		
		if (nRet1 == BD_GOOD)
		{
			ReinstateStep = 50000;
			mn_change[RIGHTSINK_SITE][0] = 0;
			mn_change[RIGHTSINK_SITE][1] = 0;
			mn_change[RIGHTSINK_SITE][2] = 0;
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
 		st_work.mn_reinstatement_mode[RIGHTSINK_SITE] = 0;	// 초기화 정보를 완료했다, 클리어 
		st_work.mn_RightHsRobotStacker_Reinstatement_Ok = TRUE;//20121126
		FuncRet = RET_GOOD;
		break;
	}
}

int CRun_Stacker_Load_RightHeat::Set_TrayLatchClinder(int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker2_tray_clamp_onoff, n_onoff);
	mb_latchcylinder_flag = FALSE;
	ml_latchcylinder_Time[0] = GetCurrentTime();

	return nFuncRet;
}

int CRun_Stacker_Load_RightHeat::Get_TrayLatchClinder(int n_onoff)
{
	int nFuncRet = -1;

	if(COMI.mn_simulation_mode == 1) // 20130509
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
		else if (FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_ON) == CTL_ON && 
			FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_off_chk, IO_OFF) == CTL_OFF)	// On 상태.
		{
			ml_latchcylinder_Time[2] = ml_latchcylinder_Time[1] - ml_latchcylinder_Time[0];
			
			if (ml_latchcylinder_Time[2] < 0)		
			{
				ml_latchcylinder_Time[0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
			{
				if (ml_latchcylinder_Time[2] > st_wait.n_limit_wait_time[25])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_latchcylinder_flag = FALSE;
				}
			}
			else if (ml_latchcylinder_Time[2] > st_wait.n_on_wait_time[25])
			{
				nFuncRet = CYLINDER_ON;
				mb_latchcylinder_flag = FALSE;
			}
		}
		else if (FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_OFF) == CTL_OFF && 
			FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_off_chk, IO_ON) == CTL_ON)	// Off 상태.
		{
			ml_latchcylinder_Time[2] = ml_latchcylinder_Time[1] - ml_latchcylinder_Time[0];
			
			if (ml_latchcylinder_Time[2] < 0)		
			{
				ml_latchcylinder_Time[0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
			{
				if (ml_latchcylinder_Time[2] > st_wait.n_limit_wait_time[25])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_latchcylinder_flag = FALSE;
				}
			}
			else if (ml_latchcylinder_Time[2] > st_wait.n_on_wait_time[25])
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
			
			if (ml_latchcylinder_Time[2] > st_wait.n_limit_wait_time[25])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_latchcylinder_flag = FALSE;
			}
		}
	}	
	return nFuncRet;
}

int CRun_Stacker_Load_RightHeat::OnRighthsStackerReadyPos(int n_StackerPos, int nMode)
{
	int nRet_1 = VARIABLE_INIT, nRet_2 = VARIABLE_INIT;
	int nFuncRet = CTLBD_RET_PROCEED;
	int	nMotorNum;

	if(n_StackerPos == 0) nMotorNum = M_HS_B_STACKER_1;
	else				  nMotorNum = M_HS_B_STACKER_2;


	if(st_basic.n_mode_device == 2)
	{
		return CTLBD_RET_GOOD;
	}

	//2016.0526
	if(n_StackerPos == 0)
	{
		Func.ThreadFunctionStepTrace(68, ReadyPosStep[n_StackerPos]);
	}
	else
	{
		Func.ThreadFunctionStepTrace(69, ReadyPosStep[n_StackerPos]);
	}

	switch(ReadyPosStep[n_StackerPos])
	{
	case 0:
		//mb_sdstop = TRUE;
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
			ReadyPosStep[n_StackerPos] = 1000;
		}
		else
		{
			m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);
			if(m_dNowPos[n_StackerPos] > st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT] - st_motor[nMotorNum].md_pos[P_STOCKER_UP_OFFSET])
			{
				COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
				ReadyPosStep[n_StackerPos] = 1000;
			}
			else
			{
				ReadyPosStep[n_StackerPos] = 100;
			}
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
			//030010 0 03 "Left Heat Sink#l stacker Motor 위치 이동 에러."
//			if(nMotorNum == 0) alarm.mstr_code = "030010";
//			else               alarm.mstr_code = "040010";
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1470, st_work.mn_run_status, COMI.mc_alarmcode);
			ReadyPosStep[n_StackerPos] = 0;
		}
		else if(nRet_1 == CTLBD_RET_SAFETY)
		{
			//030010 0 03 "Left Heat Sink#l stacker Motor 위치 이동 에러."
//			if(nMotorNum == 0) alarm.mstr_code = "030010";
//			else               alarm.mstr_code = "040010";
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1471, st_work.mn_run_status, COMI.mc_alarmcode);
			ReadyPosStep[n_StackerPos] = 0;
		}
		break;

	case 200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, nMotorNum);
			ReadyPosStep[n_StackerPos] = 300;
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_UP]);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				if (nMode == 0)
				{
					if(n_StackerPos == M_STACKER_2)
					{
						ReadyPosStep[n_StackerPos] = 5000;
					}
					else
					{
						nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
						nRet_2 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk + (n_StackerPos * 3), IO_OFF);//20120728
						if(nRet_1 == IO_ON || nRet_2 == IO_ON)
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
				else if(nMode == 2)// SD 감지후에 DOWN하는 모드
				{
					ReadyPosStep[n_StackerPos] = 300;
				}
				else//1 // SD 감지거나, UP_LIMIT까지만 동작하는 모드이다.
				{
					ReadyPosStep[n_StackerPos] = 5000;
				}
//				if (st_basic.n_mode_device == 0)		ReadyPosStep[n_StackerPos] = 300;
			}
			else if(nRet_1 == CTLBD_RET_ERROR)
			{
				//030010 0 03 "Left Heat Sink#l stacker Motor 위치 이동 에러."
//				if(nMotorNum == 0) alarm.mstr_code = "030010";
//				else               alarm.mstr_code = "040010";
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1472, st_work.mn_run_status, COMI.mc_alarmcode);
				ReadyPosStep[n_StackerPos] = 100;
			}
			else if(nRet_1 == CTLBD_RET_SAFETY)
			{
				//030010 0 03 "Left Heat Sink#l stacker Motor 위치 이동 에러."
//				if(nMotorNum == 0) alarm.mstr_code = "030010";
//				else               alarm.mstr_code = "040010";
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1473, st_work.mn_run_status, COMI.mc_alarmcode);
				ReadyPosStep[n_StackerPos] = 100;
			}
///////////////////////////////////////////////// 20120728
//			else if(nRet_1 == CTLBD_RET_RETRY)
//			{
//				ReadyPosStep[n_StackerPos] = 100;
//			}
/////////////////////////////////////////////////
		}
		break;

	case 300:
		if(nMode == 0)//한장 올린다
		{
			m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);
			m_dTargetPos[n_StackerPos] = m_dNowPos[n_StackerPos] + st_motor[nMotorNum].md_pos[P_STOCKER_UP_OFFSET];
			if(m_dTargetPos[n_StackerPos] > st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT])
			{
				m_dTargetPos[n_StackerPos] = st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT];
			}
			ReadyPosStep[n_StackerPos] = 400;
		}
		else if(nMode == 2)//한장만큼 내린다.
		{
			m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);			
			m_dTargetPos[n_StackerPos] = m_dNowPos[n_StackerPos] - (st_motor[nMotorNum].md_pos[P_STOCKER_DN_OFFSET]*3);
			
			if (m_dTargetPos[n_StackerPos] < st_motor[nMotorNum].md_pos[P_STOCKER_DOWN])
			{
				m_dTargetPos[n_StackerPos] = st_motor[nMotorNum].md_pos[P_STOCKER_DOWN];
			}
			
			ReadyPosStep[n_StackerPos] = 400;
		}
		else
		{
			COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
			ReadyPosStep[n_StackerPos] = 5000;
			ReadyPosStep[n_StackerPos] = 1000;
		}
		break;

	case 400:
		COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum, m_dTargetPos[n_StackerPos], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			if(nMode == 0)//UP
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk + (n_StackerPos * 3), IO_ON) == IO_ON)
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
			CTL_Lib.Alarm_Error_Occurrence(1474, st_work.mn_run_status, COMI.mc_alarmcode);
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
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep[n_StackerPos] = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1475, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1100:
		m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);
		if(m_dNowPos[n_StackerPos] > st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT] - st_motor[nMotorNum].md_pos[P_STOCKER_UP_OFFSET]) break;
		
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);

		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			nRet_1 = CTLBD_RET_PROCEED;
		}

		if(nRet_1 != CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, nMotorNum);
			//if(mb_sdstop == TRUE && nMode == 0)//UP할때만 적용
			//{
			//	mb_sdstop = FALSE;
			//	ReadyPosStep[n_StackerPos] = 100;

			//}
			if (nMode == 1)//대기
			{
				ReadyPosStep[n_StackerPos] = 5000;
			}
			else if(nMode == 0)
			{
				ReadyPosStep[n_StackerPos] = 100;
			}
			else
			{
				ReadyPosStep[n_StackerPos] = 100;											// SD센서에 감지가 되지 않으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
			}
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_DOWN]);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				ReadyPosStep[n_StackerPos] = 1200;
			}
			else if(nRet_1 == CTLBD_RET_ERROR)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1476, st_work.mn_run_status, COMI.mc_alarmcode);
				ReadyPosStep[n_StackerPos] = 1000;
			}
			else if(nRet_1 == CTLBD_RET_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1477, st_work.mn_run_status, COMI.mc_alarmcode);
				ReadyPosStep[n_StackerPos] = 1000;
			}
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

	case 2000://tray는 있으나, UP Pos까지 갔는데 SD가 감지 않된다, stacker를 Up한 후 SD센서를 본다
		ReadyPosStep[n_StackerPos] = 2100;
		break;

	case 2100:
		COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ReadyPosStep[n_StackerPos] = 2200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ReadyPosStep[n_StackerPos] = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1479, st_work.mn_run_status, COMI.mc_alarmcode);
			ReadyPosStep[n_StackerPos] = 2000;
		}
		break;

	case 2200:	
		if(n_StackerPos == 0)
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_tray_chk, IO_OFF);
		}
		else
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_chk, IO_OFF);
		}
		//nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_tray_chk + (n_StackerPos*4), IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			ReadyPosStep[n_StackerPos] = 2300;
		}
		else// if(nRet_1 == IO_OFF)
		{
			ReadyPosStep[n_StackerPos] = 100;
		}
		break;

	case 2300:		
		st_sync.n_ld_righths_tray_supply[n_StackerPos] = CTL_NOTREADY;
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

int CRun_Stacker_Load_RightHeat::Set_RailClampOnOff(int n_onoff)
{
	int nFuncRet = CTL_GOOD;

	FAS_IO.set_out_bit(st_io.o_hs_bwd_clamp_onoff, n_onoff);
	mb_clampcylinder_flag = FALSE;
	ml_clampcylinder_Time[0] = GetCurrentTime();

	return nFuncRet;	
}

int CRun_Stacker_Load_RightHeat::Get_RailClampOnOff(int n_onoff)
{
	int nFuncRet = -1;
	
	if(COMI.mn_simulation_mode == 1) // 20130509
	{
		nFuncRet = n_onoff;
	}
	else
	{
		ml_clampcylinder_Time[1] = GetCurrentTime();
		
		if (mb_clampcylinder_flag != TRUE)
		{
			ml_clampcylinder_Time[0] = GetCurrentTime();
			mb_clampcylinder_flag = TRUE;
		}
		else if (FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_off_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_on_chk, IO_ON) == IO_ON)	// On 상태.
		{
			ml_clampcylinder_Time[2] = ml_clampcylinder_Time[1] - ml_clampcylinder_Time[0];
			
			if (ml_clampcylinder_Time[2] < 0)		
			{
				ml_clampcylinder_Time[0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
			{
				if (ml_clampcylinder_Time[2] > st_wait.n_limit_wait_time[20])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_clampcylinder_flag = FALSE;
				}
			}
			else if (ml_clampcylinder_Time[2] > st_wait.n_on_wait_time[20])
			{
				nFuncRet = CYLINDER_ON;
				mb_clampcylinder_flag = FALSE;
			}
		}
		else if (FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_off_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_on_chk, IO_OFF) == IO_OFF)	// Off 상태.
		{
			ml_clampcylinder_Time[2] = ml_clampcylinder_Time[1] - ml_clampcylinder_Time[0];
			
			if (ml_clampcylinder_Time[2] < 0)		
			{
				ml_clampcylinder_Time[0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
			{
				if (ml_clampcylinder_Time[2] > st_wait.n_limit_wait_time[20])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_clampcylinder_flag = FALSE;
				}
			}
			else if (ml_clampcylinder_Time[2] > st_wait.n_on_wait_time[20])
			{
				nFuncRet = CYLINDER_OFF;	
				mb_clampcylinder_flag = FALSE;
			}
		}
		else
		{
			ml_clampcylinder_Time[2] = ml_clampcylinder_Time[1] - ml_clampcylinder_Time[0];
			
			if (ml_clampcylinder_Time[2] < 0)		
			{
				ml_clampcylinder_Time[0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (ml_clampcylinder_Time[2] > st_wait.n_limit_wait_time[20])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_clampcylinder_flag = FALSE;
			}
		}
	}
	
	return nFuncRet;
}

int CRun_Stacker_Load_RightHeat::Set_StackerUpDnClinder(int nStacker, int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff == CYLINDER_OFF)
	{
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_tray_chk + (nStacker * 4), IO_OFF) == IO_ON)
		{
			//060100 0 06 "Right Heat Sink#l stacker에 트레이가 존재합니다.제거해 주세요."
			//070100 0 07 "Right Heat Sink#2 stacker에 트레이가 존재합니다.제거해 주세요."
			if(nStacker == 0)	sprintf(mc_jamcode,"060100");
			else                sprintf(mc_jamcode,"070100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1482, st_work.mn_run_status, mc_jamcode);
			nFuncRet = CYLINDER_ERROR;
		}
	}
	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker1_up_onoff + (nStacker * 2), n_onoff);
	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker1_dn_onoff + (nStacker * 2), !n_onoff);
	mb_updncylinder_flag[nStacker] = FALSE;
	ml_undncylinder_Time[nStacker][0] = GetCurrentTime();

	return nFuncRet;
}

int CRun_Stacker_Load_RightHeat::Get_StackerUpDnClinder(int nStacker, int n_onoff)
{
	int nFuncRet = -1;
	
	ml_undncylinder_Time[nStacker][1] = GetCurrentTime();
	
	if (mb_updncylinder_flag[nStacker] != TRUE)
	{
		ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
		mb_updncylinder_flag[nStacker] = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_up_chk + (nStacker * 4), IO_ON) == CTL_ON) && 
		(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_dn_chk + (nStacker * 4), IO_OFF) == CTL_OFF))	// On 상태.
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
	else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_up_chk + (nStacker * 4), IO_OFF) == CTL_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_dn_chk + (nStacker * 4), IO_ON) == CTL_ON))	// Off 상태.
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


int CRun_Stacker_Load_RightHeat::Set_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff == CYLINDER_OFF)
	{
	/*	if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk + (nStacker*3), IO_OFF) == IO_ON)
		{
			//060100 0 06 "Right Heat Sink#l stacker에 트레이가 존재합니다.제거해 주세요."
			sprintf(mc_jamcode,"060100");
			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			alarm.mn_alarm_assign_section = 1705;
			CTL_Lib.Alarm_Error_Occurrence(alarm.mn_count_mode, st_work.mn_run_status, mc_jamcode);
			nFuncRet = CYLINDER_ERROR;
		}*/
	}

	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker1_rail_fwd_onoff + (nStacker*2), n_onoff);
	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker1_rail_bwd_onoff + (nStacker*2), !n_onoff);
	mb_pushercylinder_flag[nStacker] = FALSE;
	ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();

	return nFuncRet;
}

int CRun_Stacker_Load_RightHeat::Get_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff)
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
				if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[21] + (nStacker*2))
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_pushercylinder_flag[nStacker] = FALSE;
				}
			}
			else if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_on_wait_time[21] + (nStacker*2))
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
				if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[21] + (nStacker*2))
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_pushercylinder_flag[nStacker] = FALSE;
				}
			}
			else if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_on_wait_time[21] + (nStacker*2))
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
			
			if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[21] + (nStacker*2))
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker] = FALSE;
			}
		}
	}
	
	return nFuncRet;
}

int CRun_Stacker_Load_RightHeat::LD_Righths_TrayMove()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet_1;

	Func.ThreadFunctionStepTrace(64, TrayMoveStep);
	switch(TrayMoveStep)
	{
	case 0:
//		if (FAS_IO.get_in_bit(st_io.i_m_stacker1_tray_chk, IO_ON) == IO_ON)
//		{
			TrayMoveStep = 1000;
//		}
//		else
//		{
//			//030101
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"[LeftHs TrayMove Error] Stacker1에 트레이가 존재하지 않습니다.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
//			alarm.mstr_pcode = _T("030101");
//			alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
//			alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 			
//			CtlBdFunc.Alarm_Error_Occurrence(alarm.mn_count_mode, alarm.mn_type_mode, st_work.mn_run_status, alarm.mstr_pcode);
//		}
		break;

	case 1000:
		TrayMoveStep = 2000;
		break;

	case 2000:
		if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_fwd_chk, IO_OFF) == IO_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_bwd_chk, IO_ON) == IO_ON))
		{
			mn_retry = 0;
			Set_PusherCylinderOnOff(CYLINDER_ON);
			TrayMoveStep = 2200;
		}
		else
		{
			//060107 0 06 "Right Heat Sink#l stacker 레일 backward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"060107");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1484, st_work.mn_run_status, mc_jamcode);
		}
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
				//060104 0 06 "Right Heat Sink#l stacker 트레이 pusher forward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"060104");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1485, st_work.mn_run_status, mc_jamcode);
				TrayMoveStep = 2100;			
			}
			else
			{
				TrayMoveStep = 2100;			
			}
		}
		break;

	case 2300:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_rail_tray_chk, IO_ON);
			
		if (st_basic.n_mode_device == 0)		nRet_1 = IO_ON;
		
		if(nRet_1 == IO_ON)
		{
			TrayMoveStep = 3000;
		}
		else
		{
			//060101 0 06 "Right Heat Sink#l stacker에 트레이가 존재지 않습니다."
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[RightHeat TrayMove Error] Stacker2에 트레이가 존재하지 않습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[RightHeat TrayMove Error] Stacker2 does not exist on the tray");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			sprintf(mc_jamcode,"060101");
			CTL_Lib.Alarm_Error_Occurrence(1486, CTL_dWARNING, mc_jamcode);
		}
		break;

	case 3000:
		nFuncRet = CTLBD_RET_GOOD;
		TrayMoveStep = 0;
		break;
	}

	return nFuncRet;
}

int CRun_Stacker_Load_RightHeat::LD_Righths_Seperate_Tray(int nOnOff)
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet_1;

	Func.ThreadFunctionStepTrace(67, TraySeperateStep);
	switch(TraySeperateStep)
	{
	case 0:
		if(nOnOff == IO_OFF)
		{
			if (FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_off_chk, !nOnOff) == !nOnOff && 
				FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_on_chk, nOnOff) == nOnOff)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_clamp_up_chk, !nOnOff) == !nOnOff && 
					FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_clamp_dn_chk, nOnOff) == nOnOff)
				{
					TraySeperateStep = 2000;
				}
				else
				{
					mn_retry = 0;
					Set_PusherUpDnCylinderOnOff(nOnOff);
					TraySeperateStep = 1000;
				}
			}
			else
			{
				//1.다운 후 clamp를 오무린다
				mn_retry = 0;
				Set_PusherUpDnCylinderOnOff(!nOnOff);
				TraySeperateStep = 20;				
			}
		}
		else
		{
			if (FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_off_chk, nOnOff) == nOnOff && 
				FAS_IO.get_in_bit(st_io.i_hs_bwd_clamp_on_chk, !nOnOff) == !nOnOff)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_clamp_up_chk, !nOnOff) == !nOnOff && 
					FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_clamp_dn_chk, nOnOff) == nOnOff)
				{
					TraySeperateStep = 2000;
				}
				else
				{
					mn_retry = 0;
					Set_PusherUpDnCylinderOnOff(nOnOff);
					TraySeperateStep = 1000;
				}
			}
			else
			{
				mn_retry = 0;
				Set_RailClampOnOff(!nOnOff);
				TraySeperateStep = 400;
			}
		}
		break;

	case 10:
		Set_PusherUpDnCylinderOnOff(!nOnOff);
		TraySeperateStep = 20;
		break;

	case 20:
		nRet_1 = Get_PusherUpDnCylinderOnOff(!nOnOff);
		if (nRet_1 == !nOnOff)
		{
			TraySeperateStep = 30;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//070108 0 07 "Left Heat Sink#l stacker 트레이 pusher clamp 업이 동작을 하지 않습니다."
				//070109 0 07 "Left Heat Sink#l stacker 트레이 pusher clamp 다운이 동작을 하지 않습니다."
				if(nOnOff == CYLINDER_OFF)	sprintf(mc_jamcode,"070108");
				else                        sprintf(mc_jamcode,"070109");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1397, st_work.mn_run_status, mc_jamcode);
				TraySeperateStep = 10;
			}
			else
			{
				TraySeperateStep = 10;
			}
		}
		break;

	case 30:
		mn_retry = 0;
		Set_RailClampOnOff(nOnOff);
		TraySeperateStep = 200;
		break;

	case 100:
		Set_RailClampOnOff(nOnOff);
		TraySeperateStep = 200;
		break;

	case 200:
		nRet_1 = Get_RailClampOnOff(nOnOff);
		if(nRet_1 == nOnOff)
		{
			TraySeperateStep = 0;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			Set_RailClampOnOff(nOnOff);
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
//				060500 0 03 "Right Heat Sink#l Stacker 레일 Clamp Fwd가 동작이 되지 않습니다."
//				060501 0 03 "Right Heat Sink#l Stacker 레일 Clamp Bwd가 동작이 되지 않습니다."
				if(nOnOff == CYLINDER_OFF)	sprintf(mc_jamcode,"060500");
				else                        sprintf(mc_jamcode,"060501");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1391, st_work.mn_run_status, mc_jamcode);
				TraySeperateStep = 100;
			}
			else
			{
				TraySeperateStep = 100;
			}
		}
		break;	
		
	case 300:
		Set_RailClampOnOff(!nOnOff);
		TraySeperateStep = 400;
		break;

	case 400:
		nRet_1 = Get_RailClampOnOff(!nOnOff);
		if(nRet_1 == !nOnOff)
		{
			TraySeperateStep = 0;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			Set_RailClampOnOff(!nOnOff);
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
//				060500 0 03 "Right Heat Sink#l Stacker 레일 Clamp Fwd가 동작이 되지 않습니다."
//				060501 0 03 "Right Heat Sink#l Stacker 레일 Clamp Bwd가 동작이 되지 않습니다."
				if(nOnOff == CYLINDER_OFF)	sprintf(mc_jamcode,"060500");
				else                        sprintf(mc_jamcode,"060501");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1392, st_work.mn_run_status, mc_jamcode);
				TraySeperateStep = 300;
			}
			else
			{
				TraySeperateStep = 300;
			}
		}
		break;	
	
	case 900:
		Set_PusherUpDnCylinderOnOff(nOnOff);
		TraySeperateStep = 1000;
		break;

	case 1000:
		nRet_1 = Get_PusherUpDnCylinderOnOff(nOnOff);
		if (nRet_1 == nOnOff)
		{
			TraySeperateStep = 2000;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
//				060108 0 06 "Right Heat Sink#l stacker 업이 동작을 하지 않습니다."
//				060109 0 06 "Right Heat Sink#l stacker 다운이 동작을 하지 않습니다."
				if(nOnOff == CYLINDER_OFF)	sprintf(mc_jamcode,"060108");
				else                        sprintf(mc_jamcode,"060109");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1380, st_work.mn_run_status, mc_jamcode);
				TraySeperateStep = 900;
			}
			else
			{
				TraySeperateStep = 900;
			}
		}
		break;

	case 2000:
		if(nOnOff == IO_OFF)
		{
			nFuncRet = CTLBD_RET_GOOD;
			TraySeperateStep = 0;
		}
		else
		{
			TraySeperateStep = 2100;
		}
		break;

	case 2100:
		Set_RailClampOnOff(nOnOff);
		TraySeperateStep = 2200;
		break;

	case 2200:
		nRet_1 = Get_RailClampOnOff(nOnOff);
		if(nRet_1 == nOnOff)
		{
			mn_retry = 0;
			Set_PusherUpDnCylinderOnOff(!nOnOff);
			TraySeperateStep = 2400;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			Set_RailClampOnOff(!nOnOff);
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
//				060500 0 03 "Right Heat Sink#l Stacker 레일 Clamp Fwd가 동작이 되지 않습니다."
//				060501 0 03 "Right Heat Sink#l Stacker 레일 Clamp Bwd가 동작이 되지 않습니다."
				if(nOnOff == CYLINDER_OFF)	sprintf(mc_jamcode,"060500");
				else                        sprintf(mc_jamcode,"060501");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1393, st_work.mn_run_status, mc_jamcode);
				TraySeperateStep = 2100;
			}
			else
			{
				TraySeperateStep = 2100;
			}
		}
		break;

	case 2300:
		Set_PusherUpDnCylinderOnOff(!nOnOff);
		TraySeperateStep = 2400;
		break;

	case 2400:
		nRet_1 = Get_PusherUpDnCylinderOnOff(!nOnOff);
		if (nRet_1 == !nOnOff)
		{
			nFuncRet = CTLBD_RET_GOOD;
			TraySeperateStep = 0;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//030108 0 03 "Left Heat Sink#l stacker 트레이 pusher clamp 업이 동작을 하지 않습니다."
				//030109 0 03 "Left Heat Sink#l stacker 트레이 pusher clamp 다운이 동작을 하지 않습니다."
				if(nOnOff == CYLINDER_OFF)	sprintf(mc_jamcode,"030108");
				else                        sprintf(mc_jamcode,"030109");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1399, st_work.mn_run_status, mc_jamcode);
				TraySeperateStep = 2300;
			}
			else
			{
				TraySeperateStep = 2300;
			}
		}
		break;
	}
	
	return nFuncRet;	
}

int CRun_Stacker_Load_RightHeat::Set_PusherCylinderOnOff(int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff == CYLINDER_OFF)
	{
		if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		{
			//060100 0 06 "Right Heat Sink#l stacker에 트레이가 존재합니다.제거해 주세요."
			sprintf(mc_jamcode,"060101");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1487, st_work.mn_run_status, mc_jamcode);
			nFuncRet = CYLINDER_ERROR;
		}
	}

	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker_tray_pusher_fwd_onoff, !n_onoff);
	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker_tray_pusher_bwd_onoff, n_onoff);
	mb_cylinder_flag = FALSE;
	ml_cylinder_Time[0] = GetCurrentTime();

	return nFuncRet;	
}

int CRun_Stacker_Load_RightHeat::Get_PusherCylinderOnOff(int n_onoff)
{
	int nFuncRet = -1;

	if(COMI.mn_simulation_mode == 1) // 20130509
	{
		nFuncRet = n_onoff;
	}
	else
	{
		ml_cylinder_Time[1] = GetCurrentTime();
		
		if (mb_cylinder_flag != TRUE)
		{
			ml_cylinder_Time[0] = GetCurrentTime();
			mb_cylinder_flag = TRUE;
		}
		else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_fwd_chk, IO_ON) == CTL_ON) && 
			(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_bwd_chk, IO_ON) == CTL_OFF))	// On 상태.
		{
			ml_cylinder_Time[2] = ml_cylinder_Time[1] - ml_cylinder_Time[0];
			
			if (ml_cylinder_Time[2] < 0)		
			{
				ml_cylinder_Time[0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
			{
				if (ml_cylinder_Time[2] > st_wait.n_limit_wait_time[18])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_cylinder_flag = FALSE;
				}
			}
			else if (ml_cylinder_Time[2] > st_wait.n_on_wait_time[18])
			{
				nFuncRet = CYLINDER_ON;
				mb_cylinder_flag = FALSE;
			}
		}
		else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_fwd_chk, IO_OFF) == CTL_OFF) &&
			(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_bwd_chk, IO_ON) == CTL_ON))	// Off 상태.
		{
			ml_cylinder_Time[2] = ml_cylinder_Time[1] - ml_cylinder_Time[0];
			
			if (ml_cylinder_Time[2] < 0)		
			{
				ml_cylinder_Time[0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
			{
				if (ml_cylinder_Time[2] > st_wait.n_limit_wait_time[18])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_cylinder_flag = FALSE;
				}
			}
			else if (ml_cylinder_Time[2] > st_wait.n_on_wait_time[18])
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
			
			if (ml_cylinder_Time[2] > st_wait.n_limit_wait_time[18])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_cylinder_flag = FALSE;
			}
		}
	}
	
	return nFuncRet;
}

int CRun_Stacker_Load_RightHeat::LD_Righths_TrayBack()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet_1;

	Func.ThreadFunctionStepTrace(65, TrayBackStep);
	switch(TrayBackStep)
	{
	case 0:
		TrayBackStep = 1000;
		break;

	case 1000:
		if (FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_clamp_up_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_clamp_dn_chk, IO_OFF) == IO_OFF)
		{
			TrayBackStep = 1100;
		}
		else
		{
			mn_retry = 0;
			Set_PusherUpDnCylinderOnOff(CYLINDER_ON);
			TrayBackStep = 1020;
		}
		break;

	case 1010:
		nRet_1 = Set_PusherUpDnCylinderOnOff(CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			TrayBackStep = 1020;
		}
		break;

	case 1020:
		nRet_1 = Get_PusherUpDnCylinderOnOff(CYLINDER_ON);
		if (nRet_1 == CYLINDER_ON)
		{
			TrayBackStep = 1100;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//060108 0 06 "Right Heat Sink#l stacker 업이 동작을 하지 않습니다."
				sprintf(mc_jamcode,"060108");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1488, st_work.mn_run_status, mc_jamcode);
				TrayBackStep = 1010;
			}
			else
			{
				TrayBackStep = 1010;
			}
		}
		break;

	case 1100:
		if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_fwd_chk, IO_OFF) == IO_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_bwd_chk, IO_ON) == IO_ON))
		{
			TrayBackStep = 2000;
		}
		else
		{
			mn_retry = 0;
			Set_PusherCylinderOnOff(CYLINDER_OFF);
			TrayBackStep = 1120;
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
			TrayBackStep = 1100;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				//060105 0 06 "Right Heat Sink#l stacker 트레이 pusher backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"060105");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1489, st_work.mn_run_status, mc_jamcode);
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


int CRun_Stacker_Load_RightHeat::Set_PusherUpDnCylinderOnOff(int n_onoff)
{
	int nFuncRet = CTL_GOOD;

	FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker_tray_pusher_clamp_updn, n_onoff);
	mb_pusherupdncylinder_flag = FALSE;
	ml_pusherupdncylinder_Time[0] = GetCurrentTime();

	return nFuncRet;
}

int CRun_Stacker_Load_RightHeat::Get_PusherUpDnCylinderOnOff(int n_onoff)
{
	int nFuncRet = -1;

	if(COMI.mn_simulation_mode == 1) // 20130509
	{
		nFuncRet = n_onoff; 
	}
	else
	{
		ml_pusherupdncylinder_Time[1] = GetCurrentTime();
		
		if (mb_pusherupdncylinder_flag != TRUE)
		{
			ml_pusherupdncylinder_Time[0] = GetCurrentTime();
			mb_pusherupdncylinder_flag = TRUE;
		}
		else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_clamp_up_chk, IO_OFF) == IO_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_clamp_dn_chk, IO_ON) == IO_ON))	// On 상태.
		{
			ml_pusherupdncylinder_Time[2] = ml_pusherupdncylinder_Time[1] - ml_pusherupdncylinder_Time[0];
			
			if (ml_pusherupdncylinder_Time[2] < 0)		
			{
				ml_pusherupdncylinder_Time[0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
			{
				if (ml_pusherupdncylinder_Time[2] > st_wait.n_limit_wait_time[20])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_pusherupdncylinder_flag = FALSE;
				}
			}
			else if (ml_pusherupdncylinder_Time[2] > st_wait.n_on_wait_time[20])
			{
				nFuncRet = CYLINDER_ON;
				mb_pusherupdncylinder_flag = FALSE;
			}
		}
		else if ((FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_clamp_up_chk, IO_ON) == IO_ON) && 
			(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker_tray_pusher_clamp_dn_chk, IO_OFF) == IO_OFF))	// Off 상태.
		{
			ml_pusherupdncylinder_Time[2] = ml_pusherupdncylinder_Time[1] - ml_pusherupdncylinder_Time[0];
			
			if (ml_pusherupdncylinder_Time[2] < 0)		
			{
				ml_pusherupdncylinder_Time[0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
			{
				if (ml_pusherupdncylinder_Time[2] > st_wait.n_limit_wait_time[20])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_pusherupdncylinder_flag = FALSE;
				}
			}
			else if (ml_pusherupdncylinder_Time[2] > st_wait.n_on_wait_time[20])
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
			
			if (ml_pusherupdncylinder_Time[2] > st_wait.n_limit_wait_time[20])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pusherupdncylinder_flag = FALSE;
			}
		}
	}
	
	return nFuncRet;
}
