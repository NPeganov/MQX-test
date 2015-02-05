@echo OFF
set NOPAUSE=%1

mingw32-make TOOL=ds5 CONFIG=debug LOAD=ddr clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=ds5 CONFIG=debug LOAD=intram clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=ds5 CONFIG=release LOAD=ddr clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=ds5 CONFIG=release LOAD=intram clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

if not "%1" == "nopause" (pause)
