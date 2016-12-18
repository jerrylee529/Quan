// StockAnlysisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StockAnlysis.h"
#include "StockAnlysisDlg.h"
#include "Utilities.h"
#include "StockAnalyst.h"
#include "Configuration.h"
#include "ConfigDlg.h"
#include "HistDataLoaderFactory.h"
#include "Break20WeekKLine.h"
#include "TheLargestGain.h"
#include "NewHighPrice.h"
#include "CrossMALine.h"
#include "BOLLLine.h"
#include "TrendMALine.h"
#include "Product.h"
#include "TrendStrategy.h"
#include "QuanAnalyzer.h"
#include "MAStrategy.h"
#include "CrossMAStrategy.h"
#include "FrameworkCls.h"
#include <fstream>
#include "qa_msgno.h"
#include "DlgMAEditor.h"
#include "strategycalculator.h"
#include "DlgStrategyResult.h"
#include "DlgStrategyEditor.h"
#include "calculatestrategythread.h"
#include "instrumentmanager.h"
#include "statresultfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace fmkcls;


// 定义列序号
const int ID_CODE = 0;   // 股票代码
const int ID_NAME = 1;   // 股票名称
const int ID_LATEST = 2; // 最新价
const int ID_CHANGE = 3; // 股价涨跌幅
const int ID_STRATEGY = 4; // 运行策略
const int ID_STRATEGY_ROI = 5; // 策略总收益率
const int ID_STRATEGY_MAXGAIN = 6; // 策略最大收益率
const int ID_STRATEGY_MAXLOSS = 7; // 策略最大回撤
const int ID_STRATEGY_NUM_OF_POSGAIN = 8; // 正收益次数
const int ID_STRATEGY_RATE_OF_POSGAIN = 9; // 正收益比例 
const int ID_STRATEGY_NUM_OF_BUY = 10; // 买入次数
const int ID_STRATEGY_NUM_OF_SELL = 11; // 卖出次数
const int ID_STATUS = 12;   // 策略状态


const COLORREF MA_COLORS[] = {RGB(255,128,255), RGB(255,0,128)}; 

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CStockAnlysisDlg dialog




CStockAnlysisDlg::CStockAnlysisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStockAnlysisDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dlgKline = NULL;
}

void CStockAnlysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_listCtrl);

	DDX_Text(pDX, IDC_EDIT_CODE, m_strCode);
}

