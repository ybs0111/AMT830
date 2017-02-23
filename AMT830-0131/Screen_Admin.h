#if !defined(AFX_SCREEN_ADMIN_H__EB1175A4_638C_47E2_B09B_2AB54A977A29__INCLUDED_)
#define AFX_SCREEN_ADMIN_H__EB1175A4_638C_47E2_B09B_2AB54A977A29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Admin.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "ListBoxST.h"      // 칼라 아이콘 리스트 박스 클래스
#include "xpbutton.h"		// XP 칼라 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "FloatST.h"
#include "LedButton.h"				// LED 버튼 생성 클래스 추가 

#include "Digit.h"

// ******************************************************************************
// 설정 정보 로딩 및 저장 클래스
// ******************************************************************************
#include "MyBasicData.h"
// ******************************************************************************

#define TMR_BUFFERREADY		600
#define TMR_BUFFERIN		601
#define TMR_SOCKETCHK		602
#define TMR_APCTSOCKETINFO	603
#define TMR_APCTBLOT		604
/////////////////////////////////////////////////////////////////////////////
// CScreen_Admin form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Admin : public CFormView
{
public:
	CScreen_Admin();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Admin)

// Form Data
public:
	//{{AFX_DATA(CScreen_Admin)
	enum { IDD = IDD_SCREEN_ADMIN };
	CButton	m_chk_Virtual;
	CButton	m_chk_nto_fail;
	CButton	m_chk_hto_fail;
	CButton	m_chk_bto_fail;
	CButton	m_chk_uld_convayor;
	CMacButton	m_btn_bufferdb_delete;
	CButton	m_chk_reverse;
	CMacButton	m_btn_acpt_blot_test;
	CButton	m_chk_barcode;
	CButton	m_chk_debug;
	CMacButton	m_btn_socket_chk_test;
	CMacButton	m_btn_buffer_in_test;
	CMacButton	m_btn_apct_socket_info_test;
	CMacButton	m_btn_buffer_ready_test;
	CGradientStatic	m_msg_server_port_fmachine;
	CMacButton	m_btn_server_port_save_fmachine;
	CGradientStatic	m_msg_server_port_apct;
	CMacButton	m_btn_server_port_save_apct;
	CGradientStatic	m_msg_port_apct;
	CGradientStatic	m_msg_ipaddress_apct;
	CIPAddressCtrl	m_ipaddress_apct;
	CXPGroupBox	m_group_next_apct_set;
	CMacButton	m_btn_test_apct;
	CMacButton	m_btn_port_save_apct;
	CMacButton	m_btn_ip_save_apct;
	CMacButton	m_btn_connect_apct;
	//}}AFX_DATA

// Attributes
public:
	CEXDigitST		m_dgt_eject_count;
	CEXDigitST		m_dgt_pick_count;

	CFont* mp_admin_font;			// 폰트 정보 설정 변수
	
	CMyBasicData    mcls_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	
	// **************************************************************************
	// 칼라 에디트 박스 클래스 변수 선언
	// **************************************************************************
	CEditEx	 m_edit_port_apct;
	CEditEx	 m_edit_server_port_apct;
	CEditEx	 m_edit_server_port_fmachine;
	// **************************************************************************
	
	// **************************************************************************
	// 데이터 저장 클래스 변수 선언
	// **************************************************************************
	BYTE nIPFiled0[3], nIPFiled1[3], nIPFiled2[3], nIPFiled3[3];		// 0: BPC, 1: NEXT MACHINE, 2:APCT
	CString mstr_epc_server_port, mstr_next_machine_server_port, mstr_next_apct_server_port;

	int nAPCTType;

	int mn_buffer_ready_step;
	int mn_buffer_in_step;
	int mn_apct_socket_info_step;
	int mn_apct_blot_step;

	long mlWait_Time[3];
	int mnError_cnt[3];

// Operations
public:
	int APCT_BLot();
	int APCT_Socket_Info();
	int Buffer_In();
	int Buffer_Ready();
	void OnIP_Address(CString str_ip, int n_section);
	void OnAdmin_Data_Set();
	void OnAdmin_Label_Set();
	void OnAdmin_EditBox_Set();
	void OnAdmin_GroupBox_Set();
	void OnAdmin_Dgt_Set();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Admin)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Admin();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Admin)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnIpSaveApct();
	afx_msg void OnBtnPortSaveApct();
	afx_msg void OnBtnServerPortSaveApct();
	afx_msg void OnBtnServerPortSaveNmachine();
	afx_msg void OnBtnServerPortSaveFmachine();
	afx_msg void OnBtnBufferReadyTest();
	afx_msg void OnBtnBufferInTest();
	afx_msg void OnBtnSocketChkTest();
	afx_msg void OnBtnApctSocketInfoTest();
	afx_msg void OnChkDebug();
	afx_msg void OnChkBarcode();
	afx_msg void OnBtnApctBlotTest();
	afx_msg void OnDgtEjectCount();
	afx_msg void OnDgtPickCount();
	afx_msg void OnChkReverse();
	afx_msg void OnBtnBufferdbDelete();
	afx_msg void OnChkUldConvayor();
	afx_msg void OnChkNtoFail();
	afx_msg void OnChkHtoFail();
	afx_msg void OnChkBtoFail();
	afx_msg void OnChkVirtual();
	afx_msg void OnBtnFileEditor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_ADMIN_H__EB1175A4_638C_47E2_B09B_2AB54A977A29__INCLUDED_)
