/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
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
*   This file contains the shell command for bootloader application
*
*
*END************************************************************************/

#include <mqx.h>
#include <fio.h>
#include <rtcs.h>
#include <ftpc.h>
#include <shell.h>
#include <sh_rtcs.h>
#include "config.h"
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "bootloader.h"
#include "tftp.h"

extern uint32_t _boot_addr;

#if BOOTLOADER_NOR_FLASH_SUPPORT
    extern FLASHX_BLOCK_INFO_STRUCT_PTR  info_ptr;
#endif

extern uint32_t    g_block_num;
static unsigned char ChartoHex(unsigned char c);
static uint32_t str_to_hex(char *hex);
extern void bootloader_boot(void);
int32_t Shell_bootloader_del_image(int32_t argc, char *argv[] );
int32_t Shell_bootloader_list_image(int32_t argc, char *argv[] );
int32_t Shell_bootloader_del_table(int32_t argc, char *argv[] );
int32_t Shell_bootloader_set_default(int32_t argc, char *argv[] );
int32_t Shell_bootloader_tftp_client(int32_t argc, char *argv[] );
int32_t Shell_bootloader_go_image(int32_t argc, char *argv[] );
int32_t Shell_bootloader_reset(int32_t argc, char *argv[] );
int32_t Shell_bootloader_set_timeout(int32_t argc, char *argv[] );
int32_t Shell_bootloader_img_rename(int32_t argc, char *argv[] );
int32_t Shell_bootloader_img_set_boot_addr(int32_t argc, char *argv[] );
int32_t Shell_nand_check_bad_block (int32_t argc, char *argv[] );
int32_t Shell_nand_erase_force (int32_t argc, char *argv[] );
int32_t Shell_nand_erase (int32_t argc, char *argv[] );

const SHELL_COMMAND_STRUCT Shell_commands[] = {
     { "arpadd",    Shell_arpadd },
     { "arpdel",    Shell_arpdel },
     { "arpdisp",   Shell_arpdisp },
#if RTCSCFG_ENABLE_NAT
     { "dnat",      Shell_dnat },
#endif
#if RTCSCFG_ENABLE_UDP
     { "exec",      Shell_exec },
#endif
     { "exit",      Shell_exit },
     { "gate",      Shell_gate },
     { "gethbn",    Shell_get_host_by_name },
     { "getrt",     Shell_getroute },
     { "help",      Shell_help },
     { "ipconfig",  Shell_ipconfig },
#if RTCSCFG_ENABLE_UDP
     { "load",      Shell_load },
#endif
#if RTCSCFG_ENABLE_NAT
     { "natinit",   Shell_natinit },
     { "natinfo",   Shell_natinfo },
#endif
     { "netstat",   Shell_netstat },
#if RTCSCFG_ENABLE_ICMP
     { "ping",      Shell_ping },
#endif
#if RTCSCFG_ENABLE_UDP
     { "sendto",    Shell_sendto },
#endif
#if RTCSCFG_ENABLE_SNMP
     { "snmpd",     Shell_SNMPd },
#endif
#if RTCSCFG_ENABLE_UDP
     { "tftp",      Shell_TFTP_client },
#endif

     { "walkrt",    Shell_walkroute },
/* Bootloader commands */

#if BOOTLOADER_NAND_FLASH_SUPPORT
    { "nande",     Shell_nand_erase },
    { "nandef",    Shell_nand_erase_force },
    { "nandcheck", Shell_nand_check_bad_block },
#endif
    { "imglst",    Shell_bootloader_list_image },
    { "imgdel",    Shell_bootloader_del_image },
    { "tbldel",    Shell_bootloader_del_table },
    { "autoboot",  Shell_bootloader_set_default },
    { "imggo",     Shell_bootloader_go_image },
    { "imgload",   Shell_bootloader_tftp_client},
    { "imgren",    Shell_bootloader_img_rename},
    { "imgaddr",   Shell_bootloader_img_set_boot_addr},
    { "imgtimeout",Shell_bootloader_set_timeout},
    { "reset",     Shell_bootloader_reset},

     { "?",         Shell_command_list },
     { NULL,        NULL }
};

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_list_image
* Returned Value : none
* Comments       : SHELL utility to list images in table
*
*END------------------------------------------------------------------*/

