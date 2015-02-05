@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrk60n512
cmd /c clean_iar nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrk60n512
cmd /c clean_iar nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrk60n512
cmd /c clean_iar nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrk60n512
cmd /c clean_iar nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrk60n512
cmd /c clean_iar nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrk60n512
cmd /c clean_iar nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrk60n512
cmd /c clean_iar nopause

if not "%1" == "nopause" (pause)
