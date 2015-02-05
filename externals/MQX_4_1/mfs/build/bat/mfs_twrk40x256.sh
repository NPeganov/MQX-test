#!/bin/sh

# expect forward slash paths
ROOTDIR="${1}"
OUTPUTDIR="${2}"
TOOL="${3}"


# copy common files
mkdir -p "${OUTPUTDIR}"
mkdir -p "${OUTPUTDIR}/"
cp -f "${ROOTDIR}/mfs/source/include/part_mgr.h" "${OUTPUTDIR}/part_mgr.h"
cp -f "${ROOTDIR}/mfs/source/include/mfs.h" "${OUTPUTDIR}/mfs.h"
cp -f "${ROOTDIR}/mfs/source/include/mfs_rev.h" "${OUTPUTDIR}/mfs_rev.h"
cp -f "${ROOTDIR}/mfs/source/include/mfs_cnfg.h" "${OUTPUTDIR}/mfs_cnfg.h"


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


