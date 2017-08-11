	// Animate.cpp : implementation file
//
#include "stdafx.h"
#include "Animate.h"
#include "Public_Function.h"
#include "FastechPublic_IO.h"
#include "math.h"

#include "Variable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimate

#define getrandom(min,max) ((rand()%(int)(((max)+1)-(min)))+(min));

//*********************************************
//					CAxisMod
//*********************************************
CAxisMod::CAxisMod() : m_pBitAxis(NULL),m_HomePoint(100,100),m_dEqWidth(1),m_nImgCurPos_X(1),m_nImgCurPos_Y(1)
					  ,m_dRatio(1),m_dEqCurPos_X(1),m_dEqCurPos_Y(1)
{

}

CAxisMod::~CAxisMod()
{
	m_BitAxis.DeleteObject();
	m_BitAxisMask.DeleteObject();
}

void CAxisMod::ComverEqPosToImgPos()
{
	m_nImgCurPos_X =    m_HomePoint.x + int(m_dEqCurPos_X * m_dRatio)  - int(m_ImgSize.cx/2.0+ 0.5) ;
	m_nImgCurPos_Y =    m_HomePoint.y + int(m_dEqCurPos_Y * m_dRatio)  - int(m_ImgSize.cy/2.0+ 0.5) ;
}
void CAxisMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk)
{
	m_DcAxis.CreateCompatibleDC(pDC);
	m_BitAxis.LoadBitmap(ImgID);
	m_pBitAxis = (CBitmap *)m_DcAxis.SelectObject(&m_BitAxis);

	m_DcAxisMask.CreateCompatibleDC(pDC);
	m_BitAxisMask.LoadBitmap(ImgIDMsk);
	m_pBitAxisMask = (CBitmap *)m_DcAxisMask.SelectObject(&m_BitAxisMask);	
	
	BITMAP bs;
	m_BitAxis.GetBitmap(&bs);
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;	
}

void CAxisMod::DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();

	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx, m_ImgSize.cy, &m_DcAxisMask, 0, 0, SRCAND);
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx, m_ImgSize.cy, &m_DcAxis, 0, 0, SRCPAINT);
}


//*********************************************
//					CGlass
//*********************************************
CEqGlass::CEqGlass():m_sizGlassSize(80,140) ,m_nGlassPos(100,100),m_nDegree(0),m_nDir(2)
{}	 
CEqGlass::~CEqGlass()
{} 

void CEqGlass::CellRectCf(CRect &CellRect)
{

	return;
	CellRect.right	-= 10;
	CellRect.top	+= 10;
	

	const int deg =  abs(m_nDegree%180) /45;

	
	switch(m_nDir)
	{
	case 0:
		switch(deg)
		{
		case 0:
			CellRect.right	+= 10;
			CellRect.top	+= 10;
			break;

		case 1:
			CellRect.left	+= 10;
			CellRect.top	+= 10;
			break;

		case 3:
			CellRect.right	+= 10;
			CellRect.top	+= 10;
			break;
		}
		break;

	case 1:
		CellRect.left	+= 10;
		CellRect.bottom -= 10;
		break;

	case 2:
		CellRect.right	-= 10;
		CellRect.top	+= 10;
//
//		switch(deg)
//		{
//		case 0:
//			CellRect.left 	-= 10;
//			CellRect.top	+= 10;
//			break;
//
//		case 1:
//			CellRect.right	-= 10;
//			CellRect.top	+= 10;
//			break;
//
//		case 3:
//			CellRect.right	-= 10;
//			CellRect.top	+= 10;
//			break;
//		}

		break;

	case 3:
		CellRect.right	-= 10;
		CellRect.bottom	-= 10;
		break;
	}	
}

void CEqGlass::RotateRectAngle(CRect rect,double nDegree, CRect rectCen)
{
	CPoint pointCenter;
	double cosine = cos(nDegree * 3.14159265359 / 180.);
	double sine	  = sin(nDegree * 3.14159265359 / 180.);
	// 중심축을 기준으로 회전
	pointCenter.x = rect.CenterPoint().x;
	pointCenter.y = rect.CenterPoint().y;

	int x0 = pointCenter.x;
	int y0 = pointCenter.y;

	double x1 = cosine	*	(rect.TopLeft().x - x0)		- sine		*	(rect.TopLeft().y - y0)		+ x0;
	double y1 = sine	*   (rect.TopLeft().x - x0)		+ cosine	*	(rect.TopLeft().y - y0)		+ y0;

	double x2 = cosine	*	(rect.BottomRight().x - x0) - sine		*   (rect.TopLeft().y - y0)		+ x0;
	double y2 = sine	*   (rect.BottomRight().x - x0) + cosine	*	(rect.TopLeft().y - y0)		+ y0;

	double x3 = cosine	*	(rect.BottomRight().x - x0) - sine		*   (rect.BottomRight().y - y0) + x0;
	double y3 = sine	*   (rect.BottomRight().x - x0) + cosine	*	(rect.BottomRight().y - y0) + y0;

	double x4 = cosine	*	(rect.TopLeft().x - x0)		- sine		*   (rect.BottomRight().y - y0) + x0;
    double y4 = sine	*	(rect.TopLeft().x - x0)		+ cosine	*   (rect.BottomRight().y - y0) + y0;	

	m_arrGlassPoint[0].x = int(x1); 
	m_arrGlassPoint[0].y = int(y1);
	m_arrGlassPoint[1].x = int(x2); 
	m_arrGlassPoint[1].y = int(y2); 
	m_arrGlassPoint[2].x = int(x3); 
	m_arrGlassPoint[2].y = int(y3); 
	m_arrGlassPoint[3].x = int(x4); 
	m_arrGlassPoint[3].y = int(y4);		
}

void CEqGlass::DrawImg(CDC &dc)
{
//	m_sizGlassSize.cx = GdbSetCellRecipe.fCellInfoShort/7.5;
//	m_sizGlassSize.cy = GdbSetCellRecipe.fCellInfoLong/7.5;

	CRect CellRect;
	CBrush tmpBrush,tmpBrush1,*pBrush;
	CellRect.left	 = m_nGlassPos.x - (m_sizGlassSizeMod.cx / 2);
	CellRect.top	 = m_nGlassPos.y - (m_sizGlassSizeMod.cy / 2);
	CellRect.right	 = m_nGlassPos.x + (m_sizGlassSizeMod.cx / 2);
	CellRect.bottom	 = m_nGlassPos.y + (m_sizGlassSizeMod.cy / 2);
	RotateRectAngle(CellRect, m_nDegree, CellRect);
//	tmpBrush.CreateSolidBrush(GdbSetColor.colorLightGray);
	pBrush = dc.SelectObject(&tmpBrush);
	dc.Polygon(m_arrGlassPoint, 4);
	CellRectCf(CellRect);
	RotateRectAngle(CellRect, m_nDegree, CellRect);
//	tmpBrush1.CreateSolidBrush(GdbSetColor.colorUser3);
	dc.SelectObject(&tmpBrush1);
	dc.Polygon(m_arrGlassPoint, 4);
	dc.SelectObject(pBrush);
	tmpBrush1.DeleteObject();
	tmpBrush.DeleteObject();

}

//*********************************************
//					CGlass2
//*********************************************
CEqGlass2::CEqGlass2():m_sizGlassSize2(80,140) ,m_nGlassPos2(100,100),m_nDegree2(0),m_nDir2(2)
{}	 
CEqGlass2::~CEqGlass2()
{} 

void CEqGlass2::CellRectCf2(CRect &CellRect2)
{

	return;
	CellRect2.right	-= 10;
	CellRect2.top	+= 10;
	

	const int deg2 =  abs(m_nDegree2%180) /45;

	
	switch(m_nDir2)
	{
	case 0:
		switch(deg2)
		{
		case 0:
			CellRect2.right	+= 10;
			CellRect2.top	+= 10;
			break;

		case 1:
			CellRect2.left	+= 10;
			CellRect2.top	+= 10;
			break;

		case 3:
			CellRect2.right	+= 10;
			CellRect2.top	+= 10;
			break;
		}
		break;

	case 1:
		CellRect2.left	+= 10;
		CellRect2.bottom -= 10;
		break;

	case 2:
		CellRect2.right	-= 10;
		CellRect2.top	+= 10;
//
//		switch(deg)
//		{
//		case 0:
//			CellRect.left 	-= 10;
//			CellRect.top	+= 10;
//			break;
//
//		case 1:
//			CellRect.right	-= 10;
//			CellRect.top	+= 10;
//			break;
//
//		case 3:
//			CellRect.right	-= 10;
//			CellRect.top	+= 10;
//			break;
//		}

		break;

	case 3:
		CellRect2.right	-= 10;
		CellRect2.bottom	-= 10;
		break;
	}	
}

