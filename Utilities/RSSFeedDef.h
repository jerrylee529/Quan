/*******************************************************************************
* (C) Copyright 2011 
* 
* These computer program listings and specifications are the property of Jerry 
* Lee and shall not be reproduced or copied or used in whole or in 
* part without written permission from Jerry Lee.
*
* Project:      IAdvertising 
* File Name:	RSSFeedDef.h 
* Programer(s):	Jerry Lee 
* Created:      20110104 
* Description:	RSS数据定义单元
* History:
*******************************************************************************/

#ifndef _RSS_FEED_DEF_H
#define _RSS_FEED_DEF_H

#include <vector>
#include <string>

using namespace std;


// RSS Header
#define RSS_HEADER          ("rss") // RSS头 

#define RSS_CHANNEL         ("channel") // channel

#define RSS_ITEM            ("item")  // item

// RSS Channel definition
#define CHAN_LANGUAGE       ("language") // 频道使用的语言种类，例如en-us，简体中文是zh-cn。它方便聚集器组织
                                            // 同一语言的站点。 您可以使W3C预定义的值
#define CHAN_COPYRIGHT      ("copyright")       // 频道内容的版权声明
#define CHAN_MEDITOR        ("managingEditor") // 对该频道内容负责的个人的Email 地址
#define CHAN_WEBMASTER      ("webMaster") // 对该频道的技术支持负责的个人的Email 地址
#define CHAN_PUBDATE        ("pubDate")   // 该频道内容的公布日期。例如，一个根据纽约时间按日更新的频道每24 小
                                             // 时公布日期就滚动一次。即该频道的pubDate更改的时间。所有RSS 中使
                                             // 用的日期时间遵守RFC 822 规范，年份可以是两位或者四位（首选四位）
#define CHAN_LASTBUILDDATE  ("lastBuildDate") // 上次频道内容更改的时间
#define CHAN_CATEGORY       ("category")  // 说明频道属于哪一个或多个分类，其规则和<item>级别的category元素
                                             // 一样
#define CHAN_GENERATOR      ("generator") // 说明用于生成该频道的程序
#define CHAN_DOCS           ("docs")      // RSS文件所使用格式的说明文档所在的URL。它可能指向本文档。它有助于
                                             // 让人理解该RSS文件。
#define CHAN_CLOUD          ("cloud")     // 允许进程注册为“cloud”，频道更新时通知它，为 RSS 提要实现了一种轻量
                                             // 级的发布-订阅协议
#define CHAN_TTL            ("ttl")       // ttl 代表存活时间，存活时间是一个数字，表示提要在刷新之前缓冲的分钟数
#define CHAN_IMAGE          ("image")     // 指定一个能在频道中显示的GIF、JPEG 或PNG 图像
#define CHAN_RATING         ("rating")    // 关于该频道的 PICS 评价
#define CHAN_TEXTINPUT      ("textInput") // 定义可与频道一起显示的输入框
#define CHAN_SKIPHOURS      ("skipHours") // 告诉聚集器哪些小时的更新可以忽略
#define CHAN_SKIPDAYS       ("skipDays")  // 告诉聚集器那一天的更新可以忽略

// RSS Item definition
#define ITEM_TITLE          ("title")       // 项（item）的标题
#define ITEM_LINK           ("link")        // 项（item）的URL
#define ITEM_DESC           ("description") // 项（item）的大纲
#define ITEM_AUTHOR         ("author")      // 项（item）作者的Email 地址
#define ITEM_CATEGORY       ("category")    // 包括项（item）的一个或多个分类
#define ITEM_COMMENTS       ("comments")    // 关于项（item）的注释页的URL
#define ITEM_ENCLOSURE      ("enclosure")   // 支持和该项（item）有关的媒体对象
#define ITEM_GUID           ("guid")        // 唯一与该项（item）联系在一起的永久性链接
#define ITEM_PUBDATE        ("pubDate")     // 该项（item）是什么时候发布的
#define ITEM_SOURCE         ("source")      // 该项（item）来自哪个RSS 频道，当把项（item）聚合在一起时非常有用

// RSS Attribute name
#define ATTIBUTE_URL        ("url")      
#define ATTIBUTE_LEGNTH     ("length")
#define ATTIBUTE_TYPE       ("type")
//

