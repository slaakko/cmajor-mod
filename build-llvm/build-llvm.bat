@echo off
rem git clone https://github.com/llvm/llvm-project llvm-project
D:
mkdir D:\llvm-project\build
cd D:\llvm-project\build
cmake -G "Visual Studio 17 2022" -A=x64 -DLLVM_ENABLE_PROJECTS=clang;lld;lldb -DLLVM_TARGETS_TO_BUILD=X86 -Thost=x64 -S ..\llvm -B .
