@echo off
ml64.exe /c control.txt.asm
link.exe control.txt.obj "D:\KAD-2025-CourseProject-main\KAD-2025\x64\Debug\StaticLib.lib" /out:result.exe
result.exe
pause