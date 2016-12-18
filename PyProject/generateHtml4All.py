# coding=utf8

__author__ = 'Administrator'

import csv2html

column_name_list = {}
column_name_list['name'] = ['代码', '名称', '每股收益', '净资产收益率', '市盈率', '市净率', 'PEG']
column_name_list['index'] = [0, 1, 2, 5, 20, 21, 22]

outputfile = "%s/mafilter_%d_%s.csv" % (commondatadef.resultPath, ma, today.strftime("%Y%m%d"))

csv2html("f://thebestroe_report_20161014.csv", "f://demo_1.html", "test", column_name_list)