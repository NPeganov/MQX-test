@echo OFF
set NOPAUSE=%1

mingw32-make TOOL=ds5 CONFIG=debug LINKER_FILE=$(MQX_ROOTDIR)/mqx/examples/bootloader_vybrid/sram_ds5_m4.scf build -j3 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=ds5 CONFIG=release LINKER_FILE=$(MQX_ROOTDIR)/mqx/examples/bootloader_vybrid/sram_ds5_m4.scf build -j3 
if errorlevel 1 (
set NOPAUSE=0
pause
)

if not "%1" == "nopause" (pause)
