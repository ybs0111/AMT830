// Dialog_Work_buffer.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Work_buffer.h"
#include "Variable.h"
#include "Dialog_Select.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_buffer dialog


CDialog_Work_buffer::CDialog_Work_buffer(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Work_buffer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Work_buffer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Work_buffer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Work_buffer)
	DDX_Control(pDX, IDC_CHK_MODULE4, m_chk_module4);
	DDX_Control(pDX, IDC_CHK_MODULE3, m_chk_module3);
	DDX_Control(pDX, IDC_CHK_MODULE2, m_chk_module2);
	DDX_Control(pDX, IDC_CHK_MODULE, m_chk_module1);
	DDX_Control(pDX, IDC_CHK_FRONT_HS4, m_chk_front_hs4);
	DDX_Control(pDX, IDC_CHK_FRONT_HS3, m_chk_front_hs3);
	DDX_Control(pDX, IDC_CHK_FRONT_HS2, m_chk_front_hs2);
	DDX_Control(pDX, IDC_CHK_FRONT_HS, m_chk_front_hs1);
	DDX_Control(pDX, IDC_CHK_BACK_HS4, m_chk_back_hs4);
	DDX_Control(pDX, IDC_CHK_BACK_HS3, m_chk_back_hs3);
	DDX_Control(pDX, IDC_CHK_BACK_HS2, m_chk_back_hs2);
	DDX_Control(pDX, IDC_CHK_BACK_HS, m_chk_back_hs1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Work_buffer, CDialog)
	//{{AFX_MSG_MAP(CDialog_Work_buffer)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CHK_BACK_HS, OnChkBackHs)
	ON_BN_CLICKED(IDC_CHK_BACK_HS2, OnChkBackHs2)
	ON_BN_CLICKED(IDC_CHK_BACK_HS3, OnChkBackHs3)
	ON_BN_CLICKED(IDC_CHK_BACK_HS4, OnChkBackHs4)
	ON_BN_CLICKED(IDC_CHK_MODULE, OnChkModule)
	ON_BN_CLICKED(IDC_CHK_MODULE2, OnChkModule2)
	ON_BN_CLICKED(IDC_CHK_MODULE3, OnChkModule3)
	ON_BN_CLICKED(IDC_CHK_MODULE4, OnChkModule4)
	ON_BN_CLICKED(IDC_CHK_FRONT_HS, OnChkFrontHs)
	ON_BN_CLICKED(IDC_CHK_FRONT_HS2, OnChkFrontHs2)
	ON_BN_CLICKED(IDC_CHK_FRONT_HS3, OnChkFrontHs3)
	ON_BN_CLICKED(IDC_CHK_FRONT_HS4, OnChkFrontHs4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_buffer message handlers

BOOL CDialog_Work_buffer::OnInitDialog() 
{
	CDialog::OnInitDialog();

	mp_font = NULL;
	mp_font = new CFont;
	mp_font->CreateFont(15,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	
	Init_Data();
	OnWorkBuffer_Button_Set();
	Display_Moduleata();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Work_buffer::Init_Data()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		nWorkBuffer[i][1] = st_modulemap.WorkBuffer[D_EXIST][i][LDMODULE_SITE];
		nFrontHs[i][1] = st_modulemap.WorkBuffer[D_EXIST][i][LEFTSINK_SITE];
		nRearHs[i][1] = st_modulemap.WorkBuffer[D_EXIST][i][RIGHTSINK_SITE];
	}
}

void CDialog_Work_buffer::OnWorkBuffer_Button_Set()
{
	short	shBtnColor = 30;

	m_chk_front_hs1.SetFont(mp_font);
 	m_chk_front_hs1.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
	m_chk_front_hs1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_front_hs1.SetButtonColor(1);

	m_chk_front_hs2.SetFont(mp_font);
 	m_chk_front_hs2.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
	m_chk_front_hs2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_front_hs2.SetButtonColor(1);

	m_chk_front_hs3.SetFont(mp_font);
 	m_chk_front_hs3.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
	m_chk_front_hs3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_front_hs3.SetButtonColor(1);
	
	m_chk_front_hs4.SetFont(mp_font);
 	m_chk_front_hs4.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_front_hs4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_front_hs4.SetButtonColor(1);

	m_chk_module1.SetFont(mp_font);
 	m_chk_module1.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_module1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_module1.SetButtonColor(1);	

	m_chk_module2.SetFont(mp_font);
 	m_chk_module2.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_module2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_module2.SetButtonColor(1);	

	m_chk_module3.SetFont(mp_font);
 	m_chk_module3.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_module3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_module3.SetButtonColor(1);

	m_chk_module4.SetFont(mp_font);
 	m_chk_module4.SetIcon(IDI_ENABLE, IDI_DISABLE);		//(IDI_BCR_ON, IDI_BCR_OFF);
 	m_chk_module4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_module4.SetButtonColor(1);	

	m_chk_back_hs1.SetFont(mp_font);
 	m_chk_back_hs1.SetIcon(IDI_ENABLE, IDI_DISABLE);
	m_chk_back_hs1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_back_hs1.SetButtonColor(1);

	m_chk_back_hs2.SetFont(mp_font);
 	m_chk_back_hs2.SetIcon(IDI_ENABLE, IDI_DISABLE);
	m_chk_back_hs2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_back_hs2.SetButtonColor(1);

	m_chk_back_hs3.SetFont(mp_font);
 	m_chk_back_hs3.SetIcon(IDI_ENABLE, IDI_DISABLE);
	m_chk_back_hs3.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_back_hs3.SetButtonColor(1);
	
	m_chk_back_hs4.SetFont(mp_font);
 	m_chk_back_hs4.SetIcon(IDI_ENABLE, IDI_DISABLE);
 	m_chk_back_hs4.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
 	m_chk_back_hs4.SetButtonColor(1);
}

void CDialog_Work_buffer::Display_Moduleata()
{
	m_chk_front_hs1.SetCheck(nFrontHs[0][1]);
	m_chk_front_hs2.SetCheck(nFrontHs[1][1]);
	m_chk_front_hs3.SetCheck(nFrontHs[2][1]);
	m_chk_front_hs4.SetCheck(nFrontHs[3][1]);

	m_chk_back_hs1.SetCheck(nRearHs[0][1]);
	m_chk_back_hs2.SetCheck(nRearHs[1][1]);
	m_chk_back_hs3.SetCheck(nRearHs[2][1]);
	m_chk_back_hs4.SetCheck(nRearHs[3][1]);

	m_chk_module1.SetCheck(nWorkBuffer[0][1]);
	m_chk_module2.SetCheck(nWorkBuffer[1][1]);
	m_chk_module3.SetCheck(nWorkBuffer[2][1]);
	m_chk_module4.SetCheck(nWorkBuffer[3][1]);
}

void CDialog_Work_buffer::OnWorkBuffer_Data_Recovery()
{
	int i;
	for (i = 0;i < 4; i++)
	{
		nFrontHs[i][1]	  = nFrontHs[i][0];
		nRearHs[i][1]	  = nRearHs[i][0];
		nWorkBuffer[i][1] = nWorkBuffer[i][0];
	}
}

int CDialog_Work_buffer::OnWorkBuffer_Data_Comparison()
{
	int i,Ret;

	for (i = 0;i < 4; i++)
	{
		if (nFrontHs[i][0]		!= nFrontHs[i][1])
		{
			Ret = RET_ERROR;
			break;
		}
		if (nRearHs[i][0]		!= nRearHs[i][1])
		{
			Ret = RET_ERROR;
			break;
		}

		if (nWorkBuffer[i][0]	!= nWorkBuffer[i][1])
		{
			Ret = RET_ERROR;
			break;
		}
	}

	return Ret;
}

void CDialog_Work_buffer::OnWorkBuffer_Data_Apply()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		st_modulemap.WorkBuffer[D_EXIST][i][LDMODULE_SITE] = nWorkBuffer[i][1];
		st_modulemap.WorkBuffer[D_EXIST][i][LEFTSINK_SITE] = nFrontHs[i][1];
		st_modulemap.WorkBuffer[D_EXIST][i][RIGHTSINK_SITE] = nRearHs[i][1];
	}
}

