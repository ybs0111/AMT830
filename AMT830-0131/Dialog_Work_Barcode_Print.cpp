// Dialog_Work_Barcode_Print.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Work_Barcode_Print.h"
#include "AMTLotManager.h"

#include "Dialog_Select.h"
#include "math.h"
#include "io.h"			// 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요
#include "KeyboardDll.h"	//20120514 유성준
#include "Dialog_Message.h"

#include "Ctlbd_Variable.h"
/////////////////////////////////
#include "Dialog_KeyPad.h"
#include "Dialog_Keyboard.h"
#include "Dialog_Work_Barcode_Info.h"	//20120625
#include "Dialog_Select.h"
#include "Variable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Barcode_Print dialog
#define TM_IN_READ		100
#define TM_OUT_READ		200
#define TM_BARCODE_PRINT 300 //20140214

CDialog_Work_Barcode_Print::CDialog_Work_Barcode_Print(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Work_Barcode_Print::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Work_Barcode_Print)
	//}}AFX_DATA_INIT
}


void CDialog_Work_Barcode_Print::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Work_Barcode_Print)
	DDX_Control(pDX, IDC_BTN_BARCODE_CREATE, m_btn_barcode_create);
	DDX_Control(pDX, IDC_EDIT_BARCODE_CREATE, m_ed_barcode_create);
	DDX_Control(pDX, IDC_GROUP_BARCODE_LABEL, m_group_barcode_label);
	DDX_Control(pDX, IDC_MSG_BARCODE_LABEL, m_msg_barcode_label);
	DDX_Control(pDX, IDC_BTN_BARCODE_PRINT, m_btn_barcode_print);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btn_apply);
	DDX_Control(pDX, IDC_GROUP_BARCODE_STATUS, m_group_barcode_status);
	DDX_Control(pDX, IDC_GROUP_BARCODE_MODE, m_group_barcode_mode);
	DDX_Control(pDX, IDC_CHK_BARCODE_MODE, m_chk_barcode_mode);

	//////////////////////////////////////////////////////////////20120604
	DDX_Control(pDX, IDC_GROUP_LABEL_INFO, m_group_label_info);
	DDX_Control(pDX, IDC_LIST_LABEL_TYPE1, m_list_label_type1);
	DDX_Control(pDX, IDC_MSG_LABELSET_SD1, m_msg_labelset_sd1);
	DDX_Control(pDX, IDC_MSG_LABELSET_LT1, m_msg_labelset_lt1);
	DDX_Control(pDX, IDC_MSG_LABELSET_LH1, m_msg_labelset_lh1);
	DDX_Control(pDX, IDC_MSG_LABELSET_LH2, m_msg_labelset_lh2);
	DDX_Control(pDX, IDC_MSG_LABELSET_X1_POS1, m_msg_labelset_x1_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_X2_POS1, m_msg_labelset_x2_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_X3_POS1, m_msg_labelset_x3_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_X4_POS1, m_msg_labelset_x4_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_X5_POS1, m_msg_labelset_x5_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_X6_POS1, m_msg_labelset_x6_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y1_POS1, m_msg_labelset_y1_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y2_POS1, m_msg_labelset_y2_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y3_POS1, m_msg_labelset_y3_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y4_POS1, m_msg_labelset_y4_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y5_POS1, m_msg_labelset_y5_pos1);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y6_POS1, m_msg_labelset_y6_pos1);
	/////////////////////////////////////////////////////// 20120601
	DDX_Control(pDX, IDC_GROUP_LABELSET_POS2, m_group_labelset_pos2);
	DDX_Control(pDX, IDC_GROUP_LABELSET_SIZE2, m_group_labelset_size2);
	DDX_Control(pDX, IDC_GROUP_LABELSET_POS3, m_group_labelset_pos3);


	DDX_Control(pDX, IDC_MSG_LABELSET_X1_WIDTH1, m_msg_labelset_x1_width1);
	DDX_Control(pDX, IDC_MSG_LABELSET_X2_WIDTH1, m_msg_labelset_x2_width1);
	DDX_Control(pDX, IDC_MSG_LABELSET_X3_WIDTH1, m_msg_labelset_x3_width1);
	DDX_Control(pDX, IDC_MSG_LABELSET_X4_WIDTH1, m_msg_labelset_x4_width1);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y1_HEIGHT1, m_msg_labelset_y1_height1);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y2_HEIGHT1, m_msg_labelset_y2_height1);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y3_HEIGHT1, m_msg_labelset_y3_height1);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y4_HEIGHT1, m_msg_labelset_y4_height1);
	
	DDX_Control(pDX, IDC_STATIC_LABEL_VIEW, m_static_label_view);

	DDX_Control(pDX, IDC_STATIC_LABEL_VIEW2, m_static_label_view2);
	DDX_Control(pDX, IDC_STATIC_LABEL_VIEW3, m_static_label_view3);
	DDX_Control(pDX, IDC_STATIC_LABEL_VIEW4, m_static_label_view4);

	/////////////////////////////////////////////////////// 20120605
	DDX_Control(pDX, IDC_MSG_NUMBER, m_msg_number);
	DDX_Control(pDX, IDC_MSG_NUMBER2, m_msg_number2);
	DDX_Control(pDX, IDC_MSG_NUMBER3, m_msg_number3);
	DDX_Control(pDX, IDC_MSG_NUMBER4, m_msg_number4);
	DDX_Control(pDX, IDC_MSG_NUMBER5, m_msg_number5);
	DDX_Control(pDX, IDC_MSG_NUMBER6, m_msg_number6);
	DDX_Control(pDX, IDC_MSG_NUMBER7, m_msg_number7);
	DDX_Control(pDX, IDC_MSG_NUMBER8, m_msg_number8);
	DDX_Control(pDX, IDC_MSG_NUMBER9, m_msg_number9);
	DDX_Control(pDX, IDC_MSG_NUMBER10, m_msg_number10);


	DDX_Control(pDX, IDC_MSG_PREVIEW, m_msg_preview);
	DDX_Control(pDX, IDC_MSG_PREVIEW2, m_msg_preview2);
	DDX_Control(pDX, IDC_MSG_PREVIEW3, m_msg_preview3);
	DDX_Control(pDX, IDC_MSG_PREVIEW4, m_msg_preview4);

	DDX_Control(pDX, IDC_GROUP_LABEL_TEST, m_group_labeltest);
	

	DDX_Control(pDX, IDC_EDIT_LABELSET_DATA, m_edit_labelset_data);
	DDX_Control(pDX, IDC_EDIT_LABELSET_DATA2, m_edit_labelset_data2);
	DDX_Control(pDX, IDC_EDIT_LABELSET_DATA3, m_edit_labelset_data3);
	DDX_Control(pDX, IDC_EDIT_LABELSET_DATA4, m_edit_labelset_data4);

	/////////////////////////////////////////////////////// 20120625
	DDX_Control(pDX, IDC_GROUP_LABELSET_DATA, m_group_labelset_data);
	DDX_Control(pDX, IDC_EDIT_BARCODE_CREATE2, m_ed_barcode_create2);
	DDX_Control(pDX, IDC_EDIT_BARCODE_CREATE3, m_ed_barcode_create3);
	DDX_Control(pDX, IDC_GROUP_LABELSET_FUNC, m_group_labelset_func);
	DDX_Control(pDX, IDC_GROUP_LABELSET_FUNC2, m_group_labelset_func2);
	DDX_Control(pDX, IDC_GROUP_LABELSET_FUNC3, m_group_labelset_func3); // 20120622
	DDX_Control(pDX, IDC_GROUP_LABELSET_FUNC4, m_group_labelset_func4); // 20120622
	DDX_Control(pDX, IDC_GROUP_LABELSET_FUNC5, m_group_labelset_func5); // 20120622

	DDX_Control(pDX, IDC_RADIO_PRT_COMP_REPLY_USE, m_radio_prt_comp_reply_use); // 20120622
	DDX_Control(pDX, IDC_RADIO_PRT_COMP_REPLY_NOTUSE, m_radio_prt_comp_reply_notuse); // 20120622

	DDX_Control(pDX, IDC_RADIO_PRT_MODE, m_radio_prt_tear_off_mode); // 20120622
	DDX_Control(pDX, IDC_RADIO_PRT_MODE2, m_radio_prt_peal_off_mode); // 20120622
	DDX_Control(pDX, IDC_RADIO_PRT_MODE3, m_radio_prt_rewind_mode); // 20120622

//////////20120906
	DDX_Control(pDX, IDC_DGT_LABELSET_X1_POS1, m_dgt_labelset_x1_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_X2_POS1, m_dgt_labelset_x2_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_X3_POS1, m_dgt_labelset_x3_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_X4_POS1, m_dgt_labelset_x4_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_X5_POS1, m_dgt_labelset_x5_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_X6_POS1, m_dgt_labelset_x6_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y1_POS1, m_dgt_labelset_y1_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y2_POS1, m_dgt_labelset_y2_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y3_POS1, m_dgt_labelset_y3_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y4_POS1, m_dgt_labelset_y4_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y5_POS1, m_dgt_labelset_y5_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y6_POS1, m_dgt_labelset_y6_pos1);
	DDX_Control(pDX, IDC_DGT_LABELSET_X1_WIDTH1, m_dgt_labelset_x1_width1);
	DDX_Control(pDX, IDC_DGT_LABELSET_X2_WIDTH1, m_dgt_labelset_x2_width1);
	DDX_Control(pDX, IDC_DGT_LABELSET_X3_WIDTH1, m_dgt_labelset_x3_width1);
	DDX_Control(pDX, IDC_DGT_LABELSET_X4_WIDTH1, m_dgt_labelset_x4_width1);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y1_HEIGHT1, m_dgt_labelset_y1_height1);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y2_HEIGHT1, m_dgt_labelset_y2_height1);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y3_HEIGHT1, m_dgt_labelset_y3_height1);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y4_HEIGHT1, m_dgt_labelset_y4_height1);
///////////


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Work_Barcode_Print, CDialog)
	//{{AFX_MSG_MAP(CDialog_Work_Barcode_Print)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHK_BARCODE_MODE, OnChkBarcodeMode)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_BARCODE_PRINT, OnBtnBarcodePrint)
	ON_BN_CLICKED(IDC_BTN_BARCODE_CREATE, OnBtnBarcodeCreate)
	ON_BN_CLICKED(IDC_MSG_BARCODE_LABEL, OnMsgBarcodeLabel)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_LBN_SELCHANGE(IDC_LIST_LABEL_TYPE1, OnSelchangeListLabelType1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_SD1, OnDgtLabelsetSd1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LT1, OnDgtLabelsetLt1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X1_POS1, OnDgtLabelsetX1Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X2_POS1, OnDgtLabelsetX2Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X3_POS1, OnDgtLabelsetX3Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X4_POS1, OnDgtLabelsetX4Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X5_POS1, OnDgtLabelsetX5Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X6_POS1, OnDgtLabelsetX6Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y1_POS1, OnDgtLabelsetY1Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y2_POS1, OnDgtLabelsetY2Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y3_POS1, OnDgtLabelsetY3Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y4_POS1, OnDgtLabelsetY4Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y5_POS1, OnDgtLabelsetY5Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y6_POS1, OnDgtLabelsetY6Pos1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X1_WIDTH1, OnDgtLabelsetX1Width1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y1_HEIGHT1, OnDgtLabelsetY1Height1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X2_WIDTH1, OnDgtLabelsetX2Width1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y2_HEIGHT1, OnDgtLabelsetY2Height1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X3_WIDTH1, OnDgtLabelsetX3Width1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y3_HEIGHT1, OnDgtLabelsetY3Height1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X4_WIDTH1, OnDgtLabelsetX4Width1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y4_HEIGHT1, OnDgtLabelsetY4Height1)
	ON_BN_CLICKED(IDC_BTN_LABELSET_MOVE, OnBtnLabelsetMove)
	ON_BN_CLICKED(IDC_BTN_LABELSET_MOVE2, OnBtnLabelsetMove2)
	ON_BN_CLICKED(IDC_BTN_LABELSET_MOVE3, OnBtnLabelsetMove3)
	ON_BN_CLICKED(IDC_BTN_LABELSET_MOVE4, OnBtnLabelsetMove4)
	ON_BN_CLICKED(IDC_BTN_LABELSET_MOVE5, OnBtnLabelsetMove5)
	ON_BN_CLICKED(IDC_BTN_LABELSET_MOVE6, OnBtnLabelsetMove6)
	ON_BN_CLICKED(IDC_BTN_LABEL_APPLY, OnBtnLabelApply)
	ON_BN_CLICKED(IDC_BTN_LABEL_CANCLE, OnBtnLabelCancle)
	ON_BN_CLICKED(IDC_BTN_LABELSET_SIZE, OnBtnLabelsetSize)
	ON_BN_CLICKED(IDC_BTN_LABELSET_SIZE2, OnBtnLabelsetSize2)
	ON_BN_CLICKED(IDC_BTN_LABELSET_SIZE3, OnBtnLabelsetSize3)
	ON_BN_CLICKED(IDC_BTN_LABELSET_SIZE4, OnBtnLabelsetSize4)
	ON_BN_CLICKED(IDC_BTN_LABELSET_DATA_INPUT, OnBtnLabelsetDataInput)
	ON_BN_CLICKED(IDC_BTN_LABELSET_DATA_INPUT2, OnBtnLabelsetDataInput2)
	ON_BN_CLICKED(IDC_BTN_LABELSET_DATA_INPUT3, OnBtnLabelsetDataInput3)
	ON_BN_CLICKED(IDC_BTN_LABELSET_DATA_INPUT4, OnBtnLabelsetDataInput4)
	ON_BN_CLICKED(IDC_BTN_BARCODE_CREATE2, OnBtnBarcodeCreate2)	//20120620
	ON_BN_CLICKED(IDC_BTN_BCR, OnBtnBcr)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BTN_BARCODE_FUNC3, OnBtnBarcodeFunc3)
	ON_BN_CLICKED(IDC_BTN_BARCODE_FUNC, OnBtnBarcodeFunc)
	ON_BN_CLICKED(IDC_BTN_BARCODE_FUNC2, OnBtnBarcodeFunc2)
	ON_BN_CLICKED(IDC_BTN_BARCODE_FUNC4, OnBtnBarcodeFunc4)
	ON_BN_CLICKED(IDC_BTN_BARCODE_FUNC5, OnBtnBarcodeFunc5)
	ON_BN_CLICKED(IDC_DGT_LABELSET_FUNC_PAPER, OnDgtLabelsetFuncPaper)
	ON_BN_CLICKED(IDC_BTN_BARCODE_CREATE3, OnBtnBarcodeCreate3)
	ON_BN_CLICKED(IDC_RADIO_PRT_COMP_REPLY_USE, OnRadioPrtCompReplyUse)
	ON_BN_CLICKED(IDC_RADIO_PRT_COMP_REPLY_NOTUSE, OnRadioPrtCompReplyNotuse)
	ON_BN_CLICKED(IDC_RADIO_PRT_MODE, OnRadioPrtMode)
	ON_BN_CLICKED(IDC_RADIO_PRT_MODE2, OnRadioPrtMode2)
	ON_BN_CLICKED(IDC_RADIO_PRT_MODE3, OnRadioPrtMode3)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LH1, OnDgtLabelsetLh1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LH2, OnDgtLabelsetLh2)
	ON_BN_CLICKED(IDC_BTN_BARCODE_PRINT_RESET, OnBtnBarcodePrintReset)
	ON_BN_CLICKED(IDC_BTN_PRINT_CALIBRATE, OnBtnPrintCalibrate)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCell_Left_Click)
 	ON_MESSAGE(WM_WORK_END, OnWork_Barcode_Info_Display)  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 //20120625
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Barcode_Print message handlers

