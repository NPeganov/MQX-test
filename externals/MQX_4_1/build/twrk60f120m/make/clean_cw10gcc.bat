@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrk60f120m
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\ffs\build\make\ffs_twrk60f120m
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrk60f120m
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrk60f120m
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrk60f120m
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrk60f120m
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrk60f120m
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrk60f120m
cmd /c clean_cw10gcc nopause

if not "%1" == "nopause" (pause)
