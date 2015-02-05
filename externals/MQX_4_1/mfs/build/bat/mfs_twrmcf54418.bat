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
copy "%ROOTDIR%\mfs\source\include\part_mgr.h" "%OUTPUTDIR%\part_mgr.h" /Y
copy "%ROOTDIR%\mfs\source\include\mfs.h" "%OUTPUTDIR%\mfs.h" /Y
copy "%ROOTDIR%\mfs\source\include\mfs_rev.h" "%OUTPUTDIR%\mfs_rev.h" /Y
copy "%ROOTDIR%\mfs\source\include\mfs_cnfg.h" "%OUTPUTDIR%\mfs_cnfg.h" /Y


goto tool_%TOOL%

rem cw10 files
:tool_cw10
goto copy_end

:copy_end

