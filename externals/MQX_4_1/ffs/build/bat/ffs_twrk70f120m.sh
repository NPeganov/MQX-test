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


# iar files
if [ "${TOOL}" = "iar" ]; then
:
fi

# cw10gcc files
if [ "${TOOL}" = "cw10gcc" ]; then
:
fi

# kds files
if [ "${TOOL}" = "kds" ]; then
:
fi

# uv4 files
if [ "${TOOL}" = "uv4" ]; then
:
fi

# gcc_arm files
if [ "${TOOL}" = "gcc_arm" ]; then
:
fi


