// Run_Stacker_Sorter.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Stacker_Sorter.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Function.h"
#include "ComizoaPublic.h"
#include "CtlBd_Library.h"
#include "Cmmsdk.h"

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

const int M_STACKER_1 = 0;
const int M_STACKER_2 = 1;
/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Sorter
CRun_Stacker_Sorter Run_Stacker_Sorter;
CRun_Stacker_Sorter::CRun_Stacker_Sorter()
{
	Thread_Variable_Initial();
}

CRun_Stacker_Sorter::~CRun_Stacker_Sorter()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Sorter message handlers
void CRun_Stacker_Sorter::Thread_Run()
{
 //	COMI.Set_Motor_IO_Property(M_ULDM_STACKER_1, cmSD_LOGIC, cmTRUE);
 //	COMI.Set_Motor_IO_Property(M_ULDM_STACKER_1, cmSD_MODE, cmTRUE);
//	COMI.Set_Motor_IO_Property(M_ULDM_STACKER_1, cmSD_EN, cmTRUE);//cmTRUE);
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

		COMI.Set_MotStop(0, M_ULDM_STACKER_1);
		CTL_Lib.mn_single_motmove_step[M_ULDM_STACKER_1] = 0;
		COMI.Set_MotStop(0, M_ULDM_STACKER_2);
		CTL_Lib.mn_single_motmove_step[M_ULDM_STACKER_2] = 0;
		InitStep = 0;
		ReinstateStep = 0;
		break;
	}
}

void CRun_Stacker_Sorter::OnSorterStacker_FinalPos()//20121126
{
	if(st_work.mn_reinstatement_mode[M_ULDM_STACKER_1] == 0) //stop시 최초 한번만 기록하기 위함
	{
		st_work.d_reinstate_pos[0][M_ULDM_STACKER_1] = COMI.md_cmdpos_backup[M_ULDM_STACKER_1];
		st_work.d_reinstate_pos[0][M_ULDM_STACKER_2] = COMI.md_cmdpos_backup[M_ULDM_STACKER_2];
		st_work.d_reinstate_pos[0][M_ULDM_STACKER_MOVE] = COMI.md_cmdpos_backup[M_ULDM_STACKER_MOVE];

		st_work.mn_PickerUpDnStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_updn_onoff, IO_OFF);
		st_work.mn_FingerStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_gripper_onoff, IO_OFF);
		st_work.mn_TrayClampStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_stacker2_tray_clamp_onoff, IO_OFF);
		
		st_work.mn_reinstatement_mode[M_ULDM_STACKER_1] = 1;	//최종 위치를 기록 했다
	}
}
void CRun_Stacker_Sorter::Run_Init()
{
	int i=0, j=0, nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	double dCurrentPos[3] = {0,};
	int ratio = 60;

	if(st_handler.mn_init_state[INIT_ULD_STACKER] != CTL_NO)	return;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_ULDM_STACKER_1);
		CTL_Lib.mn_single_motmove_step[M_ULDM_STACKER_1] = 0;
		COMI.Set_MotStop(0, M_ULDM_STACKER_2);
		CTL_Lib.mn_single_motmove_step[M_ULDM_STACKER_2] = 0;
		COMI.Set_MotStop(0, M_ULDM_STACKER_MOVE);
		CTL_Lib.mn_single_motmove_step[M_ULDM_STACKER_MOVE] = 0;
		ReadyPosStep[0] = ReadyPosStep[1] = 0;
		COMI.mn_home_step[M_ULDM_STACKER_1] = 0;
		COMI.mn_home_step[M_ULDM_STACKER_2] = 0;
		COMI.mn_home_step[M_ULDM_STACKER_MOVE] = 0;
		return;
	}
	dCurrentPos[0] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_1);
	dCurrentPos[1] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_2);
	dCurrentPos[2] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_MOVE);

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
		if(COMI.mn_simulation_mode == 1)
		{
			InitStep = 10;
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
			{//090202 0 09 "Unload Stacker#1 레일에 트레이가 있습니다.
				sprintf(mc_jamcode,"090202");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1600, st_work.mn_run_status, mc_jamcode);
			}
			else if(FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_OFF) == IO_ON)
			{//100202 0 10 "Unload Stacker#2 레일에 트레이가 있습니다."
				sprintf(mc_jamcode,"100202");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1601, st_work.mn_run_status, mc_jamcode);
			}//2015.0126
			else if(FAS_IO.get_in_bit(st_io.i_uld_ld_slide_tray_chk, IO_OFF) == IO_ON)
			{
				//100100 1 10 "Sorter Stacker#2에 트레이가 존재합니다.제거해 주세요."
				sprintf(mc_jamcode, "100601"); 
				CTL_Lib.Alarm_Error_Occurrence(1623, st_work.mn_run_status, mc_jamcode);	
			}
			else
			{
				InitStep = 10;
			}
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
				//100201 0 10 "Unload Stacker#2 레일에 트레이가 존재하지 않습니다."
				sprintf(mc_jamcode, "100201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1602, st_work.mn_run_status, mc_jamcode);
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
			//090204 0 09 "Unload Stacker#1 레일 backward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"090204");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1603, st_work.mn_run_status, mc_jamcode);
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
			//100107 0 10 "Unload Stacker#2 레일 backward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"100107");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1604, st_work.mn_run_status, mc_jamcode);
		}
		break;
		
	case 1000:
		InitStep = 1100;
		break;
		
		if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		{//090100 1 09 "Sorter Stacker#1에 트레이가 존재합니다."
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Sorter Stacker#1에 트레이가 존재합니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[Initialize Error] Sorter Stacker # 1 on the tray");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			sprintf(mc_jamcode,"090100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1605,  st_work.mn_run_status, mc_jamcode);			
		}
		else if(FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_OFF) == IO_ON)
		{//100100 1 10 "Sorter Stacker#2에 트레이가 존재합니다."
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Sorter Stacker#2에 트레이가 존재합니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[Initialize Error] Sorter Stacker # 2 on the tray");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}
			sprintf(mc_jamcode,"100100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1606,  st_work.mn_run_status, mc_jamcode);			
		}
		else
		{
			InitStep = 1100;
		}
		break;

	case 1100:
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_unclamp_onoff, IO_ON);
		InitStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_stacker_guide_clamp_onoff, st_io.i_uld_stacker_guide_unclamp_off_chk, st_io.i_uld_stacker_guide_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[37], st_wait.n_limit_wait_time[37]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//090102 0 09 "Unload Stacker#1의 guide clamp가 On이 되지 않습니다."
			sprintf(mc_jamcode,"090102");
			CTL_Lib.Alarm_Error_Occurrence(1607,  CTL_dWARNING, mc_jamcode);			
		}
		break;

	case 1300:
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker_tray_pusher_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker_tray_pusher_bwd_onoff, IO_ON);
		InitStep = 1400;
		break;

	case 1400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_stacker_tray_pusher_fwd_onoff, st_io.i_uld_stacker_tray_pusher_bwd_chk, st_io.i_uld_stacker_tray_pusher_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[30], st_wait.n_limit_wait_time[30]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//090105 0 09 "Unload stacker 트레이 pusher backward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"090105");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1608, st_work.mn_run_status, mc_jamcode);			
		}
		break;

	case 1500:
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker1_rail_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker1_rail_bwd_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker2_rail_fwd_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker2_rail_bwd_onoff, IO_ON);
		InitStep = 1600;
		break;

	case 1600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_stacker1_rail_bwd_onoff, st_io.i_uld_stacker1_rail_bwd_chk, st_io.i_uld_stacker1_rail_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[34], st_wait.n_limit_wait_time[34]);
		nRet_2 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_stacker2_rail_bwd_onoff, st_io.i_uld_stacker2_rail_bwd_chk, st_io.i_uld_stacker2_rail_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[36], st_wait.n_limit_wait_time[36]);
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			InitStep = 1700;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//090106 0 09 "Unload Stacker#1 레일 forward가 동작이 되지 않습니다."
		//090107 0 09 "Unload Stacker#1 레일 backward가 동작이 되지 않습니다."
			if(nRet_1 != RET_PROCEED) sprintf(mc_jamcode,"090107");
			else                      sprintf(mc_jamcode,"100107");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1609,  st_work.mn_run_status, mc_jamcode);			
		}
		break;

	case 1700:
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker1_up_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker1_dn_onoff, IO_ON);
		//FAS_IO.OnCylinderAction(st_io.o_uld_stacker2_up_onoff, IO_OFF);
		//FAS_IO.OnCylinderAction(st_io.o_uld_stacker2_dn_onoff, IO_ON);
		InitStep = 1800;
		break;

	case 1800:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_stacker1_dn_onoff, st_io.i_uld_stacker1_dn_chk, st_io.i_uld_stacker1_up_chk,
			IO_ON, st_wait.n_on_wait_time[29], st_wait.n_limit_wait_time[29]);
		nRet_2 = RET_GOOD;
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{
			InitStep = 1900;
		}
		else if(nRet_1 != RET_PROCEED && nRet_2 != RET_PROCEED)
		{//090108 0 09 "Unload Stacker#1 업이 동작을 하지 않습니다."
		//090109 0 09 "Unload Stacker#1 다운이 동작을 하지 않습니다."

			if(nRet_1 != RET_PROCEED) sprintf(mc_jamcode,"090109");
			else                      sprintf(mc_jamcode,"100109");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1610,  st_work.mn_run_status, mc_jamcode);			
			InitStep = 1700;
		}
		break;

	case 1900:
		if(initial_smove == YES)
		{
			if(COMI.Get_MotIOSensor(M_ULDM_STACKER_MOVE, MOT_SENS_ELP) == CTLBD_RET_GOOD)
			{
				InitStep = 2300;
			}
			else if(COMI.Get_MotIOSensor(M_ULDM_STACKER_MOVE, MOT_SENS_ELM) == CTLBD_RET_GOOD)
			{
				InitStep = 2100;
			}
			else//2016.0524
			{
				InitStep = 2000;
			}
		}
		else
		{
			InitStep = 2000;
		}
		break;

	case 2000:
		if(dCurrentPos[2] < st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS])//stacker move
		{
			InitStep = 2010;
		}
		else if(dCurrentPos[2] > st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS] &&
			dCurrentPos[2] < st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS])
		{
			InitStep = 2010;
		}
		else //st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS];
		{
			InitStep = 2300;
		}
		break;

	case 2010:
		if(dCurrentPos[1] <= st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN])
		{
			InitStep = 2100;
		}
		else if(dCurrentPos[1] > st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN] && 
			dCurrentPos[1] < st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_UP] - st_motor[M_ULDM_STACKER_1].mn_allow)
		{
			InitStep = 2300;
		}
		else //dCurrentPos[1] > st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT]
		{
			InitStep = 2100;
		}
		break;

	case 2100:
		nRet_1 = COMI.HomeCheck_Mot(M_ULDM_STACKER_MOVE, st_motor[M_ULDM_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);
		if(nRet_1 == BD_GOOD)
		{
			InitStep = 2200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			InitStep = 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1611, st_work.mn_run_status, COMI.mc_alarmcode);			
		}
		break;

	case 2200:
		nRet_1 = CTL_Lib.Single_Move(M_ULDM_STACKER_MOVE, st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS], ratio);
		if(nRet_1 == BD_YES)
		{
			mn_retry = 0;
			init_stacker_move = YES;
			InitStep = 2300;
		}
		else if (nRet_1 == BD_RETRY)
		{
			InitStep = 2200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1612, st_work.mn_run_status, COMI.mc_alarmcode);			
		}
		break;

	case 2300:		
		nRet_1 = COMI.HomeCheck_Mot(M_ULDM_STACKER_1, st_motor[M_ULDM_STACKER_1].mn_homecheck_method, MOT_TIMEOUT);

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
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1613, st_work.mn_run_status, COMI.mc_alarmcode);			
				COMI.mn_home_step[M_ULDM_STACKER_1] = 0;
			}
			else if(mn_retry == 3)
			{
				COMI.mn_home_step[M_ULDM_STACKER_1] = 0;
			}
		}
		break;

	case 2400:
		nRet_1 = COMI.HomeCheck_Mot(M_ULDM_STACKER_2, st_motor[M_ULDM_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
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
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1614, st_work.mn_run_status, COMI.mc_alarmcode);			
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
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_unclamp_onoff, IO_ON);
		InitStep = 2700;
		break;

	case 2700:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_stacker_guide_unclamp_onoff, st_io.i_uld_stacker_guide_unclamp_off_chk, st_io.i_uld_stacker_guide_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[37], st_wait.n_limit_wait_time[37]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 2800;
		}
		else if(nRet_1 != RET_PROCEED)
		{//090103 0 09 "Unload Stacker#1의 guide clamp가 Off가 되지 않습니다."
			sprintf(mc_jamcode,"090103");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1615, st_work.mn_run_status, mc_jamcode);			
		}
		break;

	case 2800:
		nRet_1 = CTL_Lib.Single_Move(M_ULDM_STACKER_2, st_motor[M_ULDM_STACKER_2].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 3000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2800;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1611, st_work.mn_run_status, COMI.mc_alarmcode);			
		}
		break;
		
	case 3000:
		if(st_work.mn_run_status == dLOTREADY)
		{
			InitStep = 4000;
		}
		else
		{
			st_handler.mn_init_state[INIT_ULD_STACKER] = CTL_YES;
			InitStep = 0;
		}
		break;
		
	case 4000:
		if (st_handler.mn_init_state[INIT_LD_STACKER] == CTL_YES)
		{
			st_handler.mn_init_state[INIT_LD_STACKER] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_LEFTHS_STACKER] == CTL_YES)
		{
			st_handler.mn_init_state[INIT_LEFTHS_STACKER] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_RIGHTHS_STACKER] == CTL_YES)
		{
			st_handler.mn_init_state[INIT_RIGHTHS_STACKER] = CTL_READY;
		}
		//if (st_handler.mn_init_state[INIT_ULD_STACKER] == CTL_YES)
		//{
			st_handler.mn_init_state[INIT_ULD_STACKER] = CTL_READY;
		//}
		if (st_handler.mn_init_state[INIT_CLIP_STACKER] == CTL_YES)//2014.0519
		{
			st_handler.mn_init_state[INIT_CLIP_STACKER] = CTL_READY;
		}
//		if (st_handler.mn_init_state[INIT_RBT_MODULE] == CTL_YES)
//		{
			st_handler.mn_init_state[INIT_RBT_MODULE] = CTL_READY;
//		}
//		if (st_handler.mn_init_state[INIT_RBT_LEFT_SINK] == CTL_YES)
//		{
			st_handler.mn_init_state[INIT_RBT_LEFT_SINK] = CTL_READY;
//		}
//		if (st_handler.mn_init_state[INIT_RBT_RIGHT_SINK] == CTL_YES)
//		{
			st_handler.mn_init_state[INIT_RBT_RIGHT_SINK] = CTL_READY;
//		}
//		if (st_handler.mn_init_state[INIT_RBT_CLIP_MODULE] == CTL_YES)
//		{
			st_handler.mn_init_state[INIT_RBT_CLIP_MODULE] = CTL_READY;
//		}
		if (st_handler.mn_init_state[INIT_RBT_SORTER] == CTL_YES)
		{
			st_sync.n_lotend_module_uldmovingtray_site = CTL_NO;
			st_handler.mn_init_state[INIT_RBT_SORTER] = CTL_READY;
		}
		if (st_handler.mn_init_state[INIT_LAVEL_VISION] == CTL_YES)
		{
			st_handler.mn_init_state[INIT_LAVEL_VISION] = CTL_READY;
		}		
//		if (st_handler.mn_init_state[INIT_WORK_BUFFER] == CTL_YES)
//		{
			st_handler.mn_init_state[INIT_WORK_BUFFER] = CTL_READY;
