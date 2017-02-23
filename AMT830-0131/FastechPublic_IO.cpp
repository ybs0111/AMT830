// FastechPublic_IO.cpp: implementation of the CFastechPublic_IO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FastechPublic_IO.h"

#include "FAS_HSSI.h"
#include "math.h"
//#include "io.h"     // ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� INCLUDE �ʿ� 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFastechPublic_IO  FAS_IO;  

CFastechPublic_IO::CFastechPublic_IO()
{
	SafetyAreaStep = 0;
	n_retry = 0;
}

CFastechPublic_IO::~CFastechPublic_IO()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//2011.1019 james lee WORD ������ I/O�� �����Ѵ�
//Ver 0.0.1_2011.1019
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


//2011.1019 james //Word ������ ����� ����ϴ� �Լ� 
BOOL CFastechPublic_IO::get_in_bit(int n_IONo, int n_OnOff)  //2011.1019 Simulation ���۱��� �������� onoff �߰� 
{
/*
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo = 0;
	BOOL    bData=0;
*/
	if(mn_simulation_mode == 1) //Simulation ����̸� ���α׷��� �����ؾ� �ϴ� I/O ���¸� ���ϴ� ������ ������ �ش� 
	{
		n_in[n_IONo] = n_OnOff;
		return n_in[n_IONo]; 
	}

	return n_in[n_IONo];

	//���ϴ� ������� �ʴ´� 
	/*
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadInputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}	
	return bData;*/
}

int CFastechPublic_IO::set_out_bit(int n_IONo, BOOL b_OnOff) 
{
/*
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;
*/

	n_out[n_IONo] = b_OnOff;
	return STATUS_SUCCESS;


	//���ϴ� ������� �ʴ´� 
	/*
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_WriteOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_OnOff);  // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;*/
}

BOOL CFastechPublic_IO::get_out_bit(int n_IONo, int n_OnOff)
{
/*
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0,nBitNo=0;
	BOOL    bData;
*/

	if(mn_simulation_mode == 1) //Simulation ����̸� ���α׷��� �����ؾ� �ϴ� I/O ���¸� ���ϴ� ������ ������ �ش� 
	{		
		return n_out[n_IONo] = n_OnOff;
	}

	return n_out[n_IONo];
	//���ϴ� ������� �ʴ´� 
	/*
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);
	
	nRet = HSSI_ReadOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}	
	return bData;*/
}