int32_t  Shell_bootloader_list_image(int32_t argc, char *argv[] )
{ /* Body */
    bool             print_usage, shorthelp = FALSE;
    int32_t              return_code = SHELL_EXIT_SUCCESS;
    MQX_FILE_PTR        flash_hdl;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {

        if (argc != 1) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            else {
                _bootloader_list_image(flash_hdl);
            }

            fclose(flash_hdl);
        }
    }

    if (print_usage) {
        if (shorthelp) {
            printf("%s\n", argv[0]);
        }
        else {
            printf("Usage: %s ", argv[0]);
            printf("   <%s> to list image table\n", argv[0]);
        }
    }
    return return_code;
} /* Endbody */

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_del_image
* Returned Value : none
* Comments       : SHELL utility to delete a specific image
*
*END------------------------------------------------------------------*/

int32_t  Shell_bootloader_del_image(int32_t argc, char *argv[] )
{ /* Body */
    bool print_usage, shorthelp = FALSE;
    int32_t return_code = SHELL_EXIT_SUCCESS;
    uint32_t index;
    MQX_FILE_PTR flash_hdl;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {

        if ((argc < 2) || (argc >= 3)) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            index = _io_atoi(argv[1]);
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            else {
                _bootloader_del_image(flash_hdl, index);
            }
            fclose(flash_hdl);
        }

    }
    if (print_usage) {
        if (shorthelp) {
            printf("%s <index> \n", argv[0]);
        }
        else {
            printf("Usage: %s <index> \n", argv[0]);
            printf("  <index> = index of image to delete\n");
        }
    }
    return return_code;
}

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_go_image
* Returned Value : void
* Comments       : SHELL utility to jump and boot a specific image
*
*END------------------------------------------------------------------*/

int32_t  Shell_bootloader_go_image(int32_t argc, char *argv[] )
{ /* Body */
    bool             print_usage, shorthelp = FALSE;
    int32_t              return_code = SHELL_EXIT_SUCCESS;
    uint32_t             image_index;
    MQX_FILE_PTR        flash_hdl;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {
        if (argc != 2) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            image_index = _io_atoi(argv[1]);
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            else {
                printf("\nStarting application at image %d ...", image_index);
                printf("\n");
                if (MQX_OK == _bootloader_load(flash_hdl, image_index)) {
                    /* Exit bootloader app and execute the new image */
                    _mqx_exit(0);
                }
                else {
                    printf("\nError!\n");
                }
            }
            fclose(flash_hdl);
        }
    }

    if (print_usage) {
        if (shorthelp) {
            printf("%s <index>\n", argv[0]);
        }
        else {
            printf("Usage: %s <index>\n", argv[0]);
            printf("   <index> = image index in memory table \n");
        }
    }
    return return_code;
}

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_del_table
* Returned Value :
* Comments       : SHELL utility to delete table images
*
*END------------------------------------------------------------------*/

int32_t  Shell_bootloader_del_table(int32_t argc, char *argv[] )
{ /* Body */
    bool             print_usage, shorthelp = FALSE;
    int32_t              return_code = SHELL_EXIT_SUCCESS;
    MQX_FILE_PTR        flash_hdl;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {

        if (argc != 1) {

            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            if (IO_ERROR == _bootloader_del_table(flash_hdl)) {
                printf("Delete table error\n ");
                return_code = SHELL_EXIT_ERROR;
            }
            else {
                return_code = SHELL_EXIT_SUCCESS;
            }
            fclose(flash_hdl);
        }
    }

    if (print_usage) {
        if (shorthelp) {
            printf("%s \n", argv[0]);
        }
        else {
            printf("Usage: %s ", argv[0]);
            printf("   <%s> Delete table in memory \n", argv[0]);
        }
    }
    return return_code;
}

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_reset
* Returned Value : none
* Comments       : SHELL utility to run soft reset
*
*END------------------------------------------------------------------*/

