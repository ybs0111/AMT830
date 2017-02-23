#if !defined(AFX_DIALOG_PART_MANUAL_H__A53A0EB9_29A2_49B7_8A75_CFCC8AD06A03__INCLUDED_)
#define AFX_DIALOG_PART_MANUAL_H__A53A0EB9_29A2_49B7_8A75_CFCC8AD06A03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Part_Manual.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Part_Manual dialog
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가  
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "EditEx.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "LedButton.h"      // LED 버튼 클래스
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"		// XP 칼라 버튼 클래스
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"		// Fastech 관련 동작 Class
#include "ComizoaPublic.h"
#include "GridControlAlg.h"
#include "Digit.h"

class CDialog_Part_Manual : public CDialog
{
public:
	CFont			*m_p_font;
	CPoint			m_cp_coord;


	int				m_n_part;
	BOOL            m_b_p_limit_check;//20120719
	BOOL            m_b_p_limit_check2;//20120719
	BOOL            m_b_p_limit_check3;//20120719
	BOOL            m_b_p_limit_check4;//20120719

/////////////////////////////// 20120718
	int				m_n_move_stacker_step2;
	int				ReadyPosStep2;
	BOOL			m_b_stop_req2;
	BOOL			m_b_run_req2;
	int				m_n_motor_no2;
	int				mn_MotorNum2;
	int				mn_StackerPos2;
	double			m_dTargetPos2;
/////////////////////////////// 20120719
	int				m_n_move_stacker_step3;
	int				ReadyPosStep3;
	BOOL			m_b_stop_req3;
	BOOL			m_b_run_req3;
	int				m_n_motor_no3;
	int				mn_MotorNum3;
	int				mn_StackerPos3;
	double			m_dTargetPos3;
/////////////////////////////// 20120721
	int				m_n_move_stacker_step4;
	int				ReadyPosStep4;
	BOOL			m_b_stop_req4;
	BOOL			m_b_run_req4;
	int				m_n_motor_no4;
	int				mn_MotorNum4;
	int				mn_StackerPos4;
	double			m_dTargetPos4;
/////////////////////////////// 20120724
	int				m_move_stacker_step1;
	int				m_move_stacker_step2;
	int				m_ReadyPosStep;
	int				m_motor_no;
	int				m_MotorNum;
	int				m_StackerPos;

////////////////////////////////////////////// 20130213

	int				m_n_Stacker1;
	int				m_n_Stacker2;
	int				m_n_Stacker_Move;
//	int				m_n_motor_no;
	int				m_n_motor_no11;
	int				m_n_motor_no12;
	int				m_n_MotorMove_Step;
    int				m_n_StackerPos1;
	int				m_n_StackerPos2;
	int             m_n_Error_Stop;
	int				init_stackermove;   // Stacker Move의 이동 유무 확인
	int				init_stacker1;      // Stacker1의 이동 유무 확인
	int				init_stacker2;      // Stacker2의 이동 유무 확인
	int				init_picker;		// Picker의 초기화 확인
	char			mc_jamcode[10];
	BOOL			mb_latchcylinder_flag;
	long			ml_latchcylinder_Time[3];
	int				m_n_Stackerstep;
	int				m_n_Stacker1step;
	int				m_n_StackerMoveStep;
	double			m_CurrentPos[2];
	double			m_TargetPos[2];

/////////////////////////////////////////////// 20130225

	int				m_n_Rbt_Y; // 모듈 Rbt test
	int				m_n_Rbt_Z; // 모듈 Rbt test
	int				mn_Stacker_MovingStep;
	int				mn_TrayBackStep;
	int				mn_TrayMoveStep;
	int				mn_S2Step;
	int				mn_S2MoveStep;
	int				mn_MDRbt_step;
	int			    PickerAction[D_INFOSIZE][PICKER_NUM];
	BOOL			b_FingerFlag[PICKER_NUM];
	long			l_PickWaitTime[PICKER_NUM][3];
	long			l_FingWaitTime[PICKER_NUM][3];
	BOOL			b_PickFlag[PICKER_NUM];
	long			l_until_wait_time[3];
	double			md_AbsPos[2];
	int				mn_move_flag;
	int				mn_pickerenable_cnt;
	int				mn_FirstPicker_Y_Pos;
	int				mn_FirstPicker_X_Pos;
	int				mn_FirstTray_X_Pos;
	int				mn_FirstTray_Y_Pos;
	int				mCurrent_RowPos[10];
	int				Tray_Pick_Step;
	double			md_Robot_Pos;
	int				i_DVCRepickCnt;
	int				RunStep;
	int				mTempPicker[D_INFOSIZE][PICKER_NUM];
	double			md_CurrentPos;
	int				mResult[PICKER_NUM];
	int				i_PartialPickCnt;
	int				TrayPlaceStep;
	double			md_Robot_TrayPlacePos;
	int				mn_NextPos;
	int				PosPlus;

//////////////////////////////////////////////

