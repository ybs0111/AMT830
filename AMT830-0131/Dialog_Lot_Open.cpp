// Dialog_Lot_Open.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Lot_Open.h"
#include "KeyboardDll.h"	//20120514 유성준
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Lot_Open dialog


CDialog_Lot_Open::CDialog_Lot_Open(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Lot_Open::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Lot_Open)
	//}}AFX_DATA_INIT
}


void CDialog_Lot_Open::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Lot_Open)
	DDX_Control(pDX, IDC_MSG_LOT_QTY2, m_msg);
	DDX_Control(pDX, IDC_STATIC_PARTID, m_label_part_id);
	DDX_Control(pDX, IDC_STATIC_OPERID, m_label_oper_info);
	DDX_Control(pDX, IDC_STATIC_LOTID, m_label_lot_id);
	DDX_Control(pDX, IDC_STATIC_LOT_QRT, m_label_lot_qty);
	DDX_Control(pDX, IDC_STATIC_EQPID, m_label_eqp_id);
	DDX_Control(pDX, IDC_MSG_PART_ID, m_msg_part_id);
	DDX_Control(pDX, IDC_MSG_OPER_ID, m_msg_oper_id);
	DDX_Control(pDX, IDC_MSG_LOT_QTY, m_msg_lot_qty);
	DDX_Control(pDX, IDC_MSG_LOT_ID, m_msg_lot_id);
	DDX_Control(pDX, IDC_MSG_EQP_ID, m_msg_eqp_id);
	DDX_Control(pDX, IDC_GROUP_LOT_START, m_group_lot_start);
	DDX_Control(pDX, IDC_BTN_LOT_DISPLAY, m_btn_lot_display);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btn_lot_delete);	
	DDX_Control(pDX, IDC_BTN_LOT_SRART, m_btn_lot_start);
	DDX_Control(pDX, IDC_BTN_LOT_CANCEL, m_btn_lot_cancel);
	DDX_Control(pDX, IDC_BTN_OK, m_btn_Lot_open_ok);
	DDX_Control(pDX, IDC_MSG_LOT_OPEN, m_msg_lot_open);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Lot_Open, CDialog)
	//{{AFX_MSG_MAP(CDialog_Lot_Open)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_LOT_SRART, OnBtnLotStart)
	ON_BN_CLICKED(IDC_BTN_LOT_CANCEL, OnBtnLotCancel)
	ON_BN_CLICKED(IDC_BTN_LOT_DISPLAY, OnBtnLotDisplay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_LOT_END, OnBtnLotEnd)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Lot_Open message handlers

BOOL CDialog_Lot_Open::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(20,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	
	Init_Button();
	Init_Label();
	Init_GroupBox_Set();
	Init_Data();
	if(st_work.n_eqp_lotend == CTL_YES)
	{
		m_btn_lot_start.EnableWindow(TRUE);
	}
	else
	{
		m_btn_lot_start.EnableWindow(FALSE);
	}
	m_btn_lot_display.EnableWindow(TRUE);
	m_btn_Lot_open_ok.EnableWindow(TRUE);

	return TRUE;
}

void CDialog_Lot_Open::Init_Data()
{
	m_msg_lot_id.SetWindowText(mstr_lot_no);
	m_msg_eqp_id.SetWindowText(st_lamp.mstr_equip_id);
	m_msg_part_id.SetWindowText(st_NW.mstr_part_id[1]);
//	m_msg_oper_id.SetWindowText(st_work.str_worker_name + "(" + st_work.mstr_op_id + ")");//st_work.mstr_op_id); //20120712
	m_msg_oper_id.SetWindowText(st_work.mstr_op_id);//st_work.mstr_op_id); //20120712
	m_msg_lot_qty.SetWindowText(st_NW.mstr_qty[1]);

}

void CDialog_Lot_Open::Init_GroupBox_Set()
{
	CSxLogFont ls_Logo(15,FW_SEMIBOLD,false,"맑은 고딕");

	m_group_lot_start.SetFont(ls_Logo);
	m_group_lot_start.SetCatptionTextColor(RGB(145,25,0));
	m_group_lot_start.SetFontBold(TRUE);
}