void CEqGlass2::RotateRectAngle2(CRect rect2,double nDegree2, CRect rectCen2)
{
	CPoint pointCenter2;
	double cosine2 = cos(nDegree2 * 3.14159265359 / 180.);
	double sine2	  = sin(nDegree2 * 3.14159265359 / 180.);
	// 중심축을 기준으로 회전
	pointCenter2.x = rect2.CenterPoint().x;
	pointCenter2.y = rect2.CenterPoint().y;

	int x0 = pointCenter2.x;
	int y0 = pointCenter2.y;

	double x1 = cosine2	*	(rect2.TopLeft().x - x0)		- sine2		*	(rect2.TopLeft().y - y0)		+ x0;
	double y1 = sine2	*   (rect2.TopLeft().x - x0)		+ cosine2	*	(rect2.TopLeft().y - y0)		+ y0;

	double x2 = cosine2	*	(rect2.BottomRight().x - x0) - sine2		*   (rect2.TopLeft().y - y0)		+ x0;
	double y2 = sine2	*   (rect2.BottomRight().x - x0) + cosine2	*	(rect2.TopLeft().y - y0)		+ y0;

	double x3 = cosine2	*	(rect2.BottomRight().x - x0) - sine2		*   (rect2.BottomRight().y - y0) + x0;
	double y3 = sine2	*   (rect2.BottomRight().x - x0) + cosine2	*	(rect2.BottomRight().y - y0) + y0;

	double x4 = cosine2	*	(rect2.TopLeft().x - x0)		- sine2		*   (rect2.BottomRight().y - y0) + x0;
    double y4 = sine2	*	(rect2.TopLeft().x - x0)		+ cosine2	*   (rect2.BottomRight().y - y0) + y0;	

	m_arrGlassPoint2[0].x = int(x1); 
	m_arrGlassPoint2[0].y = int(y1);
	m_arrGlassPoint2[1].x = int(x2); 
	m_arrGlassPoint2[1].y = int(y2); 
	m_arrGlassPoint2[2].x = int(x3); 
	m_arrGlassPoint2[2].y = int(y3); 
	m_arrGlassPoint2[3].x = int(x4); 
	m_arrGlassPoint2[3].y = int(y4);		
}

void CEqGlass2::DrawImg2(CDC &dc)
{
//	m_sizGlassSize.cx = GdbSetCellRecipe.fCellInfoShort/7.5;
//	m_sizGlassSize.cy = GdbSetCellRecipe.fCellInfoLong/7.5;

	CRect CellRect2;
	CBrush tmpBrush,tmpBrush2,*pBrush2;
	CellRect2.left	 = m_nGlassPos2.x - (m_sizGlassSizeMod2.cx / 2);
	CellRect2.top	 = m_nGlassPos2.y - (m_sizGlassSizeMod2.cy / 2);
	CellRect2.right	 = m_nGlassPos2.x + (m_sizGlassSizeMod2.cx / 2);
	CellRect2.bottom	 = m_nGlassPos2.y + (m_sizGlassSizeMod2.cy / 2);
	RotateRectAngle2(CellRect2, m_nDegree2, CellRect2);
//	tmpBrush.CreateSolidBrush(GdbSetColor.colorLightGray);
	pBrush2 = dc.SelectObject(&tmpBrush2);
	dc.Polygon(m_arrGlassPoint2, 4);
	CellRectCf2(CellRect2);
	RotateRectAngle2(CellRect2, m_nDegree2, CellRect2);
//	tmpBrush1.CreateSolidBrush(GdbSetColor.colorUser3);
	dc.SelectObject(&tmpBrush2);
	dc.Polygon(m_arrGlassPoint2, 4);
	dc.SelectObject(pBrush2);
	tmpBrush2.DeleteObject();
	tmpBrush.DeleteObject();
}

//*********************************************
//					CStageMod
//*********************************************

CStageMod::CStageMod():m_StageSize(100,50),m_nDegree(0),m_GlassColor(RGB(0,255,0)),m_TableColor(RGB(0,0,0)),m_bGlass(0)
{

}

CStageMod::~CStageMod()
{
	m_BitStage.DeleteObject();
	m_BitStage_mask.DeleteObject();

}

void CStageMod::CellRectCf(CRect &CellRect, int nDir)
{
	switch(nDir)
	{
	case 0:
		CellRect.left	+= 10;
		CellRect.top	+= 10;
		break;

	case 1:
		CellRect.left	+= 10;
		CellRect.bottom -= 10;
		break;

	case 2:
		CellRect.right	-= 10;
		CellRect.top	+= 10;
		break;

	case 3:
		CellRect.right	-= 10;
		CellRect.bottom	-= 10;
		break;
	}

}

void CStageMod::RotateRectAngle(CRect rect, double nDegree)
{
	CPoint pointCenter;

	
	double cosine = cos(nDegree * 3.14159265359 / 180.);
	double sine	  = sin(nDegree * 3.14159265359 / 180.);
	// 중심축을 기준으로 회전
	pointCenter.x = rect.CenterPoint().x;
	pointCenter.y = rect.CenterPoint().y;

	int x0 = pointCenter.x;
	int y0 = pointCenter.y;

	double x1 = cosine	*	(rect.TopLeft().x - x0)		- sine		*	(rect.TopLeft().y - y0)		+ x0;
	double y1 = sine	*   (rect.TopLeft().x - x0)		+ cosine	*	(rect.TopLeft().y - y0)		+ y0;

	double x2 = cosine	*	(rect.BottomRight().x - x0) - sine		*   (rect.TopLeft().y - y0)		+ x0;
	double y2 = sine	*   (rect.BottomRight().x - x0) + cosine	*	(rect.TopLeft().y - y0)		+ y0;

	double x3 = cosine	*	(rect.BottomRight().x - x0) - sine		*   (rect.BottomRight().y - y0) + x0;
	double y3 = sine	*   (rect.BottomRight().x - x0) + cosine	*	(rect.BottomRight().y - y0) + y0;

	double x4 = cosine	*	(rect.TopLeft().x - x0)		- sine		*   (rect.BottomRight().y - y0) + x0;
    double y4 = sine	*	(rect.TopLeft().x - x0)		+ cosine	*   (rect.BottomRight().y - y0) + y0;	

	m_StagePont[0].x = int(x1); 
	m_StagePont[0].y = int(y1);
	m_StagePont[1].x = int(x2); 
	m_StagePont[1].y = int(y2); 
	m_StagePont[2].x = int(x3); 
	m_StagePont[2].y = int(y3); 
	m_StagePont[3].x = int(x4); 
	m_StagePont[3].y = int(y4);


}



void CStageMod:: DrawImg(CDC &dc)
{
//	CBrush tmpBrush,tmpCfBrush,*pOldtmpBrush;
    
   CAxisMod::ComverEqPosToImgPos();
    
	m_nDegree = (int)m_dEqCurPos_T;
	m_nHight = (int)m_dEqCurPos_Z;



//-----------------------
//|p1                   |p2
//|                     |
//|p3                   |
//-----------------------
/*
    Graphics graphics(dc.m_hDC);
*/	

   m_StageSizeMod.cx = m_StageSize.cx + m_nHight/10;
   m_StageSizeMod.cy = m_StageSize.cy + m_nHight/10;

	CRect rect;
	rect.top    = m_nImgCurPos_Y + m_ImgSize.cy/2  - m_StageSizeMod.cy/2;
	rect.left   = m_nImgCurPos_X + m_ImgSize.cx/2  - m_StageSizeMod.cx/2;
	rect.right  = m_nImgCurPos_X + m_ImgSize.cx/2  - m_StageSizeMod.cx/2 + m_StageSizeMod.cx;
	rect.bottom = m_nImgCurPos_Y + m_ImgSize.cy/2  - m_StageSizeMod.cy/2 + m_StageSizeMod.cy;
    

	RotateRectAngle(rect,m_nDegree);
/*	
	Point points[] = {Point(m_StagePont[0].x ,m_StagePont[0].y),
			Point(m_StagePont[1].x , m_StagePont[1].y),
			Point(m_StagePont[3].x, m_StagePont[3].y)};
		
		graphics.DrawImage(&image, points, 3);
*/
	if(m_bGlass)
	{
		m_Glass.m_nDegree				= m_nDegree; 
		m_Glass.m_sizGlassSizeMod.cx	= m_Glass.m_sizGlassSize.cx + m_nHight/10;
    	m_Glass.m_sizGlassSizeMod.cy	= m_Glass.m_sizGlassSize.cy + m_nHight/10;
		m_Glass.m_nGlassPos.x			= rect.left + rect.Width()/2;
		m_Glass.m_nGlassPos.y			= rect.top  + rect.Height()/2;
		m_Glass.DrawImg(dc);
	}
}



//*********************************************
//					CVariantMod
//*********************************************
CVariantMod::CVariantMod()
{
}

CVariantMod::~CVariantMod()
{
}

void CVariantMod::DrawImg(CDC &dc)
{
    CAxisMod::ComverEqPosToImgPos();
 
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx, m_ImgSize.cy, &m_DcAxisMask, 0, 0, SRCAND);
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx, m_ImgSize.cy, &m_DcAxis, 0, 0, SRCPAINT);
}

//*********************************************
//					CTrnaferMod
//*********************************************

CTrnaferMod::CTrnaferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTrnaferMod::~CTrnaferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTrnaferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}


void CTrnaferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTrnaferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 
// 	if(!m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 - m_dEqCurPos_Z );
//     	m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 - m_dEqCurPos_Z );
// 
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CCrrPkTransferMod
//*********************************************
CCrrPkTransferMod::CCrrPkTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CCrrPkTransferMod::~CCrrPkTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CCrrPkTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CCrrPkTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CCrrPkTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CCrrPkTransferMod
//*********************************************

