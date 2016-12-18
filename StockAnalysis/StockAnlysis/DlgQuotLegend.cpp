// DlgQuotLegend.cpp : 实现文件
//

#include "stdafx.h"
#include "StockAnlysis.h"
#include "DlgQuotLegend.h"

const int MARGIN_TOP	= 20;
const int MARGIN_LEFT	= 20;
const int MARGIN_RIGHT	= 30;
const int MARGIN_BOTTOM = 30;
const int TITLE_WIDTH	= 60;
const int TEXT_WIDTH	= 80;
const int TEXT_HEIGHT	= 30;
const int MARGIN_TEXT  = 5;

const COLORREF CR_BACKGROUND = RGB(0, 0, 0);
const COLORREF CR_FOREGROUND = RGB(255, 255, 255);

// CDlgQuotLegend 对话框

IMPLEMENT_DYNAMIC(CDlgQuotLegend, CDialog)

CDlgQuotLegend::CDlgQuotLegend(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQuotLegend::IDD, pParent)
{

}

CDlgQuotLegend::~CDlgQuotLegend()
{
}

void CDlgQuotLegend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgQuotLegend, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgQuotLegend 消息处理程序

void CDlgQuotLegend::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	CRect rc;
	GetClientRect(&rc);

	CBrush newBrush;
	newBrush.CreateSolidBrush(CR_BACKGROUND);

	CBrush* pOldBrush = dc.SelectObject(&newBrush);

	dc.Rectangle(&rc);

	dc.SelectObject(pOldBrush);

	CString strText;

	dc.SetBkMode(TRANSPARENT);

	dc.SetTextColor(CR_FOREGROUND);

	CRect rcDate(rc.left + MARGIN_LEFT, rc.top + MARGIN_TOP, rc.left + MARGIN_LEFT + TITLE_WIDTH, 
		rc.top + MARGIN_TOP + TEXT_HEIGHT);

	dc.DrawText("日期: ", &rcDate, DT_RIGHT|DT_VCENTER);

	CRect rcOpen(rc.left + MARGIN_LEFT, rcDate.bottom + MARGIN_TEXT, rc.left + MARGIN_LEFT + TITLE_WIDTH, 
		rcDate.bottom + MARGIN_TEXT + TEXT_HEIGHT);

	dc.DrawText("开盘价: ", &rcOpen, DT_RIGHT|DT_VCENTER);

	CRect rcHigh(rc.left + MARGIN_LEFT, rcOpen.bottom + MARGIN_TEXT, rc.left + MARGIN_LEFT + TITLE_WIDTH, 
		rcOpen.bottom + MARGIN_TEXT + TEXT_HEIGHT);
	dc.DrawText("最高价: ", &rcHigh, DT_RIGHT|DT_VCENTER);
	
	CRect rcLow(rc.left + MARGIN_LEFT, rcHigh.bottom + MARGIN_TEXT, rc.left + MARGIN_LEFT + TITLE_WIDTH, 
		rcHigh.bottom + MARGIN_TEXT + TEXT_HEIGHT);
	dc.DrawText("最低价: ", &rcLow, DT_RIGHT|DT_VCENTER);
	
	CRect rcClose(rc.left + MARGIN_LEFT, rcLow.bottom + MARGIN_TEXT, rc.left + MARGIN_LEFT + TITLE_WIDTH, 
		rcLow.bottom + MARGIN_TEXT + TEXT_HEIGHT);
	dc.DrawText("收盘价: ", &rcClose, DT_RIGHT|DT_VCENTER);
	
	CRect rcVolume(rc.left + MARGIN_LEFT, rcClose.bottom + MARGIN_TEXT, rc.left + MARGIN_LEFT + TITLE_WIDTH, 
		rcClose.bottom + MARGIN_TEXT + TEXT_HEIGHT);
	dc.DrawText("成交量: ", &rcVolume, DT_RIGHT|DT_VCENTER);

	CRect rcUpRate(rc.left + MARGIN_LEFT, rcVolume.bottom + MARGIN_TEXT, rc.left + MARGIN_LEFT + TITLE_WIDTH, 
		rcVolume.bottom + MARGIN_TEXT + TEXT_HEIGHT);
	dc.DrawText("涨幅: ", &rcUpRate, DT_RIGHT|DT_VCENTER);

	// 日期
	strText.Format("%s", m_quot.getDatetime().c_str());
	rcDate.left = rcDate.right + 5;
	rcDate.right = rcDate.left + 150;
	dc.SetTextColor(RGB(0, 0, 255));
	dc.DrawText(strText, rcDate, DT_LEFT|DT_VCENTER);

	// 开盘价
	strText.Format("%.2f", m_quot.getOpen());
	rcOpen.left = rcOpen.right + 5;
	rcOpen.right = rcOpen.left + TEXT_WIDTH;
	dc.SetTextColor(RGB(0, 0, 255));
	dc.DrawText(strText, rcOpen, DT_LEFT|DT_VCENTER);

	// 最高价 
	strText.Format("%.2f", m_quot.getHigh());
	rcHigh.left = rcHigh.right + 5;
	rcHigh.right = rcHigh.left + TEXT_WIDTH;
	dc.SetTextColor(GetTextColor(m_quot.High, m_quot.Open));
	dc.DrawText(strText, rcHigh, DT_LEFT|DT_VCENTER);

	// 最低价 
	strText.Format("%.2f", m_quot.getLow());
	rcLow.left = rcLow.right + 5;
	rcLow.right = rcLow.left + TEXT_WIDTH;
	dc.SetTextColor(GetTextColor(m_quot.Low, m_quot.Open));
	dc.DrawText(strText, rcLow, DT_LEFT|DT_VCENTER);

	// 收盘价
	strText.Format("%.2f", m_quot.getClose());
	rcClose.left = rcClose.right + 5;
	rcClose.right = rcClose.left + TEXT_WIDTH;
	dc.SetTextColor(GetTextColor(m_quot.Close, m_quot.Open));
	dc.DrawText(strText, rcClose, DT_LEFT|DT_VCENTER);

	// 成交量
	strText.Format("%.0f", m_quot.getVolume());
	rcVolume.left = rcVolume.right + 5;
	rcVolume.right = rcVolume.left + TEXT_WIDTH;
	dc.SetTextColor(RGB(0, 0, 255));
	dc.DrawText(strText, rcVolume, DT_LEFT|DT_VCENTER);

	// 涨跌幅
	double upRate = (double)(m_quot.Close-m_quot.Open)*100/m_quot.Close;
	strText.Format("%.2f%%", upRate);
	rcUpRate.left = rcUpRate.right + 5;
	rcUpRate.right = rcUpRate.left + TEXT_WIDTH;
	dc.SetTextColor(upRate>0?RGB(255, 0, 0):RGB(0, 255, 0));
	dc.DrawText(strText, rcUpRate, DT_LEFT|DT_VCENTER);

	// 涨跌幅
	CRect rcIndicator;
	rcIndicator.left = rc.left;
	rcIndicator.right = rc.left + TEXT_WIDTH;
	rcIndicator.bottom = rc.bottom;
	rcIndicator.top = rcUpRate.bottom + 5;
	dc.SetTextColor(RGB(255, 0, 0));
	dc.DrawText(m_indicatorText, rcIndicator, DT_LEFT|DT_VCENTER);
}

void CDlgQuotLegend::SetQuotation(const Quot& quot)
{
	m_quot = quot;

	InvalidateRect(0);

	UpdateWindow();
}

void CDlgQuotLegend::SetIndicator(const char* text)
{
	m_indicatorText = text;
}

COLORREF CDlgQuotLegend::GetTextColor(const int value1, const int value2)
{
	if (value1 > value2)
	{
		return RGB(255, 0, 0);
	}
	else if (value1 < value2)
	{
		return RGB(0, 255, 0);
	}
	else
	{
		return RGB(0, 0, 255);
	}
}
BOOL CDlgQuotLegend::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;//CDialog::OnEraseBkgnd(pDC);
}
