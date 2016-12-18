// DlgChart.cpp : 实现文件
//

#include "stdafx.h"
#include "StockAnlysis.h"
#include "DlgChart.h"
#include "series.h"
#include "surfaceseries.h"
#include "waterfallseries.h"
#include "TeeChartDefines.h"
#include "SeriesTextField.h"
#include "aspect.h"
#include "legend.h"
#include "axes.h"
#include "axis.h"
#include "titles.h"
#include "strings.h"
#include "axistitle.h"
#include "axislabels.h"
#include "panel.h"
#include "gradient.h"
#include "lineseries.h"
#include "pen.h"
#include "teeopengl.h"
#include "candleseries.h"
#include "scroll.h"
#include <vector>
#include "StockDataStructures.h"
#include "Utilities.h"


using namespace std;

// CDlgChart 对话框

IMPLEMENT_DYNAMIC(CDlgChart, CDialog)

CDlgChart::CDlgChart(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChart::IDD, pParent)
{

}

CDlgChart::~CDlgChart()
{
}

void CDlgChart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TCHART1, m_chart);
}


BEGIN_MESSAGE_MAP(CDlgChart, CDialog)
END_MESSAGE_MAP()


// CDlgChart 消息处理程序
BOOL CDlgChart::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_chart.GetLegend().SetVisible(false);//隐藏图例  
	m_chart.GetAspect().SetView3D(FALSE);  //取消3D显示
	m_chart.GetScroll().SetEnable(pmHorizontal);

	/*
	//设置图标标题  
	m_chart.GetHeader().GetText().SetItem(0,COleVariant("传感器实时数据曲线"));  

	//设置纵轴标题
	m_chart.GetAxis().GetLeft().GetTitle().SetCaption("数值");  

	//设置渐变背景
	m_chart.GetPanel().GetGradient().SetVisible(true);
	m_chart.GetPanel().GetGradient().SetStartColor(RGB(192,192,192));
	m_chart.GetPanel().GetGradient().SetEndColor(RGB(255,255,255));  
	*/

	/*
	//设置曲线属性  
	m_chart.Series(0).SetColor(RGB(255,0,0));//颜色  
	m_chart.Series(0).GetAsLine().GetLinePen().SetWidth(2); //线型宽度  

	//设置x轴的取值范围  
	m_chart.GetAxis().GetBottom().SetMinMax(0,100);  

	//设置x轴上值的格式
	m_chart.GetAxis().GetBottom().GetLabels().SetValueFormat("0.0");  
	*/

	m_chart.RemoveAllSeries();

	//添加曲线
	m_chart.AddSeries(scCandle);  

	CCandleSeries series = m_chart.Series(0).GetAsCandle();

	vector<Quot>& quots = *m_product->getQuotations();

	double d=1000;
	foreach(quots, it, vector<Quot>)
	{
		tm tm_;
		memset(&tm_, 0, sizeof(tm_));

    	int year, month, day;  
		sscanf(it->Date,"%04d%02d%02d", &year, &month, &day);  
		tm_.tm_year  = year-1900;  
		tm_.tm_mon   = month-1;  
		tm_.tm_mday  = day;  
		tm_.tm_hour  = 0;  
		tm_.tm_min   = 0;  
		tm_.tm_sec   = 0;  
		tm_.tm_isdst = 0;  

		time_t t = mktime(&tm_);

		series.AddCandle(t, it->Open, it->Close, it->High, it->Low, 0, clNone);
/*
		if (d > 1003)
		{
			break;
		}
		*/
	}


	return TRUE;
}

void CDlgChart::SetProduct(Product* product)
{
	ASSERT(product != NULL);

	m_product = product;
}