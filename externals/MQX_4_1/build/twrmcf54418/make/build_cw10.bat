@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrmcf54418
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\ffs\build\make\ffs_twrmcf54418
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrmcf54418
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrmcf54418
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrmcf54418
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrmcf54418
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrmcf54418
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrmcf54418
cmd /c build_cw10 nopause

if not "%1" == "nopause" (pause)
