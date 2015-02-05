#!/bin/sh

# expect forward slash paths
ROOTDIR="${1}"
OUTPUTDIR="${2}"
TOOL="${3}"

# remove processor expert files if exists
rm -rf "${OUTPUTDIR}/Generated_Code"
rm -rf "${OUTPUTDIR}/Sources"

# copy common files
mkdir -p "${OUTPUTDIR}"
mkdir -p "${OUTPUTDIR}/"
mkdir -p "${OUTPUTDIR}/.."
mkdir -p "${OUTPUTDIR}/Generated_Code"
cp -f "${ROOTDIR}/mqx/source/io/lwgpio/lwgpio.h" "${OUTPUTDIR}/lwgpio.h"
cp -f "${ROOTDIR}/mqx/source/io/hwtimer/hwtimer_pit.h" "${OUTPUTDIR}/hwtimer_pit.h"
cp -f "${ROOTDIR}/mqx/source/io/spi/spi_dspi_common.h" "${OUTPUTDIR}/spi_dspi_common.h"
cp -f "${ROOTDIR}/mqx/source/io/i2c/i2c_ki2c.h" "${OUTPUTDIR}/i2c_ki2c.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/PE_Types.h" "${OUTPUTDIR}/Generated_Code/PE_Types.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/PE_LDD.h" "${OUTPUTDIR}/Generated_Code/PE_LDD.h"
cp -f "${ROOTDIR}/mqx/source/io/pcflash/ata.h" "${OUTPUTDIR}/ata.h"
cp -f "${ROOTDIR}/mqx/source/io/pcflash/pcflash.h" "${OUTPUTDIR}/pcflash.h"
cp -f "${ROOTDIR}/mqx/source/io/sai/int/sai_int_prv.h" "${OUTPUTDIR}/sai_int_prv.h"
cp -f "${ROOTDIR}/mqx/source/io/hwtimer/hwtimer_lpt.h" "${OUTPUTDIR}/hwtimer_lpt.h"
cp -f "${ROOTDIR}/mqx/source/io/debug/iodebug.h" "${OUTPUTDIR}/iodebug.h"
cp -f "${ROOTDIR}/mqx/source/include/mqx.h" "${OUTPUTDIR}/mqx.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/init_lpm.h" "${OUTPUTDIR}/init_lpm.h"
cp -f "${ROOTDIR}/mqx/source/io/cm/cm_kinetis.h" "${OUTPUTDIR}/cm_kinetis.h"
cp -f "${ROOTDIR}/mqx/source/io/spi/spi_dspi_dma.h" "${OUTPUTDIR}/spi_dspi_dma.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/enet.h" "${OUTPUTDIR}/enet.h"
cp -f "${ROOTDIR}/mqx/source/io/cm/cm.h" "${OUTPUTDIR}/cm.h"
cp -f "${ROOTDIR}/mqx/source/io/flashx/flashx.h" "${OUTPUTDIR}/flashx.h"
cp -f "${ROOTDIR}/mqx/source/io/timer/timer_qpit.h" "${OUTPUTDIR}/timer_qpit.h"
cp -f "${ROOTDIR}/mqx/source/io/usb/if_dev_khci.h" "${OUTPUTDIR}/if_dev_khci.h"
cp -f "${ROOTDIR}/mqx/source/io/tfs/tfs.h" "${OUTPUTDIR}/tfs.h"
cp -f "${ROOTDIR}/mqx/source/io/serial/serl_kuart.h" "${OUTPUTDIR}/serl_kuart.h"
cp -f "${ROOTDIR}/mqx/source/io/pcflash/apcflshpr.h" "${OUTPUTDIR}/apcflshpr.h"
cp -f "${ROOTDIR}/mqx/source/io/gpio/io_gpio.h" "${OUTPUTDIR}/io_gpio.h"
cp -f "${ROOTDIR}/config/common/small_ram_config.h" "${OUTPUTDIR}/../small_ram_config.h"
cp -f "${ROOTDIR}/mqx/source/io/pcb/io_pcb.h" "${OUTPUTDIR}/io_pcb.h"
cp -f "${ROOTDIR}/config/common/maximum_config.h" "${OUTPUTDIR}/../maximum_config.h"
cp -f "${ROOTDIR}/mqx/source/io/usb/usb_bsp.h" "${OUTPUTDIR}/usb_bsp.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/bsp_rev.h" "${OUTPUTDIR}/bsp_rev.h"
cp -f "${ROOTDIR}/mqx/source/io/adc/adc.h" "${OUTPUTDIR}/adc.h"
cp -f "${ROOTDIR}/mqx/source/io/can/flexcan/kflexcan.h" "${OUTPUTDIR}/kflexcan.h"
cp -f "${ROOTDIR}/mqx/source/io/lpm/lpm_kinetis.h" "${OUTPUTDIR}/lpm_kinetis.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/ethernet.h" "${OUTPUTDIR}/ethernet.h"
cp -f "${ROOTDIR}/mqx/source/io/adc/kadc/adc_mk21f.h" "${OUTPUTDIR}/adc_mk21f.h"
cp -f "${ROOTDIR}/mqx/source/io/pccard/apccard.h" "${OUTPUTDIR}/apccard.h"
cp -f "${ROOTDIR}/mqx/source/io/lpm/lpm.h" "${OUTPUTDIR}/lpm.h"
cp -f "${ROOTDIR}/mqx/source/io/sdcard/sdcard_esdhc/sdcard_esdhc.h" "${OUTPUTDIR}/sdcard_esdhc.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/twrk21f120m.h" "${OUTPUTDIR}/twrk21f120m.h"
cp -f "${ROOTDIR}/mqx/source/io/dma/dma.h" "${OUTPUTDIR}/dma.h"
cp -f "${ROOTDIR}/config/common/verif_enabled_config.h" "${OUTPUTDIR}/../verif_enabled_config.h"
cp -f "${ROOTDIR}/mqx/source/io/sai/sai_ksai.h" "${OUTPUTDIR}/sai_ksai.h"
cp -f "${ROOTDIR}/mqx/source/io/dma/edma.h" "${OUTPUTDIR}/edma.h"
cp -f "${ROOTDIR}/mqx/source/io/adc/kadc/adc_kadc.h" "${OUTPUTDIR}/adc_kadc.h"
cp -f "${ROOTDIR}/mqx/source/io/hwtimer/hwtimer.h" "${OUTPUTDIR}/hwtimer.h"
cp -f "${ROOTDIR}/mqx/source/io/esdhc/esdhc.h" "${OUTPUTDIR}/esdhc.h"
cp -f "${ROOTDIR}/config/twrk21f120m/user_config.h" "${OUTPUTDIR}/../user_config.h"
cp -f "${ROOTDIR}/mqx/source/io/lwadc/lwadc_kadc.h" "${OUTPUTDIR}/lwadc_kadc.h"
cp -f "${ROOTDIR}/mqx/source/io/flashx/freescale/flash_ftfe.h" "${OUTPUTDIR}/flash_ftfe.h"
cp -f "${ROOTDIR}/mqx/source/io/io_mem/io_mem.h" "${OUTPUTDIR}/io_mem.h"
cp -f "${ROOTDIR}/mqx/source/io/spi/spi_dspi.h" "${OUTPUTDIR}/spi_dspi.h"
cp -f "${ROOTDIR}/mqx/source/io/hwtimer/hwtimer_systick.h" "${OUTPUTDIR}/hwtimer_systick.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/bsp.h" "${OUTPUTDIR}/bsp.h"
cp -f "${ROOTDIR}/mqx/source/io/lwadc/lwadc.h" "${OUTPUTDIR}/lwadc.h"
cp -f "${ROOTDIR}/mqx/source/io/sdcard/sdcard_spi/sdcard_spi.h" "${OUTPUTDIR}/sdcard_spi.h"
cp -f "${ROOTDIR}/mqx/source/io/usb/if_host_khci.h" "${OUTPUTDIR}/if_host_khci.h"
cp -f "${ROOTDIR}/mqx/source/io/dma/edma_prv.h" "${OUTPUTDIR}/edma_prv.h"
cp -f "${ROOTDIR}/mqx/source/io/rtc/krtc.h" "${OUTPUTDIR}/krtc.h"
cp -f "${ROOTDIR}/mqx/source/io/spi/spi.h" "${OUTPUTDIR}/spi.h"
cp -f "${ROOTDIR}/mqx/source/io/serial/serial.h" "${OUTPUTDIR}/serial.h"
cp -f "${ROOTDIR}/mqx/source/io/pccard/pccardflexbus.h" "${OUTPUTDIR}/pccardflexbus.h"
cp -f "${ROOTDIR}/mqx/source/io/pcb/mqxa/pcbmqxav.h" "${OUTPUTDIR}/pcbmqxav.h"
cp -f "${ROOTDIR}/mqx/source/io/usb/if_host_ehci.h" "${OUTPUTDIR}/if_host_ehci.h"
cp -f "${ROOTDIR}/mqx/source/io/pcb/mqxa/pcb_mqxa.h" "${OUTPUTDIR}/pcb_mqxa.h"
cp -f "${ROOTDIR}/mqx/source/io/sai/sai.h" "${OUTPUTDIR}/sai.h"
cp -f "${ROOTDIR}/mqx/source/io/usb/if_dev_ehci.h" "${OUTPUTDIR}/if_dev_ehci.h"
cp -f "${ROOTDIR}/mqx/source/io/rtc/rtc.h" "${OUTPUTDIR}/rtc.h"
cp -f "${ROOTDIR}/mqx/source/io/pipe/io_pipe.h" "${OUTPUTDIR}/io_pipe.h"
cp -f "${ROOTDIR}/mqx/source/io/timer/qpit.h" "${OUTPUTDIR}/qpit.h"
cp -f "${ROOTDIR}/mqx/source/io/flashx/freescale/flash_mk21.h" "${OUTPUTDIR}/flash_mk21.h"
cp -f "${ROOTDIR}/mqx/source/io/enet/enet_wifi.h" "${OUTPUTDIR}/enet_wifi.h"
cp -f "${ROOTDIR}/mqx/source/io/lwgpio/lwgpio_kgpio.h" "${OUTPUTDIR}/lwgpio_kgpio.h"
cp -f "${ROOTDIR}/mqx/source/io/io_null/io_null.h" "${OUTPUTDIR}/io_null.h"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/bsp_cm.h" "${OUTPUTDIR}/bsp_cm.h"
cp -f "${ROOTDIR}/mqx/source/io/gpio/kgpio/io_gpio_kgpio.h" "${OUTPUTDIR}/io_gpio_kgpio.h"
cp -f "${ROOTDIR}/mqx/source/io/i2c/i2c.h" "${OUTPUTDIR}/i2c.h"
cp -f "${ROOTDIR}/mqx/source/io/sai/sai_audio.h" "${OUTPUTDIR}/sai_audio.h"
cp -f "${ROOTDIR}/mqx/source/io/sdcard/sdcard.h" "${OUTPUTDIR}/sdcard.h"
cp -f "${ROOTDIR}/mqx/source/io/adc/adc_conf.h" "${OUTPUTDIR}/adc_conf.h"
cp -f "${ROOTDIR}/config/common/smallest_config.h" "${OUTPUTDIR}/../smallest_config.h"


