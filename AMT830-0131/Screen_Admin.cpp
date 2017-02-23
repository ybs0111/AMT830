// Screen_Admin.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Admin.h"

// ******************************************************************************
// 대화 상자 클래스 추가
// ******************************************************************************
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyBoard.h"

#include "Dialog_KeyPad.h"
// ******************************************************************************

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin

IMPLEMENT_DYNCREATE(CScreen_Admin, CFormView)

CScreen_Admin::CScreen_Admin()
	: CFormView(CScreen_Admin::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Admin)
	//}}AFX_DATA_INIT
}

CScreen_Admin::~CScreen_Admin()
{
}

void CScreen_Admin::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Admin)
	DDX_Control(pDX, IDC_CHK_VIRTUAL, m_chk_Virtual);
	DDX_Control(pDX, IDC_CHK_NTO_FAIL, m_chk_nto_fail);
	DDX_Control(pDX, IDC_CHK_HTO_FAIL, m_chk_hto_fail);
	DDX_Control(pDX, IDC_CHK_BTO_FAIL, m_chk_bto_fail);
	DDX_Control(pDX, IDC_CHK_ULD_CONVAYOR, m_chk_uld_convayor);
	DDX_Control(pDX, IDC_BTN_BUFFERDB_DELETE, m_btn_bufferdb_delete);
	DDX_Control(pDX, IDC_CHK_REVERSE, m_chk_reverse);
	DDX_Control(pDX, IDC_BTN_APCT_BLOT_TEST, m_btn_acpt_blot_test);
	DDX_Control(pDX, IDC_CHK_BARCODE, m_chk_barcode);
	DDX_Control(pDX, IDC_CHK_DEBUG, m_chk_debug);
	DDX_Control(pDX, IDC_BTN_SOCKET_CHK_TEST, m_btn_socket_chk_test);
	DDX_Control(pDX, IDC_BTN_BUFFER_IN_TEST, m_btn_buffer_in_test);
	DDX_Control(pDX, IDC_BTN_APCT_SOCKET_INFO_TEST, m_btn_apct_socket_info_test);
	DDX_Control(pDX, IDC_BTN_BUFFER_READY_TEST, m_btn_buffer_ready_test);
	DDX_Control(pDX, IDC_MSG_SERVER_PORT_FMACHINE, m_msg_server_port_fmachine);
	DDX_Control(pDX, IDC_BTN_SERVER_PORT_SAVE_FMACHINE, m_btn_server_port_save_fmachine);
	DDX_Control(pDX, IDC_MSG_SERVER_PORT_APCT, m_msg_server_port_apct);
	DDX_Control(pDX, IDC_BTN_SERVER_PORT_SAVE_APCT, m_btn_server_port_save_apct);
	DDX_Control(pDX, IDC_MSG_PORT_APCT, m_msg_port_apct);
	DDX_Control(pDX, IDC_MSG_IPADDRESS_APCT, m_msg_ipaddress_apct);
	DDX_Control(pDX, IDC_IPADDRESS_APCT, m_ipaddress_apct);
	DDX_Control(pDX, IDC_GROUP_NEXT_APCT_SET, m_group_next_apct_set);
	DDX_Control(pDX, IDC_BTN_TEST_APCT, m_btn_test_apct);
	DDX_Control(pDX, IDC_BTN_PORT_SAVE_APCT, m_btn_port_save_apct);
	DDX_Control(pDX, IDC_BTN_IP_SAVE_APCT, m_btn_ip_save_apct);
	DDX_Control(pDX, IDC_BTN_CONNECT_APCT, m_btn_connect_apct);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Admin, CFormView)
	//{{AFX_MSG_MAP(CScreen_Admin)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_IP_SAVE_APCT, OnBtnIpSaveApct)
	ON_BN_CLICKED(IDC_BTN_PORT_SAVE_APCT, OnBtnPortSaveApct)
	ON_BN_CLICKED(IDC_BTN_SERVER_PORT_SAVE_APCT, OnBtnServerPortSaveApct)
	ON_BN_CLICKED(IDC_BTN_SERVER_PORT_SAVE_FMACHINE, OnBtnServerPortSaveFmachine)
	ON_BN_CLICKED(IDC_BTN_BUFFER_READY_TEST, OnBtnBufferReadyTest)
	ON_BN_CLICKED(IDC_BTN_BUFFER_IN_TEST, OnBtnBufferInTest)
	ON_BN_CLICKED(IDC_BTN_SOCKET_CHK_TEST, OnBtnSocketChkTest)
	ON_BN_CLICKED(IDC_BTN_APCT_SOCKET_INFO_TEST, OnBtnApctSocketInfoTest)
	ON_BN_CLICKED(IDC_CHK_DEBUG, OnChkDebug)
	ON_BN_CLICKED(IDC_CHK_BARCODE, OnChkBarcode)
	ON_BN_CLICKED(IDC_BTN_APCT_BLOT_TEST, OnBtnApctBlotTest)
	ON_BN_CLICKED(IDC_DGT_EJECT_COUNT, OnDgtEjectCount)
	ON_BN_CLICKED(IDC_DGT_PICK_COUNT, OnDgtPickCount)
	ON_BN_CLICKED(IDC_CHK_REVERSE, OnChkReverse)
	ON_BN_CLICKED(IDC_BTN_BUFFERDB_DELETE, OnBtnBufferdbDelete)
	ON_BN_CLICKED(IDC_CHK_ULD_CONVAYOR, OnChkUldConvayor)
	ON_BN_CLICKED(IDC_CHK_NTO_FAIL, OnChkNtoFail)
	ON_BN_CLICKED(IDC_CHK_HTO_FAIL, OnChkHtoFail)
	ON_BN_CLICKED(IDC_CHK_BTO_FAIL, OnChkBtoFail)
	ON_BN_CLICKED(IDC_CHK_VIRTUAL, OnChkVirtual)
	ON_BN_CLICKED(IDC_BTN_FILE_EDITOR, OnBtnFileEditor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin diagnostics

#ifdef _DEBUG
void CScreen_Admin::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Admin::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin message handlers

void CScreen_Admin::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다
	// **************************************************************************
	mp_admin_font = NULL;
	mp_admin_font = new CFont;
	mp_admin_font->CreateFont(15,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	// **************************************************************************

	mn_buffer_ready_step = 0;
	mn_buffer_in_step = 0;
	mn_apct_socket_info_step = 0;
	mn_apct_blot_step = 0;

	OnAdmin_GroupBox_Set();
	OnAdmin_EditBox_Set();
	OnAdmin_Label_Set();

	OnAdmin_Data_Set();

	OnAdmin_Dgt_Set();
}

void CScreen_Admin::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다
	// **************************************************************************
	delete mp_admin_font;
	mp_admin_font = NULL;
	// **************************************************************************

	CFormView::OnDestroy();
}

