// Dialog_Add_Fail.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Add_Fail.h"


#include "Dialog_Select.h"
#include "Dialog_Keyboard.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"

#include "io.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Add_Fail dialog


CDialog_Add_Fail::CDialog_Add_Fail(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Add_Fail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Add_Fail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "add fail Box";
}


void CDialog_Add_Fail::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Add_Fail)
	DDX_Control(pDX, IDC_LABEL_SERIAL, m_label_serial);
	DDX_Control(pDX, IDC_LABEL_AS_SC, m_label_as_sc);
	DDX_Control(pDX, IDC_LABEL_A4202_SC, m_label_a4202_sc);
	DDX_Control(pDX, IDC_LABEL_A4201_SC, m_label_a4201_sc);
	DDX_Control(pDX, IDC_LABEL_A4200_SC, m_label_a4200_sc);
	DDX_Control(pDX, IDC_GROUP_ADD_SCRAP_TO_SERVER, m_group_add_scrap_to_server);
	DDX_Control(pDX, IDC_GROUP_ADD_4202_SCRAP_TO_SERVER, m_group_add_4202_scrap_to_server);
	DDX_Control(pDX, IDC_GROUP_ADD_4201_SCRAP_TO_SERVER, m_group_add_4201_scrap_to_server);
	DDX_Control(pDX, IDC_GROUP_ADD_4200_SCRAP_TO_SERVER, m_group_add_4200_scrap_to_server);
	DDX_Control(pDX, IDC_BTN_ADD_FAIL, m_btn_add_fail);
	DDX_Control(pDX, IDC_BTN_ADD_4202, m_btn_add_4202);
	DDX_Control(pDX, IDC_BTN_ADD_4201, m_btn_add_4201);
	DDX_Control(pDX, IDC_BTN_ADD_4200, m_btn_add_4200);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Add_Fail, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Add_Fail)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ADD_FAIL, OnBtnAddFail)
	ON_BN_CLICKED(IDC_BTN_ADD_4200, OnBtnAdd4200)
	ON_BN_CLICKED(IDC_BTN_ADD_4201, OnBtnAdd4201)
	ON_BN_CLICKED(IDC_BTN_ADD_4202, OnBtnAdd4202)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Add_Fail message handlers

