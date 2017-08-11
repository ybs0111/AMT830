// Run_Rbt_Load_Clip.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Rbt_Load_Clip.h"

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


/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Load_Clip
CRun_Rbt_Load_Clip Run_Rbt_Load_Clip;
CRun_Rbt_Load_Clip::CRun_Rbt_Load_Clip()
{
	Thread_Variable_Initial();
}

CRun_Rbt_Load_Clip::~CRun_Rbt_Load_Clip()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Load_Clip message handlers
void CRun_Rbt_Load_Clip::Thread_Run()
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
		Run_ClipInsertMove();
		Run_ClipBufferMove();
		Run_ModuleClampMove();
		Run_ClipLotEndMove();
		break;

	case dREINSTATE :
		Run_Reinstate();  // 20130416
		break;
	
	default:
		if(st_work.mn_run_status == dSTOP)
		{
			OnClipRbt_FinalPos(); // 현재 위치를 저장한다. 
		}

		//dSTOP, dJAM, dLOTEND, dWARNING, dLOCK, dMAINT (장비가 동작하지 않는다면!)
		if(st_handler.mn_menu_num == 501)  return;   //XYZ Robot motor 화면이면..		

//		COMI.Set_MotStop(1, M_CLIP_INSERT_Y);
//		COMI.Set_MotStop(1, M_CLIP_INSERT_Z);
//		CTL_Lib.mn_single_motmove_step[M_CLIP_INSERT_Y] = 0;
//		CTL_Lib.mn_single_motmove_step[M_CLIP_INSERT_Z] = 0;
//		COMI.Set_MotStop(1, M_M_CLAMP_RBT_Z);
//		COMI.Set_MotStop(1, M_M_CLAMP_RBT_Y);
//		CTL_Lib.mn_single_motmove_step[M_M_CLAMP_RBT_Z] = 0;
//		CTL_Lib.mn_single_motmove_step[M_M_CLAMP_RBT_Y] = 0;
		InitStep = 0;
		ReinstateStep = 0;

		break;
	}
}

void CRun_Rbt_Load_Clip::OnClipRbt_FinalPos()//20121126
{
	if(st_work.mn_reinstatement_mode[LDCLIP_SITE] == 0) //stop시 최초 한번만 기록하기 위함
	{
		st_work.md_reinstatement_Pos[0][M_CLIP_BUFFER]         =  COMI.md_cmdpos_backup[M_CLIP_BUFFER];
		st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_GRIPPER] =  COMI.md_cmdpos_backup[M_M_CLAMP_RBT_GRIPPER];
		st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Y]	   =  COMI.md_cmdpos_backup[M_M_CLAMP_RBT_Y];
		st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Z]	   =  COMI.md_cmdpos_backup[M_M_CLAMP_RBT_Z];
		st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Y]	   =  COMI.md_cmdpos_backup[M_CLIP_INSERT_Y];
		st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Z]	   =  COMI.md_cmdpos_backup[M_CLIP_INSERT_Z];
		st_work.md_reinstatement_Pos[0][M_M_CLIP_CLAMP]		   =  COMI.md_cmdpos_backup[M_M_CLIP_CLAMP];

		st_work.mn_ClipInsertStatusBackup[0] = FAS_IO.get_out_bit(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		st_work.mn_ClipBufferStatusBackup[0] = FAS_IO.get_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);		
	
		st_work.mn_reinstatement_mode[LDCLIP_SITE] = 1;	//최종 위치를 기록 했다
	}

}

void CRun_Rbt_Load_Clip::Robot_BackMovePosValCheck()//20121126
{
	//여기서 이전에 동작했던 위치를 다시 계산해야한다.
	st_work.md_reinstatement_Pos[1][M_CLIP_BUFFER]         =  st_work.md_reinstatement_Pos[0][M_CLIP_BUFFER];
	st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_GRIPPER] =  st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_GRIPPER];
	st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Y]       =  st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Y];
	st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Z]       =  st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Z];
	st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Y]       =  st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Y];
	st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Z]       =  st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Z];
	st_work.md_reinstatement_Pos[1][M_M_CLIP_CLAMP]        =  st_work.md_reinstatement_Pos[0][M_M_CLIP_CLAMP];
}

void CRun_Rbt_Load_Clip::Run_Reinstate()
{
	int Ret1 = 0, Ret2 = 0, i = 0, n_response = 0;
	int nRet1 = 0, nRet2 = 0;

	if (st_work.mn_ClipRobot_Reinstatement_Ok == TRUE) return;//20121126

	switch (ReinstateStep)
	{
	case 0:
		ReinstateStep = 1000;
		break;

	case 1000://현재 위치 확인
		dClipCurPos[0] = COMI.Get_MotCurrentPos(M_CLIP_BUFFER);
		dClipCurPos[1] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
		dClipCurPos[2] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
		dClipCurPos[3] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
		dClipCurPos[4] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
		dClipCurPos[5] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
		dClipCurPos[6] = COMI.Get_MotCurrentPos(M_M_CLIP_CLAMP);

		for (i = 0; i < 7; i++)
		{
			mn_change[LDCLIP_SITE][i] = 0;
		}
	
		//변경된 위치값 출력
		if (dClipCurPos[0] > st_work.md_reinstatement_Pos[0][M_CLIP_BUFFER] + COMI.mn_allow_value[M_CLIP_BUFFER]
			|| dClipCurPos[0] < st_work.md_reinstatement_Pos[0][M_CLIP_BUFFER] - COMI.mn_allow_value[M_CLIP_BUFFER])
		{
			mn_change[LDCLIP_SITE][0]++;
			sprintf(st_msg.c_abnormal_msg, "Clip Buffer POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_CLIP_BUFFER], (long)dClipCurPos[0], (long)COMI.mn_allow_value[M_CLIP_BUFFER]);
		}
		else if (dClipCurPos[1] > st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_GRIPPER] + COMI.mn_allow_value[M_M_CLAMP_RBT_GRIPPER]
			|| dClipCurPos[1] < st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_GRIPPER] - COMI.mn_allow_value[M_M_CLAMP_RBT_GRIPPER])
		{
			mn_change[LDCLIP_SITE][1]++;
			sprintf(st_msg.c_abnormal_msg, "Clamp Rbt Gripper POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_GRIPPER], (long)dClipCurPos[1], (long)COMI.mn_allow_value[M_M_CLAMP_RBT_GRIPPER]);
		}
		else if (dClipCurPos[2] > st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Y] + COMI.mn_allow_value[M_M_CLAMP_RBT_Y]
			|| dClipCurPos[2] < st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Y] - COMI.mn_allow_value[M_M_CLAMP_RBT_Y])
		{
			mn_change[LDCLIP_SITE][2]++;
			sprintf(st_msg.c_abnormal_msg, "Clamp Rbt Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Y], (long)dClipCurPos[2], (long)COMI.mn_allow_value[M_M_CLAMP_RBT_Y]);
		}

		else if (dClipCurPos[3] > st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Z] + COMI.mn_allow_value[M_M_CLAMP_RBT_Z]
			|| dClipCurPos[3] < st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Z] - COMI.mn_allow_value[M_M_CLAMP_RBT_Z])
		{
			mn_change[LDCLIP_SITE][3]++;
			sprintf(st_msg.c_abnormal_msg, "Clamp Rbt Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Z], (long)dClipCurPos[3], (long)COMI.mn_allow_value[M_M_CLAMP_RBT_Z]);
		}
		else if (dClipCurPos[4] > st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Y] + COMI.mn_allow_value[M_CLIP_INSERT_Y]
			|| dClipCurPos[4] < st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Y] - COMI.mn_allow_value[M_CLIP_INSERT_Y])
		{
			mn_change[LDCLIP_SITE][4]++;
			sprintf(st_msg.c_abnormal_msg, "Clip Insert Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Y], (long)dClipCurPos[4], (long)COMI.mn_allow_value[M_CLIP_INSERT_Y]);
		}
		else if (dClipCurPos[5] > st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Z] + COMI.mn_allow_value[M_CLIP_INSERT_Z]
			|| dClipCurPos[5] < st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Z] - COMI.mn_allow_value[M_CLIP_INSERT_Z])
		{
			mn_change[LDCLIP_SITE][5]++;
			sprintf(st_msg.c_abnormal_msg, "Clip Insert Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Z], (long)dClipCurPos[5], (long)COMI.mn_allow_value[M_CLIP_INSERT_Z]);
		}
		else if (dClipCurPos[6] > st_work.md_reinstatement_Pos[0][M_M_CLIP_CLAMP] + COMI.mn_allow_value[M_M_CLIP_CLAMP]
			|| dClipCurPos[6] < st_work.md_reinstatement_Pos[0][M_M_CLIP_CLAMP] - COMI.mn_allow_value[M_M_CLIP_CLAMP])
		{
			mn_change[LDCLIP_SITE][6]++;
			sprintf(st_msg.c_abnormal_msg, "Clip Clamp POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_M_CLIP_CLAMP], (long)dClipCurPos[6], (long)COMI.mn_allow_value[M_M_CLIP_CLAMP]);
		}


		if(mn_change[LDCLIP_SITE][0] > 0 || mn_change[LDCLIP_SITE][1] > 0 || mn_change[LDCLIP_SITE][2] > 0 || mn_change[LDCLIP_SITE][3] > 0 ||
		   mn_change[LDCLIP_SITE][4] > 0 || mn_change[LDCLIP_SITE][5] > 0 || mn_change[LDCLIP_SITE][6] > 0) //rbt y
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
		Robot_BackMovePosValCheck();

		COMI.Set_MotPower(M_CLIP_BUFFER, TRUE);
		COMI.Set_MotPower(M_M_CLAMP_RBT_GRIPPER, TRUE);
		COMI.Set_MotPower(M_M_CLAMP_RBT_Y, TRUE);
		COMI.Set_MotPower(M_M_CLAMP_RBT_Z, TRUE);
		COMI.Set_MotPower(M_CLIP_INSERT_Y, TRUE);
		COMI.Set_MotPower(M_CLIP_INSERT_Z, TRUE);
		COMI.Set_MotPower(M_M_CLIP_CLAMP, TRUE);

		ReinstateStep = 3000;
		break;

	case 3000:
		if(st_work.mn_ClipBufferStatusBackup[0] == IO_OFF && st_work.mn_ClipInsertStatusBackup[0] == IO_OFF)
		{
			ReinstateStep = 4000;
		}
		else
		{
			ReinstateStep = 3100;
		}
		break;

	case 3100:
		FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		ReinstateStep = 3200;
		break;

	case 3200:
		nRet1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet1 == RET_GOOD)
		{
			ReinstateStep = 3300;
		}
		else if (nRet1 == RET_ERROR)
		{
			ReinstateStep = 3100;
		}
		break;

	case 3300:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		ReinstateStep = 3400;
		break;

	case 3400:
		nRet1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet1 == RET_GOOD)
		{
			ReinstateStep = 4000;
		}
		else if (nRet1 == RET_ERROR)
		{
			ReinstateStep = 3300;
		}	
		break;

	case 4000:// 		dClipCurPos[5] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
		nRet1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4100;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2391, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 4100:
		nRet1 = COMI.Check_SingleMove(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4200;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2391, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;
	
	case 4200:// 		dClipCurPos[4] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
		nRet1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_LD_SAFETY], st_basic.nManualSpeed);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4300;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2321, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 4300:
		nRet1 = COMI.Check_SingleMove(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_LD_SAFETY]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4400;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2321, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;
		
	case 4400:// 		dClipCurPos[0] = COMI.Get_MotCurrentPos(M_CLIP_BUFFER);
		nRet1 = CTL_Lib.Single_Move(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].md_pos[LD_CLIP_SAFETY], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4500;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2323, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 4500:
		nRet1 = COMI.Check_SingleMove(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].md_pos[LD_CLIP_SAFETY]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4600;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2323, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 4600:// 		dClipCurPos[1] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
		nRet1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4700;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2353, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 4700:
		nRet1 = COMI.Check_SingleMove(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4800;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2353, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 4800:// 		dClipCurPos[3] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
		nRet1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 4900;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2340, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 4900:
		nRet1 = COMI.Check_SingleMove(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 5000;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2340, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;
	
	case 5000:// 		dClipCurPos[2] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
		nRet1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 5100;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2341, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 5100:
		nRet1 = COMI.Check_SingleMove(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_LD_SAFETY]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 5200;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2341, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;		

	case 5200:
		st_work.mn_ClipRobot_Reinstatement_Ready = YES;
		ReinstateStep = 5300;
		break;

	case 5300:	
		if(st_work.mn_WorkBufferRobot_Reinstatement_Ok == YES)
		{
			ReinstateStep = 6000;
		}
		break;

	case 6000:
		nRet1 = CTL_Lib.Single_Move(M_CLIP_BUFFER, st_work.md_reinstatement_Pos[1][M_CLIP_BUFFER], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 6100;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(2323, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}			

		break;

	case 6100:
		nRet1 = COMI.Check_SingleMove(M_CLIP_BUFFER, st_work.md_reinstatement_Pos[1][M_CLIP_BUFFER]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 6200;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(2323, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 6200:
//		st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Y] = 392.395;
		nRet1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Y, st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Y], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 6300;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(2341, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}			
		break;

	case 6300:
		nRet1 = COMI.Check_SingleMove(M_M_CLAMP_RBT_Y, st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Y]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 6400;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2341, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;


	case 6400:
		nRet1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Z], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 6500;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(2384, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}			
		break;

	case 6500:
		nRet1 = COMI.Check_SingleMove(M_M_CLAMP_RBT_Z, st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Z]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 6600;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2384, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;


	case 6600:
		nRet1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_GRIPPER], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 6700;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(2388, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}			

	case 6700:
		nRet1 = COMI.Check_SingleMove(M_M_CLAMP_RBT_GRIPPER, st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_GRIPPER]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 6800;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2353, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}

		break;

	case 6800:
		nRet1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Y], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 6900;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(2321, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}			
		break;

	case 6900:
		nRet1 = COMI.Check_SingleMove(M_CLIP_INSERT_Y, st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Y]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 7000;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2321, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 7000:
		// 	st_work.mn_ClipBufferStatusBackup[0] = FAS_IO.get_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		if(st_work.mn_ClipBufferStatusBackup[0] == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
			ReinstateStep = 7100;		
		}
		else
		{
			ReinstateStep = 7200;
		}
		break;

	case 7100:
		nRet1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet1 == RET_GOOD)
		{
			ReinstateStep = 7200;
		}
		else if (nRet1 == RET_ERROR)
		{
			ReinstateStep = 7100;
		}
		break;
	

	case 7200:
		nRet1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Z], st_basic.nRunSpeed);
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 7300;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(2377, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}			
		break;
		
	case 7300:
		nRet1 = COMI.Check_SingleMove(M_CLIP_INSERT_Z, st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Z]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 7400;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(2377, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 7400:
		// 	st_work.mn_ClipInsertStatusBackup[0] = FAS_IO.get_out_bit(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		if(st_work.mn_ClipInsertStatusBackup[0] == IO_ON)
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_ON);
			ReinstateStep = 7500;		
		}
		else
		{
			ReinstateStep = 50000;
		}
		break;

	case 7500:
		nRet1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_dn_chk, st_io.i_clip_rbt_up_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet1 == RET_GOOD)
		{
			ReinstateStep = 50000;
		}
		else if (nRet1 == RET_ERROR)
		{
			ReinstateStep = 7500;
		}
		break;

	case 20000:
		st_handler.n_sync_reinstate = NO;
		ReinstateStep = 0;
		break;

	case 50000:
		st_work.mn_ClipRobot_Reinstatement_Ready = YES;
		st_work.mn_ClipRobot_Reinstatement_Ok = YES;//20121126
		ReinstateStep = 0;
		break;
	}	
}

