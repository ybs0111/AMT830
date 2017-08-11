#if !defined(AFX_DIALOG_MANUAL_BACKHS_H__50360AA1_352B_4427_8FA3_D37195F9B6DB__INCLUDED_)
#define AFX_DIALOG_MANUAL_BACKHS_H__50360AA1_352B_4427_8FA3_D37195F9B6DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Manual_BackHs.h : header file
//
#include "LedButton.h"
#include "Digit.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_BackHs dialog
#define TMR_BACKHSMOT_CMD	950
#define TMR_BACKHSBUFFERMOT_CMD	951
#define TMR_BACKHSSTEPMOT_CMD		952
#define TMR_BACKHS_IO_MONITOR_CMD	953

class CDialog_Manual_BackHs : public CDialog
{
// Construction
public:
	CDialog_Manual_BackHs(CWnd* pParent = NULL);   // standard constructor
//////////////////////////////////////////////////// 20121006
	GridControlAlg	m_p_grid;	
	TSpread			*m_grid_motor_pos;
	TSpread			*m_grid_motor_info;
	int				m_n_home;
	int				m_n_sd;
	int				m_n_plus_el;
	int				m_n_minus_el;
	double			m_d_data;
	int				m_n_position_num;
	CString			m_str_position_name[25];
	CString			m_str_axis_name;
	CString			m_str_cmd_motor;
	double			m_d_mot_goto_spd;
	double			m_d_mot_goto_pos; // ���� �̵� �Ÿ�
	double			m_d_position[25][2];
	int				m_n_axis;
	int				m_n_part;
	int				m_n_direction;
	int				m_n_account;
	int				m_n_move_mode;
	int				m_n_cmd_no;			// ���� ��� ��ȣ ���� ���� �ʱ�ȭ
	int				m_n_cmd_motor_no;	// ���� ��ȣ ���� ���� �ʱ�ȭ
	
////////////////////////////////////////////////////
// Dialog Data
	//{{AFX_DATA(CDialog_Manual_BackHs)
	enum { IDD = IDD_DIALOG_MANUAL_BACKHS };
	CMacButton	m_btn_stop;
	CButtonST	m_btn_right;
	CButtonST	m_btn_home;
	CButtonST	m_btn_left;
	CXPGroupBox	m_group_motormove;
	CXPGroupBox	m_group_motor;
	CComboBox	m_cmb_motor_pos;
	CMacButton	m_btn_guide2_onoff;
	CGradientStatic	m_msg_buffer4;
	CGradientStatic	m_msg_buffer3;
	CGradientStatic	m_msg_buffer2;
	CGradientStatic	m_msg_buffer1;
	CGradientStatic	m_msg_buffer_hs;
	CLedButton	m_io_guide2_onoff;
	CLedButton	m_io_clamp2_onoff;
	CLedButton	m_io_clamp1_onoff;
	CComboBox	m_cmb_tray_pos;
	CComboBox   m_cmb_b_hs_pos;
	CComboBox	m_cmb_buffer_pos;
	CMacButton	m_btn_clamp2_onoff;
	CMacButton	m_btn_clamp1_onoff;
	CMacButton	m_btn_go_tray_pos;
	CMacButton	m_btn_go_step_rbt_move;
	CMacButton	m_btn_go_buffer_pos;
	CMacButton	m_btn_go_b_hs_pos;
	CButtonST	m_btn_finger_sol_all;
	CButtonST	m_btn_finger_sol2;
	CButtonST	m_btn_finger_sol1;
	CButtonST	m_btn_picker_sol_all;
	CButtonST	m_btn_picker_sol2;
	CButtonST	m_btn_picker_sol1;
	CButtonST	m_btn_buffer_backhs_sol1;
	CButtonST	m_btn_buffer_backhs_sol2;
	CButtonST	m_btn_buffer_backhs_sol3;
	CButtonST	m_btn_buffer_backhs_sol4;
	CGradientStatic	m_msg_finger_sol;
	CGradientStatic	m_msg_picker_sol;
	CGradientStatic	m_msg_picker2;
	CGradientStatic	m_msg_picker1;
	CXPGroupBox	m_group_buffer_rbt_move;
	CXPGroupBox	m_group_buffer;
	CXPGroupBox	m_group_tray_pos_clamp_guide;
	CXPGroupBox	m_group_step_rbt_move;
	CXPGroupBox	m_group_picker_rbt_move;
	CXPGroupBox	m_group_picker;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Manual_BackHs)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CFont* m_p_font;	// ��Ʈ ���� ���� ����
	CFont* m_load_combo_font;
	CFont* m_motor_font;	// ��Ʈ ���� ���� ����

	int mn_picker[2];
	int mn_finger[2];
	int mn_backhs_buffer[4];

	int mn_move_step;
	int mn_home_step;
	int mn_stop_req;
	int mn_cmd_no;
	int mn_motor_no;
	double md_mot_start_pos;
	int mn_io_clamp1_onoff;
	int mn_io_clamp2_onoff;
	int mn_io_guide2_onoff;
	void Init_GroupBox_Set();
	void Init_Label();
	void Init_Data();
	void Init_Button();
	void OnSite_Controls_Enable(BOOL iFlag);
	void OnSite_Step_Clear();
	void OnSite_RadioButton_Set();
	void OnSiteRbt_Button_Status();
	void OnRead_IO_Check();
	void OnSite_IO_Button_Status();
	void OnSite_IO_Status_Read();
	int OnSite_Homing_Excution();
	int OnSite_Move_Excution();