//		}
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
			InitStep = 4100;
 		}
		st_handler.mn_init_state[INIT_ULD_STACKER] = CTL_NO;
		break;

	case 4100:
		Run_Rbt_Load_LeftHeat.RunStep = -1;
		Run_Rbt_Load_Module.RunStep = -1;
		Run_Rbt_Load_RightHeat.RunStep = -1;
		Run_Rbt_Sorter.RunSortStep = -1;
		Run_Rbt_VisionLabel.Run_labelStep = -1;
		Run_Rbt_Work_Buffer.RunBuff1Step = -1;
		Run_Rbt_Load_Clip.RunInsertStep = -1;
		Run_Stacker_Load_Clip.RunStep = -1;
		Run_Stacker_Load_LeftHeat.RunTransStep = -1;
		Run_Stacker_Load_RightHeat.RunTransStep = -1;
		Run_Stacker_Load_Module.RunTransStep = -1;
		RunTransStep = -1;
		InitStep = 4200;

		st_handler.n_lotend_ready = CTL_NO;
		::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_LOTEND_READY, 1);
		break;

	case 4200:
		if(st_handler.n_lotend_ready == 11)
		{
			for (i = 0; i < 4; i++)
			{
				st_work.n_loadlot_count[i] = 0;
			}

			st_handler.mn_menu_lock = FALSE;

			st_work.i_sort_good_cnt = 0;
			st_work.i_sort_reject_cnt = 0;

			st_handler.mb_bcr_read = FALSE;
			st_handler.mn_jobComplete = NO;
			st_handler.mn_receive = NO;
			st_handler.nBootFlagCheck = CTL_NO;

			st_handler.n_more_lefths = CTL_NO;
			st_handler.n_more_righths = CTL_NO;
			st_handler.n_more_uld_tray = CTL_NO;

			st_work.n_lotend = CTL_NO;
			st_work.n_eqp_lotend = CTL_NO;

			st_work.nMdlInputCount[0][0] = 0;
			st_work.nMdlPassCount[0][0] = 0;
			st_work.nMdlRejectCount[0][0] = 0;

			for (i = 0; i < st_traybuffer[UNLOADER_SITE].i_loader_row; i++)
			{
				st_modulemap.UnloadTray[0][i] = DVC_NO;
				st_modulemap.UnloadTray[1][i] = DVC_NO;
			}	
			for (i = 0; i < st_traybuffer[REJECT_SITE].i_loader_row; i++)
			{
				st_modulemap.RejectTray[i] = DVC_NO;
			}	
			if (st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_EQP_LOTEND, 0);					
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_SORTER_PICKER);			// 피커 정보 
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_UNLOADER_PICKER);			// 피커 정보 
				st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_UNLOADER_TRAY);			// 피커 정보 
				st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_REJECT_TRAY);			// 피커 정보 
			}
			
			//994008 0 99 "자재및 모듈을 투입해 주세요."
			sprintf(mc_jamcode,"994008");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1695, st_work.mn_run_status, mc_jamcode);			
			InitStep = 0;
		}
		break;
	}
}

void CRun_Stacker_Sorter::Robot_BackMovePosValCheck()//20121126
{
	//여기서 이전에 동작했던 위치를 다시 계산해야한다.
		st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_1] =  st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1];
		st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_2] =  st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2];
		st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_MOVE] =  st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE];
}

void CRun_Stacker_Sorter::Run_Reinstate()//20121126
{
	int Ret1 = 0, Ret2 = 0, i = 0, n_response = 0;
	int FuncRet = RET_PROCEED;
	int nRet1 = 0, nRet2 = 0;
	double dPosition[10];

	if (st_work.mn_UnloadStacker_Reinstatement_Ok == TRUE) return;//20121126
	
	if (st_work.mn_reinstatement_mode[UNLOADER_SITE] == 0) // 완전 초기화 후만 관리함 
	{	// 현재 위치 백업 
		st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1] =  COMI.md_cmdpos_backup[M_ULDM_STACKER_1];
		st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2] =  COMI.md_cmdpos_backup[M_ULDM_STACKER_2];
		st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] =  COMI.md_cmdpos_backup[M_ULDM_STACKER_MOVE];

		st_work.mn_PickerUpDnStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_updn_onoff, IO_OFF);
		st_work.mn_FingerStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_gripper_onoff, IO_OFF);
		st_work.mn_TrayClampStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_stacker2_tray_clamp_onoff, IO_OFF);

		st_work.mn_reinstatement_mode[UNLOADER_SITE] = 1; //한번 데이터를 저장한다 
		ReinstateStep = 0;

	}	 

	switch (ReinstateStep)
	{
	case 0:
		ReinstateStep = 1000;
		break;

	case 1000://현재 위치 확인
		dPosition[0] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_1);
		dPosition[1] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_2);
		dPosition[2] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_MOVE);

		for (i = 0; i < 3; i++)
		{
			mn_change[UNLOADER_SITE][i] = 0;
		}
	
		//변경된 위치값 출력
		if (dPosition[0] >  st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1] + COMI.mn_allow_value[M_ULDM_STACKER_1]
			|| dPosition[0] < st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1] - COMI.mn_allow_value[M_ULDM_STACKER_1])
		{
			mn_change[UNLOADER_SITE][0]++;
			sprintf(st_msg.c_abnormal_msg, "Unloader Stacker#1 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1], (long)dPosition[0], (long)COMI.mn_allow_value[M_ULDM_STACKER_1]);
		}
		if (dPosition[1] > st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2] + COMI.mn_allow_value[M_ULDM_STACKER_2]
			|| dPosition[1] < st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2] - COMI.mn_allow_value[M_ULDM_STACKER_2])
		{
			mn_change[UNLOADER_SITE][1]++;
			sprintf(st_msg.c_abnormal_msg, "Unloader Stacker#2 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2], (long)dPosition[1], (long)COMI.mn_allow_value[M_ULDM_STACKER_2]);
		}
		if (dPosition[2] > st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] + COMI.mn_allow_value[M_ULDM_STACKER_MOVE]
			|| dPosition[2] < st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] - COMI.mn_allow_value[M_ULDM_STACKER_MOVE])
		{
			mn_change[UNLOADER_SITE][2]++;
			sprintf(st_msg.c_abnormal_msg, "Unloader Stacker Move POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE], (long)dPosition[2], (long)COMI.mn_allow_value[M_ULDM_STACKER_MOVE]);
		}

		st_msg.mstr_event_msg[0] = "Recovery Mode.......";
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);

		if(mn_change[UNLOADER_SITE][0] > 0 || mn_change[UNLOADER_SITE][1] > 0) //stacker 1,2
		{
			ReinstateStep = 2000;
		}
		else if(mn_change[UNLOADER_SITE][2] > 0) // tray move
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
		
		COMI.Set_MotPower(M_ULDM_STACKER_1, TRUE);
		COMI.Set_MotPower(M_ULDM_STACKER_2, TRUE);
		COMI.Set_MotPower(M_ULDM_STACKER_MOVE, TRUE);
	
		mn_check[0] = VARIABLE_INIT;
		mn_check[1] = VARIABLE_INIT;

		if(mn_change[UNLOADER_SITE][0] > 0 || mn_change[UNLOADER_SITE][1] > 0) //stacker 1,2
		{
			ReinstateStep = 2010;
		}
		else if(mn_change[UNLOADER_SITE][2] > 0)// tray move
		{
			ReinstateStep = 2100;
		}
		else
		{
			ReinstateStep = 1100;
		}
		break;

	case 2010: //stacker move 축 확인

		if (st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] + COMI.mn_allow_value[M_ULDM_STACKER_MOVE] < st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS]
		|| st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] - COMI.mn_allow_value[M_ULDM_STACKER_MOVE] < st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS])
		{
			ReinstateStep = 2100;
		}
		else
		{
			ReinstateStep = 2020;
		}
		break;

	case 2020: //stacker move 축 확인

		if (st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] + COMI.mn_allow_value[M_ULDM_STACKER_MOVE] >= st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS]
		|| st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] - COMI.mn_allow_value[M_ULDM_STACKER_MOVE] >= st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS])
		{
			ReinstateStep = 2100;
		}
		else
		{
			ReinstateStep = 2030;
		}
		break;

	case 2030://stacker 높이 강제고정 1 : 500, 2 : 600
		if (st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1] + COMI.mn_allow_value[M_ULDM_STACKER_1] >= 500   
			|| st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1] - COMI.mn_allow_value[M_ULDM_STACKER_1] >= 500)
		{
			ReinstateStep = 2050;
			
		}
		else
		{
			ReinstateStep = 2040;
		}
		break;
	case 2040:
		if (st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1] + COMI.mn_allow_value[M_ULDM_STACKER_1] <= st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN]   
			|| st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1] - COMI.mn_allow_value[M_ULDM_STACKER_1] <= st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN])
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
		if (st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2] + COMI.mn_allow_value[M_ULDM_STACKER_2] > 600   
			|| st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2] - COMI.mn_allow_value[M_ULDM_STACKER_2] > 600)
		{
			ReinstateStep = 2070;
			
		}
		else
		{
			ReinstateStep = 2060;
		}
		break;
	
	case 2060:
		if (st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2] + COMI.mn_allow_value[M_ULDM_STACKER_2] <= st_motor[M_ULDM_STACKER_2].md_pos[P_STOCKER_DOWN]   
			|| st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2] - COMI.mn_allow_value[M_ULDM_STACKER_2] <= st_motor[M_ULDM_STACKER_2].md_pos[P_STOCKER_DOWN])
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
		nRet1 = COMI.HomeCheck_Mot(M_ULDM_STACKER_MOVE, st_motor[M_ULDM_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet1 == BD_GOOD)
		{
			ReinstateStep = 2100;
		}
		else if (nRet1 == BD_RETRY)
		{
			ReinstateStep = 2070;
		}
		else if (nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5003, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;
		

	case 2100:
		nRet1 = COMI.HomeCheck_Mot(M_ULDM_STACKER_1, st_motor[M_ULDM_STACKER_1].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 2110;
		}		
		else if (nRet1 == BD_RETRY)
		{
			ReinstateStep = 2100;
		}
		else if (nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5001, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2110:
		nRet1 = COMI.HomeCheck_Mot(M_ULDM_STACKER_2, st_motor[M_ULDM_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet1 == BD_GOOD)
		{
			ReinstateStep = 2120;
		}		
		else if (nRet1 == BD_RETRY)
		{
			ReinstateStep = 2110;
		}
		else if (nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5002, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2120:
		nRet1 = COMI.HomeCheck_Mot(M_ULDM_STACKER_MOVE, st_motor[M_ULDM_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet1 == BD_GOOD)
		{
			ReinstateStep = 2130;
		}		
		else if (nRet1 == BD_RETRY)
		{
			ReinstateStep = 2120;
		}
		else if (nRet1 == BD_ERROR || nRet1 == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5003, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2130:
		if((st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] > (st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS] + st_motor[M_ULDM_STACKER_MOVE].mn_allow)) ||
			(st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] < (st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] - st_motor[M_ULDM_STACKER_MOVE].mn_allow)))
		{
			ReinstateStep = 2140;
		}
		else
		{
			ReinstateStep = 2150;
		}
		break;

	case 2140:
		nRet1 = CTL_Lib.Single_Move(M_ULDM_STACKER_MOVE, st_motor[M_ULDM_STACKER_MOVE].md_pos[st_work.mn_back_site[M_ULDM_STACKER_MOVE]], st_basic.nManualSpeed);
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
		nRet1 = CTL_Lib.Single_Move(M_ULDM_STACKER_MOVE, st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE], st_basic.nManualSpeed);
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
		nRet1 = CTL_Lib.Single_Move(M_ULDM_STACKER_1, st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1], MOT_TIMEOUT);		
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
		nRet1 = CTL_Lib.Single_Move(M_ULDM_STACKER_2, st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2], MOT_TIMEOUT);		
		if (nRet1 == BD_GOOD)
		{
			ReinstateStep = 50000;
			mn_change[UNLOADER_SITE][0] = 0;
			mn_change[UNLOADER_SITE][1] = 0;
			mn_change[UNLOADER_SITE][2] = 0;
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
		st_work.mn_UnloadStacker_Reinstatement_Ok = TRUE;//20121126
		FuncRet = RET_GOOD;
		ReinstateStep = 0;
		break;
	}
}

void CRun_Stacker_Sorter::Thread_Variable_Initial()
{
	int i;
	RunStep = 0;
	MoveStep = 0;
	InitStep = 0;
	TrayBackStep = 0;
	TrayMoveStep = 0;
	ReinstateStep = 0;
	ReadyPosStep_old[0] = ReadyPosStep_old[1] = 0;
	ReadyPosStep[0] = ReadyPosStep[1] = 0;
	MotorMoveStep = 0;
	StackerReayStep = 0;
	RunTransStep = 0;
	TrayMoveStep = 0;
	TrayBackStep = 0;
	mn_retry = 0;
	mn_StackerTrayReady = NO;
	mn_stacker_move_flag = CTL_NO;
	mn_Moving_stacker = NO;
	init_stacker_move = NO;
	init_stacker_1 = NO;
	init_stacker_2 = NO;



	for (i = 0; i < st_traybuffer[UNLOADER_SITE].i_loader_row; i++)
	{
		st_modulemap.UnloadTray[0][i] = DVC_NO;
		st_modulemap.UnloadTray[1][i] = DVC_NO;
	}	
	for (i = 0; i < st_traybuffer[REJECT_SITE].i_loader_row; i++)
	{
		st_modulemap.RejectTray[i] = DVC_NO;
	}	


	st_sync.mn_uld_module_sortertray_supply[0] = st_sync.mn_uld_module_sortertray_supply[1] = CTL_CLEAR;
	ReadyPosStep[0] = ReadyPosStep[1] = 0;
	mn_stacker_updn_cyliner[M_STACKER_1] = mn_stacker_updn_cyliner[M_STACKER_2] = CYLINDER_OFF;
	
	st_sync.mn_ld_module_sortertray_change[0] = st_sync.mn_ld_module_sortertray_change[1] = CTL_NO;
	
	st_sync.n_lotend_unload_site = CTL_NO;
	st_sync.n_lotend_module_ldsorterstacker_site = CTL_NO;
	st_sync.n_lotend_module_uldsorterstacker_site = CTL_NO;
	st_sync.mn_ld_module_sortertray_change[0] = st_sync.mn_ld_module_sortertray_change[1] = CTL_NO;

	mn_LeakM_LotEnd[0] = mn_LeakM_LotEnd[1] = mn_LeakM_LotEnd[2] = mn_LeakM_LotEnd[3] = NO;
}

