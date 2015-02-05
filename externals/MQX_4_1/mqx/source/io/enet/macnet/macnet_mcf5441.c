/*HEADER**********************************************************************
*
* Copyright 2010 Freescale Semiconductor, Inc.
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
* 
* Copyright (c) 2010 Freescale Semiconductor;
* All Rights Reserved
*
* 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
*
*
* $FileName: macnet_mcf5441.c$
* $Version : 3.8.11.0$
* $Date    : Jun-6-2012$
*
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <enet.h>
#include <enetprv.h>
#include <macnet_prv.h>

const uint32_t MACNET_vectors[MACNET_DEVICE_COUNT][ENET_NUM_INTS] = {
    {
        MCF5441_INT_ENET0_EIR_TXF,
        MCF5441_INT_ENET0_EIR_TXB,
        MCF5441_INT_ENET0_EIR_UN,
        MCF5441_INT_ENET0_EIR_RL,
        MCF5441_INT_ENET0_EIR_RXF,
        MCF5441_INT_ENET0_EIR_RXB,
        MCF5441_INT_ENET0_EIR_MII,
        MCF5441_INT_ENET0_EIR_LC,
        MCF5441_INT_ENET0_EIR_GRA,
        MCF5441_INT_ENET0_EIR_EBERR,
        MCF5441_INT_ENET0_EIR_BABT,
        MCF5441_INT_ENET0_EIR_BABR,
        MCF5441_INT_ENET0_EIR_TS_AVAIL,
        MCF5441_INT_ENET0_EIR_WAKEUP,
        MCF5441_INT_ENET0_EIR_PLR,
        MCF5441_INT_ENET0_EIR_EBERR /* TS_TIMER interrupt is not generated by MACNET on M5441x */
    },
    {
        MCF5441_INT_ENET1_EIR_TXF,
        MCF5441_INT_ENET1_EIR_TXB,
        MCF5441_INT_ENET1_EIR_UN,
        MCF5441_INT_ENET1_EIR_RL,
        MCF5441_INT_ENET1_EIR_RXF,
        MCF5441_INT_ENET1_EIR_RXB,
        MCF5441_INT_ENET1_EIR_MII,
        MCF5441_INT_ENET1_EIR_LC,
        MCF5441_INT_ENET1_EIR_GRA,
        MCF5441_INT_ENET1_EIR_EBERR,
        MCF5441_INT_ENET1_EIR_BABT,
        MCF5441_INT_ENET1_EIR_BABR,
        MCF5441_INT_ENET1_EIR_TS_AVAIL,
        MCF5441_INT_ENET1_EIR_WAKEUP,
        MCF5441_INT_ENET1_EIR_PLR,
        MCF5441_INT_ENET1_EIR_EBERR /* TS_TIMER interrupt is not generated by MACNET on M5441x */
    }    
};

#if ENETCFG_SUPPORT_PTP
extern uint64_t  MACNET_PTP_seconds;
#endif /* ENETCFG_SUPPORT_PTP */

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : MACNET_get_base_address  
* Returned Value   : Pointer to desired enem device or NULL if not present
* Comments         :
*    This function returns pointer to base address of address space of the 
*    desired enet device. Returns NULL otherwise.
*
*END*----------------------------------------------------------------------*/