void CScreen_Admin::OnTimer(UINT nIDEvent) 
{
	int n_response;

	if (nIDEvent == TMR_BUFFERREADY)
	{
		n_response = Buffer_Ready();
		
		if (n_response == RET_GOOD)
		{
			mn_buffer_ready_step = 0;
			KillTimer(TMR_BUFFERREADY);
		}
		else
		{
			mlWait_Time[1] = GetCurrentTime();
			mlWait_Time[2] = mlWait_Time[1] - mlWait_Time[0];
			
			if (mlWait_Time[2] > 5000)
			{
				mn_buffer_ready_step = 0;
				KillTimer(TMR_BUFFERREADY);
			}
		}
	}
	else if (nIDEvent == TMR_BUFFERIN)
	{
		n_response = Buffer_In();
		
		if (n_response == RET_GOOD)
		{
			mn_buffer_in_step = 0;
			KillTimer(TMR_BUFFERIN);
		}
		else
		{
			mlWait_Time[1] = GetCurrentTime();
			mlWait_Time[2] = mlWait_Time[1] - mlWait_Time[0];
			
			if (mlWait_Time[2] > 5000)
			{
				mn_buffer_in_step = 0;
				KillTimer(TMR_BUFFERIN);
			}
		}
	}
	else if (nIDEvent == TMR_APCTSOCKETINFO)
	{
		n_response = APCT_Socket_Info();
		
		if (n_response == RET_GOOD)
		{
			mn_apct_socket_info_step = 0;
			KillTimer(TMR_APCTSOCKETINFO);
		}
		else
		{
			mlWait_Time[1] = GetCurrentTime();
			mlWait_Time[2] = mlWait_Time[1] - mlWait_Time[0];
			
			if (mlWait_Time[2] > 5000)
			{
				mn_apct_socket_info_step = 0;
				KillTimer(TMR_APCTSOCKETINFO);
			}
		}
	}
	else if (nIDEvent == TMR_APCTBLOT)
	{
		n_response = APCT_BLot();
		
		if (n_response == RET_GOOD)
		{
			mn_apct_blot_step = 0;
			KillTimer(TMR_APCTBLOT);
		}
		else
		{
			mlWait_Time[1] = GetCurrentTime();
			mlWait_Time[2] = mlWait_Time[1] - mlWait_Time[0];
			
			if (mlWait_Time[2] > 5000)
			{
				mn_apct_blot_step = 0;
				KillTimer(TMR_APCTBLOT);
			}
		}
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Admin::OnAdmin_GroupBox_Set()
{
	CSxLogFont admin_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_next_apct_set.SetFont(admin_font);
	m_group_next_apct_set.SetCatptionTextColor(RGB(145,25,0));
	m_group_next_apct_set.SetFontBold(TRUE);
}

void CScreen_Admin::OnAdmin_EditBox_Set()
{
	m_edit_port_apct.SubclassDlgItem(IDC_EDIT_PORT_APCT, this);
	m_edit_port_apct.bkColor(RGB(50, 100, 150));
	m_edit_port_apct.textColor(RGB(255, 255,255));
	m_edit_port_apct.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_server_port_apct.SubclassDlgItem(IDC_EDIT_SERVER_PORT_APCT, this);
	m_edit_server_port_apct.bkColor(RGB(50, 100, 150));
	m_edit_server_port_apct.textColor(RGB(255, 255,255));
	m_edit_server_port_apct.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_server_port_fmachine.SubclassDlgItem(IDC_EDIT_SERVER_PORT_FMACHINE, this);
	m_edit_server_port_fmachine.bkColor(RGB(50, 100, 150));
	m_edit_server_port_fmachine.textColor(RGB(255, 255,255));
	m_edit_server_port_fmachine.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");
}

void CScreen_Admin::OnAdmin_Label_Set()
{
	m_msg_port_apct.SetFont(mp_admin_font);
	m_msg_port_apct.SetWindowText(_T("Port"));
	m_msg_port_apct.SetCenterText();
	m_msg_port_apct.SetColor(RGB(0,0,255));
	m_msg_port_apct.SetGradientColor(RGB(0,0,0));
	m_msg_port_apct.SetTextColor(RGB(255,255,255));
	
	m_msg_ipaddress_apct.SetFont(mp_admin_font);
	m_msg_ipaddress_apct.SetWindowText(_T("IP Address"));
	m_msg_ipaddress_apct.SetCenterText();
	m_msg_ipaddress_apct.SetColor(RGB(0,0,255));
	m_msg_ipaddress_apct.SetGradientColor(RGB(0,0,0));
	m_msg_ipaddress_apct.SetTextColor(RGB(255,255,255));

	m_msg_server_port_apct.SetFont(mp_admin_font);
	m_msg_server_port_apct.SetWindowText(_T("APCT Server Port"));
	m_msg_server_port_apct.SetCenterText();
	m_msg_server_port_apct.SetColor(RGB(0,0,255));
	m_msg_server_port_apct.SetGradientColor(RGB(0,0,0));
	m_msg_server_port_apct.SetTextColor(RGB(255,255,255));

	m_msg_server_port_fmachine.SetFont(mp_admin_font);
	m_msg_server_port_fmachine.SetWindowText(_T("Front Machine Server Port"));
	m_msg_server_port_fmachine.SetCenterText();
	m_msg_server_port_fmachine.SetColor(RGB(0,0,255));
	m_msg_server_port_fmachine.SetGradientColor(RGB(0,0,0));
	m_msg_server_port_fmachine.SetTextColor(RGB(255,255,255));
}

void CScreen_Admin::OnAdmin_Data_Set()
{
	CString str_apct_ip, str_apct_port;
	CString str_apct_server_port, str_fmachine_server_port;

	str_apct_ip = st_Network[NETWORK_APCT].mstr_ip;
	str_apct_port = st_Network[NETWORK_APCT].mstr_port;

	str_apct_server_port = st_Network[NETWORK_APCT].mstr_server_port;
	str_fmachine_server_port = st_Network[NETWORK_NMACHINE].mstr_server_port;

	OnIP_Address(str_apct_ip, NETWORK_APCT);
	m_ipaddress_apct.SetAddress(nIPFiled0[NETWORK_APCT], nIPFiled1[NETWORK_APCT], nIPFiled2[NETWORK_APCT], nIPFiled3[NETWORK_APCT]);
	
	((CEdit*)GetDlgItem(IDC_EDIT_PORT_APCT))->SetWindowText(str_apct_port);

	((CEdit*)GetDlgItem(IDC_EDIT_SERVER_PORT_APCT))->SetWindowText(str_apct_server_port);
	((CEdit*)GetDlgItem(IDC_EDIT_SERVER_PORT_FMACHINE))->SetWindowText(str_fmachine_server_port);


	m_chk_debug.SetCheck(st_handler.mn_debug_mode);
	m_chk_reverse.SetCheck( st_handler.mn_reverse_mode );
	m_chk_uld_convayor.SetCheck(st_handler.n_uld_convayor);
	m_chk_Virtual.SetCheck( st_handler.mn_virtual_mode );
	

	m_chk_nto_fail.SetCheck(st_handler.n_nto_fail);	
	m_chk_hto_fail.SetCheck(st_handler.n_hto_fail);	
	m_chk_bto_fail.SetCheck(st_handler.n_bto_fail);
}

void CScreen_Admin::OnBtnIpSaveApct() 
{
	int n_response;  // 대화 상자에 대한 리턴 값 저장 변수
	CString str;
	BYTE nFiled0, nFiled1, nFiled2, nFiled3;
	
	CDialog_Keyboard keyboard_dlg;
	CDialog_Message msg_dlg;

	CString mstr_apct_ip;
	
	if (st_handler.mn_level_teach != TRUE)
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");

		if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		n_response = msg_dlg.DoModal();
		return;
	}
	
	m_ipaddress_apct.GetAddress(nFiled0 , nFiled1, nFiled2, nFiled3);
	str.Format("%d.%d.%d.%d", nFiled0, nFiled1, nFiled2, nFiled3);
	mstr_apct_ip = str;
	
	st_msg.mstr_keypad_msg = "뒷쪽 APCT IP를 입력하세요.";
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Enter the IP Next Step APCT");
	
	if (mstr_apct_ip.IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();  // 키보드 대화 상자에 출력할 정보 저장 변수 초기화 
	}
	else 
	{
		mstr_apct_ip.MakeUpper();
		mstr_apct_ip.TrimLeft(' ');               
		mstr_apct_ip.TrimRight(' ');
		
		st_msg.mstr_typing_msg = mstr_apct_ip;
	}
	
	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		mstr_apct_ip = st_msg.mstr_typing_msg;
		
		mstr_apct_ip.MakeUpper();
		mstr_apct_ip.TrimLeft(' ');               
		mstr_apct_ip.TrimRight(' ');
		
		OnIP_Address(mstr_apct_ip, NETWORK_APCT);
		
		m_ipaddress_apct.SetAddress(nIPFiled0[NETWORK_APCT], nIPFiled1[NETWORK_APCT], nIPFiled2[NETWORK_APCT], nIPFiled3[NETWORK_APCT]);

		st_Network[NETWORK_APCT].mstr_ip = mstr_apct_ip;
		:: WritePrivateProfileString("ADMIN", "APCT_IP", LPCTSTR(mstr_apct_ip), st_path.mstr_basic);
	}
}

