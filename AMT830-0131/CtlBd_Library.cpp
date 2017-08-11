// CtlBd_Library.cpp: implementation of the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtlBd_Library.h"


#include "io.h"			//2011.0514  파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요
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
//알람발생 처리방법, 모터 및 I/O 관련 라이브러리를 초기화 및 응용하여 실제사용하는 함수정리 
//
//참고 기본함수 
//1. #include "ComizoaPublic.h"
//2. #include "FastechPublic_IO.h"
///////////////////////////////////////////////////////////////////////////////

int CCtlBd_Library::Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10])
{

	int n_cur_num = -1;

	CTL_Lib.mn_jampos = n_jampos; //jam이 발생한 위치를 저장한다 
	CTL_Lib.mn_run_status = n_run_status; //장비의 가동상태를 변경한다 
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
			sprintf(st_msg.c_abnormal_msg, "[%d][%s]:%s", n_jampos, alarm.mstr_code, st_alarm.mstr_e_msg[n_cur_num]);//20140212 유성준  , alarm.mstr_code, st_alarm.mstr_e_msg[n_cur_num] 추가
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


int CCtlBd_Library::Initialize_motor_board(int n_bd_type, CString s_filename)	//motor 보드 초기화시 사용
{
	int nRet = 0, i=0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if(n_bd_type == 0) //0:파스텍 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{		//모터 보드별 축수를 정의한다 

		COMI.mn_totalmotboard_number = 3;  //모터보드가 3개 있다 
		COMI.mn_totalmotoraxis_number= 18; //모터의 총 수량은 18개이다  
		COMI.mn_bd_axis_number[0] = 8;   //0번 모터보드는 8축용 보드이다  
		COMI.mn_bd_axis_number[1] = 6;   //1번 모터보드는 6축용 보드이다 
		COMI.mn_bd_axis_number[2] = 4;   //2번 모터보드는 4축용 보드이다 
 	}
	else if(n_bd_type == 1) //1:커미조아 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{
		//1. 보드 정보 읽어들인다 
		//2. 각종 셋팅 정보를 적용한다 
		nRet = COMI.Initialize_MotBoard(s_filename);			// MOTOR 보드 초기화 여부 검사한다
		if (nRet == BD_ERROR)
		{
			sprintf(mc_normal_msg,"[MOTOR BOARD] 초기화 에러.");
			LogFile_Write("c:\\AMT_LOG\\Motor\\", mc_normal_msg);
			return BD_ERROR;
		}		

		COMI.mn_totalmotboard_number = 3;  //모터보드가 3개 있다 
		COMI.mn_totalmotoraxis_number= 18; //모터의 총 수량은 18개이다  
		COMI.mn_bd_axis_number[0] = 8;   //0번 모터보드는 8축용 보드이다  
		COMI.mn_bd_axis_number[1] = 6;   //1번 모터보드는 6축용 보드이다 
		COMI.mn_bd_axis_number[2] = 4;   //2번 모터보드는 4축용 보드이다 

		if(COMI.mn_motorbd_init_end == BD_YES)
		{			
			for(i=0; i<COMI.mn_totalmotoraxis_number; i++)
			{//n_simul_mode => 0:시뮬레이션 모드 비활성, 1:시뮬레이션 모드 활성

				if(COMI.mn_simulation_mode == 1) //모터를 가상으로 제어하여 실제 모터 출력이 나가고 기구물이 이동한것처럼 동작한다 
				{//활성
					COMI.Set_Simulation_Mode(i, 1); 
				}
				else
				{//비활성 
					COMI.Set_Simulation_Mode(i, 0); 
				}
			}		
		}


		/*
		//예)		
		//COMI.Set_MotUnitDist(M_DCLDRBT_Y, 187.5);						 
		//COMI.Set_MotUnitDist(M_DCLDRBT_X, 500);  
		*/

		/* 예)
		//for (i = 0; i < MOT_MAXMOTOR; i++)
		//{
		//	COMI.Set_CMD_CTL_Mode(i, cmCTRL_SEMI_C);//cmCTRL_SEMI_C , 엔코더,(피드백기준) 기준으로 목표좌표를 설정하여 이송합니다
		//}*/

		/*예
	//	for (i = 0; i < COMI.mn_totalmotoraxis_number; i++) //2011.0126 Inposition를 default로 기능 추가 
	//	{
	// 		if(i == M_DCLDRBT_Y || i == M_DCLDRBT_X || i == M_LDTRANSFER_X || i == M_LDTRANSFER_Z || i == M_EMPTYTRANSFER_X || i == M_EMPTYTRANSFER_Z || i == M_ULDTRANSFER_X || i == M_ULDTRANSFER_Z)
	// 		{
	// 			COMI.Set_Motor_IO_Property(i, cmINP_EN, cmTRUE); //inposition 적용 
	// 		}
	// 		else
	// 		{
	// 			COMI.Set_Motor_IO_Property(i, cmINP_EN, cmFALSE);  //inposition 제외 
	// 		}
	//		COMI.Set_Motor_IO_Property(i, cmINP_EN, cmTRUE); //inposition 적용 
	//	}*/

		/* 예)
		//최대 출력 주파수를 설정하는 명령으로 최소는 
		//2011.0106 james 추가 15.26 pps(min) ~ 1,000,000 pps(max), 6000rpm	
		////////////////////////최소 주파수 설정 공식////////////////////////////////////////////
		//MinPPS = MaxPPS / 65,535
		//예를 들어서 MaxPPS 가 655,350 이면 MinPPS = 655,350 / 65,535 = 10 (PPS)이 됩니다.
		for (i = 0; i < COMI.mn_totalmotoraxis_number; i++) //2011.0126 Inposition를 default로 기능 추가 
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

		/* 예)
		// 초기 속도를 2000으로 잡는다. 셋 팅이 없으면 0
  		COMI.Set_MotInitSpeed(M_DCLDRBT_Z, 20000);
  		COMI.Set_MotInitSpeed(M_DCULDRBT_Z, 20000);
	    */


		/* 예)
		//////////////////////////////////////////////////////
		//SD Sensor 관련 변수 테스트 
		//////////////////////////////////////////////////////
		COMI.Set_Motor_IO_Property(M_LDTRANSFER_Z, cmSD_EN, cmTRUE);    //cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 	
		COMI.Set_Motor_IO_Property(M_LDTRANSFER_Z, cmSD_LOGIC, cmLOGIC_A); //cmSD_LOGIC=15, 0 (cmLOGIC_A) : A접점 방식,1 (cmLOGIC_B) : B접점 방식
		COMI.Set_Motor_IO_Property(M_LDTRANSFER_Z, cmSD_LATCH, cmFALSE);//16
		//0 (cmFALSE) : SD가 ON되어 감속중이거나 초기속도로 운전중일 때 SD신호가 다시 OFF상태로 변경되면 작업속도까지 다시 가속됩니다
		//1 (cmTRUE) : SD가 ON상태에서 OFF상태로 바뀌어도 작업속도로 가속하지 않습니다.																						
		COMI.Set_Motor_IO_Property(M_LDTRANSFER_Z, cmSD_MODE, cmTRUE);//17
		//0 : SD신호가 ON되면 초기속도까지 감속합니다(정지(停止)하지 않음).
		//1 : SD신호가 ON되면 감속 후 정지(停止)합니다.

		COMI.Set_Motor_IO_Property(M_EMPTYTRANSFER_Z, cmSD_EN, cmTRUE);    //cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 	
		COMI.Set_Motor_IO_Property(M_EMPTYTRANSFER_Z, cmSD_LOGIC, cmLOGIC_A); //cmSD_LOGIC=15, 0 (cmLOGIC_A) : A접점 방식,1 (cmLOGIC_B) : B접점 방식
		COMI.Set_Motor_IO_Property(M_EMPTYTRANSFER_Z, cmSD_LATCH, cmFALSE);//16
		//0 (cmFALSE) : SD가 ON되어 감속중이거나 초기속도로 운전중일 때 SD신호가 다시 OFF상태로 변경되면 작업속도까지 다시 가속됩니다
		//1 (cmTRUE) : SD가 ON상태에서 OFF상태로 바뀌어도 작업속도로 가속하지 않습니다.																						
		COMI.Set_Motor_IO_Property(M_EMPTYTRANSFER_Z, cmSD_MODE, cmTRUE);//17
		//0 : SD신호가 ON되면 초기속도까지 감속합니다(정지(停止)하지 않음).
		//1 : SD신호가 ON되면 감속 후 정지(停止)합니다.

		COMI.Set_Motor_IO_Property(M_ULDTRANSFER_Z, cmSD_EN, cmTRUE);    //cmSD_EN=14 //cmFALSE = 0 SD 비활성, cmTRUE = 1 SD 활성 	
		COMI.Set_Motor_IO_Property(M_ULDTRANSFER_Z, cmSD_LOGIC, cmLOGIC_A); //cmSD_LOGIC=15, 0 (cmLOGIC_A) : A접점 방식,1 (cmLOGIC_B) : B접점 방식
		COMI.Set_Motor_IO_Property(M_ULDTRANSFER_Z, cmSD_LATCH, cmFALSE);//16
		//0 (cmFALSE) : SD가 ON되어 감속중이거나 초기속도로 운전중일 때 SD신호가 다시 OFF상태로 변경되면 작업속도까지 다시 가속됩니다
		//1 (cmTRUE) : SD가 ON상태에서 OFF상태로 바뀌어도 작업속도로 가속하지 않습니다.																						
		COMI.Set_Motor_IO_Property(M_ULDTRANSFER_Z, cmSD_MODE, cmTRUE);//17
		//0 : SD신호가 ON되면 초기속도까지 감속합니다(정지(停止)하지 않음).
		//1 : SD신호가 ON되면 감속 후 정지(停止)합니다.
		*/

		/*
		//ex)
		COMI.mn_homecheck_method[0] = 6;			// - Limit Sensor 감지
		COMI.mn_homecheck_method[1] = 6;			// - Limit Sensor 감지
		COMI.mn_homecheck_method[2] = 6;			// - Limit Sensor 감지
		COMI.mn_homecheck_method[3] = 1;			// - Limit Sensor 감지
		COMI.mn_homecheck_method[4] = 1;			// org Sensor 감지
		*/

		/*
		//예) 모든 모터의 리미트 위치를 셋팅한다 
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

		for (i = 0; i < COMI.mn_totalmotoraxis_number; i++) //2011.0126 Inposition를 default로 기능 추가 
		{//홈 체크시 등 작업자 사용시 문제가 되니 스킵한다 
			cmmCfgSetSoftLimit(i, TRUE, st_motor[i].d_limit_position[0], st_motor[i].d_limit_position[1]);
		}*/	 

	}

	return  BD_GOOD;
}

