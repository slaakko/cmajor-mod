@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
dumpbin /disasm:nobytes /out:"D:/work/cmajor-mod/cmajor/sbin/make_stripped_asm/-v.stripped.asm.000" "D:/work/cmajor-mod/cmajor/sbin/make_stripped_asm/-v"