int32_t  Shell_bootloader_reset(int32_t argc, char *argv[] )
{ /* Body */
    bool             print_usage, shorthelp = FALSE;
    int32_t              return_code = SHELL_EXIT_SUCCESS;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {

        if (argc != 1) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            _boot_addr = RESET_MAGIC_NUMBER;
            /* Exit bootloader app and execute soft reset */
            _mqx_exit(0);
            /* Never reach here */
            return_code = SHELL_EXIT_SUCCESS;
        }
    }

    if (print_usage) {
        if (shorthelp) {
            printf("%s \n", argv[0]);
        }
        else {
            printf("Usage: %s ", argv[0]);
            printf("   <%s> booloader reset  \n", argv[0]);
        }
    }
    return return_code;
}

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_set_timeout
* Returned Value : none
* Comments       : SHELL utility to set auto boot timeout value
*
*END------------------------------------------------------------------*/

int32_t  Shell_bootloader_set_timeout(int32_t argc, char *argv[] )
{ /* Body */
    bool           print_usage, shorthelp = FALSE;
    int32_t            return_code = SHELL_EXIT_SUCCESS;
    MQX_FILE_PTR      flash_hdl;
    int32_t            index, timeout;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {
        if (argc != 3) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            index = _io_atoi(argv[1]);
            timeout = _io_atoi(argv[2]);
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            if (MQX_OK == _bootloader_imgctl(flash_hdl, IMAGE_SET_TIME_OUT, index, &timeout)) {
                printf("Set image %d autoboot timeout in %d seconds success\n", index, timeout);
                return_code = SHELL_EXIT_SUCCESS;
            }
            else {
                printf("Set time out error\n ");
                return_code = SHELL_EXIT_ERROR;
            }
            fclose(flash_hdl);
        }
    }
    if (print_usage) {
        if (shorthelp) {
            printf("%s <index> <timeout>\n", argv[0]);
        }
        else {
            printf("Usage: %s <index> <timeout> \n", argv[0]);
            printf("   <index>    = Image index to set auto boot timeout\n");
            printf("   <timeout>  = Timeout value in second\n");
        }
    }
    return return_code;
} /* Endbody */

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_set_default
* Returned Value : none
* Comments       : SHELL utility to set auto boot image
*
*END------------------------------------------------------------------*/

int32_t  Shell_bootloader_set_default(int32_t argc, char *argv[] )
{ /* Body */
    bool             print_usage, shorthelp = FALSE;
    int32_t              return_code = SHELL_EXIT_SUCCESS;
    MQX_FILE_PTR        flash_hdl;
    int8_t               index;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {

        if (argc != 2) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            index = _io_atoi(argv[1]);
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            if (MQX_OK == _bootloader_imgctl(flash_hdl, IMAGE_SET_AUTO_BOOT,
                    index, &index)) {
                printf("Set image %d as default boot image success\n", index);
            }
            else {
                printf("Set default error\n ");
                return_code = SHELL_EXIT_ERROR;
            }
            fclose(flash_hdl);
        }
    }
    if (print_usage) {
        if (shorthelp) {
            printf("%s [<index>] \n", argv[0]);
        }
        else {
            printf("Usage: %s ", argv[0]);
            printf("   <%s> Image index to boot\n", argv[0]);
        }
    }
    return return_code;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  ChartoHex
