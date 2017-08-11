#if !defined(AFX_RUN_RBT_LOAD_MODULE_H__F08EC344_48C1_4951_88C7_F837640B7016__INCLUDED_)
#define AFX_RUN_RBT_LOAD_MODULE_H__F08EC344_48C1_4951_88C7_F837640B7016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Rbt_Load_Module.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Load_Module command target

class CRun_Rbt_Load_Module
{
public:
	CRun_Rbt_Load_Module();           // protected constructor used by dynamic creation
	virtual ~CRun_Rbt_Load_Module();

// Attributes
public:
	void Thread_Run();
	int Robot_BarcodeVerification();
	void Run_Move();
	void Run_Init();
	void Run_Reinstate();
	void Thread_Variable_Initial();
	void OnLdModuleRbt_FinalPos();//20121126
	int Robot_AutoMove_Safety_Zone(int n_mode, int n_site);
	int Robot_CurrentPos_Check(double y_pos, double z_pos);
	int Robot_TrayDVCPick(int n_bin, int n_tray_y);
	int Robot_ModuleDVCPlace(int n_site, int n_bin);
	int Robot_BeforeModuleDVCPlaceMove(int n_site, int n_bin);
	int Robot_ModuleDVCTrayPlace(int nRow, int ActionPicker[D_INFOSIZE][PICKER_NUM]);
	void Robot_BackMovePosValCheck();//20121126

	int mn_posok;
	// XYZ동작 관련 =====================================================================================
	int PrevStep;
	int RunStep;
	int MoveStep;
	int InitStep;
	int SafetyZone_Step;
	int ReinstateStep;
	int TrayBackStep;
	int TrayMoveStep;
	int TrayPlaceStep;
	int Tray_Pick_Step;
	int Module_Place_Step;
	int Before_Module_Place_Step;
	int mn_pre_move_flag;
	int mn_pickerenable_cnt;
	//int mn_FirstPicker_X_Pos;
	int mn_FirstPicker_Y_Pos;
	int mn_FirstTray_X_Pos;
	int mn_FirstTray_Y_Pos;
	int mCurrent_RowPos[10];
	int mn_FirstPicker_X_Pos;
	char mc_alarmcode[10];
	int i_PartialPickCnt;

	double md_CurrentPos;
	int i_DVCRepickCnt;
	int mn_NextPos;

	int MDHeadVal;

	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	double dLDModuleCurPos[10];//20121126
	int mn_Clipretry;//20121126
	
	int mMultiAxis[2];
	double md_AbsPos[2];
	double md_Velocity[2];
	double md_Robot_Pos;
	double md_Robot_TrayPlacePos;
	double md_Robot_PlacePos;
	double md_SafetyPos;
	
	int n_pickeroff_bufferoff_flag[PICKER_NUM];
	BOOL b_PickerFlag[PICKER_NUM];
	int ActionPicker[D_INFOSIZE][PICKER_NUM];
	int FingerInfo[D_INFOSIZE][PICKER_NUM];
	int mTempPicker[D_INFOSIZE][PICKER_NUM];
	BOOL b_FingerFlag[PICKER_NUM];
	long l_PickerWaitTime[PICKER_NUM][3];
	long l_FingerWaitTime[PICKER_NUM][3];
	int mResult[PICKER_NUM];
	int mn_PickLoad;

	long l_until_wait_time[3];
	long l_ReinStateWaitTime[3];

	int HeadVal;

	void Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	int Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	void Set_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM]);
	int Get_FingerOnOff(int OnOff, int FingerInfo[D_INFOSIZE][PICKER_NUM]);
	
// Operations
public:

};
extern CRun_Rbt_Load_Module Run_Rbt_Load_Module;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_RBT_LOAD_MODULE_H__F08EC344_48C1_4951_88C7_F837640B7016__INCLUDED_)
