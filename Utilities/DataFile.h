
#ifndef _DATA_FILE_
#define _DATA_FILE_

#include <vector>
#include <fstream>
#include "datatypes.h"

using namespace std;

//
class DataFile
{
public:
    DataFile()
    {

    }
    ~DataFile()
    {

    }

public:
    void Open(STRING &fileName)
    {
        m_fin.open(fileName.c_str(), ios_base::binary|ios_base::in);
    }

    void Close()
    {
        if (IsOpen())
        {
            m_fin.close();
            m_fin.clear();
        }
    }

    bool IsOpen()
    {
        return m_fin.is_open();
    }

    bool IsEof()
    {
        return m_fin.eof();
    }

    template<class T>
    bool Read(T *buf, int size = sizeof(T))
    {
        return m_fin.read((char *)buf, size) != NULL?true:false;
    }

private:
    ifstream m_fin;
};

#endif
