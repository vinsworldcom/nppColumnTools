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

??
http://opensource.org/licenses/Zlib

????
http://sourceforge.jp/projects/opensource/wiki/licenses%2Fzlib_libpng_license
*/

#include "HorizontalRuler.h"
#include "menuCmdID.h"
#include "ini.h"
#include "PluginDefinition.h"

#include <windows.h>
#include <Commctrl.h>
#include <stdio.h>
#include <stdlib.h>

extern NppData nppData;
extern FuncItem funcItem[];

extern WNDPROC oldWndProc;
extern bool g_isActiveHi;
extern int  g_iEdgeModeOrig;
extern int  g_iEdgeColOrig;

#define FRAMESIZE GetSystemMetrics(SM_CXEDGE)
//SM_CXBORDER //3D??
//SM_CXEDGE    //3D
//SM_CXFRAME    //????????
//SM_CYFIXEDFRAME //??????
//Dprintf("???? Border %d  Edge %d  Frame %d  Fix %d\n",
//    GetSystemMetrics(SM_CXBORDER), GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CXFRAME), GetSystemMetrics(SM_CXFIXEDFRAME)
//    );

HorizontalRuler::HorizontalRuler() :
    nTopMargin( 0 ),
    nCharWidth( 0 ),
    nCharHeight( 0 ),
    nMarginWidth( 0 ),
    nppHwnd( 0 ),
    scintillaHwnd( 0 ),
    tabHwnd( 0 ),
    enable( 1 ),
    bFontFix( false ),
    nFontSize( 10 )
{

}

HorizontalRuler::~HorizontalRuler()
{
    if ( this->hFont )
        DeleteObject( this->hFont );

    // Ruler
    Ini::getInstance()->writeDate( TEXT( "HorizontalRuler" ),
                                   TEXT( "FontSize" ), this->nFontSize );

    int nBuf;

    if ( this->bFontFix == true )
        nBuf = 1;
    else
        nBuf = 0;

    Ini::getInstance()->writeDate( TEXT( "HorizontalRuler" ), TEXT( "Fix" ),
                                   nBuf );


    Ini::getInstance()->writeDate( TEXT( "HorizontalRuler" ),
                                   TEXT( "Visible" ), this->enable );

    // Column
    Ini::getInstance()->writeDate( TEXT( "ColumnHighlight" ),
                                   TEXT( "Mode" ), g_iEdgeModeOrig );

    Ini::getInstance()->writeDate( TEXT( "ColumnHighlight" ),
                                   TEXT( "Column" ), g_iEdgeColOrig );

    if ( g_isActiveHi == true )
        nBuf = 1;
    else
        nBuf = 0;

    Ini::getInstance()->writeDate( TEXT( "ColumnHighlight" ), TEXT( "Enable" ),
                                   nBuf );
}

void HorizontalRuler::Init( HWND npp, HWND scintilla, HWND tab )
{
    TCHAR configDir[MAX_PATH];
    TCHAR buf[MAX_PATH];
    int nBuf;

    this->nppHwnd = npp;
    this->scintillaHwnd = scintilla;
    this->tabHwnd = tab;

    SendMessage( this->nppHwnd, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH,
                 ( LPARAM )configDir );

    Ini::getInstance()->setIniPath( configDir );

    // Ruler
    Ini::getInstance()->readDate( TEXT( "HorizontalRuler" ),
                                  TEXT( "FontSize" ), buf, MAX_PATH );
    nBuf = _ttoi( buf );

    if ( nBuf != 0 )
        this->nFontSize = nBuf;

    Ini::getInstance()->readDate( TEXT( "HorizontalRuler" ), TEXT( "Fix" ),
                                  buf, MAX_PATH );
    nBuf = _ttoi( buf );

    if ( nBuf != 0 )
        this->bFontFix = true;

    Ini::getInstance()->readDate( TEXT( "HorizontalRuler" ), TEXT( "Visible" ),
                                  &this->enable );

    // Column
    Ini::getInstance()->readDate( TEXT( "ColumnHighlight" ), TEXT( "Mode" ),
                                  &g_iEdgeModeOrig );

    Ini::getInstance()->readDate( TEXT( "ColumnHighlight" ), TEXT( "Column" ),
                                  &g_iEdgeColOrig );

    Ini::getInstance()->readDate( TEXT( "ColumnHighlight" ), TEXT( "Enable" ),
                                  buf, MAX_PATH );
    nBuf = _ttoi( buf );

    if ( nBuf != 0 )
    {
        enColHi();
        HMENU hMenu = ::GetMenu( nppData._nppHandle );
        UINT state = ::GetMenuState( hMenu, funcItem[MENU_HIGHLIGHT]._cmdID,
                                     MF_BYCOMMAND );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
               funcItem[MENU_HIGHLIGHT]._cmdID, ( state | MF_CHECKED ) );
    }

    return;
}