CLoadCvTransferMod::CLoadCvTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CLoadCvTransferMod::~CLoadCvTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CLoadCvTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CLoadCvTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CLoadCvTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.88 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.70 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx-55,m_nImgCurPos_Y+m_ImgSize.cy-100);
		m_Glass.DrawImg(dc);
	}
}

//*********************************************
//					CAttPkRbtLTransferMod
//*********************************************

CAttPkRbtLTransferMod::CAttPkRbtLTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAttPkRbtLTransferMod::~CAttPkRbtLTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAttPkRbtLTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAttPkRbtLTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAttPkRbtLTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.7 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.15 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	

}

//*********************************************
//					CBufPlateLTransferModCAttPkRbtLTransferMod
//*********************************************

CBufPlateLTransferMod::CBufPlateLTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CBufPlateLTransferMod::~CBufPlateLTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CBufPlateLTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CBufPlateLTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CBufPlateLTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.35 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.05 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
}


//*********************************************
//					CAttPkRbtRTransferMod
//*********************************************

CAttPkRbtRTransferMod::CAttPkRbtRTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAttPkRbtRTransferMod::~CAttPkRbtRTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAttPkRbtRTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAttPkRbtRTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAttPkRbtRTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.7 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.15 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	
}

//*********************************************
//					CBufPlateRTransferMod
//*********************************************

CBufPlateRTransferMod::CBufPlateRTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CBufPlateRTransferMod::~CBufPlateRTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CBufPlateRTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CBufPlateRTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CBufPlateRTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.35 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.05 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
}

//*********************************************
//					CAttCvTransferMod
//*********************************************

CAttCvTransferMod::CAttCvTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAttCvTransferMod::~CAttCvTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAttCvTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAttCvTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAttCvTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.88 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx-55,m_nImgCurPos_Y+m_ImgSize.cy-60);
		m_Glass.DrawImg(dc);
	}

}

//*********************************************
//					CPCBPkTransferLMod
//*********************************************
CPCBPkTransferLMod::CPCBPkTransferLMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CPCBPkTransferLMod::~CPCBPkTransferLMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CPCBPkTransferLMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}


void CPCBPkTransferLMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CPCBPkTransferLMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.4 + m_dEqCurPos_Z );
    	m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.7 + m_dEqCurPos_Z );

		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CPCBPkTransferRMod
//*********************************************
CPCBPkTransferRMod::CPCBPkTransferRMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CPCBPkTransferRMod::~CPCBPkTransferRMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CPCBPkTransferRMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}


void CPCBPkTransferRMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CPCBPkTransferRMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.4 + m_dEqCurPos_Z );
    	m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.7 + m_dEqCurPos_Z );

		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CAttPkTransferMod
//*********************************************

CAttPkTransferMod::CAttPkTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAttPkTransferMod::~CAttPkTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAttPkTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}


void CAttPkTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAttPkTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 - m_dEqCurPos_Z );
    	m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 - m_dEqCurPos_Z );

		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CConveyorMod
//*********************************************
CConveyorMod::CConveyorMod():m_nRotateSeq(1)
{
}

CConveyorMod::~CConveyorMod()
{

}

void CConveyorMod::ComverEqPosToImgPos()
{

	m_nImgCurPos_X =    m_HomePoint.x + int(m_dEqCurPos_X * m_dRatio)  - int(m_ImgSize.cx/2) ;
	m_nImgCurPos_Y =    m_HomePoint.y + int(m_dEqCurPos_Y * m_dRatio)  - int(m_ImgSize.cy/2) ;
}

void CConveyorMod::DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();
   
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y ,m_ImgSize.cx,m_ImgSize.cy, &m_DcAxisMask, 0, 0, SRCAND);
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y ,m_ImgSize.cx,m_ImgSize.cy, &m_DcAxis, 0, 0, SRCPAINT);
}

CDoor::CDoor(){}
CDoor::~CDoor(){}

void CDoor::DrawImg(CDC &dc)
{
	CBrush DoorBrush,*pBrush = NULL;

	
	if(m_bDoorOpen)
	{
		if(bTogle) 
		{
//			pBrush = (CBrush*)dc.SelectObject(&GdbSetColor.brushRed);
		}
		else
		{
//			pBrush = (CBrush*)dc.SelectObject(&GdbSetColor.brushWhite);
		}
		
		bTogle = !bTogle;
		dc.Rectangle(m_DooRect);
		dc.SelectObject(pBrush);
			
	}

}

CEMO::CEMO():m_nRotateSeq(1)
{}
CEMO::~CEMO(){}

void CEMO::DrawImg(CDC &dc)
{ 
	
	ComverEqPosToImgPos();
	if(m_bSwOn)
		(m_nRotateSeq > 1) ? m_nRotateSeq =0 : m_nRotateSeq++;
	else
		m_nRotateSeq = 2;

	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx , m_ImgSize.cy, &m_DcAxisMask,m_ImgSize.cx * m_nRotateSeq, 0, SRCAND);
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx/3 , m_ImgSize.cy, &m_DcAxis,m_ImgSize.cx * m_nRotateSeq/3, 0, SRCPAINT);
	m_nRotateSeq++; 

	
}

//*********************************************
//					CAnimate
//*********************************************

CAnimate::CAnimate():m_CursorPoint(0,0)
{
	int nBtnWidth, nBtnGab;
	nBtnWidth = 120;
	nBtnGab = 30;
	for(int i=0; i<3; i++)
	{
		m_BtnRect[i] = CRect((nBtnGab+(nBtnWidth)*(i)),65,((nBtnWidth)*(i+1)),95);
		m_bClicked[i] = FALSE;
	}
}

CAnimate::~CAnimate()
{
	m_AniFont.DeleteObject();
	m_AniPen.DeleteObject();
	m_AniBrush.DeleteObject();
	m_AniPen1.DeleteObject();
	m_AniBrush1.DeleteObject();
	m_AniBicFont.DeleteObject();
	m_FrameBase.DeleteDC();
}

BEGIN_MESSAGE_MAP(CAnimate, CWnd)
	//{{AFX_MSG_MAP(CAnimate)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAnimate message handlers
BOOL CAnimate::Create ( DWORD exstyle, DWORD style, const CRect& r, CWnd *p, UINT id, CCreateContext *pc ) 
{					
	
	CBrush mybrush ( RGB (255, 255, 0 ) ) ;

	CString mywindowclass ;
	mywindowclass = AfxRegisterWndClass ( CS_HREDRAW | CS_VREDRAW, 
										AfxGetApp( ) -> LoadCursor ( IDI_CURSOR ), 
										mybrush, 0 ) ;

	return CreateEx ( exstyle, mywindowclass, "", style, 
						r.left, r.top, r.Width(),r.Height(), 
						p -> GetSafeHwnd( ), NULL, NULL ) ;
	mybrush.DeleteObject();

	return TRUE;
	
}

void CAnimate::OnDestroy() 
{
	CWnd::OnDestroy();

	m_AniFont.DeleteObject();
	m_AniPen.DeleteObject();
	m_AniBicFont.DeleteObject();
	m_AniPen1.DeleteObject();
	m_AniBrush1.DeleteObject();

}


void CAnimate::InitAniWnd()
{
	CDC* pAniDC = GetDC(); 

	m_AniFont.CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH|FF_SWISS, "굴림체");

	m_AniBicFont.CreateFont(17, 10, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH|FF_SWISS, "굴림체");

	m_AniSmallFont.CreateFont(9, 10, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH|FF_SWISS, "굴림체");

	m_AniPen.CreatePen(PS_DASH, 3, RGB(0,255,50));
	m_AniBrush.CreateSolidBrush(RGB(0,255,50));

	m_AniPen1.CreatePen(PS_DASH, 3, RGB(255,0,0));
	m_AniBrush1.CreateSolidBrush(RGB(255,0,0));

	m_FrameBase.CreateCompatibleDC(pAniDC);

	// ===== BackFrame ======
 	m_FrameBaseBmp.LoadBitmap(IDB_MAIN_ANIVIEW);
 	m_pFrameBaseBmp = (CBitmap *)m_FrameBase.SelectObject(&m_FrameBaseBmp);

 	// ===== Unit Position ======
    //////////////////////////////////////////////ready
