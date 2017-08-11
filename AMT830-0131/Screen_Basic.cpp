// Screen_Basic.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Basic.h"
#include "AMTLotManager.h"

#include "io.h"  // 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 INCLUDE 필요

// ******************************************************************************
// 대화 상자 클래스 추가
// ******************************************************************************
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyBoard.h"
#include "Dialog_Pass_Check.h"
#include "Keyboarddll.h"

#include "Dialog_KeyPad.h"
#include "Dialog_Work_buffer.h"
// ******************************************************************************
////2014.0818
#include "Run_Rbt_Load_Module.h"
#include "Run_Rbt_Load_LeftHeat.h"
#include "Run_Rbt_Load_RightHeat.h"
#include "Run_Rbt_Load_Clip.h"
#include "Run_Rbt_Sorter.h"
////



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TM_MANUAL_BCR_HS_YIELD_TEST			994	

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic

IMPLEMENT_DYNCREATE(CScreen_Basic, CFormView)

CScreen_Basic::CScreen_Basic()
	: CFormView(CScreen_Basic::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Basic)
	//}}AFX_DATA_INIT
	st_handler.nModelChangeCheck = FALSE; 
}

CScreen_Basic::~CScreen_Basic()
{
}

void CScreen_Basic::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Basic)
	DDX_Control(pDX, IDC_MSG_VISION_HS_TOTAL_ERR, m_msg_vision_hs_total_err);
	DDX_Control(pDX, IDC_MSG_VISION_HS_CONTINUE_ERR, m_msg_vision_hs_cont_err);
	DDX_Control(pDX, IDC_CHK_VISION_HS_CONTINUE, m_chk_use_vision_hs_continue);
	DDX_Control(pDX, IDC_GROUP_DVC_REPICK_CNT2, m_group_vison_hs_cont);
	DDX_Control(pDX, IDC_LIST_PART_TYPE, m_list_part_type);
	DDX_Control(pDX, IDC_BTN_PART_DELETE, m_btn_part_delete);
	DDX_Control(pDX, IDC_BTN_PART_CREATE, m_btn_part_create);
	DDX_Control(pDX, IDC_GROUP_PART_NUMBER, m_group_part_number);
	DDX_Control(pDX, IDC_DGT_CURR_CLAMP_CLIP_BIN, m_dgt_curr_clamp_clip_bin);
	DDX_Control(pDX, IDC_DGT_CURR_CLIP_BIN, m_dgt_curr_clip_bin);
	DDX_Control(pDX, IDC_MSG_STACK_CLIP, m_msg_stack_clip);
	DDX_Control(pDX, IDC_MSG_SET_CLIP, m_msg_set_clip);
	DDX_Control(pDX, IDC_DGT_CLIP_BIN, m_dgt_clip_bin);
	DDX_Control(pDX, IDC_DGT_CLAMP_CLIP_BIN, m_dgt_clamp_clip_bin);
	DDX_Control(pDX, IDC_MSG_CLIP_BIN, m_msg_clip_bin);
	DDX_Control(pDX, IDC_MSG_CLAMP_CLIP_BIN, m_msg_clamp_clip_bin);
	DDX_Control(pDX, IDC_GROUP_CLIP_BIN, m_group_clip_bin);
	DDX_Control(pDX, IDC_CHK_M_REMOVE_CLIP, m_chk_remove_clip);
	DDX_Control(pDX, IDC_CHK_CLIP_ALARM, m_chk_clip_alarm);
	DDX_Control(pDX, IDC_CHK_CAP_REMOVE, m_chk_cap_remove); // 20140220
	DDX_Control(pDX, IDC_CHK_DOOR_LOCK, m_chk_door_lock);
	DDX_Control(pDX, IDC_CHK_USE_SORTER_PICKER, m_chk_use_sorter_picker);
	DDX_Control(pDX, IDC_GROUP_USE_BIN, m_group_use_bin);
	DDX_Control(pDX, IDC_GROUP_USE_DOOR, m_group_use_door);
	DDX_Control(pDX, IDC_GROUP_USE_SORTER_PICKER, m_group_use_sorter_picker);
	DDX_Control(pDX, IDC_CHK_M_BCR, m_chk_bcr);
	DDX_Control(pDX, IDC_GROUP_BARCODE, m_group_bcr);
	DDX_Control(pDX, IDC_CHK_HS_DIRECTION, m_chk_hs_direction);
	DDX_Control(pDX, IDC_CHK_M_DIRECTION, m_chk_m_direction);
	DDX_Control(pDX, IDC_GROUP_MODULE_HEAT_DIRECTION, m_group_mhs_direction);
	DDX_Control(pDX, IDC_RADIO_TRAY_MODE_USE, m_radio_notray);
	DDX_Control(pDX, IDC_GROUP_RECIPE_SELECT, m_group_recipe_select);
	DDX_Control(pDX, IDC_MSG_TRAY_Y, m_msg_tray_y);
	DDX_Control(pDX, IDC_MSG_TRAY_X, m_msg_tray_x);
	DDX_Control(pDX, IDC_GROUP_TRAY_SIZE_SETTING, m_group_tray_size_setting);
	DDX_Control(pDX, IDC_GROUP_ALARM_DELAY_TIME, m_group_alarm_delay_time);
	DDX_Control(pDX, IDC_GROUP_NETWORK_WAIT_TIME, m_group_network_wait_time);
	DDX_Control(pDX, IDC_GROUP_NETWORK_RETRY_COUNT, m_group_network_retry_count);
	DDX_Control(pDX, IDC_LABEL_ELECTROSTATIC_5, m_label_electrostatic_5);
	DDX_Control(pDX, IDC_LABEL_ELECTROSTATIC_4, m_label_electrostatic_4);
	DDX_Control(pDX, IDC_LABEL_ELECTROSTATIC_3, m_label_electrostatic_3);
	DDX_Control(pDX, IDC_LABEL_ELECTROSTATIC_2, m_label_electrostatic_2);
	DDX_Control(pDX, IDC_LABEL_ELECTROSTATIC_1, m_label_electrostatic_1);
	DDX_Control(pDX, IDC_MSG_ELECTROSTATIC_5, m_msg_electrostatic_5);
	DDX_Control(pDX, IDC_MSG_ELECTROSTATIC_4, m_msg_electrostatic_4);
	DDX_Control(pDX, IDC_MSG_ELECTROSTATIC_3, m_msg_electrostatic_3);
	DDX_Control(pDX, IDC_MSG_ELECTROSTATIC_2, m_msg_electrostatic_2);
	DDX_Control(pDX, IDC_MSG_ELECTROSTATIC_1, m_msg_electrostatic_1);
	DDX_Control(pDX, IDC_RADIO_DEVICE_MODE_USE, m_radio_device_mode_use);
	DDX_Control(pDX, IDC_RADIO_DEVICE_MODE_NOTUSE, m_radio_device_mode_notuse);
	DDX_Control(pDX, IDC_DGT_ELECTROSTATIC_CHECK_VALUE, m_dgt_electrostatic_check_value);
	DDX_Control(pDX, IDC_LIST_DEVICE_TYPE, m_list_device_type);
	DDX_Control(pDX, IDC_GROUP_ELECTROSTATIC_TEST, m_group_electrostatic_test);
	DDX_Control(pDX, IDC_GROUP_ELECTROSTATIC_CHECK_VALUE, m_group_electrostatic_check_value);
	DDX_Control(pDX, IDC_GROUP_ELECTROSTATIC_CHECK_GAP, m_group_electrostatic_check_gap);
	DDX_Control(pDX, IDC_GROUP_DEVICE_MODE, m_group_device_mode);
	DDX_Control(pDX, IDC_BTN_ELECTROSTATIC_TEST, m_btn_electostatic_test);
	DDX_Control(pDX, IDC_BTN_DVC_DELETE, m_btn_dvc_delete);
	DDX_Control(pDX, IDC_BTN_DVC_CREATE, m_btn_dvc_create);
	DDX_Control(pDX, IDC_BTN_BASIC_CANCLE, m_btn_basic_cancel);
	DDX_Control(pDX, IDC_BTN_BASIC_APPLY, m_btn_basic_apply);
	DDX_Control(pDX, IDC_MSG_LOTSET_MODULE_LOAD, m_msg_lotset_Module_load);
	DDX_Control(pDX, IDC_MSG_LOTSET_HS_LEFT, m_msg_lotset_hs_left);
	DDX_Control(pDX, IDC_MSG_LOTSET_HS_RIGHT, m_msg_lotset_hs_right);
	DDX_Control(pDX, IDC_MSG_LOTSET_CLIP, m_msg_lotset_clip);
	DDX_Control(pDX, IDC_MSG_LOTSET_UNLOADER, m_msg_lotset_unloader);//20120707
	DDX_Control(pDX, IDC_MSG_LOTSET_REJECT, m_msg_lotset_reject);//20120707
	DDX_Control(pDX, IDC_GROUP_LOTSET, m_group_lotset);
	DDX_Control(pDX, IDC_GROUP_SPEED, m_group_basic_speed);
	DDX_Control(pDX, IDC_GROUP_RUN_SPEED, m_group_basic_run_speed);
	DDX_Control(pDX, IDC_GROUP_MANUAL_SPEED, m_group_basic_manual_speed);
	DDX_Control(pDX, IDC_GROUP_JOG_SPEED, m_group_basic_jog_speed);
	DDX_Control(pDX, IDC_GROUP_BARCODE_SPEED, m_group_basic_barcode_speed);
	DDX_Control(pDX, IDC_GROUP_LABEL_INFO, m_group_label_info);
	DDX_Control(pDX, IDC_BTN_LABEL_CREATE, m_btn_label_create);
	DDX_Control(pDX, IDC_BTN_LABEL_DELETE, m_btn_label_delete);
	DDX_Control(pDX, IDC_LIST_LABEL_TYPE, m_list_label_type);
	DDX_Control(pDX, IDC_BTN_LABEL_CANCLE, m_btn_label_cancel);
	DDX_Control(pDX, IDC_BTN_LABEL_APPLY, m_btn_label_apply);
	DDX_Control(pDX, IDC_MSG_LABELSET_SD, m_msg_labelset_sd);
	DDX_Control(pDX, IDC_MSG_LABELSET_LT, m_msg_labelset_lt);
	DDX_Control(pDX, IDC_MSG_LABELSET_X1_POS, m_msg_labelset_x1_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_X2_POS, m_msg_labelset_x2_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_X3_POS, m_msg_labelset_x3_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_X4_POS, m_msg_labelset_x4_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_X5_POS, m_msg_labelset_x5_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_X6_POS, m_msg_labelset_x6_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y1_POS, m_msg_labelset_y1_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y2_POS, m_msg_labelset_y2_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y3_POS, m_msg_labelset_y3_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y4_POS, m_msg_labelset_y4_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y5_POS, m_msg_labelset_y5_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y6_POS, m_msg_labelset_y6_pos);
	DDX_Control(pDX, IDC_MSG_LABELSET_X1_WIDTH, m_msg_labelset_x1_width);
	DDX_Control(pDX, IDC_MSG_LABELSET_X2_WIDTH, m_msg_labelset_x2_width);
	DDX_Control(pDX, IDC_MSG_LABELSET_X3_WIDTH, m_msg_labelset_x3_width);
	DDX_Control(pDX, IDC_MSG_LABELSET_X4_WIDTH, m_msg_labelset_x4_width);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y1_HEIGHT, m_msg_labelset_y1_height);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y2_HEIGHT, m_msg_labelset_y2_height);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y3_HEIGHT, m_msg_labelset_y3_height);
	DDX_Control(pDX, IDC_MSG_LABELSET_Y4_HEIGHT, m_msg_labelset_y4_height);
	DDX_Control(pDX, IDC_GROUP_LABELSET_POS, m_group_labelset_pos);
	DDX_Control(pDX, IDC_GROUP_LABELSET_SIZE, m_group_labelset_size);
	DDX_Control(pDX, IDC_GROUP_LABEL_PRINT_ERROR_CNT, m_group_label_print_error_cnt);
	DDX_Control(pDX, IDC_GROUP_USE_CLIP_ALARM, m_group_use_clip_alarm);
	DDX_Control(pDX, IDC_GROUP_USE_CAP_REMOVE, m_group_use_cap_remove); // 20140220
	DDX_Control(pDX, IDC_GROUP_LABEL_BIN_PRINT, m_group_label_bin_print); // 20140224
	DDX_Control(pDX, IDC_GROUP_ALARM_FILE_READ, m_group_alarm_file_read);
	DDX_Control(pDX, IDC_GROUP_LABEL_VISION_PAPER, m_group_label_vision_paper);
	DDX_Control(pDX, IDC_GROUP_PICKER_GAP, m_group_picker_gap);	//20120625
	DDX_Control(pDX, IDC_GROUP_DVC_REPICK_CNT, m_group_dvc_repick_cnt);//20120731
	DDX_Control(pDX, IDC_STATIC_NODE, m_group_node);	 
	DDX_Control(pDX, IDC_DGT_PICKER_GAP_MODULE, m_dgt_picker_gap_module);
	DDX_Control(pDX, IDC_DGT_PICKER_GAP_HS_LEFT, m_dgt_picker_gap_hs_left);
	DDX_Control(pDX, IDC_DGT_PICKER_GAP_HS_RIGHT, m_dgt_picker_gap_hs_right);
	DDX_Control(pDX, IDC_MSG_PICKER_GAP_MODULE, m_msg_picker_gap_module);
	DDX_Control(pDX, IDC_MSG_PICKER_GAP_HS_LEFT, m_msg_picker_gap_hs_left);
	DDX_Control(pDX, IDC_MSG_PICKER_GAP_HS_RIGHT, m_msg_picker_gap_hs_right);
	DDX_Control(pDX, IDC_BTN_PART_SELECT, m_btn_part_select);
	DDX_Control(pDX, IDC_LIST_PART_SELECT, m_list_part_select);
	DDX_Control(pDX, IDC_EDIT_LD_NODE, m_edit_ld_node);	 
	DDX_Control(pDX, IDC_EDIT_HS_LEFT_NODE, m_edit_hs_left_node);	 
	DDX_Control(pDX, IDC_EDIT_HS_RIGHTT_NODE, m_edit_hs_right_node);	 
	DDX_Control(pDX, IDC_EDIT_CLIP_NODE, m_edit_clip_node);	 
	DDX_Control(pDX, IDC_EDIT_ULD_NODE, m_edit_uld_node);	
	DDX_Control(pDX, IDC_DGT_LABELSET_X1_POS, m_dgt_labelset_x1_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_X2_POS, m_dgt_labelset_x2_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_X3_POS, m_dgt_labelset_x3_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_X4_POS, m_dgt_labelset_x4_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_X5_POS, m_dgt_labelset_x5_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_X6_POS, m_dgt_labelset_x6_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y1_POS, m_dgt_labelset_y1_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y2_POS, m_dgt_labelset_y2_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y3_POS, m_dgt_labelset_y3_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y4_POS, m_dgt_labelset_y4_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y5_POS, m_dgt_labelset_y5_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y6_POS, m_dgt_labelset_y6_pos);
	DDX_Control(pDX, IDC_DGT_LABELSET_X1_WIDTH, m_dgt_labelset_x1_width);
	DDX_Control(pDX, IDC_DGT_LABELSET_X2_WIDTH, m_dgt_labelset_x2_width);
	DDX_Control(pDX, IDC_DGT_LABELSET_X3_WIDTH, m_dgt_labelset_x3_width);
	DDX_Control(pDX, IDC_DGT_LABELSET_X4_WIDTH, m_dgt_labelset_x4_width);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y1_HEIGHT, m_dgt_labelset_y1_height);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y2_HEIGHT, m_dgt_labelset_y2_height);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y3_HEIGHT, m_dgt_labelset_y3_height);
	DDX_Control(pDX, IDC_DGT_LABELSET_Y4_HEIGHT, m_dgt_labelset_y4_height);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Basic, CFormView)
	//{{AFX_MSG_MAP(CScreen_Basic)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_DEVICE_MODE_USE, OnRadioDeviceModeUse)
	ON_BN_CLICKED(IDC_RADIO_DEVICE_MODE_NOTUSE, OnRadioDeviceModeNotuse)
	ON_BN_CLICKED(IDC_DGT_ELECTROSTATIC_CHECK_GAP, OnDgtElectrostaticCheckGap)
	ON_BN_CLICKED(IDC_DGT_ELECTROSTATIC_CHECK_VALUE, OnDgtElectrostaticCheckValue)
	ON_BN_CLICKED(IDC_BTN_DVC_CREATE, OnBtnDvcCreate)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_TYPE, OnSelchangeListDeviceType)
	ON_BN_CLICKED(IDC_BTN_DVC_DELETE, OnBtnDvcDelete)
	ON_BN_CLICKED(IDC_BTN_ELECTROSTATIC_TEST, OnBtnElectrostaticTest)
	ON_BN_CLICKED(IDC_BTN_BASIC_APPLY, OnBtnBasicApply)
	ON_BN_CLICKED(IDC_BTN_BASIC_CANCLE, OnBtnBasicCancle)
	ON_BN_CLICKED(IDC_DGT_NETWORK_WAIT_TIME, OnDgtNetworkWaitTime)
	ON_BN_CLICKED(IDC_DGT_NETWORK_RETRY_COUNT, OnDgtNetworkRetryCount)
	ON_BN_CLICKED(IDC_DGT_ALARM_DELAY_TIME, OnDgtAlarmDelayTime)
	ON_BN_CLICKED(IDC_DGT_TRAY_X, OnDgtTrayX)
	ON_BN_CLICKED(IDC_DGT_TRAY_Y, OnDgtTrayY)
	ON_BN_CLICKED(IDC_DGT_LOTSET_MODULE_LOAD, OnDgtLotsetModuleLoad)
	ON_BN_CLICKED(IDC_DGT_LOTSET_HS_LEFT, OnDgtLotsetHsLeft)
	ON_BN_CLICKED(IDC_DGT_LOTSET_HS_RIGHT, OnDgtLotsetHsRight)
	ON_BN_CLICKED(IDC_DGT_LOTSET_CLIP, OnDgtLotsetClip)
	ON_BN_CLICKED(IDC_DGT_RUN_SPEED, OnDgtRunSpeed)
	ON_BN_CLICKED(IDC_DGT_JOG_SPEED, OnDgtJogSpeed)
	ON_BN_CLICKED(IDC_DGT_BARCODE_SPEED, OnDgtBarcodeSpeed)
	ON_BN_CLICKED(IDC_DGT_MANUAL_SPEED, OnDgtManualSpeed)
	ON_BN_CLICKED(IDC_BTN_LABEL_CREATE, OnBtnLabelCreate)
	ON_BN_CLICKED(IDC_BTN_LABEL_DELETE, OnBtnLabelDelete)
	ON_BN_CLICKED(IDC_BTN_LABEL_APPLY, OnBtnLabelApply)
	ON_BN_CLICKED(IDC_BTN_LABEL_CANCLE, OnBtnLabelCancle)
	ON_LBN_SELCHANGE(IDC_LIST_LABEL_TYPE, OnSelchangeListLabelType)
	ON_BN_CLICKED(IDC_DGT_LABELSET_SD, OnDgtLabelsetSd)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LT, OnDgtLabelsetLt)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X1_POS, OnDgtLabelsetX1Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X2_POS, OnDgtLabelsetX2Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X3_POS, OnDgtLabelsetX3Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X4_POS, OnDgtLabelsetX4Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X5_POS, OnDgtLabelsetX5Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X6_POS, OnDgtLabelsetX6Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y1_POS, OnDgtLabelsetY1Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y2_POS, OnDgtLabelsetY2Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y3_POS, OnDgtLabelsetY3Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y4_POS, OnDgtLabelsetY4Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y5_POS, OnDgtLabelsetY5Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y6_POS, OnDgtLabelsetY6Pos)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X1_WIDTH, OnDgtLabelsetX1Width)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y1_HEIGHT, OnDgtLabelsetY1Height)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X2_WIDTH, OnDgtLabelsetX2Width)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y2_HEIGHT, OnDgtLabelsetY2Height)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X3_WIDTH, OnDgtLabelsetX3Width)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y3_HEIGHT, OnDgtLabelsetY3Height)
	ON_BN_CLICKED(IDC_DGT_LABELSET_X4_WIDTH, OnDgtLabelsetX4Width)
	ON_BN_CLICKED(IDC_DGT_LABELSET_Y4_HEIGHT, OnDgtLabelsetY4Height)
	ON_BN_CLICKED(IDC_RADIO_TRAY_MODE_USE, OnRadioTrayModeUse)
	ON_BN_CLICKED(IDC_DGT_LABEL_PRINT_ERROR_CNT, OnDgtLabelPrintErrorCnt)
	ON_BN_CLICKED(IDC_DGT_PICKER_GAP_MODULE, OnDgtPickerGapModule)
	ON_BN_CLICKED(IDC_DGT_PICKER_GAP_HS_LEFT, OnDgtPickerGapHsLeft)
	ON_BN_CLICKED(IDC_DGT_PICKER_GAP_HS_RIGHT, OnDgtPickerGapHsRight)
	ON_BN_CLICKED(IDC_DGT_LABEL_VISION_PAPER, OnDgtLabelVisionPaper)
	ON_BN_CLICKED(IDC_DGT_LABEL_BIN_PRINT, OnDgtLabelBinPrint) // 20140224
	ON_BN_CLICKED(IDC_DGT_LOTSET_UNLOADER, OnDgtLotsetUnloader)
	ON_BN_CLICKED(IDC_DGT_LOTSET_REJECT, OnDgtLotsetReject)
	ON_BN_CLICKED(IDC_CHK_M_DIRECTION, OnChkMDirection)
	ON_BN_CLICKED(IDC_CHK_HS_DIRECTION, OnChkHsDirection)
	ON_BN_CLICKED(IDC_DGT_DVC_REPICK_CNT, OnDgtDvcRepickCnt)
	ON_BN_CLICKED(IDC_CHK_M_BCR, OnChkMBcr)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LH1, OnDgtLabelsetLh1)
	ON_BN_CLICKED(IDC_DGT_LABELSET_LH2, OnDgtLabelsetLh2)
	ON_BN_CLICKED(IDC_CHK_M_REMOVE_CLIP, OnChkMRemoveClip)
	ON_BN_CLICKED(IDC_DGT_CLAMP_CLIP_BIN, OnDgtClampClipBin)
	ON_BN_CLICKED(IDC_DGT_CLIP_BIN, OnDgtClipBin)
	ON_BN_CLICKED(IDC_DGT_CURR_CLAMP_CLIP_BIN, OnDgtCurrClampClipBin)
	ON_BN_CLICKED(IDC_DGT_CURR_CLIP_BIN, OnDgtCurrClipBin)
	ON_BN_CLICKED(IDC_CHK_DOOR_LOCK, OnChkDoorLock)
	ON_BN_CLICKED(IDC_CHK_USE_SORTER_PICKER, OnChkUseSorterPicker)
	ON_BN_CLICKED(IDC_BTN_PART_CREATE, OnBtnPartCreate)
	ON_BN_CLICKED(IDC_BTN_PART_DELETE, OnBtnPartDelete)
	ON_LBN_SELCHANGE(IDC_LIST_PART_TYPE, OnSelchangeListPartType)
	ON_BN_CLICKED(IDC_BTN_PART_SELECT, OnBtnPartSelect)
	ON_LBN_SELCHANGE(IDC_LIST_PART_SELECT, OnSelchangeListPartSelect)
	ON_BN_CLICKED(IDC_CHK_CLIP_ALARM, OnChkClipAlarm)
	ON_BN_CLICKED(IDC_CHK_CAP_REMOVE, OnChkCapRemove) // 20140220
	ON_BN_CLICKED(IDC_BTN_ALARM_FILE_READ, OnBtnAlarmFileRead)
	ON_BN_CLICKED(IDC_BTN_NODE, OnBtnNode)
	ON_BN_CLICKED(IDC_CHK_VISION_HS_CONTINUE, OnChkVisionHsContinue)
	ON_BN_CLICKED(IDC_DGT_DVC_REPICK_CNT2, OnDgtDvcRepickCnt2)
	ON_BN_CLICKED(IDC_DGT_DVC_REPICK_CNT3, OnDgtDvcRepickCnt3)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_BASIC_APPLY, OnBasic_Info_Apply)  // BASIC DATA를 APPLY 시키기 위한 사용자 정의 메시지 선언
	ON_MESSAGE(WM_BCR_ML_END, OnBasic_Info_Display)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic diagnostics

