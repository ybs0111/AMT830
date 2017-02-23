// Dialog_Motor_Axis_Tab.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Motor_Axis_Tab.h"
#include "Dialog_Motor_Axis_1.h"
#include "Dialog_Motor_Axis_2.h"
#include "Dialog_Motor_Axis_3.h"
#include "Dialog_Motor_Axis_4.h"
#include "Dialog_Motor_Axis_5.h"
#include "Dialog_Motor_Axis_6.h"
#include "Dialog_Motor_Axis_7.h"
#include "Dialog_Motor_Axis_8.h"
#include "Dialog_Motor_Axis_9.h"
#include "Dialog_Motor_Axis_10.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Axis_Tab
CDialog_Motor_Axis_Tab::CDialog_Motor_Axis_Tab()
{
	m_p_dialog_motor_axis_1		= NULL;
	m_p_dialog_motor_axis_2		= NULL;
	m_p_dialog_motor_axis_3		= NULL;
	m_p_dialog_motor_axis_4		= NULL;
	m_p_dialog_motor_axis_5		= NULL;
	m_p_dialog_motor_axis_6		= NULL;
	m_p_dialog_motor_axis_7		= NULL;
	m_p_dialog_motor_axis_8		= NULL;
	m_p_dialog_motor_axis_9		= NULL;
	m_p_dialog_motor_axis_10	= NULL;
}

CDialog_Motor_Axis_Tab::~CDialog_Motor_Axis_Tab()
{
	OnMotor_Axis_Delete();
}


BEGIN_MESSAGE_MAP(CDialog_Motor_Axis_Tab, CTabCtrl)
	//{{AFX_MSG_MAP(CDialog_Motor_Axis_Tab)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Motor_Axis_Tab message handlers

void CDialog_Motor_Axis_Tab::Init_Tab(CWnd *p_wnd, int n_part)
{
	int i;

	m_n_part	= n_part;

	for(i=0; i<st_motor_info.n_part_axis_cnt[m_n_part]; i++)
	{
		OnMotor_Axis_Create(i);
	}

	OnMotor_Axis_Change(0);
}

