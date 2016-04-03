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
#include "common.h"
#include "filereseditor.h"
#include "fileresinfo.h"
#include "filehelper.h"
#include "strhelper.h"

HANDLE ResOpen(const WCHAR *libFileName)
{
	return BeginUpdateResourceW(libFileName, FALSE);
}

bool ResClose(HANDLE resource)
{
	if (resource == NULL)
	{
		return false;
	}

	if (!EndUpdateResourceW(resource, FALSE))
		return false;

	resource = NULL;

	return true;
}

bool UpdateVersion(HANDLE resource, const WCHAR *filePath, const WCHAR *newVersion, bool updateFileVersion)
{
	unsigned char *data = 0;
	DWORD verSize = 0, handle = 0;
	VS_FIXEDFILEINFO *fixedFileInfo;
	unsigned int fixedFileInfoLen = 0;
	Translate *translate = 0;
	unsigned int translateLen = 0;
	unsigned int len = 0;
	DWORD newVersionInt[4];

	VerionStrToInt(newVersionInt, newVersion);

	if ((verSize = GetFileVersionInfoSizeW(filePath, &handle)) == 0)
	{
		return false;
	}

	data = (unsigned char *)malloc(verSize * sizeof(unsigned char));
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

	if (!VerQueryValueW(data, L"\\VarFileInfo\\Translation", (LPVOID*)&translate, &translateLen))
	{
		SAFE_FREE(data);
		return false;
	}

	if (updateFileVersion) 
	{
		fixedFileInfo->dwFileVersionMS = ((newVersionInt[0] & 0xFFFF) << 16) | (newVersionInt[1] & 0xFFFF);
		fixedFileInfo->dwFileVersionLS = ((newVersionInt[2] & 0xFFFF) << 16) | (newVersionInt[3] & 0xFFFF);
	}
	else 
	{
		fixedFileInfo->dwProductVersionMS = ((newVersionInt[0] & 0xFFFF) << 16) | (newVersionInt[1] & 0xFFFF);
		fixedFileInfo->dwProductVersionLS = ((newVersionInt[2] & 0xFFFF) << 16) | (newVersionInt[3] & 0xFFFF);
	}
	
	if (!UpdateResourceW(resource, RT_VERSION, MAKEINTRESOURCEW(VS_VERSION_INFO), translate->Language, data, verSize))
		return false;

	SAFE_FREE(data);
	return true;
}

bool UpdateStringVersion(HANDLE resource, const WCHAR *filePath, const WCHAR *field, const WCHAR *newValue)
{
	unsigned char *data = 0;
	//VS_FIXEDFILEINFO *fixedFileInfo;
	//unsigned int fixedFileInfoLen = 0;
	DWORD verSize = 0;
	DWORD handle = 0;
	Translate *translate;
	unsigned int translateLen = 0;
	WCHAR *value;
	unsigned int len = 0;

	if ((verSize = GetFileVersionInfoSizeW(filePath, &handle)) == 0)
	{
		return false;
	}

	data = (unsigned char *)malloc(verSize * sizeof(unsigned char));
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

	if (!VerQueryValueW(data, L"\\VarFileInfo\\Translation", (LPVOID*)&translate, &translateLen))
	{
		SAFE_FREE(data);
		return false;
	}

	if (!QueryInfo(&value, &len, field, data, translate->Language, translate->CodePage))
	{
		SAFE_FREE(data);
		return false;
	}

	SecureZeroMemory(value, len);
	wcsncpy_s(value, len, newValue, _TRUNCATE);

	if (!UpdateResourceW(resource, RT_VERSION, MAKEINTRESOURCEW(VS_VERSION_INFO), translate->Language, data, verSize))
		return false;

	SAFE_FREE(data);
	return true;
}
