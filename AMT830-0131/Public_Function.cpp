// Public_Function.cpp: implementation of the CPublic_Function class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Handler.h"
#include "Variable.h"			// 전역 변수 정의 클래스
#include "Public_Function.h"	// 모터 보드 및 I/O 보드 관련 클래스
#include <nb30.h>
#include <wininet.h>
#include "Alg_Mysql.h"
#include "FastechPublic_IO.h"
#include "FAS_HSSI.h"
#include "ComizoaPublic.h"
#include "Cmmsdkdef.h"
#include "AMTLotManager.h"
#include "CtlBd_Function.h"
#include "CtlBd_Library.h"
#include "APartHandler.h"
// #include "stdafx.h"
// #include "iostream.h"
// #include "afxmt.h"



#include "ACriticalSection.h"
#include "ABpcManager.h"
#include "AMTRegistry.h"

#include "AMTRegistry.h"
// ******************************************************************************
// 대화 상자 클래스 추가                                                         
// ******************************************************************************
#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"

#include "Dialog_KeyPad.h"

#include "Dialog_Work_Barcode_Print.h"	//20120628


////2014,1210
#include "Run_Rbt_Load_Clip.h"
#include "Run_Rbt_Load_LeftHeat.h"
#include "Run_Rbt_Load_Module.h"
#include "Run_Rbt_Load_RightHeat.h"
#include "Run_Rbt_Sorter.h"
#include "Run_Rbt_VisionLabel.h"
#include "Run_Rbt_Work_Buffer.h"
#include "Run_Stacker_Load_Clip.h"
#include "Run_Stacker_Load_LeftHeat.h"
#include "Run_Stacker_Load_Module.h"
#include "Run_Stacker_Load_RightHeat.h"
#include "Run_Stacker_Sorter.h"
////


// ******************************************************************************
#define	deFildSu	1000//2014,0408						//자료을 읽어 올.. 필드의 수.

typedef struct _ASTAT_ 
{ 

ADAPTER_STATUS adapt; 
NAME_BUFFER NameBuff [30]; 

}ASTAT, * PASTAT; 

ASTAT Adapter; 

#include "math.h"
#include "io.h"			// 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요

//CMyBasicData     mcls_p_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
CMutex pmutex;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPublic_Function  Func;				// 전역 클래스 접근자 선언

CPublic_Function::CPublic_Function()
{
	Mot_RetryCount = 3;	
	BarcodeScanStep = 0;
	mn_retry = 0;
}

CPublic_Function::~CPublic_Function()
{
	
}

void CPublic_Function::On_LogFile_Add(int n_msg_select, CString str_msg)
{
	CSingleLock sing(&pmutex);

	sing.Lock();

	CString mstr_cur_year, mstr_cur_month, mstr_cur_day, str_display_time; // 현재 년, 월, 일 정보 문자형으로 변환하여 저장할 변수 
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour, mn_cur_min, mn_cur_sec; // 현재 년, 월, 일 정보 저장 변수 
	CString mstr_file_name;		// 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file;	// 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name, mstr_temp_data;  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content;		// 한 라인에 출력할 문자열 정보 저장 변수 
	COleDateTime time_cur;		// 검사할 시간 정보 저장 변수 
	CTime m_time_current;		// 간략한 헝식의 현재 시간 정보 저장 변수
	int mn_existence;			// 파일 존재 유무 설정 플래그 
	char fileName[256];			// 검색할 파일 정보 설정 함수 
	FILE  *fp ;					// 파일에 대한 포인터 설정 변수 

	CString BackupName;
	CString mstr_cur_hour, mstr_cur_min, mstr_cur_sec;
	CString mstr_folder, mstr_file;
	int Ret;

	CString sTime;
	
	SYSTEMTIME csTime;
	
	GetLocalTime(&csTime);
	
	csTime.wYear;
	csTime.wMonth;
	csTime.wDay;
	csTime.wHour;
	csTime.wMinute;
	csTime.wSecond;
	csTime.wMilliseconds;
	
	sTime.Format("[%04d/%02d/%02d %02d:%02d:%02d.%03d] ", csTime.wYear, csTime.wMonth, csTime.wDay, csTime.wHour, csTime.wMinute, csTime.wSecond, csTime.wMilliseconds);

	if (str_msg.IsEmpty() == TRUE)  
	{
		sing.Unlock();
		return;
	}

	// **************************************************************************
	// 파일 이름으로 사용할 날짜 정보를 얻는다                                   
	// **************************************************************************
	time_cur = COleDateTime::GetCurrentTime();  // 현재 시간 정보를 얻는다. 

	m_time_current = CTime::GetCurrentTime();  // 간략한 형식의 현재 시간 정보 얻는다. 

	mn_cur_year = time_cur.GetYear();  

	mn_cur_month = time_cur.GetMonth();  

    mn_cur_day = time_cur.GetDay();  

	mn_cur_hour = time_cur.GetHour();

	mn_cur_min = time_cur.GetMinute();
	mn_cur_sec = time_cur.GetSecond();
	// **************************************************************************

	// **************************************************************************
	// 날짜 정보를 문자형으로 변환하여 변수에 설정한다                           
	// **************************************************************************
	mstr_cur_year.Format("%04d", mn_cur_year);
	mstr_cur_month.Format("%02d", mn_cur_month);
	mstr_cur_day.Format("%02d", mn_cur_day);

	// **************************************************************************
	// 현재 시간 정보 얻는다                                                     
	// **************************************************************************
	mn_cur_hour = time_cur.GetHour();	// 현재 시간 정보를 설정한다. 
	mstr_cur_hour.Format("%d",time_cur.GetHour());	// 현재 시간 정보를 설정한다.
	mstr_cur_min.Format("%d",time_cur.GetMinute());	// 현재 분 정보를 설정한다. 
	mstr_cur_sec.Format("%d",time_cur.GetSecond());	// 현재 초 정보를 설정한다. 
	str_display_time = m_time_current.Format("%c");  // 리스트 파일에 출력할 타이틀 시간 정보 설정 
	// **************************************************************************

	mstr_file.Format("%d.txt",time_cur.GetHour());

	switch (n_msg_select)			//Operation Setting
	{		
	case LOG_OPERATION:
		mstr_file_name = "OP" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_operation + mstr_file_name;
		break;
	case LOG_TESTINTERFACE:							//GPIB
		mstr_file_name = "TI" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_interface + mstr_file_name;
		break;
	case LOG_MACHINE:								//장비 동작 관련.
		mstr_file_name = "MA" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_DEBUG:									// 장비 디버깅 관련.
		mstr_file_name = "DEBUG" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_BARCODE:								// 바코드 관련.
		mstr_file_name = "BARCODE" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_barcode + mstr_file_name;
		break;
	////2014,0303
	case LOG_VISION:
		mstr_file_name = "VISION" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_vision + mstr_file_name;
		break;
	////
	case LOG_TIME:									// TIME관련
		mstr_file_name = "TIME" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_AUTOSOCKETOFF:							//장비 동작 관련.
		mstr_file_name = "AUTOSOCKETOFF" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_WORK:									//장비 동작 관련.
		mstr_file_name = "WORK" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_ADJUSTMENT:							//장비 동작 관련.
		mstr_file_name = "ADJUSTMENT" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_machine + mstr_file_name;
		break;
	case LOG_HISTORY:								//장비 동작 관련.
		mstr_file_name = "HISTORY" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.str_log_path + mstr_file_name;
		break;
		
	//2015.0129
	case LOG_UPH:
		mstr_file_name = "UPH" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		if(mn_cur_hour > 22 && mn_cur_day < 31)
		{
			mstr_cur_day.Format("%02d", mn_cur_day + 1);
			mstr_file_name += mstr_cur_day;
		}
		else	mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_uph + mstr_file_name;
		
		break;

	case LOG_UPH_TOT:
		mstr_file_name = "PROTOT" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		if(mn_cur_hour > 22 && mn_cur_day < 31)
		{
			mstr_cur_day.Format("%02d", mn_cur_day + 1);
			mstr_file_name += mstr_cur_day;
		}
		else	mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_uph_total + mstr_file_name;
		break;

	case LOG_TOTAL:									//전체 메세지.
		mstr_file_name = "TOT" + mstr_cur_year;
		mstr_file_name += mstr_cur_month; 
		mstr_file_name += mstr_cur_day; 
		mstr_create_file = st_path.mstr_total + mstr_file_name;

		//MakeLogFolder(st_path.mstr_total, str_msg);//2015.0126
		break;

	default:
		return;
	}

	mstr_create_file += ".TXT";

	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName, 0);
	
	if (mn_existence == -1)
	{
		switch (n_msg_select)			//Operation Setting
		{		
		case LOG_OPERATION:
			mstr_file_name = "OP" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_operation + mstr_file_name;
			break;
		case LOG_TESTINTERFACE:							//GPIB
			mstr_file_name = "TI" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_interface + mstr_file_name;
			break;
		case LOG_MACHINE:								//장비 동작 관련.
			mstr_file_name = "MA" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_DEBUG:									// 장비 디버깅 관련.
			mstr_file_name = "DEBUG" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_BARCODE:								// 바코드 관련.
			mstr_file_name = "BARCODE" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_barcode + mstr_file_name;
			break;
		////2014,0303
		case LOG_VISION:
			mstr_file_name = "VISION" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_vision + mstr_file_name;
			break;
		////
		case LOG_TIME:									// TIME관련
			mstr_file_name = "TIME" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_AUTOSOCKETOFF:							//장비 동작 관련.
			mstr_file_name = "AUTOSOCKETOFF" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_WORK:									//장비 동작 관련.
			mstr_file_name = "WORK" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_ADJUSTMENT:							//장비 동작 관련.
			mstr_file_name = "ADJUSTMENT" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_machine + mstr_file_name;
			break;
		case LOG_HISTORY:								//장비 동작 관련.
			mstr_file_name = "HISTORY" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.str_log_path + mstr_file_name;
			break;

		//2015.0129
		case LOG_UPH:
			mstr_file_name = "UPH" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			if(mn_cur_hour > 22 && mn_cur_day < 31)
			{
				mstr_cur_day.Format("%02d", mn_cur_day + 1);
				mstr_file_name += mstr_cur_day;
			}
			else	mstr_file_name += mstr_cur_day; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_uph + mstr_file_name;
			
			break;

		case LOG_UPH_TOT:
			mstr_file_name = "PROTOT" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			if(mn_cur_hour > 22 && mn_cur_day < 31)
			{
				mstr_cur_day.Format("%02d", mn_cur_day + 1);
				mstr_file_name += mstr_cur_day;
			}
			else	mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_uph_total + mstr_file_name;
			break;

		case LOG_TOTAL:									//전체 메세지.
			mstr_file_name = "TOT" + mstr_cur_year;
			mstr_file_name += mstr_cur_month; 
			mstr_file_name += mstr_cur_day; 
			mstr_create_file = st_path.mstr_total + mstr_file_name;
			
			//MakeLogFolder(st_path.mstr_total, str_msg);//2015.0126
			break;
			
		default:
			return;
		}
		mstr_create_file += ".TXT";
		// **********************************************************************
	}

	Ret = FileSizeCheck(mstr_create_file, 1000000, CTL_YES); //size and rename
	
	if (Ret == 1) //file size over
	{
		CString strTmp = mstr_create_file.Left( mstr_create_file.GetLength() - 4 );
		BackupName.Format("%s.%02d%02d%02d.bak",strTmp, mn_cur_hour, mn_cur_min, mn_cur_sec );
		BackupName = mstr_create_file + mstr_cur_hour + mstr_cur_min +  mstr_cur_sec + ".bak" ;
		rename(mstr_create_file, BackupName);
	}

	// **************************************************************************
	// 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성              
	// **************************************************************************
	if ((fp = fopen(mstr_create_file,"a+")) == NULL)  
	{
		CString strLog;
		strLog.Format("File Open Failue .. [%s]", mstr_create_file);
		Func.MsgLog(strLog);
		sing.Unlock();
		return;
	}
	// **************************************************************************

	// **************************************************************************
	// 로그 파일에 현재 발생한 알람 정보 저장한다                                
	// **************************************************************************
//	mstr_content += str_display_time;
	mstr_content += sTime;

	mstr_content += " : " + str_msg;

	fprintf(fp,"%s\n",mstr_content);

	if (ferror(fp))  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg = _T("File save failure.");
			sprintf(st_msg.c_abnormal_msg, "File save failure.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		sing.Unlock();
		return ;
	}
	// **************************************************************************
	fclose(fp);  // 파일을 종료한다.

	sing.Unlock();
}

BOOL CPublic_Function::CreateFolder(LPCTSTR szPath)
{
	int nRet = 0;
	
	CString strPath = szPath;
	
    do{
        nRet = strPath.Find('\\' , nRet + 1);
        if (nRet > 0 )
        {
            CString strTemp = strPath.Left(nRet+1);
			
            if (_access(strTemp.operator LPCTSTR(), 00) == -1)
			{
				CreateDirectory(strTemp , NULL);
			}
        }
    }while (nRet != -1);
	
    return nRet;
}

//int CPublic_Function::OnNetworkDataAnalysis(char* strBody)
int CPublic_Function::OnNetworkDataAnalysis(CString strBody)
{
	int nFuncRet = CTLBD_RET_GOOD;

	int i;
	int n_check_num = 0, n_check_string = 0, n_check_extra = 0;
	CString strFunction, strTemp;
	int nID = -1;
	CString strTmp[10];
	CString strPCBOXID;
	CString strpcbox_id;
	CString strEnable;
	int nTempString;
	CString strTestResult;
	int nByPassCheck = 0;
	CString str;
	CString strBN;
	CString strBinTemp;

	int nOperCnt = 0;
	CString strOperInfo;
	BOOL bRet;
	COleDateTime mtime_cur;						// 현재 시간 저장 변수

	strFunction = OnNetworkBodyAnalysis(strBody, "FUNCTION_RPY");

	st_NW.mnID = -1;
	if (strFunction == "NULL")											// 답변이 없다.
	{
		strFunction = OnNetworkBodyAnalysis(strBody, "FUNCTION");

		if (strFunction == "NULL")								st_NW.mnID = NW_INVALID_FUNCTION;
		else if (strFunction ==	"TEMP_CONTROL")					st_NW.mnID = NW_TEMP_CONTROL;
		else if (strFunction == "PCBOX_START")					st_NW.mnID = NW_PCBOX_START;
		else if (strFunction == "PGM_START")					st_NW.mnID = NW_PGM_START;
		else if (strFunction == "BOOT_OK")						st_NW.mnID = NW_BOOT_OK;
		else if (strFunction == "PCBOX_END")					st_NW.mnID = NW_PCBOX_END;			// 여기까진 BPC가 보내는부분.

		else if (strFunction == "PCBOX_STATUS")					st_NW.mnID = NW_PCBOX_STATUS;
		else if (strFunction == "PCBOX_DOWN")					st_NW.mnID = NW_PCBOX_DOWN;

		else if (strFunction == "BUFFER_READY")					st_NW.mnID = NW_BUFFER_READY;
		else if (strFunction == "BUFFER_IN")					st_NW.mnID = NW_BUFFER_IN;
		else if (strFunction == "ONLINE_END")					st_NW.mnID = NW_ONLINE_END;
		else if (strFunction == "NEW_LOT_IN")					st_NW.mnID = NW_NEW_LOT_IN;

		else if (strFunction == "LOT_NAME")						st_NW.mnID = NW_LOT_NAME;

		else if (strFunction == "BLOT_COUNT")					st_NW.mnID = NW_BLOT_COUNT;
		else if (strFunction == "RETEST_REQ")					st_NW.mnID = NW_RETEST_REQ;

		else if (strFunction == "LABEL_INFO")					st_NW.mnID = NW_LABEL_INFO_RPY;
		else if (strFunction == "LABEL_SAMPLE_INFO")			st_NW.mnID = NW_LABEL_SAMPLE_INFO_RPY;


		else if (strFunction == "LOT_START")
		{
			if (st_client[1].n_client_no == 1)
			{
				st_NW.mnID = NW_LOT_START_RPY;				
			}
			else if (st_client[7].n_client_no == 7)
			{
				st_NW.mnID = NEW_LOT_START_RPY;  // 20130115
			}
		}
		
		else if (strFunction == "LOT_CANCEL")
		{
			if (st_client[1].n_client_no == 1)
			{
				st_NW.mnID = NW_LOT_CANCEL;
			}
			else if (st_client[7].n_client_no == 7)
			{
				st_NW.mnID = NEW_LOT_CANCEL;     // 20130115			
			}
		}
		else if (strFunction == "LOT_END")						
		{
			if (st_client[1].n_client_no == 1)
			{
				st_NW.mnID = NW_LABEL_LOT_END_RPY;
			}
			else if (st_client[7].n_client_no == 7)
			{
				st_NW.mnID = NEW_LOT_END_RPY;	 // 20130115			
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		else if (strFunction == "MAT_CHECK")					st_NW.mnID = NEW_MAT_CHECK_RPY;  // 20130115
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		if (strFunction ==      "TRACK_IN")						st_NW.mnID = NW_TRACK_IN_RPY;
		else if (strFunction == "BPC_BUFFER_READY")				st_NW.mnID = NW_BPC_BUFFER_READY_RPY;
		else if (strFunction == "BPC_BUFFER_IN")				st_NW.mnID = NW_BPC_BUFFER_IN_RPY;
		else if (strFunction == "PCBOX_TIMEOUT")				st_NW.mnID = NW_PCBOX_TIMEOUT_RPY;
		else if (strFunction == "ONLINE_END")					st_NW.mnID = NW_ONLINE_END_RPY;
		else if (strFunction == "PCBOX_OFF")					st_NW.mnID = NW_PCBOX_OFF_RPY;
		else if (strFunction == "LAST_RETEST")					st_NW.mnID = NW_LAST_RETEST_RPY;
		else if (strFunction == "TRACK_OUT")					st_NW.mnID = NW_TRACK_OUT_RPY;		// 여기까진 EPC의 답변
		
		else if (strFunction == "PCBOX_STATUS")					st_NW.mnID = NW_PCBOX_STATUS_RPY;
		else if (strFunction == "PCBOX_DOWN")					st_NW.mnID = NW_PCBOX_DOWN_RPY;
		else if (strFunction == "VIDEO_SELECT")					st_NW.mnID = NW_VIDEO_SELECT_RPY;	// 여기까진 서로 보내는 부분.

		else if (strFunction == "BUFFER_IN")					st_NW.mnID = NW_BUFFER_IN_RPY;
		else if (strFunction == "BUFFER_READY")					st_NW.mnID = NW_BUFFER_READY_RPY;
		else if (strFunction == "RETEST_REQ")					st_NW.mnID = NW_RETEST_REQ_RPY;
		else if (strFunction == "NEW_LOT_IN")					st_NW.mnID = NW_NEW_LOT_IN_RPY;

		else if (strFunction == "APCTSOCKETINFO")				st_NW.mnID = NW_APCTSOCKETINFO_RPY;
		else if (strFunction == "LOT_NAME")						st_NW.mnID = NW_LOT_NAME_RPY;

		else if (strFunction == "BLOT_COUNT")					st_NW.mnID = NW_BLOT_COUNT_RPY;
		else if (strFunction == "VERSION_UPDATE")				st_NW.mnID = NW_VERSION_UPDATE_RPY;
		else if (strFunction == "SCRAP_INFO")					st_NW.mnID = NW_SCRAP_INFO_RPY;
		else if (strFunction == "ONLINE_END")					st_NW.mnID = NW_ONLINE_END_RPY;
		else if (strFunction == "SORTER_LOT_END")				st_NW.mnID = NW_TRACK_OUT_RPY;
		else if (strFunction == "LOTDISPLAY")					st_NW.mnID = NW_LOT_DISPLAY_RPY;

		else if (strFunction == "LABEL_INFO")					st_NW.mnID = NW_LABEL_INFO_RPY;
		else if (strFunction == "LABEL_SAMPLE_INFO")			st_NW.mnID = NW_LABEL_SAMPLE_INFO_RPY;
		else if (strFunction == "LABEL_NEW_INFO")				st_NW.mnID = NW_LABEL_INFO_RPY;
		
		else if (strFunction == "LOT_START")
		{
			if (st_client[1].n_client_no == 1)
			{
				st_NW.mnID = NW_LOT_START_RPY;				
			}
			else if (st_client[7].n_client_no == 7)
			{
				st_NW.mnID = NEW_LOT_START_RPY;  // 20130115
			}
		}
		else if (strFunction == "LOT_CANCEL")
		{
			if (st_client[1].n_client_no == 1)
			{
				st_NW.mnID = NW_LOT_CANCEL_RPY;
			}
			else if (st_client[7].n_client_no == 7)
			{
				st_NW.mnID = NEW_LOT_CANCEL_RPY;     // 20130115			
			}
		}
		else if (strFunction == "LOT_END")						
		{
			if (st_client[1].n_client_no == 1)
			{
				st_NW.mnID = NW_LABEL_LOT_END_RPY;
			}
			else  if (st_client[7].n_client_no == 7)
			{
				st_NW.mnID = NEW_LOT_END_RPY;	 // 20130115			
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		else if (strFunction == "MAT_CHECK")					st_NW.mnID = NEW_MAT_CHECK_RPY;  // 20130115
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}

	st_NW.mstr_pcbox_id[1] = "";

	switch (st_NW.mnID)
	{
//====================================================================================================================================
// 여기서부터는 장비가 질문/통보를 받는 부분이다.
//====================================================================================================================================
	case NW_INVALID_FUNCTION:
		// RESULT=FAIL MSG="INVALID FUNCTION!"
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");
		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG", 2);

		st_NW.mn_invaild_function_flag = CTL_YES;
		st_NW.mstr_result[1] = "";//2014,0212

		break;

	case NW_VERSION_UPDATE_RPY:
		// FUNCTION_RPY=VERSION_UPDATE RESULT=PASS MSG="" 
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");

		if (st_NW.mstr_result[1] == "PASS")
		{
			st_NW.mn_update_version_flag = CTL_YES;
		}
		break;

	case NW_TRACK_OUT_RPY:
		// FUNCTION_RPY=SORTER_LOT_END LOTNO=SKXZ000205 EQPID=PS-GL RESULT=PASS MSG="SiMAX TRANSACTION OK!" 
		st_NW.mstr_result[1] =			OnNetworkBodyAnalysis(strBody, "RESULT");
		st_sync.mstr_track_out =		st_NW.mstr_result[1];
		st_sync.mstr_track_out_msg =	OnNetworkBodyAnalysis(strBody, "MSG", 8);

		if (st_sync.mstr_track_out_msg != "")
		{
			st_msg.mstr_event_msg[0] = st_sync.mstr_track_out_msg;
			::SendMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
		}
		break;

	case NW_SCRAP_INFO_RPY:
		// FUNCTION_RPY=SCRAP_INFO LOTNO=SKXZ000205 EQPID=PS-GL SCRAP_CNT=4 SCRAP_INFO001=(SCRAP_CODE=4111 SERIAL=SKXZ00020532DF4D28) SCRAP_INFO002=(SCRAP_CODE=4111 SERIAL=SKXZ00020532DF5291) SCRAP_INFO003=(SCRAP_CODE=4108 SERIAL=SKXZ00020532DF4E17) SCRAP_INFO004=(SCRAP_CODE=4108 SERIAL=SKXZ00020532DF4D4D) RESULT=PASS MSG="" 
		// FUNCTION_RPY=SCRAP_INFO LOTNO=SIMAXTEST1 EQPID=PS-HL RESULT=FAIL MSG="LOT INFO SELECT ERROR!(LOTNO=SIMAXTEST1 SQLCODE=1403)"  
		st_NW.mstr_new_lot_no[1] =	OnNetworkBodyAnalysis(strBody, "LOTNO");
		st_NW.mstr_m_eqp_id[1] =	OnNetworkBodyAnalysis(strBody, "EQPID");
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");

		st_sync.mstr_scrap_info =	st_NW.mstr_result[1];

		st_work.strMdlRejectRouterSorter.RemoveAll();
		st_work.strMdlRejectCodeRouterSorter.RemoveAll();
		st_work.strMdlRejectFCT.RemoveAll();
		st_work.strMdlRejectCodeFCT.RemoveAll();
		st_work.strMdlRejectPCTSorter.RemoveAll();
		st_work.strMdlRejectCodePCTSorter.RemoveAll();

		if (st_NW.mstr_result[1] == "PASS")
		{
			st_work.strMdlReject.RemoveAll();
			st_work.strMdlRejectCode.RemoveAll();

			st_NW.mstr_scrap_cnt[1] =		OnNetworkBodyAnalysis(strBody, "SCRAP_CNT");
			st_work.n_scrap_info_fail_qty = atoi(st_NW.mstr_scrap_cnt[1]);

			for (i = 0; i < atoi(st_NW.mstr_scrap_cnt[1]); i++)
			{
				strTemp.Format("SCRAP_INFO%03d", i + 1);
				st_NW.mstr_temp[1] =	OnNetworkBodyAnalysis(strBody, strTemp, 9);

				st_work.strMdlReject.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SERIAL", 2));
				st_work.strMdlRejectCode.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SCRAP_CODE"));

				if (atoi(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SCRAP_CODE")) <= 3999)
				{
					st_work.strMdlRejectRouterSorter.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SERIAL", 2));
					st_work.strMdlRejectCodeRouterSorter.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SCRAP_CODE"));
				}
				else if (atoi(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SCRAP_CODE")) <= 4099)
				{
					st_work.strMdlRejectFCT.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SERIAL", 2));
					st_work.strMdlRejectCodeFCT.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SCRAP_CODE"));
				}
				else
				{
					st_work.strMdlRejectPCTSorter.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SERIAL", 2));
					st_work.strMdlRejectCodePCTSorter.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SCRAP_CODE"));
				}
			}

			if (st_handler.cwnd_scrap_info != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_SCRAP_DSP, 0, 0);
			}
		}
		break;


////////////////////////////////////////////////////////////////////////////////
	case NEW_LOT_START_RPY: // 20130115
		st_NW.mstr_lot_id[1] =		OnNetworkBodyAnalysis(strBody, "LOTNO");
		st_NW.mstr_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQPID");
		//		st_NW.mstr_qty[1] =			OnNetworkBodyAnalysis(strBody, "QTY");
		st_NW.mstr_part_id[1] =		OnNetworkBodyAnalysis(strBody, "PARTID");
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");
		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG", 1);
		if (st_NW.mstr_result[1] == "PASS")
		{
		}
		break;
		
	case NEW_LOT_END_RPY:  // 20130115
		st_NW.mstr_lot_id[1] =		OnNetworkBodyAnalysis(strBody, "LOTNO");
		st_NW.mstr_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQPID");
		st_NW.mstr_oper_id[1] =		OnNetworkBodyAnalysis(strBody, "OPERID");
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");
		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG", 1);
		if(st_NW.mstr_result[1] == "PASS")
		{
			//	st_handler.mn_lot_cancel_mode = CTL_YES;
		}
		break;
		
	case NEW_LOT_CANCEL_RPY:   // 20130115
		st_NW.mstr_lot_id[1] =		OnNetworkBodyAnalysis(strBody, "LOTNO");
		st_NW.mstr_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQPID");
		st_NW.mstr_oper_id[1] =		OnNetworkBodyAnalysis(strBody, "OPERID");
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");
		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG", 1);
		if(st_NW.mstr_result[1] == "PASS")
		{
			//	st_handler.mn_lot_cancel_mode = CTL_YES;
		}
		break;
		
	case NEW_MAT_CHECK_RPY:  // 20130115
		st_NW.mstr_lot_id[1] =		OnNetworkBodyAnalysis(strBody, "LOTNO");
		st_NW.mstr_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQPID");
		st_NW.mstr_oper_id[1] =		OnNetworkBodyAnalysis(strBody, "OPERID");
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");
		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG", 1);
		if(st_NW.mstr_result[1] == "PASS")
		{
			//	st_handler.mn_lot_cancel_mode = CTL_YES;
		}
		break;
//////////////////////////////////////////////////////////////////////////////////////


	case NW_ONLINE_END_RPY:
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");

		st_sync.mstr_online_end = st_NW.mstr_result[1];
		break;

	case NW_LOT_DISPLAY_RPY:
		st_NW.mstr_qty[1] =			OnNetworkBodyAnalysis(strBody, "QTY");
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");

		st_sync.mstr_lot_display = st_NW.mstr_result[1];
		st_work.n_lotdisply_qty = atoi(st_NW.mstr_qty[1]);
		break;
		
	case NW_LOT_START_RPY:
		st_NW.mstr_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQPID");
		st_NW.mstr_lot_id[1] =		OnNetworkBodyAnalysis(strBody, "LOTNO");
		st_NW.mstr_qty[1] =			OnNetworkBodyAnalysis(strBody, "QTY");
		st_NW.mstr_part_id[1] =		OnNetworkBodyAnalysis(strBody, "PARTID");
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");
		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG", 1);
		if (st_NW.mstr_result[1] == "PASS")
		{
		}
		break;

	case NW_LABEL_INFO_RPY:
		st_NW.mstr_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQPID");
		st_NW.mstr_lot_id[1] =		OnNetworkBodyAnalysis(strBody, "LOTNO");
		//st_NW.mstr_qty[1] =			OnNetworkBodyAnalysis(strBody, "QTY");
		//st_NW.mstr_part_id[1] =		OnNetworkBodyAnalysis(strBody, "PARTID");
		st_NW.mstr_oper_id[1] =		OnNetworkBodyAnalysis(strBody, "OPERID");

		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");
		
//		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG", 1);
		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG",2);//20120913

		st_NW.mstr_qty[1] = "10";//20120913
		st_sync.mstr_lot_display = st_NW.mstr_result[1];

		st_work.n_lotdisply_qty = atoi(st_NW.mstr_qty[1]);
		
		if (st_NW.mstr_result[1] == "PASS")
		{
			st_work.strlabelFormat.RemoveAll();
			for (i = 0; i < atoi(st_NW.mstr_qty[1]); i++)
			{
				st_NW.mstr_Label_Pasing[i] = "";//2014,0215
				strTemp.Format("FORMAT_%d", i + 1);
				st_NW.mstr_temp[1] =	OnNetworkBodyAnalysis(strBody, strTemp, 9);

				/////////////////////////////////////////////////// 20120826
//				st_work.strlabelFormatCode.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "FORMAT", 2));
				st_NW.mstr_Recive_Data[i] = OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "FORMAT", 2);
				bRet = OnLabelPrint_Parsing(st_NW.mstr_Recive_Data[i], i);	
				if (bRet == TRUE)
				{//필요할때만 출력

//					sprintf(st_msg.c_normal_msg, "라벨 전산 데이타 파싱 완료");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				else if (bRet == FALSE)
				{
					sprintf(st_msg.c_normal_msg, "라벨 전산 데이타 파싱 실패");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "Computerized data parsing failed label!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					st_NW.mstr_Label_Pasing[i] = "FAIL"; //2014,0215
				}
				///////////////////////////////////////////////////
				st_work.strlabelFormat.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SERIAL"));
			}
			//if (st_handler.cwnd_scrap_info != NULL)
			//{
			//	st_handler.cwnd_main->PostMessage(WM_SCRAP_DSP, 0, 0);
			//}

			////2014,0215
			for (i = 0; i < atoi(st_NW.mstr_qty[1]); i++)
			{
				if(st_NW.mstr_Label_Pasing[i] == "FAIL")
				{
					st_NW.mstr_result[1] = "FAIL";
					break;
				}
			}
			////

			st_handler.mn_lot_label_info = CTL_YES;
		}
		break;

	case NW_LABEL_LOT_END_RPY:
		st_NW.mstr_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQPID");
		st_NW.mstr_lot_id[1] =		OnNetworkBodyAnalysis(strBody, "LOTNO");
		st_NW.mstr_oper_id[1] =		OnNetworkBodyAnalysis(strBody, "OPERID");
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");
		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG", 1);
		if(st_NW.mstr_result[1] == "PASS")
		{
			st_handler.mn_lot_end = CTL_YES;
		}
		break;
		
	case NW_LOT_CANCEL_RPY:
		st_NW.mstr_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQPID");
		st_NW.mstr_lot_id[1] =		OnNetworkBodyAnalysis(strBody, "LOTNO");
		st_NW.mstr_oper_id[1] =		OnNetworkBodyAnalysis(strBody, "OPERID");
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");
		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG", 1);
		if(st_NW.mstr_result[1] == "PASS")
		{
			st_handler.mn_lot_cancel_mode = CTL_YES;
		}
		break;

	case NW_LABEL_SAMPLE_INFO_RPY:
		st_NW.mstr_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQPID");
		st_NW.mstr_lot_id[1] =		OnNetworkBodyAnalysis(strBody, "LOTNO");
		st_NW.mstr_oper_id[1] =		OnNetworkBodyAnalysis(strBody, "OPERID");
		
		st_NW.mstr_result[1] =		OnNetworkBodyAnalysis(strBody, "RESULT");
		
		st_NW.mstr_msg[1] =			OnNetworkBodyAnalysis(strBody, "MSG", 1);
		if(st_NW.mstr_result[1] == "PASS")
		{
			st_work.strlabelFormat.RemoveAll();
			for (i = 0; i < 1; i++)
			{
				strTemp.Format("FORMAT_%d", i + 1);
				st_NW.mstr_temp[1] =	OnNetworkBodyAnalysis(strBody, strTemp, 9);

				st_work.strsamplelabelFormatCode.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "FORMAT", 2));
				st_work.strsamplelabelFormat.AddTail(OnNetworkBodyAnalysis(st_NW.mstr_temp[1], "SERIAL"));
			}
			//if (st_handler.cwnd_scrap_info != NULL)
			//{
			//	st_handler.cwnd_main->PostMessage(WM_SCRAP_DSP, 0, 0);
			//}
		}
		break;

	case NW_LOT_CANCEL:
		break;

	case NW_NEW_LOT_IN:				// 0000000000000050 FUNCTION=NEW_LOT_IN LOT_NO=TEST_LOT EQP_ID=IR8130
		st_NW.mstr_new_lot_no[1] = OnNetworkBodyAnalysis(strBody, "LOT_NO");

		break;
//====================================================================================================================================
// 여기까지는 장비가 질문/통보를 받는 부분이다.
//====================================================================================================================================

//====================================================================================================================================
// 여기서부터는 장비의 질문에 답변이 온 부분이다.
//====================================================================================================================================
	case NW_TRACK_IN_RPY:
		break;
// 여기까지는 장비의 질문에 답변이 온 부분이다.

//====================================================================================================================================
// 여기서부터는 앞장비에서 온 통신이다.
//====================================================================================================================================
	case NW_BUFFER_READY:
		st_NW.mstr_m_lot_no[1] =		OnNetworkBodyAnalysis(strBody, "LOT_NO");
		st_NW.mstr_m_part_no[1] =		OnNetworkBodyAnalysis(strBody, "PART_NO");

		st_NW.mstr_m_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQP_ID");
		st_NW.mstr_m_buffer_no[1] =		OnNetworkBodyAnalysis(strBody, "BUFFER_NO");
		st_NW.mstr_m_last_module[1] =	OnNetworkBodyAnalysis(strBody, "LAST_MODULE");
		st_NW.mstr_m_bypass[1] =		OnNetworkBodyAnalysis(strBody, "BYPASS");
		strTemp =						OnNetworkBodyAnalysis(strBody, "RETEST", 2);

		st_work.n_sorter_retest = CTL_NO;

		if (strTemp == "YES")
		{
			st_work.n_sorter_retest = CTL_YES;
		}
		
		st_handler.str_main_lot = st_NW.mstr_m_lot_no[1];
		st_handler.str_main_part = st_NW.mstr_m_part_no[1];
		st_handler.str_main_last = st_NW.mstr_m_last_module[1];
		st_handler.str_main_bypass = st_NW.mstr_m_bypass[1];
		st_handler.str_main_cycle = "0";

		nTempString = atoi(st_NW.mstr_m_buffer_no[1]) - 1;					// Buffer No

		if (nTempString < 0 || nTempString > 1)							return CTLBD_RET_ERROR;

		if ( g_AMTLotMgr.FindLotByName( st_NW.mstr_m_lot_no[1] ).CheckPartNo_forDIMM( st_NW.mstr_m_part_no[1] ) == false )	return CTLBD_RET_ERROR;
		
		sprintf(st_buffer_tray[nTempString].cTotalLotNumber, st_NW.mstr_m_lot_no[1]);

		st_handler.mstr_front_lot_name =					st_buffer_tray[nTempString].cTotalLotNumber;

		sprintf(st_buffer_tray[nTempString].cTotalPartNumber, st_NW.mstr_m_part_no[1]);
		
		if (st_NW.mstr_m_last_module[1] == "YES")
		{
			sprintf(st_msg.c_normal_msg, "%d번 버퍼에서 라스트 모듈이 들어왔습니다. 0번 배열 상태 [%s]", nTempString, st_module_datamap[nTempString][0].mstrLastModule);
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_normal_msg, "No%d The last came from the buffer module. No0 Arrangement [%s]", nTempString, st_module_datamap[nTempString][0].mstrLastModule);
			}
			

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청

			st_buffer_tray[nTempString].nLastModule = CTL_YES;
			st_work.n_sorter_retest = CTL_YES;
		}
		else
		{
			st_buffer_tray[nTempString].nLastModule = CTL_NO;
		}

		if (st_NW.mstr_m_bypass[1] == "YES")
		{
			st_buffer_tray[nTempString].nByPass = CTL_YES;
		}
		else
		{
			st_buffer_tray[nTempString].nByPass = CTL_NO;
		}
		
		st_sync.mn_rbuffer_work_id[nTempString] = NW_BUFFER_READY;
		st_sync.mn_rbuffer_work[nTempString] = CTL_REQ;

		st_NW.mstr_buffer_in_recv[nTempString] = ""; 
		
		strBN.Format( "WorkID = BUFFER_READY, BufferNo = %d", nTempString );
		Func.On_Log_Title_Msg( LOG_TEST, strBN, "BUFFER_READY" );
		break;

	case NW_BUFFER_IN:
		st_NW.mstr_m_buffer_no[1] =		OnNetworkBodyAnalysis(strBody, "BUFFER_NO");

		nTempString = atoi(st_NW.mstr_m_buffer_no[1]) - 1;					// Buffer No
		
		if (nTempString < 0 || nTempString > 1)							return CTLBD_RET_ERROR;

		if ( st_NW.mb_buffer_in_received[nTempString] == false )			return CTLBD_RET_ERROR;

		st_sync.mn_rbuffer_work_id[nTempString] = NW_BUFFER_IN;
		st_sync.mn_rbuffer_work[nTempString] = CTL_REQ;

		strBN.Format( "WorkID = BUFFER_IN, BufferNo = %d", nTempString );
		Func.On_Log_Title_Msg( LOG_TEST, strBN, "BUFFER_IN" );

		if ( st_NW.mstr_buffer_in_recv[nTempString] == strBody )			return CTLBD_RET_GOOD;

		st_NW.mstr_m_lot_no[1] =		OnNetworkBodyAnalysis(strBody, "LOT_NO");
		st_NW.mstr_m_eqp_id[1] =		OnNetworkBodyAnalysis(strBody, "EQP_ID");

		if ( st_buffer_tray[nTempString].cTotalLotNumber != "" &&
			st_NW.mstr_m_lot_no[1] != st_buffer_tray[nTempString].cTotalLotNumber )
		{
			sprintf(cJamcode, "200700");
			CTL_Lib.Alarm_Error_Occurrence(5000, CTL_dWARNING, cJamcode);
			return CTLBD_RET_ERROR;
		}

		st_NW.mstr_m_module_cnt[1] =	OnNetworkBodyAnalysis(strBody, "MDL_CNT");

		for (i = 0; i < 20; i++)
		{
			strTemp.Format("MDL_%02d", i + 1);

			if (i < 19)
			{
				if ( st_handler.mn_reverse_mode == 1 && st_handler.mn_machine_pos == 0)
				{
					st_NW.mstr_m_module_data[19 - i][1] =	OnNetworkBodyAnalysis(strBody, strTemp);
				}
				else
				{
					st_NW.mstr_m_module_data[i][1] =	OnNetworkBodyAnalysis(strBody, strTemp);
				}
			}
			else
			{
				if (st_handler.mn_reverse_mode == 1 && st_handler.mn_machine_pos == 0)
				{
					st_NW.mstr_m_module_data[19 - i][1] =	OnNetworkBodyAnalysis(strBody, strTemp);
				}
				else
				{
					st_NW.mstr_m_module_data[i][1] =	OnNetworkBodyAnalysis(strBody, strTemp, 2);
				}
			}
		}
		
		sprintf(st_buffer_tray[nTempString].cTotalLotNumber, st_NW.mstr_m_lot_no[1]);
		sprintf(st_buffer_tray[nTempString].cModuleCnt, st_NW.mstr_m_module_cnt[1]);

		st_buffer_tray[nTempString].nRetestMdlCnt[0] = 0;
		st_buffer_tray[nTempString].nRetestMdlCnt[1] = 0;

		for (i = 0; i < 20; i++)
		{
			if (st_NW.mstr_m_module_data[i][1] == "NONE")
			{
				st_buffer_tray[nTempString].nDeviceExistence[0][0][i] = CTL_NO;
			}
			else
			{
				st_buffer_tray[nTempString].nDeviceExistence[0][0][i] = CTL_YES;
			}

			sprintf(st_buffer_tray[nTempString].cModuleData[0][i], st_NW.mstr_m_module_data[i][1]);

			if (st_NW.mstr_m_module_data[i][1].GetLength() > 22)
			{
				sprintf(st_buffer_tray[nTempString].cLotNumber[0][i], OnDeviceInfoAnalysis(st_buffer_tray[nTempString].cModuleData[0][i], 0));				// Lot No
				sprintf(st_buffer_tray[nTempString].cSerialNumber[0][i], OnDeviceInfoAnalysis(st_buffer_tray[nTempString].cModuleData[0][i], 1));			// Serial
				st_buffer_tray[nTempString].nArrayInfo[0][i] = atoi(OnDeviceInfoAnalysis(st_buffer_tray[nTempString].cModuleData[0][i], 2));				// Array info
				sprintf(st_buffer_tray[nTempString].cTestResult[0][i], OnDeviceInfoAnalysis(st_buffer_tray[nTempString].cModuleData[0][i], 3));			// Test Result
				
				strBinTemp = st_buffer_tray[nTempString].cTestResult[0][i];

				if (strBinTemp == "PASS")
				{
					st_buffer_tray[nTempString].nDeviceExistence[1][0][i] = BIN_PASS;
				}
				else if (strBinTemp == "REJECT")
				{
					st_buffer_tray[nTempString].nDeviceExistence[1][0][i] = BIN_REJECT;
				}
				else if (strBinTemp == "HTO")
				{
					st_buffer_tray[nTempString].nDeviceExistence[1][0][i] = BIN_HTO;
				}
				else if (strBinTemp == "NTO")
				{
					st_buffer_tray[nTempString].nDeviceExistence[1][0][i] = BIN_NTO;
				}
				else if (strBinTemp == "BTO")
				{
					st_buffer_tray[nTempString].nDeviceExistence[1][0][i] = BIN_BTO;
				}
				else if (strBinTemp == "BP")
				{
					st_buffer_tray[nTempString].nDeviceExistence[1][0][i] = BIN_BP;
				}
				else 
				{
					str.Format("[%02d]",i);
					Func.On_LogFile_Add(3, "NW_BUFFER_IN  Other Bin" + str);
					st_buffer_tray[nTempString].nDeviceExistence[1][0][i] = BIN_REJECT;
				}

				st_buffer_tray[nTempString].nDeviceExistence[2][0][i] = atoi(OnDeviceInfoAnalysis(st_buffer_tray[nTempString].cModuleData[0][i], 4));				// Retest Count
			}
		}

		if ( st_buffer_tray[nTempString].nLastModule == CTL_YES )
		{
			g_AMTLotMgr.SetCanPushNewLot();
		}

		OnLogRecord_Data( nTempString, 1, true, "Public_Function-NW_BUFFER_IN" );

		st_bufferdb.mole_date = COleDateTime::GetCurrentTime();
		st_bufferdb.mstrlot_no = st_NW.mstr_m_lot_no[1];
		st_bufferdb.mstrpart_no = st_NW.mstr_m_part_no[1];
		st_bufferdb.mnbuffer_pos = nTempString;
		st_bufferdb.mnlast_module = st_buffer_tray[nTempString].nLastModule;
		st_bufferdb.mnbypass = st_buffer_tray[nTempString].nByPass;
		st_bufferdb.mnmodule_cnt = atoi(st_NW.mstr_m_module_cnt[1]);
		st_bufferdb.mnretest;
		
// 		DB_Write_Buffer(st_bufferdb);

		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BUFFER_DSP, 0);	// Buffer 갱신.
		}

		st_NW.mstr_buffer_in_temp[nTempString] = strBody;
		break;

	case NW_RETEST_REQ:
		// 0000000000000101FUNCTION=RETEST_REQ LOT_NO=S0H4000204 BUFFER_NO=02 SOCKET_OFF=1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
		str = OnNetworkBodyAnalysis(strBody, "SOCKET_OFF", 2);

		for (i = 0; i < 20; i++)
		{
			if (str.Mid(i * 2, 1) == "1")
			{
				st_modulemap.nRetestSocketUse[i] = CTL_YES;
			}
			else
			{
				st_modulemap.nRetestSocketUse[i] = CTL_NO;
			}
		}
		break;
	}
	
	return nFuncRet;
}

void CPublic_Function::IO_Map_Initialize()
{
	int i = 0;
	int nPost = 0;
	

	// ************************************************************
	//  Module Stacker
	// ************************************************************
	st_io.o_m_stacker_guide_clamp_onoff				= 0;		//s0000
	st_io.o_m_stacker_guide_unclamp_onoff			= 1;
	st_io.o_m_stacker1_up_onoff						= 2;
	st_io.o_m_stacker1_dn_onoff						= 3;
	st_io.o_m_stacker2_up_onoff						= 4;
	st_io.o_m_stacker2_dn_onoff						= 5;
	st_io.o_m_stacker_tray_pusher_bwd_onoff			= 6;
	st_io.o_m_stacker_tray_pusher_fwd_onoff			= 7;
	st_io.o_m_stacker_tray_pusher_clamp_updn		= 8;
	st_io.o_m_stacker1_rail_fwd_onoff				= 9;
	st_io.o_m_stacker1_rail_bwd_onoff				= 10;
	st_io.o_m_stacker2_rail_fwd_onoff				= 11;
	st_io.o_m_stacker2_rail_bwd_onoff				= 12;
	st_io.o_m_stacker2_tray_clamp_onoff				= 13;

	st_io.i_m_stacker_guide_clamp_on_chk			= 100;		//ps0100
	st_io.i_m_stacker_guide_unclamp_off_chk			= 101;

	st_io.i_m_stacker1_up_chk						= 102;
	st_io.i_m_stacker1_dn_chk						= 103;
	st_io.i_m_stacker1_tray_chk						= 104;
	st_io.i_m_stacker_1_updn_pos_chk				= 105;

	st_io.i_m_ld_slide_tray_chk						= 102;
	st_io.i_m_ld_slide_onoff_chk					= 103;
	st_io.i_m_uld_slide_tray_chk					= 104;
	st_io.i_m_uld_slide_onoff_chk					= 105;

	st_io.i_m_stacker2_up_chk						= 106;
	st_io.i_m_stacker2_dn_chk						= 107;
	st_io.i_m_stacker2_tray_chk						= 108;
	st_io.i_m_stacker_2_updn_pos_chk				= 109;
	st_io.i_m_stacker_tray_pusher_bwd_chk			= 110;
	st_io.i_m_stacker_tray_pusher_fwd_chk			= 111;
	st_io.i_m_stacker_tray_pusher_clamp_up_chk		= 112;
	st_io.i_m_stacker_tray_pusher_clamp_dn_chk		= 113;
	st_io.i_m_stacker_tray_guide_chk				= 114;

	st_io.i_m_stacker1_rail_tray_chk				= 200;		//ps200
	st_io.i_m_stacker1_rail_fwd_chk					= 201;
	st_io.i_m_stacker1_rail_bwd_chk					= 202;
	st_io.i_m_stacker2_rail_tray_chk				= 203;
	st_io.i_m_stacker2_rail_fwd_chk					= 204;
	st_io.i_m_stacker2_rail_bwd_chk					= 205;
	st_io.i_m_stacker2_tray_clamp_on_chk			= 206;
	st_io.i_m_stacker2_tray_clamp_off_chk			= 207;
	st_io.i_HS_Forward_Barcode_Up_chk			    = 208;
	st_io.i_HS_Forward_Barcode_Down_chk			    = 209;
	st_io.i_HS_Backward_Barcode_Up_chk			    = 210;
	st_io.i_HS_Backward_Barcode_Down_chk			= 211;
	st_io.i_Left_Clip_Barcode_Up_chk		    	= 212;
	st_io.i_Left_Clip_Barcode_Down_chk			    = 213;
	st_io.i_Right_Clip_Barcode_Up_chk		    	= 214;
	st_io.i_Right_Clip_Barcode_Down_chk	    		= 215;

	// ************************************************************
	//  Heat Sink Stacker
	// ************************************************************
	st_io.o_hs_fwd_stacker_guide_clamp_onoff		= 300;  //s0300
	st_io.o_hs_fwd_stacker_guide_unclamp_onoff		= 301;
	st_io.o_hs_fwd_stacker1_up_onoff				= 302;
	st_io.o_hs_fwd_stacker1_dn_onoff				= 303;
	st_io.o_clip_remove_blower_on   				= 304;//20121020
	st_io.o_clip_remove_blower_off  				= 305;//20121020
	st_io.o_hs_fwd_stacker_tray_pusher_bwd_onoff	= 306;
	st_io.o_hs_fwd_stacker_tray_pusher_fwd_onoff	= 307;
	st_io.o_hs_fwd_stacker_tray_pusher_clamp_updn	= 308;
	st_io.o_hs_fwd_stacker1_rail_fwd_onoff			= 309;
	st_io.o_hs_fwd_stacker1_rail_bwd_onoff			= 310;
	st_io.o_hs_fwd_stacker2_rail_fwd_onoff			= 311;
	st_io.o_hs_fwd_stacker2_rail_bwd_onoff			= 312;
	st_io.o_hs_fwd_stacker2_tray_clamp_onoff		= 313;
	st_io.o_hs_fwd_clamp_onoff						= 314;	//2012,0720 추가
	st_io.o_hs_bwd_clamp_onoff						= 315;


	st_io.i_hs_fwd_stacker_guide_clamp_on_chk		= 400;	//ps0400
	st_io.i_hs_fwd_stacker_guide_unclamp_off_chk	= 401;

	st_io.i_hs_fwd_stacker1_up_chk					= 402;
	st_io.i_hs_fwd_stacker1_dn_chk					= 403;
	st_io.i_hs_fwd_stacker1_tray_chk				= 404;
	st_io.i_hs_fwd_stacker1_updn_pos_chk			= 405;

	st_io.i_hs_fwd_ld_slide_tray_chk				= 402;
	st_io.i_hs_fwd_ld_slide_onoff_chk				= 403;
	st_io.i_hs_fwd_uld_slide_tray_chk				= 404;
	st_io.i_hs_fwd_uld_slide_onoff_chk				= 405;

	st_io.i_hs_fwd_stacker2_up_chk					= 406;
	st_io.i_hs_fwd_stacker2_dn_chk					= 407;
	st_io.i_hs_fwd_stacker2_tray_chk				= 408;
	st_io.i_hs_fwd_stacker2_updn_pos_chk			= 409;
	st_io.i_hs_fwd_stacker_tray_pusher_bwd_chk		= 410;
	st_io.i_hs_fwd_stacker_tray_pusher_fwd_chk		= 411;
	st_io.i_hs_fwd_stacker_tray_pusher_clamp_up_chk	= 412;
	st_io.i_hs_fwd_stacker_tray_pusher_clamp_dn_chk	= 413;
	st_io.i_hs_fwd_stacker_tray_guide_chk			= 414;

	st_io.i_hs_fwd_stacker1_rail_tray_chk			= 500;	//s0500
	st_io.i_hs_fwd_stacker1_rail_fwd_chk			= 501;
	st_io.i_hs_fwd_stacker1_rail_bwd_chk			= 502;
	st_io.i_hs_fwd_stacker2_rail_tray_chk			= 503;
	st_io.i_hs_fwd_stacker2_rail_fwd_chk			= 504;
	st_io.i_hs_fwd_stacker2_rail_bwd_chk			= 505;
	st_io.i_hs_fwd_stacker2_tray_clamp_on_chk		= 506;
	st_io.i_hs_fwd_stacker2_tray_clamp_off_chk		= 507;

	//2012,0720 추가
	st_io.i_hs_fwd_clamp_on_chk						= 510;
	st_io.i_hs_fwd_clamp_off_chk					= 511;
	st_io.i_hs_bwd_clamp_on_chk						= 512;
	st_io.i_hs_bwd_clamp_off_chk					= 513;

	
	st_io.o_hs_bwd_stacker_guide_clamp_onoff		= 600;	//ps0600
	st_io.o_hs_bwd_stacker_guide_unclamp_onoff		= 601;
	st_io.o_hs_bwd_stacker1_up_onoff				= 602;
	st_io.o_hs_bwd_stacker1_dn_onoff				= 603;
	st_io.o_hs_bwd_stacker2_up_onoff				= 604;
	st_io.o_hs_bwd_stacker2_dn_onoff				= 605;
	st_io.o_hs_bwd_stacker_tray_pusher_fwd_onoff	= 606;
	st_io.o_hs_bwd_stacker_tray_pusher_bwd_onoff	= 607;
	st_io.o_hs_bwd_stacker_tray_pusher_clamp_updn	= 608;
	st_io.o_hs_bwd_stacker1_rail_fwd_onoff			= 609;
	st_io.o_hs_bwd_stacker1_rail_bwd_onoff			= 610;
	st_io.o_hs_bwd_stacker2_rail_fwd_onoff			= 611;
	st_io.o_hs_bwd_stacker2_rail_bwd_onoff			= 612;
	st_io.o_hs_bwd_stacker2_tray_clamp_onoff		= 613;
	

	st_io.i_hs_bwd_stacker_guide_clamp_on_chk		= 700;	//ps0700
	st_io.i_hs_bwd_stacker_guide_unclamp_off_chk	= 701;

	st_io.i_hs_bwd_stacker1_up_chk					= 702;
	st_io.i_hs_bwd_stacker1_dn_chk					= 703;
	st_io.i_hs_bwd_stacker1_tray_chk				= 704;
	st_io.i_hs_bwd_stacker1_updn_pos_chk			= 705;

	st_io.i_hs_bwd_ld_slide_tray_chk				= 702;
	st_io.i_hs_bwd_ld_slide_onoff_chk				= 703;
	st_io.i_hs_bwd_uld_slide_tray_chk				= 704;
	st_io.i_hs_bwd_uld_slide_onoff_chk				= 705;

	st_io.i_hs_bwd_stacker2_up_chk					= 706;
	st_io.i_hs_bwd_stacker2_dn_chk					= 707;
	st_io.i_hs_bwd_stacker2_tray_chk				= 708;
	st_io.i_hs_bwd_stacker2_updn_pos_chk			= 709;
	st_io.i_hs_bwd_stacker_tray_pusher_bwd_chk		= 710;
	st_io.i_hs_bwd_stacker_tray_pusher_fwd_chk		= 711;
	st_io.i_hs_bwd_stacker_tray_pusher_clamp_up_chk	= 712;
	st_io.i_hs_bwd_stacker_tray_pusher_clamp_dn_chk	= 713;
	st_io.i_hs_bwd_stacker_tray_guide_chk			= 714;

	st_io.i_hs_bwd_stacker1_rail_tray_chk			= 1000; //ps1000
	st_io.i_hs_bwd_stacker1_rail_fwd_chk			= 1001;
	st_io.i_hs_bwd_stacker1_rail_bwd_chk			= 1002;
	st_io.i_hs_bwd_stacker2_rail_tray_chk			= 1003;
	st_io.i_hs_bwd_stacker2_rail_fwd_chk			= 1004;
	st_io.i_hs_bwd_stacker2_rail_bwd_chk			= 1005;
	st_io.i_hs_bwd_stacker2_tray_clamp_on_chk		= 1006;
	st_io.i_hs_bwd_stacker2_tray_clamp_off_chk		= 1007;

	// ************************************************************
	//  Unload Clamp
	// ************************************************************
	// module 11
	st_io.o_uld_stacker_guide_clamp_onoff			= 1100;
	st_io.o_uld_stacker_guide_unclamp_onoff			= 1101;
	st_io.o_uld_stacker2_up_onoff					= 1102;
	st_io.o_uld_stacker2_dn_onoff					= 1103;
	st_io.o_uld_stacker1_up_onoff					= 1104;
	st_io.o_uld_stacker1_dn_onoff					= 1105;
	st_io.o_uld_stacker_tray_pusher_fwd_onoff		= 1106;
	st_io.o_uld_stacker_tray_pusher_bwd_onoff		= 1107;
	st_io.o_uld_stacker_tray_pusher_clamp_updn		= 1108;
	st_io.o_uld_stacker1_rail_fwd_onoff				= 1109;
	st_io.o_uld_stacker1_rail_bwd_onoff				= 1110;
	st_io.o_uld_stacker2_rail_fwd_onoff				= 1111;
	st_io.o_uld_stacker2_rail_bwd_onoff				= 1112;
	st_io.o_uld_stacker2_tray_clamp_onoff			= 1113;
	
	// module 12
	st_io.i_uld_stacker_guide_clamp_on_chk			= 1200;
	st_io.i_uld_stacker_guide_unclamp_off_chk		= 1201;

	st_io.i_uld_stacker2_up_chk						= 1202;
	st_io.i_uld_stacker2_dn_chk						= 1203;
	st_io.i_uld_stacker2_tray_chk					= 1204;
	st_io.i_uld_stacker2_updn_pos_chk				= 1205;

	st_io.i_uld_ld_slide_tray_chk					= 1202;
	st_io.i_uld_ld_slide_onoff_chk					= 1203;
	st_io.i_uld_uld_slide_tray_chk					= 1204;
	st_io.i_uld_uld_slide_onoff_chk					= 1205;

	st_io.i_uld_stacker1_up_chk						= 1206;
	st_io.i_uld_stacker1_dn_chk						= 1207;
	st_io.i_uld_stacker1_tray_chk					= 1208;
	st_io.i_uld_stacker1_updn_pos_chk				= 1209;
	st_io.i_uld_stacker_tray_pusher_bwd_chk			= 1210;
	st_io.i_uld_stacker_tray_pusher_fwd_chk			= 1211;
	st_io.i_uld_stacker_tray_pusher_clamp_up_chk	= 1212;
	st_io.i_uld_stacker_tray_pusher_clamp_dn_chk	= 1213;
	st_io.i_uld_stacker_tray_guide_chk				= 1214;

	// module 13
	st_io.i_uld_stacker1_rail_tray_chk				= 1300;
	st_io.i_uld_stacker1_rail_fwd_chk				= 1301;
	st_io.i_uld_stacker1_rail_bwd_chk				= 1302;
	st_io.i_uld_stacker2_rail_tray_chk				= 1303;
	st_io.i_uld_stacker2_rail_fwd_chk				= 1304;
	st_io.i_uld_stacker2_rail_bwd_chk				= 1305;	
	st_io.i_uld_stacker2_tray_clamp_on_chk			= 1306;
	st_io.i_uld_stacker2_tray_clamp_off_chk			= 1307;	
	st_io.i_uld_stacker_direction_chk				= 1308;

	// ************************************************************
	//  Module Robot Picker On/Off
	// ************************************************************
	st_io.o_ld_module_rbt_picker1_updn				= 1400;			// s1400
	st_io.o_ld_module_rbt_glipper1_onoff			= 1401;			// s1401
	st_io.o_ld_module_rbt_picker2_updn				= 1402;			// s1402
	st_io.o_ld_module_rbt_glipper2_onoff			= 1403;			// s1403
	for (i = 0 ; i < PICKER_NUM; i++)
	{
		st_io.o_ld_module_picker_updn[i]			= 1400 + (i*2);
		st_io.o_ld_module_glipper_onoff[i]			= 1401 + (i*2);
	}
	st_io.o_front_hs_bcr_onoff						= 1404;
	st_io.o_back_hs_bcr_onoff						= 1405;
	st_io.o_left_clip_bcr_onoff						= 1406;
	st_io.o_right_clip_bcr_onoff					= 1407;
	
	st_io.i_ld_module_rbt_picker1_up_chk			= 1408;			// ps1400
	st_io.i_ld_module_rbt_picker1_dn_chk			= 1409;			// ps1401
	st_io.i_ld_module_rbt_glipper1_dvc_chk			= 1410;		// ps1402
	st_io.i_ld_module_rbt_picker2_up_chk			= 1411;			// ps1403
	st_io.i_ld_module_rbt_picker2_dn_chk			= 1412;			// ps1404
	st_io.i_ld_module_rbt_glipper2_dvc_chk			= 1413;		// ps1405
	st_io.i_ld_module_rbt_dvc_direction_chk			= 1414;
	for (i = 0 ; i < PICKER_NUM; i++)
	{
		st_io.i_ld_module_picker_up_chk[i]			= 1408 + (i*3);
		st_io.i_ld_module_picker_dn_chk[i]			= 1409 + (i*3);
		st_io.i_ld_module_dvc_chk[i]				= 1410 + (i*3);
	}

	// module 15
	st_io.o_hs_fwd_rbt_picker1_updn					= 1500;		// s1500
	st_io.o_hs_fwd_rbt_picker1_gripper_onoff		= 1501;
	st_io.o_hs_fwd_rbt_picker2_updn					= 1502;
	st_io.o_hs_fwd_rbt_picker2_gripper_onoff		= 1503;
	for (i = 0 ; i < PICKER_NUM; i++)
	{
		st_io.o_hs_fwd_rbt_picker_picker_updn[i]	= 1500 + (i*2);
		st_io.o_hs_fwd_rbt_glipper_onoff[i]			= 1501 + (i*2);
	}

	st_io.i_hs_fwd_rbt_picker1_up_chk				= 1508;
	st_io.i_hs_fwd_rbt_picker1_dn_chk				= 1509;
	st_io.i_hs_fwd_rbt_picker1_gripper_dvc_chk		= 1510;
	st_io.i_hs_fwd_rbt_picker2_up_chk				= 1511;
	st_io.i_hs_fwd_rbt_picker2_dn_chk				= 1512;
	st_io.i_hs_fwd_rbt_picker2_gripper_dvc_chk		= 1513;
	st_io.i_hs_fwd_rbt_direction_1					= 1514;
	st_io.i_hs_fwd_rbt_direction_2					= 1515;
	
	for (i = 0 ; i < PICKER_NUM; i++)
	{
		st_io.i_hs_fwd_picker_up_chk[i]				= 1508 + (i*3);
		st_io.i_hs_fwd_picker_dn_chk[i]				= 1509 + (i*3);
		st_io.i_hs_fwd_dvc_chk[i]					= 1510 + (i*3);
	}
				
	// module 16
	st_io.o_hs_bwd_rbt_picker1_updn					= 1600;		// s1600
	st_io.o_hs_bwd_rbt_picker1_gripper_onoff		= 1601;
	st_io.o_hs_bwd_rbt_picker2_updn					= 1602;
	st_io.o_hs_bwd_rbt_picker2_gripper_onoff		= 1603;
	for (i = 0 ; i < PICKER_NUM; i++)
	{
		st_io.o_hs_bwd_rbt_picker_picker_updn[i]	= 1600 + (i*2);
		st_io.o_hs_bwd_rbt_glipper_onoff[i]			= 1601 + (i*2);
	}

	st_io.i_hs_bwd_rbt_picker1_up_chk				= 1608;
	st_io.i_hs_bwd_rbt_picker1_dn_chk				= 1609;
	st_io.i_hs_bwd_rbt_picker1_gripper_dvc_chk		= 1610;
	st_io.i_hs_bwd_rbt_picker2_up_chk				= 1611;
	st_io.i_hs_bwd_rbt_picker2_dn_chk				= 1612;
	st_io.i_hs_bwd_rbt_picker2_gripper_dvc_chk		= 1613;
	st_io.i_hs_bwd_rbt_direction_1					= 1614;
	st_io.i_hs_bwd_rbt_direction_2					= 1615;

	for (i = 0 ; i < PICKER_NUM; i++)
	{
		st_io.i_hs_bwd_picker_up_chk[i]				= 1608 + (i*3);
		st_io.i_hs_bwd_picker_dn_chk[i]				= 1609 + (i*3);
		st_io.i_hs_bwd_dvc_chk[i]					= 1610 + (i*3);
	}

	st_io.o_ld_tube_s_latch_fwdbwd_onoff			= 1700;			// s1700 초기전진
	st_io.o_tube_s_m_plate_fwd_onoff				= 1701;
	st_io.o_tube_s_m_plate_bwd_onoff				= 1702;
	st_io.o_tube_s_m_plate_short_updn_onoff			= 1703;
	st_io.o_tube_s_m_plate_long_updn_onoff			= 1704;
	st_io.i_ld_tube_s_latch_fwd_chk					= 1705;
	st_io.i_ld_tube_s_latch_bwd_chk					= 1706;
	st_io.i_ld_tube_s_cover_on_chk					= 1707;
	st_io.i_ld_tube_s_empty_chk						= 1708;
	st_io.i_ld_tube_s_full_chk						= 1709;
	st_io.i_tube_s_m_plate_fwd_chk					= 1710;
	st_io.i_tube_s_m_plate_bwd_chk					= 1711;
	st_io.i_tube_s_m_plate_short_up_chk				= 1712;
	st_io.i_tube_s_m_plate_short_dn_chk				= 1713;
	st_io.i_tube_s_m_plate_long_up_chk				= 1714;
	st_io.i_tube_s_m_plate_long_dn_chk				= 1715;

	st_io.o_tube_rotator_0_onoff					= 2000;							//s2000
	st_io.o_tube_rotator_90_onoff					= 2001;
	st_io.o_tube_rotator_clamp_onoff				= 2002;
	st_io.o_tube_rotator_unclamp_onoff				= 2003;
	st_io.o_tube_rotator_cap_remove_onoff			= 2004;
	st_io.o_tube_rotator_latch_fwd_onoff			= 2005;
	st_io.o_tube_rotator_latch_bwd_onoff			= 2006;

	st_io.i_tube_rotator_0_chk						= 2008;
	st_io.i_tube_rotator_90_chk						= 2009;
	st_io.i_tube_rotator_clamp_chk					= 2010;
	st_io.i_tube_rotator_unclamp_chk				= 2011;
	st_io.i_tube_rotator_cap_remove_on_chk			= 2012;
	st_io.i_tube_rotator_cap_remove_off_chk			= 2013;
	st_io.i_tube_rotator_latch_fwd_onoff			= 2014;              //초기전진
	st_io.i_tube_rotator_latch_bwd_onoff			= 2015;

	st_io.o_uld_tube_pusher_fwdbwd_onoff			= 2100;				// s2100
	st_io.o_uld_tube_s_updn_onoff					= 2101;
	st_io.o_clip_buffer_updn_onoff					= 2102;

	st_io.i_ld_tube_chk								= 2105;
	st_io.i_rotate_tube_chk							= 2106;
	st_io.i_uld_tube_s_up_chk						= 2107;
	st_io.i_uld_tube_s_dn_chk						= 2108;
	st_io.i_uld_tube_s_cover_on_chk					= 2109;
	st_io.i_uld_tube_chk							= 2110;
	st_io.i_uld_tube_s_full_chk						= 2111;
	st_io.i_clip_buff_up_chk						= 2112;
	st_io.i_clip_buff_dn_chk						= 2113;
	st_io.i_clip_buff_clip1_chk						= 2114;
	st_io.i_clip_buff_clip2_chk						= 2115;

	// ************************************************************
	//  Work Buffer 1,2,3,4
	// ************************************************************
	st_io.o_buf1_f_vacuum_onoff						= 2200;		// s2200
	st_io.o_buf1_f_eject_onoff						= 2201;		// s2201
	st_io.o_buf1_f_hs_onoff							= 2201;		// s2201

	st_io.o_buf1_m_clamp_onoff						= 2204;		// s2204
	st_io.o_buf1_hs_clamp_onoff						= 2205;		// s2205

	st_io.i_buf1_f_vacuum_chk						= 2208;		// ps2200 8
	st_io.i_buf1_b_vacuum_chk						= 2209;		// ps2201
	st_io.i_buf1_m_clamp_on_chk						= 2210;		// ps2202
	st_io.i_buf1_m_clamp_off_chk					= 2211;		// ps2203
	st_io.i_buf1_hs_clamp_on_chk					= 2212;		// ps2204
	st_io.i_buf1_hs_clamp_off_chk					= 2213;		// ps2205
	st_io.i_tube_push_fwd_chk						= 2214;
	st_io.i_tube_push_bwd_chk						= 2215;

	st_io.o_buf2_f_vacuum_onoff						= 2300;		// s2300
	st_io.o_buf2_f_eject_onoff						= 2301;		// s2301
	st_io.o_buf2_f_hs_onoff							= 2301;		// s2201
	st_io.o_buf2_b_vacuum_onoff						= 2302;		// s2302
	st_io.o_buf2_b_eject_onoff						= 2303;		// s2303
	st_io.o_buf2_m_clamp_onoff						= 2304;		// s2304
	st_io.o_buf2_hs_clamp_onoff						= 2305;		// s2305

	st_io.i_buf2_f_vacuum_chk						= 2308;		// ps2300 8
	st_io.i_buf2_b_vacuum_chk						= 2309;		// ps2301
	st_io.i_buf2_m_clamp_on_chk						= 2310;		// ps2302
	st_io.i_buf2_m_clamp_off_chk					= 2311;		// ps2303
	st_io.i_buf2_hs_clamp_on_chk					= 2312;		// ps2304
	st_io.i_buf2_hs_clamp_off_chk					= 2313;		// ps2305

	st_io.o_buf3_f_vacuum_onoff						= 2400;		// s2400
	st_io.o_buf3_f_eject_onoff						= 2401;		// s2401
	st_io.o_buf3_f_hs_onoff							= 2401;		// s2201
	st_io.o_buf3_b_vacuum_onoff						= 2402;		// s2402
	st_io.o_buf3_b_eject_onoff						= 2403;		// s2403
	st_io.o_buf3_m_clamp_onoff						= 2404;		// s2404
	st_io.o_buf3_hs_clamp_onoff						= 2405;		// s2405
	st_io.o_clip_buffer_align_onoff					= 2406;		// s2406 //add 2012,0720

	st_io.i_buf3_f_vacuum_chk						= 2408;		// ps2400 8
	st_io.i_buf3_b_vacuum_chk						= 2409;		// ps2401
	st_io.i_buf3_m_clamp_on_chk						= 2410;		// ps2402
	st_io.i_buf3_m_clamp_off_chk					= 2411;		// ps2403
	st_io.i_buf3_hs_clamp_on_chk					= 2412;		// ps2404
	st_io.i_buf3_hs_clamp_off_chk					= 2413;		// ps2405
	st_io.i_clip_buffer_align_on_chk				= 2414;		// ps2406 //add 2012,0720
	st_io.i_clip_buffer_align_off_chk				= 2415;		// ps2407

	st_io.o_buf4_f_vacuum_onoff						= 2500;		// s2500
	st_io.o_buf4_f_eject_onoff						= 2501;		// s2501
	st_io.o_buf4_f_hs_onoff							= 2501;		// s2201
	st_io.o_buf4_b_vacuum_onoff						= 2502;		// s2502
	st_io.o_buf4_b_eject_onoff						= 2503;		// s2503
	st_io.o_buf4_m_clamp_onoff						= 2504;		// s2504
	st_io.o_buf4_hs_clamp_onoff						= 2505;		// s2505
	st_io.o_vlp_bcr_clamp_onoff						= 2506;		// s2506 //add 2012,0720

	st_io.i_buf4_f_vacuum_chk						= 2508;		// ps2500 8
	st_io.i_buf4_b_vacuum_chk						= 2509;		// ps2501
	st_io.i_buf4_m_clamp_on_chk						= 2510;		// ps2502
	st_io.i_buf4_m_clamp_off_chk					= 2511;		// ps2503
	st_io.i_buf4_hs_clamp_on_chk					= 2512;		// ps2504
	st_io.i_buf4_hs_clamp_off_chk					= 2513;		// ps2505
	st_io.i_vlp_bcr_clamp_on_chk					= 2514;		// ps2506 //add 2012,0720
	st_io.i_vlp_bcr_clamp_off_chk					= 2515;		// ps2507

	// ************************************************************
	//  Clip Robot
	// ************************************************************
	st_io.o_clip_rbt_updn_onoff						= 2600;		// s2600
	st_io.o_clip_remove_up_onoff					= 2601;
	st_io.o_clip_remove_down_onoff					= 2602;
	
	st_io.o_ldclip_tube_updn_onoff					= 2605;
	st_io.o_uldclip_tube_updn_onoff					= 2606;

	st_io.i_clip_rbt_up_chk							= 2608;		// ps2600
	st_io.i_clip_rbt_dn_chk							= 2609;
	st_io.i_vision1_module_chk						= 2610;
	st_io.i_vision2_module_chk						= 2611;
	st_io.i_ldclip_tube_up_chk						= 2612;
	st_io.i_ldclip_tube_dn_chk						= 2613;
	st_io.i_uldclip_tube_up_chk						= 2614;
	st_io.i_uldclip_tube_dn_chk						= 2615;

	// ************************************************************
	//  Picker Robot
	// ************************************************************
	st_io.o_uld_picker_rbt_updn_onoff					= 2700;	// s2700
	st_io.o_uld_picker_rbt_gripper_onoff				= 2701;							
	st_io.o_sorter_picker_rbt_updn_onoff				= 2702;
	st_io.o_sorter_picker_rbt_gripper_onoff				= 2703;
	
	st_io.i_uld_picker_rbt_up_chk						= 2706;	// ps2700
	st_io.i_uld_picker_rbt_dn_chk						= 2707;
	st_io.i_uld_picker_rbt_dvc_chk						= 2708;
	st_io.i_sorter_picker_rbt_up_chk					= 2709;
	st_io.i_sorter_picker_rbt_dn_chk					= 2710;
	st_io.i_sorter_picker_rbt_dvc_chk					= 2711;

	for (i = 0; i < 3; i++)
	{
		st_io.i_safety_door[i]					    	= 2712 + i;
	}

	// ************************************************************
	//  Lavel Attach
	// ************************************************************
	st_io.o_label_attach_vacuum_onoff				= 3000;	// s3000
	st_io.o_label_attach_eject_onoff				= 3001;
	st_io.o_label_attach_unit_fwd_onoff				= 3002;
	st_io.o_label_attach_unit_bwd_onoff				= 3003;
	

	st_io.i_print_label_chk							= 3006;	// ps3000
	st_io.i_label_attach_vacuum_on_chk				= 3007;
	st_io.i_label_attach_unit_fwd_chk				= 3008;
	st_io.i_label_attach_unit_bwd_chk				= 3009;
	st_io.i_reject_tray_chk							= 3010;

	st_io.i_vision1_safety_up_chk					= 3012;
	st_io.i_vision1_safety_dn_chk					= 3013;
	st_io.i_vision2_safety_up_chk					= 3014;
	st_io.i_vision2_safety_dn_chk					= 3015;


	// ************************************************************
	//  Vision Buffer Rotator
	// ************************************************************
	st_io.o_vis_buff1_rotator_onoff					= 3100;						// s3100
	st_io.o_vis_buff1_clamp_onoff					= 3101;
	st_io.o_vis_buff2_rotator_onoff					= 3102;
	st_io.o_vis_buff2_clamp_onoff					= 3103;
	st_io.o_ionizer_onoff							= 3104;

	for (i = 0; i < 2; i++)
	{
		st_io.o_vis_buffer_rotator_onoff[i]			= 3100 + (2*i);
		st_io.o_vis_buffer_clamp_onoff[i]			= 3101 + (2*i);
	}

	st_io.i_vis_buff1_rotator_on_chk				= 3106;
	st_io.i_vis_buff1_rotator_off_chk				= 3107;
	st_io.i_vis_buff1_clamp_on_chk					= 3108;
	st_io.i_vis_buff1_clamp_off_chk					= 3109;
	st_io.i_vis_buff2_rotator_on_chk				= 3110;
	st_io.i_vis_buff2_rotator_off_chk				= 3111;
	st_io.i_vis_buff2_clamp_on_chk					= 3112;
	st_io.i_vis_buff2_clamp_off_chk					= 3113;
	st_io.i_ionizer_err_chk							= 3114; //add 2012,0720

	for (i = 0; i < 2; i++)
	{
		st_io.i_vis_buff_rotator_on_chk[i]			= 3106 + (4 * i);
		st_io.i_vis_buff_rotator_off_chk[i]			= 3107 + (4 * i);
		st_io.i_vis_buff_clamp_on_chk[i]			= 3108 + (4 * i);
		st_io.i_vis_buff_clamp_off_chk[i]			= 3109 + (4 * i);
	}

	st_io.o_vis_reset1_onoff						= 3200;							// s3200
	st_io.o_vis_reset2_onoff						= 3201;
	st_io.o_vis_trigger1_onoff						= 3202;
	st_io.o_vis_trigger2_onoff						= 3203;
	st_io.o_bcr_print_feed							= 3204;

	st_io.i_vis_ready1_chk							= 3206;
	st_io.i_vis_ready2_chk							= 3207;
	st_io.i_vis_ready3_chk							= 3208;
	st_io.i_vis_ready4_chk							= 3209;
	st_io.i_bcr_print_ribbon_row					= 3210;
	st_io.i_bcr_print_servide_required				= 3211;
	st_io.i_bcr_print_end_print						= 3212;
	st_io.i_bcr_print_media_out						= 3213;
	st_io.i_bcr_print_ribbon_out					= 3214;
	st_io.i_bcr_print_data_ready					= 3215;


	// ************************************************************
	//  Switch & Tower Lamp
	// ************************************************************
	st_io.o_start_switch_lamp						= 3300;		// s3300
	st_io.o_stop_switch_lamp						= 3301;		// s3301
	st_io.o_alarm_reset_switch_lamp					= 3302;		// s3302	
	st_io.o_buzzer_off_switch_lamp					= 3303;		// s3303
	
	st_io.i_start_switch_chk						= 3308;		// PS3300
	st_io.i_stop_switch_chk							= 3309;		// PS3301
	st_io.i_alarm_reset_switch_chk					= 3310;		// PS3302
	st_io.i_buzzer_switch_chk						= 3311;		// PS3303
	st_io.i_auto_mode_switch_chk					= 3312; 	// PS3304
	st_io.i_manual_mode_switch_chk					= 3313;		// PS3305
	st_io.i_front_select_switch_chk					= 3314;		// PS3406
	st_io.i_rear_select_switch_chk					= 3315;		// PS3407
	// ************************************************************
	st_io.o_main_air								= 3400;		// S3400
	st_io.o_towerlamp_green							= 3401;		// S3401
	st_io.o_towerlamp_yellow						= 3402;		// S3402
	st_io.o_towerlamp_red							= 3403;		// S3403
	st_io.o_buzzer_1								= 3404;		// s3404
	st_io.o_buzzer_2								= 3405;		// s3405
	st_io.o_lock_door_onoff							= 3406;		// s3406 All door
	for (i = 0; i < 2; i++)
	{
		st_io.o_buzzer[i]							= 3404+i;
	}

	st_io.i_main_air_chk							= 3408;		// PS3400
	st_io.i_btm_door1_chk							= 3409;		// PS3401
	st_io.i_btm_door2_chk							= 3410;		// PS3402
	st_io.i_btm_door3_chk							= 3411;		// PS3403
	st_io.i_btm_door4_chk							= 3412;		// PS3404
	st_io.i_btm_door5_chk							= 3413;		// PS3405
	st_io.i_btm_door6_chk							= 3414;		// PS3406
	st_io.i_btm_door7_chk							= 3415;		// PS3407

	st_io.i_top_door1_chk							= 3500;								// PS3500
	st_io.i_top_door2_chk							= 3501;
	st_io.i_top_door3_chk							= 3502;
	st_io.i_top_door4_chk							= 3503;
	st_io.i_top_door5_chk							= 3504;
	st_io.i_top_door6_chk							= 3505;
	st_io.i_top_door7_chk							= 3506;
	st_io.i_top_door8_chk							= 3507;
	st_io.i_top_door9_chk							= 3508;
	st_io.i_top_door10_chk							= 3509;
	st_io.i_top_door11_chk							= 3510;
	st_io.i_top_door12_chk							= 3511;
	st_io.i_top_door13_chk							= 3512;
	st_io.i_top_door14_chk							= 3513;

	for (i= 0; i < 14; i++)
	{
		st_io.i_top_door[i]							= 3500+i;
	}

	st_io.i_mc1_chk									= 3600;		//MOTOR CHECK
	st_io.i_mc2_chk									= 3601;

	st_io.i_emo1_chk								= 3604;
	st_io.i_emo2_chk								= 3605;
	st_io.i_emo3_chk								= 3606;
	st_io.i_emo4_chk								= 3607;
	
	st_io.i_stacker_light_curtain_chk				= 3614;
	st_io.i_vision_light_curtain_chk				= 3615;

	st_io.i_vision_ready1							= 3706;		// PS3700
	st_io.i_vision_ready2							= 3707;
	st_io.i_vision_complste1						= 3708;
	st_io.i_vision_passfail1						= 3709;
	st_io.i_vision_complste2						= 3710;
	st_io.i_vision_passfail2						= 3711;
	st_io.i_vision_yield1							= 3712;
	st_io.i_vision_yield12							= 3713;
	st_io.i_vision_error1							= 3714;
	st_io.i_vision_error2							= 3715;
}

CString CPublic_Function::Get_MotorName(int n_motor_no)
{
	CString strRet;

	switch (n_motor_no)
	{
	case 0:
		strRet = "Module Loader Stacker_1";
		break;
	case 1:
		strRet = "Module Loader Stacker_2";
		break;
	case 2:
		strRet = "Module Loader stacker move";
		break;
	case 3:
		strRet = "Heat Sink Front stacker_1";
		break;
	case 4:
		strRet = "Heat Sink Front stacker_2";
		break;
	case 5:
		strRet = "Heat Sink Front stacker_move";
		break;
	case 6:
		strRet = "Heat Sink Back stacker_1";
		break;
	case 7:
		strRet = "Heat Sink Back stacker_2";
		break;
	case 8:
		strRet = "Heat Sink Back stacker_move";
		break;
	case 9:
		strRet = "Unloader Module stacker_1";
		break;
	case 10:
		strRet = "Unloader Module stacker_2";
		break;
	case 11:
		strRet = "Unloader Module stacker_move";
		break;
	case 12:
		strRet = "Loader Module Robot Y";
		break;
	case 13:
		strRet = "Loader Module Robot Z";
		break;
	case 14:
		strRet = "Heat Front Robot Y";
		break;
	case 15:
		strRet = "Heat Front Robot Z";
		break;
	case 16:
		strRet = "Heat Back Robot Y";
		break;
	case 17:
		strRet = "Heat Back Robot Z";
		break;
	case 18:
		strRet = "Clip Buffer";
		break;
	case 19:
		strRet = "Moving Work Buffer_1";
		break;
	case 20:
		strRet = "Moving Work Buffer_2";
		break;
	case 21:
		strRet = "Moving Work Buffer_3";
		break;
	case 22:
		strRet = "Moving Work Buffer_4";
		break;
	case 23:
		strRet = "Clamp Robot Gripper";
		break;
	case 24:
		strRet = "Clamp Robot Y";
		break;
	case 25:
		strRet = "Clamp Robot Z";
		break;
	case 26:
		strRet = "Clip Insert Y";
		break;
	case 27:
		strRet = "Clip Insert Z";
		break;
	case 28:
		strRet = "Sorter Robot X";
		break;
	case 29:
		strRet = "Sorter Robot Y";
		break;
	case 30:
		strRet = "Sorter Robot Z";
		break;
	case 31:
		strRet = "Unloader Robot Z";
		break;
	case 32:
		strRet = "Unloader Robot X";
		break;
	case 33:
		strRet = "Unloader Robot Y";
		break;
	case 34:
		strRet = "NO Motor";
		break;
	case 35:
		strRet = "Label Attach Z";
		break;
	case 36:
		strRet = "Vision Robot_1 Y";
		break;
	case 37:
		strRet = "Vision Robot_1 Z";
		break;
	case 38:
		strRet = "Vision Robot_2 Y";
		break;
	case 39:
		strRet = "Vision Robot_2 Z";
		break;

	default:
		strRet = "Unknown_Motor";
		break;
	}
	
	return strRet;
}

int CPublic_Function::OnElect_CheckData(CString str_data)
{
	int nFuncRet = CTLBD_RET_GOOD;
	
	char	cp_cmd[1024];
	int     nLength;  // 데이터 총 길이 저장 변수 
	CString strBody;  // BODY 데이터 저장 변수 
	CString strTemp;
	
	int nChannel;
	double dValue;
	
	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 
	
	// **************************************************************************
	// 수신 데이터 최소 길이 검사한다 [24자리가 되야 한다.]
	// **************************************************************************
	nLength = str_data.GetLength();
	if (nLength != 25)			
		return CTLBD_RET_ERROR;
	// **************************************************************************
	
	strBody = str_data.Mid(2, nLength - 3);  // [헤더+테일] 제거 
	strBody.TrimLeft(' ');
	strBody.TrimRight(' ');
	
	strTemp = strBody.Mid(11, 1);			// 채널.
	nChannel = atoi(strTemp);
	
	strTemp = strBody.Mid(12, 5);
	dValue = atof(strTemp);
	
	st_work.mdReadElectData[nChannel-1] = dValue;	// 값을 넣는다.
	
	if (strBody.Mid(12, 2) == "OP")
	{
//		st_msg.mstr_normal_msg.Format("%dPort : OPEN", nChannel);
		sprintf(st_msg.c_normal_msg, "%dPort : OPEN", nChannel);
		st_grounddb.mstrChannel[nChannel-1].Format("OPEN");
		st_work.mdReadElectData[nChannel-1] = -99999;	// 값을 넣는다.
	}
	else if (strBody.Mid(12, 2) == "OV")
	{
//		st_msg.mstr_normal_msg.Format("%dPort : OVER", nChannel);
		sprintf(st_msg.c_normal_msg, "%dPort : OVER", nChannel);
		st_grounddb.mstrChannel[nChannel-1].Format("OVER");
		st_work.mdReadElectData[nChannel-1] = -99998;	// 값을 넣는다.
	}
	else
	{
//		st_msg.mstr_normal_msg.Format("%dPort : %0.2f", nChannel, dValue);
		sprintf(st_msg.c_normal_msg, "%dPort : %0.2f", nChannel, dValue);
		st_grounddb.mstrChannel[nChannel-1].Format("%0.2f", dValue);
	}
	
	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
	
	if (st_handler.cwnd_basic != NULL)											// BASIC 화면 존재
	{
		st_handler.cwnd_basic->SendMessage(WM_BASIC_APPLY, 1, nChannel);		// DATA APPLY 요청
	}
	
	return nFuncRet;
}

void CPublic_Function::OnMot_Speed_Setting()
{
	int i;

	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		if (COMI.md_spd_vel[i][1] < 1)
		{
			COMI.md_spd_vel[i][1] = 10000;

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg.Format("%s Motor Acc is under 1 -> 10000 Set up", Get_MotorName(i));
				sprintf(st_msg.c_abnormal_msg, "%s Motor Acc is under 1 -> 10000 Set up", Get_MotorName(i));
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}
		
		if (COMI.md_spd_vel[i][2] < 1)
		{
			COMI.md_spd_vel[i][2] = 10000;

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "%s Motor Dec is under 1 -> 10000 Set up", Get_MotorName(i));
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
		}

		COMI.Set_MotSpeed(MOT_SPD_VEL, i, cmSMODE_T, COMI.md_spd_vel[i][0], COMI.md_spd_vel[i][1], COMI.md_spd_vel[i][2]);
		COMI.Set_MotSpeed(MOT_SPD_ORG, i, cmSMODE_T, st_motor[i].md_spd_home, COMI.md_spd_vel[i][1]/10, COMI.md_spd_vel[i][2]/10); //st_motor[i].d_spd_home[0] * 10, st_motor[i].d_spd_home[0] * 10);

		COMI.Set_HomeSetConfig(i, COMI.mn_homecheck_method[i], 0, 2, 1);
		Sleep(100);
	}
}

int CPublic_Function::DoorOpenCheckSpot()
{
	int Ret = CTLBD_RET_GOOD, n_check = 0, i = 0;
	
	if (st_basic.n_mode_use_door == 0)
	{
		for (i = 0; i < 14; i++)
 		{
 			if (FAS_IO.get_in_bit(st_io.i_top_door[i], IO_OFF) != TRUE && n_check == 0)
 			{
 				n_check++;
 				break;
 			} 
 		}
 		
 		if (n_check > 0)
 		{
 			//if (st_work.mn_run_status == dRUN)
 			//{
			if(COMI.mn_simulation_mode == 1)
			{
				Ret = CTLBD_RET_GOOD;
			}
			else
			{
 				alarm.mstr_code.Format("9001%02d", i);
				sprintf(cJamcode,"9001%02d", i);
 				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
 				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
				CTL_Lib.Alarm_Error_Occurrence(5010, CTL_dWARNING, cJamcode);
				Ret = CTLBD_RET_ERROR;
			}
 		}

 		for (i = 0; i < 3; i++)
 		{
 			if (FAS_IO.get_in_bit(st_io.i_safety_door[i], IO_OFF) != IO_OFF && n_check == 0)
 			{
 				n_check++;
 				break;
 			} 
 		}

 		if (n_check > 0)
 		{
			if(COMI.mn_simulation_mode == 1) // 20130509
			{
				Ret = CTLBD_RET_GOOD;
			}
			else
			{
				//{//900120 0 90 "Sadety Door1 Open Error."
// 				alarm.mstr_code.Format("90012%d", i);  
//				sprintf(cJamcode,"90012%d", i);
				alarm.mstr_code.Format("9001%02d", i);
				sprintf(cJamcode,"9001%02d", i);
				alarm.mn_count_mode = 0;				// 알람 카운트 여부 플래그 설정 (알람 카운트 작업 미진행)
 				alarm.mn_type_mode = eWARNING;		// 현재 발생한 알람 상태 플래그 설정 
				CTL_Lib.Alarm_Error_Occurrence(5011, CTL_dWARNING, cJamcode);
				Ret = CTLBD_RET_ERROR;
			}
 		}		
	}
	return Ret;
}


int CPublic_Function::OnMotor_Go_Check(int n_motor, double d_pos)
{
	CString str_msg, str_motor;
	
	int n_response, nRet = CTL_NO;
	
	CDialog_Select	select_dlg;
	
	str_motor = Get_MotorName(n_motor);
	
	str_msg.Format("%s Motor, Move value is %3.3f, Go?",str_motor, (float)d_pos);
	
	st_msg.mstr_confirm_msg = _T(str_msg);
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		nRet = CTL_YES;
	}
	else
	{
		nRet = CTL_NO;
	}
	
	return nRet;
}

int CPublic_Function::On_Encoder_Check(double d_pos)
{
	CString str_msg;
	
	int n_response, nRet = CTL_NO;
	
	CDialog_Select	select_dlg;
	
	str_msg.Format("Now Reading Value is %ld, Setting?", (long)d_pos);
	st_msg.mstr_confirm_msg = _T(str_msg);
	n_response = select_dlg.DoModal();
	if (n_response == IDOK) 
	{
		nRet = CTL_YES;
	}
	else
	{
		nRet = CTL_NO;
	}
	
	return nRet;
}

int CPublic_Function::IO_Board_Initialize()
{
	int i=0,j=0;
	INT HSSI_speed = 0;
	int Ret = 0;
	INT pMasterNo;
	
	if (FAS_IO.mn_simulation_mode == 1)
	{
		st_handler.mn_io_board_initial = TRUE;
		return TRUE;
	}
	/////////////////초기에 초기화시 //////////////////////////////////////////////////////////////////////
	Ret = FAS_IO.Set_IO_BoardOpen(&pMasterNo, START_NOTHING); //START_LOADDATA); //START_NOTHING); //START_CLEAR);
	
	if (Ret != STATUS_SUCCESS)
	{
		return FALSE;
	}
	
	Ret = FAS_IO.Set_IO_PortAutoEnable(0, 0);
	
	if (Ret != CTL_YES)
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "0 Port, not enable");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}

		st_handler.mn_io_board_initial = FALSE;
		return FALSE;
	}

	Ret = FAS_IO.Set_IO_PortAutoEnable(0, 1);

	if (Ret != CTL_YES)
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "1 Port, not enable");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}

		st_handler.mn_io_board_initial = FALSE;
		return FALSE;
	}

	Ret = FAS_IO.Set_IO_PortAutoEnable(0, 2);
	
	if (Ret != CTL_YES)
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "2 Port, not enable");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		st_handler.mn_io_board_initial = FALSE;
		return FALSE;
	}

	Ret = FAS_IO.Set_IO_PortAutoEnable(0, 3);
	
	if (Ret != CTL_YES)
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "3 Port, not enable");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		
		st_handler.mn_io_board_initial = FALSE;
		return FALSE;
	}

	for (i = 0; i < 8; i++)
	{
		Ret = FAS_IO.Set_IO_SlaveEnable( 0, 0, i, TRUE);
		
		if (Ret != CTL_YES)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "Slave #%d, not enable", i);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			i = -1;
			
			if (j < 3)
			{
				j++;
			}
			else
			{
				st_handler.mn_io_board_initial = FALSE;
				return FALSE;
			}
			continue;
		}
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "Slave #%d, enable success", i);
			st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 성공 출력 요청
		}
		Sleep(100);
	}
	for (i = 0; i < 8; i++)
	{
		Ret = FAS_IO.Set_IO_SlaveEnable( 0, 1, i, TRUE);
		
		if (Ret != CTL_YES)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "Slave #%d, not enable", i+10);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			i = -1;
			
			if (j < 3)
			{
				j++;
			}
			else
			{
				st_handler.mn_io_board_initial = FALSE;
				return FALSE;
			}
			continue;
		}
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "Slave #%d, enable success", i+10);
			st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 성공 출력 요청
		}
		Sleep(100);
	}

	for (i = 0; i < 8; i++)
	{
		Ret = FAS_IO.Set_IO_SlaveEnable( 0, 2, i, TRUE);
		
		if (Ret != CTL_YES)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "Slave #%d, not enable",i + 20);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			i = -1;
			
			if (j < 3)
			{
				j++;
			}
			else
			{
				st_handler.mn_io_board_initial = FALSE;
				return FALSE;
			}
			continue;
		}
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "Slave #%d, enable success", i + 20);
			st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 성공 출력 요청
		}
		Sleep(100);
	}

	for (i = 0; i < 8; i++)
	{
		Ret = FAS_IO.Set_IO_SlaveEnable( 0, 3, i, TRUE);
		
		if (Ret != CTL_YES)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "Slave #%d, not enable",i + 30);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			i = -1;
			
			if (j < 3)
			{
				j++;
			}
			else
			{
				st_handler.mn_io_board_initial = FALSE;
				return FALSE;
			}
			continue;
		}
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "Slave #%d, enable success", i + 30);
			st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 성공 출력 요청
		}
		Sleep(100);
	}
	
	for (i = 0; i < 4; i++)
	{
		Ret = FAS_IO.Set_IO_HSSISpeed(0, i, PORT_SPEED_10);
		Ret = FAS_IO.Get_IO_HSSISpeed(0, i, &HSSI_speed);
	}

	FAS_IO.Set_IO_DefineWord(0, 0, 0, 0xffff); // 0번 Master, 0번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 1, 0x0000); // 0번 Master, 0번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 2, 0x0000); // 0번 Master, 0번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 3, 0xffff); // 0번 Master, 0번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 4, 0x0000); // 0번 Master, 1번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 5, 0x0000); // 0번 Master, 1번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 6, 0xffff); // 0번 Master, 1번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 7, 0x0000); // 0번 Master, 1번 포트, 3번 슬레이브 , 16bit 셋팅
	
	FAS_IO.Set_IO_DefineWord(0, 1, 0, 0x0000); // 0번 Master, 1번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 1, 0xffff); // 0번 Master, 1번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 2, 0x0000); // 0번 Master, 1번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 3, 0x0000); // 0번 Master, 2번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 4, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 5, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 6, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 7, 0x001f); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	
	FAS_IO.Set_IO_DefineWord(0, 2, 0, 0x00ff); // 0번 Master, 1번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 1, 0x001f); // 0번 Master, 1번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 2, 0x00ff); // 0번 Master, 1번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 3, 0x00ff); // 0번 Master, 2번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 4, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 5, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 6, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 7, 0x003f); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅

	FAS_IO.Set_IO_DefineWord(0, 3, 0, 0x003f); // 0번 Master, 1번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 1, 0x003f); // 0번 Master, 1번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 2, 0x003f); // 0번 Master, 1번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 3, 0x00ff); // 0번 Master, 2번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 4, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 5, 0x0000); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 6, 0x0000); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 7, 0x003f); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅

	st_handler.mn_io_board_initial = TRUE;
	

	return TRUE;
}

void CPublic_Function::OnSet_IO_Out_Port_Clear()
{
	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_OFF);
	FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_OFF);
	FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
	FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_OFF);
	FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_OFF);
	FAS_IO.set_out_bit(st_io.o_buzzer_off_switch_lamp, IO_OFF);
	
	FAS_IO.set_out_bit(st_io.o_main_air, IO_ON);
	
	if (FAS_IO.get_in_bit(st_io.i_main_air_chk, IO_ON) != IO_ON)
	{
		FAS_IO.set_out_bit(st_io.o_main_air, IO_OFF);
		
		Sleep(300);
		
		FAS_IO.set_out_bit(st_io.o_main_air, IO_ON);
	}
}

void CPublic_Function::OnSet_IO_Port_Stop()
{
	if ( st_timedb.mnTime_Section != dSTOP )
	{
		st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
		st_timedb.mnSectionBuffer = dSTOP;
		
		st_timedb.mole_date		= COleDateTime::GetCurrentTime();
		
		DB_Write_Time(st_timedb);
	}
	st_work.mn_run_status = dSTOP;
	
	st_handler.mn_status =  CTL_NO;					// 장비가 가동중이냐.. 2K9/12/29/ViboX
	
//	FAS_IO.set_out_bit(st_io.o_front_machine_front_request_sginal, CTL_NO);	// 셔틀에서 밀어 넣을까봐 일단 OFF시킨다. 2K10/08/10/ViboX
//	FAS_IO.set_out_bit(st_io.o_front_machine_rear_request_sginal, CTL_NO);	// 셔틀에서 밀어 넣을까봐 일단 OFF시킨다. 2K10/08/10/ViboX

	FAS_IO.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
	FAS_IO.set_out_bit(st_io.o_stop_switch_lamp, IO_ON);
	FAS_IO.set_out_bit(st_io.o_ionizer_onoff, IO_OFF);//20121205		

	// 메뉴얼 모드라면 도어를 연다.
	st_handler.mn_manual = YES;
	if (st_handler.mn_manual == YES)
	{
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_bwd_onoff, IO_OFF);
//		OnSet_Door_Open();
		st_handler.mn_manual = NO;
	}
	if (st_handler.cwnd_title != NULL) st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.mn_run_status);
}

void CPublic_Function::OnSet_Door_Close()
{
	FAS_IO.set_out_bit(st_io.o_lock_door_onoff,	IO_ON);
}

void CPublic_Function::OnSet_Door_Open()
{
	FAS_IO.set_out_bit(st_io.o_lock_door_onoff,	IO_OFF);
}
void CPublic_Function::OnSet_IO_Port_Run()
{
	if (st_handler.mn_selfcheck == CTL_YES)
	{
		st_work.mn_run_status = dSELFCHECK;
	}
	else
	{
		st_work.mn_run_status = dRUN;
		st_work.ml_last_shifter_move_time = 0;
	}

	if ( st_timedb.mnTime_Section != st_timedb.mnSectionBuffer )
	{
		st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
		
		if (st_handler.mn_selfcheck == CTL_YES)
		{
			st_timedb.mnSectionBuffer = dSELFCHECK;
		}
		else
		{
			st_timedb.mnSectionBuffer = dRUN;
		}	
		
		st_timedb.mole_date		= COleDateTime::GetCurrentTime();
		
		DB_Write_Time(st_timedb);
	}
	
	
	//레벨 초기화.
	st_handler.mn_level_teach = FALSE;
	st_handler.mn_level_maint = FALSE;
	st_handler.mn_safety = CTL_YES;						// 안전 장치 작동 2K6/06/09/ViboX
	
	st_handler.mn_status =  CTL_YES;					// 장비가 가동중이냐.. 2K9/12/29/ViboX
	
	FAS_IO.set_out_bit(st_io.o_start_switch_lamp,	IO_ON);
	FAS_IO.set_out_bit(st_io.o_stop_switch_lamp,	IO_OFF);
	FAS_IO.set_out_bit(st_io.o_ionizer_onoff, IO_ON);//20121205

	// 도어를 잠근다.
	st_handler.mn_manual = NO;
	FAS_IO.set_out_bit(st_io.o_lock_door_onoff,				IO_ON);
	
	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, LEVEL_MODE, 0);
	st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MACHINE_STATUS, st_work.mn_run_status);
}

void CPublic_Function::OnSet_IO_Port_Init()
{
	st_work.mn_run_status = dINIT;

	// 도어를 잠근다.
	FAS_IO.set_out_bit(st_io.o_lock_door_onoff,				IO_ON);
}

int CPublic_Function::Handler_Recovery_Data(int n_debug, int n_site, int n_mode)
{
	int i = 0, j = 0;
	//	FILE *fp;
	int nExistFile = 0, nFuncRet = CTLBD_RET_GOOD;
	CString sFileName, sFolderName;

	return nFuncRet;
}

void CPublic_Function::OnSet_IO_Port_Alarm()
{
	if ( st_timedb.mnTime_Section != CTL_dWARNING )
	{
		st_timedb.mnTime_Section	= st_timedb.mnSectionBuffer;
		st_timedb.mnSectionBuffer = CTL_dWARNING;
		
		st_timedb.mole_date			= COleDateTime::GetCurrentTime();		
		DB_Write_Time(st_timedb);
	}
	OnSet_IO_Port_Sound(IO_ON);							// Buzzer를 ON 시킨다.
	
	if (st_work.mn_run_status != dLOTEND)		
	{
		FAS_IO.set_out_bit(st_io.o_alarm_reset_switch_lamp, IO_ON);	// Reset Switch에 Lamp를 On 시킨다.
	}
	
	st_handler.mn_reinstatement = CTL_NO;					// 복구 동작중에 알람이 발생 했을 경우에 복구 동작임을 해제 시킨다.
	
	// 도어를 연다.
	FAS_IO.set_out_bit(st_io.o_lock_door_onoff,	IO_ON);
}

void CPublic_Function::OnSet_IO_Port_Sound(int n_status, int n_opt)
{
	switch (n_status)
	{
	case 0:
		FAS_IO.set_out_bit(st_io.o_buzzer[0],	IO_OFF);	// Buzzer 모드 상관없이 끈다.
		FAS_IO.set_out_bit(st_io.o_buzzer[1],	IO_OFF);	// Buzzer 모드 상관없이 끈다.
//		FAS_IO.set_out_bit(st_io.o_buzzer[2],	IO_OFF);	// Buzzer 모드 상관없이 끈다.
//		FAS_IO.set_out_bit(st_io.o_buzzer[3],	IO_OFF);	// Buzzer 모드 상관없이 끈다.
		FAS_IO.set_out_bit(st_io.o_buzzer_off_switch_lamp, IO_OFF);
		break;
		
	case 1:
		if (st_lamp.mn_buzzer_mode == CTL_YES)			// Buzzer 사용 모드에서만 켠다.
		{
			FAS_IO.set_out_bit(st_io.o_buzzer[n_opt],	IO_ON);
			FAS_IO.set_out_bit(st_io.o_buzzer_off_switch_lamp, IO_ON);
		}
		break;
	}
}

CString CPublic_Function::OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt)
{
	CString strFuncRet;
	int n_title, n_equal, n_end, n_length, n_qw = 0;    
	
	n_title =	strBody.Find(strFind, 0);							// TITLE의 위치를 찾는다.
	n_equal =	strBody.Find("=", n_title + 1);						// 해당 item의 끝 위치를 찾는다.
	
	if (n_title == -1 || n_equal == -1)		return "NULL";
	
	if (nOpt == 0)
	{
		n_end =		strBody.Find(" ", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_equal - 1;							// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if (nOpt == 7)
	{
		strFuncRet = strBody.Mid(n_title + 5);
	}
	else if (nOpt == 2)
	{
		n_end =		strBody.Find("=", n_title + 1);					// 해당 item의 끝 위치를 찾는다.						// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_end + 1);
	}
	else if (nOpt == 3)
	{
		n_end =		strBody.Find("=", n_title + 1);					// 해당 item의 끝 위치를 찾는다.						// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_end + 1, 1);
	}
	else if (nOpt == 4)
	{
		n_qw =		strBody.Find("((", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_end =		strBody.Find("))", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_qw - 1;								// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_qw + 2, n_length - 1);
	}
	else if (nOpt == 5)
	{
		n_qw =		strBody.Find("FAIL_REGNUMBER", n_title + 22);					// 해당 item의 끝 위치를 찾는다.
		n_equal =	strBody.Find("=", n_qw + 1);	
		n_end =		strBody.Find(" ", n_qw + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_equal - 1;							// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if (nOpt == 6)
	{
		n_qw =		strBody.Find("ID", n_title + 6);					// 해당 item의 끝 위치를 찾는다.
		n_equal =	strBody.Find("=", n_qw + 1);	
		n_end =		strBody.Find(" ", n_qw + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_equal - 1;							// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if ( nOpt == 8)
	{
		n_qw =		strBody.Find("\"", n_equal + 1 );
		n_end =		strBody.Find("\"", n_equal + 2 );
		n_length =	n_end - n_qw - 1;
		strFuncRet = strBody.Mid(n_qw + 1, n_length);
	}
	else
	{
		n_qw =		strBody.Find("(", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_end =		strBody.Find(")", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_qw - 1;								// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_qw + 1, n_length);
	}
	
	//	strFuncRet.MakeUpper(); //20120826
	strFuncRet.TrimLeft(' ');
	strFuncRet.TrimRight(' ');

	return strFuncRet;
}

CString CPublic_Function::OnNetworkDataMake(int nID)
{
	CString strHeader;
	CString strBody;
	int nBody = 0, i;

	CString strTemp;

	strBody = "";
	switch (nID)
	{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case NEW_LOT_START:											// 20130115
		st_NW.mstr_function[0]		= "LOT_START";
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_oper_id[0]		= st_work.mstr_op_id;
		
		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOTNO=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "OPER_ID=" + st_NW.mstr_oper_id[0];// + " ";
		break;

	case NEW_LOT_END:											// 20130115
		st_NW.mstr_function[0]		= "LOT_END";
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_oper_id[0]		= st_work.mstr_op_id;

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOTNO=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "OPER_ID=" + st_NW.mstr_oper_id[0];// + " ";
		
		if (st_sync.mn_add_fail != CTL_YES)
		{
			if (st_work.strMdlReject.GetCount() > 0)
			{
				strTemp.Format(" SCRAP_CNT=%d", st_work.strMdlReject.GetCount());
				strBody		+= strTemp;
				
				for (i = 0; i < st_work.strMdlReject.GetCount(); i++)
				{
					if (st_work.strMdlReject.GetAt(st_work.strMdlReject.FindIndex(i)).GetLength() < 10)
					{
						strTemp.Format(" SCRAP_INFO%03d=(SCRAP_CODE=%s SCRAP_QTY=%s)", i + 1, st_work.strMdlRejectCode.GetAt(st_work.strMdlRejectCode.FindIndex(i)), st_handler.mstr_lot_name[0] + st_work.strMdlReject.GetAt(st_work.strMdlReject.FindIndex(i))); 
					}
					else
					{
						strTemp.Format(" SCRAP_INFO%03d=(SCRAP_CODE=%s SCRAP_QTY=%s)", i + 1, st_work.strMdlRejectCode.GetAt(st_work.strMdlRejectCode.FindIndex(i)), st_work.strMdlReject.GetAt(st_work.strMdlReject.FindIndex(i))); 
					}
					strBody		+= strTemp;
				}
			}
		}
		else
		{			
			if (st_work.strMdlRejectAdd.GetCount() > 0)
			{
				strTemp.Format(" SCRAP_CNT=%d", st_work.strMdlRejectAdd.GetCount());
				strBody		+= strTemp;
				
				for (i = 0; i < st_work.strMdlRejectAdd.GetCount(); i++)
				{
					if (st_work.strMdlRejectAdd.GetAt(st_work.strMdlRejectAdd.FindIndex(i)).GetLength() < 10)
					{
						strTemp.Format(" SCRAP_INFO%03d=(SCRAP_CODE=%s SCRAP_QTY=%s)", i + 1, st_work.strMdlRejectAddCode.GetAt(st_work.strMdlRejectAddCode.FindIndex(i)), st_handler.mstr_lot_name[0] + st_work.strMdlRejectAdd.GetAt(st_work.strMdlRejectAdd.FindIndex(i))); 
					}
					else
					{
						strTemp.Format(" SCRAP_INFO%03d=(SCRAP_CODE=%s SCRAP_QTY=%s)", i + 1, st_work.strMdlRejectAddCode.GetAt(st_work.strMdlRejectAddCode.FindIndex(i)), st_work.strMdlRejectAdd.GetAt(st_work.strMdlRejectAdd.FindIndex(i))); 
					}
					strBody		+= strTemp;
				}
			}
		}
		break;

	case NEW_LOT_CANCEL:							// 20130115
		st_NW.mstr_function[0]		= "LOT_CANCEL";
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_oper_id[0]		= st_work.mstr_op_id;
		
		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOTNO=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "OPER_ID=" + st_NW.mstr_oper_id[0];
		break;	

	case NEW_MAT_CHECK:								// 20130115
//FMATS=(LOTID=A2K08300600 CODE=LA61-00036A, VEND=B4FW;LOTID=A3K12290300,CODE=LA61-00036A,VEND=B4FW)
//BMATS=(LOTID=test,CODE=LA61-00036A,VEND=B4FW)
		st_NW.mstr_function[0]		= "MAT_CHECK";
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_oper_id[0]		= st_work.mstr_op_id;
		st_NW.mstr_fmats[0]			= "(LOTID=A2K08300600,CODE=LA61-00036A,VEND=B4FW;LOTID=A3K12290300,CODE=LA61-00036A,VEND=B4FW)";
		st_NW.mstr_bmats[0]			= "(LOTID=test,CODE=LA61-00036A,VEND=B4FW)";

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOTNO=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "OPER_ID=" + st_NW.mstr_oper_id[0];
		strBody     += "FMATS=" + st_NW.mstr_fmats[0] + " ";
		strBody     += "BMATS=" + st_NW.mstr_bmats[0] + " ";
		
// 		if (st_sync.mn_add_fail != CTL_YES)
// 		{
// 			if (st_work.strMdlReject.GetCount() > 0)
// 			{
// 				strTemp.Format(" SCRAP_CNT=%d", st_work.strMdlReject.GetCount());
// 				strBody		+= strTemp;
		break;	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	case NW_LOT_START:
		st_NW.mstr_function[0]		= "LOT_START";
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_oper_id[0]		= st_work.mstr_op_id;

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "LOTNO=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "OPER_ID=" + st_NW.mstr_oper_id[0];// + " ";
		break;

	case NW_LOT_END:
		st_NW.mstr_function[0]		= "LOT_END";
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_lot_id[0]			= st_work.mstr_lot_no;
		st_NW.mstr_oper_id[0]			= st_work.mstr_op_id;
		st_NW.mstr_scrap_cnt[0]			= st_work.mstr_scrap_cnt;
		st_NW.mstr_scrap_info[0]		= st_work.mstr_scrap_info;

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "LOTNO=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "OPERID=" + st_NW.mstr_oper_id[0] + " ";
		strBody		+= "SCRAP_CNT=" + st_NW.mstr_scrap_cnt[0] + " ";
		strBody		+= "SCRAP_INFO=" + st_NW.mstr_scrap_info[0];
		break;

	case NW_LOT_CANCEL:
		st_NW.mstr_function[0]		= "LOT_CANCEL";
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_oper_id[0]		= st_work.mstr_op_id;

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "OPER_ID=" + st_NW.mstr_oper_id[0];
		break;






	case NW_SCRAP_INFO:
		st_NW.mstr_function[0]		= "SCRAP_INFO";
		st_NW.mstr_lot_id[0]		= st_handler.mstr_lot_name[0];
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_oper_id[0]		= st_work.mstr_op_id;


		strBody		= "FUNCTION=" + st_NW.mstr_function[0];
		strBody		+= " LOTNO=" + st_NW.mstr_lot_id[0];
		strBody		+= " EQPID=" + st_NW.mstr_eqp_id[0];
		break;

// 여기서부터는 장비에서 답변을 주는 부분이다.
	case NW_ONLINE_END:		
		strBody		= "FUNCTION=ONLINE_END";
		strBody		+= " LOTID=" + st_handler.mstr_lot_name[0];
		strBody		+= " EQPID=" + st_lamp.mstr_equip_id;
		strBody		+= " OPERID=06653301";// + st_NW.mstr_oper_id[0];

		if (st_sync.mn_add_fail != CTL_YES)
		{
			if (st_work.strMdlReject.GetCount() > 0)
			{
				strTemp.Format(" SCRAP_CNT=%d", st_work.strMdlReject.GetCount());
				strBody		+= strTemp;

				for (i = 0; i < st_work.strMdlReject.GetCount(); i++)
				{
					if (st_work.strMdlReject.GetAt(st_work.strMdlReject.FindIndex(i)).GetLength() < 10)
					{
						strTemp.Format(" SCRAP_INFO%03d=(SCRAP_CODE=%s SERIAL=%s)", i + 1, st_work.strMdlRejectCode.GetAt(st_work.strMdlRejectCode.FindIndex(i)), st_handler.mstr_lot_name[0] + st_work.strMdlReject.GetAt(st_work.strMdlReject.FindIndex(i))); 
					}
					else
					{
						strTemp.Format(" SCRAP_INFO%03d=(SCRAP_CODE=%s SERIAL=%s)", i + 1, st_work.strMdlRejectCode.GetAt(st_work.strMdlRejectCode.FindIndex(i)), st_work.strMdlReject.GetAt(st_work.strMdlReject.FindIndex(i))); 
					}
					strBody		+= strTemp;
				}
			}
		}
		else
		{			
			if (st_work.strMdlRejectAdd.GetCount() > 0)
			{
				strTemp.Format(" SCRAP_CNT=%d", st_work.strMdlRejectAdd.GetCount());
				strBody		+= strTemp;

				for (i = 0; i < st_work.strMdlRejectAdd.GetCount(); i++)
				{
					if (st_work.strMdlRejectAdd.GetAt(st_work.strMdlRejectAdd.FindIndex(i)).GetLength() < 10)
					{
						strTemp.Format(" SCRAP_INFO%03d=(SCRAP_CODE=%s SERIAL=%s)", i + 1, st_work.strMdlRejectAddCode.GetAt(st_work.strMdlRejectAddCode.FindIndex(i)), st_handler.mstr_lot_name[0] + st_work.strMdlRejectAdd.GetAt(st_work.strMdlRejectAdd.FindIndex(i))); 
					}
					else
					{
						strTemp.Format(" SCRAP_INFO%03d=(SCRAP_CODE=%s SERIAL=%s)", i + 1, st_work.strMdlRejectAddCode.GetAt(st_work.strMdlRejectAddCode.FindIndex(i)), st_work.strMdlRejectAdd.GetAt(st_work.strMdlRejectAdd.FindIndex(i))); 
					}
					strBody		+= strTemp;
				}
			}
		}
		break;

	case NW_DELETE_SCRAP:
		st_NW.mstr_function[0]		= "DELETE_SCRAP";
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_serial[0]		= st_work.mstr_delete_serial;
		
		strBody		= "FUNCTION=" + st_NW.mstr_function[0];
		strBody		+= " LOTNO=" + st_NW.mstr_lot_id[0];
		strBody		+= " SERIAL=" + st_NW.mstr_serial[0];
		break;

	case NW_SORTER_LOT_END:
		st_NW.mstr_function[0]		= "SORTER_LOT_END";
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		
		strBody		= "FUNCTION=" + st_NW.mstr_function[0];
		strBody		+= " LOTNO=" + st_NW.mstr_lot_id[0];
		strBody		+= " EQPID=" + st_NW.mstr_eqp_id[0];
		break;

	

	case NW_LABEL_SAMPLE_INFO:
		st_NW.mstr_function[0]		= "LABEL_SAMPLE_INFO";
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_oper_id[0]		= st_work.mstr_op_id;
//		st_NW.mstr_result[0]		= st_NW.mstr_result[1];

		strBody		= "FUNCTION_RPY=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "OPERID=" + st_NW.mstr_oper_id[0];// + " ";
//		strBody		+= "RESULT=" + st_NW.mstr_result[0];
		break;
	
	case NW_NEW_LOT_IN_SEND:			// 0000000000000050 FUNCTION=NEW_LOT_IN LOT_NO=TEST_LOT EQP_ID=IR8130
		strBody		= "FUNCTION=NEW_LOT_IN";
		strBody		+= " LOTNO=" + st_NW.mstr_new_lot_no[1];
		strBody		+= " EQPID=" + st_lamp.mstr_equip_id;
		break;

	case NW_LOT_NAME:					// 0000000000000050 FUNCTION=LOT_NAME LOT_NO=TEST_LOT
		strBody		= "FUNCTION=LOT_NAME";
		strBody		+= " LOT_NO=" + st_NW.mstr_m_lot_no[1];
		break;

	case NW_LOT_START_ENABLE:
		st_NW.mstr_function[0]		= "LOT_START_ENABLE";
		st_NW.mstr_lot_id[0]		= st_NW.mstr_lot_id[1];
		st_NW.mstr_eqp_id[0]		= st_NW.mstr_eqp_id[1];
		st_NW.mstr_step_cnt[0]		= st_NW.mstr_step_cnt[1];
		st_NW.mstr_result[0]		= st_NW.mstr_result[1];
		st_NW.mstr_fail_log[0]		= st_NW.mstr_fail_log[1];

		strBody		= "FUNCTION_RPY=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "STEP_COUNT" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "RESULT=PASS ";
		strBody		+= "FAIL_LOG" + st_NW.mstr_fail_log[0];
		break;
// 여기까지는 장비에서 답변을 주는 부분이다.


// 여기서부터는 장비가 질문을 하거나 통보를 하는 부분이다.
	case NW_TRACK_IN:		
		strBody		= "FUNCTION=TRACK_IN";
		strBody		+= " LOT_NO=" + st_NW.mstr_lot_no[0];
		strBody		+= " EQP_ID=" + st_lamp.mstr_equip_id;
		strBody		+= " STATION=" + st_NW.mstr_m_station[0];
		strBody		+= " MODE=" + st_NW.mstr_m_mode[0];
		strBody		+= " LAST_MODULE=" + st_NW.mstr_m_last_module[0];
		strBody		+= " MDL_CNT=" + st_NW.mstr_m_module_cnt[0];

		for (i = 0; i < 20; i++)
		{
			strTemp.Format(" MDL_%02d=%s", i + 1, st_NW.mstr_m_module_data[i][0]);
			strBody		+= strTemp;
		}
		break;

	case NW_BPC_BUFFER_READY:
		strBody		= "FUNCTION=BUFFER_READY ";
		strBody	   += st_NW.mstr_bpc_buffer_ready;
		break;

	case NW_BPC_BUFFER_IN:
		strBody		= "FUNCTION=BUFFER_IN ";
		strBody	   += st_NW.mstr_bpc_buffer_in;
		break;

	case NW_TRACK_OUT:
		st_NW.mstr_function[0]		= "SORTER_LOT_END";
		st_NW.mstr_lot_no[0]		= st_handler.mstr_lot_name[0];
		
		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOTNO=" + st_NW.mstr_lot_no[0];
		strBody		+= " EQPID=" + st_lamp.mstr_equip_id;
		break;

	case NW_LOT_CANCEL_SEND:
		st_NW.mstr_function[0]		= "LOT_CANCEL";
		st_NW.mstr_lot_no[0]		= st_work.mstr_lot_no;
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		
		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOT_NO=" + st_NW.mstr_lot_no[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0];
		break;
	// 새로 추가됨. 파워를 꺼도 좋냐는 질문은 이걸로 한다. 2K11/03/02/ViboX

	case NW_UNLOAD_COMPT:
		st_NW.mstr_function[0]		= "UNLOAD_COMPT";
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_pcbox_id[0]		= st_work.mstr_pcbox_id;

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "PCBOX_ID=" + st_NW.mstr_pcbox_id[0];
		break;
	case NW_CANCEL_END:
		st_NW.mstr_function[0]		= "CANCEL_END";
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_oper_id[0]		= st_work.mstr_op_id;
		st_NW.mstr_pcbox_cnt[0]		= st_work.mstr_pcbox_cnt;
		st_NW.mstr_pcbox_info[0]	= st_work.mstr_pcbox_info;

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "OPER_ID=" + st_NW.mstr_oper_id[0] + " ";
		strBody		+= "PCBOX_CNT" + st_NW.mstr_pcbox_cnt[0] + " ";
		strBody		+= "PCBOX_INFO" + st_NW.mstr_pcbox_info[0];
		break;



	case NW_FAIL_INFO_SEND:
		st_NW.mstr_function[0]		= "FAIL_INFO_SEND";
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_step_cnt[0]		= st_work.mstr_step_no;
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_fail_no[0]		= st_NW.mstr_fail_no[0];
		st_NW.mstr_pcbox_id[0]		= st_work.mstr_e_pcbox_id;
		st_NW.mstr_fail_slot[0]		= st_NW.mstr_fail_slot[0];
		st_NW.mstr_fail_type[0]		= st_NW.mstr_fail_type[0];
		st_NW.mstr_fail_log[0]		= st_NW.mstr_fail_log[0];
		st_NW.mstr_retest_no[0]		= st_NW.mstr_retest_no[0];
		st_NW.mstr_retest_id[0]		= st_NW.mstr_retest_id[0];

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_COUNT" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "FAIL_NO" + st_NW.mstr_fail_no[0] + " ";
		strBody		+= "PCBOX_ID" + st_NW.mstr_pcbox_id[0] + " ";
		strBody		+= "FAIL_SLOT" + st_NW.mstr_fail_slot[0] + " ";
		strBody		+= "FAIL_TYPE" + st_NW.mstr_fail_type[0] + " ";
		strBody		+= "FAIL_LOG" + st_NW.mstr_fail_log[0] + " ";
		strBody		+= "RETEST_NO" + st_NW.mstr_retest_no[0] + " ";
		strBody		+= "RETEST_ID" + st_NW.mstr_retest_id[0];
		break;
	case NW_FAIL_REGNUMBER_REQUEST:
		st_NW.mstr_function[0]		= "FAIL_REGNUMBER_REQUEST";
		st_NW.mstr_pcbox_id[0]		= st_NW.mstr_pcbox_id[1];
		st_NW.mstr_lot_id[0]		= st_NW.mstr_lot_id[1];
		st_NW.mstr_step_cnt[0]		= st_NW.mstr_step_cnt[1];
		st_NW.mstr_slot_no[0]		= st_NW.mstr_slot_no[1];
		st_NW.mstr_test_idx[0]		= st_NW.mstr_test_idx[1];

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "PCBOX_ID=" + st_NW.mstr_pcbox_id[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_CNT=" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "SLOT_NO=" + st_NW.mstr_slot_no[0] + " ";
		strBody		+= "TEST_IDX=" + st_NW.mstr_test_idx[0];
		break;
	case NW_FAIL_INFO_SERACH:
		st_NW.mstr_function[0]			= "FAIL_INFO_SERACH";
		st_NW.mstr_lot_id[0]			= st_work.mstr_lot_no;
		st_NW.mstr_step_cnt[0]			= st_work.mstr_step_no;
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_fail_regnumber[0]	= st_NW.mstr_fail_regnumber[0];

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_COUNT" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "FAIL_REGNUMBER" + st_NW.mstr_fail_regnumber[0];
		break;
	case NW_FAIL_INFO_SAVE:
		st_NW.mstr_function[0]			= "FAIL_INFO_SAVE";
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_lot_id[0]			= st_NW.mstr_lot_id[1];
		st_NW.mstr_step_cnt[0]			= st_NW.mstr_step_cnt[1];
		st_NW.mstr_fail_regnumber[0]	= st_NW.mstr_fail_regnumber[1];
		st_NW.mstr_pcbox_id[0]			= st_NW.mstr_pcbox_id[1];
		st_NW.mstr_slot_no[0]			= st_NW.mstr_slot_no[1];
		st_NW.mstr_test_idx[0]			= st_NW.mstr_test_idx[1];
		st_NW.mstr_fail_desc[0]			= st_NW.mstr_fail_desc[1];
		st_NW.mstr_fail_code[0]			= st_NW.mstr_fail_code[1];
		st_NW.mstr_fail_cnt[0]			= st_NW.mstr_fail_cnt[1];
		st_NW.mstr_retest_flag[0]		= st_NW.mstr_retest_flag[1];

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_CNT=" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "FAIL_REGNUMBER=" + st_NW.mstr_fail_regnumber[0] + " ";
		strBody		+= "PCBOX_ID=" + st_NW.mstr_pcbox_id[0] + " ";
		strBody		+= "SLOT_NO=" + st_NW.mstr_slot_no[0] + " ";
		strBody		+= "TEST_IDX=" + st_NW.mstr_test_idx[0] + " ";
		strBody		+= "FAIL_DESC=" + st_NW.mstr_fail_desc[0] + " ";
		strBody		+= "FAIL_CODE=" + st_NW.mstr_fail_code[0] + " ";
		strBody		+= "FAIL_CNT=" + st_NW.mstr_fail_cnt[0] + " ";
		strBody		+= "RETEST_FLAG=" + st_NW.mstr_retest_flag[0];
		break;
		
	case NW_FAIL_INFO_DELETE:
		st_NW.mstr_function[0]			= "FAIL_INFO_DELETE";
		st_NW.mstr_lot_id[0]			= st_NW.mstr_lot_id[1];
		st_NW.mstr_step_cnt[0]			= st_NW.mstr_step_cnt[1];
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_fail_regnumber[0]	= st_NW.mstr_fail_regnumber[1];

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_COUNT=" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "FAIL_REGNUMBER=" + st_NW.mstr_fail_regnumber[0];
		break;

	case NW_LABEL_INFO:
		st_NW.mstr_function[0]			= "LABEL_NEW_INFO";//= "LABEL_INFO";//
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_lot_id[0]			= st_work.mstr_lot_no;
		//st_NW.mstr_part_id[0]			= st_work.mstr_part_no;
		st_NW.mstr_oper_id[0]			= st_work.mstr_op_id;
		st_NW.mstr_format_retry[0]		= st_work.mstr_format_retry = _T("10");

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "LOTNO=" + st_NW.mstr_lot_id[0] + " ";
		//strBody		+= "PARTID=" + st_NW.mstr_part_id[0] + " ";
		strBody		+= "OPERID=" + st_NW.mstr_oper_id[0] + " ";
		//strBody		+= "RETRY=" + st_NW.mstr_format_retry[0];

		strBody		+= "LABEL_QTY=" + st_work.mstr_format_retry;
		break;

	case NW_LABEL_FAIL_INFO_SEND: ///보내주지 않아도 된다
		st_NW.mstr_function[0]		= "LABEL_FAIL_INFO";
		st_NW.mstr_eqp_id[0]		= st_lamp.mstr_equip_id;
		st_NW.mstr_lot_id[0]		= st_work.mstr_lot_no;
		st_NW.mstr_oper_id[0]		= st_work.mstr_op_id;
		st_NW.mstr_fail_no[0]		= st_NW.mstr_fail_no[0];
		st_NW.mstr_result[1]		= st_NW.mstr_result[0];
		//st_work.n_label_info_fail_qty;

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "LOTNO=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "OPERID=" + st_work.mstr_oper_info_check + " ";
		strBody		+= "RESULT" + st_NW.mstr_result[0] + " ";
		strBody		+= "MSG" + st_NW.mstr_pcbox_id[0] + " ";
		strBody		+= "FAIL_SLOT" + st_NW.mstr_fail_slot[0] + " ";
		strBody		+= "FAIL_TYPE" + st_NW.mstr_fail_type[0] + " ";
		strBody		+= "FAIL_LOG" + st_NW.mstr_fail_log[0] + " ";
		strBody		+= "RETEST_NO" + st_NW.mstr_retest_no[0] + " ";
		strBody		+= "RETEST_ID" + st_NW.mstr_retest_id[0];
		break;
	
	
	case NW_LOT_DISPLAY:
		st_NW.mstr_function[0]			= "LOTDISPLAY";
		st_NW.mstr_lot_id[0]			= st_work.mstr_lot_no_check;
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_oper_id[0]			= st_work.mstr_oper_info_check;

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOTID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "EQPID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "OPERID=" + st_NW.mstr_oper_id[0];
		break;

	case NW_EQP_LOT_START:
		st_NW.mstr_function[0]			= "EQP_LOT_START";
		st_NW.mstr_lot_id[0]			= st_work.mstr_lot_no_check;
		st_NW.mstr_step_cnt[0]			= st_work.mstr_step_no_check;
		st_NW.mstr_lot_qty[0]			= st_work.mstr_lot_qty_check;
		st_NW.mstr_cycle_id[0]			= st_work.mstr_cycle_id_check;
		st_NW.mstr_part_id[0]			= st_work.mstr_part_id_check;
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_oper_cnt[0]			= st_work.mstr_oper_cnt_check;

		st_NW.mstr_oper_info[0] = "";
		for (i = 0; i < atoi(st_NW.mstr_oper_cnt[0]); i++)
		{
			strTemp.Format("OPER_%02d=", i + 1);
			st_NW.mstr_oper_info[0] += strTemp;
			st_NW.mstr_oper_info[0] += st_work.mstr_oper_info_check;
			
			if (i != atoi(st_NW.mstr_oper_info[0]) - 1)
			{
				st_NW.mstr_oper_info[0] += " ";
			}
		}

		st_NW.mstr_pcbox_cnt[0]			= st_work.mstr_pcbox_cnt_check;

		st_NW.mstr_pcbox_info[0] = "";
		for (i = 0; i < atoi(st_NW.mstr_pcbox_cnt[0]); i++)
		{
			strTemp.Format("PCBOX_ID_%03d=", i + 1);
			st_NW.mstr_pcbox_info[0] += strTemp;
			st_NW.mstr_pcbox_info[0] += st_NW.mstr_pcbox[1][i];

			if (i != atoi(st_NW.mstr_pcbox_cnt[0]) - 1)
			{
				st_NW.mstr_pcbox_info[0] += " ";
			}
		}

		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_COUNT=" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "LOT_QTY=" + st_NW.mstr_lot_qty[0] + " ";
		strBody		+= "CYCLE_ID=" + st_NW.mstr_cycle_id[0] + " ";
		strBody		+= "PART_ID=" + st_NW.mstr_part_id[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "OPER_CNT=" + st_NW.mstr_oper_cnt[0] + " ";
		strBody		+= "OPER_INFO=(" + st_NW.mstr_oper_info[0] + ") ";
		strBody		+= "PCBOX_CNT=" + st_NW.mstr_pcbox_cnt[0] + " ";
		strBody		+= "PCBOX_INFO=(" + st_NW.mstr_pcbox_info[0] + ")";
		break;

	case NW_EQP_LOT_CANCEL:
		st_NW.mstr_function[0]			= "EQP_LOT_CANCEL";
		st_NW.mstr_lot_id[0]			= st_work.mstr_lot_no_check;
		st_NW.mstr_step_cnt[0]			= st_work.mstr_step_no_check;
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_mode[0]				= st_work.mstr_mode;
		st_NW.mstr_oper_id[0]			= st_work.mstr_oper_info_check;

		st_NW.mstr_pcbox_cnt[0]			= st_work.mstr_pcbox_cnt_check;
		
		st_NW.mstr_pcbox_info[0] = "";
		for (i = 0; i < atoi(st_NW.mstr_pcbox_cnt[0]); i++)
		{
			strTemp.Format("PCBOX_ID_%03d=", i + 1);
			st_NW.mstr_pcbox_info[0] += strTemp;
			st_NW.mstr_pcbox_info[0] += st_NW.mstr_pcbox[1][i];
			
			if (i != atoi(st_NW.mstr_pcbox_cnt[0]) - 1)
			{
				st_NW.mstr_pcbox_info[0] += " ";
			}
		}
		
		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_COUNT=" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "MODE=AUTO ";
		strBody		+= "OPER_ID=" + st_NW.mstr_oper_id[0] + " ";
		strBody		+= "PCBOX_CNT=" + st_NW.mstr_pcbox_cnt[0] + " ";
		strBody		+= "PCBOX_INFO=(" + st_NW.mstr_pcbox_info[0] + ")";
		break;

	case NW_EQP_LOT_END:
		st_NW.mstr_function[0]			= "EQP_LOT_END";
		st_NW.mstr_lot_id[0]			= st_work.mstr_lot_no_check;
		st_NW.mstr_step_cnt[0]			= st_work.mstr_step_no_check;
		st_NW.mstr_lot_qty[0]			= st_work.mstr_lot_qty_check;
		st_NW.mstr_eqp_id[0]			= st_lamp.mstr_equip_id;
		st_NW.mstr_part_id[0]			= st_work.mstr_part_id_check;
		st_NW.mstr_oper_id[0]			= st_work.mstr_oper_info_check;
		st_NW.mstr_bin_info[0]			= st_work.mstr_bin_info;
		st_NW.mstr_scrap_cnt[0]			= st_work.mstr_scrap_cnt;
		st_NW.mstr_scrap_info[0]		= st_work.mstr_scrap_info;
		
		strBody		= "FUNCTION=" + st_NW.mstr_function[0] + " ";
		strBody		+= "LOT_ID=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "STEP_COUNT=" + st_NW.mstr_step_cnt[0] + " ";
		strBody		+= "LOT_QTY=" + st_NW.mstr_lot_qty[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "PART_ID=" + st_NW.mstr_part_id[0] + " ";
		strBody		+= "MODE=AUTO ";
		strBody		+= "OPER_ID=" + st_NW.mstr_oper_id[0] + " ";
		strBody		+= "BIN_INFO=" + st_NW.mstr_scrap_cnt[0] + " ";
		strBody		+= "SCRAP_CNT=" + st_NW.mstr_scrap_cnt[0] + " ";
		strBody		+= "SCRAP_INFO=" + st_NW.mstr_scrap_info[0];
		break;
	case NW_PGM_START:
		st_NW.mstr_function[0]		= "PGM_START";
		st_NW.mstr_lot_id[0] = st_NW.mstr_lot_no[1];
		st_NW.mstr_eqp_id[0] = st_lamp.mstr_equip_id;
		st_NW.mstr_slot_no[0] = st_NW.mstr_slot_no[1];
		st_NW.mstr_pcbox_id[0] = st_NW.mstr_pcbox_id[1];
		st_NW.mstr_result[0] = st_NW.mstr_test_result[1];
		st_NW.mstr_error_log[0] = st_NW.mstr_error_log[1];
		
		strBody		= "FUNCTION_RPY=" + st_NW.mstr_function[0] + " ";
		strBody		+= "RESULT=PASS ";
		strBody		+= "LOT_NO=" + st_NW.mstr_lot_id[0] + " ";
		strBody		+= "EQP_ID=" + st_NW.mstr_eqp_id[0] + " ";
		strBody		+= "PCBOX_ID=" + st_NW.mstr_pcbox_id[0];
		break;
// 여기까지는 장비가 질문을 하거나 통보를 하는 부분이다.

	case NW_BUFFER_IN_SEND:
		strBody		= "FUNCTION=BUFFER_IN";
		strBody		+= " LOT_NO=" + st_NW.mstr_m_lot_no[1];
		strBody		+= " EQP_ID=" + st_lamp.mstr_equip_id;
		strBody		+= " BUFFER_NO=" + st_NW.mstr_m_buffer_no[1];
		strBody		+= " PCT_BYPASS=" + st_NW.mstr_pct_bypass[1];
		strBody		+= " MDL_CNT=" + st_NW.mstr_m_module_cnt[1];

		for (i = 0; i < 20; i++)
		{
			strTemp.Format(" MDL_%02d=%s", i + 1, st_NW.mstr_m_module_data[i][1]);

			if (st_handler.mn_machine_pos == 0 || st_handler.mn_reverse_mode == 0 )
			{
				strTemp.Format(" MDL_%02d=%s", i + 1, st_NW.mstr_m_module_data[i][1]);
			}
			else
			{
				strTemp.Format(" MDL_%02d=%s", i + 1, st_NW.mstr_m_module_data[19 - i][1]);
			}

			strBody		+= strTemp;
		}
		break;

	case NW_BUFFER_READY_SEND:
		strBody		= "FUNCTION=BUFFER_READY";
		strBody		+= " LOT_NO=" + st_NW.mstr_m_lot_no[1];
		strBody		+= " PART_NO=" + st_NW.mstr_m_part_no[1];
		strBody		+= " EQP_ID=" + st_lamp.mstr_equip_id;
		strBody		+= " BUFFER_NO=" + st_NW.mstr_m_buffer_no[1];
		strBody		+= " LAST_MODULE=" + st_NW.mstr_m_last_module[1];
		strBody		+= " BYPASS=" + st_NW.mstr_m_bypass[1];
		strBody		+= " RETEST=" + st_NW.mstr_m_retestcheck[1];				// 새로 추가 됐음.

		{
			CString strTemp;
			strTemp.Format("[LAST MODULE] NW_BUFFER_READY_SEND	LOT:[%s]	LASTMODULE=[%s]", st_NW.mstr_m_lot_no[1], st_NW.mstr_m_last_module[1] );
			On_LogFile_Add(3, strTemp);
		}

		break;

	case NW_RETEST_REQ:
		strBody		= "FUNCTION=RETEST_REQ";
		strBody		+= " LOT_NO=" + st_NW.mstr_m_lot_no[1];
		strBody		+= " BUFFER_NO=" + st_NW.mstr_m_buffer_no[1];
		strBody		+= " SOCKET_OFF=" + st_NW.mstr_m_socket_off[1];
		break;

	case NW_APCTSOCKETINFO:
		strBody		= "FUNCTION=APCTSOCKETINFO";
		strBody		+= " STATION=" + st_NW.mstr_m_station[1];
		strBody		+= " BUFFER_NO=" + st_NW.mstr_m_buffer_no[1];
		break;

	case NW_BLOT_COUNT:
		strBody		= "FUNCTION=BLOT_COUNT";
		break;
	}

	nBody = strBody.GetLength();

	if (nBody > 0)
	{
		if (nID == NW_BUFFER_READY_SEND || nID == NW_BUFFER_IN_SEND || nID == NW_RETEST_REQ || nID == NW_APCTSOCKETINFO
			 || nID == NW_NEW_LOT_IN_SEND || nID == NW_LOT_NAME || nID == NW_LOT_DISPLAY || nID == NW_SCRAP_INFO || nID == NW_ONLINE_END || nID == NW_TRACK_OUT
			 || nID == NW_BPC_BUFFER_READY ||  nID == NW_BPC_BUFFER_IN || nID == NW_DELETE_SCRAP || nID == NW_LOT_START || nID == NW_LABEL_INFO)
		{
			strHeader.Format("%d", nBody);
			int	a = strHeader.GetLength();
			for (int i = 0; i < 16-a; i++)
			{
				strHeader += " ";
			}
		}
		else
		{
			strHeader.Format("%010d", nBody);
		}

		st_NW.mstr_header_msg[0] = strHeader;
		st_NW.mstr_data_msg[0] = strBody;
		st_NW.mstr_full_msg[0] = strHeader + strBody;
//		st_NW.mstr_full_msg[0] = strBody;
	}
	else
	{
		st_NW.mstr_header_msg[0] = "";
		st_NW.mstr_data_msg[0] = "";
		st_NW.mstr_full_msg[0] = "";
	}

	return st_NW.mstr_full_msg[0];
}

CString CPublic_Function::OnDeviceInfoAnalysis(CString strBody, int nInfo)
{
	CString strFuncRet = "NONE";
	int nLength;

	int n_start[5], n_check[5], n_length[5];    

	nLength = strBody.GetLength();
	if (nLength < 23)		return "FAIL";		// 길이가 22보다 짧으면 FAIL이다.

	n_start[0] = 0;
	n_check[0] = strBody.Find("-", n_start[0]);

	if (n_check[0] != -1)
	{
		n_length[0] = n_check[0];
	}
	else
	{
		n_length[0] = 10;
		n_check[0] = 9;
	}

	n_start[1] = n_check[0] + 1;
	n_check[1] = strBody.Find("_", n_start[1]);
	n_length[1] = n_check[1] - n_start[1];

	n_start[2] = n_check[1] + 1;
	n_check[2] = strBody.Find("_", n_start[2]);
	n_length[2] = n_check[2] - n_start[2];

	n_start[3] = n_check[2] + 1;
	n_check[3] = strBody.Find("_", n_start[3]);
	n_length[3] = n_check[3] - n_start[3];

	n_start[4] = n_check[3] + 1;
	n_check[4] = strBody.Find("_", n_start[4]);
	n_length[4] = n_check[4] - n_start[4];

	if (nInfo < 4)
	{
		strFuncRet = strBody.Mid(n_start[nInfo], n_length[nInfo]);
	}
	else
	{
		strFuncRet = strBody.Mid(n_start[nInfo]);
	}
	return strFuncRet;
}

int CPublic_Function::FileSizeCheck(CString FileName, long size, int n_check)
{
	CFileFind finder;
	long Len=0, flag=0;
	BOOL bContinue;
	
	if (bContinue = finder.FindFile(FileName))
	{	
		if (n_check == CTL_YES)			// Size를 체크하는것이면...
		{
			finder.FindFile(FileName);
			finder.FindNextFile();
			finder.GetFileName();
			Len = finder.GetLength();
			if (Len > size)
			{
				flag = 1;
			}
			finder.Close();	
		}
		else						// Size를 확인하는것이면...
		{
			finder.FindFile(FileName);
			finder.FindNextFile();
			finder.GetFileName();
			Len = finder.GetLength();
			
			flag = Len;
			
			finder.Close();	
		}
	}
	
	return flag;
}

CString CPublic_Function::GetLocalMacAddress()
{
	NCB Ncb; 
	UCHAR uRetCode; 
	LANA_ENUM lenum; 
	int i; 
	CString str_value; 
	
	memset( &Ncb, 0, sizeof(Ncb) ); 
	Ncb.ncb_command = NCBENUM; 
	Ncb.ncb_buffer = (UCHAR *)&lenum; 
	Ncb.ncb_length = sizeof(lenum); 
	uRetCode = Netbios( &Ncb ); 
	
	for (i=0; i < lenum.length ;i++) 
	{ 
		memset( &Ncb, 0, sizeof(Ncb) ); 
		Ncb.ncb_command = NCBRESET; 
		Ncb.ncb_lana_num = lenum.lana[i]; 
		
		uRetCode = Netbios( &Ncb ); 
		memset( &Ncb, 0, sizeof (Ncb) ); 
		Ncb.ncb_command = NCBASTAT; 
		Ncb.ncb_lana_num = lenum.lana[i]; 
		
		strcpy( (char*)Ncb.ncb_callname, "* " ); 
		Ncb.ncb_buffer = (unsigned char *) &Adapter; 
		Ncb.ncb_length = sizeof(Adapter); 
		
		uRetCode = Netbios( &Ncb ); 
		
		if ( uRetCode == 0 ) 
		{ 
			str_value.Format("%02x-%02x-%02x-%02x-%02x-%02x", 
				Adapter.adapt.adapter_address[0], 
				Adapter.adapt.adapter_address[1], 
				Adapter.adapt.adapter_address[2], 
				Adapter.adapt.adapter_address[3], 
				Adapter.adapt.adapter_address[4], 
				Adapter.adapt.adapter_address[5] ); 
		} 
	} 	
	return str_value;
}


void CPublic_Function::Robot_BackData_Set(int site, int type, int xpos, int ypos, int noption)
{
	st_work.n_back_site = site;
	st_work.n_back_type = type;
	st_work.n_back_x_pos = xpos;
	st_work.n_back_y_pos = ypos;
	st_work.n_back_picker = noption;
//	st_work.n_back_z_movepos = P_Z_SAFETY;
}
void CPublic_Function::Robot_BackData_Set(int nSite, int site)
{
	st_work.mn_back_site[nSite]			= site;
}

void CPublic_Function::Robot_BackData_Z_Set(int movepos)
{
	st_work.n_back_z_movepos = movepos;
}

int CPublic_Function::LdModuleRobot_Reinstatement_Position(int mode)
{
	int Ret = 0, i = 0, n_response = 0;
	int FuncRet = RET_PROCEED;
	int nRet = 0, nRet2 = 0;

//	return RET_GOOD;

	CDialog_Select	select_dlg;
 
	
	if (mode == 0 && st_work.mn_reinstatement_mode[LDMODULE_SITE] == 0) // 완전 초기화 후만 관리함 
	{	// 현재 위치 백업 
		for (i = 0; i < MOT_MAXMOTOR; i++)
		{
			st_work.md_reinstatement_Pos[0][i] =  COMI.md_cmdpos_backup[i];
		}

		for (i = 0; PICKER_NUM; i++)
		{
			st_work.mn_PickerUpDnStatusBackup[LDMODULE_SITE][i] = FAS_IO.get_out_bit(st_io.o_ld_module_picker_updn[i], IO_OFF);
			st_work.mn_FingerStatusBackup[LDMODULE_SITE][i] = FAS_IO.get_out_bit(st_io.o_ld_module_glipper_onoff[i], IO_OFF);
			st_work.mn_DviceStatusBackup[LDMODULE_SITE][i] = FAS_IO.get_in_bit(st_io.i_ld_module_dvc_chk[i], IO_OFF);
		}				
		st_work.mn_TrayClampStatusBackup[LDMODULE_SITE][1] = FAS_IO.get_out_bit(st_io.o_m_stacker2_tray_clamp_onoff, IO_OFF);
		st_work.mn_reinstatement_mode[LDMODULE_SITE] = 1; //한번 데이터를 저장한다 
		reinstatement_Step = 0;

		return RET_GOOD;
	}	 
	else if (mode == 0)
	{
		return RET_GOOD;
	}


	switch (reinstatement_Step)
	{
	case 0:
		if (st_work.mn_reinstatement_mode[LDMODULE_SITE] == 1) // 스타트 키를 누르면 2이다 
		{
			reinstatement_Step = 1000;
		}
		break;

	case 1000:
		for (i = 0; i < MOT_MAXMOTOR; i++)
		{
			st_work.md_reinstatement_Pos[1][i] = COMI.Get_MotCurrentPos(i);
		}
		for (i = 0; i < 10; i++)
		{
			mn_change[LDMODULE_SITE][i] = 0;
		}
	
		//변경된 위치값 출력
		if (st_work.md_reinstatement_Pos[1][M_LDM_STACKER_1] >  st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1] + COMI.mn_allow_value[M_LDM_STACKER_1]
			|| st_work.md_reinstatement_Pos[1][M_LDM_STACKER_1] < st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1] - COMI.mn_allow_value[M_LDM_STACKER_1])
		{
			mn_change[LDMODULE_SITE][0]++;
			sprintf(st_msg.c_abnormal_msg, "Load Stacker#1 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1], (long)st_work.md_reinstatement_Pos[1][M_LDM_STACKER_1], (long)COMI.mn_allow_value[M_LDM_STACKER_1]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_LDM_STACKER_2] > st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2] + COMI.mn_allow_value[M_LDM_STACKER_2]
			|| st_work.md_reinstatement_Pos[1][M_LDM_STACKER_2] < st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2] - COMI.mn_allow_value[M_LDM_STACKER_2])
		{
			mn_change[LDMODULE_SITE][1]++;
			sprintf(st_msg.c_abnormal_msg, "Load Stacker#2 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2], (long)st_work.md_reinstatement_Pos[1][M_LDM_STACKER_2], (long)COMI.mn_allow_value[M_LDM_STACKER_2]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_LDM_STACKER_MOVE] > st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] + COMI.mn_allow_value[M_LDM_STACKER_MOVE]
			|| st_work.md_reinstatement_Pos[1][M_LDM_STACKER_MOVE] < st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] - COMI.mn_allow_value[M_LDM_STACKER_MOVE])
		{
			mn_change[LDMODULE_SITE][2]++;
			sprintf(st_msg.c_abnormal_msg, "Load Stacker Move POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE], (long)st_work.md_reinstatement_Pos[1][M_LDM_STACKER_MOVE], (long)COMI.mn_allow_value[M_LDM_STACKER_MOVE]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_M_RBT_Y] > st_work.md_reinstatement_Pos[0][M_M_RBT_Y] + COMI.mn_allow_value[M_M_RBT_Y]
			|| st_work.md_reinstatement_Pos[1][M_M_RBT_Y] < st_work.md_reinstatement_Pos[0][M_M_RBT_Y] - COMI.mn_allow_value[M_M_RBT_Y])
		{
			mn_change[LDMODULE_SITE][3]++;
			sprintf(st_msg.c_abnormal_msg, "Module Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_M_RBT_Y], (long)st_work.md_reinstatement_Pos[1][M_M_RBT_Y], (long)COMI.mn_allow_value[M_M_RBT_Y]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_M_RBT_Z] > st_work.md_reinstatement_Pos[0][M_M_RBT_Z] + COMI.mn_allow_value[M_M_RBT_Z]
			|| st_work.md_reinstatement_Pos[1][M_M_RBT_Z] < st_work.md_reinstatement_Pos[0][M_M_RBT_Z] - COMI.mn_allow_value[M_M_RBT_Z])
		{
			mn_change[LDMODULE_SITE][3]++;
			sprintf(st_msg.c_abnormal_msg, "Module Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_M_RBT_Z], (long)st_work.md_reinstatement_Pos[1][M_M_RBT_Z], (long)COMI.mn_allow_value[M_M_RBT_Z]);
		}

		st_msg.mstr_event_msg[0] = "Recovery Mode.......";
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);

		st_work.mn_reinstatement_mode[LDMODULE_SITE] = 3; //초기화 정보를 완료했다, 클리어 

		if(mn_change[LDMODULE_SITE][0] > 0 || mn_change[LDMODULE_SITE][1] > 0)
		{
			reinstatement_Step = 2000;
		}
		else if(mn_change[LDMODULE_SITE][2] > 0)
		{
			reinstatement_Step = 2000;
		}
		else if(mn_change[LDMODULE_SITE][3] > 0)
		{
			reinstatement_Step = 2000;
		}
		else
		{
			reinstatement_Step = 1100;
		}

		break;

	case 1100:
		backuptime[0] = GetCurrentTime();
		reinstatement_Step = 1200;
		break;

	case 1200:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if (backuptime[2] >= 500)
		{
			reinstatement_Step = 50000;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 2000:
		Robot_BackMovePosValCheck();
		
		COMI.Set_MotPower(M_LDM_STACKER_1, TRUE);
		COMI.Set_MotPower(M_LDM_STACKER_2, TRUE);
		COMI.Set_MotPower(M_LDM_STACKER_MOVE, TRUE);
		COMI.Set_MotPower(M_M_RBT_Y, TRUE);
		COMI.Set_MotPower(M_M_RBT_Z, TRUE);
	
		mn_check[0] = VARIABLE_INIT;
		mn_check[1] = VARIABLE_INIT;

		if(mn_change[0] > 0 || mn_change[1] > 0)
		{
			reinstatement_Step = 2000;
		}
		else if(mn_change[2] > 0)
		{
			reinstatement_Step = 2100;
		}
		else if(mn_change[3] > 0)
		{
			reinstatement_Step = 3000;
		}
		else
		{
			reinstatement_Step = 1100;
		}
		break;

	case 2100:
		nRet = COMI.HomeCheck_Mot(M_LDM_STACKER_1, st_motor[M_LDM_STACKER_1].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet == CTLBD_RET_GOOD)
		{
			reinstatement_Step = 2110;
		}		
		else if (nRet != CTLBD_RET_PROCEED)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5001, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2110:
		nRet = COMI.HomeCheck_Mot(M_LDM_STACKER_2, st_motor[M_LDM_STACKER_2].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet == BD_GOOD)
		{
			reinstatement_Step = 2120;
		}		
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5002, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2120:
		nRet = COMI.HomeCheck_Mot(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet == BD_GOOD)
		{
			reinstatement_Step = 2130;
		}		
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5003, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2130:
		if((st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] > (st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_LD_POS] + st_motor[M_LDM_STACKER_MOVE].mn_allow)) ||
			(st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE] < (st_motor[M_LDM_STACKER_MOVE].md_pos[P_MSTACKER_ULD_POS] - st_motor[M_LDM_STACKER_MOVE].mn_allow)))
		{
			reinstatement_Step = 2140;
		}
		else
		{
			reinstatement_Step = 2150;
		}
		break;

	case 2140:
		nRet = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_motor[M_LDM_STACKER_MOVE].md_pos[st_work.mn_back_site[M_LDM_STACKER_MOVE]], st_basic.nManualSpeed);
		if(nRet == BD_GOOD)
		{
			reinstatement_Step = 2200;
		}
		else if(nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			reinstatement_Step = 2140;
		}
		break;

	case 2150:
		nRet = CTL_Lib.Single_Move(M_LDM_STACKER_MOVE, st_work.md_reinstatement_Pos[0][M_LDM_STACKER_MOVE], st_basic.nManualSpeed);
		if(nRet == BD_GOOD)
		{
			reinstatement_Step = 2200;
		}
		else if(nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			reinstatement_Step = 2140;
		}
		break;

	case 2200:
		nRet = CTL_Lib.Single_Move(M_LDM_STACKER_1, st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1], MOT_TIMEOUT);		
		if (nRet == BD_GOOD)
		{
			reinstatement_Step = 2300;
		}		
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5002, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2300:
		nRet = CTL_Lib.Single_Move(M_LDM_STACKER_2, st_work.md_reinstatement_Pos[0][M_LDM_STACKER_2], MOT_TIMEOUT);		
		if (nRet == BD_GOOD)
		{
			reinstatement_Step = 2000;
		}		
		else if (nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5002, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3000:
		if(mn_change[3] > 0)
		{
			//Picker가 업 상태면 상관없음 
			if(st_work.mn_PickerUpDnStatusBackup[LDMODULE_SITE][0] == IO_OFF && st_work.mn_PickerUpDnStatusBackup[LDMODULE_SITE][1] == IO_OFF)
			{
				reinstatement_Step = 3100;
			}
			else
			{
				//Finger가 벌려있으면 업다운 상관없음
				if(st_work.mn_FingerStatusBackup[LDMODULE_SITE][0] == IO_OFF && st_work.mn_FingerStatusBackup[LDMODULE_SITE][1] == IO_OFF)
				{
					reinstatement_Step = 3100;
				}
				else
				{
					//둘중 하나는 디바이스가 존재한다.
					//1. Tray 위 : 상관없이 업다운 진행한다
					//2. Buffer 위 경우 체크 요망
					if(st_work.mn_DviceStatusBackup[LDMODULE_SITE][0] == IO_ON || st_work.mn_DviceStatusBackup[LDMODULE_SITE][1] == IO_ON)
					{
						if(st_work.mn_back_site[M_M_RBT_Y] < Y_LD_WORK_PLACE1)//1. 트레이 위
						{
							reinstatement_Step = 3100;
						}
						else
						{	//1.피커는 PCB를 집고 있고,Buffer는 안집고 있다
							//2.피커는 PCB를 잡고 있고,Buffer도 집고 있다.
							if(st_work.mn_back_site[M_M_RBT_Y] - Y_LD_WORK_PLACE1 >= 0)
							{
								
								if(st_work.mn_ModuleClampStatusBackup[st_work.mn_back_site[M_M_RBT_Y] - Y_LD_WORK_PLACE1] == IO_ON)
								{//1.버퍼가 움지였는지 체크
								 //2.Buffer Clamp만 ON이다.
									reinstatement_Step = 4000;
								}
								else
								{
									reinstatement_Step = 3100;
								}
							}
							else
							{
								reinstatement_Step = 3100;
							}
							
						}
					}
					else//디바이스가 없는데 다운되어 있다.
					{
						reinstatement_Step = 4000;
					}
				}
			}
		}
		else
		{
			reinstatement_Step = 50000;
		}
		break;

	case 3100:
		for (i = 0; i < PICKER_NUM; i++)
		{
			ActionPicker[D_EXIST][i] = YES;
		}
		Set_PickerUpDown(LDMODULE_SITE, IO_OFF, ActionPicker);
		nRet = COMI.HomeCheck_Mot(M_M_RBT_Z, st_motor[M_M_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet == CTLBD_RET_GOOD)
		{
			reinstatement_Step = 3110;
		}		
		else if (nRet != CTLBD_RET_PROCEED)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5020, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3110:
		nRet = Get_PickerUpDown(LDMODULE_SITE, IO_OFF, ActionPicker);
		if (nRet == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			reinstatement_Step = 3200;
		}
		else if (nRet == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			CTL_Lib.Alarm_Error_Occurrence(5021, st_work.mn_run_status, mc_alarmcode);
		}
		break;

	case 3200:
		nRet = CTL_Lib.Single_Move(M_M_RBT_Z, st_motor[M_M_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);		
		if (nRet == CTLBD_RET_GOOD)
		{
			reinstatement_Step = 3300;
		}
		else if (nRet != CTLBD_RET_PROCEED)
		{
			// 000100 1 A "Front Shifter Z Safety Pos Move Error."
			CTL_Lib.Alarm_Error_Occurrence(5022, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;
		
	case 3300:
		nRet = COMI.HomeCheck_Mot(M_M_RBT_Y, st_motor[M_M_RBT_Y].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet == CTLBD_RET_GOOD)
		{
			reinstatement_Step = 3400;
		}		
		else if (nRet != CTLBD_RET_PROCEED)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5023, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3400:		
		nRet = CTL_Lib.Single_Move(M_M_RBT_Y, st_motor[M_M_RBT_Y].md_pos[Y_LD_SAFETY], st_basic.nManualSpeed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			reinstatement_Step = 3500;
		}
		else if (nRet != CTLBD_RET_PROCEED)
		{
			// 000100 1 A "Front Shifter Z Safety Pos Move Error."
			CTL_Lib.Alarm_Error_Occurrence(5024, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;
		
	case 3500:
		nRet = CTL_Lib.Single_Move(M_M_RBT_Y, st_motor[M_M_RBT_Y].md_pos[st_work.mn_back_site[M_M_RBT_Y]], st_basic.nManualSpeed);
		
		if (nRet == CTLBD_RET_GOOD)
		{
			reinstatement_Step = 3510;
		}
		else if (nRet == CTLBD_RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(5025, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3510:
		backuptime[0] = GetCurrentTime();
		reinstatement_Step = 3600;
		break;
		
	case 3600:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if (backuptime[2] >= 500)
		{
			reinstatement_Step = 10000;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 4000:
		//일단 Buffer의 위치를 측정할 필요가 있다.
		//움직였기에 이 구간에 들어온다
		//if()
		FAS_IO.set_out_bit(st_work.mn_ModuleClampStatusBackup[st_work.mn_back_site[M_M_RBT_Y] - Y_LD_WORK_PLACE1], IO_OFF);
		backuptime[0] = GetCurrentTime();
		reinstatement_Step = 4100;
		break;

	case 4100:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if(backuptime[2] < 0) backuptime[0] = GetCurrentTime();
		if(backuptime[2] < 1000) break;
		
		if(FAS_IO.get_in_bit(st_work.mn_ModuleClampStatusBackup[st_work.mn_back_site[M_M_RBT_Y] - Y_LD_WORK_PLACE1], IO_OFF) == IO_OFF)
		{
			reinstatement_Step = 3100;
		}
		else
		{
			if(backuptime[2] > 5000)
			{
				sprintf(mc_alarmcode,"55080%d", st_work.mn_back_site[M_M_RBT_Y] - Y_LD_WORK_PLACE1);
				CTL_Lib.Alarm_Error_Occurrence(5026, CTL_dWARNING, mc_alarmcode);
			}
		}
		break;

	case 10000:
		for (i = 0; i < PICKER_NUM; i++)
		{
			FAS_IO.set_out_bit(st_io.o_ld_module_picker_updn[i], st_work.mn_PickerUpDnStatusBackup[LDMODULE_SITE][i]);
		}
		backuptime[0] = GetCurrentTime();
		reinstatement_Step = 10100;
		break;

	case 10100:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if (backuptime[2] >= 1500)
		{
			reinstatement_Step = 10200;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 10200:
		for (i = 0; i < PICKER_NUM; i++)
		{
			FAS_IO.set_out_bit(st_io.o_ld_module_glipper_onoff[i], st_work.mn_FingerStatusBackup[LDMODULE_SITE][i]);
		}
		backuptime[0] = GetCurrentTime();
		reinstatement_Step = 10300;
		break;

	case 10300:
		backuptime[1] = GetCurrentTime();
		backuptime[2] = backuptime[1] - backuptime[0];
		if (backuptime[2] >= 1000)
		{
			reinstatement_Step = 50000;
		}
		else if(backuptime[2] < 0)
		{
			backuptime[0] = GetCurrentTime();
		}
		break;

	case 50000:
		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);

		reinstatement_Step = 0;
		st_handler.mn_reinstatement = CTL_NO;			// 복구 동작이 아니다.
		st_work.mn_reinstatement_mode[0] = 0;	// 초기화 정보를 완료했다, 클리어 
		FuncRet = RET_GOOD;
		break;
	}
 	return FuncRet;
}

int CPublic_Function::LeftHsRobot_Reinstatement_Position(int mode)
{
	int i;
		for (i = 0; PICKER_NUM; i++)
		{
			st_work.mn_PickerUpDnStatusBackup[LEFTSINK_SITE][i] = FAS_IO.get_out_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[i], IO_OFF);
			st_work.mn_FingerStatusBackup[LEFTSINK_SITE][i] = FAS_IO.get_out_bit(st_io.o_hs_fwd_rbt_glipper_onoff[i], IO_OFF);
		}		

		st_work.mn_TrayClampStatusBackup[LEFTSINK_SITE][0] = FAS_IO.get_out_bit(st_io.o_hs_fwd_clamp_onoff, IO_OFF);
		st_work.mn_TrayClampStatusBackup[LEFTSINK_SITE][1] = FAS_IO.get_out_bit(st_io.o_hs_fwd_stacker2_tray_clamp_onoff, IO_OFF);

		if (st_work.md_reinstatement_Pos[1][M_HS_F_STACKER_1] > st_work.md_reinstatement_Pos[0][M_HS_F_STACKER_1] + COMI.mn_allow_value[M_HS_F_STACKER_1]
			|| st_work.md_reinstatement_Pos[1][M_HS_F_STACKER_1] < st_work.md_reinstatement_Pos[0][M_HS_F_STACKER_1] - COMI.mn_allow_value[M_HS_F_STACKER_1])
		{
			mn_change[0][LEFTSINK_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Front Heat Sink#1 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_F_STACKER_1], (long)st_work.md_reinstatement_Pos[1][M_HS_F_STACKER_1], (long)COMI.mn_allow_value[M_HS_F_STACKER_1]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_HS_F_STACKER_2] > st_work.md_reinstatement_Pos[0][M_HS_F_STACKER_2] + COMI.mn_allow_value[M_HS_F_STACKER_2]
			|| st_work.md_reinstatement_Pos[1][M_HS_F_STACKER_2] < st_work.md_reinstatement_Pos[0][M_HS_F_STACKER_2] - COMI.mn_allow_value[M_HS_F_STACKER_2])
		{
			mn_change[0][LEFTSINK_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Front Heat Sink#2 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_F_STACKER_2], (long)st_work.md_reinstatement_Pos[1][M_HS_F_STACKER_2], (long)COMI.mn_allow_value[M_HS_F_STACKER_2]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_HS_F_STACKER_MOVE] > st_work.md_reinstatement_Pos[0][M_HS_F_STACKER_MOVE] + COMI.mn_allow_value[M_HS_F_STACKER_MOVE]
			|| st_work.md_reinstatement_Pos[1][M_HS_F_STACKER_MOVE] < st_work.md_reinstatement_Pos[0][M_HS_F_STACKER_MOVE] - COMI.mn_allow_value[M_HS_F_STACKER_MOVE])
		{
			mn_change[0][LEFTSINK_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Front Heat Sink Move POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_F_STACKER_MOVE], (long)st_work.md_reinstatement_Pos[1][M_HS_F_STACKER_MOVE], (long)COMI.mn_allow_value[M_HS_F_STACKER_MOVE]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_HS_F_RBT_Y] > st_work.md_reinstatement_Pos[0][M_HS_F_RBT_Y] + COMI.mn_allow_value[M_HS_F_RBT_Y]
			|| st_work.md_reinstatement_Pos[1][M_HS_F_RBT_Y] < st_work.md_reinstatement_Pos[0][M_HS_F_RBT_Y] - COMI.mn_allow_value[M_HS_F_RBT_Y])
		{
			sprintf(st_msg.c_abnormal_msg, "HS Front Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_F_RBT_Y], (long)st_work.md_reinstatement_Pos[1][M_HS_F_RBT_Y], (long)COMI.mn_allow_value[M_HS_F_RBT_Y]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_HS_F_RBT_Z] > st_work.md_reinstatement_Pos[0][M_HS_F_RBT_Z] + COMI.mn_allow_value[M_HS_F_RBT_Z]
			|| st_work.md_reinstatement_Pos[1][M_HS_F_RBT_Z] < st_work.md_reinstatement_Pos[0][M_HS_F_RBT_Z] - COMI.mn_allow_value[M_HS_F_RBT_Z])
		{
			mn_change[0][LEFTSINK_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "HS Front Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_F_RBT_Z], (long)st_work.md_reinstatement_Pos[1][M_HS_F_RBT_Z], (long)COMI.mn_allow_value[M_HS_F_RBT_Z]);
		}
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
}

int CPublic_Function::RightHsRobot_Reinstatement_Position(int mode)
{
	int i;
		for (i = 0; PICKER_NUM; i++)
		{
			st_work.mn_PickerUpDnStatusBackup[RIGHTSINK_SITE][i] = FAS_IO.get_out_bit(st_io.o_hs_bwd_rbt_picker_picker_updn[i], IO_OFF);
			st_work.mn_FingerStatusBackup[RIGHTSINK_SITE][i] = FAS_IO.get_out_bit(st_io.o_hs_bwd_rbt_glipper_onoff[i], IO_OFF);
		}		

		st_work.mn_TrayClampStatusBackup[RIGHTSINK_SITE][0] = FAS_IO.get_out_bit(st_io.o_hs_bwd_clamp_onoff, IO_OFF);
		st_work.mn_TrayClampStatusBackup[RIGHTSINK_SITE][1] = FAS_IO.get_out_bit(st_io.o_hs_bwd_stacker2_tray_clamp_onoff, IO_OFF);


		if (st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_1] > st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1] + COMI.mn_allow_value[M_HS_B_STACKER_1]
			|| st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_1] < st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1] - COMI.mn_allow_value[M_HS_B_STACKER_1])
		{
			mn_change[0][M_HS_B_STACKER_1]++;
			sprintf(st_msg.c_abnormal_msg, "Rear Heat Sink#1 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_1], (long)st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_1], (long)COMI.mn_allow_value[M_HS_B_STACKER_1]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_2] > st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2] + COMI.mn_allow_value[M_HS_B_STACKER_2]
			|| st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_2] < st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_2] - COMI.mn_allow_value[M_HS_B_STACKER_2])
		{
			mn_change[0][M_HS_B_STACKER_1]++;
			sprintf(st_msg.c_abnormal_msg, "Rear Heat Sink#2 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_LDM_STACKER_1], (long)st_work.md_reinstatement_Pos[1][M_LDM_STACKER_1], (long)COMI.mn_allow_value[M_LDM_STACKER_1]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_MOVE] > st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] + COMI.mn_allow_value[M_HS_B_STACKER_MOVE]
			|| st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_MOVE] < st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE] - COMI.mn_allow_value[M_HS_B_STACKER_MOVE])
		{
			mn_change[0][M_HS_B_STACKER_1]++;
			sprintf(st_msg.c_abnormal_msg, "Rear Heat Sink Move POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_B_STACKER_MOVE], (long)st_work.md_reinstatement_Pos[1][M_HS_B_STACKER_MOVE], (long)COMI.mn_allow_value[M_HS_B_STACKER_MOVE]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Y] > st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Y] + COMI.mn_allow_value[M_HS_B_RBT_Y]
			|| st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Y] < st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Y] - COMI.mn_allow_value[M_HS_B_RBT_Y])
		{
			mn_change[0][M_HS_B_STACKER_1]++;
			sprintf(st_msg.c_abnormal_msg, "HS Rear Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Y], (long)st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Y], (long)COMI.mn_allow_value[M_HS_B_RBT_Y]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Z] > st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Z] + COMI.mn_allow_value[M_HS_B_RBT_Z]
			|| st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Z] < st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Z] - COMI.mn_allow_value[M_HS_B_RBT_Z])
		{
			mn_change[0][M_HS_B_STACKER_1]++;
			sprintf(st_msg.c_abnormal_msg, "HS Rear Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_HS_B_RBT_Z], (long)st_work.md_reinstatement_Pos[1][M_HS_B_RBT_Z], (long)COMI.mn_allow_value[M_HS_B_RBT_Z]);
		}
}

int CPublic_Function::LDClipRobot_Reinstatement_Position(int mode)
{	
		
		st_work.mn_PickerUpDnStatusBackup[LDCLIP_SITE][1] = FAS_IO.get_out_bit(st_io.o_clip_rbt_updn_onoff, IO_OFF);

		st_work.mn_TrayClampStatusBackup[LDCLIP_SITE][1] = FAS_IO.get_out_bit(st_io.o_clip_buffer_updn_onoff, IO_OFF);


		if (st_work.md_reinstatement_Pos[1][M_CLIP_BUFFER] > st_work.md_reinstatement_Pos[0][M_CLIP_BUFFER] + COMI.mn_allow_value[M_CLIP_BUFFER]
			|| st_work.md_reinstatement_Pos[1][M_CLIP_BUFFER] < st_work.md_reinstatement_Pos[0][M_CLIP_BUFFER] - COMI.mn_allow_value[M_CLIP_BUFFER])
		{
			mn_change[0][LDCLIP_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Clip Buffer POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_CLIP_BUFFER], (long)st_work.md_reinstatement_Pos[1][M_CLIP_BUFFER], (long)COMI.mn_allow_value[M_CLIP_BUFFER]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_GRIPPER] > st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_GRIPPER] + COMI.mn_allow_value[M_M_CLAMP_RBT_GRIPPER]
			|| st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_GRIPPER] < st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_GRIPPER] - COMI.mn_allow_value[M_M_CLAMP_RBT_GRIPPER])
		{
			mn_change[0][LDCLIP_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Clamp Rbt Gripper POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_GRIPPER], (long)st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_GRIPPER], (long)COMI.mn_allow_value[M_M_CLAMP_RBT_GRIPPER]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Y] > st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Y] + COMI.mn_allow_value[M_M_CLAMP_RBT_Y]
			|| st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Y] < st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Y] - COMI.mn_allow_value[M_M_CLAMP_RBT_Y])
		{
			sprintf(st_msg.c_abnormal_msg, "Clamp Rbt Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Y], (long)st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Y], (long)COMI.mn_allow_value[M_M_CLAMP_RBT_Y]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Z] > st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Z] + COMI.mn_allow_value[M_M_CLAMP_RBT_Z]
			|| st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Z] < st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Z] - COMI.mn_allow_value[M_M_CLAMP_RBT_Z])
		{
			mn_change[0][LDCLIP_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Clamp Rbt Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_M_CLAMP_RBT_Z], (long)st_work.md_reinstatement_Pos[1][M_M_CLAMP_RBT_Z], (long)COMI.mn_allow_value[M_M_CLAMP_RBT_Z]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Y] > st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Y] + COMI.mn_allow_value[M_CLIP_INSERT_Y]
			|| st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Y] < st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Y] - COMI.mn_allow_value[M_CLIP_INSERT_Y])
		{
			mn_change[0][LDCLIP_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Clip Insert Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Y], (long)st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Y], (long)COMI.mn_allow_value[M_CLIP_INSERT_Y]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Z] > st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Z] + COMI.mn_allow_value[M_CLIP_INSERT_Z]
			|| st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Z] < st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Z] - COMI.mn_allow_value[M_CLIP_INSERT_Z])
		{
			mn_change[0][LDCLIP_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Clip Insert Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_CLIP_INSERT_Z], (long)st_work.md_reinstatement_Pos[1][M_CLIP_INSERT_Z], (long)COMI.mn_allow_value[M_CLIP_INSERT_Z]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_M_CLIP_CLAMP] > st_work.md_reinstatement_Pos[0][M_M_CLIP_CLAMP] + COMI.mn_allow_value[M_M_CLIP_CLAMP]
			|| st_work.md_reinstatement_Pos[1][M_M_CLIP_CLAMP] < st_work.md_reinstatement_Pos[0][M_M_CLIP_CLAMP] - COMI.mn_allow_value[M_M_CLIP_CLAMP])
		{
			mn_change[0][LDCLIP_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Clip Clamp POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_M_CLIP_CLAMP], (long)st_work.md_reinstatement_Pos[1][M_M_CLIP_CLAMP], (long)COMI.mn_allow_value[M_M_CLIP_CLAMP]);
		}

		return RET_GOOD;


}

int CPublic_Function::SorterRobot_Reinstatement_Position(int mode)
{	
		
		st_work.mn_PickerUpDnStatusBackup[SORTER_SITE][1] = FAS_IO.get_out_bit(st_io.o_sorter_rbt_updn_onoff, IO_OFF);
		st_work.mn_FingerStatusBackup[SORTER_SITE][1] = FAS_IO.get_out_bit(st_io.o_sorter_rbt_gripper_onoff, IO_OFF);


		if (st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Z] > st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Z] + COMI.mn_allow_value[M_RBT_SORTER_Z]
			|| st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Z] < st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Z] - COMI.mn_allow_value[M_RBT_SORTER_Z])
		{
			mn_change[0][SORTER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Sorter Rbt Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_X], (long)st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Z], (long)COMI.mn_allow_value[M_RBT_SORTER_Z]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_RBT_SORTER_X] > st_work.md_reinstatement_Pos[0][M_RBT_SORTER_X] + COMI.mn_allow_value[M_RBT_SORTER_X]
			|| st_work.md_reinstatement_Pos[1][M_RBT_SORTER_X] < st_work.md_reinstatement_Pos[0][M_RBT_SORTER_X] - COMI.mn_allow_value[M_RBT_SORTER_X])
		{
			mn_change[0][SORTER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Sorter Rbt X POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_SORTER_X], (long)st_work.md_reinstatement_Pos[1][M_RBT_SORTER_X], (long)COMI.mn_allow_value[M_RBT_SORTER_X]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Y] > st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Y] + COMI.mn_allow_value[M_RBT_SORTER_Y]
			|| st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Y] < st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Y] - COMI.mn_allow_value[M_RBT_SORTER_Y])
		{
			mn_change[0][SORTER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Sorter Rbt Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_SORTER_Y], (long)st_work.md_reinstatement_Pos[1][M_RBT_SORTER_Y], (long)COMI.mn_allow_value[M_RBT_SORTER_Y]);
		}
return RET_GOOD;
}

int CPublic_Function::UnloadRobot_Reinstatement_Position(int mode)
{	
		
		st_work.mn_PickerUpDnStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_updn_onoff, IO_OFF);
		st_work.mn_FingerStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_picker_rbt_gripper_onoff, IO_OFF);

		st_work.mn_TrayClampStatusBackup[UNLOADER_SITE][1] = FAS_IO.get_out_bit(st_io.o_uld_stacker2_tray_clamp_onoff, IO_OFF);


		if (st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_2] > st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2] + COMI.mn_allow_value[M_ULDM_STACKER_2]
			|| st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_2] < st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2] - COMI.mn_allow_value[M_ULDM_STACKER_2])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Unload Stacker#2 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_2], (long)st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_2], (long)COMI.mn_allow_value[M_ULDM_STACKER_2]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_1] > st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1] + COMI.mn_allow_value[M_ULDM_STACKER_1]
			|| st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_1] < st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1] - COMI.mn_allow_value[M_ULDM_STACKER_1])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Unload Stacker#1 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_1], (long)st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_1], (long)COMI.mn_allow_value[M_ULDM_STACKER_1]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_MOVE] > st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] + COMI.mn_allow_value[M_ULDM_STACKER_MOVE]
			|| st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_MOVE] < st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE] - COMI.mn_allow_value[M_ULDM_STACKER_MOVE])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Unload Stacker Move POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_ULDM_STACKER_MOVE], (long)st_work.md_reinstatement_Pos[1][M_ULDM_STACKER_MOVE], (long)COMI.mn_allow_value[M_ULDM_STACKER_MOVE]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_X] > st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_X] + COMI.mn_allow_value[M_RBT_UNLOAD_X]
			|| st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_X] < st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_X] - COMI.mn_allow_value[M_RBT_UNLOAD_X])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Unload Rbt X POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_X], (long)st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_X], (long)COMI.mn_allow_value[M_RBT_UNLOAD_X]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Y] > st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Y] + COMI.mn_allow_value[M_RBT_UNLOAD_Y]
			|| st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Y] < st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Y] - COMI.mn_allow_value[M_RBT_UNLOAD_Y])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Unload Rbt Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Y], (long)st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Y], (long)COMI.mn_allow_value[M_RBT_UNLOAD_Y]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Z] > st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Z] + COMI.mn_allow_value[M_RBT_UNLOAD_Z]
			|| st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Z] < st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Z] - COMI.mn_allow_value[M_RBT_UNLOAD_Z])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Unload Rbt Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_RBT_UNLOAD_Z], (long)st_work.md_reinstatement_Pos[1][M_RBT_UNLOAD_Z], (long)COMI.mn_allow_value[M_RBT_UNLOAD_Z]);
		}
return RET_GOOD;
}

int CPublic_Function::VisionRobot_Reinstatement_Position(int mode)
{
	int i;
		for (i = 0; 2; i++)
		{
			st_work.mn_VisClampStatusBackup[i][1] = FAS_IO.get_out_bit(st_io.o_vis_buff1_clamp_onoff + (i*2), IO_OFF);
			st_work.mn_VisRotateStatusBackup[i][1] = FAS_IO.get_out_bit(st_io.o_vis_buff1_rotator_onoff + (i*2), IO_OFF);
		}

		if (st_work.md_reinstatement_Pos[1][M_LABEL_ATTACH_Z] > st_work.md_reinstatement_Pos[0][M_LABEL_ATTACH_Z] + COMI.mn_allow_value[M_LABEL_ATTACH_Z]
			|| st_work.md_reinstatement_Pos[1][M_LABEL_ATTACH_Z] < st_work.md_reinstatement_Pos[0][M_LABEL_ATTACH_Z] - COMI.mn_allow_value[M_LABEL_ATTACH_Z])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Label Attach Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_LABEL_ATTACH_Z], (long)st_work.md_reinstatement_Pos[1][M_LABEL_ATTACH_Z], (long)COMI.mn_allow_value[M_LABEL_ATTACH_Z]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_VISION1_Y] > st_work.md_reinstatement_Pos[0][M_VISION1_Y] + COMI.mn_allow_value[M_VISION1_Y]
			|| st_work.md_reinstatement_Pos[1][M_VISION1_Y] < st_work.md_reinstatement_Pos[0][M_VISION1_Y] - COMI.mn_allow_value[M_VISION1_Y])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Vision#1 Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_VISION1_Y], (long)st_work.md_reinstatement_Pos[1][M_VISION1_Y], (long)COMI.mn_allow_value[M_VISION1_Y]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_VISION1_Z] > st_work.md_reinstatement_Pos[0][M_VISION1_Z] + COMI.mn_allow_value[M_VISION1_Z]
			|| st_work.md_reinstatement_Pos[1][M_VISION1_Z] < st_work.md_reinstatement_Pos[0][M_VISION1_Z] - COMI.mn_allow_value[M_VISION1_Z])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Vision#1 Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_VISION1_Z], (long)st_work.md_reinstatement_Pos[1][M_VISION1_Z], (long)COMI.mn_allow_value[M_VISION1_Z]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_VISION2_Y] > st_work.md_reinstatement_Pos[0][M_VISION2_Y] + COMI.mn_allow_value[M_VISION2_Y]
			|| st_work.md_reinstatement_Pos[1][M_VISION2_Y] < st_work.md_reinstatement_Pos[0][M_VISION2_Y] - COMI.mn_allow_value[M_VISION2_Y])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Vision#2 Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_VISION2_Y], (long)st_work.md_reinstatement_Pos[1][M_VISION2_Y], (long)COMI.mn_allow_value[M_VISION2_Y]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_VISION2_Z] > st_work.md_reinstatement_Pos[0][M_VISION2_Z] + COMI.mn_allow_value[M_VISION2_Z]
			|| st_work.md_reinstatement_Pos[1][M_VISION2_Z] < st_work.md_reinstatement_Pos[0][M_VISION2_Z] - COMI.mn_allow_value[M_VISION2_Z])
		{
			mn_change[0][UNLOADER_SITE]++;
			sprintf(st_msg.c_abnormal_msg, "Vision#2 Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_VISION2_Z], (long)st_work.md_reinstatement_Pos[1][M_VISION2_Z], (long)COMI.mn_allow_value[M_VISION2_Z]);
		}
return RET_GOOD;
}

int CPublic_Function::ModuleBufferRobot_Reinstatement_Position(int mode)
{	
	int nRet, i;


	for (i = 0; PICKER_NUM; i++)
	{
		st_work.mn_ModuleClampStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_m_clamp_onoff + (100*i), IO_OFF);
		st_work.mn_HsClampStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_hs_clamp_onoff + (100*i), IO_OFF);
		st_work.mn_BufVacuumStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_f_vacuum_onoff + (100*i), IO_OFF);
		st_work.mn_EjectStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_f_eject_onoff + (100*i), IO_OFF);
	}

	switch (reinstatement_Step)
	{
	case 0:
		if (st_work.mn_reinstatement_mode[WORK_BUFFER_SITE] == 1) // 스타트 키를 누르면 2이다 
		{
			reinstatement_Step = 1000;
		}
		break;

	case 1000:
		for (i = 0; i < 4; i++)
		{
			mn_change[WORK_BUFFER_SITE][i] = 0;
			st_work.mn_reinstatement_Buffsafety[i] = CTL_NO;
		}

		if (st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_1] > st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_1] + COMI.mn_allow_value[M_WORK_BUFFER_1]
			|| st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_1] < st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_1] - COMI.mn_allow_value[M_WORK_BUFFER_1])
		{
			mn_change[WORK_BUFFER_SITE][WORK_BUFFER_1]++;
			sprintf(st_msg.c_abnormal_msg, "Work Buffer#1 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_1], (long)st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_1], (long)COMI.mn_allow_value[M_WORK_BUFFER_1]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_2] > st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_2] + COMI.mn_allow_value[M_WORK_BUFFER_2]
			|| st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_2] < st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_2] - COMI.mn_allow_value[M_WORK_BUFFER_2])
		{
			mn_change[WORK_BUFFER_SITE][WORK_BUFFER_2]++;
			sprintf(st_msg.c_abnormal_msg, "Work Buffer#2 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_2], (long)st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_2], (long)COMI.mn_allow_value[M_WORK_BUFFER_2]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_3] > st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_3] + COMI.mn_allow_value[M_WORK_BUFFER_3]
			|| st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_3] < st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_3] - COMI.mn_allow_value[M_WORK_BUFFER_3])
		{
			mn_change[WORK_BUFFER_SITE][WORK_BUFFER_3]++;
			sprintf(st_msg.c_abnormal_msg, "Work Buffer#3 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_3], (long)st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_3], (long)COMI.mn_allow_value[M_WORK_BUFFER_3]);
		}
		else if (st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_4] > st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_4] + COMI.mn_allow_value[M_WORK_BUFFER_4]
			|| st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_4] < st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_4] - COMI.mn_allow_value[M_WORK_BUFFER_4])
		{
			mn_change[WORK_BUFFER_SITE][WORK_BUFFER_4]++;
			sprintf(st_msg.c_abnormal_msg, "Work Buffer#4 POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.md_reinstatement_Pos[0][M_WORK_BUFFER_4], (long)st_work.md_reinstatement_Pos[1][M_WORK_BUFFER_4], (long)COMI.mn_allow_value[M_WORK_BUFFER_4]);
		}
		st_work.mn_reinstatement_mode[LDMODULE_SITE] = 3; //초기화 정보를 완료했다, 클리어 

		if(mn_change[WORK_BUFFER_SITE][WORK_BUFFER_1] > 0|| mn_change[WORK_BUFFER_SITE][WORK_BUFFER_2] > 0 || 
			mn_change[WORK_BUFFER_SITE][WORK_BUFFER_3] > 0 ||  mn_change[WORK_BUFFER_SITE][WORK_BUFFER_4] > 0)
		{
			reinstatement_Step = 1100;
		}
		else
		{
			reinstatement_Step = 6000;
		}
		break;
		
	case 1100:
// 		if(st_work.mn_reinstatement_Ysafety[LDMODULE_SITE] == CTL_YES && st_work.mn_reinstatement_Ysafety[LEFTSINK_SITE] == CTL_YES &&
// 			st_work.mn_reinstatement_Ysafety[RIGHTSINK_SITE] == CTL_YES && st_work.mn_reinstatement_Ysafety[LDCLIP_SITE] &&
// 			st_work.mn_reinstatement_Ysafety[SORTER_SITE] == CTL_YES)
		if(st_work.mn_LdModuleRobot_Reinstatement_Ready == YES &&
			st_work.mn_LeftHsRobot_Reinstatement_Ready   == YES &&
		   st_work.mn_RightHsRobot_Reinstatement_Ready  == YES &&
		   st_work.mn_ClipRobot_Reinstatement_Ready     == YES && // 20130416 
		   st_work.mn_SorterRobot_Reinstatement_Ready   == YES)
		{
			if(mn_change[WORK_BUFFER_SITE][WORK_BUFFER_1] > 0)
			{
				reinstatement_Step = 2000;
			}
			else if(mn_change[WORK_BUFFER_SITE][WORK_BUFFER_2] > 0)
			{
				reinstatement_Step = 3000;
			}
			else if (mn_change[WORK_BUFFER_SITE][WORK_BUFFER_3] > 0)
			{
				reinstatement_Step = 4000;
			}
			else if (mn_change[WORK_BUFFER_SITE][WORK_BUFFER_4] > 0)
			{
				reinstatement_Step = 5000;
			}
			else
			{
				reinstatement_Step = 6000;
			}
		}
		break;

	case 2000:
		nRet = COMI.HomeCheck_Mot(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet == CTLBD_RET_GOOD)
		{
			reinstatement_Step = 2100;
		}		
		else if (nRet != CTLBD_RET_PROCEED)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5027, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 2100:
		nRet = CTL_Lib.Single_Move(M_WORK_BUFFER_1, st_motor[M_WORK_BUFFER_1].md_pos[st_work.mn_back_site[M_WORK_BUFFER_1]], st_basic.nManualSpeed);
		if(nRet == BD_GOOD)
		{
			mn_change[WORK_BUFFER_SITE][WORK_BUFFER_1] = 0;
			reinstatement_Step = 1100;
		}
		else if(nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(5028, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3000:
		nRet = COMI.HomeCheck_Mot(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet == CTLBD_RET_GOOD)
		{
			reinstatement_Step = 3100;
		}		
		else if (nRet != CTLBD_RET_PROCEED)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5029, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 3100:
		nRet = CTL_Lib.Single_Move(M_WORK_BUFFER_2, st_motor[M_WORK_BUFFER_2].md_pos[st_work.mn_back_site[M_WORK_BUFFER_2]], st_basic.nManualSpeed);
		if(nRet == BD_GOOD)
		{
			mn_change[WORK_BUFFER_SITE][M_WORK_BUFFER_2] = 0;
			reinstatement_Step = 1100;
		}
		else if(nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(5030, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 4000:
		nRet = COMI.HomeCheck_Mot(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet == CTLBD_RET_GOOD)
		{
			reinstatement_Step = 4100;
		}		
		else if (nRet != CTLBD_RET_PROCEED)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5031, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 4100:
		nRet = CTL_Lib.Single_Move(M_WORK_BUFFER_3, st_motor[M_WORK_BUFFER_3].md_pos[st_work.mn_back_site[M_WORK_BUFFER_3]], st_basic.nManualSpeed);
		if(nRet == BD_GOOD)
		{
			mn_change[WORK_BUFFER_SITE][M_WORK_BUFFER_3] = 0;
			reinstatement_Step = 1100;
		}
		else if(nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(5032, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;
		
	case 5000:
		nRet = COMI.HomeCheck_Mot(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].mn_homecheck_method, MOT_TIMEOUT);		
		if (nRet == CTLBD_RET_GOOD)
		{
			reinstatement_Step = 5100;
		}		
		else if (nRet != CTLBD_RET_PROCEED)
		{
			// 000001 0 A "Motor Home Check Error - (Front Shifter Z)."
			CTL_Lib.Alarm_Error_Occurrence(5033, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 5100:
		nRet = CTL_Lib.Single_Move(M_WORK_BUFFER_4, st_motor[M_WORK_BUFFER_4].md_pos[st_work.mn_back_site[M_WORK_BUFFER_4]], st_basic.nManualSpeed);
		if(nRet == BD_GOOD)
		{
			mn_change[WORK_BUFFER_SITE][M_WORK_BUFFER_4] = 0;
			reinstatement_Step = 1100;
		}
		else if(nRet == BD_ERROR || nRet == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(5034, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 6000:
		for (i = 0; i < 4; i++)
		{
			st_work.mn_reinstatement_Buffsafety[i] = CTL_YES;
		}
		st_work.mn_WorkBufferRobot_Reinstatement_Ready = YES;
		reinstatement_Step = 6100;
		break;

	case 6100:
		for (i = 0; PICKER_NUM; i++)
		{
			st_work.mn_ModuleClampStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_m_clamp_onoff + (100*i), IO_OFF);
			st_work.mn_HsClampStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_hs_clamp_onoff + (100*i), IO_OFF);
			st_work.mn_BufVacuumStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_f_vacuum_onoff + (100*i), IO_OFF);
			st_work.mn_EjectStatusBackup[WORK_BUFFER_1 + i] = FAS_IO.get_out_bit(st_io.o_buf1_f_eject_onoff + (100*i), IO_OFF);
		}

	case 9000:
		reinstatement_Step = 0;
		st_work.mn_reinstatement_mode[WORK_BUFFER_SITE] = 0;	// 초기화 정보를 완료했다, 클리어 
		st_work.mn_WorkBufferRobot_Reinstatement_Ok = YES;//20121126

		break;
	}
return RET_GOOD;
}

void CPublic_Function::Robot_BackMovePosValCheck()
{
	int i;
	double dHifixGap = 0;
	double dHifixGapCal = 0;
	double dHifixGapLast = 0;
	double d_gap_pos = 0;
	double d_stamp_move_gap = 0;

	//여기서 이전에 동작했던 위치를 다시 계산해야한다.
	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		st_work.md_reinstatement_pos[i] = st_work.md_reinstatement_Pos[0][i];
		st_work.md_reinstatement_pos[i] = st_motor[i].md_pos[st_work.mn_back_site[i]];
	}
}

void CPublic_Function::ThreadFunctionStepTrace(int nPos, int nStep)
{
	if (nStep == st_work.mn_prev_step[nPos]) // step이 동일하군~
	{
		return;
	}

	CString str;
	str.Format("[%04d]%d", nPos, nStep);
	On_LogFile_Add(LOG_DEBUG, str);

	st_work.mn_prev_step[nPos] = nStep;
	StepChangeTime[nPos] = CTime::GetCurrentTime();	 
	st_work.mstr_last_change_date[nPos] = StepChangeTime[nPos].Format("%y/%m/%d %H:%M:%S");	
	
	if (st_work.mstr_history_step[nPos].GetLength() == 120 || st_work.mstr_history_step[nPos].GetLength() == 276)
	{
		str_step_temp[nPos].Format("%05d|", nStep);
		st_work.mstr_history_step[nPos] += str_step_temp[nPos];
		st_work.mstr_history_step[nPos] += "\r\n|                          |";
	}
	else if (st_work.mstr_history_step[nPos].GetLength() == 432)
	{
		str_step_temp[nPos].Format("%05d|", nStep);
		st_work.mstr_history_step[nPos] += str_step_temp[nPos];

		st_work.mstr_history_step_back_up[nPos] = st_work.mstr_history_step[nPos];
		st_work.mstr_history_step[nPos] = "";
	}
	else
	{
		str_step_temp[nPos].Format("%05d,", nStep);
		st_work.mstr_history_step[nPos] += str_step_temp[nPos];
	}
}

void CPublic_Function::TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp)
{
	CString line, str_temp_data_temp, str_content;
	
	line = "";
	
	fprintf(fp,"%s", str_sdata);
	
	switch (dir)
	{
	case 6 :
		str_temp_data_temp.Format("%6s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------+";
		break;
		
	case 14:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------+";   // +----------------------------------------+----------------------------------+
		break;
		
	case 15:
		str_temp_data_temp.Format("%33s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+----------------------------------+";
		break;
		
	case 24:
		str_temp_data_temp.Format("%24s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------+";
		break;
		
	case 140:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 30:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------------+";
		break;
		
	case 131:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-----------------------------------------------------------------------------------------------------------------------+";
		break;
		
	case 51:
		str_temp_data_temp.Format("%50s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 49:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------------------------------------------+";
		break;
		
	case 50:
		str_temp_data_temp.Format("%50s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------------------------------------------+";
		break;

	case 130:
		str_temp_data_temp.Format("%124s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";
		break;
		
	case 300:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
	}
	fprintf(fp,"%s\r\n",str_content);
}

// void CPublic_Function::OnLotCountInformationSave( const CString& strLotName )
// {
// 	CString mstr_file_name;    // 마지막으로 생성된 파일 이름 저장 변수 
// 	CString mstr_create_file;  // 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
// 	CString mstr_list_name, mstr_temp_data;  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
// 	CString mstr_content;  // 한 라인에 출력할 문자열 정보 저장 변수 
// 	int mn_existence, i = 0;      // 파일 존재 유무 설정 플래그 
// 	char fileName[256];    // 검색할 파일 정보 설정 함수 
// 	FILE  *fp ;            // 파일에 대한 포인터 설정 변수 
// 	
// 	CString str_title_time, str_file_time, str_new_file;		// 파일에 출력할 타이틀 시간 저장 변수
// 	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수
// 	int n_cur_hour, n_cur_min, n_cur_sec;
// 	
// 	COleDateTime mtime_cur;						// 현재 시간 저장 변수
// 	CTime mp_time_cur;							// 타이틀 시간 저장 변수
// 
// 	CString str_temp_data;
// 	int pos = 0, n_length = 0;
// 
// 	CString strData;
// 	
// 	/* ************************************************************************** */
// 	/* 파일 이름으로 사용할 날짜 설정한다                                         */
// 	/* ************************************************************************** */
// 	mtime_cur = COleDateTime::GetCurrentTime();
// 	
// 	n_cur_year = mtime_cur.GetYear();  
// 	n_cur_month = mtime_cur.GetMonth();  
//     n_cur_day = mtime_cur.GetDay();  
// 
// 	n_cur_hour = mtime_cur.GetHour();
// 	n_cur_min = mtime_cur.GetMinute();
// 	n_cur_sec = mtime_cur.GetSecond();
// 	
// 	str_new_file.Format("LOT_Info%04d%02d%02d_%02d%02d%02d[%s]", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_min, n_cur_sec, st_handler.mstr_lot_name[0]);
// 		
// 	mstr_create_file = "C:\\AMT830\\Data\\" + str_new_file;
// 	mstr_create_file += ".TXT";
// 	
// 	sprintf(fileName, "%s", mstr_create_file);
// 	mn_existence = access(fileName,0);
// 	
// 	if (mn_existence == -1)  /* 파일 미존재 */
// 	{
// 		mstr_create_file = "C:\\AMT830\\Data\\" + str_new_file;
// 		mstr_create_file += ".TXT";
// 		/* ********************************************************************** */
// 	}
// 	
// 	/* ************************************************************************** */
// 	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
// 	/* ************************************************************************** */
// 	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
// 	{
// 		Func.MsgLog("파일 열기 실패!..");
// 		return;
// 	}
// 	/* ************************************************************************** */
// 	
// 	mstr_content = "+------------------------------------------------------------------------------------------------------+";
// 	fprintf(fp,"%s\n",mstr_content);
// 
// 	mstr_content.Format("LOT_No     : [%s]", st_handler.mstr_save_lot_name);
// 	fprintf(fp,"%s\n",mstr_content);
// 
// 	mstr_content.Format("Start Time : [%s]", st_handler.mstr_save_lot_start_time);
// 	fprintf(fp,"%s\n",mstr_content);
// 
// 	mstr_content.Format("End Time   : [%04d%02d%02d_%02d%02d%02d]", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_min, n_cur_sec);
// 	fprintf(fp,"%s\n",mstr_content);
// 
// 	mstr_content = "+------------------------------------------------------------------------------------------------------+";
// 	fprintf(fp,"%s\n",mstr_content);
// 
// 	// by. galmg.
// 	CString strWork;
// 	int iw;
// 	LotResult lotResult = g_AMTLotMgr.FindLotByName( strLotName ).m_LotResult;
// 	for( iw = WORK_PRIME; iw < WORK_MAX; iw++ )
// 	{
// 		if ( iw == WORK_PRIME ) strWork.Format( "%s", "Prime" );
// 		else if ( iw == WORK_RETEST ) strWork.Format( "%s", "Retest" );
// 		
// 		fprintf( fp, "Bin   Count %s: GOOD : %d \n", strWork, lotResult.GetBinCountGood(iw) ); 
// 		fprintf( fp, "Bin   Count %s: FAIL : %d \n", strWork, lotResult.GetBinCountFail(iw) );
// 		fprintf( fp, "Bin   Count %s: HTO : %d \n", strWork, lotResult.GetBinCountHTO(iw) );
// 		fprintf( fp, "Bin   Count %s: BTO : %d \n", strWork, lotResult.GetBinCountBTO(iw) );
// 		fprintf( fp, "Bin   Count %s: NTO : %d \n", strWork, lotResult.GetBinCountNTO(iw) );
// 	}
// 	
// 	for( iw = WORK_PRIME; iw < WORK_MAX; iw++ )
// 	{
// 		if ( iw == WORK_PRIME ) strWork.Format( "%s", "Prime" );
// 		else if ( iw == WORK_RETEST ) strWork.Format( "%s", "Retest" );
// 		
// 		float fLoadTemp = 0.0f;
// 		if ( lotResult.GetTestCountLoad(iw) != 0 )
// 			fLoadTemp = ( (float)(lotResult.GetTestCountGood(iw) )/ (float)(lotResult.GetTestCountLoad(iw) )) * 100.0f;
// 
// 		fprintf( fp, "Total Count %s: LOAD : %d \n", strWork, lotResult.GetTestCountLoad(iw) );
// 		fprintf( fp, "Total Count %s: GOOD : %d \n", strWork, lotResult.GetTestCountGood(iw) );
// 		fprintf( fp, "Total Count %s: FAIL : %d \n", strWork, lotResult.GetTestCountFail(iw) );
// 		fprintf( fp, "Total Count %s: YIELD : %3.2f %% \n", strWork, fLoadTemp );
// 	}
// 
// 	mstr_content = "+------------------------------------------------------------------------------------------------------+";
// 	fprintf(fp,"%s\n",mstr_content);
// 	
// 	fprintf( fp, "NTO BootTest - Load : %d \n", lotResult.GetBootTestCount_Load() );
// 	fprintf( fp, "NTO BootTest - Pass : %d \n", lotResult.GetBootTestCount_Pass() );
// 	fprintf( fp, "NTO BootTest - Load_Real : %d \n", lotResult.GetBootTestCount_Load_Real() );
// 	fprintf( fp, "NTO BootTest - Pass_Real : %d \n", lotResult.GetBootTestCount_Load_Real() );
// 
// 	mstr_content = "+------------------------------------------------------------------------------------------------------+";
// 	fprintf(fp,"%s\n",mstr_content);
// 
// 	// Work Count
// 	float fLoadTemp = 0;
// 	if ( g_AMTLotMgr.GetWorkCountLoad() != 0 )
// 	{
// 		fLoadTemp = ( (float)( g_AMTLotMgr.GetWorkCountGood() ) / (float)( g_AMTLotMgr.GetWorkCountLoad() )) * 100.0f;
// 	}
// 	
// 	fprintf(fp, "Work   Count : LOAD : %d \n", g_AMTLotMgr.GetWorkCountLoad() );
// 	fprintf(fp, "Work   Count : GOOD : %d \n", g_AMTLotMgr.GetWorkCountGood() );
// 	fprintf(fp, "Work   Count : FAIL : %d \n", g_AMTLotMgr.GetWorkCountFail() );
// 	fprintf(fp, "Work   Count : TOTAL : %d \n", g_AMTLotMgr.GetWorkCountGood() + g_AMTLotMgr.GetWorkCountFail() );
// 	fprintf(fp, "Work   Count : YIELD : %3.2f %%\n", fLoadTemp );
// 
// 	mstr_content = "+------------------------------------------------------------------------------------------------------+";
// 	fprintf(fp,"%s\n",mstr_content);
// 
// 	if (ferror(fp))  
// 	{
// 		Func.MsgLog("파일 저장 실패!..");
// 		return ;
// 	}
// 	
// 	fclose(fp);  /* 파일 종료 */
// }


void CPublic_Function::DB_Write_Time(st_time_database st_timedb)
{
//	g_cs.EnterCriticalSection();
//
//	CString strTmp[7];
//	
//	st_handler.lTimeTable[1] = GetCurrentTime();
//	st_handler.lTimeTable[2] = st_handler.lTimeTable[1] - st_handler.lTimeTable[0];
//	
//	st_handler.lTimeTable[0] = GetCurrentTime();
//	
//	st_timedb.mnTime_Time = (int)st_handler.lTimeTable[2];
//	
//	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_timedb.mole_date.GetYear(), 
//		st_timedb.mole_date.GetMonth(),
//		st_timedb.mole_date.GetDay(),
//		st_timedb.mole_date.GetHour(),
//		st_timedb.mole_date.GetMinute(),
//		st_timedb.mole_date.GetSecond());
//	
//	strTmp[2].Format("%-1s", st_timedb.mstrlot_no);
//	strTmp[3].Format("%-1s", st_timedb.mstrpart_no);	
//	
//	strTmp[4].Format("%d", st_timedb.mnTime_Section);
//	strTmp[5].Format("%d", st_timedb.mnTime_Time);
//	strTmp[6].Format("%d", st_timedb.mnMode);
//
//	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
//	{
//		if (Mysql.Database_Select("AMT830"))
//		{	
//			if (Mysql.Table_Select("AMT830", "TIME_DATA"))
//			{
//				Mysql.Record_Add("TIME_DATA", strTmp);
//			}
//			else
//			{
//				if (Mysql.Table_Create("AMT830", "TIME_DATA", 7, Mysql.mstr_time_table_name, Mysql.mn_time_table_type, Mysql.mn_time_table_num))
//				{
//					Mysql.Record_Add("TIME_DATA", strTmp);
//				}
//			}
//		}
//		else
//		{
//			if (Mysql.Database_Create("AMT830"))
//			{
//				if (Mysql.Table_Create("AMT830", "TIME_DATA", 7, Mysql.mstr_time_table_name, Mysql.mn_time_table_type, Mysql.mn_time_table_num))
//				{
//					Mysql.Record_Add("TIME_DATA", strTmp);
//				}
//			}
//		}
//	}
//	
//	Mysql.MySql_Close();
//
//	g_cs.LeaveCriticalSection();
}

void CPublic_Function::DB_Write_Alarm(st_alarm_database st_alarmdb)
{	
//	if (st_basic.n_mode_device == CTL_NO)			return;
//
//	g_cs.EnterCriticalSection();
//
//	CString strTmp[6];
//	
//	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_alarmdb.mole_date.GetYear(), 
//		st_alarmdb.mole_date.GetMonth(),
//		st_alarmdb.mole_date.GetDay(),
//		st_alarmdb.mole_date.GetHour(),
//		st_alarmdb.mole_date.GetMinute(),
//		st_alarmdb.mole_date.GetSecond());
//	
//	
//	strTmp[2].Format("%-1s", st_alarmdb.mstrlot_no);
//	strTmp[3].Format("%-1s", st_alarmdb.mstrpart_no);
//	
//	strTmp[4].Format("%-1s", st_alarmdb.mstrAlarmCode);
//	strTmp[5].Format("%-1s", st_alarmdb.mstrAlarmMsg);
//	
//	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
//	{
//		if (Mysql.Database_Select("AMT830"))
//		{	
//			if (Mysql.Table_Select("AMT830", "ALARM_DATA"))
//			{
//				Mysql.Record_Add("ALARM_DATA", strTmp);
//			}
//			else
//			{
//				if (Mysql.Table_Create("AMT830", "ALARM_DATA", 6, Mysql.mstr_alarm_table_name, Mysql.mn_alarm_table_type, Mysql.mn_alarm_table_num))
//				{
//					Mysql.Record_Add("ALARM_DATA", strTmp);
//				}
//			}
//		}
//		else
//		{
//			if (Mysql.Database_Create("AMT830"))
//			{
//				if (Mysql.Table_Create("AMT830", "ALARM_DATA", 6, Mysql.mstr_alarm_table_name, Mysql.mn_alarm_table_type, Mysql.mn_alarm_table_num))
//				{
//					Mysql.Record_Add("ALARM_DATA", strTmp);
//				}
//			}
//		}
//	}
//	
//	Mysql.MySql_Close();
//
//	g_cs.LeaveCriticalSection();
}

void CPublic_Function::DB_Write_Ground(st_ground_database st_grounddb)
{
//	CString strTmp[7];
//	
//	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_grounddb.mole_date.GetYear(), 
//		st_grounddb.mole_date.GetMonth(),
//		st_grounddb.mole_date.GetDay(),
//		st_grounddb.mole_date.GetHour(),
//		st_grounddb.mole_date.GetMinute(),
//		st_grounddb.mole_date.GetSecond());
//		
//	strTmp[2].Format("%-1s", st_grounddb.mstrChannel[0]);
//	strTmp[3].Format("%-1s", st_grounddb.mstrChannel[1]);
//	strTmp[4].Format("%-1s", st_grounddb.mstrChannel[2]);
//	strTmp[5].Format("%-1s", st_grounddb.mstrChannel[3]);
//	strTmp[6].Format("%-1s", st_grounddb.mstrChannel[4]);
//
//	if ((strTmp[2] == "" || strTmp[2] == " ") && (strTmp[3] == "" || strTmp[3] == " ") && (strTmp[4] == "" || strTmp[4] == " ")
//		&& (strTmp[5] == "" || strTmp[5] == " ") && (strTmp[6] == "" || strTmp[6] == " "))
//	{
//		return;
//	}
//
//	g_cs.EnterCriticalSection();
//
//	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
//	{
//		if (Mysql.Database_Select("AMT830"))
//		{	
//			if (Mysql.Table_Select("AMT830", "GROUND_DATA"))
//			{
//				Mysql.Record_Add("GROUND_DATA", strTmp);
//			}
//			else
//			{
//				if (Mysql.Table_Create("AMT830", "GROUND_DATA", 7, Mysql.mstr_ground_table_name, Mysql.mn_ground_table_type, Mysql.mn_ground_table_num))
//				{
//					Mysql.Record_Add("GROUND_DATA", strTmp);
//				}
//			}
//		}
//		else
//		{
//			if (Mysql.Database_Create("AMT830"))
//			{
//				if (Mysql.Table_Create("AMT830", "GROUND_DATA", 7, Mysql.mstr_ground_table_name, Mysql.mn_ground_table_type, Mysql.mn_ground_table_num))
//				{
//					Mysql.Record_Add("GROUND_DATA", strTmp);
//				}
//			}
//		}
//	}
//	
//	Mysql.MySql_Close();
//
//	g_cs.LeaveCriticalSection();
}

void CPublic_Function::DB_Write_Buffer(st_buffer_database st_bufferdb)
{
//	CString strTmp[9];
//	
//	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_bufferdb.mole_date.GetYear(), 
//		st_bufferdb.mole_date.GetMonth(),
//		st_bufferdb.mole_date.GetDay(),
//		st_bufferdb.mole_date.GetHour(),
//		st_bufferdb.mole_date.GetMinute(),
//		st_bufferdb.mole_date.GetSecond());
//	
//	strTmp[2].Format("%-1s", st_bufferdb.mstrlot_no);
//	strTmp[3].Format("%-1s", st_bufferdb.mstrpart_no);
//	strTmp[4].Format("%d", st_bufferdb.mnbuffer_pos);
//	strTmp[5].Format("%d", st_bufferdb.mnlast_module);
//	strTmp[6].Format("%d", st_bufferdb.mnbypass);
//	strTmp[7].Format("%d", st_bufferdb.mnmodule_cnt);
//	strTmp[8].Format("%d", st_bufferdb.mnretest);
//	
//	g_cs.EnterCriticalSection();
//	
//	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
//	{
//		if (Mysql.Database_Select("AMT830"))
//		{	
//			if (Mysql.Table_Select("AMT830", "BUFFER_DATA"))
//			{
//				Mysql.Record_Add("BUFFER_DATA", strTmp);
//			}
//			else
//			{
//				if (Mysql.Table_Create("AMT830", "BUFFER_DATA", 9, Mysql.mstr_buffer_table_name, Mysql.mn_buffer_table_type, Mysql.mn_buffer_table_num))
//				{
//					Mysql.Record_Add("BUFFER_DATA", strTmp);
//				}
//			}
//		}
//		else
//		{
//			if (Mysql.Database_Create("AMT830"))
//			{
//				if (Mysql.Table_Create("AMT830", "BUFFER_DATA", 9, Mysql.mstr_buffer_table_name, Mysql.mn_buffer_table_type, Mysql.mn_buffer_table_num))
//				{
//					Mysql.Record_Add("BUFFER_DATA", strTmp);
//				}
//			}
//		}
//	}
//	
//	Mysql.MySql_Close();
//	
//	g_cs.LeaveCriticalSection();
}

CString CPublic_Function::GetStrCurrentTime()
{
	COleDateTime timeCur;
	int iYear, iMonth, iDay, iHour, iMin, iSec;
	CString strTime;
	
	timeCur = COleDateTime::GetCurrentTime();

	iYear = timeCur.GetYear();
	iMonth = timeCur.GetMonth();
	iDay = timeCur.GetDay();

	iHour = timeCur.GetHour();
	iMin = timeCur.GetMinute();
	iSec = timeCur.GetSecond();

	strTime.Format("%04d/%02d/%02d_%02d:%02d:%02d", iYear, iMonth, iDay, iHour, iMin, iSec );
	return strTime;
}

void CPublic_Function::OnLogGalmg_PushLot( CString strlot, CString strtmp )
{
	CString strTemp;
	strTemp.Format( "[PUSH LOT] (%s) 0: [%s] / 1: [%s] / 2: [%s] : %s", strlot, st_handler.mstr_lot_name[0], st_handler.mstr_lot_name[1], st_handler.mstr_lot_name[2], strtmp );
	On_LogFile_Add(3, strTemp);
}

void CPublic_Function::OnLogWorkStep( CString strClass, CString strFunc, int iOld, int iCur )
{
	CString strLog;
	strLog.Format( "%s	%s			%d -------> %d", strClass, strFunc, iOld, iCur );
	On_LogFile_Add( LOG_WORK, strLog );
}

void CPublic_Function::OnLogRecord_Data( int iPos, int iWhere, bool bSet, CString strWhere )
{
	CString strLotNo = st_module_datamap[iPos][iWhere].mstrLotNo;
	CString strCount = st_module_datamap[iPos][iWhere].mstrMdlCnt;
	CString strLastModule = st_module_datamap[iPos][iWhere].mstrLastModule;
	int		nLastModule = st_module_datamap[iPos][iWhere].mnLastModule;
	
	CString strTemp;
	CString strPos = "FRONT";
	if ( iPos == POS_REAR )	strPos = "REAR";
	
	CString strW = "LDBUFFER 0";
	if ( iWhere == DATAMAP_SITE1 )	strW = "SITE 1";
	else if ( iWhere == DATAMAP_SITE2 )	strW = "SITE 2";
	else if ( iWhere == DATAMAP_SITE3 )	strW = "SITE 3";
	else if ( iWhere == DATAMAP_SITE4 )	strW = "SITE 4";
	else if ( iWhere == DATAMAP_ULDBUFFER )	strW = "ULDBUFFER 0";
	else if ( iWhere == DATAMAP_SHIFTER )	strW = "SHIFTER 0";

	CString strSet = "SET";
	if ( bSet == false )	strSet = "CLEAR";

	strTemp.Format( "Data %s %s %s %s %s %s %s %d", strLotNo, strPos, strW, strCount, strLastModule, strSet, strWhere, nLastModule );

	//On_LogFile_Add( LOG_RECORD, strTemp );
}

void CPublic_Function::MsgLog( LPCTSTR strMsg )
{
	if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
	{
		sprintf(st_msg.c_abnormal_msg, strMsg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
	}
}

void CPublic_Function::DeleteOldFiles( LPCTSTR lpDirPath, int iDays )
{
	if ( lpDirPath == NULL )
		return;

	CString strNextDirPath = "";
	CFileFind find;
	BOOL bFind = find.FindFile( lpDirPath );

	if ( bFind == FALSE )
		return;

	while( bFind )
	{
		bFind = find.FindNextFile();

		// . or .. pass
		if ( find.IsDots() == TRUE )
			continue;

		if ( find.IsDirectory() )
		{
			CString strDir;
			strDir.Format("%s\\*.*", find.GetFilePath() );
			DeleteOldFiles( strDir, iDays );
		}
		else
		{
			HANDLE hF = CreateFile(find.GetFileName(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

			SYSTEMTIME st;
			GetLocalTime(&st);
			
			CTime tNow(st.wYear, st.wMonth, st.wDay, 0, 0, 0);
			CTime tTime;
			find.GetLastWriteTime(tTime);
			
			CTimeSpan span = tNow - tTime;
			int iDays = span.GetDays();

			if ( span.GetDays() > iDays )
			{
				DeleteFile( find.GetFilePath() );

				CString strLog;
				strLog.Format("Delete File - Lost %d Days : %s", iDays, find.GetFilePath() );
				Func.On_LogFile_Add(3, strLog);
			}
		}
	}

	find.Close();
}

void CPublic_Function::SetRBufferStation( int iPos, int iVal, bool bWrite )
{
	if ( st_sync.mn_rbuffer_station[iPos] != iVal )
	{
		st_sync.mn_rbuffer_station[iPos] = iVal;

		if ( st_handler.cwnd_main != NULL )
		{
			st_handler.cwnd_main->SendMessage( WM_DRAW_WORK_SITE, 0, 0 );
		}

		if ( bWrite  )
		{
			std::string strPos = "Front";
			if ( iPos == 1 )
				strPos = "Rear";
			
//			char strKey[256];
//			sprintf( strKey, "%s%s_RBuffer", REG_KEY_SITE, strPos.c_str() );
//			AMTRegistry::RegWriteInt( strKey, REG_VALUE_RBUFFER_STATION, st_sync.mn_rbuffer_station[iPos] );
		}
	}
}

bool CPublic_Function::CheckWorkingLot( CString strLotNo )
{
	for( int i=0; i<3; i++ )
	{
		if ( st_handler.mstr_lot_name[i] == strLotNo )
			return true;
	}
	return false;
}

void CPublic_Function::_insertFistLot()
{
	for( int iPos=POS_REAR; iPos < POS_COUNT; iPos ++ )
	{
		CString strLotNo = st_module_datamap[iPos][DATAMAP_SHIFTER].mstrLotNo;
		if ( strLotNo == "" )
			continue;

		if ( CheckWorkingLot( strLotNo ) == true )
			continue;
		
		for( int i=1; i>= 0; i-- )
		{
			st_handler.mstr_lot_name[i + 1] == st_handler.mstr_lot_name[i];
		}
		st_handler.mstr_lot_name[0] = strLotNo;
		
		st_handler.mn_last_module_chk[0] = CTL_YES;
		st_handler.mstr_last_module = "YES";

		break;
	}
}

void CPublic_Function::On_Log_Title_Msg( int iLogType, CString strTitle, CString strMsg )
{
	CString strTemp;
	strTemp.Format("|%s| [%s]", strTitle, strMsg);
	On_LogFile_Add( iLogType, strTemp );
}

void CPublic_Function::MakeLogFolder(CString BaseDirectory, const CString strMsg)
{
	CString FilePath = BaseDirectory.Mid(BaseDirectory.GetLength() - 1);
	//====================================================
	// Drirectroy Name Check
	//====================================================
	
	if (FilePath != "\\")
		FilePath = BaseDirectory+"\\";
	else
		FilePath = BaseDirectory;
	
	CFileFind folder;
	
	
	CTime LogTime;
	LogTime = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format("%d년%02d월%02d일%02d시%02d분%02d초",LogTime.GetYear(),LogTime.GetMonth(),
		LogTime.GetDay(),LogTime.GetHour(),LogTime.GetMinute(),LogTime.GetSecond());
	
	int nThisYear   = LogTime.GetYear();
	int nThisMonth  = LogTime.GetMonth();
	int nThisDay    = LogTime.GetDay();
	int nThisHour	= LogTime.GetHour();
	
	CString filename, strMakeFolder,strDeleteFolder;
	CString strTemp;
	
	strMakeFolder.Format("%04d년", nThisYear);
	strMakeFolder = FilePath + strMakeFolder;
	if (nThisYear != m_nLastYear)
		::CreateDirectory(strMakeFolder, NULL);
	
	strTemp.Format("\\%02d월",nThisMonth);
	strMakeFolder = strMakeFolder+strTemp;
	if (nThisMonth != m_nLastMonth) //월 폴더 생성
	{
		::CreateDirectory(strMakeFolder, NULL);
		if (nThisMonth > 2)
		{
			strDeleteFolder.Format("%02d월",(nThisMonth-1));
			strDeleteFolder = FilePath + strDeleteFolder;
			FileAccess.DeleteAllFolderFile(strDeleteFolder,"*.*");
		}
		else if (nThisMonth == 2)
		{
			int temp = 12;
			strDeleteFolder.Format("%02d월",temp);
			strDeleteFolder = FilePath + strDeleteFolder;
			FileAccess.DeleteAllFolderFile(strDeleteFolder,"*.*");
		}
		else if (nThisMonth ==1)
		{
			int temp = 11;
			strDeleteFolder.Format("%02d월",temp);
			strDeleteFolder = FilePath + strDeleteFolder;
			FileAccess.DeleteAllFolderFile(strDeleteFolder,"*.*");
		}
		else;		
	}
	
	strTemp.Format("\\%02d일",nThisDay);
	strMakeFolder = strMakeFolder+strTemp;
	if (m_nLastDay != nThisDay) //날짜 폴더 생성
	{
		::CreateDirectory(strMakeFolder, NULL);
		Sleep(10);
	}
	
	strTemp.Format("\\%02d시", nThisHour);
	strMakeFolder = strMakeFolder + strTemp;
	if (m_nLastHour != nThisHour)
	{
		::CreateDirectory(strMakeFolder, NULL);
		Sleep(10);
	}
	
	//====================================================
	//만약 folder 가 생성 되지 않는다면 Return 을 시킨다.
	//====================================================
	
	CFileFind find;
	if (find.FindFile(strMakeFolder)==0)
		return;
	strTemp.Format("\\Seq%s.log",strTime);
	strMakeFolder = strMakeFolder + strTemp;
	
	if (find.FindFile(strMakeFolder)) // 
		return;
	
	FileAccess.WriteFile(strMakeFolder,strMsg);
	
	m_nLastYear = nThisYear;
	m_nLastMonth = nThisMonth;
	m_nLastDay = nThisDay;
	m_nLastHour = nThisHour;
}

void CPublic_Function::SetActionParam()
{
	pMainFrm = (CMainFrame*)::AfxGetMainWnd();   ////////프로그램이 실행 될때...MainFrm을 초기화 해준다..
	ASSERT(pMainFrm);
}

//==================================================================//
// 프로세스의 권한 설정
//==================================================================//
char CPublic_Function::GetNtPrivilege()
{
	HANDLE h_token;
	TOKEN_PRIVILEGES privilege_info;
	
	// 현재 프로세스의 권한과 관련된 정보를 변경하기 위해 토큰정보를 연다
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY, &h_token))
	{
		// 권한과 관련된 정보 접근에 실패함
		return 0;
	}
	
	// 현재 프로세스가 SE_SHUTDOWN_NAME 권한을 사용할수 있도록 설정한다
	LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &privilege_info.Privileges[0].Luid);
	privilege_info.PrivilegeCount = 1;
	privilege_info.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	// 지정한 값으로 권한을 조정한다
	AdjustTokenPrivileges(h_token, FALSE, &privilege_info, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS)
	{
		// 권한 조정에 실패한 경우
		return 0;
	}
	return 1;
}

void CPublic_Function::DateTimeChange(CString sDateTime)
{
	// 시간 변경
	SYSTEMTIME csTime;
	csTime.wYear	= atoi(sDateTime.Mid(0,4));
	csTime.wMonth	= atoi(sDateTime.Mid(4,2));
	csTime.wDay		= atoi(sDateTime.Mid(6,2));
	csTime.wHour	= atoi(sDateTime.Mid(8,2));
	csTime.wMinute	= atoi(sDateTime.Mid(10,2));
	csTime.wSecond	= atoi(sDateTime.Mid(12,2));
	csTime.wMilliseconds = 0;
	
	GetNtPrivilege();  // 프로세스의 권한 설정
	
	if (SetLocalTime(&csTime))
	{
		sprintf(st_msg.c_normal_msg, "시스템 날짜가 변경되었습니다. : %s", sDateTime);
		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
		{
			sprintf(st_msg.c_normal_msg, "The system date is changed : %s", sDateTime);
		}

		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}
	else
	{
		sprintf(st_msg.c_abnormal_msg, "시스템 날짜를 변경할 수 없습니다. : %s", sDateTime);
		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
		{
			sprintf(st_msg.c_abnormal_msg, "You can not change the system date. : %s", sDateTime);
		}

		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
	}
}

int CPublic_Function::Find_WhereToGoRobotPos(int nSite, int nPickerInfo[D_INFOSIZE][PICKER_NUM], int nDVCYesNo, int nInfoSetFlag, int nBinNum, int nPickPlace, int nDataResult[INFO_DATA], int nMode)
{
	int nFirstPicker = -1, nFirstPos_X = -1, nFirstPos_Y = -1, nFuncRet_Pos = 0, i = 0, nNum = 0, nRet = 0;
	
	nFuncRet_Pos = RET_GOOD;
	
	nDataResult[0] = 0;	// robot picker number info
	nDataResult[1] = 0;	// tray X pos info
	nDataResult[2] = 0;	// tray Y pos info.
	
	switch (nSite)
	{
	case BT_RETEST_BUFFER:
		for (i = 0 ; i < PICKER_NUM ; i++)
		{
			if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
			{
				if (nPickerInfo[0][i] == CTL_NO && st_modulemap.nPickerEnable[i] == CTL_YES)
				{
					if (st_modulemap.nRetestSocketUse[nMode * 10 + i] == CTL_YES)	// Retest하기 위해서 집는것이기 때문에 소켓을 확인해야 한다.
					{
						nFirstPicker = i; 
						nDataResult[0] = nFirstPicker; //picker num info.
						
						if (nInfoSetFlag == INFO_SETTING) st_work.nFirstPicker_num[nSite] = nFirstPicker;			
						break;
					}
				}
			}
			else
			{	// buffer에 놓을때, 피커에 디바이스가 있고, HTO, BTO, NTO만 놓는다.
				if (nPickerInfo[0][i] == CTL_YES && (nPickerInfo[1][i] == BIN_BP || nPickerInfo[1][i] == BIN_HTO || nPickerInfo[1][i] == BIN_BTO || nPickerInfo[1][i] == BIN_NTO) && nPickerInfo[2][i] == COUNT_TEST) 
				{
					nFirstPicker = i; 
					nDataResult[0] = nFirstPicker; //첫번째 동작가능 피커정보 저장 
					
					if (nInfoSetFlag == INFO_SETTING)	st_work.nFirstPicker_num[nSite] = nFirstPicker;
					break;
				}
			}
		}
		
		if (nFirstPicker == -1) 
		{
			nFuncRet_Pos = RET_PICKERNOTFIND; 
			break;
		}
		
		nFirstPos_X = 0;

		if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
		{
			nFirstPos_Y = Find_SitePosData(BT_RETEST_BUFFER, TRAY_Y, nBinNum, DVC_YES, COUNT_NO);
		}
		else
		{
			nFirstPos_Y = Find_SitePosData(BT_RETEST_BUFFER, TRAY_Y, BINCHECK_SKIP, DVC_NO, COUNT_NO);
		}
		
		if (nFirstPos_X == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	} //tray 첸지를 결정한다 
		if (nFirstPos_Y == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	}//tray 첸지를 결정한다 
		
		if (nInfoSetFlag == INFO_SKIP) break; //INFO_NO이면 tray X,Tray Y 정보를 update 않고 빠져 나간다, 참고용으로 사용시에는 바로 밑의 내용은 사용하면 안된다 
		
		if (nFirstPos_X >= 0 && nFirstPos_X < 1)
		{
			st_work.nFirst_X_num[nSite] = nFirstPos_X;
			nDataResult[1] = nFirstPos_X;  //tray X position info.
		}
		
		if (nFirstPos_Y >= 0 && nFirstPos_Y < 40)
		{
			st_work.nFirst_Y_num[nSite] = nFirstPos_Y;
			nDataResult[2] = nFirstPos_Y;  //tray Y position info.
		}

		nDataResult[1] = 0;							// Retest Buffer는 X가 한줄밖에 없다.
		break;

	case BT_LOB_BUFFER:
		for (i = 0 ; i < PICKER_NUM ; i++)
		{
			if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
			{
				if (nPickerInfo[0][i] == CTL_NO && st_modulemap.nPickerEnable[i] == CTL_YES)
				{
					nFirstPicker = i; 
					nDataResult[0] = nFirstPicker; //picker num info.
					
					if (nInfoSetFlag == INFO_SETTING) st_work.nFirstPicker_num[nSite] = nFirstPicker;			
					break;
				}
			}
			else
			{	// buffer에 놓을때, 피커에 디바이스가 있어야 한다.
				if (nPickerInfo[0][i] == CTL_YES && st_modulemap.nPickerEnable[i] == CTL_YES) 
				{
					nFirstPicker = i; 
					nDataResult[0] = nFirstPicker; //첫번째 동작가능 피커정보 저장 
					
					if (nInfoSetFlag == INFO_SETTING)	st_work.nFirstPicker_num[nSite] = nFirstPicker;
					break;
				}
			}
		}
		
		if (nFirstPicker == -1) 
		{
			nFuncRet_Pos = RET_PICKERNOTFIND; 
			break;
		}
		
		if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
		{
			nFirstPos_Y = Find_SitePosData(BT_LOB_BUFFER, TRAY_Y, BINCHECK_SKIP, DVC_YES, COUNT_NO);
			nFirstPos_X = Find_SitePosData(BT_LOB_BUFFER, TRAY_X, BINCHECK_SKIP, DVC_YES, COUNT_NO);
		}
		else
		{
			nFirstPos_Y = Find_SitePosData(BT_LOB_BUFFER, TRAY_Y, BINCHECK_SKIP, DVC_NO, COUNT_NO);
			nFirstPos_X = Find_SitePosData(BT_LOB_BUFFER, TRAY_X, BINCHECK_SKIP, DVC_NO, COUNT_NO);
		}
		
		if (nFirstPos_X == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	} //tray 첸지를 결정한다 
		if (nFirstPos_Y == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	}//tray 첸지를 결정한다 
		
		if (nInfoSetFlag == INFO_SKIP) break; //INFO_NO이면 tray X,Tray Y 정보를 update 않고 빠져 나간다, 참고용으로 사용시에는 바로 밑의 내용은 사용하면 안된다 
		
		if (nFirstPos_X >= 0 && nFirstPos_X < 2)
		{
			st_work.nFirst_X_num[nSite] = nFirstPos_X;
			nDataResult[1] = nFirstPos_X;  //tray X position info.
		}
		
		if (nFirstPos_Y >= 0 && nFirstPos_Y < 90)
		{
			st_work.nFirst_Y_num[nSite] = nFirstPos_Y;
			nDataResult[2] = nFirstPos_Y;  //tray Y position info.
		}
		break;

	case BT_REJECT_BUFFER:
		for (i = 0 ; i < PICKER_NUM ; i++)
		{
		}
		
		if (nFirstPicker == -1) 
		{
			nFuncRet_Pos = RET_PICKERNOTFIND; 
			break;
		}
		
		nFirstPos_X = 0;
		
		if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
		{
			nFirstPos_Y = Find_SitePosData(BT_REJECT_BUFFER, TRAY_Y, BINCHECK_SKIP, DVC_YES, COUNT_NO);
		}
		else
		{
			nFirstPos_Y = Find_SitePosData(BT_REJECT_BUFFER, TRAY_Y, BINCHECK_SKIP, DVC_NO, COUNT_NO);
		}
		
		if (nFirstPos_X == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	} //tray 첸지를 결정한다 
		if (nFirstPos_Y == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	}//tray 첸지를 결정한다 
		
		if (nInfoSetFlag == INFO_SKIP) break; //INFO_NO이면 tray X,Tray Y 정보를 update 않고 빠져 나간다, 참고용으로 사용시에는 바로 밑의 내용은 사용하면 안된다 
		
		if (nFirstPos_X >= 0 && nFirstPos_X < 1)
		{
			st_work.nFirst_X_num[nSite] = nFirstPos_X;
			nDataResult[1] = nFirstPos_X;  //tray X position info.
		}
		
		if (nFirstPos_Y >= 0 && nFirstPos_Y < REJECT_NUM)
		{
			st_work.nFirst_Y_num[nSite] = nFirstPos_Y;
			nDataResult[2] = nFirstPos_Y;  //tray Y position info.
		}
		
		nDataResult[1] = 0;							// Reject Buffer는 X가 한줄밖에 없다.
		break;

	case BT_CUSTOMER_BUFFER:
		for (i = 0 ; i < PICKER_NUM ; i++)
		{
			// Customer Buffer에는 Pass 자재만 놓는다.
			if (nPickerInfo[0][i] == CTL_YES && nPickerInfo[1][i] == BIN_PASS)
			{
				nFirstPicker = i; 
				nDataResult[0] = nFirstPicker; //첫번째 동작가능 피커정보 저장 
				
				if (nInfoSetFlag == INFO_SETTING)	st_work.nFirstPicker_num[nSite] = nFirstPicker;
				break;
			}
		}
		
		if (nFirstPicker == -1) 
		{
			nFuncRet_Pos = RET_PICKERNOTFIND; 
			break;
		}
		
		nFirstPos_X = 0;
		
		if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
		{
			nFirstPos_Y = Find_SitePosData(BT_CUSTOMER_BUFFER, TRAY_Y, BINCHECK_SKIP, DVC_YES, COUNT_NO);
		}
		else
		{
			nFirstPos_Y = Find_SitePosData(BT_CUSTOMER_BUFFER, TRAY_Y, BINCHECK_SKIP, DVC_NO, COUNT_NO);
		}
		
		if (nFirstPos_X == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	} //tray 첸지를 결정한다 
		if (nFirstPos_Y == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	}//tray 첸지를 결정한다 
		
		if (nInfoSetFlag == INFO_SKIP) break; //INFO_NO이면 tray X,Tray Y 정보를 update 않고 빠져 나간다, 참고용으로 사용시에는 바로 밑의 내용은 사용하면 안된다 
		
		if (nFirstPos_X >= 0 && nFirstPos_X < 1)
		{
			st_work.nFirst_X_num[nSite] = nFirstPos_X;
			nDataResult[1] = nFirstPos_X;  //tray X position info.
		}
		
		if (nFirstPos_Y >= 0 && nFirstPos_Y < REJECT_NUM)
		{
			st_work.nFirst_Y_num[nSite] = nFirstPos_Y;
			nDataResult[2] = nFirstPos_Y;  //tray Y position info.
		}
		
		nDataResult[1] = 0;							// Reject Buffer는 X가 한줄밖에 없다.
		break;

	case BT_TRAY_1:
// 		for (i = 0; i < PICKER_NUM; i++) 
// 		{
// 			if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
// 			{
// 				if (nPickerInfo[0][i] == CTL_NO) //디바이스를 잡고닜는 피커번호 
// 				{
// 					nFirstPicker = i; 
// 					nDataResult[0] = nFirstPicker;
// 					break;
// 				}
// 			}
// 			else
// 			{
// 				if (nPickerInfo[0][i] == CTL_YES && nPickerInfo[1][i] == nBinNum) //디바이스를 잡고닜는 피커번호 
// 				{
// 					nFirstPicker = i; 
// 					nDataResult[0] = nFirstPicker;
// 					break;
// 				}
// 			}
// 		}

		for (i = (PICKER_NUM - 1) ; i >= 0 ; i--) 
		{
			if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
			{
				if (nPickerInfo[0][i] == CTL_NO) //[0]:디바이스를 잡고닜는 피커번호, [1][*]:디바이스 빈 정보  
				{
					nFirstPicker = i;					
					nDataResult[0] = nFirstPicker; //첫번째 동작가능 피커정보 저장 
					break;
				}
			}
			else
			{
				if (nPickerInfo[0][i] == CTL_YES && nPickerInfo[1][i] == nBinNum) //[0]:디바이스를 잡고닜는 피커번호, [1][*]:디바이스 빈 정보  
				{
					nFirstPicker = i;					
					nDataResult[0] = nFirstPicker; //첫번째 동작가능 피커정보 저장 
					break;
				}
			}
		}

		if (nFirstPicker == -1) 
		{
			nFuncRet_Pos = RET_PICKERNOTFIND; 
			break;
		}

		if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
		{
			nFirstPos_X = Find_SitePosData(BT_TRAY_1, TRAY_X, BINCHECK_SKIP, DVC_YES, COUNT_NO); //세로 위치의 정보를 체크한다.
			nFirstPos_Y = Find_SitePosData(BT_TRAY_1, TRAY_Y, BINCHECK_SKIP, DVC_YES, COUNT_NO);
		}
		else
		{
			nFirstPos_X = Find_SitePosData(BT_TRAY_1, TRAY_X, BINCHECK_SKIP, DVC_NO, COUNT_NO); //세로 위치의 정보를 체크한다.
			nFirstPos_Y = Find_SitePosData(BT_TRAY_1, TRAY_Y, BINCHECK_SKIP, DVC_NO, COUNT_NO);
		}

		if (nFirstPos_X == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	} //tray 첸지를 결정한다 
		if (nFirstPos_Y == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	}//tray 첸지를 결정한다 

		if (nInfoSetFlag == INFO_SKIP) break; //INFO_NO이면 tray X,Tray Y 정보를 update 않고 빠져 나간다, 참고용으로 사용시에는 바로 밑의 내용은 사용하면 안된다 

		if (nFirstPos_X >= 0 && nFirstPos_X < st_basic.n_tray_x)
		{
			st_work.nFirst_X_num[nSite] = nFirstPos_X;
			nDataResult[1] = nFirstPos_X;  //tray X position info.
		}

		if (nFirstPos_Y >= 0 && nFirstPos_Y < st_basic.n_tray_y)
		{
			st_work.nFirst_Y_num[nSite] = nFirstPos_Y;
			nDataResult[2] = nFirstPos_Y;  //tray Y position info.
		}
		break;

	case BT_TRAY_2:
// 		for (i = 0; i < PICKER_NUM; i++) 
// 		{
// 			if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
// 			{
// 				if (nPickerInfo[0][i] == CTL_NO) //디바이스를 잡고닜는 피커번호 
// 				{
// 					nFirstPicker = i; 
// 					nDataResult[0] = nFirstPicker;
// 					break;
// 				}
// 			}
// 			else
// 			{
// 				if (nPickerInfo[0][i] == CTL_YES && nPickerInfo[1][i] == nBinNum) //디바이스를 잡고닜는 피커번호 
// 				{
// 					nFirstPicker = i; 
// 					nDataResult[0] = nFirstPicker;
// 					break;
// 				}
// 			}
// 		}

		for (i = (PICKER_NUM - 1) ; i >= 0 ; i--) 
		{
			if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
			{
				if (nPickerInfo[0][i] == CTL_NO) //[0]:디바이스를 잡고닜는 피커번호, [1][*]:디바이스 빈 정보  
				{
					nFirstPicker = i;					
					nDataResult[0] = nFirstPicker; //첫번째 동작가능 피커정보 저장 
					break;
				}
			}
			else
			{
				if (nPickerInfo[0][i] == CTL_YES && nPickerInfo[1][i] == nBinNum) //[0]:디바이스를 잡고닜는 피커번호, [1][*]:디바이스 빈 정보  
				{
					nFirstPicker = i;					
					nDataResult[0] = nFirstPicker; //첫번째 동작가능 피커정보 저장 
					break;
				}
			}
		}
		
		if (nFirstPicker == -1) 
		{
			nFuncRet_Pos = RET_PICKERNOTFIND; 
			break;
		}
		
		if (nPickPlace == PICKDVC) // 버퍼에서 집어낼때 
		{
			nFirstPos_X = Find_SitePosData(BT_TRAY_2, TRAY_X, BINCHECK_SKIP, DVC_YES, COUNT_NO); //세로 위치의 정보를 체크한다.
			nFirstPos_Y = Find_SitePosData(BT_TRAY_2, TRAY_Y, BINCHECK_SKIP, DVC_YES, COUNT_NO);
		}
		else
		{
			nFirstPos_X = Find_SitePosData(BT_TRAY_2, TRAY_X, BINCHECK_SKIP, DVC_NO, COUNT_NO); //세로 위치의 정보를 체크한다.
			nFirstPos_Y = Find_SitePosData(BT_TRAY_2, TRAY_Y, BINCHECK_SKIP, DVC_NO, COUNT_NO);
		}
		
		if (nFirstPos_X == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	} //tray 첸지를 결정한다 
		if (nFirstPos_Y == RET_NOWHERE) {	nFuncRet_Pos = RET_NOWHERE; break;	}//tray 첸지를 결정한다 
		
		if (nInfoSetFlag == INFO_SKIP) break; //INFO_NO이면 tray X,Tray Y 정보를 update 않고 빠져 나간다, 참고용으로 사용시에는 바로 밑의 내용은 사용하면 안된다 
		
		if (nFirstPos_X >= 0 && nFirstPos_X < st_basic.n_tray_x)
		{
			st_work.nFirst_X_num[nSite] = nFirstPos_X;
			nDataResult[1] = nFirstPos_X;  //tray X position info.
		}
		
		if (nFirstPos_Y >= 0 && nFirstPos_Y < st_basic.n_tray_y)
		{
			st_work.nFirst_Y_num[nSite] = nFirstPos_Y;
			nDataResult[2] = nFirstPos_Y;  //tray Y position info.
		}
		break;
	}
	return nFuncRet_Pos;
}

int CPublic_Function::Find_SiteDvc_count(int nSite, int nBinNum, int nDVCYesNo)
{
	int i = 0, nNum = 0, nFuncRet_Pos=0;
	 
	nNum = -1; 

	switch (nSite)
	{
	case PICKER_DVC_CNT: // 피커의 정보를 확인한다 
		for (i = 0; i < PICKER_NUM; i++)
		{
			if (nBinNum == BIN_RETEST)
			{
				if (st_modulemap.nDeviceRobotPicker[0][i] == nDVCYesNo && (st_modulemap.nDeviceRobotPicker[1][i] == BIN_HTO || st_modulemap.nDeviceRobotPicker[1][i] == BIN_NTO  
				|| st_modulemap.nDeviceRobotPicker[1][i] == BIN_BTO || st_modulemap.nDeviceRobotPicker[1][i] == BIN_BP)) 
				{
					nNum++;
				}
			}
			else if (nBinNum == BIN_CLEARBIN)
			{
				if (st_modulemap.nDeviceRobotPicker[0][i] == nDVCYesNo) 
				{
					nNum++;
				}
			}
			else 
			{
				if (st_modulemap.nDeviceRobotPicker[0][i] == nDVCYesNo && st_modulemap.nDeviceRobotPicker[1][i] == nBinNum) 
				{
					nNum++;
				}
			}
		}  
		break;
		
	case PICKER_ENABLE_INFO: // 피커의 정보를 확인한다 
		for (i = 0; i < PICKER_NUM; i++) // PICKER_NUM = 10
		{
			if (st_modulemap.nPickerEnable[i] == CTL_YES) 
			{
				nNum++;
			}
		}
		break;
	}

	if (nNum < 0)
	{
		nFuncRet_Pos = 0;
	}
	else nFuncRet_Pos = nNum + 1; //기본이 -1이니 +1 더한겨

	return nFuncRet_Pos;
}

CString CPublic_Function::OnRetestModuleCheck(int nPos, int nLast)		// 리테스트 가능 여부를 체크한다.
{
	CString strFuncRet = "NO";
	int nBufferCnt[2] = {0, 0};
	int nPickerCnt[2] = {0, 0};
	int i = 0;

	nPickerCnt[0] = st_buffer_tray[nPos].nRetestMdlCnt[0];
	nPickerCnt[1] = st_buffer_tray[nPos].nRetestMdlCnt[1];

	for (i = 0; i < RETEST_NUM; i++)
	{
		if (st_buffer_tray[BT_RETEST_BUFFER].nDeviceExistence[0][0][i] == CTL_YES && (st_buffer_tray[BT_RETEST_BUFFER].nDeviceExistence[1][0][i] == BIN_BTO || st_buffer_tray[BT_RETEST_BUFFER].nDeviceExistence[1][0][i] == BIN_HTO  || st_buffer_tray[BT_RETEST_BUFFER].nDeviceExistence[1][0][i] == BIN_BP))
		{
			nBufferCnt[0]++;
		}

		if (st_buffer_tray[BT_RETEST_BUFFER].nDeviceExistence[0][0][i] == CTL_YES && (st_buffer_tray[BT_RETEST_BUFFER].nDeviceExistence[1][0][i] == BIN_NTO))
		{
			nBufferCnt[1]++;
		}
	}

	if (nLast == 0)
	{
		if (nBufferCnt[1] + nPickerCnt[1] >= 10)
		{
			strFuncRet = "YES";
		}

		if (nBufferCnt[0] + nPickerCnt[0] >= 20)
		{
			strFuncRet = "YES";	
		}
	}
	else
	{
		if (nBufferCnt[1] + nPickerCnt[1] >= 1)
		{
			strFuncRet = "YES";
		}
		
		if (nBufferCnt[0] + nPickerCnt[0] >= 1)
		{
			strFuncRet = "YES";	
		}
	}

	return strFuncRet;
}

int CPublic_Function::OnRetestModuleRemainCheck()
{
	int nFuncRet = 0;
	int i = 0;

	for (i = 0; i < RETEST_NUM; i++)
	{
		if (st_buffer_tray[BT_RETEST_BUFFER].nDeviceExistence[0][0][i] == CTL_YES)
		{
			nFuncRet++;
		}
	}

	return nFuncRet;
}

int CPublic_Function::Find_SitePosData(int nSite, int nRowcolmode, int nBinnum, int nDvcyesno, int nCountmode)
{
	int i = 0, j = 0, nStart = 0, Num = 0, nFuncRet=0;
	 
	Num = -1; 

	switch (nSite)
	{
	case BT_RETEST_BUFFER:
		for (i = 0; i < 1; i++)
		{
			for (j = 0; j < RETEST_NUM; j++)
			{
				if (nBinnum == BIN_RETEST)
				{
					if (st_buffer_tray[BT_RETEST_BUFFER].nDeviceExistence[0][0][j] == nDvcyesno)	//NO == 디바이스가 없으면 
					{		
						if (nCountmode == 0) //위치체크이고, 카운터 체크가 아니면 
						{
							if (nRowcolmode == TRAY_X) Num =  i;		
							else Num = j;						
							i = 1;
							j = RETEST_NUM + 1;
						}
						else
						{	//카운터 체크 모드이면 
							if (nRowcolmode == TRAY_X) Num++;		
							else Num++;
						}			
					}
				}
				else
				{
					if (st_buffer_tray[BT_RETEST_BUFFER].nDeviceExistence[0][0][j] == nDvcyesno && st_buffer_tray[BT_RETEST_BUFFER].nDeviceExistence[1][0][j] == nBinnum)	//NO == 디바이스가 없으면 
					{		
						if (nCountmode == 0) //위치체크이고, 카운터 체크가 아니면 
						{
							if (nRowcolmode == TRAY_X) Num =  i;		
							else Num = j;						
							i = 1;
							j = RETEST_NUM + 1;
						}
						else
						{	//카운터 체크 모드이면 
							if (nRowcolmode == TRAY_X) Num++;		
							else Num++;
						}			
					}
				}
			}
		}		
		break;

	case BT_REJECT_BUFFER:
		for (i = 0; i < 1; i++)
		{
			for (j = 0; j < REJECT_NUM; j++)
			{
				if (st_buffer_tray[BT_REJECT_BUFFER].nDeviceExistence[0][0][j] == nDvcyesno)	//NO == 디바이스가 없으면 
				{		
					if (nCountmode == 0) //위치체크이고, 카운터 체크가 아니면 
					{
						if (nRowcolmode == TRAY_X) Num =  i;		
						else Num = j;						
						i = 1;
						j = REJECT_NUM + 1;
					}
					else
					{	//카운터 체크 모드이면 
						if (nRowcolmode == TRAY_X) Num++;		
						else Num++;
					}			
				}
			}
		}		
		break;

	case BT_CUSTOMER_BUFFER:
		for (i = 0; i < 1; i++)
		{
			for (j = 0; j < REJECT_NUM; j++)
			{
				if (st_buffer_tray[BT_CUSTOMER_BUFFER].nDeviceExistence[0][0][j] == nDvcyesno)	//NO == 디바이스가 없으면 
				{		
					if (nCountmode == 0) //위치체크이고, 카운터 체크가 아니면 
					{
						if (nRowcolmode == TRAY_X) Num =  i;		
						else Num = j;						
						i = 1;
						j = REJECT_NUM + 1;
					}
					else
					{	//카운터 체크 모드이면 
						if (nRowcolmode == TRAY_X) Num++;		
						else Num++;
					}			
				}
			}
		}		
		break;

	case BT_TRAY_1:	//unloading tray data
// 		for (i = 0; i < st_basic.n_tray_x; i++)
// 		{
// 			for (j = 0; j < st_basic.n_tray_y; j++)
// 			{
// 				if (st_buffer_tray[BT_TRAY_1].nDeviceExistence[0][i][j] == nDvcyesno)	//NO == 디바이스가 없으면 
// 				{		
// 					if (nCountmode == 0) //위치체크이고, 카운터 체크가 아니면 
// 					{
// 						if (nRowcolmode == TRAY_X) Num =  i;		
// 						else Num = j;						
// 						i = st_basic.n_tray_x + 1;
// 						j = st_basic.n_tray_y + 1;
// 					}
// 					else
// 					{	//카운터 체크 모드이면 
// 						if (nRowcolmode == TRAY_X) Num++;		
// 						else Num++;
// 					}			
// 				}
// 			}
// 		}		

		for (i = 0; i < st_basic.n_tray_x; i++)
		{
			for (j = (st_basic.n_tray_y - 1) ; j >= 0 ; j--)
			{
				if (st_buffer_tray[BT_TRAY_1].nDeviceExistence[0][i][j] == nDvcyesno) //NO == 디바이스가 없으면 
				{		
					if (nCountmode == 0) //위치체크이고, 카운터 체크가 아니면 
					{
						if (nRowcolmode == TRAY_X) Num =  i;		
						else Num = j;						
						i = st_basic.n_tray_x + 1;
						j = -1;
					}
					else
					{	//카운터 체크 모드이면 
						if (nRowcolmode == TRAY_X) Num++;		
						else Num++;
					}			
				}
			}
		}
		break;
		
	case BT_TRAY_2:	//unloading tray data
// 		for (i = 0; i < st_basic.n_tray_x; i++)
// 		{
// 			for (j = 0; j < st_basic.n_tray_y; j++)
// 			{
// 				if (st_buffer_tray[BT_TRAY_2].nDeviceExistence[0][i][j] == nDvcyesno)	//NO == 디바이스가 없으면 
// 				{		
// 					if (nCountmode == 0) //위치체크이고, 카운터 체크가 아니면 
// 					{
// 						if (nRowcolmode == TRAY_X) Num =  i;		
// 						else Num = j;						
// 						i = st_basic.n_tray_x + 1;
// 						j = st_basic.n_tray_y + 1;
// 					}
// 					else
// 					{	//카운터 체크 모드이면 
// 						if (nRowcolmode == TRAY_X) Num++;		
// 						else Num++;
// 					}			
// 				}
// 			}
// 		}	
		
		for (i = 0; i < st_basic.n_tray_x; i++)
		{
			for (j = (st_basic.n_tray_y - 1) ; j >= 0 ; j--)
			{
				if (st_buffer_tray[BT_TRAY_2].nDeviceExistence[0][i][j] == nDvcyesno) //NO == 디바이스가 없으면 
				{		
					if (nCountmode == 0) //위치체크이고, 카운터 체크가 아니면 
					{
						if (nRowcolmode == TRAY_X) Num =  i;		
						else Num = j;						
						i = st_basic.n_tray_x + 1;
						j = -1;
					}
					else
					{	//카운터 체크 모드이면 
						if (nRowcolmode == TRAY_X) Num++;		
						else Num++;
					}			
				}
			}
		}
		break;

	case BT_LOB_BUFFER:
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 90; j++)
			{
				if (st_buffer_tray[BT_LOB_BUFFER].nDeviceExistence[0][i][j] == nDvcyesno)	//NO == 디바이스가 없으면 
				{		
					if (nCountmode == 0) //위치체크이고, 카운터 체크가 아니면 
					{
						if (nRowcolmode == TRAY_X) Num =  i;		
						else Num = j;						
						i = 2;
						j = 91;
					}
					else
					{	//카운터 체크 모드이면 
						if (nRowcolmode == TRAY_X) Num++;		
						else Num++;
					}			
				}
			}
		}		
		break;
	}

	if (Num < 0)
	{
		nFuncRet = RET_NOWHERE;  
	}
	else 
	{
		if (nCountmode == 0) //위치체크이고, 카운터 체크가 아니면 
		{
			nFuncRet = Num;
		}
		else
		{
			nFuncRet = Num + 1; //기본이 -1이니 +1 더한다
		}
	}

	return nFuncRet;
}

void CPublic_Function::OnLotDataSave()
{
	CString mstr_file_name;    // 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file;  // 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name, mstr_temp_data;  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content;  // 한 라인에 출력할 문자열 정보 저장 변수 
	int mn_existence;      // 파일 존재 유무 설정 플래그 
	char fileName[256];    // 검색할 파일 정보 설정 함수 
	FILE  *fp ;            // 파일에 대한 포인터 설정 변수 
	
	CString str_title_time, str_file_time, str_new_file;		// 파일에 출력할 타이틀 시간 저장 변수
	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수
	int n_cur_hour, n_cur_minute, n_cur_second;		// 년, 월, 일 저장 변수
	
	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	CTime mp_time_cur;							// 타이틀 시간 저장 변수

	CString str_temp_data, strTemp;

	CString strData, str_time;
	
	/* ************************************************************************** */
	/* 파일 이름으로 사용할 날짜 설정한다                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	
	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  

	n_cur_hour = mtime_cur.GetHour();
	n_cur_minute = mtime_cur.GetMinute();
	n_cur_second = mtime_cur.GetSecond();
	
	str_new_file.Format("LOTDATA%04d%02d%02d[%s]", n_cur_year, n_cur_month, n_cur_day, st_handler.mstr_lot_name[0]);
	str_time.Format("%04d/%02d/%02d %02d:%02d:%02d", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_minute, n_cur_second);
		
	mstr_create_file = st_path.str_lot_data_path + str_new_file;
	mstr_create_file += ".TXT";
	
	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* 파일 미존재 */
	{
		mstr_create_file = st_path.str_lot_data_path + str_new_file;
		mstr_create_file += ".TXT";
	}
	
	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
	{
		Func.MsgLog("파일 열기 실패!..");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("Failed to open the file!..");
		}

		return;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 파일에 타이틀 정보 출력한다                                                */
	/* ************************************************************************** */
	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "AMT830 [MODULE HEAT SINK] : " + st_handler.mstr_handler_date);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "Lot Data Info File");
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "DIVISION  : AMT Coporation");
	fprintf(fp,"%s\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "File Name : " + st_path.str_lot_data_path + str_new_file);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "Save Time : " + str_time);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "Lot Name  : " + st_handler.mstr_lot_name[0]);
	fprintf(fp,"%s\r\n", mstr_content) ;

	if (st_handler.str_main_part.GetLength() >= 16)
	{
		mstr_content.Format("| %-100s |", "Part No.  : " + st_handler.str_main_part.Mid(0, 16));
		fprintf(fp,"%s\r\n", mstr_content) ;
	}

	strTemp.Format("%06d", st_work.nMdlInputCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Input    : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlPassCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Pass     : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nMdlRejectCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Reject   : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nMdlBtoCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Bto      : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nMdlNtoCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Nto      : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nMdlHtoCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Hto      : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	strTemp.Format("%06d", st_work.nMdlFailCount[0][0]);
	mstr_content.Format("| %-100s |", "Prime Fail     : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	if (st_work.nMdlPassCount[0][0] == 0)
	{
		st_work.fMdlyield[0][0] = 0;
	}
	else
	{
		st_work.fMdlyield[0][0] = (double)((double)st_work.nMdlPassCount[0][0] / (double)((double)st_work.nMdlPassCount[0][0] + (double)st_work.nMdlRejectCount[0][0])) * (double)100;
	}
	
	strTemp.Format("%3.2f%%", st_work.fMdlyield[0][0]);
	
	mstr_content.Format("| %-100s |", "Prime Yield    : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content);

	mstr_content.Format("| %-100s |", "" + strTemp);
	fprintf(fp,"%s\r\n", mstr_content);

	strTemp.Format("%06d", st_work.nMdlInputCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Input      : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlPassCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Pass       : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlRejectCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Reject     : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlBtoCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Bto        : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlNtoCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Nto        : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlHtoCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Hto        : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format("%06d", st_work.nMdlFailCount[1][0]);
	mstr_content.Format("| %-100s |", "Cum Fail       : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	if (st_work.nMdlPassCount[1][0] == 0)
	{
		st_work.fMdlyield[1][0] = 0;
	}
	else
	{
		st_work.fMdlyield[1][0] = (double)((double)st_work.nMdlPassCount[1][0] / (double)((double)st_work.nMdlPassCount[1][0] + (double)st_work.nMdlRejectCount[1][0])) * (double)100;
	}
	
	strTemp.Format("%3.2f%%", st_work.fMdlyield[1][0]);

	mstr_content.Format("| %-100s |", "Cum Yield      : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	str_temp_data = "";
	
	if (ferror(fp))  
	{
		Func.MsgLog("파일 저장 실패!..") ;
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("Failed to save the file!..");
		}

		return ;
	}
	
	fclose(fp);  /* 파일 종료 */
}

void CPublic_Function::OnDailyDataSave()
{
	CString mstr_file_name;    // 마지막으로 생성된 파일 이름 저장 변수 
	CString mstr_create_file;  // 알람 정보 저장할 파일에 대한 [폴더]+[파일명]+[확장자] 설정 변수 
	CString mstr_list_name, mstr_temp_data;  // 각 부분별 알람 발생 횟수 정보 저장할 로그 파일 정보 저장 변수 
	CString mstr_content;  // 한 라인에 출력할 문자열 정보 저장 변수 
	int mn_existence;      // 파일 존재 유무 설정 플래그 
	char fileName[256];    // 검색할 파일 정보 설정 함수 
	FILE  *fp ;            // 파일에 대한 포인터 설정 변수 
	
	CString str_title_time, str_file_time, str_new_file;		// 파일에 출력할 타이틀 시간 저장 변수
	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수
	int n_cur_hour, n_cur_minute, n_cur_second;		// 년, 월, 일 저장 변수
	
	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	CTime mp_time_cur;							// 타이틀 시간 저장 변수

	CString str_temp_data, strTemp;

	CString strData, str_time;
	CString strCnt;
	
//	float fMdlyield;
	
	/* ************************************************************************** */
	/* 파일 이름으로 사용할 날짜 설정한다                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	
	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  

	n_cur_hour = mtime_cur.GetHour();
	n_cur_minute = mtime_cur.GetMinute();
	n_cur_second = mtime_cur.GetSecond();
	
	str_new_file.Format("DAY_DATA%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	str_time.Format("%04d/%02d/%02d %02d:%02d:%02d", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_minute, n_cur_second);
		
	mstr_create_file = st_path.str_lot_data_path + str_new_file;
	mstr_create_file += ".TXT";
	
	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* 파일 미존재 */
	{
		mstr_create_file = st_path.str_lot_data_path + str_new_file;
		mstr_create_file += ".TXT";
	}
	
	/* ************************************************************************** */
	/* 알람 발생 횟수 정보 저장 파일에 추가 가능한 형태 파일로 생성               */
	/* ************************************************************************** */
	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
	{
		Func.MsgLog("파일 열기 실패!..");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("Failed to open the file!..");
		}

		return;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 파일에 타이틀 정보 출력한다                                                */
	/* ************************************************************************** */
	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "AMT830 [MODULE HEAT SINK] : " + st_handler.mstr_handler_date);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "Day Data Info File");
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "DIVISION  : AMT Coporation");
	fprintf(fp,"%s\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "File Name : " + st_path.str_lot_data_path + str_new_file);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "Save Time : " + str_time);
	fprintf(fp,"%s\r\n", mstr_content) ;

	////2014,0305
	strCnt.Format("%d", st_work.nMdlInputTotalCount);
	mstr_content.Format("| %-100s |", "Module_Input_Total_Count : " + strCnt);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strCnt.Format("%d", st_work.nMdlPassTotalCount);
	mstr_content.Format("| %-100s |", "Module_Pass_Total_Count : " + strCnt);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strCnt.Format("%d", st_work.nMdlRejectTotalCount);
	mstr_content.Format("| %-100s |", "Module_Reject_Total_Count : " + strCnt);
	fprintf(fp,"%s\r\n", mstr_content) ;
	////


	strTemp.Format( "%02d:%02d:%02d", st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds() );
	mstr_content.Format("| %-100s |", "Run Time : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format( "%02d:%02d:%02d", st_handler.m_tDS.GetTotalHours(), st_handler.m_tDS.GetMinutes(), st_handler.m_tDS.GetSeconds() );
	mstr_content.Format("| %-100s |", "Stop Time : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format( "%02d:%02d:%02d", st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds() );
	mstr_content.Format("| %-100s |", "Alarm Time : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	strTemp.Format( "%d", st_work.m_nAlarmCnt );// g_handler.GetAlarmCnt() );//2014,0305
	mstr_content.Format("| %-100s |", "Alarm Count : " + strTemp);
	fprintf(fp,"%s\r\n", mstr_content) ;

	int nSec_MTBI = st_handler.m_tDR.GetTotalHours() * 3600 + st_handler.m_tDR.GetMinutes() * 60 + st_handler.m_tDR.GetSeconds();

	if( st_work.m_nAlarmCnt == 0 )//g_handler.GetAlarmCnt() == 0 )
	{
		nSec_MTBI = 0;
		strTemp.Format( "%02d:%02d:%02d", st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds() );
		mstr_content.Format("| %-100s |", "MTBI : " + strTemp);
		fprintf(fp,"%s\r\n", mstr_content) ;
	}
	else
	{
		nSec_MTBI /= st_work.m_nAlarmCnt;// g_handler.GetAlarmCnt();

		strTemp.Format( "%02d:%02d:%02d", nSec_MTBI / 3600, (nSec_MTBI % 3600) / 60, nSec_MTBI % 60 );
		mstr_content.Format("| %-100s |", "MTBI : " + strTemp);
		fprintf(fp,"%s\r\n", mstr_content) ;
	}




	
	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	str_temp_data = "";
	
	if (ferror(fp))  
	{
		Func.MsgLog("파일 저장 실패!..") ;
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("Failed to save the file!..");
		}

		return ;
	}
	
	fclose(fp);  /* 파일 종료 */
}

void CPublic_Function::OnDayDataCheck()
{
	CString mstr_chk_temp;
	int mn_chk_year, mn_chk_month, mn_chk_day, mn_chk_hour; // Data 저장 시간정보 저장 변수 
	COleDateTime time_cur;
	int mn_cur_year, mn_cur_month, mn_cur_day, mn_cur_hour; // 현재 시간정보 저장 변수 
	
	mstr_chk_temp.Format("%10d", st_work.n_data_save_time);
	
	mn_chk_year = atoi(mstr_chk_temp.Mid(0, 4));
	mn_chk_month = atoi(mstr_chk_temp.Mid(4, 2));
	mn_chk_day = atoi(mstr_chk_temp.Mid(6, 2));
	mn_chk_hour = atoi(mstr_chk_temp.Mid(8, 2));
	
	time_cur = COleDateTime::GetCurrentTime();
	
	mn_cur_year = time_cur.GetYear();  
	mn_cur_month = time_cur.GetMonth();  
    mn_cur_day = time_cur.GetDay();  
	mn_cur_hour = time_cur.GetHour();
	
	// 현재 시간이 Day Start Time이 되기 전이라면...
	if (mn_cur_hour < st_basic.n_day_start_time)
	{
		if (mn_chk_day == mn_cur_day)						// 저장된 날짜와 현재 날짜가 같다면...
		{
			
		}
		else								// 같지 않다면 지난 Data이다.
		{
			if (mn_chk_hour < st_basic.n_day_start_time)	// 저장된 시간이 Day Start 시간보다 이전이면.. Reset이다.
			{
//				OnDayDataReset();			// Day Data Reset
			}
		}
	}
	else		// 현재 시간이 Day Start Time 이후라면...
	{
		if (mn_chk_day == mn_cur_day)						// 저장된 날짜와 현재 날짜가 같다면...
		{
			if (mn_chk_hour < st_basic.n_day_start_time)	// 저장된 시간이 Day Start 시간보다 이전이면.. Reset이다.
			{
//				OnDayDataReset();		// Day Data Reset
			}
		}
		else							// 같지 않다면 지난 Data이다.
		{
//			OnDayDataReset();			// Day Data Reset
		}
	}
}

void CPublic_Function::OnDayDataReset()
{
	CString mstr_temp;

	st_handler.m_tDR = 0;
	st_handler.m_tDJ = 0;
	st_handler.m_tDM = 0;
	st_handler.m_tDS = 0;

	st_work.m_nAlarmCnt = 0;//g_handler.ClearAlarmCnt();//2014,0305

	////2014,0305
	st_work.nMdlInputTotalCount = 0;
	st_work.nMdlPassTotalCount = 0;
	st_work.nMdlRejectTotalCount = 0;

	mstr_temp.Format("%d",st_work.nMdlInputTotalCount);
	:: WritePrivateProfileString("BasicData", "Daily_Input_Total_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_work.nMdlPassTotalCount);
	:: WritePrivateProfileString("BasicData", "Daily_Pass_Total_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_work.nMdlRejectTotalCount);
	:: WritePrivateProfileString("BasicData", "Daily_Reject_Total_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
	////


	mstr_temp.Format("%d",st_handler.m_tDR);
	:: WritePrivateProfileString("BasicData", "Daily_Run_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDS);
	:: WritePrivateProfileString("BasicData", "Daily_Stop_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDJ);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%d",st_handler.m_tDM);
	:: WritePrivateProfileString("BasicData", "Daily_Maint_Time", LPCTSTR(mstr_temp), st_path.mstr_basic);

	mstr_temp.Format("%d",st_work.m_nAlarmCnt);
	:: WritePrivateProfileString("BasicData", "Daily_Alarm_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);



	
	if (st_handler.cwnd_main != NULL)
	{
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);
	}
	
	st_work.mn_daily_fail_count = 0;
	
	mstr_temp.Format("%d", st_work.mn_daily_fail_count);
	:: WritePrivateProfileString("BasicData", "Daily_Fail_Count", LPCTSTR(mstr_temp), st_path.mstr_basic);
}

void CPublic_Function::WriteRegistry_LotNo()
{
	AMTRegistry::RegWriteString( REG_KEY_LOTNO, REG_VALUE_LOTNO_0, (LPCSTR)st_handler.mstr_lot_name[0] );
	AMTRegistry::RegWriteString( REG_KEY_LOTNO, REG_VALUE_LOTNO_1, (LPCSTR)st_handler.mstr_lot_name[1] );
	AMTRegistry::RegWriteString( REG_KEY_LOTNO, REG_VALUE_LOTNO_2, (LPCSTR)st_handler.mstr_lot_name[2] );
	AMTRegistry::RegWriteInt( REG_KEY_LOTNO, REG_VALUE_LOTNO_IDX, st_handler.mn_lot_name_chk );
}

void CPublic_Function::ReadRegistry_LotNo()
{
	st_handler.mstr_lot_name[0] = AMTRegistry::RegReadString( REG_KEY_LOTNO, REG_VALUE_LOTNO_0, "" ).c_str();
	st_handler.mstr_lot_name[1] = AMTRegistry::RegReadString( REG_KEY_LOTNO, REG_VALUE_LOTNO_1, "" ).c_str();
	st_handler.mstr_lot_name[2] = AMTRegistry::RegReadString( REG_KEY_LOTNO, REG_VALUE_LOTNO_2, "" ).c_str();
	st_handler.mn_lot_name_chk = AMTRegistry::RegReadInt( REG_KEY_LOTNO, REG_VALUE_LOTNO_IDX, 0 );
}

//////////////////////////////////////////// 20120521
int CPublic_Function::Barcode_Print()
{
	int mn_existence;   // 파일 존재 여부 설정 변수 
	int n_by= -1, n_bc = -1;
	int pos[3];
	int length;
	
	char fileName[256];   // 출력할 파일 정보 저장 변수 
	char buffer[1024+1]; // 파일 내용 중 한 라인 정보 임시 저장 변수 
	
	CString str_print_data;
	CString str_tmp;
	CString str_description; // 선택된 파일 내용 임시 저장 변수 
	
	sprintf(fileName, "C:\\AMT830\\BARCODE.TXT");
	
	mn_existence = access(fileName, 0);
	
	if (mn_existence != -1)  
	{
		FILE *fp = fopen ( fileName, "r" );
		
		if ( fp == (FILE *)NULL ) 
		{
			return CTLBD_RET_ERROR;
		}
		
		str_description.Empty();  // 파일 내용 저장 변수 초기화
		
		while (!feof(fp))
		{
			if (fgets(buffer, 1024, fp) == NULL)  break;
			
			if (strlen(buffer))   buffer[strlen(buffer)] = 0;
			strcat (buffer, "\r\n");
			str_description += buffer;
		}
		
		fclose(fp);
	}

	length = str_description.GetLength();
	
	pos[0] = str_description.Find("^BY", 0);
	pos[1] = str_description.Find("^FD", pos[0]);
	pos[2] = str_description.Find("^FS", pos[1]);
	
	str_print_data = str_description.Mid(0, pos[1]);
	str_print_data += st_work.str_barcode_tag;


// 	if( length >= pos[2] + 3 )
// 	{
		//str_print_data += str_description.Mid(pos[2]+3, -1); //자릿수 끝까지 
		str_print_data += str_description.Mid(pos[2]+3, length - (pos[2]+3));
// 	}

		
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

//////////////////////////////////////////////////// 20120625
int CPublic_Function::Print_Complete_Set()	//20120630
{
	CString str_print_data;

	str_print_data = "^XA^SXK,A,Y,Y^XZ";//20120906
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

int CPublic_Function::Print_Complete_Reset()	//20120630
{
	CString str_print_data;

	str_print_data = "^XA^SXK,A,Y,Y^XZ";//20120906
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}


//////////////////////////////////////////////////// 20120625
int CPublic_Function::Multi_Paper_Print(int str_Data)
{
	CString str_print_data;
	CString str_tmp;
	
	str_tmp.Format("%d", str_Data);
	str_print_data = "^XA^FO500,500^FD1234^FS";
	str_print_data += "^pq";
	str_print_data += str_tmp;
	str_print_data += "^XZ";
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

int CPublic_Function::Single_Paper_Print()
{
	CString str_print_data;

	str_print_data = "^XA~PH^XZ";	//^PH 사용가능
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

int CPublic_Function::Tear_Off_Mode()
{
	CString str_print_data;

	str_print_data = "^XA^MMT^XZ";	//Tear off
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

int CPublic_Function::Peel_Off_Mode()
{
	CString str_print_data;

	str_print_data = "^XA^MMP^XZ";	//Peel off
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

int CPublic_Function::Rewind_Mode()
{
	CString str_print_data;

	str_print_data = "^XA^MMR^XZ";	//Rewind
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

int CPublic_Function::Pause_Btn_On()
{
	CString str_print_data;

	str_print_data = "^XA~PP^XZ";	//^PP 사용가능
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

int CPublic_Function::Pause_Btn_Off()
{
	CString str_print_data;

	str_print_data = "^XA~PS^XZ";
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

int CPublic_Function::Cancel_Btn()
{
	CString str_print_data;

	str_print_data = "^XA~JP^XZ";
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

int CPublic_Function::Print_Calibrate_Btn()//20120920
{
	CString str_print_data;

	str_print_data = "^XA~JC^XZ";
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}


int CPublic_Function::Label_Printer_Status()
{
	CString str_print_data;

	str_print_data = "~HS";
	st_barcode.n_barcode_hs_cnt = 0;
	
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}

	return CTLBD_RET_GOOD;
}

int CPublic_Function::BCR_Trigger()
{
	int Ret=0, Ret_1=0, i=0; 
	int	FuncRet = RET_PROCEED;
	double dSpeedRatio[3] = {100,100,100};
	int ratio = 50;
	CString strClient;
	
	CDialog_Select  select_dlg;
	Func.ThreadFunctionStepTrace(86, m_auto_bcr_trigger_step);
	switch(m_auto_bcr_trigger_step)
	{
		case 0:
			nCheckTemp = 0;
			mn_retry = 0;//2014,0304
			m_auto_bcr_trigger_step = 100;
			break;

		case 100:	//BCR Trigger1 Ready check
			if(FAS_IO.Get_In_Bit(st_io.i_vision_ready1) == IO_ON)	//ready 상태 확인
			{
				if(st_handler.cwnd_list != NULL)
				{
//					sprintf(st_msg.c_normal_msg, "BCR Trigger1 Ready 상태!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}

				m_auto_bcr_trigger_step = 109;
			}
			else 
			{
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					FuncRet = RET_ERROR;
					m_auto_bcr_trigger_step = 0;
					sprintf(cJamcode, "560700");
					CTL_Lib.Alarm_Error_Occurrence(5035, CTL_dWARNING, cJamcode);
				}
			}
			break;

		case 109:
			FAS_IO.set_out_bit(st_io.o_vis_reset1_onoff, IO_ON);
			if(st_handler.cwnd_list != NULL)
			{
//				sprintf(st_msg.c_normal_msg, "BCR Reset1 신호 ON!!");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			l_trigger_wait_time[1][0] = GetCurrentTime();
			m_auto_bcr_trigger_step = 110;
			break;


		case 110:	//Trigger On
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 500) break;

			FAS_IO.set_out_bit(st_io.o_vis_reset1_onoff, IO_OFF);
			l_trigger_wait_time[1][0] = GetCurrentTime();
			if(st_handler.cwnd_list != NULL)
			{
//				sprintf(st_msg.c_normal_msg, "BCR Reset1 신호 Off!!");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			
			m_auto_bcr_trigger_step = 115;
			break;


		case 115:
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 100) break;
			
			if(FAS_IO.Get_In_Bit(st_io.i_vision_complste1) != IO_ON)
			{
				FAS_IO.set_out_bit(st_io.o_vis_trigger1_onoff, IO_ON);
				if(st_handler.cwnd_list != NULL)
				{
//					sprintf(st_msg.c_normal_msg, "BCR Trigger1 신호 ON!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				OnSet_Vision_send(VISION_TRIGGER);
				l_trigger_wait_time[1][0] = GetCurrentTime();

				m_auto_bcr_trigger_step = 116;
			}
			else
			{
				if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
				if(l_trigger_wait_time[1][2] < 500) break;

				if(l_trigger_wait_time[1][2] > 3000)
				{
					sprintf(cJamcode, "560702");
					CTL_Lib.Alarm_Error_Occurrence(5037, CTL_dWARNING, cJamcode);
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 에러!! - Complete가 Off 상태가 아님!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "BCR Trigger1 test error! - Complete the Off state is not!!");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					}
					m_auto_bcr_trigger_step = 110;
					FuncRet = RET_ERROR;
				}
			}
			break;

		case 116:
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 500) break;
			FAS_IO.set_out_bit(st_io.o_vis_trigger1_onoff, IO_OFF);
			if(st_handler.cwnd_list != NULL)
			{
//				sprintf(st_msg.c_normal_msg, "BCR Trigger1 신호 OFF!!");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			m_auto_bcr_trigger_step = 120;
			l_trigger_wait_time[1][0] = GetCurrentTime();
			break;

		case 120:  //Vision Complete & Fail check
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 500) break;
			
			if(FAS_IO.Get_In_Bit(st_io.i_vision_complste1) == IO_ON) //Complete 상태 확인
			{
				if(st_handler.cwnd_list != NULL)
				{
//					sprintf(st_msg.c_normal_msg, "BCR Trigger1 Complete 완료!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				l_trigger_wait_time[1][0] = GetCurrentTime();
				m_auto_bcr_trigger_step = 130;
			}
			else
			{
				nCheckTemp++;
				if (nCheckTemp > 2)
				{
					nCheckTemp = 0;
					if(l_trigger_wait_time[1][2] < 3000) break;
					FuncRet = RET_ERROR;
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 에러!! - Complete가 On 상태가 아님!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "BCR Trigger1 test error! - Complete the On state is not!!");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					}
					sprintf(cJamcode, "560706");
					CTL_Lib.Alarm_Error_Occurrence(5038, CTL_dWARNING, cJamcode);
					m_auto_bcr_trigger_step = 0;		
				}
				else
				{
					m_auto_bcr_trigger_step = 100;
				}
			}
			break;
			
		case 130:  //Vision Complete & Fail check
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 500) break;
			if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail1) == IO_ON)	//Fail 상태 확인
			{
				FuncRet = RET_DOOROPEN;
				//0907
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 !! - Fail 상태!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "BCR Trigger1 test! - Fail status!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}

				m_auto_bcr_trigger_step = 140;
				
				if (st_basic.mn_Label_Print_Error_Cnt > m_print_error_cnt )
				{
					m_print_error_cnt = m_print_error_cnt + 1;
					m_auto_bcr_trigger_step = 140;
				}
				else if (st_basic.mn_Label_Print_Error_Cnt == m_print_error_cnt )
				{
					m_print_error_cnt = 0;

//					st_msg.mstr_confirm_msg = _T("바코드 프린터 상태를 확인해 주세요.");						
					//select_dlg.DoModal();
					
					// 20140207
//					//560708 1 56 '바코드 프린터 상태를 확인해 주세요"
					sprintf(cJamcode, "560602");
					CTL_Lib.Alarm_Error_Occurrence(5049, CTL_dWARNING, cJamcode);
//
//					return -1;
				}

			}
			else if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail1) != IO_ON)	//성공 상태 확인
			{
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_normal_msg, "BCR Trigger1 테스트 !! - Success 상태!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
//				}

				m_print_error_cnt = 0;  // 20140228

				m_auto_bcr_trigger_step = 140;
				FuncRet = RET_GOOD;
			}

			break;
		////2014,0303
		case 140:
			if(st_other.b_SERIAL_NO_RECEIVE == TRUE)
			{
				m_auto_bcr_trigger_step = 0;
				st_other.b_SERIAL_NO_RECEIVE = FALSE;
				for (i = 0; i < 499; i++)
				{
					st_work.mstr_Vision_Read_LotNo[i] = st_work.mstr_Vision_Read_LotNo[i+1];
				}
								
				for (i = 0; i < 500; i++)
				{
					if( st_work.mstr_Vision_Read_LotNo[i] == st_work.mstr_vision_lotno)
					{
						//2015
						//sprintf(st_msg.c_abnormal_msg, "Vision에서 읽은 Lot No에 동일 Serial No(%s) 가 있습니다. ", st_work.mstr_vision_lotno);
						//if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						//{
						//	sprintf(st_msg.c_abnormal_msg, "Vision to read from the same Lot No Serial No(%s) to be. ", st_work.mstr_vision_lotno);
						//}

						//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , ABNORMAL_MSG);
					}
				}
				st_work.mstr_Vision_Read_LotNo[499] = (LPCTSTR)st_work.mstr_vision_lotno;
				Func.On_LogFile_Add(LOG_VISION, st_work.mstr_vision_lotno);
				st_work.mstr_vision_lotno = "";
					
			}
			else
			{
				m_auto_bcr_trigger_step = 0;
				st_other.b_SERIAL_NO_RECEIVE = FALSE;
				st_work.mstr_vision_lotno = "";
			}
			break;
		////
	}
	return FuncRet;
}

int CPublic_Function::HS_Trigger()
{
	int Ret=0, Ret_1=0, i=0; 
	int	FuncRet = RET_PROCEED;
	double dSpeedRatio[3] = {100,100,100};
	int ratio = 50;
	CString strClient;
/*
	CDialog_Select  select_dlg;

	switch(m_auto_hs_trigger_step)
	{
		case 0:
			nCheckTemp = 0;
			m_auto_hs_trigger_step = 100;
			break;

		case 100:	//BCR Trigger2 Ready check
			if(FAS_IO.Get_In_Bit(st_io.i_vision_ready2) == IO_ON)	//ready 상태 확인
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 Ready 상태!!");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				m_auto_hs_trigger_step = 109;
			}
			else 
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 에러!! - Trigger가 Ready 상태가 아님!!");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				sprintf(cJamcode, "560701");
				CTL_Lib.Alarm_Error_Occurrence(5039, CTL_dWARNING, cJamcode);
				m_auto_hs_trigger_step = 0;
				FuncRet = RET_ERROR;
			}
			break;

		case 105:
			FAS_IO.set_out_bit(st_io.o_vis_reset2_onoff, IO_ON);
			l_trigger_wait_time[1][0] = GetCurrentTime();
			m_auto_hs_trigger_step = 106;
			break;

		case 106:
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 500) break;
			FAS_IO.set_out_bit(st_io.o_vis_reset2_onoff, IO_OFF);
			m_auto_hs_trigger_step = 100;
			break;

		case 109:
			FAS_IO.set_out_bit(st_io.o_vis_reset2_onoff, IO_ON);
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "BCR Reset2 신호 ON!!");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			l_trigger_wait_time[1][0] = GetCurrentTime();
			m_auto_hs_trigger_step = 110;
			break;


		case 110:	//Trigger On
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 200) break;
			
			FAS_IO.set_out_bit(st_io.o_vis_reset2_onoff, IO_OFF);
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "BCR Reset2 신호 Off!!");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			l_trigger_wait_time[1][0] = GetCurrentTime();

			m_auto_hs_trigger_step = 115;
			break;

		case 115:
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 200) break;
			
			if(FAS_IO.Get_In_Bit(st_io.i_vision_complste2) != IO_ON)
			{
				::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_TRIGGER);
				FAS_IO.set_out_bit(st_io.o_vis_trigger2_onoff, IO_ON);
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 신호 ON!!");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				}
				l_trigger_wait_time[1][0] = GetCurrentTime();
				m_auto_hs_trigger_step = 117;
			}
		
			else if(st_other.b_HSRET == TRUE)
			{
				st_other.b_HSRET = FALSE;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, " HSRET 신호 On!!");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				}
				l_trigger_wait_time[1][0] = GetCurrentTime();
			}
			else
			{
				nCheckTemp++;
				if (nCheckTemp > 3)
				{
					nCheckTemp = 0;
					if(l_trigger_wait_time[1][2] > 3000)
					{
						FuncRet = RET_ERROR;
						if(st_handler.cwnd_list != NULL)
						{
							sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 에러!! - Complete가 Off 상태가 아님!!");
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
						}
						sprintf(cJamcode, "560703");
						CTL_Lib.Alarm_Error_Occurrence(5040, CTL_dWARNING, cJamcode);
						m_auto_hs_trigger_step = 0;
					}
				}
				else
				{
					m_auto_hs_trigger_step = 100;
				}
			}

			break;

		case 117:
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 100) break;
			
			FAS_IO.set_out_bit(st_io.o_vis_trigger2_onoff, IO_OFF);
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "BCR Trigger2 신호 OFF!!");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			l_trigger_wait_time[1][0] = GetCurrentTime();
			m_auto_hs_trigger_step = 120;
			break;
		
		case 120:  //Vision Complete & Fail check
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 500) break;
			
			if(st_other.b_HSRET == TRUE)
			{
				st_other.b_HSRET = FALSE;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, " HSRET 신호 On!!");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				l_trigger_wait_time[1][0] = GetCurrentTime();
				m_auto_hs_trigger_step = 115;
			}
			else if(FAS_IO.Get_In_Bit(st_io.i_vision_complste2) == IO_ON) //Complete 상태 확인
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 Complete 완료!!");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				m_auto_hs_trigger_step = 130;
			}
			else
			{
				if(l_trigger_wait_time[1][2] < 5000) break;
				FuncRet = RET_ERROR;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 에러!! - Complete가 On 상태가 아님!!");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				sprintf(cJamcode, "560707");
				CTL_Lib.Alarm_Error_Occurrence(5041, CTL_dWARNING, cJamcode);
				m_auto_hs_trigger_step = 0;
			}
			break;

		case 130:  //Vision Complete & Fail check
			if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail2) == IO_ON)	//Fail 상태 확인
			{
				FuncRet = RET_GOOD;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 !! - Fail 상태!!");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				m_auto_hs_trigger_step = 0;
//				st_msg.mstr_confirm_msg = _T("HEATSINK 조립 상태를 확인해 주세요.");						
//				select_dlg.DoModal();
//				return RET_ERROR;

			}
			else if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail2) != IO_ON)	//성공 상태 확인
			{
				FuncRet = RET_GOOD;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "BCR Trigger2 테스트 !! - Success 상태!!");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				m_auto_hs_trigger_step = 0;
			}
			break;
	}*/
	Func.ThreadFunctionStepTrace(87, m_auto_hs_trigger_step);
	switch(m_auto_hs_trigger_step)
	{
		case 0:
			nCheckTemp = 0;
			m_auto_hs_trigger_step = 100;
			break;

		case 100:	//BCR Trigger2 Ready check
			if(FAS_IO.Get_In_Bit(st_io.i_vision_ready2) == IO_ON)	//ready 상태 확인
			{
				if(st_handler.cwnd_list != NULL)
				{
//					sprintf(st_msg.c_normal_msg, "HS Trigger Ready 상태!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				m_auto_hs_trigger_step = 110;//109;
			}
			else 
			{
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 에러!! - Trigger가 Ready 상태가 아님!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "HS Trigger test error! - Trigger Not Ready state!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				sprintf(cJamcode, "560701");
				CTL_Lib.Alarm_Error_Occurrence(5039, CTL_dWARNING, cJamcode);
				m_auto_hs_trigger_step = 0;
				FuncRet = RET_ERROR;
			}
			break;

		case 110://109:
//			::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_TRIGGER_RESET);//20120927
			Func.OnSet_Vision_send(HEATSINK_TRIGGER_RESET);
//			FAS_IO.set_out_bit(st_io.o_vis_reset2_onoff, IO_ON);
			if(st_handler.cwnd_list != NULL)
			{
//				sprintf(st_msg.c_normal_msg, "HS Reset 신호 ON!!");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			l_trigger_wait_time[1][0] = GetCurrentTime();
			m_auto_hs_trigger_step = 120;//110;
			break;


		case 120://110:	//Trigger On
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 200) break;
			
//			FAS_IO.set_out_bit(st_io.o_vis_reset2_onoff, IO_OFF);
			if(st_handler.cwnd_list != NULL)
			{
//				sprintf(st_msg.c_normal_msg, "HS Reset 신호 Off!!");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			l_trigger_wait_time[1][0] = GetCurrentTime();

			m_auto_hs_trigger_step = 130;//115;
			break;

		case 130://115:
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 100) break;
			
			if(FAS_IO.Get_In_Bit(st_io.i_vision_complste2) != IO_ON)
			{
//				::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_TRIGGER);
				Func.OnSet_Vision_send(HEATSINK_TRIGGER);
//				FAS_IO.set_out_bit(st_io.o_vis_trigger2_onoff, IO_ON);
				if(st_handler.cwnd_list != NULL)
				{
//					sprintf(st_msg.c_normal_msg, "HS Trigger 신호 ON!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				}
				l_trigger_wait_time[1][0] = GetCurrentTime();
				m_auto_hs_trigger_step = 140;//117;
			}
		
			else if(st_other.b_HSRET == TRUE)
			{
				st_other.b_HSRET = FALSE;
				if(st_handler.cwnd_list != NULL)
				{
//					sprintf(st_msg.c_normal_msg, " HSRET 신호 On!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				}
				l_trigger_wait_time[1][0] = GetCurrentTime();
			}
			else
			{
// 				nCheckTemp++;
// 				if (nCheckTemp > 3)
// 				{
//					nCheckTemp = 0;
					if(l_trigger_wait_time[1][2] > 3000)
					{
						FuncRet = RET_ERROR;
						if(st_handler.cwnd_list != NULL)
						{
							sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 에러!! - Complete가 Off 상태가 아님!!");
							if(st_handler.mn_language == LANGUAGE_ENGLISH) 
							{
								sprintf(st_msg.c_normal_msg, "HS Trigger test error! - Complete the Off state is not!!");
							}

							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
						}
						sprintf(cJamcode, "560703");
						CTL_Lib.Alarm_Error_Occurrence(5040, CTL_dWARNING, cJamcode);
						m_auto_hs_trigger_step = 0;
					}
// 				}
// 				else
// 				{
// 					m_auto_hs_trigger_step = 100;
// 				}
			}

			break;

		case 140://117:
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 100) break;
			
//			FAS_IO.set_out_bit(st_io.o_vis_trigger2_onoff, IO_OFF);
			if(st_handler.cwnd_list != NULL)
			{
//				sprintf(st_msg.c_normal_msg, "HS Trigger 신호 OFF!!");
//				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			l_trigger_wait_time[1][0] = GetCurrentTime();
			m_auto_hs_trigger_step = 150;//120;
			break;
		
		case 150://120:  //Vision Complete & Fail check
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 100) break;
			
			if(st_other.b_HSRET == TRUE)
			{
				st_other.b_HSRET = FALSE;
				if(st_handler.cwnd_list != NULL)
				{
//					sprintf(st_msg.c_normal_msg, " HSRET 신호 On!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				l_trigger_wait_time[1][0] = GetCurrentTime();
				m_auto_hs_trigger_step = 130;//115;
			}
			else if(FAS_IO.Get_In_Bit(st_io.i_vision_complste2) == IO_ON) //Complete 상태 확인
			{
				if(st_handler.cwnd_list != NULL)
				{
//					sprintf(st_msg.c_normal_msg, "HS Trigger Complete 완료!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				m_auto_hs_trigger_step = 160;//130;
				l_trigger_wait_time[1][0] = GetCurrentTime();
			}
			else
			{
				if(l_trigger_wait_time[1][2] < 5000) break;
				FuncRet = RET_ERROR;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 에러!! - Complete가 On 상태가 아님!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "HS Trigger test error! - Complete the On state is not!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				sprintf(cJamcode, "560707");
				CTL_Lib.Alarm_Error_Occurrence(5041, CTL_dWARNING, cJamcode);
				m_auto_hs_trigger_step = 0;
			}
			break;

		case 160://130:  //Vision Complete & Fail check
			////////// 20120924
			l_trigger_wait_time[1][1] = GetCurrentTime();
			l_trigger_wait_time[1][2] = l_trigger_wait_time[1][1] - l_trigger_wait_time[1][0];
			if(l_trigger_wait_time[1][2] < 0) l_trigger_wait_time[1][0] = GetCurrentTime();
			if(l_trigger_wait_time[1][2] < 100) break;

			if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail2) == IO_ON && st_other.b_HS_Fail == TRUE)	//Fail 상태 확인 //20120924
			{
				st_other.b_HS_Fail = FALSE;//20120924
				FuncRet = RET_GOOD;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "HS Trigger 통신 & IO 테스트 !! - Fail 상태!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "HS Trigger Communications & IO test! - Fail status!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				m_auto_hs_trigger_step = 0;
				break;
			}
			else if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail2) != IO_ON && st_other.b_HS_Pass == TRUE)	//성공 상태 확인 //20120924
			{
				st_other.b_HS_Pass = FALSE;//20120924
				FuncRet = RET_GOOD;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_normal_msg, "HS Trigger 통신 & IO 테스트 !! - Success 상태!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
//				}
				m_auto_hs_trigger_step = 0;
				break;
			}
			else if(st_other.b_HS_Fail == TRUE)	//Fail 상태 확인 //20120924
			{
				st_other.b_HS_Fail = FALSE;//20120924
				FuncRet = RET_GOOD;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "HS Trigger 통신 테스트 !! - Fail 상태!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "HS Trigger Communications test! - Fail status!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				m_auto_hs_trigger_step = 0;
				break;
			}
			else if(st_other.b_HS_Pass == TRUE)	//성공 상태 확인 //20120924
			{
				st_other.b_HS_Pass = FALSE;//20120924
				FuncRet = RET_GOOD;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_normal_msg, "HS Trigger 통신 테스트 !! - Success 상태!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
//				}
				m_auto_hs_trigger_step = 0;
				break;
			}
			////////// 
			
			else if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail2) == IO_ON)	//Fail 상태 확인
			{
				st_other.b_HS_Fail = FALSE;//20120924
				FuncRet = RET_GOOD;
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 !! - Fail 상태!!");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						sprintf(st_msg.c_normal_msg, "HS Trigger test! - Fail status!!");
					}

					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
				}
				m_auto_hs_trigger_step = 0;
			}
			else if(FAS_IO.Get_In_Bit(st_io.i_vision_passfail2) != IO_ON)	//성공 상태 확인
			{
				st_other.b_HS_Pass = FALSE;//20120924
				FuncRet = RET_GOOD;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_normal_msg, "HS Trigger 테스트 !! - Success 상태!!");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
//				}
				m_auto_hs_trigger_step = 0;

			}
			break;
	}
	return FuncRet;
}

int CPublic_Function::Barcode_Create(int Print, int Status)
{
	int n_by= -1, n_bc = -1;
	int i;
	CString str_barcode;
	CString str_tmp, str_tmp1;
	CString str_description; // 선택된 파일 내용 임시 저장 변수 
	CString str_print_data;
	CString mstr_hidden_label;
	CString str_labelerrchk;

//^XA^MMR~SD14^LT10^LH15,35^PON^FS
//^FO337.50,2.50^A0,57.50,25.00^FD16GB 4Rx4 PC3L-8500R-07-10-F0-D2^FS
//^FO50.00,87.50^A0,37.50,37.50^FDMade in Korea^FS
//^FO300.00,67.50^A0,57.50,45.00^FDM393B2K70CM0-YF8^FS
//^FO750.00,68.75^A0,55.00,40.00^FD1223^FS
//^FO62.50,122.50^BY3,3.00,75.00^BC,N,N,0,N,N^FDKR M393B2K70CM0-YF8 1223^FS
//^FO875.00,0.00^BXN,6.25,200.00,,,,^FDY012A0122323AD2E77^FS
//^FO712.50,68.75^A0,55.00,40.00^FD^FS
//^FO837.50,0.25^A0R,25.00,20.00^FDY012A01223^FS
//^PQ1^XZ

//	if(Status == dRUN)// && st_basic.n_mode_device == 1)
//	{


		str_print_data = "";
		str_print_data = "^XA^MMR";

		str_tmp.Format("%02d", st_basic.mn_labelset_sd);
		str_print_data += "~SD" + str_tmp;
		str_tmp.Format("%02d", st_basic.mn_labelset_lt);
		str_print_data += "^LT" + str_tmp;

		str_tmp.Format("%02d", st_basic.mn_labelset_lh1);
		str_tmp1.Format("%02d", st_basic.mn_labelset_lh2);
		str_print_data += "^LH" + str_tmp + "," + str_tmp1;
		
		str_print_data += st_NW.mstr_Print_Data[Print];	

		str_labelerrchk = st_NW.mstr_Print_Data[Print].Mid(0,4);
		if(str_labelerrchk != "^PON")
		{
			return CTLBD_RET_ERROR;
		}

		////2014,0214
		for (i = 0; i < 499; i++)
		{
			st_work.m_strBarcode_OutPirnt[i] = st_work.m_strBarcode_OutPirnt[i+1];
			st_work.m_strLotNoChk[i]         = st_work.m_strLotNoChk[i+1];//2014,0218
		}
						
		for (i = 0; i < 500; i++)
		{
			if( st_work.m_strBarcode_OutPirnt[i] == st_NW.mstr_LotNoContents_1[Print] &&
				st_work.m_strLotNoChk[i] == st_NW.mstr_lot_id[1])//2014,0218
			{
				sprintf(st_msg.c_abnormal_msg, "동일 Lot No에 동일 Serial No(%s) 입니다. ", st_NW.mstr_LotNoContents_1[Print]);
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "Same Lot No, same Serial No(%s) is. ", st_NW.mstr_LotNoContents_1[Print]);
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , ABNORMAL_MSG);

				return CTLBD_RET_ERROR;
			}
		}
		////


		str_print_data += "^PQ1^XZ";
		Func.On_LogFile_Add(LOG_BARCODE, st_NW.mstr_LotNoContents_1[Print]);
		Func.On_LogFile_Add(99, str_print_data);
			
		if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
		{
			return CTLBD_RET_ERROR;
		}

		st_work.m_strBarcode_OutPirnt[499] = (LPCTSTR)st_NW.mstr_LotNoContents_1[Print];
		st_work.m_strLotNoChk[499] = (LPCTSTR)st_NW.mstr_lot_id[1];





	
/*	else if(Status == 0)
	{
		str_print_data = "";
		str_print_data = "^XA^MMR";

		str_tmp.Format("%02d", st_basic.mn_labelset_sd);
		str_print_data += "~SD" + str_tmp;
		str_tmp.Format("%02d", st_basic.mn_labelset_lt);
		str_print_data += "^LT" + str_tmp;

		// str_print_data = "^LH30,30"
		str_tmp.Format("%02d", st_basic.mn_labelset_lh1);
		str_tmp1.Format("%02d", st_basic.mn_labelset_lh2);
		str_print_data += "^LH" + str_tmp + "," + str_tmp1;

		str_print_data += st_NW.mstr_Unkown1[0];


////////// 20120906
		//						 ^FO387.5,12.5^A0,57.5,21.25^FD32GB 4Rx4 PC3-14900L-13-12-C0^FS
		// 1	str_print_data = ^FO337.5,2.5^A0,57.5,25^FD16GB 4Rx4 PC3L-8500R-07-10-F0-D2^FS
		str_tmp.Format("%4.2f", st_basic.md_labelset_x1_pos);
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", st_basic.md_labelset_y1_pos);
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_x1_width);
		str_print_data += "^A0," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_y1_height);
		str_print_data += "," + str_tmp;
		str_print_data += "^FD" + st_NW.mstr_1Contents_1[0];
		str_print_data += "^FS";


		//                       ^FO50,87.5^A0,37.5,37.5^FDMade in Korea^FS
		// 2	str_print_data = ^FO50,87.5^A0,37.5,37.5^FDMade in Korea^FS
		str_tmp.Format("%4.2f", st_basic.md_labelset_x2_pos);
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", st_basic.md_labelset_y2_pos);
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_x2_width);
		str_print_data += "^A0," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_y2_height);
		str_print_data += "," + str_tmp;

		str_print_data += "^FD" + st_NW.mstr_2Contents_1[0];
		
		str_print_data += "^FS";


		//                       ^FO287.5,80^A0,50,40^FDM386B4G70DM0-CMA4Q^FS
		// 3	str_print_data = ^FO300,67.5^A0,57.5,45^FDM393B2K70CM0-YF8^FS
		str_tmp.Format("%4.2f", st_basic.md_labelset_x3_pos);
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", st_basic.md_labelset_y3_pos);
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_x3_width);
		str_print_data += "^A0," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_y3_height);
		str_print_data += "," + str_tmp;

		str_print_data += "^FD" + st_NW.mstr_3Contents_1[0];// + " " + st_NW.mstr_3Contents_back_1[0];

		str_print_data += "^FS";
		
		//                           ^FO712.5,75^A0,55,27.5^FD1402^FS
		// 3 back 2	str_print_data = ^FO750,68.75^A0,55,40^FD1223^FS
		str_tmp.Format("%4.2f", atof(st_NW.mstr_3PosX_back_1[0]));
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", atof(st_NW.mstr_3PosY_back_1[0]));
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", atof(st_NW.mstr_3Wid_back_1[0]));
		str_print_data += "^A0," + str_tmp;

		str_tmp.Format("%4.2f", atof(st_NW.mstr_3High_back_1[0]));
		str_print_data += "," + str_tmp;

		str_print_data += "^FD" + st_NW.mstr_3Contents_back_1[0];
		str_print_data += "^FS";

	
		// 6	str_print_data = ^FO62.5,122.5^BY3,3,75^BC,N,N,0,N,N^FDKR M393B2K70CM0-YF8 1223^FS
		str_tmp.Format("%4.2f", st_basic.md_labelset_x6_pos);
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", st_basic.md_labelset_y6_pos);
		str_print_data += "," + str_tmp;

		str_print_data += "^" + st_NW.mstr_4Etc_1[0];

		str_tmp.Format("%4.2f", atof(st_NW.mstr_4Wid_1[0]));
		str_print_data += "," + str_tmp;
		
		str_tmp.Format("%4.2f", atof(st_NW.mstr_4High_1[0]));
		str_print_data += "," + str_tmp;

		str_print_data += "^" + st_NW.mstr_4Contents_1[0];

		str_print_data += "^FD" + st_NW.mstr_4Contents_2[0];
		str_print_data += "^FS";


		// 5	str_print_data = ^FO875,0^BXN,6.25,200,,,,^FDY012A0122323ACDF2E^FS
		str_tmp.Format("%4.2f", atof(st_NW.mstr_2DPosX_1[0]));
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", atof(st_NW.mstr_2DPosY_1[0]));
		str_print_data += "," + str_tmp;
 
		str_print_data += "^" + st_NW.mstr_2DEtc_1[0];
 
		str_tmp.Format("%4.2f", atof(st_NW.mstr_2DWid_1[0]));
		str_print_data += "," + str_tmp;
		
		str_tmp.Format("%4.2f", atof(st_NW.mstr_2DHigh_1[0]));
		str_print_data += "," + str_tmp;

		str_print_data += "," + st_NW.mstr_2DEtc_2[0];
		
		str_print_data += "^FD" + st_NW.mstr_2DContents_1[0].Mid(2,100);

		str_print_data += "^FS";

		// 3 back 1	str_print_data = ^FO712.5,68.75^A0,55,40^FDM^FS
		str_tmp.Format("%4.2f", atof(st_NW.mstr_Unkown2PosX_1[0]));
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", atof(st_NW.mstr_Unkown2PosY_1[0]));
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", atof(st_NW.mstr_Unkown2Wid_1[0]));
		str_print_data += "^A0," + str_tmp;

		str_tmp.Format("%4.2f", atof(st_NW.mstr_Unkown2High_1[0]));
		str_print_data += "," + str_tmp;

		str_print_data += "^FD" + st_NW.mstr_Unkown2_1[0];

		str_print_data += "^FS";



		// 4	str_print_data = ^FO837.5,.25^A0R,25,20^FDY012A01223^FS
		str_tmp.Format("%4.2f", atof(st_NW.mstr_LotNoPosX_1[0]));
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", atof(st_NW.mstr_LotNoPosY_1[0]));
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", atof(st_NW.mstr_LotNoWid_1[0]));
		str_print_data += "^A0R," + str_tmp;

		str_tmp.Format("%4.2f", atof(st_NW.mstr_LotNoHigh_1[0]));
		str_print_data += "," + str_tmp;

		str_print_data += "^FD" + st_NW.mstr_LotNoContents_1[0];

		str_print_data += "^FS";


		str_print_data += "^PQ1^XZ";

		Func.On_LogFile_Add(99, str_print_data);
	}
	else if(Status == 2)
	{	
		str_print_data = "";
		str_print_data = "^XA^MMR";

		str_tmp.Format("%02d", st_basic.mn_labelset_sd);
		str_print_data += "~SD" + str_tmp;
		str_tmp.Format("%02d", st_basic.mn_labelset_lt);
		str_print_data += "^LT" + str_tmp;

		// str_print_data = "^LH30,30"
		str_tmp.Format("%02d", st_basic.mn_labelset_lh1);
		str_tmp1.Format("%02d", st_basic.mn_labelset_lh2);
		str_print_data += "^LH" + str_tmp + "," + str_tmp1;

		str_print_data += "^PON^FS";

////////// 20120906
		// 1	str_print_data = ^FO337.5,2.5^A0,57.5,25^FD16GB 4Rx4 PC3L-8500R-07-10-F0-D2^FS
		str_tmp.Format("%4.2f", st_basic.md_labelset_x1_pos);
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", st_basic.md_labelset_y1_pos);
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_x1_width);
		str_print_data += "^A0," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_y1_height);
		str_print_data += "," + str_tmp;
		str_print_data += "^FD" + st_basic.mstr_barcode_data[0];
		str_print_data += "^FS";


		// 2	str_print_data = ^FO50,87.5^A0,37.5,37.5^FDMade in Korea^FS
		str_tmp.Format("%4.2f", st_basic.md_labelset_x2_pos);
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", st_basic.md_labelset_y2_pos);
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_x2_width);
		str_print_data += "^A0," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_y2_height);
		str_print_data += "," + str_tmp;

		str_print_data += "^FD" + st_basic.mstr_barcode_data[1];
		
		str_print_data += "^FS";


		// 3	str_print_data = ^FO300,67.5^A0,57.5,45^FDM393B2K70CM0-YF8^FS
		str_tmp.Format("%4.2f", st_basic.md_labelset_x3_pos);
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", st_basic.md_labelset_y3_pos);
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_x3_width);
		str_print_data += "^A0," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_y3_height);
		str_print_data += "," + str_tmp;

		str_print_data += "^FD" + st_basic.mstr_barcode_data[2];// + " " + st_NW.mstr_3Contents_back_1[0];

		str_print_data += "^FS";
		
		// 3 back 2	str_print_data = ^FO750,68.75^A0,55,40^FD1223^FS
		str_tmp.Format("%4.2f", atof(st_NW.mstr_3PosX_back_1[0]));
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", atof(st_NW.mstr_3PosY_back_1[0]));
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", atof(st_NW.mstr_3Wid_back_1[0]));
		str_print_data += "^A0," + str_tmp;

		str_tmp.Format("%4.2f", atof(st_NW.mstr_3High_back_1[0]));
		str_print_data += "," + str_tmp;

		str_print_data += "^FD";// + st_NW.mstr_3Contents_back_1[0];
		str_print_data += "^FS";

	
		// 4	str_print_data = ^FO837.5,.25^A0R,25,20^FDY012A01223^FS
		str_tmp.Format("%4.2f", st_basic.md_labelset_x4_pos);
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", st_basic.md_labelset_y4_pos);
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_x4_width);
		str_print_data += "^A0R," + str_tmp;

		str_tmp.Format("%4.2f", st_basic.md_labelset_y4_height);
		str_print_data += "," + str_tmp;

		str_print_data += "^FD" + st_basic.mstr_barcode_data[3];

		str_print_data += "^FS";

		// 5	str_print_data = ^FO875,0^BXN,6.25,200,,,,^FDY012A0122323ACDF2E^FS
		str_tmp.Format("%4.2f", st_basic.md_labelset_x5_pos);
		str_print_data += "^FO" + str_tmp;
		str_tmp.Format("%4.2f", st_basic.md_labelset_y5_pos);
		str_print_data += "," + str_tmp;
		str_print_data += "^BXN,6.25,200,,,,";		
		str_print_data += "^FD" + st_basic.mstr_barcode_data[3];

//		str_print_data += "23ACDF2E^FS";

		// 3 back 1	str_print_data = ^FO712.5,68.75^A0,55,40^FDM^FS
		str_tmp.Format("%4.2f", atof(st_NW.mstr_Unkown2PosX_1[0]));
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", atof(st_NW.mstr_Unkown2PosY_1[0]));
		str_print_data += "," + str_tmp;

		str_tmp.Format("%4.2f", atof(st_NW.mstr_Unkown2Wid_1[0]));
		str_print_data += "^A0," + str_tmp;

		str_tmp.Format("%4.2f", atof(st_NW.mstr_Unkown2High_1[0]));
		str_print_data += "," + str_tmp;

		str_print_data += "^FD" + st_NW.mstr_Unkown2_1[0];

		str_print_data += "^FS";

		// 6	str_print_data = ^FO62.5,122.5^BY3,3,75^BC,N,N,0,N,N^FDKR M393B2K70CM0-YF8 1223^FS
		str_tmp.Format("%4.2f", st_basic.md_labelset_x6_pos);
		str_print_data += "^FO" + str_tmp;
		
		str_tmp.Format("%4.2f", st_basic.md_labelset_y6_pos);
		str_print_data += "," + str_tmp;

		str_print_data += "^BY3,3,75^BC,N,N,0,N,N";

//		str_tmp.Format("%4.2f", atof(st_NW.mstr_4Wid_1[0]));
//		str_print_data += "," + str_tmp;
//		
//		str_tmp.Format("%4.2f", atof(st_NW.mstr_4High_1[0]));
//		str_print_data += "," + str_tmp;

//		str_print_data += "^BC,N,N,0,N,N";

		str_print_data += "^FDKR" + st_basic.mstr_barcode_data[2];
		str_print_data += "^FS";


		str_print_data += "^PQ1^XZ";

		Func.On_LogFile_Add(99, str_print_data);
//////////
	}
		
	if(OutLptPort(str_print_data) == CTLBD_RET_ERROR)
	{
		return CTLBD_RET_ERROR;
	}
*/
	return CTLBD_RET_GOOD;
}

////////////////////////////////////////////////////

int CPublic_Function::OutLptPort(CString str_text)
{
	HANDLE hLpt;
	DWORD dwWritten; 

	
    hLpt = CreateFile("LPT1:", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
    if (hLpt == INVALID_HANDLE_VALUE)
	{ 
		if(st_handler.cwnd_list != NULL)
		{
			if(COMI.mn_simulation_mode == 1)
			{
				return CTLBD_RET_GOOD;
			}

			sprintf(st_msg.c_abnormal_msg,"Can not open LPT1");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
 //       AfxMessageBox("Can not open LPT1 !!!");
		int a = GetLastError();
		CloseHandle(hLpt); //20120918
		return CTLBD_RET_ERROR;
	}
    else
	{ 
        WriteFile(hLpt, str_text, str_text.GetLength(), &dwWritten, NULL);
        CloseHandle(hLpt); 
    }


	return CTLBD_RET_GOOD;
}

//////////////////////////////////////////// 20120620
int CPublic_Function::OnBCR_CheckData(CString strCheckData, int port)
{
	//3.22 Keyence
	int     nLength;  // 데이터 총 길이 저장 변수 
	CString strBody;  // BODY 데이터 저장 변수 
	
	char  cp_cmd[1024];
	
	int     i;
	int     nStartIndex;//, nEndIndex;  // BCR 데이터의 시작 및 종료 위치 저장 변수 
	int     nReceivedDataCount=0;
	CString strTempDataCount;  // 데이터 갯수 저장 변수 
	CString strResultData[10];
	
	CString str_Tmp;
	CString str_Tmp_Data;
	
	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 
	
	// **************************************************************************
	// 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]                          
	// **************************************************************************
	nLength = strCheckData.GetLength();
	if(nLength < 3)
	{
		return FALSE;
	}
	// **************************************************************************
	
	strBody = strCheckData.Mid(0, nLength - 1);  // [테일] 제거 
	strBody.TrimLeft(' ');
	strBody.TrimRight(' ');
	
	// **************************************************************************
	// 에러 메시지 수신 유무 검사한다                                            
	// **************************************************************************
	if(strBody.Find("ERROR") >= 0)
	{
		sprintf(st_msg.c_normal_msg,"Barcode ERROR!!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
		return FALSE;
	}
	// **************************************************************************
	
	sprintf(cp_cmd, "%s", strCheckData);
	
	if(cp_cmd[nLength - 1] != 0x03)  // Tail 
	{
		return FALSE;
	}
		
	nReceivedDataCount = 0;
	nStartIndex = 0;
	// ==============================================================================
	// 실재로 읽은 BCR 데이터 전역 변수에 설정한다
	// ==============================================================================

	if (st_handler.cwnd_main != NULL)
	{
		sprintf(st_msg.c_abnormal_msg,"%s", strBody);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
	}
	for(i = 0 ; i < 1 ; i++)  // BCR_MAX_CNT == 8 
	{
		/*nEndIndex   = strBody.Find(",", nStartIndex);  // BCR 데이터 종료 위치 설정 
		
		if (nEndIndex == -1)
		{
			if (nLength - nStartIndex > 0)
			{
				nEndIndex = nLength - 1;
			}
			else
			{
				break;
			}
		}
		strResultData[i] = "";
		strResultData[i] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR 데이터 문자열 파싱 
		nReceivedDataCount++;
		nStartIndex = nEndIndex + 1;*/


		//////////////////////// 20120524
		if (st_handler.cwnd_main != NULL)
		{
			if (port == 3)
			{
				if(FAS_IO.get_in_bit(st_io.i_HS_Forward_Barcode_Up_chk, IO_ON) == IO_ON)
				{
					st_handler.mstr_bcr_id = strBody;//strResultData[i];
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, port );		
				}
				else
				{
					st_handler.mstr_bcr_id_1 = strBody;//strResultData[i];
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, port + 4 );		
				}
				break;
			}
			else if (port == 4)
			{
				if(FAS_IO.get_in_bit(st_io.i_HS_Backward_Barcode_Up_chk, IO_ON) == IO_ON)
				{
					st_handler.mstr_bcr_id2 = strBody;//strResultData[i];
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, port );		
				}
				else
				{
					st_handler.mstr_bcr_id2_1 = strBody;//strResultData[i];
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, port + 4 );		
				}
				break;
			}
			else if (port == 5)
			{
				if(FAS_IO.get_in_bit(st_io.i_Left_Clip_Barcode_Up_chk, IO_ON) == IO_ON)
				{
					st_handler.mstr_bcr_id3 = strBody;//strResultData[i];
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, port );		
				}
				else
				{
					st_handler.mstr_bcr_id3_1 = strBody;//strResultData[i];
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, port + 4 );		
				}
				break;
			}
			else if (port == 6)
			{
				if(FAS_IO.get_in_bit(st_io.i_Right_Clip_Barcode_Up_chk, IO_ON) == IO_ON)
				{
					st_handler.mstr_bcr_id4 = strBody;//strResultData[i];
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, port );		
				}
				else
				{
					st_handler.mstr_bcr_id4_1 = strBody;//strResultData[i];
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, port + 4 );		
				}
				break;
			}
		}
		//////////////////////// 
	}

	return TRUE;
}

int CPublic_Function::OnVision_CheckData(CString strCheckData)
{
	int     nLength;  // 데이터 총 길이 저장 변수 
	CString strBody;  // BODY 데이터 저장 변수 
	
	char  cp_cmd[1024];
	int i;
	
	int     nStartIndex, nEndIndex;  // BCR 데이터의 시작 및 종료 위치 저장 변수 
	int     nReceivedDataCount=0;
	CString strTempDataCount;  // 데이터 갯수 저장 변수 
	CString strResultData[10];
	
	CString str_Tmp;
	CString str_Tmp_Data;
	
	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 
	
	// **************************************************************************
	// 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]                          
	// **************************************************************************
	nLength = strCheckData.GetLength();
	if(nLength < 3)
	{
		return FALSE;
	}
	// **************************************************************************
	
	strBody = strCheckData.Mid(0, nLength - 1);  // [테일] 제거 
	strBody.TrimLeft(' ');
	strBody.TrimRight(' ');
	
	// **************************************************************************
	// 에러 메시지 수신 유무 검사한다                                            
	// **************************************************************************
	if(strBody.Find("ERROR") >= 0)
	{
		sprintf(st_msg.c_normal_msg,"Barcode ERROR!!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
		return FALSE;
	}
	// **************************************************************************
	
	if (st_interface.b_ml_select == TRUE)
	{
		strBody = strBody.Mid(1, nLength - 1);  // [테일] 제거 
		strBody.TrimLeft(' ');
		strBody.TrimRight(' ');
		strBody = strBody + ",";
		sprintf(cp_cmd, "%s", strBody );

		nLength = strBody.GetLength();
		
	}
	else
	{
		strBody = strCheckData.Mid(1, nLength - 1);  // [테일] 제거 
		strBody.TrimLeft(' ');
		strBody.TrimRight(' ');

		sprintf(cp_cmd, "%s", strBody);

		nLength = strBody.GetLength();
	
		if(cp_cmd[nLength - 1] != 0x03)  // Tail 
		{
			return FALSE;
		}	

	}
	
	nReceivedDataCount = 0;
	nStartIndex = 0;
	// ==============================================================================
	// 실재로 읽은 BCR 데이터 전역 변수에 설정한다
	// ==============================================================================
	if (st_interface.b_ml_select == FALSE)
		for(i = 0 ; i < BCR_MAX_CNT ; i++)  // BCR_MAX_CNT == 8 
		{
			nEndIndex   = strBody.Find(",", nStartIndex);  // BCR 데이터 종료 위치 설정 
			
			if (nEndIndex == -1)
			{
				if (nLength - nStartIndex > 0)
				{
					nEndIndex = nLength - 1;
				}
				else
				{
					break;
				}
			}
			strResultData[i] = "";
			strResultData[i] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR 데이터 문자열 파싱 
			nReceivedDataCount++;
			nStartIndex = nEndIndex + 1;
			
// 			//////////////////////// 20120524
 			if (st_handler.cwnd_main != NULL)
 			{
 				///////////////////////////////////////////////////// 20120608
 				if (i == 0)
 				{//
					////2014,0303
					str_Tmp.Format(strResultData[i]);//SERIAL_NO:Y07X00040923C7B0E8_OK or _ERR
 					str_Tmp_Data = str_Tmp.Mid(0,9);
 					if (str_Tmp_Data == "SERIAL_NO")
 					{
						str_Tmp_Data = str_Tmp.Mid(10,18);
						if(st_work.mstr_lot_no == str_Tmp_Data.Mid(0,10))
 						{
							st_work.mstr_vision_lotno = str_Tmp_Data;
							st_other.b_SERIAL_NO_RECEIVE = TRUE;
						}
						else
						{
							st_work.mstr_vision_lotno = str_Tmp_Data;
							st_other.b_SERIAL_NO_RECEIVE = FALSE;
						}
					}

					str_Tmp_Data = str_Tmp.Mid(0,10);
					if (str_Tmp_Data == "NO_BARCODE")//시리얼 번호만 못 읽은거 or 라벨에 내용이 없는거 
					{
						st_work.mstr_vision_lotno = str_Tmp_Data;
						st_other.b_SERIAL_NO_RECEIVE = FALSE;
					}

					str_Tmp_Data = str_Tmp.Mid(0,7);
					if (str_Tmp_Data == "NO_READ")//라벨이 없는거 이미지만 있을때
					{
						st_work.mstr_vision_lotno = str_Tmp_Data;
						st_other.b_SERIAL_NO_RECEIVE = FALSE;
					}

					
					////

 					str_Tmp.Format(strResultData[i]);
 					str_Tmp_Data = str_Tmp.Mid(0,4);
 					if (str_Tmp_Data == "BCRP")
 					{
 						st_handler.mn_vision_yield_yes[0] = atoi(str_Tmp.Mid(4,3));
 						st_handler.mn_vision_yield_yes[1] = atoi(str_Tmp.Mid(8,3));
 						st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, 0 );
						st_other.b_BCR_Data_Reset_OK = TRUE;//20120924

 					}
 					str_Tmp_Data = str_Tmp.Mid(0,3);
 					if (str_Tmp_Data == "HSP")
 					{
 						st_handler.mn_hs_yield_yes[0] = atoi(str_Tmp.Mid(3,3));
 						st_handler.mn_hs_yield_yes[1] = atoi(str_Tmp.Mid(7,3));
 						st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_BCR_DISPLAY, 1 );
						st_other.b_HS_Data_Reset_OK = TRUE;//20120924

 					}
 					////////////////////////////////////// 20120615
 					str_Tmp_Data = str_Tmp.Mid(0,5);
 					if (str_Tmp_Data == "BCRCJ")
 					{
						st_handler.n_bSendBcr = TRUE;
 						sprintf(st_msg.c_normal_msg,"BCR CJ 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response BCR CJ!!");
						}

 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
 					}
 					str_Tmp_Data = str_Tmp.Mid(0,7);
 					if (str_Tmp_Data == "BCRTKIN")
 					{
 						sprintf(st_msg.c_normal_msg,"BCR TKIN 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response BCR TKIN!!");
						}

 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
 					}
 					str_Tmp_Data = str_Tmp.Mid(0,8);
 					if (str_Tmp_Data == "BCRTKOUT")
 					{
 						sprintf(st_msg.c_normal_msg,"BCR TKOUT 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response BCR TKOUT!!");
						}

 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
 					}
 					str_Tmp_Data = str_Tmp.Mid(0,4);
 					if (str_Tmp_Data == "HSCJ")
 					{
						st_handler.n_bSendHeatSink = TRUE;
 						sprintf(st_msg.c_normal_msg,"HS CJ 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response HS CJ!!");
						}

 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
 					}
 					str_Tmp_Data = str_Tmp.Mid(0,6);
 					if (str_Tmp_Data == "HSTKIN")
 					{
 						sprintf(st_msg.c_normal_msg,"HS TKIN 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response HS TKIN!!");
						}
						

 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
 					}
 					str_Tmp_Data = str_Tmp.Mid(0,7);
 					if (str_Tmp_Data == "HSTKOUT")
 					{
 						sprintf(st_msg.c_normal_msg,"HS TKOUT 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response HS TKOUT!!");
						}

 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
 					}
					str_Tmp_Data = str_Tmp.Mid(0,5);
					if (str_Tmp_Data == "HSRET")
					{
						st_other.b_HSRET = TRUE;	//20120612
					}
					///////////////////////////////// 20120924
					str_Tmp_Data = str_Tmp.Mid(0,7);
					if (str_Tmp_Data == "HS_PASS")
					{
						st_other.b_HS_Pass = TRUE;
 						sprintf(st_msg.c_normal_msg,"HS PASS 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response HS PASS!!");
						}

 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					}
					str_Tmp_Data = str_Tmp.Mid(0,7);
					if (str_Tmp_Data == "HS_FAIL")
					{
						st_other.b_HS_Fail = TRUE;
 						sprintf(st_msg.c_normal_msg,"HS FAIL 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response HS FAIL!!");
						}

 						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					}
					//////////////////////////////////		
					
 					break;
 				/////////////////////////////////////////////////////
 				}
 			}

			//////////////////////// 20120608
			else if (st_handler.cwnd_interface != NULL)
			{
				if (i == 0)
				{
					str_Tmp.Format(strResultData[i]);
					str_Tmp_Data = str_Tmp.Mid(0,4);
					if (str_Tmp_Data == "BCRP")
					{
						st_handler.mn_vision_yield_yes[0] = atoi(str_Tmp.Mid(4,3));
						st_handler.mn_vision_yield_yes[1] = atoi(str_Tmp.Mid(8,3));
						st_handler.cwnd_interface->PostMessage(WM_WORK_END, INTERFACE_BCR_DISPLAY, 0 );
//						
						st_other.b_yield_ok = TRUE;	//20120612
					}
					str_Tmp_Data = str_Tmp.Mid(0,3);
					if (str_Tmp_Data == "HSP")
					{
						st_handler.mn_hs_yield_yes[0] = atoi(str_Tmp.Mid(3,3));
						st_handler.mn_hs_yield_yes[1] = atoi(str_Tmp.Mid(7,3));
						st_handler.cwnd_interface->PostMessage(WM_WORK_END, INTERFACE_BCR_DISPLAY, 1 );
//						
						st_other.b_yield_ok = TRUE;	//20120612
					}
					////////////////////////////////////// 20120615
					str_Tmp_Data = str_Tmp.Mid(0,5);
					if (str_Tmp_Data == "BCRCJ")
					{
						if (st_handler.cwnd_list != NULL)
						{
							st_handler.n_bSendBcr = TRUE;
							sprintf(st_msg.c_normal_msg, "BCR CJ 응답 완료!!");
							if(st_handler.mn_language == LANGUAGE_ENGLISH) 
							{
								sprintf(st_msg.c_normal_msg, "Complete response BCR CJ!!");
							}

							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
						}
					}
					str_Tmp_Data = str_Tmp.Mid(0,7);
					if (str_Tmp_Data == "BCRTKIN")
					{
						sprintf(st_msg.c_normal_msg,"BCR TKIN 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response BCR TKIN!!");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					}
					str_Tmp_Data = str_Tmp.Mid(0,8);
					if (str_Tmp_Data == "BCRTKOUT")
					{
						sprintf(st_msg.c_normal_msg,"BCR TKOUT 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response BCR TKOUT!!");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					}
					str_Tmp_Data = str_Tmp.Mid(0,4);
					if (str_Tmp_Data == "HSCJ")
					{
						st_handler.n_bSendHeatSink = TRUE;
						sprintf(st_msg.c_normal_msg,"HS CJ 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response HS CJ!!");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					}
					str_Tmp_Data = str_Tmp.Mid(0,6);
					if (str_Tmp_Data == "HSTKIN")
					{
						sprintf(st_msg.c_normal_msg,"HS TKIN 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response HS TKIN!!");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					}
					str_Tmp_Data = str_Tmp.Mid(0,7);
					if (str_Tmp_Data == "HSTKOUT")
					{
						sprintf(st_msg.c_normal_msg,"HS TKOUT 응답 완료!!");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							sprintf(st_msg.c_normal_msg, "Complete response HS TKOUT!!");
						}

						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
					}	
					str_Tmp_Data = str_Tmp.Mid(0,5);
					if (str_Tmp_Data == "HSRET")
					{
						st_other.b_HSRET = TRUE;	//20120612
					}
					break;
				}
			}
			else
			{
				str_Tmp.Format(strResultData[i]);
				str_Tmp_Data = str_Tmp.Mid(0,5);
				if (str_Tmp_Data == "HSRET")
				{
					st_other.b_HSRET = TRUE;	//20120612
				}
				break;
			}
			//////////////////////// 
		}
	else if(st_interface.b_ml_select == TRUE)
	{
		if(st_interface.b_vision_ml_select == TRUE)
		{
			for(i = 0 ; i < ML_MAX_CNT ; i++)  // BCR_MAX_CNT == 8 
			{
				nEndIndex   = strBody.Find(",", nStartIndex);  // BCR 데이터 종료 위치 설정 
				
				if (nEndIndex == -1)
				{
					if (nLength - nStartIndex > 0)
					{
						nEndIndex = nLength - 1;
					}
					else
					{
						if (st_handler.cwnd_interface != NULL)
						{
							st_handler.cwnd_interface->PostMessage(WM_WORK_END, INTERFACE_BCR_DISPLAY, 2 );
						}
						else if (st_handler.cwnd_basic != NULL) // 20130103
						{
							st_handler.cwnd_basic->PostMessage(WM_BCR_ML_END, BASIC_BCR_DISPLAY, 2 );
						}
						//						::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, VISION_ML_YES);
						Func.OnSet_Vision_send(VISION_ML_YES);
						st_other.b_ml_ok = TRUE;	//20120612
						st_interface.b_ml_select = FALSE;
						st_interface.b_vision_ml_select = FALSE;

						break;
					}
				}
				strResultData[i] = "";
				strResultData[i] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR 데이터 문자열 파싱 
				
				nReceivedDataCount++;
				nStartIndex = nEndIndex + 1;
				
				//////////////////////// 20120608
				if (st_handler.cwnd_interface != NULL)
				{
					////////////////////////////////////// 20120618
					if (i == 0)
					{
						str_Tmp_Data = strResultData[i].Mid(0,3);
						if (str_Tmp_Data == "BCR")
						{
							st_handler.mn_vision_ml_cnt = atoi(strResultData[i].Mid(3,1));	
						}
					}
					else
					{
						if (strResultData[i] != "")
						{
							st_handler.str_vision_ml_yes[i] = strResultData[i];	
						}	
					}				
				}
				//////////////////////// 
				else if (st_handler.cwnd_basic != NULL)                 //20130103
				{
					////////////////////////////////////// 20120618
					if (i == 0)
					{
						str_Tmp_Data = strResultData[i].Mid(0,3);
						if (str_Tmp_Data == "BCR")
						{
							st_handler.mn_vision_ml_cnt = atoi(strResultData[i].Mid(3,1));	
						}
					}
					else
					{
						if (strResultData[i] != "")
						{
							st_handler.str_vision_ml_yes[i] = strResultData[i];	
						}	
					}				
				}
			}
		}
		else if(st_interface.b_hs_ml_select == TRUE)
		{
			for(i = 0 ; i < ML_MAX_CNT ; i++)  // BCR_MAX_CNT == 8 
			{
				nEndIndex   = strBody.Find(",", nStartIndex);  // BCR 데이터 종료 위치 설정 
				
				if (nEndIndex == -1)
				{
					if (nLength - nStartIndex > 0)
					{
						nEndIndex = nLength - 1;
					}
					else
					{
						st_handler.cwnd_interface->PostMessage(WM_WORK_END, INTERFACE_BCR_DISPLAY, 3 );
//						::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_ML_YES);
						Func.OnSet_Vision_send(HEATSINK_ML_YES);
						st_other.b_ml_ok = TRUE;	//20120612
						st_interface.b_ml_select = FALSE;
						st_interface.b_hs_ml_select = FALSE;

						break;
					}
				}
				strResultData[i] = "";
				strResultData[i] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR 데이터 문자열 파싱 
				
				nReceivedDataCount++;
				nStartIndex = nEndIndex + 1;
				
				//////////////////////// 20120608
				if (st_handler.cwnd_interface != NULL)
				{
					////////////////////////////////////// 20120618
					if (i == 0)
					{
						str_Tmp_Data = strResultData[i].Mid(0,2);
						if (str_Tmp_Data == "HS")
						{
							st_handler.mn_hs_ml_cnt = atoi(strResultData[i].Mid(2,1));	
						}
					}
					else
					{
						if (strResultData[i] != "")
						{
							st_handler.str_hs_ml_yes[i] = strResultData[i];	
						}	
					}				
				}
				//////////////////////// 
			}
		}
	}

	return TRUE;

}

int CPublic_Function::OnLabelPrint_CheckData(CString strCheckData)
{

	//3.22 Keyence
	int     nLength;  // 데이터 총 길이 저장 변수 
	CString strBody;  // BODY 데이터 저장 변수 
	
	char  cp_cmd[1024];
	
	int     j;
	int     nStartIndex, nEndIndex;  // BCR 데이터의 시작 및 종료 위치 저장 변수 
	int     nReceivedDataCount=0;
	CString strTempDataCount;  // 데이터 갯수 저장 변수 
	CString strResultData[100];
	
	CString str_Tmp;
	CString str_Tmp_Data;
	
	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 
	
	// **************************************************************************
	// 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]                          
	// **************************************************************************
	nLength = strCheckData.GetLength();
	if(nLength < 3)
	{
		return FALSE;
	}
	// **************************************************************************
	strCheckData.TrimLeft("\r\n");
	strCheckData.TrimRight("\r\n");

	strBody = strCheckData;//.Mid(0, nLength - 1);  // [테일] 제거 
	strBody.TrimLeft(0x02);
	strBody.TrimRight(0x03);
	strBody = strBody + ",";
	
	// **************************************************************************
	// 에러 메시지 수신 유무 검사한다                                            
	// **************************************************************************
	if(strBody.Find("ERROR") >= 0)
	{
		sprintf(st_msg.c_normal_msg,"Label Print ERROR!!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
		return FALSE;
	}
	// **************************************************************************
	
	sprintf(cp_cmd, "%s", strBody);
	nLength = strBody.GetLength();
		
	nReceivedDataCount = 0;
	nStartIndex = 0;
	// ==============================================================================
	// 실재로 읽은 BCR 데이터 전역 변수에 설정한다
	// ==============================================================================
		for(j = 0 ; j < 13 ; j++)
		{

			nEndIndex   = strBody.Find(",", nStartIndex);  // BCR 데이터 종료 위치 설정 
			
			if (nEndIndex == -1)
			{
				if (nLength - nStartIndex > 0)
				{
					nEndIndex = nLength - 1;
				}
				else
				{
					break;
				}
			}
			strResultData[j] = "";
			strResultData[j] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR 데이터 문자열 파싱 
			nReceivedDataCount++;
			nStartIndex = nEndIndex + 1;
			
			if (st_handler.cwnd_barcode_print != NULL)
			{
 				if (st_barcode.n_barcode_hs_cnt == 0)
 				{
 					st_barcode.str_barcode_print[j] = strResultData[j];
					if (j == 1 ) 
					{
						st_barcode.n_barcode_hs_paper_out_error = atoi(st_barcode.str_barcode_print[j]);
					}
					else if (j == 2 ) 
					{
						st_barcode.n_barcode_hs_pause_error = atoi(st_barcode.str_barcode_print[j]);
					}					
					if (j == 11) 
					{
					////////////////////////////////////////// 20120621
					st_handler.str_printer_hs_list = strBody = strBody.Mid(0, nLength - 1);
					st_handler.cwnd_barcode_print->PostMessage(WM_WORK_END, BARCODE_HS_DISPLAY, 0 );
	                ////////////////////////////////////////// 

					st_barcode.n_barcode_hs_cnt += 1;
					break;
					}
 				}
 				else if (st_barcode.n_barcode_hs_cnt == 1)
 				{
 					st_barcode.str_barcode_print[j+12] = strResultData[j];
					if (j == 2 ) 
					{
						st_barcode.n_barcode_hs_head_up_error = atoi(st_barcode.str_barcode_print[j+12]);
					}
					else if (j == 3 ) 
					{
						st_barcode.n_barcode_hs_ribbon_out_error = atoi(st_barcode.str_barcode_print[j+12]);
					}					
					else if (j == 5 ) 
					{
						st_barcode.n_barcode_hs_print_mode = atoi(st_barcode.str_barcode_print[j+12]);
					}					
					
					if (j == 10) 
					{
					////////////////////////////////////////// 20120621
					st_handler.str_printer_hs_list2 = strBody = strBody.Mid(0, nLength - 1);
					st_handler.cwnd_barcode_print->PostMessage(WM_WORK_END, BARCODE_HS_DISPLAY, 1 );
	                ////////////////////////////////////////// 

					st_barcode.n_barcode_hs_cnt += 1;
					break;
					}
 				}
 				else if (st_barcode.n_barcode_hs_cnt == 2)
 				{
 					st_barcode.str_barcode_print[j+23] = strResultData[j];
					if (j == 1) 
					{
					////////////////////////////////////////// 20120621
					st_handler.str_printer_hs_list3 = strBody = strBody.Mid(0, nLength - 1);
					st_handler.cwnd_barcode_print->PostMessage(WM_WORK_END, BARCODE_HS_DISPLAY, 2 );
	                ////////////////////////////////////////// 

					st_barcode.n_barcode_hs_cnt = 0;
					break;
					}
 				}
			}
			else if (st_handler.cwnd_main != NULL)
			{
 				if (st_barcode.n_barcode_hs_cnt == 0)
 				{
 					st_barcode.str_barcode_print[j] = strResultData[j];
					if (j == 1 ) 
					{
						st_barcode.n_barcode_hs_paper_out_error = atoi(st_barcode.str_barcode_print[j]);
					}
					else if (j == 2 ) 
					{
						st_barcode.n_barcode_hs_pause_error = atoi(st_barcode.str_barcode_print[j]);
					}					
					if (j == 11) 
					{
					st_barcode.n_barcode_hs_cnt += 1;
					break;
					}
 				}
 				else if (st_barcode.n_barcode_hs_cnt == 1)
 				{
 					st_barcode.str_barcode_print[j+12] = strResultData[j];
					if (j == 2 ) 
					{
						st_barcode.n_barcode_hs_head_up_error = atoi(st_barcode.str_barcode_print[j+12]);
					}
					else if (j == 3 ) 
					{
						st_barcode.n_barcode_hs_ribbon_out_error = atoi(st_barcode.str_barcode_print[j+12]);
					}					
					else if (j == 5 ) 
					{
						st_barcode.n_barcode_hs_print_mode = atoi(st_barcode.str_barcode_print[j+12]);
					}					
					
					if (j == 10) 
					{
					st_barcode.n_barcode_hs_cnt += 1;
					break;
					}
 				}
 				else if (st_barcode.n_barcode_hs_cnt == 2)
 				{
 					st_barcode.str_barcode_print[j+23] = strResultData[j];
					if (j == 1) 
					{
					st_barcode.n_barcode_hs_cnt = 0;
					break;
					}
 				}
			}
		}
		
		
	return TRUE;
}

//////////////////////////////////////////////////////////// 20120707
int CPublic_Function::OnPaperOut_CheckData(CString strCheckData)
{

	//3.22 Keyence
	int     nLength;  // 데이터 총 길이 저장 변수 
	CString strBody;  // BODY 데이터 저장 변수 
	
	char  cp_cmd[1024];
	
	int     j;
	int     nStartIndex, nEndIndex;  // BCR 데이터의 시작 및 종료 위치 저장 변수 
	int     nReceivedDataCount=0;
	CString strTempDataCount;  // 데이터 갯수 저장 변수 
	CString strResultData[100];
	
	CString str_Tmp;
	CString str_Tmp_Data;
	
	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 
	
	// **************************************************************************
	// 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]                          
	// **************************************************************************
	nLength = strCheckData.GetLength();
	if(nLength < 3)
	{
		return FALSE;
	}
	// **************************************************************************
	strCheckData.TrimLeft("\r\n");
	strCheckData.TrimRight("\r\n");

	strBody = strCheckData;//.Mid(0, nLength - 1);  // [테일] 제거 
	strBody.TrimLeft(0x02);
	strBody.TrimRight(0x03);
	
	// **************************************************************************
	// 에러 메시지 수신 유무 검사한다                                            
	// **************************************************************************
	if(strBody.Find("ERROR") >= 0)
	{
		sprintf(st_msg.c_normal_msg,"Paper Out ERROR!!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
		return FALSE;
	}
	// **************************************************************************
	
	sprintf(cp_cmd, "%s", strBody);
	nLength = strBody.GetLength();
		
	nReceivedDataCount = 0;
	nStartIndex = 0;
	// ==============================================================================
	// 실재로 읽은 BCR 데이터 전역 변수에 설정한다
	// ==============================================================================
		for(j = 0 ; j < 6 ; j++)
		{

			nEndIndex   = strBody.Find(" ", nStartIndex);  // BCR 데이터 종료 위치 설정 
			
			if (nEndIndex == -1)
			{
				if (nLength - nStartIndex > 0)
				{
					nEndIndex = nLength - 1;
				}
				else
				{
					break;
				}
			}
			strResultData[j] = "";
			strResultData[j] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR 데이터 문자열 파싱 
			nReceivedDataCount++;
			nStartIndex = nEndIndex + 1;
			
			if (st_handler.cwnd_main == NULL)
			{
 				if (strResultData[j] == "COMPLETED")
 				{
					st_barcode.b_print_complete = TRUE;
					break;
				}
			}

		}
		
		
	return TRUE;
}


////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////

int CPublic_Function::OnBCR_Parsing(CString strSourceData, int *nEnableIndex, CString *strResultData)
{
/*	int     i;
	int     nLength;
	int     nStartIndex, nEndIndex;  // BCR 데이터의 시작 및 종료 위치 저장 변수 
	int     nReceivedDataCount=0, nEnableSetCount=0;
	CString strBody;           // 데이터부 저장 변수 
	CString strTempDataCount;  // 데이터 갯수 저장 변수 

	char cp_cmd[1024];

	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 

	// ==============================================================================
	// 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]
	// ==============================================================================
	nLength = strSourceData.GetLength();
	if(nLength < 3)
	{
		return FALSE;
	}
	// ==============================================================================

	// ==============================================================================
	// 수신 데이터에서 [헤더+TAIL] 제거한다
	// ==============================================================================
	strBody = strSourceData.Mid(1, nLength - 2);
	strBody.TrimLeft(' ');
	strBody.TrimRight(' ');
	// ==============================================================================

	if(strBody.Find("ERR") >= 0)
	{
		return FALSE;
	}

	sprintf(cp_cmd, "%s", strSourceData);

	// ==============================================================================
	// 수신 데이터 헤더 검사한다
	// ==============================================================================
	if(cp_cmd[0] != 0x02)
	{
		return FALSE;
	}
	// ==============================================================================

	// ==============================================================================
	// 수신 데이터 TAIL 검사한다
	// ==============================================================================
	if(cp_cmd[nLength - 1] != 0x03)
	{
		return FALSE;
	}
	// ==============================================================================

	if(st_barcode.iScanningMethod == BCR_FLYING)       // 8개 자재 한꺼번에 읽기 
	{
		if(strBody.Find("ST=0") >= 0 || strBody.Find("ST=5") >= 0)
		{
			nStartIndex = strBody.Find("n=", 0) + 2;
			strTempDataCount = strBody.Mid(nStartIndex, 2);
			strTempDataCount.TrimLeft(" _");
			strTempDataCount.TrimRight(" ");

			nReceivedDataCount = atoi(strTempDataCount);

			for(i = 0 ; i < BCR_MAX_CNT ; i++)  // BCR_MAX_CNT == 8 
			{
				if(nEnableIndex[i] == YES)
				{
					nEnableSetCount++ ;  // 모듈 존재 갯수 
				}
			}

			if(nReceivedDataCount > BCR_MAX_CNT)
			{
				return FALSE;
			}
			else if(nReceivedDataCount == 0)
			{
				return FALSE;
			}
			else if(nReceivedDataCount != nEnableSetCount)
			{
				return FALSE;
			}

			// ==============================================================================
			// 실재로 읽은 BCR 데이터 전역 변수에 설정한다
			// ==============================================================================
			for(i = 0 ; i < BCR_MAX_CNT ; i++)  // BCR_MAX_CNT == 8 
			{
				if(nEnableIndex[i] != YES)
				{
					strResultData[i] = BCR_NONE;
				}
				else
				{
					nStartIndex = strBody.Find(10, nStartIndex) + 1;  // BCR 데이터 시작 위치 설정 
					nEndIndex   = strBody.Find(10, nStartIndex) - 1;  // BCR 데이터 종료 위치 설정 

					strResultData[i] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR 데이터 문자열 파싱 

					nStartIndex = nEndIndex + 2;
				}
			}

			return TRUE;
			// ==============================================================================
		}

		if(strBody.Find("no  code") >= 0 || strBody.Find("n=_0") >= 0)
		{
			return FALSE;
		}
	}
	else if(st_barcode.iScanningMethod == BCR_STEPBYSTEP)
	{
		if ((strBody.Find("ST=0") >= 0) || (strBody.Find("ST=5") >= 0))
		{
			nStartIndex = strBody.Find("n=", 0) + 2;
			strTempDataCount = strBody.Mid(nStartIndex, 2);
			strTempDataCount.TrimLeft(" _");
			strTempDataCount.TrimRight(" ");

			nReceivedDataCount = atoi(strTempDataCount);

			if(nReceivedDataCount != 1)
			{
				return FALSE;
			}
			else
			{
				nStartIndex = strBody.Find(10, nStartIndex) + 1;
				nEndIndex   = strBody.Find(10, nStartIndex) - 1;

				strResultData[(TEST_PARA - 1) - st_barcode.i_read_work_pos] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);

				return TRUE;
			}
		}

		if (strBody.Find("no  code") >= 0 ||
			strBody.Find("n=_0") >= 0)
		{
			strResultData[(TEST_PARA - 1) - st_barcode.i_read_work_pos] = _T("NO CODE");

			return FALSE;
		}
	}

	// ==============================================================================
	// 동일한 조건 검사 [ECHO 데이터]
	// ==============================================================================
	if (strBody == _T("11") ||
		strBody == _T("12") ||
		strBody == _T("13") ||
		strBody == _T("15") ||
		strBody == _T("1K") ||
		strBody == _T("21") ||
		strBody == _T("22"))
	{
		return BCR_ECHO;
	}
	// ==============================================================================

	return FALSE;*/
	int     i;
	int     nLength;
	int     nStartIndex, nEndIndex;  // BCR 데이터의 시작 및 종료 위치 저장 변수 
	int     nReceivedDataCount=0, nEnableSetCount=0;
	CString strBody;           // 데이터부 저장 변수 
	CString strTempDataCount;  // 데이터 갯수 저장 변수 

	char cp_cmd[1024];

	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 

	// ==============================================================================
	// 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]
	// ==============================================================================
	nLength = strSourceData.GetLength();
	if(nLength < 3)
	{
		return FALSE;
	}
	// ==============================================================================

	// ==============================================================================
	// 수신 데이터에서 [TAIL] 제거한다
	// ==============================================================================
	strBody = strSourceData.Mid(0, nLength - 1);
	strBody.TrimLeft(' ');
	strBody.TrimRight(' ');
	// ==============================================================================

	if(strBody.Find("ERROR") >= 0)
	{
		return FALSE;
	}

	sprintf(cp_cmd, "%s", strSourceData);

	// ==============================================================================
	// 수신 데이터 TAIL 검사한다
	// ==============================================================================
	if(cp_cmd[nLength - 1] != 0x03)
	{
		return FALSE;
	}
	// ==============================================================================

	if(st_barcode.iScanningMethod == BCR_FLYING)       // 8개 자재 한꺼번에 읽기 
	{
		for(i = 0 ; i < 1 ; i++)  // BCR_MAX_CNT == 8 
		{
			if(nEnableIndex[i] == YES)
			{
				nEnableSetCount++ ;  // 모듈 존재 갯수 
			}
		}

		nReceivedDataCount = 0;
		nStartIndex = 0;
		// ==============================================================================
		// 실재로 읽은 BCR 데이터 전역 변수에 설정한다
		// ==============================================================================
		for(i = 0 ; i < 1/*10*/ ; i++)  // BCR_MAX_CNT == 8 
		{
			//ybs 4.2
			/*if(nEnableIndex[i] == YES)
			{
				nEndIndex   = strBody.Find(",", nStartIndex);  // BCR 데이터 종료 위치 설정 
				
				if (nEndIndex == -1)
				{
					if (nLength - nStartIndex > 0)
					{
						nEndIndex = nLength - 1;
					}
					else
					{
						break;
					}
				}
				
				strResultData[i] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR 데이터 문자열 파싱 
				nReceivedDataCount++;
				nStartIndex = nEndIndex + 1;
			}*/
			nReceivedDataCount++;
			strResultData[i] = strBody;
		}
		
		// 확인용 vibox
		//st_other.str_op_normal_msg.Format("Total : %02d, [1:%s, 2:%s, 3:%s, 4:%s, 5:%s, 6:%s, 7:%s, 8:%s, 9:%s, 10:%s]", nReceivedDataCount
		//	, strResultData[0], strResultData[1], strResultData[2], strResultData[3], strResultData[4], strResultData[5], strResultData[6], strResultData[7]
		//	, strResultData[8], strResultData[9]);
		//st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

		if(nReceivedDataCount > BCR_MAX_CNT)
		{
			return FALSE;
		}
		else if(nReceivedDataCount == 0)
		{
			return FALSE;
		}
		else if(nReceivedDataCount != nEnableSetCount)
		{
			return FALSE;
		}

		return TRUE;
		// ==============================================================================
	}
	else if(st_barcode.iScanningMethod == BCR_STEPBYSTEP)
	{
		nReceivedDataCount = 0;
		nStartIndex = 0;
		// ==============================================================================
		// 실재로 읽은 BCR 데이터 전역 변수에 설정한다
		// ==============================================================================
		for(i = 0 ; i < BCR_MAX_CNT ; i++)  // BCR_MAX_CNT == 8 
		{
			//ybs 4.2
			if(nEnableIndex[i] == YES)
			{

				nEndIndex   = strBody.Find(",", nStartIndex);  // BCR 데이터 종료 위치 설정 
				
				if (nEndIndex == -1)
				{
					if (nLength - nStartIndex > 0)
					{
						nEndIndex = nLength - 1;
					}
					else
					{
						break;
					}
				}
				
				//strResultData[(TEST_PARA - 1) - st_barcode.i_read_work_pos] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);
				strResultData[(PICKER_NUM - 1) - st_barcode.i_read_work_pos] = strBody.Mid(nStartIndex, nEndIndex - nStartIndex);
				nReceivedDataCount++;
				nStartIndex = nEndIndex + 1;
			}
		}
		
		// 확인용 vibox
		//st_other.str_op_normal_msg.Format("Total : %02d, [1:%s, 2:%s, 3:%s, 4:%s, 5:%s, 6:%s, 7:%s, 8:%s, 9:%s, 10:%s]", nReceivedDataCount
		//	, strResultData[0], strResultData[1], strResultData[2], strResultData[3], strResultData[4], strResultData[5], strResultData[6], strResultData[7]
		//	, strResultData[8], strResultData[9]);
		//st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

		if (nReceivedDataCount == 0)
		{
			strResultData[(PICKER_NUM - 1) - st_barcode.i_read_work_pos] = _T("NO CODE");

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

int CPublic_Function::OnBCR_Command(int nCommand, CString *strCreatingCommand)
{
	if(strCreatingCommand == NULL)
	{
		return FALSE;
	}

	switch(nCommand)
	{
	case BCR_READON :
//		strCreatingCommand->Format("%cESC Z CR%c", 0x02, 0x03);  // [STX+명령어+ETX] : [21:READ ON] 
//		strCreatingCommand->Format("%cESC Z CR%c");  // [STX+명령어+ETX] : [21:READ ON] 		
//		strCreatingCommand->Format("6983673290326782");//Format("%c6983673290326782%c", 0x02, 0x03);  // [STX+명령어+ETX] : [21:READ ON] 

//		strCreatingCommand->Format("%c21%c", 0x02, 0x03);  // [STX+명령어+ETX] : [21:READ ON] 
//		strCreatingCommand->Format("%cLON%c", 0x02, 0x03);  // [STX+명령어+ETX] : [21:READ ON] 
		strCreatingCommand->Format("%cY%c", 0x02, 0x03);  // [STX+명령어+ETX] : [Y:READ ON] 
		break;

	case BCR_READOFF :
//		strCreatingCommand->Format("%c22%c", 0x02, 0x03);  // [STX+명령어+ETX] : [22:READ OFF] 
//		strCreatingCommand->Format("%cLOFF%c", 0x02, 0x03);  // [STX+명령어+ETX] : [22:READ OFF] 
		strCreatingCommand->Format("%cZ%c", 0x02, 0x03);  // [STX+명령어+ETX] : [Z:READ ON] 
		break;

	default :
		strCreatingCommand->Format("UNDEFINE");
		return FALSE;
		break;
	}

	return TRUE;
}

int CPublic_Function::Robot_BarcodeScanning(int nPort)
{
	int Ret=0, Ret_1=0, FuncRet, i=0;
	FuncRet = RET_PROCEED;


	switch(BarcodeScanStep)
	{
	case 0:
		//st_barcode.strBarcode.Empty();

		l_until_wait_time[0] = GetCurrentTime();

		BcrScanMoreSlow = TRUE;
		BcrScanMoreSlowAgain = FALSE;
		st_barcode.iScanningMethod = BCR_FLYING;
		iBarcodeScanRetryCnt = 0;

		//BarcodeScanStep = 100;  //일단 스케닝 방향은 왼쪽으로 고정이다!! (메뉴얼 동작으로 사용하기로 함)	
		
		st_serial.str_rec[nPort] = _T("");
		BarcodeScanStep = 1000; 
		break;
		
	case 1000:
		// ==============================================================================================================
		// 바코드 스케닝을 시작하자!
		// ==============================================================================================================
		st_client[NETWORK_CLIENT_7].n_client_send_flag = NO;//20121120
		if(Func.OnBCR_Command(BCR_READON, &str_command))
		{
			st_serial.n_snd_chk[nPort] = COM_READY;
			st_serial.str_snd[nPort] = str_command;                  // 송신 데이터 설정
			l_until_wait_time[0] = GetCurrentTime();

			::PostMessage(st_handler.hWnd, WM_DATA_SEND, nPort, 0);  // 데이터 송신 요청

			BarcodeScanStep = 2000;
		}
		else
		{
			//전송할 데이터 생성 오류다~~ 알람발생~
			//510100 0 51 "HS Front Barcode Send Data Make Error."
			//511100 0 51 "HS Back Barcode Send Data Make Error."
			//512100 0 51 "Clip Left Barcode Send Data Make Error."
			//513100 0 51 "Clip Right Barcode Send Data Make Error."
			if (nPort == 3) alarm.mstr_pcode = _T("510100"); 
			if (nPort == 4) alarm.mstr_pcode = _T("511100"); 
			if (nPort == 5) alarm.mstr_pcode = _T("512100"); 
			if (nPort == 6) alarm.mstr_pcode = _T("513100"); 

			alarm.mn_count_mode = 0;	
			alarm.mn_type_mode = eWARNING; 
			st_work.mn_run_status = dWARNING;

			alarm.mn_alarm_assign_section = 178;

			BarcodeScanStep = 8000;
		}
		break;

	case 2000:
		// ==============================================================================================================
		// 바코드 스케너가 켜졌는지 확인!
		// ==============================================================================================================
		if(st_serial.n_snd_chk[nPort] == TRUE) // 보냈다
		{
			BarcodeScanStep = 2100;
		}
		else if(st_serial.n_snd_chk[nPort] == COM_ERROR) // 통신상 무언가 오류가 발생했다..
		{
			//전송 요청했지만 완료가 안되어졌다.. 알람발생
			//510000 0 51 "HS Front Barcode Data Send Error."
			//511000 0 51 "HS Back Barcode Data Send Error."
			//512000 0 51 "Clip Left Barcode Data Send Error."
			//513000 0 51 "Clip Right Barcode Data Send Error."
			if (nPort == 3) alarm.mstr_pcode = _T("510000"); 
			if (nPort == 4) alarm.mstr_pcode = _T("511000"); 
			if (nPort == 5) alarm.mstr_pcode = _T("512000"); 
			if (nPort == 6) alarm.mstr_pcode = _T("513000"); 

			alarm.mn_count_mode = 0;	
			alarm.mn_type_mode = eWARNING; 
			st_work.mn_run_status = dWARNING;

			alarm.mn_alarm_assign_section = 179;

			BarcodeScanStep = 8000;
		}
		else
		{
			l_until_wait_time[1] = GetCurrentTime();
			l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];

			if(l_until_wait_time[2] > BCR_SND_WAIT)
			{
				//전송 요청했지만 완료가 안되어졌다.. 시간초과~  알람발생
				//510002 0 51 "HS Front Barcode Serial Port Timeout Error."
				//511002 0 51 "HS Back Barcode Serial Port Timeout Error."
				//512002 0 51 "Clip Left Barcode Serial Port Timeout Error."
				//513002 0 51 "Clip Right Barcode Serial Port Timeout Error."

				if (nPort == 3) alarm.mstr_pcode = _T("510002"); 
				if (nPort == 4) alarm.mstr_pcode = _T("511002"); 
				if (nPort == 5) alarm.mstr_pcode = _T("512002"); 
				if (nPort == 6) alarm.mstr_pcode = _T("513002"); 

				alarm.mn_count_mode = 0;	
				alarm.mn_type_mode = eWARNING; 
				st_work.mn_run_status = dWARNING;
				alarm.mn_alarm_assign_section = 180;
				BarcodeScanStep = 8000;
			}
		}
		break;

	case 2100:
		////////20121120
		if(st_client[NETWORK_CLIENT_7].n_client_send_flag == YES)
		{
		////////
			st_barcode.iScanningOnFlag = FALSE;	
			l_until_wait_time[0] = GetCurrentTime();
			BarcodeScanStep = 2200;
		}
		break;

	case 2200:
		// ==============================================================================================================
		// 바코드 스케닝을 종료하자!
		// ==============================================================================================================
		/////////////////////////////////////// 20121120
		st_server[0].n_rev_bcr = NO;
		st_client[NETWORK_CLIENT_7].n_client_send_flag = NO;
		///////////////////////////////////////
		
		if(Func.OnBCR_Command(BCR_READOFF, &str_command))
		{
			st_serial.n_snd_chk[nPort] = COM_READY;
			st_serial.str_snd[nPort] = str_command;                  // 송신 데이터 설정
			l_until_wait_time[0] = GetCurrentTime();

			st_serial.n_rec_chk[nPort] = COM_READY;

			::PostMessage(st_handler.hWnd, WM_DATA_SEND, nPort, 0);  // 데이터 송신 요청

			BarcodeScanStep = 2300;
		}
		else
		{
			//전송할 데이터 생성 오류다~~ 알람발생~
			//510100 0 51 "HS Front Barcode Send Data Make Error."
			//511100 0 51 "HS Back Barcode Send Data Make Error."
			//512100 0 51 "Clip Left Barcode Send Data Make Error."
			//513100 0 51 "Clip Right Barcode Send Data Make Error."
			if (nPort == 3) alarm.mstr_pcode = _T("510100"); 
			if (nPort == 4) alarm.mstr_pcode = _T("511100"); 
			if (nPort == 5) alarm.mstr_pcode = _T("512100"); 
			if (nPort == 6) alarm.mstr_pcode = _T("513100"); 

			alarm.mn_count_mode = 0;	
			alarm.mn_type_mode = eWARNING; 
			st_work.mn_run_status = dWARNING;

			alarm.mn_alarm_assign_section = 181;

			BarcodeScanStep = 8000;
		}
		break;

		
	case 2300:
		// ==============================================================================================================
		// 바코드 스케너가 꺼졌는지 확인!
		// ==============================================================================================================
		if(st_serial.n_snd_chk[nPort] == TRUE) // 보냈다
		{
			l_until_wait_time[0] = GetCurrentTime();

			BarcodeScanStep = 2310;//3000;20121120
		}
		else if(st_serial.n_snd_chk[nPort] == COM_ERROR) // 통신상 무언가 오류가 발생했다..
		{
			//전송 요청했지만 완료가 안되어졌다.. 알람발생
			//510000 0 51 "HS Front Barcode Data Send Error."
			//511000 0 51 "HS Back Barcode Data Send Error."
			//512000 0 51 "Clip Left Barcode Data Send Error."
			//513000 0 51 "Clip Right Barcode Data Send Error."
			if (nPort == 3) alarm.mstr_pcode = _T("510000"); 
			if (nPort == 4) alarm.mstr_pcode = _T("511000"); 
			if (nPort == 5) alarm.mstr_pcode = _T("512000"); 
			if (nPort == 6) alarm.mstr_pcode = _T("513000"); 

			alarm.mn_count_mode = 0;	
			alarm.mn_type_mode = eWARNING; 
			st_work.mn_run_status = dWARNING;

			alarm.mn_alarm_assign_section = 182;

			BarcodeScanStep = 8000;
		}
		else
		{
			l_until_wait_time[1] = GetCurrentTime();
			l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];

			if(l_until_wait_time[2] > BCR_SND_WAIT)
			{
				//전송 요청했지만 완료가 안되어졌다.. 시간초과~  알람발생
				//510002 0 51 "HS Front Barcode Serial Port Timeout Error."
				//511002 0 51 "HS Back Barcode Serial Port Timeout Error."
				//512002 0 51 "Clip Left Barcode Serial Port Timeout Error."
				//513002 0 51 "Clip Right Barcode Serial Port Timeout Error."

				if (nPort == 3) alarm.mstr_pcode = _T("510002"); 
				if (nPort == 4) alarm.mstr_pcode = _T("511002"); 
				if (nPort == 5) alarm.mstr_pcode = _T("512002"); 
				if (nPort == 6) alarm.mstr_pcode = _T("513002"); 
				alarm.mn_count_mode = 0;	
				alarm.mn_type_mode = eWARNING; 
				st_work.mn_run_status = dWARNING;

				alarm.mn_alarm_assign_section = 183;

				BarcodeScanStep = 8000;
			}
		}
		break;
		/////////////////////////////////////// 20121120
	case 2310:
		if(st_client[NETWORK_CLIENT_7].n_client_send_flag == YES)
		{
			BarcodeScanStep = 3000;
		}
		break;
		///////////////////////////////////////

	case 3000:
		// ==============================================================================================================
		// 수신된 바코드 정보를 확인하자!!
		// ==============================================================================================================
		///////////////////////////////////// 20121120
		if(st_server[0].n_rev_bcr == YES)
		{
		/////////////////////////////////////
			if(st_serial.n_rec_chk[nPort] == TRUE) // 정보를 받았다
			{
				Ret = Func.OnBCR_Parsing(st_serial.str_rec[nPort], iBcrInfo, st_barcode.strBarcode);

				if(Ret == TRUE)
				//if(Ret == TRUE || st_basic.i_mode_interface == FALSE)
				{
					//strBcrReadData.Format("[Barcode]");
					for(i = 0 ; i < 1 ; i++)
					{
						//ybs 3.30 
						//if(st_basic.i_mode_interface == FALSE)
						//{
						//	if(iBcrInfo[i] == YES)
						//	{
						//		st_barcode.strBarcode[i].Format("TEMP_BARCODE_%02d", i + 1);
						//	}
						//	else
						//	{
						//		st_barcode.strBarcode[i] = _T("NO CODE");
						//	}
						//}

						//strBcrReadDataEach.Format(" ; %d:%s", i + 1, st_barcode.strBarcode[i]);
						strBcrReadDataEach.Format("%s",st_barcode.strBarcode[i]);
						strBcrReadData += strBcrReadDataEach;
					}
					if(st_handler.cwnd_list != NULL)
					{
						st_msg.mstr_recive = strBcrReadDataEach;
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, nPort, RECEIVE_MSG);
					}

					BarcodeScanStep = 10000; //정상  동작 또는 바코드를 하지 하지 않을때 
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg,"%s",st_serial.str_rec[nPort]);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}

					BarcodeScanStep = 3100;  //fly retry!!
				}
			}
			else if(st_serial.n_rec_chk[nPort] == FALSE || st_serial.n_rec_chk[nPort] == COM_ERROR) // 파싱결과 오류, 통신상 무언가 오류가 발생했다..
			{
				//전송 요청했지만 완료가 안되어졌다.. 알람발생
				//510001 0 51 "HS Front Barcode Data Recevied Error."
				//511001 0 51 "HS Back Barcode Data Recevied Error."
				//512001 0 51 "Clip Left Barcode Data Recevied Error."
				//513001 0 51 "Clip Right Barcode Data Recevied Error."

				if (nPort == 3) alarm.mstr_pcode = _T("510001"); 
				if (nPort == 4) alarm.mstr_pcode = _T("511001"); 
				if (nPort == 5) alarm.mstr_pcode = _T("512001"); 
				if (nPort == 6) alarm.mstr_pcode = _T("513001"); 

				alarm.mn_count_mode = 0;	
				alarm.mn_type_mode = eWARNING; 
				st_work.mn_run_status = dWARNING;

				alarm.mn_alarm_assign_section = 184;
				
				BarcodeScanStep = 8000;
			}
			else
			{
				l_until_wait_time[1] = GetCurrentTime();
				l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];

				if(l_until_wait_time[2] > 5000)//BCR_RCV_WAIT)
				{
					//전송 요청했지만 완료가 안되어졌다.. 알람발생 타임아웃
					//510002 0 51 "HS Front Barcode Serial Port Timeout Error."
					//511002 0 51 "HS Back Barcode Serial Port Timeout Error."
					//512002 0 51 "Clip Left Barcode Serial Port Timeout Error."
					//513002 0 51 "Clip Right Barcode Serial Port Timeout Error."

					if (nPort == 3) alarm.mstr_pcode = _T("510002"); 
					if (nPort == 4) alarm.mstr_pcode = _T("511002"); 
					if (nPort == 5) alarm.mstr_pcode = _T("512002"); 
					if (nPort == 6) alarm.mstr_pcode = _T("513002"); 
					alarm.mn_count_mode = 0;	
					alarm.mn_type_mode = eWARNING; 
					st_work.mn_run_status = dWARNING;

					alarm.mn_alarm_assign_section = 185;

					BarcodeScanStep = 8000;
				}
			}
		///////////////////////////////////// 20121120
		}
		else
		{
			l_until_wait_time[1] = GetCurrentTime();
			l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];

			if(l_until_wait_time[2] > 5000)//BCR_RCV_WAIT)
			{
				//전송 요청했지만 완료가 안되어졌다.. 알람발생 타임아웃
				//510002 0 51 "HS Front Barcode Serial Port Timeout Error."
				//511002 0 51 "HS Back Barcode Serial Port Timeout Error."
				//512002 0 51 "Clip Left Barcode Serial Port Timeout Error."
				//513002 0 51 "Clip Right Barcode Serial Port Timeout Error."

				if (nPort == 3) alarm.mstr_pcode = _T("510002"); 
				if (nPort == 4) alarm.mstr_pcode = _T("511002"); 
				if (nPort == 5) alarm.mstr_pcode = _T("512002"); 
				if (nPort == 6) alarm.mstr_pcode = _T("513002"); 
				alarm.mn_count_mode = 0;	
				alarm.mn_type_mode = eWARNING; 
				st_work.mn_run_status = dWARNING;

				alarm.mn_alarm_assign_section = 185;

				BarcodeScanStep = 8000;
			}
		}
		/////////////////////////////////////
		break;

	case 3100: //읽기 에러  발생 
		for(i = 0 ; i < PICKER_NUM ; i++)
		{
			st_barcode.strBarcode[i].Empty();
		}

		iBarcodeScanRetryCnt++;
		if(iBarcodeScanRetryCnt > 1)  //반복 횟수를 설정~!
		{
			iBarcodeScanRetryCnt = 0;
			BarcodeScanStep = 3200;	//step mode reading go!
		}
		else
		{
			BcrScanMoreSlow = TRUE;		
			//Carl 2010,0615
			BcrScanMoreSlowAgain = TRUE;//재시도 할때 좀더 느리게 해보자~!!

			BarcodeScanStep = 3000;  //fly retry!!
		}
		break;

	case 3200:
		//step mode로 설정~
		st_barcode.iScanningMethod = BCR_STEPBYSTEP;
		BcrStepReadIndex = 0;
		iBarcodeScanRetryCnt = 0;
		//Carl 2010,0615
		//BcrScanMoreSlow = FALSE;
		BcrScanMoreSlow = TRUE;
		BcrScanMoreSlowAgain = FALSE;

		BarcodeScanStep = 3300;
		break;

	case 3300:
		st_barcode.i_read_work_pos = BcrStepReadIndex;
		if(iBcrInfo[(PICKER_NUM - 1) - BcrStepReadIndex] == YES)  //8번 피커 자재의 바코드 부터 읽어야 하므로.. (자재를 가지고 있어야 읽도록 함)
		{
			BarcodeScanStep = 3400;
		}
		else
		{
			BarcodeScanStep = 5000;  //자재가 없다니 다음 피커 확인하러 가자
		}
		break;

	case 3400:
		// ==============================================================================================================
		// 바코드 스케닝을 시작하자!
		// ==============================================================================================================
		st_client[NETWORK_CLIENT_7].n_client_send_flag = NO;//20121120
		if(Func.OnBCR_Command(BCR_READON, &str_command))
		{
			st_serial.n_snd_chk[nPort] = COM_READY;
			st_serial.str_snd[nPort] = str_command;                  // 송신 데이터 설정
			l_until_wait_time[0] = GetCurrentTime();

			::PostMessage(st_handler.hWnd, WM_DATA_SEND, nPort, 0);  // 데이터 송신 요청

			BarcodeScanStep = 3500;
		}
		else
		{
			//전송할 데이터 생성 오류다~~ 알람발생~
			//510100 0 51 "HS Front Barcode Send Data Make Error."
			//511100 0 51 "HS Back Barcode Send Data Make Error."
			//512100 0 51 "Clip Left Barcode Send Data Make Error."
			//513100 0 51 "Clip Right Barcode Send Data Make Error."
			if (nPort == 3) alarm.mstr_pcode = _T("510100"); 
			if (nPort == 4) alarm.mstr_pcode = _T("511100"); 
			if (nPort == 5) alarm.mstr_pcode = _T("512100"); 
			if (nPort == 6) alarm.mstr_pcode = _T("513100"); 

			alarm.mn_count_mode = 0;	
			alarm.mn_type_mode = eWARNING; 
			st_work.mn_run_status = dWARNING;

			alarm.mn_alarm_assign_section = 217;

			BarcodeScanStep = 8000;
		}
		break;

	case 3500:
		// ==============================================================================================================
		// 바코드 스케너가 켜졌는지 확인!
		// ==============================================================================================================
		if(st_serial.n_snd_chk[nPort - 1] == TRUE) // 보냈다
		{
			BarcodeScanStep = 3510;//3600;20121120
		}
		else if(st_serial.n_snd_chk[nPort - 1] == COM_ERROR) // 통신상 무언가 오류가 발생했다..
		{
			//전송 요청했지만 완료가 안되어졌다.. 알람발생
			//510000 0 51 "HS Front Barcode Data Send Error."
			//511000 0 51 "HS Back Barcode Data Send Error."
			//512000 0 51 "Clip Left Barcode Data Send Error."
			//513000 0 51 "Clip Right Barcode Data Send Error."
			if (nPort == 3) alarm.mstr_pcode = _T("510000"); 
			if (nPort == 4) alarm.mstr_pcode = _T("511000"); 
			if (nPort == 5) alarm.mstr_pcode = _T("512000"); 
			if (nPort == 6) alarm.mstr_pcode = _T("513000"); 

			alarm.mn_count_mode = 0;	
			alarm.mn_type_mode = eWARNING; 
			st_work.mn_run_status = dWARNING;

			alarm.mn_alarm_assign_section = 218;

			BarcodeScanStep = 8000;
		}
		else
		{
			l_until_wait_time[1] = GetCurrentTime();
			l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];

			if(l_until_wait_time[2] > BCR_SND_WAIT)
			{
				//전송 요청했지만 완료가 안되어졌다.. 시간초과~  알람발생
				//510002 0 51 "HS Front Barcode Serial Port Timeout Error."
				//511002 0 51 "HS Back Barcode Serial Port Timeout Error."
				//512002 0 51 "Clip Left Barcode Serial Port Timeout Error."
				//513002 0 51 "Clip Right Barcode Serial Port Timeout Error."

				if (nPort == 3) alarm.mstr_pcode = _T("510002"); 
				if (nPort == 4) alarm.mstr_pcode = _T("511002"); 
				if (nPort == 5) alarm.mstr_pcode = _T("512002"); 
				if (nPort == 6) alarm.mstr_pcode = _T("513002"); 

				alarm.mn_count_mode = 0;	
				alarm.mn_type_mode = eWARNING; 
				st_work.mn_run_status = dWARNING;

				alarm.mn_alarm_assign_section = 219;

				BarcodeScanStep = 8000;
			}
		}
		break;

		////////20121120
	case 3510:
		if(st_client[NETWORK_CLIENT_7].n_client_send_flag == YES)
		{
			l_until_wait_time[0] = GetCurrentTime();
			BarcodeScanStep = 3600;
		}
		break;
		////////

	case 3600:
		// ==============================================================================================================
		// 바코드 스케닝을 종료하자!
		// ==============================================================================================================
		/////////////////////////////////////// 20121120
		st_server[0].n_rev_bcr = NO;
		st_client[NETWORK_CLIENT_7].n_client_send_flag = NO;
		///////////////////////////////////////
		
		if(Func.OnBCR_Command(BCR_READOFF, &str_command))
		{
			st_serial.n_snd_chk[nPort] = COM_READY;
			st_serial.str_snd[nPort] = str_command;                  // 송신 데이터 설정
			l_until_wait_time[0] = GetCurrentTime();

			st_serial.n_rec_chk[nPort] = COM_READY;

			::PostMessage(st_handler.hWnd, WM_DATA_SEND, nPort, 0);  // 데이터 송신 요청

			BarcodeScanStep = 3700;
		}
		else
		{
			//전송할 데이터 생성 오류다~~ 알람발생~
			//510100 0 51 "HS Front Barcode Send Data Make Error."
			//511100 0 51 "HS Back Barcode Send Data Make Error."
			//512100 0 51 "Clip Left Barcode Send Data Make Error."
			//513100 0 51 "Clip Right Barcode Send Data Make Error."
			if (nPort == 3) alarm.mstr_pcode = _T("510100"); 
			if (nPort == 4) alarm.mstr_pcode = _T("511100"); 
			if (nPort == 5) alarm.mstr_pcode = _T("512100"); 
			if (nPort == 6) alarm.mstr_pcode = _T("513100"); 

			alarm.mn_count_mode = 0;	
			alarm.mn_type_mode = eWARNING; 
			st_work.mn_run_status = dWARNING;

			alarm.mn_alarm_assign_section = 220;

			BarcodeScanStep = 8000;
		}
		break;

	case 3700:
		// ==============================================================================================================
		// 바코드 스케너가 꺼졌는지 확인!
		// ==============================================================================================================
		if(st_serial.n_snd_chk[nPort - 1] == TRUE) // 보냈다
		{
			l_until_wait_time[0] = GetCurrentTime();

			BarcodeScanStep = 3710;//3800;20121120
		}
		else if(st_serial.n_snd_chk[nPort - 1] == COM_ERROR) // 통신상 무언가 오류가 발생했다..
		{
			//전송 요청했지만 완료가 안되어졌다.. 알람발생
			//510000 0 51 "HS Front Barcode Data Send Error."
			//511000 0 51 "HS Back Barcode Data Send Error."
			//512000 0 51 "Clip Left Barcode Data Send Error."
			//513000 0 51 "Clip Right Barcode Data Send Error."
			if (nPort == 3) alarm.mstr_pcode = _T("510000"); 
			if (nPort == 4) alarm.mstr_pcode = _T("511000"); 
			if (nPort == 5) alarm.mstr_pcode = _T("512000"); 
			if (nPort == 6) alarm.mstr_pcode = _T("513000"); 

			alarm.mn_count_mode = 0;	
			alarm.mn_type_mode = eWARNING; 
			st_work.mn_run_status = dWARNING;

			alarm.mn_alarm_assign_section = 221;

			BarcodeScanStep = 8000;
		}
		else
		{
			l_until_wait_time[1] = GetCurrentTime();
			l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];

			if(l_until_wait_time[2] > BCR_SND_WAIT)
			{
				//전송 요청했지만 완료가 안되어졌다.. 시간초과~  알람발생
				//510002 0 51 "HS Front Barcode Serial Port Timeout Error."
				//511002 0 51 "HS Back Barcode Serial Port Timeout Error."
				//512002 0 51 "Clip Left Barcode Serial Port Timeout Error."
				//513002 0 51 "Clip Right Barcode Serial Port Timeout Error."

				if (nPort == 3) alarm.mstr_pcode = _T("510002"); 
				if (nPort == 4) alarm.mstr_pcode = _T("511002"); 
				if (nPort == 5) alarm.mstr_pcode = _T("512002"); 
				if (nPort == 6) alarm.mstr_pcode = _T("513002"); 

				alarm.mn_count_mode = 0;	
				alarm.mn_type_mode = eWARNING; 
				st_work.mn_run_status = dWARNING;

				alarm.mn_alarm_assign_section = 222;

				BarcodeScanStep = 8000;
			}
		}
		break;
		
		/////////////////////////////////////// 20121120
	case 3710:
		if(st_client[NETWORK_CLIENT_7].n_client_send_flag == YES)
		{
			BarcodeScanStep = 3800;
		}
		break;
		///////////////////////////////////////

	case 3800:
		// ==============================================================================================================
		// 수신된 바코드 정보를 확인하자!!
		// ==============================================================================================================
		///////////////////////////////////// 20121120
		if(st_server[0].n_rev_bcr == YES)
		{
		/////////////////////////////////////
			if(st_serial.n_rec_chk[nPort - 1] == TRUE) // 정보를 받았다
			{	
				Ret = Func.OnBCR_Parsing(st_serial.str_rec[nPort - 1], iBcrInfo, st_barcode.strBarcode);

				if(Ret == TRUE || st_basic.n_mode_device == 0)
				{
					strBcrReadData.Format("[Barcode]");
					//ybs 3.30 
					if(st_basic.n_mode_device == 0)
					{
						st_barcode.strBarcode[(PICKER_NUM - 1) - BcrStepReadIndex].Format("TEMP_BARCODE_%02d", (PICKER_NUM - BcrStepReadIndex));
					}

					strBcrReadDataEach.Format(" ; %d:%s", (PICKER_NUM - BcrStepReadIndex), st_barcode.strBarcode[(PICKER_NUM - 1) - BcrStepReadIndex]);
					strBcrReadData += strBcrReadDataEach;

					if(st_handler.cwnd_list != NULL)
					{
						st_msg.mstr_recive = strBcrReadData;
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, nPort, RECEIVE_MSG);
					}

					BarcodeScanStep = 4000;  //해당 자재는 성공이다 다음꺼 읽을게 있는지 확인하러!!
				}
				else
				{
					if(st_handler.cwnd_list != NULL)
					{
						sprintf(st_msg.c_abnormal_msg, "%s",st_serial.str_rec[nPort - 1]);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}

					st_barcode.strBarcode[(PICKER_NUM - 1) - BcrStepReadIndex] =  _T("NO CODE");

					BarcodeScanStep = 3900;	//실패했다 재시도할지 확인하자!
				}
			}
			else if(st_serial.n_rec_chk[nPort - 1] == FALSE || st_serial.n_rec_chk[nPort - 1] == COM_ERROR) // 파싱결과 오류, 통신상 무언가 오류가 발생했다..
			{
				//전송 요청했지만 완료가 안되어졌다.. 알람발생
				//510001 0 51 "HS Front Barcode Data Recevied Error."
				//511001 0 51 "HS Back Barcode Data Recevied Error."
				//512001 0 51 "Clip Left Barcode Data Recevied Error."
				//513001 0 51 "Clip Right Barcode Data Recevied Error."

				if (nPort == 3) alarm.mstr_pcode = _T("510001"); 
				if (nPort == 4) alarm.mstr_pcode = _T("511001"); 
				if (nPort == 5) alarm.mstr_pcode = _T("512001"); 
				if (nPort == 6) alarm.mstr_pcode = _T("513001"); 

				alarm.mn_count_mode = 0;	
				alarm.mn_type_mode = eWARNING; 
				st_work.mn_run_status = dWARNING;

				alarm.mn_alarm_assign_section = 223;
				
				if(BcrStepReadIndex < PICKER_NUM)	
				{
					st_barcode.strBarcode[(PICKER_NUM - 1) - BcrStepReadIndex] = _T("NO CODE");
					BarcodeScanStep = 4000;
				}
				else
				{
					BarcodeScanStep = 8000;
				}
			}
			else
			{
				l_until_wait_time[1] = GetCurrentTime();
				l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];

				if(l_until_wait_time[2] > BCR_RCV_WAIT)
				{
					//전송 요청했지만 완료가 안되어졌다.. 알람발생 타임아웃
					//510002 0 51 "HS Front Barcode Serial Port Timeout Error."
					//511002 0 51 "HS Back Barcode Serial Port Timeout Error."
					//512002 0 51 "Clip Left Barcode Serial Port Timeout Error."
					//513002 0 51 "Clip Right Barcode Serial Port Timeout Error."

					if (nPort == 3) alarm.mstr_pcode = _T("510002"); 
					if (nPort == 4) alarm.mstr_pcode = _T("511002"); 
					if (nPort == 5) alarm.mstr_pcode = _T("512002"); 
					if (nPort == 6) alarm.mstr_pcode = _T("513002"); 

					alarm.mn_count_mode = 0;	
					alarm.mn_type_mode = eWARNING; 
					st_work.mn_run_status = dWARNING;

					alarm.mn_alarm_assign_section = 224;

					BarcodeScanStep = 8000;
				}
			}
		///////////////////////////////////// 20121120
		}
		else
		{
			l_until_wait_time[1] = GetCurrentTime();
			l_until_wait_time[2] = l_until_wait_time[1] - l_until_wait_time[0];

			if(l_until_wait_time[2] > 5000)//BCR_RCV_WAIT)
			{
				//전송 요청했지만 완료가 안되어졌다.. 알람발생 타임아웃
				//510002 0 51 "HS Front Barcode Serial Port Timeout Error."
				//511002 0 51 "HS Back Barcode Serial Port Timeout Error."
				//512002 0 51 "Clip Left Barcode Serial Port Timeout Error."
				//513002 0 51 "Clip Right Barcode Serial Port Timeout Error."

				if (nPort == 3) alarm.mstr_pcode = _T("510002"); 
				if (nPort == 4) alarm.mstr_pcode = _T("511002"); 
				if (nPort == 5) alarm.mstr_pcode = _T("512002"); 
				if (nPort == 6) alarm.mstr_pcode = _T("513002"); 
				alarm.mn_count_mode = 0;	
				alarm.mn_type_mode = eWARNING; 
				st_work.mn_run_status = dWARNING;

				alarm.mn_alarm_assign_section = 185;

				BarcodeScanStep = 8000;
			}
		}
		/////////////////////////////////////
		break;

	case 3900:
		iBarcodeScanRetryCnt++;
		if(iBarcodeScanRetryCnt > 1)  //반복 횟수를 설정~!
		{
			iBarcodeScanRetryCnt = 0;
			BarcodeScanStep = 4000;		//재시도 했는데도 실패다 ㅠㅠ
		}
		else
		{
			BcrScanMoreSlow = TRUE; 
			BcrScanMoreSlowAgain = TRUE;//재시도 할때 좀더 느리게 해라~

			BarcodeScanStep = 3600;
		}
		break;

	case 4000:
		BcrScanMoreSlow = TRUE;
		BcrScanMoreSlowAgain = FALSE;

		BcrStepReadIndex++;
		if(BcrStepReadIndex > PICKER_NUM)
		{
			//8개 다 돌았다!!  (다 정상적으로 읽었을 수도 있고 아닐수도 있다 ^^)
			BarcodeScanStep = 5000;
		}
		else
		{
			iBarcodeScanRetryCnt = 0;  //다음 읽어 보아야 할 자재가 있다!!
			BarcodeScanStep = 3600;
		}
		break;

	case 5000:
		BarcodeScanStep = 10000;
		break;


	case 8000:
		// ==============================================================================================================
		// 수신된 바코드가 비정상이다!! 정보를 넘겨주자~!
		// ==============================================================================================================		
		st_barcode.iScanningOnFlag = FALSE;		//이제부턴 z축이 safety가 아니면 xy 동작 불가!!!!

		st_barcode.iScanningOnFlag = TRUE;
		FuncRet = RET_ERROR;
		BarcodeScanStep = 0;
		break;
		for(i = 0 ; i < PICKER_NUM ; i++)
		{
			st_barcode.strBarcode[i] = _T("NO CODE");
//			st_modulemap.strBcr_DeviceInRobotPicker[i] = st_barcode.strBarcode[i];			
		}

		BarcodeScanStep = 0;
		break;	

	case 10000:
		// ==============================================================================================================
		// 수신된 바코드가 정상이다!! 정보를 넘겨주자~!
		// ==============================================================================================================
		st_barcode.iScanningOnFlag = FALSE;		//이제부턴 z축이 safety가 아니면 xy 동작 불가!!!!

		for(i = 0 ; i < PICKER_NUM ; i++)
		{
//			st_modulemap.strBcr_DeviceInRobotPicker[i] = st_barcode.strBarcode[i];
			st_barcode.strBarcode[i] = _T("NO CODE");
		}

		BarcodeScanStep = 0;
		FuncRet = RET_GOOD;
		break;
	}

	return FuncRet;
}

void CPublic_Function::OnSet_Vision_send(int nVisionMethod)
{
	Func.Vision_Data(nVisionMethod, &st_work.str_vision_msg);
	st_serial.str_snd[VISION_PORT] = st_work.str_vision_msg;
	::PostMessage(st_handler.hWnd, WM_DATA_SEND, VISION_PORT, 0);  // 데이터 송신 요청
}

//////////////////////////////////////////// 20120608
int CPublic_Function::Vision_Data(int nMode, CString* strCommand)
{
	//yield Chr(2) + Command(3) + Data + Chr(3)
	//StripTotal,StripGood,StripRejected,UnitTotal,UnitGood,UnitRejected,LocFail,Missing
	//Extra,Pitch,Position,Large,Small,General
	//Command -> Yes
	//Data Length=> Data에 해당하는 전체길이
	//Model Change
	//Chr(2) + Command(2) + job번호(3) + Chr(3)
	//Command : CJ
	//Job번호 : 003
	switch(nMode)
	{
	case VISION_CJ:
		strCommand->Format("%cBCRCJ%03d%c",0x02,st_work.n_bcrNojob,0x03);
		break;
	case VISION_TKIN:
		strCommand->Format("%cBCRTKIN%c",0x02,0x03);
		break;
	case VISION_TKOUT:
		strCommand->Format("%cBCRTKOUT%c",0x02,0x03);
		break;

	case VISION_YIELD:
		strCommand->Format("%cBCRYIELD%c",0x02,0x03);
		break;
	case VISION_YIELD_YES:
		strCommand->Format("%cBCRYIELDYES%c",0x02,0x03);
		break;
	//////////////////////////////////////////////////// 20120618
	case VISION_ML:
		strCommand->Format("%cBCRML%c",0x02,0x03);
		break;
	case VISION_ML_YES:
		strCommand->Format("%cBCRMLYES%c",0x02,0x03);
		break;
	//////////////////////////////////////////////////// 
	case VISION_TRIGGER:
		strCommand->Format("%cBCRTRIGGER%c",0x02,0x03);
		break;

	case HEATSINK_CJ:
		strCommand->Format("%cHSCJ%03d%c",0x02,st_work.n_hsNojob,0x03);//20120831
		break;
	case HEATSINK_TKIN:
		strCommand->Format("%cHSTKIN%c",0x02,0x03);
		break;
	case HEATSINK_TKOUT:
		strCommand->Format("%cHSTKOUT%c",0x02,0x03);
		break;

	case HEATSINK_YIELD:
		strCommand->Format("%cHSYIELD%c",0x02,0x03);
		break;
	case HEATSINK_YIELD_YES:
		strCommand->Format("%cHSYIELDYES%c",0x02,0x03);
		break;
	//////////////////////////////////////////////////// 20120618
	case HEATSINK_ML:
		strCommand->Format("%cHSML%c",0x02,0x03);
		break;
	case HEATSINK_ML_YES:
		strCommand->Format("%cHSMLYES%c",0x02,0x03);
		break;
	//////////////////////////////////////////////////// 
	//////////////////////////////////////////////////// 
	case HEATSINK_TRIGGER:
		strCommand->Format("%cHSTRIGGER%c",0x02,0x03);
		break;

	////////////////////////////// 20120927
	case HEATSINK_TRIGGER_RESET:
		strCommand->Format("%cHSRESET%c",0x02,0x03);
		break;
	////////////////////////////// 
	}
	return TRUE;
}


double CPublic_Function::XYRobot_MovePosValCheck(int Site1, int Site2, int PosNum1, int PosNum2)  //MotorNum 사용하지 않음.. X축은 항상 고정?
{
	double MotorPos = 0;
	double TeachGap;
	
	switch(Site1)
	{
	case REJECT_SITE:
		TeachGap = (st_motor[M_RBT_UNLOAD_Y].md_pos[5] - st_motor[M_RBT_UNLOAD_Y].md_pos[4]) / (st_traybuffer[REJECT_SITE].i_loader_row - 1);
		MotorPos = st_motor[M_RBT_UNLOAD_Y].md_pos[4] + (TeachGap * PosNum1); //피커 번호가 클수록 값은 TeachGap 만큼 증가됨 
		break;

	case UNLOADER_SITE:
		TeachGap = (st_motor[M_RBT_UNLOAD_Y].md_pos[2] - st_motor[M_RBT_UNLOAD_Y].md_pos[1]) / (st_traybuffer[UNLOADER_SITE].i_loader_row - 1);
		MotorPos = st_motor[M_RBT_UNLOAD_Y].md_pos[1] + (TeachGap * PosNum1); //피커 번호가 클수록 값은 TeachGap 만큼 증가됨 
		break;

	case LDMODULE_SITE:
		TeachGap = (st_motor[M_M_RBT_Y].md_pos[Y_LD_TRAY_END] - st_motor[M_M_RBT_Y].md_pos[Y_LD_TRAY_START]) / (st_traybuffer[LDMODULE_SITE].i_loader_row - 1);
		MotorPos = st_motor[M_M_RBT_Y].md_pos[Y_LD_TRAY_START] + (TeachGap * PosNum2);// + (PosNum1 * st_basic.md_picker_gap_module) + (TeachGap * PosNum2); //피커 번호가 클수록 값은 TeachGap 만큼 증가됨 
		break;

	case LEFTSINK_SITE://PosNum1: 1,2 ?번째 피커
		TeachGap = (st_motor[M_HS_F_RBT_Y].md_pos[Y_LD_TRAY_END] - st_motor[M_HS_F_RBT_Y].md_pos[Y_LD_TRAY_START]) / (st_traybuffer[LEFTSINK_SITE].i_loader_row - 1);
		//MotorPos = st_motor[M_HS_F_RBT_Y].md_pos[Y_LD_TRAY_START] + (PosNum1 * st_basic.md_picker_gap_hs_left) + (TeachGap * PosNum2); //피커 번호가 클수록 값은 TeachGap 만큼 증가됨 
		MotorPos = st_motor[M_HS_F_RBT_Y].md_pos[Y_LD_TRAY_START] + (TeachGap * PosNum2); //피커 번호가 클수록 값은 TeachGap 만큼 증가됨 
		break;

	case RIGHTSINK_SITE:
		TeachGap = (st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_TRAY_END] - st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_TRAY_START]) / (st_traybuffer[RIGHTSINK_SITE].i_loader_row - 1);
		//MotorPos = st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_TRAY_START] + (PosNum1 * st_basic.md_picker_gap_hs_right) + (TeachGap * PosNum2); //피커 번호가 클수록 값은 TeachGap 만큼 증가됨 
		MotorPos = st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_TRAY_START] + (TeachGap * PosNum2); //피커 번호가 클수록 값은 TeachGap 만큼 증가됨 
		break;

	case WORK_BUFFER_SITE:
		if(Site2 == LDMODULE_SITE)
		{
			MotorPos = st_motor[M_M_RBT_Y].md_pos[Y_LD_WORK_PLACE1+PosNum1] - (PosNum2* st_basic.md_picker_gap_module);
		}
		else if(Site2 == LEFTSINK_SITE)
		{
			MotorPos = st_motor[M_HS_F_RBT_Y].md_pos[Y_LD_WORK_PLACE1+PosNum1] - (PosNum2 * st_basic.md_picker_gap_hs_left);
		}
		else if(Site2 == RIGHTSINK_SITE)
		{
			MotorPos = st_motor[M_HS_B_RBT_Y].md_pos[Y_LD_WORK_PLACE1+PosNum1] - (PosNum2*st_basic.md_picker_gap_hs_right);
		}
		else if(Site2 == LDCLIP_SITE)
		{
			MotorPos = st_motor[LDCLIP_SITE].md_pos[Y_LD_WORK_PLACE1+PosNum1];// - (PosNum2*st_work.n_PickerPitch[WORK_BUFFER_SITE]);
		}
		else if(Site2 == SORTER_SITE)
		{
			MotorPos = st_motor[SORTER_SITE].md_pos[Y_LD_WORK_PLACE1+PosNum1];// - (PosNum2*st_work.n_PickerPitch[WORK_BUFFER_SITE]);
		}
		break;

	case LDCLIP_SITE:
		if(PosNum1 == LD_CLIP_POS)
		{
			MotorPos = st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_POS];
		}
		else if(PosNum1 == ULD_CLIP_POS)
		{
			MotorPos = st_motor[M_CLIP_INSERT_Y].md_pos[ULD_CLIP_POS];
		}
		else
		{
			MotorPos = st_motor[M_CLIP_INSERT_Y].md_pos[LD_CLIP_SAFETY];
		}
		break;

	}

	return MotorPos;
}


int CPublic_Function::PickerDeviceStatusCheck(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM], int Result[PICKER_NUM])
{
	int i, FuncRet;
	int iPickerStats;
	char jamcode[10] = {0,};
	
	if(st_basic.n_mode_device == 0 || st_basic.n_mode_device == 2) //without device
	{
		for(i = 0 ; i < PICKER_NUM ; i++) 
		{
			Result[i] = YES;
		}
		return RET_GOOD;
	}
	
	FuncRet = RET_GOOD;

	if(st_handler.mn_back_mode == NO)
	{
		for(i = 0 ; i < PICKER_NUM ; i++)
		{
			Result[i] = NO;
			iPickerStats = FAS_IO.get_in_bit(st_io.i_hs_fwd_dvc_chk[i], IO_ON);

			if(OnOff == IO_ON)		// 디바이스를 집은후  
			{
				if(iPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)  //동작 시킨 피컨데 센싱이 된다면 자재가 없다는 의미!!
				{		
					if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

					Result[i] = NO; 
					FuncRet = RET_ERROR;
					
					//410500 1 41 "Forward Heat Sink Picker #1 Module On Check Error.[PS0400]."
					sprintf(alarm.mc_code, "41050%d",  i); //자재가 감지가 되야하는데 센싱이 안된다!
					alarm.mstr_code = _T(jamcode); 
					alarm.mn_count_mode = 0;
					alarm.mn_type_mode = dWARNING;
					//CTL_Lib.Alarm_Error_Occurrence(5042, CTL_dWARNING, alarm.mc_code);

					alarm.mn_alarm_assign_section = 59;
				}	
				else if(iPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)
				{
					Result[i] = YES; 
				}
			}
			else if(OnOff == IO_OFF) // 디바이스를 놓은 후 체크 
			{					
				if(iPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)	//나올리 없겠지.. (나오면 그리퍼가 동작이 안된거다)
				{
					if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

					Result[i] = NO;
					FuncRet = RET_ERROR;
					
					//410600 1 41 "Forward Heat Sink Picker #1 Module Off Check Error.[PS0400]."
					sprintf(alarm.mc_code, "41060%d",  i); //피커가 벌렸는데 센서가 ON 되었다..
					alarm.mstr_code = _T(alarm.mc_code); 
					alarm.mn_alarm_assign_section = 60;
					//CTL_Lib.Alarm_Error_Occurrence(5043, CTL_dWARNING, alarm.mc_code);
				}
				else if(iPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)
				{				 
					Result[i] = YES;
				}
			}
			else					// 피커의 상태를 파악한다 
			{		
				if(iPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)
				{	//디바이스가 있어야 하는데 없을때
					if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다. (door를 열었다 닫아야 한다!)

					Result[i] = NO;
					FuncRet = RET_ERROR;

					//410500 1 41 "Forward Heat Sink Picker #1 Module On Check Error.[PS0400]."
					sprintf(alarm.mc_code, "41050%d",  i); //on check error
					alarm.mstr_code = _T(alarm.mc_code); 

					alarm.mn_alarm_assign_section = 61;
					//CTL_Lib.Alarm_Error_Occurrence(5044, CTL_dWARNING, alarm.mc_code);
				}
			}
		}		
	}
	else
	{
		for(i = 0 ; i < PICKER_NUM ; i++)
		{
			Result[i] = NO;
			iPickerStats = FAS_IO.get_in_bit(st_io.i_hs_bwd_dvc_chk[i], IO_ON);

			if(OnOff == IO_ON)		// 디바이스를 집은후  
			{
				if(iPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)  //동작 시킨 피컨데 센싱이 된다면 자재가 없다는 의미!!
				{		
					if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

					Result[i] = NO; 
					FuncRet = RET_ERROR;
					
					//420500 1 42 "Backward Heat Sink Picker #1 Module On Check Error.[PS0400]."
					sprintf(alarm.mc_code, "42050%d",  i); //자재가 감지가 되야하는데 센싱이 안된다!
					alarm.mstr_code = _T(jamcode); 
					//CTL_Lib.Alarm_Error_Occurrence(5045, CTL_dWARNING, alarm.mc_code);

					alarm.mn_alarm_assign_section = 59;
				}	
				else if(iPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)
				{
					Result[i] = YES; 
				}
			}
			else if(OnOff == IO_OFF) // 디바이스를 놓은 후 체크 
			{					
				if(iPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)	//나올리 없겠지.. (나오면 그리퍼가 동작이 안된거다)
				{
					if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

					Result[i] = NO;
					FuncRet = RET_ERROR;
					
					//420600 1 42 "Backward Heat Sink Picker #1 Module Off Check Error.[PS0400]."
					sprintf(alarm.mc_code, "42060%d",  i); //피커가 벌렸는데 센서가 ON 되었다..
					alarm.mstr_code = _T(alarm.mc_code); 
					//CTL_Lib.Alarm_Error_Occurrence(5046, CTL_dWARNING, alarm.mc_code);
				}
				else if(iPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)
				{				 
					Result[i] = YES;
				}
			}
			else					// 피커의 상태를 파악한다 
			{		
				if(iPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)
				{	//디바이스가 있어야 하는데 없을때
					if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다. (door를 열었다 닫아야 한다!)

					Result[i] = NO;
					FuncRet = RET_ERROR;

					//420500 1 42 "Backward Heat Sink Picker #1 Module On Check Error.[PS0400]."
					sprintf(alarm.mc_code, "42050%d",  i); //on check error
					alarm.mstr_code = _T(alarm.mc_code); 

					alarm.mn_alarm_assign_section = 61;
					//CTL_Lib.Alarm_Error_Occurrence(5047, CTL_dWARNING, alarm.mc_code);
				}
			}
		}		
	}	
	return FuncRet;
}


int CPublic_Function::PickerModuleStatusCheck(int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM], int Result[PICKER_NUM])
{
	int i, FuncRet;
	int iPickerStats;
	char jamcode[10] = {0,};
	
	if(st_basic.n_mode_device == 0 || st_basic.n_mode_device == 2) //without device
	{
		for(i = 0 ; i < PICKER_NUM ; i++) 
		{
			Result[i] = YES;
		}
		return RET_GOOD;
	}
	
	FuncRet = RET_GOOD;
	
	for(i = 0 ; i < PICKER_NUM ; i++)
	{
		Result[i] = NO;
		iPickerStats = FAS_IO.get_in_bit(st_io.i_ld_module_dvc_chk[i], IO_ON);

		if(OnOff == IO_ON)		// 디바이스를 집은후  
		{
			if(iPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)  //동작 시킨 피컨데 센싱이 된다면 자재가 없다는 의미!!
			{		
				if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

				Result[i] = NO; 
				FuncRet = RET_ERROR;
				
				//400500 1 40 "Module Picker #1 Module On Check Error."
				sprintf(alarm.mc_code, "40050%d",  i); //자재가 감지가 되야하는데 센싱이 안된다!
				alarm.mstr_code = _T(jamcode); 
				alarm.mn_alarm_assign_section = 59;
			}	
			else if(iPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)
			{
				Result[i] = YES; 
			}
		}
		else if(OnOff == IO_OFF) // 디바이스를 놓은 후 체크 
		{					
			if(iPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)	//나올리 없겠지.. (나오면 그리퍼가 동작이 안된거다)
			{
				if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

				Result[i] = NO;
				FuncRet = RET_ERROR;
				
				//400600 1 40 "Module Picker #1 Module Off Check Error."
				sprintf(alarm.mc_code, "40060%d",  i); //피커가 벌렸는데 센서가 ON 되었다..
				alarm.mstr_code = _T(jamcode); 
			}
			else if(iPickerStats == IO_OFF && PickerInfo[D_EXIST][i] == YES)
			{				 
				Result[i] = YES;
			}
		}
		else					// 피커의 상태를 파악한다 
		{		
			if(iPickerStats == IO_ON && PickerInfo[D_EXIST][i] == YES)
			{	//디바이스가 있어야 하는데 없을때
				if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다. (door를 열었다 닫아야 한다!)

				Result[i] = NO;
				FuncRet = RET_ERROR;

				//400500 1 40 "Module Picker #1 Module On Check Error."
				sprintf(jamcode, "40050%d",  i); //on check error
				alarm.mstr_code = _T(jamcode); 
			}
		}
	}
	
	return FuncRet;
}

int CPublic_Function::PickerSorterStatusCheck(int OnOff, int PickerInfo[D_INFOSIZE], int Result[LDULD_PARA])
{
	int FuncRet;
	int iPickerStats;
	char jamcode[10] = {0,};
	
	if(st_basic.n_mode_device != 1) //without device
	{
		Result[0] = YES;
		return RET_GOOD;
	}
	
	FuncRet = RET_GOOD;
	
	Result[0] = NO;
	iPickerStats = FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dvc_chk, IO_ON);

	if(OnOff == IO_ON)		// 디바이스를 집은후  
	{
		if(iPickerStats == IO_ON && PickerInfo[D_EXIST] == YES)  //동작 시킨 피컨데 센싱이 된다면 자재가 없다는 의미!!
		{		
			if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

			Result[0] = NO; 
			FuncRet = RET_ERROR;
			
//			550504 1 55 "Sorter Picker에 모듈이 감지 되지 않습니다."
//			550505 1 55 "Sorter Picker에 모듈이 감지 됩니다. 제거해 주세요."
			alarm.mstr_code = _T("550504"); 
			sprintf(alarm.mc_code, "550504");
			alarm.mn_count_mode = 0;
			alarm.mn_type_mode = dWARNING;

			alarm.mn_alarm_assign_section = 259;
		}	
		else if(iPickerStats == IO_OFF && PickerInfo[D_EXIST] == YES)
		{
			Result[0] = YES; 
		}
	}
	else if(OnOff == IO_OFF) // 디바이스를 놓은 후 체크 
	{					
		if(iPickerStats == IO_ON && PickerInfo[D_EXIST] == YES)
		{
			if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

			Result[0] = NO;
			FuncRet = RET_ERROR;
			
			alarm.mstr_code = _T("550505"); 
			sprintf(alarm.mc_code, "550505");
			alarm.mn_count_mode = 0;
			alarm.mn_type_mode = dWARNING;
			alarm.mn_alarm_assign_section = 260;
		}
		else if(iPickerStats == IO_OFF && PickerInfo[D_EXIST] == YES)
		{				 
			Result[0] = YES;
		}
	}
	else					// 피커의 상태를 파악한다 
	{		
		if(iPickerStats == IO_ON && PickerInfo[D_EXIST] == YES)
		{	//디바이스가 있어야 하는데 없을때
			if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다. (door를 열었다 닫아야 한다!)

			Result[0] = NO;
			FuncRet = RET_ERROR;

			alarm.mstr_code = _T("550504"); 
			sprintf(alarm.mc_code, "550504");
			alarm.mn_count_mode = 0;
			alarm.mn_type_mode = dWARNING;

			alarm.mn_alarm_assign_section = 261;
		}
	}
	
	return FuncRet;
}

int CPublic_Function::PickerUnloadStatusCheck(int OnOff, int PickerInfo[D_INFOSIZE], int Result[LDULD_PARA])
{
	int FuncRet;
	int iPickerStats;
	char jamcode[10] = {0,};
	
	if(st_basic.n_mode_device == 0 || st_basic.n_mode_device == 2) //without device
	{
		Result[0] = YES;
		return RET_GOOD;
	}
	
	FuncRet = RET_GOOD;
	
	Result[0] = NO;
	iPickerStats = FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dvc_chk, IO_ON);

	if(OnOff == IO_ON)		// 디바이스를 집은후  
	{
		if(iPickerStats == IO_ON && PickerInfo[D_EXIST] == YES)  //동작 시킨 피컨데 센싱이 된다면 자재가 없다는 의미!!
		{		
			if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

			Result[0] = NO; 
			FuncRet = RET_ERROR;
			
//			550604 1 55 "Sorter Picker에 모듈이 감지 되지 않습니다."
//			550605 1 55 "Unload Picker에 모듈이 감지 됩니다. 제거해 주세요."
			alarm.mstr_code = _T("550604"); 
			sprintf(alarm.mc_code, "550604");
			alarm.mn_count_mode = 0;
			alarm.mn_type_mode = dWARNING;

			alarm.mn_alarm_assign_section = 259;
		}	
		else if(iPickerStats == IO_OFF && PickerInfo[D_EXIST] == YES)
		{
			Result[0] = YES; 
		}
	}
	else if(OnOff == IO_OFF) // 디바이스를 놓은 후 체크 
	{					
		if(iPickerStats == IO_ON && PickerInfo[D_EXIST] == YES)
		{
			if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다.

			Result[0] = NO;
			FuncRet = RET_ERROR;
			
			alarm.mstr_code = _T("550605"); 
			sprintf(alarm.mc_code, "550605");
			alarm.mn_count_mode = 0;
			alarm.mn_type_mode = dWARNING;
			alarm.mn_alarm_assign_section = 260;
		}
		else if(iPickerStats == IO_OFF && PickerInfo[D_EXIST] == YES)
		{				 
			Result[0] = YES;
		}
	}
	else					// 피커의 상태를 파악한다 
	{		
		if(iPickerStats == IO_ON && PickerInfo[D_EXIST] == YES)
		{	//디바이스가 있어야 하는데 없을때
			if(OnOff == PICKERSTATUS) st_alarm.n_picking_error = YES;					//Picker Alarm이다. (door를 열었다 닫아야 한다!)

			Result[0] = NO;
			FuncRet = RET_ERROR;

			alarm.mstr_code = _T("550604"); 
			alarm.mn_count_mode = 0;
			alarm.mn_type_mode = dWARNING;

			alarm.mn_alarm_assign_section = 261;
		}
	}
	
	return FuncRet;
}

int CPublic_Function::Find_TrayInfo_Count(int n_site, int n_dvcyesno, int n_tray_y_pos)
{
	int x=0, y=0, nFlag=CTL_NO, nCnt = 0;

	if(n_site == LDMODULE_SITE)
	{
		if(n_tray_y_pos >= 0 && n_tray_y_pos <= st_traybuffer[LDMODULE_SITE].i_loader_col - 1) //지정한 n_tray_y_pos 만 체크 
		{
			for(x=0 ; x<st_traybuffer[LDMODULE_SITE].i_loader_row; x++)
			{
				if(st_modulemap.LoadTray[1][x] == n_dvcyesno && st_modulemap.LoadTrayState[x] == TRAYROW_ON) //디바이스가 있으면 
				{
					nCnt++;					 
				}
			}
		}
		else //if(n_tray_y_pos == -1) //트레이 전체 정보를 구한다 
		{
			for(x=0 ; x<st_traybuffer[LDMODULE_SITE].i_loader_row; x++)
			{
				if(st_modulemap.LoadTray[1][x] == n_dvcyesno)// && st_modulemap.LoadTrayState[x] == TRAYROW_ON) //디바이스가 있으면 
				{
					nCnt++;
				}
			}

		}		
		
	}
	else if(n_site == LEFTSINK_SITE)
	{
		if(n_tray_y_pos >= 0 && n_tray_y_pos <= st_traybuffer[LEFTSINK_SITE].i_loader_col - 1) //지정한 n_tray_y_pos 만 체크 
		{
			for(x=0 ; x<st_traybuffer[LEFTSINK_SITE].i_loader_row; x++)
			{
				//if(st_modulemap.LefthsTray[1][n_tray_y_pos][x] == n_dvcyesno && st_modulemap.LefthsTrayState[n_tray_y_pos][x] == TRAYROW_ON) //디바이스가 있으면 
				if(st_modulemap.LefthsTray[1][x] == n_dvcyesno)// && st_modulemap.LefthsTrayState[n_tray_y_pos][x] == TRAYROW_ON) //디바이스가 있으면 
				{
					nCnt++;					 
				}
			}
		}
		else //if(n_tray_y_pos == -1) //트레이 전체 정보를 구한다 
		{
			//for(y=0 ; y<st_traybuffer[LDMODULE_SITE].i_loader_col; y++)
			//{
				for(x=0 ; x<st_traybuffer[LEFTSINK_SITE].i_loader_row; x++)
				{
					if(st_modulemap.LoadTray[1][x] == n_dvcyesno)// && st_modulemap.LefthsTrayState[x] == TRAYROW_ON) //디바이스가 있으면 
					{
						nCnt++;						 
					}
				}
			//}
		}		
	}
	else if(n_site == RIGHTSINK_SITE)
	{
		if(n_tray_y_pos >= 0 && n_tray_y_pos <= st_traybuffer[RIGHTSINK_SITE].i_loader_col - 1) //지정한 n_tray_y_pos 만 체크 
		{
			for(x=0 ; x<st_traybuffer[RIGHTSINK_SITE].i_loader_row; x++)
			{
				//if(st_modulemap.RighthsTray[1][n_tray_y_pos][x] == n_dvcyesno && st_modulemap.RighthsTrayState[n_tray_y_pos][x] == TRAYROW_ON) //디바이스가 있으면 
				if(st_modulemap.RighthsTray[1][x] == n_dvcyesno)// && st_modulemap.RighthsTrayState[n_tray_y_pos][x] == TRAYROW_ON) //디바이스가 있으면 
				{
					nCnt++;					 
				}
			}
		}
		else //if(n_tray_y_pos == -1) //트레이 전체 정보를 구한다 
		{
			//for(y=0 ; y<st_traybuffer[RIGHTSINK_SITE].i_loader_col; y++)
			//{
				for(x=0 ; x<st_traybuffer[RIGHTSINK_SITE].i_loader_row; x++)
				{
					//if(st_modulemap.RighthsTray[1][y][x] == n_dvcyesno && st_modulemap.RighthsTrayState[y][x] == TRAYROW_ON) //디바이스가 있으면 
					if(st_modulemap.RighthsTray[1][x] == n_dvcyesno)// && st_modulemap.RighthsTrayState[y][x] == TRAYROW_ON) //디바이스가 있으면 
					{
						nCnt++;						 
					}
				}
			//}
		}		
	}
	else if(n_site == WORK_BUFFER_SITE)
	{
		for (x = 0; x <4 ; x++)
		{
			if(st_modulemap.WorkBuffer[D_EXIST][x][LDMODULE_SITE] == n_dvcyesno)
			{
				nCnt++;
			}
		}
	}	
	else if(n_site == VISION_SITE)
	{
		if(st_modulemap.VisionBuffer[D_EXIST][TOPBUFFER] == n_dvcyesno || n_dvcyesno == -1)
		{
			nCnt++;
		}
		if(st_modulemap.VisionBuffer[D_EXIST][BTMBUFFER] == n_dvcyesno || n_dvcyesno == -1)
		{
			nCnt++;
		}
	}
	return nCnt;
}


int CPublic_Function::Find_Picker_Count(int nSite, int n_enable, int n_dvcyesno, int n_bin)
{
	int x=0, y=0, i=0, nCnt = 0;

	if(nSite == LDMODULE_SITE)
	{
		for(i = 0 ; i < PICKER_NUM ; i++)
		{
			if((st_modulemap.nModuleRobotPicker[D_EXIST][i] == n_dvcyesno || n_dvcyesno == -1)
				&& (st_modulemap.nModuleRobotPicker[D_BIN][i] == n_bin || n_bin == BIN_NOTUESD))
			{
				if(n_enable == CTL_YES) //picker enable check count
				{
					if(st_picker.n_module_ldrbt_enable[i] == YES)
					{
						nCnt++;
					}
				}
				else
				{
					nCnt++;
				}
			}
		}
	}
	else if(nSite == LEFTSINK_SITE)
	{
		if(st_picker.n_heat_leftldrbt_enable[0] == TRUE)
		{
			if((st_modulemap.nLefthsRobotPicker[D_EXIST][0] == n_dvcyesno || n_dvcyesno == -1)
				&& (st_modulemap.nLefthsRobotPicker[D_BIN][0] == n_bin || n_bin == BIN_NOTUESD))
			{
				if(n_enable == CTL_YES) //picker enable check count
				{
					if(st_picker.n_heat_leftldrbt_enable[0] == YES)
					{
						nCnt++;
					}
				}
				else
				{
					nCnt++;
				}
			}
		}

		else if(st_picker.n_heat_leftldrbt_enable[1] == TRUE)
		{
			if((st_modulemap.nLefthsRobotPicker[D_EXIST][1] == n_dvcyesno || n_dvcyesno == -1)
				&& (st_modulemap.nLefthsRobotPicker[D_BIN][1] == n_bin || n_bin == BIN_NOTUESD))
			{
				if(n_enable == CTL_YES) //picker enable check count
				{
					if(st_picker.n_heat_leftldrbt_enable[1] == YES)
					{
						nCnt++;
					}
				}
				else
				{
					nCnt++;
				}
			}
		}
		else
		{
			for(i = 0 ; i < PICKER_NUM ; i++) //BUF1_SIZE 과 BUF2_SIZE는 같다고 본다 , 같아야 한다 
			{
				if((st_modulemap.nLefthsRobotPicker[D_EXIST][i] == n_dvcyesno || n_dvcyesno == -1)
					&& (st_modulemap.nLefthsRobotPicker[D_BIN][i] == n_bin || n_bin == BIN_NOTUESD))
				{
					if(n_enable == CTL_YES) //picker enable check count
					{
						if(st_picker.n_heat_leftldrbt_enable[i] == YES)
						{
							nCnt++;
						}
					}
					else
					{
						nCnt++;
					}
				}
			}
		}
	}
	else if(nSite == RIGHTSINK_SITE)
	{
		if(st_picker.n_heat_rightldrbt_enable[0] == TRUE)
		{
			if((st_modulemap.nRighthsRobotPicker[D_EXIST][0] == n_dvcyesno || n_dvcyesno == -1)
				&& (st_modulemap.nRighthsRobotPicker[D_BIN][0] == n_bin || n_bin == BIN_NOTUESD))
			{
				if(n_enable == CTL_YES) //picker enable check count
				{
					if(st_picker.n_heat_rightldrbt_enable[0] == YES)
					{
						nCnt++;
					}
				}
				else
				{
					nCnt++;
				}
			}
		}

		else if(st_picker.n_heat_rightldrbt_enable[1] == TRUE)
		{
			if((st_modulemap.nRighthsRobotPicker[D_EXIST][1] == n_dvcyesno || n_dvcyesno == -1)
				&& (st_modulemap.nRighthsRobotPicker[D_BIN][1] == n_bin || n_bin == BIN_NOTUESD))
			{
				if(n_enable == CTL_YES) //picker enable check count
				{
					if(st_picker.n_heat_rightldrbt_enable[1] == YES)
					{
						nCnt++;
					}
				}
				else
				{
					nCnt++;
				}
			}
		}

		else
		{
			for(i = 0 ; i < PICKER_NUM ; i++) //BUF1_SIZE 과 BUF2_SIZE는 같다고 본다 , 같아야 한다 
			{
				if((st_modulemap.nRighthsRobotPicker[D_EXIST][i] == n_dvcyesno || n_dvcyesno == -1)
					&& (st_modulemap.nRighthsRobotPicker[D_BIN][i] == n_bin || n_bin == BIN_NOTUESD))
				{
					if(n_enable == CTL_YES) //picker enable check count
					{
						if(st_picker.n_heat_rightldrbt_enable[i] == YES)
						{
							nCnt++;
						}
					}
					else
					{
						nCnt++;
					}
				}
			}
		}
	}
	else if(nSite == SORTER_SITE)
	{
		if((st_modulemap.nSortRobotPicker[D_EXIST] == n_dvcyesno || n_dvcyesno == -1)
			&& (st_modulemap.nSortRobotPicker[D_BIN] == n_bin || n_bin == BIN_NOTUESD))
		{
			nCnt++;
		}
	}
	else if(nSite == UNLOADER_SITE)
	{
		if((st_modulemap.nUldRobotPicker[D_EXIST] == n_dvcyesno || n_dvcyesno == -1)
			&& (st_modulemap.nUldRobotPicker[D_BIN] == n_bin || n_bin == BIN_NOTUESD))
		{
			nCnt++;
		}
	}
	return nCnt;	
}


void CPublic_Function::OnReject_Tray_Reset()
{
	int j;

	for(j = 0 ; j < st_traybuffer[REJECT_SITE].i_loader_row ; j++)
	{
		st_modulemap.RejectTray[j] = DVC_NO;			// Reset 
		st_modulemap.RejectTray[j] = DVC_NO;			// Reset 
	}

	if(st_handler.cwnd_main != NULL)
	{
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_REJECTINFO, 0);  //언로더 정보 
	}
}

//
//int CPublic_Function::OnNetworkDataAnalysis(CString strBody)
//{
//	int nFuncRet = RET_GOOD;
////	CString strHeader, strBody;
//	int i;
//	int n_check_num = 0, n_check_string = 0, n_check_extra = 0;
//	CString strFunction, strTemp;
//	int nID = -1, nPcBoxCnt, nPcBoxNo;
//	int nResult1, nResult2;
//	CString strTmp[10];
//			
//// 	if (st_NW.mstr_full_msg[1].GetLength() < 1)								// 넘어온 데이타가 0일때...
//// 	{
//// 		return RET_ERROR;
//// 	}
//// 
//// 	if (n_head_check == CTL_NO)
//// 	{
//// 		strHeader = st_NW.mstr_full_msg[1].Mid(0, 10);				// 앞에서 부터 10자리가 헤더이고, 나머지 자리수이다.
//// 
//// 		if (strHeader != "")										// 헤더가 제대로 있을 때, 2K10/01/18/ViboX
//// 		{
//// 			strcpy(chr_data, strHeader);
//// 			
//// 			for (i = 0; i < 10; i++)
//// 			{
//// 				if (chr_data[i] >= 48 && chr_data[i] <= 57)			// 숫자 확인
//// 				{
//// 					n_check_num++;
//// 				}
//// 				else if (chr_data[i] >= 65 && chr_data[i] <= 90)	// 문자 확인
//// 				{
//// 					n_check_string++;
//// 				}
//// 				else if (chr_data[i] == 32)
//// 				{
//// 					n_check_num++;
//// 				}
//// 				else
//// 				{
//// 					n_check_extra++;
//// 				}
//// 			}
//// 			
//// 			if (n_check_num == 10)									// 전부 숫자라면. 2K10/01/18/ViboX								
//// 			{
//// 				nHeader = atoi(strHeader);
//// 			}
//// 			else
//// 			{
//// 				return RET_ERROR;
//// 			}
//// 		}
//// 
//// 		strBody = st_NW.mstr_full_msg[1].Mid(10);					// Body를 취한다.
//// 		nBody = strBody.GetLength();								// Body의 길이를 얻는다.
//// 		
//// 		if (nBody == 0)
//// 		{
//// 			st_NW.mn_recived = NO;
//// 			n_head_check = CTL_YES;
//// 			return RET_PROCEED;
//// 		}
//// 	}
//// 	else
//// 	{
//// 		if (st_NW.mn_recived == COMPLETE)
//// 		{
//// 			strBody = st_NW.mstr_full_msg[1];					// Body를 취한다.
//// 			nBody = strBody.GetLength();						// Body의 길이를 얻는다.
//// 			
//// 			if (nBody == 0)
//// 			{
//// 				n_head_check = CTL_YES;
//// 				return RET_PROCEED;
//// 			}
//// 
//// 			n_head_check = CTL_NO;
//// 		}
//// 		else
//// 		{
//// 			return RET_PROCEED;
//// 		}
//// 	}
//// 
//// 	if (nHeader != nBody)										// 헤더에서 알려준 길이와 실제 길이가 다를 경우에...
//// 	{
//// 		return RET_ERROR;					
//// 	}
//
//	strFunction = OnNetworkBodyAnalysis(strBody, "FUNCTION");
//	
//	if (strFunction == "LOT_START")							st_NW.mnID = NW_LOT_START;
//	else if (strFunction == "LOT_CANCEL")					st_NW.mnID = NW_LOT_CANCEL;
//	else if (strFunction == "PCBOX_START")					st_NW.mnID = NW_PCBOX_START;
//	else if (strFunction == "PCBOX_END")					st_NW.mnID = NW_PCBOX_END;
//	else if (strFunction == "PCBOX_CHANGE")					st_NW.mnID = NW_PCBOX_CHANGE;
//	else if (strFunction == "ENABLE")						st_NW.mnID = NW_ENABLE;
//	else if (strFunction == "DISABLE")						st_NW.mnID = NW_DISABLE;
//	else if (strFunction == "SLOT_MASK")					st_NW.mnID = NW_SLOT_MASK;
//	else if (strFunction == "LOT_START_ENABLE")				st_NW.mnID = NW_LOT_START_ENABLE;
//	else if (strFunction == "PCBOX_REMOVE")					st_NW.mnID = NW_PCBOX_REMOVE;
//	else if (strFunction == "PCBOX_ERROR")					st_NW.mnID = NW_PCBOX_ERROR;
//	else if (strFunction == "PCBOX_LOT_INFO_RPY")			st_NW.mnID = NW_PCBOX_LOT_INFO_RPY;
//	else if (strFunction == "FAIL_INFO_SEND_RPY")			st_NW.mnID = NW_FAIL_INFO_SEND_RPY;
//	else if (strFunction == "FAIL_REGNUMBER_REQUEST_RPY")	st_NW.mnID = NW_FAIL_REGNUMBER_REQUEST_RPY;
//	else if (strFunction == "FAIL_INFO_SERACH_RPY")			st_NW.mnID = NW_FAIL_INFO_SERACH_RPY;
//	else if (strFunction == "FAIL_INFO_SAVE_RPY")			st_NW.mnID = NW_FAIL_INFO_SAVE_RPY;
//	else if (strFunction == "FAIL_INFO_DELETE_RPY")			st_NW.mnID = NW_FAIL_INFO_DELETE_RPY;
//	else if (strFunction == "LOT_DISPLAY_RPY")				st_NW.mnID = NW_LOT_DISPLAY_RPY;
//	else if (strFunction == "EQP_LOT_START_RPY")			st_NW.mnID = NW_EQP_LOT_START_RPY;
//	else if (strFunction == "EQP_LOT_CANCEL_RPY")			st_NW.mnID = NW_EQP_LOT_CANCEL_RPY;
//	else if (strFunction == "EQP_LOT_END_RPY")				st_NW.mnID = NW_EQP_LOT_END_RPY;
//
//	switch (st_NW.mnID)
//	{
//// 여기서부터는 장비가 질문/통보를 받는 부분이다.
//	case NW_LOT_START:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_oper_id[1] = OnNetworkBodyAnalysis(strBody, "OPER_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_part_id[1] = OnNetworkBodyAnalysis(strBody, "PART_ID");
//		st_NW.mstr_pcbox_cnt[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_CNT");
//		nPcBoxCnt = atoi(st_NW.mstr_pcbox_cnt[1]);
//		st_NW.mstr_lot_qty[1] = OnNetworkBodyAnalysis(strBody, "LOT_QTY");
//		st_NW.mstr_pcbox_info[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_INFO", 1);
//
//		// 데이터가 정상인지 검사.
//		if (st_NW.mstr_lot_id[1] != "" && st_NW.mstr_eqp_id[1] != "" && st_NW.mstr_oper_id[1] != "" && st_NW.mstr_step_cnt[1] != "" &&
//			st_NW.mstr_part_id[1] != "" && nPcBoxCnt > 0 && st_NW.mstr_lot_qty[1] != "" && st_NW.mstr_pcbox_info[1] != "")
//		{
//			st_NW.mstr_result[1] = "PASS";
//		}
//		else
//		{
//			st_NW.mstr_result[1] = "FAIL";
//			return RET_ERROR;
//		}
//
//		for (i = 0; i < nPcBoxCnt; i++)
//		{
//			strTemp.Format("PCBOX_%03d", i + 1);
//			st_NW.mstr_pcbox[1][i] = OnNetworkBodyAnalysis(st_NW.mstr_pcbox_info[1], strTemp);
//			strTemp.Format("SLOT_%03d", i + 1);
//			st_NW.mstr_pcboxslot[1][i] = OnNetworkBodyAnalysis(st_NW.mstr_pcbox_info[1], strTemp);
//		}
//
//		st_work.str_lot_no = st_NW.mstr_lot_id[1];				// Lot Start 할때 받은 Lot No를 넣어둔다.
//		st_work.str_part_no = st_NW.mstr_part_id[1];			// Lot Start 할때 받은 Lot No를 넣어둔다.
//		st_work.str_op_id = st_NW.mstr_oper_id[1];				// Lot Start 할때 받은 Oper ID를 넣어둔다.
//
//		if (st_handler.cwnd_main != NULL)
//		{
//			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
//		}
//		break;
//
//	case NW_LOT_CANCEL:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_oper_id[1] = OnNetworkBodyAnalysis(strBody, "OPER_ID");
//		st_NW.mstr_pcbox_cnt[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_CNT");
//		nPcBoxCnt = atoi(st_NW.mstr_pcbox_cnt[1]);
//		st_NW.mstr_pcbox_info[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_INFO", 1);
//		
//		for (i = 0; i < nPcBoxCnt; i++)
//		{
//			strTemp.Format("PCBOX_%03d", i + 1);
//			st_NW.mstr_pcbox[1][i] = OnNetworkBodyAnalysis(st_NW.mstr_pcbox_info[1], strTemp);
//			strTemp.Format("SLOT_%03d", i + 1);
//			st_NW.mstr_pcboxslot[1][i] = OnNetworkBodyAnalysis(st_NW.mstr_pcbox_info[1], strTemp);
//		}
//		break;
//
//	case NW_PCBOX_START:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_pcbox_id[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_ID");
//		
//		nPcBoxNo = OnPCBOXIDToPCBOXNumber(st_NW.mstr_pcbox_id[1]);				// ID를 Box 번호로 바꾼다. 
//
//		st_work.mn_PCBOX_Status[nPcBoxNo] = WORK_START;							// 해당 Box가 Start 되었다고 세팅한다.
//		st_work.mstr_PCBOX_Result[nPcBoxNo] = "";								// 해당 BOX의 결과를 RESET한다.
//
//		if (nPcBoxNo < 60)			// Lower
//		{
////			st_board[LOWER_PART][nPcBoxNo].nboard_status = BOARD_TEST_START;
////			st_board[LOWER_PART][nPcBoxNo].nresult[0] = BIN_CLEAR;
////			st_board[LOWER_PART][nPcBoxNo].nresult[1] = BIN_CLEAR;
////			st_board[LOWER_PART][nPcBoxNo].strlot_no = st_NW.mstr_lot_id[1];
//
//			st_board[LOWER_PART][nPcBoxNo + 10].nboard_status = BOARD_TEST_START;
//			st_board[LOWER_PART][nPcBoxNo + 10].nresult[0]	= BIN_CLEAR;
//			st_board[LOWER_PART][nPcBoxNo + 10].nresult[1]	= BIN_CLEAR;
//			st_board[LOWER_PART][nPcBoxNo + 10].strlot_no		= st_NW.mstr_lot_id[1];
//			
//			// 20100408 tae추가
//			st_board[LOWER_PART][nPcBoxNo + 10].strpcbox_id		= st_NW.mstr_pcbox_id[1];
//		}
//		else						// Upper
//		{
////			st_board[UPPER_PART][nPcBoxNo - 60].nboard_status = BOARD_TEST_START;
////			st_board[UPPER_PART][nPcBoxNo - 60].nresult[0] = BIN_CLEAR;
////			st_board[UPPER_PART][nPcBoxNo - 60].nresult[1] = BIN_CLEAR;
////			st_board[UPPER_PART][nPcBoxNo - 60].strlot_no = st_NW.mstr_lot_id[1];
//
//			st_board[UPPER_PART][nPcBoxNo - 50].nboard_status = BOARD_TEST_START;
//			st_board[UPPER_PART][nPcBoxNo - 50].nresult[0] = BIN_CLEAR;
//			st_board[UPPER_PART][nPcBoxNo - 50].nresult[1] = BIN_CLEAR;
//			st_board[UPPER_PART][nPcBoxNo - 50].strlot_no = st_NW.mstr_lot_id[1];
//
//			// 20100408 tae추가
//			st_board[UPPER_PART][nPcBoxNo - 50].strpcbox_id		= st_NW.mstr_pcbox_id[1];
//		}
//		break;
//
//	case NW_PCBOX_END:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_pcbox_id[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_ID");
//		st_NW.mstr_test_result[1] = OnNetworkBodyAnalysis(strBody, "TEST_RESULT");
//		st_NW.mstr_slot_mask[1] = OnNetworkBodyAnalysis(strBody, "SLOT_MASK");
//		st_NW.mstr_test_idx[1] = OnNetworkBodyAnalysis(strBody, "TEST_IDX");
//		st_NW.mstr_error_log[1] = OnNetworkBodyAnalysis(strBody, "ERROR_LOG", 1);
//
//		nPcBoxNo = OnPCBOXIDToPCBOXNumber(st_NW.mstr_pcbox_id[1]);				// ID를 Box 번호로 바꾼다. 
//		st_work.mn_PCBOX_Status[nPcBoxNo] = WORK_END;							// 해당 Box가 End 되었다고 세팅한다.
//		st_work.mstr_PCBOX_Result[nPcBoxNo] = st_NW.mstr_test_result[1];		// 해당 BOX의 결과를 RESET한다.
//
//		if (st_work.mstr_PCBOX_Result[nPcBoxNo].GetLength() > 1)
//		{
//			if (st_work.mstr_PCBOX_Result[nPcBoxNo].Mid(0, 1) == "G")
//			{
////				nResult1 = 1;
//				nResult1 = BIN_GOODBIN;
//			}
//			else if (st_work.mstr_PCBOX_Result[nPcBoxNo].Mid(0, 1) == "F")
//			{
////				nResult1 = 2;
//				nResult1 = BIN_REJECTBIN;
//			}
//
//			if (st_work.mstr_PCBOX_Result[nPcBoxNo].Mid(1, 1) == "G")
//			{
////				nResult2 = 1;
//				nResult2 = BIN_GOODBIN;
//			}
//			else if (st_work.mstr_PCBOX_Result[nPcBoxNo].Mid(1, 1) == "F")
//			{
////				nResult2 = 2;
//				nResult2 = BIN_REJECTBIN;
//			}
//		}
//
//		if (nPcBoxNo < 60)			// Lower
//		{
////			st_board[LOWER_PART][nPcBoxNo].nboard_status = BOARD_TEST_END;
////			st_board[LOWER_PART][nPcBoxNo].nresult[0] = nResult1;
////			st_board[LOWER_PART][nPcBoxNo].nresult[1] = nResult2;
////			st_board[LOWER_PART][nPcBoxNo].strlot_no = st_NW.mstr_lot_id[1];
//
//			st_board[LOWER_PART][nPcBoxNo+10].nboard_status = BOARD_TEST_END;
//			st_board[LOWER_PART][nPcBoxNo+10].nresult[0] = nResult1;
//			st_board[LOWER_PART][nPcBoxNo+10].nresult[1] = nResult2;
//			st_board[LOWER_PART][nPcBoxNo+10].strlot_no = st_NW.mstr_lot_id[1];
//
//			// 20100408 tae추가
//			st_board[LOWER_PART][nPcBoxNo + 10].strpcbox_id		= st_NW.mstr_pcbox_id[1];
//		}
//		else						// Upper
//		{
////			st_board[UPPER_PART][nPcBoxNo - 60].nboard_status = BOARD_TEST_END;
////			st_board[UPPER_PART][nPcBoxNo - 60].nresult[0] = nResult1;
////			st_board[UPPER_PART][nPcBoxNo - 60].nresult[1] = nResult2;
////			st_board[UPPER_PART][nPcBoxNo - 60].strlot_no = st_NW.mstr_lot_id[1];
//
//			st_board[UPPER_PART][nPcBoxNo - 50].nboard_status = BOARD_TEST_END;
//			st_board[UPPER_PART][nPcBoxNo - 50].nresult[0] = nResult1;
//			st_board[UPPER_PART][nPcBoxNo - 50].nresult[1] = nResult2;
//			st_board[UPPER_PART][nPcBoxNo - 50].strlot_no = st_NW.mstr_lot_id[1];
//
//			// 20100408 tae추가
//			st_board[UPPER_PART][nPcBoxNo - 50].strpcbox_id		= st_NW.mstr_pcbox_id[1];
//		}
//
//		// 화면에 결과를 뿌린다.
//		if (st_handler.cwnd_main != NULL)
//		{
//			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_TESTRESULT, nPcBoxNo);
//		}
//		break;
//
//	case NW_PCBOX_CHANGE:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_oper_id[1] = OnNetworkBodyAnalysis(strBody, "OPER_ID");
//		st_NW.mstr_pcbox_cnt[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_CNT");
//		nPcBoxCnt = atoi(st_NW.mstr_pcbox_cnt[1]);
//		st_NW.mstr_pcbox_info[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_INFO", 1);
//
//		for (i = 0; i < nPcBoxCnt; i++)
//		{
//			strTemp.Format("PCBOX_%03d", i + 1);
//			st_NW.mstr_pcbox[1][i] = OnNetworkBodyAnalysis(st_NW.mstr_pcbox_info[1], strTemp);
//			strTemp.Format("LOCATION_%03d", i + 1);
//			st_NW.mstr_pcboxlocation[1][i] = OnNetworkBodyAnalysis(st_NW.mstr_pcbox_info[1], strTemp);
//			strTemp.Format("ENABLE_%03d", i + 1);
//			st_NW.mstr_pcboxenable[1][i] = OnNetworkBodyAnalysis(st_NW.mstr_pcbox_info[1], strTemp);
//		}
//
//		for (i = 0; i < 150; i++)
//		{
//			if (atoi(st_NW.mstr_pcboxlocation[1][i]) >= 0)		// 위치가 있을때...
//			{
//				if (atoi(st_NW.mstr_pcboxlocation[1][i]) < 60)			// Lower
//				{
////					st_pos[LOWER_PART][atoi(st_NW.mstr_pcboxlocation[1][i])].strpcbox_id = st_NW.mstr_pcbox[1][i];
////					st_pos[0][atoi(st_NW.mstr_pcboxlocation[1][i])].nused = atoi(st_NW.mstr_pcboxenable[1][i]); 
//					st_pos[LOWER_PART][atoi(st_NW.mstr_pcboxlocation[1][i])+10].strpcbox_id = st_NW.mstr_pcbox[1][i];
//					st_pos[LOWER_PART][atoi(st_NW.mstr_pcboxlocation[1][i])+10].nused = atoi(st_NW.mstr_pcboxenable[1][i]); 
//				}
//				else
//				{
////					st_pos[UPPER_PART][atoi(st_NW.mstr_pcboxlocation[1][i]) - 60].strpcbox_id = st_NW.mstr_pcbox[1][i];
////					st_pos[1][atoi(st_NW.mstr_pcboxlocation[1][i])].nused = atoi(st_NW.mstr_pcboxenable[1][i]); 
//					st_pos[UPPER_PART][atoi(st_NW.mstr_pcboxlocation[1][i]) - 50].strpcbox_id = st_NW.mstr_pcbox[1][i];
//					st_pos[UPPER_PART][atoi(st_NW.mstr_pcboxlocation[1][i]) - 50].nused = atoi(st_NW.mstr_pcboxenable[1][i]); 
//				}
//			}
//		}
//		break;
//
//	// [2009.11.13 20:24:00] : [[9998] BPC ==> EQ_PC : [FUNCTION=ENABLE LOT_ID=WEB9604946 EQP_ID=APT-09-1 OPER_ID=08051024 PCBOX_CNT=1 PCBOX_INFO=(PCBOX_ID_01=SG9PD4-2216) ]]
//	case NW_ENABLE:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_oper_id[1] = OnNetworkBodyAnalysis(strBody, "OPER_ID");
//		st_NW.mstr_pcbox_cnt[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_CNT");
//		nPcBoxCnt = atoi(st_NW.mstr_pcbox_cnt[1]);
//		st_NW.mstr_pcbox_info[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_INFO", 1);
//
//		for (i = 0; i < nPcBoxCnt; i++)
//		{
//			strTemp.Format("PCBOX_ID_%03d", i + 1);
//			st_NW.mstr_pcbox[1][i] = OnNetworkBodyAnalysis(st_NW.mstr_pcbox_info[1], strTemp);
//		}
//
//		for (i = 0; i < 150; i++)
//		{
//			if (i < 60)
//			{
//				if (st_pos[LOWER_PART][i+10].strpcbox_id == st_NW.mstr_pcbox[1][i])
//				{
////					st_pos[0][i].nused = YES;
//					st_pos[LOWER_PART][i+10].nused = YES;
//				}
//			}
//			else
//			{
//				if (st_pos[UPPER_PART][i - 50].strpcbox_id == st_NW.mstr_pcbox[1][i])
//				{
////					st_pos[1][i].nused = YES;
//					st_pos[UPPER_PART][i - 50].nused = YES;
//				}
//			}
//		}
//		break;
//
//	// [2009.11.11 21:32:39] : [FUNCTION=DISABLE EQPID=APT-09-1 OPERID=05101258 PCBOX_CNT=1 PCBOX_INFO=(PCBOX_ID_01=SG9PD4-2488) TYPE=PCBOX CODE= DESC="" ]
//	case NW_DISABLE:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_oper_id[1] = OnNetworkBodyAnalysis(strBody, "OPER_ID");
//		st_NW.mstr_pcbox_cnt[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_CNT");
//		nPcBoxCnt = atoi(st_NW.mstr_pcbox_cnt[1]);
//		st_NW.mstr_pcbox_info[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_INFO", 1);
//		for (i = 0; i < nPcBoxCnt; i++)
//		{
//			strTemp.Format("PCBOX_ID_%03d", i + 1);
//			st_NW.mstr_pcbox[1][i] = OnNetworkBodyAnalysis(st_NW.mstr_pcbox_info[1], strTemp);
//		}
//
//		for (i = 0; i < 150; i++)
//		{
//			if (i < 60)
//			{
//				if (st_pos[LOWER_PART][i + 10].strpcbox_id == st_NW.mstr_pcbox[1][i])
//				{
////					st_pos[0][i].nused = NO;
//					st_pos[LOWER_PART][i + 10].nused = NO;
//				}
//			}
//			else
//			{
//				if (st_pos[UPPER_PART][i - 50].strpcbox_id == st_NW.mstr_pcbox[1][i])
//				{
//					st_pos[UPPER_PART][i - 50].nused = NO;
//				}
//			}
//		}
//		break;
//
//	case NW_SLOT_MASK:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_oper_id[1] = OnNetworkBodyAnalysis(strBody, "OPER_ID");
//		st_NW.mstr_pcbox_cnt[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_CNT");
//		nPcBoxCnt = atoi(st_NW.mstr_pcbox_cnt[1]);
//		st_NW.mstr_pcbox_info[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_INFO", 1);
//		for (i = 0; i < nPcBoxCnt; i++)
//		{
//			strTemp.Format("PCBOX_ID_%03d", i + 1);
//			st_NW.mstr_pcbox[1][i] = OnNetworkBodyAnalysis(st_NW.mstr_pcbox_info[1], strTemp);
//		}
//		st_NW.mstr_slot[1] = OnNetworkBodyAnalysis(strBody, "SLOT");
//		break;
//
//	case NW_LOT_START_ENABLE:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_temp[1] = OnNetworkBodyAnalysis(strBody, "TEMP");
//
//		// 데이터가 정상인지 검사.
//		if (st_NW.mstr_lot_id[1] == st_work.str_prev_lot_no && st_NW.mstr_step_cnt[1] == st_work.str_prev_step_no)
//		{
//			st_NW.mstr_result[1] = "FAIL";
//			st_NW.mstr_fail_log[1] = "()";
//			return RET_ERROR;
//		}
//		else
//		{
//			st_NW.mstr_result[1] = "PASS";
//			st_NW.mstr_fail_log[1] = "()";
//		}
//		break;
//
//	case NW_PCBOX_REMOVE:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_pcbox_id[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_ID");
//		break;
//
//	case NW_PCBOX_ERROR:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_pcbox_id[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_ID");
//		break;
//// 여기까지는 장비가 질문/통보를 받는 부분이다.
//
//// 여기서부터는 장비의 질문에 답변이 온 부분이다.
//	case NW_PCBOX_LOT_INFO_RPY:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_pcbox_id[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_ID");
//		st_NW.mstr_result[1] = OnNetworkBodyAnalysis(strBody, "RESULT");
//		st_NW.mstr_fail_log[1] = OnNetworkBodyAnalysis(strBody, "FAIL_LOG", 1);
//		break;
//
//	case NW_FAIL_INFO_SEND_RPY:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_fail_no[1] = OnNetworkBodyAnalysis(strBody, "FAIL_NO");
//		st_NW.mstr_message_id[1] = OnNetworkBodyAnalysis(strBody, "MESSAGE_ID");
//		st_NW.mstr_fail_code[1] = OnNetworkBodyAnalysis(strBody, "FAIL_CODE");
//		st_NW.mstr_fail_info[1] = OnNetworkBodyAnalysis(strBody, "FAIL_INFO");
//		st_NW.mstr_result[1] = OnNetworkBodyAnalysis(strBody, "RESULT");
//		break;
//
//	case NW_FAIL_REGNUMBER_REQUEST_RPY:
//		st_NW.mstr_pcbox_id[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_ID");
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_slot_no[1] = OnNetworkBodyAnalysis(strBody, "SLOT_NO");
//		st_NW.mstr_test_idx[1] = OnNetworkBodyAnalysis(strBody, "TEST_IDX");
//		st_NW.mstr_result[1] = OnNetworkBodyAnalysis(strBody, "RESULT");
//
//// 		m_failinfo_table_name[0]	= "no";
//// 		m_failinfo_table_name[1]	= "PCBOX_ID";
//// 		m_failinfo_table_name[2]	= "LOT_ID";
//// 		m_failinfo_table_name[3]	= "STEP_CNT";
//// 		m_failinfo_table_name[4]	= "SLOT_NO";
//// 		m_failinfo_table_name[5]	= "TEST_IDX";
//// 		m_failinfo_table_name[6]	= "FAIL_REG";
//// 		m_failinfo_table_name[7]	= "FAIL_DESC";
//// 		m_failinfo_table_name[8]	= "FAIL_CODE";
//// 		m_failinfo_table_name[9]	= "MEMO";
//	
//		strTmp[1] = st_NW.mstr_pcbox_id[1];
//		strTmp[2] = st_NW.mstr_lot_id[1];
//		strTmp[3] = st_NW.mstr_step_cnt[1];
//		strTmp[4] = st_NW.mstr_slot_no[1];
//		strTmp[5] = st_NW.mstr_test_idx[1];
//
//		if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
//		{
//			if (Mysql.Database_Select("AMT"))
//			{	
//				if (Mysql.Table_Select("AMT", "FAIL_INFO"))
//				{
//					Mysql.Record_Add("FAIL_INFO", strTmp);
//				}
//				else
//				{
//					if (Mysql.Table_Create("AMT", "FAIL_INFO", 10, Mysql.m_failinfo_table_name, Mysql.m_fi_table_type, Mysql.m_fi_table_num))
//					{
//						Mysql.Record_Add("FAIL_INFO", strTmp);
//					}
//				}
//			}
//			else
//			{
//				if (Mysql.Database_Create("AMT"))
//				{
//					if (Mysql.Table_Create("AMT", "FAIL_INFO", 10, Mysql.m_failinfo_table_name, Mysql.m_fi_table_type, Mysql.m_fi_table_num))
//					{
//						Mysql.Record_Add("FAIL_INFO", strTmp);
//					}
//				}
//			}
//		}
//		Mysql.MySql_Close();
//		break;
//
//	case NW_FAIL_INFO_SERACH_RPY:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_fail_regnumber[1] = OnNetworkBodyAnalysis(strBody, "FAIL_REGNUMBER");
//		st_NW.mstr_result[1] = OnNetworkBodyAnalysis(strBody, "RESULT");
//		break;
//
//	case NW_FAIL_INFO_SAVE_RPY:
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_fail_regnumber[1] = OnNetworkBodyAnalysis(strBody, "FAIL_REGNUMBER");
//		st_NW.mstr_pcbox_id[1] = OnNetworkBodyAnalysis(strBody, "PCBOX_ID");
//		st_NW.mstr_slot_no[1] = OnNetworkBodyAnalysis(strBody, "SLOT_NO");
//		st_NW.mstr_test_idx[1] = OnNetworkBodyAnalysis(strBody, "TEST_IDX");
//		st_NW.mstr_result[1] = OnNetworkBodyAnalysis(strBody, "RESULT");
//		break;
//
//	case NW_FAIL_INFO_DELETE_RPY:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_fail_regnumber[1] = OnNetworkBodyAnalysis(strBody, "FAIL_REGNUMBER");
//		st_NW.mstr_result[1] = OnNetworkBodyAnalysis(strBody, "RESULT");
//		break;
//
//	case NW_LOT_DISPLAY_RPY:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_oper_cnt[1] = OnNetworkBodyAnalysis(strBody, "OPER_CNT");
//		st_NW.mstr_oper_info[1] = OnNetworkBodyAnalysis(strBody, "OPER_INFO");
//		st_NW.mstr_lot_qty[1] = OnNetworkBodyAnalysis(strBody, "LOT_QTY");
//		st_NW.mstr_cycle_id[1] = OnNetworkBodyAnalysis(strBody, "CYCLE_ID");
//		st_NW.mstr_part_id[1] = OnNetworkBodyAnalysis(strBody, "PART_NO");
//		st_NW.mstr_result[1] = OnNetworkBodyAnalysis(strBody, "RESULT");
//		st_NW.mstr_msg[1] = OnNetworkBodyAnalysis(strBody, "MSG", 1);
//		break;
//
//	case NW_EQP_LOT_START_RPY:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_result[1] = OnNetworkBodyAnalysis(strBody, "RESULT");
//		st_NW.mstr_msg[1] = OnNetworkBodyAnalysis(strBody, "MSG", 1);
//		break;
//
//	case NW_EQP_LOT_CANCEL_RPY:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_result[1] = OnNetworkBodyAnalysis(strBody, "RESULT");
//		st_NW.mstr_msg[1] = OnNetworkBodyAnalysis(strBody, "MSG", 1);
//		break;
//
//	case NW_EQP_LOT_END_RPY:
//		st_NW.mstr_lot_id[1] = OnNetworkBodyAnalysis(strBody, "LOT_ID");
//		st_NW.mstr_eqp_id[1] = OnNetworkBodyAnalysis(strBody, "EQP_ID");
//		st_NW.mstr_step_cnt[1] = OnNetworkBodyAnalysis(strBody, "STEP_CNT");
//		st_NW.mstr_recyclelotid[1] = OnNetworkBodyAnalysis(strBody, "RECYCLELOTID");
//		st_NW.mstr_recycleqty[1] = OnNetworkBodyAnalysis(strBody, "RECYCLEQTY");
//		st_NW.mstr_result[1] = OnNetworkBodyAnalysis(strBody, "RESULT");
//		st_NW.mstr_msg[1] = OnNetworkBodyAnalysis(strBody, "MSG", 1);
//		break;
//// 여기까지는 장비의 질문에 답변이 온 부분이다.
//	}
//
//	return nFuncRet;
//}
//
//CString CPublic_Function::OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt)
//{
//	CString strFuncRet;
//	int n_title, n_equal, n_end, n_length, n_qw = 0;
//	
//	n_title =	strBody.Find(strFind, 0);							// TITLE의 위치를 찾는다.
//	n_equal =	strBody.Find("=", n_title + 1);						// 해당 item의 끝 위치를 찾는다.
//
//	if (n_title == -1)		return "NULL";
//
//	if (nOpt == 0)
//	{
//		n_end =		strBody.Find(" ", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
//		n_length =	n_end - n_equal - 1;							// DATA의 길이 계산.
//		strFuncRet = strBody.Mid(n_equal + 1, n_length);
//	}
//	else
//	{
//		n_qw =		strBody.Find("(", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
//		n_end =		strBody.Find(")", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
//		n_length =	n_end - n_qw - 1;								// DATA의 길이 계산.
//		strFuncRet = strBody.Mid(n_qw + 1, n_length);
//	}
//
//	return strFuncRet;
//}



int CPublic_Function::RobotPickerUpSensorCheck(int n_mode, int n_Site)
{
	int iPicker;
	CString strIo;
	unsigned long k = 0;

	if(n_mode != 2)
	{
		if(n_Site == LDMODULE_SITE)
		{
			mn_retry = 0;
			for(iPicker = 0 ; iPicker < PICKER_NUM ; iPicker++)
			{
				if(FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[iPicker], IO_ON) == IO_ON ||
					FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[iPicker], IO_OFF) == IO_OFF)
				{
					mn_retry++;
					if(mn_retry > 1)
					{
						mn_retry = 0;	
						//다운된 피커가 있다!!
						if(st_handler.cwnd_list != NULL)
						{
							strIo.Format("1_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[iPicker], IO_ON),FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[iPicker], IO_ON));
							sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
						}

						sprintf(cJamcode,"120101"); 
						CTL_Lib.Alarm_Error_Occurrence(5048, CTL_dWARNING, cJamcode);

						return CTL_ERROR;
					}
				}
			}
		}
		if(n_Site == LEFTSINK_SITE)
		{
			mn_retry = 0;
			for(iPicker = 0 ; iPicker < PICKER_NUM ; iPicker++)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[iPicker], IO_ON) == IO_ON ||
					FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_up_chk[iPicker], IO_OFF) == IO_OFF)				
				{	
					mn_retry++;
					if(mn_retry > 1)
					{
						mn_retry = 0;					
						//다운된 피커가 있다!!
						if(st_handler.cwnd_list != NULL)
						{
							strIo.Format("2_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[iPicker], IO_ON),
								FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_up_chk[iPicker], IO_ON));
							sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
						}

						//400201 1 40 "Unloader Picker Up Check Error."
						sprintf(cJamcode,"140101");
						CTL_Lib.Alarm_Error_Occurrence(5049, CTL_dWARNING, cJamcode);

						return CTL_ERROR;
					}
					
				}
			}
		}
		if(n_Site == RIGHTSINK_SITE)
		{
			mn_retry = 0;
			for(iPicker = 0 ; iPicker < PICKER_NUM ; iPicker++)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[iPicker], IO_ON) == IO_ON ||
					FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[iPicker], IO_OFF) == IO_OFF)				
				{	
					mn_retry++;
					if(mn_retry > 1)
					{
						mn_retry = 0;
						if(st_handler.cwnd_list != NULL)
						{
							strIo.Format("2_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[iPicker], IO_ON),
								FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[iPicker], IO_ON));
							sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
						}

						//400201 1 40 "Unloader Picker Up Check Error."
						sprintf(cJamcode,"160101");
						CTL_Lib.Alarm_Error_Occurrence(5050, CTL_dWARNING, cJamcode);

						return CTL_ERROR;								
						//다운된 피커가 있다!!
					}	
				}
			}
		}
		if(n_Site == LDCLIP_SITE)
		{
			if(FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON) == IO_ON ||
				FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_OFF)				
			{				
				//다운된 피커가 있다!!
				if(st_handler.cwnd_list != NULL)
				{
					strIo.Format("2_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON), FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON));
					sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}

				sprintf(cJamcode,"260101"); 
				CTL_Lib.Alarm_Error_Occurrence(5051, CTL_dWARNING, cJamcode);

				return CTL_ERROR;				
			}		
		}		
		if(n_Site == SORTER_SITE)
		{
			if(FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_dn_chk, IO_OFF) == IO_ON ||
				FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_up_chk, IO_ON) == IO_OFF)				
			{				
				//다운된 피커가 있다!!
				if(st_handler.cwnd_list != NULL)
				{
					strIo.Format("2_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_picker_rbt_dn_chk, IO_ON), FAS_IO.get_in_bit(st_io.i_picker_rbt_up_chk, IO_ON));
					sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}

				//440201 1 44 "Sorter Picker Down Check Error.[PS0308]."
				sprintf(cJamcode,"440201");
				CTL_Lib.Alarm_Error_Occurrence(5052, CTL_dWARNING, cJamcode);

				return CTL_ERROR;				
			}		
		}		
		if(n_Site == UNLOADER_SITE || n_Site == REJECT_SITE)
		{
			if(FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dn_chk, IO_OFF) == IO_ON ||
				FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_up_chk, IO_ON) == IO_OFF)				
			{				
				//다운된 피커가 있다!!
				if(st_handler.cwnd_list != NULL)
				{
					strIo.Format("2_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dn_chk, IO_ON), FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_up_chk, IO_ON));
					sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				//440100 1 44 "Unloader Picker Up Check Error.[PS0300]."
				sprintf(cJamcode,"440100");
				CTL_Lib.Alarm_Error_Occurrence(5053, CTL_dWARNING, cJamcode);

				return CTL_ERROR;				
			}
		}
	}
	else
	{
		if(n_Site == LDMODULE_SITE)
		{
			for(iPicker = 0 ; iPicker < PICKER_NUM ; iPicker++)
			{
				if(FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[iPicker], MAX_DELAY_IO) == IO_OFF)
				{
					//다운된 피커가 있다!!
					if(st_handler.cwnd_list != NULL)
					{
						strIo.Format("3_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[iPicker], IO_ON),
							FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[iPicker], IO_ON));
						sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}

					//400100 1 40 "Picker #1 Up Check Error."
					sprintf(cJamcode,"40010%d", iPicker+1);
					CTL_Lib.Alarm_Error_Occurrence(5054, CTL_dWARNING, cJamcode);

					return CTL_ERROR;
				}
			}
		}
		if(n_Site == LEFTSINK_SITE)
		{
			for(iPicker = 0 ; iPicker < PICKER_NUM ; iPicker++)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_up_chk[iPicker], MAX_DELAY_IO) == IO_OFF)
				{
					//다운된 피커가 있다!!
					if(st_handler.cwnd_list != NULL)
					{
						strIo.Format("4_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[iPicker], IO_ON),
							FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_up_chk[iPicker], IO_ON));
						sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}

					//400201 1 40 "Unloader Picker Up Check Error."
					sprintf(cJamcode,"40020%d", iPicker+1);
					CTL_Lib.Alarm_Error_Occurrence(5055, CTL_dWARNING, cJamcode);

					return CTL_ERROR;				
				}
			}
		}
		if(n_Site == RIGHTSINK_SITE)
		{
			for(iPicker = 0 ; iPicker < PICKER_NUM ; iPicker++)
			{
				if(FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[iPicker], MAX_DELAY_IO) == IO_OFF)
				{
					//다운된 피커가 있다!!
					if(st_handler.cwnd_list != NULL)
					{
						strIo.Format("4_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[iPicker], IO_ON),
							FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[iPicker], IO_ON));
						sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
					}

					//400201 1 40 "Unloader Picker Up Check Error."
					sprintf(cJamcode,"40020%d", iPicker+1);
					CTL_Lib.Alarm_Error_Occurrence(5056, CTL_dWARNING, cJamcode);

					return CTL_ERROR;				
				}
			}
		}
		if(n_Site == LDCLIP_SITE)
		{
			if(FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, MAX_DELAY_IO) == IO_OFF)
			{
				//다운된 피커가 있다!!
				if(st_handler.cwnd_list != NULL)
				{
					strIo.Format("4_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON),
						FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON));
					sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				//400201 1 40 "Unloader Picker Up Check Error."
				sprintf(cJamcode,"40020%d", iPicker+1);
				CTL_Lib.Alarm_Error_Occurrence(5057, CTL_dWARNING, cJamcode);

				return CTL_ERROR;				
			}
		}
		if(n_Site == SORTER_SITE)
		{
			if(FAS_IO.get_in_bit(st_io.i_sorter_picker_rbt_up_chk, MAX_DELAY_IO) == IO_OFF)
			{
				//다운된 피커가 있다!!
				if(st_handler.cwnd_list != NULL)
				{
					strIo.Format("4_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_picker_rbt_dn_chk, IO_ON),
						FAS_IO.get_in_bit(st_io.i_picker_rbt_up_chk, IO_ON));
					sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				//400201 1 40 "Unloader Picker Up Check Error."
				sprintf(cJamcode,"40020%d", iPicker+1);
				CTL_Lib.Alarm_Error_Occurrence(5058, CTL_dWARNING, cJamcode);

				return CTL_ERROR;				
			}
		}

		if(n_Site == UNLOADER_SITE)
		{
			if(FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_up_chk, MAX_DELAY_IO) == IO_OFF)
			{
				//다운된 피커가 있다!!
				if(st_handler.cwnd_list != NULL)
				{
					strIo.Format("4_UpIn:%d DnIn:%d",FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_dn_chk, IO_ON),
						FAS_IO.get_in_bit(st_io.i_uld_picker_rbt_up_chk, IO_ON));
					sprintf(st_msg.c_abnormal_msg,"[Safety Error] %s",strIo);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
				//400201 1 40 "Unloader Picker Up Check Error."
				sprintf(cJamcode,"40020%d", iPicker+1);
				CTL_Lib.Alarm_Error_Occurrence(5059, CTL_dWARNING, cJamcode);

				return CTL_ERROR;				
			}
		}

	}
	return CTL_GOOD;
}


int CPublic_Function::OnRetNextBufferPos(int nPos)
{
	int nRetPos = 0;
	nRetPos = nPos;

	if(st_picker.n_work_buffer_enable[nRetPos] == YES)
	{
		nRetPos = nPos;
	}
	else
	{
		while (1)
		{
			nRetPos++;
			if(nRetPos >= 4) nRetPos = WORK_BUFFER_1;
			if(st_picker.n_work_buffer_enable[nRetPos] == YES)
			{
				return nRetPos;
			}
		}
	}

	return nRetPos;
}

int CPublic_Function::ChkTrayForLotEnd(int n_Site)
{
	int i, j, num = 0;
	switch(n_Site)
	{
		case LEFTSINK_SITE:
			for (i = 0; i < 2; i++)
			{
				for (j = 0; j < st_traybuffer[LEFTSINK_SITE].i_loader_row; j++)
				{
					if(st_modulemap.LefthsTray[i][j] == DVC_YES)
					{
						num++;
					}
				}
			}
			break;

		case RIGHTSINK_SITE:
			for (i = 0; i < 2; i++)
			{
				for (j = 0; j < st_traybuffer[RIGHTSINK_SITE].i_loader_row; j++)
				{
					if(st_modulemap.RighthsTray[i][j] == DVC_YES)
					{
						num++;
					}
				}
			}
			break;
	}

	return num;
}


int CPublic_Function::ChkForLotEnd(int n_Site)
{
	int i;
	int nFuncRet = -1;
	int nDvcNum = 0;
	//버퍼 모듈에 디바이스가 남아있는지 
	if(st_work.nMdlInputCount[0][0] >= (st_work.n_loadlot_count[LDMODULE_SITE] + 2) && st_work.n_lotend != CTL_YES) 
	{
		nFuncRet = RET_GOOD;
	}
	//MODULE_SITE
//	else if(st_work.n_lot_count == st_work.n_loadlot_count)
//	{
//		nFuncRet = RET_ERROR;
//	}
	else
	{
		switch(n_Site)
		{
			case LEFTSINK_SITE:
				//if(st_sync.n_lotend_module_ldrbt == CTL_YES)
				//{
					for (i = 0; i < 4; i++)
					{
						 if(st_sync.n_module_site_status[LDMODULE_SITE][i][0] == BUFF_DVC_LOAD &&
							 st_sync.n_module_site_status[LEFTSINK_SITE][i][0] == BUFF_EMPTY)
						 {
							 nDvcNum++;
						 }
					}
				//}
				//else
				//{
				//	nDvcNum = PICKER_NUM;
				//}
				break;

			case RIGHTSINK_SITE:
				//if(st_sync.n_lotend_lefths_ldrbt == CTL_YES)
				//{
					for (i = 0; i < 4; i++)
					{
						 if(st_sync.n_module_site_status[LDMODULE_SITE][i][0] == BUFF_DVC_LOAD &&
							 st_sync.n_module_site_status[RIGHTSINK_SITE][i][0] == BUFF_EMPTY)
						 {
							 nDvcNum++;
						 }
					}
				//}
				//else
				//{
				//	nDvcNum = PICKER_NUM;
				//}
				break;
				
			case LDCLIP_SITE:
				//if(st_sync.n_lotend_righths_ldrbt == CTL_YES)
				//{
					for (i = 0; i < 4; i++)
					{
						 if(st_sync.n_module_site_status[LDMODULE_SITE][i][0] == BUFF_DVC_LOAD &&
							 st_sync.n_module_site_status[LDCLIP_SITE][i][0] == BUFF_EMPTY)
						 {
							 nDvcNum++;
						 }
					}
				//}
				//else
				//{
			//		nDvcNum = 1;
				//}
				break;

			case SORTER_SITE:
				//if(st_sync.n_lotend_clip_ldrbt == CTL_YES)
				//{
					for (i = 0; i < 4; i++)
					{
						 if(st_sync.n_module_site_status[LDMODULE_SITE][i][0] == BUFF_DVC_LOAD && 
							 st_sync.n_module_site_status[SORTER_SITE][i][0] == BUFF_EMPTY)
						 {
							 nDvcNum++;
						 }
					}
			//	}
				//else
				//{
				//	nDvcNum = 1;
				//}
				break;
		}
		
		if(nDvcNum >= PICKER_NUM) nFuncRet = RET_GOOD;
		else
		{
			if(nDvcNum == 1)  nFuncRet = RET_PROCEED;
			else              nFuncRet = RET_ERROR;
		}
	}

	return nFuncRet;
}

//////////////20120711
void CPublic_Function::Database_Worker(st_worker_db db)
{
	CString		strTmp[5];
	
	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d",	
		              db.time.GetYear(), 
					  db.time.GetMonth(),
				      db.time.GetDay(),
					  db.time.GetHour(),
					  db.time.GetMinute(),
					  db.time.GetSecond());
	
	strTmp[2] = db.str_worker_name;
	strTmp[3] = db.str_worker_crem;
	strTmp[4] = db.str_worker_no;
	
	if(Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		// amt database 선택
		if(Mysql.Database_Select("AMT830"))
		{
			//  lot_data table 선택 
			if(Mysql.Table_Select("AMT830", "WORKER_DB"))
			{
				// lot_data table에 데이터 추가
				Mysql.Record_Add("WORKER_DB", strTmp);
			}
			else
			{
				// table이 없으면 새로 table 추가
				if(Mysql.Table_Create("AMT830", "WORKER_DB", 5, Mysql.m_str_worker_table_name, Mysql.m_n_worker_table_type, Mysql.m_n_worker_table_num))
				{
					// lot_data table에 데이터 추가
					Mysql.Record_Add("WORKER_DB", strTmp);
				}
			}
		}
		else
		{
			// amt database가 없으면 database 추가
			if(Mysql.Database_Create("AMT830"))
			{
				// lot_data table 새로 추가 
				if(Mysql.Table_Create("AMT830", "WORKER_DB", 5, Mysql.m_str_worker_table_name, Mysql.m_n_worker_table_type, Mysql.m_n_worker_table_num))
				{
					Mysql.Record_Add("WORKER_DB", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();
}

/////////////////////////////
void CPublic_Function::OnLoadModule_Tray_Clear()
{
	int i;

	for(i = 0 ; i < st_traybuffer[LDMODULE_SITE] .i_loader_row ; i++)
	{
		st_modulemap.LoadTray[1][i] = NO;			// Reset 
	}

	if(st_handler.cwnd_main != NULL)
	{
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOADINFO, 0);  //로더 정보 
	}
}

void CPublic_Function::Set_PickerUpDown(int n_Site, int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i;
	if(n_Site == LDMODULE_SITE)
	{
		for (i = 0; i < PICKER_NUM; i++)
		{
			if(PickerInfo[D_EXIST][i] == YES)
			{
				FAS_IO.set_out_bit(st_io.o_ld_module_picker_updn[i], OnOff);
			}
			b_PickerFlag[i] = FALSE;
			l_PickerWaitTime[i][0] = GetCurrentTime();
		}
	}
	else if(n_Site == LEFTSINK_SITE)
	{
	}
	else if(n_Site == RIGHTSINK_SITE)
	{
	}
}

int CPublic_Function::Get_PickerUpDown(int n_Site, int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i, FuncRet = RET_PROCEED;
	int RetVal[PICKER_NUM] = {0,};
	char jamcode[10] = {0,};
	CString str;

	if(n_Site == LDMODULE_SITE)
	{
		for(i = 0 ; i < PICKER_NUM ; i++)
		{
			RetVal[i] = NO;
			if(OnOff == IO_ON) //Picker ON (Down이라면..)
			{
				if(PickerInfo[D_EXIST][i] == YES) //동작 시킨 피커면!
				{
					if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_ON) == IO_ON
						&& FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[i], IO_OFF) == IO_OFF)
					{
						b_PickerFlag[i] = TRUE;
						l_PickerWaitTime[i][0] = GetCurrentTime();   
					}
					else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_ON) == IO_ON
						&& FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[i], IO_OFF) == IO_OFF)
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();  
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
						
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[38] + (i*2))
						{
							RetVal[i] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();  
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						
						if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[38] + (i*2))
						{
							//400200 1 40 "Module Picker #1 Down Check Error."						
							sprintf(jamcode, "40020%d", i);
							sprintf(mc_alarmcode, "40020%d", i);						
							alarm.mstr_pcode = _T(jamcode); 
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							alarm.mn_alarm_assign_section = 186;

							FuncRet = RET_ERROR;
						}
					}
				}
				else RetVal[i] = YES;
			}
			else //Picker OFF (UP이라면..)
			{
				if(PickerInfo[D_EXIST][i] == YES) //동작 시킨 피커면!
				{
					if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_OFF) == IO_OFF
						&& FAS_IO.Get_In_Bit(st_io.i_ld_module_picker_up_chk[i]) == IO_ON)
					{
						b_PickerFlag[i] = TRUE;
						l_PickerWaitTime[i][0] = GetCurrentTime();   
					}
					else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_OFF) == IO_OFF
						&& FAS_IO.Get_In_Bit(st_io.i_ld_module_picker_up_chk[i]) == IO_ON)
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();  
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
						
						if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[38] + (i*2))
						{
							RetVal[i] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();  
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						
						if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[38] + (i*2))
						{
							if(st_handler.cwnd_list != NULL)
							{
								str = _T("[UPICKER Error] Picker up sensor check error.");
								sprintf(st_msg.c_normal_msg,"%s", str);
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
								Func.On_LogFile_Add(0, str);
								Func.On_LogFile_Add(99, str);
							}
							//400100 1 40 "Module Picker #1 Up Check Error."
							sprintf(jamcode, "40010%d", i);
							sprintf(mc_alarmcode,  "40010%d", i);					
							alarm.mstr_pcode = _T(jamcode); 
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							//st_handler.i_run_status = dWARNING;

							alarm.mn_alarm_assign_section = 187;

							FuncRet = RET_ERROR;
						}
					}
				}
				else RetVal[i] = YES;
			}
		}
		
		if(RetVal[0] == YES && RetVal[1] == YES) 
		{
			FuncRet = RET_GOOD;
		}
	}
	else if(n_Site == LEFTSINK_SITE)
	{
	}
	else if(n_Site == RIGHTSINK_SITE)
	{
	}


	return FuncRet;
}

int CPublic_Function::GetMotCurrentSite(int n_axis, double d_TargetPos)
{

	int nRetSite=-1, i=0, nSite=0, nPos=0;
	double dCurrentPos=0;

	if(n_axis == M_VISION1_Y || n_axis == M_VISION2_Y)
	{
		nSite = 5;
		if(d_TargetPos == 0)
		{
			dCurrentPos = COMI.Get_MotCurrentPos(n_axis);

			for (i = 0; i < nSite; i++)
			{
				if(dCurrentPos > st_motor[n_axis].md_pos[i] - st_motor[n_axis].mn_allow * 10 &&
					dCurrentPos < st_motor[n_axis].md_pos[i] + st_motor[n_axis].mn_allow * 10)
				{
					if(i == 2) i = 1;//VISION_Y_LABEL_POS->VISION_Y_LDRBT_POS
					else if(i == 0) i = 3;//VISION_Y_READY_POS->VISION_Y_VISION_POS
					nRetSite = i;
					break;
				}
			}
		}
		else
		{
			for (i = 0; i < nSite; i++)
			{
				if(d_TargetPos > st_motor[n_axis].md_pos[i] - st_motor[n_axis].mn_allow * 10 &&
					d_TargetPos < st_motor[n_axis].md_pos[i] + st_motor[n_axis].mn_allow * 10)
				{
					if(i == 2) i = 1;//VISION_Y_LABEL_POS->VISION_Y_LDRBT_POS
					else if(i == 0) i = 3;//VISION_Y_READY_POS->VISION_Y_VISION_POS
					nRetSite = i;
					break;
				}
			}
		}
	}
	return nRetSite;
}

double CPublic_Function::fMainMax(int n_axis, int opt)//opt : 0 Min opt 1: max
{
	int i=0, nSite=0;
	double fValue = 0;

	if(n_axis == M_VISION1_Z || n_axis == M_VISION2_Z)
	{
		nSite = 5;
		if(opt == 0)
		{
			fValue = st_motor[n_axis].md_pos[0];
			for (i = 0; i < nSite; i++)
			{
				if(fValue > st_motor[n_axis].md_pos[i])
				{
					fValue = st_motor[n_axis].md_pos[i];
				}
			}
		}
		else
		{
			fValue = st_motor[n_axis].md_pos[0];
			for (i = 0; i < nSite; i++)
			{
				if(fValue < st_motor[n_axis].md_pos[i])
				{
					fValue = st_motor[n_axis].md_pos[i];
				}
			}
		}
	}

	return fValue;
}

#include "Cmmsdk.h"

int CPublic_Function::CheckVisionPos(int n_axis, double d_targetpos)
{
	double d_CurPos[5]={0,}, d_CompareVal=0, d_CompareVal1=0,d_CompareVal2=0,d_CompareVal3=0;
	int nPos1=0, nPos2=0, nPos3=0, nPos4=0;
	int nRet, nFuncRet = CTLBD_RET_PROCEED;

	double d_CompareVal4=0,d_CompareVal5=0,d_CompareVal6=0;
	int nPos5=0, nPos6=0, nPos7=0;
	long dwMotionDone=0;

	d_CurPos[0] = COMI.Get_MotCurrentPos(M_VISION1_Z);
	d_CurPos[1] = COMI.Get_MotCurrentPos(M_VISION2_Z);
	d_CurPos[2] = COMI.Get_MotCurrentPos(M_VISION1_Y);
	d_CurPos[3] = COMI.Get_MotCurrentPos(M_VISION2_Y);
	d_CurPos[4] = COMI.Get_MotCurrentPos(M_LABEL_ATTACH_Z);//2014.0418


	switch(n_axis)
	{
	case M_VISION1_Z:
		////2015.0128
		if(d_targetpos <= st_motor[M_VISION1_Z].md_pos[VISION_Z_LDMODULE] + st_motor[M_VISION1_Z].mn_allow &&
		   d_targetpos >= st_motor[M_VISION1_Z].md_pos[VISION_Z_LDMODULE] - st_motor[M_VISION1_Z].mn_allow)//load module 위치
		{
//			d_CompareVal1 =  (double)fabs(d_targetpos - d_CurPos[1]);
//			d_CompareVal2 = st_motor[M_VISION1_Z].md_pos[VISION_Z_LDMODULE] - st_motor[M_VISION2_Z].md_pos[VISION_Z_DOWN_READY];
//			if(d_CompareVal1 >= d_CompareVal2 - st_motor[n_axis].mn_allow)
//			{
//			}
//			else
//			{//360009 0 36 "Vision1 Z 로봇 목표위치가 안전위치에 있지 않음."
//				nPos1 = GetMotCurrentSite(M_VISION1_Y);
//				nPos2 = GetMotCurrentSite(M_VISION2_Y);
//				if(nPos1 != -1 && nPos1 != nPos2) //nPos2가 동작중일 경우 -1 
//				{
//					nFuncRet = CTLBD_RET_GOOD;
//				}
//				else
//				{
//					sprintf(cJamcode, "%02d0009", n_axis);
//					CTL_Lib.Alarm_Error_Occurrence(5060, CTL_dWARNING, cJamcode);
//					nFuncRet = CTLBD_RET_ERROR;
//				}
//			}
			//360009 0 36 "Vision1 Z "
			nPos1 = GetMotCurrentSite(M_VISION1_Y);
			nPos2 = GetMotCurrentSite(M_VISION2_Y);
			nRet = cmmSxIsDone(M_VISION2_Y, &dwMotionDone);			// Motion Done이냐?
			if(nPos2 == -1 || dwMotionDone != cmTRUE)
			{
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					CString strLog;
					strLog.Format("[CheckVisionPos1_1] SxIsDone: M_VISION2_Y =  %d , MotionDone = %d", nPos2,dwMotionDone);
					Func.On_LogFile_Add(LOG_HISTORY, strLog);
				}
				nFuncRet = CTLBD_RET_PROCEED;

			}
			else if(nPos1 != -1 && nPos1 != nPos2)
			{
				nFuncRet = CTLBD_RET_GOOD;
			}
			else
			{
				sprintf(COMI.mc_alarmcode, "%02d0009", n_axis);
				CTL_Lib.Alarm_Error_Occurrence(5060, CTL_dWARNING, COMI.mc_alarmcode);
				nFuncRet = CTLBD_RET_ERROR;
			}
		}
		else if(d_targetpos <= st_motor[M_VISION1_Z].md_pos[VISION_Z_LABEL] + st_motor[M_VISION1_Z].mn_allow &&
		        d_targetpos >= st_motor[M_VISION1_Z].md_pos[VISION_Z_LABEL] - st_motor[M_VISION1_Z].mn_allow)//label 위치
		{
//			d_CompareVal1 =  (double)fabs(d_targetpos - d_CurPos[1]);
//			d_CompareVal2 = st_motor[M_VISION1_Z].md_pos[VISION_Z_LABEL] - st_motor[M_VISION1_Z].md_pos[VISION_Z_DOWN_READY];
//			if(d_CompareVal1 >= d_CompareVal2 - st_motor[n_axis].mn_allow)
//			{
//			}
//			else
//			{//360009 0 36 "Vision1 Z 로봇 목표위치가 안전위치에 있지 않음."
//				nPos1 = GetMotCurrentSite(M_VISION1_Y);
//				nPos2 = GetMotCurrentSite(M_VISION2_Y);
//				if(nPos1 != -1 && nPos1 != nPos2) //nPos2가 동작중일 경우 -1 
//				{
//					nFuncRet = CTLBD_RET_GOOD;
//				}
//				else
//				{
//					sprintf(cJamcode, "%02d0009", n_axis);
//					CTL_Lib.Alarm_Error_Occurrence(5061, CTL_dWARNING, cJamcode);
//					nFuncRet = CTLBD_RET_ERROR;
//				}
//			}
			nPos1 = GetMotCurrentSite(M_VISION1_Y);
			nPos2 = GetMotCurrentSite(M_VISION2_Y);
			nRet = cmmSxIsDone(M_VISION2_Y, &dwMotionDone);			// Motion Done이냐?
			if(nPos2 == -1 || dwMotionDone != cmTRUE)
			{
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					CString strLog;
					strLog.Format("[CheckVisionPos1_2] SxIsDone: M_VISION2_Y =  %d , MotionDone = %d", nPos2,dwMotionDone);
					Func.On_LogFile_Add(LOG_HISTORY, strLog);
				}
				nFuncRet = CTLBD_RET_PROCEED;

			}
			else if(nPos1 != -1 && nPos1 != nPos2)
			{
				nFuncRet = CTLBD_RET_GOOD;
			}
			else
			{
				sprintf(COMI.mc_alarmcode, "%02d0009", n_axis);
				CTL_Lib.Alarm_Error_Occurrence(5060, CTL_dWARNING, COMI.mc_alarmcode);
				nFuncRet = CTLBD_RET_ERROR;
			}


		}
		else if(d_targetpos <= st_motor[M_VISION1_Z].md_pos[VISION_Y_ULDRBT_POS] + st_motor[M_VISION1_Z].mn_allow &&
		        d_targetpos >= st_motor[M_VISION1_Z].md_pos[VISION_Y_ULDRBT_POS] - st_motor[M_VISION1_Z].mn_allow)//uld pos 위치
		{
//			d_CompareVal1 =  (double)fabs(d_targetpos - d_CurPos[1]);
			//d_CompareVal2 = fMainMax(M_VISION1_Z, 0);
//			d_CompareVal2 = st_motor[M_VISION1_Z].md_pos[VISION_Y_ULDRBT_POS] - st_motor[M_VISION1_Z].md_pos[VISION_Z_DOWN_READY];
			
//			if(d_CompareVal1 >= d_CompareVal2 - st_motor[n_axis].mn_allow)
//			{
//			}
//			else
//			{//360009 0 36 "Vision1 Z 로봇 목표위치가 안전위치에 있지 않음."
//				nPos1 = GetMotCurrentSite(M_VISION1_Y);
//				nPos2 = GetMotCurrentSite(M_VISION2_Y);
//				if(nPos1 != -1 && nPos1 != nPos2) //nPos2가 동작중일 경우 -1 
//				{
//					nFuncRet = CTLBD_RET_GOOD;
//				}
//				else
//				{
//					sprintf(cJamcode, "%02d0009", n_axis);
//					CTL_Lib.Alarm_Error_Occurrence(5062, CTL_dWARNING, cJamcode);
//					nFuncRet = CTLBD_RET_ERROR;
//					break;
//				}
//			}
			nPos1 = GetMotCurrentSite(M_VISION1_Y);
			nPos2 = GetMotCurrentSite(M_VISION2_Y);
			nRet = cmmSxIsDone(M_VISION2_Y, &dwMotionDone);			// Motion Done이냐?
			if(nPos2 == -1 || dwMotionDone != cmTRUE)
			{
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					CString strLog;
					strLog.Format("[CheckVisionPos1_3] SxIsDone: M_VISION2_Y =  %d , MotionDone = %d", nPos2,dwMotionDone);
					Func.On_LogFile_Add(LOG_HISTORY, strLog);
				}
				nFuncRet = CTLBD_RET_PROCEED;

			}
			else if(nPos1 != -1 && nPos1 != nPos2)
			{
				nFuncRet = CTLBD_RET_GOOD;
			}
			else
			{
				sprintf(COMI.mc_alarmcode, "%02d0009", n_axis);
				CTL_Lib.Alarm_Error_Occurrence(5060, CTL_dWARNING, COMI.mc_alarmcode);
				nFuncRet = CTLBD_RET_ERROR;
			}
		}
		else
		{
			nFuncRet = CTLBD_RET_GOOD;
		}
		////
		break;

	case M_VISION2_Z:
		////2014,1124
		if(d_targetpos <= st_motor[M_VISION2_Z].md_pos[VISION_Z_LDMODULE] + st_motor[M_VISION2_Z].mn_allow &&
			d_targetpos >= st_motor[M_VISION2_Z].md_pos[VISION_Z_LDMODULE] - st_motor[M_VISION2_Z].mn_allow)//load module 위치
		{
//			d_CompareVal1 =  (double)fabs(d_targetpos - d_CurPos[1]);
//			d_CompareVal2 = st_motor[M_VISION2_Z].md_pos[VISION_Z_LDMODULE] - st_motor[M_VISION2_Z].md_pos[VISION_Z_DOWN_READY];
//			if(d_CompareVal1 >= d_CompareVal2 - st_motor[n_axis].mn_allow)
//			{
//			}
//			else
//			{//360009 0 36 "Vision1 Z 로봇 목표위치가 안전위치에 있지 않음."
//				nPos1 = GetMotCurrentSite(M_VISION1_Y);
//				nPos2 = GetMotCurrentSite(M_VISION2_Y);
//				if(nPos1 != -1 && nPos1 != nPos2) //nPos2가 동작중일 경우 -1 
//				{
//					nFuncRet = CTLBD_RET_GOOD;
//				}
//				else
//				{
//					sprintf(cJamcode, "%02d0009", n_axis);
//					CTL_Lib.Alarm_Error_Occurrence(5063, CTL_dWARNING, cJamcode);
//					nFuncRet = CTLBD_RET_ERROR;
//				}
//			}

			nPos1 = GetMotCurrentSite(M_VISION1_Y);
			nPos2 = GetMotCurrentSite(M_VISION2_Y);
			nRet = cmmSxIsDone(M_VISION1_Y, &dwMotionDone);			// Motion Done이냐?
			if(nPos1 == -1 || dwMotionDone != cmTRUE)
			{
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					CString strLog;
					strLog.Format("[CheckVisionPos2_1] SxIsDone: M_VISION1_Y =  %d , MotionDone = %d", nPos1,dwMotionDone);
					Func.On_LogFile_Add(LOG_HISTORY, strLog);
				}
				nFuncRet = CTLBD_RET_PROCEED;

			}
			else if(nPos1 != -1 && nPos1 != nPos2) //nPos2가 동작중일 경우 -1 
			{
				nFuncRet = CTLBD_RET_GOOD;
			}
			else
			{
				sprintf(COMI.mc_alarmcode, "%02d0009", n_axis);
				CTL_Lib.Alarm_Error_Occurrence(5063, CTL_dWARNING, COMI.mc_alarmcode);
				nFuncRet = CTLBD_RET_ERROR;
			}

		}
		else if(d_targetpos <= st_motor[M_VISION2_Z].md_pos[VISION_Z_LABEL] + st_motor[M_VISION2_Z].mn_allow &&
			d_targetpos >= st_motor[M_VISION2_Z].md_pos[VISION_Z_LABEL] - st_motor[M_VISION2_Z].mn_allow)//label 위치
		{
//			d_CompareVal1 =  (double)fabs(d_targetpos - d_CurPos[1]);
//			d_CompareVal2 = st_motor[M_VISION2_Z].md_pos[VISION_Z_LABEL] - st_motor[M_VISION2_Z].md_pos[VISION_Z_DOWN_READY];
//			if(d_CompareVal1 >= d_CompareVal2 - st_motor[n_axis].mn_allow)
//			{
//			}
//			else
//			{//360009 0 36 "Vision1 Z 로봇 목표위치가 안전위치에 있지 않음."
//				nPos1 = GetMotCurrentSite(M_VISION1_Y);
//				nPos2 = GetMotCurrentSite(M_VISION2_Y);
//				if(nPos1 != -1 && nPos1 != nPos2) //nPos2가 동작중일 경우 -1 
//				{
//					nFuncRet = CTLBD_RET_GOOD;
//				}
//				else
//				{
//					sprintf(cJamcode, "%02d0009", n_axis);
//					CTL_Lib.Alarm_Error_Occurrence(5064, CTL_dWARNING, cJamcode);
//					nFuncRet = CTLBD_RET_ERROR;
//				}
//			}
			nPos1 = GetMotCurrentSite(M_VISION1_Y);
			nPos2 = GetMotCurrentSite(M_VISION2_Y);
			nRet = cmmSxIsDone(M_VISION1_Y, &dwMotionDone);			// Motion Done이냐?
			if(nPos1 == -1 || dwMotionDone != cmTRUE)
			{
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					CString strLog;
					strLog.Format("[CheckVisionPos2_2] SxIsDone: M_VISION1_Y =  %d , MotionDone = %d", nPos1,dwMotionDone);
					Func.On_LogFile_Add(LOG_HISTORY, strLog);
				}
				nFuncRet = CTLBD_RET_PROCEED;

			}
			else if(nPos1 != -1 && nPos1 != nPos2) //nPos2가 동작중일 경우 -1 
			{
				nFuncRet = CTLBD_RET_GOOD;
			}
			else
			{
				sprintf(COMI.mc_alarmcode, "%02d0009", n_axis);
				CTL_Lib.Alarm_Error_Occurrence(5063, CTL_dWARNING, COMI.mc_alarmcode);
				nFuncRet = CTLBD_RET_ERROR;
			}
		}
		else if(d_targetpos <= st_motor[M_VISION2_Z].md_pos[VISION_Y_ULDRBT_POS] + st_motor[M_VISION2_Z].mn_allow &&
			d_targetpos >= st_motor[M_VISION2_Z].md_pos[VISION_Y_ULDRBT_POS] - st_motor[M_VISION2_Z].mn_allow)//uld pos 위치
		{
//			d_CompareVal1 =  (double)fabs(d_targetpos - d_CurPos[1]);
			//d_CompareVal2 = fMainMax(M_VISION1_Z, 0);
//			d_CompareVal2 = st_motor[M_VISION2_Z].md_pos[VISION_Y_ULDRBT_POS] - st_motor[M_VISION1_Z].md_pos[VISION_Z_DOWN_READY];
			
//			if(d_CompareVal1 >= d_CompareVal2 - st_motor[n_axis].mn_allow)
//			{
//			}
//			else
//			{//360009 0 36 "Vision1 Z 로봇 목표위치가 안전위치에 있지 않음."
//				nPos1 = GetMotCurrentSite(M_VISION1_Y);
//				nPos2 = GetMotCurrentSite(M_VISION2_Y);
//				if(nPos1 != -1 && nPos1 != nPos2) //nPos2가 동작중일 경우 -1 
//				{
//					nFuncRet = CTLBD_RET_GOOD;
//				}
//				else
//				{
//					sprintf(cJamcode, "%02d0009", n_axis);
//					CTL_Lib.Alarm_Error_Occurrence(5065, CTL_dWARNING, cJamcode);
//					nFuncRet = CTLBD_RET_ERROR;
//					break;
//				}
//			}
			nPos1 = GetMotCurrentSite(M_VISION1_Y);
			nPos2 = GetMotCurrentSite(M_VISION2_Y);
			nRet = cmmSxIsDone(M_VISION1_Y, &dwMotionDone);			// Motion Done이냐?
			if(nPos1 == -1 || dwMotionDone != cmTRUE)
			{
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					CString strLog;
					strLog.Format("[CheckVisionPos2_3] SxIsDone: M_VISION1_Y =  %d , MotionDone = %d", nPos1,dwMotionDone);
					Func.On_LogFile_Add(LOG_HISTORY, strLog);
				}
				nFuncRet = CTLBD_RET_PROCEED;

			}
			else if(nPos1 != -1 && nPos1 != nPos2) //nPos2가 동작중일 경우 -1 
			{
				nFuncRet = CTLBD_RET_GOOD;
			}
			else
			{
				sprintf(COMI.mc_alarmcode, "%02d0009", n_axis);
				CTL_Lib.Alarm_Error_Occurrence(5063, CTL_dWARNING, COMI.mc_alarmcode);
				nFuncRet = CTLBD_RET_ERROR;
			}
		}
		else
		{
			nFuncRet = CTLBD_RET_GOOD;
		}

		break;


	case M_VISION1_Y:
		nPos1 = GetMotCurrentSite(M_VISION1_Y);
		nPos2 = GetMotCurrentSite(M_VISION2_Y);
		nPos3 = GetMotCurrentSite(M_VISION1_Y, d_targetpos);
		
		d_CompareVal1 =  (double)fabs(d_CurPos[0] - d_CurPos[1]);
		d_CompareVal2 = st_motor[M_VISION1_Z].md_pos[VISION_Z_VISION] - st_motor[M_VISION2_Z].md_pos[VISION_Z_DOWN_READY];
		d_CompareVal3 = st_motor[M_VISION2_Z].md_pos[VISION_Z_VISION] - st_motor[M_VISION1_Z].md_pos[VISION_Z_DOWN_READY];

		if(d_CompareVal2 > d_CompareVal3)
		{
			d_CompareVal = d_CompareVal3;
		}
		else
		{
			d_CompareVal = d_CompareVal2;
		}
		if(d_CompareVal1 >= d_CompareVal - (3*COMI.mn_allow_value[M_VISION1_Z]))
		{
			nFuncRet = CTLBD_RET_GOOD;
		}
		else
		{
			nRet = cmmSxIsDone(M_VISION2_Y, &dwMotionDone);			// Motion Done이냐?
			if(nPos2 == -1 || dwMotionDone != cmTRUE)
			{
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					CString strLog;
					strLog.Format("[CheckVisionPos2_3] SxIsDone: M_VISION1_Y =  %d , MotionDone = %d", nPos1,dwMotionDone);
					Func.On_LogFile_Add(LOG_HISTORY, strLog);
				}
				nFuncRet = CTLBD_RET_PROCEED;

			}
			else if((st_work.mn_run_status == dINIT || st_work.mn_run_status == dLOTREADY) ||
				(nPos1 != -1 && nPos1 != nPos2 && nPos1 > nPos2) ) //nPos2가 동작중일 경우 -1 
			{
				nFuncRet = CTLBD_RET_GOOD;
			}
			//2015.0126
			if(nPos2 == nPos3 && 
				(st_work.mn_run_status != dINIT && st_work.mn_run_status != dLOTREADY)  )
			{//350009 0 35 "Vision1 Y 로봇 목표위치가 안전위치에 있지 않음."
				sprintf(COMI.mc_alarmcode, "%02d0009", n_axis);
				CTL_Lib.Alarm_Error_Occurrence(5066, CTL_dWARNING, COMI.mc_alarmcode);
				nFuncRet = CTLBD_RET_ERROR;
			}
		}
		break;
	
	case M_VISION2_Y:
		nPos1 = GetMotCurrentSite(M_VISION2_Y);
		nPos2 = GetMotCurrentSite(M_VISION1_Y);
		nPos3 = GetMotCurrentSite(M_VISION2_Y, d_targetpos);
		
		d_CompareVal1 =  (double)fabs(d_CurPos[1] - d_CurPos[0]);
		d_CompareVal2 = st_motor[M_VISION2_Z].md_pos[VISION_Z_VISION] - st_motor[M_VISION1_Z].md_pos[VISION_Z_DOWN_READY];
		d_CompareVal3 = st_motor[M_VISION1_Z].md_pos[VISION_Z_VISION] - st_motor[M_VISION2_Z].md_pos[VISION_Z_DOWN_READY];

		if(d_CompareVal2 > d_CompareVal3)
		{
			d_CompareVal = d_CompareVal3;
		}
		else
		{
			d_CompareVal = d_CompareVal2;
		}

		if(d_CompareVal1 >= d_CompareVal - (3*COMI.mn_allow_value[M_VISION2_Z]))
		{
			nFuncRet = CTLBD_RET_GOOD;
		}
		else
		{
			nRet = cmmSxIsDone(M_VISION1_Y, &dwMotionDone);			// Motion Done이냐?
			if(nPos2 == -1 || dwMotionDone != cmTRUE)
			{
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					CString strLog;
					strLog.Format("[CheckVisionPos2_3] SxIsDone: M_VISION1_Y =  %d , MotionDone = %d", nPos1,dwMotionDone);
					Func.On_LogFile_Add(LOG_HISTORY, strLog);
				}
				nFuncRet = CTLBD_RET_PROCEED;

			}
			else if((st_work.mn_run_status == dINIT || st_work.mn_run_status == dLOTREADY) ||
				(nPos1 != -1 && nPos1 != nPos2 && nPos1 > nPos2)) //nPos2가 동작중일 경우 -1 
			{
				nFuncRet = CTLBD_RET_GOOD;
			}
			//2015.0126
			if(nPos2 == nPos3 &&
				(st_work.mn_run_status != dINIT && st_work.mn_run_status != dLOTREADY) )
			{//370009 0 37 "Vision2 Y 로봇 목표위치가 안전위치에 있지 않음."
				sprintf(COMI.mc_alarmcode, "%02d0009", n_axis);
				CTL_Lib.Alarm_Error_Occurrence(5067, CTL_dWARNING, COMI.mc_alarmcode);
				nFuncRet = CTLBD_RET_ERROR;
			}
		}
		//2014.0418
		// d_CurPos[0] = COMI.Get_MotCurrentPos(M_VISION1_Z);
		// d_CurPos[1] = COMI.Get_MotCurrentPos(M_VISION2_Z);
		// d_CurPos[4] = COMI.Get_MotCurrentPos(M_LABEL_ATTACH_Z);
// 		if(d_CurPos[4] > st_motor[M_LABEL_ATTACH_Z].md_pos[Z_LABEL_PICK_POS]  + st_motor[M_LABEL_ATTACH_Z].mn_allow )
// 		{
// 			if(d_CurPos[1] <= st_motor[M_VISION2_Z].md_pos[Z_VISION_LABEL] + st_motor[M_VISION2_Z].mn_allow &&
// 				d_CurPos[1] >= st_motor[M_VISION2_Z].md_pos[Z_VISION_LABEL] - st_motor[M_VISION2_Z].mn_allow)
// 			{
// 				{
// 					sprintf(cJamcode, "%02d0009", n_axis);
// 					CTL_Lib.Alarm_Error_Occurrence(5062, CTL_dWARNING, cJamcode);
// 					nFuncRet = CTLBD_RET_ERROR;
// 				}	
// 			}
// 		}
		//
		break;

	default:
		nFuncRet = CTLBD_RET_ERROR;
		break;
	}

	return nFuncRet;
}

///////////////////////// 20120826 2015.0126 FPH 추가
int CPublic_Function::OnLabelPrint_Parsing(CString strCheckData, int i)
{

	//3.22 Keyence
	int     nLength;  // 데이터 총 길이 저장 변수 

	CString strBody;  // BODY 데이터 저장 변수 
	
	char  cp_cmd[1024];
	
	int     j;
	int     nStartIndex, nEndIndex;  // BCR 데이터의 시작 및 종료 위치 저장 변수 
	int     nStartIndex1, nEndIndex1;  // BCR 데이터의 시작 및 종료 위치 저장 변수 

	int     nReceivedDataCount=0;
	CString strTempDataCount;  // 데이터 갯수 저장 변수 
	CString strResultData[100];
	CString strResultData1[100];
	
	CString str_Tmp;
	CString str_Tmp_Data;
	
	CString str_LabelStartCode;

	memset(&cp_cmd, 0, sizeof(cp_cmd));  // 데이터 저장 변수 초기화 
	
	// **************************************************************************
	// 수신 데이터 최소 길이 검사한다 [최소 3자리 이상]                          
	// **************************************************************************	
	nLength = strCheckData.GetLength();
	if(nLength < 3)
	{
		return FALSE;
	}
	// **************************************************************************
	// 에러 메시지 수신 유무 검사한다                                            
	// **************************************************************************
	if(strCheckData.Find("ERROR") >= 0)
	{
		sprintf(st_msg.c_normal_msg,"Label Print ERROR!!!");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
		return FALSE;
	}
	// **************************************************************************
	
	//20140213
	str_LabelStartCode = strCheckData.Mid(0,3);
	if(str_LabelStartCode != "^XA")
	{
		return FALSE;
	}
	//

	sprintf(cp_cmd, "%s", strCheckData);
	nLength = strCheckData.GetLength();
		
	nReceivedDataCount = 0;
	nStartIndex = 0;
	nStartIndex1 = 0;

	// ==============================================================================
	// 실재로 읽은 BCR 데이터 전역 변수에 설정한다
	// ==============================================================================
		for(j = 0 ; j < nLength ; j++)
		{

			nEndIndex   = strCheckData.Find("^", nStartIndex);  // BCR 데이터 종료 위치 설정 
			
			if (nEndIndex == -1)
			{
				if (nLength - nStartIndex > 0)
				{
					nEndIndex = nLength - 1;
				}
				else
				{
					break;
				}
			}
			strResultData[j] = "";
			strResultData[j] = strCheckData.Mid(nStartIndex, nEndIndex - nStartIndex);  // BCR 데이터 문자열 파싱 
			nReceivedDataCount++;
			nStartIndex = nEndIndex + 1;
			
//////////////////// 20120906
// 			if (j == 3 || j == 4) //Unkown1 위치 및 Unkown1 내용 
// 			{
// 				if (j == 3) st_NW.mstr_Unkown1[i] = "^" + strResultData[j];
// 				else if (j == 4) st_NW.mstr_Unkown1[i] += "^" + strResultData[j];	//^PON^FS
// 			}			
// 			else if (j == 5 || j == 6 || j == 7 || j == 8 || j == 9) //1번 위치 및 1번 내용
// 			{
// 				if (j == 5) //^FO387.5,12.5
// 				{
// 					st_NW.mstr_1Position[i] = "^" + strResultData[j]; 
// 									
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 					st_NW.mstr_1PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  
// 					st_NW.mstr_1PosX_1[i] = st_NW.mstr_1PosX_1[i].Mid(2,100);	// FO X
// 					
// 					nStartIndex1 = nEndIndex1 + 1;
// 					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 					st_NW.mstr_1PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;
// 				}
// 				else if (j == 6) //A0,57.5,20
// 				{
// 					st_NW.mstr_1Position[i] += "^" + strResultData[j];
// 
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
// 					
//  					nStartIndex1 = nEndIndex1 + 1;
//  					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 					st_NW.mstr_1Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
// 				
// 					nStartIndex1 = nEndIndex1 + 1;
// 					st_NW.mstr_1High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;
// 
// 				}
// 				else if (j == 7) //FPH,2 
// 				{
// 					st_NW.mstr_FPH_1[i] = "^" + strResultData[j];
// 				}
// 				else if (j == 8) //^FD32GB 4Rx4 PC3L-12800L-11-12-C0
// 				{
// 					st_NW.mstr_1Contents[i] = "^" + strResultData[j];
// 					st_NW.mstr_1Contents_1[i] = strResultData[j].Mid(2,100);
// 				}
// 				else if (j == 9) st_NW.mstr_1Contents[i] += "^" + strResultData[j];
// 			}
// 			else if (j == 10 || j == 11 || j == 12 || j == 13) //2번 위치 및 2번 내용
// 			{
// 				if (j == 10) //^FO50,87.5
// 				{
// 					st_NW.mstr_2Position[i] = "^" + strResultData[j];
// 
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 					st_NW.mstr_2PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
// 					st_NW.mstr_2PosX_1[i] = st_NW.mstr_2PosX_1[i].Mid(2,100);	// FO X
// 					
// 					nStartIndex1 = nEndIndex1 + 1;
// 					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 					st_NW.mstr_2PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;
// 				}
// 				else if (j == 11) //A0,37.5,37.5
// 				{
// 					st_NW.mstr_2Position[i] += "^" + strResultData[j];
// 
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
// 					
//  					nStartIndex1 = nEndIndex1 + 1;
//  					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 					st_NW.mstr_2Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width 
// 				
// 					nStartIndex1 = nEndIndex1 + 1;
// 					st_NW.mstr_2High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;					
// 				}
// 				else if (j == 12)//MADE IN KOREA  FDMade in China^FS
// 				{
// 					st_NW.mstr_2Contents[i] = "^" + strResultData[j];
// 					st_NW.mstr_2Contents_1[i] = strResultData[j].Mid(2,100);					
// 				}
// 				else if (j == 13) st_NW.mstr_2Contents[i] += "^" + strResultData[j];
// 			}
// 			else if (j == 14 || j == 15 || j == 16 || j == 17 || j == 18) //3번 위치 및 3번 내용
// 			{
// 				if (j == 14) //^FO285,80
// 				{
// 					st_NW.mstr_3Position[i] = "^" + strResultData[j];
// 					
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 					st_NW.mstr_3PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  
// 					st_NW.mstr_3PosX_1[i] = st_NW.mstr_3PosX_1[i].Mid(2,100);	// FO X
// 					
// 					nStartIndex1 = nEndIndex1 + 1;
// 					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 					st_NW.mstr_3PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;				
// 				
// 				}
// 				else if (j == 15) //^A0,50,40
// 				{
// 					st_NW.mstr_3Position[i] += "^" + strResultData[j];
// 				
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
// 					
//  					nStartIndex1 = nEndIndex1 + 1;
//  					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 					st_NW.mstr_3Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
// 				
// 					nStartIndex1 = nEndIndex1 + 1;
// 					st_NW.mstr_3High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;					
// 				
// 				}
// 				else if (j == 16) //FPH,2 
// 				{
// 					st_NW.mstr_FPH[i] = "^" + strResultData[j];
// 				}
// 				else if (j == 17) //PARTNO FDM386B4G70DM0-YK03^FS  ppp
// 				{
// 					st_NW.mstr_3Contents[i] = "^" + strResultData[j];
// 					st_NW.mstr_3Contents_1[i] = strResultData[j].Mid(2, 100);
// 					st_NW.mstr_Recive_PartNo[i] = st_NW.mstr_3Contents_1[i];//2012,0911
// 				}
// 				else if (j == 18) st_NW.mstr_3Contents[i] += "^" + strResultData[j];
// 			}
// 			else if (j == 22 || j == 19 || j == 20 || j == 21) //3번 뒤 위치 및 3번 뒤 내용
// 			{
// 				if (j == 19) //FO730,75
// 				{
// 					st_NW.mstr_3Position_back[i] = "^" + strResultData[j];
// 
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 					st_NW.mstr_3PosX_back_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
// 					st_NW.mstr_3PosX_back_1[i] = st_NW.mstr_3PosX_back_1[i].Mid(2,100);	// FO X
// 					
// 					nStartIndex1 = nEndIndex1 + 1;
// 					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 					st_NW.mstr_3PosY_back_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;				
// 									
// 				}
// 				else if (j == 20) 
// 				{
// 					st_NW.mstr_3Position_back[i] += "^" + strResultData[j];
// 
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
// 					
//  					nStartIndex1 = nEndIndex1 + 1;
//  					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 					st_NW.mstr_3Wid_back_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
// 				
// 					nStartIndex1 = nEndIndex1 + 1;
// 					st_NW.mstr_3High_back_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;				
// 				}
// 				else if (j == 21) 
// 				{
// 					st_NW.mstr_3Contents_back[i] = "^" + strResultData[j];
// 					st_NW.mstr_3Contents_back_1[i] = strResultData[j].Mid(2,100);
// 				
// 					int m_d_data = 	atoi(st_NW.mstr_3Contents_back_1[i]);
// 					st_NW.mstr_3Contents_back_1[i] = atoi(st_NW.mstr_3Contents_back_1[i]) + 1;
// 					st_NW.mstr_3Contents_back_1[i] = strResultData[j].Mid(2,100);
// 				}
// 				else if (j == 22) st_NW.mstr_3Contents_back[i] += "^" + strResultData[j];
// 			}
// 			else if (j == 27 || j == 23 || j == 24 || j == 25 || j == 26) //4번 위치 및 4번 내용(바코드)
// 			{
// 				if (j == 23) 
// 				{
// 					st_NW.mstr_4Position[i] = "^" + strResultData[j];
// 
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 					st_NW.mstr_4PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
// 					st_NW.mstr_4PosX_1[i] = st_NW.mstr_4PosX_1[i].Mid(2,100);// FO X
// 					
// 					nStartIndex1 = nEndIndex1 + 1;
// 					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 					st_NW.mstr_4PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;				
// 				}
// 				else if (j == 24) 
// 				{
// 					st_NW.mstr_4Position[i] += "^" + strResultData[j];
// 				
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // BY3
// 					st_NW.mstr_4Etc_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  
// 
//  					nStartIndex1 = nEndIndex1 + 1;
//  					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 					st_NW.mstr_4Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  
// 				
// 					nStartIndex1 = nEndIndex1 + 1;
//  					nEndIndex1   = strResultData[j].Find("", nStartIndex1);  
// 					st_NW.mstr_4High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;				
// 				}
// 				else if (j == 25) 
// 				{
// 					st_NW.mstr_4Position[i] += "^" + strResultData[j];
// 					st_NW.mstr_4Contents_1[i] = strResultData[j];  // BC,N,N,0,N,N  					
// 				}
// 				else if (j == 26)
// 				{
// 					st_NW.mstr_4Contents[i] = "^" + strResultData[j];
// 					st_NW.mstr_4Contents_2[i] = strResultData[j].Mid(2,100);
// 				}
// 				else if (j == 27) st_NW.mstr_4Contents[i] += "^" + strResultData[j];
// 			}
// 			else if (j == 31 || j == 28 || j == 29 || j == 30) //2D 위치 및 2D 내용
// 			{
// 				if (j == 28) 
// 				{
// 					st_NW.mstr_2DPosition[i] = "^" + strResultData[j];
// 
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 					st_NW.mstr_2DPosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
// 					st_NW.mstr_2DPosX_1[i] = st_NW.mstr_2DPosX_1[i].Mid(2,100);	// FO X
// 					
// 					nStartIndex1 = nEndIndex1 + 1;
// 					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 					st_NW.mstr_2DPosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;				
// 
// 
// 				}
// 				else if (j == 29) 
// 				{
// 					st_NW.mstr_2DPosition[i] += "^" + strResultData[j];
// 
// 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // BXN
// 					st_NW.mstr_2DEtc_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  
// 
//  					nStartIndex1 = nEndIndex1 + 1;
//  					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 					st_NW.mstr_2DWid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  
// 				
//  					nStartIndex1 = nEndIndex1 + 1;
//  					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 					st_NW.mstr_2DHigh_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // High  
// 
// 					nStartIndex1 = nEndIndex1 + 1;
// 					st_NW.mstr_2DEtc_2[i] = strResultData[j].Mid(nStartIndex1, 100);  // etc
// 					
// 					nEndIndex1 = 0;
// 					nStartIndex1 = 0;				
// 
// 				}
// 				else if (j == 30) 
// 				{
// 					st_NW.mstr_2DContents[i] = "^" + strResultData[j];
// 					st_NW.mstr_2DContents_1[i] = strResultData[j];
// 				}
// 				else if (j == 31) st_NW.mstr_2DContents[i] += "^" + strResultData[j];
// 			}
// 			else if (j == 35 || j == 32 || j == 33 || j == 34) //Unkown2 위치 및 Unkown2 내용
// 			{
// 				if (j == 32) //M393B4G70BM0-YF8Q8
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_Unkown2[i] = "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 						st_NW.mstr_Unkown2PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
// 						st_NW.mstr_Unkown2PosX_1[i] = st_NW.mstr_Unkown2PosX_1[i].Mid(2,100);	// FO X
// 						
// 						nStartIndex1 = nEndIndex1 + 1;
// 						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 						st_NW.mstr_Unkown2PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;			
// 					}
// 					else
// 					{
// 						st_NW.mstr_LotNoPosition[i] = "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 						st_NW.mstr_LotNoPosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
// 						st_NW.mstr_LotNoPosX_1[i] = st_NW.mstr_LotNoPosX_1[i].Mid(2,100);	// FO X
// 						
// 						nStartIndex1 = nEndIndex1 + 1;
// 						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 						st_NW.mstr_LotNoPosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;				
// 					}
// 				}
// 				else if (j == 33) 
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
// 						
//  						nStartIndex1 = nEndIndex1 + 1;
//  						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 						st_NW.mstr_Unkown2Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
// 					
// 						nStartIndex1 = nEndIndex1 + 1;
// 						st_NW.mstr_Unkown2High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;				
// 					}
// 					else
// 					{
// 						st_NW.mstr_LotNoPosition[i] += "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0R
// 						
//  						nStartIndex1 = nEndIndex1 + 1;
//  						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 						st_NW.mstr_LotNoWid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
// 					
// 						nStartIndex1 = nEndIndex1 + 1;
// 						st_NW.mstr_LotNoHigh_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;			
// 					}
// 				}
// 				else if (j == 34) 
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
// 						st_NW.mstr_Unkown2_1[i] = strResultData[j].Mid(2,100);
// 					}
// 					else
// 					{
// 						st_NW.mstr_LotNoContents[i] = "^" + strResultData[j];
// 						st_NW.mstr_LotNoContents_1[i] = strResultData[j].Mid(2, 100);
// 
// 
// 					}
// 				}
// 				else if (j == 35) 
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
// 					}
// 					else
// 					{
// 						st_NW.mstr_LotNoContents[i] += "^" + strResultData[j];
// 					}
// 
// 				}
// 			}
// 			else if (j == 39 || j == 36 || j == 37 || j == 38) //Lot No 위치 및 Lot No 내용
// 			{
// 				if (j == 36) 
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_LotNoPosition[i] = "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 						st_NW.mstr_LotNoPosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
// 						st_NW.mstr_LotNoPosX_1[i] = st_NW.mstr_LotNoPosX_1[i].Mid(2,100);	// FO X
// 						
// 						nStartIndex1 = nEndIndex1 + 1;
// 						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 						st_NW.mstr_LotNoPosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;				
// 					}
// 					else
// 					{
// 						st_NW.mstr_Unkown2[i] = "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 						st_NW.mstr_Unkown2PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
// 						st_NW.mstr_Unkown2PosX_1[i] = st_NW.mstr_Unkown2PosX_1[i].Mid(2,100);	// FO X
// 						
// 						nStartIndex1 = nEndIndex1 + 1;
// 						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 						st_NW.mstr_Unkown2PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;			
// 					}			
// 				}
// 				else if (j == 37) 
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_LotNoPosition[i] += "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0R
// 						
//  						nStartIndex1 = nEndIndex1 + 1;
//  						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 						st_NW.mstr_LotNoWid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
// 					
// 						nStartIndex1 = nEndIndex1 + 1;
// 						st_NW.mstr_LotNoHigh_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;			
// 					}
// 					else
// 					{
// 						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
// 						
//  						nStartIndex1 = nEndIndex1 + 1;
//  						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 						st_NW.mstr_Unkown2Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
// 					
// 						nStartIndex1 = nEndIndex1 + 1;
// 						st_NW.mstr_Unkown2High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;				
// 					}
// 				}
// 				else if (j == 38) 
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_LotNoContents[i] = "^" + strResultData[j];
// 						st_NW.mstr_LotNoContents_1[i] = strResultData[j].Mid(2, 100);
// 					}
// 					else
// 					{
// 						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
// 						st_NW.mstr_Unkown2_1[i] = strResultData[j].Mid(2,100);
// 					}
// 				}
// 				else if (j == 39) 
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_LotNoContents[i] += "^" + strResultData[j];
// 						st_NW.mstr_Print_Data[i] = st_NW.mstr_Unkown1[i]; 
// 						//2016.0511
// 						//st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_1Contents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_FPH_1[i] + st_NW.mstr_1Contents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_2Position[i] + st_NW.mstr_2Contents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position[i] + st_NW.mstr_FPH[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Contents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position_back[i] + st_NW.mstr_3Contents_back[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_4Position[i] + st_NW.mstr_4Contents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_2DPosition[i] + st_NW.mstr_2DContents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_Unkown2[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_LotNoPosition[i] + st_NW.mstr_LotNoContents[i];
// 					}
// 					else
// 					{
// 						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
// 						st_NW.mstr_Print_Data[i] = st_NW.mstr_Unkown1[i]; 
// 						//2016.0511
// 						//st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_1Contents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_FPH_1[i] + st_NW.mstr_1Contents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_2Position[i] + st_NW.mstr_2Contents[i]; 
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position[i] + st_NW.mstr_FPH[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Contents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position_back[i] + st_NW.mstr_3Contents_back[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_4Position[i] + st_NW.mstr_4Contents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_2DPosition[i] + st_NW.mstr_2DContents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_LotNoPosition[i] + st_NW.mstr_LotNoContents[i];
// 						st_NW.mstr_Print_Data[i] += st_NW.mstr_Unkown2[i];
// 					}	 
// 					Func.On_LogFile_Add(0, st_NW.mstr_Print_Data[i]);
// 					Func.On_LogFile_Add(99, st_NW.mstr_Print_Data[i]);
// 					break;
// 				}
// 			}



			if (j == 3 || j == 4) //Unkown1 위치 및 Unkown1 내용 
			{
				if (j == 3) st_NW.mstr_Unkown1[i] = "^" + strResultData[j];
				else if (j == 4) st_NW.mstr_Unkown1[i] += "^" + strResultData[j];	//^PON^FS
			}			
			else if (j == 5 || j == 6 || j == 7 || j == 8 || j == 9 || j == 10 || j == 11 || j == 12 || j == 13 || j == 14) //1번 위치 및 1번 내용
			{
				if (j == 5) //^FO387.5,12.5
				{
					st_NW.mstr_1Position[i] = "^" + strResultData[j]; 
					
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_1PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  
					st_NW.mstr_1PosX_1[i] = st_NW.mstr_1PosX_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_1PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;
				}
				else if (j == 6) //A0,57.5,20
				{
					st_NW.mstr_1Position[i] += "^" + strResultData[j];
					
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_1Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
					
					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_1High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;
					
				}
				else if (j == 7) //FPH,2 
				{
					st_NW.mstr_FPH_2[i] = "^" + strResultData[j];
				}
				else if (j == 8) //^FD32GB 4Rx4 PC3L-12800L-11-12-C0
				{
					st_NW.mstr_0Contents[i] = "^" + strResultData[j];
					st_NW.mstr_0Contents_1[i] = strResultData[j].Mid(2,100);
				}
				else if (j == 9) 
				{
					st_NW.mstr_0Contents[i] += "^" + strResultData[j];
				}
				else if (j == 10) //^FO50,87.5
				{
					st_NW.mstr_0Position[i] = "^" + strResultData[j];
					
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_0PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
					st_NW.mstr_0PosX_1[i] = st_NW.mstr_0PosX_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_0PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;
				}
				else if (j == 11) //A0,37.5,37.5
				{
					st_NW.mstr_0Position[i] += "^" + strResultData[j];
					
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_0Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width 
					
					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_0High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;					
				}
				else if (j == 12) //FPH,2 
				{
					st_NW.mstr_FPH_1[i] = "^" + strResultData[j];
				}
				else if (j == 13) //^FD32GB 4Rx4 PC3L-12800L-11-12-C0
				{
					st_NW.mstr_1Contents[i] = "^" + strResultData[j];
					st_NW.mstr_1Contents_1[i] = strResultData[j].Mid(2,100);
				}
				else if (j == 14) st_NW.mstr_1Contents[i] += "^" + strResultData[j];

			}
			else if (j == 15 || j == 16 || j == 17 || j == 18) //2번 위치 및 2번 내용
			{
				if (j == 15) //^FO50,87.5
				{
					st_NW.mstr_2Position[i] = "^" + strResultData[j];
					
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_2PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
					st_NW.mstr_2PosX_1[i] = st_NW.mstr_2PosX_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_2PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;
				}
				else if (j == 16) //A0,37.5,37.5
				{
					st_NW.mstr_2Position[i] += "^" + strResultData[j];
					
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_2Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width 
					
					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_2High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;					
				}
				else if (j == 17)//MADE IN KOREA  FDMade in China^FS
				{
					st_NW.mstr_2Contents[i] = "^" + strResultData[j];
					st_NW.mstr_2Contents_1[i] = strResultData[j].Mid(2,100);					
				}
				else if (j == 18) st_NW.mstr_2Contents[i] += "^" + strResultData[j];
			}
			else if (j == 19 || j == 20 || j == 21 || j == 22 || j == 23) //3번 위치 및 3번 내용
			{
				if (j == 19) //^FO285,80
				{
					st_NW.mstr_3Position[i] = "^" + strResultData[j];
					
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_3PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  
					st_NW.mstr_3PosX_1[i] = st_NW.mstr_3PosX_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_3PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				
				
				}
				else if (j == 20) //^A0,50,40
				{
					st_NW.mstr_3Position[i] += "^" + strResultData[j];
				
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
					
 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_3Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
				
					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_3High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;					
				
				}
				else if (j == 21) //FPH,2 
				{
					st_NW.mstr_FPH[i] = "^" + strResultData[j];
				}
				else if (j == 22) //PARTNO FDM386B4G70DM0-YK03^FS  ppp
				{
					st_NW.mstr_3Contents[i] = "^" + strResultData[j];
					st_NW.mstr_3Contents_1[i] = strResultData[j].Mid(2, 100);
					st_NW.mstr_Recive_PartNo[i] = st_NW.mstr_3Contents_1[i];//2012,0911
				}
				else if (j == 23) st_NW.mstr_3Contents[i] += "^" + strResultData[j];
			}
			else if (j == 24 || j == 25 || j == 26 || j == 27) //3번 뒤 위치 및 3번 뒤 내용
			{
				if (j == 24) //FO730,75
				{
					st_NW.mstr_3Position_back[i] = "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_3PosX_back_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
					st_NW.mstr_3PosX_back_1[i] = st_NW.mstr_3PosX_back_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_3PosY_back_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				
									
				}
				else if (j == 25) 
				{
					st_NW.mstr_3Position_back[i] += "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
					
 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
					st_NW.mstr_3Wid_back_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
				
					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_3High_back_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				
				}
				else if (j == 26) 
				{
					st_NW.mstr_3Contents_back[i] = "^" + strResultData[j];
					st_NW.mstr_3Contents_back_1[i] = strResultData[j].Mid(2,100);
				
					int m_d_data = 	atoi(st_NW.mstr_3Contents_back_1[i]);
					st_NW.mstr_3Contents_back_1[i] = atoi(st_NW.mstr_3Contents_back_1[i]) + 1;
					st_NW.mstr_3Contents_back_1[i] = strResultData[j].Mid(2,100);
				}
				else if (j == 27) st_NW.mstr_3Contents_back[i] += "^" + strResultData[j];
			}
			else if (j == 28 || j == 29 || j == 30 || j == 31 || j == 32) //4번 위치 및 4번 내용(바코드)
			{
				if (j == 28) 
				{
					st_NW.mstr_4Position[i] = "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_4PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
					st_NW.mstr_4PosX_1[i] = st_NW.mstr_4PosX_1[i].Mid(2,100);// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_4PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				
				}
				else if (j == 29) 
				{
					st_NW.mstr_4Position[i] += "^" + strResultData[j];
				
					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // BY3
					st_NW.mstr_4Etc_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  

 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_4Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  
				
					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find("", nStartIndex1);  
					st_NW.mstr_4High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				
				}
				else if (j == 30) 
				{
					st_NW.mstr_4Position[i] += "^" + strResultData[j];
					st_NW.mstr_4Contents_1[i] = strResultData[j];  // BC,N,N,0,N,N  					
				}
				else if (j == 31)
				{
					st_NW.mstr_4Contents[i] = "^" + strResultData[j];
					st_NW.mstr_4Contents_2[i] = strResultData[j].Mid(2,100);
				}
				else if (j == 32) st_NW.mstr_4Contents[i] += "^" + strResultData[j];
			}
			else if (j == 33 || j == 34 || j == 35 || j == 36) //2D 위치 및 2D 내용
			{
				if (j == 33) 
				{
					st_NW.mstr_2DPosition[i] = "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_2DPosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
					st_NW.mstr_2DPosX_1[i] = st_NW.mstr_2DPosX_1[i].Mid(2,100);	// FO X
					
					nStartIndex1 = nEndIndex1 + 1;
					nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
					st_NW.mstr_2DPosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				


				}
				else if (j == 34) 
				{
					st_NW.mstr_2DPosition[i] += "^" + strResultData[j];

					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // BXN
					st_NW.mstr_2DEtc_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  

 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_2DWid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width  
				
 					nStartIndex1 = nEndIndex1 + 1;
 					nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
					st_NW.mstr_2DHigh_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // High  

					nStartIndex1 = nEndIndex1 + 1;
					st_NW.mstr_2DEtc_2[i] = strResultData[j].Mid(nStartIndex1, 100);  // etc
					
					nEndIndex1 = 0;
					nStartIndex1 = 0;				

				}
				else if (j == 35) 
				{
					st_NW.mstr_2DContents[i] = "^" + strResultData[j];
					st_NW.mstr_2DContents_1[i] = strResultData[j];
				}
				else if (j == 36) st_NW.mstr_2DContents[i] += "^" + strResultData[j];
			}
			else if (j == 37 || j == 38 || j == 39 || j == 40) //Unkown2 위치 및 Unkown2 내용
			{
				if (j == 37) //M393B4G70BM0-YF8Q8
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_Unkown2[i] = "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
						st_NW.mstr_Unkown2PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
						st_NW.mstr_Unkown2PosX_1[i] = st_NW.mstr_Unkown2PosX_1[i].Mid(2,100);	// FO X
						
						nStartIndex1 = nEndIndex1 + 1;
						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
						st_NW.mstr_Unkown2PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;			
					}
					else
					{
						st_NW.mstr_LotNoPosition[i] = "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
						st_NW.mstr_LotNoPosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
						st_NW.mstr_LotNoPosX_1[i] = st_NW.mstr_LotNoPosX_1[i].Mid(2,100);	// FO X
						
						nStartIndex1 = nEndIndex1 + 1;
						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
						st_NW.mstr_LotNoPosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;				
					}
				}
				else if (j == 38) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
						
 						nStartIndex1 = nEndIndex1 + 1;
 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
						st_NW.mstr_Unkown2Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
					
						nStartIndex1 = nEndIndex1 + 1;
						st_NW.mstr_Unkown2High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;				
					}
					else
					{
						st_NW.mstr_LotNoPosition[i] += "^" + strResultData[j];

						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0R
						
 						nStartIndex1 = nEndIndex1 + 1;
 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
						st_NW.mstr_LotNoWid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
					
						nStartIndex1 = nEndIndex1 + 1;
						st_NW.mstr_LotNoHigh_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
						
						nEndIndex1 = 0;
						nStartIndex1 = 0;			
					}
				}
				else if (j == 39) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
						st_NW.mstr_Unkown2_1[i] = strResultData[j].Mid(2,100);
					}
					else
					{
						st_NW.mstr_LotNoContents[i] = "^" + strResultData[j];
						st_NW.mstr_LotNoContents_1[i] = strResultData[j].Mid(2, 100);


					}
				}
				else if (j == 40) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
					}
					else
					{
						st_NW.mstr_LotNoContents[i] += "^" + strResultData[j];
					}

				}
			}
			else if (j == 41 /*|| j == 42 || j == 43 || j == 44*/) //Lot No 위치 및 Lot No 내용
			{
// 				if (j == 41) 
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_LotNoPosition[i] = "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 						st_NW.mstr_LotNoPosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
// 						st_NW.mstr_LotNoPosX_1[i] = st_NW.mstr_LotNoPosX_1[i].Mid(2,100);	// FO X
// 						
// 						nStartIndex1 = nEndIndex1 + 1;
// 						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 						st_NW.mstr_LotNoPosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;				
// 					}
// 					else
// 					{
// 						st_NW.mstr_Unkown2[i] = "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  
// 						st_NW.mstr_Unkown2PosX_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);   
// 						st_NW.mstr_Unkown2PosX_1[i] = st_NW.mstr_Unkown2PosX_1[i].Mid(2,100);	// FO X
// 						
// 						nStartIndex1 = nEndIndex1 + 1;
// 						nEndIndex1   = strResultData[j].Find("", nStartIndex1);   
// 						st_NW.mstr_Unkown2PosY_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // Y 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;			
// 					}			
// 				}
// 				else if (j == 42) 
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_LotNoPosition[i] += "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0R
// 						
//  						nStartIndex1 = nEndIndex1 + 1;
//  						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 						st_NW.mstr_LotNoWid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
// 					
// 						nStartIndex1 = nEndIndex1 + 1;
// 						st_NW.mstr_LotNoHigh_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;			
// 					}
// 					else
// 					{
// 						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
// 
// 						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);  // A0
// 						
//  						nStartIndex1 = nEndIndex1 + 1;
//  						nEndIndex1   = strResultData[j].Find(",", nStartIndex1);   
// 						st_NW.mstr_Unkown2Wid_1[i] = strResultData[j].Mid(nStartIndex1, nEndIndex1 - nStartIndex1);  // width
// 					
// 						nStartIndex1 = nEndIndex1 + 1;
// 						st_NW.mstr_Unkown2High_1[i] = strResultData[j].Mid(nStartIndex1, 100);  // High 
// 						
// 						nEndIndex1 = 0;
// 						nStartIndex1 = 0;				
// 					}
// 				}
// 				else if (j == 43) 
// 				{
// 					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
// 					{
// 						st_NW.mstr_LotNoContents[i] = "^" + strResultData[j];
// 						st_NW.mstr_LotNoContents_1[i] = strResultData[j].Mid(2, 100);
// 					}
// 					else
// 					{
// 						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
// 						st_NW.mstr_Unkown2_1[i] = strResultData[j].Mid(2,100);
// 					}
// 				}
				if (j == 41) 
				{
					if(st_NW.mstr_Recive_PartNo[i].GetLength() > 16 && st_NW.mstr_Recive_PartNo[i].GetAt(16) == 'Q')
					{
						st_NW.mstr_LotNoContents[i] += "^" + strResultData[j];
						st_NW.mstr_Print_Data[i] = st_NW.mstr_Unkown1[i]; 
						//2016.0511
						//st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_1Contents[i];
						//2016.0615
						//st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_FPH_1[i] + st_NW.mstr_1Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_FPH_2[i] + st_NW.mstr_0Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_0Position[i] + st_NW.mstr_FPH_1[i] + st_NW.mstr_1Contents[i];

						st_NW.mstr_Print_Data[i] += st_NW.mstr_2Position[i] + st_NW.mstr_2Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position[i] + st_NW.mstr_FPH[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position_back[i] + st_NW.mstr_3Contents_back[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_4Position[i] + st_NW.mstr_4Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_2DPosition[i] + st_NW.mstr_2DContents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_Unkown2[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_LotNoPosition[i] + st_NW.mstr_LotNoContents[i];
					}
					else
					{
						st_NW.mstr_Unkown2[i] += "^" + strResultData[j];
						st_NW.mstr_Print_Data[i] = st_NW.mstr_Unkown1[i]; 
						//2016.0511
						//st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_1Contents[i];
						//2016.0615
						//st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_FPH_1[i] + st_NW.mstr_1Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_1Position[i] + st_NW.mstr_FPH_2[i] + st_NW.mstr_0Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_0Position[i] + st_NW.mstr_FPH_1[i] + st_NW.mstr_1Contents[i];

						st_NW.mstr_Print_Data[i] += st_NW.mstr_2Position[i] + st_NW.mstr_2Contents[i]; 
						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position[i] + st_NW.mstr_FPH[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_3Position_back[i] + st_NW.mstr_3Contents_back[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_4Position[i] + st_NW.mstr_4Contents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_2DPosition[i] + st_NW.mstr_2DContents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_LotNoPosition[i] + st_NW.mstr_LotNoContents[i];
						st_NW.mstr_Print_Data[i] += st_NW.mstr_Unkown2[i];
					}	 
					Func.On_LogFile_Add(0, st_NW.mstr_Print_Data[i]);
					Func.On_LogFile_Add(99, st_NW.mstr_Print_Data[i]);
					break;
				}
			}
		}
	return TRUE;

}


///////////////////////////////////////////////////////////////////////////
//2017.0428

int CPublic_Function::GetModelFromPartID( CString PartID, CString& strModel)
{
	int nFuncRet = RET_ERROR, nRet = 0, nMdl = 0;
	
	PartID.TrimLeft(' ');	
	PartID.TrimRight(' ');
	
	nRet = LoadRegModelData();
	if( nRet == RET_GOOD)
	{
		nItemLength = 0;
		nMdl = GetModelNumFromModel(st_work.m_strCurModel);
		nRet = ModelFileload(st_work.m_strCurModel);
		if( nMdl>=0 && nRet == RET_GOOD && nItemLength > 0) 
		{
			for (int ii = 0; ii < nItemLength; ii++)
			{
				m_strModel[nMdl][ii] = m_strItemValue[ii];
				if(m_strItemValue[ii].Compare( (LPCTSTR)PartID) == 0)
				{
					strModel = st_work.m_strCurModel;
					nFuncRet = RET_GOOD;
					if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
					{
						sprintf(st_msg.c_normal_msg, "%s Model is same", st_work.m_strCurModel);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
					}
					
					break;
				}
			}
		}
		
		if(nFuncRet != RET_GOOD)
		{
			//2017.0811
			//자동모델등록을 막는다.
// 			for (int i= 0; i < st_work.m_nMdlTotal; i++ )
// 			{
// 				nItemLength = 0;
// 				nRet = ModelFileload(st_work.m_strModelName[i]);
// 				if( nRet == RET_GOOD && nItemLength > 0) 
// 				{
// 					for (int ii = 0; ii < nItemLength; ii++)
// 					{
// 						m_strModel[i][ii] = m_strItemValue[ii];
// 						if(m_strItemValue[ii].Compare( (LPCTSTR)PartID) == 0)
// 						{
// 							strModel = st_work.m_strModelName[i];
// 							nFuncRet = RET_GOOD;
// 							if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 							{
// 								sprintf(st_msg.c_normal_msg, "%s Model is different", strModel);
// 								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
// 							}
// 							break;
// 						}
// 					}
// 				}
// 			}
		}	
	}
	return nFuncRet;
}

int CPublic_Function::ModelFileload(CString strMdl)
{
	int nFunRet = RET_ERROR;
	CString strFilePath;
	strFilePath = _T("C:\\AMT830\\Setting\\ModelName\\");
	strMdl.TrimLeft(); strMdl.TrimRight();
	CString strFile = strMdl + _T(".TXT");
	//FileRead(strFilePath + strFile);
	
	int nTotal = 0;
	FILE    *fp ;
	int     existence;
	char chr_data[100];	
	
	strFilePath += strFile;
	
	existence = access(strFilePath, 0);
	
	if (!existence)
	{
		if ((fp=fopen(strFilePath,"rb")) == NULL)
		{
			return nFunRet;
		}
	}
	else
	{
		// 		m_nMdlTotal = 0;
		return nFunRet;
	}
	
	for(int i = 0; i < 500; i++)  
	{
		m_strItemValue[i].Empty();
	}
	
	
	:: GetPrivateProfileString(strMdl, "Total", "0", chr_data, 100, strFilePath);
	nTotal = atoi(chr_data);
	
	nItemLength = nTotal;
	
	CString str,strLabel;
	for (i = 0; i < nTotal; i++)
	{
		str.Format("%d",i+1);
		:: GetPrivateProfileString(strMdl, str, "0", chr_data, 100, strFilePath);
		strLabel = chr_data;
		strLabel.TrimLeft(); strLabel.TrimRight();
		m_strItemValue[i] = strLabel;
	}
	fclose(fp);
	nFunRet = RET_GOOD;
	
	return nFunRet;
}


int CPublic_Function::GetModelNumFromModel( CString strModel)
{
	int nFuncRet = -1;
	int nRet = RET_ERROR;
	nRet = LoadRegModelData();
	if(nRet == RET_GOOD)
	{
		for ( int i = 0; i < st_work.m_nMdlTotal; i++ )
		{
			if( st_work.m_strModelName[i].Compare( (LPCTSTR) strModel) == 0 )
			{
				nFuncRet = i;
				break;
			}
		}
	}
	return nFuncRet;
	
}
int CPublic_Function::LoadRegModelData()
{
	int nFuncRet = RET_ERROR;
	int nTotal = 0;
	FILE    *fp ;
	int     existence;
	char chr_data[100];
	
	CString str_LoadFile = "C:\\AMT830\\Setting\\ModelName.ini";
	
	existence = access(str_LoadFile, 0);
	
	if (!existence)
	{
		if ((fp=fopen(str_LoadFile,"rb")) == NULL)
		{
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg, "[%s] file open error.", str_LoadFile);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			st_work.m_nMdlTotal = 0;
			return nFuncRet;
		}
	}
	else
	{
		if (st_handler.cwnd_list != NULL)
		{
			sprintf(st_msg.c_abnormal_msg, "[%s] file is not exist.", str_LoadFile);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		st_work.m_nMdlTotal = 0;
		return nFuncRet;
	}
	:: GetPrivateProfileString("Model_Name", "Total", "0", chr_data, 100, str_LoadFile);
	nTotal = atoi(chr_data);
	
	st_work.m_nMdlTotal = nTotal;
	
	CString str,strLabel;
	for (int i = 0; i < nTotal; i++)
	{
		str.Format("%d",i+1);
		:: GetPrivateProfileString("Model_Name", str, "0", chr_data, 100, str_LoadFile);
		strLabel = chr_data;
		strLabel.TrimLeft(' ');               
		strLabel.TrimRight(' ');
		st_work.m_strModelName[i] = strLabel;
	}
	fclose(fp);
	
	nFuncRet = RET_GOOD;
	
	return nFuncRet;
}
///////////////////////////////////////////////////////////////////////////

int CPublic_Function::OnHeatSinkModel_Change_Req()
{
	int nFuncRet = RET_GOOD;
	
	int num = 0, num1 =0;
	//2016.0823  M386A->DDR4  RDMM을 DDR4로 변경
// 	if(st_NW.mstr_Recive_PartNo[0].Mid(0,5) == _T("M393B") ||
// 		st_NW.mstr_Recive_PartNo[0].Mid(0,5) == _T("M386A") || 
// 		st_NW.mstr_Recive_PartNo[0].Mid(0,5) == _T("M393A") ) //2017.0223
// 	{
// 		st_work.n_hsNojob = 3;//RDMM 003//20120831
// 	}
// 	else if(st_NW.mstr_Recive_PartNo[0].Mid(0,5) == _T("M392B"))
// 	{
// 		st_work.n_hsNojob = 1;//VLPDIMM 001//20120831
// 	}
// 	else if(st_NW.mstr_Recive_PartNo[0].Mid(0,5) == _T("M386B"))
// 	{
// 		st_work.n_hsNojob = 2;//LRDMM 001//20140114
// 	}
// 	else
// 	{
// 		nFuncRet = RET_ERROR; //CTLBD_RET_ERROR  // num1 = 0;//error NRDIMM 002
// 		//992000 0 99 "BCR Job Change Model이 없습니다."
// 		//992001 0 99 "Heat Sink Job Change Model이 없습니다."
// 		sprintf(cJamcode,"992001");
// 		CTL_Lib.Alarm_Error_Occurrence(5064, CTL_dWARNING, cJamcode);
// 		if(st_handler.cwnd_list != NULL)
// 		{  // 리스트 바 화면 존재 //
// 			sprintf(st_msg.c_abnormal_msg, "Heat Sink Job Change Model이 없습니다.");
// 			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
// 			{
// 				sprintf(st_msg.c_abnormal_msg, "No Heat Sink Job Change Model");
// 			}
// 
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
// 		}
// 		return RET_ERROR;
// 
// 	}

	//////////////////////////////////////////////////////////////////////////////
	//2017.0428
	CString strLotID,strPartID, strModel;
	CString strname = st_basic.mstr_device_name;
	strname.Replace(".TXT", "");
	st_work.m_strCurModel = strname;
//	strPartID = st_NW.mstr_Recive_PartNo[0].Mid(0,5);
	strPartID = st_NW.mstr_Recive_PartNo[0].Mid(0,12);
	nMdl = Func.GetModelFromPartID(strPartID, strModel);
	if( nMdl != RET_GOOD )
	{
		nFuncRet = RET_ERROR; //CTLBD_RET_ERROR  // num1 = 0;//error NRDIMM 002
		//992000 0 99 "BCR Job Change Model이 없습니다."
		//992001 0 99 "Heat Sink Job Change Model이 없습니다."
		sprintf(cJamcode,"992001");
		CTL_Lib.Alarm_Error_Occurrence(5064, CTL_dWARNING, cJamcode);
		if(st_handler.cwnd_list != NULL)
		{  // 리스트 바 화면 존재 //
			sprintf(st_msg.c_abnormal_msg, "Heat Sink Job Change Model이 없습니다.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "No Heat Sink Job Change Model. Choose model in basic.");
			}
			
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
		}
		return RET_ERROR;
		
	}

	if(strModel=="RDIMM")		st_work.n_hsNojob = 3;
	else if(strModel=="LRDMM")  st_work.n_hsNojob = 2;
	else if(strModel=="TSV")	st_work.n_hsNojob = 1;
	else
	{
 		nFuncRet = RET_ERROR; //CTLBD_RET_ERROR  // num1 = 0;//error NRDIMM 002
 		//992000 0 99 "BCR Job Change Model이 없습니다."
 		//992001 0 99 "Heat Sink Job Change Model이 없습니다."
 		sprintf(cJamcode,"992001");
 		CTL_Lib.Alarm_Error_Occurrence(5064, CTL_dWARNING, cJamcode);
 		if(st_handler.cwnd_list != NULL)
 		{  // 리스트 바 화면 존재 //
 			sprintf(st_msg.c_abnormal_msg, "Heat Sink Job Change Model이 없습니다.");
 			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
 			{
 				sprintf(st_msg.c_abnormal_msg, "No Heat Sink Job Change Model");
 			}
 
 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
 		}
 		return RET_ERROR;
	}

	st_handler.n_bSendHeatSink = FALSE;
	//	::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_CJ);
	Func.OnSet_Vision_send(HEATSINK_CJ);

	ml_UntilWaitTime[0] = GetCurrentTime();
	return nFuncRet;
}

int CPublic_Function::OnHeatSinkModel_Change_Rev()
{
	int nFuncRet = RET_PROCEED;

	ml_UntilWaitTime[1] = GetCurrentTime();

	ml_UntilWaitTime[2] = ml_UntilWaitTime[1] - ml_UntilWaitTime[0];
	if(ml_UntilWaitTime[2] < 0) ml_UntilWaitTime[0] = GetCurrentTime();

	if(st_handler.n_bSendHeatSink == TRUE)
	{
		nFuncRet = RET_GOOD;
		return nFuncRet;
	}

	if(ml_UntilWaitTime[2] < 5000) return nFuncRet;
	else nFuncRet = RET_ERROR;
	
	return nFuncRet;

}

int CPublic_Function::OnBCRModel_Change_Req()
{
	
	int nFuncRet = RET_GOOD;
	int num2 = 0;
	int i;
	BOOL b_partnumber_check;
	CString  str_part_Check;
	
//	 st_NW.mstr_Recive_PartNo[0] = _T("M386B4G70DM0-CMA4");	

	if(st_NW.mstr_Recive_PartNo[0].GetLength() < 10)		
	{
		nFuncRet = RET_ERROR;//st_work.n_bcrNojob = 0; //error
		//992000 0 99 "BCR Job Change Model이 없습니다."
		//992001 0 99 "Heat Sink Job Change Model이 없습니다."
		if(st_handler.cwnd_list != NULL)
		{  // 리스트 바 화면 존재 //
			sprintf(st_msg.c_abnormal_msg, "Can't Read PartNo. Again try please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
		}
		sprintf(cJamcode,"992000");
		CTL_Lib.Alarm_Error_Occurrence(5064, CTL_dWARNING, cJamcode);
		return RET_ERROR;
	}


	PartNumber_Check();
	b_partnumber_check = FALSE;
	for(i = 0 ; i < 100 ; i++)  
	{
		if (st_basic.mstr_part_number_Check[i] != "")
		{
			if(st_NW.mstr_Recive_PartNo[0] == st_basic.mstr_part_number_Check[i])
			{
				str_part_Check = st_basic.mstr_mode_part_Check[i].Mid(0,3);
				b_partnumber_check = TRUE;
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (b_partnumber_check == FALSE)
	{
		if(st_handler.cwnd_list != NULL)
		{  // 리스트 바 화면 존재 //
			sprintf(st_msg.c_abnormal_msg, "Can't Read PartNo. Again try please");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
		}

		sprintf(cJamcode,"992000");
		CTL_Lib.Alarm_Error_Occurrence(5064, CTL_dWARNING, cJamcode);
		return RET_ERROR;		
	}

	//////////////////////////////////////// 20120828
	//001) 002)G1_HPQ 003)G1_NORMAL 004)G2_NORMAL 005) 006)G2_HPQ 
	if(str_part_Check == "002") 
	{
		st_work.n_bcrNojob = 2;
		st_basic.mstr_label_name = "002)G1_HPQ.TXT";
		st_basic.n_mode_label =  st_work.n_bcrNojob;
		mcls_m_basic.OnBarcode_Label_Data_Load();
		mcls_m_basic.OnBarcode_Label_Data_Save();
	}
	else if(str_part_Check == "003") 
	{
		st_work.n_bcrNojob = 3;
		st_basic.mstr_label_name = "003)G1_NORMAL.TXT";
		st_basic.n_mode_label =  st_work.n_bcrNojob;
		mcls_m_basic.OnBarcode_Label_Data_Load();
		mcls_m_basic.OnBarcode_Label_Data_Save();

	}
	///////////////////////////////// 20140206
	else if(str_part_Check == "010") 
	{
		st_work.n_bcrNojob = 10;
		st_basic.mstr_label_name = "010)G2_HPQ.TXT";
		st_basic.n_mode_label =  st_work.n_bcrNojob;
		mcls_m_basic.OnBarcode_Label_Data_Load();
		mcls_m_basic.OnBarcode_Label_Data_Save();

	}
	else if(str_part_Check == "009") 
	{
		st_work.n_bcrNojob = 9;
		st_basic.mstr_label_name = "009)G2_NORMAL.TXT";
		st_basic.n_mode_label =  st_work.n_bcrNojob;
		mcls_m_basic.OnBarcode_Label_Data_Load();
		mcls_m_basic.OnBarcode_Label_Data_Save();
	}
	////////////////////////////////////////
	//////////////////////////////////////// 20140114
	else if(str_part_Check == "008") 
	{
		st_work.n_bcrNojob = 8;
		st_basic.mstr_label_name = "008)G4_HPQ.TXT";
		st_basic.n_mode_label =  st_work.n_bcrNojob;
		mcls_m_basic.OnBarcode_Label_Data_Load();
		mcls_m_basic.OnBarcode_Label_Data_Save();

	}
	else if(str_part_Check == "007") 
	{
		st_work.n_bcrNojob = 7;
		st_basic.mstr_label_name = "007)G4_NORMAL.TXT";
		st_basic.n_mode_label =  st_work.n_bcrNojob;
		mcls_m_basic.OnBarcode_Label_Data_Load();
		mcls_m_basic.OnBarcode_Label_Data_Save();
	}
	////////////////////////////////////////

	else
	{
		nFuncRet = RET_ERROR;//st_work.n_bcrNojob = 0; //error
		//992000 0 99 "BCR Job Change Model이 없습니다."
		//992001 0 99 "Heat Sink Job Change Model이 없습니다."
		if(st_handler.cwnd_list != NULL)
		{  // 리스트 바 화면 존재 //
			sprintf(st_msg.c_abnormal_msg, "BCR Job Change Model이 없습니다.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				sprintf(st_msg.c_abnormal_msg, "No BCR Job Change Model");
			}

			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
		}

		sprintf(cJamcode,"992000");
		CTL_Lib.Alarm_Error_Occurrence(5065, CTL_dWARNING, cJamcode);
		return RET_ERROR;
	}

	st_handler.n_bSendBcr = FALSE;
	//::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, VISION_CJ);
	Func.OnSet_Vision_send(VISION_CJ);
	ml_UntilWaitTime[0] = GetCurrentTime();

	return nFuncRet;


// 	int nFuncRet = RET_GOOD;
// 	int num2 = 0;
// 
// // st_NW.mstr_Recive_PartNo[0] = _T("M393B5673GB0-CH9Q9");	
// //	st_NW.mstr_Recive_PartNo[0] = _T("M378B5273CH0-CK0");
// //	st_NW.mstr_Recive_PartNo[0] = _T("M379B5273DH0-YK0");
// 	int g,hpq, num = 0, num1 =0;
// 	if(st_NW.mstr_Recive_PartNo[0].GetLength() <= 10)		
// 	{
// 		nFuncRet = RET_ERROR;//st_work.n_bcrNojob = 0; //error
// //992000 0 99 "BCR Job Change Model이 없습니다."
// //992001 0 99 "Heat Sink Job Change Model이 없습니다."
// 		if(st_handler.cwnd_list != NULL)
// 		{  // 리스트 바 화면 존재 //
// 			sprintf(st_msg.c_abnormal_msg, "Can't Read PartNo. Again try please");
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
// 		}
// 
// 		sprintf(cJamcode,"992000");
// 		CTL_Lib.Alarm_Error_Occurrence(5064, CTL_dWARNING, cJamcode);
// 		return RET_ERROR;
// 	}
// 	if(st_NW.mstr_Recive_PartNo[0].GetAt(9) == 'B' || st_NW.mstr_Recive_PartNo[0].GetAt(9) == 'D')
// 	{
// 		g = 2;
// 	}
// 	else g = 1;
// 	
// 	num2 = st_NW.mstr_Recive_PartNo[0].GetLength();
// 
// 	if(st_NW.mstr_Recive_PartNo[0].GetLength() > 16 && st_NW.mstr_Recive_PartNo[0].GetAt(16) == 'Q')
// 	{
// 		hpq = 1;
// 	}
// 	else hpq = 0;
// 
// 	//////////////////////////////////////// 20120828
// 	//001) 002)G1_HPQ 003)G1_NORMAL 004)G2_NORMAL 005) 006)G2_HPQ 
// // 	if(g == 1 && hpq == 1) st_work.n_bcrNojob = 2;
// // 	else if(g == 1 && hpq == 0) st_work.n_bcrNojob = 3;
// // 	else if(g == 2 && hpq == 1) st_work.n_bcrNojob = 6;
// // 	else if(g == 2 && hpq == 0) st_work.n_bcrNojob = 4;
// 	
// 	if(g == 1 && hpq == 1) 
// 	{
// 		st_work.n_bcrNojob = 2;
// 		st_basic.mstr_label_name = "002)G1_HPQ.TXT";
// 		st_basic.n_mode_label =  st_work.n_bcrNojob;
// 		mcls_m_basic.OnBarcode_Label_Data_Load();
// 		mcls_m_basic.OnBarcode_Label_Data_Save();
// 	}
// 	else if(g == 1 && hpq == 0) 
// 	{
// 		st_work.n_bcrNojob = 3;
// 		st_basic.mstr_label_name = "003)G1_NORMAL.TXT";
// 		st_basic.n_mode_label =  st_work.n_bcrNojob;
// 		mcls_m_basic.OnBarcode_Label_Data_Load();
// 		mcls_m_basic.OnBarcode_Label_Data_Save();
// 
// 	}
// 	else if(g == 2 && hpq == 1) 
// 	{
// 		st_work.n_bcrNojob = 6;
// 		st_basic.mstr_label_name = "006)G2_HPQ.TXT";
// 		st_basic.n_mode_label =  st_work.n_bcrNojob;
// 		mcls_m_basic.OnBarcode_Label_Data_Load();
// 		mcls_m_basic.OnBarcode_Label_Data_Save();
// 
// 	}
// 	else if(g == 2 && hpq == 0) 
// 	{
// 		st_work.n_bcrNojob = 4;
// 		st_basic.mstr_label_name = "004)G2_NORMAL.TXT";
// 		st_basic.n_mode_label =  st_work.n_bcrNojob;
// 		mcls_m_basic.OnBarcode_Label_Data_Load();
// 		mcls_m_basic.OnBarcode_Label_Data_Save();
// 	}
// 	////////////////////////////////////////
//	//////////////////////////////////////// 20140114
///*	else if(g == 4 && hpq == 1)  
//	{
//		st_work.n_bcrNojob = 8;
//		st_basic.mstr_label_name = "008)G4_HPQ.TXT";
//		st_basic.n_mode_label =  st_work.n_bcrNojob;
//		mcls_m_basic.OnBarcode_Label_Data_Load();
//		mcls_m_basic.OnBarcode_Label_Data_Save();
//
//	}
//	else if(g == 4 && hpq == 0) 
//	{
//		st_work.n_bcrNojob = 7;
//		st_basic.mstr_label_name = "007)G4_NORMAL.TXT";
//		st_basic.n_mode_label =  st_work.n_bcrNojob;
//		mcls_m_basic.OnBarcode_Label_Data_Load();
//		mcls_m_basic.OnBarcode_Label_Data_Save();
//	}*/
//	////////////////////////////////////////
//
// 	else
// 	{
// 		nFuncRet = RET_ERROR;//st_work.n_bcrNojob = 0; //error
// //992000 0 99 "BCR Job Change Model이 없습니다."
// //992001 0 99 "Heat Sink Job Change Model이 없습니다."
// 		if(st_handler.cwnd_list != NULL)
// 		{  // 리스트 바 화면 존재 //
// 			sprintf(st_msg.c_abnormal_msg, "BCR Job Change Model이 없습니다.");
// 			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청 //
// 		}
// 
// 		sprintf(cJamcode,"992000");
// 		CTL_Lib.Alarm_Error_Occurrence(5065, CTL_dWARNING, cJamcode);
// 		return RET_ERROR;
// 	}
// 
// 	st_handler.n_bSendBcr = FALSE;
// 	//::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, VISION_CJ);
// 	Func.OnSet_Vision_send(VISION_CJ);
// 	ml_UntilWaitTime[0] = GetCurrentTime();
// 
// 	return nFuncRet;
}

int CPublic_Function::OnBCRModel_Change_Rev()
{
	int nFuncRet = RET_PROCEED;

	ml_UntilWaitTime[1] = GetCurrentTime();

	ml_UntilWaitTime[2] = ml_UntilWaitTime[1] - ml_UntilWaitTime[0];
	if(ml_UntilWaitTime[2] < 0) ml_UntilWaitTime[0] = GetCurrentTime();

	if(st_handler.n_bSendBcr == TRUE)
	{
		nFuncRet = RET_GOOD;
	}
	else
	{
		if(ml_UntilWaitTime[2] < 5000) return nFuncRet;
		else nFuncRet = RET_ERROR;
	}
	
	return nFuncRet;
}


void CPublic_Function::PartNumber_Check()
{
	int nStartIndex, nEndIndex;  // BCR 데이터의 시작 및 종료 위치 저장 변수 
	int nLength;  // 데이터 총 길이 저장 변수 
	int i, j;

	for(i = 0 ; i < 100 ; i++)  
	{
		if (st_basic.mstr_part_number[i] != "")
		{
			nStartIndex = 0;
			nLength = st_basic.mstr_part_number[i].GetLength();
			for(j = 0 ; j < 2 ; j++)  
			{
				nEndIndex   = st_basic.mstr_part_number[i].Find(",", nStartIndex);  // BCR 데이터 종료 위치 설정 
				
				if (nEndIndex == -1)
				{
					if (nLength - nStartIndex > 0)
					{
						nEndIndex = nLength;
					}
					else
					{
						break;
					}
				}
				if (j == 0)
				{
					st_basic.mstr_part_number_Check[i] = st_basic.mstr_part_number[i].Mid(nStartIndex, nEndIndex - nStartIndex);
				}
				if (j == 1) 
				{
					st_basic.mstr_mode_part_Check[i] = st_basic.mstr_part_number[i].Mid(nStartIndex, nEndIndex - nStartIndex);
					break;
				}
				nStartIndex = nEndIndex + 1;
			}
		}
		else
		{
			break;
		}
	}
}

int CPublic_Function::InitNode()
{	    
	MDhead = (st_MDRbt*)malloc(sizeof(st_MDRbt));
	MDtail = (st_MDRbt*)malloc(sizeof(st_MDRbt));

	LHShead = (st_LHSRbt*)malloc(sizeof(st_LHSRbt));
	LHStail = (st_LHSRbt*)malloc(sizeof(st_LHSRbt));

	RHShead = (st_RHSRbt*)malloc(sizeof(st_RHSRbt));
	RHStail = (st_RHSRbt*)malloc(sizeof(st_RHSRbt));

	CLIPhead = (st_CLIPRbt*)malloc(sizeof(st_CLIPRbt));
	CLIPtail = (st_CLIPRbt*)malloc(sizeof(st_CLIPRbt));

	ULDhead = (st_ULDRbt*)malloc(sizeof(st_ULDRbt));
	ULDtail = (st_ULDRbt*)malloc(sizeof(st_ULDRbt));
	
	MDhead->next = MDtail;
	LHShead->next = LHStail;
	RHShead->next = RHStail;
	MDhead->next = MDtail;
	MDhead->next = MDtail;

 	MDhead->n_site = -1;
	LHShead->n_site = -1;
	RHShead->n_site = -1;
	CLIPhead->n_site = -1;
	ULDhead->n_site = -1;
		
	MDtail = NULL;
	LHStail = NULL;
	RHStail = NULL;
	CLIPtail = NULL;
	ULDtail = NULL;

	for(int i = 0; i < 4; i++)
	{
		if(st_picker.n_work_buffer_enable[i] == FALSE)
		{
		}
		else
		{ 
			MDInsertNode(i);
		}
	}	
	return 1;
}

int CPublic_Function::MDInsertNode(int n_site)
{
	st_MDRbt *s;
	
	if(MDtail == NULL) {
		MDtail = (st_MDRbt*)malloc(sizeof(st_MDRbt));
		if(MDtail == NULL)
			exit(0);
		MDtail->n_site = n_site;
		MDtail->next = NULL;
	}
	else
	{
		s = MDtail;
		while(s->next != NULL)
			s = s->next;
		s->next = (st_MDRbt*)malloc(sizeof(st_MDRbt));
		if(s->next == NULL)
			exit(0);
		s = s->next;
		s->n_site = n_site;
		s->next = NULL;
	}
	MDhead = MDtail;
	
	return 1;
}

int CPublic_Function::LHSInsertNode(int n_site)
{
	st_LHSRbt *s;
		
	if(LHStail == NULL) {
		LHStail = (st_LHSRbt*)malloc(sizeof(st_LHSRbt));
		if(LHStail == NULL)
			exit(0);
		LHStail->n_site = n_site;
		LHStail->next = NULL;
	}
	else
	{
		s = LHStail;
		while(s->next != NULL)
			s = s->next;
		s->next = (st_LHSRbt*)malloc(sizeof(st_LHSRbt));
		if(s->next == NULL)
			exit(0);
		s = s->next;
		s->n_site = n_site;
		s->next = NULL;
	}
	LHShead = LHStail;
	
	return 1;
}
int CPublic_Function::RHSInsertNode(int n_site)
{
	st_RHSRbt *s;
	
	if(RHStail == NULL) {
		RHStail = (st_RHSRbt*)malloc(sizeof(st_RHSRbt));
		if(RHStail == NULL)
			exit(0);
		RHStail->n_site = n_site;
		RHStail->next = NULL;
	}
	else
	{
		s = RHStail;
		while(s->next != NULL)
			s = s->next;
		s->next = (st_RHSRbt*)malloc(sizeof(st_RHSRbt));
		if(s->next == NULL)
			exit(0);
		s = s->next;
		s->n_site = n_site;
		s->next = NULL;
	}
	RHShead = RHStail;
	
	return 1;
}
int CPublic_Function::CLIPInsertNode(int n_site)
{
	st_CLIPRbt *s;
	
	if(CLIPtail == NULL) {
		CLIPtail = (st_CLIPRbt*)malloc(sizeof(st_CLIPRbt));
		if(CLIPtail == NULL)
			exit(0);
		CLIPtail->n_site = n_site;
		CLIPtail->next = NULL;
	}
	else
	{
		s = CLIPtail;
		while(s->next != NULL)
			s = s->next;
		s->next = (st_CLIPRbt*)malloc(sizeof(st_CLIPRbt));
		if(s->next == NULL)
			exit(0);
		s = s->next;
		s->n_site = n_site;
		s->next = NULL;
	}
	CLIPhead = CLIPtail;
	
	return 1;
}
int CPublic_Function::ULDInsertNode(int n_site)
{
	st_ULDRbt *s;
	
	if(ULDtail == NULL) {
		ULDtail = (st_ULDRbt*)malloc(sizeof(st_ULDRbt));
		if(ULDtail == NULL)
			exit(0);
		ULDtail->n_site = n_site;
		ULDtail->next = NULL;
	}
	else
	{
		s = ULDtail;
		while(s->next != NULL)
			s = s->next;
		s->next = (st_ULDRbt*)malloc(sizeof(st_ULDRbt));
		if(s->next == NULL)
			exit(0);
		s = s->next;
		s->n_site = n_site;
		s->next = NULL;
	}
	ULDhead = ULDtail;
	
	return 1;
}

int CPublic_Function::MDDelete_Node()
{
	if(MDtail == NULL)
		return 0;
	MDtail = MDhead;
	if(MDtail != NULL)
	{
		MDhead = MDtail->next;
		free(MDtail);
		MDtail = MDhead;
	}

	return 1;
}

int CPublic_Function::LHSDelete_Node()
{
	if(LHStail == NULL)
		return 0;
	LHStail = LHShead;
	if(LHStail != NULL)
	{
		LHShead = LHStail->next;
		free(LHStail);
		LHStail = LHShead;
	}
	
	return 1;
}

int CPublic_Function::RHSDelete_Node()
{
	if(RHStail == NULL)
		return 0;
	RHStail = RHShead;
	if(RHStail != NULL)
	{
		RHShead = RHStail->next;
		free(RHStail);
		RHStail = RHShead;
	}
	
	return 1;
}

int CPublic_Function::CLIPDelete_Node()
{
	if(CLIPtail == NULL)
		return 0;
	CLIPtail = CLIPhead;
	if(CLIPtail != NULL)
	{
		CLIPhead = CLIPtail->next;
		free(CLIPtail);
		CLIPtail = CLIPhead;
	}
	
	return 1;
}

int CPublic_Function::ULDDelete_Node()
{
	if(ULDtail == NULL)
		return 0;
	ULDtail = ULDhead;
	if(ULDtail != NULL)
	{
		ULDhead = ULDtail->next;
		free(ULDtail);
		ULDtail = ULDhead;
	}
	
	return 1;
}

int CPublic_Function::MDFindNode()
{
	st_MDRbt *s;
	int ReturnFlag;
	s = MDhead;

	if(MDhead == NULL)
	{
		ReturnFlag = -1;
	}
	else
	{
		ReturnFlag = s->n_site;
	}	
	return ReturnFlag;
}

int CPublic_Function::LHSFindNode()
{
	st_LHSRbt *s;	
	int ReturnFlag;
	s = LHShead;
	
	if(LHShead == NULL)
	{
		ReturnFlag = -1;
	}
	else
	{
		ReturnFlag = s->n_site;
	}

	return ReturnFlag;
}

int CPublic_Function::RHSFindNode()
{
	st_RHSRbt *s;
	int ReturnFlag;
	s = RHShead;

	if(RHShead == NULL)
	{
		ReturnFlag = -1;
	}
	else
	{
		ReturnFlag = s->n_site;
	}

	return ReturnFlag;
}

int CPublic_Function::CLIPFindNode()
{
	st_CLIPRbt *s;
	int ReturnFlag;
	s = CLIPhead;

	if(CLIPhead == NULL)
	{
		ReturnFlag = -1;
	}
	else
	{
		ReturnFlag = s->n_site;
	}

	return ReturnFlag;
}

int CPublic_Function::ULDFindNode()
{
	st_ULDRbt *s;
	int ReturnFlag;
	s = ULDhead;

	if(ULDhead == NULL)
	{
		ReturnFlag = -1;
	}
	else
	{
		ReturnFlag = s->n_site;
	}

	return ReturnFlag;
}

//// 20131210

void CPublic_Function::Set_Module_AllPickerUpDn()
{
	for (int i = 0; i < PICKER_NUM; i++)
	{
		FAS_IO.set_out_bit(st_io.o_ld_module_rbt_picker1_updn + (i * 2), IO_OFF);
	}
}

int CPublic_Function::Get_Module_AllPickerUpDn()
{
	for (int i = 0; i < PICKER_NUM; i++)
	{
		if(FAS_IO.get_out_bit(st_io.i_ld_module_rbt_picker1_up_chk + (i * 2), IO_ON) == IO_OFF)
		{

		}
		else
		{
			return RET_ERROR;
		}
	}

	return RET_GOOD;
}

void CPublic_Function::Set_LeftHS_AllPickerUpDn()
{
	for (int i = 0; i < PICKER_NUM; i++)
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_picker1_updn + (i * 2), IO_OFF);
	}
}

int CPublic_Function::Get_LeftHS_AllPickerUpDn()
{
	for (int i = 0; i < PICKER_NUM; i++)
	{
		if(FAS_IO.get_out_bit(st_io.i_hs_fwd_rbt_picker1_up_chk + (i * 2), IO_ON) == IO_OFF)
		{

		}
		else
		{
			return RET_ERROR;
		}
	}

	return RET_GOOD;
}

void CPublic_Function::Set_RightHS_AllPickerUpDn()
{
	for (int i = 0; i < PICKER_NUM; i++)
	{
		FAS_IO.set_out_bit(st_io.o_hs_bwd_rbt_picker1_updn + (i * 2), IO_OFF);
	}
}

int CPublic_Function::Get_RightHS_AllPickerUpDn()
{
	for (int i = 0; i < PICKER_NUM; i++)
	{
		if(FAS_IO.get_out_bit(st_io.i_hs_bwd_rbt_picker1_up_chk + (i * 3), IO_ON) == IO_OFF)
		{

		}
		else
		{
			return RET_ERROR;
		}
	}

	return RET_GOOD;
}

void CPublic_Function::Excel_File_Read()//2014,0408
{
	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수
	int n_cur_hour, n_cur_minute, n_cur_second;		// 년, 월, 일 저장 변수
	
	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	CTime mp_time_cur;							// 타이틀 시간 저장 변수
	CString strTmp;
	CString sPath, sTemp;
	CString mstr_name[10];
	
	CString		sFieldName[ deFildSu ];
	char		cDelimChar = ',';
	int			iFieldCnt;

	char fileName[256];
	int n_existence;		// 파일 존재 유무 플래그

	mtime_cur = COleDateTime::GetCurrentTime();
	
	st_work.m_nExcel_Cnt = 0;

	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  
	
	n_cur_hour = mtime_cur.GetHour();
	n_cur_minute = mtime_cur.GetMinute();
	n_cur_second = mtime_cur.GetSecond();
	
	strTmp.Format("%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	sTemp = strTmp + _T(".csv");	
	sPath = st_path.mstr_Excel + sTemp;

	sprintf(fileName, "%s", sPath);
	n_existence = access(fileName,0) ;
	if (n_existence == -1)  // 해당 파일이 미존재
	{
		CStdioFile file(fileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeText);
		CString strHeader;
		strHeader.Format("Date, Model, Lot, In, Pass, Fail, Start time, End Time, Run Time\n");
		file.WriteString(strHeader);
		st_work.m_nExcel_Cnt++;
		return;
	}

	CStdioFile file(sPath, CFile::modeRead|CFile::shareDenyWrite);

	while( 1 )
	{
		for( int i=0 ; i<deFildSu ; i++ )
		{
			sFieldName[i] = "";
		}
		
 		GetStrings(&file, cDelimChar, sFieldName, &iFieldCnt);				//한 라인을 구분 하여 읽어 들임니다.
		if(iFieldCnt <= 0)
		{	//읽어 들인 것이 없다면..
			st_work.m_nExcel_Cnt++;
			break;
		}
		else
		{

			for( i=0 ; i<iFieldCnt ; i++ )
			{	//문자 열로 만듭니다.				

				if(i == 0) st_work.mstr_Excel_Date[st_work.m_nExcel_Cnt] = sFieldName[i];
				if(i == 1) st_work.mstr_Excel_Model[st_work.m_nExcel_Cnt] = sFieldName[i];
				if(i == 2) st_work.mstr_Excel_LotNo[st_work.m_nExcel_Cnt] = sFieldName[i];
				if(i == 3) st_work.m_nExcel_Input[st_work.m_nExcel_Cnt] = atoi(sFieldName[i]);
				if(i == 4) st_work.m_nExcel_Pass[st_work.m_nExcel_Cnt] = atoi(sFieldName[i]);
				if(i == 5) st_work.m_nExcel_Fail[st_work.m_nExcel_Cnt] = atoi(sFieldName[i]);
				if(i == 6) st_work.mstr_Excel_StartTime[st_work.m_nExcel_Cnt] = sFieldName[i];
				if(i == 7) st_work.mstr_Excel_EndTime[st_work.m_nExcel_Cnt] = sFieldName[i];
				if(i == 8) st_work.mstr_Excel_RunTime[st_work.m_nExcel_Cnt] = sFieldName[i];
				
			}
			st_work.m_nExcel_Cnt++;
		}
	}

}

void CPublic_Function::Excel_File_Save()//2014,0408
{
	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수
	int n_cur_hour, n_cur_minute, n_cur_second;		// 년, 월, 일 저장 변수
	int n_existence;		// 파일 존재 유무 플래그

	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	CTime mp_time_cur;							// 타이틀 시간 저장 변수
	CString strTmp;
	CString sPath, sTemp;
	CString mstr_name[10];
	char fileName[256];
	CString strHeader;

	mtime_cur = COleDateTime::GetCurrentTime();
	
	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  
	
	n_cur_hour = mtime_cur.GetHour();
	n_cur_minute = mtime_cur.GetMinute();
	n_cur_second = mtime_cur.GetSecond();
	
	strTmp.Format("%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	sTemp = strTmp + _T(".csv");	
	sPath = st_path.mstr_Excel + sTemp;

	sprintf(fileName, "%s", sPath);
	n_existence = access(fileName,0) ;
	if (n_existence == -1)  // 해당 파일이 미존재
	{
	}
	else
	{
		CStdioFile file(fileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeText);
		strHeader.Format("Date, Model, Lot, In, Pass, Fail, Start time, End Time, Run Time\n");
		file.WriteString(strHeader);

		for( int i=0; i<st_work.m_nExcel_Cnt; i++ )
		{	
			if(i != 0)
			{
 				if(st_work.mstr_Excel_Date[i] == "")
 				{
					strHeader.Format("%s,%s,%s,%d,%d,%d,%s,%s,%s\n", strTmp, st_NW.mstr_Recive_PartNo[0], st_NW.mstr_lot_id[1], st_work.nMdlInputCount[0][0], st_work.nMdlPassCount[0][0], st_work.nMdlRejectCount[0][0], st_work.mstr_Start_time, st_work.mstr_End_time, st_work.mstr_Run_time);
					file.WriteString(strHeader);
				}
				else
				{
					strHeader.Format("%s,%s,%s,%d,%d,%d,%s,%s,%s\n",st_work.mstr_Excel_Date[i],	st_work.mstr_Excel_Model[i], st_work.mstr_Excel_LotNo[i], st_work.m_nExcel_Input[i], st_work.m_nExcel_Pass[i], st_work.m_nExcel_Fail[i], st_work.mstr_Excel_StartTime[i], st_work.mstr_Excel_EndTime[i], st_work.mstr_Excel_RunTime[i]);
					file.WriteString(strHeader);
				}
			}
		}

	}
}

BOOL CPublic_Function::GetStrings(CFile* pFile, char Delim, CString * str, int* pCnt)//2014,0408
{

	char	ch1 = '\n';							//파일에서 읽어 오는 문자 하나.
	char	ch2;
	char	ch3;
	int		FiCt = 0;					//지금 읽어 들인 필의 수.
	BOOL	bDm   = false;				//(") 가 시작 되었는가..??
	CString	sData = "";	


	if(1 != pFile->Read(&ch1, 1))
	{	//일어 들인 문자가 없다.
		*pCnt = FiCt;
		return TRUE;
	}

	if( ch1 == '"' )
	{	//구분자로 사용 될수 있는 문자 인가.
		if(1 != pFile->Read(&ch2, 1))
		{	//일어 들인 문자가 없다.
			*pCnt = FiCt;
			return TRUE;
		}

		if( ch2 == '"' )
		{	//또다시 구분자로 사용 될수 있는 문자 이다.
			if(1 != pFile->Read(&ch3, 1))
			{	//일어 들인 문자가 없다.
				*pCnt = FiCt;
				return TRUE;
			}

			if( ch3 == '"' )
			{	//""" 인 경우.
				sData+=ch3;				//구분자가 아니다.
				bDm = true;				//구분자가 시작되었다.
			}
			else
			{	//"" 인 경우.
				sData+=ch3;				//구분자가 아니다.
			}
		}
		else
		{	//"인 경우.
			sData+=ch2;
			bDm = true;				//구분자가 시작되었다.
		}
	}
	else
	{
		sData+=ch1;
	}

	while( 1 )
	{

		while( 1 )
		{
			if(1 != pFile->Read(&ch1, 1))
			{	//일어 들인 문자가 없다.
				*pCnt = FiCt;
				return TRUE;
			}

			if( ch1 == '\r' )
			{	// '\r' 이면 한번 더..( \r 과 \n 을 특별히 구분 하지 않을 려구 )
				if(1 != pFile->Read(&ch1, 1))
				{	//일어 들인 문자가 없다.
					str[FiCt] = sData;
					FiCt++;
					*pCnt = FiCt;
					return TRUE;
				}
			}


			if( ch1 == '"' )
			{	//구분자로 사용 될수 있는 문자 인가..
				if(1 != pFile->Read(&ch2, 1))
				{	//일어 들인 문자가 없다.
					str[FiCt] = sData;
					FiCt++;
					*pCnt = FiCt;
					return TRUE;
				}

				if( ch2 == '\r' )
				{	// '\r' 이면 한번 더..( \r 과 \n 을 특별히 구분 하지 않을 려구 )
					if(1 != pFile->Read(&ch2, 1))
					{	//일어 들인 문자가 없다.
						str[FiCt] = sData;
						FiCt++;
						*pCnt = FiCt;
						return TRUE;
					}
				}

				if( ch2 == '"' )
				{	//또다시 구분자로 사용 될수 있는 문자 이다.
					if(1 != pFile->Read(&ch3, 1))
					{	//일어 들인 문자가 없다.
						*pCnt = FiCt;
						return TRUE;
					}

					if( ch3 == '\r' )
					{	// '\r' 이면 한번 더..
						if(1 != pFile->Read(&ch3, 1))
						{	//일어 들인 문자가 없다.
							str[FiCt] = sData;
							FiCt++;
							*pCnt = FiCt;
							return TRUE;
						}
					}

					if( ch3 == '"' )
					{	//""" 인 경우.
						sData+=ch2;				//구분자가 아니다.
						if( bDm )
						{	//이미 구분자가 나왔다면.

							if(1 != pFile->Read(&ch3, 1))
							{	//일어 들인 문자가 없다.
								*pCnt = FiCt;
								return TRUE;
							}

							if( ch3 == '"' )
							{	//4번 연속..
								sData+=ch3;
							}
							else
							{
								str[FiCt] = sData;
								FiCt++;
								bDm = false;			//구분자가 끝남.							

								if( ch3 != Delim )
								{
									pFile->Seek(-1, CFile::current);
									break;
								}
							}

										
						}
						else
						{
							bDm = true;				//구분자가 시작되었다.
						}
					}
					else
					{	//"" 인 경우.
						if( bDm )
						{	//구분 자가 이미 나온 경우.
							sData+=ch2;					// "" 을 " 로 넣는다.
							sData+=ch3;					//다음 문자도 넣는다.
						}
						else if( ch3 == '\n' )
						{
							str[FiCt] = sData;
							FiCt++;
							bDm = false;			//구분자가 끝남.
							break;
						}
						else if( ch3 != Delim )
						{
							sData+=ch2;					// "" 을 " 로 넣는다.
							sData+=ch3;					//다음 문자도 넣는다.
						}
						else
						{
							str[FiCt] = sData;
							FiCt++;
							bDm = false;			//구분자가 끝남.
							break;
						}
					}
				}
				else
				{	//"인 경우.

					if( bDm )
					{
						str[FiCt] = sData;			//단어의 끝이다.
						FiCt++;
						bDm = false;
						break;
					}

					if( ch2 == '\n' )
					{
						str[FiCt] = sData;			//단어의 끝이다.
						FiCt++;
						bDm = false;
						break;
					}
					else
					{
						if( ch2 == Delim )
						{	//(,) 이면.
							if( bDm )
							{	//구분자가 시작 되었는가??
								sData+=ch2;					// "" 을 " 로 넣는다.
							}
							else
							{
								bDm = false;			//구분자가 끝남.
								str[FiCt] = sData;			//단어의 끝이다.
								FiCt++;
								break;
							}

						}
						else
						{
							sData+=ch2;					// "" 을 " 로 넣는다.
							bDm = true;					//구분자가 시작되었다.
						}
					}
				}

			}
			else
			{
				if( ch1 == Delim )
				{
					if( bDm )
					{	//구분자가 시작 되었는가??
						sData+=ch1;		//단어 의 끝이 아니다.
					}
					else
					{	//구분자가 시작 되지 않았다.
						str[FiCt] = sData;			//단어의 끝이다.
						FiCt++;
						bDm = false;			//구분자가 끝남.
						break;
					}
				}
				else
				{
					if( ch1 == '\n' )
					{	// " 도 아니고.. , 도 아니고.. \n 라면.
						if( bDm )
						{	//이미 구분자가 시작 되었는가??
							sData+=ch1;
						}
						else
						{
							str[FiCt] = sData;			//단어의 끝이다.
							FiCt++;
							bDm = false;			//구분자가 끝남.
							break;
						}
					}
					else
					{
						sData+=ch1;
					}
				}
			}
		}

		sData = "";

		while( 1 )
		{
/*			if(1 != pFile->Read(&ch3, 1))
			{	//일어 들인 문자가 없다.
				*pCnt = FiCt;
				return TRUE;
			}

			if( ch3 != Delim )
			{
				pFile->Seek(-1, CFile::current);
			}
*/
			if( ch1 == '\n' || ch2 == '\n' || ch3 == '\n' )
			{
//				pFile->Seek(-1, CFile::current);
				*pCnt = FiCt;
				return true;
			}
			
			ch1 = 0;
			ch2 = 0;
			ch3 = 0;

			break;

		}

	}

	
	return true;

}

////2014,1210
int CPublic_Function::Handler_Recovery_Varible_Data(int n_mode, int n_site, int n_debug )//2014,1210
{
	int i, j;
	FILE *fp;
	int nExistFile, nFuncRet = CTL_YES;
	CString sFileName, sFolderName;
	
	CString sDebug_FileName, sTimeName; //091206 추가 
	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	int nYear, nMonth, nDay, nHour, nMinite;
	
	mtime_cur = COleDateTime::GetCurrentTime(); 
	nYear   = mtime_cur.GetYear();  
	nMonth  = mtime_cur.GetMonth();  
    nDay    = mtime_cur.GetDay(); 
	nHour   = mtime_cur.GetHour(); 
	nMinite = mtime_cur.GetMinute(); 
	sTimeName.Format("%04d%02d%02d%02d%02d_", nYear, nMonth, nDay, nHour, nMinite);
	sFolderName.Format("%04d%02d%02d%02d%02d", nYear, nMonth, nDay, nHour, nMinite);

	sDebug_FileName = "C:\\AMT830\\Log\\Step\\" + sFolderName + "\\";
	if(n_mode == DATA_READ) //file read 데이터 확인시 사용 
	{
		sDebug_FileName = "C:\\AMT830\\Log\\Step\\";
	}
	else
	{
		sDebug_FileName = "C:\\AMT830\\Log\\Step\\" + sFolderName + "\\";
	}

	switch(n_site)
	{	
		case 0:	// 
			CreateFolder(sDebug_FileName);
			
			sDebug_FileName += "st_basic.dat";
			sFileName = sDebug_FileName;
			
			
			if(n_mode == DATA_WRITE) //file write
			{
				if((fp=fopen(sFileName,"wb")) == NULL)
				{			
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청

//						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					}				
					nFuncRet = CTL_NO;
					break;
				}
				else
				{ //정상 
					fwrite(&st_basic, sizeof(st_basic_param), 1, fp);
					if (ferror(fp)) 
					{
						if (st_handler.cwnd_list != NULL)
							
						{
							st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						}
						clearerr(fp);
						nFuncRet = CTL_NO;
						break;
					}	
					fclose(fp);
				}		
			}
			else if(n_mode == DATA_READ) //file read
			{
				nExistFile = access(sFileName, 0);	
				if (nExistFile)
				{
					st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					nFuncRet = CTL_NO;
					break;
				}
				
				if((fp=fopen(sFileName,"rb")) == NULL)
				{			
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}				
					nFuncRet = CTL_NO;
					break;
				}
				else
				{ //정상 
					fread(&st_basic, sizeof(st_basic_param), 1, fp);
					if (ferror(fp)) 
					{
						if (st_handler.cwnd_list != NULL)
						{
							st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						}
						clearerr(fp);
						nFuncRet = CTL_NO;
						break;
					}	
					fclose(fp);
				}		
			}
			break;

		case 1:
			sDebug_FileName += "st_sync.dat";
			sFileName = sDebug_FileName;
			
			
			if(n_mode == DATA_WRITE) //file write
			{
				if((fp=fopen(sFileName,"wb")) == NULL)
				{			
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}				
					nFuncRet = CTL_NO;
					break;
				}
				else
				{ //정상 
					fwrite(&st_sync, sizeof(st_sync_param), 1, fp);
					if (ferror(fp)) 
					{
						if (st_handler.cwnd_list != NULL)
							
						{
							st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						}
						clearerr(fp);
						nFuncRet = CTL_NO;
						break;
					}	
					fclose(fp);
				}		
			}
			else if(n_mode == DATA_READ) //file read
			{
				nExistFile = access(sFileName, 0);	
				if (nExistFile)
				{
					st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					nFuncRet = CTL_NO;
					break;
				}
				
				if((fp=fopen(sFileName,"rb")) == NULL)
				{			
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}				
					nFuncRet = CTL_NO;
					break;
				}
				else
				{ //정상 
					fread(&st_sync, sizeof(st_sync_param), 1, fp);
					if (ferror(fp)) 
					{
						if (st_handler.cwnd_list != NULL)
						{
							st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						}
						clearerr(fp);
						nFuncRet = CTL_NO;
						break;
					}	
					fclose(fp);
				}		
			}
			break;

		case 2:
			sDebug_FileName += "st_work.dat";
			sFileName = sDebug_FileName;
			
			
			if(n_mode == DATA_WRITE) //file write
			{
				if((fp=fopen(sFileName,"wb")) == NULL)
				{			
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}				
					nFuncRet = CTL_NO;
					break;
				}
				else
				{ //정상 
					fwrite(&st_sync, sizeof(st_work_param), 1, fp);
					if (ferror(fp)) 
					{
						if (st_handler.cwnd_list != NULL)
							
						{
							st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						}
						clearerr(fp);
						nFuncRet = CTL_NO;
						break;
					}	
					fclose(fp);
				}		
			}
			else if(n_mode == DATA_READ) //file read
			{
				nExistFile = access(sFileName, 0);	
				if (nExistFile)
				{
					st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					nFuncRet = CTL_NO;
					break;
				}
				
				if((fp=fopen(sFileName,"rb")) == NULL)
				{			
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}				
					nFuncRet = CTL_NO;
					break;
				}
				else
				{ //정상 
					fread(&st_sync, sizeof(st_work_param), 1, fp);
					if (ferror(fp)) 
					{
						if (st_handler.cwnd_list != NULL)
						{
							st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						}
						clearerr(fp);
						nFuncRet = CTL_NO;
						break;
					}	
					fclose(fp);
				}		
			}
			break;

		case 3:
			sDebug_FileName += "st_wait.dat";
			sFileName = sDebug_FileName;
			
		 
			if(n_mode == DATA_WRITE) //file write
			{
				if((fp=fopen(sFileName,"wb")) == NULL)
				{			
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}				
					nFuncRet = CTL_NO;
					break;
				}
				else
				{ //정상 
					fwrite(&st_wait, sizeof(st_io_wait_param), 1, fp);
					if (ferror(fp)) 
					{
						if (st_handler.cwnd_list != NULL)

						{
							st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						}
						clearerr(fp);
						nFuncRet = CTL_NO;
							break;
					}	
					fclose(fp);
				}		
			}
			else if(n_mode == DATA_READ) //file read
			{
				nExistFile = access(sFileName, 0);	
				if (nExistFile)
				{
					st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					nFuncRet = CTL_NO;
					break;
				}

				if((fp=fopen(sFileName,"rb")) == NULL)
				{			
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}				
					nFuncRet = CTL_NO;
					break;
				}
				else
				{ //정상 
					fread(&st_wait, sizeof(st_io_wait_param), 1, fp);
					if (ferror(fp)) 
					{
						if (st_handler.cwnd_list != NULL)
						{
							st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						}
						clearerr(fp);
						nFuncRet = CTL_NO;
							break;
					}	
					fclose(fp);
				}		
			}
			break;

		case 4:
			sDebug_FileName += "st_modulemap.dat";
			sFileName = sDebug_FileName;
			
		 
			if(n_mode == DATA_WRITE) //file write
			{
				if((fp=fopen(sFileName,"wb")) == NULL)
				{			
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}				
					nFuncRet = CTL_NO;
					break;
				}
				else
				{ //정상 
					fwrite(&st_modulemap, sizeof(st_modulemap_size), 1, fp);
					if (ferror(fp)) 
					{
						if (st_handler.cwnd_list != NULL)

						{
							st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						}
						clearerr(fp);
						nFuncRet = CTL_NO;
							break;
					}	
					fclose(fp);
				}		
			}
			else if(n_mode == DATA_READ) //file read
			{
				nExistFile = access(sFileName, 0);	
				if (nExistFile)
				{
					st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					nFuncRet = CTL_NO;
					break;
				}

				if((fp=fopen(sFileName,"rb")) == NULL)
				{			
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}				
					nFuncRet = CTL_NO;
					break;
				}
				else
				{ //정상 
					fread(&st_modulemap, sizeof(st_modulemap_size), 1, fp);
					if (ferror(fp)) 
					{
						if (st_handler.cwnd_list != NULL)
						{
							st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
						}
						clearerr(fp);
						nFuncRet = CTL_NO;
							break;
					}	
					fclose(fp);
				}		
			}
			break;

	case 5:
		sDebug_FileName += "st_thread_Run_Rbt_Load_Clip_param.dat";
		sFileName = sDebug_FileName;
		
	 
		if(n_mode == DATA_WRITE) //file write
		{
 			th_reco_Run_Rbt_Load_Clip.PrevStep = Run_Rbt_Load_Clip.PrevStep;
			th_reco_Run_Rbt_Load_Clip.RunInsertStep = Run_Rbt_Load_Clip.RunInsertStep;
			th_reco_Run_Rbt_Load_Clip.RunClampStep = Run_Rbt_Load_Clip.RunClampStep;
			th_reco_Run_Rbt_Load_Clip.MoveStep = Run_Rbt_Load_Clip.MoveStep;
			th_reco_Run_Rbt_Load_Clip.InitStep = Run_Rbt_Load_Clip.InitStep;
			th_reco_Run_Rbt_Load_Clip.RotatorStep = Run_Rbt_Load_Clip.RotatorStep;
			th_reco_Run_Rbt_Load_Clip.RunBufferStep = Run_Rbt_Load_Clip.RunBufferStep;
			th_reco_Run_Rbt_Load_Clip.ClipBufferStep = Run_Rbt_Load_Clip.ClipBufferStep;
			th_reco_Run_Rbt_Load_Clip.ClipPickStep = Run_Rbt_Load_Clip.ClipPickStep;
			th_reco_Run_Rbt_Load_Clip.ClipPlaceStep = Run_Rbt_Load_Clip.ClipPlaceStep;
			th_reco_Run_Rbt_Load_Clip.ClampRbtStep = Run_Rbt_Load_Clip.ClampRbtStep;
			th_reco_Run_Rbt_Load_Clip.ReinstateStep = Run_Rbt_Load_Clip.ReinstateStep;
			th_reco_Run_Rbt_Load_Clip.mn_NextPos = Run_Rbt_Load_Clip.mn_NextPos;
			th_reco_Run_Rbt_Load_Clip.InsertStep = Run_Rbt_Load_Clip.InsertStep;
			th_reco_Run_Rbt_Load_Clip.DropStep = Run_Rbt_Load_Clip.DropStep;
			th_reco_Run_Rbt_Load_Clip.LdClipStep = Run_Rbt_Load_Clip.LdClipStep;
			th_reco_Run_Rbt_Load_Clip.ClipLotStep = Run_Rbt_Load_Clip.ClipLotStep;
			th_reco_Run_Rbt_Load_Clip.PickClipStep = Run_Rbt_Load_Clip.PickClipStep;
			th_reco_Run_Rbt_Load_Clip.UldClipStep = Run_Rbt_Load_Clip.UldClipStep;
			th_reco_Run_Rbt_Load_Clip.mn_pre_move_flag = Run_Rbt_Load_Clip.mn_pre_move_flag;
			th_reco_Run_Rbt_Load_Clip.mn_pre_insert_flag = Run_Rbt_Load_Clip.mn_pre_insert_flag;
			th_reco_Run_Rbt_Load_Clip.mn_pre_clamp_flag = Run_Rbt_Load_Clip.mn_pre_clamp_flag;
			for(i=0; i<D_INFOSIZE; i++)
			{
				th_reco_Run_Rbt_Load_Clip.ActionPicker[i] = Run_Rbt_Load_Clip.ActionPicker[i];		
			}
			th_reco_Run_Rbt_Load_Clip.mn_Result = Run_Rbt_Load_Clip.mn_Result;
			th_reco_Run_Rbt_Load_Clip.md_CurrentPos = Run_Rbt_Load_Clip.md_CurrentPos;
			th_reco_Run_Rbt_Load_Clip.md_clampPos = Run_Rbt_Load_Clip.md_clampPos;

			th_reco_Run_Rbt_Load_Clip.HeadVal = Run_Rbt_Load_Clip.HeadVal;
			for(i=0; i<10; i++)
			{
				for(j=0; j<10; j++)
				{
					th_reco_Run_Rbt_Load_Clip.mn_change[i][j] = th_reco_Run_Rbt_Load_Clip.mn_change[i][j];
				}
			}
			for(i=0; i<2; i++)
			{
				th_reco_Run_Rbt_Load_Clip.mn_check[i] = Run_Rbt_Load_Clip.mn_check[i];		
			}
			for(i=0; i<10; i++)
			{
				th_reco_Run_Rbt_Load_Clip.dClipCurPos[i] = Run_Rbt_Load_Clip.dClipCurPos[i];		
			}
			th_reco_Run_Rbt_Load_Clip.Clip_remove_Cnt = Run_Rbt_Load_Clip.Clip_remove_Cnt;

			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Rbt_Load_Clip, sizeof(st_thread_Run_Rbt_Load_Clip_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)

					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
						break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}

			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Rbt_Load_Clip, sizeof(st_thread_Run_Rbt_Load_Clip_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
						break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 6:
		sDebug_FileName += "st_thread_Run_Rbt_Load_LeftHeat_param.dat";
		sFileName = sDebug_FileName;
		
		
		if(n_mode == DATA_WRITE) //file write
		{
			th_reco_Run_Rbt_Load_LeftHeat.RunStep = Run_Rbt_Load_LeftHeat.RunStep;
			th_reco_Run_Rbt_Load_LeftHeat.MoveStep = Run_Rbt_Load_LeftHeat.MoveStep;
			th_reco_Run_Rbt_Load_LeftHeat.InitStep = Run_Rbt_Load_LeftHeat.InitStep;
			th_reco_Run_Rbt_Load_LeftHeat.ReinstateStep = Run_Rbt_Load_LeftHeat.ReinstateStep;
			th_reco_Run_Rbt_Load_LeftHeat.SafetyZone_Step = Run_Rbt_Load_LeftHeat.SafetyZone_Step;
			th_reco_Run_Rbt_Load_LeftHeat.Tray_Pick_Step = Run_Rbt_Load_LeftHeat.Tray_Pick_Step;
			th_reco_Run_Rbt_Load_LeftHeat.Module_Place_Step = Run_Rbt_Load_LeftHeat.Module_Place_Step;
			th_reco_Run_Rbt_Load_LeftHeat.Before_Module_Place_Step = Run_Rbt_Load_LeftHeat.Before_Module_Place_Step;
			th_reco_Run_Rbt_Load_LeftHeat.mn_posok = Run_Rbt_Load_LeftHeat.mn_posok;
			th_reco_Run_Rbt_Load_LeftHeat.TrayPlaceStep = Run_Rbt_Load_LeftHeat.TrayPlaceStep;
			th_reco_Run_Rbt_Load_LeftHeat.mn_pre_move_flag = Run_Rbt_Load_LeftHeat.mn_pre_move_flag;
			th_reco_Run_Rbt_Load_LeftHeat.mn_Pickerenable_cnt = Run_Rbt_Load_LeftHeat.mn_Pickerenable_cnt;
			th_reco_Run_Rbt_Load_LeftHeat.mn_FirstPicker_Y_Pos = Run_Rbt_Load_LeftHeat.mn_FirstPicker_Y_Pos;
			th_reco_Run_Rbt_Load_LeftHeat.mn_FirstTray_Y_Pos = Run_Rbt_Load_LeftHeat.mn_FirstTray_Y_Pos;
			th_reco_Run_Rbt_Load_LeftHeat.mn_NextPos = Run_Rbt_Load_LeftHeat.mn_NextPos;
			for(i=0; i<D_INFOSIZE; i++)
			{
				for(j=0; j<PICKER_NUM; j++)
				{
					th_reco_Run_Rbt_Load_LeftHeat.ActionPicker[i][j] = th_reco_Run_Rbt_Load_LeftHeat.ActionPicker[i][j];
					th_reco_Run_Rbt_Load_LeftHeat.mTempPicker[i][j] = th_reco_Run_Rbt_Load_LeftHeat.mTempPicker[i][j];
					th_reco_Run_Rbt_Load_LeftHeat.FingerInfo[i][j] = th_reco_Run_Rbt_Load_LeftHeat.FingerInfo[i][j];
				}
			}
			for(i=0; i<10; i++)
			{
				th_reco_Run_Rbt_Load_LeftHeat.mCurrent_RowPos[i] = th_reco_Run_Rbt_Load_LeftHeat.mCurrent_RowPos[i];
				th_reco_Run_Rbt_Load_LeftHeat.dHSLeftCurPos[i] = th_reco_Run_Rbt_Load_LeftHeat.dHSLeftCurPos[i];
			}
			th_reco_Run_Rbt_Load_LeftHeat.mn_pickerenable_cnt = Run_Rbt_Load_LeftHeat.mn_pickerenable_cnt;
			th_reco_Run_Rbt_Load_LeftHeat.mn_FirstTray_X_Pos = Run_Rbt_Load_LeftHeat.mn_FirstTray_X_Pos;
			th_reco_Run_Rbt_Load_LeftHeat.mn_FirstPicker_X_Pos = Run_Rbt_Load_LeftHeat.mn_FirstPicker_X_Pos;
			th_reco_Run_Rbt_Load_LeftHeat.i_DVCRepickCnt = Run_Rbt_Load_LeftHeat.i_DVCRepickCnt;
			for(i=0; i<PICKER_NUM; i++)
			{
				th_reco_Run_Rbt_Load_LeftHeat.mResult[i] = th_reco_Run_Rbt_Load_LeftHeat.mResult[i];
				th_reco_Run_Rbt_Load_LeftHeat.mn_check[i] = th_reco_Run_Rbt_Load_LeftHeat.mn_check[i];
			}
			for(i=0; i<10; i++)
			{
				for(j=0; j<10; j++)
				{
					th_reco_Run_Rbt_Load_LeftHeat.mn_change[i][j] = th_reco_Run_Rbt_Load_LeftHeat.mn_change[i][j];
				}
			}
			th_reco_Run_Rbt_Load_LeftHeat.mn_Clipretry = Run_Rbt_Load_LeftHeat.mn_Clipretry;
			th_reco_Run_Rbt_Load_LeftHeat.md_AbsPos = Run_Rbt_Load_LeftHeat.md_AbsPos;
			th_reco_Run_Rbt_Load_LeftHeat.md_SafetyPos = Run_Rbt_Load_LeftHeat.md_SafetyPos;
			th_reco_Run_Rbt_Load_LeftHeat.md_CurrentPos = Run_Rbt_Load_LeftHeat.md_CurrentPos;
			th_reco_Run_Rbt_Load_LeftHeat.md_Robot_PlacePos = Run_Rbt_Load_LeftHeat.md_Robot_PlacePos;
			th_reco_Run_Rbt_Load_LeftHeat.md_Robot_Pos = Run_Rbt_Load_LeftHeat.md_Robot_Pos;
			th_reco_Run_Rbt_Load_LeftHeat.md_Robot_TrayPlacePos = Run_Rbt_Load_LeftHeat.md_Robot_TrayPlacePos;
			th_reco_Run_Rbt_Load_LeftHeat.HeadVal = Run_Rbt_Load_LeftHeat.HeadVal;

			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Rbt_Load_LeftHeat, sizeof(st_thread_Run_Rbt_Load_LeftHeat_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Rbt_Load_LeftHeat, sizeof(st_thread_Run_Rbt_Load_LeftHeat_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 7:
		sDebug_FileName += "st_thread_Run_Rbt_Load_Module_param.dat";
		sFileName = sDebug_FileName;
		
		
		if(n_mode == DATA_WRITE) //file write
		{
			th_reco_Run_Rbt_Load_Module.mn_posok = Run_Rbt_Load_Module.mn_posok;
			th_reco_Run_Rbt_Load_Module.PrevStep = Run_Rbt_Load_Module.PrevStep;
			th_reco_Run_Rbt_Load_Module.RunStep = Run_Rbt_Load_Module.RunStep;
			th_reco_Run_Rbt_Load_Module.MoveStep = Run_Rbt_Load_Module.MoveStep;
			th_reco_Run_Rbt_Load_Module.InitStep = Run_Rbt_Load_Module.InitStep;
			th_reco_Run_Rbt_Load_Module.SafetyZone_Step = Run_Rbt_Load_Module.SafetyZone_Step;
			th_reco_Run_Rbt_Load_Module.ReinstateStep = Run_Rbt_Load_Module.ReinstateStep;
			th_reco_Run_Rbt_Load_Module.TrayBackStep = Run_Rbt_Load_Module.TrayBackStep;
			th_reco_Run_Rbt_Load_Module.TrayMoveStep = Run_Rbt_Load_Module.TrayMoveStep;
			th_reco_Run_Rbt_Load_Module.TrayPlaceStep = Run_Rbt_Load_Module.TrayPlaceStep;
			th_reco_Run_Rbt_Load_Module.Tray_Pick_Step = Run_Rbt_Load_Module.Tray_Pick_Step;
			th_reco_Run_Rbt_Load_Module.Module_Place_Step = Run_Rbt_Load_Module.Module_Place_Step;
			th_reco_Run_Rbt_Load_Module.Before_Module_Place_Step = Run_Rbt_Load_Module.Before_Module_Place_Step;
			th_reco_Run_Rbt_Load_Module.mn_pre_move_flag = Run_Rbt_Load_Module.mn_pre_move_flag;
			th_reco_Run_Rbt_Load_Module.mn_pickerenable_cnt = Run_Rbt_Load_Module.mn_pickerenable_cnt;
			th_reco_Run_Rbt_Load_Module.mn_FirstPicker_Y_Pos = Run_Rbt_Load_Module.mn_FirstPicker_Y_Pos;
			th_reco_Run_Rbt_Load_Module.mn_FirstTray_X_Pos = Run_Rbt_Load_Module.mn_FirstTray_X_Pos;
			th_reco_Run_Rbt_Load_Module.mn_FirstTray_Y_Pos = Run_Rbt_Load_Module.mn_FirstTray_Y_Pos;
			for(i=0; i<10; i++)
			{
				th_reco_Run_Rbt_Load_Module.mCurrent_RowPos[i] = th_reco_Run_Rbt_Load_Module.mCurrent_RowPos[i];
				th_reco_Run_Rbt_Load_Module.dLDModuleCurPos[i] = th_reco_Run_Rbt_Load_Module.dLDModuleCurPos[i];
			}
			th_reco_Run_Rbt_Load_Module.mn_FirstPicker_X_Pos = Run_Rbt_Load_Module.mn_FirstPicker_X_Pos;
			th_reco_Run_Rbt_Load_Module.i_PartialPickCnt = Run_Rbt_Load_Module.i_PartialPickCnt;
			th_reco_Run_Rbt_Load_Module.md_CurrentPos = Run_Rbt_Load_Module.md_CurrentPos;
			th_reco_Run_Rbt_Load_Module.i_DVCRepickCnt = Run_Rbt_Load_Module.i_DVCRepickCnt;
			th_reco_Run_Rbt_Load_Module.mn_NextPos = Run_Rbt_Load_Module.mn_NextPos;
			th_reco_Run_Rbt_Load_Module.MDHeadVal = Run_Rbt_Load_Module.MDHeadVal;
			for(i=0; i<10; i++)
			{
				for(j=0; j<10; j++)
				{
					th_reco_Run_Rbt_Load_Module.mn_change[i][j] = th_reco_Run_Rbt_Load_Module.mn_change[i][j];
				}
			}
			for(i=0; i<PICKER_NUM; i++)
			{
				th_reco_Run_Rbt_Load_Module.mn_check[i] = th_reco_Run_Rbt_Load_Module.mn_check[i];
				th_reco_Run_Rbt_Load_Module.mMultiAxis[i] = th_reco_Run_Rbt_Load_Module.mMultiAxis[i];
				th_reco_Run_Rbt_Load_Module.md_AbsPos[i] = th_reco_Run_Rbt_Load_Module.md_AbsPos[i];
				th_reco_Run_Rbt_Load_Module.md_Velocity[i] = th_reco_Run_Rbt_Load_Module.md_Velocity[i];
				th_reco_Run_Rbt_Load_Module.n_pickeroff_bufferoff_flag[i] = th_reco_Run_Rbt_Load_Module.n_pickeroff_bufferoff_flag[i];
				th_reco_Run_Rbt_Load_Module.mResult[i] = th_reco_Run_Rbt_Load_Module.mResult[i];
			}
			th_reco_Run_Rbt_Load_Module.mn_Clipretry = Run_Rbt_Load_Module.mn_Clipretry;
			th_reco_Run_Rbt_Load_Module.md_Robot_Pos = Run_Rbt_Load_Module.md_Robot_Pos;
			th_reco_Run_Rbt_Load_Module.md_Robot_TrayPlacePos = Run_Rbt_Load_Module.md_Robot_TrayPlacePos;
			th_reco_Run_Rbt_Load_Module.md_Robot_PlacePos = Run_Rbt_Load_Module.md_Robot_PlacePos;
			th_reco_Run_Rbt_Load_Module.md_SafetyPos = Run_Rbt_Load_Module.md_SafetyPos;
			th_reco_Run_Rbt_Load_Module.mn_PickLoad = Run_Rbt_Load_Module.mn_PickLoad;
			th_reco_Run_Rbt_Load_Module.HeadVal = Run_Rbt_Load_Module.HeadVal;
			for(i=0; i<D_INFOSIZE; i++)
			{
				for(j=0; j<PICKER_NUM; j++)
				{
					th_reco_Run_Rbt_Load_Module.ActionPicker[i][j] = th_reco_Run_Rbt_Load_Module.ActionPicker[i][j];
					th_reco_Run_Rbt_Load_Module.FingerInfo[i][j] = th_reco_Run_Rbt_Load_Module.FingerInfo[i][j];
					th_reco_Run_Rbt_Load_Module.mTempPicker[i][j] = th_reco_Run_Rbt_Load_Module.mTempPicker[i][j];
				}
			}
			
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Rbt_Load_Module, sizeof(st_thread_Run_Rbt_Load_Module_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Rbt_Load_Module, sizeof(st_thread_Run_Rbt_Load_Module_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 8:
		sDebug_FileName += "st_thread_Run_Rbt_Load_RightHeat_param.dat";
		sFileName = sDebug_FileName;
		
		if(n_mode == DATA_WRITE) //file write
		{
			th_reco_Run_Rbt_Load_RightHeat.RunStep = Run_Rbt_Load_RightHeat.RunStep;
			th_reco_Run_Rbt_Load_RightHeat.MoveStep = Run_Rbt_Load_RightHeat.MoveStep;
			th_reco_Run_Rbt_Load_RightHeat.InitStep = Run_Rbt_Load_RightHeat.InitStep;
			th_reco_Run_Rbt_Load_RightHeat.ReinstateStep = Run_Rbt_Load_RightHeat.ReinstateStep;
			th_reco_Run_Rbt_Load_RightHeat.SafetyZone_Step = Run_Rbt_Load_RightHeat.SafetyZone_Step;
			th_reco_Run_Rbt_Load_RightHeat.Tray_Pick_Step = Run_Rbt_Load_RightHeat.Tray_Pick_Step;
			th_reco_Run_Rbt_Load_RightHeat.Module_Place_Step = Run_Rbt_Load_RightHeat.Module_Place_Step;
			th_reco_Run_Rbt_Load_RightHeat.Before_Module_Place_Step = Run_Rbt_Load_RightHeat.Before_Module_Place_Step;
			th_reco_Run_Rbt_Load_RightHeat.TrayPlaceStep = Run_Rbt_Load_RightHeat.TrayPlaceStep;
			th_reco_Run_Rbt_Load_RightHeat.mn_pre_move_flag = Run_Rbt_Load_RightHeat.mn_pre_move_flag;
			th_reco_Run_Rbt_Load_RightHeat.mn_Pickerenable_cnt = Run_Rbt_Load_RightHeat.mn_Pickerenable_cnt;
			th_reco_Run_Rbt_Load_RightHeat.mn_FirstPicker_Y_Pos = Run_Rbt_Load_RightHeat.mn_FirstPicker_Y_Pos;
			th_reco_Run_Rbt_Load_RightHeat.mn_FirstTray_Y_Pos = Run_Rbt_Load_RightHeat.mn_FirstTray_Y_Pos;
			th_reco_Run_Rbt_Load_RightHeat.md_AbsPos = Run_Rbt_Load_RightHeat.md_AbsPos;
			th_reco_Run_Rbt_Load_RightHeat.md_SafetyPos = Run_Rbt_Load_RightHeat.md_SafetyPos;
			th_reco_Run_Rbt_Load_RightHeat.md_CurrentPos = Run_Rbt_Load_RightHeat.md_CurrentPos;
			th_reco_Run_Rbt_Load_RightHeat.md_Robot_PlacePos = Run_Rbt_Load_RightHeat.md_Robot_PlacePos;
			th_reco_Run_Rbt_Load_RightHeat.md_Robot_Pos = Run_Rbt_Load_RightHeat.md_Robot_Pos;
			th_reco_Run_Rbt_Load_RightHeat.md_Robot_TrayPlacePos = Run_Rbt_Load_RightHeat.md_Robot_TrayPlacePos;
			th_reco_Run_Rbt_Load_RightHeat.mn_NextPos = Run_Rbt_Load_RightHeat.mn_NextPos;
			th_reco_Run_Rbt_Load_RightHeat.mn_FirstTray_X_Pos = Run_Rbt_Load_RightHeat.mn_FirstTray_X_Pos;
			th_reco_Run_Rbt_Load_RightHeat.mn_FirstPicker_X_Pos = Run_Rbt_Load_RightHeat.mn_FirstPicker_X_Pos;
			th_reco_Run_Rbt_Load_RightHeat.i_DVCRepickCnt = Run_Rbt_Load_RightHeat.i_DVCRepickCnt;
			th_reco_Run_Rbt_Load_RightHeat.RHSHeadVal = Run_Rbt_Load_RightHeat.RHSHeadVal;
			th_reco_Run_Rbt_Load_RightHeat.mn_Clipretry = Run_Rbt_Load_RightHeat.mn_Clipretry;
			th_reco_Run_Rbt_Load_RightHeat.HeadVal = Run_Rbt_Load_RightHeat.HeadVal;
			th_reco_Run_Rbt_Load_RightHeat.mn_pickerenable_cnt = Run_Rbt_Load_RightHeat.mn_pickerenable_cnt;
			for(i=0; i<D_INFOSIZE; i++)
			{
				for(j=0; j<PICKER_NUM; j++)
				{
					th_reco_Run_Rbt_Load_RightHeat.mTempPicker[i][j] = Run_Rbt_Load_RightHeat.mTempPicker[i][j];
					th_reco_Run_Rbt_Load_RightHeat.ActionPicker[i][j] = Run_Rbt_Load_RightHeat.ActionPicker[i][j];
					th_reco_Run_Rbt_Load_RightHeat.FingerInfo[i][j] = Run_Rbt_Load_RightHeat.FingerInfo[i][j];
				}
			}
			for(i=0; i<PICKER_NUM; i++)
			{
				th_reco_Run_Rbt_Load_RightHeat.mResult[i] = Run_Rbt_Load_RightHeat.mResult[i];
				th_reco_Run_Rbt_Load_RightHeat.mn_check[i] = Run_Rbt_Load_RightHeat.mn_check[i];
			}
			for(i=0; i<10; i++)
			{
				for(j=0; j<10; j++)
				{
					th_reco_Run_Rbt_Load_RightHeat.mn_change[i][j] = Run_Rbt_Load_RightHeat.mn_change[i][j];
				}
			}
			for(i=0; i<10; i++)
			{
				th_reco_Run_Rbt_Load_RightHeat.dHSRightCurPos[i] = Run_Rbt_Load_RightHeat.dHSRightCurPos[i];
				th_reco_Run_Rbt_Load_RightHeat.mCurrent_RowPos[i] = Run_Rbt_Load_RightHeat.mCurrent_RowPos[i];
			}
			
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Rbt_Load_RightHeat, sizeof(st_thread_Run_Rbt_Load_RightHeat_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Rbt_Load_RightHeat, sizeof(st_thread_Run_Rbt_Load_RightHeat_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 9:
		sDebug_FileName += "st_thread_Run_Rbt_Sorter_param.dat";
		sFileName = sDebug_FileName;
		
		
		if(n_mode == DATA_WRITE) //file write
		{
			th_reco_Run_Rbt_Sorter.PrevStep = Run_Rbt_Sorter.PrevStep;
			th_reco_Run_Rbt_Sorter.RunSortStep = Run_Rbt_Sorter.RunSortStep;
			th_reco_Run_Rbt_Sorter.RunUldStep = Run_Rbt_Sorter.RunUldStep;
			th_reco_Run_Rbt_Sorter.MoveStep = Run_Rbt_Sorter.MoveStep;
			th_reco_Run_Rbt_Sorter.InitStep = Run_Rbt_Sorter.InitStep;
			th_reco_Run_Rbt_Sorter.SafetyZone_Step = Run_Rbt_Sorter.SafetyZone_Step;
			th_reco_Run_Rbt_Sorter.UldSafetyZone_Step = Run_Rbt_Sorter.UldSafetyZone_Step;
			th_reco_Run_Rbt_Sorter.ReinstateStep = Run_Rbt_Sorter.ReinstateStep;
			th_reco_Run_Rbt_Sorter.ReinstateStep1 = Run_Rbt_Sorter.ReinstateStep1;
			th_reco_Run_Rbt_Sorter.Module_Pick_Step = Run_Rbt_Sorter.Module_Pick_Step;
			th_reco_Run_Rbt_Sorter.TrayPlace_Step = Run_Rbt_Sorter.TrayPlace_Step;
			th_reco_Run_Rbt_Sorter.UldModule_Pick_Step = Run_Rbt_Sorter.UldModule_Pick_Step;
			th_reco_Run_Rbt_Sorter.Module_Place_Step = Run_Rbt_Sorter.Module_Place_Step;
			th_reco_Run_Rbt_Sorter.mn_StackerPos = Run_Rbt_Sorter.mn_StackerPos;
			th_reco_Run_Rbt_Sorter.mn_NextPos = Run_Rbt_Sorter.mn_NextPos;
			th_reco_Run_Rbt_Sorter.i_DVCSortRepickCnt = Run_Rbt_Sorter.i_DVCSortRepickCnt;
			th_reco_Run_Rbt_Sorter.i_DVCSortReplaceCnt = Run_Rbt_Sorter.i_DVCSortReplaceCnt;
			th_reco_Run_Rbt_Sorter.i_DVCUldRepickCnt = Run_Rbt_Sorter.i_DVCUldRepickCnt;
			th_reco_Run_Rbt_Sorter.i_DVCUldReplaceCnt = Run_Rbt_Sorter.i_DVCUldReplaceCnt;
			th_reco_Run_Rbt_Sorter.mn_bcr_goodbin_cnt = Run_Rbt_Sorter.mn_bcr_goodbin_cnt;
			th_reco_Run_Rbt_Sorter.mn_bcr_rejectbin_cnt = Run_Rbt_Sorter.mn_bcr_rejectbin_cnt;
			th_reco_Run_Rbt_Sorter.mn_Clipretry = Run_Rbt_Sorter.mn_Clipretry;
			th_reco_Run_Rbt_Sorter.md_Robot_X_Pos = Run_Rbt_Sorter.md_Robot_X_Pos;
			th_reco_Run_Rbt_Sorter.md_Robot_Y_Pos = Run_Rbt_Sorter.md_Robot_Y_Pos;
			th_reco_Run_Rbt_Sorter.md_Robot_Z_Pos = Run_Rbt_Sorter.md_Robot_Z_Pos;
			th_reco_Run_Rbt_Sorter.md_UldRobot_X_Pos = Run_Rbt_Sorter.md_UldRobot_X_Pos;
			th_reco_Run_Rbt_Sorter.md_UldRobot_Y_Pos = Run_Rbt_Sorter.md_UldRobot_Y_Pos;
			th_reco_Run_Rbt_Sorter.md_UldRobot_Z_Pos = Run_Rbt_Sorter.md_UldRobot_Z_Pos;
			th_reco_Run_Rbt_Sorter.m_dTargetPos = Run_Rbt_Sorter.m_dTargetPos;
			th_reco_Run_Rbt_Sorter.mn_FirstTray_Y_Pos = Run_Rbt_Sorter.mn_FirstTray_Y_Pos;
			th_reco_Run_Rbt_Sorter.mCurrent_RowPos = Run_Rbt_Sorter.mCurrent_RowPos;
			th_reco_Run_Rbt_Sorter.md_CurrentPos = Run_Rbt_Sorter.md_CurrentPos;
			th_reco_Run_Rbt_Sorter.HeadVal = Run_Rbt_Sorter.HeadVal;
			th_reco_Run_Rbt_Sorter.mResult[LDULD_PARA] = Run_Rbt_Sorter.mResult[LDULD_PARA];
			th_reco_Run_Rbt_Sorter.mUldResult[LDULD_PARA] = Run_Rbt_Sorter.mUldResult[LDULD_PARA];
 			for(i=0; i<D_INFOSIZE; i++)
			{
				th_reco_Run_Rbt_Sorter.TempPicker[i] = Run_Rbt_Sorter.TempPicker[i]; 
				th_reco_Run_Rbt_Sorter.mTempPicker[i] = Run_Rbt_Sorter.mTempPicker[i]; 
				th_reco_Run_Rbt_Sorter.UldTempPicker[i] = Run_Rbt_Sorter.UldTempPicker[i]; 
				th_reco_Run_Rbt_Sorter.mUldTempPicker[i] = Run_Rbt_Sorter.mUldTempPicker[i]; 
				th_reco_Run_Rbt_Sorter.mlp_AxisNum[i] = Run_Rbt_Sorter.mlp_AxisNum[i]; 
				th_reco_Run_Rbt_Sorter.msp_AxisNum[i] = Run_Rbt_Sorter.msp_AxisNum[i]; 
			}
			for(i=0; i<2; i++)
			{
				th_reco_Run_Rbt_Sorter.mn_stacker_updn_cyliner[i] = Run_Rbt_Sorter.mn_stacker_updn_cyliner[i]; 
				th_reco_Run_Rbt_Sorter.mn_check[i] = Run_Rbt_Sorter.mn_check[i]; 
				th_reco_Run_Rbt_Sorter.dAbsPos[i] = Run_Rbt_Sorter.dAbsPos[i]; 
				th_reco_Run_Rbt_Sorter.dUldAbsPos[i] = Run_Rbt_Sorter.dUldAbsPos[i]; 
				th_reco_Run_Rbt_Sorter.md_SortAbsPos[i] = Run_Rbt_Sorter.md_SortAbsPos[i]; 
				th_reco_Run_Rbt_Sorter.md_UldAbsPos[i] = Run_Rbt_Sorter.md_UldAbsPos[i]; 
				th_reco_Run_Rbt_Sorter.mn_pre_move_flag[i] = Run_Rbt_Sorter.mn_pre_move_flag[i]; 
			}
			for(i=0; i<10; i++)
			{
				th_reco_Run_Rbt_Sorter.dUldCurPos[i] = Run_Rbt_Sorter.dUldCurPos[i]; 
				th_reco_Run_Rbt_Sorter.dSorterCurPos[i] = Run_Rbt_Sorter.dSorterCurPos[i]; 
			}
			for(i=0; i<D_INFOSIZE; i++)
			{
				for(j=0; j<PICKER_NUM; j++)
				{
					th_reco_Run_Rbt_Sorter.mn_change[i][j] = Run_Rbt_Sorter.mn_change[i][j];
				}
			}
			for(i=0; i<2; i++)
			{
				for(j=0; j<D_INFOSIZE; j++)
				{
					th_reco_Run_Rbt_Sorter.ActionPicker[i][j] = Run_Rbt_Sorter.ActionPicker[i][j];
				}
			}
			
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Rbt_Sorter, sizeof(st_thread_Run_Rbt_Sorter_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Rbt_Sorter, sizeof(st_thread_Run_Rbt_Sorter_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 10:
		sDebug_FileName += "st_thread_Run_Rbt_VisionLabel_param.dat";
		sFileName = sDebug_FileName;
		
		
		if(n_mode == DATA_WRITE) //file write
		{
			for(i=0; i<2; i++)
			{
				th_reco_Run_Rbt_VisionLabel.mn_VisionStep[i] = Run_Rbt_VisionLabel.mn_VisionStep[i];
				th_reco_Run_Rbt_VisionLabel.mn_VisionRbt_WorkingPos_Site[i] = Run_Rbt_VisionLabel.mn_VisionRbt_WorkingPos_Site[i];
				th_reco_Run_Rbt_VisionLabel.m_nTrargetPos[i] = Run_Rbt_VisionLabel.m_nTrargetPos[i];
				th_reco_Run_Rbt_VisionLabel.m_nZPos[i] = Run_Rbt_VisionLabel.m_nZPos[i];
				th_reco_Run_Rbt_VisionLabel.m_dMovePos_1[i] = Run_Rbt_VisionLabel.m_dMovePos_1[i];
				th_reco_Run_Rbt_VisionLabel.m_dMovePos_2[i] = Run_Rbt_VisionLabel.m_dMovePos_2[i];
				th_reco_Run_Rbt_VisionLabel.m_buffPos[i] = Run_Rbt_VisionLabel.m_buffPos[i];
				th_reco_Run_Rbt_VisionLabel.m_buffStatus[i] = Run_Rbt_VisionLabel.m_buffStatus[i];
				th_reco_Run_Rbt_VisionLabel.mn_check[i] = Run_Rbt_VisionLabel.mn_check[i];
			}
			th_reco_Run_Rbt_VisionLabel.PrevStep = Run_Rbt_VisionLabel.PrevStep;
			th_reco_Run_Rbt_VisionLabel.Run_labelStep = Run_Rbt_VisionLabel.Run_labelStep;
			th_reco_Run_Rbt_VisionLabel.Run_Vision1 = Run_Rbt_VisionLabel.Run_Vision1;
			th_reco_Run_Rbt_VisionLabel.Run_Vision2 = Run_Rbt_VisionLabel.Run_Vision2;
			th_reco_Run_Rbt_VisionLabel.MoveStep = Run_Rbt_VisionLabel.MoveStep;
			th_reco_Run_Rbt_VisionLabel.InitStep = Run_Rbt_VisionLabel.InitStep;
			th_reco_Run_Rbt_VisionLabel.ReinstateStep = Run_Rbt_VisionLabel.ReinstateStep;
			th_reco_Run_Rbt_VisionLabel.mn_retry = Run_Rbt_VisionLabel.mn_retry;
			th_reco_Run_Rbt_VisionLabel.m_nSite = Run_Rbt_VisionLabel.m_nSite;
			th_reco_Run_Rbt_VisionLabel.m_nLabelNum = Run_Rbt_VisionLabel.m_nLabelNum;
			th_reco_Run_Rbt_VisionLabel.m_nVisChkNum = Run_Rbt_VisionLabel.m_nVisChkNum;
			th_reco_Run_Rbt_VisionLabel.m_nPick_Label = Run_Rbt_VisionLabel.m_nPick_Label;
			th_reco_Run_Rbt_VisionLabel.mn_printnum = Run_Rbt_VisionLabel.mn_printnum;
			th_reco_Run_Rbt_VisionLabel.m_nOutPrint = Run_Rbt_VisionLabel.m_nOutPrint;
			th_reco_Run_Rbt_VisionLabel.m_inspect = Run_Rbt_VisionLabel.m_inspect;
			th_reco_Run_Rbt_VisionLabel.m_nPick = Run_Rbt_VisionLabel.m_nPick;
			th_reco_Run_Rbt_VisionLabel.m_nOrder = Run_Rbt_VisionLabel.m_nOrder;
			th_reco_Run_Rbt_VisionLabel.m_dTargetLabel = Run_Rbt_VisionLabel.m_dTargetLabel;
			th_reco_Run_Rbt_VisionLabel.mn_Safety_Flag = Run_Rbt_VisionLabel.mn_Safety_Flag;
			th_reco_Run_Rbt_VisionLabel.mn_attach_err = Run_Rbt_VisionLabel.mn_attach_err;
			th_reco_Run_Rbt_VisionLabel.md_offset = Run_Rbt_VisionLabel.md_offset;
			th_reco_Run_Rbt_VisionLabel.mn_visionretry = Run_Rbt_VisionLabel.mn_visionretry;
			th_reco_Run_Rbt_VisionLabel.n_bcrlot_step = Run_Rbt_VisionLabel.n_bcrlot_step;
			th_reco_Run_Rbt_VisionLabel.mn_singleprint = Run_Rbt_VisionLabel.mn_singleprint;
			th_reco_Run_Rbt_VisionLabel.mn_emptyprint = Run_Rbt_VisionLabel.mn_emptyprint;
			th_reco_Run_Rbt_VisionLabel.mn_labelprint = Run_Rbt_VisionLabel.mn_labelprint;
			for(i=0; i<3; i++)
			{
				th_reco_Run_Rbt_VisionLabel.backuptime[i] = Run_Rbt_VisionLabel.backuptime[i];
				th_reco_Run_Rbt_VisionLabel.NetworkConnectTime[i] = Run_Rbt_VisionLabel.NetworkConnectTime[i];
				th_reco_Run_Rbt_VisionLabel.NetworkingTime[i] = Run_Rbt_VisionLabel.NetworkingTime[i];
			}
			for(i=0; i<10; i++)
			{
				th_reco_Run_Rbt_VisionLabel.dVisionCurPos[i] = Run_Rbt_VisionLabel.dVisionCurPos[i];
			}
			for(i=0; i<100; i++)
			{
				th_reco_Run_Rbt_VisionLabel.mn_vis_result[i] = Run_Rbt_VisionLabel.mn_vis_result[i];
			}
			for(i=0; i<10; i++)
			{
				for(j=0; j<10; j++)
				{
					th_reco_Run_Rbt_VisionLabel.mn_change[i][j] = Run_Rbt_VisionLabel.mn_change[i][j];
				}
			}

			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Rbt_VisionLabel, sizeof(st_thread_Run_Rbt_VisionLabel_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Rbt_VisionLabel, sizeof(st_thread_Run_Rbt_VisionLabel_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 11:
		sDebug_FileName += "st_thread_Run_Rbt_Work_Buffer_param.dat";
		sFileName = sDebug_FileName;
		
		
		if(n_mode == DATA_WRITE) //file write
		{
			th_reco_Run_Rbt_Work_Buffer.MoveStep = Run_Rbt_Work_Buffer.MoveStep;
			th_reco_Run_Rbt_Work_Buffer.InitStep = Run_Rbt_Work_Buffer.InitStep;
			th_reco_Run_Rbt_Work_Buffer.ReinstateStep = Run_Rbt_Work_Buffer.ReinstateStep;
			th_reco_Run_Rbt_Work_Buffer.RunBuff1Step = Run_Rbt_Work_Buffer.RunBuff1Step;
			th_reco_Run_Rbt_Work_Buffer.RunBuff2Step = Run_Rbt_Work_Buffer.RunBuff2Step;
			th_reco_Run_Rbt_Work_Buffer.RunBuff3Step = Run_Rbt_Work_Buffer.RunBuff3Step;
			th_reco_Run_Rbt_Work_Buffer.RunBuff4Step = Run_Rbt_Work_Buffer.RunBuff4Step;
			th_reco_Run_Rbt_Work_Buffer.HS1Step = Run_Rbt_Work_Buffer.HS1Step;
			th_reco_Run_Rbt_Work_Buffer.HS2Step = Run_Rbt_Work_Buffer.HS2Step;
			th_reco_Run_Rbt_Work_Buffer.HS3Step = Run_Rbt_Work_Buffer.HS3Step;
			th_reco_Run_Rbt_Work_Buffer.HS4Step = Run_Rbt_Work_Buffer.HS4Step;
			th_reco_Run_Rbt_Work_Buffer.mn_Clipretry = Run_Rbt_Work_Buffer.mn_Clipretry;
			th_reco_Run_Rbt_Work_Buffer.mn_retry = Run_Rbt_Work_Buffer.mn_retry;
			for(i=0; i<3; i++)
			{
				th_reco_Run_Rbt_Work_Buffer.backuptime[i] = Run_Rbt_Work_Buffer.backuptime[i];
			}
			for(i=0; i<2; i++)
			{
				th_reco_Run_Rbt_Work_Buffer.mn_check[i] = Run_Rbt_Work_Buffer.mn_check[i];
			}
			for(i=0; i<4; i++)
			{
				th_reco_Run_Rbt_Work_Buffer.mn_clipretry[i] = Run_Rbt_Work_Buffer.mn_clipretry[i];
			}
			for(i=0; i<10; i++)
			{
				th_reco_Run_Rbt_Work_Buffer.dWorkBufferCurPos[i] = Run_Rbt_Work_Buffer.dWorkBufferCurPos[i];
			}
			for(i=0; i<10; i++)
			{
				for(j=0; j<10; j++)
				{
					th_reco_Run_Rbt_Work_Buffer.mn_change[i][j] = Run_Rbt_Work_Buffer.mn_change[i][j];
				}
			}
				
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Rbt_Work_Buffer, sizeof(st_thread_Run_Rbt_Work_Buffer_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Rbt_Work_Buffer, sizeof(st_thread_Run_Rbt_Work_Buffer_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 12:
		sDebug_FileName += "st_thread_Run_Stacker_Load_Clip_param.dat";
		sFileName = sDebug_FileName;
		
		
		if(n_mode == DATA_WRITE) //file write
		{
			th_reco_Run_Stacker_Load_Clip.PrevStep = Run_Stacker_Load_Clip.PrevStep;
			th_reco_Run_Stacker_Load_Clip.RunStep = Run_Stacker_Load_Clip.RunStep;
			th_reco_Run_Stacker_Load_Clip.RunTransStep = Run_Stacker_Load_Clip.RunTransStep;
			th_reco_Run_Stacker_Load_Clip.MoveStep = Run_Stacker_Load_Clip.MoveStep;
			th_reco_Run_Stacker_Load_Clip.InitStep = Run_Stacker_Load_Clip.InitStep;
			th_reco_Run_Stacker_Load_Clip.PushStep = Run_Stacker_Load_Clip.PushStep;
			th_reco_Run_Stacker_Load_Clip.TransUpDnStep = Run_Stacker_Load_Clip.TransUpDnStep;
			th_reco_Run_Stacker_Load_Clip.ClipOutStep = Run_Stacker_Load_Clip.ClipOutStep;
			th_reco_Run_Stacker_Load_Clip.FixStep = Run_Stacker_Load_Clip.FixStep;
			th_reco_Run_Stacker_Load_Clip.RotatorStep = Run_Stacker_Load_Clip.RotatorStep;
			th_reco_Run_Stacker_Load_Clip.ReinstateStep = Run_Stacker_Load_Clip.ReinstateStep;
			th_reco_Run_Stacker_Load_Clip.RunInsertStep = Run_Stacker_Load_Clip.RunInsertStep;
			th_reco_Run_Stacker_Load_Clip.Change_step = Run_Stacker_Load_Clip.Change_step;
			th_reco_Run_Stacker_Load_Clip.MoveTransferStep = Run_Stacker_Load_Clip.MoveTransferStep;
			th_reco_Run_Stacker_Load_Clip.RunLdClipStep = Run_Stacker_Load_Clip.RunLdClipStep;
			th_reco_Run_Stacker_Load_Clip.RunUldClipStep = Run_Stacker_Load_Clip.RunUldClipStep;
			th_reco_Run_Stacker_Load_Clip.RunLdTubeStep = Run_Stacker_Load_Clip.RunLdTubeStep;
			th_reco_Run_Stacker_Load_Clip.RunUldTubeStep = Run_Stacker_Load_Clip.RunUldTubeStep;
			th_reco_Run_Stacker_Load_Clip.mn_LdTube = Run_Stacker_Load_Clip.mn_LdTube;
			th_reco_Run_Stacker_Load_Clip.mn_LdRotateTube = Run_Stacker_Load_Clip.mn_LdRotateTube;
			th_reco_Run_Stacker_Load_Clip.mn_UnloadTubeNum = Run_Stacker_Load_Clip.mn_UnloadTubeNum;
			th_reco_Run_Stacker_Load_Clip.mn_retry = Run_Stacker_Load_Clip.mn_retry;
			th_reco_Run_Stacker_Load_Clip.mn_change_clip = Run_Stacker_Load_Clip.mn_change_clip;
			th_reco_Run_Stacker_Load_Clip.mn_Tuberetry = Run_Stacker_Load_Clip.mn_Tuberetry;
			th_reco_Run_Stacker_Load_Clip.mn_Ldretry = Run_Stacker_Load_Clip.mn_Ldretry;
			th_reco_Run_Stacker_Load_Clip.mn_Uldretry = Run_Stacker_Load_Clip.mn_Uldretry;
			for(i=0; i<10; i++)
			{
				th_reco_Run_Stacker_Load_Clip.dClipStackerCurPos[i] = Run_Stacker_Load_Clip.dClipStackerCurPos[i];
			}
			for(i=0; i<10; i++)
			{
				for(j=0; j<10; j++)
				{
					th_reco_Run_Stacker_Load_Clip.mn_change[i][j] = Run_Stacker_Load_Clip.mn_change[i][j];
				}
			}
			for(i=0; i<2; i++)
			{
				th_reco_Run_Stacker_Load_Clip.mn_check[i] = Run_Stacker_Load_Clip.mn_check[i];
			}

			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Stacker_Load_Clip, sizeof(st_thread_Run_Stacker_Load_Clip_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Stacker_Load_Clip, sizeof(st_thread_Run_Stacker_Load_Clip_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 13:
		sDebug_FileName += "st_thread_Run_Stacker_Load_Module_param.dat";
		sFileName = sDebug_FileName;
		
		
		if(n_mode == DATA_WRITE) //file write
		{
			th_reco_Run_Stacker_Load_Module.RunS1Step = Run_Stacker_Load_Module.RunS1Step;
			th_reco_Run_Stacker_Load_Module.RunS2Step = Run_Stacker_Load_Module.RunS2Step;
			th_reco_Run_Stacker_Load_Module.RunMStep = Run_Stacker_Load_Module.RunMStep;
			th_reco_Run_Stacker_Load_Module.MoveStep = Run_Stacker_Load_Module.MoveStep;
			th_reco_Run_Stacker_Load_Module.InitStep = Run_Stacker_Load_Module.InitStep;
			th_reco_Run_Stacker_Load_Module.RunTransStep = Run_Stacker_Load_Module.RunTransStep;
			th_reco_Run_Stacker_Load_Module.ReinstateStep = Run_Stacker_Load_Module.ReinstateStep;
			th_reco_Run_Stacker_Load_Module.TrayMoveStep = Run_Stacker_Load_Module.TrayMoveStep;
			th_reco_Run_Stacker_Load_Module.TrayBackStep = Run_Stacker_Load_Module.TrayBackStep;
			th_reco_Run_Stacker_Load_Module.mn_stacker_move_flag = Run_Stacker_Load_Module.mn_stacker_move_flag;
			th_reco_Run_Stacker_Load_Module.initial_smove = Run_Stacker_Load_Module.initial_smove;
			th_reco_Run_Stacker_Load_Module.mn_retry = Run_Stacker_Load_Module.mn_retry;
			th_reco_Run_Stacker_Load_Module.init_stacker_move = Run_Stacker_Load_Module.init_stacker_move;
			th_reco_Run_Stacker_Load_Module.init_stacker_1 = Run_Stacker_Load_Module.init_stacker_1;
			th_reco_Run_Stacker_Load_Module.init_stacker_2 = Run_Stacker_Load_Module.init_stacker_2;
			th_reco_Run_Stacker_Load_Module.mn_retry1 = Run_Stacker_Load_Module.mn_retry1;
			th_reco_Run_Stacker_Load_Module.mn_retry2 = Run_Stacker_Load_Module.mn_retry2;
			th_reco_Run_Stacker_Load_Module.mn_ms_retry = Run_Stacker_Load_Module.mn_ms_retry;
			th_reco_Run_Stacker_Load_Module.mn_stacker1_init_chk = Run_Stacker_Load_Module.mn_stacker1_init_chk;
			th_reco_Run_Stacker_Load_Module.mn_stacker2_init_chk = Run_Stacker_Load_Module.mn_stacker2_init_chk;
			th_reco_Run_Stacker_Load_Module.mn_Moving_stacker = Run_Stacker_Load_Module.mn_Moving_stacker;
			th_reco_Run_Stacker_Load_Module.mn_m_uld_stacker = Run_Stacker_Load_Module.mn_m_uld_stacker;
			for(i=0; i<2; i++)
			{
				th_reco_Run_Stacker_Load_Module.mn_check[i] = Run_Stacker_Load_Module.mn_check[i];
				th_reco_Run_Stacker_Load_Module.m_dTargetPos[i] = Run_Stacker_Load_Module.m_dTargetPos[i];
				th_reco_Run_Stacker_Load_Module.m_dNowPos[i] = Run_Stacker_Load_Module.m_dNowPos[i];
				th_reco_Run_Stacker_Load_Module.mn_stacker_updn_cyliner[i] = Run_Stacker_Load_Module.mn_stacker_updn_cyliner[i];
				th_reco_Run_Stacker_Load_Module.mb_pushercylinder_flag[i] = Run_Stacker_Load_Module.mb_pushercylinder_flag[i];
				th_reco_Run_Stacker_Load_Module.mb_updncylinder_flag[i] = Run_Stacker_Load_Module.mb_updncylinder_flag[i];
			}
			for(i=0; i<10; i++)
			{
				for(j=0; j<10; j++)
				{
					th_reco_Run_Stacker_Load_Module.mn_change[i][j] = Run_Stacker_Load_Module.mn_change[i][j];
				}
			}
			for(i=0; i<4; i++)
			{
				th_reco_Run_Stacker_Load_Module.mn_LeakM_LotEnd[i] = Run_Stacker_Load_Module.mn_LeakM_LotEnd[i];
			}
		
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Stacker_Load_Module, sizeof(st_thread_Run_Stacker_Load_Module_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Stacker_Load_Module, sizeof(st_thread_Run_Stacker_Load_Module_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 14:
		sDebug_FileName += "st_thread_Run_Stacker_Load_RightHeat_param.dat";
		sFileName = sDebug_FileName;
		
		
		if(n_mode == DATA_WRITE) //file write
		{
			th_reco_Run_Stacker_Load_RightHeat.PrevStep = Run_Stacker_Load_RightHeat.PrevStep;
			th_reco_Run_Stacker_Load_RightHeat.RunS1Step = Run_Stacker_Load_RightHeat.RunS1Step;
			th_reco_Run_Stacker_Load_RightHeat.RunS2Step = Run_Stacker_Load_RightHeat.RunS2Step;
			th_reco_Run_Stacker_Load_RightHeat.RunMStep = Run_Stacker_Load_RightHeat.RunMStep;
			th_reco_Run_Stacker_Load_RightHeat.MoveStep = Run_Stacker_Load_RightHeat.MoveStep;
			th_reco_Run_Stacker_Load_RightHeat.InitStep = Run_Stacker_Load_RightHeat.InitStep;
			th_reco_Run_Stacker_Load_RightHeat.RunTransStep = Run_Stacker_Load_RightHeat.RunTransStep;
			th_reco_Run_Stacker_Load_RightHeat.ReinstateStep = Run_Stacker_Load_RightHeat.ReinstateStep;
			th_reco_Run_Stacker_Load_RightHeat.TrayMoveStep = Run_Stacker_Load_RightHeat.TrayMoveStep;
			th_reco_Run_Stacker_Load_RightHeat.TrayBackStep = Run_Stacker_Load_RightHeat.TrayBackStep;
			th_reco_Run_Stacker_Load_RightHeat.TraySeperateStep = Run_Stacker_Load_RightHeat.TraySeperateStep;
			th_reco_Run_Stacker_Load_RightHeat.mn_stacker_move_flag = Run_Stacker_Load_RightHeat.mn_stacker_move_flag;
			th_reco_Run_Stacker_Load_RightHeat.mb_sdstop = Run_Stacker_Load_RightHeat.mb_sdstop;
			th_reco_Run_Stacker_Load_RightHeat.init_stacker_move = Run_Stacker_Load_RightHeat.init_stacker_move;
			th_reco_Run_Stacker_Load_RightHeat.init_stacker_1 = Run_Stacker_Load_RightHeat.init_stacker_1;
			th_reco_Run_Stacker_Load_RightHeat.init_stacker_2 = Run_Stacker_Load_RightHeat.init_stacker_2;
			th_reco_Run_Stacker_Load_RightHeat.initial_smove = Run_Stacker_Load_RightHeat.initial_smove;
			th_reco_Run_Stacker_Load_RightHeat.mn_StackerPos = Run_Stacker_Load_RightHeat.mn_StackerPos;
			th_reco_Run_Stacker_Load_RightHeat.mn_retry = Run_Stacker_Load_RightHeat.mn_retry;
			th_reco_Run_Stacker_Load_RightHeat.mb_clampcylinder_flag = Run_Stacker_Load_RightHeat.mb_clampcylinder_flag;
			th_reco_Run_Stacker_Load_RightHeat.mn_retry1 = Run_Stacker_Load_RightHeat.mn_retry1;
			th_reco_Run_Stacker_Load_RightHeat.mn_retry2 = Run_Stacker_Load_RightHeat.mn_retry2;
			th_reco_Run_Stacker_Load_RightHeat.mn_ms_retry = Run_Stacker_Load_RightHeat.mn_ms_retry;
			th_reco_Run_Stacker_Load_RightHeat.mn_stacker1_init_chk = Run_Stacker_Load_RightHeat.mn_stacker1_init_chk;
			th_reco_Run_Stacker_Load_RightHeat.mn_stacker2_init_chk = Run_Stacker_Load_RightHeat.mn_stacker2_init_chk;
			th_reco_Run_Stacker_Load_RightHeat.mn_Moving_stacker = Run_Stacker_Load_RightHeat.mn_Moving_stacker;
			th_reco_Run_Stacker_Load_RightHeat.mn_m_uld_stacker = Run_Stacker_Load_RightHeat.mn_m_uld_stacker;
			for(i=0; i<2; i++)
			{
				th_reco_Run_Stacker_Load_RightHeat.ReadyPosStep[i] = Run_Stacker_Load_RightHeat.ReadyPosStep[i];
				th_reco_Run_Stacker_Load_RightHeat.MotorMoveStep[i] = Run_Stacker_Load_RightHeat.MotorMoveStep[i];
				th_reco_Run_Stacker_Load_RightHeat.m_dTargetPos[i] = Run_Stacker_Load_RightHeat.m_dTargetPos[i];
				th_reco_Run_Stacker_Load_RightHeat.m_dNowPos[i] = Run_Stacker_Load_RightHeat.m_dNowPos[i];
				th_reco_Run_Stacker_Load_RightHeat.mn_stacker_updn_cyliner[i] = Run_Stacker_Load_RightHeat.mn_stacker_updn_cyliner[i];
				th_reco_Run_Stacker_Load_RightHeat.mb_updncylinder_flag[i] = Run_Stacker_Load_RightHeat.mb_updncylinder_flag[i];
				th_reco_Run_Stacker_Load_RightHeat.mb_pushercylinder_flag[i] = Run_Stacker_Load_RightHeat.mb_pushercylinder_flag[i];
				th_reco_Run_Stacker_Load_RightHeat.mn_check[i] = Run_Stacker_Load_RightHeat.mn_check[i];
			}
			for(i=0; i<10; i++)
			{
				for(j=0; j<10; j++)
				{
					th_reco_Run_Stacker_Load_RightHeat.mn_change[i][j] = Run_Stacker_Load_RightHeat.mn_change[i][j];
				}
			}
			for(i=0; i<4; i++)
			{
				th_reco_Run_Stacker_Load_RightHeat.mn_LeakM_LotEnd[i] = Run_Stacker_Load_RightHeat.mn_LeakM_LotEnd[i];
			}

			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Stacker_Load_RightHeat, sizeof(st_thread_Run_Stacker_Load_RightHeat_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Stacker_Load_RightHeat, sizeof(st_thread_Run_Stacker_Load_RightHeat_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 15:
		sDebug_FileName += "st_thread_Run_Stacker_Sorter_param.dat";
		sFileName = sDebug_FileName;
		
		
		if(n_mode == DATA_WRITE) //file write
		{
			th_reco_Run_Stacker_Sorter.RunMStep = Run_Stacker_Sorter.RunMStep;
			th_reco_Run_Stacker_Sorter.MoveStep = Run_Stacker_Sorter.MoveStep;
			th_reco_Run_Stacker_Sorter.InitStep = Run_Stacker_Sorter.InitStep;
			th_reco_Run_Stacker_Sorter.ReinstateStep = Run_Stacker_Sorter.ReinstateStep;
			th_reco_Run_Stacker_Sorter.MotorMoveStep = Run_Stacker_Sorter.MotorMoveStep;
			th_reco_Run_Stacker_Sorter.WorkPosStep_old = Run_Stacker_Sorter.WorkPosStep_old;
			th_reco_Run_Stacker_Sorter.WorkPosStep = Run_Stacker_Sorter.WorkPosStep;
			th_reco_Run_Stacker_Sorter.RunS1Step = Run_Stacker_Sorter.RunS1Step;
			th_reco_Run_Stacker_Sorter.RunS2Step = Run_Stacker_Sorter.RunS2Step;
			th_reco_Run_Stacker_Sorter.RunTransStep = Run_Stacker_Sorter.RunTransStep;
			th_reco_Run_Stacker_Sorter.StackerReayStep = Run_Stacker_Sorter.StackerReayStep;
			th_reco_Run_Stacker_Sorter.TrayMoveStep = Run_Stacker_Sorter.TrayMoveStep;
			th_reco_Run_Stacker_Sorter.TraySeperateStep = Run_Stacker_Sorter.TraySeperateStep;
			th_reco_Run_Stacker_Sorter.TrayBackStep = Run_Stacker_Sorter.TrayBackStep;
			th_reco_Run_Stacker_Sorter.mn_StackerPos = Run_Stacker_Sorter.mn_StackerPos;
			th_reco_Run_Stacker_Sorter.mn_StackerTrayReady = Run_Stacker_Sorter.mn_StackerTrayReady;
			th_reco_Run_Stacker_Sorter.mn_stacker_move_flag = Run_Stacker_Sorter.mn_stacker_move_flag;
			th_reco_Run_Stacker_Sorter.initial_smove = Run_Stacker_Sorter.initial_smove;
			th_reco_Run_Stacker_Sorter.mn_retry = Run_Stacker_Sorter.mn_retry;
			th_reco_Run_Stacker_Sorter.dTargetPos = Run_Stacker_Sorter.dTargetPos;
			th_reco_Run_Stacker_Sorter.mResult[0] = Run_Stacker_Sorter.mResult[0];
			th_reco_Run_Stacker_Sorter.init_stacker_move = Run_Stacker_Sorter.init_stacker_move;
			th_reco_Run_Stacker_Sorter.init_stacker_1 = Run_Stacker_Sorter.init_stacker_1;
			th_reco_Run_Stacker_Sorter.init_stacker_2 = Run_Stacker_Sorter.init_stacker_2;
			th_reco_Run_Stacker_Sorter.md_CurrentPos = Run_Stacker_Sorter.md_CurrentPos;
			th_reco_Run_Stacker_Sorter.mn_retry1 = Run_Stacker_Sorter.mn_retry1;
			th_reco_Run_Stacker_Sorter.mn_retry2 = Run_Stacker_Sorter.mn_retry2;
			th_reco_Run_Stacker_Sorter.mn_ms_retry = Run_Stacker_Sorter.mn_ms_retry;
			th_reco_Run_Stacker_Sorter.mn_stacker1_init_chk = Run_Stacker_Sorter.mn_stacker1_init_chk;
			th_reco_Run_Stacker_Sorter.mn_stacker2_init_chk = Run_Stacker_Sorter.mn_stacker2_init_chk;
			th_reco_Run_Stacker_Sorter.mn_Moving_stacker = Run_Stacker_Sorter.mn_Moving_stacker;
			th_reco_Run_Stacker_Sorter.mn_m_uld_stacker = Run_Stacker_Sorter.mn_m_uld_stacker;
			for(i=0; i<2; i++)
			{
				th_reco_Run_Stacker_Sorter.ReadyPosStep_old[i] = Run_Stacker_Sorter.ReadyPosStep_old[i];
				th_reco_Run_Stacker_Sorter.ReadyPosStep[i] = Run_Stacker_Sorter.ReadyPosStep[i];
				th_reco_Run_Stacker_Sorter.m_dTargetPos[i] = Run_Stacker_Sorter.m_dTargetPos[i];
				th_reco_Run_Stacker_Sorter.m_dNowPos[i] = Run_Stacker_Sorter.m_dNowPos[i];
				th_reco_Run_Stacker_Sorter.mn_check[i] = Run_Stacker_Sorter.mn_check[i];
				th_reco_Run_Stacker_Sorter.mb_updncylinder_flag[i] = Run_Stacker_Sorter.mb_updncylinder_flag[i];
				th_reco_Run_Stacker_Sorter.mn_stacker_updn_cyliner[i] = Run_Stacker_Sorter.mn_stacker_updn_cyliner[i];
			}
			for(i=0; i<10; i++)
			{
				for(j=0; j<10; j++)
				{
					th_reco_Run_Stacker_Sorter.mn_change[i][j] = Run_Stacker_Sorter.mn_change[i][j];
				}
			}
			for(i=0; i<4; i++)
			{
				th_reco_Run_Stacker_Sorter.mn_LeakM_LotEnd[i] = Run_Stacker_Sorter.mn_LeakM_LotEnd[i];
			}
	
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&th_reco_Run_Stacker_Sorter, sizeof(st_thread_Run_Stacker_Sorter_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&th_reco_Run_Stacker_Sorter, sizeof(st_thread_Run_Stacker_Sorter_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	case 16:	// 
		CreateFolder(sDebug_FileName);
		
		sDebug_FileName += "st_interface.dat";
		sFileName = sDebug_FileName;
		
		
		if(n_mode == DATA_WRITE) //file write
		{
			if((fp=fopen(sFileName,"wb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					
					//						st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fwrite(&st_interface, sizeof(st_interface_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
						
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Write Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		else if(n_mode == DATA_READ) //file read
		{
			nExistFile = access(sFileName, 0);	
			if (nExistFile)
			{
				st_other.str_abnormal_msg.Format("[%s] file is not exist.", sFileName);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				nFuncRet = CTL_NO;
				break;
			}
			
			if((fp=fopen(sFileName,"rb")) == NULL)
			{			
				if (st_handler.cwnd_list != NULL)
				{
					st_other.str_abnormal_msg.Format("Fail in Creating [%s] FILE.", sFileName);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
				}				
				nFuncRet = CTL_NO;
				break;
			}
			else
			{ //정상 
				fread(&st_interface, sizeof(st_interface_param), 1, fp);
				if (ferror(fp)) 
				{
					if (st_handler.cwnd_list != NULL)
					{
						st_other.str_abnormal_msg.Format("[%s] FILE Read Error.", sFileName);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					clearerr(fp);
					nFuncRet = CTL_NO;
					break;
				}	
				fclose(fp);
			}		
		}
		break;

	}

	return nFuncRet;
}

void CPublic_Function::OnWriteProduct()
{
	if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
	{
		CString strLog;
		strLog.Format("Product per hour : %d, Total: %d\r\n", st_work.mn_output_dvc, st_work.nMdlPassCount[0][0]);
		Func.On_LogFile_Add(LOG_UPH_TOT, strLog);
		st_work.mn_output_dvc = 0;
	}
}
////