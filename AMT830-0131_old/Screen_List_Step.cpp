// Screen_List_Step.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_List_Step.h"
#include "Dialog_Select.h"
#include "math.h"
#include "io.h"			// 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요
#include "KeyboardDll.h"	//20120514 유성준
#include "Dialog_Message.h"

#include "Ctlbd_Variable.h"
/////////////////////////////////
#include "Dialog_KeyPad.h"
#include "Dialog_Keyboard.h"

/* ****************************************************************************** */
/* 쓰레드 관련 클래스                                                             */
/* ****************************************************************************** */


 #include "Run_Rbt_Load_Clip.h"
 #include "Run_Rbt_Load_LeftHeat.h"
 #include "Run_Rbt_Load_Module.h"
 #include "Run_Rbt_Load_RightHeat.h"
 #include "Run_Rbt_Sorter.h"
 #include "Run_Rbt_VisionLabel.h"
 #include "Run_Rbt_Work_Buffer.h"
 #include "Run_Stacker_Load_Clip.h"
 #include "Run_Stacker_Load_LeftHeat.h"
 #include "Run_Stacker_Load_Module.h"
 #include "Run_Stacker_Load_RightHeat.h"
 #include "Run_Stacker_Sorter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step

IMPLEMENT_DYNCREATE(CScreen_List_Step, CFormView)

CScreen_List_Step::CScreen_List_Step()
	: CFormView(CScreen_List_Step::IDD)
{
	//{{AFX_DATA_INIT(CScreen_List_Step)
	//}}AFX_DATA_INIT
}

CScreen_List_Step::~CScreen_List_Step()
{
}

void CScreen_List_Step::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_List_Step)
	DDX_Control(pDX, IDC_GROUP_STEP_LIST, m_group_step_list);
	DDX_Control(pDX, IDC_MSG_THREAD_1, m_msg_thread_1);
	DDX_Control(pDX, IDC_MSG_THREAD_2, m_msg_thread_2);
	DDX_Control(pDX, IDC_MSG_THREAD_3, m_msg_thread_3);
	DDX_Control(pDX, IDC_MSG_THREAD_4, m_msg_thread_4);
	DDX_Control(pDX, IDC_MSG_THREAD_5, m_msg_thread_5);
	DDX_Control(pDX, IDC_MSG_THREAD_6, m_msg_thread_6);
	DDX_Control(pDX, IDC_MSG_THREAD_7, m_msg_thread_7);
	DDX_Control(pDX, IDC_MSG_THREAD_8, m_msg_thread_8);
	DDX_Control(pDX, IDC_MSG_THREAD_9, m_msg_thread_9);
	DDX_Control(pDX, IDC_MSG_THREAD_10, m_msg_thread_10);
	DDX_Control(pDX, IDC_MSG_THREAD_11, m_msg_thread_11);
	DDX_Control(pDX, IDC_MSG_THREAD_12, m_msg_thread_12);
	DDX_Control(pDX, IDC_MSG_THREAD_13, m_msg_thread_13);
	DDX_Control(pDX, IDC_MSG_THREAD_14, m_msg_thread_14);
	DDX_Control(pDX, IDC_MSG_THREAD_15, m_msg_thread_15);
	DDX_Control(pDX, IDC_MSG_THREAD_16, m_msg_thread_16);
	DDX_Control(pDX, IDC_MSG_THREAD_17, m_msg_thread_17);
	DDX_Control(pDX, IDC_MSG_THREAD_18, m_msg_thread_18);
	DDX_Control(pDX, IDC_MSG_THREAD_19, m_msg_thread_19);
	DDX_Control(pDX, IDC_MSG_THREAD_20, m_msg_thread_20);
	DDX_Control(pDX, IDC_MSG_THREAD_21, m_msg_thread_21);
	DDX_Control(pDX, IDC_MSG_THREAD_22, m_msg_thread_22);
	DDX_Control(pDX, IDC_MSG_THREAD_23, m_msg_thread_23);
	DDX_Control(pDX, IDC_MSG_THREAD_24, m_msg_thread_24);
	DDX_Control(pDX, IDC_MSG_THREAD_25, m_msg_thread_25);
	DDX_Control(pDX, IDC_MSG_THREAD_26, m_msg_thread_26);
	DDX_Control(pDX, IDC_MSG_THREAD_27, m_msg_thread_27);
	DDX_Control(pDX, IDC_MSG_THREAD_28, m_msg_thread_28);
	DDX_Control(pDX, IDC_MSG_THREAD_29, m_msg_thread_29);
	DDX_Control(pDX, IDC_MSG_THREAD_30, m_msg_thread_30);
	DDX_Control(pDX, IDC_MSG_THREAD_31, m_msg_thread_31);
	DDX_Control(pDX, IDC_MSG_THREAD_32, m_msg_thread_32);
	DDX_Control(pDX, IDC_MSG_THREAD_33, m_msg_thread_33);
	DDX_Control(pDX, IDC_MSG_THREAD_34, m_msg_thread_34);
	DDX_Control(pDX, IDC_MSG_THREAD_35, m_msg_thread_35);
	DDX_Control(pDX, IDC_MSG_THREAD_36, m_msg_thread_36);
	DDX_Control(pDX, IDC_MSG_THREAD_37, m_msg_thread_37);
	DDX_Control(pDX, IDC_MSG_THREAD_38, m_msg_thread_38);
	DDX_Control(pDX, IDC_MSG_THREAD_39, m_msg_thread_39);
	DDX_Control(pDX, IDC_MSG_THREAD_40, m_msg_thread_40);
	DDX_Control(pDX, IDC_MSG_THREAD_41, m_msg_thread_41);
	DDX_Control(pDX, IDC_MSG_THREAD_42, m_msg_thread_42);
	DDX_Control(pDX, IDC_MSG_THREAD_43, m_msg_thread_43);
	DDX_Control(pDX, IDC_MSG_THREAD_44, m_msg_thread_44);
	DDX_Control(pDX, IDC_MSG_THREAD_45, m_msg_thread_45);
	DDX_Control(pDX, IDC_MSG_THREAD_46, m_msg_thread_46);
	DDX_Control(pDX, IDC_MSG_THREAD_47, m_msg_thread_47);
	DDX_Control(pDX, IDC_MSG_THREAD_48, m_msg_thread_48);
	DDX_Control(pDX, IDC_MSG_THREAD_49, m_msg_thread_49);
	DDX_Control(pDX, IDC_MSG_THREAD_50, m_msg_thread_50);
	DDX_Control(pDX, IDC_MSG_THREAD_51, m_msg_thread_51);
	DDX_Control(pDX, IDC_MSG_THREAD_52, m_msg_thread_52);
 	DDX_Control(pDX, IDC_MSG_THREAD_53, m_msg_thread_53);
 	DDX_Control(pDX, IDC_MSG_THREAD_54, m_msg_thread_54);
// 	DDX_Control(pDX, IDC_MSG_THREAD_55, m_msg_thread_55);
// 	DDX_Control(pDX, IDC_MSG_THREAD_56, m_msg_thread_56);
// 	DDX_Control(pDX, IDC_MSG_THREAD_57, m_msg_thread_57);
// 	DDX_Control(pDX, IDC_MSG_THREAD_58, m_msg_thread_58);
// 	DDX_Control(pDX, IDC_MSG_THREAD_59, m_msg_thread_59);
// 	DDX_Control(pDX, IDC_MSG_THREAD_60, m_msg_thread_60);
// 	DDX_Control(pDX, IDC_MSG_THREAD_61, m_msg_thread_61);
// 	DDX_Control(pDX, IDC_MSG_THREAD_62, m_msg_thread_62);
// 	DDX_Control(pDX, IDC_MSG_THREAD_63, m_msg_thread_63);
// 	DDX_Control(pDX, IDC_MSG_THREAD_64, m_msg_thread_64);
// 	DDX_Control(pDX, IDC_MSG_THREAD_65, m_msg_thread_65);
	
	
	DDX_Control(pDX, IDC_DATA_THREAD_1, m_data_thread_1);
	DDX_Control(pDX, IDC_DATA_THREAD_2, m_data_thread_2);
	DDX_Control(pDX, IDC_DATA_THREAD_3, m_data_thread_3);
	DDX_Control(pDX, IDC_DATA_THREAD_4, m_data_thread_4);
	DDX_Control(pDX, IDC_DATA_THREAD_5, m_data_thread_5);
	DDX_Control(pDX, IDC_DATA_THREAD_6, m_data_thread_6);
	DDX_Control(pDX, IDC_DATA_THREAD_7, m_data_thread_7);
	DDX_Control(pDX, IDC_DATA_THREAD_8, m_data_thread_8);
	DDX_Control(pDX, IDC_DATA_THREAD_9, m_data_thread_9);
	DDX_Control(pDX, IDC_DATA_THREAD_10, m_data_thread_10);
	DDX_Control(pDX, IDC_DATA_THREAD_11, m_data_thread_11);
	DDX_Control(pDX, IDC_DATA_THREAD_12, m_data_thread_12);
	DDX_Control(pDX, IDC_DATA_THREAD_13, m_data_thread_13);
	DDX_Control(pDX, IDC_DATA_THREAD_14, m_data_thread_14);
	DDX_Control(pDX, IDC_DATA_THREAD_15, m_data_thread_15);
	DDX_Control(pDX, IDC_DATA_THREAD_16, m_data_thread_16);
	DDX_Control(pDX, IDC_DATA_THREAD_17, m_data_thread_17);
	DDX_Control(pDX, IDC_DATA_THREAD_18, m_data_thread_18);
	DDX_Control(pDX, IDC_DATA_THREAD_19, m_data_thread_19);
	DDX_Control(pDX, IDC_DATA_THREAD_20, m_data_thread_20);
	DDX_Control(pDX, IDC_DATA_THREAD_21, m_data_thread_21);
	DDX_Control(pDX, IDC_DATA_THREAD_22, m_data_thread_22);
	DDX_Control(pDX, IDC_DATA_THREAD_23, m_data_thread_23);
	DDX_Control(pDX, IDC_DATA_THREAD_24, m_data_thread_24);
	DDX_Control(pDX, IDC_DATA_THREAD_25, m_data_thread_25);
	DDX_Control(pDX, IDC_DATA_THREAD_26, m_data_thread_26);
	DDX_Control(pDX, IDC_DATA_THREAD_27, m_data_thread_27);
	DDX_Control(pDX, IDC_DATA_THREAD_28, m_data_thread_28);
	DDX_Control(pDX, IDC_DATA_THREAD_29, m_data_thread_29);
	DDX_Control(pDX, IDC_DATA_THREAD_30, m_data_thread_30);
	DDX_Control(pDX, IDC_DATA_THREAD_31, m_data_thread_31);
	DDX_Control(pDX, IDC_DATA_THREAD_32, m_data_thread_32);
	DDX_Control(pDX, IDC_DATA_THREAD_33, m_data_thread_33);
	DDX_Control(pDX, IDC_DATA_THREAD_34, m_data_thread_34);
	DDX_Control(pDX, IDC_DATA_THREAD_35, m_data_thread_35);
	DDX_Control(pDX, IDC_DATA_THREAD_36, m_data_thread_36);
	DDX_Control(pDX, IDC_DATA_THREAD_37, m_data_thread_37);
	DDX_Control(pDX, IDC_DATA_THREAD_38, m_data_thread_38);
	DDX_Control(pDX, IDC_DATA_THREAD_39, m_data_thread_39);
	DDX_Control(pDX, IDC_DATA_THREAD_40, m_data_thread_40);
	DDX_Control(pDX, IDC_DATA_THREAD_41, m_data_thread_41);
	DDX_Control(pDX, IDC_DATA_THREAD_42, m_data_thread_42);
	DDX_Control(pDX, IDC_DATA_THREAD_43, m_data_thread_43);
	DDX_Control(pDX, IDC_DATA_THREAD_44, m_data_thread_44);
	DDX_Control(pDX, IDC_DATA_THREAD_45, m_data_thread_45);
	DDX_Control(pDX, IDC_DATA_THREAD_46, m_data_thread_46);
	DDX_Control(pDX, IDC_DATA_THREAD_47, m_data_thread_47);
	DDX_Control(pDX, IDC_DATA_THREAD_48, m_data_thread_48);
	DDX_Control(pDX, IDC_DATA_THREAD_49, m_data_thread_49);
	DDX_Control(pDX, IDC_DATA_THREAD_50, m_data_thread_50);
	DDX_Control(pDX, IDC_DATA_THREAD_51, m_data_thread_51);
	DDX_Control(pDX, IDC_DATA_THREAD_52, m_data_thread_52);
 	DDX_Control(pDX, IDC_DATA_THREAD_53, m_data_thread_53);
 	DDX_Control(pDX, IDC_DATA_THREAD_54, m_data_thread_54);
