#if !defined(AFX_DIALOG_LOT_OPEN_H__069507AC_514B_475A_AB23_0029EF1F2E29__INCLUDED_)
#define AFX_DIALOG_LOT_OPEN_H__069507AC_514B_475A_AB23_0029EF1F2E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Lot_Open.h : header file
//
#include "Variable.h"			// 전역 변수 정의 클래스 추가
#include "Public_Function.h"
#include "EXDigitST.h"			// 디지털 카운터 클래스
#include "XPGroupBox.h"			// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"			// XP 칼라 버튼 클래스
#include "FloatST.h"
#include "GridControlAlg.h"
#include "Digit.h"
#include "Dialog_Keyboard.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_Lot_Open dialog
#define TMR_LOT_DISPLAY		991
#define TMR_LOT_END			992
#define TMR_LOT_CANCEL		993
class CDialog_Lot_Open : public CDialog
{
// Construction
public:
	CFont			* m_p_font;
	int				mn_work_mode;

public:
	void	Init_Label();
	void	Init_Button();
	void	Init_Data();
	void	Init_GroupBox_Set();
	int		n_lotopen_step;

	CString mstr_lot_no;

	CDialog_Lot_Open(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Lot_Open)
	enum { IDD = IDD_LOT_OPEN };
	CGradientStatic	m_msg;
	CGradientStatic	m_label_part_id;
	CGradientStatic	m_label_oper_info;
	CGradientStatic	m_label_lot_id;
	CGradientStatic	m_label_lot_qty;
	CGradientStatic	m_label_eqp_id;
	CGradientStatic	m_msg_part_id;
	CGradientStatic	m_msg_oper_id;
	CGradientStatic	m_msg_lot_qty;
	CGradientStatic	m_msg_lot_id;
	CGradientStatic	m_msg_eqp_id;
	CXPGroupBox	m_group_lot_start;
	CXPButton	m_btn_lot_display;
	CXPButton	m_btn_lot_start;
	CXPButton	m_btn_lot_cancel;
	CButtonST	m_btn_Lot_open_ok;
	CGradientStatic m_msg_lot_open;
	CEditEx	 m_edit_Lot_open;

	CXPButton   m_btn_lot_delete;
	//}}AFX_DATA

public:
	int OnLotStart_Excution();
	void Server_Data_Display();
	int OnLotEnd_Excution();
	int OnLotCancel_Excution();

	long NetworkConnectTime[3];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Lot_Open)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Lot_Open)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnOk();
	afx_msg void OnBtnLotStart();
	afx_msg void OnBtnLotCancel();
	afx_msg void OnBtnLotDisplay();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnLotEnd();
	afx_msg void OnBtnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_LOT_OPEN_H__069507AC_514B_475A_AB23_0029EF1F2E29__INCLUDED_)