void CRun_Rbt_Load_Clip::Thread_Variable_Initial()
{
	int i;
	mn_NextPos = 0;
	InitStep = 0;
	RunInsertStep = 0;
	RunClampStep = 0;
	DropStep = 0;
	LdClipStep = 0;
	ClipLotStep = 0;
	PickClipStep = 0;
	UldClipStep= 0;
	st_sync.n_lotend_clip_ldrbt = NO;
	st_sync.n_lotend_clamp_ldrbt = NO;
	st_sync.n_lotend_clipbuffer_ldrbt = NO;
	st_sync.i_remove_clip_bin = NO;
	st_sync.i_load_clip = NO;
	st_sync.i_unload_clip = NO;
	st_sync.n_lotend_clip_Lot = NO;
	st_sync.n_Start_Stop_flag = FALSE;
	
	st_modulemap.nClipRobotPicker[D_EXIST] = DVC_NO;
	st_modulemap.nClipRobotPicker[D_BIN] = BIN_CLEAR;
	st_modulemap.nClipRobotPicker[D_RE_CNT] = T_NOTTEST;
	st_modulemap.nClipRobotPicker[D_SCR_BIN] = BIN_CLEAR;

	st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;

	for (i = 0; i < 4; i++)
	{
		st_sync.i_WorkClamp_Clip[i] = NO;
		st_sync.i_WorkBuffer_Clip[i] = NO;
	}
	st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] = -1;
	st_sync.n_module_buffer_action_site[LDCLIP_SITE][1] = -1;

	ReinstateStep = 0;
	st_work.mn_ClipRobot_Reinstatement_Ok = NO;
	st_work.mn_ClipRobot_Reinstatement_Ready = NO;

}

void CRun_Rbt_Load_Clip::Run_Init()
{
	int nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;

	if(st_handler.mn_init_state[INIT_RBT_CLIP_MODULE] != CTL_NO) return;

	switch(InitStep)
	{
	case 0:
		InitStep = 10;
		break;

	case 10:
		if(FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_OFF) == IO_ON ||
			FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON) == IO_OFF)
		{
			Clip_remove_Cnt = 0;//2014,0228
			FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
			InitStep = 100;
		}
		else
		{
			InitStep = 1000;
		}
		break;

	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			//260100 0 26 "Clip Insert Picker up 에러."
			mn_retry = 0;
			sprintf(mc_alarmcode,"260100");
			CTL_Lib.Alarm_Error_Occurrence(2300, CTL_dWARNING, mc_alarmcode);
			InitStep = 10;
		}
		break;

	case 200:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		InitStep = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		
		if (nRet_1 == RET_GOOD)
		{
			InitStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180801");
			CTL_Lib.Alarm_Error_Occurrence(2305, CTL_dWARNING, mc_alarmcode);

		}
		break;

	case 1000:
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].mn_homecheck_method, MOT_TIMEOUT);

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
			CTL_Lib.Alarm_Error_Occurrence(2302, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1100:
		ActionPicker[D_EXIST] = YES;
		Set_PickerUpDown(IO_OFF, ActionPicker);
		InitStep = 1200;
		break;

	case 1200:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		nRet_1 = RET_GOOD;
		if (nRet_1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			InitStep = 2000;
		}
		else if (nRet_1 == CTLBD_RET_ERROR)
		{
			sprintf(mc_alarmcode,"260100");
			CTL_Lib.Alarm_Error_Occurrence(2303, CTL_dWARNING, mc_alarmcode);
		}
		break;

	case 2000:
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_retry = 0;
			InitStep = 2300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2304, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;
		
	case 2300:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		InitStep = 2400;
		break;

	case 2400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if (nRet_1 == RET_GOOD)
		{
			InitStep = 2500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180801");
			CTL_Lib.Alarm_Error_Occurrence(2305, CTL_dWARNING, mc_alarmcode);

		}
		break;

	case 2500:
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 2600;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2500;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2306, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2600:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 2700;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2600;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2307, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2700:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 2800;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2700;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2308, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2800:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 2810;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2800;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2810:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE1], st_basic.nManualSpeed);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 2900;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2810;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2900:
//		nRet_1 = Run_RemoveClip();
//		if(nRet_1 == RET_GOOD)
//		{
			InitStep = 2905;
//		}
		break;

	case 2905:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 2910;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2905;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;


	case 2910:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF);
		if(nRet_1 == IO_ON || nRet_2 == IO_ON)
		{
			InitStep = 2920;
		}
		else
		{
			InitStep = 2960;
		}
		break;

	case 2920:
		nRet_1 = UnloadClipMove();
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 2930;
		}
		break;

	case 2930:
		nRet_1 = PickUpClipForBin();
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 2940;
		}
		break;

	case 2940:
//		nRet_1 = Run_RemoveClip();
//		if(nRet_1 == RET_GOOD)
//		{
			InitStep = 2950;
//		}
		break;

	case 2950:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF);

		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{
			InitStep = 2960;
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
			CTL_Lib.Alarm_Error_Occurrence(2309, st_work.mn_run_status, mc_alarmcode);
			InitStep = 2910;
		}
		break;

	case 2960:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_PICKPOS], st_basic.nManualSpeed);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitStep = 3000;
			InitStep = 2970;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2810;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2970:
		nRet_1 = Run_RemoveClip();
		if(nRet_1 == RET_GOOD)
		{
			InitStep = 3000;
		}
		break;
		
	case 3000:
		st_handler.mn_init_state[INIT_RBT_CLIP_MODULE] = CTL_YES;
		InitStep = 0;
		break;
	}
}

