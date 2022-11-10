#ifndef RESOURCE_H
#define RESOURCE_H

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/* VERSION DEFINITIONS */
#define VER_MAJOR   1
#define VER_MINOR   4
#define VER_RELEASE 5
#define VER_BUILD   1
#define VER_STRING  STR(VER_MAJOR) "." STR(VER_MINOR) "." STR(VER_RELEASE) "." STR(VER_BUILD)

#define FILE_DESCRIPTION  "Notepad++ Plugin for column highlighting and ruler."
#define INTERNAL_NAME     "Column Tools"
#define FILENAME          "ColumnTools"

#define COMPANY_NAME      "Vin's World"
#define FILE_VERSION      VER_STRING
#define LEGAL_COPYRIGHT   "Copyright (C) VinsWorld. All Rights Reserved."
#define LEGAL_TRADEMARKS  ""
#define ORIGINAL_FILENAME FILENAME ".dll"
#define PRODUCT_NAME      "Column Tools"
#define PRODUCT_VERSION   VER_STRING

/* ADDITIONAL DEFINITIONS */

// Settings
#ifndef IDC_STATIC
#define IDC_STATIC  -1
#endif

#define IDD_SETTINGS      2600

#define IDB_OK                 (IDD_SETTINGS + 1)

#define IDC_CHK_BSU            (IDD_SETTINGS + 2)
#define IDC_CHK_IGLF           (IDD_SETTINGS + 3)

#define IDC_GRP1               (IDD_SETTINGS + 4)
#define IDC_RBN_0              (IDD_SETTINGS + 5)
#define IDC_RBN_1              (IDD_SETTINGS + 6)

#define IDC_STC_VER            (IDD_SETTINGS + 7)

#endif