BOOL CDialog_Add_Fail::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// ************************************************************************** 
	// 컨트롤에 설정할 폰트 생성한다                                              
	// ************************************************************************** 
	mp_add_fail_font = NULL;
	mp_add_fail_font = new CFont;
	mp_add_fail_font->CreateFont(15,0,0,0,0,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	
	OnAddFail_GroupBox_Set();
	OnAddFail_Button_Set();
	OnAddFail_EditBox_Set();
	OnAddFail_Label_Set();

	mn_edit = CTL_NO;
	mn_timer_act = CTL_NO;
	mn_ec_server_step = 0;

	st_sync.mn_add_fail = CTL_YES;
	st_handler.cwnd_add_fail = this;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Add_Fail::OnDestroy() 
{
	st_sync.mn_add_fail = CTL_NO;
	st_handler.cwnd_add_fail = NULL;
	
	delete mp_add_fail_font;
	mp_add_fail_font = NULL;
	
	delete mp_add_fail_ok;      // OK 버튼에 설정된 정보 삭제 

	StandardDialog::OnDestroy();
}

void CDialog_Add_Fail::OnTimer(UINT nIDEvent) 
{
	int nRet;
	
	if (nIDEvent == TMR_ADD_FAIL)
	{
		nRet = OnAddFail_Server_Send();
		
		if (nRet == CTLBD_RET_GOOD)
		{
			((CEdit*)GetDlgItem(IDC_EDIT_SERIAL))->SetWindowText(_T(""));
			((CEdit*)GetDlgItem(IDC_EDIT_AS_SC))->SetWindowText(_T(""));
			((CEdit*)GetDlgItem(IDC_EDIT_A4200_SC))->SetWindowText(_T(""));
			((CEdit*)GetDlgItem(IDC_EDIT_A4201_SC))->SetWindowText(_T(""));
			((CEdit*)GetDlgItem(IDC_EDIT_A4202_SC))->SetWindowText(_T(""));

			mn_timer_act = CTL_NO;

			(GetDlgItem(IDOK))->EnableWindow(true);
			KillTimer(TMR_ADD_FAIL);
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			mn_timer_act = CTL_NO;
			
			(GetDlgItem(IDOK))->EnableWindow(true);
			KillTimer(TMR_ADD_FAIL);
		}
	}

	StandardDialog::OnTimer(nIDEvent);
}

void CDialog_Add_Fail::OnBtnAddFail() 
{
	CString strSerial, strCode;

	st_work.strMdlRejectAdd.RemoveAll();
	st_work.strMdlRejectAddCode.RemoveAll();

	((CEdit*)GetDlgItem(IDC_EDIT_SERIAL))->GetWindowText(_T(strSerial));
	((CEdit*)GetDlgItem(IDC_EDIT_AS_SC))->GetWindowText(_T(strCode));

	st_work.strMdlRejectAdd.AddTail(strSerial);
	st_work.strMdlRejectAddCode.AddTail(strCode);

	SetTimer(TMR_ADD_FAIL, 10, NULL);
}

void CDialog_Add_Fail::OnBtnAdd4200() 
{
	CString strTemp;
	int nCount, i;

	st_work.strMdlRejectAdd.RemoveAll();
	st_work.strMdlRejectAddCode.RemoveAll();

	((CEdit*)GetDlgItem(IDC_EDIT_A4200_SC))->GetWindowText(_T(strTemp));
	nCount = atoi(strTemp);

	if (nCount > 0)
	{
		for (i = 0; i < nCount; i++)
		{
			strTemp.Format("%s%08d", st_handler.mstr_lot_name[0], st_sync.n_scrap_serial);
			st_work.strMdlRejectAdd.AddTail(strTemp);
			st_work.strMdlRejectAddCode.AddTail("4200");
			st_sync.n_scrap_serial++;
		}

		SetTimer(TMR_ADD_FAIL, 10, NULL);
	}
}

void CDialog_Add_Fail::OnBtnAdd4201() 
{
	CString strTemp;
	int nCount, i;

	st_work.strMdlRejectAdd.RemoveAll();
	st_work.strMdlRejectAddCode.RemoveAll();
	
	((CEdit*)GetDlgItem(IDC_EDIT_A4201_SC))->GetWindowText(_T(strTemp));
	nCount = atoi(strTemp);

	if (nCount > 0)
	{
		for (i = 0; i < nCount; i++)
		{
			strTemp.Format("%s%08d", st_handler.mstr_lot_name[0], st_sync.n_scrap_serial);
			st_work.strMdlRejectAdd.AddTail(strTemp);
			st_work.strMdlRejectAddCode.AddTail("4201");
			st_sync.n_scrap_serial++;
		}

		SetTimer(TMR_ADD_FAIL, 10, NULL);
	}
}

void CDialog_Add_Fail::OnBtnAdd4202() 
{
	CString strTemp;
	int nCount, i;

	st_work.strMdlRejectAdd.RemoveAll();
	st_work.strMdlRejectAddCode.RemoveAll();
	
	((CEdit*)GetDlgItem(IDC_EDIT_A4202_SC))->GetWindowText(_T(strTemp));
	nCount = atoi(strTemp);

	if (nCount > 0)
	{
		for (i = 0; i < nCount; i++)
		{
			strTemp.Format("%s%08d", st_handler.mstr_lot_name[0], st_sync.n_scrap_serial);
			st_work.strMdlRejectAdd.AddTail(strTemp);
			st_work.strMdlRejectAddCode.AddTail("4202");
			st_sync.n_scrap_serial++;
		}

		SetTimer(TMR_ADD_FAIL, 10, NULL);
	}
}

void CDialog_Add_Fail::OnAddFail_GroupBox_Set()
{
	CSxLogFont addfail_font(11, FW_THIN, false, "Bitstream Vera Sans Mono");

	m_group_add_scrap_to_server.SetFont(addfail_font);
	m_group_add_scrap_to_server.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_add_scrap_to_server.SetFontBold(TRUE);

	m_group_add_4202_scrap_to_server.SetFont(addfail_font);
	m_group_add_4202_scrap_to_server.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_add_4202_scrap_to_server.SetFontBold(TRUE);

	m_group_add_4201_scrap_to_server.SetFont(addfail_font);
	m_group_add_4201_scrap_to_server.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_add_4201_scrap_to_server.SetFontBold(TRUE);

	m_group_add_4200_scrap_to_server.SetFont(addfail_font);
	m_group_add_4200_scrap_to_server.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_add_4200_scrap_to_server.SetFontBold(TRUE);
}

void CDialog_Add_Fail::OnAddFail_Button_Set()
{
	mp_add_fail_ok = CreateOkButton(IDOK, this);
}

int CDialog_Add_Fail::OnAddFail_Server_Send()
{
	int nFuncRet = CTLBD_RET_PROCEED;
	CString strTemp, strMsgTemp, strHeader, strBody;
	CString strEvent[10];

	CDialog_Message  msg_dlg;

	Func.ThreadFunctionStepTrace(0, mn_ec_server_step);

	switch (mn_ec_server_step)
	{
	case 0:
		if (st_work.mn_client_ready[NETWORK_ECSERVER] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
			{
				mn_ec_server_step  = 200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_ECSERVER, 0);
				}
				
				ml_wait_time[0] = GetCurrentTime();
				mn_ec_server_step  = 100;
			}
		}
		break;
		
	case 100:
		if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
		{
			mn_ec_server_step  = 200;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				nFuncRet = CTLBD_RET_ERROR;
				st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
				mn_ec_server_step  = 0;
			}
		}
		break;
		
	case 200:	
		st_work.mstr_client_send_msg[NETWORK_ECSERVER] = Func.OnNetworkDataMake(NW_ONLINE_END);

		st_NW.mn_invaild_function_flag = CTL_NO;
		st_work.mn_client_rpy[NETWORK_ECSERVER] = CTL_YES;
		st_sync.mstr_online_end = "READY";
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->SendMessage(CLIENT_SEND, NETWORK_ECSERVER, 0);
		}
		
		ml_retry_wait[0] = GetCurrentTime();
		mn_ec_server_step  = 300;
		break;
		
	case 300:						// Buffer ready에 대한 Reply가 오면 Buffer를 다음 장비로 넣는다.
		if (st_NW.mn_invaild_function_flag == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_ec_server_step  = 1500;
		}
		else if (st_sync.mstr_online_end == "PASS")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_ec_server_step  = 400;
		}
		else if (st_sync.mstr_online_end == "FAIL")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_ec_server_step  = 1500;
		}
		else
		{
			ml_retry_wait[1] = GetCurrentTime();
			ml_retry_wait[2] = ml_retry_wait[1] - ml_retry_wait[0];
			
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			
			if (ml_retry_wait[2] > st_basic.mn_network_wait_time * 1000)		// 분으로 세팅하기 때문에 X 60... msec이기 때문에 X 1000
			{
				mn_ec_server_step  = 0;
			}
		}
		break;
		
	case 400:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_ec_server_step  = 1000;
		break;

	case 1000:
		if (st_work.mn_client_ready[NETWORK_ECSERVER] == CTL_YES)
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_NO;
			
			if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
			{
				mn_ec_server_step  = 1200;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)
				{
//					st_handler.cwnd_list->PostMessage(CLIENT_ACCEPT, NETWORK_ECSERVER, 0);
				}
				
				ml_wait_time[0] = GetCurrentTime();
				mn_ec_server_step  = 1100;
			}
		}
		break;
		
	case 1100:
		if (st_work.mn_client_accept[NETWORK_ECSERVER] == CTL_YES)
		{
			mn_ec_server_step  = 1200;
		}
		else
		{
			ml_wait_time[1] = GetCurrentTime();
			ml_wait_time[2] = ml_wait_time[1] - ml_wait_time[0];
			
			if (ml_wait_time[2] > 5000)
			{
				nFuncRet = CTLBD_RET_ERROR;
				st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
				mn_ec_server_step  = 1000;
			}
		}
		break;
		
	case 1200:	
		st_work.mstr_client_send_msg[NETWORK_ECSERVER] = Func.OnNetworkDataMake(NW_SCRAP_INFO);
		
		st_work.mn_client_rpy[NETWORK_ECSERVER] = CTL_YES;
		st_NW.mn_scrap_info_flag = CTL_READY;
		
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->SendMessage(CLIENT_SEND, NETWORK_ECSERVER, 0);
		}
		
		ml_retry_wait[0] = GetCurrentTime();
		mn_ec_server_step  = 1300;
		break;
		
	case 1300:						// Buffer ready에 대한 Reply가 오면 Buffer를 다음 장비로 넣는다.
		if (st_sync.mstr_scrap_info == "PASS")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_ec_server_step  = 1400;
		}
		else if (st_sync.mstr_scrap_info == "FAIL")
		{
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			mn_ec_server_step  = 1500;
		}
		else
		{
			ml_retry_wait[1] = GetCurrentTime();
			ml_retry_wait[2] = ml_retry_wait[1] - ml_retry_wait[0];
			
			st_work.mn_client_ready[NETWORK_ECSERVER] = CTL_YES;
			
			if (ml_retry_wait[2] > st_basic.mn_network_wait_time * 1000)		// 분으로 세팅하기 때문에 X 60... msec이기 때문에 X 1000
			{
				mn_ec_server_step  = 0;
			}
		}
		break;

	case 1400:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_ec_server_step  = 0;
		nFuncRet = CTLBD_RET_GOOD;
		break;

	case 1500:
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(CLIENT_CLOSE, NETWORK_ECSERVER, 0);
		}
		mn_ec_server_step  = 0;
		nFuncRet = CTLBD_RET_ERROR;
		break;
	}

	return nFuncRet;
}

