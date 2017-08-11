#if !defined(AFX_RUN_RBT_LOAD_LEFTHEAT_H__4D5DC95B_7563_4926_B9E2_5FEE6152CB06__INCLUDED_)
#define AFX_RUN_RBT_LOAD_LEFTHEAT_H__4D5DC95B_7563_4926_B9E2_5FEE6152CB06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Rbt_Load_LeftHeat.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Load_LeftHeat command target

class CRun_Rbt_Load_LeftHeat
{
public:
	CRun_Rbt_Load_LeftHeat();           // protected constructor used by dynamic creation
	virtual ~CRun_Rbt_Load_LeftHeat();

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
	int mn_posok;
	int TrayPlaceStep;

	int mn_pre_move_flag;
	int mn_Pickerenable_cnt;
	int mn_FirstPicker_Y_Pos;
	int mn_FirstTray_Y_Pos;
	int mn_NextPos;
	
	BOOL b_PickerFlag[PICKER_NUM];
	BOOL b_FingerFlag[PICKER_NUM];
	int ActionPicker[D_INFOSIZE][PICKER_NUM];
	int mTempPicker[D_INFOSIZE][PICKER_NUM];
	int FingerInfo[D_INFOSIZE][PICKER_NUM];
	long l_PickerWaitTime[PICKER_NUM][3];
	long l_FingerWaitTime[PICKER_NUM][3];
	int mCurrent_RowPos[10];
	int mn_pickerenable_cnt;
	
	int mn_FirstTray_X_Pos;
	int mn_FirstPicker_X_Pos;
	int i_DVCRepickCnt;
	int mResult[PICKER_NUM];
	char mc_alarmcode[10];

	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	int mn_Clipretry;//20121126
	double dHSLeftCurPos[10];//20121126

	long l_until_wait_time[3];
	long l_ReinStateWaitTime[3];
	
	double md_AbsPos;
	double md_SafetyPos;
	double md_CurrentPos;
	double md_Robot_PlacePos;
	double md_Robot_Pos;
	double md_Robot_TrayPlacePos;

	int HeadVal;

// Operations
public:
	void Thread_Run();
	void Run_Move();
	void Run_Init();
	void Run_Reinstate();
	void Thread_Variable_Initial();
	void Robot_BackMovePosValCheck();//20121126
	void OnHSLeftRbt_FinalPos();//20121126

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
extern CRun_Rbt_Load_LeftHeat Run_Rbt_Load_LeftHeat;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_RBT_LOAD_LEFTHEAT_H__4D5DC95B_7563_4926_B9E2_5FEE6152CB06__INCLUDED_)
