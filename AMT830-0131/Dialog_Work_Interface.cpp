// Dialog_Work_Interface.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Work_Interface.h"
#include "KeyboardDll.h"	//20120514 유성준
#include "Dialog_Select.h"
#include "Cmmsdk.h"
#include "FastechPublic_IO.h"
#include "Variable.h"			// 전역 변수 정의 클래스 추가

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Interface dialog
#define TM_MANUAL_VISION_TEST				992
//#define TM_MANUAL_BCR_TEST					993
//////////////////////////////////////////////////////// 20120620
#define TM_MANUAL_BCR_TRIGER_TEST			993	
#define TM_MANUAL_BCR_HS_YIELD_TEST			994	
#define TM_MANUAL_BCR_TRIGER_TEST2			995	
#define TM_LEFTHS_BCR_TEST					996
//////////////////////////////////////////////////////// 

CDialog_Work_Interface::CDialog_Work_Interface(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Work_Interface::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Work_Interface)
	//}}AFX_DATA_INIT
}


void CDialog_Work_Interface::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Work_Interface)
	DDX_Control(pDX, IDC_MSG_ETC_SERVER_PORT, m_msg_etc_server_port);
	DDX_Control(pDX, IDC_MSG_SEC_SERVER_PORT, m_msg_sec_server_port);
	DDX_Control(pDX, IDC_BTN_ETC_SERVER_PORT, m_btn_etc_server_port);
	DDX_Control(pDX, IDC_BTN_SEC_SERVER_PORT, m_btn_sec_server_port);
	DDX_Control(pDX, IDC_GROUP_ETC_SERVER, m_group_etc_server);
	DDX_Control(pDX, IDC_GROUP_SEC_SERVER, m_group_sec_server);
	//DDX_Control(pDX, IDC_BTN_MODEL_BCR_TEST, m_btn_bcr_test);
	DDX_Control(pDX, IDC_BTN_MODEL_BCR_TEST, m_btn_bcr_triger_test);
	DDX_Control(pDX, IDC_BTN_MODEL_BCR_TEST2, m_btn_bcr_triger_test2);//20120612
	DDX_Control(pDX, IDC_BTN_MODEL_BCR_TEST3, m_btn_bcr_triger_test3);
	DDX_Control(pDX, IDC_BTN_MODEL_BCR_TEST4, m_btn_bcr_triger_test4);//20120612
	
	/////////////////////////////////////////////////////////////// 20120514 유성준
	DDX_Control(pDX, IDC_GROUP_SERIAL_4, m_group_serial_4);
	DDX_Control(pDX, IDC_MSG_STOP_4, m_msg_stop_4);
	DDX_Control(pDX, IDC_MSG_RATE_4, m_msg_rate_4);
	DDX_Control(pDX, IDC_MSG_PORT_4, m_msg_port_4);
	DDX_Control(pDX, IDC_MSG_PARITY_4, m_msg_parity_4);
	DDX_Control(pDX, IDC_MSG_DATA_4, m_msg_data_4);
	DDX_Control(pDX, IDC_COMBO_STOP_4, m_cb_stop_4);
	DDX_Control(pDX, IDC_COMBO_RATE_4, m_cb_rate_4);
	DDX_Control(pDX, IDC_COMBO_PORT_4, m_cb_port_4);
	DDX_Control(pDX, IDC_COMBO_PARITY_4, m_cb_parity_4);
	DDX_Control(pDX, IDC_COMBO_DATA_4, m_cb_data_4);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_4, m_btn_serial_connect_4);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_4, m_btn_serial_apply_4);
	/////////////////////////////////////////////////////////////// 20120519 유성준
	DDX_Control(pDX, IDC_GROUP_SERIAL_5, m_group_serial_5);
	DDX_Control(pDX, IDC_MSG_STOP_5, m_msg_stop_5);
	DDX_Control(pDX, IDC_MSG_RATE_5, m_msg_rate_5);
	DDX_Control(pDX, IDC_MSG_PORT_5, m_msg_port_5);
	DDX_Control(pDX, IDC_MSG_PARITY_5, m_msg_parity_5);
	DDX_Control(pDX, IDC_MSG_DATA_5, m_msg_data_5);
	DDX_Control(pDX, IDC_COMBO_STOP_5, m_cb_stop_5);
	DDX_Control(pDX, IDC_COMBO_RATE_5, m_cb_rate_5);
	DDX_Control(pDX, IDC_COMBO_PORT_5, m_cb_port_5);
	DDX_Control(pDX, IDC_COMBO_PARITY_5, m_cb_parity_5);
	DDX_Control(pDX, IDC_COMBO_DATA_5, m_cb_data_5);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_5, m_btn_serial_connect_5);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_5, m_btn_serial_apply_5);
	/////////////////////////////////////////////////////////////// 20120610 유성준
	DDX_Control(pDX, IDC_GROUP_SERIAL_6, m_group_serial_6);
	DDX_Control(pDX, IDC_MSG_STOP_6, m_msg_stop_6);
	DDX_Control(pDX, IDC_MSG_RATE_6, m_msg_rate_6);
	DDX_Control(pDX, IDC_MSG_PORT_6, m_msg_port_6);
	DDX_Control(pDX, IDC_MSG_PARITY_6, m_msg_parity_6);
	DDX_Control(pDX, IDC_MSG_DATA_6, m_msg_data_6);
	DDX_Control(pDX, IDC_COMBO_STOP_6, m_cb_stop_6);
	DDX_Control(pDX, IDC_COMBO_RATE_6, m_cb_rate_6);
	DDX_Control(pDX, IDC_COMBO_PORT_6, m_cb_port_6);
	DDX_Control(pDX, IDC_COMBO_PARITY_6, m_cb_parity_6);
	DDX_Control(pDX, IDC_COMBO_DATA_6, m_cb_data_6);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_6, m_btn_serial_connect_6);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_6, m_btn_serial_apply_6);


	///////////////////////////////////////////////////////////////

	DDX_Control(pDX, IDC_GROUP_SERIAL_3, m_group_serial_3);
	DDX_Control(pDX, IDC_MSG_STOP_3, m_msg_stop_3);
	DDX_Control(pDX, IDC_MSG_RATE_3, m_msg_rate_3);
	DDX_Control(pDX, IDC_MSG_PORT_3, m_msg_port_3);
	DDX_Control(pDX, IDC_MSG_PARITY_3, m_msg_parity_3);
	DDX_Control(pDX, IDC_MSG_DATA_3, m_msg_data_3);
	DDX_Control(pDX, IDC_COMBO_STOP_3, m_cb_stop_3);
	DDX_Control(pDX, IDC_COMBO_RATE_3, m_cb_rate_3);
	DDX_Control(pDX, IDC_COMBO_PORT_3, m_cb_port_3);
	DDX_Control(pDX, IDC_COMBO_PARITY_3, m_cb_parity_3);
	DDX_Control(pDX, IDC_COMBO_DATA_3, m_cb_data_3);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_3, m_btn_serial_connect_3);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_3, m_btn_serial_apply_3);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_2, m_btn_serial_connect_2);
	DDX_Control(pDX, IDC_MSG_STOP_2, m_msg_stop_2);
	DDX_Control(pDX, IDC_MSG_RATE_2, m_msg_rate_2);
	DDX_Control(pDX, IDC_MSG_PORT_2, m_msg_port_2);
	DDX_Control(pDX, IDC_MSG_PARITY_2, m_msg_parity_2);
	DDX_Control(pDX, IDC_MSG_DATA_2, m_msg_data_2);
	DDX_Control(pDX, IDC_GROUP_SERIAL_2, m_group_serial_2);
	DDX_Control(pDX, IDC_COMBO_STOP_2, m_cb_stop_2);
	DDX_Control(pDX, IDC_COMBO_RATE_2, m_cb_rate_2);
	DDX_Control(pDX, IDC_COMBO_PORT_2, m_cb_port_2);
	DDX_Control(pDX, IDC_COMBO_PARITY_2, m_cb_parity_2);
	DDX_Control(pDX, IDC_COMBO_DATA_2, m_cb_data_2);
	DDX_Control(pDX, IDC_MSG_CLIENT_IP_2, m_msg_client_ip_2);
	DDX_Control(pDX, IDC_MSG_CLIENT_PORT_2, m_msg_client_port_2);
	DDX_Control(pDX, IDC_BTN_SERIAL_CONNECT_1, m_btn_serial_connect_1);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_2, m_btn_serial_apply_2);
	DDX_Control(pDX, IDC_BTN_SERIAL_APPLY_1, m_btn_serial_apply_1);
	DDX_Control(pDX, IDC_BTN_SERVER_PORT_1, m_btn_server_port_1);

	DDX_Control(pDX, IDC_BTN_CLIENT_PORT_2, m_btn_client_port_2);
	DDX_Control(pDX, IDC_BTN_CLIENT_PORT_1, m_btn_client_port_1);
	DDX_Control(pDX, IDC_BTN_CLIENT_IP_2, m_btn_client_ip_2);
	DDX_Control(pDX, IDC_BTN_CLIENT_IP_1, m_btn_client_ip_1);
	DDX_Control(pDX, IDC_GROUP_SERIAL_1, m_group_serial_1);
	DDX_Control(pDX, IDC_MSG_STOP_1, m_msg_stop_1);
	DDX_Control(pDX, IDC_MSG_SERVER_PORT_1, m_msg_server_port_1);

	DDX_Control(pDX, IDC_MSG_RATE_1, m_msg_rate_1);
	DDX_Control(pDX, IDC_MSG_PORT_1, m_msg_port_1);
	DDX_Control(pDX, IDC_MSG_PARITY_1, m_msg_parity_1);
	DDX_Control(pDX, IDC_MSG_DATA_1, m_msg_data_1);
	DDX_Control(pDX, IDC_MSG_CLIENT_PORT_1, m_msg_client_port_1);
	DDX_Control(pDX, IDC_MSG_CLIENT_IP_1, m_msg_client_ip_1);
	DDX_Control(pDX, IDC_GROUP_SERVER_1, m_group_server_1);
	
	DDX_Control(pDX, IDC_GROUP_SERIAL, m_group_serial);
	DDX_Control(pDX, IDC_GROUP_NETWORK, m_group_network);
	DDX_Control(pDX, IDC_GROUP_CLIENT_2, m_group_client_2);
	DDX_Control(pDX, IDC_GROUP_CLIENT_1, m_group_client_1);
	DDX_Control(pDX, IDC_COMBO_STOP_1, m_cb_stop_1);
	DDX_Control(pDX, IDC_COMBO_RATE_1, m_cb_rate_1);
	DDX_Control(pDX, IDC_COMBO_PORT_1, m_cb_port_1);
	DDX_Control(pDX, IDC_COMBO_PARITY_1, m_cb_parity_1);
	DDX_Control(pDX, IDC_COMBO_DATA_1, m_cb_data_1);
	DDX_Control(pDX, IDC_CLIENT_IP_2, m_client_ip_2);
	DDX_Control(pDX, IDC_CLIENT_IP_1, m_client_ip_1);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btn_apply);

	///////////////////////////////////////////////////////////////// 20120608
	DDX_Control(pDX, IDC_BTN_JOB_CHANGE, m_btn_jobchange);
	DDX_Control(pDX, IDC_BTN_JOB_CHANGE2, m_btn_jobchange2);
	DDX_Control(pDX, IDC_BTN_JOB_CHANGE3, m_btn_jobchange3);
	DDX_Control(pDX, IDC_BTN_JOB_CHANGE4, m_btn_jobchange4);
	DDX_Control(pDX, IDC_BTN_JOB_CHANGE5, m_btn_jobchange5);
	DDX_Control(pDX, IDC_BTN_JOB_CHANGE6, m_btn_jobchange6);
	DDX_Control(pDX, IDC_BTN_JOB_CHANGE7, m_btn_jobchange7);
	DDX_Control(pDX, IDC_BTN_JOB_CHANGE8, m_btn_jobchange8);
	
	DDX_Control(pDX, IDC_GROUP_JOB_CHANGE, m_group_jobchange);
	DDX_Control(pDX, IDC_GROUP_JOB_CHANGE2, m_group_jobchange2);
	
	/////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////// 20120620
	DDX_Control(pDX, IDC_BTN_JOB_CHANGE9, m_btn_jobchange9);
	DDX_Control(pDX, IDC_BTN_JOB_CHANGE10, m_btn_jobchange10);
	DDX_Control(pDX, IDC_LIST_BCR_ML, m_list_bcr_ml);
	DDX_Control(pDX, IDC_LIST_HEATSINK_ML, m_list_hs_ml);
	DDX_Control(pDX, IDC_GROUP_ML, m_group_bcr_ml);
	DDX_Control(pDX, IDC_GROUP_ML2, m_group_hs_ml);
	/////////////////////////////////////////////////////////////////

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialog_Work_Interface, CDialog)
	//{{AFX_MSG_MAP(CDialog_Work_Interface)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_1, OnBtnSerialApply1)
	ON_BN_CLICKED(IDC_BTN_CLIENT_IP_1, OnBtnClientIp1)
	ON_BN_CLICKED(IDC_BTN_CLIENT_PORT_1, OnBtnClientPort1)
	ON_BN_CLICKED(IDC_BTN_CLIENT_IP_2, OnBtnClientIp2)
	ON_BN_CLICKED(IDC_BTN_CLIENT_PORT_2, OnBtnClientPort2)
	ON_BN_CLICKED(IDC_BTN_SERVER_PORT_1, OnBtnServerPort1)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_1, OnBtnSerialConnect1)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_2, OnBtnSerialApply2)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_2, OnBtnSerialConnect2)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_3, OnBtnSerialApply3)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_3, OnBtnSerialConnect3)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_4, OnBtnSerialApply4)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_4, OnBtnSerialConnect4)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_5, OnBtnSerialApply5)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_5, OnBtnSerialConnect5)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BTN_MODEL_BCR_TEST, OnBtnModelBcrTest)
	ON_BN_CLICKED(IDC_BTN_SEC_SERVER_PORT, OnBtnSecServerPort)
	ON_BN_CLICKED(IDC_BTN_ETC_SERVER_PORT, OnBtnEtcServerPort)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BTN_JOB_CHANGE, OnBtnJobChange)
	ON_BN_CLICKED(IDC_BTN_JOB_CHANGE2, OnBtnJobChange2)
	ON_BN_CLICKED(IDC_BTN_JOB_CHANGE3, OnBtnJobChange3)
	ON_BN_CLICKED(IDC_BTN_JOB_CHANGE4, OnBtnJobChange4)
	ON_BN_CLICKED(IDC_BTN_JOB_CHANGE5, OnBtnJobChange5)
	ON_BN_CLICKED(IDC_BTN_JOB_CHANGE6, OnBtnJobChange6)
	ON_BN_CLICKED(IDC_BTN_JOB_CHANGE7, OnBtnJobChange7)
	ON_BN_CLICKED(IDC_BTN_JOB_CHANGE8, OnBtnJobChange8)
	ON_BN_CLICKED(IDC_BTN_SERIAL_APPLY_6, OnBtnSerialApply6)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CONNECT_6, OnBtnSerialConnect6)
	ON_BN_CLICKED(IDC_BTN_MODEL_BCR_TEST2, OnBtnModelBcrTest2)
	ON_BN_CLICKED(IDC_BTN_JOB_CHANGE9, OnBtnJobChange9)
	ON_BN_CLICKED(IDC_BTN_JOB_CHANGE10, OnBtnJobChange10)
	ON_LBN_SELCHANGE(IDC_LIST_BCR_ML, OnSelchangeListBcrMl)
	ON_LBN_SELCHANGE(IDC_LIST_HEATSINK_ML, OnSelchangeListHeatsinkMl)
	ON_BN_CLICKED(IDC_BTN_MODEL_BCR_TEST3, OnBtnModelBcrTest3)
	ON_BN_CLICKED(IDC_BTN_MODEL_BCR_TEST4, OnBtnModelBcrTest4)
	ON_BN_CLICKED(IDC_BTN_BCR_READ, OnBtnBcrRead)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCell_Left_Click)
 	ON_MESSAGE(WM_WORK_END, OnWork_Interface_Info_Display)  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 //20120608

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Interface message handlers


