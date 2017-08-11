// Dialog_Work_Maintenane.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_Work_Maintenane.h"
/////////////////////////////////20150515
#include "Keyboarddll.h"
#include "Ctlbd_Variable.h"
/////////////////////////////////

#include "Dialog_Pass_Change.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"

#include "Dialog_KeyPad.h"
#include "Dialog_Keyboard.h"

#include "io.h"  // ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� INCLUDE �ʿ�
#include "FileDialogST.h"
// ******************************************************************************
// ��ȭ ���� Ŭ���� �߰�                                                         
// ******************************************************************************
//#include "Dialog_Infor.h"

// ******************************************************************************

//#include "AMTLotManager.h"	//20150515


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Maintenane dialog


CDialog_Work_Maintenane::CDialog_Work_Maintenane(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Work_Maintenane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Work_Maintenane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Work_Maintenane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Work_Maintenane)
	DDX_Control(pDX, IDC_LIST1, m_lstboxtxt);
	DDX_Control(pDX, IDC_EDIT1, m_edit_mdl);
	DDX_Control(pDX, IDC_COMBO_MODEL, m_cbo_model);
	DDX_Control(pDX, IDC_GROUP_NEW_MODEL, m_group_newgmodel);
	DDX_Control(pDX, IDC_GROUP_REG_MODEL, m_group_regmodel);
	DDX_Control(pDX, IDC_CHK_HS_REAR, m_chk_hs_rear);
	DDX_Control(pDX, IDC_CHK_HS_FRONT, m_chk_hs_front);
	DDX_Control(pDX, IDC_GROUP_HEAT_SINK, m_group_hs);
	DDX_Control(pDX, IDC_BTN_PASSWORD_CHANGE_LEVLE2, m_btn_password_change_level2);
	DDX_Control(pDX, IDC_BTN_PASSWORD_CHANGE_LEVLE1, m_btn_password_change_level1);
	DDX_Control(pDX, IDC_BTN_DATA_BACKUP, m_btn_data_backup);
	DDX_Control(pDX, IDC_BTN_STEP_SAVE, m_btn_step_save);
	DDX_Control(pDX, IDC_GROUP_LAMP_CHANGE_TIME, m_group_lamp_change_time);
	DDX_Control(pDX, IDC_PAD_EQUIP_NO, m_pad_equip_no);
	DDX_Control(pDX, IDC_PAD_EQUIP_ID, m_pad_equip_id);
	DDX_Control(pDX, IDC_MSG_EQUIP_NO, m_msg_equip_no);
	DDX_Control(pDX, IDC_MSG_EQUIP_ID, m_msg_equip_id);
	DDX_Control(pDX, IDC_LAMP_STOP, m_lamp_stop);
	DDX_Control(pDX, IDC_LAMP_SELFCHECK, m_lamp_selfcheck);
	DDX_Control(pDX, IDC_LAMP_WARNING, m_lamp_warning);
	DDX_Control(pDX, IDC_LAMP_RUN, m_lamp_run);
	DDX_Control(pDX, IDC_LAMP_LOTEND, m_lamp_lotend);
	DDX_Control(pDX, IDC_LAMP_LOCK, m_lamp_lock);
	DDX_Control(pDX, IDC_LAMP_INITIAL, m_lamp_initial);
	DDX_Control(pDX, IDC_LAMP_ALARM, m_lamp_alarm);
	DDX_Control(pDX, IDC_GROUP_TOWERLAMP, m_group_towerlamp);
	DDX_Control(pDX, IDC_GROUP_PASSWORD_CHANGE, m_group_password_change);
	DDX_Control(pDX, IDC_GROUP_LAMP_WARNING, m_group_lamp_warning);
	DDX_Control(pDX, IDC_GROUP_LAMP_STOP, m_group_lamp_stop);
	DDX_Control(pDX, IDC_GROUP_LAMP_SELFCHECK, m_group_lamp_selfcheck);
	DDX_Control(pDX, IDC_GROUP_LAMP_RUN, m_group_lamp_run);
	DDX_Control(pDX, IDC_GROUP_LAMP_LOTEND, m_group_lamp_lotend);
	DDX_Control(pDX, IDC_GROUP_LAMP_LOCK, m_group_lamp_lock);
	DDX_Control(pDX, IDC_GROUP_LAMP_INITIAL, m_group_lamp_initial);
	DDX_Control(pDX, IDC_GROUP_LAMP_FLICKER_TIME, m_group_lamp_flicker_time);
	DDX_Control(pDX, IDC_GROUP_LAMP_ALARM, m_group_lamp_alarm);
	DDX_Control(pDX, IDC_GROUP_EQP_SET, m_group_eqp_set);
	DDX_Control(pDX, IDC_GROUP_DATA_BACKUP, m_group_data_backup);
	DDX_Control(pDX, IDC_GROUP_BUZZER, m_group_buzzer);
	DDX_Control(pDX, IDC_CHK_BUZZER, m_chk_buzzer);
	DDX_Control(pDX, IDC_BTN_MAINTENANCE_APPLY, m_btn_maintenance_apply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Work_Maintenane, CDialog)
	//{{AFX_MSG_MAP(CDialog_Work_Maintenane)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DGT_FLICKER_TIME, OnDgtFlickerTime)
	ON_BN_CLICKED(IDC_CHK_BUZZER, OnChkBuzzer)
	ON_BN_CLICKED(IDC_BTN_DATA_BACKUP, OnBtnDataBackup)
	ON_BN_CLICKED(IDC_PAD_EQUIP_NO, OnPadEquipNo)
	ON_BN_CLICKED(IDC_PAD_EQUIP_ID, OnPadEquipId)
	ON_BN_CLICKED(IDC_BTN_PASSWORD_CHANGE_LEVLE1, OnBtnPasswordChangeLevle1)
	ON_BN_CLICKED(IDC_BTN_PASSWORD_CHANGE_LEVLE2, OnBtnPasswordChangeLevle2)
	ON_BN_CLICKED(IDC_BTN_MAINTENANCE_APPLY, OnBtnMaintenanceApply)
	ON_EN_SETFOCUS(IDC_EDIT_EQUIP_ID, OnSetfocusEditEquipId)
	ON_EN_SETFOCUS(IDC_EDIT_EQUIP_NO, OnSetfocusEditEquipNo)
	ON_BN_CLICKED(IDC_DGT_CHANGE_TIME, OnDgtChangeTime)
	ON_BN_CLICKED(IDC_BTN_STEP_SAVE, OnBtnStepSave)
	ON_BN_CLICKED(IDC_CHK_HS_FRONT, OnChkHsFront)
	ON_BN_CLICKED(IDC_CHK_HS_REAR, OnChkHsRear)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, OnSelchangeComboModel)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_LAMP_RLAMP, ID_LAMP_RLAMP+SELFCHECK, OnRLampClick)
	ON_COMMAND_RANGE(ID_LAMP_YLAMP, ID_LAMP_YLAMP+SELFCHECK, OnYLampClick)
	ON_COMMAND_RANGE(ID_LAMP_GLAMP, ID_LAMP_GLAMP+SELFCHECK, OnGLampClick)
	ON_MESSAGE(WM_MAINTENANCE_APPLY, OnMaintenance_Info_Apply)				// Tower Lamp DATA�� APPLY ��Ű�� ���� ����� ���� �޽��� ����
		
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Maintenane message handlers