void CScreen_Admin::OnBtnPortSaveApct() 
{
	int n_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString str_temp;  // 저장할 정보 임시 저장 변수 
	
	CDialog_KeyPad pad_dlg;
	CDialog_Message msg_dlg;

	CString mstr_apct_port;
	
	if (st_handler.mn_level_teach != TRUE)
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		n_response = msg_dlg.DoModal();
		return;
	}
	
	((CEdit*)GetDlgItem(IDC_EDIT_PORT_APCT))->GetWindowText(mstr_apct_port);
	mstr_apct_port.TrimLeft(' ');	
	mstr_apct_port.TrimRight(' ');
	
	str_temp = mstr_apct_port;
	
	st_msg.mstr_keypad_msg = _T("Rear APCT port Setting");
	
	st_msg.mstr_keypad_val = str_temp;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_port_save_apct.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	n_response = pad_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		mstr_apct_port = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_PORT_APCT))->SetWindowText(mstr_apct_port);

		st_Network[NETWORK_APCT].mstr_port = mstr_apct_port;
		:: WritePrivateProfileString("ADMIN", "APCT_Port", LPCTSTR(mstr_apct_port), st_path.mstr_basic);
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CScreen_Admin::OnIP_Address(CString str_ip, int n_section)
{
	int nDot1, nDot2, nDot3, nStart;
	
	nStart = 0;
	nDot1 = str_ip.Find('.');
	nDot2 = str_ip.Find('.', nDot1 + 1);
	nDot3 = str_ip.Find('.', nDot2 + 1);
	
	if (n_section < NETWORK_ECSERVER || n_section > NETWORK_APCT)		// 0: BPC, 1: NEXT MACHINE, 2:APCT
	{
		return;
	}
	
	nIPFiled0[n_section] = atoi(str_ip.Mid(nStart, nDot1 - nStart));
	nStart = nDot1 + 1;
	nIPFiled1[n_section] = atoi(str_ip.Mid(nStart, nDot2 - nStart));
	nStart = nDot2 + 1;
	nIPFiled2[n_section] = atoi(str_ip.Mid(nStart, nDot3 - nStart));
	nStart = nDot3 + 1;
	nIPFiled3[n_section] = atoi(str_ip.Mid(nStart));
}