void CDialog_Motor_Axis_Tab::OnMotor_Axis_Change(int n_mode)
{
	if(st_handler.mn_menu_lock) return;

	switch(n_mode)
	{
		case 0:
			if(m_p_dialog_motor_axis_1 != NULL)
			{
				m_p_dialog_motor_axis_1->ShowWindow(SW_SHOW);
			}

			if(m_p_dialog_motor_axis_2 != NULL)
			{
				m_p_dialog_motor_axis_2->ShowWindow(SW_HIDE);
			}

			if(m_p_dialog_motor_axis_3 != NULL)
			{
				m_p_dialog_motor_axis_3->ShowWindow(SW_HIDE);
			}

			if(m_p_dialog_motor_axis_4 != NULL)
			{
				m_p_dialog_motor_axis_4->ShowWindow(SW_HIDE);
			}

			if(m_p_dialog_motor_axis_5 != NULL)
			{
				m_p_dialog_motor_axis_5->ShowWindow(SW_HIDE);
			}

			if(m_p_dialog_motor_axis_6 != NULL)
			{
				m_p_dialog_motor_axis_6->ShowWindow(SW_HIDE);
			}

			if(m_p_dialog_motor_axis_7 != NULL)
			{
				m_p_dialog_motor_axis_7->ShowWindow(SW_HIDE);
			}

			if(m_p_dialog_motor_axis_8 != NULL)
			{
				m_p_dialog_motor_axis_8->ShowWindow(SW_HIDE);
			}

			if(m_p_dialog_motor_axis_9 != NULL)
			{
				m_p_dialog_motor_axis_9->ShowWindow(SW_HIDE);
			}

			if(m_p_dialog_motor_axis_10 != NULL)
			{
				m_p_dialog_motor_axis_10->ShowWindow(SW_HIDE);
			}
			break;

		case 1:
			if(m_p_dialog_motor_axis_1 != NULL)
			{
				m_p_dialog_motor_axis_1->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_2 != NULL)
			{
				m_p_dialog_motor_axis_2->ShowWindow(SW_SHOW);
			}

			if(m_p_dialog_motor_axis_3 != NULL)
			{
				m_p_dialog_motor_axis_3->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_4 != NULL)
			{
				m_p_dialog_motor_axis_4->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_5 != NULL)
			{
				m_p_dialog_motor_axis_5->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_6 != NULL)
			{
				m_p_dialog_motor_axis_6->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_7 != NULL)
			{
				m_p_dialog_motor_axis_7->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_8 != NULL)
			{
				m_p_dialog_motor_axis_8->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_9 != NULL)
			{
				m_p_dialog_motor_axis_9->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_10 != NULL)
			{
				m_p_dialog_motor_axis_10->ShowWindow(SW_HIDE);
			}
			break;

		case 2:
			if(m_p_dialog_motor_axis_1 != NULL)
			{
				m_p_dialog_motor_axis_1->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_2 != NULL)
			{
				m_p_dialog_motor_axis_2->ShowWindow(SW_HIDE);
			}

			if(m_p_dialog_motor_axis_3 != NULL)
			{
				m_p_dialog_motor_axis_3->ShowWindow(SW_SHOW);
			}
			
			if(m_p_dialog_motor_axis_4 != NULL)
			{
				m_p_dialog_motor_axis_4->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_5 != NULL)
			{
				m_p_dialog_motor_axis_5->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_6 != NULL)
			{
				m_p_dialog_motor_axis_6->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_7 != NULL)
			{
				m_p_dialog_motor_axis_7->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_8 != NULL)
			{
				m_p_dialog_motor_axis_8->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_9 != NULL)
			{
				m_p_dialog_motor_axis_9->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_10 != NULL)
			{
				m_p_dialog_motor_axis_10->ShowWindow(SW_HIDE);
			}
			break;

		case 3:
			if(m_p_dialog_motor_axis_1 != NULL)
			{
				m_p_dialog_motor_axis_1->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_2 != NULL)
			{
				m_p_dialog_motor_axis_2->ShowWindow(SW_HIDE);
			}
			if(m_p_dialog_motor_axis_3 != NULL)
			{
				m_p_dialog_motor_axis_3->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_4 != NULL)
			{
				m_p_dialog_motor_axis_4->ShowWindow(SW_SHOW);
			}
			
			if(m_p_dialog_motor_axis_5 != NULL)
			{
				m_p_dialog_motor_axis_5->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_6 != NULL)
			{
				m_p_dialog_motor_axis_6->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_7 != NULL)
			{
				m_p_dialog_motor_axis_7->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_8 != NULL)
			{
				m_p_dialog_motor_axis_8->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_9 != NULL)
			{
				m_p_dialog_motor_axis_9->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_10 != NULL)
			{
				m_p_dialog_motor_axis_10->ShowWindow(SW_HIDE);
			}
			break;

		case 4:
			if(m_p_dialog_motor_axis_1 != NULL)
			{
				m_p_dialog_motor_axis_1->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_2 != NULL)
			{
				m_p_dialog_motor_axis_2->ShowWindow(SW_HIDE);
			}
			if(m_p_dialog_motor_axis_3 != NULL)
			{
				m_p_dialog_motor_axis_3->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_4 != NULL)
			{
				m_p_dialog_motor_axis_4->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_5 != NULL)
			{
				m_p_dialog_motor_axis_5->ShowWindow(SW_SHOW);
			}
			
			if(m_p_dialog_motor_axis_6 != NULL)
			{
				m_p_dialog_motor_axis_6->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_7 != NULL)
			{
				m_p_dialog_motor_axis_7->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_8 != NULL)
			{
				m_p_dialog_motor_axis_8->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_9 != NULL)
			{
				m_p_dialog_motor_axis_9->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_10 != NULL)
			{
				m_p_dialog_motor_axis_10->ShowWindow(SW_HIDE);
			}
			break;

		case 5:
			if(m_p_dialog_motor_axis_1 != NULL)
			{
				m_p_dialog_motor_axis_1->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_2 != NULL)
			{
				m_p_dialog_motor_axis_2->ShowWindow(SW_HIDE);
			}
			if(m_p_dialog_motor_axis_3 != NULL)
			{
				m_p_dialog_motor_axis_3->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_4 != NULL)
			{
				m_p_dialog_motor_axis_4->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_5 != NULL)
			{
				m_p_dialog_motor_axis_5->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_6 != NULL)
			{
				m_p_dialog_motor_axis_6->ShowWindow(SW_SHOW);
			}
			
			if(m_p_dialog_motor_axis_7 != NULL)
			{
				m_p_dialog_motor_axis_7->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_8 != NULL)
			{
				m_p_dialog_motor_axis_8->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_9 != NULL)
			{
				m_p_dialog_motor_axis_9->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_10 != NULL)
			{
				m_p_dialog_motor_axis_10->ShowWindow(SW_HIDE);
			}
			break;

		case 6:
			if(m_p_dialog_motor_axis_1 != NULL)
			{
				m_p_dialog_motor_axis_1->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_2 != NULL)
			{
				m_p_dialog_motor_axis_2->ShowWindow(SW_HIDE);
			}
			if(m_p_dialog_motor_axis_3 != NULL)
			{
				m_p_dialog_motor_axis_3->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_4 != NULL)
			{
				m_p_dialog_motor_axis_4->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_5 != NULL)
			{
				m_p_dialog_motor_axis_5->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_6 != NULL)
			{
				m_p_dialog_motor_axis_6->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_7 != NULL)
			{
				m_p_dialog_motor_axis_7->ShowWindow(SW_SHOW);
			}
			
			if(m_p_dialog_motor_axis_8 != NULL)
			{
				m_p_dialog_motor_axis_8->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_9 != NULL)
			{
				m_p_dialog_motor_axis_9->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_10 != NULL)
			{
				m_p_dialog_motor_axis_10->ShowWindow(SW_HIDE);
			}
			break;

		case 7:
			if(m_p_dialog_motor_axis_1 != NULL)
			{
				m_p_dialog_motor_axis_1->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_2 != NULL)
			{
				m_p_dialog_motor_axis_2->ShowWindow(SW_HIDE);
			}
			if(m_p_dialog_motor_axis_3 != NULL)
			{
				m_p_dialog_motor_axis_3->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_4 != NULL)
			{
				m_p_dialog_motor_axis_4->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_5 != NULL)
			{
				m_p_dialog_motor_axis_5->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_6 != NULL)
			{
				m_p_dialog_motor_axis_6->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_7 != NULL)
			{
				m_p_dialog_motor_axis_7->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_8 != NULL)
			{
				m_p_dialog_motor_axis_8->ShowWindow(SW_SHOW);
			}
			
			if(m_p_dialog_motor_axis_9 != NULL)
			{
				m_p_dialog_motor_axis_9->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_10 != NULL)
			{
				m_p_dialog_motor_axis_10->ShowWindow(SW_HIDE);
			}
			break;

		case 8:
			if(m_p_dialog_motor_axis_1 != NULL)
			{
				m_p_dialog_motor_axis_1->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_2 != NULL)
			{
				m_p_dialog_motor_axis_2->ShowWindow(SW_HIDE);
			}
			if(m_p_dialog_motor_axis_3 != NULL)
			{
				m_p_dialog_motor_axis_3->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_4 != NULL)
			{
				m_p_dialog_motor_axis_4->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_5 != NULL)
			{
				m_p_dialog_motor_axis_5->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_6 != NULL)
			{
				m_p_dialog_motor_axis_6->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_7 != NULL)
			{
				m_p_dialog_motor_axis_7->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_8 != NULL)
			{
				m_p_dialog_motor_axis_8->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_9 != NULL)
			{
				m_p_dialog_motor_axis_9->ShowWindow(SW_SHOW);
			}
			
			if(m_p_dialog_motor_axis_10 != NULL)
			{
				m_p_dialog_motor_axis_10->ShowWindow(SW_HIDE);
			}
			break;

		case 9:
			if(m_p_dialog_motor_axis_1 != NULL)
			{
				m_p_dialog_motor_axis_1->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_2 != NULL)
			{
				m_p_dialog_motor_axis_2->ShowWindow(SW_HIDE);
			}
			if(m_p_dialog_motor_axis_3 != NULL)
			{
				m_p_dialog_motor_axis_3->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_4 != NULL)
			{
				m_p_dialog_motor_axis_4->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_5 != NULL)
			{
				m_p_dialog_motor_axis_5->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_6 != NULL)
			{
				m_p_dialog_motor_axis_6->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_7 != NULL)
			{
				m_p_dialog_motor_axis_7->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_8 != NULL)
			{
				m_p_dialog_motor_axis_8->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_9 != NULL)
			{
				m_p_dialog_motor_axis_9->ShowWindow(SW_HIDE);
			}
			
			if(m_p_dialog_motor_axis_10 != NULL)
			{
				m_p_dialog_motor_axis_10->ShowWindow(SW_SHOW);
			}
			break;
	}
}

