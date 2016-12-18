# correlation calculator
# -*- coding:utf-8 -*-
__author__ = 'jerry'


import tushare as ts
import pandas as pd
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei'] #指定默认字体

plt.rcParams['axes.unicode_minus'] = False #解决保存图像是负号'-'显示为方块的问题

# sh=上证指数 sz=深圳成指 hs300=沪深300指数 sz50=上证50 zxb=中小板 cyb=创业板
# 获取历史数据
#result = ts.get_hist_data('600535')

def get_return_index(code):
    histdata = ts.get_hist_data(code)
    returns = histdata['close'].pct_change()
    return_index = (1 + returns).cumprod()
    return return_index

code = '600036'
name = '中国平安'

#index_list = ['sh','sz', 'hs300', 'sz50', 'zxb', 'cyb', code]
index_list = ['sh', code]

#index_name = {'sh':'上证', 'sz':'深证', 'hs300':"沪深300", 'sz50':'上证50', 'zxb':'中小板', 'cyb':'创业板', code:name}
index_name = {'sh':'上证', code:name}

all_data = pd.DataFrame()

for index in index_list:
    all_data[index_name[index]] = get_return_index(index)


#all_data = all_data.cumsum()

all_data.plot()

#print all_data

plt.show()