// 	DDX_Control(pDX, IDC_DATA_THREAD_55, m_data_thread_55);
// 	DDX_Control(pDX, IDC_DATA_THREAD_56, m_data_thread_56);
// 	DDX_Control(pDX, IDC_DATA_THREAD_57, m_data_thread_57);
// 	DDX_Control(pDX, IDC_DATA_THREAD_58, m_data_thread_58);
// 	DDX_Control(pDX, IDC_DATA_THREAD_59, m_data_thread_59);
// 	DDX_Control(pDX, IDC_DATA_THREAD_60, m_data_thread_60);
// 	DDX_Control(pDX, IDC_DATA_THREAD_61, m_data_thread_61);
// 	DDX_Control(pDX, IDC_DATA_THREAD_62, m_data_thread_62);
// 	DDX_Control(pDX, IDC_DATA_THREAD_63, m_data_thread_63);
// 	DDX_Control(pDX, IDC_DATA_THREAD_64, m_data_thread_64);
// 	DDX_Control(pDX, IDC_DATA_THREAD_65, m_data_thread_65);


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_List_Step, CFormView)
	//{{AFX_MSG_MAP(CScreen_List_Step)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STEP_SAVE, OnBtnStepSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step diagnostics

#ifdef _DEBUG
void CScreen_List_Step::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_List_Step::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step message handlers

void CScreen_List_Step::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	/* ************************************************************************** */
	/* 컨트롤에 설정할 폰트 생성한다                                              */
	/* ************************************************************************** */
	mp_step_list_font = NULL;
	mp_step_list_font = new CFont;
	mp_step_list_font->CreateFont(15,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */

	OnList_Step_GroupBox_Set();
	OnList_Step_Lable_Set();

	mn_timer_kill = FALSE;          // 타이머 삭제 요청 플래그 초기화
	SetTimer(TMR_STEP, 250, NULL);  // 쓰레드 스텝 출력 타이머 생성
}


void CScreen_List_Step::OnTimer(UINT nIDEvent) 
{
	int i;

	if (nIDEvent == TMR_STEP) 
	{
		if (mn_timer_kill != TRUE)
		{
			OnList_Step_Data_Set();			// 스텝 정보 클래스 변수에 설정하는 함수
			OnList_Step_Data_Display();		// 스텝 정보 화면 출력 함수
		}
		else
		{
			KillTimer(TMR_STEP);			// 타이머 재 삭제

			On_Step_List_File_Name_Init();  // 쓰레드 스텝 저장 파일 정보 설정 함수

			////2014,1210
			for (i = 0; i < 18; i++)
			{
				int nRet = Func.Handler_Recovery_Varible_Data(DATA_WRITE, i, 0);
			}
			////
			////2014,1210
				int nRet = Func.Handler_Recovery_Varible_Data(DATA_READ, 0, 0);
			////

		}
	}

	CFormView::OnTimer(nIDEvent);
}

void CScreen_List_Step::OnList_Step_GroupBox_Set()
{
	CSxLogFont step_list_font(15,FW_SEMIBOLD,false,"MS Sans Serif");

//	m_group_step_list.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
//	m_group_step_list.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
 	m_group_step_list.SetFont(step_list_font);
	m_group_step_list.SetCatptionTextColor(BLUE_C);
	m_group_step_list.SetBorderColor(BLUE_C);
	m_group_step_list.SetFontBold(TRUE);


}

