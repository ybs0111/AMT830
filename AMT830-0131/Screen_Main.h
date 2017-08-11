#if !defined(AFX_SCREEN_MAIN_H__7A483F1B_61FF_4EB9_B41D_251A637D6074__INCLUDED_)
#define AFX_SCREEN_MAIN_H__7A483F1B_61FF_4EB9_B41D_251A637D6074__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Main.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"

#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "LedButton.h"      // LED 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 생성 클래스 추가 
#include "MacButtons.h"
#include "Digit.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "CheckerCtrl.h"
#include "Picture.h"

#include "FloatST.h"

#include "MyBasicData.h"//20120924
#include "PictureEx.h"		// GIF 파일을 로딩하기 위한 클래스 추가 
#include "BmpImageST.h"		// 알람 비트맵 로딩 클래스 추가 

#include "Animate.h"

#define TM_LOTEND_READY				5990
#define TM_BCR_HS_DATA_RESET		5991//20120924
/////////////////////////////////////////////////////////////////////////////
// CScreen_Main form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Main : public CFormView
{
public:
	int				m_npos;

	TSpread			*m_grid_lot;
	TSpread			*m_grid_status;
	TSpread			*m_grid_time;
	GridControlAlg	m_p_grid;

	CAnimate         m_Animate;

public:
	CScreen_Main();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Main)