// 	m_Transfer2.SetRatioEqToImg(1.15);
// 	m_Transfer2.SetImgHomePos(CPoint(82,166));
// 	m_Pitch2.SetRatioEqToImg(1.15);
// 	m_Pitch2.SetImgHomePos(CPoint(70,106));
// 	m_Arm.SetRatioEqToImg(0.95);
// 	m_Arm.SetImgHomePos(CPoint(107,250));
// 	m_Cell2_Up.SetRatioEqToImg(0.0);
// 	m_Cell2_Up.SetImgHomePos(CPoint(215,77));//align
// 	m_Cell2_Down.SetRatioEqToImg(0.0);
// 	m_Cell2_Down.SetImgHomePos(CPoint(215,127));//align
// 	
// 	m_AlignClamp1.SetRatioEqToImg(0.8);
// 	m_AlignClamp1.SetImgHomePos(CPoint(227,45));
// 	m_AlignClamp2.SetRatioEqToImg(0.8);
// 	m_AlignClamp2.SetImgHomePos(CPoint(228,153));
// 	m_AlignClamp3.SetRatioEqToImg(0.8);
// 	m_AlignClamp3.SetImgHomePos(CPoint(255,100));
// 	
// 	m_Transfer1.SetRatioEqToImg(0.95);
// 	m_Transfer1.SetImgHomePos(CPoint(461,166));
// 	m_Pitch1.SetRatioEqToImg(0.95);
// 	m_Pitch1.SetImgHomePos(CPoint(474,104));
// 	
// 	//	m_Pitch1.SetImgHomePos(CPoint(314,97));y
// 	m_Cell1_Up.SetRatioEqToImg(0.0);
// 	m_Cell1_Up.SetImgHomePos(CPoint(40,203));
// 	m_Cell1_Down.SetRatioEqToImg(0.0);
// 	m_Cell1_Down.SetImgHomePos(CPoint(40,253));
// 	
// 	m_TrayRobot.SetRatioEqToImg(0.5);
// 	m_TrayRobot.SetImgHomePos(CPoint(406,268));//328));
// 	m_Tray1.SetRatioEqToImg(0.0);
// 	m_Tray1.SetImgHomePos(CPoint(435,229));
// 	m_Tray2.SetRatioEqToImg(0.0);
// 	m_Tray2.SetImgHomePos(CPoint(435,108));

	m_Workbuffer1.SetRatioEqToImg(0.95);
	m_Workbuffer1.SetImgHomePos(CPoint(65,139));
	m_Workbuffer2.SetRatioEqToImg(0.95);
	m_Workbuffer2.SetImgHomePos(CPoint(65,103));
	m_Workbuffer3.SetRatioEqToImg(0.95);
	m_Workbuffer3.SetImgHomePos(CPoint(65,65));
	m_Workbuffer4.SetRatioEqToImg(0.95);
	m_Workbuffer4.SetImgHomePos(CPoint(65,27));

	m_ModuleRbt.SetRatioEqToImg(0.6);
	m_ModuleRbt.SetImgHomePos(CPoint(80,295));
	m_LeftHSRbt.SetRatioEqToImg(0.6);
	m_LeftHSRbt.SetImgHomePos(CPoint(218,304));
	m_RightHSRbt.SetRatioEqToImg(0.6);
	m_RightHSRbt.SetImgHomePos(CPoint(385,295));
	m_ClipRbt.SetRatioEqToImg(0.6);
	m_ClipRbt.SetImgHomePos(CPoint(555,179));
	m_ClampRbt.SetRatioEqToImg(0.6); 
	m_ClampRbt.SetImgHomePos(CPoint(555,138));
	m_SorterRbt.SetRatioEqToImg(0.6);
	m_SorterRbt.SetImgHomePos(CPoint(740,195));
	m_UnloaderRbt.SetRatioEqToImg(0.6);
	m_UnloaderRbt.SetImgHomePos(CPoint(884,265));
	
	m_MDStacker1.SetRatioEqToImg(0.5);
	m_MDStacker1.SetImgHomePos(CPoint(46,400));
	m_MDStacker2.SetRatioEqToImg(0.5);
	m_MDStacker2.SetImgHomePos(CPoint(134,400));
	m_LHSStacker1.SetRatioEqToImg(0.5);
	m_LHSStacker1.SetImgHomePos(CPoint(224,400));
	m_LHSStacker2.SetRatioEqToImg(0.5);
	m_LHSStacker2.SetImgHomePos(CPoint(324,400));
	m_RHSStacker1.SetRatioEqToImg(0.5);
	m_RHSStacker1.SetImgHomePos(CPoint(424,400));
	m_RHSStacker2.SetRatioEqToImg(0.5);
	m_RHSStacker2.SetImgHomePos(CPoint(524,400));
	m_ClipTube1.SetRatioEqToImg(0.5);
	m_ClipTube1.SetImgHomePos(CPoint(604,400));
	m_ClipTube2.SetRatioEqToImg(0.5);
	m_ClipTube2.SetImgHomePos(CPoint(644,400));
	m_ULDStacker1.SetRatioEqToImg(0.5);
	m_ULDStacker1.SetImgHomePos(CPoint(724,400));
	m_ULDStacker2.SetRatioEqToImg(0.5);
	m_ULDStacker2.SetImgHomePos(CPoint(824,400));
	m_Reject.SetRatioEqToImg(0.5);
	m_Reject.SetImgHomePos(CPoint(910,400));
	
	//////////////////////////////////////////////

 	// ===== Move Unit ======	
// 	m_Transfer1.CreateImg(pAniDC,IDB_STATIC_ANI_TRANSFER1,IDB_STATIC_ANI_TRANSFER1_BK,IDB_STATIC_ANI_TRANSFER1,IDB_STATIC_ANI_TRANSFER1_BK);
// 	m_Transfer2.CreateImg(pAniDC,IDB_STATIC_ANI_TRANSFER2,IDB_STATIC_ANI_TRANSFER2_BK,IDB_STATIC_ANI_TRANSFER2,IDB_STATIC_ANI_TRANSFER2_BK);
// 	m_Arm.CreateImg(pAniDC,IDB_STATIC_ANI_ARM,IDB_STATIC_ANI_ARM_BK,IDB_STATIC_ANI_ARM,IDB_STATIC_ANI_ARM_BK);
// 	m_Pitch1.CreateImg(pAniDC,IDB_STATIC_ANI_PITCH1,IDB_STATIC_ANI_PITCH1_BK,IDB_STATIC_ANI_PITCH1,IDB_STATIC_ANI_PITCH1_BK);
// 	m_Pitch2.CreateImg(pAniDC,IDB_STATIC_ANI_PITCH2,IDB_STATIC_ANI_PITCH2_BK,IDB_STATIC_ANI_PITCH2,IDB_STATIC_ANI_PITCH2_BK);
// 	m_Cell1_Up.CreateImg(pAniDC,IDB_STATIC_ANI_CELL1_UP,IDB_STATIC_ANI_CELL1_UP_BK,IDB_STATIC_ANI_CELL1_UP,IDB_STATIC_ANI_CELL1_UP_BK);
// 	m_Cell1_Down.CreateImg(pAniDC,IDB_STATIC_ANI_CELL1_DOWN,IDB_STATIC_ANI_CELL1_DOWN_BK,IDB_STATIC_ANI_CELL1_DOWN,IDB_STATIC_ANI_CELL1_DOWN_BK);
// 	m_Cell2_Up.CreateImg(pAniDC,IDB_STATIC_ANI_CELL2_UP,IDB_STATIC_ANI_CELL2_UP_BK,IDB_STATIC_ANI_CELL2_UP,IDB_STATIC_ANI_CELL2_UP_BK);
// 	m_Cell2_Down.CreateImg(pAniDC,IDB_STATIC_ANI_CELL2_DOWN,IDB_STATIC_ANI_CELL2_DOWN_BK,IDB_STATIC_ANI_CELL2_DOWN,IDB_STATIC_ANI_CELL2_DOWN_BK);
// 	m_TrayRobot.CreateImg(pAniDC,IDB_STATIC_ANI_TRAYROBOT,IDB_STATIC_ANI_TRAYROBOT_BK,IDB_STATIC_ANI_TRAYROBOT,IDB_STATIC_ANI_TRAYROBOT_BK);
// 	m_Tray1.CreateImg(pAniDC,IDB_STATIC_ANI_TRAY1,IDB_STATIC_ANI_TRAY1_BK,IDB_STATIC_ANI_TRAY1,IDB_STATIC_ANI_TRAY1_BK);
// 	m_Tray2.CreateImg(pAniDC,IDB_STATIC_ANI_TRAY2,IDB_STATIC_ANI_TRAY2_BK,IDB_STATIC_ANI_TRAY2,IDB_STATIC_ANI_TRAY2_BK);
// 
//  m_AlignClamp1.CreateImg(pAniDC,IDB_STATIC_ANI_ALIGNCLAMP1,IDB_STATIC_ANI_ALIGNCLAMP1_BK,IDB_STATIC_ANI_ALIGNCLAMP1,IDB_STATIC_ANI_ALIGNCLAMP1_BK);
//  m_AlignClamp2.CreateImg(pAniDC,IDB_STATIC_ANI_ALIGNCLAMP2,IDB_STATIC_ANI_ALIGNCLAMP2_BK,IDB_STATIC_ANI_ALIGNCLAMP2,IDB_STATIC_ANI_ALIGNCLAMP2_BK);
//  m_AlignClamp3.CreateImg(pAniDC,IDB_STATIC_ANI_ALIGNCLAMP3,IDB_STATIC_ANI_ALIGNCLAMP3_BK,IDB_STATIC_ANI_ALIGNCLAMP3,IDB_STATIC_ANI_ALIGNCLAMP3_BK);

	m_Workbuffer1.CreateImg(pAniDC,IDB_WORKBUFFER1,IDB_WORKBUFFER1_BK,IDB_WORKBUFFER1,IDB_WORKBUFFER1_BK);
	m_Workbuffer2.CreateImg(pAniDC,IDB_WORKBUFFER1,IDB_WORKBUFFER1_BK,IDB_WORKBUFFER1,IDB_WORKBUFFER1_BK);
	m_Workbuffer3.CreateImg(pAniDC,IDB_WORKBUFFER1,IDB_WORKBUFFER1_BK,IDB_WORKBUFFER1,IDB_WORKBUFFER1_BK);
	m_Workbuffer4.CreateImg(pAniDC,IDB_WORKBUFFER1,IDB_WORKBUFFER1_BK,IDB_WORKBUFFER1,IDB_WORKBUFFER1_BK);
	
	m_ModuleRbt.CreateImg(pAniDC,IDB_MODULERBT, IDB_MODULERBT_BK, IDB_MODULERBT, IDB_MODULERBT_BK);
	m_LeftHSRbt.CreateImg(pAniDC,IDB_MODULERBT, IDB_MODULERBT_BK, IDB_MODULERBT, IDB_MODULERBT_BK);
	m_RightHSRbt.CreateImg(pAniDC,IDB_MODULERBT, IDB_MODULERBT_BK, IDB_MODULERBT, IDB_MODULERBT_BK);
	m_ClipRbt.CreateImg(pAniDC,IDB_MODULERBT, IDB_MODULERBT_BK, IDB_MODULERBT, IDB_MODULERBT_BK);
	m_ClampRbt.CreateImg(pAniDC,IDB_CLIPCLAMP, IDB_CLIPCLAMP_BK, IDB_CLIPCLAMP, IDB_CLIPCLAMP_BK);
	m_SorterRbt.CreateImg(pAniDC,IDB_MODULERBT, IDB_MODULERBT_BK, IDB_MODULERBT, IDB_MODULERBT_BK);
	m_UnloaderRbt.CreateImg(pAniDC,IDB_MODULERBT, IDB_MODULERBT_BK, IDB_MODULERBT, IDB_MODULERBT_BK);

	m_MDStacker1.CreateImg(pAniDC,IDB_TRAY, IDB_TRAY_BK, IDB_TRAY, IDB_TRAY_BK);
	m_MDStacker2.CreateImg(pAniDC,IDB_TRAY_THIS, IDB_TRAY_BK, IDB_TRAY_THIS, IDB_TRAY_BK);
	m_LHSStacker1.CreateImg(pAniDC,IDB_TRAY, IDB_TRAY_BK, IDB_TRAY, IDB_TRAY_BK);
	m_LHSStacker2.CreateImg(pAniDC,IDB_TRAY_THIS, IDB_TRAY_BK, IDB_TRAY_THIS, IDB_TRAY_BK);
	m_RHSStacker1.CreateImg(pAniDC,IDB_TRAY, IDB_TRAY_BK, IDB_TRAY, IDB_TRAY_BK);
	m_RHSStacker2.CreateImg(pAniDC,IDB_TRAY_THIS, IDB_TRAY_BK, IDB_TRAY_THIS, IDB_TRAY_BK);
	m_ClipTube1.CreateImg(pAniDC,IDB_CLIPTUBE, IDB_CLIPTUBE_BK, IDB_CLIPTUBE, IDB_CLIPTUBE_BK);
	m_ClipTube2.CreateImg(pAniDC,IDB_CLIPTUBE, IDB_CLIPTUBE_BK, IDB_CLIPTUBE, IDB_CLIPTUBE_BK);
	m_ULDStacker1.CreateImg(pAniDC,IDB_TRAY, IDB_TRAY_BK, IDB_TRAY, IDB_TRAY_BK);
	m_ULDStacker2.CreateImg(pAniDC,IDB_TRAY_THIS, IDB_TRAY_BK, IDB_TRAY_THIS, IDB_TRAY_BK);
	m_Reject.CreateImg(pAniDC,IDB_TRAY, IDB_TRAY_BK, IDB_TRAY, IDB_TRAY_BK);

	ReleaseDC(pAniDC);	
}