//Clip을 Clip 버퍼에 넣은 후 Clamp Module Rbt, CLip Insert Rbt 잘 움직여서 제어할것
void CRun_Rbt_Load_Clip::Run_ClipInsertMove()
{
	int Val_1 = 0, Val_2 = 0, nRet_1 = 0, nRet_2 = 0, nRet_3 = 0, flag = 0;
	double dCurrPos;
	int i = 0;
	CString strTmp;//2014.0722 ysj

//	if(alarm.n_area_ready_alarm[1] == TRUE)
//	{
//		COMI.Set_MotStop(1, M_CLIP_INSERT_Z);
//		COMI.Set_MotStop(1, M_CLIP_INSERT_Y);
//		CTL_Lib.mn_single_motmove_step[M_CLIP_INSERT_Z] = 0;
//		CTL_Lib.mn_single_motmove_step[M_CLIP_INSERT_Y] = 0;
//		return;
//	}

	Func.ThreadFunctionStepTrace(10, RunInsertStep);
	switch(RunInsertStep)
	{
	case -1:
		Thread_Variable_Initial();
		st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_NO;
		ClipPickStep = 0;
		RunInsertStep = 0;
		break;
		
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_clip_ldrbt == CTL_NO)
			{
				RunInsertStep = 100;
			}
		}
		break;
		
	case 100:
		Func.Robot_BackData_Set(M_CLIP_INSERT_Z,Z_LD_SAFETY_UP);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			RunInsertStep = 200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			RunInsertStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(2320, CTL_dWARNING, COMI.mc_alarmcode);
			RunInsertStep = 100;
		}
		break;
		

	case 200:
		Func.Robot_BackData_Set(M_CLIP_INSERT_Z,Z_LD_SAFETY_UP);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			RunInsertStep = 300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunInsertStep = 200;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2321, CTL_dWARNING, COMI.mc_alarmcode);
			RunInsertStep = 200;
		}
		break;

	case 210:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		RunInsertStep = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if (nRet_1 == RET_GOOD)
		{
			RunInsertStep = 400;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(2322, CTL_dWARNING, mc_alarmcode);

				RunInsertStep = 210;
			}
			else
			{
				RunInsertStep = 210;
			}
		}
		break;

	case 400:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].md_pos[LD_CLIP_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunInsertStep = 500;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunInsertStep = 600;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunInsertStep = 400;
			CTL_Lib.Alarm_Error_Occurrence(2323, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 500:
		if(FAS_IO.get_in_bit(st_io.i_clip_buff_dn_chk, IO_ON) == IO_ON && 
			FAS_IO.get_in_bit(st_io.i_clip_buff_up_chk, IO_OFF) == IO_OFF)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			RunInsertStep = 600;
		}
		else
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			RunInsertStep = 600;
		}
		break;

	case 510:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		RunInsertStep = 900;
		break;

	case 600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if (nRet_1 == RET_GOOD)
		{
			st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_REQ; //Clip Buffer 로딩 자재 요청 
			RunInsertStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(2324, CTL_dWARNING, mc_alarmcode);

				RunInsertStep = 510;
			}
			else
			{
				RunInsertStep = 510;
			}
		}
		break;

	case 1000:
		if(st_sync.n_lotend_clipbuffer_ldrbt == CTL_YES)
		{
			RunInsertStep = 10000;
		}
		if(st_sync.n_lotend_clip_ldrbt == CTL_NO)
		{
			RunInsertStep = 1100;
		}
		if(COMI.mn_simulation_mode == 1)
		{
			st_ani.nSelectMotPos[M_CLIP_INSERT_Y] = 0;
		}
		break;

	case 1100:		
		//if((st_work.n_loadlot_count[LDMODULE_SITE] >= st_work.nMdlInputCount[0][0]) && st_work.n_lotend != CTL_YES && 
		//	Func.ChkForLotEnd(LDCLIP_SITE) >= RET_PROCEED)
		if(st_work.n_lotend != CTL_YES || Func.ChkForLotEnd(LDCLIP_SITE) >= RET_PROCEED)
		{
			mn_pre_move_flag = CTL_NO;
			mn_pre_insert_flag = CTL_NO;

			RunInsertStep = 1200;
		}
		else
		{
//			st_sync.n_lotend_clip_ldrbt = CTL_YES;
			RunInsertStep = 10000;
		}
		break;

	case 1200:
		md_ClipAbsPos = Func.XYRobot_MovePosValCheck(LDCLIP_SITE, 0, LD_CLIP_POS);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, md_ClipAbsPos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunInsertStep = 1210;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunInsertStep = 1200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2325, CTL_dWARNING, COMI.mc_alarmcode);
			RunInsertStep = 1200;
		}
		break;

	case 1210:
		//if(st_sync.n_module_ldrbt_clipbuffer_req[0] == CTL_CHANGE)
		//{
		//	st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_LOCK;
		
		st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] = -1;
		RunInsertStep = 1250;		
		break;

	case 1250:
 		if(st_work.mn_ClipRobot_Reinstatement_Ok == YES || st_work.n_multilot_start == CTL_NO) // 20130416
 		{
 			nRet_3 = Func.CLIPFindNode();		
 			if(nRet_3 != -1)
 			{
 				Func.ULDInsertNode(nRet_3);
				st_basic.n_clip_node = nRet_3;//2014.0818
 			}
 			Func.CLIPDelete_Node();
 			HeadVal = nRet_3;

			////2014.0722 ysj
//			strTmp.Format("Run_Rbt_Load_Clip - CLIPFindNode : %d", HeadVal);
//			Func.On_LogFile_Add(99, strTmp);
			////
		}

		RunInsertStep = 1300;
		break;

	case 1300:
 		if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] == -1)
		{
			st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] = -1; // 20130416
		
			if(st_sync.n_module_site_status[LDMODULE_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
				st_sync.n_module_site_status[LEFTSINK_SITE][HeadVal][0] == BUFF_DVC_LOAD && 
				st_sync.n_module_site_status[RIGHTSINK_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
				st_sync.n_module_site_status[LDCLIP_SITE][HeadVal][0] == BUFF_EMPTY)
				{
					st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] = HeadVal;
					st_sync.n_module_buffer_action_site[LDCLIP_SITE][1] = HeadVal;
					st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][HeadVal][0] = CTL_REQ;
					break;
				}
		}
		
		if(st_sync.n_module_ldrbt_clipbuffer_req[0] == CTL_FREE)
		{
			RunInsertStep = 2000;

			if(HeadVal == -1)
			{
				RunInsertStep = 1250;
			}

			if(st_work.n_lotend == CTL_YES && Func.ChkForLotEnd(LDCLIP_SITE) == RET_ERROR)
			{
				RunInsertStep = 10000;
			}
		}
		break;
		
	case 2000:
		dCurrPos = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
		if(dCurrPos > st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_POS] - st_motor[M_CLIP_INSERT_Y].mn_allow &&
			dCurrPos < st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_POS] + st_motor[M_CLIP_INSERT_Y].mn_allow)
		{
			st_sync.n_module_ldrbt_clamp_req[0] = CTL_REQ;
			RunInsertStep = 2100;
		}
		else
		{
			RunInsertStep = 2010;
		}		
		break;

	case 2010:
		md_ClipAbsPos = Func.XYRobot_MovePosValCheck(LDCLIP_SITE, 0, LD_CLIP_POS);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, md_ClipAbsPos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			st_sync.n_module_ldrbt_clamp_req[0] = CTL_REQ;
			RunInsertStep = 2100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunInsertStep = 2010;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunInsertStep = 2010;
			CTL_Lib.Alarm_Error_Occurrence(2326, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2100:
		//PickupClip
		nRet_1 = OnPickupClip(LD_CLIP_POS);
		if(nRet_1 == RET_GOOD)
		{
			RunInsertStep = 2200;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			RunInsertStep = 2110;
		}
		break;

	case 2110:
		st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_REQ; //Clip Buffer 로딩 자재 요청 
		RunInsertStep = 2120;
		break;

	case 2120:
		if(st_sync.n_module_ldrbt_clipbuffer_req[0] == CTL_FREE)
		{
			RunInsertStep = 2130;
		}
		break;

	case 2130:
		md_ClipAbsPos = Func.XYRobot_MovePosValCheck(LDCLIP_SITE, 0, LD_CLIP_POS);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, md_ClipAbsPos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunInsertStep = 2100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunInsertStep = 2010;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunInsertStep = 2010;
			CTL_Lib.Alarm_Error_Occurrence(2326, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2200:
		st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_REQ; //Clip Buffer 로딩 자재 요청 
		RunInsertStep = 2300;
		break;

//		if(st_sync.n_module_ldrbt_clamp_req[0] == CTL_READY)
//		{
//			RunInsertStep = 2300;
//		}
/*		break;*/

	case 2300:
// 		if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] != -1) // 20130416
// 		{
// 			RunInsertStep = 3000;
// 			break;
// 		}

		st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] = -1;
		if(st_sync.n_module_site_status[LDMODULE_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[LEFTSINK_SITE][HeadVal][0] == BUFF_DVC_LOAD && 
			st_sync.n_module_site_status[RIGHTSINK_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[LDCLIP_SITE][HeadVal][0] == BUFF_EMPTY)
			{
				st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] = HeadVal;
				st_sync.n_module_buffer_action_site[LDCLIP_SITE][1] = HeadVal;
				st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][HeadVal][0] = CTL_REQ;
				mn_pre_move_flag = CTL_NO;
				RunInsertStep = 3000;
			}

		if(HeadVal == -1)
		{
			RunInsertStep = 1250;
		}

		if(st_work.n_lotend == CTL_YES && Func.ChkForLotEnd(LDCLIP_SITE) == RET_ERROR)
		{
			RunInsertStep = 10000;
		}
		
		break;
		
	case 3000:
		//2012,0817 if(st_sync.n_module_ldrbt_clamp_req[0] == CTL_READY)
		//{
		if(COMI.mn_simulation_mode == 1)
		{
			st_ani.nSelectMotPos[M_CLIP_INSERT_Y] = st_sync.n_module_buffer_action_site[LDCLIP_SITE][1] + 2;
			
			l_ClipWaitTime[0] = GetCurrentTime();
			
// 			st_sync.n_module_ldrbt_clamp_req[0] = CTL_FREE;
			RunInsertStep = 3010;
		}
		else
		{
			RunInsertStep = 3010;
		}
		//}
		break;
		
	case 3010:
 		nRet_1 = OnPlaceClip(st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]);
		if(nRet_1 == RET_GOOD)
		{
			st_sync.n_module_ldrbt_clamp_req[0] = CTL_FREE;
			
			if(COMI.mn_simulation_mode == 1)
			{
				RunInsertStep = 3050;
			}
			else
			{
				RunInsertStep = 3100;
			}
		}
		break;

	case 3050:
		if(COMI.mn_simulation_mode == 1)
		{
			l_ClipWaitTime[1] = GetCurrentTime();
			l_ClipWaitTime[2] = l_ClipWaitTime[1] - l_ClipWaitTime[0];

			st_ani.nSelectMotPos[M_CLIP_INSERT_Z] = 2;

			if(l_ClipWaitTime[2] < 0)
			{
				l_ClipWaitTime[0] = GetCurrentTime();
				break;
			}
			else if(l_ClipWaitTime[2] > 1000)
			{
				st_ani.nSelectMotPos[M_CLIP_INSERT_Z] = 0;

				RunInsertStep = 3100;
			}
		}
		break;

	case 3100:
		if(st_sync.n_module_ldrbt_clamp_req[0] == CTL_NOTREADY)
		{
			RunInsertStep = 1000;
		}
		break;

	case 10000:
		st_sync.n_lotend_clip_ldrbt = CTL_YES;
		RunInsertStep = 0;
		break;
	}
}

void CRun_Rbt_Load_Clip::Run_ClipBufferMove()
{
	int nRet_1,nRet_2;
	Func.ThreadFunctionStepTrace(11, RunBufferStep);

//	if(alarm.n_area_ready_alarm == TRUE)
//	{
//		COMI.Set_MotStop(1, M_CLIP_BUFFER);
//		CTL_Lib.mn_single_motmove_step[M_CLIP_BUFFER] = 0;
//		return;
//	}

	switch(RunBufferStep)
	{
	case -1:
		st_sync.n_module_clip_req[0] = CTL_NO;
		RunBufferStep = 100;
		break;

	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_clipbuffer_ldrbt == CTL_NO)
			{
				RunBufferStep = 100;
			}
		}
		break;

	case 100:
		if(st_sync.n_lotend_clip_ldrbt == CTL_YES)
		{
			RunBufferStep = 10000;
		}
		else if(st_sync.n_lotend_clip_ldtray_site == CTL_YES)
		{
			RunBufferStep = 10000;
		}
		else if(st_sync.n_module_ldrbt_clipbuffer_req[0] == CTL_REQ)
		{
			st_sync.n_module_clip_req[0] = CTL_REQ;
			RunBufferStep = 1000;
		}
		break;

	case 1000:
		if(st_sync.n_ld_clip_tray_supply != CTL_LOCK) return;
		nRet_1 = LdUldClipBufferPos(LD_CLIP_POS);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
			RunBufferStep = 1100;
		}
		break;

	case 1010:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		RunBufferStep = 1100;
		break;

	case 1100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			RunBufferStep = 1200;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(2330, CTL_dWARNING, mc_alarmcode);
				RunBufferStep = 1010;
			}			
			else
			{
				RunBufferStep = 1010;
			}
		}
		break;

	case 1200:
		st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_READY;
		RunBufferStep = 2000;
		break;

	case 2000:
		if (st_sync.n_module_clip_req[0] == CTL_FREE)
		{
//			if(st_sync.n_module_ldrbt_clipbuffer_req[0] == CTL_LOCK)
//			{
//				st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_FREE;
//				RunBufferStep = 100;
//			}

			//CTL_LOCK이 동시 접근 체크 요망
			if(st_sync.n_module_ldrbt_clipbuffer_req[0] == CTL_CHANGE)
			{
				//st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_FREE;
				RunBufferStep = 2100;
			}
		}
		else if(st_sync.n_module_clip_req[0] == CTL_CHANGE)
		{
			RunBufferStep = 2010;
		}
		else if(st_sync.n_lotend_clip_ldrbt == CTL_YES)
		{
			RunBufferStep = 100;
		}
		break;

	case 2010:
		st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_REQ;
		st_sync.n_module_clip_req[0] = CTL_REQ;
		RunBufferStep = 1000;
		break;

	case 2100:
		nRet_1 = LdUldClipBufferPos(ULD_CLIP_POS);
		if(nRet_1 == RET_GOOD)
		{
			st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_LOCK;
			RunBufferStep = 2200;
		}
		break;

	case 2200:
		RunBufferStep = 2300;//2014,0304
		break;
		
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_ON);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_ON);
		if(st_basic.n_mode_device != 1) nRet_1 = nRet_2 = IO_ON;
		if(nRet_1 == IO_OFF || nRet_2 == IO_OFF)
		{//180802 0 18 "Clip buffer clip#1 Clip이 체크가 되지 않습니다."
		 //180803 0 18 "Clip buffer clip#2 Clip이 체크가 되지 않습니다."
			if(nRet_1 == IO_OFF) sprintf(mc_alarmcode,"180802");
			else                sprintf(mc_alarmcode,"180803"); 
			CTL_Lib.Alarm_Error_Occurrence(2331, CTL_dWARNING, mc_alarmcode);
		}
		else
		{
			RunBufferStep = 2300;
		}
		break;

	case 2300:
		if(st_sync.n_module_ldrbt_clipbuffer_req[0] == CTL_LOCK)
		{
			st_sync.n_module_ldrbt_clipbuffer_req[0] = CTL_FREE;
			RunBufferStep = 3000;
		}
		break;

	case 3000:
		RunBufferStep = 100;
		break;

	case 10000:
		st_sync.n_lotend_clipbuffer_ldrbt = CTL_YES;
		RunBufferStep = 0;
		break;

	}	
}

