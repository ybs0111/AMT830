// AMTRegistry.h: interface for the AMTRegistry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMTREGISTRY_H__A6E1C5C0_C1BD_43A0_9D28_67FE1ECAC207__INCLUDED_)
#define AFX_AMTREGISTRY_H__A6E1C5C0_C1BD_43A0_9D28_67FE1ECAC207__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#define REG_KEY_LOTNO	"Software\\AMT\\AMT830\\LotNo\\"
#define REG_VALUE_LOTNO_0	"LotNo_0"
#define REG_VALUE_LOTNO_1	"LotNo_1"
#define REG_VALUE_LOTNO_2	"LotNo_2"

#define REG_VALUE_LOTNO_IDX	"LotNo_Idx"
#define REG_KEY_COUNT				"Software\\AMT\\AMT830\\COUNT\\"
#define REG_VAL_COUNT_ALARM			"ALARM"

#define REG_KEY_TIME				"Software\\AMT\\AMT830\\TIME\\"
#define REG_VAL_TIME				"DAILY"

class AMTRegistry  
{
public:
	AMTRegistry();
	virtual ~AMTRegistry();

	static UINT RegReadInt( LPCTSTR lpKey, LPCTSTR lpValue, UINT nDefault = 0 );	
	static BOOL RegWriteInt( LPCTSTR lpKey, LPCTSTR lpValue, UINT nData );
	static std::string RegReadString( LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault );
	static BOOL RegWriteString( LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData );
	static BOOL RegRemoveValue(LPCTSTR lpKey, LPCTSTR lpValue);

	static void RegWriteTime( SYSTEMTIME t );
	static SYSTEMTIME RegReadTime();
};

#endif // !defined(AFX_AMTREGISTRY_H__A6E1C5C0_C1BD_43A0_9D28_67FE1ECAC207__INCLUDED_)