BEGIN_MESSAGE_MAP(CStockAnlysisDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CStockAnlysisDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStockAnlysisDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CStockAnlysisDlg::OnNMDblclkList1)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CStockAnlysisDlg::OnNMRclickList1)
	ON_BN_CLICKED(IDC_RADIO1, &CStockAnlysisDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CStockAnlysisDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CStockAnlysisDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO4, &CStockAnlysisDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO6, &CStockAnlysisDlg::OnBnClickedRadio1)
	ON_COMMAND(ID_CAL_MA1, &CStockAnlysisDlg::OnCalMa1)
	ON_COMMAND(ID_CAL_MA2, &CStockAnlysisDlg::OnCalMa2)
	ON_COMMAND(ID_CAL_TREND1, &CStockAnlysisDlg::OnCalTrend1)
	ON_COMMAND(ID_VIEW_KLINE, &CStockAnlysisDlg::OnViewKline)
	ON_COMMAND(ID_CAL_MA1_AUTO, &CStockAnlysisDlg::OnCalMa1Auto)
	ON_COMMAND(ID_MENU_EXPORT, &CStockAnlysisDlg::OnMenuExport)
	ON_BN_CLICKED(IDC_RADIO5, &CStockAnlysisDlg::OnBnClickedRadio1)
	ON_WM_DESTROY()
	ON_COMMAND(ID_CAL_ALL, &CStockAnlysisDlg::OnCalAll)
	ON_BN_CLICKED(IDC_BUTTON2, &CStockAnlysisDlg::OnBnClickedButton2)
	ON_COMMAND(ID_CAL_ALL_CROSSMA, &CStockAnlysisDlg::OnCalAllCrossma)
	ON_COMMAND(ID_CROSSMA_RESULT, &CStockAnlysisDlg::OnCrossmaResult)
	ON_COMMAND(ID_MM_CAL_CROSSMA, &CStockAnlysisDlg::OnMmCalCrossma)
	ON_COMMAND(ID_MM_QUIT, &CStockAnlysisDlg::OnMmQuit)
	ON_COMMAND(ID_MM_RECVQUOT, &CStockAnlysisDlg::OnMmRecvquot)
	ON_COMMAND(ID_MM_CAL_TREND, &CStockAnlysisDlg::OnMmCalTrend)
	ON_COMMAND(ID_MM_STOP_QUOT, &CStockAnlysisDlg::OnMmStopQuot)
	ON_COMMAND(ID_MM_WND_ANALYSIS, &CStockAnlysisDlg::OnMmWndAnalysis)
	ON_COMMAND(ID_STRATEGY_RUN_CROSSMA, &CStockAnlysisDlg::OnStrategyRunCrossma)
	ON_COMMAND(ID_STRATEGY_RUN_TREND, &CStockAnlysisDlg::OnStrategyRunTrend)
	ON_MESSAGE(QAM_QUOTATION_LATEST, OnQAM_QUOTATION_LATEST)
	ON_MESSAGE(QAM_PRODUCT_TRADE, OnQAM_PRODUCT_TRADE)
	ON_MESSAGE(QAM_PROGRESS_SETRANGE, OnQAM_Progress_SetRange)
	ON_MESSAGE(QAM_PROGRESS_SETPOS, OnQAM_Progress_SetPos)
	ON_MESSAGE(QAM_PROGRESS_DONE, OnQAM_Progress_Done)
	ON_UPDATE_COMMAND_UI(ID_MM_RECVQUOT, &CStockAnlysisDlg::OnUpdateMmRecvquot)
	ON_UPDATE_COMMAND_UI(ID_MM_STOP_QUOT, &CStockAnlysisDlg::OnUpdateMmStopQuot)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_MM_CONFIG, &CStockAnlysisDlg::OnMmConfig)
	ON_UPDATE_COMMAND_UI(ID_MM_WND_ANALYSIS, &CStockAnlysisDlg::OnUpdateMmWndAnalysis)
	ON_COMMAND(ID_MM_AUTOCAL_ALL, &CStockAnlysisDlg::OnMmAutocalAll)
	ON_COMMAND(ID_MM_AUTOCAL_CURRENT, &CStockAnlysisDlg::OnMmAutocalCurrent)
	ON_COMMAND(ID_MM_SAVE, &CStockAnlysisDlg::OnMmSave)
	ON_COMMAND(ID_MM_LOAD, &CStockAnlysisDlg::OnMmLoad)
	ON_COMMAND(ID_CAL_TRENDCROSS, &CStockAnlysisDlg::OnCalTrendcross)
	ON_COMMAND(ID_CAL_VOLCROSS, &CStockAnlysisDlg::OnCalVolcross)
	ON_COMMAND(ID_MA_ADD, &CStockAnlysisDlg::OnMaAdd)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CStockAnlysisDlg::OnBnClickedBtnSearch)
	ON_COMMAND(ID_STRATEGY_STAT, &CStockAnlysisDlg::OnStrategyStat)
	ON_COMMAND(ID_MM_CAL_STR_CURRENT, &CStockAnlysisDlg::OnMmCalStrCurrent)
	ON_COMMAND(D_MM_CAL_STR_CHOSEN, &CStockAnlysisDlg::OnMmCalStrChosen)
	ON_COMMAND(D_MM_CAL_STR_ALL, &CStockAnlysisDlg::OnMmCalStrAll)
	ON_COMMAND(ID_STRTEGY_OFFSET, &CStockAnlysisDlg::OnStrtegyOffset)
END_MESSAGE_MAP()


// CStockAnlysisDlg message handlers

