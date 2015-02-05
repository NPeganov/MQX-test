@echo OFF
set NOPAUSE=%1

mingw32-make TOOL=cw10 CONFIG=debug LOAD=extmram clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=cw10 CONFIG=debug LOAD=intflash clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=cw10 CONFIG=release LOAD=intflash clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

if not "%1" == "nopause" (pause)
