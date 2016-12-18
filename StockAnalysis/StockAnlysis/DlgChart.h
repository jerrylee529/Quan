#pragma once

#include "tchart.h"
#include "seriestextsource.h"
#include "teecommander.h"
#include "Product.h"

// CDlgChart 对话框

class CDlgChart : public CDialog
{
	DECLARE_DYNAMIC(CDlgChart)

public:
	CDlgChart(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgChart();

// 对话框数据
	enum { IDD = IDD_DLG_CHART };

	void SetProduct(Product* product);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL CDlgChart::OnInitDialog();


	DECLARE_MESSAGE_MAP()

	CTChart	m_chart;

	Product* m_product;
};
