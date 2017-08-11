#if !defined(AFX_DIALOG_ADD_FAIL_H__5E706ACB_F45A_45F5_A8A0_78BF3245B5E8__INCLUDED_)
#define AFX_DIALOG_ADD_FAIL_H__5E706ACB_F45A_45F5_A8A0_78BF3245B5E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Add_Fail.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 

#define TMR_ADD_FAIL	2000
/////////////////////////////////////////////////////////////////////////////
// CDialog_Add_Fail dialog

class CDialog_Add_Fail : public StandardDialog
{
// Construction
public:
	CDialog_Add_Fail(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Add_Fail)
	enum { IDD = IDD_DIALOG_ADD_FAIL };
	CGradientStatic	m_label_serial;
	CGradientStatic	m_label_as_sc;
	CGradientStatic	m_label_a4202_sc;
	CGradientStatic	m_label_a4201_sc;
	CGradientStatic	m_label_a4200_sc;
	CXPGroupBox	m_group_add_scrap_to_server;
	CXPGroupBox	m_group_add_4202_scrap_to_server;
	CXPGroupBox	m_group_add_4201_scrap_to_server;
	CXPGroupBox	m_group_add_4200_scrap_to_server;
	CMacButton	m_btn_add_fail;
	CMacButton	m_btn_add_4202;
	CMacButton	m_btn_add_4201;
	CMacButton	m_btn_add_4200;
	//}}AFX_DATA

// Attributes
public:
	CButton *mp_add_fail_ok;	// 버튼에 대한 포인터 선언
	CFont* mp_add_fail_font;			// 폰트 정보 설정 변수
	
	int mn_edit;
	int mn_timer_act;
	int mn_ec_server_step;

	long ml_wait_time[3];
	long ml_retry_wait[3];

	CEditEx	 m_edit_serial;
	CEditEx	 m_edit_scrap_code;
	CEditEx	 m_edit_4200_scrap_count;
	CEditEx	 m_edit_4201_scrap_count;
	CEditEx	 m_edit_4202_scrap_count;
	
// Operations
public:
	void OnAddFail_Label_Set();
	void OnAddFail_EditBox_Set();
	int OnAddFail_Server_Send();
	void OnAddFail_Button_Set();
	void OnAddFail_GroupBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Add_Fail)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Add_Fail)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnAddFail();
	afx_msg void OnBtnAdd4200();
	afx_msg void OnBtnAdd4201();
	afx_msg void OnBtnAdd4202();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_ADD_FAIL_H__5E706ACB_F45A_45F5_A8A0_78BF3245B5E8__INCLUDED_)
