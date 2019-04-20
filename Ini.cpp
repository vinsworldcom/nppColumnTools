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

#include "Ini.h"

extern TCHAR NPP_PLUGIN_NAME[];

Ini::Ini() :
    inited( 0 ),
    fileName( _T( "ColumnTools.ini" ) )
{

}

Ini::~Ini()
{

}

int Ini::setIniPath( const TCHAR *setPath )
{

    int flag;

    if ( this->inited != 0 )
        return 0;

    //_tcscat_s(this->fileName, MAX_PATH, _T("HorizontalRuler.ini"));

    if ( setPath == NULL )
    {
        //int SetExecuteCurrentPath()
        //{
        //    //wchar_t t[MAX_PATH];
        //    //GetCurrentDirectory(MAX_PATH, t);
        //    wchar_t wcExePath[MAX_PATH];
        //    wchar_t *pwcLastDirectory;
        //    GetModuleFileName(NULL, wcExePath, _MAX_PATH);
        //    pwcLastDirectory = wcsrchr(wcExePath, L'\\');
        //    *pwcLastDirectory = L'\0';
        //    SetCurrentDirectory(wcExePath);
        //
        //    return 0;
        //}
        TCHAR *ptsLastDirectory;
        GetModuleFileName( NULL, this->path, _MAX_PATH );
        ptsLastDirectory = _tcsrchr( this->path, _T( '\\' ) );
        *ptsLastDirectory = _T( '\0' );
    }
    else
    {
        flag = _tcscpy_s( this->path, MAX_PATH, setPath );

        if ( flag != 0 )
            return flag;
    }

    flag = _tcscpy_s( this->fullPath, MAX_PATH, this->path );

    if ( flag != 0 )
        return flag;

    flag = _tcscat_s( this->fullPath, MAX_PATH, _T( "\\" ) );

    if ( flag != 0 )
        return flag;

    flag = _tcscat_s( this->fullPath, MAX_PATH, this->fileName );

    if ( flag != 0 )
        return flag;

    //int SetIniPath(wchar_t *wcSetStr, int nStrSize)
    //{
    //    int flag;
    //    wchar_t wcExePath[MAX_PATH];
    //    wchar_t *pwcLastDirectory;
    //    GetModuleFileName(NULL, wcExePath, _MAX_PATH);
    //    pwcLastDirectory = wcsrchr(wcExePath, L'\\');
    //    *pwcLastDirectory = L'\0';
    //    flag = wcscat_s(wcExePath, (size_t)nStrSize, L"\\");
    //    if(flag != 0) return flag;
    //    flag = wcscat_s(wcExePath, (size_t)nStrSize, FILENAME);
    //    if(flag != 0) return flag;
    //
    //    flag = wcscpy_s(wcSetStr, (size_t)nStrSize, wcExePath);
    //
    //    return flag;
    //}
    this->inited = 1;
    return 1;
}

int Ini::checkIniFile()
{
    FILE *fp = NULL;
    errno_t error;

    error = _tfopen_s( &fp, this->fullPath, _T( "r" ) );

    if ( error != 0 )
        return 0;

    fclose( fp );
//int CheckIniFile(wchar_t *pwsIniPath)
//{
//    FILE *fp;
//    int error;
//    error = _wfopen_s(&fp, pwsIniPath, L"r");
//    if( error != 0)
//    {
//        DefalutIniMake(pwsIniPath);
//        return 1;
//    }
//    fclose(fp);
//
//    return 0;
//}
    return 1;
}

int Ini::defaultIni()
{
//int DefalutIniMake(wchar_t *filePath)
//{
//    return 0;
//}
    return 1;
}

int Ini::writeDate( const TCHAR *section, const TCHAR *key, float data )
{
    TCHAR buf[NUMDIGIT];
    _stprintf_s( buf, NUMDIGIT, _T( "%f" ), data );
    return ( int )WritePrivateProfileString( section, key, buf,
            this->fullPath );
}

int Ini::writeDate( const TCHAR *section, const TCHAR *key, int data )
{
    TCHAR buf[NUMDIGIT];
    _itot_s( data, buf, NUMDIGIT, 10 );
    return ( int )WritePrivateProfileString( section, key, buf,
            this->fullPath );
}

int Ini::writeDate( const TCHAR *section, const TCHAR *key,
                    const TCHAR *data )
{
    return ( int )WritePrivateProfileString( section, key, data,
            this->fullPath );
}

int Ini::readDate( const TCHAR *section, const TCHAR *key, TCHAR *data,
                   int dataSize )
{
    return ( int )GetPrivateProfileString( section, key, L"", data,
                                           ( DWORD )dataSize, this->fullPath );
}

int Ini::readDate( const TCHAR *section, const TCHAR *key, int *data )
{
    TCHAR buf[NUMDIGIT];
    int readSize;
    readSize = ( int )GetPrivateProfileString( section, key, L"", buf,
               ( DWORD )NUMDIGIT, this->fullPath );

    if ( ( readSize == ( NUMDIGIT - 1 ) ) || ( readSize == ( NUMDIGIT - 2 ) ) )
    {
        *data = 0;
        return 0;
    }

    *data = _ttoi( buf );

    return readSize;
}

int Ini::readDate( const TCHAR *section, const TCHAR *key, float *data )
{
    TCHAR buf[NUMDIGIT];
    int readSize;
    readSize = ( int )GetPrivateProfileString( section, key, L"", buf,
               ( DWORD )NUMDIGIT, this->fullPath );

    if ( ( readSize == ( NUMDIGIT - 1 ) ) || ( readSize == ( NUMDIGIT - 2 ) ) )
    {
        *data = 0;
        return 0;
    }

    *data = ( float )_tstof( buf );

    return readSize;
}