void CScreen_List_Step::OnList_Step_Lable_Set()
{
	// 여기부터 수정..
	/* ************************************************************************** */
	/* 타이틀 출력 칼라 텍스트 박스 생성한다                                      */
	/* ************************************************************************** */	
/////// Module
	m_msg_thread_6.SetText(" 1. Rbt Module Run Step");
	m_msg_thread_6.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_6.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_6.SetFontBold(TRUE);
	m_msg_thread_6.SetFontName("MS Sans Serif");
	m_msg_thread_6.SetFontSize(12);

	m_msg_thread_13.SetText(" 2. Stacker Module Transfer Step");
	m_msg_thread_13.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_13.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_13.SetFontBold(TRUE);
	m_msg_thread_13.SetFontName("MS Sans Serif");
	m_msg_thread_13.SetFontSize(12);

	m_msg_thread_22.SetText(" 3. Stacker Module Stacker1Move Step");
	m_msg_thread_22.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_22.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_22.SetFontBold(TRUE);
	m_msg_thread_22.SetFontName("MS Sans Serif");
	m_msg_thread_22.SetFontSize(12);

	m_msg_thread_23.SetText(" 4. Stacker Module Stacker2Move Step");
	m_msg_thread_23.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_23.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_23.SetFontBold(TRUE);
	m_msg_thread_23.SetFontName("MS Sans Serif");
	m_msg_thread_23.SetFontSize(12);

	m_msg_thread_41.SetText(" 5. Stacker Module Tray Back Step");
	m_msg_thread_41.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_41.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_41.SetFontBold(TRUE);
	m_msg_thread_41.SetFontName("MS Sans Serif");
	m_msg_thread_41.SetFontSize(12);

	m_msg_thread_42.SetText(" 6. Stacker Module ReadyPosStep[0]");
	m_msg_thread_42.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_42.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_42.SetFontBold(TRUE);
	m_msg_thread_42.SetFontName("MS Sans Serif");
	m_msg_thread_42.SetFontSize(12);

	m_msg_thread_43.SetText(" 7. Stacker Module ReadyPosStep[1]");
	m_msg_thread_43.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_43.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_43.SetFontBold(TRUE);
	m_msg_thread_43.SetFontName("MS Sans Serif");
	m_msg_thread_43.SetFontSize(12);

	m_msg_thread_44.SetText(" 8. Stacker Module Tray Move Step");
	m_msg_thread_44.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_44.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_44.SetFontBold(TRUE);
	m_msg_thread_44.SetFontName("MS Sans Serif");
	m_msg_thread_44.SetFontSize(12);

/////// HS Left
	m_msg_thread_5.SetText(" 1. Rbt LeftHeat Run Step");
	m_msg_thread_5.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_5.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_5.SetFontBold(TRUE);
	m_msg_thread_5.SetFontName("MS Sans Serif");
	m_msg_thread_5.SetFontSize(12);

	m_msg_thread_12.SetText(" 2. Stacker LeftHeat Transfer Step");
	m_msg_thread_12.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_12.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_12.SetFontBold(TRUE);
	m_msg_thread_12.SetFontName("MS Sans Serif");
	m_msg_thread_12.SetFontSize(12);

	m_msg_thread_20.SetText(" 3. Stacker LeftHeat Stacker1Move Step");
	m_msg_thread_20.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_20.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_20.SetFontBold(TRUE);
	m_msg_thread_20.SetFontName("MS Sans Serif");
	m_msg_thread_20.SetFontSize(12);

	m_msg_thread_21.SetText(" 4. Stacker LeftHeat Stacker2Move Step");
	m_msg_thread_21.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_21.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_21.SetFontBold(TRUE);
	m_msg_thread_21.SetFontName("MS Sans Serif");
	m_msg_thread_21.SetFontSize(12);

	m_msg_thread_28.SetText(" 5. Rbt LeftHeat Module Place Step");
	m_msg_thread_28.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_28.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_28.SetFontBold(TRUE);
	m_msg_thread_28.SetFontName("MS Sans Serif");
	m_msg_thread_28.SetFontSize(12);

	m_msg_thread_37.SetText(" 6. Stacker LeftHeat Tray Move Step");
	m_msg_thread_37.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_37.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_37.SetFontBold(TRUE);
	m_msg_thread_37.SetFontName("MS Sans Serif");
	m_msg_thread_37.SetFontSize(12);

	m_msg_thread_38.SetText(" 7. Stacker LeftHeat Tray Back Step");
	m_msg_thread_38.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_38.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_38.SetFontBold(TRUE);
	m_msg_thread_38.SetFontName("MS Sans Serif");
	m_msg_thread_38.SetFontSize(12);

	m_msg_thread_39.SetText(" 8. Stacker LeftHeat ReadyPosStep[0]");
	m_msg_thread_39.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_39.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_39.SetFontBold(TRUE);
	m_msg_thread_39.SetFontName("MS Sans Serif");
	m_msg_thread_39.SetFontSize(12);

	m_msg_thread_40.SetText(" 9. Stacker LeftHeat ReadyPosStep[1]");
	m_msg_thread_40.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_40.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_40.SetFontBold(TRUE);
	m_msg_thread_40.SetFontName("MS Sans Serif");
	m_msg_thread_40.SetFontSize(12);

	m_msg_thread_54.SetText(" 10. Stacker LeftHeat Tray Pick Step");
	m_msg_thread_54.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_54.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_54.SetFontBold(TRUE);
	m_msg_thread_54.SetFontName("MS Sans Serif");
	m_msg_thread_54.SetFontSize(12);

/////// HS Right
	m_msg_thread_7.SetText(" 1. Rbt RightHeat Run Step");
	m_msg_thread_7.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_7.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_7.SetFontBold(TRUE);
	m_msg_thread_7.SetFontName("MS Sans Serif");
	m_msg_thread_7.SetFontSize(12);

	m_msg_thread_14.SetText(" 2. Stacker RightHeat Transfer Step");
	m_msg_thread_14.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_14.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_14.SetFontBold(TRUE);
	m_msg_thread_14.SetFontName("MS Sans Serif");
	m_msg_thread_14.SetFontSize(12);
	
	m_msg_thread_24.SetText(" 3. Stacker RightHeat Stacker1Move Step");
	m_msg_thread_24.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_24.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_24.SetFontBold(TRUE);
	m_msg_thread_24.SetFontName("MS Sans Serif");
	m_msg_thread_24.SetFontSize(12);

	m_msg_thread_25.SetText(" 4. Stacker RightHeat Stacker2Move Step");
	m_msg_thread_25.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_25.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_25.SetFontBold(TRUE);
	m_msg_thread_25.SetFontName("MS Sans Serif");
	m_msg_thread_25.SetFontSize(12);

	m_msg_thread_29.SetText(" 5. Rbt RightHeat Tray Pick Step");
	m_msg_thread_29.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_29.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_29.SetFontBold(TRUE);
	m_msg_thread_29.SetFontName("MS Sans Serif");
	m_msg_thread_29.SetFontSize(12);

	m_msg_thread_30.SetText(" 6. Rbt RightHeat Module Place Step");
	m_msg_thread_30.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_30.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_30.SetFontBold(TRUE);
	m_msg_thread_30.SetFontName("MS Sans Serif");
	m_msg_thread_30.SetFontSize(12);

	m_msg_thread_45.SetText(" 7. Stacker RightHeat Tray Move Step");
	m_msg_thread_45.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_45.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_45.SetFontBold(TRUE);
	m_msg_thread_45.SetFontName("MS Sans Serif");
	m_msg_thread_45.SetFontSize(12);

	m_msg_thread_46.SetText(" 8. Stacker RightHeat Tray Back Step");
	m_msg_thread_46.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_46.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_46.SetFontBold(TRUE);
	m_msg_thread_46.SetFontName("MS Sans Serif");
	m_msg_thread_46.SetFontSize(12);

	m_msg_thread_47.SetText(" 9. Stacker RightHeat ReadyPosStep[0]");
	m_msg_thread_47.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_47.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_47.SetFontBold(TRUE);
	m_msg_thread_47.SetFontName("MS Sans Serif");
	m_msg_thread_47.SetFontSize(12);

	m_msg_thread_48.SetText(" 10. Stacker RightHeat ReadyPosStep[1]");
	m_msg_thread_48.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_48.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_48.SetFontBold(TRUE);
	m_msg_thread_48.SetFontName("MS Sans Serif");
	m_msg_thread_48.SetFontSize(12);


/////// Work Buffer
	m_msg_thread_32.SetText(" 1. Rbt Work RunBuff1 Step");
	m_msg_thread_32.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_32.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_32.SetFontBold(TRUE);
	m_msg_thread_32.SetFontName("MS Sans Serif");
	m_msg_thread_32.SetFontSize(12);

	m_msg_thread_33.SetText(" 2. Rbt Work RunBuff2 Step");
	m_msg_thread_33.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_33.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_33.SetFontBold(TRUE);
	m_msg_thread_33.SetFontName("MS Sans Serif");
	m_msg_thread_33.SetFontSize(12);

	m_msg_thread_34.SetText(" 3. Rbt Work RunBuff3 Step");
	m_msg_thread_34.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_34.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_34.SetFontBold(TRUE);
	m_msg_thread_34.SetFontName("MS Sans Serif");
	m_msg_thread_34.SetFontSize(12);

	m_msg_thread_35.SetText(" 4. Rbt Work RunBuff4 Step");
	m_msg_thread_35.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_35.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_35.SetFontBold(TRUE);
	m_msg_thread_35.SetFontName("MS Sans Serif");
	m_msg_thread_35.SetFontSize(12);


/////// Sorter
	m_msg_thread_8.SetText(" 1. Rbt Sorter Sort Step");
	m_msg_thread_8.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_8.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_8.SetFontBold(TRUE);
	m_msg_thread_8.SetFontName("MS Sans Serif");
	m_msg_thread_8.SetFontSize(12);
 
	m_msg_thread_15.SetText(" 2. Stacker Sorter Transfer Step");
	m_msg_thread_15.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_15.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_15.SetFontBold(TRUE);
	m_msg_thread_15.SetFontName("MS Sans Serif");
	m_msg_thread_15.SetFontSize(12);

	m_msg_thread_19.SetText(" 3. Rbt Sorter Unload Move Step");
	m_msg_thread_19.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_19.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_19.SetFontBold(TRUE);
	m_msg_thread_19.SetFontName("MS Sans Serif");
	m_msg_thread_19.SetFontSize(12);

	m_msg_thread_26.SetText(" 4. Stacker Sorter Stacker1Move Step");
	m_msg_thread_26.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_26.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_26.SetFontBold(TRUE);
	m_msg_thread_26.SetFontName("MS Sans Serif");
	m_msg_thread_26.SetFontSize(12);

	m_msg_thread_27.SetText(" 5. Stacker Sorter Stacker2Move Step");
	m_msg_thread_27.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_27.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_27.SetFontBold(TRUE);
	m_msg_thread_27.SetFontName("MS Sans Serif");
	m_msg_thread_27.SetFontSize(12);

	m_msg_thread_31.SetText(" 6. Rbt Sorter Module_Pick_Step");
	m_msg_thread_31.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_31.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_31.SetFontBold(TRUE);
	m_msg_thread_31.SetFontName("MS Sans Serif");
	m_msg_thread_31.SetFontSize(12);

	m_msg_thread_49.SetText(" 7. Stacker Sorter Run Step");
	m_msg_thread_49.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_49.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_49.SetFontBold(TRUE);
	m_msg_thread_49.SetFontName("MS Sans Serif");
	m_msg_thread_49.SetFontSize(12);

	m_msg_thread_50.SetText(" 8. Stacker Sorter ReadyPosStep[0]");
	m_msg_thread_50.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_50.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_50.SetFontBold(TRUE);
	m_msg_thread_50.SetFontName("MS Sans Serif");
	m_msg_thread_50.SetFontSize(12);

	m_msg_thread_51.SetText(" 9. Stacker Sorter ReadyPosStep[1]");
	m_msg_thread_51.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_51.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_51.SetFontBold(TRUE);
	m_msg_thread_51.SetFontName("MS Sans Serif");
	m_msg_thread_51.SetFontSize(12);

	m_msg_thread_52.SetText(" 10. Stacker Sorter Work Pos Step");
	m_msg_thread_52.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_52.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_52.SetFontBold(TRUE);
	m_msg_thread_52.SetFontName("MS Sans Serif");
	m_msg_thread_52.SetFontSize(12);

	m_msg_thread_53.SetText(" 11. Stacker Sorter Tray Back Step");
	m_msg_thread_53.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_53.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_53.SetFontBold(TRUE);
	m_msg_thread_53.SetFontName("MS Sans Serif");
	m_msg_thread_53.SetFontSize(12);

/////// Clip
	m_msg_thread_11.SetText(" 1. Stacker Clip Transfer Step");
	m_msg_thread_11.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_11.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_11.SetFontBold(TRUE);
	m_msg_thread_11.SetFontName("MS Sans Serif");
	m_msg_thread_11.SetFontSize(12);

	m_msg_thread_16.SetText(" 2. Rbt Clip Insert Step");
	m_msg_thread_16.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_16.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_16.SetFontBold(TRUE);
	m_msg_thread_16.SetFontName("MS Sans Serif");
	m_msg_thread_16.SetFontSize(12);

	m_msg_thread_17.SetText(" 3. Rbt Clip Clamp Step");
	m_msg_thread_17.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_17.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_17.SetFontBold(TRUE);
	m_msg_thread_17.SetFontName("MS Sans Serif");
	m_msg_thread_17.SetFontSize(12);

	m_msg_thread_18.SetText(" 4. Rbt Clip Buffer Step");
	m_msg_thread_18.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_18.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_18.SetFontBold(TRUE);
	m_msg_thread_18.SetFontName("MS Sans Serif");
	m_msg_thread_18.SetFontSize(12);

	m_msg_thread_36.SetText(" 5. Stacker Clip Run Step");
	m_msg_thread_36.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_36.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_36.SetFontBold(TRUE);
	m_msg_thread_36.SetFontName("MS Sans Serif");
	m_msg_thread_36.SetFontSize(12);

	
// 	m_msg_thread_1.SetText(" 0. ");
// 	m_msg_thread_1.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_1.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_1.SetFontBold(TRUE);
// 	m_msg_thread_1.SetFontName("MS Sans Serif");
// 	m_msg_thread_1.SetFontSize(12);
// 
// 	m_msg_thread_2.SetText(" 1. ");
// 	m_msg_thread_2.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_2.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_2.SetFontBold(TRUE);
// 	m_msg_thread_2.SetFontName("MS Sans Serif");
// 	m_msg_thread_2.SetFontSize(12);
// 
// 	m_msg_thread_3.SetText(" 2. ");
// 	m_msg_thread_3.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_3.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_3.SetFontBold(TRUE);
// 	m_msg_thread_3.SetFontName("MS Sans Serif");
// 	m_msg_thread_3.SetFontSize(12);
// 
// 	m_msg_thread_4.SetText(" 3. ");
// 	m_msg_thread_4.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_4.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_4.SetFontBold(TRUE);
// 	m_msg_thread_4.SetFontName("MS Sans Serif");
// 	m_msg_thread_4.SetFontSize(12);
// 
// 	m_msg_thread_9.SetText(" 8. ");
// 	m_msg_thread_9.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_9.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_9.SetFontBold(TRUE);
// 	m_msg_thread_9.SetFontName("MS Sans Serif");
// 	m_msg_thread_9.SetFontSize(12);
// 
// 	m_msg_thread_10.SetText(" 9. ");
// 	m_msg_thread_10.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_10.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_10.SetFontBold(TRUE);
// 	m_msg_thread_10.SetFontName("MS Sans Serif");
// 	m_msg_thread_10.SetFontSize(12);
//
// 	m_msg_thread_55.SetText(" 54. ");
// 	m_msg_thread_55.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_55.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_55.SetFontBold(TRUE);
// 	m_msg_thread_55.SetFontName("MS Sans Serif");
// 	m_msg_thread_55.SetFontSize(12);
// 
// 	m_msg_thread_56.SetText(" 55. ");
// 	m_msg_thread_56.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_56.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_56.SetFontBold(TRUE);
// 	m_msg_thread_56.SetFontName("MS Sans Serif");
// 	m_msg_thread_56.SetFontSize(12);
// 
// 	
// 	m_msg_thread_57.SetText(" 56. ");
// 	m_msg_thread_57.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_57.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_57.SetFontBold(TRUE);
// 	m_msg_thread_57.SetFontName("MS Sans Serif");
// 	m_msg_thread_57.SetFontSize(12);
// 
// 	m_msg_thread_58.SetText(" 57. ");
// 	m_msg_thread_58.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_58.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_58.SetFontBold(TRUE);
// 	m_msg_thread_58.SetFontName("MS Sans Serif");
// 	m_msg_thread_58.SetFontSize(12);
// 
// 	m_msg_thread_59.SetText(" 58. ");
// 	m_msg_thread_59.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_59.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_59.SetFontBold(TRUE);
// 	m_msg_thread_59.SetFontName("MS Sans Serif");
// 	m_msg_thread_59.SetFontSize(12);
// 
// 	m_msg_thread_60.SetText(" 59. ");
// 	m_msg_thread_60.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_60.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_60.SetFontBold(TRUE);
// 	m_msg_thread_60.SetFontName("MS Sans Serif");
// 	m_msg_thread_60.SetFontSize(12);
// 
// 
// 	m_msg_thread_61.SetText(" 60. ");
// 	m_msg_thread_61.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_61.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_61.SetFontBold(TRUE);
// 	m_msg_thread_61.SetFontName("MS Sans Serif");
// 	m_msg_thread_61.SetFontSize(12);
// 
// 	m_msg_thread_62.SetText(" 61. ");
// 	m_msg_thread_62.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_62.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_62.SetFontBold(TRUE);
// 	m_msg_thread_62.SetFontName("MS Sans Serif");
// 	m_msg_thread_62.SetFontSize(12);
// 
// 	m_msg_thread_63.SetText(" 62. ");
// 	m_msg_thread_63.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_63.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_63.SetFontBold(TRUE);
// 	m_msg_thread_63.SetFontName("MS Sans Serif");
// 	m_msg_thread_63.SetFontSize(12);
// 
// 	m_msg_thread_64.SetText(" 63. ");
// 	m_msg_thread_64.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_64.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_64.SetFontBold(TRUE);
// 	m_msg_thread_64.SetFontName("MS Sans Serif");
// 	m_msg_thread_64.SetFontSize(12);
// 
// 	m_msg_thread_65.SetText(" 64. ");
// 	m_msg_thread_65.SetTextColor(RGB(25, 25, 85));
// 	m_msg_thread_65.SetBkColor(RGB(180, 230, 250));
// 	m_msg_thread_65.SetFontBold(TRUE);
// 	m_msg_thread_65.SetFontName("MS Sans Serif");
// 	m_msg_thread_65.SetFontSize(12);
// 

	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 쓰레드 스텝 출력 칼라 텍스트 박스 생성한다                                 */
	/* ************************************************************************** */
	m_data_thread_1.SetFont(mp_step_list_font);
	m_data_thread_1.SetCenterText();
	m_data_thread_1.SetColor(RGB(0,0,255));
	m_data_thread_1.SetGradientColor(RGB(0,0,0));
	m_data_thread_1.SetTextColor(RGB(255,255,255));

	m_data_thread_2.SetFont(mp_step_list_font);
	m_data_thread_2.SetCenterText();
	m_data_thread_2.SetColor(RGB(0,0,255));
	m_data_thread_2.SetGradientColor(RGB(0,0,0));
	m_data_thread_2.SetTextColor(RGB(255,255,255));

	m_data_thread_3.SetFont(mp_step_list_font);
	m_data_thread_3.SetCenterText();
	m_data_thread_3.SetColor(RGB(0,0,255));
	m_data_thread_3.SetGradientColor(RGB(0,0,0));
	m_data_thread_3.SetTextColor(RGB(255,255,255));

	m_data_thread_4.SetFont(mp_step_list_font);
	m_data_thread_4.SetCenterText();
	m_data_thread_4.SetColor(RGB(0,0,255));
	m_data_thread_4.SetGradientColor(RGB(0,0,0));
	m_data_thread_4.SetTextColor(RGB(255,255,255));

	m_data_thread_5.SetFont(mp_step_list_font);
	m_data_thread_5.SetCenterText();
	m_data_thread_5.SetColor(RGB(0,0,255));
	m_data_thread_5.SetGradientColor(RGB(0,0,0));
	m_data_thread_5.SetTextColor(RGB(255,255,255));

	m_data_thread_6.SetFont(mp_step_list_font);
	m_data_thread_6.SetCenterText();
	m_data_thread_6.SetColor(RGB(0,0,255));
	m_data_thread_6.SetGradientColor(RGB(0,0,0));
	m_data_thread_6.SetTextColor(RGB(255,255,255));

	m_data_thread_7.SetFont(mp_step_list_font);
	m_data_thread_7.SetCenterText();
	m_data_thread_7.SetColor(RGB(0,0,255));
	m_data_thread_7.SetGradientColor(RGB(0,0,0));
	m_data_thread_7.SetTextColor(RGB(255,255,255));

	m_data_thread_8.SetFont(mp_step_list_font);
	m_data_thread_8.SetCenterText();
	m_data_thread_8.SetColor(RGB(0,0,255));
	m_data_thread_8.SetGradientColor(RGB(0,0,0));
	m_data_thread_8.SetTextColor(RGB(255,255,255));

	m_data_thread_9.SetFont(mp_step_list_font);
	m_data_thread_9.SetCenterText();
	m_data_thread_9.SetColor(RGB(0,0,255));
	m_data_thread_9.SetGradientColor(RGB(0,0,0));
	m_data_thread_9.SetTextColor(RGB(255,255,255));

	m_data_thread_10.SetFont(mp_step_list_font);
	m_data_thread_10.SetCenterText();
	m_data_thread_10.SetColor(RGB(0,0,255));
	m_data_thread_10.SetGradientColor(RGB(0,0,0));
	m_data_thread_10.SetTextColor(RGB(255,255,255));

	m_data_thread_11.SetFont(mp_step_list_font);
	m_data_thread_11.SetCenterText();
	m_data_thread_11.SetColor(RGB(0,0,255));
	m_data_thread_11.SetGradientColor(RGB(0,0,0));
	m_data_thread_11.SetTextColor(RGB(255,255,255));

	m_data_thread_12.SetFont(mp_step_list_font);
	m_data_thread_12.SetCenterText();
	m_data_thread_12.SetColor(RGB(0,0,255));
	m_data_thread_12.SetGradientColor(RGB(0,0,0));
	m_data_thread_12.SetTextColor(RGB(255,255,255));

	m_data_thread_13.SetFont(mp_step_list_font);
	m_data_thread_13.SetCenterText();
	m_data_thread_13.SetColor(RGB(0,0,255));
	m_data_thread_13.SetGradientColor(RGB(0,0,0));
	m_data_thread_13.SetTextColor(RGB(255,255,255));

	m_data_thread_14.SetFont(mp_step_list_font);
	m_data_thread_14.SetCenterText();
	m_data_thread_14.SetColor(RGB(0,0,255));
	m_data_thread_14.SetGradientColor(RGB(0,0,0));
	m_data_thread_14.SetTextColor(RGB(255,255,255));

	m_data_thread_15.SetFont(mp_step_list_font);
	m_data_thread_15.SetCenterText();
	m_data_thread_15.SetColor(RGB(0,0,255));
	m_data_thread_15.SetGradientColor(RGB(0,0,0));
	m_data_thread_15.SetTextColor(RGB(255,255,255));

	m_data_thread_16.SetFont(mp_step_list_font);
	m_data_thread_16.SetCenterText();
	m_data_thread_16.SetColor(RGB(0,0,255));
	m_data_thread_16.SetGradientColor(RGB(0,0,0));
	m_data_thread_16.SetTextColor(RGB(255,255,255));

	m_data_thread_17.SetFont(mp_step_list_font);
	m_data_thread_17.SetCenterText();
	m_data_thread_17.SetColor(RGB(0,0,255));
	m_data_thread_17.SetGradientColor(RGB(0,0,0));
	m_data_thread_17.SetTextColor(RGB(255,255,255));

	m_data_thread_18.SetFont(mp_step_list_font);
	m_data_thread_18.SetCenterText();
	m_data_thread_18.SetColor(RGB(0,0,255));
	m_data_thread_18.SetGradientColor(RGB(0,0,0));
	m_data_thread_18.SetTextColor(RGB(255,255,255));

	m_data_thread_19.SetFont(mp_step_list_font);
	m_data_thread_19.SetCenterText();
	m_data_thread_19.SetColor(RGB(0,0,255));
	m_data_thread_19.SetGradientColor(RGB(0,0,0));
	m_data_thread_19.SetTextColor(RGB(255,255,255));

	m_data_thread_20.SetFont(mp_step_list_font);
	m_data_thread_20.SetCenterText();
	m_data_thread_20.SetColor(RGB(0,0,255));
	m_data_thread_20.SetGradientColor(RGB(0,0,0));
	m_data_thread_20.SetTextColor(RGB(255,255,255));

	m_data_thread_21.SetFont(mp_step_list_font);
	m_data_thread_21.SetCenterText();
	m_data_thread_21.SetColor(RGB(0,0,255));
	m_data_thread_21.SetGradientColor(RGB(0,0,0));
	m_data_thread_21.SetTextColor(RGB(255,255,255));

	m_data_thread_22.SetFont(mp_step_list_font);
	m_data_thread_22.SetCenterText();
	m_data_thread_22.SetColor(RGB(0,0,255));
	m_data_thread_22.SetGradientColor(RGB(0,0,0));
	m_data_thread_22.SetTextColor(RGB(255,255,255));

	m_data_thread_23.SetFont(mp_step_list_font);
	m_data_thread_23.SetCenterText();
	m_data_thread_23.SetColor(RGB(0,0,255));
	m_data_thread_23.SetGradientColor(RGB(0,0,0));
	m_data_thread_23.SetTextColor(RGB(255,255,255));

	m_data_thread_24.SetFont(mp_step_list_font);
	m_data_thread_24.SetCenterText();
	m_data_thread_24.SetColor(RGB(0,0,255));
	m_data_thread_24.SetGradientColor(RGB(0,0,0));
	m_data_thread_24.SetTextColor(RGB(255,255,255));

	m_data_thread_25.SetFont(mp_step_list_font);
	m_data_thread_25.SetCenterText();
	m_data_thread_25.SetColor(RGB(0,0,255));
	m_data_thread_25.SetGradientColor(RGB(0,0,0));
	m_data_thread_25.SetTextColor(RGB(255,255,255));

	m_data_thread_26.SetFont(mp_step_list_font);
	m_data_thread_26.SetCenterText();
	m_data_thread_26.SetColor(RGB(0,0,255));
	m_data_thread_26.SetGradientColor(RGB(0,0,0));
	m_data_thread_26.SetTextColor(RGB(255,255,255));

	m_data_thread_27.SetFont(mp_step_list_font);
	m_data_thread_27.SetCenterText();
	m_data_thread_27.SetColor(RGB(0,0,255));
	m_data_thread_27.SetGradientColor(RGB(0,0,0));
	m_data_thread_27.SetTextColor(RGB(255,255,255));

	m_data_thread_28.SetFont(mp_step_list_font);
	m_data_thread_28.SetCenterText();
	m_data_thread_28.SetColor(RGB(0,0,255));
	m_data_thread_28.SetGradientColor(RGB(0,0,0));
	m_data_thread_28.SetTextColor(RGB(255,255,255));

	m_data_thread_29.SetFont(mp_step_list_font);
	m_data_thread_29.SetCenterText();
	m_data_thread_29.SetColor(RGB(0,0,255));
	m_data_thread_29.SetGradientColor(RGB(0,0,0));
	m_data_thread_29.SetTextColor(RGB(255,255,255));

	m_data_thread_30.SetFont(mp_step_list_font);
	m_data_thread_30.SetCenterText();
	m_data_thread_30.SetColor(RGB(0,0,255));
	m_data_thread_30.SetGradientColor(RGB(0,0,0));
	m_data_thread_30.SetTextColor(RGB(255,255,255));
	
	m_data_thread_31.SetFont(mp_step_list_font);
	m_data_thread_31.SetCenterText();
	m_data_thread_31.SetColor(RGB(0,0,255));
	m_data_thread_31.SetGradientColor(RGB(0,0,0));
	m_data_thread_31.SetTextColor(RGB(255,255,255));

	m_data_thread_32.SetFont(mp_step_list_font);
	m_data_thread_32.SetCenterText();
	m_data_thread_32.SetColor(RGB(0,0,255));
	m_data_thread_32.SetGradientColor(RGB(0,0,0));
	m_data_thread_32.SetTextColor(RGB(255,255,255));

	m_data_thread_33.SetFont(mp_step_list_font);
	m_data_thread_33.SetCenterText();
	m_data_thread_33.SetColor(RGB(0,0,255));
	m_data_thread_33.SetGradientColor(RGB(0,0,0));
	m_data_thread_33.SetTextColor(RGB(255,255,255));

	m_data_thread_34.SetFont(mp_step_list_font);
	m_data_thread_34.SetCenterText();
	m_data_thread_34.SetColor(RGB(0,0,255));
	m_data_thread_34.SetGradientColor(RGB(0,0,0));
	m_data_thread_34.SetTextColor(RGB(255,255,255));

	m_data_thread_35.SetFont(mp_step_list_font);
	m_data_thread_35.SetCenterText();
	m_data_thread_35.SetColor(RGB(0,0,255));
	m_data_thread_35.SetGradientColor(RGB(0,0,0));
	m_data_thread_35.SetTextColor(RGB(255,255,255));

	m_data_thread_36.SetFont(mp_step_list_font);
	m_data_thread_36.SetCenterText();
	m_data_thread_36.SetColor(RGB(0,0,255));
	m_data_thread_36.SetGradientColor(RGB(0,0,0));
	m_data_thread_36.SetTextColor(RGB(255,255,255));

	m_data_thread_37.SetFont(mp_step_list_font);
	m_data_thread_37.SetCenterText();
	m_data_thread_37.SetColor(RGB(0,0,255));
	m_data_thread_37.SetGradientColor(RGB(0,0,0));
	m_data_thread_37.SetTextColor(RGB(255,255,255));

	m_data_thread_38.SetFont(mp_step_list_font);
	m_data_thread_38.SetCenterText();
	m_data_thread_38.SetColor(RGB(0,0,255));
	m_data_thread_38.SetGradientColor(RGB(0,0,0));
	m_data_thread_38.SetTextColor(RGB(255,255,255));

	m_data_thread_39.SetFont(mp_step_list_font);
	m_data_thread_39.SetCenterText();
	m_data_thread_39.SetColor(RGB(0,0,255));
	m_data_thread_39.SetGradientColor(RGB(0,0,0));
	m_data_thread_39.SetTextColor(RGB(255,255,255));

	m_data_thread_40.SetFont(mp_step_list_font);
	m_data_thread_40.SetCenterText();
	m_data_thread_40.SetColor(RGB(0,0,255));
	m_data_thread_40.SetGradientColor(RGB(0,0,0));
	m_data_thread_40.SetTextColor(RGB(255,255,255));

	m_data_thread_41.SetFont(mp_step_list_font);
	m_data_thread_41.SetCenterText();
	m_data_thread_41.SetColor(RGB(0,0,255));
	m_data_thread_41.SetGradientColor(RGB(0,0,0));
	m_data_thread_41.SetTextColor(RGB(255,255,255));

	m_data_thread_42.SetFont(mp_step_list_font);
	m_data_thread_42.SetCenterText();
	m_data_thread_42.SetColor(RGB(0,0,255));
	m_data_thread_42.SetGradientColor(RGB(0,0,0));
	m_data_thread_42.SetTextColor(RGB(255,255,255));

	m_data_thread_43.SetFont(mp_step_list_font);
	m_data_thread_43.SetCenterText();
	m_data_thread_43.SetColor(RGB(0,0,255));
	m_data_thread_43.SetGradientColor(RGB(0,0,0));
	m_data_thread_43.SetTextColor(RGB(255,255,255));

	m_data_thread_44.SetFont(mp_step_list_font);
	m_data_thread_44.SetCenterText();
	m_data_thread_44.SetColor(RGB(0,0,255));
	m_data_thread_44.SetGradientColor(RGB(0,0,0));
	m_data_thread_44.SetTextColor(RGB(255,255,255));

	m_data_thread_45.SetFont(mp_step_list_font);
	m_data_thread_45.SetCenterText();
	m_data_thread_45.SetColor(RGB(0,0,255));
	m_data_thread_45.SetGradientColor(RGB(0,0,0));
	m_data_thread_45.SetTextColor(RGB(255,255,255));

	m_data_thread_46.SetFont(mp_step_list_font);
	m_data_thread_46.SetCenterText();
	m_data_thread_46.SetColor(RGB(0,0,255));
	m_data_thread_46.SetGradientColor(RGB(0,0,0));
	m_data_thread_46.SetTextColor(RGB(255,255,255));

	m_data_thread_47.SetFont(mp_step_list_font);
	m_data_thread_47.SetCenterText();
	m_data_thread_47.SetColor(RGB(0,0,255));
	m_data_thread_47.SetGradientColor(RGB(0,0,0));
	m_data_thread_47.SetTextColor(RGB(255,255,255));

	m_data_thread_48.SetFont(mp_step_list_font);
	m_data_thread_48.SetCenterText();
	m_data_thread_48.SetColor(RGB(0,0,255));
	m_data_thread_48.SetGradientColor(RGB(0,0,0));
	m_data_thread_48.SetTextColor(RGB(255,255,255));

	m_data_thread_49.SetFont(mp_step_list_font);
	m_data_thread_49.SetCenterText();
	m_data_thread_49.SetColor(RGB(0,0,255));
	m_data_thread_49.SetGradientColor(RGB(0,0,0));
	m_data_thread_49.SetTextColor(RGB(255,255,255));

	m_data_thread_50.SetFont(mp_step_list_font);
	m_data_thread_50.SetCenterText();
	m_data_thread_50.SetColor(RGB(0,0,255));
	m_data_thread_50.SetGradientColor(RGB(0,0,0));
	m_data_thread_50.SetTextColor(RGB(255,255,255));


	m_data_thread_51.SetFont(mp_step_list_font);
	m_data_thread_51.SetCenterText();
	m_data_thread_51.SetColor(RGB(0,0,255));
	m_data_thread_51.SetGradientColor(RGB(0,0,0));
	m_data_thread_51.SetTextColor(RGB(255,255,255));

	m_data_thread_52.SetFont(mp_step_list_font);
	m_data_thread_52.SetCenterText();
	m_data_thread_52.SetColor(RGB(0,0,255));
	m_data_thread_52.SetGradientColor(RGB(0,0,0));
	m_data_thread_52.SetTextColor(RGB(255,255,255));

	m_data_thread_53.SetFont(mp_step_list_font);
	m_data_thread_53.SetCenterText();
	m_data_thread_53.SetColor(RGB(0,0,255));
	m_data_thread_53.SetGradientColor(RGB(0,0,0));
	m_data_thread_53.SetTextColor(RGB(255,255,255));

	m_data_thread_54.SetFont(mp_step_list_font);
	m_data_thread_54.SetCenterText();
	m_data_thread_54.SetColor(RGB(0,0,255));
	m_data_thread_54.SetGradientColor(RGB(0,0,0));
	m_data_thread_54.SetTextColor(RGB(255,255,255));

// 	m_data_thread_55.SetFont(mp_step_list_font);
// 	m_data_thread_55.SetCenterText();
// 	m_data_thread_55.SetColor(RGB(0,0,255));
// 	m_data_thread_55.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_55.SetTextColor(RGB(255,255,255));
// 
// 	m_data_thread_56.SetFont(mp_step_list_font);
// 	m_data_thread_56.SetCenterText();
// 	m_data_thread_56.SetColor(RGB(0,0,255));
// 	m_data_thread_56.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_56.SetTextColor(RGB(255,255,255));
// 
// 	m_data_thread_57.SetFont(mp_step_list_font);
// 	m_data_thread_57.SetCenterText();
// 	m_data_thread_57.SetColor(RGB(0,0,255));
// 	m_data_thread_57.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_57.SetTextColor(RGB(255,255,255));
// 
// 	m_data_thread_58.SetFont(mp_step_list_font);
// 	m_data_thread_58.SetCenterText();
// 	m_data_thread_58.SetColor(RGB(0,0,255));
// 	m_data_thread_58.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_58.SetTextColor(RGB(255,255,255));
// 
// 	m_data_thread_59.SetFont(mp_step_list_font);
// 	m_data_thread_59.SetCenterText();
// 	m_data_thread_59.SetColor(RGB(0,0,255));
// 	m_data_thread_59.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_59.SetTextColor(RGB(255,255,255));
// 
// 	m_data_thread_60.SetFont(mp_step_list_font);
// 	m_data_thread_60.SetCenterText();
// 	m_data_thread_60.SetColor(RGB(0,0,255));
// 	m_data_thread_60.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_60.SetTextColor(RGB(255,255,255));
// 
// 	m_data_thread_61.SetFont(mp_step_list_font);
// 	m_data_thread_61.SetCenterText();
// 	m_data_thread_61.SetColor(RGB(0,0,255));
// 	m_data_thread_61.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_61.SetTextColor(RGB(255,255,255));
// 
// 	m_data_thread_62.SetFont(mp_step_list_font);
// 	m_data_thread_62.SetCenterText();
// 	m_data_thread_62.SetColor(RGB(0,0,255));
// 	m_data_thread_62.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_62.SetTextColor(RGB(255,255,255));
// 
// 	m_data_thread_63.SetFont(mp_step_list_font);
// 	m_data_thread_63.SetCenterText();
// 	m_data_thread_63.SetColor(RGB(0,0,255));
// 	m_data_thread_63.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_63.SetTextColor(RGB(255,255,255));
// 
// 	m_data_thread_64.SetFont(mp_step_list_font);
// 	m_data_thread_64.SetCenterText();
// 	m_data_thread_64.SetColor(RGB(0,0,255));
// 	m_data_thread_64.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_64.SetTextColor(RGB(255,255,255));
// 
// 	m_data_thread_65.SetFont(mp_step_list_font);
// 	m_data_thread_65.SetCenterText();
// 	m_data_thread_65.SetColor(RGB(0,0,255));
// 	m_data_thread_65.SetGradientColor(RGB(0,0,0));
// 	m_data_thread_65.SetTextColor(RGB(255,255,255));

}

