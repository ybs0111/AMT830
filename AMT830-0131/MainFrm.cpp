// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Handler.h"

#include "MainFrm.h"

// ******************************************************************************
// ��ȭ ���� �� �ܺ� Ŭ���� �߰�                                                 
// ******************************************************************************
#include "MenuHead.h"		// ȭ�鿡 ���� ��� ���ǵ� Ŭ���� �߰� 
#include "Dialog_Menu.h"	// �޴� ���� ��ȭ ���� Ŭ���� �߰� 

#include "Dialog_About.h"	// ���α׷� About ��ȭ ���� Ŭ���� �߰� 
#include "Dialog_Exit.h"	// ���� ���α׷� ���� ȭ�� Ŭ���� �߰� 
#include "Dialog_Pass_Check.h"
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyBoard.h"
#include "Dialog_Pass_Check.h"
#include "Public_Function.h"	// ���� ���� �� I/O ���� ���� Ŭ����
#include "RUN_IO_ReadWrite.h"
#include "Screen_Pgm_Info.h"
#include "Dialog_Scrap_Info_View.h"
#include "Dialog_Track_Out.h"
#include "ServerSocket.h"

#include "ComizoaPublic.h"
#include "Cmmsdk.h"

#include "AVersion.h"
#include "CtlBd_Library.h"

#include "io.h" 
#include "FAS_HSSI.h"
#include "FastechPublic_IO.h"

/////////////////////// 20120515
#include "Screen_Work_Info.h"
#include "Dialog_Work_Wait_Time.h"//20120517
#include "Screen_List_Step.h"	//20120701

/////////////////////// 
#include "ACriticalSection.h"
#include "AMTRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMutex s_mutex;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

st_handler_param			st_handler;
st_robot_picker_param		st_robot_picker[PICKER_NUM];

st_setting_param			st_set;
st_message_param			st_msg;
st_time_param				st_time;
st_lamp_param				st_lamp;
st_basic_param				st_basic;
st_alarm_param				alarm;
st_dsp_alarm_param			st_alarm;
st_work_param				st_work;
st_testreference_param		st_testreference;
st_network_param			st_Network[NETWORK_CNT];
st_modulemap_size			st_modulemap;
st_test_param				st_test;
st_motor_param				st_motor[MOT_MAXMOTOR];
st_path_param				st_path;
st_nw_param					st_NW;
st_io_param					st_io;
st_sync_param				st_sync;
st_ctlbd_param				st_ctlbd;
st_ctlbd_param				st_reco_ctlbd;  
st_serial_param				st_serial;
st_linearmotion_param		st_linearmot[8];
st_coordmotion_param		st_coordmot[16];
st_module_datamap_param		st_module_datamap[2][13];
st_client_param				st_client[10];
st_server_param				st_server[10];
st_io_wait_param			st_wait;

st_time_database			st_timedb;
st_alarm_database			st_alarmdb;
st_ground_database			st_grounddb;

st_buffer_database			st_bufferdb;

st_data_param				st_data;	//20120514 ������
st_other_param				st_other;	//20120514 ������			// ��Ÿ ���� ���� ����ü ����
struct st_serial_info		rs_232;		//20120514 ������

st_part_motor_info			st_motor_info;
st_part_info				st_part;
st_buffer_tray_param		st_buffer_tray[MAX_SITE_INFO];
st_traybuffer_param			st_traybuffer[MAX_SITE_INFO];
st_barcode_param			st_barcode;
st_picker_param				st_picker;

st_interface_param			st_interface;	//20120620
st_lot_database				st_lotdb;

st_ani_param				st_ani; // 20130513

st_MDRbt			*MDhead;
st_MDRbt			*MDtail;
st_LHSRbt			*LHShead;
st_LHSRbt			*LHStail;
st_RHSRbt			*RHShead;
st_RHSRbt			*RHStail;
st_CLIPRbt			*CLIPhead;
st_CLIPRbt			*CLIPtail;
st_ULDRbt			*ULDhead;
st_ULDRbt			*ULDtail;

////2014,1210
st_thread_Run_Rbt_Load_Clip_param  th_reco_Run_Rbt_Load_Clip;
st_thread_Run_Rbt_Load_LeftHeat_param  th_reco_Run_Rbt_Load_LeftHeat;
st_thread_Run_Rbt_Load_Module_param  th_reco_Run_Rbt_Load_Module;
st_thread_Run_Rbt_Load_RightHeat_param  th_reco_Run_Rbt_Load_RightHeat;
st_thread_Run_Rbt_Sorter_param  th_reco_Run_Rbt_Sorter;
st_thread_Run_Rbt_VisionLabel_param  th_reco_Run_Rbt_VisionLabel;
st_thread_Run_Rbt_Work_Buffer_param  th_reco_Run_Rbt_Work_Buffer;
st_thread_Run_Stacker_Load_Clip_param  th_reco_Run_Stacker_Load_Clip;
st_thread_Run_Stacker_Load_Module_param  th_reco_Run_Stacker_Load_Module;
st_thread_Run_Stacker_Load_RightHeat_param  th_reco_Run_Stacker_Load_RightHeat;
st_thread_Run_Stacker_Sorter_param  th_reco_Run_Stacker_Sorter;
////


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONDBLCLK()
	ON_WM_NCRBUTTONDOWN()
	ON_COMMAND(ID_MAIN, OnMain)
	ON_COMMAND(ID_BASIC, OnBasic)
	ON_COMMAND(ID_SETTING, OnSetting)
//	ON_COMMAND(ID_WAIT, OnWait)	//20120517
	ON_COMMAND(ID_MOTOR, OnMotor)
	ON_COMMAND(ID_IO, OnIo)
	ON_COMMAND(ID_LIST, OnList)
	ON_COMMAND(ID_LEVEL, OnLevel)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_COMMAND(ID_ALARM, OnAlarm)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_RESET, OnReset)
	ON_COMMAND(ID_LOCK, OnLock)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_MOT_TEACH, OnMotTeach)
	ON_WM_TIMER()
	ON_COMMAND(ID_MOT_SPEED, OnMotSpeed)
	ON_COMMAND(ID_SET_INTERFACE, OnSetInterface)
//	ON_COMMAND(ID_SET_MAINTENANCE, OnSetMaintenance)	//20120515
	ON_COMMAND(ID_ADMIN, OnAdmin)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_LIST_OPER, OnListOper)
	ON_COMMAND(ID_LIST_DATAVIEW, OnListDataview)
	ON_COMMAND(ID_LIST_STEP, OnListStep)	//20120701

	ON_COMMAND(ID_FILE_EDITOR, OnFileEditor)
	ON_COMMAND(ID_PGM_INFO, OnPgmInfo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FORM_CHANGE, OnViewChangeMode)					// Post Message�� ���� ȭ�� ��ȯ ����� ����� ���� �޽��� ����
	ON_MESSAGE(WM_MAINFRAME_WORK, OnMainframe_Work)
	ON_MESSAGE(WM_MAIN_EVENT, OnMainMessageEvent)					// �̺�Ʈ ���� �޽���
	ON_MESSAGE(WM_COMM_DATA, OnCommunication)						// RS-232C �ø��� ��Ʈ ���� �޽���
	ON_MESSAGE(WM_COMM_EVENT, OnCommunicationEvent)					// RS-232C �ø��� ��Ʈ �̺�Ʈ ���� �޽���
	ON_MESSAGE(WM_DATA_SEND, OnDataSend)							// RS-232C �ø��� ��Ʈ ���� ������ �۽� �޽���
	ON_MESSAGE(WM_DATA_CHANGE, OnDataChange)	//20120516 ������
	ON_MESSAGE(WM_SERIAL_PORT, OnSerialPort)	//20120514 ������
	ON_MESSAGE(WM_WORK_COMMAND, OnWork_Command)	//20120608
	ON_MESSAGE(WM_CLIENT_MSG_1, OnCommand_Client_1)					// ���� ���õ� �۾�(Reserved)
	ON_MESSAGE(WM_SERVER_MSG_1, OnCommand_Server_1)					// Network���õ� �۾��� ����Ѵ�.


END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	int i;
	///////////////////////////////////// 20120516
	for(i=0; i<10; i++)
	{
		m_p_client[i] = NULL;
		m_p_server[i] = NULL;

		st_client[i].n_connect	= NO;
		st_server[i].n_connect	= NO;
	}
	/////////////////////////////////////

	st_work.b_Barcode_Create_Btn_Use = TRUE;//2014,0411

	st_handler.lTimeTable[0] = GetCurrentTime();					// Time DB�� �����ϱ� ����.
	st_timedb.mnSectionBuffer = dSTOP;

	FAS_IO.mn_simulation_mode = 0; //ybs 2013,0508 
	COMI.mn_simulation_mode = 0; //ybs 2013,0508

	OnMain_Var_Default_Set();

	MyJamData.On_Alarm_Info_Load();

	Func.IO_Map_Initialize();										// I/O ��� ��ȣ �� ��巹�� ���� ���� �Լ�
	
	Func.InitNode(); // 20130314

	//////////////////////////////// 20120620
	for(i=1; i<MAX_PORT; i++)
	{
		OnMain_Port_Create(i);
	}


}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	char cJamcode[10] = {NULL};

	CString str_temp;
	int i = 0, j = 0;

	st_basic.mb_controlpanel_enable = false;

	st_handler.hWnd = GetSafeHwnd() ;  // ���� �����ӿ� ���� �ڵ� ���� ����

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndToolBar.SetBorders(1, 1, 1, 1);

	if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	m_wndToolBar.LoadTrueColorToolBar(TOOLBAR_DRAW_BUTTON_WIDTH,
								   IDB_TOOLBAR_DRAW,
		                           IDB_TOOLBAR_DRAW_HOT,
								   IDB_TOOLBAR_DRAW_DISABLED);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);
//	DockControlBar(&m_wndToolBar);					// Docking ����� �ƴϴ�.

	// **************************************************************************
    // Text ���� Icon ������ ����
    // **************************************************************************
	SIZE size1={32, 32}, size2={62, 49};
	m_wndToolBar.SetSizes(size2, size1);
	// **************************************************************************

	// **************************************************************************
    // Ÿ��Ʋ ���� ��� Initial Dialog bar ����
    // **************************************************************************
	if (!m_wndTitleBar.Create(this, IDD_DIALOG_TITLE, CBRS_ALIGN_TOP, IDD_DIALOG_TITLE))
	{
		TRACE0("Failed to create my init bar\n");
		return -1;      // fail to create
	}
	st_handler.cwnd_title = &m_wndTitleBar;
	// **************************************************************************
	
	mcls_m_basic.OnAdmin_Data_Load();

	// **************************************************************************
    // ����Ʈ ���� ��� Initial Dialog bar ����
	// -> ���� �� �ؿ� ��ġ�ϵ��� �Ϸ��� �� �κ��� ���� �� ���� �κ� ���� �ø���.
    // **************************************************************************
	if (!m_wndListBar.Create(this, IDD_DIALOG_LIST, CBRS_ALIGN_BOTTOM, IDD_DIALOG_LIST))
	{
		TRACE0("Failed to create my information bar\n");
		return -1;      // fail to create
	}
	st_handler.cwnd_list = &m_wndListBar;
	// **************************************************************************

	st_handler.mstr_handler_name = "AMT830";
	st_handler.mstr_handler_version = "Ver 1.0.2";
	str_temp = _T("2016,0516");
	st_handler.mstr_handler_date = str_temp;
	
	// **************************************************************************
	// ���� ���α׷� Ÿ��Ʋ ���� ���
	// **************************************************************************
	CString strPgmOption;
	strPgmOption.Format("%s [Handler] : %s_%s", st_handler.mstr_handler_name, st_handler.mstr_handler_version, st_handler.mstr_handler_date);
	
	// **************************************************************************
    // ���� ���α׷��� ���� Ÿ��Ʋ ���� ���
    // ************************************************************************** 
	SetWindowText(strPgmOption);	// Ÿ��Ʋ ���� ��� 
	CenterWindow();					// ���� ���� ���α׷��� �� �߾ӿ� ��ġ
	// **************************************************************************

	// **************************************************************************
    // ���� ���� �� I/O ���� �ʱ�ȭ �� ��� ��Ʈ Ŭ���� �۾� �����Ѵ�        
    // **************************************************************************

	Func.IO_Board_Initialize();				// IO ���� ���̺귯�� �ʱ�ȭ �Լ�

	RUN_IO_ReadWrite.Init_Total_IORead(0);
	
	Func.OnSet_IO_Out_Port_Clear();			// I/O ������ ��� ��Ʈ �ʱ�ȭ �Լ�
	
	st_handler.mn_mot_brd_initial = COMI.Initialize_MotBoard(st_path.mstr_motor);				// MOTOR ���� �ʱ�ȭ ���� �˻��Ѵ�

	if (COMI.mn_simulation_mode == 1)
	{
		st_handler.mn_mot_brd_initial = TRUE;
	}
	

	mcls_m_basic.OnMotorSpeed_Set_Data_Load();
	mcls_m_basic.OnBasic_Data_Load();							// ������ ���� ���� ���� ������ �ε��ϴ� �Լ�
	mcls_m_basic.OnWaitTime_Data_Load();
	mcls_m_basic.OnMaintenance_Data_Load();
	mcls_m_basic.OnSkip_Data_Load();	//20120525
	mcls_m_basic.OnBasic_Label_Data_Load();	//20120531

  	mcls_m_basic.OnBasic_Part_Number_Load();	//20121220

	Func.PartNumber_Check();

	
/*	if (st_picker.n_module_ldrbt_enable[0] == 1 && st_picker.n_module_ldrbt_enable[1] == 1 )	
	{
		sprintf(cJamcode, "990001");
		CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
	}
	else if (st_picker.n_heat_leftldrbt_enable[0] == 1 && st_picker.n_heat_leftldrbt_enable[1] == 1)			
	{
		sprintf(cJamcode, "990002");
		CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
	}
	else if (st_picker.n_heat_rightldrbt_enable[0] == 1 && st_picker.n_heat_rightldrbt_enable[1] == 1)						
	{
		sprintf(cJamcode, "990003");
		CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
	}*/


	//////////////////////////////////////////20120604
	st_barcode.b_barcode_first_load = TRUE;	
	mcls_m_basic.OnBarcode_Label_Data_Load();
	//////////////////////////////////////////

	
	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);	
	}
	if (st_handler.cwnd_title != NULL)	
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
		//////////////////////////////////////////////////////////////////////// 20120525
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, SKIP_MODE, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, BCR_MODE, 0);
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DIRECTION_MODE, 0);

		////////////////////////////////////////////////////////////////////////

	}

	g_ver.SetVersion();


	OnMain_Motor_Setting();
	
	OnMain_Thread_Creating();

	Init_ToolTip();
	
	// **************************************************************************
    // �ʱ�ȭ ���� ȭ������ ��ȯ�Ͽ� ��� �ʱ�ȭ �۾� �����Ѵ�                   
    // **************************************************************************
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 3);  // �ʱ�ȭ ���� ȭ�� ��ȯ 
	// **************************************************************************
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_1, SERVER_CONNECT, 0); // LABEL SERVER 20121226 �ӽ�
	::PostMessage(st_handler.hWnd, WM_SERVER_MSG_7, SERVER_CONNECT, 0); // SEC   SERVER 20121226 �ӽ�

	//	Func.SetActionParam();

	SetTimer(TMR_MAIN_REFRESH, 500, NULL);  // ����Ʈ ���� ���� Ÿ�̸�
	SetTimer(TMR_FILE_CREATE, 1000, NULL);  // ����Ʈ ���� ���� Ÿ�̸�

	mnWait_Time[0] = GetCurrentTime();
	mnError_cnt[0] = 0;
	//	SetTimer(TMR_PCBOXQUERY, 100, NULL);
	
	// �׽�Ʈ��.
	//st_handler.mn_level_teach =  TRUE;	//20120515

	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);

	st_work.m_iFailCount_OnlineEnd = 0;
	st_work.m_iFailCount_TrackIn = 0;

	st_work.ml_last_shifter_move_time = 0;

//	if (st_picker.n_vision_eable[0] == TRUE)
//	{
		Func.Print_Complete_Set();
//	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if ( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_SYSMENU;
	cs.style &= ~FWS_ADDTOTITLE;								// ȭ�� Ÿ��Ʋ ���� ���� �����ϵ��� ��Ÿ�� ���� 
	cs.lpszClass=((CHandlerApp*)AfxGetApp())->GetClassName();	// ���� ���� ���α׷��� ���� Ŭ���� �̸��� ��´�(Need for Single Instance)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	//Ÿ��Ʋ�ٸ� Ŭ�� ������ ���Ͻ�Ų��.
	if (nHitTest == HTCAPTION) return;
	
	CFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	//Ÿ��Ʋ�ٸ� Ŭ�� ������ ���Ͻ�Ų��.
	if (nHitTest == HTCAPTION) return;
	
	CFrameWnd::OnNcLButtonDown(nHitTest, point);
}

void CMainFrame::OnNcRButtonDblClk(UINT nHitTest, CPoint point) 
{
	//Ÿ��Ʋ�ٸ� Ŭ�� ������ ���Ͻ�Ų��.
	if (nHitTest == HTCAPTION) return;
	
	CFrameWnd::OnNcRButtonDblClk(nHitTest, point);
}

void CMainFrame::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	//Ÿ��Ʋ�ٸ� Ŭ�� ������ ���Ͻ�Ų��.
	if (nHitTest == HTCAPTION) return;
	
	CFrameWnd::OnNcRButtonDown(nHitTest, point);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->hwnd == m_wndToolBar.m_hWnd)
	{
		if (m_wndToolBar.IsFloating())  // ���ٰ� �÷��� �� ��쿡 �ٽ� ȭ�� ��ܿ� ���� 
		{
			m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
			EnableDocking(CBRS_ALIGN_TOP);
			DockControlBar(&m_wndToolBar);
		}
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnMain() 
{
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	// int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	// if (nmenu_chk != TRUE)  return;
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Main)))   return;
	OnSwitchToForm(IDW_SCREEN_MAIN);
}

void CMainFrame::OnBasic() 
{
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Basic)))   return;
	OnSwitchToForm(IDW_SCREEN_BASIC);
}

void CMainFrame::OnSetting() 
{
	
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
//////////////////////////////////////////////////////////////////////////////////////// 20120515
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Work_Info)))   return;
	OnSwitchToForm(IDW_SCREEN_WORK_INFO);