#ifdef _DEBUG
void CScreen_Basic::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Basic::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic message handlers

void CScreen_Basic::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다
	// **************************************************************************
	mp_basic_font = NULL;
	mp_basic_font = new CFont;
	mp_basic_font->CreateFont(15,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	mp_label_font = NULL;
	mp_label_font = new CFont;
	mp_label_font->CreateFont(10,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	// **************************************************************************
 	
 	OnBasic_Data_Set();				// 전역 변수의 Data를 받아온다.
 	OnBasic_Data_BackUp();
 	
 	OnBasic_Data_Change();
	
	OnBasic_GroupBox_Set();
	OnBasic_EditBox_Set();
 	OnBasic_Digital_Count_Set();
	OnBasic_Label_Set();
	OnBasic_Button_Set();	//20120529
 	OnBasic_Data_Display();
	// **************************************************************************
    // 칼라 리스트 박스 컨트롤 생성한다
    // **************************************************************************
	OnBasic_Create_ImageList();		// 이미지 리스트 생성 함수
	
	// Associate image list to list box
	m_list_device_type.SetImageList(&mp_device_image_list);
	
	OnBasic_Init_List(st_path.mstr_path_dvc);  // 파일 폴더 초기화 함수
	// **************************************************************************
	
	OnBasic_Device_Focus_Set();  // 선택된 디바이스명 포커스 설정 함수

	st_basic.mn_data_change_list = 0;
	
 	OnBasic_Label_Data_Set();				// 전역 변수의 Data를 받아온다.
 	OnBasic_Label_Data_BackUp();
	OnBasic_Label_Font_Set();

	//////////////////////////////// 20120530
	OnBasic_Label_Init_List(st_path.mstr_label_folder);
	OnBasic_Label_Focus_Set();
	//////////////////////////////// 
	OnBasic_Label_Data_Display();


	////////////////////////////////////////////// 20121220 김광수
	OnBasic_Part_List_TextSet();
	mn_response = FALSE;
	//////////////////////////////////////////////

	st_handler.cwnd_basic = this;  // BASIC 화면에 대한 핸들 정보 설정
}

void CScreen_Basic::OnDestroy() 
{
	int n_response = 0;  // 대화 상자에 대한 리턴 값 저장 변수
	int Ret = 0;
	
	CDialog_Select select_dlg;
	
	mp_device_image_list.DeleteImageList();
	
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다
	// **************************************************************************
	delete mp_basic_font;
	mp_basic_font = NULL;
	/////////////////////////// 20120601
	delete mp_label_font;
	mp_label_font = NULL;
	///////////////////////////
	// **************************************************************************
	
	st_handler.cwnd_basic = NULL;  // BASIC 화면 핸들 초기화

	CFormView::OnDestroy();	
}

void CScreen_Basic::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnTimer(nIDEvent);
}


void CScreen_Basic::OnBasic_GroupBox_Set()
{
	CSxLogFont basic_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_device_mode.SetFont(basic_font);
	m_group_device_mode.SetCatptionTextColor(RGB(145,25,0));
	m_group_device_mode.SetFontBold(TRUE);

	m_group_recipe_select.SetFont(basic_font);
	m_group_recipe_select.SetCatptionTextColor(RGB(145,25,0));
	m_group_recipe_select.SetFontBold(TRUE);

	m_group_electrostatic_check_gap.SetFont(basic_font);
	m_group_electrostatic_check_gap.SetCatptionTextColor(RGB(145,25,0));
	m_group_electrostatic_check_gap.SetFontBold(TRUE);

	m_group_electrostatic_check_value.SetFont(basic_font);
	m_group_electrostatic_check_value.SetCatptionTextColor(RGB(145,25,0));
	m_group_electrostatic_check_value.SetFontBold(TRUE);

	m_group_electrostatic_test.SetFont(basic_font);
	m_group_electrostatic_test.SetCatptionTextColor(RGB(145,25,0));
	m_group_electrostatic_test.SetFontBold(TRUE);

	m_group_alarm_delay_time.SetFont(basic_font);
	m_group_alarm_delay_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_alarm_delay_time.SetFontBold(TRUE);

	m_group_network_retry_count.SetFont(basic_font);
	m_group_network_retry_count.SetCatptionTextColor(RGB(145,25,0));
	m_group_network_retry_count.SetFontBold(TRUE);

	m_group_network_wait_time.SetFont(basic_font);
	m_group_network_wait_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_network_wait_time.SetFontBold(TRUE);

	m_group_tray_size_setting.SetFont(basic_font);
	m_group_tray_size_setting.SetCatptionTextColor(RGB(145,25,0));
	m_group_tray_size_setting.SetFontBold(TRUE);


	/////////////////////////////////////////////// 20120522
	m_group_lotset.SetFont(basic_font);
	m_group_lotset.SetCatptionTextColor(RGB(145,25,0));
	m_group_lotset.SetFontBold(TRUE);
	/////////////////////////////////////////////// 20120523
	m_group_basic_speed.SetFont(basic_font);
	m_group_basic_speed.SetCatptionTextColor(RGB(145,25,0));
	m_group_basic_speed.SetFontBold(TRUE);

	m_group_basic_run_speed.SetFont(basic_font);
	m_group_basic_run_speed.SetCatptionTextColor(RGB(145,25,0));
	m_group_basic_run_speed.SetFontBold(TRUE);

	m_group_basic_manual_speed.SetFont(basic_font);
	m_group_basic_manual_speed.SetCatptionTextColor(RGB(145,25,0));
	m_group_basic_manual_speed.SetFontBold(TRUE);

	m_group_basic_jog_speed.SetFont(basic_font);
	m_group_basic_jog_speed.SetCatptionTextColor(RGB(145,25,0));
	m_group_basic_jog_speed.SetFontBold(TRUE);

	m_group_basic_barcode_speed.SetFont(basic_font);
	m_group_basic_barcode_speed.SetCatptionTextColor(RGB(145,25,0));
	m_group_basic_barcode_speed.SetFontBold(TRUE);
 
	/////////////////////////////////////////////// 20120529
	m_group_label_info.SetFont(basic_font);
	m_group_label_info.SetCatptionTextColor(BLUE_C);
	m_group_label_info.SetFontBold(TRUE);
	///////////////////////////////////////////////

	m_group_label_print_error_cnt.SetFont(basic_font);
	m_group_label_print_error_cnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_label_print_error_cnt.SetFontBold(TRUE);

	m_group_use_clip_alarm.SetFont(basic_font);
	m_group_use_clip_alarm.SetCatptionTextColor(RGB(145,25,0));
	m_group_use_clip_alarm.SetFontBold(TRUE);

	m_group_use_cap_remove.SetFont(basic_font); // 20140220
	m_group_use_cap_remove.SetCatptionTextColor(RGB(145,25,0));
	m_group_use_cap_remove.SetFontBold(TRUE);
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_group_use_cap_remove.SetWindowText("Cap removed using mode");
	}



	m_group_label_bin_print.SetFont(basic_font); // 20140220
	m_group_label_bin_print.SetCatptionTextColor(RGB(145,25,0));
	m_group_label_bin_print.SetFontBold(TRUE);
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_group_label_bin_print.SetWindowText("Sheet output bin");
	}
	
	m_group_alarm_file_read.SetFont(basic_font); // 20140220
	m_group_alarm_file_read.SetCatptionTextColor(RGB(145,25,0));
	m_group_alarm_file_read.SetFontBold(TRUE);

	m_group_label_vision_paper.SetFont(basic_font);
	m_group_label_vision_paper.SetCatptionTextColor(RGB(145,25,0));
	m_group_label_vision_paper.SetFontBold(TRUE);
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		m_group_label_vision_paper.SetWindowText("Printer <-> Vision Paper (EA)");
	}

	

	/////////////////////////////////////////////// 20120731
	m_group_dvc_repick_cnt.SetFont(basic_font);
	m_group_dvc_repick_cnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_dvc_repick_cnt.SetFontBold(TRUE);
	///////////////////////////////////////////////

	////2014.0818
	m_group_node.SetFont(basic_font);
	m_group_node.SetCatptionTextColor(RGB(145,25,0));
	m_group_node.SetFontBold(TRUE);
	////

	/////////////////////////////////////////////// 20120625
	m_group_picker_gap.SetFont(basic_font);
	m_group_picker_gap.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker_gap.SetFontBold(TRUE);

	m_group_mhs_direction.SetFont(basic_font);
	m_group_mhs_direction.SetCatptionTextColor(RGB(145,25,0));
	m_group_mhs_direction.SetFontBold(TRUE);

	m_group_bcr.SetFont(basic_font);
	m_group_bcr.SetCatptionTextColor(RGB(145,25,0));
	m_group_bcr.SetFontBold(TRUE);

	m_group_use_bin.SetFont(basic_font);
	m_group_use_bin.SetCatptionTextColor(RGB(145,25,0));
	m_group_use_bin.SetFontBold(TRUE);

	m_group_clip_bin.SetFont(basic_font);
	m_group_clip_bin.SetCatptionTextColor(RGB(145,25,0));
	m_group_clip_bin.SetFontBold(TRUE);

	m_group_use_door.SetFont(basic_font);
	m_group_use_door.SetCatptionTextColor(RGB(145,25,0));
	m_group_use_door.SetFontBold(TRUE);

	m_group_use_sorter_picker.SetFont(basic_font);
	m_group_use_sorter_picker.SetCatptionTextColor(RGB(145,25,0));
	m_group_use_sorter_picker.SetFontBold(TRUE);

	///////////////////////////////////////////////// 20121220 김광수
	m_group_part_number.SetFont(basic_font);
	m_group_part_number.SetCatptionTextColor(RGB(145,25,0));
	m_group_part_number.SetFontBold(TRUE);
	/////////////////////////////////////////////////

	//2015.0129
	m_group_vison_hs_cont.SetFont(basic_font);
	m_group_vison_hs_cont.SetCatptionTextColor(RGB(145,25,0));
	m_group_vison_hs_cont.SetFontBold(TRUE);

}

void CScreen_Basic::OnBasic_EditBox_Set()
{
	m_edit_device_type.SubclassDlgItem(IDC_EDIT_DEVICE_TYPE, this);
	m_edit_device_type.bkColor(RGB(50, 100, 150));
	m_edit_device_type.textColor(RGB(255, 255,255));
	m_edit_device_type.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");

}

void CScreen_Basic::OnBasic_Digital_Count_Set()
{	
	m_dgt_electrostatic_check_gap.SubclassDlgItem(IDC_DGT_ELECTROSTATIC_CHECK_GAP, this);
	m_dgt_electrostatic_check_gap.SetStyle(IDB_BIG3, 4);
	m_dgt_electrostatic_check_gap.SetValue(mn_electrostatic_check_gap[1]);
		
	m_dgt_electrostatic_check_value.SetStyle(CDigit::DS_FLOAT3, 7, CDigit::DC_BBLACK);

	m_dgt_alarm_delay_time.SubclassDlgItem(IDC_DGT_ALARM_DELAY_TIME, this);
	m_dgt_alarm_delay_time.SetStyle(IDB_BIG3, 4);
	m_dgt_alarm_delay_time.SetValue(mn_alarm_delay_time[1]);

	m_dgt_network_retry_cnt.SubclassDlgItem(IDC_DGT_NETWORK_RETRY_COUNT, this);
	m_dgt_network_retry_cnt.SetStyle(IDB_BIG3, 4);
	m_dgt_network_retry_cnt.SetValue(mn_network_retry_cnt[1]);

	m_dgt_network_wait_time.SubclassDlgItem(IDC_DGT_NETWORK_WAIT_TIME, this);
	m_dgt_network_wait_time.SetStyle(IDB_BIG3, 4);
	m_dgt_network_wait_time.SetValue(mn_network_wait_time[1]);

	m_dgt_tray_x.SubclassDlgItem(IDC_DGT_TRAY_X, this);
	m_dgt_tray_x.SetStyle(IDB_BIG3, 2);
	m_dgt_tray_x.SetValue(mn_tray_x[1]);

	m_dgt_tray_y.SubclassDlgItem(IDC_DGT_TRAY_Y, this);
	m_dgt_tray_y.SetStyle(IDB_BIG3, 2);
	m_dgt_tray_y.SetValue(mn_tray_y[1]);

	////////////////////////////////////////////////////////// 20120522
	m_dgt_lotset_module_load.SubclassDlgItem(IDC_DGT_LOTSET_MODULE_LOAD, this);
	m_dgt_lotset_module_load.SetStyle(IDB_BIG3, 3);
	m_dgt_lotset_module_load.SetValue(mn_lotset_module_load[1]);

	m_dgt_lotset_hs_left.SubclassDlgItem(IDC_DGT_LOTSET_HS_LEFT, this);
	m_dgt_lotset_hs_left.SetStyle(IDB_BIG3, 3);
	m_dgt_lotset_hs_left.SetValue(mn_lotset_hs_left[1]);

	m_dgt_lotset_hs_right.SubclassDlgItem(IDC_DGT_LOTSET_HS_RIGHT, this);
	m_dgt_lotset_hs_right.SetStyle(IDB_BIG3, 3);
	m_dgt_lotset_hs_right.SetValue(mn_lotset_hs_right[1]);

	m_dgt_lotset_clip.SubclassDlgItem(IDC_DGT_LOTSET_CLIP, this);
	m_dgt_lotset_clip.SetStyle(IDB_BIG3, 3);
	m_dgt_lotset_clip.SetValue(mn_lotset_clip[1]);

	///////////////////////////////////////////////////////////////// 20120707
	m_dgt_lotset_unloader.SubclassDlgItem(IDC_DGT_LOTSET_UNLOADER, this);
	m_dgt_lotset_unloader.SetStyle(IDB_BIG3, 3);
	m_dgt_lotset_unloader.SetValue(mn_lotset_unloader[1]);

	m_dgt_lotset_reject.SubclassDlgItem(IDC_DGT_LOTSET_REJECT, this);
	m_dgt_lotset_reject.SetStyle(IDB_BIG3, 3);
	m_dgt_lotset_reject.SetValue(mn_lotset_reject[1]);



	////////////////////////////////////////////////////////// 20120523
	m_dgt_basic_run_speed.SubclassDlgItem(IDC_DGT_RUN_SPEED, this);
	m_dgt_basic_run_speed.SetStyle(IDB_BIG3, 3);
	m_dgt_basic_run_speed.SetValue(mn_basic_run_speed[1]);

	m_dgt_basic_manual_speed.SubclassDlgItem(IDC_DGT_MANUAL_SPEED, this);
	m_dgt_basic_manual_speed.SetStyle(IDB_BIG3, 3);
	m_dgt_basic_manual_speed.SetValue(mn_basic_manual_speed[1]);

	m_dgt_basic_jog_speed.SubclassDlgItem(IDC_DGT_JOG_SPEED, this);
	m_dgt_basic_jog_speed.SetStyle(IDB_BIG3, 3);
	m_dgt_basic_jog_speed.SetValue(mn_basic_jog_speed[1]);

	m_dgt_basic_barcode_speed.SubclassDlgItem(IDC_DGT_BARCODE_SPEED, this);
	m_dgt_basic_barcode_speed.SetStyle(IDB_BIG3, 3);
	m_dgt_basic_barcode_speed.SetValue(mn_basic_barcode_speed[1]);

	////////////////////////////////////////////////////////// 20120601
	m_dgt_labelset_sd.SubclassDlgItem(IDC_DGT_LABELSET_SD, this);
	m_dgt_labelset_sd.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_sd.SetValue(mn_labelset_sd[1]);

	m_dgt_labelset_lt.SubclassDlgItem(IDC_DGT_LABELSET_LT, this);
	m_dgt_labelset_lt.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lt.SetValue(mn_labelset_lt[1]);

	m_dgt_labelset_lh1.SubclassDlgItem(IDC_DGT_LABELSET_LH1, this);
	m_dgt_labelset_lh1.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lh1.SetValue(mn_labelset_lh1[1]);

	m_dgt_labelset_lh2.SubclassDlgItem(IDC_DGT_LABELSET_LH2, this);
	m_dgt_labelset_lh2.SetStyle(IDB_BIG3, 2);
	m_dgt_labelset_lh2.SetValue(mn_labelset_lh2[1]);

	m_dgt_clamp_clip_bin.SetStyle(CDigit::DS_INT, 2, CDigit::DC_BBLACK);
	m_dgt_clamp_clip_bin.SetVal(mn_clamp_clip_bin[1]);
	m_dgt_clip_bin.SetStyle(CDigit::DS_INT, 2, CDigit::DC_BBLACK);
	m_dgt_clip_bin.SetVal(mn_clip_bin[1]);

	m_dgt_curr_clamp_clip_bin.SetStyle(CDigit::DS_INT, 2, CDigit::DC_BBLACK);
	m_dgt_curr_clamp_clip_bin.SetVal(mn_curr_clamp_clip_bin[1]);
	m_dgt_curr_clip_bin.SetStyle(CDigit::DS_INT, 2, CDigit::DC_BBLACK);
	m_dgt_curr_clip_bin.SetVal(mn_curr_clip_bin[1]);

////////////////////// 20120906
// 	m_dgt_labelset_x1_pos.SubclassDlgItem(IDC_DGT_LABELSET_X1_POS, this);
// 	m_dgt_labelset_x1_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x1_pos.SetValue(mn_labelset_x1_pos[1]);
	m_dgt_labelset_x1_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x2_pos.SubclassDlgItem(IDC_DGT_LABELSET_X2_POS, this);
// 	m_dgt_labelset_x2_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x2_pos.SetValue(mn_labelset_x2_pos[1]);
	m_dgt_labelset_x2_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x3_pos.SubclassDlgItem(IDC_DGT_LABELSET_X3_POS, this);
// 	m_dgt_labelset_x3_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x3_pos.SetValue(mn_labelset_x3_pos[1]);
	m_dgt_labelset_x3_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x4_pos.SubclassDlgItem(IDC_DGT_LABELSET_X4_POS, this);
// 	m_dgt_labelset_x4_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x4_pos.SetValue(mn_labelset_x4_pos[1]);
	m_dgt_labelset_x4_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x5_pos.SubclassDlgItem(IDC_DGT_LABELSET_X5_POS, this);
// 	m_dgt_labelset_x5_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x5_pos.SetValue(mn_labelset_x5_pos[1]);
	m_dgt_labelset_x5_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x6_pos.SubclassDlgItem(IDC_DGT_LABELSET_X6_POS, this);
// 	m_dgt_labelset_x6_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_x6_pos.SetValue(mn_labelset_x6_pos[1]);
	m_dgt_labelset_x6_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y1_pos.SubclassDlgItem(IDC_DGT_LABELSET_Y1_POS, this);
// 	m_dgt_labelset_y1_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y1_pos.SetValue(mn_labelset_y1_pos[1]);
	m_dgt_labelset_y1_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y2_pos.SubclassDlgItem(IDC_DGT_LABELSET_Y2_POS, this);
// 	m_dgt_labelset_y2_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y2_pos.SetValue(mn_labelset_y2_pos[1]);
	m_dgt_labelset_y2_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y3_pos.SubclassDlgItem(IDC_DGT_LABELSET_Y3_POS, this);
// 	m_dgt_labelset_y3_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y3_pos.SetValue(mn_labelset_y3_pos[1]);
	m_dgt_labelset_y3_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y4_pos.SubclassDlgItem(IDC_DGT_LABELSET_Y4_POS, this);
// 	m_dgt_labelset_y4_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y4_pos.SetValue(mn_labelset_y4_pos[1]);
	m_dgt_labelset_y4_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y5_pos.SubclassDlgItem(IDC_DGT_LABELSET_Y5_POS, this);
// 	m_dgt_labelset_y5_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y5_pos.SetValue(mn_labelset_y5_pos[1]);
	m_dgt_labelset_y5_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y6_pos.SubclassDlgItem(IDC_DGT_LABELSET_Y6_POS, this);
// 	m_dgt_labelset_y6_pos.SetStyle(IDB_BIG3, 4);
// 	m_dgt_labelset_y6_pos.SetValue(mn_labelset_y6_pos[1]);
	m_dgt_labelset_y6_pos.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x1_width.SubclassDlgItem(IDC_DGT_LABELSET_X1_WIDTH, this);
// 	m_dgt_labelset_x1_width.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_x1_width.SetValue(mn_labelset_x1_width[1]);
	m_dgt_labelset_x1_width.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x2_width.SubclassDlgItem(IDC_DGT_LABELSET_X2_WIDTH, this);
// 	m_dgt_labelset_x2_width.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_x2_width.SetValue(mn_labelset_x2_width[1]);
	m_dgt_labelset_x2_width.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x3_width.SubclassDlgItem(IDC_DGT_LABELSET_X3_WIDTH, this);
// 	m_dgt_labelset_x3_width.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_x3_width.SetValue(mn_labelset_x3_width[1]);
	m_dgt_labelset_x3_width.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_x4_width.SubclassDlgItem(IDC_DGT_LABELSET_X4_WIDTH, this);
// 	m_dgt_labelset_x4_width.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_x4_width.SetValue(mn_labelset_x4_width[1]);
	m_dgt_labelset_x4_width.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y1_height.SubclassDlgItem(IDC_DGT_LABELSET_Y1_HEIGHT, this);
// 	m_dgt_labelset_y1_height.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_y1_height.SetValue(mn_labelset_y1_height[1]);
	m_dgt_labelset_y1_height.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y2_height.SubclassDlgItem(IDC_DGT_LABELSET_Y2_HEIGHT, this);
// 	m_dgt_labelset_y2_height.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_y2_height.SetValue(mn_labelset_y2_height[1]);
	m_dgt_labelset_y2_height.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y3_height.SubclassDlgItem(IDC_DGT_LABELSET_Y3_HEIGHT, this);
// 	m_dgt_labelset_y3_height.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_y3_height.SetValue(mn_labelset_y3_height[1]);
	m_dgt_labelset_y3_height.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

// 	m_dgt_labelset_y4_height.SubclassDlgItem(IDC_DGT_LABELSET_Y4_HEIGHT, this);
// 	m_dgt_labelset_y4_height.SetStyle(IDB_BIG3, 3);
// 	m_dgt_labelset_y4_height.SetValue(mn_labelset_y4_height[1]);
	m_dgt_labelset_y4_height.SetStyle(CDigit::DS_FLOAT3, 6, CDigit::DC_BBLACK);

//////////////////////

	m_dgt_Label_Print_Error_Cnt.SubclassDlgItem(IDC_DGT_LABEL_PRINT_ERROR_CNT, this);
	m_dgt_Label_Print_Error_Cnt.SetStyle(IDB_BIG3, 1);
	m_dgt_Label_Print_Error_Cnt.SetValue(mn_Label_Print_Error_Cnt[1]);

///////////// 20120731	
	m_dgt_Dvc_Repick_Cnt.SubclassDlgItem(IDC_DGT_DVC_REPICK_CNT, this);
	m_dgt_Dvc_Repick_Cnt.SetStyle(IDB_BIG3, 1);
	m_dgt_Dvc_Repick_Cnt.SetValue(mn_Dvc_Repick_Cnt[1]);
/////////////

	//2015.0129
	m_dgt_Dvc_Hs_Vis_Cont_Err.SubclassDlgItem(IDC_DGT_DVC_REPICK_CNT2, this);
	m_dgt_Dvc_Hs_Vis_Cont_Err.SetStyle(IDB_BIG3, 3);
	m_dgt_Dvc_Hs_Vis_Cont_Err.SetValue(mn_vis_cont_err[1]);

	m_dgt_Dvc_Hs_Vis_Tot_Err.SubclassDlgItem(IDC_DGT_DVC_REPICK_CNT3, this);
	m_dgt_Dvc_Hs_Vis_Tot_Err.SetStyle(IDB_BIG3, 3);
	m_dgt_Dvc_Hs_Vis_Tot_Err.SetValue(mn_vis_tot_err[1]);
	
	//////////////////////////////////////////////////////////
	m_dgt_picker_gap_module.SetStyle(CDigit::DS_FLOAT2, 6, CDigit::DC_BBLACK);
	m_dgt_picker_gap_hs_left.SetStyle(CDigit::DS_FLOAT2, 6, CDigit::DC_BBLACK);
	m_dgt_picker_gap_hs_right.SetStyle(CDigit::DS_FLOAT2, 6, CDigit::DC_BBLACK);

	m_dgt_label_vision_paper.SubclassDlgItem(IDC_DGT_LABEL_VISION_PAPER, this);
	m_dgt_label_vision_paper.SetStyle(IDB_BIG3, 2);
	m_dgt_label_vision_paper.SetValue(mn_label_vision_paper[1]);

	// 20140224
	m_dgt_label_bin_print.SubclassDlgItem(IDC_DGT_LABEL_BIN_PRINT, this);
	m_dgt_label_bin_print.SetStyle(IDB_BIG3, 2);
	m_dgt_label_bin_print.SetValue(mn_label_bin_print[1]);
	//

	//////////////////////////////////////////////////////////
}

