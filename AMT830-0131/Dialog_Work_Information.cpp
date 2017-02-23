// Dialog_Work_Information.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Work_Information.h"

#include "Dialog_Message.h"
#include "Dialog_Select.h"
#include "CtlBd_Library.h"
#include "FastechPublic_IO.h"

#include "Run_Rbt_Load_Module.h"
#include "Run_Rbt_Load_LeftHeat.h"
#include "Run_Rbt_Load_RightHeat.h"
#include "Run_Rbt_Work_Buffer.h"
#include "Run_Rbt_Sorter.h"
#include "Run_Rbt_Load_Clip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Information dialog


CDialog_Work_Information::CDialog_Work_Information(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Work_Information::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Work_Information)
	//}}AFX_DATA_INIT
}


void CDialog_Work_Information::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Work_Information)
	DDX_Control(pDX, ID_CLIP_RBT_INIT, m_clip_clamp_rbt);
	DDX_Control(pDX, IDOK, m_btn_ok);
	DDX_Control(pDX, ID_APPLY, m_btn_apply);
	DDX_Control(pDX, IDC_MSG_WARNING, m_msg_warnning);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Work_Information, CDialog)
	//{{AFX_MSG_MAP(CDialog_Work_Information)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_STOP, OnStop)
	ON_BN_CLICKED(ID_CLIP_RBT_INIT, OnClipRbtInit)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Work_Information message handlers

BOOL CDialog_Work_Information::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	init_Label();
	Init_Data();
	Init_Grid_Info();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Work_Information::init_Label()
{
	m_msg_warnning.SetNumberOfLines(1);
	m_msg_warnning.SetXCharsPerLine(40);
	m_msg_warnning.SetSize(CMatrixStatic::LARGE);
	m_msg_warnning.SetDisplayColors(RGB(0, 0, 0), RGB(60, 255, 0), RGB(103, 30, 0));
	m_msg_warnning.AdjustClientXToSize(40);
	m_msg_warnning.AdjustClientYToSize(1);
	m_msg_warnning.SetAutoPadding(true, '.');
	m_msg_warnning.DoScroll(100, CMatrixStatic::LEFT);
	m_msg_warnning.SetWindowPos( &wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE );
	m_msg_warnning.ShowWindow(FALSE);
	m_msg_warnning.SetText(_T("Operation... Wait Please.."));

	mb_display_opration = FALSE;
}

void CDialog_Work_Information::Init_Data()
{
	int i,j,k;

	st_handler.n_module_skip = NO;
	st_handler.n_fronths_skip = NO;
	st_handler.n_rearhs_skip = NO;
	st_handler.n_clipbuffer_skip = NO;
	st_handler.n_cliprbtpicker_skip = NO;
	st_handler. n_clamprbt_skip = NO;
	st_handler.n_sorter_skip = NO;
	st_handler.n_unload_skip = NO;
	st_handler.n_vision_skip = NO;

	m_b_strop_req = FALSE;

	for (i = 0; i < 4; i++)
	{
		st_handler.n_workbuff_skip[i] = NO;
	}

	for (i = 0; i < MOT_MAXMOTOR; i++)
	{
		st_handler.n_motor_init[MOT_MAXMOTOR] = CTL_NO;
		n_motor_init[i][1] = CTL_NO;

		n_motor_init[i][1] = CTL_YES;
	}
	
	for(i = 0; i <D_INFOSIZE; i++)
	{
		for (j = 0; j < PICKER_NUM; j++)
		{
			nModuleRobotPicker[i][j][1] = st_modulemap.nModuleRobotPicker[i][j];
			nLefthsRobotPicker[i][j][1] = st_modulemap.nLefthsRobotPicker[i][j];
			nRighthsRobotPicker[i][j][1] = st_modulemap.nRighthsRobotPicker[i][j];
		}
		nClipRobotPicker[i][1] = st_modulemap.nClipRobotPicker[i];
		nSortRobotPicker[i][1] = st_modulemap.nSortRobotPicker[i];
		nUldRobotPicker[i][1] = st_modulemap.nUldRobotPicker[i];

		for (j = 0; j < 2; j++)
		{
			VisionBuffer[i][j][1] = st_modulemap.VisionBuffer[i][j];
		}

		for (j = 0; j < 4; j ++)//Work1234
		{
			for (k = 0; k < 5; k++) //LDMODULE,LEFT,RIGHT,CLIP,sorter
			{
				WorkBuffer[i][j][k][1] = st_modulemap.WorkBuffer[i][j][k];
			}
		}


	}
	InitSortStep = 0;
	ClipInitStep = 0;
}

