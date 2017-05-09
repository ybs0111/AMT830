#if !defined(AFX_DIALOG_MANUAL_CLIP_CLAMP_H__3779DC9C_4DE8_483B_8A6F_604930B57783__INCLUDED_)
#define AFX_DIALOG_MANUAL_CLIP_CLAMP_H__3779DC9C_4DE8_483B_8A6F_604930B57783__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Manual_Clip_Clamp.h : header file
//

#include "LedButton.h"
#include "Digit.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Clip_Clamp dialog

#define TMR_CLIPMOT_CMD	    1040
#define TMR_CLIP_BUFFERMOT_CMD	1041
#define TMR_CLIP_STEPMOT_CMD		1042
#define TMR_CLIP_IO_MONITOR_CMD	1043

class CDialog_Manual_Clip_Clamp : public CDialog
{
// Construction
public:
	CDialog_Manual_Clip_Clamp(CWnd* pParent = NULL);   // standard constructor

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
	//{{AFX_DATA(CDialog_Manual_Clip_Clamp)
	enum { IDD = IDD_DIALOG_MANUAL_CLIP_CLAMP };
	CMacButton	m_btn_stop;
	CButtonST	m_btn_right;
	CButtonST	m_btn_home;
	CButtonST	m_btn_left;
	CXPGroupBox	m_group_clip;
	CXPGroupBox	m_group_buffer_rbt_move;
	CXPGroupBox	m_group_buffer;
	CXPGroupBox	m_group_motor;
	CXPGroupBox	m_group_clip_rbt_move;
	CGradientStatic	m_msg_clip_1;
	CGradientStatic	m_msg_clip_2;
	CGradientStatic	m_msg_clip_3;
	CGradientStatic	m_msg_clip_4;
	CGradientStatic	m_msg_clip_5;
	CGradientStatic	m_msg_clip_6;
	CGradientStatic	m_msg_clip_7;
	CGradientStatic	m_msg_clip_8;
	CGradientStatic	m_msg_clip_9;
	CGradientStatic	m_msg_clip_10;
	CGradientStatic	m_msg_clip_11;
	CGradientStatic	m_msg_clip_12;
	CGradientStatic	m_msg_clip_13;
	CGradientStatic	m_msg_clip_14;
	CGradientStatic	m_msg_clip_15;
	CGradientStatic	m_msg_clip_16;
	CGradientStatic	m_msg_buffer4;
	CGradientStatic	m_msg_buffer3;
	CGradientStatic	m_msg_buffer2;
	CGradientStatic	m_msg_buffer1;
	CGradientStatic	m_msg_buffer_module;
	CGradientStatic	m_msg_buffer_hs;
	CButtonST	m_btn_buffer_module_sol4;
	CButtonST	m_btn_buffer_module_sol3;
	CButtonST	m_btn_buffer_module_sol2;
	CButtonST	m_btn_buffer_module_sol1;
	CButtonST	m_btn_buffer_hs_sol4;
	CButtonST	m_btn_buffer_hs_sol3;
	CButtonST	m_btn_buffer_hs_sol2;
	CButtonST	m_btn_buffer_hs_sol1;
	CMacButton	m_btn_go_buffer_pos;
	CMacButton	m_btn_go_clip_pos;
	CButtonST	m_btn_clip_sol_1;
	CButtonST	m_btn_clip_sol_2;
	CButtonST	m_btn_clip_sol_3;
	CButtonST	m_btn_clip_sol_4;
	CButtonST	m_btn_clip_sol_5;
	CButtonST	m_btn_clip_sol_6;
	CButtonST	m_btn_clip_sol_7;
	CButtonST	m_btn_clip_sol_8;
	CButtonST	m_btn_clip_sol_9;
	CButtonST	m_btn_clip_sol_10;
	CButtonST	m_btn_clip_sol_11;
	CButtonST	m_btn_clip_sol_12;
	CButtonST	m_btn_clip_sol_13;
	CButtonST	m_btn_clip_sol_14;
	CButtonST	m_btn_clip_sol_15;
	CButtonST	m_btn_clip_sol_16;
	CComboBox	m_cmb_motor_pos;
	CComboBox	m_cmb_buffer_pos;
	CComboBox	m_cmb_clip_pos;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Manual_Clip_Clamp)
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

	int mn_sol[20];
	int mn_module_buffer[4];
	int mn_hs_buffer[4];

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

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Manual_Clip_Clamp)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnClipSol1();
	afx_msg void OnBtnClipSol2();
	afx_msg void OnBtnClipSol3();
	afx_msg void OnBtnClipSol4();
	afx_msg void OnBtnClipSol5();
	afx_msg void OnBtnClipSol6();
	afx_msg void OnBtnClipSol7();
	afx_msg void OnBtnClipSol8();
	afx_msg void OnBtnClipSol9();
	afx_msg void OnBtnClipSol10();
	afx_msg void OnBtnClipSol11();
	afx_msg void OnBtnClipSol12();
	afx_msg void OnBtnClipSol13();
	afx_msg void OnBtnBufferModuleSol1();
	afx_msg void OnBtnBufferModuleSol2();
	afx_msg void OnBtnBufferModuleSol3();
	afx_msg void OnBtnBufferModuleSol4();
	afx_msg void OnBtnBufferHsSol1();
	afx_msg void OnBtnBufferHsSol2();
	afx_msg void OnBtnBufferHsSol3();
	afx_msg void OnBtnBufferHsSol4();
	afx_msg void OnBtnGoBufferPos();
	afx_msg void OnBtnGoClipPos();
	afx_msg void OnBtnStop();
	afx_msg void OnSelchangeCmbMotorPos();
	afx_msg void OnBtnHome();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnClipSol14();
	afx_msg void OnBtnClipSol15();
	afx_msg void OnBtnClipSol16();
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

#endif // !defined(AFX_DIALOG_MANUAL_CLIP_CLAMP_H__3779DC9C_4DE8_483B_8A6F_604930B57783__INCLUDED_)
