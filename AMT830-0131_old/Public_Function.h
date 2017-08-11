// Public_Function.h: interface for the CPublic_Function class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLIC_FUNCTION_H__692EA0CF_F10D_4B4E_A14A_1CD148AC075B__INCLUDED_)
#define AFX_PUBLIC_FUNCTION_H__692EA0CF_F10D_4B4E_A14A_1CD148AC075B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrm.h"

#include "Variable.h"		// 전역 변수 정의 클래스
#include <wininet.h>
#include <string>
#include <vector>
#include "ClientSocket.h"

class CMainFrame;
class CDialog_Part_IO;
class CDialog_Motor_Part;
class CDialog_Part_Manual;

class CPublic_Function  
	{
public:
	CPublic_Function();
	virtual ~CPublic_Function();

public:
	CDialog_Part_IO		*m_p_io;
	CDialog_Motor_Part	*m_p_motor_part;
	CDialog_Part_Manual	*m_p_manual;

	bool CYLFlag[MAX_IO];
	long CYLTime[MAX_IO][3];
	bool SiteFlag[MAX_IO];
	long SiteTime[MAX_IO][3];
	bool IOFlag[MAX_IO];
	long IOTime[MAX_IO][3];
	int Mot_HomeStep[MOT_MAXMOTOR];
	int Mot_RetryCount;
	int Mot_CurTime[MOT_MAXMOTOR][3];
	int Mot_RetryFlag[MOT_MAXMOTOR];

	int reinstatement_Step;
	long backuptime[3];
	int mn_check[2];
	int mn_stamp_change;
	
	CString strFile;
	int nGetPCBInfoStep;
	
	HINTERNET m_hOpen;
	HINTERNET m_hConnect;
	
	long NetworkConnectTime[3];
	long NetworkingTime[3];
	long TesterCommTime[3];
	char cJamcode[10];

	CString strFTPFileName[100];
	int nFTPFileCheck;
	int nCheckTemp;

	CString strQuery;			//쿼리문을 담을 변수 

	CString Itemsortnum;		//정렬조건을 담을 문자열 변수
	
	CString str_O_Pulse, str_N_Pulse;

	int mn_change[10][10];

	int m_nLastYear;
	int m_nLastMonth;
	int m_nLastDay;
	int m_nLastHour;

	int BarcodeScanStep;
	int BcrScanMoreSlow;
	int BcrScanMoreSlowAgain;
	int BcrStepReadIndex;
	int iBarcodeScanRetryCnt;

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

// 	int VisionScanStep;
// 	int VisionScanMoreSlow;
// 	int VisionScanMoreSlowAgain;
// 	int VisionStepReadIndex;

	int iVisionScanRetryCnt;	//20120514 유성준

	char mc_alarmcode[10];//20120705

	double bBcrStepGap;

	long l_until_wait_time[3];
	long l_trigger_wait_time[2][3];

	long ml_UntilWaitTime[3];

	int mn_retry;

	int nItemLength;
	int nMdl;
	CString m_strModel[100][500];
	CString m_strItemValue[500];


	CMainFrame						*pMainFrm;
	CMyBasicData     mcls_m_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	
public:

	////2014,0408
	void Excel_File_Save();
	void Excel_File_Read();
	BOOL GetStrings(CFile* pFile, char Delim, CString * str, int* pCnt);
	////

	void Set_PickerUpDown(int n_Site, int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);
	int Get_PickerUpDown(int n_Site, int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM]);