void CScreen_List_Step::OnList_Step_Data_Set()
{
	
	CString str_temp;		// 스텝 임시 저장 변수

	char chr_buf[20];


// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_Clip_Transfer.InitStep), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_Clip_Transfer.MoveStep ), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_Clip_Transfer.PrevStep ), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_Clip_Transfer.ReinstateStep ), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_Clip_Transfer.RunStep), chr_buf, 10));
//  	mstr_thread_1 = str_temp;

	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_LeftSink_Tray_Transfer.InitStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_LeftSink_Tray_Transfer.MoveStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_LeftSink_Tray_Transfer.ReinstateStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_LeftSink_Tray_Transfer.TrayBackStep), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_LeftSink_Tray_Transfer.TrayMoveStep), chr_buf, 10));	
//  str_temp = LPCTSTR(_itoa((int)(Run_Load_LeftSink_Tray_Transfer.RunStep, chr_buf, 10));
// 	mstr_thread_2 = str_temp;
	

// 	str_temp = LPCTSTR(_itoa((int)(Run_Load_RightSink_Tray_Transfer.RunStep), chr_buf, 10));
// 	mstr_thread_3 = str_temp;


// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.PrevStep), chr_buf, 10));
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.RunInsertStep), chr_buf, 10));
	mstr_thread_16 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.RunClampStep ), chr_buf, 10));
	mstr_thread_17 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.InitStep), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.RotatorStep), chr_buf, 10));
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.RunBufferStep), chr_buf, 10));
	mstr_thread_18 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.ClipBufferStep), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.ClipPickStep), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.ClipPlaceStep), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.ClampRbtStep), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.ReinstateStep), chr_buf, 10));
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.MoveStep), chr_buf, 10));
//	mstr_thread_4 = str_temp;



// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_LeftHeat.InitStep), chr_buf, 10));
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_LeftHeat.Module_Place_Step), chr_buf, 10));
	mstr_thread_28 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_LeftHeat.ReinstateStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_LeftHeat.SafetyZone_Step), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_LeftHeat.Tray_Pick_Step), chr_buf, 10));	
	mstr_thread_54 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_LeftHeat.MoveStep), chr_buf, 10));
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_LeftHeat.RunStep), chr_buf, 10));	
	mstr_thread_5 = str_temp;


// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.PrevStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.InitStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.SafetyZone_Step), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.ReinstateStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.TrayBackStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.TrayMoveStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.Tray_Pick_Step), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.Module_Place_Step), chr_buf, 10));	
//str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.MoveStep), chr_buf, 10));	
	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.RunStep), chr_buf, 10));	
	mstr_thread_6 = str_temp;

// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_RightHeat.InitStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_RightHeat.ReinstateStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_RightHeat.SafetyZone_Step), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_RightHeat.Tray_Pick_Step), chr_buf, 10));	
	mstr_thread_29 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_RightHeat.Module_Place_Step), chr_buf, 10));	
	mstr_thread_30 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_RightHeat.MoveStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Load_RightHeat.RunStep), chr_buf, 10));	
	mstr_thread_7 = str_temp;


// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.PrevStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.RunUldStep), chr_buf, 10));	
	mstr_thread_19 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.InitStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.SafetyZone_Step), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.UldSafetyZone_Step), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.ReinstateStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.Module_Pick_Step), chr_buf, 10));	
	mstr_thread_31 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.TrayPlace_Step), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.UldModule_Pick_Step), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.Module_Place_Step), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.MoveStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.RunSortStep), chr_buf, 10));	
	mstr_thread_8 = str_temp;
	// 
	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_VisionLabel.PrevStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_VisionLabel.Run_labelStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_VisionLabel.Run_Vision1), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_VisionLabel.Run_Vision2), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_VisionLabel.MoveStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_VisionLabel.InitStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_VisionLabel.ReinstateStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_VisionLabel.MoveStep), chr_buf, 10));	
//	mstr_thread_9 = str_temp;

	// 

// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.InitStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.ReinstateStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.RunBuff1Step), chr_buf, 10));	
	mstr_thread_32 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.RunBuff2Step), chr_buf, 10));	
	mstr_thread_33 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.RunBuff3Step), chr_buf, 10));	
	mstr_thread_34 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.RunBuff4Step), chr_buf, 10));	
	mstr_thread_35 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.MoveStep), chr_buf, 10));	
//	mstr_thread_10 = str_temp;

	
	// 
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.PrevStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.InitStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.PushStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.TransUpDnStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.ClipOutStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.FixStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.RotatorStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.ReinstateStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.RunInsertStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.RunStep), chr_buf, 10));	
	mstr_thread_36 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.MoveStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.RunTransStep), chr_buf, 10));	
	mstr_thread_11 = str_temp;


	// 
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.PrevStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.RunS1Step), chr_buf, 10));	
	mstr_thread_20 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.RunS2Step), chr_buf, 10));	
	mstr_thread_21 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.InitStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.ReinstateStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.TrayMoveStep), chr_buf, 10));	
	mstr_thread_37 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.TrayBackStep), chr_buf, 10));	
	mstr_thread_38 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.ReadyPosStep[0]), chr_buf, 10));	
	mstr_thread_39 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.ReadyPosStep[1]), chr_buf, 10));	
	mstr_thread_40 = str_temp;
	// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.MotorMoveStep[0]), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.MotorMoveStep[1]), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.MoveStep), chr_buf, 10));		
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.RunTransStep), chr_buf, 10));	
	mstr_thread_12 = str_temp;

	// 	

 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.RunS1Step), chr_buf, 10));	
	mstr_thread_22 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.RunS2Step), chr_buf, 10));	
	mstr_thread_23 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.InitStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.TrayBackStep), chr_buf, 10));	
 	mstr_thread_41 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.ReadyPosStep[0]), chr_buf, 10));	
	mstr_thread_42 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.ReadyPosStep[1]), chr_buf, 10));	
	mstr_thread_43 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.MotorMoveStep[0]), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.MotorMoveStep[1]), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.ReinstateStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.TrayMoveStep), chr_buf, 10));	
	mstr_thread_44 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.MoveStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.RunTransStep), chr_buf, 10));	
	mstr_thread_13 = str_temp;

		
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.PrevStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.RunS1Step), chr_buf, 10));	
	mstr_thread_24 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.RunS2Step), chr_buf, 10));	
	mstr_thread_25 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.InitStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.ReinstateStep), chr_buf, 10));	
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.TrayMoveStep), chr_buf, 10));	
	mstr_thread_45 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.TrayBackStep), chr_buf, 10));	
	mstr_thread_46 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.ReadyPosStep[0]), chr_buf, 10));	
	mstr_thread_47 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.ReadyPosStep[1]), chr_buf, 10));	
	mstr_thread_48 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.MotorMoveStep[0]), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.MotorMoveStep[1]), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.MoveStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.RunTransStep), chr_buf, 10));	
	mstr_thread_14 = str_temp;

	// 	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.RunStep), chr_buf, 10));	
	mstr_thread_49 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.InitStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.ReinstateStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.ReadyPosStep_old[0]), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.ReadyPosStep_old[1]), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.ReadyPosStep[0]), chr_buf, 10));	
	mstr_thread_50 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.ReadyPosStep[1]), chr_buf, 10));	
	mstr_thread_51 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.MotorMoveStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.WorkPosStep_old), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.WorkPosStep), chr_buf, 10));	
	mstr_thread_52 = str_temp;
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.RunS1Step), chr_buf, 10));	
	mstr_thread_26 = str_temp;
	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.RunS2Step), chr_buf, 10));	
	mstr_thread_27 = str_temp;
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.StackerReayStep), chr_buf, 10));	
// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.TrayMoveStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.TrayBackStep), chr_buf, 10));	
	mstr_thread_53 = str_temp;

	// 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.MoveStep), chr_buf, 10));	
 	str_temp = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.RunTransStep), chr_buf, 10));	
	mstr_thread_15 = str_temp;
	
	

}

void CScreen_List_Step::OnList_Step_Data_Display()
{
	m_data_thread_1.SetWindowText(_T(mstr_thread_1));
	m_data_thread_2.SetWindowText(_T(mstr_thread_2));
	m_data_thread_3.SetWindowText(_T(mstr_thread_3));
	m_data_thread_4.SetWindowText(_T(mstr_thread_4));
	m_data_thread_5.SetWindowText(_T(mstr_thread_5));
	m_data_thread_6.SetWindowText(_T(mstr_thread_6));
	m_data_thread_7.SetWindowText(_T(mstr_thread_7));
	m_data_thread_8.SetWindowText(_T(mstr_thread_8));
	m_data_thread_9.SetWindowText(_T(mstr_thread_9));
	m_data_thread_10.SetWindowText(_T(mstr_thread_10));
	m_data_thread_11.SetWindowText(_T(mstr_thread_11));
	m_data_thread_12.SetWindowText(_T(mstr_thread_12));
	m_data_thread_13.SetWindowText(_T(mstr_thread_13));
	m_data_thread_14.SetWindowText(_T(mstr_thread_14));
	m_data_thread_15.SetWindowText(_T(mstr_thread_15));
	m_data_thread_16.SetWindowText(_T(mstr_thread_16));
	m_data_thread_17.SetWindowText(_T(mstr_thread_17));
	m_data_thread_18.SetWindowText(_T(mstr_thread_18));
	m_data_thread_19.SetWindowText(_T(mstr_thread_19));
	m_data_thread_20.SetWindowText(_T(mstr_thread_20));
	m_data_thread_21.SetWindowText(_T(mstr_thread_21));
	m_data_thread_22.SetWindowText(_T(mstr_thread_22));
	m_data_thread_23.SetWindowText(_T(mstr_thread_23));
	m_data_thread_24.SetWindowText(_T(mstr_thread_24));
	m_data_thread_25.SetWindowText(_T(mstr_thread_25));
	m_data_thread_26.SetWindowText(_T(mstr_thread_26));
	m_data_thread_27.SetWindowText(_T(mstr_thread_27));
	m_data_thread_28.SetWindowText(_T(mstr_thread_28));
	m_data_thread_29.SetWindowText(_T(mstr_thread_29));
	m_data_thread_30.SetWindowText(_T(mstr_thread_30));
	m_data_thread_31.SetWindowText(_T(mstr_thread_31));
	m_data_thread_32.SetWindowText(_T(mstr_thread_32));
	m_data_thread_33.SetWindowText(_T(mstr_thread_33));
	m_data_thread_34.SetWindowText(_T(mstr_thread_34));
	m_data_thread_35.SetWindowText(_T(mstr_thread_35));
	m_data_thread_36.SetWindowText(_T(mstr_thread_36));
	m_data_thread_37.SetWindowText(_T(mstr_thread_37));
	m_data_thread_38.SetWindowText(_T(mstr_thread_38));
	m_data_thread_39.SetWindowText(_T(mstr_thread_39));
	m_data_thread_40.SetWindowText(_T(mstr_thread_40));
	m_data_thread_41.SetWindowText(_T(mstr_thread_41));
	m_data_thread_42.SetWindowText(_T(mstr_thread_42));
	m_data_thread_43.SetWindowText(_T(mstr_thread_43));
	m_data_thread_44.SetWindowText(_T(mstr_thread_44));
	m_data_thread_45.SetWindowText(_T(mstr_thread_45));
	m_data_thread_46.SetWindowText(_T(mstr_thread_46));
	m_data_thread_47.SetWindowText(_T(mstr_thread_47));
	m_data_thread_48.SetWindowText(_T(mstr_thread_48));
	m_data_thread_49.SetWindowText(_T(mstr_thread_49));
	m_data_thread_50.SetWindowText(_T(mstr_thread_50));
	m_data_thread_51.SetWindowText(_T(mstr_thread_51));
	m_data_thread_52.SetWindowText(_T(mstr_thread_52));
 	m_data_thread_53.SetWindowText(_T(mstr_thread_53));
 	m_data_thread_54.SetWindowText(_T(mstr_thread_54));
// 	m_data_thread_55.SetWindowText(_T(mstr_thread_55));
// 	m_data_thread_56.SetWindowText(_T(mstr_thread_56));
// 	m_data_thread_57.SetWindowText(_T(mstr_thread_57));
// 	m_data_thread_58.SetWindowText(_T(mstr_thread_58));
// 	m_data_thread_59.SetWindowText(_T(mstr_thread_59));
// 	m_data_thread_60.SetWindowText(_T(mstr_thread_60));
// 	m_data_thread_61.SetWindowText(_T(mstr_thread_61));
// 	m_data_thread_62.SetWindowText(_T(mstr_thread_62));
// 	m_data_thread_63.SetWindowText(_T(mstr_thread_63));
// 	m_data_thread_64.SetWindowText(_T(mstr_thread_64));
// 	m_data_thread_65.SetWindowText(_T(mstr_thread_65));
// 
}

void CScreen_List_Step::On_Step_List_File_Name_Init()
{
	CString str_folder_data;					// 쓰레드 스텝 저장 파일 생성할 폴더 저장 변수
	CString str_new_file;						// 생성할 파일명 저장 변수
	CString str_title_time;						// 파일에 출력할 타이틀 시간 저장 변수
	int n_cur_year, n_cur_month, n_cur_day;		// 년, 월, 일 저장 변수

	COleDateTime mtime_cur;						// 현재 시간 저장 변수
	CTime mp_time_cur;							// 타이틀 시간 저장 변수

	str_folder_data = (st_path.mstr_path_back_1) + "Step\\";  // 쓰레드 스텝 저장 파일 생성 폴더 설정

	/* ************************************************************************** */
	/* 파일 이름으로 사용할 날짜 설정한다                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	mp_time_cur = CTime::GetCurrentTime() ;

	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  

	str_new_file.Format("%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	str_title_time = mp_time_cur.Format("%Y/%m/%d %H:%M:%S"); 
	/* ************************************************************************** */

	On_Step_List_File_Compare(str_folder_data, str_new_file, str_title_time);  // 쓰레드 스텝 저장 파일 존재 유무 검사 및 파일 생성 함수
}

