// CtlBd_Library.cpp: implementation of the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtlBd_Library.h"


#include "io.h"			//2011.0514  ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� �ʿ�
#include "FAS_HSSI.h"
#include "ComizoaPublic.h"
#include "FastechPublic_IO.h"
#include "CtlBd_Function.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCtlBd_Library::CCtlBd_Library()
{

}

CCtlBd_Library::~CCtlBd_Library()
{

}

CCtlBd_Library CTL_Lib;

//////////////////////////////////////////////////////////////////////////////
//version:0.1 (2011.1026) 
//�˶��߻� ó�����, ���� �� I/O ���� ���̺귯���� �ʱ�ȭ �� �����Ͽ� ��������ϴ� �Լ����� 
//
//���� �⺻�Լ� 
//1. #include "ComizoaPublic.h"
//2. #include "FastechPublic_IO.h"
///////////////////////////////////////////////////////////////////////////////

int CCtlBd_Library::Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10])
{

	int n_cur_num = -1;

	CTL_Lib.mn_jampos = n_jampos; //jam�� �߻��� ��ġ�� �����Ѵ� 
	CTL_Lib.mn_run_status = n_run_status; //����� �������¸� �����Ѵ� 
	COMI.mn_run_status = n_run_status;
	st_work.mn_run_status = n_run_status;
	alarm.mstr_code = c_alarmcode;
	alarm.stl_cur_alarm_time = GetCurrentTime();
	strcpy(mc_alarmcode, c_alarmcode);

	for(int n_cnt=0; n_cnt<2000; n_cnt++)
	{  
		if ((st_alarm.mstr_e_code[n_cnt]).Compare(alarm.mstr_code) == 0 )
		{
			n_cur_num = n_cnt; 
			break;
		}
	}

	if (n_cur_num > -1)
	{
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[%d][%s]:%s", n_jampos, alarm.mstr_code, st_alarm.mstr_e_msg[n_cur_num]);//20140212 ������  , alarm.mstr_code, st_alarm.mstr_e_msg[n_cur_num] �߰�
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
	}
	else
	{
		if(st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "JamPos:[%d][%s]:%s", n_jampos, alarm.mstr_code, st_alarm.mstr_e_msg[n_cur_num]);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
	}

	return BD_GOOD;
}


int CCtlBd_Library::Initialize_motor_board(int n_bd_type, CString s_filename)	//motor ���� �ʱ�ȭ�� ���
{
	int nRet = 0, i=0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if(n_bd_type == 0) //0:�Ľ��� ���带 ����ϴ� ����̸�, 1:Ŀ������ ���͸� ����ϴ� ��� 
	{		//���� ���庰 ����� �����Ѵ� 

		COMI.mn_totalmotboard_number = 3;  //���ͺ��尡 3�� �ִ� 
		COMI.mn_totalmotoraxis_number= 18; //������ �� ������ 18���̴�  
		COMI.mn_bd_axis_number[0] = 8;   //0�� ���ͺ���� 8��� �����̴�  
		COMI.mn_bd_axis_number[1] = 6;   //1�� ���ͺ���� 6��� �����̴� 
		COMI.mn_bd_axis_number[2] = 4;   //2�� ���ͺ���� 4��� �����̴� 
 	}
	else if(n_bd_type == 1) //1:Ŀ������ ���带 ����ϴ� ����̸�, 1:Ŀ������ ���͸� ����ϴ� ��� 
	{
		//1. ���� ���� �о���δ� 
		//2. ���� ���� ������ �����Ѵ� 
		nRet = COMI.Initialize_MotBoard(s_filename);			// MOTOR ���� �ʱ�ȭ ���� �˻��Ѵ�
		if (nRet == BD_ERROR)
		{
			sprintf(mc_normal_msg,"[MOTOR BOARD] �ʱ�ȭ ����.");
			LogFile_Write("c:\\AMT_LOG\\Motor\\", mc_normal_msg);
			return BD_ERROR;
		}		

		COMI.mn_totalmotboard_number = 3;  //���ͺ��尡 3�� �ִ� 
		COMI.mn_totalmotoraxis_number= 18; //������ �� ������ 18���̴�  
		COMI.mn_bd_axis_number[0] = 8;   //0�� ���ͺ���� 8��� �����̴�  
		COMI.mn_bd_axis_number[1] = 6;   //1�� ���ͺ���� 6��� �����̴� 
		COMI.mn_bd_axis_number[2] = 4;   //2�� ���ͺ���� 4��� �����̴� 

		if(COMI.mn_motorbd_init_end == BD_YES)
		{			
			for(i=0; i<COMI.mn_totalmotoraxis_number; i++)
			{//n_simul_mode => 0:�ùķ��̼� ��� ��Ȱ��, 1:�ùķ��̼� ��� Ȱ��

				if(COMI.mn_simulation_mode == 1) //���͸� �������� �����Ͽ� ���� ���� ����� ������ �ⱸ���� �̵��Ѱ�ó�� �����Ѵ� 
				{//Ȱ��
					COMI.Set_Simulation_Mode(i, 1); 
				}
				else
				{//��Ȱ�� 
					COMI.Set_Simulation_Mode(i, 0); 
				}
			}		
		}


		/*
		//��)		
		//COMI.Set_MotUnitDist(M_DCLDRBT_Y, 187.5);						 
		//COMI.Set_MotUnitDist(M_DCLDRBT_X, 500);  
		*/

		/* ��)
		//for (i = 0; i < MOT_MAXMOTOR; i++)
		//{
		//	COMI.Set_CMD_CTL_Mode(i, cmCTRL_SEMI_C);//cmCTRL_SEMI_C , ���ڴ�,(�ǵ�����) �������� ��ǥ��ǥ�� �����Ͽ� �̼��մϴ�
		//}*/

		/*��
	//	for (i = 0; i < COMI.mn_totalmotoraxis_number; i++) //2011.0126 Inposition�� default�� ��� �߰� 
	//	{
	// 		if(i == M_DCLDRBT_Y || i == M_DCLDRBT_X || i == M_LDTRANSFER_X || i == M_LDTRANSFER_Z || i == M_EMPTYTRANSFER_X || i == M_EMPTYTRANSFER_Z || i == M_ULDTRANSFER_X || i == M_ULDTRANSFER_Z)
	// 		{
	// 			COMI.Set_Motor_IO_Property(i, cmINP_EN, cmTRUE); //inposition ���� 
	// 		}
	// 		else
	// 		{
	// 			COMI.Set_Motor_IO_Property(i, cmINP_EN, cmFALSE);  //inposition ���� 
	// 		}
	//		COMI.Set_Motor_IO_Property(i, cmINP_EN, cmTRUE); //inposition ���� 
	//	}*/

		/* ��)
		//�ִ� ��� ���ļ��� �����ϴ� ������� �ּҴ� 
		//2011.0106 james �߰� 15.26 pps(min) ~ 1,000,000 pps(max), 6000rpm	
		////////////////////////�ּ� ���ļ� ���� ����////////////////////////////////////////////
		//MinPPS = MaxPPS / 65,535
		//���� �� MaxPPS �� 655,350 �̸� MinPPS = 655,350 / 65,535 = 10 (PPS)�� �˴ϴ�.
		for (i = 0; i < COMI.mn_totalmotoraxis_number; i++) //2011.0126 Inposition�� default�� ��� �߰� 
		{
			if(i == M_DCLDRBT_Z || i == M_DCULDRBT_Z || i == M_INSERTRBT_Z || i == M_REMOVERBT_Z || i == M_SORTBUFFRBT_Z || i == M_ULDRBT_Z)
			{
				cmmCfgSetSpeedRange(i, 1000000);//2011.0106 cmmCfgSetSpeedRange)	(long Axis, double MaxPPS);
			}
			else
			{
				cmmCfgSetSpeedRange(i, 650000);
			}
		}*/

		/* ��)
		// �ʱ� �ӵ��� 2000���� ��´�. �� ���� ������ 0
  		COMI.Set_MotInitSpeed(M_DCLDRBT_Z, 20000);
  		COMI.Set_MotInitSpeed(M_DCULDRBT_Z, 20000);
	    */


		/* ��)
		//////////////////////////////////////////////////////
		//SD Sensor ���� ���� �׽�Ʈ 
		//////////////////////////////////////////////////////
		COMI.Set_Motor_IO_Property(M_LDTRANSFER_Z, cmSD_EN, cmTRUE);    //cmSD_EN=14 //cmFALSE = 0 SD ��Ȱ��, cmTRUE = 1 SD Ȱ�� 	
		COMI.Set_Motor_IO_Property(M_LDTRANSFER_Z, cmSD_LOGIC, cmLOGIC_A); //cmSD_LOGIC=15, 0 (cmLOGIC_A) : A���� ���,1 (cmLOGIC_B) : B���� ���
		COMI.Set_Motor_IO_Property(M_LDTRANSFER_Z, cmSD_LATCH, cmFALSE);//16
		//0 (cmFALSE) : SD�� ON�Ǿ� �������̰ų� �ʱ�ӵ��� �������� �� SD��ȣ�� �ٽ� OFF���·� ����Ǹ� �۾��ӵ����� �ٽ� ���ӵ˴ϴ�
		//1 (cmTRUE) : SD�� ON���¿��� OFF���·� �ٲ� �۾��ӵ��� �������� �ʽ��ϴ�.																						
		COMI.Set_Motor_IO_Property(M_LDTRANSFER_Z, cmSD_MODE, cmTRUE);//17
		//0 : SD��ȣ�� ON�Ǹ� �ʱ�ӵ����� �����մϴ�(����(���)���� ����).
		//1 : SD��ȣ�� ON�Ǹ� ���� �� ����(���)�մϴ�.

		COMI.Set_Motor_IO_Property(M_EMPTYTRANSFER_Z, cmSD_EN, cmTRUE);    //cmSD_EN=14 //cmFALSE = 0 SD ��Ȱ��, cmTRUE = 1 SD Ȱ�� 	
		COMI.Set_Motor_IO_Property(M_EMPTYTRANSFER_Z, cmSD_LOGIC, cmLOGIC_A); //cmSD_LOGIC=15, 0 (cmLOGIC_A) : A���� ���,1 (cmLOGIC_B) : B���� ���
		COMI.Set_Motor_IO_Property(M_EMPTYTRANSFER_Z, cmSD_LATCH, cmFALSE);//16
		//0 (cmFALSE) : SD�� ON�Ǿ� �������̰ų� �ʱ�ӵ��� �������� �� SD��ȣ�� �ٽ� OFF���·� ����Ǹ� �۾��ӵ����� �ٽ� ���ӵ˴ϴ�
		//1 (cmTRUE) : SD�� ON���¿��� OFF���·� �ٲ� �۾��ӵ��� �������� �ʽ��ϴ�.																						
		COMI.Set_Motor_IO_Property(M_EMPTYTRANSFER_Z, cmSD_MODE, cmTRUE);//17
		//0 : SD��ȣ�� ON�Ǹ� �ʱ�ӵ����� �����մϴ�(����(���)���� ����).
		//1 : SD��ȣ�� ON�Ǹ� ���� �� ����(���)�մϴ�.

		COMI.Set_Motor_IO_Property(M_ULDTRANSFER_Z, cmSD_EN, cmTRUE);    //cmSD_EN=14 //cmFALSE = 0 SD ��Ȱ��, cmTRUE = 1 SD Ȱ�� 	
		COMI.Set_Motor_IO_Property(M_ULDTRANSFER_Z, cmSD_LOGIC, cmLOGIC_A); //cmSD_LOGIC=15, 0 (cmLOGIC_A) : A���� ���,1 (cmLOGIC_B) : B���� ���
		COMI.Set_Motor_IO_Property(M_ULDTRANSFER_Z, cmSD_LATCH, cmFALSE);//16
		//0 (cmFALSE) : SD�� ON�Ǿ� �������̰ų� �ʱ�ӵ��� �������� �� SD��ȣ�� �ٽ� OFF���·� ����Ǹ� �۾��ӵ����� �ٽ� ���ӵ˴ϴ�
		//1 (cmTRUE) : SD�� ON���¿��� OFF���·� �ٲ� �۾��ӵ��� �������� �ʽ��ϴ�.																						
		COMI.Set_Motor_IO_Property(M_ULDTRANSFER_Z, cmSD_MODE, cmTRUE);//17
		//0 : SD��ȣ�� ON�Ǹ� �ʱ�ӵ����� �����մϴ�(����(���)���� ����).
		//1 : SD��ȣ�� ON�Ǹ� ���� �� ����(���)�մϴ�.
		*/

		/*
		//ex)
		COMI.mn_homecheck_method[0] = 6;			// - Limit Sensor ����
		COMI.mn_homecheck_method[1] = 6;			// - Limit Sensor ����
		COMI.mn_homecheck_method[2] = 6;			// - Limit Sensor ����
		COMI.mn_homecheck_method[3] = 1;			// - Limit Sensor ����
		COMI.mn_homecheck_method[4] = 1;			// org Sensor ����
		*/

		/*
		//��) ��� ������ ����Ʈ ��ġ�� �����Ѵ� 
		st_motor[M_DCLDRBT_Y].d_limit_position[0] = -1;
		st_motor[M_DCLDRBT_Y].d_limit_position[1] = 878; //805;

		st_motor[M_DCLDRBT_X].d_limit_position[0] = -1;
		st_motor[M_DCLDRBT_X].d_limit_position[1] = 300; //273;

		st_motor[M_DCLDRBT_Z].d_limit_position[0] = -1;
		st_motor[M_DCLDRBT_Z].d_limit_position[1] = 78;

		st_motor[M_DCLDRBT_P_X].d_limit_position[0] = -1;
		st_motor[M_DCLDRBT_P_X].d_limit_position[1] = 66;//65;

		st_motor[M_DCLDRBT_P_Y].d_limit_position[0] = -1;
		st_motor[M_DCLDRBT_P_Y].d_limit_position[1] = 35;  

		st_motor[M_DCULDRBT_Y].d_limit_position[0] = -1;
		st_motor[M_DCULDRBT_Y].d_limit_position[1] = 590;		 

		for (i = 0; i < COMI.mn_totalmotoraxis_number; i++) //2011.0126 Inposition�� default�� ��� �߰� 
		{//Ȩ üũ�� �� �۾��� ���� ������ �Ǵ� ��ŵ�Ѵ� 
			cmmCfgSetSoftLimit(i, TRUE, st_motor[i].d_limit_position[0], st_motor[i].d_limit_position[1]);
		}*/	 

	}

	return  BD_GOOD;
}

