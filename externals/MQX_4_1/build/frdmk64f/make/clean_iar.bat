@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_frdmk64f
cmd /c clean_iar nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_frdmk64f
cmd /c clean_iar nopause
cd %CWD%\..\..\..\mqx\build\make\psp_frdmk64f
cmd /c clean_iar nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_frdmk64f
cmd /c clean_iar nopause
cd %CWD%\..\..\..\shell\build\make\shell_frdmk64f
cmd /c clean_iar nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_frdmk64f
cmd /c clean_iar nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_frdmk64f
cmd /c clean_iar nopause

if not "%1" == "nopause" (pause)
