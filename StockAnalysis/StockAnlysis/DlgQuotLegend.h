#pragma once

#include "StockDataStructures.h"

// CDlgQuotLegend 对话框

class CDlgQuotLegend : public CDialog
{
	DECLARE_DYNAMIC(CDlgQuotLegend)

public:
	CDlgQuotLegend(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgQuotLegend();

// 对话框数据
	enum { IDD = IDD_DLG_QUOT_LEGEND };

	void SetQuotation(const Quot& quot);

	void SetIndicator(const char* text);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	COLORREF GetTextColor(const int value1, const int value2);

	Quot m_quot;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CString m_indicatorText;
};