// 	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Maintenance)))   return;
// 	OnSwitchToForm(IDW_SCREEN_SET_MAINTENANCE);	
////////////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////////////	20120517
// void CMainFrame::OnWait() 
// {
// 	// **************************************************************************
//     // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
//     // **************************************************************************
// 	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
// 	if (nmenu_chk != TRUE)  return;
// 	
// 	// **************************************************************************
// 	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
// 	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
// 	// **************************************************************************
// 	if (st_handler.mn_level_teach !=  TRUE)
// 	{
// 		if (st_handler.mn_level_maint !=  TRUE) 
// 		{
// 			OnLevel();
// 			if (st_handler.mn_level_teach !=  TRUE)
// 			{
// 				if (st_handler.mn_level_maint !=  TRUE)  return;
// 			}
// 		}
// 	}
// 	// **************************************************************************
// 	
// 	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Wait_Time)))   return;
// 	OnSwitchToForm(IDW_SCREEN_WAIT_TIME);
// }
////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnMotor() 
{
//	int nRet;

	CDialog_Menu menu_dlg;
	
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		} 
	}
	// **************************************************************************

//	nRet = FAS_IO.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
//	nRet2 = FAS_IO.get_in_bit(st_io.i_rear_select_switch_chk, IO_OFF);
	
//	if (nRet == IO_ON && nRet2 == IO_ON)
//	{			
//		return;
//	}
	if (st_handler.mn_manual != YES)		return;
	
	st_msg.mstr_parent_menu = "Motor Screen";  // ���� ���õ� �� �޴� ���� ����
	int nResponse = menu_dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
}

void CMainFrame::OnIo() 
{
	int nRet, nRet2;
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	nRet = FAS_IO.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
	nRet2 = FAS_IO.get_in_bit(st_io.i_rear_select_switch_chk, IO_OFF);
	
	if (nRet == IO_ON && nRet2 == IO_ON)
	{			
		return;
	}

	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_IO)))   return;
	OnSwitchToForm(IDW_SCREEN_IO);
}

void CMainFrame::OnList() 
{
	CDialog_Menu menu_dlg;
	
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// ������ ���� �ְ� ����. -�̻�� �븮- 2K4/11/05/ViboX
	/* ************************************************************************** */
	/* �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                               */
	/* -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                       */
	/* ************************************************************************** */
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	/* ************************************************************************** */
	
	st_msg.mstr_parent_menu = "List Screen";  // ���� ���õ� �� �޴� ���� ����
	int nResponse = menu_dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
}

void CMainFrame::OnLevel() 
{
	int n_response;  // ��ȭ ���ڿ� ���� ���� �� ���� ���� ���� 
	
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	if (st_handler.mn_menu_lock != FALSE)  return;
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	
	st_handler.mstr_pass_level = "LevelChange";
	
	CDialog_Pass_Check pass_dlg;
	
	n_response = pass_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CMainFrame::OnSave() 
{
	int n_response;  // ��ȭ ���� ���� �÷���
	
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	if (st_handler.mn_menu_lock != FALSE)  return;
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Main : All Setting Data Save?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		st_handler.mn_menu_lock = TRUE;
		
		OnExitData_Saving();					// ȭ�鿡 ������ ��� ������ ���� �Լ�
		
		st_handler.mn_menu_lock = FALSE;
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CMainFrame::OnAlarm() 
{
	/* ************************************************************************** */
    /* ��޸��� �˶� ȭ�� ����Ѵ�                                                */
	/* -> ���� �˶� ȭ�� ��µ� ��쿡�� �ش� ȭ�鿡 ��Ŀ���� ����                */
    /* ************************************************************************** */
	if (mp_alarm_dlg != NULL)  mp_alarm_dlg->SetFocus();
	else
	{
		mp_alarm_dlg = new CDialog_Alarm;
		mp_alarm_dlg->Create(this);
		mp_alarm_dlg->ShowWindow(SW_SHOW);
	}
	/* ************************************************************************** */
}

void CMainFrame::OnAbout() 
{
	CDialog_About about_dlg;
	
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.
    // **************************************************************************
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	// **************************************************************************
	
	st_handler.mn_menu_lock = FALSE;
	int nResponse = about_dlg.DoModal();
	
	if (nResponse == IDOK)
	{
		ShellExecute( NULL, NULL, TEXT("UpdateList.txt"), NULL, TEXT("ReadMe"), SW_SHOW );
	}
}

void CMainFrame::OnReset() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnLock() 
{
	int n_response;  // ��ȭ ���� ���� �÷���
	
	CDialog_Pass_Check  pass_chk_dlg;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	st_handler.mstr_pass_level=_T("SystemLock");  // ��ȣ ���� ����
	
	n_response = pass_chk_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, FORM_NAME, 2);  // ���� �����ӿ� SYSTEM LOCK ȭ�� ��� ��û
		
		if (st_handler.cwnd_list != NULL)
		{
//			st_msg.mstr_normal_msg = "System Locking.";
			sprintf(st_msg.c_normal_msg, "System Locking.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CMainFrame::OnExit() 
{
	int mn_response;  // ��ȭ ���ڿ� ���� ���� �� ���� ���� ���� 
	
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.mn_menu_lock != FALSE)	return;
	if (st_handler.mn_system_lock != FALSE)  return;  // ���� �ý��� Lock ������ ��� ���� 
	
	CDialog_Message msg_dlg;

	// **************************************************************************
	// ���� ��� ���°� STOP �������� �˻�                                       
	// **************************************************************************
	if (st_work.mn_run_status != dSTOP)
	{
		st_msg.str_fallacy_msg = _T("Module Heat Sink is active.");
		mn_response = msg_dlg.DoModal();
		if (mn_response == IDOK)  return ;
	} 
	// **************************************************************************

	st_handler.mn_level_teach =  FALSE;
	st_handler.mn_level_maint =  FALSE;
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
		
	// **************************************************************************
    // ���� ���α׷� ���� ȭ�� ���                                              
    // **************************************************************************
	CDialog_Exit  dlg;
	
	mn_response = dlg.DoModal();
	
	if (mn_response == IDOK)     
	{
		FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff,	IO_OFF);

		FAS_IO.set_out_bit(st_io.o_m_stacker2_rail_fwd_onoff,	IO_ON);
		FAS_IO.set_out_bit(st_io.o_m_stacker2_rail_bwd_onoff,	IO_OFF);
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_rail_fwd_onoff,	IO_ON);
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_rail_bwd_onoff,	IO_OFF);
		FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker2_rail_fwd_onoff,	IO_ON);
		FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker2_rail_bwd_onoff,	IO_OFF);
		FAS_IO.set_out_bit(st_io.o_uld_stacker2_rail_fwd_onoff,	IO_ON);
		FAS_IO.set_out_bit(st_io.o_uld_stacker2_rail_bwd_onoff,	IO_OFF);

		FAS_IO.set_out_bit(st_io.o_m_stacker2_tray_clamp_onoff,	IO_OFF);
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_tray_clamp_onoff,	IO_OFF);
		FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker2_tray_clamp_onoff,	IO_OFF);
		FAS_IO.set_out_bit(st_io.o_uld_stacker2_tray_clamp_onoff,	IO_OFF);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_0_onoff,	IO_ON);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_90_onoff,	IO_OFF);
		FAS_IO.set_out_bit(st_io.o_lock_door_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_label_attach_vacuum_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_ionizer_onoff, IO_OFF);

		::Sleep(3500);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_tube_rotator_unclamp_onoff, IO_ON);

		sprintf(st_msg.c_normal_msg, "[Exit OK]");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);

		st_timedb.mnTime_Section	= dSTOP;		
		st_timedb.mole_date			= COleDateTime::GetCurrentTime();
		
		//Func.DB_Write_Time(st_timedb);
		OnMain_Work_Mysql_Write();

		All_Stop = 1 ;				// ������ ���� ���� ���� ���� �ʱ�ȭ
		OnMain_Thread_Destroy();	// ������ ���� �Լ�

//		st_basic.n_mode_use_door = 0; //20130109 �ӽ�

		OnMain_Motor_Destroy();						//��� ���� ���� ����.
		mcls_m_basic.OnDaily_Count_Save();			// Daily Data Save 2K4/08/20/ViboX
		
		OnExitData_Saving();						// ȭ�鿡 ������ ��� ������ ���� �Լ�
//		mcls_m_basic.OnDaily_Count_Save();			// Daily Data Save 2K4/08/20/ViboX
		
		CView* pOldActiveView = GetActiveView();	// ���� ȭ�鿡 ��µ� �� ���� ���� (save old view)
		
		pOldActiveView->ShowWindow(SW_HIDE);		// ������ ��µ� �並 ����� (hide the old)
		pOldActiveView->DestroyWindow();			// ������ ��µ� �並 �ı���Ų�� (Old View Destroy ��Ű�� ���Ͽ� �߰�)
		pOldActiveView = NULL;						// ������ ��µ� �信 ���� ���� ���� ���� �ʱ�ȭ 
		
		delete	pOldActiveView;						// ������ ��µ� �� ������ �����Ѵ� (kill old view)

		CFrameWnd::OnClose();
	}
}

int CMainFrame::OnMenu_Change_Checking()
{
	int nRet;
	
	nRet = TRUE;  // �ʱ� ���� �� ���� 
	
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.mn_menu_lock != FALSE)	nRet = FALSE;
	if (st_handler.mn_system_lock != FALSE)  nRet = FALSE;  // ���� �ý��� Lock ������ ��� ���� 
	
	// **************************************************************************
	// ���� ��� ���°� STOP �������� �˻�                                       
	// **************************************************************************
	if (st_work.mn_run_status != dSTOP && st_work.mn_run_status != dLOCK)
	{
//		st_msg.mstr_normal_msg = _T("Handler is active. Stop first.");
		sprintf(st_msg.c_normal_msg, "Handler is active. Stop first.");
		if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		
		nRet = FALSE;
	}
	// **************************************************************************
	
	return nRet;
}

void CMainFrame::OnSwitchToForm(int nForm)
{
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	if (st_handler.mn_menu_lock != FALSE)	return;
	if (st_handler.mn_system_lock != FALSE)  return;		// ���� �ý��� Lock ������ ��� ���� 

	CView* pOldActiveView = GetActiveView();			// ���� ȭ�鿡 ��µ� �� ���� ���� (save old view)
	CView* pNewActiveView = (CView*)GetDlgItem(nForm);  // ���� ����� �� ���� ���� (get new view)

	if (pNewActiveView == NULL)							// ���� ����� �䰡 ���� ��µ��� �ʾҴ��� �˻� (if it hasn't been created yet)
	{
		switch (nForm)			// �信 ������ ID ���� �� (these IDs are the dialog IDs of the view but can use anything)
		{
		case IDW_SCREEN_MAIN:				// ���� ȭ�� 
			pNewActiveView = (CView*)new CScreen_Main;
			break;
		case IDW_SCREEN_LOCK:				// System Lock ȭ��
			pNewActiveView = (CView*)new CScreen_Lock;
			break;
		case IDW_SCREEN_INIT:				// �ʱ�ȭ ȭ�� 
			pNewActiveView = (CView*)new CScreen_Initial;
			break;
		case IDW_SCREEN_BASIC:				// Basic ȭ��
			pNewActiveView = (CView*)new CScreen_Basic;
			break;
		case IDW_SCREEN_PGM_INFO:
			pNewActiveView = (CView*)new CScreen_Pgm_Info;
			break;
/////////////////////////////////////////////////////////////////////// 20120515		
		case IDW_SCREEN_WORK_INFO:
			pNewActiveView = (CView*)new CScreen_Work_Info;
			break;
// 		case IDW_SCREEN_SET_MAINTENANCE:	// ����Ʈ ȭ�� 
// 			pNewActiveView = (CView*)new CScreen_Set_Maintenance;
// 			break;
///////////////////////////////////////////////////////////////////////
//  		case IDW_SCREEN_WAIT_TIME:			// ��� �ð� ȭ�� 
//  			pNewActiveView = (CView*)new CScreen_Wait_Time;
//  			break;
		case IDW_SCREEN_MOTOR_TEACH:		// Motor Teach ȭ��
			pNewActiveView = (CView*)new CScreen_Motor;
			break;
		case IDW_SCREEN_MOTOR_SPEED:		// Motor Speed ȭ�� 
			pNewActiveView = (CView*)new CScreen_Motor_Speed;
			break;
		case IDW_SCREEN_LIST_OPERATION:		// ���۷��̼� ���� ��� ȭ�� 
			pNewActiveView = (CView*)new CScreen_List_Operation;
			break;
		case IDW_SCREEN_LIST_DATA_VIEW:			// ������ ���� ���� ��� ȭ�� 
			pNewActiveView = (CView*)new CScreen_List_Data_View;
			break;
/////////////////////////////////////////// 20120701
		case IDW_SCREEN_LIST_STEP:			// ������ ���� ���� ��� ȭ�� 
			pNewActiveView = (CView*)new CScreen_List_Step;
			break;
///////////////////////////////////////////

		case IDW_SCREEN_IO:					// I/O ��� ȭ�� 
			pNewActiveView = (CView*)new CScreen_IO;
			break;

		case IDW_SCREEN_ADMINISTRATOR:		// ADMINISTRATOR ��� ȭ�� 
			pNewActiveView = (CView*)new CScreen_Admin;
			break;
		case IDW_SCREEN_FILE_EDITOR:		// File Editor ��� ȭ�� 
			pNewActiveView = (CView*)new CScreen_File_Editor;
			break;
		}

		CCreateContext context;  // ���ο� �信 ��ť��Ʈ Attach ( attach the document to the new view)
		context.m_pCurrentDoc = pOldActiveView->GetDocument();

		pNewActiveView->Create(NULL, NULL, 0L, CFrameWnd::rectDefault, this, nForm, &context);
		pNewActiveView->OnInitialUpdate();       // ���� ������ ����� ������ �� �κ��� ���ش�.
	}

//  ���� ������ ���.
//	pOldActiveView->ShowWindow(SW_HIDE);	// ������ ��µ� �並 ����� (hide the old)
//	pOldActiveView->DestroyWindow();		// ������ ��µ� �並 �ı���Ų�� (Old View Destroy ��Ű�� ���Ͽ� �߰�)
//	pOldActiveView = NULL;					// ������ ��µ� �信 ���� ���� ���� ���� �ʱ�ȭ 
//	
//	pNewActiveView->OnInitialUpdate();      // �ı��Ǵ� ������ �ٲ� ������ ���� ��µǴ� �信�� ���� ���ؼ� �̸��� �� ����.
//	SetActiveView(pNewActiveView);			// ���� ��µ� �並 ��Ƽ�� ��� ���� (change the active view)
//	pNewActiveView->ShowWindow(SW_SHOW);	// ���ο� �並 ��� (show the new window)
//
//	::SetWindowLong(pNewActiveView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);  // gotta have it
//	RecalcLayout();							// �������� �����Ѵ� (adjust frame)
//	delete	pOldActiveView;					// ������ ��µ� �� ������ �����Ѵ� (kill old view)
//
//  ���� ���.
	SetActiveView(pNewActiveView);			// ���� ��µ� �並 ��Ƽ�� ��� ���� (change the active view)
	pNewActiveView->ShowWindow(SW_SHOW);	// ���ο� �並 ��� (show the new window)

	pOldActiveView->ShowWindow(SW_HIDE);	// ������ ��µ� �並 ����� (hide the old)
	pOldActiveView->DestroyWindow();		// ������ ��µ� �並 �ı���Ų�� (Old View Destroy ��Ű�� ���Ͽ� �߰�)
	pOldActiveView = NULL;					// ������ ��µ� �信 ���� ���� ���� ���� �ʱ�ȭ 

	::SetWindowLong(pNewActiveView->m_hWnd, GWL_ID, AFX_IDW_PANE_FIRST);  // gotta have it
	RecalcLayout();							// �������� �����Ѵ� (adjust frame)
	delete	pOldActiveView;					// ������ ��µ� �� ������ �����Ѵ� (kill old view)
	
	st_handler.mn_menu_num = nForm; 

	// **************************************************************************
	// ���� ȭ�� ���� Ÿ��Ʋ�ٿ� ����Ѵ�.                                       
	// **************************************************************************
	if (st_handler.cwnd_title != NULL)   st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, FORM_NAME, nForm);
	// **************************************************************************
}

// ******************************************************************************
// ����� ���� �޽����� ���� �� ȭ�� ��ȯ �Լ�                                   
// ******************************************************************************
LRESULT CMainFrame::OnViewChangeMode(WPARAM wParam,LPARAM lParam)
{

	if( GetActiveView() == NULL )
	{
		int a = 1;
		return 0;
	}
	int mn_chg_num = wParam;  // �� ȭ�鿡 ���� ID �� ���� 
	
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
	// -> �˶� ȭ���� ������ ȭ�鿡 ���ؼ��� �޴� ��� ���� ���� �˻�            
    // **************************************************************************
	if (mn_chg_num != 9)  
	{
		if (st_handler.mn_menu_lock != FALSE)  return 0;
		if (st_handler.mn_system_lock != FALSE)  return 0;  // ���� �ý��� Lock ������ ��� ���� 
	}
	// **************************************************************************
	
	switch(mn_chg_num)
	{
	case 1: 
		if (lParam==1)  OnMain();					// ���� ȭ�� ��ȯ 
		else if (lParam==2)							// System Locking ȭ�� ��ȯ 
		{
			if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Lock)))   return 0;
			OnSwitchToForm(IDW_SCREEN_LOCK) ;
		}
		else if (lParam==3)							// �ʱ�ȭ ȭ�� ��ȯ 
		{
			if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Initial)))   return 0;
			OnSwitchToForm(IDW_SCREEN_INIT) ;
		}
		else if (lParam==5)//2015.0128
		{
			SendMessage(WM_CLOSE, 0, 0);
		}
		break;
	case 2: 
		OnBasic();									// ������ ȭ�� ��ȯ 
		break;
 	case 3:
		//////////////////////////////////////////////////////////////////// 20120515
 		//if (lParam==1)       OnSetMaintenance();	// ����Ʈ ȭ�� ��ȯ 
		if (lParam==1)       OnSetting();	// ����Ʈ ȭ�� ��ȯ 
		////////////////////////////////////////////////////////////////////
 		else if (lParam==2)  OnSetInterface();		// Interface ���� ���� ȭ�� ��ȯ
 		break;
 	case 4 :
// 		OnWait();	//20120517
 		break;
 	case 5 : 
 		if (lParam==1)       OnMotTeach();			// ���� ��ġ ȭ�� ��ȯ 
 		else if (lParam==2)	 OnMotSpeed();			// ���� �ӵ� ȭ�� ��ȯ 
 		break;
	case 6 :
		if (lParam==1)       OnListOper();			// Operation ��� ����Ʈ ȭ�� ��ȯ 
		else if (lParam == 2)OnListDataview();
		else if (lParam == 3)OnListStep();	//20120701
		break;

	case 7 : 
		if (lParam==1)       OnAlarm();				// �˶� ȭ�� ��ȯ 
		else if (lParam==2)  OnAlarm_Destroy();		// �˶� ȭ�� ���� �Լ�
		break;
	case 8 : 
		if (lParam==1)       OnAdmin();				// Administrator ȭ�� ��ȯ 
		else if (lParam==2)  OnFileEditor();		// File Editor ȭ�� ��ȯ.
		break;
	default:
		break;
	}
	
	return 0 ;
}
// ******************************************************************************