// below are standard MIME types
//扩展名    类型/子类型                 IIS 4.0     IIS 5.0 
// *        application/octet-stream    是          是 
// 323      text/h323                   否          是 
// acx      application/internet-property-stream 否 是 
// ai       application/postscript 是 是 
// aif      audio/x-aiff 是 是 
// aifc     audio/x-aiff 是 是 
// aiff     audio/x-aiff 是 是 
// asf      video/x-ms-asf 否 是 
// asr      video/x-ms-asf 否 是 
// asx      video/x-ms-asf 否 是 
// au       audio/basic 是 是 
// avi      video/x-msvideo 是 是 
// axs      application/olescript 否 是 
// bas      text/plain 是 是 
// bcpio    application/x-bcpio 是 是 
// bin      application/octet-stream 是 是 
// bmp      image/bmp 是 是 
// c        text/plain 是 是 
// cat      application/vnd.ms-pkiseccat 否 是 
// cdf      application/x-cdf 否 是 
// cer      application/x-x509-ca-cert 否 是 
// class    application/octet-stream 是 是 
// clp      application/x-msclip 是 是 
// cmx      image/x-cmx 是 是 
// cod      image/cis-cod 是 是 
// cpio     application/x-cpio 是 是 
// crd      application/x-mscardfile 是 是 
// crl      application/pkix-crl 否 是 
// crt      application/x-x509-ca-cert 否 是 
// csh      application/x-csh 是 是 
// css      text/css 否 是 
// dcr      application/x-director 是 是 
// der      application/x-x509-ca-cert 否 是 
// dir      application/x-director 是 是 
// dll      application/x-msdownload 否 是 
// dms      application/octet-stream 是 是 
// doc      application/msword 是 是 
// dot      application/msword 是 是 
// dvi      application/x-dvi 是 是 
// dxr      application/x-director 是 是 
// eps      application/postscript 是 是 
// etx      text/x-setext 是 是 
// evy      application/envoy 是 是 
// exe      application/octet-stream 是 是 
// fif      application/fractals 否 是 
// flr      x-world/x-vrml 是 是 
// gif      image/gif 是 是 
// gtar     application/x-gtar 是 是 
// gz       application/x-gzip 否 是 
// h        text/plain 是 是 
// hdf      application/x-hdf 是 是 
// hlp      application/winhlp 是 是 
// hqx      application/mac-binhex40 是 是 
// hta      application/hta 否 是 
// htc      text/x-component 否 是 
// htm      text/html 是 是 
// html     text/html 是 是 
// htt      text/webviewhtml 否 是 
// ico      image/x-icon 否 是 
// ief      image/ief 是 是 
// iii      application/x-iphone 否 是 
// ins      application/x-internet-signup 否 是 
// isp      application/x-internet-signup 否 是 
// jfif     image/pipeg 否 是 
// jpe      image/jpeg 是 是 
// jpeg     image/jpeg 是 是 
// jpg      image/jpeg 是 是 
// js       application/x-javascript 是 是 
// latex    application/x-latex 是 是 
// lha      application/octet-stream 是 是 
// lsf      video/x-la-asf 否 是 
// lsx      video/x-la-asf 否 是 
// lzh      application/octet-stream 是 是 
// m13      application/x-msmediaview 是 是 
// m14      application/x-msmediaview 是 是 
// m3u      audio/x-mpegurl 否 是 
// man      application/x-troff-man 是 是 
// mdb      application/x-msaccess 是 是 
// me       application/x-troff-me 是 是 
// mht      message/rfc822 否 是 
// mhtml    message/rfc822 否 是 
// mid      audio/mid 否 是 
// mny      application/x-msmoney 是 是 
// mov      video/quicktime 是 是 
// movie    video/x-sgi-movie 是 是 
// mp2      video/mpeg 是 是 
// mp3      audio/mpeg 否 是 
// mpa      video/mpeg 是 是 
// mpe      video/mpeg 是 是 
// mpeg     video/mpeg 是 是 
// mpg      video/mpeg 是 是 
// mpp      application/vnd.ms-project 是 是 
// mpv2     video/mpeg 是 是 
// ms       application/x-troff-ms 是 是 
// mvb      application/x-msmediaview 是 是 
// nws      message/rfc822 否 是 
// oda      application/oda 是 是 
// p10      application/pkcs10 否 是 
// p12      application/x-pkcs12 否 是 
// p7b      application/x-pkcs7-certificates 否 是 
// p7c      application/x-pkcs7-mime 否 是 
// p7m      application/x-pkcs7-mime 否 是 
// p7r      application/x-pkcs7-certreqresp 否 是 
// p7s      application/x-pkcs7-signature 否 是 
// pbm      image/x-portable-bitmap 是 是 
// pdf      application/pdf 是 是 
// pfx      application/x-pkcs12 否 是 
// pgm      image/x-portable-graymap 是 是 
// pko      application/ynd.ms-pkipko 否 是 
// pma      application/x-perfmon 是 是 
// pmc      application/x-perfmon 是 是 
// pml      application/x-perfmon 是 是 
// pmr      application/x-perfmon 是 是 
// pmw      application/x-perfmon 是 是 
// pnm      image/x-portable-anymap 是 是 
// pot      application/vnd.ms-powerpoint 是 是 
// ppm      image/x-portable-pixmap 是 是 
// pps      application/vnd.ms-powerpoint 是 是 
// ppt      application/vnd.ms-powerpoint 是 是 
// prf      application/pics-rules 否 是 
// ps       application/postscript 是 是 
// pub      application/x-mspublisher 是 是 
// qt       video/quicktime 是 是 
// ra       audio/x-pn-realaudio 是 是 
// ram      audio/x-pn-realaudio 是 是 
// ras      image/x-cmu-raster 是 是 
// rgb      image/x-rgb 是 是 
// rmi      audio/mid 否 是 
// roff     application/x-troff 是 是 
// rtf      application/rtf 是 是 
// rtx      text/richtext 是 是 
// scd      application/x-msschedule 是 是 
// sct      text/scriptlet 否 是 
// setpay   application/set-payment-initiation 否 是 
// setreg   application/set-registration-initiation 否 是 
// sh       application/x-sh 是 是 
// shar     application/x-shar 是 是 
// sit      application/x-stuffit 否 是 
// snd      audio/basic 是 是 
// spc      application/x-pkcs7-certificates 否 是 
// spl      application/futuresplash 否 是 
// src      application/x-wais-source 是 是 
// sst      application/vnd.ms-pkicertstore 否 是 
// stl      application/vnd.ms-pkistl 否 是 
// stm      text/html 是 是 
// sv4cpio  application/x-sv4cpio 是 是 
// sv4crc   application/x-sv4crc 是 是 
// t        application/x-troff 是 是 
// tar      application/x-tar 是 是 
// tcl      application/x-tcl 是 是 
// tex      application/x-tex 是 是 
// texi     application/x-texinfo 是 是 
// texinfo  application/x-texinfo 是 是 
// tgz      application/x-compressed 否 是 
// tif      image/tiff 是 是 
// tiff     image/tiff 是 是 
// tr       application/x-troff 是 是 
// trm      application/x-msterminal 是 是 
// tsv      text/tab-separated-values 是 是 
// txt      text/plain 是 是 
// uls      text/iuls 否 是 
// ustar    application/x-ustar 是 是 
// vcf      text/x-vcard 否 是 
// vrml     x-world/x-vrml 是 是 
// wav      audio/x-wav 是 是 
// wcm      application/vnd.ms-works 是 是 
// wdb      application/vnd.ms-works 是 是 
// wks      application/vnd.ms-works 是 是 
// wmf      application/x-msmetafile 是 是 
// wps      application/vnd.ms-works 是 是 
// wri      application/x-mswrite 是 是 
// wrl      x-world/x-vrml 是 是 
// wrz      x-world/x-vrml 是 是 
// xaf      x-world/x-vrml 是 是 
// xbm      image/x-xbitmap 是 是 
// xla      application/vnd.ms-excel 是 是 
// xlc      application/vnd.ms-excel 是 是 
// xlm      application/vnd.ms-excel 是 是 
// xls      application/vnd.ms-excel 是 是 
// xlt      application/vnd.ms-excel 是 是 
// xlw      application/vnd.ms-excel 是 是 
// xof      x-world/x-vrml 是 是 
// xpm      image/x-xpixmap 是 是 
// xwd      image/x-xwindowdump 是 是 
// z        application/x-compress 否 是 
// zip      application/zip 是 是 
// evy      application/envoy 是 是 
// fif      application/fractals  否 是 
// spl      application/futuresplash  否 是 
// hta      application/hta 否 是 
// acx      application/internet-property-stream 否 是 
// hqx      application/mac-binhex40 是 是 
// doc      application/msword 是 是 
// dot      application/msword 是 是 
// *        application/octet-stream 是 是 
// bin      application/octet-stream 是 是 
// class    application/octet-stream 是 是 
// dms      application/octet-stream 是 是 
// exe      application/octet-stream  是 是 
// lha      application/octet-stream 是 是 
// lzh      application/octet-stream 是 是 
// oda      application/oda 是 是 
// axs      application/olescript 否 是 
// pdf      application/pdf 是 是 
// prf      application/pics-rules 否 是 
// p10      application/pkcs10 否 是 
// crl      application/pkix-crl 否 是 
// ai       application/postscript 是 是 
// eps      application/postscript 是 是 
// ps       application/postscript 是 是 
// rtf      application/rtf 是 是 
// setpay   application/set-payment-initiation  否 是 
// setreg   application/set-registration-initiation  否 是 
// xla      application/vnd.ms-excel 是 是 
// xlc      application/vnd.ms-excel 是 是 
// xlm      application/vnd.ms-excel 是 是 
// xls      application/vnd.ms-excel  是 是 
// xlt      application/vnd.ms-excel 是 是 
// xlw      application/vnd.ms-excel 是 是 

