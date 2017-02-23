#if !defined(AFX_DIALOG_WORK_BARCODE_PRINT_H__3A2DCE3A_8604_48B5_BB20_250F5861994F__INCLUDED_)
#define AFX_DIALOG_WORK_BARCODE_PRINT_H__3A2DCE3A_8604_48B5_BB20_250F5861994F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Work_Barcode_Print.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Barcode_Print dialog
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

// 
 


// #include "GridControlAlg.h"
#include "Digit.h"
#include "Dialog_Keyboard.h"
#include "MyBasicData.h"

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Alarm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDialog_Work_Barcode_Print : public CDialog
{
public:
	TSpread			*m_grid_io;
	void			Data_HistoryLog();
	void			Data_Recovery();
	int				Data_Comparison();
	void			Data_Apply();
	void			Data_Backup();
	void			Data_Init();

	long			Until_Wait_Time[3];
		
	CString			strBarcodeData;	//20120524
	BOOL			m_b_stop_req;	//20120525

	int				m_n_mode_barcode_print[2];
	CFont			* m_p_font;
	CFont			* mp_label_font;			// 폰트 정보 설정 변수
	CFont			* mp_label_print_font;
	CFont			* mp_label_print_font1;


	GridControlAlg	m_p_grid;
	CDialog_Keyboard  keyboard_dlg;

	CMyBasicData    mcls_barcode;		// 기본 셋팅 정보 로딩 및 저장 클래스

	///////////////////////////////////////////// 20120601
	int mn_labelset_sd1[2];
	int	mn_labelset_lt1[2];
	int	mn_labelset_lh1[2];
	int	mn_labelset_lh2[2];
	/////////// 20120906
	double md_labelset_x1_pos1[2];
	double md_labelset_x2_pos1[2];
	double md_labelset_x3_pos1[2];
	double md_labelset_x4_pos1[2];
	double md_labelset_x5_pos1[2];
	double md_labelset_x6_pos1[2];
	double md_labelset_y1_pos1[2];
	double md_labelset_y2_pos1[2];
	double md_labelset_y3_pos1[2];
	double md_labelset_y4_pos1[2];
	double md_labelset_y5_pos1[2];
	double md_labelset_y6_pos1[2];
	double md_labelset_x1_width1[2];
	double md_labelset_x2_width1[2];
	double md_labelset_x3_width1[2];
	double md_labelset_x4_width1[2];
	double md_labelset_y1_height1[2];
	double md_labelset_y2_height1[2];
	double md_labelset_y3_height1[2];
	double md_labelset_y4_height1[2];
	///////////
	int mn_labelset_func_paper[2];	//20120625

	///////////////////////////////////////////// 20120601
	CEXDigitST	m_dgt_labelset_sd1;
	CEXDigitST	m_dgt_labelset_lt1;
	CEXDigitST	m_dgt_labelset_lh1;
	CEXDigitST	m_dgt_labelset_lh2;
/////////// 20120906
	CDigit	m_dgt_labelset_x1_pos1;
	CDigit	m_dgt_labelset_x2_pos1;
	CDigit	m_dgt_labelset_x3_pos1;
	CDigit	m_dgt_labelset_x4_pos1;
	CDigit	m_dgt_labelset_x5_pos1;
	CDigit	m_dgt_labelset_x6_pos1;
	CDigit	m_dgt_labelset_y1_pos1;
	CDigit	m_dgt_labelset_y2_pos1;
	CDigit	m_dgt_labelset_y3_pos1;
	CDigit	m_dgt_labelset_y4_pos1;
	CDigit	m_dgt_labelset_y5_pos1;
	CDigit	m_dgt_labelset_y6_pos1;
	CDigit	m_dgt_labelset_x1_width1;
	CDigit	m_dgt_labelset_x2_width1;
	CDigit	m_dgt_labelset_x3_width1;
	CDigit	m_dgt_labelset_x4_width1;
	CDigit	m_dgt_labelset_y1_height1;
	CDigit	m_dgt_labelset_y2_height1;
	CDigit	m_dgt_labelset_y3_height1;
	CDigit	m_dgt_labelset_y4_height1;
///////////


	CEXDigitST m_dgt_labelset_func_paper;	//20120625

	int mn_Prt_Comp_Reply[2];
	int mn_Prt_Mode[2];

	//////////////////////////////// 20120523
	CString mstr_label_name1[2];		// 선택된 디바이스 종류 저장 변수
	//////////////////////////////// 20120530
	int mn_label_mode1[2];				// With, With Out, No Tray
	int mn_label_name1;					// 선택된 디바이스의 리스트 위치 저장 변수
	CString mstr_new_label1;			// 새로 생성할 디바이스의 이름.
	//////////////////////////////// 20120531
	CString mstr_temp_label1;			// 선택된 디바이스 종류 임시 저장 변수
	CString mstr_selected_lbl1;
	BOOL b_create_list1;

	//////////////////// 20120605
	CRect modLabelTest;
	CRect modLabelPos; 
	CRect modLabelPos2; 
	CRect modLabelPos3; 
	CRect modLabelPos4; 
	CRect modLabelPos5; 
	CRect modLabelPos6; 

	CRect modLabelSize; 
	CRect modLabelSize2; 
	CRect modLabelSize3; 

	int	m_nmodLabelTestTop, m_nmodLabelTestBottm, m_nmodLabelTestLeft, m_nmodLabelTestRight, m_nmodLabelTestWidth, m_nmodLabelTestHeight;

	int	m_nmodLabelPosTop, m_nmodLabelPosBottm, m_nmodLabelPosLeft, m_nmodLabelPosRight, m_nmodLabelPosWidth, m_nmodLabelPosHeight;
	int	m_nmodLabelPosTop2, m_nmodLabelPosBottm2, m_nmodLabelPosLeft2, m_nmodLabelPosRight2, m_nmodLabelPosWidth2, m_nmodLabelPosHeight2;
	int	m_nmodLabelPosTop3, m_nmodLabelPosBottm3, m_nmodLabelPosLeft3, m_nmodLabelPosRight3, m_nmodLabelPosWidth3, m_nmodLabelPosHeight3;
	int	m_nmodLabelPosTop4, m_nmodLabelPosBottm4, m_nmodLabelPosLeft4, m_nmodLabelPosRight4, m_nmodLabelPosWidth4, m_nmodLabelPosHeight4;
	int	m_nmodLabelPosTop5, m_nmodLabelPosBottm5, m_nmodLabelPosLeft5, m_nmodLabelPosRight5, m_nmodLabelPosWidth5, m_nmodLabelPosHeight5;
	int	m_nmodLabelPosTop6, m_nmodLabelPosBottm6, m_nmodLabelPosLeft6, m_nmodLabelPosRight6, m_nmodLabelPosWidth6, m_nmodLabelPosHeight6;

	int	m_nmodLabelSizeTop, m_nmodLabelSizeBottm, m_nmodLabelSizeLeft, m_nmodLabelSizeRight, m_nmodLabelSizeWidth, m_nmodLabelSizeHeight;
	int	m_nmodLabelSizeTop2, m_nmodLabelSizeBottm2, m_nmodLabelSizeLeft2, m_nmodLabelSizeRight2, m_nmodLabelSizeWidth2, m_nmodLabelSizeHeight2;
	int	m_nmodLabelSizeTop3, m_nmodLabelSizeBottm3, m_nmodLabelSizeLeft3, m_nmodLabelSizeRight3, m_nmodLabelSizeWidth3, m_nmodLabelSizeHeight3;

	int	m_nmodLabelResetPosTop, m_nmodLabelResetPosBottm, m_nmodLabelResetPosLeft, m_nmodLabelResetPosRight, m_nmodLabelResetPosWidth, m_nmodLabelResetPosHeight;
	int	m_nmodLabelResetPosTop2, m_nmodLabelResetPosBottm2, m_nmodLabelResetPosLeft2, m_nmodLabelResetPosRight2, m_nmodLabelResetPosWidth2, m_nmodLabelResetPosHeight2;
	int	m_nmodLabelResetPosTop3, m_nmodLabelResetPosBottm3, m_nmodLabelResetPosLeft3, m_nmodLabelResetPosRight3, m_nmodLabelResetPosWidth3, m_nmodLabelResetPosHeight3;
	int	m_nmodLabelResetPosTop4, m_nmodLabelResetPosBottm4, m_nmodLabelResetPosLeft4, m_nmodLabelResetPosRight4, m_nmodLabelResetPosWidth4, m_nmodLabelResetPosHeight4;
	int	m_nmodLabelResetPosTop5, m_nmodLabelResetPosBottm5, m_nmodLabelResetPosLeft5, m_nmodLabelResetPosRight5, m_nmodLabelResetPosWidth5, m_nmodLabelResetPosHeight5;
	int	m_nmodLabelResetPosTop6, m_nmodLabelResetPosBottm6, m_nmodLabelResetPosLeft6, m_nmodLabelResetPosRight6, m_nmodLabelResetPosWidth6, m_nmodLabelResetPosHeight6;

	int	m_nmodLabelResetSizeTop, m_nmodLabelResetSizeBottm, m_nmodLabelResetSizeLeft, m_nmodLabelResetSizeRight, m_nmodLabelResetSizeWidth, m_nmodLabelResetSizeHeight;
	int	m_nmodLabelResetSizeTop2, m_nmodLabelResetSizeBottm2, m_nmodLabelResetSizeLeft2, m_nmodLabelResetSizeRight2, m_nmodLabelResetSizeWidth2, m_nmodLabelResetSizeHeight2;
	int	m_nmodLabelResetSizeTop3, m_nmodLabelResetSizeBottm3, m_nmodLabelResetSizeLeft3, m_nmodLabelResetSizeRight3, m_nmodLabelResetSizeWidth3, m_nmodLabelResetSizeHeight3;

	int n_barcoderead_step;//20140214
	long NetworkConnectTime[3];//20140214
	long ml_untilwait_time[3];
	//////////////////////////////// 
	int mn_printnum;
	////////////////////////////////////////


	
// Construction
public:
	void Init_Label();
	void OnInputCheck();
	void OnOutputCheck();
	void Init_Grid_Barcode();
	void Init_Button();
	void Init_Group();
	void Init_Data();//20120828
	int OutLptPort(CString str_text);	//20120620
	void OnBARCODE_HS_DISPLAY(int n_state);	//20120625

	void OnBARCODE_CREATE_BUTTON(int n_state);//2014,0411

	CDialog_Work_Barcode_Print(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Work_Barcode_Print)
	enum { IDD = IDD_WORK_BARCODE_PRINT };
	CButtonST	m_btn_barcode_create;
	CEdit	m_ed_barcode_create;
	CXPGroupBox	m_group_barcode_label;
	CGradientStatic	m_msg_barcode_label;
	CButtonST	m_btn_barcode_print;
	CButtonST	m_btn_apply;
	CXPGroupBox	m_group_barcode_status;
	CXPGroupBox	m_group_barcode_mode;
	CButtonST	m_chk_barcode_mode;

	//////////////////////////////////20120604
	CXPGroupBox	m_group_label_info;	
	CListBoxST	m_list_label_type1;
	///////////////////////////////////////////// 20120531
	CGradientStatic m_msg_labelset_sd1;
	CGradientStatic m_msg_labelset_lt1;
	CGradientStatic m_msg_labelset_lh1;
	CGradientStatic m_msg_labelset_lh2;
	CGradientStatic m_msg_labelset_x1_pos1;
	CGradientStatic m_msg_labelset_x2_pos1;
	CGradientStatic m_msg_labelset_x3_pos1;
	CGradientStatic m_msg_labelset_x4_pos1;
	CGradientStatic m_msg_labelset_x5_pos1;
	CGradientStatic m_msg_labelset_x6_pos1;
	CGradientStatic m_msg_labelset_y1_pos1;
	CGradientStatic m_msg_labelset_y2_pos1;
	CGradientStatic m_msg_labelset_y3_pos1;
	CGradientStatic m_msg_labelset_y4_pos1;
	CGradientStatic m_msg_labelset_y5_pos1;
	CGradientStatic m_msg_labelset_y6_pos1;
	///////////////////////////////////////////// 20120601
	CGradientStatic m_msg_labelset_x1_width1;
	CGradientStatic m_msg_labelset_x2_width1;
	CGradientStatic m_msg_labelset_x3_width1;
	CGradientStatic m_msg_labelset_x4_width1;
	CGradientStatic m_msg_labelset_y1_height1;
	CGradientStatic m_msg_labelset_y2_height1;
	CGradientStatic m_msg_labelset_y3_height1;
	CGradientStatic m_msg_labelset_y4_height1;
	CXPGroupBox m_group_labelset_pos2;
	CXPGroupBox m_group_labelset_size2;
	CXPGroupBox m_group_labelset_pos3;
	
	////////////////////////////////////// 20120625
	CXPGroupBox m_group_labelset_data;	
	CXPGroupBox m_group_labelset_func;	
	CXPGroupBox m_group_labelset_func2;	
	CXPGroupBox m_group_labelset_func3;	
	CXPGroupBox m_group_labelset_func4;	
	CXPGroupBox m_group_labelset_func5;	

	CEdit	m_ed_barcode_create2;	
	CEdit	m_ed_barcode_create3;
	//////////////////////////////////////

	CStatic	m_static_label_view;
	CStatic	m_static_label_view2;
	CStatic	m_static_label_view3;
	CStatic	m_static_label_view4;

	CButtonST m_btn_labelset_move;
	CButtonST m_btn_labelset_move2;
	CButtonST m_btn_labelset_move3;
	CButtonST m_btn_labelset_move4;
	CButtonST m_btn_labelset_move5;
	CButtonST m_btn_labelset_move6;
	
	///////////////////////////////////////////// 20120605
	CXPGroupBox m_group_labeltest;

	CGradientStatic m_msg_number;
	CGradientStatic m_msg_number2;
	CGradientStatic m_msg_number3;
	CGradientStatic m_msg_number4;
	CGradientStatic m_msg_number5;
	CGradientStatic m_msg_number6;
	CGradientStatic m_msg_number7;
	CGradientStatic m_msg_number8;
	CGradientStatic m_msg_number9;
	CGradientStatic m_msg_number10;

	CEdit	m_edit_labelset_data;
	CEdit	m_edit_labelset_data2;
	CEdit	m_edit_labelset_data3;
	CEdit	m_edit_labelset_data4;

	CGradientStatic m_msg_preview;
	CGradientStatic m_msg_preview2;
	CGradientStatic m_msg_preview3;
	CGradientStatic m_msg_preview4;

	CLedButton m_radio_prt_comp_reply_use;
	CLedButton m_radio_prt_comp_reply_notuse;

	CLedButton m_radio_prt_tear_off_mode;
	CLedButton m_radio_prt_peal_off_mode;
	CLedButton m_radio_prt_rewind_mode;

	///////////////////////////////////////////// 


	//}}AFX_DATA
public:
	///////////////////////////////////////////// 20120604
	void OnBarcode_Digital_Count_Set();
	void OnBarcode_LogFile_Create();
	void OnBarcode_Label_Data_Apply();

	void OnBarcode_Label_DeviceData_Apply();
	void OnBarcode_Label_Data_Set();				// 전역 변수의 Data를 받아온다.
	void OnBarcode_Label_Data_BackUp();
	void OnBarcode_Label_Data_Display();
	void OnBarcode_Label_Data_Recovery();
	void OnBarcode_Label_Font_Set();
	int OnBarcode_Label_Input_Data_Check();
	int OnBarcode_Label_Init_List(LPCTSTR pszPath);
	void OnBarcode_Label_Focus_Set();
	BOOL Label_AddItem(int nIndex, WIN32_FIND_DATA *pfd);
	void LogFile_Create();
	
	int OnBarcodeRead_Excution();//20140214
		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Work_Barcode_Print)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Work_Barcode_Print)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnChkBarcodeMode();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnApply();
	afx_msg void OnBtnBarcodePrint();
	afx_msg void OnBtnBarcodeCreate();
	afx_msg void OnMsgBarcodeLabel();
	afx_msg void OnButtonStop();
	afx_msg void OnSelchangeListLabelType1();
	afx_msg void OnDgtLabelsetSd1();
	afx_msg void OnDgtLabelsetLt1();
	afx_msg void OnDgtLabelsetX1Pos1();
	afx_msg void OnDgtLabelsetX2Pos1();
	afx_msg void OnDgtLabelsetX3Pos1();
	afx_msg void OnDgtLabelsetX4Pos1();
	afx_msg void OnDgtLabelsetX5Pos1();
	afx_msg void OnDgtLabelsetX6Pos1();
	afx_msg void OnDgtLabelsetY1Pos1();
	afx_msg void OnDgtLabelsetY2Pos1();
	afx_msg void OnDgtLabelsetY3Pos1();
	afx_msg void OnDgtLabelsetY4Pos1();
	afx_msg void OnDgtLabelsetY5Pos1();
	afx_msg void OnDgtLabelsetY6Pos1();
	afx_msg void OnDgtLabelsetX1Width1();
	afx_msg void OnDgtLabelsetY1Height1();
	afx_msg void OnDgtLabelsetX2Width1();
	afx_msg void OnDgtLabelsetY2Height1();
	afx_msg void OnDgtLabelsetX3Width1();
	afx_msg void OnDgtLabelsetY3Height1();
	afx_msg void OnDgtLabelsetX4Width1();
	afx_msg void OnDgtLabelsetY4Height1();
	afx_msg void OnDgtLabelsetX5Width1();
	afx_msg void OnDgtLabelsetY5Height1();
	afx_msg void OnDgtLabelsetX6Width1();
	afx_msg void OnDgtLabelsetY6Height1();
	afx_msg void OnBtnLabelsetMove();
	afx_msg void OnBtnLabelsetMove2();
	afx_msg void OnBtnLabelsetMove3();
	afx_msg void OnBtnLabelsetMove4();
	afx_msg void OnBtnLabelsetMove5();
	afx_msg void OnBtnLabelsetMove6();
	afx_msg void OnBtnLabelApply();
	afx_msg void OnBtnLabelCancle();
	afx_msg void OnBtnLabelsetMove7();
	afx_msg void OnBtnLabelsetMove8();
	afx_msg void OnBtnLabelsetMove9();
	afx_msg void OnBtnLabelsetMove10();
	afx_msg void OnBtnLabelsetSize();
	afx_msg void OnBtnLabelsetSize2();
	afx_msg void OnBtnLabelsetSize3();
	afx_msg void OnBtnLabelsetSize4();
	afx_msg void OnBtnLabelsetDataInput();
	afx_msg void OnBtnLabelsetDataInput2();
	afx_msg void OnBtnLabelsetDataInput3();
	afx_msg void OnBtnLabelsetDataInput4();
	afx_msg void OnBtnBarcodeCreate2();	//20120620
	afx_msg void OnBtnBcr();
	afx_msg void OnButton10();
	afx_msg void OnBtnBarcodeFunc3();
	afx_msg void OnBtnBarcodeFunc();
	afx_msg void OnBtnBarcodeFunc2();
	afx_msg void OnBtnBarcodeFunc4();
	afx_msg void OnBtnBarcodeFunc5();
	afx_msg void OnDgtLabelsetFuncPaper();
	afx_msg void OnBtnBarcodeFunc6();
	afx_msg void OnBtnBarcodeFunc7();
	afx_msg void OnBtnBarcodeFunc8();
	afx_msg void OnBtnBarcodeCreate3();
	afx_msg void OnRadioPrtCompReplyUse();
	afx_msg void OnRadioPrtCompReplyNotuse();
	afx_msg void OnRadioPrtMode();
	afx_msg void OnRadioPrtMode2();
	afx_msg void OnRadioPrtMode3();
	afx_msg void OnDgtLabelsetLh1();
	afx_msg void OnDgtLabelsetLh2();
	afx_msg void OnBtnBarcodePrintReset();
	afx_msg void OnBtnPrintCalibrate();
	//}}AFX_MSG
	afx_msg void OnCell_Left_Click(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWork_Barcode_Info_Display(WPARAM wParam,LPARAM lParam);  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 //20120621
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORK_BARCODE_PRINT_H__3A2DCE3A_8604_48B5_BB20_250F5861994F__INCLUDED_)
