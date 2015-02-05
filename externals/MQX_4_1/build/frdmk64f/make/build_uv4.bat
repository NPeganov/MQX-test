@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_frdmk64f
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_frdmk64f
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\mqx\build\make\psp_frdmk64f
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_frdmk64f
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\shell\build\make\shell_frdmk64f
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_frdmk64f
cmd /c build_uv4 nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_frdmk64f
cmd /c build_uv4 nopause

if not "%1" == "nopause" (pause)
