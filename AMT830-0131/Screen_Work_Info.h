#if !defined(AFX_SCREEN_WORK_INFO_H__534415EB_851A_4C8B_8F36_65A2DB69E8FD__INCLUDED_)
#define AFX_SCREEN_WORK_INFO_H__534415EB_851A_4C8B_8F36_65A2DB69E8FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Work_Info.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen_Work_Info form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "editex.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 

#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰� 

#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "sxlogfont.h"

#include "LedButton.h"      // LED ��ư Ŭ����

#include "EXDigitST.h"		// ������ ī���� ���� Ŭ���� �߰� 
#include "MacButtons.h"
#include "Digit.h"
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "PictureEx.h"		// GIF ������ �ε��ϱ� ���� Ŭ���� �߰� 
#include "BmpImageST.h"		// �˶� ��Ʈ�� �ε� Ŭ���� �߰� 
#include "CheckerCtrl.h"
#include "CharSetCtrl.h"
#include "FloatST.h"
#include "GridControlAlg.h"
#include "Dialog_Work_Info_Tab.h"


class CScreen_Work_Info : public CFormView
{
public:
	CFont		* m_p_font;
public:
	CScreen_Work_Info();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Work_Info)


// Form Data
public:
	//{{AFX_DATA(CScreen_Work_Info)
	enum { IDD = IDD_SCREEN_WORK_INFO };
	CButtonST	m_chk_write_rfid;
	CButtonST	m_chk_maintenance;
	CButtonST	m_chk_interface;
	CButtonST   m_chk_wait_time;
	///////////////////////////////////// 20120521
	CButtonST	m_chk_barcode_print;
	///////////////////////////////////// 20120525
	CButtonST	m_chk_skip_mode;
	/////////////////////////////////////

	CDialog_Work_Info_Tab	m_tab_work_info;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void Init_Button();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Work_Info)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Work_Info();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Work_Info)
	afx_msg void OnChkMaintenance();
	afx_msg void OnChkInterface();
	afx_msg void OnChkWaitTime();
	afx_msg void OnChkBarcodePrint();
	afx_msg void OnChkSkipMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_WORK_INFO_H__534415EB_851A_4C8B_8F36_65A2DB69E8FD__INCLUDED_)
