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
*   The Common Configurable information for the RTCS 
*   Shell examples.
*
*
*END************************************************************************/



#define ENET_DEVICE BSP_DEFAULT_ENET_DEVICE

#ifdef ENET_DEVICE
/*
** The target's IP address and netmask
*/

#ifndef ENET_IPADDR
    #define ENET_IPADDR  IPADDR(192,168,1,4) 
#endif

#ifndef ENET_IPMASK
    #define ENET_IPMASK  IPADDR(255,255,255,0) 
#endif

#endif  /* ENET_DEVICE */

/*
** Define a default gateway
*/
#define GATE_ADDR       IPADDR(192,168,1,1)

/* EOF */
