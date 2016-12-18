/*******************************************************************************
* (C) Copyright 2011 
* 
* These computer program listings and specifications are the property of Jerry 
* Lee and shall not be reproduced or copied or used in whole or in 
* part without written permission from Jerry Lee.
*
* Project:      IAdvertising 
* File Name:	XMLGroupBuyFeed.h 
* Programer(s):	Jerry Lee 
* Created:      20110104 
* Description:	RSS Feed生成器，用于RSS Feed文件的管理和生成
* History:
*******************************************************************************/

#ifndef _XML_GROUPBUY_FEED_H
#define _XML_GROUPBUY_FEED_H

#include "GroupBuyDef.h"
#include "GroupBuyConf.h"
#include "Markup.h"

//
// GroupBuy Feed读取器
class XMLGroupBuyFeedReader : public GroupBuyFeedReader
{
public:
    XMLGroupBuyFeedReader();
    ~XMLGroupBuyFeedReader();

    virtual bool Read(const string &filename, GroupBuyFeed &feed);

protected:
    virtual void ReadItem(GroupBuyInfo &item);

private:
    CMarkup m_xmlDoc;
};

// GroupBuy Feed写入器
class XMLGroupBuyFeedWriter : public GroupBuyFeedWriter
{
public:
    XMLGroupBuyFeedWriter();
    ~XMLGroupBuyFeedWriter();

    virtual bool Write(const string &filename, const GroupBuyFeed &feed);

protected:
    virtual void WriteItem(const GroupBuyInfo &item);

private:
    CMarkup m_xmlDoc;
};

// GroupBuy Configuration
class XMLGroupBuyConfReader : public GroupBuyConfReader
{
public:
    virtual bool Read(const string &filename, GroupBuyConf &groupBuyConf);

private:
    void ReadSite(GroupBuySite &groupBySite);

    CMarkup m_xmlDoc;
};


#endif // _XML_GROUPBUY_FEED_H