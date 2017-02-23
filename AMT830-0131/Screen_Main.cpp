// Screen_Main.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Main.h"

#include "Dialog_KeyBoard.h"
#include "Dialog_Keypad.h"
#include "Dialog_Message.h"
#include "FastechPublic_IO.h"

#include "Dialog_Select.h"
#include "Dialog_Buffer_Status.h"
#include "io.h" 

#include "CtlBd_Function.h"
#include "AShifterModuleManager.h"
#include "GridControlAlg.h"

#include "Dialog_Scrap_Info_View.h"
#include "Dialog_Track_Out.h"

#include "CtlBd_Library.h"

#include "MainFrm.h"
#include "Dialog_Lot_Open.h"	//20120522
#include "Dialog_Worker.h"	//20120711
#include "Variable.h"
#include "Dialog_Track_Out.h"
#include "Dialog_Work_Information.h"
#include "Run_Handler_Check.h"
#include "APartHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main

IMPLEMENT_DYNCREATE(CScreen_Main, CFormView)

#define	TM_ANIMATION 500
#define TM_CLIP_REMOVE 600
#define TM_CLIP_RECOVERY 700
#define TM_STACKER_DOWN 800

CScreen_Main::CScreen_Main()
	: CFormView(CScreen_Main::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Main)
	//}}AFX_DATA_INIT
}

CScreen_Main::~CScreen_Main()
{
	//KillTimer(TM_LOTEND_READY);
}

void CScreen_Main::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Main)

	DDX_Control(pDX, IDC_BTN_CLIP_CAP_RESET, m_clip_recovery);
	DDX_Control(pDX, IDC_BTN_CLIP_CAP_REMOVE, m_clip_remove);
	DDX_Control(pDX, IDC_BTN_MULTILOT_INIT, m_btn_multilot_init);
	DDX_Control(pDX, IDC_BTN_VISION_CNT_RESET, m_btn_viscnt_reset);
	DDX_Control(pDX, IDC_BTN_LOT_CANCEL, m_btn_lot_cancel);
	DDX_Control(pDX, IDC_PAD_BCR_ID4_1, m_pad_bcr_id4_1);
	DDX_Control(pDX, IDC_PAD_BCR_ID3_1, m_pad_bcr_id3_1);
	DDX_Control(pDX, IDC_PAD_BCR_ID2_1, m_pad_bcr_id2_1);
	DDX_Control(pDX, IDC_PAD_BCR_ID_1, m_pad_bcr_id_1);
	DDX_Control(pDX, IDC_GROUP_BUFFER_STATUS, m_group_buffer_status);
	DDX_Control(pDX, IDC_BTN_DOOR_OPEN, m_btn_door_open);
	DDX_Control(pDX, IDC_DGT_ALARM, m_dgt_alarm);
	DDX_Control(pDX, IDC_DGT_MTBI, m_dgt_mtbi);
	DDX_Control(pDX, IDC_MSG_STOP, m_msg_stop);
	DDX_Control(pDX, IDC_MSG_RUN, m_msg_run);
	DDX_Control(pDX, IDC_MSG_ALARM, m_msg_alarm);
	DDX_Control(pDX, IDC_MSG_MTBI, m_msg_mtbi);
	DDX_Control(pDX, IDC_GROUP_DAILY_YIELD_INFO, m_group_daily_yield_info);
	DDX_Control(pDX, IDC_GROUP_TIME_INFO, m_group_time_info);
	DDX_Control(pDX, IDC_GROUP_LOT_INFO, m_group_lot_info);
	DDX_Control(pDX, IDC_GROUP_LOT_YIELD_INFO, m_group_lot_yield_info);
	DDX_Control(pDX, IDC_DGT_STOP, m_dgt_stop);
	DDX_Control(pDX, IDC_DGT_RUN, m_dgt_run);
	DDX_Control(pDX, IDC_BTN_LOT_OPEN, m_btn_lot_open);	//20120522
	DDX_Control(pDX, IDC_PAD_BCR_ID, m_pad_bcr_id);	//20120508유성준 bcr
	DDX_Control(pDX, IDC_PAD_BCR_ID2, m_pad_bcr_id2);	//20120508유성준 bcr
	DDX_Control(pDX, IDC_PAD_BCR_ID3, m_pad_bcr_id3);	//20120508유성준 bcr
	DDX_Control(pDX, IDC_PAD_BCR_ID4, m_pad_bcr_id4);	//20120508유성준 bcr
	DDX_Control(pDX, IDC_MSG_BCR_ID, m_msg_bcr_id);	//20120508유성준 bcr
	DDX_Control(pDX, IDC_MSG_BCR_ID2, m_msg_bcr_id2);	//20120508유성준 bcr
	DDX_Control(pDX, IDC_MSG_BCR_ID3, m_msg_bcr_id3);	//20120508유성준 bcr
	DDX_Control(pDX, IDC_MSG_BCR_ID4, m_msg_bcr_id4);	//20120508유성준 bcr
	DDX_Control(pDX, IDC_GROUP_BCR_SET, m_group_bcr_set);	//20120508유성준 bcr
	DDX_Control(pDX, IDC_GROUP_JOB_CHANGE, m_group_jobchange);	
	DDX_Control(pDX, IDC_GROUP_JOB_CHANGE2, m_group_jobchange2);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Main, CFormView)
	//{{AFX_MSG_MAP(CScreen_Main)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_DOOR_OPEN, OnBtnDoorOpen)
	ON_BN_CLICKED(IDC_BTN_LOT_OPEN, OnBtnLotOpen)	//20120522
 	ON_BN_CLICKED(IDC_PAD_BCR_ID, OnPadBcrId)
	ON_BN_CLICKED(IDC_PAD_BCR_ID2, OnPadBcrId2)
	ON_BN_CLICKED(IDC_PAD_BCR_ID3, OnPadBcrId3)
	ON_BN_CLICKED(IDC_PAD_BCR_ID4, OnPadBcrId4)
	ON_EN_SETFOCUS(IDC_EDIT_BCR_ID, OnSetfocusEditBcrId)
	ON_EN_SETFOCUS(IDC_EDIT_BCR_ID2, OnSetfocusEditBcrId2)
	ON_EN_SETFOCUS(IDC_EDIT_BCR_ID3, OnSetfocusEditBcrId3)
	ON_EN_SETFOCUS(IDC_EDIT_BCR_ID4, OnSetfocusEditBcrId4)
	ON_EN_SETFOCUS(IDC_EDIT_BCR_ID_1, OnSetfocusEditBcrId_1)
	ON_EN_SETFOCUS(IDC_EDIT_BCR_ID2_1, OnSetfocusEditBcrId2_1)
	ON_EN_SETFOCUS(IDC_EDIT_BCR_ID3_1, OnSetfocusEditBcrId3_1)
	ON_EN_SETFOCUS(IDC_EDIT_BCR_ID4_1, OnSetfocusEditBcrId4_1)
	ON_BN_CLICKED(IDC_BTN_WORK_INFORMATION, OnBtnWorkInformation)
	ON_BN_CLICKED(IDC_PAD_BCR_ID_1, OnPadBcrId1)
	ON_BN_CLICKED(IDC_PAD_BCR_ID2_1, OnPadBcrId21)
	ON_BN_CLICKED(IDC_PAD_BCR_ID3_1, OnPadBcrId31)
	ON_BN_CLICKED(IDC_PAD_BCR_ID4_1, OnPadBcrId41)
	ON_BN_CLICKED(IDC_BTN_LOT_CANCEL, OnBtnLotCancel)
	ON_BN_CLICKED(IDC_BTN_VISION_CNT_RESET, OnBtnVisionCntReset)
	ON_BN_CLICKED(IDC_BTN_MULTILOT_INIT, OnBtnMultilotInit)
	ON_BN_CLICKED(IDC_BTN_WORK_INFORMATION2, OnBtnWorkInformation2)
	ON_BN_CLICKED(IDC_BTN_WORK_INFORMATION3, OnBtnWorkInformation3)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER1_MDPOS, OnBtnWorkbuffer1Mdpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER1_LHPOS, OnBtnWorkbuffer1Lhpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER1_RHSPOS, OnBtnWorkbuffer1Rhspos)
	ON_BN_CLICKED(IDC_WORKBUFFER1_CLIP, OnWorkbuffer1Clip)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER1_ULDPOS, OnBtnWorkbuffer1Uldpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER2_MDPOS, OnBtnWorkbuffer2Mdpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER2_LHPOS, OnBtnWorkbuffer2Lhpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER2_RHSPOS, OnBtnWorkbuffer2Rhspos)
	ON_BN_CLICKED(IDC_WORKBUFFER2_CLIP, OnWorkbuffer2Clip)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER2_ULDPOS, OnBtnWorkbuffer2Uldpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER3_MDPOS, OnBtnWorkbuffer3Mdpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER3_LHPOS, OnBtnWorkbuffer3Lhpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER3_RHSPOS, OnBtnWorkbuffer3Rhspos)
	ON_BN_CLICKED(IDC_WORKBUFFER3_CLIP, OnWorkbuffer3Clip)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER3_ULDPOS, OnBtnWorkbuffer3Uldpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER4_MDPOS, OnBtnWorkbuffer4Mdpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER4_LHPOS, OnBtnWorkbuffer4Lhpos)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER4_RHSPOS, OnBtnWorkbuffer4Rhspos)
	ON_BN_CLICKED(IDC_WORKBUFFER4_CLIP, OnWorkbuffer4Clip)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER4_ULDPOS, OnBtnWorkbuffer4Uldpos)
	ON_BN_CLICKED(IDC_BTN_MDROBOT_WORKPOS1, OnBtnMdrobotWorkpos1)
	ON_BN_CLICKED(IDC_BTN_MDROBOT_WORKPOS2, OnBtnMdrobotWorkpos2)
	ON_BN_CLICKED(IDC_BTN_MDROBOT_WORKPOS3, OnBtnMdrobotWorkpos3)
	ON_BN_CLICKED(IDC_BTN_MDROBOT_WORKPOS4, OnBtnMdrobotWorkpos4)
	ON_BN_CLICKED(IDC_BTN_MDROBOT_INIT, OnBtnMdrobotInit)
	ON_BN_CLICKED(IDC_BTN_LHSROBOT_WORKPOS1, OnBtnLhsrobotWorkpos1)
	ON_BN_CLICKED(IDC_BTN_LHSROBOT_WORKPOS2, OnBtnLhsrobotWorkpos2)
	ON_BN_CLICKED(IDC_BTN_LHSROBOT_WORKPOS3, OnBtnLhsrobotWorkpos3)
	ON_BN_CLICKED(IDC_BTN_LHSROBOT_WORKPOS4, OnBtnLhsrobotWorkpos4)
	ON_BN_CLICKED(IDC_BTN_LHSROBOT_INIT, OnBtnLhsrobotInit)
	ON_BN_CLICKED(IDC_BTN_RHSROBOT_WORKPOS1, OnBtnRhsrobotWorkpos1)
	ON_BN_CLICKED(IDC_BTN_RHSROBOT_WORKPOS2, OnBtnRhsrobotWorkpos2)
	ON_BN_CLICKED(IDC_BTN_RHSROBOT_WORKPOS3, OnBtnRhsrobotWorkpos3)
	ON_BN_CLICKED(IDC_BTN_RHSROBOT_WORKPOS4, OnBtnRhsrobotWorkpos4)
	ON_BN_CLICKED(IDC_BTN_RHSROBOT_INIT, OnBtnRhsrobotInit)
	ON_BN_CLICKED(IDC_BTN_CLIPINSERT_WORKPOS1, OnBtnClipinsertWorkpos1)
	ON_BN_CLICKED(IDC_BTN_CLIPINSERT_WORKPOS2, OnBtnClipinsertWorkpos2)
	ON_BN_CLICKED(IDC_BTN_CLIPINSERT_WORKPOS3, OnBtnClipinsertWorkpos3)
	ON_BN_CLICKED(IDC_BTN_CLIPINSERT_WORKPOS4, OnBtnClipinsertWorkpos4)
	ON_BN_CLICKED(IDC_BTN_CLIPINSERT_INIT, OnBtnClipinsertInit)
	ON_BN_CLICKED(IDC_BTN_CLIPCLAMP_WORKPOS1, OnBtnClipclampWorkpos1)
	ON_BN_CLICKED(IDC_BTN_CLIPCLAMP_WORKPOS2, OnBtnClipclampWorkpos2)
	ON_BN_CLICKED(IDC_BTN_CLIPCLAMP_WORKPOS3, OnBtnClipclampWorkpos3)
	ON_BN_CLICKED(IDC_BTN_CLIPCLAMP_WORKPOS4, OnBtnClipclampWorkpos4)
	ON_BN_CLICKED(IDC_BTN_SORTERRBT_WORKPOS1, OnBtnSorterrbtWorkpos1)
	ON_BN_CLICKED(IDC_BTN_SORTERRBT_WORKPOS2, OnBtnSorterrbtWorkpos2)
	ON_BN_CLICKED(IDC_BTN_SORTERRBT_WORKPOS3, OnBtnSorterrbtWorkpos3)
	ON_BN_CLICKED(IDC_BTN_SORTERRBT_WORKPOS4, OnBtnSorterrbtWorkpos4)
	ON_BN_CLICKED(IDC_BTN_SORTERRBT_INIT, OnBtnSorterrbtInit)
	ON_BN_CLICKED(IDC_BTN_ULDROBOT_VISION_PICK, OnBtnUldrobotVisionPick)
	ON_BN_CLICKED(IDC_BTN_ULDROBOT_DOWN, OnBtnUldrobotDown)
	ON_BN_CLICKED(IDC_BTN_SIMUEL_INIT, OnBtnSimuelInit)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER1_INIT, OnBtnWorkbuffer1Init)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER2_INIT, OnBtnWorkbuffer2Init)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER3_INIT, OnBtnWorkbuffer3Init)
	ON_BN_CLICKED(IDC_BTN_WORKBUFFER4_INIT, OnBtnWorkbuffer4Init)
	ON_BN_CLICKED(IDC_BTN_MDROBOT_DOWN, OnBtnMdrobotDown)
	ON_BN_CLICKED(IDC_BTN_LHSROBOT_DOWN, OnBtnLhsrobotDown)
	ON_BN_CLICKED(IDC_BTN_RHSROBOT_DOWN, OnBtnRhsrobotDown)
	ON_BN_CLICKED(IDC_BTN_CLIPINSERT_DOWN, OnBtnClipinsertDown)
	ON_BN_CLICKED(IDC_BTN_CLIPCLAMP_DOWN, OnBtnClipclampDown)
	ON_BN_CLICKED(IDC_BTN_SORTER_DOWN, OnBtnSorterDown)
	ON_BN_CLICKED(IDC_BTN_ULDRBT_DOWN, OnBtnUldrbtDown)
	ON_BN_CLICKED(IDC_BTN_ULDRBT_INIT, OnBtnUldrbtInit)
	ON_BN_CLICKED(IDC_BTN_CLIPCLAMP_UP, OnBtnClipclampUp)
	ON_BN_CLICKED(IDC_BTN_MDROBOT_UP, OnBtnMdrobotUp)
	ON_BN_CLICKED(IDC_BTN_LHSROBOT_UP, OnBtnLhsrobotUp)
	ON_BN_CLICKED(IDC_BTN_RHSROBOT_UP, OnBtnRhsrobotUp)
	ON_BN_CLICKED(IDC_BTN_CLIPINSERT_UP, OnBtnClipinsertUp)
	ON_BN_CLICKED(IDC_BTN_SORTER_UP, OnBtnSorterUp)
	ON_BN_CLICKED(IDC_BTN_ULDRBT_UP, OnBtnUldrbtUp)
	ON_BN_CLICKED(IDC_BTN_CLIP_CAP_REMOVE, OnBtnClipCapRemove)
	ON_BN_CLICKED(IDC_BTN_CLIP_CAP_RESET, OnBtnClipCapReset)
	ON_BN_CLICKED(IDC_BTN_MD_STACKER2_TRAY_LOCK_ONOFF, OnBtnMDTrayLockOnOff)
	ON_BN_CLICKED(IDC_BTN_LH_STACKER2_TRAY_LOCK_ONOFF, OnBtnLHTrayLockOnOff)
	ON_BN_CLICKED(IDC_BTN_RH_STACKER2_TRAY_LOCK_ONOFF, OnBtnRHTrayLockOnOff)
	ON_BN_CLICKED(IDC_BTN_MD_STACKER1_DOWN, OnBtnMDStackerDown)
	ON_BN_CLICKED(IDC_BTN_LH_STACKER1_DOWN, OnBtnLHStackerDown)
	ON_BN_CLICKED(IDC_BTN_RH_STACKER1_DOWN, OnBtnRHStackerDown)
	ON_BN_CLICKED(IDC_BTN_SERIAL_NO, OnBtnSerialNo)
	ON_EN_SETFOCUS(IDC_EDIT_SERIAL_NO, OnSetfocusEditSerialNo)
	ON_EN_SETFOCUS(IDC_EDIT_LOT_NO, OnSetfocusEditLotNo)
	ON_BN_CLICKED(IDC_BTN_UMD_STACKER2_TRAY_LOCK_ONOFF, OnBtnUmdStacker2TrayLockOnoff)
	ON_BN_CLICKED(IDC_BTN_BARCODE_PRINT_RESET, OnBtnBarcodePrintReset)
	//}}AFX_MSG_MAP
 	ON_MESSAGE(WM_WORK_END, OnMain_Work_Info_Display)  // 테스트 결과 정보 화면에 출력하기 위한 사용자 정의 메시지 추가 
 	ON_MESSAGE(SSM_CLICK, OnCellClick)
// 	ON_MESSAGE(WM_RECEIVED_NEW_LOT, OnReceivedNewLot)  // 새로운 Lot을 받았다.
// 	ON_MESSAGE(WM_DRAW_LOT_TIME, OnDrawLotTime)
// 	ON_MESSAGE(WM_DRAW_WORK_SITE, OnDrawWorkSite)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main diagnostics

#ifdef _DEBUG
void CScreen_Main::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Main::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Main message handlers

void CScreen_Main::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	int i = 0;

	// ************************************************************************** 
	// 컨트롤에 설정할 폰트 생성한다                                              
	// ************************************************************************** 
	mp_main_font = NULL;
	mp_main_font = new CFont;
	mp_main_font->CreateFont(15,0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");

	mp_main_big_font = NULL;
	mp_main_big_font = new CFont;
	mp_main_big_font->CreateFont(9,0,0,0,0,0,0,0,0,0,0,DEFAULT_QUALITY,0,"2002L");
	// ************************************************************************** 

	OnMain_GroupBox_Set();
	OnMain_Label_Set();
	OnMain_Digital_Count_Set();
	OnMain_Init_Lot_Yield_info();
	OnMain_Init_Daily_Yield_info();
	OnMain_Init_Lot_Info();
	OnMain_Init_Grid();

	OnMain_Display_Lot_Info();
	OnMain_Display_Lot_Yield_info();
	OnMain_Display_Daily_Yield_info();
//	OnMain_Display_CycleTime_Info();
//	OnMain_Display_WorkBuffer();
	OnMain_Display_VisionBuffer();

	//////////////////////////////////////////// 20120707
	OnMain_CheckerCtrl_Set();
	
	for (i=0; i < 7; i++)
	{
		OnMainWork_Tray_Display(i);
		OnMainWork_Picker_Display(i);
	}

	////////////////////////////////////////////

	SetTimer(1234, 500, 0);

	mn_time_select = 0;	
	
	st_handler.cwnd_main = this;		// MAIN 화면 핸들 설정

	OnMain_Data_Set();
	Init_Grid_Lot();//20120711

	//////////////////////////////////
	OnMainMulLotDisplay();

	Init_Animation();

//	EnableButton(FALSE);

	((CEdit*)GetDlgItem(IDC_EDIT_LOT_NO))->SetWindowText(st_NW.mstr_Test_LotNo);
	((CEdit*)GetDlgItem(IDC_EDIT_SERIAL_NO))->SetWindowText(st_NW.mstr_Test_SerialNo);

	SetTimer(TM_ANIMATION,10,NULL);
}

void CScreen_Main::OnDestroy() 
{
	st_handler.cwnd_main = NULL;		// MAIN 화면 핸들 초기화
	
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다
	// **************************************************************************
	
  	delete mp_main_font;
	mp_main_font = NULL;

	delete mp_main_big_font;
	mp_main_big_font = NULL;
	// **************************************************************************
	
	KillTimer(TM_ANIMATION);
//	EnableButton(FALSE);
	CFormView::OnDestroy();
}

void CScreen_Main::OnTimer(UINT nIDEvent) 
{
	int i=0,nResponse = CTLBD_RET_GOOD;

	CDialog_Message msg_dlg;
	int mn_response;

	switch(nIDEvent)
	{
		case 200:
			if (nResponse == CTLBD_RET_GOOD)
			{
				KillTimer(200);
			}
			break;

	/////////////// 20120924
		case TM_BCR_HS_DATA_RESET:

			if (st_other.b_BCR_Data_Reset_OK1 == TRUE && st_other.b_HS_Data_Reset_OK1 == TRUE)
			{
				st_other.b_BCR_Data_Reset_OK1 = FALSE;
				st_other.b_HS_Data_Reset_OK1 = FALSE;
				sprintf(st_msg.c_normal_msg, "BCR && HS Data Reset 응답 완료!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "Complete response BCR && HS Data Reset!!");
				}
				

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);

				KillTimer(TM_BCR_HS_DATA_RESET);
			}
			else
			{
				if(st_other.b_BCR_Data_Reset_OK1 != TRUE)
				{
					l_bcr_send_wait[1] = GetCurrentTime();
					l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
					if(l_bcr_send_wait[2] < 0)
					{
						l_bcr_send_wait[0] = GetCurrentTime();
					}
					if(l_bcr_send_wait[2] > 5000)
					{
						if(st_other.b_BCR_Data_Reset_OK == TRUE)
						{
							st_other.b_BCR_Data_Reset_OK1 = TRUE;
							st_other.b_BCR_Data_Reset_OK = FALSE;
							bcr_retry = 0;
							sprintf(st_msg.c_normal_msg, "BCR Data Reset 응답 완료!!");
							if(st_handler.mn_language == LANGUAGE_ENGLISH) 
							{
								sprintf(st_msg.c_normal_msg, "Complete response BCR Data Reset!!");
							}

							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
							Func.OnSet_Vision_send(VISION_YIELD_YES);
							Func.OnSet_Vision_send(HEATSINK_YIELD);
							break;
						}
						else
						{
							bcr_retry++;
							if(bcr_retry > 3)
							{
								bcr_retry = 0;
								if(st_handler.cwnd_list != NULL)
								{
									sprintf(st_msg.c_normal_msg, "BCR Data Reset 응답 에러!!");
									if(st_handler.mn_language == LANGUAGE_ENGLISH) 
									{
										sprintf(st_msg.c_normal_msg, "BCR Data Reset response error!!");
									}

									st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
									KillTimer(TM_BCR_HS_DATA_RESET);
								}
							}
						}

					}		
				}
				
				if(st_other.b_HS_Data_Reset_OK1 != TRUE && st_other.b_BCR_Data_Reset_OK1 == TRUE)
				{
					l_hs_send_wait[1] = GetCurrentTime();
					l_hs_send_wait[2] = l_hs_send_wait[1] - l_hs_send_wait[0];
					if(l_hs_send_wait[2] < 0)
					{
						l_hs_send_wait[0] = GetCurrentTime();
					}
					if(l_hs_send_wait[2] > 5000)
					{
						if(st_other.b_HS_Data_Reset_OK == TRUE)
						{
							st_other.b_HS_Data_Reset_OK = FALSE;
							st_other.b_HS_Data_Reset_OK1 = TRUE;
							hs_retry = 0;
							sprintf(st_msg.c_normal_msg, "HS Data Reset 응답 완료!!");
							if(st_handler.mn_language == LANGUAGE_ENGLISH) 
							{
								sprintf(st_msg.c_normal_msg, "Complete response HS Data Reset!!");
							}

							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
							Func.OnSet_Vision_send(HEATSINK_YIELD_YES);
						}
						else
						{
							hs_retry++;
							if(hs_retry > 3)
							{
								hs_retry = 0;
								if(st_handler.cwnd_list != NULL)
								{
									sprintf(st_msg.c_normal_msg, "HS Data Reset 응답 에러!!");
									if(st_handler.mn_language == LANGUAGE_ENGLISH) 
									{
										sprintf(st_msg.c_normal_msg, "HS Data Reset response error!!");
									}

									st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
									KillTimer(TM_BCR_HS_DATA_RESET);
								}
							}
						}
					}					
				}
			}

			break;
	///////////////
			
		case TM_LOTEND_READY:
			if(st_handler.n_module_retest[0] == CTL_NO && st_handler.n_module_retest[1] == CTL_NO &&
				st_handler.n_module_retest[2] == CTL_NO && st_handler.n_module_retest[3] == CTL_NO)
			{
				KillTimer(TM_LOTEND_READY);
			}
			else
			{
				for (i = 0; i < 4; i++)
				{
					if(st_handler.n_module_retest[i] == CTL_READY)
					{
						if(mn_retest_onoff[i] == 0)
						{
							mn_retest_onoff[i] = 1;
							GridColor(IDC_CUSTOM_BUFFER_MOTOR, i, 0, RED_C, WHITE_C);
						}
						else
						{
							mn_retest_onoff[i] = 0;
							GridColor(IDC_CUSTOM_BUFFER_MOTOR, i, 0, WHITE_C, BLUE_D);
						}
					}
					else
					{
						GridColor(IDC_CUSTOM_BUFFER_MOTOR, i, 0, WHITE_C, BLUE_D);
					}
				}
			}
			break;

		case TM_ANIMATION:
 			UpdateScreens();
			if(st_work.b_run_slmulation_md == TRUE)
			{
				UpdateMotorMove();
			}
			else
			{
				if(st_work.mn_run_status == dRUN)
				{
					UpdateMotorMove();
				}
			}
			break;

		case TM_CLIP_REMOVE:
			nResponse = ClipRemove();
			if(nResponse == CTLBD_RET_GOOD)
			{
				KillTimer(TM_CLIP_REMOVE);
				
				st_msg.str_fallacy_msg = _T("Clip Cap 제거 동작이 완료되었습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("Clip Cap removal operation has been completed");
				}

				mn_response = msg_dlg.DoModal();

				m_remove_flag = FALSE;
				
			}
			else if(nResponse != CTLBD_RET_PROCEED)
			{
				KillTimer(TM_CLIP_REMOVE);
				
				st_msg.str_fallacy_msg = _T("Clip Cap 제거 동작이 완료되지 않았습니다. 확인해 주세요.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("Clip Cap removal operation has not been completed. Please confirm");
				}

				mn_response = msg_dlg.DoModal();

				m_remove_flag = FALSE;
				
			}
			break;
			
		case TM_CLIP_RECOVERY:
			nResponse = ClipRecovery();
			if(nResponse == CTLBD_RET_GOOD)
			{
				KillTimer(TM_CLIP_RECOVERY);
				
				st_msg.str_fallacy_msg = _T("Clip 복구 동작이 완료되었습니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("Clip the recovery operation has been completed");
				}

				mn_response = msg_dlg.DoModal();

				m_recorvery_flag = FALSE;
				
			}
			else if(nResponse != CTLBD_RET_PROCEED)
			{
				KillTimer(TM_CLIP_RECOVERY);
				
				st_msg.str_fallacy_msg = _T("Clip 복구 동작이 완료되지 않았습니다. 확인해 주세요.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH)
				{
					st_msg.str_fallacy_msg = _T("Clip the recovery operation was not completed. Please confirm");
				}

				mn_response = msg_dlg.DoModal();

				m_recorvery_flag = FALSE;
				
			}
			break;

		case TM_STACKER_DOWN:
			nResponse = OnStackerDown(st_work.mn_stacker_num);

			if(nResponse == CTLBD_RET_GOOD)
			{
				KillTimer(TM_STACKER_DOWN);

				st_msg.str_fallacy_msg = _T("Stacker Down Complete.");
				mn_response = msg_dlg.DoModal();

				st_work.mn_stacker_num = -1;
			}
			else if(nResponse != CTLBD_RET_PROCEED)
			{
				KillTimer(TM_STACKER_DOWN);

				st_msg.str_fallacy_msg = _T("Stacker Down Complete.");
				mn_response = msg_dlg.DoModal();

				st_work.mn_stacker_num = -1;
			}
	}
		
	CFormView::OnTimer(nIDEvent);

}

