// AMTLotManager.h: interface for the AMTLotManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_)
#define AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <vector>
enum { WORK_PRIME, WORK_RETEST, WORK_MAX };
enum { MAX_LOTINFO = 3 };

// LotResult		수율
// LotSite			Lot 흐르는 동안의 SITE
// AMTLot
// AMTLotManager

class LotResult
{
public:
	LotResult();
	
	void Initialize();

	int GetBinCountGood(int iWork) { return m_iBinCount_Good[iWork]; }
	int GetBinCountFail(int iWork) { return m_iBinCount_Fail[iWork]; }
	int GetBinCountNTO(int iWork) { return m_iBinCount_NTO[iWork]; }
	int GetBinCountBTO(int iWork) { return m_iBinCount_BTO[iWork]; }
	int GetBinCountHTO(int iWork) { return m_iBinCount_HTO[iWork]; }

	int GetTestCountLoad(int iWork) { return m_iTestCount_Load[iWork]; }
	int GetTestCountGood(int iWork) { return m_iTestCount_Good[iWork]; }
	int GetTestCountFail(int iWork) { return m_iTestCount_Fail[iWork]; }

	void IncBinCountGood(int iWork) { m_iBinCount_Good[iWork]++; }
	void IncBinCountFail(int iWork) { m_iBinCount_Fail[iWork]++; }
	void IncBinCountNTO(int iWork) { m_iBinCount_NTO[iWork]++; }
	void IncBinCountBTO(int iWork) { m_iBinCount_BTO[iWork]++; }
	void IncBinCountHTO(int iWork) { m_iBinCount_HTO[iWork]++; }

	void IncTestCountLoad(int iWork) { m_iTestCount_Load[iWork]++; }
	void IncTestCountGood(int iWork) { m_iTestCount_Good[iWork]++; }
	void IncTestCountFail(int iWork) { m_iTestCount_Fail[iWork]++; }

	int GetBootTestCount_Load() { return m_iBootTestCount_Load; }
	int GetBootTestCount_Pass() { return m_iBootTestCount_Pass; }
	int GetBootTestCount_Load_Real() { return m_iBootTestCount_Load_Real; }
	int GetBootTestCount_Pass_Real() { return m_iBootTestCount_Pass_Real; }

	void IncBootTestCount_Load() { m_iBootTestCount_Load++; }
	void IncBootTestCount_Pass() { m_iBootTestCount_Pass++; }
	void IncBootTestCount_Load_Real() { m_iBootTestCount_Load_Real++; }
	void IncBootTestCount_Pass_Real() { m_iBootTestCount_Pass_Real++; }

	
protected:
	// Bin Count
	int m_iBinCount_Good[WORK_MAX];
	int m_iBinCount_Fail[WORK_MAX];
	int m_iBinCount_NTO[WORK_MAX];
	int m_iBinCount_BTO[WORK_MAX];
	int m_iBinCount_HTO[WORK_MAX];

	// Test Count
	int m_iTestCount_Load[WORK_MAX];
	int m_iTestCount_Good[WORK_MAX];
	int m_iTestCount_Fail[WORK_MAX];

	int m_iBootTestCount_Load;
	int m_iBootTestCount_Pass;
	int m_iBootTestCount_Load_Real;
	int m_iBootTestCount_Pass_Real;
};

class LotSite
{
public:
	LotSite();
	LotSite( DWORD dwTestedTime );
	void Initialize();

	DWORD GetTestedTime();
protected:
	DWORD	m_dwTestedTime;
};

// AMTLot
class AMTLot
{
public:
	AMTLot();
	AMTLot( CString strLotName );

	void Initialize();

	void WriteStartTime();
	void WriteFinishTime();

	CString GetStrStartTime()	{ return m_strTimeStart; }
	CString GetStrFinishTime()	{ return m_strTimeFinish; }

	void AddTestedSite( DWORD dwTestedTime );
	CString GetStrTestedSitesTimeMax();
	CString GetStrTestedSitesTimeMin();
	CString GetStrTestedSitesTimeAvr();

	CString GetStrUPH_Pass();
	CString GetStrUPH_Fail();
	CString GetStrUPH_Total();
	LotResult& GetLotResult() { return m_LotResult; }

	bool CheckPartNo_forDIMM( CString strPartNo );

protected:
	CString GetStrTime( DWORD dwTime );

	bool m_bDimmFailed;

public:
	CString			m_strLotName;
	LotResult		m_LotResult;

	DWORD			m_dwTimeStart, m_dwTimeFinish;
	CString			m_strTimeStart;
	CString			m_strTimeFinish;

	std::vector<LotSite> m_vecSites;
};


// AMTLotManager
class AMTLotManager  
{
public:
	AMTLotManager();
	virtual ~AMTLotManager();

	void Initialize();
	void PushLot( CString strLotName );
	AMTLot& FindLotByName( CString strLotName );
	AMTLot& GetLastLot();

	int GetWorkCountLoad() { return m_iWorkCount_Load; }
	int GetWorkCountGood() { return m_iWorkCount_Good; }
	int GetWorkCountFail() { return m_iWorkCount_Fail; }
	int GetWorkCountAlarm() { return m_iWorkCount_Alarm; }

	void IncWorkCountLoad() { m_iWorkCount_Load++; }
	void IncWorkCountGood() { m_iWorkCount_Good++; }
	void IncWorkCountFail() { m_iWorkCount_Fail++; }
	void IncWorkCountAlarm() { m_iWorkCount_Alarm++; }

	void SetCanPushNewLot();

	void CheckNewLotNo( CString strLotName );
	bool CheckPartNo_forDIMM( CString strPartNo );
	bool CheckChangeableDIMM();

public:
	std::vector<AMTLot> m_vecLot;
	AMTLot m_LotDummy;

	int m_iWorkCount_Load;
	int m_iWorkCount_Good;
	int m_iWorkCount_Fail;
	int m_iWorkCount_Alarm;

	bool m_bCanPushNewLot;
	bool m_bFailed_DimmCheck;

	char cJamcode[10];
};

extern AMTLotManager g_AMTLotMgr;

#endif // !defined(AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_)
