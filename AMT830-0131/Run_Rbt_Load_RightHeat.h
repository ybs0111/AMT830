#if !defined(AFX_RUN_RBT_LOAD_RIGHTHEAT_H__43CD4263_8D2B_42C2_AB2B_492E6FEF09B3__INCLUDED_)
#define AFX_RUN_RBT_LOAD_RIGHTHEAT_H__43CD4263_8D2B_42C2_AB2B_492E6FEF09B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Rbt_Load_RightHeat.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Load_RightHeat command target

class CRun_Rbt_Load_RightHeat
{
public:
	CRun_Rbt_Load_RightHeat();           // protected constructor used by dynamic creation
	virtual ~CRun_Rbt_Load_RightHeat();

// Attributes
public:
	int RunStep;
	int MoveStep;
	int InitStep;
	int ReinstateStep;
	int SafetyZone_Step;
	int Tray_Pick_Step;
	int Module_Place_Step;
	int Before_Module_Place_Step;
	int TrayPlaceStep;

	int mn_pre_move_flag;
	int mn_Pickerenable_cnt;
	int mn_FirstPicker_Y_Pos;
	int mn_FirstTray_Y_Pos;

	double md_AbsPos;
	double md_SafetyPos;
	double md_CurrentPos;
	double md_Robot_PlacePos;
	double md_Robot_Pos;
	double md_Robot_TrayPlacePos;
	int mn_NextPos;

	int mn_FirstTray_X_Pos;
	int mn_FirstPicker_X_Pos;
	int i_DVCRepickCnt;
	int mResult[PICKER_NUM];

	int RHSHeadVal;

	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	int mn_Clipretry;//20121126
	double dHSRightCurPos[10];//20121126

	long l_until_wait_time[3];
	int mCurrent_RowPos[10];
	int mn_pickerenable_cnt;
	char mc_alarmcode[10];

	long l_ReinStateWaitTime[3];

	int HeadVal;

	BOOL b_PickerFlag[PICKER_NUM];
	BOOL b_FingerFlag[PICKER_NUM];
	int mTempPicker[D_INFOSIZE][PICKER_NUM];
	int ActionPicker[D_INFOSIZE][PICKER_NUM];
	int FingerInfo[D_INFOSIZE][PICKER_NUM];
	long l_PickerWaitTime[PICKER_NUM][3];
	long l_FingerWaitTime[PICKER_NUM][3];

// Operations
public:
	void Thread_Run();
	void Run_Move();
	void Run_Init();
	void Run_Reinstate();
	void Thread_Variable_Initial();
	void Robot_BackMovePosValCheck();//20121126
	void OnHSRightRbt_FinalPos();//20121126

	void Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	int Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	void Set_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM]);
	int Get_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM]);
	int Robot_AutoMove_Safety_Zone(int n_mode, int n_site);
	int Robot_TrayDVCPick(int n_bin, int n_tray_y);
	int Robot_ModuleDVCPlace(int n_site, int n_bin);
	int Robot_BeforeModuleDVCPlaceMove(int n_site, int n_bin);
	int Robot_CurrentPos_Check(double y_pos, double z_pos);
	int Robot_HSDVCTrayPlace(int nRow, int ActionPicker[D_INFOSIZE][PICKER_NUM]);

};
extern CRun_Rbt_Load_RightHeat Run_Rbt_Load_RightHeat;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_RBT_LOAD_RIGHTHEAT_H__43CD4263_8D2B_42C2_AB2B_492E6FEF09B3__INCLUDED_)
