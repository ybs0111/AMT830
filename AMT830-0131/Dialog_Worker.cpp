// Dialog_Worker.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Worker.h"
#include "Dialog_Worker_Create.h"
#include "Dialog_Select.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Worker dialog


CDialog_Worker::CDialog_Worker(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Worker::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Worker)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_n_pos = -1;
}


void CDialog_Worker::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Worker)
	DDX_Control(pDX, IDC_BTN_WORKER_DELETE, m_btn_worker_delete);
	DDX_Control(pDX, IDC_BTN_WORKER_SELECT, m_btn_worker_select);
	DDX_Control(pDX, IDC_BTN_WORKER_CREATE, m_btn_worker_create);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Worker, CDialog)
	//{{AFX_MSG_MAP(CDialog_Worker)
	ON_BN_CLICKED(IDC_BTN_WORKER_CREATE, OnBtnWorkerCreate)
	ON_BN_CLICKED(IDC_BTN_WORKER_SELECT, OnBtnWorkerSelect)
	ON_BN_CLICKED(IDC_BTN_WORKER_DELETE, OnBtnWorkerDelete)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Worker message handlers

BOOL CDialog_Worker::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Init_Grid_Worker();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Worker::Init_Grid_Worker()
{
	int i, j;
	int max_row, max_col;
	
	UpdateData(TRUE);
	
	m_grid_worker = (TSpread*)GetDlgItem(IDC_CUSTOM_WORKER);
	
	m_p_grid.GridReset(m_grid_worker);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_worker, TRUE);
	m_p_grid.GridRowHeader(m_grid_worker, FALSE);
	m_p_grid.GridColHeader(m_grid_worker, TRUE);
	m_p_grid.GridHorizontal(m_grid_worker, TRUE);
	m_p_grid.GridVertical(m_grid_worker, TRUE);
	m_p_grid.GridAutoSize(m_grid_worker, FALSE);
	
	max_row = 50;
	max_col = 4;
	
	m_p_grid.GridCellRows(m_grid_worker, max_row);
	m_p_grid.GridCellCols(m_grid_worker, max_col);
	
	for(i=0; i<=max_row+1; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_worker, i, (double)20);
		
		for(j=0; j<max_col; j++)
		{
			switch(j)
			{
				case 0:
					m_p_grid.GridCellWidth_L(m_grid_worker, j+1, 10);
					break;
					
				case 1:
					m_p_grid.GridCellWidth_L(m_grid_worker, j+1, 20);
					break;
					
				case 2:
					m_p_grid.GridCellWidth_L(m_grid_worker, j+1, 10);
					break;
					
				case 3:
					m_p_grid.GridCellWidth_L(m_grid_worker, j+1, 30);
					break;
			}
			
			m_p_grid.GridCellControlStatic(m_grid_worker, i, j+1);
			m_p_grid.GridCellFont(m_grid_worker, i, j+1, "MS Sans Serif", 12);
		}
	}
	
	m_p_grid.GridCellColor(m_grid_worker, 0, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_worker, 0, 1, "No.");
	
	m_p_grid.GridCellColor(m_grid_worker, 0, 2, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_worker, 0, 2, "Name");
	
	m_p_grid.GridCellColor(m_grid_worker, 0, 3, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_worker, 0, 3, "Crem");
	
	m_p_grid.GridCellColor(m_grid_worker, 0, 4, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(m_grid_worker, 0, 4, "Number");


	int count	= 0;
	CTime time	= CTime::GetCurrentTime();

	if(Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if(Mysql.Database_Select("AMT830"))
		{	
			m_oledate[0].SetDateTime(time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
			m_oledate[1].SetDateTime(time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

			count = Mysql.Record_Date_Search("WORKER_DB", "DATE", ">", m_oledate);
				
			if(count > 0)
			{
				for(i=0; i<count; i++)
				{
					Display_Worker(i);
				}
			}
		}
	}
	Mysql.MySql_Close();
}

void CDialog_Worker::Display_Worker(int n_count)
{
	CString strTmp[5], strData, str_count;
	int		length, i;
	int		pos[2];
	
	m_grid_worker = (TSpread*)GetDlgItem(IDC_CUSTOM_WORKER);
	
	
	strData = Mysql.Record_Read();
	// lot_no
	length = strData.GetLength();
	pos[0] = strData.Find(",", 0);
	
	for(i=0; i<Mysql.m_n_worker_table_cnt; i++)
	{	
		pos[1] = strData.Find(",", pos[0]+1);
		
		if(pos[1] > 0)
		{
			strTmp [i]= strData.Mid(pos[0]+1, (pos[1] - pos[0])-1);
			pos[0] = pos[1];
		}
		else
		{
			strTmp[i] = strData.Mid(pos[0]+1, (length - pos[1])-1);
		}
	}
	
	str_count.Format("%d", n_count+1);
	
	if(m_str_name == strTmp[1] && m_str_no == strTmp[3])
	{
		m_p_grid.GridCellColor(m_grid_worker, n_count+1, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_worker, n_count+1, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_worker, n_count+1, 3, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_worker, n_count+1, 4, GREEN_C, BLACK_C);

		m_n_pos = n_count+1;
	}

	m_p_grid.GridCellText(m_grid_worker, n_count+1, 1, str_count); // no
	m_p_grid.GridCellText(m_grid_worker, n_count+1, 2, strTmp[1]); // date
	m_p_grid.GridCellText(m_grid_worker, n_count+1, 3, strTmp[2]); // 
	m_p_grid.GridCellText(m_grid_worker, n_count+1, 4, strTmp[3]); //  
	m_p_grid.GridCellScroll(m_grid_worker, n_count+1);
}

void CDialog_Worker::OnBtnWorkerCreate() 
{
	// TODO: Add your control notification handler code here
	CDialog_Worker_Create dlg;

	dlg.DoModal();

	Init_Grid_Worker();
}

void CDialog_Worker::OnBtnWorkerSelect() 
{
	// TODO: Add your control notification handler code here
	st_other.b_m_exit = FALSE;
	CDialog::OnOK();	
}

void CDialog_Worker::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;

	if(wParam == IDC_CUSTOM_WORKER)
	{
		if(lpcc->Row < 1) return;

		m_grid_worker = (TSpread*)GetDlgItem(IDC_CUSTOM_WORKER);
		
		m_str_name	= m_p_grid.GridCellText(m_grid_worker, lpcc->Row, 2);
		m_str_crem	= m_p_grid.GridCellText(m_grid_worker, lpcc->Row, 3);
		m_str_no	= m_p_grid.GridCellText(m_grid_worker, lpcc->Row, 4);

		m_p_grid.GridCellColor(m_grid_worker, lpcc->Row, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_worker, lpcc->Row, 2, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_worker, lpcc->Row, 3, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_worker, lpcc->Row, 4, GREEN_C, BLACK_C);

		if(m_n_pos != -1)
		{
			m_p_grid.GridCellColor(m_grid_worker, m_n_pos, 1, WHITE_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_worker, m_n_pos, 2, WHITE_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_worker, m_n_pos, 3, WHITE_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_worker, m_n_pos, 4, WHITE_C, BLACK_C);
		}

		m_n_pos		= lpcc->Row;
	}
}

void CDialog_Worker::OnBtnWorkerDelete() 
{
	// TODO: Add your control notification handler code here
	CDialog_Select select_dlg;
	int n_response;

	st_other.str_confirm_msg.Format("[%s] 작업자를 삭제하시겠습니까?..", m_str_name);
	if(st_handler.mn_language == LANGUAGE_ENGLISH)
	{
		st_other.str_confirm_msg.Format("[%s] Are you sure you want to delete the operator?..", m_str_name);
	}
	
	n_response = select_dlg.DoModal();
	
	if(n_response == IDOK)	
	{
		if(Mysql.MySql_Open("localhost", "root", "1111", 3306))
		{
			if(Mysql.Database_Select("AMT830"))
			{	
				Mysql.Record_Text_Delete("WORKER_DB", "WORKER_NAME", m_str_name);

				m_n_pos = -1;
			}
		}
		Mysql.MySql_Close();
	}

	Init_Grid_Worker();
}

void CDialog_Worker::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	st_other.b_m_exit = TRUE;
	CDialog::OnOK();
}
