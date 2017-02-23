// Run_TowerLamp.cpp: implementation of the CRun_TowerLamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "Run_TowerLamp.h"

#include "Variable.h"				// ���� ���� ���� Class
#include "Public_Function.h"		// Motor �� Cylinder ���� Class
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
    // ���� ��µ� ȭ���� I/O ����͸� ȭ������ �˻��Ѵ�                         
	// -> I/O ����� �� Ÿ�� ������ ��� ��µǾ� I/O ���� Ȯ���� ����� ����    
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
			//  RED Ÿ�� ���� ��� �����Ѵ�												
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
			FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  GREEN Ÿ�� ���� ��� �����Ѵ�													
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 1 :
			// ************************************************************************
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step++;
			// ************************************************************************
			break;
		case 2 :
			// ************************************************************************
			//  RED Ÿ�� ���� ��� �����Ѵ�												
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
			FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			//  GREEN Ÿ�� ���� ��� �����Ѵ�													
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 3 :
			// ************************************************************************
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step++;
			// ************************************************************************
			break;
		case 4:
			// ************************************************************************
			//  RED Ÿ�� ���� ��� �����Ѵ�												
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  GREEN Ÿ�� ���� ��� �����Ѵ�													
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
			FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 5:
			// ************************************************************************
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
			// ************************************************************************
			ml_lamp_time[1] = GetCurrentTime();
			if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
				mn_lamp_step++;
			// ************************************************************************
			break;
		case 6 :
			// ************************************************************************
			//  RED Ÿ�� ���� ��� �����Ѵ�												
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
			FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_ON);
			// ************************************************************************
			// ************************************************************************
			//  GREEN Ÿ�� ���� ��� �����Ѵ�													
			// ************************************************************************
			FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
			FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_OFF);
			// ************************************************************************
			// ************************************************************************
			// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
			// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
			// ************************************************************************
			ml_lamp_time[0] = GetCurrentTime();
			mn_lamp_step++;
			// ************************************************************************
			break;
		case 7 :
			// ************************************************************************
			// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
			// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
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
		if (st_handler.mn_reinstatement == CTL_YES)	// ���� �����̴�.
		{
			switch(mn_lamp_step)
			{
			case 0 :
				// ************************************************************************
				//  GREEN Ÿ�� ���� ��� �����Ѵ�													
				// ************************************************************************
				FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);	// GREEN Ÿ�� ���� ON
				// ************************************************************************

				FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_ON);		// Start Button ���� ON

				// ************************************************************************
				// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
				// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;

			case 1 :
				// ************************************************************************
				// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
				// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
				// ************************************************************************
				ml_lamp_time[1] = GetCurrentTime();
				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
					mn_lamp_step++;
				// ************************************************************************
				break;
			
			case 2 :
				// ************************************************************************
				//  GREEN Ÿ�� ���� ��� �����Ѵ�											
				// ************************************************************************
				FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);	// GREEN Ÿ�� ���� OFF
				// ************************************************************************

				FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);			// Start Button ���� OFF

				// ************************************************************************
				// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
				// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;

			case 3 :
				// ************************************************************************
				// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
				// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
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
				//  RED Ÿ�� ���� ��� �����Ѵ�												
				// ************************************************************************
				if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 1)       // RED Ÿ�� ���� ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 0)  // RED Ÿ�� ���� OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 2)  // RED Ÿ�� ���� FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
				// ************************************************************************

				// ************************************************************************
				//  GREEN Ÿ�� ���� ��� �����Ѵ�													
				// ************************************************************************
				if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 1)       // GREEN Ÿ�� ���� ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 0)  // GREEN Ÿ�� ���� OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 2)  // GREEN Ÿ�� ���� FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
				// ************************************************************************

				// ************************************************************************
				//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
				// ************************************************************************
				if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 1)       // YELLOW Ÿ�� ���� ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 0)  // YELLOW Ÿ�� ���� OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 2)  // YELLOW Ÿ�� ���� FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
				// ************************************************************************

				// ************************************************************************
				// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
				// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;
			case 1 :
				// ************************************************************************
				// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
				// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
				// ************************************************************************
				ml_lamp_time[1] = GetCurrentTime();
				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
					mn_lamp_step++;
				// ************************************************************************
				break;
			case 2 :
				// ************************************************************************
				//  RED Ÿ�� ���� ��� �����Ѵ�												
				// ************************************************************************
				if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 1)       // RED Ÿ�� ���� ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 0)  // RED Ÿ�� ���� OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 2)  // RED Ÿ�� ���� FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
				// ************************************************************************

				// ************************************************************************
				//  GREEN Ÿ�� ���� ��� �����Ѵ�											
				// ************************************************************************
				if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 1)       // GREEN Ÿ�� ���� ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 0)  // GREEN Ÿ�� ���� OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 2)  // GREEN Ÿ�� ���� FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
				// ************************************************************************

				// ************************************************************************
				//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
				// ************************************************************************
				if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 1)       // YELLOW Ÿ�� ���� ON
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 0)  // YELLOW Ÿ�� ���� OFF
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 2)  // YELLOW Ÿ�� ���� FLICK
					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
				// ************************************************************************

				// ************************************************************************
				// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
				// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
				// ************************************************************************
				ml_lamp_time[0] = GetCurrentTime();
				mn_lamp_step++;
				// ************************************************************************
				break;
			case 3 :
				// ************************************************************************
				// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
				// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
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
// 				//  RED Ÿ�� ���� ��� �����Ѵ�												
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 1)       // RED Ÿ�� ���� ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 0)  // RED Ÿ�� ���� OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 2)  // RED Ÿ�� ���� FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  GREEN Ÿ�� ���� ��� �����Ѵ�													
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 1)       // GREEN Ÿ�� ���� ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 0)  // GREEN Ÿ�� ���� OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 2)  // GREEN Ÿ�� ���� FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 1)       // YELLOW Ÿ�� ���� ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 0)  // YELLOW Ÿ�� ���� OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 2)  // YELLOW Ÿ�� ���� FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
// 				// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
// 				// ************************************************************************
// 				ml_lamp_time[0] = GetCurrentTime();
// 				mn_lamp_step++;
// 				// ************************************************************************
// 				break;
// 			case 1 :
// 				// ************************************************************************
// 				// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
// 				// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
// 				// ************************************************************************
// 				ml_lamp_time[1] = GetCurrentTime();
// 				if ((ml_lamp_time[1] - ml_lamp_time[0]) > st_lamp.mn_lamp_wait_time)
// 					mn_lamp_step++;
// 				// ************************************************************************
// 				break;
// 			case 2 :
// 				// ************************************************************************
// 				//  RED Ÿ�� ���� ��� �����Ѵ�												
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 1)       // RED Ÿ�� ���� ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_ON);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 0)  // RED Ÿ�� ���� OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
// 				else if (st_lamp.mn_lamp_r[st_work.mn_run_status] == 2)  // RED Ÿ�� ���� FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_red, IO_OFF);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  GREEN Ÿ�� ���� ��� �����Ѵ�											
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 1)       // GREEN Ÿ�� ���� ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_ON);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 0)  // GREEN Ÿ�� ���� OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
// 				else if (st_lamp.mn_lamp_g[st_work.mn_run_status] == 2)  // GREEN Ÿ�� ���� FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_green, IO_OFF);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				//  YELLOW Ÿ�� ���� ��� �����Ѵ�											
// 				// ************************************************************************
// 				if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 1)       // YELLOW Ÿ�� ���� ON
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_ON);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 0)  // YELLOW Ÿ�� ���� OFF
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
// 				else if (st_lamp.mn_lamp_y[st_work.mn_run_status] == 2)  // YELLOW Ÿ�� ���� FLICK
// 					FAS_IO.set_out_bit(st_io.o_towerlamp_yellow, IO_OFF);
// 				// ************************************************************************
// 
// 				// ************************************************************************
// 				// Ÿ�� ���� ON/OFF ���� �����ϱ� ���Ͽ� ���� �ð� ��´�					
// 				// -> ȭ�� ���� �ð���ŭ ON/OFF �ð� �����Ѵ�								
// 				// ************************************************************************
// 				ml_lamp_time[0] = GetCurrentTime();
// 				mn_lamp_step++;
// 				// ************************************************************************
// 				break;
// 			case 3 :
// 				// ************************************************************************
// 				// ȭ�� ���� ���� ON/OFF �ð����� ����Ѵ�									
// 				// -> st_lamp.mn_lamp_wait_time : ���� ON/OFF ���� �ð� ���� ����			
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