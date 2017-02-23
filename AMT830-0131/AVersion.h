// AVersion.h: interface for the AVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVERSION_H__AA379D4C_FA4A_4E18_86C5_447EAA9E329B__INCLUDED_)
#define AFX_AVERSION_H__AA379D4C_FA4A_4E18_86C5_447EAA9E329B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define REG_KEY_VER	"Software\\AMT\\AMT830\\Version\\"
#define REG_VALUE_CREATE_VER	"Ver"
#define REG_VALUE_CREATE_DATE	"Date"

class AVersion  
{
public:
	AVersion();
	virtual ~AVersion();

	void CheckVersion();
	void UpdateVersion();

	void SetVersion();

protected:
	CString m_strUpdate;

};

extern AVersion g_ver;

#endif // !defined(AFX_AVERSION_H__AA379D4C_FA4A_4E18_86C5_447EAA9E329B__INCLUDED_)