void CDialog_Lot_Open::Init_Label()
{
	m_msg_lot_open.SetFont(m_p_font);
	m_msg_lot_open.SetWindowText(_T("LOT Open"));
	m_msg_lot_open.SetCenterText();
	m_msg_lot_open.SetColor(WHITE_C);
	m_msg_lot_open.SetGradientColor(ORANGE_C);
	m_msg_lot_open.SetTextColor(BLACK_C);
	
	m_edit_Lot_open.SubclassDlgItem(IDC_EDIT_LOT_OPEN, this);
	m_edit_Lot_open.setFont(-20, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_label_part_id.SetFont(m_p_font);
	m_label_part_id.SetWindowText(_T("Part ID"));
	m_label_part_id.SetCenterText();
	m_label_part_id.SetColor(RGB(0,0,255));
	m_label_part_id.SetGradientColor(RGB(0,0,0));
	m_label_part_id.SetTextColor(RGB(255,255,255));

	m_label_oper_info.SetFont(m_p_font);
	m_label_oper_info.SetWindowText(_T("Oper Info"));
	m_label_oper_info.SetCenterText();
	m_label_oper_info.SetColor(RGB(0,0,255));
	m_label_oper_info.SetGradientColor(RGB(0,0,0));
	m_label_oper_info.SetTextColor(RGB(255,255,255));

	m_label_lot_qty.SetFont(m_p_font);
	m_label_lot_qty.SetWindowText(_T("Lot Qty"));
	m_label_lot_qty.SetCenterText();
	m_label_lot_qty.SetColor(RGB(0,0,255));
	m_label_lot_qty.SetGradientColor(RGB(0,0,0));
	m_label_lot_qty.SetTextColor(RGB(255,255,255));

	m_label_lot_id.SetFont(m_p_font);
	m_label_lot_id.SetWindowText(_T("Lot ID"));
	m_label_lot_id.SetCenterText();
	m_label_lot_id.SetColor(RGB(0,0,255));
	m_label_lot_id.SetGradientColor(RGB(0,0,0));
	m_label_lot_id.SetTextColor(RGB(255,255,255));

	m_label_eqp_id.SetFont(m_p_font);
	m_label_eqp_id.SetWindowText(_T("EQP ID"));
	m_label_eqp_id.SetCenterText();
	m_label_eqp_id.SetColor(RGB(0,0,255));
	m_label_eqp_id.SetGradientColor(RGB(0,0,0));
	m_label_eqp_id.SetTextColor(RGB(255,255,255));

	m_msg_part_id.SetFont(m_p_font);
	m_msg_part_id.SetWindowText(_T(""));
	m_msg_part_id.SetCenterText();
	m_msg_part_id.SetColor(RGB(251,236,166));
	m_msg_part_id.SetGradientColor(RGB(240,240,240));
	m_msg_part_id.SetTextColor(RGB(0,0,0));
	
	m_msg_oper_id.SetFont(m_p_font);
	m_msg_oper_id.SetWindowText(_T(""));
	m_msg_oper_id.SetCenterText();
	m_msg_oper_id.SetColor(RGB(251,236,166));
	m_msg_oper_id.SetGradientColor(RGB(240,240,240));
	m_msg_oper_id.SetTextColor(RGB(0,0,0));
	
	m_msg_lot_qty.SetFont(m_p_font);
	m_msg_lot_qty.SetWindowText(_T(""));
	m_msg_lot_qty.SetCenterText();
	m_msg_lot_qty.SetColor(RGB(251,236,166));
	m_msg_lot_qty.SetGradientColor(RGB(240,240,240));
	m_msg_lot_qty.SetTextColor(RGB(0,0,0));
	
	m_msg_lot_id.SetFont(m_p_font);
	m_msg_lot_id.SetWindowText(_T(""));
	m_msg_lot_id.SetCenterText();
	m_msg_lot_id.SetColor(RGB(251,236,166));
	m_msg_lot_id.SetGradientColor(RGB(240,240,240));
	m_msg_lot_id.SetTextColor(RGB(0,0,0));
	
	m_msg_eqp_id.SetFont(m_p_font);
	m_msg_eqp_id.SetWindowText(_T(""));
	m_msg_eqp_id.SetCenterText();
	m_msg_eqp_id.SetColor(RGB(251,236,166));
	m_msg_eqp_id.SetGradientColor(RGB(240,240,240));
	m_msg_eqp_id.SetTextColor(RGB(0,0,0));

	m_msg.SetFont(m_p_font);
	m_msg.SetWindowText(_T(""));
	m_msg.SetCenterText();
	m_msg.SetColor(RGB(251,236,166));
	m_msg.SetGradientColor(RGB(240,240,240));
	m_msg.SetTextColor(RGB(0,0,0));
	
}

void CDialog_Lot_Open::Init_Button()
{
	short	shBtnColor = 30;

	m_btn_Lot_open_ok.SetFont(m_p_font);
 	m_btn_Lot_open_ok.SetIcon(IDI_MAINTENANCE_1);
	m_btn_Lot_open_ok.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_Lot_open_ok.SetWindowText("OK");
	m_btn_Lot_open_ok.SetButtonColor(1);
	m_btn_Lot_open_ok.SetCheck(TRUE);

}

void CDialog_Lot_Open::OnDestroy()
{

	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}

	KillTimer(TMR_LOT_DISPLAY); 
	KillTimer(TMR_LOT_END); 
	CDialog::OnInitDialog();
}

