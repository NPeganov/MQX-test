@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrk64f120m
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrk64f120m
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrk64f120m
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrk64f120m
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrk64f120m
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrk64f120m
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrk64f120m
cmd /c build_cw10gcc nopause

if not "%1" == "nopause" (pause)