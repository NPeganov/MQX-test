@echo off

rem convert path to backslash format
set ROOTDIR=%1
set ROOTDIR=%ROOTDIR:/=\%
set ROOTDIR=%ROOTDIR:"=%
set OUTPUTDIR=%2
set OUTPUTDIR=%OUTPUTDIR:/=\%
set OUTPUTDIR=%OUTPUTDIR:"=%
set TOOL=%3



rem copy common files
IF NOT EXIST "%OUTPUTDIR%\" mkdir "%OUTPUTDIR%\"
copy "%ROOTDIR%\ffs\source\nandflash_wl_ffs.h" "%OUTPUTDIR%\nandflash_wl_ffs.h" /Y
copy "%ROOTDIR%\ffs\source\nandflash_wl.h" "%OUTPUTDIR%\nandflash_wl.h" /Y
copy "%ROOTDIR%\ffs\source\nandflash_wl_config.h" "%OUTPUTDIR%\nandflash_wl_config.h" /Y


goto tool_%TOOL%

rem iar files
:tool_iar
goto copy_end

rem ds5 files
:tool_ds5
goto copy_end

rem gcc_arm files
:tool_gcc_arm
goto copy_end

:copy_end
