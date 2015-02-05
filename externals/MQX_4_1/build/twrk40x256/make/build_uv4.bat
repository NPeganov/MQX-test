@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrk40x256
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrk40x256
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrk40x256
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrk40x256
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrk40x256
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrk40x256
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrk40x256
cmd /c build_uv4 nopause

if not "%1" == "nopause" (pause)