int HorizontalRuler::IsInit()
{
    if ( ( this->tabHwnd == NULL ) || ( this->enable == 0 ) )
        return 0;

    return 1;
}

void HorizontalRuler::doVisible()
{
    this->GetInitPos();
    this->GetRuleArea();
    this->SecureArea();
    this->PaintRuler();
    return;
}

void HorizontalRuler::GetInitPos()
{
    RECT clientRc, rc, tabClientRc, tabRc;
    //POINT pt;

    GetWindowRect( this->scintillaHwnd, &rc );
    GetClientRect( this->scintillaHwnd, &clientRc );
    GetWindowRect( this->tabHwnd, &tabRc );
    GetClientRect( this->tabHwnd, &tabClientRc );

    if ( !::SendMessage( this->nppHwnd, NPPM_ISTABBARHIDDEN, 0, 0 ) )
        SendMessage( this->tabHwnd, TCM_ADJUSTRECT, FALSE,
                     ( LPARAM )&tabClientRc );

    MapWindowPoints( HWND_DESKTOP, this->nppHwnd, ( LPPOINT )&rc, 2 );
    MapWindowPoints( HWND_DESKTOP, this->nppHwnd, ( LPPOINT )&tabRc, 2 );

    this->nScintillaFrameSize = GetSystemMetrics( SM_CXEDGE );

    this->nInitClientX  = tabRc.left + tabClientRc.left;
    this->nInitClientY  = tabRc.top + tabClientRc.top +
                          this->nScintillaFrameSize;
    this->nInitWidth    = tabClientRc.right - tabClientRc.left;
    this->nInitHeight   = tabClientRc.bottom - tabClientRc.top -
                          this->nScintillaFrameSize;
    this->nInitClientX  = this->nInitClientX  < 0 ? 0 : this->nInitClientX;
    this->nInitClientY  = this->nInitClientY  < 0 ? 0 : this->nInitClientY;
    this->nInitWidth    = this->nInitWidth    < 0 ? 0 : this->nInitWidth;
    this->nInitHeight   = this->nInitHeight   < 0 ? 0 : this->nInitHeight;

    if ( clientRc.right > 0 )
    {
        SCROLLBARINFO scrollInfo;
        scrollInfo.cbSize = sizeof( scrollInfo );
        GetScrollBarInfo( this->scintillaHwnd, OBJID_VSCROLL, &scrollInfo );

        if ( scrollInfo.rgstate[0] != STATE_SYSTEM_INVISIBLE )
            tabClientRc.right -= scrollInfo.rcScrollBar.right -
                                 scrollInfo.rcScrollBar.left;
    }

    tabClientRc.top += this->nScintillaFrameSize;

    this->nDrawStartX    = tabClientRc.left;
    this->rulerRect        = tabClientRc;
    this->rulerDesctopRect    = tabClientRc;
    MapWindowPoints( this->tabHwnd, HWND_DESKTOP,
                     ( LPPOINT )&this->rulerDesctopRect, 2 );
}

void HorizontalRuler::GetRuleArea()
{
    int i;
    char sFontName[MAX_PATH];
//    int nFontSize;
    HDC hDC;

    this->nCharWidth = ( int )SendMessage( this->scintillaHwnd, SCI_TEXTWIDTH,
                                           STYLE_DEFAULT, ( LPARAM )L"P" );

    ////Scintilla??????
    this->nMarginWidth = FRAMESIZE;

    for ( i = 0; i < 5; i++ )
        this->nMarginWidth += ( int )SendMessage( this->scintillaHwnd,
                              SCI_GETMARGINWIDTHN, i, 0 );

    //???????
    if ( this->hFont )
        DeleteObject( this->hFont );

    SendMessage( this->scintillaHwnd, SCI_STYLEGETFONT, STYLE_DEFAULT,
                 ( LPARAM )sFontName );

    if ( this->bFontFix == true )
        nFontSize = this->nFontSize;
    else
        nFontSize = ( int )SendMessage( this->scintillaHwnd, SCI_STYLEGETSIZE,
                                        STYLE_DEFAULT, 0 );
    LOGFONT logFont;
    ZeroMemory( &logFont, sizeof( LOGFONT ) );
    hDC = GetDC( this->scintillaHwnd );
    logFont.lfHeight = -MulDiv( nFontSize, GetDeviceCaps( hDC, LOGPIXELSY ),
                                72 );
    ReleaseDC( this->scintillaHwnd, hDC );
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, sFontName, -1,
                         logFont.lfFaceName, 32 );

    this->hFont = CreateFontIndirect( &logFont );

    if ( this->bFontFix == true )
        this->nCharHeight = abs( logFont.lfHeight );
    else
        this->nCharHeight = ( int )SendMessage( this->scintillaHwnd,
                                                SCI_TEXTHEIGHT, 0, 0 );

    this->nTopMargin = ( int )( nCharHeight * 1.5f );

    return;
}

