#if !defined(AFX_RUN_RBT_VISIONLABEL_H__9726358E_0447_45D4_A2EA_7C1CB175C3C5__INCLUDED_)
#define AFX_RUN_RBT_VISIONLABEL_H__9726358E_0447_45D4_A2EA_7C1CB175C3C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Rbt_VisionLabel.h : header file
//
enum {
	MBUFF_EMPTY = 0,
	BUFFPOS_NONE,
	BCR_READY,
	BCR_POS,
	LOAD_MODULE_READY,
	LOAD_MODULE_POS,
	VISION_READY,
	VISION_CHK_POS,
	ULD_MODULE_READY,
	ULD_MODULE_POS,
};

enum {
	sING = 100,
	sDONE,
};

/////////////////////////////////////////////////////////////////////////////
// CRun_Rbt_VisionLabel command target

class CRun_Rbt_VisionLabel
{
public:
	CRun_Rbt_VisionLabel();           // protected constructor used by dynamic creation
	virtual ~CRun_Rbt_VisionLabel();


// Attributes
public:
	//james 2012.0629
	int mn_VisionStep[2];//[0]:vision 1, [1]:vision 2
	int mn_VisionRbt_WorkingPos_Site[2];


	int PrevStep;
	int Run_labelStep;
	int Run_Vision1;
	int Run_Vision2;
	int MoveStep;
	int InitStep;
	int ReinstateStep;

	int mn_retry;
	int m_nTrargetPos[2];
	int m_nZPos[2];
	int m_nSite;
	int m_nLabelNum;
	int m_nVisChkNum;
	int mn_vis_result[100];
	int m_nPick_Label;

	int mn_printnum;
	int m_nOutPrint;
	int m_inspect;
	BOOL m_bOutPrint[1000];
	int m_nPick;
	int m_nOrder;

	double m_dTargetLabel;
	double m_dMovePos_1[2];
	double m_dMovePos_2[2];

	BOOL b_PickerFlag;
	long l_PickerWaitTime[3];
	int mn_Safety_Flag;

	int m_buffPos[2];
	int m_buffStatus[2];
	char mc_alarmcode[10];
	int mn_attach_err;
	long md_offset;

	int mn_change[10][10];//20121126
	long backuptime[3];//20121126
	int mn_check[2];//20121126
	int mn_visionretry;//20121126
	double dVisionCurPos[10];//20121126

	bool mb_dvc_clamp_flag[2]; //[0]:v1, [1]:v2
	long ml_dvc_clamp_wait[2][3];

	bool mb_dvc_rotator_flag[2]; //[0]:v1, [1]:v2
	long ml_dvc_rotator_wait[2][3];

	int n_bcrlot_step;
	long ml_untilwait_time[3];
	
	long NetworkConnectTime[3];
	long NetworkingTime[3];
	CString str_bcr;

	int mn_singleprint;
	int mn_emptyprint;
	int mn_labelprint;

	CString strTmp;

// Operations
public:
	//2012.0629 
	int Find_VisionRobot_Current_Position(int n_site, int n_check_pos); //로보트의 현재 위치를 확인한다 

	int set_dvc_clamp(int n_site, int n_onoff);
	int get_dvc_clamp(int n_site, int n_onoff, int n_wait, int n_limit);

	int set_dvc_rotator(int n_site, int n_onoff);
	int get_dvc_rotator(int n_site, int n_onoff, int n_wait, int n_limit);
	int OnBcrRev_Excution();

	void Thread_Run();
	void Run_Label();
	void Run_Module_Vision_Top();
	void Run_Module_Vision_Btm();
	void Run_Init();
	void Run_Reinstate();
	void Thread_Variable_Initial();


	int OnChkVisionBuffer(int nPosBuff, int& zPos, int nTargetPos);
	int OnRetBufferTest(int nAxis, int nPos);

	void Robot_BackMovePosValCheck();//20121126
	void OnVisionRbt_FinalPos();//20121126

	CString VisionRobot_Current_Position( int n_check_pos);


};
extern CRun_Rbt_VisionLabel Run_Rbt_VisionLabel;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUN_RBT_VISIONLABEL_H__9726358E_0447_45D4_A2EA_7C1CB175C3C5__INCLUDED_)
