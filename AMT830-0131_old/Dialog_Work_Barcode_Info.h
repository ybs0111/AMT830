#if !defined(AFX_DIALOG_WORK_BARCODE_INFO_H__EF948897_1677_44F2_B7AB_45005D0C7159__INCLUDED_)
#define AFX_DIALOG_WORK_BARCODE_INFO_H__EF948897_1677_44F2_B7AB_45005D0C7159__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Work_Barcode_Info.h : header file
//
#include "Variable.h"			// ���� ���� ���� Ŭ���� �߰�
#include "Public_Function.h"
#include "EXDigitST.h"			// ������ ī���� Ŭ����
#include "XPGroupBox.h"			// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "sxlogfont.h"
#include "GradientStatic.h"		// �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "XpButton.h"			// XP Į�� ��ư Ŭ����
#include "FloatST.h"
#include "GridControlAlg.h"
#include "Digit.h"
#include "Dialog_Keyboard.h"
#include "MacButtons.h"
#include "EditEx.h"				// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Barcode_Info dialog

class CDialog_Work_Barcode_Info : public StandardDialog
{
// Construction
public:
	CFont			* m_p_font;
	
public:
	void	Init_Button();

	CDialog_Work_Barcode_Info(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Work_Barcode_Info)
	enum { IDD = IDD_WORK_BARCODE_INFO };
	CButtonST	m_btn_apply;
	CStatic	m_static_picture_info;
	
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Work_Barcode_Info)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Work_Barcode_Info)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORK_BARCODE_INFO_H__EF948897_1677_44F2_B7AB_45005D0C7159__INCLUDED_)