BOOL CDialog_Work_Interface::OnInitDialog() 
{
	CDialog::OnInitDialog();

	st_handler.cwnd_interface = this;		// 20120608	
	// TODO: Add extra initialization here
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	Data_Init();
	
	Init_Group();
	Init_Button();
	Init_Label();
	Init_Editbox();

	OnInterface_ML_BCR_Init_List(st_path.mstr_interface_folder);
	OnInterface_ML_BCR_Focus_Set();

	OnInterface_ML_HS_Init_List(st_path.mstr_interface_folder);
	OnInterface_ML_HS_Focus_Set();


	OnInterface_Display();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Work_Interface::OnTimer(UINT nIDEvent) 
{
	int nRet;
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TM_MANUAL_VISION_TEST)
	{
	}
	/////////////////////////////////////////////////// 20120620
	else if(nIDEvent == TM_MANUAL_BCR_TRIGER_TEST)
	{
		nRet = Manual_Bcr_Triger_Test();

		if(nRet == RET_GOOD)
		{
			m_bcr_triger_step = 0;
			KillTimer(TM_MANUAL_BCR_TRIGER_TEST) ;  // 타이머 삭제 //
			OnBtn_Enable_True();
		}
		else if(nRet == RET_ERROR)
		{
			m_bcr_triger_step = 0;
			KillTimer(TM_MANUAL_BCR_TRIGER_TEST) ;  // 타이머 삭제 //
			OnBtn_Enable_True();
		}
	}
	else if(nIDEvent == TM_MANUAL_BCR_TRIGER_TEST2)
	{
		nRet = Manual_Bcr_Triger_Test2();

		if(nRet == RET_GOOD)
		{
			m_bcr_triger_step2 = 0;
			KillTimer(TM_MANUAL_BCR_TRIGER_TEST2) ;  // 타이머 삭제 //
			OnBtn_Enable_True();
		}
		else if(nRet == RET_ERROR)
		{
			m_bcr_triger_step2 = 0;
			KillTimer(TM_MANUAL_BCR_TRIGER_TEST2) ;  // 타이머 삭제 //
			OnBtn_Enable_True();
		}
	}
	else if(nIDEvent == TM_MANUAL_BCR_HS_YIELD_TEST)
	{
		nRet = Manual_Bcr_HS_Yield_Test();

		if(nRet == RET_GOOD)
		{
			mn_vision_hs_enable = 0;
			OnBtn_BCR_HS_YIELD_Enable_True();
			KillTimer(TM_MANUAL_BCR_HS_YIELD_TEST);  // 타이머 삭제 //
			m_b_stop_req = TRUE;
		}
		else if(nRet == RET_ERROR)
		{
			mn_vision_hs_enable = 0;
			KillTimer(TM_MANUAL_BCR_HS_YIELD_TEST) ;  // 타이머 삭제 //
			OnBtn_BCR_HS_YIELD_Enable_True();
		}	
	}
	else if(nIDEvent == TM_LEFTHS_BCR_TEST)
	{
		nRet = Func.Robot_BarcodeScanning(MODULE_LOAD_PORT);
		if(nRet == RET_GOOD)
		{
			KillTimer(TM_LEFTHS_BCR_TEST);
		}
		else if(nRet == RET_ERROR)
		{
			KillTimer(TM_LEFTHS_BCR_TEST);
		}
	}
	///////////////////////////////////////////////////

	CDialog::OnTimer(nIDEvent);
}

void CDialog_Work_Interface::OnDestroy() 
{
	CDialog::OnDestroy();

	st_handler.cwnd_interface = NULL;		// 20120608		
	// TODO: Add your message handler code here
	if(Data_Comparison() == CTLBD_RET_ERROR)
	{
		Data_Apply();
	}

	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}
}



void CDialog_Work_Interface::Data_Apply()
{
	int i;

	for(i = 0; i < 10; i++)
	{
		st_client[i].str_ip = m_str_client_ip[i][1];
		st_client[i].n_port = m_n_client_port[i][1];
		
		st_server[i].n_port = m_n_server_port[i][1];

		rs_232.n_serial_port[i]		= ConverterToData(COM_PORT, m_n_port[i][1]);
		rs_232.n_serial_data[i]		= ConverterToData(COM_DATA, m_n_data[i][1]);
		rs_232.n_serial_stop[i]		= ConverterToData(COM_STOP, m_n_stop[i][1]);
		rs_232.n_serial_baudrate[i]	= ConverterToData(COM_BAUDRATE, m_n_rate[i][1]);
		rs_232.n_serial_parity[i]	= ConverterToData(COM_PARITY, m_n_parity[i][1]);
	}

	m_list_bcr_ml.ResetContent();			// 리스트 박스의 모든 목록 삭제
	m_list_hs_ml.ResetContent();			// 리스트 박스의 모든 목록 삭제

	OnInterface_ML_BCR_Init_List(st_path.mstr_interface_folder);
	OnInterface_ML_BCR_Focus_Set();

	OnInterface_ML_HS_Init_List(st_path.mstr_interface_folder);
	OnInterface_ML_HS_Focus_Set();

	/////////////////////////// 20120620
	st_interface.n_bcr_ml_mode =			mn_bcr_ml_mode[1];				// With, With Out, No Tray
	mn_bcr_ml_mode[0] =                     mn_bcr_ml_mode[1];
	st_interface.str_bcr_ml_name =			str_bcr_ml_name[1];					// 선택된 디바이스 종류 저장 변수
	str_bcr_ml_name[0] =                    str_bcr_ml_name[1]; 
	st_interface.n_hs_ml_mode =				mn_hs_ml_mode[1];				// With, With Out, No Tray
	mn_hs_ml_mode[0] =                      mn_hs_ml_mode[1];
	st_interface.str_hs_ml_name =			str_hs_ml_name[1];					// 선택된 디바이스 종류 저장 변수
	str_hs_ml_name[0] =                     str_hs_ml_name[1];
	/////////////////////////// 

}

void CDialog_Work_Interface::Data_Backup()
{
	int i;
	
	for(i = 0; i < 10; i++)
	{
		m_str_client_ip[i][0]			= m_str_client_ip[i][1];
		m_n_client_port[i][0]			= m_n_client_port[i][1];

		m_n_server_port[i][0]			= m_n_server_port[i][1];

		m_n_port[i][0]					= m_n_port[i][1];
		m_n_rate[i][0]					= m_n_rate[i][1];
		m_n_data[i][0]					= m_n_data[i][1];
		m_n_parity[i][0]				= m_n_parity[i][1];
		m_n_stop[i][0]					= m_n_stop[i][1];
	}
	/////////////////////////// 20120620
	mn_bcr_ml_mode[0] =				mn_bcr_ml_mode[1];				// With, With Out, No Tray
	str_bcr_ml_name[0] =			str_bcr_ml_name[1];					// 선택된 디바이스 종류 저장 변수
	mn_hs_ml_mode[0] =				mn_hs_ml_mode[1];				// With, With Out, No Tray
	str_hs_ml_name[0] =			    str_hs_ml_name[1];					// 선택된 디바이스 종류 저장 변수
	/////////////////////////// 

}

int CDialog_Work_Interface::Data_Comparison()
{
	int i;
	int Ret = CTLBD_RET_GOOD;

	for(i = 0; i < 10; i++)
	{
		if(m_str_client_ip[i][0]		!=	m_str_client_ip[i][1])		Ret = CTLBD_RET_ERROR;
		if(m_n_client_port[i][0]		!=	m_n_client_port[i][1])		Ret = CTLBD_RET_ERROR;
		
		if(m_n_server_port[i][0]		!=	m_n_server_port[i][1])		Ret = CTLBD_RET_ERROR;

		if(m_n_port[i][0]				!= m_n_port[i][1])				Ret = CTLBD_RET_ERROR;
		if(m_n_rate[i][0]				!= m_n_rate[i][1])				Ret = CTLBD_RET_ERROR;
		if(m_n_data[i][0]				!= m_n_data[i][1])				Ret = CTLBD_RET_ERROR;
		if(m_n_parity[i][0]				!= m_n_parity[i][1])			Ret = CTLBD_RET_ERROR;
		if(m_n_stop[i][0]				!= m_n_stop[i][1])				Ret = CTLBD_RET_ERROR;
	}
	
	return Ret;
}

void CDialog_Work_Interface::Data_HistoryLog()
{
	int i;
	CString str_msg;

	for(i = 0; i < 10; i++)
	{
		if(m_str_client_ip[i][1]		!=	m_str_client_ip[i][1])
		{
			str_msg.Format( "[Interface] [%02d]Client IP Change %s -> %s", i, m_str_client_ip[i][0], m_str_client_ip[i][1]);
			Func.On_LogFile_Add(0, str_msg);
		}
		
		if(m_n_client_port[i][1]		!=	m_n_client_port[i][1])
		{
			str_msg.Format( "[Interface] [%02d] Client Port Change %d -> %d", i, m_n_client_port[i][0], m_n_client_port[i][1]);
			Func.On_LogFile_Add(0, str_msg);
		}

		if(m_n_server_port[i][1]		!=	m_n_server_port[i][1])
		{
			str_msg.Format( "[Interface] [%02d]Server Port Change %d -> %d", i, m_n_server_port[i][0], m_n_server_port[i][1]);
			Func.On_LogFile_Add(0, str_msg);
		}

		if(m_n_port[i][0]				!= m_n_port[i][1])
		{
			str_msg.Format( "[Interface] [%02d]Serial Port Change %d -> %d", i, m_n_port[i][0], m_n_port[i][1]);
			Func.On_LogFile_Add(0, str_msg);
		}
		if(m_n_rate[i][0]				!= m_n_rate[i][1])
		{
			str_msg.Format( "[Interface] [%02d]Serial BaudRate Change %d -> %d", i, m_n_rate[i][0], m_n_rate[i][1]);
			Func.On_LogFile_Add(0, str_msg);
		}
		if(m_n_data[i][0]				!= m_n_data[i][1])
		{
			str_msg.Format( "[Interface] [%02d]Serial Data Change %d -> %d", i, m_n_data[i][0], m_n_data[i][1]);
			Func.On_LogFile_Add(0, str_msg);
		}
		if(m_n_parity[i][0]				!= m_n_parity[i][1])
		{
			str_msg.Format( "[Interface] [%02d]Serial Parity Change %d -> %d", i, m_n_parity[i][0], m_n_parity[i][1]);
			Func.On_LogFile_Add(0, str_msg);
		}
		if(m_n_stop[i][0]				!= m_n_stop[i][1])
		{
			str_msg.Format( "[Interface] [%02d]Serial Stop Change %d -> %d", i, m_n_stop[i][0], m_n_stop[i][1]);
			Func.On_LogFile_Add(0, str_msg);
		}
	}
}

void CDialog_Work_Interface::Data_Recovery()
{
	int i;
	
	for(i = 0; i < 10; i++)
	{
		m_str_client_ip[i][1]			= m_str_client_ip[i][0];
		m_n_client_port[i][1]			= m_n_client_port[i][0];
		
		m_n_server_port[i][1]			= m_n_server_port[i][0];

		m_n_port[i][1]					= m_n_port[i][0];
		m_n_rate[i][1]					= m_n_rate[i][0];
		m_n_data[i][1]					= m_n_data[i][0];
		m_n_parity[i][1]				= m_n_parity[i][0];
		m_n_stop[i][1]					= m_n_stop[i][0];
	}
	/////////////////////////// 20120620
	mn_bcr_ml_mode[1] =				mn_bcr_ml_mode[0];				// With, With Out, No Tray
	str_bcr_ml_name[1] =			str_bcr_ml_name[0];					// 선택된 디바이스 종류 저장 변수
	mn_hs_ml_mode[1] =				mn_hs_ml_mode[0];				// With, With Out, No Tray
	str_hs_ml_name[1] =			    str_hs_ml_name[0];					// 선택된 디바이스 종류 저장 변수
	/////////////////////////// 
	
}

void CDialog_Work_Interface::Data_Init()
{
	int i;
 	int pos;

	for(i = 0; i < 10; i++)
	{
		m_str_client_ip[i][1] = st_client[i].str_ip;
		m_n_client_port[i][1] = st_client[i].n_port;

		m_n_server_port[i][1] = st_server[i].n_port;

		pos = rs_232.n_serial_port[i];
		m_n_port[i][1] = ConverterToPos(COM_PORT, pos);
		
		pos = rs_232.n_serial_baudrate[i];
		m_n_rate[i][1] = ConverterToPos(COM_BAUDRATE, pos);
		
		pos = rs_232.n_serial_data[i];
		m_n_data[i][1] = ConverterToPos(COM_DATA, pos);
		
		pos = rs_232.n_serial_parity[i];
		m_n_parity[i][1] = ConverterToPos(COM_PARITY, pos);
		
		pos = rs_232.n_serial_stop[i];
		m_n_stop[i][1] = ConverterToPos(COM_STOP, pos);
	}
	
	Data_Backup();
}

void CDialog_Work_Interface::OnCell_Left_Click(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CString str_tmp, str_data;
	
	if(lpcc->Row == 0 || lpcc->Col == 1) return;
	
	if(wParam == IDC_CUSTOM_CLIENT)
	{
		m_grid_client = (TSpread*)GetDlgItem(IDC_CUSTOM_CLIENT);
	
		switch(lpcc->Col)
		{
			case 2:
				m_str_client_ip[lpcc->Row-1][1] = GetEditString(m_str_client_ip[lpcc->Row-1][1]);
				m_p_grid.GridCellText(m_grid_client, lpcc->Row, lpcc->Col, m_str_client_ip[lpcc->Row-1][1]);
				break;

			case 3:
				str_tmp.Format("%d", m_n_client_port[lpcc->Row-1][1]);
				str_tmp = GetEditString(str_tmp);
				m_n_client_port[lpcc->Row-1][1] = atoi(str_tmp);
				m_p_grid.GridCellText(m_grid_client, lpcc->Row, lpcc->Col, str_tmp);
				break;
		}
	}
}