void CRun_Stacker_Sorter::Run_Transfer()
{
	int nRet = 0, nRet_1 = VARIABLE_INIT;
	int i=0,j=0;

	////2014,0408
	CString strTmp;
    COleDateTime mtime_cur;
	int n_cur_hour, n_cur_minute, n_cur_second;
	////
	
	Func.ThreadFunctionStepTrace(70, RunTransStep);
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
			if(st_sync.n_lotend_unload_site == CTL_YES)
			{
				st_sync.n_lotend_module_ldsortertray_site = CTL_YES;		// 일단 LOT END를 시키자.
				mn_LeakM_LotEnd[1] = NO;
			}
			if(mn_LeakM_LotEnd[1] == NO && st_sync.n_lotend_module_ldsortertray_site == CTL_NO)
			{
				RunTransStep = 100;
			}			
			else if(st_sync.n_lotend_unload_site == CTL_YES && st_sync.n_lotend_module_uldsorterstacker_site == CTL_YES
				&& st_sync.n_lotend_module_ldsorterstacker_site == CTL_YES && st_sync.n_lotend_righths_ldstacker_site == CTL_YES &&
				st_sync.n_lotend_righths_uldstacker_site == CTL_YES && st_sync.n_lotend_clip_ldtray_site == CTL_YES &&
				st_sync.n_lotend_module_ldstacker_site == CTL_YES && st_sync.n_lotend_module_uldstacker_site == CTL_YES &&
				st_sync.n_lotend_lefths_ldtray_site == CTL_YES && st_sync.n_lotend_lefths_uldstacker_site == CTL_YES &&
				st_sync.n_lotend_clip_ldstacker_site == CTL_YES && st_sync.n_lotend_module_movingtray_site == CTL_YES &&
				st_sync.n_lotend_lefths_movingtray_site == CTL_YES && st_sync.n_lotend_righths_movingtray_site == CTL_YES &&
				st_sync.n_lotend_clip_Lot == YES && st_sync.n_lotend_clipbuffer_ldrbt == CTL_YES)
			{
				::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_MYSQL_LOTALARM_WRITE, 0);
				st_work.n_eqp_lotend = CTL_YES;
				st_work.b_Barcode_Create_Btn_Use = TRUE;//2014,0411
				RunTransStep = 10;
			}
		}
		break;

	case 10:
		if(st_work.n_eqp_lotend == CTL_YES)
		{
			

			if (st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOT_RESTART, 0);
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_EQP_LOTEND, 0);
			}
			/////////////// 20130130
			if (st_sync.b_lotcancel_use == TRUE)
			{
				////2014,0305
				st_work.nMdlInputTotalCount  = st_work.nMdlInputTotalCount + st_work.nMdlInputCount[0][0];
				st_work.nMdlPassTotalCount   = st_work.nMdlPassTotalCount + st_work.nMdlPassCount[0][0];
				st_work.nMdlRejectTotalCount = st_work.nMdlRejectTotalCount + st_work.nMdlRejectCount[0][0];
				////

				st_sync.b_lotcancel_use = FALSE;
				sprintf(mc_jamcode,"999000");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1620, st_work.mn_run_status, mc_jamcode);
				st_handler.n_lotend_ready = CTL_NO;
			}
			else
			{
				////2014,0305
				st_work.nMdlInputTotalCount  = st_work.nMdlInputTotalCount + st_work.nMdlInputCount[0][0];
				st_work.nMdlPassTotalCount   = st_work.nMdlPassTotalCount + st_work.nMdlPassCount[0][0];
				st_work.nMdlRejectTotalCount = st_work.nMdlRejectTotalCount + st_work.nMdlRejectCount[0][0];
				////

				sprintf(mc_jamcode,"990000");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1620, st_work.mn_run_status, mc_jamcode);
				st_handler.n_lotend_ready = CTL_NO;

				//2015.0129
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					CString strLog;
					if(st_work.m_dLotStartTime <= 0) st_work.m_dLotStartTime = GetCurrentTime();
					int nSec = (GetCurrentTime() - st_work.m_dLotStartTime)/1000;
					if( nSec < 1) nSec = 1;
					int nUPH = st_work.nMdlInputCount[0][0] * 3600 / nSec;
					strLog.Format("[Lotend : %s] Total time; %d sec. Input:%d. Good:%d. Fail:%d. UPH:%d ea.\r\n", st_NW.mstr_lot_id[1], nSec,
						st_work.nMdlInputCount[0][0], st_work.nMdlPassCount[0][0], st_work.nMdlRejectCount[0][0], nUPH );
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 .출력 요청
					Func.On_LogFile_Add(LOG_TOTAL, strLog);
					Func.On_LogFile_Add(LOG_UPH, strLog);

				}

			}
			
			////2014,0408
			mtime_cur = COleDateTime::GetCurrentTime();
			n_cur_hour = mtime_cur.GetHour();
			n_cur_minute = mtime_cur.GetMinute();
			n_cur_second = mtime_cur.GetSecond();
			
			strTmp.Format("%02d:%02d:%02d", n_cur_hour, n_cur_minute, n_cur_second);
			st_work.mstr_End_time = strTmp;
			
			strTmp.Format( "%02d:%02d:%02d", st_handler.m_tRN.GetTotalHours(), st_handler.m_tRN.GetMinutes(), st_handler.m_tRN.GetSeconds() );
			st_work.mstr_Run_time = strTmp;

			Func.Excel_File_Read();
			Func.Excel_File_Save();
			
			st_work.mstr_Start_time = "";
			st_work.mstr_End_time = "";
			st_work.mstr_Run_time = "";
			st_handler.m_tRN = 0;
			////

			RunTransStep = 20;
		}
		break;

	case 20:
		if(st_work.n_multilot_start == CTL_YES)
		{
			st_work.n_multilot_start = CTL_NO;
			::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_LOTEND_READY, 0);
			RunTransStep = 30;
		}
		break;

	case 30:
		if(st_handler.n_lotend_ready == 10)
		{
			st_sync.n_lotend_module_ldstacker_site = CTL_NO;
			st_sync.n_lotend_module_uldstacker_site = CTL_NO;
			st_sync.n_lotend_module_movingtray_site = CTL_NO;
			st_sync.n_lotend_module_ldsortertray_site = CTL_NO;
			
			st_sync.n_lotend_righths_ldstacker_site = CTL_NO;
			st_sync.n_lotend_righths_ldtray_site = CTL_NO;
			st_sync.n_lotend_righths_uldstacker_site = CTL_NO;
			st_sync.n_lotend_righths_movingtray_site = CTL_NO;

			st_sync.n_lotend_lefths_ldstacker_site = CTL_NO;
			st_sync.n_lotend_lefths_ldtray_site = CTL_NO;
			st_sync.n_lotend_lefths_uldstacker_site = CTL_NO;
			st_sync.n_lotend_lefths_movingtray_site = CTL_NO;
			
			st_sync.n_lotend_clip_ldtray_site = CTL_NO;
			st_sync.n_lotend_clip_ldstacker_site = CTL_NO;

			st_sync.n_lotend_unload_site = CTL_NO;
			st_sync.n_lotend_module_ldsorterstacker_site = CTL_NO;
			st_sync.n_lotend_module_uldsorterstacker_site = CTL_NO;
			st_sync.n_lotend_module_uldmovingtray_site = CTL_NO;
			
			st_sync.n_lotend_clip_ldrbt = CTL_NO;
			st_sync.n_lotend_clipbuffer_ldrbt = CTL_NO;
			st_sync.n_lotend_clamp_ldrbt = CTL_NO;
			st_sync.n_lotend_clip_Lot = CTL_NO;

			st_sync.n_lotend_module_ldrbt = CTL_NO;
			st_sync.n_lotend_lefths_ldrbt = CTL_NO;
			st_sync.n_lotend_righths_ldrbt = CTL_NO;

			st_sync.n_lotend_sorter_site = CTL_NO;

			st_sync.n_lotend_module_vision[TOPBUFFER] = CTL_NO;
			st_sync.n_lotend_module_vision[BTMBUFFER] = CTL_NO;			

			st_handler.mn_init_state[INIT_LD_STACKER] = CTL_NO;
			st_handler.mn_init_state[INIT_LEFTHS_STACKER] = CTL_NO;
			st_handler.mn_init_state[INIT_RIGHTHS_STACKER] = CTL_NO;
			st_handler.mn_init_state[INIT_ULD_STACKER] = CTL_NO;
			st_handler.mn_init_state[INIT_CLIP_STACKER] = CTL_NO;
			st_handler.mn_init_state[INIT_RBT_SORTER] = CTL_NO;
			st_handler.mn_init_state[INIT_RBT_MODULE] = CTL_NO;
			st_handler.mn_init_state[INIT_RBT_LEFT_SINK] = CTL_NO;
			st_handler.mn_init_state[INIT_RBT_RIGHT_SINK] = CTL_NO;
			st_handler.mn_init_state[INIT_RBT_CLIP_MODULE] = CTL_NO;
			st_handler.mn_init_state[INIT_LAVEL_VISION] = CTL_NO;
			st_handler.mn_init_state[INIT_WORK_BUFFER] = CTL_NO;
			st_handler.mn_init_state[INIT_LD_CLIP_TRANSFER] = CTL_NO;
			st_work.mn_run_status = dLOTREADY;
		}
		break;

	case 100:
//		if(st_sync.n_lotend_module_uldtray_transfer == CTL_REQ)
//		{
//			st_sync.n_lotend_module_uldtray_transfer = CTL_READY;
//		}
		if(st_sync.n_lotend_module_ldsortertray_site == CTL_YES) return;
		if(st_sync.mn_uld_module_sortertray_supply[0] == CTL_READY)
		{
			RunTransStep = 1000;
		}
		else if (st_sync.mn_uld_module_sortertray_supply[0] == CTL_NOTREADY)		// 텅 비었을 경우에...
		{			
			RunTransStep = 10000;
		}
		break;

	case 1000:
		nRet_1 = ULD_Module_TrayBack();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 1100;
		}
		break;

	case 1100:
		if(st_sync.mn_uld_module_sortertray_supply[0] == CTL_READY)
		{
			st_sync.mn_uld_module_sortertray_supply[0] = CTL_CHANGE;
			RunTransStep = 1200;
		}
		else if (st_sync.mn_uld_module_sortertray_supply[0] == CTL_NOTREADY)		// 텅 비었을 경우에...
		{			
			RunTransStep = 10000;
		}
		break;

	case 1200:		
		if(st_sync.mn_uld_module_sortertray_supply[0] == CTL_LOCK)// 교체 완료
		{
			RunTransStep = 1300;
		}
		else if(st_sync.mn_uld_module_sortertray_supply[0] == CTL_NOTREADY || (COMI.mn_simulation_mode == 1 &&(st_work.nMdlInputCount[0][0] == st_work.nMdlPassCount[0][0]))) // 텅 비었을 경우에...
		{			
			RunTransStep = 10000;
		}
		break;

	case 1300:
		if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_ON) == IO_ON ||
			mn_stacker_updn_cyliner[M_STACKER_1] == CYLINDER_ON)
		{
			//for (i = 0; i < st_traybuffer[RIGHTSINK_SITE].i_loader_col; i++)
			//{
				for (j = 0; j < st_traybuffer[UNLOADER_SITE].i_loader_row; j++)
				{
					st_modulemap.UnloadTray[0][j] = DVC_NO;
				}
			//}
			st_sync.mn_ld_module_sortertray_change[0] = CTL_YES;
			RunTransStep = 2000;
		}
		else
		{//090101 0 09 "Unload Stacker#1의 트레이가 존재하지 않습니다."
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"[Unload Transfer] Stacker1에 트레이가 존재하지 않습니다.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
			sprintf(mc_jamcode,"090101");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1621, st_work.mn_run_status, mc_jamcode);
		}
		break;
		
	case 2000:
		if(st_sync.mn_uld_module_sortertray_supply[1] == CTL_READY)//교체 완료 또는 Tray 기다림
		{
			RunTransStep = 2100;
		}
		break;

	case 2100:
		nRet_1 = ULD_Module_TrayMove();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 2200;
		}
		break;

	case 2200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_ON);			
		if (st_basic.n_mode_device != 1)		nRet_1 = IO_ON;
		
		if(nRet_1 == IO_ON)
		{
			for (i = 0; i < st_traybuffer[UNLOADER_SITE].i_loader_row; i++)
			{
				st_modulemap.UnloadTray[1][i] = st_modulemap.UnloadTray[0][i]; 
				st_modulemap.UnloadTray[0][i] = NO;
			}
			st_sync.mn_ld_module_sortertray_change[0] = CTL_NO;
			st_sync.mn_ld_module_sortertray_change[1] = CTL_YES;  // 교체할 트레이가 있다
			if(st_handler.cwnd_main != NULL)
			{			
				st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_UNLOADER_TRAY);			// 피커 정보 
			}

			RunTransStep = 3000;
		}
		else
		{
			//100101 0 10 "Unload Stacker#2의 트레이가 존재하지 않습니다."
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"[Module Transfer] Stacker2에 트레이가 존재하지 않습니다.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
			sprintf(mc_jamcode,"100101");
			CTL_Lib.Alarm_Error_Occurrence(1622, CTL_dWARNING, mc_jamcode);
		}
		break;

	case 3000:
		st_sync.mn_uld_module_sortertray_supply[1] = CTL_LOCK;
		RunTransStep = 3100;
		break;

	case 3100:
		if(st_sync.mn_uld_module_sortertray_supply[1] == CTL_FREE || (COMI.mn_simulation_mode == 1 &&(st_work.nMdlInputCount[0][0] == st_work.nMdlPassCount[0][0])))
		{
			RunTransStep = 100;			
		}
		break;

	case 10000:
		nRet_1 = ULD_Module_TrayBack();
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunTransStep = 11000;
		}
		break;

	case 11000:
		if(st_sync.n_lotend_unload_site != CTL_YES)
		{
			st_sync.n_lotend_module_ldsortertray_site = CTL_NO;		// 일단 LOT END를 시키자.
			mn_LeakM_LotEnd[1] = YES;
			RunTransStep = 0;
		}
		else
		{
			st_sync.n_lotend_module_ldsortertray_site = CTL_YES;		// 일단 LOT END를 시키자.
			RunTransStep = 0;
		}
		break;
	}
}