void CDialog_Motor_Axis_Tab::OnMotor_Axis_Delete()
{
	if(m_p_dialog_motor_axis_1 != NULL)
	{
		m_p_dialog_motor_axis_1->DestroyWindow();
		
		delete m_p_dialog_motor_axis_1;
		m_p_dialog_motor_axis_1 = NULL;
	}
	
	if(m_p_dialog_motor_axis_2 != NULL)
	{
		m_p_dialog_motor_axis_2->DestroyWindow();
		
		delete m_p_dialog_motor_axis_2;
		m_p_dialog_motor_axis_2 = NULL;
	}
	
	if(m_p_dialog_motor_axis_3 != NULL)
	{
		m_p_dialog_motor_axis_3->DestroyWindow();
		
		delete m_p_dialog_motor_axis_3;
		m_p_dialog_motor_axis_3 = NULL;
	}
	
	if(m_p_dialog_motor_axis_4 != NULL)
	{
		m_p_dialog_motor_axis_4->DestroyWindow();
		
		delete m_p_dialog_motor_axis_4;
		m_p_dialog_motor_axis_4 = NULL;
	}
	
	if(m_p_dialog_motor_axis_5 != NULL)
	{
		m_p_dialog_motor_axis_5->DestroyWindow();
		
		delete m_p_dialog_motor_axis_5;
		m_p_dialog_motor_axis_5 = NULL;
	}
	
	if(m_p_dialog_motor_axis_6 != NULL)
	{
		m_p_dialog_motor_axis_6->DestroyWindow();
		
		delete m_p_dialog_motor_axis_6;
		m_p_dialog_motor_axis_6 = NULL;
	}
	
	if(m_p_dialog_motor_axis_7 != NULL)
	{
		m_p_dialog_motor_axis_7->DestroyWindow();
		
		delete m_p_dialog_motor_axis_7;
		m_p_dialog_motor_axis_7 = NULL;
	}
	
	if(m_p_dialog_motor_axis_8 != NULL)
	{
		m_p_dialog_motor_axis_8->DestroyWindow();
		
		delete m_p_dialog_motor_axis_8;
		m_p_dialog_motor_axis_8 = NULL;
	}
	
	if(m_p_dialog_motor_axis_9 != NULL)
	{
		m_p_dialog_motor_axis_9->DestroyWindow();
		
		delete m_p_dialog_motor_axis_9;
		m_p_dialog_motor_axis_9 = NULL;
	}
	
	if(m_p_dialog_motor_axis_10 != NULL)
	{
		m_p_dialog_motor_axis_10->DestroyWindow();
		
		delete m_p_dialog_motor_axis_10;
		m_p_dialog_motor_axis_10 = NULL;
	}
}