void CDialog_Work_Interface::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_network.SetFont(main_font);
	m_group_network.SetCatptionTextColor(BLUE_C);
	m_group_network.SetBorderColor(OK_BC);
	m_group_network.SetFontBold(TRUE);
	m_group_network.SetWindowText("TCP/IP Information");
	
	m_group_client_1.SetFont(main_font);
	m_group_client_1.SetCatptionTextColor(BLUE_C);
	m_group_client_1.SetBorderColor(OK_BC);
	m_group_client_1.SetFontBold(TRUE);
	m_group_client_1.SetWindowText("SEC Client");
	
	m_group_client_2.SetFont(main_font);
	m_group_client_2.SetCatptionTextColor(BLUE_C);
	m_group_client_2.SetBorderColor(OK_BC);
	m_group_client_2.SetFontBold(TRUE);
	m_group_client_2.SetWindowText("SEC Client 2");
	

	m_group_sec_server.SetFont(main_font);
	m_group_sec_server.SetCatptionTextColor(BLUE_C);
	m_group_sec_server.SetBorderColor(OK_BC);
	m_group_sec_server.SetFontBold(TRUE);
	m_group_sec_server.SetWindowText("SEC Server");
	
	m_group_server_1.SetFont(main_font);
	m_group_server_1.SetCatptionTextColor(BLUE_C);
	m_group_server_1.SetBorderColor(OK_BC);
	m_group_server_1.SetFontBold(TRUE);
	m_group_server_1.SetWindowText("SEC Server 2");

	m_group_etc_server.SetFont(main_font);
	m_group_etc_server.SetCatptionTextColor(BLUE_C);
	m_group_etc_server.SetBorderColor(OK_BC);
	m_group_etc_server.SetFontBold(TRUE);
	m_group_etc_server.SetWindowText("Etc Server");
	
	m_group_serial.SetFont(main_font);
	m_group_serial.SetCatptionTextColor(BLUE_C);
	m_group_serial.SetBorderColor(OK_BC);
	m_group_serial.SetFontBold(TRUE);
	m_group_serial.SetWindowText("Serial Information");
	
	m_group_serial_1.SetFont(main_font);
	m_group_serial_1.SetCatptionTextColor(BLUE_C);
	m_group_serial_1.SetBorderColor(OK_BC);
	m_group_serial_1.SetFontBold(TRUE);
	m_group_serial_1.SetWindowText("HS Left");
	
	m_group_serial_2.SetFont(main_font);
	m_group_serial_2.SetCatptionTextColor(BLUE_C);
	m_group_serial_2.SetBorderColor(OK_BC);
	m_group_serial_2.SetFontBold(TRUE);
	m_group_serial_2.SetWindowText("HS Right");
	
	m_group_serial_3.SetFont(main_font);
	m_group_serial_3.SetCatptionTextColor(BLUE_C);
	m_group_serial_3.SetBorderColor(OK_BC);
	m_group_serial_3.SetFontBold(TRUE);
	m_group_serial_3.SetWindowText("Clip Left");
	
	/////////////////////////////////////////////////// 20120514 유성준
	m_group_serial_4.SetFont(main_font);
	m_group_serial_4.SetCatptionTextColor(BLUE_C);
	m_group_serial_4.SetBorderColor(OK_BC);
	m_group_serial_4.SetFontBold(TRUE);
	m_group_serial_4.SetWindowText("Clip Right");
	/////////////////////////////////////////////////// 20120519 유성준
	m_group_serial_5.SetFont(main_font);
	m_group_serial_5.SetCatptionTextColor(BLUE_C);
	m_group_serial_5.SetBorderColor(OK_BC);
	m_group_serial_5.SetFontBold(TRUE);
	m_group_serial_5.SetWindowText("Job Change");
	/////////////////////////////////////////////////// 20120610 유성준
	m_group_serial_6.SetFont(main_font);
	m_group_serial_6.SetCatptionTextColor(BLUE_C);
	m_group_serial_6.SetBorderColor(OK_BC);
	m_group_serial_6.SetFontBold(TRUE);
	m_group_serial_6.SetWindowText("Label Printer");

	/////////////////////////////////////////////////// 20120608 유성준
	m_group_jobchange.SetFont(main_font);
	m_group_jobchange.SetCatptionTextColor(BLUE_C);
	m_group_jobchange.SetBorderColor(OK_BC);
	m_group_jobchange.SetFontBold(TRUE);
	m_group_jobchange.SetWindowText("BCR Yield");

	m_group_jobchange2.SetFont(main_font);
	m_group_jobchange2.SetCatptionTextColor(BLUE_C);
	m_group_jobchange2.SetBorderColor(OK_BC);
	m_group_jobchange2.SetFontBold(TRUE);
	m_group_jobchange2.SetWindowText("HEAT SINK Yield");

	/////////////////////////////////////////////////// 
	/////////////////////////////////////////////////// 20120620
	m_group_bcr_ml.SetFont(main_font);
	m_group_bcr_ml.SetCatptionTextColor(BLUE_C);
	m_group_bcr_ml.SetBorderColor(OK_BC);
	m_group_bcr_ml.SetFontBold(TRUE);
	m_group_bcr_ml.SetWindowText("BCR ML");

	m_group_hs_ml.SetFont(main_font);
	m_group_hs_ml.SetCatptionTextColor(BLUE_C);
	m_group_hs_ml.SetBorderColor(OK_BC);
	m_group_hs_ml.SetFontBold(TRUE);
	m_group_hs_ml.SetWindowText("HEATSINK ML");

	/////////////////////////////////////////////////// 

}

void CDialog_Work_Interface::Init_Button()
{
	short	shBtnColor = 30;

	m_btn_apply.SetFont(m_p_font);
 	m_btn_apply.SetIcon(IDI_MOTOR_APPLY);
	m_btn_apply.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_apply.SetButtonColor(1);
	m_btn_apply.SetTooltipText("변경된 데이터 적용.");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_btn_apply.SetTooltipText("Apply the changed data.");
	}
}

void CDialog_Work_Interface::OnBtnApply() 
{
	// TODO: Add your control notification handler code here
	CDialog_Select select_dlg;
	int n_response;
	
//	if(Data_Comparison() == CTLBD_RET_ERROR)
//	{
		st_other.str_confirm_msg = "변경 된 데이터를 적용하시겠습니까?...";
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_other.str_confirm_msg = "Do you want to apply the data changes?...";
		}
		
		n_response = select_dlg.DoModal();
		
		if (n_response == IDOK)					// 자재가 없다.
		{
			Data_Apply();
			/////////////////////////////////////// 20120523
			mcls_work_interface.OnInterface_Data_Save();
			mcls_work_interface.OnInterface_Data_Load();
			///////////////////////////////////////
		}
//	}
}

///////////////////////////////////////////// 20120620
void CDialog_Work_Interface::Data_Set()
{
	mn_bcr_ml_mode[1] =				st_interface.n_bcr_ml_mode;				// With, With Out, No Tray
	str_bcr_ml_name[1] =			st_interface.str_bcr_ml_name;					// 선택된 디바이스 종류 저장 변수
}
/////////////////////////////////////////////


void CDialog_Work_Interface::Init_Label()
{
	m_msg_port_1.SetFont(m_p_font);
	m_msg_port_1.SetWindowText(_T("Port"));
	m_msg_port_1.SetCenterText();
	m_msg_port_1.SetColor(WHITE_C);
	m_msg_port_1.SetGradientColor(ORANGE_C);
	m_msg_port_1.SetTextColor(BLACK_C);

	m_msg_port_2.SetFont(m_p_font);
	m_msg_port_2.SetWindowText(_T("Port"));
	m_msg_port_2.SetCenterText();
	m_msg_port_2.SetColor(WHITE_C);
	m_msg_port_2.SetGradientColor(ORANGE_C);
	m_msg_port_2.SetTextColor(BLACK_C);

	m_msg_port_3.SetFont(m_p_font);
	m_msg_port_3.SetWindowText(_T("Port"));
	m_msg_port_3.SetCenterText();
	m_msg_port_3.SetColor(WHITE_C);
	m_msg_port_3.SetGradientColor(ORANGE_C);
	m_msg_port_3.SetTextColor(BLACK_C);

	///////////////////////////////////////////// 20120514 유성준
	m_msg_port_4.SetFont(m_p_font);
	m_msg_port_4.SetWindowText(_T("Port"));
	m_msg_port_4.SetCenterText();
	m_msg_port_4.SetColor(WHITE_C);
	m_msg_port_4.SetGradientColor(ORANGE_C);
	m_msg_port_4.SetTextColor(BLACK_C);
	///////////////////////////////////////////// 20120519 유성준
	m_msg_port_5.SetFont(m_p_font);
	m_msg_port_5.SetWindowText(_T("Port"));
	m_msg_port_5.SetCenterText();
	m_msg_port_5.SetColor(WHITE_C);
	m_msg_port_5.SetGradientColor(ORANGE_C);
	m_msg_port_5.SetTextColor(BLACK_C);
	///////////////////////////////////////////// 20120610 유성준
	m_msg_port_6.SetFont(m_p_font);
	m_msg_port_6.SetWindowText(_T("Port"));
	m_msg_port_6.SetCenterText();
	m_msg_port_6.SetColor(WHITE_C);
	m_msg_port_6.SetGradientColor(ORANGE_C);
	m_msg_port_6.SetTextColor(BLACK_C);

	/////////////////////////////////////////////

	m_msg_data_1.SetFont(m_p_font);
	m_msg_data_1.SetWindowText(_T("Data"));
	m_msg_data_1.SetCenterText();
	m_msg_data_1.SetColor(WHITE_C);
	m_msg_data_1.SetGradientColor(ORANGE_C);
	m_msg_data_1.SetTextColor(BLACK_C);

	m_msg_data_2.SetFont(m_p_font);
	m_msg_data_2.SetWindowText(_T("Data"));
	m_msg_data_2.SetCenterText();
	m_msg_data_2.SetColor(WHITE_C);
	m_msg_data_2.SetGradientColor(ORANGE_C);
	m_msg_data_2.SetTextColor(BLACK_C);

	m_msg_data_3.SetFont(m_p_font);
	m_msg_data_3.SetWindowText(_T("Data"));
	m_msg_data_3.SetCenterText();
	m_msg_data_3.SetColor(WHITE_C);
	m_msg_data_3.SetGradientColor(ORANGE_C);
	m_msg_data_3.SetTextColor(BLACK_C);

	///////////////////////////////////////////// 20120514 유성준
	m_msg_data_4.SetFont(m_p_font);
	m_msg_data_4.SetWindowText(_T("Data"));
	m_msg_data_4.SetCenterText();
	m_msg_data_4.SetColor(WHITE_C);
	m_msg_data_4.SetGradientColor(ORANGE_C);
	m_msg_data_4.SetTextColor(BLACK_C);
	///////////////////////////////////////////// 20120519 유성준
	m_msg_data_5.SetFont(m_p_font);
	m_msg_data_5.SetWindowText(_T("Data"));
	m_msg_data_5.SetCenterText();
	m_msg_data_5.SetColor(WHITE_C);
	m_msg_data_5.SetGradientColor(ORANGE_C);
	m_msg_data_5.SetTextColor(BLACK_C);
	///////////////////////////////////////////// 20120610 유성준
	m_msg_data_6.SetFont(m_p_font);
	m_msg_data_6.SetWindowText(_T("Data"));
	m_msg_data_6.SetCenterText();
	m_msg_data_6.SetColor(WHITE_C);
	m_msg_data_6.SetGradientColor(ORANGE_C);
	m_msg_data_6.SetTextColor(BLACK_C);

	/////////////////////////////////////////////


	m_msg_parity_1.SetFont(m_p_font);
	m_msg_parity_1.SetWindowText(_T("Parity"));
	m_msg_parity_1.SetCenterText();
	m_msg_parity_1.SetColor(WHITE_C);
	m_msg_parity_1.SetGradientColor(ORANGE_C);
	m_msg_parity_1.SetTextColor(BLACK_C);

	m_msg_parity_2.SetFont(m_p_font);
	m_msg_parity_2.SetWindowText(_T("Parity"));
	m_msg_parity_2.SetCenterText();
	m_msg_parity_2.SetColor(WHITE_C);
	m_msg_parity_2.SetGradientColor(ORANGE_C);
	m_msg_parity_2.SetTextColor(BLACK_C);

	m_msg_parity_3.SetFont(m_p_font);
	m_msg_parity_3.SetWindowText(_T("Parity"));
	m_msg_parity_3.SetCenterText();
	m_msg_parity_3.SetColor(WHITE_C);
	m_msg_parity_3.SetGradientColor(ORANGE_C);
	m_msg_parity_3.SetTextColor(BLACK_C);

	///////////////////////////////////////////// 20120514 유성준
	m_msg_parity_4.SetFont(m_p_font);
	m_msg_parity_4.SetWindowText(_T("Parity"));
	m_msg_parity_4.SetCenterText();
	m_msg_parity_4.SetColor(WHITE_C);
	m_msg_parity_4.SetGradientColor(ORANGE_C);
	m_msg_parity_4.SetTextColor(BLACK_C);
	///////////////////////////////////////////// 20120519 유성준
	m_msg_parity_5.SetFont(m_p_font);
	m_msg_parity_5.SetWindowText(_T("Parity"));
	m_msg_parity_5.SetCenterText();
	m_msg_parity_5.SetColor(WHITE_C);
	m_msg_parity_5.SetGradientColor(ORANGE_C);
	m_msg_parity_5.SetTextColor(BLACK_C);
	///////////////////////////////////////////// 20120610 유성준
	m_msg_parity_6.SetFont(m_p_font);
	m_msg_parity_6.SetWindowText(_T("Parity"));
	m_msg_parity_6.SetCenterText();
	m_msg_parity_6.SetColor(WHITE_C);
	m_msg_parity_6.SetGradientColor(ORANGE_C);
	m_msg_parity_6.SetTextColor(BLACK_C);

	/////////////////////////////////////////////


	m_msg_rate_1.SetFont(m_p_font);
	m_msg_rate_1.SetWindowText(_T("BaudRate"));
	m_msg_rate_1.SetCenterText();
	m_msg_rate_1.SetColor(WHITE_C);
	m_msg_rate_1.SetGradientColor(ORANGE_C);
	m_msg_rate_1.SetTextColor(BLACK_C);

	m_msg_rate_2.SetFont(m_p_font);
	m_msg_rate_2.SetWindowText(_T("BaudRate"));
	m_msg_rate_2.SetCenterText();
	m_msg_rate_2.SetColor(WHITE_C);
	m_msg_rate_2.SetGradientColor(ORANGE_C);
	m_msg_rate_2.SetTextColor(BLACK_C);

	m_msg_rate_3.SetFont(m_p_font);
	m_msg_rate_3.SetWindowText(_T("BaudRate"));
	m_msg_rate_3.SetCenterText();
	m_msg_rate_3.SetColor(WHITE_C);
	m_msg_rate_3.SetGradientColor(ORANGE_C);
	m_msg_rate_3.SetTextColor(BLACK_C);

	///////////////////////////////////////////// 20120514 유성준
	m_msg_rate_4.SetFont(m_p_font);
	m_msg_rate_4.SetWindowText(_T("BaudRate"));
	m_msg_rate_4.SetCenterText();
	m_msg_rate_4.SetColor(WHITE_C);
	m_msg_rate_4.SetGradientColor(ORANGE_C);
	m_msg_rate_4.SetTextColor(BLACK_C);
	///////////////////////////////////////////// 20120519 유성준
	m_msg_rate_5.SetFont(m_p_font);
	m_msg_rate_5.SetWindowText(_T("BaudRate"));
	m_msg_rate_5.SetCenterText();
	m_msg_rate_5.SetColor(WHITE_C);
	m_msg_rate_5.SetGradientColor(ORANGE_C);
	m_msg_rate_5.SetTextColor(BLACK_C);
	///////////////////////////////////////////// 20120610 유성준
	m_msg_rate_6.SetFont(m_p_font);
	m_msg_rate_6.SetWindowText(_T("BaudRate"));
	m_msg_rate_6.SetCenterText();
	m_msg_rate_6.SetColor(WHITE_C);
	m_msg_rate_6.SetGradientColor(ORANGE_C);
	m_msg_rate_6.SetTextColor(BLACK_C);

	/////////////////////////////////////////////


	m_msg_stop_1.SetFont(m_p_font);
	m_msg_stop_1.SetWindowText(_T("Stop"));
	m_msg_stop_1.SetCenterText();
	m_msg_stop_1.SetColor(WHITE_C);
	m_msg_stop_1.SetGradientColor(ORANGE_C);
	m_msg_stop_1.SetTextColor(BLACK_C);

	m_msg_stop_2.SetFont(m_p_font);
	m_msg_stop_2.SetWindowText(_T("Stop"));
	m_msg_stop_2.SetCenterText();
	m_msg_stop_2.SetColor(WHITE_C);
	m_msg_stop_2.SetGradientColor(ORANGE_C);
	m_msg_stop_2.SetTextColor(BLACK_C);

	m_msg_stop_3.SetFont(m_p_font);
	m_msg_stop_3.SetWindowText(_T("Stop"));
	m_msg_stop_3.SetCenterText();
	m_msg_stop_3.SetColor(WHITE_C);
	m_msg_stop_3.SetGradientColor(ORANGE_C);
	m_msg_stop_3.SetTextColor(BLACK_C);

	///////////////////////////////////////////// 20120514 유성준
	m_msg_stop_4.SetFont(m_p_font);
	m_msg_stop_4.SetWindowText(_T("Stop"));
	m_msg_stop_4.SetCenterText();
	m_msg_stop_4.SetColor(WHITE_C);
	m_msg_stop_4.SetGradientColor(ORANGE_C);
	m_msg_stop_4.SetTextColor(BLACK_C);
	///////////////////////////////////////////// 20120519 유성준
	m_msg_stop_5.SetFont(m_p_font);
	m_msg_stop_5.SetWindowText(_T("Stop"));
	m_msg_stop_5.SetCenterText();
	m_msg_stop_5.SetColor(WHITE_C);
	m_msg_stop_5.SetGradientColor(ORANGE_C);
	m_msg_stop_5.SetTextColor(BLACK_C);
	///////////////////////////////////////////// 20120610 유성준
	m_msg_stop_6.SetFont(m_p_font);
	m_msg_stop_6.SetWindowText(_T("Stop"));
	m_msg_stop_6.SetCenterText();
	m_msg_stop_6.SetColor(WHITE_C);
	m_msg_stop_6.SetGradientColor(ORANGE_C);
	m_msg_stop_6.SetTextColor(BLACK_C);

	/////////////////////////////////////////////


	m_msg_client_ip_1.SetFont(m_p_font);
	m_msg_client_ip_1.SetWindowText(_T("IP Address"));
	m_msg_client_ip_1.SetCenterText();
	m_msg_client_ip_1.SetColor(WHITE_C);
	m_msg_client_ip_1.SetGradientColor(ORANGE_C);
	m_msg_client_ip_1.SetTextColor(BLACK_C);

	m_msg_client_port_1.SetFont(m_p_font);
	m_msg_client_port_1.SetWindowText(_T("Port"));
	m_msg_client_port_1.SetCenterText();
	m_msg_client_port_1.SetColor(WHITE_C);
	m_msg_client_port_1.SetGradientColor(ORANGE_C);
	m_msg_client_port_1.SetTextColor(BLACK_C);

	m_msg_client_ip_2.SetFont(m_p_font);
	m_msg_client_ip_2.SetWindowText(_T("IP Address"));
	m_msg_client_ip_2.SetCenterText();
	m_msg_client_ip_2.SetColor(WHITE_C);
	m_msg_client_ip_2.SetGradientColor(ORANGE_C);
	m_msg_client_ip_2.SetTextColor(BLACK_C);
	
	m_msg_client_port_2.SetFont(m_p_font);
	m_msg_client_port_2.SetWindowText(_T("Port"));
	m_msg_client_port_2.SetCenterText();
	m_msg_client_port_2.SetColor(WHITE_C);
	m_msg_client_port_2.SetGradientColor(ORANGE_C);
	m_msg_client_port_2.SetTextColor(BLACK_C);


	m_msg_sec_server_port.SetFont(m_p_font);
	m_msg_sec_server_port.SetWindowText(_T("Port"));
	m_msg_sec_server_port.SetCenterText();
	m_msg_sec_server_port.SetColor(WHITE_C);
	m_msg_sec_server_port.SetGradientColor(ORANGE_C);
	m_msg_sec_server_port.SetTextColor(BLACK_C);

	m_msg_server_port_1.SetFont(m_p_font);
	m_msg_server_port_1.SetWindowText(_T("Port"));
	m_msg_server_port_1.SetCenterText();
	m_msg_server_port_1.SetColor(WHITE_C);
	m_msg_server_port_1.SetGradientColor(ORANGE_C);
	m_msg_server_port_1.SetTextColor(BLACK_C);

	m_msg_etc_server_port.SetFont(m_p_font);
	m_msg_etc_server_port.SetWindowText(_T("Port"));
	m_msg_etc_server_port.SetCenterText();
	m_msg_etc_server_port.SetColor(WHITE_C);
	m_msg_etc_server_port.SetGradientColor(ORANGE_C);
	m_msg_etc_server_port.SetTextColor(BLACK_C);



}

