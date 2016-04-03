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
#include <string.h>
#include "common.h"
#include "filehelper.h"
#include "strhelper.h"

bool FileExists(const WCHAR *fileName)
{
	DWORD fileAttrib = GetFileAttributes(fileName);
	return (fileAttrib != INVALID_FILE_ATTRIBUTES && !(fileAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool VerionStrToInt(DWORD *verInt, const WCHAR *verStr)
{
	if (!verStr)
	{
		return false;
	}

	WCHAR *token1 = 0;
	WCHAR *next_token1 = 0;
	int count = strLenW(verStr) + 1;
	WCHAR *temp = (WCHAR*)malloc(count * sizeof(WCHAR));

	wcsncpy_s(temp, count, verStr, _TRUNCATE);

	token1 = wcstok_s(temp, L".", &next_token1);

	while (token1 != NULL)
	{
		*verInt++ = wcstol(token1, NULL, 10);
		token1 = wcstok_s(NULL, L".", &next_token1);
	}

	SAFE_FREE(temp);
	return true;
}