void CMainFrame::OnMain_Path_Set()
{
	CString strTempPath;

// 	CString mstr_basic;			// �⺻ ���� ���� ���� ����+���ϸ� ���� ����
// 	
// 	CString mstr_path_dvc;		// ����̽��� ƼĪ ���� ���� ���� ���� ���� ���� ����
// 	CString mstr_socket_lot;	// ���Ϻ�...
// 	CString mstr_socket_day;	// ���Ϻ�...
// 	CString mstr_socket_month;	// ���Ϻ�...
// 	CString mstr_socket_shift;	// ���Ϻ�...
// 	CString mstr_path_alarm;	// ��� �˶� ���� ���� ���� ���� ����
// 	CString mstr_path_op;		// ���۷����� ���� ���� ���� ���� ����
// 	CString mstr_path_tray;		// Tray�� ���� ���� ���� ���� ���� ���� ����
// 	CString mstr_path_serial;	// Socket ���� ���� ���� ���� ���� ���� ����
// 	
// 	CString mstr_path_file;		// ���� ���� ���� ���� ����
// 	CString mstr_path_back;		// BACKUP ���� ���� ���� ���� ����
// 	
// 	CString mstr_file_basic;	// �⺻ ���� ���� ���� ���ϸ� ���� ����
// 	CString mstr_file_wait;		// ��� �ð� ���� ���� ���ϸ� ���� ����
// 	
// 	CString mstr_operation;		// Operation Log ���� ���� ����.
// 	CString mstr_interface;		// Interface Log ���� ���� ����.
// 	CString mstr_machine;		// Machine Log ���� ���� ����.
// 	CString mstr_barcode;		// Barcode Log ���� ���� ����.
// 	CString mstr_gpib;			// Total Log ���� ���� ����.
// 	CString mstr_total;			// Total Log ���� ���� ����.
// 	
// 	CString mstr_io_map;		// IO MAP ���� ��ġ ����.
// 	
// 	CString mstr_handler;

	//2015.0129
	CString strLogPath = _T("c:\\AMT830\\Log\\");


	st_path.mstr_interface_folder = _T("c:\\AMT830\\File\\File.TXT"); //20120516
	Func.CreateFolder(st_path.mstr_interface_folder);

	st_path.mstr_basic =			_T("C:\\AMT830\\Setting\\BasicTerm.TXT");		// ȯ�� ���� ���� ���� ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_basic);

//////////////////////////////////////////////// 20120530
	st_path.mstr_label_folder =			_T("C:\\AMT830\\Label\\");		// ȯ�� ���� ���� ���� ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_label_folder);
////////////////////////////////////////////////	

	st_path.mstr_basic_folder =		_T("C:\\AMT830\\Setting\\");		// ȯ�� ���� ���� ���� ���Ͽ� ���� ���� ���� 

	st_path.mstr_path_dvc =		_T("C:\\AMT830\\Device\\");					// ����̽��� ƼĪ ���� ���� ���� ���� ���� ����
	Func.CreateFolder(st_path.mstr_path_dvc);

	st_path.mstr_motor =			_T("C:\\AMT830\\Setting\\AMT830.cme2");

	st_path.mstr_file_basic =		_T("BasicTerm");								// �⺻ ���� ���� ���� ���ϸ� ����
	st_path.mstr_file_wait =		_T("WaitTime");									// ��� �ð� ���� ���� ���ϸ� ����

	CString FolderPath;
	
	st_path.mstr_path_alarm =	_T("C:\\AMT830\\Bmp\\");						// ��� �˶� ���� ���� ���� ����
	Func.CreateFolder(st_path.mstr_path_alarm);
	
	st_path.mstr_path_op =		_T("C:\\AMT830\\Operator\\");				// ���۷����� ���� ���� ���� ����
	Func.CreateFolder(st_path.mstr_path_op);
	
	st_path.mstr_path_file =		_T("C:\\AMT830\\");							// ���� ���� ���� ����
	st_path.mstr_path_back =		_T("c:\\BackUp\\");									// BACKUP ���� ���� ���� ����
	
	st_path.mstr_path_back_1 = _T("c:\\AMT830\\BackUp\\");									// BACKUP ���� ���� ���� ����
	Func.CreateFolder(st_path.mstr_path_back_1);

	st_path.mstr_operation =		_T("C:\\AMT830\\Log\\Operation\\");			// Operation Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_operation);
	
	st_path.mstr_interface =		_T("C:\\AMT830\\Log\\Interface\\");			// Interface Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_interface);
	
	st_path.mstr_machine   =		_T("C:\\AMT830\\Log\\Machine\\");			// Machine Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_machine);
	
	st_path.mstr_barcode   =		_T("C:\\AMT830\\Log\\Barcode\\");				// Machine Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_barcode);
	
	////2014,0303
	st_path.mstr_vision    =        _T("C:\\AMT830\\Log\\Vision\\");            // Vision Read�� �ø����ȣ ���� ���� ����
    Func.CreateFolder(st_path.mstr_vision);
    ////

	////2014,0408
	st_path.mstr_Excel   =        _T("C:\\AMT830\\Log\\Excel\\");
	Func.CreateFolder(st_path.mstr_Excel);
	////

	st_path.mstr_total     =		_T("C:\\AMT830\\Log\\Total\\");				// Total Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_total);

	st_path.str_log_path	=	_T("C:\\AMT830\\Log\\EqLog\\");	
	Func.CreateFolder(st_path.str_log_path);

	st_path.str_lot_data_path	=	_T("C:\\AMT830\\Data\\");	
	Func.CreateFolder(st_path.str_lot_data_path);

	Func.CreateFolder("C:\\AMT830\\Log\\AlarmCodeDebug\\");
	Func.CreateFolder("C:\\AMT830\\Alarm\\Daily\\");

	st_path.mstr_io_map			 = _T("C:\\AMT830\\Setting\\AMT830_IO_MAP.xls");
	st_path.mstr_motor_axis_map	 = _T("C:\\AMT830\\Setting\\AMT830_MOTOR_AXIS_MAP.xls");
	st_path.mstr_motor_part_map	 = _T("C:\\AMT830\\Setting\\AMT830_MOTOR_PART_MAP.xls");
	st_path.mstr_io_part_map     = _T("C:\\AMT830\\Setting\\AMT830_IO_PART_MAP.xls");
	st_path.mstr_wait_time_map   = _T("C:\\AMT830\\Setting\\AMT830_WAITTIME_MAP.xls");

//////////////////////////////////////////////// 20121220  �豤��
	st_path.mstr_part =          _T("C:\\AMT830\\Part\\PART_NUMBER.TXT");
	st_path.mstr_part_folder =				     _T("C:\\AMT830\\Part\\");
	Func.CreateFolder(st_path.mstr_part_folder);

//////////////////////////////////////////////// 

	//2015.0129
	st_path.mstr_uph     =		_T(strLogPath + "UPH\\UPH\\");				// Total Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_uph);

	st_path.mstr_uph_total     =		_T(strLogPath + "UPH\\Total\\");				// Total Log ���Ͽ� ���� ���� ���� 
	Func.CreateFolder(st_path.mstr_uph_total);


}

