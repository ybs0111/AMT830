#if !defined(AFX_SCREEN_BASIC_H__75EFE953_60C7_432F_9759_19FF79697AC2__INCLUDED_)
#define AFX_SCREEN_BASIC_H__75EFE953_60C7_432F_9759_19FF79697AC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Basic.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "ListBoxST.h"      // 칼라 아이콘 리스트 박스 클래스
#include "xpbutton.h"		// XP 칼라 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "FloatST.h"
#include "LedButton.h"				// LED 버튼 생성 클래스 추가 

#include "Digit.h"

//#include "MyBasicData.h"	//20120522

// ******************************************************************************
// 설정 정보 로딩 및 저장 클래스
// ******************************************************************************
#include "MyBasicData.h"
// ******************************************************************************
/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Basic : public CFormView
{
public:
	CScreen_Basic();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Basic)

// Form Data
public:
	//{{AFX_DATA(CScreen_Basic)
	enum { IDD = IDD_SCREEN_BASIC };
	CGradientStatic	m_msg_vision_hs_total_err;
	CGradientStatic	m_msg_vision_hs_cont_err;
	CButtonST	m_chk_use_vision_hs_continue;
	CXPGroupBox	m_group_vison_hs_cont;
	CListBoxST	m_list_part_type;  // 20121220 김광수
	CMacButton	m_btn_part_delete;	// 20121220 김광수
	CMacButton	m_btn_part_create;	// 20121220 김광수
	CXPGroupBox	m_group_part_number;	// 20121220 김광수
	CDigit	m_dgt_curr_clamp_clip_bin;
	CDigit	m_dgt_curr_clip_bin;
	CGradientStatic	m_msg_stack_clip;
	CGradientStatic	m_msg_set_clip;
	CDigit	m_dgt_clip_bin;
	CDigit	m_dgt_clamp_clip_bin;
	CGradientStatic	m_msg_clip_bin;
	CGradientStatic	m_msg_clamp_clip_bin;
	CXPGroupBox	m_group_clip_bin;
	CButtonST	m_chk_remove_clip;
	CButtonST   m_chk_clip_alarm;
	CButtonST   m_chk_cap_remove; // 20140220
	CButtonST	m_chk_door_lock;
	CButtonST   m_chk_use_sorter_picker;
	CXPGroupBox	m_group_use_bin;
	CXPGroupBox m_group_use_door;
	CXPGroupBox m_group_use_sorter_picker;
	CButtonST	m_chk_bcr;
	CXPGroupBox	m_group_bcr;
	CButtonST	m_chk_hs_direction;
	CButtonST	m_chk_m_direction;
	CXPGroupBox	m_group_mhs_direction;
	CMacRadioButton	m_radio_notray;
	CXPGroupBox	m_group_recipe_select;
	CGradientStatic	m_msg_tray_y;
	CGradientStatic	m_msg_tray_x;
	CXPGroupBox	m_group_tray_size_setting;
	CXPGroupBox	m_group_alarm_delay_time;
	CXPGroupBox	m_group_network_wait_time;
	CXPGroupBox	m_group_network_retry_count;
	CGradientStatic	m_label_electrostatic_5;
	CGradientStatic	m_label_electrostatic_4;
	CGradientStatic	m_label_electrostatic_3;
	CGradientStatic	m_label_electrostatic_2;
	CGradientStatic	m_label_electrostatic_1;
	CGradientStatic	m_msg_electrostatic_5;
	CGradientStatic	m_msg_electrostatic_4;
	CGradientStatic	m_msg_electrostatic_3;
	CGradientStatic	m_msg_electrostatic_2;
	CGradientStatic	m_msg_electrostatic_1;
	CMacRadioButton	m_radio_device_mode_use;
	CMacRadioButton	m_radio_device_mode_notuse;
	CDigit	m_dgt_electrostatic_check_value;
	CListBoxST	m_list_device_type;
	CXPGroupBox	m_group_electrostatic_test;
	CXPGroupBox	m_group_electrostatic_check_value;
	CXPGroupBox	m_group_electrostatic_check_gap;
	CXPGroupBox	m_group_device_mode;
	CMacButton	m_btn_electostatic_test;
	CMacButton	m_btn_dvc_delete;
	CMacButton	m_btn_dvc_create;
	CMacButton	m_btn_basic_cancel;
	CMacButton	m_btn_basic_apply;
	CGradientStatic	m_msg_lotset_Module_load;
	CGradientStatic	m_msg_lotset_hs_left;
	CGradientStatic	m_msg_lotset_hs_right;
	CGradientStatic	m_msg_lotset_clip;
	CGradientStatic	m_msg_lotset_unloader;//20120707
	CGradientStatic	m_msg_lotset_reject;//20120707
	CXPGroupBox m_group_lotset;
	CXPGroupBox m_group_basic_speed;
	CXPGroupBox m_group_basic_run_speed;
	CXPGroupBox m_group_basic_manual_speed;
	CXPGroupBox m_group_basic_jog_speed;
	CXPGroupBox m_group_basic_barcode_speed;
	CXPGroupBox	m_group_label_info;
	CButtonST m_btn_label_create;
	CButtonST m_btn_label_delete;
	CListBoxST	m_list_label_type;
	CMacButton	m_btn_label_cancel;
	CMacButton	m_btn_label_apply;
	CGradientStatic m_msg_labelset_sd;
	CGradientStatic m_msg_labelset_lt;
	CGradientStatic m_msg_labelset_x1_pos;
	CGradientStatic m_msg_labelset_x2_pos;
	CGradientStatic m_msg_labelset_x3_pos;
	CGradientStatic m_msg_labelset_x4_pos;
	CGradientStatic m_msg_labelset_x5_pos;
	CGradientStatic m_msg_labelset_x6_pos;
	CGradientStatic m_msg_labelset_y1_pos;
	CGradientStatic m_msg_labelset_y2_pos;
	CGradientStatic m_msg_labelset_y3_pos;
	CGradientStatic m_msg_labelset_y4_pos;
	CGradientStatic m_msg_labelset_y5_pos;
	CGradientStatic m_msg_labelset_y6_pos;
	CGradientStatic m_msg_labelset_x1_width;
	CGradientStatic m_msg_labelset_x2_width;
	CGradientStatic m_msg_labelset_x3_width;
	CGradientStatic m_msg_labelset_x4_width;
	CGradientStatic m_msg_labelset_y1_height;
	CGradientStatic m_msg_labelset_y2_height;
	CGradientStatic m_msg_labelset_y3_height;
	CGradientStatic m_msg_labelset_y4_height;
	CXPGroupBox m_group_labelset_pos;
	CXPGroupBox m_group_labelset_size;
	CXPGroupBox m_group_label_print_error_cnt;
	CXPGroupBox m_group_use_clip_alarm;
	CXPGroupBox m_group_use_cap_remove; // 20140220
	CXPGroupBox m_group_label_bin_print; // 20140224
	CXPGroupBox m_group_alarm_file_read;
	CXPGroupBox m_group_label_vision_paper;
	CXPGroupBox m_group_picker_gap;	//20120625
	CXPGroupBox m_group_dvc_repick_cnt;//20120731
	CXPGroupBox m_group_node;	//2014.0818
	CDigit	m_dgt_picker_gap_module;
	CDigit	m_dgt_picker_gap_hs_left;
	CDigit	m_dgt_picker_gap_hs_right;
	CGradientStatic m_msg_picker_gap_module;
	CGradientStatic m_msg_picker_gap_hs_left;
	CGradientStatic m_msg_picker_gap_hs_right;
	CMacButton	m_btn_part_select;  // 20121231
	CListBoxST	m_list_part_select; // 20121231 
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_basic_font;			// 폰트 정보 설정 변수
	CFont* mp_label_font;			// 폰트 정보 설정 변수
	
	CMyBasicData    mcls_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스

	// **************************************************************************
	// 칼라 에디트 박스 클래스 변수 선언
	// **************************************************************************
	CEXDigitST  m_dgt_electrostatic_check_gap;			// 정전기 확인을 몇분에 한번씩 할것인가.
	CEXDigitST  m_dgt_network_wait_time;		// Network 대기 시간
	CEXDigitST  m_dgt_network_retry_cnt;		// Network Retry 횟수
	CEXDigitST	m_dgt_alarm_delay_time;

	CEXDigitST	m_dgt_tray_x;
	CEXDigitST	m_dgt_tray_y;

	//////////////////////////////////////// 20120522
	CEXDigitST	m_dgt_lotset_module_load;
	CEXDigitST	m_dgt_lotset_hs_left;
	CEXDigitST	m_dgt_lotset_hs_right;
	CEXDigitST	m_dgt_lotset_clip;
	CEXDigitST	m_dgt_lotset_unloader;//20120707
	CEXDigitST	m_dgt_lotset_reject;//20120707

	//////////////////////////////// 20120523
	CEXDigitST m_dgt_basic_run_speed;
	CEXDigitST m_dgt_basic_manual_speed;
	CEXDigitST m_dgt_basic_jog_speed;
	CEXDigitST m_dgt_basic_barcode_speed;
	

	///////////////////////////////////////////// 20120601
	CEXDigitST	m_dgt_labelset_sd;
	CEXDigitST	m_dgt_labelset_lt;
	CEXDigitST	m_dgt_labelset_lh1;
	CEXDigitST	m_dgt_labelset_lh2;