void CDialog_Lot_Open::OnBtnOk() 
{
	// TODO: Add your control notification handler code here
// 	st_handler.mn_jobComplete = YES; //20121226 임시
// 	st_work.n_bcrNojob = 2; //20121226 임시
	CDialog::OnOK();
}


void CDialog_Lot_Open::OnBtnLotStart() 
{
	if(st_work.n_eqp_lotend == CTL_YES)
	{
		st_work.n_eqp_lotend = CTL_NO;
	}

}

void CDialog_Lot_Open::OnBtnLotDisplay() 
{
	mn_work_mode = NW_LOT_DISPLAY;

	UpdateData();

	((CEdit*)GetDlgItem(IDC_EDIT_LOT_OPEN))->GetWindowText(mstr_lot_no);
	mstr_lot_no.MakeUpper();
	mstr_lot_no.TrimLeft(' ');
	mstr_lot_no.TrimRight(' ');
	m_msg_lot_id.SetWindowText(mstr_lot_no);
	m_msg_eqp_id.SetWindowText(st_lamp.mstr_equip_id);
	m_msg_part_id.SetWindowText("");
	m_msg_oper_id.SetWindowText(st_work.mstr_op_id);//st_work.mstr_op_id); //20120712
//	m_msg_oper_id.SetWindowText(st_work.str_worker_name + "(" + st_work.mstr_op_id + ")");//st_work.mstr_op_id); //20120712
	m_msg_lot_qty.SetWindowText("");	
	n_lotopen_step = 0;
	
	st_sync.b_lotcancel_use = FALSE;//20130130

	SetTimer(TMR_LOT_DISPLAY, 100, NULL);
	//st_work.str_lot_no = "WCC2104941";
	st_work.mstr_lot_no_check = st_work.mstr_lot_no = mstr_lot_no;
	
	//Func.OnNetworkDataMake(NW_LOT_DISPLAY);	
	//Func.OnNetworkDataMake(NW_LABEL_INFO);

	m_btn_lot_display.EnableWindow(FALSE);
	m_btn_Lot_open_ok.EnableWindow(FALSE);
}

