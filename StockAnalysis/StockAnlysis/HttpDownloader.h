#ifndef _HTTP_DOWNLOADER_
#define _HTTP_DOWNLOADER_

class HttpDownloader
{
public:
	HttpDownloader(void);
public:
	~HttpDownloader(void);

	bool download(const char* lpszUrl, const char* strSaveFile);
};


#endif