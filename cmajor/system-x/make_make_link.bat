rem makes make.exe as an alias to mingw32-make.exe
rem if needed edit C:\mingw64\bin\mingw32-make.exe to contain your /path/to/mingw32-make.exe
rem run this bat as administrator
@echo off
mklink make.exe C:\mingw64\bin\mingw32-make.exe
