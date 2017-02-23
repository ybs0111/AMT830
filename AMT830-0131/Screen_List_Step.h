#if !defined(AFX_SCREEN_LIST_STEP_H__801DB059_9152_4298_BDEC_105AD9743B11__INCLUDED_)
#define AFX_SCREEN_LIST_STEP_H__801DB059_9152_4298_BDEC_105AD9743B11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_List_Step.h : header file
//
#include "Variable.h"			// 전역 변수 정의 클래스 추가
#include "Public_Function.h"	// 공용 함수 정의 클래스 추가
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editEx.h"				// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"			// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "ListBoxST.h"			// 칼라 아이콘 리스트 박스 클래스 
#include "xpbutton.h"			// XP 칼라 버튼 클래스
#include "EXDigitST.h"			// 디지털 카운터 클래스
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "FloatST.h"
#include "FastechPublic_IO.h"		// Fastech 관련 동작 Class
#include "LedButton.h"			// LED 버튼 클래스

/* ****************************************************************************** */
/* 쓰레드 스텝 정보 저장 및 로딩 클래스 추가                                      */
/* ****************************************************************************** */
#include "MyBasicData.h"
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 스텝 정보 출력 타이머 선언                                                     */
/* ****************************************************************************** */
#define TMR_STEP	    862
/* ****************************************************************************** */
/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_List_Step : public CFormView
{
public:
	CScreen_List_Step();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_List_Step)

// Form Data
public:
	//{{AFX_DATA(CScreen_List_Step)
	enum { IDD = IDD_SCREEN_LIST_STEP };
	CNewLabel	m_msg_thread_12;
	CGradientStatic	m_data_thread_12;
	CNewLabel	m_msg_thread_11;
	CGradientStatic	m_data_thread_11;
	CNewLabel	m_msg_thread_9;
	CNewLabel	m_msg_thread_8;
	CNewLabel	m_msg_thread_7;
	CNewLabel	m_msg_thread_10;
	CGradientStatic	m_data_thread_9;
	CGradientStatic	m_data_thread_8;
	CGradientStatic	m_data_thread_7;
	CGradientStatic	m_data_thread_10;
	CNewLabel	m_msg_thread_6;
	CNewLabel	m_msg_thread_5;
	CNewLabel	m_msg_thread_4;
	CNewLabel	m_msg_thread_3;
	CNewLabel	m_msg_thread_2;
	CNewLabel	m_msg_thread_1;
	CXPGroupBox	m_group_step_list;
	CGradientStatic	m_data_thread_6;
	CGradientStatic	m_data_thread_5;
	CGradientStatic	m_data_thread_4;
	CGradientStatic	m_data_thread_3;
	CGradientStatic	m_data_thread_2;
	CGradientStatic	m_data_thread_1;
	CXPButton	m_btn_step_save;

	CNewLabel	m_msg_thread_13;
	CGradientStatic	m_data_thread_13;
	CNewLabel	m_msg_thread_14;
	CGradientStatic	m_data_thread_14;
	CNewLabel	m_msg_thread_15;
	CGradientStatic	m_data_thread_15;

	CNewLabel	m_msg_thread_16;
	CGradientStatic	m_data_thread_16;
	CNewLabel	m_msg_thread_17;
	CGradientStatic	m_data_thread_17;
	CNewLabel	m_msg_thread_18;
	CGradientStatic	m_data_thread_18;
	CNewLabel	m_msg_thread_19;
	CGradientStatic	m_data_thread_19;
	CNewLabel	m_msg_thread_20;
	CGradientStatic	m_data_thread_20;
	CNewLabel	m_msg_thread_21;
	CGradientStatic	m_data_thread_21;
	CNewLabel	m_msg_thread_22;
	CGradientStatic	m_data_thread_22;
	CNewLabel	m_msg_thread_23;
	CGradientStatic	m_data_thread_23;
	CNewLabel	m_msg_thread_24;
	CGradientStatic	m_data_thread_24;
	CNewLabel	m_msg_thread_25;
	CGradientStatic	m_data_thread_25;
	CNewLabel	m_msg_thread_26;
	CGradientStatic	m_data_thread_26;
	CNewLabel	m_msg_thread_27;
	CGradientStatic	m_data_thread_27;
	CNewLabel	m_msg_thread_28;
	CGradientStatic	m_data_thread_28;
	CNewLabel	m_msg_thread_29;
	CGradientStatic	m_data_thread_29;
	CNewLabel	m_msg_thread_30;
	CGradientStatic	m_data_thread_30;
	CNewLabel	m_msg_thread_31;
	CGradientStatic	m_data_thread_31;
	CNewLabel	m_msg_thread_32;
	CGradientStatic	m_data_thread_32;
	CNewLabel	m_msg_thread_33;
	CGradientStatic	m_data_thread_33;
	CNewLabel	m_msg_thread_34;
	CGradientStatic	m_data_thread_34;
	CNewLabel	m_msg_thread_35;
	CGradientStatic	m_data_thread_35;
	CNewLabel	m_msg_thread_36;
	CGradientStatic	m_data_thread_36;
	CNewLabel	m_msg_thread_37;
	CGradientStatic	m_data_thread_37;
	CNewLabel	m_msg_thread_38;
	CGradientStatic	m_data_thread_38;
	CNewLabel	m_msg_thread_39;
	CGradientStatic	m_data_thread_39;
	CNewLabel	m_msg_thread_40;
	CGradientStatic	m_data_thread_40;
	CNewLabel	m_msg_thread_41;
	CGradientStatic	m_data_thread_41;
	CNewLabel	m_msg_thread_42;
	CGradientStatic	m_data_thread_42;
	CNewLabel	m_msg_thread_43;
	CGradientStatic	m_data_thread_43;
	CNewLabel	m_msg_thread_44;
	CGradientStatic	m_data_thread_44;
	CNewLabel	m_msg_thread_45;
	CGradientStatic	m_data_thread_45;
	CNewLabel	m_msg_thread_46;
	CGradientStatic	m_data_thread_46;
	CNewLabel	m_msg_thread_47;
	CGradientStatic	m_data_thread_47;
	CNewLabel	m_msg_thread_48;
	CGradientStatic	m_data_thread_48;
	CNewLabel	m_msg_thread_49;
	CGradientStatic	m_data_thread_49;
	CNewLabel	m_msg_thread_50;
	CGradientStatic	m_data_thread_50;
	CNewLabel	m_msg_thread_51;
	CGradientStatic	m_data_thread_51;
	CNewLabel	m_msg_thread_52;
	CGradientStatic	m_data_thread_52;
	CNewLabel	m_msg_thread_53;
	CGradientStatic	m_data_thread_53;
	CNewLabel	m_msg_thread_54;
	CGradientStatic	m_data_thread_54;
	CNewLabel	m_msg_thread_55;
	CGradientStatic	m_data_thread_55;
	CNewLabel	m_msg_thread_56;
	CGradientStatic	m_data_thread_56;
	CNewLabel	m_msg_thread_57;
	CGradientStatic	m_data_thread_57;
	CNewLabel	m_msg_thread_58;
	CGradientStatic	m_data_thread_58;
	CNewLabel	m_msg_thread_59;
	CGradientStatic	m_data_thread_59;
	CNewLabel	m_msg_thread_60;
	CGradientStatic	m_data_thread_60;
	CNewLabel	m_msg_thread_61;
	CGradientStatic	m_data_thread_61;
	CNewLabel	m_msg_thread_62;
	CGradientStatic	m_data_thread_62;
	CNewLabel	m_msg_thread_63;
	CGradientStatic	m_data_thread_63;
	CNewLabel	m_msg_thread_64;
	CGradientStatic	m_data_thread_64;
	CNewLabel	m_msg_thread_65;
	CGradientStatic	m_data_thread_65;


	//}}AFX_DATA

