// MyJamData.h: interface for the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_)
#define AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"       // ���� ���� ���� Ŭ����

class CMyJamData  
{
public:
	CMyJamData();
	virtual ~CMyJamData();

// Attributes
public:
//	CAlarmListFile mcls_alarm_list;  // �˶� ����Ʈ ���� ���� Ŭ���� ���� ����

// Operations
public:
	void On_Daily_ListFile_Add(CString str_alarm_code, CString str_display_time, CString str_alarm_msg);
	CString On_Alarm_Display_Info_Load(CString strPath, CString strDataFile, CString strActionFile, CString strCode);  // �߻� �˶� ���� ���� �Լ�

	CString On_Alarm_Bmp_Data_Load(CString strPath, CString strDataFile, CString strCode);       // �߻� �˶� ���� ǥ�ÿ� �׸� ���ϸ� ���� �Լ�
	CString On_Alarm_Style_Data_Load(CString strPath, CString strDataFile, CString strCode);     // �˶� �߻� ���� ǥ�� ���� �� ��� ��ġ ���� �Լ�
	CString On_Alarm_Repair_Data_Load(CString strPath, CString strActionFile, CString strCode);  // �߻��� �˶��� ���� ��ġ ���� ���� �Լ�
	
	void On_Alarm_Display();					// �߻��� �˶� ȭ�� ��� �Լ�
	void On_Occured_Alarm_Saving_to_File();		// �߻��� �˶� �α� ���Ͽ� �����ϴ� �Լ�
	void On_Alarm_Info_Load();					// ���Ͽ� ����� ��� �˶� ���� ���� ������ �����ϴ� �Լ�
	void On_Alarm_Info_Set_to_Variable(CString strErrCode);  // �߻��� �˶� �޽��� ���� ������ �����ϴ� �Լ�
};
extern CMyJamData			MyJamData;
#endif // !defined(AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_)
