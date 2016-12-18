# coding=utf8

__author__ = 'jerry'


# coding=utf8

__author__ = 'Administrator'

import tushare as ts
import pandas as pd
import commondatadef
import numpy as np
import datetime
from datetime import timedelta
import os
import sys
import stock_utility as su
import commondatadef as cdd
import pandas as pd

reload(sys)
sys.setdefaultencoding('utf8')

print ts.__version__

# 下载数据
instruments = su.get_stock_list()

# 如果下载成功，则保存
if not instruments is None:
    print "download instruments success, save to file %s" % (cdd.code_list_file)
    instruments.to_csv(cdd.code_list_file)
else:
    print "download instruments success, read from file %s" % (cdd.code_list_file)
    instruments = pd.read_csv(cdd.code_list_file,  index_col=['code'])

if instruments is None:
    print "instruments is empty, exit"
    exit()

#
today = datetime.date.today()

#
def downloadstock(instrument, startdate, enddate, filepath):
    df = ts.get_h_data(instrument, start=startdate, end=enddate, autype='hfq')

    df.sort_index(inplace=True)

    df.to_csv(filepath)

# 是否数字
def isNum(value):
    try:
        float(value) + 1
    except TypeError:
        return False
    except ValueError:
        return False
    else:
        return True
#
for code in instruments.index:
    filepath = "%s/%s.csv" % (commondatadef.data_dayPath, code)
    print "starting download %s, file path: %s" % (code, filepath)
    startdate = "1990-01-01"
    enddate = today.strftime("%Y-%m-%d")
    df = pd.DataFrame()
    if os.path.exists(filepath):
        df = pd.read_csv(filepath, index_col=['date'])
        last_date = datetime.datetime.strptime(df.index[-1], "%Y-%m-%d") + datetime.timedelta(days=1)
        startdate = last_date.strftime("%Y-%m-%d")
        print "file %s exists, download data from %s" % (filepath, startdate)

        #modified = False
        #dflen = df.shape[0]
        #for i in range(0, dflen):
        #    x = df.iat[i, 1]
        #    if (i > 1) and (isNum(x) is False):
        #        print "remove row: %d" % (i, )
        #        df.drop('date', inplace=True)
        #        modified = True
        #        break

        #if modified is True:
        #    print "save file %s" % (filepath, )
        #    df.to_csv(filepath)

    data_enddate = datetime.datetime.strptime(enddate, "%Y-%m-%d") + timedelta(days=1)

    print "download data, code: %s, startdate: %s, enddate: %s" % (code, startdate, data_enddate.strftime("%Y-%m-%d"))

    df_download = ts.get_h_data(code, start=startdate, end=data_enddate.strftime("%Y-%m-%d"), autype='hfq')

    if df_download is None:
        continue

    #
    df_download.sort_index(inplace=True)

    if os.path.exists(filepath):
        df_download.to_csv(filepath, mode='a', header=None)
    else:
        df_download.to_csv(filepath)


