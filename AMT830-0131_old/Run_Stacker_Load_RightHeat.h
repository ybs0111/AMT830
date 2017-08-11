#if !defined(AFX_RUN_STACKER_LOAD_RIGHTHEAT_H__BFAEEAA0_B525_42A3_A1D6_CD20FDE951D5__INCLUDED_)
#define AFX_RUN_STACKER_LOAD_RIGHTHEAT_H__BFAEEAA0_B525_42A3_A1D6_CD20FDE951D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Stacker_Load_RightHeat.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Load_RightHeat command target

class CRun_Stacker_Load_RightHeat
{
public:
	CRun_Stacker_Load_RightHeat();           // protected constructor used by dynamic creation
	virtual ~CRun_Stacker_Load_RightHeat();

// Attributes
public:
	void Thread_Run();
	void Run_Init();
	void Thread_Variable_Initial();
	void Run_Transfer();
	void Run_Stacker1Move();
	void Run_Stacker2Move();
	void Run_Moving_Stacker();
	void Run_Reinstate();
	void Robot_BackMovePosValCheck();//20121126
	void OnHSRightStacker_FinalPos();//20121126

	int OnRighthsStackerReadyPos(int n_StackerPos, int nMode);
	int Set_TrayLatchClinder(int n_onoff);
	int Get_TrayLatchClinder(int n_onoff);
	int Set_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff);
	int Get_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff);
	int Set_StackerUpDnClinder(int nStacker, int n_onoff);
	int Get_StackerUpDnClinder(int nStacker, int n_onoff);

	int LD_Righths_TrayMove();
	int LD_Righths_TrayBack();
	int LD_Righths_Seperate_Tray(int nOnOff);
	int Set_PusherCylinderOnOff(int n_onoff);
	int Get_PusherCylinderOnOff(int n_onoff);
	int Set_PusherUpDnCylinderOnOff(int n_onoff);
	int Get_PusherUpDnCylinderOnOff(int n_onoff);

	int Set_RailClampOnOff(int n_onoff);
	int Get_RailClampOnOff(int n_onoff);

	int PrevStep;
	int RunS1Step;
	int RunS2Step;
	int RunMStep;
	int MoveStep;
	int InitStep;
	int RunTransStep;
	int ReinstateStep;
	int TrayMoveStep;
	int TrayBackStep;
	int TraySeperateStep;
	int ReadyPosStep[2];
	int MotorMoveStep[2];
	char mc_jamcode[10];
	int mn_stacker_move_flag;
	int mb_sdstop;
	int init_stacker_move;
	int init_stacker_1;
	int init_stacker_2;
	int initial_smove;

	int mn_StackerPos;
	int mn_retry;

	double m_dTargetPos[2];
	double m_dNowPos[2];
	int mn_stacker_updn_cyliner[2];

	//Stacker1에만 해당
	int mb_clampcylinder_flag;
	int ml_clampcylinder_Time[3];

	int mb_pushercylinder_flag[2];
	long ml_pushercylinder_Time[2][3];

	int mb_updncylinder_flag[2];
	long ml_undncylinder_Time[2][3];

	BOOL mb_latchcylinder_flag;
	long ml_latchcylinder_Time[3];

	BOOL mb_pusherupdncylinder_flag;
	long ml_pusherupdncylinder_Time[3];

	BOOL mb_cylinder_flag;
	long ml_cylinder_Time[3];

	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126

	long ml_until_waittime[3];

	//add
	int mn_retry1;
	int mn_retry2;
	int mn_ms_retry; //Moving Stacker Retry
	int mn_stacker1_init_chk;
	int mn_stacker2_init_chk;
	int mn_Moving_stacker;
	int mn_m_uld_stacker;

	CString m_strTmp;

	int mn_LeakM_LotEnd[4]; //[0] : stacker1 [1] : Transfer [2] : Stacker2
};
extern CRun_Stacker_Load_RightHeat Run_Stacker_Load_RightHeat;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_STACKER_LOAD_RIGHTHEAT_H__BFAEEAA0_B525_42A3_A1D6_CD20FDE951D5__INCLUDED_)