void CScreen_Main::OnMain_GroupBox_Set()
{
	CSxLogFont main_font(17, FW_THIN, false, "Bitstream Vera Sans Mono");

	m_group_lot_info.SetFont(main_font);
	m_group_lot_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_lot_info.SetFontBold(TRUE);

	m_group_daily_yield_info.SetFont(main_font);
	m_group_daily_yield_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_daily_yield_info.SetFontBold(TRUE);
	
	m_group_lot_yield_info.SetFont(main_font);
	m_group_lot_yield_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_lot_yield_info.SetFontBold(TRUE);
		
	m_group_time_info.SetFont(main_font);
	m_group_time_info.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_time_info.SetFontBold(TRUE);

	///////////////////////////////////////////////////////// 20120509 유성준
	m_group_bcr_set.SetFont(main_font);
	m_group_bcr_set.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_bcr_set.SetFontBold(TRUE);

	///////////////////////////////////////////////////////// 20120608 유성준
	m_group_jobchange.SetFont(main_font);
	m_group_jobchange.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_jobchange.SetFontBold(TRUE);

	m_group_jobchange2.SetFont(main_font);
	m_group_jobchange2.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_jobchange2.SetFontBold(TRUE);
	
	/////////////////////////////////////////////////////////
	m_group_buffer_status.SetFont(main_font);
	m_group_buffer_status.SetCatptionTextColor(RGB(145, 25, 0));
	m_group_buffer_status.SetFontBold(TRUE);

}

void CScreen_Main::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	
	Grid = NULL;

	delete Grid;
}

void CScreen_Main::GridControl(UINT nID, int type, int row, int col, int pos)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	SS_CELLTYPE CellType;
	SS_DATEFORMAT dateFormat = { TRUE, '/', IDF_DDMMYY, FALSE };
	SS_TIMEFORMAT tmFormat;
	CString strList;
	strList = "combo1\tcombo2\tcombo3\tcombo4\tcombo5\tcombo6";
	double a = 0.5;

	switch (type)
	{
		case DATE:			
			Grid->SetTypeDate(&CellType, 0 , &dateFormat, NULL, NULL);
			break;

		case EDIT:
			Grid->SetTypeEdit(&CellType,ES_CENTER,60,SS_CHRSET_CHR,SS_CASE_NOCASE);
			break;

		case COMBO:
			// SetValue(col, row, data); data = ""0"(1선택) data = "1"(2선택) data = "2"(3선택)
			Grid->SetTypeComboBox(&CellType, 0, strList);
			break;

		case BUTTON:
			if (pos == 0)
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Go", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE, SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			else
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Read", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE,SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			break;

		case STATIC:
			Grid->SetTypeStaticText(&CellType, SS_TEXT_CENTER | SS_TEXT_VCENTER);
			break;

		case TIME:
			tmFormat.b24Hour=TRUE;
			tmFormat.bSpin=FALSE;
			tmFormat.bSeconds=FALSE;
			tmFormat.cSeparator=':';
			Grid->SetTypeTime(&CellType, 0, &tmFormat, NULL, NULL);
			break;

		case PERCENT:
			Grid->SetTypePercentEx(&CellType,ES_LEFTALIGN,2,.01,100,".",0,0,0,0,1.001);
			break;

		case CHECK1:
			Grid->SetTypeCheckBox(&CellType,BS_CENTER, "", 
				"CheckUp", BT_BITMAP,
				"CheckDown", BT_BITMAP,
				"CheckFocusUp", BT_BITMAP,
				"CheckFocusDown", BT_BITMAP,
				"CheckDisabledUp", BT_BITMAP,
				"CheckDisabledDown", BT_BITMAP);
			break;

		case NUMBER:
			Grid->SetTypeNumberEx(&CellType,0,pos, 0, 10000,0,".",",",0,0,1,0,1.001);
			break;
	}

	Grid->SetCellType(col,row,&CellType);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}

void CScreen_Main::OnMain_Label_Set()
{
	m_msg_alarm.SetFont(mp_main_font);
	m_msg_alarm.SetWindowText(_T("Alarm"));
	m_msg_alarm.SetCenterText();
	m_msg_alarm.SetColor(RGB(0, 0, 0));
	m_msg_alarm.SetGradientColor(RGB(255, 255, 0));
	m_msg_alarm.SetTextColor(RGB(255, 255, 255));
	
	m_msg_run.SetFont(mp_main_font);
	m_msg_run.SetWindowText(_T("Run"));
	m_msg_run.SetCenterText();
	m_msg_run.SetColor(RGB(0, 0, 0));
	m_msg_run.SetGradientColor(RGB(0, 0, 255));
	m_msg_run.SetTextColor(RGB(255, 255, 255));
	
	m_msg_stop.SetFont(mp_main_font);
	m_msg_stop.SetWindowText(_T("Stop"));
	m_msg_stop.SetCenterText();
	m_msg_stop.SetColor(RGB(0, 0, 0));
	m_msg_stop.SetGradientColor(RGB(255, 0, 0));
	m_msg_stop.SetTextColor(RGB(255, 255, 255));

	m_msg_mtbi.SetFont(mp_main_font);
	m_msg_mtbi.SetWindowText(_T("MTBI"));
	m_msg_mtbi.SetCenterText();
	m_msg_mtbi.SetColor(RGB(0, 0, 0));
	m_msg_mtbi.SetGradientColor(RGB(0, 255, 255));
	m_msg_mtbi.SetTextColor(RGB(255, 255, 255));

	m_msg_bcr_id.SetTextFont(mp_main_font);
	m_msg_bcr_id.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_bcr_id.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_msg_bcr_id2.SetTextFont(mp_main_font);
	m_msg_bcr_id2.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_bcr_id2.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_msg_bcr_id3.SetTextFont(mp_main_font);
	m_msg_bcr_id3.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_bcr_id3.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_msg_bcr_id4.SetTextFont(mp_main_font);
	m_msg_bcr_id4.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_bcr_id4.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_edit_bcr_id.SubclassDlgItem(IDC_EDIT_BCR_ID, this);
	m_edit_bcr_id.bkColor(RGB(50, 100, 150));
	m_edit_bcr_id.textColor(RGB(255, 255,255));
	m_edit_bcr_id.setFont(1, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_bcr_id2.SubclassDlgItem(IDC_EDIT_BCR_ID2, this);
	m_edit_bcr_id2.bkColor(RGB(50, 100, 150));
	m_edit_bcr_id2.textColor(RGB(255, 255,255));
	m_edit_bcr_id2.setFont(1, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_bcr_id3.SubclassDlgItem(IDC_EDIT_BCR_ID3, this);
	m_edit_bcr_id3.bkColor(RGB(50, 100, 150));
	m_edit_bcr_id3.textColor(RGB(255, 255,255));
	m_edit_bcr_id3.setFont(1, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_bcr_id4.SubclassDlgItem(IDC_EDIT_BCR_ID4, this);
	m_edit_bcr_id4.bkColor(RGB(50, 100, 150));
	m_edit_bcr_id4.textColor(RGB(255, 255,255));
	m_edit_bcr_id4.setFont(1, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_bcr_id_1.SubclassDlgItem(IDC_EDIT_BCR_ID_1, this);
	m_edit_bcr_id_1.bkColor(RGB(0, 200, 0));
	m_edit_bcr_id_1.textColor(RGB(255, 255,255));
	m_edit_bcr_id_1.setFont(1, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_bcr_id2_1.SubclassDlgItem(IDC_EDIT_BCR_ID2_1, this);
	m_edit_bcr_id2_1.bkColor(RGB(0, 200, 0));
	m_edit_bcr_id2_1.textColor(RGB(255, 255,255));
	m_edit_bcr_id2_1.setFont(1, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_bcr_id3_1.SubclassDlgItem(IDC_EDIT_BCR_ID3_1, this);
	m_edit_bcr_id3_1.bkColor(RGB(0, 200, 0));
	m_edit_bcr_id3_1.textColor(RGB(255, 255,255));
	m_edit_bcr_id3_1.setFont(1, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_bcr_id4_1.SubclassDlgItem(IDC_EDIT_BCR_ID4_1, this);
	m_edit_bcr_id4_1.bkColor(RGB(0, 200, 0));
	m_edit_bcr_id4_1.textColor(RGB(255, 255,255));
	m_edit_bcr_id4_1.setFont(1, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

/////////////////////////////////////////////////////////////

}

void CScreen_Main::OnMain_Digital_Count_Set()
{
	m_dgt_alarm.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_run.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_stop.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
	m_dgt_mtbi.SetStyle(CDigit::DS_TIMER, NULL, CDigit::DC_YELLOW, CDigit::DC_CYAN);
}

void CScreen_Main::OnMain_Init_Lot_Yield_info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 5;
	int col = 2;
	int i, j;

	CString str_tmp[4] = {"Input", "Pass", "Fail", "Yield"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_YIELD_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 20);

			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 70);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 150);
			}

			if (i == 1 && j == 1)
			{
				GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, ORANGE_C, WHITE_C);
			}
			else if (i == 1 || j == 1)
			{
				GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, PINK_D, BLACK_C);
			}
			else
			{
				if (j == 4)
				{
					GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, BLACK_C, RED_C);
				}
				else
				{
					GridColor(IDC_CUSTOM_LOT_YIELD_INFO, i, j, BLACK_C, GREEN_B);
				}
			}

			GridControl(IDC_CUSTOM_LOT_YIELD_INFO, STATIC, i, j, 0);
			GridFont(IDC_CUSTOM_LOT_YIELD_INFO, i, j, 15);
		}
	}

	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 1, 1, " ");
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 1, 2, "Prime");
//	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 1, 3, "Cum");

	for (i = 2; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_LOT_YIELD_INFO, i, 1, str_tmp[i - 2]);
	}
	Grid = NULL;
	delete Grid;
}

void CScreen_Main::OnMain_Init_Daily_Yield_info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 5;
	int col = 2;
	int i, j;

	CString str_tmp[4] = {"Input", "Pass", "Fail", "Yield"};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_DAILY_YIELD_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 19);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 70);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 150);
			}
			
			if (i == 1 && j == 1)
			{
				GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, ORANGE_C, WHITE_C);
			}
			else if (i == 1 || j == 1)
			{
				GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, PINK_D, BLACK_C);
			}
			else
			{
				if (j == 4)
				{
					GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, BLACK_C, RED_C);
				}
				else
				{
					GridColor(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, BLACK_C, GREEN_B);
				}
			}
			GridControl(IDC_CUSTOM_DAILY_YIELD_INFO, STATIC, i, j, 0);
			GridFont(IDC_CUSTOM_DAILY_YIELD_INFO, i, j, 15);
		}
	}

	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 1, 1, " ");
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 1, 2, "Prime");
//	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 1, 3, "Cum");
	
	for (i = 2; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_DAILY_YIELD_INFO, i, 1, str_tmp[i - 2]);
	}
	Grid = NULL;
	delete Grid;
}

void CScreen_Main::OnMain_Init_Lot_Info()
{
	TSpread *Grid;
	CString str_pos;
	int row = 6;//20140403 
	int col = 2;
	int i, j;

	CString str_tmp[6] = {"LOT NO", "PART ID", "EQP ID", "Oper ID", "QTY", "Label Type"};//20140403
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	for (i = 0; i <= row; i++)
	{
		for (j = 0; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 30);
  			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 70);
				GridColor(IDC_CUSTOM_LOT_INFO, i, j, YELLOW_L, BLACK_C);

				GridControl(IDC_CUSTOM_LOT_INFO, STATIC, i, j, 30);
				GridFont(IDC_CUSTOM_LOT_INFO, i, j, 15);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 150);
				GridColor(IDC_CUSTOM_LOT_INFO, i, j, BLACK_C, WHITE_C);

				GridControl(IDC_CUSTOM_LOT_INFO, STATIC, i, j, 30);

				if (i == 2)
				{
					GridFont(IDC_CUSTOM_LOT_INFO, i, j, 15);
				}
				else
				{
					GridFont(IDC_CUSTOM_LOT_INFO, i, j, 15);
				}
			}
		}
	}

	for (i = 1; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_LOT_INFO, i, 1, str_tmp[i - 1]);
	}
	Grid = NULL;
	delete Grid;
}

void CScreen_Main::OnMain_Time_Display(int n_state)
{
	int nSec;
	m_dgt_run.ActivateTimer(FALSE);
	m_dgt_stop.ActivateTimer(FALSE);
	m_dgt_alarm.ActivateTimer(FALSE);
	m_dgt_mtbi.ActivateTimer(FALSE);
	
	mn_time_select = 0;
	switch(n_state)
	{
	case dRUN:
		m_dgt_run.ActivateTimer();
		m_dgt_mtbi.ActivateTimer();//2014,0216
		if (mn_time_select == 0)
		{

			m_dgt_run.SetTime(st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds());
			nSec = st_handler.m_tDR.GetTotalHours() * 3600 + st_handler.m_tDR.GetMinutes() * 60 + st_handler.m_tDR.GetSeconds();
			if( st_work.m_nAlarmCnt == 0 )//g_handler.GetAlarmCnt() == 0 )
				nSec = nSec;
			else
			{
				nSec /= st_work.m_nAlarmCnt;//g_handler.GetAlarmCnt();
			}
			
			m_dgt_mtbi.SetTime( nSec / 3600, (nSec % 3600) / 60, nSec % 60 );
		}
		else
		{
			m_dgt_run.SetTime(st_handler.m_tR.GetTotalHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
			nSec = st_handler.m_tR.GetTotalHours() * 3600 + st_handler.m_tR.GetMinutes() * 60 + st_handler.m_tR.GetSeconds();
			if( st_work.m_nAlarmCnt == 0 )//g_handler.GetAlarmCnt() == 0 )
				nSec = nSec;
			else
			{
				nSec /= st_work.m_nAlarmCnt;//g_handler.GetAlarmCnt();
			}

			m_dgt_mtbi.SetTime( nSec / 3600, (nSec % 3600) / 60, nSec % 60 );
		}
		break;
		
	case CTL_dWARNING:
	case dJAM:
		m_dgt_alarm.ActivateTimer();
		if (mn_time_select == 0)
		{
			m_dgt_alarm.SetTime(st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds());
		}
		else
		{
			m_dgt_alarm.SetTime(st_handler.m_tJ.GetTotalHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());
		}
		break;
		
	case dLOCK:
	case dSTOP:
		m_dgt_stop.ActivateTimer();
		if (mn_time_select == 0)
		{
			m_dgt_stop.SetTime(st_handler.m_tDS.GetTotalHours(), st_handler.m_tDS.GetMinutes(), st_handler.m_tDS.GetSeconds());
		}
		else
		{
			m_dgt_stop.SetTime(st_handler.m_tS.GetTotalHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
		}
		break;
	}
	m_dgt_run.UpdateTimer();
	m_dgt_stop.UpdateTimer();
	m_dgt_alarm.UpdateTimer();
	m_dgt_mtbi.UpdateTimer();
}

void CScreen_Main::OnMain_TimeInfo_Display()
{
	m_dgt_run.ActivateTimer(FALSE);
	m_dgt_stop.ActivateTimer(FALSE);
	m_dgt_alarm.ActivateTimer(FALSE);
	m_dgt_mtbi.ActivateTimer(FALSE);

	
	if (mn_time_select == 0)
	{
		m_dgt_run.SetTime(st_handler.m_tDR.GetTotalHours(), st_handler.m_tDR.GetMinutes(), st_handler.m_tDR.GetSeconds());
		m_dgt_stop.SetTime(st_handler.m_tDS.GetTotalHours(), st_handler.m_tDS.GetMinutes(), st_handler.m_tDS.GetSeconds());
		m_dgt_alarm.SetTime(st_handler.m_tDJ.GetTotalHours(), st_handler.m_tDJ.GetMinutes(), st_handler.m_tDJ.GetSeconds());

		int nSec = st_handler.m_tDR.GetTotalHours() * 3600 + st_handler.m_tDR.GetMinutes() * 60 + st_handler.m_tDR.GetSeconds();
		if( st_work.m_nAlarmCnt == 0 )//  g_handler.GetAlarmCnt() == 0 )
			nSec = nSec;
		else
		{
			nSec /= st_work.m_nAlarmCnt;//g_handler.GetAlarmCnt();
		}
			
		m_dgt_mtbi.SetTime( nSec / 3600, (nSec % 3600) / 60, nSec % 60 );
	}
	else
	{
		m_dgt_run.SetTime(st_handler.m_tR.GetTotalHours(), st_handler.m_tR.GetMinutes(), st_handler.m_tR.GetSeconds());
		m_dgt_stop.SetTime(st_handler.m_tS.GetTotalHours(), st_handler.m_tS.GetMinutes(), st_handler.m_tS.GetSeconds());
		m_dgt_alarm.SetTime(st_handler.m_tJ.GetTotalHours(), st_handler.m_tJ.GetMinutes(), st_handler.m_tJ.GetSeconds());

		int nSec = st_handler.m_tR.GetTotalHours() * 3600 + st_handler.m_tR.GetMinutes() * 60 + st_handler.m_tR.GetSeconds();
		if( st_work.m_nAlarmCnt == 0 )// g_handler.GetAlarmCnt() == 0 )
			nSec = nSec;
		else
		{
			nSec /= st_work.m_nAlarmCnt;//g_handler.GetAlarmCnt();
		}

		m_dgt_mtbi.SetTime( nSec / 3600, (nSec % 3600) / 60, nSec % 60 );
	}
	
	m_dgt_run.UpdateTimer();
	m_dgt_stop.UpdateTimer();
	m_dgt_alarm.UpdateTimer();
	m_dgt_mtbi.UpdateTimer();
}


LRESULT CScreen_Main::OnMain_Work_Info_Display(WPARAM wParam,LPARAM lParam) 
{
	int mn_state = wParam;
	
	switch(mn_state)
	{	
	case MAIN_TIMEINFO:
		OnMain_Time_Display(lParam);
		break;

	case MAIN_COUNTINFO:
		OnMain_Display_Lot_Yield_info();
		OnMain_Display_Daily_Yield_info();
		break;

	case MAIN_LOTINFO:
		OnMain_Display_Lot_Info();
		break;
		
	//////////////////// 20121002
	case MAIN_VISION_CNT_RESET:
		OnBtnVisionCntReset();
		break;
	//////////////////// 

	case MAIN_CYCLETIME:
		OnMain_Display_CycleTime_Info();
		break;
	////////////////////////////////// 20120524	
	case MAIN_BCR_DISPLAY:
		OnMain_BCR_DISPLAY(lParam);
		break;
    ////////////////////////////////// 20120707	
	case TRAY_INFO:
		OnMainWork_Tray_Display(lParam);
		break;
	case MAIN_PICKERINFO:
		OnMainWork_Picker_Display(lParam);
		break;

	//////////////////////////////////
	case MAIN_REJECT_TRAY:
		OnMainWork_RejectTray_Change();
		break;

	//////////////////////////////////
	case MAIN_LOT_RESTART:
		OnMainLotReStart_Display();
		break;

	case MAIN_EQP_LOTEND:
		OnMainMulLotDisplay();
		break;
	//////////////////////////////////
	case MAIN_CAP_REMOVE_CHECK:
		EnableButton(TRUE);
		break;

	default:
		break;
	}

	return 0;
}

////////////////////////////////// 20120524	
void CScreen_Main::OnMain_BCR_DISPLAY(int n_state)
{
	CString str_yield, str_yield2, str_yield3, str_yield4;

	switch(n_state)
	{
		case 0:
			str_yield.Format("%d", st_handler.mn_vision_yield_yes[0]);
			((CEdit*)GetDlgItem(IDC_EDIT_VISION_YIELD_YES))->SetWindowText(str_yield);
			str_yield2.Format("%d", st_handler.mn_vision_yield_yes[1]);
			((CEdit*)GetDlgItem(IDC_EDIT_VISION_YIELD_YES2))->SetWindowText(str_yield2);
			st_handler.mn_vision_yield_yes[0] = 0;
			st_handler.mn_vision_yield_yes[1] = 0;
			break;
		case 1:	
			str_yield3.Format("%d", st_handler.mn_hs_yield_yes[0]);
			((CEdit*)GetDlgItem(IDC_EDIT_HS_YIELD_YES))->SetWindowText(str_yield3);
			str_yield4.Format("%d", st_handler.mn_hs_yield_yes[1]);
			((CEdit*)GetDlgItem(IDC_EDIT_HS_YIELD_YES2))->SetWindowText(str_yield4);
			st_handler.mn_hs_yield_yes[0] = 0;
			st_handler.mn_hs_yield_yes[1] = 0;
			break;

		case 3:
			mstr_bcr_id[1] = st_handler.mstr_bcr_id;
			((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID))->SetWindowText(mstr_bcr_id[1]);
			break;
		case 4:
			mstr_bcr_id2[1] = st_handler.mstr_bcr_id2;
			((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID2))->SetWindowText(mstr_bcr_id2[1]);
			break;
		case 5:
			mstr_bcr_id3[1] = st_handler.mstr_bcr_id3;
			((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID3))->SetWindowText(mstr_bcr_id3[1]);
			break;
		case 6:
			mstr_bcr_id4[1] = st_handler.mstr_bcr_id4;
			((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID4))->SetWindowText(mstr_bcr_id4[1]);
			break;
		case 7:
			mstr_bcr_id_1[1] = st_handler.mstr_bcr_id_1;
			((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID_1))->SetWindowText(mstr_bcr_id_1[1]);
			break;
		case 8:
			mstr_bcr_id2_1[1] = st_handler.mstr_bcr_id2_1;
			((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID2_1))->SetWindowText(mstr_bcr_id2_1[1]);
			break;
		case 9:
			mstr_bcr_id3_1[1] = st_handler.mstr_bcr_id3_1;
			((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID3_1))->SetWindowText(mstr_bcr_id3_1[1]);
			break;
		case 10:
			mstr_bcr_id4_1[1] = st_handler.mstr_bcr_id4_1;
			((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID4_1))->SetWindowText(mstr_bcr_id4_1[1]);
			break;//////////////////////////////////////////

	}
}
//////////////////////////////////

void CScreen_Main::OnBtnDoorOpen() 
{
 	int nRet, nRet2;
 
 	//Func.OnSet_IO_Port_Stop();
 	
 	nRet = FAS_IO.get_in_bit(st_io.i_front_select_switch_chk, IO_OFF);
 	nRet2 = FAS_IO.get_in_bit(st_io.i_rear_select_switch_chk, IO_OFF);
 
 	if (nRet == IO_ON)//&& nRet2 == IO_ON)
 	{
 		sprintf(st_msg.c_abnormal_msg, "Manual Mode가 아닙니다.");
 		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
 		{
 			sprintf(st_msg.c_abnormal_msg, "Manual Mode is not");
 		}
 
 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
 		return;
 	}
 
 	if (st_work.mn_run_status != dSTOP)
 	{
 		sprintf(st_msg.c_abnormal_msg, "장비가 Stop 상태가 아닙니다.");
 		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
 		{
 			sprintf(st_msg.c_abnormal_msg, "Stop the equipment is not state");
 		}
 
 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
 		return ;
 	} 
 
 	// **************************************************************************
 	if (st_handler.mn_level_teach !=  TRUE)
 	{
 		sprintf(st_msg.c_abnormal_msg, "레벨2 가 아닙니다.");
 		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
 		{
 			sprintf(st_msg.c_abnormal_msg, "Level 2 is not");
 		}
 
 		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
 		return;
 	}
 
 	FAS_IO.set_out_bit(st_io.o_lock_door_onoff, IO_OFF);
}

void CScreen_Main::OnMain_Display_Lot_Info()
{

	GridData(IDC_CUSTOM_LOT_INFO, 1, 2, st_NW.mstr_lot_id[1]);//st_handler.str_main_lot);20120913

// 	if (st_handler.str_main_part.GetLength() >= 16)
// 	{
// 		GridData(IDC_CUSTOM_LOT_INFO, 2, 2, st_NW.mstr_part_id[1]);
// 	}
// 	else
// 	{
	GridData(IDC_CUSTOM_LOT_INFO, 2, 2, st_NW.mstr_Recive_PartNo[0]);//st_NW.mstr_part_id[1]);20120913
// 	}
	GridData(IDC_CUSTOM_LOT_INFO, 3, 2, st_lamp.mstr_equip_id);

	GridData(IDC_CUSTOM_LOT_INFO, 5, 2, st_NW.mstr_qty[1]);

	
 	GridData(IDC_CUSTOM_LOT_INFO, 6, 2, st_basic.mstr_label_name.Mid(4,20));//20140403

}

void CScreen_Main::OnMain_Display_Lot_Yield_info()
{
	CString strTemp;
	// Prime
	strTemp.Format("%d", st_work.nMdlInputCount[0][0]);
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 2, 2, strTemp);		// Input
	strTemp.Format("%d", st_work.nMdlPassCount[0][0]);
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 3, 2, strTemp);		// Pass
	strTemp.Format("%d", st_work.nMdlRejectCount[0][0]);
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 4, 2, strTemp);		// Reject

