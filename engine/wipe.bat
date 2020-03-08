@echo off
call clean.bat
if exist verge.exe del verge.exe
if exist verge.lib del verge.lib
if exist verge.bak del verge.bak
if exist *.obj del *.obj
if exist *.err del *.err
echo   Wiped!
