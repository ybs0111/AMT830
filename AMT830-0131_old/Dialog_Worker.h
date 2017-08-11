#if !defined(AFX_DIALOG_WORKER_H__222DF8B9_054F_4A12_B7EC_45EF541A29CC__INCLUDED_)
#define AFX_DIALOG_WORKER_H__222DF8B9_054F_4A12_B7EC_45EF541A29CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Worker.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Worker dialog
#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰�  
#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "EditEx.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "EXDigitST.h"		// ������ ī���� Ŭ����
#include "LedButton.h"      // LED ��ư Ŭ����
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "SxLogFont.h"
#include "MacButtons.h"
// #include "xShadeButton.h"	// ���� ��ư ���� Ŭ���� �߰�
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "XpButton.h"		// XP Į�� ��ư Ŭ����
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"		// Fastech ���� ���� Class
#include "ComizoaPublic.h"
#include "GridControlAlg.h"
#include "Digit.h"
class CDialog_Worker : public CDialog
{
public:
	COleDateTime		m_oledate[2];

	CString				m_str_name;
	CString				m_str_crem;
	CString				m_str_no;

	int					m_n_pos;
	TSpread				*m_grid_worker;
	GridControlAlg		m_p_grid;
// Construction
public:
	void Display_Worker(int n_count);
	void Init_Grid_Worker();
	CDialog_Worker(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Worker)
	enum { IDD = IDD_DIALOG_WORKER };
	CMacButton	m_btn_worker_delete;
	CMacButton	m_btn_worker_select;
	CMacButton	m_btn_worker_create;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Worker)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Worker)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnWorkerCreate();
	afx_msg void OnBtnWorkerSelect();
	afx_msg void OnBtnWorkerDelete();
	afx_msg void OnBtnExit();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORKER_H__222DF8B9_054F_4A12_B7EC_45EF541A29CC__INCLUDED_)
