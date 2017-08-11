#if !defined(AFX_RUN_STACKER_SORTER_H__BDEE73C6_F757_47CD_9AAC_DD0425B69B55__INCLUDED_)
#define AFX_RUN_STACKER_SORTER_H__BDEE73C6_F757_47CD_9AAC_DD0425B69B55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Stacker_Sorter.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Sorter command target

class CRun_Stacker_Sorter
{
public:
	CRun_Stacker_Sorter();           // protected constructor used by dynamic creation
	virtual ~CRun_Stacker_Sorter();

// Attributes
public:

// Operations
public:
	void Thread_Run();
	void Run_Transfer();
	void Run_Stacker1Move();
	void Run_Stacker2Move();
	void Run_Moving_Stacker();
	void Run_Init();
	void Run_Reinstate();
	void Thread_Variable_Initial();
	void Robot_BackMovePosValCheck();//20121126
	void OnSorterStacker_FinalPos();//20121126

	int RunStep;
	int RunMStep;
	int MoveStep;
	int InitStep;
	int ReinstateStep;
	int ReadyPosStep_old[2];
	int ReadyPosStep[2];
	int MotorMoveStep;
	int WorkPosStep_old;
	int WorkPosStep;
	int RunS1Step;
	int RunS2Step;
	int RunTransStep;
	int StackerReayStep;
	int TrayMoveStep;
	int TraySeperateStep;
	int TrayBackStep;
	char mc_jamcode[10];
	int mn_StackerPos;
	int mn_StackerTrayReady;
	int mn_stacker_move_flag;
	int initial_smove;

	int mn_retry;
	BOOL mb_pusherupdncylinder_flag;
	BOOL mb_pushercylinder_flag[2];
	BOOL mb_latchcylinder_flag;
	BOOL b_PickerFlag[2];
	BOOL mb_pusher_flag;
	BOOL mb_pushercylinder;
	long ml_pusherupdncylinder_Time[3];
	long ml_pushercylinder_Time[2][3];
	long ml_latchcylinder_Time[3];
	long l_PickerWaitTime[2][3];
	double dTargetPos;
	double m_dTargetPos[2];
	double m_dNowPos[2];
	BOOL mb_clamp_flag;
	long ml_clamp_time[3];
	long ml_pusher_wait[3];
	int mResult[1];
	int mb_updncylinder_flag[2];
	long ml_undncylinder_Time[2][3];
	int mn_stacker_updn_cyliner[2];
	long ml_pusher_Time[3];

	int init_stacker_move;
	int init_stacker_1;
	int init_stacker_2;


	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126

	double md_CurrentPos;
	
	int OnElevatorWorkPos();
	int TrayUpPossibleCheck();
	int ULD_Module_TrayBack();
	int ULD_Module_TrayMove();

	int Set_TrayLatchClinder(int n_onoff);
	int Get_TrayLatchClinder(int n_onoff);

	int Set_PusherUpDnCylinderOnOff(int n_onoff);
	int Get_PusherUpDnCylinderOnOff(int n_onoff);
	int Set_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff);
	int Get_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff);
	int Set_LDZoneBaseRailOnOff(int n_onoff);
	int Get_LDZoneBaseRailOnOff(int n_onoff, int n_option=1);
	int Set_PusherCylinderOnOff(int n_onoff);
	int Get_PusherCylinderOnOff(int n_onoff);
	int MotorMove(int nMotor, double dMovePos, int nSpeedRate);
	int OnModuleStackerReadyPos(int n_StackerPos, int nMode);
	int Set_StackerUpDnClinder(int nStacker, int n_onoff);
	int Get_StackerUpDnClinder(int nStacker, int n_onoff);

	//add
	int mn_retry1;
	int mn_retry2;
	int mn_ms_retry; //Moving Stacker Retry
	int mn_stacker1_init_chk;
	int mn_stacker2_init_chk;
	int mn_Moving_stacker;
	int mn_m_uld_stacker;

	int mn_LeakM_LotEnd[4]; //[0] : stacker1 [1] : Transfer [2] : Stacker2
};
extern CRun_Stacker_Sorter Run_Stacker_Sorter;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_STACKER_SORTER_H__BDEE73C6_F757_47CD_9AAC_DD0425B69B55__INCLUDED_)