int CDialog_Work_Interface::ConverterToData(int mode, int pos)
{
	int Ret;

	switch(mode)
	{
		case COM_PORT:
			switch(pos)
			{
				case 0:
					Ret = 1;
					break;
				case 1:
					Ret = 2;
					break;
				case 2:
					Ret = 3;
					break;
				case 3:
					Ret = 4;
					break;
				case 4:
					Ret = 5;
					break;
				case 5:
					Ret = 6;
					break;
				case 6:
					Ret = 7;
					break;
				case 7:
					Ret = 8;
					break;
			}
			break;
		case COM_BAUDRATE:
			switch(pos)
			{
				case 0:
					Ret = 2400;
					break;
				case 1:
					Ret = 4800;
					break;
				case 2:
					Ret = 9600;
					break;
				case 3:
					Ret = 14400;
					break;
				case 4:
					Ret = 19200;
					break;
				case 5:
					Ret = 38400;
					break;
				case 6:
					Ret = 57600;
					break;
				case 7:
					Ret = 115200;
					break;
			}
			break;
		case COM_DATA:
			switch(pos)
			{
				case 0:
					Ret = 5;
					break;
				case 1:
					Ret = 6;
					break;
				case 2:
					Ret = 7;
					break;
				case 3:
					Ret = 8;
					break;
			}
			break;
		case COM_STOP:
			switch(pos)
			{
				case 0:
					Ret = 1;
					break;
				case 1:
					Ret = 2;
					break;
			}
			break;
		case COM_PARITY:
			switch(pos)
			{
				case 0:
					Ret = 2;
					break;
				case 1:
					Ret = 1;
					break;
				case 2:
					Ret = 0;
					break;
			}
			break;
	}

	return Ret;
}

int CDialog_Work_Interface::ConverterToPos(int mode, int data)
{
	int Ret;

	switch(mode)
	{
		case COM_PORT:
			switch(data)
			{
				case 1:
					Ret = 0;
					break;
				case 2:
					Ret = 1;
					break;
				case 3:
					Ret = 2;
					break;
				case 4:
					Ret = 3;
					break;
				case 5:
					Ret = 4;
					break;
				case 6:
					Ret = 5;
					break;
				case 7:
					Ret = 6;
					break;
				case 8:
					Ret = 7;
					break;
			}
			break;
		case COM_BAUDRATE:
			switch(data)
			{
				case 2400:
					Ret = 0;
					break;
				case 4800:
					Ret = 1;
					break;
				case 9600:
					Ret = 2;
					break;
				case 14400:
					Ret = 3;
					break;
				case 19200:
					Ret = 4;
					break;
				case 38400:
					Ret = 5;
					break;
				case 57600:
					Ret = 6;
					break;
				case 115200:
					Ret = 7;
					break;
			}
			break;
		case COM_DATA:
			switch(data)
			{
				case 5:
					Ret = 0;
					break;
				case 6:
					Ret = 1;
					break;
				case 7:
					Ret = 2;
					break;
				case 8:
					Ret = 3;
					break;
			}
			break;
		case COM_STOP:
			switch(data)
			{
				case 1:
					Ret = 0;
					break;
				case 2:
					Ret = 1;
					break;
			}
			break;
		case COM_PARITY:
			switch(data)
			{
				case 0:
					Ret = 2;
					break;
				case 1:
					Ret = 1;
					break;
				case 2:
					Ret = 0;
					break;
			}
			break;
	}

	return Ret;
}

void CDialog_Work_Interface::OnInterface_Display()
{
	CString str_tmp;

	m_cb_port_1.SetCurSel(m_n_port[MODULE_LOAD_PORT][1]);
	m_cb_rate_1.SetCurSel(m_n_rate[MODULE_LOAD_PORT][1]);
	m_cb_data_1.SetCurSel(m_n_data[MODULE_LOAD_PORT][1]);
	m_cb_parity_1.SetCurSel(m_n_parity[MODULE_LOAD_PORT][1]);
	m_cb_stop_1.SetCurSel(m_n_stop[MODULE_LOAD_PORT][1]);
	
	m_cb_port_2.SetCurSel(m_n_port[HEATSINK_LEFT_PORT][1]);
	m_cb_rate_2.SetCurSel(m_n_rate[HEATSINK_LEFT_PORT][1]);
	m_cb_data_2.SetCurSel(m_n_data[HEATSINK_LEFT_PORT][1]);
	m_cb_parity_2.SetCurSel(m_n_parity[HEATSINK_LEFT_PORT][1]);
	m_cb_stop_2.SetCurSel(m_n_stop[HEATSINK_LEFT_PORT][1]);
	
	m_cb_port_3.SetCurSel(m_n_port[HEATSINK_RIGHT_PORT][1]);
	m_cb_rate_3.SetCurSel(m_n_rate[HEATSINK_RIGHT_PORT][1]);
	m_cb_data_3.SetCurSel(m_n_data[HEATSINK_RIGHT_PORT][1]);
	m_cb_parity_3.SetCurSel(m_n_parity[HEATSINK_RIGHT_PORT][1]);
	m_cb_stop_3.SetCurSel(m_n_stop[HEATSINK_RIGHT_PORT][1]);

	///////////////////////////////////////////20120514 유성준
	m_cb_port_4.SetCurSel(m_n_port[CLIP_PORT][1]);
	m_cb_rate_4.SetCurSel(m_n_rate[CLIP_PORT][1]);
	m_cb_data_4.SetCurSel(m_n_data[CLIP_PORT][1]);
	m_cb_parity_4.SetCurSel(m_n_parity[CLIP_PORT][1]);
	m_cb_stop_4.SetCurSel(m_n_stop[CLIP_PORT][1]);
	///////////////////////////////////////////20120519 유성준
	m_cb_port_5.SetCurSel(m_n_port[VISION_PORT][1]);
	m_cb_rate_5.SetCurSel(m_n_rate[VISION_PORT][1]);
	m_cb_data_5.SetCurSel(m_n_data[VISION_PORT][1]);
	m_cb_parity_5.SetCurSel(m_n_parity[VISION_PORT][1]);
	m_cb_stop_5.SetCurSel(m_n_stop[VISION_PORT][1]);
	///////////////////////////////////////////20120610 유성준
	m_cb_port_6.SetCurSel(m_n_port[LABEL_PRINTER_PORT][1]);
	m_cb_rate_6.SetCurSel(m_n_rate[LABEL_PRINTER_PORT][1]);
	m_cb_data_6.SetCurSel(m_n_data[LABEL_PRINTER_PORT][1]);
	m_cb_parity_6.SetCurSel(m_n_parity[LABEL_PRINTER_PORT][1]);
	m_cb_stop_6.SetCurSel(m_n_stop[LABEL_PRINTER_PORT][1]);


	///////////////////////////////////////////

	str_tmp.Format("%d", m_n_client_port[NETWORK_CLIENT_1][1]);
	m_edit_client_port_1.SetWindowText(str_tmp);
	
	str_tmp.Format("%d", m_n_client_port[NETWORK_CLIENT_7][1]);
	m_edit_client_port_2.SetWindowText(str_tmp);

	str_tmp.Format("%d", m_n_server_port[0][1]);//BCR
	m_edit_server_port_1.SetWindowText(str_tmp);

	str_tmp.Format("%d", m_n_server_port[1][1]);//SBA
	m_edit_sec_server_port.SetWindowText(str_tmp);

	str_tmp.Format("%d", m_n_server_port[2][1]);//ETC
	m_edit_etc_server_port.SetWindowText(str_tmp);

	OnInterface_IP(m_str_client_ip[NETWORK_CLIENT_1][1]);
	m_client_ip_1.SetAddress(m_by_ip[0], m_by_ip[1], m_by_ip[2], m_by_ip[3]);
	
	OnInterface_IP(m_str_client_ip[NETWORK_CLIENT_7][1]);
	m_client_ip_2.SetAddress(m_by_ip[0], m_by_ip[1], m_by_ip[2], m_by_ip[3]);

}

void CDialog_Work_Interface::Init_Editbox()
{
	m_edit_client_port_1.SubclassDlgItem(IDC_EDIT_CLIENT_PORT_1, this);
	m_edit_client_port_1.bkColor(RGB(50, 100, 150));
	m_edit_client_port_1.textColor(RGB(255, 255,255));
	m_edit_client_port_1.setFont(-10);
	
	m_edit_client_port_2.SubclassDlgItem(IDC_EDIT_CLIENT_PORT_2, this);
	m_edit_client_port_2.bkColor(RGB(50, 100, 150));
	m_edit_client_port_2.textColor(RGB(255, 255,255));
	m_edit_client_port_2.setFont(-10);

	m_edit_sec_server_port.SubclassDlgItem(IDC_EDIT_SEC_SERVER_PORT, this);
	m_edit_sec_server_port.bkColor(RGB(50, 100, 150));
	m_edit_sec_server_port.textColor(RGB(255, 255,255));
	m_edit_sec_server_port.setFont(-10);

	m_edit_server_port_1.SubclassDlgItem(IDC_EDIT_SERVER_PORT_1, this);
	m_edit_server_port_1.bkColor(RGB(50, 100, 150));
	m_edit_server_port_1.textColor(RGB(255, 255,255));
	m_edit_server_port_1.setFont(-10);
	
	m_edit_etc_server_port.SubclassDlgItem(IDC_EDIT_ETC_SERVER_PORT, this);
	m_edit_etc_server_port.bkColor(RGB(50, 100, 150));
	m_edit_etc_server_port.textColor(RGB(255, 255,255));
	m_edit_etc_server_port.setFont(-10);
}

void CDialog_Work_Interface::OnInterface_IP(CString str_ip)
{
	int nDot1, nDot2, nDot3, nStart;
	
	nStart = 0;
	nDot1 = str_ip.Find('.');
	nDot2 = str_ip.Find('.', nDot1 + 1);
	nDot3 = str_ip.Find('.', nDot2 + 1);
	
	m_by_ip[0]	= atoi(str_ip.Mid(nStart, nDot1 - nStart));
	nStart		= nDot1 + 1;
	m_by_ip[1]	= atoi(str_ip.Mid(nStart, nDot2 - nStart));
	nStart		= nDot2 + 1;
	m_by_ip[2]	= atoi(str_ip.Mid(nStart, nDot3 - nStart));
	nStart		= nDot3 + 1;
	m_by_ip[3]	= atoi(str_ip.Mid(nStart));
}

void CDialog_Work_Interface::OnBtnSerialApply1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	m_n_port[MODULE_LOAD_PORT][1]			= m_cb_port_1.GetCurSel();
	m_n_rate[MODULE_LOAD_PORT][1]			= m_cb_rate_1.GetCurSel();
	m_n_data[MODULE_LOAD_PORT][1]			= m_cb_data_1.GetCurSel();
	m_n_stop[MODULE_LOAD_PORT][1]			= m_cb_stop_1.GetCurSel();
	m_n_parity[MODULE_LOAD_PORT][1]	    	= m_cb_parity_1.GetCurSel();
}

void CDialog_Work_Interface::OnBtnClientIp1() 
{
	// TODO: Add your control notification handler code here

	///////////////////////////////////////// 20120516
	//	m_str_client_ip[NETWORK_CLIENT_1][1] = GetEditString(m_str_client_ip[NETWORK_CLIENT_1][1]);
	
	int n_response;									// 대화 상자 리턴 플래그
	
	(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	
	st_msg.mstr_typing_msg = m_str_client_ip[NETWORK_CLIENT_1][1];
	
	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		m_str_client_ip[NETWORK_CLIENT_1][1] = st_msg.mstr_typing_msg;
	}
	else
	{
		return;
	}
	/////////////////////////////////////////

	m_str_client_ip[NETWORK_CLIENT_1][1].MakeUpper();
	m_str_client_ip[NETWORK_CLIENT_1][1].TrimLeft(' ');               
	m_str_client_ip[NETWORK_CLIENT_1][1].TrimRight(' ');
	
	OnInterface_IP(m_str_client_ip[NETWORK_CLIENT_1][1]);
	
	m_client_ip_1.SetAddress(m_by_ip[0], m_by_ip[1], m_by_ip[2], m_by_ip[3]);
}

void CDialog_Work_Interface::OnBtnClientPort1() 
{
	// TODO: Add your control notification handler code here
	CString str_tmp;  // 저장할 정보 임시 저장 변수 
	
	str_tmp.Format("%d", m_n_client_port[NETWORK_CLIENT_1][1]);
	
	///////////////////////////////////////// 20120516
	//str_tmp = GetEditString(str_tmp);
	
	int n_response;									// 대화 상자 리턴 플래그
	
	(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	
	st_msg.mstr_typing_msg = str_tmp;
	
	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		str_tmp = st_msg.mstr_typing_msg;
	}
	else
	{
		return;
	}
	/////////////////////////////////////////

	m_n_client_port[NETWORK_CLIENT_1][1] = atoi(str_tmp);	
	m_edit_client_port_1.SetWindowText(str_tmp);
}

void CDialog_Work_Interface::OnBtnClientIp2() 
{
	// TODO: Add your control notification handler code here

	///////////////////////////////////////// 20120516
	//m_str_client_ip[NETWORK_CLIENT_7][1] = GetEditString(m_str_client_ip[NETWORK_CLIENT_7][1]);
	
	int n_response;									// 대화 상자 리턴 플래그
	
	(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	
	st_msg.mstr_typing_msg = m_str_client_ip[NETWORK_CLIENT_7][1];
	
	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		m_str_client_ip[NETWORK_CLIENT_7][1] = st_msg.mstr_typing_msg;
	}
	else
	{
		return;
	}
	/////////////////////////////////////////

	m_str_client_ip[NETWORK_CLIENT_7][1].MakeUpper();
	m_str_client_ip[NETWORK_CLIENT_7][1].TrimLeft(' ');               
	m_str_client_ip[NETWORK_CLIENT_7][1].TrimRight(' ');
	
	OnInterface_IP(m_str_client_ip[NETWORK_CLIENT_7][1]);
	
	m_client_ip_2.SetAddress(m_by_ip[0], m_by_ip[1], m_by_ip[2], m_by_ip[3]);
}

	