#define TITLE_SIZE 128
#define URL_SIZE   128
#define DATE_SIZE  32
#define DESC_SIZE  512

#ifndef XML_HEADER
#define XML_HEADER       "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\r"
#endif

#define MEDIA_FORMAT_AUDIO  ("WMA,WAV,MP1,MP2,MP3,MPA,OGG,M4A,M4B")
#define MEDIA_FORMAT_VIDEO  ("MP4,MOV,3GP,K3G,MQV,WMV,AVI,DIVX,GVI,OGM,M4V,MPEG,MPG")


// RSS频道子项Category结构定义
typedef struct tagRSSCategory
{
    string domain;	// Category的domain URL
    string title;	// Category的名称

    tagRSSCategory::tagRSSCategory()
    {
        
    }

    tagRSSCategory::tagRSSCategory(const tagRSSCategory &category)
    {
        domain = category.domain;
        title = category.title;
    }

    tagRSSCategory &tagRSSCategory::operator=(const tagRSSCategory &category)
    {
        if (this != &category)
        {
            this->domain = category.domain;
            this->title = category.title;
        }

        return *this;
    }
} RSSCategory, * PRSSCategory;


// RSS频道子项Enclosure结构定义
typedef struct tagRSSEnclosure
{
    unsigned long length;   // length 描述它有多少字节
    string url;	            // 一个http URL
    string type;	        // 描述其类型(一个标准的MIME类型)

    tagRSSEnclosure::tagRSSEnclosure()
    {

    }

    tagRSSEnclosure::tagRSSEnclosure(const tagRSSEnclosure &enclosure)
    {
        length = enclosure.length;
        url = enclosure.url;
        type = enclosure.type;
    }

    tagRSSEnclosure &tagRSSEnclosure::operator=(const tagRSSEnclosure &enclosure)
    {
        if (this != &enclosure)
        {
            length = enclosure.length;
            url = enclosure.url;
            type = enclosure.type;
        }

        return *this;
    }
} RSSEnclosure, * PRSSEnclosure;


