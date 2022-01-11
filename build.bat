@echo off

set "diag_options= /nologo /FC /diagnostics:column /W4 /wd4996 /wd4116 /wd4100 /wd4201 /wd4101 /wd4204 /wd4200"
set "nocrt_options= /Zl /GR- /GS- /Gs9999999"
set "noctr_link= /STACK:0x100000,0x100000 /NODEFAULTLIB /SUBSYSTEM:windows"
set "fast_fp= /fp:fast /fp:except-"

IF NOT EXIST .\build mkdir build
pushd .\build

set "compile_options= /DMM_DEBUG %nocrt_options% %diag_options% /Od /Zo /Zf /Z7"
set "link_options= /INCREMENTAL:NO /opt:ref /STACK:0x100000,0x100000 /NODEFAULTLIB /SUBSYSTEM:console"

cl %compile_options% ..\src\main.c /link %link_options% /PDB:dcloc.pdb /ENTRY:WinMainCRTStartup Kernel32.lib Winmm.lib User32.lib Shell32.lib Shlwapi.lib /out:dcloc.exe

del main.obj

popd