void CDialog_Work_Interface::OnBtnClientPort2() 
{
	// TODO: Add your control notification handler code here
	CString str_tmp;  // 저장할 정보 임시 저장 변수 
	
	str_tmp.Format("%d", m_n_client_port[NETWORK_CLIENT_7][1]);
	
	///////////////////////////////////////// 20120516
	//str_tmp = GetEditString(str_tmp);
	
	int n_response;									// 대화 상자 리턴 플래그
	
	(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	
	st_msg.mstr_typing_msg = str_tmp;
	
	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		str_tmp = st_msg.mstr_typing_msg;
	}
	else
	{
		return;
	}
	/////////////////////////////////////////


	m_n_client_port[NETWORK_CLIENT_7][1] = atoi(str_tmp);	
	m_edit_client_port_2.SetWindowText(str_tmp);
}

// int CDialog_Work_Interface::Manual_Bcr_Test()
// {
// 	int Ret=0, Ret_1=0, i=0; 
// 	int	FuncRet = RET_PROCEED;
// 	double dSpeedRatio[3] = {100,100,100};
// 	int ratio = 50;
// 	CString strClient;
// 
// 	if(m_b_stop_req == TRUE)
// 	{
// // 		COMI.Set_MotStop(0, MOT_MIDDLE_PUSH);
// 		m_bcr_step = 8000;
// 	}
// 
// 
// 	switch(m_bcr_step)
// 	{
// 	case 0:
// 		m_bcr_step = 1000; 
// 		break;
// 		
// 	case 1000:
// 		m_bcr_step = 1200;
// 		break;
// 
// 	case 1200:
// 		//Read Barcode
// 		// ==============================================================================================================
// 		// 바코드 스케닝을 시작하자!
// 		// ==============================================================================================================
// 		st_client[NETWORK_CLIENT_7].n_client_send_flag = NO;
// 		if(Func.OnBCR_Command(BCR_READON, &str_command))
// 		{
// 			st_data.str_sndBarcode.Empty();
// 			st_data.str_sndBarcode = str_command;                  // 송신 데이터 설정
// 			l_bcr_send_wait[0] = GetCurrentTime();
// 
// 			::PostMessage(st_handler.hWnd,WM_WORK_COMMAND, MAIN_RCV_BARCODE, 0);  // 데이터 송신 요청
// 			m_bcr_step = 1300;
// 		}
// 		break;
// 
// 	case 1300:
// 		// ==============================================================================================================
// 		// 바코드 스케너가 켜졌는지 확인!
// 		// ==============================================================================================================
// 		l_bcr_send_wait[1] = GetCurrentTime();
// 		l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
// 		if(l_bcr_send_wait[2] < 0)
// 		{
// 			l_bcr_send_wait[0] = GetCurrentTime();
// 			break;
// 		}
// 		if(l_bcr_send_wait[2] > 200)
// 		{
// 			//if(st_client[NETWORK_CLIENT_7].n_connect == YES)
// 			if(st_client[NETWORK_CLIENT_7].n_client_send_flag == YES)
// 			{
// 				l_bcr_send_wait[0] = GetCurrentTime();
// 				m_bcr_step = 1400;
// 			}
// 		}
// 		break;
// 
// 	case 1400:
// 		l_bcr_send_wait[1] = GetCurrentTime();
// 		l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
// 		if(l_bcr_send_wait[2] < 0)
// 		{
// 			l_bcr_send_wait[0] = GetCurrentTime();
// 			break;
// 		}
// 		if(l_bcr_send_wait[2] > 100)
// 		{
// 			m_bcr_step = 2000;
// 		}
// 		break;		
// 
// 	case 2000:
// 		// ==============================================================================================================
// 		// 바코드 스케닝을 종료하자!
// 		// ==============================================================================================================
// 		st_server[0].n_rev_bcr = NO;
// 		st_client[NETWORK_CLIENT_7].n_client_send_flag = NO;
// 		if(Func.OnBCR_Command(BCR_READOFF, &str_command))
// 		{
// 			st_data.str_sndBarcode.Empty();
// 			st_data.str_sndBarcode = str_command;                  // 송신 데이터 설정
// 			l_bcr_send_wait[0] = GetCurrentTime();			
// 			::SendMessage(st_handler.hWnd,WM_WORK_COMMAND, MAIN_RCV_BARCODE, 0);  // 데이터 송신 요청
// 			m_bcr_step = 2100;
// 		}
// 		break;
// 
// 	case 2100:
// 		l_bcr_send_wait[1] = GetCurrentTime();
// 		l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
// 		if(l_bcr_send_wait[2] < 0)
// 		{
// 			l_bcr_send_wait[0] = GetCurrentTime();
// 			break;
// 		}
// 		if(l_bcr_send_wait[2] > 200)
// 		{
// 			//if(st_client[NETWORK_CLIENT_7].n_connect == YES)
// 			if(st_client[NETWORK_CLIENT_7].n_client_send_flag == YES)
// 			{
// 				l_bcr_send_wait[0] = GetCurrentTime();
// 				m_bcr_step = 2110;
// 			}
// 		}
// 		break;
// 
// 	case 2110:
// 		l_bcr_send_wait[1] = GetCurrentTime();
// 		l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
// 		if(l_bcr_send_wait[2] < 0)
// 		{
// 			l_bcr_send_wait[0] = GetCurrentTime();
// 			break;
// 		}
// 		if(l_bcr_send_wait[2] > 500)
// 		{
// 			l_bcr_send_wait[0] = GetCurrentTime();
// 			m_bcr_step = 2200;
// 		}
// 		break;		
// 
// 
// 	case 2200:
// 		// ==============================================================================================================
// 		// 수신된 바코드 정보를 확인하자!!
// 		// ==============================================================================================================
// 		//if(st_client[NETWORK_CLIENT_7].n_connect == TRUE) // 정보를 받았다
// 		if(st_server[0].n_rev_bcr == YES)
// 		{
// 			strClient.Format("%s",st_client[NETWORK_CLIENT_7].ch_rev);
// 			Ret = Func.OnBCR_Parsing(strClient, iPickerInfo, st_barcode.strBarcode);
// 
// 			if(Ret == TRUE)
// 			//if(Ret == TRUE || st_basic.i_mode_interface == FALSE)
// 			{
// 				strBcrReadData.Format("[Barcode]");
// 				for(i = 0 ; i < BCR_MAX_CNT-1 ; i++)
// 				{					
// 					strBcrReadDataEach.Format(" ; %d:%s", i + 1, st_barcode.strBarcode[i]);
// 					strBcrReadData += strBcrReadDataEach;
// 				}
// 				if(st_handler.cwnd_list != NULL)
// 				{
// 					st_other.str_op_msg = strBcrReadData;
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, BCR_PORT, RECEIVE_MSG);
// 				}
// 
// 				st_modulemap.BcrData_UldBuffer.Format("%s",st_barcode.strBarcode[0]);
// 
// 				m_bcr_step = 5000; //정상  동작 또는 바코드를 하지 하지 않을때 
// 			}
// 			else
// 			{
// 				if(st_handler.cwnd_list != NULL)
// 				{
// 					//st_other.str_abnormal_msg = st_serial.str_rec[BCR_PORT - 1];
// 					sprintf(st_other.c_abnormal_msg,"%s",st_client[NETWORK_CLIENT_7].ch_rev);
// 					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 				}
// 
// 				m_bcr_step = 3000;  //fly retry!!
// 			}
// 		}
// 		else
// 		{
// 			l_bcr_send_wait[1] = GetCurrentTime();
// 			l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
// 			if(l_bcr_send_wait[2] < 0)
// 			{
// 				l_bcr_send_wait[0] = GetCurrentTime();
// 				break;
// 			}
// 			if(l_bcr_send_wait[2] > 1000)//BCR_RCV_WAIT)
// 			{
// 				//전송 요청했지만 완료가 안되어졌다.. 알람발생 타임아웃
// 				//510002 0 51 "Barcode Serial Port Timeout Error."
// 				alarm.mstr_code = _T("510002");
// 				alarm.mn_count_mode = 0;
// 				alarm.mn_type_mode = eWARNING;
// 				st_work.mn_run_status = dWARNING;
// 				alarm.mn_alarm_assign_section = 181;
// 				m_b_stop_req = TRUE;
// 				m_bcr_step = 1000;
// 			}
// 		}
// 		break;
// 
// 	case 3000:
// 		for(i = 0 ; i < PICKER_NUM ; i++)
// 		{
// 			st_barcode.strBarcode[i].Empty();
// 		}
// 
// 		iBarcodeScanRetryCnt++;
// 		if(iBarcodeScanRetryCnt > 5)  //반복 횟수를 설정~!
// 		{
// 			iBarcodeScanRetryCnt = 0;
// 			m_bcr_step = 4000;	//step mode reading go!
// 		}
// 		else
// 		{
// 			m_bcr_step = 1000;  //fly retry!!
// 		}
// 		break;
// 
// 	case 4000:
// 		//에러 발생
// 
// 		m_bcr_step = 8000;  //fly retry!!
// 		break;
// 
// 	case 5000:
// 		st_modulemap.BcrData_UldBuffer = st_barcode.strBarcode[0];
// 		st_barcode.strBarcode[0] = _T("NO CODE");
// 		m_bcr_step = 0;
// 		FuncRet = RET_GOOD;
// 		break;
// 
// 
// 	case 8000:
// 		FuncRet = RET_ERROR;
// 		m_bcr_step = 0;
// 		break;
// 
// 	}
// 
//  	return FuncRet;
// }

void CDialog_Work_Interface::OnBtnServerPort1() 
{
	// TODO: Add your control notification handler code here
	CString str_tmp;  // 저장할 정보 임시 저장 변수 
	
	str_tmp.Format("%d", m_n_server_port[0][1]);

	///////////////////////////////////////// 20120516
	//str_tmp = GetEditString(str_tmp);
	
	int n_response;									// 대화 상자 리턴 플래그
	
	(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	
	st_msg.mstr_typing_msg = str_tmp;
	
	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		str_tmp = st_msg.mstr_typing_msg;
	}
	else
	{
		return;
	}
	/////////////////////////////////////////

	m_n_server_port[0][1] = atoi(str_tmp);	
	m_edit_server_port_1.SetWindowText(str_tmp);


}

void CDialog_Work_Interface::OnBtnSerialConnect1() 
{
	// TODO: Add your control notification handler code here
	rs_232.n_serial_port[MODULE_LOAD_PORT]		= ConverterToData(COM_PORT, m_n_port[MODULE_LOAD_PORT][1]);
	rs_232.n_serial_data[MODULE_LOAD_PORT]		= ConverterToData(COM_DATA, m_n_data[MODULE_LOAD_PORT][1]);
	rs_232.n_serial_stop[MODULE_LOAD_PORT]		= ConverterToData(COM_STOP, m_n_stop[MODULE_LOAD_PORT][1]);
	rs_232.n_serial_baudrate[MODULE_LOAD_PORT]	= ConverterToData(COM_BAUDRATE, m_n_rate[MODULE_LOAD_PORT][1]);
	rs_232.n_serial_parity[MODULE_LOAD_PORT]	= ConverterToData(COM_PARITY, m_n_parity[MODULE_LOAD_PORT][1]);

	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, YES, rs_232.n_serial_port[MODULE_LOAD_PORT]);
}


void CDialog_Work_Interface::OnBtnSerialApply2() 
{
	UpdateData(TRUE);
	
	m_n_port[HEATSINK_LEFT_PORT][1]			= m_cb_port_2.GetCurSel();
	m_n_rate[HEATSINK_LEFT_PORT][1]			= m_cb_rate_2.GetCurSel();
	m_n_data[HEATSINK_LEFT_PORT][1]			= m_cb_data_2.GetCurSel();
	m_n_stop[HEATSINK_LEFT_PORT][1]			= m_cb_stop_2.GetCurSel();
	m_n_parity[HEATSINK_LEFT_PORT][1]		= m_cb_parity_2.GetCurSel();
	
}

void CDialog_Work_Interface::OnBtnSerialConnect2() 
{
	// TODO: Add your control notification handler code here
	rs_232.n_serial_port[HEATSINK_LEFT_PORT]		= ConverterToData(COM_PORT, m_n_port[HEATSINK_LEFT_PORT][1]);
	rs_232.n_serial_data[HEATSINK_LEFT_PORT]		= ConverterToData(COM_DATA, m_n_data[HEATSINK_LEFT_PORT][1]);
	rs_232.n_serial_stop[HEATSINK_LEFT_PORT]		= ConverterToData(COM_STOP, m_n_stop[HEATSINK_LEFT_PORT][1]);
	rs_232.n_serial_baudrate[HEATSINK_LEFT_PORT]	= ConverterToData(COM_BAUDRATE, m_n_rate[HEATSINK_LEFT_PORT][1]);
	rs_232.n_serial_parity[HEATSINK_LEFT_PORT]	= ConverterToData(COM_PARITY, m_n_parity[HEATSINK_LEFT_PORT][1]);

	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, YES, rs_232.n_serial_port[HEATSINK_LEFT_PORT]);
	
}

////////////////////////////////////////// 20120514 유성준
void CDialog_Work_Interface::OnBtnSerialApply4() 
{
	UpdateData(TRUE);
	
	m_n_port[CLIP_PORT][1]			= m_cb_port_4.GetCurSel();
	m_n_rate[CLIP_PORT][1]			= m_cb_rate_4.GetCurSel();
	m_n_data[CLIP_PORT][1]			= m_cb_data_4.GetCurSel();
	m_n_stop[CLIP_PORT][1]			= m_cb_stop_4.GetCurSel();
	m_n_parity[CLIP_PORT][1]		= m_cb_parity_4.GetCurSel();
	
}

void CDialog_Work_Interface::OnBtnSerialConnect4() 
{
	// TODO: Add your control notification handler code here
	rs_232.n_serial_port[CLIP_PORT]		= ConverterToData(COM_PORT, m_n_port[CLIP_PORT][1]);
	rs_232.n_serial_data[CLIP_PORT]		= ConverterToData(COM_DATA, m_n_data[CLIP_PORT][1]);
	rs_232.n_serial_stop[CLIP_PORT]		= ConverterToData(COM_STOP, m_n_stop[CLIP_PORT][1]);
	rs_232.n_serial_baudrate[CLIP_PORT]	= ConverterToData(COM_BAUDRATE, m_n_rate[CLIP_PORT][1]);
	rs_232.n_serial_parity[CLIP_PORT]	= ConverterToData(COM_PARITY, m_n_parity[CLIP_PORT][1]);

	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, YES, rs_232.n_serial_port[CLIP_PORT]);
	
}



void CDialog_Work_Interface::OnBtnSerialApply3() 
{
	UpdateData(TRUE);
	
	m_n_port[HEATSINK_RIGHT_PORT][1]			= m_cb_port_3.GetCurSel();
	m_n_rate[HEATSINK_RIGHT_PORT][1]			= m_cb_rate_3.GetCurSel();
	m_n_data[HEATSINK_RIGHT_PORT][1]			= m_cb_data_3.GetCurSel();
	m_n_stop[HEATSINK_RIGHT_PORT][1]			= m_cb_stop_3.GetCurSel();
	m_n_parity[HEATSINK_RIGHT_PORT][1]		= m_cb_parity_3.GetCurSel();
	
}

void CDialog_Work_Interface::OnBtnSerialConnect3() 
{
	rs_232.n_serial_port[HEATSINK_RIGHT_PORT]		= ConverterToData(COM_PORT, m_n_port[HEATSINK_RIGHT_PORT][1]);
	rs_232.n_serial_data[HEATSINK_RIGHT_PORT]		= ConverterToData(COM_DATA, m_n_data[HEATSINK_RIGHT_PORT][1]);
	rs_232.n_serial_stop[HEATSINK_RIGHT_PORT]		= ConverterToData(COM_STOP, m_n_stop[HEATSINK_RIGHT_PORT][1]);
	rs_232.n_serial_baudrate[HEATSINK_RIGHT_PORT]	= ConverterToData(COM_BAUDRATE, m_n_rate[HEATSINK_RIGHT_PORT][1]);
	rs_232.n_serial_parity[HEATSINK_RIGHT_PORT]	= ConverterToData(COM_PARITY, m_n_parity[HEATSINK_RIGHT_PORT][1]);

	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, YES, rs_232.n_serial_port[HEATSINK_RIGHT_PORT]);	
}



