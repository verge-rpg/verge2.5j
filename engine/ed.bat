@echo off
if exist %1 goto a
if exist watcom\%1 goto b
goto end
:a
edit /h %1
goto end
:b
edit /h watcom\%1
:end