BOOL CDialog_Work_Barcode_Print::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initializ ation here
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	mp_label_font = NULL;
	mp_label_font = new CFont;
	mp_label_font->CreateFont(18,6,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	mp_label_print_font = NULL;
	mp_label_print_font = new CFont;
	mp_label_print_font->CreateFont(10,4,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	mp_label_print_font1 = NULL;
	mp_label_print_font1 = new CFont;
	mp_label_print_font1->CreateFont(18,3,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");


	Data_Init();

	Init_Group();
	Init_Button();
	Init_Label();
	Init_Data();//20120828
	Init_Grid_Barcode();


 	OnBarcode_Label_Data_Set();				// 전역 변수의 Data를 받아온다.
 	OnBarcode_Label_Data_BackUp();
	OnBarcode_Label_Font_Set();

	//////////////////////////////// 20120530
	OnBarcode_Digital_Count_Set();
	OnBarcode_Label_Init_List(st_path.mstr_label_folder);
	OnBarcode_Label_Focus_Set();
	OnBarcode_Label_Data_Display();
	m_static_label_view.ShowWindow(TRUE);
	m_static_label_view2.ShowWindow(TRUE);
	m_static_label_view3.ShowWindow(TRUE);
	m_static_label_view4.ShowWindow(TRUE);
	//////////////////////////////// 

//	if (st_picker.n_vision_eable[0] == TRUE)
//	{
		if (st_barcode.n_Prt_Comp_Reply == 0)
		{
			OnRadioPrtCompReplyUse();
		}
		else
		{
			OnRadioPrtCompReplyNotuse();
		}


		if (st_barcode.n_Prt_mode == 0)
		{
			OnRadioPrtMode();
		}
		else if (st_barcode.n_Prt_mode == 1)
		{
			OnRadioPrtMode2();
		}
		else if (st_barcode.n_Prt_mode == 2)
		{
			OnRadioPrtMode3();
		}
//	}

	////2014,0411
	if(st_work.b_Barcode_Create_Btn_Use == TRUE)
	{
		OnBARCODE_CREATE_BUTTON(1);
	}
	else
	{
		OnBARCODE_CREATE_BUTTON(0);
	}
	////

	st_barcode.n_barcode_hs_cnt = 0;	//20120620

	SetTimer(TM_IN_READ, 100, NULL);
	SetTimer(TM_OUT_READ, 100, NULL);

	

	st_handler.cwnd_barcode_print = this;	//20120620

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialog_Work_Barcode_Print::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}
	
	if(Data_Comparison() == CTLBD_RET_ERROR)
	{
		Data_HistoryLog();
		Data_Apply();
	}

	if(mp_label_font != NULL)
	{
		/////////////////////////// 20120601
		delete mp_label_font;
		mp_label_font = NULL;
		///////////////////////////
	}

	if(mp_label_print_font != NULL)
	{
		/////////////////////////// 20120601
		delete mp_label_print_font;
		mp_label_print_font = NULL;
		///////////////////////////
	}

	if(mp_label_print_font1 != NULL)
	{
		/////////////////////////// 20120601
		delete mp_label_print_font1;
		mp_label_print_font1 = NULL;
		///////////////////////////
	}
	
	st_barcode.b_Maunal_Input = FALSE;//20120913

	KillTimer(TM_IN_READ);
	KillTimer(TM_OUT_READ);
	KillTimer(TM_BARCODE_PRINT);//20140214

	mcls_barcode.OnBarcode_Label_Data_Load();  //20120605

	st_handler.cwnd_barcode_print = NULL;	//20120620

}

void CDialog_Work_Barcode_Print::Data_Apply()
{
//	st_basic.n_mode_barcode_print	=	m_n_mode_barcode_print[1];
	st_barcode.n_Prt_Comp_Reply = mn_Prt_Comp_Reply[1];
	st_barcode.n_Prt_mode =	mn_Prt_Mode[1];

}

void CDialog_Work_Barcode_Print::Data_Backup()
{
//	m_n_mode_barcode_print[0]	=	m_n_mode_barcode_print[1];
	mn_Prt_Comp_Reply[0] = mn_Prt_Comp_Reply[1];
	mn_Prt_Mode[0]       = mn_Prt_Mode[1]  ;

}

int CDialog_Work_Barcode_Print::Data_Comparison()
{
	int Ret = CTLBD_RET_GOOD;
	
	if(m_n_mode_barcode_print[1]	!=	m_n_mode_barcode_print[0])		Ret = CTLBD_RET_ERROR;
	
	return Ret;
}

void CDialog_Work_Barcode_Print::Data_HistoryLog()
{
	CString str_msg;

	if(m_n_mode_barcode_print[1]	!=	m_n_mode_barcode_print[0])
	{
		str_msg.Format( "[Interface] Barcode Print Mode Change %d -> %d",m_n_mode_barcode_print[0], m_n_mode_barcode_print[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
}

void CDialog_Work_Barcode_Print::Data_Recovery()
{
//	m_n_mode_barcode_print[1]	= m_n_mode_barcode_print[0];
	mn_Prt_Comp_Reply[1] = mn_Prt_Comp_Reply[0];
	mn_Prt_Mode[1]       = mn_Prt_Mode[0]  ;

	if (st_barcode.n_Prt_Comp_Reply == 0)
	{
		OnRadioPrtCompReplyUse();
	}
	else
	{
		OnRadioPrtCompReplyNotuse();
	}


	if (st_barcode.n_Prt_mode == 0)
	{
		OnRadioPrtMode();
	}
	else if (st_barcode.n_Prt_mode == 1)
	{
		OnRadioPrtMode2();
	}
	else if (st_barcode.n_Prt_mode == 2)
	{
		OnRadioPrtMode3();
	}
}

void CDialog_Work_Barcode_Print::Data_Init()
{
//	m_n_mode_barcode_print[1]	= st_basic.n_mode_barcode_print;
	
	Data_Backup();
}

void CDialog_Work_Barcode_Print::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_barcode_mode.SetFont(main_font);
	m_group_barcode_mode.SetCatptionTextColor(BLUE_C);
	m_group_barcode_mode.SetBorderColor(BLUE_C);
	m_group_barcode_mode.SetFontBold(TRUE);
	m_group_barcode_mode.SetWindowText("Barcode Print");

 	m_group_barcode_status.SetFont(main_font);
	m_group_barcode_status.SetCatptionTextColor(BLUE_C);
	m_group_barcode_status.SetBorderColor(BLUE_C);
	m_group_barcode_status.SetFontBold(TRUE);
	m_group_barcode_status.SetWindowText("Barcode Print Status");

	m_group_barcode_label.SetFont(main_font);
	m_group_barcode_label.SetCatptionTextColor(BLUE_C);
	m_group_barcode_label.SetBorderColor(BLUE_C);
	m_group_barcode_label.SetFontBold(TRUE);
	m_group_barcode_label.SetWindowText("Barcode Label Info");

	////////////////////////////////////////////////////// 20120604
	m_group_label_info.SetFont(main_font);
	m_group_label_info.SetCatptionTextColor(BLUE_C);
	m_group_label_info.SetBorderColor(BLUE_C);
	m_group_label_info.SetFontBold(TRUE);
	m_group_label_info.SetWindowText("Label Info");

	////////////////////////////////////////////////////// 20120625
	m_group_labelset_pos2.SetFont(main_font);
	m_group_labelset_pos2.SetCatptionTextColor(BLUE_C);
	m_group_labelset_pos2.SetBorderColor(BLUE_C);
	m_group_labelset_pos2.SetFontBold(TRUE);
	m_group_labelset_pos2.SetWindowText("위치");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_group_labelset_pos2.SetWindowText("Position");
	}

	m_group_labelset_pos3.SetFont(main_font);
	m_group_labelset_pos3.SetCatptionTextColor(BLUE_C);
	m_group_labelset_pos3.SetBorderColor(BLUE_C);
	m_group_labelset_pos3.SetFontBold(TRUE);
	m_group_labelset_pos3.SetWindowText("라벨 인쇄 시작 위치");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_group_labelset_pos3.SetWindowText("Label printing start position");
	}
	m_group_labelset_data.SetFont(main_font);
	m_group_labelset_data.SetCatptionTextColor(BLUE_C);
	m_group_labelset_data.SetBorderColor(BLUE_C);
	m_group_labelset_data.SetFontBold(TRUE);
	m_group_labelset_data.SetWindowText("빈 용지 출력");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_group_labelset_data.SetWindowText("Sheet output bin");
	}


	m_group_labelset_func.SetFont(main_font);
	m_group_labelset_func.SetCatptionTextColor(BLUE_C);
	m_group_labelset_func.SetBorderColor(BLUE_C);
	m_group_labelset_func.SetFontBold(TRUE);
	m_group_labelset_func.SetWindowText("프린트 기능");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_group_labelset_func.SetWindowText("Print Function");
	}


	m_group_labelset_func2.SetFont(main_font);
	m_group_labelset_func2.SetCatptionTextColor(BLUE_C);
	m_group_labelset_func2.SetBorderColor(BLUE_C);
	m_group_labelset_func2.SetFontBold(TRUE);

	m_group_labelset_func3.SetFont(main_font);
	m_group_labelset_func3.SetCatptionTextColor(BLUE_C);
	m_group_labelset_func3.SetBorderColor(BLUE_C);
	m_group_labelset_func3.SetFontBold(TRUE);
	m_group_labelset_func3.SetWindowText("프린트 모드");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_group_labelset_func3.SetWindowText("Print Mode");
	}

	m_group_labelset_func4.SetFont(main_font);
	m_group_labelset_func4.SetCatptionTextColor(BLUE_C);
	m_group_labelset_func4.SetBorderColor(BLUE_C);
	m_group_labelset_func4.SetFontBold(TRUE);
	m_group_labelset_func4.SetWindowText("프린터 버튼");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_group_labelset_func4.SetWindowText("Printer Button");
	}

	m_group_labelset_func5.SetFont(main_font);
	m_group_labelset_func5.SetCatptionTextColor(BLUE_C);
	m_group_labelset_func5.SetBorderColor(BLUE_C);
	m_group_labelset_func5.SetFontBold(TRUE);
	m_group_labelset_func5.SetWindowText("Complete Reply");


	m_group_labelset_size2.SetFont(main_font);
	m_group_labelset_size2.SetCatptionTextColor(BLUE_C);
	m_group_labelset_size2.SetBorderColor(BLUE_C);
	m_group_labelset_size2.SetFontBold(TRUE);
	m_group_labelset_size2.SetWindowText("폰트 사이즈");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_group_labelset_size2.SetWindowText("Font Size");
	}

	//////////////////////////////////////////////////////



}

void CDialog_Work_Barcode_Print::Init_Button()
{
	short	shBtnColor = 30;
	
	m_chk_barcode_mode.SetFont(m_p_font);
	m_chk_barcode_mode.SetIcon(IDI_ENABLE, IDI_DISABLE);
	m_chk_barcode_mode.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_barcode_mode.SetButtonColor(1);
	m_chk_barcode_mode.SetCheck(m_n_mode_barcode_print[1]);
// 	if(m_chk_barcode_mode.GetCheck())
// 	{
// 		SetDlgItemText(IDC_CHK_BUZZER, "LPT Print");
// 	}
// 	else
// 	{
// 		SetDlgItemText(IDC_CHK_BUZZER, "RS-232 Print");
// 	}

	m_btn_apply.SetFont(m_p_font);
	m_btn_apply.SetIcon(IDI_MAINTENANCE_1);
	m_btn_apply.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_apply.SetButtonColor(1);
	m_btn_apply.SetCheck(TRUE);

	m_btn_barcode_print.SetFont(m_p_font);
	m_btn_barcode_print.SetIcon(IDI_PRINT);
	m_btn_barcode_print.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_barcode_print.SetButtonColor(1);
	m_btn_barcode_print.SetTooltipText("Barcode Print.");

	m_btn_barcode_create.SetFont(m_p_font);
	m_btn_barcode_create.SetIcon(IDI_BARCODE_LABEL);
	m_btn_barcode_create.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_barcode_create.SetButtonColor(1);
	m_btn_barcode_create.SetTooltipText("Barcode Label.");

    m_radio_prt_comp_reply_use.SetImage(IDB_REDBUTTON, 15);
    m_radio_prt_comp_reply_notuse.SetImage(IDB_REDBUTTON, 15);

    m_radio_prt_tear_off_mode.SetImage(IDB_REDBUTTON, 15);
    m_radio_prt_peal_off_mode.SetImage(IDB_REDBUTTON, 15);
    m_radio_prt_rewind_mode.SetImage(IDB_REDBUTTON, 15);


}

void CDialog_Work_Barcode_Print::OnChkBarcodeMode() 
{
	// TODO: Add your control notification handler code here
	if(m_chk_barcode_mode.GetCheck())
	{
		m_n_mode_barcode_print[1] = 1;
		SetDlgItemText(IDC_CHK_BARCODE_MODE, "LPT Print");
	}
	else
	{
		m_n_mode_barcode_print[1] = 0;
		SetDlgItemText(IDC_CHK_BARCODE_MODE, "RS-232 Print");
	}
}

void CDialog_Work_Barcode_Print::Init_Grid_Barcode()
{
	int   i, j;
	int   row;
	CString str_tmp;
	
	m_grid_io = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);
	
	m_p_grid.GridReset(m_grid_io);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_io, TRUE);
	
	m_p_grid.GridRowHeader(m_grid_io, FALSE);
	m_p_grid.GridColHeader(m_grid_io, TRUE);
	m_p_grid.GridHorizontal(m_grid_io, FALSE);
	m_p_grid.GridVertical(m_grid_io, FALSE);
	m_p_grid.GridAutoSize(m_grid_io, FALSE);
	m_p_grid.GridAutoSize(m_grid_io, FALSE);
	m_p_grid.GridCellRows(m_grid_io, 7);
	m_p_grid.GridCellCols(m_grid_io, 2);
	
	for(i=0; i<8; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_io, i, 25);
		for(j=0; j<2; j++)
		{
			if(j==0)
			{
				m_p_grid.GridCellWidth_L(m_grid_io, j+1, 10);
			}
			else
			{
				m_p_grid.GridCellWidth_L(m_grid_io, j+1, 19);
			}
			
			m_p_grid.GridCellControlStatic(m_grid_io, i, j+1);
		}
	}
	
	row = 0;
	m_p_grid.GridCellFont(m_grid_io, 0, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_io, 0, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_io, 0, 1, "IO");
	
	m_p_grid.GridCellFont(m_grid_io, 0, 2, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_io, 0, 2, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_io, 0, 2, "Description");
	

	row = 1;
	m_p_grid.GridCellFont(m_grid_io, row, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_io, row, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_io, row, 1, "OUTPUT");

	m_p_grid.GridCellFont(m_grid_io, row, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_io, row, 2, 
						   RGB(st_work.n_grid_r[1][1], st_work.n_grid_g[1][1], st_work.n_grid_b[1][1]), 
						   RGB(st_work.n_text_r[1][1], st_work.n_text_g[1][1], st_work.n_text_b[1][1]));
	m_p_grid.GridCellText(m_grid_io, row, 2, "Label Feed");


	row = 2;
	m_p_grid.GridCellFont(m_grid_io, row, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_io, row, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_io, row, 1, "INPUT");
	
	m_p_grid.GridCellFont(m_grid_io, row, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_io, row, 2, 
						   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
						   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	m_p_grid.GridCellText(m_grid_io, row, 2, "Ribbon Low");

	row = 3;
	m_p_grid.GridCellFont(m_grid_io, row, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_io, row, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_io, row, 1, "INPUT");
	
	m_p_grid.GridCellFont(m_grid_io, row, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_io, row, 2, 
						   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
						   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	m_p_grid.GridCellText(m_grid_io, row, 2, "Service Required");

	row = 4;
	m_p_grid.GridCellFont(m_grid_io, row, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_io, row, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_io, row, 1, "INPUT");
	
	m_p_grid.GridCellFont(m_grid_io, row, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_io, row, 2, 
						   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
						   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	m_p_grid.GridCellText(m_grid_io, row, 2, "End print");

	row = 5;
	m_p_grid.GridCellFont(m_grid_io, row, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_io, row, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_io, row, 1, "INPUT");
	
	m_p_grid.GridCellFont(m_grid_io, row, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_io, row, 2, 
						   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
						   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	m_p_grid.GridCellText(m_grid_io, row, 2, "Media Output");

	row = 6;
	m_p_grid.GridCellFont(m_grid_io, row, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_io, row, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_io, row, 1, "INPUT");
	
	m_p_grid.GridCellFont(m_grid_io, row, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_io, row, 2, 
						   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
						   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	m_p_grid.GridCellText(m_grid_io, row, 2, "Ribbon Output");

	row = 7;
	m_p_grid.GridCellFont(m_grid_io, row, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_io, row, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_io, row, 1, "INPUT");
	
	m_p_grid.GridCellFont(m_grid_io, row, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_io, row, 2, 
						   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
						   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	m_p_grid.GridCellText(m_grid_io, row, 2, "Data Ready");
}

void CDialog_Work_Barcode_Print::OnCell_Left_Click(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CString str_tmp, str_data;
	int bit;
	if(lpcc->Row == 0 || lpcc->Col == 1) return;

	if(wParam == IDC_CUSTOM_IO)
	{
		bit = 0; // 나중에 
		
		if(FAS_IO.n_out[bit] == IO_ON)
		{
			FAS_IO.n_out[bit]	= IO_OFF;
		}
		else
		{
			FAS_IO.n_out[bit]	= IO_ON;
		}
	}
}

void CDialog_Work_Barcode_Print::OnOutputCheck()
{
	int bit;
	int row;
	
	m_grid_io = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);
	
	bit = 1;
	row = 1;
	if(FAS_IO.n_out[bit] == IO_ON)
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
						       RGB(st_work.n_grid_r[1][0], st_work.n_grid_g[1][0], st_work.n_grid_b[1][0]), 
							   RGB(st_work.n_text_r[1][0], st_work.n_text_g[1][0], st_work.n_text_b[1][0]));
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[1][1], st_work.n_grid_g[1][1], st_work.n_grid_b[1][1]), 
							   RGB(st_work.n_text_r[1][1], st_work.n_text_g[1][1], st_work.n_text_b[1][1]));
	}
}