void CDialog_Motor_Axis_Tab::OnMotor_Axis_Create(int n_mode)
{
	switch(n_mode)
	{
		case 0:
			if(m_p_dialog_motor_axis_1 == NULL)
			{
				m_p_dialog_motor_axis_1				= new CDialog_Motor_Axis_1;
				m_p_dialog_motor_axis_1->m_n_part	= m_n_part;
				m_p_dialog_motor_axis_1->Create(IDD_MOTOR_AXIS_1, this);
			}
			break;

		case 1:
			if(m_p_dialog_motor_axis_2 == NULL)
			{
				m_p_dialog_motor_axis_2				= new CDialog_Motor_Axis_2;
				m_p_dialog_motor_axis_2->m_n_part	= m_n_part;
				m_p_dialog_motor_axis_2->Create(IDD_MOTOR_AXIS_2, this);
			}
			break;

		case 2:
			if(m_p_dialog_motor_axis_3 == NULL)
			{
				m_p_dialog_motor_axis_3				= new CDialog_Motor_Axis_3;
				m_p_dialog_motor_axis_3->m_n_part	= m_n_part;
				m_p_dialog_motor_axis_3->Create(IDD_MOTOR_AXIS_3, this);
			}
			break;

		case 3:
			if(m_p_dialog_motor_axis_4 == NULL)
			{
				m_p_dialog_motor_axis_4				= new CDialog_Motor_Axis_4;
				m_p_dialog_motor_axis_4->m_n_part	= m_n_part;
				m_p_dialog_motor_axis_4->Create(IDD_MOTOR_AXIS_4, this);
			}
			break;

		case 4:
			if(m_p_dialog_motor_axis_5 == NULL)
			{
				m_p_dialog_motor_axis_5				= new CDialog_Motor_Axis_5;
				m_p_dialog_motor_axis_5->m_n_part	= m_n_part;
				m_p_dialog_motor_axis_5->Create(IDD_MOTOR_AXIS_5, this);
			}
			break;

		case 5:
			if(m_p_dialog_motor_axis_6 == NULL)
			{
				m_p_dialog_motor_axis_6				= new CDialog_Motor_Axis_6;
				m_p_dialog_motor_axis_6->m_n_part	= m_n_part;
				m_p_dialog_motor_axis_6->Create(IDD_MOTOR_AXIS_6, this);
			}
			break;

		case 6:
			if(m_p_dialog_motor_axis_7 == NULL)
			{
				m_p_dialog_motor_axis_7				= new CDialog_Motor_Axis_7;
				m_p_dialog_motor_axis_7->m_n_part	= m_n_part;
				m_p_dialog_motor_axis_7->Create(IDD_MOTOR_AXIS_7, this);
			}
			break;

		case 7:
			if(m_p_dialog_motor_axis_8 == NULL)
			{
				m_p_dialog_motor_axis_8				= new CDialog_Motor_Axis_8;
				m_p_dialog_motor_axis_8->m_n_part	= m_n_part;
				m_p_dialog_motor_axis_8->Create(IDD_MOTOR_AXIS_8, this);
			}
			break;

		case 8:
			if(m_p_dialog_motor_axis_9 == NULL)
			{
				m_p_dialog_motor_axis_9				= new CDialog_Motor_Axis_9;
				m_p_dialog_motor_axis_9->m_n_part	= m_n_part;
				m_p_dialog_motor_axis_9->Create(IDD_MOTOR_AXIS_9, this);
			}
			break;

		case 9:
			if(m_p_dialog_motor_axis_10 == NULL)
			{
				m_p_dialog_motor_axis_10			= new CDialog_Motor_Axis_10;
				m_p_dialog_motor_axis_10->m_n_part	= m_n_part;
				m_p_dialog_motor_axis_10->Create(IDD_MOTOR_AXIS_10, this);
			}
			break;
	}
}