//�Ʒ� ������ I/O ���� �ʱ�ȭ�� �ʿ��� ������ ���� �������̴� 
//��� �ʿ��� ������ �����Ͽ� ����ϸ� �ȴ�.
int CCtlBd_Library::Initialize_io_Board(int n_bd_type)
{
	//if(n_bd_type == 0) //�Ľ��� HSSI I/O board 
	//{
	//}
	int i=0,j=0;
	INT HSSI_speed = 0;
	int Ret = 0;
	INT pMasterNo;
	

	/////////////////�ʱ⿡ �ʱ�ȭ�� //////////////////////////////////////////////////////////////////////
	Ret = FAS_IO.Set_IO_BoardOpen(&pMasterNo, START_NOTHING); //START_LOADDATA); //START_NOTHING); //START_CLEAR);
	
	if (Ret == FALSE)
	{
		return BD_ERROR;
	}

	/* ��) �Ʒ� ������ ��� ������ Ʋ���� �����Ͽ� ����ϸ� �ȴ� 
	Ret = FAS_IO.Set_IO_PortAutoEnable(0, 0);	
	if (Ret != BD_GOOD)
	{
		sprintf(mc_normal_msg,"0 Master, 0 Port, not enable");
	
		FAS_IO.mn_io_board_initial_flag = BD_NO;
		return BD_ERROR;
	}

	Ret = FAS_IO.Set_IO_PortAutoEnable(0, 1);
	if (Ret != BD_GOOD)
	{
		sprintf(mc_normal_msg,"0 Master, 1 Port, not enable");
	
		FAS_IO.mn_io_board_initial_flag = BD_NO;
		return BD_ERROR;
	}

	Ret = FAS_IO.Set_IO_PortAutoEnable(0, 2);
	
	if (Ret != CTLBD_RET_GOOD)
	{
		sprintf(mc_normal_msg,"0 Master, 2 Port, not enable");
		
		FAS_IO.mn_io_board_initial_flag = BD_NO;
		return BD_ERROR;
	}


	for (i = 0; i < 8; i++)
	{
		Ret = FAS_IO.Set_IO_SlaveEnable(0, 0, i, TRUE);
		
		if (Ret != CTLBD_RET_GOOD)
		{
			sprintf(mc_normal_msg,"Slave #%d, not enable", i);
				
			i = -1;
			
			if (j < 3)
			{
				j++;
			}
			else
			{
				FAS_IO.mn_io_board_initial_flag = BD_NO;
				return BD_ERROR;
			}
			continue;
		}
		
		sprintf(mc_normal_msg,"Slave #%d, enable success", i);

		Sleep(100);
	} 

	for (i = 0; i < 4; i++)
	{
		Ret = FAS_IO.Set_IO_HSSISpeed(0, i, PORT_SPEED_10);
		Ret = FAS_IO.Get_IO_HSSISpeed(0, i, &HSSI_speed);

		if (i <= 3)
		{
			Ret = FAS_IO.Set_IO_HSSISpeed(1, i, PORT_SPEED_10);
			Ret = FAS_IO.Get_IO_HSSISpeed(1, i, &HSSI_speed);
		}
	}

	FAS_IO.Set_IO_DefineWord(0, 0, 0, 0x0003); // 0�� Master, 0�� ��Ʈ, 0�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 1, 0x0000); // 0�� Master, 0�� ��Ʈ, 1�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 2, 0x0000); // 0�� Master, 0�� ��Ʈ, 2�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 3, 0x000f); // 0�� Master, 0�� ��Ʈ, 3�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 4, 0x000f); // 0�� Master, 0�� ��Ʈ, 4�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 5, 0x000f); // 0�� Master, 0�� ��Ʈ, 5�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 6, 0x000f); // 0�� Master, 0�� ��Ʈ, 6�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 0, 7, 0x006f); // 0�� Master, 0�� ��Ʈ, 7�� �����̺� , 16bit ����

	FAS_IO.Set_IO_DefineWord(0, 1, 0, 0xffff); // 0�� Master, 1�� ��Ʈ, 0�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 1, 1, 0xffff); // 0�� Master, 1�� ��Ʈ, 1�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 1, 2, 0x0000); // 0�� Master, 1�� ��Ʈ, 2�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 1, 3, 0xffff); // 0�� Master, 1�� ��Ʈ, 3�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 1, 4, 0xffff); // 0�� Master, 1�� ��Ʈ, 4�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 1, 5, 0xffff); // 0�� Master, 1�� ��Ʈ, 5�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 1, 6, 0x0000); // 0�� Master, 1�� ��Ʈ, 6�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 1, 7, 0xffff); // 0�� Master, 1�� ��Ʈ, 7�� �����̺� , 16bit ����

	FAS_IO.Set_IO_DefineWord(0, 2, 0, 0xffff); // 0�� Master, 2�� ��Ʈ, 0�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 2, 1, 0x00ff); // 0�� Master, 2�� ��Ʈ, 1�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 2, 2, 0xffff); // 0�� Master, 2�� ��Ʈ, 2�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 2, 3, 0xffff); // 0�� Master, 2�� ��Ʈ, 3�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 2, 4, 0xffff); // 0�� Master, 2�� ��Ʈ, 4�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 2, 5, 0xffff); // 0�� Master, 2�� ��Ʈ, 5�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 2, 6, 0x00ff); // 0�� Master, 2�� ��Ʈ, 6�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 2, 7, 0xffff); // 0�� Master, 2�� ��Ʈ, 7�� �����̺� , 16bit ����

	FAS_IO.Set_IO_DefineWord(0, 3, 0, 0xffff); // 0�� Master, 3�� ��Ʈ, 0�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 3, 1, 0xffff); // 0�� Master, 3�� ��Ʈ, 1�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 3, 2, 0xffff); // 0�� Master, 3�� ��Ʈ, 2�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 3, 3, 0x00ff); // 0�� Master, 3�� ��Ʈ, 3�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 3, 4, 0xffff); // 0�� Master, 3�� ��Ʈ, 4�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 3, 5, 0xffff); // 0�� Master, 3�� ��Ʈ, 5�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 3, 6, 0x0000); // 0�� Master, 3�� ��Ʈ, 6�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(0, 3, 7, 0xffff); // 0�� Master, 3�� ��Ʈ, 7�� �����̺� , 16bit ����

	FAS_IO.Set_IO_DefineWord(1, 0, 0, 0x003f); // 1�� Master, 0�� ��Ʈ, 0�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 0, 1, 0x00ff); // 1�� Master, 0�� ��Ʈ, 1�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 0, 2, 0x001f); // 1�� Master, 0�� ��Ʈ, 2�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 0, 3, 0x001f); // 1�� Master, 0�� ��Ʈ, 3�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 0, 4, 0x001f); // 1�� Master, 0�� ��Ʈ, 4�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 0, 5, 0x003f); // 1�� Master, 0�� ��Ʈ, 5�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 0, 6, 0x00ff); // 1�� Master, 0�� ��Ʈ, 6�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 0, 7, 0x00ff); // 1�� Master, 0�� ��Ʈ, 7�� �����̺� , 16bit ����
	
	FAS_IO.Set_IO_DefineWord(1, 1, 0, 0x001f); // 1�� Master, 1�� ��Ʈ, 0�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 1, 1, 0x001f); // 1�� Master, 1�� ��Ʈ, 1�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 1, 2, 0x001f); // 1�� Master, 1�� ��Ʈ, 2�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 1, 3, 0x001f); // 1�� Master, 1�� ��Ʈ, 3�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 1, 4, 0x000f); // 1�� Master, 1�� ��Ʈ, 4�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 1, 5, 0x001f); // 1�� Master, 1�� ��Ʈ, 5�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 1, 6, 0x000f); // 1�� Master, 1�� ��Ʈ, 6�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 1, 7, 0x00ff); // 1�� Master, 1�� ��Ʈ, 7�� �����̺� , 16bit ����
	
	FAS_IO.Set_IO_DefineWord(1, 2, 0, 0x00ff); // 1�� Master, 2�� ��Ʈ, 0�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 2, 1, 0x00ff); // 1�� Master, 2�� ��Ʈ, 1�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 2, 2, 0x00ff); // 1�� Master, 2�� ��Ʈ, 2�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 2, 3, 0x0000); // 1�� Master, 2�� ��Ʈ, 3�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 2, 4, 0x0000); // 1�� Master, 2�� ��Ʈ, 4�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 2, 5, 0x0000); // 1�� Master, 2�� ��Ʈ, 5�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 2, 6, 0x0000); // 1�� Master, 2�� ��Ʈ, 6�� �����̺� , 16bit ����
	FAS_IO.Set_IO_DefineWord(1, 2, 7, 0x0000); // 1�� Master, 2�� ��Ʈ, 7�� �����̺� , 16bit ����
	*/

	FAS_IO.mn_io_board_initial_flag = BD_YES;
	
	return BD_GOOD;
}


