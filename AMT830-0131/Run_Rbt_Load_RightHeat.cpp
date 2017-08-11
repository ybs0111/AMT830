// Run_Rbt_Load_RightHeat.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Rbt_Load_RightHeat.h"
#include "ComizoaPublic.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Function.h"
#include "CtlBd_Library.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Load_RightHeat
CRun_Rbt_Load_RightHeat Run_Rbt_Load_RightHeat;
CRun_Rbt_Load_RightHeat::CRun_Rbt_Load_RightHeat()
{
	Thread_Variable_Initial();
}

CRun_Rbt_Load_RightHeat::~CRun_Rbt_Load_RightHeat()
{
}
/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Load_RightHeat message handlers
void CRun_Rbt_Load_RightHeat::Thread_Run()
{
	// **************************************************************************
    // ������ ���� ���� ���� �˻��Ѵ� (System Lock)                              
    // **************************************************************************
	if(st_handler.mn_system_lock != FALSE)  return;	// ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************
	
	switch(st_work.mn_run_status)
	{
	case dINIT:
		Run_Init();
		break;

	case dRUN:
		Run_Move();
		break;

	case dREINSTATE :
		Run_Reinstate();
		break;
	
	default:
		if(st_work.mn_run_status == dSTOP)
		{
			OnHSRightRbt_FinalPos();//20121126
		}

		//dSTOP, dJAM, dLOTEND, dWARNING, dLOCK, dMAINT (��� �������� �ʴ´ٸ�!)
		if(st_handler.mn_menu_num == 501)  return;   //XYZ Robot motor ȭ���̸�..		

		COMI.Set_MotStop(0, M_HS_B_RBT_Z);
		COMI.Set_MotStop(0, M_HS_B_RBT_Y);
		CTL_Lib.mn_single_motmove_step[M_HS_B_RBT_Z] = 0;
		CTL_Lib.mn_single_motmove_step[M_HS_B_RBT_Y] = 0;
		InitStep = 0;
		ReinstateStep = 0;
		break;
	}
}


void CRun_Rbt_Load_RightHeat::Run_Init()
{
	int i,nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;
	
	if(st_handler.mn_init_state[INIT_RBT_RIGHT_SINK] != CTL_NO) return;

	st_handler.mn_back_mode = YES;
	switch(InitStep)
	{
	case 0:
		for (i = 0; i < PICKER_NUM; i++)
		{
			ActionPicker[D_EXIST][i] = YES;
		}
		Set_FingerOnOff(IO_OFF, ActionPicker);
		InitStep = 10;
		break;

	case 10:
		nRet_1 = Get_FingerOnOff(IO_OFF, ActionPicker);

		if(nRet_1 == RET_GOOD)
		{
			InitStep = 20;
		}
		else if(nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			InitStep = 10;
		}
		break;
		
	case 20:
		// *************************************************************
		//  �Ǹ����� UP��Ų��.                                          
		// *************************************************************

		for (i = 0; i < PICKER_NUM; i++)
		{
			ActionPicker[D_EXIST][i] = YES;
		}
		Set_PickerUpDown(IO_OFF, ActionPicker);
		InitStep = 100;
		break;

	case 100:
		// *************************************************************
		//  UP ��Ų �Ǹ����� CHECK�Ѵ�.                                 
		// *************************************************************
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);

		if (nRet_1 == RET_GOOD)			// ���������� Up�� ���� ���.
		{
			InitStep = 1000;
		}
		else if (nRet_1 == CTLBD_RET_ERROR)
		{
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"[Initialize Error] Wouldn't backward heat sink Picker Up.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2200, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 1000:		//z axis���� homecheck ����!!
		// *************************************************************
		//  Ȩüũ�� �Ѵ�.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// ���������� Home Check�� ������ ���.
		{
			l_until_wait_time[0] = GetCurrentTime();
			InitStep = 1100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2201, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1100:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		if(l_until_wait_time[2] < 0)	l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
		if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) 	break;

		nRet_1 = RET_GOOD;
		for (i = 0; i < PICKER_NUM; i++)
		{
			if(ActionPicker[D_EXIST][i] == DVC_YES && st_picker.n_heat_rightldrbt_enable[i] == YES)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_bwd_dvc_chk[i], IO_ON) == IO_OFF)
				{
					//420600 1 42 "Backward Heat Sink Picker #1 Module Off Check Error.[PS0400]."
					sprintf(mc_alarmcode, "42060%d",  i); //��Ŀ�� ���ȴµ� ������ ON �Ǿ���..
					CTL_Lib.Alarm_Error_Occurrence(2202, CTL_dWARNING, mc_alarmcode);
					nRet_1 = RET_ERROR;
					break;
				}
			}
		}

		if(nRet_1 == RET_GOOD)
		{
			InitStep = 2000;	
		}
		break;

	case 2000:
		nRet_1 = COMI.HomeCheck_Mot(M_HS_B_RBT_Y, st_motor[M_HS_B_RBT_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// ���������� Home Check�� ������ ���.
		{
			InitStep = 2100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitStep = 2000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2202, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2100:
		for (i = 0; i < PICKER_NUM; i++)
		{
			FingerInfo[D_EXIST][i] = YES;
		}
		Set_FingerOnOff(IO_OFF,FingerInfo);
		InitStep = 3000;
		break;

	case 3000:
		st_handler.mn_init_state[INIT_RBT_RIGHT_SINK] = CTL_YES;
		InitStep = 0;
		break;		
	}
}

void CRun_Rbt_Load_RightHeat::Thread_Variable_Initial()
{
	int i;
	//mn_posok = 0;
	InitStep = 0;
	RunStep = 0;
	SafetyZone_Step = 0;
	Tray_Pick_Step = 0;
	Module_Place_Step = 0;
	Before_Module_Place_Step = 0;
	TrayPlaceStep = 0;

	st_sync.n_lotend_righths_ldrbt = CTL_NO;
	for (i = 0; i < PICKER_NUM; i++)
	{
		st_modulemap.RighthsPickerEnable[i] = CTL_YES;
//		st_picker.n_heat_rightldrbt_enable[i] = CTL_YES;
	}
	st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][0] = -1;
	st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1] = -1;

	ReinstateStep = 0;
	st_work.mn_RightHsRobot_Reinstatement_Ok = NO;
	st_work.mn_RightHsRobot_Reinstatement_Ready = NO;

	mn_Clipretry = 0;

}