void CDialog_Work_Barcode_Print::OnInputCheck()
{
	int bit;
	int row;

	m_grid_io = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);
	
	bit = 1;
	row = 2;
	if(FAS_IO.n_in[bit] == IO_ON)
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][0], st_work.n_grid_g[0][0], st_work.n_grid_b[0][0]), 
							   RGB(st_work.n_text_r[0][0], st_work.n_text_g[0][0], st_work.n_text_b[0][0]));
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
							   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	}

	bit = 2;
	row = 3;
	if(FAS_IO.n_in[bit] == IO_ON)
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][0], st_work.n_grid_g[0][0], st_work.n_grid_b[0][0]), 
							   RGB(st_work.n_text_r[0][0], st_work.n_text_g[0][0], st_work.n_text_b[0][0]));
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
							   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	}

	bit = 2;
	row = 4;
	if(FAS_IO.n_in[bit] == IO_ON)
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][0], st_work.n_grid_g[0][0], st_work.n_grid_b[0][0]), 
							   RGB(st_work.n_text_r[0][0], st_work.n_text_g[0][0], st_work.n_text_b[0][0]));
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
							   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	}

	bit = 2;
	row = 5;
	if(FAS_IO.n_in[bit] == IO_ON)
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][0], st_work.n_grid_g[0][0], st_work.n_grid_b[0][0]), 
							   RGB(st_work.n_text_r[0][0], st_work.n_text_g[0][0], st_work.n_text_b[0][0]));
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
							   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	}

	bit = 2;
	row = 6;
	if(FAS_IO.n_in[bit] == IO_ON)
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][0], st_work.n_grid_g[0][0], st_work.n_grid_b[0][0]), 
						       RGB(st_work.n_text_r[0][0], st_work.n_text_g[0][0], st_work.n_text_b[0][0]));
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
							   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	}

	bit = 2;
	row = 7;
	if(FAS_IO.n_in[bit] == IO_ON)
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][0], st_work.n_grid_g[0][0], st_work.n_grid_b[0][0]), 
							   RGB(st_work.n_text_r[0][0], st_work.n_text_g[0][0], st_work.n_text_b[0][0]));
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_io, row, 2, 
							   RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
							   RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
	}
}

void CDialog_Work_Barcode_Print::OnTimer(UINT nIDEvent) 
{
	int nRet_1;
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
		case TM_IN_READ:
			OnInputCheck();
			break;
			
		case TM_OUT_READ:
			OnOutputCheck();
			break;
		
		////20140214
		case TM_BARCODE_PRINT:
			nRet_1 = OnBarcodeRead_Excution();
			if(nRet_1 == RET_GOOD)
			{
				KillTimer(TM_BARCODE_PRINT);
			}
			else if(nRet_1 == RET_ERROR)
			{
				KillTimer(TM_BARCODE_PRINT);
			}		

			break;
        ////
 
	}
	CDialog::OnTimer(nIDEvent);
}

void CDialog_Work_Barcode_Print::OnBtnApply() 
{
	// TODO: Add your control notification handler code here
	CDialog_Select select_dlg;
	int n_response;
	
	st_msg.mstr_confirm_msg = "프린터 기능을 적용하시겠습니까?...";
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = "Do you want to apply the printer Function?...";
	}

	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)	
	{
		Data_Apply();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		LogFile_Create();		// Log File 생성.
		Data_Backup();
		
		//////////////////////////////////// 20120522
		mcls_barcode.OnBarcode_Label_Data_Save();	
		mcls_barcode.OnBarcode_Label_Data_Load();
	}
	else if (n_response == IDCANCEL)
	{
		Data_Recovery();
	}	
}

void CDialog_Work_Barcode_Print::OnBtnBarcodePrint() 
{
	// TODO: Add your control notification handler code here
///////////////////////////////////// 20120524
	//	Func.Barcode_Print();	//20120524
	CDialog_Select select_dlg;

	if(m_b_stop_req == TRUE)
	{
		if(st_serial.n_connect[1] == YES)
		{
			st_serial.str_snd[1] = strBarcodeData;
			::PostMessage(st_handler.hWnd, WM_DATA_SEND, 2, 0);  // 데이터 송신 요청
		}
		else
		{
			st_msg.mstr_confirm_msg = _T("바코드 프린터 통신상태를 확인해 주세요.");
			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
			{
				st_msg.mstr_confirm_msg = _T("Please check the communication status of bar code printers.");
			}

			select_dlg.DoModal();
		}
	}
/////////////////////////////////////
}

void CDialog_Work_Barcode_Print::OnBtnBarcodeCreate() 
{
////////////////////////////////////////////////////////////////////// 20120620
	// TODO: Add your control notification handler code here

	n_barcoderead_step = 0;
	SetTimer(TM_BARCODE_PRINT, 100, NULL);

}

/*
int CDialog_Work_Barcode_Print::OutLptPort(CString str_text)
{
	HANDLE hLpt;
	DWORD dwWritten; 

    hLpt = CreateFile("LPT1:", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
    if (hLpt == INVALID_HANDLE_VALUE)
	{ 
 //       AfxMessageBox("Can not open LPT1 !!!");
		CloseHandle(hLpt); 

		return CTLBD_RET_ERROR;
	}
    else
	{ 
        WriteFile(hLpt, str_text, str_text.GetLength(), &dwWritten, NULL);
        CloseHandle(hLpt); 
    }

	return CTLBD_RET_GOOD;
}*/
int CDialog_Work_Barcode_Print::OutLptPort(CString str_text)
{
	HANDLE hLpt;
	DWORD dwWritten; 
//	OVERLAPPED ov = {0};
//	INFO info = {0};


    hLpt = CreateFile("LPT1:", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
//	ov.Offset = str_text.GetLength()
//	ov.hEvent = CreateEvent(NULL, TRUE, FALSE, "");
	
    if (hLpt == INVALID_HANDLE_VALUE)
	{ 
 //       AfxMessageBox("Can not open LPT1 !!!");
		CloseHandle(hLpt);
		return CTLBD_RET_ERROR;
	}
    else
	{ 
        //WriteFile(hLpt, str_text, str_text.GetLength(), &dwWritten, NULL);
        //CloseHandle(hLpt); 
		BOOL bRet = WriteFile(hLpt, str_text, str_text.GetLength(), &dwWritten, NULL);
		if(bRet == FALSE)
		{
			if(GetLastError() != ERROR_IO_PENDING)
			{
 				if (st_handler.cwnd_list != NULL)
				{ 
					sprintf(st_msg.c_abnormal_msg, "WriteFile Error");
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				}
			}
			return CTLBD_RET_ERROR;
		}
		//WaitForSingleObject(hLpt, INFINITE);
		for(;;)
		{
			if(WaitForSingleObject(hLpt, 2000) == WAIT_OBJECT_0)
				break;

			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (!AfxGetApp()->PumpMessage())
					break;
			}
		}
		CloseHandle(hLpt); 
    }

	return CTLBD_RET_GOOD;
}
//////////////////////////////////////////////////////////////////////

void CDialog_Work_Barcode_Print::Init_Label()
{
	m_msg_barcode_label.SetFont(m_p_font);
	m_msg_barcode_label.SetWindowText(_T("-"));
	m_msg_barcode_label.SetCenterText();
	m_msg_barcode_label.SetColor(YELLOW_L);
	m_msg_barcode_label.SetGradientColor(YELLOW_L);
	m_msg_barcode_label.SetTextColor(BLACK_C);

	m_msg_number.SetFont(m_p_font);
	m_msg_number.SetWindowText(_T("1"));
	m_msg_number.SetCenterText();
	m_msg_number.SetColor(RED_C);
	m_msg_number.SetGradientColor(RED_C);
	m_msg_number.SetTextColor(BLACK_C);

	m_msg_number2.SetFont(m_p_font);
	m_msg_number2.SetWindowText(_T("2"));
	m_msg_number2.SetCenterText();
	m_msg_number2.SetColor(RED_C);
	m_msg_number2.SetGradientColor(RED_C);
	m_msg_number2.SetTextColor(BLACK_C);

	m_msg_number3.SetFont(m_p_font);
	m_msg_number3.SetWindowText(_T("3"));
	m_msg_number3.SetCenterText();
	m_msg_number3.SetColor(RED_C);
	m_msg_number3.SetGradientColor(RED_C);
	m_msg_number3.SetTextColor(BLACK_C);

	m_msg_number4.SetFont(m_p_font);
	m_msg_number4.SetWindowText(_T("4"));
	m_msg_number4.SetCenterText();
	m_msg_number4.SetColor(RED_C);
	m_msg_number4.SetGradientColor(RED_C);
	m_msg_number4.SetTextColor(BLACK_C);

	m_msg_number5.SetFont(m_p_font);
	m_msg_number5.SetWindowText(_T("5"));
	m_msg_number5.SetCenterText();
	m_msg_number5.SetColor(RED_C);
	m_msg_number5.SetGradientColor(RED_C);
	m_msg_number5.SetTextColor(BLACK_C);

	m_msg_number6.SetFont(m_p_font);
	m_msg_number6.SetWindowText(_T("6"));
	m_msg_number6.SetCenterText();
	m_msg_number6.SetColor(RED_C);
	m_msg_number6.SetGradientColor(RED_C);
	m_msg_number6.SetTextColor(BLACK_C);

	m_msg_number7.SetFont(m_p_font);
	m_msg_number7.SetWindowText(_T("1"));
	m_msg_number7.SetCenterText();
	m_msg_number7.SetColor(RED_C);
	m_msg_number7.SetGradientColor(RED_C);
	m_msg_number7.SetTextColor(BLACK_C);

	m_msg_number8.SetFont(m_p_font);
	m_msg_number8.SetWindowText(_T("2"));
	m_msg_number8.SetCenterText();
	m_msg_number8.SetColor(RED_C);
	m_msg_number8.SetGradientColor(RED_C);
	m_msg_number8.SetTextColor(BLACK_C);

	m_msg_number9.SetFont(m_p_font);
	m_msg_number9.SetWindowText(_T("3"));
	m_msg_number9.SetCenterText();
	m_msg_number9.SetColor(RED_C);
	m_msg_number9.SetGradientColor(RED_C);
	m_msg_number9.SetTextColor(BLACK_C);

	m_msg_number10.SetFont(m_p_font);
	m_msg_number10.SetWindowText(_T("4"));
	m_msg_number10.SetCenterText();
	m_msg_number10.SetColor(RED_C);
	m_msg_number10.SetGradientColor(RED_C);
	m_msg_number10.SetTextColor(BLACK_C);


	m_msg_preview.SetFont(mp_label_print_font1);
	m_msg_preview.SetWindowText(_T(""));
	m_msg_preview.SetCenterText();
// 	m_msg_preview.SetColor(SKY_C);
// 	m_msg_preview.SetGradientColor(SKY_C);
//	m_msg_preview.SetTextColor(BLACK_C);

	m_msg_preview2.SetFont(mp_label_print_font);
	m_msg_preview2.SetWindowText(_T(""));
	m_msg_preview2.SetCenterText();
// 	m_msg_preview2.SetColor(SKY_C);
// 	m_msg_preview2.SetGradientColor(SKY_C);
//	m_msg_preview2.SetTextColor(BLACK_C);

	m_msg_preview3.SetFont(mp_label_font);
	m_msg_preview3.SetWindowText(_T(""));
	m_msg_preview3.SetCenterText();
// 	m_msg_preview3.SetColor(SKY_C);
// 	m_msg_preview3.SetGradientColor(SKY_C);
 //	m_msg_preview3.SetTextColor(BLACK_C);

}

///////////////////////////////////////////////// 20120828
void CDialog_Work_Barcode_Print::Init_Data()
{
	CString mst_temp;
	if(st_handler.mn_jobComplete == YES)
	{
		m_edit_labelset_data.SetWindowText(_T(st_NW.mstr_1Contents_1[0]));
		m_edit_labelset_data2.SetWindowText(_T(st_NW.mstr_2Contents_1[0]));
		mst_temp = st_NW.mstr_3Contents_1[0] + " " + st_NW.mstr_3Contents_back_1[0];
		m_edit_labelset_data3.SetWindowText(_T(mst_temp));
		m_edit_labelset_data4.SetWindowText(_T(st_NW.mstr_LotNoContents_1[0]));
	}
	else
	{
		m_edit_labelset_data.SetWindowText(_T("32GB 4Rx4 PC3-149000L-13-12-C0"));
		m_edit_labelset_data2.SetWindowText(_T("Made in Korea"));
		m_edit_labelset_data3.SetWindowText(_T("M386B4G70DM0-CMA4 1402"));
		m_edit_labelset_data4.SetWindowText(_T("W03A000402"));
	}
}
/////////////////////////////////////////////////
void CDialog_Work_Barcode_Print::OnMsgBarcodeLabel() 
{
	
	CString str_tmp;
	
	///////////////////////////////////////// 20120516
	int n_response;									// 대화 상자 리턴 플래그
	
	str_tmp.Format("%s", "LED Barcode Test");
	(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	st_msg.mstr_typing_msg = str_tmp;
	n_response = keyboard_dlg.DoModal();
	if (n_response == IDOK)
	{
		m_b_stop_req = TRUE;	//20120525
		str_tmp = st_msg.mstr_typing_msg;
	}
	else
	{
		return;
	}
	/////////////////////////////////////////

	UpdateData(FALSE);
	m_msg_barcode_label.SetWindowText(str_tmp);
}
//////////////////////////////////////////////////// 20120525
void CDialog_Work_Barcode_Print::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
		m_b_stop_req = FALSE;	

}
////////////////////////////////////////////////////


//////////////////////////////////////////////////// 20120604

void CDialog_Work_Barcode_Print::OnSelchangeListLabelType1() 
{
	// TODO: Add your control notification handler code here
	CString str_selected_lbl;						// 선택된 디바이스 이름 저장 변수
	int n_response;				// 대화 상자 리턴 플래그
	CDialog_Select  select_dlg;
	
	int n_index = m_list_label_type1.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_label_type1.GetText(n_index, str_selected_lbl);
		mstr_label_name1[1] = str_selected_lbl;
		mn_label_mode1[1] = n_index;
	}


	st_msg.mstr_confirm_msg = _T(mstr_label_name1[1] + " Label을 선택하시겠습니까?? ");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Are you sure you want to select the Label" + mstr_label_name1[1]);
	}
	
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		st_basic.mstr_label_name = mstr_label_name1[1];

		st_basic.n_mode_label = mn_label_mode1[1];

		mcls_barcode.OnBarcode_Label_Data_Load();
		OnBarcode_Label_Data_Set();
		OnBarcode_Label_Data_Display();

	}
	else if (n_response == IDCANCEL)
	{
		OnBarcode_Label_Focus_Set();

		mn_label_mode1[1] = mn_label_mode1[0];
		mstr_label_name1[1] = mstr_label_name1[0];
	}		
}

void CDialog_Work_Barcode_Print::OnBarcode_Digital_Count_Set()
{	
	////////////////////////////////////////////////////////// 20120601
	m_dgt_labelset_sd1.SubclassDlgItem(IDC_DGT_LABELSET_SD1, this);
	m_dgt_labelset_sd1.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_sd1.SetValue(mn_labelset_sd1[1]);

	m_dgt_labelset_lt1.SubclassDlgItem(IDC_DGT_LABELSET_LT1, this);
	m_dgt_labelset_lt1.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lt1.SetValue(mn_labelset_lt1[1]);

	m_dgt_labelset_lh1.SubclassDlgItem(IDC_DGT_LABELSET_LH1, this);
	m_dgt_labelset_lh1.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lh1.SetValue(mn_labelset_lh1[1]);

	m_dgt_labelset_lh2.SubclassDlgItem(IDC_DGT_LABELSET_LH2, this);
	m_dgt_labelset_lh2.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lh2.SetValue(mn_labelset_lh2[1]);

	
//////////////// 20120906	
// 	m_dgt_labelset_x1_pos1.SubclassDlgItem(IDC_DGT_LABELSET_X1_POS1, this);
// 	m_dgt_labelset_x1_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x1_pos1.SetValue(mn_labelset_x1_pos1[1]);
	m_dgt_labelset_x1_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);


// 	m_dgt_labelset_x2_pos1.SubclassDlgItem(IDC_DGT_LABELSET_X2_POS1, this);
// 	m_dgt_labelset_x2_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x2_pos1.SetValue(mn_labelset_x2_pos1[1]);
	m_dgt_labelset_x2_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x3_pos1.SubclassDlgItem(IDC_DGT_LABELSET_X3_POS1, this);
// 	m_dgt_labelset_x3_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x3_pos1.SetValue(mn_labelset_x3_pos1[1]);
	m_dgt_labelset_x3_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x4_pos1.SubclassDlgItem(IDC_DGT_LABELSET_X4_POS1, this);
// 	m_dgt_labelset_x4_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x4_pos1.SetValue(mn_labelset_x4_pos1[1]);
	m_dgt_labelset_x4_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x5_pos1.SubclassDlgItem(IDC_DGT_LABELSET_X5_POS1, this);
// 	m_dgt_labelset_x5_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x5_pos1.SetValue(mn_labelset_x5_pos1[1]);
	m_dgt_labelset_x5_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x6_pos1.SubclassDlgItem(IDC_DGT_LABELSET_X6_POS1, this);
// 	m_dgt_labelset_x6_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x6_pos1.SetValue(mn_labelset_x6_pos1[1]);
	m_dgt_labelset_x6_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y1_pos1.SubclassDlgItem(IDC_DGT_LABELSET_Y1_POS1, this);
