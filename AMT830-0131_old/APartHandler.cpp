// APartHandler.cpp: implementation of the APartHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APartHandler.h"
//#include "../IO_Manager.h"
//#include "../ComizoaPublic.h"
//#include "APartHandler_COK.h"
//#include "APartTray.h"
//#include "APartRobot.h"
//#include "APartTestSite.h"
//#include "APartNetworkManager.h"
//#include "../AMTLotManager.h"
#include "AMTRegistry.h"

//#include "../SrcBase\ALocalization.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
APartHandler	g_handler;

APartHandler::APartHandler()
{
//	m_empos = EMPOS_FRONT;
//
//	m_step_conv[0] = EC_NONE;
//	m_step_conv[1] = EC_NONE;
//	m_step_conv[2] = EC_NONE;
//
//	m_step_ttrans = ETT_TO_NONE;
//
//	m_epriority_bin = TPOS_NONE;
//	m_esecondary_bin = TPOS_NONE;
//
//	m_step_virtual_supply = EVS_NONE;
//
//	m_nVedio = -1;
//
//	m_bRejAlarm = false;
//
//	m_bReady_SendNext = false;
//
//	m_bRecv_NextMachine = true;

	m_nAlarmCnt = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_ALARM );
}

APartHandler::~APartHandler()
{

}

void APartHandler::AddAlarmCnt()
{
	 m_nAlarmCnt++;
	 AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_ALARM, m_nAlarmCnt );
}

void APartHandler::ClearAlarmCnt()
{
	m_nAlarmCnt = 0;
	 AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_ALARM, m_nAlarmCnt );
}
