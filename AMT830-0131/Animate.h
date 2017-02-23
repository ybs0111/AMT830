#if !defined(AFX_Animate_H__31696054_73E4_4AC0_9F8C_C6739EEC9C94__INCLUDED_)
#define AFX_Animate_H__31696054_73E4_4AC0_9F8C_C6739EEC9C94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Animate.h : header file
//
//#include "DCBmp.h"
/////////////////////////////////////////////////////////////////////////////
// CAnimate window

// ======CAxisMod============
class CAxisMod:public CWnd
{
public:
	CAxisMod();
	virtual ~CAxisMod();

public:
	double  m_dEqCurPos_X;
	double  m_dEqCurPos_Y;
	double  m_dEqWidth;
	int		m_nImgCurPos_X;
	int		m_nImgCurPos_Y;
	CPoint  m_HomePoint;
	double  m_dRatio;
	double	m_dRatio_Y;
	CSize   m_ImgSize;
	CSize   m_ImgSize2;
	double  m_dEqCurPos_X1;
	double  m_dEqCurPos_X2;


	CDC      m_DcAxis; 
  	CBitmap  m_BitAxis;
	CBitmap* m_pBitAxis;	
	CDC      m_DcAxisMask; 
  	CBitmap  m_BitAxisMask;
	CBitmap* m_pBitAxisMask;
	


public:

	void SetImgHomePos(const CPoint point)		{m_HomePoint    = point;}
	void SetImgPosX(const int x)				{m_nImgCurPos_X = x;}
	void SetImgPosY(const int y)				{m_nImgCurPos_Y = y;}
	void SetRatioEqToImg(const double dValue)	{m_dRatio		= dValue;}								
//	HBITMAP GetRotatedBitmap(HBITMAP hBitmap, float radians, COLORREF clrBack);

public: 
	virtual void ComverEqPosToImgPos();							// 실제 좌표  - >  이미지 좌표  
	virtual void DrawImg(CDC &dc);	
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk);// 이미지  원 이미지와 MASK 이미지로 비트맵 생성
	
};

// ======CGlass============
class CEqGlass:CWnd
{
public:
	CEqGlass();	
	virtual ~CEqGlass();
	
	CSize	 m_sizGlassSize,m_sizGlassSizeMod;
	CPoint   m_nGlassPos;
	int		 m_nDegree;
	

public: 
	void SetCellDir(int nDir) {m_nDir = nDir;}
	void CellRectCf(CRect &CellRect);	
	void RotateRectAngle(CRect rect,double nDegree, CRect rectCen);
	void DrawImg(CDC &dc);

private:
	int	   m_nDir;
	CPoint m_arrGlassPoint[4];

};

// ======CGlass2============
class CEqGlass2:CWnd
{
public:
	CEqGlass2();	
	virtual ~CEqGlass2();
	
	CSize	 m_sizGlassSize2,m_sizGlassSizeMod2;
	CPoint   m_nGlassPos2;
	int		 m_nDegree2;
	

public: 
	void SetCellDir2(int nDir) {m_nDir2 = nDir;}
	void CellRectCf2(CRect &CellRect2);	
	void RotateRectAngle2(CRect rect2,double nDegree2, CRect rectCen2);
	void DrawImg2(CDC &dc);

private:
	int	   m_nDir2;
	CPoint m_arrGlassPoint2[4];

};

// ======CStageMod============

class CStageMod:public CAxisMod
{
public:

	CStageMod();	
	virtual ~CStageMod(); 
	int			m_nDegree;
	int         m_nHight;
	BOOL		m_bGlass;
	BOOL        m_bSelected;
	CEqGlass	m_Glass;
	CEqGlass2	m_Glass2;
    

	double		m_dEqCurPos_T,m_dEqCurPos_Z;
private:
	CDC      m_DcStage; 
	CBitmap  m_BitStage;
	CBitmap* m_pBitStage;
    
	CDC      m_DcStage_mask; 
	CBitmap  m_BitStage_mask;
	CBitmap* m_pBitStage_mask;
	
protected:
	CPoint	 m_StagePont[4];
	CSize    m_StageSize,m_StageSizeMod;
	COLORREF m_GlassColor;
	COLORREF m_TableColor;

public:

