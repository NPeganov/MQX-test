/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
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
*   This file contains definitions for the MFC5225 interrupt controller
*   functions.
*
*
*END************************************************************************/

#include "mqx_inc.h"
#include "bsp.h"

/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : compute_icntl_registers
* Returned Value  : uint32_t
* Comments        :
*   Compute which registers to touch for the given interrupt number
*
*END*---------------------------------------------------------------------*/

static bool compute_icntl_registers
   (
      // [IN] Interrupt number
      PSP_INTERRUPT_TABLE_INDEX irq,

      // [OUT] Interrupt control register
      volatile unsigned char     **ictlr,

      // [OUT] Interrupt mask register
      volatile uint32_t   **maskr,

      // [OUT] Interrupt mask register bit
      uint32_t                   *bit,

      // [OUT] Interrupt clear bit 0 pointer to register
      volatile uint32_t   **clear
   )
{ /* Body */

   VMCF52XX_ICTRL0_STRUCT_PTR ictrl0;
   VMCF52XX_ICTRL1_STRUCT_PTR ictrl1;

   if (irq >= PSP_INT_FIRST_EXTERNAL) {
      uint32_t index = irq - PSP_INT_FIRST_EXTERNAL;

      if (index < 32) {
         ictrl0 = PSP_GET_ICTRL0_BASE();
         *ictlr = &ictrl0->ICR[index];
         *maskr = &ictrl0->ICTRL.IMRL;
         *bit = 1 << index;
         *clear = &ictrl0->ICTRL.IMRL;
         return TRUE;
      } /* Endif */

      if (index < 64) {
         ictrl0 = PSP_GET_ICTRL0_BASE();
         *ictlr = &ictrl0->ICR[index];
         *maskr = &ictrl0->ICTRL.IMRH;
         *bit = 1 << (index - 32);
         *clear = &ictrl0->ICTRL.IMRL;
         return TRUE;
      } /* Endif */

      if (index < 96){
         index = irq - MCF5225_ICTRL1_BASEx;
         ictrl1 = PSP_GET_ICTRL1_BASE();
         *ictlr = &ictrl1->ICR[index];
         *maskr = &ictrl1->ICTRL.IMRL;
         *bit = 1 << (index - 64);
         *clear = &ictrl1->ICTRL.IMRL;
         return TRUE;
      } /* Endif */

      if (index < 128){
         index = irq - MCF5225_ICTRL1_BASEx;
         ictrl1 = PSP_GET_ICTRL1_BASE();
         *ictlr = &ictrl1->ICR[index];
         *maskr = &ictrl1->ICTRL.IMRH;
         *bit = 1 << (index - 96);
         *clear = &ictrl1->ICTRL.IMRL;
         return TRUE;
      } /* Endif */

   } /* Endif */

   return FALSE;

} /* Endbody */


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mcf5225_int_init
* Returned Value  : uint32_t
* Comments        :
*   Initialize a specific interrupt in the proper interrupt controller
*
*END*---------------------------------------------------------------------*/

uint32_t _mcf5225_int_init
   (
      // [IN} Interrupt number
      PSP_INTERRUPT_TABLE_INDEX irq,

      // [IN} Interrupt priority level
      _int_level                level,

      // [IN} Interrupt sub-priority level within priority
      _int_priority             sublevel,

      // [IN} Unmask the interrupt now?
      bool                   unmask
   )
{ /* Body */

   volatile unsigned char        *ictrl;
   volatile uint32_t      *maskr, * clear;
   uint32_t bit, mask;
   uint32_t temp;

   if (compute_icntl_registers(irq,&ictrl,&maskr,&bit,&clear)) {
      temp = _psp_get_sr();
      _psp_set_sr(temp | 0x0700);
      mask = *maskr;
      if (!unmask) {
         *maskr = mask | bit;
      } /* Endif */
      *ictrl = ((level & 7) << 3) | (sublevel & 7);
      if (unmask) {
         *maskr = mask & ~bit;
         *clear = *clear & 0xFFFFFFFE;
      } /* Endif */
      _psp_set_sr(temp);
      return MQX_OK;
   } /* Endif */

   return MQX_INVALID_PARAMETER;

} /* Endbody */


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mcf5225_int_unmask
* Returned Value  : uint32_t
* Comments        :
*   Clear the mask bit that inhibits the interrupt
*
*END*---------------------------------------------------------------------*/

uint32_t _mcf5225_int_unmask
   (
      // [IN} Interrupt number
      PSP_INTERRUPT_TABLE_INDEX irq
   )
{ /* Body */

   volatile unsigned char        *ictrl;
   volatile uint32_t      *maskr, * clear;
   uint32_t bit;

   if (compute_icntl_registers(irq,&ictrl,&maskr,&bit,&clear)) {
      *maskr = *maskr & ~bit;
      *clear = *clear & 0xFFFFFFFE;
      return MQX_OK;
   } /* Endif */

   return MQX_INVALID_PARAMETER;

} /* Endbody */


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mcf5225_int_mask
* Returned Value  : uint32_t
* Comments        :
*   Set the mask bit that inhibits the interrupt
*
*END*---------------------------------------------------------------------*/

uint32_t _mcf5225_int_mask
   (
      // [IN} Interrupt number
      PSP_INTERRUPT_TABLE_INDEX irq
   )
{ /* Body */

   volatile unsigned char        *ictrl;
   volatile uint32_t      *maskr, * clear;
   uint32_t bit;

   if (compute_icntl_registers(irq,&ictrl,&maskr,&bit,&clear)) {
      *maskr = *maskr | bit;
      return MQX_OK;
   } /* Endif */

   return MQX_INVALID_PARAMETER;

} /* Endbody */


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mcf5225_int_mask_all
* Returned Value  : void
* Comments        :
*   Mask all external interrupts
*
*END*---------------------------------------------------------------------*/

void  _mcf5225_int_mask_all
   (
      void
   )
{ /* Body */

   PSP_GET_ICTRL0_BASE()->ICTRL.IMRL = MCF52XX_ICTR_IMRL_MASKALL;
   PSP_GET_ICTRL1_BASE()->ICTRL.IMRL = MCF52XX_ICTR_IMRL_MASKALL;

} /* Endbody */

/* EOF */