void HorizontalRuler::SecureArea()
{
    this->x = this->nInitClientX;
    this->y = this->nInitClientY + nTopMargin;
    this->width = this->nInitWidth;
    this->height = this->nInitHeight - nTopMargin;

    if ( this->y > 0 )
        MoveWindow( this->scintillaHwnd, this->x, this->y, this->width,
                    this->height, TRUE );

    return;
}

void HorizontalRuler::PaintRuler()
{
    HDC hDC;
    HFONT oldFont;

    RECT rc, drawRc;
    SCROLLINFO si;
    int nScrollMod;
    int i;
    int nMaxCol;
    int nRulerStartX;
    int nStartCol;
    int tmp;
    wchar_t sColumNumber[10] = {L""};
    int nLength;
    //??????????
    int nCaret;

    if ( this->nCharWidth == 0 )
        return;

    if ( this->tabHwnd  == NULL )
        return;

    nRulerStartX = this->nDrawStartX + this->nMarginWidth;
    rc = this->rulerRect;

    rc.bottom = rc.top + this->nTopMargin;

    //?????????????
    memset( &si, 0, sizeof( SCROLLINFO ) );
    si.cbSize = sizeof( SCROLLINFO );
    si.fMask = SIF_POS;// SIF_TRACKPOS POS????????
    GetScrollInfo( this->scintillaHwnd, SB_HORZ, &si );
    nScrollMod = ( si.nPos % this->nCharWidth );
    nMaxCol = ( rc.right - nRulerStartX ) / this->nCharWidth;
    nStartCol = si.nPos / this->nCharWidth;

    //??????????
    nCaret = this->GetCaretPos();

    //??????
    hDC = GetWindowDC( this->tabHwnd );
    oldFont = ( HFONT )SelectObject( hDC, this->hFont );

    drawRc = rc;
    drawRc.right = nRulerStartX;
    FillRect( hDC, &drawRc, GetSysColorBrush( 0 ) );
    drawRc.left = nRulerStartX;
    drawRc.right = rc.right;
    FillRect( hDC, &drawRc, ( HBRUSH )GetStockObject( WHITE_BRUSH ) );

    //????
    if ( nScrollMod == 0 )
    {
        if ( ( nStartCol % 10 ) == 0 )
        {
            memset( sColumNumber, 0, sizeof( sColumNumber ) );
            nLength = swprintf_s( sColumNumber, 10, L"%d", nStartCol );
            SetBkMode( hDC, TRANSPARENT );
            ExtTextOut( hDC, nRulerStartX + 1, rc.top, ETO_CLIPPED, &rc,
                        sColumNumber, nLength, 0 );

            MoveToEx( hDC, nRulerStartX, rc.top, NULL );
            LineTo( hDC, nRulerStartX, rc.top + this->nTopMargin );
        }
        else
        {
            MoveToEx( hDC, nRulerStartX, rc.top + this->nCharHeight, NULL );
            LineTo( hDC, nRulerStartX, rc.top + this->nTopMargin );
        }

        if ( nStartCol == nCaret )
        {
            RECT rcCaret;
            rcCaret.left = nRulerStartX + 2;
            rcCaret.top = rc.top + this->nCharHeight;
            rcCaret.right = rcCaret.left + this->nCharWidth - 3;
            rcCaret.bottom = rc.bottom - 1;
            FillRect( hDC, &rcCaret, ( HBRUSH )GetStockObject( BLACK_BRUSH ) );
        }
    }

    for ( i = 1; i < nMaxCol; i++ )
    {
        tmp = nRulerStartX - nScrollMod + ( i * this->nCharWidth );

        if ( ( nStartCol + i ) % 10 == 0 )
        {
            memset( sColumNumber, 0, sizeof( sColumNumber ) );
            nLength = swprintf_s( sColumNumber, 10, L"%d", nStartCol + i );
            SetBkMode( hDC, TRANSPARENT );
            ExtTextOut( hDC, tmp + 1, rc.top, ETO_CLIPPED, &rc, sColumNumber,
                        nLength, 0 );
            //TextOut(hDC, tmp+1, rc.top, sColumNumber, nLength);

            MoveToEx( hDC, tmp, rc.top, NULL );
            LineTo( hDC, tmp, rc.top + this->nTopMargin );
        }
        else
        {
            MoveToEx( hDC, tmp, rc.top + this->nCharHeight, NULL );
            LineTo( hDC, tmp, rc.top + this->nTopMargin );
        }

        if ( ( nStartCol + i ) == nCaret )
        {
            RECT rcCaret;
            rcCaret.left = tmp + 2;
            rcCaret.top = rc.top + this->nCharHeight;
            rcCaret.right = rcCaret.left + this->nCharWidth - 3;
            rcCaret.bottom = rc.bottom - 1;
            FillRect( hDC, &rcCaret, ( HBRUSH )GetStockObject( BLACK_BRUSH ) );
        }
    }

    SelectObject( hDC, oldFont );
    ReleaseDC( this->tabHwnd, hDC );
}