///////////20120906
	CDigit	m_dgt_labelset_x1_pos;
	CDigit	m_dgt_labelset_x2_pos;
	CDigit	m_dgt_labelset_x3_pos;
	CDigit	m_dgt_labelset_x4_pos;
	CDigit	m_dgt_labelset_x5_pos;
	CDigit	m_dgt_labelset_x6_pos;
	CDigit	m_dgt_labelset_y1_pos;
	CDigit	m_dgt_labelset_y2_pos;
	CDigit	m_dgt_labelset_y3_pos;
	CDigit	m_dgt_labelset_y4_pos;
	CDigit	m_dgt_labelset_y5_pos;
	CDigit	m_dgt_labelset_y6_pos;
	CDigit	m_dgt_labelset_x1_width;
	CDigit	m_dgt_labelset_x2_width;
	CDigit	m_dgt_labelset_x3_width;
	CDigit	m_dgt_labelset_x4_width;
	CDigit	m_dgt_labelset_y1_height;
	CDigit	m_dgt_labelset_y2_height;
	CDigit	m_dgt_labelset_y3_height;
	CDigit	m_dgt_labelset_y4_height;
/////////////


	CEXDigitST  m_dgt_Label_Print_Error_Cnt;
	CEXDigitST  m_dgt_Dvc_Repick_Cnt;// 20120731

	CEXDigitST	m_dgt_label_vision_paper;

	CEXDigitST	m_dgt_label_bin_print; // 20140224


	CEXDigitST  m_dgt_Dvc_Hs_Vis_Cont_Err;// 2015.0129
	CEXDigitST  m_dgt_Dvc_Hs_Vis_Tot_Err;// 2015.0129

	////2014.0818
	CEditEx  m_edit_ld_node;
	CEditEx  m_edit_hs_left_node;
	CEditEx  m_edit_hs_right_node;
	CEditEx  m_edit_clip_node;
	CEditEx  m_edit_uld_node;
	////

	CEditEx	 m_edit_device_type;
	
	int mn_device_mode[2];				// With, With Out, No Tray
	
	int mn_electrostatic_check_gap[2];	// 정전기 확인 시간.
	int mn_alarm_delay_time[2];

	int mn_network_wait_time[2];		// Network 대기 시간
	int mn_network_retry_cnt[2];		// Network Retry 횟수

	int mn_Label_Print_Error_Cnt[2];
	int mn_Dvc_Repick_Cnt[2]; //20120731
	int mn_tray_x[2];					// Tray X
	int mn_tray_y[2];					// Tray Y
	int mn_clamp_clip_bin[2];
	int mn_clip_bin[2];
	int mn_curr_clamp_clip_bin[2];
	int mn_curr_clip_bin[2];

	//////////////////////////////// 20120522
	int mn_lotset_module_load[2];
	int mn_lotset_hs_left[2];
	int mn_lotset_hs_right[2];	
	int mn_lotset_clip[2];
	int mn_lotset_unloader[2];//20120707
	int mn_lotset_reject[2];//20120707

	///////////////////////////////////////////// 20120601
	int mn_labelset_sd[2];
	int	mn_labelset_lt[2];
	int	mn_labelset_lh1[2];
	int	mn_labelset_lh2[2];
	
	///////////////////////////// 20120906
	double	md_labelset_x1_pos[2];
	double	md_labelset_x2_pos[2];
	double	md_labelset_x3_pos[2];
	double	md_labelset_x4_pos[2];
	double	md_labelset_x5_pos[2];
	double	md_labelset_x6_pos[2];
	double	md_labelset_y1_pos[2];
	double	md_labelset_y2_pos[2];
	double	md_labelset_y3_pos[2];
	double	md_labelset_y4_pos[2];
	double	md_labelset_y5_pos[2];
	double	md_labelset_y6_pos[2];
	double	md_labelset_x1_width[2];
	double	md_labelset_x2_width[2];
	double	md_labelset_x3_width[2];
	double	md_labelset_x4_width[2];
	double	md_labelset_y1_height[2];
	double	md_labelset_y2_height[2];
	double	md_labelset_y3_height[2];
	double	md_labelset_y4_height[2];
	//////////////////////////////


	//////////////////////////////// 20120523
	int mn_basic_run_speed[2];
	int mn_basic_manual_speed[2];
	int mn_basic_jog_speed[2];
	int mn_basic_barcode_speed[2];
	//////////////////////////////// 20120523
	CString mstr_label_name[2];		// 선택된 디바이스 종류 저장 변수
	//////////////////////////////// 20120530
	int mn_label_mode[2];				// With, With Out, No Tray
	int mn_label_name;					// 선택된 디바이스의 리스트 위치 저장 변수
	CString mstr_new_label;			// 새로 생성할 디바이스의 이름.
	//////////////////////////////// 20120531
	CString mstr_temp_label;			// 선택된 디바이스 종류 임시 저장 변수
	CString mstr_selected_lbl;
	BOOL b_create_list;

	//////////////////////////////// 
	double md_picker_gap_module[2];
	double md_picker_gap_hs_left[2];
	double md_picker_gap_hs_right[2];

	int	mn_label_vision_paper[2];

	int	mn_label_bin_print[2]; // 20140224



	double md_electrostatic_value[2];
	int mn_mode_m_direction[2];
	int mn_mode_hs_direction[2];
	int mn_mode_bcr[2];
	int mn_mode_module_clip_swing[2];
	int mn_mode_remove_clip[2];
	int mn_mode_clip_alarm[2];
	int mn_mode_door_lock[2];
	int mn_mode_use_sorter_picker[2];
	int mn_mode_use_cap_remove[2]; // 20140220

	//2015.0129
	int mn_mode_use_vis_cont_err[2];
	int mn_vis_cont_err[2];
	int mn_vis_tot_err[2];

	// **************************************************************************
	// 리스트 박스 컨트롤 정보 저장 클래스 변수 선언
	// **************************************************************************
	CImageList mp_device_image_list;	// 이미지 리스트 컨트롤 선언
	
	CString mstr_device_name[2];		// 선택된 디바이스 종류 저장 변수
	CString mstr_temp_device;			// 선택된 디바이스 종류 임시 저장 변수
	CString mstr_load_file;				// 로딩할 파일에 대한 [폴더]+[파일명] 저장 변수
	int mn_device_name;					// 선택된 디바이스의 리스트 위치 저장 변수
	
	CString mstr_new_device;			// 새로 생성할 디바이스의 이름.
	// **************************************************************************
	
	// **************************************************************************
	// 칼라 에디트 박스 클래스 변수 선언                                         
	// **************************************************************************
	CEditEx  m_edit_ecserver_port;
	// **************************************************************************
	
	// **************************************************************************
	// 데이터 저장 클래스 변수 선언
	// **************************************************************************
	BYTE nIPFiled0[2], nIPFiled1[2], nIPFiled2[2], nIPFiled3[2];		// 0: ECSERVER, 1: NEXT MACHINE

	CString mstr_ec_server_ip[2];
	CString mstr_ec_server_ldport[2];

	CString mstr_next_machine_ip[2];
	CString mstr_next_machine_port[2];

    //////////////////////////////// 20121220 김광수
	CString mstr_new_part;          // 새로 생성할 디바이스 이름.
	CString mstr_part_name[2];      // 디바이스 종류 저장 변수
	int  mn_part_mode[2];			// with, with out, no tray
	int  mn_response;
	CString st_g;
	//////////////////////////////// 
	int     mn_vision_hs_enable;
	BOOL	m_b_stop_req;
	long    l_bcr_send_wait[3];
	CString str_bcr_ml_name[2];   // 선택된 디바이스 종류 저장 변수
	int mn_bcr_ml_mode[2];		  // With, With Out, No Tray
	int mn_bcr_ml_name;

