/*
This file is:
The zlib/libpng License
Copyright (c) 2012 freesftys<freesftys@gmail.com>

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software in
       a product, an acknowledgment in the product documentation would be
       appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.

http://opensource.org/licenses/Zlib

http://sourceforge.jp/projects/opensource/wiki/licenses%2Fzlib_libpng_license
*/


#ifndef INI_H
#define INI_H

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define NUMDIGIT    64
class Ini
{
    private:
        TCHAR path[MAX_PATH];
        const TCHAR *fileName;
        TCHAR fullPath[MAX_PATH];

        Ini();
        ~Ini();

    public:
        static Ini *getInstance()
        {
            static Ini _ini;
            return &_ini;
        }

        TCHAR *getPath()
        {
            return this->path;
        };
        const TCHAR *getFileName()
        {
            return this->fileName;
        };
        TCHAR *getFullPath()
        {
            return this->fullPath;
        };

        int inited;
        int setIniPath( const TCHAR *path );
        int checkIniFile();
        int defaultIni();
        int writeDate( const TCHAR *section, const TCHAR *key, int data );
        int writeDate( const TCHAR *section, const TCHAR *key, float data );
        int writeDate( const TCHAR *section, const TCHAR *key, const TCHAR *data );
        int readDate( const TCHAR *section, const TCHAR *key, TCHAR *data,
                      int dataSize );
        int readDate( const TCHAR *section, const TCHAR *key, int *data );
        int readDate( const TCHAR *section, const TCHAR *key, float *data );
};

#endif