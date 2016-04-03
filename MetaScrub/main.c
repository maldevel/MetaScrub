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

#define VERSION "1.0"

#include <Windows.h>
#include <stdbool.h>
#include "common.h"
#include "filehelper.h"
#include "fileresinfo.h"
#include "filereseditor.h"
#include "strhelper.h"

void printUsage(void);

int main(void)
{
	bool dumpInfo = false, showHelp = false, unknown = false;
	bool replaceFileVer = false, replaceProductVer = false, validFilePath = false;
	int nArgs = 0;
	WCHAR filePath[MAX_PATH];
	AppVersion appVersion;
	WCHAR *newFileVersion = 0;
	WCHAR *newProductVersion = 0;
	HANDLE resource = 0;
	WCHAR *companyName = 0;
	bool replaceCompanyName = false, replaceFileDescription = false;
	WCHAR *fileDescription = 0;
	WCHAR *internalName = 0;
	bool replaceInternalName = false, replaceClopy = false;
	WCHAR *clopyright = 0;
	WCHAR *originalName = 0;
	WCHAR *productName = 0;
	bool replaceOriginalName = false, replaceProductName = false;
	WCHAR *stringFileVersion = 0;
	WCHAR *stringProductVersion = 0;
	bool replaceStringFileVersion = false, replaceStringProductVersion = false;

	LPWSTR *args = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	if (args != NULL)
	{

		if (nArgs < 2)
		{
			printUsage();
			return EXIT_FAILURE;
		}

		for (int i = 0; i < nArgs; i++)
		{
			if (args[i] != NULL && args[i][0] == '-')
			{
				switch (args[i][1])
				{

				case 'i':dumpInfo = true; break;

				case 'h':showHelp = true; break;

				case 'f':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						if (FileExists(args[i + 1]))
						{
							wcsncpy_s(filePath, MAX_PATH, args[++i], _TRUNCATE);
							validFilePath = true;
							break;
						}
					}
					PRINT("You have to specify a valid file path.");
					SetLastError(ERROR_FILE_NOT_FOUND);
					exit(EXIT_FAILURE);

				case 'v':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						int count = strLenW(args[i + 1]);
						newFileVersion = (WCHAR*)malloc((count + 1) * sizeof(WCHAR));
						if (newFileVersion == NULL)
						{
							exit(EXIT_FAILURE);
						}
						SecureZeroMemory(newFileVersion, (count + 1) * sizeof(WCHAR));
						wcsncpy_s(newFileVersion, count + 1, args[++i], _TRUNCATE);
						replaceFileVer = true;
					}
					break;

				case 'p':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						int count = strLenW(args[i + 1]);
						newProductVersion = (WCHAR*)malloc((count + 1) * sizeof(WCHAR));
						if (newProductVersion == NULL)
						{
							exit(EXIT_FAILURE);
						}
						SecureZeroMemory(newProductVersion, (count + 1) * sizeof(WCHAR));
						wcsncpy_s(newProductVersion, count + 1, args[++i], _TRUNCATE);
						replaceProductVer = true;
					}
					break;

				case 'c':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						int count = strLenW(args[i + 1]);
						companyName = (WCHAR*)malloc((count + 1) * sizeof(WCHAR));
						if (companyName == NULL)
						{
							exit(EXIT_FAILURE);
						}
						SecureZeroMemory(companyName, (count + 1) * sizeof(WCHAR));
						wcsncpy_s(companyName, count + 1, args[++i], _TRUNCATE);
						replaceCompanyName = true;
					}
					break;

				case 'd':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						int count = strLenW(args[i + 1]);
						fileDescription = (WCHAR*)malloc((count + 1) * sizeof(WCHAR));
						if (fileDescription == NULL)
						{
							exit(EXIT_FAILURE);
						}
						SecureZeroMemory(fileDescription, (count + 1) * sizeof(WCHAR));
						wcsncpy_s(fileDescription, count + 1, args[++i], _TRUNCATE);
						replaceFileDescription = true;
					}
					break;

				case 'n':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						int count = strLenW(args[i + 1]);
						internalName = (WCHAR*)malloc((count + 1) * sizeof(WCHAR));
						if (internalName == NULL)
						{
							exit(EXIT_FAILURE);
						}
						SecureZeroMemory(internalName, (count + 1) * sizeof(WCHAR));
						wcsncpy_s(internalName, count + 1, args[++i], _TRUNCATE);
						replaceInternalName = true;
					}
					break;

				case 'l':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						int count = strLenW(args[i + 1]);
						clopyright = (WCHAR*)malloc((count + 1) * sizeof(WCHAR));
						if (clopyright == NULL)
						{
							exit(EXIT_FAILURE);
						}
						SecureZeroMemory(clopyright, (count + 1) * sizeof(WCHAR));
						wcsncpy_s(clopyright, count + 1, args[++i], _TRUNCATE);
						replaceClopy = true;
					}
					break;

				case 'r':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						int count = strLenW(args[i + 1]);
						originalName = (WCHAR*)malloc((count + 1) * sizeof(WCHAR));
						if (originalName == NULL)
						{
							exit(EXIT_FAILURE);
						}
						SecureZeroMemory(originalName, (count + 1) * sizeof(WCHAR));
						wcsncpy_s(originalName, count + 1, args[++i], _TRUNCATE);
						replaceOriginalName = true;
					}
					break;

				case 'u':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						int count = strLenW(args[i + 1]);
						productName = (WCHAR*)malloc((count + 1) * sizeof(WCHAR));
						if (productName == NULL)
						{
							exit(EXIT_FAILURE);
						}
						SecureZeroMemory(productName, (count + 1) * sizeof(WCHAR));
						wcsncpy_s(productName, count + 1, args[++i], _TRUNCATE);
						replaceProductName = true;
					}
					break;

				case 'e':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						int count = strLenW(args[i + 1]);
						stringFileVersion = (WCHAR*)malloc((count + 1) * sizeof(WCHAR));
						if (stringFileVersion == NULL)
						{
							exit(EXIT_FAILURE);
						}
						SecureZeroMemory(stringFileVersion, (count + 1) * sizeof(WCHAR));
						wcsncpy_s(stringFileVersion, count + 1, args[++i], _TRUNCATE);
						replaceStringFileVersion = true;
					}
					break;

				case 'o':
					if (args[i + 1] && args[i + 1][0] != '-')
					{
						int count = strLenW(args[i + 1]);
						stringProductVersion = (WCHAR*)malloc((count + 1) * sizeof(WCHAR));
						if (stringProductVersion == NULL)
						{
							exit(EXIT_FAILURE);
						}
						SecureZeroMemory(stringProductVersion, (count + 1) * sizeof(WCHAR));
						wcsncpy_s(stringProductVersion, count + 1, args[++i], _TRUNCATE);
						replaceStringProductVersion = true;
					}
					break;

				case '-':
					break;

				default:unknown = true;

				}
			}
		}
		LocalFree(args);
	}

	if (unknown)
	{
		PRINT("Unrecognized option\n");
		printUsage();
		return EXIT_FAILURE;
	}

	if (showHelp)
	{
		printUsage();
		return EXIT_SUCCESS;
	}

	if (dumpInfo && validFilePath)
	{
		if (GetVerInfo(&appVersion, filePath))
		{
			printf("File version: %d.%d.%d.%d\n",
				appVersion.FileMajor,
				appVersion.FileMinor,
				appVersion.FileBuild,
				appVersion.FileRevision);

			printf("Product version: %d.%d.%d.%d\n",
				appVersion.ProductMajor,
				appVersion.ProductMinor,
				appVersion.ProductBuild,
				appVersion.ProductRevision);

			printf("Language: %d\n", appVersion.Language);

			printf("CodePage: %d\n", appVersion.CodePage);

			wprintf(L"Company Name: %s\n"
				L"File Description: %s\n"
				L"File Version: %s\n"
				L"Internal Name: %s\n"
				L"Copyright: %s\n"
				L"Original Name: %s\n"
				L"Product Name: %s\n"
				L"Product Version: %s\n",
				appVersion.CompanyName,
				appVersion.FileDescription,
				appVersion.FileVersion,
				appVersion.InternalName,
				appVersion.Copyright,
				appVersion.OriginalName,
				appVersion.ProductName,
				appVersion.ProductVersion
				);

			SAFE_FREE(appVersion.CompanyName);
			SAFE_FREE(appVersion.FileDescription);
			SAFE_FREE(appVersion.FileVersion);
			SAFE_FREE(appVersion.InternalName);
			SAFE_FREE(appVersion.Copyright);
			SAFE_FREE(appVersion.OriginalName);
			SAFE_FREE(appVersion.ProductName);
			SAFE_FREE(appVersion.ProductVersion);
		}
	}

	if (replaceFileVer && validFilePath)
	{
		resource = ResOpen(filePath);

		UpdateVersion(resource, filePath, newFileVersion, true);

		ResClose(resource);

		SAFE_FREE(newFileVersion);
	}

	if (replaceProductVer && validFilePath)
	{
		resource = ResOpen(filePath);

		UpdateVersion(resource, filePath, newProductVersion, false);

		ResClose(resource);

		SAFE_FREE(newProductVersion);
	}

	if (replaceCompanyName && validFilePath)
	{
		resource = ResOpen(filePath);

		UpdateStringVersion(resource, filePath, COMPANY_QUERY, companyName);

		ResClose(resource);

		SAFE_FREE(companyName);
	}

	if (replaceFileDescription && validFilePath)
	{
		resource = ResOpen(filePath);

		UpdateStringVersion(resource, filePath, FILEDESC_QUERY, fileDescription);

		ResClose(resource);

		SAFE_FREE(fileDescription);
	}

	if (replaceInternalName && validFilePath)
	{
		resource = ResOpen(filePath);

		UpdateStringVersion(resource, filePath, INTERNALNAME_QUERY, internalName);

		ResClose(resource);

		SAFE_FREE(internalName);
	}

	if (replaceClopy && validFilePath)
	{
		resource = ResOpen(filePath);

		UpdateStringVersion(resource, filePath, CLOPY_QUERY, clopyright);

		ResClose(resource);

		SAFE_FREE(clopyright);
	}

	if (replaceOriginalName && validFilePath)
	{
		resource = ResOpen(filePath);

		UpdateStringVersion(resource, filePath, ORIGINALFILENAME_QUERY, originalName);

		ResClose(resource);

		SAFE_FREE(originalName);
	}

	if (replaceProductName && validFilePath)
	{
		resource = ResOpen(filePath);

		UpdateStringVersion(resource, filePath, PRODUCTNAME_QUERY, productName);

		ResClose(resource);

		SAFE_FREE(productName);
	}

	if (replaceStringFileVersion && validFilePath)
	{
		resource = ResOpen(filePath);

		UpdateStringVersion(resource, filePath, FILEVERSION_QUERY, stringFileVersion);

		ResClose(resource);

		SAFE_FREE(stringFileVersion);
	}

	if (replaceStringProductVersion && validFilePath)
	{
		resource = ResOpen(filePath);

		UpdateStringVersion(resource, filePath, PRODUCTVER_QUERY, stringProductVersion);

		ResClose(resource);

		SAFE_FREE(stringProductVersion);
	}

	return EXIT_SUCCESS;
}


