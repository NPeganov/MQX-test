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
copy "%ROOTDIR%\mcc\source\include\mcc_vf600.h" "%OUTPUTDIR%\mcc_vf600.h" /Y
copy "%ROOTDIR%\mcc\source\include\mcc_api.h" "%OUTPUTDIR%\mcc_api.h" /Y
copy "%ROOTDIR%\mcc\source\include\mcc_common.h" "%OUTPUTDIR%\mcc_common.h" /Y
copy "%ROOTDIR%\mcc\source\include\mcc_mqx.h" "%OUTPUTDIR%\mcc_mqx.h" /Y
copy "%ROOTDIR%\mcc\source\include\mcc_config.h" "%OUTPUTDIR%\mcc_config.h" /Y


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

