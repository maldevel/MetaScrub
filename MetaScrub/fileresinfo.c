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

#include <Windows.h>
#include <stdbool.h>
#include <strsafe.h>
#include "common.h"
#include "fileresinfo.h"

bool QueryInfo(WCHAR **dest, unsigned int *dlen, const WCHAR *query, const void *data, WORD language, WORD codepage)
{
	WCHAR queryStr[QUERYSIZE];

	HRESULT hr = StringCchPrintfW(queryStr, QUERYSIZE, query, language, codepage);

	if (FAILED(hr))
	{
		return false;
	}

	if (!VerQueryValueW(data, queryStr, (LPVOID*)dest, dlen))
	{
		return false;
	}

	return true;
}

bool GetVerInfo(AppVersion *appVersion, const WCHAR *filePath)
{
	VS_FIXEDFILEINFO *fixedFileInfo;
	DWORD verSize = 0;
	DWORD handle = 0;
	unsigned char *data = 0;
	unsigned int fixedFileInfoLen;
	Translate *translate;
	unsigned int translateLen;
	int i = 0;
	//HRESULT hr;
	WCHAR *value;
	unsigned int len = 0;

	if ((verSize = GetFileVersionInfoSizeW(filePath, &handle)) == 0)
	{
		return false;
	}

	data = (unsigned char *)malloc(verSize*sizeof(unsigned char));
	if (data == NULL)
	{
		return false;
	}
	SecureZeroMemory(data, verSize * sizeof(unsigned char));

	if (!GetFileVersionInfoW(filePath, 0, verSize, data))
	{
		SAFE_FREE(data);
		return false;
	}

	if (!VerQueryValueW(data, L"\\", (LPVOID*)&fixedFileInfo, &fixedFileInfoLen))
	{
		SAFE_FREE(data);
		return false;
	}

	if (fixedFileInfoLen < sizeof(&fixedFileInfo))
	{
		SAFE_FREE(data);
		return false;
	}

	appVersion->FileMajor = HIWORD(fixedFileInfo->dwFileVersionMS);
	appVersion->FileMinor = LOWORD(fixedFileInfo->dwFileVersionMS);
	appVersion->FileBuild = HIWORD(fixedFileInfo->dwFileVersionLS);
	appVersion->FileRevision = LOWORD(fixedFileInfo->dwFileVersionLS);

	appVersion->ProductMajor = HIWORD(fixedFileInfo->dwProductVersionMS);
	appVersion->ProductMinor = LOWORD(fixedFileInfo->dwProductVersionMS);
	appVersion->ProductBuild = HIWORD(fixedFileInfo->dwProductVersionLS);
	appVersion->ProductRevision = LOWORD(fixedFileInfo->dwProductVersionLS);

	if (!VerQueryValueW(data, L"\\VarFileInfo\\Translation", (LPVOID*)&translate, &translateLen))
	{
		SAFE_FREE(data);
		return false;
	}

	appVersion->Language = translate->Language;
	appVersion->CodePage = translate->CodePage;

	if (!QueryInfo(&value, &len, COMPANY_QUERY, data, appVersion->Language, appVersion->CodePage))
	{
		SAFE_FREE(data);
		return false;
	}

	appVersion->CompanyName = (WCHAR *)malloc(len * sizeof(WCHAR));
	if (appVersion->CompanyName == NULL)
	{
		SAFE_FREE(data);
		return false;
	}
	SecureZeroMemory(appVersion->CompanyName, len * sizeof(WCHAR));
	wcsncpy_s(appVersion->CompanyName, len, value, len);

	if (!QueryInfo(&value, &len, FILEDESC_QUERY, data, appVersion->Language, appVersion->CodePage))
	{
		SAFE_FREE(data);
		return false;
	}

	appVersion->FileDescription = (WCHAR *)malloc(len * sizeof(WCHAR));
	if (appVersion->FileDescription == NULL)
	{
		SAFE_FREE(data);
		return false;
	}
	SecureZeroMemory(appVersion->FileDescription, len * sizeof(WCHAR));
	wcsncpy_s(appVersion->FileDescription, len, value, len);

	if (!QueryInfo(&value, &len, FILEVERSION_QUERY, data, appVersion->Language, appVersion->CodePage))
	{
		SAFE_FREE(data);
		return false;
	}

	appVersion->FileVersion = (WCHAR *)malloc(len * sizeof(WCHAR));
	if (appVersion->FileVersion == NULL)
	{
		SAFE_FREE(data);
		return false;
	}
	SecureZeroMemory(appVersion->FileVersion, len * sizeof(WCHAR));
	wcsncpy_s(appVersion->FileVersion, len, value, len);

	if (!QueryInfo(&value, &len, INTERNALNAME_QUERY, data, appVersion->Language, appVersion->CodePage))
	{
		SAFE_FREE(data);
		return false;
	}

	appVersion->InternalName = (WCHAR *)malloc(len * sizeof(WCHAR));
	if (appVersion->InternalName == NULL)
	{
		SAFE_FREE(data);
		return false;
	}
	SecureZeroMemory(appVersion->InternalName, len * sizeof(WCHAR));
	wcsncpy_s(appVersion->InternalName, len, value, len);

	if (!QueryInfo(&value, &len, CLOPY_QUERY, data, appVersion->Language, appVersion->CodePage))
	{
		SAFE_FREE(data);
		return false;
	}

	appVersion->Copyright = (WCHAR *)malloc(len * sizeof(WCHAR));
	if (appVersion->Copyright == NULL)
	{
		SAFE_FREE(data);
		return false;
	}
	SecureZeroMemory(appVersion->Copyright, len * sizeof(WCHAR));
	wcsncpy_s(appVersion->Copyright, len, value, len);

	if (!QueryInfo(&value, &len, ORIGINALFILENAME_QUERY, data, appVersion->Language, appVersion->CodePage))
	{
		SAFE_FREE(data);
		return false;
	}

	appVersion->OriginalName = (WCHAR *)malloc(len * sizeof(WCHAR));
	if (appVersion->OriginalName == NULL)
	{
		SAFE_FREE(data);
		return false;
	}
	SecureZeroMemory(appVersion->OriginalName, len * sizeof(WCHAR));
	wcsncpy_s(appVersion->OriginalName, len, value, len);

	if (!QueryInfo(&value, &len, PRODUCTNAME_QUERY, data, appVersion->Language, appVersion->CodePage))
	{
		SAFE_FREE(data);
		return false;
	}

	appVersion->ProductName = (WCHAR *)malloc(len * sizeof(WCHAR));
	if (appVersion->ProductName == NULL)
	{
		SAFE_FREE(data);
		return false;
	}
	SecureZeroMemory(appVersion->ProductName, len * sizeof(WCHAR));
	wcsncpy_s(appVersion->ProductName, len, value, len);

	if (!QueryInfo(&value, &len, PRODUCTVER_QUERY, data, appVersion->Language, appVersion->CodePage))
	{
		SAFE_FREE(data);
		return false;
	}

	appVersion->ProductVersion = (WCHAR *)malloc(len * sizeof(WCHAR));
	if (appVersion->ProductVersion == NULL)
	{
		SAFE_FREE(data);
		return false;
	}
	SecureZeroMemory(appVersion->ProductVersion, len * sizeof(WCHAR));
	wcsncpy_s(appVersion->ProductVersion, len, value, len);

	SAFE_FREE(data);
	return true;
}