void CDialog_Work_Information::Init_Grid_Info()
{
	int   i, j;
	CString str_tmp;

	TSpread *Grid[4]; 
	Grid[0] = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_INFORMATION);
	
	m_p_grid.GridReset(Grid[0]);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(Grid[0], TRUE);
	
	m_p_grid.GridRowHeader(Grid[0], FALSE);
	m_p_grid.GridColHeader(Grid[0], FALSE);
	m_p_grid.GridHorizontal(Grid[0], FALSE);
	m_p_grid.GridVertical(Grid[0], FALSE);
	m_p_grid.GridAutoSize(Grid[0], FALSE);
	m_p_grid.GridAutoSize(Grid[0], FALSE);
	m_p_grid.GridCellRows(Grid[0], 4);
	m_p_grid.GridCellCols(Grid[0], 4);

	for(i=0; i<4; i++)
	{
		m_p_grid.GridCellHeight_L(Grid[0], i+1, 20);
		for(j=0; j<4; j++)
		{
			m_p_grid.GridCellWidth_L(Grid[0], j+1, 20);
	
			m_p_grid.GridCellControlStatic(Grid[0], i+1, j+1);
		}
	}

	m_p_grid.GridCellMerge(Grid[0], 1, 1, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 1, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 1, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[0], 1, 1, "name");

	m_p_grid.GridCellMerge(Grid[0], 1, 2, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 1, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 1, 2, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[0], 1, 2, "Front");

	m_p_grid.GridCellMerge(Grid[0], 1, 3, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 1, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 1, 3, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[0], 1, 3, "Rear");

	m_p_grid.GridCellMerge(Grid[0], 1, 4, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 1, 4, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 1, 4, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[0], 1, 4, "초기화");

	m_p_grid.GridCellMerge(Grid[0], 2, 1, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 2, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[0], 2, 1, "Module Picker");

	m_p_grid.GridCellMerge(Grid[0], 2, 2, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 2, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 2, 2, BLUE_D, WHITE_C);
	str_tmp.Format("%d",nModuleRobotPicker[D_EXIST][0][1]);
	m_p_grid.GridCellText(Grid[0], 2, 2, str_tmp);

	m_p_grid.GridCellMerge(Grid[0], 2, 3, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 2, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 2, 3, BLUE_D, WHITE_C);
	str_tmp.Format("%d",nModuleRobotPicker[D_EXIST][1][1]);
	m_p_grid.GridCellText(Grid[0], 2, 3, str_tmp);

	m_p_grid.GridCellMerge(Grid[0], 2, 4, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 2, 4, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 2, 4, BLUE_D, WHITE_C);
	str_tmp.Format("%d",n_motor_init[LDRBTMODULE_SITE][1]);
	m_p_grid.GridCellText(Grid[0], 2, 4, str_tmp);

	m_p_grid.GridCellMerge(Grid[0], 3, 1, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 3, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 3, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[0], 3, 1, "Left Module Picker");

	m_p_grid.GridCellMerge(Grid[0], 3, 2, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 3, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 3, 2, BLUE_D, WHITE_C);
	str_tmp.Format("%d",nLefthsRobotPicker[D_EXIST][0][1]);
	m_p_grid.GridCellText(Grid[0], 3, 2, str_tmp);

	m_p_grid.GridCellMerge(Grid[0], 3, 3, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 3, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 3, 3, BLUE_D, WHITE_C);
	str_tmp.Format("%d",nLefthsRobotPicker[D_EXIST][1][1]);
	m_p_grid.GridCellText(Grid[0], 3, 3, str_tmp);

	m_p_grid.GridCellMerge(Grid[0], 3, 4, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 3, 4, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 3, 4, BLUE_D, WHITE_C);
	str_tmp.Format("%d",n_motor_init[LEFTRBTSINK_SITE][1]);
	m_p_grid.GridCellText(Grid[0], 3, 4, str_tmp);

	m_p_grid.GridCellMerge(Grid[0], 4, 1, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 4, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 4, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[0], 4, 1, "Right Module Picker");

	m_p_grid.GridCellMerge(Grid[0], 4, 2, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 4, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 4, 2, BLUE_D, WHITE_C);
	str_tmp.Format("%d",nRighthsRobotPicker[D_EXIST][0][1]);
	m_p_grid.GridCellText(Grid[0], 4, 2, str_tmp);

	m_p_grid.GridCellMerge(Grid[0], 4, 3, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 4, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 4, 3, BLUE_D, WHITE_C);
	str_tmp.Format("%d",nRighthsRobotPicker[D_EXIST][1][1]);
	m_p_grid.GridCellText(Grid[0], 4, 3, str_tmp);

	m_p_grid.GridCellMerge(Grid[0], 4, 4, 1, 1);
	m_p_grid.GridCellFont(Grid[0], 4, 4, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[0], 4, 4, BLUE_D, WHITE_C);
	str_tmp.Format("%d",n_motor_init[RIGHTRBTSINK_SITE][1]);
	m_p_grid.GridCellText(Grid[0], 4, 4, str_tmp);

	
	Grid[1] = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_CLIP);
	m_p_grid.GridReset(Grid[1]);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(Grid[1], TRUE);
	
	m_p_grid.GridRowHeader(Grid[1], FALSE);
	m_p_grid.GridColHeader(Grid[1], FALSE);
	m_p_grid.GridHorizontal(Grid[1], FALSE);
	m_p_grid.GridVertical(Grid[1], FALSE);
	m_p_grid.GridAutoSize(Grid[1], FALSE);
	m_p_grid.GridAutoSize(Grid[1], FALSE);
	m_p_grid.GridCellRows(Grid[1], 2);
	m_p_grid.GridCellCols(Grid[1], 3);
	

	for(i=0; i<2; i++)
	{
		m_p_grid.GridCellHeight_L(Grid[1], i+1, 20);
		for(j=0; j<3; j++)
		{
			m_p_grid.GridCellWidth_L(Grid[1], j+1, 18);	
			m_p_grid.GridCellControlStatic(Grid[1], i+1, j+1);
		}
	}

	m_p_grid.GridCellMerge(Grid[1], 1, 1, 1, 1);
	m_p_grid.GridCellFont(Grid[1], 1, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[1], 1, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[1], 1, 1, "name");

	m_p_grid.GridCellMerge(Grid[1], 1, 2, 1, 1);
	m_p_grid.GridCellFont(Grid[1], 1, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[1], 1, 2, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[1], 1, 2, "Clip");

	m_p_grid.GridCellMerge(Grid[1], 1, 3, 1, 1);
	m_p_grid.GridCellFont(Grid[1], 1, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[1], 1, 3, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[1], 1, 3, "초기화");

	m_p_grid.GridCellMerge(Grid[1], 2, 1, 1, 1);
	m_p_grid.GridCellFont(Grid[1], 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[1], 2, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[1], 2, 1, "ClipRbt");

	m_p_grid.GridCellMerge(Grid[1], 2, 2, 1, 1);
	m_p_grid.GridCellFont(Grid[1], 2, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[1], 2, 2, BLUE_D, WHITE_C);
	str_tmp.Format("%d",nClipRobotPicker[D_EXIST][1]);
	m_p_grid.GridCellText(Grid[1], 2, 2, str_tmp);

	m_p_grid.GridCellMerge(Grid[1], 2, 3, 1, 1);
	m_p_grid.GridCellFont(Grid[1], 2, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[1], 2, 3, BLUE_D, WHITE_C);
	str_tmp.Format("%d",n_motor_init[LDRBTCLIPINSERT_SITE][1]);
	m_p_grid.GridCellText(Grid[1], 2, 3, str_tmp);


	Grid[2] = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_BUFFER);
	
	m_p_grid.GridReset(Grid[2]);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(Grid[2], TRUE);
	
	m_p_grid.GridRowHeader(Grid[2], FALSE);
	m_p_grid.GridColHeader(Grid[2], FALSE);
	m_p_grid.GridHorizontal(Grid[2], FALSE);
	m_p_grid.GridVertical(Grid[2], FALSE);
	m_p_grid.GridAutoSize(Grid[2], FALSE);
	m_p_grid.GridAutoSize(Grid[2], FALSE);
	m_p_grid.GridCellRows(Grid[2], 5);
	m_p_grid.GridCellCols(Grid[2], 7);

	for(i=0; i<5; i++)
	{
		m_p_grid.GridCellHeight_L(Grid[2], i+1, 20);
		for(j=0; j<7; j++)
		{
			m_p_grid.GridCellWidth_L(Grid[2], j+1, 15);
	
			m_p_grid.GridCellControlStatic(Grid[2], i+1, j+1);
		}
	}

	m_p_grid.GridCellMerge(Grid[2], 1, 1, 1, 1);
	m_p_grid.GridCellFont(Grid[2], 1, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[2], 1, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[2], 1, 1, "name");

	m_p_grid.GridCellMerge(Grid[2], 1, 2, 1, 1);
	m_p_grid.GridCellFont(Grid[2], 1, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[2], 1, 2, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[2], 1, 2, "Front HS");

	m_p_grid.GridCellMerge(Grid[2], 1, 3, 1, 1);
	m_p_grid.GridCellFont(Grid[2], 1, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[2], 1, 3, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[2], 1, 3, "Module");

	m_p_grid.GridCellMerge(Grid[2], 1, 4, 1, 1);
	m_p_grid.GridCellFont(Grid[2], 1, 4, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[2], 1, 4, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[2], 1, 4, "Back HS");

	m_p_grid.GridCellMerge(Grid[2], 1, 5, 1, 1);
	m_p_grid.GridCellFont(Grid[2], 1, 5, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[2], 1, 5, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[2], 1, 5, "Clip");

	m_p_grid.GridCellMerge(Grid[2], 1, 6, 1, 1);
	m_p_grid.GridCellFont(Grid[2], 1, 6, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[2], 1, 6, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[2], 1, 6, "Sorter");

	m_p_grid.GridCellMerge(Grid[2], 1, 7, 1, 1);
	m_p_grid.GridCellFont(Grid[2], 1, 7, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[2], 1, 7, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[2], 1, 7, "초기화");

	for (i = 0; i < 4; i ++)
	{
		m_p_grid.GridCellMerge(Grid[2], 2+i, 1, 1, 1);
		m_p_grid.GridCellFont(Grid[2], 2+i, 1, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(Grid[2], 2+i, 1, BLUE_D, WHITE_C);
		str_tmp.Format("WorkBuffer#%d", i+1);
		m_p_grid.GridCellText(Grid[2], 2+i, 1, str_tmp);

		m_p_grid.GridCellMerge(Grid[2], 2+i, 2, 1, 1);
		m_p_grid.GridCellFont(Grid[2], 2+i, 2, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(Grid[2], 2+i, 2, BLUE_D, WHITE_C);
		str_tmp.Format("%d",WorkBuffer[D_EXIST][WORK_BUFFER_1 + i][LEFTSINK_SITE][1]);
		m_p_grid.GridCellText(Grid[2], 2+i, 2, str_tmp);

		m_p_grid.GridCellMerge(Grid[2], 2+i, 3, 1, 1);
		m_p_grid.GridCellFont(Grid[2], 2+i, 3, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(Grid[2], 2+i, 3, BLUE_D, WHITE_C);
		str_tmp.Format("%d",WorkBuffer[D_EXIST][WORK_BUFFER_1 + i][LDMODULE_SITE][1]);
		m_p_grid.GridCellText(Grid[2], 2+i, 3, str_tmp);

		m_p_grid.GridCellMerge(Grid[2], 2+i, 4, 1, 1);
		m_p_grid.GridCellFont(Grid[2], 2+i, 4, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(Grid[2], 2+i, 4, BLUE_D, WHITE_C);
		str_tmp.Format("%d",WorkBuffer[D_EXIST][WORK_BUFFER_1 + i][RIGHTSINK_SITE][1]);
		m_p_grid.GridCellText(Grid[2], 2+i, 4, str_tmp);

		m_p_grid.GridCellMerge(Grid[2], 2+i, 5, 1, 1);
		m_p_grid.GridCellFont(Grid[2], 2+i, 5, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(Grid[2], 2+i, 5, BLUE_D, WHITE_C);
		str_tmp.Format("%d",WorkBuffer[D_EXIST][WORK_BUFFER_1 + i][LDCLIP_SITE][1]);
		m_p_grid.GridCellText(Grid[2], 2+i, 5, str_tmp);

		m_p_grid.GridCellMerge(Grid[2], 2+i, 6, 1, 1);
		m_p_grid.GridCellFont(Grid[2], 2+i, 6, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(Grid[2], 2+i, 6, BLUE_D, WHITE_C);
		str_tmp.Format("%d",WorkBuffer[D_EXIST][WORK_BUFFER_1 + i][SORTER_SITE][1]);
		m_p_grid.GridCellText(Grid[2], 2+i, 6, str_tmp);
		
		m_p_grid.GridCellMerge(Grid[2], 2+i, 7, 1, 1);
		m_p_grid.GridCellFont(Grid[2], 2+i, 7, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(Grid[2], 2+i, 7, BLUE_D, WHITE_C);
		str_tmp.Format("%d",n_motor_init[WORK_BUFFER1_SITE + i][1]);
		m_p_grid.GridCellText(Grid[2], 2+i, 7, str_tmp);
	}

	Grid[3] = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_UNLOAD);
	
	m_p_grid.GridReset(Grid[3]);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(Grid[3], TRUE);
	
	m_p_grid.GridRowHeader(Grid[3], FALSE);
	m_p_grid.GridColHeader(Grid[3], FALSE);
	m_p_grid.GridHorizontal(Grid[3], FALSE);
	m_p_grid.GridVertical(Grid[3], FALSE);
	m_p_grid.GridAutoSize(Grid[3], FALSE);
	m_p_grid.GridAutoSize(Grid[3], FALSE);
	m_p_grid.GridCellRows(Grid[3], 5);
	m_p_grid.GridCellCols(Grid[3], 3);

	for(i=0; i<5; i++)
	{
		m_p_grid.GridCellHeight_L(Grid[3], i+1, 20);
		for(j=0; j<3; j++)
		{
			m_p_grid.GridCellWidth_L(Grid[3], j+1, 15);
	
			m_p_grid.GridCellControlStatic(Grid[3], i+1, j+1);
		}
	}

	m_p_grid.GridCellMerge(Grid[3], 1, 1, 1, 1);
	m_p_grid.GridCellFont(Grid[3], 1, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[3], 1, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[3], 1, 1, "name");

	m_p_grid.GridCellMerge(Grid[3], 1, 2, 1, 1);
	m_p_grid.GridCellFont(Grid[3], 1, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[3], 1, 2, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[3], 1, 2, "Module");

	m_p_grid.GridCellMerge(Grid[3], 1, 3, 1, 1);
	m_p_grid.GridCellFont(Grid[3], 1, 3, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(Grid[3], 1, 3, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid[3], 1, 3, "초기화");

	for (i = 0; i < 4; i ++)
	{
		m_p_grid.GridCellMerge(Grid[3], 2+i, 1, 1, 1);
		m_p_grid.GridCellFont(Grid[3], 2+i, 1, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(Grid[3], 2+i, 1, BLUE_D, WHITE_C);
		if	   (i == 0)	str_tmp = _T("Sorter Rbt");
		else if(i == 1)	str_tmp = _T("TOP Vision");
		else if(i == 2)	str_tmp = _T("BTM Vision");
		else			str_tmp = _T("Unload Rbt");
		m_p_grid.GridCellText(Grid[3], 2+i, 1, str_tmp);

		m_p_grid.GridCellMerge(Grid[3], 2+i, 2, 1, 1);
		m_p_grid.GridCellFont(Grid[3], 2+i, 2, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(Grid[3], 2+i, 2, BLUE_D, WHITE_C);
		if	   (i == 0)	str_tmp.Format("%d", nSortRobotPicker[D_EXIST][1]);
		else if(i == 1) str_tmp.Format("%d", VisionBuffer[D_EXIST][TOPBUFFER][1]);
		else if(i == 2) str_tmp.Format("%d", VisionBuffer[D_EXIST][BTMBUFFER][1]);
		else            str_tmp.Format("%d", nUldRobotPicker[D_EXIST][1]);
		m_p_grid.GridCellText(Grid[3], 2+i, 2, str_tmp);

		m_p_grid.GridCellMerge(Grid[3], 2+i, 3, 1, 1);
		m_p_grid.GridCellFont(Grid[3], 2+i, 3, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(Grid[3], 2+i, 3, BLUE_D, WHITE_C);
		str_tmp.Format("%d",n_motor_init[SORTERRBT_SITE + i][1]);
		m_p_grid.GridCellText(Grid[3], 2+i, 3, str_tmp);
	}


}

void CDialog_Work_Information::OnWorkInform_Data_BackUp()
{
	int i,j,k;
	for (i = 0; i < PICKER_NUM; i++)
	{
		nModuleRobotPicker[D_EXIST][i][0] = nModuleRobotPicker[D_EXIST][i][1];
		nLefthsRobotPicker[D_EXIST][i][0] = nLefthsRobotPicker[D_EXIST][i][1];
		nRighthsRobotPicker[D_EXIST][i][0] = nRighthsRobotPicker[D_EXIST][i][1];
	}
	nClipRobotPicker[D_EXIST][0] = nClipRobotPicker[D_EXIST][1];
	nSortRobotPicker[D_EXIST][0] = nSortRobotPicker[D_EXIST][1];
	nUldRobotPicker[D_EXIST][0] = nUldRobotPicker[D_EXIST][1];

	for (j = 0; j < 2; j++)
	{
		VisionBuffer[D_EXIST][j][0] = VisionBuffer[D_EXIST][j][1];
	}

	for (j = 0; j < 4; j ++)//Work1234
	{
		for (k = 0; k < 5; k++) //LDMODULE,LEFT,RIGHT,CLIP,sorter
		{
			WorkBuffer[D_EXIST][j][k][0] = WorkBuffer[D_EXIST][j][k][1];
		}
	}

}

void CDialog_Work_Information::OnWorkInform_LogFile_Create()
{
	int i,j;
	CString str_msg;
	for (i = 0; i < 4; i++)
	{
		if (n_motor_init[LDRBTMODULE_SITE][1] == CTL_YES)
		{
			if (nModuleRobotPicker[D_EXIST][i][0] != nModuleRobotPicker[D_EXIST][i][1])
			{
				str_msg.Format("[Module [%d]] %d was changed by %d", i, nModuleRobotPicker[0][i][0], nModuleRobotPicker[0][i][1]);
				Func.On_LogFile_Add(0, str_msg);
				Func.On_LogFile_Add(99, str_msg);
			}
		}
		if (n_motor_init[LEFTRBTSINK_SITE][1] == CTL_YES)
		{
			if (nLefthsRobotPicker[D_EXIST][i][0] != nLefthsRobotPicker[D_EXIST][i][1])
			{
				str_msg.Format("[Left Hs[%d]] %d was changed by %d", i, nLefthsRobotPicker[0][i][0], nLefthsRobotPicker[0][i][1]);
				Func.On_LogFile_Add(0, str_msg);
				Func.On_LogFile_Add(99, str_msg);
			}
		}
		if (n_motor_init[RIGHTRBTSINK_SITE][1] == CTL_YES)
		{
			if (nRighthsRobotPicker[D_EXIST][i][0] != nRighthsRobotPicker[D_EXIST][i][1])
			{
				str_msg.Format("[Right Hs[%d]] %d was changed by %d", i, nRighthsRobotPicker[0][i][0], nRighthsRobotPicker[0][i][1]);
				Func.On_LogFile_Add(0, str_msg);
				Func.On_LogFile_Add(99, str_msg);
			}
		}
	}

	str_msg.Format("[Clip[%d]] %d was changed by %d", i, nClipRobotPicker[0][0], nClipRobotPicker[0][1]);
	Func.On_LogFile_Add(0, str_msg);
	Func.On_LogFile_Add(99, str_msg);

	str_msg.Format("[Sort[%d]] %d was changed by %d", i, nSortRobotPicker[0][0], nSortRobotPicker[0][1]);
	Func.On_LogFile_Add(0, str_msg);
	Func.On_LogFile_Add(99, str_msg);

	str_msg.Format("[Unload[%d]] %d was changed by %d", i, nUldRobotPicker[0][0], nUldRobotPicker[0][1]);
	Func.On_LogFile_Add(0, str_msg);
	Func.On_LogFile_Add(99, str_msg);

	for (i = 0;i < 2; i++)
	{
		str_msg.Format("[Vision[%d]] %d was changed by %d", i, VisionBuffer[0][i][0], VisionBuffer[0][i][1]);
		Func.On_LogFile_Add(0, str_msg);
		Func.On_LogFile_Add(99, str_msg);
	}

	for (i = 0;i < 4; i++)
	{
		for (j = 0;j < 5; j++)
		{
			str_msg.Format("[Work[%d]] %d was changed by %d", i, WorkBuffer[0][i][j][0], WorkBuffer[0][i][j][1]);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
	}
}

void CDialog_Work_Information::OnWorkInform_Data_Comparison()
{
	int i,j,Ret;

	for (i = 0;i < 2; i++)
	{
		if (nModuleRobotPicker[0][i][0]		!= nModuleRobotPicker[0][i][1])
		{
			Ret = RET_ERROR;
			break;
		}
		if (nLefthsRobotPicker[0][i][0]		!= nLefthsRobotPicker[0][i][1])
		{
			Ret = RET_ERROR;
			break;
		}

		if (nRighthsRobotPicker[0][i][0]	!= nRighthsRobotPicker[0][i][1])
		{
			Ret = RET_ERROR;
			break;
		}
	}
	if (nClipRobotPicker[0][0]		!= nClipRobotPicker[0][1])
	{
		Ret = RET_ERROR;
	}
	if (nSortRobotPicker[0][0]		!= nSortRobotPicker[0][1])
	{
		Ret = RET_ERROR;
	}
	if (nUldRobotPicker[0][0]		!= nUldRobotPicker[0][1])
	{
		Ret = RET_ERROR;
	}
	for (i = 0;i < 2; i++)
	{
		if (VisionBuffer[0][i][0]	!= VisionBuffer[0][i][1])
		{
			Ret = RET_ERROR;
			break;
		}
	}
	for (i = 0;i < 4; i++)
	{
		for (j = 0;j < 5; j++)
		{
			if (WorkBuffer[0][i][j][0]	!= WorkBuffer[0][i][j][1])
			{
				Ret = RET_ERROR;
				break;
			}
		}
	}
}

void CDialog_Work_Information::GridText(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	m_p_grid.GridCellText(Grid,col, row, data);

	Grid = NULL;
	delete Grid;
}
void CDialog_Work_Information::OnWorkInform_Data_Apply()
{
	int i,j;
	st_handler.n_module_skip = NO;
	st_handler.n_fronths_skip = NO;
	st_handler.n_rearhs_skip = NO;
	st_handler.n_cliprbtpicker_skip = NO;
	st_handler.n_sorter_skip = NO;
	st_handler.n_unload_skip = NO;
	st_handler.n_vision_skip = NO;

	for (i = 0; i < 2; i++)
	{
		if(st_modulemap.nModuleRobotPicker[D_EXIST][i] != nModuleRobotPicker[D_EXIST][i][1])
		{
			st_modulemap.nModuleRobotPicker[D_EXIST][i] = nModuleRobotPicker[D_EXIST][i][1];
			st_handler.n_module_skip = YES;
		}
		if(st_modulemap.nLefthsRobotPicker[D_EXIST][i] != nLefthsRobotPicker[D_EXIST][i][1])
		{
			st_modulemap.nLefthsRobotPicker[D_EXIST][i] = nLefthsRobotPicker[D_EXIST][i][1];
			st_handler.n_fronths_skip = YES;
		}
		if(st_modulemap.nRighthsRobotPicker[D_EXIST][i] != nRighthsRobotPicker[D_EXIST][i][1])
		{
			st_modulemap.nRighthsRobotPicker[D_EXIST][i] = nRighthsRobotPicker[D_EXIST][i][1];
			st_handler.n_rearhs_skip = YES;
		}
	}
	if(st_handler.n_module_skip == YES)
	{
//		Init_module_step = 0;
//		GridText(IDC_CUSTOM_WORK_INFORMATION, 2, 4, "..ing");
//		SetTimer(TMR_MOTOR_PICK_MODULE, 100, NULL);
	}
	if(st_handler.n_fronths_skip == YES)
	{
//		Init_front_step = 0;
//		GridText(IDC_CUSTOM_WORK_INFORMATION, 3, 4, "..ing");
//		SetTimer(TMR_MOTOR_FRONT_MODULE, 100, NULL);
	}
	if(st_handler.n_rearhs_skip == YES)
	{
//		Init_rear_step = 0;
//		GridText(IDC_CUSTOM_WORK_INFORMATION, 4, 4, "..ing");
//		SetTimer(TMR_MOTOR_REAR_MODULE, 100, NULL);
	}

	for (i = 0; i < 4; i++)//Work1234
	{
		st_handler.n_workbuff_skip[i] = NO;
		for (j = 0; j < 5; j++) //LDMODULE,LEFT,RIGHT,CLIP,sorter
		{
			if(st_modulemap.WorkBuffer[D_EXIST][i][j] != WorkBuffer[D_EXIST][i][j][1])
			{
				st_modulemap.WorkBuffer[D_EXIST][i][j] = WorkBuffer[D_EXIST][i][j][1];
				st_handler.n_workbuff_skip[i] = YES;
			}
		}
	}
	for (i = 0; i < 4; i++)//Work1234
	{
		if(st_handler.n_workbuff_skip[i] == YES)
		{
			if(i == 0)
			{
				Init_buffer_step_1 = 0;
				GridText(IDC_CUSTOM_WORK_INFORMATION, 2, 7, "..ing");
			}
			else if(i == 1)
			{
				Init_buffer_step_2 = 0;
				GridText(IDC_CUSTOM_WORK_INFORMATION, 2, 7, "..ing");
			}
			else if(i == 2)
			{
				Init_buffer_step_3 = 0;
				GridText(IDC_CUSTOM_WORK_INFORMATION, 2, 7, "..ing");
			}
			else
			{
				Init_buffer_step_4 = 0;
				GridText(IDC_CUSTOM_WORK_INFORMATION, 2, 7, "..ing");
			}
		}
	}

	if(st_modulemap.nClipRobotPicker[D_EXIST] != nClipRobotPicker[D_EXIST][1])
	{
		st_modulemap.nClipRobotPicker[D_EXIST] = nClipRobotPicker[D_EXIST][1];
		st_handler.n_cliprbtpicker_skip = YES;
	}

//	if(st_handler.n_cliprbtpicker_skip == YES)
//	{
//		SetTimer(TMR_MOTOR_CLIP_MODULE, 100, NULL);
//	}

	if(st_modulemap.nSortRobotPicker[D_EXIST] != nSortRobotPicker[D_EXIST][1])
	{
		st_modulemap.nSortRobotPicker[D_EXIST] = nSortRobotPicker[D_EXIST][1];
		st_handler.n_sorter_skip = YES;
	}
//	if(st_handler.n_sorter_skip == YES)
//	{
//		SetTimer(TMR_MOTOR_SORTER_MODULE, 100, NULL);
//	}


	if(st_modulemap.nUldRobotPicker[D_EXIST] != nUldRobotPicker[D_EXIST][1])
	{
		st_modulemap.nUldRobotPicker[D_EXIST] = nUldRobotPicker[D_EXIST][1];
		st_handler.n_unload_skip = YES;
	}
//	if(st_handler.n_unload_skip == YES)
//	{
//		SetTimer(TMR_MOTOR_UNLOAD_MODULE, 100, NULL);
//	}
//

	for (i = 0; i < 2; i++)
	{
		if(st_modulemap.VisionBuffer[D_EXIST][i] != VisionBuffer[D_EXIST][i][1])
		{
			st_modulemap.VisionBuffer[D_EXIST][i] = VisionBuffer[D_EXIST][i][1];
			st_handler.n_vision_skip = YES;
		}
	}
//	if(st_handler.n_vision_skip == YES)
//	{
//		SetTimer(TMR_MOTOR_VISION_MODULE, 100, NULL);
//	}

}

void CDialog_Work_Information::OnApply() 
{
	int n_response;
	CDialog_Select  select_dlg;

	st_msg.mstr_confirm_msg = _T("Work Information : Setting Data Apply?");

	n_response = select_dlg.DoModal();

	if (n_response == IDOK)
	{
		OnWorkInform_Data_Comparison();
		OnWorkInform_Data_Apply();	
		OnWorkInform_LogFile_Create();
		OnWorkInform_Data_BackUp();
	}
	else if (n_response == IDCANCEL)
	{

	}	
}


void CDialog_Work_Information::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CString str_tmp, str_file;
	int nResponse;

	if(lpcc->Row < 2 || lpcc->Col < 2) return;

	switch(lpcc->Col)
	{
	case 0:
	case 1:
		break;

	case 2:
		if(wParam == IDC_CUSTOM_WORK_INFORMATION)
		{
			m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_INFORMATION);
		
			if (lpcc->Row == 2)
			{
				if(nModuleRobotPicker[D_EXIST][0][1] == DVC_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg = _T("Module Picker의 디바이스를 제거하고 초기화를 진행행하시겠습니까?");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						st_msg.mstr_confirm_msg = _T("Module Picker device initialization line Are you sure you want to remove");
					}

						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						nModuleRobotPicker[D_EXIST][0][1] = DVC_NO;
						str_tmp.Format("%d",nModuleRobotPicker[D_EXIST][0][1]);
						m_p_grid.GridCellText(m_grid_info, 2, 2, str_tmp);

						nMotor_num = M_M_RBT_Z;
						Init_module_step = 0;
						m_b_strop_req = FALSE;

						n_motor_init[LDRBTMODULE_SITE][1] = CTL_READY;

						SetTimer(TMR_MOTOR_PICK_MODULE, 100, NULL);
						m_p_grid.GridCellText(m_grid_info, 2, 4, "..ing");
						mb_display_opration = TRUE;

					}
				}
			}
			else if(lpcc->Row == 3)
			{
				if(nLefthsRobotPicker[D_EXIST][0][1] == DVC_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg = _T("Left Hs Front Picker의 디바이스를 제거하시겠습니까?");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						st_msg.mstr_confirm_msg = _T("Left Hs Front Picker Are you sure you want to remove the device");
					}

						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						nLefthsRobotPicker[D_EXIST][0][1] = DVC_NO;
						str_tmp.Format("%d",nLefthsRobotPicker[D_EXIST][0][1]);
						m_p_grid.GridCellText(m_grid_info, 3, 2, str_tmp);

						nMotor_num = M_HS_F_RBT_Z;
						Init_front_step = 0;
						m_b_strop_req = FALSE;

						n_motor_init[LEFTRBTSINK_SITE][1] = CTL_READY;

						SetTimer(TMR_MOTOR_FRONT_MODULE, 100, NULL);
						m_p_grid.GridCellText(m_grid_info, 3, 4, "..ing");
						mb_display_opration = TRUE;
					}
				}
			}		
			else if(lpcc->Row == 4)
			{
				if(nRighthsRobotPicker[D_EXIST][0][1] == DVC_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg = _T("Right Hs Front Picker의 디바이스를 제거하시겠습니까?");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						st_msg.mstr_confirm_msg = _T("Right Hs Front Picker Are you sure you want to remove the device");
					}
						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						nRighthsRobotPicker[D_EXIST][0][1] = DVC_NO;
						str_tmp.Format("%d",nRighthsRobotPicker[D_EXIST][0][1]);
						m_p_grid.GridCellText(m_grid_info, 4, 2, str_tmp);

						nMotor_num = M_HS_B_RBT_Z;
						Init_rear_step = 0;
						m_b_strop_req = FALSE;

						n_motor_init[RIGHTRBTSINK_SITE][1] = CTL_READY;

						SetTimer(TMR_MOTOR_REAR_MODULE, 100, NULL);
						m_p_grid.GridCellText(m_grid_info, 4, 4, "..ing");
						mb_display_opration = TRUE;
					}
				}
			}
		}
		if(wParam == IDC_CUSTOM_WORK_BUFFER)
		{
			m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_BUFFER);

			if (lpcc->Row == 2 || lpcc->Row == 3 || lpcc->Row == 4 || lpcc->Row == 5)
			{
				if(WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE][1] == DVC_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg.Format("Work Buffer#%d의 Heat Sink를 제거하고 초기화를 진행하시겠습니까?", lpcc->Row - 2 + 1);
						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						mb_display_opration = TRUE;
						str_tmp.Format("%d",WorkBuffer[D_EXIST][lpcc->Row - 2][LDMODULE_SITE][1]);
						m_p_grid.GridCellText(m_grid_info, lpcc->Row, 2, str_tmp);

						if(lpcc->Row == 2)
						{
							nBuffer_num_1 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_1 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_1, 100, NULL);
						}
						else if(lpcc->Row == 3)
						{
							nBuffer_num_2 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_2 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_2, 100, NULL);
						}
						else if(lpcc->Row == 4)
						{
							nBuffer_num_3 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_3 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_3, 100, NULL);
						}
						else
						{
							nBuffer_num_4 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_4 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_4, 100, NULL);
						}

						m_b_strop_req = FALSE;
						WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE][1] = DVC_EMPTY;
						WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][RIGHTSINK_SITE][1] = DVC_EMPTY;

						str_tmp.Format("%d",WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE][1]);
						GridText(IDC_CUSTOM_WORK_BUFFER,lpcc->Row, 2,str_tmp);
						GridText(IDC_CUSTOM_WORK_BUFFER,lpcc->Row, 4,str_tmp);

						n_motor_init[M_WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_READY;

						//0907
						st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_FREE;
						st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;

						//0916
						st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = DVC_EMPTY;
						st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = DVC_EMPTY;

						m_p_grid.GridCellText(m_grid_info, lpcc->Row, 7, "..ing");

					}
				}
			}
		}
		if(wParam == IDC_CUSTOM_WORK_UNLOAD)
		{
			m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_UNLOAD);
			if (lpcc->Row == 2)
			{
				if(nSortRobotPicker[D_EXIST][1] == DVC_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg = _T("Module Sorter Picker의 디바이스를 제거하시겠습니까?");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						st_msg.mstr_confirm_msg = _T("Module Sorter Picker Are you sure you want to remove the device");
					}
						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						mb_display_opration = TRUE;
						if(st_sync.n_module_site_status[SORTER_SITE][st_sync.n_module_buffer_action_site[SORTER_SITE][1]][0] != BUFF_EMPTY)
						{
							nMotor_num = M_WORK_BUFFER_1 + st_sync.n_module_buffer_action_site[SORTER_SITE][1];
							nSortRobotPicker[D_EXIST][1] = BUFF_EMPTY;
						}
						else
						{
							st_sync.n_module_site_status[LDMODULE_SITE][st_sync.n_module_buffer_action_site[0][1]][0] 	 = CTL_NO;
							st_sync.n_module_site_status[LDMODULE_SITE][st_sync.n_module_buffer_action_site[0][1]][1] 	 = CTL_NO;

							st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][st_sync.n_module_buffer_action_site[0][1]][0]	 = CTL_NO;
							st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][st_sync.n_module_buffer_action_site[0][1]][1]	 = CTL_NO;
							if     (st_sync.n_module_buffer_action_site[0][1] == 0)	Run_Rbt_Work_Buffer.RunBuff1Step = 100;
							else if(st_sync.n_module_buffer_action_site[0][1] == 1)	Run_Rbt_Work_Buffer.RunBuff2Step = 100;
							else if(st_sync.n_module_buffer_action_site[0][1] == 2)	Run_Rbt_Work_Buffer.RunBuff3Step = 100;
							else if(st_sync.n_module_buffer_action_site[0][1] == 3)	Run_Rbt_Work_Buffer.RunBuff4Step = 100;
						}

						nModuleRobotPicker[D_EXIST][1][1] = DVC_NO;
						str_tmp.Format("%d",nModuleRobotPicker[D_EXIST][1][1]);
						m_p_grid.GridCellText(m_grid_info, 2, 3, str_tmp);

					}
				}
			}
		}
		break;

	case 3 :
		if(wParam == IDC_CUSTOM_WORK_INFORMATION)
		{
			m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_INFORMATION);
			if (lpcc->Row == 2)
			{
				if(nModuleRobotPicker[D_EXIST][1][1] == DVC_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg = _T("Module Rear Picker의 디바이스를 제거하시겠습니까?");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						st_msg.mstr_confirm_msg = _T("Module Rear Picker Are you sure you want to remove the device");
					}
						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						mb_display_opration = TRUE;
						if(st_sync.n_module_site_status[LDMODULE_SITE][st_sync.n_module_buffer_action_site[0][1]][0] != BUFF_EMPTY)
						{
							nMotor_num = M_WORK_BUFFER_1 + st_sync.n_module_buffer_action_site[0][1];
							WorkBuffer[D_EXIST][st_sync.n_module_buffer_action_site[0][1]][LDMODULE_SITE][1] = BUFF_EMPTY;
						}
						else
						{
							st_sync.n_module_site_status[LDMODULE_SITE][st_sync.n_module_buffer_action_site[0][1]][0] 	 = CTL_NO;
							st_sync.n_module_site_status[LDMODULE_SITE][st_sync.n_module_buffer_action_site[0][1]][1] 	 = CTL_NO;

							st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][st_sync.n_module_buffer_action_site[0][1]][0]	 = CTL_NO;
							st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][st_sync.n_module_buffer_action_site[0][1]][1]	 = CTL_NO;
							if     (st_sync.n_module_buffer_action_site[0][1] == 0)	Run_Rbt_Work_Buffer.RunBuff1Step = 100;
							else if(st_sync.n_module_buffer_action_site[0][1] == 1)	Run_Rbt_Work_Buffer.RunBuff2Step = 100;
							else if(st_sync.n_module_buffer_action_site[0][1] == 2)	Run_Rbt_Work_Buffer.RunBuff3Step = 100;
							else if(st_sync.n_module_buffer_action_site[0][1] == 3)	Run_Rbt_Work_Buffer.RunBuff4Step = 100;
						}

						nModuleRobotPicker[D_EXIST][1][1] = DVC_NO;
						str_tmp.Format("%d",nModuleRobotPicker[D_EXIST][1][1]);
						m_p_grid.GridCellText(m_grid_info, 2, 3, str_tmp);

					}
				}
			}
			else if(lpcc->Row == 3)
			{
				if(nLefthsRobotPicker[D_EXIST][1][1] == DVC_YES)
				{
					//초기화를 진행하면서 선택한 변수를 설정한다 
					if(n_motor_init[LEFTRBTSINK_SITE][1] == CTL_YES)
					{
						CDialog_Select msg_dlg;
 						st_msg.mstr_confirm_msg = _T("Heat Sink Front Picker의 디바이스를 제거하시겠습니까?");
						if(st_handler.mn_language == LANGUAGE_ENGLISH) 
						{
							st_msg.mstr_confirm_msg = _T("Heat Sink Front Picker Are you sure you want to remove the device");
						}
							
						nResponse = msg_dlg.DoModal();
						if(nResponse == IDOK)
						{

							mb_display_opration = TRUE;
							if(st_sync.n_module_site_status[LEFTRBTSINK_SITE][st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1]][0] != BUFF_EMPTY)
							{
								nMotor_num = M_WORK_BUFFER_1 + st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1];
								WorkBuffer[D_EXIST][st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1]][LEFTSINK_SITE][1] = BUFF_EMPTY;
							}
							else
							{
								st_sync.n_module_site_status[LEFTSINK_SITE][st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1]][0] 	 = CTL_NO;
								st_sync.n_module_site_status[LEFTSINK_SITE][st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1]][1] 	 = CTL_NO;

								st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1]][0]	 = CTL_NO;
								st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1]][1]	 = CTL_NO;
								
								st_sync.n_module_site_status[RIGHTRBTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1]][0] 	 = CTL_NO;
								st_sync.n_module_site_status[RIGHTRBTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1]][1] 	 = CTL_NO;

								st_sync.n_module_ldrbt_to_work[RIGHTRBTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1]][0]	 = CTL_NO;
								st_sync.n_module_ldrbt_to_work[RIGHTRBTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1]][1]	 = CTL_NO;

								if     (st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1] == 0)	Run_Rbt_Work_Buffer.RunBuff1Step = 100;
								else if(st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1] == 1)	Run_Rbt_Work_Buffer.RunBuff2Step = 100;
								else if(st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1] == 2)	Run_Rbt_Work_Buffer.RunBuff3Step = 100;
								else if(st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1] == 3)	Run_Rbt_Work_Buffer.RunBuff4Step = 100;
							}

							nLefthsRobotPicker[D_EXIST][1][1] = DVC_NO;
							str_tmp.Format("%d",nLefthsRobotPicker[D_EXIST][1][1]);
							m_p_grid.GridCellText(m_grid_info, 3, 3, str_tmp);

						}
					}
				}
			}
			else if(lpcc->Row == 4)
			{
				if(nRighthsRobotPicker[D_EXIST][1][1] == DVC_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg = _T("Heat Sink Rear Picker의 디바이스를 제거하시겠습니까?");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						st_msg.mstr_confirm_msg = _T("Heat Sink Rear Picker Are you sure you want to remove the device");
					}
						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{

						mb_display_opration = TRUE;
						if(st_sync.n_module_site_status[RIGHTRBTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1]][0] != BUFF_EMPTY)
						{
							nMotor_num = M_WORK_BUFFER_1 + st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1];
							WorkBuffer[D_EXIST][st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1]][RIGHTRBTSINK_SITE][1] = BUFF_EMPTY;
						}
						else
						{
							st_sync.n_module_site_status[RIGHTRBTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1]][0] 	 = CTL_NO;
							st_sync.n_module_site_status[RIGHTRBTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1]][1] 	 = CTL_NO;

							st_sync.n_module_ldrbt_to_work[RIGHTRBTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1]][0]	 = CTL_NO;
							st_sync.n_module_ldrbt_to_work[RIGHTRBTSINK_SITE][st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1]][1]	 = CTL_NO;
							if     (st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1] == 0)	Run_Rbt_Work_Buffer.RunBuff1Step = 100;
							else if(st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1] == 1)	Run_Rbt_Work_Buffer.RunBuff2Step = 100;
							else if(st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1] == 2)	Run_Rbt_Work_Buffer.RunBuff3Step = 100;
							else if(st_sync.n_module_buffer_action_site[RIGHTRBTSINK_SITE][1] == 3)	Run_Rbt_Work_Buffer.RunBuff4Step = 100;
						}


						nRighthsRobotPicker[D_EXIST][1][1] = DVC_NO;
						str_tmp.Format("%d",nRighthsRobotPicker[D_EXIST][1][1]);
						m_p_grid.GridCellText(m_grid_info, 4, 3, str_tmp);

					}
				}
			}
		}
		if(wParam == IDC_CUSTOM_WORK_BUFFER)
		{
			m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_BUFFER);

			if (lpcc->Row == 2 || lpcc->Row == 3 || lpcc->Row == 4 || lpcc->Row == 5)
			{
				if(WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LDMODULE_SITE][1] == DVC_YES)
				{
					CDialog_Select msg_dlg;
					if(WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE][1] == DVC_YES)
					{
 						st_msg.mstr_confirm_msg.Format("Work Buffer#%d을 Module 및 Heat Sink를 제거하고 초기화를 진행행하시겠습니까?", lpcc->Row - 2 + 1);
					}
					else
					{
 						st_msg.mstr_confirm_msg.Format("Work Buffer#%d을 Module를 제거하고 초기화를 진행행하시겠습니까?", lpcc->Row - 2 + 1);
					}						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						mb_display_opration = TRUE;
						if (lpcc->Row == 2)
						{
							nBuffer_num_1 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_1 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_1, 100, NULL);
						}
						else if (lpcc->Row == 3)
						{
							nBuffer_num_2 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_2 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_2, 100, NULL);
						}
						else if (lpcc->Row == 4)
						{
							nBuffer_num_3 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_3 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_3, 100, NULL);
						}
						else
						{
							nBuffer_num_4 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_4 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_4, 100, NULL);
						}

						nMotor_num = M_WORK_BUFFER_1 + lpcc->Row - 2;
						m_b_strop_req = FALSE;

						WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LDMODULE_SITE][1] = DVC_EMPTY;
						WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE][1] = DVC_EMPTY;
						WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][RIGHTSINK_SITE][1] = DVC_EMPTY;

						str_tmp.Format("%d",WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE][1]);
						GridText(IDC_CUSTOM_WORK_BUFFER,lpcc->Row, 2,str_tmp);
						GridText(IDC_CUSTOM_WORK_BUFFER,lpcc->Row, 3,str_tmp);
						GridText(IDC_CUSTOM_WORK_BUFFER,lpcc->Row, 4,str_tmp);
						
						n_motor_init[M_WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_READY;

						st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LDMODULE_SITE]	 = DVC_NO;
						st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE]	 = DVC_NO;
						st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][RIGHTSINK_SITE] = DVC_NO;
						st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LDCLIP_SITE]	 = DVC_NO;
						st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][SORTER_SITE]	 = DVC_NO;

						st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] 	 = CTL_NO;
						st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] 	 = CTL_NO;
						st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] 	 = CTL_NO;
						st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] 	 = CTL_NO;
						st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] 	 = CTL_NO;

						st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] 	 = CTL_NO;
						st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] 	 = CTL_NO;
						st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] 	 = CTL_NO;
						st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] 	 = CTL_NO;
						st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] 	 = CTL_NO;

						st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_NO;

						st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;

						st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_FREE;


						m_p_grid.GridCellText(m_grid_info, lpcc->Row, 7, "..ing");
					}
				}
			}
		}
		break;

	case 4:
		if(wParam == IDC_CUSTOM_WORK_INFORMATION)
		{
			m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_INFORMATION);
			if (lpcc->Row == 2)
			{
				if(n_motor_init[LDRBTMODULE_SITE][1] == CTL_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg = _T("Module Picker를 초기화 하시겠습니끼?");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						st_msg.mstr_confirm_msg = _T("Are you sure you want to initialize the Module Picker");
					}
						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						Init_module_step = 0;
						mb_display_opration = TRUE;
						m_b_strop_req = FALSE;

						SetTimer(TMR_MOTOR_PICK_MODULE, 100, NULL);
						m_p_grid.GridCellText(m_grid_info, 2, 4, "..ing");

					}
				}
			}
			else if(lpcc->Row == 3)
			{
				if(n_motor_init[LEFTRBTSINK_SITE][1] == CTL_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg = _T("Front Picker를 초기화 하시겠습니끼?");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						st_msg.mstr_confirm_msg = _T("Are you sure you want to initialize the Front Picker");
					}

						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						mb_display_opration = TRUE;
						Init_front_step = 0;
						m_b_strop_req = FALSE;

						SetTimer(TMR_MOTOR_FRONT_MODULE, 100, NULL);
						m_p_grid.GridCellText(m_grid_info, 3, 4, "..ing");

					}
				}
			}
			else if(lpcc->Row == 4)
			{
				if(n_motor_init[RIGHTRBTSINK_SITE][1] == CTL_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg = _T("Rear Picker를 초기화 하시겠습니끼?");
					if(st_handler.mn_language == LANGUAGE_ENGLISH) 
					{
						st_msg.mstr_confirm_msg = _T("Are you sure you want to initialize the Rear Picker");
					}
						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						mb_display_opration = TRUE;
						Init_rear_step = 0;
						m_b_strop_req = FALSE;

						SetTimer(TMR_MOTOR_REAR_MODULE, 100, NULL);
						m_p_grid.GridCellText(m_grid_info, 4, 4, "..ing");

					}
				}
			}
		}
		if(wParam == IDC_CUSTOM_WORK_BUFFER)
		{
			m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_BUFFER);

			if (lpcc->Row == 2 || lpcc->Row == 3 || lpcc->Row == 4 || lpcc->Row == 5)
			{
				if(WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][RIGHTSINK_SITE][1] == DVC_YES)
				{
					CDialog_Select msg_dlg;
 					st_msg.mstr_confirm_msg.Format("Work Buffer#%d을 Module 및 Heat Sink를 제거하고 초기화를 진행행하시겠습니까?", lpcc->Row - 2 + 1);
						
					nResponse = msg_dlg.DoModal();
					if(nResponse == IDOK)
					{
						mb_display_opration = TRUE;
						str_tmp.Format("%d",WorkBuffer[D_EXIST][lpcc->Row - 2][LDMODULE_SITE][1]);
						m_p_grid.GridCellText(m_grid_info, lpcc->Row, 2, str_tmp);

						if(lpcc->Row == 2)
						{
							nBuffer_num_1 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_1 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_1, 100, NULL);
						}
						else if(lpcc->Row == 3)
						{
							nBuffer_num_2 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_2 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_2, 100, NULL);
						}
						else if(lpcc->Row == 4)
						{
							nBuffer_num_3 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_3 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_3, 100, NULL);
						}
						else
						{
							nBuffer_num_4 = M_WORK_BUFFER_1 + lpcc->Row - 2;
							Init_buffer_step_4 = 0;
							SetTimer(TMR_MOTOR_WORK_BUFFER_4, 100, NULL);
						}

						m_b_strop_req = FALSE;
						WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE][1] = DVC_EMPTY;
						WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][RIGHTSINK_SITE][1] = DVC_EMPTY;

						str_tmp.Format("%d",WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE][1]);
						GridText(IDC_CUSTOM_WORK_BUFFER,lpcc->Row, 2,str_tmp);
						GridText(IDC_CUSTOM_WORK_BUFFER,lpcc->Row, 4,str_tmp);
						
						n_motor_init[M_WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_READY;

						//0907
						st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_FREE;
						st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;

						//0916
						st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = DVC_EMPTY;
						st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = DVC_EMPTY;

						m_p_grid.GridCellText(m_grid_info, lpcc->Row, 7, "..ing");
					}
				}
			}
		}
		break;

	case 7:
		if(wParam == IDC_CUSTOM_WORK_BUFFER)
		{
			m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_BUFFER);

			if (lpcc->Row == 2 || lpcc->Row == 3 || lpcc->Row == 4 || lpcc->Row == 5)
			{
				CDialog_Select msg_dlg;
 				st_msg.mstr_confirm_msg.Format("Work Buffer#%d의 초기화를 진행하시겠습니까?", lpcc->Row - 2 + 1);
					
				nResponse = msg_dlg.DoModal();
				if(nResponse == IDOK)
				{
					mb_display_opration = TRUE;
					str_tmp.Format("%d",WorkBuffer[D_EXIST][lpcc->Row - 2][LDMODULE_SITE][1]);
					m_p_grid.GridCellText(m_grid_info, lpcc->Row, 2, str_tmp);

					if(lpcc->Row == 2)
					{
						nBuffer_num_1 = M_WORK_BUFFER_1 + lpcc->Row - 2;
						Init_buffer_step_1 = 0;
						SetTimer(TMR_MOTOR_WORK_BUFFER_1, 100, NULL);
					}
					else if(lpcc->Row == 3)
					{
						nBuffer_num_2 = M_WORK_BUFFER_1 + lpcc->Row - 2;
						Init_buffer_step_2 = 0;
						SetTimer(TMR_MOTOR_WORK_BUFFER_2, 100, NULL);
					}
					else if(lpcc->Row == 4)
					{
						nBuffer_num_3 = M_WORK_BUFFER_1 + lpcc->Row - 2;
						Init_buffer_step_3 = 0;
						SetTimer(TMR_MOTOR_WORK_BUFFER_3, 100, NULL);
					}
					else
					{
						nBuffer_num_4 = M_WORK_BUFFER_1 + lpcc->Row - 2;
						Init_buffer_step_4 = 0;
						SetTimer(TMR_MOTOR_WORK_BUFFER_4, 100, NULL);
					}

					m_b_strop_req = FALSE;
					n_motor_init[M_WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_READY;

					if(st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_1+ lpcc->Row - 2][0] != BUFF_EMPTY)
					{
						if(st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1+ lpcc->Row - 2][0] != BUFF_EMPTY)
						{
							if(st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1+ lpcc->Row - 2][0] != BUFF_EMPTY)
							{
								if (st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_1+ lpcc->Row - 2][0] != BUFF_EMPTY)
								{
									if (st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_1+ lpcc->Row - 2][0] != BUFF_EMPTY)
									{
										return;
									}
									else
									{
										st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1][0] = CTL_FREE;
									}
								}
								else
								{
									st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1][0] = CTL_FREE;
									st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1][1] = CTL_NO;
								}
							}
							else
							{
								st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_FREE;
								st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;
								st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = DVC_EMPTY;
								st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = DVC_EMPTY;
							}
						}
						else
						{
							st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_FREE;
							st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;
							st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = DVC_EMPTY;
							st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = DVC_EMPTY;
						}
					}
					else
					{
						WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LDMODULE_SITE][1] = DVC_EMPTY;
						WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE][1] = DVC_EMPTY;
						WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][RIGHTSINK_SITE][1] = DVC_EMPTY;

						n_motor_init[M_WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_READY;

						st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LDMODULE_SITE]	 = DVC_NO;
						st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LEFTSINK_SITE]	 = DVC_NO;
						st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][RIGHTSINK_SITE] = DVC_NO;
						st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][LDCLIP_SITE]	 = DVC_NO;
						st_modulemap.WorkBuffer[D_EXIST][WORK_BUFFER_1 + lpcc->Row - 2][SORTER_SITE]	 = DVC_NO;

						st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] 	 = CTL_NO;
						st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] 	 = CTL_NO;
						st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] 	 = CTL_NO;
						st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] 	 = CTL_NO;
						st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] 	 = CTL_NO;

						st_sync.n_module_site_status[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] 	 = CTL_NO;
						st_sync.n_module_site_status[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] 	 = CTL_NO;
						st_sync.n_module_site_status[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] 	 = CTL_NO;
						st_sync.n_module_site_status[LDCLIP_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] 	 = CTL_NO;
						st_sync.n_module_site_status[SORTER_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] 	 = CTL_NO;

						st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_NO;

						st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[RIGHTSINK_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[LDCLIP_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;
						st_sync.n_module_ldrbt_to_work[SORTER_SITE][WORK_BUFFER_1 + lpcc->Row - 2][1] = CTL_NO;

						st_sync.n_module_ldrbt_to_work[LDMODULE_SITE][WORK_BUFFER_1 + lpcc->Row - 2][0] = CTL_FREE;
					}

					m_p_grid.GridCellText(m_grid_info, lpcc->Row, 7, "..ing");

				}
			}
		}
		break;
	}
	
	Data_Display();
}

