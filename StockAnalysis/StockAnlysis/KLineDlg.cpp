// KLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StockAnlysis.h"
#include "KLineDlg.h"
#include "Configuration.h"
#include "Utilities.h"
#include "TrendStrategy.h"
#include "DlgMAEditor.h"
#include "CrossMAStrategy.h"
#include "TrendStrategy.h"
#include "VolumeCrossStrategy.h"
#include "TrendMACrossStrategy.h"
#include "BOLLLine.h"
#include "weekline.h"
#include "MonthLine.h"
#include "macd.h"

const int KLINE_WIDTH = 7;
const int VERT_MARGIN = 40;
const int HORI_MARGIN = 60;
const int HORZ_LINE_NUM = 8;
const int LINE_WIDTH = 2;
const int KWIDTH[] = {1, 3, 5, 7, 9, 11};

const COLORREF MA_COLORS[] = {RGB(255,255,255), RGB(255,255,0), RGB(255, 0, 255), RGB(0, 255, 0)}; 

const COLORREF CR_BACKGROUND = RGB(0, 0, 0);
const COLORREF CR_FOREGROUND = RGB(255, 0, 0);

// CKLineDlg dialog

IMPLEMENT_DYNAMIC(CKLineDlg, CDialog)

CKLineDlg::CKLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKLineDlg::IDD, pParent)
{
	m_lowest = 0;
	m_highest = 0;
	m_highest = 0;
	m_lowest = 0;
	m_product = NULL;
	m_bKLineVisible = TRUE;
	m_scale = 3;
	m_ptPrevious.x = 0;
	m_ptPrevious.y = 0;
	m_bShowCross = FALSE;
	m_currcycle = LINE_DAY;
	m_currind = IND_MACD;
	m_indicator = 0;
}

CKLineDlg::~CKLineDlg()
{
	//绘图完成后的清理  
	if (MemDC.GetSafeHdc())
	{
		MemDC.SelectObject(pOldBit);
		MemDC.DeleteDC();
	}

	if (MemBitmap.GetSafeHandle())
	{
		MemBitmap.DeleteObject();  
	}

	// 清空所有均线
	foreach(m_malines, it, vector<MALine*>)
	{
		MALine* p = *it;
		delete p;
	}

	m_malines.clear();

	//
	if (m_indicator)
	{
		delete m_indicator;
	}

	g_Configuration.removeObserver(this);
}

void CKLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKLineDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CAL_MA1, &CKLineDlg::OnCalMa1)
	ON_COMMAND(ID_CAL_TREND1, &CKLineDlg::OnCalTrend1)
	ON_COMMAND(ID_CAL_TRENDCROSS, &CKLineDlg::OnCalTrendcross)
	ON_COMMAND(ID_CAL_VOLCROSS, &CKLineDlg::OnCalVolcross)
	ON_COMMAND(ID_MA_ADD, &CKLineDlg::OnMaAdd)
	ON_WM_CLOSE()
	ON_COMMAND(ID_IND_BOLL, &CKLineDlg::OnIndBoll)
	ON_COMMAND(ID_IND_MACD, &CKLineDlg::OnIndMacd)
	ON_COMMAND(ID_IND_KDJ, &CKLineDlg::OnIndKdj)
	ON_COMMAND(ID_IND_RSI, &CKLineDlg::OnIndRsi)
	ON_COMMAND(ID_CYCLE_DAY, &CKLineDlg::OnCycleDay)
	ON_COMMAND(ID_CYCLE_WEEK, &CKLineDlg::OnCycleWeek)
	ON_COMMAND(ID_CYCLE_MONTH, &CKLineDlg::OnCycleMonth)
	ON_COMMAND(ID_CAL_CROSSMACD, &CKLineDlg::OnCalCrossmacd)
	ON_UPDATE_COMMAND_UI(ID_CYCLE_DAY, &CKLineDlg::OnUpdateCycleDay)
	ON_UPDATE_COMMAND_UI(ID_CYCLE_WEEK, &CKLineDlg::OnUpdateCycleWeek)
	ON_UPDATE_COMMAND_UI(ID_CYCLE_MONTH, &CKLineDlg::OnUpdateCycleMonth)
	ON_WM_INITMENUPOPUP()
	ON_UPDATE_COMMAND_UI(ID_CAL_MA1, &CKLineDlg::OnUpdateCalMa1)
	ON_UPDATE_COMMAND_UI(ID_CAL_CROSSMACD, &CKLineDlg::OnUpdateCalCrossmacd)
	ON_UPDATE_COMMAND_UI(ID_CAL_TREND1, &CKLineDlg::OnUpdateCalTrend1)
	ON_COMMAND(ID_STRATEGY_MA, &CKLineDlg::OnStrategyMa)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_MA, &CKLineDlg::OnUpdateStrategyMa)
	ON_COMMAND(ID_STRATEGY_NEWHIGHLOW, &CKLineDlg::OnStrategyNewhighlow)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_NEWHIGHLOW, &CKLineDlg::OnUpdateStrategyNewhighlow)
	ON_COMMAND(ID_STRATEGY_MACD, &CKLineDlg::OnStrategyMacd)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_MACD, &CKLineDlg::OnUpdateStrategyMacd)
	ON_UPDATE_COMMAND_UI(ID_CAL_TRENDCROSS, &CKLineDlg::OnUpdateCalTrendcross)
	ON_UPDATE_COMMAND_UI(ID_CAL_VOLCROSS, &CKLineDlg::OnUpdateCalVolcross)
	ON_COMMAND(ID_STRATEGY_REBOUND, &CKLineDlg::OnStrategyRebound)
	ON_COMMAND(ID_STRTEGY_OFFSET, &CKLineDlg::OnStrtegyOffset)
END_MESSAGE_MAP()


BOOL CKLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	ASSERT(m_product != NULL);

	m_quotations = m_product->GetQuotations();

	m_indicator = new MACD(12, 26, 9);

	ResetAllPosition();

	CalcMALines();

	CalcIndicator();

	CalcStrategies();

	CString str;
	str.Format("股票代码: %s", m_product->code().c_str());

	SetWindowText(str);

	g_Configuration.addObserver(this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// CKLineDlg message handlers

void CKLineDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	CRect rc;
	GetClientRect(&rc);

	//将内存中的图拷贝到屏幕上进行显示  
	//SCROLLINFO si;
	//GetScrollInfo(SB_HORZ, &si);

	//将内存图拷贝到屏幕
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);  
}