void printUsage(void)
{
	printf("usage: MetaScrub.exe [-f FILE] [-i] [-h] [-v FILEVERSION] [-p PRODUCTVERSION]\n" 
		"                     [-c COMPANY] [-d DESCRIPTION] [-n NAME] [-l COPYRIGHT]\n"
		"                     [-r ORIGINALNAME] [-u PRODUCTNAME] [-e FILEVERSION]\n"
		"                     [-o PRODUCTVERSION]\n\n"
		"MetaScrub %s\n"
		"--[ Remove personal metadata from files\n"
		"--[ Copyright (c) 2016 @maldevel\n\n"
		"optional arguments:\n"
		"-f FILE\t\t\tFile to examine and/or edit\n"
		"-v FILEVERSION\t\tReplace file version. (ex. 2.0.0.1)\n"
		"-p PRODUCTVERSION\tReplace product version. (ex. 2.0.0.1)\n"
		"-c COMPANY\t\tReplace Company name.\n"
		"-d DESCRIPTION\t\tReplace File description.\n"
		"-n NAME\t\t\tReplace Internal name.\n"
		"-l COPYRIGHT\t\tReplace Copyright.\n"
		"-r ORIGINALNAME\t\tReplace Original name.\n"
		"-u PRODUCTNAME\t\tReplace Product name.\n"
		"-e FILEVERSION\t\tReplace string file version.\n"
		"-o PRODUCTVERSION\tReplace string product version.\n"
		"-i\t\t\tExport file properties\n"
		"-h\t\t\tShow this help message and exit\n",
		VERSION
		);
}
