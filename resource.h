#ifndef RESOURCE_H
#define RESOURCE_H

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/* VERSION DEFINITIONS */
#define VER_MAJOR   1
#define VER_MINOR   3
#define VER_RELEASE 6
#define VER_BUILD   1
#define VER_STRING  STR(VER_MAJOR) "." STR(VER_MINOR) "." STR(VER_RELEASE) "." STR(VER_BUILD)

#define FILE_DESCRIPTION  "Notepad++ Plugin providing column tools for highlighting and ruler."
#define INTERNAL_NAME	  "Notepad++ Column Tools"

#define COMPANY_NAME	  "Vin's World"
#define FILE_VERSION	  VER_STRING
#define LEGAL_COPYRIGHT	  "Copyright (C) VinsWorld. All Rights Reserved."
#define LEGAL_TRADEMARKS  ""
#define ORIGINAL_FILENAME "ColumnTools"
#define PRODUCT_NAME	  "COLUMNTOOLS"
#define PRODUCT_VERSION	  VER_STRING

/* ADDITIONAL DEFINITIONS */

#endif