void CKLineDlg::DrawOneLine(CDC* pDC, CRect& rc, int x, COLORREF crLine, Quot& sti, BOOL turnpoint)
{
	int nHeight = rc.Height();

	int yBegin = rc.bottom - ((sti.Low - m_lowest)*nHeight/(m_highest-m_lowest));
	int yEnd = rc.bottom - ((sti.High - m_lowest)*nHeight/(m_highest-m_lowest));

	int yOpen = rc.bottom - ((sti.Open - m_lowest)*nHeight/(m_highest-m_lowest));
	int yClose = rc.bottom - ((sti.Close - m_lowest)*nHeight/(m_highest-m_lowest));

	CRect rcLine(rc.left+x*GetKWidth()+1, yOpen, rc.left+x*GetKWidth()+GetKWidth()-1, yClose);

	CPen newPen;

	newPen.CreatePen(PS_SOLID, 1, crLine);

	CPen* pOldPen = pDC->SelectObject(&newPen);

	CBrush newBrush;
	newBrush.CreateSolidBrush(crLine);

	CBrush* pOldBrush = pDC->SelectObject(&newBrush);

    pDC->MoveTo(rcLine.left+GetKWidth()/2-1, yBegin);
	pDC->LineTo(rcLine.left+GetKWidth()/2-1, yEnd);

	pDC->Rectangle(rcLine);

	pDC->SelectObject(pOldPen);

	BOOL bPt = FALSE;
	COLORREF crPt;

	vector<string>& buyPoints = m_strategycalculator.buyPoints();
	vector<string>& sellPoints = m_strategycalculator.sellPoints();

	if (find(buyPoints.begin(), buyPoints.end(), sti.getDatetime()) != buyPoints.end())
	{
		crPt = RGB(255, 0, 0);
		bPt = TRUE;
	}
	else if (find(sellPoints.begin(), sellPoints.end(), sti.getDatetime()) != sellPoints.end())
	{
		crPt = RGB(0, 255, 0);
		bPt = TRUE;
	}
	
	if (bPt)
	{
		CPen ptPen;
		ptPen.CreatePen(PS_SOLID, 1, crPt);

		pOldPen = pDC->SelectObject(&ptPen);
		
		pDC->MoveTo(rcLine.left+LINE_WIDTH, rc.top);
		pDC->LineTo(rcLine.left+LINE_WIDTH, rc.bottom);
		
		pDC->SelectObject(pOldPen);
	}


	if (turnpoint)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(rcLine.left+LINE_WIDTH, yBegin, "!");
	}

	//pDC->SelectObject(pOldPen);

	pDC->SelectObject(pOldBrush);
}

void CKLineDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call defaul
	SCROLLINFO scrollInfo;          
	GetScrollInfo(SB_HORZ,   &scrollInfo,   SIF_ALL);          
	switch   (nSBCode)          
	{          
	case   SB_LINEUP:              
		scrollInfo.nPos -= 1;              
		if   (scrollInfo.nPos   <   scrollInfo.nMin)              
		{                  
			scrollInfo.nPos =   scrollInfo.nMin;                  
		}              
		SetScrollInfo(SB_HORZ,   &scrollInfo,   SIF_ALL);              
		//ScrollWindow(10,   0);              
		break;          
	case   SB_LINEDOWN:             
		scrollInfo.nPos += 1;              
		if   (scrollInfo.nPos   >   scrollInfo.nMax)              
		{                  
			scrollInfo.nPos =   scrollInfo.nMax;                  
		}              
		SetScrollInfo(SB_HORZ,   &scrollInfo,   SIF_ALL);              
		//ScrollWindow(-10,   0);              
		break;          
	case   SB_TOP:              
		//ScrollWindow(  (scrollInfo.nPos   -   scrollInfo.nMin)   *   10,0);              
		scrollInfo.nPos =   scrollInfo.nMin;              
		SetScrollInfo(SB_HORZ,   &scrollInfo,   SIF_ALL);              
		break;          
	case   SB_BOTTOM:              
		//ScrollWindow(  -(scrollInfo.nMax   -   scrollInfo.nPos)   *   10,0);              
		scrollInfo.nPos =   scrollInfo.nMax;              
		SetScrollInfo(SB_HORZ,   &scrollInfo,   SIF_ALL);             
		break;          
	case   SB_PAGEUP:              
		scrollInfo.nPos -= 5;              
		if   (scrollInfo.nPos   <   scrollInfo.nMin)              
		{                 
			scrollInfo.nPos =   scrollInfo.nMin;                  
		}              
		SetScrollInfo(SB_HORZ,&scrollInfo,SIF_ALL);              
		//ScrollWindow(  10   *   5,0);              
		break;          
	case   SB_PAGEDOWN:              
		scrollInfo.nPos += 5;              
		if   (scrollInfo.nPos   >   scrollInfo.nMax)              
		{                  
			scrollInfo.nPos =   scrollInfo.nMax;                  
		}              
		SetScrollInfo(SB_HORZ,   &scrollInfo,   SIF_ALL);              
		//ScrollWindow(   -10   *   5,0);              
		break;          
	case   SB_ENDSCROLL:              
		break;          
	case   SB_THUMBPOSITION:              
		break;          
	case   SB_THUMBTRACK:              
		//ScrollWindow((scrollInfo.nPos   -   nPos)   *   10, 0  );              
		scrollInfo.nPos =   nPos;              
		SetScrollInfo(SB_HORZ,   &scrollInfo,   SIF_ALL);              
		break;          
	}

	DrawMemDC();

	InvalidateRect(NULL);

	//UpdateWindow();
	//CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CKLineDlg::DrawMemDC()
{
	ASSERT(m_product != NULL);

	// clear memory dc
	if (MemDC.GetSafeHdc() != NULL)
	{
		MemDC.SelectObject(pOldBit);
		MemDC.DeleteDC();
	}

	// clear memory bitmap
	if (MemBitmap.GetSafeHandle() != NULL)
	{
		MemBitmap.DeleteObject();
	}

	//
	CalculateMaxMin();
		
     // 
	CClientDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	//随后建立与屏幕显示兼容的内存显示设备  
	MemDC.CreateCompatibleDC(NULL);  

	//这时还不能绘图，因为没有地方画 ^_^  
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小  
	MemBitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());  

	//将位图选入到内存显示设备中  
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上  
	pOldBit = MemDC.SelectObject(&MemBitmap);  

	//先用背景色将位图清除干净，这里我用的是白色作为背景  
	//你也可以用自己应该用的颜色  
	MemDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), CR_BACKGROUND);  

	// 
	DrawLegend(&MemDC, GetLegendRect());

	// draw x y axis
    DrawXYAxis(&MemDC, GetValidRect());

	// draw all K lines
	if (m_bKLineVisible)
	{
		DrawAllKLines(&MemDC, GetValidRect());
	}

	// 画所有的均线
	int i = 0;
	foreach(m_malines, it, vector<MALine*>)
	{
		MALine& line = **it;

		DrawAllMeanLines(&MemDC, GetValidRect(), line.cycle(), line.getMAItems(), MA_COLORS[i%4]);

		i++;
	}

	// 画所有的策略信息
	DrawStrategies(&MemDC);

	//
	DrawIndicatorXY(&MemDC, GetMiddleRect());

	//
	DrawIndicator(&MemDC, GetMiddleRect());

	// 画成交量坐标轴
	DrawVolumeXY(&MemDC, GetBottomRect());

	// 画成交量
	DrawVolume(&MemDC, GetBottomRect());
}