void CAnimate::CylImageChange(CDC *pDC /*= NULL*/)
{ 
	////////////////////////////////////////////////////////////
	// 선택 되어 있는 image object를 dc에 넣고 삭제한다. 
	////////////////////////////////////////////////////////////
// 	m_ConveyorZ.m_DcAxis.SelectObject(&m_ConveyorZ.m_BitAxis);
// 	DeleteObject(m_ConveyorZ.m_BitAxis);
// 
// 	m_PickerVac1.m_DcAxis.SelectObject(&m_PickerVac1.m_BitAxis);
// 	DeleteObject(m_PickerVac1.m_BitAxis);
// 	
// 	m_PickerVac2.m_DcAxis.SelectObject(&m_PickerVac2.m_BitAxis);
// 	DeleteObject(m_PickerVac2.m_BitAxis);
// 	
// 	m_PickerVac3.m_DcAxis.SelectObject(&m_PickerVac3.m_BitAxis);
// 	DeleteObject(m_PickerVac3.m_BitAxis);
// 	
// 	m_PickerVac4.m_DcAxis.SelectObject(&m_PickerVac4.m_BitAxis);
// 	DeleteObject(m_PickerVac4.m_BitAxis);
// 	
// 	m_PickerCyl1.m_DcAxis.SelectObject(&m_PickerCyl1.m_BitAxis);
// 	DeleteObject(m_PickerCyl1.m_BitAxis);
// 	
// 	m_PickerCyl2.m_DcAxis.SelectObject(&m_PickerCyl2.m_BitAxis);
// 	DeleteObject(m_PickerCyl2.m_BitAxis);
// 	
// 	m_PickerCyl3.m_DcAxis.SelectObject(&m_PickerCyl3.m_BitAxis);
// 	DeleteObject(m_PickerCyl3.m_BitAxis);
// 	
// 	m_PickerCyl4.m_DcAxis.SelectObject(&m_PickerCyl4.m_BitAxis);
// 	DeleteObject(m_PickerCyl4.m_BitAxis);
// 	
// 	m_ConveyorPX.m_DcAxis.SelectObject(&m_ConveyorPX.m_BitAxis);
// 	DeleteObject(m_ConveyorPX.m_BitAxis);
// 	
// 	m_ConveyorPY.m_DcAxis.SelectObject(&m_ConveyorPY.m_BitAxis);
// 	DeleteObject(m_ConveyorPY.m_BitAxis);
// 
//  	if(ani_cyl_pos.m_nConveyorPX_Up_Dn == FALSE )
// 		m_ConveyorPX.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR_PX);
//  	else
// 		m_ConveyorPX.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR_PX_FWD);
//  	if(ani_cyl_pos.m_nConveyorRail_Fwd_Bwd == FALSE)
// 		m_ConveyorPY.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR);
//  	else
// 		m_ConveyorPY.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR_FWD);
//  	if(ani_cyl_pos.m_nConveyorZ_Up_Dn == FALSE)
// 		m_ConveyorZ.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR_Z_DN);
//  	else
// 		m_ConveyorZ.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR_Z_UP);
// 	if(ani_cyl_pos.m_nPickerVac_On_Off[0] == FALSE)
// 		m_PickerVac1.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_OFF);
// 	else
// 		m_PickerVac1.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_ON);
// 	if(ani_cyl_pos.m_nPickerVac_On_Off[1] == FALSE)
// 		m_PickerVac2.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_OFF);
// 	else
// 		m_PickerVac2.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_ON);
// 	if(ani_cyl_pos.m_nPickerVac_On_Off[2] == FALSE)
// 		m_PickerVac3.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_OFF);
// 	else
// 		m_PickerVac3.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_ON);
// 	if(ani_cyl_pos.m_nPickerVac_On_Off[3] == FALSE)
// 		m_PickerVac4.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_OFF);
// 	else
// 		m_PickerVac4.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_ON);
// 	if(ani_cyl_pos.m_nPickerCyl_On_Off[0] == TRUE)
// 		m_PickerCyl1.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_DN);
// 	else
// 		m_PickerCyl1.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_UP);
// 	if(ani_cyl_pos.m_nPickerCyl_On_Off[1] == TRUE)
// 		m_PickerCyl2.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_DN);
// 	else
// 		m_PickerCyl2.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_UP);
// 	if(ani_cyl_pos.m_nPickerCyl_On_Off[2] == TRUE)
// 		m_PickerCyl3.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_DN);
// 	else
// 		m_PickerCyl3.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_UP);
// 	if(ani_cyl_pos.m_nPickerCyl_On_Off[3] == TRUE)
// 		m_PickerCyl4.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_DN);
// 	else
// 		m_PickerCyl4.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_UP);
// 
// 	////////////////////////////////////////////////////////////
// 	// 선택 되어 있는 image object를 삭제한다. 
// 	////////////////////////////////////////////////////////////
// 	m_ConveyorZ.m_DcAxis.SelectObject(&m_ConveyorZ.m_BitAxis);
// 	DeleteObject(m_ConveyorZ.m_BitAxis);
// 	
// 	m_PickerVac1.m_DcAxis.SelectObject(&m_PickerVac1.m_BitAxis);
// 	DeleteObject(m_PickerVac1.m_BitAxis);
// 	
// 	m_PickerVac2.m_DcAxis.SelectObject(&m_PickerVac2.m_BitAxis);
// 	DeleteObject(m_PickerVac2.m_BitAxis);
// 	
// 	m_PickerVac3.m_DcAxis.SelectObject(&m_PickerVac3.m_BitAxis);
// 	DeleteObject(m_PickerVac3.m_BitAxis);
// 	
// 	m_PickerVac4.m_DcAxis.SelectObject(&m_PickerVac4.m_BitAxis);
// 	DeleteObject(m_PickerVac4.m_BitAxis);
// 	
// 	m_PickerCyl1.m_DcAxis.SelectObject(&m_PickerCyl1.m_BitAxis);
// 	DeleteObject(m_PickerCyl1.m_BitAxis);
// 	
// 	m_PickerCyl2.m_DcAxis.SelectObject(&m_PickerCyl2.m_BitAxis);
// 	DeleteObject(m_PickerCyl2.m_BitAxis);
// 	
// 	m_PickerCyl3.m_DcAxis.SelectObject(&m_PickerCyl3.m_BitAxis);
// 	DeleteObject(m_PickerCyl3.m_BitAxis);
// 	
// 	m_PickerCyl4.m_DcAxis.SelectObject(&m_PickerCyl4.m_BitAxis);
// 	DeleteObject(m_PickerCyl4.m_BitAxis);
// 	
// 	m_ConveyorPX.m_DcAxis.SelectObject(&m_ConveyorPX.m_BitAxis);
// 	DeleteObject(m_ConveyorPX.m_BitAxis);
// 	
// 	m_ConveyorPY.m_DcAxis.SelectObject(&m_ConveyorPY.m_BitAxis);
// 	DeleteObject(m_ConveyorPY.m_BitAxis);
// 
// 	ani_cyl_pos.m_nImageChange = FALSE;
}