*  Returned Value:
*  Comments       :  convert a character to hex
*
*
*END*-----------------------------------------------------------------*/
static unsigned char ChartoHex(unsigned char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return c = 0; /* not Hex digit  */
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  str_to_hex
*  Returned Value:
*  Comments      :  convert a String to Hexa
*
*
*END*-----------------------------------------------------------------*/
static uint32_t str_to_hex(char *hex) {
    uint32_t hexa_value = 0;
    uint8_t n = 0, i;

    while (hex[n] != 0) {
        n++;
    }
    for (i = 0; i < n; i++) {
        hexa_value += ChartoHex(hex[i])*pow(16,n-1-i);
    }
    return hexa_value;
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  Shell_TFTP_client
*  Returned Value:  none
*  Comments  :  SHELL utility to get image from TFTP host
*
*
*END*-----------------------------------------------------------------*/

int32_t  Shell_bootloader_tftp_client(int32_t argc, char *argv[] )
{
    _ip_address          hostaddr;
    char                 hostname[MAX_HOSTNAMESIZE];
    uint32_t              tftp_handle, buffer_size;
    unsigned char            *buffer_ptr;
    TFTP_DATA_STRUCT     tftp_data;
    MQX_FILE_PTR         flash_hdl;
    bool              error = FALSE;
    bool              print_usage, shorthelp = FALSE;
    int32_t               return_code = SHELL_EXIT_SUCCESS;
    int32_t               i=0,address;
    bool              next_trans = TRUE;
    char             *image_name;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {
        if ((argc >= 4) && (argc <= 5)) {
            RTCS_resolve_ip_address(argv[1], &hostaddr, hostname, MAX_HOSTNAMESIZE);
            if (!hostaddr) {
                printf("Unable to resolve host.\n");
                return_code = SHELL_EXIT_ERROR;
            }
            else {
                tftp_data.SERVER = hostaddr;
                tftp_data.FILENAME = argv[2];
                tftp_data.FILEMODE = "octet";
                image_name = argv[2];
                if (argc > 4) {
                    image_name  = argv[4];
                }
                address = str_to_hex(argv[3]);
                flash_hdl = fopen(FLASH_NAME_INIT, NULL);
                if (flash_hdl == NULL) {
                    printf("\nUnable to open flash device");
                    return SHELL_EXIT_ERROR;
                }
#if BOOTLOADER_NOR_FLASH_SUPPORT
                _io_ioctl(flash_hdl, FLASH_IOCTL_ENABLE_SECTOR_CACHE, NULL);
#endif
                printf("\nDownloading file %s from TFTP server: %s [%ld.%ld.%ld.%ld]\n",
                        tftp_data.FILENAME, hostname, IPBYTES(hostaddr));
                tftp_handle = (*FT_TFTP->OPEN)((void *) &tftp_data);
                if (tftp_handle != RTCS_OK) {
                    printf("\nError opening file %s\n", tftp_data.FILENAME);
                    return_code = SHELL_EXIT_ERROR;
                }
                else {
                    while (!(*FT_TFTP->EOFT)()) {
                        next_trans = TRUE;
                        while(1) {
                            /* Read TFTP */
                            buffer_ptr = (*FT_TFTP->READ)( &buffer_size );
                            if((NULL == buffer_ptr)||(TFTP_DATA_SIZE != buffer_size)) {
                                next_trans = FALSE;
                            }
                            switch (buffer_size) {
                                case (RTCSERR_TFTP_TIMEOUT):
                                    printf("\nTFTP timeout\n");
                                    error = TRUE;
                                    break;
                                case (RTCSERR_TFTP_ERROR + 1):
                                    printf("\nFile %s not found\n", tftp_data.FILENAME);
                                    error = TRUE;
                                    break;
                                case (RTCSERR_TFTP_ERROR + 2):
                                    printf("\nAccess violation\n");
                                    error = TRUE;
                                    break;
                                case (RTCSERR_TFTP_ERROR + 3):
                                    printf("\nDisk full or allocation exceeded\n");
                                    error = TRUE;
                                    break;
                                case (RTCSERR_TFTP_ERROR + 4):
                                    printf("\nIllegal TFTP operation\n");
                                    error = TRUE;
                                    break;
                                case (RTCSERR_TFTP_ERROR + 5):
                                    printf("\nUnknown transfer ID\n");
                                    error = TRUE;
                                    break;
                                case (RTCSERR_TFTP_ERROR + 6):
                                    printf("\nFile already exists\n");
                                    error = TRUE;
                                    break;
                                case (RTCSERR_TFTP_ERROR + 7):
                                    printf("\nNo such user\n");
                                    error = TRUE;
                                    break;
                                default:
                                    break;
                            } /* Endswitch */
                            /* Anyway, store data to flash */
                            if(BOOTLOADER_OK != _bootloader_store_image_data(flash_hdl, buffer_ptr, buffer_size, next_trans, address, (unsigned char *)image_name, error)) {
                                return_code = (*FT_TFTP->CLOSE)();
                                fclose(flash_hdl);
                                return return_code;
                            }
                            /* Transaction finished */
                            if((NULL == buffer_ptr)||(TFTP_DATA_SIZE != buffer_size)) {
                                return_code = (*FT_TFTP->CLOSE)();
                                fclose(flash_hdl);
                                return return_code;
                            }
                        } /* End Read TFTP */
                        break;
                    }
                    return_code = (*FT_TFTP->CLOSE)();
                }
                fclose(flash_hdl);
            }
        }
        else {
            printf("Error, %s invoked with incorrect number of arguments\n", argv[0]);
            print_usage = TRUE;
        }
    }

    if (print_usage) {
        if (shorthelp) {
            printf("%s <host> <source> <add> [<name>]\n", argv[0]);
        }
        else {
            printf("Usage: %s <host> <source> <add> [<name>]\n", argv[0]);
            printf("   <host>    = TFTP server address or name\n");
            printf("   <source>  = Source file name\n");
            printf("   <add>     = RAM start address\n");
            printf("   [<name>]  = Name of image will be stored in table, maximum %d characters in length\n", IMAGE_NAME_LENGTH);
        }
    }
    return return_code;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name : Shell_img_rename
*  Returned Value: none
*  Comments      : SHELL utility to modify the name of specific image
*
*END*-----------------------------------------------------------------*/

int32_t  Shell_bootloader_img_rename(int32_t argc, char *argv[] )
{ /* Body */
    bool             print_usage, shorthelp = FALSE;
    int32_t              return_code = SHELL_EXIT_SUCCESS;
    MQX_FILE_PTR        flash_hdl;
    int8_t               index;
    char            *name_ptr;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {

        if (argc != 3) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            index = _io_atoi(argv[1]);
            name_ptr = argv[2];
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            if (MQX_OK == _bootloader_imgctl(flash_hdl, IMAGE_SET_NAME, index,
                    name_ptr)) {
                printf("Image renaming success\n", index);
            }
            else {
                printf("Image renaming error\n ");
                return_code = SHELL_EXIT_ERROR;
            }
            fclose(flash_hdl);
        }
    }
    if (print_usage) {
        if (shorthelp) {
            printf("%s <index> <name>\n", argv[0]);
        }
        else {
            printf("Usage: %s <index> <name>\n", argv[0]);
            printf("   <index> = image index to rename\n");
            printf("   <name>  = new image name\n");
        }
    }
    return return_code;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name : Shell_img_set_boot_addr
*  Returned Value: none
*  Comments      : SHELL utility to set the boot address
*
*END*-----------------------------------------------------------------*/

int32_t  Shell_bootloader_img_set_boot_addr(int32_t argc, char *argv[] )
{ /* Body */
    bool             print_usage, shorthelp = FALSE;
    int32_t              return_code = SHELL_EXIT_SUCCESS;
    MQX_FILE_PTR        flash_hdl;
    int8_t               index;
    uint32_t             boot_addr;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {
        if (argc != 3) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            index = _io_atoi(argv[1]);
            boot_addr = str_to_hex(argv[2]);
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            if (MQX_OK == _bootloader_imgctl(flash_hdl, IMAGE_SET_BOOT_ADDR,
                    index, &boot_addr)) {
                printf("Image set boot address success\n", index);
            }
            else {
                printf("Image set boot address error\n ");
                return_code = SHELL_EXIT_ERROR;
            }
            fclose(flash_hdl);
        }
    }
    if (print_usage) {
        if (shorthelp) {
            printf("%s <index> <addr>\n", argv[0]);
        }
        else {
            printf("Usage: %s <index> <addr>\n", argv[0]);
            printf("   <index> = image index to set boot address\n");
            printf("   <addr>  = new image boot address\n");
        }
    }
    return return_code;
} /* Endbody */

#if BOOTLOADER_NAND_FLASH_SUPPORT
/*FUNCTION*-----------------------------------------------------------
*
* Function Name  : Shell_nand_check_bad_block
* Returned Value :
* Comments       : SHELL utility to check the NAND flash bad block
*
*END------------------------------------------------------------------*/

int32_t  Shell_nand_check_bad_block(int32_t argc, char *argv[] )
{ /* Body */
    bool             print_usage, shorthelp = FALSE;
    int32_t              return_code = SHELL_EXIT_SUCCESS;
    MQX_FILE_PTR        flash_hdl;
    uint32_t             i;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {

        if (argc != 1) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            for (i = 0; i < BSP_NAND_NUM_BLOCKS; i++) {
                if(NANDFLASHERR_BLOCK_BAD == ioctl(flash_hdl, NANDFLASH_IOCTL_CHECK_BLOCK, (void*)i)) {
                    printf("\nBlock #%d is bad", i);
                }
            }
            printf("\n");
            fclose(flash_hdl);
        }
    }

    if (print_usage) {
        if (shorthelp) {
            printf("%s \n", argv[0]);
        }
        else {
            printf("Usage: %s ", argv[0]);
            printf("   <%s> Nand Flash check bad block  \n", argv[0]);
        }
    }
    return return_code;
} /* Endbody */

/*FUNCTION*-----------------------------------------------------------
*
* Function Name  : Shell_nand_check_bad_block
* Returned Value :
* Comments       : SHELL utility to force erase a block number
*
*END------------------------------------------------------------------*/

int32_t  Shell_nand_erase_force(int32_t argc, char *argv[] )
{ /* Body */
    bool             print_usage, shorthelp = FALSE;
    int32_t              return_code = SHELL_EXIT_SUCCESS;
    MQX_FILE_PTR        flash_hdl;
    uint32_t             block;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {

        if (argc != 2) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            block = _io_atoi(argv[1]);
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            printf("\nForce Erase block %d...", block);
            if (MQX_OK == ioctl(flash_hdl, NANDFLASH_IOCTL_ERASE_BLOCK_FORCE,
                    (void*) block)) {
                printf("OK");
            }
            else {
                printf("ERROR");
                return_code = SHELL_EXIT_ERROR;
            }
            printf("\n");
            fclose(flash_hdl);
        }
    }

    if (print_usage) {
        if (shorthelp) {
            printf("%s <block> \n", argv[0]);
        }
        else {
            printf("Usage: %s <block>", argv[0]);
            printf("   <block> = force erase block number \n");
        }
    }
    return return_code;
} /* Endbody */

