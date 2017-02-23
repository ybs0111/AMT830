#if !defined(AFX_DIALOG_WORK_MAINTENANE_H__BB3C551B_4232_4DA5_9254_EB399E96FD07__INCLUDED_)
#define AFX_DIALOG_WORK_MAINTENANE_H__BB3C551B_4232_4DA5_9254_EB399E96FD07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Work_Maintenane.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가
#include "Public_Function.h"
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가  
#include "FloatST.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "XpButton.h"		// XP 칼라 버튼 클래스
#include "UniButtonEx.h"	// 유니 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 생성 클래스 추가 

#include "Digit.h"			// 디지털 카운터 클래스 21020516
#include "NewLabel.h"		//  21020516
#include "CheckerCtrl.h"	// SITE 클래스. 21020516

// ******************************************************************************
// 설정 정보 로딩 및 저장 클래스                                                 
// ******************************************************************************
#include "MyBasicData.h"
// ******************************************************************************

// ******************************************************************************
// 타워 램프 ON/OFF 칼라 정의                                                    
// ******************************************************************************
#define R_ON	(RGB(255, 0, 0))
#define R_OFF	(RGB(162, 0, 0))
#define Y_ON	(RGB(255, 255, 0))
#define Y_OFF	(RGB(162, 162, 0))
#define G_ON	(RGB(0, 255, 0))
#define G_OFF	(RGB(0, 162, 0))

#define FLICK	(RGB(190, 220, 255))
#include "Ctlbd_Variable.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Maintenane dialog

