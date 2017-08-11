#if !defined(AFX_RUN_RBT_SORTER_H__C9303B6D_E28A_4DEE_87E1_0BAC4A15921B__INCLUDED_)
#define AFX_RUN_RBT_SORTER_H__C9303B6D_E28A_4DEE_87E1_0BAC4A15921B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Rbt_Sorter.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Sorter command target

class CRun_Rbt_Sorter
{
public:
	CRun_Rbt_Sorter();           // protected constructor used by dynamic creation
	virtual ~CRun_Rbt_Sorter();

// Attributes
public:
	int PrevStep;
	int RunSortStep;
	int RunUldStep;
	int MoveStep;
	int InitStep;
	int SafetyZone_Step;
	int UldSafetyZone_Step;
	int ReinstateStep;
	int ReinstateStep1;//20121126
	int Module_Pick_Step;
	int TrayPlace_Step;
	int UldModule_Pick_Step;
	int Module_Place_Step;
	int mn_StackerPos;
	int mn_NextPos;

//	int mn_FirstPicker_X_Pos;
	int mn_retry;
	int mn_uldretry;
	int i_DVCSortRepickCnt;
	int i_DVCSortReplaceCnt;
	int i_DVCUldRepickCnt;
	int i_DVCUldReplaceCnt;
	BOOL b_PickerFlag[2];
	BOOL b_FingerFlag[2];
	int mResult[LDULD_PARA];
	int mUldResult[LDULD_PARA];
	int ActionPicker[2][D_INFOSIZE];
	int mdlPlaceActionPicker[2][D_INFOSIZE];
	int TempPicker[D_INFOSIZE];
	int mTempPicker[D_INFOSIZE];
	int UldTempPicker[D_INFOSIZE];
	int mUldTempPicker[D_INFOSIZE];
	long l_PickerWaitTime[2][3]; //[0][] : sorter [1][] : unloader
	long l_FingerWaitTime[2][3];
	int mn_stacker_updn_cyliner[2];
	char mc_alarmcode[10];
	int mn_pre_move_flag[2];
	int	mn_bcr_goodbin_cnt;
	int mn_bcr_rejectbin_cnt;

	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	int mn_Clipretry;//20121126
	double dUldCurPos[10];//20121126
	double dSorterCurPos[10];//20121126

	long l_until_wait_time[2][3];
	long l_ReinStateWaitTime[3];

	double dAbsPos[2];
	double dUldAbsPos[2];
	double md_Robot_X_Pos;
	double md_Robot_Y_Pos;
	double md_Robot_Z_Pos;
	double md_UldRobot_X_Pos;
	double md_UldRobot_Y_Pos;
	double md_UldRobot_Z_Pos;
	double m_dTargetPos;

	int mn_FirstTray_Y_Pos;
	double mCurrent_RowPos;
	double md_CurrentPos;

	int HeadVal;

	long	mlp_AxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
	long	msp_AxisNum[4];

	double md_SortAbsPos[2];
	double md_UldAbsPos[2];

	int mn_hs_cont_err;//2015.0129
	int mn_hs_tot_err;//2015.0129

	int mn_uldz_pos;//2016.0826

// Operations
public:
	void Thread_Run();
	void Run_Sorter_Move();
	void Run_Unload_Move();
	void Run_Init();
//	void Run_Reinstate();
	void Thread_Variable_Initial();
	void Robot_Sorter_BackMovePosValCheck();//20121126
	void OnSorterRbt_FinalPos();//20121126
	void Robot_Unloader_BackMovePosValCheck();//20121126
	void OnUnloaderRbt_FinalPos();//20121126
	void Run_Sorter_Reinstate();//20121126
	void Run_Unload_Reinstate();//20121126

	void Set_PickerUpDown(int nSite, int OnOff, int PickerInfo[2][D_INFOSIZE]);
	int Get_PickerUpDown(int nSite, int OnOff, int PickerInfo[2][D_INFOSIZE]);
	void Set_FingerOnOff(int nSite, int OnOff, int FingerInfo[2][D_INFOSIZE]);
	int Get_FingerOnOff(int nSite, int OnOff, int FingerInfo[2][D_INFOSIZE]);

	int Robot_CurrentPos_Check(int n_site, double x_pos, double y_pos);
	int SorterRobot_AutoMove_Safety_Zone(int n_mode, int n_flag);
	int UldRobot_AutoMove_Safety_Zone(int n_mode, int n_flag);
	int OnPickupModule(int n_pos, int n_flag);
	int OnPlaceModule(int n_Site, double d_XPos, double d_YPos, int n_flag);
	int OnUldPickupModule(int n_flag);
	int OnUldPlaceModuleTray(int n_bin, int n_tray_y);
	int Find_TrayInfo_Count(int n_site, int n_dvcyesno, int n_tray_y_pos);
	int Find_Picker_Count(int n_enable, int n_dvcyesno, int n_bin);


};
extern CRun_Rbt_Sorter Run_Rbt_Sorter;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_RBT_SORTER_H__C9303B6D_E28A_4DEE_87E1_0BAC4A15921B__INCLUDED_)
