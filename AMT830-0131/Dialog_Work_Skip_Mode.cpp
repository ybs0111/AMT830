// Dialog_Work_Skip_Mode.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Work_Skip_Mode.h"

#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyPad.h"

#include "Dialog_Keyboard.h"
#include "KeyboardDll.h"	//20120514 유성준
#include "CtlBd_Library.h"
#include "Ctlbd_Variable.h"
#include "Dialog_Pass_Change.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Skip_Mode dialog


CDialog_Work_Skip_Mode::CDialog_Work_Skip_Mode(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Work_Skip_Mode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Work_Skip_Mode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Work_Skip_Mode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Work_Skip_Mode)
	DDX_Control(pDX, IDC_GROUP_SKIP_MODULE_LOADER, m_group_skip_module_loader);
	DDX_Control(pDX, IDC_GROUP_SKIP_HEATSINK_LEFT, m_group_skip_heatsink_left);
	DDX_Control(pDX, IDC_GROUP_SKIP_HEATSINK_RIGHT, m_group_skip_heatsink_right);
	DDX_Control(pDX, IDC_GROUP_SKIP_PICKER_ROBOT, m_group_skip_picker_robot);
	DDX_Control(pDX, IDC_GROUP_SKIP_CLIP, m_group_skip_clip);
	DDX_Control(pDX, IDC_GROUP_SKIP_LABEL, m_group_skip_label);
	DDX_Control(pDX, IDC_GROUP_SKIP_VISION, m_group_skip_vision);
	DDX_Control(pDX, IDC_GROUP_SKIP_WORK_BUFFER1, m_group_skip_work_buffer);

	DDX_Control(pDX, IDC_CHK_SKIP_MODULE_LOADER_ENABLE1, m_chk_skip_module_loader_enable1);
	DDX_Control(pDX, IDC_CHK_SKIP_MODULE_LOADER_ENABLE2, m_chk_skip_module_loader_enable2);
	DDX_Control(pDX, IDC_CHK_SKIP_HEATSINK_LEFT_ENABLE1, m_chk_skip_heatsink_left_enable1);
	DDX_Control(pDX, IDC_CHK_SKIP_HEATSINK_LEFT_ENABLE2, m_chk_skip_heatsink_left_enable2);
	DDX_Control(pDX, IDC_CHK_SKIP_HEATSINK_RIGHT_ENABLE1, m_chk_skip_heatsink_right_enable1);
	DDX_Control(pDX, IDC_CHK_SKIP_HEATSINK_RIGHT_ENABLE2, m_chk_skip_heatsink_right_enable2);
	DDX_Control(pDX, IDC_CHK_SKIP_LABEL_ENABLE, m_chk_skip_label_enable);
	DDX_Control(pDX, IDC_CHK_SKIP_PICKER_ENABLE, m_chk_skip_picker_enable);
	DDX_Control(pDX, IDC_CHK_SKIP_VISION_ENABLE, m_chk_skip_vision_enable);
	DDX_Control(pDX, IDC_CHK_SKIP_VISION_ENABLE2, m_chk_skip_vision_enable2);
	DDX_Control(pDX, IDC_CHK_SKIP_CLIP_ENABLE, m_chk_skip_clip_enable);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHK_SKIP_WORK_BUFFER_ENABLE1, m_chk_skip_work_buffer_use[0]);
	DDX_Control(pDX, IDC_CHK_SKIP_WORK_BUFFER_ENABLE2, m_chk_skip_work_buffer_use[1]);
	DDX_Control(pDX, IDC_CHK_SKIP_WORK_BUFFER_ENABLE3, m_chk_skip_work_buffer_use[2]);
	DDX_Control(pDX, IDC_CHK_SKIP_WORK_BUFFER_ENABLE4, m_chk_skip_work_buffer_use[3]);


}

