#if !defined(AFX_DIALOG_WORK_INTERFACE_H__D146D253_E566_4753_9DD1_621B74AC0ED2__INCLUDED_)
#define AFX_DIALOG_WORK_INTERFACE_H__D146D253_E566_4753_9DD1_621B74AC0ED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Work_Interface.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Interface dialog
#include "Variable.h"			// 전역 변수 정의 클래스 추가
#include "Public_Function.h"
#include "EXDigitST.h"			// 디지털 카운터 클래스
#include "XPGroupBox.h"			// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"			// XP 칼라 버튼 클래스
#include "FloatST.h"
#include "GridControlAlg.h"
#include "Digit.h"
#include "Dialog_Keyboard.h"
#include "MyBasicData.h"
#include "Screen_IO.h"	//20120620

class CDialog_Work_Interface : public CDialog
{
public:
	CFont			* m_p_font;
	CMyBasicData    mcls_work_interface;		// 기본 셋팅 정보 로딩 및 저장 클래스

	BYTE			m_by_ip[4];		// 0: FTP, 1: TAMS

	int				m_n_client_cnt;
	int				m_n_server_cnt;
	int				m_n_client_port[10][2]; 
	int				m_n_server_port[10][2]; // 0: bcr 1: sba : 2: sec
	int				m_n_port[10][2];
	int				m_n_data[10][2];
	int				m_n_parity[10][2];
	int				m_n_stop[10][2];
	int				m_n_rate[10][2];
	int				m_vision_step;
	int				m_bcr_step;
	int				m_bcr_triger_step;	//20120620
	int				m_bcr_triger_step2;	//20120620

	int				nColNum;
	double          endPos;
	double			startPos;
	BOOL			m_b_stop_req;

	//bcr
	long			l_bcr_send_wait[3];
	int				iBarcodeScanRetryCnt;
	int				iVisionScanRetryCnt;	//20120514 유성준

	//////////////////// 20120620
	int iBCRCJRetryCnt;
	int iBCRTKINRetryCnt;
	int iBCRTKOUTRetryCnt;
	int iBCRYIELDRetryCnt;
	
	int iHSCJRetryCnt;
	int iHSTKINRetryCnt;
	int iHSTKOUTRetryCnt;
	int iHSYIELDRetryCnt;
	////////////////////


	CString			str_command;
	CString			strBcrReadData;
	CString			strBcrReadDataEach;
	int				iPickerInfo[PICKER_NUM];

	CString			m_str_client_ip[10][2];

	TSpread			*m_grid_client;
	TSpread			*m_grid_server;

	CEditEx			m_edit_client_port_1;
	CEditEx			m_edit_server_port_1;
	CEditEx			m_edit_client_port_3;	//20120514 유성준
	CEditEx			m_edit_client_port_2;

	CEditEx			m_edit_sec_server_port;
	CEditEx			m_edit_etc_server_port;

	GridControlAlg	m_p_grid;
	int				m_Nmodel;

	//////////////////////////////////// 20120620
	int m_bcr_hs_step;	
	int mn_vision_hs_enable;
	CString str_bcr_ml_name[2];		// 선택된 디바이스 종류 저장 변수
	int mn_bcr_ml_name;
	int mn_bcr_ml_mode[2];				// With, With Out, No Tray
	CString str_hs_ml_name[2];		// 선택된 디바이스 종류 저장 변수
	int mn_hs_ml_name;
	int mn_hs_ml_mode[2];				// With, With Out, No Tray
	////////////////////////////////////
	CDialog_Keyboard  keyboard_dlg;
// Construction
public:
//	int Manual_Bcr_Test();
	int Manual_Bcr_Triger_Test();	//20120620	
	int Manual_Vision_Test();
	void OnInterface_IP(CString str_ip);
	void Init_Editbox();
	void OnInterface_Display();
	int ConverterToData(int mode, int pos);
	int ConverterToPos(int mode, int data);
	void	Init_Label();
	void	Init_Button();
	void	Init_Group();
	void	Data_HistoryLog();
	void	Data_Recovery();
	int		Data_Comparison();
	void	Data_Apply();
	void	Data_Backup();
	void	Data_Init();
	void OnInterface_BCR_DISPLAY(int n_state);	//20120608
	
	///////////////////////////// 20120620
	void OnBtn_Enable_True();
	void OnBtn_Enable_False();
	void OnBtn_BCR_HS_YIELD_Enable_True();	
	void OnBtn_BCR_HS_YIELD_Enable_False();	
	int Manual_Bcr_HS_Yield_Test();
	int Manual_Bcr_Triger_Test2();	
	void Data_Set();	
	void BCR_Ml_Focus_Set();
	void HS_Ml_Focus_Set();

