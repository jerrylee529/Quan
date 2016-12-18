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

#ifndef _RSS_FEED_H
#define _RSS_FEED_H

#include "RSSFeedDef.h"
#include "Markup.h"



//
class CRSSFeedGenerator
{
public:
    void Load(const string &filename);

    void Save(const string &filename);

    void AddItem(int index);

    void DelItem(int index);

    int GetItemCount()
    {
        return static_cast<int>(m_rssFeed.channel.items.size());
    }

public:
    RSSFeed &GetFeed();

protected:
    

private:
    RSSFeed m_rssFeed;
};


#endif // _RSS_FEED_H