void CRun_Stacker_Sorter::Run_Stacker1Move()
{
	int nRet_1, nRet_2, nRet_3;
	int mn_MotorNum;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_ULDM_STACKER_1);
		CTL_Lib.mn_single_motmove_step[M_ULDM_STACKER_1] = 0;
		ReadyPosStep[M_STACKER_1] = 0;
		COMI.mn_home_step[M_ULDM_STACKER_1] = 0;
		return;
	}
	mn_MotorNum = M_ULDM_STACKER_1;
	mn_StackerPos = M_STACKER_1;
	
	Func.ThreadFunctionStepTrace(71, RunS1Step);

	switch(RunS1Step)
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_unload_site == CTL_YES)
			{
				st_sync.n_lotend_module_ldsorterstacker_site = CTL_YES;		// 일단 LOT END를 시키자.
				mn_LeakM_LotEnd[0] = NO;
			}
			if(mn_LeakM_LotEnd[0] == NO && st_sync.n_lotend_module_ldsorterstacker_site == CTL_NO)
			{
				RunS1Step = 10;
			}
		}
		break;

	case 10:
		//100600 0 10 "Unload Load Module Slide에 트레이가 존재하지 않습니다."
		//100601 0 10 "Unload Load Module Slide에 트레이를 제거해 주세요."
		//100602 0 10 "Unload Unload Module Slide에 트레이가 존재하지 않습니다."
		//100603 0 10 "Unload Unload Module Slide에 트레이를 제거해 주세요."
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_ld_slide_tray_chk, IO_OFF);
		nRet_3 = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_tray_chk, IO_ON);
		if(nRet_1 == IO_ON || nRet_3 == IO_OFF)
		{
			if(nRet_1 == IO_ON)
			{
				//100100 1 10 "Sorter Stacker#2에 트레이가 존재합니다.제거해 주세요."
				sprintf(mc_jamcode, "100601"); 
			}
			else
			{
				//100100 1 10 "Sorter Stacker#2에 트레이가 존재합니다.제거해 주세요."
				sprintf(mc_jamcode, "100602"); 
			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1623, st_work.mn_run_status, mc_jamcode);	
		}
		else
		{
			RunS1Step = 20;
		}
		break;

	case 20:
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			RunS1Step = 30;
			break;
		}

		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_ON);
		if(nRet_1 == IO_ON)
		{
			//090202 0 09 "Unload Stacker#1 레일에 트레이가 있습니다."
			sprintf(mc_jamcode, "090202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1624, st_work.mn_run_status, mc_jamcode);	
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

		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_ld_slide_onoff_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_onoff_chk, IO_OFF);
		if(nRet_1 == IO_ON && nRet_2 == IO_ON)
		{
			mn_retry1 = 0;
			RunS1Step = 100;
		}
		else
		{//100802 0 10 "Unload Module Slide가 Open되어 있습니다. Close 해 주세요."
		 //100804 0 10 "Unload Unload Module Slide가 Open되어 있습니다. Close 해 주세요."
			if(nRet_1 == IO_OFF) sprintf(mc_jamcode, "100802");
			else				sprintf(mc_jamcode, "100804");
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
				mn_retry1 = 0;
				//090204 0 09 "Unload Stacker#1 레일 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode, "090204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1625, st_work.mn_run_status, mc_jamcode);
				RunS1Step = 30;
			}
			else
			{
				RunS1Step = 30;
			}
		}
		break;

	case 200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF);		
		if (nRet_1 == IO_OFF)
		{
			RunS1Step = 300;
		}
		else
		{
			//090202 0 09 "Unload Stacker#1 레일에 트레이가 있습니다."
			sprintf(mc_jamcode, "090202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1626, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(M_ULDM_STACKER_1, st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_stacker1_init_chk = YES;
			RunS1Step = 500;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunS1Step = 300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1647, st_work.mn_run_status, COMI.mc_alarmcode);			
		}
		break;


	case 500:
		if(mn_Moving_stacker == YES)
		{
			RunS1Step = 900;
		}
		break;

	case 900:
		Set_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
		RunS1Step = 910;
		break;

	case 910:
		nRet_1 = Get_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_retry2 = 0;
			mn_stacker_updn_cyliner[mn_StackerPos] = CYLINDER_ON;//항상 UP
			RunS1Step = 1000;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//060205 0 06 "Right Heat Sink#l Stacker 레일 Up 센서 에러."
				sprintf(mc_jamcode,"060205");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1755, st_work.mn_run_status, mc_jamcode);
				RunS1Step = 900;
			}
			else
			{
				RunS1Step = 900;
			}
		}
		break;
		
	case 1000:
		nRet_1 = OnModuleStackerReadyPos(mn_StackerPos,1); //SD 대기
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			st_sync.mn_uld_module_sortertray_supply[0] = CTL_READY;
			RunS1Step = 1100;
		}
		else if(nRet_1 == CTL_FULL)
		{
			//000302 1 00 "Unload Stocker 1 Tray Full 에러"
			sprintf(mc_jamcode, "000302"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1627, st_work.mn_run_status, mc_jamcode);	
			RunS1Step = 1000;
		}
		else if(nRet_1 == CTLBD_RET_SKIP) //스태커에 트레이가 없을때 
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				st_sync.n_lotend_module_ldsorterstacker_site = CTL_YES;
				RunS1Step = 1100;
			}
		}
		break;		

	case 1100:
		if(st_sync.n_lotend_module_uldsorterstacker_site == CTL_YES || mn_LeakM_LotEnd[1] == YES)
		{
			RunS1Step = 10000;
		}
		else if(st_sync.mn_uld_module_sortertray_supply[0] == CTL_CHANGE)
		{
			RunS1Step = 1200;
		}
		break;

	case 1200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_ON)
		{
			//090100 1 09 "Unload Stacker#2에 트레이가 존재합니다.제거해 주세요."
			sprintf(mc_jamcode,"090100");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1628, st_work.mn_run_status, mc_jamcode);			
		}
		else
		{
			RunS1Step = 1300;
		}
		break;

	case 1300:
		if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_fwd_chk + (mn_StackerPos*3), IO_OFF) == IO_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_bwd_chk + (mn_StackerPos*3), IO_ON) == IO_ON))	// Off 상태.
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
				//090204 0 09 "Unload Stacker#1 레일 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"090204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1629, st_work.mn_run_status, mc_jamcode);
				RunS1Step = 1310;
			}
			else
			{
				RunS1Step = 1310;
			}
		}
		break;

	case 1500:
		nRet_1 = OnModuleStackerReadyPos(mn_StackerPos, 0); //트레이 하나 Up한다
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 1600;
		}
		else if (nRet_1 == CTLBD_RET_SKIP)
		{
			st_sync.n_lotend_module_ldsorterstacker_site = CTL_YES;
			RunS1Step = 1100;
		}
		break;

	case 1600:
		nRet_1 = COMI.Get_MotIOSensor(mn_MotorNum, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
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
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS1Step = 1800;
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
			
			//090203 0 09 "Unload Stacker#1 레일 forward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"090203");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1631, st_work.mn_run_status, mc_jamcode);
			RunS1Step = 1700;
		}
		break;

	case 2000:
		nRet_1 = OnModuleStackerReadyPos(mn_StackerPos, 2); //트레 하나 Dn한다
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS1Step = 2100;
		}
		else if(nRet_1 == CTL_FULL)
		{
			//090302 1 09 "Unload Stocker1 Tray Full 에러"
			sprintf(mc_jamcode, "090302"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1632, st_work.mn_run_status, mc_jamcode);	
			RunS1Step = 2000;
		}
		else if (nRet_1 == CTLBD_RET_SKIP)
		{
			RunS1Step = 1100;
		}
		break;

	case 2100:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_ON);
		
		if (nRet_1 == IO_ON || st_basic.n_mode_device == 2)	// 트레이가 있다
		{
			RunS1Step = 3000;
		}
		else
		{
			//090201 0 09 "Unload Stacker#1 레일에 트레이가 존재하지 않습니다."
			//sprintf(mc_jamcode,"090201");
			//st_work.mn_run_status = CTL_dWARNING;
			//CTL_Lib.Alarm_Error_Occurrence(1632, st_work.mn_run_status, mc_jamcode);
			RunS1Step = 2200;
		}
		break;

	case 2200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_ON);
		
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
		st_sync.mn_uld_module_sortertray_supply[mn_StackerPos] = CTL_LOCK;		// 교체 끝났다고 설정.
		//트레이 정보 
		RunS1Step = 1000;
		break;


	case 10000:
		COMI.Set_Motor_IO_Property(M_ULDM_STACKER_1, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(M_ULDM_STACKER_1, 5, st_basic.nRunSpeed);
		
		if (nRet_1 == BD_GOOD)
		{
			RunS1Step = 10010;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunS1Step= 800;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//090010 0 09 "Unload Stacker#1 Motor 위치 이동 에러."
			sprintf(mc_jamcode,"090010");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1633, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 10010:
		FAS_IO.set_out_bit(st_io.o_uld_stacker1_up_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_uld_stacker1_dn_onoff, IO_ON);
		RunS1Step = 10100;
		break;

	case 10100:
		nRet_1 = Set_StackerUpDnClinder(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			RunS1Step = 10110;
		}
		break;

	case 10110:
		nRet_1 = Get_StackerUpDnClinder(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_stacker_updn_cyliner[mn_StackerPos] = CYLINDER_OFF;
			RunS1Step = 10200;
			RunS1Step = 10300;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000205 0 00 "Stacker1 레일 Up 센서 에러."
			if(mn_StackerPos == 0)			sprintf(mc_jamcode,"090205");
			else                            sprintf(mc_jamcode,"100205");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1086, st_work.mn_run_status, mc_jamcode);
			RunS1Step = 10200;
		}
		break;

	case 10200:
		if(st_sync.n_lotend_unload_site == CTL_YES)
		{
			RunS1Step = 10300;
		}
		else if (st_sync.mn_uld_module_sortertray_supply[0] == CTL_REQ)//제품 요구시
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_tray_chk, IO_OFF);
			
			if(nRet_1 == IO_OFF)
			{//991005 0 99 "Unload  트레이를 공급해 주세요."
				//st_sync.n_lotend_module_ldstacker_site = CTL_YES;
				sprintf(mc_jamcode,"991005");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1691, st_work.mn_run_status, mc_jamcode);
			}
			else//다시 시도한다
			{
				RunS1Step = 1000;
			}
		}
		break;		

	case 10300:
		nRet_1 = COMI.HomeCheck_Mot(M_ULDM_STACKER_1, st_motor[M_ULDM_STACKER_1].mn_homecheck_method, MOT_TIMEOUT);
		
		if (nRet_1 == BD_GOOD)
		{
			RunS1Step = 10400;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunS1Step = 10100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//090300 0 09 "Unload Stacker1 Motor 홈 체크 에러."
			sprintf(mc_jamcode,"090300");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1634, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 10400:
		if(st_sync.n_lotend_unload_site != CTL_YES)
		{
			st_sync.n_lotend_module_ldsorterstacker_site = CTL_NO;		// 일단 LOT END를 시키자.
			mn_LeakM_LotEnd[0] = YES;
			RunS1Step = 0;
		}
		else
		{
			st_sync.n_lotend_module_ldsorterstacker_site = CTL_YES;
			RunS1Step = 0;
		}
		break;	
	}
}