	void CellRectCf(CRect& CellRect,int nDir);
	void RotateRectAngle(CRect rect, double nDegree);
	virtual void DrawImg(CDC &dc);
	void SetStageSize(const CSize size) {m_StageSize = size;}

};


// ======CVariantMod============
class CVariantMod:public CAxisMod
{
public:
	CVariantMod();	
	virtual ~CVariantMod();
	


public: 

	void DrawImg(CDC &dc);

private:
	int	   m_nDir;
	CPoint m_arrGlassPoint[4];

};

// ======CTrnaferMod============
class CTrnaferMod: public CAxisMod
{
	
public:
	CTrnaferMod();
	virtual ~CTrnaferMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;

public:
	CEqGlass m_Glass;

private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CCrrPkTransferMod============
class CCrrPkTransferMod: public CAxisMod
{
	
public:
	CCrrPkTransferMod();
	virtual ~CCrrPkTransferMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CLoadCvTransferMod============
class CLoadCvTransferMod: public CAxisMod
{
	
public:
	CLoadCvTransferMod();
	virtual ~CLoadCvTransferMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CAttCvTransferMod============
class CAttCvTransferMod: public CAxisMod
{
	
public:
	CAttCvTransferMod();
	virtual ~CAttCvTransferMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CAttPkRbtLTransferMod============
class CAttPkRbtLTransferMod: public CAxisMod
{
	
public:
	CAttPkRbtLTransferMod();
	virtual ~CAttPkRbtLTransferMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CBufPlateLTransferMod============
class CBufPlateLTransferMod: public CAxisMod
{
	
public:
	CBufPlateLTransferMod();
	virtual ~CBufPlateLTransferMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};


// ======CAttPkRbtRTransferMod============
class CAttPkRbtRTransferMod: public CAxisMod
{
	
public:
	CAttPkRbtRTransferMod();
	virtual ~CAttPkRbtRTransferMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CBufPlateRTransferMod============
class CBufPlateRTransferMod: public CAxisMod
{
	
public:
	CBufPlateRTransferMod();
	virtual ~CBufPlateRTransferMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CPCBPkTransferLMod============
class CPCBPkTransferLMod: public CAxisMod
{
	
public:
	CPCBPkTransferLMod();
	virtual ~CPCBPkTransferLMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CPCBPkTransferRMod============
class CPCBPkTransferRMod: public CAxisMod
{
	
public:
	CPCBPkTransferRMod();
	virtual ~CPCBPkTransferRMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CAttPkTransferMod============
class CAttPkTransferMod: public CAxisMod
{
	
public:
	CAttPkTransferMod();
	virtual ~CAttPkTransferMod();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

class CConveyorMod:public CAxisMod
{
public:
	CConveyorMod();	
	virtual ~CConveyorMod(); 

	BOOL	m_bMotorOn;
	BOOL    m_bSelected;

private:
	int		m_nRotateSeq;


public:
	virtual void ComverEqPosToImgPos();
	virtual void DrawImg(CDC &dc);
};

class CDoor
{
public:
	CDoor();
	virtual ~CDoor();

public:
	CRect m_DooRect;
	BOOL  m_bDoorOpen;
	bool  bTogle;
	void DrawImg(CDC &dc);
	void SetColor(CBrush* Brush1,CBrush* Brush2) {m_pBrush1 = Brush1; m_pBrush2 = Brush2; }
public:
	CBrush *m_pBrush1,*m_pBrush2;
};

class CEMO:public CAxisMod
{
public:
	CEMO();
	virtual ~CEMO();