void CDialog_Work_Information::Data_Display()
{
	if(mb_display_opration == TRUE)
	{
		m_btn_apply.EnableWindow(FALSE);
		m_btn_ok.EnableWindow(FALSE);
		m_msg_warnning.ShowWindow(TRUE);
	}
	else
	{
		m_msg_warnning.ShowWindow(FALSE);
		m_btn_apply.EnableWindow(TRUE);
		m_btn_ok.EnableWindow(TRUE);
	}	
}

void CDialog_Work_Information::OnTimer(UINT nIDEvent) 
{
	int nRet_1 = RET_PROCEED;
	if(nIDEvent == TMR_MOTOR_PICK_MODULE)
	{
		nRet_1 = ModuleRbtInit();
		if(nRet_1 == RET_GOOD)
		{
			KillTimer(TMR_MOTOR_PICK_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_PICK_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_FRONT_MODULE)
	{
		nRet_1 = FrontHsInit();
		if(nRet_1 == RET_GOOD)
		{
			KillTimer(TMR_MOTOR_FRONT_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_FRONT_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_REAR_MODULE)
	{
		nRet_1 = BackHsInit();
		if(nRet_1 == RET_GOOD)
		{
			KillTimer(TMR_MOTOR_REAR_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_REAR_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_WORK_BUFFER_1)
	{
		nRet_1 = WorkBufferInit_1();
		if(nRet_1 == RET_GOOD)
		{
//			if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] == WORK_BUFFER_1)
//			{
//				OnClipRbtInit();
//			}
			KillTimer(TMR_MOTOR_WORK_BUFFER_1);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_WORK_BUFFER_1);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_WORK_BUFFER_2)
	{
		nRet_1 = WorkBufferInit_2();
		if(nRet_1 == RET_GOOD)
		{
//			if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] == WORK_BUFFER_2)
//			{
//				OnClipRbtInit();
//			}
			KillTimer(TMR_MOTOR_WORK_BUFFER_2);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_WORK_BUFFER_2);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_WORK_BUFFER_3)
	{
		nRet_1 = WorkBufferInit_3();
		if(nRet_1 == RET_GOOD)
		{
//			if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] == WORK_BUFFER_3)
//			{
//				OnClipRbtInit();
//			}
			mb_display_opration = FALSE;
			Data_Display();
			KillTimer(TMR_MOTOR_WORK_BUFFER_3);
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_WORK_BUFFER_3);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_WORK_BUFFER_4)
	{
		nRet_1 = WorkBufferInit_4();
		if(nRet_1 == RET_GOOD)
		{
//			if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][0] == WORK_BUFFER_4)
//			{
//				OnClipRbtInit();
//			}
			KillTimer(TMR_MOTOR_WORK_BUFFER_4);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_WORK_BUFFER_4);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_CLIP_MODULE)
	{
		//nRet_1 = WorkBufferInit_4();
		if(nRet_1 == RET_GOOD)
		{
			KillTimer(TMR_MOTOR_CLIP_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_CLIP_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_SORTER_MODULE)
	{
		nRet_1 = SorterRbtInit();
		if(nRet_1 == RET_GOOD)
		{
			KillTimer(TMR_MOTOR_SORTER_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_SORTER_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_UNLOAD_MODULE)
	{
		nRet_1 = UldRbtInit();
		if(nRet_1 == RET_GOOD)
		{
			KillTimer(TMR_MOTOR_UNLOAD_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_UNLOAD_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_VISION_MODULE)
	{
		//nRet_1 = WorkBufferInit_4();
		if(nRet_1 == RET_GOOD)
		{
			KillTimer(TMR_MOTOR_VISION_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_VISION_MODULE);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	else if(nIDEvent == TMR_MOTOR_CLIP_CLAMP)
	{
		nRet_1 = ClipClampRbtInit();
		if(nRet_1 == RET_GOOD)
		{
			KillTimer(TMR_MOTOR_CLIP_CLAMP);
			m_clip_clamp_rbt.EnableWindow(TRUE);
			mb_display_opration = FALSE;
			Data_Display();
		}
		else if(nRet_1 == RET_ERROR)
		{
			KillTimer(TMR_MOTOR_CLIP_CLAMP);
			m_clip_clamp_rbt.EnableWindow(TRUE);
			mb_display_opration = FALSE;
			Data_Display();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

int CDialog_Work_Information::ModuleRbtInit()
{
	int i, nRet_1, FuncRet = RET_PROCEED;
	if(m_b_strop_req == TRUE)
	{
		COMI.mn_home_step[M_M_RBT_Z] = 0;
		Init_module_step = 1500;
	}
	switch(Init_module_step)
	{
	case 0:
		for (i = 0; i < PICKER_NUM; i++)
		{
			ActionPicker[D_EXIST][i] = YES;
		}
		Set_PickerUpDown(LDMODULE_SITE, IO_OFF, ActionPicker);
		Init_module_step = 100;
		break;

	case 100:
		// *************************************************************
		//  UP 시킨 실린더를 CHECK한다.                                 
		// *************************************************************
		nRet_1 = Get_PickerUpDown(LDMODULE_SITE, IO_OFF, ActionPicker);

		if (nRet_1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			Init_module_step = 1000;
		}
		else if (nRet_1 == RET_ERROR)
		{
			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			CTL_Lib.Alarm_Error_Occurrence(6100, st_work.mn_run_status, mc_alarmcode);
			Init_module_step = 1500;
			
		}
		break;

	case 1000:		//z axis부터 homecheck 하자!!
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_M_RBT_Z, st_motor[M_M_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			Init_module_step = 1600;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			Init_module_step = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6101, st_work.mn_run_status, COMI.mc_alarmcode);
			Init_module_step = 1500;
		}
		break;

	case 1500:
		FuncRet = RET_ERROR;
		Init_module_step = 0;
		break;

	case 2000:
		n_motor_init[LDRBTMODULE_SITE][1] = CTL_YES;
		FuncRet = RET_GOOD;
		st_handler.n_module_skip = NO;
		Run_Rbt_Load_Module.RunStep = 1000;

		GridText(IDC_CUSTOM_WORK_INFORMATION, 2, 4, "Done");
		Init_module_step = 0;
		break;		
	}

	return FuncRet;
}

int CDialog_Work_Information::FrontHsInit()
{
	int i, nRet_1, FuncRet = RET_PROCEED;
	int n_site = 0;

	if(m_b_strop_req == TRUE)
	{
		COMI.mn_home_step[M_HS_F_RBT_Z] = 0;
		Init_module_step = 1500;
	}
	switch(Init_front_step)
	{
	case 0:
		for (i = 0; i < PICKER_NUM; i++)
		{
			ActionPicker[D_EXIST][i] = YES;
		}
		Set_PickerUpDown(LEFTRBTSINK_SITE, IO_OFF, ActionPicker);
		Init_front_step = 100;
		break;

	case 100:
		// *************************************************************
		//  UP 시킨 실린더를 CHECK한다.                                 
		// *************************************************************
		nRet_1 = Get_PickerUpDown(LEFTRBTSINK_SITE, IO_OFF, ActionPicker);

		if (nRet_1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			Init_front_step = 1000;
		}
		else if (nRet_1 == RET_ERROR)
		{
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Manual  Picker Up.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}

			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			CTL_Lib.Alarm_Error_Occurrence(6102, st_work.mn_run_status, mc_alarmcode);
			Init_front_step = 1500;
			
		}
		break;

	case 1000:		//z axis부터 homecheck 하자!!
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_HS_F_RBT_Z, st_motor[M_HS_F_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			Init_front_step = 1600;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			Init_front_step = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6103, st_work.mn_run_status, COMI.mc_alarmcode);
			Init_front_step = 1500;
		}
		break;

	case 1500:
		FuncRet = RET_ERROR;
		Init_front_step = 0;
		break;
		
	case 1600:
//		n_site = st_sync.n_module_buffer_action_site[LEFTSINK_SITE][1];
		if(st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][n_site][0] == CTL_LOCK)
		{
			st_sync.n_module_ldrbt_to_work[LEFTSINK_SITE][n_site][0] = CTL_FREE;
			st_sync.n_module_site_status[LEFTSINK_SITE][n_site][0] = BUFF_EMPTY;
		}
		Init_front_step = 2000;
		break;

	case 2000:
		n_motor_init[LEFTRBTSINK_SITE][1] = CTL_YES;
		FuncRet = RET_GOOD;
		st_handler.n_fronths_skip = NO;
		Run_Rbt_Load_LeftHeat.RunStep = 1000;

		m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_INFORMATION);
		m_p_grid.GridCellText(m_grid_info, 3, 4, "Done");
		Init_front_step = 0;
		break;		
	}

	return FuncRet;
}

int CDialog_Work_Information::BackHsInit()
{
	int i, nRet_1, FuncRet = RET_PROCEED;

	if(m_b_strop_req == TRUE)
	{
		COMI.mn_home_step[M_HS_F_RBT_Z] = 0;
		Init_module_step = 1500;
	}
	switch(Init_rear_step)
	{
	case 0:
		for (i = 0; i < PICKER_NUM; i++)
		{
			ActionPicker[D_EXIST][i] = YES;
		}
		Set_PickerUpDown(RIGHTRBTSINK_SITE, IO_OFF, ActionPicker);
		Init_rear_step = 100;
		break;

	case 100:
		// *************************************************************
		//  UP 시킨 실린더를 CHECK한다.                                 
		// *************************************************************
		nRet_1 = Get_PickerUpDown(RIGHTRBTSINK_SITE, IO_OFF, ActionPicker);

		if (nRet_1 == RET_GOOD)			// 정상적으로 Up이 됐을 경우.
		{
			Init_rear_step = 1000;
		}
		else if (nRet_1 == RET_ERROR)
		{
			if(st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_abnormal_msg,"[Manual  Picker Up.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			}

			st_work.mn_run_status = CTL_dWARNING;		// 현재 발생한 알람 상태 플래그 설정 
			CTL_Lib.Alarm_Error_Occurrence(6104, st_work.mn_run_status, mc_alarmcode);
			Init_rear_step = 1500;
			
		}
		break;

	case 1000:		//z axis부터 homecheck 하자!!
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_HS_B_RBT_Z, st_motor[M_HS_B_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			Init_rear_step = 2000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			Init_rear_step = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6105, st_work.mn_run_status, COMI.mc_alarmcode);
			Init_rear_step = 1500;
		}
		break;

	case 1500:
		FuncRet = RET_ERROR;
		Init_rear_step = 0;
		break;		

	case 2000:
		st_handler.n_rearhs_skip = NO;
		n_motor_init[RIGHTRBTSINK_SITE][1] = CTL_YES;
		FuncRet = RET_GOOD;
		Run_Rbt_Load_RightHeat.RunStep = 1000;

		m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_WORK_INFORMATION);
		m_p_grid.GridCellText(m_grid_info, 4, 4, "Done");
		Init_rear_step = 0;
		break;		
	}

	return FuncRet;
}

int CDialog_Work_Information::WorkBufferInit_1()
{
	int nRet_1, FuncRet = RET_PROCEED, nSite;
	double dCurrInsertPos[2]={0,}, dCurrClampPos[3]={0,};
	CString str_tmp;

	if(nBuffer_num_1 < M_WORK_BUFFER_1 || nBuffer_num_1 > M_WORK_BUFFER_4)
	{
		return RET_ERROR;
	}
	nSite = nBuffer_num_1;

	if(m_b_strop_req == TRUE)
	{
		COMI.mn_home_step[nSite] = 0;
		Init_buffer_step_1 = 1500;
	}
	switch(Init_buffer_step_1)
	{
	case 0:
		if(st_handler.n_module_skip == NO && st_handler.n_fronths_skip == NO && 
			st_handler.n_rearhs_skip == NO && st_handler.n_cliprbtpicker_skip == NO &&
			st_handler.n_sorter_skip == NO)
		{
			Init_buffer_step_1 = 100;
		}
		break;

	case 100:
		dCurrInsertPos[0] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
		dCurrClampPos[0]  = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
		dCurrInsertPos[1] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
		dCurrClampPos[1]  = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
		dCurrClampPos[2] = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_GRIPPER);
		if(dCurrInsertPos[0] > st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE1] - st_motor[M_CLIP_INSERT_Y].mn_allow &&
			dCurrInsertPos[0] < st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE1] + st_motor[M_CLIP_INSERT_Y].mn_allow)
		{
			mn_Clipretry = 0;
			if(FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_ON ||
				FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_OFF)
			{
				FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
				Init_buffer_step_1 = 110;
				break;
			}
			if(dCurrInsertPos[1] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Y].mn_allow)
			{
				Init_buffer_step_1 = 110;
				break;
			}
		}
//		if(dCurrClampPos[0] > st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE1] - st_motor[M_M_CLAMP_RBT_Y].mn_allow &&
//			dCurrClampPos[0] < st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE1] + st_motor[M_M_CLAMP_RBT_Y].mn_allow)
//		{
			if(dCurrClampPos[1] > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_CLAMP_RBT_Z].mn_allow)
			{
				Init_buffer_step_1 = 130;
				break;
			}
//		}
		Init_buffer_step_1 = 200;
		break;

	case 110:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
			Init_buffer_step_1 = 120;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_1 = 110;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_1 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 120:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == RET_GOOD)
		{
			Init_buffer_step_1 = 100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			//260100 0 26 "Clip Insert Picker up 에러."
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"260100");
				CTL_Lib.Alarm_Error_Occurrence(2300, CTL_dWARNING, mc_alarmcode);
				Init_buffer_step_1 = 1500;
			}
			else
			{
				Init_buffer_step_1 = 110;
			}
		}
		break;

	case 130:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_1 = 140;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_1 = 130;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_1 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 140:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_1 = 100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_1 = 140;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_1 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 200:
		if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][1] == WORK_BUFFER_1)
		{
			st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;
			Init_buffer_step_1 = 300;
		}
		else
		{
			Init_buffer_step_1 = 400;
		}
		break;

	case 300:
		nRet_1 = Run_Rbt_Load_Clip.Run_RemoveClip();
		if(nRet_1 == RET_GOOD)
		{
			Init_buffer_step_1 = 350;
		}
		break;

	case 350:
		st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;
		Run_Rbt_Load_Clip.RunInsertStep = 1000;
		Run_Rbt_Load_Clip.ClipPlaceStep = 0;
		Run_Rbt_Load_Clip.RunClampStep = 1100;
		Run_Rbt_Load_Clip.ClampRbtStep= 0;
		Init_buffer_step_1 = 400;

	case 400:		
		nRet_1 = CTL_Lib.Single_Move(nSite, st_motor[nSite].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_1 = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_1 = 10;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_1 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1000:		//z axis부터 homecheck 하자!!
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(nSite, st_motor[nSite].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			Init_buffer_step_1 = 2000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			Init_buffer_step_1 = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6171, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1500:
		FuncRet = RET_ERROR;
		Init_buffer_step_1 = 0;
		break;		

	case 2000:
		n_motor_init[nSite][1] = CTL_YES;
		st_handler.n_workbuff_skip[WORK_BUFFER_1] = NO;
		GridText(IDC_CUSTOM_WORK_BUFFER, WORK_BUFFER_1 + 2, 7, "Done");//7

		FuncRet = RET_GOOD;
		Run_Rbt_Work_Buffer.RunBuff1Step = 1000;
		Init_buffer_step_1 = 0;
		break;		
	}

	return FuncRet;
}

int CDialog_Work_Information::WorkBufferInit_2()
{
	int nRet_1, FuncRet = RET_PROCEED, nSite;
	double dCurrInsertPos[2]={0,}, dCurrClampPos[2]={0,};
	CString str_tmp;

	if(nBuffer_num_2 < M_WORK_BUFFER_1 || nBuffer_num_2 > M_WORK_BUFFER_4)
	{
		return RET_ERROR;
	}
	nSite = nBuffer_num_2;

	if(m_b_strop_req == TRUE)
	{
		COMI.mn_home_step[nSite] = 0;
		Init_buffer_step_2 = 1500;
	}
	switch(Init_buffer_step_2)
	{
	case 0:
		if(st_handler.n_module_skip == NO && st_handler.n_fronths_skip == NO && 
			st_handler.n_rearhs_skip == NO && st_handler.n_cliprbtpicker_skip == NO &&
			st_handler.n_sorter_skip == NO)
		{
			Init_buffer_step_2 = 100;
		}
		break;

	case 100:
		dCurrInsertPos[0] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
		dCurrClampPos[0]  = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
		dCurrInsertPos[1] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
		dCurrClampPos[1]  = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
		if(dCurrInsertPos[0] > st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE2] - st_motor[M_CLIP_INSERT_Y].mn_allow &&
			dCurrInsertPos[0] < st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE2] + st_motor[M_CLIP_INSERT_Y].mn_allow)
		{
			mn_Clipretry = 0;
			if(FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_ON ||
				FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_OFF)
			{
				FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
				Init_buffer_step_2 = 110;
				break;
			}
			if(dCurrInsertPos[1] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Z].mn_allow)
			{
				Init_buffer_step_2 = 110;
				break;
			}
		}
//		if(dCurrClampPos[0] > st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE2] - st_motor[M_M_CLAMP_RBT_Y].mn_allow &&
//			dCurrClampPos[0] < st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE2] + st_motor[M_M_CLAMP_RBT_Y].mn_allow)
//		{
			if(dCurrClampPos[1] > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_CLAMP_RBT_Z].mn_allow)
			{
				Init_buffer_step_2 = 130;
				break;
			}
//		}
		Init_buffer_step_2 = 200;
		break;

	case 110:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
			Init_buffer_step_2 = 120;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_2 = 110;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
			Init_buffer_step_2 = 1500;
		}
		break;

	case 120:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == RET_GOOD)
		{
			Init_buffer_step_2 = 100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			//260100 0 26 "Clip Insert Picker up 에러."
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"260100");
				CTL_Lib.Alarm_Error_Occurrence(2300, CTL_dWARNING, mc_alarmcode);
				Init_buffer_step_2 = 1500;
			}
			else
			{
				Init_buffer_step_2 = 110;
			}
		}
		break;

	case 130:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_2 = 140;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_2 = 130;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_2 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 140:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_2 = 100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_2 = 140;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_2 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 200:
		if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][1] == WORK_BUFFER_2)
		{
			st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;
			Init_buffer_step_2 = 300;
		}
		else
		{
			Init_buffer_step_2 = 400;
		}
		break;

	case 300:
		nRet_1 = Run_Rbt_Load_Clip.Run_RemoveClip();
		if(nRet_1 == RET_GOOD)
		{
			Init_buffer_step_2 = 350;
		}
		break;

	case 350:
		st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;
		Run_Rbt_Load_Clip.RunInsertStep = 1000;
		Run_Rbt_Load_Clip.ClipPlaceStep = 0;
		Run_Rbt_Load_Clip.RunClampStep = 1100;
		Run_Rbt_Load_Clip.ClampRbtStep= 0;
		Init_buffer_step_2 = 400;

	case 400:	
		nRet_1 = CTL_Lib.Single_Move(nSite, st_motor[nSite].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_2 = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_2 = 10;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_2 = 10;
			CTL_Lib.Alarm_Error_Occurrence(6270, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1000:		//z axis부터 homecheck 하자!!
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(nSite, st_motor[nSite].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			Init_buffer_step_2 = 2000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			Init_buffer_step_2 = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6171, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1500:
		FuncRet = RET_ERROR;
		Init_buffer_step_2 = 0;
		break;		

	case 2000:
		n_motor_init[nSite][1] = CTL_YES;
		FuncRet = RET_GOOD;
		st_handler.n_workbuff_skip[WORK_BUFFER_2] = NO;
		Run_Rbt_Work_Buffer.RunBuff2Step = 100;
		GridText(IDC_CUSTOM_WORK_BUFFER, WORK_BUFFER_2 + 2, 7, "Done");

		Init_buffer_step_2 = 0;
		break;		
	}

	return FuncRet;
}

int CDialog_Work_Information::WorkBufferInit_3()
{
	int nRet_1, FuncRet = RET_PROCEED, nSite;
	double dCurrInsertPos[2]={0,}, dCurrClampPos[2]={0,};
	CString str_tmp;

	if(nBuffer_num_3 < M_WORK_BUFFER_1 || nBuffer_num_3 > M_WORK_BUFFER_4)
	{
		return RET_ERROR;
	}

	nSite = nBuffer_num_3;

	if(m_b_strop_req == TRUE)
	{
		COMI.mn_home_step[nSite] = 0;
		Init_buffer_step_3 = 1500;
	}
	switch(Init_buffer_step_3)
	{
	case 0:
		if(st_handler.n_module_skip == NO && st_handler.n_fronths_skip == NO && 
			st_handler.n_rearhs_skip == NO && st_handler.n_cliprbtpicker_skip == NO &&
			st_handler.n_sorter_skip == NO)
		{
			Init_buffer_step_3 = 100;
		}
		break;

	case 100:
		dCurrInsertPos[0] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
		dCurrClampPos[0]  = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
		dCurrInsertPos[1] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
		dCurrClampPos[1]  = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
		if(dCurrInsertPos[0] > st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE1] - st_motor[M_CLIP_INSERT_Y].mn_allow &&
			dCurrInsertPos[0] < st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE1] + st_motor[M_CLIP_INSERT_Y].mn_allow)
		{
			mn_Clipretry = 0;
			if(FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_ON ||
				FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_OFF)
			{
				FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
				Init_buffer_step_3 = 110;
				break;
			}
			if(dCurrInsertPos[1] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Z].mn_allow)
			{
				Init_buffer_step_3 = 110;
				break;
			}
		}
