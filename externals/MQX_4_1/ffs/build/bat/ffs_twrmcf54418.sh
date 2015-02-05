#!/bin/sh

# expect forward slash paths
ROOTDIR="${1}"
OUTPUTDIR="${2}"
TOOL="${3}"


# copy common files
mkdir -p "${OUTPUTDIR}"
mkdir -p "${OUTPUTDIR}/"
cp -f "${ROOTDIR}/ffs/source/nandflash_wl_ffs.h" "${OUTPUTDIR}/nandflash_wl_ffs.h"
cp -f "${ROOTDIR}/ffs/source/nandflash_wl.h" "${OUTPUTDIR}/nandflash_wl.h"
cp -f "${ROOTDIR}/ffs/source/nandflash_wl_config.h" "${OUTPUTDIR}/nandflash_wl_config.h"


# cw10 files
if [ "${TOOL}" = "cw10" ]; then
:
fi