//	strTemp.Format("%d", st_work.n_loadlot_count[LDMODULE_SITE]);
//	GridData(IDC_CUSTOM_LOT_INFO, 5, 2, strTemp);


	if (st_work.nMdlPassCount[0][0] == 0)
	{
		st_work.fMdlyield[0][0] = 0;
	}
	else
	{
		st_work.fMdlyield[0][0] = (double)((double)st_work.nMdlPassCount[0][0] / (double)((double)st_work.nMdlPassCount[0][0] + (double)st_work.nMdlRejectCount[0][0])) * (double)100;
	}

	strTemp.Format("%3.2f%%", st_work.fMdlyield[0][0]);
	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 5, 2, strTemp);		// Yield

	// Cum
//	strTemp.Format("%06d", st_work.nMdlInputCount[1][0]);
//	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 2, 3, strTemp);		// Input
//	strTemp.Format("%06d", st_work.nMdlPassCount[1][0]);
//	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 3, 3, strTemp);		// Pass
//	strTemp.Format("%06d", st_work.nMdlRejectCount[1][0]);
//	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 4, 3, strTemp);		// Reject

//	if (st_work.nMdlPassCount[1][0] == 0)
//	{
//		st_work.fMdlyield[1][0] = 0;
//	}
//	else
//	{
//		st_work.fMdlyield[1][0] = (double)((double)st_work.nMdlPassCount[1][0] / (double)((double)st_work.nMdlPassCount[1][0] + (double)st_work.nMdlRejectCount[1][0])) * (double)100;
//	}
//	
//	strTemp.Format("%3.2f%%", st_work.fMdlyield[1][0]);
//
//	GridData(IDC_CUSTOM_LOT_YIELD_INFO, 5, 3, strTemp);		// Yield
}	

void CScreen_Main::OnMain_Display_Daily_Yield_info()
{
	CString strTemp;
	// Prime
	strTemp.Format("%06d", st_work.nMdlInputCount[0][1]);
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 2, 2, strTemp);		// Input
	strTemp.Format("%06d", st_work.nMdlPassCount[0][1]);
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 3, 2, strTemp);		// Pass
	strTemp.Format("%06d", st_work.nMdlRejectCount[0][1]);
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 4, 2, strTemp);		// Reject
	
	if (st_work.nMdlPassCount[0][1] == 0)
	{
		st_work.fMdlyield[0][1] = 0;
	}
	else
	{
		st_work.fMdlyield[0][1] = (double)((double)st_work.nMdlPassCount[0][1] / (double)((double)st_work.nMdlPassCount[0][1] + (double)st_work.nMdlRejectCount[0][1])) * (double)100;
	}
	strTemp.Format("%3.2f%%", st_work.fMdlyield[0][1]);
	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 5, 2, strTemp);		// Yield
	
	// Retest
//	strTemp.Format("%06d", st_work.nMdlInputCount[1][1]);
//	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 2, 3, strTemp);		// Input
//	strTemp.Format("%06d", st_work.nMdlPassCount[1][1]);
//	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 3, 3, strTemp);		// Pass
//	strTemp.Format("%06d", st_work.nMdlRejectCount[1][1]);
//	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 4, 3, strTemp);		// Reject
//
//	if (st_work.nMdlPassCount[1][1] == 0)
//	{
//		st_work.fMdlyield[1][1] = 0;
//	}
//	else
//	{
//		st_work.fMdlyield[1][1] = (double)((double)st_work.nMdlPassCount[1][1] / (double)((double)st_work.nMdlPassCount[1][1] + (double)st_work.nMdlRejectCount[1][1])) * (double)100;
//	}
//	strTemp.Format("%3.2f%%", st_work.fMdlyield[1][1]);
//	GridData(IDC_CUSTOM_DAILY_YIELD_INFO, 5, 3, strTemp);		// Yield
}

void CScreen_Main::OnMain_Init_Grid()
{
	CRect area[2];
	int   x, y;
	int   width, height;
	int   cen_x, cen_y;
	int   i;
	int   row, col;
	CString strTmp;
	
	CString str_tmp[4] = {"Module", "Front", "Back", "Unload"};
	TSpread *grid[2]; 
	row = 4;
	col = 0;

	grid[0] = (TSpread*)GetDlgItem(IDC_CUSTOM_BUFFER_MOTOR);
	grid[1] = (TSpread*)GetDlgItem(IDC_CUSTOM_VISION_MOTOR);
	
	m_p_grid.GridReset(grid[0]);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(grid[0], TRUE);
	
	m_p_grid.GridRowHeader(grid[0], TRUE);
	m_p_grid.GridColHeader(grid[0], TRUE);
	m_p_grid.GridHorizontal(grid[0], FALSE);
	m_p_grid.GridVertical(grid[0], FALSE);
	m_p_grid.GridAutoSize(grid[0], FALSE);
	m_p_grid.GridAutoSize(grid[0], FALSE);
	m_p_grid.GridCellRows(grid[0], row-1);
	m_p_grid.GridCellCols(grid[0], col+1);
	
	grid[0]->GetClientRect(area[0]); // Grid pass rect 구하기 

	GetDlgItem(IDC_CUSTOM_BUFFER_MOTOR)->GetWindowRect(area[1]); // group box rect 구하기 
	ScreenToClient(area[1]); // window상에서 group box 좌표 구하기

	width		= area[0].Width();
	height		= area[0].Height();
	
	x			= width / (col + 1);
	y			= height / row-1;

	// group box에서 grid 센터에 표시하기 위한 x좌표 구하기 
	cen_x = area[1].left + ((area[1].Width() - (x*(col+1)+2)) / 2);
	// group box에서 grid 센터에 표시하기 위한 y좌표 구하기 
	cen_y = area[1].top + ((area[1].Height() - (y*(row+1)+2)) / 2) + 5;
	// grid size 계산하여 중심에 다시 그리기 
	//grid[0]->MoveWindow(cen_x, cen_y, x*(col+1)+3, y*(row+1)+3, TRUE);
	
	for(i=0; i<row; i++)
	{
		m_p_grid.GridCellHeight_P(grid[0], i, y);
		m_p_grid.GridCellWidth_P(grid[0], 0, x);
		m_p_grid.GridCellControlStatic(grid[0], i, 0);

		strTmp.Format("%d", 4-i);
		if(st_handler.n_lotend_ready != CTL_NO)
		{
			SetTimer(TM_LOTEND_READY,500, NULL);
			if(st_handler.n_module_retest[i] == CTL_YES)
			{
				m_p_grid.GridCellColor(grid[0], i, 0, RED_C, WHITE_C);
			}
			else
			{
				m_p_grid.GridCellColor(grid[0], i, 0, WHITE_C, BLUE_D);
			}
		}
		else
		{
			m_p_grid.GridCellColor(grid[0], i, 0, WHITE_C, BLUE_D);
		}
		m_p_grid.GridCellFont(grid[0], i, 0,"MS Sans Serif", 14); 
		//m_p_grid.GridCellText(grid[0], i, 0, strTmp);
		m_p_grid.GridCellText(grid[0], i, 0, str_tmp[i]);		
	}

	row = 2;
	col = 0;

	m_p_grid.GridReset(grid[1]);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(grid[1], TRUE);
	
	m_p_grid.GridRowHeader(grid[1], TRUE);
	m_p_grid.GridColHeader(grid[1], TRUE);
	m_p_grid.GridHorizontal(grid[1], FALSE);
	m_p_grid.GridVertical(grid[1], FALSE);
	m_p_grid.GridAutoSize(grid[1], FALSE);
	m_p_grid.GridAutoSize(grid[1], FALSE);
	m_p_grid.GridCellRows(grid[1], row-1);
	m_p_grid.GridCellCols(grid[1], col+1);
	
	grid[1]->GetClientRect(area[0]); // Grid pass rect 구하기 

	GetDlgItem(IDC_CUSTOM_BUFFER_MOTOR)->GetWindowRect(area[1]); // group box rect 구하기 
	ScreenToClient(area[1]); // window상에서 group box 좌표 구하기

	width		= area[0].Width();
	height		= area[0].Height();
	
	x			= width / (col + 1);
	y			= height / row-1;

	// group box에서 grid 센터에 표시하기 위한 x좌표 구하기 
	cen_x = area[1].left + ((area[1].Width() - (x*(col+1)+2)) / 2);
	// group box에서 grid 센터에 표시하기 위한 y좌표 구하기 
	cen_y = area[1].top + ((area[1].Height() - (y*(row+1)+2)) / 2) + 5;
	// grid size 계산하여 중심에 다시 그리기 
	//grid[0]->MoveWindow(cen_x, cen_y, x*(col+1)+3, y*(row+1)+3, TRUE);
	
	for(i=0; i<row; i++)
	{
		m_p_grid.GridCellHeight_P(grid[1], i, y);
		m_p_grid.GridCellWidth_P(grid[1], 0, x);
		m_p_grid.GridCellControlStatic(grid[1], i, 0);

		strTmp.Format("%d", 2-i);
		m_p_grid.GridCellColor(grid[1], i, 0, WHITE_C, BLUE_D);
		m_p_grid.GridCellFont(grid[1], i, 0,"MS Sans Serif", 14); 
		m_p_grid.GridCellText(grid[1], i, 0, strTmp);
	}
}

void CScreen_Main::OnMain_Display_VisionBuffer()
{
	int i = 0;
	TSpread *grid; 
	CString strTmp;

	grid = (TSpread*)GetDlgItem(IDC_CUSTOM_VISION_MOTOR);

	if(st_modulemap.VisionBuffer[D_EXIST][TOPBUFFER] == DVC_YES)
	{
		m_p_grid.GridCellColor(grid, 0, 0, GREEN_C, BLACK_C);
	}
	else
	{
		m_pGrid.GridCellColor(grid, 0, 0, WHITE_C, BLACK_C);
	}

	if(st_modulemap.VisionBuffer[D_EXIST][BTMBUFFER] == DVC_YES)
	{
		m_p_grid.GridCellColor(grid, 1, 0, GREEN_C, BLACK_C);
	}
	else
	{
		m_pGrid.GridCellColor(grid, 1, 0, WHITE_C, BLACK_C);
	}

}
void CScreen_Main::OnMain_Display_CycleTime_Info()
{
// 	GridData(IDC_CUSTOM_LOT_INFO, 5, 2, st_work.str_cycle_time);
}

void CScreen_Main::OnPadBcrId() 
{
	// TODO: Add your control notification handler code here
	int n_response1;  // 대화 상자 리턴 플래그

	CDialog_Keyboard  keyboard_dlg ;

	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID))->GetWindowText(mstr_bcr_id[1]) ;
	if (mstr_bcr_id[1].IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();          // 키보드 상자 출력 데이터 초기화
	}
	else 
	{
		mstr_bcr_id[1].MakeUpper();
		mstr_bcr_id[1].TrimLeft(' ');               
		mstr_bcr_id[1].TrimRight(' ');

		st_msg.mstr_typing_msg = mstr_bcr_id[1];  // 키보드 상자 출력 데이터 설정
	}

	n_response1 = keyboard_dlg.DoModal();
	if (n_response1 == IDOK)
	{
		mstr_bcr_id[1] = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정

		mstr_bcr_id[1].MakeUpper();
		mstr_bcr_id[1].TrimLeft(' ');               
		mstr_bcr_id[1].TrimRight(' ');
		((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID))->SetWindowText(mstr_bcr_id[1]);
	}	
	// **************************************************************************
	// 전역 변수에 설정한다                          
	// **************************************************************************
	st_handler.mstr_bcr_id = mstr_bcr_id[1];	//전역변수 저장 
	mstr_bcr_id[0] = mstr_bcr_id[1];	//backup

}

void CScreen_Main::OnPadBcrId2() 
{
	// TODO: Add your control notification handler code here
	int n_response2;  // 대화 상자 리턴 플래그

	CDialog_Keyboard  keyboard_dlg ;

	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID2))->GetWindowText(mstr_bcr_id2[1]) ;
	if (mstr_bcr_id2[1].IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();          // 키보드 상자 출력 데이터 초기화
	}
	else 
	{
		mstr_bcr_id2[1].MakeUpper();
		mstr_bcr_id2[1].TrimLeft(' ');               
		mstr_bcr_id2[1].TrimRight(' ');

		st_msg.mstr_typing_msg = mstr_bcr_id2[1];  // 키보드 상자 출력 데이터 설정
	}

	n_response2 = keyboard_dlg.DoModal();
	if (n_response2 == IDOK)
	{
		mstr_bcr_id2[1] = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정

		mstr_bcr_id2[1].MakeUpper();
		mstr_bcr_id2[1].TrimLeft(' ');               
		mstr_bcr_id2[1].TrimRight(' ');
		((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID2))->SetWindowText(mstr_bcr_id2[1]);
	}	


	// **************************************************************************
	// 전역 변수에 설정한다                          
	// **************************************************************************
	st_handler.mstr_bcr_id2 = mstr_bcr_id2[1];	//전역변수 저장 
	mstr_bcr_id2[0] = mstr_bcr_id2[1];	//backup
}

void CScreen_Main::OnPadBcrId3() 
{
	// TODO: Add your control notification handler code here
	int n_response3;  // 대화 상자 리턴 플래그

	CDialog_Keyboard  keyboard_dlg ;

	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID3))->GetWindowText(mstr_bcr_id3[1]) ;
	if (mstr_bcr_id3[1].IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();          // 키보드 상자 출력 데이터 초기화
	}
	else 
	{
		mstr_bcr_id3[1].MakeUpper();
		mstr_bcr_id3[1].TrimLeft(' ');               
		mstr_bcr_id3[1].TrimRight(' ');

		st_msg.mstr_typing_msg = mstr_bcr_id3[1];  // 키보드 상자 출력 데이터 설정
	}

	n_response3 = keyboard_dlg.DoModal();
	if (n_response3 == IDOK)
	{
		mstr_bcr_id3[1] = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정

		mstr_bcr_id3[1].MakeUpper();
		mstr_bcr_id3[1].TrimLeft(' ');               
		mstr_bcr_id3[1].TrimRight(' ');
		((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID3))->SetWindowText(mstr_bcr_id3[1]);
	}	


	// **************************************************************************
	// 전역 변수에 설정한다                          
	// **************************************************************************
	st_handler.mstr_bcr_id3 = mstr_bcr_id3[1];	//전역변수 저장 
	mstr_bcr_id3[0] = mstr_bcr_id3[1];	//backup	
	
}

void CScreen_Main::OnPadBcrId4() 
{
	// TODO: Add your control notification handler code here
	int n_response4;  // 대화 상자 리턴 플래그

	CDialog_Keyboard  keyboard_dlg ;

	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID4))->GetWindowText(mstr_bcr_id4[1]) ;
	if (mstr_bcr_id4[1].IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();          // 키보드 상자 출력 데이터 초기화
	}
	else 
	{
		mstr_bcr_id4[1].MakeUpper();
		mstr_bcr_id4[1].TrimLeft(' ');               
		mstr_bcr_id4[1].TrimRight(' ');

		st_msg.mstr_typing_msg = mstr_bcr_id4[1];  // 키보드 상자 출력 데이터 설정
	}

	n_response4 = keyboard_dlg.DoModal();
	if (n_response4 == IDOK)
	{
		mstr_bcr_id4[1] = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정

		mstr_bcr_id4[1].MakeUpper();
		mstr_bcr_id4[1].TrimLeft(' ');               
		mstr_bcr_id4[1].TrimRight(' ');
		((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID4))->SetWindowText(mstr_bcr_id4[1]);
	}
	// **************************************************************************
	// 전역 변수에 설정한다                          
	// **************************************************************************
	st_handler.mstr_bcr_id4 = mstr_bcr_id4[1];	//전역변수 저장 
	mstr_bcr_id4[0] = mstr_bcr_id4[1];	//backup
		
}

void CScreen_Main::OnSetfocusEditBcrId()
{
}

void CScreen_Main::OnSetfocusEditBcrId2()
{
}

void CScreen_Main::OnSetfocusEditBcrId3()
{
}

void CScreen_Main::OnSetfocusEditBcrId4()
{	
}

void CScreen_Main::OnMain_Data_Set()
{
	m_pad_bcr_id.SetIcon(IDI_KEYBOARD);
	m_pad_bcr_id.DrawBorder(FALSE);

	m_pad_bcr_id2.SetIcon(IDI_KEYBOARD);
	m_pad_bcr_id2.DrawBorder(FALSE);

	m_pad_bcr_id3.SetIcon(IDI_KEYBOARD);
	m_pad_bcr_id3.DrawBorder(FALSE);

	m_pad_bcr_id4.SetIcon(IDI_KEYBOARD);
	m_pad_bcr_id4.DrawBorder(FALSE);

	m_pad_bcr_id_1.SetIcon(IDI_KEYBOARD);
	m_pad_bcr_id_1.DrawBorder(FALSE);

	m_pad_bcr_id2_1.SetIcon(IDI_KEYBOARD);
	m_pad_bcr_id2_1.DrawBorder(FALSE);

	m_pad_bcr_id3_1.SetIcon(IDI_KEYBOARD);
	m_pad_bcr_id3_1.DrawBorder(FALSE);

	m_pad_bcr_id4_1.SetIcon(IDI_KEYBOARD);
	m_pad_bcr_id4_1.DrawBorder(FALSE);

	mstr_bcr_id[1] = st_handler.mstr_bcr_id;
	mstr_bcr_id2[1] = st_handler.mstr_bcr_id2;
	mstr_bcr_id3[1] = st_handler.mstr_bcr_id3;
	mstr_bcr_id4[1] = st_handler.mstr_bcr_id4;
	mstr_bcr_id_1[1] = st_handler.mstr_bcr_id_1;
	mstr_bcr_id2_1[1] = st_handler.mstr_bcr_id2_1;
	mstr_bcr_id3_1[1] = st_handler.mstr_bcr_id3_1;
	mstr_bcr_id4_1[1] = st_handler.mstr_bcr_id4_1;

	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID))->SetWindowText(mstr_bcr_id[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID2))->SetWindowText(mstr_bcr_id2[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID3))->SetWindowText(mstr_bcr_id3[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID4))->SetWindowText(mstr_bcr_id4[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID_1))->SetWindowText(mstr_bcr_id_1[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID2_1))->SetWindowText(mstr_bcr_id2_1[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID3_1))->SetWindowText(mstr_bcr_id3_1[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID4_1))->SetWindowText(mstr_bcr_id4_1[1]);
}
/////////////////////////////////////////////////////////////////

///////////////////////////////////////// 20120522
void CScreen_Main::OnBtnLotOpen()
{
	int n_response;  // 대화 상자 리턴 플래그

	CDialog_Lot_Open Lot_Open_dlg;

	n_response = Lot_Open_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		
	}
	st_lotdb.mstart_date = COleDateTime::GetCurrentTime();
	st_work.mn_lot_start = CTL_YES;
	st_work.m_dLotStartTime = GetCurrentTime();//2015.0128
	Func.InitNode();

}

/////////////////////////////////////////