// RSS频道子项GUID结构定义
typedef struct tagRSSGUID
{
    bool isPermaLink;   // 是否为持久连接
    string url;	        // guid的URL

    tagRSSGUID::tagRSSGUID()
    {

    }

    tagRSSGUID::tagRSSGUID(const tagRSSGUID &guid)
    {
        isPermaLink = guid.isPermaLink;
        url = guid.url;
    }

    tagRSSGUID &tagRSSGUID::operator=(const tagRSSGUID &guid)
    {
        if (this != &guid)
        {
            isPermaLink = guid.isPermaLink;
            url = guid.url;
        }

        return *this;
    }
} RSSGUID, * PRSSGUID;


// RSS频道子项Source结构定义
typedef struct tagRSSSource
{
    string url;	    // item的标题
    string content;	// item的URL

    tagRSSSource::tagRSSSource()
    {
        
    }

    tagRSSSource::tagRSSSource(const tagRSSSource &source)
    {
        url = source.url;
        content = source.content;
    }

    tagRSSSource &tagRSSSource::operator=(const tagRSSSource &source)
    {
        if (this != &source)
        {
            url = source.url;
            content = source.content;
        }

        return *this;
    }
} RSSSource, * PRSSSource;


// RSS频道子项Image结构定义
typedef struct tagRSSImage
{
    string title;	// 图片的标题
    string url;	    // 图片的URL
    string link;	// 图片所在域的URL

    tagRSSImage::tagRSSImage()
    {

    }

    tagRSSImage::tagRSSImage(const tagRSSImage &image)
    {
        title = image.title;
        url = image.url;
        link = image.link;
    }

    tagRSSImage &tagRSSImage::operator=(const tagRSSImage &image)
    {
        if (this != &image)
        {
            title = image.title;
            url = image.url;
            link = image.link;
        }

        return *this;
    }
} RSSImage, * PRSSImage;


