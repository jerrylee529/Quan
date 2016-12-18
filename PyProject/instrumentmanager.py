# coding=utf8

__author__ = 'Administrator'

import tushare as ts
import pandas as pd
import commondatadef
import os
import sys

reload(sys)
sys.setdefaultencoding('utf8')

print ts.__version__

# 获取股票代码
configfile = commondatadef.configPath + '/instruments.txt'

instruments = pd.DataFrame()

if not os.path.exists(configfile):
    instruments = ts.get_industry_classified()
    instruments.to_csv(configfile, header=None)
else:
    instruments = pd.read_csv(configfile, names=['code','name','industry'])
