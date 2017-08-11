#if !defined(AFX_DIALOG_LIST_H__5B34FDA5_586B_4820_8AF1_ED8F0B390110__INCLUDED_)
#define AFX_DIALOG_LIST_H__5B34FDA5_586B_4820_8AF1_ED8F0B390110__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_List.h : header file
//
#include "InitDialogBar.h" // Initial Dialog Bar ���� Ŭ���� �߰� 
/////////////////////////////////////////////////////////////////////////////
// CDialog_List dialog

#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include "ColorListBox.h"	// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "InitDialogBar.h"	// Initial Dialog Bar ���� Ŭ���� �߰� 
#include "ClockST.h"		// ������ �ð� ���� Ŭ���� �߰� 
#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "Variable.h"			// ���� ���� ���� Ŭ���� �߰� 
#include <afxmt.h>

class CDialog_List : public CInitDialogBar
{
// Construction
public:
	long ml_list_msgterm_time_normal[4]; 
	long ml_list_msgterm_time_abnormal[4]; //2011.0504  
	long ml_listmsg_write_time;
	
	CString mstr_old_msg;

public:
	CDialog_List();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_List)
	enum { IDD = IDD_DIALOG_LIST };
	CEdit	m_edit_list_msg;
	CGradientStatic	m_data_machine;
	CGradientStatic	m_data_device_name;
	CNewLabel	m_msg_machine;
	CNewLabel	m_msg_device_name;
	CColorListBox	m_list_data;
	CClockST	m_list_clock;
	//}}AFX_DATA

// Attributes
private:
	CFont* mp_list_font;		// ��Ʈ ���� ���� ���� 

	CString mstr_machine_info;		// ��� ���� ���� ���� 
	CString mstr_device_name;	// ���� �۾� ������ Device ���� ���� ���� 
	CString mstr_normal_old_msg;
	CString mstr_abnormal_old_msg;

	int n_len;
	int n_len_h;
	int n_len_d;

	int nReturnStep;
	int nReturnSvr;
	
// Operation
public:
	CCriticalSection cs;
	void OnList_Device_Info_Set();
	void OnList_Machine_Info_Set();
	void OnList_Receive_Msg_Display(int n_mode);
	void OnList_Send_Msg_Display(int n_mode);
	void OnList_Abnormal_Msg_Display();
	void OnList_Normal_Msg_Display();
	void OnList_Message_Clear();
	void OnList_Lable_Set();
	void OnList_Check_Tab_Stops(int mb_use_tab_stops);
	void OnList_Digital_Clock_Set();
	void OnPick_Next_Available();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_List)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_List)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialogBar();
	//}}AFX_MSG
	afx_msg LRESULT OnListDataUpdate(WPARAM wParam,LPARAM lParam) ;  // ���� �޽��� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰� �޽��� ����
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_LIST_H__5B34FDA5_586B_4820_8AF1_ED8F0B390110__INCLUDED_)