BEGIN_MESSAGE_MAP(CDialog_Work_Skip_Mode, CDialog)
	//{{AFX_MSG_MAP(CDialog_Work_Skip_Mode)
	ON_BN_CLICKED(IDC_BTN_SKIP_APPLY, OnBtnSkipApply)
	ON_BN_CLICKED(IDC_CHK_SKIP_MODULE_LOADER_ENABLE1, OnChkSkipModuleLoaderEnable1)
	ON_BN_CLICKED(IDC_CHK_SKIP_MODULE_LOADER_ENABLE2, OnChkSkipModuleLoaderEnable2)
	ON_BN_CLICKED(IDC_CHK_SKIP_HEATSINK_LEFT_ENABLE1, OnChkSkipHeatsinkLeftEnable1)
	ON_BN_CLICKED(IDC_CHK_SKIP_HEATSINK_LEFT_ENABLE2, OnChkSkipHeatsinkLeftEnable2)
	ON_BN_CLICKED(IDC_CHK_SKIP_HEATSINK_RIGHT_ENABLE1, OnChkSkipHeatsinkRightEnable1)
	ON_BN_CLICKED(IDC_CHK_SKIP_HEATSINK_RIGHT_ENABLE2, OnChkSkipHeatsinkRightEnable2)
	ON_BN_CLICKED(IDC_CHK_SKIP_LABEL_ENABLE, OnChkSkipLabelEnable)
	ON_BN_CLICKED(IDC_CHK_SKIP_PICKER_ENABLE, OnChkSkipPickerEnable)
	ON_BN_CLICKED(IDC_CHK_SKIP_VISION_ENABLE, OnChkSkipVisionEnable)
	ON_BN_CLICKED(IDC_CHK_SKIP_CLIP_ENABLE, OnChkSkipClipEnable)
	ON_BN_CLICKED(IDC_CHK_SKIP_WORK_BUFFER_ENABLE1, OnChkSkipWorkBufferEnable1)
	ON_BN_CLICKED(IDC_CHK_SKIP_WORK_BUFFER_ENABLE2, OnChkSkipWorkBufferEnable2)
	ON_BN_CLICKED(IDC_CHK_SKIP_WORK_BUFFER_ENABLE3, OnChkSkipWorkBufferEnable3)
	ON_BN_CLICKED(IDC_CHK_SKIP_WORK_BUFFER_ENABLE4, OnChkSkipWorkBufferEnable4)
	ON_BN_CLICKED(IDC_CHK_SKIP_VISION_ENABLE2, OnChkSkipVisionEnable2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Skip_Mode message handlers


BOOL CDialog_Work_Skip_Mode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char cJamcode[10] = {NULL};

	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다                                             
	// **************************************************************************
	mp_skip_font = NULL;
	mp_skip_font = new CFont;
	mp_skip_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
	OnSkip_Init_Button();
	OnSkip_GroupBox_Set();

	OnSkip_Data_Set();
	OnSkip_Data_BackUp();		
	OnSkip_Data_Display(); 	


	st_handler.cwnd_skip = this;  
	return TRUE;

}

void CDialog_Work_Skip_Mode::OnSkip_Init_Button()
{
	short	shBtnColor = 30;
	int i;
	
	m_chk_skip_module_loader_enable1.SetFont(mp_skip_font);
	m_chk_skip_module_loader_enable1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_module_loader_enable1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_module_loader_enable1.SetButtonColor(1);


	m_chk_skip_module_loader_enable2.SetFont(mp_skip_font);
	m_chk_skip_module_loader_enable2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_module_loader_enable2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_module_loader_enable2.SetButtonColor(1);


	m_chk_skip_heatsink_left_enable1.SetFont(mp_skip_font);
	m_chk_skip_heatsink_left_enable1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_heatsink_left_enable1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_heatsink_left_enable1.SetButtonColor(1);


	m_chk_skip_heatsink_left_enable2.SetFont(mp_skip_font);
	m_chk_skip_heatsink_left_enable2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_heatsink_left_enable2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_heatsink_left_enable2.SetButtonColor(1);


	m_chk_skip_heatsink_right_enable1.SetFont(mp_skip_font);
	m_chk_skip_heatsink_right_enable1.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_heatsink_right_enable1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_heatsink_right_enable1.SetButtonColor(1);

	m_chk_skip_heatsink_right_enable2.SetFont(mp_skip_font);
	m_chk_skip_heatsink_right_enable2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_heatsink_right_enable2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_heatsink_right_enable2.SetButtonColor(1);

	m_chk_skip_clip_enable.SetFont(mp_skip_font);
	m_chk_skip_clip_enable.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_clip_enable.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_clip_enable.SetButtonColor(1);


	m_chk_skip_picker_enable.SetFont(mp_skip_font);
	m_chk_skip_picker_enable.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_picker_enable.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_picker_enable.SetButtonColor(1);

	m_chk_skip_label_enable.SetFont(mp_skip_font);
	m_chk_skip_label_enable.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_label_enable.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_label_enable.SetButtonColor(1);


	m_chk_skip_vision_enable.SetFont(mp_skip_font);
	m_chk_skip_vision_enable.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_vision_enable.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_vision_enable.SetButtonColor(1);

/////////20120912
	m_chk_skip_vision_enable2.SetFont(mp_skip_font);
	m_chk_skip_vision_enable2.SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
	m_chk_skip_vision_enable2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_skip_vision_enable2.SetButtonColor(1);
/////////

	for (i=0; i<4; i++)
	{
		m_chk_skip_work_buffer_use[i].SetFont(mp_skip_font);
		m_chk_skip_work_buffer_use[i].SetIcon(IDI_GREEN_LED_ICON, IDI_RED_LED_ICON);
		m_chk_skip_work_buffer_use[i].OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		m_chk_skip_work_buffer_use[i].SetButtonColor(1);
		m_chk_skip_work_buffer_use[i].SetCheck(mn_skip_work_buffer_enable[i][1]);
	}

}
void CDialog_Work_Skip_Mode::OnDestroy() 
{
	CDialog::OnDestroy();	

	int mn_response;  // 대화 상자에 대한 리턴 값 저장 변수
	int Ret;

	CDialog_Select select_dlg;

	//프로그램을 빠져나가면서 Save 하지 않고, Form이 Destroy되는것이라면 Setting을 포기한다는 말이다.
	//비교도 하지 말자.
	if (st_handler.b_program_exit == false)
	{
		Ret = OnSkip_Data_Comparison();

		if (Ret == RET_ERROR)
		{
			st_msg.mstr_confirm_msg = _T("Changed Data! Apply?");
			mn_response = select_dlg.DoModal();
			
			if (mn_response == IDOK)
			{
				OnSkip_Data_Apply();
				OnSkip_LogFile_Create();

			}
			else if (mn_response == IDCANCEL)
			{
				OnSkip_Data_Apply();
				OnSkip_Data_Display();
			}	

		}
	}

	// **************************************************************************
	// 생성한 폰트 정보 삭제한다                                                 
	// **************************************************************************
	delete mp_skip_font;
	mp_skip_font = NULL;
	// **************************************************************************

	st_handler.cwnd_skip = NULL;	// Maintenance 화면에 대한 핸들 정보 저장 변수 초기화 
}