/////////////////////////////////////////////// 20120704
void CScreen_Main::OnMain_CheckerCtrl_Set()
{
	register li;
	CRect rect_check_ctrl;
	int iTotalPocket;
	
// 	// =====================================================================================================================
// 	// =====================================================================================================================
// 	//  Loader Tray를 생성한다
// // 	// =====================================================================================================================
// 	GetDlgItem(IDC_STATIC_M_LOADER_TRAY)->GetWindowRect(rect_check_ctrl);
// 	ScreenToClient(rect_check_ctrl);
// 	st_handler.b_main_tray_display = FALSE;//20120710
// 
// 	m_ctrl_loader_tray.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_LOADER_TRAY);
// 	m_ctrl_loader_tray.SetTotalBlocks(1, st_basic.mn_lotset_module_load);
// 
// 	for(li = 0 ; li < st_basic.mn_lotset_module_load ; li++)
// 	{
// 		m_ctrl_loader_tray.SetBlock(li, GREEN_C);
// 	}
// 	m_ctrl_loader_tray.Refresh();
// 
// 
// 	// =====================================================================================================================
// 	// =====================================================================================================================
// 	//  HS Left Tray를 생성한다
// 	// =====================================================================================================================
// 	GetDlgItem(IDC_STATIC_M_HS_LEFT_TRAY)->GetWindowRect(rect_check_ctrl);
// 	ScreenToClient(rect_check_ctrl);
// 	st_handler.b_main_tray_display = FALSE;//20120710
// 
// 	m_ctrl_hs_left_tray.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_HS_LEFT_TRAY);
// 	m_ctrl_hs_left_tray.SetTotalBlocks(1, st_basic.mn_lotset_hs_left);
// 
// 	for(li = 0 ; li < st_basic.mn_lotset_hs_left ; li++)
// 	{
// 		m_ctrl_hs_left_tray.SetBlock(li, GREEN_C);
// 	}
// 	m_ctrl_hs_left_tray.Refresh();
// 
// 	// =====================================================================================================================
// 	// =====================================================================================================================
// 	//  HS Right Tray를 생성한다
// 	// =====================================================================================================================
// 	GetDlgItem(IDC_STATIC_M_HS_RIGHT_TRAY)->GetWindowRect(rect_check_ctrl);
// 	ScreenToClient(rect_check_ctrl);
// 	st_handler.b_main_tray_display = FALSE;//20120710
// 
// 	m_ctrl_hs_right_tray.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_HS_RIGHT_TRAY);
// 	m_ctrl_hs_right_tray.SetTotalBlocks(1, st_basic.mn_lotset_hs_right);
// 
// 	for(li = 0 ; li < st_basic.mn_lotset_hs_right ; li++)
// 	{
// 		m_ctrl_hs_right_tray.SetBlock(li, GREEN_C);
// 	}
// 	m_ctrl_hs_right_tray.Refresh();
// 
// 	// =====================================================================================================================
// 	// =====================================================================================================================
// 	//  Clip1를 생성한다
// 	// =====================================================================================================================
// 	GetDlgItem(IDC_STATIC_M_CLIP_TRAY)->GetWindowRect(rect_check_ctrl);
// 	ScreenToClient(rect_check_ctrl);
// 	st_handler.b_main_tray_display = TRUE;//20120710
// 
// 	m_ctrl_clip1_tray.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_CLIP_TRAY);
// 	m_ctrl_clip1_tray.SetTotalBlocks(1, st_basic.mn_lotset_clip);
// 
// 	for(li = 0 ; li < st_basic.mn_lotset_clip ; li++)
// 	{
// 		m_ctrl_clip1_tray.SetBlock(li, GREEN_C);
// 	}
// 	m_ctrl_clip1_tray.Refresh();
// 
// 
// 	// =====================================================================================================================
// 	// =====================================================================================================================
// 	//  Clip2를 생성한다
// 	// =====================================================================================================================
// 	GetDlgItem(IDC_STATIC_M_CLIP_TRAY2)->GetWindowRect(rect_check_ctrl);
// 	ScreenToClient(rect_check_ctrl);
// 	st_handler.b_main_tray_display = TRUE;//20120710
// 
// 	m_ctrl_clip2_tray.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_CLIP_TRAY2);
// 	m_ctrl_clip2_tray.SetTotalBlocks(1, st_basic.mn_lotset_clip);
// 
// 	for(li = 0 ; li < st_basic.mn_lotset_clip ; li++)
// 	{
// 		m_ctrl_clip2_tray.SetBlock(li, GREEN_C);
// 	}
// 	m_ctrl_clip2_tray.Refresh();
// 
// 
// 	// =====================================================================================================================
// 	// =====================================================================================================================
// 	//  Unloader Tray를 생성한다
// 	// =====================================================================================================================
// 	GetDlgItem(IDC_STATIC_M_UNLOADER_TRAY)->GetWindowRect(rect_check_ctrl);
// 	ScreenToClient(rect_check_ctrl);
// 	st_handler.b_main_tray_display = FALSE;//20120710
// 
// 	m_ctrl_unloader_tray.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_UNLOADER_TRAY);
// 	m_ctrl_unloader_tray.SetTotalBlocks(1, st_basic.mn_lotset_unloader);
// 
// 	for(li = 0 ; li < st_basic.mn_lotset_unloader ; li++)
// 	{
// 		m_ctrl_unloader_tray.SetBlock(li, GREEN_C);
// 	}
// 	m_ctrl_unloader_tray.Refresh();
// 
// 
// 	// =====================================================================================================================
// 	// =====================================================================================================================
// 	//  Reject를 생성한다
// 	// =====================================================================================================================
// 	GetDlgItem(IDC_STATIC_M_REJECT_TRAY)->GetWindowRect(rect_check_ctrl);
// 	ScreenToClient(rect_check_ctrl);
// 	st_handler.b_main_tray_display = FALSE;//20120710
// 
// 	m_ctrl_reject_tray.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_REJECT_TRAY);
// 	m_ctrl_reject_tray.SetTotalBlocks(1, st_basic.mn_lotset_reject);
// 
// 	for(li = 0 ; li < st_basic.mn_lotset_reject ; li++)
// 	{
// 		m_ctrl_reject_tray.SetBlock(li, GREEN_C);
// 	}
// 	m_ctrl_reject_tray.Refresh();

/////////////////////////////////// 20120707
	// =====================================================================================================================
	// =====================================================================================================================
	//  Loader Picker를 생성한다
	// =====================================================================================================================
	GetDlgItem(IDC_STATIC_M_LOADER_PICKER)->GetWindowRect(rect_check_ctrl);
	ScreenToClient(rect_check_ctrl);
	st_handler.b_main_tray_display = FALSE;//20120710

	m_ctrl_loader_picker.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_LOADER_PICKER);
	m_ctrl_loader_picker.SetTotalBlocks(1, 2);
	
	iTotalPocket = 1 * 2;

	for(li = 0 ; li < iTotalPocket ; li++)
	{
		m_ctrl_loader_picker.SetBlock(li, GREEN_C);
	}
	m_ctrl_loader_picker.Refresh();

	// =====================================================================================================================
	// =====================================================================================================================
	//  HS Left Picker를 생성한다
	// =====================================================================================================================
	GetDlgItem(IDC_STATIC_M_HS_LEFT_PICKER)->GetWindowRect(rect_check_ctrl);
	ScreenToClient(rect_check_ctrl);
	st_handler.b_main_tray_display = FALSE;//20120710

	m_ctrl_hs_left_picker.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_HS_LEFT_PICKER);
	m_ctrl_hs_left_picker.SetTotalBlocks(1, 2);
	
	iTotalPocket = 1 * 2;

	for(li = 0 ; li < iTotalPocket ; li++)
	{
		m_ctrl_hs_left_picker.SetBlock(li, GREEN_C);
	}
	m_ctrl_hs_left_picker.Refresh();

	// =====================================================================================================================
	// =====================================================================================================================
	//  HS Right Picker를 생성한다
	// =====================================================================================================================
	GetDlgItem(IDC_STATIC_M_HS_RIGHT_PICKER)->GetWindowRect(rect_check_ctrl);
	ScreenToClient(rect_check_ctrl);
	st_handler.b_main_tray_display = FALSE;//20120710

	m_ctrl_hs_right_picker.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_HS_RIGHT_PICKER);
	m_ctrl_hs_right_picker.SetTotalBlocks(1, 2);
	
	iTotalPocket = 1 * 2;

	for(li = 0 ; li < iTotalPocket ; li++)
	{
		m_ctrl_hs_right_picker.SetBlock(li, GREEN_C);
	}
	m_ctrl_hs_right_picker.Refresh();

	// =====================================================================================================================
	// =====================================================================================================================
	//  Clip Picker를 생성한다
	// =====================================================================================================================
	GetDlgItem(IDC_STATIC_M_CLIP_PICKER)->GetWindowRect(rect_check_ctrl);
	ScreenToClient(rect_check_ctrl);
	st_handler.b_main_tray_display = FALSE;//20120710

	m_ctrl_clip1_picker.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_CLIP_PICKER);
	m_ctrl_clip1_picker.SetTotalBlocks(2, 1);

	iTotalPocket = 2 * 1;
	
	for(li = 0 ; li < iTotalPocket ; li++)
	{
		m_ctrl_clip1_picker.SetBlock(li, GREEN_C);
	}
	m_ctrl_clip1_picker.Refresh();

	// =====================================================================================================================
	// =====================================================================================================================
	//  Sorter Picker를 생성한다
	// =====================================================================================================================
	GetDlgItem(IDC_STATIC_M_SORETR_PICKER)->GetWindowRect(rect_check_ctrl);
	ScreenToClient(rect_check_ctrl);
	st_handler.b_main_tray_display = FALSE;//20120710

	m_ctrl_sorter_picker.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_SORETR_PICKER);
	m_ctrl_sorter_picker.SetTotalBlocks(1, 1);
	
	for(li = 0 ; li < 1 ; li++)
	{
		m_ctrl_sorter_picker.SetBlock(li, GREEN_C);
	}
	m_ctrl_sorter_picker.Refresh();


	// =====================================================================================================================
	// =====================================================================================================================
	//  Unloader Picker를 생성한다
	// =====================================================================================================================
	GetDlgItem(IDC_STATIC_M_UNLOADER_PICKER)->GetWindowRect(rect_check_ctrl);
	ScreenToClient(rect_check_ctrl);
	st_handler.b_main_tray_display = FALSE;//20120710

	m_ctrl_unloader_picker.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect_check_ctrl, this, IDC_STATIC_M_UNLOADER_PICKER);
	m_ctrl_unloader_picker.SetTotalBlocks(1, 1);
	
	for(li = 0 ; li < 1 ; li++)
	{
		m_ctrl_unloader_picker.SetBlock(li, GREEN_C);
	}
	m_ctrl_unloader_picker.Refresh();

/////////////////////////////////// 
}

void CScreen_Main::OnMainWork_Tray_Display(int iSite)
{
// 	int li, max_cnt;//, i_tray_info[90];
// 	CRect rect_check_ctrl;

// 	switch(iSite)
// 	{
// 	case MOT_LOADER_TRAY:	
// 		max_cnt = st_traybuffer[LDMODULE_SITE].i_loader_row;
// 		for(li = 0 ; li < max_cnt ; li++)
// 		{
// 			switch(st_modulemap.LoadTray[1][li])
// 			{
// 			case DVC_NO:			// Tray 없슴.
// 				m_ctrl_loader_tray.SetBlock(max_cnt - li -1, WHITE_C);
// 				break;
// 
// 			case DVC_YES:		// Tray Ready.
// 				m_ctrl_loader_tray.SetBlock(max_cnt - li -1, RGB(50, 255, 50));
// 				break;
// 
// 			default:
// 				m_ctrl_loader_tray.SetBlock(max_cnt - li -1, RGB(50, 50, 50));
// 				break;
// 
// 			}
// 		}
// 		m_ctrl_loader_tray.Refresh();
// 		break;
// 
// 	case MOT_HS_LEFT_TRAY:
// 		max_cnt = st_traybuffer[LEFTSINK_SITE].i_loader_row;
// 		for(li = 0 ; li < max_cnt ; li++)
// 		{
// 			switch(st_modulemap.LefthsTray[1][li])
// 			{
// 			case DVC_NO:			// Tray 없슴.
// 				m_ctrl_hs_left_tray.SetBlock(max_cnt - li -1, WHITE_C);
// 				break;
// 
// 			case DVC_YES:		// Tray Ready.
// 				m_ctrl_hs_left_tray.SetBlock(max_cnt - li -1, RGB(50, 255, 50));
// 				break;
// 
// 			default:
// 				m_ctrl_loader_tray.SetBlock(max_cnt - li -1, RGB(50, 50, 50));
// 				break;
// 			}
// 		}
// 		m_ctrl_hs_left_tray.Refresh();
// 		break;
// 
// 	case MOT_HS_RIGHT_TRAY:
// 		max_cnt = st_traybuffer[RIGHTSINK_SITE].i_loader_row;
// 		for(li = 0 ; li < max_cnt ; li++)
// 		{
// 			switch(st_modulemap.RighthsTray[1][li])
// 			{
// 			case DVC_NO:			// Tray 없슴.
// 				m_ctrl_hs_right_tray.SetBlock(max_cnt - li -1, WHITE_C);
// 				break;
// 
// 			case DVC_YES:		// Tray Ready.
// 				m_ctrl_hs_right_tray.SetBlock(max_cnt - li -1, RGB(50, 255, 50));
// 				break;
// 
// 			default:
// 				m_ctrl_hs_right_tray.SetBlock(max_cnt - li -1, RGB(50, 50, 50));
// 				break;
// 			}
// 		}
// 		m_ctrl_hs_right_tray.Refresh();
// 		break;
// 
// 	case MOT_CLIP1_TRAY:
// 		max_cnt = st_traybuffer[LDCLIP_SITE].i_loader_row;
// 		for(li = 0 ; li < max_cnt ; li++)
// 		{
// 			switch(st_modulemap.ClipTray[0][li])
// 			{
// 			case DVC_NO:			// Tray 없슴.
// 				m_ctrl_clip1_tray.SetBlock(max_cnt - li -1, WHITE_C);
// 				break;
// 
// 			case DVC_YES:		// Tray Ready.
// 				m_ctrl_clip1_tray.SetBlock(max_cnt - li -1, RGB(50, 255, 50));
// 				break;
// 
// 			default:
// 				m_ctrl_clip1_tray.SetBlock(max_cnt - li -1, RGB(50, 50, 50));
// 				break;
// 			}
// 		}
// 		m_ctrl_clip1_tray.Refresh();
// 		break;
// 
// 	case MOT_CLIP2_TRAY:
// 		max_cnt = st_traybuffer[LDCLIP_SITE].i_loader_row;
// 		for(li = 0 ; li < max_cnt ; li++)
// 		{
// 			switch(st_modulemap.ClipTray[1][li])
// 			{
// 			case DVC_NO:			// Tray 없슴.
// 				m_ctrl_clip2_tray.SetBlock(max_cnt - li -1, WHITE_C);
// 				break;
// 
// 			case DVC_YES:		// Tray Ready.
// 				m_ctrl_clip2_tray.SetBlock(max_cnt - li -1, RGB(50, 255, 50));
// 				break;
// 
// 			default:
// 				m_ctrl_clip2_tray.SetBlock(max_cnt - li -1, RGB(50, 50, 50));
// 				break;
// 			}
// 		}
// 		m_ctrl_clip2_tray.Refresh();
// 		break;
// 
// 	case MOT_UNLOADER_TRAY:
// 		max_cnt = st_traybuffer[UNLOADER_SITE].i_loader_row;
// 		for(li = 0 ; li < max_cnt ; li++)
// 		{
// 			switch(st_modulemap.UnloadTray[1][li])
// 			{
// 			case DVC_NO:			// Tray 없슴.
// 				m_ctrl_unloader_tray.SetBlock(max_cnt - li -1, WHITE_C);
// 				break;
// 
// 			case DVC_YES:		// Tray Ready.
// 				m_ctrl_unloader_tray.SetBlock(max_cnt - li -1, RGB(50, 50, 255));
// 				break;
// 
// 			default:
// 				m_ctrl_unloader_tray.SetBlock(max_cnt - li -1, RGB(50, 50, 50));
// 				break;
// 			}
// 		}
// 		m_ctrl_unloader_tray.Refresh();
// 		break;
// 
// 	case MOT_REJECT_TRAY:		
// 		max_cnt = st_traybuffer[REJECT_SITE].i_loader_row;
// 		for(li = 0 ; li < max_cnt ; li++)
// 		{
// 			switch(st_modulemap.RejectTray[li])
// 			{
// 			case DVC_NO:			// Tray 없슴.
// 				m_ctrl_reject_tray.SetBlock(max_cnt - li -1, WHITE_C);
// 				break;
// 
// 			case DVC_YES:		// Tray Ready.
// 				m_ctrl_reject_tray.SetBlock(max_cnt - li -1, RGB(255, 50, 50));
// 				break;
// 				
// 			default:
// 				m_ctrl_reject_tray.SetBlock(max_cnt - li -1, RGB(50, 50, 50));
// 				break;
// 			}
// 		}
// 		m_ctrl_reject_tray.Refresh();
// 		break;
// 	}
}
///////////////////////////////////////////////////////////////// 20120707
void CScreen_Main::OnMainWork_Picker_Display(int iSite)
{
	int li;

	switch(iSite)
	{
		case MOT_LOADER_PICKER:
			for(li = 0 ; li < 2; li++)
			{
				switch(st_modulemap.nModuleRobotPicker[D_EXIST][li])
				{
				case DVC_NO:			// Tray 없슴.
					m_ctrl_loader_picker.SetBlock(li, WHITE_C);
					break;

				case DVC_YES:		// Tray Ready.
					m_ctrl_loader_picker.SetBlock(li, RGB(50, 255, 50));
					break;
				}
			}
		break;

		case MOT_HS_LEFT_PICKER:
			for(li = 0 ; li < 2; li++)
			{
				switch(st_modulemap.nLefthsRobotPicker[D_EXIST][li])
				{
				case DVC_NO:			// Tray 없슴.
					m_ctrl_hs_left_picker.SetBlock(li, WHITE_C);
					break;

				case DVC_YES:		// Tray Ready.
					m_ctrl_hs_left_picker.SetBlock(li, RGB(50, 255, 50));
					break;
				}
			}
		break;

		case MOT_HS_RIGHT_PICKER:
			for(li = 0 ; li < 2; li++)
			{
				switch(st_modulemap.nRighthsRobotPicker[D_EXIST][li])
				{
				case DVC_NO:			// Tray 없슴.
					m_ctrl_hs_right_picker.SetBlock(li, WHITE_C);
					break;

				case DVC_YES:		// Tray Ready.
					m_ctrl_hs_right_picker.SetBlock(li, RGB(50, 255, 50));
					break;

				}
			}
		break;


		case MOT_CLIP1_PICKER:
			for(li = 0 ; li < 2; li++)
			{
				switch(st_modulemap.nClipRobotPicker[D_EXIST])
				{
				case DVC_NO:			// Tray 없슴.
					m_ctrl_clip1_picker.SetBlock(li, WHITE_C);
					break;

				case DVC_YES:		// Tray Ready.
					m_ctrl_clip1_picker.SetBlock(li, RGB(50, 255, 50));
					break;
				}
			}
			break;

		case MOT_CLIP2_PICKER:
// 			switch(st_modulemap.nClipRobotPicker[D_EXIST])
// 			{
// 			case PICKER_EMPTY:			// Tray 없슴.
// 				m_ctrl_clip1_picker.SetBlock(0, WHITE_C);
// 				break;
// 
// 			case PICKER_CHECK1:		// Tray Ready.
// 				m_ctrl_clip1_picker.SetBlock(0, GREEN_C);
// 				break;
// 
// // 			case PICKER_CHECK2:			// Tray Full.
// // 				m_ctrl_clip1_picker.SetBlock(0, GREEN_C);
// // 				break;
// 
// 			case PICKER_FULL:		// Tray Empty.
// 				m_ctrl_clip1_picker.SetBlock(0, GREEN_C);
// 				break;
// 				
// // 				default:
// // 					m_ctrl_clip1_picker.SetBlock(0, GREEN_C);
// // 					break;
// 			}
// 		m_ctrl_clip1_picker.Refresh();
		break;


		case MOT_SORTER_PICKER:
			switch(st_modulemap.nSortRobotPicker[D_EXIST])
			{
			case DVC_NO:			// Tray 없슴.
				m_ctrl_sorter_picker.SetBlock(0, WHITE_C);
				break;

			case DVC_YES:		// Tray Ready.
				m_ctrl_sorter_picker.SetBlock(0, RGB(50, 255, 50));
				break;
			}
		break;

		case MOT_UNLOADER_PICKER:
			switch(st_modulemap.nUldRobotPicker[D_EXIST])
			{
			case DVC_NO:			// Tray 없슴.
				m_ctrl_unloader_picker.SetBlock(0, WHITE_C);
				break;

			case DVC_YES:		// Tray Ready.
				if(st_modulemap.nUldRobotPicker[D_BIN] == BIN_GOODBIN)
				{
					m_ctrl_unloader_picker.SetBlock(0, RGB(50, 50, 255));
				}
				else
				{
					m_ctrl_unloader_picker.SetBlock(0, RGB(255, 50, 50));
				}
				break;

			}
		break;



///////////////////////////////////////
	}
}

#include "Dialog_Pass_Check.h"

////////////////////////// 20120711
void CScreen_Main::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CDialog_Worker dlg;

	CString strTmp;
	double	m_d_data;
	int n_response;

	if(wParam == IDC_CUSTOM_BUFFER_MOTOR)
	{
//		if(st_handler.n_lotend_ready != CTL_NO)
//		{
//			if(lpcc->Row == 1)//Module
//			{
//				mn_stacker_Step = 0;
//				mn_motor_site = LDMODULE_SITE;
//				SetTimer(TM_LOTEND_MODULE_READY, 100, NULL);
//			}
//			else if(lpcc->Row == 2)//Front
//			{
//				mn_motor_site = LEFTSINK_SITE;
//				SetTimer(TM_LOTEND_FRONTHS_READY, 100, NULL);
//			}
//			else if(lpcc->Row == 3)//Rear
//			{
//				mn_motor_site = RIGHTSINK_SITE;
//				SetTimer(TM_LOTEND_BACKTHS_READY, 100, NULL);
//			}
//			else if(lpcc->Row == 4)//Uld Module
//			{
//				mn_motor_site = UNLOADER_SITE;
//				SetTimer(TM_LOTEND_ULDMODULE_READY, 100, NULL);
//			}
//			else
//			{
//				mn_motor_site = -1;
//			}
//		}
	}
	else if(wParam == IDC_CUSTOM_LOT_INFO)
	{
        if(lpcc->Row == 4)
		{
			dlg.m_str_name				= st_work.str_worker_name;
			dlg.m_str_crem				= st_work.str_worker_crem;
			dlg.m_str_no				= st_work.mstr_op_id;
			
			dlg.DoModal();
			if (st_other.b_m_exit != TRUE)
			{
				st_work.str_worker_name		= dlg.m_str_name;
				st_work.str_worker_crem		= dlg.m_str_crem;
				st_work.mstr_op_id	= dlg.m_str_no;
				
				m_grid_lot = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
//				m_p_grid.GridCellText(m_grid_lot, 4, 2, st_work.str_worker_name + "(" + st_work.mstr_op_id + ")");
				m_p_grid.GridCellText(m_grid_lot, 4, 2, st_work.mstr_op_id);

			}
			else if (st_other.b_m_exit == TRUE)
			{
				st_other.b_m_exit = FALSE;
				dlg.m_str_name				= st_work.str_worker_name;
				dlg.m_str_crem				= st_work.str_worker_crem;
				dlg.m_str_no				= st_work.mstr_op_id;

			}
		}
	}
	else if(wParam == IDC_CUSTOM_LOT_YIELD_INFO)
	{
		CDialog_KeyPad	pad_dlg;
		m_grid_lot = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_YIELD_INFO);
		
		if(lpcc->Col == 2 && lpcc->Row == 2)//Input
		{
			strTmp.Format("%d", st_work.nMdlInputCount[0][0]);
			st_msg.mstr_keypad_msg.Format("[InPut Data]  set");
			st_msg.mstr_keypad_val = strTmp;
			
			st_msg.mstr_pad_high_limit.Format("100000");
			st_msg.mstr_pad_low_limit.Format("0");
			
			st_msg.mn_dot_use = CTL_YES;
			st_msg.mn_pad_dialog = CTL_YES;
			
			n_response = pad_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				strTmp = st_msg.mstr_keypad_val;
				m_d_data = atoi(strTmp);
				st_work.nMdlInputCount[0][0] = m_d_data;
				m_p_grid.GridCellText(m_grid_lot, lpcc->Row, lpcc->Col, strTmp);
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg,"[LOT_YIELD_INFO] Input Cpunt : %d",st_work.nMdlInputCount[0][0]);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					Func.On_LogFile_Add(99, st_msg.c_normal_msg);
				}
			}
			else if (n_response == IDCANCEL)
			{
				
			}
		}
		else if(lpcc->Col == 2 && lpcc->Row == 3)//Pass
		{
			//2015.0131
			st_handler.mn_level_teach =  FALSE;
			st_handler.mn_level_maint =  FALSE;
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE) 
				{
					st_handler.mstr_pass_level = "LevelChange";
					
					CDialog_Pass_Check pass_dlg;
					
					n_response = pass_dlg.DoModal();
					
					if (n_response == IDOK)
					{
						
					}
					else if (n_response == IDCANCEL)
					{
						
					}
					if (st_handler.mn_level_teach !=  TRUE)
					{
						if (st_handler.mn_level_maint !=  TRUE)  return;
					}
				}
			}

			strTmp.Format("%d", st_work.nMdlPassCount[0][0]);
			st_msg.mstr_keypad_msg.Format("[Good Data]  set");
			st_msg.mstr_keypad_val = strTmp;
			
			st_msg.mstr_pad_high_limit.Format("100000");
			st_msg.mstr_pad_low_limit.Format("0");
			
			st_msg.mn_dot_use = CTL_YES;
			st_msg.mn_pad_dialog = CTL_YES;
			
			n_response = pad_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				strTmp = st_msg.mstr_keypad_val;
				m_d_data = atoi(strTmp);
				st_work.nMdlPassCount[0][0] = m_d_data;
				m_p_grid.GridCellText(m_grid_lot, lpcc->Row, lpcc->Col, strTmp);
				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg,"[LOT_YIELD_INFO] Pass Cpunt : %d",st_work.nMdlPassCount[0][0]);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					Func.On_LogFile_Add(99, st_msg.c_normal_msg);
				}
			}
			else if (n_response == IDCANCEL)
			{
				
			}
		}
		else if(lpcc->Col == 2 && lpcc->Row == 4)//Fail
		{
			//2015.0131
			st_handler.mn_level_teach =  FALSE;
			st_handler.mn_level_maint =  FALSE;
			if (st_handler.mn_level_teach !=  TRUE)
			{
				if (st_handler.mn_level_maint !=  TRUE) 
				{
					st_handler.mstr_pass_level = "LevelChange";
					
					CDialog_Pass_Check pass_dlg;
					
					n_response = pass_dlg.DoModal();
					
					if (n_response == IDOK)
					{
						
					}
					else if (n_response == IDCANCEL)
					{
						
					}
					if (st_handler.mn_level_teach !=  TRUE)
					{
						if (st_handler.mn_level_maint !=  TRUE)  return;
					}
				}
			}
			strTmp.Format("%d", st_work.nMdlRejectCount[0][0]);
			st_msg.mstr_keypad_msg.Format("[Reject Data]  set");
			st_msg.mstr_keypad_val = strTmp;
			
			st_msg.mstr_pad_high_limit.Format("100000");
			st_msg.mstr_pad_low_limit.Format("0");
			
			st_msg.mn_dot_use = CTL_YES;
			st_msg.mn_pad_dialog = CTL_YES;
			
			n_response = pad_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				strTmp = st_msg.mstr_keypad_val;
				m_d_data = atoi(strTmp);
				st_work.nMdlRejectCount[0][0] = m_d_data;
				m_p_grid.GridCellText(m_grid_lot, lpcc->Row, lpcc->Col, strTmp);

				if(st_handler.cwnd_list != NULL)
				{
					sprintf(st_msg.c_normal_msg,"[LOT_YIELD_INFO] Fail Cpunt : %d",st_work.nMdlRejectCount[0][0]);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
					Func.On_LogFile_Add(99, st_msg.c_normal_msg);
				}

			}
			else if (n_response == IDCANCEL)
			{
				
			}


		}	
	}
	else if(wParam == IDC_CUSTOM_BUFFER_MOTOR)
	{

	}
	else if(wParam == IDC_CUSTOM_VISION_MOTOR)
	{
	}
	
}