	int OnInterface_ML_BCR_Init_List(LPCTSTR pszPath);
	void OnInterface_ML_BCR_Focus_Set();
	BOOL ML_BCR_AddItem(int nIndex, WIN32_FIND_DATA *pfd);

	int OnInterface_ML_HS_Init_List(LPCTSTR pszPath);
	void OnInterface_ML_HS_Focus_Set();
	BOOL ML_HS_AddItem(int nIndex, WIN32_FIND_DATA *pfd);

	///////////////////////////// 

	CDialog_Work_Interface(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Work_Interface)
	enum { IDD = IDD_WORK_INTERFACE };
	CGradientStatic	m_msg_etc_server_port;
	CGradientStatic	m_msg_sec_server_port;
	CMacButton	m_btn_etc_server_port;
	CMacButton	m_btn_sec_server_port;
	CXPGroupBox	m_group_etc_server;
	CXPGroupBox	m_group_sec_server;
//	CMacButton	m_btn_bcr_test;
	CMacButton	m_btn_bcr_triger_test;	//20120620
	CMacButton	m_btn_bcr_triger_test2;	//20120620
	CMacButton	m_btn_bcr_triger_test3;	//20120620
	CMacButton	m_btn_bcr_triger_test4;	//20120620

	/////////////////////////////////////// 20120514 유성준
	CXPGroupBox	m_group_serial_4;
	CGradientStatic	m_msg_stop_4;
	CGradientStatic	m_msg_rate_4;
	CGradientStatic	m_msg_port_4;
	CGradientStatic	m_msg_parity_4;
	CGradientStatic	m_msg_data_4;
	CComboBox	m_cb_stop_4;
	CComboBox	m_cb_rate_4;
	CComboBox	m_cb_port_4;
	CComboBox	m_cb_parity_4;
	CComboBox	m_cb_data_4;
	CMacButton	m_btn_serial_connect_4;
	CMacButton	m_btn_serial_apply_4;
	/////////////////////////////////////// 20120519 유성준
	CXPGroupBox	m_group_serial_5;
	CGradientStatic	m_msg_stop_5;
	CGradientStatic	m_msg_rate_5;
	CGradientStatic	m_msg_port_5;
	CGradientStatic	m_msg_parity_5;
	CGradientStatic	m_msg_data_5;
	CComboBox	m_cb_stop_5;
	CComboBox	m_cb_rate_5;
	CComboBox	m_cb_port_5;
	CComboBox	m_cb_parity_5;
	CComboBox	m_cb_data_5;
	CMacButton	m_btn_serial_connect_5;
	CMacButton	m_btn_serial_apply_5;
	/////////////////////////////////////// 20120610 유성준
	CXPGroupBox	m_group_serial_6;
	CGradientStatic	m_msg_stop_6;
	CGradientStatic	m_msg_rate_6;
	CGradientStatic	m_msg_port_6;
	CGradientStatic	m_msg_parity_6;
	CGradientStatic	m_msg_data_6;
	CComboBox	m_cb_stop_6;
	CComboBox	m_cb_rate_6;
	CComboBox	m_cb_port_6;
	CComboBox	m_cb_parity_6;
	CComboBox	m_cb_data_6;
	CMacButton	m_btn_serial_connect_6;
	CMacButton	m_btn_serial_apply_6;



	///////////////////////////////////////	

	CXPGroupBox	m_group_serial_3;
	CGradientStatic	m_msg_stop_3;
	CGradientStatic	m_msg_rate_3;
	CGradientStatic	m_msg_port_3;
	CGradientStatic	m_msg_parity_3;
	CGradientStatic	m_msg_data_3;
	CComboBox	m_cb_stop_3;
	CComboBox	m_cb_rate_3;
	CComboBox	m_cb_port_3;
	CComboBox	m_cb_parity_3;
	CComboBox	m_cb_data_3;
	CMacButton	m_btn_serial_connect_3;
	CMacButton	m_btn_serial_apply_3;
	CMacButton	m_btn_serial_connect_2;
	CGradientStatic	m_msg_stop_2;
	CGradientStatic	m_msg_rate_2;
	CGradientStatic	m_msg_port_2;
	CGradientStatic	m_msg_parity_2;
	CGradientStatic	m_msg_data_2;
	CXPGroupBox	m_group_serial_2;
	CComboBox	m_cb_stop_2;
	CComboBox	m_cb_rate_2;
	CComboBox	m_cb_port_2;
	CComboBox	m_cb_parity_2;
	CComboBox	m_cb_data_2;

	CGradientStatic	m_msg_client_ip_2;