void CDialog_Work_Skip_Mode::OnBtnSkipApply() 
{
	// TODO: Add your control notification handler code here
	int n_response;  // 대화 상자 리턴 플래그
	char cJamcode[10] = {NULL};
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Skip Mode : Skip Data Apply?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		
		if(OnSkip_Data_Comparison() == CTLBD_RET_GOOD)
		{
			OnSkip_Data_Apply();			// Tower Lamp 화면 셋팅 정보 전역 변수에 전송 함수
			OnSkip_LogFile_Create();
			OnSkip_Data_BackUp();

			mcls_skip.OnSkip_Data_Save();				// 전역 변수에 저장된 Basic 셋팅 정보를 파일에 저장하는 함수
			mcls_skip.OnSkip_Data_Load();

			OnSkip_Data_Display();	
		}
		else 
		{
			if (mn_skip_m_loader_enable[0][1] == 0 && mn_skip_m_loader_enable[1][1] == 0)	
			{
				sprintf(cJamcode, "990001");
				CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
			}
			else if (mn_skip_hs_left_enable[0][1] == 0 && mn_skip_hs_left_enable[1][1] == 0)			
			{
				sprintf(cJamcode, "990002");
				CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
			}
			else if (mn_skip_hs_right_enable[0][1] == 0 && mn_skip_hs_right_enable[1][1] == 0)					
			{
				sprintf(cJamcode, "990003");
				CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
			}
			else if (mn_skip_work_buffer_enable[0][1] == 0 && mn_skip_work_buffer_enable[1][1] == 0 && 
		             mn_skip_work_buffer_enable[2][1] == 0 && mn_skip_work_buffer_enable[3][1] == 0)
			{
				sprintf(cJamcode, "990004");
				CTL_Lib.Alarm_Error_Occurrence(0, CTL_dWARNING, cJamcode);
			}
		}


	}
	else if (n_response == IDCANCEL)
	{
		
	}	
}

