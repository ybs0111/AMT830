// Dialog_Work_Info_Tab.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Work_Info_Tab.h"
#include "Dialog_Work_Maintenane.h"
#include "Dialog_Work_Interface.h"
#include "Dialog_Work_Wait_Time.h"
#include "Dialog_Work_Barcode_Print.h"	//20120521
#include "Dialog_Work_Skip_Mode.h"	//20120525

// #include "Dialog_Work_Motor_Speed.h"
// #include "Dialog_Work_Maz_Interface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Info_Tab

CDialog_Work_Info_Tab::CDialog_Work_Info_Tab()
{
	m_p_maintenance		= NULL;
	m_p_interface		= NULL;
// 	m_p_motor_speed		= NULL;
	m_p_wait_time		= NULL;
	m_p_skip_mode		= NULL;	//20120525
	m_p_barcode_print	= NULL;	//20120521

// 	m_p_maz_interface	= NULL;
}

CDialog_Work_Info_Tab::~CDialog_Work_Info_Tab()
{
	if(m_p_maintenance != NULL)
	{
		m_p_maintenance->DestroyWindow();
		delete m_p_maintenance;
		m_p_maintenance = NULL;
 	}
	
	if(m_p_interface != NULL)
	{
		m_p_interface->DestroyWindow();
		delete m_p_interface;
		m_p_interface = NULL;
	}

	if(m_p_wait_time != NULL)
	{
		m_p_wait_time->DestroyWindow();
		delete m_p_wait_time;
		m_p_wait_time = NULL;
 	}
	
	////////////////////////////////////// 20120521
	if(m_p_barcode_print != NULL)
	{
		m_p_barcode_print->DestroyWindow();
		delete m_p_barcode_print;
		m_p_barcode_print = NULL;
 	}
	////////////////////////////////////// 20120525
	if(m_p_skip_mode != NULL)
	{
		m_p_skip_mode->DestroyWindow();
		delete m_p_skip_mode;
		m_p_skip_mode = NULL;
 	}

	//////////////////////////////////////

}


BEGIN_MESSAGE_MAP(CDialog_Work_Info_Tab, CTabCtrl)
	//{{AFX_MSG_MAP(CDialog_Work_Info_Tab)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Info_Tab message handlers

void CDialog_Work_Info_Tab::Init_Tab(CWnd *p_wnd, CRect rect_area)
{
	m_rect_area = rect_area;

	m_p_maintenance		= new CDialog_Work_Maintenane;
	m_p_maintenance->Create(IDD_WORK_MAINTENACE, this);

	m_p_interface		= new CDialog_Work_Interface;
	m_p_interface->Create(IDD_WORK_INTERFACE, this);

	m_p_wait_time		= new CDialog_Work_Wait_Time;
	m_p_wait_time->Create(IDD_WORK_WAIT_TIME, this);

	////////////////////////////////////////////////////////////// 20120521
	m_p_barcode_print		= new CDialog_Work_Barcode_Print;
	m_p_barcode_print->Create(IDD_WORK_BARCODE_PRINT, this);
	////////////////////////////////////////////////////////////// 20120525
	m_p_skip_mode		= new CDialog_Work_Skip_Mode;
	m_p_skip_mode->Create(IDD_WORK_SKIP_MODE, this);
	
	////////////////////////////////////////////////////////////// 

// 
// 	m_p_motor_speed		= new CDialog_Work_Motor_Speed;
// 	m_p_motor_speed->Create(IDD_WORK_MOTOR_SPEED, this);
// 
// 	m_p_maz_interface	= new CDialog_Work_Maz_Interface;
// 	m_p_maz_interface->Create(IDD_WORK_RFID_WRITE, this);
	OnWork_Change(0);
}