BOOL CStockAnlysisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_dlgProgress = new CDlgProgress();
	m_dlgProgress->Create(IDD_DLG_PROGRESS, this);

	// 初始化显示列表
	LONG lStyle;
	lStyle = GetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_listCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_listCtrl.SetExtendedStyle(dwStyle); //设置扩展风格

	InitializeList();

	// 初始化市场
	CheckRadioButton(IDC_RADIO1, IDC_RADIO6, IDC_RADIO1);

	// 初始化
	g_QuanAnalyer.Init(this->GetSafeHwnd());
    
	// 载入品种
	LoadStockProducts();

	m_dlgKline = new CKLineDlg(this);

	int n = m_listCtrl.GetSelectionMark();

	if (n >= 0)
	{
		int code = (int)m_listCtrl.GetItemData(n);

		CString strCode;

		strCode.Format("%06d", code);

		Product* product = g_QuanAnalyer.GetProduct(strCode);

		m_dlgKline->SetProduct(product);

		m_dlgKline->Create(IDD_DIALOG_KLINE, this);

		m_dlgKline->ShowWindow(SW_NORMAL);
	}

	//
	m_strCode = "";

	UpdateData(FALSE);

	ShowWindow(SW_SHOWMAXIMIZED);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStockAnlysisDlg::OnSysCommand(UINT nID, LPARAM lParam)
{ 
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStockAnlysisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStockAnlysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CStockAnlysisDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}

void CStockAnlysisDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//
    //OnCancel();    
}

void CStockAnlysisDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if (m_listCtrl.GetSafeHwnd() != NULL)
	{
		m_listCtrl.MoveWindow(10, 70, 300, cy - 70);
	}

	if (m_dlgKline && m_dlgKline->GetSafeHwnd() != NULL)
	{
		m_dlgKline->MoveWindow(310, 10, cx - 320, cy - 10);
	}
}

void CStockAnlysisDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	OnViewKline();

	//
	*pResult = 0;
}

BOOL CStockAnlysisDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnEraseBkgnd(pDC);
}

void CStockAnlysisDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (MessageBox("确认退出程序", "宽客分析", MB_YESNO|MB_ICONWARNING) == IDYES)
	{
		m_dlgKline->DestroyWindow();
		delete m_dlgKline;

		CDialog::OnOK();
	}
}

void CStockAnlysisDlg::InitializeList()
{
	m_listCtrl.DeleteAllItems();

	while (m_listCtrl.DeleteColumn(0)){}

	m_listCtrl.InsertColumn(ID_CODE, "股票代码", 0, 60);
	m_listCtrl.InsertColumn(ID_NAME, "股票名称", 0, 100);
	m_listCtrl.InsertColumn(ID_LATEST, "最新价", 0, 100);
	m_listCtrl.InsertColumn(ID_CHANGE, "历史涨跌幅%", 0, 90);
	m_listCtrl.InsertColumn(ID_STRATEGY_ROI, "策略总收益率%", 0, 110);
	m_listCtrl.InsertColumn(ID_STRATEGY_MAXGAIN, "策略最大收益率%", 0, 110);
	m_listCtrl.InsertColumn(ID_STRATEGY_MAXLOSS, "策略最大回撤%", 0, 110);
	m_listCtrl.InsertColumn(ID_STRATEGY_NUM_OF_POSGAIN, "正收益次数", 0, 110);
	m_listCtrl.InsertColumn(ID_STRATEGY_RATE_OF_POSGAIN, "正收益比例%", 0, 110);
	m_listCtrl.InsertColumn(ID_STRATEGY_NUM_OF_BUY, "买入次数", 0, 110);
	m_listCtrl.InsertColumn(ID_STRATEGY_NUM_OF_SELL, "卖出次数", 0, 110);
	m_listCtrl.InsertColumn(ID_STRATEGY, "运行策略", 0, 200);
	m_listCtrl.InsertColumn(ID_STATUS, "策略状态", 0, 200);
}

void CStockAnlysisDlg::LoadStockProducts()
{
	InstrumentManager::SECTOR mkt = (InstrumentManager::SECTOR)GetCurrentSector();

	g_InstrumentMgr.SetCurrentSector(mkt);

	vector<Instrument>& products = g_InstrumentMgr.GetInstruments(mkt);

	int i = 0;

	m_listCtrl.DeleteAllItems();

	m_list.clear();

	foreach(products, it, vector<Instrument>)
	{
		m_listCtrl.InsertItem(i, it->Code.c_str());	

		m_listCtrl.SetItemText(i, ID_NAME, it->Name.c_str());

		m_listCtrl.SetItemData(i, (DWORD_PTR)it->getCodeByInt());

		m_list.insert(make_pair(it->Code, i));

		i++;
	}

	if (m_listCtrl.GetItemCount() > 0)
	{
		m_listCtrl.SetSelectionMark(0);
	}
}

void CStockAnlysisDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	/*
	CMenu   menu;

	VERIFY(menu.LoadMenu(IDR_MENU_LIST));
	
	CMenu* pPopup = menu.GetSubMenu(0);
	
	ASSERT(pPopup != NULL);

	DWORD dwPos = GetMessagePos();
	CPoint pt( LOWORD(dwPos), HIWORD(dwPos) );
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, pt.x, pt.y, this);
*/
	*pResult = 0;
}

void CStockAnlysisDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadStockProducts();
}

void CStockAnlysisDlg::CalculateSelected(const char* strategyName)
{

}

void CStockAnlysisDlg::OnCalMa1()
{
	// TODO: 在此添加命令处理程序代码
	CalculateSelected("CrossMA");
}

void CStockAnlysisDlg::OnCalMa2()
{
	// TODO: 在此添加命令处理程序代码
	CalculateSelected("MA");
}

void CStockAnlysisDlg::OnCalTrend1()
{
	// TODO: 在此添加命令处理程序代码
	CalculateSelected("Trend");
}

void CStockAnlysisDlg::OnViewKline()
{
	// TODO: 在此添加命令处理程序代码
	// draw line
	int i = m_listCtrl.GetSelectionMark();

	if (i < 0)
	{
		return;
	}

	int code = (int)m_listCtrl.GetItemData(i);

	CString strCode;

	strCode.Format("%06d", code);

	ViewKLineWindow(strCode);
}

void CStockAnlysisDlg::OnCalMa1Auto()
{
	// TODO: 在此添加命令处理程序代码

}

void CStockAnlysisDlg::OnMenuExport()
{
	// TODO: 在此添加命令处理程序代码
	CString filePath;  
	char szFilter[] = {"All Files(*.*)|*.*||"};  
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL);  

	if(dlg.DoModal() == IDOK)  
	{  
		filePath = dlg.GetPathName();  
		filePath.Replace('/','\\');

		ofstream os(filePath.GetBuffer(), ios::out);
		
		if (os)
		{
			for (int i = 0; i < m_listCtrl.GetItemCount(); i++)
			{
				CString str;

				for (int col = 0; col < m_listCtrl.GetHeaderCtrl()->GetItemCount(); col++)
				{
					str += m_listCtrl.GetItemText(i, col);
					str += "	";
				}

				os<<str.GetBuffer()<<endl;

			}

			os.close();
		}
	}  
	else  
	{  
	}  
}


void CStockAnlysisDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CStockAnlysisDlg::OnCalAll()
{
	// TODO: 在此添加命令处理程序代码
}

int CStockAnlysisDlg::GetCurrentSector()
{
	int iRadioButton = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO6); 

	InstrumentManager::SECTOR mkt = InstrumentManager::SH;

	if (iRadioButton==IDC_RADIO1) 
		mkt = InstrumentManager::SH;

	if (iRadioButton==IDC_RADIO2) 
		mkt = InstrumentManager::SZ;

	if (iRadioButton==IDC_RADIO3) 
		mkt = InstrumentManager::ZXB;

	if (iRadioButton==IDC_RADIO4)
	{
		mkt = InstrumentManager::CYB;
	}

	if (iRadioButton==IDC_RADIO5)
	{
		mkt = InstrumentManager::ZXBK;
	}

	if (iRadioButton==IDC_RADIO6)
	{
		mkt = InstrumentManager::INDEX;
	}

	return mkt;
}

void CStockAnlysisDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CStockAnlysisDlg::OnCalAllCrossma()
{
	// TODO: 在此添加命令处理程序代码
}

void CStockAnlysisDlg::OnCrossmaResult()
{
	// TODO: 在此添加命令处理程序代码

}

void CStockAnlysisDlg::OnMmCalCrossma()
{
	// TODO: 在此添加命令处理程序代码
}

void CStockAnlysisDlg::OnMmQuit()
{
	// TODO: 在此添加命令处理程序代码
	PostMessage(WM_CLOSE, 0, 0);
}

void CStockAnlysisDlg::OnMmRecvquot()
{
	// TODO: 在此添加命令处理程序代码
	if (!g_QuanAnalyer.IsQuotReceiverRunning())
	{
		g_QuanAnalyer.StartQuotReceiver();
	}
}