int CDialog_Work_Skip_Mode::OnSkip_Data_Comparison()
{
	int nFuncRet = CTLBD_RET_GOOD;

	if (mn_skip_m_loader_enable[0][1] == 0 && mn_skip_m_loader_enable[1][1] == 0)				nFuncRet = CTLBD_RET_ERROR;
	if (mn_skip_hs_left_enable[0][1] == 0 && mn_skip_hs_left_enable[1][1] == 0)					nFuncRet = CTLBD_RET_ERROR;
	if (mn_skip_hs_right_enable[0][1] == 0 && mn_skip_hs_right_enable[1][1] == 0)				nFuncRet = CTLBD_RET_ERROR;
	if (mn_skip_work_buffer_enable[0][1] == 0 && mn_skip_work_buffer_enable[1][1] == 0 && 
		mn_skip_work_buffer_enable[2][1] == 0 && mn_skip_work_buffer_enable[3][1] == 0)			nFuncRet = CTLBD_RET_ERROR;

	return nFuncRet;
}
void CDialog_Work_Skip_Mode::OnSkip_Data_Apply()
{
	int i;

	for(i=0; i<2; i++)
	{
		st_picker.n_module_ldrbt_enable[i]    = mn_skip_m_loader_enable[i][1];
		st_picker.n_heat_leftldrbt_enable[i]  = mn_skip_hs_left_enable[i][1];
		st_picker.n_heat_rightldrbt_enable[i] = mn_skip_hs_right_enable[i][1];	
	}

		st_picker.n_vision_eable[0]			  = mn_skip_vision_enable[1];//20120912
		st_picker.n_vision_eable[1]			  = mn_skip_vision_enable2[1];//20120912

		st_picker.n_label_enable			  = mn_skip_label_enable[1];
		st_picker.n_picker_ldrbt_enable[1]    = mn_skip_picker_ldrbt_enable[1];
		st_picker.n_heat_clipldrbt_enable[1]  = mn_skip_clip_enable[1];

	for(i=0; i<4; i++)
	{
		st_picker.n_work_buffer_enable[i]  = mn_skip_work_buffer_enable[i][1];
	}

}