void CAnimate::uMesJudgResultMonitoring(CDC *pDC, int *nMEStoEQ_TotalSotInfo, int *nPCBVisionGoodInfo, int *nPickerPickUpDnInfo, 
										int *nLeftGoodBufExistInfo, int *nRightGoodBufExistInfo)
{
 	CString sTmp,sMesDtSum,sVisionDtSum,sPickerDtSum,sLBufTmpDtSum,sRBufTmpDtSum;
 	
// 	sMesDtSum	 = "MES RESULT : ";
// 	sVisionDtSum = "VISION_RESULT : ";
// 	sPickerDtSum = "PICKER_RESULT : ";
// 
// 	sLBufTmpDtSum = "LEFT_BUF_TMP : ";
// 	sRBufTmpDtSum  = "RIGHT_BUF_TMP : ";
// 
//  	for(int i=0; i<st_basic.n_Tray_YCnt; i++)
// 	{
// 		if(i < _PICKER_4)
// 		{
// 			sTmp.Format("%d", nPickerPickUpDnInfo[i]);
// 			sPickerDtSum += sTmp;
// 		}
//  		sTmp.Format("%d", nMEStoEQ_TotalSotInfo[i]);
// 		sMesDtSum += sTmp;
// 		sTmp.Format("%d", nPCBVisionGoodInfo[i]);
// 		sVisionDtSum += sTmp;
// 
// 		sTmp.Format("%d", nLeftGoodBufExistInfo[i]);
// 		sLBufTmpDtSum += sTmp;
// 
// 		sTmp.Format("%d", nRightGoodBufExistInfo[i]);
// 		sRBufTmpDtSum += sTmp;
//  	}
// 
// 	pDC->TextOut(352+(i*10), 10, sPickerDtSum);
// 	pDC->TextOut(352+(i*10), 25, sVisionDtSum);
// 	pDC->TextOut(373+(i*10), 40, sMesDtSum);
// 	pDC->TextOut(359+(i*10), 55, sLBufTmpDtSum);
// 	pDC->TextOut(352+(i*10), 70, sRBufTmpDtSum);
// 
// 	sTmp.Format("SAV:%d/ACC:%d", st_basic.n_Discharge_Lifter_Limit, st_work.m_nUnloadTrayAccCnt);
// 	pDC->TextOut(40, 123, sTmp);
// 	sTmp.Format("SAV:%d/ACC:%d", st_basic.n_tray_Lifter, st_work.m_nCoverTrayAccCnt);
// 	pDC->TextOut(170, 123, sTmp);
// 
// 	if(nColChange == FALSE)
// 	{
// 		nColChange = TRUE;
// 		nColChangeWait[0] = GetCurrentTime();
// 	}
// 	else
// 	{
// 		nColChangeWait[1] = GetCurrentTime();
// 		nColChangeWait[2] = (nColChangeWait[1] - nColChangeWait[0]);
// 		if(nColChangeWait[2] > 400)
// 		{
// 			nColChange = FALSE;
// 			nRandCol = getrandom(0,255);
// 			pDC->SetTextColor(RGB(nRandCol,nRandCol,0));
// 			
// 		}
// 
// 	}
// 	pDC->SetTextColor(RGB(nRandCol,0,0));
// 	sTmp.Format("%s", st_work.strCurActionMsg);
// 	pDC->TextOut(310, 127, sTmp);
}

void CAnimate::uGoodBufLPcbMonitoring(CDC *pDC, int *nLeftGoodBufExistInfo)
{
// 	int nRtn,nTmp;
// 	int nRejectLineStPos = 575;	

// 	nTmp = st_basic.n_Tray_YCnt / 2;
// 
// 	for(int i=0; i<nTmp; i++)
// 	{
// 		nRtn = nLeftGoodBufExistInfo[i];
// 		if(nRtn == TRUE)
// 		{	
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 270;
// 			
// 			pDC->MoveTo(pRejectPt);
// 			
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 170;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 		else
// 		{
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 			
// 			pDC->MoveTo(pRejectPt);
// 			
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 	}
}

void CAnimate::uGoodBufRPcbMonitoring(CDC *pDC, int *nRightGoodBufExistInfo)
{
// 	int nRtn, nTmp;
// 	int nRejectLineStPos = 575 /*622*/;

// 	nTmp = st_basic.n_Tray_YCnt/* / 2*/;
// 
// 	for(int i=0; i<nTmp; i++)
// 	{
// 		nRtn = nRightGoodBufExistInfo[/*nTmp + */i];
// 		if(nRtn == TRUE)
// 		{	
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 270;
// 			
// 			pDC->MoveTo(pRejectPt);
// 			
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 170;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 		else
// 		{
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 			
// 			pDC->MoveTo(pRejectPt);
// 			
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 	}
}

void CAnimate::uRejectBufPcbMonitoring(CDC *pDC, int *nRejectBufPcbExistInfo)
{
//	int nRtn,nRejectLineStPos = 446;
// 	for(int i=0; i<st_basic.n_Tray_YCnt; i++)
// 	{
// 		nRtn = nRejectBufPcbExistInfo[i];
// 		if(nRtn == TRUE)
// 		{	
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 270;
// 
// 			pDC->MoveTo(pRejectPt);
// 
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 170;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 		else
// 		{
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 
// 			pDC->MoveTo(pRejectPt);
// 
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 	}
}