// RSS频道子项Cloud结构定义
typedef struct tagRSSCloud
{
    int port;		        // 该地址提供的端口
    string domain;	        // 检索频道通知的地址
    string path;	        // 该通知所在的路径
    string registerProc;    // 被调用的过程名
    string protocol;	    // 使用的协议名称

    tagRSSCloud::tagRSSCloud()
    {

    }

    tagRSSCloud::tagRSSCloud(const tagRSSCloud &cloud)
    {
        port = cloud.port;
        domain = cloud.domain;
        path = cloud.path;
        registerProc = cloud.registerProc;
        protocol = cloud.protocol;
    }

    tagRSSCloud &tagRSSCloud::operator=(const tagRSSCloud &cloud)
    {
        if (this != &cloud)
        {
            port = cloud.port;
            domain = cloud.domain;
            path = cloud.path;
            registerProc = cloud.registerProc;
            protocol = cloud.protocol;
        }

        return *this;
    }
} RSSCloud, * PRSSCloud;


// RSS频道子项textInput结构定义
typedef struct tagRSSTextInput
{
    string title;       // 输入字段的标题，例如Submit或Search
    string description; // 简短的输入字段的用途说明
    string name;        // 输入字段的名称
    string link;        // 处理输入字段请求的服务器CGI脚本的URL

    tagRSSTextInput::tagRSSTextInput()
    {

    }

    tagRSSTextInput::tagRSSTextInput(const tagRSSTextInput &textInput)
    {
        title = textInput.title;
        description = textInput.description;
        name = textInput.name;
        link = textInput.link;
    }

    tagRSSTextInput &tagRSSTextInput::operator=(const tagRSSTextInput &textInput)
    {
        if (this != &textInput)
        {
            title = textInput.title;
            description = textInput.description;
            name = textInput.name;
            link = textInput.link;
        }
        
        return *this;
    }
} RSSTextInput, * PRSSTextInput;


// RSS频道子项结构定义
typedef struct tagRSSItem
{
    string title;	        // item的标题
    string link;	        // item的URL
    string description;	    // item大纲
    string author;          // 与该项相关的作者的email地址
    string comments;        // 关于项目的的注释页的URL
    RSSCategory category;   // 包括该项在内的分类
    RSSEnclosure enclosure; // 支持和该item有关的媒体对象
    RSSGUID guid;		    // 唯一与该item联系在一起的永久性链接
    string pubDate;         // 该项目发布的时间
    RSSSource source;	    // 该item来自哪个RSS 频道，当把item聚合在一起时非常有用

    tagRSSItem::tagRSSItem()
    {

    }

    tagRSSItem::tagRSSItem(const tagRSSItem &item)
    {
        title = item.title;
        link = item.link;
        description = item.description;
        author = item.author;
        comments = item.comments;
        category = item.category;
        enclosure = item.enclosure;
        guid = item.guid;
        pubDate = item.pubDate;
        source = item.source;
    }

    tagRSSItem &tagRSSItem::operator=(const tagRSSItem &item)
    {
        if (this != &item)
        {
            title = item.title;
            link = item.link;
            description = item.description;
            author = item.author;
            comments = item.comments;
            category = item.category;
            enclosure = item.enclosure;
            guid = item.guid;
            pubDate = item.pubDate;
            source = item.source;
        }

        return *this;
    }
} RSSItem, * PRSSItem;


// RSS频道结构定义
typedef vector<RSSItem> VT_RSSITEM;