void CDialog_Lot_Open::OnTimer(UINT nIDEvent) 
{
	int nRet;

	COleDateTime mtime_cur;
	int n_cur_hour, n_cur_minute, n_cur_second;
	CString strTmp;


	if (nIDEvent == TMR_LOT_DISPLAY)
	{
		nRet = OnLotStart_Excution();

		if (nRet == RET_GOOD)
		{
			m_btn_lot_display.EnableWindow(TRUE);
			m_btn_Lot_open_ok.EnableWindow(TRUE);

			st_work.mn_Barcode_OutPrint = 0;

			////2014,0408
			mtime_cur = COleDateTime::GetCurrentTime();
			
			n_cur_hour = mtime_cur.GetHour();
			n_cur_minute = mtime_cur.GetMinute();
			n_cur_second = mtime_cur.GetSecond();

			strTmp.Format("%02d:%02d:%02d", n_cur_hour, n_cur_minute, n_cur_second);
			st_work.mstr_Start_time = strTmp;
			////

			KillTimer(TMR_LOT_DISPLAY); 
		}
		else if (nRet == RET_ERROR)
		{
			m_btn_lot_display.EnableWindow(TRUE);
			m_btn_Lot_open_ok.EnableWindow(TRUE);
			KillTimer(TMR_LOT_DISPLAY);
		}
	}
	else if (nIDEvent == TMR_LOT_END)
	{
		nRet = OnLotEnd_Excution();

		if (nRet == RET_GOOD)
		{
			KillTimer(TMR_LOT_END); 
		}
		else if (nRet == RET_ERROR)
		{
			KillTimer(TMR_LOT_END);
		}
	}
	else if(nIDEvent == TMR_LOT_CANCEL)
	{
		nRet = OnLotCancel_Excution();

		if (nRet == RET_GOOD)
		{
			KillTimer(TMR_LOT_END); 
		}
		else if (nRet == RET_ERROR)
		{
			KillTimer(TMR_LOT_END);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CDialog_Lot_Open::Server_Data_Display()
{
	m_msg_eqp_id.SetWindowText(st_NW.mstr_eqp_id[1]);
	m_msg_part_id.SetWindowText(st_NW.mstr_Recive_PartNo[0]);//st_NW.mstr_part_id[1]);//20120913
	m_msg_lot_id.SetWindowText(st_NW.mstr_lot_id[1]);
	m_msg_lot_qty.SetWindowText(st_NW.mstr_qty[1]);
	m_msg.SetWindowText(st_NW.mstr_msg[1]);

//		mst_temp = st_NW.mstr_3Contents_1[0] + " " + st_NW.mstr_3Contents_back_1[0];
//		m_edit_labelset_data3.SetWindowText(_T(mst_temp));

}


int CDialog_Lot_Open::OnLotStart_Excution()
{
	int nRet_1 = RET_PROCEED, FuncRet = RET_PROCEED;
	int n_response;							// 대화 상자 리턴 플래그
	CString str_serial,str_partname;
	CDialog_Message msg_dlg;

	switch (n_lotopen_step)
	{
	case 0:
		// **********************************************************************
		//  서버와의 통신전에 접속 상태를 확인한다.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect == NO)		// 접속중이 아니면 접속을 시도한다.
		{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
			NetworkConnectTime[0] = GetCurrentTime();
			n_lotopen_step = 10;
		}
		else													// 접속중이면...
		{
			n_lotopen_step = 100;
		}
		break;

	case 10:
		// **********************************************************************
		//  접속을 확인한다.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != YES)
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 10000)	// 10초 동안 연결이 되지 않으면...
			{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
				
				n_lotopen_step = 5000;				
				st_msg.str_fallacy_msg = _T("[LABEL SERVER] 연결이 되지 않습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[LABEL SERVER] is not connected.");
				}

				n_response = msg_dlg.DoModal();

				if (n_response == IDOK)
				{
					
				}
			}
		}
		else											// 연결이 되었다.
		{
			n_lotopen_step = 100;
		}
		break;

	case 100:
		// **********************************************************************
		//  시작을 알린다.
		// **********************************************************************
		sprintf(st_client[NETWORK_CLIENT_1].ch_send,"%s",Func.OnNetworkDataMake(NW_LABEL_INFO));

		//if (Ret == RET_GOOD)
		//{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, NETWORK_CLIENT_1);	// 메인 프레임으로 메시지 전송
			NetworkConnectTime[0] = GetCurrentTime();
			n_lotopen_step = 200;
		//}
		break;

	case 200:
		// **********************************************************************
		//  완료가 될때까지 대기.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != NO)	// 작업이 끝나지 않았다면...
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 20000)	// 10초 동안 들어오지 않는다면...
			{
				n_lotopen_step = 5000;
				st_msg.str_fallacy_msg = _T("[LABEL SERVER] 5초간 답변이 오지 않았습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[LABEL SERVER] Answer did not come 5 seconds.");
				}

				n_response = msg_dlg.DoModal();

				if (n_response == IDOK)
				{
					
				}
			}
		}
		else if (st_client[NETWORK_CLIENT_1].n_connect == NO)	// 작업이 끝났다면...
		{
			n_lotopen_step = 300;
//			n_lotopen_step = 700;
		}	
		break;

	case 300:
		// **********************************************************************
		//  Reply를 분석한다.
		// **********************************************************************
		if(st_NW.mstr_result[1] == "PASS")
		{
			n_lotopen_step = 400;
		}
		else								// 비정상이면...
		{
			n_lotopen_step = 5000;

			if (st_NW.mstr_msg[1] == "")
			{
				st_msg.str_fallacy_msg = _T("[LABEL SERVER - LIST] 비정상 데이터가 전송되었습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[LABEL SERVER] Data was sent to the abnormal.");
				}

			}
			else
			{	
				st_NW.mstr_msg[1].TrimRight(' ');
				st_NW.mstr_msg[1].TrimLeft(' ');
				st_msg.str_fallacy_msg.Format("[LABEL SERVER ERROR] : %s", st_NW.mstr_msg[1]);
			}
			n_response = msg_dlg.DoModal();

			if (n_response == IDOK)
			{
				
			}
		}
		break;

	case 400:
		Server_Data_Display();
		n_lotopen_step = 500;
		break;
		

	case 500:
		// 다 정상이니까, 테스터에게 새로운 LOT임을 알려준다.
