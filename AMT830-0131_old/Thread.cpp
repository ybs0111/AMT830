// Thread.cpp: implementation of the Thread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iostream.h"
#include "afxmt.h"
#include "Thread.h"

#include "Variable.h"  // 전역 변수 정의 클래스

//******************************************************************************
//쓰레드 동작 관련 클래스 추가                                                  
//******************************************************************************
#include "Run_Handler_Check.h"
#include "Run_TowerLamp.h"
#include "RUN_IO_ReadWrite.h"
#include "Run_Rbt_Load_Module.h"
#include "Run_Stacker_Load_Clip.h"
#include "Run_Rbt_Work_Buffer.h"
#include "Public_Function.h"
#include "Run_Rbt_Sorter.h"
#include "Run_Stacker_Sorter.h"
#include "Run_Stacker_Load_LeftHeat.h"
#include "Run_Stacker_Load_RightHeat.h"
#include "Run_Stacker_Load_Module.h"
#include "Run_Rbt_Load_LeftHeat.h"
#include "Run_Rbt_Load_RightHeat.h"
#include "Run_Rbt_Load_Clip.h"
#include "Run_Rbt_VisionLabel.h"
//******************************************************************************

#include "MyJamData.h"  // 알람 출력 정보 로딩 및 알람 메시지 저장 클래스 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//******************************************************************************
//알람 출력 클래스 변수 선언                                                    
//******************************************************************************
CMyJamData   mcls_alarm_disp;  // 발생 알람 출력 정보 로딩 클래스 변수
//******************************************************************************

//******************************************************************************
//쓰레드 및 핸들 저장 변수 선언                                                 
//******************************************************************************
CWinThread  *m_thread[20];
HANDLE		hThrHandle[20];
//******************************************************************************

//******************************************************************************
//전역 오퍼레이션 쓰레드 함수 선언한다                                          
//******************************************************************************
UINT OnThread_Handler_Check(LPVOID  lParam);			// m_thread[0]
UINT OnThread_Tower_Lamp_Check(LPVOID  lParam);			// m_thread[1]
UINT OnThread_Alarm_Display(LPVOID  lParam);			// m_thread[2]
UINT OnThread_IO(LPVOID  lParam);						// m_thread[3]
UINT OnThread_Work_Buffer(LPVOID  lParam);				// m_thread[4]
UINT OnThread_Work_Buffer_2(LPVOID  lParam);			// m_thread[5]
UINT OnThread_Work_Buffer_3(LPVOID  lParam);			// m_thread[6]
UINT OnThread_Work_Buffer_4(LPVOID  lParam);			// m_thread[7]
UINT OnThread_Rbt_Load_Module(LPVOID  lParam);			// m_thread[8]
UINT OnThread_Stacker_Load_Module(LPVOID  lParam);		// m_thread[9]
UINT OnThread_Rbt_Load_LeftHeat(LPVOID  lParam);		// m_thread[10]
UINT OnThread_Stacker_Load_LeftHeat(LPVOID  lParam);	// m_thread[11]
UINT OnThread_Rbt_Load_RightHeat(LPVOID  lParam);		// m_thread[12]
UINT OnThread_Stacker_Load_RightHeat(LPVOID  lParam);	// m_thread[13]
UINT OnThread_Rbt_Load_Clip(LPVOID  lParam);			// m_thread[14] Clip insert
UINT OnThread_Rbt_Module_Clamp(LPVOID  lParam);			// m_thread[15]
UINT OnThread_Stacker_Load_Clip(LPVOID  lParam);		// m_thread[16]
UINT OnThread_Rbt_VisionLabel(LPVOID  lParam);			// m_thread[17]
UINT OnThread_Rbt_Sorter(LPVOID  lParam);				// m_thread[18]
UINT OnThread_Stacker_Sorter(LPVOID  lParam);			// m_thread[19]
//******************************************************************************

CMutex mutex;
int All_Stop;		// 쓰레드 상태 플래그
int threadtimer[3];
int countflag=0;

