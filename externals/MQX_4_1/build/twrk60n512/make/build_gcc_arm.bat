@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrk60n512
cmd /c build_gcc_arm nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrk60n512
cmd /c build_gcc_arm nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrk60n512
cmd /c build_gcc_arm nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrk60n512
cmd /c build_gcc_arm nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrk60n512
cmd /c build_gcc_arm nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrk60n512
cmd /c build_gcc_arm nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrk60n512
cmd /c build_gcc_arm nopause

if not "%1" == "nopause" (pause)
