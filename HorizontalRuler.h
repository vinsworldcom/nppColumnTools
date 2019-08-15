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
#ifndef HORIZONTAL_RULER
#define HORIZONTAL_RULER

#include "PluginInterface.h"

LRESULT CALLBACK RulerWndProc( HWND hwnd, UINT uMsg, WPARAM wParam,
                               LPARAM lParam );

class HorizontalRuler
{
    private:
        int nTopMargin;
        int nCharWidth;
        int nCharHeight;
        int nMarginWidth;

        int nScintillaFrameSize;
        int nScintillaYFrameSize;
        int nScintillaXFrameSize;

        int nInitClientY;
        int nInitClientX;
        int nInitWidth;
        int nInitHeight;

        int x;
        int y;
        int width;
        int height;
        HFONT hFont;
        HWND nppHwnd;
        HWND scintillaHwnd;
        HWND tabHwnd;

        RECT rulerRect;
        RECT rulerDesctopRect;
        int nDrawStartX;

        //無効・有効
        int enable;
    public:
        int nFontSize;
        bool bFontFix;

        HorizontalRuler();
        ~HorizontalRuler();

        void HorizontalRuler::Init( HWND npp, HWND scintilla, HWND tab );
        int IsInit();
        int     GetEnable()
        {
            return this->enable;
        };
        void SetEnable( int flag )
        {
            this->enable = flag;
        };

        void doVisible();

        void GetInitPos();
        void GetRuleArea();
        void SecureArea();
        void PaintRuler();
        int GetCaretPos();

        void SendSizeToMain();

        bool HitDrawArea( int x, int y );
        int EdgeLine( int x, int y );
};

#endif