void CDialog_Work_Maintenane::DrawButton(int i)
{
	// **************************************************************************
    // Ÿ�� ���� ����� ��Ʈ�ѿ� ���� ���� ��´�                                
    // **************************************************************************
	CRect r;
	r = mr_area[i];  // ��Ʈ�� ���� ũ�� ����

	//Warning�ϰ�, Self Check�� �����.
	//if (i == dSELFCHECK || i == dINIT)	return;
	if (i == dINIT)	return;

	mn_pos_x[i][0] = r.left;
	mn_pos_x[i][1] = r.left + ((int)r.Width()-4);
	mn_pos_y[i][0] = r.top;
	mn_pos_y[i][1] = r.bottom-4;
	mn_y_diff[i] = (int) (mn_pos_y[i][1] - mn_pos_y[i][0])/7;
	mn_y_space[i] = (int) mn_y_diff[i]/6;
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� ���� ��� �̹��� ���� ������ ���� ���� �����Ѵ�                      
    // **************************************************************************
	CRgn b1, b2, e1, e2;
	COLORREF cr;
	int x1=0, x2, y1=0, y2, w=mn_pos_x[i][1] - mn_pos_x[i][0];
	CPoint p;
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� ���� ���� ���� �ձ� ���� ����� ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�      
	// -> CreateEllipticRgn() : �־��� �簢�� �������� Ÿ�� ������ �����Ѵ�      
	//  : ���������� ������ �����Ǿ����� ������ �ڵ� ���� ����                   
	//  : ������ �������� �ʾ����� NULL�� ����                                   
    // **************************************************************************
	p.x = mn_pos_x[i][0];
	p.y = mn_pos_y[i][1] - mn_y_space[i] * 4;
	x2 = w;	
	y2 = mn_y_space[i] * 4;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	mp_btn_base[i].Create("", WS_CHILD|WS_VISIBLE, p, e1, this, IDC_LAMP_BASE+i, 1, RGB(150, 159, 183), RGB(150, 159, 183), RGB(150, 159, 183), RGB(150, 159, 183));
	mp_btn_base[i].EnableWindow(FALSE);

	e1.DeleteObject();
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� ������ �ϴ� ���� �̹��� ��� ��ġ ����Ͽ� ��Ʈ�ѿ� ����Ѵ�         
	// -> CreateRectRgn() : �־��� ��ǥ�� �簢 ������ �׸���                     
	// -> OffsetRgn() : ������ �簢���� ��ǥ�� �̵��Ѵ�                          
	// -> CombineRgn() : �����ϴ� �� ���� �������� ���ļ� �� ��° ������ �����  
	//  : �� ��° ������ �ڵ��� �� �Լ��� ���̱� ���� �ݵ�� ��������߸�        
	//    ���ο� �������� ��ü�ȴ�                                               
	// -> GetRgnBox() : �־��� ������ ������ �ѷ��δ� ���� ���� ���簢�� �׸���  
    // **************************************************************************
	x2 = (int) w / 3;
	y2 = (int) mn_y_space[i] * 4 / 3;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	y2 = mn_y_diff[i] * 2 - mn_y_space[i] * 2;
	b1.CreateRectRgn(x1, y1, x2, y2);

	x2 = w;
	y2 = mn_y_space[i] * 4;
	e2.CreateEllipticRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, mn_y_diff[i]*2 - mn_y_space[i]*2 - (int)mn_y_space[i]*2/3);
	e2.OffsetRgn(-(int)w/3, -mn_y_space[i]);
	b1.CombineRgn(&e1, &b1, RGN_OR);
	b1.CombineRgn(&b1, &e2, RGN_DIFF);

	CRect rr;
	b1.GetRgnBox(&rr);

	p.y = rr.top;
	p.x = mn_pos_x[i][0] + (int) w/3;
	p.y += (mn_pos_y[i][1] - mn_y_diff[i] * 2);
	mp_btn_pole[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_POLE+i, 1, RGB(203, 185, 156), RGB(203, 185, 156), RGB(203, 185, 156), RGB(203, 185, 156));
	mp_btn_pole[i].EnableWindow(FALSE);

	e1.DeleteObject();
	e2.DeleteObject();
	b1.DeleteObject();
	// **************************************************************************

	// **************************************************************************
    // GREEN ���� �ؿ� �����ϴ� �ö�ƽ ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�         
    // **************************************************************************
	x2 = w;
	y2 = mn_y_diff[i];
	b1.CreateRectRgn(x1, y1, x2, y2);

	y2 = mn_y_space[i] * 4;
	e1.CreateEllipticRgn(x1, y1, x2, y2);

	y2 = mn_y_space[i] * 4;
	e2.CreateEllipticRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, -mn_y_space[i]*2);
	b1.CombineRgn(&b1, &e1, RGN_DIFF);
	e2.OffsetRgn(0, mn_y_diff[i] - mn_y_space[i]*2);
	b1.CombineRgn(&b1, &e2, RGN_OR);

	p.x = mn_pos_x[i][0];
	p.y = mn_pos_y[i][1] - mn_y_diff[i] * 3 + mn_y_space[i];
	mp_btn_lower[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_LOWER+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	mp_btn_lower[i].EnableWindow(FALSE);

	e1.OffsetRgn(0, mn_y_space[i]*2);
	e2.OffsetRgn(0, -(mn_y_diff[i] - mn_y_space[i]*2));

	b1.DeleteObject();
	// **************************************************************************

	// **************************************************************************
    // GREEN ���� �ٷ� �ؿ� �����ϴ� �׵θ� ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�      
    // **************************************************************************
	y2 = mn_y_space[i];
	b1.CreateRectRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, -mn_y_space[i]);
	b1.CombineRgn(&b1, &e1, RGN_OR);
	e2.OffsetRgn(0, -mn_y_space[i]*2);
	b1.CombineRgn(&b1, &e2, RGN_DIFF);

	p.y -= mn_y_space[i];
	mp_btn_step1[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP1+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	mp_btn_step1[i].EnableWindow(FALSE);
	e1.OffsetRgn(0, mn_y_space[i]);
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� GREEN ���� ��� ��ġ ����Ͽ� ��Ʈ�ѿ� ����Ѵ�                      
    // **************************************************************************
	y2 = mn_y_diff[i] - mn_y_space[i];
	b2.CreateRectRgn(x1, y1, x2, y2);

	e1.OffsetRgn(0, mn_y_diff[i] - mn_y_space[i]*3);
	b2.CombineRgn(&b2, &e1, RGN_OR);
	b2.CombineRgn(&b2, &e2, RGN_DIFF);

	CString title;  // ���� ���� ���� ����

	p.y -= (mn_y_diff[i] - mn_y_space[i]);
	if (mn_g_lamp[1][i]==0) 
	{
		cr = G_OFF;
		title = "OFF";
	}
	else if (mn_g_lamp[1][i]==1)
	{
		cr = G_ON;
		title = "ON";
	}
	else if (mn_g_lamp[1][i]==2)
	{
		cr = G_OFF;
		title = "Flicker";
	}

	mp_btn_g_lamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_GLAMP+i, 1, cr, G_ON, cr, cr);
	e1.OffsetRgn(0, -(mn_y_diff[i] - mn_y_space[i]*3));
	// **************************************************************************

	// **************************************************************************
    // GREEN ������ YELLOW ���� ������ �׵θ� ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�    
    // **************************************************************************
	p.y -= mn_y_space[i];

	mp_btn_step2[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP2+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	mp_btn_step2[i].EnableWindow(FALSE);
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� YELLOW ���� ��� ��ġ ����Ͽ� ��Ʈ�ѿ� ����Ѵ�                     
    // **************************************************************************
	p.y -= (mn_y_diff[i] - mn_y_space[i]);
	if (mn_y_lamp[1][i]==0) 
	{
		cr = Y_OFF;
		title = "OFF";
	}
	else if (mn_y_lamp[1][i]==1)
	{
		cr = Y_ON;
		title = "ON";
	}
	else if (mn_y_lamp[1][i]==2)
	{
		cr = Y_OFF;
		title = "Flicker";
	}

	mp_btn_y_lamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_YLAMP+i, 1, cr, Y_ON, cr, cr);
	// **************************************************************************

	// **************************************************************************
    // YELLOW ������ RED ���� ������ �׵θ� ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�      
    // **************************************************************************
	p.y -= mn_y_space[i];

	 mp_btn_step3[i].Create("", WS_CHILD|WS_VISIBLE, p, b1, this, IDC_LAMP_SEP3+i, 1, RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
	 mp_btn_step3[i].EnableWindow(FALSE);
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� RED ���� ��� ��ġ ����Ͽ� ��Ʈ�ѿ� ����Ѵ�                        
    // **************************************************************************
	p.y -= (mn_y_diff[i] - mn_y_space[i]);
	if (mn_r_lamp[1][i]==0) 
	{
		cr = R_OFF;
		title = "OFF";
	}
	else if (mn_r_lamp[1][i]==1)
	{
		cr = R_ON;
		title = "ON";
	}
	else if (mn_r_lamp[1][i]==2)
	{
		cr = R_OFF;
		title = "Flicker";
	}

	mp_btn_r_lamp[i].Create(title, WS_CHILD|WS_VISIBLE, p, b2, this, ID_LAMP_RLAMP+i, 1, cr, R_ON, cr, cr);
	// **************************************************************************

	// **************************************************************************
    // RED ���� ���� �����ϴ� �ö�ƽ ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�           
    // **************************************************************************
	p.y -= (mn_y_diff[i]-mn_y_space[i]);

	mp_btn_upper[i].Create("", WS_CHILD|WS_VISIBLE, p, b2, this, IDC_LAMP_UPPER+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	mp_btn_upper[i].EnableWindow(FALSE);
	// **************************************************************************

	// **************************************************************************
    // Ÿ�� ���� �� �� �ձ� �ö�ƽ ���� ����Ͽ� ��Ʈ�ѿ� ����Ѵ�             
    // **************************************************************************
	p.y -= mn_y_space[i]*2;

	mp_btn_roof[i].Create("", WS_CHILD|WS_VISIBLE, p, e1, this, IDC_LAMP_ROOF+i, 1, RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217), RGB(217, 217, 217));
	mp_btn_roof[i].EnableWindow(FALSE);
	// **************************************************************************
}


void CDialog_Work_Maintenane::OnMaintenance_GroupBox_Set()
{
	CSxLogFont maintenance_font(15,FW_SEMIBOLD,false,"MS Sans Serif");

// 	m_group_towerlamp.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_towerlamp.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_towerlamp.SetFont(maintenance_font);
	m_group_towerlamp.SetCatptionTextColor(RGB(145,25,0));
	m_group_towerlamp.SetFontBold(TRUE);

// 	m_group_lamp_run.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_lamp_run.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_lamp_run.SetFont(maintenance_font);
	m_group_lamp_run.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_run.SetFontBold(TRUE);

// 	m_group_lamp_stop.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_lamp_stop.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_lamp_stop.SetFont(maintenance_font);
	m_group_lamp_stop.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_stop.SetFontBold(TRUE);

// 	m_group_lamp_alarm.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_lamp_alarm.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_lamp_alarm.SetFont(maintenance_font);
	m_group_lamp_alarm.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_alarm.SetFontBold(TRUE);

// 	m_group_lamp_lock.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_lamp_lock.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_lamp_lock.SetFont(maintenance_font);
	m_group_lamp_lock.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_lock.SetFontBold(TRUE);

// 	m_group_lamp_lotend.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_lamp_lotend.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_lamp_lotend.SetFont(maintenance_font);
	m_group_lamp_lotend.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_lotend.SetFontBold(TRUE);

// 	m_group_lamp_initial.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_lamp_initial.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_lamp_initial.SetFont(maintenance_font);
	m_group_lamp_initial.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_initial.SetFontBold(TRUE);
	m_group_lamp_initial.ShowWindow(FALSE);

// 	m_group_lamp_warning.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_lamp_warning.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_lamp_warning.SetFont(maintenance_font);
	m_group_lamp_warning.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_warning.SetFontBold(TRUE);

// 	m_group_lamp_selfcheck.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_lamp_selfcheck.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_lamp_selfcheck.SetFont(maintenance_font);
	m_group_lamp_selfcheck.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_selfcheck.SetFontBold(TRUE);
// 	m_group_lamp_selfcheck.ShowWindow(FALSE);

// 	m_group_lamp_flicker_time.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_lamp_flicker_time.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_lamp_flicker_time.SetFont(maintenance_font);
	m_group_lamp_flicker_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_flicker_time.SetFontBold(TRUE);

// 	m_group_buzzer.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_buzzer.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_buzzer.SetFont(maintenance_font);
	m_group_buzzer.SetCatptionTextColor(RGB(145,25,0));
	m_group_buzzer.SetFontBold(TRUE);

// 	m_group_password_change.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_password_change.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_password_change.SetFont(maintenance_font);
	m_group_password_change.SetCatptionTextColor(RGB(145,25,0));
	m_group_password_change.SetFontBold(TRUE);

// 	m_group_eqp_set.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_eqp_set.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_eqp_set.SetFont(maintenance_font);
	m_group_eqp_set.SetCatptionTextColor(RGB(145,25,0));
	m_group_eqp_set.SetFontBold(TRUE);

// 	m_group_data_backup.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
// 	m_group_data_backup.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_data_backup.SetFont(maintenance_font);
	m_group_data_backup.SetCatptionTextColor(RGB(145,25,0));
	m_group_data_backup.SetFontBold(TRUE);

	m_group_lamp_change_time.SetFont(maintenance_font);
	m_group_lamp_change_time.SetCatptionTextColor(RGB(145,25,0));
	m_group_lamp_change_time.SetFontBold(TRUE);



	//2016.0516
	m_group_hs.SetFont(maintenance_font);
	m_group_hs.SetCatptionTextColor(RGB(145,25,0));
	m_group_hs.SetFontBold(TRUE);
}


