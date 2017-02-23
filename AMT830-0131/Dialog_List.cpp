// Dialog_List.cpp : implementation file
//

#include "stdafx.h"
#include "HANDLER.h"
#include "Dialog_List.h"
#include <atlconv.h>  // TCP/IP ���� Ŭ���� �߰�

#include "Public_Function.h"
// ******************************************************************************
// ��Ʈ��ũ ��� ��� ����                                                       
// ******************************************************************************
const int SOCK_TCP = 0;
const int SOCK_UDP = 1;
// ******************************************************************************

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_List dialog


CDialog_List::CDialog_List()
{
	//{{AFX_DATA_INIT(CDialog_List)
	//}}AFX_DATA_INIT
	mstr_normal_old_msg = "";
	mstr_abnormal_old_msg = "";
}


void CDialog_List::DoDataExchange(CDataExchange* pDX)
{
	CInitDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_List)
	DDX_Control(pDX, IDC_EDIT_LIST_MSG, m_edit_list_msg);
	DDX_Control(pDX, IDC_DATA_MACHINE, m_data_machine);
	DDX_Control(pDX, IDC_DATA_DEVICE, m_data_device_name);
	DDX_Control(pDX, IDC_MSG_MACHINE, m_msg_machine);
	DDX_Control(pDX, IDC_MSG_DEVICE_NAME, m_msg_device_name);
	DDX_Control(pDX, IDC_LIST_DATA, m_list_data);
	DDX_Control(pDX, IDC_LIST_CLOCK, m_list_clock);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_List, CInitDialogBar)
	//{{AFX_MSG_MAP(CDialog_List)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LIST_DATA, OnListDataUpdate)  // ���� �޽��� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� �޽��� ����  
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_List message handlers
BOOL CDialog_List::OnInitDialogBar() 
{
	CInitDialogBar::OnInitDialogBar();

	// **************************************************************************
	// ��Ʈ�ѿ� ������ ��Ʈ �����Ѵ�                                             
	// **************************************************************************
	mp_list_font = NULL;
	mp_list_font = new CFont;
	mp_list_font->CreateFont(14,6,0,0,900,0,0,0,0,0,0,PROOF_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
		
	OnList_Digital_Clock_Set();				// �ð� ����.
	OnList_Check_Tab_Stops(TRUE);			// Tab ��� ���� ���� �Լ�
	OnList_Lable_Set();						// ��Ÿ ������ ���� Label ���� �Լ�
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialog_List::OnList_Digital_Clock_Set()
{
	m_list_clock.Start(IDB_CLOCKST_PANE, IDB_CLOCKST_BIG, IDB_CLOCKST_SMALL);
}

void CDialog_List::OnList_Check_Tab_Stops(int mb_use_tab_stops)
{
	DWORD dwRemove = (mb_use_tab_stops ? 0 : LBS_USETABSTOPS);
	DWORD dwAdd = (mb_use_tab_stops ? LBS_USETABSTOPS : 0);

	m_list_data.ModifyStyle(dwRemove, dwAdd);
	m_list_data.RedrawWindow();
}

void CDialog_List::OnList_Lable_Set()
{
	m_msg_machine.SetText("EQP NO");
	m_msg_machine.SetTextColor(RGB(0, 0, 0));
	m_msg_machine.SetBkColor(RGB(181, 182, 140));
	m_msg_machine.SetFontBold(TRUE);
	m_msg_machine.SetFontName("MS Sans Serif");
	m_msg_machine.SetFontSize(11);

	m_data_machine.SetFont(mp_list_font);
	m_data_machine.SetCenterText();
	m_data_machine.SetColor(RGB(0,0,0));
	m_data_machine.SetGradientColor(RGB(0,0,0));
	m_data_machine.SetTextColor(RGB(255,255,255));

	m_msg_device_name.SetText("DEVICE NAME");
	m_msg_device_name.SetTextColor(RGB(0, 0, 0));
	m_msg_device_name.SetBkColor(RGB(181, 182, 140));
	m_msg_device_name.SetFontBold(TRUE);
	m_msg_device_name.SetFontName("MS Sans Serif");
	m_msg_device_name.SetFontSize(11);

	m_data_device_name.SetFont(mp_list_font);
	m_data_device_name.SetCenterText();
	m_data_device_name.SetColor(RGB(0,0,0));
	m_data_device_name.SetGradientColor(RGB(0,0,0));
	m_data_device_name.SetTextColor(RGB(255,255,255));
}

void CDialog_List::OnList_Message_Clear()
{
	int nIndex = m_list_data.GetCount();

	if (nIndex < 500) return;

	m_list_data.ResetContent();   // ����Ʈ �ڽ� ��Ʈ�ѿ� �߰��� ��� ������ ���� 
}

void CDialog_List::OnList_Normal_Msg_Display()
{
/*	int mn_index;				// ����Ʈ �ڽ��� �߰��� ������ ��ġ ���� ���� 
	CString s;					// �ý��� �ð� ���� �ӽ� ���� ���� 
	CString mstr_name;			// �߰��� Ÿ��Ʋ ���� ���� ���� 
	CString mstr_msg = "\t";	// ����Ʈ �ڽ��� �߰��� ���� ���� ���� 
	CTime mp_cur_time;			// ���� �ð� ���� ���� ���� 

	char szMyName[] = "OK";
	char cp_cmd[] = "OPT";
	char szName[20];			// ��� Ÿ��Ʋ ���� ���� ���� 


	for (int i=0; i<20; i++)
		szName[i] = ' ';
 
	// **************************************************************************
    // �ø��� ��Ʈ �� �޽��� ������ ���� ���� ���� �κ�                          
    // **************************************************************************
	strncpy(szName, cp_cmd, sizeof(cp_cmd) -1 );
	strncpy(szName + 8, szMyName, sizeof(szMyName) -1 );
	strncpy(szName + 16, " : ", 3);
	szName[19] = '\0';
	// **************************************************************************

	mp_cur_time = CTime::GetCurrentTime() ;  // ���� �ð� ���� ��´�. 
	s = mp_cur_time.Format("%p %H %M %S") ;  // �ð� ���� ���� ���� (����/���� , �ð� , �� , �� )

	mstr_name = szName + s;  // �߰��� Ÿ��Ʋ ���� ���� 

	mn_index = m_list_data.AddString(mstr_name, RGB(0, 0, 255));  // ����Ʈ �ڽ� ��Ʈ�ѿ� Ÿ��Ʋ ���� �߰� 
	m_list_data.SetCurSel(mn_index);

	mstr_msg = "[NORMAL]";
	if (st_msg.str_normal_msg != _T(""))  mstr_msg += st_msg.str_normal_msg;
	else   mstr_msg += "Display Message is Empty";

//	Func.On_LogFile_Add(2, mstr_msg);
	Func.On_LogFile_Add(99, mstr_msg);

	mn_index = m_list_data.AddString(mstr_msg, RGB(25, 25, 85));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
	m_list_data.SetCurSel(mn_index);
*/
	int mn_index;	
	CString mstr_msg =  "";
	
	//2011.0504 �߰� ���� �ð� �̻��϶��� �޽��� ���� Ȯ���Ͽ� ���Ϸ� ���� 
	if (mstr_old_msg == st_msg.c_normal_msg)//st_msg.str_abnormal_msg) //2011.0829 test  if(st_msg.str_old_abnormal_msg == st_msg.str_abnormal_msg)
	{
		//2011.0504 �߰� ���� �޼����� ������ ���� ó�� 
		ml_list_msgterm_time_normal[1] = GetCurrentTime();
		ml_list_msgterm_time_normal[2] = ml_list_msgterm_time_normal[1] - ml_list_msgterm_time_normal[0];
		
		if (ml_list_msgterm_time_normal[2] > ml_listmsg_write_time) 
		{
			//OK ���Ϸ� ���� ���� 
		}
		else
		{
			ml_list_msgterm_time_normal[0] = GetCurrentTime();
			return;
		}
	}
	
	if (st_msg.c_normal_msg != _T(""))
	{
		ml_list_msgterm_time_normal[0] = GetCurrentTime();
		
		mstr_old_msg =	st_msg.c_normal_msg;
		mstr_msg   =	st_msg.c_normal_msg;
	}
	else
	{
		mstr_msg = "Display Message is Empty";
	}
	
	mn_index = m_list_data.AddString(mstr_msg, RGB(0, 0, 255));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
	m_list_data.SetCurSel(mn_index);
	
	Func.On_LogFile_Add(2, mstr_msg);
	Func.On_LogFile_Add(99, mstr_msg);
}

void CDialog_List::OnList_Abnormal_Msg_Display()
{
/*	int mn_index;				// ����Ʈ �ڽ��� �߰��� ������ ��ġ ���� ���� 
	CString s;					// �ý��� �ð� ���� �ӽ� ���� ���� 
	CString mstr_name;			// �߰��� Ÿ��Ʋ ���� ���� ���� 
	CString mstr_msg = "\t";	// ����Ʈ �ڽ��� �߰��� ���� ���� ���� 
	CTime mp_cur_time;			// ���� �ð� ���� ���� ���� 

	char szMyName[] = "ERR";
	char cp_cmd[] = "OPT";
	char szName[20];			// ��� Ÿ��Ʋ ���� ���� ���� 


	for (int i=0; i<20; i++)
		szName[i] = ' ';
 
	// **************************************************************************
    // �ø��� ��Ʈ �� �޽��� ������ ���� ���� ���� �κ�                          
    // **************************************************************************
	strncpy(szName, cp_cmd, sizeof(cp_cmd) -1 );
	strncpy(szName + 8, szMyName, sizeof(szMyName) -1 );
	strncpy(szName + 16, " : ", 3);
	szName[19] = '\0';
	// **************************************************************************

	mp_cur_time = CTime::GetCurrentTime() ;  // ���� �ð� ���� ��´�. 
	s = mp_cur_time.Format("%p %H %M %S") ;  // �ð� ���� ���� ���� (����/���� , �ð� , �� , �� )

	mstr_name = szName + s;  // �߰��� Ÿ��Ʋ ���� ���� 

	mn_index = m_list_data.AddString(mstr_name, RGB(255, 0, 0));  // ����Ʈ �ڽ� ��Ʈ�ѿ� Ÿ��Ʋ ���� �߰� 
	m_list_data.SetCurSel(mn_index);

	mstr_msg = "[ABNORMAL]";
	if (st_msg.str_abnormal_msg != _T(""))  mstr_msg += st_msg.str_abnormal_msg;
	else   mstr_msg += "Display Message is Empty";

	Func.On_LogFile_Add(99, mstr_msg);

	mn_index = m_list_data.AddString(mstr_msg, RGB(25, 25, 85));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
	m_list_data.SetCurSel(mn_index);
*/
	int mn_index;	
	CString mstr_msg =  "";
	
	//2011.0504 �߰� ���� �ð� �̻��϶��� �޽��� ���� Ȯ���Ͽ� ���Ϸ� ���� 
	if (mstr_old_msg == st_msg.c_abnormal_msg)//st_msg.str_abnormal_msg) //2011.0829 test  if(st_msg.str_old_abnormal_msg == st_msg.str_abnormal_msg)
	{
		//2011.0504 �߰� ���� �޼����� ������ ���� ó�� 
		ml_list_msgterm_time_abnormal[1] = GetCurrentTime();
		ml_list_msgterm_time_abnormal[2] = ml_list_msgterm_time_abnormal[1] - ml_list_msgterm_time_abnormal[0];
		
		if (ml_list_msgterm_time_abnormal[2] > ml_listmsg_write_time) 
		{
			//OK ���Ϸ� ���� ���� 
		}
		else
		{
			ml_list_msgterm_time_abnormal[0] = GetCurrentTime();
			return;
		}
	}
	
	if (st_msg.c_abnormal_msg != _T(""))
	{
		ml_list_msgterm_time_abnormal[0] = GetCurrentTime();
		
		mstr_old_msg =	st_msg.c_abnormal_msg;
		mstr_msg   =	st_msg.c_abnormal_msg;
	}
	else
	{
		mstr_msg = "Display Message is Empty";
	}
	
	mn_index = m_list_data.AddString(mstr_msg, RGB(255, 0, 0));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
	m_list_data.SetCurSel(mn_index);
	
	Func.On_LogFile_Add(2, mstr_msg);
	Func.On_LogFile_Add(99, mstr_msg);
}

void CDialog_List::OnList_Send_Msg_Display(int n_mode)
{
	int mn_index;				// ����Ʈ �ڽ��� �߰��� ������ ��ġ ���� ���� 
	CString s;					// �ý��� �ð� ���� �ӽ� ���� ���� 
	CString mstr_name;			// �ø��� ��Ʈ �� �ð� ���� ���� ���� 
	CString mstr_msg = "\t";	// ����Ʈ �ڽ��� �߰��� ���� ���� ���� 
	CTime mp_cur_time;			// ���� �ð� ���� ���� ���� 

	char szMyName[] = "SND";
	char szName[20];			// ��� Ÿ��Ʋ ���� ���� ���� 
	char cp_cmd[7];				// �ø��� ��Ʈ ���� ���� ���� 

	memset(&cp_cmd, 0, sizeof(cp_cmd)); // ���� �޸� �ʱ�ȭ 

	for(int i=0; i<20; i++)
		szName[i] = ' ';
 
	switch(n_mode)
	{
	case 0:
	default:
		// **************************************************************************
		// �ø��� ��Ʈ �� �޽��� ������ ���� ���� ���� �κ�                          
		// **************************************************************************
		sprintf(cp_cmd, "Com %02d", n_mode);
		break;
	}	
	strncpy(szName, cp_cmd, sizeof(cp_cmd) -1 );
	strncpy(szName + 8, szMyName, sizeof(szMyName) -1 );
	strncpy(szName + 16, " : ", 3);
	szName[19] = '\0';

	mp_cur_time = CTime::GetCurrentTime() ;  // ���� �ð� ���� ��´�. 
	s = mp_cur_time.Format("%p %H %M %S") ;  // �ð� ���� ���� ���� (����/���� , �ð� , �� , �� )

	mstr_name = szName + s;  // �߰��� Ÿ��Ʋ ���� ���� 

	mn_index = m_list_data.AddString(mstr_name, RGB(0, 0, 255));  // ����Ʈ �ڽ� ��Ʈ�ѿ� Ÿ��Ʋ ���� �߰� 
	m_list_data.SetCurSel(mn_index);

	st_msg.mstr_send = st_serial.comm_snd[n_mode-1];
	if (st_msg.mstr_send != _T(""))  mstr_msg += st_msg.mstr_send;
	else   mstr_msg += "Display Message is Empty";

	mn_index = m_list_data.AddString(mstr_msg, RGB(50, 100, 150));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
	m_list_data.SetCurSel(mn_index);
}

void CDialog_List::OnList_Receive_Msg_Display(int n_mode)
{
	int mn_index;		// ����Ʈ �ڽ��� �߰��� ������ ��ġ ���� ���� 
	CString s;			// �ý��� �ð� ���� �ӽ� ���� ���� 
	CString mstr_name;  // �ø��� ��Ʈ �� �ð� ���� ���� ���� 
	CString mstr_msg = "\t";  // ����Ʈ �ڽ��� �߰��� ���� ���� ���� 
	CTime mp_cur_time;  // ���� �ð� ���� ���� ���� 

	char szMyName[] = "RCV";
	char szName[20];	// ��� Ÿ��Ʋ ���� ���� ���� 
	char cp_cmd[7];		// �ø��� ��Ʈ ���� ���� ���� 

	memset(&cp_cmd, 0, sizeof(cp_cmd)); // ���� �޸� �ʱ�ȭ 

	for(int i=0; i<20; i++)
		szName[i] = ' ';
 
	switch(n_mode)
	{
	case 0:
	default:
		// **************************************************************************
		//  �ø��� ��Ʈ �� �޽��� ������ ���� ���� ���� �κ�                         
		// **************************************************************************
		sprintf(cp_cmd, "Com %02d", n_mode);
		break;
	}
	strncpy(szName, cp_cmd, sizeof(cp_cmd) -1 );
	strncpy(szName + 8, szMyName, sizeof(szMyName) -1 );
	strncpy(szName + 16, " : ", 3);
	szName[19] = '\0';
		
	mp_cur_time = CTime::GetCurrentTime() ;  // ���� �ð� ���� ��´�. 
	s = mp_cur_time.Format("%p %H %M %S") ;  // �ð� ���� ���� ���� (����/���� , �ð� , �� , �� )

	mstr_name = szName + s;  // �߰��� Ÿ��Ʋ ���� ���� 

	mn_index = m_list_data.AddString(mstr_name, RGB(255, 0, 255));  // ����Ʈ �ڽ� ��Ʈ�ѿ� Ÿ��Ʋ ���� �߰� 
	m_list_data.SetCurSel(mn_index);

	if (st_msg.mstr_recive != _T(""))  mstr_msg += st_msg.mstr_recive;
	else   mstr_msg += "Display Message is Empty";

	mn_index = m_list_data.AddString(mstr_msg, RGB(50, 100, 150));  // ���� �Էµ� ���� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� 
	m_list_data.SetCurSel(mn_index);
}

void CDialog_List::OnList_Machine_Info_Set()
{
	m_data_machine.SetWindowText(_T(st_lamp.mstr_equip_no));		// ��� ȣ�� ���� ��� 
}

void CDialog_List::OnList_Device_Info_Set()
{
	m_data_device_name.SetWindowText(_T(st_basic.mstr_device_name));		// DEVICE ���� ��� 
}

// ******************************************************************************
//  ����Ʈ �ڽ� ��Ʈ�ѿ� ���� �޽��� �߰���Ű�� ����� ���� �Լ�                 
// ******************************************************************************
LRESULT CDialog_List:: OnListDataUpdate(WPARAM wParam,LPARAM lParam)
{
	int n_mode = wParam;					// �ø��� ��Ʈ ���� �Լ� ������ ���� 

	OnList_Message_Clear();					// ����Ʈ �ڽ� ��Ʈ�ѿ� �߰��� ��� ������ ���� �Լ�

	switch(lParam)
	{
		case RECEIVE_MSG:						// ���� �޽��� 
			OnList_Receive_Msg_Display(n_mode);	// ����Ʈ �ڽ� ��Ʈ�ѿ� ���� �޽��� �߰� �Լ�
			break;

		case SEND_MSG:							// �۽� �޼���
			OnList_Send_Msg_Display(n_mode);	// ����Ʈ �ڽ� ��Ʈ�ѿ� �۽� �޽��� �߰� �Լ�
			break;

		case NORMAL_MSG:						// ���� ����
			OnList_Normal_Msg_Display();		// ����Ʈ �ڽ� ��Ʈ�ѿ� ���� �޽��� �߰� �Լ�
			break;

		case ABNORMAL_MSG:						// ������ ����
			OnList_Abnormal_Msg_Display();		// ����Ʈ �ڽ� ��Ʈ�ѿ� ���� �޽��� �߰� �Լ�
			break;		

		case MACHINE_INFO:						// ��� ���� ����
			OnList_Machine_Info_Set();
			break;

		case DEVICE_INFO:						// ����̽��� ���� ����
			OnList_Device_Info_Set();
			break;
	}

	return 0 ;
}
// ******************************************************************************

BOOL CDialog_List::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	// **************************************************************************
	// ������ ��Ʈ ���� �����Ѵ�                                                 
	// **************************************************************************
	delete mp_list_font;
	mp_list_font = NULL;

	// **************************************************************************
	return CInitDialogBar::DestroyWindow();
}

void CDialog_List::OnPick_Next_Available()
{
/*	mp_cur_server = NULL;  // ���� ���� �ʱ�ȭ
	
	for(int i=0; i<MAX_CONNECTION; i++)  // ����� ���� ������ŭ ���� �Ŵ��� ����
	{
		if (!m_socket_manager[i].IsOpen())  // ���ӵ� ������ �ƴ� ���
		{
			mp_cur_server = &m_socket_manager[i];  // ���ο� ���� ����
			break;
		}
	}*/
}

void CDialog_List::OnTimer(UINT nIDEvent) 
{
	CInitDialogBar::OnTimer(nIDEvent);
}

void CDialog_List::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	// **************************************************************************
	// ������ ��Ʈ ���� �����Ѵ�                                                 
	// **************************************************************************
	delete mp_list_font;
	mp_list_font = NULL;

	// **************************************************************************
	CInitDialogBar::PostNcDestroy();
}