void CRun_Stacker_Sorter::Run_Stacker2Move()
{
	int nRet_1, nRet_2;
	int mn_MotorNum;
	double dCurrentPos;

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_ULDM_STACKER_2);
		CTL_Lib.mn_single_motmove_step[M_ULDM_STACKER_2] = 0;
		ReadyPosStep[M_STACKER_2] = 0;
		COMI.mn_home_step[M_ULDM_STACKER_2] = 0;
		return;
	}
	mn_MotorNum = M_ULDM_STACKER_2;
	mn_StackerPos = M_STACKER_2;
	Func.ThreadFunctionStepTrace(72, RunS2Step);

	switch(RunS2Step)
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(mn_LeakM_LotEnd[2] == NO && st_sync.n_lotend_module_uldsorterstacker_site == CTL_NO)
			{
				////2014,0305

				////

				RunS2Step = 10;
			}
			if(COMI.mn_simulation_mode == 1 && mn_LeakM_LotEnd[2] == YES && st_sync.n_lotend_module_uldsorterstacker_site == CTL_NO) // 20130509
			{
				RunS2Step = 10000;
			}
		}
		break;

	case 10:
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_chk, IO_ON);
		if(nRet_2 == IO_OFF)
		{
			//100101 1 10 "Sorter Stacker#2에 트레이가 존재X."
			sprintf(mc_jamcode,"100101");
			CTL_Lib.Alarm_Error_Occurrence(1635, CTL_dWARNING, mc_jamcode);			
		}
		else
		{
			RunS2Step = 20;
		}
		break;
		
	case 20:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_OFF);
		if(nRet_1 == IO_ON)
		{
			//100202 0 00 "Unload Stacker#2 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"100202");
			CTL_Lib.Alarm_Error_Occurrence(1636, CTL_dWARNING, mc_jamcode);			
		}
		else
		{
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
				//090204 0 09 "Unload Stacker#1 레일 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"090204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1637, st_work.mn_run_status, mc_jamcode);
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
			//st_sync.mn_uld_module_sortertray_supply[1] = CTL_FREE;
			RunS2Step = 70;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
//			100207 0 10 "Unload Stocker2 Latch Clamp Tray On 에러."
//			100208 0 10 "Unload Stocker2 Latch Clamp Tray Off 에러."
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				alarm.mstr_code = "100208";
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1638, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 50;
			}
			else
			{
				RunS2Step = 50;
			}
		}
		break;

	case 70:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_OFF);
		
		if (nRet_1 == IO_OFF)
		{
			//FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_clamp_onoff, IO_ON);
			//FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_unclamp_onoff, IO_OFF);
			//Set_TrayLatchClinder(CYLINDER_OFF);
			RunS2Step = 90;
		}
		else
		{
			//090202 0 09 "Unload 090202 레일에 트레이가 있습니다."
			sprintf(mc_jamcode, "090202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1639, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 90:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_OFF);
		
		if (nRet_1 == IO_OFF)	// 트레이가 없으면...
		{
			mn_retry2 = 0;
			RunS2Step = 95;
		}
		else
		{
			//100202 0 10 "Unload Stacker#2 레일에 트레이가 있습니다."
			sprintf(mc_jamcode,"100202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1640, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 95:
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			RunS2Step = 100;
			break;
		}

		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_ld_slide_onoff_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_onoff_chk, IO_OFF);
		if(nRet_1 == IO_ON && nRet_2 == IO_ON)
		{
			RunS2Step = 100;
		}
		else
		{//100802 0 10 "Unload Load Module Slide가 Open되어 있습니다. Close 해 주세요."
		 //100804 0 10 "Unload Unload Module Slide가 Open되어 있습니다. Close 해 주세요."
			if(nRet_1 == IO_OFF)	sprintf(mc_jamcode, "100802");
			else				sprintf(mc_jamcode, "100804");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1030, st_work.mn_run_status, mc_jamcode);	
		}
		break;

	case 100:
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			RunS2Step = 110;
			break;
		}

		dCurrentPos = COMI.Get_MotCurrentPos(M_ULDM_STACKER_2);
		if(dCurrentPos > st_motor[M_ULDM_STACKER_2].md_pos[P_STOCKER_UP] - st_motor[M_ULDM_STACKER_2].mn_allow && 
			dCurrentPos < st_motor[M_ULDM_STACKER_2].md_pos[P_STOCKER_P_LIMIT] + st_motor[M_ULDM_STACKER_2].mn_allow)
		{
			RunS2Step = 110;
		}
		else
		{//100051 0 10 "Unload Stacker#2 Motor Up Limit 위치 이동 에러."
			sprintf(mc_jamcode, "100051"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1668, st_work.mn_run_status, mc_jamcode);
		}

	case 110:
		st_sync.n_uld_module_guide_tray_stacker[0] = CTL_REQ;
		RunS2Step = 120;
		break;

	case 120:
		if(st_sync.n_uld_module_guide_tray_stacker[1] == CTL_LOCK)
		{
			st_sync.n_uld_module_guide_tray_stacker[0] = CTL_CLEAR;
			st_sync.n_uld_module_guide_tray_stacker[1] = CTL_READY;
			RunS2Step = 130;
		}
		break;

	case 130:
		if(mn_stacker1_init_chk == YES)
		{
			RunS2Step = 140;
		}
		break;

	case 140:		
		nRet_1 = CTL_Lib.Single_Move(M_ULDM_STACKER_2, st_motor[M_ULDM_STACKER_2].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunS2Step = 150;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunS2Step = 140;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1648, st_work.mn_run_status, COMI.mc_alarmcode);			
		}
		break;

	case 150:
		mn_stacker2_init_chk = YES;
		RunS2Step = 200;
		break;

	case 200:
		if(mn_Moving_stacker == YES)
		{
			RunS2Step = 500;
		}
		break;
		/*nRet_1 = CTL_Lib.Single_Move(M_ULDM_STACKER_MOVE, 180.0, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry2 = 0;
			mn_StackerTrayReady = YES;
			RunS2Step = 500;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunS2Step= 400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1641, st_work.mn_run_status, COMI.mc_alarmcode);
			RunS2Step = 400;
		}
		break;*/

	case 500:
		//nRet_1 = Set_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
		//if(nRet_1 == CTL_GOOD)
		//{
			RunS2Step = 600;
		//}
		break;

	case 600:
	//	nRet_1 = Get_StackerUpDnClinder(mn_StackerPos, CYLINDER_ON);
	//	if(nRet_1 == CYLINDER_ON)
	//	{
			RunS2Step = 700;
	//	}
	/*	else if(nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//100205 0 10 "Unload Stacker#2 레일 Up 센서 에러."
				sprintf(mc_jamcode,"100205");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1642, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 500;
			}
			else
			{
				RunS2Step = 500;
			}
		}*/
		break;

	case 700:
		/*if (st_sync.mn_ld_module_sortertray_change[0] == CTL_YES)		// 로드 트레이를 올려 놓을 꺼라고 요청.
		{
			RunS2Step = 800;
		}
		else
		{
			if(st_sync.n_lotend_unload_site == YES)
			{
				RunS2Step = 10000;
			}
			else if (st_sync.n_lotend_module_ldsorterstacker_site == CTL_YES)
			{
				st_sync.n_lotend_module_uldsorterstacker_site = CTL_READY;
				RunS2Step = 800;
			}
		}*/
		RunS2Step = 800;
		break;

	case 800:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_OFF);
		if (nRet_1 == IO_OFF)
		{
			//if (st_sync.n_lotend_module_uldsorterstacker_site == CTL_READY)		// Lot End 동작이면..
			//{
			//	RunS2Step = 10000;
			//}
			//else
			//{
				RunS2Step = 1000;
			//}
		}
		else// if (nRet_1 == IO_ON)
		{
			mn_retry2 = 0;
			Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			RunS2Step = 810;
		}
		break;

	case 810:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS2Step = 820;
		}
		break;

	case 820:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			RunS2Step = 830;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//100204 0 10 "Unload Stacker#2 레일푸셔 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"100204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1643, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 810;
			}
			else
			{
				RunS2Step = 810;
			}
		}
		break;

	case 830:
		if(FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_off_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_on_chk, IO_OFF) == IO_OFF)
		{
			RunS2Step = 900;
		}
		else
		{
			mn_retry2 = 0;
			nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
			if(nRet_1 == CYLINDER_ON)
			{
				RunS2Step = 850;
			}
		}
		break;

	case 840:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 850;
		}
		break;

	case 850:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			RunS2Step = 900;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{//100201 0 10 "Unload Stacker#2 레일에 트레이가 존재하지 않습니다."
				mn_retry2 = 0;
				sprintf(mc_jamcode,"100201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1644, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 840;
			}
			else
			{
				RunS2Step = 840;
			}
		}
		break;

	case 900:
		//if (st_sync.n_lotend_module_uldsorterstacker_site == CTL_READY)		// Lot End 동작이면..
		//{
		//	RunS2Step = 10000;
		//}
		//else
		//{
			RunS2Step = 1000;
		//}
		break;

	case 1000:		
		nRet_1 = OnModuleStackerReadyPos(mn_StackerPos,1); //SD 대기		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			mn_retry2 = 0;
			Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_ON);
			RunS2Step = 1200;
		}
		else if(nRet_1 == CTL_FULL)
		{
			//100200 0 10 "Unload Stacker#2 Tray Full 에러"
			sprintf(mc_jamcode, "100200"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1645, st_work.mn_run_status, mc_jamcode);			
			RunS2Step = 1000;
		}
		break;

	case 1100:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS2Step = 1200;
		}
		break;
		
	case 1200:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_ON);
		if (nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 1300;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				//100203 0 10 "Unload Stacker#2 레일푸셔 forward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"100203");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1646, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 1100;
			}
			else
			{
				RunS2Step = 1100;
			}
		}
		break;

	case 1300:
		if((FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_off_chk, IO_OFF)) == CTL_ON && 
			(FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_on_chk, IO_ON) == CTL_OFF) || COMI.mn_simulation_mode == 1) // 20130509
		{
			RunS2Step = 2000;			
		}
		else
		{
			mn_retry2 = 0;
			Set_TrayLatchClinder(CYLINDER_OFF);
			RunS2Step = 1500;
		}
		break;
		
	case 1400:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 1500;
		}
		break;

	case 1500:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
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
				//100208 0 10 "Unload Stocker#2 Latch Clamp Tray Off 에러."
				sprintf(mc_jamcode, "100208");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1647, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 1400;
			}
			else
			{
				RunS2Step = 1400;
			}
		}
		break;


	case 2000:
		st_sync.mn_uld_module_sortertray_supply[1] = CTL_READY; //준비됨.
		RunS2Step = 2100;
		break;

	case 2100:
		if(st_sync.mn_uld_module_sortertray_supply[1] == CTL_LOCK)
		{
			RunS2Step = 2200;
		}
		else if(st_sync.mn_uld_module_sortertray_supply[0] == CTL_NOTREADY || (COMI.mn_simulation_mode == 1 &&(st_work.nMdlInputCount[0][0] == st_work.nMdlPassCount[0][0])))//2012,0910 && st_sync.n_lotend_unload_site == CTL_YES)
		{
			RunS2Step = 10000;
		}
		break;

	case 2200:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_ON);

		if (nRet_1 == IO_ON || st_basic.n_mode_device == 2)	// 트레이가 있으면...
		{
			mn_retry2 = 0;
			Set_TrayLatchClinder(CYLINDER_ON);
			RunS2Step = 2500;
		}
		else
		{
			//100201 0 10 "Unload Stacker#2 레일에 트레이가 존재하지 않습니다."
			sprintf(mc_jamcode, "100201");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1648, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 2300:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 2500;
		}
		break;

	case 2500:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_ON);
		if (nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 2600;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				//100207 0 10 "Unload Stocker#2 Latch Clamp Tray On 에러."
				sprintf(mc_jamcode, "100207");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1649, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 2300;
			}
			else
			{
				RunS2Step = 2300;
			}
		}
		break;

	case 2600:		
		st_sync.mn_uld_module_sortertray_supply[1] = CTL_FREE; //교체 완료
		RunS2Step = 2700;
		break;

	case 2700:
		if(st_sync.n_module_uldrbt_tray_req[0] == CTL_REQ)
		{
			st_sync.n_module_uldrbt_tray_req[0] = CTL_READY;
			st_sync.n_module_uldrbt_tray_req[1] = BIN_LDBUFFERBIN;
			RunS2Step = 3000;
		}
		break;

	case 3000://한장 빼기
		if(st_sync.n_module_uldrbt_tray_req[0] == CTL_REQ)
		{
			st_sync.mn_uld_module_sortertray_supply[1] = CTL_CHANGE;
			RunS2Step = 3100;
		}
		else if(st_sync.n_lotend_unload_site == CTL_YES || st_handler.n_lotend_ready == 6)
		{
// 			 st_sync.n_module_uldrbt_tray_req[0] = CTL_REQ; //2016.0525
			st_sync.mn_uld_module_sortertray_supply[1] = CTL_CHANGE;
			RunS2Step = 3100;
		}
		break;

	case 3100:
		if(st_sync.mn_uld_module_sortertray_supply[1] != CTL_CHANGE) return;

		if (FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON && 
		FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_off_chk, IO_OFF) == IO_OFF)	// On 상태.
		{
			nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
			{
				mn_retry2 = 0;
				RunS2Step = 3300;
			}
		}
		else
		{
			nRet_1 = Set_TrayLatchClinder(CYLINDER_ON);
			if(nRet_1 == CYLINDER_ON)
			{
				mn_retry2 = 0;
				RunS2Step = 3200;
			}
		}
		break;

	case 3110:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 3200;
		}
		break;

	case 3200:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_ON);
		if (nRet_1 == CYLINDER_OFF)
		{
			nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
			if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
			{
				mn_retry2 = 0;
				RunS2Step = 3300;
			}
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				//100207 0 10 "Unload Stocker#2 Latch Clamp Tray On 에러."
				sprintf(mc_jamcode, "100207");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1650, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 3110;
			}
			else
			{
				RunS2Step = 3110;
			}
		}
		break;
		
	case 3210:
		nRet_1 = Set_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)//CYLINDER_ON)//20150216
		{
			RunS2Step = 3300;
		}
		break;

	case 3300:
		nRet_1 = Get_RailFwdbwdCylinderOnOff(mn_StackerPos, CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			ReadyPosStep[mn_StackerPos] = 0;
			RunS2Step = 3400;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//100204 0 10 "Unload Stacker#2 레일푸셔 backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode, "100204");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1651, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 3210;
			}
			else
			{
				RunS2Step = 3210;
			}
		}
		break;

	case 3400:
		if (FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_fwd_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_bwd_chk, IO_ON) == IO_ON)	// On 상태.
		{
			RunS2Step = 3420;
		}
		else
		{
			mn_retry2 = 0;
			RunS2Step = 3210;
		}
		break;

	case 3410:
		nRet_1 = Set_TrayLatchClinder(CYLINDER_OFF);
		if(nRet_1 == CYLINDER_ON)
		{
			RunS2Step = 3500;
		}
		break;

	case 3420:
		nRet_1 = OnModuleStackerReadyPos(mn_StackerPos, 0); //트레이 하나 Up한다

		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			nRet_1 = CTLBD_RET_GOOD;
		}

		if(nRet_1 == CTLBD_RET_GOOD)
		{
			mn_retry2 = 0;
			Set_TrayLatchClinder(CYLINDER_OFF);
			RunS2Step = 3500;
		}
		else if(nRet_1 == CTLBD_RET_SKIP) //2016.0526
		{
// 			if( st_sync.n_lotend_module_ldsorterstacker_site == CTL_YES )
// 			{
// 			}
			//
			nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_OFF);
			
			if (nRet_1 == IO_OFF)// 트레이가 없다
			{
				//100201 0 10 "Unload Stacker#2 Rail tray does not exist in.[PS1202]"
				sprintf(mc_jamcode, "100201");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(3653, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 3400;
			}
		}

		break;

	case 3500:
		nRet_1 = Get_TrayLatchClinder(CYLINDER_OFF);
		if (nRet_1 == CYLINDER_OFF)
		{
			ReadyPosStep[M_STACKER_2] = 0;
			RunS2Step = 3600;
		}
		else if (nRet_1 == CYLINDER_ERROR)
		{
			mn_retry2++;
			if(mn_retry2 > 3)
			{
				mn_retry2 = 0;
				//100208 0 10 "Unload Stocker#2 Latch Clamp Tray Off 에러."
				sprintf(mc_jamcode, "100208");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1652, st_work.mn_run_status, mc_jamcode);
				RunS2Step = 3410;
			}
			else
			{
				RunS2Step = 3410;
			}
		}
		break;

	case 3600:
		nRet_1 = OnModuleStackerReadyPos(mn_StackerPos, 2); //트레이 하나 Dn한다
 
		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			nRet_1 = CTLBD_RET_GOOD;
		}
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS2Step = 3700;
		}
		//else if (nRet_1 == CTLBD_RET_SKIP)
		//{
		//	RunS2Step = 1000;
		//}
		break;

	case 3700:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_OFF);
		
		if (nRet_1 == IO_OFF || st_basic.n_mode_device == 2)	// 트레이가 없다
		{
			RunS2Step = 4000;
		}
		else
		{
			//100202 0 10 "Unload Stacker#2 레일에 트레이가 있습니다."
			sprintf(mc_jamcode, "100202");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1653, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 4000:
		st_sync.mn_uld_module_sortertray_supply[mn_StackerPos] = CTL_FREE;		// 교체 끝났다고 설정.
		//트레이 정보 
		RunS2Step = 1000;
		break;

	case 10000:
		COMI.Set_Motor_IO_Property(M_ULDM_STACKER_2, cmSD_EN, cmFALSE);
		nRet_1 = CTL_Lib.Single_Move(M_ULDM_STACKER_2, 5, st_basic.nRunSpeed);
		
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			RunS2Step = 10010;
		}
		else if (nRet_1 == CTLBD_RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1654, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 10010:
		FAS_IO.set_out_bit(st_io.o_uld_stacker2_up_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_uld_stacker2_dn_onoff, IO_ON);
		RunS2Step = 10100;
		break;


	case 10100:
		nRet_1 = COMI.HomeCheck_Mot(M_ULDM_STACKER_2, st_motor[M_ULDM_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);
		
		if (nRet_1 == BD_GOOD)
		{
			RunS2Step = 10200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunS2Step = 10100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			//100300 0 10 "Unload Stacker2 Motor 홈 체크 에러."
			alarm.mstr_code = "100300";
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1655, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 10200:
		if(st_sync.n_lotend_unload_site != CTL_YES)
		{
			st_sync.n_lotend_module_uldsorterstacker_site = CTL_NO;		// 일단 LOT END를 시키자.
			mn_LeakM_LotEnd[2] = YES;
			RunS2Step = 0;
		}
		else
		{
			st_sync.n_lotend_module_uldsorterstacker_site = CTL_YES;
			RunS2Step = 0;
		}
		break;	
	}
}

void CRun_Stacker_Sorter::Run_Moving_Stacker()
{
	int nRet_1;	

	if(alarm.n_area_ready_alarm[0] == TRUE)
	{
		COMI.Set_MotStop(0, M_ULDM_STACKER_MOVE);
		CTL_Lib.mn_single_motmove_step[M_ULDM_STACKER_MOVE] = 0;
		COMI.mn_home_step[M_ULDM_STACKER_MOVE] = 0;
		return;
	}

	Func.ThreadFunctionStepTrace(77, RunMStep);

	switch(RunMStep)
	{
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(mn_LeakM_LotEnd[3] == NO && st_sync.n_lotend_module_uldmovingtray_site == CTL_NO)
			{
				mn_ms_retry = 0;
				RunMStep = 100;
			}
		}		
		break;

	case 100:
		if(st_sync.n_uld_module_guide_tray_stacker[0] == CTL_REQ)
		{
			RunMStep = 110;
		}
		break;

	case 110:
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_clamp_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_unclamp_onoff, IO_OFF);
		RunMStep = 200;
		break;

	case 200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_stacker_guide_clamp_onoff, st_io.i_uld_stacker_guide_clamp_on_chk, st_io.i_uld_stacker_guide_unclamp_off_chk,
			IO_ON, st_wait.n_on_wait_time[37], st_wait.n_limit_wait_time[37]);
		if(nRet_1 == RET_GOOD)
		{
			RunMStep = 300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//090400 0 09 "Unload stacker1 guide clamp가 On이 되지 않습니다."
		 //090401 0 09 "Unload Stacker1 guide clamp가 Off가 되지 않습니다."
			mn_ms_retry++;
			if(mn_ms_retry > 5)
			{
				if(FAS_IO.get_in_bit(st_io.i_uld_stacker_guide_clamp_on_chk, IO_ON) == IO_ON
					&& FAS_IO.get_in_bit(st_io.i_uld_stacker_guide_clamp_on_chk,IO_OFF) == IO_OFF)
				{
					RunMStep = 300;
				}
				else
				{
					mn_ms_retry = 0;
					sprintf(mc_jamcode, "090400");			
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1676, st_work.mn_run_status, mc_jamcode);
					RunMStep = 110;
				}
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_unclamp_onoff, IO_OFF);
				RunMStep = 110;
			}
		}
		break;

	case 300:
		st_sync.n_uld_module_guide_tray_stacker[1] = CTL_LOCK;
		RunMStep = 400;
		break;

	case 400:
		if(st_sync.n_uld_module_guide_tray_stacker[1] == CTL_READY)
		{
			RunMStep = 1000;
		}
		break;

	case 1000:
		if(mn_stacker1_init_chk == YES && mn_stacker2_init_chk == YES)
		{
			RunMStep = 1100;
		}
		break;

	case 1100:
		nRet_1 = CTL_Lib.Single_Move(M_ULDM_STACKER_MOVE, st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			RunMStep = 1200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunMStep = 1100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1678, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1200:
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_uld_stacker_guide_unclamp_onoff, IO_ON);
		RunMStep = 1300;
		break;

	case 1300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_stacker_guide_clamp_onoff, st_io.i_uld_stacker_guide_unclamp_off_chk, st_io.i_uld_stacker_guide_clamp_on_chk,
			IO_ON, st_wait.n_on_wait_time[37], st_wait.n_limit_wait_time[37]);
		if(nRet_1 == RET_GOOD)
		{
			RunMStep = 1400;
		}
		else if(nRet_1 != RET_PROCEED)
		{//090400 0 09 "Unload stacker1 guide clamp가 On이 되지 않습니다."
		 //090401 0 09 "Unload Stacker1 guide clamp가 Off가 되지 않습니다."
			mn_ms_retry++;
			if(mn_ms_retry > 3)
			{
				mn_ms_retry = 0;
				sprintf(mc_jamcode, "090401");			
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1677, st_work.mn_run_status, mc_jamcode);
				RunMStep = 1200;
			}
			else
			{
				RunMStep = 1200;
			}
		}
		break;
		
	case 1400:
		mn_Moving_stacker = YES;
		RunMStep = 2000;
		break;

	case 2000:
		if(st_sync.n_lotend_unload_site == CTL_YES && st_sync.n_lotend_module_ldsorterstacker_site == CTL_YES && 
			st_sync.n_lotend_module_uldsorterstacker_site == CTL_YES)
		{
			RunMStep = 2100;
		}
		else if(mn_LeakM_LotEnd[0] == YES && mn_LeakM_LotEnd[1] == YES && mn_LeakM_LotEnd[2] == YES)
		{
			RunMStep = 2100;
		}
		break;

	case 2100:
		if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk, IO_ON) == IO_ON &&
			FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk, IO_OFF) == IO_OFF)
		{
			RunMStep = 3000;
		}
		else
		{//090109 0 09 "Unload stacker#1 트레이 다운 실린더가 동작을 하지 않습니다."
			sprintf(mc_jamcode, _T("090109"));
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1695, st_work.mn_run_status, mc_jamcode);
		}
		break;

	case 3000:
		nRet_1 = CTL_Lib.Single_Move(M_ULDM_STACKER_MOVE, st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunMStep = 3100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunMStep = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1679, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3100:
		if(mn_LeakM_LotEnd[0] == YES && mn_LeakM_LotEnd[1] == YES && mn_LeakM_LotEnd[2] == YES)
		{
			st_sync.n_lotend_module_uldmovingtray_site = CTL_NO;
			mn_Moving_stacker = NO;
			 mn_LeakM_LotEnd[3] = YES;
			 RunMStep = 4000;
		}
		else
		{				
			st_sync.n_lotend_module_uldmovingtray_site = CTL_YES;
			mn_Moving_stacker = NO;
			RunMStep = 0;
		}
		st_sync.n_uld_module_guide_tray_stacker[1] = CTL_CLEAR;
		break;

		
	case 4000:
		if(mn_LeakM_LotEnd[0] == YES && mn_LeakM_LotEnd[1] == YES && mn_LeakM_LotEnd[2] == YES)
		{
			if(st_handler.n_lotend_ready == 6)
			{
				st_sync.n_lot_reready[3] = CTL_REQ;
			}
			RunMStep = 4100;
		}
		break;

	case 4100:
		if(st_sync.n_lot_reready[0] == CTL_REQ && st_sync.n_lot_reready[1] == CTL_REQ && 
			st_sync.n_lot_reready[2] == CTL_REQ && st_sync.n_lot_reready[3] == CTL_REQ)
		{
			RunMStep = 4200;
		}
//		else if(st_work.n_lotend != CTL_YES && st_work.n_loadlot_count[LDMODULE_SITE] > (st_work.nMdlPassCount[0][0] + st_work.nMdlRejectCount[0][0]))
		else if(st_handler.n_more_uld_tray == CTL_YES)
		{
			if(FAS_IO.get_in_bit(st_io.i_uld_ld_slide_tray_chk, IO_OFF) == IO_OFF)
			{
				st_handler.mn_init_state[INIT_ULD_STACKER] = CTL_NO;
				InitStep = 0;
				RunMStep = 4400;
			}
			else
			{
				RunMStep = 4200;
			}
		}
		break;

	case 4200:
		//994006 0 99 "ULD Module 자재Tray를 제거해 주세요."
		sprintf(mc_jamcode, "994006"); 
		st_work.mn_run_status = CTL_dWARNING;
		CTL_Lib.Alarm_Error_Occurrence(1680, st_work.mn_run_status, mc_jamcode);
		RunMStep = 4300;
		break;


	case 4300:
		if(FAS_IO.get_in_bit(st_io.i_uld_ld_slide_tray_chk, IO_OFF) == IO_OFF)
		{
			st_handler.mn_init_state[INIT_ULD_STACKER] = CTL_NO;
			RunMStep = 4400;
		}
		else
		{
			RunMStep = 4200;
		}
		break;

	case 4400:
		InitStep = 0;
		if(st_handler.n_lotend_ready == 6)
		{
			st_sync.n_lot_reready[0] = st_sync.n_lot_reready[1] = st_sync.n_lot_reready[2] = st_sync.n_lot_reready[3] = CTL_READY;
		}
		RunMStep = 4500;
		break;

	case 4500:
		Run_Init();
		if(st_handler.mn_init_state[INIT_ULD_STACKER] == CTL_YES)
		{
			st_sync.n_lot_reready[3] = CTL_LOCK;
			RunMStep = 4600;
		}
		break;
		
	case 4600:
		if(st_sync.n_lot_reready[0] == CTL_LOCK && st_sync.n_lot_reready[1] == CTL_LOCK && st_sync.n_lot_reready[2] == CTL_LOCK && st_sync.n_lot_reready[3] == CTL_LOCK)
		{
			st_sync.n_lot_reready[0] = st_sync.n_lot_reready[1] = st_sync.n_lot_reready[2] = st_sync.n_lot_reready[3] = CTL_CLEAR;
			RunMStep = 4700;
		}
		else if(st_handler.n_more_uld_tray == CTL_YES)
		{
			RunMStep = 4700;
		}
		break;

	case 4700:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_tray_chk, IO_OFF);
		if(nRet_1 == IO_OFF)
		{
			//994007 0 99 "ULD Module Blank(빈) Tray를 투입해 주세요."
			sprintf(mc_jamcode, "994007"); 
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1681, st_work.mn_run_status, mc_jamcode);
		}
		else
		{
			RunMStep = 4800;
		}
		break;

	case 4800:
		st_handler.n_more_uld_tray = CTL_NO;
		st_sync.mn_uld_module_sortertray_supply[0] = CTL_CLEAR;
		mn_LeakM_LotEnd[0] = mn_LeakM_LotEnd[1] = mn_LeakM_LotEnd[2] = mn_LeakM_LotEnd[3] = NO;
		if(st_handler.n_lotend_ready == 6)	st_handler.n_lotend_ready = CTL_NO;
		RunMStep = 0;
		break;

	}
}


