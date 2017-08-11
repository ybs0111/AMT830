// AMTLotManager.cpp: implementation of the AMTLotManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "AMTLotManager.h"
#include "Public_Function.h"

#include "CtlBd_Function.h"
#include "CtlBd_Library.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AMTLotManager g_AMTLotMgr;

LotResult::LotResult()
{
	Initialize();
}

void LotResult::Initialize()
{
	for( int i = WORK_PRIME; i < WORK_MAX; i++ )
	{
		// Bin Test
		m_iBinCount_Good[i] = 0;
		m_iBinCount_Fail[i] = 0;
		m_iBinCount_NTO[i] = 0;
		m_iBinCount_BTO[i] = 0;
		m_iBinCount_HTO[i] = 0;
		
		// Test Count
		m_iTestCount_Load[i] = 0;
		m_iTestCount_Good[i] = 0;
		m_iTestCount_Fail[i] = 0;
	}

	m_iBootTestCount_Load = 0;
	m_iBootTestCount_Pass = 0;
	m_iBootTestCount_Load_Real = 0;
	m_iBootTestCount_Pass_Real = 0;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
LotSite::LotSite()
{
	Initialize();
}

LotSite::LotSite( DWORD dwTestedTime )
{
	Initialize();
	m_dwTestedTime = dwTestedTime;
}

void LotSite::Initialize()
{
	m_dwTestedTime = 0;
}


DWORD LotSite::GetTestedTime()
{
	// strTime.Format( "%02d: %02d", iMin, iSec );
	return m_dwTestedTime;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
AMTLot::AMTLot()
{
	m_strLotName = "";
	Initialize();
}

AMTLot::AMTLot( CString strLotName )
{
	m_strLotName = strLotName;
	Initialize();
}

void AMTLot::Initialize()
{
	m_LotResult.Initialize();
	m_strTimeStart = "";
	m_strTimeFinish = "";
	m_dwTimeStart = 0;
	m_dwTimeFinish = 0;
}

void AMTLot::WriteStartTime()
{
	if( m_strTimeStart != "" )
		return;

	m_strTimeStart = Func.GetStrCurrentTime(); 
	m_dwTimeStart = GetCurrentTime();

	if( st_handler.cwnd_main != NULL )
	{
		st_handler.cwnd_main->PostMessage( WM_DRAW_LOT_TIME, 0, 0 );
	}
}

void AMTLot::WriteFinishTime()
{
	m_strTimeFinish = Func.GetStrCurrentTime(); 
	m_dwTimeFinish = GetCurrentTime();

	if( st_handler.cwnd_main != NULL )
	{
		st_handler.cwnd_main->PostMessage( WM_DRAW_LOT_TIME, 0, 0 );
	}

	st_work.ml_last_shifter_move_time = 0;
}

void AMTLot::AddTestedSite( DWORD dwTestedTime )
{
	LotSite site( dwTestedTime );
	m_vecSites.push_back( site );

	if( st_handler.cwnd_main != NULL )
	{
		st_handler.cwnd_main->PostMessage( WM_DRAW_LOT_TIME, 0, 0 );
	}
}

CString AMTLot::GetStrTime( DWORD dwTime )
{
	int iMin, iSec;
	iMin = dwTime / 60000;
	iSec = (dwTime % 60000) / 1000;
	
	CString strRtn;
	strRtn.Format( "%02d:%02d", iMin, iSec );
	return strRtn;
}

CString AMTLot::GetStrTestedSitesTimeMin()
{
	int iCnt = m_vecSites.size();
	DWORD dwMin = 0;
	for( int i=0; i<iCnt; i++ )
	{
		DWORD dwTime = m_vecSites.at(i).GetTestedTime();
		if( (dwMin == 0) || ( dwMin > dwTime ) )
			dwMin = dwTime;
	}

	return GetStrTime( dwMin );
}

CString AMTLot::GetStrTestedSitesTimeMax()
{
	int iCnt = m_vecSites.size();
	DWORD dwMax = 0;
	for( int i=0; i<iCnt; i++ )
	{
		DWORD dwTime = m_vecSites.at(i).GetTestedTime();
		if( dwMax < dwTime )
			dwMax = dwTime;
	}

	return GetStrTime( dwMax );
}

CString AMTLot::GetStrTestedSitesTimeAvr()
{
	int iCnt = m_vecSites.size();
	DWORD dwTotal = 0, dwAvr = 0;
	for( int i=0; i<iCnt; i++ )
	{
		DWORD dwTime = m_vecSites.at(i).GetTestedTime();
		dwTotal += dwTime;
	}

	if( iCnt > 0 )
		dwAvr = dwTotal / iCnt;

	return GetStrTime( dwAvr );
}

CString AMTLot::GetStrUPH_Pass()
{
	CString strRtn;
	DWORD dwEndTime;
	float fUPH = 0.0f;

	if( m_dwTimeFinish == 0 )
		dwEndTime = GetCurrentTime();
	else
		dwEndTime = m_dwTimeFinish;

	DWORD dwWorkTime = dwEndTime - m_dwTimeStart;
	if( dwWorkTime != 0 )
		fUPH = (float)m_LotResult.GetBinCountGood(WORK_PRIME) * 60.f * 60.f * 1000.f / (float)dwWorkTime;
	else
		fUPH = 0.f;
	
	strRtn.Format( "%.02f  UPEH", fUPH );
	return strRtn;
}

CString AMTLot::GetStrUPH_Fail()
{
	CString strRtn;
	DWORD dwEndTime;
	float fUPH = 0.0f;

	if( m_dwTimeFinish == 0 )
		dwEndTime = GetCurrentTime();
	else
		dwEndTime = m_dwTimeFinish;

	DWORD dwWorkTime = dwEndTime - m_dwTimeStart;
	if( dwWorkTime != 0 )
		fUPH = (float)m_LotResult.GetBinCountFail(WORK_PRIME) * 60.f * 60.f * 1000.f / (float)dwWorkTime;
	else
		fUPH = 0.f;
	
	strRtn.Format( "%.02f  UPEH", fUPH );
	return strRtn;
}

CString AMTLot::GetStrUPH_Total()
{
	CString strRtn;
	DWORD dwEndTime;
	float fUPH = 0.0f;

	if( m_dwTimeFinish == 0 )
		dwEndTime = GetCurrentTime();
	else
		dwEndTime = m_dwTimeFinish;

	DWORD dwWorkTime = dwEndTime - m_dwTimeStart;
	int iLoad = m_LotResult.GetBinCountGood(WORK_PRIME) + m_LotResult.GetBinCountFail(WORK_PRIME); 
	if( dwWorkTime != 0 )
		fUPH = (float)iLoad * 60.f * 60.f * 1000.f / (float)dwWorkTime;
	else
		fUPH = 0.f;
	
	strRtn.Format( "%.02f  UPEH", fUPH );
	return strRtn;
}

bool AMTLot::CheckPartNo_forDIMM( CString strPartNo )
{
	//VLPDIMM	M392B
	
	//ECCUDIMM	M391B
	//Normal(R)	M393B
	//UDIMM		M378B
	
	if( m_bDimmFailed == true )
		return true;
	
	CString strPart = strPartNo.Left( 5 );
	
	bool bVlpDimm = false;
	bool bRet = false;
	
	if( strPart == "M392B" )
		bVlpDimm = true;
	
	if( bVlpDimm && st_basic.mn_device_type == 1 )
		bRet = true;
	
	if( !bVlpDimm && st_basic.mn_device_type == 0 )
		bRet = true;
	
	if( bRet == false )
	{
		if( strPartNo == "" )
		{
//			CtlBdFunc.OnAlarmStart( "200601" );
		}
		else
		{
//			CtlBdFunc.OnAlarmStart( "200600" );
		}
		
		m_bDimmFailed = true;
	}
	
	return bRet;
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =			
AMTLotManager::AMTLotManager()
{
	Initialize();
}

AMTLotManager::~AMTLotManager()
{

}

void AMTLotManager::Initialize()
{
	// Work Count
	m_iWorkCount_Load = 0;
	m_iWorkCount_Good = 0;
	m_iWorkCount_Fail = 0;
	m_iWorkCount_Alarm = 0;

	m_bCanPushNewLot = true;
	m_bFailed_DimmCheck = false;
}

void AMTLotManager::PushLot( CString strLotName )
{
	if( strLotName == "" )
		return;

	AMTLot lot( strLotName );
	m_vecLot.push_back( lot );

	int iCnt = m_vecLot.size();
	if( iCnt > MAX_LOTINFO )
	{
		std::vector<AMTLot>::iterator iter = m_vecLot.begin();
		m_vecLot.erase(iter);
	}

	// ComboBox
	if( st_handler.cwnd_main != NULL )
	{
		st_handler.cwnd_main->SendMessage( WM_RECEIVED_NEW_LOT, 0, 0 );
	}
}

AMTLot& AMTLotManager::FindLotByName( CString strLotName )
{
	int iCnt = m_vecLot.size();
	for ( int i=(iCnt - 1); i >= 0; i-- )
	{
		if( m_vecLot.at(i).m_strLotName == strLotName )
		{
			return m_vecLot.at(i);
		}
	}
	return m_LotDummy;
}

AMTLot& AMTLotManager::GetLastLot()
{
	int iCnt = m_vecLot.size();
	if( iCnt > 0 )
		return m_vecLot.at(iCnt - 1);

	return m_LotDummy;
}

void AMTLotManager::SetCanPushNewLot()
{
	m_bCanPushNewLot = true;
	Func.On_LogFile_Add(3, "[LASTMODULE] - Received LastModule !!");
}

void AMTLotManager::CheckNewLotNo( CString strLotName )
{
	// 최근 랏이 또 들어오면 알람 발생
	if( GetLastLot().m_strLotName == strLotName )
	{
		sprintf(cJamcode, "200401");
		CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
	}
	
	// 라스트모듈을 받았었는지 확인 후, 안받았으면 알람 발생
	// 앞장비만 
	if (st_handler.mn_machine_pos == 0)
	{
		if( m_bCanPushNewLot == false )
		{
			sprintf(cJamcode, "200400");
			CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
		}
		m_bCanPushNewLot = false;
	}

	if( strLotName == "" )
	{
		sprintf(cJamcode, "200410");
		CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
	}
}

bool AMTLotManager::CheckPartNo_forDIMM( CString strPartNo )
{
	//VLPDIMM	M392B

	//ECCUDIMM	M391B
	//Normal(R)	M393B
	//UDIMM		M378B

	CString strPart = strPartNo.Left( 5 );

	bool bVlpDimm = false;
	bool bRet = false;
	
	if( strPart == "M392B" )
		bVlpDimm = true;

	if( bVlpDimm && st_basic.mn_device_type == 1 )
		bRet = true;

	if( !bVlpDimm && st_basic.mn_device_type == 0 )
		bRet = true;

	if( (bRet == false) && (m_bFailed_DimmCheck == false) )
	{
		sprintf(cJamcode, "200600");
		CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
		m_bFailed_DimmCheck = true;
	}
	
	if( bRet )
		m_bFailed_DimmCheck = false;

	return bRet;
}

bool AMTLotManager::CheckChangeableDIMM()
{

	// 버퍼 및 사이트에 자재가 있으면 안된다.
	for( int iPos = POS_REAR; iPos < POS_COUNT; iPos ++ )
	{
		for( int iSocket = 0; iSocket < SOCKET_COUNT; iSocket++ )
		{
			for( int iIdx = DATAMAP_LDBUFFER; iIdx <= DATAMAP_ULDBUFFER; iIdx++ )
			{
				if( st_module_datamap[iPos][iIdx].mnDeviceExistence[iSocket] == CTL_YES )
				{
					return false;
				}
			}
			
			if( st_module_datamap[iPos][DATAMAP_SHIFTER].mnDeviceExistence[iSocket] == CTL_YES )
			{
				return false;
			}

		}
	}

	return true;
}