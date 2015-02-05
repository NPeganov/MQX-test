@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrmcf51jf
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrmcf51jf
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrmcf51jf
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrmcf51jf
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrmcf51jf
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrmcf51jf
cmd /c build_cw10 nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrmcf51jf
cmd /c build_cw10 nopause

if not "%1" == "nopause" (pause)