void CScreen_Admin::OnBtnServerPortSaveApct() 
{
	int n_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString str_temp;  // 저장할 정보 임시 저장 변수 
	
	CDialog_KeyPad pad_dlg;
	CDialog_Message msg_dlg;
	
	CString mstr_apct_server_port;
	
	if (st_handler.mn_level_teach != TRUE)
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		n_response = msg_dlg.DoModal();
		return;
	}
	
	((CEdit*)GetDlgItem(IDC_EDIT_SERVER_PORT_APCT))->GetWindowText(mstr_apct_server_port);
	mstr_apct_server_port.TrimLeft(' ');	
	mstr_apct_server_port.TrimRight(' ');
	
	str_temp = mstr_apct_server_port;
	
	st_msg.mstr_keypad_msg = _T("Rear APCT server port Setting");
	
	st_msg.mstr_keypad_val = str_temp;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_server_port_save_apct.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	n_response = pad_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		mstr_apct_server_port = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SERVER_PORT_APCT))->SetWindowText(mstr_apct_server_port);
		
		st_Network[NETWORK_APCT].mstr_server_port = mstr_apct_server_port;
		:: WritePrivateProfileString("ADMIN", "APCT_Server_Port", LPCTSTR(mstr_apct_server_port), st_path.mstr_basic);
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CScreen_Admin::OnBtnServerPortSaveFmachine() 
{
	int n_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString str_temp;  // 저장할 정보 임시 저장 변수 
	
	CDialog_KeyPad pad_dlg;
	CDialog_Message msg_dlg;
	
	CString mstr_fmachine_server_port;
	
	if (st_handler.mn_level_teach != TRUE)
	{
		st_msg.str_fallacy_msg = _T("Level2 에서 동작 가능합니다.");
		if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.str_fallacy_msg = _T("Level2 is able to operate in");
		
		n_response = msg_dlg.DoModal();
		return;
	}
	
	((CEdit*)GetDlgItem(IDC_EDIT_SERVER_PORT_FMACHINE))->GetWindowText(mstr_fmachine_server_port);
	mstr_fmachine_server_port.TrimLeft(' ');	
	mstr_fmachine_server_port.TrimRight(' ');
	
	str_temp = mstr_fmachine_server_port;
	
	st_msg.mstr_keypad_msg = _T("Front Machine Server port Setting");
	
	st_msg.mstr_keypad_val = str_temp;
	
	st_msg.mstr_pad_high_limit = "99999";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_btn_server_port_save_fmachine.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	n_response = pad_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		mstr_fmachine_server_port = st_msg.mstr_keypad_val;	
		((CEdit*)GetDlgItem(IDC_EDIT_SERVER_PORT_FMACHINE))->SetWindowText(mstr_fmachine_server_port);
		
		st_Network[NETWORK_NMACHINE].mstr_port = mstr_fmachine_server_port;
		:: WritePrivateProfileString("ADMIN", "FMachine_Server_Port", LPCTSTR(mstr_fmachine_server_port), st_path.mstr_basic);
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CScreen_Admin::OnBtnBufferReadyTest() 
{
	st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;
	SetTimer(TMR_BUFFERREADY, 100, 0);
}

void CScreen_Admin::OnBtnBufferInTest() 
{
	SetTimer(TMR_BUFFERIN, 100, 0);
}

void CScreen_Admin::OnBtnSocketChkTest() 
{
	st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;
	SetTimer(TMR_SOCKETCHK, 100, 0);
}

void CScreen_Admin::OnBtnApctSocketInfoTest() 
{
	SetTimer(TMR_APCTSOCKETINFO, 100, 0);
}

int CScreen_Admin::Buffer_Ready()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	CString strTemp, strTmp;

	switch (mn_buffer_ready_step)
	{
	case 0:
		st_work.mn_client_accept[NETWORK_NMACHINE] = CTL_NO;

		if (st_work.mn_client_ready[NETWORK_NMACHINE] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_NMACHINE] == CTL_YES)
			{
				mn_buffer_ready_step  = 200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, 1, 0);
				}
				
				mlWait_Time[0] = GetCurrentTime();
				mn_buffer_ready_step  = 100;
			}
			mlWait_Time[0] = GetCurrentTime();
		}
		break;
		
	case 100:
		if (st_work.mn_client_accept[NETWORK_NMACHINE] == CTL_YES)
		{
			mn_buffer_ready_step  = 200;
		}
		else
		{
			mlWait_Time[1] = GetCurrentTime();
			mlWait_Time[2] = mlWait_Time[1] - mlWait_Time[0];
			
			if (mlWait_Time[2] > 5000)
			{
				mnError_cnt[0]++;
				
				if (mnError_cnt[0] > 5)
				{
					st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;
					nFuncRet = RET_GOOD;
				}
				else
				{
					st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;
					mn_buffer_ready_step  = 0;
				}
			}
		}
		break;
		
	case 200:
		st_NW.mstr_m_lot_no[1] = "ABCD";
		st_NW.mstr_m_buffer_no[1] = "01";
		st_NW.mstr_m_module_cnt[1] = "20";
		st_NW.mstr_m_part_no[1] = "PARTNUMBER";
		st_NW.mstr_m_last_module[1] = "NO";
		st_NW.mstr_m_bypass[1] = "NO";

		st_work.mstr_client_send_msg[NETWORK_NMACHINE] = Func.OnNetworkDataMake(NW_BUFFER_READY_SEND);
		st_work.mn_client_rpy[NETWORK_NMACHINE] = CTL_YES;

		st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_SEND, 1, 0);
		}
		nFuncRet = CTLBD_RET_GOOD;
		break;
	}

	return nFuncRet;
}