void CScreen_Basic::OnBasic_Data_Set()
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	
	mn_device_mode[1] =				st_basic.n_mode_device;				// With, With Out, No Tray
	
	mn_electrostatic_check_gap[1] =	st_basic.mn_electrostatic_check_gap;
	md_electrostatic_value[1] =		st_basic.md_electrostatic_value;

	md_picker_gap_module[1]	=       st_basic.md_picker_gap_module;
	md_picker_gap_hs_left[1] =      st_basic.md_picker_gap_hs_left;
	md_picker_gap_hs_right[1] =     st_basic.md_picker_gap_hs_right;

	mn_alarm_delay_time[1] =		st_basic.mn_alarm_delay_time;

	mn_network_wait_time[1] =		st_basic.mn_network_wait_time;
	mn_network_retry_cnt[1] =		st_basic.mn_network_retry_cnt = st_basic.n_retry_cnt;
	
	mn_Label_Print_Error_Cnt[1] =	st_basic.mn_Label_Print_Error_Cnt;
	
	mn_Dvc_Repick_Cnt[1] =          st_basic.mn_Dvc_Repick_Cnt;//20120731

	mstr_device_name[1] =			st_basic.mstr_device_name;					// 선택된 디바이스 종류 저장 변수
	
	mstr_ec_server_ip[1] =			st_Network[NETWORK_ECSERVER].mstr_ip;
	mstr_ec_server_ldport[1] =		st_Network[NETWORK_ECSERVER].mstr_ldport;

	mn_tray_x[1] =					st_basic.n_tray_x;
	mn_tray_y[1] =					st_basic.n_tray_y;

	////////////////////////////////////////////////////////////////////// 20120522
	mn_lotset_module_load[1] =		st_traybuffer[LDMODULE_SITE].i_loader_row = st_basic.mn_lotset_module_load;
	mn_lotset_hs_left[1] =		    st_traybuffer[LEFTSINK_SITE].i_loader_row = st_basic.mn_lotset_hs_left;
	mn_lotset_hs_right[1] =		    st_traybuffer[RIGHTSINK_SITE].i_loader_row = st_basic.mn_lotset_hs_right;
	mn_lotset_clip[1] =	        	st_traybuffer[LDCLIP_SITE].i_loader_row = st_basic.mn_lotset_clip;
	////////////////////////////////////////////////////////////////////// 20120707
	mn_lotset_unloader[1] =		    st_traybuffer[UNLOADER_SITE].i_loader_row = st_basic.mn_lotset_unloader;
	mn_lotset_reject[1] =		    st_traybuffer[REJECT_SITE].i_loader_row = st_basic.mn_lotset_reject;


	////////////////////////////////////////////////////////////////////// 20120523
	mn_basic_run_speed[1] =		    st_basic.nRunSpeed;
	mn_basic_manual_speed[1] =		st_basic.nManualSpeed;
	mn_basic_jog_speed[1] =		    st_basic.nJogSpeed;
	mn_basic_barcode_speed[1] =		st_basic.nBarcodeSpeed;

	mn_label_vision_paper[1] =      st_basic.mn_label_vision_paper;

	mn_label_bin_print[1]       =   st_basic.mn_label_bin_print; // 20140224

	//////////////////////////////////////////////////////////////////////
	mn_mode_m_direction[1] =		st_basic.n_mode_m_direction;
	mn_mode_hs_direction[1] =		st_basic.n_mode_hs_direction;
	mn_mode_bcr[1] =				st_basic.n_mode_bcr;
	mn_mode_module_clip_swing[1] =  st_basic.n_mode_module_clip_swing;
	mn_mode_remove_clip[1]	     =  st_basic.n_mode_use_bin;

	mn_mode_clip_alarm[1]		 = st_basic.n_mode_clip_alarm;
	mn_mode_use_cap_remove[1]	 = st_basic.n_mode_cap_remove; // 20140220

	mn_mode_door_lock[1] = st_basic.n_mode_use_door;
	mn_mode_use_sorter_picker[1] = st_basic.n_mode_use_sorter_picker;

	mn_clamp_clip_bin[1]			= st_basic.n_clamp_clip_bin;
	mn_clip_bin[1]					= st_basic.n_clip_bin;

	mn_curr_clamp_clip_bin[1]			= st_basic.n_curr_clamp_clip_bin;
	mn_curr_clip_bin[1]					= st_basic.n_curr_clip_bin;

	//2015.0129
	mn_mode_use_vis_cont_err[1]	 = st_basic.n_mode_use_vis_cont_err;
	mn_vis_cont_err[1] =          st_basic.n_vis_cont_err;//20120731
	mn_vis_tot_err[1] =          st_basic.n_vis_tot_err;//20120731
}

void CScreen_Basic::OnBasic_Data_BackUp()
{
	mn_device_mode[0] =				mn_device_mode[1];				// With, With Out, No Tray

	mn_electrostatic_check_gap[0] =	mn_electrostatic_check_gap[1];
	md_electrostatic_value[0] =		md_electrostatic_value[1];

	md_picker_gap_module[0]	=       md_picker_gap_module[1];
	md_picker_gap_hs_left[0] =      md_picker_gap_hs_left[1];
	md_picker_gap_hs_right[0] =     md_picker_gap_hs_right[1];

	mn_alarm_delay_time[0] =		mn_alarm_delay_time[1];

	mn_network_wait_time[0] =		mn_network_wait_time[1];
	mn_network_retry_cnt[0] =		mn_network_retry_cnt[1];

	mn_Label_Print_Error_Cnt[0] =		mn_Label_Print_Error_Cnt[1];
	
	mn_Dvc_Repick_Cnt[0] =          mn_Dvc_Repick_Cnt[1];//20120731
	
	mstr_device_name[0] =			mstr_device_name[1];					// 선택된 디바이스 종류 저장 변수
	
	mstr_ec_server_ip[0] =			mstr_ec_server_ip[1];
	mstr_ec_server_ldport[0] =		mstr_ec_server_ldport[1];

	mn_tray_x[0] =					mn_tray_x[1];
	mn_tray_y[0] =					mn_tray_y[1];

	////////////////////////////////////////////////////////////////////// 20120522
	mn_lotset_module_load[0] =		mn_lotset_module_load[1];
	mn_lotset_hs_left[0] =		    mn_lotset_hs_left[1];
	mn_lotset_hs_right[0] =		    mn_lotset_hs_right[1];
	mn_lotset_clip[0] =	        	mn_lotset_clip[1];
	////////////////////////////////////////////////////////////////////// 20120707
	mn_lotset_unloader[0] =		mn_lotset_unloader[1];
	mn_lotset_reject[0] =		    mn_lotset_reject[1];


	////////////////////////////////////////////////////////////////////// 20120523
	mn_basic_run_speed[0] =		    mn_basic_run_speed[1];
	mn_basic_manual_speed[0] =		mn_basic_manual_speed[1];
	mn_basic_jog_speed[0] =		    mn_basic_jog_speed[1];
	mn_basic_barcode_speed[0] =		mn_basic_barcode_speed[1];
	////////////////////////////////////////////////////////////////////// 20120530
	mn_label_mode[0] =				mn_label_mode[1];				// With, With Out, No Tray
	mstr_label_name[0] =			mstr_label_name[1];					// 선택된 디바이스 종류 저장 변수


	mn_label_vision_paper[0] =      mn_label_vision_paper[1];
	mn_label_bin_print[0] =      mn_label_bin_print[1]; // 20140224
	
	///////////////////////////////////////////////////////////
	mn_mode_m_direction[0] =		mn_mode_m_direction[1];
	mn_mode_hs_direction[0] =		mn_mode_hs_direction[1];

	mn_clamp_clip_bin[0]			= mn_clamp_clip_bin[1];
	mn_clip_bin[0]					= mn_clip_bin[1];

	mn_curr_clamp_clip_bin[0]		= mn_curr_clamp_clip_bin[1];
	mn_curr_clip_bin[0]				= mn_curr_clip_bin[1];

	mn_mode_use_vis_cont_err[0]		= mn_mode_use_vis_cont_err[1];
	mn_vis_cont_err[0]				= mn_vis_cont_err[1];
	mn_vis_tot_err[0]				= mn_vis_tot_err[1];

}

void CScreen_Basic::OnBasic_Data_Recovery()
{
	mn_device_mode[1] =				mn_device_mode[0];				// With, With Out, No Tray

	mn_electrostatic_check_gap[1] =	mn_electrostatic_check_gap[0];
	md_electrostatic_value[1] =		md_electrostatic_value[0];

	md_picker_gap_module[1]	=       md_picker_gap_module[0];
	md_picker_gap_hs_left[1] =      md_picker_gap_hs_left[0];
	md_picker_gap_hs_right[1] =     md_picker_gap_hs_right[0];

	mn_alarm_delay_time[1] =		mn_alarm_delay_time[0];

	mn_network_wait_time[1] =		mn_network_wait_time[0];
	mn_network_retry_cnt[1] =		mn_network_retry_cnt[0];

	mn_Label_Print_Error_Cnt[1] =		mn_Label_Print_Error_Cnt[0];
	
	mn_Dvc_Repick_Cnt[1] = mn_Dvc_Repick_Cnt[0];//20120731

	mstr_device_name[1] =			mstr_device_name[0];					// 선택된 디바이스 종류 저장 변수
	
	mstr_ec_server_ip[1] =			mstr_ec_server_ip[0];
	mstr_ec_server_ldport[1] =		mstr_ec_server_ldport[0];

	mn_tray_x[1] =					mn_tray_x[0];
	mn_tray_y[1] =					mn_tray_y[0];

	////////////////////////////////////////////////////////////////////// 20120522
	mn_lotset_module_load[1] =		mn_lotset_module_load[0];
	mn_lotset_hs_left[1] =		    mn_lotset_hs_left[0];
	mn_lotset_hs_right[1] =		    mn_lotset_hs_right[0];
	mn_lotset_clip[1] =	        	mn_lotset_clip[0];
	////////////////////////////////////////////////////////////////////// 20120707
	mn_lotset_unloader[1] =		mn_lotset_unloader[0];
	mn_lotset_reject[1] =		    mn_lotset_reject[0];

	////////////////////////////////////////////////////////////////////// 20120523
	mn_basic_run_speed[1] =		    mn_basic_run_speed[0];
	mn_basic_manual_speed[1] =		mn_basic_manual_speed[0];
	mn_basic_jog_speed[1] =		    mn_basic_jog_speed[0];
	mn_basic_barcode_speed[1] =	    mn_basic_barcode_speed[0];

	mn_label_vision_paper[1] =      mn_label_vision_paper[0];
	mn_label_bin_print[1] =      mn_label_bin_print[0]; // 20140224
	
	//////////////////////////////////////////////////////////////////////
	mn_mode_m_direction[1] =		mn_mode_m_direction[0];
	mn_mode_hs_direction[1] =		mn_mode_hs_direction[0];

	mn_clamp_clip_bin[1]			= mn_clamp_clip_bin[0];
	mn_clip_bin[1]					= mn_clip_bin[0];

	mn_curr_clamp_clip_bin[1]		= mn_curr_clamp_clip_bin[0];
	mn_curr_clip_bin[1]				= mn_curr_clip_bin[0];

	mn_mode_use_vis_cont_err[1]		= mn_mode_use_vis_cont_err[0];
	mn_vis_cont_err[1]				= mn_vis_cont_err[0];
	mn_vis_tot_err[1]				= mn_vis_tot_err[0];

}

void CScreen_Basic::OnBasic_Data_Change()
{
	OnBasic_DeviceMode_Change();
}

/////////////////////////////////////////////////// 20120529
void CScreen_Basic::OnBasic_Button_Set()
{
	short	shBtnColor = 30;

	m_btn_label_create.SetFont(mp_basic_font);
 	m_btn_label_create.SetIcon(IDI_MOTOR_APPLY);
	m_btn_label_create.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_label_create.SetButtonColor(1);

	m_btn_label_delete.SetFont(mp_basic_font);
 	m_btn_label_delete.SetIcon(IDI_MOTOR_APPLY);
	m_btn_label_delete.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_label_delete.SetButtonColor(1);
	
	m_chk_m_direction.SetFont(mp_basic_font);
 	m_chk_m_direction.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_m_direction.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_m_direction.SetButtonColor(1);

	m_chk_hs_direction.SetFont(mp_basic_font);
 	m_chk_hs_direction.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_hs_direction.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_hs_direction.SetButtonColor(1);	


	m_chk_bcr.SetFont(mp_basic_font);
 	m_chk_bcr.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_bcr.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_bcr.SetButtonColor(1);

	m_chk_remove_clip.SetFont(mp_basic_font);
 	m_chk_remove_clip.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_remove_clip.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_remove_clip.SetButtonColor(1);

	m_chk_clip_alarm.SetFont(mp_basic_font);
 	m_chk_clip_alarm.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_clip_alarm.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_clip_alarm.SetButtonColor(1);

	m_chk_cap_remove.SetFont(mp_basic_font);
 	m_chk_cap_remove.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_cap_remove.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_cap_remove.SetButtonColor(1);

	m_chk_door_lock.SetFont(mp_basic_font);
 	m_chk_door_lock.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_door_lock.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_door_lock.SetButtonColor(1);

	m_chk_use_sorter_picker.SetFont(mp_basic_font);
 	m_chk_use_sorter_picker.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_use_sorter_picker.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_use_sorter_picker.SetButtonColor(1);


	//2015.0129
	m_chk_use_vision_hs_continue.SetFont(mp_basic_font);
 	m_chk_use_vision_hs_continue.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_use_vision_hs_continue.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_use_vision_hs_continue.SetButtonColor(1);


}
///////////////////////////////////////////////////

void CScreen_Basic::OnBasic_Label_Set()
{
	m_msg_electrostatic_1.SetFont(mp_basic_font);
	m_msg_electrostatic_1.SetWindowText(_T(""));
	m_msg_electrostatic_1.SetCenterText();
	m_msg_electrostatic_1.SetColor(YELLOW_C);
	m_msg_electrostatic_1.SetGradientColor(YELLOW_C);
	m_msg_electrostatic_1.SetTextColor(RGB(0,0,0));

	m_msg_electrostatic_2.SetFont(mp_basic_font);
	m_msg_electrostatic_2.SetWindowText(_T(""));
	m_msg_electrostatic_2.SetCenterText();
	m_msg_electrostatic_2.SetColor(YELLOW_C);
	m_msg_electrostatic_2.SetGradientColor(YELLOW_C);
	m_msg_electrostatic_2.SetTextColor(RGB(0,0,0));

	m_msg_electrostatic_3.SetFont(mp_basic_font);
	m_msg_electrostatic_3.SetWindowText(_T(""));
	m_msg_electrostatic_3.SetCenterText();
	m_msg_electrostatic_3.SetColor(YELLOW_C);
	m_msg_electrostatic_3.SetGradientColor(YELLOW_C);
	m_msg_electrostatic_3.SetTextColor(RGB(0,0,0));

	m_msg_electrostatic_4.SetFont(mp_basic_font);
	m_msg_electrostatic_4.SetWindowText(_T(""));
	m_msg_electrostatic_4.SetCenterText();
	m_msg_electrostatic_4.SetColor(YELLOW_C);
	m_msg_electrostatic_4.SetGradientColor(YELLOW_C);
	m_msg_electrostatic_4.SetTextColor(RGB(0,0,0));

	m_msg_electrostatic_5.SetFont(mp_basic_font);
	m_msg_electrostatic_5.SetWindowText(_T(""));
	m_msg_electrostatic_5.SetCenterText();
	m_msg_electrostatic_5.SetColor(YELLOW_C);
	m_msg_electrostatic_5.SetGradientColor(YELLOW_C);
	m_msg_electrostatic_5.SetTextColor(RGB(0,0,0));

	m_label_electrostatic_1.SetFont(mp_basic_font);
	m_label_electrostatic_1.SetWindowText(_T("Robot Z Axis"));
	m_label_electrostatic_1.SetCenterText();
	m_label_electrostatic_1.SetColor(RGB(0,0,255));
	m_label_electrostatic_1.SetGradientColor(RGB(0,0,0));
	m_label_electrostatic_1.SetTextColor(RGB(255,255,255));
	
	m_label_electrostatic_2.SetFont(mp_basic_font);
	m_label_electrostatic_2.SetWindowText(_T("LOB Buffer 1"));
	m_label_electrostatic_2.SetCenterText();
	m_label_electrostatic_2.SetColor(RGB(0,0,255));
	m_label_electrostatic_2.SetGradientColor(RGB(0,0,0));
	m_label_electrostatic_2.SetTextColor(RGB(255,255,255));
	
	m_label_electrostatic_3.SetFont(mp_basic_font);
	m_label_electrostatic_3.SetWindowText(_T("LOB Buffer 2"));
	m_label_electrostatic_3.SetCenterText();
	m_label_electrostatic_3.SetColor(RGB(0,0,255));
	m_label_electrostatic_3.SetGradientColor(RGB(0,0,0));
	m_label_electrostatic_3.SetTextColor(RGB(255,255,255));
	
	m_label_electrostatic_4.SetFont(mp_basic_font);
	m_label_electrostatic_4.SetWindowText(_T("Reject Buffer"));
	m_label_electrostatic_4.SetCenterText();
	m_label_electrostatic_4.SetColor(RGB(0,0,255));
	m_label_electrostatic_4.SetGradientColor(RGB(0,0,0));
	m_label_electrostatic_4.SetTextColor(RGB(255,255,255));
	
	m_label_electrostatic_5.SetFont(mp_basic_font);
	m_label_electrostatic_5.SetWindowText(_T("Retest Buffer"));
	m_label_electrostatic_5.SetCenterText();
	m_label_electrostatic_5.SetColor(RGB(0,0,255));
	m_label_electrostatic_5.SetGradientColor(RGB(0,0,0));
	m_label_electrostatic_5.SetTextColor(RGB(255,255,255));

	m_msg_tray_x.SetFont(mp_basic_font);
	m_msg_tray_x.SetWindowText(_T("X size"));
	m_msg_tray_x.SetCenterText();
	m_msg_tray_x.SetColor(RGB(0,0,255));
	m_msg_tray_x.SetGradientColor(RGB(0,0,0));
	m_msg_tray_x.SetTextColor(RGB(255,255,255));

	m_msg_tray_y.SetFont(mp_basic_font);
	m_msg_tray_y.SetWindowText(_T("Y size"));
	m_msg_tray_y.SetCenterText();
	m_msg_tray_y.SetColor(RGB(0,0,255));
	m_msg_tray_y.SetGradientColor(RGB(0,0,0));
	m_msg_tray_y.SetTextColor(RGB(255,255,255));

	/////////////////////////////////////////////////// 20120522
	m_msg_lotset_Module_load.SetFont(mp_basic_font);
	m_msg_lotset_Module_load.SetWindowText(_T("MODULE LOAD"));
	m_msg_lotset_Module_load.SetCenterText();
	m_msg_lotset_Module_load.SetColor(RGB(0,0,255));
	m_msg_lotset_Module_load.SetGradientColor(RGB(0,0,0));
	m_msg_lotset_Module_load.SetTextColor(RGB(255,255,255));

	m_msg_lotset_hs_left.SetFont(mp_basic_font);
	m_msg_lotset_hs_left.SetWindowText(_T("HEATSINK LEFT"));
	m_msg_lotset_hs_left.SetCenterText();
	m_msg_lotset_hs_left.SetColor(RGB(0,0,255));
	m_msg_lotset_hs_left.SetGradientColor(RGB(0,0,0));
	m_msg_lotset_hs_left.SetTextColor(RGB(255,255,255));

	m_msg_lotset_hs_right.SetFont(mp_basic_font);
	m_msg_lotset_hs_right.SetWindowText(_T("HEATSINK RIGHT"));
	m_msg_lotset_hs_right.SetCenterText();
	m_msg_lotset_hs_right.SetColor(RGB(0,0,255));
	m_msg_lotset_hs_right.SetGradientColor(RGB(0,0,0));
	m_msg_lotset_hs_right.SetTextColor(RGB(255,255,255));

	m_msg_lotset_clip.SetFont(mp_basic_font);
	m_msg_lotset_clip.SetWindowText(_T("CLIP"));
	m_msg_lotset_clip.SetCenterText();
	m_msg_lotset_clip.SetColor(RGB(0,0,255));
	m_msg_lotset_clip.SetGradientColor(RGB(0,0,0));
	m_msg_lotset_clip.SetTextColor(RGB(255,255,255));

	m_msg_lotset_unloader.SetFont(mp_basic_font);
	m_msg_lotset_unloader.SetWindowText(_T("UNLOADER"));
	m_msg_lotset_unloader.SetCenterText();
	m_msg_lotset_unloader.SetColor(RGB(0,0,255));
	m_msg_lotset_unloader.SetGradientColor(RGB(0,0,0));
	m_msg_lotset_unloader.SetTextColor(RGB(255,255,255));

	m_msg_lotset_reject.SetFont(mp_basic_font);
	m_msg_lotset_reject.SetWindowText(_T("REJECT"));
	m_msg_lotset_reject.SetCenterText();
	m_msg_lotset_reject.SetColor(RGB(0,0,255));
	m_msg_lotset_reject.SetGradientColor(RGB(0,0,0));
	m_msg_lotset_reject.SetTextColor(RGB(255,255,255));



	m_msg_picker_gap_module.SetFont(mp_basic_font);
	m_msg_picker_gap_module.SetWindowText(_T("MODULE LOAD"));
	m_msg_picker_gap_module.SetCenterText();
	m_msg_picker_gap_module.SetColor(RGB(0,0,255));
	m_msg_picker_gap_module.SetGradientColor(RGB(0,0,0));
	m_msg_picker_gap_module.SetTextColor(RGB(255,255,255));

	m_msg_picker_gap_hs_left.SetFont(mp_basic_font);
	m_msg_picker_gap_hs_left.SetWindowText(_T("HEATSINK LEFT"));
	m_msg_picker_gap_hs_left.SetCenterText();
	m_msg_picker_gap_hs_left.SetColor(RGB(0,0,255));
	m_msg_picker_gap_hs_left.SetGradientColor(RGB(0,0,0));
	m_msg_picker_gap_hs_left.SetTextColor(RGB(255,255,255));

	m_msg_picker_gap_hs_right.SetFont(mp_basic_font);
	m_msg_picker_gap_hs_right.SetWindowText(_T("HEATSINK RIGHT"));
	m_msg_picker_gap_hs_right.SetCenterText();
	m_msg_picker_gap_hs_right.SetColor(RGB(0,0,255));
	m_msg_picker_gap_hs_right.SetGradientColor(RGB(0,0,0));
	m_msg_picker_gap_hs_right.SetTextColor(RGB(255,255,255));

	m_msg_clamp_clip_bin.SetFont(mp_basic_font);
	m_msg_clamp_clip_bin.SetWindowText(_T("Clamp Bin"));
	m_msg_clamp_clip_bin.SetCenterText();
	m_msg_clamp_clip_bin.SetColor(RGB(0,0,255));
	m_msg_clamp_clip_bin.SetGradientColor(RGB(0,0,0));
	m_msg_clamp_clip_bin.SetTextColor(RGB(255,255,255));

	m_msg_clip_bin.SetFont(mp_basic_font);
	m_msg_clip_bin.SetWindowText(_T("Clip Bin"));
	m_msg_clip_bin.SetCenterText();
	m_msg_clip_bin.SetColor(RGB(0,0,255));
	m_msg_clip_bin.SetGradientColor(RGB(0,0,0));
	m_msg_clip_bin.SetTextColor(RGB(255,255,255));

	m_msg_set_clip.SetFont(mp_basic_font);
	m_msg_set_clip.SetWindowText(_T("설정값"));
	m_msg_set_clip.SetCenterText();
	m_msg_set_clip.SetColor(RGB(0,0,255));
	m_msg_set_clip.SetGradientColor(RGB(0,0,0));
	m_msg_set_clip.SetTextColor(RGB(255,255,255));

	m_msg_stack_clip.SetFont(mp_basic_font);
	m_msg_stack_clip.SetWindowText(_T("현재값"));
	m_msg_stack_clip.SetCenterText();
	m_msg_stack_clip.SetColor(RGB(0,0,255));
	m_msg_stack_clip.SetGradientColor(RGB(0,0,0));
	m_msg_stack_clip.SetTextColor(RGB(255,255,255));

	m_msg_vision_hs_cont_err.SetFont(mp_basic_font);
	m_msg_vision_hs_cont_err.SetWindowText(_T("Cont Err."));
	m_msg_vision_hs_cont_err.SetCenterText();
	m_msg_vision_hs_cont_err.SetColor(RGB(0,0,255));
	m_msg_vision_hs_cont_err.SetGradientColor(RGB(0,0,0));
	m_msg_vision_hs_cont_err.SetTextColor(RGB(255,255,255));

	m_msg_vision_hs_total_err.SetFont(mp_basic_font);
	m_msg_vision_hs_total_err.SetWindowText(_T("Total Err."));
	m_msg_vision_hs_total_err.SetCenterText();
	m_msg_vision_hs_total_err.SetColor(RGB(0,0,255));
	m_msg_vision_hs_total_err.SetGradientColor(RGB(0,0,0));
	m_msg_vision_hs_total_err.SetTextColor(RGB(255,255,255));
}
void CScreen_Basic::OnRadioTrayModeUse() 
{
	if (mn_device_mode[1] == 2)	return;
	
	mn_device_mode[1] = 2;
	OnBasic_DeviceMode_Change();
	
}

