// MyBasicData.cpp: implementation of the CMyBasicData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "MyBasicData.h"
#include "io.h"
#include "math.h"
#include "ComizoaPublic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBasicData::CMyBasicData()
{

}

CMyBasicData::~CMyBasicData()
{

}

void CMyBasicData::OnPassWord_Load(CString str_load_level)
{
	CString str_pass;
	char chr_data[50];						// 암호는 25글자 이내로 제한

	(st_msg.mstr_password).Empty();		// 암호 저장 변수 초기화

	if (str_load_level == "Lock")			// SYSTEM LOCK 암호
		:: GetPrivateProfileString("Password", "SysLock", "M", chr_data, 50, st_path.mstr_basic);
	else if (str_load_level == "Level1")	// 메인트 암호
		:: GetPrivateProfileString("Password", "Level_1", "M", chr_data, 50, st_path.mstr_basic);
	else if (str_load_level == "Level2")	// 티칭 암호
		:: GetPrivateProfileString("Password", "Level_2", "T", chr_data, 50, st_path.mstr_basic);
	else  
		return;

	str_pass = chr_data;
	str_pass.MakeUpper();     // 문자열 대문자로 변경
	str_pass.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	str_pass.TrimRight(' ');  // 우측 문자열 공백 제거

	st_msg.mstr_password = str_pass;  // 암호 전역 변수에 설정
}

void CMyBasicData::OnPassWord_Save(CString str_save_level, CString str_pass_data)
{
	if (str_save_level=="Lock")         // SYSTEM LOCK 암호
		:: WritePrivateProfileString("Password", "SysLock", LPCTSTR(str_pass_data), st_path.mstr_basic);
	else if (str_save_level=="Level1")  // 메인트 암호
		:: WritePrivateProfileString("Password", "Level_1", LPCTSTR(str_pass_data), st_path.mstr_basic);
	else if (str_save_level=="Level2")  // 티칭 암호
		:: WritePrivateProfileString("Password", "Level_2", LPCTSTR(str_pass_data), st_path.mstr_basic);
}

CString CMyBasicData::OnStep_File_Index_Load()
{
	CString str_file_index;  // 파일 인덱스 저장 변수

	char chr_load[20];

	:: GetPrivateProfileString("Thread_Step_file", "File_Index", "00", chr_load, 20, st_path.mstr_basic);
	str_file_index = chr_load;
	str_file_index.TrimLeft(' ');               
	str_file_index.TrimRight(' ');

	return str_file_index;  // 파일 인덱스 리턴
}

void CMyBasicData::OnStep_File_Index_Save(CString str_index)
{
	:: WritePrivateProfileString("Thread_Step_file", "File_Index", LPCTSTR(str_index), st_path.mstr_basic);
}

CString CMyBasicData::OnStep_File_Name_Load()
{
	CString str_file_name;  // 파일명 저장 변수

	char chr_load[20];

	:: GetPrivateProfileString("Thread_Step_file", "File_Name", "DEFAULT", chr_load, 20, st_path.mstr_basic);
	str_file_name = chr_load;
	str_file_name.TrimLeft(' ');               
	str_file_name.TrimRight(' ');

	return str_file_name;  // 파일명 리턴
}

void CMyBasicData::OnStep_File_Name_Save(CString str_file)
{
	:: WritePrivateProfileString("Thread_Step_file", "File_Name", LPCTSTR(str_file), st_path.mstr_basic);
}

void CMyBasicData::OnMotorSpeed_Set_Data_Load()
{
	int  n_chk, i = 0, j = 0;
	CString str_temp, str_part;       // 임시 저장 변수
	CString str_motor_name;

	char chr_data[20];

	CString str_msg, str_LoadFile;       // 임시 저장 변수
	
	FILE    *fp ;
	int     existence;
	
	str_LoadFile = OnGet_Teach_File_Name("Speed");  // 데이터 저장 파일명 로딩 함수 
	
	existence = access(str_LoadFile, 0);
	
	if (!existence)
	{
		if ((fp=fopen(str_LoadFile,"rb")) == NULL)
		{
			if (st_handler.cwnd_list != NULL)
			{
//				st_msg.mstr_abnormal_msg.Format("[%s] file open error.", str_LoadFile);
				sprintf(st_msg.c_abnormal_msg, "[%s] file open error.", str_LoadFile);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return ;
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] file is not exist.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}
	
	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
// 		fread(&st_motor[i].md_spd_acc,		sizeof(st_motor[i].md_spd_acc), 1, fp);
// 		fread(&st_motor[i].md_spd_vel,		sizeof(st_motor[i].md_spd_vel), 1, fp);
// 		fread(&st_motor[i].md_spd_dec,		sizeof(st_motor[i].md_spd_dec), 1, fp);
// 		fread(&st_motor[i].md_spd_home,		sizeof(st_motor[i].md_spd_home), 1, fp);
// 		fread(&st_motor[i].md_spd_jog,		sizeof(st_motor[i].md_spd_jog), 1, fp);
 		fread(&st_motor[i].mn_allow,		sizeof(st_motor[i].mn_allow), 1, fp);
		fread(&COMI.md_spd_vel[i][0],		sizeof(COMI.md_spd_vel[i][0]), 1, fp);
		fread(&COMI.md_spd_vel[i][1],		sizeof(COMI.md_spd_vel[i][1]), 1, fp);
		fread(&COMI.md_spd_vel[i][2],		sizeof(COMI.md_spd_vel[i][2]), 1, fp);
		fread(&COMI.md_spd_home[i],			sizeof(COMI.md_spd_home[i]), 1, fp);
		fread(&st_motor[i].md_spd_jog,		sizeof(st_motor[i].md_spd_jog), 1, fp);
		fread(&COMI.mn_allow_value[i],		sizeof(COMI.mn_allow_value[i]), 1, fp);

	}
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] FILE Read Error.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			clearerr(fp);
		}
	}

	fclose(fp) ;

	//Speed Rate
	:: GetPrivateProfileString("Motor_Speed_Rate", "Run", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	st_handler.md_run_speed = (double)n_chk;
	COMI.mn_runspeed_rate = st_handler.md_run_speed;

	:: GetPrivateProfileString("Motor_Speed_Rate", "Manual", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	st_handler.md_manual_speed = (double)n_chk;
	COMI.mn_manualspeed_rate = st_handler.md_manual_speed;
}

void CMyBasicData::OnMotorSpeed_Set_Data_Save()
{
	int i = 0, j = 0;
	CString str_temp, str_part;       // 임시 저장 변수
	CString str_motor_name;
	char chr_buf[20];
	
	CString str_SaveFile;			// 임시 저장 변수
	
	str_SaveFile = OnGet_Teach_File_Name("Speed");		// 데이터 저장 파일명 로딩 함수 
	
	FILE  *fp;
	int   err = 0;
	
	if ((fp = fopen(str_SaveFile, "wb")) == NULL) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("Fail in Creating [%s] FILE.", str_SaveFile);
			sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}

	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
// 		fwrite(&st_motor[i].md_spd_acc,		sizeof(st_motor[i].md_spd_acc), 1, fp);
// 		fwrite(&st_motor[i].md_spd_vel,		sizeof(st_motor[i].md_spd_vel), 1, fp);
// 		fwrite(&st_motor[i].md_spd_dec,		sizeof(st_motor[i].md_spd_dec), 1, fp);
// 		fwrite(&st_motor[i].md_spd_home,	sizeof(st_motor[i].md_spd_home), 1, fp);
// 		fwrite(&st_motor[i].md_spd_jog,		sizeof(st_motor[i].md_spd_jog), 1, fp);
 		fwrite(&st_motor[i].mn_allow,		sizeof(st_motor[i].mn_allow), 1, fp);

		fwrite(&COMI.md_spd_vel[i][0],		sizeof(COMI.md_spd_vel[i][0]), 1, fp);
		fwrite(&COMI.md_spd_vel[i][1],		sizeof(COMI.md_spd_vel[i][1]), 1, fp);
		fwrite(&COMI.md_spd_vel[i][2],		sizeof(COMI.md_spd_vel[i][2]), 1, fp);
		fwrite(&COMI.md_spd_home[i],		sizeof(COMI.md_spd_home[i]), 1, fp);
		fwrite(&st_motor[i].md_spd_jog,		sizeof(st_motor[i].md_spd_jog), 1, fp);
		fwrite(&COMI.mn_allow_value[i],		sizeof(COMI.mn_allow_value[i]), 1, fp);
	}

	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] FILE Write Error.", str_SaveFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		clearerr(fp);
	}
	
	fclose(fp);
	
	// Speed Rate
	str_temp = LPCTSTR(_itoa((int)(COMI.mn_runspeed_rate), chr_buf, 10));
	:: WritePrivateProfileString("Motor_Speed_Rate", "Run", LPCTSTR(str_temp), st_path.mstr_basic);
	
	str_temp = LPCTSTR(_itoa((int)(COMI.mn_manualspeed_rate), chr_buf, 10));
	:: WritePrivateProfileString("Motor_Speed_Rate", "Manual", LPCTSTR(str_temp), st_path.mstr_basic);
}

void CMyBasicData::OnWaitTime_Data_Load()
{
	CString str_temp;       // 임시 저장 변수
	CString str_motor_name;
	
	CString str_msg, str_LoadFile;       // 임시 저장 변수
	
	FILE    *fp ;
	int     existence;
	
	str_LoadFile = OnGet_Teach_File_Name("Time");  // 데이터 저장 파일명 로딩 함수 
	
	existence = access(str_LoadFile, 0);
	
	if (!existence)
	{
		if ((fp=fopen(str_LoadFile,"rb")) == NULL)
		{
			if (st_handler.cwnd_list != NULL)
			{
//				st_msg.mstr_abnormal_msg.Format("[%s] file open error.", str_LoadFile);
				sprintf(st_msg.c_abnormal_msg, "[%s] file open error.", str_LoadFile);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return ;
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] file is not exist.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}
	
	fread(&st_wait, sizeof(st_wait), 1, fp);
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] FILE Read Error.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			clearerr(fp);
		}
	}

	fclose(fp) ;

// 	st_wait.i_picker_updn_w[0] = st_wait.i_picker_updn_w[1] = 100;//[0]up [1]dn
// 	st_wait.i_picker_l = 10000;
// 	st_wait.i_finger_onoff_w[0] = st_wait.i_finger_onoff_w[1] = 100;//[0]on [1]off
// 	st_wait.i_finger_l = 10000;
// 
// 	st_wait.i_module_clamp_w[0] = st_wait.i_module_clamp_w[1] = 100;
// 	st_wait.i_module_l = 10000;
// 
// 	st_wait.i_stacker_guide_w[0] = st_wait.i_stacker_guide_w[1] = 100;
// 	st_wait.i_stacker_guide_l = 10000;
// 
// 	st_wait.i_stacker_f_b_w[0] = st_wait.i_stacker_f_b_w[1] = 100;
// 	st_wait.i_stacker_f_b_l = 10000;
// 
// 	st_wait.i_stacker_rail_push_back_w[0] = st_wait.i_stacker_rail_push_back_w[1] = 100;
// 	st_wait.i_stacker_rail_push_back_l = 10000;
// 
// 	st_wait.i_stacker_updn_w[0] = st_wait.i_stacker_updn_w[1] = 100;
// 	st_wait.i_stacker_updn_l = 10000;
// 	
// 	st_wait.i_hs_stacker_guide_w[0] = st_wait.i_hs_stacker_guide_w[1] = 100;
// 	st_wait.i_hs_stacker_guide_l = 10000;
// 	
// 	st_wait.i_stacker_clip_w[0] = st_wait.i_stacker_clip_w[1] = 100;
// 	st_wait.i_stacker_clip_l = 10000;
// 
// 	st_wait.i_label_onoff_w[0] = st_wait.i_label_onoff_w[1] = 100;
// 	st_wait.i_label_l = 10000;
// 
// 	st_wait.i_label_vacuum = 10000;
// 	st_wait.i_label_eject = 10000;	
}

void CMyBasicData::OnWaitTime_Data_Save()
{
	CString str_temp;       // 임시 저장 변수
	CString str_motor_name;
	
	CString str_SaveFile;			// 임시 저장 변수
	
	str_SaveFile = OnGet_Teach_File_Name("Time");		// 데이터 저장 파일명 로딩 함수 
	
	FILE  *fp;
	int   err = 0;
	
	if ((fp = fopen(str_SaveFile, "wb")) == NULL) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("Fail in Creating [%s] FILE.", str_SaveFile);
			sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}
	
	fwrite(&st_wait, sizeof(st_wait), 1, fp);
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] FILE Write Error.", str_SaveFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		clearerr(fp);
	}
	
	fclose(fp);