void CMainFrame::OnMain_Var_Default_Set()
{
	int i = 0, j = 0, k = 0,mn_chk = 0;
	CString mstr_temp;

	OnMain_Path_Set();

	mcls_m_basic.OnInterface_Data_Load();	//20120516


	st_basic.mb_controlpanel_enable = false;

	st_handler.mn_lot_shift = CTL_NO;
	st_handler.mn_lot_cancel_mode = CTL_NO;		// Lot Cancel Mode�� �ƴϴ�.
	st_handler.mn_lot_label_info = CTL_NO;
	st_handler.mn_lot_end = CTL_NO;

	st_handler.mn_retest_check = CTL_NO;		// FRONT���� ���׽�Ʈ�� �ϰ� �ϱ� ���� ����. ������ NO��. 2K11/08/15/ViboX

	for (i = 0; i < MAXSITE; i++)
	{
		st_handler.mn_init_state[i] = CTL_NO;
	}

	for (i = 0; i < TESTSIZE; i++)						// �����Ϳ� ���簡 ���ٰ� �ʱ� ����.
	{
		st_modulemap.FrontShifter[0][i] = CTL_NO;
		st_modulemap.RearShifter[0][i] = CTL_NO;
	}

	st_handler.mn_debug_mode = CTL_NO;
	st_handler.mn_back_mode = NO;

	mp_alarm_dlg = NULL;
	mp_msg_dlg = NULL;

	st_handler.mn_selfcheck = CTL_NO;
	nElectrostaticCheckStep = 0;
	mn_heater_write_step = 0;
	mn_calpella_display_step = 0;
	mn_new_lot_in_step  = 0;
	mn_bd_live_chk_step = 0;
	mn_lot_name_step = 0;
	mnQueryStep = 0;

	st_handler.mn_lot_name_chk = 0;
	for (i = 0; i < 3; i++)
	{
		st_handler.mstr_lot_name[i] = "";
	}
	st_handler.mstr_front_lot_name = "";

	st_handler.mn_blot_count = 0;

	nElectrostaticCheckStep = 0;					// ������ Ȯ�� ���� Ŭ����.
	nElectChannel = 1;						// ������ Ȯ�� ä�� 1~5

	st_sync.mn_barcode_to_shifter[0] = CTL_NO;		// �ϴ� �����ϸ� �ȵȴ�.
	st_sync.mn_barcode_to_shifter[1] = CTL_NO;		// �ϴ� �����ϸ� �ȵȴ�.

	st_sync.mn_shifter_to_sbuffer[0] = CTL_NO;
	st_sync.mn_shifter_to_sbuffer[1] = CTL_NO;

	COMI.mn_motorbd_init_end = CTLBD_NO; 
	st_handler.mn_mot_brd_initial = CTLBD_NO;

	st_handler.mn_board_onoff_dialog = CTL_NO;

	st_sync.mn_buffer_work = CTL_NO;

	st_handler.mstr_last_module = "NO";
	for (i = 0; i < 3; i++)
	{
		st_handler.mn_last_module_chk[i] = CTL_NO;
		st_handler.mn_last_module_chk_chk[i][POS_FRONT] = CTL_NO;		// 2K11/07/06/ViboX
		st_handler.mn_last_module_chk_chk[i][POS_REAR] = CTL_NO;		// 2K11/07/06/ViboX
	}
	st_handler.mn_last_module_retest_chk = CTL_NO;

	st_handler.mstr_last_module_lotname = "";

	CString str_load_device, str_chk_ext, str_load_label;	//20120530
	int mn_pos;
	char chr_data[50], chr_data1[50]; //20120530
	char chr_buf[50];
	
	:: GetPrivateProfileString("BasicData", "Device_Type", "DEFAULT", chr_data, 50, st_path.mstr_basic);
	str_load_device = chr_data;
	str_load_device.TrimLeft(' ');               
	str_load_device.TrimRight(' ');
	
	mn_pos = str_load_device.Find(".");								// Ȯ���� ��ġ �˻�
	if (mn_pos == -1)
		st_basic.mstr_device_name = str_load_device + _T(".TXT");	// Ȯ���� �߰�
	else 
	{
		str_chk_ext = str_load_device.Mid(mn_pos);					// ���� Ȯ���� ����
		if (str_chk_ext != _T(".TXT"))  
			st_basic.mstr_device_name = _T("DEFAULT.TXT");			// �ʱ� ����̽� ���ϸ� ����
		else 
			st_basic.mstr_device_name = str_load_device;				// ����̽� ���ϸ� ����
	}


////////////////////////////////////////////////////////// 20120530
	:: GetPrivateProfileString("BasicData", "Label_Type", "", chr_data1, 50, st_path.mstr_basic);
	str_load_label = chr_data1;
	str_load_label.TrimLeft(' ');               
	str_load_label.TrimRight(' ');
	
	mn_pos = str_load_label.Find(".");								// Ȯ���� ��ġ �˻�
	if (mn_pos == -1)
		st_basic.mstr_label_name = str_load_label + _T(".TXT");	// Ȯ���� �߰�
	else 
	{
		str_chk_ext = str_load_label.Mid(mn_pos);					// ���� Ȯ���� ����
		if (str_chk_ext != _T(".TXT"))  
			st_basic.mstr_label_name = _T("G2.TXT");			// �ʱ� ����̽� ���ϸ� ����
		else 
			st_basic.mstr_label_name = str_load_label;				// ����̽� ���ϸ� ����
 	}
//////////////////////////////////////////////////////////


	:: GetPrivateProfileString("BasicData", "Reverse_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_handler.mn_reverse_mode = 0;
		mstr_temp = LPCTSTR(_itoa(st_handler.mn_reverse_mode, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Reverse_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_handler.mn_reverse_mode = mn_chk;

	if( st_handler.mn_reverse_mode == 0 )
	{
		st_sync.mn_pos_rear = 0;
		st_sync.mn_pos_front = 1;
	}
	else
	{
		st_sync.mn_pos_rear = 1;
		st_sync.mn_pos_front = 0;
	}

	:: GetPrivateProfileString("BasicData", "Virtual_Mode", "0", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_handler.mn_virtual_mode = 0;
		mstr_temp = LPCTSTR(_itoa(st_handler.mn_virtual_mode, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Virtual_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_handler.mn_virtual_mode = mn_chk;


	:: GetPrivateProfileString("BasicData", "Eject_Count", "1", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 10)
	{
		st_handler.mn_eject_count = 1;
		mstr_temp = LPCTSTR(_itoa(st_handler.mn_eject_count, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Eject_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_handler.mn_eject_count = mn_chk;
	
	:: GetPrivateProfileString("BasicData", "Pick_Count", "1", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 10)
	{
		st_handler.mn_pick_count = 1;
		mstr_temp = LPCTSTR(_itoa(st_handler.mn_pick_count, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Pick_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_handler.mn_pick_count = mn_chk;

	:: GetPrivateProfileString("BasicData", "Uld_Convayor_Mode", "-1", chr_data, 10, st_path.mstr_basic);
	mn_chk = atoi(chr_data);
	if (mn_chk < 0 || mn_chk > 1)
	{
		st_handler.n_uld_convayor = 0;
		mstr_temp = LPCTSTR(_itoa(st_handler.n_uld_convayor, chr_buf, 10)) ;
		:: WritePrivateProfileString("BasicData", "Uld_Convayor_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else  st_handler.n_uld_convayor = mn_chk;




	for (i = 0; i < 3; i++)
	{
		st_work.mn_client_accept[i] = CTL_NO;
		st_work.mn_client_ready[i]	= CTL_YES;
		st_work.mn_client_rpy[i]	= CTL_NO;
	}

	st_sync.mn_front_machine_msg = CTL_YES;
	mcls_m_basic.On_Teach_Data_Load();

	for (i = 0; i < 3; i++)
	{
		st_handler.mstr_lot_name[i] = "";
		st_handler.mn_lot_start_time_check[i] = CTL_NO;
		st_handler.mstr_lot_start_time[i] = "";
	}

	Func.ReadRegistry_LotNo();

	st_NW.mb_buffer_in_received[POS_REAR] = true;
	st_NW.mb_buffer_in_received[POS_FRONT] = true;

	st_sync.n_airblowdoor_to_transfer = CTL_NO;

	for (i = 0 ; i < PICKER_NUM; i++)
	{
		st_modulemap.nPickerEnable[i] = CTL_YES;
	}
	
	for (i = 0 ; i < SOCKET_NUM; i++)
	{
		st_modulemap.nRetestSocketUse[i] = CTL_YES;
	}

	st_sync.n_length_change = CTL_NO;

	st_handler.n_nto_fail = CTL_NO;
	st_handler.n_hto_fail = CTL_NO;
	st_handler.n_bto_fail = CTL_NO;

	st_basic.n_retry_cnt = 3;


	st_work.i_robot_device_loading = FALSE;

	for (i = 0 ; i < D_INFOSIZE; i++)
	{
		for (j = 0; j < 4; j++)
		{
			for (k = 0; k < 4; k++)
			{
				st_modulemap.WorkBuffer[i][j][k] = CTL_CLEAR;
			}
		}
	}
	st_work.n_jamcode_flag = NO;

	for (i = 0; i < 4; i++)
	{
		st_work.n_loadlot_count[i] = 0;
	}

	st_work.i_sort_good_cnt = 0;
	st_work.i_sort_reject_cnt = 0;

	st_handler.mb_bcr_read = FALSE;
	st_handler.mn_jobComplete = NO;
	st_handler.mn_receive = NO;
	st_handler.nBootFlagCheck = CTL_NO;
	st_handler.n_lotend_ready = CTL_NO;

	st_handler.n_more_lefths = CTL_NO;
	st_handler.n_more_righths = CTL_NO;
	st_handler.n_more_uld_tray = CTL_NO;

	st_work.n_lotend = CTL_NO;
	st_work.n_eqp_lotend = CTL_NO;
	st_handler.n_vision_reset = NO;

	st_handler.n_hs1_fix = CTL_NO;
	st_handler.n_hs2_fix = CTL_NO;
	st_handler.n_hs3_fix = CTL_NO;
	st_handler.n_hs4_fix = CTL_NO;

	st_handler.n_plate_fwdbwd_onoff = NO;
	st_work.n_multilot_start = CTL_NO;

	st_handler.n_loadClip = CTL_NO;
	st_handler.n_unloadClip = CTL_NO;
	st_handler.n_unloadCenterChk = CTL_NO;
	st_handler.n_loadCenterChk = CTL_NO;

	////20140214
	for (i = 0; i < 500; i++)
	{
		st_work.m_strBarcode_OutPirnt[i] = "";
		st_work.m_strLotNoChk[i] = "";//2014,0218
		st_work.mstr_Vision_Read_LotNo[i];//2014,0303
	}
	////
	OnMain_BootFlagCheck();
}	

void CMainFrame::OnMotTeach() 
{
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Motor)))   return;
	OnSwitchToForm(IDW_SCREEN_MOTOR_TEACH);
}

LRESULT CMainFrame::OnMainframe_Work(WPARAM wParam, LPARAM lParam)
{	
	int mn_command_num = wParam;  // ��Ʈ��ũ �۾��� �� ���� ����
	
	switch (mn_command_num)
	{		
	case 2:		// ������ ȭ�鿡�� ������ Ȯ��
		SetTimer(TMR_MAIN_ELEC_CHK, 100, NULL);
		break;
				
	case 1000:
		OnMainFrame_MessageDisplay();
		break;
		
	case 1001:
		OnMainFrame_SelectMessageDisplay();
		break;

	case 1004:
		SetTimer(TMR_NEW_LOT_IN, 100, NULL);
		break;

	case 1005:
		SetTimer(TMR_LOT_NAME, 100, NULL);
		break;

	case 1100:
		OnMainFrame_ScrapInfoDisplay();
		break;

	case 1200:
		OnMainFrame_TrackOutDisplay();
		break;

	case 1300:
		OnMainFrame_TrackInDisplay();
		break;

	}
	
	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	CString str_display_time;
	int n_hour, n_minute, n_second;
//	int n_temp;  // �� �ð� [��] ���� ����
	int n_min_temp;	// �� �ð� [��] ���� ���� 
	int n_year, n_month, n_day;				// ��, ��, �� ���� ����
	int n_response;

	int nIndexMin = 0, nIndexSec = 0;
	int nCyclexMin = 0, nCycleSec = 0;

	CString strTimeLog;
	CString mstr_temp;

	CString mStartTime, mEndTime; 
//	char cStartTime[60], cEndTime[60];

	COleDateTime cur_time;
	CTime bin_time;

	if (nIDEvent == TMR_MAIN_REFRESH)
	{
		CTime cur = CTime::GetCurrentTime();
		CTimeSpan diff = cur - st_handler.m_tRef;
		st_handler.m_tRef = cur;

		switch(st_work.mn_run_status)
		{			
		case dRUN:
			st_handler.m_tR = st_handler.m_tR + diff;
			st_handler.m_tDR = st_handler.m_tDR + diff;
			st_handler.m_tRN = st_handler.m_tRN + diff;//2014,0408
			break;

		case CTL_dWARNING:
		case dJAM:		
			st_handler.m_tJ = st_handler.m_tJ + diff;
			st_handler.m_tDJ = st_handler.m_tDJ + diff;
			break;

		case dLOCK:
			st_handler.m_tM = st_handler.m_tM + diff;
			st_handler.m_tDM = st_handler.m_tDM + diff;
			break;
		
		case dLOTEND:												// Lot End �ÿ��� Stop���� �ð��� �ø���.
		case dSTOP:
			st_handler.m_tS = st_handler.m_tS + diff;
			st_handler.m_tDS = st_handler.m_tDS + diff;
			break;
		}

		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_TIMEINFO, st_work.mn_run_status);
		}
	}
	else if (nIDEvent == TMR_FILE_CREATE)
	{
		// **********************************************************************
		// ���� ���� �� �ʿ��� �ð� �����Ѵ�                                     
		// **********************************************************************
		cur_time = COleDateTime::GetCurrentTime();

		n_hour = cur_time.GetHour();
		n_minute = cur_time.GetMinute();
		n_second = cur_time.GetSecond();
		
		n_year = cur_time.GetYear();  
		n_month = cur_time.GetMonth();  
		n_day = cur_time.GetDay();  
		
		if (n_hour == 22 && n_minute == 0 && n_second == 0)
		{
			Func.OnDailyDataSave();
			Func.OnDayDataReset();
			Func.OnWriteProduct();
		}
	
		
		n_min_temp = n_hour * 60 + n_minute;
		
		// ������ Ȯ���� ���� �ð� Ȯ��.
		if (n_second == 0)
		{
			if (n_min_temp % st_basic.mn_electrostatic_check_gap == 0)
			{
				SetTimer(TMR_MAIN_ELEC_CHK, 200, NULL);
			}
		}
		//2015.0129
		if ( n_hour != 22 && n_second == 0 )
		{
			if ( n_min_temp % 60 == 0)
			{
				Func.OnWriteProduct();
			}
		}
	}
	else if (nIDEvent == TMR_MAIN_ELEC_CHK)
	{
/*		nRet = OnElectrostatic_Check();

		if (nRet == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_MAIN_ELEC_CHK);
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			KillTimer(TMR_MAIN_ELEC_CHK);
		}*/
	}
	else if (nIDEvent == TMR_NEW_LOT_IN)
	{
		KillTimer(TMR_NEW_LOT_IN);

// 		n_response = OnMain_New_Lot_In();
// 		
// 		if (n_response == RET_GOOD)
// 		{
// 			KillTimer(TMR_NEW_LOT_IN);
// 		}
	}
	else if (nIDEvent == TMR_LOT_NAME)
	{
		n_response = OnMain_Lot_name();
		
		if (n_response == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_LOT_NAME);
		}
	}
	else if(nIDEvent == TMR_MAIN_RCV_BARCODE)
	{
		n_response = OnMain_Rcv_Barcode();
		if(n_response == CTLBD_RET_GOOD)
		{
			KillTimer(TMR_MAIN_RCV_BARCODE);
		}
		else if(n_response == CTLBD_RET_ERROR)
		{
			KillTimer(TMR_MAIN_RCV_BARCODE);
		}
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}

int CMainFrame::OnElectrostatic_Check()
{
	int nFunRet = CTLBD_RET_PROCEED;
	int i;

	// �ø��� ��Ʈ�� �������� �ʾҴٸ� �׳� �н�...
	if (st_handler.mnSerial_Port_Creating[GMS_PORT-1] != CTL_YES)
	{
		return CTLBD_RET_GOOD;
	}
	
	switch (nElectrostaticCheckStep)
	{
	case 0:
		strElectCommand.Format("%c%cP%-8sCH%d %c", 0x10, 0x02, "SAMSUNG1", nElectChannel, 0x03);
		st_serial.str_snd[GMS_PORT-1] = strElectCommand;
		st_work.mn_elec_serial_work = CTL_READY;
		lElectSerialTime[0] = GetCurrentTime();
		OnDataSend(GMS_PORT, 0);
		nElectrostaticCheckStep = 100;
		break;
		
	case 100:
		if (st_work.mn_elec_serial_work == CTL_YES)				// ���� �Ϸ�
		{
			nElectrostaticCheckStep = 200;
		}
		else if (st_work.mn_elec_serial_work == CTL_NO)			// ������ �Ϸ�
		{
			nFunRet = CTLBD_RET_ERROR;
			nElectrostaticCheckStep = 0;
		}
		else													// �̵����� �ƴҶ�...
		{
			lElectSerialTime[1] = GetCurrentTime();
			lElectSerialTime[2] = lElectSerialTime[1] - lElectSerialTime[0];
			
			if (lElectSerialTime[2] > 100000)					// 10�� ���
			{
				nFunRet = CTLBD_RET_ERROR;
				nElectrostaticCheckStep = 0;
			}
		}
		break;
		
	case 200:
		if (nElectChannel == 5)									// ������ ä�α��� ������...
		{
			nElectChannel = 1;									// �����ϰ�.
			nElectrostaticCheckStep = 300;
		}
		else if (nElectChannel < 5)
		{
			nElectChannel++;
			nElectrostaticCheckStep = 0;
		}
		break;
		
	case 300:
		st_grounddb.mole_date = COleDateTime::GetCurrentTime();
		Func.DB_Write_Ground(st_grounddb);			// �����Ѵ�.
		
		for (i = 0; i < 5; i++)
		{
			if (st_work.mdReadElectData[i]  > st_basic.md_electrostatic_value)		// ������ + ������ �⺻�� ���� ũ��
			{
				nFunRet = CTLBD_RET_ERROR;
				nElectrostaticCheckStep = 0;
				break;
			}
		}
		
		// �����̴ϱ� ����.
		for (i = 0; i < 5; i++)
		{
			st_work.mdReadElectData[i] = 0;
			st_grounddb.mstrChannel[i] = "";
		}
		
		nElectrostaticCheckStep = 0;
		nFunRet = CTLBD_RET_GOOD;
		break;
	}
	
	return nFunRet;
}

void CMainFrame::OnMotSpeed() 
{
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Motor_Speed)))   return;
	OnSwitchToForm(IDW_SCREEN_MOTOR_SPEED);	
}

void CMainFrame::OnSetInterface() 
{
// 	// **************************************************************************
//     // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
//     // **************************************************************************
// 	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
// 	if (nmenu_chk != TRUE)  return;
// 	
// 	// **************************************************************************
// 	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
// 	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
// 	// **************************************************************************
// 	if (st_handler.mn_level_teach !=  TRUE)
// 	{
// 		if (st_handler.mn_level_maint !=  TRUE) 
// 		{
// 			OnLevel();
// 			if (st_handler.mn_level_teach !=  TRUE)
// 			{
// 				if (st_handler.mn_level_maint !=  TRUE)  return;
// 			}
// 		}
// 	}
// 	// **************************************************************************
// 	
// 	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Interface)))   return;
// 	OnSwitchToForm(IDW_SCREEN_SET_INTERFACE);	
}

////////////////////////////////////////////////////////////////////////////////////// 20120515

// void CMainFrame::OnSetMaintenance() 
// {
// 	// **************************************************************************
//     // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
//     // **************************************************************************
// 	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
// 	if (nmenu_chk != TRUE)  return;
// 	
// 	// **************************************************************************
// 	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
// 	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
// 	// **************************************************************************
// 	if (st_handler.mn_level_teach !=  TRUE)
// 	{
// 		if (st_handler.mn_level_maint !=  TRUE) 
// 		{
// 			OnLevel();
// 			if (st_handler.mn_level_teach !=  TRUE)
// 			{
// 				if (st_handler.mn_level_maint !=  TRUE)  return;
// 			}
// 		}
// 	}
// 	// **************************************************************************
// 	
// 	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Set_Maintenance)))   return;
// 	OnSwitchToForm(IDW_SCREEN_SET_MAINTENANCE);	
// }
//////////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnAdmin() 
{
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Admin)))   return;
	OnSwitchToForm(IDW_SCREEN_ADMINISTRATOR);
}

void CMainFrame::OnMain_Motor_Setting()
{
	int i = 0, j = 0, nRet = 0;

	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		COMI.Set_Simulation_Mode(i, 0);
	}

	//�����: LD X�� ���ְŸ�����=>10000(���ش�)/40mm(1ȸ���̵��Ÿ�) = 250(1mm�̵��� �ʿ��� pulse��), �̸� 0.1mm������ �������� 250/1000= 0.25 ���� ��

	// 1mm �̵��ϴµ� �ʿ��� pulse
	COMI.Set_MotUnitDist(M_LDM_STACKER_1, 1000);
	COMI.Set_MotUnitDist(M_LDM_STACKER_2, 1000);
 	COMI.Set_MotUnitDist(M_LDM_STACKER_MOVE, 200.1076579); //100.053829
 	COMI.Set_MotUnitDist(M_HS_F_STACKER_1, 1000);
	COMI.Set_MotUnitDist(M_HS_F_STACKER_2, 1000);
	COMI.Set_MotUnitDist(M_HS_F_STACKER_MOVE, 200.1076579); //100.053829
	COMI.Set_MotUnitDist(M_HS_B_STACKER_1, 1000);
	COMI.Set_MotUnitDist(M_HS_B_STACKER_2, 1000);
	COMI.Set_MotUnitDist(M_HS_B_STACKER_MOVE, 200.1076579); //100.053829
	COMI.Set_MotUnitDist(M_ULDM_STACKER_1, 1000);
	COMI.Set_MotUnitDist(M_ULDM_STACKER_2, 1000);
 	COMI.Set_MotUnitDist(M_ULDM_STACKER_MOVE, 200.1076579); //100.053829
 	COMI.Set_MotUnitDist(M_M_RBT_Y, 160.0861135);
	COMI.Set_MotUnitDist(M_M_RBT_Z, 1000);
	COMI.Set_MotUnitDist(M_HS_F_RBT_Y, 160.0861135);
	COMI.Set_MotUnitDist(M_HS_F_RBT_Z, 1000);
	COMI.Set_MotUnitDist(M_HS_B_RBT_Y, 160.0861135);
	COMI.Set_MotUnitDist(M_HS_B_RBT_Z, 1000);

	COMI.Set_MotUnitDist(M_CLIP_BUFFER, 333.4778404);
	COMI.Set_MotUnitDist(M_WORK_BUFFER_1, 196.3479285);
 	COMI.Set_MotUnitDist(M_WORK_BUFFER_2, 196.3479285);
 	COMI.Set_MotUnitDist(M_WORK_BUFFER_3, 196.3479285);
	COMI.Set_MotUnitDist(M_WORK_BUFFER_4, 196.3479285);
	COMI.Set_MotUnitDist(M_M_CLAMP_RBT_GRIPPER, 5000);
	COMI.Set_MotUnitDist(M_M_CLAMP_RBT_Y, 160.0861135);
	COMI.Set_MotUnitDist(M_M_CLAMP_RBT_Z, 1000);
	COMI.Set_MotUnitDist(M_CLIP_INSERT_Y, 160.0861135);
	COMI.Set_MotUnitDist(M_CLIP_INSERT_Z, 1000);
	COMI.Set_MotUnitDist(M_RBT_SORTER_X, 300.1614869);
 	COMI.Set_MotUnitDist(M_RBT_SORTER_Y, 200.1076579);
 	COMI.Set_MotUnitDist(M_RBT_SORTER_Z, 1000);
	COMI.Set_MotUnitDist(M_RBT_UNLOAD_Z, 1000);
	COMI.Set_MotUnitDist(M_RBT_UNLOAD_X, 300.1614869);
	COMI.Set_MotUnitDist(M_RBT_UNLOAD_Y, 200.1076579);
	COMI.Set_MotUnitDist(M_M_CLIP_CLAMP, 10000);
	COMI.Set_MotUnitDist(M_LABEL_ATTACH_Z, 1000);
	COMI.Set_MotUnitDist(M_VISION1_Y, 1000);
	COMI.Set_MotUnitDist(M_VISION1_Z, 1000);
	COMI.Set_MotUnitDist(M_VISION2_Y, 1000);
	COMI.Set_MotUnitDist(M_VISION2_Z, 1000);


	// 1mm �̵��ϴµ� �ʿ��� pulse (�ӵ��� 1mm/�ʷ� �����ϰ� �ȴ�.)
	COMI.Set_MotUnitSpeed(M_LDM_STACKER_1, 1000);
	COMI.Set_MotUnitSpeed(M_LDM_STACKER_2, 1000);
 	COMI.Set_MotUnitSpeed(M_LDM_STACKER_MOVE, 200.1076579);
 	COMI.Set_MotUnitSpeed(M_HS_F_STACKER_1, 1000);
	COMI.Set_MotUnitSpeed(M_HS_F_STACKER_2, 1000);
	COMI.Set_MotUnitSpeed(M_HS_F_STACKER_MOVE, 200.1076579);
	COMI.Set_MotUnitSpeed(M_HS_B_STACKER_1, 1000);
	COMI.Set_MotUnitSpeed(M_HS_B_STACKER_2, 1000);
	COMI.Set_MotUnitSpeed(M_HS_B_STACKER_MOVE, 200.1076579);
	COMI.Set_MotUnitSpeed(M_ULDM_STACKER_1, 1000);
	COMI.Set_MotUnitSpeed(M_ULDM_STACKER_2, 1000);
 	COMI.Set_MotUnitSpeed(M_ULDM_STACKER_MOVE, 200.1076579);
 	COMI.Set_MotUnitSpeed(M_M_RBT_Y, 160.0861135);
	COMI.Set_MotUnitSpeed(M_M_RBT_Z, 1000);
	COMI.Set_MotUnitSpeed(M_HS_F_RBT_Y, 160.0861135);
	COMI.Set_MotUnitSpeed(M_HS_F_RBT_Z, 1000);
	COMI.Set_MotUnitSpeed(M_HS_B_RBT_Y, 160.0861135);
	COMI.Set_MotUnitSpeed(M_HS_B_RBT_Z, 1000);

	COMI.Set_MotUnitSpeed(M_CLIP_BUFFER, 333.4778404);
	COMI.Set_MotUnitSpeed(M_WORK_BUFFER_1, 196.3479285);
 	COMI.Set_MotUnitSpeed(M_WORK_BUFFER_2, 196.3479285);
 	COMI.Set_MotUnitSpeed(M_WORK_BUFFER_3, 196.3479285);
	COMI.Set_MotUnitSpeed(M_WORK_BUFFER_4, 196.3479285);
	COMI.Set_MotUnitSpeed(M_M_CLAMP_RBT_GRIPPER, 5000);
	COMI.Set_MotUnitSpeed(M_M_CLAMP_RBT_Y, 160.0861135);
	COMI.Set_MotUnitSpeed(M_M_CLAMP_RBT_Z, 1000);
	COMI.Set_MotUnitSpeed(M_CLIP_INSERT_Y, 160.0861135);
	COMI.Set_MotUnitSpeed(M_CLIP_INSERT_Z, 1000);
	COMI.Set_MotUnitSpeed(M_RBT_SORTER_X, 300.1614869);
 	COMI.Set_MotUnitSpeed(M_RBT_SORTER_Y, 200.1076579);
 	COMI.Set_MotUnitSpeed(M_RBT_SORTER_Z, 1000);
	COMI.Set_MotUnitSpeed(M_RBT_UNLOAD_Z, 1000);
	COMI.Set_MotUnitSpeed(M_RBT_UNLOAD_X, 300.1614869);
	COMI.Set_MotUnitSpeed(M_RBT_UNLOAD_Y, 200.1076579);
	COMI.Set_MotUnitSpeed(M_M_CLIP_CLAMP, 10000);
	COMI.Set_MotUnitSpeed(M_LABEL_ATTACH_Z, 1000);
	COMI.Set_MotUnitSpeed(M_VISION1_Y, 1000);
	COMI.Set_MotUnitSpeed(M_VISION1_Z, 1000);
	COMI.Set_MotUnitSpeed(M_VISION2_Y, 1000);
	COMI.Set_MotUnitSpeed(M_VISION2_Z, 1000);

	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		COMI.md_limit_position[i][0] = -20;
		COMI.md_limit_position[i][1] = 3000;
	}

	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		cmmCfgSetCtrlMode(i, cmCTRL_SEMI_C);//cmCTRL_SEMI_C , ���ڴ�,(�ǵ�����) �������� ��ǥ��ǥ�� �����Ͽ� �̼��մϴ�
	}

	for (i = 0; i < MOT_MAXMOTOR; i++)
	{ 
		COMI.mn_homecheck_method[i] = 6;	//el on-> stop-> back-> el off - > stop 
	}

//	COMI.mn_homecheck_method[M_ROBOT_X] = 1;
//	COMI.mn_homecheck_method[M_ROBOT_Y] = 1;
//	COMI.mn_homecheck_method[M_ROBOT_Z] = 1;

	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		nRet = COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 2, 1);
		
		if (nRet != CTL_YES)
		{
			sprintf(st_msg.c_abnormal_msg, "Set_HomeSet_Config Error [MotNum[%d]]", i);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� �Ϸ� ��� ��û
		}
	}

	// �ִ� ��� ���ļ��� �����ϴ� ������� �ּҴ� 
	// 15.26 pps(min) ~ 1,000,000 pps(max), 6000rpm

	////////////////////////�ּ� ���ļ� ���� ����////////////////////////////////////////////
	//MinPPS = MaxPPS / 65,535
	//���� �� MaxPPS �� 655,350 �̸� MinPPS = 655,350 / 65,535 = 10 (PPS)�� �˴ϴ�.
	/////////////////////////////////////////////////////////////////////////////////////////
	cmmCfgSetSpeedRange(M_M_RBT_Z, 1000000); 
	cmmCfgSetSpeedRange(M_HS_F_RBT_Z, 1000000); 
	cmmCfgSetSpeedRange(M_HS_B_RBT_Z, 1000000); 
	cmmCfgSetSpeedRange(M_M_CLAMP_RBT_Z, 1000000); 
	cmmCfgSetSpeedRange(M_RBT_SORTER_Z, 1000000); 
	cmmCfgSetSpeedRange(M_RBT_UNLOAD_Z, 1000000); 
	cmmCfgSetSpeedRange(M_LABEL_ATTACH_Z, 1000000); 
	cmmCfgSetSpeedRange(M_VISION1_Z, 1000000); 
	cmmCfgSetSpeedRange(M_VISION2_Z, 1000000); 

	COMI.Set_MotInitSpeed(M_LDM_STACKER_1,  10); 
	COMI.Set_MotInitSpeed(M_LDM_STACKER_2,  10);
	COMI.Set_MotInitSpeed(M_HS_F_STACKER_1, 10);
	COMI.Set_MotInitSpeed(M_HS_F_STACKER_2, 10);
	COMI.Set_MotInitSpeed(M_HS_B_STACKER_1, 10); 
	COMI.Set_MotInitSpeed(M_HS_B_STACKER_2, 10);
	COMI.Set_MotInitSpeed(M_ULDM_STACKER_1, 10);
	COMI.Set_MotInitSpeed(M_ULDM_STACKER_2, 10);

	////////////////////////////////////////////////////////
	// SD ������ �����Ǹ� ��� �����ϰ� ���� 
	////////////////////////////////////////////////////////
	COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_MODE, cmTRUE);//cmFALSE:�ʱ�ӵ��� ���� cmTRUE: ������ ����
	COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_MODE, cmTRUE);
	COMI.Set_Motor_IO_Property(M_HS_F_STACKER_1, cmSD_MODE, cmTRUE);
	COMI.Set_Motor_IO_Property(M_HS_F_STACKER_2, cmSD_MODE, cmTRUE);
	COMI.Set_Motor_IO_Property(M_HS_B_STACKER_1, cmSD_MODE, cmTRUE);
	COMI.Set_Motor_IO_Property(M_HS_B_STACKER_2, cmSD_MODE, cmTRUE);
	COMI.Set_Motor_IO_Property(M_ULDM_STACKER_1, cmSD_MODE, cmTRUE);
	COMI.Set_Motor_IO_Property(M_ULDM_STACKER_2, cmSD_MODE, cmTRUE);


	COMI.Set_Motor_IO_Property(M_LDM_STACKER_1, cmSD_LOGIC, cmLOGIC_B); //cmLOGIC_A : 0 A����
	COMI.Set_Motor_IO_Property(M_LDM_STACKER_2, cmSD_LOGIC, cmLOGIC_B);
	COMI.Set_Motor_IO_Property(M_HS_F_STACKER_1, cmSD_LOGIC, cmLOGIC_B);
	COMI.Set_Motor_IO_Property(M_HS_F_STACKER_2, cmSD_LOGIC, cmLOGIC_B);
	COMI.Set_Motor_IO_Property(M_HS_B_STACKER_1, cmSD_LOGIC, cmLOGIC_B);
	COMI.Set_Motor_IO_Property(M_HS_B_STACKER_2, cmSD_LOGIC, cmLOGIC_B);
	COMI.Set_Motor_IO_Property(M_ULDM_STACKER_1, cmSD_LOGIC, cmLOGIC_B);
	COMI.Set_Motor_IO_Property(M_ULDM_STACKER_2, cmSD_LOGIC, cmLOGIC_B);

	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		COMI.Set_MotPower(i, IO_ON);
	}

	if (st_handler.cwnd_list != NULL)  //2010.1027 ����Ʈ �� ȭ�� ����
	{
		sprintf(st_msg.c_normal_msg, "[OnMain_Motor_Setting]");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
	}