//		if (st_handler.cwnd_main != NULL)
//		{
//			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
//		}
		st_handler.mn_jobComplete = NO;
		NetworkConnectTime[0] = GetCurrentTime();
		n_lotopen_step = 600;
		break;

	case 600:
		NetworkConnectTime[1] = GetCurrentTime();
		NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];
		if(NetworkConnectTime[2] < 0) NetworkConnectTime[0] = GetCurrentTime();

		if(st_handler.mn_receive == YES)
		{
			NetworkConnectTime[0] = GetCurrentTime();
			n_lotopen_step = 700;
		}
		else if(nRet_1 == RET_ERROR)
		{
			n_lotopen_step = 5000;
		}
		else
		{
			if(NetworkConnectTime[2] < 10000) break;
			else
			{
				//타임아읏 에러//2014,0212
				st_handler.mn_receive = NO;
			}
		}
		break;

	case 700:
		NetworkConnectTime[1] = GetCurrentTime();
		NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];
		if(NetworkConnectTime[2] < 0) NetworkConnectTime[0] = GetCurrentTime();

		nRet_1 = Func.OnBCRModel_Change_Req();
		if(nRet_1 == RET_GOOD)
		{

			NetworkConnectTime[0] = GetCurrentTime();
			n_lotopen_step = 800;
		}
		else if(nRet_1 == RET_ERROR)
		{
			n_lotopen_step = 5000;
		}
		else
		{
			if(NetworkConnectTime[2] < 10000) break;
			else
			{
				n_lotopen_step = 5000;//타임아읏 에러
			}
		}
		break;

	case 800:
		NetworkConnectTime[1] = GetCurrentTime();
		NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];
		if(NetworkConnectTime[2] < 0) NetworkConnectTime[0] = GetCurrentTime();

		nRet_1 = Func.OnBCRModel_Change_Rev();
		if(nRet_1 == RET_GOOD)
		{
			sprintf(st_msg.c_abnormal_msg, "BCR Job Req 응답 완료.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "Complete response BCR Job Req");
			}

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //

			n_lotopen_step = 900;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재 //
				sprintf(st_msg.c_abnormal_msg, "BCR Job Req 응답이 없습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "There was no response BCR Job Req");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
			}
			n_lotopen_step = 5000;
		}
		else
		{
			if(NetworkConnectTime[2] < 10000) break;
			else
			{
				if(st_handler.cwnd_list != NULL)
				{  // 리스트 바 화면 존재 //
					sprintf(st_msg.c_abnormal_msg, "BCR Job Req 응답이 없습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "There was no response BCR Job Req");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
				}
				n_lotopen_step = 5000;
				//타임아읏 에러
			}
		}

	case 900:
		nRet_1 = Func.OnHeatSinkModel_Change_Req();
		if(nRet_1 == RET_GOOD)
		{
			NetworkConnectTime[0] = GetCurrentTime();
			n_lotopen_step = 1000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재 //
				sprintf(st_msg.c_abnormal_msg, "Heat Sink Job Req 응답이 없습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "There was no response Heat Sink Job Req");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
			}
			n_lotopen_step = 5000;
		}
		else
		{
			if(NetworkConnectTime[2] < 10000) break;
			else
			{
				if(st_handler.cwnd_list != NULL)
				{  // 리스트 바 화면 존재 //
					sprintf(st_msg.c_abnormal_msg, "Heat Sink Job Req 응답이 없습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "There was no response Heat Sink Job Req");
					}
					

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
				}
				n_lotopen_step = 5000;
				//타임아읏 에러
			}
		}
		break;

	case 1000:
		NetworkConnectTime[1] = GetCurrentTime();
		NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];
		if(NetworkConnectTime[2] < 0) NetworkConnectTime[0] = GetCurrentTime();
		nRet_1 = Func.OnHeatSinkModel_Change_Rev();
		if(nRet_1 == RET_GOOD)
		{
			sprintf(st_msg.c_abnormal_msg, "Heat Sink Job Rcv 응답 완료.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "Complete Heat Sink Job Rcv response");
			}
			

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //

			NetworkConnectTime[0] = GetCurrentTime();
			n_lotopen_step = 2000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재 //
				sprintf(st_msg.c_abnormal_msg, "Heat Sink Job Rcv 응답이 없습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "No Heat Sink Job Rcv response");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
			}
			n_lotopen_step = 5000;
		}
		else
		{
			if(NetworkConnectTime[2] < 10000) break;
			else
			{
				if(st_handler.cwnd_list != NULL)
				{  // 리스트 바 화면 존재 //
					sprintf(st_msg.c_abnormal_msg, "Heat Sink Job Rcv 응답이 없습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "No Heat Sink Job Rcv response");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
				}
				n_lotopen_step = 5000;
				//타임아읏 에러
			}
		}
		break;

	case 1100:
		nRet_1 = Func.OnBCRModel_Change_Rev();
		if(nRet_1 == RET_GOOD)
		{
			n_lotopen_step = 2000;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_handler.cwnd_list != NULL)
			{  // 리스트 바 화면 존재 //
				sprintf(st_msg.c_abnormal_msg, "BCR Job Req 응답이 없습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "No BCR Job Req response");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
			}
			n_lotopen_step = 5000;
		}
		else
		{
			if(NetworkConnectTime[2] < 10000) break;
			else
			{
				if(st_handler.cwnd_list != NULL)
				{  // 리스트 바 화면 존재 //
					sprintf(st_msg.c_abnormal_msg, "BCR Job Req 응답이 없습니다.");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_abnormal_msg, "No BCR Job Req response");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
				}
				n_lotopen_step = 5000;
				//타임아읏 에러
			}
		}
		break;
		
	////2014,0406
	case 2000:
		if(st_work.n_hsNojob == 2)//LRDMM
		{
			st_basic.mstr_device_name = "LRDMM.TXT";
			::PostMessage(st_handler.hWnd, WM_DATA_CHANGE, 1, 0);
		}
		else if(st_work.n_hsNojob == 3)//RDMM
		{
			st_basic.mstr_device_name = "RDIMM.TXT";
			::PostMessage(st_handler.hWnd, WM_DATA_CHANGE, 1, 0);
		}

		st_work.b_Auto_Model_change = FALSE;

		NetworkConnectTime[0] = GetCurrentTime();
		n_lotopen_step = 2100;
		break;

	case 2100:
		NetworkConnectTime[1] = GetCurrentTime();
		NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];
		
		if(NetworkConnectTime[2] > 500)
		{
			if (st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
			}

			n_lotopen_step = 3000;
		}
		else if(NetworkConnectTime[2] < 0)
		{
			NetworkConnectTime[0] = GetCurrentTime();
		}
		break;

	////

	case 3000:
		if(st_work.b_Auto_Model_change == TRUE)
		{
			st_work.b_Auto_Model_change = FALSE;
			st_handler.mn_jobComplete = YES;
			FuncRet = RET_GOOD;
			n_lotopen_step = 0;
		}
		break;

	case 5000:
		FuncRet = RET_ERROR;
		n_lotopen_step = 0;
		break;

	default:
		break;
	}

	return FuncRet;
}