class CDialog_Work_Maintenane : public CDialog
{
// Construction
public:
	int ExCopyFile(CString strFrom, CString strTo);
	BOOL CopyFolder(CString& strFrom, CString& strTo);
	void OnMaintenance_Step_Save();
	void OnMaintenance_LogFile_Create();
	void OnMaintenance_Digital_Count_Set();
	void OnMaintenance_File_Backup(CString strTargetFile, CString strCreateFile, CString strBackupFolder, CString strBackupFile, BOOL b_over_write);
	void OnMaintenance_Lamp_Change();
	void OnMaintenance_Label_Set();
	void OnMaintenance_Data_Recovery();
	int OnMaintenance_Data_Comparison();
	void OnMaintenance_Data_Display();
	void OnMaintenance_ListBar_Info_Send();
	void OnMaintenance_Data_BackUp();
	void OnMaintenance_Data_Apply();
	void OnMaintenance_EditBox_Set();
	void OnMaintenance_Buzzer_Change();
	void OnMaintenance_Hs_Change();
	void OnMaintenance_Button_Set();
	void OnMaintenance_Data_Set();
	void OnMaintenance_Green_State_Change(int idx);
	void OnMaintenance_Yellow_State_Change(int idx);
	void OnMaintenance_Red_State_Change(int idx);
	void DrawButton(int i);
	void OnMaintenance_GroupBox_Set();
	CDialog_Work_Maintenane(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
public:
	//{{AFX_DATA(CDialog_Work_Maintenane)
	enum { IDD = IDD_WORK_MAINTENACE };
	CButtonST	m_chk_hs_rear;
	CButtonST	m_chk_hs_front;
	CXPGroupBox	m_group_hs;
	CMacButton	m_btn_password_change_level2;
	CMacButton	m_btn_password_change_level1;
	CMacButton	m_btn_data_backup;
	CMacButton	m_btn_step_save;
	CXPGroupBox	m_group_lamp_change_time;
	CButtonST	m_pad_equip_no;
	CButtonST	m_pad_equip_id;
	CFloatST	m_msg_equip_no;
	CFloatST	m_msg_equip_id;
	CStatic	m_lamp_stop;
	CStatic	m_lamp_selfcheck;
	CStatic	m_lamp_warning;
	CStatic	m_lamp_run;
	CStatic	m_lamp_lotend;
	CStatic	m_lamp_lock;
	CStatic	m_lamp_initial;
	CStatic	m_lamp_alarm;
	CXPGroupBox	m_group_towerlamp;
	CXPGroupBox	m_group_password_change;
	CXPGroupBox	m_group_lamp_warning;
	CXPGroupBox	m_group_lamp_stop;
	CXPGroupBox	m_group_lamp_selfcheck;
	CXPGroupBox	m_group_lamp_run;
	CXPGroupBox	m_group_lamp_lotend;
	CXPGroupBox	m_group_lamp_lock;
	CXPGroupBox	m_group_lamp_initial;
	CXPGroupBox	m_group_lamp_flicker_time;
	CXPGroupBox	m_group_lamp_alarm;
	CXPGroupBox	m_group_eqp_set;
	CXPGroupBox	m_group_data_backup;
	CXPGroupBox	m_group_buzzer;
	CButtonST	m_chk_buzzer;
	CXPButton	m_btn_maintenance_apply;
	//}}AFX_DATA
public:
	enum { IDC_LAMP_BASE	= 3000, 
		   IDC_LAMP_ROOF	= 3010, 
		   IDC_LAMP_POLE	= 3020, 
		   IDC_LAMP_LOWER	= 3030, 
		   IDC_LAMP_UPPER	= 3040, 
		   IDC_LAMP_SEP1	= 3050, 
		   IDC_LAMP_SEP2	= 3060, 
		   IDC_LAMP_SEP3	= 3070, 
		   ID_LAMP_RLAMP	= 3080, 
		   ID_LAMP_RFLICKER	= 3090, 
		   ID_LAMP_YLAMP	= 3100, 
		   ID_LAMP_YFLICKER	= 3110, 
		   ID_LAMP_GLAMP	= 3120, 
		   ID_LAMP_GFLICKER	= 3130, 
	};

	enum { STOP			= 0,
		   RUN			= 1,
		   ALARM		= 2,
		   LOTEND		= 3,
		   INIT			= 4,
		   WARNING		= 5,
		   LOCK			= 6,
		   SELFCHECK	= 7
	};

	CFont* mp_maintenance_font;			// 폰트 정보 설정 변수

	CMyBasicData     mcls_maintenance;	// 기본 셋팅 정보 로딩 및 저장 클래스

	// **************************************************************************
	// 칼라 에디트 박스 클래스 변수 선언                                         
	// **************************************************************************
	CEditEx	 m_edit_equip_no;
	CEditEx	 m_edit_equip_id;

	// **************************************************************************
	// 디지털 카운터 클래스 변수 선언                                            
	// **************************************************************************
	CEXDigitST  m_dgt_flicker_time;
	CEXDigitST  m_dgt_change_time;

	int	mn_buzzer_mode[2];
	int	mn_front_mode[2];
	int	mn_rear_mode[2];

	int mn_lamp_flicker_time[2];
	CString mstr_equip_no[2];
	CString mstr_equip_id[2];
	int mn_lamp_change_time[2];



private:
	BOOL mb_first;     // 타워 램프 처음 생성 유무 플래그
	CRect mr_area[8];  // 컨트롤 영역 크기 저장 변수

	int mn_r_lamp[2][8];  // RED 램프 상태 플래그		[0:OFF    1:ON    2:FLICKER]
	int mn_y_lamp[2][8];  // YELLOW 램프 상태 플래그	[0:OFF    1:ON    2:FLICKER]
	int mn_g_lamp[2][8];  // GREEN 램프 상태 플래그		[0:OFF    1:ON    2:FLICKER]
	int mn_pos_x[8][2], mn_pos_y[8][2];  // 컨트롤 위치 저장 변수
	int mn_y_diff[8], mn_y_space[8];     // 출력 영역 저장 변수

	CUniButtonEx mp_btn_base[8], mp_btn_roof[8], mp_btn_pole[8], mp_btn_lower[8], mp_btn_upper[8];
	CUniButtonEx mp_btn_step1[8], mp_btn_step2[8], mp_btn_step3[8], mp_btn_r_lamp[8], mp_btn_y_lamp[8], mp_btn_g_lamp[8];

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Work_Maintenane)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialog_Work_Maintenane)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnDgtFlickerTime();
	afx_msg void OnChkBuzzer();
	afx_msg void OnBtnDataBackup();
	afx_msg void OnPadEquipNo();
	afx_msg void OnPadEquipId();
	afx_msg void OnBtnPasswordChangeLevle1();
	afx_msg void OnBtnPasswordChangeLevle2();
	afx_msg void OnBtnMaintenanceApply();
	afx_msg void OnSetfocusEditEquipId();
	afx_msg void OnSetfocusEditEquipNo();
	afx_msg void OnDgtChangeTime();
	afx_msg void OnBtnStepSave();
	afx_msg void OnChkHsFront();
	afx_msg void OnChkHsRear();
	//}}AFX_MSG
	afx_msg void OnRLampClick(UINT);
	afx_msg void OnYLampClick(UINT);
	afx_msg void OnGLampClick(UINT);
	afx_msg LRESULT OnMaintenance_Info_Apply(WPARAM wParam, LPARAM lParam);  // Tower Lamp 화면 정보 갱신 요청 메시지
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORK_MAINTENANE_H__BB3C551B_4232_4DA5_9254_EB399E96FD07__INCLUDED_)
