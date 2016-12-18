// DlgStrategyResult.cpp : 实现文件
//

#include "stdafx.h"
#include "StockAnlysis.h"
#include "DlgStrategyResult.h"
#include "instrumentmanager.h"


// 定义列序号
const int ID_CODE = 0;   // 股票代码
const int ID_NAME = 1;   // 股票名称
const int ID_STRATEGY_ROI = 2; // 策略总收益率
const int ID_STRATEGY_MAXGAIN = 3; // 策略最大收益率
const int ID_STRATEGY_MAXLOSS = 4; // 策略最大回撤
const int ID_STRATEGY_NUM_OF_POSGAIN = 5; // 正收益次数
const int ID_STRATEGY_RATE_OF_POSGAIN = 6; // 正收益比例 
const int ID_STRATEGY_NUM_OF_BUY = 7; // 买入次数
const int ID_STRATEGY_NUM_OF_SELL = 8; // 卖出次数
const int ID_STATUS = 9;   // 策略状态

// CDlgStrategyResult 对话框

IMPLEMENT_DYNAMIC(CDlgStrategyResult, CDialog)

CDlgStrategyResult::CDlgStrategyResult(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStrategyResult::IDD, pParent)
{

}

CDlgStrategyResult::~CDlgStrategyResult()
{
}

void CDlgStrategyResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_RESULT, m_list);
}


BEGIN_MESSAGE_MAP(CDlgStrategyResult, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_RESULT, &CDlgStrategyResult::OnLvnColumnclickListResult)
END_MESSAGE_MAP()


// CDlgStrategyResult 消息处理程序

BOOL CDlgStrategyResult::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 初始化显示列表
	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_list.SetExtendedStyle(dwStyle); //设置扩展风格

	InitializeList();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgStrategyResult::InitializeList()
{
	m_list.DeleteAllItems();

	while (m_list.DeleteColumn(0)){}

	m_list.InsertColumn(ID_CODE, "股票代码", 0, 60);
	m_list.InsertColumn(ID_NAME, "股票名称", 0, 100);
	m_list.InsertColumn(ID_STRATEGY_ROI, "策略总收益率%", 0, 110);
	m_list.InsertColumn(ID_STRATEGY_MAXGAIN, "策略最大收益率%", 0, 110);
	m_list.InsertColumn(ID_STRATEGY_MAXLOSS, "策略最大回撤%", 0, 110);
	m_list.InsertColumn(ID_STRATEGY_NUM_OF_POSGAIN, "正收益次数", 0, 110);
	m_list.InsertColumn(ID_STRATEGY_RATE_OF_POSGAIN, "正收益比例%", 0, 110);
	m_list.InsertColumn(ID_STRATEGY_NUM_OF_BUY, "买入次数", 0, 110);
	m_list.InsertColumn(ID_STRATEGY_NUM_OF_SELL, "卖出次数", 0, 110);
	m_list.InsertColumn(ID_STATUS, "策略状态", 0, 200);
}

void CDlgStrategyResult::SetStrategyResults(const vector<StrategyResult>& results)
{
	m_list.DeleteAllItems();

	int i = 0;

	foreach(results, it, vector<StrategyResult>)
	{
		CString str;

		m_list.InsertItem(i, it->code_.c_str());

		m_list.SetItemText(i, ID_NAME, g_InstrumentMgr.GetInstrument(it->code_.c_str()).Name.c_str());

		str.Format("%.2f", it->GetROI());
		m_list.SetItemText(i, ID_STRATEGY_ROI, str);

		str.Format("%.2f", it->GetMaxGain());
		m_list.SetItemText(i, ID_STRATEGY_MAXGAIN, str);

		str.Format("%.2f", it->GetMaxLoss());
		m_list.SetItemText(i, ID_STRATEGY_MAXLOSS, str);

		str.Format("%d", it->GetNumOfPosGain());
		m_list.SetItemText(i, ID_STRATEGY_NUM_OF_POSGAIN, str);

		str.Format("%.2f", it->GetRateOfPosGain());
		m_list.SetItemText(i, ID_STRATEGY_RATE_OF_POSGAIN, str);

		str.Format("%d", it->GetNumOfBuy());
		m_list.SetItemText(i, ID_STRATEGY_NUM_OF_BUY, str);

		str.Format("%d", it->GetNumOfSell());
		m_list.SetItemText(i, ID_STRATEGY_NUM_OF_SELL, str);

		if (it->CanBuy())
		{
			str = "买入";
		}
		else if (it->CanSell())
		{
			str = "卖出";
		}
		else if (it->CanHold())
		{
			str = "持有";
		}
		else
		{
			str = "空仓";
		}

		m_list.SetItemText(i, ID_STATUS, str);

		i++;
	}
}
void CDlgStrategyResult::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_list.GetSafeHwnd())
	{
		m_list.MoveWindow(5, 5, cx-10, cy-10);
	}
}

struct COMPAREDATA  
{  
	CListCtrl *plist;//用于存储列表控件的指针  
	int col;//用于存储要排序主列的序号  
};//回调函数第三个参数对应的数据结构，可以自定义，至少要包含这两项

BOOL bSort = FALSE;

int CALLBACK MylistCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	COMPAREDATA * MyData = (COMPAREDATA*)lParamSort;  

	int col = MyData->col;//点击的列项传递给col,用来判断点击了第几列 

	//取项的字符串  
	CString strItem1, strItem2;  
	strItem1 = MyData->plist->GetItemText(lParam1,col);  
	strItem2 = MyData->plist->GetItemText(lParam2,col);//获得要比较的字符	

	if (bSort)
	{
		return strItem1.CompareNoCase(strItem2);
	}
	else
	{
		return strItem2.CompareNoCase(strItem1);
	}
}

void CDlgStrategyResult::OnLvnColumnclickListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	// TODO: 在此添加控件通知处理程序代码

	COMPAREDATA data;  
	data.col = pNMLV->iSubItem;//取列  
	data.plist = &m_list;//取列表指针 

	bSort=!bSort;//排序每点一次列就变一次，若想固定排序，那就去掉这局  

	//设置列表相关项，以便排序  
	for(int i=0; i<m_list.GetItemCount(); i++)  
	{  
		m_list.SetItemData(i, i);  
	}  

	m_list.SortItems(MylistCompare,(LPARAM)&data);  

	*pResult = 0;
}