// Form Data
public:
	//{{AFX_DATA(CScreen_Main)
	enum { IDD = IDD_SCREEN_MAIN };
	CMacButton	m_btn_unload_tray2;
	CMacButton	m_btn_back_tray2;
	CMacButton	m_btn_front_tray2;
	CMacButton	m_btn_mdl_tray2;
	CMacButton	m_clip_recovery;
	CMacButton	m_clip_remove;
	CMacButton	m_btn_multilot_init;
	CMacButton	m_btn_viscnt_reset;
	CMacButton	m_btn_lot_cancel;
	CButtonST m_pad_bcr_id4_1;
	CButtonST m_pad_bcr_id3_1;
	CButtonST m_pad_bcr_id2_1;
	CButtonST m_pad_bcr_id_1;
	CXPGroupBox	m_group_buffer_status;
	CMacButton	m_btn_door_open;
	CDigit	m_dgt_alarm;
	CDigit	m_dgt_mtbi;
	CGradientStatic	m_msg_wrun;
	CGradientStatic	m_msg_stop;
	CGradientStatic	m_msg_run;
	CGradientStatic	m_msg_brun;
	CGradientStatic	m_msg_change;
	CGradientStatic	m_msg_alarm;
	CGradientStatic	m_msg_mtbi;
	CXPGroupBox	m_group_daily_yield_info;
	CXPGroupBox	m_group_time_info;
	CXPGroupBox	m_group_lot_info;
	CXPGroupBox	m_group_lot_yield_info;
	CDigit	m_dgt_stop;
	CDigit	m_dgt_run;
	CMacButton	m_btn_lot_open;	//20120522
	CButtonST m_pad_bcr_id;
	CButtonST m_pad_bcr_id2;
	CButtonST m_pad_bcr_id3;
	CButtonST m_pad_bcr_id4;
	CFloatST  m_msg_bcr_id;
	CFloatST  m_msg_bcr_id2;
	CFloatST  m_msg_bcr_id3;
	CFloatST  m_msg_bcr_id4;
	CXPGroupBox	m_group_bcr_set;
	CXPGroupBox m_group_jobchange;
	CXPGroupBox m_group_jobchange2;
	CEditEx	 m_edit_bcr_id;
	CEditEx	 m_edit_bcr_id2;
	CEditEx	 m_edit_bcr_id3;
	CEditEx	 m_edit_bcr_id4;
	CEditEx	 m_edit_bcr_id_1;
	CEditEx	 m_edit_bcr_id2_1;
	CEditEx	 m_edit_bcr_id3_1;
	CEditEx	 m_edit_bcr_id4_1;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_main_font;			// 폰트 정보 설정 변수
	CFont* mp_main_big_font;		// 폰트 정보 설정 변수
	int Init_Multistep;

	int mn_time_select;
	int mn_retest_onoff[4];
	int mn_motor_site;
	int mn_stacker_Step;

	int mn_air_blow_check[2];

	CString mstr_rear_buffer[2];

	int mn_rear_buffer[2];

	int mnDeviceExistence;
	/////////////////////////// 20120924
	long l_bcr_send_wait[3];
	long l_hs_send_wait[3];
	int bcr_retry;
	int hs_retry;
	CMyBasicData     mcls_m_main;		// 기본 셋팅 정보 로딩 및 저장 클래스
	///////////////////////////

	GridControlAlg m_pGrid;

	CPicture		m_p_bmp_view;
	CPicture		m_p_bmp_view1;
	CPicture		m_p_bmp_view2;
	CPicture		m_p_bmp_view3;
	CPicture		m_p_bmp_view4;
	CPicture		m_p_bmp_view5;
	CPicture		m_p_bmp_view6;
	CPicture		m_p_bmp_view7;
	CPicture		m_p_bmp_view8;
	CPicture		m_p_bmp_view9;
	CPicture		m_p_bmp_view10;
	CPicture		m_p_bmp_view11;

	///////////////////////////////////// 20120707
	CCheckerCtrl  m_ctrl_loader_tray;
	CCheckerCtrl  m_ctrl_hs_left_tray;
	CCheckerCtrl  m_ctrl_hs_right_tray;
	CCheckerCtrl  m_ctrl_clip1_tray;
	CCheckerCtrl  m_ctrl_clip2_tray;
	CCheckerCtrl  m_ctrl_unloader_tray;
	CCheckerCtrl  m_ctrl_reject_tray;


	CCheckerCtrl  m_ctrl_loader_picker;
	CCheckerCtrl  m_ctrl_hs_left_picker;
	CCheckerCtrl  m_ctrl_hs_right_picker;
	CCheckerCtrl  m_ctrl_clip1_picker;
	CCheckerCtrl  m_ctrl_clip2_picker;	
	CCheckerCtrl  m_ctrl_unloader_picker;
	CCheckerCtrl  m_ctrl_reject_picker;
	CCheckerCtrl  m_ctrl_sorter_picker;

	
	/////////////////////////////////////	

	CString mstr_bcr_id[2];
	CString mstr_bcr_id2[2];
	CString mstr_bcr_id3[2];
	CString mstr_bcr_id4[2];
	CString mstr_bcr_id_1[2];
	CString mstr_bcr_id2_1[2];
	CString mstr_bcr_id3_1[2];
	CString mstr_bcr_id4_1[2];


	int StopFlag;
	int CancelFlag;
	int StartFlag;
	int InitialFlag;
	int SimuelFlag;

	int m_bExeExcute;
	int m_Tray;

	int m_remove_step;
	int m_recorvery_step;
	int m_stacker_step;
	int mn_retry;
	
	char mc_alarmcode[10];
	
	long l_Until_WaitTime[3];
	
	int ClipBufferStep;
	int mn_Clipretry;

	int m_remove_flag;
	int m_recorvery_flag;

	int n_clipremove_step;
	int n_cliprecovery_step;
	
	char cJamcode[10];

/////////////////////////////////////////////////////////////////////////////////