BOOL CDialog_Work_Maintenane::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// **************************************************************************
	// ��Ʈ�ѿ� ������ ��Ʈ �����Ѵ�                                             
	// **************************************************************************
	mp_maintenance_font = NULL;
	mp_maintenance_font = new CFont;
	mp_maintenance_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
	OnMaintenance_GroupBox_Set();
	OnMaintenance_Button_Set();
	OnMaintenance_EditBox_Set();
	OnMaintenance_Label_Set();
	OnMaintenance_Digital_Count_Set();

	OnMaintenance_Data_Set();
	mb_first = TRUE;
	OnMaintenance_Data_BackUp();

	OnMaintenance_Data_Display();

	OnMaintenance_Buzzer_Change();
	OnMaintenance_Hs_Change();//2016.0516


	RegModel();

	st_handler.cwnd_maintenance = this;  // Tower Lamp ȭ�鿡 ���� �ڵ� ���� ���� 
	return TRUE;

}

void CDialog_Work_Maintenane::OnDestroy() 
{
	CDialog::OnDestroy();	

	int mn_response;  // ��ȭ ���ڿ� ���� ���� �� ���� ����
	int Ret;

	CDialog_Select select_dlg;

	//���α׷��� ���������鼭 Save ���� �ʰ�, Form�� Destroy�Ǵ°��̶�� Setting�� �����Ѵٴ� ���̴�.
	//�񱳵� ���� ����.
	if (st_handler.b_program_exit == false)
	{
		Ret = OnMaintenance_Data_Comparison();

		if (Ret == RET_ERROR)
		{
			st_msg.mstr_confirm_msg = _T("Changed Data! Apply?");
			mn_response = select_dlg.DoModal();
			
			if (mn_response == IDOK)
			{
				OnMaintenance_Data_Apply();

				OnMaintenance_LogFile_Create();

				OnMaintenance_ListBar_Info_Send();
			}
			else if (mn_response == IDCANCEL)
			{
				OnMaintenance_Data_Recovery();

				OnMaintenance_Data_Display();

				OnMaintenance_Data_Apply();

				OnMaintenance_ListBar_Info_Send();
			}	
		}
	}

	// **************************************************************************
	// ������ ��Ʈ ���� �����Ѵ�                                                 
	// **************************************************************************
	delete mp_maintenance_font;
	mp_maintenance_font = NULL;
	// **************************************************************************

	st_handler.cwnd_maintenance = NULL;	// Maintenance ȭ�鿡 ���� �ڵ� ���� ���� ���� �ʱ�ȭ 

}

void CDialog_Work_Maintenane::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (mb_first)          // Ÿ�� ���� ó�� ���� ���� �˻�
	{
		mb_first = FALSE;  // Ÿ�� ���� ó�� ���� ���� �÷��� ����

		// **********************************************************************
		// Ÿ�� ���� ����� ��Ʈ�� ���� ũ�� ��´�                              
		// -> Ÿ�� ���� ��� ���� ���� �Ŀ��� �ش� ��Ʈ�� ������ �ʰ� �����     
		// -> Ÿ�� ���� ���������� ����ϴ� ���� ����̱� �����̴�               
		// -> GetWindowRect() : �����츦 �ѷ��δ� �簢���� ũ�⸦ ����           
		//    lprc�� ����Ű�� RECT ����ü�� �� ���� �����Ѵ�                     
		//  : ����� ��ǥ�� �ȼ� �����̸� ȭ���� ���� ����� �������� ����Ѵ�   
		//  : ���� �������� ũ��� ��ġ�� ȭ�� ��� ��ǥ�� �˰� ���� �� ����Ѵ� 
		// **********************************************************************
		CRect r;
		GetWindowRect(&r);

		m_lamp_run.GetWindowRect(&mr_area[RUN]);
		m_lamp_stop.GetWindowRect(&mr_area[STOP]);
		m_lamp_alarm.GetWindowRect(&mr_area[ALARM]);
		m_lamp_lock.GetWindowRect(&mr_area[LOCK]);
		m_lamp_lotend.GetWindowRect(&mr_area[LOTEND]);
		m_lamp_initial.GetWindowRect(&mr_area[INIT]);
		m_lamp_warning.GetWindowRect(&mr_area[WARNING]);
		m_lamp_selfcheck.GetWindowRect(&mr_area[SELFCHECK]);

		for(int i=0; i<=SELFCHECK; i++)
		{
			mr_area[i].left -= r.left;
			mr_area[i].right -= r.left;
			mr_area[i].top -= r.top;
			mr_area[i].bottom -= r.top;
		}

		m_lamp_run.ShowWindow(SW_HIDE);
		m_lamp_stop.ShowWindow(SW_HIDE);
		m_lamp_alarm.ShowWindow(SW_HIDE);
		m_lamp_lock.ShowWindow(SW_HIDE);
		m_lamp_lotend.ShowWindow(SW_HIDE);
		m_lamp_initial.ShowWindow(SW_HIDE);
		m_lamp_warning.ShowWindow(SW_HIDE);
		m_lamp_selfcheck.ShowWindow(SW_HIDE);
		// **********************************************************************

		for(int j=0; j<=SELFCHECK; j++)
			DrawButton(j);							// Ÿ�� ���� ��ư ���� �Լ�

		for(int k=0; k<=SELFCHECK; k++)
		{
			OnMaintenance_Red_State_Change(k);		// RED Ÿ�� ���� ��ư ���� ��ȯ �Լ�
			OnMaintenance_Yellow_State_Change(k);	// YELLOW Ÿ�� ���� ��ư ���� ��ȯ �Լ�
			OnMaintenance_Green_State_Change(k);	// GREEN Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		}
	}
}



void CDialog_Work_Maintenane::OnMaintenance_Red_State_Change(int idx)
{
	//Wanring�ϰ�, Self Check�� �����.
//	if (idx == dSELFCHECK || idx == dINIT)	return;
	if (idx == dINIT)	return;

	int mn_red = mn_r_lamp[1][idx];  // RED ���� ����

	switch(mn_red)
	{
	case 0 :  // OFF
		mp_btn_r_lamp[idx].SetWindowText("OFF");
		mp_btn_r_lamp[idx].ChangeColor(R_OFF, R_ON, R_OFF, R_OFF);
		break;
	case 1 :  // ON
		mp_btn_r_lamp[idx].SetWindowText("ON");
		mp_btn_r_lamp[idx].ChangeColor(R_ON, R_ON, R_ON, R_ON);
		break;
	case 2 :  // FLICKER
		mp_btn_r_lamp[idx].SetWindowText("FLICK");
		mp_btn_r_lamp[idx].ChangeColor(FLICK, R_ON, FLICK, R_ON);
		break;
	default :
		break;
	}
}

void CDialog_Work_Maintenane::OnMaintenance_Yellow_State_Change(int idx)
{
	//Wanring�ϰ�, Self Check�� �����.
//	if (idx == dSELFCHECK || idx == dINIT)	return;
	if (idx == dINIT)	return;

	int mn_yellow = mn_y_lamp[1][idx];  // YELLOW ���� ����

	switch(mn_yellow)
	{
	case 0 :  // OFF
		mp_btn_y_lamp[idx].SetWindowText("OFF");
		mp_btn_y_lamp[idx].ChangeColor(Y_OFF, Y_ON, Y_OFF, Y_OFF);
		break;
	case 1 :  // ON
		mp_btn_y_lamp[idx].SetWindowText("ON");
		mp_btn_y_lamp[idx].ChangeColor(Y_ON, Y_ON, Y_ON, Y_ON);
		break;
	case 2 :  // FLICKER
		mp_btn_y_lamp[idx].SetWindowText("FLICK");
		mp_btn_y_lamp[idx].ChangeColor(FLICK, Y_ON, FLICK, Y_OFF);
		break;
	default :
		break;
	}
}

void CDialog_Work_Maintenane::OnMaintenance_Green_State_Change(int idx)
{
	//Wanring�ϰ�, Self Check�� �����.
//	if (idx == dSELFCHECK || idx == dINIT)	return;
	if (idx == dINIT)	return;

	int mn_green = mn_g_lamp[1][idx];  // GREEN ���� ����

	switch(mn_green)
	{
	case 0 :  // OFF
		mp_btn_g_lamp[idx].SetWindowText("OFF");
		mp_btn_g_lamp[idx].ChangeColor(G_OFF, G_ON, G_OFF, G_OFF);
		break;
	case 1 :  // ON
		mp_btn_g_lamp[idx].SetWindowText("ON");
		mp_btn_g_lamp[idx].ChangeColor(G_ON, G_ON, G_ON, G_ON);
		break;
	case 2 :  // FLICKER
		mp_btn_g_lamp[idx].SetWindowText("FLICK");
		mp_btn_g_lamp[idx].ChangeColor(FLICK, G_ON, FLICK, G_OFF);
		break;
	default :
		break;
	}
}