int CFastechPublic_IO::IO_ReadWrite(int n_bd_num, int n_port_num, int n_slave_num, int n_rw_mode)
{//n_rw_mode : -1:������ �ʱ�ȭ, 0=> initialize i/o ���� read, 1:input, 2:output
	WORD	wData = 0;
	int     i=0, nRet=0, nBoardID=-1, nPortNo=0, nSlaveNo=0, nIO_Num = 0;
 
//#define MAX_PCI_SLOT 8	// 0~7 8��
//#define MAX_PORT_NO 4	// 0~3 4��
//#define MAX_SLAVE_NO 8	// 0~7 8��


	//���� ������ �̿��Ͽ� io port ��ȣ�� �����Ѵ� 
	if(n_bd_num < 0 || n_bd_num > MAX_PCI_SLOT - 6) //MAX_PCI_SLOT - 1)
	{//	����� ���� 8����� ��밡���ϳ� ���� MAX�� 2������ �Ѵ� 
		AfxMessageBox("IO Board [0 ~ 1] Max Num Over");//[0 ~ 7] Max Num Over");
		return BD_ERROR;
	}
	else nBoardID = n_bd_num * 4000;

	if(n_port_num < 0 || n_port_num > MAX_PORT_NO - 1)
	{
		AfxMessageBox("IO Board Port [0 ~ 3] Max Num Over");
		return BD_ERROR;
	}
	else nPortNo   = n_port_num * 1000;

	if(n_slave_num < 0 || n_slave_num > MAX_SLAVE_NO - 1)
	{
		AfxMessageBox("IO Board Slave [0 ~ 7] Max Num Over");
		return BD_ERROR;
	}
	else nSlaveNo   = n_slave_num * 100;
	
	
    nIO_Num = nBoardID + nPortNo + nSlaveNo;
	if(nIO_Num > HSSI_MAX_IO - 1) //#define HSSI_MAX_IO		8716 //max io board 2�� ������ �����Ͽ� ������� //max io board 4�� ������ ó���Ѵ� 
	{
		AfxMessageBox("IO Num [8715] Max Num Over");
		return BD_ERROR;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//��� I/O port�� define �� �����Ʈ �ʱ�ȭ �� I/O ���� �������� �� ��Ʈ�� ���¸� �о� ������ �����Ͽ� �ش� 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(n_rw_mode == -1) //input, output port ������� ó�� �ѹ� �ʱ�ȭ �� �� 
	{
		for(i=0; i<HSSI_MAX_IO; i++) //��Ʈ ��� ���� �ʱ�ȭ 
		{
			n_in[i]  = 0;//-1;
			n_out[i] = -1;
		}

// 		n_out[st_io.o_robot_picker_01_cyl] = 1;			//  S0000
// 		n_out[st_io.o_robot_picker_02_cyl] = 1;			//  S0001
// 		n_out[st_io.o_robot_picker_03_cyl] = 1;			//  S0002
// 		n_out[st_io.o_robot_picker_04_cyl] = 1;			//  S0003
// 		n_out[st_io.o_robot_picker_05_cyl] = 1;			//  S0004
// 		n_out[st_io.o_robot_picker_06_cyl] = 1;			//  S0005
// 		n_out[st_io.o_robot_picker_07_cyl] = 1;			//  S0006
// 		n_out[st_io.o_robot_picker_08_cyl] = 1;			//  S0007
// 		n_out[st_io.o_robot_picker_09_cyl] = 1;			//  S0008
// 		n_out[st_io.o_robot_picker_10_cyl] = 1;			//  S0009
// 		
// 		// ************************************************************
// 		//  Robot Gripper On/Off
// 		// ************************************************************
// 		n_out[st_io.o_robot_gripper_01_cyl] = 1;			//  S0100
// 		n_out[st_io.o_robot_gripper_02_cyl] = 1;			//  S0101
// 		n_out[st_io.o_robot_gripper_03_cyl] = 1;			//  S0102
// 		n_out[st_io.o_robot_gripper_04_cyl] = 1;			//  S0103
// 		n_out[st_io.o_robot_gripper_05_cyl] = 1;			//  S0104
// 		n_out[st_io.o_robot_gripper_06_cyl] = 1;			//  S0105
// 		n_out[st_io.o_robot_gripper_07_cyl] = 1;			//  S0106
// 		n_out[st_io.o_robot_gripper_08_cyl] = 1;			//  S0107
// 		n_out[st_io.o_robot_gripper_09_cyl] = 1;			//  S0108
// 		n_out[st_io.o_robot_gripper_10_cyl] = 1;			//  S0109
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  Robot Gripper On Check
// 		// ************************************************************
// 		// ************************************************************
// 		//  Buffer Check
// 		// ************************************************************
// 		n_out[st_io.o_reject_buf_shift_on_cyl] = 1;			//  S0301
// 		n_out[st_io.o_reject_buf_shift_off_cyl] = 1;		//  S0302
// 		n_out[st_io.o_gripper_length_ready_cyl] = 1;		//  S0303
// 		n_out[st_io.o_gripper_length_unready_cyl] = 1;		//  S0304	
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  Loader Conveyor
// 		// ************************************************************
// 		n_out[st_io.o_ld_conveyor_tray_stopper_cyl] = 1;			//  S0400
// 		n_out[st_io.o_ld_conveyor_tray_elev_change_on_cyl] = 1;			//  S0401
// 		n_out[st_io.o_ld_conveyor_tray_elev_change_off_cyl] = 1;			//  S0402
// 		n_out[st_io.o_ld_conveyor_motor_onoff] = 1;			//  S0403
// 		n_out[st_io.o_ld_conveyor_motor_cw] = 1;			//  S0404
// 		n_out[st_io.o_ld_conveyor_motor_ccw] = 1;			//  S0405
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  Loader Tray Check
// 		// ************************************************************
// 		n_out[st_io.o_ld_zone_tray_change_on_cyl] = 1;			//  S0500
// 		n_out[st_io.o_ld_zone_tray_change_off_cyl] = 1;			//  S0501
// 		n_out[st_io.o_ld_zone_tray_seperate_cyl] = 1;			//  S0502
// 		n_out[st_io.o_ld_zone_tray_base_rail_fbwd_cyl] = 1;			//  S0503
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  Loader Tray Move
// 		// ************************************************************
// 		n_out[st_io.o_ld_zone_1_tray_clamp_cyl] = 1;			//  S0600
// 		n_out[st_io.o_ld_zone_2_tray_clamp_cyl] = 1;			//  S0601
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  Loader Tray Move
// 		// ************************************************************
// 		n_out[st_io.o_ld_zone_tray_move_fwd_cyl] = 1;			//  S0700
// 		n_out[st_io.o_ld_zone_tray_move_bwd_cyl] = 1;			//  S0701
// 		n_out[st_io.o_ld_zone_tray_hook_cyl] = 1;			//  S0702
// 		n_out[st_io.o_uld_zone_tray_hook_cyl] = 1;			//  S0703
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  Unloader Air Blow
// 		// ************************************************************
// 		n_out[st_io.o_uld_air_blow_tray_in_shutter_cyl] = 1;			//  S1000
// 		n_out[st_io.o_uld_air_blow_1_onoff] = 1;			//  S1001
// 		n_out[st_io.o_uld_air_blow_2_onoff] = 1;			//  S1002
// 		n_out[st_io.o_uld_air_blow_tray_out_shutter_cyl] = 1;			//  S1003
// 		
// 		n_out[st_io.o_uld_air_blow_motor_onoff] = 1;			//  S1006
// 		n_out[st_io.o_uld_air_blow_motor_cw] = 1;			//  S1007
// 		n_out[st_io.o_uld_air_blow_motor_ccw] = 1;			//  S1008
// 		n_out[st_io.o_uld_tray_pusher_motor_onoff] = 1;			//  S1009
// 		n_out[st_io.o_uld_tray_pusher_motor_cw] = 1;			//  S1010
// 		n_out[st_io.o_uld_tray_pusher_motor_ccw] = 1;			//  S1011
// 
// 		n_out[st_io.o_door_onoff_cyl] = 1;			//  S1013
// 		n_out[st_io.o_reject_buf_door_onoff] = 1;			//  S1014
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  Unloader Air Blow Box
// 		// ************************************************************
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  Unloader Air Blow Box
// 		// ************************************************************
// 		n_out[st_io.o_uld_tray_base_rail_move_cyl] = 1;			//  S1200
// 		n_out[st_io.o_uld_tray_clamp_cyl] = 1;			//  S1201
// 		n_out[st_io.o_uld_tray_small_size_stopper_cyl] = 1;			//  S1202
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  Unloader Air Blow Box
// 		// ************************************************************
// 		n_out[st_io.o_uld_conveyor_tray_stopper_cyl] = 1;			//  S1300
// 		n_out[st_io.o_uld_conveyor_motor_onoff] = 1;			//  S1301
// 		n_out[st_io.o_uld_conveyor_motor_cw] = 1;			//  S1302
// 		n_out[st_io.o_uld_conveyor_motor_ccw] = 1;			//  S1303
// 		n_out[st_io.o_uld_conveyor_tray_change_on_cyl] = 1;			//  S1304
// 		n_out[st_io.o_uld_conveyor_tray_change_off_cyl] = 1;			//  S1305
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  Switch & Tower Lamp
// 		// ************************************************************
// 		n_out[st_io.o_start_switch_lamp] = 1;			//	S1400
// 		n_out[st_io.o_stop_switch_lamp] = 1;			//	S1401
// 		n_out[st_io.o_alarm_reset_switch_lamp] = 1;			//	S1402
// 		n_out[st_io.o_buzzer_off_switch_lamp] = 1;			//	S1403
// 		n_out[st_io.o_main_air] = 1;			//	S1404
// 		n_out[st_io.o_towerlamp_green] = 1;			//	S1405
// 		n_out[st_io.o_towerlamp_yellow] = 1;			//	S1406
// 		n_out[st_io.o_towerlamp_red] = 1;			//	S1407
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  LIGHT CURTAIN
// 		// ************************************************************
// 		n_out[st_io.o_buzzer_1] = 1;			//	S1500
// 		n_out[st_io.o_buzzer_2] = 1;			//	S1501
// 		n_out[st_io.o_buzzer_3] = 1;			//	S1502
// 		n_out[st_io.o_buzzer_4] = 1;			//	S1503
// 		n_out[st_io.o_dust_collector_onoff] = 1;			//  S1504
// 		// ************************************************************
// 
// 		// ************************************************************
// 		//  SMEMA
// 		// ************************************************************
// 		n_out[st_io.o_front_machine_front_request_sginal] = 1;			//	S1600
// 		n_out[st_io.o_front_machine_rear_request_sginal] = 1;			//	S1601
// 		n_out[st_io.o_rear_machine_transfer_signal] = 1;			//	S1602
// 	

	}

	if(mn_simulation_mode == 1) //Simulation ����̸� ���α׷��� �����ؾ� �ϴ� I/O ���¸� ���ϴ� ������ ������ �ش� 
	{//������ ���ķ��̼� ���� ���α׷� �׽�Ʈ�� ������ ������ ������ ������ Ŭ���� �Ǵ� �����Ѵ� 
		return BD_GOOD; 
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ��� �ʱ�ȭ�� ó���ѹ� Input �� output port�� ���¸� �޴´� 
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(n_rw_mode == 0) //initialize total input/ output read mode 
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//���� ���� ���� �ѹ� �ʱ�ȭ
		// in/out ������ -1�̸� ������� �ʴ� ��Ʈ�̰�, 0 �Ǵ� 1�̸� ���������� ��񿡼� �ÿ��ϴ� �����̴� 
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// ************************************************************************** //
		// �Է�(��� ��ȣ 00��)								                          //
		// ************************************************************************** //
		nRet = HSSI_ReadInputWord(n_bd_num, n_port_num, n_slave_num, &wData); //(iMasterID, iPortNo, iSlaveNo, &wData); 
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error

		n_in[nIO_Num + 0]	= (wData & DBIT0)   ? 1 : 0 ;		// PS000 (���: 0	��Ʈ: 0) 
		n_in[nIO_Num + 1]	= (wData & DBIT1)   ? 1 : 0 ;		// PS001 (���: 0	��Ʈ: 1) 
		n_in[nIO_Num + 2]	= (wData & DBIT2)   ? 1 : 0 ;		// PS002 (���: 0	��Ʈ: 2) 
		n_in[nIO_Num + 3]	= (wData & DBIT3)   ? 1 : 0 ;		// PS003 (���: 0	��Ʈ: 3) 
		n_in[nIO_Num + 4]	= (wData & DBIT4)   ? 1 : 0 ;		// PS004 (���: 0	��Ʈ: 4)
		n_in[nIO_Num + 5]	= (wData & DBIT5)   ? 1 : 0 ;		// PS005 (���: 0	��Ʈ: 5) 
		n_in[nIO_Num + 6]	= (wData & DBIT6)   ? 1 : 0 ;		// PS006 (���: 0	��Ʈ: 6) 
		n_in[nIO_Num + 7]	= (wData & DBIT7)   ? 1 : 0 ;		// PS007 (���: 0	��Ʈ: 7) 
		n_in[nIO_Num + 8]	= (wData & DBIT8)   ? 1 : 0 ;		// PS008 (���: 0	��Ʈ: 8) 
		n_in[nIO_Num + 9]	= (wData & DBIT9)   ? 1 : 0 ;		// PS009 (���: 0	��Ʈ: 9) 
		n_in[nIO_Num + 10]	= (wData & DBIT10)  ? 1 : 0 ;		// PS010 (���: 0	��Ʈ: 10) 
		n_in[nIO_Num + 11]	= (wData & DBIT11)  ? 1 : 0 ;		// PS011 (���: 0	��Ʈ: 11) 
		n_in[nIO_Num + 12]	= (wData & DBIT12)  ? 1 : 0 ;		// PS012 (���: 0	��Ʈ: 12) 
		n_in[nIO_Num + 13]	= (wData & DBIT13)  ? 1 : 0 ;		// PS013 (���: 0	��Ʈ: 13) 
		n_in[nIO_Num + 14]	= (wData & DBIT14)  ? 1 : 0 ;		// PS014 (���: 0	��Ʈ: 14) 
		n_in[nIO_Num + 15]	= (wData & DBIT15)  ? 1 : 0 ;		// PS015 (���: 0	��Ʈ: 15) 

		// ************************************************************************** //
		// ���(��� ��ȣ 00��)								                          //
		// ************************************************************************** //
		nRet = HSSI_ReadOutputWord(n_bd_num, n_port_num, n_slave_num, &wData); //(iMasterID, iPortNo, iSlaveNo, &wData); 
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error

		for(i=0; i<16; i++)//io port 16��
		{
// 			if(n_out[nIO_Num + i] == 0 || n_out[nIO_Num + i] == 1) //0:off, 1:on
// 			{
				if     (i == 0)	n_out[nIO_Num + 0]		= (wData & DBIT0)   ? 1 : 0 ;		// S000 (���: 0	��Ʈ: 0) 
				else if(i == 1)	n_out[nIO_Num + 1]		= (wData & DBIT1)   ? 1 : 0 ;		// S001 (���: 0	��Ʈ: 1) 
				else if(i == 2) n_out[nIO_Num + 2]		= (wData & DBIT2)   ? 1 : 0 ;		// S002 (���: 0	��Ʈ: 2) 
				else if(i == 3) n_out[nIO_Num + 3]		= (wData & DBIT3)   ? 1 : 0 ;		// S003 (���: 0	��Ʈ: 3) 
				else if(i == 4) n_out[nIO_Num + 4]		= (wData & DBIT4)   ? 1 : 0 ;		// S004 (���: 0	��Ʈ: 4)
				else if(i == 5) n_out[nIO_Num + 5]		= (wData & DBIT5)   ? 1 : 0 ;		// S005 (���: 0	��Ʈ: 5) 
				else if(i == 6) n_out[nIO_Num + 6]		= (wData & DBIT6)   ? 1 : 0 ;		// S006 (���: 0	��Ʈ: 6) 
				else if(i == 7) n_out[nIO_Num + 7]		= (wData & DBIT7)   ? 1 : 0 ;		// S007 (���: 0	��Ʈ: 7) 
				else if(i == 8) n_out[nIO_Num + 8]		= (wData & DBIT8)   ? 1 : 0 ;		// S008 (���: 0	��Ʈ: 8) 
				else if(i == 9) n_out[nIO_Num + 9]		= (wData & DBIT9)   ? 1 : 0 ;		// S009 (���: 0	��Ʈ: 9) 
				else if(i == 10) n_out[nIO_Num + 10]	= (wData & DBIT10)  ? 1 : 0 ;		// S010 (���: 0	��Ʈ: 10) 
				else if(i == 11) n_out[nIO_Num + 11]	= (wData & DBIT11)  ? 1 : 0 ;		// S011 (���: 0	��Ʈ: 11) 
				else if(i == 12) n_out[nIO_Num + 12]	= (wData & DBIT12)  ? 1 : 0 ;		// S012 (���: 0	��Ʈ: 12) 
				else if(i == 13) n_out[nIO_Num + 13]	= (wData & DBIT13)  ? 1 : 0 ;		// S013 (���: 0	��Ʈ: 13) 
				else if(i == 14) n_out[nIO_Num + 14]	= (wData & DBIT14)  ? 1 : 0 ;		// S014 (���: 0	��Ʈ: 14) 
				else if(i == 15) n_out[nIO_Num + 15]	= (wData & DBIT15)  ? 1 : 0 ;		// S015 (���: 0	��Ʈ: 15) 
//			}
		}
	}	

	if(n_rw_mode == 1 || n_rw_mode == 2) //input read mode
	{
		// ************************************************************************** //
		// �Է�(��� ��ȣ 00��)								                          //
		// ************************************************************************** //
		nRet = HSSI_ReadInputWord(n_bd_num, n_port_num, n_slave_num, &wData); //(iMasterID, iPortNo, iSlaveNo, &wData); 
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error

		n_in[nIO_Num + 0]	= (wData & DBIT0)   ? 1 : 0 ;		// PS000 (���: 0	��Ʈ: 0) 
		n_in[nIO_Num + 1]	= (wData & DBIT1)   ? 1 : 0 ;		// PS001 (���: 0	��Ʈ: 1) 
		n_in[nIO_Num + 2]	= (wData & DBIT2)   ? 1 : 0 ;		// PS002 (���: 0	��Ʈ: 2) 
		n_in[nIO_Num + 3]	= (wData & DBIT3)   ? 1 : 0 ;		// PS003 (���: 0	��Ʈ: 3) 
		n_in[nIO_Num + 4]	= (wData & DBIT4)   ? 1 : 0 ;		// PS004 (���: 0	��Ʈ: 4)
		n_in[nIO_Num + 5]	= (wData & DBIT5)   ? 1 : 0 ;		// PS005 (���: 0	��Ʈ: 5) 
		n_in[nIO_Num + 6]	= (wData & DBIT6)   ? 1 : 0 ;		// PS006 (���: 0	��Ʈ: 6) 
		n_in[nIO_Num + 7]	= (wData & DBIT7)   ? 1 : 0 ;		// PS007 (���: 0	��Ʈ: 7) 
		n_in[nIO_Num + 8]	= (wData & DBIT8)   ? 1 : 0 ;		// PS008 (���: 0	��Ʈ: 8) 
		n_in[nIO_Num + 9]	= (wData & DBIT9)   ? 1 : 0 ;		// PS009 (���: 0	��Ʈ: 9) 
		n_in[nIO_Num + 10]	= (wData & DBIT10)  ? 1 : 0 ;		// PS010 (���: 0	��Ʈ: 10) 
		n_in[nIO_Num + 11]	= (wData & DBIT11)  ? 1 : 0 ;		// PS011 (���: 0	��Ʈ: 11) 
		n_in[nIO_Num + 12]	= (wData & DBIT12)  ? 1 : 0 ;		// PS012 (���: 0	��Ʈ: 12) 
		n_in[nIO_Num + 13]	= (wData & DBIT13)  ? 1 : 0 ;		// PS013 (���: 0	��Ʈ: 13) 
		n_in[nIO_Num + 14]	= (wData & DBIT14)  ? 1 : 0 ;		// PS014 (���: 0	��Ʈ: 14) 
		n_in[nIO_Num + 15]	= (wData & DBIT15)  ? 1 : 0 ;		// PS015 (���: 0	��Ʈ: 15) 
	}

	if(n_rw_mode == 1 || n_rw_mode == 2) //output mode
	{
		// ************************************************************************** //
		// �Է�(��� ��ȣ 00��)								                          //
		// ************************************************************************** //
		wData = 0;
		for(i=0; i<16; i++)//io port 16��
		{
			if(n_out[nIO_Num + i] == 0 || n_out[nIO_Num + i] == 1) //0:off, 1:on
			{
				if     (i == 0)  wData += n_out[nIO_Num + i]		* DBIT0;  // S000 (���: 0	��Ʈ: 0)
				else if(i == 1)  wData += n_out[nIO_Num + i]		* DBIT1;  // S001 (���: 0	��Ʈ: 1)
				else if(i == 2)  wData += n_out[nIO_Num + i]		* DBIT2;  // S002 (���: 0	��Ʈ: 2)
				else if(i == 3)  wData += n_out[nIO_Num + i]		* DBIT3;  // S003 (���: 0	��Ʈ: 3)
				else if(i == 4)  wData += n_out[nIO_Num + i]		* DBIT4;  // S004 (���: 0	��Ʈ: 4)
				else if(i == 5)  wData += n_out[nIO_Num + i]		* DBIT5;  // S005 (���: 0	��Ʈ: 5)
				else if(i == 6)  wData += n_out[nIO_Num + i]		* DBIT6;  // S006 (���: 0	��Ʈ: 6)
				else if(i == 7)  wData += n_out[nIO_Num + i]		* DBIT7;  // S007 (���: 0	��Ʈ: 7)
				else if(i == 8)  wData += n_out[nIO_Num + i]		* DBIT8;  // S008 (���: 0	��Ʈ: 8)
				else if(i == 9)  wData += n_out[nIO_Num + i]		* DBIT9;  // S009 (���: 0	��Ʈ: 9)
				else if(i == 10) wData += n_out[nIO_Num + i]		* DBIT10; // S010 (���: 0	��Ʈ: 10)
				else if(i == 11) wData += n_out[nIO_Num + i]		* DBIT11; // S011 (���: 0	��Ʈ: 11)
				else if(i == 12) wData += n_out[nIO_Num + i]		* DBIT12; // S012 (���: 0	��Ʈ: 12)
				else if(i == 13) wData += n_out[nIO_Num + i]		* DBIT13; // S013 (���: 0	��Ʈ: 13)
				else if(i == 14) wData += n_out[nIO_Num + i]		* DBIT14; // S014 (���: 0	��Ʈ: 14)
				else if(i == 15) wData += n_out[nIO_Num + i]		* DBIT15; // S015 (���: 0	��Ʈ: 15)
			}
		}

		nRet = HSSI_WriteOutputWord(n_bd_num, n_port_num, n_slave_num, wData);//(iMasterID, iPortNo, iSlaveNo, wData); //TRUE = enable, FALSE = Dasible
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error 
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//2011.1019 james lee WORD ������ I/O�� �����Ѵ�
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





// ******************************************************************************
// I/O ���� ���̺귯�� �ʱ�ȭ �Լ�                                                   
// ******************************************************************************
// ******************************************************************************
// I/O ���� ���� �Լ�                                                            
// ******************************************************************************
int CFastechPublic_IO::Set_IO_BoardOpen(INT* pn_MasterNo, int n_OpenMethod) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterNo=0;

	nRet = HSSI_Open(&nMasterNo, n_OpenMethod);  // HSSI �ʱ�ȭ �Լ� 
	if (nRet == STATUS_SUCCESS)
	{
		*pn_MasterNo = nMasterNo;
	}
	else
	{
 		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// I/O ���� ���� �Լ�                                                            
// ******************************************************************************
int CFastechPublic_IO::Set_IO_BoardClose(bool b_CloseMethod) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_Close(b_CloseMethod);  // HSSI ���ҽ� ���� �Լ� 
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
 		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� �Լ� [WORD ����]                                 
// ******************************************************************************
int CFastechPublic_IO::Set_IO_DefineWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_WriteDirectionWord(n_MasterID, n_PortNo, n_SlaveNo, w_Data);  // SLAVE I/O ����� ����� ���� �Լ� [WORD ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� �Լ� [BYTE ����]                                 
// ******************************************************************************
int CFastechPublic_IO::Set_IO_DefineByte(int n_MasterID, int n_PortNo, int n_SlaveNo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_WriteDirectionByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, bt_Data);  // SLAVE I/O ����� ����� ���� �Լ� [BYTE ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� �Լ� [BIT ����]                                  
// ******************************************************************************
int CFastechPublic_IO::Set_IO_DefineBit(int n_IONo, BOOL b_Data) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_WriteDirectionBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_Data);  // SLAVE I/O ����� ����� ���� �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� ���� ���� [WORD ����]                            
// ******************************************************************************
int CFastechPublic_IO::Get_IO_DefineWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD* pw_Data) 
{
	CString sMsg;
	int     nRet=0;
	WORD    wData=0;

	nRet = HSSI_ReadDirectionWord(n_MasterID, n_PortNo, n_SlaveNo, &wData);  // SLAVE I/O ����� ����� ���� ���� ���� [WORD ����]
	if (nRet == STATUS_SUCCESS)
	{
		*pw_Data = wData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� ���� ���� [BYTE ����]                            
// ******************************************************************************
int CFastechPublic_IO::Get_IO_DefineByte(int n_MasterID, INT n_PortNo, int n_SlaveNo, int n_Type, BYTE* pb_Data) 
{
	CString sMsg;
	int     nRet=0;
	BYTE    bData=0;

	nRet = HSSI_ReadDirectionByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, &bData);  // SLAVE I/O ����� ����� ���� ���� ���� [BYTE ����]
	if (nRet == STATUS_SUCCESS)
	{
		*pb_Data = bData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� ����� ���� ���� ���� [BIT ����]                             
// ******************************************************************************
int CFastechPublic_IO::Get_IO_DefineBit(int n_IONo, BOOL* pb_Data) 
{
	CString sMsg;
	int     nRet=0;
	BOOL    bData;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadDirectionBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ����� ����� ���� ���� ���� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
		*pb_Data = bData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PC�� ����� ������ ���� ���� �˻� �Լ�                                        
// ******************************************************************************
int CFastechPublic_IO::Search_IO_Master(INT* pn_MasterNo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterNo=0;

	nRet = HSSI_GetAttachedMasterBoard(&nMasterNo);  // ���� PC�� ����Ǿ� �ִ� ������ ���� ���� ���� �Լ�
	if (nRet == STATUS_SUCCESS)
	{
		*pn_MasterNo = nMasterNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�  
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}

	return nRet;
}
// ******************************************************************************


// ******************************************************************************
// ������ ���忡 ����� PORT ���� �˻� �Լ�                                      
// ******************************************************************************
int CFastechPublic_IO::Search_IO_Port(int n_MasterID, INT* pn_PortNo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nPortNo=0;

	nRet = HSSI_GetAttachedPort(n_MasterID, &nPortNo);  // ������ ������ ���� �����ϴ� PORT ���� ���� �Լ�
	if (nRet == STATUS_SUCCESS)
	{
		*pn_PortNo = nPortNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT�� ����� SLAVE �˻� �Լ�                                                 
// ******************************************************************************
int CFastechPublic_IO::Search_IO_Slave(int n_MasterID, int n_PortNo, INT* pn_SlaveNo) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nSlaveNo=0;

	nRet = HSSI_GetAttachedSlaveModule(n_MasterID, n_PortNo, &nSlaveNo);  // PORT�� ����� SLAVE I/O ��� �˻� �Լ�
	if (nRet == STATUS_SUCCESS)
	{
		*pn_SlaveNo = nSlaveNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O MODULE�� ENABLE ���·� ������ ���忡 ����Ǿ� �ִ��� �˻� �Լ�      
// ******************************************************************************
int CFastechPublic_IO::Check_IO_SlaveStatus(int n_MasterID, int n_PortNo, int n_SlaveNo) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_IsExistSlaveModule(n_MasterID, n_PortNo, n_SlaveNo);  // SLAVE I/O MODULE�� ENABLE ���·� ������ ���忡 ����Ǿ� �ִ��� �˻� �Լ�
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT�� ����� SLAVE [ENABLE/DISABLE] ���� �˻� �Լ�                           
// ******************************************************************************
BOOL CFastechPublic_IO::Check_IO_Slave(int n_MasterID, int n_PortNo, int n_SlaveID) 
{
	int nSlaveInfo;

	FAS_STATUS  fasResult;

	// **************************************************************************
	// nSlaveInfo : �������κ��� ���� ��Ʈ �������� 8���� BIT���� �� SLAVE����   
	//              ���� ���°� ����� BCD�� ����ִ�                            
	// -> BIT 1   : SLAVE I/O ����� ����Ǿ� ENABLE�� ����                      
	// -> BIT 0   : SLAVE I/O ����� ������� �ʾ� DISABLE�� ����                
	// **************************************************************************
	fasResult = HSSI_GetAttachedSlaveModule(n_MasterID, n_PortNo, &nSlaveInfo);  // PORT�� ����� SLAVE I/O ��� �˻� �Լ�
	if (((nSlaveInfo >> n_SlaveID) & 0x1) != 1)
	{
		return BD_ERROR;
	}
	// **************************************************************************

	return BD_GOOD;
}
// ******************************************************************************

// ******************************************************************************
// PORT�� SLAVE I/O ������ ��� �ӵ� ���� �Լ�                                 
// -> PORT_SPEED_05 : 0.5 Mbps                                                   
// -> PORT_SPEED_10 : 1.0 Mbps                                                   
// -> PORT_SPEED_20 : 2.0 Mbps                                                   
// ******************************************************************************
int CFastechPublic_IO::Set_IO_HSSISpeed(int n_MasterID, int n_PortNo, int n_Type) 
{
	CString sMsg;
	int     nRet=0, nFlag = BD_ERROR;

	nRet = HSSI_SetCommSpeed(n_MasterID, n_PortNo, n_Type);  // �ش� PORT�� I/O ��� ������ ��� �ӵ� ���� �Լ�
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT�� SLAVE I/O ������ ��� �ӵ� ���� �Լ�                                 
// -> PORT_SPEED_05 : 0.5 Mbps                                                   
// -> PORT_SPEED_10 : 1.0 Mbps                                                   
// -> PORT_SPEED_20 : 2.0 Mbps                                                   
// ******************************************************************************
int CFastechPublic_IO::Get_IO_HSSISpeed(int n_MasterID, int n_PortNo, INT* pn_Type) 
{
	CString sMsg;
	int     nRet=0;
	INT     nType;

	nRet = HSSI_GetCommSpeed(n_MasterID, n_PortNo, &nType);  // �ش� PORT�� I/O ��� ������ ��� �ӵ� ���� �Լ�
	if (nRet == STATUS_SUCCESS)
	{
		*pn_Type = nType;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// ���� ���� ���� �˻� �Լ�                                                      
// ******************************************************************************
int CFastechPublic_IO::Check_IO_BoardExist(int n_MaxSlot, int n_MaxPort, int n_MaxSlave) 
{
	int nMasterID, nPortNo, nSlaveID;
	int nRet=0;

	while(1)
	{
		for(nMasterID=0; nMasterID<n_MaxSlot; nMasterID++)
		{
			nRet = Check_IO_MasterStatus(nMasterID);  // ������ ���� ���� ���� �˻� �Լ�
			if (nRet != BD_GOOD)
			{
				continue ;
			}

			for(nPortNo=0; nPortNo<n_MaxPort; nPortNo)
			{
				nRet = Check_IO_PortStatus(nMasterID, nPortNo);  // PORT ���� ���� �� ���� ���� �˻� �Լ�
				if (nRet != BD_GOOD)
				{
					continue ;
				}

				for(nSlaveID=0; nSlaveID<n_MaxSlave; nSlaveID++)
				{
					nRet = Check_IO_SlaveStatus(nMasterID, nPortNo, nSlaveID);  // SLAVE I/O MODULE�� ENABLE ���·� ������ ���忡 ����Ǿ� �ִ��� �˻� �Լ�
					if (nRet != BD_GOOD)
					{
						continue ;
					}
				}
			}
		}
	}

	return 0;
}
// ******************************************************************************

// ******************************************************************************
// PORT�� ����� ��� SLAVE I/O ��� �ڵ� ENABLE �Լ�                            
// ******************************************************************************
int CFastechPublic_IO::Set_IO_PortAutoEnable(int n_MasterID, int n_PortNo) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_PortAutoEnable(n_MasterID, n_PortNo);  // �ش� PORT�� ����� ��� SLAVE I/O ��� �ڵ� ENABLE �Լ�
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� ���� ENABLE/DISABLE ���� �Լ�                                  
// ******************************************************************************
int CFastechPublic_IO::Set_IO_SlaveEnable(int n_MasterID, int n_PortNo, int n_SlaveNo, BOOL b_Enable) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_EnableSlaveModule(n_MasterID, n_PortNo, n_SlaveNo, b_Enable);  // �ش� SLAVE I/O ��� ���� ENABLE/DISABLE ���� �Լ�
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************


// ******************************************************************************

// ******************************************************************************
// Ư�� ��ġ BIT�� �˻� �Լ�                                                     
// ******************************************************************************
BOOL CFastechPublic_IO::Check_Bit(long n_Value, int n_BitPos) 
{
	unsigned long nMask = 0x0001;  // ����ũ BIT ���� 
	nMask <<= n_BitPos;  // ������ BIT ��ġ�� �̵� [SHIFT] 

	return (n_Value & nMask);  // �ش� BIT ���� �� ���� 
}
// ******************************************************************************

// ******************************************************************************
// Ư�� ��ġ BIT ���� �Լ�                                                       
// ******************************************************************************
void CFastechPublic_IO::Set_Bit(long &n_Value, int n_Bit, BOOL b_On) 
{
	unsigned long nMask = 0x0001;  // ����ũ BIT ���� 
	nMask <<= n_Bit;  // ������ BIT ��ġ�� �̵� [SHIFT] 

	if ( b_On )
	{
		n_Value |= nMask;
	}
	else
	{
		n_Value &= ~nMask;
	}
}
// ******************************************************************************

// ******************************************************************************
// HSSI ���� ������ Ȯ�� �� �˻� �Լ�                                            
// -> �Է� �Ķ���ʹ� �ϵ��� �������� �ԷµǾ�� �Ѵ�                            
//  : I/O MAP ���� �����Ѵ�                                                      
//  : I/O MAP ������ 4�ڸ��� �����ȴ�                                            
// ******************************************************************************
int CFastechPublic_IO::Search_IO_BoardInformation(int n_Type, int n_Data) 
{
	int nFlag     = BD_ERROR;
	int nBardData = 0;

	// **************************************************************************
	// I/O PORT�� ���� �ִ� ������� 7127�� �����Ѵ�                             
	// -> �ִ� ��� : ������ ���� 2���� ���� [I/O PORT�� 2048]                   
	// **************************************************************************
	if (n_Data > HSSI_MAX_IO) //HSSI_MAX_IO = 7715
		return BD_ERROR;
	// **************************************************************************

	switch(n_Type)
	{
	case 0 :  // ������ ���� 
		// **********************************************************************
		// 0[0, 1, 2, 3]    1[4, 5, 6, 7]    2[8, 9, 10, 11]    3[12, 13, 14, 15]
		// -> ������ ���� 1�忡 �ִ� 4 PORT ����ȴ�                             
		// **********************************************************************
		nBardData = n_Data / 4000;
		// **********************************************************************
		break;

	case 1 :  // PORT 
		// **********************************************************************
		// I/O MAP ��ȣ�� 1000���� ������ ���� PORT ��ȣ�� �ǵ��� ���ǵǾ� �ִ�  
		// -> PORT ���� ������ �ִ� [0 - 15] �̴�                                
		// -> ������ ����� �ִ� 2������ �����Ѵ�                                
		// **********************************************************************
		nBardData = n_Data / 1000;

		if (nBardData>3)
			nBardData -= 4;
		// **********************************************************************
		break;

	case 2 :  // SLAVE 
		// **********************************************************************
		// �� PORT�� ����� SLAVE ��ȣ�� �����Ѵ�                                
		// -> PORT �� �ִ� [0 - 7] ����ȴ�                                      
		// **********************************************************************
		nBardData = n_Data     % 1000;
		nBardData = nBardData / 100;
		// **********************************************************************
		break;

	case 3 :  // I/O BIT 
		// **********************************************************************
		// �ش� I/O BIT ��ġ �����Ѵ�                                            
		// -> I/O BIT�� �ִ� [0 - 15] ������ �� �ִ�                             
		//  : IN/OUT ���´� ���Ƿ� ���� �����Ѵ�                                 
		// **********************************************************************
		nBardData = n_Data     % 1000;
		nBardData = nBardData % 100;
		// **********************************************************************
		break;

	default :
		return BD_ERROR;
		break;
	}

	return nBardData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [WORD ����]                    
// ******************************************************************************
int CFastechPublic_IO::Set_IO_LatchWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_SetLatchWord(n_MasterID, n_PortNo, n_SlaveNo, w_Data);  // SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [WORD ����]
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [BYTE ����]                    
// ******************************************************************************
int CFastechPublic_IO::Set_IO_LatchByte(int n_MasterID, int n_PortNo, int n_SlaveNo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_SetLatchByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, bt_Data);  // SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [BYTE ����]
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [BIT ����]                     
// ******************************************************************************
int CFastechPublic_IO::Set_IO_LatchBit(int n_IONo, BOOL b_Data) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_SetLatchBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_Data);  // SLAVE I/O ����� �Է��� LATCH ���� ���� �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [WORD ����]                             
// ******************************************************************************
WORD CFastechPublic_IO::Get_IO_LatchWord(int n_MasterID, int n_PortNo, int n_SlaveNo) 
{
	CString sMsg;
	int     nRet=0;
	
	WORD    wData=0;

	nRet = HSSI_ReadLatchWord(n_MasterID, n_PortNo, n_SlaveNo, &wData);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [WORD ����]
	if (nRet == STATUS_SUCCESS)
	{
		//wData�� ������ ���� //wData = BD_GOOD;
	}
	else
	{
		wData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return wData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [BYTE ����]                             
// ******************************************************************************
BYTE CFastechPublic_IO::Get_IO_LatchByte(int n_MasterID, INT n_PortNo, int n_SlaveNo, int n_Type) 
{
	CString sMsg;
	int     nRet=0;	
	BYTE    bData=0;

	nRet = HSSI_ReadLatchByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, &bData);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [BYTE ����]
	if (nRet == STATUS_SUCCESS)
	{	 
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return bData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [BIT ����]                              
// ******************************************************************************
BOOL CFastechPublic_IO::Get_IO_LatchBit(int n_IONo) 
{
	CString sMsg;
	int     nRet=0;
	BOOL    bData=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadLatchBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return bData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [WORD ����]                             
// ******************************************************************************
int CFastechPublic_IO::Reset_IO_LatchWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_ResetLatchWord(n_MasterID, n_PortNo, n_SlaveNo, w_Data);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [WORD ����]
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [BYTE ����]                             
// ******************************************************************************
int CFastechPublic_IO::Reset_IO_LatchByte(int n_MasterID, int n_PortNo, int n_SlaveNo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_ResetLatchByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, bt_Data);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [BYTE ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ����� LATCH ���� ���� �Լ� [BIT ����]                              
// ******************************************************************************
int CFastechPublic_IO::Reset_IO_LatchBit(int n_IONo, BOOL b_Data) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_SetLatchBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_Data);  // SLAVE I/O ����� LATCH ���� ���� �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ� 
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// ������ ���� ���� ���� �˻� �Լ�                                               
// ******************************************************************************
int CFastechPublic_IO::Check_IO_MasterStatus(INT n_MasterID) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_IsExistMasterBoard(n_MasterID);  // ������ ���� ���� ���� �˻� �Լ�
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT ���� ���� �� ���� ���� �˻� �Լ�                                         
// ******************************************************************************
int CFastechPublic_IO::Check_IO_PortStatus(int n_MasterID, int n_PortNo) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_IsExistPort(n_MasterID, n_PortNo);  // PORT ���� ���� �� ���� ���� �˻� �Լ�
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//I/O�� ���� �����Ѵ�  james Lee 2008.9.5
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [WORD ����]                                
// ******************************************************************************
int CFastechPublic_IO::Set_Out_Word(int n_IONo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_WriteOutputWord(nMasterID, nPortNo, nSlaveNo, w_Data);  // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [WORD ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BYTE ����]                                
// ******************************************************************************
int CFastechPublic_IO::Set_Out_Byte(int n_IONo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_WriteOutputByte(nMasterID, nPortNo, nSlaveNo, n_Type, bt_Data);  // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BYTE ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BIT ����]                                 
// ******************************************************************************
int CFastechPublic_IO::Set_Out_Bit(int n_IONo, BOOL b_OnOff) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_WriteOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_OnOff);  // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� �Է� ��ȣ READ �Լ� [WORD ����]                                
// ******************************************************************************
WORD CFastechPublic_IO::Get_In_Word(int n_IONo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	WORD    wData=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_ReadInputWord(nMasterID, nPortNo, nSlaveNo, &wData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [WORD ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		wData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return wData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ READ �Լ� [WORD ����]                                
// ******************************************************************************
WORD CFastechPublic_IO::Get_Out_Word(int n_IONo)
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	WORD    wData=0;
	
	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);
	
	nRet = HSSI_ReadOutputWord(nMasterID, nPortNo, nSlaveNo, &wData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [WORD ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		wData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return wData;	
}

// ******************************************************************************
// SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BYTE ����]                                
// ******************************************************************************
BYTE CFastechPublic_IO::Get_In_Byte(int n_IONo, int n_Type) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	BYTE    btData=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_ReadInputByte(nMasterID, nPortNo, nSlaveNo, n_Type, &btData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BYTE ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		btData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return btData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ READ �Լ� [BYTE ����]                                
// ******************************************************************************
BYTE CFastechPublic_IO::Get_Out_Byte(int n_IONo, int n_Type)
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	BYTE    btData=0;
	
	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);
	
	nRet = HSSI_ReadOutputByte(nMasterID, nPortNo, nSlaveNo, n_Type, &btData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BYTE ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		btData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return btData;
}

// ******************************************************************************
// SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BIT ����]                                 
// ******************************************************************************
BOOL CFastechPublic_IO::Get_In_Bit(int n_IONo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo = 0;
	BOOL    bData;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadInputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return bData;
}




// ******************************************************************************
// SLAVE I/O ��� ��� ��ȣ READ �Լ� [BIT ����]                                 
// ******************************************************************************
BOOL CFastechPublic_IO::Get_Out_Bit(int n_IONo)
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0,nBitNo=0;
	BOOL    bData;
	
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI ���� ������ Ȯ�� �� �˻� �Լ�
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);
	
	nRet = HSSI_ReadOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BIT ����]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return bData;
}
// ******************************************************************************

// ******************************************************************************
// ���� I/O ���� ��� �Լ� [�ִ� 8 PORT ���� ����]                               
// -> int nSite     : ����� I/O PORT ����                                       
//  : ���Ŀ� I/O PORT ��� ���� ���� �˻� �� �ʿ��ϴ�                            
// -> int nModuleNo : ����� I/O PORT ���� ��� ��ȣ                             
// -> int nOffset   : I/O BIT TYPE                                               
//  : TYPE_LBYTE [0x2] : 16 BIT �� ���� 8BIT �о� �����Ѵ�                       
//  : TYPE_HBYTE [0x3] : 16 BIT �� ���� 8BIT �о� �����Ѵ�                       
// -> int *npBit    : ���� ������ I/O PORT ��ȣ                                  
// -> int *npOnOff  : ��� I/O PORT ���� [0:OFF    1:ON]                         
// -> int nBitCount : ���� ������ I/O PORT ����                                  
//  : [npBit/npOnOff] �迭 ũ�����ٴ� �۴��� ���� ���� ���̴�                    
// ******************************************************************************
int CFastechPublic_IO::Set_IO_Multi_Action(int nSite, int nModuleNo, int nOffset,
										int *npBit, int *npOnOff, int nBitCount, int n_onoff) 
{
	int  i;
	int  nType;     // ���� I/O BIT Ÿ�� ���� ���� 
	int  iPortMap;  // I/O PORT MAP ������ ���� ���� 
	int n_val_1 = 0, n_val_2 = 0;
	long lOutData;  // ��� ��Ʈ ������ ���� ���� 

	if (nBitCount > 8)  // �ִ� BIT ���� 
	{
		return BD_ERROR;
	}

	// **************************************************************************
	// OUT PORT ��� ���� ���� �߰��Ѵ�                                          
	// **************************************************************************
	
	// **************************************************************************

	if (nOffset == 0)
	{
		nType = TYPE_LBYTE;
	}
	else
	{
		nType = TYPE_HBYTE;
	}

	n_val_1 = Get_In_Byte(nModuleNo, nType);       // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BYTE ����]

	if(n_onoff == IO_ON)						// On ��Ű�� �����϶�...
	{
		for(i = 0; i < nBitCount; i++)
		{
			if(npOnOff[i] == YES)
			{
				n_val_2 += (int)pow(2, i);
			}
		}
		lOutData = n_val_1 | n_val_2;
	}
	else										// Off ��Ű�� �����϶�...
	{
		for (i = 0; i < nBitCount; i++)
		{
			if(npOnOff[i] == NO)
			{
				n_val_2 += (int)pow(2, i);
			}
		}

		lOutData = n_val_1 & n_val_2;
	}

	Set_Out_Byte(nModuleNo, nType, (int)lOutData);  // SLAVE I/O ��� ��� ��ȣ �߻� �Լ� [BYTE ����]

	// **************************************************************************
	// -> ��Ƽ PORT �� ù��° PORT MAP�� �迭 ������ ����Ѵ�                    
	//  : ������ PORT�� ���ÿ� 2������ ���۽�Ű�� ���� ���� ������ Ÿ�̸� ���� 
	//    ������ ���̴�                                                          
	//  : �׷��Ƿ� �迭 ũ��� I/O PORT �� ���� ��ȣ�� ���� MAP ��ȣ����         
	//    ũ�ų� ���ƾ� �Ѵ�                                                     
	// **************************************************************************
	iPortMap = (nModuleNo * 100) + npBit[0];  // ù��° I/O PORT�� MAP ������ ���� 

	lCylinderTime[iPortMap][0] = GetCurrentTime();
	bCylinderFlag[iPortMap] = true;
	// **************************************************************************

	return BD_GOOD;
}
// ******************************************************************************

// ******************************************************************************
// ���� I/O ���� �˻� �Լ� [�ִ� 8 PORT ���� ����]                               
// -> int nSite      : ����� I/O PORT ����                                      
//  : ���Ŀ� I/O PORT ��� ���� ���� �˻� �� �ʿ��Ѵ�                            
// -> int nModuleNo  : ����� I/O PORT ���� ��� ��ȣ                            
// -> int nOffset    : I/O BIT TYPE                                              
//  : TYPE_LBYTE [0x2] : 16 BIT �� ���� 8BIT �о� �����Ѵ�                       
//  : TYPE_HBYTE [0x3] : 16 BIT �� ���� 8BIT �о� �����Ѵ�                       
// -> int *npBit     : ���� ������ I/O PORT ��ȣ                                 
// -> int *npOnOff   : ��� I/O PORT ���� [0:OFF    1:ON]                        
// -> int nBitCount  : ���� ������ I/O PORT ����                                 
//  : [npBit/npOnOff] �迭 ũ�����ٴ� �۴��� ���� ���� ���̴�                    
// -> int nWaitTime  : OUT PORT ���� �� ����� ��� �ð�                         
// -> int nLimitTime : OUT PORT ���� �Ϸ� �˻� ����Ʈ �ð�                       
// ******************************************************************************
int CFastechPublic_IO::Get_IO_Multi_ActionCheck(int nSite, int nModuleNo, int nOffset,
											 int *npBit, int *npOnOff, int nBitCount, int nWaitTime, int nLimitTime) 
{
	int  i;
	int  nType;     // ���� I/O BIT Ÿ�� ���� ���� 
	int  iCount=0;  // �˻��� PORT ���� 
	int  iState;    // PORT ���� ���� ���� 
	int  iPortMap;  // I/O PORT MAP ������ ���� ���� 
	int  iFuncRet = BD_PROCEED;
	long lInData;   // ��Ʈ ������ ���� ���� 

	if (nBitCount>8)  // �ִ� BIT ���� 
	{
		return BD_ERROR;
	}

	// **************************************************************************
	// OUT PORT ��� ���� ���� �߰��Ѵ�                                          
	// **************************************************************************
	
	// **************************************************************************

	if (nOffset == 0)
	{
		nType = TYPE_LBYTE;
	}
	else
	{
		nType = TYPE_HBYTE;
	}

	lInData = Get_In_Byte(nModuleNo, nType);  // SLAVE I/O ��� �Է� ��ȣ READ �Լ� [BYTE ����]

	for(i=0; i<nBitCount; i++)
	{
		iState = (int)Check_Bit(lInData, npBit[i]);
		if (iState == npOnOff[i])
		{
			iCount++ ;
		}
	}

	// **************************************************************************
	// -> ��Ƽ PORT �� ù��° PORT MAP�� �迭 ������ ����Ѵ�                    
	//  : ������ PORT�� ���ÿ� 2������ ���۽�Ű�� ���� ���� ������ Ÿ�̸� ���� 
	//    ������ ���̴�                                                          
	//  : �׷��Ƿ� �迭 ũ��� I/O PORT �� ���� ��ȣ�� ���� MAP ��ȣ����         
	//    ũ�ų� ���ƾ� �Ѵ�                                                     
	// **************************************************************************
	iPortMap = (nModuleNo * 100) + npBit[0];  // ù��° I/O PORT�� MAP ������ ���� 

	if ((bCylinderFlag[iPortMap] == true) &&
		(iCount == nBitCount))
	{
		bCylinderFlag[iPortMap] = false;
		lCylinderTime[iPortMap][0] = GetCurrentTime();
	}
	else if ((bCylinderFlag[iPortMap] == false) &&
		(iCount == nBitCount))
	{
		lCylinderTime[iPortMap][1] = GetCurrentTime();
		lCylinderTime[iPortMap][2] = lCylinderTime[iPortMap][1] - lCylinderTime[iPortMap][0];
		if (lCylinderTime[iPortMap][2] > nWaitTime)
		{
			iFuncRet = BD_GOOD;
		}
		else if (lCylinderTime[iPortMap][2] < 0)
		{
			lCylinderTime[iPortMap][0] = GetCurrentTime();
		}
	}
	else
	{
		lCylinderTime[iPortMap][1] = GetCurrentTime();
		lCylinderTime[iPortMap][2] = lCylinderTime[iPortMap][1] - lCylinderTime[iPortMap][0];
		if (lCylinderTime[iPortMap][2] > nLimitTime)
		{
			iFuncRet = BD_ERROR;
		}
		else if (lCylinderTime[iPortMap][2] < 0)
		{
			lCylinderTime[iPortMap][0] = GetCurrentTime();
		}
	}
	// **************************************************************************

	return iFuncRet;
}
// ******************************************************************************


// ******************************************************************************
// HSSI DLL �Լ� ȣ�� ���� ���� �Ľ� �Լ�                                        
// -> nErrorNum                : �߻��� ���� ��ȣ                                
// -> STATUS_SUCCESS = 0x0     : ���� ����                                       
// -> STATUS_ERROR_NO_DEVICE   : ���� ���� ���� ������                           
// -> STATUS_ERROR_READ        : READ ���� ����                                  
// -> STATUS_ERROR_WRITE       : WRITE ���� ����                                 
// -> STATUS_ERROR_INITIALIZE  : �ʱ�ȭ ���� [RESET ����]                        
// -> STATUS_ERROR_COMMAND     : CMD ��뿡 ���� �߻�                            
// -> STATUS_ERROR_FILE        : FILE�� ���� �߻�                                
// -> STATUS_ERROR_PORT        : PORT ������                                     
// -> STATUS_ERROR_SLAVE       : SLAVE�� ON �ƴ�                                 
// -> STATUS_NOTUSE_COMMAND    : �������� �ʴ� ��ɾ�                            
// -> STATUS_INVALID_BOARDID   : �ش� ���� ������                                
// -> STATUS_INVALID_PORTNO    : �ش� PORT ������                                
// -> STATUS_INVALID_SLAVEID   : �ش� SLAVE ������                               
// -> STATUS_INVALID_PARAMETER : �Ķ���� �Է� ����                              
// -> STATUS_INVALID_DLL       : �߸��� DLL ���                                 
// ******************************************************************************
char* CFastechPublic_IO::ErrorOccurInfo(int nErrorNum) 
{

 	CString sErrorMsg;  // ���� �޽��� �ӽ� ���� ���� 
 
 	static char cMsg[60]={0,};

	if (nErrorNum == STATUS_ERROR_NO_DEVICE)
	{
		sErrorMsg = _T("���� ���� ���� ������");
	}
	else if (nErrorNum == STATUS_ERROR_READ)
	{
		sErrorMsg = _T("READ ���� ����");
	}
	else if (nErrorNum == STATUS_ERROR_WRITE)
	{
		sErrorMsg = _T("WRITE ���� ����");
	}
	else if (nErrorNum == STATUS_ERROR_INITIALIZE)
	{
		sErrorMsg = _T("�ʱ�ȭ ���� [RESET ����]");
	}
	else if (nErrorNum == STATUS_ERROR_COMMAND)
	{
		sErrorMsg = _T("CMD ��뿡 ���� �߻�");
	}
	else if (nErrorNum == STATUS_ERROR_FILE)
	{
		sErrorMsg = _T("FILE�� ���� �߻�");
	}
	else if (nErrorNum == STATUS_ERROR_PORT)
	{
		sErrorMsg = _T("PORT ������");
	}
	else if (nErrorNum == STATUS_ERROR_SLAVE)
	{
		sErrorMsg = _T("SLAVE�� ON �ƴ�");
	}
	else if (nErrorNum == STATUS_NOTUSE_COMMAND)
	{
		sErrorMsg = _T("�������� �ʴ� ��ɾ�");
	}
	else if (nErrorNum == STATUS_INVALID_BOARDID)
	{
		sErrorMsg = _T("�ش� ���� ������");
	}
	else if (nErrorNum == STATUS_INVALID_PORTNO)
	{
		sErrorMsg = _T("�ش� PORT ������");
	}
	else if (nErrorNum == STATUS_INVALID_SLAVEID)
	{
		sErrorMsg = _T("�ش� SLAVE ������");
	}
	else if (nErrorNum == STATUS_INVALID_PARAMETER)
	{
		sErrorMsg = _T("�Ķ���� �Է� ����");
	}
	else if (nErrorNum == STATUS_INVALID_DLL)
	{
		sErrorMsg = _T("�߸��� DLL ���");
	}
	else
	{
		sErrorMsg.Format(_T("���ǵ��� ���� HSSI ���� �ڵ� = %d"), nErrorNum);
	}

	strncpy(cMsg, sErrorMsg, sizeof(cMsg)-1);
	cMsg[sizeof(cMsg)-1] = '\0'; //2011.0518

	return cMsg;
}
// ******************************************************************************

/*
int CFastechPublic_IO::Check_IO_Port(int n_io, int n_time, int n_flag)
{
	int Ret = CTLBD_READY;	// �Լ� ���� �÷��� ���� 
	
	if (n_time == 0)	// ��� �ð����� �ٷ� ���ϰ��� �޾ƾ� �Ѵ�.
	{
		if (Get_IO_InportBit(n_io) == TRUE)
		{
			Ret = CTLBD_YES;
		}
		else	Ret = CTLBD_NO;
	}
	else				// ���� �ð����� Ȯ���ϰ�, ���ϰ��� �޴´�.
	{
		if (IOFlag[n_io] != true)
		{
			IOTime[n_io][0]	= GetCurrentTime();
			IOFlag[n_io] = true;
		}
		else if (Get_IO_InportBit(n_io) == TRUE && IOFlag[n_io] == true)
		{
			IOTime[n_io][1] = GetCurrentTime();
			IOTime[n_io][2] = IOTime[n_io][1] - IOTime[n_io][0];
			
			if (IOTime[n_io][2] >= n_time)
			{
				IOFlag[n_io] = false;
				Ret = CTLBD_YES;
			}
		}
		else if (Get_IO_InportBit(n_io) == FALSE && IOFlag[n_io] == true)
		{
			IOTime[n_io][1] = GetCurrentTime();
			IOTime[n_io][2] = IOTime[n_io][1] - IOTime[n_io][0];
			
			if (IOTime[n_io][2] >= n_time)
			{
				IOFlag[n_io] = false;
				Ret = CTLBD_NO;
			}
		}
	}
	
	return Ret;
}

int CFastechPublic_IO::Check_IO_Port_Yield(int n_io, int n_time, int n_yield)
{
	int Ret = CTLBD_READY;	// �Լ� ���� �÷��� ���� 
	
	if (IOYFlag[n_io] != true)
	{
		IOYTime[n_io][0] = GetCurrentTime();
		IOYFlag[n_io] = true;
		// Yield Check �ʱ�ȭ. 2K6/05/18/ViboX
		IOYCount[n_io][0] = 0;					// false
		IOYCount[n_io][1] = 0;					// true
		IOYCount[n_io][2] = 0;					// total
	}
	else if (Get_IO_InportBit(n_io) == TRUE && IOYFlag[n_io] == true)
	{
		IOYCount[n_io][1]++;
	}
	else if (Get_IO_InportBit(n_io) == FALSE && IOYFlag[n_io] == true)
	{
		IOYCount[n_io][0]++;
	}
	
	if (IOYFlag[n_io] == true)
	{
		IOYTime[n_io][1] = GetCurrentTime();
		IOYTime[n_io][2] = IOYTime[n_io][1] - IOYTime[n_io][0];
		
		if (IOYTime[n_io][2] >= n_time)
		{
			
			IOYCount[n_io][2] = IOYCount[n_io][1] + IOYCount[n_io][0];
			
			if (IOYCount[n_io][2] <= 0)
			{
				return CTLBD_NO;
			}
			IOYYield[n_io] = (float)IOYCount[n_io][1] / IOYCount[n_io][2];
			
			if (IOYYield[n_io] >= n_yield)
			{
				Ret = CTLBD_YES;
			}
			else
			{
				Ret = CTLBD_NO;
			}
			IOYFlag[n_io] = false;
		}
	}
	return Ret;
}

*/

// ******************************************************************************
//  �Ǹ��� ���۽�Ű��  �Լ� (�ܵ�)                                               
// ******************************************************************************
void CFastechPublic_IO::OnCylinderAction(int n_cyl, int n_act)
{
	//Set_Out_Bit(n_cyl, n_act);
	set_out_bit(n_cyl, n_act);
	
	CYLTime[n_cyl][0] = GetCurrentTime();
	
	CYLFlag[n_cyl] = false;
}

// ******************************************************************************
//  �Ǹ��� ���۽�Ű��  �Լ� (������)                                             
// ******************************************************************************
void CFastechPublic_IO::OnCylinderAction(int n_cyl, int n_cyl2, int n_act)
{
	//Set_Out_Bit(n_cyl, n_act);
	//Set_Out_Bit(n_cyl2, !n_act);
	set_out_bit(n_cyl, n_act);
	set_out_bit(n_cyl2, !n_act);
	
	CYLTime[n_cyl][0] = GetCurrentTime();
	
	CYLFlag[n_cyl] = false;
}

// ******************************************************************************
//  �Ǹ��� ���� ��Ų�� Ȯ���ϴ� �Լ� (���� �ϳ���)                               
// ******************************************************************************
int CFastechPublic_IO::OnCylinderActionCheck(int n_cyl, int n_sen, int n_act, int n_wait, int n_limit)
{
	int Ret = RET_PROCEED;  // �Լ� ���� �÷��� ���� 
	
	if (get_in_bit(n_sen, n_act) == n_act && CYLFlag[n_cyl] == false)
	{
		CYLTime[n_cyl][0]	= GetCurrentTime();
		CYLFlag[n_cyl] = true;
	}
	else if (get_in_bit(n_sen, n_act) == n_act && CYLFlag[n_cyl] == true)
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if(CYLTime[n_cyl][2] < 0)
		{
			CYLTime[n_cyl][0]	= GetCurrentTime();
			return Ret;
		}

		if (CYLTime[n_cyl][2] >= n_wait)
		{
			Ret = RET_GOOD;
		}
	}
	else
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if(CYLTime[n_cyl][2] < 0)
		{
			CYLTime[n_cyl][0]	= GetCurrentTime();
			return Ret;
		}

		if (CYLTime[n_cyl][2] >= n_limit)
		{
			Ret = RET_ERROR;
		}
	}
	return Ret;
}

// ******************************************************************************
//  �Ǹ��� ���� ��Ų�� Ȯ���ϴ� �Լ� (���� �ΰ���)                               
// ******************************************************************************
int CFastechPublic_IO::OnCylinderActionCheck(int n_cyl, int n_sen1, int n_sen2, int n_act, int n_wait, int n_limit)
{
	int Ret = RET_PROCEED;  // �Լ� ���� �÷��� ���� 
	
	if (get_in_bit(n_sen1, n_act) == n_act && 
		get_in_bit(n_sen2, !n_act) == !n_act && 
		CYLFlag[n_cyl] == false)
	{
		CYLTime[n_cyl][0]	= GetCurrentTime();
		CYLFlag[n_cyl] = true;
	}
	else if (get_in_bit(n_sen1, n_act) == n_act && 
		get_in_bit(n_sen2, !n_act) == !n_act && 
		CYLFlag[n_cyl] == true)
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if(CYLTime[n_cyl][2] < 0)
		{
			CYLTime[n_cyl][0]	= GetCurrentTime();
			return Ret;
		}

		if (CYLTime[n_cyl][2] >= n_wait)
		{
			Ret = RET_GOOD;
		}
	}
	else
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if(CYLTime[n_cyl][2] < 0)
		{
			CYLTime[n_cyl][0]	= GetCurrentTime();
			return Ret;
		}

		if (CYLTime[n_cyl][2] >= n_limit)
		{
			Ret = RET_ERROR;
		}
	}
	return Ret;
}


int CFastechPublic_IO::Reset_Safety_Area_OnOff(int nOnOff)
{
	int nRetFunc = RET_PROCEED;
	switch(SafetyAreaStep)
	{
	case 0:
//		FAS_IO.set_out_bit(st_io.o_mute_on_switch_lamp, nOnOff);
//		FAS_IO.set_out_bit(st_io.o_mute_off_switch_lamp, !nOnOff);
		ml_delaysafety[0] = GetCurrentTime();
		SafetyAreaStep = 100;		
		break;

	case 100:
		ml_delaysafety[1] = GetCurrentTime();
		ml_delaysafety[2] = ml_delaysafety[1] - ml_delaysafety[0];
		if(ml_delaysafety[2] > 500)
		{
//			if(FAS_IO.get_in_bit(st_io.i_mute_on_switch_chk, nOnOff) == nOnOff &&
//				FAS_IO.get_in_bit(st_io.i_mute_off_switch_chk, !nOnOff) == nOnOff)
			if(1)
			{
				SafetyAreaStep = 200;
			}
			else
			{
				n_retry++;
				if(n_retry > 2)
				{
					//900400 0 90 "Mute Switch ���� ����"
					alarm.mstr_pcode = _T("900400");
					alarm.mn_count_mode = 0;				// �˶� ī��Ʈ �÷��� ���� (0:�˶� ī��Ʈ ������    1:�˶� ī��Ʈ ����)
					alarm.mn_type_mode = eWARNING;		// �˶� ���� ���� (0:���    1:���༺    2:��� ����)
					SafetyAreaStep = 0;
					nRetFunc = RET_ERROR;
				}
				else
				{
					SafetyAreaStep = 0;
				}
			}
		}
		break;

	case 200:
		//FAS.Set_Out_Bit(st_io.o_safety_area_onoff, IO_OFF);
		ml_delaysafety[0] = GetCurrentTime();
		SafetyAreaStep = 300;		
		break;

	case 300:
		ml_delaysafety[1] = GetCurrentTime();
		ml_delaysafety[2] = ml_delaysafety[1] - ml_delaysafety[0];
		if(ml_delaysafety[2] > 500)
		{
			if(1)//FAS_IO.Get_Out_Bit(st_io.o_safety_area_onoff) == IO_ON)
			{
				SafetyAreaStep = 500;
			}
			else
			{
				n_retry++;
				if(n_retry > 5)
				{
					n_retry = 0;
					SafetyAreaStep = 0;
					//130201 0 13 "Unloader Elevator's Area Sensor timeout,(S1103 PS1108)"
					alarm.mstr_pcode = _T("130201");		// Alarm ȭ���� ����ش�. 2K5/03/10/ViboX
					alarm.mn_count_mode = 0;				// �˶� ī��Ʈ ���� �÷��� ���� (�˶� ī��Ʈ �۾� ������)
					alarm.mn_type_mode = eWARNING;		// ���� �߻��� �˶� ���� �÷��� ����
					//st_work.n_run_status = dWARNING;	// dJAM,dWARNING 

					alarm.mn_alarm_assign_section = 68;
					nRetFunc = RET_ERROR;
				}
				else
				{
					SafetyAreaStep = 200;
				}
			}
		}
		break;

	case 500:
		nRetFunc = RET_GOOD;
		SafetyAreaStep = 0;
		break;
	
	}
	return nRetFunc;
}