void CScreen_Basic::OnRadioDeviceModeUse() 
{
	if (mn_device_mode[1] == CTL_YES)	return;
	
	mn_device_mode[1] = CTL_YES;
	OnBasic_DeviceMode_Change();
}

void CScreen_Basic::OnRadioDeviceModeNotuse() 
{
	if (mn_device_mode[1] == CTL_NO)	return;
	
	mn_device_mode[1] = CTL_NO;
	OnBasic_DeviceMode_Change();
}

void CScreen_Basic::OnBasic_DeviceMode_Change()
{
	if (mn_device_mode[1] < 0 || mn_device_mode[1] > 2)	mn_device_mode[1] = 0;
	
	switch (mn_device_mode[1])
	{
	case 0:
		m_radio_notray.SetCheck(FALSE);
		m_radio_device_mode_use.SetCheck(FALSE);
		m_radio_device_mode_notuse.SetCheck(TRUE);
		break;
		
	case 1:
		m_radio_notray.SetCheck(FALSE);
		m_radio_device_mode_use.SetCheck(TRUE);
		m_radio_device_mode_notuse.SetCheck(FALSE);
		break;

	case 2:
		m_radio_notray.SetCheck(TRUE);
		m_radio_device_mode_use.SetCheck(FALSE);
		m_radio_device_mode_notuse.SetCheck(FALSE);
		break;
	}
}

void CScreen_Basic::OnIP_Address(CString str_ip, int n_section)
{
	int nDot1, nDot2, nDot3, nStart;
	
	nStart = 0;
	nDot1 = str_ip.Find('.');
	nDot2 = str_ip.Find('.', nDot1 + 1);
	nDot3 = str_ip.Find('.', nDot2 + 1);
	
	if (n_section < NETWORK_ECSERVER || n_section > NETWORK_ECSERVER)		// 0: TESTER, 1: FTP
	{
		return;
	}
	
	nIPFiled0[n_section] = atoi(str_ip.Mid(nStart, nDot1 - nStart));
	nStart = nDot1 + 1;
	nIPFiled1[n_section] = atoi(str_ip.Mid(nStart, nDot2 - nStart));
	nStart = nDot2 + 1;
	nIPFiled2[n_section] = atoi(str_ip.Mid(nStart, nDot3 - nStart));
	nStart = nDot3 + 1;
	nIPFiled3[n_section] = atoi(str_ip.Mid(nStart));
}

void CScreen_Basic::OnDgtElectrostaticCheckGap() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_electrostatic_check_gap.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("정전기 확인 시간 (분)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Check the electrostatic time (minutes)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "1440";
	st_msg.mstr_pad_low_limit = "1";

	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_electrostatic_check_gap.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_electrostatic_check_gap[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_electrostatic_check_gap.SetValue(mn_electrostatic_check_gap[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnDgtElectrostaticCheckValue() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double d_dgt_data;
	
	CDialog_KeyPad pad_dlg;
	
	d_dgt_data = (double)m_dgt_electrostatic_check_value.GetFloatVal();
	mstr_temp.Format("%4.3f", d_dgt_data);
	
	st_msg.mstr_keypad_msg = _T("그라운드 설정값");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Setting ground");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mn_dot_use = CTL_YES;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "0";
	
	CRect r;
	
	m_dgt_electrostatic_check_value.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		d_dgt_data = atof(mstr_temp);
		md_electrostatic_value[1] = atof(mstr_temp);
		m_dgt_electrostatic_check_value.SetFloatVal((float)d_dgt_data);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnBtnDvcCreate() 
{
	int n_response;									// 대화 상자 리턴 플래그
	CString str_selected_dvc;						// 선택된 디바이스 이름 저장 변수
	
	CDialog_Keyboard  keyboard_dlg;
	
	(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	
	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		mstr_new_device = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정
		
		if (OnBasic_New_Device_Check() == FALSE)	return;
		
		OnBasic_Create_Device();
		
		// ******************************************************************
		// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다
		// ******************************************************************
		m_list_device_type.ResetContent();		// 리스트 박스의 모든 목록 삭제
		mp_device_image_list.DeleteImageList();	// 이미지 리스트에 설정된 정보 삭제
		
		OnBasic_Create_ImageList();				// 이미지 리스트 생성 함수
		
		// Associate image list to list box
		m_list_device_type.SetImageList(&mp_device_image_list);
		
		OnBasic_Init_List(st_path.mstr_path_dvc);// 파일 폴더 초기화 함수
		OnSelchangeListDeviceType();
		OnBasic_Device_Focus_Set();				// 선택된 디바이스명 포커스 설정 함수
		
		if (mn_device_name > 0)
		{
			m_list_device_type.GetText(mn_device_name, str_selected_dvc);
		}
		
		OnBasic_Device_Display(str_selected_dvc);	// 선택된 디바이스 이름 출력 함수
		// ******************************************************************
	}
	else if (n_response == IDCANCEL)
	{
	}
}

int CScreen_Basic::OnBasic_New_Device_Check()
{
	CString str_chk_file;  // 생성할 [폴더]+[파일명] 저장 변수
	CString str_chk_ext;   // 확장자 저장 변수
	int n_pos;
	
	// **************************************************************************
    // 입력된 디바이스명 설정한다
    // **************************************************************************
	mstr_new_device.MakeUpper();
	mstr_new_device.TrimLeft(' ');               
	mstr_new_device.TrimRight(' ');
	
	if (mstr_new_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg = _T("[DEVICE] A name input error occurrence.");
			sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name input error occurrence.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	// **************************************************************************
	
	str_chk_file = st_path.mstr_path_dvc + mstr_new_device;  // 생성할 [폴더]+[파일명] 설정

	n_pos = str_chk_file.Find(".");  // 확장자 검사
	if (n_pos == -1) 
		str_chk_file += _T(".TXT");
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE] A name extension input error occurrence.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name extension input error occurrence.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return FALSE;
		}
	}
	
	mstr_load_file = str_chk_file;  // 생성할 [폴더]+[파일명]+[확장자] 설정
	
	return TRUE;
}

void CScreen_Basic::OnBasic_Create_Device()
{
	int n_existence;								// 파일 존재 유무 플래그
	char chr_file_name[256];
	CString str_device_temp;
	CString str_original_path;
	CString str_original_file;
	CString str_copy_file;
	CString str_chk_ext;
	CString str_copy_info;
	
	sprintf(chr_file_name, "%s", mstr_load_file);	// [폴더]+[파일명] 설정
	
	n_existence = access(chr_file_name, 0) ;
	
	if (n_existence != -1)							// 파일 존재
		return ;
	else  
	{
		str_device_temp = st_basic.mstr_device_name;
		st_basic.mstr_device_name = mstr_new_device;
		
		:: WritePrivateProfileString("Device", "DeviceFile", LPCTSTR(mstr_new_device), mstr_load_file); 
		
		mcls_basic.On_Teach_Data_Save();
		
		st_basic.mstr_device_name = str_device_temp;
	}
}

void CScreen_Basic::OnSelchangeListDeviceType() 
{
	CString str_selected_dvc;						// 선택된 디바이스 이름 저장 변수
	
	int n_index = m_list_device_type.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_device_type.GetText(n_index, str_selected_dvc);
		OnBasic_Device_Display(str_selected_dvc);	// 선택된 디바이스 이름 출력 함수
	}
	else
		OnBasic_Device_Display("");	// 빈 이름 출력 함수
}

void CScreen_Basic::OnBasic_Device_Focus_Set()
{
	if (mn_device_name == -1)
		return;
	
	m_list_device_type.SetCurSel(mn_device_name);	
}

///////////////////////////////////////////////// 20120530
void CScreen_Basic::OnBasic_Label_Focus_Set()
{
	if (mn_label_name == -1)
		return;
	
	m_list_label_type.SetCurSel(mn_label_name);
	
}
/////////////////////////////////////////////////



void CScreen_Basic::OnBasic_Device_Display(CString str_dvc)
{
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText(_T(str_dvc));
}

void CScreen_Basic::OnBasic_Create_ImageList()
{
	BOOL bRetValue = FALSE;
	
	// Create image list
	bRetValue = mp_device_image_list.Create(32, 32, ILC_COLOR32 | ILC_MASK, 5, 1);
	ASSERT(bRetValue == TRUE);
}

int CScreen_Basic::OnBasic_Init_List(LPCTSTR pszPath)
{
	int n_chk;						// 함수 리턴 플래그
    CString str_path = pszPath;		// 폴더 설정
	
	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// 리스트 박스에 추가한 목록 갯수 저장 변수

	mn_device_name = -1;
	
	if (str_path.Right (1) != "\\")
        str_path += "\\";
	
	str_path += "*.*";
    
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			n_chk = AddItem(n_count, &fd);
			if (n_chk == TRUE)
				n_count++;
        }
		
        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				n_chk = AddItem(n_count, &fd);
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

////////////////////////////////////////////////////////////////////////// 20120530
int CScreen_Basic::OnBasic_Label_Init_List(LPCTSTR pszPath)
{
	int n_chk;						// 함수 리턴 플래그
    CString str_path = pszPath;		// 폴더 설정
	
	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// 리스트 박스에 추가한 목록 갯수 저장 변수

	mn_label_name = -1;
	
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
				st_basic.mn_data_change_list++;	//20120608
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
//////////////////////////////////////////////////////////////////////////

BOOL CScreen_Basic::AddItem(int nIndex, WIN32_FIND_DATA *pfd)
{
	CString str_temp_dvc;
	CString str_device;
	CString str_expansion;
	int n_pos;
	
	HICON	hIcon = NULL;
	
	str_temp_dvc = (pfd->cFileName);	// 파일 이름 설정
	
	str_temp_dvc.MakeUpper();
	
	// **************************************************************************
	// 파일 이름 중 확장자 확인
	// **************************************************************************
	n_pos = str_temp_dvc.Find('.');		// 확장자 위치 설정
	
	if (n_pos == -1)					// 확장자 미존재
		str_expansion = "";
	else								// 확장자 존재
		str_expansion = str_temp_dvc.Mid(n_pos + 1);
	// **************************************************************************
	
	if (str_expansion != "TXT")
	{
		return CTL_READY;	
	}
	
	str_device = str_temp_dvc;			// 파일명 전체 화면 출력
	
	if (str_device == mstr_device_name[1])
		mn_device_name = nIndex-1;
	// **************************************************************************
	
	hIcon = AfxGetApp()->LoadIcon(IDI_MODULE);
	mp_device_image_list.Add(hIcon);
	
    if (m_list_device_type.AddString(_T(str_device), nIndex - 1) == -1)
		return FALSE;
	
    return TRUE;
}

////////////////////////////////////////////////// 20120530
BOOL CScreen_Basic::Label_AddItem(int nIndex, WIN32_FIND_DATA *pfd)
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
	
	if (str_label == mstr_label_name[1])
		mn_label_name = nIndex-1;
	// **************************************************************************
	
// 	hIcon = AfxGetApp()->LoadIcon(IDI_MODULE);
// 	mp_device_image_list.Add(hIcon);
	
    if (m_list_label_type.AddString(_T(str_label), nIndex - 1) == -1)
		return FALSE;
	
    return TRUE;
}






//////////////////////////////////////////////////
void CScreen_Basic::OnBtnDvcDelete() 
{
	int n_response;				// 대화 상자 리턴 플래그
	int n_existence;			// 파일 존재 유무 플래그
	CString str_file_name;
	CString str_selected_dvc;
	int n_pos;
	
	char chr_file_name[256];
	char chr_speed_file_name[256];
	char chr_teach_file_name[256];
	char chr_time_file_name[256];
	
	CDialog_Select  select_dlg;
	CDialog_Message msg_dlg;
	
	if (OnBasic_Input_Device_Check() == FALSE)  // 입력된 디바이스 이름 정상 유무 검사 함수
		return ;
	
	st_msg.mstr_confirm_msg = _T(mstr_temp_device + " Device Type Delete?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		sprintf(chr_file_name, "%s", mstr_load_file);  // [폴더]+[파일명] 설정

		n_pos = mstr_load_file.Find(".");  // 확장자 검사
		
		if (n_pos == -1) 
		{
			sprintf(chr_speed_file_name, "%s_Speed.DAT", mstr_load_file);  // [폴더]+[파일명] 설정
			sprintf(chr_teach_file_name, "%s_Teach.DAT", mstr_load_file);  // [폴더]+[파일명] 설정
			sprintf(chr_time_file_name, "%s_Time.DAT", mstr_load_file);  // [폴더]+[파일명] 설정
		}
		else 
		{
			sprintf(chr_speed_file_name, "%s_Speed.DAT", mstr_load_file.Mid(0, n_pos));  // [폴더]+[파일명] 설정
			sprintf(chr_teach_file_name, "%s_Teach.DAT", mstr_load_file.Mid(0, n_pos));  // [폴더]+[파일명] 설정
			sprintf(chr_time_file_name, "%s_Time.DAT", mstr_load_file.Mid(0, n_pos));  // [폴더]+[파일명] 설정
		}
		
		n_existence = access(chr_file_name, 0) ;
		
		if (n_existence != -1)					// 파일 존재
		{
			DeleteFile(chr_file_name);			// 해당 파일 삭제
			DeleteFile(chr_speed_file_name);	// 해당 파일 삭제
			DeleteFile(chr_teach_file_name);	// 해당 파일 삭제
			DeleteFile(chr_time_file_name);		// 해당 파일 삭제
			
			// ******************************************************************
			// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다
			// ******************************************************************
			m_list_device_type.ResetContent();		// 리스트 박스의 모든 목록 삭제
			mp_device_image_list.DeleteImageList();	// 이미지 리스트에 설정된 정보 삭제
			
			OnBasic_Create_ImageList();				// 이미지 리스트 생성 함수
			
			// Associate image list to list box
			m_list_device_type.SetImageList(&mp_device_image_list);
			
			OnBasic_Init_List(st_path.mstr_path_dvc);// 파일 폴더 초기화 함수
			OnSelchangeListDeviceType();
			OnBasic_Device_Focus_Set();				// 선택된 디바이스명 포커스 설정 함수
			
			if (mn_device_name > -1)
			{
				m_list_device_type.GetText(mn_device_name, str_selected_dvc);
				
				OnBasic_Device_Display(str_selected_dvc);	// 선택된 디바이스 이름 출력 함수
			}
			// ******************************************************************
		}
		else  
			return ;
	}
	else if (n_response == IDCANCEL)
	{
	}
}

int CScreen_Basic::OnBasic_Input_Device_Check()
{
	CString str_chk_file;  // 생성할 [폴더]+[파일명] 저장 변수
	CString str_chk_ext;   // 확장자 저장 변수
	int n_pos;
	
	// **************************************************************************
    // 입력된 디바이스명 설정한다                                                
    // **************************************************************************
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_temp_device) ;
	mstr_temp_device.MakeUpper();
	mstr_temp_device.TrimLeft(' ');               
	mstr_temp_device.TrimRight(' ');
	
	if (mstr_temp_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg = _T("[DEVICE] A name input error occurrence.");
			sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name input error occurrence.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	// **************************************************************************
	
	str_chk_file = st_path.mstr_path_dvc + mstr_temp_device;  // 생성할 [폴더]+[파일명] 설정
	n_pos = str_chk_file.Find(".");  // 확장자 검사
	if (n_pos == -1) 
		str_chk_file += _T(".TXT");
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE] A name extension input error occurrence.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name extension input error occurrence.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return FALSE;
		}
	}
	
	mstr_load_file = str_chk_file;  // 생성할 [폴더]+[파일명]+[확장자] 설정
	
	return TRUE;
}

void CScreen_Basic::OnBtnElectrostaticTest() 
{
	m_msg_electrostatic_1.SetWindowText(_T(""));
	m_msg_electrostatic_2.SetWindowText(_T(""));
	m_msg_electrostatic_3.SetWindowText(_T(""));
	m_msg_electrostatic_4.SetWindowText(_T(""));
	m_msg_electrostatic_5.SetWindowText(_T(""));
	
	::PostMessage(st_handler.hWnd, WM_MAINFRAME_WORK, 2, 0);
	
	m_btn_electostatic_test.EnableWindow(FALSE);
}

void CScreen_Basic::OnBasic_Electrostatic_Data_View(int nChannel)
{
	CString strData;
	switch (nChannel)
	{
	case 1:
		if (st_work.mdReadElectData[nChannel-1] == -99999)
		{
			strData = "OPEN";
		}
		else if (st_work.mdReadElectData[nChannel-1] == -99998)
		{
			strData = "OVER";
		}
		else
		{
			strData.Format("%0.3f", st_work.mdReadElectData[nChannel-1]);
		}
		m_msg_electrostatic_1.SetWindowText(_T(strData));
		break;
		
	case 2:
		if (st_work.mdReadElectData[nChannel-1] == -99999)
		{
			strData = "OPEN";
		}
		else if (st_work.mdReadElectData[nChannel-1] == -99998)
		{
			strData = "OVER";
		}
		else
		{
			strData.Format("%0.3f", st_work.mdReadElectData[nChannel-1]);
		}
		m_msg_electrostatic_2.SetWindowText(_T(strData));
		break;
		
	case 3:
		if (st_work.mdReadElectData[nChannel-1] == -99999)
		{
			strData = "OPEN";
		}
		else if (st_work.mdReadElectData[nChannel-1] == -99998)
		{
			strData = "OVER";
		}
		else
		{
			strData.Format("%0.3f", st_work.mdReadElectData[nChannel-1]);
		}
		m_msg_electrostatic_3.SetWindowText(_T(strData));
		break;
		
	case 4:
		if (st_work.mdReadElectData[nChannel-1] == -99999)
		{
			strData = "OPEN";
		}
		else if (st_work.mdReadElectData[nChannel-1] == -99998)
		{
			strData = "OVER";
		}
		else
		{
			strData.Format("%0.3f", st_work.mdReadElectData[nChannel-1]);
		}
		m_msg_electrostatic_4.SetWindowText(_T(strData));
		break;
		
	case 5:
		if (st_work.mdReadElectData[nChannel-1] == -99999)
		{
			strData = "OPEN";
		}
		else if (st_work.mdReadElectData[nChannel-1] == -99998)
		{
			strData = "OVER";
		}
		else
		{
			strData.Format("%0.3f", st_work.mdReadElectData[nChannel-1]);
		}
		m_msg_electrostatic_5.SetWindowText(_T(strData));
		m_btn_electostatic_test.EnableWindow(TRUE);
		break;
	}
}

LRESULT CScreen_Basic::OnBasic_Info_Apply(WPARAM wParam, LPARAM lParam)  
{
	if (wParam == 0)
	{
		OnBasic_Data_Apply();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		
		OnBasic_LogFile_Create();		// Log File 생성.
		
		OnBasic_Data_BackUp();
		
		OnBasic_TitleBar_Info_Send();   // 타이틀 바 화면에 정보 출력 요청하는 함수
	}
	else if (wParam == 1)
	{
		OnBasic_Electrostatic_Data_View(lParam);
	}
	
	return 0;
}

void CScreen_Basic::OnBasic_Data_Apply()
{
	int n_response;
	CDialog_Message msg_dlg;

	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_temp_device);
	mstr_temp_device.MakeUpper();
	mstr_temp_device.TrimLeft(' ');               
	mstr_temp_device.TrimRight(' ');
	
	if (mstr_device_name[0] != mstr_temp_device)
	{
		//2017.0810
		st_handler.nModelChangeCheck = TRUE;
		st_msg.str_fallacy_msg = _T("[Basic]Model이 변경 되었습니다. 엘리베이터에 Tray가 존재하는지 확인해 주세요.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("[Basic]Model is changed. You have to check that if there is any tray in Elivator.");
		}
		sprintf(st_msg.c_normal_msg, "[Basic]Model is changed. You have to check that if there is any tray in Elivator.");
		if(st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청 //
		}
		n_response = msg_dlg.DoModal();			
		if (n_response == IDOK)
		{
		}


		OnBasic_DeviceData_Apply();
		
		OnBasic_Data_Set();				// 전역 변수의 Data를 받아온다.
		OnBasic_Data_BackUp();
		
		OnBasic_Data_Change();

		OnBasic_Data_Display();

	}
	else
	{
		st_basic.n_mode_device =	mn_device_mode[1];		// With, With Out, No Tra

		st_basic.mn_network_wait_time = mn_network_wait_time[1];
		st_basic.mn_network_retry_cnt = st_basic.n_retry_cnt = mn_network_retry_cnt[1];
		st_basic.mn_Label_Print_Error_Cnt = mn_Label_Print_Error_Cnt[1];
		st_basic.mn_Dvc_Repick_Cnt = mn_Dvc_Repick_Cnt[1];//20120731

		////////////////////////////////////////////////////////////////////// 20120522
		st_basic.mn_lotset_module_load = st_traybuffer[LDMODULE_SITE].i_loader_row = mn_lotset_module_load[1];
		st_basic.mn_lotset_hs_left =	 st_traybuffer[LEFTSINK_SITE].i_loader_row = mn_lotset_hs_left[1];
		st_basic.mn_lotset_hs_right =	 st_traybuffer[RIGHTSINK_SITE].i_loader_row = mn_lotset_hs_right[1];
		st_basic.mn_lotset_clip =	     st_traybuffer[LDCLIP_SITE].i_loader_row = mn_lotset_clip[1];
		////////////////////////////////////////////////////////////////////// 20120707
		st_basic.mn_lotset_unloader =	 st_traybuffer[UNLOADER_SITE].i_loader_row = mn_lotset_unloader[1];
		st_basic.mn_lotset_reject =	     st_traybuffer[REJECT_SITE].i_loader_row = mn_lotset_reject[1];
		////////////////////////////////////////////////////////////////////// 20120522
		st_basic.nRunSpeed =        mn_basic_run_speed[1];
		st_basic.nManualSpeed =	 mn_basic_manual_speed[1];
		st_basic.nJogSpeed =    	 mn_basic_jog_speed[1];
		st_basic.nBarcodeSpeed =    mn_basic_barcode_speed[1];

		//////////////////////////////////////////////////////////////////////



		st_basic.mn_electrostatic_check_gap =	mn_electrostatic_check_gap[1];
		st_basic.md_electrostatic_value =		md_electrostatic_value[1];

	    st_basic.md_picker_gap_module =         md_picker_gap_module[1];
	    st_basic.md_picker_gap_hs_left =        md_picker_gap_hs_left[1];
	    st_basic.md_picker_gap_hs_right =       md_picker_gap_hs_right[1];

		st_basic.mn_label_vision_paper =      mn_label_vision_paper[1];

		st_basic.mn_label_bin_print =	    mn_label_bin_print[1]; // 20140224

		st_Network[NETWORK_ECSERVER].mstr_ip =		mstr_ec_server_ip[1];
		
		st_basic.mn_alarm_delay_time = mn_alarm_delay_time[1];

		st_basic.n_tray_x = mn_tray_x[1];
		st_basic.n_tray_y = mn_tray_y[1];

		if (st_basic.n_tray_x == 1)
		{
			st_basic.n_tray_size = 0;
		}
		else
		{
			st_basic.n_tray_size = 1;
		}
		st_basic.n_mode_m_direction       = mn_mode_m_direction[1];
		st_basic.n_mode_hs_direction      = mn_mode_hs_direction[1];
		st_basic.n_mode_bcr               = mn_mode_bcr[1];
		st_basic.n_mode_module_clip_swing =	mn_mode_module_clip_swing[1];
		st_basic.n_mode_use_bin			  = mn_mode_remove_clip[1];
	    st_basic.n_mode_clip_alarm        = mn_mode_clip_alarm[1];
		st_basic.n_mode_cap_remove		  = mn_mode_use_cap_remove[1]; // 20140220

		st_basic.n_mode_use_door          = mn_mode_door_lock[1];
	    st_basic.n_mode_use_sorter_picker = mn_mode_use_sorter_picker[1];

		st_basic.n_clamp_clip_bin		  = mn_clamp_clip_bin[1];
		st_basic.n_clip_bin				  = mn_clip_bin[1];
		st_basic.n_curr_clamp_clip_bin	  = mn_curr_clamp_clip_bin[1];
		st_basic.n_curr_clip_bin		  = mn_curr_clip_bin[1];

		//2015.0129
		st_basic.n_mode_use_vis_cont_err  = mn_mode_use_vis_cont_err[1];
		st_basic.n_vis_cont_err  = mn_vis_cont_err[1];
		st_basic.n_vis_tot_err  = mn_vis_tot_err[1];

	}

	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
	}

}

