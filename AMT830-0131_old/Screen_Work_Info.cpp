// Screen_Work_Info.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Work_Info.h"
#include "Dialog_Work_Info_Tab.h"
#include "Dialog_Pass_Check.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Work_Info

IMPLEMENT_DYNCREATE(CScreen_Work_Info, CFormView)

CScreen_Work_Info::CScreen_Work_Info()
	: CFormView(CScreen_Work_Info::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Work_Info)
	//}}AFX_DATA_INIT
}

CScreen_Work_Info::~CScreen_Work_Info()
{
}

void CScreen_Work_Info::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Work_Info)
	DDX_Control(pDX, IDC_CHK_MAINTENANCE, m_chk_maintenance);
	DDX_Control(pDX, IDC_CHK_INTERFACE, m_chk_interface);
	DDX_Control(pDX, IDC_TAB_WORK_INFO, m_tab_work_info);
	DDX_Control(pDX, IDC_CHK_WAIT_TIME, m_chk_wait_time);
//////////////////////////////////////////////////////////// 20120521
	DDX_Control(pDX, IDC_CHK_BARCODE_PRINT, m_chk_barcode_print);
	DDX_Control(pDX, IDC_CHK_SKIP_MODE, m_chk_skip_mode);
////////////////////////////////////////////////////////////
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Work_Info, CFormView)
	//{{AFX_MSG_MAP(CScreen_Work_Info)
	ON_BN_CLICKED(IDC_CHK_MAINTENANCE, OnChkMaintenance)
	ON_BN_CLICKED(IDC_CHK_INTERFACE, OnChkInterface)
 	ON_BN_CLICKED(IDC_CHK_WAIT_TIME, OnChkWaitTime)
	ON_BN_CLICKED(IDC_CHK_BARCODE_PRINT, OnChkBarcodePrint)
	ON_BN_CLICKED(IDC_CHK_SKIP_MODE, OnChkSkipMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Work_Info diagnostics

#ifdef _DEBUG
void CScreen_Work_Info::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Work_Info::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Work_Info message handlers

void CScreen_Work_Info::OnInitialUpdate() 
{
	// TODO: Add your specialized code here and/or call the base class
	CFormView::OnInitialUpdate();

	CRect rect_area;
	rect_area.SetRect(214, 151, 1256, 916);

	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	Init_Button();

	m_tab_work_info.Init_Tab(this, rect_area);
}

BOOL CScreen_Work_Info::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}

	for(int i=1; i<MAX_PORT; i++)//20120620
	{
		if(i == 2)  break;
		::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, YES, i);
	}

	return CFormView::DestroyWindow();
}


void CScreen_Work_Info::OnChkMaintenance() 
{
	// TODO: Add your control notification handler code here
	if(!m_chk_maintenance.GetCheck())
	{
		m_chk_maintenance.SetCheck(TRUE);
		return;
	}
	
	if(m_chk_interface.GetCheck())
	{
		m_chk_interface.SetCheck(FALSE);
	}
	
	if(m_chk_wait_time.GetCheck())
	{
		m_chk_wait_time.SetCheck(FALSE);
	}

	////////////////////////////////////////////// 20120521
	if(m_chk_barcode_print.GetCheck())
	{
		m_chk_barcode_print.SetCheck(FALSE);
	}
	///////////////////////////////////////// 20120525
	if(m_chk_skip_mode.GetCheck())
	{
		m_chk_skip_mode.SetCheck(FALSE);
	}

	//////////////////////////////////////////////


// 	
// 	if(m_chk_motor_speed.GetCheck())
// 	{
// 		m_chk_motor_speed.SetCheck(FALSE);
// 	}

// 	if(m_chk_write_rfid.GetCheck())
// 	{
// 		m_chk_write_rfid.SetCheck(FALSE);
// 	}
	
	m_tab_work_info.OnWork_Change(0);
}

void CScreen_Work_Info::OnChkInterface() 
{
	// TODO: Add your control notification handler code here
	if(!m_chk_interface.GetCheck())
	{
		m_chk_interface.SetCheck(TRUE);
		return;
	}
	
	if(m_chk_maintenance.GetCheck())
	{
		m_chk_maintenance.SetCheck(FALSE);
	}
	
	if(m_chk_wait_time.GetCheck())
	{
		m_chk_wait_time.SetCheck(FALSE);
	}

	////////////////////////////////////////////// 20120521
	if(m_chk_barcode_print.GetCheck())
	{
		m_chk_barcode_print.SetCheck(FALSE);
	}
	///////////////////////////////////////// 20120525
	if(m_chk_skip_mode.GetCheck())
	{
		m_chk_skip_mode.SetCheck(FALSE);
	}

	//////////////////////////////////////////////


// 	
// 	if(m_chk_motor_speed.GetCheck())
// 	{
// 		m_chk_motor_speed.SetCheck(FALSE);
// 	}

// 	if(m_chk_write_rfid.GetCheck())
// 	{
// 		m_chk_write_rfid.SetCheck(FALSE);
// 	}

	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, NO, 1);

	m_tab_work_info.OnWork_Change(1);
}