void CAnimate::ActionDisplay(CDC *pDC /*= NULL*/)
{
	//if(!IsWindowVisible()) return;
	//===default Code =====
	CDC		memDC;
	CFont	*pOldFont = NULL;
	CPen    *pOldPen = NULL;
	CBrush	*pOldBrush = NULL;
	CString strTmp;		 
  	if(pDC== NULL)
		pDC  = GetDC();

	CRect rect;

	CRect rBtn[3] = {(m_BtnRect[0]),(m_BtnRect[1]),(m_BtnRect[2])};
 	rBtn[0] += CRect(6,6,6,6);
	rBtn[1] += CRect(6,6,6,6);

	CRect Rectangle = m_BtnRect[0];

 	GetClientRect(&rect);
	CBitmap memBitmap, *OldmemBitmap;
	memDC.CreateCompatibleDC(pDC);

	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	OldmemBitmap = (CBitmap *)memDC.SelectObject(&memBitmap);

	pOldFont  = memDC.SelectObject(&m_AniFont);
	pOldPen  = memDC.SelectObject(&m_AniPen);
	pOldBrush  = memDC.SelectObject(&m_AniBrush);

	memDC.SetBkMode(TRANSPARENT);	 
	memDC.SetTextColor(RGB(0,0,255));

	memDC.BitBlt(0,0, rect.Width(), rect.Height(), &m_FrameBase, 0, 0, SRCCOPY);				

	//////////////////////////////////////////////////////
	// 이송 축
	//////////////////////////////////////////////////////
// 	m_AlignClamp1.DrawImg(memDC);
// 	m_AlignClamp2.DrawImg(memDC);
// 	m_AlignClamp3.DrawImg(memDC);
// 	m_Arm.DrawImg(memDC);
// 	m_Pitch2.DrawImg(memDC);
// 	m_Transfer2.DrawImg(memDC);
// 	m_Tray1.DrawImg(memDC);
// 	m_Tray2.DrawImg(memDC);
// 	m_TrayRobot.DrawImg(memDC);
// 	m_Pitch1.DrawImg(memDC);
// 	m_Transfer1.DrawImg(memDC);

	m_Workbuffer1.DrawImg(memDC);
	m_Workbuffer2.DrawImg(memDC);
	m_Workbuffer3.DrawImg(memDC);
	m_Workbuffer4.DrawImg(memDC);

	m_ModuleRbt.DrawImg(memDC);
	m_LeftHSRbt.DrawImg(memDC);
	m_RightHSRbt.DrawImg(memDC);
	m_ClipRbt.DrawImg(memDC);
	m_ClampRbt.DrawImg(memDC);
	m_SorterRbt.DrawImg(memDC);
	m_UnloaderRbt.DrawImg(memDC);

	m_MDStacker1.DrawImg(memDC);
	m_MDStacker2.DrawImg(memDC);
	m_LHSStacker1.DrawImg(memDC); 
	m_LHSStacker2.DrawImg(memDC);
	m_RHSStacker1.DrawImg(memDC);
	m_RHSStacker2.DrawImg(memDC);
	m_ClipTube1.DrawImg(memDC);
	m_ClipTube2.DrawImg(memDC);
	m_ULDStacker1.DrawImg(memDC);
	m_ULDStacker2.DrawImg(memDC);
	m_Reject.DrawImg(memDC);
	

	ModuleTrayCheck(&memDC);
	LHSTrayCheck(&memDC);
	RHSTrayCheck(&memDC);
	ClipTube1Check(&memDC);
	ClipTube2Check(&memDC);
	ULDTrayCheck(&memDC);
	RejectTrayCheck(&memDC);

	pOldPen  = memDC.SelectObject(&m_AniPen1);
	pOldBrush  = memDC.SelectObject(&m_AniBrush1);

	MDTray1Check(&memDC);
	LHSTray1Check(&memDC);
	RHSTray1Check(&memDC);
	ULDTray1Check(&memDC);

	// 	//===default Code =====
 	pDC->BitBlt(0,0,  rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(OldmemBitmap);
	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldPen);
	memDC.SelectObject(pOldBrush);
	DeleteObject(OldmemBitmap);
	ReleaseDC(pDC);
	memDC.DeleteDC();
	
	//=====================
}

void CAnimate::AniPioStatus()
{
	//
}


void CAnimate::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_BtnRect[0].PtInRect(point))
	{
		m_bClicked[0] = TRUE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[0]);
	}

	if(m_BtnRect[1].PtInRect(point))
	{
		m_bClicked[1] = TRUE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[1]);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CAnimate::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_BtnRect[0].PtInRect(point))
	{
		m_bClicked[0] = FALSE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[0]);
	}

	if(m_BtnRect[1].PtInRect(point))
	{
		m_bClicked[1] = FALSE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[1]);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CAnimate::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_CursorPoint = point;

	if(m_bClicked[0] == TRUE)
	{
		if(!m_BtnRect[0].PtInRect(point))
		{
			m_bClicked[0] = FALSE;
			RedrawWindow(&m_BtnRect[0]);
		}
	}

	if(m_bClicked[1] == TRUE)
	{
		if(!m_BtnRect[1].PtInRect(point))
		{
			m_bClicked[1] = FALSE;
			RedrawWindow(&m_BtnRect[1]);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

//*********************************************
//					CTransferMod_1
//*********************************************

CTransferMod_1::CTransferMod_1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTransferMod_1::~CTransferMod_1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTransferMod_1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CTransferMod_1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTransferMod_1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CTransferMod_2
//*********************************************

CTransferMod_2::CTransferMod_2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTransferMod_2::~CTransferMod_2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTransferMod_2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CTransferMod_2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTransferMod_2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CArm
//*********************************************

CArm::CArm():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CArm::~CArm()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CArm::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;

	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CArm::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CArm:: DrawImg(CDC &dc)
{
	int i;

	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0)
			m_bGlass[i] = 0;	
	}
		
	if(m_bGlass[0])
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/4 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/8 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/7.8,m_nImgCurPos_Y+m_ImgSize.cy/4.25);
		m_Glass.DrawImg(dc);
	}	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	if(m_bGlass[1])
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx/4 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy/8 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/7.8,m_nImgCurPos_Y+m_ImgSize2.cy/1.32);
		m_Glass2.DrawImg2(dc);
	}	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
		
}

//*********************************************
//					CPitch1
//*********************************************

CPitch1::CPitch1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CPitch1::~CPitch1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CPitch1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;

	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CPitch1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CPitch1:: DrawImg(CDC &dc)
{
	int i;
	
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0)
			m_bGlass[i] = 0;	
	}
	
	if(m_bGlass[0])
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/5 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/7 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/5.3);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	if(m_bGlass[1])
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx/5 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy/7 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/2,m_nImgCurPos_Y+m_ImgSize2.cy/1.42);
		m_Glass2.DrawImg2(dc);
	}	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);		

}

//*********************************************
//					CPitch2
//*********************************************

CPitch2::CPitch2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CPitch2::~CPitch2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CPitch2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;

	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CPitch2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CPitch2:: DrawImg(CDC &dc)
{
	int i;
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0)
			m_bGlass[i] = 0;	
	}
	
	if(m_bGlass[0])
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/5 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/7 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/5.3);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	if(m_bGlass[1])
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx/5 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy/7 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/2,m_nImgCurPos_Y+m_ImgSize2.cy/1.42);
		m_Glass2.DrawImg2(dc);
	}	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);		
}

//*********************************************
//					CCell1_Up
//*********************************************

CCell1_Up::CCell1_Up():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CCell1_Up::~CCell1_Up()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CCell1_Up::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CCell1_Up::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CCell1_Up:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/3 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/7 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CCell2_Up
//*********************************************

CCell2_Up::CCell2_Up():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CCell2_Up::~CCell2_Up()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CCell2_Up::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CCell2_Up::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CCell2_Up:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/4 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/6 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CCell1_Down
//*********************************************

CCell1_Down::CCell1_Down():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CCell1_Down::~CCell1_Down()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CCell1_Down::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CCell1_Down::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CCell1_Down:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/3 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/7 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CCell2_Down
//*********************************************

CCell2_Down::CCell2_Down():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CCell2_Down::~CCell2_Down()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CCell2_Down::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CCell2_Down::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CCell2_Down:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/4 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/6 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CTrayRobot
//*********************************************

CTrayRobot::CTrayRobot():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTrayRobot::~CTrayRobot()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTrayRobot::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CTrayRobot::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTrayRobot:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 1.44 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.65 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/1.41,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CTray1
//*********************************************

CTray1::CTray1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTray1::~CTray1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTray1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CTray1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTray1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;	

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 1.44 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.65 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CTray2
//*********************************************

CTray2::CTray2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTray2::~CTray2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTray2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;

}

void CTray2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTray2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;	

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 1.44 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.65 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CAlignClamp1
//*********************************************

CAlignClamp1::CAlignClamp1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAlignClamp1::~CAlignClamp1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAlignClamp1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAlignClamp1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAlignClamp1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CAlignClamp2
//*********************************************

CAlignClamp2::CAlignClamp2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAlignClamp2::~CAlignClamp2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAlignClamp2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAlignClamp2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAlignClamp2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CAlignClamp3
//*********************************************

CAlignClamp3::CAlignClamp3():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAlignClamp3::~CAlignClamp3()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAlignClamp3::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAlignClamp3::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAlignClamp3:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CWorkbuffer_1
//*********************************************

CWorkbuffer1::CWorkbuffer1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CWorkbuffer1::~CWorkbuffer1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CWorkbuffer1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CWorkbuffer1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CWorkbuffer1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 1.2 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.25 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
// 	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CWorkbuffer_2
//*********************************************

CWorkbuffer2::CWorkbuffer2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CWorkbuffer2::~CWorkbuffer2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CWorkbuffer2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CWorkbuffer2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CWorkbuffer2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CWorkbuffer_3
//*********************************************

CWorkbuffer3::CWorkbuffer3():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CWorkbuffer3::~CWorkbuffer3()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CWorkbuffer3::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CWorkbuffer3::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CWorkbuffer3:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CWorkbuffer_4
//*********************************************

CWorkbuffer4::CWorkbuffer4():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CWorkbuffer4::~CWorkbuffer4()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CWorkbuffer4::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CWorkbuffer4::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CWorkbuffer4:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CModuleRbt
//*********************************************

CModuleRbt::CModuleRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CModuleRbt::~CModuleRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CModuleRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CModuleRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CModuleRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CLeftHSRbt
//*********************************************

CLeftHSRbt::CLeftHSRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CLeftHSRbt::~CLeftHSRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CLeftHSRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CLeftHSRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CLeftHSRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CRightHSRbt
//*********************************************

CRightHSRbt::CRightHSRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CRightHSRbt::~CRightHSRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CRightHSRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CRightHSRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CRightHSRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CClipRbt
//*********************************************

CClipRbt::CClipRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CClipRbt::~CClipRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CClipRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CClipRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CClipRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CClampRbt
//*********************************************

CClampRbt::CClampRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CClampRbt::~CClampRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CClampRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CClampRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CClampRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CSorterRbt
//*********************************************

