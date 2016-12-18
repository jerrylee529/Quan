#pragma once

#include "Product.h"

// DlgMAEditor 对话框

class DlgMAEditor : public CDialog
{
	DECLARE_DYNAMIC(DlgMAEditor)

public:
	DlgMAEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgMAEditor();

// 对话框数据
	enum { IDD = IDD_DIALOG_MA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListMa();

private:
	void LoadMALines();

	CButton m_btnDel;
	CButton m_btnAdd;
	CListBox m_listbox;
	int m_cycle;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
};