// 	m_dgt_labelset_y1_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y1_pos1.SetValue(mn_labelset_y1_pos1[1]);
	m_dgt_labelset_y1_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y2_pos1.SubclassDlgItem(IDC_DGT_LABELSET_Y2_POS1, this);
// 	m_dgt_labelset_y2_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y2_pos1.SetValue(mn_labelset_y2_pos1[1]);
	m_dgt_labelset_y2_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y3_pos1.SubclassDlgItem(IDC_DGT_LABELSET_Y3_POS1, this);
// 	m_dgt_labelset_y3_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y3_pos1.SetValue(mn_labelset_y3_pos1[1]);
	m_dgt_labelset_y3_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y4_pos1.SubclassDlgItem(IDC_DGT_LABELSET_Y4_POS1, this);
// 	m_dgt_labelset_y4_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y4_pos1.SetValue(mn_labelset_y4_pos1[1]);
	m_dgt_labelset_y4_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y5_pos1.SubclassDlgItem(IDC_DGT_LABELSET_Y5_POS1, this);
// 	m_dgt_labelset_y5_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y5_pos1.SetValue(mn_labelset_y5_pos1[1]);
	m_dgt_labelset_y5_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y6_pos1.SubclassDlgItem(IDC_DGT_LABELSET_Y6_POS1, this);
// 	m_dgt_labelset_y6_pos1.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y6_pos1.SetValue(mn_labelset_y6_pos1[1]);
	m_dgt_labelset_y6_pos1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x1_width1.SubclassDlgItem(IDC_DGT_LABELSET_X1_WIDTH1, this);
// 	m_dgt_labelset_x1_width1.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_x1_width1.SetValue(mn_labelset_x1_width1[1]);
	m_dgt_labelset_x1_width1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x2_width1.SubclassDlgItem(IDC_DGT_LABELSET_X2_WIDTH1, this);
// 	m_dgt_labelset_x2_width1.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_x2_width1.SetValue(mn_labelset_x2_width1[1]);
	m_dgt_labelset_x2_width1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x3_width1.SubclassDlgItem(IDC_DGT_LABELSET_X3_WIDTH1, this);
// 	m_dgt_labelset_x3_width1.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_x3_width1.SetValue(mn_labelset_x3_width1[1]);
	m_dgt_labelset_x3_width1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x4_width1.SubclassDlgItem(IDC_DGT_LABELSET_X4_WIDTH1, this);
// 	m_dgt_labelset_x4_width1.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_x4_width1.SetValue(mn_labelset_x4_width1[1]);
	m_dgt_labelset_x4_width1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y1_height1.SubclassDlgItem(IDC_DGT_LABELSET_Y1_HEIGHT1, this);
// 	m_dgt_labelset_y1_height1.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_y1_height1.SetValue(mn_labelset_y1_height1[1]);
	m_dgt_labelset_y1_height1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y2_height1.SubclassDlgItem(IDC_DGT_LABELSET_Y2_HEIGHT1, this);
// 	m_dgt_labelset_y2_height1.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_y2_height1.SetValue(mn_labelset_y2_height1[1]);
	m_dgt_labelset_y2_height1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y3_height1.SubclassDlgItem(IDC_DGT_LABELSET_Y3_HEIGHT1, this);
// 	m_dgt_labelset_y3_height1.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_y3_height1.SetValue(mn_labelset_y3_height1[1]);
	m_dgt_labelset_y3_height1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y4_height1.SubclassDlgItem(IDC_DGT_LABELSET_Y4_HEIGHT1, this);
// 	m_dgt_labelset_y4_height1.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_y4_height1.SetValue(mn_labelset_y4_height1[1]);
	m_dgt_labelset_y4_height1.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

////////////

	////////////////////////////////////////////////////////// 20120625
	m_dgt_labelset_func_paper.SubclassDlgItem(IDC_DGT_LABELSET_FUNC_PAPER, this);
	m_dgt_labelset_func_paper.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_func_paper.SetValue(mn_labelset_func_paper[1]);

	//////////////////////////////////////////////////////////
}