///////////////////////////////20121006
	void Init_Grid_Info();
	void OnButtonControl(BOOL b_flag);
	void MotorMoveStepClear();
	void Run_Init();
	void Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	int Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	void Set_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM]);
	int Get_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM]);
	void	MotorPositionCheck();
	void	MotorStatusCheck();

	BOOL b_PickerFlag[PICKER_NUM];
	long l_PickerWaitTime[PICKER_NUM][3];
	long l_UntilWaitTime[3];
	BOOL b_FingerFlag[PICKER_NUM];
	long l_FingerWaitTime[PICKER_NUM][3];
	int InitStep;
	int ActionPicker[D_INFOSIZE][PICKER_NUM];
	long l_until_wait_time[3];

///////////////////////////////
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Manual_BackHs)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnGoTrayPos();
	afx_msg void OnBtnGoBufferPos();
	afx_msg void OnBtnClamp1Onoff();
	afx_msg void OnBtnClamp2Onoff();
	afx_msg void OnBtnGuide2Onff();
	afx_msg void OnBtnPickerSol1();
	afx_msg void OnBtnPickerSol2();
	afx_msg void OnBtnPickerSolAll();
	afx_msg void OnBtnFingerSol1();
	afx_msg void OnBtnFingerSol2();
	afx_msg void OnBtnFingerSolAll();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnStop();
	afx_msg void OnBtnHome();
	afx_msg void OnBtnGoStepRbtMove();
	afx_msg void OnSelchangeCmbMotorPos();
	afx_msg void OnBtnBufferBackhsSol1();
	afx_msg void OnBtnBufferBackhsSol2();
	afx_msg void OnBtnBufferBackhsSol3();
	afx_msg void OnBtnBufferBackhsSol4();
	afx_msg void OnBtnGoBHsPos();
	//}}AFX_MSG
/////////////////////////////////////////////////////////////////20121006
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	LONG OnUserLButtonDown(WPARAM wParam, LPARAM lParam);
	LONG OnUserLButtonUp(WPARAM wParam, LPARAM lParam);
/////////////////////////////////////////////////////////////////
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_MANUAL_BACKHS_H__50360AA1_352B_4427_8FA3_D37195F9B6DB__INCLUDED_)
