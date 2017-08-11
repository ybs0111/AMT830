// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_)
#define AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrueColorToolBar.h"
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "SerialComm.h"     // 시리얼 통신 클래스

#include "NetWork_Comm.h"   // NetWork
class  CNetWork_Comm;

#include "Dialog_Alarm.h"	// 알람 출력 화면 클래스 추가 
class  CDialog_Alarm;		// 모달리스 알람 화면 대화 상자에 대한 인스턴스 선언

#include "Dialog_Event_Msg.h"

// ******************************************************************************
// Initial Dialog Bar 관련 클래스 추가 부분
// ******************************************************************************
#include "Dialog_Title.h"	// 타이틀 정보 출력 대화 상자 클래스 추가 
#include "Dialog_List.h"	// 리스트 정보 출력 대화 상자 클래스 추가 
//  ******************************************************************************

#include "MyBasicData.h"
#include "PPTooltip.h"

#define TMR_FILE_CREATE			861
#define TMR_MAIN_REFRESH		862
#define TMR_MAIN_ELEC_CHK		863
#define TMR_NEW_LOT_IN			864
#define TMR_BOARD_LIVE_CHK		865
#define TMR_LOT_NAME			866
#define TMR_RUNLOTEND_READY		867

#define CALPELLA_A_TIMER	100
#define CALPELLA_B_TIMER	200
#define CALPELLA_C_TIMER	300
#define CALPELLA_D_TIMER	400
#define CALPELLA_E_TIMER	500
#define CALPELLA_F_TIMER	600
#define CALPELLA_G_TIMER	700
#define CALPELLA_H_TIMER	800
#define MAIN_TIMER			900
#define HEATER_READ			1000
#define HEATER_SET			1010

#define TMR_MAIN_SEND_BARCODE 3017
#define TMR_MAIN_RCV_BARCODE  3018

#define TOOLBAR_DRAW_BUTTON_WIDTH 32

class  CClientSocket;
class  CServerSocket;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CServerSocket	*m_p_server[10];
	CClientSocket	*m_p_client[10];
	// **************************************************************************
	// Initial Dialog Bar 관련 클래스 포인터 변수 선언
	// **************************************************************************
	CPPToolTip		m_p_tooltip;
	CDialog_Title	m_wndTitleBar;  // 타이틀 바 대화 상자에 대한 클래스 변수 선언 
	CDialog_List    m_wndListBar;   // 리스트 정보 대화 상자에 대한 클래스 변수 선언 
	// **************************************************************************

	CMyBasicData    mcls_m_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	CMyJamData      mcls_frm_alarm;		// 알람 출력 클래스
	CDialog_Alarm*  mp_alarm_dlg;		// 알람 화면에 대한 포인터 변수 선언

	/* ************************************************************************** */
	/* 시리얼 통신 관련 변수 선언                                                 */
	/* ************************************************************************** */
	SerialComm m_ports[MAX_PORT];       // 시리얼 포트 클래스 변수
	CString mstr_received[MAX_PORT];    // 수신 데이터 임시 저장 변수
	/* ************************************************************************** */

	CDialog_Event_Msg*  mp_msg_dlg;

	int n_summary_flag;
	int nElectrostaticCheckStep;
	CString strElectCommand;
	int nElectChannel;
	long lElectSerialTime[3];

	int mn_calpella_pos;
	int mn_part;
	int mn_board;
	int mn_power_on_pos;
	int mn_power_on_step;
	int mn_power_off_pos;
	int mn_power_off_step;
	int mn_a_station_step;
	int mn_calpella_display_error;
	long mn_Calpella_Wait[3];
	int mn_calpella_display_step;
	int mn_calpella_display_old_pos;
	int mn_power_off_chk_step;
	int mn_power_off_chk_pos;
	int mn_video_chk_step;
	int mn_video_chk_pos;
	CString mstr_received_calpella[MAX_PORT];    // 수신 데이터 임시 저장 변수

	int mn_heater_write_step;
	CString mstrHeaterWCommand;
	long lTempSerialTime[3];

	int mn_new_lot_in_step;
	long ml_wait_time[3];
	long ml_retry_wait[3];

	int mn_bd_live_chk_step;
	int mn_bd_live_chk_no;

	int mn_lot_name_step;

	int mnQueryStep;
	long mnWait_Time[3];
	int mnError_cnt[8];

	int				m_n_rcv_bcr_step;
	long			m_n_rcv_bcr_wait[3];

	int m_nHead;
	CString m_strData;
	SYSTEMTIME m_timeLastDay;
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnMain_BootFlagCheck();
	void OnMainFrame_TrackInDisplay();
	void OnMainFrame_TrackOutDisplay();
	void OnMainFrame_ScrapInfoDisplay();
	int OnMain_Lot_name();
	int OnMain_New_Lot_In();
	void OnAlarm_Destroy();
	void OnSendDataBCR(WPARAM wParam, LPARAM lParam);
	void OnReceiveDataBCR(WPARAM port, LPARAM ch);
	void OnMain_Rcv_GMS(WPARAM port, LPARAM ch);
	void OnMain_Snd_Serial(WPARAM wParam, LPARAM lParam);
	void OnMainFrame_SelectMessageDisplay();
	void OnMainFrame_MessageDisplay();
	void OnMain_Motor_Destroy();
	void OnMain_Port_Close();
	void OnExitData_Saving();
	void OnMain_Thread_Destroy();

	///////////////////////////////////////// 20120514 유성준
	void OnMain_Port_Create(int n_port);
	///////////////////////////////////////// 20120610 유성준
	void OnReceiveDataVision(WPARAM port, LPARAM ch);
	///////////////////////////////////////// 20120620
	void OnReceiveDataLabelPrint(WPARAM port, LPARAM ch);
	///////////////////////////////////// 20120711
	void OnMain_Alarm_Mysql_Write();

	void OnMain_Work_Mysql_Write();
	void OnMain_LotAlarm_Mysql_Write();
	void OnMainLotReadyExecute(int nID);
	/////////////////////////////////////////
	int OnMain_Rcv_Barcode();//20121120

	void OnMain_Thread_Creating();
	void OnMain_Motor_Setting();
	int OnElectrostatic_Check();
	void OnMain_Var_Default_Set();
	void OnMain_Path_Set();
	void OnSwitchToForm(int nForm);
	int OnMenu_Change_Checking();
	void Init_ToolTip();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CTrueColorToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnMain();
	afx_msg void OnBasic();
	afx_msg void OnSetting();
