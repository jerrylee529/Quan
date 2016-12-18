/*******************************************************************************
* (C) Copyright 2008 
* 
*
* Project:      IViewer 
* File Name:	JLFuncs.h 
* Programer(s):	Jerry Lee 
* Created:      20080310 
* Description:	define the functions for common use
* History: 
* 2008.3.14: added the calcrectwidth and calcrectheight functions
* July 21 2008, Jerry Lee, added the function of DrawCustomText, FindProcess
*                          FindMainThread, SuspendAllThreads, SuspendProcess,
*                          ConvertTime           
* July 25 2008, Jerry Lee, added the function of DrawBitmap
* Aug 28 2008, Jerry Lee, add the function fo DrawTextInFixRectEx
* Sep 30 2008, Jerry Lee, add the function for getting week day by specified year, month, day.
*******************************************************************************/

#if !defined(_JLFuncs_H)
#define _JLFuncs_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xstring"

using namespace std;

// get the left of rect
#define _X(x)	(x.left)
// get the top of rect
#define _Y(x)	(x.top)
// get the width of rect
#define _W(x)	(x.right - x.left)
// get the height of rect
#define _H(x)	(x.bottom - x.top)
// get the length of string
#define _LEN(x) (sizeof((x))/sizeof((x[0])))

// resolve the problem about the cycle freezing 
extern void ProcessMessages();

// Update the window safely
extern void SafeInvalidate(CWnd *pWnd, BOOL bEraseBkgnd = FALSE);

// Unsubclass Window safely
extern void SafeUnsubclassWindow(CWnd &wnd);

//**************************************************************************************
//
//	GetBkBrush( HWND hWnd, UINT nID, HBITMAP hBmBk )
//  
//	Creates Background brush for the control specified by nID
//	according to its position in the parent dialog window.
//
//	hWnd [in]: Handle to the parent window
//  nID [in]: Control ID
//  hBmBk [in]: Bitmap handle of the parent dialog's background image
//
//
//**************************************************************************************
extern HBRUSH GetBkBrushFromWnd(HWND hWnd, UINT nID, HBITMAP hBmBk);

//**************************************************************************************
//
//	GetBkBrush( HWND hWnd, UINT nID, HBITMAP hBmBk )
//  
//	Creates Background brush for the control specified by nID
//	according to its position in the parent dialog window.
//
//	hWnd [in]: Handle to the parent window
//  nID [in]: Control ID
//  hBmBk [in]: Bitmap handle of the parent dialog's background image
//
//
//**************************************************************************************
extern HBRUSH GetBkBrushByPos(HWND hWnd, UINT nID, HBITMAP hBmBk, const POINT &point,
                              int nWidth, int nHeight);

//**************************************************************************************
//
//	DrawThumbImage(HDC hDC, HBITMAP hBitmap, const RECT &rect)
//  
//	Draw Thumb Image for the control specified by rect
//	according to hBitmap
//
//	hDC [in]: dc to the window
//  hBitmap [in]: bitmap handle
//  rect [in]: the rect that need to be draw
//
//
//**************************************************************************************
extern void DrawThumbImage(HDC hDC, HBITMAP hBitmap, const RECT &rect);

//**************************************************************************************
//
//	SetStringValue(LPTSTR lpszDesc, int nDescLen, LPCTSTR lpszSrc)
//  
//	Set string value to a string pointer
//	according to hBitmap
//
//	lpszDesc [in]: specifies the pointer to destination string
//  nDescLen [in]: specifies the buffer size of destination string
//  lpszSrc [in]: specifies the source string
//
//
//**************************************************************************************
extern void SetStringValue(LPTSTR lpszDesc, int nDescLen, LPCTSTR lpszSrc);