void CScreen_List_Step::On_Step_List_File_Compare(CString str_folder, CString str_file_name, CString str_time_info)
{
	CString str_step_file;		// 쓰레드 스텝 저장 파일명 설정 변수 [날짜로 구성]
	CString str_index_data;		// 파일 인덱스 저장 변수
	CString str_chk_file;		// 존재 유무 검사할 파일에 대한 [폴더]+[파일명] 저장 변수
	CString str_create_file;	// 생성할 파일 이름 저장 변수 [파일명] +[인덱스]+[확장자]
	CTime mp_time_cur;
	CString str_title_time;
	int n_index_data;			// 파일 인덱스 저장 변수
	int n_return;				// 함수 리턴 플래그

	/* ************************************************************************** */
	/* 쓰레드 스텝 저장 파일명 및 인덱스 로딩한다                                 */
	/* ************************************************************************** */
	str_step_file = mcls_file.OnStep_File_Name_Load();    // 쓰레드 스텝 저장 파일명 로딩 함수
	str_index_data = mcls_file.OnStep_File_Index_Load();  // 쓰레드 스텝 저장 파일 인덱스 로딩 함수
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 생성할 파일명과 마지막 생성 파일명 비교하여 인덱스 설정                    */
	/* -> str_step_file : 마지막 생성된 파일명                                    */
	/* -> str_file_name : 새로 생성할 파일명                                      */
	/* ************************************************************************** */
	if (str_step_file == str_file_name)  
	{
		n_index_data = atoi(str_index_data);
		n_index_data++;
		if (n_index_data > 99)  // 인덱스 최대 설정 범위 검사
			n_index_data = 1;
	}
	else  
		n_index_data = 1;		// 날짜 변경되었으므로 인덱스 초기화

	str_index_data.Format("%02d", n_index_data);  // 인덱스 2자리 문자로 변경

	mp_time_cur = CTime::GetCurrentTime() ;
	str_title_time = mp_time_cur.Format("%Y%m%d%H%M%S");
	str_index_data.Format("%s", str_title_time);  // 인덱스 2자리 문자로 변경
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 생성할 파일에 대한 폴더+파일명 설정한다                                    */
	/* ************************************************************************** */
	str_chk_file = str_folder; 

	str_create_file.Format("AMT830_");  
//	str_create_file += str_file_name;
	str_create_file += str_index_data;
	str_create_file += ".TXT";

	str_chk_file += str_create_file;
	/* ************************************************************************** */

	n_return = On_Step_List_File_Search(str_chk_file);								// 파일 존재 유무 검사 함수
	if (n_return == TRUE)  
		On_Step_List_File_Create(str_chk_file, str_create_file, str_time_info);		// 쓰레드 스텝 저장 파일 생성 함수
	else if (n_return == FALSE)
		On_Step_List_File_Create(str_chk_file, str_create_file, str_time_info);		// 쓰레드 스텝 저장 파일 생성 함수

	On_Step_List_File_Info_Save(str_file_name, str_index_data);						// 쓰레드 스텝 저장 파일 이름 저장 함수
}

void CScreen_List_Step::On_Step_List_File_Create(CString str_step_file, CString str_create_file, CString str_time)
{
	CString str_content;    // 한 라인에 출력할 문자열 저장 변수
	CString str_temp_data;  // 저장할 정보 임시 저장 변
	FILE  *fp ;				// 파일 포인터 변수
	int n_length;
	char chr_buf[20] = {0,};
	int n_response;				// 대화 상자 리턴 플래그
	CDialog_Select select_dlg;

	if ((fp=fopen(str_step_file,"w+"))==NULL)  // 파일 연다
	{
		AfxMessageBox("파일 열기 실패!..");
		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
		{
			AfxMessageBox("Failed to open file!..");
		}

		return ;
	}

	/* ************************************************************************** */
	/* 파일에 타이틀 정보 출력한다                                                */
	/* ************************************************************************** */
	str_content = "**************************************************";
	fprintf(fp,"%s\r\n", str_content) ;

	str_content = "| Thread Step Info File                          |";
	fprintf(fp,"%s\r\n", str_content) ;

	str_content = "| DIVISION  : AMT Coporation                     |";
	fprintf(fp,"%s\n", str_content) ;
	
	str_content = "| File Name : " + str_create_file;
    str_content += "   |\r\n";
	fprintf(fp,"%s", str_content) ;

	str_content = "| Save Time : " + str_time;
    str_content += "                |\r\n";
	fprintf(fp,"%s", str_content) ;

	str_content = "**************************************************";
	fprintf(fp,"%s\r\n", str_content) ;

	str_content = "|  Thread Part                      | Move Step  |";
	fprintf(fp,"%s\n", str_content );
	
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content) ;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 쓰레드 스텝 파일에 출력한다                                                */
	/* ************************************************************************** */
// 	str_temp_data = LPCTSTR(_itoa((int)(Run_Load_Clip_Transfer.MoveStep), chr_buf, 10));
// 	n_length = str_temp_data.GetLength();
// 	str_content = "|  Run_Load_Clip_Transfer MoveStep		| ";
// 	str_content += str_temp_data;
// 	str_content += FileTextLine(1, n_length);
// 	fprintf(fp,"%s\r\n", str_content);
// 	str_content = "+----------------------------------------+-------+";
// 	fprintf(fp,"%s\r\n",str_content);

// 	str_temp_data = LPCTSTR(_itoa((int)(Run_Load_LeftSink_Tray_Transfer.MoveStep), chr_buf, 10));
// 	n_length = str_temp_data.GetLength();
// 	str_content = "|  Run_Load_LeftSink_Tray_Transfer MoveStep| ";
// 	str_content += str_temp_data;
// 	str_content += FileTextLine(1, n_length);
// 	fprintf(fp,"%s\r\n", str_content);
// 	str_content = "+----------------------------------------+-------+";
// 	fprintf(fp,"%s\r\n",str_content);

// 	str_temp_data = LPCTSTR(_itoa((int)(Run_Load_RightSink_Tray_Transfer.RunStep), chr_buf, 10));
// 	n_length = str_temp_data.GetLength();
// 	str_content = "|  Run_Load_LeftSink_Tray_Transfer MoveStep| ";
// 	str_content += str_temp_data;
// 	str_content += FileTextLine(1, n_length);
// 	fprintf(fp,"%s\r\n", str_content);
// 	str_content = "+----------------------------------------+-------+";
// 	fprintf(fp,"%s\r\n",str_content);


	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.RunInsertStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Load_Clip.RunInsertStep		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.RunClampStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Load_Clip.RunClampStep		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Load_Clip.RunBufferStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Load_Clip.RunBufferStep		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.RunStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_Clip.RunStep		            | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_Clip.RunTransStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_Clip.RunTransStep		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Load_LeftHeat.Module_Place_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Load_LeftHeat.Module_Place_Step		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Load_LeftHeat.Tray_Pick_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Load_LeftHeat.Tray_Pick_Step		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Load_LeftHeat.RunStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Load_LeftHeat.RunStep		            | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Load_Module.RunStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Load_Module.RunStep		            | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Load_RightHeat.Tray_Pick_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Load_RightHeat.Tray_Pick_Step		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Load_RightHeat.Module_Place_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Load_RightHeat.Module_Place_Step		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Load_RightHeat.RunStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Load_RightHeat.RunStep		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.RunUldStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Sorter.RunUldStep		                | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);
	
	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.Module_Pick_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Sorter.Module_Pick_Step		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Sorter.RunSortStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Sorter.RunSortStep		            | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

//
	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.RunBuff1Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Work_Buffer.RunBuff1Step		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.RunBuff2Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Work_Buffer.RunBuff2Step		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.RunBuff3Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Work_Buffer.RunBuff3Step		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Rbt_Work_Buffer.RunBuff4Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Rbt_Work_Buffer.RunBuff4Step		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);
//
	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.RunS1Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_LeftHeat.RunS1Step		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.RunS2Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_LeftHeat.RunS2Step		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.TrayMoveStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_LeftHeat.TrayMoveStep		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.TrayBackStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_LeftHeat.TrayBackStep		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.ReadyPosStep[0]), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_LeftHeat.ReadyPosStep[0]		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.ReadyPosStep[1]), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_LeftHeat.ReadyPosStep[1]		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_LeftHeat.RunTransStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_LeftHeat.RunTransStep		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);



// 	
	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.RunS1Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_Module.RunS1Step		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.RunS2Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_Module.RunS2Step		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.TrayBackStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_Module.TrayBackStep		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.ReadyPosStep[0]), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_Module.ReadyPosStep[0]		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.ReadyPosStep[1]), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_Module.ReadyPosStep[1]		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.TrayMoveStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_Module.TrayMoveStep		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_Module.RunTransStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_Module.RunTransStep		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	
//
	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.RunS1Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_RightHeat.RunS1Step		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.RunS2Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_RightHeat.RunS2Step		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.TrayMoveStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_RightHeat.TrayMoveStep		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);
	

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.TrayBackStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_RightHeat.TrayBackStep		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.ReadyPosStep[0]), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_RightHeat.ReadyPosStep[0]	| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);
	
	
	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.ReadyPosStep[1]), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_RightHeat.ReadyPosStep[1]	| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);
	
	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Load_RightHeat.RunTransStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Load_RightHeat.RunTransStep		| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