void CScreen_Main::Init_Grid_Lot()
{
//	int   i, j;
	int	  max_row, max_col;
	CString str_tmp;
	
	max_row = 6;//20140403
	max_col = 2;
	
	m_grid_lot = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
	
// 	m_p_grid.GridReset(m_grid_lot);
// 	// 대문자 
// 	m_p_grid.GridCellSelectDisplay(m_grid_lot, TRUE);
// 	m_p_grid.GridRowHeader(m_grid_lot, FALSE);
// 	m_p_grid.GridColHeader(m_grid_lot, FALSE);
// 	m_p_grid.GridHorizontal(m_grid_lot, FALSE);
// 	m_p_grid.GridVertical(m_grid_lot, FALSE);
// 	m_p_grid.GridAutoSize(m_grid_lot, FALSE);
// 	m_p_grid.GridAutoSize(m_grid_lot, FALSE);
// 	m_p_grid.GridCellRows(m_grid_lot, max_row);
// 	m_p_grid.GridCellCols(m_grid_lot, max_col);
// 	
// 	for(i=0; i<max_row+1; i++)
// 	{
// 		m_p_grid.GridCellHeight_L(m_grid_lot, i, 23.5);
// 		
// 		for(j=0; j<max_col; j++)
// 		{
// 			switch(j)
// 			{
// 				case 0:
// 					m_p_grid.GridCellWidth_L(m_grid_lot, j+1, 15);
// 					break;
// 					
// 				case 1:
// 					m_p_grid.GridCellWidth_L(m_grid_lot, j+1, 15);
// 					break;
// 			}
// 			
// 			m_p_grid.GridCellControlStatic(m_grid_lot, i, j+1);
// 		}
// 	}
// 	
// 	m_p_grid.GridCellMerge(m_grid_lot, 5, 1, 1, 2);
// 	m_p_grid.GridCellFont(m_grid_lot, 5, 1, "MS Sans Serif", 16);
// 	m_p_grid.GridCellColor(m_grid_lot, 5, 1, BLUE_D, WHITE_C);
// 	m_p_grid.GridCellText(m_grid_lot, 5, 1, "Worker");
	
	m_p_grid.GridCellMerge(m_grid_lot, 4, 2, 1, 2);
	m_p_grid.GridCellFont(m_grid_lot, 4, 2, "MS Sans Serif", 18);
// 	m_p_grid.GridCellColor(m_grid_lot, 4, 2, YELLOW_L, BLACK_C);
	m_p_grid.GridCellText(m_grid_lot, 4, 2, st_work.mstr_op_id);
//	m_p_grid.GridCellText(m_grid_lot, 4, 2, st_work.str_worker_name + "(" + st_work.mstr_op_id + ")");
	m_p_grid.GridCellText(m_grid_lot, 5, 2, st_NW.mstr_qty[1]);//20120913

 	m_p_grid.GridCellText(m_grid_lot, 6, 2, st_basic.mstr_label_name.Mid(4,20));//20140403
}


void CScreen_Main::OnBtnWorkInformation() 
{
	if(st_work.mn_run_status == dSTOP)
	{
		CDialog_Work_Information dlg;
		dlg.DoModal();
	}
	
}

void CScreen_Main::OnPadBcrId1() 
{
	// TODO: Add your control notification handler code here
	int n_response1;  // 대화 상자 리턴 플래그

	CDialog_Keyboard  keyboard_dlg ;

	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID_1))->GetWindowText(mstr_bcr_id_1[1]) ;
	if (mstr_bcr_id_1[1].IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();          // 키보드 상자 출력 데이터 초기화
	}
	else 
	{
		mstr_bcr_id_1[1].MakeUpper();
		mstr_bcr_id_1[1].TrimLeft(' ');               
		mstr_bcr_id_1[1].TrimRight(' ');

		st_msg.mstr_typing_msg = mstr_bcr_id_1[1];  // 키보드 상자 출력 데이터 설정
	}

	n_response1 = keyboard_dlg.DoModal();
	if (n_response1 == IDOK)
	{
		mstr_bcr_id_1[1] = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정

		mstr_bcr_id_1[1].MakeUpper();
		mstr_bcr_id_1[1].TrimLeft(' ');               
		mstr_bcr_id_1[1].TrimRight(' ');
		((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID_1))->SetWindowText(mstr_bcr_id_1[1]);
	}	
	// **************************************************************************
	// 전역 변수에 설정한다                          
	// **************************************************************************
	st_handler.mstr_bcr_id_1 = mstr_bcr_id_1[1];	//전역변수 저장 
	mstr_bcr_id_1[0] = mstr_bcr_id_1[1];	//backup	
}

void CScreen_Main::OnPadBcrId21() 
{
	// TODO: Add your control notification handler code here
	int n_response2;  // 대화 상자 리턴 플래그

	CDialog_Keyboard  keyboard_dlg ;

	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID2_1))->GetWindowText(mstr_bcr_id2_1[1]) ;
	if (mstr_bcr_id2_1[1].IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();          // 키보드 상자 출력 데이터 초기화
	}
	else 
	{
		mstr_bcr_id2_1[1].MakeUpper();
		mstr_bcr_id2_1[1].TrimLeft(' ');               
		mstr_bcr_id2_1[1].TrimRight(' ');

		st_msg.mstr_typing_msg = mstr_bcr_id2_1[1];  // 키보드 상자 출력 데이터 설정
	}

	n_response2 = keyboard_dlg.DoModal();
	if (n_response2 == IDOK)
	{
		mstr_bcr_id2_1[1] = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정

		mstr_bcr_id2_1[1].MakeUpper();
		mstr_bcr_id2_1[1].TrimLeft(' ');               
		mstr_bcr_id2_1[1].TrimRight(' ');
		((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID2_1))->SetWindowText(mstr_bcr_id2_1[1]);
	}	


	// **************************************************************************
	// 전역 변수에 설정한다                          
	// **************************************************************************
	st_handler.mstr_bcr_id2_1 = mstr_bcr_id2_1[1];	//전역변수 저장 
	mstr_bcr_id2_1[0] = mstr_bcr_id2_1[1];	//backup
	
}

void CScreen_Main::OnPadBcrId31() 
{
	// TODO: Add your control notification handler code here
	int n_response3;  // 대화 상자 리턴 플래그

	CDialog_Keyboard  keyboard_dlg ;

	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID3_1))->GetWindowText(mstr_bcr_id3_1[1]) ;
	if (mstr_bcr_id3_1[1].IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();          // 키보드 상자 출력 데이터 초기화
	}
	else 
	{
		mstr_bcr_id3_1[1].MakeUpper();
		mstr_bcr_id3_1[1].TrimLeft(' ');               
		mstr_bcr_id3_1[1].TrimRight(' ');

		st_msg.mstr_typing_msg = mstr_bcr_id3_1[1];  // 키보드 상자 출력 데이터 설정
	}

	n_response3 = keyboard_dlg.DoModal();
	if (n_response3 == IDOK)
	{
		mstr_bcr_id3_1[1] = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정

		mstr_bcr_id3_1[1].MakeUpper();
		mstr_bcr_id3_1[1].TrimLeft(' ');               
		mstr_bcr_id3_1[1].TrimRight(' ');
		((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID3_1))->SetWindowText(mstr_bcr_id3_1[1]);
	}	


	// **************************************************************************
	// 전역 변수에 설정한다                          
	// **************************************************************************
	st_handler.mstr_bcr_id3_1 = mstr_bcr_id3_1[1];	//전역변수 저장 
	mstr_bcr_id3_1[0] = mstr_bcr_id3_1[1];	//backup	
	
}

void CScreen_Main::OnPadBcrId41() 
{
	// TODO: Add your control notification handler code here
	int n_response4;  // 대화 상자 리턴 플래그

	CDialog_Keyboard  keyboard_dlg ;

	((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID4_1))->GetWindowText(mstr_bcr_id4_1[1]) ;
	if (mstr_bcr_id4_1[1].IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();          // 키보드 상자 출력 데이터 초기화
	}
	else 
	{
		mstr_bcr_id4_1[1].MakeUpper();
		mstr_bcr_id4_1[1].TrimLeft(' ');               
		mstr_bcr_id4_1[1].TrimRight(' ');

		st_msg.mstr_typing_msg = mstr_bcr_id4_1[1];  // 키보드 상자 출력 데이터 설정
	}

	n_response4 = keyboard_dlg.DoModal();
	if (n_response4 == IDOK)
	{
		mstr_bcr_id4_1[1] = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정

		mstr_bcr_id4_1[1].MakeUpper();
		mstr_bcr_id4_1[1].TrimLeft(' ');               
		mstr_bcr_id4_1[1].TrimRight(' ');
		((CEdit*)GetDlgItem(IDC_EDIT_BCR_ID4_1))->SetWindowText(mstr_bcr_id4_1[1]);
	}
	// **************************************************************************
	// 전역 변수에 설정한다                          
	// **************************************************************************
	st_handler.mstr_bcr_id4_1 = mstr_bcr_id4_1[1];	//전역변수 저장 
	mstr_bcr_id4_1[0] = mstr_bcr_id4_1[1];	//backup	
}

void CScreen_Main::OnSetfocusEditBcrId_1()
{
}

void CScreen_Main::OnSetfocusEditBcrId2_1()
{
}

void CScreen_Main::OnSetfocusEditBcrId3_1()
{
}

void CScreen_Main::OnSetfocusEditBcrId4_1()
{	
}

void CScreen_Main::OnMainWork_RejectTray_Change()
{
//	int nResponse;
//	CDialog_Message  msg_dlg;

//	Func.OnSet_IO_Port_Sound(IO_ON);  // SOUND OFF 시 I/O 출력 함수 

	//20140220
	if(FAS_IO.get_in_bit(st_io.i_reject_tray_chk, IO_ON) == IO_ON)
	{
		st_sync.n_RejectTrayChange = CTL_NO;
		sprintf(mc_alarmcode,"900505"); // Reject Tray Empty Error.
		st_work.mn_run_status = CTL_dWARNING;
		st_sync.n_RejectTrayChange = CTL_READY;
		CTL_Lib.Alarm_Error_Occurrence(0220, st_work.mn_run_status, mc_alarmcode);
	}
	else
	{
		st_sync.n_RejectTrayChange = CTL_YES;
	}
	
//	st_msg.str_fallacy_msg = _T("RejectTray를 교체해 주세요.");
//	nResponse = msg_dlg.DoModal();
//	// 20140220
//	if(nResponse == IDOK) // mlct start
//	{
//		st_sync.n_RejectTrayChange = CTL_YES;
//	}
// 	else
// 	{
//		st_sync.n_RejectTrayChange = CTL_NO;
// 	}
//	Func.OnSet_IO_Port_Sound(IO_OFF);  // SOUND OFF 시 I/O 출력 함수 
}


void CScreen_Main::OnMainLotReStart_Display()
{
	int i;
	CString str_tmp[4] = {"Module", "Front", "Back", "Unload"};
	
	if(st_handler.n_lotend_ready != CTL_NO)
	{
		SetTimer(TM_LOTEND_READY,500, NULL);
	}
	else
	{
		KillTimer(TM_LOTEND_READY);
	}

	for(i=0; i<4; i++)
	{
		GridData(IDC_CUSTOM_BUFFER_MOTOR, i, 0, str_tmp[i]);
		if(st_handler.n_module_retest[i] != CTL_NO && st_handler.n_lotend_ready == CTL_YES)
		{
			GridColor(IDC_CUSTOM_BUFFER_MOTOR, i, 0, RED_C, WHITE_C);
		}
		else
		{
			GridColor(IDC_CUSTOM_BUFFER_MOTOR, i, 0, WHITE_C, BLUE_D);
		}
		mn_retest_onoff[i] = 0;
	}

}

void CScreen_Main::OnBtnLotCancel() 
{
	int n_response = 0;
	
	if(st_work.mn_run_status != dSTOP) return;
	CDialog_Select select_dlg;

	st_msg.mstr_confirm_msg = _T("Lot Cancel을 하시겠습니까??");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Do you want to Lot Cancel?");
	}

	
	n_response = select_dlg.DoModal();
	if(n_response == IDOK)
	{
		st_work.nMdlInputCount[0][0] = st_work.n_loadlot_count[LDMODULE_SITE];
		st_sync.b_lotcancel_use = TRUE;//20130130
		//추후 서버 전송 추가
	}
}

void CScreen_Main::OnBtnVisionCntReset() 
{
	if(st_work.mn_run_status == dRUN) return;
	// TODO: Add your control notification handler code here
	if (st_NW.mstr_Old_LotNo != st_NW.mstr_LotNoContents_1[0])
	{
		st_NW.mstr_Old_LotNo = st_NW.mstr_LotNoContents_1[0];
		mcls_m_main.OnBarcode_Label_Data_Save();
		bcr_retry = 0;
		hs_retry = 0;
		l_bcr_send_wait[0] = GetCurrentTime();
		l_hs_send_wait[0] = GetCurrentTime();
//		::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, VISION_YIELD);
//		::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, HEATSINK_YIELD);
		Func.OnSet_Vision_send(VISION_YIELD);
		SetTimer(TM_BCR_HS_DATA_RESET,100, NULL);
	}		
}

void CScreen_Main::OnMainMulLotDisplay()
{
	int i;
	if(st_work.n_eqp_lotend != CTL_YES)
	{
		m_btn_multilot_init.EnableWindow(FALSE);
	}
	else
	{
		////20140214
		for (i = 0; i < 500; i++)
		{
			st_work.m_strBarcode_OutPirnt[i] = "";
			st_work.mstr_Vision_Read_LotNo[i] = "";//2014,0303
		}
		////
		m_btn_multilot_init.EnableWindow(TRUE);
		m_btn_lot_open.EnableWindow(TRUE);//2014,0408
	}

	////2014,0411
	if(st_work.b_Barcode_Create_Btn_Use == FALSE)
	{
		m_btn_lot_open.EnableWindow(FALSE);
	}
	////

}

void CScreen_Main::OnBtnMultilotInit() 
{
//	int n_response;//, nRet;
	char cJamcode[10] = {0,};

//	if (st_handler.mn_motor_init_check != CTL_YES)	// 모터 초기화를 하지 않았다면 동작하지 않는다. 2K4/11/23/ViboX
//	{
//		Func.OnSet_IO_Port_Sound(IO_ON);
//		sprintf(st_msg.c_abnormal_msg, "[MOTOR INIT CHECK] Motor Initial...");
//		if (st_handler.cwnd_list != NULL)
//		{
//			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//		}
//		sprintf(cJamcode,"909002");
//		st_work.mn_run_status = CTL_dWARNING;
//		CTL_Lib.Alarm_Error_Occurrence(9999, st_work.mn_run_status, cJamcode);
//		return;
//	}
//	if (Func.DoorOpenCheckSpot() == RET_ERROR)	
//	{
//		Func.OnSet_IO_Port_Sound(IO_ON);
//
//		sprintf(st_msg.c_abnormal_msg, "[DOOR CHECK] Door Open being done Run become.");
//		if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);			
//		st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX
//		return;
//	}
//	// Air 감지 센서를 확인한다.
//	if (FAS_IO.get_in_bit(st_io.i_main_air_chk, IO_ON) == IO_OFF)
//	{
//		//900200 0 90 "Air Supply Check Error."
//		sprintf(cJamcode,"900200");    // 발생한 알람 코드 설정 //
//		st_work.mn_run_status = dWARNING;  // 장비 동작 상태 설정 //
//		CTL_Lib.Alarm_Error_Occurrence(5500, st_work.mn_run_status, cJamcode);
//		return;
//	}	
//
//	if (FAS_IO.Get_In_Bit(st_io.i_mc1_chk) == CTL_NO || FAS_IO.Get_In_Bit(st_io.i_mc2_chk) == CTL_NO )
//	{
//		st_handler.mstrSelectMessage = "먼저 RESET 버튼을 눌러 주세요.";
//		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
//		{
//			st_handler.mstrSelectMessage = "RESET Button Push.";
//		}
//		
//		st_handler.mnSelectMessage = 0;		
//		::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1001, 0);		
//		if (st_alarm.mstr_cur_state == "0")	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_ON);
//		else                                FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_ON);
//		return;
//	}
//	CDialog_Select select_dlg;
//
//	st_msg.mstr_confirm_msg = _T("Multi Lot 초기화를 진행하시겠습니까?");
//	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
//	{
//		st_msg.mstr_confirm_msg = _T("Are you want to proceed with the initialization Multi Lot?");
//	}
//
//	
//	n_response = select_dlg.DoModal();
//	if(n_response == IDOK)
//	{
//		/// 20140128   180106 
//
////2014.0519	
//// 		nRet = FAS_IO.get_in_bit(st_io.i_ld_tube_s_empty_chk, IO_OFF);
//// 		if(nRet == IO_ON)
//// 		{//180106 0 18 "로드 튜브에 튜브가 체크되었습니다. 제거해 주세요."
//// 			sprintf(mc_alarmcode,"180106");
//// 			st_work.mn_run_status = CTL_dWARNING;
//// 			CTL_Lib.Alarm_Error_Occurrence(8090, st_work.mn_run_status, mc_alarmcode);
//// 			return;
//// 		}
//
//		if(st_handler.mn_menu_lock != TRUE)
//		{
//			st_handler.mn_menu_lock = TRUE;
//		}
//
//		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);		// 메인 프레임으로 메시지 전송하여 알람 화면 종료 요청 
//		if (st_handler.mn_menu_num != 101)
//		{
//			::SendMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
//		}
//
//		st_work.n_multilot_start = CTL_YES;
//		Func.OnSet_IO_Port_Run();									// 장비 상태 : 동작 상태인 경우 I/O 출력 내보내는 함수
//		if(st_work.n_jamcode_flag == YES)
//		{
//			st_work.n_jamcode_flag	= NO;
//			st_work.t_jamcode_end	= CTime::GetCurrentTime();
//			::PostMessage(st_handler.hWnd, WM_WORK_END, MAIN_MYSQL_ALARM_WRITE, 0);
//		}
//
//		Run_Handler_Check.n_emo_chk = FALSE;
//		Run_Handler_Check.n_air_chk = FALSE;
//		Run_Handler_Check.n_light_curtain_chk = CTL_NO;
//		alarm.n_area_alarm = FALSE;
//		alarm.n_emo_alarm = FALSE;
//
//		st_work.mn_Barcode_OutPrint = -1;//20140215
//		st_sync.n_visionrbt_workend_flag[VISION_1_SITE] = CTL_NO;//20140215
//		st_sync.n_visionrbt_workend_flag[VISION_2_SITE] = CTL_NO;//20140215
//
//		Func.InitNode();
//
//		st_sync.b_lotcancel_use = FALSE;//20130130
//
//		if(st_handler.n_lotend_ready == 10)
//		{
//			st_work.mn_run_status = dLOTREADY;
//		}
//		if(st_handler.cwnd_list != NULL)
//		{
//			sprintf(st_msg.c_normal_msg, "Multi Init를 진행합니다.");
//			if(st_handler.mn_language == LANGUAGE_ENGLISH) 
//			{
//				sprintf(st_msg.c_normal_msg, "Proceed to the Multi Init!!");
//			}
//
//			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
//		}
//
//	}

//	int nmenu_chk = OnMenu_Change_Checking(); // 메뉴 사용 가능 여부 검사 함수 //   
//	if (nmenu_chk != TRUE)  return;
		st_handler.mn_menu_lock = FALSE;
		st_handler.mn_system_lock = FALSE;
		

		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);		// 메인 프레임으로 메시지 전송하여 알람 화면 종료 요청 
		if (st_handler.mn_menu_num != 101)
		{
			::SendMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
		}
	//if(st_handler.mn_level_teach != TRUE)
	//{
	//	if(st_handler.mn_level_maint != TRUE)
	//	{
	//		if(st_handler.cwnd_list != NULL)
	//		{
	//			st_other.str_op_abnormal_msg = _T("[Request Level] Maint or Teaching!");
	//			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
	//		}

	//		return ;
	//	}
	//}

	//2015.0128
	int nResponse;  // 대화 상자 리턴 플래그 
	CDialog_Select  dlgInfo;
	st_other.str_confirm_msg = _T("Do equipment initialization.");


	nResponse = dlgInfo.DoModal();
	if(nResponse == IDOK)
	{
		
		::SendMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 5);
		//초기화를 진행하자~~~
		ShellExecute(NULL, NULL,"HANDLER_RESETOR.EXE", _T("AutoResetOn"), NULL, SW_SHOWNORMAL);

		if(st_work.mn_run_status != dSTOP)
		{
			st_work.mn_run_status = dSTOP; // 장비 상태
		}
		Sleep(1500);
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 5);  // 종료 대화 상자 출력 요청  // 강제종료하자..
	}

}

void CScreen_Main::OnBtnWorkInformation2()  //20121120
{
	// TODO: Add your control notification handler code here
	::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1100, 0);
}

void CScreen_Main::OnBtnWorkInformation3()  //20121120
{
	// TODO: Add your control notification handler code here
	::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1200, 0);
	
}

void CScreen_Main::OnBtnWorkbuffer1Mdpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 1;
}

void CScreen_Main::OnBtnWorkbuffer1Lhpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 2;
}

void CScreen_Main::OnBtnWorkbuffer1Rhspos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 3;
}

void CScreen_Main::OnWorkbuffer1Clip() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 4;
}

void CScreen_Main::OnBtnWorkbuffer1Uldpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 5;
}

void CScreen_Main::OnBtnWorkbuffer1Init() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 0;	
}

void CScreen_Main::OnBtnWorkbuffer2Mdpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 1;
}

void CScreen_Main::OnBtnWorkbuffer2Lhpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 2;
}

void CScreen_Main::OnBtnWorkbuffer2Rhspos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 3;
}

void CScreen_Main::OnWorkbuffer2Clip() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 4;
}

void CScreen_Main::OnBtnWorkbuffer2Uldpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 5;
}

void CScreen_Main::OnBtnWorkbuffer2Init() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 0;	
}

void CScreen_Main::OnBtnWorkbuffer3Mdpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 1;
}

void CScreen_Main::OnBtnWorkbuffer3Lhpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 2;
}

void CScreen_Main::OnBtnWorkbuffer3Rhspos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 3;
}

void CScreen_Main::OnWorkbuffer3Clip() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 4;
}

void CScreen_Main::OnBtnWorkbuffer3Uldpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 5;
}

void CScreen_Main::OnBtnWorkbuffer3Init() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 0;	
}

void CScreen_Main::OnBtnWorkbuffer4Mdpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 1;
}

void CScreen_Main::OnBtnWorkbuffer4Lhpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 2;

}

void CScreen_Main::OnBtnWorkbuffer4Rhspos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 3;
	
}

void CScreen_Main::OnWorkbuffer4Clip() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 4;
	
}

void CScreen_Main::OnBtnWorkbuffer4Uldpos() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 5;	
}

void CScreen_Main::OnBtnWorkbuffer4Init() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 0;	
}

void CScreen_Main::OnBtnMdrobotWorkpos1() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_RBT_Y] = 3;	
}

void CScreen_Main::OnBtnMdrobotWorkpos2() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_RBT_Y] = 4;	
}

void CScreen_Main::OnBtnMdrobotWorkpos3() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_RBT_Y] = 5;	
}

void CScreen_Main::OnBtnMdrobotWorkpos4()
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_RBT_Y] = 6;	
}

void CScreen_Main::OnBtnMdrobotInit() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_RBT_Y] = 0;
}

void CScreen_Main::OnBtnMdrobotUp() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_RBT_Z] = 0;	
}

void CScreen_Main::OnBtnMdrobotDown() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_RBT_Z] = 2;
}

void CScreen_Main::OnBtnLhsrobotWorkpos1() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_F_RBT_Y] = 3;	
}

void CScreen_Main::OnBtnLhsrobotWorkpos2() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_F_RBT_Y] = 4;
}

void CScreen_Main::OnBtnLhsrobotWorkpos3() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_F_RBT_Y] = 5;
}

void CScreen_Main::OnBtnLhsrobotWorkpos4() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_F_RBT_Y] = 6;
}

void CScreen_Main::OnBtnLhsrobotInit() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_F_RBT_Y] = 0;
}

void CScreen_Main::OnBtnLhsrobotUp() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_F_RBT_Z] = 0;	
}

void CScreen_Main::OnBtnLhsrobotDown() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_F_RBT_Z] = 2;
}

