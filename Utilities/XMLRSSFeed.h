/*******************************************************************************
* (C) Copyright 2011 
* 
* These computer program listings and specifications are the property of Jerry 
* Lee and shall not be reproduced or copied or used in whole or in 
* part without written permission from Jerry Lee.
*
* Project:      IAdvertising 
* File Name:	RSSFeed.h 
* Programer(s):	Jerry Lee 
* Created:      20110104 
* Description:	RSS Feed生成器，用于RSS Feed文件的管理和生成
* History:
*******************************************************************************/

#ifndef _XML_RSS_FEED_H
#define _XML_RSS_FEED_H

#include "RSSFeedDef.h"
#include "Markup.h"

//
// RSS Feed读取器
class XMLRSSFeedReader : public RSSFeedReader
{
public:
    XMLRSSFeedReader();
    ~XMLRSSFeedReader();

    virtual bool Read(const string &filename, RSSFeed &feed);

protected:
    virtual void ReadChannel(RSSChannel &channel);
    virtual void ReadItem(RSSItem &item);
    virtual void ReadCategory(RSSCategory &category);
    virtual void ReadEnclosure(RSSEnclosure &enclosure);
    virtual void ReadGuid(RSSGUID &guid);
    virtual void ReadImage(RSSImage &image);
    virtual void ReadCloud(RSSCloud &cloud);
    virtual void ReadSource(RSSSource &source);

private:
    CMarkup m_xmlDoc;
};

// RSS Feed写入器
class XMLRSSFeedWriter : public RSSFeedWriter
{
public:
    XMLRSSFeedWriter();
    ~XMLRSSFeedWriter();

    virtual bool Write(const string &filename, const RSSFeed &feed);

protected:
    virtual void WriteChannel(const RSSChannel &channel);
    virtual void WriteItem(const RSSItem &item);
    virtual void WriteCategory(const RSSCategory &category);
    virtual void WriteEnclosure(const RSSEnclosure &enclosure);
    virtual void WriteGuid(const RSSGUID &guid);
    virtual void WriteImage(const RSSImage &image);
    virtual void WriteCloud(const RSSCloud &cloud);
    virtual void WriteSource(const RSSSource &source);

private:
    CMarkup m_xmlDoc;
};


#endif // _XML_RSS_FEED_H