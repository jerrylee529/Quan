// StockAnlysisDlg.h : header file
//

#pragma once

#include "DlgProgress.h"
#include "KLineDlg.h"


// CStockAnlysisDlg dialog
class CStockAnlysisDlg : public CDialog
{
// Construction
public:
	CStockAnlysisDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_STOCKANLYSIS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

private:
	void InitializeList();

	void LoadStockProducts();

    void CalculateSelected(const char* strategyName);

	int GetCurrentSector();

	void ViewKLineWindow(const char* code);

	void CalculateStrategy(const char* strategyname);

	CComboBox m_ccbStockProduct;
	CComboBox m_ccbStrategy;
	CEdit m_edtDownloadFile;
	CEdit m_edtGain;
	CListCtrl m_listCtrl;

	CString m_strCode;
	CKLineDlg* m_dlgKline;
	CDlgProgress* m_dlgProgress;

	CButton m_btnStart;

	map<string, int> m_list;// 保存股票代码与listctrl索引的对应关系

	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnCalMa1();
	afx_msg void OnCalMa2();
	afx_msg void OnCalTrend1();
	afx_msg void OnViewKline();
	afx_msg void OnCalMa1Auto();
	afx_msg void OnMenuExport();
	afx_msg void OnDestroy();
	afx_msg void OnCalAll();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCalAllCrossma();
	afx_msg void OnCrossmaResult();
	afx_msg void OnMmCalCrossma();
	afx_msg void OnMmQuit();
	afx_msg void OnMmRecvquot();
	afx_msg void OnMmCalTrend();
	afx_msg void OnMmStopQuot();
	afx_msg void OnMmWndAnalysis();
	afx_msg void OnStrategyRunCrossma();
	afx_msg void OnStrategyRunTrend();
	afx_msg LRESULT OnQAM_QUOTATION_LATEST(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQAM_PRODUCT_TRADE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQAM_Progress_SetRange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQAM_Progress_SetPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQAM_Progress_Done(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateMmRecvquot(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMmStopQuot(CCmdUI *pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnMmConfig();
	afx_msg void OnUpdateMmWndAnalysis(CCmdUI *pCmdUI);
	afx_msg void OnMmAutocalAll();
	afx_msg void OnMmAutocalCurrent();
	afx_msg void OnMmSave();
	afx_msg void OnMmLoad();
	afx_msg void OnCalTrendcross();
	afx_msg void OnCalVolcross();
	afx_msg void OnMaAdd();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnStrategyStat();
	afx_msg void OnMmCalStrCurrent();
	afx_msg void OnMmCalStrChosen();
	afx_msg void OnMmCalStrAll();
	afx_msg void OnStrtegyOffset();
};