//		if(dCurrClampPos[0] > st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE3] - st_motor[M_M_CLAMP_RBT_Y].mn_allow &&
//			dCurrClampPos[0] < st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE3] + st_motor[M_M_CLAMP_RBT_Y].mn_allow)
//		{
			if(dCurrClampPos[1] > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_CLAMP_RBT_Z].mn_allow)
			{
				Init_buffer_step_3 = 130;
				break;
			}
//		}
		Init_buffer_step_3 = 200;
		break;

	case 110:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
			Init_buffer_step_3 = 120;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_3 = 110;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_3 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 120:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == RET_GOOD)
		{
			Init_buffer_step_3 = 100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			//260100 0 26 "Clip Insert Picker up 에러."
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"260100");
				CTL_Lib.Alarm_Error_Occurrence(2300, CTL_dWARNING, mc_alarmcode);
				Init_buffer_step_3 = 1500;
			}
			else
			{
				Init_buffer_step_3 = 110;
			}
		}
		break;

	case 130:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_3 = 140;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_3 = 130;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_3 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 140:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_3 = 100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_3 = 140;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_3 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 200:
		if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][1] == WORK_BUFFER_3)
		{
			st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;
			Init_buffer_step_3 = 300;
		}
		else
		{
			Init_buffer_step_1 = 400;
		}
		break;

	case 300:
		nRet_1 = Run_Rbt_Load_Clip.Run_RemoveClip();
		if(nRet_1 == RET_GOOD)
		{
			Init_buffer_step_3 = 350;
		}
		break;

	case 350:
		st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;
		Run_Rbt_Load_Clip.RunInsertStep = 1000;
		Run_Rbt_Load_Clip.ClipPlaceStep = 0;
		Run_Rbt_Load_Clip.RunClampStep = 1100;
		Run_Rbt_Load_Clip.ClampRbtStep= 0;
		Init_buffer_step_3 = 400;

	case 400:	
		nRet_1 = CTL_Lib.Single_Move(nSite, st_motor[nSite].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_3 = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_3 = 10;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_3 = 10;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1000:		//z axis부터 homecheck 하자!!
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(nSite, st_motor[nSite].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			Init_buffer_step_3 = 2000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			Init_buffer_step_3 = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6171, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1500:
		FuncRet = RET_ERROR;
		Init_buffer_step_3 = 0;
		break;		

	case 2000:
		n_motor_init[nSite][1] = CTL_YES;
		st_handler.n_workbuff_skip[WORK_BUFFER_3] = NO;
		FuncRet = RET_GOOD;
		GridText(IDC_CUSTOM_WORK_BUFFER, WORK_BUFFER_3 + 2, 7, "Done");
		Run_Rbt_Work_Buffer.RunBuff3Step = 100;
		Init_buffer_step_3 = 0;
		break;		
	}

	return FuncRet;
}

