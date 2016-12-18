/*******************************************************************************
* (C) Copyright 2011 
* 
* These computer program listings and specifications are the property of Jerry 
* Lee and shall not be reproduced or copied or used in whole or in 
* part without written permission from Jerry Lee.
*
* Project:      IAdvertising 
* File Name:	XMLGroupBuyFeed.cpp 
* Programer(s):	Jerry Lee 
* Created:      20110104 
* Description:	RSS Feed XML∂¡–¥π§æﬂ¿‡
* History:
*******************************************************************************/


#include "XMLGroupBuyFeed.h"
#include "Markup.h"

#define TAG_HEADER          "GroupBuy"
#define TAG_VERSION         "Version"
#define TAG_ITEM            "Item"
#define TAG_NAME            "Name"
#define TAG_TITLE           "Title"
#define TAG_PRICE           "Price"
#define TAG_TIME            "Time"
#define TAG_LINK            "Link"
#define TAG_CATEGORY        "Category"
#define TAG_DESCRIPTION     "Description"
#define TAG_CITYID          "CityID"
#define TAG_CITYNAME        "CityName"


//
XMLGroupBuyFeedReader::XMLGroupBuyFeedReader()
{

}

XMLGroupBuyFeedReader::~XMLGroupBuyFeedReader()
{

}

bool XMLGroupBuyFeedReader::Read(const string &filename, GroupBuyFeed &feed)
{
    if (!m_xmlDoc.Load(filename))
    {
        return false;
    }

    //
    m_xmlDoc.ResetMainPos();
    
    //
    if (m_xmlDoc.FindElem(TAG_HEADER))
    {
        feed.version = atoi(m_xmlDoc.GetAttrib(TAG_VERSION).c_str());

        // moves into rss level
        m_xmlDoc.IntoElem(); 

        //
        m_xmlDoc.ResetChildPos();

        while(m_xmlDoc.FindChildElem(TAG_ITEM))
        {
            GroupBuyInfo item;

            ReadItem(item);

            feed.items.push_back(item);
        }

        //
        m_xmlDoc.OutOfElem();
    }

    return true;
}


void XMLGroupBuyFeedReader::ReadItem(GroupBuyInfo &item)
{
    // move into item level
    m_xmlDoc.IntoElem();

    // get title
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(TAG_NAME))
    {
        item.title = m_xmlDoc.GetChildData();
    }

    // get title
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(TAG_TITLE))
    {
        item.title = m_xmlDoc.GetChildData();
    }

    // get description
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(TAG_DESCRIPTION))
    {
        item.description = m_xmlDoc.GetChildData();
    }

    // get price
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(TAG_PRICE))
    {
        item.price = atoi(m_xmlDoc.GetChildData().c_str());
    }

    // get time
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(TAG_TIME))
    {
        item.time = atoi(m_xmlDoc.GetChildData().c_str());
    }

    // get link
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(TAG_LINK))
    {
        item.url = m_xmlDoc.GetChildData();
    }

    // get category
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(TAG_CATEGORY))
    {
        item.category = m_xmlDoc.GetChildData();
    }

    // get cityid
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(TAG_CITYID))
    {
        item.cityId = m_xmlDoc.GetChildData();
    }

    // get cityname
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindChildElem(TAG_CITYNAME))
    {
        item.cityName = m_xmlDoc.GetChildData();
    }

    //
    m_xmlDoc.OutOfElem();
}


//
XMLGroupBuyFeedWriter::XMLGroupBuyFeedWriter()
{

}

XMLGroupBuyFeedWriter::~XMLGroupBuyFeedWriter()
{

}

bool XMLGroupBuyFeedWriter::Write(const string &filename, const GroupBuyFeed &feed)
{
    m_xmlDoc.SetDoc(XML_HEADER);

    m_xmlDoc.AddElem(TAG_HEADER);
    
    m_xmlDoc.SetAttrib(TAG_VERSION, 1000);

    m_xmlDoc.IntoElem(); 

    for (vector<GroupBuyInfo>::const_iterator it = feed.items.begin(); it != feed.items.end();
        it++)
    {
        WriteItem(*it);
    }
    
    m_xmlDoc.OutOfElem();

    return m_xmlDoc.Save(filename);
}

void XMLGroupBuyFeedWriter::WriteItem(const GroupBuyInfo &item)
{
    m_xmlDoc.AddElem(TAG_ITEM);

    m_xmlDoc.IntoElem();
    
    m_xmlDoc.AddElem(TAG_TITLE, item.title);
    m_xmlDoc.AddElem(TAG_NAME, item.name);
    m_xmlDoc.AddElem(TAG_PRICE, item.price);
    m_xmlDoc.AddElem(TAG_TIME, item.time);
    m_xmlDoc.AddElem(TAG_LINK, item.url);
    m_xmlDoc.AddElem(TAG_CATEGORY, item.category);
    m_xmlDoc.AddElem(TAG_DESCRIPTION, item.description);
    m_xmlDoc.AddElem(TAG_CITYID, item.cityId);
    m_xmlDoc.AddElem(TAG_CITYNAME, item.cityName);
    
    m_xmlDoc.OutOfElem();
}

//////////////////////////////////////////////////////////////////////////
#define TAG_SITES       "Sites"
#define TAG_SITE        "Site"
#define TAG_CITY        "City"
#define TAG_PRODUCT     "Product"
#define TAG_DECODE      "DecodeDLL"
#define TAG_DOWNLOAD    "Download"
#define TAG_FILENAME    "FileName"

bool XMLGroupBuyConfReader::Read(const string &filename, GroupBuyConf &groupBuyConf)
{
    if (!m_xmlDoc.Load(filename))
    {
        return false;
    }

    //
    m_xmlDoc.ResetMainPos();

    //
    if (m_xmlDoc.FindElem(TAG_HEADER))
    {
        // moves into rss level
        m_xmlDoc.IntoElem(); 

        //
        m_xmlDoc.ResetChildPos();
    
        if (m_xmlDoc.FindElem(TAG_SITES))
        {
            m_xmlDoc.IntoElem();

            m_xmlDoc.ResetChildPos();
            if (m_xmlDoc.FindElem(TAG_DOWNLOAD))
            {
                groupBuyConf.downloadPath = m_xmlDoc.GetData();
            }

            m_xmlDoc.ResetChildPos();
            while(m_xmlDoc.FindElem(TAG_SITE))
            {
                m_xmlDoc.IntoElem();

                GroupBuySite gbs;
                
                ReadSite(gbs);

                groupBuyConf.sites.push_back(gbs);

                m_xmlDoc.OutOfElem();
            }

            m_xmlDoc.OutOfElem();
       }

        //
        m_xmlDoc.OutOfElem();
    }

    return true;
}

void XMLGroupBuyConfReader::ReadSite(GroupBuySite &groupBySite)
{
    // get name
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindElem(TAG_NAME))
    {
        groupBySite.name = m_xmlDoc.GetData();
    }

    // get city
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindElem(TAG_CITY))
    {
        groupBySite.cityUrl = m_xmlDoc.GetData();
    }

    // get product
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindElem(TAG_PRODUCT))
    {
        groupBySite.productUrl = m_xmlDoc.GetData();
    }

    // get decode dll
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindElem(TAG_DECODE))
    {
        groupBySite.decodeDll = m_xmlDoc.GetData();
    }

    // get filename
    m_xmlDoc.ResetChildPos();
    if (m_xmlDoc.FindElem(TAG_FILENAME))
    {
        groupBySite.filename = m_xmlDoc.GetData();
    }
}