// Operations
public:
	void OnMain_Display_CycleTime_Info();
	void OnMain_Display_Daily_Yield_info();
	void OnMain_Display_Lot_Yield_info();
	void OnMain_Display_Lot_Info();
	void Device_Lot_Display(int part, int pos);
	void Device_Info_Display(int part, int pos);
	void OnMain_Init_Lot_Info();
	void OnMain_Init_Daily_Yield_info();
	void OnMain_Init_Lot_Yield_info();
	void OnMain_TimeInfo_Display(); 
	void OnMain_Time_Display(int n_state);
	void OnMain_Digital_Count_Set();
	void OnMain_Label_Set();
	void GridMerge(UINT nID, int srow, int scol, int nrow, int ncol);
	void GridFont(UINT nID, int row, int col, int size);
	void GridData(UINT nID, int row, int col, CString data);
	void GridControl(UINT nID, int type, int row, int col, int pos);
	void GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk);
	void OnMain_GroupBox_Set();

	void OnMain_Data_Set();

	void OnMain_BCR_DISPLAY(int n_state);	//20120524


	void UpdateScreens();
	void UpdateMotorMove();
	void Init_Animation();
	
	////////////////////////////////////////// 20120704
	void OnMain_CheckerCtrl_Set();
	void OnMainWork_Tray_Display(int iSite);
	void OnMainWork_Picker_Display(int iSite);
	//////////////////////////////////////////////
	void Init_Grid_Lot();//20120711
	void OnMain_Init_Grid();
	void OnMain_Display_VisionBuffer();
	void OnMainWork_RejectTray_Change();
	void OnMainLotReStart_Display();
	void OnMainMulLotDisplay();

	void EnableButton(int iEnable);
	void MoveStepClear();
	
	int ClipRemove();
	int ClipRecovery();
	int LdUldClipBufferPos(int nPos);

	int OnRemoveClip();
	int OnRecoveryClip();
	////////
	int OnStackerDown(int nMotorNum);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Main)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Main();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Main)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnDoorOpen();
	afx_msg void OnBtnLotOpen();	//20120522
	afx_msg void OnPadBcrId();
	afx_msg void OnPadBcrId2();
	afx_msg void OnPadBcrId3();
	afx_msg void OnPadBcrId4();
	afx_msg void OnSetfocusEditBcrId();
	afx_msg void OnSetfocusEditBcrId2();
	afx_msg void OnSetfocusEditBcrId3();
	afx_msg void OnSetfocusEditBcrId4();
	afx_msg void OnSetfocusEditBcrId_1();
	afx_msg void OnSetfocusEditBcrId2_1();
	afx_msg void OnSetfocusEditBcrId3_1();
	afx_msg void OnSetfocusEditBcrId4_1();
	afx_msg void OnBtnWorkInformation();
	afx_msg void OnPadBcrId1();
	afx_msg void OnPadBcrId21();
	afx_msg void OnPadBcrId31();
	afx_msg void OnPadBcrId41();
	afx_msg void OnBtnLotCancel();
	afx_msg void OnBtnVisionCntReset();
	afx_msg void OnBtnMultilotInit();
	afx_msg void OnBtnWorkInformation2();
	afx_msg void OnBtnWorkInformation3();
	afx_msg void OnBtnWorkbuffer1Mdpos();
	afx_msg void OnBtnWorkbuffer1Lhpos();
	afx_msg void OnBtnWorkbuffer1Rhspos();
	afx_msg void OnWorkbuffer1Clip();
	afx_msg void OnBtnWorkbuffer1Uldpos();
	afx_msg void OnBtnWorkbuffer2Mdpos();
	afx_msg void OnBtnWorkbuffer2Lhpos();
	afx_msg void OnBtnWorkbuffer2Rhspos();
	afx_msg void OnWorkbuffer2Clip();
	afx_msg void OnBtnWorkbuffer2Uldpos();
	afx_msg void OnBtnWorkbuffer3Mdpos();
	afx_msg void OnBtnWorkbuffer3Lhpos();
	afx_msg void OnBtnWorkbuffer3Rhspos();
	afx_msg void OnWorkbuffer3Clip();
	afx_msg void OnBtnWorkbuffer3Uldpos();
	afx_msg void OnBtnWorkbuffer4Mdpos();
	afx_msg void OnBtnWorkbuffer4Lhpos();
	afx_msg void OnBtnWorkbuffer4Rhspos();
	afx_msg void OnWorkbuffer4Clip();
	afx_msg void OnBtnWorkbuffer4Uldpos();
	afx_msg void OnBtnMdrobotWorkpos1();
	afx_msg void OnBtnMdrobotWorkpos2();
	afx_msg void OnBtnMdrobotWorkpos3();
	afx_msg void OnBtnMdrobotWorkpos4();
	afx_msg void OnBtnMdrobotInit();
	afx_msg void OnBtnLhsrobotWorkpos1();
	afx_msg void OnBtnLhsrobotWorkpos2();
	afx_msg void OnBtnLhsrobotWorkpos3();
	afx_msg void OnBtnLhsrobotWorkpos4();
	afx_msg void OnBtnLhsrobotInit();
	afx_msg void OnBtnRhsrobotWorkpos1();
	afx_msg void OnBtnRhsrobotWorkpos2();
	afx_msg void OnBtnRhsrobotWorkpos3();
	afx_msg void OnBtnRhsrobotWorkpos4();
	afx_msg void OnBtnRhsrobotInit();
	afx_msg void OnBtnClipinsertWorkpos1();
	afx_msg void OnBtnClipinsertWorkpos2();
	afx_msg void OnBtnClipinsertWorkpos3();
	afx_msg void OnBtnClipinsertWorkpos4();
	afx_msg void OnBtnClipinsertInit();
	afx_msg void OnBtnClipclampWorkpos1();
	afx_msg void OnBtnClipclampWorkpos2();
	afx_msg void OnBtnClipclampWorkpos3();
	afx_msg void OnBtnClipclampWorkpos4();
	afx_msg void OnBtnSorterrbtWorkpos1();
	afx_msg void OnBtnSorterrbtWorkpos2();
	afx_msg void OnBtnSorterrbtWorkpos3();
	afx_msg void OnBtnSorterrbtWorkpos4();
	afx_msg void OnBtnSorterrbtInit();
	afx_msg void OnPaint();
	afx_msg void OnBtnUldrobotVisionPick();
	afx_msg void OnBtnUldrobotDown();
	afx_msg void OnBtnSimuelInit();
	afx_msg void OnBtnWorkbuffer1Init();
	afx_msg void OnBtnWorkbuffer2Init();
	afx_msg void OnBtnWorkbuffer3Init();
	afx_msg void OnBtnWorkbuffer4Init();
	afx_msg void OnBtnMdrobotDown();
	afx_msg void OnBtnLhsrobotDown();
	afx_msg void OnBtnRhsrobotDown();
	afx_msg void OnBtnClipinsertDown();
	afx_msg void OnBtnClipclampDown();
	afx_msg void OnBtnSorterDown();
	afx_msg void OnBtnUldrbtDown();
	afx_msg void OnBtnUldrbtInit();
	afx_msg void OnBtnClipclampUp();
	afx_msg void OnBtnMdrobotUp();
	afx_msg void OnBtnLhsrobotUp();
	afx_msg void OnBtnRhsrobotUp();
	afx_msg void OnBtnClipinsertUp();
	afx_msg void OnBtnSorterUp();
	afx_msg void OnBtnUldrbtUp();
	afx_msg void OnBtnClipCapRemove();
	afx_msg void OnBtnClipCapReset();
	afx_msg void OnBtnMDTrayLockOnOff();
	afx_msg void OnBtnLHTrayLockOnOff();
	afx_msg void OnBtnRHTrayLockOnOff();
	afx_msg void OnBtnMDStackerDown();
	afx_msg void OnBtnLHStackerDown();
	afx_msg void OnBtnRHStackerDown();
	afx_msg void OnBtnSerialNo();
	afx_msg void OnSetfocusEditSerialNo();
	afx_msg void OnSetfocusEditLotNo();
	afx_msg void OnBtnUmdStacker2TrayLockOnoff();
	afx_msg void OnBtnBarcodePrintReset();
	afx_msg void OnBtnMdlTray2();
	afx_msg void OnBtnFrontTray2();
	afx_msg void OnBtnFrontBack2();
	afx_msg void OnBtnFrontUnload2();
	//}}AFX_MSG
//	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMain_Work_Info_Display(WPARAM wParam,LPARAM lParam);  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 
//	afx_msg LRESULT OnReceivedNewLot( WPARAM wParam, LPARAM lParam );
//	afx_msg LRESULT OnDrawLotTime( WPARAM wParam, LPARAM lParam );
//	afx_msg LRESULT OnDrawWorkSite( WPARAM wParam, LPARAM lParam );
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);	//20120711
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_MAIN_H__7A483F1B_61FF_4EB9_B41D_251A637D6074__INCLUDED_)