void CRun_Rbt_Load_Clip::Run_ClipLotEndMove()
{
	int nRet_1;
	Func.ThreadFunctionStepTrace(80, ClipLotStep);
	switch(ClipLotStep)
	{
	case 0:
		if(st_sync.n_lotend_clip_Lot == NO)
		{
			ClipLotStep = 100;
		}
		break;

	case 100:
		if(st_sync.n_lotend_clip_ldrbt == CTL_YES && //st_sync.n_lotend_clipbuffer_ldrbt == CTL_YES && 
			st_sync.n_lotend_clamp_ldrbt == CTL_YES && st_sync.n_lotend_clip_ldtray_site == CTL_YES)
		{
			ClipLotStep = 1000;
		}
		break;

	case 1000:
		if(st_sync.i_remove_clip_bin == CTL_REQ)
		{
			ClipLotStep = 2000;
		}
		else if(st_sync.i_load_clip == CTL_REQ)
		{
			ClipLotStep = 3000;
		}
		else if(st_sync.i_unload_clip == CTL_REQ)
		{
			ClipLotStep = 4000;
		}
		else if(st_sync.i_pickup_clip == CTL_REQ)
		{
			ClipLotStep = 5000;
		}
		else if(st_sync.n_lotend_clip_ldstacker_site == YES)
		{
			st_sync.n_lotend_clip_Lot = YES;
			ClipLotStep = 0;
		}
		break;

	case 2000:
		nRet_1 = Run_RemoveClip();
		if(nRet_1 == RET_GOOD)
		{
			ClipLotStep = 2100;
		}
		break;

	case 2100:
// 		if(st_basic.n_curr_clamp_clip_bin >= st_basic.n_clamp_clip_bin)
// 		{//995000 0 99 "Clamp에서 Clip 제거 Bin 통을 제거해 주세요."
// 			sprintf(mc_alarmcode,"995000"); 
// 			CTL_Lib.Alarm_Error_Occurrence(2332, CTL_dWARNING, mc_alarmcode);
// 		}
// //		else if(st_basic.n_curr_clip_bin >= st_basic.n_clip_bin)
// //		{//995001 0 99 "Clip 제거 Bin 통을 제거해 주세요."
// //			sprintf(mc_alarmcode,"995001"); 
// //			CTL_Lib.Alarm_Error_Occurrence(2333, CTL_dWARNING, mc_alarmcode);
// //		}
// 		else
// 		{
			ClipLotStep = 1000;
// 		}
		break;

	case 3000:
		nRet_1 = LoadClipMove();
		if(nRet_1 == RET_GOOD)
		{
			ClipLotStep = 1000;
		}
		break;

	case 4000:
		nRet_1 = UnloadClipMove();
		if(nRet_1 == RET_GOOD)
		{
			ClipLotStep = 1000;
		}
		break;
		
	case 5000:
		nRet_1 = PickUpClipForBin();
		if(nRet_1 == RET_GOOD)
		{
			ClipLotStep = 1000;
		}
		break;
	}

}

int CRun_Rbt_Load_Clip::LdUldClipBufferPos(int nPos)
{// nPos : 0 safety 1 : ld 2 : uld  
	int nRet_1,nFuncRet = RET_PROCEED;

	Func.ThreadFunctionStepTrace(12, ClipBufferStep);
	switch(ClipBufferStep)
	{
	case 0:
		if(FAS_IO.get_in_bit(st_io.i_clip_buff_up_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_clip_buff_dn_chk, IO_ON) == IO_ON)
		{
			ClipBufferStep = 1000;
		}
		else
		{
			mn_Clipretry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			ClipBufferStep = 100;
		}
		break;

	case 10:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		ClipBufferStep = 100;
		break;

	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			ClipBufferStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(2332, CTL_dWARNING, mc_alarmcode);
				ClipBufferStep = 10;
			}
			else
			{
				ClipBufferStep = 10;
			}
		}
		break;

	case 1000:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].md_pos[nPos], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipBufferStep = 2000;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipBufferStep = 1000;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			ClipBufferStep = 1000;
			CTL_Lib.Alarm_Error_Occurrence(2333, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2000:
		ClipBufferStep = 0;
		nFuncRet = RET_GOOD;
		break;
	}

	return nFuncRet;
}

void CRun_Rbt_Load_Clip::Run_ModuleClampMove()
{
	int Val_1 = 0, Val_2 = 0, nRet_1 = 0, nRet_2 = 0, nRet_3 = 0, flag = 0;
	double dCurrPos1, dCurrPos2;
	int i = 0;

//	if(alarm.n_area_ready_alarm == TRUE)
//	{
//		COMI.Set_MotStop(1, M_M_CLAMP_RBT_Z);
//		COMI.Set_MotStop(1, M_M_CLAMP_RBT_Y);
//		CTL_Lib.mn_single_motmove_step[M_M_CLAMP_RBT_Z] = 0;
//		CTL_Lib.mn_single_motmove_step[M_M_CLAMP_RBT_Y] = 0;
//		return;
//	}
	Func.ThreadFunctionStepTrace(13, RunClampStep);
	switch(RunClampStep)
	{
	case -1:
		ClampRbtStep = 0;
		st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;
		RunClampStep = 100;
		break;
		
	case 0:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_clamp_ldrbt == CTL_NO)
			{
				RunClampStep = 100;
			}
		}
		break;
		
	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunClampStep = 300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunClampStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunClampStep = 100;
			CTL_Lib.Alarm_Error_Occurrence(2340, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{

			RunClampStep = 500;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunClampStep = 300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunClampStep = 300;
			CTL_Lib.Alarm_Error_Occurrence(2341, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 500:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunClampStep = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunClampStep = 500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunClampStep = 500;
			CTL_Lib.Alarm_Error_Occurrence(2342, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1000:
		if(st_work.mn_lot_start == CTL_YES) //Lot이 시작되었으면 시작한다 
		{
			if(st_sync.n_lotend_clamp_ldrbt == CTL_NO)
			{
				RunClampStep = 1100;
			}
		}
		break;

	case 1100:
		if(st_sync.n_lotend_clip_ldrbt == CTL_YES)
		{
			RunClampStep = 10000;
		}
		if(st_sync.n_module_ldrbt_clamp_req[0] == CTL_REQ)
		{			
			if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] != -1)
			{
				mn_pre_clamp_flag = CTL_NO;
				RunClampStep = 1200;
			}
		}
		if(COMI.mn_simulation_mode == 1)
		{
			st_ani.nSelectMotPos[M_M_CLAMP_RBT_Z] = 0;
			st_ani.nSelectMotPos[M_M_CLAMP_RBT_Y] = st_sync.n_module_buffer_action_site[LDCLIP_SITE][1] + 1;
		}
		break;

	case 1200:
		if(st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]][1] == CTL_READY)
		{ 
			md_clampPos = 0.0;
			RunClampStep = 2000;
		}
		else//대기한다
		{
			if(mn_pre_move_flag == CTL_YES)
			{
				if(mn_pre_clamp_flag == CTL_NO)
				{
					RunClampStep = 1300;
				}
			}
		}
		break;

	case 1300:
		dCurrPos1 = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
		dCurrPos2 = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
		if(dCurrPos1 > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_CLAMP_RBT_Z].mn_allow)
		{
			RunClampStep = 1400;
		}
		else
		{
			if(dCurrPos2 < st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY] - st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow && 
				dCurrPos2 > st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY] + st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow)
			{
				RunClampStep = 1600;
			}
			else
			{
				RunClampStep = 1500;
			}
		}
		break;

	case 1400:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunClampStep = 1300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunClampStep = 1400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunClampStep = 1400;
			CTL_Lib.Alarm_Error_Occurrence(2343, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1500:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunClampStep = 1300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunClampStep = 1500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunClampStep = 1500;
			CTL_Lib.Alarm_Error_Occurrence(2344, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1600://0702
		//nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLIP_WORK_PLACE1+mn_NextPos-1], st_basic.nRunSpeed);
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE1+mn_NextPos], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunClampStep = 1700;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunClampStep = 1600;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunClampStep = 1600;
			CTL_Lib.Alarm_Error_Occurrence(2345, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1700:
		if(st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]][1] == CTL_READY)
		{
			md_clampPos = 0.0;
			RunClampStep = 2000;
		}
		break;

	case 2000:
		nRet_1 = OnClampRbtMove(st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]);
		if(nRet_1 == RET_GOOD)
		{
			st_handler.mn_remain_count = st_sync.n_module_buffer_action_site[LDCLIP_SITE][1];
			st_sync.n_module_ldrbt_clamp_req[0] = CTL_READY;			
			RunClampStep = 2100;

			if(COMI.mn_simulation_mode == 1)
			{
				st_ani.nSelectMotPos[M_M_CLAMP_RBT_Z] = 1;
			}
		}
		break;

	case 2100:
		if(st_sync.n_module_ldrbt_clamp_req[0] == CTL_FREE)
		{
			st_sync.n_module_ldrbt_clamp_req[0] = CTL_NOTREADY;
			RunClampStep = 2200;
		}		
		break;

	case 2200:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
		if(md_CurrentPos < st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY] + st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow)
		{
			RunClampStep = 2400;
		}
		else
		{
			mn_retry = 0;
			RunClampStep = 2210;
		}
		break;

	case 2210:
//		FAS_IO.set_out_bit(st_io.o_buf1_f_vacuum_onoff + (s t_sync.n_module_buffer_action_site[LDCLIP_SITE][1]*100), IO_OFF);
//		FAS_IO.set_out_bit(st_io.o_buf1_b_vacuum_onoff + (st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]*100), IO_OFF);
//		FAS_IO.set_out_bit(st_io.o_buf1_f_eject_onoff + (st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]*100), IO_ON);
//		FAS_IO.set_out_bit(st_io.o_buf1_b_eject_onoff + (st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]*100), IO_ON);
		FAS_IO.set_out_bit(st_io.o_buf1_f_vacuum_onoff + (st_handler.mn_remain_count*100), IO_OFF);
		FAS_IO.set_out_bit(st_io.o_buf1_b_vacuum_onoff + (st_handler.mn_remain_count*100), IO_OFF);
		FAS_IO.set_out_bit(st_io.o_buf1_f_eject_onoff + (st_handler.mn_remain_count*100), IO_ON);
		FAS_IO.set_out_bit(st_io.o_buf1_b_eject_onoff + (st_handler.mn_remain_count*100), IO_ON);
		l_ClipWaitTime[0] = GetCurrentTime();
		RunClampStep = 2220;
		break;

	case 2220:
		l_ClipWaitTime[1] = GetCurrentTime();
		l_ClipWaitTime[2] = l_ClipWaitTime[1] - l_ClipWaitTime[0];
		if(l_ClipWaitTime[2] < 500) break;
		else if(l_ClipWaitTime[2] < 0) l_ClipWaitTime[0] = GetCurrentTime();
