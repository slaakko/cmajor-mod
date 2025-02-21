@echo off
rem git clone --depth 1 --branch llvmorg-19.1.7 https://github.com/llvm/llvm-project.git llvm-project
rem start Visual Studio 2022 x64 Native Tools command prompt
D:
mkdir D:\llvm-project\build
cd D:\llvm-project\build
cmake -G "Visual Studio 17 2022" -A=x64 -DLLVM_TARGETS_TO_BUILD=X86 -Thost=x64 -S ..\llvm -B .
