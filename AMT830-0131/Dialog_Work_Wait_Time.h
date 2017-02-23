#if !defined(AFX_DIALOG_WORK_WAIT_TIME_H__BEC47F03_BA53_41DF_924E_C8241916C7DE__INCLUDED_)
#define AFX_DIALOG_WORK_WAIT_TIME_H__BEC47F03_BA53_41DF_924E_C8241916C7DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Work_Wait_Time.h : header file
//
#include "Variable.h"			// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "MacButtons.h"
#include "EXDigitST.h"			// 디지털 카운터 생성 클래스 추가 
#include "FloatST.h"
#include "XpButton.h"			// XP 칼라 버튼 클래스
#include "MyBasicData.h"


/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Wait_Time dialog

class CDialog_Work_Wait_Time : public CDialog
{
// Construction
public:
	CFont* mp_wait_time_font;		// 폰트 정보 설정 변수
	TSpread			*m_grid_time;
	
	CMyBasicData     mcls_waittime;	// 기본 셋팅 정보 로딩 및 저장 클래스

	GridControlAlg	mp_grid;
	
	int				mn_wait_time_cnt;
	int				mn_on_wait[MAX_WAIT_TIME_NUM][2];
	int				mn_off_wait[MAX_WAIT_TIME_NUM][2];
	int				mn_limit[MAX_WAIT_TIME_NUM][2];
	int				mn_on_used[MAX_WAIT_TIME_NUM];
	int				mn_off_used[MAX_WAIT_TIME_NUM];
	int				mn_limit_used[MAX_WAIT_TIME_NUM];
	
	CString			mstr_wait_time_name[MAX_WAIT_TIME_NUM];
	
	int mn_time_count[100][2];
	
	int mn_enable;

public:
	void On_Controls_Enable(int nState);
	void OnWaitTime_Init_Time();
	void OnWaitTime_LogFile_Create();
	void OnWaitTime_Data_Apply();
	void OnWaitTime_Data_BackUp();
	int OnWaitTime_Data_Comparison();
	void OnWaitTime_Data_Display();
	void OnWaitTime_Data_Recovery();
	void OnWaitTime_Data_Set();
	void OnChangeValue(int nid, int nrow, int ncol);
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);


	CDialog_Work_Wait_Time(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Work_Wait_Time)
	enum { IDD = IDD_WORK_WAIT_TIME };
	CMacButton	m_btn_wait_apply;

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Work_Wait_Time)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Work_Wait_Time)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnWaitApply();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWaitTime_Info_Apply(WPARAM wParam, LPARAM lParam);  // Wait Time 화면 정보 갱신 요청 메시지
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORK_WAIT_TIME_H__BEC47F03_BA53_41DF_924E_C8241916C7DE__INCLUDED_)