//	if (st_handler.cwnd_list != NULL)  //2010.1027 ����Ʈ �� ȭ�� ����
//	{
//		sprintf(st_msg.c_normal_msg, "[OnMain_Motor_Setting]");
//		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
//	}
}

void CMainFrame::OnMain_Thread_Creating()
{
	// **********************************************************************
	// ����ġ �˻� ���� ���� ������ �����Ѵ�                                 
	// **********************************************************************
	m_thread[0]=AfxBeginThread(OnThread_Handler_Check, this);
	if (m_thread[0] != NULL) 	
		hThrHandle[0] = m_thread[0]->m_hThread;
	// **********************************************************************

	// **********************************************************************
	// Ÿ�� ���� ��� ���� ���� ������ �����Ѵ�                              
	// **********************************************************************
	m_thread[1]=AfxBeginThread(OnThread_Tower_Lamp_Check, this);
	if (m_thread[1] != NULL) 	
		hThrHandle[1] = m_thread[1]->m_hThread;
	// **********************************************************************
	
	// **********************************************************************
	// ��� ���� �� �߻��� �˶� ����ϱ� ���� ������ �����Ѵ�                
	// **********************************************************************
	m_thread[2]=AfxBeginThread(OnThread_Alarm_Display, this);
	if (m_thread[2] != NULL) 	
		hThrHandle[2] = m_thread[2]->m_hThread;
	// **********************************************************************	

	// **********************************************************************
	//  IO ������ ���� �����带 �����Ѵ�.                         
	// **********************************************************************
	m_thread[3]=AfxBeginThread(OnThread_IO,this);
	if (m_thread[3] != NULL) 	
		hThrHandle[3] = m_thread[3]->m_hThread;
	// **********************************************************************

}

void CMainFrame::OnMain_Thread_Destroy()
{
	// **************************************************************************
	if (hThrHandle[0])  // ������ ����
		::WaitForSingleObject(hThrHandle[0], 1500);
	// **************************************************************************
	
	// **************************************************************************
	if (hThrHandle[1])  // ������ ����
		::WaitForSingleObject(hThrHandle[1], 1500);
	// **************************************************************************
	
	// **************************************************************************
	if (hThrHandle[2])  // ������ ����
		::WaitForSingleObject(hThrHandle[2], 1500);
	// **************************************************************************
	
	// **************************************************************************
	if (hThrHandle[3])  // ������ ����
		::WaitForSingleObject(hThrHandle[3], 1500);
	// **************************************************************************
	
	// **************************************************************************
	if (hThrHandle[4])  // ������ ����
		::WaitForSingleObject(hThrHandle[4], 1500);
	// **************************************************************************
	
	// **************************************************************************
	if (hThrHandle[5])  // ������ ����
		::WaitForSingleObject(hThrHandle[5], 1500);
	// **************************************************************************
	
	// **************************************************************************
	if (hThrHandle[6])  // ������ ����
		::WaitForSingleObject(hThrHandle[6], 1500);
	// **************************************************************************
	
	// **************************************************************************
	if (hThrHandle[7])  // ������ ����
		::WaitForSingleObject(hThrHandle[7], 1500);
	// **************************************************************************
	
	// **************************************************************************
	if (hThrHandle[8])  // ������ ����
		::WaitForSingleObject(hThrHandle[8], 1500);
	// **************************************************************************
	
	// **************************************************************************
	if (hThrHandle[9])  // ������ ����
		::WaitForSingleObject(hThrHandle[9], 1500);
	// **************************************************************************	

	// **************************************************************************
	if (hThrHandle[10])  // ������ ����
		::WaitForSingleObject(hThrHandle[10], 1500);
	// **************************************************************************	

	// **************************************************************************
	if (hThrHandle[11])  // ������ ����
		::WaitForSingleObject(hThrHandle[11], 1500);
	// **************************************************************************

	// **************************************************************************
	if (hThrHandle[12])  // ������ ����
		::WaitForSingleObject(hThrHandle[12], 1500);
	// **************************************************************************

	// **************************************************************************
	if (hThrHandle[13])  // ������ ����
		::WaitForSingleObject(hThrHandle[13], 1500);
	// **************************************************************************

	// **************************************************************************
	if (hThrHandle[14])  // ������ ����
		::WaitForSingleObject(hThrHandle[14], 1500);
	// **************************************************************************

	// **************************************************************************
	if (hThrHandle[15])  // ������ ����
		::WaitForSingleObject(hThrHandle[15], 1500);
	// **************************************************************************

	// **************************************************************************
	if (hThrHandle[16])  // ������ ����
		::WaitForSingleObject(hThrHandle[16], 1500);
	// **************************************************************************

	// **************************************************************************
	if (hThrHandle[17])  // ������ ����
		::WaitForSingleObject(hThrHandle[17], 1500);
	// **************************************************************************

	// **************************************************************************
	if (hThrHandle[18])  // ������ ����
		::WaitForSingleObject(hThrHandle[18], 1500);
	// **************************************************************************

	// **************************************************************************
	if (hThrHandle[19])  // ������ ����
		::WaitForSingleObject(hThrHandle[19], 1500);
	// **************************************************************************
}

//////////////////////////////////////////////////////////////// 20120514 ������
LRESULT CMainFrame::OnSerialPort(WPARAM wParam,LPARAM lParam)
{
	if(wParam == YES)
	{
		OnMain_Port_Create(lParam);
	}
	else if(wParam == NO)
	{
		OnMain_Port_Close();
	}
	return 0;
}
////////////////////////////////////////////////////////////////

void CMainFrame::OnMain_Port_Create(int n_port)
{
	//////////////////////////////////////////////////////////////// 20120514 ������
	char parity;
	DWORD dwCommEvents;

	if(st_serial.n_connect[n_port] == YES) return;

	dwCommEvents = m_ports[n_port].MmdSerialGetCommEvents();  // �ø��� �̺�Ʈ ����
	dwCommEvents = 0;
	dwCommEvents |= EV_CTS;
	dwCommEvents |= EV_RXCHAR;
	
	switch(rs_232.n_serial_parity[n_port])
	{
		case 2:
			parity = 'E';
			break;
			
		case 1:
			parity = 'O';
			break;
			
		case 0:
			parity = 'N';
			break;
			
		default:
			parity = 'N';
			break;
	}
	
	if(m_ports[n_port].MmdSerialInitializePort(this, 
											   rs_232.n_serial_port[n_port], 
											   rs_232.n_serial_baudrate[n_port], 
											   parity, 
											   rs_232.n_serial_data[n_port], 
											   rs_232.n_serial_stop[n_port], 
											   1024, 
											   dwCommEvents)) 
	{
		m_ports[n_port].MmdSerialStartMonitoring();
	}
	else 
	{
		st_serial.str_port_chk[n_port] = "NOT FOUND";
	}
	
	mstr_received[n_port].Empty();  // ���� �޽��� �ӽ� ���� ���� �ʱ�ȭ
	// **************************************************************************
	
	if (st_serial.str_port_chk[n_port] == "NOT FOUND")
	{
		st_serial.n_connect[n_port] = NO;
	}
	else
	{
		st_serial.n_connect[n_port] = YES;
		st_serial.n_rec_chk[n_port] = TRUE;
	}
	////////////////////////////////////////////////////////////////
	
// 	int i;
// 	DWORD dwCommEvents[MAX_PORT];
// 
// 	if (DEVELOPER_PC)				// ���߿� PC�̸� ���~
// 	{
// 		return;
// 	}
// 	
// 	for (i = 0; i < MAX_PORT; i++)
// 	{		
// 		if (st_handler.mnSerial_Port_Creating[i] == CTL_NO)		// �����Ǿ� ���� ��������, �����Ѵ�.
// 		{
// 			// **************************************************************************
// 			//  ��Ʈ �����Ѵ�                                                    
// 			// **************************************************************************
// 			dwCommEvents[i] = m_ports[i].MmdSerialGetCommEvents();  // �ø��� �̺�Ʈ ����
// 			dwCommEvents[i] = 0;
// 			dwCommEvents[i] |= EV_CTS;
// 			dwCommEvents[i] |= EV_RXCHAR;
// 			
// 			if (m_ports[i].MmdSerialInitializePort(this, i + 1, 9600, 78, 8, 1, 1024, dwCommEvents[i])) 
// 				m_ports[i].MmdSerialStartMonitoring();
// 			else 
// 				st_serial.mstr_port_chk[i] = "NOT FOUND";
// 			
// 			mstr_received[i].Empty();  // ���� �޽��� �ӽ� ���� ���� �ʱ�ȭ
// 			// **************************************************************************
// 			
// 			if (st_serial.mstr_port_chk[i] == "NOT FOUND")
// 			{
// 				st_handler.mnSerial_Port_Creating[i] = CTL_READY;
// 			}
// 			else
// 			{
// 				st_handler.mnSerial_Port_Creating[i] = CTL_YES;
// 			}
// 		}
// 	}
}

void CMainFrame::OnExitData_Saving()
{
	st_handler.mn_menu_lock = TRUE;
	
	if (st_handler.cwnd_basic != NULL)				// BASIC ȭ�� ����
	{
		st_handler.cwnd_basic->SendMessage(WM_BASIC_APPLY, 0, 0);					// BASIC DATA APPLY ��û
	}
	else if (st_handler.cwnd_maintenance != NULL)	// Tower Lamp ȭ�� ����
	{
		st_handler.cwnd_maintenance->SendMessage(WM_MAINTENANCE_APPLY, 0, 0);		// Tower Lamp DATA APPLY ��û
	}
	else if (st_handler.cwnd_testreference != NULL)	// Test Reference ȭ�� ����
	{
		st_handler.cwnd_testreference->SendMessage(WM_TESTREFERENCE_APPLY, 0, 0);	// Test Reference DATA APPLY ��û
	}
	else if (st_handler.cwnd_waittime != NULL)		// Wait Time ȭ�� ����
	{
		st_handler.cwnd_waittime->SendMessage(WM_WAITTIME_APPLY, 0, 0);				// Wait Time DATA APPLY ��û
	}
	else if (st_handler.cwnd_motorspeed != NULL)	// Motor Speed ȭ�� ����
	{
		st_handler.cwnd_motorspeed->SendMessage(WM_MOTORSPEED_APPLY, 0, 0);			// Motor Speed DATA APPLY ��û
	}
	
	/* ************************************************************************** */
    /* ȭ�鿡 ������ ��� ������ ���Ͽ� �����Ѵ�                                  */
	/* -> ���α׷� ���� �������� ���۷����Ͱ� ������ ���� ���� �����ϴ� ��� ��� */
	/* -> ��� ���� �߿� ���� �����Ǵ��� ���� ���� ���� �����ȴ�                */
    /* ************************************************************************** */
	mcls_m_basic.OnBasic_Data_Save();				// ���� ������ ����� Basic ���� ������ ���Ͽ� �����ϴ� �Լ�
	mcls_m_basic.OnMotorSpeed_Set_Data_Save();		// ���� ������ ����� Motor Speed ���� ������ ���Ͽ� �����ϴ� �Լ�.
	mcls_m_basic.On_Teach_Data_Save();
	mcls_m_basic.OnMaintenance_Data_Save();
	mcls_m_basic.OnInterface_Data_Save();	//20120516
	mcls_m_basic.OnSkip_Data_Save();	//20120525
	mcls_m_basic.OnBasic_Label_Data_Save();	//20120531
	mcls_m_basic.OnBarcode_Label_Data_Save();	//20120604
	mcls_m_basic.OnTray_Data_Save();
	mcls_m_basic.OnWaitTime_Data_Save();			// ���� ������ ����� Wait Time ���� ������ ���Ͽ� �����ϴ� �Լ�
// 	mcls_m_basic.OnWaitTime_Data_Load();			// ���� ������ ����� Wait Time ���� ������ ���Ͽ� �����ϴ� �Լ�
	mcls_m_basic.OnSkip_Data_Save();				// ���� ������ ����� Basic ���� ������ ���Ͽ� �����ϴ� �Լ�
//	mcls_m_basic.OnSkip_Data_Load();

//	mcls_m_basic.On_WorkSite_Save();
	
	st_handler.mn_menu_lock = FALSE;
}

///////////////////////////////////////////////////////// 20120516
void CMainFrame::OnDataChange(WPARAM port, LPARAM ch)
{
	if(port == 0)
	{
		mcls_m_basic.OnBasic_Data_Save();				// ���� ������ ����� Basic ���� ������ ���Ͽ� �����ϴ� �Լ�
		mcls_m_basic.OnWaitTime_Data_Save();			// ���� ������ ����� Wait Time ���� ������ ���Ͽ� �����ϴ� �Լ�
		mcls_m_basic.OnMotorSpeed_Set_Data_Save();		// ���� ������ ����� Motor Speed ���� ������ ���Ͽ� �����ϴ� �Լ�.
		mcls_m_basic.On_Teach_Data_Save();
		mcls_m_basic.OnMaintenance_Data_Save();
		mcls_m_basic.OnInterface_Data_Save();	//20120516
		mcls_m_basic.OnSkip_Data_Save();	//20120525
		mcls_m_basic.OnBasic_Label_Data_Save();	//20120531
		mcls_m_basic.OnBarcode_Label_Data_Save();	//20120604
	}
	////2014,0406
	else if(port == 1 || port == 2) // 1: LRDIMM, 2: RDIMM
	{
		st_work.b_Auto_Model_change = TRUE;

		mcls_m_basic.OnBasic_Data_Load();				// ���� ������ ����� Basic ���� ������ ���Ͽ� �����ϴ� �Լ�
		mcls_m_basic.OnMotorSpeed_Set_Data_Load();		// ���� ������ ����� Motor Speed ���� ������ ���Ͽ� �����ϴ� �Լ�.
		mcls_m_basic.On_Teach_Data_Load();
		mcls_m_basic.OnMaintenance_Data_Load();
		mcls_m_basic.OnInterface_Data_Load();	//20120516
		mcls_m_basic.OnSkip_Data_Load();	//20120525
		mcls_m_basic.OnBasic_Label_Data_Load();	//20120531
		mcls_m_basic.OnBarcode_Label_Data_Load();	//20120604
		mcls_m_basic.OnWaitTime_Data_Load();			// ���� ������ ����� Wait Time ���� ������ ���Ͽ� �����ϴ� �Լ�
		mcls_m_basic.OnSkip_Data_Load();				// ���� ������ ����� Basic ���� ������ ���Ͽ� �����ϴ� �Լ�

		if(st_handler.cwnd_title != NULL)
		{
			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, 1);			// Device Mode

			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, BCR_MODE, 0);			// Device Mode
			st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DIRECTION_MODE, 0);			// Device Mode
		}
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);
		}

	}
	////

}
///////////////////////////////////////////////////////// 