// Attributes
public:
	CFont* mp_step_list_font;  // 폰트 설정 변수 선언 

	/* ************************************************************************** */
	/* 생성할 스텝 저장 파일명 및 인덱스 로딩/저장 클래스 변수                    */
	/* ************************************************************************** */
	CMyBasicData mcls_file;   
	/* ************************************************************************** */

	int mn_timer_kill;  // 타이머 삭제 요청 플래그 

	/* ************************************************************************** */
	/* 쓰레드 스텝 정보 저장 클래스 변수 선언                                     */
	/* ************************************************************************** */
	CString mstr_thread_1;	// 로더 엘레베이터 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_2;  // 로더 트렌스퍼 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_3;	// 언로더 엘레베이터 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_4;	// 언로더 트렌스퍼 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_5;	// XYZ 로봇 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_6;	// 테스트 사이트 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_7;
	CString mstr_thread_8;
	CString mstr_thread_9;
	CString mstr_thread_10;
	CString mstr_thread_11;
	CString mstr_thread_12;
	CString mstr_thread_13;
	CString mstr_thread_14;
	CString mstr_thread_15;
/////////////////////////////// 20120707
	CString mstr_thread_16;
	CString mstr_thread_17;
	CString mstr_thread_18;
	CString mstr_thread_19;
	CString mstr_thread_20;
	CString mstr_thread_21;
	CString mstr_thread_22;
	CString mstr_thread_23;
	CString mstr_thread_24;
	CString mstr_thread_25;
	CString mstr_thread_26;
	CString mstr_thread_27;
    CString mstr_thread_28;
    CString mstr_thread_29;
	CString mstr_thread_30;
	CString mstr_thread_31;
	CString mstr_thread_32;
	CString mstr_thread_33;
	CString mstr_thread_34;
	CString mstr_thread_35;
	CString mstr_thread_36;
	CString mstr_thread_37;
	CString mstr_thread_38;
	CString mstr_thread_39;
	CString mstr_thread_40;
	CString mstr_thread_41;
	CString mstr_thread_42;
	CString mstr_thread_43;
	CString mstr_thread_44;
	CString mstr_thread_45;
	CString mstr_thread_46;
	CString mstr_thread_47;
	CString mstr_thread_48;
	CString mstr_thread_49;
	CString mstr_thread_50;
	CString mstr_thread_51;
	CString mstr_thread_52;
	CString mstr_thread_53;
	CString mstr_thread_54;
	CString mstr_thread_55;
	CString mstr_thread_56;
	CString mstr_thread_57;
	CString mstr_thread_58;
	CString mstr_thread_59;
	CString mstr_thread_60;
	CString mstr_thread_61;
	CString mstr_thread_62;
	CString mstr_thread_63;
	CString mstr_thread_64;
	CString mstr_thread_65;

	/* ************************************************************************** */

// Operations
public:
	void TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp);
	CString FileTextLine(int dir, int length);
	void On_Step_List_File_Info_Save(CString str_step_file, CString str_file_index);
	int On_Step_List_File_Search(CString str_chk_file);
	void On_Step_List_File_Create(CString str_step_file, CString str_create_file, CString str_time);
	void On_Step_List_File_Compare(CString str_folder, CString str_file_name, CString str_time_info);
	void On_Step_List_File_Name_Init();
	void OnList_Step_Data_Display();
	void OnList_Step_Data_Set();
	void OnList_Step_Lable_Set();
	void OnList_Step_GroupBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_List_Step)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_List_Step();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_List_Step)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnStepSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_LIST_STEP_H__801DB059_9152_4298_BDEC_105AD9743B11__INCLUDED_)
