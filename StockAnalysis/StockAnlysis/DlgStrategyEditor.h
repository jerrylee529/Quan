#pragma once

#include <vector>
#include <string>

using namespace std;

// CDlgStrategyEditor 对话框

class CDlgStrategyEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgStrategyEditor)

public:
	CDlgStrategyEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStrategyEditor();

// 对话框数据
	enum { IDD = IDD_DLG_STRATEGY };

	void SetStrategies(vector<string>* strategies);

	int GetCycle(){ return m_cycle; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CListBox m_listbox;
	CComboBox m_combobox;
	int m_cycle;
	vector<string>* m_strategies;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnCbnSelchangeComboCycle();
};