int CDialog_Work_Information::WorkBufferInit_4()
{
	int nRet_1, FuncRet = RET_PROCEED, nSite;
	double dCurrInsertPos[2]={0,}, dCurrClampPos[2]={0,};
	CString str_tmp;

	if(nBuffer_num_4 < M_WORK_BUFFER_1 || nBuffer_num_4 > M_WORK_BUFFER_4)
	{
		return RET_ERROR;
	}
	nSite = nBuffer_num_4;

	if(m_b_strop_req == TRUE)
	{
		COMI.mn_home_step[nSite] = 0;
		Init_buffer_step_4 = 1500;
	}
	switch(Init_buffer_step_4)
	{
	case 0:
		if(st_handler.n_module_skip == NO && st_handler.n_fronths_skip == NO && 
			st_handler.n_rearhs_skip == NO && st_handler.n_cliprbtpicker_skip == NO &&
			st_handler.n_sorter_skip == NO)
		{
			Init_buffer_step_4 = 100;
		}
		break;

	case 100:
		dCurrInsertPos[0] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Y);
		dCurrClampPos[0]  = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Y);
		dCurrInsertPos[1] = COMI.Get_MotCurrentPos(M_CLIP_INSERT_Z);
		dCurrClampPos[1]  = COMI.Get_MotCurrentPos(M_M_CLAMP_RBT_Z);
		if(dCurrInsertPos[0] > st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE1] - st_motor[M_CLIP_INSERT_Y].mn_allow &&
			dCurrInsertPos[0] < st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_WORK_PLACE1] + st_motor[M_CLIP_INSERT_Y].mn_allow)
		{
			mn_Clipretry = 0;
			if(FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_ON ||
				FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_ON) == IO_OFF)
			{
				FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
				Init_buffer_step_4 = 110;
				break;
			}
			if(dCurrInsertPos[1] > st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_CLIP_INSERT_Z].mn_allow)
			{
				Init_buffer_step_4 = 110;
				break;
			}
		}
		if(dCurrClampPos[0] > st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE4] - st_motor[M_M_CLAMP_RBT_Y].mn_allow &&
			dCurrClampPos[0] < st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE4] + st_motor[M_M_CLAMP_RBT_Y].mn_allow)
		{
			if(dCurrClampPos[1] > st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP] + st_motor[M_M_CLAMP_RBT_Z].mn_allow)
			{
				Init_buffer_step_4 = 130;
				break;
			}
		}
		Init_buffer_step_4 = 200;
		break;

	case 110:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
			Init_buffer_step_4 = 120;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_4 = 110;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_4 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 120:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == RET_GOOD)
		{
			Init_buffer_step_4 = 100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			//260100 0 26 "Clip Insert Picker up 에러."
			mn_Clipretry++;
			if(mn_Clipretry > 3)
			{
				mn_Clipretry = 0;
				sprintf(mc_alarmcode,"260100");
				CTL_Lib.Alarm_Error_Occurrence(2300, CTL_dWARNING, mc_alarmcode);
				Init_buffer_step_4 = 1500;
			}
			else
			{
				Init_buffer_step_4 = 110;
			}
		}
		break;

	case 130:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].md_pos[GLIPPER_SAFETY], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_4 = 140;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_4 = 130;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_4 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 140:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].md_pos[Z_LD_SAFETY_UP], st_basic.nManualSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_4 = 100;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_4 = 140;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_4 = 1500;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 200:
		if(st_sync.n_module_buffer_action_site[LDCLIP_SITE][1] == WORK_BUFFER_4)
		{
			st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;
			Init_buffer_step_4 = 300;
		}
		else
		{
			Init_buffer_step_4 = 400;
		}
		break;

	case 300:
		nRet_1 = Run_Rbt_Load_Clip.Run_RemoveClip();
		if(nRet_1 == RET_GOOD)
		{
			Init_buffer_step_4 = 350;
		}
		break;

	case 350:
		st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;
		Run_Rbt_Load_Clip.RunInsertStep = 1000;
		Run_Rbt_Load_Clip.ClipPlaceStep = 0;
		Run_Rbt_Load_Clip.RunClampStep = 1100;
		Run_Rbt_Load_Clip.ClampRbtStep= 0;
		Init_buffer_step_4 = 400;

	case 400:	
		nRet_1 = CTL_Lib.Single_Move(nSite, st_motor[nSite].md_pos[WORK_SAFETY], st_basic.nRunSpeed);
		if(nRet_1 == BD_GOOD)
		{
			Init_buffer_step_4 = 1000;
		}
		else if(nRet_1 == BD_RETRY)
		{
			Init_buffer_step_4 = 10;
		}
		else if(nRet_1 != CTLBD_RET_PROCEED)
		{
			Init_buffer_step_4 = 10;
			CTL_Lib.Alarm_Error_Occurrence(6170, CTL_dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 1000:		//z axis부터 homecheck 하자!!
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(nSite, st_motor[nSite].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			Init_buffer_step_4 = 2000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			Init_buffer_step_4 = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6171, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1500:
		FuncRet = RET_ERROR;
		Init_buffer_step_4 = 0;
		break;		

	case 2000:
		n_motor_init[nSite][1] = CTL_YES;
		st_handler.n_workbuff_skip[WORK_BUFFER_4] = NO;
		FuncRet = RET_GOOD;
		Run_Rbt_Work_Buffer.RunBuff4Step = 100;
		GridText(IDC_CUSTOM_WORK_BUFFER, WORK_BUFFER_4 + 2, 7, "Done");
		Init_buffer_step_4 = 0;
		break;		
	}

	return FuncRet;
}

int CDialog_Work_Information::SorterRbtInit()
{
	int nRet_1, FuncRet = RET_PROCEED;
	if(m_b_strop_req == TRUE)
	{
		COMI.mn_home_step[M_RBT_SORTER_Z] = 0;
		InitSortStep = 1500;
	}
	switch(InitSortStep)
	{
	case 0:
		ActionPicker[0][D_EXIST] = YES;
		Set_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		InitSortStep = 100;
		break;

	case 100:
		nRet_1 = Get_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			InitSortStep = 1000;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6130, st_work.mn_run_status,mc_alarmcode);
		}
		break;
		
	case 1000:
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_RBT_SORTER_Z, st_motor[M_RBT_SORTER_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitSortStep = 2000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitSortStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6131, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1500:
		FuncRet = RET_ERROR;
		InitSortStep = 0;
		break;

	case 2000:
		st_handler.n_sorter_skip = NO;
//		if(st_handler.n_sorter_skip == YES && 
//			st_sync.n_module_ldrbt_to_work[SORTER_SITE][st_sync.n_module_buffer_action_site[SORTER_SITE][0]][0] != CTL_NO)
//		{
//			st_sync.n_module_ldrbt_to_work[SORTER_SITE][st_sync.n_module_buffer_action_site[SORTER_SITE][0]][0] = CTL_NO;
//		}
		n_motor_init[SORTERRBT_SITE][1] = CTL_YES;
		Run_Rbt_Sorter.RunSortStep = 100;		
		Run_Rbt_Sorter.Module_Pick_Step = 0;
		GridText(IDC_CUSTOM_WORK_UNLOAD, 2, 4, "Done");
		InitSortStep = 0;
		FuncRet = RET_GOOD;
		break;		
	}

	return FuncRet;
}

