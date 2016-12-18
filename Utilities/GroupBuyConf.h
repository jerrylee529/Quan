
#ifndef _GROUPBUY_CONF_H
#define _GROUPBUY_CONF_H

#include "GroupBuyDef.h"

// 团购信息网站
typedef struct tagGroupBuySite
{
    string name;      // 网站名称
    string cityUrl;   // 获取城市信息的url
    string productUrl;// 获取产品信息的url
    string decodeDll; // 解析动态库
    string filename;  // 保存后的文件名
} GroupBuySite;


// 定义团购网站列表
typedef vector<GroupBuySite> VT_GROUPBYSITE;


// 团购信息配置
typedef struct tagGroupBuyConf
{
    string downloadPath;    // 下载保存路径
    VT_GROUPBYSITE sites;   // 团购站点信息   
} GroupBuyConf;


// 团购信息读取器
class GroupBuyConfReader
{
public:
    virtual bool Read(const string &filename, GroupBuyConf &groupBuyConf) = 0;

};


#endif
