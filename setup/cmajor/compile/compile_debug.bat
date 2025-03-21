call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
msbuild -verbosity:minimal -t:Rebuild -p:Configuration="Debug" "D:\work\cmajor-mod\setup\cmajor\program\setup.sln" > "D:\work\cmajor-mod\setup\cmajor\compile\debug_stdout.txt" 2> "D:\work\cmajor-mod\setup\cmajor\compile\debug_stderr.txt"
