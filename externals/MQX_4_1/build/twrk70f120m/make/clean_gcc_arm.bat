@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_twrk70f120m
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\ffs\build\make\ffs_twrk70f120m
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_twrk70f120m
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\mqx\build\make\psp_twrk70f120m
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_twrk70f120m
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\shell\build\make\shell_twrk70f120m
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_twrk70f120m
cmd /c clean_gcc_arm nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_twrk70f120m
cmd /c clean_gcc_arm nopause

if not "%1" == "nopause" (pause)