int CRun_Stacker_Sorter::Set_TrayLatchClinder(int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	FAS_IO.set_out_bit(st_io.o_uld_stacker2_tray_clamp_onoff, n_onoff);
	mb_latchcylinder_flag = FALSE;
	ml_latchcylinder_Time[0] = GetCurrentTime();

	return nFuncRet;
	
}

int CRun_Stacker_Sorter::Get_TrayLatchClinder(int n_onoff)
{
	int nFuncRet;// = -1;//20140924
	
	if (COMI.mn_simulation_mode == 1) // 20130509
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
		else if (FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_on_chk, IO_ON) == CTL_ON && 
			FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_off_chk, IO_OFF) == CTL_OFF)	// On 상태.
		{
			ml_latchcylinder_Time[2] = ml_latchcylinder_Time[1] - ml_latchcylinder_Time[0];
			
			if (ml_latchcylinder_Time[2] < 0)		
			{
				ml_latchcylinder_Time[0] = GetCurrentTime();
				//return nFuncRet;
				nFuncRet = CYLINDER_ERROR;	//20140924
				mb_latchcylinder_flag = FALSE;
			}
			
			if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
			{
				if (ml_latchcylinder_Time[2] > st_wait.n_limit_wait_time[37])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_latchcylinder_flag = FALSE;
				}
			}
			else if (ml_latchcylinder_Time[2] > st_wait.n_on_wait_time[37])
			{
				nFuncRet = CYLINDER_ON;
				mb_latchcylinder_flag = FALSE;
			}
		}
		else if (FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_on_chk, IO_OFF) == CTL_OFF && 
			FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_off_chk, IO_ON) == CTL_ON)	// Off 상태.
		{
			ml_latchcylinder_Time[2] = ml_latchcylinder_Time[1] - ml_latchcylinder_Time[0];
			
			if (ml_latchcylinder_Time[2] < 0)		
			{
				ml_latchcylinder_Time[0] = GetCurrentTime();
				//return nFuncRet;
				nFuncRet = CYLINDER_ERROR;	//20140924
				mb_latchcylinder_flag = FALSE;
			}
			
			if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
			{
				if (ml_latchcylinder_Time[2] > st_wait.n_limit_wait_time[37])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_latchcylinder_flag = FALSE;
				}
			}
			else if (ml_latchcylinder_Time[2] > st_wait.n_on_wait_time[37])
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
//				return nFuncRet;
				nFuncRet = CYLINDER_ERROR;	//20140924
				mb_latchcylinder_flag = FALSE;
			}
			
			if (ml_latchcylinder_Time[2] > st_wait.n_limit_wait_time[37])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_latchcylinder_flag = FALSE;
			}
		}
	}	
	return nFuncRet;
}

int CRun_Stacker_Sorter::OnModuleStackerReadyPos(int n_StackerPos, int nMode)
{
	int nRet_1 = VARIABLE_INIT,nRet_2 = VARIABLE_INIT;
	int nFuncRet = CTLBD_RET_PROCEED;
	int nMotorNum;

	if(n_StackerPos == M_STACKER_1)
	{
		if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk, IO_ON) == IO_ON && FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk, IO_OFF) == IO_OFF)
		{
			mn_stacker_updn_cyliner[n_StackerPos] = CYLINDER_ON;
		}
		else
		{
			mn_stacker_updn_cyliner[n_StackerPos] = CYLINDER_OFF;
		}
		nMotorNum = M_ULDM_STACKER_1;
	}
	else
	{
		nMotorNum = M_ULDM_STACKER_2;
		mn_stacker_updn_cyliner[n_StackerPos] = CYLINDER_ON;
	}

	if(st_basic.n_mode_device == 2)
	{
		return CTLBD_RET_GOOD;
	}
	
	//2016.0526
	if(n_StackerPos == M_STACKER_1)
	{
		Func.ThreadFunctionStepTrace(73, ReadyPosStep[n_StackerPos]);
	}
	else
	{
		Func.ThreadFunctionStepTrace(74, ReadyPosStep[n_StackerPos]);
	}

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
			mn_retry = 0;
			ReadyPosStep[n_StackerPos] = 100;
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
			// 090051 0 09 "Unload Stacker#1 Motor Up Limit 위치 이동 에러."
			// 100051 0 10 "Unload Stacker#2 Motor Up Limit 위치 이동 에러."
//			if(nMotorNum == M_LDM_STACKER_1) alarm.mstr_code = "090051";
//			else                             alarm.mstr_code = "100051";
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1660, st_work.mn_run_status, COMI.mc_alarmcode);
			ReadyPosStep[n_StackerPos] = 0;
		}
		else if(nRet_1 == CTLBD_RET_SAFETY)
		{
			// 090051 0 09 "Unload Stacker#1 Motor Up Limit 위치 이동 에러."
			// 100051 0 10 "Unload Stacker#2 Motor Up Limit 위치 이동 에러."
//			if(nMotorNum == M_LDM_STACKER_1) alarm.mstr_code = "090051";
//			else                             alarm.mstr_code = "100051";
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1661, st_work.mn_run_status, COMI.mc_alarmcode);
			ReadyPosStep[n_StackerPos] = 0;
		}
		break;

	case 200:
		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(1, nMotorNum);

			if(n_StackerPos == M_STACKER_1)
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
			else
			{
				ReadyPosStep[n_StackerPos] = 300;
			}
		}
		else
		{
			nRet_1 = COMI.Check_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_UP]);
			if(nRet_1 == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
			{
				if (nMode == 0)
				{
					//ReadyPosStep[n_StackerPos] = 2000;

					if(n_StackerPos == M_STACKER_1)
					{
						nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);
						nRet_2 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF);
						if(nRet_1 == IO_ON || nRet_2 == IO_ON)
						{
							if(nRet_1 == IO_ON)
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
					else
					{
						if(mn_stacker_updn_cyliner[n_StackerPos] == CYLINDER_ON) ReadyPosStep[n_StackerPos] = 5000;
						else
						{
							COMI.Set_Motor_IO_Property(nMotorNum, cmSD_EN, cmFALSE);
							ReadyPosStep[n_StackerPos] = 1000;
						}
					}
				}
				else if (nMode == 2)// SD 감지후에 DOWN하는 모드
				{
					ReadyPosStep[n_StackerPos] = 300;
				}
				else//1 // SD 감지거나, UP_LIMIT까지만 동작하는 모드이다.
				{
					ReadyPosStep[n_StackerPos] = 5000;
				}

//				if (st_basic.n_mode_device == 2)		ReadyPosStep[n_StackerPos] = 300;
			}
			else if (nRet_1 == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
			{
				// 090051 0 09 "Unload Stacker#1 Motor Up Limit 위치 이동 에러."
				// 100051 0 10 "Unload Stacker#2 Motor Up Limit 위치 이동 에러."
//				if(nMotorNum == M_LDM_STACKER_1) alarm.mstr_code = "090051";
//				else                             alarm.mstr_code = "100051";
				mn_retry++;
				if(mn_retry > 2)
				{
					mn_retry = 0;
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1662, st_work.mn_run_status, COMI.mc_alarmcode);
					ReadyPosStep[n_StackerPos] = 100;
				}
				else
				{
					ReadyPosStep[n_StackerPos] = 100;
				}
			}
			else if (nRet_1 == CTLBD_RET_SAFETY)
			{
				// 090051 0 09 "Unload Stacker#1 Motor Up Limit 위치 이동 에러."
				// 100051 0 10 "Unload Stacker#2 Motor Up Limit 위치 이동 에러."
//				if(nMotorNum == M_LDM_STACKER_1) alarm.mstr_code = "090051";
//				else                             alarm.mstr_code = "100051";
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1663, st_work.mn_run_status, COMI.mc_alarmcode);
				ReadyPosStep[n_StackerPos] = 100;
			}
			//else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
			//{
			//	ReadyPosStep[n_StackerPos] = 100;
			//}
		}
		break;

	case 300:
		if (nMode == 0)//한장 올린다
		{
			m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);
			m_dTargetPos[n_StackerPos] = m_dNowPos[n_StackerPos] + st_motor[nMotorNum].md_pos[P_STOCKER_UP_OFFSET];
			if(m_dTargetPos[n_StackerPos] > st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT])
			{
				m_dTargetPos[n_StackerPos] = st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT];
			}
			ReadyPosStep[n_StackerPos] = 400;
		}
		else if (nMode == 2)//한장만큼 내린다.
		{ 
			m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);			
			m_dTargetPos[n_StackerPos] = m_dNowPos[n_StackerPos] - st_motor[nMotorNum].md_pos[P_STOCKER_DN_OFFSET];
			
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
		if(nRet_1 == BD_GOOD)
		{
			//ReadyPosStep[n_StackerPos] = 5000;
		
			if(nMode == 0)//UP
			{
				if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk + (n_StackerPos * 3), IO_ON) == IO_ON)
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
			CTL_Lib.Alarm_Error_Occurrence(1664, st_work.mn_run_status, COMI.mc_alarmcode);
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
			CTL_Lib.Alarm_Error_Occurrence(1665, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1100:
		m_dNowPos[n_StackerPos] = COMI.Get_MotCurrentPos(nMotorNum);
		if(m_dNowPos[n_StackerPos] > st_motor[nMotorNum].md_pos[P_STOCKER_P_LIMIT] - st_motor[nMotorNum].md_pos[P_STOCKER_UP_OFFSET] + st_motor[nMotorNum].mn_allow) break;

		nRet_1 = COMI.Get_MotIOSensor(nMotorNum, MOT_SENS_SD);

		if(COMI.mn_simulation_mode == 1) // 20130509
		{
			nRet_1 = CTLBD_RET_PROCEED;
		}

		if(nRet_1 != CTLBD_RET_GOOD)
		{
// 			ml_moveWaitTime[1] = GetCurrentTime();
// 			ml_moveWaitTime[2] = ml_moveWaitTime[1] - ml_moveWaitTime[0];
			COMI.Set_MotStop(1, nMotorNum);
			
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
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				ReadyPosStep[n_StackerPos] = 1200;
			}
			else if(nRet_1 == CTLBD_RET_ERROR)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1666, st_work.mn_run_status, COMI.mc_alarmcode);
				ReadyPosStep[n_StackerPos] = 1000;
			}
			else if(nRet_1 == CTLBD_RET_SAFETY)
			{
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1667, st_work.mn_run_status, COMI.mc_alarmcode);
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
		
	case 2000:
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
			CTL_Lib.Alarm_Error_Occurrence(1669, st_work.mn_run_status, COMI.mc_alarmcode);
			ReadyPosStep[n_StackerPos] = 2000;
		}
		break;

	case 2020:
		if(n_StackerPos == M_STACKER_1)
		{
			ReadyPosStep[n_StackerPos] = 2100;
		}
		else
		{
			ReadyPosStep[n_StackerPos] = 2300;
		}
		break;

	case 2100:
		nRet_1 = Set_StackerUpDnClinder(n_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep[n_StackerPos] = 2110;
		}
		break;

	case 2110:
		nRet_1 = Get_StackerUpDnClinder(n_StackerPos, CYLINDER_OFF);
		if(nRet_1 == CYLINDER_OFF)
		{
			mn_stacker_updn_cyliner[n_StackerPos] = CYLINDER_OFF;
			ReadyPosStep[n_StackerPos] = 2120;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000205 0 00 "Stacker1 레일 Up 센서 에러."
			if(n_StackerPos == 0)			sprintf(mc_jamcode, "000205");
			else                            sprintf(mc_jamcode, "010205");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1670, st_work.mn_run_status, mc_jamcode);
			ReadyPosStep[n_StackerPos] = 2100;
		}
		break;

	case 2120:
		if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_tray_chk, IO_ON) == IO_ON) ReadyPosStep[n_StackerPos] = 2200;
		else                                                                 ReadyPosStep[n_StackerPos] = 2300;
		break;
		
	case 2200:
		nRet_1 = Set_StackerUpDnClinder(n_StackerPos, CYLINDER_ON);
		if(nRet_1 == CTL_GOOD)
		{
			ReadyPosStep[n_StackerPos] = 2210;
		}
		break;

	case 2210:
		nRet_1 = Get_StackerUpDnClinder(n_StackerPos, CYLINDER_ON);
		if(nRet_1 == CYLINDER_ON)
		{
			mn_stacker_updn_cyliner[n_StackerPos] = CYLINDER_ON;
			ReadyPosStep[n_StackerPos] = 100;
		}
		else if(nRet_1 == CYLINDER_ERROR)
		{
			//000205 0 00 "Stacker1 레일 Up 센서 에러."
			if(n_StackerPos == 0)			sprintf(mc_jamcode, "000205");
			else                            sprintf(mc_jamcode, "010205");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1670, st_work.mn_run_status, mc_jamcode);
			ReadyPosStep[n_StackerPos] = 2100;
		}
		break;

	case 2300:
		st_sync.mn_uld_module_sortertray_supply[n_StackerPos] = CTL_NOTREADY;		// 트레이가 없음.
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


int CRun_Stacker_Sorter::Set_StackerUpDnClinder(int nStacker, int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	/*if(n_onoff == CYLINDER_OFF)
	{
		if(FAS_IO.get_in_bit(st_io.i_m_stacker1_tray_chk + (nStacker * 4), IO_ON) == IO_ON)
		{
			//000100 0 00 "Stacker1에 트레이가 존재합니다.제거해 주세요."
			if(nStacker == 0)	sprintf(mc_jamcode,"000100");
			else                sprintf(mc_jamcode,"010100");
			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			alarm.mn_alarm_assign_section = 1004;
			CTL_Lib.Alarm_Error_Occurrence(alarm.mn_count_mode, st_work.mn_run_status, mc_jamcode);
			nFuncRet = CYLINDER_ERROR;
		}
	}*/

	if (nStacker == 0)
	{
		FAS_IO.set_out_bit(st_io.o_uld_stacker1_up_onoff, n_onoff);
		FAS_IO.set_out_bit(st_io.o_uld_stacker1_dn_onoff, !n_onoff);
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_uld_stacker2_up_onoff, n_onoff);
		FAS_IO.set_out_bit(st_io.o_uld_stacker2_dn_onoff, !n_onoff);
	}
	mb_updncylinder_flag[nStacker] = FALSE;
	ml_undncylinder_Time[nStacker][0] = GetCurrentTime();

	return nFuncRet;
	
}