int HorizontalRuler::GetCaretPos()
{
    //??????????
    int i;

    char *curLin;
    wchar_t *wideCurLin;
    char *ansiCurLin;
    int nWideExchange;
    int nAnsiExchange;

    int nCaret;
    int nTabSpace;
    int nLineLength;

    nLineLength = ( int )SendMessage( this->scintillaHwnd, SCI_GETCURLINE, 0,
                                      0 );
    curLin = new char[nLineLength];
    nCaret = ( int )SendMessage( this->scintillaHwnd, SCI_GETCURLINE,
                                 nLineLength, ( LPARAM )curLin );
    nTabSpace = ( int )SendMessage( this->scintillaHwnd, SCI_GETTABWIDTH, 0,
                                    0 );

    curLin[nCaret] = '\0';
    //utf8->utf16
    nWideExchange = MultiByteToWideChar( CP_UTF8, 0, curLin, -1, 0, 0 );
    wideCurLin = new wchar_t[nWideExchange];
    MultiByteToWideChar( CP_UTF8, 0, curLin, -1, wideCurLin, nWideExchange );
    nAnsiExchange = WideCharToMultiByte( CP_ACP, 0, wideCurLin, nWideExchange,
                                         0, 0, NULL, NULL );
    //utf16->ansi
    ansiCurLin = new char[nAnsiExchange];
    WideCharToMultiByte( CP_ACP, 0, wideCurLin, nWideExchange, ansiCurLin,
                         nAnsiExchange, NULL, NULL );

    //???????
    nCaret = 0;

    for ( i = 0; i < nAnsiExchange - 1; i++ )
    {
        if ( ansiCurLin[i] == '\t' )
            nCaret = ( ( nCaret / nTabSpace ) + 1 ) * nTabSpace;
        else
            nCaret++;
    }

    delete[] ansiCurLin;
    delete[] wideCurLin;
    delete[] curLin;
    ansiCurLin = NULL;
    wideCurLin = NULL;
    curLin = NULL;

    return nCaret;
}

void HorizontalRuler::SendSizeToMain()
{

    RECT rc;
    GetWindowRect( this->nppHwnd, &rc );
    SendMessage( this->nppHwnd, WM_SIZE, 0, MAKELPARAM( rc.right - rc.left,
                 rc.bottom - rc.top ) );

    return;
}

bool HorizontalRuler::HitDrawArea( int lx, int ly )
{
    if ( ( lx > this->rulerDesctopRect.left + this->nMarginWidth ) &&
            ( lx < this->rulerDesctopRect.right ) &&
            ( ly > this->rulerDesctopRect.top ) &&
            ( ly < this->rulerDesctopRect.top + this->nTopMargin ) )
        return true;

    return false;
}

int HorizontalRuler::EdgeLine( int lx, int /* y */)
{
    int nSetEdgeLine, nNowEdgeLine;
    int nEdgeLineMode;
    nSetEdgeLine = ( lx - ( this->rulerDesctopRect.left +
                           this->nMarginWidth ) ) / this->nCharWidth;
    nNowEdgeLine = ( int )SendMessage( this->scintillaHwnd, SCI_GETEDGECOLUMN,
                                       0, 0 );
    nEdgeLineMode = ( int )SendMessage( this->scintillaHwnd, SCI_GETEDGEMODE,
                                        0, 0 );

    if ( ( nNowEdgeLine != nSetEdgeLine ) || ( nEdgeLineMode == EDGE_NONE ) )
    {
        SendMessage( this->scintillaHwnd, SCI_SETEDGECOLUMN, nSetEdgeLine, 0 );
        SendMessage( this->scintillaHwnd, SCI_SETEDGEMODE, EDGE_LINE, 0 );
    }
    else
        SendMessage( this->scintillaHwnd, SCI_SETEDGEMODE, EDGE_NONE, 0 );

    return 0;
}