// void CScreen_Work_Info::OnChkMotorSpeed() 
// {
// 	CDialog_Pass_Check	pass_dlg;
// 	int					n_response;
// 	// **************************************************************************
// 	// 메뉴 사용 불가능한 경우에 대한 조건 검사한다                              
// 	// -> 메인트 모드 또는 티칭 모드가 설정되어 있는지 검사                      
// 	// **************************************************************************
// 	
// 	
// 	
// 	n_response = pass_dlg.DoModal();
// 	
// 	if (n_response != IDOK)
// 	{
// 		if(m_chk_motor_speed.GetCheck())
// 		{
// 			m_chk_motor_speed.SetCheck(FALSE);
// 		}
// 		return;
// 	}
// 
// 	if(!m_chk_motor_speed.GetCheck())
// 	{
// 		m_chk_motor_speed.SetCheck(TRUE);
// 		return;
// 	}
// 	
// 	if(m_chk_maintenance.GetCheck())
// 	{
// 		m_chk_maintenance.SetCheck(FALSE);
// 	}
// 
// 	if(m_chk_interface.GetCheck())
// 	{
// 		m_chk_interface.SetCheck(FALSE);
// 	}
// 	
// // 	if(m_chk_wait_time.GetCheck())
// // 	{
// // 		m_chk_wait_time.SetCheck(FALSE);
// // 	}
// // 	
// // 	if(m_chk_write_rfid.GetCheck())
// // 	{
// // 		m_chk_write_rfid.SetCheck(FALSE);
// // 	}
// 
// 	
// 	m_tab_work_info.OnWork_Change(3);
// }

void CScreen_Work_Info::OnChkWaitTime() 
{
	// TODO: Add your control notification handler code here
	if(!m_chk_wait_time.GetCheck())
	{
		m_chk_wait_time.SetCheck(TRUE);
		return;
	}
	
	if(m_chk_maintenance.GetCheck())
	{
		m_chk_maintenance.SetCheck(FALSE);
	}

	if(m_chk_interface.GetCheck())
	{
		m_chk_interface.SetCheck(FALSE);
	}

	////////////////////////////////////////////// 20120521
	if(m_chk_barcode_print.GetCheck())
	{
		m_chk_barcode_print.SetCheck(FALSE);
	}
	///////////////////////////////////////// 20120525
	if(m_chk_skip_mode.GetCheck())
	{
		m_chk_skip_mode.SetCheck(FALSE);
	}
	/////////////////////////////////////////


// 	if(m_chk_motor_speed.GetCheck())
// 	{
// 		m_chk_motor_speed.SetCheck(FALSE);
// 	}
// 
// 	if(m_chk_write_rfid.GetCheck())
// 	{
// 		m_chk_write_rfid.SetCheck(FALSE);
// 	}

	m_tab_work_info.OnWork_Change(2);
}

//////////////////////////////////////////////// 20120521
void CScreen_Work_Info::OnChkBarcodePrint() 
{
	// TODO: Add your control notification handler code here
	if(!m_chk_barcode_print.GetCheck())
	{
		m_chk_barcode_print.SetCheck(TRUE);
		return;
	}
	
	if(m_chk_maintenance.GetCheck())
	{
		m_chk_maintenance.SetCheck(FALSE);
	}

	if(m_chk_interface.GetCheck())
	{
		m_chk_interface.SetCheck(FALSE);
	}
	///////////////////////////////////////// 20120525
	if(m_chk_skip_mode.GetCheck())
	{
		m_chk_skip_mode.SetCheck(FALSE);
	}
	/////////////////////////////////////////


// 	if(m_chk_motor_speed.GetCheck())
// 	{
// 		m_chk_motor_speed.SetCheck(FALSE);
// 	}
// 
// 	if(m_chk_write_rfid.GetCheck())
// 	{
// 		m_chk_write_rfid.SetCheck(FALSE);
// 	}

	if(m_chk_wait_time.GetCheck())
	{
		m_chk_wait_time.SetCheck(FALSE);
	}

	m_tab_work_info.OnWork_Change(3);
}
////////////////////////////////////////////////

