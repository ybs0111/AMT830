#if !defined(AFX_RUN_WORKER_BUFFER_1_H__4DA97F7C_3289_40CE_8AE9_E8210D6636F1__INCLUDED_)
#define AFX_RUN_WORKER_BUFFER_1_H__4DA97F7C_3289_40CE_8AE9_E8210D6636F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Worker_Buffer_1.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Worker_Buffer_1 command target

class CRun_Worker_Buffer_1
{
public:
	CRun_Worker_Buffer_1();           // protected constructor used by dynamic creation
	virtual ~CRun_Worker_Buffer_1();

// Attributes
public:
	int RunStep;
	int MoveStep;
	int InitStep;
	int ReinstateStep;

// Operations
public:
	void Thread_Run();
	void Run_Move();
	void Run_Init();
	void Run_Reinstate();
	void Thread_Variable_Initial();

};
extern CRun_Worker_Buffer_1	Run_Worker_Buffer_1;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_WORKER_BUFFER_1_H__4DA97F7C_3289_40CE_8AE9_E8210D6636F1__INCLUDED_)
