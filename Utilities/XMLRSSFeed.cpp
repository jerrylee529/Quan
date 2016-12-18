/*******************************************************************************
* (C) Copyright 2011 
* 
* These computer program listings and specifications are the property of Jerry 
* Lee and shall not be reproduced or copied or used in whole or in 
* part without written permission from Jerry Lee.
*
* Project:      IAdvertising 
* File Name:	XMLRSSFeed.cpp 
* Programer(s):	Jerry Lee 
* Created:      20110104 
* Description:	RSS Feed XML∂¡–¥π§æﬂ¿‡
* History:
*******************************************************************************/


#include "XMLRSSFeed.h"
#include "Markup.h"

//
XMLRSSFeedReader::XMLRSSFeedReader()
{

}

XMLRSSFeedReader::~XMLRSSFeedReader()
{

}

bool XMLRSSFeedReader::Read(const string &filename, RSSFeed &feed)
{
    if (!m_xmlDoc.Load(filename))
    {
        return false;
    }

    //
    m_xmlDoc.ResetMainPos();
    
    //
    if (m_xmlDoc.FindElem(RSS_HEADER))
    {
        // moves into rss level
        m_xmlDoc.IntoElem(); 

        //
        ReadChannel(feed.channel);

        //
        m_xmlDoc.OutOfElem();
    }

    return true;
}

void XMLRSSFeedReader::ReadChannel(RSSChannel &channel)
{
    if (m_xmlDoc.FindElem(RSS_CHANNEL))
    {
        string str;

        // moves into channel
        // 
        m_xmlDoc.ResetChildPos();
        if (m_xmlDoc.FindChildElem(CHAN_TTL))
        {
            channel.ttl = atoi(m_xmlDoc.GetChildData().c_str());
        }

        //
        m_xmlDoc.ResetChildPos();
        if (m_xmlDoc.FindChildElem(CHAN_LASTBUILDDATE))
        {
            channel.lastBuildDate = m_xmlDoc.GetChildData();
        }

        //
        m_xmlDoc.ResetChildPos();
        if (m_xmlDoc.FindChildElem(CHAN_SKIPDAYS))
        {
            channel.skipDays = atoi(m_xmlDoc.GetChildData().c_str());
        }

        //
        m_xmlDoc.ResetChildPos();
        if (m_xmlDoc.FindChildElem(CHAN_SKIPHOURS))
        {
            channel.skipHours = atoi(m_xmlDoc.GetChildData().c_str());
        }

        m_xmlDoc.ResetChildPos();
        while(m_xmlDoc.FindChildElem(RSS_ITEM))
        {
            RSSItem item;

            ReadItem(item);

            channel.items.push_back(item);
        }
        //
    }
}

void XMLRSSFeedReader::ReadItem(RSSItem &item)
{
    // move into item level
    m_xmlDoc.IntoElem();

    // get title
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(ITEM_TITLE))
    {
        item.title = m_xmlDoc.GetChildData();
    }

    // get link
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(ITEM_LINK))
    {
        item.link = m_xmlDoc.GetChildData();
    }

    // get description
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(ITEM_DESC))
    {
        item.description = m_xmlDoc.GetChildData();
    }

    // get pubdate
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(ITEM_PUBDATE))
    {
        item.pubDate = m_xmlDoc.GetChildData();
    }

    // get enclosure
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(ITEM_ENCLOSURE))
    {
        ReadEnclosure(item.enclosure);
    }

    //
    m_xmlDoc.OutOfElem();
}

void XMLRSSFeedReader::ReadCategory(RSSCategory &category)
{

}

void XMLRSSFeedReader::ReadEnclosure(RSSEnclosure &enclosure)
{
    enclosure.url = m_xmlDoc.GetChildAttrib(ATTIBUTE_URL);
    enclosure.length = atoi(m_xmlDoc.GetChildAttrib(ATTIBUTE_LEGNTH).c_str());
    enclosure.type = m_xmlDoc.GetChildAttrib(ATTIBUTE_TYPE);
}

void XMLRSSFeedReader::ReadGuid(RSSGUID &guid)
{

}

void XMLRSSFeedReader::ReadImage(RSSImage &image)
{

}

void XMLRSSFeedReader::ReadCloud(RSSCloud &cloud)
{

}

void XMLRSSFeedReader::ReadSource(RSSSource &source)
{

}


//
XMLRSSFeedWriter::XMLRSSFeedWriter()
{

}

XMLRSSFeedWriter::~XMLRSSFeedWriter()
{

}

bool XMLRSSFeedWriter::Write(const string &filename, const RSSFeed &feed)
{
    m_xmlDoc.SetDoc(XML_HEADER);

    m_xmlDoc.AddElem(RSS_HEADER);
    
    m_xmlDoc.IntoElem(); 

    WriteChannel(feed.channel);

    m_xmlDoc.OutOfElem();

    return m_xmlDoc.Save(filename);
}

void XMLRSSFeedWriter::WriteChannel(const RSSChannel &channel)
{
    m_xmlDoc.AddElem(RSS_CHANNEL);

    m_xmlDoc.IntoElem(); 

    m_xmlDoc.AddElem(CHAN_TTL, channel.ttl);
    m_xmlDoc.AddElem(CHAN_LASTBUILDDATE, channel.lastBuildDate);
    m_xmlDoc.AddElem(CHAN_SKIPDAYS, channel.skipDays);
    m_xmlDoc.AddElem(CHAN_SKIPHOURS, channel.skipHours);
    for (VT_RSSITEM::const_iterator it = channel.items.begin(); 
        it != channel.items.end(); it++)
    {
        WriteItem(*it);
    }

    m_xmlDoc.OutOfElem();
}

void XMLRSSFeedWriter::WriteItem(const RSSItem &item)
{
    m_xmlDoc.AddElem(RSS_ITEM);

    m_xmlDoc.IntoElem();
    
    m_xmlDoc.AddElem(ITEM_TITLE, item.title);
    m_xmlDoc.AddElem(ITEM_LINK, item.link);
    m_xmlDoc.AddElem(ITEM_DESC, item.description);
    m_xmlDoc.AddElem(ITEM_PUBDATE, item.pubDate);

    WriteEnclosure(item.enclosure);
    
    m_xmlDoc.OutOfElem();
}

void XMLRSSFeedWriter::WriteCategory(const RSSCategory &category)
{

}

void XMLRSSFeedWriter::WriteEnclosure(const RSSEnclosure &enclosure)
{
    m_xmlDoc.AddElem(ITEM_ENCLOSURE);

    m_xmlDoc.AddAttrib(ATTIBUTE_URL, enclosure.url);
    m_xmlDoc.AddAttrib(ATTIBUTE_TYPE, enclosure.type);
    m_xmlDoc.AddAttrib(ATTIBUTE_LEGNTH, enclosure.length);
}

void XMLRSSFeedWriter::WriteGuid(const RSSGUID &guid)
{

}

void XMLRSSFeedWriter::WriteImage(const RSSImage &image)
{

}

void XMLRSSFeedWriter::WriteCloud(const RSSCloud &cloud)
{

}

void XMLRSSFeedWriter::WriteSource(const RSSSource &source)
{

}
