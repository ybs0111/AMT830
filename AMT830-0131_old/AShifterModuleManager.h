// AShifterModuleManager.h: interface for the AShifterModuleManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASHIFTERMODULEMANAGER_H__3988C4F0_3C64_4CEB_AA66_A82DAF6F62D4__INCLUDED_)
#define AFX_ASHIFTERMODULEMANAGER_H__3988C4F0_3C64_4CEB_AA66_A82DAF6F62D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ePickFail 
{
	PICK_FAIL_PASS,
	PICK_FAIL_DISAPPEAR,
	PICK_FAIL_APPEAR,
};

class AShifterModuleManager  
{
public:
	AShifterModuleManager();
	virtual ~AShifterModuleManager();

	void SetPickFail( int iPos, int iSocket, int iFail ) { m_iPickFail[iPos][iSocket] = iFail; }
	int  GetPickFail( int iPos, int iSocket ) { return m_iPickFail[iPos][iSocket]; }

	void Initialize();
	void InitPickFail( int iPos );

protected:
	int m_iPickFail[POS_COUNT][SOCKET_COUNT];

};
extern AShifterModuleManager g_ShifterModule;

#endif // !defined(AFX_ASHIFTERMODULEMANAGER_H__3988C4F0_3C64_4CEB_AA66_A82DAF6F62D4__INCLUDED_)