//		nRet_1 = FAS_IO.get_in_bit(st_io.i_buf1_f_vacuum_chk + (st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]*100), IO_OFF);
//		nRet_2 = FAS_IO.get_in_bit(st_io.i_buf1_b_vacuum_chk + (st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]*100), IO_OFF);
		nRet_1 = FAS_IO.get_in_bit(st_io.i_buf1_f_vacuum_chk + (st_handler.mn_remain_count*100), IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_buf1_b_vacuum_chk + (st_handler.mn_remain_count*100), IO_OFF);
		nRet_2 = IO_OFF;
		if((nRet_1 == IO_OFF && nRet_2 == IO_OFF) || st_basic.n_mode_device != 1)
		{
			mn_retry = 0;
//			FAS_IO.set_out_bit(st_io.o_buf1_f_eject_onoff + (st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]*100), IO_OFF);
//			FAS_IO.set_out_bit(st_io.o_buf1_b_eject_onoff + (st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]*100), IO_OFF);
			FAS_IO.set_out_bit(st_io.o_buf1_f_eject_onoff + (st_handler.mn_remain_count*100), IO_OFF);
			FAS_IO.set_out_bit(st_io.o_buf1_b_eject_onoff + (st_handler.mn_remain_count*100), IO_OFF);
			RunClampStep = 2300;
		}
		else
		{
			mn_retry++;
			if(mn_retry < 3)
			{
				RunClampStep = 2210;
			}
			else
			{//550100 0 55 "Clip#1 Front Clamp Vacuum Error"	
			 //550104 0 55 "Clip#1 Rear Clamp Vacuum Error"
				if(nRet_1 == RET_ERROR) sprintf(mc_alarmcode,"55010%d",st_handler.mn_remain_count);
				else                    sprintf(mc_alarmcode,"55010%d",st_handler.mn_remain_count+4);					
				CTL_Lib.Alarm_Error_Occurrence(2346, CTL_dWARNING, mc_alarmcode);
				RunClampStep = 2210;
			}
		}
		break;

	case 2300:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunClampStep = 2400;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunClampStep = 2200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunClampStep = 2200;
			CTL_Lib.Alarm_Error_Occurrence(2347, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;


	case 2400:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			RunClampStep = 3000;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			RunClampStep = 2400;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			RunClampStep = 2400;
			CTL_Lib.Alarm_Error_Occurrence(2348, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 3000:
		
		//st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][nPos][1] = CTL_CHANGE;
//		st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]][0] = CTL_NO;
//		st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]][1] = CTL_CHANGE;
//		st_sync.n_module_ldrbt_clamp_req[0] = CTL_NOTREADY;
//		st_sync.i_WorkClamp_Clip[st_sync.n_module_buffer_action_site[LDCLIP_SITE][1]] = NO;
		st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][st_handler.mn_remain_count][0] = CTL_NO;
		st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][st_handler.mn_remain_count][1] = CTL_CHANGE;
		RunClampStep = 1100;
		break;

	case 10000:
		st_sync.n_lotend_clamp_ldrbt = CTL_YES;
		RunClampStep = 0;
		break;
	}
}

int CRun_Rbt_Load_Clip::OnPlaceClip(int nPos)
{
	int nFuncRet = RET_PROCEED;
	double dCurr[3];
	int nRet_1;

	dCurr[0] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
	dCurr[1] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
	dCurr[2] = COMI.Get_MotCurrentPos(M_M_CLIP_CLAMP);
	Func.ThreadFunctionStepTrace(14, ClipPlaceStep);

	switch(ClipPlaceStep)
	{
	case 0:
		if (nPos == WORK_BUFFER_1 || nPos == WORK_BUFFER_2 || nPos == WORK_BUFFER_3 || nPos == WORK_BUFFER_4)
		{
			mn_Clipretry = 0;
			ClipPlaceStep = 1;
		}
		else
		{
			//260100 0 26 "Clip Insert Picker up 에러."
			//260101 0 26 "Clip Insert Picker down 에러."
			sprintf(mc_alarmcode,"260100");
			CTL_Lib.Alarm_Error_Occurrence(2349, CTL_dWARNING, mc_alarmcode);
			ClipPlaceStep = 0;
		}
		break;

	case 1:
		nRet_1 = COMI.Start_SingleMove(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_MIDCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 2;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 1;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 1;
			CTL_Lib.Alarm_Error_Occurrence(2399, CTL_dWARNING, COMI.mc_alarmcode);				
		}
		break;
		
	case 2:
		if(dCurr[0] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Z].mn_allow)
		{
			ClipPlaceStep = 100;
		}
		else
		{
			if(FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_OFF ||
				FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_ON)
			{
				FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
				ClipPlaceStep = 100;
			}
			else
			{
				ClipPlaceStep = 1000;
			}
			//else if(dCurr[1] < st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY] - st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow || 
			//	dCurr[1] > st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY] + st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow)
			//{
			//	ClipPlaceStep = 300;
			//}
			//else
			//{
			//	ClipPlaceStep = 1000;
			//}
		}
		break;

	case 100:
		FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[LD_CLIP_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 100;
			CTL_Lib.Alarm_Error_Occurrence(2351, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == RET_GOOD)
		{
			ClipPlaceStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{//260100 0 26 "Clip Insert Picker up 에러."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"260100");
				CTL_Lib.Alarm_Error_Occurrence(2352, CTL_dWARNING, mc_alarmcode);
				ClipPlaceStep = 100;
			}
			else
			{
				ClipPlaceStep = 100;
			}
		}
		break;
		
	case 300:
		if(dCurr[1] > st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY] - st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow &&
			dCurr[1] < st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY] + st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow)
		{
			ClipPlaceStep = 1000;
		}
		else
		{
			nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nRunSpeed);
			if(nRet_1 == BD_GOOD)
			{
				ClipPlaceStep = 2;
			}
			else if(nRet_1 == BD_RETRY)
			{
				ClipPlaceStep = 300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				ClipPlaceStep = 300;
				CTL_Lib.Alarm_Error_Occurrence(2353, CTL_dWARNING, COMI.mc_alarmcode);
			}
		}
		break;

	case 1000:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y , st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE1+nPos], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 1100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 1000;
			CTL_Lib.Alarm_Error_Occurrence(2354, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1100:
		if(st_sync.n_module_ldrbt_clamp_req[0] != CTL_READY)
		{
			break;
		}
		
		if(dCurr[2] > st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_CLAMP] - st_motor[M_M_CLIP_CLAMP].mn_allow &&
			dCurr[2] < st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_CLAMP] + st_motor[M_M_CLIP_CLAMP].mn_allow)
		{
			ClipPlaceStep = 1110;//1200;
		}
		else
		{
			ClipPlaceStep = 1110;
		}
		break;

	case 1110:		
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_CLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 1200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 1110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 1110;
			CTL_Lib.Alarm_Error_Occurrence(2398, CTL_dWARNING, COMI.mc_alarmcode);				
		}
		break;

	case 1200://2 Work Buffer Clip down POS 0701
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[ULD_CLIP_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 1300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 1200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 1200;
			CTL_Lib.Alarm_Error_Occurrence(2356, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1300:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Set_PickerUpDown(IO_ON, ActionPicker);
			ClipPlaceStep = 1400;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 1300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 1300;
			CTL_Lib.Alarm_Error_Occurrence(2399, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1400:
		ActionPicker[D_EXIST] = YES;
		ClipPlaceStep = 1410;
		l_PickerWaitTime[0] = GetCurrentTime();//2014,0304
		break;

//	case 1405:
//		//nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_MIDCLAMP], st_basic.nRunSpeed);
//		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
//		if(nRet_1 == BD_GOOD)
//		{
//			ClipPlaceStep = 1410;
//		}
//		else if(nRet_1 == BD_RETRY)
//		{
//			ClipPlaceStep = 1405;
//		}
//		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
//		{
//			ClipPlaceStep = 1405;
//			CTL_Lib.Alarm_Error_Occurrence(2399, CTL_dWARNING, COMI.mc_alarmcode);
//
//		}
//		break;

	case 1410:
		nRet_1 = Get_PickerUpDown(IO_ON, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPlaceStep = 1500;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			ClipPlaceStep = 1300;//1400;
			st_work.mn_run_status = dWARNING;
		}
		break;

	case 1500:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME)break;
		ClipPlaceStep = 1510;
		break;

	case 1510:
		//nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_MIDCLAMP], st_basic.nRunSpeed);
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 1700;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 1510;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 1510;
			CTL_Lib.Alarm_Error_Occurrence(2398, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1600:
//		nRet_1 = COMI.Start_SingleMove(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_MIDCLAMP], st_basic.nRunSpeed);
		nRet_1 = COMI.Start_SingleMove(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPlaceStep = 1700;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 1600;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 1600;
			CTL_Lib.Alarm_Error_Occurrence(2357, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;	

	case 1700:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME) break;
		ClipPlaceStep = 1800;
		break;

	case 1800:
		ActionPicker[D_EXIST] = YES;
		Set_PickerUpDown(IO_OFF, ActionPicker);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPlaceStep = 1900;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 2100;
			CTL_Lib.Alarm_Error_Occurrence(2358, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1900:
		CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPlaceStep = 2000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
		}
		break;		

	case 2000:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0) l_PickerWaitTime[0] = GetCurrentTime();
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME) break;

		st_modulemap.WorkBuffer[D_EXIST][nPos][LDCLIP_SITE] = DVC_YES; //WORK_BUFFER_1 2 3 4
		st_modulemap.WorkBuffer[D_BIN][nPos][LDCLIP_SITE] = BIN_LDBUFFERBIN;		// BIN_LOADTRAYBIN
		st_modulemap.WorkBuffer[D_RE_CNT][nPos][LDCLIP_SITE] = T_NOTTEST;			// Test Count Reset
		st_modulemap.WorkBuffer[D_SCR_BIN][nPos][LDCLIP_SITE] = BIN_CLEAR;			// Screen Bin

		st_modulemap.nClipRobotPicker[D_EXIST]   = DVC_NO;
		st_modulemap.nClipRobotPicker[D_BIN]     = BIN_CLEAR;		// BIN_LOADTRAYBIN
		st_modulemap.nClipRobotPicker[D_RE_CNT]  = T_NOTTEST;		// Test Count Reset
		st_modulemap.nClipRobotPicker[D_SCR_BIN] = BIN_CLEAR;		// Screen Bin
		if(st_sync.n_module_site_status[LDCLIP_SITE][nPos][0] != BUFF_DVC_LOAD)
		{
			st_sync.n_module_site_status[LDCLIP_SITE][nPos][0] = BUFF_DVC_LOAD;
//			Func.CLIPDelete_Node();
		}

		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_CLIP1_PICKER);			// 피커 정보 
		}
		ClipPlaceStep = 3000;
		break;

		
	case 3000:	
		nFuncRet = RET_GOOD;
		ClipPlaceStep = 0;
	}

	return nFuncRet;
		
}

