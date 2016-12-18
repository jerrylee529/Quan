// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StockAnlysis.h"
#include "ConfigDlg.h"
#include "Configuration.h"
#include "Utilities.h"

// CConfigDlg dialog

IMPLEMENT_DYNAMIC(CConfigDlg, CDialog)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, m_meanCycle);

	DDX_Text(pDX, IDC_EDIT2, m_adjustCycle);

	DDX_Text(pDX, IDC_EDIT3, m_threshold);

	DDX_Text(pDX, IDC_EDIT4, m_stoploss);

	DDX_Text(pDX, IDC_EDIT5, m_safeMACycle);

	DDX_Text(pDX, IDC_EDIT_HIST_PATH, m_strHistPath);

	DDX_Text(pDX, IDC_EDIT_INS_PATH, m_strInstPath);

	DDX_Text(pDX, IDC_EDIT_SAVE_PATH, m_strSavePath);
}

BOOL CConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	Configuration& config = Singleton<Configuration>::getInstance();
	m_meanCycle = config.getMeanCycle();
	m_adjustCycle = config.getAdjustCycle();
	m_threshold = config.getThreshold();
	m_stoploss = config.getStopLoss();
	m_safeMACycle = config.safeMA_cycle();
	m_strHistPath = config.getHitoryDataStorePath().c_str();
	m_strInstPath = config.GetInstrumentFilePath().c_str();
	m_strSavePath = config.GetSaveFilePath().c_str();

	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_HIST_PATH, &CConfigDlg::OnBnClickedButtonHistPath)
	ON_BN_CLICKED(IDC_BUTTON_INS_PATH, &CConfigDlg::OnBnClickedButtonInsPath)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PATH, &CConfigDlg::OnBnClickedButtonSavePath)
END_MESSAGE_MAP()


// CConfigDlg message handlers

void CConfigDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	g_Configuration.setMeanCycle(m_meanCycle);

	g_Configuration.setAdjustCycle(m_adjustCycle);

	g_Configuration.setThreshold(m_threshold);

	g_Configuration.setStopLoss(m_stoploss);

	g_Configuration.setsafeMA_cycle(m_safeMACycle);

	g_Configuration.SetHitoryDataStorePath(m_strHistPath);

	g_Configuration.SetInstrumentFilePath(m_strInstPath);

	g_Configuration.SetSaveFilePath(m_strSavePath);

	g_Configuration.Save();

	OnOK();
}

void CConfigDlg::OnBnClickedButtonHistPath()
{
	// TODO: 在此添加控件通知处理程序代码

	TCHAR pszPath[MAX_PATH];
	BROWSEINFO bi; 
	bi.hwndOwner      = this->GetSafeHwnd();
	bi.pidlRoot       = NULL;
	bi.pszDisplayName = NULL; 
	bi.lpszTitle      = TEXT("请选择历史数据文件路径"); 
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn           = NULL; 
	bi.lParam         = 0;
	bi.iImage         = 0; 

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL)
	{
		return;
	}

	if (SHGetPathFromIDList(pidl, pszPath))
	{
		m_strHistPath = pszPath;

		UpdateData(FALSE);
	}
}

void CConfigDlg::OnBnClickedButtonInsPath()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR pszPath[MAX_PATH];
	BROWSEINFO bi; 
	bi.hwndOwner      = this->GetSafeHwnd();
	bi.pidlRoot       = NULL;
	bi.pszDisplayName = NULL; 
	bi.lpszTitle      = TEXT("请选择股票代码文件路径"); 
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn           = NULL; 
	bi.lParam         = 0;
	bi.iImage         = 0; 

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL)
	{
		return;
	}

	if (SHGetPathFromIDList(pidl, pszPath))
	{
		m_strInstPath = pszPath;

		UpdateData(FALSE);
	}
}

void CConfigDlg::OnBnClickedButtonSavePath()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR pszPath[MAX_PATH];
	BROWSEINFO bi; 
	bi.hwndOwner      = this->GetSafeHwnd();
	bi.pidlRoot       = NULL;
	bi.pszDisplayName = NULL; 
	bi.lpszTitle      = TEXT("请选择文件保存路径"); 
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn           = NULL; 
	bi.lParam         = 0;
	bi.iImage         = 0; 

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL)
	{
		return;
	}

	if (SHGetPathFromIDList(pidl, pszPath))
	{
		m_strSavePath = pszPath;

		UpdateData(FALSE);
	}
}