int CScreen_Admin::Buffer_In()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	
	switch (mn_buffer_in_step)
	{
	case 0:
		st_work.mn_client_accept[NETWORK_NMACHINE] = CTL_NO;

		if (st_work.mn_client_ready[NETWORK_NMACHINE] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_NMACHINE] == CTL_YES)
			{
				mn_buffer_in_step  = 200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, 1, 0);
				}
				
				mlWait_Time[0] = GetCurrentTime();
				mn_buffer_in_step  = 100;
			}
			mlWait_Time[0] = GetCurrentTime();
		}
		break;
		
	case 100:
		if (st_work.mn_client_accept[NETWORK_NMACHINE] == CTL_YES)
		{
			mn_buffer_in_step  = 200;
		}
		else
		{
			mlWait_Time[1] = GetCurrentTime();
			mlWait_Time[2] = mlWait_Time[1] - mlWait_Time[0];
			
			if (mlWait_Time[2] > 5000)
			{
				mnError_cnt[0]++;
				
				if (mnError_cnt[0] > 5)
				{
					st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;
					nFuncRet = RET_GOOD;
				}
				else
				{
					st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;
					mn_buffer_in_step  = 0;
				}
			}
		}
		break;
		
	case 200:
		st_work.mstr_client_send_msg[NETWORK_NMACHINE] = Func.OnNetworkDataMake(NW_BUFFER_IN_SEND);
		st_work.mn_client_rpy[NETWORK_NMACHINE] = CTL_YES;
		st_work.mn_client_ready[NETWORK_NMACHINE] = CTL_YES;
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_SEND, 1, 0);
		}
		nFuncRet = CTLBD_RET_GOOD;
		break;
	}

	return nFuncRet;
}

