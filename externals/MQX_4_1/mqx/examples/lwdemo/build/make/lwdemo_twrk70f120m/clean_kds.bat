@echo OFF
set NOPAUSE=%1

mingw32-make TOOL=kds CONFIG=debug LOAD=intflash_ddrdata clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=kds CONFIG=debug LOAD=intflash_sramdata clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=kds CONFIG=release LOAD=intflash_ddrdata clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=kds CONFIG=release LOAD=intflash_sramdata clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

if not "%1" == "nopause" (pause)