int CCtlBd_Library::Motor_SafetyCheck(int n_mode, int n_axis, double d_targetpos) 	//���� ���� ���� ���ý� ��� 
{
	//���� �����Ͽ��� �����Ͽ� ��� �����Ѵ� 

	// ��) 
	// n_Mode = 0:Home Check, 1:Start, 2:Check, 3:Jog, 4:Length Change
	char cJamcode[10] = {NULL};
	int nRet = 0, nRet1 = 0, nRet2 = 0, nRet3 = 0;
	double d_CurPos[20] = {0,};
	double d_Pos[4] = {0,};
	double d_GapCheck = 0;
	int i = 0;

//2011.0201 �ּ�ó�� 	int n_SetTime = 50; //091127 20->50���� ���� 100; //500ms ���� ��� ����Ǹ� �׶� ��μ� I/O ���� return 

	// **************************************************************************
	// ���� �˶� ���� �˻��Ѵ�                                                   
	// -> �˶� �߻��� ��� �˶� �����Ѵ�                                         
	// **************************************************************************
	//software limit �� ���� üũ ���� 

	if (Func.DoorOpenCheckSpot() == CTLBD_RET_ERROR)	
	{
		st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm ȭ���� ����ش�. 2K4/11/21/ViboX

		return BD_ERROR;
	}
	
	if(d_targetpos < COMI.md_limit_position[n_axis][0]) //
	{
		sprintf(COMI.mc_alarmcode, "%02d0004", n_axis);
		Alarm_Error_Occurrence(4001, CTL_dWARNING, COMI.mc_alarmcode);
		
		if (st_handler.cwnd_list != NULL)
		{  // ����Ʈ �� ȭ�� ����
			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][0], d_targetpos);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		return BD_ERROR;
	}	
	if(d_targetpos > COMI.md_limit_position[n_axis][1]) //
	{
		sprintf(COMI.mc_alarmcode, "%02d0004", n_axis);
		Alarm_Error_Occurrence(4002, CTL_dWARNING, COMI.mc_alarmcode);
		
		if (st_handler.cwnd_list != NULL)
		{  // ����Ʈ �� ȭ�� ����
			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][1], d_targetpos);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û
		}
		return BD_ERROR;
	}

	// **************************************************************************
	// ���� �Ŀ� ���� �˻��Ѵ�                                                   
	// -> ���� POWER OFF �� POWER ON ���·� �����                               
	// **************************************************************************
	if (COMI.Get_MotPower(n_axis) == CTL_ERROR )          // ���� POWER ���� �˻� �Լ� 
	{
		if (COMI.Set_MotPower(n_axis, CTL_ON) == CTLBD_RET_GOOD)       // ���� POWER ON ���� �Լ� 
		{
			if (st_handler.cwnd_list != NULL)
			{  // ����Ʈ �� ȭ�� ����
				sprintf(st_msg.c_normal_msg, "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_axis, st_motor[n_axis].n_retry_cnt);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
			}

			if (COMI.Get_MotPower(n_axis) == CTL_ERROR)  // ���� POWER ���� �˻� �Լ� 
			{
				if (st_motor[n_axis].n_retry_cnt > (MOT_RTY_CNT))
				{
					sprintf(COMI.mc_alarmcode, "%02d0005", n_axis);
					Alarm_Error_Occurrence(4003, CTL_dWARNING, COMI.mc_alarmcode);	
					return BD_ERROR;
				}
				else
				{
					COMI.mn_retry_cnt[n_axis]++ ;
					return BD_RETRY;
				}
			}
		}
		else
		{
			if (st_motor[n_axis].n_retry_cnt > MOT_RTY_CNT)
			{
				sprintf(COMI.mc_alarmcode, "%02d0005", n_axis);
				Alarm_Error_Occurrence(4004, CTL_dWARNING, COMI.mc_alarmcode);		
				return BD_ERROR;
			}
			else
			{
				COMI.mn_retry_cnt[n_axis]++ ;
				return BD_RETRY;
			}
		}
	}

	//���� �˶� �߻��� ó�� ���� 
	if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR)      // ���� ALARM ���� �˻� �Լ� 
	{
		if (COMI.Set_MotAlarmClear(n_axis) == CTL_GOOD)       // ���� ALARM CLEAR �Լ� 
		{
			//091119 james			Sleep(1000);  // ���� �ð� �Ŀ� ���� Ȯ���ϱ� ���� SLEEP ����Ѵ� 
			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			{
//				st_msg.mstr_normal_msg.Format("[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis]);
				sprintf(st_msg.c_normal_msg, "[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis]);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR )  // ���� ALARM ���� �˻� �Լ� 
			{
				if (COMI.mn_retry_cnt[n_axis] > MOT_RTY_CNT)
				{			
					sprintf(COMI.mc_alarmcode, "%02d0002",  n_axis); alarm.mstr_code = _T(COMI.mc_alarmcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
					sprintf(alarm.mc_code, "%02d0002", n_axis);
					CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Get Alarm Check Error", n_axis, COMI.mn_retry_cnt[n_axis]);				
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, COMI.mc_alarmcode, CtlBdFunc.ms_ErrMsg);
					
					//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // �˶� ���� �õ� Ƚ�� 
					return CTL_ERROR;
				}
				else
				{
					COMI.mn_retry_cnt[n_axis]++ ;
					return CTL_RETRY;
				}
			}
		}
		else
		{
			if (COMI.mn_retry_cnt[n_axis] > MOT_RTY_CNT)
			{
				sprintf(COMI.mc_alarmcode, "%02d0002",  n_axis); alarm.mstr_code = _T(COMI.mc_alarmcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
				sprintf(alarm.mc_code, "%02d0002", n_axis);
				CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Set Alarm Return Error", n_axis, COMI.mn_retry_cnt[n_axis]);				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, COMI.mc_alarmcode, CtlBdFunc.ms_ErrMsg);
				
				//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // �˶� ���� �õ� Ƚ�� 
				return CTL_ERROR;
			}
			else
			{
				COMI.mn_retry_cnt[n_axis]++ ;
				return CTL_RETRY;
			}
		}
	}
	// **************************************************************************

	// **************************************************************************
	// ������ ���� ��ġ�� üũ�Ѵ�                                                 
	//***************************************************************************
	switch(n_axis)
	{
	case M_M_RBT_Y:
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_M_RBT_Y);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_M_RBT_Z);
		if(d_CurPos[2] > st_motor[M_M_RBT_Z].md_pos[Z_SAFETY] + st_motor[M_M_RBT_Z].mn_allow)
		{
			sprintf(COMI.mc_alarmcode,"%02d0008",n_axis);
			//Alarm_Error_Occurrence(4005, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		nRet = Func.RobotPickerUpSensorCheck(1,LDMODULE_SITE);
		if(nRet == CTL_ERROR)
		{
//			if(st_handler.cwnd_list != NULL)
//			{
//				//st_other.str_abnormal_msg = _T("[Safety Error] Robot Picker is Down.");
//				sprintf(st_msg.c_abnormal_msg,"[Safety Error] Robot Picker is Down.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
			sprintf(COMI.mc_alarmcode,"260101");
			//Alarm_Error_Occurrence(4006, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		break;

	case M_HS_F_RBT_Y:
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_HS_F_RBT_Y);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_HS_F_RBT_Z);
		if(d_CurPos[2] > st_motor[M_HS_F_RBT_Z].md_pos[Z_SAFETY] + st_motor[M_HS_F_RBT_Z].mn_allow)
		{
			sprintf(COMI.mc_alarmcode,"%02d0008",n_axis);
			//Alarm_Error_Occurrence(4007, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		nRet = Func.RobotPickerUpSensorCheck(1,LEFTSINK_SITE);
		if(nRet == CTL_ERROR)
		{
//			if(st_handler.cwnd_list != NULL)
//			{
//				//st_other.str_abnormal_msg = _T("[Safety Error] Robot Picker is Down.");
//				sprintf(st_msg.c_abnormal_msg,"[Safety Error] Left hs Robot Picker is Down.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
			sprintf(COMI.mc_alarmcode,"410200");
			//Alarm_Error_Occurrence(4008, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		break;

	case M_HS_B_RBT_Y:
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_HS_B_RBT_Y);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);
		if(d_CurPos[2] > st_motor[M_HS_B_RBT_Z].md_pos[Z_SAFETY] + st_motor[M_HS_B_RBT_Z].mn_allow)
		{
			sprintf(COMI.mc_alarmcode,"%02d0008",n_axis);
//			Alarm_Error_Occurrence(4009, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		nRet = Func.RobotPickerUpSensorCheck(1,RIGHTSINK_SITE);
		if(nRet == CTL_ERROR)
		{
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"[Safety Error] Right hs Robot Picker is Down.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
			sprintf(COMI.mc_alarmcode,"420200");
//			Alarm_Error_Occurrence(4010, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		break;
		
	case M_RBT_SORTER_X:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_RBT_SORTER_X);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_RBT_SORTER_Y);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(d_CurPos[2] > st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			sprintf(COMI.mc_alarmcode,"%02d0008",M_RBT_SORTER_Z);
//			Alarm_Error_Occurrence(4011, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		if(d_targetpos >  st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY] + st_motor[M_RBT_SORTER_X].mn_allow)
		{
			if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_1_SITE)
			{
				if(d_CurPos[1] < st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER] - st_motor[M_RBT_SORTER_Y].mn_allow)			
				{//000009 0 00 "Stacker1 Motor Destination is Non Safety Pos."
					sprintf(COMI.mc_alarmcode,"%02d0009",n_axis);
					return BD_ERROR;
				}
			}
			else
			{
				//////////////////////////20121114
				if(d_CurPos[1] < st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER2 - 4] - st_motor[M_RBT_SORTER_Y].mn_allow)			
				{//000009 0 00 "Stacker1 Motor Destination is Non Safety Pos."
					sprintf(COMI.mc_alarmcode,"%02d0009",n_axis);
					return BD_ERROR;
				}			
				//////////////////////////
			}
		}
		break;

	case M_RBT_SORTER_Y:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_RBT_SORTER_X);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_RBT_SORTER_Y);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
		if(d_CurPos[0] > st_motor[M_RBT_SORTER_X].md_pos[X_SORT_SAFETY] + st_motor[M_RBT_SORTER_X].mn_allow)
		{
			if(st_sync.n_ldrbt_visionrbt_req[1] == VISION_1_SITE)
			{
				if(d_CurPos[1] < st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER] - st_motor[M_RBT_SORTER_Y].mn_allow)
				{
					sprintf(COMI.mc_alarmcode,"%02d0008",M_RBT_SORTER_X);
					return BD_ERROR;
				}
				if(d_targetpos < st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER] - st_motor[M_RBT_SORTER_Y].mn_allow)
				{
					sprintf(COMI.mc_alarmcode,"%02d0008",M_RBT_SORTER_X);
					return BD_ERROR;
				}
			}
			else
			{
				//////////////////////////20121114
				if(d_CurPos[1] < st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER2 - 4] - st_motor[M_RBT_SORTER_Y].mn_allow)
				{
					sprintf(COMI.mc_alarmcode,"%02d0008",M_RBT_SORTER_X);
					return BD_ERROR;
				}
				if(d_targetpos < st_motor[M_RBT_SORTER_Y].md_pos[Y_SORT_VISION_BUFFER2 - 4] - st_motor[M_RBT_SORTER_Y].mn_allow)
				{
					sprintf(COMI.mc_alarmcode,"%02d0008",M_RBT_SORTER_X);
					return BD_ERROR;
				}
				//////////////////////////
			}
		}
		if(d_CurPos[2] > st_motor[M_RBT_SORTER_Z].md_pos[Z_SORT_SAFETY_UP] + st_motor[M_RBT_SORTER_Z].mn_allow)
		{
			sprintf(COMI.mc_alarmcode,"%02d0008",n_axis);
//			Alarm_Error_Occurrence(4011, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		//2012,0930
//		nRet = Func.RobotPickerUpSensorCheck(1,SORTER_SITE);
//		if(nRet == CTL_ERROR)
//		{
//			sprintf(COMI.mc_alarmcode,"400201");
//			return BD_ERROR;
//		}
		break;
		
	case M_RBT_UNLOAD_X:
	case M_RBT_UNLOAD_Y:
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Y);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);
		if(d_CurPos[2] > st_motor[M_RBT_UNLOAD_Z].md_pos[Z_SAFETY] + st_motor[M_RBT_UNLOAD_Z].mn_allow)
		{
			sprintf(COMI.mc_alarmcode,"%02d0008",n_axis);
//			Alarm_Error_Occurrence(4013, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		nRet = Func.RobotPickerUpSensorCheck(1,UNLOADER_SITE);
		if(nRet == CTL_ERROR)
		{
			sprintf(COMI.mc_alarmcode,"400201");
			return BD_ERROR;
		}
		break;
	
	case M_CLIP_BUFFER:
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
		if(FAS_IO.get_in_bit(st_io.i_clip_buff_dn_chk, IO_ON) == IO_OFF || 
			FAS_IO.get_in_bit(st_io.i_clip_buff_up_chk, IO_OFF) == IO_ON)
		{//180801 0 18 "Clip buffer down üũ ������ �߻��߽��ϴ�."
			sprintf(COMI.mc_alarmcode,"180801");
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"%s", "Clip buffer down üũ ������ �߻��߽��ϴ�.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� ���� ��� ��û				
//			}
//			Alarm_Error_Occurrence(4015, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		if(d_CurPos[1] < st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_POS] + st_motor[M_CLIP_INSERT_Y].mn_allow)
		{
			if(d_CurPos[2] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_SAFETY] + st_motor[M_CLIP_INSERT_Z].mn_allow)
			{//270009 0 27 "Clip Inset Z �κ� ��ǥ��ġ�� ������ġ�� ���� ����."
				sprintf(COMI.mc_alarmcode,"270009");
//				Alarm_Error_Occurrence(4016, CTL_dWARNING, COMI.mc_alarmcode);
				return BD_GOOD;
			}
		}
		break;

		
	case M_CLIP_INSERT_Y:
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
		if(d_CurPos[2] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_SAFETY] + st_motor[M_CLIP_INSERT_Z].mn_allow)
		{
			sprintf(COMI.mc_alarmcode,"%02d0008",n_axis);
			Alarm_Error_Occurrence(4017, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		nRet = Func.RobotPickerUpSensorCheck(1,LDCLIP_SITE);
		if(nRet == CTL_ERROR)
		{
//			if(st_handler.cwnd_list != NULL)
//			{
//				//st_other.str_abnormal_msg = _T("[Safety Error] Robot Picker is Down.");
//				sprintf(st_msg.c_abnormal_msg,"[Safety Error] Clip Insert Robot Picker is Down.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//			}
			sprintf(COMI.mc_alarmcode,"260101"); 
//			Alarm_Error_Occurrence(4018, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		break;

	case M_CLIP_INSERT_Z:
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);

		if( d_targetpos == st_motor[M_CLIP_INSERT_Z].md_pos[ULD_CLIP_POS] )
		{
			if( ( ( d_CurPos[1] > (st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE1] - st_motor[M_CLIP_INSERT_Y].mn_allow ) ) &&
				( d_CurPos[1] < (st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE1] + st_motor[M_CLIP_INSERT_Y].mn_allow) ) ) ||
				( ( d_CurPos[1] > (st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE2] - st_motor[M_CLIP_INSERT_Y].mn_allow ) ) &&
				( d_CurPos[1] < (st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE2] + st_motor[M_CLIP_INSERT_Y].mn_allow) ) ) ||
				( ( d_CurPos[1] > (st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE3] - st_motor[M_CLIP_INSERT_Y].mn_allow ) ) &&
				( d_CurPos[1] < (st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE3] + st_motor[M_CLIP_INSERT_Y].mn_allow) ) ) ||
				( ( d_CurPos[1] > (st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE4] - st_motor[M_CLIP_INSERT_Y].mn_allow ) ) &&
				( d_CurPos[1] < (st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE4] + st_motor[M_CLIP_INSERT_Y].mn_allow) ) ) )
			{
			}
			else
			{
				sprintf(COMI.mc_alarmcode,"%02d0003",M_CLIP_INSERT_Y);
				Alarm_Error_Occurrence(4099, CTL_dWARNING, COMI.mc_alarmcode);
				return BD_ERROR;
			}
		}
		break;

	case M_M_CLAMP_RBT_GRIPPER:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
		if((d_targetpos > st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_CLAMP_POS] + st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow))
		{//230009 0 23 "Clamp rbt Gripper �κ� ��ǥ��ġ�� ������ġ�� ���� ����."
			sprintf(COMI.mc_alarmcode,"230009");
//			Alarm_Error_Occurrence(4019, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		break;

		//2016.0526
	case M_M_CLAMP_RBT_Y:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
		if(d_CurPos[0] > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_CLAMP_RBT_Z].mn_allow)
		{
			sprintf(COMI.mc_alarmcode,"%02d0008",n_axis);
			Alarm_Error_Occurrence(7917, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;		
		}
		break;

	case M_M_CLAMP_RBT_Z:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
		if(d_CurPos[0] > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_CLAMP_RBT_Z].mn_allow)
		{
			if(d_CurPos[1] > st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_READY_POS])
			{//250100 0 25 "Clamp rbt Z �κ��� ������ �� ����. Clamp gripper ������ġ ����."
				if(st_handler.cwnd_list != NULL)
				{
					//st_other.str_abnormal_msg = _T("[Safety Error] Robot Picker is Down.");
					sprintf(st_msg.c_abnormal_msg,"[Safety Error] Clamp gripper robot is safety error.");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				sprintf(COMI.mc_alarmcode,"250100");
				Alarm_Error_Occurrence(4020, CTL_dWARNING, COMI.mc_alarmcode);
				return BD_ERROR;
			}		
		}
		if(d_CurPos[1] > st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_READY_POS] + st_motor[M_M_CLAMP_RBT_GRIPPER].mn_allow)
		{
			if(d_targetpos < st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_GLIPPER_WORK_POS] - (st_motor[M_M_CLAMP_RBT_Z].mn_allow))
			{//230008 0 23 "Clamp rbt Gripper �κ� ������ġ�� ���� ����."
				sprintf(COMI.mc_alarmcode,"230008");
//				Alarm_Error_Occurrence(4021, CTL_dWARNING, COMI.mc_alarmcode);
				return BD_ERROR;
			}
		}			
		if((d_targetpos > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_GLIPPER_WORK_POS] + st_motor[M_M_CLAMP_RBT_Z].mn_allow) ||	d_targetpos > 55.4)
		{//250009 0 25 "Clamp rbt Z �κ� ��ǥ��ġ�� ������ġ�� ���� ����."			
			sprintf(COMI.mc_alarmcode,"250009");
//			Alarm_Error_Occurrence(4022, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		break;

	case M_WORK_BUFFER_1:
//		if(ChekSafetyWorkBuffer(d_targetpos, WORK_BUFFER_1) == BD_ERROR)
//		{
//			return BD_ERROR;
//		}
		break;
		
	case M_WORK_BUFFER_2:
//		if(ChekSafetyWorkBuffer(d_targetpos, WORK_BUFFER_2) == BD_ERROR)
//		{
//			return BD_ERROR;
//		}
		break;

	case M_WORK_BUFFER_3:
//		if(ChekSafetyWorkBuffer(d_targetpos, WORK_BUFFER_3) == BD_ERROR)
//		{
//			return BD_ERROR;
//		}
		break;
	
	case M_WORK_BUFFER_4:
//		if(ChekSafetyWorkBuffer(d_targetpos, WORK_BUFFER_4) == BD_ERROR)
//		{
//			return BD_ERROR;
//		}
		break;

	case M_LDM_STACKER_1:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_LDM_STACKER_1);
		if(d_targetpos <  st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DOWN] + st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_DN_OFFSET] * 2)
		{
			nRet1 = FAS_IO.get_in_bit(st_io.i_m_uld_slide_tray_chk, IO_OFF);
			if(nRet1 == IO_ON)
			{
				//000503 0 01 "Unload Module Slide�� Ʈ���̸� ������ �ּ���."
				sprintf(COMI.mc_alarmcode,"000503");
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{//000802 0 01 "Load Module Slide�� Open�Ǿ� �ֽ��ϴ�. Close �� �ּ���."
			sprintf(COMI.mc_alarmcode,"000802");
			return BD_ERROR;
		}
		break;

	case M_LDM_STACKER_2:
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_MOVE);
		if(d_CurPos[1] > st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS] + st_motor[M_LDM_STACKER_MOVE].mn_allow &&
			d_CurPos[1] < st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] - st_motor[M_LDM_STACKER_MOVE].mn_allow)
		{
			//020008 0 02 "Stacker Move Axis Motor is Non Safety Pos."
			sprintf(COMI.mc_alarmcode,"020008");
			//Alarm_Error_Occurrence(4068, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		break;

	case M_LDM_STACKER_MOVE:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_LDM_STACKER_1);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_LDM_STACKER_2);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_LDM_STACKER_MOVE);
		if(d_CurPos[2] > st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS] + (10*st_motor[M_LDM_STACKER_MOVE].mn_allow))
		{
			if(d_targetpos < st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS] + st_motor[M_LDM_STACKER_MOVE].mn_allow)
			{
				if(d_CurPos[0] < st_motor[M_LDM_STACKER_1].md_pos[P_STOCKER_UP] - st_motor[M_LDM_STACKER_1].mn_allow)
				{
					//000008 0 00 "Stacker1 Axis Motor is Non Safety Pos."
					//sprintf(COMI.mc_alarmcode,"000008");
					//Alarm_Error_Occurrence(4065, CTL_dWARNING, COMI.mc_alarmcode);
					//return BD_ERROR;
				}
				if(d_CurPos[1] > st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN] + (st_motor[M_LDM_STACKER_2].mn_allow * 5) &&
					d_CurPos[1] < st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_UP] - st_motor[M_LDM_STACKER_2].mn_allow)
				{
					//010008 0 01 "Stacker2 Axis Motor is Non Safety Pos."
					sprintf(COMI.mc_alarmcode,"010008");
					Alarm_Error_Occurrence(4066, CTL_dWARNING, COMI.mc_alarmcode);
					return BD_ERROR;
				}
			}
		}
		if(d_targetpos > st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] + (10*st_motor[M_LDM_STACKER_MOVE].mn_allow))
		{
			if(d_CurPos[1] > st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN] + st_motor[M_LDM_STACKER_2].mn_allow ||
				FAS_IO.get_in_bit(st_io.i_m_stacker1_up_chk, IO_OFF) == IO_ON)
			{
				//020009 0 02 "Stacker Move Motor Destination is Non Safety Pos."
				sprintf(COMI.mc_alarmcode,"020009");
				//Alarm_Error_Occurrence(4030, CTL_dWARNING, COMI.mc_alarmcode);
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_m_uld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{ //000804 0 01 "Unload Module Slide�� Open�Ǿ� �ֽ��ϴ�. Close �� �ּ���."
			sprintf(COMI.mc_alarmcode, "000804");
			return BD_ERROR;
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_dn_chk, IO_ON);
		nRet2 = FAS_IO.get_in_bit(st_io.i_m_stacker2_up_chk, IO_OFF);
		if(nRet1 == IO_OFF || nRet2 == IO_ON)
		{
			//000108 0 00 "stacker1 ���� ������ ���� �ʽ��ϴ�."
			 //000109 0 00 "stacker1 �ٿ��� ������ ���� �ʽ��ϴ�."
			COMI.Set_MotStop(0, M_LDM_STACKER_MOVE);
			sprintf(COMI.mc_alarmcode, "000109");
			return BD_ERROR;
		}
		break;

	case M_HS_F_STACKER_1:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_HS_F_STACKER_1);
		if(d_targetpos <  st_motor[M_HS_F_STACKER_1].md_pos[P_STOCKER_DOWN] + st_motor[M_HS_F_STACKER_1].md_pos[P_STOCKER_DN_OFFSET] * 2)
		{
			nRet1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_uld_slide_tray_chk, IO_OFF);
			if(nRet1 == IO_ON)
			{
				//030603 0 01 "Unload Front HS Slide�� Ʈ���̸� ������ �ּ���."
				sprintf(COMI.mc_alarmcode,"030603");
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_ld_slide_onoff_chk, IO_OFF);
		if(nRet1 == IO_OFF)
		{//030802 0 01 "Load HS FrontSlide�� Open�Ǿ� �ֽ��ϴ�. Close �� �ּ���."
			sprintf(COMI.mc_alarmcode, "030802");
			return BD_ERROR;
		}
		break;
		
	case M_HS_F_STACKER_2:
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_HS_F_STACKER_MOVE);
		if(d_CurPos[1] > st_motor[M_HS_F_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS] + st_motor[M_HS_F_STACKER_MOVE].mn_allow &&
			d_CurPos[1] < st_motor[M_HS_F_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] - st_motor[M_HS_F_STACKER_MOVE].mn_allow)
		{
			//040008 0 04 "Left Heat Sink#2 stacker Motor is Non Safety Pos."
			sprintf(COMI.mc_alarmcode,"040008");
			//Alarm_Error_Occurrence(4069, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		break;

	case M_HS_F_STACKER_MOVE:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_HS_F_STACKER_1);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_HS_F_STACKER_2);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_HS_F_STACKER_MOVE);
		if(d_CurPos[2] > st_motor[M_HS_F_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS] + (10*st_motor[M_LDM_STACKER_MOVE].mn_allow))
		{
			if(d_targetpos < st_motor[M_HS_F_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS] + st_motor[M_LDM_STACKER_MOVE].mn_allow)
			{
				if(d_CurPos[0] < st_motor[M_HS_F_STACKER_1].md_pos[P_STOCKER_UP] - st_motor[M_HS_F_STACKER_1].mn_allow)
				{
					//000008 0 00 "Stacker1 Axis Motor is Non Safety Pos."
					//sprintf(COMI.mc_alarmcode,"000008");
					//Alarm_Error_Occurrence(4065, CTL_dWARNING, COMI.mc_alarmcode);
					//return BD_ERROR;
				}
				if(d_CurPos[1] > st_motor[M_HS_F_STACKER_2].md_pos[P_STOCKER_DOWN] + (st_motor[M_HS_F_STACKER_2].mn_allow * 5) &&
					d_CurPos[1] < st_motor[M_HS_F_STACKER_2].md_pos[P_STOCKER_UP] - st_motor[M_HS_F_STACKER_2].mn_allow)
				{
					//050008 0 05 "Left Heat Sink Move stacker Motor is Non Safety Pos."
					sprintf(COMI.mc_alarmcode,"050008");
					//Alarm_Error_Occurrence(4080, CTL_dWARNING, COMI.mc_alarmcode);
					return BD_ERROR;
				}
			}
		}
		if(d_targetpos >  st_motor[M_HS_F_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] + (10*st_motor[M_HS_F_STACKER_MOVE].mn_allow))
		{
			if(d_CurPos[1] > st_motor[M_HS_F_STACKER_2].md_pos[P_STOCKER_DOWN] + st_motor[M_HS_F_STACKER_2].mn_allow ||
				FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker1_up_chk, IO_OFF) == IO_ON)
			{
				//040008 0 04 "Left Heat Sink#2 stacker Motor is Non Safety Pos."
				sprintf(COMI.mc_alarmcode,"040008");
				//Alarm_Error_Occurrence(4031, CTL_dWARNING, COMI.mc_alarmcode);
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_uld_slide_onoff_chk, IO_OFF);
		if(nRet1 == IO_OFF)
		{ //030804 0 01 "Unload HS FrontSlide�� Open�Ǿ� �ֽ��ϴ�. Close �� �ּ���."
			sprintf(COMI.mc_alarmcode, "030804");
			return BD_ERROR;
		}
		break;

	case M_HS_B_STACKER_1:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_1);
		if(d_targetpos <  st_motor[M_HS_B_STACKER_1].md_pos[P_STOCKER_DOWN] + st_motor[M_HS_B_STACKER_1].md_pos[P_STOCKER_DN_OFFSET] * 2)
		{
			nRet1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_uld_slide_tray_chk, IO_OFF);
			if(nRet1 == IO_ON)
			{
				//070603 0 01 "Unload HS Back Slide�� Ʈ���̸� ������ �ּ���."
				sprintf(COMI.mc_alarmcode,"070603");
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{//070802 0 07 "Load HS BackSlide�� Open�Ǿ� �ֽ��ϴ�. Close �� �ּ���."
			sprintf(COMI.mc_alarmcode, "070802");
			return BD_ERROR;
		}
		break;

	case M_HS_B_STACKER_2:
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_MOVE);
		if(d_CurPos[1] > st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS] + st_motor[M_HS_B_STACKER_MOVE].mn_allow &&
			d_CurPos[1] < st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] - st_motor[M_HS_B_STACKER_MOVE].mn_allow)
		{
			//080008 0 08 "Right Heat Sink Move stacker Motor is Non Safety Pos."
			sprintf(COMI.mc_alarmcode,"080008");
			return BD_ERROR;
		}
		break;

	case M_HS_B_STACKER_MOVE:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_1);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_2);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_HS_B_STACKER_MOVE);
		if(d_CurPos[2] > st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS] + (10*st_motor[M_LDM_STACKER_MOVE].mn_allow))
		{
			if(d_targetpos < st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS] + st_motor[M_LDM_STACKER_MOVE].mn_allow)
			{
				if(d_CurPos[0] < st_motor[M_HS_B_STACKER_1].md_pos[P_STOCKER_UP] - st_motor[M_HS_F_STACKER_1].mn_allow)
				{
					//000008 0 00 "Stacker1 Axis Motor is Non Safety Pos."
					//sprintf(COMI.mc_alarmcode,"000008");
					//Alarm_Error_Occurrence(4065, CTL_dWARNING, COMI.mc_alarmcode);
					//return BD_ERROR;
				}
				if(d_CurPos[1] > st_motor[M_HS_B_STACKER_2].md_pos[P_STOCKER_DOWN] + (st_motor[M_HS_B_STACKER_2].mn_allow * 5) &&
					d_CurPos[1] < st_motor[M_HS_B_STACKER_2].md_pos[P_STOCKER_UP] - st_motor[M_HS_B_STACKER_2].mn_allow)
				{
					//070008 0 07 "Right Heat Sink#2 stacker Motor is Non Safety Pos."
					sprintf(COMI.mc_alarmcode,"070008");
					//Alarm_Error_Occurrence(4080, CTL_dWARNING, COMI.mc_alarmcode);
					return BD_ERROR;
				}
			}
		}
		
		if(d_targetpos >  st_motor[M_HS_B_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] + (10*st_motor[M_HS_B_STACKER_MOVE].mn_allow))
		{
			if(d_CurPos[1] > st_motor[M_HS_B_STACKER_2].md_pos[P_STOCKER_DOWN] + st_motor[M_HS_B_STACKER_2].mn_allow ||
				FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker1_up_chk, IO_OFF) == IO_ON)
			{
				//050009 0 05 "Left Heat Sink Move stacker Motor Destination is Non Safety Pos."
				sprintf(COMI.mc_alarmcode,"050009");
				//CTL_Lib.Alarm_Error_Occurrenc(4032, CTL_dWARNING, COMI.mc_alarmcode);
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_uld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{//070804 0 07 "Unload HS BackSlide�� Open�Ǿ� �ֽ��ϴ�. Close �� �ּ���."
			if(COMI.mn_simulation_mode == 1) // 20130509
			{
				break;
			}
			sprintf(COMI.mc_alarmcode, "070804");
			st_work.mn_run_status = CTL_dWARNING;
			return BD_ERROR;
		}
		break;

	case M_ULDM_STACKER_1:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_1);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_MOVE);
		if(d_CurPos[1] > st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS] &&
			d_CurPos[1] < st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS])
		{
			//110008 0 11 "Unload Stacker Move Motor is Non Safety Pos."
			sprintf(COMI.mc_alarmcode,"110008");
			//CTL_Lib.Alarm_Error_Occurrenc(4070, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
//		if(d_targetpos <  st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN] - st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DN_OFFSET] * 2)
//		{
//			nRet1 = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_tray_chk, IO_OFF);
//			if(nRet1 == IO_ON)
//			{
//				//100603 0 10 "Unload Unload Module Slide�� Ʈ���̸� ������ �ּ���."
//				sprintf(COMI.mc_alarmcode,"100603");
//				st_work.mn_run_status = CTL_dWARNING;
//				//CTL_Lib.Alarm_Error_Occurrence(4063, st_work.mn_run_status, COMI.mc_alarmcode);
//				return BD_ERROR;
//			}
//		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_uld_ld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{//100802 0 10 "Unload Module Slide�� Open�Ǿ� �ֽ��ϴ�. Close �� �ּ���."
			sprintf(COMI.mc_alarmcode, "100802");
			return BD_ERROR;
		}
		if(d_targetpos > st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN] + st_motor[M_ULDM_STACKER_1].mn_allow)
		{//060008 0 06 "Right Heat Sink#l stacker Motor is Non Safety Pos."
			//st_io.i_uld_stacker_guide_unclamp_off_chk, st_io.i_uld_stacker_guide_clamp_on_chk
			if(FAS_IO.get_in_bit(st_io.i_uld_stacker_guide_unclamp_off_chk, IO_OFF) == IO_OFF ||
				FAS_IO.get_in_bit(st_io.i_uld_stacker_guide_clamp_on_chk, IO_ON) == IO_ON)
			{
				sprintf(COMI.mc_alarmcode, "100802");
				//st_work.mn_run_status = CTL_dWARNING;
				return BD_ERROR;
			}
		}
		break;

	case M_ULDM_STACKER_2://0826
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_2);
		if(d_targetpos <  st_motor[M_ULDM_STACKER_2].md_pos[P_STOCKER_DOWN] + st_motor[M_ULDM_STACKER_2].md_pos[P_STOCKER_UP] * 2)
		{
			nRet1 = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_tray_chk, IO_OFF);
			if(nRet1 == IO_ON)
			{
				//100603 0 10 "Unload Unload Module Slide�� Ʈ���̸� ������ �ּ���."
				sprintf(COMI.mc_alarmcode,"100603");
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{//100804 0 10 "Unload Unload Module Slide�� Open�Ǿ� �ֽ��ϴ�. Close �� �ּ���."
			sprintf(COMI.mc_alarmcode, "100804");
			return BD_ERROR;
		}
		break;


	case M_ULDM_STACKER_MOVE:
		d_CurPos[0] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_1);
		d_CurPos[1] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_2);
		d_CurPos[2] = COMI.Get_MotCurrentPos(M_ULDM_STACKER_MOVE);
		if(d_CurPos[2] > st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] - st_motor[M_ULDM_STACKER_MOVE].mn_allow)
		{
			if(d_targetpos > st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] + st_motor[M_LDM_STACKER_2].mn_allow)
			{
				if(d_CurPos[0] > st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN] + st_motor[M_ULDM_STACKER_1].mn_allow)
				{
					//000008 0 00 "Stacker1 Axis Motor is Non Safety Pos."
					sprintf(COMI.mc_alarmcode,"000008");
					//CTL_Lib.Alarm_Error_Occurrenc(4065, CTL_dWARNING, COMI.mc_alarmcode);
					return BD_ERROR;
				}
			}
		}
		if(d_CurPos[0] < st_motor[M_ULDM_STACKER_2].md_pos[P_STOCKER_UP] - st_motor[M_LDM_STACKER_2].mn_allow)
		{
			//000008 0 00 "Stacker1 Axis Motor is Non Safety Pos."
			//sprintf(COMI.mc_alarmcode,"000008");
			//Alarm_Error_Occurrence(4065, CTL_dWARNING, COMI.mc_alarmcode);
			//return BD_ERROR;
		}
		if(d_CurPos[1] > st_motor[M_LDM_STACKER_2].md_pos[P_STOCKER_DOWN] + (st_motor[M_LDM_STACKER_2].mn_allow * 5))
		{
			//0010008 0 01 "Stacker2 Axis Motor is Non Safety Pos."
			//sprintf(COMI.mc_alarmcode,"0010008");
			//Alarm_Error_Occurrence(4066, CTL_dWARNING, COMI.mc_alarmcode);
			//return BD_ERROR;
		}


		if(d_targetpos >  st_motor[M_ULDM_STACKER_MOVE].md_pos[P_MSTACKER_STFETY_POS] + st_motor[M_ULDM_STACKER_MOVE].mn_allow)
		{
			if(d_CurPos[0] > st_motor[M_ULDM_STACKER_1].md_pos[P_STOCKER_DOWN] + st_motor[M_ULDM_STACKER_1].mn_allow ||
				FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk, IO_OFF) == IO_ON)
			{
				sprintf(COMI.mc_alarmcode,"110009");
				//CTL_Lib.Alarm_Error_Occurrenc(4033, CTL_dWARNING, COMI.mc_alarmcode);
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{//100804 0 10 "Unload Unload Module Slide�� Open�Ǿ� �ֽ��ϴ�. Close �� �ּ���.""
			sprintf(COMI.mc_alarmcode, "100804");
			return BD_ERROR;
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk, IO_ON);
		nRet2 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk, IO_OFF);
		if(nRet1 == IO_OFF || nRet2 == IO_ON)
		{
			//090108 0 09 "Unload Stacker#1 ���� ������ ���� �ʽ��ϴ�."
			//090109 0 09 "Unload Stacker#1 �ٿ��� ������ ���� �ʽ��ϴ�."
			COMI.Set_MotStop(0, M_ULDM_STACKER_MOVE);
			sprintf(COMI.mc_alarmcode, "090109");
			return BD_ERROR;
		}
		break;

	case M_VISION1_Z:
	case M_VISION2_Z:		
	case M_VISION1_Y:		
	case M_VISION2_Y:
		if(n_mode == 3)//���׷� �̵� �� ���
		{
		}
		else
		{
			nRet = Func.CheckVisionPos(n_axis, d_targetpos);//2014,1209 //2014,1210
			if(nRet == CTLBD_RET_ERROR)
			{
				return BD_ERROR;
			}
			else if(nRet == CTLBD_RET_PROCEED)
			{
				return CTLBD_RET_PROCEED;
			}
		}
		break;
	}

	//2011.0201 COMI.mn_retry_cnt[n_axis] = 0;
	st_motor[n_axis].mn_retry_time_flag = CTL_NO;//2011.0201 �߰� 

	return BD_GOOD;
}

