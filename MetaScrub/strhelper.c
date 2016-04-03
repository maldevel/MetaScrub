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
#include "strhelper.h"

int strLenA(const char *str)
{
	int index = 0;
	if (str != NULL) 
		while (str[index] != 0)index++;
	return index;
}


int strLenW(const WCHAR *str)
{
	int index = 0;
	if (str != NULL) 
		while (str[index] != 0)index++;
	return index;
}