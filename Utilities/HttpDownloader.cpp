#include "StdAfx.h"
#include "HttpDownloader.h"
#include "shlwapi.h"
#include "shlobj.h"
#include "FrameworkCls.h"
#include <fstream>

using namespace fmkcls;
using namespace std;

CHttpDownloader::CHttpDownloader()
{
}

CHttpDownloader::~CHttpDownloader()
{
}

bool CHttpDownloader::Download()
{
    if (!ParseUrl())
    {
        return false;
    }

    bool bRet = false;

    //Create the Internet session handle
    HINTERNET hInternetSession = ::InternetOpen("IAdv", INTERNET_OPEN_TYPE_PRECONFIG, 
        NULL, NULL, 0);
    if (hInternetSession == NULL)
    {
        //TRACE(_T("Failed in call to InternetOpen, Error:%d\n"), ::GetLastError());
        // HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_GENERIC_ERROR);
        return bRet; 
    }

    //Setup the status callback function
    if (::InternetSetStatusCallback(hInternetSession, DoOnStatusCallBack) == INTERNET_INVALID_STATUS_CALLBACK)
    {
        //TRACE(_T("Failed in call to InternetSetStatusCallback, Error:%d\n"), ::GetLastError());
        //HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_GENERIC_ERROR);
        //return bRet;
        ::InternetCloseHandle(hInternetSession);
        return bRet;
    }

    //Make the connection to the HTTP server     
    HINTERNET hHttpConnection = NULL;

    if (!m_user.empty())
    {
        hHttpConnection = ::InternetConnect(hInternetSession, m_server.c_str(), 
            m_port, m_user.c_str(), m_password.c_str(), m_dwServiceType, 0, (DWORD)this);
    }
    else
    {
        hHttpConnection = ::InternetConnect(hInternetSession, m_server.c_str(), 
            m_port, NULL, NULL, m_dwServiceType, 0, (DWORD)this);
    }

    if (hHttpConnection == NULL)
    {
        //TRACE(_T("Failed in call to InternetConnect, Error:%d\n"), ::GetLastError());
        //HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_FAIL_CONNECT_SERVER);
        ::InternetCloseHandle(hInternetSession);
        return bRet;
    }

    //Issue the request to read the file
    LPCTSTR ppszAcceptTypes[2];
    ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
    ppszAcceptTypes[1] = NULL;
    HINTERNET hHttpFile;
    hHttpFile = HttpOpenRequest(hHttpConnection, NULL, m_object.c_str(), 
        NULL, NULL, ppszAcceptTypes, 
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION, 
        (DWORD)this);
    if (hHttpFile == NULL)
    {
        //TRACE(_T("Failed in call to HttpOpenRequest, Error:%d\n"), ::GetLastError());
        //HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_FAIL_CONNECT_SERVER);
        ::InternetCloseHandle(hHttpConnection);
        ::InternetCloseHandle(hInternetSession);
        return bRet;
    }

    //label used to jump to if we need to resend the request
resend:

    //Issue the request
    if (!::HttpSendRequest(hHttpFile, NULL, 0, NULL, 0))
    {
        //TRACE(_T("Failed in call to HttpSendRequest, Error:%d\n"), ::GetLastError());
        //HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_FAIL_CONNECT_SERVER);
        ::InternetCloseHandle(hHttpConnection);
        ::InternetCloseHandle(hHttpFile);
        ::InternetCloseHandle(hInternetSession);

        return bRet;
    }

    //Check the HTTP status code
    TCHAR szStatusCode[32];
    DWORD dwInfoSize = 32;
    if (!HttpQueryInfo(hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
    {
        //TRACE(_T("Failed in call to HttpQueryInfo for HTTP query status code, Error:%d\n"), ::GetLastError());
        //HandleThreadError(IDS_HTTPDOWNLOAD_INVALID_SERVER_RESPONSE);
        ::InternetCloseHandle(hHttpFile);
        ::InternetCloseHandle(hHttpConnection);
        ::InternetCloseHandle(hInternetSession);

        return bRet;
    }
    else
    {
        long nStatusCode = _ttol(szStatusCode);

        //Handle any authentication errors
        if (nStatusCode == HTTP_STATUS_PROXY_AUTH_REQ || nStatusCode == HTTP_STATUS_DENIED)
        {
            // We have to read all outstanding data on the Internet handle
            // before we can resubmit request. Just discard the data.
            char szData[51];
            DWORD dwSize;
            do
            {
                ::InternetReadFile(hHttpFile, (LPVOID)szData, 50, &dwSize);
            }
            while (dwSize != 0);

            //Bring up the standard authentication dialog
            if (::InternetErrorDlg(NULL, hHttpFile, ERROR_INTERNET_INCORRECT_PASSWORD, 
                FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
                FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | 
                FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL) == ERROR_INTERNET_FORCE_RETRY)
            {
                goto resend;
            }
        }
        else if (nStatusCode != HTTP_STATUS_OK)
        {
            //TRACE(_T("Failed to retrieve a HTTP 200 status, Status Code:%d\n"), nStatusCode);
            //HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_INVALID_HTTP_RESPONSE, nStatusCode);
            ::InternetCloseHandle(hHttpFile);
            ::InternetCloseHandle(hHttpConnection);
            ::InternetCloseHandle(hInternetSession);

            return bRet;
        }
    }

    // Get the length of the file.            
    TCHAR szContentLength[32];
    dwInfoSize = 32;
    DWORD dwFileSize = 0;
    BOOL bGotFileSize = FALSE;
    if (::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szContentLength, 
        &dwInfoSize, NULL))
    {
        //Set the progress control range
        bGotFileSize = TRUE;
        dwFileSize = (DWORD) _ttol(szContentLength);
    }

    //Now do the actual read of the file
    DWORD dwStartTicks = ::GetTickCount();
    DWORD dwCurrentTicks = dwStartTicks;
    DWORD dwBytesRead = 0;
    char szReadBuf[1024];
    DWORD dwBytesToRead = 1024;
    DWORD dwTotalBytesRead = 0;
    DWORD dwLastTotalBytes = 0;
    DWORD dwLastPercentage = 0;
    ofstream fileToWrite;
    fileToWrite.open(m_destFile.c_str(), ios_base::out|ios_base::binary|ios_base::trunc);

    do
    {
        if (::InternetReadFile(hHttpFile, szReadBuf, dwBytesToRead, &dwBytesRead))
        {
            //Write the data to file
            try
            {
                fileToWrite.write(szReadBuf, dwBytesRead);
            }
            catch(...)                                      
            {
                return bRet; 
            }

            //Increment the total number of bytes read
            dwTotalBytesRead += dwBytesRead;  
        }
        else if (dwBytesRead)
        {
            //TRACE(_T("Failed in call to InternetReadFile, Error:%d\n"), ::GetLastError());
            //HandleThreadErrorWithLastError(IDS_HTTPDOWNLOAD_ERROR_READFILE);
            ::InternetCloseHandle(hHttpFile);
            ::InternetCloseHandle(hHttpConnection);
            ::InternetCloseHandle(hInternetSession);

            return bRet;
        }
    }
    while (dwBytesRead);

    //fclose(fileToWrite);
    fileToWrite.clear();
    fileToWrite.close();

    ::InternetCloseHandle(hHttpFile);
    ::InternetCloseHandle(hHttpConnection);
    ::InternetCloseHandle(hInternetSession);

    return true;
}

void CALLBACK CHttpDownloader::DoOnStatusCallBack(HINTERNET hInternet, DWORD dwContext, 
                                                 DWORD dwInternetStatus, 
                                                 LPVOID lpvStatusInformation, 
                                                 DWORD dwStatusInformationLength)
{
    //Convert from the SDK C world to the C++ world
    CHttpDownloader *pDlg = (CHttpDownloader *)dwContext;
    //pDlg->OnStatusCallBack(hInternet, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
    switch (dwInternetStatus)
    {
    case INTERNET_STATUS_RESOLVING_NAME:
        {
            break;
        }
    case INTERNET_STATUS_NAME_RESOLVED:
        {
            break;
        }
    case INTERNET_STATUS_CONNECTING_TO_SERVER:
        {
            break;
        }
    case INTERNET_STATUS_CONNECTED_TO_SERVER:
        {
            break;
        }
    case INTERNET_STATUS_REDIRECT:
        {
            break;
        }
    default:
        {
            break;
        }
    }
}

bool CHttpDownloader::ParseUrl()
{
#if 0
    CString strObj, strSvr, strPort;
    if (AfxParseURL(m_url.c_str(), m_dwServiceType, strSvr, strObj, m_port))
    {
        m_server = strSvr;
        m_object = strObj;
        return true;
    }
    else
    {
        return false;
    }
#endif

    URLParser parser(m_url.c_str());
    string str = parser.getprotocol();
    if (stricmp(str.c_str(), "ftp") == 0)
    {
        m_dwServiceType = INTERNET_SERVICE_FTP;
    }
    else if (stricmp(str.c_str(), "gopher") == 0)
    {
        m_dwServiceType = INTERNET_SERVICE_GOPHER;
    } 
    else if (stricmp(str.c_str(), "http") == 0)
    {
        m_dwServiceType = INTERNET_SERVICE_HTTP;
    }
    else
    {
        return false;
    }

    m_server = parser.gethost();

    m_file = parser.getfile();

    m_object = parser.getpath() + parser.getfile();

    int port = atoi(parser.getport().c_str());
    m_port = (port == 0)?80:port; 

    return true;
}