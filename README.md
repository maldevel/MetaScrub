MetaScrub
====
Remove personal metadata from files.


Features
=====
* Export/Dump file properties
* Change file properties


Download
====
* git clone https://github.com/maldevel/MetaScrub


Usage
=====
```
usage: MetaScrub.exe [-f FILE] [-i] [-h] [-v FILEVERSION] [-p PRODUCTVERSION]
                     [-c COMPANY] [-d DESCRIPTION] [-n NAME] [-l COPYRIGHT]
                     [-r ORIGINALNAME] [-u PRODUCTNAME] [-e FILEVERSION]
                     [-o PRODUCTVERSION]

MetaScrub 1.0
--[ Remove personal metadata from files
--[ Copyright (c) 2016 @maldevel

optional arguments:
-f FILE                 File to examine and/or edit
-v FILEVERSION          Replace file version. (ex. 2.0.0.1)
-p PRODUCTVERSION       Replace product version. (ex. 2.0.0.1)
-c COMPANY              Replace Company name.
-d DESCRIPTION          Replace File description.
-n NAME                 Replace Internal name.
-l COPYRIGHT            Replace Copyright.
-r ORIGINALNAME         Replace Original name.
-u PRODUCTNAME          Replace Product name.
-e FILEVERSION          Replace string file version.
-o PRODUCTVERSION       Replace string product version.
-i                      Export file properties
-h                      Show this help message and exit
```
