/*HEADER**********************************************************************
*
* Copyright 2010 Freescale Semiconductor, Inc.
* Copyright 2004-2008 Embedded Access Inc.
* Copyright 1989-2008 ARC International
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:

*
*   The Common Configurable information for Nand Flash boot loader
*   
*
*
*END************************************************************************/
#ifndef _nandbootloader_h_
#define _nandbootloader_h_

#include "config.h"

#if BSP_TWRMPC5125 || BSP_TWRMCF54418 || BSP_M5329EVB
   #define BOOTLOADER_NAND_FLASH_SUPPORT 1
#elif BSP_M54455EVB
   #define FLASH_NAME_INIT              "flashx1:"
   #define BOOTLOADER_NOR_FLASH_SUPPORT 1
#elif BSP_M53015EVB
   #define FLASH_NAME_INIT              "flash:"   
   #define BOOTLOADER_NOR_FLASH_SUPPORT 1
#endif

#if BOOTLOADER_NAND_FLASH_SUPPORT
   #define FLASH_NAME_INIT              "nandflash:"
   #define IMAGE_START_BLOCK            (300)
   #define NAND_FLASH_PAGE_SIZE         BSP_VIRTUAL_PAGE_SIZE
   #define NAND_FLASH_BLOCK_SIZE        BSP_NAND_BLOCK_SIZE
   #define NAND_FLASH_NUM_BLOCKS        BSP_NAND_NUM_BLOCKS
   #define IMAGE_TABLE_BLOCK_LOCATION   (NAND_FLASH_NUM_BLOCKS-1)
   #define IMAGE_TABLE_LOCATION         (IMAGE_TABLE_BLOCK_LOCATION * (NAND_FLASH_BLOCK_SIZE/NAND_FLASH_PAGE_SIZE))
   #define IMAGE_TABLE_SIZE             (1)              /* size in page */
   #define RW_UNIT_SIZE                 (1)
#elif BOOTLOADER_NOR_FLASH_SUPPORT
   #define IMAGE_START_SECTOR           (30)
   #define IMAGE_TABLE_LOCATION         (0x00100000)
   #define IMAGE_TABLE_SIZE             (512)
   #define RW_UNIT_SIZE                 (512)
   #define NAND_FLASH_PAGE_SIZE         (512)
#endif

#define IMAGE_VALID                    (0xFFFF1010)
#define IMAGE_DELETED                  (0x01 << 0)
#define IMAGE_END_TABLE                (0x01 << 1)

#define AUTOBOOT_TIMEOUT               (5)
#define RESET_MAGIC_NUMBER             (0x52535445)

/* Error code */
#define BOOTLOADER_OK                  (0)
#define BOOTLOADER_ERROR_BASE          (0x80)
#define FLASH_IMAGE_SUCCESS            (0)
#define FLASH_ERASE_UNIT_FAILED        (BOOTLOADER_ERROR_BASE | 0x01)
#define CHECK_SUM_IMAGE_FAILED         (BOOTLOADER_ERROR_BASE | 0x02)
#define INVALID_PARAMETER              (BOOTLOADER_ERROR_BASE | 0x03)
#define WRITE_TABLE_FAILED             (BOOTLOADER_ERROR_BASE | 0x04)
#define WRITE_DATA_FAILED              (BOOTLOADER_ERROR_BASE | 0x05)
#define BOOTLOADER_OUT_OF_MEMORY       (BOOTLOADER_ERROR_BASE | 0x06)
#define PARSE_IMAGE_ERROR              (BOOTLOADER_ERROR_BASE | 0x07)
#define BOOTLOADER_S19_ERROR           (BOOTLOADER_ERROR_BASE | 0x08)
#define FLASH_IMAGE_ERROR              (BOOTLOADER_ERROR_BASE | 0x09)

/* File types */
#define UNKNOWN                        (0)
#define RAW_BINARY                     (1)
#define CODE_WARRIOR_BINARY            (2)
#define S19_RECORD                     (3)
#define S19_RECORD_HEADER              (0x53303033)

/* Image commands */
#define IMAGE_GET_TIME_OUT             (1)
#define IMAGE_SET_TIME_OUT             (2)
#define IMAGE_GET_NAME                 (3)
#define IMAGE_SET_NAME                 (4)
#define IMAGE_SET_BOOT_ADDR            (5)
#define IMAGE_GET_BOOT_ADDR            (6)
#define IMAGE_SET_AUTO_BOOT            (7)
#define IMAGE_GET_AUTO_BOOT            (8)

#define AUTOBOOT_ENABLE                (1)
#define AUTOBOOT_DISABLE               (2)
#define AUTOBOOT_RESET                 (3)

#define IMAGE_NAME_LENGTH              (20)

typedef struct image_info {
   /* Validating number of image */
   uint32_t VALID;
   /* Table index number */
   uint32_t INDEX;
   /* Boot address should be found in liker map file image */
   uint32_t BOOT_ADDR;
   /* Data size of image */
   uint32_t SIZE;
   /* Image check sum number */
   uint32_t CHECK_SUM;
   /* Block(Sector) location in NAND(NOR) flash support */
   uint32_t BLOCK;
   /* Data size in block (sector) if NAND(NOR) support*/
   uint32_t BLOCK_NUM;
   /* Image property */
   uint32_t PROPERTY;
   /* Auto boot flag */
   bool AUTO;
   /* Name of image */
   unsigned char   NAME[IMAGE_NAME_LENGTH];
   /* Auto boot time out in second */
   uint32_t BOOT_TIMEOUT;
} IMAGE_INFO, * IMAGE_INFO_PTR;

int32_t _bootloader_init_table(MQX_FILE_PTR);
uint32_t _bootloader_del_image(MQX_FILE_PTR, uint32_t);
void _bootloader_list_image(MQX_FILE_PTR);
int32_t _bootloader_set_default(uint32_t);
void _bootloader_reset(void);
int32_t _bootloader_get_autoboot(MQX_FILE_PTR, uint32_t *, uint32_t *);
int32_t _bootloader_del_table(MQX_FILE_PTR);
int32_t _bootloader_set_timeout(uint32_t, uint32_t);
int32_t _bootloader_check_available (MQX_FILE_PTR, uint32_t *, uint32_t *);
int32_t _bootloader_update_table(MQX_FILE_PTR,uint32_t, uint32_t,uint32_t,uint32_t,unsigned char *);
int32_t _bootloader_load(  MQX_FILE_PTR, uint32_t);
int32_t _bootloader_imgctl ( MQX_FILE_PTR, uint32_t, uint32_t, void *);
int32_t _bootloader_check_bad_block (MQX_FILE_PTR);
uint32_t  _bootloader_store_image_data (MQX_FILE_PTR, unsigned char *, uint32_t, bool, uint32_t, unsigned char *, bool);
#endif

/* EOF */