int CDialog_Work_Information::UldRbtInit()
{
	int nRet_1, FuncRet = RET_PROCEED;
	if(m_b_strop_req == TRUE)
	{
		COMI.mn_home_step[M_RBT_UNLOAD_Z] = 0;
		InitUldStep = 1500;
	}
	switch(InitUldStep)
	{
	case 0:
		ActionPicker[1][D_EXIST] = YES;
		Set_PickerUpDown(UNLOADER_SITE, IO_OFF, ActionPicker);
		InitUldStep = 100;
		break;

	case 100:
		nRet_1 = Get_PickerUpDown(SORTER_SITE, IO_OFF, ActionPicker);
		if(nRet_1 == RET_GOOD)
		{
			InitUldStep = 1000;
		}
		else if (nRet_1 != RET_PROCEED)
		{
			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6130, st_work.mn_run_status,mc_alarmcode);
		}
		break;
		
	case 1000:
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_RBT_UNLOAD_Z, st_motor[M_RBT_UNLOAD_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			InitUldStep = 2000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			InitUldStep = 1000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
			st_work.mn_run_status = CTL_dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(6136, st_work.mn_run_status, COMI.mc_alarmcode);
		}
		break;

	case 1500:
		FuncRet = RET_ERROR;
		InitUldStep = 0;
		break;

	case 2000:
		st_handler.n_sorter_skip = NO;
		n_motor_init[UNLOADRBT_SITE][1] = CTL_YES;
		FuncRet = RET_GOOD;
		Run_Rbt_Sorter.RunUldStep = 100;		
		GridText(IDC_CUSTOM_WORK_UNLOAD, 2, 4, "Done");
		InitUldStep = 0;
		break;		
	}

	return FuncRet;
}