// 	st_wait.i_picker_updn_w[0] = st_wait.i_picker_updn_w[0] = 500;//[0]up [1]dn
// 	st_wait.i_picker_l = 5000;
// 	st_wait.i_finger_onoff_w[0] = st_wait.i_finger_onoff_w[0] = 500;//[0]on [1]off
// 	st_wait.i_finger_l = 5000;
// 
// 	st_wait.i_module_clamp_w[0] = st_wait.i_module_clamp_w[1] = 500;
// 	st_wait.i_module_l = 5000;
// 
// 	st_wait.i_stacker_guide_w[0] = st_wait.i_stacker_guide_w[1] = 500;
// 	st_wait.i_stacker_guide_l = 5000;
// 
// 	st_wait.i_stacker_f_b_w[0] = st_wait.i_stacker_f_b_w[1] = 500;
// 	st_wait.i_stacker_f_b_l = 5000;
// 
// 	st_wait.i_stacker_rail_push_back_w[0] = st_wait.i_stacker_rail_push_back_w[1] = 500;
// 	st_wait.i_stacker_rail_push_back_l = 5000;
// 
// 	st_wait.i_stacker_updn_w[0] = st_wait.i_stacker_updn_w[1] = 500;
// 	st_wait.i_stacker_updn_l = 5000;
// 	
// 	st_wait.i_hs_stacker_guide_w[0] = st_wait.i_hs_stacker_guide_w[1] = 500;
// 	st_wait.i_hs_stacker_guide_l = 5000;
// 	
// 	st_wait.i_stacker_clip_w[0] = st_wait.i_stacker_clip_w[1] = 500;
// 	st_wait.i_stacker_clip_l = 5000;
// 
// 	st_wait.i_label_onoff_w[0] = st_wait.i_label_onoff_w[1] = 500;
// 	st_wait.i_label_l = 5000;
// 
// 	st_wait.i_label_vacuum = 5000;
// 	st_wait.i_label_eject = 5000;
}

