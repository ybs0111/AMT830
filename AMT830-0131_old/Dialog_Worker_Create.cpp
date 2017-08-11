// Dialog_Worker_Create.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Worker_Create.h"
#include "KeyboardDll.h"
#include "Dialog_KeyBoard.h"
#include "Public_Function.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Worker_Create dialog


CDialog_Worker_Create::CDialog_Worker_Create(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Worker_Create::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Worker_Create)
	//}}AFX_DATA_INIT
	m_str_name		= "default";
	m_str_crem		= "A";
	m_str_number	= "00000";
}


void CDialog_Worker_Create::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Worker_Create)
	DDX_Control(pDX, IDC_GROUP_WORKER, m_group_worker);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btn_cancel);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btn_apply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Worker_Create, CDialog)
	//{{AFX_MSG_MAP(CDialog_Worker_Create)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Worker_Create message handlers

BOOL CDialog_Worker_Create::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");

	Init_Label();
	Init_Group();
	Init_Grid_Worker();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialog_Worker_Create::Init_Label()
{
	
}

void CDialog_Worker_Create::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}
}

void CDialog_Worker_Create::OnBtnApply() 
{
	// TODO: Add your control notification handler code here
	struct st_worker_db st_db;
	int crem;

	m_grid_worker = (TSpread*)GetDlgItem(IDC_CUSTOM_WORKER);
	crem			= atoi(m_p_grid.GridCellText(m_grid_worker, 2, 2));

	switch(crem)
	{
		case 0:
			m_str_crem = "A";
			break;

		case 1:
			m_str_crem = "B";
			break;

		case 2:
			m_str_crem = "C";
			break;

		case 3:
			m_str_crem = "D";
			break;

		case 4:
			m_str_crem = "E";
			break;

		case 5:
			m_str_crem = "F";
			break;

		case 6:
			m_str_crem = "G";
			break;
	}

	st_db.time = CTime::GetCurrentTime();
	
	st_db.str_worker_name	= m_str_name;	// 작업자 번호.....
	st_db.str_worker_crem	= m_str_crem;	// 작업자 조......
	st_db.str_worker_no		= m_str_number;		// 사원 번호......

	Func.Database_Worker(st_db);
}

void CDialog_Worker_Create::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CDialog_Worker_Create::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_worker.SetFont(main_font);
	m_group_worker.SetCatptionTextColor(BLUE_C);
	m_group_worker.SetBorderColor(YELLOW_L);
	m_group_worker.SetFontBold(TRUE);
}


void CDialog_Worker_Create::Init_Grid_Worker()
{
	int i, j;
	int max_row, max_col;
	
	UpdateData(TRUE);
	
	m_grid_worker = (TSpread*)GetDlgItem(IDC_CUSTOM_WORKER);
	
	m_p_grid.GridReset(m_grid_worker);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_worker, TRUE);
	m_p_grid.GridRowHeader(m_grid_worker, FALSE);
	m_p_grid.GridColHeader(m_grid_worker, FALSE);
	m_p_grid.GridHorizontal(m_grid_worker, FALSE);
	m_p_grid.GridVertical(m_grid_worker, FALSE);
	m_p_grid.GridAutoSize(m_grid_worker, FALSE);
	
	max_row = 3;
	max_col = 2;
	
	m_p_grid.GridCellRows(m_grid_worker, max_row);
	m_p_grid.GridCellCols(m_grid_worker, max_col);
	
	for(i=0; i<=max_row+1; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_worker, i, (double)30);
		
		for(j=0; j<max_col; j++)
		{
			switch(j)
			{	
				case 0:
					m_p_grid.GridCellWidth_L(m_grid_worker, j+1, 25);
					break;
					
				case 1:
					m_p_grid.GridCellWidth_L(m_grid_worker, j+1, 25);
					break;
			}
			
			m_p_grid.GridCellControlStatic(m_grid_worker, i, j+1);
			m_p_grid.GridCellFont(m_grid_worker, i, j+1, "MS Sans Serif", 12);
		}
	}
	
	m_p_grid.GridCellColor(m_grid_worker, 1, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_worker, 1, 1, "Name");

	m_p_grid.GridCellColor(m_grid_worker, 1, 2, WHITE_C, BLACK_C);
	m_p_grid.GridCellText(m_grid_worker, 1, 2, "-");
	
	m_p_grid.GridCellColor(m_grid_worker, 2, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_worker, 2, 1, "Crem");

	m_p_grid.GridCellColor(m_grid_worker, 2, 2, WHITE_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_worker, 2, 2, "MS Sans Serif", 28);
	m_p_grid.GridCellControlCombo(m_grid_worker, 2, 2, "A\tB\tC\tD\tE\tF\tG");
	m_p_grid.GridCellText(m_grid_worker, 2, 2, "1");
	
	m_p_grid.GridCellColor(m_grid_worker, 3, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_worker, 3, 1, "Number");

	m_p_grid.GridCellColor(m_grid_worker, 3, 2, WHITE_C, BLACK_C);
	m_p_grid.GridCellText(m_grid_worker, 3, 2, "000000");
}

void CDialog_Worker_Create::OnCellClick(WPARAM wParam, LPARAM lParam)
{
 	int n_response;									// 대화 상자 리턴 플래그
	CDialog_Keyboard  keyboard_dlg ;

	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	
	if(wParam == IDC_CUSTOM_WORKER)
	{
		m_grid_worker = (TSpread*)GetDlgItem(IDC_CUSTOM_WORKER);
		if(lpcc->Row == 1)
		{
			(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
			
			st_msg.mstr_typing_msg = m_str_name;
			
			n_response = keyboard_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				m_str_name = st_msg.mstr_typing_msg;
				m_p_grid.GridCellText(m_grid_worker, lpcc->Row, 2, m_str_name);
			}
			else
			{
				return;
			}

		}
		else if(lpcc->Row == 3)
		{
			(st_msg.mstr_typing_msg).Empty();				// 키보드 상자 출력 데이터 초기화
			
			st_msg.mstr_typing_msg = m_str_number;
			
			n_response = keyboard_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				m_str_number = st_msg.mstr_typing_msg;
				m_p_grid.GridCellText(m_grid_worker, lpcc->Row, 2, m_str_number);
			}
			else
			{
				return;
			}

		}
	}



}