//아래 내용은 I/O 보드 초기화시 필요한 내용을 예로 들은것이니 
//장비별 필요한 내용을 셋팅하여 사용하면 된다.
int CCtlBd_Library::Initialize_io_Board(int n_bd_type)
{
	//if(n_bd_type == 0) //파스텍 HSSI I/O board 
	//{
	//}
	int i=0,j=0;
	INT HSSI_speed = 0;
	int Ret = 0;
	INT pMasterNo;
	

	/////////////////초기에 초기화시 //////////////////////////////////////////////////////////////////////
	Ret = FAS_IO.Set_IO_BoardOpen(&pMasterNo, START_NOTHING); //START_LOADDATA); //START_NOTHING); //START_CLEAR);
	
	if (Ret == FALSE)
	{
		return BD_ERROR;
	}

	/* 예) 아래 내용은 장비별 내용이 틀리니 참고하여 사용하면 된다 
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

	FAS_IO.Set_IO_DefineWord(0, 0, 0, 0x0003); // 0번 Master, 0번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 1, 0x0000); // 0번 Master, 0번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 2, 0x0000); // 0번 Master, 0번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 3, 0x000f); // 0번 Master, 0번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 4, 0x000f); // 0번 Master, 0번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 5, 0x000f); // 0번 Master, 0번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 6, 0x000f); // 0번 Master, 0번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 7, 0x006f); // 0번 Master, 0번 포트, 7번 슬레이브 , 16bit 셋팅

	FAS_IO.Set_IO_DefineWord(0, 1, 0, 0xffff); // 0번 Master, 1번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 1, 0xffff); // 0번 Master, 1번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 2, 0x0000); // 0번 Master, 1번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 3, 0xffff); // 0번 Master, 1번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 4, 0xffff); // 0번 Master, 1번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 5, 0xffff); // 0번 Master, 1번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 6, 0x0000); // 0번 Master, 1번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 7, 0xffff); // 0번 Master, 1번 포트, 7번 슬레이브 , 16bit 셋팅

	FAS_IO.Set_IO_DefineWord(0, 2, 0, 0xffff); // 0번 Master, 2번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 1, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 2, 0xffff); // 0번 Master, 2번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 3, 0xffff); // 0번 Master, 2번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 4, 0xffff); // 0번 Master, 2번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 5, 0xffff); // 0번 Master, 2번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 6, 0x00ff); // 0번 Master, 2번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 7, 0xffff); // 0번 Master, 2번 포트, 7번 슬레이브 , 16bit 셋팅

	FAS_IO.Set_IO_DefineWord(0, 3, 0, 0xffff); // 0번 Master, 3번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 1, 0xffff); // 0번 Master, 3번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 2, 0xffff); // 0번 Master, 3번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 3, 0x00ff); // 0번 Master, 3번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 4, 0xffff); // 0번 Master, 3번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 5, 0xffff); // 0번 Master, 3번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 6, 0x0000); // 0번 Master, 3번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 7, 0xffff); // 0번 Master, 3번 포트, 7번 슬레이브 , 16bit 셋팅

	FAS_IO.Set_IO_DefineWord(1, 0, 0, 0x003f); // 1번 Master, 0번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 0, 1, 0x00ff); // 1번 Master, 0번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 0, 2, 0x001f); // 1번 Master, 0번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 0, 3, 0x001f); // 1번 Master, 0번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 0, 4, 0x001f); // 1번 Master, 0번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 0, 5, 0x003f); // 1번 Master, 0번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 0, 6, 0x00ff); // 1번 Master, 0번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 0, 7, 0x00ff); // 1번 Master, 0번 포트, 7번 슬레이브 , 16bit 셋팅
	
	FAS_IO.Set_IO_DefineWord(1, 1, 0, 0x001f); // 1번 Master, 1번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 1, 1, 0x001f); // 1번 Master, 1번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 1, 2, 0x001f); // 1번 Master, 1번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 1, 3, 0x001f); // 1번 Master, 1번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 1, 4, 0x000f); // 1번 Master, 1번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 1, 5, 0x001f); // 1번 Master, 1번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 1, 6, 0x000f); // 1번 Master, 1번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 1, 7, 0x00ff); // 1번 Master, 1번 포트, 7번 슬레이브 , 16bit 셋팅
	
	FAS_IO.Set_IO_DefineWord(1, 2, 0, 0x00ff); // 1번 Master, 2번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 2, 1, 0x00ff); // 1번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 2, 2, 0x00ff); // 1번 Master, 2번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 2, 3, 0x0000); // 1번 Master, 2번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 2, 4, 0x0000); // 1번 Master, 2번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 2, 5, 0x0000); // 1번 Master, 2번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 2, 6, 0x0000); // 1번 Master, 2번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(1, 2, 7, 0x0000); // 1번 Master, 2번 포트, 7번 슬레이브 , 16bit 셋팅
	*/

	FAS_IO.mn_io_board_initial_flag = BD_YES;
	
	return BD_GOOD;
}


