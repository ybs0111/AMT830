#if !defined(AFX_DIALOG_TRACK_OUT_H__F2AE627D_0BFD_40C7_A33D_9144CF33278C__INCLUDED_)
#define AFX_DIALOG_TRACK_OUT_H__F2AE627D_0BFD_40C7_A33D_9144CF33278C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Track_Out.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "XPButton.h"

#define TMR_TRACK_OUT			2001
#define TMR_TSCRAP_INFO			2002
#define TMR_TDEL_SCRAP			2003
/////////////////////////////////////////////////////////////////////////////
// CDialog_Track_Out dialog

class CDialog_Track_Out : public StandardDialog
{
// Construction
public:
	CDialog_Track_Out(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Track_Out)
	enum { IDD = IDD_DIALOG_TRACK_OUT };
	CButton	m_chk_edit;
	CXPGroupBox	m_group_sorter_scrap_info;
	CXPGroupBox	m_group_server_scrap_info;
	CXPGroupBox	m_group_router_sorter_scrap_info;
	CXPGroupBox	m_group_pct_sorter_scrap_info;
	CXPGroupBox	m_group_fct_handler_scrap_info;
	CXPButton	m_btn_track_out;
	CMacButton	m_btn_add_fail;
	//}}AFX_DATA

// Attributes
public:
	CButton *mp_trackout_ok;	// 버튼에 대한 포인터 선언
	CFont* mp_trackout_font;			// 폰트 정보 설정 변수
	
	GridControlAlg m_pGrid;
	
	int mn_edit;
	int mn_timer_act;
	int mn_ec_server_step;
	int mn_scrap_info_step;
	int mn_delete_scrap_step;
	
	long ml_wait_time[3];
	long ml_retry_wait[3];

	int mn_track_out;
	int mn_add_fail;

	CString mstr_del_mdl_id;
	CString mstr_del_mdl_code;
	
// Operations
public:
	int OnTrackOut_Server_Send_Delete_Scrap();
	void OnTrackOut_Disp_Edit();
	void OnTrackOut_Display_Renew();
	int OnTrackOut_Server_Send_Scrap_Info();
	int OnTrackOut_Server_Send();
	void OnTrackOut_Disp_Sorter_Scrap_Info();
	void OnTrackOut_Disp_Server_Scrap_Info();
	void OnTrackOut_Disp_Router_Sorter_Scrap_Info();
	void OnTrackOut_Disp_FCT_Handler_Scrap_Info();
	void OnTrackOut_Disp_PCT_Sorter_Scrap_Info();
	void OnTrackOut_Init_PCT_Sorter_Scrap_Info();
	void OnTrackOut_Init_FCT_Handler_Scrap_Info();
	void OnTrackOut_Init_Router_Sorter_Scrap_Info();
	void OnTrackOut_Init_Server_Scrap_Info();
	void OnTrackOut_Init_Sorter_Scrap_Info();
	void OnTrackOut_GroupBox_Set();
	void OnTrackOut_Button_Set();
	void OnChangeValue(int nid, int nrow, int ncol);
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Track_Out)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Track_Out)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnTrackOut();
	afx_msg void OnBtnAddFail();
	virtual void OnOK();
	afx_msg void OnChkEdit();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrackOut_Info_Display(WPARAM wParam,LPARAM lParam);  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_TRACK_OUT_H__F2AE627D_0BFD_40C7_A33D_9144CF33278C__INCLUDED_)