	BOOL m_bSwOn;
	BOOL m_bSelected;

private:
	int		m_nRotateSeq;

public:
//	virtual void ComverEqPosToImgPos();
	virtual void DrawImg(CDC &dc);
};



// ======CTransferMod_1============
class CTransferMod_1: public CAxisMod
{
	
public:
	CTransferMod_1();
	virtual ~CTransferMod_1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CTransferMod_2============
class CTransferMod_2: public CAxisMod
{
	
public:
	CTransferMod_2();
	virtual ~CTransferMod_2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CArm============
class CArm: public CAxisMod
{
	
public:
	CArm();
	virtual ~CArm();
	BOOL	 m_bDown;
	BOOL	 m_bGlass[2];
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	CEqGlass2 m_Glass2;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CPitch1============
class CPitch1: public CAxisMod
{
	
public:
	CPitch1();
	virtual ~CPitch1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass[2];
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	CEqGlass2 m_Glass2;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CPitch2============
class CPitch2: public CAxisMod
{
	
public:
	CPitch2();
	virtual ~CPitch2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass[2];
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	CEqGlass2 m_Glass2;

private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CCell1_Up============
class CCell1_Up: public CAxisMod
{
	
public:
	CCell1_Up();
	virtual ~CCell1_Up();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CCell2_Up============
class CCell2_Up: public CAxisMod
{
	
public:
	CCell2_Up();
	virtual ~CCell2_Up();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CCell1_Down============
class CCell1_Down: public CAxisMod
{
	
public:
	CCell1_Down();
	virtual ~CCell1_Down();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CCell2_Down============
class CCell2_Down: public CAxisMod
{
	
public:
	CCell2_Down();
	virtual ~CCell2_Down();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CTrayRobot============
class CTrayRobot: public CAxisMod
{
	
public:
	CTrayRobot();
	virtual ~CTrayRobot();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CTray1============
class CTray1: public CAxisMod
{
	
public:
	CTray1();
	virtual ~CTray1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CTray2============
class CTray2: public CAxisMod
{
	
public:
	CTray2();
	virtual ~CTray2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CAlignClamp1============
class CAlignClamp1: public CAxisMod
{
	
public:
	CAlignClamp1();
	virtual ~CAlignClamp1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CAlignClamp2============
class CAlignClamp2: public CAxisMod
{
	
public:
	CAlignClamp2();
	virtual ~CAlignClamp2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CAlignClamp3============
class CAlignClamp3: public CAxisMod
{
	
public:
	CAlignClamp3();
	virtual ~CAlignClamp3();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CArm1============
class CArm1: public CAxisMod
{
	
public:
	CArm1();
	virtual ~CArm1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CArm2============
class CArm2: public CAxisMod
{
	
public:
	CArm2();
	virtual ~CArm2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CTransfer2_1============
class CTransfer2_1: public CAxisMod
{
	
public:
	CTransfer2_1();
	virtual ~CTransfer2_1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CTransfer2_2============
class CTransfer2_2: public CAxisMod
{
	
public:
	CTransfer2_2();
	virtual ~CTransfer2_2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CAligner1============
class CAligner1: public CAxisMod
{
	
public:
	CAligner1();
	virtual ~CAligner1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CAligner2============
class CAligner2: public CAxisMod
{
	
public:
	CAligner2();
	virtual ~CAligner2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};


// ======CTransfer1_1============
class CTransfer1_1: public CAxisMod
{
	
public:
	CTransfer1_1();
	virtual ~CTransfer1_1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CTransfer1_2============
class CTransfer1_2: public CAxisMod
{
	
public:
	CTransfer1_2();
	virtual ~CTransfer1_2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CWorkbuffer1============
class CWorkbuffer1: public CAxisMod
{
	
public:
	CWorkbuffer1();
	virtual ~CWorkbuffer1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CWorkbuffer2============
class CWorkbuffer2: public CAxisMod
{
	
public:
	CWorkbuffer2();
	virtual ~CWorkbuffer2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};
// ======CWorkbuffer3============
class CWorkbuffer3: public CAxisMod
{
	
public:
	CWorkbuffer3();
	virtual ~CWorkbuffer3();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};
// ======CWorkbuffer4============
class CWorkbuffer4: public CAxisMod
{
	
public:
	CWorkbuffer4();
	virtual ~CWorkbuffer4();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CModuleRbt============
class CModuleRbt: public CAxisMod
{
	
public:
	CModuleRbt();
	virtual ~CModuleRbt();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CLeftHSRbt============
class CLeftHSRbt: public CAxisMod
{
	
public:
	CLeftHSRbt();
	virtual ~CLeftHSRbt();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CRightHSRbt============
class CRightHSRbt: public CAxisMod
{
	
public:
	CRightHSRbt();
	virtual ~CRightHSRbt();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CClipRbt============
class CClipRbt: public CAxisMod
{
	
public:
	CClipRbt();
	virtual ~CClipRbt();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CClampRbt============
class CClampRbt: public CAxisMod
{
	
public:
	CClampRbt();
	virtual ~CClampRbt();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CSorterRbt============
class CSorterRbt: public CAxisMod
{
	
public:
	CSorterRbt();
	virtual ~CSorterRbt();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CUnloaderRbt============
class CUnloaderRbt: public CAxisMod
{
	
public:
	CUnloaderRbt();
	virtual ~CUnloaderRbt();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CMDStacker1============
class CMDStacker1: public CAxisMod
{
	
public:
	CMDStacker1();
	virtual ~CMDStacker1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CMDStacker2============
class CMDStacker2: public CAxisMod
{
	
public:
	CMDStacker2();
	virtual ~CMDStacker2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CLHSStacker1============
class CLHSStacker1: public CAxisMod
{
	
public:
	CLHSStacker1();
	virtual ~CLHSStacker1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CLHSStacker2============
class CLHSStacker2: public CAxisMod
{
	
public:
	CLHSStacker2();
	virtual ~CLHSStacker2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CRHSStacker1============
class CRHSStacker1: public CAxisMod
{
	
public:
	CRHSStacker1();
	virtual ~CRHSStacker1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CRHSStacker2============
class CRHSStacker2: public CAxisMod
{
	
public:
	CRHSStacker2();
	virtual ~CRHSStacker2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CClipTube1============
class CClipTube1: public CAxisMod
{
	
public:
	CClipTube1();
	virtual ~CClipTube1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CClipTube2============
class CClipTube2: public CAxisMod
{
	
public:
	CClipTube2();
	virtual ~CClipTube2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CULDStacker1============
class CULDStacker1: public CAxisMod
{
	
public:
	CULDStacker1();
	virtual ~CULDStacker1();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CULDStacker2============
class CULDStacker2: public CAxisMod
{
	
public:
	CULDStacker2();
	virtual ~CULDStacker2();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ======CRecject============
class CRecject: public CAxisMod
{
	
public:
	CRecject();
	virtual ~CRecject();
	BOOL	 m_bDown;
	BOOL	 m_bGlass;
	BOOL     m_bSelected;
	double   m_dEqCurPos_Z;
	
public:
	CEqGlass m_Glass;
	
private:
	CDC      m_DcDownPad; 
	CBitmap  m_BitDownPad;
	CBitmap* m_pBitDownPad;
	