void CDialog_Work_buffer::OnWorkBuffer_LogFile_Create()
{
	int i;
	CString str_msg;
	for (i = 0; i < 4; i++)
	{
		if (nFrontHs[i][0] != nFrontHs[i][1])
		{
			str_msg.Format("[Front Hs[%d]] %d was changed by %d", i, nFrontHs[i][0], nFrontHs[i][1]);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
		if (nWorkBuffer[i][0] != nWorkBuffer[i][1])
		{
			str_msg.Format("[Module [%d]] %d was changed by %d", i, nWorkBuffer[i][0], nWorkBuffer[i][1]);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
		if (nRearHs[i][0] != nRearHs[i][1])
		{
			str_msg.Format("[Rear Hs[%d]] %d was changed by %d", i, nRearHs[i][0], nRearHs[i][1]);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
	}
}

void CDialog_Work_buffer::OnWorkBuffer_Data_BackUp()
{
	int i;
	for (i = 0;i < 4; i++)
	{
		nFrontHs[i][0]	  = nFrontHs[i][1];
		nRearHs[i][0]	  = nRearHs[i][1];
		nWorkBuffer[i][0] = nWorkBuffer[i][1];
	}
}

void CDialog_Work_buffer::OnApply() 
{
	int n_response;
	CDialog_Select  select_dlg;

	st_msg.mstr_confirm_msg = _T("Work Buffer : Setting Data Apply?");

	n_response = select_dlg.DoModal();

	if (n_response == IDOK)
	{
		OnWorkBuffer_Data_Comparison();
		OnWorkBuffer_Data_Apply();	
		OnWorkBuffer_LogFile_Create();
		OnWorkBuffer_Data_BackUp();
	}
	else if (n_response == IDCANCEL)
	{

	}
}

void CDialog_Work_buffer::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}


BOOL CDialog_Work_buffer::DestroyWindow() 
{
	delete mp_font;
	mp_font = NULL;
	
	return CDialog::DestroyWindow();
}

void CDialog_Work_buffer::OnChkBackHs() 
{
	if(m_chk_back_hs1.GetCheck() == 1)
	{
		nRearHs[0][1] = 1;
	}
	else
	{
		nRearHs[0][1] = 0;
	}
	OnWorkBuffer_Change();		
}

void CDialog_Work_buffer::OnChkBackHs2() 
{
	if(m_chk_back_hs2.GetCheck() == 1)
	{
		nRearHs[1][1] = 1;
	}
	else
	{
		nRearHs[1][1] = 0;
	}
	OnWorkBuffer_Change();		
}


void CDialog_Work_buffer::OnChkBackHs3() 
{
	if(m_chk_back_hs3.GetCheck() == 1)
	{
		nRearHs[2][1] = 1;
	}
	else
	{
		nRearHs[2][1] = 0;
	}
	OnWorkBuffer_Change();
}

void CDialog_Work_buffer::OnChkBackHs4() 
{
	if(m_chk_back_hs4.GetCheck() == 1)
	{
		nRearHs[3][1] = 1;
	}
	else
	{
		nRearHs[3][1] = 0;
	}
	OnWorkBuffer_Change();	
}


void CDialog_Work_buffer::OnChkModule() 
{
	if(m_chk_module1.GetCheck() == 1)
	{
		nWorkBuffer[0][1] = 1;
	}
	else
	{
		nWorkBuffer[0][1] = 0;
	}
	OnWorkBuffer_Change();		
}

void CDialog_Work_buffer::OnChkModule2() 
{
	if(m_chk_module2.GetCheck() == 1)
	{
		nWorkBuffer[1][1] = 1;
	}
	else
	{
		nWorkBuffer[1][1] = 0;
	}
	OnWorkBuffer_Change();		
	
}

void CDialog_Work_buffer::OnChkModule3() 
{
	if(m_chk_module3.GetCheck() == 1)
	{
		nWorkBuffer[2][1] = 1;
	}
	else
	{
		nWorkBuffer[2][1] = 0;
	}
	OnWorkBuffer_Change();		
}

void CDialog_Work_buffer::OnChkModule4() 
{
	if(m_chk_module4.GetCheck() == 1)
	{
		nWorkBuffer[3][1] = 1;
	}
	else
	{
		nWorkBuffer[3][1] = 0;
	}
	OnWorkBuffer_Change();	
}

void CDialog_Work_buffer::OnChkFrontHs() 
{
	if(m_chk_front_hs1.GetCheck() == 1)
	{
		nFrontHs[0][1] = 1;
	}
	else
	{
		nFrontHs[0][1] = 0;
	}
	OnWorkBuffer_Change();		
}

void CDialog_Work_buffer::OnChkFrontHs2() 
{
	if(m_chk_front_hs2.GetCheck() == 1)
	{
		nFrontHs[1][1] = 1;
	}
	else
	{
		nFrontHs[1][1] = 0;
	}
	OnWorkBuffer_Change();		
}

void CDialog_Work_buffer::OnChkFrontHs3() 
{
	if(m_chk_front_hs3.GetCheck() == 1)
	{
		nFrontHs[2][1] = 1;
	}
	else
	{
		nFrontHs[2][1] = 0;
	}
	OnWorkBuffer_Change();	
}

void CDialog_Work_buffer::OnChkFrontHs4() 
{
	if(m_chk_front_hs4.GetCheck() == 1)
	{
		nFrontHs[3][1] = 1;
	}
	else
	{
		nFrontHs[3][1] = 0;
	}
	OnWorkBuffer_Change();	
}

void CDialog_Work_buffer::OnWorkBuffer_Change()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if(nRearHs[i][1] < 0 || nRearHs[i][1] > 1)	nRearHs[i][1] = 1;
		if(nWorkBuffer[i][1] < 0 || nWorkBuffer[i][1] > 1)	nWorkBuffer[i][1] = 1;
		if(nFrontHs[i][1] < 0 || nFrontHs[i][1] > 1)	nFrontHs[i][1] = 1;
	}
}