void CDialog_Work_Maintenane::OnMaintenance_Data_Set()
{
	int i;
	
	for (i = 0; i <= SELFCHECK; i++)
	{
		mn_r_lamp[1][i] = st_lamp.mn_lamp_r[i];
		mn_y_lamp[1][i] = st_lamp.mn_lamp_y[i];
		mn_g_lamp[1][i] = st_lamp.mn_lamp_g[i];
	}

	mn_buzzer_mode[1] = st_lamp.mn_buzzer_mode;
	mn_front_mode[1] = st_lamp.mn_front_mode;
	mn_rear_mode[1] = st_lamp.mn_rear_mode;

	mn_lamp_flicker_time[1] = st_lamp.mn_lamp_wait_time;
	mn_lamp_change_time[1] = st_lamp.mn_lamp_change_time;

	mstr_equip_no[1] = st_lamp.mstr_equip_no;
	mstr_equip_id[1] = st_lamp.mstr_equip_id;


}

void CDialog_Work_Maintenane::OnMaintenance_Button_Set()
{
	short	shBtnColor = 30;

	// *************************************************************
	//  CHECK BUTTON		                                        
	// *************************************************************
	m_chk_buzzer.SetFont(mp_maintenance_font);
	m_chk_buzzer.SetIcon(IDI_SOUND_ON, IDI_SOUND_OFF);
	m_chk_buzzer.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_buzzer.SetButtonColor(1);

	m_chk_hs_front.SetFont(mp_maintenance_font);
	m_chk_hs_front.SetIcon(IDI_SOUND_ON, IDI_SOUND_OFF);
	m_chk_hs_front.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_hs_front.SetButtonColor(1);


	m_chk_hs_rear.SetFont(mp_maintenance_font);
	m_chk_hs_rear.SetIcon(IDI_SOUND_ON, IDI_SOUND_OFF);
	m_chk_hs_rear.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_hs_rear.SetButtonColor(1);

	// *************************************************************
	//  PAD BUTTON		                                            
	// *************************************************************
	m_pad_equip_id.SetIcon(IDI_KEYBOARD);
	m_pad_equip_id.DrawBorder(FALSE);

	m_pad_equip_no.SetIcon(IDI_KEYBOARD);
	m_pad_equip_no.DrawBorder(FALSE);
}

void CDialog_Work_Maintenane::OnMaintenance_Buzzer_Change()
{
	if (mn_buzzer_mode[1] < 0 || mn_buzzer_mode[1] > 1)	mn_buzzer_mode[1] = TRUE;

	m_chk_buzzer.SetCheck(mn_buzzer_mode[1]);
	
	if (mn_buzzer_mode[1] == TRUE)	SetDlgItemText(IDC_CHK_BUZZER, "Buzzer On");
	else							SetDlgItemText(IDC_CHK_BUZZER, "Buzzer Off");
}

void CDialog_Work_Maintenane::OnMaintenance_Hs_Change()
{
	if (mn_front_mode[1] < 0 || mn_front_mode[1] > 1)	mn_front_mode[1] = TRUE;
	if (mn_rear_mode[1] < 0 || mn_rear_mode[1] > 1)		mn_rear_mode[1] = TRUE;
	
	m_chk_hs_front.SetCheck(mn_front_mode[1]);
	m_chk_hs_rear.SetCheck(mn_rear_mode[1]);
	
	if (mn_front_mode[1] == TRUE)	SetDlgItemText(IDC_CHK_HS_FRONT, "Front On");
	else							SetDlgItemText(IDC_CHK_HS_FRONT, "Front Off");

	if (mn_rear_mode[1] == TRUE)	SetDlgItemText(IDC_CHK_HS_REAR, "Back On");
	else							SetDlgItemText(IDC_CHK_HS_REAR, "Back Off");
}


void CDialog_Work_Maintenane::OnMaintenance_EditBox_Set()
{	
	m_edit_equip_id.SubclassDlgItem(IDC_EDIT_EQUIP_ID, this);
	m_edit_equip_id.bkColor(RGB(50, 100, 150));
	m_edit_equip_id.textColor(RGB(255, 255,255));
	m_edit_equip_id.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

	m_edit_equip_no.SubclassDlgItem(IDC_EDIT_EQUIP_NO, this);
	m_edit_equip_no.bkColor(RGB(50, 100, 150));
	m_edit_equip_no.textColor(RGB(255, 255,255));
	m_edit_equip_no.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
}

