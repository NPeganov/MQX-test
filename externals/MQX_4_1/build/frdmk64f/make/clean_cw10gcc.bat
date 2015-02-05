@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_frdmk64f
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_frdmk64f
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\mqx\build\make\psp_frdmk64f
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_frdmk64f
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\shell\build\make\shell_frdmk64f
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_frdmk64f
cmd /c clean_cw10gcc nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_frdmk64f
cmd /c clean_cw10gcc nopause

if not "%1" == "nopause" (pause)
