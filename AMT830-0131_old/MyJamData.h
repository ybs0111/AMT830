// MyJamData.h: interface for the CMyJamData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_)
#define AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"       // 전역 변수 정의 클래스

class CMyJamData  
{
public:
	CMyJamData();
	virtual ~CMyJamData();

// Attributes
public:
//	CAlarmListFile mcls_alarm_list;  // 알람 리스트 파일 생성 클래스 변수 선언

// Operations
public:
	void On_Daily_ListFile_Add(CString str_alarm_code, CString str_display_time, CString str_alarm_msg);
	CString On_Alarm_Display_Info_Load(CString strPath, CString strDataFile, CString strActionFile, CString strCode);  // 발생 알람 정보 설정 함수

	CString On_Alarm_Bmp_Data_Load(CString strPath, CString strDataFile, CString strCode);       // 발생 알람 부위 표시용 그림 파일명 설정 함수
	CString On_Alarm_Style_Data_Load(CString strPath, CString strDataFile, CString strCode);     // 알람 발생 부위 표시 형태 및 출력 위치 설정 함수
	CString On_Alarm_Repair_Data_Load(CString strPath, CString strActionFile, CString strCode);  // 발생한 알람에 대한 조치 사항 설정 함수
	
	void On_Alarm_Display();					// 발생한 알람 화면 출력 함수
	void On_Occured_Alarm_Saving_to_File();		// 발생한 알람 로그 파일에 저장하는 함수
	void On_Alarm_Info_Load();					// 파일에 저장된 모든 알람 정보 전역 변수에 설정하는 함수
	void On_Alarm_Info_Set_to_Variable(CString strErrCode);  // 발생한 알람 메시지 전역 변수에 설정하는 함수
};
extern CMyJamData			MyJamData;
#endif // !defined(AFX_MYJAMDATA_H__CB595225_06C8_44CA_9BC6_742D8AC656E1__INCLUDED_)
