#if !defined(AFX_DIALOG_WORK_SKIP_MODE_H__8C63A2CE_98CC_4C22_A5C2_72E9D8A99B63__INCLUDED_)
#define AFX_DIALOG_WORK_SKIP_MODE_H__8C63A2CE_98CC_4C22_A5C2_72E9D8A99B63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Work_Skip_Mode.h : header file
//
#include "Variable.h"			// ���� ���� ���� Ŭ���� �߰� 
#include "Public_Function.h"
#include "GradientStatic.h"		// �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "MacButtons.h"
#include "EXDigitST.h"			// ������ ī���� ���� Ŭ���� �߰� 
#include "FloatST.h"
#include "XpButton.h"			// XP Į�� ��ư Ŭ����
#include "MyBasicData.h"
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰�  
#include "UniButtonEx.h"	// ���� ��ư Ŭ����
#include "editex.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "SxLogFont.h"

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Skip_Mode dialog

class CDialog_Work_Skip_Mode : public CDialog
{
// Construction
public:
	int OnSkip_Data_Comparison();
	void OnSkip_Data_Apply();			// ȭ�� ���� ���� ���� ������ ���� �Լ�
	void OnSkip_LogFile_Create();
	void OnSkip_Data_BackUp();
	void OnSkip_Data_Display();
	void OnSkip_Data_Set();				// ���� ������ Data�� �޾ƿ´�.
	void OnSkip_Data_Recovery();
	void OnSkip_GroupBox_Set();
	void OnDestroy();
	void OnSkip_Init_Button();
	CButtonST m_chk_skip_work_buffer_use[4];

	CDialog_Work_Skip_Mode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Work_Skip_Mode)
	enum { IDD = IDD_WORK_SKIP_MODE };
	CXPGroupBox m_group_skip_module_loader;
	CXPGroupBox m_group_skip_heatsink_left;
	CXPGroupBox m_group_skip_heatsink_right;
	CXPGroupBox m_group_skip_clip;
	CXPGroupBox m_group_skip_picker_robot;
	CXPGroupBox m_group_skip_label;
	CXPGroupBox m_group_skip_vision;
	CXPGroupBox m_group_skip_work_buffer;

	CButtonST m_chk_skip_module_loader_enable1;
	CButtonST m_chk_skip_heatsink_left_enable1;
	CButtonST m_chk_skip_heatsink_right_enable1;
	CButtonST m_chk_skip_module_loader_enable2;
	CButtonST m_chk_skip_heatsink_left_enable2;
	CButtonST m_chk_skip_heatsink_right_enable2;
	CButtonST m_chk_skip_clip_enable;
	CButtonST m_chk_skip_picker_enable;
	CButtonST m_chk_skip_label_enable;
	CButtonST m_chk_skip_vision_enable;
	CButtonST m_chk_skip_vision_enable2;//20120912

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
	CFont* mp_skip_font;			// ��Ʈ ���� ���� ����
	CMyBasicData    mcls_skip;		// �⺻ ���� ���� �ε� �� ���� Ŭ����
	
	int mn_skip_m_loader_enable[2][2];				// With, With Out, No Tray
	int mn_skip_hs_left_enable[2][2];				// With, With Out, No Tray
	int mn_skip_hs_right_enable[2][2];				// With, With Out, No Tray

	int mn_skip_clip_enable[2];
	int mn_skip_picker_ldrbt_enable[2];
	int mn_skip_label_enable[2];				// With, With Out, No Tray
	int mn_skip_vision_enable[2];				// With, With Out, No Tray
	int mn_skip_vision_enable2[2];				// With, With Out, No Tray 20120912

	int mn_skip_work_buffer_enable[4][2];
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Work_Skip_Mode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Work_Skip_Mode)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSkipApply();
	afx_msg void OnChkSkipWorkBuffer4Use();
	afx_msg void OnChkSkipModuleLoaderEnable1();
	afx_msg void OnChkSkipModuleLoaderEnable2();
	afx_msg void OnChkSkipHeatsinkLeftEnable1();
	afx_msg void OnChkSkipHeatsinkLeftEnable2();
	afx_msg void OnChkSkipHeatsinkRightEnable1();
	afx_msg void OnChkSkipHeatsinkRightEnable2();
	afx_msg void OnChkSkipLabelEnable();
	afx_msg void OnChkSkipPickerEnable();
	afx_msg void OnChkSkipVisionEnable();
	afx_msg void OnChkSkipClipEnable();
	afx_msg void OnChkSkipWorkBufferEnable1();
	afx_msg void OnChkSkipWorkBufferEnable2();
	afx_msg void OnChkSkipWorkBufferEnable3();
	afx_msg void OnChkSkipWorkBufferEnable4();
	afx_msg void OnChkSkipVisionEnable2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORK_SKIP_MODE_H__8C63A2CE_98CC_4C22_A5C2_72E9D8A99B63__INCLUDED_)
