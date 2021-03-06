// Run_TowerLamp.cpp: implementation of the CRun_TowerLamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "Run_TowerLamp.h"

#include "Variable.h"				// 전역 변수 정의 Class
#include "Public_Function.h"		// Motor 및 Cylinder 동작 Class
#include "FastechPublic_IO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRun_TowerLamp Run_TowerLamp;
CRun_TowerLamp Run_TowerLamp_Manual;

CRun_TowerLamp::CRun_TowerLamp()
{
	MoveStep = 0;
	mn_lamp_step = 0;
	mn_pcb_exist_check = CTL_NO;
}

CRun_TowerLamp::~CRun_TowerLamp()
{

}

void CRun_TowerLamp::Thread_Run()
{
	Run_Move();
}

void CRun_TowerLamp::Run_Move()
{
	int Ret = 0;
	int i = 0, j = 0, k = 0;
	int x = 0, y = 0;

	// **************************************************************************
    // 현재 출력된 화면이 I/O 모니터링 화면인지 검사한다                         
	// -> I/O 디버깅 시 타워 램프는 계속 출력되어 I/O 상태 확인이 어려워 수정    
	// -> dSTOP(0)    dRUN(1)    dJAM(2)    dLOTEND(3)   dINIT(4)    dWARNING(5) 
	// -> dLOCK(6)    dSELFCHECK(7)												 
    // **************************************************************************
	if (st_handler.mn_menu_num == 701)  return;
	// **************************************************************************

	if (st_work.mn_run_status == dJAM || st_work.mn_run_status == dWARNING)
	{
	   FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_ON);
	   FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
	}

	if (st_work.mn_run_status == dINIT)
	{
		switch(mn_lamp_step)
		{
		case 0 :
			// ************************************************************************
			//  RED 타워 램프 출력 제어한다												
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
			FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW 타워 램프 출력 제어한다											
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  GREEN 타워 램프 출력 제어한다													
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 1 :
			// ************************************************************************
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step++;
			// ************************************************************************
			break;
		case 2 :
			// ************************************************************************
			//  RED 타워 램프 출력 제어한다												
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW 타워 램프 출력 제어한다											
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
			FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			//  GREEN 타워 램프 출력 제어한다													
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 3 :
			// ************************************************************************
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step++;
			// ************************************************************************
			break;
		case 4:
			// ************************************************************************
			//  RED 타워 램프 출력 제어한다												
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW 타워 램프 출력 제어한다											
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  GREEN 타워 램프 출력 제어한다													
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
			FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 5:
			// ************************************************************************
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step++;
			// ************************************************************************
			break;
		case 6 :
			// ************************************************************************
			//  RED 타워 램프 출력 제어한다												
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW 타워 램프 출력 제어한다											
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
			FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			//  GREEN 타워 램프 출력 제어한다													
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
			// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 7 :
			// ************************************************************************
			// 화면 설정 램프 ON/OFF 시간동안 대기한다									
			// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step = 0;
			// ************************************************************************
			break;
		}
	}
	else
	{
		if (st_handler.mn_reinstatement == CTL_YES)	// 복구 동작이다.
		{
			switch(mn_lamp_step)
			{
			case 0 :
				// ************************************************************************
				//  GREEN 타워 램프 출력 제어한다													
				// ************************************************************************
				FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);	// GREEN 타워 램프 ON
				// ************************************************************************

				FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_ON);		// Start Button 램프 ON

				// ************************************************************************
				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;

			case 1 :
				// ************************************************************************
				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
				// ************************************************************************
				ml_lamp_time[1] = GetCurrentTime();
				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
					mn_lamp_step++;
				// ************************************************************************
				break;
			
			case 2 :
				// ************************************************************************
				//  GREEN 타워 램프 출력 제어한다											
				// ************************************************************************
				FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);	// GREEN 타워 램프 OFF
				// ************************************************************************

				FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);			// Start Button 램프 OFF

				// ************************************************************************
				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;

			case 3 :
				// ************************************************************************
				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
				// ************************************************************************
				ml_lamp_time[1] = GetCurrentTime();
				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
					mn_lamp_step = 0;
				// ************************************************************************
				break;
			default:
				mn_lamp_step = 0;
				break;
			}
		}
		else
		{
			switch(mn_lamp_step)
			{
			case 0 :
				// ************************************************************************
				//  RED 타워 램프 출력 제어한다												
				// ************************************************************************
				if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 1)       // RED 타워 램프 ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 0)  // RED 타워 램프 OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 2)  // RED 타워 램프 FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
				// ************************************************************************

				// ************************************************************************
				//  GREEN 타워 램프 출력 제어한다													
				// ************************************************************************
				if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 1)       // GREEN 타워 램프 ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 0)  // GREEN 타워 램프 OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 2)  // GREEN 타워 램프 FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
				// ************************************************************************

				// ************************************************************************
				//  YELLOW 타워 램프 출력 제어한다											
				// ************************************************************************
				if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 1)       // YELLOW 타워 램프 ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 0)  // YELLOW 타워 램프 OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 2)  // YELLOW 타워 램프 FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
				// ************************************************************************

				// ************************************************************************
				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;
			case 1 :
				// ************************************************************************
				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
				// ************************************************************************
				ml_lamp_time[1] = GetCurrentTime();
				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
					mn_lamp_step++;
				// ************************************************************************
				break;
			case 2 :
				// ************************************************************************
				//  RED 타워 램프 출력 제어한다												
				// ************************************************************************
				if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 1)       // RED 타워 램프 ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 0)  // RED 타워 램프 OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 2)  // RED 타워 램프 FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
				// ************************************************************************

				// ************************************************************************
				//  GREEN 타워 램프 출력 제어한다											
				// ************************************************************************
				if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 1)       // GREEN 타워 램프 ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 0)  // GREEN 타워 램프 OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 2)  // GREEN 타워 램프 FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
				// ************************************************************************

				// ************************************************************************
				//  YELLOW 타워 램프 출력 제어한다											
				// ************************************************************************
				if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 1)       // YELLOW 타워 램프 ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 0)  // YELLOW 타워 램프 OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 2)  // YELLOW 타워 램프 FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
				// ************************************************************************

				// ************************************************************************
				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;
			case 3 :
				// ************************************************************************
				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
				// ************************************************************************
				ml_lamp_time[1] = GetCurrentTime();
				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
					mn_lamp_step = 0;
				// ************************************************************************
				break;
			default:
				mn_lamp_step = 0;
				break;
			}

			// Original
