#pragma once

#include <string>

using namespace std;

class HistoryDataDownloader
{
public:
	HistoryDataDownloader(void);
public:
	~HistoryDataDownloader(void);

	void download(const char* market, const char* code);

    string getDownloadFile()
	{
		return m_strDownloadFile;
	}

private:
	string m_strDownloadFile;
};