int CRun_Stacker_Sorter::Get_StackerUpDnClinder(int nStacker, int n_onoff)
{
	int nFuncRet = -1;

	if(COMI.mn_simulation_mode == 1) // 20130509 
	{
		return n_onoff;
	}
	
	if(nStacker == 0)
	{
		ml_undncylinder_Time[nStacker][1] = GetCurrentTime();
		
		if (mb_updncylinder_flag[nStacker] != TRUE)
		{
			ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
			mb_updncylinder_flag[nStacker] = TRUE;
		}
		else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk, IO_ON) == CTL_ON) && 
			(FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk, IO_OFF) == CTL_OFF))	// On 상태.
		{
			ml_undncylinder_Time[nStacker][2] = ml_undncylinder_Time[nStacker][1] - ml_undncylinder_Time[nStacker][0];
			
			if (ml_undncylinder_Time[nStacker][2] < 0)		
			{
				ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
			{
				if (ml_undncylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[28])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_updncylinder_flag[nStacker] = FALSE;
				}
			}
			else if (ml_undncylinder_Time[nStacker][2] > st_wait.n_on_wait_time[28])
			{
				nFuncRet = CYLINDER_ON;
				mb_updncylinder_flag[nStacker] = FALSE;
			}
		}
		else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk, IO_OFF) == CTL_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk, IO_ON) == CTL_ON))	// Off 상태.
		{
			ml_undncylinder_Time[nStacker][2] = ml_undncylinder_Time[nStacker][1] - ml_undncylinder_Time[nStacker][0];
			
			if (ml_undncylinder_Time[nStacker][2] < 0)		
			{
				ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
			{
				if (ml_undncylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[28])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_updncylinder_flag[nStacker] = FALSE;
				}
			}
			else if (ml_undncylinder_Time[nStacker][2] > st_wait.n_on_wait_time[28])
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
			
			if (ml_undncylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[28])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_updncylinder_flag[nStacker] = FALSE;
			}
		}
	}
	else
	{
		ml_undncylinder_Time[nStacker][1] = GetCurrentTime();
		
		if (mb_updncylinder_flag[nStacker] != TRUE)
		{
			ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
			mb_updncylinder_flag[nStacker] = TRUE;
		}
		else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker2_up_chk, IO_ON) == CTL_ON) && 
			(FAS_IO.get_in_bit(st_io.i_uld_stacker2_dn_chk, IO_OFF) == CTL_OFF))	// On 상태.
		{
			ml_undncylinder_Time[nStacker][2] = ml_undncylinder_Time[nStacker][1] - ml_undncylinder_Time[nStacker][0];
			
			if (ml_undncylinder_Time[nStacker][2] < 0)		
			{
				ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
			{
				if (ml_undncylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[28])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_updncylinder_flag[nStacker] = FALSE;
				}
			}
			else if (ml_undncylinder_Time[nStacker][2] > st_wait.n_on_wait_time[28])
			{
				nFuncRet = CYLINDER_ON;
				mb_updncylinder_flag[nStacker] = FALSE;
			}
		}
		else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker2_up_chk, IO_OFF) == CTL_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_uld_stacker2_dn_chk, IO_ON) == CTL_ON))	// Off 상태.
		{
			ml_undncylinder_Time[nStacker][2] = ml_undncylinder_Time[nStacker][1] - ml_undncylinder_Time[nStacker][0];
			
			if (ml_undncylinder_Time[nStacker][2] < 0)		
			{
				ml_undncylinder_Time[nStacker][0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
			{
				if (ml_undncylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[28])
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_updncylinder_flag[nStacker] = FALSE;
				}
			}
			else if (ml_undncylinder_Time[nStacker][2] > st_wait.n_on_wait_time[28])
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
			
			if (ml_undncylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[28])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_updncylinder_flag[nStacker] = FALSE;
			}
		}
	}
	
	return nFuncRet;
}

int CRun_Stacker_Sorter::MotorMove(int nMotor, double dMovePos, int nSpeedRate)
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet;
	
	switch (MotorMoveStep)
	{
	case 0:
		nRet = COMI.Start_SingleMove(nMotor, dMovePos, nSpeedRate);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			MotorMoveStep = 100;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			nFuncRet = CTL_ERROR;
			MotorMoveStep = 0;
		}
		else if (nRet == CTLBD_RET_SAFETY)
		{
			nFuncRet = CTLBD_RET_SAFETY;
			MotorMoveStep = 0;
		}
		break;
		
	case 100:
		nRet = COMI.Check_SingleMove(nMotor, dMovePos);
		
		if (nRet == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
		{
			MotorMoveStep = 0;
			nFuncRet = CTLBD_RET_GOOD;
		}
		else if (nRet == CTLBD_RET_ERROR)		// 정상 동작을 하지 못했다면...
		{
			nFuncRet = CTLBD_RET_ERROR;
			MotorMoveStep = 0;
		}
		else if (nRet == CTLBD_RET_SAFETY)		// 안전 관련 알람이라면...
		{
			nFuncRet = CTLBD_RET_SAFETY;
			MotorMoveStep = 0;
		}
		else if (nRet == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
		{
			MotorMoveStep = 0;
		}
		break;
	}
	
	return nFuncRet;
}

int CRun_Stacker_Sorter::Set_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff == CYLINDER_OFF)
	{
		/*if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk + (nStacker*3), IO_OFF) == IO_ON)
		{
			//090100 1 09 "Unload Stacker#1에 트레이가 존재합니다.제거해 주세요."
			//100100 1 10 "Sorter Stacker#2에 트레이가 존재합니다.제거해 주세요."
			if(nStacker == 0)		sprintf(mc_jamcode,"090100");
			else                    sprintf(mc_jamcode,"100100");
			alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
			alarm.mn_alarm_assign_section = 2030;
			CTL_Lib.Alarm_Error_Occurrence(alarm.mn_count_mode, st_work.mn_run_status, mc_jamcode);
			nFuncRet = CYLINDER_ERROR;
		}*/
	}

	FAS_IO.set_out_bit(st_io.o_uld_stacker1_rail_fwd_onoff + (nStacker*2), n_onoff);
	FAS_IO.set_out_bit(st_io.o_uld_stacker1_rail_bwd_onoff + (nStacker*2), !n_onoff);
	mb_pushercylinder_flag[nStacker] = FALSE;
	ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();

	return nFuncRet;
	 
}

int CRun_Stacker_Sorter::Get_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff)
{
	int nFuncRet = -1;

	if (COMI.mn_simulation_mode == 1) // 20130509
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
		else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_fwd_chk + (nStacker*3), IO_ON) == IO_ON) && (FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_bwd_chk + (nStacker*3), IO_OFF) == IO_OFF))	// On 상태.
		{
			ml_pushercylinder_Time[nStacker][2] = ml_pushercylinder_Time[nStacker][1] - ml_pushercylinder_Time[nStacker][0];
			
			if (ml_pushercylinder_Time[nStacker][2] < 0)		
			{
				ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
			{
				if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[33] + (nStacker*2))
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_pushercylinder_flag[nStacker] = FALSE;
				}
			}
			else if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_on_wait_time[33] + (nStacker*2))
			{
				nFuncRet = CYLINDER_ON;
				mb_pushercylinder_flag[nStacker] = FALSE;
			}
		}
		else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_fwd_chk + (nStacker*3), IO_OFF) == IO_OFF) && (FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_bwd_chk + (nStacker*3), IO_ON) == IO_ON))	// Off 상태.
		{
			ml_pushercylinder_Time[nStacker][2] = ml_pushercylinder_Time[nStacker][1] - ml_pushercylinder_Time[nStacker][0];
			
			if (ml_pushercylinder_Time[nStacker][2] < 0)		
			{
				ml_pushercylinder_Time[nStacker][0] = GetCurrentTime();
				return nFuncRet;
			}
			
			if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
			{
				if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[33] + (nStacker*2))
				{
					nFuncRet = CYLINDER_ERROR;	
					mb_pushercylinder_flag[nStacker] = FALSE;
				}
			}
			else if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_on_wait_time[33] + (nStacker*2))
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
			
			if (ml_pushercylinder_Time[nStacker][2] > st_wait.n_limit_wait_time[33] + (nStacker*2))
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder_flag[nStacker] = FALSE;
			}
		}
	}
	
	return nFuncRet;
}

int CRun_Stacker_Sorter::ULD_Module_TrayMove()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet_1;

	Func.ThreadFunctionStepTrace(74, RunStep);
	switch(TrayMoveStep)
	{
	case 0:
		TrayMoveStep = 1000;
		break;

	case 1000:
		TrayMoveStep = 2000;
		break;
		if(FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_clamp_up_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_clamp_dn_chk, IO_ON) == IO_ON)
		{
			TrayMoveStep = 2000;
		}
		else
		{
			mn_retry = 0;
			Set_PusherUpDnCylinderOnOff(CYLINDER_ON);
			TrayMoveStep = 1200;
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
				//090108 0 09 "Unload Stacker pusher clamp 업이 동작을 하지 않습니다."
				sprintf(mc_jamcode,"090108");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1680, st_work.mn_run_status, mc_jamcode);
				TrayMoveStep = 1100;			
			}
			else
			{
				TrayMoveStep = 1100;			
			}
		}
		break;

	case 2000:
		if ((FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_fwd_chk, IO_OFF) == IO_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_bwd_chk, IO_ON) == IO_ON))
		{
			mn_retry = 0;
			Set_PusherCylinderOnOff(CYLINDER_ON);
			TrayMoveStep = 2200;
		}
		else
		{
			//100107 0 10 "Unload Stacker#2 레일 backward가 동작이 되지 않습니다."
			sprintf(mc_jamcode,"100107");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1681, st_work.mn_run_status, mc_jamcode);
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
				//090104 0 09 "Unload stacker 트레이 pusher forward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"090104");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1682, st_work.mn_run_status, mc_jamcode);
				TrayMoveStep = 2100;
			}
			else
			{
				TrayMoveStep = 2100;
			}
		}
		break;

	case 2300:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_uld_stacker2_rail_tray_chk, IO_ON);
			
		if (st_basic.n_mode_device != 1)		nRet_1 = IO_ON;
		
		if(nRet_1 == IO_ON)
		{
			TrayMoveStep = 3000;
		}
		else
		{
			//100101 0 10 "Unload Stacker#2의 트레이가 존재하지 않습니다."
			sprintf(mc_jamcode,"100101");
			CTL_Lib.Alarm_Error_Occurrence(1683, CTL_dWARNING, mc_jamcode);
		}
		break;

	case 3000:
		nFuncRet = CTLBD_RET_GOOD;
		TrayMoveStep = 0;
		break;
		
	}
	return nFuncRet;
}


int CRun_Stacker_Sorter::Set_PusherUpDnCylinderOnOff(int n_onoff)
{
	int nFuncRet = CTL_GOOD;

	FAS_IO.set_out_bit(st_io.o_uld_stacker_tray_pusher_clamp_updn, n_onoff);
	mb_pusherupdncylinder_flag = FALSE;
	ml_pusherupdncylinder_Time[0] = GetCurrentTime();

	return nFuncRet;
	
}

int CRun_Stacker_Sorter::Get_PusherUpDnCylinderOnOff(int n_onoff)
{
	int nFuncRet = -1;
	
	ml_pusherupdncylinder_Time[1] = GetCurrentTime();
	
	if (mb_pusherupdncylinder_flag != TRUE)
	{
		ml_pusherupdncylinder_Time[0] = GetCurrentTime();
		mb_pusherupdncylinder_flag = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_clamp_dn_chk, IO_ON) == CTL_ON) && 
		(FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_clamp_up_chk, IO_OFF) == CTL_OFF))	// On 상태.
	{
		ml_pusherupdncylinder_Time[2] = ml_pusherupdncylinder_Time[1] - ml_pusherupdncylinder_Time[0];
		
		if (ml_pusherupdncylinder_Time[2] < 0)		
		{
			ml_pusherupdncylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_pusherupdncylinder_Time[2] > st_wait.n_limit_wait_time[32])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pusherupdncylinder_flag = FALSE;
			}
		}
		else if (ml_pusherupdncylinder_Time[2] > st_wait.n_on_wait_time[32])
		{
			nFuncRet = CYLINDER_ON;
			mb_pusherupdncylinder_flag = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_clamp_dn_chk, IO_OFF) == CTL_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_clamp_up_chk, IO_ON) == CTL_ON))	// Off 상태.
	{
		ml_pusherupdncylinder_Time[2] = ml_pusherupdncylinder_Time[1] - ml_pusherupdncylinder_Time[0];
		
		if (ml_pusherupdncylinder_Time[2] < 0)		
		{
			ml_pusherupdncylinder_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_pusherupdncylinder_Time[2] > st_wait.n_limit_wait_time[32])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pusherupdncylinder_flag = FALSE;
			}
		}
		else if (ml_pusherupdncylinder_Time[2] > st_wait.n_on_wait_time[32])
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
		
		if (ml_pusherupdncylinder_Time[2] > st_wait.n_limit_wait_time[32])
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_pusherupdncylinder_flag = FALSE;
		}
	}
	
	return nFuncRet;
}

int CRun_Stacker_Sorter::Set_PusherCylinderOnOff(int n_onoff)
{
	int nFuncRet = CTL_GOOD;
	if(n_onoff == CYLINDER_OFF)
	{
		//if(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF) == IO_ON)
		//{
			//090100 1 09 "Unload Stacker#1에 트레이가 존재합니다.제거해 주세요."
		//	sprintf(mc_jamcode,"090100");
		//	alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING; st_work.mn_run_status = CTL_dWARNING;
		//	alarm.mn_alarm_assign_section = 1904;
		//	CTL_Lib.Alarm_Error_Occurrence(alarm.mn_count_mode, st_work.mn_run_status, mc_jamcode);
		//	nFuncRet = CYLINDER_ERROR;
		//}
	}

	FAS_IO.set_out_bit(st_io.o_uld_stacker_tray_pusher_fwd_onoff, n_onoff);
	FAS_IO.set_out_bit(st_io.o_uld_stacker_tray_pusher_bwd_onoff, !n_onoff);
	mb_pushercylinder = FALSE;
	ml_pusher_Time[0] = GetCurrentTime();

	return nFuncRet;
	
}

int CRun_Stacker_Sorter::Get_PusherCylinderOnOff(int n_onoff)
{
	int nFuncRet = -1;
	
	ml_pusher_Time[1] = GetCurrentTime();
	
	if (mb_pushercylinder != TRUE)
	{
		ml_pusher_Time[0] = GetCurrentTime();
		mb_pushercylinder = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_fwd_chk, IO_ON) == CTL_ON) && 
		(FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_bwd_chk, IO_OFF) == CTL_OFF))	// On 상태.
	{
		ml_pusher_Time[2] = ml_pusher_Time[1] - ml_pusher_Time[0];
		
		if (ml_pusher_Time[2] < 0)		
		{
			ml_pusher_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_pusher_Time[2] > st_wait.n_limit_wait_time[30])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder = FALSE;
			}
		}
		else if (ml_pusher_Time[2] > st_wait.n_on_wait_time[30])
		{
			nFuncRet = CYLINDER_ON;
			mb_pushercylinder = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_fwd_chk, IO_OFF) == CTL_OFF) && 
		(FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_bwd_chk, IO_ON) == CTL_ON))	// Off 상태.
	{
		ml_pusher_Time[2] = ml_pusher_Time[1] - ml_pusher_Time[0];
		
		if (ml_pusher_Time[2] < 0)		
		{
			ml_pusher_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_pusher_Time[2] > st_wait.n_limit_wait_time[30])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_pushercylinder = FALSE;
			}
		}
		else if (ml_pusher_Time[2] > st_wait.n_on_wait_time[30])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_pushercylinder = FALSE;
		}
	}
	else
	{
		ml_pusher_Time[2] = ml_pusher_Time[1] - ml_pusher_Time[0];
		
		if (ml_pusher_Time[2] < 0)		
		{
			ml_pusher_Time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_pusher_Time[2] > st_wait.n_limit_wait_time[30])
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_pushercylinder = FALSE;
		}
	}
	
	return nFuncRet;
}

