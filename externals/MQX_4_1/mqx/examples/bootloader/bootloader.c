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
*       This file contains the basic functions for bootloader application
*
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "fio.h"
#include "io.h"
#include "bootloader.h"
#include "string.h"

uint32_t          _ram_start_addr;
extern uint8_t    _file_type;
#if BOOTLOADER_NOR_FLASH_SUPPORT
    FLASHX_BLOCK_INFO_STRUCT_PTR  info_ptr;
    uint32_t                       g_block_num;
#endif

static int32_t store_table_to_nand_flash(MQX_FILE_PTR, unsigned char *);
unsigned char *allocate_buffer(_mem_size buffer_size, char *string);
void bootloader_boot(void);
void free_buffer(unsigned char **buffer);
extern uint32_t parse_image_to_ram(uint8_t* arr,uint32_t length);
extern uint32_t _boot_addr;
static int32_t check_sum_image(MQX_FILE_PTR, IMAGE_INFO_PTR);

#if BOOTLOADER_NOR_FLASH_SUPPORT
uint32_t _nor_flash_get_seek_location( FLASHX_BLOCK_INFO_STRUCT_PTR, uint32_t);
uint32_t _nor_flash_get_sector_size(FLASHX_BLOCK_INFO_STRUCT_PTR, uint32_t);
#endif