# iar files
if [ "${TOOL}" = "iar" ]; then
mkdir -p "${OUTPUTDIR}/"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/iar/intflash.icf" "${OUTPUTDIR}/intflash.icf"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/iar/ram.icf" "${OUTPUTDIR}/ram.icf"
mkdir -p "${OUTPUTDIR}/Generated_Code"
mkdir -p "${OUTPUTDIR}/Sources"
:
fi

# cw10gcc files
if [ "${TOOL}" = "cw10gcc" ]; then
mkdir -p "${OUTPUTDIR}/"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/gcc_cw/intflash.ld" "${OUTPUTDIR}/intflash.ld"
mkdir -p "${OUTPUTDIR}/Generated_Code"
mkdir -p "${OUTPUTDIR}/Sources"
:
fi

# kds files
if [ "${TOOL}" = "kds" ]; then
mkdir -p "${OUTPUTDIR}/"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/gcc_arm/intflash.ld" "${OUTPUTDIR}/intflash.ld"
mkdir -p "${OUTPUTDIR}/Generated_Code"
mkdir -p "${OUTPUTDIR}/Sources"
:
fi

# uv4 files
if [ "${TOOL}" = "uv4" ]; then
mkdir -p "${OUTPUTDIR}/"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/uv4/intflash.scf" "${OUTPUTDIR}/intflash.scf"
mkdir -p "${OUTPUTDIR}/Generated_Code"
mkdir -p "${OUTPUTDIR}/Sources"
:
fi

# gcc_arm files
if [ "${TOOL}" = "gcc_arm" ]; then
mkdir -p "${OUTPUTDIR}/"
cp -f "${ROOTDIR}/mqx/source/bsp/twrk21f120m/gcc_arm/intflash.ld" "${OUTPUTDIR}/intflash.ld"
mkdir -p "${OUTPUTDIR}/Generated_Code"
mkdir -p "${OUTPUTDIR}/Sources"
:
fi