int CCtlBd_Library::Motor_SafetyCheck(int n_mode, int n_axis, double d_targetpos) 	//모터 안전 정보 셋팅시 사용 
{
	//안전 관련하여는 참고하여 장비별 적용한다 

	// 예) 
	// n_Mode = 0:Home Check, 1:Start, 2:Check, 3:Jog, 4:Length Change
	char cJamcode[10] = {NULL};
	int nRet = 0, nRet1 = 0, nRet2 = 0, nRet3 = 0;
	double d_CurPos[20] = {0,};
	double d_Pos[4] = {0,};
	double d_GapCheck = 0;
	int i = 0;

//2011.0201 주석처리 	int n_SetTime = 50; //091127 20->50으로 변경 100; //500ms 동안 계속 감기되면 그때 비로소 I/O 상태 return 

	// **************************************************************************
	// 모터 알람 상태 검사한다                                                   
	// -> 알람 발생한 경우 알람 해제한다                                         
	// **************************************************************************
	//software limit 값 셋팅 체크 에러 

	if (Func.DoorOpenCheckSpot() == CTLBD_RET_ERROR)	
	{
		st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX

		return BD_ERROR;
	}
	
	if(d_targetpos < COMI.md_limit_position[n_axis][0]) //
	{
		sprintf(COMI.mc_alarmcode, "%02d0004", n_axis);
		Alarm_Error_Occurrence(4001, CTL_dWARNING, COMI.mc_alarmcode);
		
		if (st_handler.cwnd_list != NULL)
		{  // 리스트 바 화면 존재
			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][0], d_targetpos);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return BD_ERROR;
	}	
	if(d_targetpos > COMI.md_limit_position[n_axis][1]) //
	{
		sprintf(COMI.mc_alarmcode, "%02d0004", n_axis);
		Alarm_Error_Occurrence(4002, CTL_dWARNING, COMI.mc_alarmcode);
		
		if (st_handler.cwnd_list != NULL)
		{  // 리스트 바 화면 존재
			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_axis, COMI.md_limit_position[n_axis][1], d_targetpos);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return BD_ERROR;
	}

	// **************************************************************************
	// 모터 파워 상태 검사한다                                                   
	// -> 모터 POWER OFF 시 POWER ON 상태로 만든다                               
	// **************************************************************************
	if (COMI.Get_MotPower(n_axis) == CTL_ERROR )          // 모터 POWER 상태 검사 함수 
	{
		if (COMI.Set_MotPower(n_axis, CTL_ON) == CTLBD_RET_GOOD)       // 모터 POWER ON 설정 함수 
		{
			if (st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재
				sprintf(st_msg.c_normal_msg, "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_axis, st_motor[n_axis].n_retry_cnt);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
			}

			if (COMI.Get_MotPower(n_axis) == CTL_ERROR)  // 모터 POWER 상태 검사 함수 
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

	//모터 알람 발생시 처리 조건 
	if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR)      // 모터 ALARM 상태 검사 함수 
	{
		if (COMI.Set_MotAlarmClear(n_axis) == CTL_GOOD)       // 모터 ALARM CLEAR 함수 
		{
			//091119 james			Sleep(1000);  // 일정 시간 후에 상태 확인하기 위해 SLEEP 사용한다 
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_normal_msg.Format("[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis]);
				sprintf(st_msg.c_normal_msg, "[Alarm Reset] Axis=%d, rcnt=%d", n_axis, COMI.mn_retry_cnt[n_axis]);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			if (COMI.Get_MotAlarmStatus(n_axis) == CTL_ERROR )  // 모터 ALARM 상태 검사 함수 
			{
				if (COMI.mn_retry_cnt[n_axis] > MOT_RTY_CNT)
				{			
					sprintf(COMI.mc_alarmcode, "%02d0002",  n_axis); alarm.mstr_code = _T(COMI.mc_alarmcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
					sprintf(alarm.mc_code, "%02d0002", n_axis);
					CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Get Alarm Check Error", n_axis, COMI.mn_retry_cnt[n_axis]);				
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_axis, COMI.mc_alarmcode, CtlBdFunc.ms_ErrMsg);
					
					//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // 알람 해제 시도 횟수 
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
				
				//2011.0210 COMI.mn_retry_cnt[n_axis] = 0;  // 알람 해제 시도 횟수 
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
	// 모터의 안전 위치를 체크한다                                                 
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
		{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
			sprintf(COMI.mc_alarmcode,"180801");
//			if(st_handler.cwnd_list != NULL)
//			{
//				sprintf(st_msg.c_abnormal_msg,"%s", "Clip buffer down 체크 에러가 발생했습니다.");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청				
//			}
//			Alarm_Error_Occurrence(4015, CTL_dWARNING, COMI.mc_alarmcode);
			return BD_ERROR;
		}
		if(d_CurPos[1] < st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_POS] + st_motor[M_CLIP_INSERT_Y].mn_allow)
		{
			if(d_CurPos[2] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_SAFETY] + st_motor[M_CLIP_INSERT_Z].mn_allow)
			{//270009 0 27 "Clip Inset Z 로봇 목표위치가 안전위치에 있지 않음."
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
		{//230009 0 23 "Clamp rbt Gripper 로봇 목표위치가 안전위치에 있지 않음."
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
			{//250100 0 25 "Clamp rbt Z 로봇을 움직일 수 없음. Clamp gripper 안전위치 에러."
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
			{//230008 0 23 "Clamp rbt Gripper 로봇 안전위치에 있지 않음."
				sprintf(COMI.mc_alarmcode,"230008");
//				Alarm_Error_Occurrence(4021, CTL_dWARNING, COMI.mc_alarmcode);
				return BD_ERROR;
			}
		}			
		if((d_targetpos > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_GLIPPER_WORK_POS] + st_motor[M_M_CLAMP_RBT_Z].mn_allow) ||	d_targetpos > 55.4)
		{//250009 0 25 "Clamp rbt Z 로봇 목표위치가 안전위치에 있지 않음."			
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
				//000503 0 01 "Unload Module Slide에 트레이를 제거해 주세요."
				sprintf(COMI.mc_alarmcode,"000503");
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_m_ld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{//000802 0 01 "Load Module Slide가 Open되어 있습니다. Close 해 주세요."
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
		{ //000804 0 01 "Unload Module Slide가 Open되어 있습니다. Close 해 주세요."
			sprintf(COMI.mc_alarmcode, "000804");
			return BD_ERROR;
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_m_stacker2_dn_chk, IO_ON);
		nRet2 = FAS_IO.get_in_bit(st_io.i_m_stacker2_up_chk, IO_OFF);
		if(nRet1 == IO_OFF || nRet2 == IO_ON)
		{
			//000108 0 00 "stacker1 업이 동작을 하지 않습니다."
			 //000109 0 00 "stacker1 다운이 동작을 하지 않습니다."
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
				//030603 0 01 "Unload Front HS Slide에 트레이를 제거해 주세요."
				sprintf(COMI.mc_alarmcode,"030603");
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_hs_fwd_ld_slide_onoff_chk, IO_OFF);
		if(nRet1 == IO_OFF)
		{//030802 0 01 "Load HS FrontSlide가 Open되어 있습니다. Close 해 주세요."
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
		{ //030804 0 01 "Unload HS FrontSlide가 Open되어 있습니다. Close 해 주세요."
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
				//070603 0 01 "Unload HS Back Slide에 트레이를 제거해 주세요."
				sprintf(COMI.mc_alarmcode,"070603");
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{//070802 0 07 "Load HS BackSlide가 Open되어 있습니다. Close 해 주세요."
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
		{//070804 0 07 "Unload HS BackSlide가 Open되어 있습니다. Close 해 주세요."
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
//				//100603 0 10 "Unload Unload Module Slide에 트레이를 제거해 주세요."
//				sprintf(COMI.mc_alarmcode,"100603");
//				st_work.mn_run_status = CTL_dWARNING;
//				//CTL_Lib.Alarm_Error_Occurrence(4063, st_work.mn_run_status, COMI.mc_alarmcode);
//				return BD_ERROR;
//			}
//		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_uld_ld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{//100802 0 10 "Unload Module Slide가 Open되어 있습니다. Close 해 주세요."
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
				//100603 0 10 "Unload Unload Module Slide에 트레이를 제거해 주세요."
				sprintf(COMI.mc_alarmcode,"100603");
				return BD_ERROR;
			}
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_onoff_chk, IO_ON);
		if(nRet1 == IO_OFF)
		{//100804 0 10 "Unload Unload Module Slide가 Open되어 있습니다. Close 해 주세요."
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
		{//100804 0 10 "Unload Unload Module Slide가 Open되어 있습니다. Close 해 주세요.""
			sprintf(COMI.mc_alarmcode, "100804");
			return BD_ERROR;
		}
		nRet1 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_dn_chk, IO_ON);
		nRet2 = FAS_IO.get_in_bit(st_io.i_uld_stacker1_up_chk, IO_OFF);
		if(nRet1 == IO_OFF || nRet2 == IO_ON)
		{
			//090108 0 09 "Unload Stacker#1 업이 동작을 하지 않습니다."
			//090109 0 09 "Unload Stacker#1 다운이 동작을 하지 않습니다."
			COMI.Set_MotStop(0, M_ULDM_STACKER_MOVE);
			sprintf(COMI.mc_alarmcode, "090109");
			return BD_ERROR;
		}
		break;

	case M_VISION1_Z:
	case M_VISION2_Z:		
	case M_VISION1_Y:		
	case M_VISION2_Y:
		if(n_mode == 3)//조그로 이동 할 경우
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
	st_motor[n_axis].mn_retry_time_flag = CTL_NO;//2011.0201 추가 

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
		if (n_check == BD_YES)			// Size를 체크하는것이면...
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
		else						// Size를 확인하는것이면...
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

	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // 현재 년, 월, 일 정보 문자형으로 변환하여 저장할 변수 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // 현재 년, 월, 일 정보 저장 변수 
	CString mstr_file_name, mstr_dir_path, str_msg;		// 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file;	// 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name, mstr_temp_data;  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content;		// 한 라인에 출력할 문자열 정보 저장 변수 
	COleDateTime time_cur;		// 검사할 시간 정보 저장 변수 
	CTime m_time_current;		// 간략한 헝식의 현재 시간 정보 저장 변수
	char chr_buf[20]={0,};
	int mn_existence, nlength;			// 파일 존재 유무 설정 플래그 
	char cfileName[256]={0,};			// 검색할 파일 정보 설정 함수 

	char clabelfileName[256]={0,};			// 20120530 검색할 파일 정보 설정 함수 

	FILE  *fp ;					// 파일에 대한 포인터 설정 변수 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	int Ret=0;

	nlength = strlen(cp_msg);
	if(nlength <= 0) //저장할 메세지가 없다 
	{
		return BD_ERROR;
	}
	str_msg = cp_msg;
 
	// **************************************************************************
	// 파일 이름으로 사용할 날짜 정보를 얻는다                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // 현재 시간 정보를 얻는다. 

	m_time_current = CTime::GetCurrentTime() ;  // 간략한 형식의 현재 시간 정보 얻는다. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();
	// **************************************************************************

	// **************************************************************************
	// 날짜 정보를 문자형으로 변환하여 변수에 설정한다                           
	// **************************************************************************
	mstr_cur_year = LPCTSTR(_itoa( mn_cur_year, chr_buf, 10 ));  
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// 현재 시간 정보 얻는다                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();				// 현재 시간 정보를 설정한다. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// 현재 시간 정보를 설정한다.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// 현재 분 정보를 설정한다. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// 현재 초 정보를 설정한다. 
	str_display_time = m_time_current.Format("%c");	// 리스트 파일에 출력할 타이틀 시간 정보 설정 
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
	{//해당 파일이 존재하지 않는 상태이다 
		CreateFolder(cp_dir_path); 
	}
	else 
	{//해당 파일이 이미 존재한다.
		//정상 
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

		fclose(fp) ;  //2011.0417  파일 종료

//2011.0417		sing.Unlock(); //2011.0417 추가 
		return BD_ERROR;
	}
	// **************************************************************************
	fclose(fp);  // 파일을 종료한다.

//2011.0417	sing.Unlock(); //2011.0417 추가 
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
		if (nRet == BD_GOOD) //이미 해당 위치에 와 있으면 동작하지 않고 모터 이동을 끝낸다 
		{
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_mot_retry_cnt[n_MotNum] = 0;
		mn_mot_max_retry_cnt = 3; //최대 3회 리트라이 동작을 수행한다 
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
		nRet = COMI.Start_SingleMove(n_MotNum, d_MotPos, n_SpeedRate);  // 해당 위치로 이동
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
		else if (nRet == BD_RETRY)  // 동작 재시도
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
		nRet = COMI.Check_SingleMove(n_MotNum, d_MotPos);  // 이동 완료 확인
		if (nRet == BD_GOOD)  // 정상 완료
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
		else if (nRet == BD_RETRY)  // 동작 재시도
		{
			// 무한루프에 빠질수도 있지만 횟수를 지정해서 사용하면 문제 없다
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
		{//이미 모든 모터의 현재 위치가 이동하려는 위치와 허용오차 안에 있으면 동작할 필요가 없으니 끝내자.
			nFuncRet = BD_GOOD;
			break;
		}
		
		mn_linear_retry_cnt[n_LinearMapIndex] = 0;
		mn_linear_max_retry_cnt = 3; //최대 3회 리트라이 동작을 수행한다 
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
		else if (nRet[0] == BD_RETRY)  // 동작 재시도
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
		if (nRet[0] == BD_GOOD)  // 정상 완료
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
		else if (nRet[0] == BD_RETRY)  // 동작 재시도
		{
			// 무한루프에 빠질수도 있지만 횟수를 지정해서 사용하면 문제 없다
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
				{//250008 0 25 "Clamp rbt Z 로봇 안전위치에 있지 않음."
					sprintf(COMI.mc_alarmcode,"250008");
					return BD_ERROR;
					
				}
			}
			if(d_CurPos[0] > st_motor[M_CLIP_INSERT_Y + nPos].md_pos[Y_CLIP_WORK_PLACE1] - st_motor[M_CLIP_INSERT_Y].mn_allow &&
				d_CurPos[0] < st_motor[M_CLIP_INSERT_Y + nPos].md_pos[Y_CLIP_WORK_PLACE1] + st_motor[M_CLIP_INSERT_Y].mn_allow)
			{
				if(d_CurPos[1] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Z].mn_allow)
				{//270008 0 27 "Clip Inset Z 로봇 안전위치에 있지 않음."
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
				{//310008 0 31 "RBT Sorter Z 로봇 안전위치에 있지 않음."
					sprintf(COMI.mc_alarmcode,"310008");
					return BD_ERROR;
					
				}
			}
		}
	}

	return nFuncRet;
}