#pragma once

#include "Strategy.h"


// CDlgStrategyResult 对话框

class CDlgStrategyResult : public CDialog
{
	DECLARE_DYNAMIC(CDlgStrategyResult)

public:
	CDlgStrategyResult(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStrategyResult();

// 对话框数据
	enum { IDD = IDD_DIALOG_RESULT };

	void SetStrategyResults(const vector<StrategyResult>& results);

protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void InitializeList();

	CListCtrl m_list;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnColumnclickListResult(NMHDR *pNMHDR, LRESULT *pResult);
};
