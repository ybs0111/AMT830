#if !defined(AFX_RUN_STACKER_LOAD_MODULE_H__9C81D450_4B1A_4B2B_92C4_5E8FE114EE0A__INCLUDED_)
#define AFX_RUN_STACKER_LOAD_MODULE_H__9C81D450_4B1A_4B2B_92C4_5E8FE114EE0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Stacker_Load_Module.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Load_Module command target

class CRun_Stacker_Load_Module
{
public:
	CRun_Stacker_Load_Module();           // protected constructor used by dynamic creation
	virtual ~CRun_Stacker_Load_Module();

// Attributes
public:

// Operations
public:
	void Thread_Run();
	void Run_Stacker1Move();
	void Run_Stacker2Move();
	void Run_Transfer();
	void Run_Moving_Stacker();
	void Run_Init();
	void Run_Reinstate();
	void Thread_Variable_Initial();
	void Robot_BackMovePosValCheck();//20121126
	void OnLdModuleStacker_FinalPos();//20121126

	int Set_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff);
	int Get_RailFwdbwdCylinderOnOff(int nStacker, int n_onoff);
	int Set_StackerUpDnClinder(int nStacker, int n_onoff);
	int Get_StackerUpDnClinder(int nStacker, int n_onoff);
	int OnModuleStackerReadyPos(int nMotorNum, int nMode = 0);
	int MotorMove(int n_StackerPos, int nMotor, double dPos, int nSpeed);
	
	int Get_StackerTrayUseCheck(int nTime = 0);
	int Set_TrayLatchClinder(int n_onoff);
	int Get_TrayLatchClinder(int n_onoff);

	int LD_Module_TrayMove();
	int LD_Module_TrayBack();
	int Set_PusherCylinderOnOff(int n_onoff);
	int Get_PusherCylinderOnOff(int n_onoff);
	int Set_PusherUpDnCylinderOnOff(int n_onoff);
	int Get_PusherUpDnCylinderOnOff(int n_onoff);

	int RunS1Step;
	int RunS2Step;
	int RunMStep;
	int MoveStep;
	int InitStep;
	int TrayBackStep;
	int ReadyPosStep[2];
	int MotorMoveStep[2];
	int ReinstateStep;
	int RunTransStep;
	int TrayMoveStep;
	int mn_stacker_move_flag;
	int mn_stacker_Up;
	char mc_jamcode[10];
	int init_stacker_move;
	int init_stacker_1;
	int init_stacker_2;
	int initial_smove;
	int mn_LeakM_LotEnd[4];

	BOOL mb_StackerTrayCheck_Flag;

	int mn_stacker_updn_cyliner[2];
	long ml_StackerTrayCheck_Wait[4][3];

	BOOL mb_latchcylinder_flag;
	long ml_latchcylinder_Time[3];

	double m_dCurPos;
	double m_dTargetPos[2];
	double m_dNowPos[2];
	int m_nMotorNum[2];

	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126


	int mb_pushercylinder_flag[2];
	long ml_pushercylinder_Time[2][3];

	int mb_updncylinder_flag[2];
	long ml_undncylinder_Time[2][3];

	//Push, Back cylinder
	int mb_cylinder_flag;
	long ml_cylinder_Time[3];

	int mb_pusherupdncylinder_flag;
	long ml_pusherupdncylinder_Time[3];


	//add
	int mn_retry;
	int mn_retry1;
	int mn_retry2;
	int mn_ms_retry; //Moving Stacker Retry
	int mn_stacker1_init_chk;
	int mn_stacker2_init_chk;
	int mn_Moving_stacker;
	int mn_m_uld_stacker;

};
extern CRun_Stacker_Load_Module Run_Stacker_Load_Module;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_STACKER_LOAD_MODULE_H__9C81D450_4B1A_4B2B_92C4_5E8FE114EE0A__INCLUDED_)