	int				m_n_move_stacker_step;
	int				m_n_motor_no;
	int				mn_StackerPos;
	BOOL			m_b_stop_req;
	BOOL			m_b_run_req;
	int				ReadyPosStep;
	int				mn_MotorNum;
	double			m_dTargetPos;
	int				mn_retry;
	char			mc_alarmcode[10];

	int mb_pushercylinder_flag[2];
	long ml_pushercylinder_Time[2][3];

	int mb_updncylinder_flag[2];
	long ml_undncylinder_Time[2][3];

	//Push, Back cylinder
	int mb_cylinder_flag;
	long ml_cylinder_Time[3];

	int mb_pusherupdncylinder_flag;
	long ml_pusherupdncylinder_Time[3];

	//Vision Test
	int mn_Error_stop;
	int mn_InitStep;
	int mn_VisionStep[2];
	int mn_labelStep;
	int mn_Visinitial;
	int mn_vision1_init;
	int mn_vision2_init;
	char cAlarmCose[10];
	int n_visionrbt_label_req[2];
	double md_offset;
	long l_PickerWaitTime[3];
	int mn_attach_err;
	double m_dTargetLabel;
	int mb_dvc_rotator_flag[2];
	long ml_dvc_rotator_wait[2][3];

	//Clip Test
	int mn_Clipinitial;
	int mn_Clip1_init;
	int mn_Clip2_init;
	int	InsertStep;
	int BufferStep;
	int	ClipPickStep;
	int ClipPlaceStep;
	int ClipBufferStep;
	int LoadClipStep;
	int ClipMoveStep;
	int PushStep;
	int RotatorStep;
	int FixStep;
	int Change_step;
	int TransUpDnStep;
	int TransferMoveStep;
	int ClipInitStep;
	int ClipStackerStep;
	int mn_Clipretry;


	//Vision Test
	int n_visionrbt_workend_flag[2];
	int n_visionrbt_working_site[2];
	int n_visionrbt_next_working_site[2];	
	int n_visionrbt_visiontest_req[2];
	int n_visionrbt_past_working_site[2];

	//Clip Test
	int n_module_ldrbt_clipbuffer_req[2];
	int n_module_clip_req[2];
	int n_ld_clip_tray_supply;
	int n_lotend_righths_ldtray_site;
	int n_lotend_clip_ldtray_site;
	int n_lotend_clip_ldrbt;
	int ClipTray[2][60];
	int ActionPicker[D_INFOSIZE];
	int nClipRobotPicker[D_INFOSIZE];
	BOOL b_PickerFlag;
	long l_Until_WaitTime[3];
	long l_UntilWaitTime[3];
	int mn_UnloadTubeNum;
	int mn_change_clip;
	int RunTransStep;
	int mn_Tuberetry;
	
	int n_lotend_clip_ldstacker_site;
	int n_loadClip;
	int i_load_clip;
	int i_unload_clip;
	int n_module_ldrbt_clamp_req[4];
	int n_lotend_clipbuffer_ldrbt;

/////////////// 20120724
	int RightInitStep;
	BOOL mb_m_latchcylinder_flag;
	long ml_m_latchcylinder_Time[3];
	int mb_m_clampcylinder_flag;
	int ml_m_clampcylinder_Time[3];

	int mn_LdTube;
	int mn_LdRotateTube;