void CMyBasicData::OnMaintenance_Data_Load()
{
	CString str_temp, str_part;  // 로딩 정보 임시 저장 변수
	int n_chk;
	CString strLoadFile;

	char chr_data[20], chr_buf[20] ;

	/* ************************************************************************** */
	/* 타워 램프 RED 상태 로딩하여 전역 변수에 설정한다                           */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Red", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[0] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[1] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[2] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[3] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[4] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[5] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[6] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Red", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_r[7] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Red", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_r[7] = n_chk;
	/* ************************************************************************** */


	/* ************************************************************************** */
	/* 타워 램프 YELLOW 상태 로딩하여 전역 변수에 설정한다                        */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Yellow", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[0] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[1] = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[2] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[3] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[4] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[5] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[6] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_y[7] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_y[7] = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 GREEN 상태 로딩하여 전역 변수에 설정한다                         */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData_Green", "Stop_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[0] = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[0] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Run_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[1] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[1] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Alarm_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[2] = 0;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[2], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[2] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "LotEnd_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[3] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[3], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[3] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Initial_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[4] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[4], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[4] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Warring_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[5] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[5], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[5] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "Lock_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[6] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[6], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[6] = n_chk;

	:: GetPrivateProfileString("TowerLampData_Green", "SelfCheck_State", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>2)
	{
		st_lamp.mn_lamp_g[7] = 2;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[7], chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData_Green", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_g[7] = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 부저 사용 모드 로딩하여 전역 변수에 설정                                   */
	/* -> 0:사용    1:미사용                                                      */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_buzzer_mode", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>1)
	{
		st_lamp.mn_buzzer_mode = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_buzzer_mode, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_buzzer_mode", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_buzzer_mode = n_chk;

	:: GetPrivateProfileString("TowerLampData", "n_front_mode", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>1)
	{
		st_lamp.mn_front_mode = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_front_mode, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_front_mode", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_front_mode = n_chk;

	:: GetPrivateProfileString("TowerLampData", "n_rear_mode", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk<0 || n_chk>1)
	{
		st_lamp.mn_rear_mode = 1;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_rear_mode, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_rear_mode", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_rear_mode = n_chk;

	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                           */
	/* -> 로딩 정보에서 앞/뒤 공백은 제거한다                                     */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "str_equip_no", "AMT", chr_data, 20, st_path.mstr_basic);
	st_lamp.mstr_equip_no = chr_data;
	(st_lamp.mstr_equip_no).TrimLeft(' ');               
	(st_lamp.mstr_equip_no).TrimRight(' ');

	:: GetPrivateProfileString("TowerLampData", "str_equip_code", "AMT", chr_data, 20, st_path.mstr_basic);
	st_lamp.mstr_equip_id = chr_data;
	(st_lamp.mstr_equip_id).TrimLeft(' ');               
	(st_lamp.mstr_equip_id).TrimRight(' ');
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                       */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_lamp_on_time_w", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk < 1)
	{
		st_lamp.mn_lamp_wait_time = 500;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_wait_time, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_lamp_on_time_w", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_wait_time = n_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 변경 시간 로딩하여 전역 변수에 설정                              */
	/* ************************************************************************** */
	:: GetPrivateProfileString("TowerLampData", "n_lamp_change_time", "0", chr_data, 10, st_path.mstr_basic);
	n_chk = atoi(chr_data);
	if (n_chk < 0)
	{
		st_lamp.mn_lamp_change_time = 5;
		str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_change_time, chr_buf, 10)) ;
		:: WritePrivateProfileString("TowerLampData", "n_lamp_change_time", LPCTSTR(str_temp), st_path.mstr_basic);
	}
	else
		st_lamp.mn_lamp_change_time = n_chk;
	/* ************************************************************************** */
}

void CMyBasicData::OnMaintenance_Data_Save()
{
	CString str_temp, str_part;  // 로딩 정보 임시 저장 변수
	char chr_buf[20] ;
	CString strSaveFile;

	/* ************************************************************************** */
	/* 타워 램프 RED 상태 정보를 파일에 저장한다                                  */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_r[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Red", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */


	/* ************************************************************************** */
	/* 타워 램프 YELLOW 상태 정보를 파일에 저장한다                               */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_y[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Yellow", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);

	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 GREEN 상태 정보를 파일에 저장한다                                */
	/* ->  STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARRING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7														  */
	/* -> 로딩 값 [0:OFF    1:ON    2:FLICKER]                                    */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[0], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Stop_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[1], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Run_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[2], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Alarm_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[3], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "LotEnd_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[4], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Initial_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[5], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Warring_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[6], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "Lock_State", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_g[7], chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData_Green", "SelfCheck_State", LPCTSTR(str_temp), st_path.mstr_basic);

	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 부저 사용 모드 로딩하여 전역 변수에 설정                                   */
	/* -> 0:사용    1:미사용                                                      */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_buzzer_mode, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_buzzer_mode", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_front_mode, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_front_mode", LPCTSTR(str_temp), st_path.mstr_basic);

	str_temp = LPCTSTR(_itoa(st_lamp.mn_rear_mode, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_rear_mode", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 장비 호기 및 장비 코드 로딩하여 전역 변수에 설정                           */
	/* -> 로딩 정보에서 앞/뒤 공백은 제거한다                                     */
	/* ************************************************************************** */
	:: WritePrivateProfileString("TowerLampData", "str_equip_no", LPCTSTR(st_lamp.mstr_equip_no), st_path.mstr_basic);
	:: WritePrivateProfileString("TowerLampData", "str_equip_code", LPCTSTR(st_lamp.mstr_equip_id), st_path.mstr_basic);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 타워 램프 ON/OFF 대기 시간 로딩하여 전역 변수에 설정                       */
	/* ************************************************************************** */
	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_wait_time, chr_buf, 10));
	:: WritePrivateProfileString("TowerLampData", "n_lamp_on_time_w", LPCTSTR(str_temp), st_path.mstr_basic);
	/* ************************************************************************** */

	str_temp = LPCTSTR(_itoa(st_lamp.mn_lamp_change_time, chr_buf, 10)) ;
	:: WritePrivateProfileString("TowerLampData", "n_lamp_change_time", LPCTSTR(str_temp), st_path.mstr_basic);
}

/////////////////////////////////////////////////////////// 20120516
void CMyBasicData::OnInterface_Data_Load()
{
	int i;
	CString str_tmp, str_name;
	char ch_data[30];

	for(i=0; i<10; i++)
	{
		str_name.Format("%02d_CLIENT_IP", i);
		:: GetPrivateProfileString("NETWORK", str_name, "", ch_data, 30, st_path.mstr_interface_folder);
		st_client[i].str_ip.Format("%s", ch_data);

		str_name.Format("%02d_CLIENT_PORT", i);
		:: GetPrivateProfileString("NETWORK", str_name, "", ch_data, 30, st_path.mstr_interface_folder);
		st_client[i].n_port = atoi(ch_data);

		str_name.Format("%02d_SERVER_PORT", i);
		:: GetPrivateProfileString("NETWORK", str_name, "", ch_data, 30, st_path.mstr_interface_folder);
		st_server[i].n_port = atoi(ch_data);
		

		str_name.Format("PORT_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "1", ch_data, 20, st_path.mstr_interface_folder);
		rs_232.n_serial_port[i] = atoi(ch_data);
		
		str_name.Format("BAUDRATE_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "9600", ch_data, 20, st_path.mstr_interface_folder);
		rs_232.n_serial_baudrate[i] = atoi(ch_data);
		
		str_name.Format("DATA_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "8", ch_data, 20, st_path.mstr_interface_folder);
		rs_232.n_serial_data[i] = atoi(ch_data);
		
		str_name.Format("STOP_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "1", ch_data, 20, st_path.mstr_interface_folder);
		rs_232.n_serial_stop[i] = atoi(ch_data);
		
		str_name.Format("PARITY_%02d", i+1);
		:: GetPrivateProfileString("SERIAL", str_name, "0", ch_data, 20, st_path.mstr_interface_folder);
		rs_232.n_serial_parity[i] = atoi(ch_data);
	}

	:: GetPrivateProfileString("Interface", "EQUIP_ID", "", ch_data, 20, st_path.mstr_interface_folder);
	str_tmp = ch_data;

//	st_work.str_equip_id = str_temp;  // 암호 전역 변수에 설정

	:: GetPrivateProfileString("Interface", "BOARD_TYPE", "", ch_data, 20, st_path.mstr_interface_folder);
	str_tmp = ch_data;
	
//	st_work.str_board_type = str_temp;  // 암호 전역 변수에 설정

	:: GetPrivateProfileString("Interface", "PGM_REVISION", "", ch_data, 20, st_path.mstr_interface_folder);
	str_tmp = ch_data;
	
//	st_work.str_pgm_revision= str_temp;  // 암호 전역 변수에 설정


// 필요시 사용 
// 	:: GetPrivateProfileString("Model_List", "ML_BCR_Mode", "0", ch_data, 10, st_path.mstr_interface_folder);
// 	mn_chk = atoi(ch_data);
// 	st_interface.n_bcr_ml_mode = mn_chk;
// 
// 	:: GetPrivateProfileString("Model_List", "ML_BCR_Type", "", ch_data, 20, st_path.mstr_interface_folder);
// 	st_interface.str_bcr_ml_name = (LPCTSTR)ch_data;
// 
// 	:: GetPrivateProfileString("Model_List", "ML_HS_Mode", "0", ch_data, 10, st_path.mstr_interface_folder);
// 	mn_chk = atoi(ch_data);
// 	st_interface.n_hs_ml_mode = mn_chk;
// 
// 	:: GetPrivateProfileString("Model_List", "ML_HS_Type", "", ch_data, 20, st_path.mstr_interface_folder);
// 	st_interface.str_hs_ml_name = (LPCTSTR)ch_data;



}

void CMyBasicData::OnInterface_Data_Save()
{
	int i;
	CString str_tmp, str_name;

	for(i=0; i<10; i++)
	{
		str_name.Format("%02d_CLIENT_IP", i);
		str_tmp.Format("%s", st_client[i].str_ip);
		:: WritePrivateProfileString("NETWORK", str_name, LPCTSTR(str_tmp), st_path.mstr_interface_folder);
		
		str_name.Format("%02d_CLIENT_PORT", i);
		str_tmp.Format("%d", st_client[i].n_port);
		:: WritePrivateProfileString("NETWORK", str_name, LPCTSTR(str_tmp), st_path.mstr_interface_folder);
		
		str_name.Format("%02d_SERVER_PORT", i);
		str_tmp.Format("%d", st_server[i].n_port);
		:: WritePrivateProfileString("NETWORK", str_name, LPCTSTR(str_tmp), st_path.mstr_interface_folder);

		str_name.Format("PORT_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_port[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.mstr_interface_folder);
		
		str_name.Format("BAUDRATE_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_baudrate[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.mstr_interface_folder);
		
		str_name.Format("DATA_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_data[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.mstr_interface_folder);
		
		str_name.Format("STOP_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_stop[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.mstr_interface_folder);
		
		str_name.Format("PARITY_%02d", i+1);
		str_tmp.Format("%d", rs_232.n_serial_parity[i]);
		:: WritePrivateProfileString("SERIAL", str_name, LPCTSTR(str_tmp), st_path.mstr_interface_folder);
	}

//	str_temp = st_work.str_equip_id;
	:: WritePrivateProfileString("Interface", "EQUIP_ID", LPCTSTR(str_tmp), st_path.mstr_interface_folder);

//	str_temp = st_work.str_board_type;
	:: WritePrivateProfileString("Interface", "BOARD_TYPE", LPCTSTR(str_tmp), st_path.mstr_interface_folder);

//	str_temp = st_work.str_pgm_revision;
	:: WritePrivateProfileString("Interface", "PGM REVISION", LPCTSTR(str_tmp), st_path.mstr_interface_folder);

// 필요시 사용 
// 	str_tmp.Format("%s", st_interface.str_bcr_ml_name);	
// 	:: WritePrivateProfileString("BasicData", "ML_BCR_Type", LPCTSTR(str_tmp), st_path.mstr_interface_folder);
// 
// 	str_tmp.Format("%d", st_interface.n_bcr_ml_mode);
// 	:: WritePrivateProfileString("BasicData", "ML_BCR_Mode", LPCTSTR(str_tmp), st_path.mstr_interface_folder);
// 
// 	str_tmp.Format("%s", st_interface.str_hs_ml_name);	
// 	:: WritePrivateProfileString("BasicData", "ML_HS_Type", LPCTSTR(str_tmp), st_path.mstr_interface_folder);
// 
// 	str_tmp.Format("%d", st_interface.n_hs_ml_mode);
// 	:: WritePrivateProfileString("BasicData", "ML_HS_Mode", LPCTSTR(str_tmp), st_path.mstr_interface_folder);


}



///////////////////////////////////////////////////////////


void CMyBasicData::OnBasic_Data_Load()
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_chk_ext;		// 파일 확장자 저장 변수
	CString mstr_temp, str_part, str_station, str_board;			// 저장할 정보 임시 저장 변수 
	char chr_data[50], chr_buf[20];	//20120530
	int mn_chk = 0, mn_pos = 0, n_pos = 0, i = 0, j = 0;
	double md_chk = 0;

	/* ************************************************************************** */
    /*  데이터 로딩할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */
	str_load_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 로딩 파일 설정
	
	n_pos = str_load_file.Find(".");  // 확장자 위치 검사

	if (n_pos == -1)
		str_load_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_load_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_load_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	/* ************************************************************************** */
	:: GetPrivateProfileString("BasicData", "Run_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_mode_run = 1;	//20120522
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_mode_run, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Run_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_mode_run = mn_chk;

	:: GetPrivateProfileString("BasicData", "Device_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_device = 1;
		mstr_temp.Format("%d", st_basic.n_mode_device);
		:: WritePrivateProfileString("BasicData", "Device_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_device = mn_chk;




	:: GetPrivateProfileString("BasicData", "Electric_Check", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 1440)
	{
		st_basic.mn_electrostatic_check_gap = 120;
		mstr_temp.Format("%d", st_basic.mn_electrostatic_check_gap);
		:: WritePrivateProfileString("BasicData", "Electric_Check", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_electrostatic_check_gap = mn_chk;


 	:: GetPrivateProfileString("BasicData", "ELECT_VALUE", "0", chr_data, 10, st_path.mstr_basic);
	md_chk = atof(chr_data);
	if (md_chk < 1 || md_chk > 1440)
	{
		st_basic.md_electrostatic_value = 120;
		mstr_temp.Format("%d", st_basic.md_electrostatic_value);
		:: WritePrivateProfileString("BasicData", "ELECT_VALUE", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.md_electrostatic_value = md_chk;

 	:: GetPrivateProfileString("BasicData", "MODULE_PICKER_GAP", "0", chr_data, 10, st_path.mstr_basic);
	md_chk = atof(chr_data);
	if (md_chk < 1 || md_chk > 1440)
	{
		st_basic.md_picker_gap_module = 0;
		mstr_temp.Format("%d", st_basic.md_picker_gap_module);
		:: WritePrivateProfileString("BasicData", "MODULE_PICKER_GAP", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.md_picker_gap_module = md_chk;

 	:: GetPrivateProfileString("BasicData", "HEATSINK_LEFT_PICKER_GAP", "0", chr_data, 10, st_path.mstr_basic);
	md_chk = atof(chr_data);
	if (md_chk < 1 || md_chk > 1440)
	{
		st_basic.md_picker_gap_hs_left = 0;
		mstr_temp.Format("%d", st_basic.md_picker_gap_hs_left);
		:: WritePrivateProfileString("BasicData", "HEATSINK_LEFT_PICKER_GAP", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.md_picker_gap_hs_left = md_chk;

 	:: GetPrivateProfileString("BasicData", "HEATSINK_RIGHT_PICKER_GAP", "0", chr_data, 10, st_path.mstr_basic);
	md_chk = atof(chr_data);
	if (md_chk < 1 || md_chk > 1440)
	{
		st_basic.md_picker_gap_hs_right = 0;
		mstr_temp.Format("%d", st_basic.md_picker_gap_hs_right);
		:: WritePrivateProfileString("BasicData", "HEATSINK_RIGHT_PICKER_GAP", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.md_picker_gap_hs_right = md_chk;


	:: GetPrivateProfileString("BasicData", "Run_Speed", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 100)
	{
		st_basic.nRunSpeed = 100;
		mstr_temp.Format("%d", st_basic.nRunSpeed);
		:: WritePrivateProfileString("BasicData", "Run_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nRunSpeed = mn_chk;

	:: GetPrivateProfileString("BasicData", "Manual_Speed", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 100)
	{
		st_basic.nManualSpeed = 60;
		mstr_temp.Format("%d", st_basic.nManualSpeed);
		:: WritePrivateProfileString("BasicData", "Manual_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nManualSpeed = mn_chk;

	// Data Logger에 있는 하루의 시작 시간을 가져온다. 같이 사용하니까.
	:: GetPrivateProfileString("DAY START TIME", "Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 24)
	{
		st_basic.n_day_start_time = 22;
	}
	else  st_basic.n_day_start_time = mn_chk;
	st_basic.n_day_start_time = 22;

	// Data Logger에 있는 Shift 시간을 가져온다. 같이 사용하니까.
	:: GetPrivateProfileString("DAY SHIFT TIME", "Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 24)
	{
		st_basic.n_day_shift_time = 8;
	}
	else  st_basic.n_day_shift_time = mn_chk;
	st_basic.n_day_shift_time = 8;

	:: GetPrivateProfileString("BasicData", "Day_Alarm_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		alarm.mn_alarm_occured_day = 0;
		mstr_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  alarm.mn_alarm_occured_day = mn_chk;


	:: GetPrivateProfileString("BasicData", "Daily_Alarm_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_work.m_nAlarmCnt = 0;
	}
	else  st_work.m_nAlarmCnt = mn_chk;


	:: GetPrivateProfileString("BasicData", "Daily_Input_Total_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_work.nMdlInputTotalCount = 0;
	}
	else  st_work.nMdlInputTotalCount = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Pass_Total_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_work.nMdlPassTotalCount = 0;
	}
	else  st_work.nMdlPassTotalCount = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Reject_Total_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_work.nMdlRejectTotalCount = 0;
	}
	else  st_work.nMdlRejectTotalCount = mn_chk;


	:: GetPrivateProfileString("BasicData", "Daily_Run_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDR = 0;
	}
	else  st_handler.m_tDR = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Stop_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDS = 0;
	}
	else  st_handler.m_tDS = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Alarm_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDJ = 0;
	}
	else  st_handler.m_tDJ = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Maint_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_handler.m_tDM = 0;
	}
	else  st_handler.m_tDM = mn_chk;

	:: GetPrivateProfileString("BasicData", "Data_Save_Time", "0", chr_data, 11, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0)
	{
		st_work.n_data_save_time = 0;
	}
	else  st_work.n_data_save_time = mn_chk;

	Func.OnDayDataCheck();

	:: GetPrivateProfileString("Interface", "ECSERVER_IP", "109.117.11.29", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	
	st_Network[NETWORK_ECSERVER].mstr_ip = mstr_temp;  // 암호 전역 변수에 설정
	
	:: GetPrivateProfileString("Interface", "ECSERVER_Port", "12108", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	
	st_Network[NETWORK_ECSERVER].mstr_port = mstr_temp;  // 암호 전역 변수에 설정

	:: GetPrivateProfileString("Interface", "ECSERVER_LDPort", "12007", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	
	st_Network[NETWORK_ECSERVER].mstr_ldport = mstr_temp;  // 암호 전역 변수에 설정
	
	:: GetPrivateProfileString("BasicData", "Network_Wait_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 120)
	{
		st_basic.mn_network_wait_time = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_network_wait_time, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Network_Wait_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_network_wait_time = mn_chk;

	:: GetPrivateProfileString("BasicData", "Network_Retry_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 120)
	{
		st_basic.mn_network_retry_cnt = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_network_retry_cnt, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Network_Retry_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_network_retry_cnt = mn_chk;

	:: GetPrivateProfileString("BasicData", "Ararm_Delay_Time", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1000)
	{
		st_basic.mn_alarm_delay_time = 10;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_alarm_delay_time, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Ararm_Delay_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_alarm_delay_time = mn_chk;

	:: GetPrivateProfileString("BasicData", "Label_Print_Error_Cnt", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 11)
	{
		st_basic.mn_Label_Print_Error_Cnt = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_Label_Print_Error_Cnt, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Label_Print_Error_Cnt", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_Label_Print_Error_Cnt = mn_chk;

///////////// 20120731
	:: GetPrivateProfileString("BasicData", "Dvc_Repick_Cnt", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 9)
	{
		st_basic.mn_Dvc_Repick_Cnt = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_Dvc_Repick_Cnt, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Dvc_Repick_Cnt", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_Dvc_Repick_Cnt = mn_chk;
/////////////

	:: GetPrivateProfileString("BasicData", "DeviceType", "0", chr_data, 10, str_load_file);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_device_type = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_device_type, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "DeviceType", LPCTSTR(mstr_temp), str_load_file);
	}
	else  st_basic.mn_device_type = mn_chk;



	:: GetPrivateProfileString("BasicData", "Tray_X", "-1", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 3)
	{
		st_basic.n_tray_x = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_tray_x, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Tray_X", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_tray_x = mn_chk;


	if (st_basic.n_tray_x == 1)
	{
		st_basic.n_tray_size = 0;
	}
	else
	{
		st_basic.n_tray_size = 1;
	}

	:: GetPrivateProfileString("BasicData", "Tray_Y", "-1", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 25)
	{
		st_basic.n_tray_y = 25;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_tray_y, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Tray_Y", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_tray_y = mn_chk;


	:: GetPrivateProfileString("BasicData", "Input_Prime_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlInputCount[0][1] = mn_chk;

	:: GetPrivateProfileString("BasicData", "Input_Cum_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlInputCount[1][1] = mn_chk;

	:: GetPrivateProfileString("BasicData", "Pass_Prime_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlPassCount[0][1] = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Pass_Cum_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlPassCount[1][1] = mn_chk;

	:: GetPrivateProfileString("BasicData", "Reject_Prime_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlRejectCount[0][1] = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Reject_Cum_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlRejectCount[1][1] = mn_chk;

	:: GetPrivateProfileString("BasicData", "Fail_Prime_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlFailCount[0][1] = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Fail_Cum_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlFailCount[1][1] = mn_chk;

	:: GetPrivateProfileString("BasicData", "Hto_Prime_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlHtoCount[0][1] = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Hto_Cum_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlHtoCount[1][1] = mn_chk;

	:: GetPrivateProfileString("BasicData", "Bto_Prime_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlBtoCount[0][1] = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Bto_Cum_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlBtoCount[1][1] = mn_chk;

	:: GetPrivateProfileString("BasicData", "Nto_Prime_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlNtoCount[0][1] = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Nto_Cum_Day", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.nMdlNtoCount[1][1] = mn_chk;

	:: GetPrivateProfileString("BasicData", "Daily_Fail_Count", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	st_work.mn_daily_fail_count = mn_chk;

	//////////////////////////////////// 20120522
	:: GetPrivateProfileString("BasicData", "Module_Load_Lot_Set", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk <= 0 || mn_chk > 100)
	{
		st_basic.mn_lotset_module_load = st_traybuffer[LDMODULE_SITE].i_loader_row = 25;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_module_load, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Module_Load_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_lotset_module_load = st_traybuffer[LDMODULE_SITE].i_loader_row = mn_chk;

	:: GetPrivateProfileString("BasicData", "HS_Left_Lot_Set", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk <= 0 || mn_chk > 100)
	{
		st_basic.mn_lotset_hs_left = st_traybuffer[LEFTSINK_SITE].i_loader_row = 30;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_hs_left, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "HS_Left_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}											   
	else  st_basic.mn_lotset_hs_left = st_traybuffer[LEFTSINK_SITE].i_loader_row = mn_chk;

	:: GetPrivateProfileString("BasicData", "HS_Right_Lot_Set", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk <= 0 || mn_chk > 100)
	{
		st_basic.mn_lotset_hs_right = st_traybuffer[RIGHTSINK_SITE].i_loader_row = 30;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_hs_right, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "HS_Right_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}											   
	else  st_basic.mn_lotset_hs_right = st_traybuffer[RIGHTSINK_SITE].i_loader_row = mn_chk;

	:: GetPrivateProfileString("BasicData", "Clip_Lot_Set", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk <= 0 || mn_chk > 100)
	{
		st_basic.mn_lotset_clip = st_traybuffer[LDCLIP_SITE].i_loader_row = 60;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_clip, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Clip_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_lotset_clip = st_traybuffer[LDCLIP_SITE].i_loader_row = mn_chk;

	:: GetPrivateProfileString("BasicData", "Unloader_Lot_Set", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk <= 0 || mn_chk > 100)
	{
		st_basic.mn_lotset_unloader = st_traybuffer[UNLOADER_SITE].i_loader_row = 25;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_unloader, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Unloader_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}											   
	else  st_basic.mn_lotset_unloader = st_traybuffer[UNLOADER_SITE].i_loader_row = mn_chk;

	:: GetPrivateProfileString("BasicData", "Reject_Lot_Set", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk <= 0 || mn_chk > 100)
	{
		st_basic.mn_lotset_reject = st_traybuffer[REJECT_SITE].i_loader_row = 25;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_reject, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Reject_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_lotset_reject = st_traybuffer[REJECT_SITE].i_loader_row = mn_chk;

	:: GetPrivateProfileString("BasicData", "M_Direction", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_m_direction = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_m_direction, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "M_Direction", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_m_direction = mn_chk;

	:: GetPrivateProfileString("BasicData", "HS_Direction", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_hs_direction = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_hs_direction, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "HS_Direction", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_hs_direction = mn_chk;

	
	:: GetPrivateProfileString("BasicData", "Bcr_Read", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_bcr = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_bcr, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Bcr_Read", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_bcr = mn_chk;


	:: GetPrivateProfileString("BasicData", "Module_Clip_Swing", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_module_clip_swing = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_module_clip_swing, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Module_Clip_Swing", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_module_clip_swing = mn_chk;

	:: GetPrivateProfileString("BasicData", "Use_ClipRemoveBin", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_use_bin = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_use_bin, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Use_ClipRemoveBin", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_use_bin = mn_chk;


	:: GetPrivateProfileString("BasicData", "Use_Clip_alarm", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_clip_alarm = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_clip_alarm, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Use_Clip_alarm", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_clip_alarm = mn_chk;

	// 20140220
	:: GetPrivateProfileString("BasicData", "Use_Cap_Remove", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_cap_remove = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_cap_remove, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Use_Cap_Remove", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_cap_remove = mn_chk;

	:: GetPrivateProfileString("BasicData", "ClampClipBin", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 10)
	{
		st_basic.n_clamp_clip_bin = 4;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_clamp_clip_bin, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "ClampClipBin", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_clamp_clip_bin = mn_chk;

	:: GetPrivateProfileString("BasicData", "ClipBin", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 100)
	{
		st_basic.n_clip_bin = 30;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_clip_bin, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "ClipBin", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_clip_bin = mn_chk;

	:: GetPrivateProfileString("BasicData", "Curr_ClampClipBin", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 10)
	{
		st_basic.n_curr_clamp_clip_bin = 10;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_curr_clamp_clip_bin, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Curr_ClampClipBin", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_curr_clamp_clip_bin = mn_chk;

	:: GetPrivateProfileString("BasicData", "Curr_ClipBin", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 100)
	{
		st_basic.n_curr_clip_bin = 10;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_curr_clip_bin, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Curr_ClipBin", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_curr_clip_bin = mn_chk;
	//////////////////////////////////// 20120523
	:: GetPrivateProfileString("BasicData", "Jog_Speed", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 100)
	{
		st_basic.nJogSpeed = 100;
		mstr_temp.Format("%d", st_basic.nJogSpeed);
		:: WritePrivateProfileString("BasicData", "Jog_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nJogSpeed = mn_chk;

	:: GetPrivateProfileString("BasicData", "Barcode_Speed", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 100)
	{
		st_basic.nBarcodeSpeed = 60;
		mstr_temp.Format("%d", st_basic.nBarcodeSpeed);
		:: WritePrivateProfileString("BasicData", "Barcode_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.nBarcodeSpeed = mn_chk;

	:: GetPrivateProfileString("BasicData", "Label_Vision_Paper", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 99)
	{
		st_basic.mn_label_vision_paper = 1;
		mstr_temp.Format("%d", st_basic.mn_label_vision_paper);
		:: WritePrivateProfileString("BasicData", "Label_Vision_Paper", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_label_vision_paper = mn_chk;

	// 20140224
	:: GetPrivateProfileString("BasicData", "Label_Bin_Print", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 1 || mn_chk > 26)
	{
		st_basic.mn_label_bin_print = 1;
		mstr_temp.Format("%d", st_basic.mn_label_bin_print);
		:: WritePrivateProfileString("BasicData", "Label_Bin_Print", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.mn_label_bin_print = mn_chk;
	//

	///////////////// 
/////////////// 20120711

	//2015.0129	
	:: GetPrivateProfileString("BasicData", "Use_Hs_Vis_Continuous_Error", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_use_vis_cont_err = 1;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_use_vis_cont_err, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Use_Hs_Vis_Continuous_Error", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_use_vis_cont_err = mn_chk;


	:: GetPrivateProfileString("BasicData", "Hs_Vis_Continuous_Error", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 100)
	{
		st_basic.n_vis_cont_err = 3;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_vis_cont_err, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Hs_Vis_Continuous_Error", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_vis_cont_err = mn_chk;

	:: GetPrivateProfileString("BasicData", "Hs_Vis_Total_Error", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 500)
	{
		st_basic.n_vis_tot_err = 10;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_vis_tot_err, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Hs_Vis_Total_Error", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_vis_tot_err = mn_chk;


	:: GetPrivateProfileString("WORKER", "NAME", "0", chr_data, 30, st_path.mstr_basic);
	st_work.str_worker_name = chr_data;

	:: GetPrivateProfileString("WORKER", "CREM", "0", chr_data, 30, st_path.mstr_basic);
	st_work.str_worker_crem = chr_data;

	:: GetPrivateProfileString("WORKER", "NUMBER", "0", chr_data, 30, st_path.mstr_basic);
	st_work.mstr_op_id = chr_data;

///////////////// 
	:: GetPrivateProfileString("BasicData", "Use_DoorLock", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_use_door = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_use_door, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Use_DoorLock", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_use_door = mn_chk;


	:: GetPrivateProfileString("BasicData", "Use_Sorter_Picker", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.n_mode_use_sorter_picker = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_use_sorter_picker, chr_buf, 10));
		:: WritePrivateProfileString("BasicData", "Use_Sorter_Picker", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_basic.n_mode_use_sorter_picker = mn_chk;

}

void CMyBasicData::OnBasic_Data_Save()
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	CString str_save_file;
	CString str_part, str_chk_ext;
	char chr_buf[20];
	COleDateTime time_cur;
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour, n_pos; // 현재 시간정보 저장 변수 

	/* ************************************************************************** */
    /*  데이터 저장할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */
	str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 저장 파일 설정
	
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
		str_save_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	/* ************************************************************************** */

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_mode_run, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Run_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.mn_electrostatic_check_gap);
	:: WritePrivateProfileString("BasicData", "Electric_Check", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_mode_device);
	:: WritePrivateProfileString("BasicData", "Device_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%0.2f", st_basic.md_picker_gap_module);
	:: WritePrivateProfileString("BasicData", "MODULE_PICKER_GAP", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%0.2f", st_basic.md_picker_gap_hs_left);
	:: WritePrivateProfileString("BasicData", "HEATSINK_LEFT_PICKER_GAP", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%0.2f", st_basic.md_picker_gap_hs_right);
	:: WritePrivateProfileString("BasicData", "HEATSINK_RIGHT_PICKER_GAP", LPCTSTR(mstr_temp), st_path.mstr_basic);

	

	mstr_temp.Format("%0.2f", st_basic.md_electrostatic_value);
	:: WritePrivateProfileString("BasicData", "ELECT_VALUE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_alarm_delay_time, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Ararm_Delay_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.nRunSpeed);
	:: WritePrivateProfileString("BasicData", "Run_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.nManualSpeed);
	:: WritePrivateProfileString("BasicData", "Manual_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	:: WritePrivateProfileString("BasicData", "Device_Type", LPCTSTR(st_basic.mstr_device_name), st_path.mstr_basic);


	mstr_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
	:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_work.m_nAlarmCnt);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDR);
	:: WritePrivateProfileString("BasicData", "Daily_Run_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDS);
	:: WritePrivateProfileString("BasicData", "Daily_Stop_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDJ);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDM);
	:: WritePrivateProfileString("BasicData", "Daily_Maint_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_work.nMdlInputTotalCount);
	:: WritePrivateProfileString("BasicData", "Daily_Input_Total_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_work.nMdlPassTotalCount);
	:: WritePrivateProfileString("BasicData", "Daily_Pass_Total_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_work.nMdlRejectTotalCount);
	:: WritePrivateProfileString("BasicData", "Daily_Reject_Total_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);



	time_cur = COleDateTime::GetCurrentTime();

	mn_cur_year = time_cur.GetYear();  
	mn_cur_month = time_cur.GetMonth();  
    mn_cur_day = time_cur.GetDay();  
	mn_cur_hour = time_cur.GetHour();
	/* ************************************************************************** */

	mstr_temp.Format("%04d%02d%02d%02d", mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour);
	:: WritePrivateProfileString("BasicData", "Data_Save_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = st_Network[NETWORK_ECSERVER].mstr_ip;
	:: WritePrivateProfileString("Interface", "ECSERVER_IP", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp = st_Network[NETWORK_ECSERVER].mstr_port;
	:: WritePrivateProfileString("Interface", "ECSERVER_Port", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = st_Network[NETWORK_ECSERVER].mstr_ldport;
	:: WritePrivateProfileString("Interface", "ECSERVER_LDPort", LPCTSTR(mstr_temp), st_path.mstr_basic);
		
	mstr_temp = LPCTSTR(_itoa(st_basic.mn_bypass_mode, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Bypass_Mode", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_network_wait_time, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Network_Wait_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_network_retry_cnt, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Network_Retry_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_Label_Print_Error_Cnt, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Label_Print_Error_Cnt", LPCTSTR(mstr_temp), st_path.mstr_basic);

//////////  20120731
	mstr_temp = LPCTSTR(_itoa(st_basic.mn_Dvc_Repick_Cnt, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Dvc_Repick_Cnt", LPCTSTR(mstr_temp), st_path.mstr_basic);
//////////
	


	//st_basic.mn_device_type
	mstr_temp = LPCTSTR(_itoa(st_basic.mn_device_type, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "DeviceType", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_tray_x, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Tray_X", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_tray_y, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Tray_Y", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_work.nMdlInputCount[0][1], chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Input_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_work.nMdlInputCount[1][1], chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Input_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_work.nMdlPassCount[0][1], chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Pass_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp = LPCTSTR(_itoa(st_work.nMdlPassCount[1][1], chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Pass_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_work.nMdlRejectCount[0][1], chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Reject_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp = LPCTSTR(_itoa(st_work.nMdlRejectCount[1][1], chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Reject_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_work.nMdlFailCount[0][1], chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Fail_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp = LPCTSTR(_itoa(st_work.nMdlFailCount[1][1], chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Fail_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_work.nMdlHtoCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Hto_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlHtoCount[1][1]);
	:: WritePrivateProfileString("BasicData", "Hto_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlBtoCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Bto_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlBtoCount[1][1]);
	:: WritePrivateProfileString("BasicData", "Bto_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlNtoCount[0][1]);
	:: WritePrivateProfileString("BasicData", "Nto_Prime_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_work.nMdlNtoCount[1][1]);
	:: WritePrivateProfileString("BasicData", "Nto_Cum_Day", LPCTSTR(mstr_temp), st_path.mstr_basic);

	///////////////// 20120522
	// LOT Set
	mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_module_load, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Module_Load_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_hs_left, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "HS_Left_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_hs_right, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "HS_Right_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_clip, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Clip_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_unloader, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Unloader_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_lotset_reject, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Reject_Lot_Set", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_m_direction, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "M_Direction", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_hs_direction, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "HS_Direction", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_bcr, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Bcr_Read", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_module_clip_swing, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Module_Clip_Swing", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_use_bin, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Use_ClipRemoveBin", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_clip_alarm, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Use_Clip_alarm", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_cap_remove, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Use_Cap_Remove", LPCTSTR(mstr_temp), st_path.mstr_basic); // 20140220

	mstr_temp = LPCTSTR(_itoa(st_basic.mn_label_bin_print, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Label_Bin_Print", LPCTSTR(mstr_temp), st_path.mstr_basic); // 20140224

	mstr_temp = LPCTSTR(_itoa(st_basic.n_clamp_clip_bin, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "ClampClipBin", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_clip_bin, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "ClipBin", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_curr_clamp_clip_bin, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Curr_ClampClipBin", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_curr_clip_bin, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Curr_ClipBin", LPCTSTR(mstr_temp), st_path.mstr_basic);
	///////////////// 20120523

	//2015.0129
	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_use_vis_cont_err, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "VIS_HS_Cont_err", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_vis_cont_err, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Hs_Vis_Continuous_Error", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_vis_tot_err, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Hs_Vis_Total_Error", LPCTSTR(mstr_temp), st_path.mstr_basic);


	mstr_temp.Format("%d", st_basic.nJogSpeed);
	:: WritePrivateProfileString("BasicData", "Jog_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.nBarcodeSpeed);
	:: WritePrivateProfileString("BasicData", "Barcode_Speed", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d", st_basic.mn_label_vision_paper);
	:: WritePrivateProfileString("BasicData", "Label_Vision_Paper", LPCTSTR(mstr_temp), st_path.mstr_basic);

/////////////// 20120711
	:: WritePrivateProfileString("WORKER", "NAME", LPCTSTR(st_work.str_worker_name), st_path.mstr_basic);

	:: WritePrivateProfileString("WORKER", "CREM", LPCTSTR(st_work.str_worker_crem), st_path.mstr_basic);

	:: WritePrivateProfileString("WORKER", "NUMBER", LPCTSTR(st_work.mstr_op_id), st_path.mstr_basic);
///////////////

	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_use_door, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Use_DoorLock", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_use_sorter_picker, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Use_Sorter_Picker", LPCTSTR(mstr_temp), st_path.mstr_basic);



}

void CMyBasicData::OnDaily_Count_Save()
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20];

	mstr_temp = LPCTSTR(_itoa(alarm.mn_alarm_occured_day, chr_buf, 10)) ;
	:: WritePrivateProfileString("BasicData", "Day_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDR);
	:: WritePrivateProfileString("BasicData", "Daily_Run_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDS);
	:: WritePrivateProfileString("BasicData", "Daily_Stop_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDJ);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_handler.m_tDM);
	:: WritePrivateProfileString("BasicData", "Daily_Maint_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp = LPCTSTR(_itoa(st_basic.n_mode_use_door, chr_buf, 10));
	:: WritePrivateProfileString("BasicData", "Use_DoorLock", LPCTSTR(mstr_temp), st_path.mstr_basic);

}

CString CMyBasicData::OnGet_File_Name()
{
	CString str_temp;       // 임시 저장 변수
	CString str_save_file;  // 저장 파일 임시 설정 변수
	CString str_chk_ext;    // 파일 확장자 저장 변수
	CString str_part, str_part2;
	CString str_new_save_file;
	int n_pos;
	
	// **************************************************************************
    // Socket Contact Count 데이터 저장할 파일 설정한다 [파일 확장자 검사]        
    // **************************************************************************
	str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 저장 파일 설정
	
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
		str_save_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정
			
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	// **************************************************************************

	return str_save_file;  // 파일명 리턴 
}

CString CMyBasicData::OnGet_Teach_File_Name(CString strName)
{
	CString str_temp;       // 임시 저장 변수
	CString str_save_file;  // 저장 파일 임시 설정 변수
	CString str_chk_ext;    // 파일 확장자 저장 변수
	CString str_part, str_part2;
	CString str_new_save_file;
	CString str_device_name;
	int n_pos;
	
	n_pos = st_basic.mstr_device_name.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
	{
		st_basic.mstr_device_name += _T(".DAT");  // 확장자 추가 
		n_pos = st_basic.mstr_device_name.Find(".");  // 확장자 위치 검사
	}
	
	str_device_name = st_basic.mstr_device_name.Mid(0, n_pos);
	
	
	if (strName.Find("Back") != -1)
	{
		str_save_file = st_path.mstr_path_back + str_device_name;  // 티칭 데이터 저장 파일 설정
	}
	else
	{
		str_save_file = st_path.mstr_path_dvc + str_device_name;  // 티칭 데이터 저장 파일 설정
	}
	
	if (strName != "")
	{
		str_save_file += "_";
		str_save_file += strName;
	}
	
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
	{
		str_save_file += _T(".DAT");  // 확장자 추가
	}
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".DAT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.DAT");  // 티칭 데이터 저장 새로운 파일 설정
			
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 오류 출력 요청
			}
		}
	}
	// **************************************************************************
	
	return str_save_file;  // 파일명 리턴 
}

void CMyBasicData::On_Teach_Data_Load()
{
	CString str_temp, str_msg, str_LoadFile;       // 임시 저장 변수
	int i;
	
	FILE    *fp ;
	int     existence;
	
	str_LoadFile = OnGet_Teach_File_Name("Teach");  // 데이터 저장 파일명 로딩 함수 
	
	existence = access(str_LoadFile, 0);
	
	if (!existence)
	{
		if ((fp=fopen(str_LoadFile,"rb")) == NULL)
		{
			if (st_handler.cwnd_list != NULL)
			{
//				st_msg.mstr_abnormal_msg.Format("[%s] file open error.", str_LoadFile);
				sprintf(st_msg.c_abnormal_msg, "[%s] file open error.", str_LoadFile);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return ;
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] file is not exist.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}
	
	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		fread(&st_motor[i].md_pos, sizeof(st_motor[i].md_pos), 1, fp);
	}
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_abnormal_msg.Format("[%s] FILE Read Error.", str_LoadFile);
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Read Error.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			clearerr(fp);
		}
	}

	fclose(fp) ;
}

void CMyBasicData::On_Teach_Data_Save()
{
	CString str_temp, str_SaveFile;			// 임시 저장 변수
	int i;
	
	str_SaveFile = OnGet_Teach_File_Name("Teach");		// 데이터 저장 파일명 로딩 함수 
	
	FILE  *fp;
	int   err = 0;
	
	if ((fp = fopen(str_SaveFile, "wb")) == NULL) 
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}
	
	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		fwrite(&st_motor[i].md_pos, sizeof(st_motor[i].md_pos), 1, fp);
	}
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		clearerr(fp);
	}
	
	fclose(fp);

	str_SaveFile = OnGet_Teach_File_Name("Teach_Back");		// 데이터 저장 파일명 로딩 함수 
		
	if ((fp = fopen(str_SaveFile, "wb")) == NULL) 
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "Fail in Creating [%s] FILE.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return;
	}
	
	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		fwrite(&st_motor[i].d_pos, sizeof(st_motor[i].d_pos), 1, fp);
	}
	
	if (ferror(fp)) 
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] FILE Write Error.", str_SaveFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		clearerr(fp);
	}
	
	fclose(fp);
}

void CMyBasicData::OnAdmin_Data_Load()
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_chk_ext;		// 파일 확장자 저장 변수
	CString mstr_temp, str_part;			// 저장할 정보 임시 저장 변수 
	char chr_data[50];
	int mn_chk = 0, mn_pos = 0, n_pos = 0, i = 0;
	double md_chk = 0;

	:: GetPrivateProfileString("ADMIN", "APCT_IP", "200.200.200.2", chr_data, 16, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거	
	st_Network[NETWORK_APCT].mstr_ip = mstr_temp;
	
	:: GetPrivateProfileString("ADMIN", "APCT_Port", "9999", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_Network[NETWORK_APCT].mstr_port = mstr_temp;

	:: GetPrivateProfileString("ADMIN", "APCT_Server_Port", "9999", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_Network[NETWORK_APCT].mstr_server_port = mstr_temp;

	:: GetPrivateProfileString("ADMIN", "FMachine_Server_Port", "9999", chr_data, 10, st_path.mstr_basic);
	mstr_temp = chr_data;
	mstr_temp.MakeUpper();     // 문자열 대문자로 변경
	mstr_temp.TrimLeft(' ');   // 좌측 문자열 공백 제거              
	mstr_temp.TrimRight(' ');  // 우측 문자열 공백 제거
	st_Network[NETWORK_NMACHINE].mstr_server_port = mstr_temp;
}

void CMyBasicData::On_WorkSite_Save()
{
	CString mstr_temp;
	CString str_save_file;
	char chr_buf[20];

	str_save_file = OnGet_File_Name();

	mstr_temp = LPCTSTR(_itoa(st_work.mn_shifter_work_site[0], chr_buf, 10));
	:: WritePrivateProfileString("WorkSiteData", "Shifter_Work_Site_0", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp = LPCTSTR(_itoa(st_work.mn_shifter_work_site[1], chr_buf, 10));
	:: WritePrivateProfileString("WorkSiteData", "Shifter_Work_Site_1", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp = LPCTSTR(_itoa(st_sync.mn_rbuffer_station[0], chr_buf, 10));
	:: WritePrivateProfileString("WorkSiteData", "RBuffer_Station_0", LPCTSTR(mstr_temp), str_save_file);

	mstr_temp = LPCTSTR(_itoa(st_sync.mn_rbuffer_station[1], chr_buf, 10));
	:: WritePrivateProfileString("WorkSiteData", "RBuffer_Station_1", LPCTSTR(mstr_temp), str_save_file);
}

void CMyBasicData::On_WorkSite_Load()
{
	CString str_load_file;
	CString str_temp;
	CString mstr_temp;
	int  n_chk;
	char chr_data[20];
	char chr_buf[20];

	str_load_file = OnGet_File_Name();

	:: GetPrivateProfileString("WorkSiteData", "Shifter_Work_Site_0", "1", chr_data, 10, str_load_file);
	n_chk = atoi(chr_data);
	if (n_chk < 1 || n_chk > 8)
	{
		st_work.mn_shifter_work_site[0] = 1;

		str_temp = LPCTSTR(_itoa(st_work.mn_shifter_work_site[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("WorkSiteData", "Shifter_Work_Site_0", LPCTSTR(mstr_temp), str_load_file);
	}
	else
		st_work.mn_shifter_work_site[0] = n_chk;

	:: GetPrivateProfileString("WorkSiteData", "Shifter_Work_Site_1", "1", chr_data, 10, str_load_file);
	n_chk = atoi(chr_data);
	if (n_chk < 1 || n_chk > 8)
	{
		st_work.mn_shifter_work_site[1] = 1;

		str_temp = LPCTSTR(_itoa(st_work.mn_shifter_work_site[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("WorkSiteData", "Shifter_Work_Site_1", LPCTSTR(mstr_temp), str_load_file);
	}
	else
		st_work.mn_shifter_work_site[1] = n_chk;

	:: GetPrivateProfileString("WorkSiteData", "RBuffer_Station_0", "0", chr_data, 10, str_load_file);
	n_chk = atoi(chr_data);
	if (n_chk < 0 || n_chk > 8)
	{
		st_sync.mn_rbuffer_station[0] = 0;

		str_temp = LPCTSTR(_itoa(st_sync.mn_rbuffer_station[0], chr_buf, 10)) ;
		:: WritePrivateProfileString("WorkSiteData", "RBuffer_Station_0", LPCTSTR(mstr_temp), str_load_file);
	}
	else
		st_sync.mn_rbuffer_station[0] = n_chk;

	:: GetPrivateProfileString("WorkSiteData", "RBuffer_Station_1", "0", chr_data, 10, str_load_file);
	n_chk = atoi(chr_data);
	if (n_chk < 0 || n_chk > 8)
	{
		st_sync.mn_rbuffer_station[1] = 0;

		str_temp = LPCTSTR(_itoa(st_sync.mn_rbuffer_station[1], chr_buf, 10)) ;
		:: WritePrivateProfileString("WorkSiteData", "RBuffer_Station_1", LPCTSTR(mstr_temp), str_load_file);
	}
	else
		st_sync.mn_rbuffer_station[1] = n_chk;
}

//////////////////////////////////////////// 20120525
void CMyBasicData::OnSkip_Data_Load()
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_chk_ext;		// 파일 확장자 저장 변수
	CString mstr_temp, str_part, str_station, str_board;			// 저장할 정보 임시 저장 변수 
	char chr_data[50];
	int mn_chk = 0, mn_pos = 0, n_pos = 0, i = 0, j = 0;
	double md_chk = 0;

	/* ************************************************************************** */
    /*  데이터 로딩할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */
	str_load_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 로딩 파일 설정
	
	n_pos = str_load_file.Find(".");  // 확장자 위치 검사

	if (n_pos == -1)
		str_load_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_load_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_load_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	/* ************************************************************************** */
	:: GetPrivateProfileString("SkipData", "module_loader1", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_module_ldrbt_enable[0] = 1;
		mstr_temp.Format("%d", st_picker.n_module_ldrbt_enable[0]);
		:: WritePrivateProfileString("SkipData", "module_loader1", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_module_ldrbt_enable[0] = mn_chk;

// 	st_picker.n_module_ldrbt_enable[0] = YES;

	:: GetPrivateProfileString("SkipData", "heatsink_left1", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_heat_leftldrbt_enable[0] = 1;
		mstr_temp.Format("%d", st_picker.n_heat_leftldrbt_enable[0]);
		:: WritePrivateProfileString("SkipData", "heatsink_left1", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_heat_leftldrbt_enable[0] = mn_chk;

// 	st_picker.n_heat_leftldrbt_enable[0] = YES;

	:: GetPrivateProfileString("SkipData", "heatsink_right1", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_heat_rightldrbt_enable[0] = 1;
		mstr_temp.Format("%d", st_picker.n_heat_rightldrbt_enable[0]);
		:: WritePrivateProfileString("SkipData", "heatsink_right1", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_heat_rightldrbt_enable[0] = mn_chk;

// 	st_picker.n_heat_rightldrbt_enable[0] = YES;

	:: GetPrivateProfileString("SkipData", "module_loader2", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_module_ldrbt_enable[1] = 1;
		mstr_temp.Format("%d", st_picker.n_module_ldrbt_enable[1]);
		:: WritePrivateProfileString("SkipData", "module_loader2", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_module_ldrbt_enable[1] = mn_chk;

// 	st_picker.n_module_ldrbt_enable[0] = YES;


	:: GetPrivateProfileString("SkipData", "heatsink_left2", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_heat_leftldrbt_enable[1] = 1;
		mstr_temp.Format("%d", st_picker.n_heat_leftldrbt_enable[1]);
		:: WritePrivateProfileString("SkipData", "heatsink_left2", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_heat_leftldrbt_enable[1] = mn_chk;

// 	st_picker.n_heat_leftldrbt_enable[0] = YES;

	:: GetPrivateProfileString("SkipData", "heatsink_right2", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_heat_rightldrbt_enable[1] = 1;
		mstr_temp.Format("%d", st_picker.n_heat_rightldrbt_enable[1]);
		:: WritePrivateProfileString("SkipData", "heatsink_right2", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_heat_rightldrbt_enable[1] = mn_chk;

// 	st_picker.n_heat_rightldrbt_enable[1] = YES;



	:: GetPrivateProfileString("SkipData", "clip", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_heat_clipldrbt_enable[1] = 1;
		mstr_temp.Format("%d", st_picker.n_heat_clipldrbt_enable[1]);
		:: WritePrivateProfileString("SkipData", "clip", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_heat_clipldrbt_enable[1] = mn_chk;

// 	st_picker.n_heat_clipldrbt_enable[1] = YES;

	:: GetPrivateProfileString("SkipData", "picker_robot", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_picker_ldrbt_enable[1] = 1;
		mstr_temp.Format("%d", st_picker.n_picker_ldrbt_enable[1]);
		:: WritePrivateProfileString("SkipData", "picker_robot", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_picker_ldrbt_enable[1] = mn_chk;

// 	st_picker.n_picker_ldrbt_enable[1] = YES;

	:: GetPrivateProfileString("SkipData", "label", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_label_enable = 1;
		mstr_temp.Format("%d", st_picker.n_label_enable);
		:: WritePrivateProfileString("SkipData", "label", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_label_enable = mn_chk;


///////20120912
	:: GetPrivateProfileString("SkipData", "vision_label", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_vision_eable[0] = 1;
		mstr_temp.Format("%d", st_picker.n_vision_eable[0]);
		:: WritePrivateProfileString("SkipData", "vision_label", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_vision_eable[0] = mn_chk;

	:: GetPrivateProfileString("SkipData", "vision_hs", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_vision_eable[1] = 1;
		mstr_temp.Format("%d", st_picker.n_vision_eable[1]);
		:: WritePrivateProfileString("SkipData", "vision_hs", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_vision_eable[1] = mn_chk;

///////
// 	st_picker.n_vision_ldrbt_eable[1] = YES;

	:: GetPrivateProfileString("SkipData", "work_buffer1", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_work_buffer_enable[0] = 1;
		mstr_temp.Format("%d", st_picker.n_work_buffer_enable[0]);
		:: WritePrivateProfileString("SkipData", "work_buffer1", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_work_buffer_enable[0] = mn_chk;

// 	st_picker.n_work_buffer1_eable[1] = YES;

	:: GetPrivateProfileString("SkipData", "work_buffer2", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_work_buffer_enable[1] = 1;
		mstr_temp.Format("%d", st_picker.n_work_buffer_enable[1]);
		:: WritePrivateProfileString("SkipData", "work_buffer2", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_work_buffer_enable[1] = mn_chk;

// 	st_picker.n_work_buffer2_eable[1] = YES;


	:: GetPrivateProfileString("SkipData", "work_buffer3", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_work_buffer_enable[2] = 1;
		mstr_temp.Format("%d", st_picker.n_work_buffer_enable[2]);
		:: WritePrivateProfileString("SkipData", "work_buffer3", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_work_buffer_enable[2] = mn_chk;

// 	st_picker.n_work_buffer3_eable[1] = YES;

	:: GetPrivateProfileString("SkipData", "work_buffer4", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_picker.n_work_buffer_enable[3] = 1;
		mstr_temp.Format("%d", st_picker.n_work_buffer_enable[3]);
		:: WritePrivateProfileString("SkipData", "work_buffer4", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_picker.n_work_buffer_enable[3] = mn_chk;

// 	st_picker.n_work_buffer4_eable[1] = YES;

}

void CMyBasicData::OnSkip_Data_Save()
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	CString str_save_file;
	CString str_part, str_chk_ext;
	COleDateTime time_cur;
	int n_pos; // 현재 시간정보 저장 변수 

	/* ************************************************************************** */
    /*  데이터 저장할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */
	str_save_file = st_path.mstr_path_dvc + st_basic.mstr_device_name;  // 티칭 데이터 저장 파일 설정
	
	n_pos = str_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
		str_save_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_save_file = st_path.mstr_path_dvc + _T("DEFAULT.TXT");  // 티칭 데이터 저장 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE FILE] The error happened at a file extension.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}
	/* ************************************************************************** */
	mstr_temp.Format("%d", st_picker.n_module_ldrbt_enable[0]);
	:: WritePrivateProfileString("SkipData", "module_loader1", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_picker.n_heat_leftldrbt_enable[0]);
	:: WritePrivateProfileString("SkipData", "heatsink_left1", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_picker.n_heat_rightldrbt_enable[0]);
	:: WritePrivateProfileString("SkipData", "heatsink_right1", LPCTSTR(mstr_temp), st_path.mstr_basic);


	mstr_temp.Format("%d", st_picker.n_module_ldrbt_enable[1]);
	:: WritePrivateProfileString("SkipData", "module_loader2", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_picker.n_heat_leftldrbt_enable[1]);
	:: WritePrivateProfileString("SkipData", "heatsink_left2", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_picker.n_heat_rightldrbt_enable[1]);
	:: WritePrivateProfileString("SkipData", "heatsink_right2", LPCTSTR(mstr_temp), st_path.mstr_basic);


	mstr_temp.Format("%d", st_picker.n_heat_clipldrbt_enable[1]);
	:: WritePrivateProfileString("SkipData", "clip", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_picker.n_picker_ldrbt_enable[1]);
	:: WritePrivateProfileString("SkipData", "picker_robot", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_picker.n_label_enable);
	:: WritePrivateProfileString("SkipData", "label", LPCTSTR(mstr_temp), st_path.mstr_basic);

////////////20120912
	mstr_temp.Format("%d", st_picker.n_vision_eable[0]);
	:: WritePrivateProfileString("SkipData", "vision_label", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_picker.n_vision_eable[1]);
	:: WritePrivateProfileString("SkipData", "vision_hs", LPCTSTR(mstr_temp), st_path.mstr_basic);
////////////

	mstr_temp.Format("%d", st_picker.n_work_buffer_enable[0]);
	:: WritePrivateProfileString("SkipData", "work_buffer1", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_picker.n_work_buffer_enable[1]);
	:: WritePrivateProfileString("SkipData", "work_buffer2", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_picker.n_work_buffer_enable[2]);
	:: WritePrivateProfileString("SkipData", "work_buffer3", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_picker.n_work_buffer_enable[3]);
	:: WritePrivateProfileString("SkipData", "work_buffer4", LPCTSTR(mstr_temp), st_path.mstr_basic);


}

//////////////////////////////////////////// 20120531
void CMyBasicData::OnBasic_Label_Data_Load()
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_chk_ext;		// 파일 확장자 저장 변수
	CString mstr_temp, str_part, str_station, str_board;			// 저장할 정보 임시 저장 변수 
	char chr_data1[50], chr_buf1[20];	//20120530
	int mn_chk = 0, mn_pos = 0, n_pos = 0, i = 0, j = 0;
	double md_chk = 0;

	/* ************************************************************************** */
    /*  데이터 로딩할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */

/////////////////////////////////////////////////////////// 20120530
	CString str_label_load_file;

	str_label_load_file = st_path.mstr_label_folder + st_basic.mstr_label_name;  // 티칭 데이터 로딩 파일 설정
	
	n_pos = str_label_load_file.Find(".");  // 확장자 위치 검사

	if (n_pos == -1)
		str_label_load_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_label_load_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_label_load_file = st_path.mstr_label_folder + _T("G2.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "[LABEL FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}

	if (st_barcode.b_barcode_first_load == TRUE)
	{
		st_barcode.b_barcode_first_load = FALSE;
		:: GetPrivateProfileString("BasicData", "Label_Mode", "0", chr_data1, 10, st_path.mstr_basic);
		mn_chk = atoi(chr_data1);
		st_basic.n_mode_label = mn_chk;

		:: GetPrivateProfileString("BasicData", "Label_Type", "", chr_data1, 20, st_path.mstr_basic);
		st_basic.mstr_label_name = (LPCTSTR)chr_data1;
	}
	else
	{
		:: GetPrivateProfileString("BasicData", "Label_Mode", "0", chr_data1, 10, str_label_load_file);
		mn_chk = atoi(chr_data1);
		st_basic.n_mode_label = mn_chk;

		:: GetPrivateProfileString("BasicData", "Label_Type", "", chr_data1, 20, str_label_load_file);
		st_basic.mstr_label_name = (LPCTSTR)chr_data1;		
	}

	:: GetPrivateProfileString("BasicData", "LabelType", "0", chr_data1, 10, str_label_load_file);
	mn_chk = atoi(chr_data1);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_label_type = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_label_type, chr_buf1, 10));
		:: WritePrivateProfileString("BasicData", "LabelType", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.mn_label_type = mn_chk;


	:: GetPrivateProfileString("Set Data", "SD", "0", chr_data1, 10, str_label_load_file);
	mn_chk = atoi(chr_data1);
	if (mn_chk < 0 || mn_chk > 99)
	{
		st_basic.mn_labelset_sd = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_labelset_sd, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "SD", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.mn_labelset_sd = mn_chk;

	:: GetPrivateProfileString("Set Data", "LT", "0", chr_data1, 10, str_label_load_file);
	mn_chk = atoi(chr_data1);
	if (mn_chk < 0 || mn_chk > 99)
	{
		st_basic.mn_labelset_lt = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_labelset_lt, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "LT", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.mn_labelset_lt = mn_chk;


	:: GetPrivateProfileString("Set Data", "LH_X", "0", chr_data1, 10, str_label_load_file);
	mn_chk = atoi(chr_data1);
	if (mn_chk < 0 || mn_chk > 40)
	{
		st_basic.mn_labelset_lh1 = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_labelset_lh1, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "LH_X", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.mn_labelset_lh1 = mn_chk;

	:: GetPrivateProfileString("Set Data", "LH_Y", "0", chr_data1, 10, str_label_load_file);
	mn_chk = atoi(chr_data1);
	if (mn_chk < 0 || mn_chk > 40)
	{
		st_basic.mn_labelset_lh2 = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_labelset_lh2, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "LH_Y", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.mn_labelset_lh2 = mn_chk;


//////////20120906
	:: GetPrivateProfileString("Set Data", "X1_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x1_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x1_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X1_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x1_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "X2_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x2_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x2_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X2_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else st_basic.md_labelset_x2_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "X3_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x3_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x3_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X3_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x3_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "X4_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x4_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x4_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X4_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x4_pos = md_chk;
	
	:: GetPrivateProfileString("Set Data", "X5_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x5_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x5_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X5_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x5_pos = md_chk;
	
	:: GetPrivateProfileString("Set Data", "X6_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x6_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x6_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X6_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x6_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y1_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y1_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y1_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y1_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y1_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y2_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y2_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y2_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y2_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y2_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y3_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y3_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y3_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y3_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y3_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y4_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y4_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y4_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y4_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y4_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y5_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y5_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y5_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y5_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y5_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y6_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y6_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y6_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y6_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y6_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "X1-WIDTH", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x1_width = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x1_width, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X1-WIDTH", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x1_width = md_chk;

	:: GetPrivateProfileString("Set Data", "X2-WIDTH", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x2_width = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x2_width, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X2-WIDTH", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x2_width = md_chk;

	:: GetPrivateProfileString("Set Data", "X3-WIDTH", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x3_width = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x3_width, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X3-WIDTH", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x3_width = md_chk;

	:: GetPrivateProfileString("Set Data", "X4-WIDTH", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x4_width = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x4_width, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X4-WIDTH", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x4_width = md_chk;

	:: GetPrivateProfileString("Set Data", "Y1-HEIGHT", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y1_height = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y1_height, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y1-HEIGHT", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y1_height = md_chk;

	:: GetPrivateProfileString("Set Data", "Y2-HEIGHT", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y2_height = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y2_height, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y2-HEIGHT", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y2_height = md_chk;

	:: GetPrivateProfileString("Set Data", "Y3-HEIGHT", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y3_height = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y3_height, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y3-HEIGHT", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y3_height = md_chk;

	:: GetPrivateProfileString("Set Data", "Y4-HEIGHT", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y4_height = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y4_height, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y4-HEIGHT", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y4_height = md_chk;
//////////
//////////////////////////////////// 20120924
	:: GetPrivateProfileString("BasicData", "Old_LotNo", "", chr_data1, 20, str_label_load_file);
	st_NW.mstr_Old_LotNo = (LPCTSTR)chr_data1;
//////////////////////////////////// 

}

void CMyBasicData::OnBasic_Label_Data_Save()
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	CString str_save_file;
	CString str_part, str_chk_ext;
	COleDateTime time_cur;
	int n_pos; // 현재 시간정보 저장 변수 

	/* ************************************************************************** */
    /*  데이터 저장할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */

//////////////////////////////////////////////// 20120530
	CString str_label_save_file;

	str_label_save_file = st_path.mstr_label_folder + st_basic.mstr_label_name;  // 티칭 데이터 저장 파일 설정
	
	n_pos = str_label_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
		str_label_save_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_label_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_label_save_file = st_path.mstr_label_folder + _T("G2.TXT");  // 티칭 데이터 저장 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "[LABEL FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}

	mstr_temp.Format("%s", st_basic.mstr_label_name);	
	:: WritePrivateProfileString("BasicData", "Label_Type", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_mode_label);
	:: WritePrivateProfileString("BasicData", "Label_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%s", st_basic.mstr_label_name);	
	:: WritePrivateProfileString("BasicData", "Label_Type", LPCTSTR(mstr_temp), str_label_save_file);

	mstr_temp.Format("%d", st_basic.n_mode_label);
	:: WritePrivateProfileString("BasicData", "Label_Mode", LPCTSTR(mstr_temp), str_label_save_file);
	
	:: WritePrivateProfileString("BasicData", "LabelType", LPCTSTR(mstr_temp), str_label_save_file);

	mstr_temp.Format("%d",st_basic.mn_labelset_sd);
	:: WritePrivateProfileString("Set Data", "SD", LPCTSTR(mstr_temp), str_label_save_file);

	mstr_temp.Format("%d",st_basic.mn_labelset_lt);
	:: WritePrivateProfileString("Set Data", "LT", LPCTSTR(mstr_temp), str_label_save_file);

	mstr_temp.Format("%d",st_basic.mn_labelset_lh1);
	:: WritePrivateProfileString("Set Data", "LH_X", LPCTSTR(mstr_temp), str_label_save_file);

	mstr_temp.Format("%d",st_basic.mn_labelset_lh2);
	:: WritePrivateProfileString("Set Data", "LH_Y", LPCTSTR(mstr_temp), str_label_save_file);
	
	////////////////// 20120906
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x1_pos);
	:: WritePrivateProfileString("Set Data", "X1_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x2_pos);
	:: WritePrivateProfileString("Set Data", "X2_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x3_pos);
	:: WritePrivateProfileString("Set Data", "X3_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x4_pos);
	:: WritePrivateProfileString("Set Data", "X4_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x5_pos);
	:: WritePrivateProfileString("Set Data", "X5_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x6_pos);
	:: WritePrivateProfileString("Set Data", "X6_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y1_pos);
	:: WritePrivateProfileString("Set Data", "Y1_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y2_pos);
	:: WritePrivateProfileString("Set Data", "Y2_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y3_pos);
	:: WritePrivateProfileString("Set Data", "Y3_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y4_pos);
	:: WritePrivateProfileString("Set Data", "Y4_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y5_pos);
	:: WritePrivateProfileString("Set Data", "Y5_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y6_pos);
	:: WritePrivateProfileString("Set Data", "Y6_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x1_width);
	:: WritePrivateProfileString("Set Data", "X1-WIDTH", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x2_width);
	:: WritePrivateProfileString("Set Data", "X2-WIDTH", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x3_width);
	:: WritePrivateProfileString("Set Data", "X3-WIDTH", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x4_width);
	:: WritePrivateProfileString("Set Data", "X4-WIDTH", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y1_height);
	:: WritePrivateProfileString("Set Data", "Y1-HEIGHT", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y2_height);
	:: WritePrivateProfileString("Set Data", "Y2-HEIGHT", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y3_height);
	:: WritePrivateProfileString("Set Data", "Y3-HEIGHT", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y4_height);
	:: WritePrivateProfileString("Set Data", "Y4-HEIGHT", LPCTSTR(mstr_temp), str_label_save_file);
	//////////////////
//////////////////////////////////// 20120924
	mstr_temp.Format("%s", st_NW.mstr_Old_LotNo);	
	:: WritePrivateProfileString("BasicData", "Old_LotNo", LPCTSTR(mstr_temp), str_label_save_file);
//////////////////////////////////// 

}

//////////////////////////////////////////// 20120531
void CMyBasicData::OnBarcode_Label_Data_Load()
{
	CString str_load_device;	// 로딩 디바이스명 저장 변수
	CString str_load_file;
	CString str_chk_ext;		// 파일 확장자 저장 변수
	CString mstr_temp, str_part, str_station, str_board;			// 저장할 정보 임시 저장 변수 
	char chr_data1[50], chr_buf1[20];	//20120530
	int mn_chk = 0, mn_pos = 0, n_pos = 0, i = 0, j = 0;
	double md_chk = 0;

	/* ************************************************************************** */
    /*  데이터 로딩할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */

/////////////////////////////////////////////////////////// 20120530
	CString str_label_load_file;
	
	str_label_load_file = st_path.mstr_label_folder + st_basic.mstr_label_name;  // 티칭 데이터 로딩 파일 설정
	
	n_pos = str_label_load_file.Find(".");  // 확장자 위치 검사

	if (n_pos == -1)
		str_label_load_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_label_load_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_label_load_file = st_path.mstr_label_folder + _T("G2.TXT");  // 로딩 로봇 X축 티칭 데이터 로딩 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "[LABEL FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}

	if (st_barcode.b_barcode_first_load == TRUE)
	{
		st_barcode.b_barcode_first_load = FALSE;
		:: GetPrivateProfileString("BasicData", "Label_Mode", "0", chr_data1, 10, st_path.mstr_basic);
		mn_chk = atoi(chr_data1);
		st_basic.n_mode_label = mn_chk;

		:: GetPrivateProfileString("BasicData", "Label_Type", "", chr_data1, 20, st_path.mstr_basic);
		st_basic.mstr_label_name = (LPCTSTR)chr_data1;
	}
	else
	{
		:: GetPrivateProfileString("BasicData", "Label_Mode", "0", chr_data1, 10, str_label_load_file);
		mn_chk = atoi(chr_data1);
		st_basic.n_mode_label = mn_chk;

		:: GetPrivateProfileString("BasicData", "Label_Type", "", chr_data1, 20, str_label_load_file);
		st_basic.mstr_label_name = (LPCTSTR)chr_data1;		
	}



	:: GetPrivateProfileString("BasicData", "LabelType", "0", chr_data1, 10, str_label_load_file);
	mn_chk = atoi(chr_data1);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_basic.mn_label_type = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_label_type, chr_buf1, 10));
		:: WritePrivateProfileString("BasicData", "LabelType", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.mn_label_type = mn_chk;


	:: GetPrivateProfileString("Set Data", "SD", "0", chr_data1, 10, str_label_load_file);
	mn_chk = atoi(chr_data1);
	if (mn_chk < 0 || mn_chk > 99)
	{
		st_basic.mn_labelset_sd = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_labelset_sd, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "SD", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.mn_labelset_sd = mn_chk;

	:: GetPrivateProfileString("Set Data", "LT", "0", chr_data1, 10, str_label_load_file);
	mn_chk = atoi(chr_data1);
	if (mn_chk < 0 || mn_chk > 99)
	{
		st_basic.mn_labelset_lt = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_labelset_lt, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "LT", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.mn_labelset_lt = mn_chk;


	:: GetPrivateProfileString("Set Data", "LH_X", "0", chr_data1, 10, str_label_load_file);
	mn_chk = atoi(chr_data1);
	if (mn_chk < 0 || mn_chk > 40)
	{
		st_basic.mn_labelset_lh1 = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_labelset_lh1, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "LH_X", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.mn_labelset_lh1 = mn_chk;

	:: GetPrivateProfileString("Set Data", "LH_Y", "0", chr_data1, 10, str_label_load_file);
	mn_chk = atoi(chr_data1);
	if (mn_chk < 0 || mn_chk > 40)
	{
		st_basic.mn_labelset_lh2 = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.mn_labelset_lh2, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "LH_Y", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.mn_labelset_lh2 = mn_chk;


//////////// 20120906
	:: GetPrivateProfileString("Set Data", "X1_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x1_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x1_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X1_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x1_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "X2_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x2_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x2_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X2_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x2_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "X3_POS", "0", chr_data1, 10, str_label_load_file);
	if (md_chk < -999.99 || md_chk > 999.99)
	if (md_chk < 0 || md_chk > 9999)
	{
		st_basic.md_labelset_x3_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x3_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X3_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x3_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "X4_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x4_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x4_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X4_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x4_pos = md_chk;
	
	:: GetPrivateProfileString("Set Data", "X5_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x5_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x5_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X5_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x5_pos = md_chk;
	
	:: GetPrivateProfileString("Set Data", "X6_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x6_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x6_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X6_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x6_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y1_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y1_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y1_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y1_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y1_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y2_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y2_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y2_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y2_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y2_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y3_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y3_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y3_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y3_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y3_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y4_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y4_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y4_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y4_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y4_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y5_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y5_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y5_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y5_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y5_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "Y6_POS", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y6_pos = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y6_pos, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y6_POS", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y6_pos = md_chk;

	:: GetPrivateProfileString("Set Data", "X1-WIDTH", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x1_width = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x1_width, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X1-WIDTH", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x1_width = md_chk;

	:: GetPrivateProfileString("Set Data", "X2-WIDTH", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x2_width = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x2_width, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X2-WIDTH", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x2_width = md_chk;

	:: GetPrivateProfileString("Set Data", "X3-WIDTH", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x3_width = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x3_width, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X3-WIDTH", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x3_width = md_chk;

	:: GetPrivateProfileString("Set Data", "X4-WIDTH", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_x4_width = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_x4_width, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "X4-WIDTH", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_x4_width = md_chk;

	:: GetPrivateProfileString("Set Data", "Y1-HEIGHT", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y1_height = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y1_height, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y1-HEIGHT", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y1_height = md_chk;

	:: GetPrivateProfileString("Set Data", "Y2-HEIGHT", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y2_height = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y2_height, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y2-HEIGHT", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y2_height = md_chk;

	:: GetPrivateProfileString("Set Data", "Y3-HEIGHT", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y3_height = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y3_height, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y3-HEIGHT", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y3_height = md_chk;

	:: GetPrivateProfileString("Set Data", "Y4-HEIGHT", "0", chr_data1, 10, str_label_load_file);
	md_chk = atof(chr_data1);
	if (md_chk < -999.99 || md_chk > 999.99)
	{
		st_basic.md_labelset_y4_height = 0;
		mstr_temp = LPCTSTR(_itoa(st_basic.md_labelset_y4_height, chr_buf1, 10));
		:: WritePrivateProfileString("Set Data", "Y4-HEIGHT", LPCTSTR(mstr_temp), str_label_load_file);
	}
	else  st_basic.md_labelset_y4_height = md_chk;
/////////////

 	:: GetPrivateProfileString("BasicData", "PRINT_COMPLETE", "0", chr_data1, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data1);
	st_barcode.n_Prt_Comp_Reply = mn_chk;

	:: GetPrivateProfileString("BasicData", "PRINT_MODE", "0", chr_data1, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data1);
	st_barcode.n_Prt_mode = mn_chk;

//////////////////////////////////// 20120924
	:: GetPrivateProfileString("BasicData", "Old_LotNo", "", chr_data1, 20, st_path.mstr_basic);
	st_NW.mstr_Old_LotNo = (LPCTSTR)chr_data1;
//////////////////////////////////// 

}

void CMyBasicData::OnBarcode_Label_Data_Save()
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	CString str_save_file;
	CString str_part, str_chk_ext;
	//char chr_buf[20];
	COleDateTime time_cur;
	int n_pos; // 현재 시간정보 저장 변수 mn_cur_day, mn_cur_hour,mn_cur_month,  mn_cur_year, 

	/* ************************************************************************** */
    /*  데이터 저장할 파일 설정한다 [파일 확장자 검사]                            */
    /* ************************************************************************** */

//////////////////////////////////////////////// 20120530
	CString str_label_save_file;

	str_label_save_file = st_path.mstr_label_folder + st_basic.mstr_label_name;  // 티칭 데이터 저장 파일 설정
	
	n_pos = str_label_save_file.Find(".");  // 확장자 위치 검사
	
	if (n_pos == -1)
		str_label_save_file += _T(".TXT");  // 확장자 추가
	else 
	{
		str_chk_ext = str_label_save_file.Mid(n_pos);  // 파일 확장자 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			str_label_save_file = st_path.mstr_label_folder + _T("G2.TXT");  // 티칭 데이터 저장 새로운 파일 설정

			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "[LABEL FILE] The error happened at a file extension.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
	}

	mstr_temp.Format("%s", st_basic.mstr_label_name);	
	:: WritePrivateProfileString("BasicData", "Label_Type", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_basic.n_mode_label);
	:: WritePrivateProfileString("BasicData", "Label_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%s", st_basic.mstr_label_name);	
	:: WritePrivateProfileString("BasicData", "Label_Type", LPCTSTR(mstr_temp), str_label_save_file);

	mstr_temp.Format("%d", st_basic.n_mode_label);
	:: WritePrivateProfileString("BasicData", "Label_Mode", LPCTSTR(mstr_temp), str_label_save_file);
	
	:: WritePrivateProfileString("BasicData", "LabelType", LPCTSTR(mstr_temp), str_label_save_file);

	mstr_temp.Format("%d",st_basic.mn_labelset_sd);
	:: WritePrivateProfileString("Set Data", "SD", LPCTSTR(mstr_temp), str_label_save_file);

	mstr_temp.Format("%d",st_basic.mn_labelset_lt);
	:: WritePrivateProfileString("Set Data", "LT", LPCTSTR(mstr_temp), str_label_save_file);

	mstr_temp.Format("%d",st_basic.mn_labelset_lh1);
	:: WritePrivateProfileString("Set Data", "LH_X", LPCTSTR(mstr_temp), str_label_save_file);

	mstr_temp.Format("%d",st_basic.mn_labelset_lh2);
	:: WritePrivateProfileString("Set Data", "LH_Y", LPCTSTR(mstr_temp), str_label_save_file);
	
////////// 20120906
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x1_pos);
	:: WritePrivateProfileString("Set Data", "X1_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x2_pos);
	:: WritePrivateProfileString("Set Data", "X2_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x3_pos);
	:: WritePrivateProfileString("Set Data", "X3_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x4_pos);
	:: WritePrivateProfileString("Set Data", "X4_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x5_pos);
	:: WritePrivateProfileString("Set Data", "X5_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x6_pos);
	:: WritePrivateProfileString("Set Data", "X6_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y1_pos);
	:: WritePrivateProfileString("Set Data", "Y1_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y2_pos);
	:: WritePrivateProfileString("Set Data", "Y2_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y3_pos);
	:: WritePrivateProfileString("Set Data", "Y3_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y4_pos);
	:: WritePrivateProfileString("Set Data", "Y4_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y5_pos);
	:: WritePrivateProfileString("Set Data", "Y5_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y6_pos);
	:: WritePrivateProfileString("Set Data", "Y6_POS", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x1_width);
	:: WritePrivateProfileString("Set Data", "X1-WIDTH", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x2_width);
	:: WritePrivateProfileString("Set Data", "X2-WIDTH", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x3_width);
	:: WritePrivateProfileString("Set Data", "X3-WIDTH", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_x4_width);
	:: WritePrivateProfileString("Set Data", "X4-WIDTH", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y1_height);
	:: WritePrivateProfileString("Set Data", "Y1-HEIGHT", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y2_height);
	:: WritePrivateProfileString("Set Data", "Y2-HEIGHT", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y3_height);
	:: WritePrivateProfileString("Set Data", "Y3-HEIGHT", LPCTSTR(mstr_temp), str_label_save_file);
	
	mstr_temp.Format("%0.2f",st_basic.md_labelset_y4_height);
	:: WritePrivateProfileString("Set Data", "Y4-HEIGHT", LPCTSTR(mstr_temp), str_label_save_file);
//////////

	mstr_temp.Format("%d", st_barcode.n_Prt_Comp_Reply);
	:: WritePrivateProfileString("BasicData", "PRINT_COMPLETE", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d", st_barcode.n_Prt_mode);
	:: WritePrivateProfileString("BasicData", "PRINT_MODE", LPCTSTR(mstr_temp), st_path.mstr_basic);

//////////////////////////////////// 20120924
	mstr_temp.Format("%s", st_NW.mstr_Old_LotNo);	
	:: WritePrivateProfileString("BasicData", "Old_LotNo", LPCTSTR(mstr_temp), st_path.mstr_basic);
//////////////////////////////////// 

}

void CMyBasicData::OnTray_Data_Load()
{
	CString str_load_file;  // 로딩 파일 임시 저장 변수
	CString str_temp, strTrayIndex;       // 임시 저장 변수
	int  iChk, iMax, iTrayIndex, /*iCol,*/ iRow;
	int k=0;

	char chr_data[20];

	str_load_file = OnGet_File_Name();

	::GetPrivateProfileString("Tray_Data", "Loader_Row", "-1", chr_data, 10, str_load_file);
	iChk = atoi(chr_data);
	if(iChk < 0 || iChk > 30)
	{
		for(k = 0; k < MAX_SITE_INFO; k++)
		{
			st_traybuffer[k].i_loader_row = 25;
		}
		::WritePrivateProfileString("Tray_Data", "Loader_Row", _T("25"), str_load_file);
	}
	else
	{
		for(k = 0; k < MAX_SITE_INFO; k++)
		{
			st_traybuffer[k].i_loader_row = iChk;
		}
	}

	iMax = st_traybuffer[LDMODULE_SITE].i_loader_row;
	for(iTrayIndex = 0 ; iTrayIndex < iMax ; iTrayIndex++)
	{
		strTrayIndex.Format("Loader_State_%02d", iTrayIndex + 1);

		iRow = iTrayIndex % st_traybuffer[LDMODULE_SITE].i_loader_row;

		::GetPrivateProfileString("Load_TraySet", strTrayIndex, "0", chr_data, 10, str_load_file);
		iChk = atoi(chr_data);
		if(iChk < 1 || iChk > 4)
		{
			st_modulemap.LoadTrayState[iRow] = TRAYROW_ON;
			:: WritePrivateProfileString("Load_TraySet", strTrayIndex, _T("1"), str_load_file);
		}
		else
		{
			st_modulemap.LoadTrayState[iRow] = iChk;
		}
	}


}

void CMyBasicData::OnTray_Data_Save()
{
	CString str_save_file;  // 로딩 파일 임시 저장 변수
	CString str_temp, strTrayIndex;       // 임시 저장 변수
	int iMax, iTrayIndex, /*iCol,*/ iRow;

	char chr_buf[20];

	str_save_file = OnGet_File_Name();

	str_temp = LPCTSTR(_itoa(st_basic.n_tray_y, chr_buf, 10));
	::WritePrivateProfileString("Tray_Data", "Loader_Row", LPCTSTR(str_temp), str_save_file);

	//loader
	iMax = st_traybuffer[LDMODULE_SITE].i_loader_row;
	for(iTrayIndex = 0 ; iTrayIndex < iMax ; iTrayIndex++)
	{
		strTrayIndex.Format("Loader_State_%02d", iTrayIndex + 1);

		iRow = iTrayIndex % st_traybuffer[LDMODULE_SITE].i_loader_row;

		str_temp = LPCTSTR(_itoa(st_modulemap.LoadTrayState[iRow], chr_buf, 10));
		::WritePrivateProfileString("Load_TraySet", strTrayIndex, LPCTSTR(str_temp), str_save_file);
	}

}

void CMyBasicData::OnBasic_Part_Number_Load()
{
	int i = 0;

	/////////////////////////////////////////////////////////// 20120530
	CFile file;
	CString str_temp;

	file.Open(_T(st_path.mstr_part),CFile::modeRead);
	
	CArchive ar(&file, CArchive::load);

	while(ar.ReadString(str_temp))
	{
		st_basic.mstr_part_number[i] = str_temp;
		i++;
	}		
	ar.Close();
	file.Close();

}

void CMyBasicData::OnBasic_Part_Number_Save()
{
}

