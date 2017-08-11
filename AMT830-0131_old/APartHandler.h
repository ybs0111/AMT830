// APartHandler.h: interface for the APartHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_)
#define AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "APartTray.h"

enum ENUM_MACHINE_POSITION
{
	EMPOS_FRONT,
	EMPOS_REAR,
	EMPOS_ALONE,
};

enum ESTEP_CONV
{
	EC_NONE,

	EC_RECEIVE,
	EC_RECEIVE_CHECK,
	
	EC_UP,
	EC_UP_CHK,

	EC_DOWN,
	EC_DOWN_CHK,

	EC_SEND,
	EC_SEND_CHK,
};

enum ESTEP_TTRANS
{
	ETT_TO_NONE,

	ETT_TO_REJECT,
	ETT_TO_REJECT_CHK,

	ETT_TO_BIN3,
	ETT_TO_BIN3_CHK,

	ETT_TRANSFER_TO_REJECT,
	ETT_TRANSFER_TO_REJECT_CHK,

	ETT_TRANSFER_TO_BIN3,
	ETT_TRANSFER_TO_BIN3_CHK,
};

enum ESTEP_ROBOT
{
	ER_NONE,

	ER_PICK_FROM_BIN,
	ER_PICK_FROM_BIN_CHK,

	ER_PICK_FROM_TSITE,
	ER_PICK_FROM_TSITE_CHK,

	ER_PLACE_TO_BIN,
	ER_PLACE_TO_BIN_CHK,

	ER_PLACE_TO_TSITE,
	ER_PLACE_TO_TSITE_CHK,
};

enum ESTEP_VIRTUAL_SUPPLY
{
	EVS_NONE,
	EVS_CALC,
	EVS_ADDTRAY,
	EVS_FINISH,
};

class APartHandler  
{
public:
	APartHandler();
	virtual ~APartHandler();
//
//	void Thread_Run();
//	void Run_Move_Main();
//	void Run_Move_SMEMA();
//	void Run_Move_Conv(int nIdx);
//	void Run_Move_Tray_Transfer();
//	void Run_Move_Robot();
//	void Run_Move_Virtual_Supply();
//	//////////////////////////////////////////////////////////////////////////
//	//int mn_run_status;  // 장비 동작 상태 정보 저장 변수 (O:STOP   1:RUN    2:ALARM    3:MAINT    4:INIT)
//
//	void SetMachinePos( ENUM_MACHINE_POSITION empos ) { m_empos = empos; }
//	ENUM_MACHINE_POSITION GetMachinePos() { return m_empos; }
//
//	void CalcPriorityBinByIn( ETRAY_POS epos );
//	void CalcPriorityBinByOut( ETRAY_POS epos );
//	void CalcPriorityBin( ETRAY_POS eFrom, ETRAY_POS eTo );
//	ETRAY_POS GetPriorityBin() { return m_epriority_bin; }
//	void SetPriorityBin( ETRAY_POS epos ) { m_epriority_bin = epos; }
//
//	void SetSecondaryBin( ETRAY_POS epos ) { m_esecondary_bin = epos; }
//	ETRAY_POS GetSecondaryBin() { return m_esecondary_bin; }
//
//
//	void ControlConv( int iIdx, ESTEP_CONV ec );
//	double CalcTargetPosition( int nAxis, BOOL bPick, int nTarget, int nPickerIdx );
//	int CalctargetIdx( int nAxis, int nTarget );
//	ESTEP_CONV GetControlConv( int iIdx ) { return m_step_conv[iIdx]; }
//
//	int GetVideo() { return m_nVedio; }
//	
//	void CheckLotEnd();

	void AddAlarmCnt();
	int GetAlarmCnt() { return m_nAlarmCnt; }
	void ClearAlarmCnt();

public:
//	void CalcTrayMoveNext_fromBin();
//	void CalcTrayMoveNext_fromBin3();
//	void CalcTrayMoveNext_fromReject();
//	void CalcTrayMoveNext_fromRejFull();
//
//	bool IsSkipable_Z_Safety();
//
//	bool IsTrayOthers();

public:
//	bool Chk_VarRail();
//
//	void SetRejAlarm( bool bVal ) { m_bRejAlarm = bVal; }
//	bool GetRejAlarm() { return m_bRejAlarm; }
//	void ClearRejAlarm() { if( m_bRejAlarm ) m_bRejAlarm = false; }
//
//	//ybs
//	void SetReadySendNext( bool bVal, bool bEmpty=0, CString strLotid="" ) { m_bReady_SendNext = bVal; m_bEmptyTray_SendNext = bEmpty; m_strLotID_SendNext = strLotid;}
//	bool GetReadySendNext() { return m_bReady_SendNext; }
//
//	void SetRecvStateNextMachine( bool bVal ) { m_bRecv_NextMachine = bVal; }
//	bool GetRecvStateNextMachine() { return m_bRecv_NextMachine; }
//
//	ESTEP_TTRANS GetStepTTrans() { return m_step_ttrans; }

protected:
//	void Control_byTray();
//	void ControlRobot();
//	bool CR_forEject_4( std::vector<int> vecTestEndInsert, std::vector<int> vecEmptyCell );
//	bool CR_forEject_2( std::vector<int> vecTestEndInsert, std::vector<int> vecEmptyCell );
//	bool CR_forExchange( std::vector<int> vecTestEndInsert, std::vector<int> vecMustTestMdl, std::vector<int> vecEmptyInsert, int nLotIdx );
//	bool CR_forTest( int nPickerCnt, std::vector<int> vecEmptyInsert, std::vector<int> vecMustTestMdl );
//	bool CR_forTest_Single( std::vector<int> vecEmptyInsert_Single, std::vector<int> vecMustTestMdl );
//	bool CR_forReject( std::vector<int> vecFail, std::vector<int> vecEmptyRej );
//	bool CR_forMoveRejectDvc( std::vector<int> vecFail, std::vector<int> vecEmptyRej, std::vector<int> vecEmptyCell );//ybs
//	bool CR_forSort( std::vector<int> vecPriEmptyCell, std::vector<int> vecSubGoodCell );
//	bool CR_forStacker3MoveDvc(std::vector<int> vecEmptyRej,std::vector<int> vecEmptyInsert_Single, std::vector<int> vecEmptyInsert, std::vector<int> vecFail);//2012,1225
//
//	int GetDivide( int nAxis, int nTarget );
//
//	double GetPosRel_Cok( int nAxis );
//	double GetPosRel_zPlace_Site();
//	double GetPosRel_zPlace_Tray();
//	double GetPosRel_zPlace_cokSite();
//	double GetPosRel_zPlace_cokStacker();
//


	

protected:
	ENUM_MACHINE_POSITION m_empos;	
//
//	ESTEP_CONV				m_step_conv[3];
//	ESTEP_TTRANS			m_step_ttrans;
//
//	ETRAY_POS				m_epriority_bin;
//	ETRAY_POS				m_esecondary_bin;
//
//	ESTEP_VIRTUAL_SUPPLY	m_step_virtual_supply;
//	DWORD					m_dwTime_virtual_supply;
//
//	int						m_nVedio;
//
//	bool					m_bRejAlarm;
//
//	bool					m_bReady_SendNext;
//	//ybs
//	bool					m_bEmptyTray_SendNext;
//	CString					m_strLotID_SendNext;
//
//	bool					m_bRecv_NextMachine;

	int						m_nAlarmCnt;
};

extern APartHandler	g_handler;

#endif // !defined(AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_)