//
	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.RunStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Sorter.RunStep		            | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.ReadyPosStep[0]), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Sorter.ReadyPosStep[0]		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.ReadyPosStep[1]), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Sorter.ReadyPosStep[1]		    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.WorkPosStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Sorter.WorkPosStep		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.RunS1Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Sorter.RunS1Step		            | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.RunS2Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Sorter.RunS2Step		            | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.TrayBackStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Sorter.TrayBackStep		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Stacker_Sorter.RunTransStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Stacker_Sorter.RunTransStep		        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	
	str_temp_data.Format("%s", st_work.mstr_history_step[0]);
	str_content = "|  st_work.mstr_history_step[0]          | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[1]);
	str_content = "|  st_work.mstr_history_step[1]          | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[3]);
	str_content = "|  st_work.mstr_history_step[3]          | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[4]);
	str_content = "|  st_work.mstr_history_step[4]          | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[5]);
	str_content = "|  st_work.mstr_history_step[5]          | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[6]);
	str_content = "|  st_work.mstr_history_step[6]          | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[7]);
	str_content = "|  st_work.mstr_history_step[7]          | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[8]);
	str_content = "|  st_work.mstr_history_step[8]          | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[9]);
	str_content = "|  st_work.mstr_history_step[9]          | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[10]);
	str_content = "|  st_work.mstr_history_step[10]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[11]);
	str_content = "|  st_work.mstr_history_step[11]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[12]);
	str_content = "|  st_work.mstr_history_step[12]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[13]);
	str_content = "|  st_work.mstr_history_step[13]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[14]);
	str_content = "|  st_work.mstr_history_step[14]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[15]);
	str_content = "|  st_work.mstr_history_step[15]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[16]);
	str_content = "|  st_work.mstr_history_step[16]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[17]);
	str_content = "|  st_work.mstr_history_step[17]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[18]);
	str_content = "|  st_work.mstr_history_step[18]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[19]);
	str_content = "|  st_work.mstr_history_step[19]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[20]);
	str_content = "|  st_work.mstr_history_step[20]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[21]);
	str_content = "|  st_work.mstr_history_step[21]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[22]);
	str_content = "|  st_work.mstr_history_step[22]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[23]);
	str_content = "|  st_work.mstr_history_step[23]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[24]);
	str_content = "|  st_work.mstr_history_step[24]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[25]);
	str_content = "|  st_work.mstr_history_step[25]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[26]);
	str_content = "|  st_work.mstr_history_step[26]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[27]);
	str_content = "|  st_work.mstr_history_step[27]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[28]);
	str_content = "|  st_work.mstr_history_step[28]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[29]);
	str_content = "|  st_work.mstr_history_step[29]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[30]);
	str_content = "|  st_work.mstr_history_step[30]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[31]);
	str_content = "|  st_work.mstr_history_step[31]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[32]);
	str_content = "|  st_work.mstr_history_step[32]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[33]);
	str_content = "|  st_work.mstr_history_step[33]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[34]);
	str_content = "|  st_work.mstr_history_step[34]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[35]);
	str_content = "|  st_work.mstr_history_step[35]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[36]);
	str_content = "|  st_work.mstr_history_step[36]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[37]);
	str_content = "|  st_work.mstr_history_step[37]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[38]);
	str_content = "|  st_work.mstr_history_step[38]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[39]);
	str_content = "|  st_work.mstr_history_step[39]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[40]);
	str_content = "|  st_work.mstr_history_step[40]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);



	str_temp_data.Format("%s", st_work.mstr_history_step[41]);
	str_content = "|  st_work.mstr_history_step[41]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[42]);
	str_content = "|  st_work.mstr_history_step[42]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[43]);
	str_content = "|  st_work.mstr_history_step[43]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[44]);
	str_content = "|  st_work.mstr_history_step[44]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[45]);
	str_content = "|  st_work.mstr_history_step[45]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[46]);
	str_content = "|  st_work.mstr_history_step[46]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[47]);
	str_content = "|  st_work.mstr_history_step[47]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[48]);
	str_content = "|  st_work.mstr_history_step[48]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[49]);
	str_content = "|  st_work.mstr_history_step[49]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[50]);
	str_content = "|  st_work.mstr_history_step[50]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);
	

	str_temp_data.Format("%s", st_work.mstr_history_step[51]);
	str_content = "|  st_work.mstr_history_step[51]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[52]);
	str_content = "|  st_work.mstr_history_step[52]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[53]);
	str_content = "|  st_work.mstr_history_step[53]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[54]);
	str_content = "|  st_work.mstr_history_step[54]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[55]);
	str_content = "|  st_work.mstr_history_step[55]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[56]);
	str_content = "|  st_work.mstr_history_step[56]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[57]);
	str_content = "|  st_work.mstr_history_step[57]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[58]);
	str_content = "|  st_work.mstr_history_step[58]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[59]);
	str_content = "|  st_work.mstr_history_step[59]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[60]);
	str_content = "|  st_work.mstr_history_step[60]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[61]);
	str_content = "|  st_work.mstr_history_step[61]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[62]);
	str_content = "|  st_work.mstr_history_step[62]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[63]);
	str_content = "|  st_work.mstr_history_step[63]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[64]);
	str_content = "|  st_work.mstr_history_step[64]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[65]);
	str_content = "|  st_work.mstr_history_step[65]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[66]);
	str_content = "|  st_work.mstr_history_step[66]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[67]);
	str_content = "|  st_work.mstr_history_step[67]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[68]);
	str_content = "|  st_work.mstr_history_step[68]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[69]);
	str_content = "|  st_work.mstr_history_step[69]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[70]);
	str_content = "|  st_work.mstr_history_step[70]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[71]);
	str_content = "|  st_work.mstr_history_step[71]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[72]);
	str_content = "|  st_work.mstr_history_step[72]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[73]);
	str_content = "|  st_work.mstr_history_step[73]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[74]);
	str_content = "|  st_work.mstr_history_step[74]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[75]);
	str_content = "|  st_work.mstr_history_step[75]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[76]);
	str_content = "|  st_work.mstr_history_step[76]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[77]);
	str_content = "|  st_work.mstr_history_step[77]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[78]);
	str_content = "|  st_work.mstr_history_step[78]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[79]);
	str_content = "|  st_work.mstr_history_step[79]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[80]);
	str_content = "|  st_work.mstr_history_step[80]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[81]);
	str_content = "|  st_work.mstr_history_step[81]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[82]);
	str_content = "|  st_work.mstr_history_step[82]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[83]);
	str_content = "|  st_work.mstr_history_step[83]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[84]);
	str_content = "|  st_work.mstr_history_step[84]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[85]);
	str_content = "|  st_work.mstr_history_step[85]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[86]);
	str_content = "|  st_work.mstr_history_step[86]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[87]);
	str_content = "|  st_work.mstr_history_step[87]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[88]);
	str_content = "|  st_work.mstr_history_step[88]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[89]);
	str_content = "|  st_work.mstr_history_step[89]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[90]);//st_sync.n_visionrbt_workend_flag[VISION_1_SITE]);
	str_content = "|  st_work.mstr_history_step[90]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[91]);//st_sync.n_ldrbt_visionrbt_req[0]);
	str_content = "|  st_work.mstr_history_step[91]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[92]);//st_sync.n_ldrbt_visionrbt_req[1]);
	str_content = "|  st_work.mstr_history_step[92]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[93]);//st_sync.n_visionrbt_workend_flag[VISION_2_SITE]);
	str_content = "|  st_work.mstr_history_step[93]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[94]);//st_sync.n_ldrbt_visionrbt_req[0]);
	str_content = "|  st_work.mstr_history_step[94]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[95]);//st_sync.n_ldrbt_visionrbt_req[1]);
	str_content = "|  st_work.mstr_history_step[95]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[96]);
	str_content = "|  st_work.mstr_history_step[96]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data.Format("%s", st_work.mstr_history_step[97]);
	str_content = "|  st_work.mstr_history_step[97]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[98]);
	str_content = "|  st_work.mstr_history_step[98]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);


	str_temp_data.Format("%s", st_work.mstr_history_step[99]);
	str_content = "|  st_work.mstr_history_step[99]         | ";
	str_content += str_temp_data;
	fprintf(fp,"%s\r\n",str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);
	
	if (ferror(fp)) 
	{
		AfxMessageBox("파일 쓰기 실패!..");
		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
		{
			AfxMessageBox("File write failed!..");
		}

		return ;
	}

	fclose(fp);  // 파일 종료
	st_msg.mstr_confirm_msg = "파일을 저장 하였습니다!!...";
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("March to save the file");
	}

	n_response = select_dlg.DoModal();

	if (n_response == IDOK)
	{
		
	}

}

int CScreen_List_Step::On_Step_List_File_Search(CString str_chk_file)
{
	int n_existence;		// 파일 존재 유무 플래그

	char fileName[256];

	sprintf(fileName, "%s", str_chk_file);
	n_existence = access(fileName,0) ;
	if (n_existence == -1)  // 해당 파일이 미존재
		return FALSE;
	
	DeleteFile(fileName);	// 존재하는 쓰레드 스텝 저장 파일 삭제

	return TRUE;
}

void CScreen_List_Step::On_Step_List_File_Info_Save(CString str_step_file, CString str_file_index)
{
	mcls_file.OnStep_File_Name_Save(str_step_file);		// 쓰레드 스텝 저장 파일명 파일에 저장하는 함수
	mcls_file.OnStep_File_Index_Save(str_file_index);	// 쓰레드 스텝 저장 파일 인덱스 파일에 저장하는 함수
}

CString CScreen_List_Step::FileTextLine(int dir, int length)
{
 	CString line;

	switch(dir)
	{
	case 0 :
		if (length == 1 || length == 0 )
		{
			line.Format("%14c", '|');
		}
		else if (length == 2)
		{
			line.Format("%13c", '|');
		}
		else if (length == 3)
		{
			line.Format("%12c", '|');
		}
		else if (length == 4)
		{
			line.Format("%11c", '|');
		}
		else if (length == 5)
		{
			line.Format("%10c", '|');
		}
		else if (length == 6)
		{
			line.Format("%9c", '|');
		}
		else if (length == 7)
		{
			line.Format("%8c", '|');
		}
		else if (length == 8)
		{
			line.Format("%7c", '|');
		}
		else
		{
			line.Format("%8c", '|');
		}
		break;
	case 1 :  
		if (length == 1 || length == 0 )
		{
			line.Format("%6c", '|');
		}
		else if (length == 2)
		{
			line.Format("%5c", '|');
		}
		else if (length == 3)
		{
			line.Format("%4c", '|');
		}
		else if (length == 4)
		{
			line.Format("%3c", '|');
		}
		else if (length == 5)
		{
			line.Format("%2c", '|');
		}
		else
		{
			line.Format("%1c", '|');
		}
		break;
	case 2 :
		if (length == 1 || length == 0 )
		{
			line.Format("%5c", '|');
		}
		else if (length == 2)
		{
			line.Format("%4c", '|');
		}
		else if (length == 3)
		{
			line.Format("%3c", '|');
		}
		else if (length == 4)
		{
			line.Format("%2c", '|');
		}
		else
		{
			line.Format("%1c", '|');
		}
		break;
	case 3 : 
		if (length == 1 || length == 0 )
		{
			//line.Format("%16c", '|');
			line.Format("%17c", '|');
		}
		else if (length == 2)
		{
			line.Format("%15c", '|');
		}
		else if (length == 3)
		{
			line.Format("%14c", '|');
		}
		else if (length == 4)
		{
			line.Format("%13c", '|');
		}
		else if (length == 5)
		{
			line.Format("%12c", '|');
		}
		else if (length == 6)
		{
			line.Format("%11c", '|');
		}
		else if (length == 7)
		{
			line.Format("%10c", '|');
		}
		else if (length == 8)
		{
			line.Format("%9c", '|');
		}
		else if (length == 9)
		{
			line.Format("%8c", '|');
		}
		else if (length == 10)
		{
			line.Format("%7c", '|');
		}
		else if (length == 11)
		{
			line.Format("%6c", '|');
		}
		else
		{
			line.Format("%5c", '|');
		}
		break;
	default :
		break;
	}

	return line;
}

void CScreen_List_Step::OnBtnStepSave() 
{
	int n_response;				// 대화 상자 리턴 플래그

	CDialog_Select select_dlg;

	
	st_msg.mstr_confirm_msg = "Step 정보를 저장 하시겠습니까?...";
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Do you want to save the Step Information?");
	}

	
	n_response = select_dlg.DoModal();

	if (n_response == IDOK)
		mn_timer_kill = TRUE;	// 타이머 삭제 요청 플래그 설정 
}

void CScreen_List_Step::TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp)
{
	CString line, str_temp_data_temp, str_content;
	
	line = "";
	
	fprintf(fp,"%s", str_sdata);
	
	switch(dir)
	{
	case 6 :
		str_temp_data_temp.Format("%6s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------+";
		break;
		
	case 14:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------+";
		break;
		
	case 140:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 30:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------------+";
		break;
		
	case 300:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
	}
	fprintf(fp,"%s\r\n",str_content);
}

BOOL CScreen_List_Step::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* 생성한 폰트 정보 삭제한다                                                  */
	/* ************************************************************************** */
	delete mp_step_list_font;
	mp_step_list_font = NULL;
	/* ************************************************************************** */

	KillTimer(TMR_STEP);			// 타이머 재 삭제
	return CFormView::DestroyWindow();
}