void CKLineDlg::DrawAllKLines(CDC* pDC, CRect& rc)
{
	int i = 0;

	vector<Quot>& klines = GetQuotations();

	int nEnd = (GetPosition() + GetSizePerPage() < klines.size())?(GetPosition() + GetSizePerPage()):klines.size();

	TRACE("Page Size: begin: %d, end: %d\n", GetPosition(), nEnd);

	for (int n = GetPosition(); n < nEnd; n++)
	{
		COLORREF color;

		Quot& sti = klines[n];

		if (sti.isUpLine())
		{
			color = RGB(0xFF, 0, 0);
		}
		else if (sti.isDownLine())
		{
			color = RGB(0, 0xFF, 0);
		}
		else
		{
			color = RGB(0, 0, 0xFF); 
		}
		
		DrawOneLine(pDC, rc, i, color, sti, FALSE);	

		i++;
	}
}

void CKLineDlg::DrawAllMeanLines(CDC* pDC, CRect& rc, int nCycle, vector<int>& meanLine, COLORREF crColor)
{
	CPen newPen;

	newPen.CreatePen(PS_SOLID, LINE_WIDTH, crColor);

	CPen* pOldPen = pDC->SelectObject(&newPen);

	int nHeight = rc.Height();

	BOOL bHasOriginalPoint = FALSE;

    // 画剩余的其它点
	int nEnd = (GetPosition() + GetSizePerPage() < meanLine.size())?(GetPosition() + GetSizePerPage()):meanLine.size();

	int i = 0;

	for (int n = GetPosition(); n < nEnd; n++)
	{
		// 计算x和y坐标
		int x = rc.left + GetKWidth()*i + GetKWidth()/2 + 1;

		i++;

		// 计算y坐标
		int y = rc.bottom - (meanLine[n] - m_lowest)*nHeight/(m_highest-m_lowest);

		// 计算起始点Y坐标
		if (meanLine[n] != INVALID_VALUE && !bHasOriginalPoint)
		{
			pDC->MoveTo(x, y);

			bHasOriginalPoint = TRUE;

			continue;
		}

		if (bHasOriginalPoint)
		{
			pDC->LineTo(x, y);
		}
	}

	pDC->SelectObject(pOldPen);
}

void CKLineDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	ASSERT(m_product != NULL);

	if (GetQuotations().size() <= 0)
	{
		return;
	}

	SetScrollRange(SB_HORZ, 0, GetQuotations().size()-1, TRUE);

	SCROLLINFO si;
	GetScrollInfo(SB_HORZ, &si);
	si.nPage = GetSizePerPage();
	SetScrollInfo(SB_HORZ, &si);

	DrawMemDC();

	InvalidateRect(NULL);
}

CRect CKLineDlg::GetValidRect()
{
	CRect rc;
	GetClientRect(rc);

	int height = rc.Height();

	rc.left += HORI_MARGIN;
	rc.right -= HORI_MARGIN;
	rc.top += VERT_MARGIN;
	rc.bottom -= height/2;

	return rc;
}

CRect CKLineDlg::GetMiddleRect()
{
	CRect rc;
	GetClientRect(rc);

	int height = rc.Height();

	rc.left += HORI_MARGIN;
	rc.right -= HORI_MARGIN;
	rc.top += height/2 + VERT_MARGIN;
	rc.bottom = rc.top + height/4 - VERT_MARGIN;

	return rc;
}

CRect CKLineDlg::GetBottomRect()
{
	CRect rc;
	GetClientRect(rc);

	rc.left += HORI_MARGIN;
	rc.right -= HORI_MARGIN;
	rc.top += rc.Height()*3/4 + VERT_MARGIN;
	rc.bottom -= VERT_MARGIN;

	return rc;
}