//	afx_msg void OnWait();	//20120517
	afx_msg void OnMotor();
	afx_msg void OnIo();
	afx_msg void OnList();
	afx_msg void OnLevel();
	afx_msg void OnSave();
	afx_msg void OnAlarm();
	afx_msg void OnAbout();
	afx_msg void OnReset();
	afx_msg void OnLock();
	afx_msg void OnExit();
	afx_msg void OnMotTeach();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMotSpeed();
	afx_msg void OnSetInterface();
//	afx_msg void OnSetMaintenance();	//20120515
	afx_msg void OnAdmin();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnListOper();
	afx_msg void OnListDataview();
	afx_msg void OnListStep();	//20120701
	afx_msg void OnFileEditor();
	afx_msg void OnPgmInfo();
	//}}AFX_MSG
	afx_msg LRESULT OnViewChangeMode(WPARAM wParam,LPARAM lParam) ;		// Post Message에 대한 화면 전환 사용자 사용자 정의 메시지 선언 
	afx_msg LRESULT OnMainframe_Work(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMainMessageEvent(WPARAM wParam,LPARAM lParam);
	afx_msg LONG OnCommunication(UINT, LONG);							// RS-232C 시리얼 포트 제어 메시지
	afx_msg LONG OnCommunicationEvent(UINT, LONG);						// RS-232C 시리얼 포트 이벤트 설정 메시지
	afx_msg LRESULT OnDataSend(WPARAM wParam, LPARAM lParam);			// RS-232C 시리얼 포트 통한 데이터 송신 메시지
	
	afx_msg LRESULT OnSerialPort(WPARAM wParam,LPARAM lParam) ;			// 20120514 유성준 시리얼 포트 생성/삭제 메시지 선언 
	afx_msg void OnDataChange(WPARAM wParam,LPARAM lParam) ;		// 20120516
	afx_msg LRESULT OnWork_Command(WPARAM wParam,LPARAM lParam);	//20120608

	afx_msg LRESULT OnCommand_Client_1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCommand_Server_1(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_)
