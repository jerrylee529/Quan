// StockAnlysis.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "StockAnlysis.h"
#include "StockAnlysisDlg.h"
#include "Utilities.h"
#include "Configuration.h"
#include "QuanAnalyzer.h"
#include "calculatestrategythread.h"
#include "instrumentmanager.h"
#include <string>
#include <boost/program_options.hpp>

using namespace boost::program_options;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define NO_INTERFACE

// CStockAnlysisApp

BEGIN_MESSAGE_MAP(CStockAnlysisApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CStockAnlysisApp construction

CStockAnlysisApp::CStockAnlysisApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CStockAnlysisApp object

CStockAnlysisApp theApp;


// CStockAnlysisApp initialization

BOOL CStockAnlysisApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	options_description opts("test options");  

	opts.add_options()  
		("help,h","help info")  
		("configuration,c",value<string>(),"configuration filename");  

	variables_map vm;  
	try  
	{  
		store(parse_command_line(__argc, __argv, opts), vm); // 分析参数  
	}  
	catch(boost::program_options::error_with_no_option_name &ex)  
	{  
		cout<<ex.what()<<endl;  
	}  

	notify(vm); // 将解析的结果存储到外部变量  

	string strConfFile;

	if (vm.count("help"))  
	{  
		cout<<opts<<endl;  
		return -1;  
	}  

	if(vm.count("configuration"))  
	{  
		strConfFile = vm["configuration"].as<std::string>();

		cout<<vm["configuration"].as<std::string>()<<endl;  
	}  

	// 载入配置
	g_Configuration.Load(strConfFile.c_str());


	CStockAnlysisDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}



	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