void CKLineDlg::DrawXYAxis(CDC* pDC, CRect& rc)
{
	CPen newPen;
	newPen.CreatePen(PS_SOLID, LINE_WIDTH, CR_FOREGROUND);

	CPen* pOldPen = pDC->SelectObject(&newPen);

	CBrush newBrush;
	newBrush.CreateSolidBrush(CR_BACKGROUND);

	CBrush* pOldBrush = pDC->SelectObject(&newBrush);

	pDC->Rectangle(&rc);

	pDC->SelectObject(pOldPen);

	pDC->SelectObject(pOldBrush);

	CPen penLine;
	
	penLine.CreatePen(PS_DOT, 1, CR_FOREGROUND);

	pOldPen = pDC->SelectObject(&penLine);

	int nGap = rc.Height()/HORZ_LINE_NUM;

	int nPriceGap = (m_highest-m_lowest)/HORZ_LINE_NUM;
	
	for (int i = 1; i < HORZ_LINE_NUM; i++)
	{
		int nY = rc.bottom - nGap*i;

		// Draw X 
		pDC->MoveTo(rc.left, nY);
		pDC->LineTo(rc.right, nY);

		//在Y轴画价格

		CString str;
		str.Format("%.2f", double(m_lowest+nPriceGap*i)/100);

		CRect rcText(0, nY - 10, HORI_MARGIN - 2, nY + 10);
		pDC->DrawText(str, &rcText, DT_RIGHT|DT_VCENTER);
	}

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(CR_FOREGROUND);

	//在X轴画时间
	CString strBeginDate, strEndDate;

	int nPos = GetPosition();

	vector<Quot>& klines = GetQuotations();

	if (klines.size() > 0 && nPos < klines.size())
    {
		strBeginDate = klines[nPos].getDate().c_str();

		int nLastPos = (klines.size() > nPos+GetSizePerPage())?nPos+GetSizePerPage():klines.size()-1;

		strEndDate = klines[nLastPos].getDate().c_str();

		pDC->TextOut(rc.left + GetKWidth()/2, rc.bottom, strBeginDate);

		int nRight = (nLastPos-nPos) * GetKWidth() + GetKWidth()/2;

		pDC->TextOut(nRight, rc.bottom, strEndDate);
	}

	pDC->SelectObject(pOldPen);
}

void CKLineDlg::DrawVolumeXY(CDC* pDC, CRect& rc)
{
	CPen newPen;

	newPen.CreatePen(PS_SOLID, LINE_WIDTH, CR_FOREGROUND);

	CPen* pOldPen = pDC->SelectObject(&newPen);

	CBrush newBrush;
	newBrush.CreateSolidBrush(CR_BACKGROUND);

	CBrush* pOldBrush = pDC->SelectObject(&newBrush);

	pDC->Rectangle(&rc);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	CPen penLine;

	penLine.CreatePen(PS_DOT, 1, CR_FOREGROUND);

	pOldPen = pDC->SelectObject(&penLine);

	int nGap = rc.Height()/HORZ_LINE_NUM;

	int nPriceGap = (m_highVol-m_lowVol)/HORZ_LINE_NUM;

	for (int i = 1; i < HORZ_LINE_NUM; i++)
	{
		int nY = rc.bottom - nGap*i;

		// Draw X 
		pDC->MoveTo(rc.left, nY);
		pDC->LineTo(rc.right, nY);

		//在Y轴画价格
		CString str;
		str.Format("%d", m_lowVol+nPriceGap*i);

		CRect rcText(0, nY - 10, HORI_MARGIN - 2, nY + 10);
		pDC->DrawText(str, &rcText, DT_RIGHT|DT_VCENTER);
	}

	pDC->SelectObject(pOldPen);
}

void CKLineDlg::DrawVolume(CDC* pDC, CRect& rc)
{
	int i = 0;
	
	vector<Quot>& klines = GetQuotations();

	int nEnd = (GetPosition() + GetSizePerPage() < klines.size())?(GetPosition() + GetSizePerPage()):klines.size();

	TRACE("Page Size: begin: %d, end: %d\n", GetPosition(), nEnd);

	for (int n = GetPosition(); n < nEnd; n++)
	{
		COLORREF color;

		Quot& sti = klines[n];

		if (sti.isUpLine())
		{
			color = RGB(0xFF, 0, 0);
		}
		else if (sti.isDownLine())
		{
			color = RGB(0, 0xFF, 0);
		}
		else
		{
			color = RGB(0, 0, 0xFF); 
		}

		int yBegin = rc.bottom - ((sti.getVolume() - m_lowVol)*rc.Height()/(m_highVol-m_lowVol));

		CRect rcLine(rc.left+i*GetKWidth()+1, yBegin, rc.left+i*GetKWidth()+GetKWidth()-1, rc.bottom-LINE_WIDTH);

		CPen newPen;

		newPen.CreatePen(PS_SOLID, 1, color);

		CPen* pOldPen = pDC->SelectObject(&newPen);

		CBrush newBrush;
		newBrush.CreateSolidBrush(color);

		CBrush* pOldBrush = pDC->SelectObject(&newBrush);

		pDC->Rectangle(rcLine);

		pDC->SelectObject(pOldBrush);

		pDC->SelectObject(pOldPen);

		i++;
	}
}

int CKLineDlg::GetPosition()
{
	SCROLLINFO si;
	GetScrollInfo(SB_HORZ, &si);

	return si.nPos;
}

void CKLineDlg::SetPosition(const int value)
{
	SCROLLINFO si;
	GetScrollInfo(SB_HORZ, &si);

	si.nPos = value;
	si.nPage = GetSizePerPage();
	SetScrollInfo(SB_HORZ, &si);

	InvalidateRect(NULL);
}

int CKLineDlg::GetSizePerPage()
{
	CRect rc = GetValidRect();

	TRACE("Size: %d\n", rc.Width());
	
	return rc.Width()/GetKWidth();
}

void CKLineDlg::SetProduct(Product* product)
{
	ASSERT(product != NULL);

	m_product = product;

	if (GetSafeHwnd() != NULL)
	{
		CString str;
		str.Format("股票代码: %s", m_product->code().c_str());

		SetWindowText(str);

		m_quotations = m_product->GetQuotations();

		CalcCycle();

		CalcMALines();

		ResetAllPosition();

		CalcIndicator();
	
		CalcStrategies();

		Refresh();
	}
}