void CRun_Rbt_Load_RightHeat::Run_Move()
{	
	int Val_1 = 0, Val_2 = 0, nRet_1 = 0, nRet_2 = 0, nRet_3 = 0, flag = 0;
	double dCurrPos;
	int i = 0;
	CString strTmp;//2014.0722 ysj
//	if(alarm.n_area_ready_alarm == TRUE)
//	{
//		COMI.Set_MotStop(1, M_HS_B_RBT_Z);
//		COMI.Set_MotStop(1, M_HS_B_RBT_Y);
//		CTL_Lib.mn_single_motmove_step[M_HS_B_RBT_Z] = 0;
//		CTL_Lib.mn_single_motmove_step[M_HS_B_RBT_Y] = 0;
//		return;
//	}
	st_handler.mn_back_mode = YES;
	Func.ThreadFunctionStepTrace(21, RunStep);
	switch(RunStep)
	{
	case -1:
		Thread_Variable_Initial();
		RunStep = 0;
		break;

	case 0:
		if(st_work.mn_lot_start == CTL_YES)
		{
			if(st_sync.n_lotend_righths_ldrbt == CTL_NO)
			{
				RunStep = 100;
			}
		}
		break;


	case 10:
		st_handler.mn_init_state[INIT_RBT_RIGHT_SINK] = CTL_NO;
		RunStep = 20;
		break;

	case 20:
		Run_Init();
		RunStep = 30;
		break;

	case 30:
		if(st_handler.mn_init_state[INIT_RBT_RIGHT_SINK] == CTL_YES)
		{
			RunStep = 100;
		}
		break;

	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunStep = 300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunStep = 100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunStep = 100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2210, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunStep = 500;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunStep = 300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunStep = 300;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2211, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;


	case 500:
		if(st_sync.n_ld_righths_tray_supply[1] == CTL_FREE)
		{
			st_sync.n_righths_ldrbt_tray_req[0] = CTL_REQ;
//			st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][0] = -1;
//			for (i = 0; i < 4; i++)
//			{
//				 if(st_sync.n_module_site_status[LDMODULE_SITE][i][0] == BUFF_DVC_LOAD &&
//					 st_sync.n_module_site_status[RIGHTSINK_SITE][i][0] == BUFF_DVC_LOAD &&
//					 st_sync.n_module_site_status[RIGHTSINK_SITE][i][0] == BUFF_EMPTY)
//				 {
//					 st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][0] = i;
//					 if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][i][0] == CTL_FREE)
//					 {
//						 st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][i][0] = CTL_REQ;
//						 break;
//					 }
//				 }
//			}
			RunStep = 600;		
		}
		break;

	case 600:
		md_AbsPos = Func.XYRobot_MovePosValCheck(RIGHTSINK_SITE, 0, 0);
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, md_AbsPos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunStep = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunStep = 600;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunStep = 600;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2212, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1000:
		for(i = 0; i < PICKER_NUM; i++)
		{
			if(st_picker.n_heat_rightldrbt_enable[i] == FALSE)
			{
				st_modulemap.nRighthsRobotPicker[D_EXIST][i] = 0;
				st_modulemap.nRighthsRobotPicker[D_BIN][i] = 0;
			}
		}
	
		mn_pre_move_flag = CTL_NO;
		if (st_picker.n_heat_rightldrbt_enable[0] == NO && st_picker.n_heat_rightldrbt_enable[1] == NO )	
		{
			sprintf(mc_alarmcode, "990003");
			CTL_Lib.Alarm_Error_Occurrence(2213, CTL_dWARNING, mc_alarmcode);
			break;
		}

		Val_1 = 0;
		for (i = 0; i < PICKER_NUM; i++)
		{
			if(st_modulemap.nRighthsRobotPicker[D_EXIST][i] == DVC_YES && st_modulemap.nRighthsRobotPicker[D_BIN][i] == BIN_LDBUFFERBIN)
			{
				Val_1++;
			}
		}
		if(Val_1 > 0)
		{
			RunStep = 3000;
			break;
		}

		mn_pickerenable_cnt = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_YES, -1, BIN_NOTUESD); //picker enable ���� 
		Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD);
		if(Val_1 > 0)
		{
			RunStep = 3000;
			break;
		}

		////////////////////////////////////////////////////////////////////
		// LOT END ���� Ȯ��
		////////////////////////////////////////////////////////////////////
		if (st_sync.n_lotend_righths_ldtray_site == CTL_YES && st_sync.n_lotend_righths_tray_transfer == CTL_NO)
		{
			st_sync.n_lotend_righths_tray_transfer = CTL_REQ;
			break;
		}
		else if(st_sync.n_lotend_righths_tray_transfer == CTL_REQ)
		{
			//break;
		}
		else if(st_sync.n_lotend_module_tray_transfer == CTL_READY) //Ʈ���̿��� ���޵Ǵ� �ε� ����� ��� ó���� ���� ���� 
		{
			Val_1 = Func.Find_TrayInfo_Count(RIGHTSINK_SITE, CTL_YES, st_sync.n_righths_ldrbt_tray_req[2]); 
			if(Val_1 <= 0)
			{
 				RunStep = 10000; //XY RObot LOT END ���� 
			}
		}
		/////////////////////////// LOT END üũ END //////////////////////////////////////////////////	
		
		if(st_sync.n_righths_ldrbt_tray_req[0] == CTL_READY)
		{
			if(st_sync.n_righths_ldrbt_tray_req[1] != BIN_LDBUFFERBIN)
			{
				st_sync.n_righths_ldrbt_tray_req[0] = CTL_REQ;
				st_sync.n_righths_ldrbt_tray_req[1] = BIN_LDBUFFERBIN;

				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, _T("[RIGHTHS Tray Change Err] No Tray Exchange Error"));
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				break;
			}
			
			if(COMI.mn_simulation_mode == 1)
			{
				l_until_wait_time[0] = GetCurrentTime();
				st_ani.nSelectMotPos[M_HS_B_RBT_Y] = 1;
				RunStep = 1500;
			}
			else
			{
				RunStep = 2000;
			}
			break;
		}
		else
		{//���� Ʈ���̰� �غ� ���� ���� ���·� �κ�Ʈ�� �̸� Ʈ���� �غ� ��ġ�� �̵��Ͽ� �غ� �Ѵ�  			
			if(mn_pre_move_flag == CTL_NO)
			{
				mn_FirstPicker_Y_Pos = -1;
				for(i=0 ; i<PICKER_NUM; i++) 
				{
					if(st_modulemap.nRighthsRobotPicker[D_EXIST][i] == DVC_NO && st_modulemap.RighthsPickerEnable[i] == YES) //����̽��� ����������� ��Ŀ��ȣ (�������� ã�°Ŵϱ�..)
					{
						mn_FirstTray_X_Pos= 0;
						mn_FirstPicker_Y_Pos = i;
						break;
					}
				}

				if(mn_FirstPicker_Y_Pos >= 0)
				{
					RunStep = 1100; 
				}
			}
		}
		break;

	case 1100:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, st_motor[M_HS_F_RBT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{	
			RunStep = 1200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunStep = 1100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunStep = 1100;
			CTL_Lib.Alarm_Error_Occurrence(2217, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1200:
		if(st_sync.n_righths_ldrbt_tray_req[0] == CTL_READY)
		{
			if(st_sync.n_righths_ldrbt_tray_req[1] != BIN_LDBUFFERBIN)
			{
				st_sync.n_righths_ldrbt_tray_req[0] = CTL_REQ;
				st_sync.n_righths_ldrbt_tray_req[1] = BIN_LDBUFFERBIN;

				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_abnormal_msg, _T("[RIGHTHS Tray Change Err] No Tray Exchange Error"));
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				break;
			}
			RunStep = 1700;
		}
		break;

	case 1500:
		if(COMI.mn_simulation_mode == 1)
		{
			l_until_wait_time[1] = GetCurrentTime();
			l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];

			st_ani.nSelectMotPos[M_HS_B_RBT_Z] = 2;

			if(l_until_wait_time[2] < 0)
			{
				l_until_wait_time[0] = GetCurrentTime();
				break;
			}
			else if(l_until_wait_time[2] > 1000)
			{
				st_ani.nSelectMotPos[M_HS_B_RBT_Z] = 0;
				
				RunStep = 2000;
			}
		}
		break;

	case 1700:
		mCurrent_RowPos[RIGHTSINK_SITE] = mn_FirstTray_X_Pos - (mn_FirstPicker_Y_Pos * PICKING_GAP);
		md_AbsPos = Func.XYRobot_MovePosValCheck(RIGHTSINK_SITE,  0, mCurrent_RowPos[RIGHTSINK_SITE]); //�̸� ��Ŀ�� ����Ų�� 
		nRet_1 =  CTL_Lib.Single_Move(M_HS_B_RBT_Y, md_AbsPos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunStep = 2000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunStep = 1700;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunStep = 1700;
			CTL_Lib.Alarm_Error_Occurrence(2214, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2000:
		nRet_1 = Robot_TrayDVCPick(BIN_LDBUFFERBIN, st_sync.n_righths_ldrbt_tray_req[2]);	//Ʈ������ Y�� ���� 	

		Val_1 = Func.Find_TrayInfo_Count(RIGHTSINK_SITE, CTL_YES, st_sync.n_righths_ldrbt_tray_req[2]); //Ʈ���� �ش� Y �ٿ� ����̽��� ������ CTL_YES, ������ CTL_NO �� 
		Val_2 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD);  //����ϴ� ��Ŀ�� �������� ��Ŀ�� �ϳ��� ������ �� ����� �Ѵ� 

		if(nRet_1 == RET_GOOD) // ���絵 �����ְ�, ��Ŀ�� ����̽��� ��� ������
		{
			if(st_work.n_lotend == CTL_YES)
			{//lot ending......
				
				if(Val_2 > 0)
				{
					Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
					st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
					RunStep = 3000;
				}
				else
				{	
					RunStep = 1000; //���� �ִ� ���簡 ����  
				}
				break;
			}

			if(Val_2 == 0)
			{
				RunStep = 1000;
				break;
			}

			if(Val_1 == 0) //���̻� �� Ʈ���� Y�ٿ��� ����̽��� ���� 
			{
				if(Val_2 > 0)
				{
					Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
					st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
					RunStep = 3000;
					break;
				}

				st_sync.n_righths_ldrbt_tray_req[0] = CTL_REQ;	// Lot�� ���۵Ǿ����� Loader�� ��� ��Ų��.
				st_sync.n_righths_ldrbt_tray_req[1] = BIN_LDBUFFERBIN;				

				if(Val_2 >= mn_pickerenable_cnt) //���̻� ���縦 ���� ��Ŀ�� ����, ���縦 ���� �ִ� ���� 
				{
					Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
					st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
					RunStep = 3000;
				}
				else
				{//���縦 �� ���� �̵��ؾ� �Ѵ� 
					if(st_sync.n_righths_ldrbt_tray_req[2] >= st_traybuffer[RIGHTSINK_SITE].i_loader_col - 1) //X = 0
					{//���� �۾��� �κ��� Ʈ������ ������ ���̸� Ʈ���̸� ��ü�� ���̴�  
						if(Val_2 >= mn_pickerenable_cnt) //���ۿ� ����ִ� ��Ŀ�� ������ŭ �ε� �� ���簡 ������ ó���Ϸ� �ε����۷� �̵��Ѵ� 
						{
							Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
							st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
							RunStep = 3000; //������ Ʈ���� ���� ���縦 ó���߰� ���̻� �� Ʈ���̿��� ���簡 ������ "��ü �ð��� 8�� �̻����� �ε� ������ ���縦 ���´� 
						}
						else
						{
							RunStep = 2000; //������ ���縦 Ʈ���� Y�� �Ǵ� Ʈ���̰� ��ü ��ü �Ǳ⸦ ��ٸ� �� �ٽ� ���´�  
						}
					}
					else
					{
						RunStep = 2000; //������ ���縦 Ʈ���� Y�� �Ǵ� Ʈ���̰� ��ü ��ü �Ǳ⸦ ��ٸ� �� �ٽ� ���´�  
					}
				} 				
			}
			else if(Val_2 >= mn_pickerenable_cnt) //Ʈ���� �ٿ��� ���簡 �����ְ�, ��Ŀ�� ��� ���� �־� ���̻� ���� �� ��Ŀ�� ���� 
			{
				Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
				st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
				RunStep = 3000;
			}
			else
			{//���� �����ִ� ���簡 �����ϴ�. �� ���� 
				RunStep = 2000;
			}
		}
		else if(nRet_1 == RET_PICKERNOTFIND) //��Ŀ���� ���̻� ������ �� �ִ� ��Ŀ�� ���� 
		{
			st_sync.n_righths_ldrbt_tray_req[0] = CTL_REQ;	// Lot�� ���۵Ǿ����� Loader�� ��� ��Ų��.
			st_sync.n_righths_ldrbt_tray_req[1] = BIN_LDBUFFERBIN;	

			if((mn_pickerenable_cnt - Val_2) > 0)
			{
				Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
				st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
				RunStep = 3000; //������ ����� �ε� ���ۿ��� ���´� 
			}
			else if(Val_2 > mn_pickerenable_cnt) //���ۿ� ����ִ� ��Ŀ�� ���� �̻�����  �ε� �� ���簡 ������ ó���Ϸ� �ε����۷� �̵��Ѵ� 
			{
				Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
				st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
				RunStep = 3000; //������ ����� �ε� ���ۿ��� ���´� 
			}
			else
			{
				if(Val_2 > 0)  //����̽��� �ϳ��� ���� ������ 
				{
					Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
					st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
					RunStep = 3000;
				}
				else
				{
					RunStep = 1000; //������ ���縦 Ʈ���� Y���� ��ü �Ǳ⸦ ��ٸ� �� �ٽ� ���´�
				}
			}
		}
		else if(nRet_1 == RET_NOWHERE) //Ʈ���̿��� ����̽��� ���̻� ���� ���� �ʴ´� 
		{
			st_sync.n_righths_ldrbt_tray_req[0] = CTL_REQ;	// Lot�� ���۵Ǿ����� Loader�� ��� ��Ų��.
			st_sync.n_righths_ldrbt_tray_req[1] = BIN_LDBUFFERBIN;

			if((mn_pickerenable_cnt - Val_2) > 0 )
			{
				Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
				st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
				RunStep = 3000; //������ ����� �ε� ���ۿ��� ���´� 
			}
			else if(Val_2 > mn_pickerenable_cnt) //���ۿ� ����ִ� ��Ŀ�� ���� �̻�����  �ε� �� ���簡 ������ ó���Ϸ� �ε����۷� �̵��Ѵ� 
			{
				Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
				st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
				RunStep = 3000; //������ ����� �ε� ���ۿ��� ���´� 
			}
			else
			{
				if(Val_2 > 0)  //����̽��� �ϳ��� ���� ������ 
				{
					Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
					st_work.n_loadlot_count[RIGHTSINK_SITE] = st_work.n_loadlot_count[RIGHTSINK_SITE] + Val_1;
					RunStep = 3000;
				}
				else
				{
					st_handler.n_more_righths = CTL_YES;
					RunStep = 1000; //������ ���縦 Ʈ���� Y���� ��ü �Ǳ⸦ ��ٸ� �� �ٽ� ���´�
				}
			}
		}
		else if(nRet_1 == RET_LOADTRAYSKIP)
		{
			st_sync.n_lotend_righths_ldrbt = CTL_YES;
			RunStep = 10000;
		}
		break;

	case 3000:
		flag = CTL_NO;		
		//���� ��Ŀ�� ���簡 �ֳ� ������ Ȯ���Ѵ� 
		Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD);
		if(Val_1 > 0)
		{
			RunStep = 3100;
		}
		else
		{
			RunStep = 1000;
		}
		break;

	case 3100:
		if(st_sync.n_righths_ldrbt_tray_req[0] == CTL_READY)
		{
			nRet_3 = Func.RHSFindNode();
			HeadVal = nRet_3;

			////2014.0722 ysj
//			strTmp.Format("Run_Rbt_Load_RightHeat - RHSFindNode : %d", HeadVal);
//			Func.On_LogFile_Add(99, strTmp);			
			////
			if(HeadVal != -1)
			{
				Func.CLIPInsertNode(nRet_3); // 20130416
				Func.RHSDelete_Node();

				mn_NextPos = HeadVal;
				if(mn_NextPos >= 4 || mn_NextPos == -1)
					mn_NextPos = WORK_BUFFER_1;
	 			mn_NextPos = Func.OnRetNextBufferPos(mn_NextPos);
 				mn_pre_move_flag = CTL_YES;

				st_basic.n_hs_back_node = HeadVal;//2014.0818

 				RunStep = 3110;	

			}
			else
			{
				RunStep = 3110;
			}
		}
		break;

	case 3110:
		dCurrPos = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);
		if(dCurrPos > st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_HS_B_RBT_Z].mn_allow)
		{
			RunStep = 3111; //Z���� ���� ��ġ�� ��� 
		}
		else
		{
			for (i = 0; i < PICKER_NUM; i++)
			{
				ActionPicker[D_EXIST][i] = YES;
			}
			l_until_wait_time[0] = GetCurrentTime();
			RunStep = 3130;
		}
		break; 

	case 3111:
		for (i = 0; i < PICKER_NUM; i++)
		{
			ActionPicker[D_EXIST][i] = YES;
		}
		Set_PickerUpDown(IO_OFF, ActionPicker);
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunStep = 3110;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunStep = 3111;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2215, CTL_dWARNING, COMI.mc_alarmcode);
			RunStep = 3111;
		}
		break;

	case 3112:
		Set_PickerUpDown(IO_OFF, ActionPicker);
		RunStep = 3120;
		break;

	case 3120:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		if (nRet_1 == RET_GOOD)			// ���������� Up�� ���� ���.
		{
			l_until_wait_time[0] = GetCurrentTime();
			RunStep = 3130;
		}
		else if (nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;		// ���� �߻��� �˶� ���� �÷��� ���� 
			CTL_Lib.Alarm_Error_Occurrence(2216, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 3130:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		
		nRet_1 = Func.PickerDeviceStatusCheck(IO_OFF, st_modulemap.nRighthsRobotPicker, mResult); // ��Ŀ ������ ���� ���� ���¸� üũ 
		
		if(nRet_1 == RET_ERROR)
		{
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //���縦 üũ�� �� �ִ� ���� �ð��� �ش�  
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
					break;
				}			
			}
			RunStep = 3112;
		}
		else
		{
			RunStep = 3140;
		}
		break;

	case 3140:
		Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); 
		if(Val_1 <= 0) //�ε� ����̽��� �ϳ� �̻� ��Ŀ�� �ִ� ���� 
		{
			COMI.Set_MotStop(0, M_HS_B_RBT_Y);
			RunStep = 1000;
			break;
		}
		for(i=0 ; i<PICKER_NUM; i++) 
		{
			if(st_modulemap.nRighthsRobotPicker[D_EXIST][i] == DVC_YES)
			{
				st_work.i_FirstPickerNum[RIGHTSINK_SITE] = i;
				break;
			}
		}