void CDialog_Work_Info_Tab::OnWork_Change(int n_mode)
{
	switch(n_mode)
	{
		case 0:
			if(m_p_maintenance != NULL)
			{
				m_p_maintenance->MoveWindow(m_rect_area, TRUE);
				m_p_maintenance->ShowWindow(SW_SHOW);
			}
			
			if(m_p_interface != NULL)
			{
				m_p_interface->ShowWindow(SW_HIDE);
			}

			if(m_p_wait_time != NULL)
			{
				m_p_wait_time->ShowWindow(SW_HIDE);
			}

			////////////////////////////////////////////// 20120521
			if(m_p_barcode_print != NULL)
			{
				m_p_barcode_print->ShowWindow(SW_HIDE);
			}
			////////////////////////////////////////////// 20120525
			if(m_p_skip_mode != NULL)
			{
				m_p_skip_mode->ShowWindow(SW_HIDE);
			}


			//////////////////////////////////////////////

			break;

		case 1:
			if(m_p_maintenance != NULL)
			{
				m_p_maintenance->ShowWindow(SW_HIDE);
			}
			
			if(m_p_interface != NULL)
			{
				m_p_interface->MoveWindow(m_rect_area.left, m_rect_area.top, m_rect_area.Width(), m_rect_area.Height(), TRUE);
				m_p_interface->ShowWindow(SW_SHOW);
			}

			if(m_p_wait_time != NULL)
			{
				m_p_wait_time->ShowWindow(SW_HIDE);
			}

			////////////////////////////////////////////// 20120521
			if(m_p_barcode_print != NULL)
			{
				m_p_barcode_print->ShowWindow(SW_HIDE);
			}

			////////////////////////////////////////////// 20120525
			if(m_p_skip_mode != NULL)
			{
				m_p_skip_mode->ShowWindow(SW_HIDE);
			}

			//////////////////////////////////////////////

			break;

		case 2:
			if(m_p_maintenance != NULL)
			{
				m_p_maintenance->ShowWindow(SW_HIDE);
			}
			
			if(m_p_interface != NULL)
			{
				m_p_interface->ShowWindow(SW_HIDE);
			}

			if(m_p_wait_time != NULL)
			{
				m_p_wait_time->MoveWindow(m_rect_area.left, m_rect_area.top, m_rect_area.Width(), m_rect_area.Height(), TRUE);
				m_p_wait_time->ShowWindow(SW_SHOW);
			}

			////////////////////////////////////////////// 20120521
			if(m_p_barcode_print != NULL)
			{
				m_p_barcode_print->ShowWindow(SW_HIDE);
			}

			////////////////////////////////////////////// 20120525
			if(m_p_skip_mode != NULL)
			{
				m_p_skip_mode->ShowWindow(SW_HIDE);
			}

			//////////////////////////////////////////////



			break;

		case 3:
			if(m_p_maintenance != NULL)
			{
				m_p_maintenance->ShowWindow(SW_HIDE);
			}
			
			if(m_p_interface != NULL)
			{
				m_p_interface->ShowWindow(SW_HIDE);
			}
			////////////////////////////////////////////// 20120521
			if(m_p_wait_time != NULL)
			{
				m_p_wait_time->ShowWindow(SW_HIDE);
			}

			if(m_p_barcode_print != NULL)
			{
				m_p_barcode_print->MoveWindow(m_rect_area.left, m_rect_area.top, m_rect_area.Width(), m_rect_area.Height(), TRUE);
				m_p_barcode_print->ShowWindow(SW_SHOW);
			}
			//////////////////////////////////////////////
			
			////////////////////////////////////////////// 20120525
			if(m_p_skip_mode != NULL)
			{
				m_p_skip_mode->ShowWindow(SW_HIDE);
			}

			//////////////////////////////////////////////
			
			break;

		case 4:
			//////////////////////////////////////////////////// 20120525
			if(m_p_maintenance != NULL)
			{
				m_p_maintenance->ShowWindow(SW_HIDE);
			}
			
			if(m_p_interface != NULL)
			{
				m_p_interface->ShowWindow(SW_HIDE);
			}

			if(m_p_wait_time != NULL)
			{
				m_p_wait_time->ShowWindow(SW_HIDE);
			}

			if(m_p_barcode_print != NULL)
			{
				m_p_barcode_print->ShowWindow(SW_HIDE);
			}

			if(m_p_skip_mode != NULL)
			{
				m_p_skip_mode->MoveWindow(m_rect_area.left, m_rect_area.top, m_rect_area.Width(), m_rect_area.Height(), TRUE);
				m_p_skip_mode->ShowWindow(SW_SHOW);
			}

			//////////////////////////////////////////////
			break;
	}
}
