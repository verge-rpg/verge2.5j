@echo off
if exist vcc.exe del vcc.exe
gcc -s -Wall -Werror -m486 -O3 vcc.cpp preproc.cpp lexical.cpp compile.cpp funclib.cpp linked.cpp -o dj\vcc.exe
rem gcc -Wall -Werror -pg vcc.cc preproc.cpp lexical.cpp compile.cpp funclib.cpp -o dj\vcc
if not exist dj\vcc.exe goto end
if exist ..\djp.exe djp dj\vcc.exe
:end