void CMainFrame::OnMain_Port_Close()
{
	////////////////////////////////////////////////////// 20120514 ������
	int i;
	// **************************************************************************
	// ������ RS-232C �ø��� ��Ʈ �����Ѵ�                                       
	// -> ������ RS-232C �ø��� ��Ʈ�� �ٸ� ȭ�鿡�� ����ϰ��� �� ��쿡��      
	//    �ݵ�� ��� �� �κ��� ��Ʈ�� �ϴ� ������ �� ����ؾ� �Ѵ�              
	//  : �̹� �ش� RS-232C �ø��� ��Ʈ ���� ���¿��� �ٽ� �� �� ���� �����̴�   
	// **************************************************************************
	for(i = 1; i < MAX_PORT; i++)	//20120620
	{
		if(st_serial.n_connect[i] == YES)
		{
			m_ports[i].MmdSerialDestroyObject();
			st_serial.n_connect[i] = NO;
		}	
	}
	// **************************************************************************
	//////////////////////////////////////////////////////
}

void CMainFrame::OnMain_Motor_Destroy()
{

}

void CMainFrame::OnMainFrame_MessageDisplay()
{
	int ReturnVal;
	
	CDialog_Message msg_dlg;
	
	ReturnVal = msg_dlg.DoModal();
	
	if (ReturnVal == IDOK)
	{
		st_handler.mnSelectMessage = 1;
	}
	else
	{
		st_handler.mnSelectMessage = 1;
	}
}

LRESULT CMainFrame::OnMainMessageEvent(WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	CString strTemp;
	
	if (wParam == CTL_YES)
	{
		if (mp_msg_dlg != NULL && IsWindow(mp_msg_dlg->m_hWnd))
		{
			mp_msg_dlg->SetFocus();	// ��ȭ���ڸ� Ȱ��ȭ
			mp_msg_dlg->OnEventMsg_Text_Set();
		}
		else
		{
			mp_msg_dlg = new CDialog_Event_Msg;
			mp_msg_dlg->Create();
			mp_msg_dlg->ShowWindow(SW_SHOW);
		}
	}
	else if (wParam == CTL_NO)
	{
		st_msg.mn_event_msg_list = 0;
		
		for (i = 0; i < 3; i++)
		{
			st_msg.mstr_event_msg[i] = "";
		}
		
		if (mp_msg_dlg != NULL && IsWindow(mp_msg_dlg->m_hWnd))
		{
			mp_msg_dlg->ShowWindow(SW_HIDE);
			delete mp_msg_dlg;
			mp_msg_dlg = NULL;
		}
		if (st_alarm.mstr_cur_state == "0")	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_OFF);
		else                                FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_OFF);

	}
	
	return 0;
}

void CMainFrame::OnMainFrame_SelectMessageDisplay()
{
	int ReturnVal;
	
	CDialog_Select select_dlg;
	
	st_msg.mstr_confirm_msg = _T(st_handler.mstrSelectMessage);
	ReturnVal = select_dlg.DoModal();
	
	if (ReturnVal == IDOK)
	{
		st_handler.mnSelectMessage = 1;
	}
	else
	{
		st_handler.mnSelectMessage = 2;
	}
}

// ******************************************************************************
// RS-232C �ø��� ��Ʈ ���� �Լ�                                                 
// ******************************************************************************
LONG CMainFrame::OnCommunication(WPARAM port, LPARAM ch)
{ 
	// **************************************************************************
    // RS-232C �ø��� ��Ʈ �˻��Ѵ�                                              
    // **************************************************************************
	if (port <= 0 || port > MAX_PORT)  // COMM ��Ʈ �˻�
		return -1;
	// **************************************************************************
    
	if (ch == -1)  // �б� ����
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_serial.comm_err[port-1] = _T("[���� ������] �ø��� ��Ʈ �б� ���� �߻�");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, port, RECEIVE_MSG);  // ���� �޽��� ��� ��û
		}

		st_serial.n_rec_chk[port-1] = COM_ERROR;  // ���� ���� �÷��� ����
		return 0;
	}
	else if (ch == -2)  // ���� ����
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_serial.comm_err[port-1] = _T("[���� ������] �ø��� ��Ʈ ���� ���� �߻�");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, port, SEND_MSG);  // ���� �޽��� ��� ��û
		}

		st_serial.n_rec_chk[port-1] = COM_ERROR;  // ���� ���� �÷��� ����
		return 0;
	}
//////////////////////////////////////////// 20120620
// 	if (port == GMS_PORT)
// 	{
// 		OnMain_Rcv_GMS(port, ch);
// 	}

	

	if(port == rs_232.n_serial_port[MODULE_LOAD_PORT] || port == rs_232.n_serial_port[HEATSINK_LEFT_PORT] || port == rs_232.n_serial_port[HEATSINK_RIGHT_PORT] || port == rs_232.n_serial_port[CLIP_PORT] )  //BARCODE_PORT //20120608//20120519
	{
		OnReceiveDataBCR(port, ch);  // ���ŵ� 01 BCR ������ ó�� �Լ� 
	}
	else if (port == rs_232.n_serial_port[VISION_PORT])
	{
		OnReceiveDataVision(port, ch);  // ���ŵ� 01 BCR ������ ó�� �Լ� 
	}
	else if (port == rs_232.n_serial_port[LABEL_PRINTER_PORT])
	{
		OnReceiveDataLabelPrint(port, ch);  // ���ŵ� 01 BCR ������ ó�� �Լ� 
	}
	/////////////////////////////////////////////////////
	return 0;
}
// ******************************************************************************

// ******************************************************************************
// RS-232C �ø��� ��Ʈ �̺�Ʈ ���� �Լ�                                          
// ******************************************************************************
LONG CMainFrame::OnCommunicationEvent(WPARAM port, LPARAM event)
{
	// **************************************************************************
    // �̺�Ʈ ������ �ø��� ��Ʈ ��Ʈ �˻��Ѵ�                                   
    // **************************************************************************
	if (port <= 0 || port > MAX_PORT)  // COMM ��Ʈ �˻�
		return -1;
	// **************************************************************************

	if (event & EV_BREAK)
		Func.MsgLog ("EV_BREAK event");
	else if (event & EV_CTS)
		Func.MsgLog ("EV_CTS event");
	else if (event & EV_DSR)
		Func.MsgLog ("EV_DSR event");
	else if (event & EV_ERR)
		Func.MsgLog ("EV_ERR event");
	else if (event & EV_RING)
		Func.MsgLog ("EV_RING event");
	else if (event & EV_RLSD)
		Func.MsgLog ("EV_RLSD event");
	else if (event & EV_RXCHAR)
		Func.MsgLog ("EV_RXCHAR event");
	else if (event & EV_RXFLAG)
		Func.MsgLog ("EV_RXFLAG event");
	else if (event & EV_TXEMPTY)
		Func.MsgLog ("EV_TXEMPTY event");

	return 0;
}
// ******************************************************************************

// ******************************************************************************
// RS-232C �ø��� ��Ʈ�� ���� �޽��� ���� �Լ�                                   
// -> wParam : �޽��� ������ ��Ʈ �ѹ�                                           
// ******************************************************************************
LRESULT CMainFrame::OnDataSend(WPARAM wParam, LPARAM lParam)
{	
	int port = wParam; 
	int pos = lParam;
	int i = 0;
	CString data, temp;
	
	switch(port)
	{
		////////////////////////////////////////// 20120620
// 		case GMS_PORT:
// 			OnMain_Snd_Serial(wParam, lParam);
// 			break;

		case LABEL_PRINTER_PORT://20120612
		case VISION_PORT:	//20120608 BARCODE_PORT: 	//20120519
		case MODULE_LOAD_PORT:
		case HEATSINK_LEFT_PORT:
		case HEATSINK_RIGHT_PORT:
		case CLIP_PORT:
			OnSendDataBCR(wParam, lParam);  // 01 BCR ������ �۽� �Լ� 
		////////////////////////////////////////// 
			break;
	}
	return 0;
}

void CMainFrame::OnMain_Snd_Serial(WPARAM wParam, LPARAM lParam)
{
	int n_serial_chk;  // ������ �۽� �÷���
	
	char buf[1024];
	
	// **************************************************************************
	// �۽� ������ �ӽ� ���� ���� �ʱ�ȭ                                         
	// **************************************************************************
	memset(&buf, 0, sizeof(buf));
	//buf[0] = '\0';
	// **************************************************************************
	
	// **************************************************************************
	// �۽� ������ �� �������� �� ����/���� ���� ���� �����Ѵ�                 
	// **************************************************************************
	//(st_serial.str_snd[wParam-1]).TrimLeft(' ');
	//(st_serial.str_snd[wParam-1]).TrimRight(' ');
	// **************************************************************************
	
	// **************************************************************************
	// �۽� ������ ������ ���� �˻��Ѵ�                                          
	// **************************************************************************
	if (st_serial.str_snd[wParam] == _T(""))  // �۽� ������ ���� �˻�
	{
		if (st_handler.cwnd_list != NULL)       // ����Ʈ �� ȭ�� ����
		{
			st_serial.comm_err[wParam] = _T("[�۽� ������] ���� ������ ���� ���� �߻�");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, SEND_MSG);  // ���� �޽��� ��� ��û
		}
		
		st_serial.n_snd_chk[wParam] = COM_ERROR;  // �۽� ���� �÷��� ����
		return;
	}
	// **************************************************************************
	
	st_serial.comm_snd[wParam] = st_serial.str_snd[wParam];  
	sprintf(buf, st_serial.str_snd[wParam]);  // �۽� ������ ����
	
	// **************************************************************************
	// �Էµ� �۽� ������ �ø��� ��Ʈ�� ���� �����Ѵ�                            
	// **************************************************************************
	n_serial_chk = (m_ports[wParam]).MmdSerialWriteData(buf, strlen(buf), SERIAL_WAIT, SERIAL_TIME_INFINITE);
	
	if (n_serial_chk == 0)        // ���� �۽�
	{
		//if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		//    st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, 2);  // �۽� �޽��� ��� ��û
		
		st_serial.n_snd_chk[wParam] = CTL_YES;
	}
	else if (n_serial_chk == -1)  // ���� ����
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_serial.comm_err[wParam] = _T("[�۽� ������] �ø��� ��Ʈ ���� ���� �߻�");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, SEND_MSG);  // ���� �޽��� ��� ��û
		}
		
		st_serial.n_snd_chk[wParam] = -1;  
	}
	else if (n_serial_chk == -2)  // TIME OUT
	{
		if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
		{
			st_serial.comm_err[wParam] = _T("[�۽� ������] �ø��� ��Ʈ TIME OUT �߻�");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, SEND_MSG);  // ���� �޽��� ��� ��û
		}
		
		st_serial.n_snd_chk[wParam] = -2;
	}
	// **************************************************************************
}

void CMainFrame::OnSendDataBCR(WPARAM wParam, LPARAM lParam)
{
	int  nSerialCheck;
	char buf[1024];

	CSingleLock sing(&s_mutex);
	sing.Lock();

	/* ************************************************************************** */
	/* �۽� ������ �ӽ� ���� ���� �ʱ�ȭ                                          */
	/* ************************************************************************** */
	memset(&buf, 0, sizeof(buf));
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* �۽� ������ �� �������� �� ����/���� ���� ���� �����Ѵ�                  */
	/* ************************************************************************** */
	//(st_serial.str_snd[wParam-1]).TrimLeft(' ');
	//(st_serial.str_snd[wParam-1]).TrimRight(' ');
	/* ************************************************************************** */

	if(st_serial.n_connect[wParam] == NO)
	{
		OnMain_Port_Create(wParam);
	}

	if (st_serial.str_snd[wParam] == _T(""))  // �۽� ������ ���� �˻� 
	{
		if (st_handler.cwnd_list != NULL)       // ����Ʈ �� ȭ�� ���� 
		{
			//st_serial.comm_err[wParam-1] =_T("[�۽� ������] ���� ������ ���� ���� �߻�");
			st_serial.comm_err[wParam] =_T("[Send Data Error] Detected Blink in Data");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, 5);  // ���� �޽��� ��� ��û 
		}

		st_serial.n_snd_chk[wParam] = COM_ERROR;  // �۽� ���� �÷��� ���� 

		return ;
	}

	st_serial.comm_snd[wParam] = st_serial.str_snd[wParam];
	sprintf(buf, st_serial.str_snd[wParam]);  // �۽� ������ ���� 

	
	nSerialCheck = (m_ports[wParam]).MmdSerialWriteData(buf, strlen(buf), SERIAL_WAIT, SERIAL_TIME_INFINITE);
	if (nSerialCheck == 0)        // ���� �۽� 
	{
		st_serial.n_snd_chk[wParam] = 1;
	}
	else if (nSerialCheck == -1)  // ���� ���� 
	{
		if (st_handler.cwnd_list != NULL)
		{
			st_serial.comm_err[wParam] =_T("[COM Port Error] Sending Data");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, 5);
		}

		st_serial.n_snd_chk[wParam] = -1;
	}
	else if (nSerialCheck == -2)  // TIME OUT 
	{
		if (st_handler.cwnd_list != NULL)
		{
			st_serial.comm_err[wParam] =_T("[COM Port Error] Time Out");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, wParam, 5);
		}

		st_serial.n_snd_chk[wParam] = -2;
	}

	sing.Unlock();
}
void CMainFrame::OnReceiveDataBCR(WPARAM port, LPARAM ch)
{

	BOOL bRet;
	if (ch == 13 || ch == 0x03)  // ������ ������ �˻� [ETX] 20120620
	{
		mstr_received[port] += (char)ch;
		mstr_received[port].TrimLeft(' ');
		mstr_received[port].TrimRight(' ');
		
		st_serial.str_rec[port] = mstr_received[port];
		
		(mstr_received[port]).Empty();
		
		bRet = Func.OnBCR_CheckData(st_serial.str_rec[port], port);	//20120620
		
		if (bRet == TRUE)
		{
			sprintf(st_msg.c_normal_msg, mstr_received[port]);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			
			st_serial.n_rec_chk[port] = TRUE;
		}
		else if (bRet == FALSE)
		{
			st_serial.n_rec_chk[port] = FALSE;
		}
	}
	else
		mstr_received[port] += (char)ch;
}

void CMainFrame::OnReceiveDataVision(WPARAM port, LPARAM ch)
{

	BOOL bRet;
	if (ch == 0x03)  // ������ ������ �˻� [ETX]
	{
		mstr_received[port] += (char)ch;
		mstr_received[port].TrimLeft(' ');
		mstr_received[port].TrimRight(' ');
		
		st_serial.str_rec[port] = mstr_received[port];
		
		(mstr_received[port]).Empty();
		
		bRet = Func.OnVision_CheckData(st_serial.str_rec[port]);
		
		if (bRet == TRUE)
		{
			sprintf(st_msg.c_normal_msg, mstr_received[port]);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			
			st_serial.n_rec_chk[port] = TRUE;
		}
		else if (bRet == FALSE)
		{
			st_serial.n_rec_chk[port] = FALSE;
		}
	}
	else
		mstr_received[port] += (char)ch;
}

////////////// 20120620
void CMainFrame::OnReceiveDataLabelPrint(WPARAM port, LPARAM ch)
{

	BOOL bRet;
	CString sResult;

	if (ch == 0x03)   // ������ ������ �˻� [ETX]
	{
		mstr_received[port] += (char)ch;
		mstr_received[port].TrimLeft(' ');
		mstr_received[port].TrimRight(' ');
		
		st_serial.str_rec[port] = mstr_received[port];
		
		(mstr_received[port]).Empty();
		
		bRet = Func.OnLabelPrint_CheckData(st_serial.str_rec[port]);
		
		if (bRet == TRUE)
		{
			sprintf(st_msg.c_normal_msg, mstr_received[port]);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			
			st_serial.n_rec_chk[port] = TRUE;
		}
		else if (bRet == FALSE)
		{
			st_serial.n_rec_chk[port] = FALSE;
		}
	}
//////////////////////////////////////////////////////// 20120707
	else if(ch == 0x5d)   // "]"
	{
// 		mstr_received[port] += (char)ch;
// 		mstr_received[port].TrimLeft(' ');
// 		mstr_received[port].TrimRight(' ');
// 		
// 		st_serial.str_rec[port] = mstr_received[port];
// 		
// 		(mstr_received[port]).Empty();
// 		
// 		bRet = Func.OnPaperOut_CheckData(st_serial.str_rec[port]);
// 
// 		if (bRet == TRUE)
// 		{
// //			sprintf(st_msg.c_normal_msg, mstr_received[port]);
// //			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
// 			st_serial.n_rec_chk[port] = TRUE;
// 		}
// 		else if (bRet == FALSE)
// 		{
// 			st_serial.n_rec_chk[port] = FALSE;
// 		}

		/////////////// 20120906
		mstr_received[port] += (char)ch;
		st_serial.str_rec[port] = mstr_received[port];

		sResult.Empty();
		AfxExtractSubString(sResult, mstr_received[port], 3, ' ');//st_serial.comm_rec[port], 2, ' ');
		sResult.TrimLeft(' ');
		sResult.TrimLeft(' ');
		mstr_received[port].Empty();

		if (sResult == "COMPLETED")
		{
			sprintf(st_msg.c_normal_msg, mstr_received[port]);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			st_serial.n_rec_chk[port] = TRUE;
			st_barcode.b_print_complete = TRUE;
		}
		else
		{
			st_serial.n_rec_chk[port] = FALSE;
//			st_barcode.b_print_complete = FALSE;
		}

		/////////////////
	}
//////////////////////////////////////////////////////// 

	else
		mstr_received[port] += (char)ch;
}

//////////////