//	int Robot_Reinstatement_Position(int mode);
	int LdModuleRobot_Reinstatement_Position(int mode);
	int LeftHsRobot_Reinstatement_Position(int mode);
	int RightHsRobot_Reinstatement_Position(int mode);
	int LDClipRobot_Reinstatement_Position(int mode);
	int SorterRobot_Reinstatement_Position(int mode);
	int UnloadRobot_Reinstatement_Position(int mode);
	int VisionRobot_Reinstatement_Position(int mode);
	int ModuleBufferRobot_Reinstatement_Position(int mode);

	int OnHeatSinkModel_Change_Req();
	int OnHeatSinkModel_Change_Rev();
	int OnBCRModel_Change_Req();
	int OnBCRModel_Change_Rev();

	void PartNumber_Check();
	
	void Robot_BackData_Z_Set(int movepos);
	void OnDayDataReset();
	void OnDayDataCheck();
	void OnDailyDataSave();
	void OnLotDataSave();
	int Find_SitePosData(int nSite, int nRowcolmode, int nBinnum, int nDvcyesno, int nCountmode);
	int OnRetestModuleRemainCheck();
	CString OnRetestModuleCheck(int nPos, int nLast = 0);
	int Find_SiteDvc_count(int nSite, int nBinNum, int nDVCYesNo);
	int Find_WhereToGoRobotPos(int nSite, int nPickerInfo[D_INFOSIZE][PICKER_NUM], int nDVCYesNo, int nInfoSetFlag, int nBinNum, int nPickPlace, int nDataResult[INFO_DATA], int nMode);
	void DateTimeChange(CString sDateTime);
	char GetNtPrivilege();
	void SetActionParam();
	void MakeLogFolder(CString BaseDirectory,const CString strMsg);
	void DB_Write_Buffer(st_buffer_database st_bufferdb);
	void DB_Write_Ground(st_ground_database st_grounddb);
	void DB_Write_Alarm(st_alarm_database st_alarmdb);
	void DB_Write_Time(st_time_database st_timedb);
//	void OnLotCountInformationSave( const CString& strLotName );
	void TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp);
	void ThreadFunctionStepTrace(int nPos, int nStep);
	void Robot_BackData_Set(int nSite, int site);
	void Robot_BackData_Set(int site, int type, int xpos, int ypos, int noption = -1);
	void Robot_BackMovePosValCheck();
	CString GetLocalMacAddress();
	int FileSizeCheck(CString FileName, long size, int n_check);
	CString OnDeviceInfoAnalysis(CString strBody, int nInfo);
	CString OnNetworkDataMake(int nID);
	CString OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt = 0);
	void OnSet_IO_Port_Sound(int n_status, int n_opt = 0);
	void OnSet_IO_Port_Alarm();
	int Handler_Recovery_Data(int n_debug, int n_site, int n_mode);
	void OnSet_IO_Port_Init();
	void OnSet_IO_Port_Run();
	void OnSet_IO_Port_Stop();
	void OnSet_IO_Out_Port_Clear();
	int IO_Board_Initialize();
	int On_Encoder_Check(double d_pos);
	int OnMotor_Go_Check(int n_motor, double d_pos);
	int DoorOpenCheckSpot();
	void OnMot_Speed_Setting();
	int OnElect_CheckData(CString str_data);
	CString Get_MotorName(int n_motor_no);
	void IO_Map_Initialize();