void CDialog_Work_Maintenane::OnMaintenance_Data_Apply()
{
	int i;

	for(i = 0; i <= SELFCHECK; i++)
	{
		// **********************************************************************
		// RED Ÿ�� ���� ���� ���� ������ �����Ѵ�                               
		// **********************************************************************
		if (mn_r_lamp[1][i]<0 || mn_r_lamp[1][i]>2) 
		{
			mn_r_lamp[1][i] = 0;
			OnMaintenance_Red_State_Change(i);     // RED Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		}
		st_lamp.mn_lamp_r[i] = mn_r_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// YELLOW Ÿ�� ���� ���� ���� ������ �����Ѵ�                            
		// **********************************************************************
		if (mn_y_lamp[1][i]<0 || mn_y_lamp[1][i]>2) 
		{
			mn_y_lamp[1][i] = 0;
			OnMaintenance_Yellow_State_Change(i);  // YELLOW Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		}
		st_lamp.mn_lamp_y[i] = mn_y_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// GREEN Ÿ�� ���� ���� ���� ������ �����Ѵ�                             
		// **********************************************************************
		if (mn_g_lamp[1][i]<0 || mn_g_lamp[1][i]>2) 
		{
			mn_g_lamp[1][i] = 0;
			OnMaintenance_Green_State_Change(i);   // GREEN Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		}
		st_lamp.mn_lamp_g[i] = mn_g_lamp[1][i];
		// **********************************************************************
	
	}

	st_lamp.mn_buzzer_mode = mn_buzzer_mode[1];
	st_lamp.mn_front_mode = mn_front_mode[1];;
	st_lamp.mn_rear_mode = mn_rear_mode[1];;

	st_lamp.mn_lamp_wait_time = mn_lamp_flicker_time[1];
	st_lamp.mn_lamp_change_time = mn_lamp_change_time[1];

	// **************************************************************************
	// ��/�� ���� ������ ��� ȣ�� ���� ������ �����Ѵ�                          
	// **************************************************************************
	((CEdit*)GetDlgItem(IDC_EDIT_EQUIP_NO))->GetWindowText(mstr_equip_no[1]);
	mstr_equip_no[1].MakeUpper();
	mstr_equip_no[1].TrimLeft(' ');
	mstr_equip_no[1].TrimRight(' ');

	if (mstr_equip_no[1].IsEmpty())  
	{
		mstr_equip_no[1] = "AMT";
	}
	st_lamp.mstr_equip_no = mstr_equip_no[1];
	// **************************************************************************

	// **************************************************************************
	// ��/�� ���� ������ ��� �ڵ� ���� ������ �����Ѵ�                          
	// **************************************************************************
	((CEdit*)GetDlgItem(IDC_EDIT_EQUIP_ID))->GetWindowText(mstr_equip_id[1]);
	mstr_equip_id[1].MakeUpper();
	mstr_equip_id[1].TrimLeft(' ');               
	mstr_equip_id[1].TrimRight(' ');

	if (mstr_equip_id[1].IsEmpty())  
	{
		mstr_equip_id[1] = "AMT";
	}
	st_lamp.mstr_equip_id = mstr_equip_id[1];
	// **************************************************************************

	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
	}



}

void CDialog_Work_Maintenane::OnMaintenance_Data_BackUp()
{
	int i;

	for(i=0; i<=SELFCHECK; i++)
	{
		// **********************************************************************
		// RED Ÿ�� ���� ���¸� ����Ѵ�                                         
		// **********************************************************************
		mn_r_lamp[0][i] = mn_r_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// YELLOW Ÿ�� ���� ���¸� ����Ѵ�                                      
		// **********************************************************************
		mn_y_lamp[0][i] = mn_y_lamp[1][i];
		// **********************************************************************

		// **********************************************************************
		// GREEN Ÿ�� ���� ���¸� ����Ѵ�                                       
		// **********************************************************************
		mn_g_lamp[0][i] = mn_g_lamp[1][i];
		// **********************************************************************
	}

	mn_buzzer_mode[0] = mn_buzzer_mode[1];

	mn_lamp_flicker_time[0] = mn_lamp_flicker_time[1];
	mn_lamp_change_time[0] = mn_lamp_change_time[1];

	mstr_equip_no[0] = mstr_equip_no[1];
	mstr_equip_id[0] = mstr_equip_id[1];



}

void CDialog_Work_Maintenane::OnMaintenance_ListBar_Info_Send()
{
	if (st_handler.cwnd_list != NULL)
	{
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO);
	}
}

void CDialog_Work_Maintenane::OnMaintenance_Data_Display()
{
	((CEdit*)GetDlgItem(IDC_EDIT_EQUIP_ID))->SetWindowText(mstr_equip_id[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_EQUIP_NO))->SetWindowText(mstr_equip_no[1]);
	m_dgt_flicker_time.SetValue(mn_lamp_flicker_time[1]);
	m_dgt_change_time.SetValue(mn_lamp_change_time[1]);
}

int CDialog_Work_Maintenane::OnMaintenance_Data_Comparison()
{
	int i;
	int Ret = RET_GOOD;

	for(i=0; i<=SELFCHECK; i++)
	{
		// **********************************************************************
		// RED Ÿ�� ���� ���¸� Ȯ���Ѵ�                                         
		// **********************************************************************
		if (mn_r_lamp[0][i] != mn_r_lamp[1][i])					Ret = RET_ERROR;
		// **********************************************************************

		// **********************************************************************
		// YELLOW Ÿ�� ���� ���¸� Ȯ���Ѵ�                                      
		// **********************************************************************
		if (mn_y_lamp[0][i] != mn_y_lamp[1][i])					Ret = RET_ERROR;
		// **********************************************************************

		// **********************************************************************
		// GREEN Ÿ�� ���� ���¸� Ȯ���Ѵ�                                       
		// **********************************************************************
		if (mn_g_lamp[0][i] != mn_g_lamp[1][i])					Ret = RET_ERROR;
		// **********************************************************************
	}

	if (mn_buzzer_mode[0] != mn_buzzer_mode[1])					Ret = RET_ERROR;
	if (mn_lamp_flicker_time[0] != mn_lamp_flicker_time[1])		Ret = RET_ERROR;
	if (mn_lamp_change_time[0] != mn_lamp_change_time[1])		Ret = RET_ERROR;
	if (mstr_equip_no[0] != mstr_equip_no[1])					Ret = RET_ERROR;
	if (mstr_equip_id[0] != mstr_equip_id[1])					Ret = RET_ERROR;
	if (mn_front_mode[0] != mn_front_mode[1])					Ret = RET_ERROR;
	if (mn_rear_mode[0] != mn_rear_mode[1])						Ret = RET_ERROR;





	return Ret;
}

void CDialog_Work_Maintenane::OnMaintenance_Data_Recovery()
{
	int i;

	for (i = 0; i <= SELFCHECK; i++)
	{
		// **********************************************************************
		// RED Ÿ�� ���� ���¸� �����Ѵ�                                         
		// **********************************************************************
		mn_r_lamp[1][i] = mn_r_lamp[0][i];
		// **********************************************************************

		// **********************************************************************
		// YELLOW Ÿ�� ���� ���¸� �����Ѵ�                                      
		// **********************************************************************
		mn_y_lamp[1][i] = mn_y_lamp[0][i];
		// **********************************************************************

		// **********************************************************************
		// GREEN Ÿ�� ���� ���¸� �����Ѵ�                                       
		// **********************************************************************
		mn_g_lamp[1][i] = mn_g_lamp[0][i];
		// **********************************************************************
	}

	mn_buzzer_mode[1] = mn_buzzer_mode[0];

	mn_lamp_flicker_time[1] = mn_lamp_flicker_time[0];
	mn_lamp_change_time[1] = mn_lamp_change_time[0];

	mstr_equip_no[1] = mstr_equip_no[0];
	mstr_equip_id[1] = mstr_equip_id[0];

}

void CDialog_Work_Maintenane::OnMaintenance_Label_Set()
{
	m_msg_equip_id.SetTextFont(mp_maintenance_font);
	m_msg_equip_id.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_equip_id.SetTextAlign(CFloatST::STA_CENTER, 10);

	m_msg_equip_no.SetTextFont(mp_maintenance_font);
	m_msg_equip_no.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_equip_no.SetTextAlign(CFloatST::STA_CENTER, 10);
}

void CDialog_Work_Maintenane::OnMaintenance_Digital_Count_Set()
{
	m_dgt_flicker_time.SubclassDlgItem(IDC_DGT_FLICKER_TIME, this);
	m_dgt_flicker_time.SetStyle(IDB_BIG3, 5);

	m_dgt_change_time.SubclassDlgItem(IDC_DGT_CHANGE_TIME, this);
	m_dgt_change_time.SetStyle(IDB_BIG3, 5);
}

void CDialog_Work_Maintenane::OnMaintenance_Lamp_Change()
{
	int i;

	for(i = 0; i <= SELFCHECK; i++)
	{
		OnMaintenance_Red_State_Change(i);      // RED Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		OnMaintenance_Yellow_State_Change(i);   // YELLOW Ÿ�� ���� ��ư ���� ��ȯ �Լ�
		OnMaintenance_Green_State_Change(i);    // GREEN Ÿ�� ���� ��ư ���� ��ȯ �Լ�
	}
}

void CDialog_Work_Maintenane::OnMaintenance_File_Backup(CString strTargetFile, CString strCreateFile, CString strBackupFolder, CString strBackupFile, BOOL b_over_write)
{
	CString str_backup_file;	// ������ ��� ������ [���+���ϸ�+Ȯ����] ���� ����
	int n_existence;			// ���� ���� ���� �÷���
	int n_response;				// ��ȭ ���� ���� �÷���
	
	char chr_file[256];
	char szDrive[4] = {'\0',};
	char szDir[MAX_PATH] = {'\0',};
	char szName[MAX_PATH] = {'\0',};
	char szExt[MAX_PATH] = {'\0',};

	CDialog_Select  select_dlg;

	// **************************************************************************
	// ����� ������ �̹� �����ϴ��� �˻��Ѵ�                                    
	// -> �̹� �����ϴ� ��쿡�� ����ڿ��� ����� ���� ���� �ȴ�              
	// **************************************************************************
	sprintf(chr_file, "%s", strCreateFile);

	n_existence = access(chr_file,0) ;

	if (n_existence != -1)  // ���� ����
	{
		st_msg.mstr_confirm_msg = _T("Backup File Exist~! Overwrite??");

		n_response = select_dlg.DoModal();
		if (n_response == IDCANCEL)
			return ; 
	}
	// **************************************************************************

	// **************************************************************************
	// ��� ���� �����Ѵ�                                                        
	// **************************************************************************
	_tsplitpath(strTargetFile, szDrive, szDir, szName, szExt); 
	str_backup_file.Format("%s\\%s%s", strBackupFolder, strBackupFile, szExt);
	
	CopyFile(strTargetFile, str_backup_file, !b_over_write);
	// **************************************************************************
}

// ******************************************************************************
//  Tower Lamp ȭ�� ���� ���� ��û �޽���                                        
// ******************************************************************************
LRESULT CDialog_Work_Maintenane::OnMaintenance_Info_Apply(WPARAM wParam, LPARAM lParam)  
{
	OnMaintenance_Data_Apply();			// Tower Lamp ȭ�� ���� ���� ���� ������ ���� �Լ�

	OnMaintenance_LogFile_Create();

	OnMaintenance_Data_BackUp();

	OnMaintenance_ListBar_Info_Send();   // Ÿ��Ʋ �� ȭ�鿡 ���� ��� ��û�ϴ� �Լ�

	return 0;
}

void CDialog_Work_Maintenane::OnRLampClick(UINT nID)
{
	int idx = nID - ID_LAMP_RLAMP;		// �迭 ��ġ ����

	mn_r_lamp[1][idx]++;
	if (mn_r_lamp[1][idx]<0 || mn_r_lamp[1][idx]>2)
		mn_r_lamp[1][idx] = 0;

	OnMaintenance_Red_State_Change(idx);		// RED Ÿ�� ���� ��ư ���� ��ȯ �Լ�
}

void CDialog_Work_Maintenane::OnYLampClick(UINT nID)
{
	int idx = nID - ID_LAMP_YLAMP;		// �迭 ��ġ ����
	
	mn_y_lamp[1][idx]++;
	if (mn_y_lamp[1][idx]<0 || mn_y_lamp[1][idx]>2)
		mn_y_lamp[1][idx] = 0;

	OnMaintenance_Yellow_State_Change(idx);	// YELLOW Ÿ�� ���� ��ư ���� ��ȯ �Լ�
}

void CDialog_Work_Maintenane::OnGLampClick(UINT nID)
{
	int idx = nID - ID_LAMP_GLAMP;		// �迭 ��ġ ����
	
	mn_g_lamp[1][idx]++;
	if (mn_g_lamp[1][idx]<0 || mn_g_lamp[1][idx]>2)
		mn_g_lamp[1][idx] = 0;

	OnMaintenance_Green_State_Change(idx);		// GREEN Ÿ�� ���� ��ư ���� ��ȯ �Լ�
}

void CDialog_Work_Maintenane::OnDgtFlickerTime() 
{
	int mn_response;	// ��ȭ ���ڿ� ���� ���� �� ���� ����
	CString mstr_temp;  // ������ ���� �ӽ� ���� ���� 
	char chr_buf[20] ;

	CDialog_KeyPad pad_dlg;

	mn_lamp_flicker_time[1] = m_dgt_flicker_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_lamp_flicker_time[1], chr_buf, 10));

	st_msg.mstr_keypad_msg = _T("Lamp Flicker Time Setting");
    
	st_msg.mstr_keypad_val = mstr_temp;

	st_msg.mstr_pad_high_limit = "10000";
	st_msg.mstr_pad_low_limit = "10";

	CRect r;
	
	m_dgt_flicker_time.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	pad_dlg.m_ptRefLeft = CPoint(r.left, r.top);	//20120516
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		mn_lamp_flicker_time[1] = atoi(mstr_temp);

		m_dgt_flicker_time.SetValue(mn_lamp_flicker_time[1]);
	}
	else if (mn_response == IDCANCEL)
	{

	}
}

void CDialog_Work_Maintenane::OnChkBuzzer() 
{
	if (m_chk_buzzer.GetCheck() == TRUE)	mn_buzzer_mode[1] = TRUE;
	else									mn_buzzer_mode[1] = FALSE;

	OnMaintenance_Buzzer_Change();
}

