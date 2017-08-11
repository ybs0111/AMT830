#if !defined(AFX_RUN_RBT_WORK_BUFFER_H__84D42513_23D7_4A47_83EB_7952D211DF13__INCLUDED_)
#define AFX_RUN_RBT_WORK_BUFFER_H__84D42513_23D7_4A47_83EB_7952D211DF13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Rbt_Work_Buffer.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_Work_Buffer command target

class CRun_Rbt_Work_Buffer
{
public:
	CRun_Rbt_Work_Buffer();           // protected constructor used by dynamic creation
	virtual ~CRun_Rbt_Work_Buffer();

// Attributes
public:
	int MoveStep;
	int InitStep;
	int ReinstateStep;
	int RunBuff1Step;
	int RunBuff2Step;
	int RunBuff3Step;
	int RunBuff4Step;
	int HS1Step;
	int HS2Step;
	int HS3Step;
	int HS4Step;

	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	int mn_Clipretry;//20121126
	double dWorkBufferCurPos[10];//20121126

	long l_ReinStateWaitTime[3];

	char mc_alarmcode[10];
	int mn_retry;
	int mn_clipretry[4];
	long l_Unitil_WaitTime[3];//20120816
	long l_ClipWaitTime[4][3];

	long l_Work1_WaitTime[3];
	long l_Work2_WaitTime[3];
	long l_Work3_WaitTime[3];
	long l_Work4_WaitTime[3];

// Operations
public:
	void Thread_Run();
	void Run_Init();
	void Run_Reinstate();
	void Thread_Variable_Initial();
	void Run_Worker_Buffer1();
	void Run_Worker_Buffer2();
	void Run_Worker_Buffer3();
	void Run_Worker_Buffer4();
	void Run_HS1_OnFix();
	void Run_HS2_OnFix();
	void Run_HS3_OnFix();
	void Run_HS4_OnFix();
	void Robot_BackMovePosValCheck();//20121126
	void OnWorkBufferRbt_FinalPos();//20121126

};
extern CRun_Rbt_Work_Buffer Run_Rbt_Work_Buffer;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_RBT_WORK_BUFFER_H__84D42513_23D7_4A47_83EB_7952D211DF13__INCLUDED_)