//		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_WORK_PLACE1 + mn_NextPos], st_basic.nRunSpeed);
		md_AbsPos = Func.XYRobot_MovePosValCheck(WORK_BUFFER_SITE, RIGHTSINK_SITE, mn_NextPos, st_work.i_FirstPickerNum[RIGHTSINK_SITE]);
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, md_AbsPos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			mn_pre_move_flag = NO;
			RunStep = 3150;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunStep = 3140;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2217, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;
		
	case 3150:
		Val_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD);
		if(Val_1 <= 0) //�ε� ����̽��� �ϳ� �̻� ��Ŀ�� �ִ� ���� 
		{
			RunStep = 1000;
			break; 
		}

		if(HeadVal == -1)
		{
			for(i=0;i<4;i++)
			{
				if(st_sync.n_module_site_status[LDMODULE_SITE][i][0] == BUFF_DVC_LOAD &&
					st_sync.n_module_site_status[LEFTSINK_SITE][i][0] == BUFF_DVC_LOAD &&
					st_sync.n_module_site_status[RIGHTSINK_SITE][i][0] == BUFF_EMPTY)
				{
					HeadVal=i;
					break;
				}
			}
			if(HeadVal == -1)
			{
				RunStep = 3100;
				break;
			}
		}

		st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][0] = -1;
		if(st_sync.n_module_site_status[LDMODULE_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[LEFTSINK_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[RIGHTSINK_SITE][HeadVal][0] == BUFF_EMPTY) 
		{
			st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][0] = HeadVal;
			st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1] = HeadVal;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][HeadVal][0] = CTL_REQ;
			RunStep = 4000;

			if(COMI.mn_simulation_mode == 1)
			{
				st_ani.nSelectMotPos[M_HS_B_RBT_Y] = st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1] + 3;
				l_until_wait_time[0] = GetCurrentTime();
			}

			break;
		}

		if((st_work.n_loadlot_count[LDMODULE_SITE] >= st_work.nMdlInputCount[0][0]) && st_work.n_lotend == CTL_YES && 
			Func.ChkForLotEnd(RIGHTSINK_SITE) == RET_ERROR)
		{
			RunStep = 10000;
		}
		else if(st_work.n_lotend != CTL_YES && st_handler.n_lotend_ready == 3  && Func.Find_TrayInfo_Count(WORK_BUFFER_SITE, CTL_YES, -1) <= 0
			&& st_work.n_loadlot_count[RIGHTSINK_SITE] >= st_work.n_loadlot_count[LDMODULE_SITE])
		{
			st_handler.n_lotend_ready = 4;
		}
		else
		{
			//RunStep = 3000;
			//if(st_handler.cwnd_list != NULL)
			//{
			//	sprintf(st_msg.c_abnormal_msg, _T("[Module RearSiteError] No Shuttle Place Error"));
			//	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			//}
		}
		break;
	
	case 3200:
		st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][0] = -1;
		if(st_sync.n_module_site_status[LDMODULE_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			 st_sync.n_module_site_status[LEFTSINK_SITE][HeadVal][0] == BUFF_DVC_LOAD &&
			st_sync.n_module_site_status[RIGHTSINK_SITE][HeadVal][0] == BUFF_EMPTY)
		{
			st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1] = HeadVal;
			st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][0] = HeadVal;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][HeadVal][0] = CTL_REQ;
			RunStep = 4000;

			if(COMI.mn_simulation_mode == 1)
			{
				st_ani.nSelectMotPos[M_HS_B_RBT_Y] = st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1] + 3;
				l_until_wait_time[0] = GetCurrentTime();
			}

			break;
		}
		
		break;

	case 4000:		
		flag = CTL_NO;
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1]][1] == CTL_READY)
		{
			flag = CTL_YES;
		}

		if(flag == CTL_YES)
		{
			if(COMI.mn_simulation_mode == 1)
			{
	 			l_until_wait_time[1] = GetCurrentTime();
	 			l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
				
				st_ani.nSelectMotPos[M_HS_B_RBT_Z] = 2;

				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime();
					break;
				}
				else if(l_until_wait_time[2] > 1500)
				{
					Module_Place_Step = 0;
					RunStep = 4100; //������ ����Ʈ�� �ε��Ѵ� 

					st_ani.nSelectMotPos[M_HS_B_RBT_Z] = 0;
				}
			}
			else
			{
				Module_Place_Step = 0;
				RunStep = 4100; //������ ����Ʈ�� �ε��Ѵ� 
			}
		}
		else
		{///�ε� ������ ������ ���� //������ �������� 	
			RunStep = 3200;
		}
		break;

	case 4100:
		nRet_1 = Robot_ModuleDVCPlace(st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1], BIN_NOTUESD) ;
		if(nRet_1 == RET_GOOD)
		{
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1]][0] = CTL_NO;
			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1]][1] = CTL_CHANGE;
			RunStep = 5000;
		}
		else if(nRet_1 == RET_PICKERNOTFIND)// ��Ŀ ������ �´°� ����  //������ ���� ���� 
		{//���õ� PH���� �� �۾��� ������ �ִ����� Ȯ���Ѵ� 
//			st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1]][1] = CTL_CHANGE;
//			RunStep = 5000;
		}
		else if(nRet_1 == RET_NOWHERE) // PH ������ ������ ����	//������ ���� ���� 
		{//���õ� PH���� �� �۾��� ������ �ִ����� Ȯ���Ѵ� 
			//st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTSINK_SITE][1]][1] = CTL_CHANGE;
//			RunStep = 5000;
		}		
		break;

	case 5000:
		RunStep = 1000;
		break;

	case 10000:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunStep = 11000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunStep = 10000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunStep = 10000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2218, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;


	case 11000:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunStep = 12000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunStep = 11100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunStep = 11000;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2219, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 12000:
		st_sync.n_lotend_righths_ldrbt = CTL_YES;

		if(COMI.mn_simulation_mode == 1)
		{
			st_ani.nSelectMotPos[M_HS_B_RBT_Y] = 0;
		}

		RunStep = 0;
		break;
		

	}
}

void CRun_Rbt_Load_RightHeat::OnHSRightRbt_FinalPos()//20121126
{
	int i = 0;
	if(st_work.mn_reinstatement_mode[RIGHTSINK_SITE] == 0) //stop�� ���� �ѹ��� ����ϱ� ����
	{
		st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Y] = COMI.md_cmdpos_backup[M_HS_B_RBT_Y];
		st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Z] = COMI.md_cmdpos_backup[M_HS_B_RBT_Z];

		for (i = 0; i < 2; i++)
		{
			st_work.mn_PickerUpDnStatusBackup[RIGHTSINK_SITE][i] = FAS_IO.get_out_bit(st_io.o_hs_bwd_rbt_picker1_updn + (i*2), IO_ON);
			st_work.mn_FingerStatusBackup[RIGHTSINK_SITE][i] = FAS_IO.get_in_bit(st_io.o_hs_bwd_rbt_picker1_gripper_onoff + (i*2), IO_OFF);
		}
		
		st_work.mn_reinstatement_mode[RIGHTSINK_SITE] = 1;	//���� ��ġ�� ��� �ߴ�
	}
}


void CRun_Rbt_Load_RightHeat::Robot_BackMovePosValCheck()//20121126
{
	//���⼭ ������ �����ߴ� ��ġ�� �ٽ� ����ؾ��Ѵ�.
	st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Y] =  st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Y];
	st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Z] =  st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Z];
}

void CRun_Rbt_Load_RightHeat::Run_Reinstate()//20121126
{
	int Ret1 = 0, Ret2 = 0, i = 0, n_response = 0;
	int nRet1 = 0, nRet2 = 0;

	if (st_work.mn_RightHsRobot_Reinstatement_Ok == TRUE) return;//20121126
	
	switch (ReinstateStep)
	{
	case 0:
		ReinstateStep = 1000;
		break;

	case 1000://���� ��ġ Ȯ��
		dHSRightCurPos[0] = COMI.Get_MotCurrentPos(M_HS_B_RBT_Y);
		dHSRightCurPos[1] = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);

		for (i = 0; i < 2; i++)
		{
			mn_change[RIGHTSINK_SITE][i] = 0;
		}
	
		//����� ��ġ�� ���
		if (dHSRightCurPos[0] > st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Y] + COMI.mn_allow_value[M_HS_B_RBT_Y]
			|| dHSRightCurPos[0] < st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Y] - COMI.mn_allow_value[M_HS_B_RBT_Y])
		{
			mn_change[RIGHTSINK_SITE][0]++;
			sprintf(st_msg.c_abnormal_msg, "Right HS Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Y], (long)dHSRightCurPos[0], (long)COMI.mn_allow_value[M_HS_B_RBT_Y]);
		}
		else if (dHSRightCurPos[1] > st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Z] + COMI.mn_allow_value[M_HS_B_RBT_Z]
			|| dHSRightCurPos[1] < st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Z] - COMI.mn_allow_value[M_HS_B_RBT_Z])
		{
			mn_change[RIGHTSINK_SITE][1]++;
			sprintf(st_msg.c_abnormal_msg, "Right HS Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Z], (long)dHSRightCurPos[1], (long)COMI.mn_allow_value[M_HS_B_RBT_Z]);
		}


		if(mn_change[RIGHTSINK_SITE][0] > 0 || mn_change[RIGHTSINK_SITE][1] > 0) //rbt y
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
		
		COMI.Set_MotPower(M_HS_B_RBT_Y, TRUE);
		COMI.Set_MotPower(M_HS_B_RBT_Z, TRUE);
	
		mn_check[0] = VARIABLE_INIT;
		mn_check[1] = VARIABLE_INIT;

		ReinstateStep = 3000;
		break;


	case 3000:
		if(st_picker.n_heat_rightldrbt_enable[0] == TRUE)
		{
			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_rbt_picker1_gripper_dvc_chk, IO_OFF) == IO_OFF)
			{
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_hs_bwd_rbt_picker1_gripper_onoff, IO_OFF);
			}
		}

		else if(st_picker.n_heat_rightldrbt_enable[0] == TRUE)
		{
			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_rbt_picker2_gripper_dvc_chk, IO_OFF) == IO_OFF)
			{
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_hs_bwd_rbt_picker2_gripper_onoff, IO_OFF);
			}
		}
		else
		{
			for (i = 0; i < PICKER_NUM; i++)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_bwd_rbt_picker1_gripper_dvc_chk + (i*3), IO_OFF) == IO_OFF)
				{
				}
				else
				{
					FAS_IO.set_out_bit(st_io.o_hs_bwd_rbt_picker1_gripper_onoff + (i*2), IO_OFF);
				}
			}
		}
		ReinstateStep = 3010;	
		break;

	case 3010:
//		for (i = 0; i < PICKER_NUM; i++)
//		{
//			ActionPicker[D_EXIST][i] = YES;
//		}
//		Set_PickerUpDown(IO_OFF, ActionPicker);
		Func.Set_RightHS_AllPickerUpDn();

		nRet1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3020;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5022, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3020:
//		nRet1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		nRet1 = Func.Get_RightHS_AllPickerUpDn();
		if (nRet1 == RET_GOOD)			// ���������� Up�� ���� ���.
		{
			ReinstateStep = 3100;
		}
		else if (nRet1 == RET_ERROR)
		{
			ReinstateStep = 3010;
		}
		break;

	case 3100:		
		nRet1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_SAFETY], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3110;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5024, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;
		
	case 3110:
		st_work.mn_RightHsRobot_Reinstatement_Ready = YES;
		ReinstateStep = 3120;
		break;
		
	case 3120:	
		if(st_work.mn_WorkBufferRobot_Reinstatement_Ok == YES)
		{
			ReinstateStep = 3200;
		}
		break;

	case 3200:
		nRet1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Y], st_basic.nManualSpeed);
		
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
		nRet1 = COMI.Check_SingleMove(M_HS_B_RBT_Y, st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Y]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3400;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5025, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3400:
		nRet1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Z], st_basic.nManualSpeed);
		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3500;
		}
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//				CTL_Lib.Alarm_Error_Occurrence(5022, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;
	
	case 3500:
		nRet1 = COMI.Check_SingleMove(M_HS_B_RBT_Z, st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Z]);		
		if (nRet1 == CTLBD_RET_GOOD)
		{
			ReinstateStep = 3600;
		}		
		else if (nRet1 == CTLBD_RET_ERROR)
		{
//			CTL_Lib.Alarm_Error_Occurrence(5022, CTL_dWARNING, COMI.mc_alarmcode);
			ReinstateStep = 20000;
		}
		break;

	case 3600:
		// 	st_work.mn_PickerUpDnStatusBackup[RIGHTSINK_SITE][0] = FAS_IO.get_out_bit(st_io.i_hs_bwd_picker_up_chk[i], IO_ON);
		if(st_picker.n_heat_rightldrbt_enable[0] == TRUE)
		{
			if(st_work.mn_PickerUpDnStatusBackup[RIGHTSINK_SITE][0] == IO_ON)
			{
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_rbt_picker_picker_updn[0], IO_ON);
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_rbt_picker_picker_updn[0], IO_OFF);
			}
		}

		else if(st_picker.n_heat_rightldrbt_enable[1] == TRUE)
		{
			if(st_work.mn_PickerUpDnStatusBackup[RIGHTSINK_SITE][1] == IO_ON)
			{
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_rbt_picker_picker_updn[1], IO_ON);
			}
			else
			{
				FAS_IO.OnCylinderAction(st_io.o_hs_bwd_rbt_picker_picker_updn[1], IO_OFF);
			}
		}

		else
		{
			for (i = 0; i < PICKER_NUM; i++)
			{
				if(st_work.mn_PickerUpDnStatusBackup[RIGHTSINK_SITE][i] == IO_ON)
				{
					FAS_IO.OnCylinderAction(st_io.o_hs_bwd_rbt_picker_picker_updn[i], IO_ON);
				}
				else
				{
					FAS_IO.OnCylinderAction(st_io.o_hs_bwd_rbt_picker_picker_updn[i], IO_OFF);
				}
			}
		}
		ReinstateStep = 50000;		
		break;
		

	case 20000:
		st_handler.n_sync_reinstate = NO;
		ReinstateStep = 0;
		break;

	case 50000:
		st_work.mn_RightHsRobot_Reinstatement_Ready = YES;
		st_work.mn_RightHsRobot_Reinstatement_Ok = TRUE;//20121126
		ReinstateStep = 0;
		break;
	}
}

void CRun_Rbt_Load_RightHeat::Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i;

	for (i = 0; i < PICKER_NUM; i++)
	{
		if(PickerInfo[D_EXIST][i] == YES)
		{
			FAS_IO.set_out_bit(st_io.o_hs_bwd_rbt_picker_picker_updn[i], OnOff);
		}
//		else
//		{
//			FAS_IO.set_out_bit(st_io.o_hs_bwd_rbt_picker_picker_updn[i], !OnOff);
//		}
		b_PickerFlag[i] = FALSE;
		l_PickerWaitTime[i][0] = GetCurrentTime();
	}
}

int CRun_Rbt_Load_RightHeat::Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i, FuncRet = RET_PROCEED;
	int RetVal[PICKER_NUM] = {0,};
	char jamcode[10] = {0,};
	CString str;

	for (i = 0; i < PICKER_NUM; i++)
	{
		RetVal[i] = NO;
		if(OnOff == IO_ON)
		{
			if(PickerInfo[D_EXIST][i] == YES)
			{
				if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[i], IO_ON == IO_ON) &&
					FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[i], IO_OFF) == IO_OFF)
				{
					b_PickerFlag[i] = TRUE;
					l_PickerWaitTime[i][0] = GetCurrentTime();
				}
				else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[i], IO_ON) == IO_ON &&
					FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[i], IO_OFF) == IO_OFF)
				{
					l_PickerWaitTime[i][1] = GetCurrentTime();
					l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];

					if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[46] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_PickerWaitTime[i][1] = GetCurrentTime();
					l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[1][0];
					if(l_PickerWaitTime[i][2] < 0)
					{
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[46] + (i*2))
					{
						//420200 1 40 "Heat Sink Picker #1 Down Check Error.[PS0308]."					
						sprintf(mc_alarmcode, "42020%d", i);
						CTL_Lib.Alarm_Error_Occurrence(2220, CTL_dWARNING, mc_alarmcode);

						FuncRet = RET_ERROR;
					}			
					
				}			
			}
			else
			{
				RetVal[i] = YES;
			}
		}
		else
		{
			if(PickerInfo[D_EXIST][i] == YES)
			{ 
				if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[i], IO_ON == IO_ON) &&
					FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[i], IO_OFF) == IO_OFF)
				{
					b_PickerFlag[i] = TRUE;
					l_PickerWaitTime[i][0] = GetCurrentTime();
				}
				else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[i], IO_ON == IO_ON) &&
					FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[i], IO_OFF) == IO_OFF)
				{
					l_PickerWaitTime[i][1] = GetCurrentTime();
					l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
					if(l_PickerWaitTime[i][2] < 0)
					{
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[46] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_PickerWaitTime[i][1] = GetCurrentTime();
					l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[1][0];
					if(l_PickerWaitTime[i][2] < 0)
					{
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[46] + (i*2))
					{
						//420100 1 40 "Heat Sink Picker #1 Up Check Error.[PS0300]."				
						sprintf(mc_alarmcode, "42010%d", i);
						CTL_Lib.Alarm_Error_Occurrence(2221, CTL_dWARNING, mc_alarmcode);

						FuncRet = RET_ERROR;
					}	
				}
			}
			else
			{
				RetVal[i] = YES;
			}
		}
	}

	if(RetVal[0] == YES && RetVal[1] == YES)
	{
		FuncRet = RET_GOOD;
	}
	
	return FuncRet;
}


void CRun_Rbt_Load_RightHeat::Set_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i;
	
	for (i = 0; i < PICKER_NUM; i++)
	{
		if(FingerInfo[D_EXIST][i] == YES)
		{
			FAS_IO.set_out_bit(st_io.o_hs_bwd_rbt_glipper_onoff[i], OnOff);
		}
		//else
		//{
		//	FAS_IO.set_out_bit(st_io.o_hs_bwd_rbt_glipper_onoff[i], !OnOff);
		//}
		b_FingerFlag[i] = FALSE;
		l_FingerWaitTime[i][0] = GetCurrentTime();
	}
}

int CRun_Rbt_Load_RightHeat::Get_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i, FuncRet = RET_PROCEED;
	int RetVal[PICKER_NUM] = {0,};
	char jamcode[10] = {0,};

	for (i = 0; i < PICKER_NUM; i++)
	{
		RetVal[i] = NO;
		if(OnOff == IO_ON)
		{
			if(FingerInfo[D_EXIST][i] == YES)
			{
				if(b_FingerFlag[i] == FALSE)
				{
					b_FingerFlag[i] = TRUE;
					l_FingerWaitTime[i][0] = GetCurrentTime();
				}
				else if(b_FingerFlag[i] == TRUE)
				{
					l_FingerWaitTime[i][1] = GetCurrentTime();
					l_FingerWaitTime[i][2] = l_FingerWaitTime[i][1] - l_FingerWaitTime[i][0];
					if(l_FingerWaitTime[i][2] < 0)
					{
						l_FingerWaitTime[i][0] = GetCurrentTime();
					}
					if(l_FingerWaitTime[i][2] > st_wait.n_on_wait_time[47] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_FingerWaitTime[i][1] = GetCurrentTime();
					l_FingerWaitTime[i][2] = l_FingerWaitTime[i][1] - l_FingerWaitTime[i][0];
					if(l_FingerWaitTime[i][2] < 0)
					{
						l_FingerWaitTime[i][0] = GetCurrentTime();
					}
					
					if(l_FingerWaitTime[i][2] > st_wait.n_limit_wait_time[47] + (i*2))
					{
						//420500 1 40 "Heat Sink Picker #1 Module On Check Error.[PS0400]."
						sprintf(mc_alarmcode, "42050%d", i);
						alarm.mn_count_mode = 0;	
						alarm.mn_type_mode = eWARNING; 
						alarm.mn_alarm_assign_section = 188;

						FuncRet = RET_ERROR;
					}
				}
			}
			else
			{
				RetVal[i] = YES;
			}
		}
		else
		{
			if (FingerInfo[D_EXIST][i] == YES)
			{
				if(b_FingerFlag[i] == FALSE)
				{
					b_FingerFlag[i] = TRUE;
					l_FingerWaitTime[i][0] = GetCurrentTime();
				}
				else if(b_FingerFlag[i] == TRUE)
				{
					l_FingerWaitTime[i][1] = GetCurrentTime();
					l_FingerWaitTime[i][2] = l_FingerWaitTime[i][1] - l_FingerWaitTime[i][0];
					if(l_FingerWaitTime[i][2] > st_wait.n_on_wait_time[47] + (i*2))
					{
						RetVal[i] = YES;
					}
				}
				else
				{
					l_FingerWaitTime[i][1] = GetCurrentTime();
					l_FingerWaitTime[i][2] = l_FingerWaitTime[i][1] - l_FingerWaitTime[i][0];
					if(l_FingerWaitTime[i][2] < 0)
					{
						l_FingerWaitTime[i][0] = GetCurrentTime();
					}

					if(l_FingerWaitTime[i][2] > st_wait.n_limit_wait_time[47] + (i*2))
					{
						//410600 1 40 "Heat Sink Picker #1 Module Off Check Error.[PS0400]."				
						sprintf(mc_alarmcode, "41060%d", i);
						alarm.mn_count_mode = 0;	
						alarm.mn_type_mode = eWARNING; 
						alarm.mn_alarm_assign_section = 188;

						FuncRet = RET_ERROR;
					}
				}
			}
			else
			{
				RetVal[i] = YES;
			}
		}
	}

	if(RetVal[0] == YES && RetVal[1] == YES)
	{
		FuncRet = RET_GOOD;
	}

	return FuncRet;
}

