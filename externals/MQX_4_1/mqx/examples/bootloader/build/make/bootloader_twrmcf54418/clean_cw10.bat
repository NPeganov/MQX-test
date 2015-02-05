@echo OFF
set NOPAUSE=%1

mingw32-make TOOL=cw10 CONFIG=debug LINKER_FILE=$(MQX_ROOTDIR)/mqx/examples/bootloader/lcf/twrmcf54418/extflash.lcf clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=cw10 CONFIG=debug LINKER_FILE=$(MQX_ROOTDIR)/mqx/examples/bootloader/lcf/twrmcf54418/extram.lcf clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

mingw32-make TOOL=cw10 CONFIG=release LINKER_FILE=$(MQX_ROOTDIR)/mqx/examples/bootloader/lcf/twrmcf54418/extflash.lcf clean 
if errorlevel 1 (
set NOPAUSE=0
pause
)

if not "%1" == "nopause" (pause)
