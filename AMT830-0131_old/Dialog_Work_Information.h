#if !defined(AFX_DIALOG_WORK_INFORMATION_H__6BFCA853_A77F_4ED3_8978_72FED2E5381E__INCLUDED_)
#define AFX_DIALOG_WORK_INFORMATION_H__6BFCA853_A77F_4ED3_8978_72FED2E5381E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Work_Information.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Information dialog

#define TMR_MOTOR_PICK_MODULE	7770
#define TMR_MOTOR_FRONT_MODULE	7771
#define TMR_MOTOR_REAR_MODULE	7772
#define TMR_MOTOR_WORK_BUFFER_1	7773
#define TMR_MOTOR_WORK_BUFFER_2	7774
#define TMR_MOTOR_WORK_BUFFER_3	7775
#define TMR_MOTOR_WORK_BUFFER_4	7776
#define TMR_MOTOR_CLIP_MODULE	7777
#define TMR_MOTOR_SORTER_MODULE	7779
#define TMR_MOTOR_UNLOAD_MODULE	7780
#define TMR_MOTOR_VISION_MODULE	7781
#define TMR_MOTOR_CLIP_CLAMP	7782

class CDialog_Work_Information : public CDialog
{
// Construction
public:
	CDialog_Work_Information(CWnd* pParent = NULL);   // standard constructor


public:
	GridControlAlg	m_p_grid;
	TSpread			*m_grid_info;

	int nModuleRobotPicker[D_INFOSIZE][PICKER_NUM][2];
	int nLefthsRobotPicker[D_INFOSIZE][PICKER_NUM][2];
	int nRighthsRobotPicker[D_INFOSIZE][PICKER_NUM][2];
	int n_motor_init[MOT_MAXMOTOR][2];

	int nClipRobotPicker[D_INFOSIZE][2];

	int nSortRobotPicker[D_INFOSIZE][2];
	int nUldRobotPicker[D_INFOSIZE][2];
	int VisionBuffer[D_INFOSIZE][2][2];

	int WorkBuffer[D_INFOSIZE][4][5][2];


	int nMotor_num;
	int nBuffer_num_1;
	int nBuffer_num_2;
	int nBuffer_num_3;
	int nBuffer_num_4;
	int m_nsite;
	int Init_module_step;
	int Init_front_step;
	int Init_rear_step;
	int Init_buffer_step_1;
	int Init_buffer_step_2;
	int Init_buffer_step_3;
	int Init_buffer_step_4;
	int InitSortStep;
	int InitUldStep;
	int ClipInitStep;
	int mn_Clipretry;
	int mn_retry;
	char mc_alarmcode[10];
	int ActionPicker[D_INFOSIZE][PICKER_NUM];
	int CActionPicker[D_INFOSIZE];

	BOOL b_PickerFlag[2];
	long l_PickerWaitTime[2][3];
	long l_UntilWaitTime[3];

	BOOL m_b_strop_req;
	BOOL mb_display_opration;
	

// Dialog Data
	//{{AFX_DATA(CDialog_Work_Information)
	enum { IDD = IDD_DIALOG_INFORMATION };
	CMacButton	m_clip_clamp_rbt;
	CMacButton	m_btn_ok;
	CMacButton	m_btn_apply;
	CMatrixStatic	m_msg_warnning;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Work_Information)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init_Grid_Info();
	void init_Label();
	void Init_Data();
	void Data_Display();
	void GridText(UINT nID, int row, int col, CString data);
	void OnWorkInform_Data_Apply();
	void OnWorkInform_Data_Comparison();
	void OnWorkInform_LogFile_Create();
	void OnWorkInform_Data_BackUp();

	int ModuleRbtInit();
	int FrontHsInit();
	int BackHsInit();
	int WorkBufferInit_1();
	int WorkBufferInit_2();
	int WorkBufferInit_3();
	int WorkBufferInit_4();
	int SorterRbtInit();
	int UldRbtInit();
	void Set_PickerUpDown(int nSite, int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	int Get_PickerUpDown(int nSite, int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	int ClipClampRbtInit();

public:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Work_Information)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStop();
	afx_msg void OnClipRbtInit();
	//}}AFX_MSG
	afx_msg void OnCellClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_WORK_INFORMATION_H__6BFCA853_A77F_4ED3_8978_72FED2E5381E__INCLUDED_)