void CStockAnlysisDlg::OnMmCalTrend()
{
	// TODO: 在此添加命令处理程序代码
}

void CStockAnlysisDlg::OnMmStopQuot()
{
	// TODO: 在此添加命令处理程序代码
	if (g_QuanAnalyer.IsQuotReceiverRunning())
	{
		g_QuanAnalyer.StopQuotReceiver();
	}
}

void CStockAnlysisDlg::OnMmWndAnalysis()
{
	// TODO: 在此添加命令处理程序代码
	OnViewKline();
}

void CStockAnlysisDlg::ViewKLineWindow(const char* code)
{
	CString strTitle;
	strTitle.Format("宽客分析-%s", code);

	SetWindowText(strTitle);

	Product* product = g_QuanAnalyer.GetProduct(code);

	if (product)
	{
		m_dlgKline->SetProduct(product);

		m_dlgKline->SetFocus();
	}
}

void CStockAnlysisDlg::OnStrategyRunCrossma()
{
	// TODO: 在此添加命令处理程序代码

	CalculateStrategy("CrossMA");
}

void CStockAnlysisDlg::OnStrategyRunTrend()
{
	// TODO: 在此添加命令处理程序代码
}

LRESULT CStockAnlysisDlg::OnQAM_PRODUCT_TRADE(WPARAM wParam, LPARAM lParam)
{
	auto_ptr<MsgProductTrade> msg((MsgProductTrade*)wParam);

	map<string, int>::iterator it = m_list.find(msg->code);

	if (it != m_list.end())
	{
		m_listCtrl.SetItemText(it->second, ID_STATUS, msg->flag?"卖出":"买入");
	}

	return 0;
}

LRESULT CStockAnlysisDlg::OnQAM_QUOTATION_LATEST(WPARAM wParam, LPARAM lParam)
{
	auto_ptr<MsgRecvQuotation> msg((MsgRecvQuotation*)wParam);

	typedef map<string, Quot> MAPQUOT;

	foreach(msg->quotations, it, MAPQUOT)
	{
		Product* product = g_QuanAnalyer.GetProduct(it->first.c_str());

		if (product)
		{
			//
			map<string, int>::iterator itList = m_list.find(it->first);

			if (itList != m_list.end())
			{
				CString str;
				str.Format("%.2f", it->second.getClose());
				m_listCtrl.SetItemText(itList->second, ID_LATEST, str);
			}
		}
	}

	return 0;
}

void CStockAnlysisDlg::OnUpdateMmRecvquot(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(g_QuanAnalyer.IsQuotReceiverRunning()?FALSE:TRUE);
}

void CStockAnlysisDlg::OnUpdateMmStopQuot(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(g_QuanAnalyer.IsQuotReceiverRunning()?TRUE:FALSE);
}

void CStockAnlysisDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: 在此处添加消息处理程序代码
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child Windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

void CStockAnlysisDlg::OnMmConfig()
{
	// TODO: 在此添加命令处理程序代码
	CConfigDlg dlg;
	dlg.DoModal();
}

void CStockAnlysisDlg::OnUpdateMmWndAnalysis(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_listCtrl.GetSelectionMark() > 0);
}

void CStockAnlysisDlg::OnMmAutocalAll()
{
	// TODO: 在此添加命令处理程序代码
	//g_QuanAnalyer.CalculateCrossMAStrategy(GetSafeHwnd(), QuanAnalyzer::ALL);
}

void CStockAnlysisDlg::OnMmAutocalCurrent()
{
	// TODO: 在此添加命令处理程序代码
	//g_QuanAnalyer.CalculateCrossMAStrategy(GetSafeHwnd(), GetCurrentSector());
	CalculateStrategy("CrossMA");
}

LRESULT CStockAnlysisDlg::OnQAM_Progress_SetRange(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(m_dlgProgress->GetSafeHwnd(), QAM_PROGRESS_SETRANGE, wParam, lParam);

	return 0;
}

LRESULT CStockAnlysisDlg::OnQAM_Progress_SetPos(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(m_dlgProgress->GetSafeHwnd(), QAM_PROGRESS_SETPOS, wParam, lParam);

	return 0;
}