void CScreen_Basic::OnBasic_LogFile_Create()
{
	CString str_msg;

	if (mn_device_mode[0] !=			mn_device_mode[1])
	{
		switch (mn_device_mode[1])
		{
		case 0:
			str_msg = "[Device Mode] Was changed by With Device.";
			break;
		case 1:
			str_msg = "[Device Mode] Was changed by With Out Device.";
			break;
		}

		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_electrostatic_check_gap[0] != mn_electrostatic_check_gap[1])
	{
		str_msg.Format("[Basic] Electrostatic Time Gap was changed by %d to %d", mn_electrostatic_check_gap[0], mn_electrostatic_check_gap[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_electrostatic_value[0] != md_electrostatic_value[1])
	{
		str_msg.Format("[Basic] Electrostatic Value was changed by %d to %d", md_electrostatic_value[0], md_electrostatic_value[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_picker_gap_module[0] != md_picker_gap_module[1])
	{
		str_msg.Format("[Basic] Module Picker Gap Value was changed by %d to %d", md_picker_gap_module[0], md_picker_gap_module[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_picker_gap_hs_left[0] != md_picker_gap_hs_left[1])
	{
		str_msg.Format("[Basic] HS Left Picker Gap Value was changed by %d to %d", md_picker_gap_hs_left[0], md_picker_gap_hs_left[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_picker_gap_hs_right[0] != md_picker_gap_hs_right[1])
	{
		str_msg.Format("[Basic] HS Right Picker Gap Value was changed by %d to %d", md_picker_gap_hs_right[0], md_picker_gap_hs_right[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}


	if (mstr_ec_server_ip[0] != mstr_ec_server_ip[1])
	{
		str_msg.Format("[Basic] EC Server IP was changed by %s to %s", mstr_ec_server_ip[0], mstr_ec_server_ip[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}


	if (mn_network_retry_cnt[0] != mn_network_retry_cnt[1])
	{
		str_msg.Format("[Basic] Network retry count was changed by %d to %d", mn_network_retry_cnt[0], mn_network_retry_cnt[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_Label_Print_Error_Cnt[0] != mn_Label_Print_Error_Cnt[1])
	{
		str_msg.Format("[Basic] Label Print Error Count was changed by %d to %d", mn_Label_Print_Error_Cnt[0], mn_Label_Print_Error_Cnt[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
////////////// 20120731
	if (mn_Dvc_Repick_Cnt[0] != mn_Dvc_Repick_Cnt[1])
	{
		str_msg.Format("[Basic] DVC Repick Count was changed by %d to %d", mn_Dvc_Repick_Cnt[0], mn_Dvc_Repick_Cnt[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
//////////////
	if (mn_network_wait_time[0] != mn_network_wait_time[1])
	{
		str_msg.Format("[Basic] Network wait time was changed by %d to %d", mn_network_wait_time[0], mn_network_wait_time[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if( mn_alarm_delay_time[0] != mn_alarm_delay_time[1] )
	{
		str_msg.Format("[Basic] Alarm Delay Time was changed by %d to %d", mn_alarm_delay_time[0], mn_alarm_delay_time[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	////////////////////////////////////////////////////////////////////// 20120522
	if (mn_lotset_module_load[0] != mn_lotset_module_load[1])
	{
		str_msg.Format("[Basic] Module Load Lot Set was changed by %d to %d", mn_lotset_module_load[0], mn_lotset_module_load[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_lotset_hs_left[0] != mn_lotset_hs_left[1])
	{
		str_msg.Format("[Basic] HeatSink Left Lot Set was changed by %d to %d", mn_lotset_hs_left[0], mn_lotset_hs_left[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_lotset_hs_right[0] != mn_lotset_hs_right[1])
	{
		str_msg.Format("[Basic] HeatSink Right Lot Set was changed by %d to %d", mn_lotset_hs_right[0], mn_lotset_hs_right[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_lotset_clip[0] != mn_lotset_clip[1])
	{
		str_msg.Format("[Basic] Clip Lot Set was changed by %d to %d", mn_lotset_clip[0], mn_lotset_clip[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
//////////////////////////////////////////////////////////////////// 20120707
	if (mn_lotset_unloader[0] != mn_lotset_unloader[1])
	{
		str_msg.Format("[Basic] Unloader Lot Set was changed by %d to %d", mn_lotset_unloader[0], mn_lotset_unloader[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_lotset_reject[0] != mn_lotset_reject[1])
	{
		str_msg.Format("[Basic] Reject Lot Set was changed by %d to %d", mn_lotset_reject[0], mn_lotset_reject[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_mode_m_direction[0] != mn_mode_m_direction[1])
	{
		str_msg.Format("[Basic] Module Direction by %d to %d", mn_mode_m_direction[0], mn_mode_m_direction[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_mode_hs_direction[0] != mn_mode_hs_direction[1])
	{
		str_msg.Format("[Basic] Heat Sink Direction by %d to %d", mn_mode_hs_direction[0], mn_mode_hs_direction[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}	

	if (mn_mode_bcr[0] != mn_mode_bcr[1])
	{
		str_msg.Format("[Basic] Mode BCR by %d to %d", mn_mode_bcr[0], mn_mode_bcr[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_mode_module_clip_swing[0] != mn_mode_module_clip_swing[1])
	{
		str_msg.Format("[Basic] Module Clip Swing by %d to %d", mn_mode_module_clip_swing[0], mn_mode_module_clip_swing[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if(mn_mode_remove_clip[0] != mn_mode_remove_clip[1])
	{
		str_msg.Format("[Basic] Clip remove by %d to %d", mn_mode_remove_clip[0], mn_mode_remove_clip[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if(mn_mode_clip_alarm[0] != mn_mode_clip_alarm[1])
	{
		str_msg.Format("[Basic] Clip alarm by %d to %d", mn_mode_clip_alarm[0], mn_mode_clip_alarm[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if(mn_mode_use_cap_remove[0] != mn_mode_use_cap_remove[1]) // 20140220
	{
		str_msg.Format("[Basic] Clip alarm by %d to %d", mn_mode_use_cap_remove[0], mn_mode_use_cap_remove[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}


	if(mn_mode_door_lock[0] != mn_mode_door_lock[1])
	{
		str_msg.Format("[Basic] AMT Door Use by %d to %d", mn_mode_door_lock[0], mn_mode_door_lock[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if(mn_mode_use_sorter_picker[0] != mn_mode_use_sorter_picker[1])
	{
		str_msg.Format("[Basic] Sorter Picker Use by %d to %d", mn_mode_use_sorter_picker[0], mn_mode_use_sorter_picker[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}


	if(mn_mode_use_vis_cont_err[0] != mn_mode_use_vis_cont_err[1])
	{
		str_msg.Format("[Basic] VisionHS Mode Continue err by %d to %d", mn_mode_use_vis_cont_err[0], mn_mode_use_vis_cont_err[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if(mn_vis_cont_err[0] != mn_vis_cont_err[1])
	{
		str_msg.Format("[Basic] VisionHS Continue err by %d to %d", mn_mode_use_vis_cont_err[0], mn_mode_use_vis_cont_err[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
	if(mn_vis_tot_err[0] != mn_vis_tot_err[1])
	{
		str_msg.Format("[Basic] VisionHS Total err by %d to %d", mn_mode_use_vis_cont_err[0], mn_mode_use_vis_cont_err[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////// 20120523
	if (mn_basic_run_speed[0] != mn_basic_run_speed[1])
	{
		str_msg.Format("[Basic] Run Speed Set was changed by %d to %d", mn_basic_run_speed[0], mn_basic_run_speed[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_basic_manual_speed[0] != mn_basic_manual_speed[1])
	{
		str_msg.Format("[Basic] Manual Speed Set was changed by %d to %d", mn_basic_manual_speed[0], mn_basic_manual_speed[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_basic_jog_speed[0] != mn_basic_run_speed[1])
	{
		str_msg.Format("[Basic] Jog Speed Set was changed by %d to %d", mn_basic_jog_speed[0], mn_basic_jog_speed[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_basic_barcode_speed[0] != mn_basic_barcode_speed[1])
	{
		str_msg.Format("[Basic] Barcode Speed Set was changed by %d to %d", mn_basic_barcode_speed[0], mn_basic_barcode_speed[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_label_vision_paper[0] != mn_label_vision_paper[1])
	{
		str_msg.Format("[Basic] Label<->Vision Paper Set was changed by %d to %d", mn_label_vision_paper[0], mn_label_vision_paper[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	// 20140224
	if (mn_label_bin_print[0] != mn_label_bin_print[1])
	{
		str_msg.Format("[Basic] Label Bin Print was changed by %d to %d", mn_label_bin_print[0], mn_label_bin_print[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
	//
	
	if (mn_clamp_clip_bin[0] != mn_clamp_clip_bin[1])
	{
		str_msg.Format("[Basic] Clamp Clip Bin was changed by %d to %d", mn_clamp_clip_bin[0], mn_clamp_clip_bin[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_clip_bin[0] != mn_clip_bin[1])
	{
		str_msg.Format("[Basic] Clip Bin was changed by %d to %d", mn_clip_bin[0], mn_clip_bin[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
	if (mn_curr_clamp_clip_bin[0] != mn_curr_clamp_clip_bin[1])
	{
		str_msg.Format("[Basic] Current Clamp Clip Bin was changed by %d to %d", mn_curr_clamp_clip_bin[0], mn_curr_clamp_clip_bin[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_curr_clip_bin[0] != mn_curr_clip_bin[1])
	{
		str_msg.Format("[Basic] Current Clip Bin was changed by %d to %d", mn_curr_clamp_clip_bin[0], mn_curr_clamp_clip_bin[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
}

void CScreen_Basic::OnBasic_TitleBar_Info_Send()
{
	if(st_handler.cwnd_title != NULL)
	{
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DEVICE_MODE, mn_device_mode[1]);			// Device Mode
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, BCR_MODE, 0);			// Device Mode
		st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, DIRECTION_MODE, 0);			// Device Mode
	}
	
	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);
	}
}

void CScreen_Basic::OnBasic_DeviceData_Apply()
{
	int n_response = 0;  // 대화 상자 리턴 플래그
	int n_check;
	CString str_msg;
	
	n_check = OnBasic_Input_Data_Check();  // 입력 정보 정상 유무 검사 함수
	
	if (n_check == TRUE)  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);  // 디바이스명 출력 요청
		
		str_msg = "[Test Device ] Was changed by " + mstr_device_name[1];
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
	else if (n_check == FALSE)  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg = _T("[LOT INFORMATION] An input device setting up error.");
			sprintf(st_msg.c_abnormal_msg, "[LOT INFORMATION] An input device setting up error.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return ;
	}
	
	st_basic.mstr_device_name =	mstr_device_name[1];
	mstr_device_name[0] =		mstr_device_name[1];
	
	// **********************************************************************
	// 디바이스 관련 화면 설정 데이터 로딩 부분                              
	// **********************************************************************
	mcls_basic.OnBasic_Data_Load();
	mcls_basic.On_Teach_Data_Load();
	// **********************************************************************

	////////////// 20121128
	st_work.mn_reinstatement_mode[LDMODULE_SITE] = 0;		// 복구 동작용 위치값을 다시 읽어라. 2K4/09/21/ViboX
	st_work.mn_reinstatement_mode[WORK_BUFFER_SITE] = 0;		// 복구 동작용 위치값을 다시 읽어라. 2K4/09/21/ViboX
	st_work.mn_reinstatement_mode[LEFTSINK_SITE] = 0;		// 복구 동작용 위치값을 다시 읽어라. 2K4/09/21/ViboX
	st_work.mn_reinstatement_mode[RIGHTSINK_SITE] = 0;		// 복구 동작용 위치값을 다시 읽어라. 2K4/09/21/ViboX
	st_work.mn_reinstatement_mode[SORTER_SITE] = 0;		// 복구 동작용 위치값을 다시 읽어라. 2K4/09/21/ViboX
	st_work.mn_reinstatement_mode[UNLOADER_SITE] = 0;		// 복구 동작용 위치값을 다시 읽어라. 2K4/09/21/ViboX
//	st_work.mn_reinstatement_mode[0] = 0;		// 복구 동작용 위치값을 다시 읽어라. 2K4/09/21/ViboX
	//////////////

	// **************************************************************************
	// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다                        
	// **************************************************************************
	m_list_device_type.ResetContent();			// 리스트 박스의 모든 목록 삭제
	mp_device_image_list.DeleteImageList();		// 이미지 리스트에 설정된 정보 삭제
	
	OnBasic_Create_ImageList();					// 이미지 리스트 생성 함수
	
	// Associate image list to list box
	m_list_device_type.SetImageList(&mp_device_image_list);
	
	OnBasic_Init_List(st_path.mstr_path_dvc);	// 파일 폴더 초기화 함수
	
	OnBasic_Device_Focus_Set();					// 선택된 디바이스명 포커스 설정 함수
	// **************************************************************************
	
	if (st_handler.cwnd_list != NULL)			// 리스트 바 화면 존재
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO);  // 디바이스 종류 출력 요청
}

void CScreen_Basic::OnBasic_Data_Display()
{
	CString strTemp;

	m_dgt_electrostatic_check_gap.SetValue(mn_electrostatic_check_gap[1]);
	m_dgt_electrostatic_check_value.SetFloatVal((float)md_electrostatic_value[1]);
	
	m_dgt_picker_gap_module.SetFloatVal((float)md_picker_gap_module[1]);
	m_dgt_picker_gap_hs_left.SetFloatVal((float)md_picker_gap_hs_left[1]);
	m_dgt_picker_gap_hs_right.SetFloatVal((float)md_picker_gap_hs_right[1]);

	m_dgt_picker_gap_module.SetFloatVal((float)md_picker_gap_module[1]);
	m_dgt_picker_gap_hs_left.SetFloatVal((float)md_picker_gap_hs_left[1]);
	m_dgt_picker_gap_hs_right.SetFloatVal((float)md_picker_gap_hs_right[1]);


	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText(mstr_device_name[1]);
	

	////2014.0818
	strTemp.Format("%d", st_basic.n_ld_node);
 	((CEdit*)GetDlgItem(IDC_EDIT_LD_NODE))->SetWindowText(strTemp);
	strTemp.Format("%d", st_basic.n_hs_front_node);
 	((CEdit*)GetDlgItem(IDC_EDIT_HS_LEFT_NODE))->SetWindowText(strTemp);
	strTemp.Format("%d", st_basic.n_hs_back_node);
	((CEdit*)GetDlgItem(IDC_EDIT_HS_RIGHTT_NODE))->SetWindowText(strTemp);
	strTemp.Format("%d", st_basic.n_clip_node);
	((CEdit*)GetDlgItem(IDC_EDIT_CLIP_NODE))->SetWindowText(strTemp);
// 	strTemp.Format("%d", st_basic.n_uld_node);
// 	((CEdit*)GetDlgItem(IDC_EDIT_ULD_NODE))->SetWindowText(strTemp);
	////



	OnIP_Address(mstr_ec_server_ip[1], NETWORK_ECSERVER);
	

	m_dgt_tray_x.SetValue(mn_tray_x[1]);
	m_dgt_tray_y.SetValue(mn_tray_y[1]);

	//////////////////////////////////////// 20120523
	m_dgt_lotset_module_load.SetValue(mn_lotset_module_load[1]);
	m_dgt_lotset_hs_left.SetValue(mn_lotset_hs_left[1]);
	m_dgt_lotset_hs_right.SetValue(mn_lotset_hs_right[1]);
	m_dgt_lotset_clip.SetValue(mn_lotset_clip[1]);
	m_dgt_lotset_unloader.SetValue(mn_lotset_unloader[1]);//20120707
	m_dgt_lotset_reject.SetValue(mn_lotset_reject[1]);//20120707

	m_dgt_basic_run_speed.SetValue(mn_basic_run_speed[1]);
	m_dgt_basic_manual_speed.SetValue(mn_basic_manual_speed[1]);
	m_dgt_basic_jog_speed.SetValue(mn_basic_jog_speed[1]);
	m_dgt_basic_barcode_speed.SetValue(mn_basic_barcode_speed[1]);
	

	m_dgt_alarm_delay_time.SetValue(mn_alarm_delay_time[1]);

	m_dgt_network_wait_time.SetValue(mn_network_wait_time[1]);		// Network 대기 시간
	m_dgt_network_retry_cnt.SetValue(mn_network_retry_cnt[1]);		// Network Retry 횟수

	m_dgt_Label_Print_Error_Cnt.SetValue(mn_Label_Print_Error_Cnt[1]);		// Network Retry 횟수
	
	m_dgt_Dvc_Repick_Cnt.SetValue(mn_Dvc_Repick_Cnt[1]);//20120731

	m_dgt_label_vision_paper.SetValue(mn_label_vision_paper[1]);

	m_dgt_label_bin_print.SetValue(mn_label_bin_print[1]); // 20140224

	OnBasic_direction_Change();

	//////////////////////////////////////////////////////////////////////
}

int CScreen_Basic::OnBasic_Input_Data_Check()
{
	CString str_chk_ext;   // 확장자 저장 변수
	int n_pos;
	
	/* ************************************************************************** */
    /* 입력된 디바이스명 설정한다                                                 */
    /* ************************************************************************** */
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_temp_device) ;
	mstr_temp_device.MakeUpper();
	mstr_temp_device.TrimLeft(' ');               
	mstr_temp_device.TrimRight(' ');
	
	if (mstr_temp_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)			// 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg = _T("[DEVICE] A name input error occurrence.");
			sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name input error occurrence.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	/* ************************************************************************** */
	
	n_pos = mstr_temp_device.Find(".");				// 확장자 검사
	if (n_pos == -1) 
		mstr_temp_device += _T(".TXT");
	else 
	{
		str_chk_ext = mstr_temp_device.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)		// 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = _T("[DEVICE] A name extension input error occurrence.");
				sprintf(st_msg.c_abnormal_msg, "[DEVICE] A name extension input error occurrence.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return FALSE;
		}
	}
	
	mstr_device_name[1] = mstr_temp_device;			// 디바이스 파일명 설정
	/* ************************************************************************** */
	
	return TRUE;
}

void CScreen_Basic::OnBtnBasicApply() 
{
	CString str_selected_dvc;
	int nDeviceCur = m_list_device_type.GetCurSel();

	if( nDeviceCur != mn_device_name )
	{
		if( g_AMTLotMgr.CheckChangeableDIMM() == false )
		{
			st_msg.mstr_event_msg[0] = "Can't Change Device Type for Working";
			::SendMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 0);
			return;
		}
	}

	int n_response;  // 대화 상자 리턴 플래그
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Basic : Setting Data Apply?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnBasic_Data_Comparison();	//20120516

		OnBasic_Data_Apply();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		
		OnBasic_LogFile_Create();		// Log File 생성.
		
		OnBasic_Data_BackUp();
		
		OnBasic_TitleBar_Info_Send();    // 타이틀 바 화면에 정보 출력 요청하는 함수

		//////////////////////////////////// 20120522
		OnBasic_Data_Display();

		mcls_basic.OnBasic_Data_Save();//20120731
		mcls_basic.OnBasic_Data_Load();//20120731

		////////////////////////////////////
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnBtnBasicCancle() 
{
	int n_response;  // 대화 상자 리턴 플래그
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Basic : Previous Data Recovery?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnBasic_Data_Recovery();			// 베이식 화면 셋팅 정보 백업 받아놓은 변수로 복구 함수
		
		OnBasic_Data_Change();				// Data에 맞게 변경.
		
		OnBasic_Data_Display();
		
		OnBasic_TitleBar_Info_Send();		// 타이틀 바 화면에 정보 출력 요청하는 함수
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}

int CScreen_Basic::OnBasic_Data_Comparison()
{
	int nFuncRet = CTLBD_RET_GOOD;

	if (mn_device_mode[0] !=				mn_device_mode[1])					return RET_ERROR;

	if (mn_electrostatic_check_gap[0] !=	mn_electrostatic_check_gap[1])		return RET_ERROR;
	if (md_electrostatic_value[0] !=		md_electrostatic_value[1])			return RET_ERROR;
	
	if (md_picker_gap_module[0] !=		md_picker_gap_module[1])				return RET_ERROR;
	if (md_picker_gap_hs_left[0] !=		md_picker_gap_hs_left[1])				return RET_ERROR;
	if (md_picker_gap_hs_right[0] !=		md_picker_gap_hs_right[1])			return RET_ERROR;

	if( mn_alarm_delay_time[0] !=			mn_alarm_delay_time[1] )			return RET_ERROR;
	
	if (mstr_device_name[0] !=				mstr_device_name[1])				return RET_ERROR;
	
	if (mstr_ec_server_ip[0] !=				mstr_ec_server_ip[1])				return RET_ERROR;
	
	if (mn_label_vision_paper[0] !=      mn_label_vision_paper[1])				return RET_ERROR;

	if (mn_mode_m_direction[0] !=      mn_mode_m_direction[1])					return RET_ERROR;
	if (mn_mode_hs_direction[0] !=      mn_mode_hs_direction[1])				return RET_ERROR;
	if (mn_mode_bcr[0] !=				mn_mode_bcr[1])							return RET_ERROR;
	if (mn_mode_module_clip_swing[0] !=      mn_mode_module_clip_swing[1])      return RET_ERROR;
	if (mn_mode_remove_clip[0] !=			mn_mode_remove_clip[1])				return RET_ERROR;
	if (mn_mode_clip_alarm[0] !=			mn_mode_clip_alarm[1])				return RET_ERROR;
	if (mn_mode_use_cap_remove[0] !=		mn_mode_use_cap_remove[1])			return RET_ERROR; // 20140220
	if (mn_label_bin_print[0] !=      mn_label_bin_print[1])					return RET_ERROR; // 20140224


	if (mn_mode_door_lock[0] !=			mn_mode_door_lock[1])			    	return RET_ERROR;
	if (mn_mode_use_sorter_picker[0] !=			mn_mode_use_sorter_picker[1])  	return RET_ERROR;

	if (mstr_part_name[0] !=			mstr_part_name[1])  	return RET_ERROR;//20121220

	//2015.0129
	if( mn_mode_use_vis_cont_err[0] != mn_mode_use_vis_cont_err[1])		return RET_ERROR;
	if( mn_vis_cont_err[0] != mn_vis_cont_err[1])		return RET_ERROR;
	if( mn_vis_tot_err[0] != mn_vis_tot_err[1])		return RET_ERROR;


	return nFuncRet;
}

void CScreen_Basic::OnDgtNetworkWaitTime() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_network_wait_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Network 메세지 대기 시간 (초)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Network wait time (sec)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "120";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_network_wait_time.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_network_wait_time[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_network_wait_time.SetValue(mn_network_wait_time[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnDgtNetworkRetryCount() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_network_retry_cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Network 메세지 재전송 횟수");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Network retry count");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "120";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_network_retry_cnt.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_network_retry_cnt[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_network_retry_cnt.SetValue(mn_network_retry_cnt[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnDgtAlarmDelayTime() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Sec;
	
	CDialog_KeyPad pad_dlg;
	
	mn_Sec = m_dgt_alarm_delay_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Sec, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Alarm Delay Time (Sec)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "1000";
	st_msg.mstr_pad_low_limit = "1";

	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_alarm_delay_time.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_alarm_delay_time[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_alarm_delay_time.SetValue(mn_alarm_delay_time[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
}





void CScreen_Basic::OnDgtTrayX() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_x;
	
	CDialog_KeyPad pad_dlg;
	
	mn_x = m_dgt_tray_x.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_x, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Tray X Size [EA]");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "3";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_tray_x.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_tray_x[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_tray_x.SetValue(mn_tray_x[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnDgtTrayY() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_y;
	
	CDialog_KeyPad pad_dlg;
	
	mn_y = m_dgt_tray_y.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_y, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Tray Y Size [EA]");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "25";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_tray_y.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_tray_y[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_tray_y.SetValue(mn_tray_y[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}


///////////////////////////////////////////////////////////// 20120522
void CScreen_Basic::OnDgtLotsetModuleLoad() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_lotset_module_load.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("MODULE LOAD LOT SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("MODULE LOAD LOT SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_lotset_module_load.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_lotset_module_load[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_lotset_module_load.SetValue(mn_lotset_module_load[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
	
}

void CScreen_Basic::OnDgtLotsetHsLeft() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_lotset_hs_left.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("HEATSINK LEFT LOT SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("HEATSINK LEFT LOT SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_lotset_hs_left.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_lotset_hs_left[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_lotset_hs_left.SetValue(mn_lotset_hs_left[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
}

void CScreen_Basic::OnDgtLotsetHsRight() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_lotset_hs_right.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("HEATSINK RIGHT LOT SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("HEATSINK RIGHT LOT SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_lotset_hs_right.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_lotset_hs_right[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_lotset_hs_right.SetValue(mn_lotset_hs_right[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLotsetClip() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_lotset_clip.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("CLIP LOT SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("CLIP LOT SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_lotset_clip.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_lotset_clip[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_lotset_clip.SetValue(mn_lotset_clip[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
	
}

///////////////////////////////////////////////////////////// 20120523	
void CScreen_Basic::OnDgtRunSpeed() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_basic_run_speed.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("RUN SPEED (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("RUN SPEED SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_basic_run_speed.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_basic_run_speed[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_basic_run_speed.SetValue(mn_basic_run_speed[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
}
void CScreen_Basic::OnDgtManualSpeed() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_basic_manual_speed.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("MANUAL SPEED (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("MANUAL SPEED SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_basic_manual_speed.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_basic_manual_speed[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_basic_manual_speed.SetValue(mn_basic_manual_speed[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}
void CScreen_Basic::OnDgtJogSpeed() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_basic_jog_speed.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("JOG SPEED (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("JOG SPEED SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_basic_jog_speed.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_basic_jog_speed[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_basic_jog_speed.SetValue(mn_basic_jog_speed[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}

void CScreen_Basic::OnDgtBarcodeSpeed() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_basic_barcode_speed.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("BARCODE SPEED (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("BARCODE SPEED SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_basic_barcode_speed.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_basic_barcode_speed[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_basic_barcode_speed.SetValue(mn_basic_barcode_speed[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}				
}
///////////////////////////////////////////////////////////// 

///////////////////////////////////////////////////// 20120529

void CScreen_Basic::OnBtnLabelCreate() 
{
	// TODO: Add your control notification handler code here
	int n_response;									// 대화 상자 리턴 플래그
	CString str_selected_lbl, str_filename;		//20120530					// 선택된 디바이스 이름 저장 변수

	CString str_tmp;
	CString str_print_data;
	
	CDialog_Keyboard  keyboard_dlg;
	
	(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	
	//////////////////////////////////////// 20120608
	str_tmp.Format("%03d", (st_basic.mn_data_change_list + 1));
	str_print_data = str_tmp + ")";
	st_msg.mstr_typing_msg = str_print_data;
	////////////////////////////////////////


	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		mstr_new_label = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정
		mstr_new_label = mstr_new_label + ".TXT";
		if (OnBasic_New_Label_Check(mstr_new_label) == FALSE)	return;
		b_create_list = TRUE;
		OnBasic_Create_Label();
		
		// ******************************************************************
		// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다
		// ******************************************************************
		m_list_label_type.ResetContent();		// 리스트 박스의 모든 목록 삭제
		
		st_basic.mn_data_change_list = 0;	//20120608
		OnBasic_Label_Init_List(st_path.mstr_label_folder);// 파일 폴더 초기화 함수
		//OnSelchangeListLabelType();
		OnBasic_Label_Focus_Set();				// 선택된 디바이스명 포커스 설정 함수
		
		if (mn_label_name > 0)
		{
			m_list_label_type.GetText(mn_label_name, str_selected_lbl);
		}
		
		// ******************************************************************
	}
	else if (n_response == IDCANCEL)
	{
	}	
}

void CScreen_Basic::OnBtnLabelDelete() 
{
	// TODO: Add your control notification handler code here
	int n_response;				// 대화 상자 리턴 플래그
	int n_existence;			// 파일 존재 유무 플래그
	CString str_label_file_name;
	CString str_selected_lbl;
	
	char chr_file_name[256];
	
	CDialog_Select  select_dlg;
	CDialog_Message msg_dlg;
	
	if (OnBasic_New_Label_Check(mstr_label_name[1]) == FALSE)  // 입력된 디바이스 이름 정상 유무 검사 함수
		return ;
	
	st_msg.mstr_confirm_msg = _T(mstr_label_name[1] + " Label을 삭제 하시겠습니까??");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Are you sure you want to delete the Label?" + mstr_label_name[1]);
	}

	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		sprintf(chr_file_name, "%s", mstr_load_file);  // [폴더]+[파일명] 설정
	
		n_existence = access(chr_file_name, 0) ;
		
		if (n_existence != -1)					// 파일 존재
		{
 			DeleteFile(chr_file_name);			// 해당 파일 삭제
			
			// ******************************************************************
			// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다
			// ******************************************************************
			m_list_label_type.ResetContent();		// 리스트 박스의 모든 목록 삭제
			
			st_basic.mn_data_change_list = 0;	//20120608
			OnBasic_Label_Init_List(st_path.mstr_label_folder);// 파일 폴더 초기화 함수
			//OnSelchangeListLabelType();
			OnBasic_Label_Focus_Set();				// 선택된 디바이스명 포커스 설정 함수
			
			if (mn_label_name > -1)
			{
				m_list_label_type.GetText(mn_label_name, str_selected_lbl);		
			}
			// ******************************************************************
		}
		else  
			return ;
	}
	else if (n_response == IDCANCEL)
	{
	}	
}


void CScreen_Basic::OnSelchangeListLabelType() 
{
	// TODO: Add your control notification handler code here
	CString str_selected_lbl;						// 선택된 디바이스 이름 저장 변수
	int n_response;				// 대화 상자 리턴 플래그
	CDialog_Select  select_dlg;
	
	int n_index = m_list_label_type.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_label_type.GetText(n_index, str_selected_lbl);
		mstr_label_name[1] = str_selected_lbl;
		mn_label_mode[1] = n_index;
	}


	st_msg.mstr_confirm_msg = _T(mstr_label_name[1] + " Label을 선택하시겠습니까?? ");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Are you sure you want to select the Label?" + mstr_label_name[1]);
	}

	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		st_basic.mstr_label_name = mstr_label_name[1];

		st_basic.n_mode_label = mn_label_mode[1];
	
		if (b_create_list == TRUE)
		{
			b_create_list = FALSE;
			OnBasic_Label_Data_Set();
			OnBasic_Label_Data_Display();
		}
		else
		{
			mcls_basic.OnBasic_Label_Data_Load();
			OnBasic_Label_Data_Set();
			OnBasic_Label_Data_Display();
		}
				
// 		if (n_index == 1)
// 		{
// 			m_static_label_view.ShowWindow(TRUE);
// 			m_static_label_view2.ShowWindow(FALSE);
// 			m_static_label_view3.ShowWindow(FALSE);
// 		}
// 		else if (n_index == 2)
// 		{
// 			m_static_label_view.ShowWindow(FALSE);
// 			m_static_label_view2.ShowWindow(TRUE);
// 			m_static_label_view3.ShowWindow(FALSE);
// 		}
// 		else if (n_index == 3)
// 		{			
// 			m_static_label_view.ShowWindow(FALSE);
// 			m_static_label_view2.ShowWindow(FALSE);
// 			m_static_label_view3.ShowWindow(TRUE);
// 		}
// 		else
// 		{
// 			m_static_label_view.ShowWindow(FALSE);
// 			m_static_label_view2.ShowWindow(FALSE);
// 			m_static_label_view3.ShowWindow(FALSE);
// 		}
	}
	else if (n_response == IDCANCEL)
	{
		OnBasic_Label_Focus_Set();
		mn_label_mode[1] = mn_label_mode[0];
		mstr_label_name[1] = mstr_label_name[0];
			

	}	

	
}

///////////////////////////////////////////////////// 20120530
int CScreen_Basic::OnBasic_New_Label_Check(CString str_label)
{
	CString str_chk_file;  // 생성할 [폴더]+[파일명] 저장 변수
	CString str_chk_ext;   // 확장자 저장 변수
	int n_pos;

	/* ************************************************************************** */
    /* 입력된 디바이스명 설정한다                                                 */
    /* ************************************************************************** */
	str_label.MakeUpper();
	str_label.TrimLeft(' ');               
	str_label.TrimRight(' ');

	if(str_label.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_normal_msg = _T("[LABEL] A name input error occurrence.");
			sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	/* ************************************************************************** */

	str_chk_file = st_path.mstr_label_folder + str_label;  // 생성할 [폴더]+[파일명] 설정
	n_pos = str_chk_file.Find(".");  // 확장자 검사
	if (n_pos == -1) 
		str_chk_file += _T(".TXT");
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				st_other.str_normal_msg = _T("[LABEL] A name extension input error occurrence.");
				sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
			}
			return FALSE;
		}
	}

	mstr_load_file = str_chk_file;  // 생성할 [폴더]+[파일명]+[확장자] 설정

	return TRUE;
}

void CScreen_Basic::OnBasic_Create_Label()
{
	int n_existence;								// 파일 존재 유무 플래그
	char chr_file_name[256];
	CString str_label_temp;
	CString str_original_path;
	CString str_original_file;
	CString str_copy_file;
	CString str_chk_ext;
	CString str_copy_info;
	
	sprintf(chr_file_name, "%s", mstr_load_file);	// [폴더]+[파일명] 설정
	
	n_existence = access(chr_file_name, 0) ;
	
	if (n_existence != -1)							// 파일 존재
		return ;
	else  
	{
		str_label_temp = st_basic.mstr_label_name;
		st_basic.mstr_label_name = mstr_new_label;
		
		:: WritePrivateProfileString("Device", "DeviceFile", LPCTSTR(mstr_new_label), mstr_load_file); 
			
		st_basic.mstr_label_name = str_label_temp;
	}
}
//////////////////////////////////////////////////////////////////// 20120531
void CScreen_Basic::OnBtnLabelApply() 
{
	// TODO: Add your control notification handler code here
	CString str_selected_lbl;
	int nLabelCur = m_list_label_type.GetCurSel();

	if( nLabelCur != mn_label_name )
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
	
	st_msg.mstr_confirm_msg = _T("Basic : Label Data를 등록하시겠습니까??");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Basic : Do you want to Label Data??");
	}

	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnBasic_Label_Data_Apply();			// 베이식 화면 셋팅 정보 전역 변수에 전송 함수
		
		OnBasic_Label_LogFile_Create();		// Log File 생성.

		OnBasic_Label_Data_Display();

		mcls_basic.OnBasic_Label_Data_Save();	
		mcls_basic.OnBasic_Label_Data_Load();

				
	}
	else if (n_response == IDCANCEL)
	{
		
	}
}
////////////////////////////////////////////////////////////////////
void CScreen_Basic::OnBtnLabelCancle() 
{
	// TODO: Add your control notification handler code here
	int n_response;  // 대화 상자 리턴 플래그
	
	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Basic : 취소 하시겠습니까??");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Basic : Do you want to Cancel??");
	}

	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnBasic_Label_Data_Recovery();			// 베이식 화면 셋팅 정보 백업 받아놓은 변수로 복구 함수
		OnBasic_Label_Data_Display();
	}
	else if (n_response == IDCANCEL)
	{
		
	}	
}

int CScreen_Basic::OnBasic_Label_Data_Comparison()
{
 	int nFuncRet = CTLBD_RET_GOOD;
// 
// 	if (mn_label_mode[0] !=		mn_label_mode[1])				return RET_ERROR;
// 
// 	if (mstr_label_name[0] !=	mstr_label_name[1])			return RET_ERROR;
// 	
 	return nFuncRet;
}

void CScreen_Basic::OnBasic_Label_Data_Apply()
{
// 	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_temp_device);
// 	mstr_temp_label.MakeUpper();
// 	mstr_temp_label.TrimLeft(' ');               
// 	mstr_temp_label.TrimRight(' ');
	
	if (mstr_label_name[0] != mstr_label_name[1])//mstr_temp_label)
	{
		OnBasic_Label_DeviceData_Apply();
		OnBasic_Label_Data_Set();				// 전역 변수의 Data를 받아온다.
		OnBasic_Label_Data_BackUp();
		OnBasic_Label_Data_Display();		
	}
	else
	{
		st_basic.n_mode_label =	mn_label_mode[1];		// With, With Out, No Tra
		
		st_basic.mn_labelset_sd = mn_labelset_sd[1];
		st_basic.mn_labelset_lt = mn_labelset_lt[1];
		st_basic.mn_labelset_lh1 = mn_labelset_lh1[1];
		st_basic.mn_labelset_lh2 = mn_labelset_lh2[1];
		////////////////////20120906
		st_basic.md_labelset_x1_pos = md_labelset_x1_pos[1];
		st_basic.md_labelset_x2_pos = md_labelset_x2_pos[1];
		st_basic.md_labelset_x3_pos = md_labelset_x3_pos[1];
		st_basic.md_labelset_x4_pos = md_labelset_x4_pos[1];
		st_basic.md_labelset_x5_pos = md_labelset_x5_pos[1];
		st_basic.md_labelset_x6_pos = md_labelset_x6_pos[1];
		st_basic.md_labelset_y1_pos = md_labelset_y1_pos[1];
		st_basic.md_labelset_y2_pos = md_labelset_y2_pos[1];
		st_basic.md_labelset_y3_pos = md_labelset_y3_pos[1];
		st_basic.md_labelset_y4_pos = md_labelset_y4_pos[1];
		st_basic.md_labelset_y5_pos = md_labelset_y5_pos[1];
		st_basic.md_labelset_y6_pos = md_labelset_y6_pos[1];
		st_basic.md_labelset_x1_width = md_labelset_x1_width[1];
		st_basic.md_labelset_x2_width = md_labelset_x2_width[1];
		st_basic.md_labelset_x3_width = md_labelset_x3_width[1];
		st_basic.md_labelset_x4_width = md_labelset_x4_width[1];
		st_basic.md_labelset_y1_height = md_labelset_y1_height[1];
		st_basic.md_labelset_y2_height = md_labelset_y2_height[1];
		st_basic.md_labelset_y3_height = md_labelset_y3_height[1];
		st_basic.md_labelset_y4_height = md_labelset_y4_height[1];
		////////////////////

	}


}

void CScreen_Basic::OnBasic_Label_LogFile_Create()
{
	CString str_msg;

	if (mn_label_mode[0] !=			mn_label_mode[1])
	{
		switch (mn_label_mode[1])
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
	if (mn_labelset_sd[0] != mn_labelset_sd[1])
	{
		str_msg.Format("[Basic] SD Label Set was changed by %d to %d", mn_labelset_sd[0], mn_labelset_sd[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_labelset_lt[0] != mn_labelset_lt[1])
	{
		str_msg.Format("[Basic] LT Label Set was changed by %d to %d", mn_labelset_lt[0], mn_labelset_lt[1]);
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

	////////////////////20120906
	if (md_labelset_x1_pos[0] != md_labelset_x1_pos[1])
	{
		str_msg.Format("[Basic] X1-POS Label Set was changed by %d to %d", md_labelset_x1_pos[0], md_labelset_x1_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x2_pos[0] != md_labelset_x2_pos[1])
	{
		str_msg.Format("[Basic] X2-POS Label Set was changed by %d to %d", md_labelset_x2_pos[0], md_labelset_x2_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x3_pos[0] != md_labelset_x3_pos[1])
	{
		str_msg.Format("[Basic] X3-POS Label Set was changed by %d to %d", md_labelset_x3_pos[0], md_labelset_x3_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x4_pos[0] != md_labelset_x4_pos[1])
	{
		str_msg.Format("[Basic] X4-POS Label Set was changed by %d to %d", md_labelset_x4_pos[0], md_labelset_x4_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x5_pos[0] != md_labelset_x5_pos[1])
	{
		str_msg.Format("[Basic] X5-POS Label Set was changed by %d to %d", md_labelset_x5_pos[0], md_labelset_x5_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x6_pos[0] != md_labelset_x6_pos[1])
	{
		str_msg.Format("[Basic] X6-POS Label Set was changed by %d to %d", md_labelset_x6_pos[0], md_labelset_x6_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y1_pos[0] != md_labelset_y1_pos[1])
	{
		str_msg.Format("[Basic] Y1-POS Label Set was changed by %d to %d", md_labelset_y1_pos[0], md_labelset_y1_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y2_pos[0] != md_labelset_y2_pos[1])
	{
		str_msg.Format("[Basic] Y2-POS Label Set was changed by %d to %d", md_labelset_y2_pos[0], md_labelset_y2_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y3_pos[0] != md_labelset_y3_pos[1])
	{
		str_msg.Format("[Basic] Y3-POS Label Set was changed by %d to %d", md_labelset_y3_pos[0], md_labelset_y3_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y4_pos[0] != md_labelset_y4_pos[1])
	{
		str_msg.Format("[Basic] Y4-POS Label Set was changed by %d to %d", md_labelset_y4_pos[0], md_labelset_y4_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y5_pos[0] != md_labelset_y5_pos[1])
	{
		str_msg.Format("[Basic] Y5-POS Label Set was changed by %d to %d", md_labelset_y5_pos[0], md_labelset_y5_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y6_pos[0] != md_labelset_y6_pos[1])
	{
		str_msg.Format("[Basic] Y6-POS Label Set was changed by %d to %d", md_labelset_y6_pos[0], md_labelset_y6_pos[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x1_width[0] != md_labelset_x1_width[1])
	{
		str_msg.Format("[Basic] X1-WIDTH Label Set was changed by %d to %d", md_labelset_x1_width[0], md_labelset_x1_width[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x2_width[0] != md_labelset_x2_width[1])
	{
		str_msg.Format("[Basic] X2-WIDTH Label Set was changed by %d to %d", md_labelset_x2_width[0], md_labelset_x2_width[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x3_width[0] != md_labelset_x3_width[1])
	{
		str_msg.Format("[Basic] X3-WIDTH Label Set was changed by %d to %d", md_labelset_x3_width[0], md_labelset_x3_width[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_x4_width[0] != md_labelset_x4_width[1])
	{
		str_msg.Format("[Basic] X4-WIDTH Label Set was changed by %d to %d", md_labelset_x4_width[0], md_labelset_x4_width[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y1_height[0] != md_labelset_y1_height[1])
	{
		str_msg.Format("[Basic] Y1-HEIGHT Label Set was changed by %d to %d", md_labelset_y1_height[0], md_labelset_y1_height[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y2_height[0] != md_labelset_y2_height[1])
	{
		str_msg.Format("[Basic] Y2-HEIGHT Label Set was changed by %d to %d", md_labelset_y2_height[0], md_labelset_y2_height[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y3_height[0] != md_labelset_y3_height[1])
	{
		str_msg.Format("[Basic] Y3-HEIGHT Label Set was changed by %d to %d", md_labelset_y3_height[0], md_labelset_y3_height[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (md_labelset_y4_height[0] != md_labelset_y4_height[1])
	{
		str_msg.Format("[Basic] Y4-HEIGHT Label Set was changed by %d to %d", md_labelset_y4_height[0], md_labelset_y4_height[1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
	////////////////////



}
void CScreen_Basic::OnBasic_Label_Data_BackUp()
{
	mn_label_mode[0] =				mn_label_mode[1];				// With, With Out, No Tray
	mstr_label_name[0] =			mstr_label_name[1];					// 선택된 디바이스 종류 저장 변수

	mn_labelset_sd[0] =	        	mn_labelset_sd[1];
	mn_labelset_lt[0] =	        	mn_labelset_lt[1];
	mn_labelset_lh1[0] =	       	mn_labelset_lh1[1];
	mn_labelset_lh2[0] =	       	mn_labelset_lh2[1];

	////////////////////////// 20120906
	md_labelset_x1_pos[0] =	        md_labelset_x1_pos[1];
	md_labelset_x2_pos[0] =	        md_labelset_x2_pos[1];
	md_labelset_x3_pos[0] =	        md_labelset_x3_pos[1];
	md_labelset_x4_pos[0] =	        md_labelset_x4_pos[1];
	md_labelset_x5_pos[0] =	        md_labelset_x5_pos[1];
	md_labelset_x6_pos[0] =	        md_labelset_x6_pos[1];
	md_labelset_y1_pos[0] =	        md_labelset_y1_pos[1];
	md_labelset_y2_pos[0] =	        md_labelset_y2_pos[1];
	md_labelset_y3_pos[0] =	        md_labelset_y3_pos[1];
	md_labelset_y4_pos[0] =	        md_labelset_y4_pos[1];
	md_labelset_y5_pos[0] =	        md_labelset_y5_pos[1];
	md_labelset_y6_pos[0] =	        md_labelset_y6_pos[1];
	md_labelset_x1_width[0] =	    md_labelset_x1_width[1];
	md_labelset_x2_width[0] =	    md_labelset_x2_width[1];
	md_labelset_x3_width[0] =	    md_labelset_x3_width[1];
	md_labelset_x4_width[0] =	    md_labelset_x4_width[1];
	md_labelset_y1_height[0] =	    md_labelset_y1_height[1];
	md_labelset_y2_height[0] =	    md_labelset_y2_height[1];
	md_labelset_y3_height[0] =	    md_labelset_y3_height[1];
	md_labelset_y4_height[0] =	    md_labelset_y4_height[1];
	//////////////////////////


}

void CScreen_Basic::OnBasic_Label_Data_Recovery()
{
	mn_label_mode[1] =				mn_label_mode[0];				// With, With Out, No Tray
	mstr_label_name[1] =			mstr_label_name[0];					// 선택된 디바이스 종류 저장 변수	

	mn_labelset_sd[1] =	        	mn_labelset_sd[0];
	mn_labelset_lt[1] =	        	mn_labelset_lt[0];
	mn_labelset_lh1[1] =	       	mn_labelset_lh1[0];
	mn_labelset_lh2[1] =	       	mn_labelset_lh2[0];

	/////////////////////////// 20120906
	md_labelset_x1_pos[1] =	        md_labelset_x1_pos[0];
	md_labelset_x2_pos[1] =	        md_labelset_x2_pos[0];
	md_labelset_x3_pos[1] =	        md_labelset_x3_pos[0];
	md_labelset_x4_pos[1] =	        md_labelset_x4_pos[0];
	md_labelset_x5_pos[1] =	        md_labelset_x5_pos[0];
	md_labelset_x6_pos[1] =	        md_labelset_x6_pos[0];
	md_labelset_y1_pos[1] =	        md_labelset_y1_pos[0];
	md_labelset_y2_pos[1] =	        md_labelset_y2_pos[0];
	md_labelset_y3_pos[1] =	        md_labelset_y3_pos[0];
	md_labelset_y4_pos[1] =	        md_labelset_y4_pos[0];
	md_labelset_y5_pos[1] =	        md_labelset_y5_pos[0];
	md_labelset_y6_pos[1] =	        md_labelset_y6_pos[0];
	md_labelset_x1_width[1] =	    md_labelset_x1_width[0];
	md_labelset_x2_width[1] =	    md_labelset_x2_width[0];
	md_labelset_x3_width[1] =	    md_labelset_x3_width[0];
	md_labelset_x4_width[1] =	    md_labelset_x4_width[0];
	md_labelset_y1_height[1] =	    md_labelset_y1_height[0];
	md_labelset_y2_height[1] =	    md_labelset_y2_height[0];
	md_labelset_y3_height[1] =	    md_labelset_y3_height[0];
	md_labelset_y4_height[1] =	    md_labelset_y4_height[0];
	///////////////////////////


}

void CScreen_Basic::OnBasic_Label_Data_Set()
{
	
	mn_label_mode[1] =				st_basic.n_mode_label;				// With, With Out, No Tray
	mstr_label_name[1] =			st_basic.mstr_label_name;					// 선택된 디바이스 종류 저장 변수

	mn_labelset_sd[1] =	        	st_basic.mn_labelset_sd;
	mn_labelset_lt[1] =	        	st_basic.mn_labelset_lt;
	mn_labelset_lh1[1] =	        st_basic.mn_labelset_lh1;
	mn_labelset_lh2[1] =	        st_basic.mn_labelset_lh2;

	//////////////////////// 20120906
	md_labelset_x1_pos[1] =	        st_basic.md_labelset_x1_pos;
	md_labelset_x2_pos[1] =	        st_basic.md_labelset_x2_pos;
	md_labelset_x3_pos[1] =	        st_basic.md_labelset_x3_pos;
	md_labelset_x4_pos[1] =	        st_basic.md_labelset_x4_pos;
	md_labelset_x5_pos[1] =	        st_basic.md_labelset_x5_pos;
	md_labelset_x6_pos[1] =	        st_basic.md_labelset_x6_pos;
	md_labelset_y1_pos[1] =	        st_basic.md_labelset_y1_pos;
	md_labelset_y2_pos[1] =	        st_basic.md_labelset_y2_pos;
	md_labelset_y3_pos[1] =	        st_basic.md_labelset_y3_pos;
	md_labelset_y4_pos[1] =	        st_basic.md_labelset_y4_pos;
	md_labelset_y5_pos[1] =	        st_basic.md_labelset_y5_pos;
	md_labelset_y6_pos[1] =	        st_basic.md_labelset_y6_pos;
	md_labelset_x1_width[1] =	    st_basic.md_labelset_x1_width;
	md_labelset_x2_width[1] =	    st_basic.md_labelset_x2_width;
	md_labelset_x3_width[1] =	    st_basic.md_labelset_x3_width;
	md_labelset_x4_width[1] =	    st_basic.md_labelset_x4_width;
	md_labelset_y1_height[1] =	    st_basic.md_labelset_y1_height;
	md_labelset_y2_height[1] =	    st_basic.md_labelset_y2_height;
	md_labelset_y3_height[1] =	    st_basic.md_labelset_y3_height;
	md_labelset_y4_height[1] =	    st_basic.md_labelset_y4_height;
	////////////////////////

	
}

void CScreen_Basic::OnBasic_Label_DeviceData_Apply()
{
	int n_response = 0;  // 대화 상자 리턴 플래그
	int n_check;
	CString str_msg;
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	
	n_check = OnBasic_Label_Input_Data_Check();  // 입력 정보 정상 유무 검사 함수
	
	if (n_check == TRUE)  
	{
		str_msg = "[Test Label ] Was changed by " + mstr_label_name[1];
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

	mcls_basic.OnBasic_Label_Data_Save();
	mcls_basic.OnBasic_Label_Data_Load();
	// **********************************************************************
		
	// **************************************************************************
	// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다                        
	// **************************************************************************
	m_list_label_type.ResetContent();			// 리스트 박스의 모든 목록 삭제
// 	mp_device_image_list.DeleteImageList();		// 이미지 리스트에 설정된 정보 삭제
	
	//OnBasic_Create_ImageList();					// 이미지 리스트 생성 함수
	
	// Associate image list to list box
	//m_list_label_type.SetImageList(&mp_device_image_list);

	st_basic.mn_data_change_list = 0;	//20120608
	OnBasic_Label_Init_List(st_path.mstr_label_folder);	// 파일 폴더 초기화 함수
	
	OnBasic_Label_Focus_Set();					// 선택된 디바이스명 포커스 설정 함수
	// **************************************************************************

	st_basic.mstr_label_name =	mstr_label_name[1];
	mstr_label_name[0] =		mstr_label_name[1];
	st_basic.n_mode_label = mn_label_mode[1];
	mn_label_mode[0] =		mn_label_mode[1];
	
}

int CScreen_Basic::OnBasic_Label_Input_Data_Check()
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
	mstr_temp_label = mstr_label_name[1];
	if (mstr_temp_label.IsEmpty())  
	{
		return FALSE;
	}
	/* ************************************************************************** */
	
	n_pos = mstr_temp_label.Find(".");				// 확장자 검사
	if (n_pos == -1) 
		mstr_temp_label += _T(".TXT");
	else 
	{
		str_chk_ext = mstr_temp_label.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			return FALSE;
		}
	}
	
	mstr_label_name[1] = mstr_temp_label;			// 디바이스 파일명 설정
	/* ************************************************************************** */
	
	return TRUE;
}

void CScreen_Basic::OnDgtLabelsetSd() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_sd.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("SD LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("SD LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "20";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_sd.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_sd[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_sd.SetValue(mn_labelset_sd[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}
void CScreen_Basic::OnDgtLabelsetLt() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_labelset_lt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LT LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("LT LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "10";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_lt.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_labelset_lt[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_labelset_lt.SetValue(mn_labelset_lt[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}				
}


void CScreen_Basic::OnDgtLabelsetX1Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x1_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X1 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X1 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_x1_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x1_pos[1] = atof(mstr_temp);
		m_dgt_labelset_x1_pos.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetX2Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x2_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X2 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X2 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_x2_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x2_pos[1] = atof(mstr_temp);
		m_dgt_labelset_x2_pos.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetX3Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x3_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X3 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X3 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_x3_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x3_pos[1] = atof(mstr_temp);
		m_dgt_labelset_x3_pos.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}

void CScreen_Basic::OnDgtLabelsetX4Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x4_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X4 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X4 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_x4_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x4_pos[1] = atof(mstr_temp);
		m_dgt_labelset_x4_pos.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetX5Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x5_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X5 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X5 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_x5_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x5_pos[1] = atof(mstr_temp);
		m_dgt_labelset_x5_pos.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetX6Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x6_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("X6 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X6 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_x6_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{

		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x6_pos[1] = atof(mstr_temp);
		m_dgt_labelset_x6_pos.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetY1Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y1_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y1 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y1 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_y1_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y1_pos[1] = atof(mstr_temp);
		m_dgt_labelset_y1_pos.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetY2Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y2_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y2 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y2 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_y2_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y2_pos[1] = atof(mstr_temp);
		m_dgt_labelset_y2_pos.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetY3Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y3_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y3 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y3 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_y3_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y3_pos[1] = atof(mstr_temp);
		m_dgt_labelset_y3_pos.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetY4Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y4_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y4 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y4 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_y4_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y4_pos[1] = atof(mstr_temp);
		m_dgt_labelset_y4_pos.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetY5Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y5_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y5 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y5 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_y5_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y5_pos[1] = atof(mstr_temp);
		m_dgt_labelset_y5_pos.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetY6Pos() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y6_pos.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);
	
	st_msg.mstr_keypad_msg = _T("Y6 POS LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y6 POS LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_y6_pos.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y6_pos[1] = atof(mstr_temp);
		m_dgt_labelset_y6_pos.SetFloatVal((float)md_minute);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelsetX1Width() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x1_width.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	
	st_msg.mstr_keypad_msg = _T("X1 WIDTH LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X1 WIDTH LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_x1_width.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x1_width[1] = atof(mstr_temp);
		m_dgt_labelset_x1_width.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}					
}

void CScreen_Basic::OnDgtLabelsetY1Height() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y1_height.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	st_msg.mstr_keypad_msg = _T("Y1 HEIGHT LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y1 HEIGHT LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_y1_height.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y1_height[1] = atof(mstr_temp);
		m_dgt_labelset_y1_height.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}

void CScreen_Basic::OnDgtLabelsetX2Width() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x2_width.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	
	st_msg.mstr_keypad_msg = _T("X2 WIDTH LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X2 WIDTH LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_x2_width.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x2_width[1] = atof(mstr_temp);
		m_dgt_labelset_x2_width.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}					
}

void CScreen_Basic::OnDgtLabelsetY2Height() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y2_height.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	st_msg.mstr_keypad_msg = _T("Y2 HEIGHT LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y2 HEIGHT LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_y2_height.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y2_height[1] = atof(mstr_temp);
		m_dgt_labelset_y2_height.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}

void CScreen_Basic::OnDgtLabelsetX3Width() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x3_width.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	
	st_msg.mstr_keypad_msg = _T("X3 WIDTH LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X3 WIDTH LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_x3_width.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x3_width[1] = atof(mstr_temp);
		m_dgt_labelset_x3_width.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}					
}

void CScreen_Basic::OnDgtLabelsetY3Height() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y3_height.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	st_msg.mstr_keypad_msg = _T("Y3 HEIGHT LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y3 HEIGHT LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_y3_height.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y3_height[1] = atof(mstr_temp);
		m_dgt_labelset_y3_height.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}

void CScreen_Basic::OnDgtLabelsetX4Width() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_x4_width.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	
	st_msg.mstr_keypad_msg = _T("X4 WIDTH LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("X4 WIDTH LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_x4_width.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_x4_width[1] = atof(mstr_temp);
		m_dgt_labelset_x4_width.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}					
}

void CScreen_Basic::OnDgtLabelsetY4Height() //20120906
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double md_minute;
	
	CDialog_KeyPad pad_dlg;
	
	md_minute = (double)m_dgt_labelset_y4_height.GetFloatVal();
	mstr_temp.Format("%4.2f", md_minute);

	st_msg.mstr_keypad_msg = _T("Y4 HEIGHT LABEL SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Y4 HEIGHT LABEL SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "999.99";
	st_msg.mstr_pad_low_limit = "-999.99";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_labelset_y4_height.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		md_minute = atof(mstr_temp);
		md_labelset_y4_height[1] = atof(mstr_temp);
		m_dgt_labelset_y4_height.SetFloatVal((float)md_minute);

	}
	else if (mn_response == IDCANCEL)
	{
		
	}			
}


void CScreen_Basic::OnBasic_Label_Font_Set()
{
	//////////////////////////////////////////////////////// 20120531
	m_msg_labelset_sd.SetFont(mp_label_font);
	m_msg_labelset_sd.SetWindowText(_T("인쇄농도(SD)"));
	m_msg_labelset_sd.SetCenterText();
	m_msg_labelset_sd.SetColor(RGB(0,0,255));
	m_msg_labelset_sd.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_sd.SetTextColor(RGB(255,255,255));

	m_msg_labelset_lt.SetFont(mp_label_font);
	m_msg_labelset_lt.SetWindowText(_T("전체높이(LT)"));
	m_msg_labelset_lt.SetCenterText();
	m_msg_labelset_lt.SetColor(RGB(0,0,255));
	m_msg_labelset_lt.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_lt.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x1_pos.SetFont(mp_label_font);
	m_msg_labelset_x1_pos.SetWindowText(_T("1번 X값"));
	m_msg_labelset_x1_pos.SetCenterText();
	m_msg_labelset_x1_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_x1_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x1_pos.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x2_pos.SetFont(mp_label_font);
	m_msg_labelset_x2_pos.SetWindowText(_T("2번 X값"));
	m_msg_labelset_x2_pos.SetCenterText();
	m_msg_labelset_x2_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_x2_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x2_pos.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x3_pos.SetFont(mp_label_font);
	m_msg_labelset_x3_pos.SetWindowText(_T("3번 X값"));
	m_msg_labelset_x3_pos.SetCenterText();
	m_msg_labelset_x3_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_x3_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x3_pos.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x4_pos.SetFont(mp_label_font);
	m_msg_labelset_x4_pos.SetWindowText(_T("4번 X값"));
	m_msg_labelset_x4_pos.SetCenterText();
	m_msg_labelset_x4_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_x4_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x4_pos.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x5_pos.SetFont(mp_label_font);
	m_msg_labelset_x5_pos.SetWindowText(_T("5번 X값"));
	m_msg_labelset_x5_pos.SetCenterText();
	m_msg_labelset_x5_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_x5_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x5_pos.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x6_pos.SetFont(mp_label_font);
	m_msg_labelset_x6_pos.SetWindowText(_T("6번 X값"));
	m_msg_labelset_x6_pos.SetCenterText();
	m_msg_labelset_x6_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_x6_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x6_pos.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y1_pos.SetFont(mp_label_font);
	m_msg_labelset_y1_pos.SetWindowText(_T("1번 Y값"));
	m_msg_labelset_y1_pos.SetCenterText();
	m_msg_labelset_y1_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_y1_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y1_pos.SetTextColor(RGB(255,255,255));
	
	m_msg_labelset_y2_pos.SetFont(mp_label_font);
	m_msg_labelset_y2_pos.SetWindowText(_T("2번 Y값"));
	m_msg_labelset_y2_pos.SetCenterText();
	m_msg_labelset_y2_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_y2_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y2_pos.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y3_pos.SetFont(mp_label_font);
	m_msg_labelset_y3_pos.SetWindowText(_T("3번 Y값"));
	m_msg_labelset_y3_pos.SetCenterText();
	m_msg_labelset_y3_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_y3_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y3_pos.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y4_pos.SetFont(mp_label_font);
	m_msg_labelset_y4_pos.SetWindowText(_T("4번 Y값"));
	m_msg_labelset_y4_pos.SetCenterText();
	m_msg_labelset_y4_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_y4_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y4_pos.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y5_pos.SetFont(mp_label_font);
	m_msg_labelset_y5_pos.SetWindowText(_T("5번 Y값"));
	m_msg_labelset_y5_pos.SetCenterText();
	m_msg_labelset_y5_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_y5_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y5_pos.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y6_pos.SetFont(mp_label_font);
	m_msg_labelset_y6_pos.SetWindowText(_T("6번 Y값"));
	m_msg_labelset_y6_pos.SetCenterText();
	m_msg_labelset_y6_pos.SetColor(RGB(0,0,255));
	m_msg_labelset_y6_pos.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y6_pos.SetTextColor(RGB(255,255,255));
	

	m_msg_labelset_x1_width.SetFont(mp_label_font);
	m_msg_labelset_x1_width.SetWindowText(_T("1번 넓이"));
	m_msg_labelset_x1_width.SetCenterText();
	m_msg_labelset_x1_width.SetColor(RGB(0,0,255));
	m_msg_labelset_x1_width.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x1_width.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x2_width.SetFont(mp_label_font);
	m_msg_labelset_x2_width.SetWindowText(_T("2번 넓이"));
	m_msg_labelset_x2_width.SetCenterText();
	m_msg_labelset_x2_width.SetColor(RGB(0,0,255));
	m_msg_labelset_x2_width.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x2_width.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x3_width.SetFont(mp_label_font);
	m_msg_labelset_x3_width.SetWindowText(_T("3번 넓이"));
	m_msg_labelset_x3_width.SetCenterText();
	m_msg_labelset_x3_width.SetColor(RGB(0,0,255));
	m_msg_labelset_x3_width.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x3_width.SetTextColor(RGB(255,255,255));

	m_msg_labelset_x4_width.SetFont(mp_label_font);
	m_msg_labelset_x4_width.SetWindowText(_T("4번 넓이"));
	m_msg_labelset_x4_width.SetCenterText();
	m_msg_labelset_x4_width.SetColor(RGB(0,0,255));
	m_msg_labelset_x4_width.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_x4_width.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y1_height.SetFont(mp_label_font);
	m_msg_labelset_y1_height.SetWindowText(_T("1번 높이"));
	m_msg_labelset_y1_height.SetCenterText();
	m_msg_labelset_y1_height.SetColor(RGB(0,0,255));
	m_msg_labelset_y1_height.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y1_height.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y2_height.SetFont(mp_label_font);
	m_msg_labelset_y2_height.SetWindowText(_T("2번 높이"));
	m_msg_labelset_y2_height.SetCenterText();
	m_msg_labelset_y2_height.SetColor(RGB(0,0,255));
	m_msg_labelset_y2_height.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y2_height.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y3_height.SetFont(mp_label_font);
	m_msg_labelset_y3_height.SetWindowText(_T("3번 높이"));
	m_msg_labelset_y3_height.SetCenterText();
	m_msg_labelset_y3_height.SetColor(RGB(0,0,255));
	m_msg_labelset_y3_height.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y3_height.SetTextColor(RGB(255,255,255));

	m_msg_labelset_y4_height.SetFont(mp_label_font);
	m_msg_labelset_y4_height.SetWindowText(_T("4번 높이"));
	m_msg_labelset_y4_height.SetCenterText();
	m_msg_labelset_y4_height.SetColor(RGB(0,0,255));
	m_msg_labelset_y4_height.SetGradientColor(RGB(0,0,0));
	m_msg_labelset_y4_height.SetTextColor(RGB(255,255,255));

	///////////////////////////////////////////////////
}

void CScreen_Basic::OnBasic_Label_Data_Display()
{
	m_dgt_labelset_sd.SetValue(mn_labelset_sd[1]);
	m_dgt_labelset_lt.SetValue(mn_labelset_lt[1]);
	m_dgt_labelset_lh1.SetValue(mn_labelset_lh1[1]);
	m_dgt_labelset_lh2.SetValue(mn_labelset_lh2[1]);
	
	////////////////////////////////////////////////////////// 20120906
	m_dgt_labelset_x1_pos.SetFloatVal((float)md_labelset_x1_pos[1]);
	m_dgt_labelset_x2_pos.SetFloatVal((float)md_labelset_x2_pos[1]);
	m_dgt_labelset_x3_pos.SetFloatVal((float)md_labelset_x3_pos[1]);
	m_dgt_labelset_x4_pos.SetFloatVal((float)md_labelset_x4_pos[1]);
	m_dgt_labelset_x5_pos.SetFloatVal((float)md_labelset_x5_pos[1]);
	m_dgt_labelset_x6_pos.SetFloatVal((float)md_labelset_x6_pos[1]);;
	m_dgt_labelset_y1_pos.SetFloatVal((float)md_labelset_y1_pos[1]);
	m_dgt_labelset_y2_pos.SetFloatVal((float)md_labelset_y2_pos[1]);
	m_dgt_labelset_y3_pos.SetFloatVal((float)md_labelset_y3_pos[1]);
	m_dgt_labelset_y4_pos.SetFloatVal((float)md_labelset_y4_pos[1]);
	m_dgt_labelset_y5_pos.SetFloatVal((float)md_labelset_y5_pos[1]);
	m_dgt_labelset_y6_pos.SetFloatVal((float)md_labelset_y6_pos[1]);
	m_dgt_labelset_x1_width.SetFloatVal((float)md_labelset_x1_width[1]);
	m_dgt_labelset_x2_width.SetFloatVal((float)md_labelset_x2_width[1]);
	m_dgt_labelset_x3_width.SetFloatVal((float)md_labelset_x3_width[1]);
	m_dgt_labelset_x4_width.SetFloatVal((float)md_labelset_x4_width[1]);
	m_dgt_labelset_y1_height.SetFloatVal((float)md_labelset_y1_height[1]);
	m_dgt_labelset_y2_height.SetFloatVal((float)md_labelset_y2_height[1]);
	m_dgt_labelset_y3_height.SetFloatVal((float)md_labelset_y3_height[1]);
	m_dgt_labelset_y4_height.SetFloatVal((float)md_labelset_y4_height[1]);
	//////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////
}


void CScreen_Basic::OnDgtLabelPrintErrorCnt() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_Label_Print_Error_Cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Label Print Error Cnt 메세지 재전송 횟수");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Label Print Error count");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "10";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_Label_Print_Error_Cnt.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_Label_Print_Error_Cnt[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_Label_Print_Error_Cnt.SetValue(mn_Label_Print_Error_Cnt[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
	
}

void CScreen_Basic::OnDgtPickerGapModule() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double d_dgt_data;
	
	CDialog_KeyPad pad_dlg;
	
	d_dgt_data = (double)m_dgt_picker_gap_module.GetFloatVal();
	mstr_temp.Format("%4.2f", d_dgt_data);
	
	st_msg.mstr_keypad_msg = _T("MODULE PICKER GAP SETTING");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("MODULE PICKER GAP SETTING");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mn_dot_use = CTL_YES;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "0";
	
	CRect r;
	
	m_dgt_picker_gap_module.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		d_dgt_data = atof(mstr_temp);
		md_picker_gap_module[1] = atof(mstr_temp);
		m_dgt_picker_gap_module.SetFloatVal((float)d_dgt_data);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnDgtPickerGapHsLeft() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double d_dgt_data;
	
	CDialog_KeyPad pad_dlg;
	
	d_dgt_data = (double)m_dgt_picker_gap_hs_left.GetFloatVal();
	mstr_temp.Format("%4.2f", d_dgt_data);
	
	st_msg.mstr_keypad_msg = _T("HEATSINK LEFT PICKER GAP SETTING");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("HEATSINK LEFT PICKER GAP SETTING");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mn_dot_use = CTL_YES;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "0";
	
	CRect r;
	
	m_dgt_picker_gap_hs_left.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		d_dgt_data = atof(mstr_temp);
		md_picker_gap_hs_left[1] = atof(mstr_temp);
		m_dgt_picker_gap_hs_left.SetFloatVal((float)d_dgt_data);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
}

void CScreen_Basic::OnDgtPickerGapHsRight() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	double d_dgt_data;
	
	CDialog_KeyPad pad_dlg;
	
	d_dgt_data = (double)m_dgt_picker_gap_hs_right.GetFloatVal();
	mstr_temp.Format("%4.2f", d_dgt_data);
	
	st_msg.mstr_keypad_msg = _T("HEATSINK RIGHT PICKER GAP SETTING");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("HEATSINK RIGHT PICKER GAP SETTING");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mn_dot_use = CTL_YES;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "0";
	
	CRect r;
	
	m_dgt_picker_gap_hs_right.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		d_dgt_data = atof(mstr_temp);
		md_picker_gap_hs_right[1] = atof(mstr_temp);
		m_dgt_picker_gap_hs_right.SetFloatVal((float)d_dgt_data);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtLabelVisionPaper() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_label_vision_paper.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("LABEL<->VISION Paper SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("LABEL<->VISION Paper SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "99";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_label_vision_paper.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_label_vision_paper[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_label_vision_paper.SetValue(mn_label_vision_paper[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
	
}
////////////////////////////////////////////////////////// 20120707
void CScreen_Basic::OnDgtLotsetUnloader() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_lotset_unloader.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("UNLOADER LOT SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("UNLOADER LOT SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_lotset_unloader.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_lotset_unloader[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_lotset_unloader.SetValue(mn_lotset_unloader[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
	
}

void CScreen_Basic::OnDgtLotsetReject() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_lotset_reject.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("REJECT LOT SET (EA)");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("REJECT LOT SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_lotset_reject.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_lotset_reject[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_lotset_reject.SetValue(mn_lotset_reject[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
}

//////////////////////////////////////////////////////////

void CScreen_Basic::OnChkMDirection() 
{
	if(m_chk_m_direction.GetCheck() == 1)
	{
		mn_mode_m_direction[1] = 1;
	}
	else
	{
		mn_mode_m_direction[1] = 0;
	}

	OnBasic_direction_Change();	
}

void CScreen_Basic::OnChkHsDirection() 
{
	if(m_chk_hs_direction.GetCheck() == 1)
	{
		mn_mode_hs_direction[1] = 1;
	}
	else
	{
		mn_mode_hs_direction[1] = 0;
	}

	OnBasic_direction_Change();		
}


void CScreen_Basic::OnChkMBcr() 
{
	if(m_chk_bcr.GetCheck() == 1)
	{
		mn_mode_bcr[1] = 1;
	}
	else
	{
		mn_mode_bcr[1] = 0;
	}

	OnBasic_direction_Change();		
}

void CScreen_Basic::OnChkMRemoveClip() 
{
	if(m_chk_remove_clip.GetCheck() == 1)
	{
		mn_mode_remove_clip[1] = 1;
	}
	else
	{
		mn_mode_remove_clip[1] = 0;
	}

	OnBasic_direction_Change();		
}

void CScreen_Basic::OnBasic_direction_Change()
{
	if(mn_mode_m_direction[1] < 0 || mn_mode_m_direction[1] > 1)	mn_mode_m_direction[1] = 1;
	if(mn_mode_hs_direction[1] < 0 || mn_mode_hs_direction[1] > 1)	mn_mode_hs_direction[1] = 1;
	if(mn_mode_bcr[1] < 0 || mn_mode_bcr[1] > 1)	mn_mode_bcr[1] = 1;
	if(mn_mode_remove_clip[1] < 0 || mn_mode_remove_clip[1] > 1) mn_mode_remove_clip[1] = 1;

	if(mn_mode_clip_alarm[1] < 0 || mn_mode_clip_alarm[1] > 1) mn_mode_clip_alarm[1] = 1;
	if(mn_mode_use_cap_remove[1] < 0 || mn_mode_use_cap_remove[1] > 1) mn_mode_use_cap_remove[1] = 1;  // 20140220
	
	if(mn_mode_door_lock[1] < 0 || mn_mode_door_lock[1] > 1) mn_mode_door_lock[1] = 1;
	if(mn_mode_use_sorter_picker[1] < 0 || mn_mode_use_sorter_picker[1] > 1) mn_mode_use_sorter_picker[1] = 1;

	//2015.0129
	if(mn_mode_use_vis_cont_err[1] < 0 || mn_mode_use_vis_cont_err[1] > 1) mn_mode_use_vis_cont_err[1] = 1;


	m_chk_m_direction.SetCheck(mn_mode_m_direction[1]);
	m_chk_hs_direction.SetCheck(mn_mode_hs_direction[1]);
	m_chk_bcr.SetCheck(mn_mode_bcr[1]);
	m_chk_remove_clip.SetCheck(mn_mode_remove_clip[1]);
	m_chk_clip_alarm.SetCheck(mn_mode_clip_alarm[1]);
	m_chk_cap_remove.SetCheck(mn_mode_use_cap_remove[1]); // 20140220

	m_chk_door_lock.SetCheck(mn_mode_door_lock[1]);
	m_chk_use_sorter_picker.SetCheck(mn_mode_use_sorter_picker[1]);
	m_chk_use_vision_hs_continue.SetCheck(mn_mode_use_vis_cont_err[1]);


	if(mn_mode_m_direction[1] == 1)
	{
		SetDlgItemText(IDC_CHK_M_DIRECTION, "Module Direction Use");
	}
	else
	{
		SetDlgItemText(IDC_CHK_M_DIRECTION, "Module Direction Skip");
	}

	if(mn_mode_hs_direction[1] == 1)
	{
		SetDlgItemText(IDC_CHK_HS_DIRECTION, "Heat Sink Direction Use");
	}
	else
	{
		SetDlgItemText(IDC_CHK_HS_DIRECTION, "Heat Sink Direction Skip");
	}

	if(mn_mode_bcr[1] == 1)
	{
		SetDlgItemText(IDC_CHK_M_BCR, "Bcr Read Use");
	}
	else
	{
		SetDlgItemText(IDC_CHK_M_BCR, "Bcr Read Skip");
	}
	if(mn_mode_remove_clip[1] == 1)
	{
		SetDlgItemText(IDC_CHK_M_REMOVE_CLIP, "ClipRemove Use");
	}
	else
	{
		SetDlgItemText(IDC_CHK_M_REMOVE_CLIP, "ClipRemove Skip");
	}

	if(mn_mode_clip_alarm[1] == 1)
	{
		SetDlgItemText(IDC_CHK_CLIP_ALARM, "Clip removed Alarm Use");
	}
	else
	{
		SetDlgItemText(IDC_CHK_CLIP_ALARM, "Clip removed Alarm Skip");
	}

	if(mn_mode_use_cap_remove[1] == 1) // 20140220
	{
		SetDlgItemText(IDC_CHK_CAP_REMOVE, "Cap Remove Use");
	}
	else
	{
		SetDlgItemText(IDC_CHK_CAP_REMOVE, "Cap Remove Skip");
	}

	if(mn_mode_door_lock[1] == 1)
	{
		SetDlgItemText(IDC_CHK_DOOR_LOCK, "AMT Use");
	}
	else
	{
		SetDlgItemText(IDC_CHK_DOOR_LOCK, "AMT Skip");
	}

	if(mn_mode_use_sorter_picker[1] == 1)
	{
		SetDlgItemText(IDC_CHK_USE_SORTER_PICKER, "Sorter Picker Use");
	}
	else
	{
		SetDlgItemText(IDC_CHK_USE_SORTER_PICKER, "Sorter Picker Skip");
	}



}
///////////// 20120731
void CScreen_Basic::OnDgtDvcRepickCnt() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_Dvc_Repick_Cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("DVC Repick Cnt 메세지 재전송 횟수");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("DVC Repick Count");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "9";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_Dvc_Repick_Cnt.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_Dvc_Repick_Cnt[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_Dvc_Repick_Cnt.SetValue(mn_Dvc_Repick_Cnt[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
		
}


void CScreen_Basic::OnDgtLabelsetLh1() 
{
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

void CScreen_Basic::OnDgtLabelsetLh2() 
{
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



void CScreen_Basic::OnDgtClampClipBin() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_num;
	
	CDialog_KeyPad pad_dlg;
	
	mn_num = m_dgt_clamp_clip_bin.GetVal();
	mstr_temp = LPCTSTR(_itoa(mn_num, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("CLAMP CLIP BIN");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "10";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_clamp_clip_bin.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_clamp_clip_bin[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_clamp_clip_bin.SetVal(mn_clamp_clip_bin[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Basic::OnDgtClipBin() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_num;
	
	CDialog_KeyPad pad_dlg;
	
	mn_num = m_dgt_clip_bin.GetVal();
	mstr_temp = LPCTSTR(_itoa(mn_num, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("CLIP BIN");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_clip_bin.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_clip_bin[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_clip_bin.SetVal(mn_clip_bin[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
}

void CScreen_Basic::OnDgtCurrClampClipBin() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_num;
	
	CDialog_KeyPad pad_dlg;
	
	mn_num = m_dgt_curr_clamp_clip_bin.GetVal();
	mstr_temp = LPCTSTR(_itoa(mn_num, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("CLIP BIN");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "8";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_msg_clamp_clip_bin.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_curr_clamp_clip_bin[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_curr_clamp_clip_bin.SetVal(mn_curr_clamp_clip_bin[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}

void CScreen_Basic::OnDgtCurrClipBin() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_num;
	
	CDialog_KeyPad pad_dlg;
	
	mn_num = m_dgt_curr_clip_bin.GetVal();
	mstr_temp = LPCTSTR(_itoa(mn_num, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("CLIP BIN");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "60";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_msg_clip_bin.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_curr_clip_bin[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_curr_clip_bin.SetVal(mn_curr_clip_bin[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
}

void CScreen_Basic::OnChkDoorLock() 
{
	// TODO: Add your control notification handler code here

	CDialog_Pass_Check pass_dlg;
	int n_response;

	st_handler.mstr_pass_level = "Master";
	
	n_response = pass_dlg.DoModal();
	
	if (n_response != IDOK)
	{
		OnBasic_direction_Change();			
		return;
	}
	else
	{
		if(m_chk_door_lock.GetCheck() == 1)
		{
			mn_mode_door_lock[1] = 1;
		}
		else
		{
			mn_mode_door_lock[1] = 0;
		}
		
		OnBasic_direction_Change();			
	}
	
}

void CScreen_Basic::OnChkUseSorterPicker() 
{
	// TODO: Add your control notification handler code here
		
	if(m_chk_use_sorter_picker.GetCheck() == 1)
	{
		mn_mode_use_sorter_picker[1] = 1;
	}
	else
	{
		mn_mode_use_sorter_picker[1] = 0;
	}
	
	OnBasic_direction_Change();		
}

///////////////////////////////////////////////////////// 20121220 김광수
void CScreen_Basic::OnBasic_Create_Part_Number()
{
	int i;
	CString mstr_content; // 한 라인에 출력할 문자열 정보 저장 변수
	FILE *fp;
	CDialog_Message   messeage_dlg;
	int n_response;									// 대화 상자 리턴 플래그
	BOOL b_part_number_have;

	b_part_number_have = FALSE;
	mstr_content += mstr_new_part;
	mstr_content += "," + str_bcr_ml_name[1];
	
	for(i=0; i<100; i++)
	{
		if(st_basic.mstr_part_number[i] == mstr_content)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				b_part_number_have = TRUE;
			
				st_msg.str_fallacy_msg = _T("동일한 " + mstr_content + " Part_Number가 존재합니다.");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					st_msg.str_fallacy_msg = _T(mstr_content + "Exist in the same Part_Number");
				}
				
				n_response = messeage_dlg.DoModal();
				sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
				break;
			}
		}
	}

	if(b_part_number_have == FALSE)
	{
		if((fp = fopen(st_path.mstr_part,"a+")) == NULL)
		{
			CString strLog;
			strLog.Format("File Open Failus .. [%s]", st_path.mstr_part);
			Func.MsgLog(strLog);
			return;
		}
		fprintf(fp,"%s\n",mstr_content); // text 파일에 문자열 삽입
		fclose(fp);  // 파일을 종료한다.

		for(i=0; i<100; i++)
		{
			if(st_basic.mstr_part_number[i] == "")
			{
				st_basic.mstr_part_number[i] = mstr_content;		
				break;
			}
		}
		b_part_number_have = FALSE;
	}
}

int CScreen_Basic::OnBasic_New_Part_Check(CString str_part)
{
	/* ************************************************************************** */
    /* 입력된 디바이스명 설정한다                                                 */
    /* ************************************************************************** */
	str_part.MakeUpper();
	str_part.TrimLeft(' ');               
	str_part.TrimRight(' ');
	
	if(str_part.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_normal_msg = _T("[PART] A name input error occurrence.");
			sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	return TRUE;
}


void CScreen_Basic::OnBtnPartCreate() 
{
	// TODO: Add your control notification handler code here
	int n_response;									// 대화 상자 리턴 플래그

	CDialog_Message   messeage_dlg;
	CDialog_Keyboard  keyboard_dlg;

 	if(mn_response ==  FALSE)
	{
		st_msg.str_fallacy_msg = _T("ML의 List G1 ~ G8 중 하나를 선택해 주십시오.");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			st_msg.str_fallacy_msg = _T("please choose one of the ML's List G1 ~ G8");
		}

		n_response = messeage_dlg.DoModal();
	}
	else
	{
		(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
	
		n_response = keyboard_dlg.DoModal();
		
		if(n_response == IDOK)
		{
			mstr_new_part = st_msg.mstr_typing_msg;  // 키보드 상자 입력 정보 클래스 변수에 설정
			if (OnBasic_New_Part_Check(mstr_new_part) == FALSE)	return;
			OnBasic_Create_Part_Number();
			
			// ******************************************************************
			// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다
			// ******************************************************************
			m_list_part_type.ResetContent();		// 리스트 박스의 모든 목록 삭제
			
 			OnBasic_Part_List_TextSet();
			Func.PartNumber_Check();	
			mn_response = FALSE;

//			m_list_part_type.
			// ******************************************************************
		}
		else if (n_response == IDCANCEL)
		{
			mn_response = FALSE;
		}	 
	}	
}

void CScreen_Basic::OnBtnPartDelete() 
{
	// TODO: Add your control notification handler code here
	int n_response;				// 대화 상자 리턴 플래그
	CString str_part_file_name;
	CString str_selected_part;

	CDialog_Select  select_dlg;
	CDialog_Message msg_dlg;

	if (OnBasic_New_Part_Check(mstr_part_name[1]) == FALSE)  // 입력된 디바이스 이름 정상 유무 검사 함수
		return ;
	
	st_msg.mstr_confirm_msg = _T(mstr_part_name[1] + " Part_Number를 삭제 하시겠습니까??");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Are you sure you want to delete Part_Number?" + mstr_part_name[1]);
	}

	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnBasic_Part_List_Delete();
		// ******************************************************************
		// 폴더에 존재하는 파일 목록 리스트 박스에 재설정한다
		// ******************************************************************
		m_list_part_type.ResetContent();		// 리스트 박스의 모든 목록 삭제
				
		OnBasic_Part_List_TextSet();
		Func.PartNumber_Check();
		
	}
}

void CScreen_Basic::OnSelchangeListPartType() 
{
 	CString str_selected_part;						// 선택된 디바이스 이름 저장 변수
	int n_index = m_list_part_type.GetCurSel();		// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_part_type.GetText(n_index, str_selected_part);
		mstr_part_name[1] = str_selected_part;
		mn_part_mode[1] = n_index;
	}

}


void CScreen_Basic::OnBasic_Part_List_TextSet()
{
	int i = 0;
	CFile file;
	CString str_temp;

	m_list_part_type.ResetContent();
	file.Open(_T(st_path.mstr_part),CFile::modeRead);
	
	CArchive ar(&file, CArchive::load);
	while(ar.ReadString(str_temp))
	{
		m_list_part_type.AddString(str_temp);
		st_basic.mstr_part_number[i] = str_temp;
		i++;

	}		
	ar.Close();
	file.Close();




// 	CString str_temp;
// 
// 	file.Open(_T(st_path.mstr_part),CFile::modeRead);
// 	
// 	CArchive ar(&file, CArchive::load);
// 
// 	while(ar.ReadString(str_temp))
// 	{
// 		st_basic.mstr_part_number[i] = str_temp;
// 		i++;
// 	}		
// 	ar.Close();
// 	file.Close();
}

void CScreen_Basic::OnBasic_Part_List_Delete()
{ 
	CString str_select_part;
	CString str_Temp;
	CFile file;
	FILE *fp;
	int index, nTemp=0;

	index = m_list_part_type.GetCurSel(); 

	fp = fopen(st_path.mstr_part,"w+");
	for(int i=0; i<100; i++)
	{
		nTemp = m_list_part_type.GetTextLen(i);
		if(nTemp != -1)
		{
			if(index == i)
			{
				st_basic.mstr_part_number[i] = "";
				continue;
			}
			m_list_part_type.GetText(i, str_Temp);
			st_basic.mstr_part_number[i] = "";
			fprintf(fp,"%s\n",str_Temp);
		}
		else
		{
			break;
		}
	}
	fclose(fp); 
}

//////////////////////////////////////////////////////////// 20121231 김광수

void CScreen_Basic::OnBtnPartSelect() 
{	

	rs_232.n_serial_port[VISION_PORT]		= 2;
	rs_232.n_serial_data[VISION_PORT]		= 8;
	rs_232.n_serial_stop[VISION_PORT]		= 1;
	rs_232.n_serial_baudrate[VISION_PORT]	= 9600;
	rs_232.n_serial_parity[VISION_PORT]	= 0;

	::PostMessage(st_handler.hWnd, WM_SERIAL_PORT, YES, rs_232.n_serial_port[VISION_PORT]);

	if (st_serial.n_connect[2] == YES)
	{
		st_other.str_normal_msg =  _T("[Main] BCR ML Button Click");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}

		mn_vision_hs_enable = 9;
		m_b_stop_req = FALSE;
		st_interface.b_ml_select = TRUE;
		st_interface.b_vision_ml_select = TRUE;	//20120625
	//	OnBtn_BCR_HS_YIELD_Enable_False();
		l_bcr_send_wait[0] = GetCurrentTime();
		
		m_list_part_select.ResetContent(); //20120625
		
		
		//	::PostMessage(st_handler.hWnd, WM_WORK_COMMAND, MAIN_VISION_SEND, VISION_ML);
		Func.OnSet_Vision_send(VISION_ML);
		
		SetTimer(TM_MANUAL_BCR_HS_YIELD_TEST, 200, 0);		
	}
	else
	{
		st_other.str_normal_msg =  _T("[Manual] 통신포트를 연결하세요.");
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		if (st_handler.cwnd_list != NULL)
		{
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
		}
		
	}	
}

void CScreen_Basic::OnSelchangeListPartSelect() 
{
	CString str_selected_lbl;						// 선택된 디바이스 이름 저장 변수

	int n_index = m_list_part_select.GetCurSel();	// 선택된 리스트 위치 설정
	if (n_index != LB_ERR)							// 에러 검사
	{
		m_list_part_select.GetText(n_index, str_selected_lbl);
		str_bcr_ml_name[1] = str_selected_lbl;
		mn_bcr_ml_mode[1] = n_index;

		st_interface.str_bcr_ml_name = str_bcr_ml_name[1];
		st_interface.n_bcr_ml_mode = mn_bcr_ml_mode[1];
	}
	else 
	{
		OnBasic_ML_BCR_Focus_Set();		
		str_bcr_ml_name[1] = str_bcr_ml_name[0];
		mn_bcr_ml_mode[1] = mn_bcr_ml_mode[0];
	}
	mn_response = TRUE; 
}

int CScreen_Basic::Manual_Bcr_HS_Yield_Test()
{
	int	FuncRet = RET_PROCEED;

	if(m_b_stop_req == TRUE)
	{
		st_other.b_yield_ok = FALSE;
		st_other.b_ml_ok = FALSE;	//20120618 
		FuncRet = RET_ERROR;
		sprintf(st_msg.c_normal_msg, "BCR & HS Yield 테스트 에러!! - Stop 버튼 누름");
		if(st_handler.mn_language == LANGUAGE_ENGLISH) 
		{
			sprintf(st_msg.c_normal_msg, "BCR & HS Yield test error! - Stop button is pressed!!");
		}

		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
	}

	if (mn_vision_hs_enable == 9)	//bcr ml
	{
		l_bcr_send_wait[1] = GetCurrentTime();
		l_bcr_send_wait[2] = l_bcr_send_wait[1] - l_bcr_send_wait[0];
		if(l_bcr_send_wait[2] < 0)
		{
			l_bcr_send_wait[0] = GetCurrentTime();
		}
		if(l_bcr_send_wait[2] > 200)
		{
			if(	st_other.b_ml_ok == TRUE)
			{
				st_other.b_ml_ok = FALSE;
				FuncRet = RET_GOOD;
				sprintf(st_msg.c_normal_msg, "BCR ML 응답 완료!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "Complete response BCR ML!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			else
			{
				FuncRet = RET_ERROR;
				sprintf(st_msg.c_normal_msg, "BCR ML 응답 에러!!");
				if(st_handler.mn_language == LANGUAGE_ENGLISH) 
				{
					sprintf(st_msg.c_normal_msg, "BCR ML response error!!");
				}

				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0 , NORMAL_MSG);
			}
			st_interface.b_vision_ml_select = FALSE;
		}
	}
	return FuncRet;
}

void CScreen_Basic::OnBasic_ML_BCR_Focus_Set() // 20130102
{
	if(mn_bcr_ml_name == -1)
		return;
	m_list_part_select.SetCurSel(mn_bcr_ml_name);

}

//////////////////////////////////////////////////////////// 20130103

void CScreen_Basic::OnBasic_BCR_DISPLAY(int n_state)
{
	CString str_yield, str_yield2, str_yield3, str_yield4;
	CString str_ml, str_ml2, str_ml3, str_ml4;	// 20120618
	int i;

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

	case 2:
		for (i = 1; i < (st_handler.mn_vision_ml_cnt + 1); i++)
		{
			if (st_handler.str_vision_ml_yes[i] != "")
			{
				m_list_part_select.AddString(st_handler.str_vision_ml_yes[i]);				
			}
		}

		break;
	case 3:
		for (i = 1; i < (st_handler.mn_hs_ml_cnt + 1); i++)
		{
			if (st_handler.str_hs_ml_yes[i] != "")
			{
				m_list_part_select.AddString(st_handler.str_hs_ml_yes[i]);				
			}
		}
		
		break;
	}
}

LRESULT CScreen_Basic::OnBasic_Info_Display(WPARAM wParam,LPARAM lParam)   
{
	int mn_state = wParam;
	
	switch(mn_state)
	{	
	case BASIC_BCR_DISPLAY:
		OnBasic_BCR_DISPLAY(lParam);
		break;

	default:
		break;
	}

	return 0;
}

void CScreen_Basic::OnChkClipAlarm() 
{
	// TODO: Add your control notification handler code here
	if(m_chk_clip_alarm.GetCheck() == 1)
	{
		mn_mode_clip_alarm[1] = 1;
	}
	else
	{
		mn_mode_clip_alarm[1] = 0;
	}

	OnBasic_direction_Change();			
}

// 20140220
void CScreen_Basic::OnChkCapRemove() 
{
	// TODO: Add your control notification handler code here
	if(m_chk_cap_remove.GetCheck() == 1)
	{
		mn_mode_use_cap_remove[1] = 1;
	}
	else
	{
		mn_mode_use_cap_remove[1] = 0;
	}

	OnBasic_direction_Change();			
}

// 20140224
void CScreen_Basic::OnDgtLabelBinPrint() 
{
	// TODO: Add your control notification handler code here
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_minute;
	
	CDialog_KeyPad pad_dlg;
	
	mn_minute = m_dgt_label_bin_print.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_minute, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Label Bin Print SET");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("Label Bin Print SET");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "25";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_label_bin_print.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_label_bin_print[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_label_bin_print.SetValue(mn_label_bin_print[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
	
}

void CScreen_Basic::OnBtnAlarmFileRead() 
{
	// TODO: Add your control notification handler code here
	MyJamData.On_Alarm_Info_Load();
}

////2014.0818
void CScreen_Basic::OnBtnNode() 
{
	// TODO: Add your control notification handler code here
	Run_Rbt_Load_Module.HeadVal = st_basic.n_ld_node;
	Run_Rbt_Load_LeftHeat.HeadVal = st_basic.n_hs_front_node;
	Run_Rbt_Load_RightHeat.HeadVal = st_basic.n_hs_back_node;
	Run_Rbt_Load_Clip.HeadVal = st_basic.n_clip_node;
//	Run_Rbt_Sorter.HeadVal = st_basic.n_uld_node;
}
////

void CScreen_Basic::OnChkVisionHsContinue() 
{
	if(m_chk_use_vision_hs_continue.GetCheck() == 1)
	{
		mn_mode_use_vis_cont_err[1] = 1;
	}
	else
	{
		mn_mode_use_vis_cont_err[1] = 0;
	}

	OnBasic_direction_Change();				
}

void CScreen_Basic::OnDgtDvcRepickCnt2() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_error;
	
	CDialog_KeyPad pad_dlg;
	
	mn_error = m_dgt_Dvc_Hs_Vis_Cont_Err.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_error, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("HS Vision Continue error Cnt 메세지 재전송 횟수");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("HS Vision Continue error Count");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_Dvc_Hs_Vis_Cont_Err.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_vis_cont_err[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_Dvc_Hs_Vis_Cont_Err.SetValue(mn_vis_cont_err[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}	
}

void CScreen_Basic::OnDgtDvcRepickCnt3() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_error;
	
	CDialog_KeyPad pad_dlg;
	
	mn_error = m_dgt_Dvc_Hs_Vis_Tot_Err.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_error, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("HS Vision Total error Cnt 메세지 재전송 횟수");
	if (st_handler.mn_language == LANGUAGE_ENGLISH)	st_msg.mstr_keypad_msg = _T("HS Vision Total error Count");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "500";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_Dvc_Hs_Vis_Cont_Err.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_vis_tot_err[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_Dvc_Hs_Vis_Tot_Err.SetValue(mn_vis_tot_err[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}		
}
