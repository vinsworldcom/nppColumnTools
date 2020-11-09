#include <windows.h>
#include <shlobj.h>
#include <string>

#include "PluginInterface.h"
#include "PluginDefinition.h"
#include "resource.h"

extern HINSTANCE g_hInst;
extern NppData   nppData;

extern bool  g_bBsUnindent;
extern bool  g_bIndentGuideLF;

INT_PTR CALLBACK SettingsDlg( HWND hWndDlg, UINT msg, WPARAM wParam,
                              LPARAM lParam )
{
    switch ( msg )
    {
        case WM_INITDIALOG:
        {
            SendMessage( GetDlgItem( hWndDlg, IDC_CHK_BSU ), BM_SETCHECK,
                         ( WPARAM )( g_bBsUnindent ? 1 : 0 ), 0 );
            SendMessage( GetDlgItem( hWndDlg, IDC_CHK_IGLF ), BM_SETCHECK,
                         ( WPARAM )( g_bIndentGuideLF ? 1 : 0 ), 0 );

            std::string version;
            version = "<a>";
            version += VER_STRING;
            version += "</a>";
            SetDlgItemTextA(hWndDlg, IDC_STC_VER, version.c_str());

            return TRUE;
        }

        case WM_CLOSE:
        {
            PostMessage( hWndDlg, WM_DESTROY, 0, 0 );
            return TRUE;
        }

        case WM_DESTROY:
        {
            EndDialog( hWndDlg, 0 );
            return TRUE;
        }

        case WM_NOTIFY:
        {
            switch (((LPNMHDR)lParam)->code)
            {
                case NM_CLICK:
                case NM_RETURN:
                {
                    PNMLINK pNMLink = (PNMLINK)lParam;
                    LITEM   item    = pNMLink->item;
                    HWND ver = GetDlgItem( hWndDlg, IDC_STC_VER );

                    if ((((LPNMHDR)lParam)->hwndFrom == ver) && (item.iLink == 0))
                        ShellExecute(hWndDlg, TEXT("open"), TEXT("https://github.com/VinsWorldcom/nppColumnTools"), NULL, NULL, SW_SHOWNORMAL);

                    return TRUE;
                }
            }
            break;
        }

        case WM_COMMAND:
        {
            switch ( wParam )
            {
                case IDB_OK:
                    PostMessage( hWndDlg, WM_CLOSE, 0, 0 );
                    return TRUE;

                case IDC_CHK_BSU:
                {
                    int check = ( int )::SendMessage( GetDlgItem( hWndDlg, IDC_CHK_BSU ),
                                                      BM_GETCHECK, 0, 0 );

                    if ( check & BST_CHECKED )
                        g_bBsUnindent = true;
                    else
                        g_bBsUnindent = false;

                    doBufferSets();

                    return TRUE;
                }

                case IDC_CHK_IGLF:
                {
                    int check = ( int )::SendMessage( GetDlgItem( hWndDlg, IDC_CHK_IGLF ),
                                                      BM_GETCHECK, 0, 0 );

                    if ( check & BST_CHECKED )
                        g_bIndentGuideLF = true;
                    else
                        g_bIndentGuideLF = false;

                    doBufferSets();

                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

void doSettings()
{
    DialogBoxParam( g_hInst, MAKEINTRESOURCE( IDD_SETTINGS ),
                    nppData._nppHandle, SettingsDlg, 0 );
}
