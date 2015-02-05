@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrk21f120m
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrk21f120m
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrk21f120m
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrk21f120m
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrk21f120m
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrk21f120m
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrk21f120m
cmd /c build_uv4 nopause

if not "%1" == "nopause" (pause)