int CRun_Rbt_Load_Clip::OnPickupClip(int nPos)
{
	int nFuncRet = RET_PROCEED;
	double dCurr[3];
	int nRet_1,nRet_2;

	dCurr[0] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
	dCurr[1] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
	dCurr[2] = COMI.Get_MotCurrentPos(M_M_CLIP_CLAMP);
	Func.ThreadFunctionStepTrace(15, ClipPickStep);
	switch(ClipPickStep)
	{
	case 0:
		if (nPos == LD_CLIP_POS || nPos == ULD_CLIP_POS || nPos == LD_CLIP_SAFETY)
		{
			if(dCurr[0] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Z].mn_allow)
			{
				ClipPickStep = 0;
				ClipPickStep = 100;
			}
			else
			{
				if(dCurr[2] < st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP] + st_motor[M_M_CLIP_CLAMP].mn_allow)
				{
					if(FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_OFF ||
						FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_ON)
					{
						mn_Clipretry = 0;
						FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
						ClipPickStep = 10;
					}
					else
					{
						ClipPickStep = 1000;
					}
				}
				else
				{
					ClipPickStep = 300;
				}

			}
		}
		else
		{
			//260105 0 26 "Clip Pickup Position이 아닙니다."
			sprintf(mc_alarmcode,"260105");
			CTL_Lib.Alarm_Error_Occurrence(2359, CTL_dWARNING, mc_alarmcode);
			ClipPickStep = 0;
		}
		break;
		
	case 5:
		FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			ClipPickStep = 10;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ClipPickStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다.
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2360, CTL_dWARNING, COMI.mc_alarmcode);

			ClipPickStep = 100;
		}
		break;
		
	case 10:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == RET_GOOD)
		{
			ClipPickStep = 0;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{//260100 0 26 "Clip Insert Picker up 에러."
			 //180904 0 18 "Clamp rbt unclamp 에러가 발생했습니다."
				sprintf(mc_alarmcode,"260100");
				mn_Clipretry = 0;
				CTL_Lib.Alarm_Error_Occurrence(2370, CTL_dWARNING, mc_alarmcode);
				ClipPickStep = 5;
			}
			else
			{
				ClipPickStep = 5;
			}
		}
		break;
		

	case 100:
//		if(FAS_IO.get_in_bit(st_io.i_clip_rbt_clamp_chk, IO_OFF) == IO_OFF &&
//			FAS_IO.get_in_bit(st_io.i_clip_rbt_unclamp_chk, IO_ON) == IO_ON)
//		{
			ClipPickStep = 200;
//		}
//		else
//		{
//				sprintf(mc_alarmcode,"180904");
//				mn_Clipretry = 0;
//				CTL_Lib.Alarm_Error_Occurrence(2371, CTL_dWARNING, mc_alarmcode);
//				ClipPickStep = 5;
//		}
		break;

	case 200:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPickStep = 210;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPickStep = 200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPickStep = 100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2372, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 210:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_picker_rbt_updn_onoff, st_io.i_uld_picker_rbt_up_chk, st_io.i_uld_stacker1_dn_chk,
			IO_ON, st_wait.n_on_wait_time[50], st_wait.n_limit_wait_time[50]);
		if(nRet_1 == RET_GOOD)
		{
			ClipPickStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{//180903 0 18 "Clamp rbt clamp 에러가 발생했습니다."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"260100");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2373, CTL_dWARNING, mc_alarmcode);

				ClipPickStep = 100;
			}
			else
			{
				ClipPickStep = 100;
			}
		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPickStep = 0;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPickStep = 300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPickStep = 300;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2374, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;
		
	case 1000:		//Y_LD_SAFETY+LD_CLIP_POS = Y_LD_TRAY_START ->LD_CLIP_POS 로 이동
		if(st_work.mn_run_status != dINIT)
		{
			if(st_sync.n_module_ldrbt_clipbuffer_req[0] != CTL_FREE && 
				st_sync.n_module_ldrbt_clipbuffer_req[0] != CTL_READY && 
				st_sync.n_lotend_clamp_ldrbt != CTL_YES) break; //2012,0911
		}
		mn_Clipretry = 0;
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y , st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPickStep = 1200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPickStep = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPickStep = 1000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2375, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1100:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		ClipPickStep = 1200;
		break;

	case 1200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			mn_check[0] = IO_OFF;
			mn_check[1] = IO_OFF;
			ClipPickStep = 1210;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 2)
			{////180800 0 18 "Clip buffer Up 체크 에러가 발생했습니다."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"180800");
				CTL_Lib.Alarm_Error_Occurrence(2376, CTL_dWARNING, mc_alarmcode);
				ClipPickStep = 1100;
			}
			else
			{
				ClipPickStep = 1100;
			}
		}
		break;

	case 1210:
		//180804 0 18 "Clip buffer clip#1에 Clip이 존재합니다. 제거 해 주세요."
		//180805 0 18 "Clip buffer clip#2에 Clip이 존재합니다. 제거 해 주세요."
		mn_check[0] = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_ON);
		mn_check[1] = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_ON);

		if(mn_check[0] == IO_ON && mn_check[1] == IO_ON)
		{
			ClipPickStep = 1300;
		}
		else
		{
			ClipPickStep = 1300;
		}
		break;


	case 1300:		
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z , st_motor[M_CLIP_INSERT_Z].md_pos[LD_CLIP_POS], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipPickStep = 2000;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipPickStep = 1200;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			ClipPickStep = 1200;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2377, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;	

	case 2000:
		if(FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_ON)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPickStep = 2100;
		}
		else
		{
			ActionPicker[D_EXIST] = YES;
			Set_PickerUpDown(IO_OFF, ActionPicker);
			ClipPickStep = 2010;
		}
		break;

	case 2010:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPickStep = 2100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
		}
		break;

	case 2100:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME) break;
		mn_Clipretry = 0;
		ClipPickStep = 2200;
		break;

	case 2200:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_MIDCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPickStep = 2300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 2200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 2200;
			CTL_Lib.Alarm_Error_Occurrence(2378, CTL_dWARNING, COMI.mc_alarmcode);				
		}
		break;	

	case 2300:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME) break;

		ClipPickStep = 2400;
		break;

	case 2400:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z , st_motor[M_CLIP_INSERT_Z].md_pos[LD_CLIP_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipPickStep = 2500;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipPickStep = 2400;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			ClipPickStep = 2400;
			CTL_Lib.Alarm_Error_Occurrence(2379, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2500:
		//180804 0 18 "Clip buffer clip#1에 Clip이 존재합니다. 제거 해 주세요."
		//180805 0 18 "Clip buffer clip#2에 Clip이 존재합니다. 제거 해 주세요."
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF);

		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF && mn_check[0] == IO_ON && mn_check[1] == IO_ON)
		{
			ClipPickStep = 2900;
			Clip_remove_Cnt = 0;//2015.0130
		}
		else
		{//180804 0 18 "Clip buffer clip#1을 제거 해 주세요."
		 //180805 0 18 "Clip buffer clip#2을 제거 해 주세요."
		//260200 0 26 "Clip Insert Picker Clip이 존재하지 않습니다. 체크해 주세요."
			if(nRet_1 == IO_ON)
			{
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] Clip buffer clip#1을 제거 해 주세요.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"260200");
			}
			else
			{
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Error] Clip buffer clip#2을 제거 해 주세요.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"260200");
			}
			mn_Clipretry = 0;

			if(st_basic.n_mode_clip_alarm == YES)
			{
				CTL_Lib.Alarm_Error_Occurrence(2380, CTL_dWARNING, mc_alarmcode);				
			}
			//ClipPickStep = 0;
			ClipPickStep = 2510;//2012,0911
//			Clip_remove_Cnt = 0;//2014,0228
		}
		break;

	case 2510:
		nRet_1 = Run_RemoveClip();
		if(nRet_1 == RET_GOOD)
		{
			ClipPickStep = 2515;
			//2015.0130
			if(st_work.mn_run_status == dRUN)
			{
				Clip_remove_Cnt++;

				if (Clip_remove_Cnt > 5)
				{
					Clip_remove_Cnt = 0;
					//180806 1 18 "Clip load error(5ea) continusly."

					sprintf(mc_alarmcode,"180806");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1599, st_work.mn_run_status, mc_alarmcode);
				}
			}

		}
		////2014,0228
		//else 
		else if(nRet_1 != RET_PROCEED)//2015.0130
		{
			if(st_work.mn_run_status == dRUN)
			{
				Clip_remove_Cnt++;
				if (Clip_remove_Cnt > 5)
				{
					Clip_remove_Cnt = 0;

					nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF);
					nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF);

					if(nRet_1 == IO_ON || nRet_2 == IO_ON)
					{
					 //180804 0 18 "Clip buffer clip#1을 제거 해 주세요."
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
						CTL_Lib.Alarm_Error_Occurrence(1512, st_work.mn_run_status, mc_alarmcode);
					}
				}
			}
		}
		////
		break;

	case 2515:
// 		if(st_basic.n_curr_clamp_clip_bin >= st_basic.n_clamp_clip_bin)
// 		{//995000 0 99 "Clamp에서 Clip 제거 Bin 통을 제거해 주세요."
// 			sprintf(mc_alarmcode,"995000"); 
// 			CTL_Lib.Alarm_Error_Occurrence(2381, CTL_dWARNING, mc_alarmcode);
// 		}
// //		else if(st_basic.n_curr_clip_bin >= st_basic.n_clip_bin)
// //		{//995001 0 99 "Clip 제거 Bin 통을 제거해 주세요."
// //			sprintf(mc_alarmcode,"995001"); 
// //			CTL_Lib.Alarm_Error_Occurrence(2382, CTL_dWARNING, mc_alarmcode);
// //		}
// 		else
// 		{
			ClipPickStep = 2520;
//		}
		break;

	case 2520:
		nRet_1 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip1_chk, IO_OFF);
		nRet_2 = FAS_IO.get_in_bit(st_io.i_clip_buff_clip2_chk, IO_OFF);

		if(nRet_1 == IO_OFF && nRet_2 == IO_OFF)
		{			
			nFuncRet = RET_ERROR;

			ClipPickStep = 0;
		}
		else
		{
			ClipPickStep = 2530;
		}
		break;

	case 2530:
		if(dCurr[0] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Z].mn_allow)
		{
			ClipPickStep = 2550;
		}
		else
		{
			if(dCurr[2] < st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP] + st_motor[M_M_CLIP_CLAMP].mn_allow)
			{
				if(FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_OFF ||
					FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_ON)
				{
					mn_Clipretry = 0;
					FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
					ClipPickStep = 2540;
				}
				else
				{
					ClipPickStep = 2600;
				}
			}
			else
			{
				ClipPickStep = 2570;
			}

		}
		break;
		
	case 2535:
		FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			ClipPickStep = 2540;
		}
		else if (nRet_1 == BD_RETRY)
		{
			ClipPickStep = 2550;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다.
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2360, CTL_dWARNING, COMI.mc_alarmcode);

			ClipPickStep = 2550;
		}
		break;
		
	case 2540:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == RET_GOOD)
		{
			ClipPickStep = 2530;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{//260100 0 26 "Clip Insert Picker up 에러."
			 //180904 0 18 "Clamp rbt unclamp 에러가 발생했습니다."
				sprintf(mc_alarmcode,"260100");
				mn_Clipretry = 0;
				CTL_Lib.Alarm_Error_Occurrence(2370, CTL_dWARNING, mc_alarmcode);
				ClipPickStep = 2535;
			}
			else
			{
				ClipPickStep = 2535;
			}
		}
		break;
		

	case 2550:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPickStep = 2530;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPickStep = 2550;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPickStep = 2535;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2372, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2560:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_picker_rbt_updn_onoff, st_io.i_uld_picker_rbt_up_chk, st_io.i_uld_stacker1_dn_chk,
			IO_ON, st_wait.n_on_wait_time[50], st_wait.n_limit_wait_time[50]);
		if(nRet_1 == RET_GOOD)
		{
			ClipPickStep = 2600;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{//180903 0 18 "Clamp rbt clamp 에러가 발생했습니다."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"260100");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2373, CTL_dWARNING, mc_alarmcode);

				ClipPickStep = 2535;
			}
			else
			{
				ClipPickStep = 2535;
			}
		}
		break;

	case 2570:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPickStep = 2530;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPickStep = 2570;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPickStep = 2570;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2374, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;
		
	case 2600:
		mn_Clipretry = 0;
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y , st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClipPickStep = 2620;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPickStep = 2600;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPickStep = 2600;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2375, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2610:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		ClipPickStep = 2620;
		break;

	case 2620:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			ClipPickStep = 2630;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{////180800 0 18 "Clip buffer Up 체크 에러가 발생했습니다."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"180800");
				CTL_Lib.Alarm_Error_Occurrence(2376, CTL_dWARNING, mc_alarmcode);
				ClipPickStep = 2610;
			}
			else
			{
				ClipPickStep = 2610;
			}
		}
		break;

	case 2630:		
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z , st_motor[M_CLIP_INSERT_Z].md_pos[LD_CLIP_POS], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipPickStep = 2700;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipPickStep = 2630;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			ClipPickStep = 2630;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2377, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;	

	case 2700:
		if(FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_OFF &&
			FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_ON)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPickStep = 2800;
		}
		else
		{
			ActionPicker[D_EXIST] = YES;
			Set_PickerUpDown(IO_OFF, ActionPicker);
			ClipPickStep = 2710;
		}
		break;

	case 2710:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPickStep = 2800;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
		}
		break;

	case 2800:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME) break;
		mn_Clipretry = 0;
		ClipPickStep = 2810;
		break;

	case 2810:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_MIDCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			ClipPickStep = 2820;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClipPlaceStep = 2810;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClipPlaceStep = 2810;
			CTL_Lib.Alarm_Error_Occurrence(2378, CTL_dWARNING, COMI.mc_alarmcode);				
		}
		break;	

	case 2820:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME) break;

		ClipPickStep = 2830;
		break;

	case 2830:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z , st_motor[M_CLIP_INSERT_Z].md_pos[LD_CLIP_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipPickStep = 2510;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipPickStep = 2830;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			ClipPickStep = 2830;
			CTL_Lib.Alarm_Error_Occurrence(2379, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2900:
		ClipPickStep = 3000;
		break;
// 		nRet_1 = COMI.Start_SingleMove(M_M_CLIP_CLAMP , st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_CLAMP], st_basic.nRunSpeed);
// 		if(nRet_1 == BD_GOOD)
// 		{
// 			ClipPickStep = 3000;
// 		}
// 		else if(nRet_1 == BD_RETRY)
// 		{
// 			ClipPlaceStep = 2900;
// 		}
// 		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 		{
// 			ClipPlaceStep = 2900;
// 			CTL_Lib.Alarm_Error_Occurrence(2399, CTL_dWARNING, COMI.mc_alarmcode);				
// 		}
// 		break;

	case 3000:
		st_modulemap.nClipRobotPicker[D_EXIST]   = DVC_YES;
		st_modulemap.nClipRobotPicker[D_BIN]     = BIN_LDBUFFERBIN;
		st_modulemap.nClipRobotPicker[D_RE_CNT]  = T_NOTTEST;
		st_modulemap.nClipRobotPicker[D_SCR_BIN] = BIN_LDBUFFERBIN;
		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_CLIP1_PICKER);			// 피커 정보 
		}

		nFuncRet = RET_GOOD;
		ClipPickStep = 0;
		break;
	}

	return nFuncRet;
}


