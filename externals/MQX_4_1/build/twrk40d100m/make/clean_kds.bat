@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrk40d100m
cmd /c clean_kds nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrk40d100m
cmd /c clean_kds nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrk40d100m
cmd /c clean_kds nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrk40d100m
cmd /c clean_kds nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrk40d100m
cmd /c clean_kds nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrk40d100m
cmd /c clean_kds nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrk40d100m
cmd /c clean_kds nopause

if not "%1" == "nopause" (pause)