extern void _bootstrap_reset(void);
extern void _bsp_exit_handler(void);
void _bootloader_exit(void);

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : allocate_buffer
* Returned Value   : pointer to alocated buffer
* Comments         :
*    This function alocate a system memory
*
*END*----------------------------------------------------------------------*/
unsigned char *allocate_buffer(_mem_size buffer_size, char *string)
{
    unsigned char *buffer_ptr;

    buffer_ptr = _mem_alloc_system_zero(buffer_size);

    if (buffer_ptr == NULL) {
        printf("\nFailed to get %s buffer", string);
    }
    return buffer_ptr;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : free_buffer
* Returned Value   : void
* Comments         :
*    This function free a system memory
*
*END*----------------------------------------------------------------------*/
void free_buffer(unsigned char **buffer)
{
    _mem_free(*buffer);
    *buffer = NULL;
    return;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bootloader_init_table
* Returned Value   : 0 if successful, -1 if has unexpected error
* Comments         :
*    This function initialize a empty table and check properly data if image ready
*
*END*----------------------------------------------------------------------*/
int32_t _bootloader_init_table(MQX_FILE_PTR flash_hdl)
{
    IMAGE_INFO_PTR      image;
    unsigned char           *read_buffer;
    int32_t              result = IO_ERROR;
    uint32_t             check_sum = 0;
    bool             image_found;

    if (flash_hdl == NULL) {
          printf("\nFlash device not opened");
          return IO_ERROR;
    }
    read_buffer = allocate_buffer(IMAGE_TABLE_SIZE*NAND_FLASH_PAGE_SIZE, "read");
    image = (IMAGE_INFO_PTR)read_buffer;
    image_found = FALSE;
#if BOOTLOADER_NAND_FLASH_SUPPORT
    if(NANDFLASHERR_BLOCK_BAD == ioctl(flash_hdl, NANDFLASH_IOCTL_CHECK_BLOCK, (void*)IMAGE_TABLE_BLOCK_LOCATION)) {
        printf("\nBlock %d is bad, please use another bock as table images!",IMAGE_TABLE_BLOCK_LOCATION);
    }
#elif BOOTLOADER_NOR_FLASH_SUPPORT
    result = ioctl(flash_hdl, FLASH_IOCTL_GET_BLOCK_GROUPS,&g_block_num);
    if (result != MQX_OK) {
        printf("\nFLASH_IOCTL_GET_NUM_BLOCKS failed");
    } /* Endif */
    info_ptr = _mem_alloc_system_zero(g_block_num*sizeof(FLASHX_BLOCK_INFO_STRUCT));
    result = ioctl(flash_hdl, FLASH_IOCTL_GET_BLOCK_MAP,
     (uint32_t *)&info_ptr);
    if (result != MQX_OK) {
        printf("\nFLASH_IOCTL_GET_BLOCK_MAP failed.");
    } /* Endif */
#endif
    fseek(flash_hdl, IMAGE_TABLE_LOCATION, IO_SEEK_SET);
    result = read(flash_hdl, read_buffer,IMAGE_TABLE_SIZE);

    while(IMAGE_VALID == image->VALID) {
            image_found = TRUE;
            result = BOOTLOADER_OK;
            image++;
    }
    image = (IMAGE_INFO_PTR)read_buffer;
    while(IMAGE_VALID == image->VALID) {
        if(!(image->PROPERTY & IMAGE_DELETED)){
            printf("\nImage %d check sum...",image->INDEX);
            if(CHECK_SUM_IMAGE_FAILED == check_sum_image(flash_hdl,image)) {
                printf("Error");
                if (TRUE == image->AUTO){
                    result = IO_ERROR;
                }
            }else {
                printf("OK");
            }
        }
            image++;
    }

    image = (IMAGE_INFO_PTR)read_buffer;
    if(!image_found){
        image->VALID = IMAGE_VALID;
        image->PROPERTY = IMAGE_DELETED | IMAGE_END_TABLE;
        image->BOOT_ADDR = 0;
        image->SIZE = 0;
#if BOOTLOADER_NAND_FLASH_SUPPORT
        image->BLOCK = IMAGE_START_BLOCK;
#else BOOTLOADER_NOR_FLASH_SUPPORT
        image->BLOCK = IMAGE_START_SECTOR;
#endif
        image->BLOCK_NUM = 0;

        if(BOOTLOADER_OK == store_table_to_nand_flash(flash_hdl,read_buffer)) {
            result = BOOTLOADER_OK;
        }
        else{
            result = IO_ERROR;
        }
    }
    free_buffer(&read_buffer);
    return result;
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : _bootloader_load
* Returned Value : 0 if successful, -1 if has unexpected error
* Comments       :
*   This function read data from flash device and copy to ram memory.
*
*END*--------------------------------------------------------------------*/
int32_t _bootloader_load
(
  /* [IN] The file handler */
  MQX_FILE_PTR     flash_hdl,
  /* [IN] The image index  */
  uint32_t      image_index
)
{ /* Body */
    IMAGE_INFO_PTR      image;
    unsigned char           *read_buffer;
    int32_t              result = IO_ERROR;
    uint32_t             check_sum = 0;
    uint32_t             image_check_sum;
    bool             image_found;
    int32_t              size, i;
    unsigned char           *s;
    uint32_t             seek_location;

    if (flash_hdl == NULL) {
          printf("\nFlash device not opened");
          return IO_ERROR;
    }
    read_buffer = allocate_buffer(IMAGE_TABLE_SIZE*NAND_FLASH_PAGE_SIZE, "read");
    image = (IMAGE_INFO_PTR)read_buffer;
    image_found = FALSE;
    fseek(flash_hdl, IMAGE_TABLE_LOCATION, IO_SEEK_SET);
    result = read(flash_hdl, read_buffer,IMAGE_TABLE_SIZE);

    image = (IMAGE_INFO_PTR)read_buffer;
    while(IMAGE_VALID == image->VALID) {
        if(!(image->PROPERTY & IMAGE_DELETED)){
            if(image->INDEX == image_index) {
                _boot_addr = image->BOOT_ADDR;
                break;
            }
        }
            image++;
    }
    size = image->SIZE;
    i = image->SIZE;
    s = (unsigned char *)image->BOOT_ADDR;

    image_check_sum = image->CHECK_SUM;
#if BOOTLOADER_NOR_FLASH_SUPPORT
     seek_location = _nor_flash_get_seek_location(info_ptr, image->BLOCK);
#elif BOOTLOADER_NAND_FLASH_SUPPORT
     seek_location = image->BLOCK*(NAND_FLASH_BLOCK_SIZE/NAND_FLASH_PAGE_SIZE);
#endif
    fseek(flash_hdl, seek_location, IO_SEEK_SET);
    while(i > 0) {
        if(RW_UNIT_SIZE != read(flash_hdl, read_buffer,RW_UNIT_SIZE)){
            printf("\nRead flash fail");
            break;
        };
        i -= NAND_FLASH_PAGE_SIZE;
        if(PARSE_IMAGE_ERROR == parse_image_to_ram(read_buffer, (uint32_t)(i>0 ? NAND_FLASH_PAGE_SIZE : NAND_FLASH_PAGE_SIZE+i)))
            break;
    }

    if(RAW_BINARY == _file_type){
        for(i = 0; i < size; i++) {
            check_sum += *s++;
        }

        if(image_check_sum != check_sum){
            free_buffer(&read_buffer);
            return IO_ERROR;
        }
    }
    free_buffer(&read_buffer);
    return BOOTLOADER_OK;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : bootloader_boot
* Returned Value   : void
* Comments         :
*    Execute boot from boot address
*
*END*----------------------------------------------------------------------*/

void bootloader_boot(void)
{
    void           (*boot)(void);
    /* Reset command, only support on Power PC */
    if(RESET_MAGIC_NUMBER == _boot_addr) {
#if BSP_TWRMPC5125
         _bootstrap_reset();
#elif BSP_M5329EVB
         _bsp_soft_reset();
#endif
    }
    boot = (void *)(_boot_addr);
    (*boot)();
    return;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bootloader_exit
* Returned Value   : void
* Comments         : Exit bootloader app. and execute the new image.
*
*
*END*----------------------------------------------------------------------*/

void _bootloader_exit(void) {
   _bsp_exit_handler();
   bootloader_boot();
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bootloader_get_timeout
* Returned Value   : 0 if successful, -1 if has unexpected error
* Comments         :
*    get boot time out of specific image
*
*END*----------------------------------------------------------------------*/
int32_t _bootloader_get_autoboot(MQX_FILE_PTR flash_hdl, uint32_t *timeout, uint32_t *image_index)
{
    IMAGE_INFO_PTR      image;
    unsigned char           *read_buffer;
    bool             image_found = FALSE;

    if(flash_hdl == NULL) {
        printf("\nFlash device not opened");
        return IO_ERROR;
    }
    read_buffer = allocate_buffer(IMAGE_TABLE_SIZE*NAND_FLASH_PAGE_SIZE, "read");
    image = (IMAGE_INFO_PTR)read_buffer;
    fseek(flash_hdl, IMAGE_TABLE_LOCATION, IO_SEEK_SET);
    read(flash_hdl, read_buffer,IMAGE_TABLE_SIZE);

    while(IMAGE_VALID == image->VALID) {
        if(image->AUTO == TRUE){
          *timeout = image->BOOT_TIMEOUT;
          *image_index = image->INDEX;
          free_buffer(&read_buffer);
          return BOOTLOADER_OK;
        }
      if(!(IMAGE_DELETED & image->PROPERTY)){
            image_found = TRUE;
        }
        image++;
    }
    free_buffer(&read_buffer);

    if (image_found){
        return BOOTLOADER_OK;
    }
    return IO_ERROR;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bootloader_imgctl
* Returned Value   : 0 if successful, -1 if has unexpected error
* Comments         :
*    set boot time out of specific image
*
*END*----------------------------------------------------------------------*/
int32_t _bootloader_imgctl
    (
        /* [IN] The opened file point to Flash driver */
        MQX_FILE_PTR        flash_hdl,

        /* [IN] The command to perform */
        uint32_t             cmd,

        /* [IN] The indicator to image */
        uint32_t             index,

        /* [IN/OUT] Parameters for the command */
        void               *param_ptr
    )
{
    IMAGE_INFO_PTR      image, image_tmp;
    unsigned char           *read_buffer;
    int32_t              result = IO_ERROR;

    if(flash_hdl == NULL) {
        printf("\nFlash device not opened");
        return IO_ERROR;
    }
    if( param_ptr == NULL){
         printf("\nInvalid parameter");
         return IO_ERROR;
    }
    image = NULL;
    read_buffer = allocate_buffer(IMAGE_TABLE_SIZE*NAND_FLASH_PAGE_SIZE, "read");
    image_tmp = (IMAGE_INFO_PTR)read_buffer;
    fseek(flash_hdl, IMAGE_TABLE_LOCATION, IO_SEEK_SET);
    read(flash_hdl, read_buffer,IMAGE_TABLE_SIZE);

    while(IMAGE_VALID == image_tmp->VALID) {
        if(index == image_tmp->INDEX){
            image = image_tmp;
        }
        if(IMAGE_SET_AUTO_BOOT == cmd){
             image_tmp->AUTO = FALSE;
        }
        image_tmp++;
    }

    if(NULL == image){
        printf("\nImage not found !");
        free_buffer(&read_buffer);
        return BOOTLOADER_OK;
    }
    switch(cmd){
        case IMAGE_GET_TIME_OUT:
            *(uint32_t *)param_ptr = image->BOOT_TIMEOUT;
            break;
        case IMAGE_GET_NAME:
            strcpy((char *)param_ptr,(char *)image->NAME);
            break;
        case IMAGE_GET_BOOT_ADDR:
            *(uint32_t *)param_ptr = image->BOOT_ADDR;
            break;
        case IMAGE_GET_AUTO_BOOT:
            *(uint32_t *)param_ptr = image->AUTO;
            break;
        case IMAGE_SET_AUTO_BOOT:
            image->AUTO = TRUE;
            if(BOOTLOADER_OK != store_table_to_nand_flash(flash_hdl,read_buffer)) {
                result = IO_ERROR;
            }
            result = BOOTLOADER_OK;
            break;
        case IMAGE_SET_BOOT_ADDR:
            image->BOOT_ADDR = *(uint32_t *)param_ptr;
            if(BOOTLOADER_OK != store_table_to_nand_flash(flash_hdl,read_buffer)) {
                result = IO_ERROR;
            }
            result = BOOTLOADER_OK;
            break;
        case IMAGE_SET_NAME:
            _mem_copy((char *)param_ptr, (char *)image->NAME, IMAGE_NAME_LENGTH);
            if(BOOTLOADER_OK != store_table_to_nand_flash(flash_hdl,read_buffer)) {
                result = IO_ERROR;
            }
            result = BOOTLOADER_OK;
            break;
        case IMAGE_SET_TIME_OUT:
            image->BOOT_TIMEOUT= *(uint32_t *)param_ptr;
            if(BOOTLOADER_OK != store_table_to_nand_flash(flash_hdl,read_buffer)) {
                result = IO_ERROR;
            }
            result = BOOTLOADER_OK;
            break;
        default:
            result = IO_ERROR;
    }

    free_buffer(&read_buffer);
    return result;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : store_table_to_nand_flash
* Returned Value   :
* Comments         :
*
*
*END*----------------------------------------------------------------------*/
static int32_t store_table_to_nand_flash
(
    MQX_FILE_PTR flash_hdl,
    unsigned char    *table_ptr
)
{
    uint32_t seek_location;

    if(flash_hdl == NULL) {
        printf("\nFlash device not opened");
        return INVALID_PARAMETER;
    }

    seek_location = IMAGE_TABLE_LOCATION;
    /* Write table to Nand Flash */
    fseek(flash_hdl, seek_location, IO_SEEK_SET);

#if BOOTLOADER_NOR_FLASH_SUPPORT
    _io_ioctl(flash_hdl, FLASH_IOCTL_ENABLE_SECTOR_CACHE, NULL);
    if(BOOTLOADER_OK == ioctl(flash_hdl, FLASH_IOCTL_ERASE_SECTOR, NULL)) {
#elif BOOTLOADER_NAND_FLASH_SUPPORT
    if(MQX_OK == ioctl(flash_hdl, NANDFLASH_IOCTL_ERASE_BLOCK_FORCE, (void*)IMAGE_TABLE_BLOCK_LOCATION)) {
#endif
        if(IMAGE_TABLE_SIZE == write(flash_hdl, table_ptr,IMAGE_TABLE_SIZE)) {
            return BOOTLOADER_OK;
        }
        else{
            printf("\nWrite table to Flash Failed ");
            return WRITE_TABLE_FAILED;
        }
     }
     else {
         printf("\nErase Block:%d Failed ",IMAGE_TABLE_LOCATION);
         return FLASH_ERASE_UNIT_FAILED;
     }
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : check_sum_image
* Returned Value   :
* Comments         : This function to checksum a image
*
*
*END*----------------------------------------------------------------------*/
static int32_t check_sum_image
(
    MQX_FILE_PTR   flash_hdl,
    IMAGE_INFO_PTR image_ptr
)
{
    uint32_t           check_sum = 0;
    uint32_t           size, i;
    uint32_t           result;
    unsigned char         *read_buffer;
    uint32_t           seek_location;

    if(flash_hdl == NULL) {
        printf("\nFlash device not opened");
        return IO_ERROR;
    }
    read_buffer = allocate_buffer(NAND_FLASH_PAGE_SIZE, "read");
    size = image_ptr->SIZE;
#if BOOTLOADER_NOR_FLASH_SUPPORT
    seek_location = _nor_flash_get_seek_location(info_ptr, image_ptr->BLOCK);
#elif BOOTLOADER_NAND_FLASH_SUPPORT
    seek_location = image_ptr->BLOCK*(NAND_FLASH_BLOCK_SIZE/NAND_FLASH_PAGE_SIZE);
#endif

    fseek(flash_hdl, seek_location, IO_SEEK_SET);
    while(size) {
        result = read(flash_hdl, read_buffer,RW_UNIT_SIZE);
#if BOOTLOADER_NAND_FLASH_SUPPORT
        result = result * NAND_FLASH_PAGE_SIZE;
#endif
        for(i = 0; i < result; i++) {
            check_sum +=  read_buffer[i];
            size--;
            if(0 == size)
                break;
        }
    }
    free_buffer(&read_buffer);
    if(check_sum == image_ptr->CHECK_SUM) {
        return BOOTLOADER_OK;
    }
    return CHECK_SUM_IMAGE_FAILED;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bootloader_list_image
* Returned Value   :
* Comments         : This function to list a table of images
*
*
*END*----------------------------------------------------------------------*/
void _bootloader_list_image(MQX_FILE_PTR flash_hdl)
{
    unsigned char            *read_buffer;
    IMAGE_INFO_PTR      image;
    uint32_t             i,image_num = 0;

    if(flash_hdl == NULL) {
        printf("\nFlash device not opened");
        return;
    }
    read_buffer = allocate_buffer(IMAGE_TABLE_SIZE*NAND_FLASH_PAGE_SIZE, "read");
    image = (IMAGE_INFO_PTR)read_buffer;

    fseek(flash_hdl, IMAGE_TABLE_LOCATION, IO_SEEK_SET);
    read(flash_hdl, read_buffer,IMAGE_TABLE_SIZE);
    while(IMAGE_VALID == image->VALID) {
        if(!(IMAGE_DELETED & image->PROPERTY)){
            image_num++;
        }
        image++;
    }
    printf("\nTable of stored images:\n");
    printf("============================================================================\n");
    printf("| Index |         Name        | Start address |    Size    | Auto | Timeout|\n");
    printf("============================================================================\n");

    for(i = 1; i <= image_num; i++) {
        image = (IMAGE_INFO_PTR)read_buffer;
        while(IMAGE_VALID == image->VALID) {
            if((image->INDEX == i)&&(!(image->PROPERTY & IMAGE_DELETED))){
                printf("|  %3d  |%20s |   0x%08x  | 0x%08x |   %1d  | %6d |\n",
                image->INDEX,image->NAME,image->BOOT_ADDR,image->SIZE,
                image->AUTO,image->BOOT_TIMEOUT);
            }
            image++;
        }
    }
    printf("|       |                     |               |            |      |        |\n");
    printf("============================================================================\n");

    free_buffer(&read_buffer);
    return;

}
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bootloader_del_image
* Returned Value   :
* Comments         :
*    Delete a specific image
*
*END*----------------------------------------------------------------------*/
uint32_t _bootloader_del_image(MQX_FILE_PTR flash_hdl,uint32_t index)
{

    bool             shorthelp = FALSE;
    unsigned char           *read_buffer;
    IMAGE_INFO_PTR      image;
    uint32_t             result;
    uint32_t             image_num = 0;
    bool             image_found;

    if (flash_hdl == NULL) {
        printf("\nFlash device not opened");
        return INVALID_PARAMETER;
    }
    read_buffer = allocate_buffer(IMAGE_TABLE_SIZE * NAND_FLASH_PAGE_SIZE, "read");

    image = (IMAGE_INFO_PTR) read_buffer;

    fseek(flash_hdl, IMAGE_TABLE_LOCATION, IO_SEEK_SET);
    read(flash_hdl, read_buffer, IMAGE_TABLE_SIZE);

    image_found = FALSE;

    while (IMAGE_VALID == image->VALID) {
        /* Delete table index and block */
        if ((index == image->INDEX)&&(!(image->PROPERTY & IMAGE_DELETED))) {
            image_found = TRUE;
            image->PROPERTY |= IMAGE_DELETED;
            image->AUTO = FALSE;
            /* Check if deleted image in end of table */
            if(image->PROPERTY & IMAGE_END_TABLE) {
                image->VALID = 0xFFFFFFFF;
                if(image_num) {
                    (image-1)->PROPERTY |= IMAGE_END_TABLE;
                }
            }
        }
        image_num++;
        image++;
    }

    image = (IMAGE_INFO_PTR) read_buffer;
    while (IMAGE_VALID == image->VALID) {
        if((index < image->INDEX)&&(!(image->PROPERTY & IMAGE_DELETED))) {
            image->INDEX --;
        }
        image++;
    }

    if (image_found) {
        if (BOOTLOADER_OK == store_table_to_nand_flash(flash_hdl, read_buffer)) {
            printf("Delete image: %d successful\n", index);
            result = BOOTLOADER_OK;
        }
        else {
            result = IO_ERROR;
        }
    }
    else {
        printf("\n No image found !\n");
    }

    free_buffer(&read_buffer);
    return result;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bootloader_del_table
* Returned Value   : none
* Comments         :
*     delete all images and re initialize table
*
*END*----------------------------------------------------------------------*/
int32_t _bootloader_del_table(MQX_FILE_PTR flash_hdl)
{
    unsigned char            *buffer;
    IMAGE_INFO_PTR       image;
    int32_t               result;

    if (flash_hdl == NULL) {
        printf("\nFlash device not opened");
        return;
    }

    buffer = allocate_buffer(IMAGE_TABLE_SIZE * NAND_FLASH_PAGE_SIZE, "read");
    image = (IMAGE_INFO_PTR) buffer;
    fseek(flash_hdl, IMAGE_TABLE_LOCATION, IO_SEEK_SET);
    read(flash_hdl, buffer, IMAGE_TABLE_SIZE);
    /* clear all valid images */
    while (IMAGE_VALID == image->VALID) {
        image->VALID = 0xFFFFFFFF;
        image++;
    }

    image = (IMAGE_INFO_PTR) buffer;

    /* re-initialize image table */
    image->VALID = IMAGE_VALID;
    image->PROPERTY = IMAGE_DELETED | IMAGE_END_TABLE;
    image->BOOT_ADDR = 0;
    image->SIZE = 0;
#if BOOTLOADER_NAND_FLASH_SUPPORT
    image->BLOCK = IMAGE_START_BLOCK;
#else BOOTLOADER_NOR_FLASH_SUPPORT
    image->BLOCK = IMAGE_START_SECTOR;
#endif
    image->BLOCK_NUM = 0;

    /* Update table to flash memory */
    if (BOOTLOADER_OK == store_table_to_nand_flash(flash_hdl, buffer)) {
        printf("Delete table successful\n");
        result = BOOTLOADER_OK;
    }
    else {
        result = IO_ERROR;
    }
    free_buffer(&buffer);
    return result;
}

/*FUCTION*-----------------------------------------------------------------
*
* Function Name  : _bootloader_update_table
* Returned Value : 0 if successful, -1 if error
* Comments       :
*   Update the table of images
*
*END------------------------------------------------------------------*/

int32_t _bootloader_update_table
    (
        /* [IN] The flash file handler */
        MQX_FILE_PTR flash_hdl,
        /* [IN] The block number of image */
        uint32_t      block,
        /* [IN] The address of image */
        uint32_t      address,
        /* [IN] The size of image */
        uint32_t      size,
        /* [IN] The checksum data of image */
        uint32_t      check_sum,
        /* [IN] The name of image */
        unsigned char    *name
    )
{ /* Body */

    uint32_t             block_num;
    uint32_t             i,j,image_num;
    unsigned char           *img_table;
    unsigned char           *read_write_buff;
    IMAGE_INFO_PTR      image;
    IMAGE_INFO          img_new;
    int32_t              result;
    uint32_t             page_read, page_write;
    int32_t              temp;

    if (flash_hdl == NULL) {
        printf("\nFlash device not opened");
        return IO_ERROR;
    }
    image_num = 0;
    block_num = 0;
    img_new.INDEX = 0;

    /* Read image table */
    img_table = allocate_buffer(IMAGE_TABLE_SIZE * NAND_FLASH_PAGE_SIZE, "read");
    read_write_buff = allocate_buffer(NAND_FLASH_PAGE_SIZE, "read/write");

    fseek(flash_hdl, IMAGE_TABLE_LOCATION, IO_SEEK_SET);
    read(flash_hdl, img_table, IMAGE_TABLE_SIZE);

    image = (IMAGE_INFO_PTR) img_table;

    while (IMAGE_VALID == image->VALID) {
        if(!(image->PROPERTY & IMAGE_DELETED)) {
            img_new.INDEX++;
        }
        image->AUTO = FALSE;
        if(IMAGE_END_TABLE == (image+1)->PROPERTY) {
            image->PROPERTY &= ~IMAGE_END_TABLE;
        }
        block_num += image->BLOCK_NUM;
        image_num++;
        image++;
    }
    img_new.VALID = IMAGE_VALID;
    img_new.BOOT_ADDR = address;
    img_new.INDEX += 1;
    img_new.SIZE = size;
    img_new.BLOCK = block;

#if BOOTLOADER_NAND_FLASH_SUPPORT
    img_new.BLOCK_NUM = (size + NAND_FLASH_BLOCK_SIZE) / NAND_FLASH_BLOCK_SIZE;
#elif BOOTLOADER_NOR_FLASH_SUPPORT
    temp = size;
    img_new.BLOCK_NUM = 0;
    while(temp > 0) {
        temp -= _nor_flash_get_sector_size(info_ptr,(img_new.BLOCK+img_new.BLOCK_NUM));
        img_new.BLOCK_NUM++;
    }
#endif

    img_new.PROPERTY = IMAGE_END_TABLE;
    img_new.AUTO = TRUE;
    img_new.BOOT_TIMEOUT = AUTOBOOT_TIMEOUT;
    img_new.CHECK_SUM = check_sum;

    _mem_copy((char *) name, (char *) img_new.NAME, IMAGE_NAME_LENGTH);
    image = (IMAGE_INFO_PTR) img_table;

    /* Empty table store it in position 0 */
    if ((image_num <= 1) && (image->PROPERTY & IMAGE_DELETED)) {
        *(image) = img_new;
    }
    else {

        /* Finding location where enough memory */
        for (i = 0; i < image_num; i++) {
            if((image->PROPERTY & IMAGE_DELETED) && (img_new.BLOCK_NUM <= image->BLOCK_NUM)) {
                /* found appropriate position copy old postion to new postion */
#if BOOTLOADER_NAND_FLASH_SUPPORT
                page_read = block*(NAND_FLASH_BLOCK_SIZE/NAND_FLASH_PAGE_SIZE);
                page_write = image->BLOCK*(NAND_FLASH_BLOCK_SIZE/NAND_FLASH_PAGE_SIZE);
                temp = size;
                /* Erase block */
                for(j = image->BLOCK; j < (image->BLOCK + image->BLOCK_NUM); j++) {
                    if(MQX_OK != ioctl(flash_hdl, NANDFLASH_IOCTL_ERASE_BLOCK_FORCE, (void*)(j))) {
                        printf("Erasing block %d failed.\n",(j));
                        free_buffer(&img_table);
                        free_buffer(&read_write_buff);
                        return IO_ERROR;
                    }
                }
                while(temp > 0 ) {
                    fseek(flash_hdl, page_read, IO_SEEK_SET);
                    read(flash_hdl, read_write_buff, RW_UNIT_SIZE);
                    fseek(flash_hdl, page_write, IO_SEEK_SET);
                    write(flash_hdl, read_write_buff, RW_UNIT_SIZE);
                    temp -= NAND_FLASH_PAGE_SIZE;
                    page_read++;
                    page_write++;
                }
#elif BOOTLOADER_NOR_FLASH_SUPPORT
                page_read = _nor_flash_get_seek_location(info_ptr,block);
                page_write = _nor_flash_get_seek_location(info_ptr,image->BLOCK);
                temp = size;
                /* Erase sectors */
                for(j = image->BLOCK; j < (image->BLOCK + image->BLOCK_NUM); j++) {
                    fseek(flash_hdl,_nor_flash_get_seek_location(info_ptr, j),IO_SEEK_SET);
                    if(MQX_OK != ioctl(flash_hdl, FLASH_IOCTL_ERASE_SECTOR, NULL)) {
                        printf("Erasing sector %d failed.\n",(j));
                        free_buffer(&img_table);
                        free_buffer(&read_write_buff);
                        return IO_ERROR;
                    }
                }
                while(temp > 0 ) {
                    fseek(flash_hdl, page_read, IO_SEEK_SET);
                    read(flash_hdl, read_write_buff, RW_UNIT_SIZE);
                    fseek(flash_hdl, page_write, IO_SEEK_SET);
                    write(flash_hdl, read_write_buff, RW_UNIT_SIZE);
                    temp -= RW_UNIT_SIZE;
                    page_read += RW_UNIT_SIZE;
                    page_write += RW_UNIT_SIZE;
                }
#endif
                image->INDEX = img_new.INDEX;
                image->SIZE = img_new.SIZE;
                image->BOOT_ADDR = img_new.BOOT_ADDR;
                image->PROPERTY &= ~IMAGE_END_TABLE;
                image->PROPERTY &= ~IMAGE_DELETED;
                _mem_copy((char *)img_new.NAME, (char *)image->NAME, IMAGE_NAME_LENGTH);
                image->AUTO = TRUE;
                img_new = *(image);
                break;
            }
            image++;
        }
        /* store in end of table */
        if (i == image_num) {
            (image - 1)->PROPERTY &= ~IMAGE_END_TABLE;
            img_new.BLOCK = (image - 1)->BLOCK + (image - 1)->BLOCK_NUM;
            *(image) = img_new;
        }
    }

    image = (IMAGE_INFO_PTR) img_table;
    fseek(flash_hdl, IMAGE_TABLE_LOCATION, IO_SEEK_SET);
#if BOOTLOADER_NOR_FLASH_SUPPORT
    _io_ioctl(flash_hdl, FLASH_IOCTL_ENABLE_SECTOR_CACHE, NULL);
    if(MQX_OK == ioctl(flash_hdl, FLASH_IOCTL_ERASE_SECTOR, NULL)) {
#elif BOOTLOADER_NAND_FLASH_SUPPORT
    if (MQX_OK == ioctl(flash_hdl, NANDFLASH_IOCTL_ERASE_BLOCK_FORCE, (void*) IMAGE_TABLE_BLOCK_LOCATION)) {
#endif

        /* Write table to Nand Flash */
        if (IMAGE_TABLE_SIZE == write(flash_hdl, img_table, IMAGE_TABLE_SIZE)) {
            result = BOOTLOADER_OK;
        }
        else {
            result = IO_ERROR;
        }
    }
    else {
        result = IO_ERROR;
    }

    free_buffer(&img_table);
    free_buffer(&read_write_buff);
    return result;

}

/*FUCTION*-----------------------------------------------------------------
*
* Function Name  : _bootloader_check_available
* Returned Value : Return available block position
*                  Return available size
* Comments       :
*   check available space of flash device.
*
*END------------------------------------------------------------------*/

int32_t _bootloader_check_available
    (
        /* [IN] The file handler */
        MQX_FILE_PTR flash_hdl,
        /* [OUT] The valid bock position */
        uint32_t  *erased_unit,
        /* [OUT] The remain space in Byte */
        uint32_t  *remain_size
    )
{
    uint32_t                       block_count;
    uint32_t                       block;
    unsigned char                     *read_buffer;
    IMAGE_INFO_PTR                image;
#if BOOTLOADER_NOR_FLASH_SUPPORT
    uint32_t                       i;
    /*int32_t                        temp;*/
    uint32_t                       num_sector = 0;
    /*uint32_t                       start_sector = 0;*/
#endif

    if (flash_hdl == NULL) {
        printf("\nFlash device not opened");
        return INVALID_PARAMETER;
    }

    block_count = 0;

    /* Read image table */
    read_buffer = allocate_buffer(IMAGE_TABLE_SIZE * NAND_FLASH_PAGE_SIZE, "read");

    fseek(flash_hdl, IMAGE_TABLE_LOCATION, IO_SEEK_SET);
    read(flash_hdl, read_buffer, IMAGE_TABLE_SIZE);

    image = (IMAGE_INFO_PTR) read_buffer;

    while (IMAGE_VALID == image->VALID) {
        block_count += image->BLOCK_NUM;
        block = image->BLOCK;
        image++;
    }
#if BOOTLOADER_NOR_FLASH_SUPPORT
    for(i = 0; i < g_block_num; i++) {
        num_sector += info_ptr[i].NUM_SECTORS;
    }
    for(i = block+1; i < num_sector; i++) {
        *remain_size += _nor_flash_get_sector_size(info_ptr,i);
    }
    /* Calculate start image sector */
    *erased_unit = IMAGE_START_SECTOR + block_count;
#else BOOTLOADER_NAND_FLASH_SUPPORT
    *erased_unit = IMAGE_START_BLOCK + block_count;

    *remain_size = (NAND_FLASH_NUM_BLOCKS - (*erased_unit))*NAND_FLASH_BLOCK_SIZE;
#endif
    free_buffer(&read_buffer);
    return BOOTLOADER_OK;
}

#if BOOTLOADER_NOR_FLASH_SUPPORT
/*FUCTION*-----------------------------------------------------------------
*
* Function Name  : _nor_flash_get_sector_size
* Returned Value :
* Comments       :
*
*
*END------------------------------------------------------------------*/

uint32_t _nor_flash_get_sector_size
    (
        /* [IN] The flash file info pointer */
        FLASHX_BLOCK_INFO_STRUCT_PTR inf_ptr,
        /* [IN] The sector number */
        uint32_t sector
    )
{
    uint32_t tmp=0;
    uint32_t i;

    for (i = 0; i < g_block_num; i++)
    {
        tmp += inf_ptr[i].NUM_SECTORS;
        if(sector < tmp)
        return inf_ptr[i].SECTOR_SIZE;
    }
}

/*FUCTION*-----------------------------------------------------------------
*
* Function Name  : _nor_flash_get_seek_location
* Returned Value :
* Comments       :
*
*
*END------------------------------------------------------------------*/

uint32_t _nor_flash_get_seek_location
    (
        /* [IN] The flash file info pointer */
        FLASHX_BLOCK_INFO_STRUCT_PTR  inf_ptr,
        /* [IN] The sector number */
        uint32_t                       sector
    )
{
    uint32_t tmp=0;
    uint32_t i;

    for (i = 0; i < sector; i++)
    {
        tmp += _nor_flash_get_sector_size(inf_ptr,i);
    }
    return tmp;
}
#endif /* Endif BOOTLOADER_NOR_FLASH_SUPPORT */
/*FUCTION*-----------------------------------------------------------------
*
* Function Name  : _bootloader_store_image_data
* Returned Value :
* Comments       :
*   Store data to flash, update image table in case no error in transaction found
*
*END------------------------------------------------------------------*/

uint32_t  _bootloader_store_image_data
    (
        /* [IN] The flash file handler */
        MQX_FILE_PTR flash_hdl,
        /* [IN] Pointer to buffer data */
        unsigned char *buffer_ptr,
        /* [IN] Size of buffer */
        uint32_t buffer_size,
        /* [IN] End of transaction checking */
        bool next_trans,
        /* [IN] The address of image will be stored */
        uint32_t address,
        /* [IN] The name of image will be stored */
        unsigned char *name,
        /* [IN] Error in transaction checking */
        bool error
    )
{
    static bool    checked = FALSE, erased = FALSE;
    static uint32_t    check_sum = 0;
    static uint32_t    erased_unit, next_erased_unit;
    static uint32_t    remain_size;
    static uint32_t    byte_number = 0;
    static uint32_t    write_buff_index = 0;
    static unsigned char  *write_buff;
    static uint32_t    seek_location;
    uint32_t           result = BOOTLOADER_OK;
    uint32_t           i, tmp;

    if (FALSE == checked) {
        /* Check available space of flash device.*/
        _bootloader_check_available(flash_hdl, &erased_unit, &tmp);
        remain_size = tmp;
#if BOOTLOADER_NOR_FLASH_SUPPORT
        _io_ioctl(flash_hdl, FLASH_IOCTL_ENABLE_SECTOR_CACHE, NULL);
        seek_location = _nor_flash_get_seek_location(info_ptr, erased_unit);
#elif BOOTLOADER_NAND_FLASH_SUPPORT
        seek_location = erased_unit * (NAND_FLASH_BLOCK_SIZE/NAND_FLASH_PAGE_SIZE);
#endif
        fseek(flash_hdl, seek_location, IO_SEEK_SET);
        write_buff = _mem_alloc_zero(NAND_FLASH_PAGE_SIZE);
        if (NULL == write_buff) {
            printf("\nAllocate buffer failed ");
            return IO_ERROR;
        }
        checked = TRUE;
    }
    /* Clear statitic variables when error in transaction */
    if (TRUE == error) {
        checked = FALSE;
        erased = FALSE;
        write_buff_index = 0;
        remain_size = 0;
        check_sum = 0;
        byte_number = 0;
        next_erased_unit = 0;
        seek_location = 0;
        free_buffer(&write_buff);
        return WRITE_DATA_FAILED;
    }
    if (FALSE == erased) {
#if BOOTLOADER_NOR_FLASH_SUPPORT
        printf("\nErasing Sector: %d...",(erased_unit + next_erased_unit));
        if(BOOTLOADER_OK != ioctl(flash_hdl, FLASH_IOCTL_ERASE_SECTOR, NULL)) {
            printf("ERROR\n");
            result = FLASH_ERASE_UNIT_FAILED;
        }
        else {
            printf("OK\n");
            printf("Flashing ");
            erased = TRUE;
        }
    }
#elif BOOTLOADER_NAND_FLASH_SUPPORT
        printf("\nErasing Block: %d...", (erased_unit + next_erased_unit));

        if (MQX_OK != ioctl(flash_hdl, NANDFLASH_IOCTL_ERASE_BLOCK_FORCE, (void*) (erased_unit + next_erased_unit))) {
            printf("ERROR\n");
            result = FLASH_ERASE_UNIT_FAILED;
        }
        else {
            printf("OK\n");
            printf("Flashing ");
            erased = TRUE;
        }
    }
#endif
    /* Calculate check sum and write to flash */
    for (i = 0; i < buffer_size; i++) {
        check_sum += buffer_ptr[i];
        write_buff[write_buff_index] = buffer_ptr[i];
        write_buff_index++;
        if(write_buff_index == NAND_FLASH_PAGE_SIZE) {
            if(RW_UNIT_SIZE != write(flash_hdl, write_buff, RW_UNIT_SIZE)) {
                printf("\nWrite data to flash failed");
                result = WRITE_DATA_FAILED;
            }
            write_buff_index = 0;
            printf("#");
        }
    }
    byte_number += buffer_size;

    /* Calcuate erase unit */
#if BOOTLOADER_NOR_FLASH_SUPPORT
    tmp = 0;
    for(i = 0; i < g_block_num; i++) {
        tmp += info_ptr[i].NUM_SECTORS;
        if((erased_unit + next_erased_unit) < tmp) {
            if(byte_number >= ((next_erased_unit + 1) * info_ptr[i].SECTOR_SIZE)) {
                next_erased_unit++;
                erased = FALSE;
            }
            break;
        }
    }
#elif BOOTLOADER_NAND_FLASH_SUPPORT
    if (byte_number >= ((next_erased_unit + 1) * NAND_FLASH_BLOCK_SIZE)) {
        next_erased_unit++;
        erased = FALSE;
    }
#endif
    if (byte_number > remain_size) {
        printf("\nNot enough flash memory to store image.");
        result = BOOTLOADER_OUT_OF_MEMORY;
    }

    if (FALSE == next_trans) {
        if (byte_number != 0) {
            if (write_buff_index) {
                /* Write remain data in buffer to flash */
                if (RW_UNIT_SIZE != write(flash_hdl, write_buff, RW_UNIT_SIZE)) {
                    printf("\n Write data to flash failed");
                    result = WRITE_DATA_FAILED;
                }
            }
        }
        if (BOOTLOADER_OK == result) {
            printf("\nTransfer 0x%x byte successful", byte_number);
            free_buffer(&write_buff);
            if (BOOTLOADER_OK == _bootloader_update_table(flash_hdl, erased_unit, address, byte_number, check_sum, name)) {
                printf("\nListing images...");
                _bootloader_list_image(flash_hdl);
            }
            else {
                printf("\nWrite image to table fail\n");
                result = WRITE_TABLE_FAILED;
            }
        }
    }
    /* Transaction done, clear static data */
    if ((BOOTLOADER_OK != result) || (FALSE == next_trans)) {
        checked = FALSE;
        erased = FALSE;
        write_buff_index = 0;
        remain_size = 0;
        check_sum = 0;
        byte_number = 0;
        next_erased_unit = 0;
        seek_location = 0;
        free_buffer(&write_buff);
    }
    return result;
}
/* EOF */