void CScreen_Main::OnBtnRhsrobotWorkpos1() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_B_RBT_Y] = 3;
}

void CScreen_Main::OnBtnRhsrobotWorkpos2() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_B_RBT_Y] = 4;
}

void CScreen_Main::OnBtnRhsrobotWorkpos3() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_B_RBT_Y] = 5;
}

void CScreen_Main::OnBtnRhsrobotWorkpos4() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_B_RBT_Y] = 6;
}

void CScreen_Main::OnBtnRhsrobotInit() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_B_RBT_Y] = 0;
}

void CScreen_Main::OnBtnRhsrobotUp() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_B_RBT_Z] = 0;	
}

void CScreen_Main::OnBtnRhsrobotDown() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_HS_B_RBT_Z] = 2;
}

void CScreen_Main::OnBtnClipinsertWorkpos1() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_CLIP_INSERT_Y] = 2;
}

void CScreen_Main::OnBtnClipinsertWorkpos2() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_CLIP_INSERT_Y] = 3;
}

void CScreen_Main::OnBtnClipinsertWorkpos3() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_CLIP_INSERT_Y] = 4;
}

void CScreen_Main::OnBtnClipinsertWorkpos4() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_CLIP_INSERT_Y] = 5;
}

void CScreen_Main::OnBtnClipinsertInit() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_CLIP_INSERT_Y] = 0;
}

void CScreen_Main::OnBtnClipinsertUp() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_CLIP_INSERT_Z] = 0;	
}

void CScreen_Main::OnBtnClipinsertDown() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_CLIP_INSERT_Z] = 2;
}

void CScreen_Main::OnBtnClipclampWorkpos1() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_CLAMP_RBT_Y] = 1;
}

void CScreen_Main::OnBtnClipclampWorkpos2() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_CLAMP_RBT_Y] = 2;
}

void CScreen_Main::OnBtnClipclampWorkpos3() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_CLAMP_RBT_Y] = 3;
}

void CScreen_Main::OnBtnClipclampWorkpos4() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_CLAMP_RBT_Y] = 4;	
}

void CScreen_Main::OnBtnClipclampUp() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_CLAMP_RBT_Z] = 0;	
}

void CScreen_Main::OnBtnClipclampDown() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_M_CLAMP_RBT_Z] = 1;
}

void CScreen_Main::OnBtnSorterrbtWorkpos1() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_SORTER_Y] = 1;
}

void CScreen_Main::OnBtnSorterrbtWorkpos2() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_SORTER_Y] = 2;	
}

void CScreen_Main::OnBtnSorterrbtWorkpos3() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_SORTER_Y] = 3;
}

void CScreen_Main::OnBtnSorterrbtWorkpos4() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_SORTER_Y] = 4;	
}

void CScreen_Main::OnBtnSorterrbtInit() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_SORTER_Y] = 0;	
}

void CScreen_Main::OnBtnSorterUp() 
{
	st_work.b_run_slmulation_md = TRUE;	
	st_ani.nSelectMotPos[M_RBT_SORTER_Z] = 0;	
}

void CScreen_Main::OnBtnSorterDown() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_SORTER_Z] = 1;
}

void CScreen_Main::OnBtnUldrobotVisionPick() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_UNLOAD_Y] = 1;
}

void CScreen_Main::OnBtnUldrobotDown() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_UNLOAD_Y] = 2;
}

void CScreen_Main::OnBtnUldrbtInit() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_UNLOAD_Y] = 0;	
}

void CScreen_Main::OnBtnUldrbtUp() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 0;	
}

void CScreen_Main::OnBtnUldrbtDown() 
{
	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 2;	
}

//void CScreen_Main::OnBtnSimuelStart()	
//{ 
//	st_work.mn_run_status = dRUN;  
//	st_motor[M_WORK_BUFFER_1].md_pos[WORK_LDMODULE];
//}


void CScreen_Main::OnBtnSimuelInit() 
{
	st_work.mn_run_status = dREINSTATE; 

	int n_response;
	char cJamcode[10] = {0,};
	
	if (st_handler.mn_motor_init_check != CTL_YES)	// 모터 초기화를 하지 않았다면 동작하지 않는다. 2K4/11/23/ViboX
	{
		Func.OnSet_IO_Port_Sound(IO_ON);
		sprintf(st_msg.c_abnormal_msg, "[MOTOR INIT CHECK] Motor Initial...");
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		sprintf(cJamcode,"909002");
		st_work.mn_run_status = CTL_dWARNING;
		CTL_Lib.Alarm_Error_Occurrence(9999, st_work.mn_run_status, cJamcode);
		return;
	}
	if (Func.DoorOpenCheckSpot() == RET_ERROR)	
	{
		Func.OnSet_IO_Port_Sound(IO_ON);
		
		sprintf(st_msg.c_abnormal_msg, "[DOOR CHECK] Door Open being done Run become.");
		if (st_handler.cwnd_list != NULL)  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);			
		st_work.mn_run_status = dWARNING;	// dJAM,dWARNING  Alarm 화면을 띄워준다. 2K4/11/21/ViboX
		return;
	}
	// Air 감지 센서를 확인한다.
	if (FAS_IO.get_in_bit(st_io.i_main_air_chk, IO_ON) == IO_OFF)
	{
		//900200 0 90 "Air Supply Check Error."
		sprintf(cJamcode,"900200");    // 발생한 알람 코드 설정 //
		st_work.mn_run_status = dWARNING;  // 장비 동작 상태 설정 //
		CTL_Lib.Alarm_Error_Occurrence(5500, st_work.mn_run_status, cJamcode);
		return;
	}	
	
	if (FAS_IO.Get_In_Bit(st_io.i_mc1_chk) == CTL_NO || FAS_IO.Get_In_Bit(st_io.i_mc2_chk) == CTL_NO )
	{
		st_handler.mstrSelectMessage = "먼저 RESET 버튼을 눌러 주세요.";		
		st_handler.mnSelectMessage = 0;		
		::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 1001, 0);		
		if (st_alarm.mstr_cur_state == "0")	FAS_IO.set_out_bit(st_io.o_buzzer_1, IO_ON);
		else                                FAS_IO.set_out_bit(st_io.o_buzzer_2, IO_ON);
		return;
	}
	CDialog_Select select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Multi Lot 초기화를 진행하시겠습니까?");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Are you sure you want to proceed with the initialization Multi Lot?");
	}

	
	n_response = select_dlg.DoModal();
	if(n_response == IDOK)
	{
		
		if(st_handler.mn_menu_lock != TRUE)
		{
			st_handler.mn_menu_lock = TRUE;
		}
		
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 7, 2);		// 메인 프레임으로 메시지 전송하여 알람 화면 종료 요청 
		if (st_handler.mn_menu_num != 101)
		{
			::SendMessage(st_handler.hWnd, WM_FORM_CHANGE, 1, 1);	// 메인 프레임으로 메시지 전송하여 메인 화면으로 전환 
		}
		
		st_work.n_multilot_start = CTL_YES;
		Func.OnSet_IO_Port_Run();									// 장비 상태 : 동작 상태인 경우 I/O 출력 내보내는 함수
		if(st_work.n_jamcode_flag == YES)
		{
			st_work.n_jamcode_flag	= NO;
			st_work.t_jamcode_end	= CTime::GetCurrentTime();
			::PostMessage(st_handler.hWnd, WM_WORK_END, MAIN_MYSQL_ALARM_WRITE, 0);
		}
		
		Run_Handler_Check.n_emo_chk = FALSE;
		Run_Handler_Check.n_air_chk = FALSE;
		Run_Handler_Check.n_light_curtain_chk = CTL_NO;
		alarm.n_area_alarm = FALSE;
		alarm.n_emo_alarm = FALSE;
		
		Func.InitNode();
		
		st_sync.b_lotcancel_use = FALSE;//20130130
		
		if(st_handler.n_lotend_ready == 10)
		{
			st_work.mn_run_status = dLOTREADY;
		}
	}

	st_work.b_run_slmulation_md = TRUE;
	st_ani.nSelectMotPos[M_WORK_BUFFER_1] = 0;
	st_ani.nSelectMotPos[M_WORK_BUFFER_2] = 0;
	st_ani.nSelectMotPos[M_WORK_BUFFER_3] = 0;
	st_ani.nSelectMotPos[M_WORK_BUFFER_4] = 0;
	st_ani.nSelectMotPos[M_M_RBT_Y] = 0;
	st_ani.nSelectMotPos[M_M_RBT_Z] = 0;
	st_ani.nSelectMotPos[M_HS_F_RBT_Y] = 0;
	st_ani.nSelectMotPos[M_HS_F_RBT_Z] = 0;
	st_ani.nSelectMotPos[M_HS_B_RBT_Y] = 0;
	st_ani.nSelectMotPos[M_HS_B_RBT_Z] = 0;
	st_ani.nSelectMotPos[M_CLIP_INSERT_Y] = 0;
	st_ani.nSelectMotPos[M_CLIP_INSERT_Z] = 0;
	st_ani.nSelectMotPos[M_M_CLAMP_RBT_Y] = 0;
	st_ani.nSelectMotPos[M_M_CLAMP_RBT_Z] = 0;
	st_ani.nSelectMotPos[M_RBT_SORTER_Y] = 0;
	st_ani.nSelectMotPos[M_RBT_SORTER_Z] = 0;
	st_ani.nSelectMotPos[M_RBT_UNLOAD_Y] = 0;
	st_ani.nSelectMotPos[M_RBT_UNLOAD_Z] = 0;
}


void CScreen_Main::Init_Animation()
{
	CRect rect(0,0,0,0);
	GetDlgItem(IDC_STATIC_ANIVIEW)->GetClientRect(&rect);	
	m_Animate.Create( NULL, WS_VISIBLE | WS_CHILD,rect,GetDlgItem(IDC_STATIC_ANIVIEW), NULL ) ;
	m_Animate.InitAniWnd();
	m_Animate.ActionDisplay();
	
	if(m_bExeExcute != 1) 
	{ 
		for(int i=0; i < MAXMOTOR; i++) 
		{	
			st_ani.nSelectMotPos[i] = -1; 
		}	
		m_bExeExcute = TRUE; 
	}

	st_work.b_run_slmulation_md = FALSE;
}

void CScreen_Main::UpdateScreens()
{
	if(!CScreen_Main::IsWindowVisible())
		return;

	m_Animate.ActionDisplay();
}