	int mn_stacker_updn_cyliner[2];
////////////////////////////////// 20130320
	int ClipStep; 
	int ClipRbtStep;
	// Construction
public:
	BOOL Create();
	CDialog_Part_Manual(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Part_Manual)
	enum { IDD = IDD_DIALOG_PART_MANUAL };
	CMacButton	m_btn_vision_end;
	CMacButton	m_btn_vision_run;
	CMacButton	m_btn_io_move_12;
	CMacButton	m_btn_io_move_11;
	CMacButton	m_btn_io_move_10;
	CMacButton	m_btn_io_move_9;
	CMacButton	m_btn_io_move_8;
	CMacButton	m_btn_io_move_7;
	CMacButton	m_btn_io_move_6;
	CMacButton	m_btn_io_move_5;
	CMacButton	m_btn_io_move_4;
	CMacButton	m_btn_io_move_3;
	CMacButton	m_btn_io_move_2;
	CMacButton	m_btn_io_move_1;
	CMacButton	m_btn_motor_move_12;
	CMacButton	m_btn_motor_move_11;
	CMacButton	m_btn_motor_move_10;
	CMacButton	m_btn_motor_move_9;
	CMacButton	m_btn_motor_move_8;
	CMacButton	m_btn_motor_move_7;
	CMacButton	m_btn_motor_move_6;
	CMacButton	m_btn_motor_move_5;
	CMacButton	m_btn_motor_move_4;
	CMacButton	m_btn_motor_move_3;
	CMacButton	m_btn_motor_move_2;
	CMacButton	m_btn_motor_move_1;
	CXPGroupBox	m_group_motor;
	CXPGroupBox	m_group_io;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Part_Manual)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnPManual_Button_Set();
	void OnPManual_GroupBox_Set();
	void MoveStepClear();
	int StackerMovingExcution();
//////////////////////////////////// 20120718
	int StackerMovingExcution2();
 	int StackerMovingExcution3();//20120719
 	int StackerMovingExcution4();
	int Set_RailFwdbwdCylinderOnOff2(int nStacker2, int n_onoff2);
 	int Set_RailFwdbwdCylinderOnOff3(int nStacker3, int n_onoff3);//20120719
 	int Set_RailFwdbwdCylinderOnOff4(int nStacker4, int n_onoff4);
	int Get_RailFwdbwdCylinderOnOff2(int nStacker2, int n_onoff2);
 	int Get_RailFwdbwdCylinderOnOff3(int nStacker3, int n_onoff3);//20120719
 	int Get_RailFwdbwdCylinderOnOff4(int nStacker4, int n_onoff4);

	int Set_StackerUpDnClinder2(int nStacker2, int n_onoff2);
 	int Set_StackerUpDnClinder3(int nStacker3, int n_onoff3);//20120719
 	int Set_StackerUpDnClinder4(int nStacker4, int n_onoff4);
	int Get_StackerUpDnClinder2(int nStacker2, int n_onoff2);
 	int Get_StackerUpDnClinder3(int nStacker3, int n_onoff3);//20120719
 	int Get_StackerUpDnClinder4(int nStacker4, int n_onoff4);
////////////////////////////////////	
	
	int OnModuleStackerReadyPos(int n_StackerPos, int nMode);
	int OnLefthsStackerReadyPos(int n_StackerPos2, int nMode2);//20120718
	int OnRighthsStackerReadyPos(int n_StackerPos3, int nMode3);//20120719
	int OnULDStackerReadyPos(int n_StackerPos4, int nMode4);//20120721

	int Set_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff);
	int Get_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff);
	int Set_StackerUpDnClinder(int nStacker, int n_onoff);
	int Get_StackerUpDnClinder(int nStacker, int n_onoff);
	void EnableButton(int iEnable);

	//Vision
	int InitVision();
	int Module_Vision_Btm();
	int Module_Vision_Top();
	int Label_Test();
	int Find_VisionRobot_Current_Position(int n_site, int n_check_pos); //로보트의 현재 위치를 확인한다 
	int set_dvc_rotator(int n_site, int n_onoff);
	int get_dvc_rotator(int n_site, int n_onoff, int n_wait, int n_limit);

	//CLip Test
	int InitClip();
	int LoadClipMove();
	int LoadClipTransfer();
	int ClipInsertMove();
	int ClipBufferMove();
	int OnLoadTransferMove();
	int OnPlaceClip(int nPos);
	int OnPickupClip(int nPos);
	int LdUldClipBufferPos(int nPos);
	int CheckTransferBeforeMove(int nChange=0);
	void SetMovePusherCyliner(BOOL bFwdMove);
	int GetMovePusherCyliner(BOOL bFwdMove);
	int OnSetRotatorUpdn(BOOL bUpDn);
	int OnFixLoadTube();
	int ChangeLoadTray();
	int OnLoadClipTransferStackerUpDn();
	void Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE]);
	int Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE]);
	int ClipInit();
	int InitClip_1();
	int InitClip_2();
	int ClipInit_Clip();
	int ClipInit_Stacker();