void CDialog_Work_Interface::OnButtonStop() 
{
	m_b_stop_req = TRUE;	
}

////////////////////////////////////////////////////////// 20120620
void CDialog_Work_Interface::OnBtnSecServerPort() 
{
	CString str_tmp;  // 저장할 정보 임시 저장 변수 
	
	str_tmp.Format("%d", m_n_server_port[1][1]);
	
	//str_tmp = GetEditString(str_tmp);

	int n_response;									// 대화 상자 리턴 플래그
	
	(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	
	st_msg.mstr_typing_msg = str_tmp;
	
	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		str_tmp = st_msg.mstr_typing_msg;
	}
	else
	{
		return;
	}
	
	m_n_server_port[1][1] = atoi(str_tmp);	
	m_edit_sec_server_port.SetWindowText(str_tmp);	
}

void CDialog_Work_Interface::OnBtnEtcServerPort() 
{
	CString str_tmp;  // 저장할 정보 임시 저장 변수 
	
	str_tmp.Format("%d", m_n_server_port[2][1]);
	
	//str_tmp = GetEditString(str_tmp);
	
	int n_response;									// 대화 상자 리턴 플래그
	
	(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	
	st_msg.mstr_typing_msg = str_tmp;
	
	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		str_tmp = st_msg.mstr_typing_msg;
	}
	else
	{
		return;
	}


	m_n_server_port[2][1] = atoi(str_tmp);	
	m_edit_etc_server_port.SetWindowText(str_tmp);	
}

void CDialog_Work_Interface::OnButtonTest() 
{
	::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, 0);
	::Sleep(1000);
	sprintf(st_client[0].ch_send,"TEST_TEST_TEST");
	if(st_client[0].n_connect == YES)
	{
		::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, 0);
	}
}

void CDialog_Work_Interface::OnBtnSerialApply5() 
{
	UpdateData(TRUE);
	
	m_n_port[VISION_PORT][1]			= m_cb_port_5.GetCurSel();
	m_n_rate[VISION_PORT][1]			= m_cb_rate_5.GetCurSel();
	m_n_data[VISION_PORT][1]			= m_cb_data_5.GetCurSel();
	m_n_stop[VISION_PORT][1]			= m_cb_stop_5.GetCurSel();
	m_n_parity[VISION_PORT][1]		= m_cb_parity_5.GetCurSel();
	
}

void CDialog_Work_Interface::OnBtnSerialConnect5() 
{
	// TODO: Add your control notification handler code here
	rs_232.n_serial_port[VISION_PORT]		= ConverterToData(COM_PORT, m_n_port[VISION_PORT][1]);
	rs_232.n_serial_data[VISION_PORT]		= ConverterToData(COM_DATA, m_n_data[VISION_PORT][1]);
	rs_232.n_serial_stop[VISION_PORT]		= ConverterToData(COM_STOP, m_n_stop[VISION_PORT][1]);
	rs_232.n_serial_baudrate[VISION_PORT]	= ConverterToData(COM_BAUDRATE, m_n_rate[VISION_PORT][1]);
	rs_232.n_serial_parity[VISION_PORT]	= ConverterToData(COM_PARITY, m_n_parity[VISION_PORT][1]);

	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, YES, rs_232.n_serial_port[VISION_PORT]);
	
}

void CDialog_Work_Interface::OnBtnSerialApply6() 
{
	UpdateData(TRUE);
	
	m_n_port[LABEL_PRINTER_PORT][1]			= m_cb_port_6.GetCurSel();
	m_n_rate[LABEL_PRINTER_PORT][1]			= m_cb_rate_6.GetCurSel();
	m_n_data[LABEL_PRINTER_PORT][1]			= m_cb_data_6.GetCurSel();
	m_n_stop[LABEL_PRINTER_PORT][1]			= m_cb_stop_6.GetCurSel();
	m_n_parity[LABEL_PRINTER_PORT][1]		= m_cb_parity_6.GetCurSel();
	
}

void CDialog_Work_Interface::OnBtnSerialConnect6() 
{
	// TODO: Add your control notification handler code here
	rs_232.n_serial_port[LABEL_PRINTER_PORT]		= ConverterToData(COM_PORT, m_n_port[LABEL_PRINTER_PORT][1]);
	rs_232.n_serial_data[LABEL_PRINTER_PORT]		= ConverterToData(COM_DATA, m_n_data[LABEL_PRINTER_PORT][1]);
	rs_232.n_serial_stop[LABEL_PRINTER_PORT]		= ConverterToData(COM_STOP, m_n_stop[LABEL_PRINTER_PORT][1]);
	rs_232.n_serial_baudrate[LABEL_PRINTER_PORT]	= ConverterToData(COM_BAUDRATE, m_n_rate[LABEL_PRINTER_PORT][1]);
	rs_232.n_serial_parity[LABEL_PRINTER_PORT]	= ConverterToData(COM_PARITY, m_n_parity[LABEL_PRINTER_PORT][1]);

	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, YES, rs_232.n_serial_port[LABEL_PRINTER_PORT]);
	
}

void CDialog_Work_Interface::OnBtnJobChange() 
{
	// TODO: Add your control notification handler code here
	CString str_tmp;
	CString str_print_data;

	if (st_serial.n_connect[2] == YES)
	{
		st_other.str_normal_msg =  _T("[Manual] BCR CJ Button Click");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}

		mn_vision_hs_enable = 1;
		
		m_b_stop_req = FALSE;
		OnBtn_BCR_HS_YIELD_Enable_False();
		l_bcr_send_wait[0] = GetCurrentTime();

	//	str_tmp.Format(st_basic.mstr_label_name);
		str_tmp.Format(st_interface.str_bcr_ml_name);
		str_print_data = str_tmp.Mid(0,3);
		st_work.n_bcrNojob = atoi(str_print_data);
		st_basic.mstr_label_name = st_interface.str_bcr_ml_name;
		
		Func.OnSet_Vision_send(VISION_CJ);
		
		SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);		
	}
	else
	{
		st_other.str_normal_msg =  _T("[Manual] 통신포트를 연결하세요.");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}
		
	}

}

void CDialog_Work_Interface::OnBtnJobChange2() 
{
	// TODO: Add your control notification handler code here
// 	st_other.str_normal_msg =  _T("[Manual] BCR TKIN Button Click");
// 	sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
// 	if (st_handler.cwnd_list != NULL)
// 	{
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 	}
// 
// 	mn_vision_hs_enable = 2;
// 
// 	m_b_stop_req = FALSE;
// 	OnBtn_BCR_HS_YIELD_Enable_False();
// 	l_bcr_send_wait[0] = GetCurrentTime();
// //	::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, VISION_TKIN);	
// 	Func.OnSet_Vision_send(VISION_TKIN);
// 	SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);		

}

void CDialog_Work_Interface::OnBtnJobChange3() 
{

	// TODO: Add your control notification handler code here
// 	st_other.str_normal_msg =  _T("[Manual] BCR TKOUT Button Click");
// 	sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
// 	if (st_handler.cwnd_list != NULL)
// 	{
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 	}
// 
// 	mn_vision_hs_enable = 3;
// 
// 	m_b_stop_req = FALSE;
// 	OnBtn_BCR_HS_YIELD_Enable_False();
// 	l_bcr_send_wait[0] = GetCurrentTime();
// //	::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, VISION_TKOUT);
// 	Func.OnSet_Vision_send(VISION_TKOUT);
// 
// 	SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);		

}

void CDialog_Work_Interface::OnBtnJobChange4() 
{
	// TODO: Add your control notification handler code here
	if (st_serial.n_connect[2] == YES)
	{
		st_other.str_normal_msg =  _T("[Manual] BCR Yield Button Click");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}

		mn_vision_hs_enable = 4;
		m_b_stop_req = FALSE;
		OnBtn_BCR_HS_YIELD_Enable_False();
		l_bcr_send_wait[0] = GetCurrentTime();

	//	::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, VISION_YIELD);
		Func.OnSet_Vision_send(VISION_YIELD);

		SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);		
	}
	else
	{
		st_other.str_normal_msg =  _T("[Manual] 통신포트를 연결하세요.");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}
		
	}

}

void CDialog_Work_Interface::OnBtnJobChange5() 
{
	CString str_tmp;
	CString str_print_data;

	if (st_serial.n_connect[2] == YES)
	{
		// TODO: Add your control notification handler code here
		st_other.str_normal_msg =  _T("[Manual] HS CJ Button Click");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}

		mn_vision_hs_enable = 5;

		m_b_stop_req = FALSE;
		OnBtn_BCR_HS_YIELD_Enable_False();
		l_bcr_send_wait[0] = GetCurrentTime();

	// 	str_tmp.Format(st_basic.mstr_label_name);
		str_tmp.Format(st_interface.str_hs_ml_name);
		str_print_data = str_tmp.Mid(0,3);
		st_work.n_hsNojob = atoi(str_print_data);//20120831
		st_basic.mstr_label_name = st_interface.str_hs_ml_name;
		
		Func.OnSet_Vision_send(HEATSINK_CJ);

		SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);		
	}
	else
	{
		st_other.str_normal_msg =  _T("[Manual] 통신포트를 연결하세요.");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}
		
	}


}

void CDialog_Work_Interface::OnBtnJobChange6() 
{

	// TODO: Add your control notification handler code here
// 	st_other.str_normal_msg =  _T("[Manual] HS TKIN Button Click");
// 	sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
// 	if (st_handler.cwnd_list != NULL)
// 	{
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 	}
// 
// 	mn_vision_hs_enable = 6;
// 
// 	m_b_stop_req = FALSE;
// 	OnBtn_BCR_HS_YIELD_Enable_False();
// 	l_bcr_send_wait[0] = GetCurrentTime();
// //	::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_TKIN);	
// 	Func.OnSet_Vision_send(HEATSINK_TKIN);
// 
// 	SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);		

}

void CDialog_Work_Interface::OnBtnJobChange7() 
{
	// TODO: Add your control notification handler code here
// 	st_other.str_normal_msg =  _T("[Manual] HS TKOUT Button Click");
// 	sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
// 	if (st_handler.cwnd_list != NULL)
// 	{
// 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 	}
// 
// 	mn_vision_hs_enable = 7;
// 
// 	m_b_stop_req = FALSE;
// 	OnBtn_BCR_HS_YIELD_Enable_False();
// 	l_bcr_send_wait[0] = GetCurrentTime();
// //	::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_TKOUT);	
// 	Func.OnSet_Vision_send(HEATSINK_TKOUT);
// 
// 	SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);		

}

void CDialog_Work_Interface::OnBtnJobChange8() 
{
	// TODO: Add your control notification handler code here
	if (st_serial.n_connect[2] == YES)
	{
		st_other.str_normal_msg =  _T("[Manual] HS Yield Button Click");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}

		mn_vision_hs_enable = 8;
		m_b_stop_req = FALSE;
		OnBtn_BCR_HS_YIELD_Enable_False();
		l_bcr_send_wait[0] = GetCurrentTime();

	//	::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_YIELD);	
		Func.OnSet_Vision_send(HEATSINK_YIELD);

		SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);		
	}
	else
	{
		st_other.str_normal_msg =  _T("[Manual] 통신포트를 연결하세요.");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}
		
	}
}

void CDialog_Work_Interface::OnBtnJobChange9() 
{
	// TODO: Add your control notification handler code here
	
	if (st_serial.n_connect[2] == YES)
	{
		st_other.str_normal_msg =  _T("[Manual] BCR ML Button Click");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}

		mn_vision_hs_enable = 9;
		m_b_stop_req = FALSE;
		st_interface.b_ml_select = TRUE;
		st_interface.b_vision_ml_select = TRUE;	//20120625
		OnBtn_BCR_HS_YIELD_Enable_False();
		l_bcr_send_wait[0] = GetCurrentTime();

		m_list_bcr_ml.ResetContent(); //20120625


	//	::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, VISION_ML);
		Func.OnSet_Vision_send(VISION_ML);

		SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);			
	}
	else
	{
		st_other.str_normal_msg =  _T("[Manual] 통신포트를 연결하세요.");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}	
	}

}

void CDialog_Work_Interface::OnBtnJobChange10() 
{
	// TODO: Add your control notification handler code here
	if (st_serial.n_connect[2] == YES)
	{
		st_other.str_normal_msg =  _T("[Manual] HS ML Button Click");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}

		mn_vision_hs_enable = 10;
		m_b_stop_req = FALSE;
		st_interface.b_ml_select = TRUE;
		st_interface.b_hs_ml_select = TRUE;	//20120622
		OnBtn_BCR_HS_YIELD_Enable_False();
		l_bcr_send_wait[0] = GetCurrentTime();

		m_list_hs_ml.ResetContent();

	//	::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_ML);	
		Func.OnSet_Vision_send(HEATSINK_ML);

		SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);			
	}
	else
	{
		st_other.str_normal_msg =  _T("[Manual] 통신포트를 연결하세요.");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}
		
	}	
}

LRESULT CDialog_Work_Interface::OnWork_Interface_Info_Display(WPARAM wParam,LPARAM lParam) 
{
	int mn_state = wParam;
	
	switch(mn_state)
	{	
	case INTERFACE_BCR_DISPLAY:
		OnInterface_BCR_DISPLAY(lParam);
		break;

	default:
		break;
	}

	return 0;
}

void CDialog_Work_Interface::OnInterface_BCR_DISPLAY(int n_state)
{
	CString str_yield, str_yield2, str_yield3, str_yield4;
	CString str_ml, str_ml2, str_ml3, str_ml4;	// 20120618
	int i;

	switch(n_state)
	{
	case 0:
		str_yield.Format("%d", st_handler.mn_vision_yield_yes[0]);
		((CEdit*)GetDlgItem(IDC_EDIT_VISION_YIELD_YES))->SetWindowText(str_yield);
		str_yield2.Format("%d", st_handler.mn_vision_yield_yes[1]);
		((CEdit*)GetDlgItem(IDC_EDIT_VISION_YIELD_YES2))->SetWindowText(str_yield2);
		
		st_handler.mn_vision_yield_yes[0] = 0;
		st_handler.mn_vision_yield_yes[1] = 0;
		break;
	
	case 1:
		str_yield3.Format("%d", st_handler.mn_hs_yield_yes[0]);
		((CEdit*)GetDlgItem(IDC_EDIT_HS_YIELD_YES))->SetWindowText(str_yield3);
		str_yield4.Format("%d", st_handler.mn_hs_yield_yes[1]);
		((CEdit*)GetDlgItem(IDC_EDIT_HS_YIELD_YES2))->SetWindowText(str_yield4);
		
		st_handler.mn_hs_yield_yes[0] = 0;
		st_handler.mn_hs_yield_yes[1] = 0;
		break;

	case 2:
		for (i = 1; i < (st_handler.mn_vision_ml_cnt + 1); i++)
		{
			if (st_handler.str_vision_ml_yes[i] != "")
			{
				m_list_bcr_ml.AddString(st_handler.str_vision_ml_yes[i]);				
			}
		}

		break;
	case 3:
		for (i = 1; i < (st_handler.mn_hs_ml_cnt + 1); i++)
		{
			if (st_handler.str_hs_ml_yes[i] != "")
			{
				m_list_hs_ml.AddString(st_handler.str_hs_ml_yes[i]);				
			}
		}
		
		break;
	}
}

void CDialog_Work_Interface::OnBtnModelBcrTest() 
{
	st_other.str_normal_msg =  _T("[Manual] BCR Triger1 Test");
	sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
	}
	
	m_bcr_triger_step = 100;
	m_b_stop_req = FALSE;
	OnBtn_Enable_False();

	SetTimer(TM_MANUAL_BCR_TRIGER_TEST, 1000, 0);		
	
}
void CDialog_Work_Interface::OnBtnModelBcrTest2() 
{
	// TODO: Add your control notification handler code here
	st_other.str_normal_msg =  _T("[Manual] BCR Triger2 Test");
	sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
	}
	
	m_bcr_triger_step2 = 100;
	m_b_stop_req = FALSE;
	OnBtn_Enable_False();

	SetTimer(TM_MANUAL_BCR_TRIGER_TEST2, 1000, 0);		
	
}