// Operations
public:
	int OnBasic_Data_Comparison();
	void OnBasic_Data_Recovery();
	int OnBasic_Input_Data_Check();
	void OnBasic_Data_Display();
	void OnBasic_DeviceData_Apply();
	void OnBasic_TitleBar_Info_Send();
	void OnBasic_LogFile_Create();
	void OnBasic_Data_Apply();
	void OnBasic_Electrostatic_Data_View(int nChannel);
	int OnBasic_Input_Device_Check();
	BOOL AddItem(int nIndex, WIN32_FIND_DATA *pfd);
	int OnBasic_Init_List(LPCTSTR pszPath);
	void OnBasic_Create_ImageList();
	void OnBasic_Device_Display(CString str_dvc);
	void OnBasic_Device_Focus_Set();
	void OnBasic_Create_Device();
	int OnBasic_New_Device_Check();
	void OnIP_Address(CString str_ip, int n_section);
	void OnBasic_DeviceMode_Change();
	void OnBasic_Data_Change();
	void OnBasic_Data_BackUp();
	void OnBasic_Label_Set();
	void OnBasic_Data_Set();
	void OnBasic_Digital_Count_Set();
	void OnBasic_EditBox_Set();
	void OnBasic_GroupBox_Set();

	///////////////////////////////// 20120529
	void OnBasic_Button_Set();
	///////////////////////////////// 20120530
	void OnBasic_Label_Focus_Set();
	int OnBasic_Label_Init_List(LPCTSTR pszPath);
	BOOL Label_AddItem(int nIndex, WIN32_FIND_DATA *pfd);
	int OnBasic_New_Label_Check(CString str_label);
	void OnBasic_Create_Label();
	///////////////////////////////// 20120531	
	int OnBasic_Label_Data_Comparison();
	void OnBasic_Label_Data_Apply();
	void OnBasic_Label_LogFile_Create();
	void OnBasic_Label_Data_BackUp();
	void OnBasic_Label_Data_Recovery();
	void OnBasic_Label_Data_Set();
	int OnBasic_Label_Input_Data_Check();
	void OnBasic_Label_DeviceData_Apply();
	///////////////////////////////// 20120601
	void OnBasic_Label_Font_Set();
	void OnBasic_Label_Data_Display();
	/////////////////////////////////
	void OnBasic_direction_Change();

	///////////////////////////////// 20121220 김광수
	void OnBasic_Create_Part_Number();
	void OnBasic_Part_List_TextSet();
	void OnBasic_Part_List_Delete();
	int  OnBasic_New_Part_Check(CString str_part);
	/////////////////////////////////
	void OnBasic_BCR_DISPLAY(int n_state);	//20130103
	int Manual_Bcr_HS_Yield_Test();
	void OnBasic_ML_BCR_Focus_Set();

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Basic)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Basic();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Basic)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRadioDeviceModeUse();
	afx_msg void OnRadioDeviceModeNotuse();
	afx_msg void OnDgtElectrostaticCheckGap();
	afx_msg void OnDgtElectrostaticCheckValue();
	afx_msg void OnBtnDvcCreate();
	afx_msg void OnSelchangeListDeviceType();
	afx_msg void OnBtnDvcDelete();
	afx_msg void OnBtnElectrostaticTest();
	afx_msg void OnBtnBasicApply();
	afx_msg void OnBtnBasicCancle();
	afx_msg void OnDgtNetworkWaitTime();
	afx_msg void OnDgtNetworkRetryCount();
	afx_msg void OnDgtAlarmDelayTime();
	afx_msg void OnDgtTrayX();
	afx_msg void OnDgtTrayY();
	afx_msg void OnDgtLotsetModuleLoad();
	afx_msg void OnDgtLotsetHsLeft();
	afx_msg void OnDgtLotsetHsRight();
	afx_msg void OnDgtLotsetClip();
	afx_msg void OnDgtRunSpeed();
	afx_msg void OnDgtJogSpeed();
	afx_msg void OnDgtBarcodeSpeed();
	afx_msg void OnDgtManualSpeed();
	afx_msg void OnBtnLabelCreate();
	afx_msg void OnBtnLabelDelete();
	afx_msg void OnBtnLabelApply();
	afx_msg void OnBtnLabelCancle();
	afx_msg void OnSelchangeListLabelType();
	afx_msg void OnDgtLabelsetSd();
	afx_msg void OnDgtLabelsetLt();
	afx_msg void OnDgtLabelsetX1Pos();
	afx_msg void OnDgtLabelsetX2Pos();
	afx_msg void OnDgtLabelsetX3Pos();
	afx_msg void OnDgtLabelsetX4Pos();
	afx_msg void OnDgtLabelsetX5Pos();
	afx_msg void OnDgtLabelsetX6Pos();
	afx_msg void OnDgtLabelsetY1Pos();
	afx_msg void OnDgtLabelsetY2Pos();
	afx_msg void OnDgtLabelsetY3Pos();
	afx_msg void OnDgtLabelsetY4Pos();
	afx_msg void OnDgtLabelsetY5Pos();
	afx_msg void OnDgtLabelsetY6Pos();
	afx_msg void OnDgtLabelsetX1Width();
	afx_msg void OnDgtLabelsetY1Height();
	afx_msg void OnDgtLabelsetX2Width();
	afx_msg void OnDgtLabelsetY2Height();
	afx_msg void OnDgtLabelsetX3Width();
	afx_msg void OnDgtLabelsetY3Height();
	afx_msg void OnDgtLabelsetX4Width();
	afx_msg void OnDgtLabelsetY4Height();
	afx_msg void OnDgtLabelsetX5Width();
	afx_msg void OnDgtLabelsetY5Height();
	afx_msg void OnDgtLabelsetX6Width();
	afx_msg void OnDgtLabelsetY6Height();
	afx_msg void OnRadioTrayModeUse();
	afx_msg void OnDgtLabelPrintErrorCnt();
	afx_msg void OnDgtPickerGapModule();
	afx_msg void OnDgtPickerGapHsLeft();
	afx_msg void OnDgtPickerGapHsRight();
	afx_msg void OnDgtLabelVisionPaper();
	afx_msg void OnDgtLabelBinPrint();  // 20140224
	afx_msg void OnDgtLotsetUnloader();
	afx_msg void OnDgtLotsetReject();
	afx_msg void OnChkMDirection();
	afx_msg void OnChkHsDirection();
	afx_msg void OnDgtDvcRepickCnt();
	afx_msg void OnChkMBcr();
	afx_msg void OnDgtLabelsetLh1();
	afx_msg void OnDgtLabelsetLh2();
	afx_msg void OnChkMRemoveClip();
	afx_msg void OnDgtClampClipBin();
	afx_msg void OnDgtClipBin();
	afx_msg void OnDgtCurrClampClipBin();
	afx_msg void OnDgtCurrClipBin();
	afx_msg void OnChkDoorLock();
	afx_msg void OnChkUseSorterPicker();
	afx_msg void OnBtnPartCreate();
	afx_msg void OnBtnPartDelete();
	afx_msg void OnBtnPartG1();
	afx_msg void OnBtnPartG2();
	afx_msg void OnBtnPartG3();
	afx_msg void OnBtnPartG4();
	afx_msg void OnBtnPartG5();
	afx_msg void OnBtnPartG6();
	afx_msg void OnBtnPartG7();
	afx_msg void OnBtnPartG8();
	afx_msg void OnSelchangeListPartType();
	afx_msg void OnBtnPartSelect();
	afx_msg void OnSelchangeListPartSelect();
	afx_msg void OnChkClipAlarm();
	afx_msg void OnChkCapRemove(); // 20140220
	afx_msg void OnBtnAlarmFileRead();
	afx_msg void OnBtnNode();
	afx_msg void OnChkVisionHsContinue();
	afx_msg void OnDgtDvcRepickCnt2();
	afx_msg void OnDgtDvcRepickCnt3();
	//}}AFX_MSG


	afx_msg LRESULT OnBasic_Info_Apply(WPARAM wParam, LPARAM lParam);  // BASIC 화면 정보 갱신 요청 
	afx_msg LRESULT OnBasic_Info_Display(WPARAM wParam,LPARAM lParam);  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 //20130103
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_BASIC_H__75EFE953_60C7_432F_9759_19FF79697AC2__INCLUDED_)