int CDialog_Lot_Open::OnLotEnd_Excution()
{
	int FuncRet = RET_PROCEED;
	int n_response;							// 대화 상자 리턴 플래그
	CString str_serial,str_partname;

	CDialog_Message msg_dlg;

	switch (n_lotopen_step)
	{
	case 0:
		// **********************************************************************
		//  서버와의 통신전에 접속 상태를 확인한다.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect == NO)		// 접속중이 아니면 접속을 시도한다.
		{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
			NetworkConnectTime[0] = GetCurrentTime();
			n_lotopen_step = 10;
		}
		else													// 접속중이면...
		{
			n_lotopen_step = 100;
		}
		break;

	case 10:
		// **********************************************************************
		//  접속을 확인한다.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != YES)
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 5000)	// 5초 동안 연결이 되지 않으면...
			{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
				
				n_lotopen_step = 5000;				
				st_msg.str_fallacy_msg = _T("[LABEL SERVER] 연결이 되지 않습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[LABEL SERVER] The connection will not be.");
				}

				n_response = msg_dlg.DoModal();

				if (n_response == IDOK)
				{
					
				}
			}
		}
		else											// 연결이 되었다.
		{
			n_lotopen_step = 100;
		}
		break;

	case 100:
		// **********************************************************************
		//  시작을 알린다.
		// **********************************************************************
		sprintf(st_client[NETWORK_CLIENT_1].ch_send,"%s",Func.OnNetworkDataMake(NW_LOT_END));

		//if (Ret == RET_GOOD)
		//{
			st_NW.mstr_result[1] = "";
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, NETWORK_CLIENT_1);	// 메인 프레임으로 메시지 전송
			NetworkConnectTime[0] = GetCurrentTime();

			n_lotopen_step = 200;
		//}
		break;

	case 200:
		// **********************************************************************
		//  완료가 될때까지 대기.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != NO)	// 작업이 끝나지 않았다면...
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 5000)	// 5초 동안 들어오지 않는다면...
			{
				n_lotopen_step = 5000;
				st_msg.str_fallacy_msg = _T("[LABEL SERVER] 5초간 답변이 오지 않았습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[LABEL SERVER] Answer did not come 5 seconds.");
				}

				n_response = msg_dlg.DoModal();

				if (n_response == IDOK)
				{
					
				}
			}
		}
		else if (st_client[NETWORK_CLIENT_1].n_connect == NO)	// 작업이 끝났다면...
		{
			n_lotopen_step = 300;
		}	
		break;

	case 300:
		// **********************************************************************
		//  Reply를 분석한다.
		// **********************************************************************
		if(st_NW.mstr_result[1] == "PASS")
		{
			n_lotopen_step = 400;
		}
		else								// 비정상이면...
		{
			n_lotopen_step = 5000;

			if (st_NW.mstr_msg[1] == "")
			{
				st_msg.str_fallacy_msg = _T("[LABEL SERVER - LIST] 비정상 데이터가 전송되었습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[LABEL SERVER] Data was sent to the abnormal.");
				}

			}
			else
			{	
				st_NW.mstr_msg[1].TrimRight(' ');
				st_NW.mstr_msg[1].TrimLeft(' ');
				st_msg.str_fallacy_msg.Format("[LABEL SERVER ERROR] : %s", st_NW.mstr_msg[1]);
			}
			n_response = msg_dlg.DoModal();

			if (n_response == IDOK)
			{
				
			}
		}
		break;

	case 400:
		Server_Data_Display();
		n_lotopen_step = 500;
		break;

	case 500:
		// 다 정상이니까, 테스터에게 새로운 LOT임을 알려준다.
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
		}
		FuncRet = RET_GOOD;
		n_lotopen_step = 0;
		break;

	case 5000:
		FuncRet = RET_ERROR;
		n_lotopen_step = 0;
		break;

	default:
		break;
	}

	return FuncRet;
}