	CDC      m_DcDownPadMsk; 
	CBitmap  m_BitDownPadMsk;
	CBitmap* m_pBitDownPadMsk;	
	CSize	 m_DownSize;
	double	 m_dRadioZ;
	
public: 
	virtual void DrawImg(CDC &dc);
	virtual void CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk);
	virtual void ComverEqPosToImgPos();	
	void SetRatioEqToImgZ(const double dValue)	{m_dRadioZ		= dValue;}
};

// ******************************************************************
//						구현 부 
// ******************************************************************

class CAnimate : public CWnd
{
// Construction
public:
	CAnimate();
	
	CDC  m_FrameBase; 
   	CBitmap  m_FrameBaseBmp;
 	CBitmap* m_pFrameBaseBmp;

	// Base Axis 
	CDC m_FrameCarryRobot, m_FramePCBRobot, m_FramePickerRobot; 
	CBitmap m_FrameCarryRobotBmp, m_FramePCBRobotBmp, m_FramePickerRobotBmp;
	CBitmap *m_pFrameCarryRobotBmp, *m_pFramePCBRobotBmp, *m_pFramePickerRobotBmp;
	
	CTransferMod_1 m_Transfer1;
	CTransferMod_2 m_Transfer2;
	CArm           m_Arm;
	CPitch1        m_Pitch1;
	CPitch2        m_Pitch2;
	CCell1_Up      m_Cell1_Up;
	CCell2_Up      m_Cell2_Up;
	CCell1_Down    m_Cell1_Down;
	CCell2_Down    m_Cell2_Down;
	CTrayRobot     m_TrayRobot;
	CTray1         m_Tray1;
	CTray2         m_Tray2;
	CAlignClamp1   m_AlignClamp1;
	CAlignClamp2   m_AlignClamp2;
	CAlignClamp3   m_AlignClamp3;
////////////////////////////////////////////////////////////////////

