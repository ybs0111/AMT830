#if !defined(AFX_DIALOG_WORKER_CREATE_H__66E0E533_B49A_42E2_AC90_AC71F0BE5FF7__INCLUDED_)
#define AFX_DIALOG_WORKER_CREATE_H__66E0E533_B49A_42E2_AC90_AC71F0BE5FF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Worker_Create.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Worker_Create dialog
#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰�  
#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "EditEx.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "EXDigitST.h"		// ������ ī���� Ŭ����
#include "LedButton.h"      // LED ��ư Ŭ����
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "SxLogFont.h"
#include "MacButtons.h"
//#include "xShadeButton.h"	// ���� ��ư ���� Ŭ���� �߰�
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "XpButton.h"		// XP Į�� ��ư Ŭ����
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"		// Fastech ���� ���� Class
#include "ComizoaPublic.h"
#include "GridControlAlg.h"
#include "Digit.h"
class CDialog_Worker_Create : public CDialog
{
public:
	TSpread			*m_grid_worker;
	GridControlAlg	m_p_grid;

	CFont			*m_p_font;

	CString			m_str_name;
	CString			m_str_crem;
	CString			m_str_number;
// Construction
public:
	void Init_Grid_Worker();
	void Init_Group();
	void Init_Label();
	CDialog_Worker_Create(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Worker_Create)
	enum { IDD = IDD_DIALOG_WORKER_CREATE };
	CXPGroupBox	m_group_worker;
	CMacButton	m_btn_cancel;
	CMacButton	m_btn_apply;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Worker_Create)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Worker_Create)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnApply();
	afx_msg void OnBtnCancel();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORKER_CREATE_H__66E0E533_B49A_42E2_AC90_AC71F0BE5FF7__INCLUDED_)
