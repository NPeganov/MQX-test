@echo OFF
SET CWD=%CD%

setlocal
cd %CWD%\..\..\..\mqx\build\make\bsp_vybrid_autoevb_a5
cmd /c build_iar nopause
cd %CWD%\..\..\..\mcc\build\make\mcc_vybrid_autoevb_a5
cmd /c build_iar nopause
cd %CWD%\..\..\..\mfs\build\make\mfs_vybrid_autoevb_a5
cmd /c build_iar nopause
cd %CWD%\..\..\..\mqx\build\make\psp_vybrid_autoevb_a5
cmd /c build_iar nopause
cd %CWD%\..\..\..\rtcs\build\make\rtcs_vybrid_autoevb_a5
cmd /c build_iar nopause
cd %CWD%\..\..\..\shell\build\make\shell_vybrid_autoevb_a5
cmd /c build_iar nopause
cd %CWD%\..\..\..\usb\device\build\make\usbd_vybrid_autoevb_a5
cmd /c build_iar nopause
cd %CWD%\..\..\..\usb\host\build\make\usbh_vybrid_autoevb_a5
cmd /c build_iar nopause

if not "%1" == "nopause" (pause)
