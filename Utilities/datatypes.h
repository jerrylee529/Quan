
#ifndef _DATA_TYPES_H
#define _DATA_TYPES_H

#include <string>

using namespace std;

#ifdef _UNICODE
#define STRING wstring
#else
#define STRING string
#endif

typedef TCHAR           TCHAR32[32];
typedef unsigned int    YLUINT;
typedef int             YLINT;

#define YLNULL 0


#endif