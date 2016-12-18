// DlgMAEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "StockAnlysis.h"
#include "DlgMAEditor.h"
#include "Utilities.h"
#include "Configuration.h"
#include <vector>

using namespace std;

// DlgMAEditor 对话框

IMPLEMENT_DYNAMIC(DlgMAEditor, CDialog)

DlgMAEditor::DlgMAEditor(CWnd* pParent /*=NULL*/)
	: CDialog(DlgMAEditor::IDD, pParent)
{
	m_cycle = 5;
}

DlgMAEditor::~DlgMAEditor()
{
}

void DlgMAEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_MA, m_listbox);

	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);

	DDX_Control(pDX, IDC_BUTTON_DEL, m_btnDel);

	DDX_Text(pDX, IDC_EDIT_MA, m_cycle);
}


BEGIN_MESSAGE_MAP(DlgMAEditor, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_MA, &DlgMAEditor::OnLbnSelchangeListMa)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &DlgMAEditor::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &DlgMAEditor::OnBnClickedButtonDel)
END_MESSAGE_MAP()


BOOL DlgMAEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	LoadMALines();

	return TRUE;
}

// DlgMAEditor 消息处理程序

void DlgMAEditor::OnLbnSelchangeListMa()
{
	// TODO: 在此添加控件通知处理程序代码

	m_btnDel.EnableWindow((m_listbox.GetCurSel() >= 0)?TRUE:FALSE);
}

bool ma_sortfunc(int p1, int p2)
{
	return p1 < p2;
}

void DlgMAEditor::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	vector<int> cycles;

	g_Configuration.GetMALines(cycles);

	vector<int>::iterator it = find(cycles.begin(), cycles.end(), m_cycle);

	if (it == cycles.end())
	{
		cycles.push_back(m_cycle);

		sort(cycles.begin(), cycles.end(), ma_sortfunc);

		g_Configuration.SetMALines(cycles);

		LoadMALines();
	}
}

void DlgMAEditor::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int n = m_listbox.GetCurSel();

	if (n >= 0)
	{
		int cycle = (int)m_listbox.GetItemData(n);

		vector<int> cycles;

		g_Configuration.GetMALines(cycles);

		vector<int>::iterator it = find(cycles.begin(), cycles.end(), cycle);

		if (it != cycles.end())
		{
			cycles.erase(it);

			g_Configuration.SetMALines(cycles);

			LoadMALines();
		}
	}
}

void DlgMAEditor::LoadMALines()
{
	m_listbox.ResetContent();

	vector<int> malines;

	g_Configuration.GetMALines(malines);

	foreach(malines, it, vector<int>)
	{
		CString str;
		str.Format("%d", *it);
		int n = m_listbox.AddString(str);
		m_listbox.SetItemData(n, (DWORD_PTR)(*it));
	}

	if (m_listbox.GetCount() > 0)
	{
		m_listbox.SetCurSel(0);
	}
}