//	int OnNetworkDataAnalysis(char* strBody);
	int OnNetworkDataAnalysis(CString strBody);

	BOOL CreateFolder(LPCTSTR szPath);
	void On_LogFile_Add(int n_msg_select, CString str_msg);
	void OnLogGalmg_PushLot( CString strlot, CString strtmp );
	void On_Log_Title_Msg( int iLogType, CString strTitle, CString strMsg );
	void OnLogWorkStep( CString strClass, CString strFunc, int iOld, int iCur );
	void MsgLog( LPCTSTR strMsg );
	CString GetStrCurrentTime();
	void OnLogRecord_Data( int iPos, int iWhere, bool bSet, CString strWhere );
	static void DeleteOldFiles( LPCTSTR lpDirPath, int iDays );
	void SetRBufferStation( int iPos, int iVal, bool bWrite = true );

	bool CheckWorkingLot( CString strLotNo );
	void _insertFistLot();

	void WriteRegistry_LotNo();
	void ReadRegistry_LotNo();

	int OnBCR_Command(int nCommand, CString *strCreatingCommand);
	int OnBCR_CheckData(CString strCheckData, int port);	//20120620
	int OnBCR_Parsing(CString strSourceData, int *nEnableIndex, CString *strResultData);
	int Robot_BarcodeScanning(int nPort);

	double XYRobot_MovePosValCheck(int Site1, int Site2, int PosNum1, int PosNum2=0);
	int PickerDeviceStatusCheck(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM], int Result[PICKER_NUM]);
	int Find_TrayInfo_Count(int n_site, int n_dvcyesno, int n_tray_y_pos);
	int PickerModuleStatusCheck(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM], int Result[PICKER_NUM]);
	int PickerSorterStatusCheck(int OnOff, int PickerInfo[D_INFOSIZE], int Result[LDULD_PARA]);
	int PickerUnloadStatusCheck(int OnOff, int PickerInfo[D_INFOSIZE], int Result[LDULD_PARA]);

	int Find_Picker_Count(int nSite, int n_enable, int n_dvcyesno, int n_bin);
	void OnReject_Tray_Reset();
	void OnSet_Vision_send(int nVisionMethod);

	
	CString str_command;
	CString strBcrReadDataEach;
	CString strBcrReadData;
	int iBcrInfo[1];

	/////////////////////// 20120521
	int Barcode_Print();
	int OutLptPort(CString str_text);
	/////////////////////// 20120610
	int OnVision_CheckData(CString strCheckData);
	/////////////////////// 20120620
	int OnLabelPrint_CheckData(CString strCheckData);

	///////////////////////
	int Vision_Data(int nMode, CString* strCommand);	//20120608
	int OnPaperOut_CheckData(CString strCheckData);	//20120707

	/////////////////////// 20120826
	int OnLabelPrint_Parsing(CString strCheckData, int Qty);

	
	////////////////////////////////////// 20120625
	int Multi_Paper_Print(int str_Data);
	int Single_Paper_Print();	
	int Pause_Btn_On();
	int Pause_Btn_Off();
	int Cancel_Btn();
	int Print_Calibrate_Btn();//20120920
	int Tear_Off_Mode();
	int Peel_Off_Mode();
	int Rewind_Mode();
	int Label_Printer_Status();
	int Barcode_Create(int Print, int Status);

	int Print_Complete_Set();
	int Print_Complete_Reset();
	
	int BCR_Trigger();
	int HS_Trigger();

	////////////////////////////////////// 
	int RobotPickerUpSensorCheck(int n_mode, int n_Site);
	int OnRetNextBufferPos(int nPos);
	int ChkTrayForLotEnd(int n_Site);
	int ChkForLotEnd(int n_Site);
	void OnSet_Door_Open();
	void OnSet_Door_Close();
	void Database_Worker(st_worker_db db);
	void OnLoadModule_Tray_Clear();

	int CheckVisionPos(int n_axis, double d_targetpos);
	double fMainMax(int n_axis, int opt);//opt : 0 Min opt 1: max
	int GetMotCurrentSite(int n_axis, double d_TargetPos=0);

	int InitNode();
	int MDInsertNode(int n_site);
	int LHSInsertNode(int n_site);
	int RHSInsertNode(int n_site);
	int CLIPInsertNode(int n_site);
	int ULDInsertNode(int n_site);
	int MDDelete_Node();
	int LHSDelete_Node();
	int RHSDelete_Node();
	int CLIPDelete_Node();
	int ULDDelete_Node();
	int MDFindNode();
	int LHSFindNode();
	int RHSFindNode();
	int CLIPFindNode();
	int ULDFindNode();

	//// 20131210
	void Set_Module_AllPickerUpDn();
	void Set_LeftHS_AllPickerUpDn();
	void Set_RightHS_AllPickerUpDn();
	int Get_Module_AllPickerUpDn();
	int Get_LeftHS_AllPickerUpDn();
	int Get_RightHS_AllPickerUpDn();
	//
	
	int ActionPicker[D_INFOSIZE][PICKER_NUM];
	BOOL b_PickerFlag[PICKER_NUM];
	BOOL b_FingerFlag[PICKER_NUM];
	long l_PickerWaitTime[PICKER_NUM][3];
	long l_FingerWaitTime[PICKER_NUM][3];
	
	int Handler_Recovery_Varible_Data(int n_mode, int n_site, int n_debug );//2014,1210
	//2015.0129
	void OnWriteProduct();


	/////////////////////////////////////////////////////////////////////////////////
	//2017.0428
	int LoadRegModelData();
	int GetModelNumFromModel( CString strModel);
	int GetModelFromPartID( CString PartID, CString& strModel);
	int ModelFileload(CString strMdl);
	/////////////////////////////////////////////////////////////////////////////////

private:
	int AreaSensor_Time[MOT_MAXMOTOR][3];
	int AreaSensor_Flag[MOT_MAXMOTOR];

	double d_motiondone_Time[MOT_MAXMOTOR][3];
	int n_motiondone_Flag[MOT_MAXMOTOR];

	int n_barcodepos_check;

	long ml_door_check[3];
	int mn_door_check_flag;

	CString str_step_temp[100];
	CTime  StepChangeTime[100];

	int m_auto_bcr_trigger_step;
	int m_auto_hs_trigger_step;
	int m_print_error_cnt;

};
#endif // !defined(AFX_PUBLIC_FUNCTION_H__692EA0CF_F10D_4B4E_A14A_1CD148AC075B__INCLUDED_)
