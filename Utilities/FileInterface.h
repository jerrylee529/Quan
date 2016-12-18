/*******************************************************************************
* (C) Copyright 2011 
* 
* These computer program listings and specifications are the property of Jerry 
* Lee and shall not be reproduced or copied or used in whole or in 
* part without written permission from Jerry Lee.
*
* Project:      IAdvertising 
* File Name:	FileInterface.h 
* Programer(s):	Jerry Lee 
* Created:      20110104 
* Description:	定义文件操作的接口
* History:
*******************************************************************************/

#ifndef _FILE_INTERFACE_H
#define _FILE_INTERFACE_H

// 读取文件接口
template<typename T>
class FileReader
{
public:
    void Read(T &value) = 0;
protected:
private:
};


// 写入文件接口
template<typename T>
class FileWriter
{
public:
    void Write(T &value) = 0;
protected:
private:
};

#endif // _FILE_INTERFACE_H