int CScreen_Admin::APCT_Socket_Info()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	
	switch (mn_apct_socket_info_step)
	{
	case 0:
		if (st_work.mn_client_ready[NETWORK_APCT] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_APCT] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_APCT] == CTL_YES)
			{
				mn_apct_socket_info_step  = 200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_APCT, 0);
				}
				
				mlWait_Time[0] = GetCurrentTime();
				mn_apct_socket_info_step  = 100;
			}
			mlWait_Time[0] = GetCurrentTime();
		}
		break;
		
	case 100:
		if (st_work.mn_client_accept[NETWORK_APCT] == CTL_YES)
		{
			mn_apct_socket_info_step  = 200;
		}
		else
		{
			mlWait_Time[1] = GetCurrentTime();
			mlWait_Time[2] = mlWait_Time[1] - mlWait_Time[0];
			
			if (mlWait_Time[2] > 5000)
			{
				mnError_cnt[2]++;
				
				if (mnError_cnt[2] > 5)
				{
					st_work.mn_client_ready[NETWORK_APCT] = CTL_YES;
					nFuncRet = RET_GOOD;
				}
				else
				{
					st_work.mn_client_ready[NETWORK_APCT] = CTL_YES;
					mn_apct_socket_info_step  = 0;
				}
			}
		}
		break;
		
	case 200:
		st_NW.mstr_m_station[1] = "1";
		st_NW.mstr_m_buffer_no[1].Format("%02d", POS_FRONT);
		st_work.mstr_client_send_msg[NETWORK_APCT] = Func.OnNetworkDataMake(NW_APCTSOCKETINFO);
		st_work.mn_client_rpy[NETWORK_APCT] = CTL_YES;
		st_work.mn_client_ready[NETWORK_APCT] = CTL_YES;
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_SEND, NETWORK_APCT, 0);
		}
		nFuncRet = CTLBD_RET_GOOD;
		break;
	}

	return nFuncRet;
}
void CScreen_Admin::OnChkDebug() 
{
	st_handler.mn_debug_mode = !st_handler.mn_debug_mode;

	m_chk_debug.SetCheck(st_handler.mn_debug_mode);
}