void *MACNET_get_base_address(uint32_t device) {
    void   *addr = NULL;
    
    switch (device) {
    case 0:
        addr = (void *)(&((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->MACNET0);
        break;
    case 1:
        addr = (void *)(&((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->MACNET1);
        break;
    }
    
    return addr;
}


 
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : MACNET_io_init
* Returned Value   : none
* Comments         :
*    This function performs BSP-specific initialization related to ENET
*
*END*----------------------------------------------------------------------*/

void MACNET_io_init( uint32_t    device )
{

   if (device >= MACNET_DEVICE_COUNT) 
      return;

   _bsp_enet_io_init(device);
}

#if ENETCFG_SUPPORT_PTP
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : MACNET_ptp_install_ts_avail_isr
* Returned Value   : none
* Comments         :
*    This function initializes the TS_AVAIL interrupt
*
*END*----------------------------------------------------------------------*/

bool MACNET_ptp_install_ts_avail_isr( ENET_CONTEXT_STRUCT_PTR enet_ptr, MACNET_INIT_STRUCT const * enet_init_ptr)
{
   return (MACNET_install_isr(enet_ptr, 4, ENET_INT_TS_AVAIL, MACNET_ptp_store_txstamp, enet_init_ptr->ERX_LEVEL,enet_init_ptr->ERX_SUBLEVEL)) ;
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : MACNET_ptp_install_ts_timer_isr
* Returned Value   : none
* Comments         :
*    This function initializes the TS_TIMER interrupt
*
*END*----------------------------------------------------------------------*/

bool MACNET_ptp_install_ts_timer_isr( ENET_CONTEXT_STRUCT_PTR enet_ptr, MACNET_INIT_STRUCT const * enet_init_ptr)
{
   VMCF5441_STRUCT_PTR reg = (VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR();

   /* Initialize DMA Timer */
   _bsp_dtimer_gpio_init(MACNET_PTP_DTIMER);
   
   /* Output reference request, interrupt enable */
   reg->DMA_TIMER[MACNET_PTP_DTIMER].DTMR = 0; 
   /* 1588 mode */
   reg->DMA_TIMER[MACNET_PTP_DTIMER].DTXTMR = MCF5441_DTIM_DTXMR_EN1588; 
   /* Reference value 1000000000*/
   reg->DMA_TIMER[MACNET_PTP_DTIMER].DTRR = MACNET_1588_ATPER_VALUE - MACNET_1588_CLOCK_INC;
   /* Clear old events */
   reg->DMA_TIMER[MACNET_PTP_DTIMER].DTER = MCF5441_DTIM_DTER_REF; 
   reg->DMA_TIMER[MACNET_PTP_DTIMER].DTMR = MCF5441_DTIM_DTMR_ORRI | 
                                            MCF5441_DTIM_DTMR_OM |
                                            MCF5441_DTIM_DTMR_RST; 
   
   reg->CCM.MISCCR3 |= ((enet_ptr->PARAM_PTR->ENET_IF->MAC_NUMBER) << 12);
   
   if (_int_install_isr(MCF5441_INT_DTIM0+MACNET_PTP_DTIMER, MACNET_ptp_increment_seconds, (void *)enet_ptr)==NULL) {
       return FALSE;
   }
   
   /* Initialize interrupt priority and level */
   _psp_set_int_prio_and_level((PSP_INTERRUPT_TABLE_INDEX)(MCF5441_INT_DTIM0+MACNET_PTP_DTIMER), enet_init_ptr->ETX_LEVEL + 1,enet_init_ptr->ETX_SUBLEVEL + 1, TRUE);
   
   return TRUE;
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MACNET_ptp_increment_seconds
*  Returned Value :  
*  Comments       : 
*    This function is the TS_TIMER interrupt service routine
*
*END*-----------------------------------------------------------------*/
void MACNET_ptp_increment_seconds
   (
         /* [IN] the Ethernet state structure */
      void    *enet
   )
{ /* Body */
   ENET_CONTEXT_STRUCT_PTR    enet_ptr = (ENET_CONTEXT_STRUCT_PTR)enet;
   MACNET_CONTEXT_STRUCT_PTR  macnet_context_ptr = (MACNET_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;
   ENET_MemMapPtr             macnet_ptr= macnet_context_ptr->MACNET_ADDRESS;
   uint32_t                    events;
   VMCF5441_STRUCT_PTR reg = (VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR();

   if (macnet_ptr == NULL) return;

   events = macnet_ptr->EIR;
   while (events & ENET_EIR_TS_TIMER_MASK) {

      /* Clear the TS_TIMER interrupt */
      macnet_ptr->EIR = ENET_EIR_TS_TIMER_MASK; 

      /* Increment seconds */
      MACNET_PTP_seconds++;

      events = macnet_ptr->EIR;
   }
   
   /* Clear interrupt event */
   reg->DMA_TIMER[MACNET_PTP_DTIMER].DTER |= MCF5441_DTIM_DTER_REF; 
}

#endif /* ENETCFG_SUPPORT_PTP */