	CGradientStatic	m_msg_client_port_2;
	CMacButton	m_btn_serial_connect_1;
	CMacButton	m_btn_serial_apply_2;
	CMacButton	m_btn_serial_apply_1;

	CMacButton	m_btn_server_port_1;
	
	CMacButton	m_btn_client_port_2;
	CMacButton	m_btn_client_port_1;

	CMacButton	m_btn_client_ip_2;
	CMacButton	m_btn_client_ip_1;
	CXPGroupBox	m_group_serial_1;
	CGradientStatic	m_msg_stop_1;
	CGradientStatic	m_msg_server_port_1;

	CGradientStatic	m_msg_rate_1;
	CGradientStatic	m_msg_port_1;
	CGradientStatic	m_msg_parity_1;
	CGradientStatic	m_msg_data_1;
	CGradientStatic	m_msg_client_port_1;
	CGradientStatic	m_msg_client_ip_1;
	CXPGroupBox	m_group_server_1;

	CXPGroupBox	m_group_serial;
	CXPGroupBox	m_group_network;

	CXPGroupBox	m_group_client_2;
	CXPGroupBox	m_group_client_1;
	CComboBox	m_cb_stop_1;
	CComboBox	m_cb_rate_1;
	CComboBox	m_cb_port_1;
	CComboBox	m_cb_parity_1;
	CComboBox	m_cb_data_1;

	CIPAddressCtrl	m_client_ip_2;
	CIPAddressCtrl	m_client_ip_1;
	CButtonST	m_btn_apply;

	///////////////////////////// 20120608
	CMacButton m_btn_jobchange;
	CMacButton m_btn_jobchange2;
	CMacButton m_btn_jobchange3;
	CMacButton m_btn_jobchange4;
	CMacButton m_btn_jobchange5;
	CMacButton m_btn_jobchange6;
	CMacButton m_btn_jobchange7;
	CMacButton m_btn_jobchange8;

	CXPGroupBox m_group_jobchange;
	CXPGroupBox m_group_jobchange2;
	///////////////////////////// 20120620
	CMacButton m_btn_jobchange9;
	CMacButton m_btn_jobchange10;
	CListBoxST	m_list_bcr_ml;
	CListBoxST	m_list_hs_ml;
	CXPGroupBox	m_group_bcr_ml;
	CXPGroupBox	m_group_hs_ml;

	/////////////////////////////
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Work_Interface)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Work_Interface)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBtnApply();
	afx_msg void OnBtnSerialApply1();
	afx_msg void OnBtnClientIp1();
	afx_msg void OnBtnClientPort1();
	afx_msg void OnBtnClientIp2();
	afx_msg void OnBtnClientPort2();
	afx_msg void OnBtnServerPort1();
	afx_msg void OnBtnSerialConnect1();
	afx_msg void OnBtnSerialApply2();
	afx_msg void OnBtnSerialConnect2();
	afx_msg void OnBtnSerialApply3();
	afx_msg void OnBtnSerialConnect3();
	afx_msg void OnBtnSerialApply4();
	afx_msg void OnBtnSerialConnect4();
	afx_msg void OnBtnSerialApply5();
	afx_msg void OnBtnSerialConnect5();	
	afx_msg void OnButtonStop();
	afx_msg void OnBtnModelBcrTest();
	afx_msg void OnBtnSecServerPort();
	afx_msg void OnBtnEtcServerPort();
	afx_msg void OnButtonTest();
	afx_msg void OnBtnJobChange();
	afx_msg void OnBtnJobChange2();
	afx_msg void OnBtnJobChange3();
	afx_msg void OnBtnJobChange4();
	afx_msg void OnBtnJobChange5();
	afx_msg void OnBtnJobChange6();
	afx_msg void OnBtnJobChange7();
	afx_msg void OnBtnJobChange8();
	afx_msg void OnBtnSerialApply6();
	afx_msg void OnBtnSerialConnect6();
	afx_msg void OnBtnModelBcrTest2();
	afx_msg void OnBtnJobChange9();
	afx_msg void OnBtnJobChange10();
	afx_msg void OnSelchangeListBcrMl();
	afx_msg void OnSelchangeListHeatsinkMl();
	afx_msg void OnBtnModelBcrTest3();
	afx_msg void OnBtnModelBcrTest4();
	afx_msg void OnBtnBcrRead();
	//}}AFX_MSG
	afx_msg void OnCell_Left_Click(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWork_Interface_Info_Display(WPARAM wParam,LPARAM lParam);  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 //20120608

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORK_INTERFACE_H__D146D253_E566_4753_9DD1_621B74AC0ED2__INCLUDED_)