int CRun_Rbt_Load_RightHeat::Robot_BeforeModuleDVCPlaceMove(int n_site, int n_bin)
{
	int nRet_1, nRet_2=0, i, x, y=0, buffpos=0, num, ActionFlag = NO;

	int BeforeFuncRet = RET_PROCEED;
	switch(Before_Module_Place_Step)
	{
	case 0:
 		i_DVCRepickCnt = 0;

		l_until_wait_time[0] = GetCurrentTime();
		Before_Module_Place_Step = 1000;
		break;

	case 1000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_M_RBT_Z);
		if(md_CurrentPos > st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_RBT_Z].mn_allow)
		{
			Before_Module_Place_Step = 1100; //Z���� ���� ��ġ�� ��� 
		}
		else
		{
			Before_Module_Place_Step = 2000;
		}
		break; 

	case 1100:
 		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);		
		if(nRet_1 == BD_GOOD)
		{
			Before_Module_Place_Step = 2000;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == BD_RETRY)
		{
			Before_Module_Place_Step = 1100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Before_Module_Place_Step = 1100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1120, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2000:	// ���� ��Ŀ�� ���¸� �ľ��ϰ� , ������ �Ǹ� �κ�Ʈ �̵��� ��Ŀ�� ���¸� üũ�Ѵ� . 
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerModuleStatusCheck(PICKERSTATUS, st_modulemap.nModuleRobotPicker, mResult);
		
		if(nRet_1 == RET_GOOD)
		{
			Before_Module_Place_Step = 3000;
		}
		else if(nRet_1 == RET_ERROR)	// ��Ŀ ���� ���� 
		{	
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME)
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
				}
				break;
			}

			st_work.mn_run_status = dWARNING;
		}
		break;

	///////////////////////////////////////////////////////////////////////
	//���� ���� �� ��Ŀ ������ ���Ͽ� �κ�Ʈ�� �̵��� ��ġ�� ã�´� 
	///////////////////////////////////////////////////////////////////////
	case 3000:
		mn_FirstPicker_Y_Pos = -1; mn_FirstTray_X_Pos = -1;
		//ù��° ���� �� �ִ� ��Ŀ ��ġ�� ã�´� 
		for(x=0 ; x<PICKER_NUM; x++) 
		{
			if(st_modulemap.nModuleRobotPicker[D_EXIST][x] == DVC_YES)// && st_modulemap.PickerEnable[x] == YES) //����̽��� ����������� ��Ŀ��ȣ (�������� ã�°Ŵϱ�..)
			{
				mn_FirstPicker_Y_Pos = x;
				st_work.i_FirstPickerNum[LDMODULE_SITE] = x;
				break;
			}
		}
		if(mn_FirstPicker_Y_Pos == -1) 
		{
			Before_Module_Place_Step = 0;
			BeforeFuncRet = RET_PICKERNOTFIND; //�̰����� ��� �� �� �ִ� ��Ŀ�� �ϳ��� ���� 
			break;
		}	

		for(x=0; x<PICKER_NUM; x++)
		{
			num = x * PICKING_GAP;
			//if(num >= 0 && num < 1)
			//{
				if(st_modulemap.nModuleRobotPicker[D_EXIST][x] == DVC_YES &&
					st_modulemap.WorkBuffer[D_EXIST][n_site][LDMODULE_SITE] == DVC_NO) //����̽��� ������ 
				{
					mn_FirstTray_X_Pos = x; //������ 0

					st_modulemap.CurrentRows[WORK_BUFFER_SITE] = n_site;
					//�κ�Ʈ�� �̵��� ��ġ�� ã�´� 
					mCurrent_RowPos[WORK_BUFFER_SITE] = n_site; //nFirstTray_X_Pos - (nFirstPicker_X_Pos * PICKING_GAP);
					break;
				}
			//}

		}
		if(mn_FirstTray_X_Pos == -1) 
		{
			Before_Module_Place_Step = 0;
			BeforeFuncRet = RET_NOWHERE;	//Ʈ���̿��� �۾��� ������ ����, �ش� ���� ���̻� �۾��� ����̽��� ���� �����̴�, �Լ� �ۿ��� Ʈ���̸� Y �� ��ü�� �����Ѵ� 
			break;
		}

		////////////////////////////////////
		//���� �� ��Ŀ ������ �����Ѵ� 
		////////////////////////////////////
		ActionFlag = NO;
		for(i = 0 ; i < PICKER_NUM ; i++) ActionPicker[D_EXIST][i] = NO; // ��Ŀ ���� Ŭ���� ����.
		for(i = 0 ; i < PICKER_NUM ; i++) 
		{
			ActionPicker[D_EXIST][i] = NO;
			if(st_modulemap.nModuleRobotPicker[D_EXIST][i] == DVC_YES)
			{
				num = i * PICKING_GAP;

				if(st_modulemap.WorkBuffer[D_EXIST][n_site][LDMODULE_SITE] == NO)
				{
					////if(0 <= mn_FirstTray_X_Pos + num && mn_FirstTray_X_Pos + (num - (mn_FirstPicker_Y_Pos * PICKING_GAP)) < 1)
					//{
						ActionFlag = YES;
						ActionPicker[D_EXIST][i] = YES; // ���� ������ ��Ŀ 
						break;
					//}
				}
			}
        }

		if(ActionFlag == YES) // �ϳ��� ���� �����ϸ� �������� �̵� 
		{
			md_Robot_PlacePos = Func.XYRobot_MovePosValCheck(WORK_BUFFER_SITE, LDMODULE_SITE, n_site, st_work.i_FirstPickerNum[LDMODULE_SITE]);
			Before_Module_Place_Step = 3100;
		}
		else
		{	
			// �̹� �۾��� ���� ����,  
			Before_Module_Place_Step = 0;
			BeforeFuncRet = RET_NOWHERE; // tray change request	//st_work.n_sync_xz_to_ld = YES; 
		}
		break;	

	case 3100: //�κ�Ʈ ��ġ ���͸� Ʈ���̿��� ���� �� �ֵ��� �̵��Ѵ� 
		md_CurrentPos = COMI.Get_MotCurrentPos(M_M_RBT_Z);
		if(md_CurrentPos > st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_RBT_Z].mn_allow)
		{
			Before_Module_Place_Step = 3110; //Z���� ���� ��ġ�� ��� 
		}
		else
		{
			Before_Module_Place_Step = 3200;
		}
		break; 

	case 3110:
 		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Before_Module_Place_Step = 3200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Before_Module_Place_Step = 3110;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Before_Module_Place_Step = 3110;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1121, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3200:
		nRet_1 = CTL_Lib.Single_Move(M_M_RBT_Y, md_Robot_PlacePos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Before_Module_Place_Step = 3500;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Before_Module_Place_Step = 3200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Before_Module_Place_Step = 3200;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1123, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3500:
		Before_Module_Place_Step = 0;
		BeforeFuncRet = RET_GOOD;
		break;
		}

	return BeforeFuncRet;
}

int CRun_Rbt_Load_RightHeat::Robot_ModuleDVCPlace(int n_site, int n_bin)
{
	int nRet_1, nRet_2=0, i, x, y=0, buffpos=0, num, ActionFlag = NO;
//	int nFirstPicker_X_Pos = -1, nFirstTray_X_Pos = -1;

	int FuncRet = RET_PROCEED;
	Func.ThreadFunctionStepTrace(22, Module_Place_Step);
	switch(Module_Place_Step)
	{
	case 0:
 		i_DVCRepickCnt = 0;

		l_until_wait_time[0] = GetCurrentTime();
		Module_Place_Step = 1000;
		break;

	case 1000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);
		if(md_CurrentPos > st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_HS_B_RBT_Z].mn_allow)
		{
			Module_Place_Step = 1100; //Z���� ���� ��ġ�� ��� 
		}
		else
		{
			Module_Place_Step = 2000;
		}
		break; 

	case 1100:
 		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);	
		if(nRet_1 == BD_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Module_Place_Step = 2000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Module_Place_Step = 1100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Module_Place_Step = 1100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2222, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;


	case 2000:	// ���� ��Ŀ�� ���¸� �ľ��ϰ� , ������ �Ǹ� �κ�Ʈ �̵��� ��Ŀ�� ���¸� üũ�Ѵ� . 
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerDeviceStatusCheck(PICKERSTATUS, st_modulemap.nRighthsRobotPicker, mResult);
		
		if(nRet_1 == RET_GOOD)
		{
			Module_Place_Step = 3000;
		}
		else if(nRet_1 == RET_ERROR)	// ��Ŀ ���� ���� 
		{	
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME)
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
					break;
				}
				break;
			}

			st_work.mn_run_status = dWARNING;
		}
		break;

	///////////////////////////////////////////////////////////////////////
	//���� ���� �� ��Ŀ ������ ���Ͽ� �κ�Ʈ�� �̵��� ��ġ�� ã�´� 
	///////////////////////////////////////////////////////////////////////
	case 3000:
		mn_FirstPicker_Y_Pos = -1; mn_FirstTray_X_Pos = -1;
		//ù��° ���� �� �ִ� ��Ŀ ��ġ�� ã�´� 
		for(x=0 ; x<PICKER_NUM; x++) 
		{
			if(st_modulemap.nRighthsRobotPicker[D_EXIST][x] == DVC_YES)// && st_modulemap.PickerEnable[x] == YES) //����̽��� ����������� ��Ŀ��ȣ (�������� ã�°Ŵϱ�..)
			{
				mn_FirstPicker_Y_Pos = x;			
				st_work.i_FirstPickerNum[RIGHTSINK_SITE] = x;
				break;
			}
		}
		if(mn_FirstPicker_Y_Pos == -1) 
		{
			Module_Place_Step = 0;
			FuncRet = RET_PICKERNOTFIND; //�̰����� ��� �� �� �ִ� ��Ŀ�� �ϳ��� ���� 
			break;
		}	

		for(x=0; x<PICKER_NUM; x++)
		{
			num = x * PICKING_GAP;
			//if(num >= 0 && num < 2)
			//{
				if(st_modulemap.nRighthsRobotPicker[D_EXIST][x] == DVC_YES &&
					st_modulemap.WorkBuffer[D_EXIST][n_site][RIGHTSINK_SITE] == DVC_NO) //����̽��� ������ 
				{
					mn_FirstTray_X_Pos = x; //������ 0

					st_modulemap.CurrentRows[WORK_BUFFER_SITE] = n_site;
					//�κ�Ʈ�� �̵��� ��ġ�� ã�´� 
					mCurrent_RowPos[WORK_BUFFER_SITE] = n_site; //nFirstTray_X_Pos - (nFirstPicker_X_Pos * PICKING_GAP);
					break;
				}
			//}

		}
		if(mn_FirstTray_X_Pos == -1) 
		{
			Module_Place_Step = 0;
			FuncRet = RET_NOWHERE;	//Ʈ���̿��� �۾��� ������ ����, �ش� ���� ���̻� �۾��� ����̽��� ���� �����̴�, �Լ� �ۿ��� Ʈ���̸� Y �� ��ü�� �����Ѵ� 
			break;
		}

		////////////////////////////////////
		//���� �� ��Ŀ ������ �����Ѵ� 
		////////////////////////////////////
		ActionFlag = NO;
		for(i = 0; i < PICKER_NUM; i++)
		{
			if(st_picker.n_heat_rightldrbt_enable[i] == FALSE)
			{
				st_modulemap.nRighthsRobotPicker[D_EXIST][i] = 0;
				st_modulemap.nRighthsRobotPicker[D_BIN][i] = 0;
			}
		}

		for(i = 0 ; i < PICKER_NUM ; i++)	ActionPicker[D_EXIST][i] = NO; // ��Ŀ ���� Ŭ���� ����.
		for(i = 0 ; i < PICKER_NUM ; i++) 
		{
			ActionPicker[D_EXIST][i] = NO;
			if(st_modulemap.nRighthsRobotPicker[D_EXIST][i] == DVC_YES)// && st_modulemap.PickerEnable[i] == YES) // ����̽��� ���� ��Ŀ�� üũ
			{
				num = i * PICKING_GAP;

				if(st_modulemap.WorkBuffer[D_EXIST][n_site][RIGHTSINK_SITE] == DVC_NO)
				{
					//if(0 <= mn_FirstTray_X_Pos + num && mn_FirstTray_X_Pos + (num - (mn_FirstPicker_Y_Pos * PICKING_GAP)) < 1)
					//{
						ActionFlag = YES;
						ActionPicker[D_EXIST][i] = YES; // ���� ������ ��Ŀ 
						break;
					//}
				}
			}
        }

		if(ActionFlag == YES) // �ϳ��� ���� �����ϸ� �������� �̵� 
		{//2012,0905
			md_Robot_PlacePos = Func.XYRobot_MovePosValCheck(WORK_BUFFER_SITE, RIGHTSINK_SITE, n_site, st_work.i_FirstPickerNum[RIGHTSINK_SITE]);
			Module_Place_Step = 3100;
		}
		else
		{	
			// �̹� �۾��� ���� ����,  
			Module_Place_Step = 0;
			FuncRet = RET_NOWHERE; // tray change request	//st_work.n_sync_xz_to_ld = YES; 
		}
		break;	

	case 3100: //�κ�Ʈ ��ġ ���͸� Ʈ���̿��� ���� �� �ֵ��� �̵��Ѵ� 
		md_CurrentPos = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);
		if(md_CurrentPos > st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_HS_B_RBT_Z].mn_allow)
		{
			Module_Place_Step = 3110; //Z���� ���� ��ġ�� ��� 
		}
		else
		{
			Module_Place_Step = 3200;
		}
		break; 

	case 3110:
 		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Module_Place_Step = 3200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Module_Place_Step = 3110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Module_Place_Step = 3110;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2223, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3200:
	//�κ�Ʈ�� �۾� �� ��ġ�� �̵��Ѵ� 
		md_CurrentPos = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);
		if(md_CurrentPos > st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_HS_B_RBT_Z].mn_allow)
		{
			Module_Place_Step = 3100; //Z���� ���� ��ġ�� ��� 
		}
		else
		{
			Module_Place_Step = 3300;
		}
		break;

	case 3300: // ���ۿϷ� �� ��ġ üũ 
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, md_Robot_PlacePos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Module_Place_Step = 3500;
		}
		if(nRet_1 == BD_RETRY)
		{
			Module_Place_Step = 3300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Module_Place_Step = 3300;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2224, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 3500:	 // ������ ��Ŀ�� ������ ��ŷ�ϴ� ���� 
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerDeviceStatusCheck(PICKERSTATUS, st_modulemap.nRighthsRobotPicker, mResult); // ��Ŀ ������ ���� ���� ���¸� üũ 
		
		if(nRet_1 == RET_ERROR)
		{	
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME)
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
					break;
				}
				break;
			}
			st_work.mn_run_status = dWARNING;
			break;
		}

		Module_Place_Step = 4000;
		break;		

	///////////////////////////////////////////////////////////////////////
	//��û�� ������ ��ġ�� �غ� �Ǳ⸦ ��ٸ��� 
	///////////////////////////////////////////////////////////////////////
	case 4000:
//		if(n_site == WORK_BUFFER_1)
//		{
//			if(st_sync.n_module_ldrbt_to_work1[1][0] == CTL_READY) //���� ��ġ�� �غ� �Ǿ��� 
//			{
//				//���� ����!				
//				Module_Place_Step = 4100;		
//			}
//		}
//		else if(n_site == WORK_BUFFER_2)
//		{
//			if(st_sync.n_module_ldrbt_to_work2[1][0] == CTL_READY) //���� ��ġ�� �غ� �Ǿ��� 
//			{
//				Module_Place_Step = 4100;				
//			}
//		}
//		else if(n_site == WORK_BUFFER_3)
//		{
//			if(st_sync.n_module_ldrbt_to_work3[1][0] == CTL_READY) //���� ��ġ�� �غ� �Ǿ��� 
//			{
//				Module_Place_Step = 4100;		
//			}
//		}
//		else// if(n_site == WORK_BUFFER_2)
//		{
//			if(st_sync.n_module_ldrbt_to_work4[1][0] == CTL_READY) //���� ��ġ�� �غ� �Ǿ��� 
//			{
//				Module_Place_Step = 4100;				
//			}
//		}
		if (st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][n_site][1] == CTL_READY)
		{
			Module_Place_Step = 4010;
		}
		break;

	case 4010:
		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][n_site][1] = CTL_DOREADY;
		Module_Place_Step = 4100;
		break;
		
	case 4100:	//��Ŀ �� Z axis down action
		if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][n_site][1] != CTL_POSSIBLE) break;

		nRet_1 = Robot_CurrentPos_Check(md_Robot_PlacePos, -1);
		if(nRet_1 == CTL_YES) //���� �κ�Ʈ ��ġ�� ��ǥ ��ġ�� �ƴϴ�
		{
			Module_Place_Step = 0; //�ٽ� ó�� ���� ���� �Ѵ� 
		}
		else
		{
			Module_Place_Step = 4110;
		}
		break;

	case 4110:		
		Set_PickerUpDown(IO_ON, ActionPicker);
 		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_BUFFER_PLACE1+n_site], st_basic.nRunSpeed);		
		if(nRet_1 == BD_GOOD)
		{
			Module_Place_Step = 4200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Module_Place_Step = 4110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Module_Place_Step = 4110;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2225, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4200: //picker down action check
		nRet_1 = Get_PickerUpDown(IO_ON, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			Module_Place_Step = 4210;
		}
		else if(nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = dWARNING;
			Module_Place_Step = 4100;
		}
		break;

	case 4210:
		st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][n_site][1] = CTL_COMPLETE;//20120730
		Module_Place_Step = 4220;
		break;

	case 4220:
//       if(st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][n_site][1] == CTL_BUSY)
//       {
	    Module_Place_Step = 4300;
//       }
	//	Module_Place_Step = 4300;
		break;

	case 4300: //Pick ����̽� ���� ���´� 
		Set_FingerOnOff(IO_ON, ActionPicker);
		Module_Place_Step = 4310;
		break;

	case 4310:
		nRet_1 = Get_FingerOnOff(IO_ON, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Module_Place_Step = 4400;
		}
		else if(nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			Module_Place_Step = 4300;
		}
		break;	

	case 4400: //����̽� �������� üũ 
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerDeviceStatusCheck(IO_OFF, ActionPicker, mResult); // ��Ŀ ������ ���� ���� ���¸� üũ 

		if(nRet_1 == RET_GOOD)
		{
			Module_Place_Step = 4500;	
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //james 0121 ���縦 üũ�� �� �ִ� ���� �ð��� �ش�  
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
					break;
				}
				break;
			}

			for(i = 0 ; i < PICKER_NUM ; i++)
			{
				mTempPicker[D_EXIST][i] = NO;

				if(ActionPicker[D_EXIST][i] == YES && mResult[i] == NO)
				{	
					//���� �߻��� ������ �ش� ��Ʈ�� ���� ��Ű�� 
					mTempPicker[D_EXIST][i] = YES;
				}
			}
 			Module_Place_Step = 4500;	 // ���� �ΰ� üũ�Ѵµ� ������ �־� ������ �ȵȴ� 
		}
		break;
 
	case 4500: // ���� �� and picker up
		Set_PickerUpDown(IO_OFF, ActionPicker);		 
 		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
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
			Module_Place_Step = 4500;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2226, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4700: //picker up action check
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);

		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Module_Place_Step = 5000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			Module_Place_Step = 4500;
			st_work.mn_run_status = dWARNING;
		}
		break;

	//////////////////////////////////////////////////////////
	// �۾��� ��Ŀ �� Ʈ���� ������ �����Ѵ� 
	//////////////////////////////////////////////////////////
	case 5000:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		
		nRet_1 = Func.PickerDeviceStatusCheck(IO_OFF, ActionPicker, mResult); // ��Ŀ ������ ���� ���� ���¸� üũ 
		
		if(nRet_1 == RET_ERROR) //james 2010,0121
		{
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //���縦 üũ�� �� �ִ� ���� �ð��� �ش�  
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
					break;
				}
				break;
			}
		}
		
		for(i = 0 ; i < PICKER_NUM ; i++) 
		{
			if(ActionPicker[D_EXIST][i] == YES && mResult[i] == YES) // ����̽��� �ִ� ��Ŀ�� üũ
			{
				num = i * PICKING_GAP;

				st_modulemap.WorkBuffer[D_EXIST][n_site][RIGHTSINK_SITE] = DVC_YES; //WORK_BUFFER_1 2 3 4
				st_modulemap.WorkBuffer[D_BIN][n_site][RIGHTSINK_SITE] = BIN_LDBUFFERBIN;		// BIN_LOADTRAYBIN
				st_modulemap.WorkBuffer[D_RE_CNT][n_site][RIGHTSINK_SITE] = T_NOTTEST;			// Test Count Reset
				st_modulemap.WorkBuffer[D_SCR_BIN][n_site][RIGHTSINK_SITE] = BIN_CLEAR;			// Screen Bin

				st_modulemap.nRighthsRobotPicker[D_EXIST][i]   = DVC_NO;
				st_modulemap.nRighthsRobotPicker[D_BIN][i]     = BIN_CLEAR;		// BIN_LOADTRAYBIN
				st_modulemap.nRighthsRobotPicker[D_RE_CNT][i]  = T_NOTTEST;		// Test Count Reset
				st_modulemap.nRighthsRobotPicker[D_SCR_BIN][i] = BIN_CLEAR;		// Screen Bin

				buffpos = (mn_FirstTray_X_Pos + num) % 2;

				//if(mn_FirstTray_X_Pos + num >= 0 && mn_FirstTray_X_Pos + num <= 1)
				//{
					if(st_sync.n_module_site_status[RIGHTSINK_SITE][n_site][0] != BUFF_DVC_LOAD)
					{
						st_sync.n_module_site_status[RIGHTSINK_SITE][n_site][0] = BUFF_DVC_LOAD;
					}
				//}

				ActionPicker[D_EXIST][i] = NO;  
				mResult[i] = NO;
			}
		}

		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_HS_RIGHT_PICKER);			// ��Ŀ ���� 
		}

		if(nRet_1 == RET_GOOD)
		{
			Module_Place_Step = 6000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			i_DVCRepickCnt++;

			if(i_DVCRepickCnt >= st_basic.mn_Dvc_Repick_Cnt) //20120730
			{
				i_DVCRepickCnt = 0;		//��ŷ �絿���� ī��Ʈ�� �ʱ�ȭ �Ǵ°� ����!
				
				Module_Place_Step = 1000;
				st_work.mn_run_status = dWARNING;
			}
			else 
			{
				Module_Place_Step = 1000;
			}
		}
		break;

	case 6000: //���������� ��� ������� 
		Module_Place_Step = 0;
		FuncRet = RET_GOOD;			
		break;
	}

	return FuncRet;	
} 