CSorterRbt::CSorterRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CSorterRbt::~CSorterRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CSorterRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CSorterRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CSorterRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CUnloaderRbt
//*********************************************

CUnloaderRbt::CUnloaderRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CUnloaderRbt::~CUnloaderRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CUnloaderRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CUnloaderRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CUnloaderRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CMDStacker1
//*********************************************

CMDStacker1::CMDStacker1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CMDStacker1::~CMDStacker1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CMDStacker1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CMDStacker1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CMDStacker1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CMDStacker2
//*********************************************

CMDStacker2::CMDStacker2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CMDStacker2::~CMDStacker2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CMDStacker2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CMDStacker2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CMDStacker2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CLHSStacker1
//*********************************************

CLHSStacker1::CLHSStacker1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CLHSStacker1::~CLHSStacker1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CLHSStacker1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CLHSStacker1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CLHSStacker1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CLHSStacker2
//*********************************************

CLHSStacker2::CLHSStacker2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CLHSStacker2::~CLHSStacker2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CLHSStacker2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CLHSStacker2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CLHSStacker2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CRHSStacker1
//*********************************************

CRHSStacker1::CRHSStacker1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CRHSStacker1::~CRHSStacker1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CRHSStacker1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CRHSStacker1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CRHSStacker1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CRHSStacker2
//*********************************************

CRHSStacker2::CRHSStacker2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CRHSStacker2::~CRHSStacker2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CRHSStacker2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CRHSStacker2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CRHSStacker2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CClipTube1
//*********************************************

CClipTube1::CClipTube1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CClipTube1::~CClipTube1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CClipTube1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CClipTube1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CClipTube1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CClipTube2
//*********************************************

CClipTube2::CClipTube2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CClipTube2::~CClipTube2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CClipTube2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CClipTube2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CClipTube2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CULDStacker1
//*********************************************

CULDStacker1::CULDStacker1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CULDStacker1::~CULDStacker1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CULDStacker1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CULDStacker1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CULDStacker1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CULDStacker2
//*********************************************

CULDStacker2::CULDStacker2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CULDStacker2::~CULDStacker2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CULDStacker2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CULDStacker2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CULDStacker2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CRecject
//*********************************************

CRecject::CRecject():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CRecject::~CRecject()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CRecject::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CRecject::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CRecject:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

void CAnimate::ModuleTrayCheck(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 20;//1049;
	int nRejectLineStYPos = 350;//205;

	pDC->SetTextColor(RGB(0,0,255));
	
	for(int i=0; i < st_basic.mn_lotset_module_load; i++)
	{
		if(st_modulemap.LoadTray[1][i] == DVC_YES)
		{
			rect.left = nRejectLineStXPos;
			rect.right = nRejectLineStXPos+48;
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
		else
		{
			rect.left = 0;
			rect.right = 0;  
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
	}
}


void CAnimate::LHSTrayCheck(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 198;//1049;
	int nRejectLineStYPos = 340;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
	for(int i=0; i < st_basic.mn_lotset_hs_left; i++)
	{
		if(st_modulemap.LefthsTray[1][i] == DVC_YES)
		{
			rect.left = nRejectLineStXPos;
			rect.right = nRejectLineStXPos+48;
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
		else
		{
			rect.left = 0;
			rect.right = 0;  
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
	}
}


void CAnimate::RHSTrayCheck(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 398;//1049;
	int nRejectLineStYPos = 340;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
	for(int i=0; i < st_basic.mn_lotset_hs_right; i++)
	{
		if(st_modulemap.RighthsTray[1][i] == DVC_YES)
		{
			rect.left = nRejectLineStXPos;
			rect.right = nRejectLineStXPos+48;
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
		else
		{
			rect.left = 0;
			rect.right = 0;  
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
	}
}


void CAnimate::ClipTube1Check(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 595;//1049;
	int nRejectLineStYPos = 337;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
	for(int i=0; i < st_basic.mn_lotset_clip; i++)
	{
		if(st_modulemap.ClipTray[1][i] == DVC_YES)
		{
			rect.left = nRejectLineStXPos;
			rect.right = nRejectLineStXPos+15;
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
		else
		{
			rect.left = 0;
			rect.right = 0;  
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
	}
}


void CAnimate::ClipTube2Check(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 635;//1049;
	int nRejectLineStYPos = 337;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
	for(int i=0; i < st_basic.mn_lotset_clip; i++)
	{
		if(st_modulemap.ClipTray[1][i] == DVC_YES)
		{
			rect.left = nRejectLineStXPos;
			rect.right = nRejectLineStXPos+15;
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
		else
		{
			rect.left = 0;
			rect.right = 0;  
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
	}
}


void CAnimate::ULDTrayCheck(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 699;//1049;
	int nRejectLineStYPos = 350;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
	for(int i=0; i < st_basic.mn_lotset_unloader; i++)
	{
		if(st_modulemap.UnloadTray[1][i] == DVC_YES)
		{
			rect.left = nRejectLineStXPos;
			rect.right = nRejectLineStXPos+48;
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
		else
		{
			rect.left = 0;
			rect.right = 0;  
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
	}
}


void CAnimate::RejectTrayCheck(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 884;//1049;
	int nRejectLineStYPos = 350;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
	for(int i=0; i < st_basic.mn_lotset_reject; i++)
	{
		if(st_modulemap.RejectTray[i] == DVC_YES)
		{
			rect.left = nRejectLineStXPos;
			rect.right = nRejectLineStXPos+48;
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
		else
		{
			rect.left = 0;
			rect.right = 0;  
			rect.top = nRejectLineStYPos + (4 * i);
			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
			pDC->Rectangle(rect);
		}
	}
}

void CAnimate::MDTray1Check(CDC *pDC)
{
	CRect rect;
	int nRejectLineStXPos;//1049;
	int nRejectLineStYPos;//205;

	int n_io_status = {0,};

	nRejectLineStXPos = 108;
	nRejectLineStYPos = 345;

	n_io_status = FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk,IO_ON);

	if(n_io_status == IO_OFF)
	{
		rect.left = nRejectLineStXPos-2;
		rect.right = nRejectLineStXPos+50;
		rect.top = nRejectLineStYPos;
		rect.bottom = nRejectLineStYPos + (4) + 108;
		pDC->Rectangle(rect);
	}
	else
	{	
		rect.left = 0;
		rect.right = 0;  
		rect.top = nRejectLineStYPos;
		rect.bottom = nRejectLineStYPos + (4) + 108;
		pDC->Rectangle(rect);
	}
}

void CAnimate::LHSTray1Check(CDC *pDC)
{
	CRect rect;
	int nRejectLineStXPos;//1049;
	int nRejectLineStYPos;//205;
	
	int n_io_status = {0,};
	
	nRejectLineStXPos = 298;
	nRejectLineStYPos = 345;

	n_io_status = FAS_IO.get_in_bit(st_io.i_hs_fwd_ld_slide_tray_chk,IO_ON);

	if(n_io_status == IO_OFF)
	{
		rect.left = nRejectLineStXPos-2;
		rect.right = nRejectLineStXPos+50;
		rect.top = nRejectLineStYPos;
		rect.bottom = nRejectLineStYPos + (4) + 108;
		pDC->Rectangle(rect);
	}
	else
	{	
		rect.left = 0;
		rect.right = 0;  
		rect.top = nRejectLineStYPos;
		rect.bottom = nRejectLineStYPos + (4) + 108;
		pDC->Rectangle(rect);
	}
}

void CAnimate::RHSTray1Check(CDC *pDC)
{
	CRect rect;
	int nRejectLineStXPos;//1049;
	int nRejectLineStYPos;//205;
	
	int n_io_status = {0,};
	
	nRejectLineStXPos = 498;
	nRejectLineStYPos = 345;

	n_io_status = FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_tray_chk,IO_ON);

	if(n_io_status == IO_OFF)
	{
		rect.left = nRejectLineStXPos-2;
		rect.right = nRejectLineStXPos+50;
		rect.top = nRejectLineStYPos;
		rect.bottom = nRejectLineStYPos + (4) + 108;
		pDC->Rectangle(rect);
	}
	else
	{	
		rect.left = 0;
		rect.right = 0;  
		rect.top = nRejectLineStYPos;
		rect.bottom = nRejectLineStYPos + (4) + 108;
		pDC->Rectangle(rect);
	}
}

void CAnimate::ULDTray1Check(CDC *pDC)
{
	CRect rect;
	int nRejectLineStXPos;//1049;
	int nRejectLineStYPos;//205;
	
	int n_io_status = {0,};
	nRejectLineStXPos = 798;
	nRejectLineStYPos = 345;
	
	n_io_status = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_tray_chk,IO_ON);
	
	if(n_io_status == IO_OFF)
	{
		rect.left = nRejectLineStXPos-2;
		rect.right = nRejectLineStXPos+50;
		rect.top = nRejectLineStYPos;
		rect.bottom = nRejectLineStYPos + (4) + 108;
		pDC->Rectangle(rect);
	}
	else
	{	
		rect.left = 0;
		rect.right = 0;  
		rect.top = nRejectLineStYPos;
		rect.bottom = nRejectLineStYPos + (4) + 108;
		pDC->Rectangle(rect);
	}
}