typedef struct tagRSSChannel
{
    string title;               // 频道的名称
    string link;                // 与改频道关联的web站点或者站点区域的URL
    string description;         // 对该频道的一段简要描述
    string language;	        // 频道使用的语言种类,如en-us，或者zh-cn,可以使用W3C预定义的值
    string copyright;           // 频道内容的版权声明
    string managingEditor;      // 对频道内容负责的个人的Email
    string pubDate;             // 改频道公布的日期
    string lastBuildDate;       // 上次频道内容更新的日期
    RSSCategory category;       // 该频道所属的分类，和item项的定义相同
    string generator;           // 说明用于生成该频道的程序
    string docs;	            // RSS文件所用格式的说明文档所在的URL
    RSSCloud cloud;		        // 允许进程注册为"cloud",频道更新时通知它,为RSS提要实现了一种轻量级的发布-订阅协议
    int ttl;	                // ttl 代表存活时间，存活时间是一个数字，表示提要在刷新之前缓冲的分钟数
    RSSImage image;		        // 指定一个能在频道中显示的GIF,JPEG 或PNG 图像
    int rating;		            // 关于该频道的 PICS 评价
    RSSTextInput textInput;	    // 定义可与频道一起显示的输入框
    int skipHours;	            // 告诉聚集器哪些小时的更新可以忽略
    int skipDays;		        // 告诉聚集器那一天的更新可以忽略
    VT_RSSITEM items;	        // 一个频道可以包括任意个数的item
                                // 一个item代表一个"故事"--就像一张报纸或者一份杂志上的一个故事

    tagRSSChannel::tagRSSChannel()
    {

    }

    tagRSSChannel::tagRSSChannel(const tagRSSChannel &channel)
    {
        title = channel.title;
        link = channel.link;
        description = channel.description;
        language = channel.language;
        copyright = channel.copyright;
        managingEditor = channel.managingEditor;
        pubDate = channel.pubDate;
        lastBuildDate = channel.lastBuildDate;
        category = channel.category;
        generator = channel.generator;
        docs = channel.docs;
        cloud = channel.cloud;
        ttl = channel.ttl;
        image = channel.image;
        rating = channel.rating;
        textInput = channel.textInput;
        skipDays = channel.skipDays;
        skipHours = channel.skipHours;

        items.clear();
        items.assign(channel.items.begin(), channel.items.end());
    }
    
    tagRSSChannel &tagRSSChannel::operator=(const tagRSSChannel &channel)
    {
        if (this != &channel)
        {
            title = channel.title;
            link = channel.link;
            description = channel.description;
            language = channel.language;
            copyright = channel.copyright;
            managingEditor = channel.managingEditor;
            pubDate = channel.pubDate;
            lastBuildDate = channel.lastBuildDate;
            category = channel.category;
            generator = channel.generator;
            docs = channel.docs;
            cloud = channel.cloud;
            ttl = channel.ttl;
            image = channel.image;
            rating = channel.rating;
            textInput = channel.textInput;
            skipDays = channel.skipDays;
            skipHours = channel.skipHours;

            items.clear();
            items.assign(channel.items.begin(), channel.items.end());
        }

        return *this;
    }
} RSSChannel, * PRSSChannel;


// RSS结构定义
typedef struct tagRSSFeed
{
    char version[4];
    RSSChannel channel;

    tagRSSFeed::tagRSSFeed()
    {

    }

    tagRSSFeed::tagRSSFeed(const tagRSSFeed &feed)
    {
        memcpy(version, feed.version, sizeof(version));
        channel = feed.channel;
    }

    tagRSSFeed &tagRSSFeed::operator=(const tagRSSFeed &feed)
    {
        if (this != &feed)
        {
            memcpy(version, feed.version, sizeof(version));
            channel = feed.channel;
        }

        return *this;
    }
 } RSSFeed, * PRSSFeed;


// RSS Feed读取器接口
class RSSFeedReader
{
public:
    virtual bool Read(const string &filename, RSSFeed &feed) = 0;

protected:
    virtual void ReadChannel(RSSChannel &channel) = 0;
    virtual void ReadItem(RSSItem &item) = 0;
    virtual void ReadCategory(RSSCategory &category) = 0;
    virtual void ReadEnclosure(RSSEnclosure &enclosure) = 0;
    virtual void ReadGuid(RSSGUID &guid) = 0;
    virtual void ReadImage(RSSImage &image) = 0;
    virtual void ReadCloud(RSSCloud &cloud) = 0;
    virtual void ReadSource(RSSSource &source) = 0;
};

// RSS Feed写入器接口
class RSSFeedWriter
{
public:
    virtual bool Write(const string &filename, const RSSFeed &feed) = 0;

protected:
    virtual void WriteChannel(const RSSChannel &channel) = 0;
    virtual void WriteItem(const RSSItem &item) = 0;
    virtual void WriteCategory(const RSSCategory &category) = 0;
    virtual void WriteEnclosure(const RSSEnclosure &enclosure) = 0;
    virtual void WriteGuid(const RSSGUID &guid) = 0;
    virtual void WriteImage(const RSSImage &image) = 0;
    virtual void WriteCloud(const RSSCloud &cloud) = 0;
    virtual void WriteSource(const RSSSource &source) = 0;
};


#endif // _RSS_FEED_DEF_H
