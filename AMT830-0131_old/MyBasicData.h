// MyBasicData.h: interface for the CMyBasicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_)
#define AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"  // 전역 변수 정의 클래스 추가 
#include "Public_Function.h"

class CMyBasicData  
{
public:
	CMyBasicData();
	virtual ~CMyBasicData();

// Attributes
public:

// Operations
public:
	void OnAdmin_Data_Load();
	void On_Teach_Data_Save();
	void On_Teach_Data_Load();
	CString OnGet_Teach_File_Name(CString strName);
	CString OnGet_File_Name();
	void OnDaily_Count_Save();
	void OnBasic_Data_Save();
	void OnBasic_Data_Load();
	void OnMaintenance_Data_Save();
	void OnMaintenance_Data_Load();
	void OnWaitTime_Data_Save();
	void OnWaitTime_Data_Load();
	void OnMotorSpeed_Set_Data_Save();
	void OnMotorSpeed_Set_Data_Load();
	void OnStep_File_Name_Save(CString str_file);
	CString OnStep_File_Name_Load();
	void OnStep_File_Index_Save(CString str_index);
	CString OnStep_File_Index_Load();
	void OnPassWord_Save(CString str_save_level, CString str_pass_data);
	void OnPassWord_Load(CString str_load_level);

	void On_WorkSite_Save();
	void On_WorkSite_Load();
////////////////////////////////////////////////////////// 20120516	
	void OnInterface_Data_Save();
	void OnInterface_Data_Load();
///////////////////////////////////////////// 20120525
	void OnSkip_Data_Save();
	void OnSkip_Data_Load();
///////////////////////////////////////////// 20120531
	void OnBasic_Label_Data_Load();
	void OnBasic_Label_Data_Save();
///////////////////////////////////////////// 20120604
	void OnBarcode_Label_Data_Load();
	void OnBarcode_Label_Data_Save();

	void OnTray_Data_Load();
	void OnTray_Data_Save();
	
	void OnBasic_Part_Number_Load();
	void OnBasic_Part_Number_Save();
////////////////////////////////////////////////////////// 


};

#endif // !defined(AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_)