/////////////// 20120724
	void InitRightStacker();
	int Stacker1Move();
	int Stacker2Move();
	int Set_M_TrayLatchClinder(int n_onoff);
	int Get_M_TrayLatchClinder(int n_onoff);
	int Set_M_RailClampOnOff(int n_onoff);
	int Get_M_RailClampOnOff(int n_onoff);
	int Set_M_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff);
	int Get_M_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff);

////////////////////////////////////////////////////////////////// 20130213

	int ModuleStackerMenualMoving();
	int StackerMovingHome();
	int Stacker1MovingCheck();
	int Stacker1MoveMethod(int nMode);
	int Set_TrayLatchClinder(int n_onoff);
	int Get_TrayLatchClinder(int n_onoff);

////////////////////////////////////////////////////////////////// 20130225
	int Module_Rbt_Home();
	int Module_Rbt_Move();
	int Module_TrayBack();
	int Module_TrayMove();
	int Module_Stacker2_MoveMethod(int nMode);
	int Set_PusherUpDnCylinderOnOff(int n_onoff);
	int Get_PusherUpDnCylinderOnOff(int n_onoff);
	int Set_PusherCylinderOnOff(int n_onoff);
	int Get_PusherCylinderOnOff(int n_onoff);
	
	void Module_Stacker_Move();
	void Module_Stacker2_Move();

	void Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	int Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	void Set_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM]);
	int Get_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM]);
	int Robot_CurrentPos_Check(double y_pos, double z_pos);
	int Robot_TrayDVCPick(int n_bin, int n_tray_y);
	int Robot_ModuleDVCTrayPlace(int nRow, int ActionPicker[D_INFOSIZE][PICKER_NUM]);

///////////////////////////////////////////////////////////////// 20130320
	int ClipPlaceInit();
	int ClipStackerInit();
	int ClipRbtInit();
	// Generated message map functions
	//{{AFX_MSG(CDialog_Part_Manual)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnMotorMove1();
	afx_msg void OnBtnMotorMove2();
	afx_msg void OnBtnMotorMove3();
	afx_msg void OnBtnMotorMove4();
	afx_msg void OnBtnMotorMove5();
	afx_msg void OnBtnMotorMove6();
	afx_msg void OnBtnMotorMove7();
	afx_msg void OnBtnMotorMove8();
	afx_msg void OnBtnMotorMove9();
	afx_msg void OnBtnMotorMove10();
	afx_msg void OnBtnMotorMove11();
	afx_msg void OnBtnMotorMove12();
	afx_msg void OnBtnIoMove1();
	afx_msg void OnBtnIoMove2();
	afx_msg void OnBtnIoMove3();
	afx_msg void OnBtnIoMove4();
	afx_msg void OnBtnIoMove5();
	afx_msg void OnBtnIoMove6();
	afx_msg void OnBtnIoMove7();
	afx_msg void OnBtnIoMove8();
	afx_msg void OnBtnIoMove9();
	afx_msg void OnBtnIoMove10();
	afx_msg void OnBtnIoMove11();
	afx_msg void OnBtnIoMove12();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnStop();
	afx_msg void OnBtnVisionRun();
	afx_msg void OnBtnVisionEnd();
	afx_msg void OnBtnCliptestRun();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_PART_MANUAL_H__A53A0EB9_29A2_49B7_8A75_CFCC8AD06A03__INCLUDED_)