void CScreen_Admin::OnChkBarcode() 
{
	CString mstr_temp;

	st_handler.mn_barcode_mode = !st_handler.mn_barcode_mode;
	
	m_chk_barcode.SetCheck(st_handler.mn_barcode_mode);

	mstr_temp.Format("%d", st_handler.mn_barcode_mode);
	:: WritePrivateProfileString("BasicData", "Barcode_used", LPCTSTR(mstr_temp), st_path.mstr_basic);
}

void CScreen_Admin::OnBtnApctBlotTest() 
{
	SetTimer(TMR_APCTBLOT, 100, 0);
}

int CScreen_Admin::APCT_BLot()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	CString strTemp;
	CString strBody, strHeader;
	int nBody;
	
	switch (mn_apct_blot_step)
	{
	case 0:
		if (st_work.mn_client_ready[NETWORK_APCT] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_APCT] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_APCT] == CTL_YES)
			{
				mn_apct_blot_step  = 200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_APCT, 0);
				}
				
				mlWait_Time[0] = GetCurrentTime();
				mn_apct_blot_step  = 100;
			}
			mlWait_Time[0] = GetCurrentTime();
		}
		break;
		
	case 100:
		if (st_work.mn_client_accept[NETWORK_APCT] == CTL_YES)
		{
			mn_apct_blot_step  = 200;
		}
		else
		{
			mlWait_Time[1] = GetCurrentTime();
			mlWait_Time[2] = mlWait_Time[1] - mlWait_Time[0];
			
			if (mlWait_Time[2] > 5000)
			{
				mnError_cnt[2]++;
				
				if (mnError_cnt[2] > 5)
				{
					st_work.mn_client_ready[NETWORK_APCT] = CTL_YES;
					nFuncRet = RET_GOOD;
				}
				else
				{
					st_work.mn_client_ready[NETWORK_APCT] = CTL_YES;
					mn_apct_blot_step  = 0;
				}
			}
		}
		break;
		
	case 200:
		strBody		= "FUNCTION=BLOT_COUNT ";
		strBody		+= " EQP_ID=" + st_lamp.mstr_equip_id;

		nBody = strBody.GetLength();
		
		if (nBody > 0)
		{
			strHeader.Format("%016d", nBody);
			
			st_NW.mstr_header_msg[0] = strHeader;
			st_NW.mstr_data_msg[0] = strBody;
			st_NW.mstr_full_msg[0] = strHeader + strBody;
		}
		else
		{
			st_NW.mstr_header_msg[0] = "";
			st_NW.mstr_data_msg[0] = "";
			st_NW.mstr_full_msg[0] = "";
			
			st_work.mn_client_ready[NETWORK_APCT] = CTL_YES;
			
			mn_apct_blot_step = 0;
			nFuncRet = CTLBD_RET_GOOD;
			break;
		}
		
		st_work.mstr_client_send_msg[NETWORK_APCT] = st_NW.mstr_full_msg[0];
		
		st_work.mn_client_rpy[NETWORK_APCT] = CTL_YES;
		st_sync.mn_rbuffer_apctinfo[POS_FRONT] = CTL_READY;
		st_work.mn_client_ready[NETWORK_APCT] = CTL_YES;
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_SEND, NETWORK_APCT, 0);
		}
		mn_apct_blot_step = 0;
		nFuncRet = CTLBD_RET_GOOD;
		break;
	}
	
	return nFuncRet;
}