int CRun_Rbt_Load_Clip::OnClampRbtMove(int nPos)
{
	int nRet_1, nRet_2, nFuncRet = RET_PROCEED;
	double dCurr[3];

	dCurr[0] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
	dCurr[1] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
	dCurr[2] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
	Func.ThreadFunctionStepTrace(16, ClampRbtStep);

	switch(ClampRbtStep)
	{
	case 0:
		if(dCurr[2] > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_CLAMP_RBT_Z].mn_allow)
		{
			ClampRbtStep = 100;
		}
		else
		{
			if(dCurr[2] < st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY] - st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow && 
				dCurr[2] > st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY] + st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow)
			{
				ClampRbtStep = 300;
			}
			else
			{
				ClampRbtStep = 1000;
			}
		}
//		FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff + (nPos*100), IO_ON);//2012,0919
//		l_ClipWaitTime[0] = GetCurrentTime();
		break;

	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClampRbtStep = 300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClampRbtStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClampRbtStep = 100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2381, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClampRbtStep = 0;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClampRbtStep = 300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClampRbtStep = 300;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2382, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;


	case 1000://0702
		//nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLIP_WORK_PLACE1+nPos-1], st_basic.nRunSpeed);
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE1+nPos], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			ClampRbtStep = 1010;
			ClampRbtStep = 1100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClampRbtStep = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClampRbtStep = 1000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2383, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 1010:
		l_ClipWaitTime[1] = GetCurrentTime();
		l_ClipWaitTime[2] = l_ClipWaitTime[1] - l_ClipWaitTime[0];
		if(l_ClipWaitTime[2] < 0) l_ClipWaitTime[0] = GetCurrentTime();
		if(l_ClipWaitTime[2] > 500)
		{
			FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff + (nPos*100), IO_OFF);
			l_ClipWaitTime[0] = GetCurrentTime();
			ClampRbtStep = 1020;
		}
		break;

	case 1020:
		l_ClipWaitTime[1] = GetCurrentTime();
		l_ClipWaitTime[2] = l_ClipWaitTime[1] - l_ClipWaitTime[0];
		if(l_ClipWaitTime[2] < 0) l_ClipWaitTime[0] = GetCurrentTime();
		if(l_ClipWaitTime[2] > 500)
		{
			FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff + (nPos*100), IO_ON);
			l_ClipWaitTime[0] = GetCurrentTime();
			ClampRbtStep = 1030;
		}
		break;

	case 1030:
		l_ClipWaitTime[1] = GetCurrentTime();
		l_ClipWaitTime[2] = l_ClipWaitTime[1] - l_ClipWaitTime[0];
		if(l_ClipWaitTime[2] < 0) l_ClipWaitTime[0] = GetCurrentTime();
		if(l_ClipWaitTime[2] > 500)
		{
			ClampRbtStep = 1100;
		}
		break;

	case 1100:
		//if
		//if(st_sync.i_WorkBuffer_Clip[nPos] == NO)
		//{
//			st_sync.i_WorkClamp_Clip[nPos] = YES;
			FAS_IO.set_out_bit(st_io.o_buf1_f_hs_onoff + (nPos*100), IO_OFF);
			l_ClipWaitTime[0] = GetCurrentTime();
			ClampRbtStep = 1200;
		//}
		break;

	case 1200:
		l_ClipWaitTime[1] = GetCurrentTime();
		l_ClipWaitTime[2] = l_ClipWaitTime[1] - l_ClipWaitTime[0];
		if(l_ClipWaitTime[2] < 200) break;
		if(l_ClipWaitTime[2] < 0) l_ClipWaitTime[0] = GetCurrentTime();
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_GLIPPER_WORK_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			ClampRbtStep = 2000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClampRbtStep = 1000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClampRbtStep = 1000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2384, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2000:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_READY_POS + nPos] + md_clampPos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			if(st_basic.n_mode_module_clip_swing == YES)
			{
				ClampRbtStep = 2010;
			}
			else
			{
				ClampRbtStep = 2100;//2012,0821
			}
			//ClampRbtStep = 2100;//2012,0821
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClampRbtStep = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClampRbtStep = 2000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2385, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2010:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClampRbtStep = 2020;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClampRbtStep = 2010;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClampRbtStep = 2010;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2398, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2020:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_READY_POS + nPos] + md_clampPos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			ClampRbtStep = 2100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClampRbtStep = 2020;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClampRbtStep = 2020;
			CTL_Lib.Alarm_Error_Occurrence(2397, CTL_dWARNING, COMI.mc_alarmcode);

		}
		break;

	case 2100:
		FAS_IO.set_out_bit(st_io.o_buf1_f_vacuum_onoff + (nPos*100), IO_ON);
		l_ClipWaitTime[0] = GetCurrentTime();
		ClampRbtStep = 2110;
		break;

	case 2110:
		l_ClipWaitTime[1] = GetCurrentTime();
		l_ClipWaitTime[2] = l_ClipWaitTime[1] - l_ClipWaitTime[0];
		if(l_ClipWaitTime[2] < 0)
		{
			l_ClipWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_ClipWaitTime[2] > 1000)
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_buf1_f_vacuum_chk + (nPos*100), IO_ON);
			nRet_2 = FAS_IO.get_in_bit(st_io.i_buf1_b_vacuum_chk + (nPos*100), IO_ON);
			nRet_2 = IO_ON;
			if((nRet_1 == IO_ON && nRet_2 == IO_ON) || st_basic.n_mode_device != 1)
			{
				mn_retry = 0;
				ClampRbtStep = 2200;
			}
			else
			{
				mn_retry++;
				if(mn_retry < 10)
				{
					//md_clampPos += 0.05;
					FAS_IO.set_out_bit(st_io.o_buf1_f_vacuum_onoff + (nPos*100), IO_ON);
					ClampRbtStep = 2010;
				}
				else
				{//550100 0 55 "Clip#1 Front Clamp Vacuum Error"	
			     //550104 0 55 "Clip#1 Rear Clamp Vacuum Error"
					if(nRet_1 == RET_ERROR) sprintf(mc_alarmcode,"55010%d",nPos);
					else                    sprintf(mc_alarmcode,"55010%d",nPos+4);					
					mn_retry = 0;
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(2386, CTL_dWARNING, mc_alarmcode);
					nFuncRet = RET_ERROR;
					ClampRbtStep = 2020;
				}
			}
		}
		break;

	case 2120:
		FAS_IO.set_out_bit(st_io.o_buf1_f_vacuum_onoff + (nPos*100), IO_OFF);
		ClampRbtStep = 2010;
		break;

	case 2200:		
		FAS_IO.set_out_bit(st_io.o_buf1_hs_clamp_onoff + (nPos*100), IO_ON);
		l_ClipWaitTime[0] = GetCurrentTime();
		ClampRbtStep = 2210;
		break;

	case 2210:
		l_ClipWaitTime[1] = GetCurrentTime();
		l_ClipWaitTime[2] = l_ClipWaitTime[1] - l_ClipWaitTime[0];
		if(l_ClipWaitTime[2] < 0) l_ClipWaitTime[0] = GetCurrentTime();
		if(l_ClipWaitTime[2] > 500)
		{
			nRet_1 = FAS_IO.get_in_bit(st_io.i_buf1_hs_clamp_on_chk + (nPos*100), IO_OFF);
			nRet_2 = FAS_IO.get_in_bit(st_io.i_buf1_hs_clamp_off_chk + (nPos*100), IO_ON);
			if(nRet_1 == IO_OFF && nRet_2 == IO_ON || st_basic.n_mode_device != 1)
			{
				ClampRbtStep = 3000;
			}
			else
			{
				mn_retry++;
				if(mn_retry < 3)
				{
					ClampRbtStep = 2200;
				}
				else
				{//550104 0 55 "Clip#1 Clamp Off Error"
					sprintf(mc_alarmcode,"5501%02d",7+nPos);							
        			CTL_Lib.Alarm_Error_Occurrence(2387, CTL_dWARNING, mc_alarmcode);
					nFuncRet = RET_ERROR;

					ClampRbtStep = 2200;
				}
			}
		}
		break;

	case 3000:// GLIPPER_READY_POS -> GLIPPER_CLAMP_POS
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_CLAMP_POS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			nFuncRet = RET_GOOD;
			ClampRbtStep = 0;
		}
		else if(nRet_1 == BD_RETRY)
		{
			ClampRbtStep = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			ClampRbtStep = 3000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2388, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;
	}

	return nFuncRet;
}

//#define LD_CLIP_SAFETY				0
//#define LD_CLIP_POS					1
//#define ULD_CLIP_POS				2

void CRun_Rbt_Load_Clip::Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE])
{
	if(PickerInfo[D_EXIST] == YES)
	{
		FAS_IO.set_out_bit(st_io.o_clip_rbt_updn_onoff, OnOff);
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_clip_rbt_updn_onoff, !OnOff);
	}
	b_PickerFlag = FALSE;
	l_PickerWaitTime[0] = GetCurrentTime();
}