void CMainFrame::OnMain_Rcv_GMS(WPARAM port, LPARAM ch)
{
	CString str_check;
	int nRet;
	
	if (ch == 0x03)  // ������ ������ �˻�
	{
		mstr_received[port-1] += (char)ch;
		mstr_received[port-1].TrimLeft(' ');
		mstr_received[port-1].TrimRight(' ');
		
		st_serial.str_rec[port-1] = mstr_received[port-1];
		
//		st_msg.mstr_normal_msg = mstr_received[port-1];
		sprintf(st_msg.c_normal_msg, mstr_received[port-1]);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		
		(mstr_received[port-1]).Empty();
		
		nRet = Func.OnElect_CheckData(st_serial.str_rec[port-1]);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			st_work.mn_elec_serial_work = CTL_YES;		// �������� �亯�� �Դ�.
			st_serial.n_rec_chk[port-1] = TRUE;
		}
		else if (nRet == CTLBD_RET_ERROR)			// ���̰� �ȸ¾Ƽ� ����.
		{
			st_work.mn_elec_serial_work = CTL_NO;		// ���������� �亯�� �Դ�.
			st_serial.n_rec_chk[port-1] = FALSE;
		}
		
		st_serial.comm_rec[port-1] = st_serial.str_rec[port-1];  // ����Ʈ ȭ�鿡 ����� ���� ������ ����
	}
	else
	{
		mstr_received[port-1] += (char)ch;
	}
}	

void CMainFrame::OnClose() 
{
	OnExit();
}

void CMainFrame::OnDestroy() 
{
	int i;
	/* ************************************************************************** */
	/* ���� ��޸��� �˶� ȭ�� �������� �ʾҴٸ� �� ���� �۾�                     */
	/* -> mp_modal_dlg : �˶� ȭ�鿡 ���� ������ ���� ����                        */
	/* ************************************************************************** */
	if (mp_alarm_dlg)
	{
		OnAlarm_Destroy();			// �˶� ȭ�� ���� �Լ� */
		delete this;
	}
	/* ************************************************************************** */
	
	for(i=0; i<10; i++)
	{
		if(m_p_server[i] != NULL)
		{
			delete m_p_server[i];
			m_p_server[i] = NULL;
		}

		if(m_p_client[i] != NULL)
		{
			delete m_p_client[i];
			m_p_client[i] = NULL;
		}
	}
	
	KillTimer(TMR_MAIN_REFRESH);	// Time ��� Ÿ�̸� ����
	KillTimer(TMR_FILE_CREATE);

	OnMain_Port_Close();	//20120514 ������			// ������ �ø��� ��Ʈ ���� �Լ�

	CFrameWnd::OnDestroy();
}

void CMainFrame::OnAlarm_Destroy()
{
	if (mp_alarm_dlg)
	{
		if (mp_alarm_dlg->IsWindowVisible())
		{
			mp_alarm_dlg->DestroyWindow();
		}
		
		if (st_handler.mn_alarm_screen != FALSE)  // �˶� ȭ�� ���
			st_handler.mn_alarm_screen = FALSE;
	}
}

int CMainFrame::OnMain_New_Lot_In()
{
	int nFuncRet = CTLBD_RET_PROCEED;

	switch(mn_new_lot_in_step)
	{
	case 0:
		if (st_work.mn_client_ready[NETWORK_NMACHINE] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_NMACHINE] == CTL_YES)
			{
				mn_new_lot_in_step  = 200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_NMACHINE, 0);
				}
				
				ml_wait_time[0] = GetCurrentTime();
				mn_new_lot_in_step  = 100;
			}
		}
		break;
		
	case 100:
		if (st_work.mn_client_accept[NETWORK_NMACHINE] == CTL_YES)
		{
			mn_new_lot_in_step  = 200;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;
				mn_new_lot_in_step  = 0;
			}
		}
		break;
		
	case 200:
		st_sync.mstr_new_lot_in = "READY";
		ml_retry_wait[0] = GetCurrentTime();

		st_work.mstr_client_send_msg[NETWORK_NMACHINE] = Func.OnNetworkDataMake(NW_NEW_LOT_IN_SEND);
		st_work.mn_client_rpy[NETWORK_NMACHINE] = CTL_YES;
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->SendMessage(CLIENT_SEND, NETWORK_NMACHINE, 0);
		}

		mn_new_lot_in_step  = 300;
		break;

	case 300:						// Buffer ready�� ���� Reply�� ���� Buffer�� ���� ���� �ִ´�.
		if (st_sync.mstr_new_lot_in == "PASS")
		{
			st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;
			mn_new_lot_in_step  = 400;
		}
		else
		{
			ml_retry_wait[1] = GetCurrentTime();
			ml_retry_wait[2] = ml_retry_wait[1] - ml_retry_wait[0];

			st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;

			if (ml_retry_wait[2] > st_basic.mn_network_wait_time * 1000)		// ������ �����ϱ� ������ X 60... msec�̱� ������ X 1000
			{
				mn_new_lot_in_step  = 0;
			}
		}
		break;

	case 400:
		mn_new_lot_in_step  = 0;
		nFuncRet = CTLBD_RET_GOOD;
		break;
	}

	return nFuncRet;
}

int CMainFrame::OnMain_Lot_name()
{
	CString strBody, strHeader, strFullMsg;
	int nBody = 0;

	int nFuncRet = CTLBD_RET_PROCEED;

	switch(mn_lot_name_step)
	{
	case 0:
		if (st_work.mn_client_ready[NETWORK_APCT] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_APCT] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_APCT] == CTL_YES)
			{
				mn_lot_name_step  = 200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_APCT, 0);
				}
				
				ml_wait_time[0] = GetCurrentTime();
				mn_lot_name_step  = 100;
			}
		}
		break;
		
	case 100:
		if (st_work.mn_client_accept[NETWORK_APCT] == CTL_YES)
		{
			mn_lot_name_step  = 200;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				st_work.mn_client_ready[NETWORK_APCT] = CTL_YES;
				mn_lot_name_step  = 0;
			}
		}
		break;

	case 200:
//		st_NW.mstr_m_lot_no[1] = st_handler.mstr_lot_name[st_handler.mn_comm_lot_name_chk];
//		st_work.mstr_client_send_msg[NETWORK_APCT] = Func.OnNetworkDataMake(NW_LOT_NAME);

		strBody		= "FUNCTION=LOT_NAME";
		strBody		+= " LOT_NO=" + st_handler.mstr_lot_name[st_handler.mn_comm_lot_name_chk];

		nBody = strBody.GetLength();

		if (nBody > 0)
		{
			strHeader.Format("%016d", nBody);
			strFullMsg = strHeader + strBody;
		}
		else
		{
			strFullMsg = "";
		}

		st_work.mstr_client_send_msg[NETWORK_APCT]  = strFullMsg;
		
		st_work.mn_client_rpy[NETWORK_APCT] = CTL_YES;
		st_sync.mn_lot_name_send = CTL_NO;

		ml_wait_time[0] = GetCurrentTime();
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->SendMessage(CLIENT_SEND, NETWORK_APCT, 0);
		}
		mn_lot_name_step = 300;
		break;

	case 300:
		// **********************************************************************
		//  Reply�� �м��Ѵ�.
		// **********************************************************************
		if (st_sync.mn_lot_name_send == CTL_COMPLETE)
		{
			st_work.mn_client_ready[NETWORK_APCT] = CTL_YES;
			mn_lot_name_step = 5000;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				st_work.mn_client_ready[NETWORK_APCT] = CTL_YES;
				mn_lot_name_step = 0;
			}
		}
		break;

	case 5000:
		nFuncRet = CTLBD_RET_GOOD;
		mn_lot_name_step = 0;
		break;
	}

	return nFuncRet;
}

void CMainFrame::OnListOper() 
{
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	/* ************************************************************************** */
	/* �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                               */
	/* -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                       */
	/* ************************************************************************** */
	//	if (st_ICT.n_level_teach !=  TRUE)
	//	{
	//		if (st_ICT.n_level_maint !=  TRUE)  return;
	//	}
	/* ************************************************************************** */
	
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Operation)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_OPERATION);
}

void CMainFrame::OnListDataview() 
{
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	/* ************************************************************************** */
	/* �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                               */
	/* -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                       */
	/* ************************************************************************** */
	//	if (st_ICT.n_level_teach !=  TRUE)
	//	{
	//		if (st_ICT.n_level_maint !=  TRUE)  return;
	//	}
	/* ************************************************************************** */
	
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Data_View)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_DATA_VIEW);
}

//////////////////////////////////////////////////// 20120701
 void CMainFrame::OnListStep() 
 {
	/* ************************************************************************** */
    /* ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                       */
    /* ************************************************************************** */
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	/* ************************************************************************** */
	/* �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                               */
	/* -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                       */
	/* ************************************************************************** */
	//	if (st_ICT.n_level_teach !=  TRUE)
	//	{
	//		if (st_ICT.n_level_maint !=  TRUE)  return;
	//	}
	/* ************************************************************************** */
	
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_List_Step)))   return;
	OnSwitchToForm(IDW_SCREEN_LIST_STEP);
}
///////////////////////////////////////////////////////


void CMainFrame::OnMainFrame_ScrapInfoDisplay()
{
	int ReturnVal;
	
	CDialog_Scrap_Info_View		scrap_dlg;
	
	ReturnVal = scrap_dlg.DoModal();
	
	if (ReturnVal == IDOK)
	{
		st_handler.nScrapInfoView = 1;
	}
	else
	{
		st_handler.nScrapInfoView = 1;
	}
}
void CMainFrame::OnMainFrame_TrackInDisplay()
{
}

void CMainFrame::OnMainFrame_TrackOutDisplay()
{
	int ReturnVal;
	
	CDialog_Track_Out	trackout_dlg;
	
	ReturnVal = trackout_dlg.DoModal();
	
	if (ReturnVal == IDOK)
	{
		st_handler.nTrackOutView = 1;
	}
	else
	{
		st_handler.nTrackOutView = 1;
	}
}

void CMainFrame::OnFileEditor() 
{
	// **************************************************************************
    // ȭ�� �� ��ȯ �Ұ����� ���� �˻��Ѵ�.                                      
    // **************************************************************************
	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
	// **************************************************************************
	if (st_handler.mn_level_teach !=  TRUE)
	{
		if (st_handler.mn_level_maint !=  TRUE) 
		{
			OnLevel();
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE)  return;
			}
		}
	}
	// **************************************************************************
	
	if (GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_File_Editor)))   return;
	OnSwitchToForm(IDW_SCREEN_FILE_EDITOR);	
}

void CMainFrame::OnPgmInfo() 
{
	// TODO: Add your command handler code here
	CDialog_Pass_Check pass_dlg;
	int n_response;

	int nmenu_chk = OnMenu_Change_Checking(); // �޴� ��� ���� ���� �˻� �Լ�
	if (nmenu_chk != TRUE)  return;
	
	// **************************************************************************
	// �޴� ��� �Ұ����� ��쿡 ���� ���� �˻��Ѵ�                              
	// -> ����Ʈ ��� �Ǵ� ƼĪ ��尡 �����Ǿ� �ִ��� �˻�                      
	// **************************************************************************
	st_handler.mstr_pass_level = "Master";
	
	n_response = pass_dlg.DoModal();
	
	if (n_response != IDOK)
	{
		return;
	}
	
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CScreen_Pgm_Info)))   return;
	
	OnSwitchToForm(IDW_SCREEN_PGM_INFO);	
}

void CMainFrame::Init_ToolTip()
{
	CRect	rect_hot;
	int		n_index;

	m_p_tooltip.Create(this);
	
	n_index = m_wndToolBar.CommandToIndex(ID_MAIN);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Main Screen</b><ct=0x00C000><br><hr=100%><br></ct>Total Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_BASIC);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Basic Screen</b><ct=0x00C000><br><hr=100%><br></ct>Basic Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_SETTING);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Setting Screen</b><ct=0x00C000><br><hr=100%><br></ct>Setting Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_MOTOR);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Motor Screen</b><ct=0x00C000><br><hr=100%><br></ct>Motor Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_IO);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>IO Screen</b><ct=0x00C000><br><hr=100%><br></ct>IO Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_LIST);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>List Screen</b><ct=0x00C000><br><hr=100%><br></ct>List Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_SAVE);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Job File Save</b><ct=0x00C000><br><hr=100%><br></ct>Job File Save."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_ALARM);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Alarm Screen</b><ct=0x00C000><br><hr=100%><br></ct>Alarm Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_LOCK);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Program Lock/UnLock Screen</b><ct=0x00C000><br><hr=100%><br></ct>Program Lock/UnLock View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_PGM_INFO);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Program Information Screen</b><ct=0x00C000><br><hr=100%><br></ct>Program Information View Screen."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
	
	n_index = m_wndToolBar.CommandToIndex(ID_EXIT);
	m_wndToolBar.GetItemRect(n_index, rect_hot);
	m_p_tooltip.AddTool(&m_wndToolBar, 
						_T("<al_c><ct=0xFF0000><b>Program Exit</b><ct=0x00C000><br><hr=100%><br></ct>Program Exit."), 
						IDI_TT_MAIN, CSize(0, 0), rect_hot);
}

//////////////////////////////////// 20120608
LRESULT CMainFrame::OnWork_Command(WPARAM wParam,LPARAM lParam)   
{
	switch(wParam)
	{
		case MAIN_VISION_SEND:
//			Func.Vision_Data(lParam, &st_work.str_vision_msg);
//			st_serial.str_snd[VISION_PORT] = st_work.str_vision_msg;
//			::SendMessage(st_handler.hWnd, WM_DATA_SEND, VISION_PORT, 0);  // ������ �۽� ��û
			break;
	////////////////////////////////////// 20120711
		case MAIN_MYSQL_ALARM_WRITE:
			OnMain_Alarm_Mysql_Write();
			break;
	////////////////////////////////////// 
		case MAIN_MYSQL_LOTALARM_WRITE:
			OnMain_LotAlarm_Mysql_Write();
			break;

		case MAIN_MYSQL_LOTWORK_WRITE:
			OnMain_Work_Mysql_Write();
			break;

		case MAIN_LOTEND_READY:
			OnMainLotReadyExecute(lParam);
			break;
		//////////////////////////////// 20121120
		case MAIN_RCV_BARCODE:
			if(m_n_rcv_bcr_step != 0) return 0;
			SetTimer(TMR_MAIN_RCV_BARCODE, 100, NULL);
			break;
		////////////////////////////////
	}
	return 0;
}



