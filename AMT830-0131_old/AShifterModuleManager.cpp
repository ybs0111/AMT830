// AShifterModuleManager.cpp: implementation of the AShifterModuleManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "AShifterModuleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

AShifterModuleManager g_ShifterModule;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AShifterModuleManager::AShifterModuleManager()
{
	Initialize();
}

AShifterModuleManager::~AShifterModuleManager()
{

}

void AShifterModuleManager::Initialize()
{
	InitPickFail( POS_FRONT );
	InitPickFail( POS_REAR );
}

void AShifterModuleManager::InitPickFail( int iPos )
{
	for( int iSocket = 0; iSocket < SOCKET_COUNT; iSocket++ )
	{
		m_iPickFail[iPos][iSocket] = PICK_FAIL_PASS;
	}
}
