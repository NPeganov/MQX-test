@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_kwikstikk40x256
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_kwikstikk40x256
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\mqx\build\make\psp_kwikstikk40x256
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_kwikstikk40x256
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\shell\build\make\shell_kwikstikk40x256
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_kwikstikk40x256
cmd /c build_cw10gcc nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_kwikstikk40x256
cmd /c build_cw10gcc nopause

if not "%1" == "nopause" (pause)
