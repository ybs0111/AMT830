// Dialog_Work_Barcode_Info.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Work_Barcode_Info.h"
#include "KeyboardDll.h"	//20120514 À¯¼ºÁØ
#include "Dialog_Select.h"
#include "Dialog_KeyPad.h"
#include "Dialog_Message.h"
#include "Dialog_Keyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Barcode_Info dialog


CDialog_Work_Barcode_Info::CDialog_Work_Barcode_Info(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Work_Barcode_Info::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Work_Barcode_Info)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "Label Printer Status Info";

}


void CDialog_Work_Barcode_Info::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Work_Barcode_Info)
	DDX_Control(pDX, IDC_BTN_APPLY, m_btn_apply);
	DDX_Control(pDX, IDC_STATIC_PICTURE_INFO, m_static_picture_info);

	
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Work_Barcode_Info, CDialog)
	//{{AFX_MSG_MAP(CDialog_Work_Barcode_Info)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Barcode_Info message handlers

BOOL CDialog_Work_Barcode_Info::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(20,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	Init_Button();
	
	return TRUE;
}

void CDialog_Work_Barcode_Info::Init_Button()
{
	short	shBtnColor = 30;

	m_btn_apply.SetFont(m_p_font);
 	m_btn_apply.SetIcon(IDI_MAINTENANCE_1);
	m_btn_apply.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_apply.SetWindowText("OK");
	m_btn_apply.SetButtonColor(1);
	m_btn_apply.SetCheck(TRUE);
}

void CDialog_Work_Barcode_Info::OnDestroy()
{
	CDialog::OnInitDialog();

	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}

}

void CDialog_Work_Barcode_Info::OnBtnApply() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	OnDestroy();
	
}
