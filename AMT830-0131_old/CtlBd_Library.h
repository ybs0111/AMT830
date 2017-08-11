// CtlBd_Library.h: interface for the CCtlBd_Library class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_)
#define AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//����� �������� ���� ����  
#define CTL_dSTOP			0
#define CTL_dRUN			1
#define CTL_dJAM			2
#define CTL_dLOTEND			3
#define CTL_dINIT			4
#define CTL_dWARNING		5
#define CTL_dLOCK			6
#define CTL_dSELFCHECK		7
#define CTL_dMAINT			8


class CCtlBd_Library  
{
public:
	CCtlBd_Library();
	virtual ~CCtlBd_Library();

public:
	//linear move  james 2012.0509 
	int mn_linear_motmove_step[8]; //2012.0307 ���� �̵������� ���� ->[100]�� ���� 100���� ���� 
	int mn_linear_retry_cnt[8]; //�� ���ͺ� ��Ʈ���� Ƚ�� 
	int mn_linear_max_retry_cnt;  //��� ������ ���� ��Ʈ���� ����Ʈ Ƚ�� 

	int mn_single_motmove_step[100]; //2012.0307 ���� �̵������� ���� ->[100]�� ���� 100���� ���� 
	int mn_mot_retry_cnt[100]; //�� ���ͺ� ��Ʈ���� Ƚ�� 
	int mn_mot_max_retry_cnt;  //��� ������ ���� ��Ʈ���� ����Ʈ Ƚ�� 

	char mc_normal_msg[1000]; //��񿡼� �߻��ϴ� �޼����� �����Ѵ� 

	int mn_run_status;	//����� ���� ���� ���� 
	
	char mc_alarmcode[10]; //alarm code ����
	int mn_alarm_type;	//�˶��� ������ ����[��)operator error ����, maint �� error ���� ����]
	int mn_alarm_count; //�˶� ī��Ʈ ���� ���� [0:ī��Ʈ���� ����, 1:ī��Ʈ ��]
	int mn_jampos; //jam �� �߻��� ��ġ�� ������ �����Ѵ� 

public:
	int Linear_Move(int n_LinearMapIndex, long l_AxisCnt, long *lp_AxisNum, double *dp_PosList, double *dp_SpdRatio);
	int Single_Move(int n_MotNum, double d_MotPos, int n_SpeedRate);
	
	BOOL CreateFolder(LPCTSTR sz_path);  //folder ���� �ʿ�� ��� 
	int FileSizeCheck(char * cp_filename, int n_size, int n_check);
	int LogFile_Write(char * cp_dir_path, char * cp_msg);

	
	int Initialize_motor_board(int n_bd_type, CString s_filename);	//motor ���� �ʱ�ȭ�� ��� 
	int Initialize_io_Board(int n_bd_type);	//i/o ���� �ʱ�ȭ�� ��� 

	int Motor_SafetyCheck(int n_mode, int n_axis, double d_targetpos);	//���� ���� ���� ���ý� ��� 
	int Alarm_Error_Occurrence(int n_jampos, int n_run_status, char c_alarmcode[10]); //��� �˶� �߻���ų�� ����ϴ� ���� 
	int OnAlarm( CString strCode );
	int ChekSafetyWorkBuffer(double d_targetpos, int nPos);

	



	//	int alarm_func(int n_mode, char * cp_jamcode, char * cp_msg);
	//int char_msg_func(int n_mode, char * cp_source_msg); //2011.0910 

};
extern CCtlBd_Library CTL_Lib;


#endif // !defined(AFX_CTLBD_LIBRARY_H__95978C35_3D8D_44B9_A9DF_E9F5E1C39135__INCLUDED_)