void CDialog_Work_Skip_Mode::OnSkip_LogFile_Create()
{
	CString str_msg;

	if (mn_skip_m_loader_enable[0][0] !=			mn_skip_m_loader_enable[0][1])
	{
		switch (mn_skip_m_loader_enable[0][1])
		{
		case 0:
			str_msg = "[Skip Module Loader1] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Module Loader1] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_skip_hs_left_enable[0][0] !=			mn_skip_hs_left_enable[0][1])
	{
		switch (mn_skip_hs_left_enable[0][1])
		{
		case 0:
			str_msg = "[Skip HeatSink Left1] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip HeatSink Left1] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_skip_hs_right_enable[0][0] !=			mn_skip_hs_right_enable[0][1])
	{
		switch (mn_skip_hs_right_enable[0][1])
		{
		case 0:
			str_msg = "[Skip HeatSink Right1] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip HeatSink Right1] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}


	if (mn_skip_m_loader_enable[1][0] !=			mn_skip_m_loader_enable[1][1])
	{
		switch (mn_skip_m_loader_enable[1][1])
		{
		case 0:
			str_msg = "[Skip Module Loader2] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Module Loader2] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_skip_hs_left_enable[1][0] !=			mn_skip_hs_left_enable[1][1])
	{
		switch (mn_skip_hs_left_enable[1][1])
		{
		case 0:
			str_msg = "[Skip HeatSink Left2] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip HeatSink Left2] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_skip_hs_right_enable[1][0] !=			mn_skip_hs_right_enable[1][1])
	{
		switch (mn_skip_hs_right_enable[1][1])
		{
		case 0:
			str_msg = "[Skip HeatSink Right2] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip HeatSink Right2] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	//////////////////////////////////////////// 20120625
	if (mn_skip_clip_enable[0] !=			mn_skip_clip_enable[1])
	{
		switch (mn_skip_clip_enable[1])
		{
		case 0:
			str_msg = "[Skip Clip] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Clip] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_skip_picker_ldrbt_enable[0] !=			mn_skip_picker_ldrbt_enable[1])
	{
		switch (mn_skip_picker_ldrbt_enable[1])
		{
		case 0:
			str_msg = "[Skip Picker LD Robot] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Picker LD Robot] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
	////////////////////////////////////////////


	if (mn_skip_label_enable[0] !=			mn_skip_label_enable[1])
	{
		switch (mn_skip_label_enable[1])
		{
		case 0:
			str_msg = "[Skip Label] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Label] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_skip_vision_enable[0] !=			mn_skip_vision_enable[1])
	{
		switch (mn_skip_vision_enable[1])
		{
		case 0:
			str_msg = "[Skip Vision_Label] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Vision_Label] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

///////////////20120912
	if (mn_skip_vision_enable2[0] !=			mn_skip_vision_enable2[1])
	{
		switch (mn_skip_vision_enable2[1])
		{
		case 0:
			str_msg = "[Skip Vision_HS] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Vision_HS] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
///////////////

	if (mn_skip_work_buffer_enable[0][0] !=			mn_skip_work_buffer_enable[0][1])
	{
		switch (mn_skip_work_buffer_enable[0][1])
		{
		case 0:
			str_msg = "[Skip Work_Buffer1] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Work_Buffer1] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_skip_work_buffer_enable[1][0] !=			mn_skip_work_buffer_enable[1][1])
	{
		switch (mn_skip_work_buffer_enable[1][1])
		{
		case 0:
			str_msg = "[Skip Work_Buffer2] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Work_Buffer2] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_skip_work_buffer_enable[2][0] !=			mn_skip_work_buffer_enable[2][1])
	{
		switch (mn_skip_work_buffer_enable[2][1])
		{
		case 0:
			str_msg = "[Skip Work_Buffer3] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Work_Buffer3] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_skip_work_buffer_enable[3][0] !=			mn_skip_work_buffer_enable[3][1])
	{
		switch (mn_skip_work_buffer_enable[3][1])
		{
		case 0:
			str_msg = "[Skip Work_Buffer4] Was changed by With Skip.";
			break;
		case 1:
			str_msg = "[Skip Work_Buffer4] Was changed by With Out Skip.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

}
void CDialog_Work_Skip_Mode::OnSkip_Data_BackUp()
{
	int i;

	for(i=0; i<2; i++)
	{
		mn_skip_m_loader_enable[i][0]    = mn_skip_m_loader_enable[i][1];
		mn_skip_hs_left_enable[i][0]     = mn_skip_hs_left_enable[i][1];
		mn_skip_hs_right_enable[i][0]    = mn_skip_hs_right_enable[i][1];	
	}

		mn_skip_label_enable[0]          = mn_skip_label_enable[1];
		mn_skip_picker_ldrbt_enable[0]   = mn_skip_picker_ldrbt_enable[1];
		mn_skip_vision_enable[0]         = mn_skip_vision_enable[1];
		mn_skip_vision_enable2[0]         = mn_skip_vision_enable2[1];//20120912

		mn_skip_clip_enable[0]           = mn_skip_clip_enable[1];

	for(i=0; i<4; i++)
	{
		mn_skip_work_buffer_enable[i][0] = mn_skip_work_buffer_enable[i][1];
	}
}

void CDialog_Work_Skip_Mode::OnSkip_Data_Set()
{

	int i;

	for(i=0; i<2; i++)
	{
		mn_skip_m_loader_enable[i][1]    = st_picker.n_module_ldrbt_enable[i];
		mn_skip_hs_left_enable[i][1]     = st_picker.n_heat_leftldrbt_enable[i];
		mn_skip_hs_right_enable[i][1]    = st_picker.n_heat_rightldrbt_enable[i];	
	}
		mn_skip_vision_enable[1]         = st_picker.n_vision_eable[0];//20120912
		mn_skip_vision_enable2[1]         = st_picker.n_vision_eable[1];//20120912

		mn_skip_label_enable[1]          = st_picker.n_label_enable;
		mn_skip_picker_ldrbt_enable[1]   = st_picker.n_picker_ldrbt_enable[1];
		mn_skip_clip_enable[1]           = st_picker.n_heat_clipldrbt_enable[1];

	for(i=0; i<4; i++)
	{
		mn_skip_work_buffer_enable[i][1] = st_picker.n_work_buffer_enable[i];
	}


}



void CDialog_Work_Skip_Mode::OnSkip_GroupBox_Set()
{
	CSxLogFont skip_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_skip_module_loader.SetFont(skip_font);
	m_group_skip_module_loader.SetCatptionTextColor(RGB(145,25,0));
	m_group_skip_module_loader.SetFontBold(TRUE);
//	
	m_group_skip_heatsink_left.SetFont(skip_font);
	m_group_skip_heatsink_left.SetCatptionTextColor(RGB(145,25,0));
	m_group_skip_heatsink_left.SetFontBold(TRUE);
//	
	m_group_skip_heatsink_right.SetFont(skip_font);
	m_group_skip_heatsink_right.SetCatptionTextColor(RGB(145,25,0));
	m_group_skip_heatsink_right.SetFontBold(TRUE);
//
	m_group_skip_clip.SetFont(skip_font);
	m_group_skip_clip.SetCatptionTextColor(RGB(145,25,0));
	m_group_skip_clip.SetFontBold(TRUE);
//
	m_group_skip_picker_robot.SetFont(skip_font);
	m_group_skip_picker_robot.SetCatptionTextColor(RGB(145,25,0));
	m_group_skip_picker_robot.SetFontBold(TRUE);
//
	m_group_skip_label.SetFont(skip_font);
	m_group_skip_label.SetCatptionTextColor(RGB(145,25,0));
	m_group_skip_label.SetFontBold(TRUE);

	m_group_skip_vision.SetFont(skip_font);
	m_group_skip_vision.SetCatptionTextColor(RGB(145,25,0));
	m_group_skip_vision.SetFontBold(TRUE);

	m_group_skip_work_buffer .SetFont(skip_font);
	m_group_skip_work_buffer.SetCatptionTextColor(RGB(145,25,0));
	m_group_skip_work_buffer.SetFontBold(TRUE);


}

void CDialog_Work_Skip_Mode::OnSkip_Data_Recovery()
{
	int i;

	for(i=0; i<2; i++)
	{
		mn_skip_m_loader_enable[i][1]    = mn_skip_m_loader_enable[i][0];
		mn_skip_hs_left_enable[i][1]     = mn_skip_hs_left_enable[i][0];
		mn_skip_hs_right_enable[i][1]    = mn_skip_hs_right_enable[i][0];	
	}

		mn_skip_label_enable[1]          = mn_skip_label_enable[0];
		mn_skip_picker_ldrbt_enable[1]   = mn_skip_picker_ldrbt_enable[0];
		mn_skip_vision_enable[1]         = mn_skip_vision_enable[0];
		mn_skip_vision_enable2[1]         = mn_skip_vision_enable2[0];//20120912

		mn_skip_clip_enable[1]           = mn_skip_clip_enable[0];

	for(i=0; i<4; i++)
	{
		mn_skip_work_buffer_enable[i][1] = mn_skip_work_buffer_enable[i][0];
	}
}

void CDialog_Work_Skip_Mode::OnSkip_Data_Display()
{
	//((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText(mstr_device_name[1]);
	if (mn_skip_m_loader_enable[0][1] < 0 || mn_skip_m_loader_enable[0][1] > 1)	mn_skip_m_loader_enable[0][1] = TRUE;
	m_chk_skip_module_loader_enable1.SetCheck(mn_skip_m_loader_enable[0][1]);

	if (mn_skip_m_loader_enable[0][1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_MODULE_LOADER_ENABLE1, "#1 Skip Off");
	else										SetDlgItemText(IDC_CHK_SKIP_MODULE_LOADER_ENABLE1, "#1 Skip On");
//
	if (mn_skip_m_loader_enable[1][1] < 0 || mn_skip_m_loader_enable[1][1] > 1)	mn_skip_m_loader_enable[1][1] = TRUE;
	m_chk_skip_module_loader_enable2.SetCheck(mn_skip_m_loader_enable[1][1]);
	if (mn_skip_m_loader_enable[1][1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_MODULE_LOADER_ENABLE2, "#2 Skip Off");
	else										SetDlgItemText(IDC_CHK_SKIP_MODULE_LOADER_ENABLE2, "#2 Skip On");
//
	if (mn_skip_hs_left_enable[0][1] < 0 || mn_skip_hs_left_enable[0][1] > 1)	mn_skip_hs_left_enable[0][1] = TRUE;
	m_chk_skip_heatsink_left_enable1.SetCheck(mn_skip_hs_left_enable[0][1]);
	if (mn_skip_hs_left_enable[0][1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_HEATSINK_LEFT_ENABLE1, "#1 Skip Off");
	else										SetDlgItemText(IDC_CHK_SKIP_HEATSINK_LEFT_ENABLE1, "#1 Skip On");
//	
	if (mn_skip_hs_left_enable[1][1] < 0 || mn_skip_hs_left_enable[1][1] > 1)	mn_skip_hs_left_enable[1][1] = TRUE;
	m_chk_skip_heatsink_left_enable2.SetCheck(mn_skip_hs_left_enable[1][1]);

	if (mn_skip_hs_left_enable[1][1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_HEATSINK_LEFT_ENABLE2, "#2 Skip Off");
	else										SetDlgItemText(IDC_CHK_SKIP_HEATSINK_LEFT_ENABLE2, "#2 Skip On");
//
	if (mn_skip_hs_right_enable[0][1] < 0 || mn_skip_hs_right_enable[0][1] > 1)	mn_skip_hs_right_enable[0][1] = TRUE;
	m_chk_skip_heatsink_right_enable1.SetCheck(mn_skip_hs_right_enable[0][1]);

	if (mn_skip_hs_right_enable[0][1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_HEATSINK_RIGHT_ENABLE1, "#1 Skip Off");
	else										SetDlgItemText(IDC_CHK_SKIP_HEATSINK_RIGHT_ENABLE1, "#1 Skip On");
//
	if (mn_skip_hs_right_enable[1][1] < 0 || mn_skip_hs_right_enable[1][1] > 1)	mn_skip_hs_right_enable[1][1] = TRUE;
	m_chk_skip_heatsink_right_enable2.SetCheck(mn_skip_hs_right_enable[1][1]);

	if (mn_skip_hs_right_enable[1][1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_HEATSINK_RIGHT_ENABLE2, "#2 Skip Off");
	else										SetDlgItemText(IDC_CHK_SKIP_HEATSINK_RIGHT_ENABLE2, "#2 Skip On");
//
	if (mn_skip_label_enable[1] < 0 || mn_skip_label_enable[1] > 1)	mn_skip_label_enable[1] = TRUE;
	m_chk_skip_label_enable.SetCheck(mn_skip_label_enable[1]);

	if (mn_skip_label_enable[1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_LABEL_ENABLE, "Skip Off");
	else									SetDlgItemText(IDC_CHK_SKIP_LABEL_ENABLE, "Skip On");
//
	if (mn_skip_picker_ldrbt_enable[1] < 0 || mn_skip_picker_ldrbt_enable[1] > 1)	mn_skip_picker_ldrbt_enable[1] = TRUE;
	m_chk_skip_picker_enable.SetCheck(mn_skip_picker_ldrbt_enable[1]);

	if (mn_skip_picker_ldrbt_enable[1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_PICKER_ENABLE, "Skip Off");
	else									    SetDlgItemText(IDC_CHK_SKIP_PICKER_ENABLE, "Skip On");
//
	if (mn_skip_vision_enable[1] < 0 || mn_skip_vision_enable[1] > 1)	mn_skip_vision_enable[1] = TRUE;
	m_chk_skip_vision_enable.SetCheck(mn_skip_vision_enable[1]);
	
	if (mn_skip_vision_enable[1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_VISION_ENABLE, "Barcode Skip Off");
	else									SetDlgItemText(IDC_CHK_SKIP_VISION_ENABLE, "Barcode Skip On");
////////////20120912
//
	if (mn_skip_vision_enable2[1] < 0 || mn_skip_vision_enable2[1] > 1)	mn_skip_vision_enable2[1] = TRUE;
	m_chk_skip_vision_enable2.SetCheck(mn_skip_vision_enable2[1]);
	
	if (mn_skip_vision_enable2[1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_VISION_ENABLE2, "HS Skip Off");
	else									SetDlgItemText(IDC_CHK_SKIP_VISION_ENABLE2, "HS Skip On");
////////////

//	
	if (mn_skip_clip_enable[1] < 0 || mn_skip_clip_enable[1] > 1)	mn_skip_clip_enable[1] = TRUE;
	m_chk_skip_clip_enable.SetCheck(mn_skip_clip_enable[1]);
	
	if (mn_skip_clip_enable[1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_CLIP_ENABLE, "Skip Off");
	else							 	SetDlgItemText(IDC_CHK_SKIP_CLIP_ENABLE, "Skip On");
//
	if (mn_skip_work_buffer_enable[0][1] < 0 || mn_skip_work_buffer_enable[0][1] > 1)	mn_skip_work_buffer_enable[0][1] = TRUE;
	m_chk_skip_work_buffer_use[0].SetCheck(mn_skip_work_buffer_enable[0][1]);

	if (mn_skip_work_buffer_enable[0][1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_WORK_BUFFER_ENABLE1, "#1 Skip Off");
	else							 	            SetDlgItemText(IDC_CHK_SKIP_WORK_BUFFER_ENABLE1, "#1 Skip On");
//	
	if (mn_skip_work_buffer_enable[1][1] < 0 || mn_skip_work_buffer_enable[1][1] > 1)	mn_skip_work_buffer_enable[1][1] = TRUE;
	m_chk_skip_work_buffer_use[1].SetCheck(mn_skip_work_buffer_enable[1][1]);

	if (mn_skip_work_buffer_enable[1][1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_WORK_BUFFER_ENABLE2, "#2 Skip Off");
	else							 	            SetDlgItemText(IDC_CHK_SKIP_WORK_BUFFER_ENABLE2, "#2 Skip On");
//
	if (mn_skip_work_buffer_enable[2][1] < 0 || mn_skip_work_buffer_enable[2][1] > 1)	mn_skip_work_buffer_enable[2][1] = TRUE;
	m_chk_skip_work_buffer_use[2].SetCheck(mn_skip_work_buffer_enable[2][1]);

	if (mn_skip_work_buffer_enable[2][1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_WORK_BUFFER_ENABLE3, "#3 Skip Off");
	else							 	            SetDlgItemText(IDC_CHK_SKIP_WORK_BUFFER_ENABLE3, "#3 Skip On");
//	
	if (mn_skip_work_buffer_enable[3][1] < 0 || mn_skip_work_buffer_enable[3][1] > 1)	mn_skip_work_buffer_enable[3][1] = TRUE;
	m_chk_skip_work_buffer_use[3].SetCheck(mn_skip_work_buffer_enable[3][1]);

	if (mn_skip_work_buffer_enable[3][1] == TRUE)	SetDlgItemText(IDC_CHK_SKIP_WORK_BUFFER_ENABLE4, "#4 Skip Off");
	else							 	            SetDlgItemText(IDC_CHK_SKIP_WORK_BUFFER_ENABLE4, "#4 Skip On");
	

}

void CDialog_Work_Skip_Mode::OnChkSkipModuleLoaderEnable1() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_module_loader_enable1.GetCheck() == TRUE)	mn_skip_m_loader_enable[0][1] = TRUE;
	else														mn_skip_m_loader_enable[0][1] = FALSE;
	OnSkip_Data_Display();
}

void CDialog_Work_Skip_Mode::OnChkSkipModuleLoaderEnable2() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_module_loader_enable2.GetCheck() == TRUE)	mn_skip_m_loader_enable[1][1] = TRUE;
	else														mn_skip_m_loader_enable[1][1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipHeatsinkLeftEnable1() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_heatsink_left_enable1.GetCheck() == TRUE)	mn_skip_hs_left_enable[0][1] = TRUE;
	else														mn_skip_hs_left_enable[0][1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipHeatsinkLeftEnable2() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_heatsink_left_enable2.GetCheck() == TRUE)	mn_skip_hs_left_enable[1][1] = TRUE;
	else														mn_skip_hs_left_enable[1][1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipHeatsinkRightEnable1() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_heatsink_right_enable1.GetCheck() == TRUE)	mn_skip_hs_right_enable[0][1] = TRUE;
	else														mn_skip_hs_right_enable[0][1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipHeatsinkRightEnable2() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_heatsink_right_enable2.GetCheck() == TRUE)	mn_skip_hs_right_enable[1][1] = TRUE;
	else														mn_skip_hs_right_enable[1][1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipLabelEnable() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_label_enable.GetCheck() == TRUE)	mn_skip_label_enable[1] = TRUE;
	else											mn_skip_label_enable[1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipPickerEnable() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_picker_enable.GetCheck() == TRUE)	mn_skip_picker_ldrbt_enable[1] = TRUE;
	else											    mn_skip_picker_ldrbt_enable[1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipVisionEnable() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_vision_enable.GetCheck() == TRUE)	mn_skip_vision_enable[1] = TRUE;
	else											    mn_skip_vision_enable[1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipVisionEnable2()//20120912 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_vision_enable2.GetCheck() == TRUE)	mn_skip_vision_enable2[1] = TRUE;
	else											    mn_skip_vision_enable2[1] = FALSE;
	OnSkip_Data_Display();	
	
}

void CDialog_Work_Skip_Mode::OnChkSkipClipEnable() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_clip_enable.GetCheck() == TRUE)	mn_skip_clip_enable[1] = TRUE;
	else										    mn_skip_clip_enable[1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipWorkBufferEnable1() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_work_buffer_use[0].GetCheck() == TRUE)	mn_skip_work_buffer_enable[0][1] = TRUE;
	else										            mn_skip_work_buffer_enable[0][1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipWorkBufferEnable2() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_work_buffer_use[1].GetCheck() == TRUE)	mn_skip_work_buffer_enable[1][1] = TRUE;
	else										            mn_skip_work_buffer_enable[1][1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipWorkBufferEnable3() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_work_buffer_use[2].GetCheck() == TRUE)	mn_skip_work_buffer_enable[2][1] = TRUE;
	else										            mn_skip_work_buffer_enable[2][1] = FALSE;
	OnSkip_Data_Display();	
}

void CDialog_Work_Skip_Mode::OnChkSkipWorkBufferEnable4() 
{
	// TODO: Add your control notification handler code here
	if (m_chk_skip_work_buffer_use[3].GetCheck() == TRUE)	mn_skip_work_buffer_enable[3][1] = TRUE;
	else										            mn_skip_work_buffer_enable[3][1] = FALSE;
	OnSkip_Data_Display();	
}