int CRun_Rbt_Load_RightHeat::Robot_TrayDVCPick(int n_bin, int n_tray_y)
{
	int nRet_1, i=0, x, y=0, num, FuncRet, ActionFlag = NO, nPick = 0;
	int nLoadPick;
	

	FuncRet = RET_PROCEED;
	Func.ThreadFunctionStepTrace(23, Tray_Pick_Step);

	switch(Tray_Pick_Step)
	{
	case 0:
		mn_FirstPicker_X_Pos = -1;
		if(st_sync.n_righths_ldrbt_tray_req[0] != CTL_READY) //���� Ʈ���̰� �غ���� �ʾҴ� 
		{
			for(x=0 ; x<PICKER_NUM; x++) 
			{
				if(st_modulemap.nRighthsRobotPicker[D_EXIST][x] == DVC_NO) //����̽��� ����������� ��Ŀ��ȣ (�������� ã�°Ŵϱ�..)
				{
					mn_FirstTray_X_Pos  = 0;
					mn_FirstPicker_X_Pos = x; 
					break;
				}
			}

			if(mn_FirstPicker_X_Pos >= 0)
			{
				mCurrent_RowPos[RIGHTSINK_SITE] = mn_FirstTray_X_Pos - (mn_FirstPicker_X_Pos * PICKING_GAP);

				md_Robot_Pos = Func.XYRobot_MovePosValCheck(RIGHTSINK_SITE, 0, 0, mCurrent_RowPos[RIGHTSINK_SITE]); //�̸� ��Ŀ�� ����Ų�� 
				Tray_Pick_Step = 10;
				break;
			}
		}
 		i_DVCRepickCnt = 0;

		l_until_wait_time[0] = GetCurrentTime();
		Tray_Pick_Step = 1000;
		break;

	case 10:
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, md_Robot_Pos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			RunStep = 30;
		}
		else if(nRet_1 == BD_RETRY)
		{
			RunStep = 10;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			RunStep = 10;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2227, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 30://Ʈ���̰� �غ� �Ǳ⸦ ��ٸ��� 
		if(st_sync.n_righths_ldrbt_tray_req[0] == CTL_READY)
		{
			Tray_Pick_Step = 0;
		}
		break;


	case 1000:
//		md_CurrentPos = COMI.Get_MotCurrentPos(M_HS_B_RBT_Y);//20120730
		md_CurrentPos = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);//20120730
		if(md_CurrentPos > st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_HS_B_RBT_Z].mn_allow)
		{
			Tray_Pick_Step = 1100; //Z���� ���� ��ġ�� ��� 
		}
		else
		{
			Tray_Pick_Step = 2000;
		}
		break; 

	case 1100:
 		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		
		if(nRet_1 == BD_GOOD)
		{
			Tray_Pick_Step = 2000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 1100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 1100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2228, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 2000:	// ���� ��Ŀ�� ���¸� �ľ��ϰ� , ������ �Ǹ� �κ�Ʈ �̵��� ��Ŀ�� ���¸� üũ�Ѵ� .
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerDeviceStatusCheck(PICKERSTATUS, st_modulemap.nRighthsRobotPicker, mResult);
		
		if(nRet_1 == RET_GOOD)
		{
			Tray_Pick_Step = 3000;
		}
		else if(nRet_1 == RET_ERROR)	// ��Ŀ ���� ���� 
		{
			if(l_until_wait_time[2] < 0)
			{
				l_until_wait_time[0] = GetCurrentTime();
				break;
			}
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME)
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
					break;
				}
				break;
			}

			st_work.mn_run_status = CTL_dWARNING;
		}
		break;

	//Ʈ���� ���� �� ��Ŀ ������ ���Ͽ� �κ�Ʈ�� �̵��� ��ġ�� ã�´� 
	case 3000:
		if (st_picker.n_heat_rightldrbt_enable[0] == NO && st_picker.n_heat_rightldrbt_enable[1] == NO )	
		{
			sprintf(mc_alarmcode, "990003");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2229, CTL_dWARNING, mc_alarmcode);
			break;
		}

		///////////// 20120730
		//if(0 && (st_work.n_loadlot_count >= st_work.n_lot_count || st_work.n_lotend == YES))
		//{
		//	FuncRet = RET_LOADTRAYSKIP;
		//	break;
		//}
		/////////////
		mn_FirstPicker_Y_Pos = -1; mn_FirstTray_X_Pos = -1; mn_FirstTray_Y_Pos = -1;
		//ù��° ���� �� �ִ� ��Ŀ ��ġ�� ã�´� 
		for(x=0 ; x<PICKER_NUM; x++) 
		{
			if(st_modulemap.nRighthsRobotPicker[D_EXIST][x] == DVC_NO && 
				st_picker.n_heat_rightldrbt_enable[x] == YES)
			{
				mn_FirstPicker_Y_Pos = x;
				st_work.i_FirstPickerNum[RIGHTSINK_SITE] = x;
				break;
			}
		}
		if(mn_FirstPicker_Y_Pos == -1) 
		{
			Tray_Pick_Step = 0;
			FuncRet = RET_PICKERNOTFIND; //�̰����� ��� �� �� �ִ� ��Ŀ�� �ϳ��� ���� 
			break;
		}

		//ù��° ���� �� �ִ� tray y ������ ã�´� 
		for(x=0 ; x<st_traybuffer[RIGHTSINK_SITE].i_loader_row; x++)   //st_traybuffer[RIGHTSINK_SITE].i_loader_row=1,st_traybuffer[RIGHTSINK_SITE].i_loader_col=14
		{
			if(st_modulemap.RighthsTray[1][x] == DVC_YES)
			{				
				mn_FirstTray_Y_Pos = x;
				mn_FirstTray_X_Pos = 0;
				break;
			}
		}
		if(mn_FirstTray_X_Pos == -1 || mn_FirstTray_Y_Pos == -1) 
		{
			Tray_Pick_Step = 0;
			FuncRet = RET_NOWHERE;	//Ʈ���̿��� �۾��� ������ ����, �ش� ���� ���̻� �۾��� ����̽��� ���� �����̴�, �Լ� �ۿ��� Ʈ���̸� Y �� ��ü�� �����Ѵ� 
			break;
		}
	
		//�κ�Ʈ�� �̵��� ��ġ�� ã�´� 
		mCurrent_RowPos[RIGHTSINK_SITE] = mn_FirstTray_Y_Pos - (mn_FirstPicker_Y_Pos * PICKING_GAP);

		////////////////////////////////////
		//���� �� ��Ŀ ������ �����Ѵ� 
		////////////////////////////////////
		ActionFlag = NO;
		for(i = 0 ; i < PICKER_NUM ; i++) 
		{
			ActionPicker[D_EXIST][i] = NO; // ��Ŀ ���� Ŭ���� ����.
			mTempPicker[D_EXIST][i] = NO;
		}

		if(st_work.nMdlInputCount[0][0] >= (st_work.n_loadlot_count[LDMODULE_SITE] + 2))
		{
			nLoadPick = PICKER_NUM;
		}
		else
		{
			/*if(st_work.n_lotend != CTL_YES)
			{
				nLoadPick = 1;
			}
			else
			{
				if(Func.ChkForLotEnd(RIGHTSINK_SITE) == RET_GOOD) nLoadPick = PICKER_NUM;
				else if(Func.ChkForLotEnd(RIGHTSINK_SITE) == RET_PROCEED) nLoadPick = 1;
				else
				{
					nLoadPick = 0;
					Tray_Pick_Step = 0;
					FuncRet = RET_LOADTRAYSKIP;
					break;
				}
			}*/
			if(Func.ChkForLotEnd(RIGHTSINK_SITE) == RET_GOOD) nLoadPick = PICKER_NUM;
			else if(Func.ChkForLotEnd(RIGHTSINK_SITE) == RET_PROCEED) nLoadPick = 1;
			else
			{
				if(st_work.n_lotend == CTL_YES)
				{
					nLoadPick = 0;
					Tray_Pick_Step = 0;
					FuncRet = RET_LOADTRAYSKIP;
					break;
				}
				else
				{
					break;
				}
			}
		}	


		for(i = 0 ; i < PICKER_NUM ; i++) 
		{
			if(nPick >= nLoadPick)
			{
				break;
			}
			ActionPicker[D_EXIST][i] = NO;
			if(st_modulemap.nRighthsRobotPicker[D_EXIST][i] == NO && 
				st_picker.n_heat_rightldrbt_enable[i] == YES) // ����̽��� ���� ��Ŀ�� üũ
			{
				num = i * PICKING_GAP;

				if(st_modulemap.RighthsTray[1][mn_FirstTray_Y_Pos + (num - (mn_FirstPicker_Y_Pos * PICKING_GAP))] == YES)
				{
					//if(0 <= mn_FirstTray_Y_Pos + num && mn_FirstTray_Y_Pos + (num - (mn_FirstTray_Y_Pos + PICKING_GAP)) < st_traybuffer[RIGHTSINK_SITE].i_loader_row)
					if(0 <= mn_FirstTray_Y_Pos + num && ((mn_FirstTray_Y_Pos + num) < st_traybuffer[RIGHTSINK_SITE].i_loader_row))
					{

						if(st_picker.n_heat_rightldrbt_enable[i] == CTL_YES)
						{
							nPick++;//20120730
							ActionFlag = YES;
							ActionPicker[D_EXIST][i] = YES; // ���� ������ ��Ŀ
							if(0 && ((st_work.n_loadlot_count[LDMODULE_SITE] + nPick) >= st_work.n_lot_count || st_work.n_lotend == YES))
							{
								break;
							}
						}
					}
				}
			}
 			if(ActionFlag == YES) break;//2012,0917
       }

		if(ActionFlag == YES) // �ϳ��� ���� �����ϸ� �������� �̵� 
		{
			Tray_Pick_Step = 3100;
		}
		else
		{	
			// �̹� �۾��� ���� ����,  
			Tray_Pick_Step = 0;
			FuncRet = RET_NOWHERE; // tray change request	//st_work.n_sync_xz_to_ld = YES; 
		}
		break;	

	case 3100: //�κ�Ʈ ��ġ ���͸� Ʈ���̿��� ���� �� �ֵ��� �̵��Ѵ� 
		md_CurrentPos = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);
		if(md_CurrentPos > st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_HS_B_RBT_Z].mn_allow)
		{
			Tray_Pick_Step = 3110; //Z���� ���� ��ġ�� ��� 
		}
		else
		{
			Tray_Pick_Step = 3200;
		}
		break; 

	case 3110:
 		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		
		if(nRet_1 == BD_GOOD)
		{
			Tray_Pick_Step = 3200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 3110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 3110;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2230, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;


	case 3200:
		//2012,0905
		md_Robot_Pos = Func.XYRobot_MovePosValCheck(RIGHTSINK_SITE, 0, st_work.i_FirstPickerNum[RIGHTSINK_SITE], mCurrent_RowPos[RIGHTSINK_SITE]);
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, md_Robot_Pos, st_basic.nRunSpeed);
	
		if(nRet_1 == BD_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Tray_Pick_Step = 3300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 3200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 3200;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2231, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	
	case 3300: // ������ ��Ŀ�� ������ ��ŷ�ϴ� ���� 
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerDeviceStatusCheck(PICKERSTATUS, st_modulemap.nRighthsRobotPicker, mResult); // ��Ŀ ������ ���� ���� ���¸� üũ 
		
		if(nRet_1 == RET_ERROR)
		{
			//2010,0531
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //���縦 üũ�� �� �ִ� ���� �ð��� �ش�  
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
					break;
				}
				break;
			}
			st_work.mn_run_status = dWARNING;
			break;
		}

		Tray_Pick_Step = 4000;		
		break;

	case 4000: // moving after finger open
		Set_FingerOnOff(IO_ON, ActionPicker);
		Tray_Pick_Step = 4010;
		break;

	case 4010:
		nRet_1 = Get_FingerOnOff(IO_ON, ActionPicker);

		if(nRet_1 == RET_GOOD)
		{
			Tray_Pick_Step = 4100;
		}
		else if(nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;
			Tray_Pick_Step = 4000;
		}
		break;		

	case 4100:
		nRet_1 = Robot_CurrentPos_Check(md_Robot_Pos, -1);
		if(nRet_1 == CTL_YES) //���� �κ�Ʈ ��ġ�� ��ǥ ��ġ�� �ƴϴ�
		{
			Tray_Pick_Step = 0; //�ٽ� ó�� ���� ���� �Ѵ� 
			break;
		}

		Set_PickerUpDown(IO_ON, ActionPicker);

 		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_TRAY_PICK], st_basic.nRunSpeed);
		
		if(nRet_1 == BD_GOOD)
		{
			Tray_Pick_Step = 4200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 4100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 4100;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2232, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4200: //picker down action check
		nRet_1 = Get_PickerUpDown(IO_ON, ActionPicker);

		if(nRet_1 == RET_GOOD)
		{
			Tray_Pick_Step = 4300;
		}
		else if(nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = dWARNING;
			Tray_Pick_Step = 4100;
		}
		break;

	case 4300: //Pick ����̽� Ŭ���� 
		Set_FingerOnOff(IO_OFF, ActionPicker);
		Tray_Pick_Step = 4310;
		break;

	case 4310:
		nRet_1 = Get_FingerOnOff(IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Tray_Pick_Step = 4400;
		}
		else if(nRet_1 == RET_ERROR)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Tray_Pick_Step = 4300;
		}
		break;	

	case 4400: //����̽� �������� üũ 
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerDeviceStatusCheck(IO_ON, ActionPicker, mResult); // ��Ŀ ������ ���� ���� ���¸� üũ 

		if(nRet_1 == RET_GOOD)
		{
			Tray_Pick_Step = 4500;	
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //���縦 üũ�� �� �ִ� ���� �ð��� �ش�  
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
				}
				break;
			}

			for(i = 0 ; i < PICKER_NUM ; i++)
			{
				mTempPicker[D_EXIST][i] = NO;

				if(ActionPicker[D_EXIST][i] == YES && mResult[i] == NO)
				{	
					//���� �߻��� ������ �ش� ��Ʈ�� ���� ��Ű�� 
					mTempPicker[D_EXIST][i] = YES;
				}
			}
 			Tray_Pick_Step = 4500;	 // ���� �ΰ� üũ�Ѵµ� ������ �־� ������ �ȵȴ� 
		}
		break;
 
	case 4500: // ���� �� and picker up
		Set_PickerUpDown(IO_OFF, ActionPicker);		
		if(st_basic.n_mode_device == 1 && st_basic.n_mode_hs_direction == YES)
		{
			Tray_Pick_Step = 4510;
		}
		else
		{
			nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
			if(nRet_1 == BD_GOOD)
			{
				Tray_Pick_Step = 4600;
			}
			else if(nRet_1 == BD_RETRY)
			{
				Tray_Pick_Step = 4500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				Tray_Pick_Step = 4500;
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(2233, st_work.mn_run_status, COMI.mc_alarmcode);
			}
		}
		break;

	case 4510:
 		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_DIRECTION_CHECK], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Tray_Pick_Step = 4520;
		}
		else if (nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 4510;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 4510;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2260, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 4520:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);

		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Tray_Pick_Step = 4530;
		}
		else if(nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = dWARNING;
			Tray_Pick_Step = 4500;
		}
		break;

	case 4530:
		if(st_basic.n_mode_device == 1 && st_basic.n_mode_hs_direction == YES)
		{
			if(FAS_IO.get_in_bit(st_io.i_hs_bwd_rbt_direction_1, IO_ON) == IO_ON &&
			   FAS_IO.get_in_bit(st_io.i_hs_bwd_rbt_direction_2, IO_OFF) == IO_OFF)
			//if(ActionPicker[D_EXIST][0] == DVC_YES && FAS_IO.get_in_bit(st_io.i_hs_bwd_rbt_direction_1, IO_ON) == IO_ON || 
			//	ActionPicker[D_EXIST][1] == DVC_YES && FAS_IO.get_in_bit(st_io.i_hs_bwd_rbt_direction_2, IO_OFF) == IO_OFF)
			{
				Tray_Pick_Step = 4540;
			}
			else
			{//420700 1 41 "Forward Heat Sink Direction Error"
				l_until_wait_time[1] = GetCurrentTime();
				l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
				if(l_until_wait_time[2] < 0) l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
				
				nRet_1 = Func.PickerDeviceStatusCheck(IO_ON, ActionPicker, mResult); // ��Ŀ ������ ���� ���� ���¸� üũ 
				
				if(nRet_1 == RET_ERROR)
				{
					if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //���縦 üũ�� �� �ִ� ���� �ð��� �ش�  
					{
						break;
					}
					Tray_Pick_Step = 4540;
				}
				else
				{
					if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME)	break;
					Tray_Pick_Step = 4535;
				}

			}
		}
		else
		{
			Tray_Pick_Step = 4540;
		}
		break;

	case 4535:
		nRet_1 = Robot_HSDVCTrayPlace(mCurrent_RowPos[RIGHTSINK_SITE], ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			sprintf(mc_alarmcode, "420700");
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2261, st_work.mn_run_status, mc_alarmcode);
			Tray_Pick_Step = 0;
		}
		break;


	case 4540:
 		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
	
		if(nRet_1 == BD_GOOD)
		{
			Tray_Pick_Step = 4600;
		}
		else if (nRet_1 == BD_RETRY)
		{
			Tray_Pick_Step = 4500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			Tray_Pick_Step = 4500;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2262, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;
	
	case 4600: //picker up action check
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);

		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			Tray_Pick_Step = 5000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			Tray_Pick_Step = 4500;
			st_work.mn_run_status = dWARNING;
		}
		break;

	//////////////////////////////////////////////////////////
	// �۾��� ��Ŀ �� Ʈ���� ������ �����Ѵ� 
	//////////////////////////////////////////////////////////
	case 5000:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		
		nRet_1 = Func.PickerDeviceStatusCheck(IO_ON, ActionPicker, mResult); // ��Ŀ ������ ���� ���� ���¸� üũ 
		
		if(nRet_1 == RET_ERROR) //james 2010,0121
		{
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //���縦 üũ�� �� �ִ� ���� �ð��� �ش�  
			{
				if(l_until_wait_time[2] < 0) l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
				break;
			}
		}
		
		for(i = 0 ; i < PICKER_NUM ; i++) 
		{
			if(ActionPicker[D_EXIST][i] == YES && mResult[i] == NO)
			{	
				//���� �߻��� ������ �ش� ��Ʈ�� ���� ��Ű�� 
				mTempPicker[D_EXIST][i] = YES;
			}
			if(ActionPicker[D_EXIST][i] == YES && mResult[i] == YES) // ����̽��� �ִ� ��Ŀ�� üũ
			{
				num = i * PICKING_GAP;
				
				//2012,0615
				//st_modulemap.LoadTray[1][mn_FirstTray_Y_Pos + (num - (mn_FirstPicker_Y_Pos * PICKING_GAP))] = YES;
				st_modulemap.RighthsTray[1][mn_FirstTray_Y_Pos + (num - (mn_FirstPicker_Y_Pos * PICKING_GAP))] = NO; //Ʈ���̿� ����̽��� ������ ��

				st_modulemap.nRighthsRobotPicker[D_EXIST][i] = DVC_YES;
				st_modulemap.nRighthsRobotPicker[D_BIN][i] = BIN_LDBUFFERBIN;		// BIN_LOADTRAYBIN
				st_modulemap.nRighthsRobotPicker[D_RE_CNT][i] = T_NOTTEST;			// Test Count Reset
				st_modulemap.nRighthsRobotPicker[D_SCR_BIN][i] = BIN_CLEAR;			// Screen Bin

				
//				if(st_basic.n_mode_barcode == TRUE)
//				{
//					//st_modulemap.BufferLeft[st_modulemap.CurrentRows[BUFFERTRAY1] + (num - (st_work.i_FirstPickerNum[BUFFERTRAY1] * PICKING_GAP))] = st_modulemap.strBcr_DeviceInRobotPicker[i];
//					st_modulemap.strBcr_DeviceInRobotPicker[i] = _T("NO CODE");
//				}

				ActionPicker[0][i] = NO;  
				mResult[i] = NO;
			}

		}

		if(st_handler.cwnd_main != NULL)
		{			
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_PICKERINFO, MOT_HS_RIGHT_PICKER);			// ��Ŀ ���� 
			st_handler.cwnd_main->PostMessage(WM_WORK_END, TRAY_INFO, MOT_HS_RIGHT_TRAY);			// ��Ŀ ���� 
		}

		if(nRet_1 == RET_GOOD)
		{
			mn_Clipretry = 0;
			Tray_Pick_Step = 6000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			i_DVCRepickCnt++;

			if(i_DVCRepickCnt >= st_basic.mn_Dvc_Repick_Cnt) //20120730
			{
				i_DVCRepickCnt = 0;		//��ŷ �絿���� ī��Ʈ�� �ʱ�ȭ �Ǵ°� ����!

/////////////////////////// 20120730
				if(Func.Find_TrayInfo_Count(RIGHTSINK_SITE, CTL_YES, -1) <= 0)
				{	
					FuncRet = RET_NOWHERE;				// Tray�� Device�� ���ٰ� ���� �����ϰ�, ���� Tray�� ��û�Ѵ�.
					Tray_Pick_Step = 0;
					break;
				}			
				else
				{
///////////////////////////
					//�ش� Ʈ���� ������ Ŭ���� ��Ű�� ��� ����. 
					for(i = 0 ; i < PICKER_NUM ; i++) 
					{
						num = i * PICKING_GAP;
						
						if(ActionPicker[0][i] == YES && mResult[i] == NO) // �����ߴµ� ����̽��� ���� ��Ŀ�� üũ
						{
							if(mTempPicker[D_EXIST][i] == YES)
							{
								st_modulemap.RighthsTray[1][mn_FirstTray_Y_Pos + (num - (mn_FirstPicker_Y_Pos * PICKING_GAP))]  = NO; //Ʈ���̿� ����̽��� ������ ��
							}
						}
					}					
					if(st_handler.cwnd_main != NULL)
					{
						st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOADINFO, 0);  //�δ� ���� 
					}

					//2015.0131
					mn_Clipretry++;
					if( mn_Clipretry >= st_basic.mn_Dvc_Repick_Cnt)
					{//900302 1 90 "Right Heat Sink device pick error in tray."
						mn_Clipretry = 0;
						sprintf(mc_alarmcode, "900302");
						st_work.mn_run_status = dWARNING;
						CTL_Lib.Alarm_Error_Occurrence(3299, st_work.mn_run_status, mc_alarmcode);
					}
					Tray_Pick_Step = 1000;
				}
			}
			else 
			{
				Tray_Pick_Step = 1000;
			}
		}
		break;

	case 6000: //���������� ��� ������� 
		nRet_1 = Func.Find_Picker_Count(RIGHTSINK_SITE, CTL_NO, CTL_YES, BIN_NOTUESD); //��Ŀ���� ����̽� ��� ����? 

		//���縦 �ѹ��̶� ���� �ں��� loader off�� �ǵ��� ����.. (�δ�/��δ� ī��Ʈ �� ���� ���� flow�� ���߱� ����)
		if(st_work.i_robot_device_loading != TRUE)
		{
			st_work.i_robot_device_loading = TRUE;
		}

		Tray_Pick_Step = 0;
		FuncRet = RET_GOOD;			
		break;
	}

	return FuncRet;
}

