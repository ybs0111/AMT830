#ifndef _AFX_THREAD_H__
#define _AFX_THREAD_H__

// ******************************************************************************
// 쓰레드 및 쓰레드 핸들 저장 변수 선언                                          
// ******************************************************************************
extern CWinThread  *m_thread[20];
extern HANDLE		hThrHandle[20];
// ******************************************************************************

// ******************************************************************************
// 전역 쓰레드 함수 선언한다                                                     
// ******************************************************************************
extern UINT  OnThread_Handler_Check(LPVOID  lParam);			// m_thread[0]
extern UINT  OnThread_Tower_Lamp_Check(LPVOID  lParam);			// m_thread[1]
extern UINT  OnThread_Alarm_Display(LPVOID  lParam);			// m_thread[2]
extern UINT  OnThread_IO(LPVOID  lParam);						// m_thread[3]
extern UINT  OnThread_Work_Buffer(LPVOID  lParam);				// m_thread[4]
extern UINT  OnThread_Work_Buffer_2(LPVOID  lParam);			// m_thread[5]
extern UINT  OnThread_Work_Buffer_3(LPVOID  lParam);			// m_thread[6]
extern UINT  OnThread_Work_Buffer_4(LPVOID  lParam);			// m_thread[7]
extern UINT  OnThread_Rbt_Load_Module(LPVOID  lParam);			// m_thread[8]
extern UINT  OnThread_Stacker_Load_Module(LPVOID  lParam);		// m_thread[9]
extern UINT  OnThread_Rbt_Load_LeftHeat(LPVOID  lParam);		// m_thread[10]
extern UINT  OnThread_Stacker_Load_LeftHeat(LPVOID  lParam);	// m_thread[11]
extern UINT  OnThread_Rbt_Load_RightHeat(LPVOID  lParam);		// m_thread[12]
extern UINT  OnThread_Stacker_Load_RightHeat(LPVOID  lParam);	// m_thread[13]
extern UINT  OnThread_Rbt_Load_Clip(LPVOID  lParam);			// m_thread[14] Clip insert
extern UINT  OnThread_Rbt_Module_Clamp(LPVOID  lParam);			// m_thread[15]
extern UINT  OnThread_Stacker_Load_Clip(LPVOID  lParam);		// m_thread[16]
extern UINT  OnThread_Rbt_VisionLabel(LPVOID  lParam);			// m_thread[17]
extern UINT  OnThread_Rbt_Sorter(LPVOID  lParam);				// m_thread[18]
extern UINT  OnThread_Stacker_Sorter(LPVOID  lParam);			// m_thread[19]
 // ******************************************************************************

extern int All_Stop;  // 쓰레드 상태 플래그

#endif