// 			switch(mn_lamp_step)
// 			{
// 			case 0 :
// 				// ************************************************************************
// 				//  RED 타워 램프 출력 제어한다												
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 1)       // RED 타워 램프 ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 0)  // RED 타워 램프 OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 2)  // RED 타워 램프 FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  GREEN 타워 램프 출력 제어한다													
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 1)       // GREEN 타워 램프 ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 0)  // GREEN 타워 램프 OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 2)  // GREEN 타워 램프 FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  YELLOW 타워 램프 출력 제어한다											
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 1)       // YELLOW 타워 램프 ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 0)  // YELLOW 타워 램프 OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 2)  // YELLOW 타워 램프 FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
// 				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
// 				// ************************************************************************
// 				ml_lamp_time[0] = GetCurrentTime();
// 				mn_lamp_step++;
// 				// ************************************************************************
// 				break;
// 			case 1 :
// 				// ************************************************************************
// 				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
// 				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
// 				// ************************************************************************
// 				ml_lamp_time[1] = GetCurrentTime();
// 				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
// 					mn_lamp_step++;
// 				// ************************************************************************
// 				break;
// 			case 2 :
// 				// ************************************************************************
// 				//  RED 타워 램프 출력 제어한다												
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 1)       // RED 타워 램프 ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 0)  // RED 타워 램프 OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 2)  // RED 타워 램프 FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  GREEN 타워 램프 출력 제어한다											
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 1)       // GREEN 타워 램프 ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 0)  // GREEN 타워 램프 OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 2)  // GREEN 타워 램프 FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  YELLOW 타워 램프 출력 제어한다											
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 1)       // YELLOW 타워 램프 ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 0)  // YELLOW 타워 램프 OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 2)  // YELLOW 타워 램프 FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				// 타워 램프 ON/OFF 간격 제어하기 위하여 현재 시간 얻는다					
// 				// -> 화면 설정 시간만큼 ON/OFF 시간 유지한다								
// 				// ************************************************************************
// 				ml_lamp_time[0] = GetCurrentTime();
// 				mn_lamp_step++;
// 				// ************************************************************************
// 				break;
// 			case 3 :
// 				// ************************************************************************
// 				// 화면 설정 램프 ON/OFF 시간동안 대기한다									
// 				// -> st_lamp.mn_lamp_wait_time : 램프 ON/OFF 유지 시간 저장 변수			
// 				// ************************************************************************
// 				ml_lamp_time[1] = GetCurrentTime();
// 				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
// 					mn_lamp_step = 0;
// 				// ************************************************************************
// 				break;
// 			default:
// 				mn_lamp_step = 0;
// 				break;
// 			}
		}
	}
}

void CRun_TowerLamp::Run_FormChange()
{
	switch (mn_form_step)
	{
	case 100:
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 9, 1);  // IO
		ml_form_time[0] = GetCurrentTime();
		mn_form_step = 200;
		break;

	case 200:
		ml_form_time[1] = GetCurrentTime();
		ml_form_time[2] = ml_form_time[1] - ml_form_time[0];

		if (ml_form_time[2] > 100)
		{
			mn_form_step = 300;
		}
		break;

	case 300:
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 5, 1);  // XYZ
		ml_form_time[0] = GetCurrentTime();
		mn_form_step = 400;
		break;

	case 400:
		ml_form_time[1] = GetCurrentTime();
		ml_form_time[2] = ml_form_time[1] - ml_form_time[0];
		
		if (ml_form_time[2] > 100)
		{
			mn_form_step = 100;
		}
		break;
	}
}