int CRun_Rbt_Load_Clip::Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE])
{
	int FuncRet = RET_PROCEED;
	int RetVal = NO;
	char jamcode[10] = {0,};
	CString str;
	
	RetVal = NO;

	if(COMI.mn_simulation_mode == 1) 
	{
		FuncRet = RET_GOOD;
	}
	else
	{
		if(OnOff == IO_ON) //Picker ON (Down이라면..)
		{
			if(PickerInfo[D_EXIST] == YES) //동작 시킨 피커면!
			{
				if(b_PickerFlag == FALSE && FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON) == IO_ON
					&& FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_OFF) == IO_OFF)
				{
					b_PickerFlag = TRUE;
					l_PickerWaitTime[0] = GetCurrentTime();   
				}
				else if(b_PickerFlag == TRUE && FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON) == IO_ON
					&& FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_OFF) == IO_OFF)
				{
					l_PickerWaitTime[1] = GetCurrentTime();  
					l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
					
					if(l_PickerWaitTime[2] < 0)
					{
						l_PickerWaitTime[0] = GetCurrentTime();
					}
					if(l_PickerWaitTime[2] > st_wait.n_on_wait_time[64])
					{
						RetVal = YES;
					}
				}
				else
				{
					l_PickerWaitTime[1] = GetCurrentTime();  
					l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
					if(l_PickerWaitTime[2] < 0)
					{
						l_PickerWaitTime[0] = GetCurrentTime();
					}
					
					if(l_PickerWaitTime[2] > st_wait.n_limit_wait_time[64])
					{
	//					260100 0 26 "Clip Insert Picker up 에러."
	//					260101 0 26 "Clip Insert Picker down 에러."
						sprintf(mc_alarmcode,"260101"); 
						CTL_Lib.Alarm_Error_Occurrence(2389, CTL_dWARNING, mc_alarmcode);

						FuncRet = RET_ERROR;
					}
				}
			}
			else RetVal = YES;
		}
		else //Picker OFF (UP이라면..)
		{
			if(PickerInfo[D_EXIST] == YES) //동작 시킨 피커면!
			{
				if(b_PickerFlag == FALSE && FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_OFF
					&& FAS_IO.Get_In_Bit(st_io.i_clip_rbt_up_chk) == IO_ON)
				{
					b_PickerFlag = TRUE;
					l_PickerWaitTime[0] = GetCurrentTime();   
				}
				else if(b_PickerFlag == TRUE && FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_OFF
					&& FAS_IO.Get_In_Bit(st_io.i_clip_rbt_up_chk) == IO_ON)
				{
					l_PickerWaitTime[1] = GetCurrentTime();  
					l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
					
					if(l_PickerWaitTime[2] > st_wait.n_on_wait_time[64])
					{
						RetVal = YES;
					}
				}
				else
				{
					l_PickerWaitTime[1] = GetCurrentTime();  
					l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
					if(l_PickerWaitTime[2] < 0)
					{
						l_PickerWaitTime[0] = GetCurrentTime();
					}
					
					if(l_PickerWaitTime[2] > st_wait.n_limit_wait_time[64])
					{
						if(st_handler.cwnd_list != NULL)
						{
							str = _T("[UPICKER Error] Picker up sensor check error.");
							sprintf(st_msg.c_normal_msg,"%s", str);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
							Func.On_LogFile_Add(0, str);
							Func.On_LogFile_Add(99, str);
						}
	//					260100 0 26 "Clip Insert Picker up 에러."
	//					260101 0 26 "Clip Insert Picker down 에러."
						alarm.mstr_pcode = _T("260100"); 
						sprintf(mc_alarmcode,"260100"); 
						CTL_Lib.Alarm_Error_Occurrence(2390, CTL_dWARNING, mc_alarmcode);

						FuncRet = RET_ERROR;
					}
				}
			}
			else RetVal = YES;
		}
	}
	
	if(RetVal == YES) 
	{
		FuncRet = RET_GOOD;
	}

	return FuncRet;
}


int CRun_Rbt_Load_Clip::Run_RemoveClip()// Drop clip on bin
{
	int nFuncRet = RET_PROCEED;
	int nRet_1;
	double dCurrentPos;
	
	Func.ThreadFunctionStepTrace(78, DropStep);
	switch(DropStep)
	{
	case 0:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			DropStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			DropStep = 0;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(2391, CTL_dWARNING, COMI.mc_alarmcode);
			DropStep = 0;
		}
		break;
		

	case 100:
		if(st_sync.n_module_ldrbt_clamp_req[0] == CTL_NO || st_sync.n_module_ldrbt_clamp_req[0] == CTL_FREE ||
			st_sync.n_module_ldrbt_clamp_req[0] == CTL_READY ||	st_sync.n_module_ldrbt_clamp_req[0] == CTL_NOTREADY ||
			st_sync.n_module_ldrbt_clamp_req[0] == CTL_YES || st_sync.n_lotend_clamp_ldrbt == CTL_YES)
		{
			dCurrentPos = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
			if(dCurrentPos < st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE1] + st_motor[M_M_CLAMP_RBT_Y].mn_allow)
			{
				DropStep = 120;//110; 20121017
			}
			else
			{
				DropStep = 120;
			}
		}
		//0907에러 처일일 경우 무시 되어야 함
//		dCurrentPos = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
//		if(dCurrentPos < st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLIP_WORK_PLACE1] + st_motor[M_M_CLAMP_RBT_Y].mn_allow)
//		{
//			DropStep = 110;
//		}
//		else
//		{
//			DropStep = 120;
//		}
		
		break;

	case 110:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_PLACE1_REMOVE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			DropStep = 200;
			st_basic.n_curr_clamp_clip_bin++;
		}
		else if(nRet_1 == BD_RETRY)
		{
			DropStep = 100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2392, CTL_dWARNING, COMI.mc_alarmcode);
			DropStep = 100;
		}
		break;

	case 120:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_REMOVE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_retry = 0;
			DropStep = 200;
			st_basic.n_curr_clip_bin++;
		}
		else if(nRet_1 == BD_RETRY)
		{
			DropStep = 100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2392, CTL_dWARNING, COMI.mc_alarmcode);
			DropStep = 100;
		}
		break;

	case 200:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		DropStep = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			DropStep = 400;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(2393, CTL_dWARNING, mc_alarmcode);
				DropStep = 200;
			}
			else
			{
				DropStep = 200;
			}
		}	
		break;

	case 400:
		mn_retry = 0;
		DropStep = 500;
		break;
//		nRet_1 = CTL_Lib.Single_Move(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].md_pos[REMOVE_CLIP_POS], st_basic.nRunSpeed);
//		if(nRet_1 == CTLBD_RET_GOOD)
//		{
//			mn_retry = 0;
//			DropStep = 500;
//		}
//		else if(nRet_1 == CTLBD_RET_RETRY)
//		{
//			DropStep = 400;
//		}
//		else if(nRet_1 != CTLBD_RET_PROCEED)
//		{
//			DropStep = 400;
//			CTL_Lib.Alarm_Error_Occurrence(2394, CTL_dWARNING, COMI.mc_alarmcode);
//		}
//		break;

	case 500:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		DropStep = 600;
		break;

	case 600:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			DropStep = 700;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(2395, CTL_dWARNING, mc_alarmcode);
				DropStep = 500;
			}
			else
			{
				DropStep = 500;
			}
		}	
		break;
		
	case 700:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			DropStep = 800;//1000;20121020
		}
		else if(nRet_1 == BD_RETRY)
		{
			DropStep = 700;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2396, CTL_dWARNING, COMI.mc_alarmcode);				
			DropStep = 700;
		}
		break;

	case 800:
		///////////////////////// 20121018
		
		FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff,IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_on, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_off, IO_OFF);
		/////////////////////////
		l_PickerWaitTime[0] = GetCurrentTime();
		DropStep = 900;
		break;

	case 900:
		l_PickerWaitTime[1] = GetCurrentTime();
		l_PickerWaitTime[2] = l_PickerWaitTime[1] - l_PickerWaitTime[0];
		if(l_PickerWaitTime[2] < 0)
		{
			l_PickerWaitTime[0] = GetCurrentTime();
			break;
		}
		if(l_PickerWaitTime[2] < PICKERSTATUS_CHECK_TIME*2) break;

		FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff,IO_OFF);
		DropStep = 910;
		break;

	case 910:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == RET_GOOD)
		{
			DropStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			//260100 0 26 "Clip Insert Picker up 에러."
			sprintf(mc_alarmcode,"260100");
			CTL_Lib.Alarm_Error_Occurrence(2370, CTL_dWARNING, mc_alarmcode);
			DropStep = 800;
		}
		break;

	case 1000:
		st_modulemap.nClipRobotPicker[D_EXIST]   = DVC_NO;
		st_modulemap.nClipRobotPicker[D_BIN]     = BIN_CLEARBIN;
		st_modulemap.nClipRobotPicker[D_RE_CNT]  = T_NOTTEST;
		st_modulemap.nClipRobotPicker[D_SCR_BIN] = BIN_CLEARBIN;
		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_CLIP1_PICKER);			// 피커 정보 
		}

		nFuncRet = RET_GOOD;
		st_sync.i_remove_clip_bin = CTL_NO;
		DropStep = 0;
		///////////////////////// 20121018
		FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_off, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_on, IO_OFF);
		/////////////////////////
		Clip_remove_Cnt++;
		break;
	}

	return nFuncRet;
}

int CRun_Rbt_Load_Clip::LoadClipMove()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1;
	Func.ThreadFunctionStepTrace(82, LdClipStep);
	switch(LdClipStep)
	{
	case 0:
		nRet_1 = LdUldClipBufferPos(LD_CLIP_POS);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
			LdClipStep = 1000;
		}
		break;

	case 100:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		LdClipStep = 1000;
		break;

	case 1000:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			nFuncRet = RET_GOOD;
			st_sync.i_load_clip = CTL_NO;
			LdClipStep = 0;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(2330, CTL_dWARNING, mc_alarmcode);
				LdClipStep = 100;
			}
			else
			{
				LdClipStep = 100;
			}
		}
		break;
	}
	return nFuncRet;
}

int CRun_Rbt_Load_Clip::UnloadClipMove()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1;
	switch(UldClipStep)
	{
	case 0:
		nRet_1 = LdUldClipBufferPos(ULD_CLIP_POS);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
			UldClipStep = 1000;
		}
		break;

	case 10:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		UldClipStep = 1000;
		break;

	case 1000:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if(nRet_1 == RET_GOOD)
		{
			nFuncRet = RET_GOOD;
			st_sync.i_unload_clip = CTL_NO;
			UldClipStep = 0;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_retry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(2332, CTL_dWARNING, mc_alarmcode);
				mn_retry = 10;
			}
			else
			{
				mn_retry = 10;
			}
		}
		break;
	}
	return nFuncRet;
}

int CRun_Rbt_Load_Clip::PickUpClipForBin()// Drop clip on bin
{
	int nFuncRet = RET_PROCEED;
	int nRet_1;
	
	Func.ThreadFunctionStepTrace(81, PickClipStep);
	switch(PickClipStep)
	{
	case 0:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if (nRet_1 == BD_GOOD)
		{
			PickClipStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			PickClipStep = 0;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(5073, CTL_dWARNING, COMI.mc_alarmcode);
			PickClipStep = 0;
		}
		break;
		

	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_PICKPOS], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			PickClipStep = 200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			PickClipStep = 100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(5074, CTL_dWARNING, COMI.mc_alarmcode);
			PickClipStep = 100;
		}
		break;

	case 200:
		nRet_1 = LdUldClipBufferPos(ULD_CLIP_POS);
		if(nRet_1 == RET_GOOD)
		{
			PickClipStep = 300;
		}
		break;
		
	case 300:
		nRet_1 = OnPickupClip(LD_CLIP_POS);
		if(nRet_1 == RET_GOOD)
		{
			PickClipStep = 1000;
		}
		else if(nRet_1 == RET_ERROR)//2015.0131
		{
			PickClipStep = 1000;
		}
		break;

	case 1000:
		st_modulemap.nClipRobotPicker[D_EXIST]   = DVC_YES;
		st_modulemap.nClipRobotPicker[D_BIN]     = BIN_LDBUFFERBIN;
		st_modulemap.nClipRobotPicker[D_RE_CNT]  = T_NOTTEST;
		st_modulemap.nClipRobotPicker[D_SCR_BIN] = BIN_LDBUFFERBIN;
		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_CLIP1_PICKER);			// 피커 정보 
		}

		st_sync.i_pickup_clip = CTL_NO;
		nFuncRet = RET_GOOD;
		PickClipStep = 0;
	}

	return nFuncRet;
}