void CDialog_Work_Information::Set_PickerUpDown(int nSite, int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i;

	for (i = 0; i < PICKER_NUM; i++)
	{
		if(nSite == LDMODULE_SITE)
		{
			if(PickerInfo[D_EXIST][i] == YES)
			{
				FAS_IO.set_out_bit(st_io.o_ld_module_picker_updn[i], OnOff);
			}
			b_PickerFlag[i] = FALSE;
			l_PickerWaitTime[i][0] = GetCurrentTime();
		}
		else if(nSite == LEFTRBTSINK_SITE)
		{
			if(PickerInfo[D_EXIST][i] == YES)
			{
				FAS_IO.set_out_bit(st_io.o_hs_fwd_rbt_picker_picker_updn[i], OnOff);
			}
			b_PickerFlag[i] = FALSE;
			l_PickerWaitTime[i][0] = GetCurrentTime();
		}
		else if(nSite == RIGHTRBTSINK_SITE)
		{
			if(PickerInfo[D_EXIST][i] == YES)
			{
				FAS_IO.set_out_bit(st_io.o_hs_bwd_rbt_picker_picker_updn[i], OnOff);
			}
			b_PickerFlag[i] = FALSE;
			l_PickerWaitTime[i][0] = GetCurrentTime();
		}
		else if(nSite == SORTER_SITE)
		{
			if(PickerInfo[0][D_EXIST] == YES)
			{
				FAS_IO.set_out_bit(st_io.o_sorter_picker_rbt_updn_onoff, OnOff);
			}
			else
			{
				FAS_IO.set_out_bit(st_io.o_sorter_picker_rbt_updn_onoff, !OnOff);
			}
			b_PickerFlag[i] = FALSE;
			l_PickerWaitTime[i][0] = GetCurrentTime();
		}
	}
}

