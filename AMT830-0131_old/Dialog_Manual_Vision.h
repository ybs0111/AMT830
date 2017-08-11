#if !defined(AFX_DIALOG_MANUAL_VISION_H__EDB23935_D3AD_467E_BBAF_A0CC52A902AC__INCLUDED_)
#define AFX_DIALOG_MANUAL_VISION_H__EDB23935_D3AD_467E_BBAF_A0CC52A902AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Manual_Vision.h : header file
//

#include "LedButton.h"
#include "Digit.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Vision dialog
#define TMR_VISION_MOT_CMD	    11040
#define TMR_VISION_BUFFERMOT_CMD	1141
#define TMR_VISION_STEPMOT_CMD		1142
#define TMR_VISION_IO_MONITOR_CMD	1143

class CDialog_Manual_Vision : public CDialog
{
// Construction
public:
	CDialog_Manual_Vision(CWnd* pParent = NULL);   // standard constructor
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
	//{{AFX_DATA(CDialog_Manual_Vision)
	enum { IDD = IDD_DIALOG_MANUAL_VISION };
	CMacButton	m_btn_stop;
	CButtonST	m_btn_right;
	CButtonST	m_btn_home;
	CButtonST	m_btn_left;
	CXPGroupBox	m_group_vision;
	CXPGroupBox	m_group_vision_rbt_move;
	CXPGroupBox	m_group_motor;
	CGradientStatic	m_msg_Vision_1;
	CGradientStatic	m_msg_Vision_2;
	CGradientStatic	m_msg_Vision_3;
	CGradientStatic	m_msg_Vision_4;
	CGradientStatic	m_msg_Vision_5;
	CGradientStatic	m_msg_Vision_6;
	CGradientStatic	m_msg_Vision_7;
	CGradientStatic	m_msg_Vision_8;
	CGradientStatic	m_msg_Vision_9;
	CGradientStatic	m_msg_Vision_10;
	CGradientStatic	m_msg_Vision_11;
	CGradientStatic	m_msg_Vision_12;
	CGradientStatic	m_msg_Vision_13;
	CMacButton	m_btn_go_vision_pos;
	CButtonST	m_btn_Vision_sol_1;
	CButtonST	m_btn_Vision_sol_2;
	CButtonST	m_btn_Vision_sol_3;
	CButtonST	m_btn_Vision_sol_4;
	CButtonST	m_btn_Vision_sol_5;
	CButtonST	m_btn_Vision_sol_6;
	CButtonST	m_btn_Vision_sol_7;
	CButtonST	m_btn_Vision_sol_8;
	CButtonST	m_btn_Vision_sol_9;
	CButtonST	m_btn_Vision_sol_10;
	CButtonST	m_btn_Vision_sol_11;
	CButtonST	m_btn_Vision_sol_12;
	CButtonST	m_btn_Vision_sol_13;
	CComboBox	m_cmb_motor_pos;
	CComboBox	m_cmb_vision_pos;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Manual_Vision)
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
	//{{AFX_MSG(CDialog_Manual_Vision)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnVisionSol1();
	afx_msg void OnBtnVisionSol2();
	afx_msg void OnBtnVisionSol3();
	afx_msg void OnBtnVisionSol4();
	afx_msg void OnBtnVisionSol5();
	afx_msg void OnBtnVisionSol6();
	afx_msg void OnBtnVisionSol7();
	afx_msg void OnBtnVisionSol8();
	afx_msg void OnBtnVisionSol9();
	afx_msg void OnBtnVisionSol10();
	afx_msg void OnBtnClipSol11();
	afx_msg void OnBtnVisionSol12();
	afx_msg void OnBtnVisionSol11();
	afx_msg void OnBtnGoVisionPos();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnHome();
	afx_msg void OnSelchangeCmbMotorPos();
	afx_msg void OnBtnVisionSol13();
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

#endif // !defined(AFX_DIALOG_MANUAL_VISION_H__EDB23935_D3AD_467E_BBAF_A0CC52A902AC__INCLUDED_)