void CDialog_Add_Fail::OnAddFail_EditBox_Set()
{
	m_edit_serial.SubclassDlgItem(IDC_EDIT_SERIAL, this);
	m_edit_serial.bkColor(RGB(50, 100, 150));
	m_edit_serial.textColor(RGB(255, 255,255));
	m_edit_serial.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_scrap_code.SubclassDlgItem(IDC_EDIT_AS_SC, this);
	m_edit_scrap_code.bkColor(RGB(50, 100, 150));
	m_edit_scrap_code.textColor(RGB(255, 255,255));
	m_edit_scrap_code.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_4200_scrap_count.SubclassDlgItem(IDC_EDIT_A4200_SC, this);
	m_edit_4200_scrap_count.bkColor(RGB(50, 100, 150));
	m_edit_4200_scrap_count.textColor(RGB(255, 255,255));
	m_edit_4200_scrap_count.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_4201_scrap_count.SubclassDlgItem(IDC_EDIT_A4201_SC, this);
	m_edit_4201_scrap_count.bkColor(RGB(50, 100, 150));
	m_edit_4201_scrap_count.textColor(RGB(255, 255,255));
	m_edit_4201_scrap_count.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	m_edit_4202_scrap_count.SubclassDlgItem(IDC_EDIT_A4202_SC, this);
	m_edit_4202_scrap_count.bkColor(RGB(50, 100, 150));
	m_edit_4202_scrap_count.textColor(RGB(255, 255,255));
	m_edit_4202_scrap_count.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");
}