int CDialog_Work_Information::Get_PickerUpDown(int nSite, int OnOff, int PickerInfo[D_INFOSIZE][PICKER_NUM])
{
	int i, FuncRet = RET_PROCEED;
	int RetVal[PICKER_NUM] = {0,};
	char jamcode[10] = {0,};
	CString str;
	
	if(st_basic.n_mode_device != CTL_YES)
	{
	}

	for(i = 0 ; i < PICKER_NUM ; i++)
	{
		RetVal[i] = NO;

		if(OnOff == IO_ON) //Picker ON (Down이라면..)
		{
			if(nSite == LDMODULE_SITE)
			{
				if(PickerInfo[D_EXIST][i] == YES) //동작 시킨 피커면!
				{
					if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_ON) == IO_ON
						&& FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[i], IO_OFF) == IO_OFF)
					{
						b_PickerFlag[i] = TRUE;
						l_PickerWaitTime[i][0] = GetCurrentTime();   
					}
					else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_ON) == IO_ON
						&& FAS_IO.get_in_bit(st_io.i_ld_module_picker_up_chk[i], IO_OFF) == IO_OFF)
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();  
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
						
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[38] + (i*2))
						{
							RetVal[i] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();  
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						
						if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[38] + (i*2))
						{
							//400200 1 40 "Module Picker #1 Down Check Error."						
							sprintf(jamcode, "40020%d", i);
							sprintf(mc_alarmcode, "40020%d", i);						
							alarm.mstr_pcode = _T(jamcode); 
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							alarm.mn_alarm_assign_section = 186;

							FuncRet = RET_ERROR;
						}
					}
				}
				else
				{
					RetVal[i] = YES;
				}
			}
			else if(nSite == LEFTRBTSINK_SITE)
			{
				if(PickerInfo[D_EXIST][i] == YES) //동작 시킨 피커면!
				{
					if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[i], IO_ON == IO_ON) &&
						FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_up_chk[i], IO_OFF) == IO_OFF)
					{
						b_PickerFlag[i] = TRUE;
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[i], IO_ON) == IO_ON &&
						FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_up_chk[i], IO_OFF) == IO_OFF)
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];

						if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[42] + (i*2))
						{
							RetVal[i] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[1][0];
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[42] + (i*2))
						{
							//410200 1 40 "Heat Sink Picker #1 Down Check Error.[PS0308]."					
							sprintf(jamcode, "41020%d", i);
							sprintf(mc_alarmcode, "41020%d", i);
							alarm.mstr_pcode = _T(jamcode); 
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							alarm.mn_alarm_assign_section = 106;

							FuncRet = RET_ERROR;
						}			
						
					}	
				}
				else
				{
					RetVal[i] = YES;
				}
			}
			else if(nSite == RIGHTRBTSINK_SITE)
			{
				if(PickerInfo[D_EXIST][i] == YES) //동작 시킨 피커면!
				{
					if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[i], IO_ON == IO_ON) &&
						FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[i], IO_OFF) == IO_OFF)
					{
						b_PickerFlag[i] = TRUE;
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[i], IO_ON) == IO_ON &&
						FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[i], IO_OFF) == IO_OFF)
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];

						if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[46] + (i*2))
						{
							RetVal[i] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[1][0];
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[46] + (i*2))
						{
							//420200 1 40 "Heat Sink Picker #1 Down Check Error.[PS0308]."					
							sprintf(mc_alarmcode, "42020%d", i);
							CTL_Lib.Alarm_Error_Occurrence(2220, CTL_dWARNING, mc_alarmcode);

							FuncRet = RET_ERROR;
						}			
						
					}		
				}
				else
				{
					RetVal[i] = YES;
				}
			}
		}
		else //Picker OFF (UP이라면..)
		{
			if(nSite == LDMODULE_SITE)
			{
				if(PickerInfo[D_EXIST][i] == YES) //동작 시킨 피커면!
				{
					if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_OFF) == IO_OFF
						&& FAS_IO.Get_In_Bit(st_io.i_ld_module_picker_up_chk[i]) == IO_ON)
					{
						b_PickerFlag[i] = TRUE;
						l_PickerWaitTime[i][0] = GetCurrentTime();   
					}
					else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_ld_module_picker_dn_chk[i], IO_OFF) == IO_OFF
						&& FAS_IO.Get_In_Bit(st_io.i_ld_module_picker_up_chk[i]) == IO_ON)
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();  
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
						
						if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[38] + (i*2))
						{
							RetVal[i] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();  
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						
						if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[38] + (i*2))
						{
							if(st_handler.cwnd_list != NULL)
							{
								str = _T("[UPICKER Error] Picker up sensor check error.");
								sprintf(st_msg.c_normal_msg,"%s", str);
								st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
								Func.On_LogFile_Add(0, str);
								Func.On_LogFile_Add(99, str);
							}
							//400100 1 40 "Module Picker #1 Up Check Error."
							sprintf(jamcode, "40010%d", i);
							sprintf(mc_alarmcode,  "40010%d", i);					
							alarm.mstr_pcode = _T(jamcode); 
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							//st_handler.i_run_status = dWARNING;

							alarm.mn_alarm_assign_section = 187;

							FuncRet = RET_ERROR;
						}
					}
				}
				else
				{
					RetVal[i] = YES;
				}
			}
			else if(nSite == LEFTRBTSINK_SITE)
			{
				if(PickerInfo[D_EXIST][i] == YES)
				{ 
					if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_up_chk[i], IO_ON == IO_ON) &&
						FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[i], IO_OFF) == IO_OFF)
					{
						b_PickerFlag[i] = TRUE;
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_up_chk[i], IO_ON == IO_ON) &&
						FAS_IO.get_in_bit(st_io.i_hs_fwd_picker_dn_chk[i], IO_OFF) == IO_OFF)
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[42] + (i*2))
						{
							RetVal[i] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[1][0];
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[42] + (i*2))
						{
							//410100 1 40 "Heat Sink Picker #1 Up Check Error.[PS0300]."				
							sprintf(jamcode, "41010%d", i);
							sprintf(mc_alarmcode, "41010%d", i);
							alarm.mstr_pcode = _T(jamcode); 
							alarm.mn_count_mode = 0;	
							alarm.mn_type_mode = eWARNING; 
							alarm.mn_alarm_assign_section = 186;

							FuncRet = RET_ERROR;
						}	
					}
				}
				else
				{
					RetVal[i] = YES;
				}
			}
			else if(nSite == RIGHTRBTSINK_SITE)
			{

				if(PickerInfo[D_EXIST][i] == YES)
				{ 
					if(b_PickerFlag[i] == FALSE && FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[i], IO_ON == IO_ON) &&
						FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[i], IO_OFF) == IO_OFF)
					{
						b_PickerFlag[i] = TRUE;
						l_PickerWaitTime[i][0] = GetCurrentTime();
					}
					else if(b_PickerFlag[i] == TRUE && FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_up_chk[i], IO_ON == IO_ON) &&
						FAS_IO.get_in_bit(st_io.i_hs_bwd_picker_dn_chk[i], IO_OFF) == IO_OFF)
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[i][0];
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						if(l_PickerWaitTime[i][2] > st_wait.n_on_wait_time[46] + (i*2))
						{
							RetVal[i] = YES;
						}
					}
					else
					{
						l_PickerWaitTime[i][1] = GetCurrentTime();
						l_PickerWaitTime[i][2] = l_PickerWaitTime[i][1] - l_PickerWaitTime[1][0];
						if(l_PickerWaitTime[i][2] < 0)
						{
							l_PickerWaitTime[i][0] = GetCurrentTime();
						}
						if(l_PickerWaitTime[i][2] > st_wait.n_limit_wait_time[46] + (i*2))
						{
							//420100 1 40 "Heat Sink Picker #1 Up Check Error.[PS0300]."				
							sprintf(mc_alarmcode, "42010%d", i);
							CTL_Lib.Alarm_Error_Occurrence(2221, CTL_dWARNING, mc_alarmcode);

							FuncRet = RET_ERROR;
						}	
					}
				}
				else
				{
					RetVal[i] = YES;
				}
			}
		}
	}
	
	if(RetVal[0] == YES && RetVal[1] == YES) 
	{
		FuncRet = RET_GOOD;
	}

	return FuncRet;
}

void CDialog_Work_Information::OnStop() 
{
	m_b_strop_req = TRUE;	
}

void CDialog_Work_Information::OnClipRbtInit() 
{
	CDialog_Select msg_dlg;
	int nResponse;

 	st_msg.mstr_confirm_msg = _T("Clip Clamp 로봇을 초기화를 진행행하시겠습니까?");
	if(st_handler.mn_language == LANGUAGE_ENGLISH) 
	{
		st_msg.mstr_confirm_msg = _T("Are you sure you want to initialize the Clip Clamp");
	}

	nResponse = msg_dlg.DoModal();
	if(nResponse == IDOK)
	{
		m_b_strop_req = FALSE;
		m_clip_clamp_rbt.EnableWindow(FALSE);
		SetTimer(TMR_MOTOR_CLIP_CLAMP, 100, NULL);
	}
}

int CDialog_Work_Information::ClipClampRbtInit()
{
	int nFuncRet = RET_PROCEED;
	int nRet_1 = RET_PROCEED, nRet_2 = RET_PROCEED;

	if(m_b_strop_req == TRUE)
	{
		COMI.mn_home_step[M_M_CLAMP_RBT_Z] = 0;
		COMI.mn_home_step[M_CLIP_INSERT_Z] = 0;
		ClipInitStep = 2950;
	}

	switch(ClipInitStep)
	{
	case 0:
		ClipInitStep = 10;
		break;

	case 10:
		if(FAS_IO.get_in_bit(st_io.i_clip_rbt_up_chk, IO_OFF) == IO_ON ||
			FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_ON) == IO_OFF)
		{
			FAS_IO.OnCylinderAction(st_io.o_clip_rbt_updn_onoff, IO_OFF);
			ClipInitStep = 100;
		}
		else
		{
			ClipInitStep = 1000;
		}
		break;

	case 100:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_rbt_updn_onoff, st_io.i_clip_rbt_up_chk, st_io.i_clip_rbt_dn_chk,
			IO_ON, st_wait.n_on_wait_time[64], st_wait.n_limit_wait_time[64]);
		if(nRet_1 == RET_GOOD)
		{
			ClipInitStep = 1100;
		}
		else if(nRet_1 != RET_PROCEED)
		{
			//260100 0 26 "Clip Insert Picker up 에러."
			mn_Clipretry = 0;
			sprintf(mc_alarmcode,"260100");
			CTL_Lib.Alarm_Error_Occurrence(2300, CTL_dWARNING, mc_alarmcode);
			ClipInitStep = 2950;
		}
		break;
//
//	case 1000:
//		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_BUFFER, st_motor[M_CLIP_BUFFER].mn_homecheck_method, MOT_TIMEOUT);
//
//		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
//		{
//			ClipInitStep = 1100;
//		}
//		else if (nRet_1 == CTLBD_RET_RETRY)
//		{
//			ClipInitStep = 1000;
//		}
//		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
//		{
//			CTL_Lib.Alarm_Error_Occurrence(2302, CTL_dWARNING, COMI.mc_alarmcode);
//			ClipInitStep = 2950;
//
//		}
//		break;

	case 1100:
		CActionPicker[D_EXIST] = YES;
		FAS_IO.set_out_bit(st_io.o_clip_rbt_updn_onoff, IO_OFF);
		l_UntilWaitTime[0] = GetCurrentTime();
		ClipInitStep = 1200;
		break;

	case 1200:
		l_UntilWaitTime[1] = GetCurrentTime();
		l_UntilWaitTime[2] = l_UntilWaitTime[1] - l_UntilWaitTime[0];
		if(l_UntilWaitTime[2] < 0) l_UntilWaitTime[0] = GetCurrentTime();
		if(l_UntilWaitTime[2] < 1500) break;
		if(FAS_IO.get_in_bit(st_io.i_clip_rbt_dn_chk, IO_OFF) == IO_OFF && FAS_IO.Get_In_Bit(st_io.i_clip_rbt_up_chk) == IO_ON)
		{
			ClipInitStep = 2000;
		}
		else
		{
			sprintf(mc_alarmcode,"260100");
			CTL_Lib.Alarm_Error_Occurrence(2303, CTL_dWARNING, mc_alarmcode);
			ClipInitStep = 2950;
		}
		break;

	case 2000:
		// *************************************************************
		//  홈체크를 한다.                                              
		// *************************************************************
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_INSERT_Z, st_motor[M_CLIP_INSERT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			mn_retry = 0;
			ClipInitStep = 2300;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2000;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2304, CTL_dWARNING, COMI.mc_alarmcode);
			ClipInitStep = 2950;

		}
		break;
		
	case 2300:
		ClipInitStep = 2500;
		break;
		FAS_IO.OnCylinderAction(st_io.o_clip_buffer_updn_onoff, IO_OFF);
		ClipInitStep = 2400;
		break;

	case 2400:
		nRet_1 = FAS_IO.OnCylinderActionCheck(st_io.o_clip_buffer_updn_onoff, st_io.i_clip_buff_dn_chk, st_io.i_clip_buff_up_chk,
			IO_ON, st_wait.n_on_wait_time[69], st_wait.n_limit_wait_time[69]);
		if (nRet_1 == RET_GOOD)
		{
			ClipInitStep = 2500;
		}
		else if(nRet_1 != RET_PROCEED)
		{//180801 0 18 "Clip buffer down 체크 에러가 발생했습니다."
			sprintf(mc_alarmcode,"180801");
			CTL_Lib.Alarm_Error_Occurrence(2305, CTL_dWARNING, mc_alarmcode);
			ClipInitStep = 2950;

		}
		break;

	case 2500:
		nRet_1 = COMI.HomeCheck_Mot(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 2600;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2500;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2306, CTL_dWARNING, COMI.mc_alarmcode);
			ClipInitStep = 2950;

		}
		break;

	case 2600:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_GRIPPER, st_motor[M_M_CLAMP_RBT_GRIPPER].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 2700;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2600;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2307, CTL_dWARNING, COMI.mc_alarmcode);
			ClipInitStep = 2950;

		}
		break;

	case 2700:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_Z, st_motor[M_M_CLAMP_RBT_Z].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 2800;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2700;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2308, CTL_dWARNING, COMI.mc_alarmcode);
			ClipInitStep = 2950;

		}
		break;

	case 2800:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 2810;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2800;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);
			ClipInitStep = 2950;

		}
		break;

	case 2810:
		nRet_1 = CTL_Lib.Single_Move(M_M_CLAMP_RBT_Y, st_motor[M_M_CLAMP_RBT_Y].md_pos[Y_CLAMP_WORK_PLACE1], st_basic.nManualSpeed);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 2900;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2810;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);
			ClipInitStep = 2950;

		}
		break;

	case 2900:
		nRet_1 = Run_Rbt_Load_Clip.Run_RemoveClip();
		if(nRet_1 == RET_GOOD)
		{
			ClipInitStep = 2910;
		}
		break;

	case 2910:
		nRet_1 = COMI.HomeCheck_Mot(M_M_CLIP_CLAMP, st_motor[M_M_CLIP_CLAMP].mn_homecheck_method, MOT_TIMEOUT);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 2920;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2910;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);
			ClipInitStep = 2950;

		}
		break;

	case 2920:
		nRet_1 = CTL_Lib.Single_Move(M_CLIP_INSERT_Y, st_motor[M_CLIP_INSERT_Y].md_pos[Y_CLIP_PICKPOS], st_basic.nManualSpeed);

		if(nRet_1 == CTLBD_RET_GOOD)			// 정상적으로 Home Check가 끝났을 경우.
		{
			ClipInitStep = 3000;
		}
		else if (nRet_1 == CTLBD_RET_RETRY)
		{
			ClipInitStep = 2810;
		}
		else if ((nRet_1 != CTLBD_RET_GOOD && nRet_1 != CTLBD_RET_PROCEED) )
		{
 			st_work.mn_run_status = dWARNING;
			CTL_Lib.Alarm_Error_Occurrence(2309, CTL_dWARNING, COMI.mc_alarmcode);
			ClipInitStep = 2950;

		}
		break;

	case 2950:
		nFuncRet = RET_ERROR;
		ClipInitStep = 0;
		break;		

	case 3000:
		n_motor_init[LDRBTCLIPINSERT_SITE][1] = CTL_YES;
		n_motor_init[LDRBTCLIPCLAMP_SITE][1] = CTL_YES;
		st_sync.n_module_ldrbt_clamp_req[0] = CTL_NO;
		Run_Rbt_Load_Clip.RunInsertStep = 1000;
		Run_Rbt_Load_Clip.RunClampStep = 1000;
		Run_Rbt_Load_Clip.ClampRbtStep = 0;
		ClipInitStep = 0;
		nFuncRet = RET_GOOD;
		break;
	}

	return nFuncRet;
}

BOOL CDialog_Work_Information::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}

		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}
