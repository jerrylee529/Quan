#pragma once

#include "FrameworkCls.h"

using namespace fmkcls;

// CDlgProgress 对话框

class CDlgProgress : public CDialog, public IProgress
{
	DECLARE_DYNAMIC(CDlgProgress)

public:
	CDlgProgress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProgress();

// 对话框数据
	enum { IDD = IDD_DLG_PROGRESS };

	virtual void UpdateProgress(int progress);

	virtual bool IsCanceled();

	virtual void Done();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();


	DECLARE_MESSAGE_MAP()

private:
	CProgressCtrl m_progressctrl;
	BOOL m_isCanceled;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();

	afx_msg LRESULT OnQAM_Progress_SetRange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQAM_Progress_SetPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQAM_Progress_Done(WPARAM wParam, LPARAM lParam);
};