void CDialog_Work_Barcode_Print::OnBarcode_LogFile_Create()
{
	CString str_msg;
	
	if (mn_label_mode1[0] !=			mn_label_mode1[1])
	{
		switch (mn_label_mode1[1])
		{
		case 0:
			str_msg = "[Label Mode] Was changed by With Device.";
			break;
		case 1:
			str_msg = "[Label Mode] Was changed by With Out Device.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	////////////////////////////////////////////////////////////////////// 20120601
	if (mn_labelset_sd1[0] != mn_labelset_sd1[1])
	{
		str_msg.Format("[Basic] SD Label Set was changed by %d to %d", mn_labelset_sd1[0], mn_labelset_sd1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_labelset_lt1[0] != mn_labelset_lt1[1])
	{
		str_msg.Format("[Basic] LT Label Set was changed by %d to %d", mn_labelset_lt1[0], mn_labelset_lt1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_labelset_lh1[0] != mn_labelset_lh1[1])
	{
		str_msg.Format("[Basic] LH X Label Set was changed by %d to %d", mn_labelset_lh1[0], mn_labelset_lh1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_labelset_lh2[0] != mn_labelset_lh2[1])
	{
		str_msg.Format("[Basic] LH Y Label Set was changed by %d to %d", mn_labelset_lh2[0], mn_labelset_lh2[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

/////////////////20120906
	if (md_labelset_x1_pos1[0] != md_labelset_x1_pos1[1])
	{
		str_msg.Format("[Basic] X1-POS Label Set was changed by %d to %d", md_labelset_x1_pos1[0], md_labelset_x1_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x2_pos1[0] != md_labelset_x2_pos1[1])
	{
		str_msg.Format("[Basic] X2-POS Label Set was changed by %d to %d", md_labelset_x2_pos1[0], md_labelset_x2_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x3_pos1[0] != md_labelset_x3_pos1[1])
	{
		str_msg.Format("[Basic] X3-POS Label Set was changed by %d to %d", md_labelset_x3_pos1[0], md_labelset_x3_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x4_pos1[0] != md_labelset_x4_pos1[1])
	{
		str_msg.Format("[Basic] X4-POS Label Set was changed by %d to %d", md_labelset_x4_pos1[0], md_labelset_x4_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x5_pos1[0] != md_labelset_x5_pos1[1])
	{
		str_msg.Format("[Basic] X5-POS Label Set was changed by %d to %d", md_labelset_x5_pos1[0], md_labelset_x5_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x6_pos1[0] != md_labelset_x6_pos1[1])
	{
		str_msg.Format("[Basic] X6-POS Label Set was changed by %d to %d", md_labelset_x6_pos1[0], md_labelset_x6_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y1_pos1[0] != md_labelset_y1_pos1[1])
	{
		str_msg.Format("[Basic] Y1-POS Label Set was changed by %d to %d", md_labelset_y1_pos1[0], md_labelset_y1_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y2_pos1[0] != md_labelset_y2_pos1[1])
	{
		str_msg.Format("[Basic] Y2-POS Label Set was changed by %d to %d", md_labelset_y2_pos1[0], md_labelset_y2_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y3_pos1[0] != md_labelset_y3_pos1[1])
	{
		str_msg.Format("[Basic] Y3-POS Label Set was changed by %d to %d", md_labelset_y3_pos1[0], md_labelset_y3_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y4_pos1[0] != md_labelset_y4_pos1[1])
	{
		str_msg.Format("[Basic] Y4-POS Label Set was changed by %d to %d", md_labelset_y4_pos1[0], md_labelset_y4_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y5_pos1[0] != md_labelset_y5_pos1[1])
	{
		str_msg.Format("[Basic] Y5-POS Label Set was changed by %d to %d", md_labelset_y5_pos1[0], md_labelset_y5_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y6_pos1[0] != md_labelset_y6_pos1[1])
	{
		str_msg.Format("[Basic] Y6-POS Label Set was changed by %d to %d", md_labelset_y6_pos1[0], md_labelset_y6_pos1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x1_width1[0] != md_labelset_x1_width1[1])
	{
		str_msg.Format("[Basic] X1-WIDTH Label Set was changed by %d to %d", md_labelset_x1_width1[0], md_labelset_x1_width1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x2_width1[0] != md_labelset_x2_width1[1])
	{
		str_msg.Format("[Basic] X2-WIDTH Label Set was changed by %d to %d", md_labelset_x2_width1[0], md_labelset_x2_width1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x3_width1[0] != md_labelset_x3_width1[1])
	{
		str_msg.Format("[Basic] X3-WIDTH Label Set was changed by %d to %d", md_labelset_x3_width1[0], md_labelset_x3_width1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x4_width1[0] != md_labelset_x4_width1[1])
	{
		str_msg.Format("[Basic] X4-WIDTH Label Set was changed by %d to %d", md_labelset_x4_width1[0], md_labelset_x4_width1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y1_height1[0] != md_labelset_y1_height1[1])
	{
		str_msg.Format("[Basic] Y1-HEIGHT Label Set was changed by %d to %d", md_labelset_y1_height1[0], md_labelset_y1_height1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y2_height1[0] != md_labelset_y2_height1[1])
	{
		str_msg.Format("[Basic] Y2-HEIGHT Label Set was changed by %d to %d", md_labelset_y2_height1[0], md_labelset_y2_height1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y3_height1[0] != md_labelset_y3_height1[1])
	{
		str_msg.Format("[Basic] Y3-HEIGHT Label Set was changed by %d to %d", md_labelset_y3_height1[0], md_labelset_y3_height1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y4_height1[0] != md_labelset_y4_height1[1])
	{
		str_msg.Format("[Basic] Y4-HEIGHT Label Set was changed by %d to %d", md_labelset_y4_height1[0], md_labelset_y4_height1[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
////////////////////
	////////////////////////////////////////////////////////////////////// 20120625
	if (mn_labelset_func_paper[0] != mn_labelset_func_paper[1])	
	{
		str_msg.Format("[Basic] PAPER Multi out Set was changed by %d to %d", mn_labelset_func_paper[0], mn_labelset_func_paper[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	//////////////////////////////////////////////////////////////////////

}

void CDialog_Work_Barcode_Print::OnBarcode_Label_Data_Apply()
{
// 	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_temp_device);
// 	mstr_temp_label.MakeUpper();
// 	mstr_temp_label.TrimLeft(' ');               
// 	mstr_temp_label.TrimRight(' ');
	
	if (mstr_label_name1[0] != mstr_label_name1[1])//mstr_temp_label)
	{
		OnBarcode_Label_DeviceData_Apply();
		OnBarcode_Label_Data_Set();				// 전역 변수의 Data를 받아온다.
		OnBarcode_Label_Data_BackUp();
		OnBarcode_Label_Data_Display();
// 		if (b_create_list == TRUE) 
// 		{
// 			b_create_list = FALSE;
// 		}
		
	}
	else
	{
		st_basic.n_mode_label =	mn_label_mode1[1];		// With, With Out, No Tra
		
		st_basic.mn_labelset_sd = mn_labelset_sd1[1];
		st_basic.mn_labelset_lt = mn_labelset_lt1[1];
		st_basic.mn_labelset_lh1 = mn_labelset_lh1[1];
		st_basic.mn_labelset_lh2 = mn_labelset_lh2[1];
		//////////20120906
		if(st_handler.mn_jobComplete == YES)
		{
			st_basic.md_labelset_x1_pos = atof(st_NW.mstr_1PosX_1[0]);//1번 위치
			st_basic.md_labelset_x2_pos = atof(st_NW.mstr_2PosX_1[0]);//2번 위치
			st_basic.md_labelset_x3_pos = atof(st_NW.mstr_3PosX_1[0]);//3번 위치
			st_basic.md_labelset_x4_pos = atof(st_NW.mstr_LotNoPosX_1[0]);//Lot No 위치
			st_basic.md_labelset_x5_pos = atof(st_NW.mstr_2DPosX_1[0]);//2D 위치
			st_basic.md_labelset_x6_pos = atof(st_NW.mstr_4PosX_1[0]);//4번 위치(바코드)
			st_basic.md_labelset_y1_pos = atof(st_NW.mstr_1PosY_1[0]);//1번 위치
			st_basic.md_labelset_y2_pos = atof(st_NW.mstr_2PosY_1[0]);//2번 위치
			st_basic.md_labelset_y3_pos = atof(st_NW.mstr_3PosY_1[0]);//3번 위치
			st_basic.md_labelset_y4_pos = atof(st_NW.mstr_LotNoPosY_1[0]);//Lot No 위치
			st_basic.md_labelset_y5_pos = atof(st_NW.mstr_2DPosY_1[0]);//2D 위치
			st_basic.md_labelset_y6_pos = atof(st_NW.mstr_4PosY_1[0]);//4번 위치(바코드)
			st_basic.md_labelset_x1_width = atof(st_NW.mstr_1Wid_1[0]);//1번 위치
			st_basic.md_labelset_x2_width = atof(st_NW.mstr_2Wid_1[0]);//2번 위치
			st_basic.md_labelset_x3_width = atof(st_NW.mstr_3Wid_1[0]);//3번 위치
			st_basic.md_labelset_x4_width = atof(st_NW.mstr_LotNoWid_1[0]);//Lot No 위치
			st_basic.md_labelset_y1_height = atof(st_NW.mstr_1High_1[0]);//1번 위치
			st_basic.md_labelset_y2_height = atof(st_NW.mstr_2High_1[0]);//2번 위치
			st_basic.md_labelset_y3_height = atof(st_NW.mstr_3High_1[0]);//3번 위치
			st_basic.md_labelset_y4_height = atof(st_NW.mstr_LotNoHigh_1[0]);//Lot No 위치

		}
		else
		{
			st_basic.md_labelset_x1_pos = md_labelset_x1_pos1[1];
			st_basic.md_labelset_x2_pos = md_labelset_x2_pos1[1];
			st_basic.md_labelset_x3_pos = md_labelset_x3_pos1[1];
			st_basic.md_labelset_x4_pos = md_labelset_x4_pos1[1];
			st_basic.md_labelset_x5_pos = md_labelset_x5_pos1[1];
			st_basic.md_labelset_x6_pos = md_labelset_x6_pos1[1];
			st_basic.md_labelset_y1_pos = md_labelset_y1_pos1[1];
			st_basic.md_labelset_y2_pos = md_labelset_y2_pos1[1];
			st_basic.md_labelset_y3_pos = md_labelset_y3_pos1[1];
			st_basic.md_labelset_y4_pos = md_labelset_y4_pos1[1];
			st_basic.md_labelset_y5_pos = md_labelset_y5_pos1[1];
			st_basic.md_labelset_y6_pos = md_labelset_y6_pos1[1];
			st_basic.md_labelset_x1_width = md_labelset_x1_width1[1];
			st_basic.md_labelset_x2_width = md_labelset_x2_width1[1];
			st_basic.md_labelset_x3_width = md_labelset_x3_width1[1];
			st_basic.md_labelset_x4_width = md_labelset_x4_width1[1];
			st_basic.md_labelset_y1_height = md_labelset_y1_height1[1];
			st_basic.md_labelset_y2_height = md_labelset_y2_height1[1];
			st_basic.md_labelset_y3_height = md_labelset_y3_height1[1];
			st_basic.md_labelset_y4_height = md_labelset_y4_height1[1];
			///////////

		st_basic.mn_labelset_func_paper = mn_labelset_func_paper[1];	//20120625

		}

	}
}

void CDialog_Work_Barcode_Print::OnBarcode_Label_Data_BackUp()
{
	mn_label_mode1[0] =				mn_label_mode1[1];				// With, With Out, No Tray
	mstr_label_name1[0] =			mstr_label_name1[1];					// 선택된 디바이스 종류 저장 변수

	mn_labelset_sd1[0] =	        	mn_labelset_sd1[1];
	mn_labelset_lt1[0] =	        	mn_labelset_lt1[1];
	mn_labelset_lh1[0] =	        	mn_labelset_lh1[1];
	mn_labelset_lh2[0] =	        	mn_labelset_lh2[1];
	///////////20120906
	if(st_handler.mn_jobComplete == YES)
	{
		md_labelset_x1_pos1[0] =	    atof(st_NW.mstr_1PosX_1[0]);//1번 위치
		md_labelset_x2_pos1[0] =	    atof(st_NW.mstr_2PosX_1[0]);//2번 위치
		md_labelset_x3_pos1[0] =	    atof(st_NW.mstr_3PosX_1[0]);//3번 위치
		md_labelset_x4_pos1[0] =	    atof(st_NW.mstr_LotNoPosX_1[0]);//Lot No 위치
		md_labelset_x5_pos1[0] =	    atof(st_NW.mstr_2DPosX_1[0]);//2D 위치
		md_labelset_x6_pos1[0] =	    atof(st_NW.mstr_4PosX_1[0]);//4번 위치(바코드)

		md_labelset_y1_pos1[0] =	    atof(st_NW.mstr_1PosY_1[0]);//1번 위치
		md_labelset_y2_pos1[0] =	    atof(st_NW.mstr_2PosY_1[0]);//2번 위치
		md_labelset_y3_pos1[0] =	    atof(st_NW.mstr_3PosY_1[0]);//3번 위치
		md_labelset_y4_pos1[0] =	    atof(st_NW.mstr_LotNoPosY_1[0]);//Lot No 위치
		md_labelset_y5_pos1[0] =	    atof(st_NW.mstr_2DPosY_1[0]);//2D 위치
		md_labelset_y6_pos1[0] =	    atof(st_NW.mstr_4PosY_1[0]);//4번 위치(바코드)

		md_labelset_x1_width1[0] =	    atof(st_NW.mstr_1Wid_1[0]);//1번 위치
		md_labelset_x2_width1[0] =	    atof(st_NW.mstr_2Wid_1[0]);//2번 위치
		md_labelset_x3_width1[0] =	    atof(st_NW.mstr_3Wid_1[0]);//3번 위치
		md_labelset_x4_width1[0] =	    atof(st_NW.mstr_LotNoWid_1[0]);//Lot No 위치

		md_labelset_y1_height1[0] =	    atof(st_NW.mstr_1High_1[0]);//1번 위치
		md_labelset_y2_height1[0] =	    atof(st_NW.mstr_2High_1[0]);//2번 위치
		md_labelset_y3_height1[0] =	    atof(st_NW.mstr_3High_1[0]);//3번 위치
		md_labelset_y4_height1[0] =	    atof(st_NW.mstr_LotNoHigh_1[0]);//Lot No 위치

//		st_basic.md_labelset_x1_pos = md_labelset_x1_pos1[1];
//		st_basic.md_labelset_x2_pos = md_labelset_x2_pos1[1];
//		st_basic.md_labelset_x3_pos = md_labelset_x3_pos1[1];
//		st_basic.md_labelset_x4_pos = md_labelset_x4_pos1[1];
//		st_basic.md_labelset_x5_pos = md_labelset_x5_pos1[1];
//		st_basic.md_labelset_x6_pos = md_labelset_x6_pos1[1];
//		st_basic.md_labelset_y1_pos = md_labelset_y1_pos1[1];
//		st_basic.md_labelset_y2_pos = md_labelset_y2_pos1[1];
//		st_basic.md_labelset_y3_pos = md_labelset_y3_pos1[1];
//		st_basic.md_labelset_y4_pos = md_labelset_y4_pos1[1];
//		st_basic.md_labelset_y5_pos = md_labelset_y5_pos1[1];
//		st_basic.md_labelset_y6_pos = md_labelset_y6_pos1[1];
//		st_basic.md_labelset_x1_width = md_labelset_x1_width1[1];
//		st_basic.md_labelset_x2_width = md_labelset_x2_width1[1];
//		st_basic.md_labelset_x3_width = md_labelset_x3_width1[1];
//		st_basic.md_labelset_x4_width = md_labelset_x4_width1[1];
//		st_basic.md_labelset_y1_height = md_labelset_y1_height1[1];
//		st_basic.md_labelset_y2_height = md_labelset_y2_height1[1];
//		st_basic.md_labelset_y3_height = md_labelset_y3_height1[1];
//		st_basic.md_labelset_y4_height = md_labelset_y4_height1[1];

	}
	else
	{
		md_labelset_x1_pos1[0] =	        md_labelset_x1_pos1[1];
		md_labelset_x2_pos1[0] =	        md_labelset_x2_pos1[1];
		md_labelset_x3_pos1[0] =	        md_labelset_x3_pos1[1];
		md_labelset_x4_pos1[0] =	        md_labelset_x4_pos1[1];
		md_labelset_x5_pos1[0] =	        md_labelset_x5_pos1[1];
		md_labelset_x6_pos1[0] =	        md_labelset_x6_pos1[1];
		md_labelset_y1_pos1[0] =	        md_labelset_y1_pos1[1];
		md_labelset_y2_pos1[0] =	        md_labelset_y2_pos1[1];
		md_labelset_y3_pos1[0] =	        md_labelset_y3_pos1[1];
		md_labelset_y4_pos1[0] =	        md_labelset_y4_pos1[1];
		md_labelset_y5_pos1[0] =	        md_labelset_y5_pos1[1];
		md_labelset_y6_pos1[0] =	        md_labelset_y6_pos1[1];
		md_labelset_x1_width1[0] =	    md_labelset_x1_width1[1];
		md_labelset_x2_width1[0] =	    md_labelset_x2_width1[1];
		md_labelset_x3_width1[0] =	    md_labelset_x3_width1[1];
		md_labelset_x4_width1[0] =	    md_labelset_x4_width1[1];
		md_labelset_y1_height1[0] =	    md_labelset_y1_height1[1];
		md_labelset_y2_height1[0] =	    md_labelset_y2_height1[1];
		md_labelset_y3_height1[0] =	    md_labelset_y3_height1[1];
		md_labelset_y4_height1[0] =	    md_labelset_y4_height1[1];
	}
	//////////////


	mn_labelset_func_paper[0] =     mn_labelset_func_paper[1];	//20120625

}

void CDialog_Work_Barcode_Print::OnBarcode_Label_Data_Recovery()
{
	mn_label_mode1[1] =				mn_label_mode1[0];				// With, With Out, No Tray
	mstr_label_name1[1] =			mstr_label_name1[0];					// 선택된 디바이스 종류 저장 변수	

	mn_labelset_sd1[1] =	        	mn_labelset_sd1[0];
	mn_labelset_lt1[1] =	        	mn_labelset_lt1[0];
	mn_labelset_lh1[1] =	        	mn_labelset_lh1[0];
	mn_labelset_lh2[1] =	        	mn_labelset_lh2[0];
	//////////20120906
	if(st_handler.mn_jobComplete == YES)
	{
		md_labelset_x1_pos1[1] =	    atof(st_NW.mstr_1PosX_1[0]);//1번 위치
		md_labelset_x2_pos1[1] =	    atof(st_NW.mstr_2PosX_1[0]);//2번 위치
		md_labelset_x3_pos1[1] =	    atof(st_NW.mstr_3PosX_1[0]);//3번 위치
		md_labelset_x4_pos1[1] =	    atof(st_NW.mstr_LotNoPosX_1[0]);//Lot No 위치
		md_labelset_x5_pos1[1] =	    atof(st_NW.mstr_2DPosX_1[0]);//2D 위치
		md_labelset_x6_pos1[1] =	    atof(st_NW.mstr_4PosX_1[0]);//4번 위치(바코드)

		md_labelset_y1_pos1[1] =	    atof(st_NW.mstr_1PosY_1[0]);//1번 위치
		md_labelset_y2_pos1[1] =	    atof(st_NW.mstr_2PosY_1[0]);//2번 위치
		md_labelset_y3_pos1[1] =	    atof(st_NW.mstr_3PosY_1[0]);//3번 위치
		md_labelset_y4_pos1[1] =	    atof(st_NW.mstr_LotNoPosY_1[0]);//Lot No 위치
		md_labelset_y5_pos1[1] =	    atof(st_NW.mstr_2DPosY_1[0]);//2D 위치
		md_labelset_y6_pos1[1] =	    atof(st_NW.mstr_4PosY_1[0]);//4번 위치(바코드)

		md_labelset_x1_width1[1] =	    atof(st_NW.mstr_1Wid_1[0]);//1번 위치
		md_labelset_x2_width1[1] =	    atof(st_NW.mstr_2Wid_1[0]);//2번 위치
		md_labelset_x3_width1[1] =	    atof(st_NW.mstr_3Wid_1[0]);//3번 위치
		md_labelset_x4_width1[1] =	    atof(st_NW.mstr_LotNoWid_1[0]);//Lot No 위치

		md_labelset_y1_height1[1] =	    atof(st_NW.mstr_1High_1[0]);//1번 위치
		md_labelset_y2_height1[1] =	    atof(st_NW.mstr_2High_1[0]);//2번 위치
		md_labelset_y3_height1[1] =	    atof(st_NW.mstr_3High_1[0]);//3번 위치
		md_labelset_y4_height1[1] =	    atof(st_NW.mstr_LotNoHigh_1[0]);//Lot No 위치

//		st_basic.md_labelset_x1_pos = md_labelset_x1_pos1[1];
//		st_basic.md_labelset_x2_pos = md_labelset_x2_pos1[1];
//		st_basic.md_labelset_x3_pos = md_labelset_x3_pos1[1];
//		st_basic.md_labelset_x4_pos = md_labelset_x4_pos1[1];
//		st_basic.md_labelset_x5_pos = md_labelset_x5_pos1[1];
//		st_basic.md_labelset_x6_pos = md_labelset_x6_pos1[1];
//		st_basic.md_labelset_y1_pos = md_labelset_y1_pos1[1];
//		st_basic.md_labelset_y2_pos = md_labelset_y2_pos1[1];
//		st_basic.md_labelset_y3_pos = md_labelset_y3_pos1[1];
//		st_basic.md_labelset_y4_pos = md_labelset_y4_pos1[1];
//		st_basic.md_labelset_y5_pos = md_labelset_y5_pos1[1];
//		st_basic.md_labelset_y6_pos = md_labelset_y6_pos1[1];
//		st_basic.md_labelset_x1_width = md_labelset_x1_width1[1];
//		st_basic.md_labelset_x2_width = md_labelset_x2_width1[1];
//		st_basic.md_labelset_x3_width = md_labelset_x3_width1[1];
//		st_basic.md_labelset_x4_width = md_labelset_x4_width1[1];
//		st_basic.md_labelset_y1_height = md_labelset_y1_height1[1];
//		st_basic.md_labelset_y2_height = md_labelset_y2_height1[1];
//		st_basic.md_labelset_y3_height = md_labelset_y3_height1[1];
//		st_basic.md_labelset_y4_height = md_labelset_y4_height1[1];

	}
	else
	{
		md_labelset_x1_pos1[1] =	        md_labelset_x1_pos1[0];
		md_labelset_x2_pos1[1] =	        md_labelset_x2_pos1[0];
		md_labelset_x3_pos1[1] =	        md_labelset_x3_pos1[0];
		md_labelset_x4_pos1[1] =	        md_labelset_x4_pos1[0];
		md_labelset_x5_pos1[1] =	        md_labelset_x5_pos1[0];
		md_labelset_x6_pos1[1] =	        md_labelset_x6_pos1[0];
		md_labelset_y1_pos1[1] =	        md_labelset_y1_pos1[0];
		md_labelset_y2_pos1[1] =	        md_labelset_y2_pos1[0];
		md_labelset_y3_pos1[1] =	        md_labelset_y3_pos1[0];
		md_labelset_y4_pos1[1] =	        md_labelset_y4_pos1[0];
		md_labelset_y5_pos1[1] =	        md_labelset_y5_pos1[0];
		md_labelset_y6_pos1[1] =	        md_labelset_y6_pos1[0];
		md_labelset_x1_width1[1] =	    md_labelset_x1_width1[0];
		md_labelset_x2_width1[1] =	    md_labelset_x2_width1[0];
		md_labelset_x3_width1[1] =	    md_labelset_x3_width1[0];
		md_labelset_x4_width1[1] =	    md_labelset_x4_width1[0];
		md_labelset_y1_height1[1] =	    md_labelset_y1_height1[0];
		md_labelset_y2_height1[1] =	    md_labelset_y2_height1[0];
		md_labelset_y3_height1[1] =	    md_labelset_y3_height1[0];
		md_labelset_y4_height1[1] =	    md_labelset_y4_height1[0];
	}
		///////////


	mn_labelset_func_paper[1] =		mn_labelset_func_paper[0];	//20120625

}

void CDialog_Work_Barcode_Print::OnBarcode_Label_Data_Set()
{
	
	mn_label_mode1[1] =				st_basic.n_mode_label;				// With, With Out, No Tray
	mstr_label_name1[1] =			st_basic.mstr_label_name;					// 선택된 디바이스 종류 저장 변수

	mn_labelset_sd1[1] =	        	st_basic.mn_labelset_sd;
	mn_labelset_lt1[1] =	        	st_basic.mn_labelset_lt;
	mn_labelset_lh1[1] =	        	st_basic.mn_labelset_lh1;
	mn_labelset_lh2[1] =	        	st_basic.mn_labelset_lh2;

	//////////20120906
	if(st_handler.mn_jobComplete == YES)
	{
		md_labelset_x1_pos1[1] =	    atof(st_NW.mstr_1PosX_1[0]);//1번 위치
		md_labelset_x2_pos1[1] =	    atof(st_NW.mstr_2PosX_1[0]);//2번 위치
		md_labelset_x3_pos1[1] =	    atof(st_NW.mstr_3PosX_1[0]);//3번 위치
		md_labelset_x4_pos1[1] =	    atof(st_NW.mstr_LotNoPosX_1[0]);//Lot No 위치
		md_labelset_x5_pos1[1] =	    atof(st_NW.mstr_2DPosX_1[0]);//2D 위치
		md_labelset_x6_pos1[1] =	    atof(st_NW.mstr_4PosX_1[0]);//4번 위치(바코드)

		md_labelset_y1_pos1[1] =	    atof(st_NW.mstr_1PosY_1[0]);//1번 위치
		md_labelset_y2_pos1[1] =	    atof(st_NW.mstr_2PosY_1[0]);//2번 위치
		md_labelset_y3_pos1[1] =	    atof(st_NW.mstr_3PosY_1[0]);//3번 위치
		md_labelset_y4_pos1[1] =	    atof(st_NW.mstr_LotNoPosY_1[0]);//Lot No 위치
		md_labelset_y5_pos1[1] =	    atof(st_NW.mstr_2DPosY_1[0]);//2D 위치
		md_labelset_y6_pos1[1] =	    atof(st_NW.mstr_4PosY_1[0]);//4번 위치(바코드)

		md_labelset_x1_width1[1] =	    atof(st_NW.mstr_1Wid_1[0]);//1번 위치
		md_labelset_x2_width1[1] =	    atof(st_NW.mstr_2Wid_1[0]);//2번 위치
		md_labelset_x3_width1[1] =	    atof(st_NW.mstr_3Wid_1[0]);//3번 위치
		md_labelset_x4_width1[1] =	    atof(st_NW.mstr_LotNoWid_1[0]);//Lot No 위치

		md_labelset_y1_height1[1] =	    atof(st_NW.mstr_1High_1[0]);//1번 위치
		md_labelset_y2_height1[1] =	    atof(st_NW.mstr_2High_1[0]);//2번 위치
		md_labelset_y3_height1[1] =	    atof(st_NW.mstr_3High_1[0]);//3번 위치
		md_labelset_y4_height1[1] =	    atof(st_NW.mstr_LotNoHigh_1[0]);//Lot No 위치

 		st_basic.md_labelset_x1_pos = md_labelset_x1_pos1[1];
 		st_basic.md_labelset_x2_pos = md_labelset_x2_pos1[1];
 		st_basic.md_labelset_x3_pos = md_labelset_x3_pos1[1];
 		st_basic.md_labelset_x4_pos = md_labelset_x4_pos1[1];
 		st_basic.md_labelset_x5_pos = md_labelset_x5_pos1[1];
 		st_basic.md_labelset_x6_pos = md_labelset_x6_pos1[1];
 		st_basic.md_labelset_y1_pos = md_labelset_y1_pos1[1];
 		st_basic.md_labelset_y2_pos = md_labelset_y2_pos1[1];
 		st_basic.md_labelset_y3_pos = md_labelset_y3_pos1[1];
 		st_basic.md_labelset_y4_pos = md_labelset_y4_pos1[1];
 		st_basic.md_labelset_y5_pos = md_labelset_y5_pos1[1];
 		st_basic.md_labelset_y6_pos = md_labelset_y6_pos1[1];
 		st_basic.md_labelset_x1_width = md_labelset_x1_width1[1];
 		st_basic.md_labelset_x2_width = md_labelset_x2_width1[1];
 		st_basic.md_labelset_x3_width = md_labelset_x3_width1[1];
 		st_basic.md_labelset_x4_width = md_labelset_x4_width1[1];
 		st_basic.md_labelset_y1_height = md_labelset_y1_height1[1];
 		st_basic.md_labelset_y2_height = md_labelset_y2_height1[1];
 		st_basic.md_labelset_y3_height = md_labelset_y3_height1[1];
 		st_basic.md_labelset_y4_height = md_labelset_y4_height1[1];

	}
	else
	{
		md_labelset_x1_pos1[1] =	    st_basic.md_labelset_x1_pos;
		md_labelset_x2_pos1[1] =	    st_basic.md_labelset_x2_pos;
		md_labelset_x3_pos1[1] =	    st_basic.md_labelset_x3_pos;
		md_labelset_x4_pos1[1] =	    st_basic.md_labelset_x4_pos;
		md_labelset_x5_pos1[1] =	    st_basic.md_labelset_x5_pos;
		md_labelset_x6_pos1[1] =	    st_basic.md_labelset_x6_pos;
		md_labelset_y1_pos1[1] =	    st_basic.md_labelset_y1_pos;
		md_labelset_y2_pos1[1] =	    st_basic.md_labelset_y2_pos;
		md_labelset_y3_pos1[1] =	    st_basic.md_labelset_y3_pos;
		md_labelset_y4_pos1[1] =	    st_basic.md_labelset_y4_pos;
		md_labelset_y5_pos1[1] =	    st_basic.md_labelset_y5_pos;
		md_labelset_y6_pos1[1] =	    st_basic.md_labelset_y6_pos;
		md_labelset_x1_width1[1] =	    st_basic.md_labelset_x1_width;
		md_labelset_x2_width1[1] =	    st_basic.md_labelset_x2_width;
		md_labelset_x3_width1[1] =	    st_basic.md_labelset_x3_width;
		md_labelset_x4_width1[1] =	    st_basic.md_labelset_x4_width;
		md_labelset_y1_height1[1] =	    st_basic.md_labelset_y1_height;
		md_labelset_y2_height1[1] =	    st_basic.md_labelset_y2_height;
		md_labelset_y3_height1[1] =	    st_basic.md_labelset_y3_height;
		md_labelset_y4_height1[1] =	    st_basic.md_labelset_y4_height;
	}

	mn_labelset_func_paper[1] =     st_basic.mn_labelset_func_paper;	//20120625
	
}


void CDialog_Work_Barcode_Print::OnDgtLabelsetSd1() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_sd1.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("SD LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("SD LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "20";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_sd1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_sd1[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_sd1.SetValue(mn_labelset_sd1[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}
void CDialog_Work_Barcode_Print::OnDgtLabelsetLt1() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lt1.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LT LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("LT LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "50";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lt1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lt1[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lt1.SetValue(mn_labelset_lt1[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}				
}


void CDialog_Work_Barcode_Print::OnDgtLabelsetX1Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x1_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X1 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X1 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_x1_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x1_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_x1_pos1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetX2Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x2_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X2 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X2 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_x2_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x2_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_x2_pos1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetX3Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x3_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X3 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X3 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_x3_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x3_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_x3_pos1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetX4Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x4_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X4 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X4 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_x4_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x4_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_x4_pos1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetX5Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x5_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X5 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X5 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_x5_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x5_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_x5_pos1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetX6Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x6_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X6 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X6 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_x6_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x6_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_x6_pos1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetY1Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y1_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y1 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y1 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_y1_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y1_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_y1_pos1.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetY2Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y2_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y2 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y2 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_y2_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y2_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_y2_pos1.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetY3Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y3_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y3 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y3 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_y3_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y3_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_y3_pos1.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetY4Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y4_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y4 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y4 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_y4_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y4_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_y4_pos1.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetY5Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y5_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y5 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y5 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_y5_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y5_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_y5_pos1.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetY6Pos1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y6_pos1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y6 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y6 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_y6_pos1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y6_pos1[1] = atof(mstr_temp);
		m_dgt_labelset_y6_pos1.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetX1Width1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x1_width1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	
	st_msg.mstr_keypad_msg = _T("X1 WIDTH LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X1 WIDTH LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_x1_width1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x1_width1[1] = atof(mstr_temp);
		m_dgt_labelset_x1_width1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetY1Height1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y1_height1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	st_msg.mstr_keypad_msg = _T("Y1 HEIGHT LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y1 HEIGHT LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_y1_height1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y1_height1[1] = atof(mstr_temp);
		m_dgt_labelset_y1_height1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			

}

void CDialog_Work_Barcode_Print::OnDgtLabelsetX2Width1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x2_width1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	
	st_msg.mstr_keypad_msg = _T("X2 WIDTH LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X2 WIDTH LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_x2_width1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x2_width1[1] = atof(mstr_temp);
		m_dgt_labelset_x2_width1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetY2Height1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y2_height1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	st_msg.mstr_keypad_msg = _T("Y2 HEIGHT LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y2 HEIGHT LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_y2_height1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y2_height1[1] = atof(mstr_temp);
		m_dgt_labelset_y2_height1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetX3Width1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x3_width1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	
	st_msg.mstr_keypad_msg = _T("X3 WIDTH LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X3 WIDTH LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_x3_width1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x3_width1[1] = atof(mstr_temp);
		m_dgt_labelset_x3_width1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}				
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetY3Height1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y3_height1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	st_msg.mstr_keypad_msg = _T("Y3 HEIGHT LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y3 HEIGHT LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_y3_height1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y3_height1[1] = atof(mstr_temp);
		m_dgt_labelset_y3_height1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetX4Width1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x4_width1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	
	st_msg.mstr_keypad_msg = _T("X4 WIDTH LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X4 WIDTH LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_x4_width1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x4_width1[1] = atof(mstr_temp);
		m_dgt_labelset_x4_width1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}					
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetY4Height1() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y4_height1.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	st_msg.mstr_keypad_msg = _T("Y4 HEIGHT LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y4 HEIGHT LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = TRUE;
	
	CRect r;
	
	m_dgt_labelset_y4_height1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y4_height1[1] = atof(mstr_temp);
		m_dgt_labelset_y4_height1.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}

void CDialog_Work_Barcode_Print::OnBarcode_Label_Font_Set()
{
	//////////////////////////////////////////////////////// 20120531
	m_msg_labelset_sd1.SetFont(mp_label_font);
	m_msg_labelset_sd1.SetWindowText(_T("인쇄농도(SD)"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_sd1.SetWindowText(_T("Set Darkness(SD)"));
	}
	m_msg_labelset_sd1.SetCenterText();
	m_msg_labelset_sd1.SetColor(RGB(0,0,255));
	m_msg_labelset_sd1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_sd1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_lt1.SetFont(mp_label_font);
	m_msg_labelset_lt1.SetWindowText(_T("전체높이(LT)"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_lt1.SetWindowText(_T("Label Top(LT)"));
	}

	m_msg_labelset_lt1.SetCenterText();
	m_msg_labelset_lt1.SetColor(RGB(0,0,255));
	m_msg_labelset_lt1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_lt1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_lh1.SetFont(mp_label_font);
	m_msg_labelset_lh1.SetWindowText(_T("시작 X (LH)"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_lh1.SetWindowText(_T("Start X (LH)"));
	}

	m_msg_labelset_lh1.SetCenterText();
	m_msg_labelset_lh1.SetColor(RGB(0,0,255));
	m_msg_labelset_lh1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_lh1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_lh2.SetFont(mp_label_font);
	m_msg_labelset_lh2.SetWindowText(_T("시작 Y (LH)"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_lh2.SetWindowText(_T("Start Y (LH)"));
	}

	m_msg_labelset_lh2.SetCenterText();
	m_msg_labelset_lh2.SetColor(RGB(0,0,255));
	m_msg_labelset_lh2.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_lh2.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x1_pos1.SetFont(mp_label_font);
	m_msg_labelset_x1_pos1.SetWindowText(_T("1번 X값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_x1_pos1.SetWindowText(_T("X1"));
	}

	m_msg_labelset_x1_pos1.SetCenterText();
	m_msg_labelset_x1_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_x1_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x1_pos1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x2_pos1.SetFont(mp_label_font);
	m_msg_labelset_x2_pos1.SetWindowText(_T("2번 X값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_x2_pos1.SetWindowText(_T("X2"));
	}

	m_msg_labelset_x2_pos1.SetCenterText();
	m_msg_labelset_x2_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_x2_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x2_pos1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x3_pos1.SetFont(mp_label_font);
	m_msg_labelset_x3_pos1.SetWindowText(_T("3번 X값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_x3_pos1.SetWindowText(_T("X3"));
	}

	m_msg_labelset_x3_pos1.SetCenterText();
	m_msg_labelset_x3_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_x3_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x3_pos1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x4_pos1.SetFont(mp_label_font);
	m_msg_labelset_x4_pos1.SetWindowText(_T("4번 X값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_x4_pos1.SetWindowText(_T("X4"));
	}

	m_msg_labelset_x4_pos1.SetCenterText();
	m_msg_labelset_x4_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_x4_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x4_pos1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x5_pos1.SetFont(mp_label_font);
	m_msg_labelset_x5_pos1.SetWindowText(_T("5번 X값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_x5_pos1.SetWindowText(_T("X5"));
	}

	m_msg_labelset_x5_pos1.SetCenterText();
	m_msg_labelset_x5_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_x5_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x5_pos1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x6_pos1.SetFont(mp_label_font);
	m_msg_labelset_x6_pos1.SetWindowText(_T("6번 X값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_x6_pos1.SetWindowText(_T("X6"));
	}

	m_msg_labelset_x6_pos1.SetCenterText();
	m_msg_labelset_x6_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_x6_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x6_pos1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y1_pos1.SetFont(mp_label_font);
	m_msg_labelset_y1_pos1.SetWindowText(_T("1번 Y값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_y1_pos1.SetWindowText(_T("Y1"));
	}

	m_msg_labelset_y1_pos1.SetCenterText();
	m_msg_labelset_y1_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_y1_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y1_pos1.SetTextColor(RGB(255,255,255));
	
	m_msg_labelset_y2_pos1.SetFont(mp_label_font);
	m_msg_labelset_y2_pos1.SetWindowText(_T("2번 Y값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_y2_pos1.SetWindowText(_T("Y2"));
	}

	m_msg_labelset_y2_pos1.SetCenterText();
	m_msg_labelset_y2_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_y2_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y2_pos1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y3_pos1.SetFont(mp_label_font);
	m_msg_labelset_y3_pos1.SetWindowText(_T("3번 Y값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_y3_pos1.SetWindowText(_T("Y3"));
	}

	m_msg_labelset_y3_pos1.SetCenterText();
	m_msg_labelset_y3_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_y3_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y3_pos1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y4_pos1.SetFont(mp_label_font);
	m_msg_labelset_y4_pos1.SetWindowText(_T("4번 Y값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_y4_pos1.SetWindowText(_T("Y4"));
	}

	m_msg_labelset_y4_pos1.SetCenterText();
	m_msg_labelset_y4_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_y4_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y4_pos1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y5_pos1.SetFont(mp_label_font);
	m_msg_labelset_y5_pos1.SetWindowText(_T("5번 Y값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_y5_pos1.SetWindowText(_T("Y5"));
	}

	m_msg_labelset_y5_pos1.SetCenterText();
	m_msg_labelset_y5_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_y5_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y5_pos1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y6_pos1.SetFont(mp_label_font);
	m_msg_labelset_y6_pos1.SetWindowText(_T("6번 Y값"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_y6_pos1.SetWindowText(_T("Y6"));
	}

	m_msg_labelset_y6_pos1.SetCenterText();
	m_msg_labelset_y6_pos1.SetColor(RGB(0,0,255));
	m_msg_labelset_y6_pos1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y6_pos1.SetTextColor(RGB(255,255,255));
	

	m_msg_labelset_x1_width1.SetFont(mp_label_font);
	m_msg_labelset_x1_width1.SetWindowText(_T("1번 넓이"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_x1_width1.SetWindowText(_T("Width 1"));
	}

	m_msg_labelset_x1_width1.SetCenterText();
	m_msg_labelset_x1_width1.SetColor(RGB(0,0,255));
	m_msg_labelset_x1_width1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x1_width1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x2_width1.SetFont(mp_label_font);
	m_msg_labelset_x2_width1.SetWindowText(_T("2번 넓이"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_x2_width1.SetWindowText(_T("Width 2"));
	}

	m_msg_labelset_x2_width1.SetCenterText();
	m_msg_labelset_x2_width1.SetColor(RGB(0,0,255));
	m_msg_labelset_x2_width1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x2_width1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x3_width1.SetFont(mp_label_font);
	m_msg_labelset_x3_width1.SetWindowText(_T("3번 넓이"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_x3_width1.SetWindowText(_T("Width 3"));
	}

	m_msg_labelset_x3_width1.SetCenterText();
	m_msg_labelset_x3_width1.SetColor(RGB(0,0,255));
	m_msg_labelset_x3_width1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x3_width1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x4_width1.SetFont(mp_label_font);
	m_msg_labelset_x4_width1.SetWindowText(_T("4번 넓이"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_x4_width1.SetWindowText(_T("Width 4"));
	}

	m_msg_labelset_x4_width1.SetCenterText();
	m_msg_labelset_x4_width1.SetColor(RGB(0,0,255));
	m_msg_labelset_x4_width1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x4_width1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y1_height1.SetFont(mp_label_font);
	m_msg_labelset_y1_height1.SetWindowText(_T("1번 높이"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_y1_height1.SetWindowText(_T("Hight 1"));
	}

	m_msg_labelset_y1_height1.SetCenterText();
	m_msg_labelset_y1_height1.SetColor(RGB(0,0,255));
	m_msg_labelset_y1_height1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y1_height1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y2_height1.SetFont(mp_label_font);
	m_msg_labelset_y2_height1.SetWindowText(_T("2번 높이"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_y2_height1.SetWindowText(_T("Hight 2"));
	}

	m_msg_labelset_y2_height1.SetCenterText();
	m_msg_labelset_y2_height1.SetColor(RGB(0,0,255));
	m_msg_labelset_y2_height1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y2_height1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y3_height1.SetFont(mp_label_font);
	m_msg_labelset_y3_height1.SetWindowText(_T("3번 높이"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_y3_height1.SetWindowText(_T("Hight 3"));
	}

	m_msg_labelset_y3_height1.SetCenterText();
	m_msg_labelset_y3_height1.SetColor(RGB(0,0,255));
	m_msg_labelset_y3_height1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y3_height1.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y4_height1.SetFont(mp_label_font);
	m_msg_labelset_y4_height1.SetWindowText(_T("4번 높이"));
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_msg_labelset_y4_height1.SetWindowText(_T("Hight 4"));
	}

	m_msg_labelset_y4_height1.SetCenterText();
	m_msg_labelset_y4_height1.SetColor(RGB(0,0,255));
	m_msg_labelset_y4_height1.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y4_height1.SetTextColor(RGB(255,255,255));

	///////////////////////////////////////////////////
}

void CDialog_Work_Barcode_Print::OnBarcode_Label_Data_Display()
{
	m_dgt_labelset_sd1.SetValue(mn_labelset_sd1[1]);
	m_dgt_labelset_lt1.SetValue(mn_labelset_lt1[1]);
	m_dgt_labelset_lh1.SetValue(mn_labelset_lh1[1]);
	m_dgt_labelset_lh2.SetValue(mn_labelset_lh2[1]);
	///////////20120906

	m_dgt_labelset_x1_pos1.SetFloatVal((float)md_labelset_x1_pos1[1]);
	m_dgt_labelset_x2_pos1.SetFloatVal((float)md_labelset_x2_pos1[1]);
	m_dgt_labelset_x3_pos1.SetFloatVal((float)md_labelset_x3_pos1[1]);
	m_dgt_labelset_x4_pos1.SetFloatVal((float)md_labelset_x4_pos1[1]);
	m_dgt_labelset_x5_pos1.SetFloatVal((float)md_labelset_x5_pos1[1]);
	m_dgt_labelset_x6_pos1.SetFloatVal((float)md_labelset_x6_pos1[1]);
	m_dgt_labelset_y1_pos1.SetFloatVal((float)md_labelset_y1_pos1[1]);
	m_dgt_labelset_y2_pos1.SetFloatVal((float)md_labelset_y2_pos1[1]);
	m_dgt_labelset_y3_pos1.SetFloatVal((float)md_labelset_y3_pos1[1]);
	m_dgt_labelset_y4_pos1.SetFloatVal((float)md_labelset_y4_pos1[1]);
	m_dgt_labelset_y5_pos1.SetFloatVal((float)md_labelset_y5_pos1[1]);
	m_dgt_labelset_y6_pos1.SetFloatVal((float)md_labelset_y6_pos1[1]);
	m_dgt_labelset_x1_width1.SetFloatVal((float)md_labelset_x1_width1[1]);
	m_dgt_labelset_x2_width1.SetFloatVal((float)md_labelset_x2_width1[1]);
	m_dgt_labelset_x3_width1.SetFloatVal((float)md_labelset_x3_width1[1]);
	m_dgt_labelset_x4_width1.SetFloatVal((float)md_labelset_x4_width1[1]);
	m_dgt_labelset_y1_height1.SetFloatVal((float)md_labelset_y1_height1[1]);
	m_dgt_labelset_y2_height1.SetFloatVal((float)md_labelset_y2_height1[1]);
	m_dgt_labelset_y3_height1.SetFloatVal((float)md_labelset_y3_height1[1]);
	m_dgt_labelset_y4_height1.SetFloatVal((float)md_labelset_y4_height1[1]);
	///////////

	m_dgt_labelset_func_paper.SetValue(mn_labelset_func_paper[1]);	//20120625

}

void CDialog_Work_Barcode_Print::OnBarcode_Label_DeviceData_Apply()
{
	int n_response = 0;  // 대화 상자 리턴 플래그
	int n_check;
	CString str_msg;
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	
	n_check = OnBarcode_Label_Input_Data_Check();  // 입력 정보 정상 유무 검사 함수
	
	if (n_check == TRUE)  
	{
		str_msg = "[Test Label ] Was changed by " + mstr_label_name1[1];
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
	else if (n_check == FALSE)  
	{
		return ;
	}
	
	// **********************************************************************
	// 디바이스 관련 화면 설정 데이터 로딩 부분                              
	// **********************************************************************
	mstr_temp.Format("%d", st_basic.n_mode_label);
	:: WritePrivateProfileString("BasicData", "Label_Mode", LPCTSTR(mstr_temp), st_path.mstr_basic);
	
	mstr_temp.Format("%s", st_basic.mstr_label_name);	
	:: WritePrivateProfileString("BasicData", "Label_Type", LPCTSTR(mstr_temp), st_path.mstr_basic);
// 	if (b_create_list == TRUE)
// 	{
		mcls_barcode.OnBarcode_Label_Data_Save();		
// 	}
	mcls_barcode.OnBarcode_Label_Data_Load();
	// **********************************************************************
		
	// **************************************************************************
	// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다                        
	// **************************************************************************
	m_list_label_type1.ResetContent();			// 리스트 박스의 모든 목록 삭제
// 	mp_device_image_list.DeleteImageList();		// 이미지 리스트에 설정된 정보 삭제
	
	//OnBasic_Create_ImageList();					// 이미지 리스트 생성 함수
	
	// Associate image list to list box
	//m_list_label_type.SetImageList(&mp_device_image_list);
	
	OnBarcode_Label_Init_List(st_path.mstr_label_folder);	// 파일 폴더 초기화 함수
	
	OnBarcode_Label_Focus_Set();					// 선택된 디바이스명 포커스 설정 함수
	// **************************************************************************

	st_basic.mstr_label_name =	mstr_label_name1[1];
	mstr_label_name1[0] =		mstr_label_name1[1];
	st_basic.n_mode_label = mn_label_mode1[1];
	mn_label_mode1[0] =		mn_label_mode1[1];
	
}

int CDialog_Work_Barcode_Print::OnBarcode_Label_Input_Data_Check()
{
	CString str_chk_ext;   // 확장자 저장 변수
	int n_pos;
	
	/* ************************************************************************** */
    /* 입력된 디바이스명 설정한다                                                 */
    /* ************************************************************************** */
// 	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_temp_label) ;
// 	mstr_temp_label.MakeUpper();
// 	mstr_temp_label.TrimLeft(' ');               
// 	mstr_temp_label.TrimRight(' ');
	mstr_temp_label1 = mstr_label_name1[1];
	if (mstr_temp_label1.IsEmpty())  
	{
		return FALSE;
	}
	/* ************************************************************************** */
	
	n_pos = mstr_temp_label1.Find(".");				// 확장자 검사
	if (n_pos == -1) 
		mstr_temp_label1 += _T(".TXT");
	else 
	{
		str_chk_ext = mstr_temp_label1.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			return FALSE;
		}
	}
	
	mstr_label_name1[1] = mstr_temp_label1;			// 디바이스 파일명 설정
	/* ************************************************************************** */
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////// 20120530
int CDialog_Work_Barcode_Print::OnBarcode_Label_Init_List(LPCTSTR pszPath)
{
	int n_chk;						// 함수 리턴 플래그
    CString str_path = pszPath;		// 폴더 설정
	
	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// 리스트 박스에 추가한 목록 갯수 저장 변수

	mn_label_name1 = -1;
	
	if (str_path.Right (1) != "\\")
        str_path += "\\";
	
	str_path += "*.*";
    
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			n_chk = Label_AddItem(n_count, &fd);
			if (n_chk == TRUE)
				n_count++;
        }
		
        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				n_chk = Label_AddItem(n_count, &fd);
				if (n_chk == FALSE) 
					break;
			}
			if (n_chk != CTL_READY)
				n_count++;
        }
		::FindClose(hFind);
    }
	
	return n_count;
}

////////////////////////////////////////////////// 20120530
BOOL CDialog_Work_Barcode_Print::Label_AddItem(int nIndex, WIN32_FIND_DATA *pfd)
{
	CString str_temp_lbl;
	CString str_label;
	CString str_expansion;
	int n_pos;
	
	HICON	hIcon = NULL;
	
	str_temp_lbl = (pfd->cFileName);	// 파일 이름 설정
	
	str_temp_lbl.MakeUpper();
	
	// **************************************************************************
	// 파일 이름 중 확장자 확인
	// **************************************************************************
	n_pos = str_temp_lbl.Find('.');		// 확장자 위치 설정
	
	if (n_pos == -1)					// 확장자 미존재
		str_expansion = "";
	else								// 확장자 존재
		str_expansion = str_temp_lbl.Mid(n_pos + 1);
	// **************************************************************************
	
	if (str_expansion != "TXT")
	{
		return CTL_READY;	
	}
	
	str_label=str_temp_lbl;			// 파일명 전체 화면 출력
	
	if (str_label == mstr_label_name1[1])
		mn_label_name1 = nIndex-1;
	// **************************************************************************
	
// 	hIcon = AfxGetApp()->LoadIcon(IDI_MODULE);
// 	mp_device_image_list.Add(hIcon);
	
    if (m_list_label_type1.AddString(_T(str_label), nIndex - 1) == -1)
		return FALSE;
	
    return TRUE;
}

///////////////////////////////////////////////// 20120530
void CDialog_Work_Barcode_Print::OnBarcode_Label_Focus_Set()
{
	if (mn_label_name1 == -1)
		return;
	
	m_list_label_type1.SetCurSel(mn_label_name1);
	
}
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////

void CDialog_Work_Barcode_Print::OnBtnLabelsetMove() 
{
	// TODO: Add your control notification handler code here
	//1번 위치값 읽기 
	int x1_pos, y1_pos;


	m_msg_preview.GetWindowRect(modLabelPos);
	ScreenToClient(modLabelPos);
	m_nmodLabelPosTop = modLabelPos.top;
	m_nmodLabelPosLeft = modLabelPos.left;


	x1_pos = md_labelset_x1_pos1[1] - md_labelset_x1_pos1[0];// / (m_nmodLabelPosLeft - m_nmodLabelTestLeft);
	y1_pos = md_labelset_y1_pos1[1] - md_labelset_y1_pos1[0];// / (m_nmodLabelPosTop - m_nmodLabelTestTop);

	md_labelset_x1_pos1[0] = md_labelset_x1_pos1[1];//20120906
	md_labelset_y1_pos1[0] = md_labelset_y1_pos1[1];

	m_nmodLabelPosLeft = m_nmodLabelPosLeft + x1_pos;
	m_nmodLabelPosTop = m_nmodLabelPosTop + y1_pos;

	GetDlgItem(IDC_MSG_PREVIEW)->MoveWindow(m_nmodLabelPosLeft, m_nmodLabelPosTop, modLabelPos.Width(), modLabelPos.Height(), TRUE);			
}
void CDialog_Work_Barcode_Print::OnBtnLabelsetMove2() 
{
	// TODO: Add your control notification handler code here
	//2번 위치값 읽기 
	int x1_pos, y1_pos;

	m_msg_preview2.GetWindowRect(modLabelPos2);
	ScreenToClient(modLabelPos2);
	m_nmodLabelPosTop2 = modLabelPos2.top;
	m_nmodLabelPosLeft2 = modLabelPos2.left;


	x1_pos = md_labelset_x2_pos1[1] - md_labelset_x2_pos1[0];// / (m_nmodLabelPosLeft - m_nmodLabelTestLeft);
	y1_pos = md_labelset_y2_pos1[1] - md_labelset_y2_pos1[0];// / (m_nmodLabelPosTop - m_nmodLabelTestTop);

	md_labelset_x2_pos1[0] = md_labelset_x2_pos1[1];//20120906
	md_labelset_y2_pos1[0] = md_labelset_y2_pos1[1];

	m_nmodLabelPosLeft2 = m_nmodLabelPosLeft2 + x1_pos;
	m_nmodLabelPosTop2 = m_nmodLabelPosTop2 + y1_pos;

	GetDlgItem(IDC_MSG_PREVIEW2)->MoveWindow(m_nmodLabelPosLeft2, m_nmodLabelPosTop2, modLabelPos2.Width(), modLabelPos2.Height(), TRUE);			
	
}
void CDialog_Work_Barcode_Print::OnBtnLabelsetMove3() 
{
	// TODO: Add your control notification handler code here
	//3번 위치값 읽기 
	int x1_pos, y1_pos;

	m_msg_preview3.GetWindowRect(modLabelPos3);
	ScreenToClient(modLabelPos3);
	m_nmodLabelPosTop3 = modLabelPos3.top;
	m_nmodLabelPosLeft3 = modLabelPos3.left;


	x1_pos = md_labelset_x3_pos1[1] - md_labelset_x3_pos1[0];// / (m_nmodLabelPosLeft - m_nmodLabelTestLeft);
	y1_pos = md_labelset_y3_pos1[1] - md_labelset_y3_pos1[0];// / (m_nmodLabelPosTop - m_nmodLabelTestTop);

	md_labelset_x3_pos1[0] = md_labelset_x3_pos1[1];//20120906
	md_labelset_y3_pos1[0] = md_labelset_y3_pos1[1];

	m_nmodLabelPosLeft3 = m_nmodLabelPosLeft3 + x1_pos;
	m_nmodLabelPosTop3 = m_nmodLabelPosTop3 + y1_pos;

	GetDlgItem(IDC_MSG_PREVIEW3)->MoveWindow(m_nmodLabelPosLeft3, m_nmodLabelPosTop3, modLabelPos3.Width(), modLabelPos3.Height(), TRUE);			
	
}
void CDialog_Work_Barcode_Print::OnBtnLabelsetMove4() 
{
	// TODO: Add your control notification handler code here
	//4번 위치값 읽기 
	int x1_pos, y1_pos;

	m_static_label_view4.GetWindowRect(modLabelPos4);
	ScreenToClient(modLabelPos4);
	m_nmodLabelPosTop4 = modLabelPos4.top;
	m_nmodLabelPosLeft4 = modLabelPos4.left;


	x1_pos = md_labelset_x4_pos1[1] - md_labelset_x4_pos1[0];// / (m_nmodLabelPosLeft - m_nmodLabelTestLeft);
	y1_pos = md_labelset_y4_pos1[1] - md_labelset_y4_pos1[0];// / (m_nmodLabelPosTop - m_nmodLabelTestTop);

	md_labelset_x4_pos1[0] = md_labelset_x4_pos1[1];//20120906
	md_labelset_y4_pos1[0] = md_labelset_y4_pos1[1];

	m_nmodLabelPosLeft4 = m_nmodLabelPosLeft4 + x1_pos;
	m_nmodLabelPosTop4 = m_nmodLabelPosTop4 + y1_pos;

	GetDlgItem(IDC_STATIC_LABEL_VIEW4)->MoveWindow(m_nmodLabelPosLeft4, m_nmodLabelPosTop4, modLabelPos4.Width(), modLabelPos4.Height(), TRUE);			
	
}
void CDialog_Work_Barcode_Print::OnBtnLabelsetMove5() 
{
	// TODO: Add your control notification handler code here
	//5번 위치값 읽기 
	int x1_pos, y1_pos;

	m_static_label_view2.GetWindowRect(modLabelPos5);
	ScreenToClient(modLabelPos5);
	m_nmodLabelPosTop5 = modLabelPos5.top;
	m_nmodLabelPosLeft5 = modLabelPos5.left;


	x1_pos = md_labelset_x5_pos1[1] - md_labelset_x5_pos1[0];// / (m_nmodLabelPosLeft - m_nmodLabelTestLeft);
	y1_pos = md_labelset_y5_pos1[1] - md_labelset_y5_pos1[0];// / (m_nmodLabelPosTop - m_nmodLabelTestTop);

	md_labelset_x5_pos1[0] = md_labelset_x5_pos1[1];//20120906
	md_labelset_y5_pos1[0] = md_labelset_y5_pos1[1];

	m_nmodLabelPosLeft5 = m_nmodLabelPosLeft5 + x1_pos;
	m_nmodLabelPosTop5 = m_nmodLabelPosTop5 + y1_pos;

	GetDlgItem(IDC_STATIC_LABEL_VIEW2)->MoveWindow(m_nmodLabelPosLeft5, m_nmodLabelPosTop5, modLabelPos5.Width(), modLabelPos5.Height(), TRUE);			
	
}
void CDialog_Work_Barcode_Print::OnBtnLabelsetMove6() 
{
	// TODO: Add your control notification handler code here
	//6번 위치값 읽기 
	int x1_pos, y1_pos;

	m_static_label_view3.GetWindowRect(modLabelPos6);
	ScreenToClient(modLabelPos6);
	m_nmodLabelPosTop6 = modLabelPos6.top;
	m_nmodLabelPosLeft6 = modLabelPos6.left;


	x1_pos = md_labelset_x6_pos1[1] - md_labelset_x6_pos1[0];// / (m_nmodLabelPosLeft - m_nmodLabelTestLeft);
	y1_pos = md_labelset_y6_pos1[1] - md_labelset_y6_pos1[0];// / (m_nmodLabelPosTop - m_nmodLabelTestTop);

	md_labelset_x6_pos1[0] = md_labelset_x6_pos1[1];//20120906
	md_labelset_y6_pos1[0] = md_labelset_y6_pos1[1];

	m_nmodLabelPosLeft6 = m_nmodLabelPosLeft6 + x1_pos;
	m_nmodLabelPosTop6 = m_nmodLabelPosTop6 + y1_pos;

	GetDlgItem(IDC_STATIC_LABEL_VIEW3)->MoveWindow(m_nmodLabelPosLeft6, m_nmodLabelPosTop6, modLabelPos6.Width(), modLabelPos6.Height(), TRUE);				
}

void CDialog_Work_Barcode_Print::OnBtnLabelsetSize() 
{
	// TODO: Add your control notification handler code here
	
	//1번 위치값 읽기 
	int x1_pos, y1_pos;

	m_static_label_view.GetWindowRect(modLabelSize);
	ScreenToClient(modLabelSize);
	m_nmodLabelSizeWidth = modLabelSize.Width();
	m_nmodLabelSizeHeight = modLabelSize.Height();


	x1_pos = md_labelset_x1_width1[1] / 17;//- mn_labelset_x1_width1[0];// / (m_nmodLabelPosLeft - m_nmodLabelTestLeft);//20120906
	y1_pos = md_labelset_y1_height1[1] / 1.1;//- mn_labelset_y1_height1[0];// / (m_nmodLabelPosTop - m_nmodLabelTestTop);

	if(mp_label_print_font1 != NULL)
	{
		/////////////////////////// 20120601
		delete mp_label_print_font1;
		mp_label_print_font1 = NULL;
		///////////////////////////
	}

 	mp_label_print_font1 = new CFont;
	mp_label_print_font1->CreateFont(y1_pos,x1_pos,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	Init_Label();
//	mp_label_print_font1->CreateFont(18,3,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

}

void CDialog_Work_Barcode_Print::OnBtnLabelsetSize2() 
{
	// TODO: Add your control notification handler code here
	//2번 위치값 읽기 
	int x1_pos, y1_pos;

	m_static_label_view2.GetWindowRect(modLabelSize2);
	ScreenToClient(modLabelSize2);
	m_nmodLabelSizeWidth2 = modLabelSize2.Width();
	m_nmodLabelSizeHeight2 = modLabelSize2.Height();


	x1_pos = md_labelset_x2_width1[1] / 10.5;//- mn_labelset_x1_width1[0];// / (m_nmodLabelPosLeft - m_nmodLabelTestLeft);//20120906
	y1_pos = md_labelset_y2_height1[1] / 3.8;//- mn_labelset_y1_height1[0];// / (m_nmodLabelPosTop - m_nmodLabelTestTop);

	if(mp_label_print_font != NULL)
	{
		/////////////////////////// 20120601
		delete mp_label_print_font;
		mp_label_print_font = NULL;
		///////////////////////////
	}

 	mp_label_print_font = new CFont;
	mp_label_print_font->CreateFont(y1_pos,x1_pos,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	Init_Label();
//	mp_label_print_font1->CreateFont(10,4,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	
}

void CDialog_Work_Barcode_Print::OnBtnLabelsetSize3() 
{
	// TODO: Add your control notification handler code here
	//2번 위치값 읽기 
	int x1_pos, y1_pos;

	m_static_label_view3.GetWindowRect(modLabelSize3);
	ScreenToClient(modLabelSize3);
	m_nmodLabelSizeWidth3 = modLabelSize3.Width();
	m_nmodLabelSizeHeight3 = modLabelSize3.Height();


	x1_pos = md_labelset_x3_width1[1] / 9.1;//- mn_labelset_x1_width1[0];// / (m_nmodLabelPosLeft - m_nmodLabelTestLeft);//20120906
	y1_pos = md_labelset_y3_height1[1] / 2.5;//- mn_labelset_y1_height1[0];// / (m_nmodLabelPosTop - m_nmodLabelTestTop);

	if(mp_label_font != NULL)
	{
		/////////////////////////// 20120601
		delete mp_label_font;
		mp_label_font = NULL;
		///////////////////////////
	}

 	mp_label_font = new CFont;
	mp_label_font->CreateFont(y1_pos,x1_pos,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	Init_Label();
//	mp_label_font->CreateFont(18,6,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	
}

void CDialog_Work_Barcode_Print::OnBtnLabelsetSize4() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialog_Work_Barcode_Print::OnBtnLabelApply() 
{
	// TODO: Add your control notification handler code here
	CString str_selected_lbl;
	int nLabelCur = m_list_label_type1.GetCurSel();

	if( nLabelCur != mn_label_name1 )
	{
		if( g_AMTLotMgr.CheckChangeableDIMM() == false )
		{
			st_msg.mstr_event_msg[0] = "Can't Change Label Type for Working";
			::SendMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
			return;
		}
	}

	int n_response;  // 대화 상자 리턴 플래그
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Label Data Apply?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnBarcode_Label_Data_Apply();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		
		OnBarcode_LogFile_Create();		// Log File 생성.

		OnBarcode_Label_Data_Display();

		mcls_barcode.OnBarcode_Label_Data_Save();	
		mcls_barcode.OnBarcode_Label_Data_Load();

				
	}
	else if (n_response == IDCANCEL)
	{
		
	}	
}

void CDialog_Work_Barcode_Print::OnBtnLabelCancle() 
{
	// TODO: Add your control notification handler code here
	int n_response;  // 대화 상자 리턴 플래그
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Previous Data Recovery?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnBarcode_Label_Data_Recovery();			// 베이식 화면 셋팅 정보 백업 받아놓은 변수로 복구 함수
		OnBarcode_Label_Data_Display();
	}
	else if (n_response == IDCANCEL)
	{
		
	}		
}


void CDialog_Work_Barcode_Print::OnBtnLabelsetDataInput() 
{
	CString str_input_data;
	// TODO: Add your control notification handler code here
	st_barcode.b_Maunal_Input = TRUE;//20120913
	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA))->GetWindowText(str_input_data);
	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA))->GetWindowText(st_basic.mstr_barcode_data[0]);
	m_msg_preview.SetWindowText(_T(str_input_data));

// 	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA))->SetWindowText(_T(""));

}

void CDialog_Work_Barcode_Print::OnBtnLabelsetDataInput2() 
{
	CString str_input_data;
	// TODO: Add your control notification handler code here
	st_barcode.b_Maunal_Input = TRUE;//20120913
	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA2))->GetWindowText(str_input_data);
	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA2))->GetWindowText(st_basic.mstr_barcode_data[1]);

	m_msg_preview2.SetWindowText(_T(str_input_data));
// 	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA2))->SetWindowText(_T(""));

}

void CDialog_Work_Barcode_Print::OnBtnLabelsetDataInput3() 
{
	CString str_input_data;

	// TODO: Add your control notification handler code here
	st_barcode.b_Maunal_Input = TRUE;//20120913
	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA3))->GetWindowText(str_input_data);
	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA3))->GetWindowText(st_basic.mstr_barcode_data[2]);

	m_msg_preview3.SetWindowText(_T(str_input_data));
// 	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA3))->SetWindowText(_T(""));

}

void CDialog_Work_Barcode_Print::OnBtnLabelsetDataInput4() 
{
	CString str_input_data;

	// TODO: Add your control notification handler code here
	st_barcode.b_Maunal_Input = TRUE;//20120913
	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA4))->GetWindowText(str_input_data);
	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA4))->GetWindowText(st_basic.mstr_barcode_data[3]);

	m_msg_preview4.SetWindowText(_T(str_input_data));
// 	((CEdit*)GetDlgItem(IDC_EDIT_LABELSET_DATA4))->SetWindowText(_T(""));

}

/////////////////// 20120620
void CDialog_Work_Barcode_Print::OnBtnBarcodeCreate2() 
{
	// TODO: Add your control notification handler code here

	m_b_stop_req = TRUE;
	Func.Label_Printer_Status();	
	
}
/////////////////// 

void CDialog_Work_Barcode_Print::OnBtnBcr() 
{
	// TODO: Add your control notification handler code here
//	BUttiEnale();
//	Cleatstep();
//	SetTimer(TMR_vcr, 100,  0)
	
}

//void CDialog_Work_Barcode_Print:;Cleatstep
//{
//	bcrstep = 0;
//}
//
//int CDialog_Work_Barcode_Print::TestBcr()
//{
//	if(m_r_stop_req == TRUE)
//	{
//		bFuncRet = RET_ERROR;
//	}
//
//	int bFuncRet = RET_PROCEED;
//	switch(bcrstep)
//	{
//	case 0:
//		FAS_IO.set_out_bit(츠리거0)
//			Until_Wait_Time[0] = GetCurrerntim;
//
//		break;
//
//	case 100:
//		Until_Wait_Time[1] = GetCurrerntim();
//
//		if(Until_Wait_Time[2] > 1000)
//		{
//			//bFuncRet = RET_ERROR;'
//		}
//		if(FAS_IO.get_in_bit(출력))
//		{
//			bcrstep = 200;
//		}
//		break;
//
//		case 200:
//			bFuncRet = RET_GOOD
//				sdtep = 0
//				break
//
//	}
//
//	return bFuncRet
//}

void CDialog_Work_Barcode_Print::OnButton10() 
{
//	m_r_stop_req = TRUE;
}

///////////////////////////////////////////////////////////////// 20120625
void CDialog_Work_Barcode_Print::OnBtnBarcodeFunc() 
{
	// TODO: Add your control notification handler code here
	Func.Pause_Btn_On();	
}

void CDialog_Work_Barcode_Print::OnBtnBarcodeFunc2() 
{
	// TODO: Add your control notification handler code here
	Func.Pause_Btn_Off();	
}

void CDialog_Work_Barcode_Print::OnBtnBarcodeFunc3() 
{
	// TODO: Add your control notification handler code here
	Func.Single_Paper_Print();		
}

void CDialog_Work_Barcode_Print::OnBtnBarcodeFunc4() 
{
	// TODO: Add your control notification handler code here
	Func.Cancel_Btn();		
}

void CDialog_Work_Barcode_Print::OnBtnBarcodeFunc5() 
{
	// TODO: Add your control notification handler code here
	Func.Multi_Paper_Print(mn_labelset_func_paper[1]);
}



LRESULT CDialog_Work_Barcode_Print::OnWork_Barcode_Info_Display(WPARAM wParam,LPARAM lParam) 
{
	int mn_state = wParam;
	
	switch(mn_state)
	{	
	case BARCODE_HS_DISPLAY:
		OnBARCODE_HS_DISPLAY(lParam);
		break;

	default:
		break;
	}

	return 0;
}

////2014,0411
void CDialog_Work_Barcode_Print::OnBARCODE_CREATE_BUTTON(int n_state)
{
	switch(n_state)
	{
		case 0:
			m_btn_barcode_create.EnableWindow(FALSE);
			break;
		case 1:
			m_btn_barcode_create.EnableWindow(TRUE);
			break;
	}
}
////

void CDialog_Work_Barcode_Print::OnBARCODE_HS_DISPLAY(int n_state)
{
	CString str_yield, str_yield2, str_yield3, str_yield4;
	CString str_ml, str_ml2, str_ml3, str_ml4;	// 20120618

	switch(n_state)
	{
	case 0:
		((CEdit*)GetDlgItem(IDC_EDIT_BARCODE_CREATE))->SetWindowText(st_handler.str_printer_hs_list);
	case 1:
		((CEdit*)GetDlgItem(IDC_EDIT_BARCODE_CREATE2))->SetWindowText(st_handler.str_printer_hs_list2);
	case 2:
		((CEdit*)GetDlgItem(IDC_EDIT_BARCODE_CREATE3))->SetWindowText(st_handler.str_printer_hs_list3);

		
	}
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetFuncPaper() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_func_paper.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("PAPER OUT SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("PAPER OUT SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "20";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_func_paper.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_func_paper[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_func_paper.SetValue(mn_labelset_func_paper[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}				
}

void CDialog_Work_Barcode_Print::OnBtnBarcodeCreate3() 
{
	// TODO: Add your control notification handler code here
	int n_response;  // 대화 상자 리턴 플래그

	CDialog_Work_Barcode_Info Barcode_info_dlg;

	n_response = Barcode_info_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		
	}

}
///////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////// 
void CDialog_Work_Barcode_Print::OnRadioPrtCompReplyUse() 
{
	// TODO: Add your control notification handler code here
	
	m_radio_prt_comp_reply_use.Depress(true);
	m_radio_prt_comp_reply_notuse.Depress(false);
	
	mn_Prt_Comp_Reply[1] = 0;

	Func.Print_Complete_Set();
	
}

void CDialog_Work_Barcode_Print::OnRadioPrtCompReplyNotuse() 
{
	// TODO: Add your control notification handler code here

	m_radio_prt_comp_reply_use.Depress(false);
	m_radio_prt_comp_reply_notuse.Depress(true);

	mn_Prt_Comp_Reply[1] = 1; 

	Func.Print_Complete_Reset();
	
}

void CDialog_Work_Barcode_Print::OnRadioPrtMode() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_tear_off_mode.Depress(true);
	m_radio_prt_peal_off_mode.Depress(false);
	m_radio_prt_rewind_mode.Depress(false);

	mn_Prt_Mode[1] = 0;

	Func.Tear_Off_Mode();	
	
}

void CDialog_Work_Barcode_Print::OnRadioPrtMode2() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_tear_off_mode.Depress(false);
	m_radio_prt_peal_off_mode.Depress(true);
	m_radio_prt_rewind_mode.Depress(false);

	mn_Prt_Mode[1] = 1;

	Func.Peel_Off_Mode();
	
}

void CDialog_Work_Barcode_Print::OnRadioPrtMode3() 
{
	// TODO: Add your control notification handler code here
	m_radio_prt_tear_off_mode.Depress(false);
	m_radio_prt_peal_off_mode.Depress(false);
	m_radio_prt_rewind_mode.Depress(true);

	mn_Prt_Mode[1] = 2;

	Func.Rewind_Mode();
	
}

void CDialog_Work_Barcode_Print::LogFile_Create()
{
	CString str_msg;
	
	switch (st_barcode.n_Prt_Comp_Reply)
	{
	case 0:
		str_msg = "[Label Print Complete Reply] Was changed by With Device.";
		break;
	case 1:
		str_msg = "[Label Print Complete Reply] Was changed by WithOut Device.";
		break;
	}
	Func.On_LogFile_Add(0, str_msg);
	Func.On_LogFile_Add(99, str_msg);


	switch (st_barcode.n_Prt_mode)
	{
	case 0:
		str_msg = "[Tear off Mode] Was changed by With Device.";
		break;
	case 1:
		str_msg = "[Peal off Mode] Was changed by With Device.";
		break;
	case 2:
		str_msg = "[Rewind Mode] Was changed by With Device.";
		break;

	}
	Func.On_LogFile_Add(0, str_msg);
	Func.On_LogFile_Add(99, str_msg);

}


void CDialog_Work_Barcode_Print::OnDgtLabelsetLh1() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lh1.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LH X LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("LH X LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "40";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lh1.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lh1[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lh1.SetValue(mn_labelset_lh1[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}					
}

void CDialog_Work_Barcode_Print::OnDgtLabelsetLh2() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lh2.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LH Y LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("LH Y LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "40";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lh2.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lh2[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lh2.SetValue(mn_labelset_lh2[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}						
}

void CDialog_Work_Barcode_Print::OnBtnBarcodePrintReset() 
{
	int n_response;

	CDialog_Select select_dlg;
	st_msg.mstr_confirm_msg = _T("바코드 비젼 Reset 설정을 하시겠습니까?");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Do you want to set the barcode Reset Vision");
	}

	
	n_response = select_dlg.DoModal();
	if(n_response == IDOK)
	{
		st_handler.n_vision_reset = YES;
	}	
}

void CDialog_Work_Barcode_Print::OnBtnPrintCalibrate() //20120920
{
	// TODO: Add your control notification handler code here
	Func.Print_Calibrate_Btn();	
}

int CDialog_Work_Barcode_Print::OnBarcodeRead_Excution()
{
	int FuncRet = RET_PROCEED;

	int nRet_1;

	CString str_serial,str_partname;

	switch (n_barcoderead_step)
	{
	case 0:
		// **********************************************************************
		//  서버와의 통신전에 접속 상태를 확인한다.
		// **********************************************************************
		if(COMI.mn_simulation_mode == 1) // 20130509
		{ 
			n_barcoderead_step = 10000;
			break;
		}

		if (st_client[NETWORK_CLIENT_1].n_connect == NO)		// 접속중이 아니면 접속을 시도한다.
		{
			::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
			NetworkConnectTime[0] = GetCurrentTime();
			n_barcoderead_step = 10;
		}
		else													// 접속중이면...
		{
			n_barcoderead_step = 100;
		}
		break;

	case 10:
		// **********************************************************************
		//  접속을 확인한다.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != YES)
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 10000)	// 10초 동안 연결이 되지 않으면...
			{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_CONNECT, NETWORK_CLIENT_1);
				
				n_barcoderead_step = 5000;		
				
				sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] 연결이 되지 않습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] is not connected.");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
				//FuncRet = RET_ERROR;
			}
		}
		else											// 연결이 되었다.
		{
			n_barcoderead_step = 100;
		}
		break;

	case 100:
		// **********************************************************************
		//  시작을 알린다.
		// **********************************************************************
		{
			sprintf(st_client[NETWORK_CLIENT_1].ch_send,"%s",Func.OnNetworkDataMake(NW_LABEL_INFO));

			//if (Ret == RET_GOOD)
			//{
				::PostMessage(st_handler.hWnd, WM_CLIENT_MSG_1, CLIENT_SEND, NETWORK_CLIENT_1);	// 메인 프레임으로 메시지 전송
				NetworkConnectTime[0] = GetCurrentTime();
				n_barcoderead_step = 200;
			//}
		}
		break;

	case 200:
		// **********************************************************************
		//  완료가 될때까지 대기.
		// **********************************************************************
		if (st_client[NETWORK_CLIENT_1].n_connect != NO)	// 작업이 끝나지 않았다면...
		{
			NetworkConnectTime[1] = GetCurrentTime();
			NetworkConnectTime[2] = NetworkConnectTime[1] - NetworkConnectTime[0];

			if (NetworkConnectTime[2] > 13000)	// 10초 동안 들어오지 않는다면...
			{
				n_barcoderead_step = 5000;

				sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] 10초간 답변이 오지 않았습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER] Answer did not come 10 seconds");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
//				FuncRet = RET_ERROR;
				break;

			}
		}
		else if (st_client[NETWORK_CLIENT_1].n_connect == NO)	// 작업이 끝났다면...
		{
			n_barcoderead_step = 300;
		}	
		break;

	case 300:
		// **********************************************************************
		//  Reply를 분석한다.
		// **********************************************************************
		if(st_NW.mstr_result[1] == "PASS")
		{
			n_barcoderead_step = 400;
		}
		else								// 비정상이면...
		{
			n_barcoderead_step = 5000;

			if (st_NW.mstr_msg[1] == "")
			{
				sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER - LIST] 비정상 데이터가 전송되었습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_abnormal_msg, "[LABEL SERVER - LIST] data was sent to the abnormal.");
				}

			}
			else
			{	
				st_NW.mstr_msg[1].TrimRight(' ');
				st_NW.mstr_msg[1].TrimLeft(' ');
				sprintf(st_msg.c_abnormal_msg, ("[LABEL SERVER ERROR] : " + st_NW.mstr_msg[1]));
				

			}			
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 완료 출력 요청
			//FuncRet = RET_ERROR;

		}
		break;

	case 400:
		n_barcoderead_step = 500;
		break;

	case 500:
		// 다 정상이니까, 테스터에게 새로운 LOT임을 알려준다.
		if (st_handler.cwnd_main != NULL)
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
		}
		ml_untilwait_time[0] = GetCurrentTime();
		n_barcoderead_step = 600;
		
		break;

	case 600:
		ml_untilwait_time[1] = GetCurrentTime();
		ml_untilwait_time[2] = ml_untilwait_time[1] - ml_untilwait_time[0];
		if(st_handler.mn_receive == YES)
		{
			n_barcoderead_step = 1000;
		}
		if(ml_untilwait_time[2] > 5000)
		{
			n_barcoderead_step = 5000;
		}
		break;

	case 1000:
//		if( st_work.mn_Barcode_OutPrint == -1 ||
//			(st_work.mn_Barcode_OutPrint != 0 && st_work.mn_Barcode_OutPrint % 10 == 0) )
//		{	
			if(st_work.mn_Barcode_OutPrint == 10) 
			{
				n_barcoderead_step = 10000;
				break;
			}

			mn_printnum = st_work.mn_Barcode_OutPrint % 10;
			nRet_1 = Func.Barcode_Create(mn_printnum, 0);
			if(nRet_1 == CTLBD_RET_GOOD)
			{
				st_work.mn_Barcode_OutPrint++;
			}
			else
			{
				st_work.mn_Barcode_OutPrint++;
			}
//		}
//		else
//		{
//			mn_printnum = st_work.mn_Barcode_OutPrint % 10;
//			nRet_1 = Func.Barcode_Create(mn_printnum, 0);
//			if(nRet_1 == CTLBD_RET_GOOD)
//			{
//				st_work.mn_Barcode_OutPrint++;
//			}
//			else
//			{
//				st_work.mn_Barcode_OutPrint++;
//			}
//		}
		break;

	case 5000:
		FuncRet = RET_ERROR;
		n_barcoderead_step = 0;
		break;

	case 10000:
		FuncRet = RET_GOOD;
		n_barcoderead_step = 0;
		break;
	}

	return FuncRet;
}
