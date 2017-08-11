#if !defined(AFX_DIALOG_WORK_INFO_TAB_H__7A866DA6_F581_481A_8315_21643BE22DB7__INCLUDED_)
#define AFX_DIALOG_WORK_INFO_TAB_H__7A866DA6_F581_481A_8315_21643BE22DB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Work_Info_Tab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Info_Tab window
class CDialog_Work_Maintenane;
class CDialog_Work_Interface;
class CDialog_Work_Wait_Time;
class CDialog_Work_Barcode_Print;	//20120521
class CDialog_Work_Skip_Mode;	//20120525


// class CDialog_Work_Motor_Speed;
// class CDialog_Work_Maz_Interface;

class CDialog_Work_Info_Tab : public CTabCtrl
{
public:
	CDialog_Work_Maintenane	*m_p_maintenance;
	CDialog_Work_Interface		*m_p_interface;
 	CDialog_Work_Wait_Time		*m_p_wait_time;
	CDialog_Work_Barcode_Print	*m_p_barcode_print;	//20120521
	CDialog_Work_Skip_Mode		*m_p_skip_mode;	//20120525


// 	CDialog_Work_Motor_Speed	*m_p_motor_speed;
// 	CDialog_Work_Maz_Interface	*m_p_maz_interface;
	CRect m_rect_area;
// Construction
public:
	CDialog_Work_Info_Tab();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Work_Info_Tab)
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnWork_Change(int n_mode);
	void Init_Tab(CWnd *p_wnd, CRect rect_area);
	virtual ~CDialog_Work_Info_Tab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDialog_Work_Info_Tab)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORK_INFO_TAB_H__7A866DA6_F581_481A_8315_21643BE22DB7__INCLUDED_)
