#pragma once

#ifndef _HTTP_DOWNLOADER_H
#define _HTTP_DOWNLOADER_H

#include "Wininet.h"
#include <string>

using namespace std;

/*
HTTPœ¬‘ÿ¿‡

*/
class CHttpDownloader
{
public:
    CHttpDownloader();
    ~CHttpDownloader();

    bool Download();

public:
    void SetUser(const string &value)
    {
        m_user = value;
    }

    void SetPassword(const string &value)
    {
        m_password = value;
    }

    void SetUrl(const string &value)
    {
        m_url = value;
    }

    void SetDestFile(const string &value)
    {
        m_destFile = value;
    }

    string GetFile()
    {
        return m_file;
    }

private:
    static void CALLBACK DoOnStatusCallBack(HINTERNET hInternet, 
        DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, 
        DWORD dwStatusInformationLength);

    bool ParseUrl();

    string  m_user;
    string  m_password;
    string  m_url;
    string  m_destFile;
    string  m_server;
    string  m_object;
    string  m_file;
    DWORD   m_dwServiceType;
    INTERNET_PORT   m_port;
};


#endif // _HTTP_DOWNLOADER_H
