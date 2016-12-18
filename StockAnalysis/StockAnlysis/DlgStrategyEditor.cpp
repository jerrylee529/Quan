// DlgStrategyEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "StockAnlysis.h"
#include "DlgStrategyEditor.h"


const string STRATEGYNAMES[] =
{
	"CrossMA",
	"CrossMACD",
	"MACD",
	"Trend",
	"TrendCrossMA",
	"VolumeCross",
	"MA",
	"NewHigh",
	"Rebound",
	"MAOffset"
};

// CDlgStrategyEditor 对话框

IMPLEMENT_DYNAMIC(CDlgStrategyEditor, CDialog)

CDlgStrategyEditor::CDlgStrategyEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStrategyEditor::IDD, pParent)
{

}

CDlgStrategyEditor::~CDlgStrategyEditor()
{
}

void CDlgStrategyEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_listbox);
	DDX_Control(pDX, IDC_COMBO_CYCLE, m_combobox);
	DDX_CBIndex(pDX, IDC_COMBO_CYCLE, m_cycle);
}


BEGIN_MESSAGE_MAP(CDlgStrategyEditor, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgStrategyEditor::OnLbnSelchangeList1)
	ON_CBN_SELCHANGE(IDC_COMBO_CYCLE, &CDlgStrategyEditor::OnCbnSelchangeComboCycle)
END_MESSAGE_MAP()


// CDlgStrategyEditor 消息处理程序
BOOL CDlgStrategyEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 初始化显示列表
	m_listbox.AddString("均线交叉策略");
	m_listbox.AddString("MACD交叉策略");
	m_listbox.AddString("顶底策略");
	m_listbox.AddString("趋势线策略");
	m_listbox.AddString("趋势均线交叉策略");
	m_listbox.AddString("成交量均线交叉策略");
	m_listbox.AddString("均线策略");
	m_listbox.AddString("新高新低策略");
	m_listbox.AddString("反弹策略");
	m_listbox.AddString("均线偏移策略");

	//
	m_combobox.AddString("日线");
	m_combobox.AddString("周线");
	m_combobox.AddString("月线");

	m_cycle = 0;
	m_combobox.SetCurSel(m_cycle);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgStrategyEditor::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strategies->clear();

	for (int i = 0; i < m_listbox.GetCount(); i++)
	{
		if (m_listbox.GetSel(i))
		{
			m_strategies->push_back(STRATEGYNAMES[i]);
		}
	}
}

void CDlgStrategyEditor::SetStrategies(vector<string>* strategies)
{
	m_strategies = strategies;
}

void CDlgStrategyEditor::OnCbnSelchangeComboCycle()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}
