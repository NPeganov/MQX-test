/*HEADER**********************************************************************
*
* Copyright 2014 Freescale Semiconductor, Inc.
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
*   This file contains flash boot code to initialize chip selects,
*   disable the watchdog timer and initialize the PLL.
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _bsp_watchdog_disable
* Returned Value   : void
* Comments         :
*   Disable watchdog timer
*
*END*-------------------------------------------------------------------------*/

void _bsp_watchdog_disable(void) 
{
    WDOG_MemMapPtr reg = WDOG_BASE_PTR;

    /* NOTE: DO NOT SINGLE STEP THROUGH THIS FUNCTION!!!
    * There are timing requirements for the execution of the unlock. If
    * you single step through the code you will cause the CPU to reset.
    */

    /* unlock watchdog */
    reg->UNLOCK = 0xc520;
    reg->UNLOCK = 0xd928;

    /* disable watchdog */
    reg->STCTRLH &= ~(WDOG_STCTRLH_WDOGEN_MASK);
}

/*FUNCTION*---------------------------------------------------------------------
* 
* Function Name    : _bsp_flexbus_setup
* Returned Value   :  none
* Comments         :
*    Setup FlexBus pins for early MRAM operation
*
*END*-------------------------------------------------------------------------*/
static void _bsp_flexbus_setup (void) 
{
    #define ALT5                    0x05
    #define OFF_CHIP_ACCESS_ALLOW   3

    PORT_MemMapPtr  pctl;
    SIM_MemMapPtr   sim = SIM_BASE_PTR;
    FB_MemMapPtr    fb_ptr = FB_BASE_PTR;

    /* Enable clock to FlexBus module */
    sim->SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
    sim->SOPT2 |= SIM_SOPT2_FBSL(OFF_CHIP_ACCESS_ALLOW);

    pctl = (PORT_MemMapPtr)PORTB_BASE_PTR;
    pctl->PCR[10] = PORT_PCR_MUX(ALT5); /* FB_AD19 */
    pctl->PCR[11] = PORT_PCR_MUX(ALT5); /* FB_AD18 */
    pctl->PCR[16] = PORT_PCR_MUX(ALT5); /* FB_AD17 */
    pctl->PCR[17] = PORT_PCR_MUX(ALT5); /* FB_AD16 */
#if !BSPCFG_ENABLE_SAI
    pctl->PCR[18] = PORT_PCR_MUX(ALT5); /* FB_AD15 */
    pctl->PCR[19] = PORT_PCR_MUX(ALT5); /* FB_OE_b */
#endif
    pctl->PCR[20] = PORT_PCR_MUX(ALT5); /* FB_AD31 */
    pctl->PCR[21] = PORT_PCR_MUX(ALT5); /* FB_AD30 */
    pctl->PCR[22] = PORT_PCR_MUX(ALT5); /* FB_AD29 */
    pctl->PCR[23] = PORT_PCR_MUX(ALT5); /* FB_AD28 */
    
    pctl = (PORT_MemMapPtr)PORTC_BASE_PTR;
    pctl->PCR[0] = PORT_PCR_MUX(ALT5); /* FB_AD14 */
#if !BSPCFG_ENABLE_SAI
    pctl->PCR[1] = PORT_PCR_MUX(ALT5); /* FB_AD13 */
    pctl->PCR[5] = PORT_PCR_MUX(ALT5); /* FB_AD10 */
    pctl->PCR[6] = PORT_PCR_MUX(ALT5); /* FB_AD9 */
#endif
    pctl->PCR[2] = PORT_PCR_MUX(ALT5); /* FB_AD12 */
    pctl->PCR[3] = PORT_PCR_MUX(ALT5); /* FB_CLKOUT */
    pctl->PCR[4] = PORT_PCR_MUX(ALT5); /* FB_AD11 */
    pctl->PCR[7] = PORT_PCR_MUX(ALT5); /* FB_AD8 */
    pctl->PCR[8] = PORT_PCR_MUX(ALT5); /* FB_AD7 */
    pctl->PCR[9] = PORT_PCR_MUX(ALT5); /* FB_AD6 */
    pctl->PCR[10] = PORT_PCR_MUX(ALT5); /* FB_AD5 */
    pctl->PCR[11] = PORT_PCR_MUX(ALT5); /* FB_RW_b */
    pctl->PCR[12] = PORT_PCR_MUX(ALT5); /* FB_AD27 */
    pctl->PCR[13] = PORT_PCR_MUX(ALT5); /* FB_AD26 */
    pctl->PCR[14] = PORT_PCR_MUX(ALT5); /* FB_AD25 */
    pctl->PCR[15] = PORT_PCR_MUX(ALT5); /* FB_AD24 */
    
    pctl = (PORT_MemMapPtr)PORTD_BASE_PTR;
    pctl->PCR[0] = PORT_PCR_MUX(ALT5); /* FB_ALE */
    pctl->PCR[1] = PORT_PCR_MUX(ALT5); /* FB_CS0_b */
    pctl->PCR[2] = PORT_PCR_MUX(ALT5); /* FB_AD4 */
    pctl->PCR[3] = PORT_PCR_MUX(ALT5); /* FB_AD3 */
    pctl->PCR[4] = PORT_PCR_MUX(ALT5); /* FB_AD2 */
    pctl->PCR[5] = PORT_PCR_MUX(ALT5); /* FB_AD1 */
    pctl->PCR[6] = PORT_PCR_MUX(ALT5); /* FB_AD0 */

    /* configure chip select multiplexers (CS1) */
    fb_ptr->CSPMCR=FB_CSPMCR_GROUP1(1);
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _bsp_flexbus_mram_setup
* Returned Value   :
* Comments         :
*    Setup FlexBus for MRAM operation
*
*END*-------------------------------------------------------------------------*/
static void _bsp_flexbus_mram_setup (const uint32_t base_address)
{
    FB_MemMapPtr fb_ptr = FB_BASE_PTR;

    /* Enable external MRAM mapped on CS0 */
    /* CS0 base address */
    fb_ptr->CS[0].CSAR = base_address;
    /* CS0 control (8bit data, 1 wait state) */
    fb_ptr->CS[0].CSCR = FB_CSCR_AA_MASK | 
                         FB_CSCR_WS(2)   | 
                         FB_CSCR_PS(1)   | 
                         FB_CSCR_BEM_MASK; 

    /* CS0 address mask and enable */
    fb_ptr->CS[0].CSMR = FB_CSMR_BAM(0x07) | FB_CSMR_V_MASK;
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _bsp_flexbus_lcd_setup
* Returned Value   :
* Comments         :
*    Setup FlexBus for LCD operation
*
*END*-------------------------------------------------------------------------*/
void _bsp_flexbus_lcd_setup (const uint32_t base_address)
{
    FB_MemMapPtr fb_ptr = FB_BASE_PTR;

    /* Enable external LCD mapped on CS0 */
    fb_ptr->CS[0].CSAR = base_address;
    fb_ptr->CS[0].CSCR = FB_CSCR_BLS_MASK |
                         FB_CSCR_AA_MASK  |
                         FB_CSCR_PS(2)    |
                         FB_CSCR_BEM_MASK;

    /*
     * The address range is set to 128K because
     * the DC signal is connected on address wire
     */
    fb_ptr->CS[0].CSMR = FB_CSMR_BAM(1) | FB_CSMR_V_MASK;      
}

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _bsp_flexbus_pccard_setup
* Returned Value   :
* Comments         :
*    Setup flexbus for pccard operation
*
*END*-------------------------------------------------------------------------*/
void _bsp_flexbus_pccard_setup (const uint32_t base_address)
{
    FB_MemMapPtr fb_ptr = FB_BASE_PTR;

    /* invalidate CS configuration first */
    fb_ptr->CS[1].CSMR = 0; 

    /* Enable external PCCARD mapped on CS1 */
    fb_ptr->CS[1].CSAR = base_address;
    /* CS1 control (8bit data, 5 wait states) */
    fb_ptr->CS[1].CSCR = FB_CSCR_AA_MASK |
                         FB_CSCR_WS(11) |
                         FB_CSCR_PS(1) |
                         FB_CSCR_BEM_MASK; 

    /* CS1 address mask (64 KB) and enable */
    fb_ptr->CS[1].CSMR = FB_CSMR_BAM(0) | FB_CSMR_V_MASK; 
}


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : init_hardware
* Returned Value   : void
* Comments         :
*   Initialize Kinetis device.
*
*END*-------------------------------------------------------------------------*/

void init_hardware(void)
{
#if PE_LDD_VERSION
    /*  Watch Dog disabled by CPU bean (need to setup in CPU Inspector) */
    __pe_initialize_hardware();
#else
    _bsp_initialize_hardware();
    
#endif

    /* Enable pin clocks */
    _bsp_gpio_io_init ();
    /* Initialize FlexBus */
    _bsp_flexbus_setup();
    /* Initialize MRAM */
    _bsp_flexbus_mram_setup((uint32_t)BSP_EXTERNAL_MRAM_RAM_BASE);
}