int CRun_Stacker_Sorter::ULD_Module_TrayBack()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	int nRet_1;

	Func.ThreadFunctionStepTrace(75, TrayBackStep);
	switch(TrayBackStep)
	{
	case 0:
		TrayBackStep = 1000;
		break;

	case 1000:
		TrayBackStep = 1100;
		break;
		if (FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_clamp_up_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_clamp_dn_chk, IO_OFF) == IO_OFF)
		{
			TrayBackStep = 1100;
		}
		else
		{
			mn_retry = 0;
			Set_PusherUpDnCylinderOnOff(CYLINDER_OFF);
			TrayBackStep = 1010;
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
				//090108 0 09 "Unload Stacker#1 업이 동작을 하지 않습니다."
				alarm.mstr_code = "090108";
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1684, st_work.mn_run_status, mc_jamcode);
				TrayBackStep = 1010;
			}
			else
			{
				TrayBackStep = 1010;
			}
		}
		break;

	case 1100:
		if ((FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_fwd_chk, IO_OFF) == IO_OFF) && 
			(FAS_IO.get_in_bit(st_io.i_uld_stacker_tray_pusher_bwd_chk, IO_ON) == IO_ON))
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
				//090105 0 09 "Unload Stacker#1 트레이 pusher backward가 동작이 되지 않습니다."
				sprintf(mc_jamcode,"090105");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1685, st_work.mn_run_status, mc_jamcode);
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


int CRun_Stacker_Sorter::Set_LDZoneBaseRailOnOff(int n_onoff)
{
	int nFunRet = CTL_GOOD;
	
	FAS_IO.set_out_bit(st_io.o_uld_stacker1_rail_fwd_onoff, n_onoff);
	FAS_IO.set_out_bit(st_io.o_uld_stacker1_rail_bwd_onoff, !n_onoff);
	
	mb_clamp_flag = FALSE;
	ml_clamp_time[0] = GetCurrentTime(); 
	
	return nFunRet;
}

int CRun_Stacker_Sorter::Get_LDZoneBaseRailOnOff(int n_onoff, int n_option)
{
	int nFuncRet = -1;
	
	ml_clamp_time[1] = GetCurrentTime();
	
	if (n_option == 0)		mb_clamp_flag = TRUE;

	if (mb_clamp_flag != TRUE)
	{
		ml_clamp_time[0] = GetCurrentTime();
		mb_clamp_flag = TRUE;
	}
	else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_bwd_chk, !n_onoff) == CTL_OFF) &&
		(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_fwd_chk, n_onoff) == CTL_ON))	// On 상태.
	{
		if (n_option == 0)		return CYLINDER_ON;

		ml_clamp_time[2] = ml_clamp_time[1] - ml_clamp_time[0];
		
		if (ml_clamp_time[2] < 0)		
		{
			ml_clamp_time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_OFF)		// Off를 확인하고자 한것인데 On 되어 있다면...
		{
			if (ml_clamp_time[2] > st_wait.n_limit_wait_time[37])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_clamp_flag = FALSE;
			}
		}
		else if (ml_clamp_time[2] > st_wait.n_on_wait_time[37])
		{
			nFuncRet = CYLINDER_ON;
			mb_clamp_flag = FALSE;
		}
	}
	else if ((FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_bwd_chk, !n_onoff) == CTL_ON) && 
		(FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_fwd_chk, n_onoff) == CTL_OFF))	// Off 상태.
	{
		if (n_option == 0)		return CYLINDER_OFF;

		ml_clamp_time[2] = ml_clamp_time[1] - ml_clamp_time[0];
		
		if (ml_clamp_time[2] < 0)		
		{
			ml_clamp_time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (n_onoff == CYLINDER_ON)		// On를 확인하고자 한것인데 Off 되어 있다면...
		{
			if (ml_clamp_time[2] > st_wait.n_off_wait_time[37])
			{
				nFuncRet = CYLINDER_ERROR;	
				mb_clamp_flag = FALSE;
			}
		}
		else if (ml_clamp_time[2] > st_wait.n_off_wait_time[37])
		{
			nFuncRet = CYLINDER_OFF;	
			mb_clamp_flag = FALSE;
		}
	}
	else
	{
		if (n_option == 0)		return CYLINDER_ERROR;

		ml_clamp_time[2] = ml_clamp_time[1] - ml_clamp_time[0];
		
		if (ml_clamp_time[2] < 0)		
		{
			ml_clamp_time[0] = GetCurrentTime();
			return nFuncRet;
		}
		
		if (ml_clamp_time[2] > st_wait.n_limit_wait_time[37])
		{
			nFuncRet = CYLINDER_ERROR;	
			mb_clamp_flag = FALSE;
		}
	}	
	return nFuncRet;
}

int CRun_Stacker_Sorter::OnElevatorWorkPos()
{
	int nRet = VARIABLE_INIT, nRet2 = VARIABLE_INIT;
	int nRet3 = VARIABLE_INIT, nRet4 = VARIABLE_INIT;
	int i = 0, j = 0, nCnt = 0;
	int nFuncRet = CTLBD_RET_PROCEED;
	CString strTemp;
	double dNowPos;
	char cJamcode[10] = {NULL};
	
	Func.ThreadFunctionStepTrace(76, WorkPosStep);

	if(WorkPosStep_old != WorkPosStep )
	{
		Func.OnLogWorkStep( "RUN_TRAY_SUPPLY", "OnElevatorWorkPos()", WorkPosStep_old, WorkPosStep );
	}
	WorkPosStep_old = WorkPosStep;

	switch(WorkPosStep)
	{
	case 0:
		nRet = COMI.Get_MotIOSensor(M_ULDM_STACKER_1, MOT_SENS_SD, IO_OFF);

		if (nRet == CTLBD_RET_GOOD)									// 있을 때, 
		{
			WorkPosStep = 1000;
		}
		else														// 없을 때...
		{
			WorkPosStep = 100;
		}
		break;

	case 100:
		COMI.Set_Motor_IO_Property(M_ULDM_STACKER_1, cmSD_EN, cmTRUE);    //cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 	
		nRet = COMI.Start_SingleMove(M_ULDM_STACKER_1, st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_UP], st_basic.nRunSpeed);

		if (nRet == CTLBD_RET_GOOD)
		{
			WorkPosStep = 200;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			// 090101 0 09 "Unload Stacker#1의 트레이가 존재하지 않습니다."
			sprintf(cJamcode, "090101");
			CTL_Lib.Alarm_Error_Occurrence(1686, CTL_dWARNING, cJamcode);
		}
		else if (nRet == CTLBD_RET_SAFETY)
		{
			// 090008 0 09 "Unload Stacker#1 Motor is Non Safety Pos."
			sprintf(cJamcode, "090008");
			CTL_Lib.Alarm_Error_Occurrence(1687, CTL_dWARNING, cJamcode);
		}
		break;
		
	case 200:
		nRet = COMI.Get_MotIOSensor(M_ULDM_STACKER_1, MOT_SENS_SD, IO_ON);

		if (nRet == CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, M_ULDM_STACKER_1);
			WorkPosStep = 300;
		}
		else
		{
			// **********************************************************************
			//  이동이 정상적으로 끝났는지를 확인한다.
			// **********************************************************************
			nRet = COMI.Check_SingleMove(M_ULDM_STACKER_1, st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_UP]);
			
			if (nRet == CTLBD_RET_GOOD)				// 정상 동작을 했다면...
			{
				WorkPosStep = 2000;

				if (MODE_CE)
				{
					WorkPosStep = 300;
				}
			}
			else if (nRet == CTLBD_RET_ERROR)
			{
				// 000101 1 A "Loader elevator top pos move error."
				sprintf(cJamcode, "000101");
				CTL_Lib.Alarm_Error_Occurrence(1688, CTL_dWARNING, cJamcode);
				WorkPosStep = 100;
			}
			else if (nRet == CTLBD_RET_SAFETY)
			{
				// 000008 0 A "Motor Move Safety Check Error - (Loader Tray Elevator)."
				sprintf(cJamcode, "000008");
				CTL_Lib.Alarm_Error_Occurrence(1689, CTL_dWARNING, cJamcode);
				WorkPosStep = 100;
			}
			else if (nRet == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
			{
				WorkPosStep = 100;
			}
			break;
		}
		break;

	case 300:
		dNowPos = COMI.Get_MotCurrentPos(M_ULDM_STACKER_1);

		dTargetPos = dNowPos + st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_UP_OFFSET];
		
		if (dTargetPos > st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT])		dTargetPos = st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_P_LIMIT];

		if (dTargetPos < st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN] + 50)	dTargetPos = st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN] + 50;
		
		WorkPosStep = 400;
		break;
		
	case 400:
		COMI.Set_Motor_IO_Property(M_ULDM_STACKER_1, cmSD_EN, cmFALSE);    //cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 
		nRet = MotorMove(M_ULDM_STACKER_1, dTargetPos, st_basic.nRunSpeed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			WorkPosStep = 500;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			// 000102 1 A "Loader elevator offset pos move error."
			sprintf(cJamcode, "000102");
			CTL_Lib.Alarm_Error_Occurrence(1690, CTL_dWARNING, cJamcode);
		}
		else if (nRet == CTLBD_RET_SAFETY)					// 안전 관련 알람이라면...
		{
			// 000008 0 A "Motor Move Safety Check Error - (Loader Tray Elevator)."
			sprintf(cJamcode, "000008");
			CTL_Lib.Alarm_Error_Occurrence(1691, CTL_dWARNING, cJamcode);
		}
		break;

	case 500:
		nRet = Set_LDZoneBaseRailOnOff(CYLINDER_ON);

		if (nRet == CTL_GOOD)
		{
			WorkPosStep = 510;
		}
		break;

	case 510:
		nRet = Get_LDZoneBaseRailOnOff(CYLINDER_ON);
		if (nRet == CYLINDER_ON)
		{		
			dNowPos = COMI.Get_MotCurrentPos(M_ULDM_STACKER_1);
			dTargetPos = dNowPos - (st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DN_OFFSET] * 2);

			if (dTargetPos < st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN] + 50)	dTargetPos = st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN] + 50;

			WorkPosStep = 600;
		}
		else if (nRet == CYLINDER_ERROR)
		{
			// 010206 1 A "Loader zone 1 tray clamp on error[PS0602][PS0604]."
			WorkPosStep = 500;
			sprintf(cJamcode, "010206");
			CTL_Lib.Alarm_Error_Occurrence(1692, CTL_dWARNING, cJamcode);
		}
		break;

	case 600:
		nRet = COMI.Start_SingleMove(M_ULDM_STACKER_1, dTargetPos, st_basic.nRunSpeed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			WorkPosStep = 610;
		}
		else if (nRet2 == CTLBD_RET_ERROR)
		{
			// 000102 1 A "Loader elevator offset pos move error."
			sprintf(cJamcode, "000102");
			CTL_Lib.Alarm_Error_Occurrence(1693, CTL_dWARNING, cJamcode);
		}
		else if (nRet2 == CTLBD_RET_SAFETY)
		{
			// 000008 0 A "Motor Move Safety Check Error - (Loader Tray Elevator)."
			sprintf(cJamcode, "000008");
			CTL_Lib.Alarm_Error_Occurrence(1694, CTL_dWARNING, cJamcode);
		}
		break;
		
	case 610:
		nRet = COMI.Check_SingleMove(M_ULDM_STACKER_1, dTargetPos);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			WorkPosStep = 1000;
		}
		else if (nRet == CYLINDER_ERROR)
		{
			// 010202 1 A "Loader zone tray seperate on error[PS0502][PS0504]."
			sprintf(cJamcode, "010202");
			CTL_Lib.Alarm_Error_Occurrence(1695, CTL_dWARNING, cJamcode);
			WorkPosStep = 600;
		}
		else if (nRet2 == CTLBD_RET_ERROR)
		{
			// 000102 1 A "Loader elevator offset pos move error."
			sprintf(cJamcode, "000102");
			CTL_Lib.Alarm_Error_Occurrence(1696, CTL_dWARNING, cJamcode);
			WorkPosStep = 600;
		}
		else if (nRet2 == CTLBD_RET_SAFETY)
		{
			// 000008 0 A "Motor Move Safety Check Error - (Loader Tray Elevator)."
			sprintf(cJamcode, "000008");
			CTL_Lib.Alarm_Error_Occurrence(1697, CTL_dWARNING, cJamcode);
			WorkPosStep = 600;
		}
		break;

		
	case 1000:
		nRet = COMI.Start_SingleMove(M_ULDM_STACKER_1, st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			WorkPosStep = 1100;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			// 000100 1 A "Loader elevator base pos move error."
			sprintf(cJamcode, "000100");
			CTL_Lib.Alarm_Error_Occurrence(1698, CTL_dWARNING, cJamcode);
		}
		else if (nRet == CTLBD_RET_SAFETY)
		{
			// 000008 0 A "Motor Move Safety Check Error - (Loader Tray Elevator)."
			sprintf(cJamcode, "000008");
			CTL_Lib.Alarm_Error_Occurrence(1699, CTL_dWARNING, cJamcode);
		}
		break;
		
	case 1100:
		nRet = COMI.Get_MotIOSensor(M_ULDM_STACKER_1, MOT_SENS_SD, IO_OFF);
		
		if (nRet != CTLBD_RET_GOOD)
		{
			COMI.Set_MotStop(0, M_ULDM_STACKER_1);
			
			WorkPosStep = 100;											// SD센서에 감지가 되지 않으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
		}
		else
		{
			nRet = COMI.Check_SingleMove(M_ULDM_STACKER_1, st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN]);
			
			if (nRet == CTLBD_RET_GOOD)
			{
				WorkPosStep = 1200;										// SD에 감지가 되면서 끝까지 내려왔다. 이건 FULL.. 좀 빼달라고 해야 한다.
			}
			else if (nRet == CTLBD_RET_ERROR)
			{
				// 000100 1 A "Loader elevator base pos move error."
				sprintf(cJamcode, "000100");
				CTL_Lib.Alarm_Error_Occurrence(1677, CTL_dWARNING, cJamcode);
				WorkPosStep = 1000;
			}
			else if (nRet == CTLBD_RET_SAFETY)
			{
				// 000008 0 A "Motor Move Safety Check Error - (Loader Tray Elevator)."
				sprintf(cJamcode, "000008");
				CTL_Lib.Alarm_Error_Occurrence(1678, CTL_dWARNING, cJamcode);
				WorkPosStep = 1000;
			}
		}
		break;

	case 1200:
		// 000304 1 A "Loader convayor elevator tray full."
		sprintf(cJamcode, "000304");
		CTL_Lib.Alarm_Error_Occurrence(1655, CTL_dWARNING, cJamcode);
		WorkPosStep = 1300;
		break;

	case 1300:
		nRet = COMI.Get_MotIOSensor(M_ULDM_STACKER_1, MOT_SENS_SD, IO_OFF);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			WorkPosStep = 1200;
		}
		else
		{
			WorkPosStep = 100;											// SD센서에 감지가 되지 않으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
		}
		break;

	case 2000:
		nRet = MotorMove(M_ULDM_STACKER_1, st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			WorkPosStep = 2100;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			// 000100 1 A "Loader elevator base pos move error."
			sprintf(cJamcode, "000100");
			CTL_Lib.Alarm_Error_Occurrence(1656, CTL_dWARNING, cJamcode);
		}
		else if (nRet == CTLBD_RET_SAFETY)
		{
			// 000008 0 A "Motor Move Safety Check Error - (Loader Tray Elevator)."
			sprintf(cJamcode, "000008");
			CTL_Lib.Alarm_Error_Occurrence(1657, CTL_dWARNING, cJamcode);
		}
		break;

	case 2100:
		nRet = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF);

		if (nRet == IO_OFF)									// 트레이가없다면.
		{
			WorkPosStep = 2200;
		}
		else if (nRet == IO_ON)								// SD센서에 감지가 되지 않았으니까, 감지 안되었을때 동작하는 100번 Step으로 간다.
		{
			WorkPosStep = 100;
		}
		break;

	case 2200:
		nFuncRet = CTLBD_RET_SKIP;
		WorkPosStep = 0;
		break;

	case 5000:
		nFuncRet = CTLBD_RET_GOOD;
		WorkPosStep = 0;
		break;
	}

	return nFuncRet;
}

int CRun_Stacker_Sorter::TrayUpPossibleCheck()
{	
	int nFuncRet = CTLBD_RET_GOOD;
	int nRet;

	nRet = FAS_IO.get_in_bit(st_io.i_uld_stacker1_rail_tray_chk, IO_OFF);				// Zone에 Tray가 있는지 확인한다.
	if (nRet == IO_ON)				return CTLBD_RET_ERROR;

	nRet = Get_LDZoneBaseRailOnOff(CYLINDER_OFF, 0);								// Base Rail이 Off 되어 있는지 확인한다.
	if (nRet != CYLINDER_OFF)		return CTLBD_RET_ERROR;


	return nFuncRet;
}