void CDialog_Work_Interface::OnBtn_Enable_True() 
{
	m_btn_bcr_triger_test.EnableWindow(TRUE);
	m_btn_bcr_triger_test2.EnableWindow(TRUE);
	m_btn_bcr_triger_test3.EnableWindow(TRUE);
	m_btn_bcr_triger_test4.EnableWindow(TRUE);
	m_btn_client_ip_1.EnableWindow(TRUE);
	m_btn_client_port_1.EnableWindow(TRUE);
	m_btn_sec_server_port.EnableWindow(TRUE);
	m_btn_etc_server_port.EnableWindow(TRUE);
	m_btn_serial_apply_1.EnableWindow(TRUE);
	m_btn_serial_apply_2.EnableWindow(TRUE);
	m_btn_serial_apply_3.EnableWindow(TRUE);
	m_btn_serial_apply_4.EnableWindow(TRUE);
	m_btn_serial_apply_5.EnableWindow(TRUE);
	m_btn_serial_connect_1.EnableWindow(TRUE);
	m_btn_serial_connect_2.EnableWindow(TRUE);
	m_btn_serial_connect_3.EnableWindow(TRUE);
	m_btn_serial_connect_4.EnableWindow(TRUE);
	m_btn_serial_connect_5.EnableWindow(TRUE);
	m_btn_apply.EnableWindow(TRUE);
	m_btn_jobchange.EnableWindow(TRUE);
	m_btn_jobchange2.EnableWindow(TRUE);
	m_btn_jobchange3.EnableWindow(TRUE);
	m_btn_jobchange4.EnableWindow(TRUE);
	m_btn_jobchange5.EnableWindow(TRUE);
	m_btn_jobchange6.EnableWindow(TRUE);
	m_btn_jobchange7.EnableWindow(TRUE);
	m_btn_jobchange8.EnableWindow(TRUE);
	m_btn_jobchange9.EnableWindow(TRUE);	//20120618
	m_btn_jobchange10.EnableWindow(TRUE);	//20120618

}

void CDialog_Work_Interface::OnBtn_Enable_False() 
{
	m_btn_bcr_triger_test.EnableWindow(FALSE);
	m_btn_bcr_triger_test2.EnableWindow(FALSE);
	m_btn_bcr_triger_test3.EnableWindow(FALSE);
	m_btn_bcr_triger_test4.EnableWindow(FALSE);
	m_btn_client_ip_1.EnableWindow(FALSE);
	m_btn_client_port_1.EnableWindow(FALSE);
	m_btn_sec_server_port.EnableWindow(FALSE);
	m_btn_etc_server_port.EnableWindow(FALSE);
	m_btn_serial_apply_1.EnableWindow(FALSE);
	m_btn_serial_apply_2.EnableWindow(FALSE);
	m_btn_serial_apply_3.EnableWindow(FALSE);
	m_btn_serial_apply_4.EnableWindow(FALSE);
	m_btn_serial_apply_5.EnableWindow(FALSE);
	m_btn_serial_connect_1.EnableWindow(FALSE);
	m_btn_serial_connect_2.EnableWindow(FALSE);
	m_btn_serial_connect_3.EnableWindow(FALSE);
	m_btn_serial_connect_4.EnableWindow(FALSE);
	m_btn_serial_connect_5.EnableWindow(FALSE);
	m_btn_apply.EnableWindow(FALSE);
	m_btn_jobchange.EnableWindow(FALSE);
	m_btn_jobchange2.EnableWindow(FALSE);
	m_btn_jobchange3.EnableWindow(FALSE);
	m_btn_jobchange4.EnableWindow(FALSE);
	m_btn_jobchange5.EnableWindow(FALSE);
	m_btn_jobchange6.EnableWindow(FALSE);
	m_btn_jobchange7.EnableWindow(FALSE);
	m_btn_jobchange8.EnableWindow(FALSE);
	m_btn_jobchange9.EnableWindow(FALSE);	//20120618
	m_btn_jobchange10.EnableWindow(FALSE);	//20120618	

}

void CDialog_Work_Interface::OnBtn_BCR_HS_YIELD_Enable_True() 	//20120612
{
	m_btn_bcr_triger_test.EnableWindow(TRUE);
	m_btn_bcr_triger_test2.EnableWindow(TRUE);
	m_btn_bcr_triger_test3.EnableWindow(TRUE);
	m_btn_bcr_triger_test4.EnableWindow(TRUE);
	m_btn_client_ip_1.EnableWindow(TRUE);
	m_btn_client_port_1.EnableWindow(TRUE);
	m_btn_sec_server_port.EnableWindow(TRUE);
	m_btn_etc_server_port.EnableWindow(TRUE);
	m_btn_serial_apply_1.EnableWindow(TRUE);
	m_btn_serial_apply_2.EnableWindow(TRUE);
	m_btn_serial_apply_3.EnableWindow(TRUE);
	m_btn_serial_apply_4.EnableWindow(TRUE);
	m_btn_serial_apply_5.EnableWindow(TRUE);
	m_btn_serial_connect_1.EnableWindow(TRUE);
	m_btn_serial_connect_2.EnableWindow(TRUE);
	m_btn_serial_connect_3.EnableWindow(TRUE);
	m_btn_serial_connect_4.EnableWindow(TRUE);
	m_btn_serial_connect_5.EnableWindow(TRUE);
	m_btn_apply.EnableWindow(TRUE);
	m_btn_jobchange.EnableWindow(TRUE);
	m_btn_jobchange2.EnableWindow(TRUE);
	m_btn_jobchange3.EnableWindow(TRUE);
	m_btn_jobchange4.EnableWindow(TRUE);
	m_btn_jobchange5.EnableWindow(TRUE);
	m_btn_jobchange6.EnableWindow(TRUE);
	m_btn_jobchange7.EnableWindow(TRUE);
	m_btn_jobchange8.EnableWindow(TRUE);
	m_btn_jobchange9.EnableWindow(TRUE);	//20120618
	m_btn_jobchange10.EnableWindow(TRUE);	//20120618

}
	

void CDialog_Work_Interface::OnBtn_BCR_HS_YIELD_Enable_False() 	//20120612
{
	m_btn_bcr_triger_test.EnableWindow(FALSE);
	m_btn_bcr_triger_test2.EnableWindow(FALSE);
	m_btn_bcr_triger_test3.EnableWindow(FALSE);
	m_btn_bcr_triger_test4.EnableWindow(FALSE);	
	m_btn_client_ip_1.EnableWindow(FALSE);
	m_btn_client_port_1.EnableWindow(FALSE);
	m_btn_sec_server_port.EnableWindow(FALSE);
	m_btn_etc_server_port.EnableWindow(FALSE);
	m_btn_serial_apply_1.EnableWindow(FALSE);
	m_btn_serial_apply_2.EnableWindow(FALSE);
	m_btn_serial_apply_3.EnableWindow(FALSE);
	m_btn_serial_apply_4.EnableWindow(FALSE);
	m_btn_serial_apply_5.EnableWindow(FALSE);
	m_btn_serial_connect_1.EnableWindow(FALSE);
	m_btn_serial_connect_2.EnableWindow(FALSE);
	m_btn_serial_connect_3.EnableWindow(FALSE);
	m_btn_serial_connect_4.EnableWindow(FALSE);
	m_btn_serial_connect_5.EnableWindow(FALSE);
	m_btn_apply.EnableWindow(FALSE);

	if (mn_vision_hs_enable == 1)
	{
		m_btn_jobchange.EnableWindow(TRUE);	
	}
	else
	{
		m_btn_jobchange.EnableWindow(FALSE);	
	}
	if (mn_vision_hs_enable == 2)
	{
		m_btn_jobchange2.EnableWindow(TRUE);	
	}
	else
	{
		m_btn_jobchange2.EnableWindow(FALSE);	
	}
	if (mn_vision_hs_enable == 3)
	{
		m_btn_jobchange3.EnableWindow(TRUE);	
	}
	else
	{
		m_btn_jobchange3.EnableWindow(FALSE);	
	}
	if (mn_vision_hs_enable == 4)
	{
		m_btn_jobchange4.EnableWindow(TRUE);	
	}
	else
	{
		m_btn_jobchange4.EnableWindow(FALSE);	
	}	
	if (mn_vision_hs_enable == 5)
	{
		m_btn_jobchange5.EnableWindow(TRUE);	
	}
	else
	{
		m_btn_jobchange5.EnableWindow(FALSE);	
	}
	if (mn_vision_hs_enable == 6)
	{
		m_btn_jobchange6.EnableWindow(TRUE);	
	}
	else
	{
		m_btn_jobchange6.EnableWindow(FALSE);	
	}
	if (mn_vision_hs_enable == 7)
	{
		m_btn_jobchange7.EnableWindow(TRUE);	
	}
	else
	{
		m_btn_jobchange7.EnableWindow(FALSE);	
	}
	if (mn_vision_hs_enable == 8)
	{
		m_btn_jobchange8.EnableWindow(TRUE);	
	}
	else
	{
		m_btn_jobchange8.EnableWindow(FALSE);	
	}

	if (mn_vision_hs_enable == 9)
	{
		m_btn_jobchange9.EnableWindow(TRUE);	
	}
	else
	{
		m_btn_jobchange9.EnableWindow(FALSE);	
	}
	if (mn_vision_hs_enable == 10)
	{
		m_btn_jobchange10.EnableWindow(TRUE);	
	}
	else
	{
		m_btn_jobchange10.EnableWindow(FALSE);	
	}
}