//**************************************************************************************
//
//	SafePostMessage(CWnd *pWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//  
//	Post message to pWnd safely
//
//	pWnd [in]: specifies the window pointer
//  Msg [in]: specifies the message id
//  wParam [in]: specifies the WPARAM
//  lParam [in]: specifies the LPARAM
//
//
//**************************************************************************************
extern void SafePostMessage(CWnd *pWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

//**************************************************************************************
//
//	PasrePageURL(TCHAR *data)
//  
//	extract the page url from web page
//
//	data [in]: specifies the source code of web page
//**************************************************************************************
extern BOOL PasrePageURL(TCHAR *data);

//**************************************************************************************
//
//	PasreHTMLURL(TCHAR *data)
//  
//	extract the html url from web page
//
//	data [in]: specifies the source code of web page
//**************************************************************************************
extern BOOL PasreHTMLURL(TCHAR *data);

//**************************************************************************************
//
//	PasreJPGURL(TCHAR *data)
//  
//	extract the image url from web page
//
//	lpszDesc [in]: specifies the pointer to jpg url array
//	nLen [in]: specifies the buffer size of the pointer to one piece of jpg url
//	lpszSrc [in]: specifies the pointer to source code of web page
//  return: specifies the pointer to remained string
//**************************************************************************************
extern LPTSTR PasreJPGURL(LPTSTR lpszDesc, int nLen, LPCTSTR lpszSrc);

//**************************************************************************************
//
//	GetLocalFullFilePath(LPTSTR lpszPath, int nSize, LPCTSTR lpszFile)
//  
//	retrieves a full path of file specified by lpszFile
//
//	lpszPath [in]: specifies the pointer to full file path
//	nSize [in]: specifies the buffer size of the pointer to the full file path
//	lpszFile [in]: specifies the pointer to file name
//**************************************************************************************
void GetLocalFullFilePath(LPTSTR lpszPath, int nSize, LPCTSTR lpszFile);

//**************************************************************************************
//
//	ExtractFileName(LPTSTR lpszFileName, int nSize, LPCTSTR lpszPath)
//  
//	extract a file name from a full path
//
//	lpszFileName [in]: specifies the file name
//	nSize [in]: specifies the buffer size of the pointer to a file name
//	lpszPath [in]: specifies the full path of a file
//**************************************************************************************
void ExtractFileName(LPTSTR lpszFileName, int nSize, LPCTSTR lpszPath);

//**************************************************************************************
//
//	CopyFileToDir(LPTSTR lpszNew, int nSize, LPCTSTR lpszDir, LPCTSTR lpszOld)
//  
//	extract a file name from a full path
//
//	lpszNew [in]: specifies the destination file name
//	nSize [in]: specifies the buffer size of the pointer to a file name
//  lpszDir [in]: specifies the directory that copied to
//	lpszOld [in]: specifies the source file name
//**************************************************************************************
BOOL CopyFileToDir(LPTSTR lpszNew, int nSize, LPCTSTR lpszDir, LPCTSTR lpszOld);

//**************************************************************************************
//
//	FileExists(LPCTSTR lpszFile)
//  
//	determinates whether the file is existing.
//
//	lpszFile [in]: specifies the full path of a file
//  return: return TRUE if the file exists, otherwise is FALSE.
//**************************************************************************************
BOOL FileExists(LPCTSTR lpszFile);

//**************************************************************************************
//
//	GetFileTimeSpan(LPCTSTR lpszFile)
//  
//	computes the time span between creation time and current time of a file
//
//	lpszFile [in]: specifies the full path of a file
//  dwMode [in]: indicates the compare mode of time, 0: create 1: last access 2: last write
//  return: return the days if it's successful, otherwise returns -1.
//**************************************************************************************
int GetFileTimeSpan(LPCTSTR lpszFile, DWORD dwMode = 2);

//**************************************************************************************
//
//	ConvertTimeByTimeZone(LPCTSTR lpszFile)
//  
//	computes the GMT time according to the time zone
//
//  st [in]: specifies the time that needs to convert into GMT time
//	lpszZone [in]: specifies the time zone, just like "PDT"
//  return: return coverted time, if failed, it returns the st.
//**************************************************************************************
SYSTEMTIME ConvertTimeByTimeZone(SYSTEMTIME &st, LPCTSTR lpszZone);


//**************************************************************************************
//
//	DrawCircle(HDC hDC, const POINT &point, HINSTANCE hRes, UINT uID)
//  
//	draws the circle by cycle
//
//  hDC [in]: specifies the handle to DC that would be drawed
//  point [in]: specfies the position that displays circle
//  hRes [in]: specifies the handle to resource 
//	uID [in]: specifies the bitmap id of circle
//**************************************************************************************
void DrawCircle(HDC hDC, const POINT &point, HINSTANCE hRes, UINT uID);


//**************************************************************************************
//
//	DrawCustomText(HDC hDC, const POINT &point, HINSTANCE hRes, UINT uID)
//  
//	draws the custom text with specified font
//
//  hDC [in]: Handle to device context
//  lpFont [in]: Handle to LOGFONT
//  lpszText [in]: Long pointer to the string that you want to draw 
//	lpRect [in]: Long pointer to a RECT structure that contains the rectangle, 
//             in logical coordinates, in which you want to format the text.
//  crText [in]: specified the color of text 
//	uFormat [in]: Unsigned integer that specifies the method of formatting the text.
//  nBkMode [in]: indicated the text mode that used to draw the text
//**************************************************************************************
void DrawCustomText(HDC hDC, LOGFONT *lpFont, LPCTSTR lpszText, LPRECT lpRect, 
                    COLORREF crText, UINT uFormat = DT_LEFT|DT_VCENTER, 
                    int nBkMode = TRANSPARENT);


//**************************************************************************************
//
//	FindProcess(LPCTSTR lpszProcess)
//  
//	finds the process by the specified executive file name of process
//
//  lpszProcess [in]: Long pointer to the process executive file name
//**************************************************************************************
DWORD FindProcess(LPCTSTR lpszProcess);


//**************************************************************************************
//
//	FindMainThread(DWORD dwProcessID)
//  
//	finds the main thread by the specified process identifier
//
//  dwProcessID [in]: The process id
//**************************************************************************************
DWORD FindMainThread(DWORD dwProcessID);


//**************************************************************************************
//
//	SuspendAllThreads()
//  
//	Suspend all the threads of current process exclude the main thread
//  bSuspend [in]: True presents activates the thread, otherwise deactivates the thread.
//  
//**************************************************************************************
void SuspendAllThreads(BOOL bSuspend);


//**************************************************************************************
//
//	SuspendProcess()
//  
//	Suspend the process
//
//  dwProcessID [in]: suspends the process
//  
//**************************************************************************************
void SuspendProcess(DWORD dwProcessID);

//**************************************************************************************
//
//	ConvertTime(LPCTSTR lpszStr)
//  
//	Convert a rss time to the system time
//
//  lpszStr [in]: the rss time string
//  
//**************************************************************************************
SYSTEMTIME ConvertTime(LPCTSTR lpszStr);

//**************************************************************************************
//
//	DrawBitmap(HDC hDC, const RECT &rect, HBITMAP hBitmap)
//  
//	Draws image that specified by uFormat on the position that specified by nx and ny
//
//	hDC [in]: Specified the device context
//  rect [in]: specified the rect that will be drawed
//  hBitmap [in]: specified the handle of bitmap 
//
//**************************************************************************************
void DrawBitmap(HDC hDC, const RECT &rect, HBITMAP hBitmap);

//**************************************************************************************
//
//	DrawTextInFixedRect(HDC hDC, const RECT &rect, HBITMAP hBitmap)
//  
//	draws the custom text with specified font in fixed rect
//  the part that can not display would be filled in suspension points
//
//  hDC [in]: Handle to device context
//  lpFont [in]: Handle to LOGFONT
//  lpszText [in]: Long pointer to the string that you want to draw 
//	lpRect [in]: Long pointer to a RECT structure that contains the rectangle, 
//             in logical coordinates, in which you want to format the text.
//  crText [in]: specified the color of text 
//	uFormat [in]: Unsigned integer that specifies the method of formatting the text.
//  nBkMode [in]: indicated the text mode that used to draw the text
//**************************************************************************************
void DrawTextInFixedRect(HDC hDC, LOGFONT *lpFont, LPCTSTR lpszText, LPRECT lpRect, 
                         COLORREF crText, UINT uFormat = DT_LEFT|DT_VCENTER, 
                         int nBkMode = TRANSPARENT);

//**************************************************************************************
//
//	DrawTextInFixedRect(HDC hDC, const RECT &rect, HBITMAP hBitmap)
//  
//	draws the custom text with specified font in fixed rect
//  the part that can not display would be filled in suspension points
//
//  hDC [in]: Handle to device context
//  hFont [in]: Handle to font
//  lpszText [in]: Long pointer to the string that you want to draw 
//	lpRect [in]: Long pointer to a RECT structure that contains the rectangle, 
//             in logical coordinates, in which you want to format the text.
//  crText [in]: specified the color of text 
//	uFormat [in]: Unsigned integer that specifies the method of formatting the text.
//  nBkMode [in]: indicated the text mode that used to draw the text
//**************************************************************************************
void DrawTextInFixedRectEx(HDC hDC, HFONT hFont, LPCTSTR lpszText, LPRECT lpRect, 
                         COLORREF crText, UINT uFormat = DT_LEFT|DT_VCENTER, 
                         int nBkMode = TRANSPARENT);

//**************************************************************************************
//
//	SleepEx(BOOL bTerminate, DWORD dwMilliseconds)
//  
//	this function suspends the execution of current thread for a specified interval
//  or terminates by external thread 
//
//  pTerminate [in]: determines whether wake up immediately from sleep
//  dwMilliseconds [in]: specified the interval for sleeping
//**************************************************************************************
void SleepEx(BOOL *pTerminate, DWORD dwMilliseconds);


//**************************************************************************************
//
//	GetWeekDay(BOOL bTerminate, DWORD dwMilliseconds)
//  
//	this function retrieves the day of the week by the specified 
//  year and month and day.
//
//  year [in]: indicates the year. 
//  month [in]: indicates the month. 
//  day [in]: indicates the day.
//  return value: specifies the day of the week, Sunday = 0, Monday = 1.
//**************************************************************************************
int GetWeekDay(int year, int month, int day);

//**************************************************************************************
//
//	Convert24HTo12H(int nHour)
//  
//	this function converts the 12 hour to 24 hour format.
//
//  uHour [in]: specified the hour.
//  uRetHour [out]: return the hours by 12 hour format. 
//  dwAMPM [out]: return the time is AM or PM.
//  return value: true means successful, otherwise false.
//**************************************************************************************
bool Convert24HTo12H(UINT uHour, UINT &uRetHour, DWORD &dwAMPM);

//**************************************************************************************
//
//	Convert12HTo24H(int nHour, DWORD dwFormat)
//  
//	this function converts the 12 hour to 24 hour format.
//
//  uHour [in]: specified the hour. 
//  dwAMPM [in]: specified the AM or PM, 0: AM 1: PM
//  uRetHour [out]: return the hours by 24 hour format.
//  return value: 
//**************************************************************************************
bool Convert12HTo24H(UINT uHour, DWORD dwAMPM, UINT &uRetHour);

//**************************************************************************************
//
//	newGUID()
//  
//	this function creates a new GUID.
//
//  return value: a new GUID string.
//**************************************************************************************
const TCHAR* newGUID();


//**************************************************************************************
//
//	CancelTouchCalibrate()
//  
//	this function cancels touch calibration
//
//  return value: None.
//**************************************************************************************
//void CancelTouchCalibrate();

//**************************************************************************************
//
//	GetInterval()
//  
//	this function retrieves the interval between two time in second.
//  st1 [in]: indicates the subtrahend time.
//  st2 [in]: indicates the minuend time.
//  return value: seconds between two time.
//**************************************************************************************
LONGLONG GetInterval(SYSTEMTIME st1, SYSTEMTIME st2);



#endif