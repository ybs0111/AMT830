// AVersion.cpp: implementation of the AVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "AVersion.h"

#include "AMTRegistry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <string>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
AVersion g_ver;

AVersion::AVersion()
{
	m_strUpdate = "Update Desc...";
}

AVersion::~AVersion()
{

}

void AVersion::CheckVersion()
{
	std::string strVer_old = AMTRegistry::RegReadString( REG_KEY_VER, REG_VALUE_CREATE_VER, 0 );
	std::string strDate_old = AMTRegistry::RegReadString( REG_KEY_VER, REG_VALUE_CREATE_DATE, 0 );
	
	if( st_handler.mstr_handler_version != strVer_old.c_str() ||
		st_handler.mstr_handler_date != strDate_old.c_str() )
	{
		CString strMsg;
		strMsg.Format( "FUNCTION=VERSION_UPDATE EQP_ID=%s MODEL=\"PCT SORTER\" VERSION=\"%s_%s\" DESC=\"%s\"", st_lamp.mstr_equip_id, st_handler.mstr_handler_version,
			st_handler.mstr_handler_date, m_strUpdate );
		
//		Run_Network.PushSendMsg_BPC( (LPCSTR)strMsg, 5, SENDMSG_TYPE_HEAD_16 );
//		Run_Network.PushSendMsg_BPC((LPCSTR)strMsg);
	}
}

void AVersion::UpdateVersion()
{
	AMTRegistry::RegWriteString( REG_KEY_VER, REG_VALUE_CREATE_VER, (LPCSTR)st_handler.mstr_handler_version );
	AMTRegistry::RegWriteString( REG_KEY_VER, REG_VALUE_CREATE_DATE, (LPCSTR)st_handler.mstr_handler_date );
}

void AVersion::SetVersion()
{
	CString str_temp;
	st_handler.mstr_handler_name = "AMT830";
	st_handler.mstr_handler_version = "Ver 1.0.0";
	str_temp = _T("2017,0223");
	st_handler.mstr_handler_date = str_temp;

	//m_strUpdate = "SESS Setup";

	CheckVersion();
}