void CDialog_Work_Maintenane::OnBtnDataBackup() 
{
	CString str_year, str_month, str_day;	// ������ ��, ��, �� ���� ����
	CString str_back_day, str_back_time;
	CString str_back_file;					// ��� ���ϸ� ���� ����
	CString str_back_info;					// ����� ���� ���� ���� ���� [����+���ϸ�+Ȯ����]
	CString str_temp_file;					// ���ϸ� �ӽ� ���� ����
	CString str_chk_ext;					// ���� Ȯ���� ���� ����
	CString str_device;
	int n_response;							// ��ȭ ���� ���� �÷���
	int n_year, n_month, n_day;				// ��, ��, �� ���� ����
	CString str_cur_hour, str_cur_min, str_cur_sec;
	
	char chr_buf[20];

	CDialog_Select   select_dlg;
	CDialog_Message  msg_dlg;

	st_msg.mstr_confirm_msg = _T("Do you back up environment Setting information?");

	n_response = select_dlg.DoModal();

	if (n_response == IDOK)
	{
		// ******************************************************************************
		// ��� ���ϸ����� ����� ��¥ ���� �����Ѵ�                                     
		// ******************************************************************************
		COleDateTime cur_time = COleDateTime::GetCurrentTime();
		n_year = cur_time.GetYear();  
		n_month = cur_time.GetMonth();  
		n_day = cur_time.GetDay();  

		str_year = LPCTSTR(_itoa( n_year, chr_buf, 10 ));  
		str_month.Format("%02d", n_month);
		str_day.Format("%02d", n_day);

		str_back_day = str_year;
		str_back_day += str_month;
		str_back_day += str_day;
		// ******************************************************************************

		// **************************************************************************
		// ���� �ð� ���� ��´�                                                     
		// **************************************************************************
		str_cur_hour.Format("%02d",cur_time.GetHour());	// ���� �ð� ������ �����Ѵ�.
		str_cur_min.Format("%02d",cur_time.GetMinute());	// ���� �� ������ �����Ѵ�. 
		str_cur_sec.Format("%02d",cur_time.GetSecond());	// ���� �� ������ �����Ѵ�. 

		str_back_time = str_cur_hour;
		str_back_time += str_cur_min;
		str_back_time += str_cur_sec;
		// **************************************************************************

		// ******************************************************************************
		// ������ ���� ���� ���� ���� ����Ѵ�                                           
		// -> ����� ���� ������ �����ϴ��� �˻��ϰ� �ȴ�                                
		// ******************************************************************************
		str_back_file = "c:\\Backup\\" + str_back_day + "_" + str_back_time;  // ��� ���ϸ� ���� [��¥+���ϸ�]

		CopyFolder(st_path.mstr_path_dvc, str_back_file);
		CopyFolder(st_path.mstr_basic_folder, str_back_file);
	}
}

void CDialog_Work_Maintenane::OnPadEquipNo() 
{
	int n_response;  // ��ȭ ���� ���� �÷���

	CDialog_Keyboard  keyboard_dlg;

	((CEdit*)GetDlgItem(IDC_EDIT_EQUIP_NO))->GetWindowText(mstr_equip_no[1]);
	
	if (mstr_equip_no[1].IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();			// Ű���� ���� ��� ������ �ʱ�ȭ
	}
	else 
	{
		mstr_equip_no[1].MakeUpper();
		mstr_equip_no[1].TrimLeft(' ');               
		mstr_equip_no[1].TrimRight(' ');

		st_msg.mstr_typing_msg = mstr_equip_no[1];  // Ű���� ���� ��� ������ ����
	}

	n_response = keyboard_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		mstr_equip_no[1] = st_msg.mstr_typing_msg;  // Ű���� ���� �Է� ���� Ŭ���� ������ ����

		mstr_equip_no[1].MakeUpper();
		mstr_equip_no[1].TrimLeft(' ');               
		mstr_equip_no[1].TrimRight(' ');
		((CEdit*)GetDlgItem(IDC_EDIT_EQUIP_NO))->SetWindowText(mstr_equip_no[1]);
	}
}

void CDialog_Work_Maintenane::OnPadEquipId() 
{
	int n_response;  // ��ȭ ���� ���� �÷���

	CDialog_Keyboard  keyboard_dlg ;

	((CEdit*)GetDlgItem(IDC_EDIT_EQUIP_ID))->GetWindowText(mstr_equip_id[1]) ;
	if (mstr_equip_id[1].IsEmpty())
	{
		(st_msg.mstr_typing_msg).Empty();          // Ű���� ���� ��� ������ �ʱ�ȭ
	}
	else 
	{
		mstr_equip_id[1].MakeUpper();
		mstr_equip_id[1].TrimLeft(' ');               
		mstr_equip_id[1].TrimRight(' ');

		st_msg.mstr_typing_msg = mstr_equip_id[1];  // Ű���� ���� ��� ������ ����
	}

	n_response = keyboard_dlg.DoModal();
	if (n_response == IDOK)
	{
		mstr_equip_id[1] = st_msg.mstr_typing_msg;  // Ű���� ���� �Է� ���� Ŭ���� ������ ����

		mstr_equip_id[1].MakeUpper();
		mstr_equip_id[1].TrimLeft(' ');               
		mstr_equip_id[1].TrimRight(' ');
		((CEdit*)GetDlgItem(IDC_EDIT_EQUIP_ID))->SetWindowText(mstr_equip_id[1]);
	}
}