void CDialog_Add_Fail::OnAddFail_Label_Set()
{
	m_label_serial.SetFont(mp_add_fail_font);
	m_label_serial.SetWindowText(_T("Serial"));
	m_label_serial.SetCenterText();
	m_label_serial.SetColor(RGB(0255, 255, 255));
	m_label_serial.SetGradientColor(RGB(255, 255, 255));
	m_label_serial.SetTextColor(RGB(0, 0, 0));

	m_label_as_sc.SetFont(mp_add_fail_font);
	m_label_as_sc.SetWindowText(_T("Scrap Code"));
	m_label_as_sc.SetCenterText();
	m_label_as_sc.SetColor(RGB(0255, 255, 255));
	m_label_as_sc.SetGradientColor(RGB(255, 255, 255));
	m_label_as_sc.SetTextColor(RGB(0, 0, 0));

	m_label_a4202_sc.SetFont(mp_add_fail_font);
	m_label_a4202_sc.SetWindowText(_T("Scrap Count"));
	m_label_a4202_sc.SetCenterText();
	m_label_a4202_sc.SetColor(RGB(0255, 255, 255));
	m_label_a4202_sc.SetGradientColor(RGB(255, 255, 255));
	m_label_a4202_sc.SetTextColor(RGB(0, 0, 0));

	m_label_a4201_sc.SetFont(mp_add_fail_font);
	m_label_a4201_sc.SetWindowText(_T("Scrap Count"));
	m_label_a4201_sc.SetCenterText();
	m_label_a4201_sc.SetColor(RGB(0255, 255, 255));
	m_label_a4201_sc.SetGradientColor(RGB(255, 255, 255));
	m_label_a4201_sc.SetTextColor(RGB(0, 0, 0));

	m_label_a4200_sc.SetFont(mp_add_fail_font);
	m_label_a4200_sc.SetWindowText(_T("Scrap Count"));
	m_label_a4200_sc.SetCenterText();
	m_label_a4200_sc.SetColor(RGB(0255, 255, 255));
	m_label_a4200_sc.SetGradientColor(RGB(255, 255, 255));
	m_label_a4200_sc.SetTextColor(RGB(0, 0, 0));
}

BOOL CDialog_Add_Fail::PreTranslateMessage(MSG* pMsg) 
{

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}

		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return StandardDialog::PreTranslateMessage(pMsg);
}
