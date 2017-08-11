#if !defined(AFX_DIALOG_WORK_BUFFER_H__504B9C00_74A4_4ABE_9A2F_68BF668C189A__INCLUDED_)
#define AFX_DIALOG_WORK_BUFFER_H__504B9C00_74A4_4ABE_9A2F_68BF668C189A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Work_buffer.h : header file
//

#include "BtnST.h"
/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_buffer dialog

class CDialog_Work_buffer : public CDialog
{
// Construction
public:
	CDialog_Work_buffer(CWnd* pParent = NULL);   // standard constructor
	CFont* mp_font;			// 폰트 정보 설정 변수

	int nWorkBuffer[4][2];
	int nFrontHs[4][2];
	int nRearHs[4][2];

	
// Dialog Data
	//{{AFX_DATA(CDialog_Work_buffer)
	enum { IDD = IDD_DIALOG_WORK_BUFFER };
	CButtonST	m_chk_module4;
	CButtonST	m_chk_module3;
	CButtonST	m_chk_module2;
	CButtonST	m_chk_module1;
	CButtonST	m_chk_front_hs4;
	CButtonST	m_chk_front_hs3;
	CButtonST	m_chk_front_hs2;
	CButtonST	m_chk_front_hs1;
	CButtonST	m_chk_back_hs4;
	CButtonST	m_chk_back_hs3;
	CButtonST	m_chk_back_hs2;
	CButtonST	m_chk_back_hs1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Work_buffer)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init_Data();
	void Display_Moduleata();
	void OnWorkBuffer_LogFile_Create();
	void OnWorkBuffer_Data_Apply();
	int OnWorkBuffer_Data_Comparison();
	void OnWorkBuffer_Data_Recovery();
	void OnWorkBuffer_Data_BackUp();
	void OnWorkBuffer_Button_Set();
	void OnWorkBuffer_Change();

protected:
	// Generated message map functions
	//{{AFX_MSG(CDialog_Work_buffer)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnChkBackHs();
	afx_msg void OnChkBackHs2();
	afx_msg void OnChkBackHs3();
	afx_msg void OnChkBackHs4();
	afx_msg void OnChkModule();
	afx_msg void OnChkModule2();
	afx_msg void OnChkModule3();
	afx_msg void OnChkModule4();
	afx_msg void OnChkFrontHs();
	afx_msg void OnChkFrontHs2();
	afx_msg void OnChkFrontHs3();
	afx_msg void OnChkFrontHs4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORK_BUFFER_H__504B9C00_74A4_4ABE_9A2F_68BF668C189A__INCLUDED_)