bool ma_sortfunc(StrategyResult p1, StrategyResult p2)
{
	return p1.GetRateOfPosGain() > p2.GetRateOfPosGain();
}

LRESULT CStockAnlysisDlg::OnQAM_Progress_Done(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(m_dlgProgress->GetSafeHwnd(), QAM_PROGRESS_DONE, wParam, lParam);

	vector<StrategyResult> vtSR;
	
	for (size_t i = 0; i < CalculateStrategyThread::s_strategyresults.GetCount(); i++)
	{
		vtSR.push_back(CalculateStrategyThread::s_strategyresults.GetItem(i));
	}

	int count = 0;
	double percent = 0;

	foreach(vtSR, it, vector<StrategyResult>)
	{
		percent += it->GetRateOfPosGain();

		count++;
	}

	// 保存到文件
	CString strFileName;
	strFileName.Format("%s\\strategyresult.srf", g_Configuration.GetSaveFilePath().c_str());
    StatResultFile srf;
	srf.Save(vtSR, strFileName);

	// 买入top5
	vector<StrategyResult> vtTopBuy;

	// 卖出top5
	vector<StrategyResult> vtTopSell;

	// 持有top5
	vector<StrategyResult> vtTopHold;

	foreach(vtSR, it, vector<StrategyResult>)
	{
		if (it->CanBuy())
		{
			vtTopBuy.push_back(*it);
		}
		else if (it->CanSell())
		{
			vtTopBuy.push_back(*it);
		}
		else if (it->CanHold())
		{
			Date date = Date::Today();
			Date last_budy_date;
			last_budy_date.fromStringYYYYMMDD(it->GetLastBuyDate().c_str());

			int days = date - last_budy_date;

			if (days < 30)
			{
				vtTopHold.push_back(*it);
			}
		}
		else
		{

		}
	}

	// 保存买入前五
	if (!vtTopBuy.empty())
	{
		sort(vtTopBuy.begin(), vtTopBuy.end(), ma_sortfunc);
	}

	strFileName.Format("%s\\buy_top5.srf", g_Configuration.GetSaveFilePath().c_str());
	srf.Save(vtTopBuy, strFileName);

	// 保存卖出前五
	if (!vtTopSell.empty())
	{
		sort(vtTopSell.begin(), vtTopSell.end(), ma_sortfunc);
	}

	strFileName.Format("%s\\sell_top5.srf", g_Configuration.GetSaveFilePath().c_str());
	srf.Save(vtTopSell, strFileName);

	// 保存持有前五
	if (!vtTopHold.empty())
	{
		sort(vtTopHold.begin(), vtTopHold.end(), ma_sortfunc);
	}

	strFileName.Format("%s\\hold_top5.srf", g_Configuration.GetSaveFilePath().c_str());
	srf.Save(vtTopHold, strFileName);

	//
	CString str;
	str.Format("平均正收益率: %.2f%%", percent/count);
	AfxMessageBox(str);

	CDlgStrategyResult* pdlg = new CDlgStrategyResult(this);
	pdlg->Create(IDD_DIALOG_RESULT, this);
	pdlg->SetStrategyResults(vtSR);
	pdlg->ShowWindow(SW_NORMAL);

	return 0;
}

void CStockAnlysisDlg::OnMmSave()
{
	// TODO: 在此添加命令处理程序代码
	CString filePath;  
	char szFilter[] = {"All Files(*.*)|*.*||"};  
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL);  

	if(dlg.DoModal() == IDOK)  
	{  
		filePath = dlg.GetPathName();  
		filePath.Replace('/','\\');

		g_QuanAnalyer.Save(filePath);
	}  
}

void CStockAnlysisDlg::OnMmLoad()
{
	// TODO: 在此添加命令处理程序代码
	CString filePath;  
	char szFilter[] = {"All Files(*.*)|*.*||"};  
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL);  

	if(dlg.DoModal() == IDOK)  
	{  
		filePath = dlg.GetPathName();  
		filePath.Replace('/','\\');

		g_QuanAnalyer.Load(filePath);

		LoadStockProducts();
	}  
}

void CStockAnlysisDlg::OnCalTrendcross()
{
	// TODO: 在此添加命令处理程序代码
	CalculateSelected("TrendCrossMA");
}

