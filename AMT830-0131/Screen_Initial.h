#if !defined(AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_)
#define AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Initial.h : header file
//
#include "Variable.h"				// ���� ���� ���� Ŭ���� �߰� 
#include "PictureEx.h"				// GIF ���� �ε� Ŭ���� �߰� 
#include "GradientStatic.h"			// �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "XPButton.h"				// XP ���� Į�� ��ư ���� Ŭ���� �߰� 
#include "LedButton.h"				// LED ��ư ���� Ŭ���� �߰� 
#include "Public_Function.h"

#include "MacButtons.h"
#include "SuperProgressCtrl.h"		// ProgressCtrl ���� Ŭ���� �߰�

#include "Thread.h"					// ������ ���� Ŭ���� �߰� 
// ******************************************************************************
// ������ ���� ���� Ŭ���� �߰�                                                  
// ******************************************************************************
#include "Run_Handler_Check.h"
#include "Run_TowerLamp.h"
// ******************************************************************************

// ******************************************************************************
// ���� ���� �ε� �� ���� Ŭ����                                                 
// ******************************************************************************
#include "MyBasicData.h"

// ******************************************************************************
// �ʱ�ȭ ȭ�� ���� Ÿ�̸� ID ����                                               
// ******************************************************************************
#define TMR_INIT	710  // �ʱ�ȭ �۾� ���� Ÿ�̸� ID ���� 
// ******************************************************************************
/////////////////////////////////////////////////////////////////////////////
// CScreen_Initial form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Initial : public CFormView
{
public:
	CScreen_Initial();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Initial)

// Form Data
public:
	//{{AFX_DATA(CScreen_Initial)
	enum { IDD = IDD_SCREEN_INITIAL };
	CLedButton	m_radio_buffer;
	CLedButton	m_radio_rbt_vision;
	CLedButton	m_radio_ld_stacker;
	CLedButton	m_radio_rbt_rear_hs;
	CLedButton	m_radio_rbt_sorter;
	CLedButton	m_radio_rbt_ldmodule;
	CLedButton	m_radio_rbt_clip;
	CLedButton	m_radio_rbt_front_hs;
	CLedButton	m_radio_unload_stacker;
	CLedButton	m_radio_hs_stacker;
	CLedButton	m_radio_rear_hs_stacker;
	CLedButton	m_radio_clip_stacker;
	CLedButton	m_radio_mot_bd;
	CLedButton	m_radio_io_bd;
	CPictureEx	m_gif_initializing;
	CMacButton	m_btn_init_skip;
	CMacButton	m_btn_init_retry;
	CGradientStatic	m_msg_initial;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_init_font;  // ��Ʈ ���� ���� ���� 
	
	// **************************************************************************
	// ȯ�� ���� ������ �ε� �� ���� Ŭ���� ���� ����                            
	// **************************************************************************
	CMyBasicData     mcls_m_basic;		// �⺻ ���� ���� �ε� �� ���� Ŭ����
	// **************************************************************************
	
	UINT m_nDataSendCnt;
	
	int mn_motor_init_count;
	int mn_pos_step;				// ���α׷��� ��ġ ���� ���� ����
	
	// **************************************************************************
	// �ʱ�ȭ ���� : Step ������ ���� ���� �� ���� �÷��� ����                   
	// **************************************************************************
	long ml_init_step ;
	int mn_init_flag;
	// **************************************************************************

// Operations
public:
	int OnInitial_Init_Excution();
	void OnInitial_Initial_Ready();
	void OnInitial_Step_Clear();
	void OnInitial_Change_Status(int n_status);
	void OnInitial_Controls_Enable(int n_init);
	void OnInitial_RadioButton_Set();
	void OnInitial_Progress_Set();
	void OnInitial_Picture_Set();
	void OnInitial_Label_Set();
	
private:
	double NetworkingTime[3];
	char cJamcode[10];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Initial)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSuperProgressCtrl m_ctrlProgress;
	CSuperProgressCtrl m_ctrlProgress1;

	virtual ~CScreen_Initial();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Initial)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnInitSkip();
	afx_msg void OnBtnInitRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_INITIAL_H__C8338AC3_B7DD_4866_93A2_DF84D57A5B3E__INCLUDED_)
