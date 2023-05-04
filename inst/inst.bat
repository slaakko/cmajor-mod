@echo off
IF NOT EXIST cminst.exe xcopy /Y /D ..\x64\Release\cminst.exe .
IF NOT EXIST ..\bin mkdir ..\bin
IF NOT EXIST ..\lib mkdir ..\lib
IF NOT EXIST ..\rc mkdir ..\rc
cminst -v ..\x64\Debug\*.exe ..\bin
cminst -v ..\x64\Release\*.exe ..\bin
cminst -v ..\x64\Debug\*.lib ..\lib
cminst -v ..\x64\Release\*.lib ..\lib
cminst -v ..\cmajor\lexer\*.classmap.compressed ..\rc
cminst -v ..\cmajor\lexer\*.classmap.rc ..\rc
cminst -v ..\soul\rex\*.classmap.compressed ..\rc
cminst -v ..\soul\rex\*.classmap.rc ..\rc