void CStockAnlysisDlg::OnCalVolcross()
{
	// TODO: 在此添加命令处理程序代码
	CalculateSelected("VolumeCross");
}

void CStockAnlysisDlg::OnMaAdd()
{
	// TODO: 在此添加命令处理程序代码
	int n = m_listCtrl.GetSelectionMark();

	if (n >= 0)
	{
		int code = (int)m_listCtrl.GetItemData(n);

		CString strCode;
		strCode.Format("%06d", code);

		Product* product = g_QuanAnalyer.GetProduct(strCode);

		DlgMAEditor dlg;
		dlg.DoModal();
	}
}

void CStockAnlysisDlg::CalculateStrategy(const char* strategyname)
{
	CWaitCursor cursor;

	vector<StrategyResult> results;

	for(int i=0; i<m_listCtrl.GetItemCount(); i++)
	{
		if (m_listCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			int code = (int)m_listCtrl.GetItemData(i);

			CString strCode;
			strCode.Format("%06d", code);

			Product* product = g_QuanAnalyer.GetProduct(strCode);

			try
			{
				CString str;
				str.Format("%.2f", product->GetIncreaseRate());
				m_listCtrl.SetItemText(i, ID_CHANGE, str);

				StrategyCalculator calculator;

				DataHandler dh;

				calculator.AddStrategy(strategyname, dh);

				calculator.Calculate(product->GetQuotations());

				StrategyResult res = calculator.result();

				res.code_ = strCode;

				results.push_back(res);
			}
			catch (QAException& e)
			{
				AfxMessageBox(e.Message.c_str());	

				return;
			}
		}
	}

	cursor.Restore();

	// 显示结果
	CDlgStrategyResult* pdlg = new CDlgStrategyResult(this);
	pdlg->Create(IDD_DIALOG_RESULT, this);
	pdlg->SetStrategyResults(results);
	pdlg->ShowWindow(SW_NORMAL);
}

void CStockAnlysisDlg::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	ViewKLineWindow(m_strCode);
}

void CStockAnlysisDlg::OnStrategyStat()
{
	// TODO: 在此添加命令处理程序代码
}

void CStockAnlysisDlg::OnMmCalStrCurrent()
{
	// TODO: 在此添加命令处理程序代码
	vector<string> strategies;
	int cycle = 0;

	CDlgStrategyEditor dlg;
	dlg.SetStrategies(&strategies);

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	cycle = dlg.GetCycle();

	//
	DataHandler datahandler;

	CalculateStrategyThread::SetStrategy(strategies);

	CalculateStrategyThread::Run(GetCurrentSector(), GetSafeHwnd(), datahandler, cycle);
}

void CStockAnlysisDlg::OnMmCalStrChosen()
{
	// TODO: 在此添加命令处理程序代码
	vector<string> strategies;
	int cycle = 0;

	CDlgStrategyEditor dlg;
	dlg.SetStrategies(&strategies);

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	cycle = dlg.GetCycle();

	//
	vector<Instrument> instrs;

	for(int i=0; i<m_listCtrl.GetItemCount(); i++)
	{
		if (m_listCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			int code = (int)m_listCtrl.GetItemData(i);

			CString strCode;
			strCode.Format("%06d", code);

			instrs.push_back(g_InstrumentMgr.GetInstrument(strCode));
		}
	}

	DataHandler datahandler;

	CalculateStrategyThread::SetStrategy(strategies);
	CalculateStrategyThread::Run(instrs, GetSafeHwnd(), datahandler, cycle);
}

void CStockAnlysisDlg::OnMmCalStrAll()
{
	// TODO: 在此添加命令处理程序代码
	vector<string> strategies;
	int cycle = 0;

	CDlgStrategyEditor dlg;
	dlg.SetStrategies(&strategies);

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	cycle = dlg.GetCycle();

	//
	DataHandler datahandler;

	CalculateStrategyThread::SetStrategy(strategies);

	CalculateStrategyThread::Run(InstrumentManager::ALL, GetSafeHwnd(), datahandler, cycle);
}

void CStockAnlysisDlg::OnStrtegyOffset()
{
	// TODO: 在此添加命令处理程序代码
	CalculateSelected("MAOffset");
}
