#pragma once

#include <string>
#include <map>
#include <vector>
#include "StockDataStructures.h"
#include "Product.h"
#include "DlgQuotLegend.h"
#include "strategycalculator.h"
#include "indicator.h"

using namespace std;

struct MeanLine
{
	int cycle;
	vector<int>* meanLine;
	COLORREF color;
};

// CKLineDlg dialog

class CKLineDlg : public CDialog, public Observer
{
	DECLARE_DYNAMIC(CKLineDlg)

public:
	CKLineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CKLineDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_KLINE };

	void SetProduct(Product* product);

	// 设置K线开始位置
	void SetPosition(const int value);

	// 得到每页显示的K线数量
	int GetSizePerPage();

	void SetKLineVisible(BOOL bVisible);

	//
	void Refresh();

	//
	virtual void update(Observable* observable, void* arg);	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

private:

	enum E_CYCLE{ LINE_DAY, LINE_WEEK, LINE_MONTH} ;

	enum E_INDICATOR { IND_BOLL, IND_MACD, IND_KDJ, IND_RSI};

	void DrawAllKLines(CDC* pDC, CRect& rc);

	void DrawAllMeanLines(CDC* pDC, CRect& rc, int nCycle, vector<int>& meanLine, COLORREF crColor);

	void DrawOneLine(CDC* pDC, CRect& rc, int x, COLORREF crLine, Quot& sti, BOOL turnpoint);

	void DrawMemDC();

	void DrawXYAxis(CDC* pDC, CRect& rc);

	void DrawVolumeXY(CDC* pDC, CRect& rc);

	void DrawVolume(CDC* pDC, CRect& rc);

	void DrawLegend(CDC* pDC, CRect& rc);

	void DrawStrategies(CDC *pDC);

	void DrawIndicator(CDC *pDC, CRect& rc);

	void DrawIndicatorXY(CDC *pDC, CRect& rc);

	void DrawMACD(CDC* pDC, CRect& rc);

	CRect GetValidRect();

	CRect GetMiddleRect();

	CRect GetBottomRect();

	CRect GetLegendRect();

	int GetPosition();

	int GetKWidth();

	void CalculateMaxMin();

	Quot GetCurrentQuot();

	void ShowQuotLegend();

	vector<Quot>& GetQuotations();

	void CalcCycle();

	void CalcMALines();

	void CalcIndicator();

	void ResetAllPosition();

	void CalcStrategies();

	void AddStrategy(const char* name);

	int m_position;

	int m_lowest; // 最高价

	int m_highest; // 最低价

	double m_highVol; // 最大成交量

	double m_lowVol;  // 最小成交量

	int m_scale; // 放大倍数 

	Product* m_product;

	CDC MemDC; //首先定义一个显示设备对象  
	CBitmap MemBitmap;//定义一个位图对象
    CBitmap *pOldBit;

	BOOL m_bKLineVisible;

	CPoint m_ptPrevious;

	BOOL m_bShowCross;

	CDlgQuotLegend m_dlgQuotLegend;

	E_CYCLE m_currcycle;

	E_INDICATOR m_currind;

	vector<Quot> m_quotations;

	vector<MALine*> m_malines;

	Indicator* m_indicator;

	StrategyCalculator m_strategycalculator;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCalMa1();
	afx_msg void OnCalTrend1();
	afx_msg void OnCalTrendcross();
	afx_msg void OnCalVolcross();
	afx_msg void OnMaAdd();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnIndBoll();
	afx_msg void OnIndMacd();
	afx_msg void OnIndKdj();
	afx_msg void OnIndRsi();
	afx_msg void OnCycleDay();
	afx_msg void OnCycleWeek();
	afx_msg void OnCycleMonth();
	afx_msg void OnCalCrossmacd();
	afx_msg void OnUpdateCycleDay(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCycleWeek(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCycleMonth(CCmdUI *pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnUpdateCalMa1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCalCrossmacd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCalTrend1(CCmdUI *pCmdUI);
	afx_msg void OnStrategyMa();
	afx_msg void OnUpdateStrategyMa(CCmdUI *pCmdUI);
	afx_msg void OnStrategyNewhighlow();
	afx_msg void OnUpdateStrategyNewhighlow(CCmdUI *pCmdUI);
	afx_msg void OnStrategyMacd();
	afx_msg void OnUpdateStrategyMacd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCalTrendcross(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCalVolcross(CCmdUI *pCmdUI);
	afx_msg void OnStrategyRebound();
	afx_msg void OnStrtegyOffset();
};