void CScreen_Work_Info::Init_Button()
{
	short	shBtnColor = 30;

	m_chk_maintenance.SetFont(m_p_font);
 	m_chk_maintenance.SetIcon(IDI_MAINTENANCE_1);
	m_chk_maintenance.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_maintenance.SetWindowText("Maintenance");
	m_chk_maintenance.SetButtonColor(1);
	m_chk_maintenance.SetCheck(TRUE);

	m_chk_interface.SetFont(m_p_font);
 	m_chk_interface.SetIcon(IDI_MAINTENANCE_1);
	m_chk_interface.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_interface.SetWindowText("Interface");
	m_chk_interface.SetButtonColor(1);
	m_chk_interface.SetCheck(FALSE);

	m_chk_wait_time.SetFont(m_p_font);
 	m_chk_wait_time.SetIcon(IDI_MAINTENANCE_1);
	m_chk_wait_time.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_wait_time.SetWindowText("Wait Time");
	m_chk_wait_time.SetButtonColor(1);
	m_chk_wait_time.SetCheck(FALSE);

	//////////////////////////////////////////// 20120521
	m_chk_barcode_print.SetFont(m_p_font);
	m_chk_barcode_print.SetIcon(IDI_MAINTENANCE_1);
	m_chk_barcode_print.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_barcode_print.SetWindowText("Barcode Print");
	m_chk_barcode_print.SetButtonColor(1);
	m_chk_barcode_print.SetCheck(FALSE);
	////////////////////////////////////////////	20120525
	m_chk_skip_mode.SetFont(m_p_font);
	m_chk_skip_mode.SetIcon(IDI_MAINTENANCE_1);
	m_chk_skip_mode.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_mode.SetWindowText("Skip");
	m_chk_skip_mode.SetButtonColor(1);
	m_chk_skip_mode.SetCheck(FALSE);

	////////////////////////////////////////////

// 
// 	m_chk_motor_speed.SetFont(m_p_font);
//  	m_chk_motor_speed.SetIcon(IDI_MAINTENANCE_1);
// 	m_chk_motor_speed.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
// 	m_chk_motor_speed.SetWindowText("Motor Speed");
// 	m_chk_motor_speed.SetButtonColor(1);
// 	m_chk_motor_speed.SetCheck(FALSE);
// 
// 	m_chk_write_rfid.SetFont(m_p_font);
//  	m_chk_write_rfid.SetIcon(IDI_MAINTENANCE_1);
// 	m_chk_write_rfid.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
// 	m_chk_write_rfid.SetWindowText("Rfid write");
// 	m_chk_write_rfid.SetButtonColor(1);
// 	m_chk_write_rfid.SetCheck(FALSE);
}


// void CScreen_Work_Info::OnChkWriteRfid() 
// {
// 	// TODO: Add your control notification handler code here
// 
// 	if(!m_chk_write_rfid.GetCheck())
// 	{
// 		m_chk_write_rfid.SetCheck(TRUE);
// 		return;
// 	}
// 	if(m_chk_interface.GetCheck())
// 	{
// 		m_chk_interface.SetCheck(FALSE);
// 	}
// 	
// 	if(m_chk_maintenance.GetCheck())
// 	{
// 		m_chk_maintenance.SetCheck(FALSE);
// 	}
// 	
// 	if(m_chk_wait_time.GetCheck())
// 	{
// 		m_chk_wait_time.SetCheck(FALSE);
// 	}
// 	
// 	if(m_chk_motor_speed.GetCheck())
// 	{
// 		m_chk_motor_speed.SetCheck(FALSE);
// 	}
// 
// 
// //	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, NO, 0);
// 
// 	m_tab_work_info.OnWork_Change(4);	
// }

/////////////////////////////////////////// 20120525
void CScreen_Work_Info::OnChkSkipMode() 
{
	// TODO: Add your control notification handler code here
	if(!m_chk_skip_mode.GetCheck())
	{
		m_chk_skip_mode.SetCheck(TRUE);
		return;
	}

	if(m_chk_wait_time.GetCheck())
	{
		m_chk_wait_time.SetCheck(FALSE);
		
	}
	
	if(m_chk_maintenance.GetCheck())
	{
		m_chk_maintenance.SetCheck(FALSE);
	}

	if(m_chk_interface.GetCheck())
	{
		m_chk_interface.SetCheck(FALSE);
	}

	if(m_chk_barcode_print.GetCheck())
	{
		m_chk_barcode_print.SetCheck(FALSE);
	}

// 	if(m_chk_motor_speed.GetCheck())
// 	{
// 		m_chk_motor_speed.SetCheck(FALSE);
// 	}
// 
// 	if(m_chk_write_rfid.GetCheck())
// 	{
// 		m_chk_write_rfid.SetCheck(FALSE);
// 	}

	m_tab_work_info.OnWork_Change(4);
}