	CWorkbuffer1   m_Workbuffer1;
	CWorkbuffer2   m_Workbuffer2;
	CWorkbuffer3   m_Workbuffer3;
	CWorkbuffer4   m_Workbuffer4;

	CModuleRbt     m_ModuleRbt;
	CLeftHSRbt	   m_LeftHSRbt;
	CRightHSRbt	   m_RightHSRbt;
	CClipRbt	   m_ClipRbt;
	CClampRbt      m_ClampRbt;
	CSorterRbt     m_SorterRbt;
	CUnloaderRbt   m_UnloaderRbt;

	CMDStacker1     m_MDStacker1;
	CMDStacker2     m_MDStacker2;
	CLHSStacker1    m_LHSStacker1;
	CLHSStacker2    m_LHSStacker2;
	CRHSStacker1    m_RHSStacker1;
	CRHSStacker2    m_RHSStacker2;
	CClipTube1      m_ClipTube1;
	CClipTube2      m_ClipTube2;
	CULDStacker1    m_ULDStacker1;
	CULDStacker2    m_ULDStacker2;
	CRecject		m_Reject;

///////////////////////////////////////////////////////////////////////////
	CCrrPkTransferMod m_CarryPickRototUnit;
	CPCBPkTransferLMod m_PCBPickRobotUnitL;
	CPCBPkTransferRMod m_PCBPickRobotUnitR;
	CAttPkTransferMod m_AttachPickRobotUnit;
	CLoadCvTransferMod m_LoadConvPitchUnit;
	CAttCvTransferMod m_AttConvPitchUnit;
	CAttPkRbtLTransferMod m_AttPickRobotLUnit;
	CAttPkRbtRTransferMod m_AttPickRobotRUnit;

 	CBufPlateLTransferMod m_BufPlateLUnit;
 	CBufPlateRTransferMod m_BufPlateRUnit;

	CTrnaferMod m_LCamUnit;
	CTrnaferMod m_RCamUnit;

	CPoint pRejectPt;

	CRect m_BtnRect[3];
	BOOL m_bClicked[3];

	int nRandCol;
	int nColChange;
	int nColChangeWait[3];

public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimate)
	public:
	//}}AFX_VIRTUAL
	virtual BOOL Create ( DWORD exstyle, DWORD style, const CRect& r,CWnd *p, UINT id, CCreateContext *pc=NULL ) ;

// Implementation
public:
	void TestMove(double &dPos,int nTpos);
	void ActionDisplay(CDC *pDC = NULL);
	void CylImageChange(CDC *pDC = NULL);
	void uMesJudgResultMonitoring(CDC *pDC, int *nMEStoEQ_TotalSotInfo, int *nPCBVisionGoodInfo, int *nPickerPickUpDnInfo, int *nMEStoEQ_TotalSotLInfoTmp, int *nMEStoEQ_TotalSotRInfoTmp);
	void uRejectBufPcbMonitoring(CDC *pDC, int *nRejectBufPcbExistInfo);
	void uGoodBufLPcbMonitoring(CDC *pDC, int *nLeftGoodBufExistInfo);
	void uGoodBufRPcbMonitoring(CDC *pDC, int *nRightGoodBufExistInfo);
	void uBtnUndLfInfoReset(CDC *pDC);
	void uBtnCovLfInfoReset(CDC *pDC);
	void InitAniWnd();
	void AniPioStatus();

	void ModuleTrayCheck(CDC *pDC);
	void LHSTrayCheck(CDC *pDC);
	void RHSTrayCheck(CDC *pDC);
	void ClipTube1Check(CDC *pDC);
	void ClipTube2Check(CDC *pDC);
	void ULDTrayCheck(CDC *pDC);
	void RejectTrayCheck(CDC *pDC);
	void MDTray1Check(CDC *pDC);
	void LHSTray1Check(CDC *pDC);
	void RHSTray1Check(CDC *pDC);
	void ULDTray1Check(CDC *pDC);

	virtual ~CAnimate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAnimate)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	CFont	m_AniFont;
	CPen	m_AniPen, m_AniPen1;
	CBrush  m_AniBrush, m_AniBrush1;
	CFont	m_AniBicFont;
	CFont	m_AniSmallFont;
	CPoint	m_CursorPoint;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Animate_H__31696054_73E4_4AC0_9F8C_C6739EEC9C94__INCLUDED_)