BOOL CKLineDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CKLineDlg::CalculateMaxMin()
{
	vector<Quot>& klines = GetQuotations();

	if (klines.size() <= 0)
	{
		return;
	}

	int pos = GetPosition();

	int nEnd = (pos + GetSizePerPage() < klines.size())?(GetPosition() + GetSizePerPage()):klines.size();

	m_highest = klines[pos].High;
	m_lowest = klines[pos].Low;

	m_highVol = klines[pos].getVolume();
	m_lowVol = klines[pos].getVolume();

	for (int n = GetPosition() + 1; n < nEnd; n++)
	{
		Quot& sti = klines[n];

		m_highest = max(m_highest, sti.High);

		m_lowest = min(m_lowest, sti.Low);

		m_highVol = max(m_highVol, sti.getVolume());

		m_lowVol = min(m_lowVol, sti.getVolume());
	}

	// 计算均线的最大最小值
	foreach(m_malines, it, vector<MALine*>)
	{
		for (int n = GetPosition(); n < nEnd; n++)
		{
			vector<int>& vt = (*it)->getMAItems();

			if (vt[n] != INVALID_VALUE)
			{
				m_highest = max(m_highest, vt[n]);

				m_lowest = min(m_lowest, vt[n]);
			}
		}
	}

	m_highest = m_highest*110/100;
	m_lowest = m_lowest*90/100;

	m_highVol = m_highVol*110/100;
	m_lowVol = m_lowVol*90/100;

	TRACE("m_highest: %d, m_lowest: %d\n", m_highest, m_lowest);
	TRACE("m_highvol: %d, m_lowvol: %d\n", m_highVol, m_lowVol);
}

BOOL CKLineDlg::PreTranslateMessage(MSG* pMsg) 
{ 
	SCROLLINFO si;
	GetScrollInfo(SB_HORZ, &si);

	if(pMsg->message == WM_KEYDOWN) 
	{ 
		switch (pMsg->wParam) 
		{ 
		case VK_LEFT: 
			si.nPos -= 1;
			if (si.nPos < 0)
			{
				si.nPos = 0;
			}
			SetScrollPos(SB_HORZ, si.nPos);
			DrawMemDC();
			InvalidateRect(NULL);
			UpdateWindow();
			break; 
		case VK_RIGHT: 
			si.nPos += 1;
			if (si.nPos > si.nMax)              
			{                  
				si.nPos = si.nMax;                  
			}              
			SetScrollPos(SB_HORZ, si.nPos);
			Refresh();
			break; 
		case VK_UP: 
			if (m_scale < sizeof(KWIDTH)/sizeof(int) - 1)
			{
				m_scale++;

				SCROLLINFO si;
				GetScrollInfo(SB_HORZ, &si);
				si.nPage = GetSizePerPage();

				if (si.nPos + GetSizePerPage() > si.nMax)
				{
					si.nPos = si.nMax - si.nPage;
				}

				SetScrollInfo(SB_HORZ, &si);

				Refresh();
			}

			break; 
		case VK_DOWN: 
			if (m_scale > 0)
			{
				m_scale--;

				SCROLLINFO si;
				GetScrollInfo(SB_HORZ, &si);
				si.nPage = GetSizePerPage();

				if (si.nPos + GetSizePerPage() > si.nMax)
				{
					si.nPos = si.nMax - si.nPage;
				}

				SetScrollInfo(SB_HORZ, &si);

				Refresh();
			}
			break; 
		case VK_ESCAPE:
			if (m_dlgQuotLegend.GetSafeHwnd() && m_bShowCross)
			{
				m_bShowCross = FALSE;
				m_dlgQuotLegend.ShowWindow(SW_HIDE);
				Refresh();
			}
			break;
		default:
			break;
		} 
	} 

	return CDialog::PreTranslateMessage(pMsg); 
} 

void CKLineDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKLineDlg::SetKLineVisible(BOOL bVisible)
{
	m_bKLineVisible = bVisible;

	InvalidateRect(NULL);

	UpdateWindow();
}

int CKLineDlg::GetKWidth()
{
	int nRet = KWIDTH[m_scale];

	return nRet;
}

CRect CKLineDlg::GetLegendRect()
{
	CRect rc;
	GetClientRect(rc);

	rc.left += HORI_MARGIN;
	rc.right -= HORI_MARGIN;
	rc.top = 10;
	rc.bottom = VERT_MARGIN;

	return rc;
}

void CKLineDlg::DrawLegend(CDC* pDC, CRect& rc)
{
	const int TEXTWIDTH = 50;

	int i = 0;

	int x = 0;

	// 画均线 
	foreach(m_malines, it, vector<MALine*>)
	{
		x = rc.left + TEXTWIDTH*i;

		CRect rcText(x, rc.top, x+TEXTWIDTH, rc.bottom);

		CString str;
		str.Format("MA%d", (*it)->cycle());

		pDC->SetBkMode(TRANSPARENT);

		pDC->SetTextColor(MA_COLORS[i%4]);

		pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER);

		i++;
	}

	// 画买卖次数 
	x += TEXTWIDTH;

	CString str;

	pDC->SetBkMode(TRANSPARENT);

	pDC->SetTextColor(CR_FOREGROUND);

	CRect rcText(x, rc.top, rc.right, rc.bottom);

	str.Format("买入次数: %d, 卖出次数: %d, 持股收益率: %.2f, 策略收益率: %.2f, 正收益比例: %.2f", 
		m_strategycalculator.buyPoints().size(), m_strategycalculator.sellPoints().size(),
		m_product->GetIncreaseRate(), m_strategycalculator.GetROI(), m_strategycalculator.GetRateOfPosGain());

	pDC->DrawText(str, &rcText, DT_LEFT|DT_VCENTER);
}

void CKLineDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GetFocus() != this)
	{
		SetFocus();
	}

	if (!m_bShowCross)
	{
		return;
	}

	CClientDC dc(this);
	dc.DPtoLP(&point);

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	CPen* pOldPen = dc.SelectObject(&pen);

	dc.SetROP2(R2_NOT);

	CRect rc; //= GetValidRect();
	GetClientRect(&rc);
	rc.left += HORI_MARGIN;
	rc.right -= HORI_MARGIN;
	rc.top += VERT_MARGIN;
	rc.bottom -= VERT_MARGIN;

	CPoint pt(0, 0);

	// 抹掉前面的橡皮筋
	if (m_ptPrevious != pt)
	{
		dc.MoveTo(m_ptPrevious.x, rc.top);
		dc.LineTo(m_ptPrevious.x, rc.bottom);

		dc.MoveTo(rc.left, m_ptPrevious.y);
		dc.LineTo(rc.right, m_ptPrevious.y);

		m_ptPrevious.x = 0;
		m_ptPrevious.y = 0;
	}

	// 画新的橡皮筋 
	if (rc.PtInRect(point))
	{
		dc.MoveTo(point.x, rc.top);
		dc.LineTo(point.x, rc.bottom);

		dc.MoveTo(rc.left, point.y);
		dc.LineTo(rc.right, point.y);

		m_ptPrevious = point;

		ShowQuotLegend();
	}

	dc.SelectObject(pOldPen);

	CDialog::OnMouseMove(nFlags, point);
}

void CKLineDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CClientDC dc(this);

	dc.DPtoLP(&point);

	CRect rc = GetValidRect();

	if (!m_bShowCross && rc.PtInRect(point))
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

		CPen* pOldPen = dc.SelectObject(&pen);

		dc.SetROP2(R2_NOT);

		// 画新的橡皮筋 
		if (rc.PtInRect(point))
		{
			dc.MoveTo(point.x, rc.top);
			dc.LineTo(point.x, rc.bottom);

			dc.MoveTo(rc.left, point.y);
			dc.LineTo(rc.right, point.y);

			m_ptPrevious = point;

			ShowQuotLegend();
		}

		dc.SelectObject(pOldPen);

		m_bShowCross = TRUE;
	}

	//
	CDialog::OnLButtonDown(nFlags, point);
}

Quot CKLineDlg::GetCurrentQuot()
{
	ASSERT(m_product != NULL);

	Quot qt;

	CPoint pt;

	GetCursorPos(&pt);

	ScreenToClient(&pt);

	vector<Quot>& klines = GetQuotations();

	if (!klines.empty())
	{
		int i = GetPosition() + (pt.x - HORI_MARGIN) /GetKWidth();

		if (i > klines.size() - 1)
		{
			i = klines.size() - 1;
		}

		qt = klines[i];
	}

	return qt;
}

void CKLineDlg::ShowQuotLegend()
{
	if (!m_dlgQuotLegend.GetSafeHwnd())
	{
		m_dlgQuotLegend.Create(IDD_DLG_QUOT_LEGEND, this);
	}

	m_dlgQuotLegend.SetQuotation(GetCurrentQuot());

	m_dlgQuotLegend.ShowWindow(SW_SHOWNOACTIVATE);

}

void CKLineDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMenu   menu;

	VERIFY(menu.LoadMenu(IDR_MENU_LIST));

	CMenu* pPopup = menu.GetSubMenu(0);

	ASSERT(pPopup != NULL);

	DWORD dwPos = GetMessagePos();
	CPoint pt( LOWORD(dwPos), HIWORD(dwPos) );

	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, pt.x, pt.y, this);

	CDialog::OnRButtonDown(nFlags, point);
}

void CKLineDlg::OnCalMa1()
{
	// TODO: 在此添加命令处理程序代码
	AddStrategy("CrossMA");
}

void CKLineDlg::OnCalTrend1()
{
	// TODO: 在此添加命令处理程序代码
	AddStrategy("Trend");
}

void CKLineDlg::OnCalTrendcross()
{
	// TODO: 在此添加命令处理程序代码
	AddStrategy("TrendCrossMA");
}

void CKLineDlg::OnCalVolcross()
{
	// TODO: 在此添加命令处理程序代码
	AddStrategy("VolumeCross");
}

void CKLineDlg::OnMaAdd()
{
	// TODO: 在此添加命令处理程序代码
	DlgMAEditor dlg;
	dlg.DoModal();

	CalcMALines();

	ResetAllPosition();

	Refresh();
}

void CKLineDlg::DrawStrategies(CDC *pDC)
{
	vector<Strategy*>& strategies = m_strategycalculator.strategies();

	foreach(strategies, it, vector<Strategy*>)
	{
		Strategy* strategy = *it;

		if (strategy->name() == "CrossMA")
		{
			CrossMAStrategy* cms = (CrossMAStrategy*)strategy;

			//DrawAllMeanLines(pDC, GetValidRect(), cms->GetMAShortTerm()->cycle(), cms->GetMAShortTerm()->getMAItems(), MA_COLORS[0]);

			//DrawAllMeanLines(pDC, GetValidRect(), cms->GetMALongTerm()->cycle(), cms->GetMALongTerm()->getMAItems(), MA_COLORS[1]);
		}
		else if (strategy->name() == "Trend")
		{
		}
		else if (strategy->name() == "TrendCrossMA")
		{
		}
		else if (strategy->name() == "VolumeCross")
		{

		}
	}
}

void CKLineDlg::Refresh()
{
	DrawMemDC();

	InvalidateRect(0);

	UpdateWindow();
}

void CKLineDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return;
	//CDialog::OnClose();
}

void CKLineDlg::OnOK()
{
	return;
}

void CKLineDlg::OnCancel()
{
	return;
}

void CKLineDlg::DrawIndicatorXY(CDC* pDC, CRect& rc)
{
	CPen newPen;

	newPen.CreatePen(PS_SOLID, LINE_WIDTH, CR_FOREGROUND);

	CPen* pOldPen = pDC->SelectObject(&newPen);

	CBrush newBrush;
	newBrush.CreateSolidBrush(CR_BACKGROUND);

	CBrush* pOldBrush = pDC->SelectObject(&newBrush);

	pDC->Rectangle(&rc);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	CPen penLine;

	penLine.CreatePen(PS_DOT, 1, CR_FOREGROUND);

	pOldPen = pDC->SelectObject(&penLine);

	int nGap = rc.Height()/HORZ_LINE_NUM;

	for (int i = 1; i < HORZ_LINE_NUM; i++)
	{
		int nY = rc.bottom - nGap*i;

		// Draw X 
		pDC->MoveTo(rc.left, nY);
		pDC->LineTo(rc.right, nY);
	}

	pDC->SelectObject(pOldPen);
}