/*FUNCTION*------------------------------------------------------------
*
* Function Name  : Shell_nand_check_bad_block
* Returned Value :
* Comments       : SHELL utility to erase a block number
*
*END------------------------------------------------------------------*/

int32_t  Shell_nand_erase(int32_t argc, char *argv[] )
{ /* Body */
    bool             print_usage, shorthelp = FALSE;
    int32_t              return_code = SHELL_EXIT_SUCCESS;
    MQX_FILE_PTR        flash_hdl;
    uint32_t             block;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage) {

        if (argc != 2) {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else {
            block = _io_atoi(argv[1]);
            flash_hdl = fopen(FLASH_NAME_INIT, NULL);
            if (flash_hdl == NULL) {
                printf("\nUnable to open flash device");
                return_code = SHELL_EXIT_ERROR;
            }
            printf("\nErase block %d...", block);
            if (MQX_OK == ioctl(flash_hdl, NANDFLASH_IOCTL_ERASE_BLOCK,
                    (void*) block)) {
                printf("OK");
            }
            else {
                printf("ERROR");
                return_code = SHELL_EXIT_ERROR;
            }
            printf("\n");
            fclose(flash_hdl);
        }
    }

    if (print_usage) {
        if (shorthelp) {
            printf("%s <block> \n", argv[0]);
        }
        else {
            printf("Usage: %s <block>", argv[0]);
            printf("   <block> = erase block number \n");
        }
    }
    return return_code;
} /* Endbody */
#endif

/* EOF */
