@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrvf65gs10_m4
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\ffs\build\make\ffs_twrvf65gs10_m4
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\mcc\build\make\mcc_twrvf65gs10_m4
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrvf65gs10_m4
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrvf65gs10_m4
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrvf65gs10_m4
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrvf65gs10_m4
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrvf65gs10_m4
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrvf65gs10_m4
cmd /c clean_gcc_arm nopause

if not "%1" == "nopause" (pause)
