#pragma once


// CConfigDlg dialog

class CConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigDlg();

// Dialog Data
	enum { IDD = IDD_CONFIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

private:
	int m_meanCycle;
	int m_adjustCycle;
	int m_threshold;
	int m_stoploss;
	int m_safeMACycle;
	CString m_strHistPath;
	CString m_strInstPath;
	CString m_strSavePath;
	afx_msg void OnBnClickedButtonHistPath();
	afx_msg void OnBnClickedButtonInsPath();
	afx_msg void OnBnClickedButtonSavePath();
};