int CDialog_Work_Interface::Manual_Bcr_HS_Yield_Test()
{
	int	FuncRet = RET_PROCEED;

	if(m_b_stop_req == TRUE)
	{
		st_other.b_yield_ok = FALSE;
		st_other.b_ml_ok = FALSE;	//20120618
		FuncRet = RET_ERROR;
		sprintf(st_msg.c_normal_msg, "BCR & HS Yield 테스트 에러!! - Stop 버튼 누름");
		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
		{
			sprintf(st_msg.c_normal_msg, "BCR & HS Yield test error! - Stop button is pressed");
		}
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
	}

	if (mn_vision_hs_enable == 1 || mn_vision_hs_enable == 2 || mn_vision_hs_enable == 3 || mn_vision_hs_enable == 5 || mn_vision_hs_enable == 6 || mn_vision_hs_enable == 7)	
	{
		l_bcr_send_wait[1] = GetCurrentTime();
		l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
		if(l_bcr_send_wait[2] < 0)
		{
			l_bcr_send_wait[0] = GetCurrentTime();
		}
		if(l_bcr_send_wait[2] > 2000)
		{
			FuncRet = RET_GOOD;
		}
	}

	if (mn_vision_hs_enable == 4)	//bcr yield
	{
		l_bcr_send_wait[1] = GetCurrentTime();
		l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
		if(l_bcr_send_wait[2] < 0)
		{
			l_bcr_send_wait[0] = GetCurrentTime();
		}
		if(l_bcr_send_wait[2] > 5000)
		{
			if(	st_other.b_yield_ok == TRUE)
			{
				st_other.b_yield_ok = FALSE;
				FuncRet = RET_GOOD;
				sprintf(st_msg.c_normal_msg, "BCR Yield 응답 완료!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "Complete response BCR Yield!!");
				}
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
//				::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, VISION_YIELD_YES);
				Func.OnSet_Vision_send(VISION_YIELD_YES);
			}
			else
			{
				FuncRet = RET_ERROR;
				sprintf(st_msg.c_normal_msg, "BCR Yield 응답 에러!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Yield response error!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
		}
	}

	if (mn_vision_hs_enable == 8)	//hs yield
	{
		l_bcr_send_wait[1] = GetCurrentTime();
		l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
		if(l_bcr_send_wait[2] < 0)
		{
			l_bcr_send_wait[0] = GetCurrentTime();
		}
		if(l_bcr_send_wait[2] > 5000)
		{
			if(	st_other.b_yield_ok == TRUE)
			{
				st_other.b_yield_ok = FALSE;
				FuncRet = RET_GOOD;
				sprintf(st_msg.c_normal_msg, "HS Yield 응답 완료!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "Complete response HS Yield!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
//				::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_YIELD_YES);
				Func.OnSet_Vision_send(HEATSINK_YIELD_YES);
			}
			else
			{
				FuncRet = RET_ERROR;
				sprintf(st_msg.c_normal_msg, "HS Yield 응답 에러!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "HS Yield response error!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
		}
	}

	if (mn_vision_hs_enable == 9)	//bcr ml
	{
		l_bcr_send_wait[1] = GetCurrentTime();
		l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
		if(l_bcr_send_wait[2] < 0)
		{
			l_bcr_send_wait[0] = GetCurrentTime();
		}
		if(l_bcr_send_wait[2] > 300)
		{
			if(	st_other.b_ml_ok == TRUE)
			{
				st_other.b_ml_ok = FALSE;
				FuncRet = RET_GOOD;
				sprintf(st_msg.c_normal_msg, "BCR ML 응답 완료!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "Complete response BCR ML!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			else
			{
				FuncRet = RET_ERROR;
				sprintf(st_msg.c_normal_msg, "BCR ML 응답 에러!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR ML response error!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			st_interface.b_vision_ml_select = FALSE;
		}
	}
	if (mn_vision_hs_enable == 10)	//hs ml
	{
		l_bcr_send_wait[1] = GetCurrentTime();
		l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
		if(l_bcr_send_wait[2] < 0)
		{
			l_bcr_send_wait[0] = GetCurrentTime();
		}
		if(l_bcr_send_wait[2] > 300)
		{
			if(	st_other.b_ml_ok == TRUE)
			{
				st_other.b_ml_ok = FALSE;
				FuncRet = RET_GOOD;
				sprintf(st_msg.c_normal_msg, "HS ML 응답 완료!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "Complete response HS ML!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			else
			{
				FuncRet = RET_ERROR;
				sprintf(st_msg.c_normal_msg, "HS ML 응답 에러!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "HS ML response error!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			st_interface.b_hs_ml_select = FALSE;
		}
	}
	return FuncRet;
}

int CDialog_Work_Interface::Manual_Bcr_Triger_Test()
{
	int Ret=0, Ret_1=0, i=0; 
	int	FuncRet = RET_PROCEED;
	double dSpeedRatio[3] = {100,100,100};
	int ratio = 50;
	CString strClient;


	if(m_b_stop_req == TRUE)
	{
		FuncRet = RET_ERROR;
		m_bcr_triger_step = 0;
		OnBtn_Enable_True();
		sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 에러!! - Stop 버튼 누름");
		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
		{
			sprintf(st_msg.c_normal_msg, "BCR Trigger1 test error! - Stop button is pressed!!");
		}

		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
	}

	switch(m_bcr_triger_step)
	{
		case 0:
			break;

		case 100:	//BCR Trigger1 Ready check
			if(FAS_IO.Get_In_Bit(st_io.i_vision_ready1) == IO_IN)	//ready 상태 확인
			{
				sprintf(st_msg.c_normal_msg, "BCR Trigger1 Ready 상태!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 Ready status!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				FAS_IO.set_out_bit(st_io.o_vis_reset1_onoff, IO_ON);
				sprintf(st_msg.c_normal_msg, "BCR Reset1 신호 ON!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Reset1 signal ON!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				m_bcr_triger_step = 110;
			}
			else 
			{
				FuncRet = RET_ERROR;
				OnBtn_Enable_True();
				sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 에러!! - Trigger가 Ready 상태가 아님!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 test error! - Trigger Not Ready state!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				m_bcr_triger_step = 0;
			}
			break;

		case 110:	//Trigger On
			if(FAS_IO.Get_In_Bit(st_io.i_vision_ready1) == IO_IN)	//ready 상태 확인
			{
				if(FAS_IO.Get_In_Bit(st_io.i_vision_complste1) != IO_IN) //Complete 상태 확인
				{
					FAS_IO.set_out_bit(st_io.o_vis_reset1_onoff, IO_OFF);
					sprintf(st_msg.c_normal_msg, "BCR Reset1 신호 Off!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "BCR Reset1 signal Off!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

					FAS_IO.set_out_bit(st_io.o_vis_trigger1_onoff, IO_ON);
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 신호 ON!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "BCR Trigger1 signal ON!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					m_bcr_triger_step = 120;
				}
				else
				{
					FuncRet = RET_ERROR;
					OnBtn_Enable_True();
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 에러!! - Complete가 Off 상태가 아님!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "BCR Trigger1 test error! - Complete the Off state is not!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					m_bcr_triger_step = 0;	
				}
			}
			else
			{
				FuncRet = RET_ERROR;
				OnBtn_Enable_True();
				sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 에러!! - Trigger가 Ready 상태가 아님!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 test error! - Trigger Not Ready state!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				m_bcr_triger_step = 0;
			}
			break;
		
		case 120:  //Vision Complete & Fail check
			if(FAS_IO.Get_In_Bit(st_io.i_vision_complste1) == IO_IN) //Complete 상태 확인
			{
				sprintf(st_msg.c_normal_msg, "BCR Trigger1 Complete 완료!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 Complete!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				m_bcr_triger_step = 130;
			}
			else
			{
				FuncRet = RET_ERROR;
				OnBtn_Enable_True();
				sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 에러!! - Vision Complete 에러!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 test error! - Vision Complete error!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
	
				FAS_IO.set_out_bit(st_io.o_vis_trigger1_onoff, IO_OFF);
				sprintf(st_msg.c_normal_msg, "BCR Trigger1 신호 OFF!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 signal OFF!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				m_bcr_triger_step = 0;
			}
			break;
			
		case 130:  //Vision Complete & Fail check
			if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail1) == IO_IN)	//Fail 상태 확인
			{
				FuncRet = RET_GOOD;
				OnBtn_Enable_True();
				sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 !! - Fail 상태!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 test! - Fail status!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				FAS_IO.set_out_bit(st_io.o_vis_trigger1_onoff, IO_OFF);
				sprintf(st_msg.c_normal_msg, "BCR Trigger1 신호 OFF!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 signal OFF!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				m_bcr_triger_step = 0;
				
			}
			else if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail1) != IO_IN)	//성공 상태 확인
			{
				FuncRet = RET_GOOD;
				OnBtn_Enable_True();
				sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 !! - Success 상태!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 test! - Success Status!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				FAS_IO.set_out_bit(st_io.o_vis_trigger1_onoff, IO_OFF);
				sprintf(st_msg.c_normal_msg, "BCR Trigger1 신호 OFF!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 signal OFF!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				m_bcr_triger_step = 0;
			}

			break;

	}

 	return FuncRet;
}

int CDialog_Work_Interface::Manual_Bcr_Triger_Test2()
{
	int Ret=0, Ret_1=0, i=0; 
	int	FuncRet = RET_PROCEED;
	double dSpeedRatio[3] = {100,100,100};
	int ratio = 50;
	CString strClient;


	if(m_b_stop_req == TRUE)
	{
		FuncRet = RET_ERROR;
		m_bcr_triger_step2 = 0;
		OnBtn_Enable_True();
		sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 에러!! - Stop 버튼 누름");
		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
		{
			sprintf(st_msg.c_normal_msg, "BCR Trigger2 test error! - Stop button is pressed!!");
		}

		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
	}

	switch(m_bcr_triger_step2)
	{
		case 0:
			break;

		case 100:	//BCR Trigger2 Ready check
			if(FAS_IO.Get_In_Bit(st_io.i_vision_ready2) == IO_IN)	//ready 상태 확인
			{
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 Ready 상태!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 Ready status!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				FAS_IO.set_out_bit(st_io.o_vis_reset2_onoff, IO_ON);
				sprintf(st_msg.c_normal_msg, "BCR Reset2 신호 ON!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Reset2 signal ON!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				m_bcr_triger_step2 = 110;
			}
			else 
			{
				FuncRet = RET_ERROR;
				OnBtn_Enable_True();
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 에러!! - Trigger가 Ready 상태가 아님!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 test error! - Trigger Not Ready state!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				m_bcr_triger_step = 0;
			}
			break;

		case 110:	//Trigger On
			if(FAS_IO.Get_In_Bit(st_io.i_vision_ready2) == IO_IN)	//ready 상태 확인
			{
				if(FAS_IO.Get_In_Bit(st_io.i_vision_complste2) != IO_IN) //Complete 상태 확인
				{
					FAS_IO.set_out_bit(st_io.o_vis_reset2_onoff, IO_OFF);
					sprintf(st_msg.c_normal_msg, "BCR Reset2 신호 Off!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "BCR Reset2 signal Off!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

					FAS_IO.set_out_bit(st_io.o_vis_trigger2_onoff, IO_ON);
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 신호 ON!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "BCR Trigger2 signal ON!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					m_bcr_triger_step2 = 120;
				}
				else
				{
					FuncRet = RET_ERROR;
					OnBtn_Enable_True();
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 에러!! - Complete가 Off 상태가 아님!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "BCR Trigger2 test error! - Complete the Off state is not!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					m_bcr_triger_step2 = 0;	
				}
			}
			else
			{
				FuncRet = RET_ERROR;
				OnBtn_Enable_True();
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 에러!! - Trigger가 Ready 상태가 아님!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 test error! - Trigger Not Ready state!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				m_bcr_triger_step2 = 0;
				
			}
			break;
		
		case 120:  //Vision Complete & Fail check
			if(FAS_IO.Get_In_Bit(st_io.i_vision_complste2) == IO_IN) //Complete 상태 확인
			{
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 Complete 완료!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 Complete!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				m_bcr_triger_step2 = 130;
			}
			else
			{
				FuncRet = RET_ERROR;
				OnBtn_Enable_True();
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 에러!! - Vision Complete 에러!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 test error! - Vision Complete error!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				FAS_IO.set_out_bit(st_io.o_vis_trigger2_onoff, IO_OFF);
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 신호 OFF!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 signal OFF!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				m_bcr_triger_step2 = 0;
			}
			break;

		case 130:  //Vision Complete & Fail check
			if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail2) == IO_IN)	//Fail 상태 확인
			{
				FuncRet = RET_GOOD;
				OnBtn_Enable_True();
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 !! - Fail 상태!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 test! - Fail status!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				FAS_IO.set_out_bit(st_io.o_vis_trigger2_onoff, IO_OFF);
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 신호 OFF!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 signal OFF!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				m_bcr_triger_step2 = 0;
			}
			else if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail2) != IO_IN)	//성공 상태 확인
			{
				FuncRet = RET_GOOD;
				OnBtn_Enable_True();
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 !! - Success 상태!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 test! - Success Status!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				FAS_IO.set_out_bit(st_io.o_vis_trigger2_onoff, IO_OFF);
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 신호 OFF!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 signal OFF!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				m_bcr_triger_step2 = 0;
			}
			break;
	}

 	return FuncRet;
}

void CDialog_Work_Interface::OnSelchangeListBcrMl() 
{
	// TODO: Add your control notification handler code here
	CString str_selected_lbl;						// 선택된 디바이스 이름 저장 변수
	int n_response;				// 대화 상자 리턴 플래그
	CDialog_Select  select_dlg;
	
	int n_index = m_list_bcr_ml.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_bcr_ml.GetText(n_index, str_selected_lbl);
		str_bcr_ml_name[1] = str_selected_lbl;
		mn_bcr_ml_mode[1] = n_index;
	}


	st_msg.mstr_confirm_msg = _T(str_bcr_ml_name[1] + " Model List를 선택하시겠습니까?? ");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Are you sure you want to select Model List??" + str_bcr_ml_name[1]);
	}

	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		st_interface.str_bcr_ml_name = str_bcr_ml_name[1];
		st_interface.n_bcr_ml_mode = mn_bcr_ml_mode[1];

	}
	else if (n_response == IDCANCEL)
	{
		OnInterface_ML_BCR_Focus_Set();

		str_bcr_ml_name[1] = str_bcr_ml_name[0];
		mn_bcr_ml_mode[1] = mn_bcr_ml_mode[0];
	}
}

void CDialog_Work_Interface::OnSelchangeListHeatsinkMl() 
{
	// TODO: Add your control notification handler code here
	CString str_selected_lbl;						// 선택된 디바이스 이름 저장 변수
	int n_response;				// 대화 상자 리턴 플래그
	CDialog_Select  select_dlg;
	
	int n_index = m_list_hs_ml.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_hs_ml.GetText(n_index, str_selected_lbl);
		str_hs_ml_name[1] = str_selected_lbl;
		mn_hs_ml_mode[1] = n_index;
	}


	st_msg.mstr_confirm_msg = _T(str_hs_ml_name[1] + " Label을 선택하시겠습니까?? ");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Are you sure you want to select the Label??" + str_hs_ml_name[1]);
	}
	

	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		st_interface.str_hs_ml_name = str_hs_ml_name[1];
		st_interface.n_hs_ml_mode = mn_hs_ml_mode[1];

	}
	else if (n_response == IDCANCEL)
	{
		OnInterface_ML_HS_Focus_Set();

		str_hs_ml_name[1] = str_hs_ml_name[0];
		mn_hs_ml_mode[1] = mn_hs_ml_mode[0];
	}		
}

void CDialog_Work_Interface::BCR_Ml_Focus_Set()
{
	if (mn_bcr_ml_name == -1)
		return;
	
	m_list_bcr_ml.SetCurSel(mn_bcr_ml_name);
	
}
void CDialog_Work_Interface::HS_Ml_Focus_Set()
{
	if (mn_hs_ml_name == -1)
		return;
	
	m_list_hs_ml.SetCurSel(mn_hs_ml_name);
	
}

///////////////////////////////////////////////////////////////////

void CDialog_Work_Interface::OnBtnModelBcrTest3() 
{
	// TODO: Add your control notification handler code here
	FAS_IO.set_out_bit(st_io.o_vis_reset1_onoff, IO_ON);
	sprintf(st_msg.c_normal_msg, "BCR Reset1 신호 ON!!");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		sprintf(st_msg.c_normal_msg, "BCR Reset1 signal ON");
	}

	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

	Sleep(10);
	FAS_IO.set_out_bit(st_io.o_vis_reset1_onoff, IO_OFF);
	sprintf(st_msg.c_normal_msg, "BCR Reset1 신호 OFF!!");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		sprintf(st_msg.c_normal_msg, "BCR Reset1 signal OFF!!");
	}
				

	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
	
}

void CDialog_Work_Interface::OnBtnModelBcrTest4() 
{
	// TODO: Add your control notification handler code here
	FAS_IO.set_out_bit(st_io.o_vis_reset2_onoff, IO_ON);
	sprintf(st_msg.c_normal_msg, "BCR Reset2 신호 ON!!");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		sprintf(st_msg.c_normal_msg, "BCR Reset2 signal ON!!");
	}

	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

	Sleep(10);
	FAS_IO.set_out_bit(st_io.o_vis_reset2_onoff, IO_OFF);
	sprintf(st_msg.c_normal_msg, "BCR Reset2 신호 OFF!!");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		sprintf(st_msg.c_normal_msg, "BCR Reset2 signal OFF!!");
	}

	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
	
}


int CDialog_Work_Interface::OnInterface_ML_BCR_Init_List(LPCTSTR pszPath)
{
	int n_chk;						// 함수 리턴 플래그
    CString str_path = pszPath;		// 폴더 설정
	
	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// 리스트 박스에 추가한 목록 갯수 저장 변수

	mn_bcr_ml_name = -1;
	
	if (str_path.Right (1) != "\\")
        str_path += "\\";
	
	str_path += "*.*";
    
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			n_chk = ML_BCR_AddItem(n_count, &fd);
			if (n_chk == TRUE)
				n_count++;
        }
		
        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				n_chk = ML_BCR_AddItem(n_count, &fd);
				if (n_chk == FALSE) 
					break;
			}
			if (n_chk != CTL_READY)
				n_count++;
        }
		::FindClose(hFind);
    }
	
	return n_count;
}

int CDialog_Work_Interface::OnInterface_ML_HS_Init_List(LPCTSTR pszPath)
{
	int n_chk;						// 함수 리턴 플래그
    CString str_path = pszPath;		// 폴더 설정
	
	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// 리스트 박스에 추가한 목록 갯수 저장 변수

	mn_hs_ml_name = -1;
	
	if (str_path.Right (1) != "\\")
        str_path += "\\";
	
	str_path += "*.*";
    
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			n_chk = ML_BCR_AddItem(n_count, &fd);
			if (n_chk == TRUE)
				n_count++;
        }
		
        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				n_chk = ML_BCR_AddItem(n_count, &fd);
				if (n_chk == FALSE) 
					break;
			}
			if (n_chk != CTL_READY)
				n_count++;
        }
		::FindClose(hFind);
    }
	
	return n_count;
}


BOOL CDialog_Work_Interface::ML_BCR_AddItem(int nIndex, WIN32_FIND_DATA *pfd)
{
	CString str_temp_ml;
	CString str_ml;
	CString str_expansion;
	int n_pos;
	
	HICON	hIcon = NULL;
	
	str_temp_ml = (pfd->cFileName);	// 파일 이름 설정
	
	str_temp_ml.MakeUpper();
	
	// **************************************************************************
	// 파일 이름 중 확장자 확인
	// **************************************************************************
	n_pos = str_temp_ml.Find('.');		// 확장자 위치 설정
	
	if (n_pos == -1)					// 확장자 미존재
		str_expansion = "";
	else								// 확장자 존재
		str_expansion = str_temp_ml.Mid(n_pos + 1);
	// **************************************************************************
	
	if (str_expansion != "TXT")
	{
		return CTL_READY;	
	}
	
	str_ml=str_temp_ml;			// 파일명 전체 화면 출력
	
	if (str_ml == str_bcr_ml_name[1])
		mn_bcr_ml_name = nIndex-1;
	// **************************************************************************
	
// 	hIcon = AfxGetApp()->LoadIcon(IDI_MODULE);
// 	mp_device_image_list.Add(hIcon);
	
    if (m_list_bcr_ml.AddString(_T(str_ml), nIndex - 1) == -1)
		return FALSE;
	
    return TRUE;
}

BOOL CDialog_Work_Interface::ML_HS_AddItem(int nIndex, WIN32_FIND_DATA *pfd)
{
	CString str_temp_ml;
	CString str_ml;
	CString str_expansion;
	int n_pos;
	
	HICON	hIcon = NULL;
	
	str_temp_ml = (pfd->cFileName);	// 파일 이름 설정
	
	str_temp_ml.MakeUpper();
	
	// **************************************************************************
	// 파일 이름 중 확장자 확인
	// **************************************************************************
	n_pos = str_temp_ml.Find('.');		// 확장자 위치 설정
	
	if (n_pos == -1)					// 확장자 미존재
		str_expansion = "";
	else								// 확장자 존재
		str_expansion = str_temp_ml.Mid(n_pos + 1);
	// **************************************************************************
	
	if (str_expansion != "TXT")
	{
		return CTL_READY;	
	}
	
	str_ml=str_temp_ml;			// 파일명 전체 화면 출력
	
	if (str_ml == str_hs_ml_name[1])
		mn_hs_ml_name = nIndex-1;
	// **************************************************************************
	
// 	hIcon = AfxGetApp()->LoadIcon(IDI_MODULE);
// 	mp_device_image_list.Add(hIcon);
	
    if (m_list_hs_ml.AddString(_T(str_ml), nIndex - 1) == -1)
		return FALSE;
	
    return TRUE;
}

void CDialog_Work_Interface::OnInterface_ML_BCR_Focus_Set()
{
	if (mn_bcr_ml_name == -1)
		return;
	
	m_list_bcr_ml.SetCurSel(mn_bcr_ml_name);
	
}

void CDialog_Work_Interface::OnInterface_ML_HS_Focus_Set()
{
	if (mn_hs_ml_name == -1)
		return;
	
	m_list_hs_ml.SetCurSel(mn_hs_ml_name);
	
}

void CDialog_Work_Interface::OnBtnBcrRead() 
{
	// TODO: Add your control notification handler code here
	//if(Func.OnBCR_Command(BCR_READON, &str_command))
	//{
	//	st_serial.n_snd_chk[HEATSINK_RIGHT_PORT] = COM_READY;
	//	st_serial.str_snd[HEATSINK_RIGHT_PORT] = str_command;                  // 송신 데이터 설정

	//	::SendMessage(st_handler.hWnd, WM_DATA_SEND, HEATSINK_RIGHT_PORT, 0);  // 데이터 송신 요청
	//}

	SetTimer(TM_LEFTHS_BCR_TEST, 100, NULL);
}