void CScreen_Main::UpdateMotorMove()
{
	if(!CScreen_Main::IsWindowVisible())
		return;

	double dPosTmp,dPosTmp2;
	double dMaxPos, dMinPos, dMotRate;

	int n_Work1Allow, n_Work2Allow, n_Work3Allow, n_Work4Allow;
	int n_MDRbtAllow, n_LHSRbtAllow, n_RHSRbtAllow, n_ClipRbtAllow, n_ClampRbtAllow, n_SorterRbtAllow, n_ULDRbtAllow;

	if(COMI.mn_simulation_mode == 1)
	{
	 	for(int i=0; i < MAXMOTOR; i++)
		{
			dMinPos	= st_motor[i].d_limit_position[0];
			dMaxPos	= st_motor[i].d_limit_position[1];
			
			switch(i)
			{	
			case M_WORK_BUFFER_1://ok  
				{
					dMotRate = /*dMaxPos*/ 1000 / 1000;
					
					dPosTmp = (int) (st_motor[M_WORK_BUFFER_1].md_pos[st_ani.nSelectMotPos[M_WORK_BUFFER_1]] / dMotRate);

					if(dPosTmp == abs(m_Animate.m_Workbuffer1.m_dEqCurPos_X))
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_Workbuffer1.m_dEqCurPos_X > dPosTmp) 
						{
							m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X - 10;

							if(dPosTmp > m_Animate.m_Workbuffer1.m_dEqCurPos_X)
							{
								n_Work1Allow = dPosTmp - m_Animate.m_Workbuffer1.m_dEqCurPos_X;

								m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X + n_Work1Allow;
								break;
							}
						}
						else 
						{
							m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X + 10;
						}
					}
				}
				break;

			case M_WORK_BUFFER_2://ok  
				{
					dMotRate = /*dMaxPos*/ 1000 / 1000;
					
					dPosTmp = (int) (st_motor[M_WORK_BUFFER_2].md_pos[st_ani.nSelectMotPos[M_WORK_BUFFER_2]] / dMotRate);
									
					if(dPosTmp == abs(m_Animate.m_Workbuffer2.m_dEqCurPos_X))
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_Workbuffer2.m_dEqCurPos_X > dPosTmp) 
						{
							m_Animate.m_Workbuffer2.m_dEqCurPos_X = m_Animate.m_Workbuffer2.m_dEqCurPos_X - 10;
							
							if(dPosTmp > m_Animate.m_Workbuffer2.m_dEqCurPos_X)
							{
								n_Work2Allow = dPosTmp - m_Animate.m_Workbuffer2.m_dEqCurPos_X;
								
								m_Animate.m_Workbuffer2.m_dEqCurPos_X = m_Animate.m_Workbuffer2.m_dEqCurPos_X + n_Work2Allow;
								break;
							}
						}
						else 
						{
							m_Animate.m_Workbuffer2.m_dEqCurPos_X = m_Animate.m_Workbuffer2.m_dEqCurPos_X + 10;
						}
					}
				}
				break;

			case M_WORK_BUFFER_3://ok  
				{
					dMotRate = /*dMaxPos*/ 1000 / 1000;
					
					dPosTmp = (int) (st_motor[M_WORK_BUFFER_3].md_pos[st_ani.nSelectMotPos[M_WORK_BUFFER_3]] / dMotRate);
					
					if(dPosTmp == abs(m_Animate.m_Workbuffer3.m_dEqCurPos_X))
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_Workbuffer3.m_dEqCurPos_X > dPosTmp) 
						{
							m_Animate.m_Workbuffer3.m_dEqCurPos_X = m_Animate.m_Workbuffer3.m_dEqCurPos_X - 10;
							
							if(dPosTmp > m_Animate.m_Workbuffer3.m_dEqCurPos_X)
							{
								n_Work3Allow = dPosTmp - m_Animate.m_Workbuffer3.m_dEqCurPos_X;
								
								m_Animate.m_Workbuffer3.m_dEqCurPos_X = m_Animate.m_Workbuffer3.m_dEqCurPos_X + n_Work3Allow;
								break;
							}
						}
						else 
						{
							m_Animate.m_Workbuffer3.m_dEqCurPos_X = m_Animate.m_Workbuffer3.m_dEqCurPos_X + 10;
						}
					}
				}
				break;

			case M_WORK_BUFFER_4://ok  
				{
					dMotRate = /*dMaxPos*/ 1000 / 1000;
					
					dPosTmp = (int) (st_motor[M_WORK_BUFFER_4].md_pos[st_ani.nSelectMotPos[M_WORK_BUFFER_4]] / dMotRate);
					
					if(dPosTmp == abs(m_Animate.m_Workbuffer4.m_dEqCurPos_X))
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_Workbuffer4.m_dEqCurPos_X > dPosTmp) 
						{
							m_Animate.m_Workbuffer4.m_dEqCurPos_X = m_Animate.m_Workbuffer4.m_dEqCurPos_X - 10;
							
							if(dPosTmp > m_Animate.m_Workbuffer4.m_dEqCurPos_X)
							{
								n_Work4Allow = dPosTmp - m_Animate.m_Workbuffer4.m_dEqCurPos_X;
								
								m_Animate.m_Workbuffer4.m_dEqCurPos_X = m_Animate.m_Workbuffer4.m_dEqCurPos_X + n_Work4Allow;
								break;
							}
						}
						else 
						{
							m_Animate.m_Workbuffer4.m_dEqCurPos_X = m_Animate.m_Workbuffer4.m_dEqCurPos_X + 10;
						}
					}
				}
				break;

			case M_M_RBT_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;
					
					dPosTmp = (int) (st_motor[M_M_RBT_Y].md_pos[st_ani.nSelectMotPos[M_M_RBT_Y]] / dMotRate);

					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ModuleRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_ModuleRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y - 10;
							
							if(dPosTmp > m_Animate.m_ModuleRbt.m_dEqCurPos_Y)
							{
								n_MDRbtAllow = dPosTmp - m_Animate.m_ModuleRbt.m_dEqCurPos_Y;

								m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y + n_MDRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y + 10;
						}
					}
				}
				break;

			case M_M_RBT_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 25000 / 1000;
					
					dPosTmp = (int) (st_motor[M_M_RBT_Z].md_pos[st_ani.nSelectMotPos[M_M_RBT_Z]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ModuleRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_ModuleRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_ModuleRbt.m_dEqCurPos_Z)
							{
								n_MDRbtAllow = dPosTmp - m_Animate.m_ModuleRbt.m_dEqCurPos_Z;
								
								m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z + n_MDRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_HS_F_RBT_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;
					
					dPosTmp = (int) (st_motor[M_HS_F_RBT_Y].md_pos[st_ani.nSelectMotPos[M_HS_F_RBT_Y]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_LeftHSRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_LeftHSRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_LeftHSRbt.m_dEqCurPos_Y = m_Animate.m_LeftHSRbt.m_dEqCurPos_Y - 10;
							
							if(dPosTmp > m_Animate.m_LeftHSRbt.m_dEqCurPos_Y)
							{
								n_LHSRbtAllow = dPosTmp - m_Animate.m_LeftHSRbt.m_dEqCurPos_Y;

								m_Animate.m_LeftHSRbt.m_dEqCurPos_Y = m_Animate.m_LeftHSRbt.m_dEqCurPos_Y + n_LHSRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_LeftHSRbt.m_dEqCurPos_Y = m_Animate.m_LeftHSRbt.m_dEqCurPos_Y + 10;
						}
					}
				}
				break;

			case M_HS_F_RBT_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 25000 / 1000;
					
					dPosTmp = (int) (st_motor[M_HS_F_RBT_Z].md_pos[st_ani.nSelectMotPos[M_HS_F_RBT_Z]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_LeftHSRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_LeftHSRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_LeftHSRbt.m_dEqCurPos_Z = m_Animate.m_LeftHSRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_LeftHSRbt.m_dEqCurPos_Z)
							{
								n_LHSRbtAllow = dPosTmp - m_Animate.m_LeftHSRbt.m_dEqCurPos_Z;
								
								m_Animate.m_LeftHSRbt.m_dEqCurPos_Z = m_Animate.m_LeftHSRbt.m_dEqCurPos_Z + n_LHSRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_LeftHSRbt.m_dEqCurPos_Z = m_Animate.m_LeftHSRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_HS_B_RBT_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;
					
					dPosTmp = (int) (st_motor[M_HS_B_RBT_Y].md_pos[st_ani.nSelectMotPos[M_HS_B_RBT_Y]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_RightHSRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}				 
					else
					{
						if(m_Animate.m_RightHSRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_RightHSRbt.m_dEqCurPos_Y = m_Animate.m_RightHSRbt.m_dEqCurPos_Y - 10;
							
							if(dPosTmp > m_Animate.m_RightHSRbt.m_dEqCurPos_Y)
							{
								n_RHSRbtAllow = dPosTmp - m_Animate.m_RightHSRbt.m_dEqCurPos_Y;
								
								m_Animate.m_RightHSRbt.m_dEqCurPos_Y = m_Animate.m_RightHSRbt.m_dEqCurPos_Y + n_RHSRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_RightHSRbt.m_dEqCurPos_Y = m_Animate.m_RightHSRbt.m_dEqCurPos_Y + 10;
						}
					}
				}
				break;

			case M_HS_B_RBT_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 25000 / 1000;
					
					dPosTmp = (int) (st_motor[M_HS_B_RBT_Z].md_pos[st_ani.nSelectMotPos[M_HS_B_RBT_Z]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_RightHSRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}				 
					else
					{
						if(m_Animate.m_RightHSRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_RightHSRbt.m_dEqCurPos_Z = m_Animate.m_RightHSRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_RightHSRbt.m_dEqCurPos_Z)
							{
								n_RHSRbtAllow = dPosTmp - m_Animate.m_RightHSRbt.m_dEqCurPos_Z;
								
								m_Animate.m_RightHSRbt.m_dEqCurPos_Z = m_Animate.m_RightHSRbt.m_dEqCurPos_Z + n_RHSRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_RightHSRbt.m_dEqCurPos_Z = m_Animate.m_RightHSRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_CLIP_INSERT_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;
					
					dPosTmp = (int) (st_motor[M_CLIP_INSERT_Y].md_pos[st_ani.nSelectMotPos[M_CLIP_INSERT_Y]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ClipRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}				 
					else
					{
						if(m_Animate.m_ClipRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_ClipRbt.m_dEqCurPos_Y = m_Animate.m_ClipRbt.m_dEqCurPos_Y - 10;
							
							if(dPosTmp > m_Animate.m_ClipRbt.m_dEqCurPos_Y)
							{
								n_ClipRbtAllow = dPosTmp - m_Animate.m_ClipRbt.m_dEqCurPos_Y;
								
								m_Animate.m_ClipRbt.m_dEqCurPos_Y = m_Animate.m_ClipRbt.m_dEqCurPos_Y + n_ClipRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ClipRbt.m_dEqCurPos_Y = m_Animate.m_ClipRbt.m_dEqCurPos_Y + 10;
						}
					}
				}
				break;

			case M_CLIP_INSERT_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 15000 / 1000;
					
					dPosTmp = (int) (st_motor[M_CLIP_INSERT_Z].md_pos[st_ani.nSelectMotPos[M_CLIP_INSERT_Z]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ClipRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}				 
					else
					{
						if(m_Animate.m_ClipRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_ClipRbt.m_dEqCurPos_Z = m_Animate.m_ClipRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_ClipRbt.m_dEqCurPos_Z)
							{
								n_ClipRbtAllow = dPosTmp - m_Animate.m_ClipRbt.m_dEqCurPos_Z;
								
								m_Animate.m_ClipRbt.m_dEqCurPos_Z = m_Animate.m_ClipRbt.m_dEqCurPos_Z + n_ClipRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ClipRbt.m_dEqCurPos_Z = m_Animate.m_ClipRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_M_CLAMP_RBT_Y:  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;
					
					dPosTmp = (int) (st_motor[M_M_CLAMP_RBT_Y].md_pos[st_ani.nSelectMotPos[M_M_CLAMP_RBT_Y]] / dMotRate);
				
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ClampRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_ClampRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_ClampRbt.m_dEqCurPos_Y = m_Animate.m_ClampRbt.m_dEqCurPos_Y - 10;
							
							if(dPosTmp > m_Animate.m_ClampRbt.m_dEqCurPos_Y)
							{
								n_ClampRbtAllow = dPosTmp - m_Animate.m_ClampRbt.m_dEqCurPos_Y;
								
								m_Animate.m_ClampRbt.m_dEqCurPos_Y = m_Animate.m_ClampRbt.m_dEqCurPos_Y + n_ClampRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ClampRbt.m_dEqCurPos_Y = m_Animate.m_ClampRbt.m_dEqCurPos_Y + 10;
						}
					}
				}
				break;

			case M_M_CLAMP_RBT_Z:
				{
					dMotRate = /*dMaxPos*/ 10000 / 1000;
					
					dPosTmp = (int) (st_motor[M_M_CLAMP_RBT_Z].md_pos[st_ani.nSelectMotPos[M_M_CLAMP_RBT_Z]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ClampRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_ClampRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_ClampRbt.m_dEqCurPos_Z = m_Animate.m_ClampRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_ClampRbt.m_dEqCurPos_Z)
							{
								n_ClampRbtAllow = dPosTmp - m_Animate.m_ClampRbt.m_dEqCurPos_Z;
								
								m_Animate.m_ClampRbt.m_dEqCurPos_Z = m_Animate.m_ClampRbt.m_dEqCurPos_Z + n_ClampRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ClampRbt.m_dEqCurPos_Z = m_Animate.m_ClampRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_RBT_SORTER_Y://ok
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;
					
					dPosTmp = (int) (st_motor[M_RBT_SORTER_Y].md_pos[st_ani.nSelectMotPos[M_RBT_SORTER_Y]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_SorterRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_SorterRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_SorterRbt.m_dEqCurPos_Y = m_Animate.m_SorterRbt.m_dEqCurPos_Y - 10;
							
							if(dPosTmp > m_Animate.m_SorterRbt.m_dEqCurPos_Y)
							{
								n_SorterRbtAllow = dPosTmp - m_Animate.m_SorterRbt.m_dEqCurPos_Y;
								
								m_Animate.m_SorterRbt.m_dEqCurPos_Y = m_Animate.m_SorterRbt.m_dEqCurPos_Y + n_SorterRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_SorterRbt.m_dEqCurPos_Y = m_Animate.m_SorterRbt.m_dEqCurPos_Y + 10;
						}
					}
				}
				break;

			case M_RBT_SORTER_Z://ok
				{
					dMotRate = /*dMaxPos*/ 20000 / 1000;
					
					dPosTmp = (int) (st_motor[M_RBT_SORTER_Z].md_pos[st_ani.nSelectMotPos[M_RBT_SORTER_Z]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_SorterRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_SorterRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_SorterRbt.m_dEqCurPos_Z = m_Animate.m_SorterRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_SorterRbt.m_dEqCurPos_Z)
							{
								n_SorterRbtAllow = dPosTmp - m_Animate.m_SorterRbt.m_dEqCurPos_Z;
								
								m_Animate.m_SorterRbt.m_dEqCurPos_Z = m_Animate.m_SorterRbt.m_dEqCurPos_Z + n_SorterRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_SorterRbt.m_dEqCurPos_Z = m_Animate.m_SorterRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_RBT_UNLOAD_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;
					
					dPosTmp = (int) (st_motor[M_RBT_UNLOAD_Y].md_pos[st_ani.nSelectMotPos[M_RBT_UNLOAD_Y]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_UnloaderRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_UnloaderRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_UnloaderRbt.m_dEqCurPos_Y = m_Animate.m_UnloaderRbt.m_dEqCurPos_Y - 10;
							
							if(dPosTmp > m_Animate.m_UnloaderRbt.m_dEqCurPos_Y)
							{
								n_ULDRbtAllow = dPosTmp - m_Animate.m_UnloaderRbt.m_dEqCurPos_Y;

								m_Animate.m_UnloaderRbt.m_dEqCurPos_Y = m_Animate.m_UnloaderRbt.m_dEqCurPos_Y + n_ULDRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_UnloaderRbt.m_dEqCurPos_Y = m_Animate.m_UnloaderRbt.m_dEqCurPos_Y + 10;
						}
					}
				}
				break;

			case M_RBT_UNLOAD_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 20000 / 1000;
					
					dPosTmp = (int) (st_motor[M_RBT_UNLOAD_Z].md_pos[st_ani.nSelectMotPos[M_RBT_UNLOAD_Z]] / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_UnloaderRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_UnloaderRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_UnloaderRbt.m_dEqCurPos_Z = m_Animate.m_UnloaderRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_UnloaderRbt.m_dEqCurPos_Z)
							{
								n_ULDRbtAllow = dPosTmp - m_Animate.m_UnloaderRbt.m_dEqCurPos_Z;
								
								m_Animate.m_UnloaderRbt.m_dEqCurPos_Z = m_Animate.m_UnloaderRbt.m_dEqCurPos_Z + n_ULDRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_UnloaderRbt.m_dEqCurPos_Z = m_Animate.m_UnloaderRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;
			}
		}
	}
	else
	{
		//모터 엔코더 값 표시
		for(int i=0; i < MAXMOTOR; i++)
		{
			dMinPos	= st_motor[i].d_limit_position[0];
			dMaxPos	= st_motor[i].d_limit_position[1];
			
			switch(i)
			{	
			case M_WORK_BUFFER_1://ok  
				{
					dMotRate = /*dMaxPos*/ 1000 / 1000;
					
					dPosTmp2 = COMI.Get_MotCurrentPos(M_WORK_BUFFER_1);

					dPosTmp = (int) (dPosTmp2 / dMotRate);

					if(dPosTmp == abs(m_Animate.m_Workbuffer1.m_dEqCurPos_X))
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_Workbuffer1.m_dEqCurPos_X > dPosTmp) 
						{
							m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X - 4;

							if(dPosTmp > m_Animate.m_Workbuffer1.m_dEqCurPos_X)
							{
								n_Work1Allow = dPosTmp - m_Animate.m_Workbuffer1.m_dEqCurPos_X;

								m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X + n_Work1Allow;
								break;
							}
						}
						else 
						{
							m_Animate.m_Workbuffer1.m_dEqCurPos_X = m_Animate.m_Workbuffer1.m_dEqCurPos_X + 4;
						}
					}
				}
				break;

			case M_WORK_BUFFER_2://ok  
				{
					dMotRate = /*dMaxPos*/ 1000 / 1000;
					
					dPosTmp2 = COMI.Get_MotCurrentPos(M_WORK_BUFFER_2);

					dPosTmp = (int) (dPosTmp2 / dMotRate);
									
					if(dPosTmp == abs(m_Animate.m_Workbuffer2.m_dEqCurPos_X))
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_Workbuffer2.m_dEqCurPos_X > dPosTmp) 
						{
							m_Animate.m_Workbuffer2.m_dEqCurPos_X = m_Animate.m_Workbuffer2.m_dEqCurPos_X - 4;
							
							if(dPosTmp > m_Animate.m_Workbuffer2.m_dEqCurPos_X)
							{
								n_Work2Allow = dPosTmp - m_Animate.m_Workbuffer2.m_dEqCurPos_X;
								
								m_Animate.m_Workbuffer2.m_dEqCurPos_X = m_Animate.m_Workbuffer2.m_dEqCurPos_X + n_Work2Allow;
								break;
							}
						}
						else 
						{
							m_Animate.m_Workbuffer2.m_dEqCurPos_X = m_Animate.m_Workbuffer2.m_dEqCurPos_X + 4;
						}
					}
				}
				break;

			case M_WORK_BUFFER_3://ok  
				{
					dMotRate = /*dMaxPos*/ 1000 / 1000;
					
					dPosTmp2 = COMI.Get_MotCurrentPos(M_WORK_BUFFER_3);

					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					if(dPosTmp == abs(m_Animate.m_Workbuffer3.m_dEqCurPos_X))
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_Workbuffer3.m_dEqCurPos_X > dPosTmp) 
						{
							m_Animate.m_Workbuffer3.m_dEqCurPos_X = m_Animate.m_Workbuffer3.m_dEqCurPos_X - 4;
							
							if(dPosTmp > m_Animate.m_Workbuffer3.m_dEqCurPos_X)
							{
								n_Work3Allow = dPosTmp - m_Animate.m_Workbuffer3.m_dEqCurPos_X;
								
								m_Animate.m_Workbuffer3.m_dEqCurPos_X = m_Animate.m_Workbuffer3.m_dEqCurPos_X + n_Work3Allow;
								break;
							}
						}
						else 
						{
							m_Animate.m_Workbuffer3.m_dEqCurPos_X = m_Animate.m_Workbuffer3.m_dEqCurPos_X + 4;
						}
					}
				}
				break;

			case M_WORK_BUFFER_4://ok  
				{
					dMotRate = /*dMaxPos*/ 1000 / 1000;

					dPosTmp2 = COMI.Get_MotCurrentPos(M_WORK_BUFFER_4);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					if(dPosTmp == abs(m_Animate.m_Workbuffer4.m_dEqCurPos_X))
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_Workbuffer4.m_dEqCurPos_X > dPosTmp) 
						{
							m_Animate.m_Workbuffer4.m_dEqCurPos_X = m_Animate.m_Workbuffer4.m_dEqCurPos_X - 4;
							
							if(dPosTmp > m_Animate.m_Workbuffer4.m_dEqCurPos_X)
							{
								n_Work4Allow = dPosTmp - m_Animate.m_Workbuffer4.m_dEqCurPos_X;
								
								m_Animate.m_Workbuffer4.m_dEqCurPos_X = m_Animate.m_Workbuffer4.m_dEqCurPos_X + n_Work4Allow;
								break;
							}
						}
						else 
						{
							m_Animate.m_Workbuffer4.m_dEqCurPos_X = m_Animate.m_Workbuffer4.m_dEqCurPos_X + 4;
						}
					}
				}
				break;

			case M_M_RBT_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;

					dPosTmp2 = COMI.Get_MotCurrentPos(M_M_RBT_Y);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);

					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ModuleRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_ModuleRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y - 4;
							
							if(dPosTmp > m_Animate.m_ModuleRbt.m_dEqCurPos_Y)
							{
								n_MDRbtAllow = dPosTmp - m_Animate.m_ModuleRbt.m_dEqCurPos_Y;

								m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y + n_MDRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ModuleRbt.m_dEqCurPos_Y = m_Animate.m_ModuleRbt.m_dEqCurPos_Y + 4;
						}
					}
				}
				break;

			case M_M_RBT_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 25000 / 1000;

					dPosTmp2 = COMI.Get_MotCurrentPos(M_M_RBT_Z);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ModuleRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_ModuleRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_ModuleRbt.m_dEqCurPos_Z)
							{
								n_MDRbtAllow = dPosTmp - m_Animate.m_ModuleRbt.m_dEqCurPos_Z;
								
								m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z + n_MDRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ModuleRbt.m_dEqCurPos_Z = m_Animate.m_ModuleRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_HS_F_RBT_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;

					dPosTmp2 = COMI.Get_MotCurrentPos(M_HS_F_RBT_Y);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_LeftHSRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_LeftHSRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_LeftHSRbt.m_dEqCurPos_Y = m_Animate.m_LeftHSRbt.m_dEqCurPos_Y - 4;
							
							if(dPosTmp > m_Animate.m_LeftHSRbt.m_dEqCurPos_Y)
							{
								n_LHSRbtAllow = dPosTmp - m_Animate.m_LeftHSRbt.m_dEqCurPos_Y;

								m_Animate.m_LeftHSRbt.m_dEqCurPos_Y = m_Animate.m_LeftHSRbt.m_dEqCurPos_Y + n_LHSRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_LeftHSRbt.m_dEqCurPos_Y = m_Animate.m_LeftHSRbt.m_dEqCurPos_Y + 4;
						}
					}
				}
				break;

			case M_HS_F_RBT_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 25000 / 1000;
					
					dPosTmp2 = COMI.Get_MotCurrentPos(M_HS_F_RBT_Z);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_LeftHSRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_LeftHSRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_LeftHSRbt.m_dEqCurPos_Z = m_Animate.m_LeftHSRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_LeftHSRbt.m_dEqCurPos_Z)
							{
								n_LHSRbtAllow = dPosTmp - m_Animate.m_LeftHSRbt.m_dEqCurPos_Z;
								
								m_Animate.m_LeftHSRbt.m_dEqCurPos_Z = m_Animate.m_LeftHSRbt.m_dEqCurPos_Z + n_LHSRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_LeftHSRbt.m_dEqCurPos_Z = m_Animate.m_LeftHSRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_HS_B_RBT_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;

					dPosTmp2 = COMI.Get_MotCurrentPos(M_HS_B_RBT_Y);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_RightHSRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}				 
					else
					{
						if(m_Animate.m_RightHSRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_RightHSRbt.m_dEqCurPos_Y = m_Animate.m_RightHSRbt.m_dEqCurPos_Y - 4;
							
							if(dPosTmp > m_Animate.m_RightHSRbt.m_dEqCurPos_Y)
							{
								n_RHSRbtAllow = dPosTmp - m_Animate.m_RightHSRbt.m_dEqCurPos_Y;
								
								m_Animate.m_RightHSRbt.m_dEqCurPos_Y = m_Animate.m_RightHSRbt.m_dEqCurPos_Y + n_RHSRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_RightHSRbt.m_dEqCurPos_Y = m_Animate.m_RightHSRbt.m_dEqCurPos_Y + 4;
						}
					}
				}
				break;

			case M_HS_B_RBT_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 25000 / 1000;
					
					dPosTmp2 = COMI.Get_MotCurrentPos(M_HS_B_RBT_Z);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_RightHSRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}				 
					else
					{
						if(m_Animate.m_RightHSRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_RightHSRbt.m_dEqCurPos_Z = m_Animate.m_RightHSRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_RightHSRbt.m_dEqCurPos_Z)
							{
								n_RHSRbtAllow = dPosTmp - m_Animate.m_RightHSRbt.m_dEqCurPos_Z;
								
								m_Animate.m_RightHSRbt.m_dEqCurPos_Z = m_Animate.m_RightHSRbt.m_dEqCurPos_Z + n_RHSRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_RightHSRbt.m_dEqCurPos_Z = m_Animate.m_RightHSRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_CLIP_INSERT_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;

					dPosTmp2 = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ClipRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}				 
					else
					{
						if(m_Animate.m_ClipRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_ClipRbt.m_dEqCurPos_Y = m_Animate.m_ClipRbt.m_dEqCurPos_Y - 4;
							
							if(dPosTmp > m_Animate.m_ClipRbt.m_dEqCurPos_Y)
							{
								n_ClipRbtAllow = dPosTmp - m_Animate.m_ClipRbt.m_dEqCurPos_Y;
								
								m_Animate.m_ClipRbt.m_dEqCurPos_Y = m_Animate.m_ClipRbt.m_dEqCurPos_Y + n_ClipRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ClipRbt.m_dEqCurPos_Y = m_Animate.m_ClipRbt.m_dEqCurPos_Y + 4;
						}
					}
				}
				break;

			case M_CLIP_INSERT_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 15000 / 1000;
					
					dPosTmp2 = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ClipRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}				 
					else
					{
						if(m_Animate.m_ClipRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_ClipRbt.m_dEqCurPos_Z = m_Animate.m_ClipRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_ClipRbt.m_dEqCurPos_Z)
							{
								n_ClipRbtAllow = dPosTmp - m_Animate.m_ClipRbt.m_dEqCurPos_Z;
								
								m_Animate.m_ClipRbt.m_dEqCurPos_Z = m_Animate.m_ClipRbt.m_dEqCurPos_Z + n_ClipRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ClipRbt.m_dEqCurPos_Z = m_Animate.m_ClipRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_M_CLAMP_RBT_Y:  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;

					dPosTmp2 = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ClampRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_ClampRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_ClampRbt.m_dEqCurPos_Y = m_Animate.m_ClampRbt.m_dEqCurPos_Y - 4;
							
							if(dPosTmp > m_Animate.m_ClampRbt.m_dEqCurPos_Y)
							{
								n_ClampRbtAllow = dPosTmp - m_Animate.m_ClampRbt.m_dEqCurPos_Y;
								
								m_Animate.m_ClampRbt.m_dEqCurPos_Y = m_Animate.m_ClampRbt.m_dEqCurPos_Y + n_ClampRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ClampRbt.m_dEqCurPos_Y = m_Animate.m_ClampRbt.m_dEqCurPos_Y + 4;
						}
					}
				}
				break;

			case M_M_CLAMP_RBT_Z:  
				{
					dMotRate = /*dMaxPos*/ 10000 / 1000;
					
					dPosTmp2 = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_ClampRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_ClampRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_ClampRbt.m_dEqCurPos_Z = m_Animate.m_ClampRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_ClampRbt.m_dEqCurPos_Z)
							{
								n_ClampRbtAllow = dPosTmp - m_Animate.m_ClampRbt.m_dEqCurPos_Z;
								
								m_Animate.m_ClampRbt.m_dEqCurPos_Z = m_Animate.m_ClampRbt.m_dEqCurPos_Z + n_ClampRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_ClampRbt.m_dEqCurPos_Z = m_Animate.m_ClampRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_RBT_SORTER_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;

					dPosTmp2 = COMI.Get_MotCurrentPos(M_RBT_SORTER_Y);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_SorterRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_SorterRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_SorterRbt.m_dEqCurPos_Y = m_Animate.m_SorterRbt.m_dEqCurPos_Y - 4;
							
							if(dPosTmp > m_Animate.m_SorterRbt.m_dEqCurPos_Y)
							{
								n_SorterRbtAllow = dPosTmp - m_Animate.m_SorterRbt.m_dEqCurPos_Y;
								
								m_Animate.m_SorterRbt.m_dEqCurPos_Y = m_Animate.m_SorterRbt.m_dEqCurPos_Y + n_SorterRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_SorterRbt.m_dEqCurPos_Y = m_Animate.m_SorterRbt.m_dEqCurPos_Y + 4;
						}
					}
				}
				break;

			case M_RBT_SORTER_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 20000 / 1000;
					
					dPosTmp2 = COMI.Get_MotCurrentPos(M_RBT_SORTER_Z);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_SorterRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_SorterRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_SorterRbt.m_dEqCurPos_Z = m_Animate.m_SorterRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_SorterRbt.m_dEqCurPos_Z)
							{
								n_SorterRbtAllow = dPosTmp - m_Animate.m_SorterRbt.m_dEqCurPos_Z;
								
								m_Animate.m_SorterRbt.m_dEqCurPos_Z = m_Animate.m_SorterRbt.m_dEqCurPos_Z + n_SorterRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_SorterRbt.m_dEqCurPos_Z = m_Animate.m_SorterRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;

			case M_RBT_UNLOAD_Y://ok  
				{
					dMotRate = /*dMaxPos*/ 3000 / 1000;

					dPosTmp2 = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Y);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_UnloaderRbt.m_dEqCurPos_Y == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_UnloaderRbt.m_dEqCurPos_Y > dPosTmp) 
						{
							m_Animate.m_UnloaderRbt.m_dEqCurPos_Y = m_Animate.m_UnloaderRbt.m_dEqCurPos_Y - 4;
							
							if(dPosTmp > m_Animate.m_UnloaderRbt.m_dEqCurPos_Y)
							{
								n_ULDRbtAllow = dPosTmp - m_Animate.m_UnloaderRbt.m_dEqCurPos_Y;

								m_Animate.m_UnloaderRbt.m_dEqCurPos_Y = m_Animate.m_UnloaderRbt.m_dEqCurPos_Y + n_ULDRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_UnloaderRbt.m_dEqCurPos_Y = m_Animate.m_UnloaderRbt.m_dEqCurPos_Y + 4;
						}
					}
				}
				break;

			case M_RBT_UNLOAD_Z://ok  
				{
					dMotRate = /*dMaxPos*/ 10000 / 1000;
					
					dPosTmp2 = COMI.Get_MotCurrentPos(M_RBT_UNLOAD_Z);
					
					dPosTmp = (int) (dPosTmp2 / dMotRate);
					
					dPosTmp = -dPosTmp;
					
					if(m_Animate.m_UnloaderRbt.m_dEqCurPos_Z == dPosTmp)
					{
						break;
					}					 
					else
					{
						if(m_Animate.m_UnloaderRbt.m_dEqCurPos_Z > dPosTmp) 
						{
							m_Animate.m_UnloaderRbt.m_dEqCurPos_Z = m_Animate.m_UnloaderRbt.m_dEqCurPos_Z - 1;
							
							if(dPosTmp > m_Animate.m_UnloaderRbt.m_dEqCurPos_Z)
							{
								n_ULDRbtAllow = dPosTmp - m_Animate.m_UnloaderRbt.m_dEqCurPos_Z;
								
								m_Animate.m_UnloaderRbt.m_dEqCurPos_Z = m_Animate.m_UnloaderRbt.m_dEqCurPos_Z + n_ULDRbtAllow;
								break;
							}
						}
						else 
						{
							m_Animate.m_UnloaderRbt.m_dEqCurPos_Z = m_Animate.m_UnloaderRbt.m_dEqCurPos_Z + 1;
						}
					}
				}
				break;
			}
		}
	}
}

void CScreen_Main::OnBtnClipCapRemove() 
{
	CDialog_Message msg_dlg;
	int mn_response;
	
	if(m_remove_flag == TRUE)
	{
		st_msg.str_fallacy_msg = _T("Clip Cap 제거 동작중입니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("Clip Cap is removed during operation.");
		}

		mn_response = msg_dlg.DoModal();
	}
	else if(st_work.mn_run_status == dRUN)
	{
		st_msg.str_fallacy_msg = _T("장비의 상태가 RUN 상태입니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("RUN status is the status of the equipment.");
		}

		mn_response = msg_dlg.DoModal();
	}
	else
	{
		st_msg.str_fallacy_msg = _T("Clip Cap 제거 동작을 시작합니다");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("Removed the action to start Clip Cap.");
		}

		mn_response = msg_dlg.DoModal();

		MoveStepClear();
		
		m_remove_flag = TRUE;
		
		SetTimer(TM_CLIP_REMOVE, 100, NULL);
	}
}

void CScreen_Main::OnBtnClipCapReset() 
{
	CDialog_Message msg_dlg;
	int mn_response;
	
	if(m_recorvery_flag == TRUE)
	{
		st_msg.str_fallacy_msg = _T("Clip 복구 동작중 입니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("Clip of the recovery operation");
		}

		mn_response = msg_dlg.DoModal();
	}
	else if(st_work.mn_run_status == dRUN)
	{
		st_msg.str_fallacy_msg = _T("장비의 상태가 RUN 상태입니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("RUN status is the status of the equipment");
		}

		mn_response = msg_dlg.DoModal();
	}
	else
	{
		st_msg.str_fallacy_msg = _T("Clip 복구 동작을 시작합니다");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("Clip the recovery operation starts");
		}

		mn_response = msg_dlg.DoModal();
		
		MoveStepClear();

		m_recorvery_flag = TRUE;
		
		SetTimer(TM_CLIP_RECOVERY, 100, NULL);		
	}
}

void CScreen_Main::EnableButton(int iEnable)
{
	m_clip_remove.EnableWindow(iEnable);
	m_clip_recovery.EnableWindow(iEnable);
}

void CScreen_Main::MoveStepClear()
{
	m_remove_step = 0;
	m_recorvery_step = 0;
	mn_Clipretry = 0;
	ClipBufferStep = 0;
	n_cliprecovery_step = 0;
	n_clipremove_step = 0;
}

int CScreen_Main::ClipRemove()
{
	int n_remove_flag = CTLBD_RET_PROCEED;
	int nRet_1;

	switch(m_remove_step)
	{
//	case 0:
//		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
//		m_remove_step = 100;
//		break;

//	case 100:
//		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
//			IO_OFF, st_wait.i_stacker_clip_w[69], st_wait.i_stacker_clip_l);
//		
//		if(nRet_1 == CTLBD_RET_GOOD)
//		{
//			m_remove_step = 200;
//		}
//		else if(nRet_1 != CTLBD_RET_PROCEED)
//		{
//			mn_retry++;
//			if(mn_retry > 3)
//			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
//				mn_retry = 0;
//				sprintf(mc_alarmcode,"180801");
//				CTL_Lib.Alarm_Error_Occurrence(8174, CTL_dWARNING, mc_alarmcode);
//				m_remove_step = 0;
//				n_remove_flag = CTLBD_RET_ERROR;
//			}
//			else
//			{
//				m_remove_step = 0;
//			}
//		}
//		break;

	case 0:
//		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_fwd_onoff, IO_ON);
//		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_latch_bwd_onoff, IO_OFF);

		nRet_1 = LdUldClipBufferPos(ULD_CLIP_POS);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_remove_step = 100;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			n_remove_flag = CTLBD_RET_ERROR;
		}
		break;

	case 100:
		m_remove_step = 110;
		break;

	case 110:
		m_remove_step = 120;
		break;
		
	case 120:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		m_remove_step = 180;
		break;

	case 180:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_fwd_chk, st_io.i_tube_push_bwd_chk,
			IO_OFF, st_wait.n_on_wait_time[66], st_wait.n_limit_wait_time[66]);
		if(nRet_1 == RET_GOOD)
		{
			m_remove_step = 200;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180100 0 18 "로드 튜브 stacker latch On 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{//180900 0 18 "튜브 Pusher forward 체크 에러가 발생했습니다."
			//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 언로드 튜브 stacker latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180901");
				CTL_Lib.Alarm_Error_Occurrence(1503, CTL_dWARNING, mc_alarmcode);
				m_remove_step = 150;
				n_remove_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_remove_step = 150;
			}
		}		
		break;

	case 200:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, IO_OFF);
		m_remove_step = 300;
		break;

	case 300:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_0_onoff, st_io.i_tube_rotator_0_chk, st_io.i_tube_rotator_90_chk,
			IO_ON, st_wait.n_on_wait_time[59], st_wait.n_limit_wait_time[59]);
		if(nRet_1 == RET_GOOD)
		{
			m_remove_step = 400;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180706 0 18 "로테이터 튜브 0도 에러가 발생했습니다."
			 //180707 0 18 "로테이터 튜브 90도 에러가 발생했습니다."
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 0도 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180706");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1509, st_work.mn_run_status, mc_alarmcode);
				m_remove_step = 200;
				n_remove_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_remove_step = 200;
			}
		}
		break;	
 
	case 400:
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_ON);
		m_remove_step = 450;
		break;

	case 450:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_unclamp_chk, st_io.i_tube_rotator_clamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			m_remove_step = 500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180502 0 18 "로드 튜브 rotator clamp 체크 에러가 발생했습니다."
		//180503 0 18 "로드 튜브 rotator unclamp 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode,"180503");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1522, st_work.mn_run_status, mc_alarmcode);			
				m_remove_step = 400;
				n_remove_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_remove_step = 400;
			}
		}
		break;

	case 500:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
		}
		else
		{
		}
		m_remove_step = 550;
		break;

	case 550:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
				IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
			if(nRet_1 == RET_GOOD)
			{
				m_remove_step = 600;
			}
			else if(nRet_1 != RET_PROCEED)
			{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					sprintf(mc_alarmcode,"180902");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1534, st_work.mn_run_status, mc_alarmcode);
					m_remove_step = 500;
					n_remove_flag = CTLBD_RET_ERROR;
				}
				else
				{
					m_remove_step = 500;
				}
			}
		}
		else
		{
			m_remove_step = 600;
		}
		break;

	case 600:
//		nRet_1 = LdUldClipBufferPos(ULD_CLIP_POS);
//		if(nRet_1 == CTLBD_RET_GOOD)
//		{
			m_remove_step = 700;
//		}
//		else if(nRet_1 == CTLBD_RET_ERROR)
//		{
//			n_remove_flag = CTLBD_RET_ERROR;
//		}
		break;

	case 700:
//		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		m_remove_step = 800;
		break;
		
	case 800:
//		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
//			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
//		
//		if(nRet_1 == CTLBD_RET_GOOD)
//		{
			m_remove_step = 900;
//		}
//		else if(nRet_1 != CTLBD_RET_PROCEED)
//		{
//			mn_retry++;
//			if(mn_retry > 3)
//			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
//				mn_retry = 0;
//				sprintf(mc_alarmcode,"180801");
//				CTL_Lib.Alarm_Error_Occurrence(8174, CTL_dWARNING, mc_alarmcode);
//				m_remove_step = 800;
//				n_remove_flag = CTLBD_RET_ERROR;
//			}
//			else
//			{
//				m_remove_step = 800;
//			}
//		}
		break; 

	case 900:
//		nRet_1 = OnRemoveClip();
//		if(nRet_1 == CTLBD_RET_GOOD)
//		{
			m_remove_step = 1000;
//		}
//		else if(nRet_1 == CTLBD_RET_ERROR)
//		{
//			m_remove_step = 900;
//			n_remove_flag = CTLBD_RET_ERROR;
//		}
		break;

	case 1000:
		nRet_1 = LdUldClipBufferPos(LD_CLIP_POS);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_remove_step = 1100;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			m_remove_step = 1000;
			n_remove_flag = CTLBD_RET_ERROR;
		}
		break;

	case 1100:
		n_remove_flag = CTLBD_RET_GOOD;
		break;
	}

	return n_remove_flag;
}

int CScreen_Main::ClipRecovery()
{
	int n_recovery_flag = CTLBD_RET_PROCEED;
	int nRet_1;

	switch(m_recorvery_step)
	{
	case 0:
//		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_fwd_onoff, IO_OFF);
		FAS_IO.set_out_bit(st_io.o_tube_s_m_plate_bwd_onoff, IO_ON);
		m_recorvery_step = 100;
		break;
		
	case 100:
//		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
//			IO_OFF, st_wait.i_stacker_clip_w[69], st_wait.i_stacker_clip_l);
//		
//		if(nRet_1 == CTLBD_RET_GOOD)
//		{
			m_recorvery_step = 150;
//		}
//		else if(nRet_1 != CTLBD_RET_PROCEED)
//		{
//			mn_retry++;
//			if(mn_retry > 3)
//			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
//				mn_retry = 0;
//				sprintf(mc_alarmcode,"180801");
//				CTL_Lib.Alarm_Error_Occurrence(8174, CTL_dWARNING, mc_alarmcode);
//				m_recorvery_step = 0;
//				n_recovery_flag = CTLBD_RET_ERROR;
//			}
//			else
//			{
//				m_recorvery_step = 0;
//			}
//		}
		break;

	case 150:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_ON);
		m_recorvery_step = 200;
		break;

	case 200:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_fwd_chk, st_io.i_tube_push_bwd_chk,
			IO_ON, st_wait.n_on_wait_time[66], st_wait.n_limit_wait_time[66]);
		if(nRet_1 == RET_GOOD)
		{
			m_recorvery_step = 230;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180100 0 18 "로드 튜브 stacker latch On 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{//180900 0 18 "튜브 Pusher forward 체크 에러가 발생했습니다."
			//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 언로드 튜브 stacker latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180901");
				CTL_Lib.Alarm_Error_Occurrence(1503, CTL_dWARNING, mc_alarmcode);
				m_recorvery_step = 150;
				n_recovery_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_recorvery_step = 150;
			}
		}		
		break;

	case 230:	
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_clamp_onoff, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_unclamp_onoff, IO_OFF);
		m_recorvery_step = 250;
		break;

	case 250:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_clamp_onoff, st_io.i_tube_rotator_clamp_chk, st_io.i_tube_rotator_unclamp_chk,
			IO_ON, st_wait.n_on_wait_time[61], st_wait.n_limit_wait_time[61]);
		if(nRet_1 == RET_GOOD)
		{
			m_recorvery_step = 300;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180502 0 18 "로드 튜브 rotator clamp 체크 에러가 발생했습니다."
		//180503 0 18 "로드 튜브 rotator unclamp 체크 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{
				mn_retry = 0;
				sprintf(mc_alarmcode, "180503");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1523, CTL_Lib.mn_run_status, mc_alarmcode);
				m_recorvery_step = 230;
				n_recovery_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_recorvery_step = 230;
			}
		}
		break;

	case 300:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_ON);
		}
		else
		{
		}
		m_recorvery_step = 350;
		break;

	case 350:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_on_chk, st_io.i_tube_rotator_cap_remove_off_chk,
				IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
			if(nRet_1 == RET_GOOD)
			{
				mn_retry = 0;
				m_recorvery_step = 400;
			}
			else if(nRet_1 != RET_PROCEED)
			{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
				mn_retry++;
				if(mn_retry > 6)
				{
					mn_retry = 0;
					FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
					sprintf(mc_alarmcode,"180902");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1533, st_work.mn_run_status, mc_alarmcode);	
					m_recorvery_step = 2730;
					n_recovery_flag = CTLBD_RET_ERROR;
				}
				else
				{
					FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
					m_recorvery_step = 370;
				}
			}
		}
		else
		{
			m_recorvery_step = 400;
		}
		break;

	case 370:
		// 20140220
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
			IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
		if(nRet_1 == RET_GOOD)
		{
			m_recorvery_step = 300;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			m_recorvery_step = 300;
		}
		break;

	case 400:
		FAS_IO.OnCylinderAction(st_io.o_uld_tube_pusher_fwdbwd_onoff, IO_OFF);
		m_recorvery_step = 450;
		break;

	case 450:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_uld_tube_pusher_fwdbwd_onoff, st_io.i_tube_push_bwd_chk, st_io.i_tube_push_fwd_chk,
			IO_ON, st_wait.n_on_wait_time[66], st_wait.n_limit_wait_time[66]);
		if(nRet_1 == RET_GOOD)
		{
			mn_retry = 0;
			m_recorvery_step = 500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180100 0 18 "로드 튜브 stacker latch On 에러가 발생했습니다."
			mn_retry++;
			if(mn_retry > 3)
			{//180900 0 18 "튜브 Pusher forward 체크 에러가 발생했습니다."
			//180901 0 18 "튜브 Pusher backward 체크 에러가 발생했습니다."
				mn_retry = 0;
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 언로드 튜브 stacker latch On 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180901");
				CTL_Lib.Alarm_Error_Occurrence(1503, CTL_dWARNING, mc_alarmcode);
				m_recorvery_step = 400;
				n_recovery_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_recorvery_step = 400;
			}
		}		
		break;

	case 500:
//		if(st_handler.n_loadCenterChk == CTL_NO)
//		{
//			n_recovery_flag = CTLBD_RET_GOOD;
//			break;
//		}

		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_0_onoff, IO_OFF);
		FAS_IO.OnCylinderAction(st_io.o_tube_rotator_90_onoff, IO_ON);
		m_recorvery_step = 530;
		break;

	case 530:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_0_onoff, st_io.i_tube_rotator_90_chk, st_io.i_tube_rotator_0_chk,
			IO_ON, st_wait.n_on_wait_time[59], st_wait.n_limit_wait_time[59]);
		if(nRet_1 == RET_GOOD)
		{
			m_recorvery_step = 550;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_retry++;
			if(mn_retry > 3)
			{//180706 0 18 "로테이터 튜브 0도 에러가 발생했습니다."
			 //180707 0 18 "로테이터 튜브 90도 에러가 발생했습니다."
//				if(st_handler.cwnd_list != NULL)
//				{
//					sprintf(st_msg.c_abnormal_msg,"[Initialize Error] 로테이터 튜브 0도 에러가 발생했습니다.");
//					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
//				}
				sprintf(mc_alarmcode,"180707");
				st_work.mn_run_status = CTL_dWARNING;
				CTL_Lib.Alarm_Error_Occurrence(1509, st_work.mn_run_status, mc_alarmcode);
				m_recorvery_step = 500;
				n_recovery_flag = CTLBD_RET_ERROR;
			}
			else
			{
				m_recorvery_step = 500;
			}
		}
		break;	
		
	case 550:
//		nRet_1 = LdUldClipBufferPos(ULD_CLIP_POS);
//		if(nRet_1 == CTLBD_RET_GOOD)
//		{
			m_recorvery_step = 600;
//		}
//		else if(nRet_1 == CTLBD_RET_ERROR)
//		{
//			n_recovery_flag = CTLBD_RET_ERROR;
//		}
		break;
		
	case 600:
//		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_ON);
		m_recorvery_step = 700;
		break;
		
	case 700:
//		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
//			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
//		
//		if(nRet_1 == CTLBD_RET_GOOD)
//		{
			m_recorvery_step = 800;
//		}
//		else if(nRet_1 != CTLBD_RET_PROCEED)
//		{
//			mn_retry++;
//			if(mn_retry > 3)
//			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
//				mn_retry = 0;
//				sprintf(mc_alarmcode,"180801");
//				CTL_Lib.Alarm_Error_Occurrence(8174, CTL_dWARNING, mc_alarmcode);
//				m_recorvery_step = 600;
//				n_recovery_flag = CTLBD_RET_ERROR;
//			}
//			else
//			{
//				m_recorvery_step = 600;
//			}
//		}
		break;

	case 800:
//		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		m_recorvery_step = 900;
		break;

	case 900:
//		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_up_chk, st_io.i_clip_buff_dn_chk,
//			IO_OFF, st_wait.i_stacker_clip_w[69], st_wait.i_stacker_clip_l);
//		
//		if(nRet_1 == CTLBD_RET_GOOD)
//		{
			m_recorvery_step = 1000;
//		}
//		else if(nRet_1 != CTLBD_RET_PROCEED)
//		{
//			mn_retry++;
//			if(mn_retry > 3)
//			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
//				mn_retry = 0;
//				sprintf(mc_alarmcode,"180801");
//				CTL_Lib.Alarm_Error_Occurrence(8174, CTL_dWARNING, mc_alarmcode);
//				m_recorvery_step = 800;
//				n_recovery_flag = CTLBD_RET_ERROR;
//			}
//			else
//			{
//				m_recorvery_step = 800;
//			}
//		}
		break;

	case 1000:
		nRet_1 = LdUldClipBufferPos(LD_CLIP_POS);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_recorvery_step = 1100;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			m_recorvery_step = 1000;
			n_recovery_flag = CTLBD_RET_ERROR;
		}
		break;

///////////////////////////////////////////////////////////////////////////////////////////////
	case 1100:
		if(st_basic.n_mode_cap_remove == 1)
		{
			FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF); // 20140220
		}
		else
		{
		}
		m_recorvery_step = 1200;
		break;

	case 1200:
		// 20140220
		if(st_basic.n_mode_cap_remove == 1)
		{
			nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_tube_rotator_cap_remove_onoff, st_io.i_tube_rotator_cap_remove_off_chk, st_io.i_tube_rotator_cap_remove_on_chk,
				IO_ON, st_wait.n_on_wait_time[62], st_wait.n_limit_wait_time[62]);
			if(nRet_1 == RET_GOOD)
			{
				m_recorvery_step = 1300;
			}
			else if(nRet_1 != RET_PROCEED)
			{//180902 0 18 "튜브 rotator cap 제거에 에러가 발생했습니다."
				mn_retry++;
				if(mn_retry > 3)
				{
					mn_retry = 0;
					FAS_IO.set_out_bit(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
					sprintf(mc_alarmcode,"180902");
					st_work.mn_run_status = CTL_dWARNING;
					CTL_Lib.Alarm_Error_Occurrence(1539, st_work.mn_run_status, mc_alarmcode);	
					m_recorvery_step = 1200;
				}
				else
				{
					FAS_IO.OnCylinderAction(st_io.o_tube_rotator_cap_remove_onoff, IO_OFF);
					m_recorvery_step = 1200;
				}
			}
		}
		else
		{
			m_recorvery_step = 1300;
		}
		break;

	case 1300:
		n_recovery_flag = CTLBD_RET_GOOD;
		break;
	}

	return n_recovery_flag;
}

int CScreen_Main::LdUldClipBufferPos(int nPos)
{// nPos : 0 safety 1 : ld 2 : uld  
	int nRet_1,nFuncRet = RET_PROCEED;
	
	switch(ClipBufferStep)
	{
	case 0:
		if(FAS_IO.get_in_bit(st_io.i_clip_buff_up_chk, IO_OFF) == IO_OFF && 
			FAS_IO.get_in_bit(st_io.i_clip_buff_dn_chk, IO_ON) == IO_ON)
		{
			ClipBufferStep = 1000;
		}
		else
		{
			mn_Clipretry = 0;
			FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
			ClipBufferStep = 100;
		}
		break;
		
	case 10:
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		ClipBufferStep = 100;
		break;
		
	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.i_stacker_clip_w[1], st_wait.i_stacker_clip_l);
		if(nRet_1 == RET_GOOD)
		{
			ClipBufferStep = 1000;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{////180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"180801");
				CTL_Lib.Alarm_Error_Occurrence(8176, CTL_dWARNING, mc_alarmcode);
				FAS_IO.set_out_bit(st_io.o_clip_buffer_updn_onoff, IO_ON);
				nFuncRet = RET_ERROR;
				ClipBufferStep = 10;
			}
			else
			{
				ClipBufferStep = 10;
			}
		}
		break;
		
	case 1000:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].md_pos[nPos], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			ClipBufferStep = 2000;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			ClipBufferStep = 1000;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			ClipBufferStep = 1000;
			CTL_Lib.Alarm_Error_Occurrence(8177, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = CTLBD_RET_ERROR;
		}
		break;
		
	case 2000:
		ClipBufferStep = 0;
		nFuncRet = CTLBD_RET_GOOD;
		break;
	}
	
	return nFuncRet;
}

int CScreen_Main::OnRemoveClip()
{
	int nRet_1;
	int nFuncRet = CTLBD_RET_PROCEED;

	switch(n_clipremove_step)
	{
	case 0:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			n_clipremove_step = 100;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			n_clipremove_step = 0;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(8168, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = CTLBD_RET_ERROR;
			n_clipremove_step = 0;
		}		
		break;
		
	case 100:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			n_clipremove_step = 200;
		}
		else if(nRet_1 == BD_RETRY)
		{
			n_clipremove_step = 100;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(8169, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
			n_clipremove_step = 100;
		}
		break;
		
	case 200:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_PICKPOS], st_basic.nManualSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)			
		{
			n_clipremove_step = 300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			n_clipremove_step = 200;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);	
		}
		break;

	case 300:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z , st_motor[M_CLIP_INSERT_Z].md_pos[LD_CLIP_POS], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			n_clipremove_step = 400;
		}
		else if(nRet_1 == CTLBD_RET_RETRY)
		{
			n_clipremove_step = 300;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			n_clipremove_step = 300;
			CTL_Lib.Alarm_Error_Occurrence(2377, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = CTLBD_RET_ERROR;
		}
		break;

	case 400:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_MIDCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			n_clipremove_step = 500;
		}
		else if(nRet_1 == BD_RETRY)
		{
			n_clipremove_step = 400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			n_clipremove_step = 400;
			CTL_Lib.Alarm_Error_Occurrence(2398, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = CTLBD_RET_ERROR;
		}
		break;

	case 500:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			n_clipremove_step = 600;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			n_clipremove_step = 500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(8168, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = CTLBD_RET_ERROR;
			n_clipremove_step = 500;
		}		
		break;

	case 600:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_REMOVE], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			n_clipremove_step = 700;
		}
		else if(nRet_1 == BD_RETRY)
		{
			n_clipremove_step = 600;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(2392, CTL_dWARNING, COMI.mc_alarmcode);
			n_clipremove_step = 600;
			nFuncRet = CTLBD_RET_ERROR;
		}
		break;

	case 700:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].md_pos[CLIP_UNCLAMP], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			n_clipremove_step = 800;
		}
		else if(nRet_1 == BD_RETRY)
		{
			n_clipremove_step = 700;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			n_clipremove_step = 700;
			CTL_Lib.Alarm_Error_Occurrence(2398, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = CTLBD_RET_ERROR;
		}
		break;

	case 800:
		FAS_IO.set_out_bit(st_io.o_clip_remove_up_onoff, IO_ON);
		FAS_IO.set_out_bit(st_io.o_clip_remove_down_onoff, IO_OFF);
		n_clipremove_step = 900;
		break;

	case 900:
		FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_on, IO_ON);
		FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_off, IO_OFF);
		l_Until_WaitTime[0] = GetCurrentTime();
		n_clipremove_step = 1000;
		break;

	case 1000:
		l_Until_WaitTime[1] = GetCurrentTime();
		l_Until_WaitTime[2] = l_Until_WaitTime[1] - l_Until_WaitTime[0];
		if(l_Until_WaitTime[2] < 0)
		{
			l_Until_WaitTime[0] = GetCurrentTime();
		}
		else if(l_Until_WaitTime[2] > 1000)
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_off, IO_ON);
			FAS_IO.OnCylinderAction(st_io.o_clip_remove_blower_on, IO_OFF);
			n_clipremove_step = 1100;
		}
		break;

	case 1100:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nRunSpeed);
		if (nRet_1 == CTLBD_RET_GOOD)
		{
			n_clipremove_step = 1200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			n_clipremove_step = 1100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다 
			CTL_Lib.Alarm_Error_Occurrence(8168, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = CTLBD_RET_ERROR;
			n_clipremove_step = 1100;
		}		
		break;
		
	case 1200:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_LD_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			n_clipremove_step = 1300;
		}
		else if(nRet_1 == BD_RETRY)
		{
			n_clipremove_step = 1200;
		}
		else if(nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{
			CTL_Lib.Alarm_Error_Occurrence(8169, CTL_dWARNING, COMI.mc_alarmcode);
			nFuncRet = RET_ERROR;
			n_clipremove_step = 1200;
		}
		break;

	case 1300:
		nFuncRet = CTLBD_RET_GOOD;
		break;
	}
	return nFuncRet;
}

/////////////// 20140207
void CScreen_Main::OnBtnMDTrayLockOnOff()
{
	CDialog_Message msg_dlg;
	int mn_response;

	if(st_work.mn_run_status != dSTOP) 
	{
		st_msg.str_fallacy_msg = _T("장비의 상태가 Stop 상태에서 사용 가능합니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("The status of the equipment available in the Stop state");
		}

		mn_response = msg_dlg.DoModal();
		return;
	}

	if(FAS_IO.get_in_bit(st_io.i_m_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON)
	{
		FAS_IO.set_out_bit(st_io.o_m_stacker2_tray_clamp_onoff, IO_OFF);
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_m_stacker2_tray_clamp_onoff, IO_ON);
	}
}

void CScreen_Main::OnBtnLHTrayLockOnOff()
{
	CDialog_Message msg_dlg;
	int mn_response;

	if(st_work.mn_run_status != dSTOP) 
	{
		st_msg.str_fallacy_msg = _T("장비의 상태가 Stop 상태에서 사용 가능합니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("The status of the equipment available in the Stop state");
		}

		mn_response = msg_dlg.DoModal();
		return;
	}

	if(FAS_IO.get_in_bit(st_io.i_hs_fwd_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON)
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_tray_clamp_onoff, IO_OFF);
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_hs_fwd_stacker2_tray_clamp_onoff, IO_ON);
	}
}

void CScreen_Main::OnBtnRHTrayLockOnOff()
{
	CDialog_Message msg_dlg;
	int mn_response;

	if(st_work.mn_run_status != dSTOP)
	{
		st_msg.str_fallacy_msg = _T("장비의 상태가 Stop 상태에서 사용 가능합니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("The status of the equipment available in the Stop state");
		}

		mn_response = msg_dlg.DoModal();
		return;
	}

	if(FAS_IO.get_in_bit(st_io.i_hs_bwd_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON)
	{
		FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker2_tray_clamp_onoff, IO_OFF);
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_hs_bwd_stacker2_tray_clamp_onoff, IO_ON);
	}
}

void CScreen_Main::OnBtnMDStackerDown()
{
	CDialog_Message msg_dlg;
	int mn_response;

	if(st_work.mn_run_status != dSTOP)
	{
		st_msg.str_fallacy_msg = _T("장비의 상태가 Stop 상태에서 사용 가능합니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("The status of the equipment available in the Stop state");
		}

		mn_response = msg_dlg.DoModal();
		return;
	}

	st_msg.str_fallacy_msg = _T("Stacker Down을 시작합니다.");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		st_msg.str_fallacy_msg = _T("Start a Stacker Down");
	}

	mn_response = msg_dlg.DoModal();

	if(mn_response == IDOK)
	{
		st_work.mn_stacker_num = 0;
		m_stacker_step = 0;

		SetTimer(TM_STACKER_DOWN, 100, NULL);
	}
}

void CScreen_Main::OnBtnLHStackerDown()
{
	CDialog_Message msg_dlg;
	int mn_response;

	if(st_work.mn_run_status != dSTOP)
	{
		st_msg.str_fallacy_msg = _T("장비의 상태가 Stop 상태에서 사용 가능합니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("The status of the equipment available in the Stop state");
		}

		mn_response = msg_dlg.DoModal();
		return;
	}

	st_msg.str_fallacy_msg = _T("Stacker Down을 시작합니다.");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		st_msg.str_fallacy_msg = _T("Start a Stacker Down");
	}

	mn_response = msg_dlg.DoModal();

	if(mn_response == IDOK)
	{
		st_work.mn_stacker_num = 3;
		m_stacker_step = 0;

		SetTimer(TM_STACKER_DOWN, 100, NULL);
	}
}

void CScreen_Main::OnBtnRHStackerDown()
{
	CDialog_Message msg_dlg;
	int mn_response;

	if(st_work.mn_run_status != dSTOP)
	{
		st_msg.str_fallacy_msg = _T("장비의 상태가 Stop 상태에서 사용 가능합니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("The status of the equipment available in the Stop state");
		}

		mn_response = msg_dlg.DoModal();
		return;
	}

	st_msg.str_fallacy_msg = _T("Stacker Down을 시작합니다.");
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		st_msg.str_fallacy_msg = _T("Start a Stacker Down");
	}

	mn_response = msg_dlg.DoModal();

	if(mn_response == IDOK)
	{
		st_work.mn_stacker_num = 6;
		m_stacker_step = 0;

		SetTimer(TM_STACKER_DOWN, 100, NULL);
	}
}

int CScreen_Main::OnStackerDown(int nMotorNum)
{
	CDialog_Message msg_dlg;
	int nRet_1, mn_stacker_flag = CTLBD_RET_PROCEED;

	switch(m_stacker_step)
	{
	case 0:
		m_stacker_step = 100;
		break;

	case 100:
		nRet_1 = COMI.Start_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_DOWN], st_basic.nRunSpeed);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_stacker_step = 200;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			m_stacker_step = 100;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			mn_stacker_flag = CTLBD_RET_ERROR;
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(1088, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 200:
		nRet_1 = COMI.Check_SingleMove(nMotorNum, st_motor[nMotorNum].md_pos[P_STOCKER_DOWN]);
		if(nRet_1 == CTLBD_RET_GOOD)
		{
			m_stacker_step = 300;
		}
		else if(nRet_1 == CTLBD_RET_ERROR)
		{
			mn_stacker_flag = CTLBD_RET_ERROR;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)		// 재 동작을 시도한다면...
		{
			m_stacker_step = 100;
		}
		break;

	case 300:
		mn_stacker_flag = CTLBD_RET_GOOD;
		break;
	}

	return mn_stacker_flag;
}
////////////

void CScreen_Main::OnBtnSerialNo() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_LOT_NO))->GetWindowText(st_NW.mstr_Test_LotNo);
	((CEdit*)GetDlgItem(IDC_EDIT_SERIAL_NO))->GetWindowText(st_NW.mstr_Test_SerialNo);
	

	st_work.m_strLotNoChk[100] = st_NW.mstr_Test_LotNo;
	st_work.m_strBarcode_OutPirnt[100] = st_NW.mstr_Test_SerialNo;

}

void CScreen_Main::OnSetfocusEditSerialNo() 
{
	// TODO: Add your control notification handler code here
	
}

void CScreen_Main::OnSetfocusEditLotNo() 
{
	// TODO: Add your control notification handler code here
	
}

void CScreen_Main::OnBtnUmdStacker2TrayLockOnoff() //2014,0408
{
	// TODO: Add your control notification handler code here
	CDialog_Message msg_dlg;
	int mn_response;
	
	if(st_work.mn_run_status != dSTOP) 
	{
		st_msg.str_fallacy_msg = _T("장비의 상태가 Stop 상태에서 사용 가능합니다.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("The status of the equipment available in the Stop state");
		}
		
		mn_response = msg_dlg.DoModal();
		return;
	}
	
	if(FAS_IO.get_in_bit(st_io.i_uld_stacker2_tray_clamp_on_chk, IO_ON) == IO_ON)
	{
		FAS_IO.set_out_bit(st_io.o_uld_stacker2_tray_clamp_onoff, IO_OFF);
	}
	else
	{
		FAS_IO.set_out_bit(st_io.o_uld_stacker2_tray_clamp_onoff, IO_ON);
	}	
	
}

void CScreen_Main::OnBtnBarcodePrintReset() 
{
	// TODO: Add your control notification handler code here
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
