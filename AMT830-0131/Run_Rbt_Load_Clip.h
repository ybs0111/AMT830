#if !defined(AFX_RUN_RBT_LOAD_CLIP_H__D98C2194_F7F7_47EE_A757_F696B59428CF__INCLUDED_)
#define AFX_RUN_RBT_LOAD_CLIP_H__D98C2194_F7F7_47EE_A757_F696B59428CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Rbt_Load_Clip.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Load_Clip command target


class CRun_Rbt_Load_Clip
{
public:
	CRun_Rbt_Load_Clip();           // protected constructor used by dynamic creation
	virtual ~CRun_Rbt_Load_Clip();

// Attributes
public:
	int PrevStep;
	int RunInsertStep;
	int RunClampStep;
	int MoveStep;
	int InitStep;
	int RotatorStep;
	int RunBufferStep;
	int ClipBufferStep;
	int ClipPickStep;
	int ClipPlaceStep;
	int ClampRbtStep;
	int ReinstateStep;
	int mn_NextPos;
	int InsertStep;
	int DropStep;
	int LdClipStep;
	int ClipLotStep;
	int PickClipStep;
	int UldClipStep;

	int mn_retry;
	int mn_Clipretry;
	char mc_alarmcode[10];
	int mn_pre_move_flag;
	int mn_pre_insert_flag;
	int mn_pre_clamp_flag;

	BOOL b_PickerFlag;
	int ActionPicker[D_INFOSIZE];
	int mn_Result;
	long l_PickerWaitTime[3];
	double md_ClipAbsPos;
	long l_UntilWaitTime[3];
	double md_CurrentPos;
	long l_ClipWaitTime[3];
	double md_clampPos;

	long l_ReinStateWaitTime[3];

	int HeadVal;

	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	double dClipCurPos[10];//20121126

	int Clip_remove_Cnt;//2014,0228
// Operations
public:
	void Thread_Run();
	void Run_Init();
	void Run_Reinstate();
	void Thread_Variable_Initial();
	void Run_ClipInsertMove();
	void Run_ModuleClampMove();
	void Run_ClipBufferMove();
	void Run_ClipLotEndMove();
	int LdUldClipBufferPos(int nPos);
	int OnPickupClip(int nPos);
	int OnPlaceClip(int nPos);
	int OnClampRbtMove(int nPos);

	void Set_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE]);
	int Get_PickerUpDown(int OnOff, int PickerInfo[D_INFOSIZE]);

	int Run_RemoveClip();// Drop clip on bin
	int PickUpClipForBin();
	int LoadClipMove();
	int UnloadClipMove();

	void Robot_BackMovePosValCheck();//20121126
	void OnClipRbt_FinalPos();//20121126

};

/////////////////////////////////////////////////////////////////////////////
extern CRun_Rbt_Load_Clip Run_Rbt_Load_Clip;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_RBT_LOAD_CLIP_H__D98C2194_F7F7_47EE_A757_F696B59428CF__INCLUDED_)