//******************************************************************************
// 스위치 검사 동작 제어 쓰레드 함수                                            
//******************************************************************************
UINT OnThread_Handler_Check(LPVOID lParam)  // m_Thread[0]
{
	CSingleLock sing(&mutex);

	while(!All_Stop)
	{
		sing.Lock();

		Run_Handler_Check.ButtonCheck_Start();
 		Run_Handler_Check.ButtonCheck_Stop();
 		Run_Handler_Check.ButtonCheck_Alarm_Clear();
// 		Run_Handler_Check.ButtonCheck_Machine_Reset();
 		Run_Handler_Check.ButtonCheck_BuzzerOff();
// 		Run_Handler_Check.SafetyCheck_Door();
		Run_Handler_Check.LightCurtainCheck();
		Run_Handler_Check.ButtonCheck_Manual();
		Run_Handler_Check.ButtonCheck_Auto();
		Run_Handler_Check.HandlerCheck_RejectTrayChange();
     	Run_Handler_Check.CheckLdClipCoverOnOff();             
	    Run_Handler_Check.CheckUldClipCoverOnOff();        // 20130605    
		
		sing.Unlock();
		Sleep(1);
	}

	return TRUE;
}
//******************************************************************************

//******************************************************************************
// 타워 램프 출력 동작 제어 쓰레드 함수                                         
//******************************************************************************
UINT OnThread_Tower_Lamp_Check(LPVOID lParam)  // m_Thread[1]
{
	CSingleLock sing(&mutex);

	while(!All_Stop)
	{
		sing.Lock();

/*		st_inspect.threadcount++;		
		if(countflag == 0) 
		{
			threadtimer[0] = GetCurrentTime();
			countflag =1;
			st_inspect.threadcount=0;
		}
		else 
		{
			threadtimer[1] = GetCurrentTime();
			threadtimer[2] = threadtimer[1] - threadtimer[0];
			if(threadtimer[2] > 1000)
			{
				countflag = 0;
				st_inspect.threadcount = 0;
			}
		}
*/
		Run_TowerLamp.Thread_Run();
		Run_TowerLamp.Run_FormChange();

		sing.Unlock();
		Sleep(1);
	}

	return TRUE;
}
//******************************************************************************

//******************************************************************************
// 장비 동작 중 발생한 알람 출력하기 위한 쓰레드 함수                           
//******************************************************************************
UINT OnThread_Alarm_Display(LPVOID lParam)  // m_Thread[2]
{
	CSingleLock sing(&mutex);

	while(!All_Stop)
	{
		sing.Lock();

 		mcls_alarm_disp.On_Alarm_Display(); // 알람 발생한 경우 알람 화면 출력 함수
		 
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************

//******************************************************************************
// IO 동작을 위한 쓰레드 함수
//******************************************************************************
UINT OnThread_IO(LPVOID lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		RUN_IO_ReadWrite.Run_Total_IORead();
	
		sing.Unlock();
		Sleep(1);
	}

	return TRUE;
}
//******************************************************************************
UINT OnThread_Work_Buffer(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Rbt_Work_Buffer.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}

	return TRUE;
}
//******************************************************************************
UINT OnThread_Work_Buffer_2(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
//		Run_Worker_Buffer_2.Run_Thread();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************
UINT OnThread_Work_Buffer_3(LPVOID  lParam)	
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
//		Run_Worker_Buffer_3.Run_Thread();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************
UINT OnThread_Work_Buffer_4(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
//		Run_Worker_Buffer_4.Run_Thread();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************
UINT OnThread_Rbt_Load_Module(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Rbt_Load_Module.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}

	return TRUE;
}
//******************************************************************************
UINT OnThread_Stacker_Load_Module(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Stacker_Load_Module.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}

	return TRUE;
}
//******************************************************************************
UINT OnThread_Rbt_Load_LeftHeat(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Rbt_Load_LeftHeat.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}
	
	return TRUE;
}
//******************************************************************************
UINT OnThread_Stacker_Load_LeftHeat(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Stacker_Load_LeftHeat.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************
UINT OnThread_Rbt_Load_RightHeat(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Rbt_Load_RightHeat.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE; 
}
//******************************************************************************
UINT OnThread_Stacker_Load_RightHeat(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Stacker_Load_RightHeat.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************
UINT OnThread_Rbt_Load_Clip(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Rbt_Load_Clip.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************
UINT OnThread_Stacker_Load_Clip(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Stacker_Load_Clip.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************
UINT OnThread_Rbt_VisionLabel(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Rbt_VisionLabel.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************
UINT OnThread_Rbt_Sorter(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Rbt_Sorter.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************
UINT OnThread_Stacker_Sorter(LPVOID  lParam)
{
	CSingleLock sing(&mutex);
	
	while (!All_Stop)
	{
		sing.Lock();
		
		Run_Stacker_Sorter.Thread_Run();
	
		sing.Unlock();
		Sleep(1);
	}
	return TRUE;
}
//******************************************************************************