void CDialog_Work_Maintenane::OnBtnPasswordChangeLevle1() 
{
	int n_response;  // ��ȭ ���� ���� �÷���

	CDialog_Pass_Change  pass_cng_dlg;

	st_handler.mstr_pass_level=_T("Level1");  // ��ȣ ���� : ����Ʈ ����
	n_response = pass_cng_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CDialog_Work_Maintenane::OnBtnPasswordChangeLevle2() 
{
	int n_response;  // ��ȭ ���� ���� �÷���

	CDialog_Pass_Change  pass_cng_dlg;

	st_handler.mstr_pass_level=_T("Level2");  // ��ȣ ���� : ƼĪ ����
	n_response = pass_cng_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (n_response == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void CDialog_Work_Maintenane::OnBtnMaintenanceApply() 
{
	int n_response;		// ��ȭ ���� ���� �÷���

	CDialog_Select  select_dlg;

	st_msg.mstr_confirm_msg = _T("Maintenance : Setting Data Apply?");

	n_response = select_dlg.DoModal();

	if (n_response == IDOK)
	{
		OnMaintenance_Data_Comparison();	//20120516

		OnMaintenance_Data_Apply();			// Tower Lamp ȭ�� ���� ���� ���� ������ ���� �Լ�

		OnMaintenance_LogFile_Create();

		OnMaintenance_Data_BackUp();

		OnMaintenance_ListBar_Info_Send();	// ����Ʈ �� ȭ�鿡 ���� ��� ��û�ϴ� �Լ�

		/////////////////////////////////////////// 20120523
		mcls_maintenance.OnMaintenance_Data_Save();
		mcls_maintenance.OnMaintenance_Data_Load();

		/////////////////////////////////////////// 
	}
	else if (n_response == IDCANCEL)
	{

	}
}

void CDialog_Work_Maintenane::OnSetfocusEditEquipId() 
{
	m_btn_maintenance_apply.SetFocus();
}

void CDialog_Work_Maintenane::OnSetfocusEditEquipNo() 
{
	m_btn_maintenance_apply.SetFocus();
}

void CDialog_Work_Maintenane::OnMaintenance_LogFile_Create()
{
	int i;
	CString str_msg;
	CString str_data;
	CString str_mode;
	int Ret = RET_GOOD;

	for(i=0; i<=SELFCHECK; i++)
	{
		if (i == 0)			str_mode = "STOP";
		else if (i == 1)	str_mode = "RUN";
		else if (i == 2)	str_mode = "JAM";
		else if (i == 3)	str_mode = "LOT END";
		else if (i == 4)	str_mode = "INIT";
		else if (i == 5)	str_mode = "WARNING";
		else if (i == 6)	str_mode = "LOCK";
//		else if (i == 7)	str_mode = "SELF CHECK";
		else if (i == 7)	str_mode = "EMPTY RUN";
		
		// **********************************************************************
		// RED Ÿ�� ���� ���¸� Ȯ���Ѵ�                                         
		// **********************************************************************
		if (mn_r_lamp[0][i] != mn_r_lamp[1][i])
		{
			if (mn_r_lamp[1][i] == 0)		str_data = "OFF";
			else if (mn_r_lamp[1][i] == 1)	str_data = "ON";
			else if (mn_r_lamp[1][i] == 2)	str_data = "FLICKER";

			str_msg.Format("[Tower Lamp] %s Status : RED LAMP was changed by %s", str_mode, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
		// **********************************************************************

		// **********************************************************************
		// YELLOW Ÿ�� ���� ���¸� Ȯ���Ѵ�                                      
		// **********************************************************************
		if (mn_y_lamp[0][i] != mn_y_lamp[1][i])
		{
			if (mn_y_lamp[1][i] == 0)		str_data = "OFF";
			else if (mn_y_lamp[1][i] == 1)	str_data = "ON";
			else if (mn_y_lamp[1][i] == 2)	str_data = "FLICKER";

			str_msg.Format("[Tower Lamp] %s Status : YELLOW LAMP was changed by %s", str_mode, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
		// **********************************************************************

		// **********************************************************************
		// GREEN Ÿ�� ���� ���¸� Ȯ���Ѵ�                                       
		// **********************************************************************
		if (mn_g_lamp[0][i] != mn_g_lamp[1][i])
		{
			if (mn_g_lamp[1][i] == 0)		str_data = "OFF";
			else if (mn_g_lamp[1][i] == 1)	str_data = "ON";
			else if (mn_g_lamp[1][i] == 2)	str_data = "FLICKER";

			str_msg.Format("[Tower Lamp] %s Status : GREEN LAMP was changed by %s", str_mode, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
		// **********************************************************************
	}

	if (mn_buzzer_mode[0] != mn_buzzer_mode[1])
	{
		if (mn_buzzer_mode[1] == 0)
		{
			str_data = _T("On");
		}
		else
		{
			str_data = _T("Off");
		}
		
		str_msg = "[TowerLamp] Buzzer Mode was changed by " + str_data;
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_lamp_flicker_time[0] != mn_lamp_flicker_time[1])
	{
		str_data.Format("%d", (int)mn_lamp_flicker_time[1]);
		str_msg = "[TowerLamp] Lamp Flicker Time was changed by " + str_data + "(msec)";
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_lamp_change_time[0] != mn_lamp_change_time[1])
	{
		str_data.Format("%d", (int)mn_lamp_change_time[1]);
		str_msg = "[TowerLamp] Lamp Change Time was changed by " + str_data + "(Sec)";
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mstr_equip_no[0] != mstr_equip_no[1])
	{
		str_msg = "[TowerLamp] EQUIP NO was changed by " + mstr_equip_no[1];
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mstr_equip_id[0] != mstr_equip_id[1])
	{
		str_msg = "[TowerLamp] EQUIP ID was changed by " + mstr_equip_id[1];
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_front_mode[0] != mn_front_mode[1])
	{
		str_msg = "[Front Heat sink was changed by " + mn_front_mode[1];
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	if (mn_rear_mode[0] != mn_rear_mode[1])
	{
		str_msg = "[Rear Heat sink was changed by " + mn_front_mode[1];
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}
}

void CDialog_Work_Maintenane::OnDgtChangeTime() 
{
	int mn_response;	// ��ȭ ���ڿ� ���� ���� �� ���� ����
	CString mstr_temp;  // ������ ���� �ӽ� ���� ���� 
	char chr_buf[20] ;

	CDialog_KeyPad pad_dlg;

	mn_lamp_change_time[1] = m_dgt_change_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_lamp_change_time[1], chr_buf, 10));

	st_msg.mstr_keypad_msg = _T("Lamp Change Time Setting");
    
	st_msg.mstr_keypad_val = mstr_temp;

	st_msg.mstr_pad_high_limit = "100";
	st_msg.mstr_pad_low_limit = "0";

	CRect r;
	
	m_dgt_change_time.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	pad_dlg.m_ptRefLeft = CPoint(r.left, r.top);	//20120516

	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_msg.mstr_keypad_val;
		mn_lamp_change_time[1] = atoi(mstr_temp);

		m_dgt_change_time.SetValue(mn_lamp_change_time[1]);
	}
	else if (mn_response == IDCANCEL)
	{

	}
}

void CDialog_Work_Maintenane::OnBtnStepSave() 
{
	char path[MAX_PATH] = {0};
	//������ �۾���θ� ���� ���� �Ѵ�.
	GetCurrentDirectory(MAX_PATH, path);
	
	CFileDialogST	dlg(FALSE, NULL, "Step_back", OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, _T("Text File(*.txt)\0*.txt\0"), this);
	CString			sPathName, str_temp_file, str_ext, str_chk_ext;
	int				nRetValue, n_pos;
	
	BOOL			bResult; // = find.FindFile(str_file);
	
	CFileFind find;
	
	nRetValue = dlg.DoModal();
	
	if (nRetValue == IDOK)
	{
		sPathName = dlg.GetPathName();   // ȭ�鿡 �������� ������ ������ ��� �� ������ �̸��� �����!!
		
		n_pos = sPathName.Find(".txt");
		if(n_pos == -1)
			n_pos = sPathName.Find(".TXT");
		if(n_pos == -1)
			sPathName += ".txt";
		
		
		bResult = find.FindFile(sPathName);
		find.Close();
		
		if(bResult != 0)
		{
			DeleteFile(sPathName);
		}
		
		(st_msg.str_step_savefile_name).Format("%s", dlg.GetFileName());
		(st_msg.str_step_savefile_path).Format("%s", dlg.GetFileDir());

		OnMaintenance_Step_Save();
		
		SetCurrentDirectory(path);						// �۾� ��θ� ���� ��η� ���� �Ѵ�.
	}
	else if (nRetValue == IDCANCEL)
	{
		
	}
}

void CDialog_Work_Maintenane::OnMaintenance_Step_Save()
{
	CString mstr_file_name;    // ���������� ������ ���� �̸� ���� ���� 
	CString mstr_create_file;  // �˶� ���� ������ ���Ͽ� ���� [����]+[���ϸ�]+[Ȯ����] ���� ���� 
	CString mstr_list_name, mstr_temp_data;  // �� �κк� �˶� �߻� Ƚ�� ���� ������ �α� ���� ���� ���� ���� 
	CString mstr_content;  // �� ���ο� ����� ���ڿ� ���� ���� ���� 
	int mn_existence, i;      // ���� ���� ���� ���� �÷��� 
	char fileName[256];    // �˻��� ���� ���� ���� �Լ� 
	FILE  *fp ;            // ���Ͽ� ���� ������ ���� ���� 
	
	CString str_title_time, str_file_time, str_new_file;		// ���Ͽ� ����� Ÿ��Ʋ �ð� ���� ����
	int n_cur_year, n_cur_month, n_cur_day;		// ��, ��, �� ���� ����
	int n_cur_hour, n_cur_minute, n_cur_second;		// ��, ��, �� ���� ����
	
	COleDateTime mtime_cur;						// ���� �ð� ���� ����
	CTime mp_time_cur;							// Ÿ��Ʋ �ð� ���� ����

	CString str_temp_data;
	int n_length;

	CString strData, str_time;
	
	/* ************************************************************************** */
	/* ���� �̸����� ����� ��¥ �����Ѵ�                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	
	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  

	n_cur_hour = mtime_cur.GetHour();
	n_cur_minute = mtime_cur.GetMinute();
	n_cur_second = mtime_cur.GetSecond();
	
	str_new_file.Format("DATA%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	str_time.Format("%04d/%02d/%02d %02d:%02d:%02d", n_cur_year, n_cur_month, n_cur_day, n_cur_hour, n_cur_minute, n_cur_second);
		
	mstr_create_file = st_msg.str_step_savefile_path + st_msg.str_step_savefile_name;
	mstr_create_file += ".TXT";
	
	sprintf(fileName, "%s", mstr_create_file);
	mn_existence = access(fileName,0);
	
	if (mn_existence == -1)  /* ���� ������ */
	{
		mstr_create_file = st_msg.str_step_savefile_path + st_msg.str_step_savefile_name;
		mstr_create_file += ".TXT";
		/* ********************************************************************** */
	}
	
	/* ************************************************************************** */
	/* �˶� �߻� Ƚ�� ���� ���� ���Ͽ� �߰� ������ ���� ���Ϸ� ����               */
	/* ************************************************************************** */
	if ((fp=fopen(mstr_create_file,"a+"))==NULL)  
	{
		Func.MsgLog("���� ���� ����!..");
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("Failed to open file!..");
		}

		return;
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ���Ͽ� Ÿ��Ʋ ���� ����Ѵ�                                                */
	/* ************************************************************************** */
	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "AMT830 [MODULE HEAT SINK] : " + st_handler.mstr_handler_date);
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "Thread Step Info File");
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content.Format("| %-100s |", "DIVISION  : AMT Coporation");
	fprintf(fp,"%s\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "File Name : " + st_msg.str_step_savefile_path + st_msg.str_step_savefile_name);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content.Format("| %-100s |", "Save Time : " + str_time);
	fprintf(fp,"%s\r\n", mstr_content) ;

	mstr_content = "********************************************************************************************************";
	fprintf(fp,"%s\r\n", mstr_content) ;
	
	mstr_content = "+==========================+=============================================================================================================================+";
	fprintf(fp,"%s\r\n",mstr_content);
	
	mstr_content = "|  Step Trace              |                                                    History                                                                  |";
	fprintf(fp,"%s\n", mstr_content );
	
	mstr_content = "+==========================+=============================================================================================================================+";
	fprintf(fp,"%s\r\n",mstr_content) ;
	
	str_temp_data = "";
	
	for (i = 0; i < 100; i++)
	{
		str_temp_data = st_work.mstr_last_change_date[i];
		mstr_content.Format("|  Step %02d Change LastDate |", i);
		Func.TextAndLine(130, mstr_content, str_temp_data, fp);
		
		if (st_work.mstr_history_step_back_up[i] != "")
		{
			str_temp_data = st_work.mstr_history_step_back_up[i];
			n_length = str_temp_data.GetLength();
			mstr_content.Format("|  Step %02d before History  |", i);
			mstr_content += str_temp_data;
			fprintf(fp,"%s\r\n", mstr_content);
			mstr_content =      "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";
			fprintf(fp,"%s\r\n",mstr_content);
		}
		str_temp_data = st_work.mstr_history_step[i];
		n_length = str_temp_data.GetLength();
		mstr_content.Format("|  Step %02d History         |", i);

		if (str_temp_data == "")
		{
			strData.Format("%-124s |", "");
			mstr_content += strData;
			fprintf(fp,"%s\r\n", mstr_content) ;
		}
		else
		{
			mstr_content += str_temp_data;
			fprintf(fp,"%s\r\n", mstr_content);
		}
		mstr_content =      "+--------------------------+-----------------------------------------------------------------------------------------------------------------------------+";
		fprintf(fp,"%s\r\n",mstr_content);
	}
	
	if (ferror(fp))  
	{
		Func.MsgLog("���� ���� ����!..") ;
		if(st_handler.mn_language == LANGUAGE_ENGLISH)
		{
			Func.MsgLog("Failed to save file!..");
		}

		return ;
	}
	
	fclose(fp);  /* ���� ���� */
}

BOOL CDialog_Work_Maintenane::CopyFolder(CString &strFrom, CString &strTo)
{	
	CFileFind finder;
	
    CString sWildCard = strFrom + "\\*.*";
	
    BOOL bFound;
    BOOL bWorking = finder.FindFile(sWildCard);
	
    bFound = bWorking;
	
    ::CreateDirectory(strTo, NULL);
	
    while(bWorking)
    {
        bWorking = finder.FindNextFile();
		
        // skip . and .. files; otherwise, we"d
        // recur infinitely!
        if(finder.IsDots()) continue;
		
        // ���丮�̸� ��� ȣ��
        if(finder.IsDirectory())
        {
            CString sFrom2 = finder.GetFilePath();
            CString sTo2 = strTo + "\\" + finder.GetFileName();
            ::CreateDirectory(sTo2, NULL);
            
            CopyFolder(sFrom2, sTo2);
            continue;
        }
        ExCopyFile(finder.GetFilePath(), strTo);
    }
    return bFound;

}

int CDialog_Work_Maintenane::ExCopyFile(CString strFrom, CString strTo)
{
	SHFILEOPSTRUCT shfo;
	
    char pszFrom[1024] = {0};
    char pszTo[1024] = {0};
	
    WORD wFlag = 0;
	
    strcpy(pszFrom, (LPCTSTR)strFrom);
    strcpy(pszTo, (LPCTSTR)strTo);
	
    pszFrom[lstrlen(pszFrom) + 1] = 0;
	
    shfo.hwnd = m_hWnd;
    shfo.wFunc = FO_COPY;
    shfo.pFrom = pszFrom;
    shfo.pTo = pszTo;
    shfo.fFlags = 0;
    shfo.lpszProgressTitle = "File Copy..";
	
    return SHFileOperation(&shfo);

}



void CDialog_Work_Maintenane::OnChkHsFront() 
{
	if (m_chk_hs_front.GetCheck() == TRUE)	mn_front_mode[1] = TRUE;
	else									mn_front_mode[1] = FALSE;
	
	OnMaintenance_Hs_Change();	
}

void CDialog_Work_Maintenane::OnChkHsRear() 
{
	if (m_chk_hs_rear.GetCheck() == TRUE)	mn_rear_mode[1] = TRUE;
	else									mn_rear_mode[1] = FALSE;
	
	OnMaintenance_Hs_Change();	
}

void CDialog_Work_Maintenane::OnBtnDelete() 
{
	m_lstboxtxt.DeleteString(m_lstboxtxt.GetCaretIndex());
	
}

void CDialog_Work_Maintenane::OnButtonSave() 
{
	CString strFilePath, strMdlFile, strFileName, strNum;
	int nIndex = m_lstboxtxt.GetCount();
	
	m_cbo_model.GetWindowText(strFileName);
	m_cbo_model.GetLBText(m_nCboSelModel, strFileName);
	
	CString mstr_temp;
	mstr_temp.Format("%d",nIndex);
	strFilePath = "C:\\AMT830\\Setting\\ModelName\\" + strFileName + _T(".TXT");
	DeleteFile(strFilePath);
	
	:: WritePrivateProfileString(strFileName, "Total", LPCTSTR(mstr_temp), strFilePath);
	
	for (int i = 0; i < nIndex; i++)
	{
		m_lstboxtxt.GetText(i, strMdlFile);
		strNum.Format("%d",i+1);
		:: WritePrivateProfileString(strFileName, strNum, LPCTSTR(strMdlFile), strFilePath);
	}	
}

//////////////////////////////////////////////////////////////////////////
//2017.0429
void CDialog_Work_Maintenane::RegModel()
{
	m_nCboSelModel = 0;
	Func.LoadRegModelData();
	CString strCbomo="";
	
	m_cbo_model.ResetContent();
	
	for (int i= 0; i < st_work.m_nMdlTotal; i++ )
	{
		strCbomo.Format("%s", st_work.m_strModelName[i]);
		strCbomo.TrimLeft(); strCbomo.TrimRight();
		m_cbo_model.AddString( (LPCTSTR)strCbomo);
		Func.ModelFileload(strCbomo);
		if(Func.nItemLength > 0) 
		{
			for (int ii = 0; ii < Func.nItemLength; ii++)
			{
				strModel[i][ii] = Func.m_strItemValue[ii];
			}
		}
		
	}
	OnDisplay(m_nCboSelModel);
}

void CDialog_Work_Maintenane::OnDisplay(int nCboMdl)
{
	
	CString strCbomo;
	
	nItemLength = 0;
	strCbomo.Format("%s", st_work.m_strModelName[nCboMdl]);
	strCbomo.TrimLeft(); strCbomo.TrimRight();
	// 	m_cbo_model.AddString( (LPCTSTR)strCbomo);
	Func.ModelFileload(strCbomo);
	for(int i = 0; i < LMAX_LINE; i++)  
	{
		strModel[nCboMdl][i].Empty();
	}
	if(Func.nItemLength > 0) 
	{
		for (int ii = 0; ii < Func.nItemLength; ii++)
		{
			strModel[nCboMdl][ii] = Func.m_strItemValue[ii];
		}
	}
	
	m_cbo_model.SetCurSel(m_nCboSelModel);	
	
	
	if(strModel[m_nCboSelModel][0].GetLength() > 0)
	{
		int i = 0;
		do 
		{
			m_lstboxtxt.AddString(strModel[m_nCboSelModel][i]);
			i++;
		} while (strModel[m_nCboSelModel][i].GetLength() > 0);
		
	}
	
}

void CDialog_Work_Maintenane::LineAnalysis()
{
	unsigned short getAscii = 0;
	CString strTmp;
	int length, find;
	
	for(int i = 0 ; i < nItemLength ; i++)
	{
		length = strLineInfo[i].GetLength();
		find = strLineInfo[i].Find('=', 0);
		strItemName[i] = strLineInfo[i].Left(find);
		strItemName[i].TrimRight();
		strItemValue[i] = strLineInfo[i].Right(length - (find + 1));
		strItemValue[i].TrimLeft();
		strItemValue[i].TrimRight();
	}
}

void CDialog_Work_Maintenane::LineCut(BYTE* buffer)
{
	CString strBuffer, strGetchar, strTmp;
	int length;
	unsigned short getAscii = 0;
	
	nItemLength = 0;
	strBuffer = buffer;
	length = strBuffer.GetLength();
	
	for(int i = 0 ; i < length ; i++)
	{
		getAscii = strBuffer.GetAt(i);
		strGetchar.Format("%c", getAscii);
		strTmp = strTmp + strGetchar;
		if(getAscii == 10)
		{
			strLineInfo[nItemLength++] = strTmp;
			strTmp = "";
		}
	}
	LineAnalysis();
}

int CDialog_Work_Maintenane::FileWrite(CString strFileName, CString strContent)
{
	int nResult;
	//	::WaitForSingleObject(m_hMutex,100);
	
// 	CThreadSync Sync;
	
	if(strFileName.IsEmpty())
	{
		nResult = -1;
	}
	
	else
	{
		CFile file;
		
		TRY
		{
			if(file.Open(strFileName, CFile::modeWrite | CFile::modeCreate|CFile::shareDenyNone) == TRUE)
			{
				file.Write(strContent, strContent.GetLength());
				file.Close();
				nResult = 1;
				
			}
		}
		
		CATCH (CFileException, e)
		{
			AfxMessageBox("ȭ�� ���� ����");
			file.Close();
			nResult = -2;
		}
		END_CATCH
	}
	
	//	::ReleaseMutex(m_hMutex);
	return nResult;
}

int CDialog_Work_Maintenane::FileRead(CString strFileName)
{

	int nResult;
	
	// CString�� �迭�� �ʱ�ȭ.
	
	for(int i = 0; i < LMAX_LINE; i++)  
	{
		strLineInfo[i].Empty();
		strItemName[i].Empty();
		strItemValue[i].Empty();
	}
	
	
	if(strFileName.IsEmpty())
	{
		nResult = -1;
	}
	else
	{
		TRY
		{
			BYTE buffer[LMAX_SIZE];
			
			for(int i=0; i<LMAX_SIZE; i++)
				buffer[i] = NULL;
			
			CFile file(_T(strFileName), CFile::modeReadWrite|CFile::shareDenyNone);
			DWORD dwBytesRemaining = file.GetLength();
			
			while(dwBytesRemaining)
			{
				UINT nBytesRead = file.Read(buffer, sizeof(buffer));
				dwBytesRemaining -= nBytesRead;
			}
			
			file.Close();
			LineCut(&buffer[0]);
			nResult =1;
		}
		CATCH(CFileException, e)
		{
			return -2;
		}
		END_CATCH
	}
	
	return nResult;
}

int CDialog_Work_Maintenane::SaveValue(CString strFileName, CString itemName, CString itemValue)
{
	FileRead(strFileName);
	
	CString strFind, strContent, strTmp;
	int chk = 0;
	
	for(int uuu = 0 ; uuu < nItemLength ; uuu++)
	{
		if(strItemName[uuu] == itemName)
		{
			chk = 1;
			strItemValue[uuu] = itemValue;
			strLineInfo[uuu].Format("%s=%s\r\n", itemName, itemValue);
		}
		
		strContent += strLineInfo[uuu];
	}
	
	if(chk)
	{
		FileWrite(strFileName, strContent);
		return 1;
	}
	else
		return -1;
}

void CDialog_Work_Maintenane::OnButton1() 
{
	CString strTest="",strLeft="";
	m_edit_mdl.GetWindowText(strTest);
	UpdateData(TRUE);
	strTest.TrimLeft(); strTest.TrimRight();
	strTest.TrimLeft(' '); strTest.TrimRight(' ');
	if(strTest == "") return;
	strLeft = strTest.Left(5);
	m_lstboxtxt.AddString(strLeft);	
}

void CDialog_Work_Maintenane::OnSelchangeComboModel() 
{
	int nIndex = m_lstboxtxt.GetCount();
	
	
	m_lstboxtxt.ResetContent();   // ����Ʈ �ڽ� ��Ʈ�ѿ� �߰��� ��� ������ ���� 
	//	m_lstboxtxt.DeleteString()
	
	m_nCboSelModel = m_cbo_model.GetCurSel();	
	OnDisplay(m_nCboSelModel);
}