void CKLineDlg::DrawIndicator(CDC* pDC, CRect& rc)
{
	switch (m_currind)
	{
	case IND_MACD:
		DrawMACD(pDC, rc);
		break;
	case IND_BOLL:
		break;
	case IND_KDJ:
		break;
	case  IND_RSI:
		break;
	default:
		break;
	}
}

void CKLineDlg::OnIndBoll()
{
	// TODO: 在此添加命令处理程序代码
	ASSERT(m_product != NULL);
}

void CKLineDlg::OnIndMacd()
{
	// TODO: 在此添加命令处理程序代码
}

void CKLineDlg::OnIndKdj()
{
	// TODO: 在此添加命令处理程序代码
}

void CKLineDlg::OnIndRsi()
{
	// TODO: 在此添加命令处理程序代码
}

void CKLineDlg::OnCycleDay()
{
	// TODO: 在此添加命令处理程序代码
	if (LINE_DAY != m_currcycle)
	{
		m_currcycle = LINE_DAY;

		CalcCycle();

		CalcMALines();

		CalcStrategies();

		ResetAllPosition();

		CalcIndicator();

		Refresh();
	}
}

void CKLineDlg::OnCycleWeek()
{
	// TODO: 在此添加命令处理程序代码
	if (LINE_WEEK != m_currcycle)
	{
		m_currcycle = LINE_WEEK;

		CalcCycle();

		CalcMALines();

		CalcStrategies();

		ResetAllPosition();

		CalcIndicator();

		Refresh();
	}
}

void CKLineDlg::OnCycleMonth()
{
	// TODO: 在此添加命令处理程序代码
	if (LINE_MONTH != m_currcycle)
	{
		m_currcycle = LINE_MONTH;

		CalcCycle();

		CalcMALines();

		CalcStrategies();

		ResetAllPosition();

		CalcIndicator();

		Refresh();
	}
}

vector<Quot>& CKLineDlg::GetQuotations()
{
	return m_quotations;
}

void CKLineDlg::ResetAllPosition()
{
	vector<Quot>& klines = GetQuotations();

	SetScrollRange(SB_HORZ, 0, klines.size()-1, TRUE);

	SetPosition(klines.size() - GetSizePerPage());
}

void CKLineDlg::CalcMALines()
{
	// 清空所有均线
	foreach(m_malines, it, vector<MALine*>)
	{
		MALine* p = *it;
		delete p;
	}

	m_malines.clear();

	// 重新计算所有均线
	vector<int> malines;

	g_Configuration.GetMALines(malines);

	vector<Quot>& quots = GetQuotations();

	foreach(malines, it, vector<int>)
	{
		MALine* maline = new MALine(*it);//TrendMALine(*it, g_Configuration.getThreshold());

		foreach(quots, itQuot, vector<Quot>)
		{
			maline->Calculate(*itQuot);
		}

		m_malines.push_back(maline);
	}
}

void CKLineDlg::CalcStrategies()
{
	//m_strategycalculator.Calculate(m_quotations);

	DataHandler dh;
	m_strategycalculator.ReCalculate(m_quotations, dh);
}

void CKLineDlg::AddStrategy(const char* name)
{
	DataHandler dh;

	if (m_strategycalculator.IsExisting(name))
	{
		m_strategycalculator.RemoveStrategy(name);
	}
	else
	{
		m_strategycalculator.AddStrategy(name, dh);
	}

	CalcStrategies();

	Refresh();
}

void CKLineDlg::CalcCycle()
{
	m_quotations.clear();

	switch (m_currcycle)
	{
	case LINE_DAY:
		m_quotations = m_product->GetQuotations();
		break;

	case LINE_WEEK:
		{
			WeekLine wl(&m_quotations);

			vector<Quot>& vec = m_product->GetQuotations();

			foreach(vec, it, vector<Quot>)
			{
				wl.Calculate(*it);
			}
		}

		break;
	case LINE_MONTH:
		{
			MonthLine ml(&m_quotations);

			vector<Quot>& vec = m_product->GetQuotations();

			foreach(vec, it, vector<Quot>)
			{
				ml.Calculate(*it);
			}
		}

		break;
	default:
		break;
	}
}

void CKLineDlg::update(Observable* observable, void* arg)
{
	m_strategycalculator.ReCalculate(m_quotations, *(DataHandler*)arg);

	Refresh();
}