//Server��  �۽��ϴ� ����Ÿ
LRESULT CMainFrame::OnCommand_Client_1(WPARAM wParam, LPARAM lParam)
{	
	CString str_send;
	CString str_rev;
	CString strData;
	CString str_tmp;
	int nHead = 0;
	int nLength;

	switch(wParam)
	{
		case CLIENT_CONNECT:
			if(st_client[lParam].n_connect == NO)
			{
				m_p_client[lParam] = new CClientSocket;
				if(lParam < NETWORK_CLIENT_7)
				{
					if(m_p_client[lParam]->Create(lParam, st_client[NETWORK_CLIENT_1].str_ip, st_client[NETWORK_CLIENT_1].n_port, 1))
					{
						st_client[lParam].n_connect = YES;
						if(st_handler.cwnd_list != NULL)
						{
							str_tmp.Format("Client_[%02d] Connect Success..", lParam);
							sprintf(st_msg.c_normal_msg, str_tmp);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
						}
					}
					else
					{
						if(st_handler.cwnd_list != NULL)
						{
							str_tmp.Format("Client_[%02d] Connect Fail..", 0);
							sprintf(st_msg.c_abnormal_msg, str_tmp);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� �Ϸ� ��� ��û
						}
					}
				}
				else
				{
					if(m_p_client[lParam]->Create(lParam, st_client[lParam].str_ip, st_client[lParam].n_port, 1))
					{
						st_client[lParam].n_connect = YES;
						if(st_handler.cwnd_list != NULL)
						{
							str_tmp.Format("Client_[%02d] Connect Success..", lParam);
							sprintf(st_msg.c_normal_msg, str_tmp);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
						}
					}
					else
					{
						if(st_handler.cwnd_list != NULL)
						{
							str_tmp.Format("Client_[%02d] Connect Fail..", 0);
							sprintf(st_msg.c_abnormal_msg, str_tmp);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� �Ϸ� ��� ��û
						}
					}
				}

			}
			break;

		case CLIENT_CLOSE:
			if(st_client[lParam].n_connect == YES)
			{
				delete m_p_client[lParam];
				m_p_client[lParam]			= NULL;
				st_client[lParam].n_connect	= NO;

				if(st_handler.cwnd_list != NULL)
				{
					str_tmp.Format("Client_[%02d] Close..", lParam);
					sprintf(st_msg.c_normal_msg, str_tmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
				}
			}
			break;

		case CLIENT_SEND:
			if(st_client[lParam].n_connect == YES)
			{
				str_send.Format("%s", st_client[lParam].ch_send);

//				CString strHead;
//				strHead.Format("%016d",str_send.GetLength());
//				m_p_client[lParam]->Send(strHead,strHead.GetLength());
//
//				for (int i = 0; i < 100000000; i++)
//				{
//				}
//				::Sleep(10);
				
				m_p_client[lParam]->Send(str_send, str_send.GetLength());

				st_client[lParam].n_rev_info = YES;
				if(st_client[lParam].n_rev_info == NO)
				{
					delete m_p_client[lParam];
					m_p_client[lParam]			= NULL;
					st_client[lParam].n_connect	= NO;
				}

				if(st_handler.cwnd_list != NULL)
				{
					str_tmp.Format("Client_[%02d] - %s Data Send", lParam, str_send);
					sprintf(st_msg.c_normal_msg, str_tmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
				}

				st_handler.mn_receive = NO;
			}
			break;

		case CLIENT_REV:
/*			str_rev.Format("%s", st_client[lParam].ch_rev);

			st_client[lParam].n_client_no = lParam;

			st_NW.mstr_result[1] = "";
			
			Func.OnNetworkDataAnalysis(st_client[lParam].ch_rev);

			if(st_client[lParam].n_rev_info == YES)
			{
				delete m_p_client[lParam];
				m_p_client[lParam]			= NULL;
				st_client[lParam].n_connect	= NO;
			}

			if(st_handler.cwnd_list != NULL)
			{//Ȯ���� �ʿ��Ҷ��� ���
				str_tmp.Format("Client_[%02d] - %s Data Receive", lParam, str_rev);
				sprintf(st_msg.c_normal_msg, str_tmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
			}
			st_handler.mn_receive = YES;
			break;*/

			CString strPrint;
			
			str_rev.Format("%s", st_client[lParam].ch_rev);

			if(st_handler.cwnd_list != NULL)
			{//Ȯ���� �ʿ��Ҷ��� ���
				strPrint.Format("Client_[%02d] - %s Data Receive", lParam, str_rev);
				sprintf(st_msg.c_normal_msg, strPrint);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
			}

//			strData = str_rev.Mid(16, st_client[lParam].n_rev_length);
			str_tmp = strData.Mid(0, 16);
//			nHead = atoi(strData);
			int aa = str_rev.GetLength();
			nHead = atoi(str_rev);//������� ��

			char ybs[8192];

			memset(ybs,0x00,8192); 

			if (st_handler.cwnd_list != NULL)  // ����Ʈ �� ȭ�� ����
			{
				sprintf(st_msg.c_abnormal_msg, "�޸𸮸� ���� �߽��ϴ�.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "The memory has been deleted.");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� �Ϸ� ��� ��û			
			}

			for (int bb = 0; bb < st_client[lParam].n_rev_length; bb++)
			{
				ybs[bb] = st_client[lParam].ch_rev[16+bb];
			}

			strData.Format("%s", ybs);


			if(st_handler.cwnd_list != NULL)
			{//Ȯ���� �ʿ��Ҷ��� ���
				strPrint.Format("Client_[%02d] - %s Data Receive", lParam, strData);
				sprintf(st_msg.c_normal_msg, strPrint);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
			}

			if (nHead > 0)
			{
				nLength = strData.GetLength();// - 16;

				if (nHead == nLength)
				{
//					strData.Format("%s",st_client[lParam].ch_rev);
//					str_tmp = strData.Mid(16, nLength);
					//Func.OnNetworkDataAnalysis(st_client[lParam].ch_rev);
//					Func.OnNetworkDataAnalysis(str_tmp);
					Func.OnNetworkDataAnalysis(strData);
					if(st_client[lParam].n_rev_info == YES)
					{
						delete m_p_client[lParam];
						m_p_client[lParam]			= NULL;
						st_client[lParam].n_connect	= NO;

						st_handler.mn_receive = YES;
					}
				}
				else
				{
					m_nHead = nHead;
					m_strData = strData.Mid(16, nLength);
				}
			}
			else 
			{
				nLength = strData.GetLength();
				
				if (m_nHead == (m_nHead + nLength))
				{
					m_strData += strData;
					//Func.OnNetworkDataAnalysis(st_client[lParam].ch_rev);
					Func.OnNetworkDataAnalysis(m_strData);
					m_strData = "";
					m_nHead = 0;

					if(st_client[lParam].n_rev_info == YES)
					{
						delete m_p_client[lParam];
						m_p_client[lParam]			= NULL;
						st_client[lParam].n_connect	= NO;

						st_handler.mn_receive = YES;
					}
				}
				else if (m_nHead > (m_nHead + nLength))
				{
					m_strData += strData;
				}
				else
				{
					m_strData += strData.Mid(0, nLength - (m_nHead - m_strData.GetLength()));
					//Func.OnNetworkDataAnalysis(st_client[lParam].ch_rev);
					Func.OnNetworkDataAnalysis(m_strData);
					m_strData = "";
					m_nHead = 0;

					if(st_client[lParam].n_rev_info == YES)
					{
						delete m_p_client[lParam];
						m_p_client[lParam]			= NULL;
						st_client[lParam].n_connect	= NO;

						st_handler.mn_receive = YES;
					}
				}
			}
			
/*
			if(st_client[lParam].n_rev_info == YES)
			{
				delete m_p_client[lParam];
				m_p_client[lParam]			= NULL;
				st_client[lParam].n_connect	= NO;
			}
*/
			
			if(st_handler.cwnd_list != NULL)
			{//Ȯ���� �ʿ��Ҷ��� ���
//				str_tmp.Format("Client_[%02d] - %s Data Receive", lParam, str_rev);
//				sprintf(st_msg.c_normal_msg, str_tmp);
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
			}
//			st_handler.mn_receive = YES;
			if(st_client[lParam].n_rev_info == NO)
			{
				delete m_p_client[lParam];
				m_p_client[lParam]			= NULL;
				st_client[lParam].n_connect	= NO;

				st_handler.mn_receive = YES;
			}
			break;
	}
	
	return 0;
}

LRESULT CMainFrame::OnCommand_Server_1(WPARAM wParam, LPARAM lParam)
{
	CString str_send;
	CString str_rev;
	CString str_tmp;
	int		count;

	switch(wParam)
	{
		case SERVER_CONNECT:
			if(st_server[lParam].n_connect == NO)
			{
				m_p_server[lParam] = new CServerSocket;
				if(m_p_server[lParam]->Create(lParam, st_server[lParam].n_port))
				{
					st_server[lParam].n_connect = YES;
					if(st_handler.cwnd_list != NULL)
					{
						str_tmp.Format("Server_[%02d] Start Success..", lParam);
						sprintf(st_msg.c_normal_msg, str_tmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
					}
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						str_tmp.Format("Server_[%02d] Start Fail..", lParam);
						sprintf(st_msg.c_abnormal_msg, str_tmp);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // ���� �Ϸ� ��� ��û
					}
				}
			}
			break;
			
		case SERVER_CLOSE:
			if(st_server[lParam].n_connect == YES)
			{
				/*delete m_p_server[lParam];
				m_p_server[lParam]			= NULL;
				st_server[lParam].n_connect = NO;
			
				if(st_handler.cwnd_list != NULL)
				{
					str_tmp.Format("Server_[%02d] Close..", lParam);
					sprintf(st_msg.c_normal_msg, str_tmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
				}*/
			}
			break;
			
		case SERVER_SEND:
			if(st_server[lParam].n_connect == YES)
			{
				count = m_p_server[lParam]->GetClientCount();
				if (count <= 0)
				{
					break;
				}

				str_send.Format("%s", st_server[lParam].ch_send);
				m_p_server[lParam]->SendClientAt(m_p_server[lParam]->GetClient(count-1), str_send, str_send.GetLength());
				
				if(st_handler.cwnd_list != NULL)
				{
					str_tmp.Format("Server_[%02d] - %s Data Send", lParam, str_send);
					sprintf(st_msg.c_normal_msg, str_tmp);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
				}
			}

			break;
			
		case SERVER_REV:
			str_rev.Format("%s", st_server[lParam].ch_rev);
			
			if(st_handler.cwnd_list != NULL)
			{
				str_tmp.Format("1�� Last : Server_[%02d] - %s Data Receive", lParam, str_rev);
				sprintf(st_msg.c_normal_msg, str_tmp);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� �Ϸ� ��� ��û
			}
			break;

			st_client[lParam].n_client_no = lParam;
			Func.OnNetworkDataAnalysis(st_server[lParam].ch_rev);
			break;
	}
	
	return 0;
}

//////////////////////// 20120711
void CMainFrame::OnMain_Alarm_Mysql_Write()
{
	g_cs.EnterCriticalSection();
	CString		strTmp[6];
	CTime time = CTime::GetCurrentTime();
	
	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d",	st_work.t_jamcode_start.GetYear(), 
		st_work.t_jamcode_start.GetMonth(),
		st_work.t_jamcode_start.GetDay(),
		st_work.t_jamcode_start.GetHour(),
		st_work.t_jamcode_start.GetMinute(),
		st_work.t_jamcode_start.GetSecond());
	
	strTmp[2].Format("%04d/%02d/%02d %02d:%02d:%02d",	st_work.t_jamcode_end.GetYear(), 
		st_work.t_jamcode_end.GetMonth(),
		st_work.t_jamcode_end.GetDay(),
		st_work.t_jamcode_end.GetHour(),
		st_work.t_jamcode_end.GetMinute(),
		st_work.t_jamcode_end.GetSecond());
	strTmp[3] = alarm.str_lastcode;
	strTmp[4] = st_alarm.mstr_cur_msg; //"Loadng ���� ������ Ȯ���� �ּ���.";
	strTmp[5].Format("%d", st_work.t_jamcode_end - st_work.t_jamcode_start);
	
	
	if(Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		// amt database ����
		if(Mysql.Database_Select("AMT830"))
		{
			//  lot_data table ���� 
			if(Mysql.Table_Select("AMT830", "JAMCODE"))
			{
				// lot_data table�� ������ �߰�
				Mysql.Record_Add("JAMCODE", strTmp);
			}
			else
			{
				// table�� ������ ���� table �߰�
				if(Mysql.Table_Create("AMT830", "JAMCODE", 6, Mysql.m_str_jamcode_table_name, Mysql.m_n_jamcode_table_type, Mysql.m_n_jamcode_table_num))
				{
					// lot_data table�� ������ �߰�
					Mysql.Record_Add("JAMCODE", strTmp);
				}
			}
		}
		else
		{
			// amt database�� ������ database �߰�
			if(Mysql.Database_Create("AMT830"))
			{
				// lot_data table ���� �߰� 
				if(Mysql.Table_Create("AMT830", "JAMCODE", 6, Mysql.m_str_jamcode_table_name, Mysql.m_n_jamcode_table_type, Mysql.m_n_jamcode_table_num))
				{
					Mysql.Record_Add("JAMCODE", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();
	g_cs.LeaveCriticalSection();
}

void CMainFrame::OnMain_LotAlarm_Mysql_Write()
{
	g_cs.EnterCriticalSection();
	CString strTmp[6];
	
	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_alarmdb.mole_date.GetYear(), 
		st_alarmdb.mole_date.GetMonth(),
		st_alarmdb.mole_date.GetDay(),
		st_alarmdb.mole_date.GetHour(),
		st_alarmdb.mole_date.GetMinute(),
		st_alarmdb.mole_date.GetSecond());
	
	st_alarmdb.mstrlot_no = st_handler.str_main_lot;
	st_alarmdb.mstrpart_no = st_handler.str_main_part;
	
	strTmp[2].Format("%-1s", st_alarmdb.mstrlot_no);
	strTmp[3].Format("%-1s", st_alarmdb.mstrpart_no);
	
	strTmp[4].Format("%-1s", st_alarmdb.mstrAlarmCode);
	strTmp[5].Format("%-1s", st_alarmdb.mstrAlarmMsg);

	
	if(Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if(Mysql.Database_Select("AMT830"))
		{
			if(Mysql.Table_Select("AMT830", "ALARM_DATA"))
			{
				Mysql.Record_Add("ALARM_DATA", strTmp);
			}
			else
			{
				if(Mysql.Table_Create("AMT830", "ALARM_DATA", 6, Mysql.mstr_alarm_table_name, Mysql.mn_alarm_table_type, Mysql.mn_alarm_table_num))
				{
					Mysql.Record_Add("ALARM_DATA", strTmp);
				}
			}
		}
		else
		{
			if(Mysql.Database_Create("AMT830"))
			{
				if(Mysql.Table_Create("AMT830", "ALARM_DATA", 6, Mysql.mstr_alarm_table_name, Mysql.mn_alarm_table_type, Mysql.mn_alarm_table_num))
				{
					Mysql.Record_Add("ALARM_DATA", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();
	g_cs.LeaveCriticalSection();
}

void CMainFrame::OnMain_Work_Mysql_Write()
{
	g_cs.EnterCriticalSection();
	CString strTmp[8];
	
	st_lotdb.mstrlot_no = st_handler.str_main_lot;
	st_lotdb.mnTotal_count = st_work.n_total_load_count;
	st_lotdb.mnPass_count = st_work.n_pass_count;
	st_lotdb.mnFail_count = st_work.n_fail_count;
	st_lotdb.mfYield = st_work.n_fyiled;
	
	strTmp[1].Format("%-1s", st_lotdb.mstrlot_no);
	strTmp[2].Format("%04d/%02d/%02d %02d:%02d:%02d", st_lotdb.mstart_date.GetYear(), 
		st_lotdb.mstart_date.GetMonth(),
		st_lotdb.mstart_date.GetDay(),
		st_lotdb.mstart_date.GetHour(),
		st_lotdb.mstart_date.GetMinute(),
		st_lotdb.mstart_date.GetSecond());

	st_lotdb.mend_date = COleDateTime::GetCurrentTime();
	strTmp[3].Format("%04d/%02d/%02d %02d:%02d:%02d", st_lotdb.mend_date.GetYear(), 
		st_lotdb.mend_date.GetMonth(),
		st_lotdb.mend_date.GetDay(),
		st_lotdb.mend_date.GetHour(),
		st_lotdb.mend_date.GetMinute(),
		st_lotdb.mend_date.GetSecond());	
	
	strTmp[4].Format("%d", st_lotdb.mnTotal_count);
	strTmp[5].Format("%d", st_lotdb.mnPass_count);
	strTmp[6].Format("%d", st_lotdb.mnFail_count);
	strTmp[7].Format("%.1f", st_lotdb.mfYield);

	
	if(Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if(Mysql.Database_Select("AMT830"))
		{
			if(Mysql.Table_Select("AMT830", "WORK_DATA"))
			{
				Mysql.Record_Add("WORK_DATA", strTmp);
			}
			else
			{
				if(Mysql.Table_Create("AMT830", "WORK_DATA", 8, Mysql.mstr_lot_table_name, Mysql.mn_lot_table_type, Mysql.mn_lot_table_num))
				{
					Mysql.Record_Add("WORK_DATA", strTmp);
				}
			}
		}
		else
		{
			if(Mysql.Database_Create("AMT830"))
			{
				if(Mysql.Table_Create("AMT830", "WORK_DATA", 8, Mysql.mstr_lot_table_name, Mysql.mn_lot_table_type, Mysql.mn_lot_table_num))
				{
					Mysql.Record_Add("WORK_DATA", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();
	g_cs.LeaveCriticalSection();
}

void CMainFrame::OnMain_BootFlagCheck()
{
	// 2K12/02/23/ViboX
	//////////////////////////////////////////////////////////////////////////
	// PC �� ����� �� �� ���α׷��� ó�� ����� ��� �϶��� ��� ��Ʈ�ѷ��� 
	// ���ڴ� ī���� �� Ŀ�ǵ� ī���Ͱ� ���� ������ ���������� �ϸ� �ȵȴ�.
	// ������ ���� ������ �߰� �Ѵ�.
	// 1. ������ ���� ���α׷��� BootUpFlag.bat ������ �ξ� ������ ����� ��
	//    C:\ �� BootUpFlag.txt ������ ���� �Ѵ�.
	// 2. Dumping �� ���α׷� ���۽� �� ���� ���� ��� �����찡 ��������� �ν�.
	// 3. BootUpFlag.txt ������ ���� ��� ���� �Ѵ�.
	
	/***** BootUpFlag.bat *****************************************/
	//
	// 		@ECHO OFF
	// 		CLS
	// 			
	// 		SET FILENAME=C:\CellDumping\Setting\BootUpFlag.txt	
	// 			
	// 		ECHO BootUpFlag = 1 >> %FILENAME%
	// 			
	// 		ECHO. >> %FILENAME%
	//
	/**************************************************************/
	
	CFile file;
	CString strPath = "C:\\AMT830\\BootUpFlag.txt";
	
	if (!file.Open(strPath,CFile::modeRead))
	{	// ������ ���� ���� �����찡 ������� ���� ���� ���� �̴�.
		st_handler.nBootFlagCheck = CTL_NO;
	}
	else
	{	// ������� �� ��쿩�� ���������� �ϸ� �ȵȴ�.
		
		file.Close();
		file.Remove(strPath);	// ������ ���� �Ѵ�.
		st_handler.nBootFlagCheck = CTL_YES;
	}
}

void CMainFrame::OnMainLotReadyExecute(int nID)
{
	int nResponse;
	CDialog_Message  msg_dlg;
	Func.OnSet_IO_Port_Sound(IO_ON);

	if(nID == 0)
	{
		if(st_work.n_eqp_lotend == CTL_NO)
		{
			st_msg.str_fallacy_msg = _T("Lot Ready ���·� �����մϴ�.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH)
			{
				st_msg.str_fallacy_msg = _T("Lot Ready to return to the state");
			}
			

			nResponse = msg_dlg.DoModal();
		}
		st_handler.n_lotend_ready = 10;
	}
	else
	{
//		st_msg.str_fallacy_msg = _T("Lot ���� �� ����� ������ �ּ���.");
//		nResponse = msg_dlg.DoModal();
		st_handler.n_lotend_ready = 11;
	}
	Func.OnSet_IO_Port_Sound(IO_OFF);  // SOUND OFF �� I/O ��� �Լ� 
}

///////////////////// 20121120
int CMainFrame::OnMain_Rcv_Barcode()
{
	int nRet = RET_PROCEED;
	CString str_tmp;
	
	switch(m_n_rcv_bcr_step)
	{
		case 0:
			if(st_client[NETWORK_CLIENT_7].n_connect == NO)
			{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_7);
				m_n_rcv_bcr_wait[0] = GetTickCount();
				m_n_rcv_bcr_step = 100;
			}
			break;
			
		case 100:
			if(st_client[NETWORK_CLIENT_7].n_connect == YES)
			{
				m_n_rcv_bcr_step = 200;
			}
			else
			{
				m_n_rcv_bcr_wait[1] = GetTickCount();
				m_n_rcv_bcr_wait[2] = m_n_rcv_bcr_wait[1] - m_n_rcv_bcr_wait[0];
				
				if(m_n_rcv_bcr_wait[2] > 2000)
				{
					//nRet = RET_ERROR;
//					st_client[NETWORK_BARCODE_CLIENT].n_client_send_flag = NO;
					
					m_n_rcv_bcr_step = 300;
				}
			}
			break;
		case 200:
			//st_client[NETWORK_SOLDER_CLIENT].str_client_send = Func.Send_Solder(SOLDER_COUNT, "11", 1, "AA");
			//st_data.str_sndBarcode = "LON";LOFF ("LON%c", 13
			//st_data.str_sndBarcode = "LON";
//			st_client[NETWORK_CLIENT_7].str_client_send.Format("%s%c", st_data.str_sndBarcode, 13);
			sprintf(st_client[NETWORK_CLIENT_7].ch_send,"%s%c", st_data.str_sndBarcode, 13);
//			if(st_data.str_sndBarcode == "LON")	st_client[NETWORK_CLIENT_7].n_client_rpy = NO;
//			else                                    st_client[NETWORK_CLIENT_7].n_client_rpy = YES;
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, NETWORK_CLIENT_7);
			m_n_rcv_bcr_wait[0] = GetTickCount();
			m_n_rcv_bcr_step = 300;
			break;
			
		case 300:
			if(st_client[NETWORK_CLIENT_7].n_connect == NO)
			{
				st_client[NETWORK_CLIENT_7].n_client_send_flag = YES;
				nRet = RET_GOOD;
				m_n_rcv_bcr_step = 0;
			}
			else
			{
				m_n_rcv_bcr_wait[1] = GetTickCount();
				m_n_rcv_bcr_wait[2] = m_n_rcv_bcr_wait[1] - m_n_rcv_bcr_wait[0];
				
				if(m_n_rcv_bcr_wait[2] > 5000)
				{
					::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CLOSE,NETWORK_CLIENT_7);
//					st_client[NETWORK_BARCODE_CLIENT].n_client_send_flag = NO;
					nRet = RET_GOOD;
					m_n_rcv_bcr_step = 0;
				}
			}
			break;
	}

	return nRet;	
}
/////////////////////