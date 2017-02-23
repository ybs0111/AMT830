#if !defined(AFX_DIALOG_SCRAP_INFO_VIEW_H__4DCB3D55_06B3_46D0_BDDB_7EDF54CE85A5__INCLUDED_)
#define AFX_DIALOG_SCRAP_INFO_VIEW_H__4DCB3D55_06B3_46D0_BDDB_7EDF54CE85A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Scrap_Info_View.h : header file
//
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "editex.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "sxlogfont.h"
#include "MacButtons.h"

#define TMR_SCRAP_INFO			2000
#define TMR_YESBTN_SCRAP_INFO   3000 // 20130121

/////////////////////////////////////////////////////////////////////////////
// CDialog_Scrap_Info_View dialog

class CDialog_Scrap_Info_View : public StandardDialog
{
// Construction
public:
	CDialog_Scrap_Info_View(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Scrap_Info_View)
	enum { IDD = IDD_DIALOG_SCRAP_INFO_VIEW };
	CMacButton	m_btn_scrap_info;
	CMacButton	m_btn_yes;
	CMacButton	m_btn_no;
	CXPGroupBox	m_group_sorter_scrap_list;
	CXPGroupBox	m_group_sorter_scrap_info;
	CXPGroupBox	m_group_question;
	//}}AFX_DATA

// Attributes
public:
	CButton *mp_scrap_ok;	// ��ư�� ���� ������ ����
	CFont* mp_scrap_font;			// ��Ʈ ���� ���� ����
	
	GridControlAlg m_pGrid;

	int mn_edit;
	int mn_timer_act;
	int mn_ec_server_step;

	long ml_wait_time[3];
	long ml_retry_wait[3];

	int mn_data_send_to_host;
	
	int n_scrapinfo_step; // 20130121
	long NetworkConnectTime[3]; // 20130121

// Operations
public:
	void OnScrap_Display_List();
	int OnScrapinfo_Server_Send();
	int OnScrapInfo_YesBtn_Server_Send(); // 20130121
	void OnChangeValue(int nid, int nrow, int ncol);
	void OnScrapInfo_GroupBox_Set();
	void OnScrap_Display_Info();
	void OnScrapInfo_Button_Set();
	void OnScrapInfo_Init_Info();
	void OnScrapInfo_Init_List();
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Scrap_Info_View)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Scrap_Info_View)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnYes();
	afx_msg void OnBtnNo();
	afx_msg void OnBtnScrapInfo();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScrap_Info_Display(WPARAM wParam,LPARAM lParam);  // �׽�Ʈ ��� ���� ȭ�鿡 ����ϱ� ���� ����� ���� �޽��� �߰� 
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_SCRAP_INFO_VIEW_H__4DCB3D55_06B3_46D0_BDDB_7EDF54CE85A5__INCLUDED_)
