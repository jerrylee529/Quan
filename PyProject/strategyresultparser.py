# -*- coding: UTF-8 -*-
__author__ = 'Administrator'

import pandas as pd

def get_stock_trades():
    path = "E:/Stock/product/"

    filepath = path + "strategyresult.srf"

    df = pd.read_csv(filepath, header=None, sep='\t', index_col=0)

    trades_col = df[15]

    # 去掉
    trades_col = trades_col.dropna()

    stock_trades = {}

    #
    for index in trades_col.index:
        result = trades_col[index]
        trades = result.split(',')
        #print trades
        stock_trades[index] = []
        for trade in trades:
            items = trade.split(':')
            if len(items) != 6:
                continue
            trade_info = {}
            trade_info['buy_date'] = items[0]
            trade_info['buy_price'] = items[1]
            trade_info['sell_date'] = items[2]
            trade_info['sell_price'] = items[3]
            trade_info['period'] = items[4]
            trade_info['ratio'] = items[5]
            #print trade_info

            stock_trades[index].append(trade_info)
            #stocks[index] =

        #print stock_trades[index]

    return stock_trades