int CDialog_Lot_Open::OnLotCancel_Excution()
{
	int FuncRet = RET_PROCEED;
	int n_response;							// 대화 상자 리턴 플래그
	CString str_serial,str_partname;

	CDialog_Message msg_dlg;

	switch (n_lotopen_step)
	{
	case 0:
		// **********************************************************************
		//  서버와의 통신전에 접속 상태를 확인한다.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect == NO)		// 접속중이 아니면 접속을 시도한다.
		{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
			NetworkConnectTime[0] = GetCurrentTime();
			n_lotopen_step = 10;
		}
		else													// 접속중이면...
		{
			n_lotopen_step = 100;
		}
		break;

	case 10:
		// **********************************************************************
		//  접속을 확인한다.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != YES)
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 5000)	// 5초 동안 연결이 되지 않으면...
			{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
				
				n_lotopen_step = 5000;				
				st_msg.str_fallacy_msg = _T("[LABEL SERVER] 연결이 되지 않습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[LABEL SERVER] The connection will not be.");
				}

				n_response = msg_dlg.DoModal();

				if (n_response == IDOK)
				{
					
				}
			}
		}
		else											// 연결이 되었다.
		{
			n_lotopen_step = 100;
		}
		break;

	case 100:
		// **********************************************************************
		//  시작을 알린다.
		// **********************************************************************
		sprintf(st_client[NETWORK_CLIENT_1].ch_send,"%s",Func.OnNetworkDataMake(NW_LOT_CANCEL));

		//if (Ret == RET_GOOD)
		//{
			st_NW.mstr_result[1] = "";
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, NETWORK_CLIENT_1);	// 메인 프레임으로 메시지 전송
			NetworkConnectTime[0] = GetCurrentTime();

			n_lotopen_step = 200;
		//}
		break;

	case 200:
		// **********************************************************************
		//  완료가 될때까지 대기.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != NO)	// 작업이 끝나지 않았다면...
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 10000)	// 5초 동안 들어오지 않는다면...
			{
				n_lotopen_step = 5000;
				st_msg.str_fallacy_msg = _T("[LABEL SERVER] 5초간 답변이 오지 않았습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[LABEL SERVER] Answer did not come 5 seconds.");
				}

				n_response = msg_dlg.DoModal();

				if (n_response == IDOK)
				{
					
				}
			}
		}
		else if (st_client[NETWORK_CLIENT_1].n_connect == NO)	// 작업이 끝났다면...
		{
			n_lotopen_step = 300;
		}	
		break;

	case 300:
		// **********************************************************************
		//  Reply를 분석한다.
		// **********************************************************************
		if(st_NW.mstr_result[1] == "PASS")
		{
			n_lotopen_step = 400;
		}
		else								// 비정상이면...
		{
			n_lotopen_step = 5000;

			if (st_NW.mstr_msg[1] == "")
			{
				st_msg.str_fallacy_msg = _T("[LABEL SERVER - LIST] 비정상 데이터가 전송되었습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("[LABEL SERVER] Data was sent to the abnormal.");
				}

			}
			else
			{	
				st_NW.mstr_msg[1].TrimRight(' ');
				st_NW.mstr_msg[1].TrimLeft(' ');
				st_msg.str_fallacy_msg.Format("[LABEL SERVER ERROR] : %s", st_NW.mstr_msg[1]);
			}
			n_response = msg_dlg.DoModal();

			if (n_response == IDOK)
			{
				
			}
		}
		break;

	case 400:
		Server_Data_Display();
		n_lotopen_step = 500;
		break;

	case 500:
		// 다 정상이니까, 테스터에게 새로운 LOT임을 알려준다.
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
		}
		FuncRet = RET_GOOD;
		n_lotopen_step = 0;
		break;

	case 5000:
		FuncRet = RET_ERROR;
		n_lotopen_step = 0;
		break;

	default:
		break;
	}

	return FuncRet;
}

void CDialog_Lot_Open::OnBtnLotEnd() 
{
	mn_work_mode = NW_LOT_END;

	UpdateData();

	n_lotopen_step = 0;
	SetTimer(TMR_LOT_END, 100, NULL);

	
	Func.OnNetworkDataMake(NW_LOT_END);
	
}

void CDialog_Lot_Open::OnBtnLotCancel() 
{
	mn_work_mode = NW_LOT_CANCEL;

	UpdateData();

	st_handler.mn_lot_cancel_mode = CTL_NO;	
	n_lotopen_step = 0;
	SetTimer(TMR_LOT_CANCEL, 100, NULL);

	
}

BOOL CDialog_Lot_Open::PreTranslateMessage(MSG* pMsg) 
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
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialog_Lot_Open::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here
	m_edit_Lot_open.SetWindowText(_T(""));
	
}