void CScreen_Admin::OnAdmin_Dgt_Set()
{
		m_dgt_eject_count.SubclassDlgItem(IDC_DGT_EJECT_COUNT, this);
		m_dgt_eject_count.SetStyle(IDB_BIG3, 4);
		m_dgt_eject_count.SetValue(st_handler.mn_eject_count);

		m_dgt_pick_count.SubclassDlgItem(IDC_DGT_PICK_COUNT, this);
		m_dgt_pick_count.SetStyle(IDB_BIG3, 4);
		m_dgt_pick_count.SetValue(st_handler.mn_pick_count );
}

void CScreen_Admin::OnDgtEjectCount() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 

	char chr_buf[20] ;
	int mn_count;
	
	CDialog_KeyPad pad_dlg;
	
	mn_count = m_dgt_eject_count.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_count, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("EJECT MAX");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Check the Max Eject Count");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "10";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_eject_count.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right-500, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		CString mstr_temp;

		st_handler.mn_eject_count = atoi( st_msg.mstr_keypad_val );
		m_dgt_eject_count.SetValue( st_handler.mn_eject_count );

		mstr_temp.Format("%d", st_handler.mn_eject_count );
		:: WritePrivateProfileString("BasicData", "Eject_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Admin::OnDgtPickCount() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_count;
	
	CDialog_KeyPad pad_dlg;
	
	mn_count = m_dgt_pick_count.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_count, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("PICK MAX");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Check the Max Pick Count");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "10";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_pick_count.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right-500, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		CString mstr_temp;

		st_handler.mn_pick_count = atoi( st_msg.mstr_keypad_val );
		m_dgt_pick_count.SetValue( st_handler.mn_pick_count );

		mstr_temp.Format("%d", st_handler.mn_pick_count );
		:: WritePrivateProfileString("BasicData", "Pick_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Admin::OnChkReverse() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;

	st_handler.mn_reverse_mode = !st_handler.mn_reverse_mode;
	
	m_chk_reverse.SetCheck(st_handler.mn_reverse_mode);

	mstr_temp.Format("%d", st_handler.mn_reverse_mode);
	:: WritePrivateProfileString("BasicData", "Reverse_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);

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

}

void CScreen_Admin::OnBtnBufferdbDelete() 
{
	int n_response;  // 대화 상자 리턴 플래그
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("BUFFER DB를 삭제하시겠습니까?");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_confirm_msg = _T("You want to delete BUFFER DB?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
		{
			if(Mysql.Database_Select("AMT830"))
			{	
				Mysql.Table_Delete("BUFFER_DATA");
				
				AfxMessageBox("BUFFER_DATA DELETE");
			}
		}
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CScreen_Admin::OnChkUldConvayor() 
{
	CString mstr_temp;
	st_handler.n_uld_convayor = !st_handler.n_uld_convayor;
	
	m_chk_uld_convayor.SetCheck(st_handler.n_uld_convayor);
	
	mstr_temp.Format("%d", st_handler.n_uld_convayor);
	:: WritePrivateProfileString("BasicData", "Uld_Convayor_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
}

void CScreen_Admin::OnChkNtoFail() 
{
	st_handler.n_nto_fail = !st_handler.n_nto_fail;	
	m_chk_nto_fail.SetCheck(st_handler.n_nto_fail);	
}

void CScreen_Admin::OnChkHtoFail() 
{
	st_handler.n_hto_fail = !st_handler.n_hto_fail;	
	m_chk_hto_fail.SetCheck(st_handler.n_hto_fail);	
}

void CScreen_Admin::OnChkBtoFail() 
{
	st_handler.n_bto_fail = !st_handler.n_bto_fail;	
	m_chk_bto_fail.SetCheck(st_handler.n_bto_fail);	
}

void CScreen_Admin::OnChkVirtual() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;
	
	st_handler.mn_virtual_mode = !st_handler.mn_virtual_mode;
	
	m_chk_Virtual.SetCheck(st_handler.mn_virtual_mode);
	
	mstr_temp.Format("%d", st_handler.mn_virtual_mode);
	:: WritePrivateProfileString("BasicData", "Virtual_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
}

void CScreen_Admin::OnBtnFileEditor() 
{
	::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 8, 2);
}
