#pragma once

/*
MetaScrub - Remove personal metadata from files
Copyright (C) 2016  @maldevel

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define QUERYSIZE 128
#define FILEDESC_QUERY			L"\\StringFileInfo\\%04x%04x\\FileDescription\0"
#define COMPANY_QUERY			L"\\StringFileInfo\\%04x%04x\\CompanyName\0"
#define FILEVERSION_QUERY		L"\\StringFileInfo\\%04x%04x\\FileVersion\0"
#define INTERNALNAME_QUERY		L"\\StringFileInfo\\%04x%04x\\InternalName\0"
#define CLOPY_QUERY				L"\\StringFileInfo\\%04x%04x\\LegalCopyright\0"
#define ORIGINALFILENAME_QUERY	L"\\StringFileInfo\\%04x%04x\\OriginalFilename\0"
#define PRODUCTNAME_QUERY		L"\\StringFileInfo\\%04x%04x\\ProductName\0"
#define PRODUCTVER_QUERY		L"\\StringFileInfo\\%04x%04x\\ProductVersion\0"

typedef struct _AppVersion
{
	WORD FileMajor;
	WORD FileMinor;
	WORD FileBuild;
	WORD FileRevision;

	WORD ProductMajor;
	WORD ProductMinor;
	WORD ProductBuild;
	WORD ProductRevision;

	WORD Language;
	WORD CodePage;

	WCHAR *CompanyName;
	WCHAR *FileDescription;
	WCHAR *FileVersion;
	WCHAR *InternalName;
	WCHAR *Copyright;
	WCHAR *OriginalName;
	WCHAR *ProductName;
	WCHAR *ProductVersion;

} AppVersion, *PAppVersion;


typedef struct _LANGANDCODEPAGE {
	WORD Language;
	WORD CodePage;
} Translate, *PTranslate;


bool QueryInfo(WCHAR **dest, unsigned int *dlen, const WCHAR *query, const void *data, WORD language, WORD codepage);
bool GetVerInfo(AppVersion *appVersion, const WCHAR *filePath);
