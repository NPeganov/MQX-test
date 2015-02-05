@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrk70f120m
cmd /c build_iar nopause
cd %CWD%\..\..\..\ffs\build\make\ffs_twrk70f120m
cmd /c build_iar nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrk70f120m
cmd /c build_iar nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrk70f120m
cmd /c build_iar nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrk70f120m
cmd /c build_iar nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrk70f120m
cmd /c build_iar nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrk70f120m
cmd /c build_iar nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrk70f120m
cmd /c build_iar nopause

if not "%1" == "nopause" (pause)
