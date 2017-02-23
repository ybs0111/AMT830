// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_)
#define AFX_MAINFRM_H__2C6D2CE2_883C_498B_9324_12F2734B9EC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrueColorToolBar.h"
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include "SerialComm.h"     // �ø��� ��� Ŭ����

#include "NetWork_Comm.h"   // NetWork
class  CNetWork_Comm;

#include "Dialog_Alarm.h"	// �˶� ��� ȭ�� Ŭ���� �߰� 
class  CDialog_Alarm;		// ��޸��� �˶� ȭ�� ��ȭ ���ڿ� ���� �ν��Ͻ� ����

#include "Dialog_Event_Msg.h"

// ******************************************************************************
// Initial Dialog Bar ���� Ŭ���� �߰� �κ�
// ******************************************************************************
#include "Dialog_Title.h"	// Ÿ��Ʋ ���� ��� ��ȭ ���� Ŭ���� �߰� 
#include "Dialog_List.h"	// ����Ʈ ���� ��� ��ȭ ���� Ŭ���� �߰� 
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
	// Initial Dialog Bar ���� Ŭ���� ������ ���� ����
	// **************************************************************************
	CPPToolTip		m_p_tooltip;
	CDialog_Title	m_wndTitleBar;  // Ÿ��Ʋ �� ��ȭ ���ڿ� ���� Ŭ���� ���� ���� 
	CDialog_List    m_wndListBar;   // ����Ʈ ���� ��ȭ ���ڿ� ���� Ŭ���� ���� ���� 
	// **************************************************************************

	CMyBasicData    mcls_m_basic;		// �⺻ ���� ���� �ε� �� ���� Ŭ����
	CMyJamData      mcls_frm_alarm;		// �˶� ��� Ŭ����
	CDialog_Alarm*  mp_alarm_dlg;		// �˶� ȭ�鿡 ���� ������ ���� ����

	/* ************************************************************************** */
	/* �ø��� ��� ���� ���� ����                                                 */
	/* ************************************************************************** */
	SerialComm m_ports[MAX_PORT];       // �ø��� ��Ʈ Ŭ���� ����
	CString mstr_received[MAX_PORT];    // ���� ������ �ӽ� ���� ����
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
	CString mstr_received_calpella[MAX_PORT];    // ���� ������ �ӽ� ���� ����

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

	///////////////////////////////////////// 20120514 ������
	void OnMain_Port_Create(int n_port);
	///////////////////////////////////////// 20120610 ������
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
	afx_msg LRESULT OnViewChangeMode(WPARAM wParam,LPARAM lParam) ;		// Post Message�� ���� ȭ�� ��ȯ ����� ����� ���� �޽��� ���� 
	afx_msg LRESULT OnMainframe_Work(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMainMessageEvent(WPARAM wParam,LPARAM lParam);
	afx_msg LONG OnCommunication(UINT, LONG);							// RS-232C �ø��� ��Ʈ ���� �޽���
	afx_msg LONG OnCommunicationEvent(UINT, LONG);						// RS-232C �ø��� ��Ʈ �̺�Ʈ ���� �޽���
	afx_msg LRESULT OnDataSend(WPARAM wParam, LPARAM lParam);			// RS-232C �ø��� ��Ʈ ���� ������ �۽� �޽���
	
	afx_msg LRESULT OnSerialPort(WPARAM wParam,LPARAM lParam) ;			// 20120514 ������ �ø��� ��Ʈ ����/���� �޽��� ���� 
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