int CRun_Rbt_Load_RightHeat::Robot_CurrentPos_Check(double y_pos, double z_pos)
{
	double dCurPos[4]; //x,y,z,p
	int nFlag = CTL_NO;

	if(COMI.mn_simulation_mode == 1) // 20130509
	{
		return CTL_NO;
	}

	dCurPos[0] = COMI.Get_MotCurrentPos(M_HS_B_RBT_Y);
	dCurPos[1] = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);

	if ((y_pos < ((double)dCurPos[0] - (double)(st_motor[M_HS_B_RBT_Y].mn_allow))) 
		|| (y_pos > ((double)dCurPos[0] + (double)(st_motor[M_HS_B_RBT_Y].mn_allow))))
	{
		nFlag = CTL_YES;
	}
//	else if (z_pos != -1 && (z_pos < ((double)dCurPos[2] - (double)(st_motor[M_M_RBT_Z].mn_allow))) 
//		|| (z_pos > ((double)dCurPos[2] + (double)(st_motor[M_M_RBT_Z].mn_allow))))
//	{
//		nFlag = CTL_YES;
//	}
	

	return nFlag;
	//nFlag => YES:��ġ�� �����ʴ�, NO:���� ��ġ�� ��ǥ��ġ�� ��ġ�Ѵ� 
}

int CRun_Rbt_Load_RightHeat::Robot_AutoMove_Safety_Zone(int n_mode, int n_site)
{
	int nRet_1=0, nFuncRet = RET_PROCEED;

	switch(SafetyZone_Step)
	{
	case 0:
		md_SafetyPos = Func.XYRobot_MovePosValCheck(n_site, 0, 0);
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, md_SafetyPos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			SafetyZone_Step = 200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			SafetyZone_Step = 0;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			SafetyZone_Step = 0;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2234, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 200:
		nFuncRet = CTLBD_RET_GOOD;
		SafetyZone_Step = 0;
		break;
	}

	return nFuncRet;
}


int CRun_Rbt_Load_RightHeat::Robot_HSDVCTrayPlace(int nRow, int ActionPicker[D_INFOSIZE][PICKER_NUM])
{
	int nRet_1;
	int nFuncRet = RET_PROCEED;
	double md_CurrentPos;

	switch(TrayPlaceStep)
	{
	case 0:
		l_until_wait_time[0] = GetCurrentTime();
		TrayPlaceStep = 1000;
		break;

	case 1000:
		md_CurrentPos = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);
		if(md_CurrentPos > st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_HS_B_RBT_Z].mn_allow)
		{
			TrayPlaceStep = 1100; //Z���� ���� ��ġ�� ��� 
		}
		else
		{
			TrayPlaceStep = 2000;
		}
		break; 

	case 1100:
 		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);		
		if(nRet_1 == BD_GOOD)
		{
			TrayPlaceStep = 2000;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == BD_RETRY)
		{
			TrayPlaceStep = 1100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2135, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlaceStep = 1100;
		}
		break;

	case 2000:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		nRet_1 = Func.PickerModuleStatusCheck(PICKERSTATUS, ActionPicker, mResult);
		
		if(nRet_1 == RET_GOOD)
		{
			TrayPlaceStep = 3000;
		}
		else if(nRet_1 == RET_ERROR)	// ��Ŀ ���� ���� 
		{	
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME)
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
				}
				break;
			}

			st_work.mn_run_status = dWARNING;
		}
		break;	

	case 3000:
		md_Robot_TrayPlacePos = Func.XYRobot_MovePosValCheck(RIGHTSINK_SITE, 0, 0, nRow);
		nRet_1 = CTL_Lib.Single_Move(M_HS_B_RBT_Y, md_Robot_TrayPlacePos, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			TrayPlaceStep = 3100;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == BD_RETRY)
		{
			TrayPlaceStep = 3000;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2136, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlaceStep = 3000;
		}
		break;

	case 3100:
		Set_PickerUpDown(IO_ON, ActionPicker);
		nRet_1 =  CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_TRAY_PICK] - st_motor[M_HS_B_RBT_Z].mn_allow, st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			TrayPlaceStep = 3200;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == BD_RETRY)
		{
			TrayPlaceStep = 3100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2137, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlaceStep = 3100;
		}
		break;

	case 3200:
		nRet_1 = Get_PickerUpDown(IO_ON, ActionPicker);
		if (nRet_1 == RET_GOOD)			// ���������� Up�� ���� ���.
		{
			l_until_wait_time[0] = GetCurrentTime();
			TrayPlaceStep = 3300;
		}
		else if (nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;		// ���� �߻��� �˶� ���� �÷��� ���� 
			CTL_Lib.Alarm_Error_Occurrence(2138, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 3300:
		Set_FingerOnOff(IO_ON, ActionPicker);
		TrayPlaceStep = 3400;
		break;

	case 3400:
		nRet_1 = Get_FingerOnOff(IO_ON, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			l_until_wait_time[0] = GetCurrentTime();
			TrayPlaceStep = 3500;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(2139, CTL_dWARNING, alarm.mc_code);
		}
		break;

	case 3500:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		if(l_until_wait_time[2] < 0) break;
		if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) break;
		Set_PickerUpDown(IO_OFF, ActionPicker);
		nRet_1 =  CTL_Lib.Single_Move(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			TrayPlaceStep = 3600;
			l_until_wait_time[0] = GetCurrentTime();
		}
		else if(nRet_1 == BD_RETRY)
		{
			TrayPlaceStep = 3500;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2140, st_work.mn_run_status, COMI.mc_alarmcode);
			TrayPlaceStep = 3500;
		}
		break;

	case 3510:
		Set_PickerUpDown(IO_OFF, ActionPicker);
		l_until_wait_time[0] = GetCurrentTime();
		TrayPlaceStep = 3600;
		break;

	case 3600:
		nRet_1 = Get_PickerUpDown(IO_OFF, ActionPicker);
		if (nRet_1 == RET_GOOD)			// ���������� Up�� ���� ���.
		{
			l_until_wait_time[0] = GetCurrentTime();
			TrayPlaceStep = 3700;
		}
		else if (nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;		// ���� �߻��� �˶� ���� �÷��� ���� 
			CTL_Lib.Alarm_Error_Occurrence(2141, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 3700:
		l_until_wait_time[1] = GetCurrentTime();
		l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];
		
		nRet_1 = Func.PickerModuleStatusCheck(IO_OFF, ActionPicker, mResult); // ��Ŀ ������ ���� ���� ���¸� üũ 
		
		if(nRet_1 == RET_ERROR)
		{
			if(l_until_wait_time[2] < PICKERSTATUS_CHECK_TIME) //���縦 üũ�� �� �ִ� ���� �ð��� �ش�  
			{
				if(l_until_wait_time[2] < 0)
				{
					l_until_wait_time[0] = GetCurrentTime(); //���� - ���̸� �ð� �ʱ�ȭ�Ͽ� �ٽ� üũ�Ѵ� 
					break;
				}
				CTL_Lib.Alarm_Error_Occurrence(2142, CTL_dWARNING, alarm.mc_code);
			}
			TrayPlaceStep = 3510;
		}
		else
		{
			TrayPlaceStep = 4000;
		}
		break;

	case 4000:
		nFuncRet = RET_GOOD;
		TrayPlaceStep = 0;
		break;
	}

	return nFuncRet;

}
