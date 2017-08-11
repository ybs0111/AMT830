#if !defined(AFX_DIALOG_MANUAL_FRONTHS_H__75A07FA6_4EEF_4928_82E2_B102B12A9732__INCLUDED_)
#define AFX_DIALOG_MANUAL_FRONTHS_H__75A07FA6_4EEF_4928_82E2_B102B12A9732__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Manual_FrontHs.h : header file
//
#include "LedButton.h"
#include "Digit.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_FrontHs dialog
#define TMR_FRONTHSMOT_CMD			960
#define TMR_FRONTHSBUFFERMOT_CMD	961
#define TMR_FRONTHSSTEPMOT_CMD		962
#define TMR_FRONTHS_IO_MONITOR_CMD	963

class CDialog_Manual_FrontHs : public CDialog
{
// Construction
public:
	CDialog_Manual_FrontHs(CWnd* pParent = NULL);   // standard constructor
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
	double			m_d_mot_goto_pos; // 모터 이동 거리
	double			m_d_position[25][2];
	int				m_n_axis;
	int				m_n_part;
	int				m_n_direction;
	int				m_n_account;
	int				m_n_move_mode;
	int				m_n_cmd_no;			// 실행 명령 번호 저장 변수 초기화
	int				m_n_cmd_motor_no;	// 모터 번호 저장 변수 초기화
	
////////////////////////////////////////////////////
// Dialog Data
	//{{AFX_DATA(CDialog_Manual_FrontHs)
	enum { IDD = IDD_DIALOG_MANUAL_FRONTHS };
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
	CComboBox   m_cmb_f_hs_pos;
	CComboBox	m_cmb_buffer_pos;
	CMacButton	m_btn_clamp2_onoff;
	CMacButton	m_btn_clamp1_onoff;
	CMacButton	m_btn_go_tray_pos;
	CMacButton	m_btn_go_step_rbt_move;
	CMacButton	m_btn_go_buffer_pos;
	CMacButton	m_btn_go_f_hs_pos;
	CButtonST	m_btn_finger_sol_all;
	CButtonST	m_btn_finger_sol2;
	CButtonST	m_btn_finger_sol1;
	CButtonST	m_btn_picker_sol_all;
	CButtonST	m_btn_picker_sol2;
	CButtonST	m_btn_picker_sol1;
	CButtonST	m_btn_buffer_fronths_sol1;
	CButtonST	m_btn_buffer_fronths_sol2;
	CButtonST	m_btn_buffer_fronths_sol3;
	CButtonST	m_btn_buffer_fronths_sol4;
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

		// NOTE: the ClassWizard will add data members here

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Manual_FrontHs)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CFont* m_p_font;	// 폰트 정보 설정 변수
	CFont* m_load_combo_font;
	CFont* m_motor_font;	// 폰트 정보 설정 변수

	int mn_picker[2];
	int mn_finger[2];
	int mn_fronths_buffer[4];

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
	//{{AFX_MSG(CDialog_Manual_FrontHs)
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
	afx_msg void OnBtnBufferFronthsSol1();
	afx_msg void OnBtnBufferFronthsSol2();
	afx_msg void OnBtnBufferFronthsSol3();
	afx_msg void OnBtnBufferFronthsSol4();
	afx_msg void OnBtnGoFHsPos();
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

#endif // !defined(AFX_DIALOG_MANUAL_FRONTHS_H__75A07FA6_4EEF_4928_82E2_B102B12A9732__INCLUDED_)