void CKLineDlg::DrawMACD(CDC* pDC, CRect& rc)
{
	CRect rcMACD = rc;

	rcMACD.bottom -= 5;
	rcMACD.top += 5;

	CPen newPen;

	newPen.CreatePen(PS_SOLID, LINE_WIDTH, CR_FOREGROUND);

	CPen* pOldPen = pDC->SelectObject(&newPen);

	CBrush newBrush;
	newBrush.CreateSolidBrush(CR_BACKGROUND);

	CBrush* pOldBrush = pDC->SelectObject(&newBrush);

	pDC->Rectangle(&rc);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	MACD* macd = (MACD*)m_indicator;
	vector<MACDItem>& items = macd->items();

	CString str;
	str.Format("MACD(%d,%d,%d)", macd->ShortTerm(), macd->LongTerm(), macd->DeaTerm());
	CRect rcText = rcMACD;
	rcText.left = rcMACD.left+2;
	rcText.top = rcMACD.top+2;

	pDC->SetTextColor(RGB(255,0,0));
	pDC->DrawText(str, &rcText, DT_LEFT|DT_TOP);

	BOOL bHasOriginalPointForDif = FALSE;
	BOOL bHasOriginalPointForDea = FALSE;

	// 画剩余的其它点
	int nEnd = (GetPosition() + GetSizePerPage() < items.size())?(GetPosition() + GetSizePerPage()):items.size();

	// 计算均线的最大最小值
	double highest = INVALID_VALUE, lowest = 9999;

	for (int n = GetPosition(); n < nEnd; n++)
	{
		highest = max(highest, max(items[n].dif, items[n].dea));
		highest = max(highest, items[n].macd);
		lowest = min(lowest, min(items[n].dif, items[n].dea));
		lowest = min(lowest, items[n].macd);
	}

	int i = 0;

	int nHeight = rcMACD.Height();

	int zeroY = rcMACD.bottom - (0 - lowest)*nHeight/(highest-lowest);

	CPen penLine;
	penLine.CreatePen(PS_DOT, 1, CR_FOREGROUND);

	pOldPen = pDC->SelectObject(&penLine);

	pDC->MoveTo(rcMACD.left, zeroY);
	pDC->LineTo(rcMACD.right, zeroY);

	pDC->SelectObject(pOldPen);

	// draw macd
	for (int n = GetPosition(); n < nEnd; n++)
	{
		double d = items[n].macd;

		CPen newPenMacd;
		newPenMacd.CreatePen(PS_SOLID, LINE_WIDTH, (d>0)?RGB(255, 0, 0):RGB(0, 255, 0));

		pOldPen = pDC->SelectObject(&newPenMacd);

		// 计算x和y坐标
		int x = rc.left + GetKWidth()*i + GetKWidth()/2 + 1;

		i++;

		// 计算y坐标
		int y = rcMACD.bottom - (items[n].macd - lowest)*nHeight/(highest-lowest);

		pDC->MoveTo(x, zeroY);	
		pDC->LineTo(x, y);

		pDC->SelectObject(pOldPen);
	}

	// draw dif
	CPen newPenDif;
	newPenDif.CreatePen(PS_SOLID, LINE_WIDTH, RGB(255, 255, 255));

	pOldPen = pDC->SelectObject(&newPenDif);

	i = 0;

	for (int n = GetPosition(); n < nEnd; n++)
	{
		// 计算x和y坐标
		int x = rc.left + GetKWidth()*i + GetKWidth()/2 + 1;

		i++;

		// 计算y坐标
		int yDif = rcMACD.bottom - (items[n].dif - lowest)*nHeight/(highest-lowest);

		// 计算起始点Y坐标
		if (!bHasOriginalPointForDif)
		{
			pDC->MoveTo(x, yDif);

			bHasOriginalPointForDif = TRUE;

			continue;
		}

		if (bHasOriginalPointForDif)
		{
			pDC->LineTo(x, yDif);
		}
	}

	pDC->SelectObject(pOldPen);

	//
	CPen newPenDea;
	newPenDea.CreatePen(PS_SOLID, LINE_WIDTH, RGB(255, 0, 0));

	pOldPen = pDC->SelectObject(&newPenDea);

	i = 0;

	//draw dea
	for (int n = GetPosition(); n < nEnd; n++)
	{
		// 计算x和y坐标
		int x = rc.left + GetKWidth()*i + GetKWidth()/2 + 1;

		i++;

		int yDea = rcMACD.bottom - (items[n].dea - lowest)*nHeight/(highest-lowest);

		// 计算起始点Y坐标
		if (!bHasOriginalPointForDea)
		{
			pDC->MoveTo(x, yDea);

			bHasOriginalPointForDea = TRUE;

			continue;
		}

		if (bHasOriginalPointForDea)
		{
			pDC->LineTo(x, yDea);

		}
	}

	pDC->SelectObject(pOldPen);
}

void CKLineDlg::CalcIndicator()
{
	if (m_indicator)
	{
		delete m_indicator;

		m_indicator = 0;
	}

	switch (m_currind)
	{
	case IND_MACD:
		m_indicator = new MACD(12, 26, 9);
		break;
	case IND_BOLL:
		break;
	case IND_KDJ:
		break;
	case IND_RSI:
		break;
	default:
		break;
	}


	//
	if (m_indicator)
	{
		foreach(m_quotations, it, vector<Quot>)
		{
			m_indicator->Calculate(*it);
		}
	}
}
void CKLineDlg::OnCalCrossmacd()
{
	// TODO: 在此添加命令处理程序代码
	AddStrategy("CrossMACD");
}

void CKLineDlg::OnUpdateCycleDay(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetRadio(m_currcycle == LINE_DAY);
}

void CKLineDlg::OnUpdateCycleWeek(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetRadio(m_currcycle == LINE_WEEK);
}

void CKLineDlg::OnUpdateCycleMonth(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetRadio(m_currcycle == LINE_MONTH);
}

void CKLineDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	__super::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: 在此处添加消息处理程序代码
	// TODO: 在此处添加消息处理程序代码
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child Windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

void CKLineDlg::OnUpdateCalMa1(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_strategycalculator.IsExisting("CrossMA")?TRUE:FALSE);
}

void CKLineDlg::OnUpdateCalCrossmacd(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_strategycalculator.IsExisting("CrossMACD")?TRUE:FALSE);
}

void CKLineDlg::OnUpdateCalTrend1(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_strategycalculator.IsExisting("Trend")?TRUE:FALSE);
}

void CKLineDlg::OnStrategyMa()
{
	// TODO: 在此添加命令处理程序代码
	AddStrategy("MA");
}

void CKLineDlg::OnUpdateStrategyMa(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_strategycalculator.IsExisting("MA")?TRUE:FALSE);
}

void CKLineDlg::OnStrategyNewhighlow()
{
	// TODO: 在此添加命令处理程序代码
	AddStrategy("NewHigh");
}

void CKLineDlg::OnUpdateStrategyNewhighlow(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_strategycalculator.IsExisting("NewHigh")?TRUE:FALSE);
}

void CKLineDlg::OnStrategyMacd()
{
	// TODO: 在此添加命令处理程序代码
	AddStrategy("MACD");
}

void CKLineDlg::OnUpdateStrategyMacd(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_strategycalculator.IsExisting("MACD")?TRUE:FALSE);
}

void CKLineDlg::OnUpdateCalTrendcross(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_strategycalculator.IsExisting("TrendCrossMA")?TRUE:FALSE);
}

void CKLineDlg::OnUpdateCalVolcross(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_strategycalculator.IsExisting("VolumeCross")?TRUE:FALSE);
}

void CKLineDlg::OnStrategyRebound()
{
	// TODO: 在此添加命令处理程序代码
	AddStrategy("Rebound");
}

void CKLineDlg::OnStrtegyOffset()
{
	// TODO: 在此添加命令处理程序代码
	AddStrategy("MAOffset");
}
