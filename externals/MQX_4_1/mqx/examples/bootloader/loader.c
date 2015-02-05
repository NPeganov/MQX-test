/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 2004-2008 Embedded Access Inc.
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
*   This file is the parser image for boot loader. 
*
*
*END************************************************************************/
#include "mqx.h"
#include "bsp.h"
#include "fio.h"
#include "io.h"
#include "psptypes.h"
#include "bootloader.h"
#include <string.h>

/*****************************************************************************
 * Function predefinitions.
 *****************************************************************************/
 uint32_t          parse_image_to_ram(uint8_t* arr,uint32_t length);
 static uint8_t    parse_array_s19   (uint8_t* Array,uint32_t size_of_array,uint8_t *Line);
 static uint8_t    parse_line_s19    (uint8_t *);
 static uint8_t    parse_array_cw    (uint8_t *, uint32_t, uint8_t *);
 static uint8_t    parse_line_cw     (uint8_t *);
 static uint8_t    get_hex_value     (uint8_t text);
 static uint8_t    get_spair        (uint8_t *arr,uint8_t point);
 static uint32_t   get_uint32      (uint8_t* arr, uint32_t index);
 /*****************************************************************************
 * Global variables.
 *****************************************************************************/
 
uint8_t _s19_file_done;       /* parsing S19 flag */
uint32_t _s19_address;        /* address to flash */
uint8_t _bootloader_status;   /* status of loading process */
uint8_t _file_type;           /* type of image file */
uint8_t _line[260];           /* line buffer */
extern uint32_t          _boot_addr;

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : parse_image_to_ram
* Returned Value : 0 if successful, other value if error
* Comments       : parse and flash an array to flash memory
*     
*END*--------------------------------------------------------------------*/
uint32_t parse_image_to_ram
    (
        /* [IN] the array to parse */
        uint8_t*     arr,
        /* [IN] data length of the array */
        uint32_t     length
    )
{
    /* Body */
    uint32_t result;
    static  uint32_t bytes_written;  /* number of bytes was written to flash */
    uint32_t header;
    unsigned char *dst, *src;
    
    header = get_uint32(arr,0);

    /* Get image type */
    if(_file_type == UNKNOWN)
    {
        bytes_written = 0;
        /*  first four bytes is SP */
        
#if BSP_TWRMPC5125
       /* CodeWarrior 9.2 Raw binary file */
       _file_type = RAW_BINARY;
#else
       if (header ==(uint32_t)(S19_RECORD_HEADER)) {
         /* S-Record file found */
         _file_type = S19_RECORD;
       } else {
          /* CodeWarrior binary file found */
          _file_type = CODE_WARRIOR_BINARY;
       }
#endif       
    } /* EndIf */

    /* Flash image */
    switch (_file_type)
    {
        case RAW_BINARY: 
            /* Raw binary file found*/
            /* the address to write to flash */
            dst =(unsigned char *) (_boot_addr + bytes_written);
            src =(unsigned char *) arr;
            /* if flash error , break the loop */
            bytes_written += length;
            /* DisableInterrupts */
            _int_disable();
            while(length) {
               *dst++ = *src++;
               length--;
            }        
            /* EnableInterrupts */
            _int_enable();
            break;
        case CODE_WARRIOR_BINARY:
            /* CodeWarrior binary file found */
            result = parse_array_cw(arr,length,_line);
            break;
        case S19_RECORD:
            /* S19 file found */
            result = parse_array_s19(arr,length,_line);
            break;
        default:
           result = PARSE_IMAGE_ERROR;
           break;
    } /* EndSwitch */
    return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : parse_array_s19
* Returned Value : 0 if successful, other value if error
* Comments       : Get full lines from an array and flash them
*     
*END*--------------------------------------------------------------------*/
static uint8_t parse_array_s19
    (   /* [IN] the array to parse */
        uint8_t *array,
        /* [IN] data length of the array */
        uint32_t size_of_array,
        /* [IN] a allocated buffer */
        uint8_t *line
    )
{
    /* Body */
    uint8_t result;
    uint16_t i,j;
    uint8_t c;
    static uint8_t t=0;
    static uint8_t curL, totalL; /* current Length, total Length */
    static uint8_t newline = 0;

    for (i=0 ; i<size_of_array;i++)
    {
        c = array[i];
        if(curL == 4) 
        {
            /* Get total length of current line */
            t = get_spair(line,2);
            totalL = (uint8_t)((t+2)*2);
        } /* EndIf */
        if (curL>=totalL)                   /* Get full line */
        {
            result = parse_line_s19(line);   /* Parse and flash current line */
            for (j=0 ; j<totalL;j++)        /* Reset line and other variables */
            {
                line[j]=0xFF;
            } /* EndFor */
            curL=0;
            totalL=6;
            newline=0;
        } /* EndIf */
        if (newline==0)                     /* Check for newline */
        {
            if (c=='S') 
            {
                newline=1;
                line[curL]=c;
                curL++;
            } /* EndIf */
        }
        else
        {
            line[curL] = c;
            curL++;
        } /* EndIf */
    } /* EndFor */
    return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : parse_line_s19
* Returned Value : 0 if successful, other value if error
* Comments       : Parse and flash a full line
*     
*END*--------------------------------------------------------------------*/
static uint8_t parse_line_s19
    (
        /* [IN] the Line to parse */
        uint8_t *Line
    )
{
    /* Body */
    static uint8_t length;
    static uint8_t checksum;
    static uint8_t i, offset,temp,c_temp,j;
    static uint8_t type;
    static uint8_t data;
    static uint8_t cur_point; /* current pointer */
    uint8_t buffer_to_flash[256];

    c_temp=Line[0];
    if (c_temp!='S')
    {
        _bootloader_status = BOOTLOADER_S19_ERROR;
        return FLASH_IMAGE_ERROR;
    } /* EndIf */
    /* Get record length */
    cur_point = 2;
    length = get_spair(Line,cur_point);
    if(_s19_file_done) 
    {
        /* not a valid S19 file */
        _bootloader_status = BOOTLOADER_S19_ERROR;
        return FLASH_IMAGE_ERROR;
    } /* EndIf */
    cur_point--;
    checksum = length;
    type=Line[1];

     /* Take appropriate action */
     switch (type)
        {
            case '1':
            case '2':
            case '3':
                _s19_address = (uint32_t) NULL;
                type -= '0';
                cur_point=4;
                for (i = 0; i <= type ; i++)
                {
                    /* Formulate address */
                    /* Address needs to be word aligned for successful flash program */
                    data = get_spair(Line,cur_point);
                    if(_s19_file_done) 
                    {       
                        /* not a valid S19 file */
                        _bootloader_status = BOOTLOADER_S19_ERROR;
                        return FLASH_IMAGE_ERROR;
                    } /* EndIf */
                    _s19_address = (_s19_address << 8) | data;
                    /* Maintain 8-bit checksum */
                    checksum = (unsigned char)((data + checksum) & 0x00FF);
                    cur_point+=2;
                } /* EndFor */

                 /* 32-bit cores program flash in 32-bit words */
                 /* Therefore S19 address needs to be adjusted to be word aligned */
                 /* Pad beginning of buffer if address not word aligned */
                 offset = (uint8_t) (_s19_address & 0x0003);
                 _s19_address = (uint32_t) (_s19_address & 0xFFFFFFFC);
                 length += offset;
                 for (i = 0; i < offset; i++) 
                 {
                     buffer_to_flash[i] = 0xFF; 
                 } /* EndFor */
                 /* Get data and put into buffer */
                 for (i = offset; i < (length - 5); i++)
                 {
                     data=get_spair(Line,cur_point);
                     buffer_to_flash[i] =data;
                     cur_point+=2;
                     if(_s19_file_done) 
                     {
                         /* not a valid S19 file */
                         _bootloader_status = BOOTLOADER_S19_ERROR;
                         return FLASH_IMAGE_ERROR;
                     }
                 } /* EndFor */

                 /* Calculate checksum */
                 for (i = offset; i < (length - 5); i++)
                 {
                     checksum = (unsigned char)((buffer_to_flash[i] + checksum) & 0x00FF);
                 } /* EndFor */
                 /* Get checksum byte */
                 data = get_spair(Line,cur_point);
                 cur_point+=2;
                 if(_s19_file_done) 
                 {
                     /* not a valid S19 file */
                     _bootloader_status = BOOTLOADER_S19_ERROR;
                     return FLASH_IMAGE_ERROR;
                 } /* EndIf */

                 if (((data - ~checksum) & 0x00FF) != 0)
                 {
                     _bootloader_status = BOOTLOADER_S19_ERROR;
                     _s19_file_done = TRUE;
                     return FLASH_IMAGE_ERROR;
                 } /* EndIf */
                 /* For 32-bit cores Flash_Prog writes 32-bit words, not bytes */
                 /* if last 32-bit word in s-record is not complete, finish word */
                 if((i & 0x0003) != 0x0000) 
                 {
                     /* 32-bit word not complete */
                     buffer_to_flash[i++] = 0xFF;         /* pad end of word */
                     buffer_to_flash[i++] = 0xFF;         /* pad end of word */
                     buffer_to_flash[i++] = 0xFF;         /* pad end of word */
                 } /* EndIf */

                 /* NOTE: 8-bit core does not need to pad the end */
                 /* Write buffered data to Flash */
                  /* call flash program */
                  _int_disable();
                  memcpy((void *)_s19_address,(void *)buffer_to_flash,i);
                  _int_enable();
                break;
            case '7':
            case '8':
            case '9':
                _s19_address = (uint32_t) NULL; 
                type = (unsigned char)(type - '0');
                type = (unsigned char)(10 - type);
                cur_point=4;
                checksum = length;
                /* Get Address */
                for (i = 0; i <= type ; i++)
                {
                	for(j=0;j<length-1;j++)
                	{	
	                    data = get_spair(Line,cur_point);
	                    if(_s19_file_done) 
	                    {
	                        /* not a valid S19 file */
	                        _bootloader_status = BOOTLOADER_S19_ERROR;
	                        return FLASH_IMAGE_ERROR;
	                    } /* EndIf */
						checksum = (unsigned char)((data + checksum) & 0x00FF);
						cur_point+=2;
                	} /* EndFor */

                    
                    /* Read checksum value */
                    data=get_spair(Line,cur_point);
                    if(_s19_file_done)
                    {
                        /* not a valid S19 file */
                        _bootloader_status = BOOTLOADER_S19_ERROR;
                        return FLASH_IMAGE_ERROR;
                    } /* EndIf */
                    
                    /* Check checksum */
                    if (((data - ~checksum) & 0x00FF) != 0)
                    {
                        _bootloader_status = BOOTLOADER_S19_ERROR;
                        _s19_file_done = TRUE;
                        return FLASH_IMAGE_ERROR;
                    }
                    else 
                    {
                        /* File completely read successfully */
                        _bootloader_status = BOOTLOADER_OK;
                        _s19_file_done = TRUE;
                        return FLASH_IMAGE_SUCCESS;
                    } /* EndIf */
                } /* EndFor */
                break;
            case '0':
            case '4':
            case '5':
            case '6':
            default:
            break;
        } /* EndSwitch */
    return FLASH_IMAGE_SUCCESS;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : get_hex_value
* Returned Value : unsigned char, hex value of character
* Comments       : Converts ASCII character to hex value 
*     
*END*--------------------------------------------------------------------*/
static uint8_t get_hex_value 
   (
     /* [IN] the text to parse */
     uint8_t text
   )
{ /* Body */
    switch (text)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return (uint8_t)(text - '0');
        case 'A':
        case 'a':
            return 10;
        case 'B':
        case 'b':
            return 11;
        case 'C':
        case 'c':
            return 12;
        case 'D':
        case 'd':
            return 13;
        case 'E':
        case 'e':
            return 14;
        case 'F':
        case 'f':
            return 15;
        default:
            return 0xFF;
    } /* EndSwitch */
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : get_spair
* Returned Value : unsigned char, converted hex byte
* Comments       : Gets pair of characters, and converts to hex byte
*     
*END*--------------------------------------------------------------------*/
static uint8_t get_spair
   (
     /* [IN] the array to parse */
     uint8_t *arr,
     /* [IN] point to array data */
     uint8_t point
   )
{
    /* Body */
    uint8_t ch;
    uint8_t upper,lower;

    ch = arr[point];
    upper = (uint8_t)(get_hex_value(ch));
    if(upper == 0xFF) 
    {
        /* Not a proper S19 file */
        _s19_file_done = TRUE;
    } /* EndIf */
    upper = (uint8_t)(upper << 4);
    ch=arr[point+1];
    lower= (uint8_t)(get_hex_value(ch));

    if(lower == 0xFF) 
    {
        /* Not a proper S19 file */
        _s19_file_done = TRUE;
    } /* EndIf */
    return (uint8_t)(upper | lower);
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : parse_array_cw
* Returned Value : 0 if successful, other value if error
* Comments       : Get full lines from an array and flash them
*     
*END*--------------------------------------------------------------------*/
static uint8_t parse_array_cw
   (
     /* [IN] the array to parse */
     uint8_t *array,
     /* [IN] data length of the array */
     uint32_t size_of_array,
     /* [IN] a allocated buffer */
     uint8_t *line
   )
{
    /* Body */
    uint8_t result;
    uint16_t i,j;
    uint32_t data_length;
    uint8_t c;
    static uint32_t curL,totalL;             /* current Length , taltal Length */
    static uint8_t newline = 0;

    for (i = 0 ; i<size_of_array;i++)
    {
        c = array[i];
        line[curL] = c;
        curL++;
        if(curL == 8) 
        {
            data_length = get_uint32(line,4);
            /* Get total length of current line */
            totalL = data_length + 8;
            /* start a new line with address and data length */
            newline = 1;
        } /* EndIf */
        if(newline)
        {
            if (curL >= totalL)                   /* Get full line */
            {
                /* Parse and flash current line */
                result = parse_line_cw(line);
                /* Reset line and other variables */
                for (j=0 ; j<totalL;j++)
                {
                    line[j]=0xFF;
                }
                curL=0;
                totalL=0;
                newline=0;
            } /* EndIf */
        } /* EndIf */
    } /* EndFor */
    return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : parse_line_cw
* Returned Value : 0 if successful, other value if error
* Comments       : Parse and flash a full line
*     
*END*--------------------------------------------------------------------*/
static uint8_t parse_line_cw
   (
     /* [IN] the line to parse */
     uint8_t *line
   )
{ /* Body */
   uint8_t  result = FLASH_IMAGE_SUCCESS;
   unsigned char *dst;
   uint32_t   data_length;
   
   dst = (unsigned char *)get_uint32(line,0);    /* address to flash */  
   data_length = get_uint32(line,4);       /* length of data */
   line += 8;   
   /* DisableInterrupts */
   _int_disable();
   memcpy((void *)dst, (void *)line,data_length);
   /* EnableInterrupts */;
   _int_enable();
   return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : get_uint32
* Returned Value : result
* Comments       : get a unsign long number from an array
*     
*END*--------------------------------------------------------------------*/
static uint32_t get_uint32 
   (
     /* [IN] the array */
     uint8_t* arr,
     /* [IN] the index of array */
     uint32_t index
   )
{
    uint32_t result;
    result = *(uint32_t*)(arr + index);
    return result ;
} /* EndBody */
/* EOF */