BOOL CCtlBd_Library::CreateFolder(LPCTSTR szPath)
{
	int nRet = 0;
	CString strTemp;
	CString strPath = szPath;
	
    do{
        nRet = strPath.Find('\\' , nRet + 1);
        if (nRet > 0 )
        {
            strTemp = strPath.Left(nRet+1);
			
            if (_access(strTemp.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(strTemp , NULL);
			}
        }
    }while (nRet != -1);
	
    return nRet;
}

int CCtlBd_Library::FileSizeCheck(char * cp_filename, int n_size, int n_check)
{
	CFileFind finder;
	long nLen=0, nflag=0;
	BOOL bContinue;
	
	if(bContinue = finder.FindFile(cp_filename))
	{	
		if (n_check == BD_YES)			// Size�� üũ�ϴ°��̸�...
		{
			finder.FindFile(cp_filename);
			finder.FindNextFile();
			finder.GetFileName();
			nLen = finder.GetLength();
			if(nLen > n_size)
			{
				nflag = 1;
			}
			finder.Close();	
		}
		else						// Size�� Ȯ���ϴ°��̸�...
		{
			finder.FindFile(cp_filename);
			finder.FindNextFile();
			finder.GetFileName();
			nLen = finder.GetLength();
			
			nflag = nLen;
			
			finder.Close();	
		}
	}
	
	return nflag;
}

int CCtlBd_Library::LogFile_Write(char * cp_dir_path, char * cp_msg)
{
//2011.0417	sing.Lock(); //2011.0417 

	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // ���� ��, ��, �� ���� ���������� ��ȯ�Ͽ� ������ ���� 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // ���� ��, ��, �� ���� ���� ���� 
	CString mstr_file_name, mstr_dir_path, str_msg;		// ���������� ������ ���� �̸� ���� ���� 
	CString mstr_create_file;	// �˶� ���� ������ ���Ͽ� ���� [����]+[���ϸ�]+[Ȯ����] ���� ���� 
	CString mstr_list_name, mstr_temp_data;  // �� �κк� �˶� �߻� Ƚ�� ���� ������ �α� ���� ���� ���� ���� 
	CString mstr_content;		// �� ���ο� ����� ���ڿ� ���� ���� ���� 
	COleDateTime time_cur;		// �˻��� �ð� ���� ���� ���� 
	CTime m_time_current;		// ������ ����� ���� �ð� ���� ���� ����
	char chr_buf[20]={0,};
	int mn_existence, nlength;			// ���� ���� ���� ���� �÷��� 
	char cfileName[256]={0,};			// �˻��� ���� ���� ���� �Լ� 

	char clabelfileName[256]={0,};			// 20120530 �˻��� ���� ���� ���� �Լ� 

	FILE  *fp ;					// ���Ͽ� ���� ������ ���� ���� 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	int Ret=0;

	nlength = strlen(cp_msg);
	if(nlength <= 0) //������ �޼����� ���� 
	{
		return BD_ERROR;
	}
	str_msg = cp_msg;
 
	// **************************************************************************
	// ���� �̸����� ����� ��¥ ������ ��´�                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // ���� �ð� ������ ��´�. 

	m_time_current = CTime::GetCurrentTime() ;  // ������ ������ ���� �ð� ���� ��´�. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	// ��¥ ������ ���������� ��ȯ�Ͽ� ������ �����Ѵ�                           
	// **************************************************************************
	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// ���� �ð� ���� ��´�                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();				// ���� �ð� ������ �����Ѵ�. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// ���� �ð� ������ �����Ѵ�.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// ���� �� ������ �����Ѵ�. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// ���� �� ������ �����Ѵ�. 
	str_display_time = m_time_current.Format("%c");	// ����Ʈ ���Ͽ� ����� Ÿ��Ʋ �ð� ���� ���� 
	// **************************************************************************


	mstr_dir_path = cp_dir_path;
	mstr_file_name = mstr_cur_year;
	mstr_file_name += mstr_cur_month; 
	mstr_file_name += mstr_cur_day; 
	mstr_create_file = mstr_dir_path + mstr_file_name;

	mstr_create_file += ".TXT";

	sprintf(cfileName, "%s", mstr_create_file);
	mn_existence = access(cfileName,0) ;	
	if (mn_existence == -1)
	{//�ش� ������ �������� �ʴ� �����̴� 
		CreateFolder(cp_dir_path); 
	}
	else 
	{//�ش� ������ �̹� �����Ѵ�.
		//���� 
	}
	
	Ret = FileSizeCheck(cfileName, 1048576, BD_YES); //size and rename	
	if(Ret == 1) //file size over
	{
		BackupName = mstr_create_file + mstr_cur_hour + mstr_cur_min +  mstr_cur_sec + ".bak" ;
		rename(mstr_create_file, BackupName);
	}

	if ((fp = fopen(mstr_create_file,"a+")) == NULL)  
	{
		//AfxMessageBox("File open failure!..");
		sprintf(mc_normal_msg,"LogFile_Write a+ Error"); 
		
 		return BD_ERROR;
	}
	// **************************************************************************
	
	mstr_content += str_display_time;
	mstr_content += " : " + str_msg;

	fprintf(fp,"%s\n",mstr_content) ;
	if (ferror(fp))  
	{
		sprintf(mc_normal_msg,"LogFile_Write ferror return error");

		clearerr(fp); //2011.0418 

		fclose(fp) ;  //2011.0417  ���� ����

//2011.0417		sing.Unlock(); //2011.0417 �߰� 
		return BD_ERROR;
	}
	// **************************************************************************
	fclose(fp);  // ������ �����Ѵ�.

//2011.0417	sing.Unlock(); //2011.0417 �߰� 
	return BD_GOOD;
}

int CCtlBd_Library::OnAlarm( CString strCode )
{
	char cJamCode[10];
	strcpy( cJamCode, (LPCSTR) strCode );

	return Alarm_Error_Occurrence( 4050, CTL_dWARNING, cJamCode );
}

int CCtlBd_Library::Single_Move(int n_MotNum, double d_MotPos, int n_SpeedRate)
{
	int nFuncRet = BD_PROCEED;
	int nRet;
	
	switch(mn_single_motmove_step[n_MotNum])
	{
	case 0:
		nRet = COMI.Check_MotPosRange(n_MotNum, d_MotPos, COMI.mn_allow_value[n_MotNum]);
		if (nRet == BD_GOOD) //�̹� �ش� ��ġ�� �� ������ �������� �ʰ� ���� �̵��� ������ 
		{
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_mot_retry_cnt[n_MotNum] = 0;
		mn_mot_max_retry_cnt = 3; //�ִ� 3ȸ ��Ʈ���� ������ �����Ѵ� 
		mn_single_motmove_step[n_MotNum] = 10;
		break;

	case 10:
		if (n_MotNum == M_RBT_UNLOAD_X || n_MotNum == M_RBT_SORTER_X)
		{
//			nRet = Run_Robot.OnSafetyPosCheckMove(d_MotPos);
//			
//			if (nRet == CTLBD_RET_GOOD)
//			{
				mn_single_motmove_step[n_MotNum] = 100;
//			}
		}
		else
		{
			mn_single_motmove_step[n_MotNum] = 100;
		}
		break;
		
	case 100:
		nRet = COMI.Start_SingleMove(n_MotNum, d_MotPos, n_SpeedRate);  // �ش� ��ġ�� �̵�
		if (nRet == BD_GOOD)
		{
			mn_single_motmove_step[n_MotNum] = 200;
		}
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = nRet;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		else if (nRet == BD_RETRY)  // ���� ��õ�
		{
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		break;
		
	case 200:
		nRet = COMI.Check_SingleMove(n_MotNum, d_MotPos);  // �̵� �Ϸ� Ȯ��
		if (nRet == BD_GOOD)  // ���� �Ϸ�
		{
			mn_single_motmove_step[n_MotNum] = 0;
			nFuncRet = nRet;
		}
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = nRet;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		else if (nRet == BD_RETRY)  // ���� ��õ�
		{
			// ���ѷ����� �������� ������ Ƚ���� �����ؼ� ����ϸ� ���� ����
			mn_mot_retry_cnt[n_MotNum]++;
			
			if(mn_mot_retry_cnt[n_MotNum] > mn_mot_max_retry_cnt) 
			{
				mn_single_motmove_step[n_MotNum] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_single_motmove_step[n_MotNum] = 100;
			}
		}
		break;
	}
	return nFuncRet;
}

int CCtlBd_Library::Linear_Move(int n_LinearMapIndex, long l_AxisCnt, long *lp_AxisNum, double *dp_PosList, double *dp_SpdRatio)
{
	int nFuncRet = BD_PROCEED;
	int nRet[4] = {0,}, nCnt =0, i=0;
	
	switch(mn_linear_motmove_step[n_LinearMapIndex])
	{
	case 0:
		nCnt = 0;
		st_linearmot[n_LinearMapIndex].l_AxisCnt = l_AxisCnt;
		for(i=0; i<l_AxisCnt; i++)
		{		
			st_linearmot[n_LinearMapIndex].lp_AxisNum[i] = lp_AxisNum[i]; 
			st_linearmot[n_LinearMapIndex].dp_PosList[i] = dp_PosList[i];

			COMI.ml_axiscnt[n_LinearMapIndex] = 2;
			COMI.mp_axisnum[n_LinearMapIndex][i] = lp_AxisNum[i];

			nRet[i] = COMI.Check_MotPosRange(lp_AxisNum[i], dp_PosList[i], COMI.mn_allow_value[lp_AxisNum[i]]);
			if(nRet[i] == BD_GOOD) nCnt++;
		}

		if(nCnt == l_AxisCnt)
		{//�̹� ��� ������ ���� ��ġ�� �̵��Ϸ��� ��ġ�� ������ �ȿ� ������ ������ �ʿ䰡 ������ ������.
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_linear_retry_cnt[n_LinearMapIndex] = 0;
		mn_linear_max_retry_cnt = 3; //�ִ� 3ȸ ��Ʈ���� ������ �����Ѵ� 
		mn_linear_motmove_step[n_LinearMapIndex] = 10; //10;
		break;

	case 10:
		if (n_LinearMapIndex == 0)
		{
//			nRet[0] = Run_Robot.OnSafetyPosCheckMove(dp_PosList[0]); //(dp_PosList[1]);
//
//			if (nRet[0] == CTLBD_RET_GOOD)
//			{
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
//			}
		}
		else
		{
			mn_linear_motmove_step[n_LinearMapIndex] = 100;
		}
		break;
		
	case 100:
		nRet[0] = COMI.Start_LinearMove(n_LinearMapIndex, st_linearmot[n_LinearMapIndex].dp_PosList, dp_SpdRatio);
		if (nRet[0] == BD_GOOD)
		{
			mn_linear_motmove_step[n_LinearMapIndex] = 200;
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = nRet[0];
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		else if (nRet[0] == BD_RETRY)  // ���� ��õ�
		{
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		break;
		
	case 200:
		nRet[0] = COMI.Check_LinearMove(n_LinearMapIndex, st_linearmot[n_LinearMapIndex].dp_PosList) ;
		if (nRet[0] == BD_GOOD)  // ���� �Ϸ�
		{
			mn_linear_motmove_step[n_LinearMapIndex] = 0;
			nFuncRet = nRet[0];
		}
		else if (nRet[0] == BD_ERROR || nRet[0] == BD_SAFETY)
		{
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = nRet[0];
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}
		else if (nRet[0] == BD_RETRY)  // ���� ��õ�
		{
			// ���ѷ����� �������� ������ Ƚ���� �����ؼ� ����ϸ� ���� ����
			mn_linear_retry_cnt[n_LinearMapIndex]++;
			
			if(mn_linear_retry_cnt[n_LinearMapIndex] > mn_linear_max_retry_cnt) 
			{
				mn_linear_motmove_step[n_LinearMapIndex] = 0;
				nFuncRet = BD_ERROR;
			}
			else 
			{				
				mn_linear_motmove_step[n_LinearMapIndex] = 100;
			}
		}	
		break;
	}
	return nFuncRet;
}

int CCtlBd_Library::ChekSafetyWorkBuffer(double d_targetpos, int nPos)
{
	int nFuncRet = BD_GOOD;
	double d_CurPos[20] = {0,};
	
	d_CurPos[0]  = COMI.Get_MotCurrentPos(M_M_RBT_Y);
	d_CurPos[1]  = COMI.Get_MotCurrentPos(M_M_RBT_Z);
	d_CurPos[2]  = COMI.Get_MotCurrentPos(M_HS_F_RBT_Y);
	d_CurPos[3]  = COMI.Get_MotCurrentPos(M_HS_F_RBT_Z);
	d_CurPos[4]  = COMI.Get_MotCurrentPos(M_HS_B_RBT_Y);
	d_CurPos[5]  = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);
	d_CurPos[6]  = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
	d_CurPos[7]  = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
	d_CurPos[8]  = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
	d_CurPos[9]  = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
	d_CurPos[10] = COMI.Get_MotCurrentPos(M_RBT_SORTER_Y);
	d_CurPos[11] = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
	d_CurPos[12] = COMI.Get_MotCurrentPos(M_WORK_BUFFER_1);
	
	if(d_targetpos > st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_SAFETY] - st_motor[M_WORK_BUFFER_1 + nPos].mn_allow)
	{
		if(d_CurPos[12] > st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_LDMODULE] - st_motor[M_WORK_BUFFER_1 + nPos].mn_allow &&
			d_CurPos[12] < st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_LDMODULE] + st_motor[M_WORK_BUFFER_1 + nPos].mn_allow)
		{
			if(d_CurPos[0] > st_motor[M_M_RBT_Y].md_pos[Y_LD_WORK_PLACE1 + nPos] - st_motor[M_M_RBT_Y].mn_allow &&
				d_CurPos[0] < st_motor[M_M_RBT_Y].md_pos[Y_LD_WORK_PLACE1 + nPos] + st_motor[M_M_RBT_Y].mn_allow)
			{
				if(d_CurPos[1] > st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_RBT_Z].mn_allow)
				{//130008 0 13 "Module RBT Z Motor is Non Safety Pos."
					sprintf(COMI.mc_alarmcode,"%02d0008", M_WORK_BUFFER_1 + nPos);
					return BD_ERROR;
					
				}
			}
		}
		if(d_CurPos[12] > st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_LDLEFTHS] - st_motor[M_WORK_BUFFER_1 + nPos].mn_allow &&
			d_CurPos[12] < st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_LDLEFTHS] + st_motor[M_WORK_BUFFER_1 + nPos].mn_allow)
		{
			if(d_CurPos[0] > st_motor[M_HS_F_RBT_Y].md_pos[Y_LD_WORK_PLACE1 + nPos] - st_motor[M_HS_F_RBT_Y].mn_allow &&
				d_CurPos[0] < st_motor[M_HS_F_RBT_Y].md_pos[Y_LD_WORK_PLACE1 + nPos] + st_motor[M_HS_F_RBT_Y].mn_allow)
			{
				if(d_CurPos[1] > st_motor[M_HS_F_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_HS_F_RBT_Z].mn_allow)
				{//150008 0 15 "HeatSink FWD RBT Z Motor is Non Safety Pos."
					sprintf(COMI.mc_alarmcode,"150008");
					return BD_ERROR;
					
				}
			}
		}
		if(d_CurPos[12] > st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_LDRIGHTTHS] - st_motor[M_WORK_BUFFER_1 + nPos].mn_allow &&
			d_CurPos[12] < st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_LDRIGHTTHS] + st_motor[M_WORK_BUFFER_1 + nPos].mn_allow)
		{
			if(d_CurPos[0] > st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_WORK_PLACE1 + nPos] - st_motor[M_HS_B_RBT_Y].mn_allow &&
				d_CurPos[0] < st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_WORK_PLACE1 + nPos] + st_motor[M_HS_B_RBT_Y].mn_allow)
			{
				if(d_CurPos[1] > st_motor[M_HS_B_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_HS_B_RBT_Z].mn_allow)
				{//170008 0 17 "HeatSink BWD RBT Z Motor is Non Safety Pos."
					sprintf(COMI.mc_alarmcode,"170008");
					return BD_ERROR;
					
				}
			}
		}
		if(d_CurPos[12] > st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_LDCLIP] - st_motor[M_WORK_BUFFER_1].mn_allow &&
			d_CurPos[12] < st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_LDCLIP] + st_motor[M_WORK_BUFFER_1].mn_allow)
		{
			if(d_CurPos[0] > st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE1 + nPos] - st_motor[M_M_CLAMP_RBT_Y].mn_allow &&
				d_CurPos[0] < st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE1 + nPos] + st_motor[M_M_CLAMP_RBT_Y].mn_allow)
			{
				if(d_CurPos[1] > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_CLAMP_RBT_Z].mn_allow)
				{//250008 0 25 "Clamp rbt Z �κ� ������ġ�� ���� ����."
					sprintf(COMI.mc_alarmcode,"250008");
					return BD_ERROR;
					
				}
			}
			if(d_CurPos[0] > st_motor[M_CLIP_INSERT_Y + nPos].md_pos[Y_CLIP_WORK_PLACE1] - st_motor[M_CLIP_INSERT_Y].mn_allow &&
				d_CurPos[0] < st_motor[M_CLIP_INSERT_Y + nPos].md_pos[Y_CLIP_WORK_PLACE1] + st_motor[M_CLIP_INSERT_Y].mn_allow)
			{
				if(d_CurPos[1] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Z].mn_allow)
				{//270008 0 27 "Clip Inset Z �κ� ������ġ�� ���� ����."
					sprintf(COMI.mc_alarmcode,"270008");
					return BD_ERROR;
					
				}
			}
		}
		if(d_CurPos[12] > st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_ULDMODULE] - st_motor[M_WORK_BUFFER_1].mn_allow &&
			d_CurPos[12] < st_motor[M_WORK_BUFFER_1 + nPos].md_pos[WORK_ULDMODULE] + st_motor[M_WORK_BUFFER_1].mn_allow)
		{
			if(d_CurPos[0] > st_motor[M_RBT_SORTER_Y].md_pos[X_SORT_WORK_PICK1 + nPos] - st_motor[M_RBT_SORTER_Y].mn_allow &&
				d_CurPos[0] < st_motor[M_RBT_SORTER_Y].md_pos[X_SORT_WORK_PICK1 + nPos] + st_motor[M_RBT_SORTER_Y].mn_allow)
			{
				if(d_CurPos[1] > st_motor[M_RBT_SORTER_Z].md_pos[M_RBT_SORTER_Z] + st_motor[M_RBT_SORTER_Z].mn_allow)
				{//310008 0 31 "RBT Sorter Z �κ� ������ġ�� ���� ����."
					sprintf(COMI.mc_alarmcode,"310008");
					return BD_ERROR;
					
				}
			}
		}
	}

	return nFuncRet;
}