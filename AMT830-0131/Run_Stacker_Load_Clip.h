#if !defined(AFX_RUN_STACKER_LOAD_CLIP_H__32E3FD61_EAE8_49D0_8B0D_F2055BBAB74C__INCLUDED_)
#define AFX_RUN_STACKER_LOAD_CLIP_H__32E3FD61_EAE8_49D0_8B0D_F2055BBAB74C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Stacker_Load_Clip.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Stacker_Load_Clip command target

class CRun_Stacker_Load_Clip
{
public:
	CRun_Stacker_Load_Clip();           // protected constructor used by dynamic creation
	virtual ~CRun_Stacker_Load_Clip();

// Attributes
public:
	int PrevStep;
	int RunStep;
	int RunTransStep;
	int MoveStep;
	int InitStep;
	int PushStep;
	int TransUpDnStep;
	int ClipOutStep;
	int FixStep;
	int RotatorStep;
	int ReinstateStep;
	int RunInsertStep;
	int Change_step;
	int MoveTransferStep;
	int RunLdClipStep;
	int RunUldClipStep;

	int RunLdTubeStep; //20130204
	int RunUldTubeStep;//20130204

	int mn_LdTube;
	int mn_LdRotateTube;
	char mc_alarmcode[10];
	int mn_UnloadTubeNum;

	int mn_retry;
	int mn_change_clip;
	int mn_Tuberetry;

	BOOL b_PickerFlag;
	long l_PickerWaitTime[3];
	long l_Until_WaitTime[3];
	long l_UntilWaitTime[3];

	int mn_Ldretry;
	long l_Until_Ld_WaitTime[3];
	int mn_Uldretry;
	long l_Until_Uld_WaitTime[3];

	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	double dClipStackerCurPos[10];//20121126


// Operations
public:
	void Thread_Run();
	void Run_Move();
	void Run_ClipOut();
	void Run_Transfer();
	void Run_LoadClip();
	void Run_LoadTube();  //20130204
	void Run_UnloadTube();//20130204
	void Run_UnloadClip();
	void Run_Init();
	void Run_Reinstate();
	void Thread_Variable_Initial();
	void Robot_BackMovePosValCheck();//20121126
	void OnClipStacker_FinalPos();//20121126

	int CheckTransferBeforeMove(int nChange=0);
	int OnLoadTransferMove();
	int OnSetRotatorUpdn(BOOL bUpDn);
	void SetMovePusherCyliner(BOOL bFwdMove);
	int GetMovePusherCyliner(BOOL bFwdMove);
	int OnLoadClipTransferStackerUpDn();
	int OnFixLoadTube();
	int ChangeLoadTray();
};
extern CRun_Stacker_Load_Clip Run_Stacker_Load_Clip;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_STACKER_LOAD_CLIP_H__32E3FD61_EAE8_49D0_8B0D_F2055BBAB74C__INCLUDED_)
