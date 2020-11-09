/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef __cplusplus
 extern "C" {
#endif

// ----------------------------------------------------------------------------

#include "ExceptionHandlers.h"

// ----------------------------------------------------------------------------

void __attribute__((weak))
Default_Handler(void);

// Forward declaration of the specific IRQ handlers. These are aliased
// to the Default_Handler, which is a 'forever' loop. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions

void __attribute__ ((weak, alias ("Default_Handler"))) UARTRX0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTTX0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTRX1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTTX1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTRX2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTTX2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_COMB_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT1_COMB_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIMER0_Handler    (void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIMER1_Handler    (void);
void __attribute__ ((weak, alias ("Default_Handler"))) DUALTIMER_HANDLER (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTOVF0_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTOVF1_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTOVF2_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA_Handler       (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_7_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_8_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_9_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_10_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_11_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_12_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_13_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_14_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_15_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) SPI_Handler       (void);
void __attribute__ ((weak, alias ("Default_Handler"))) I2C_Handler       (void);
void __attribute__ ((weak, alias ("Default_Handler"))) TRNG_Handler      (void);
void __attribute__ ((weak, alias ("Default_Handler"))) RTC_Handler       (void);

// ----------------------------------------------------------------------------

extern unsigned int _estack;

typedef void
(* const pHandler)(void);

// ----------------------------------------------------------------------------

// The vector table.
// This relies on the linker script to place at correct location in memory.

__attribute__ ((section(".isr_vector"),used))
pHandler __isr_vectors[] =
  { //
    (pHandler) &_estack,                          // -16 The initial stack pointer
        Reset_Handler,                            // -15 The reset handler
        NMI_Handler,                              // -14 The NMI handler
        HardFault_Handler,                        // -13 The hard fault handler
        MemManage_Handler,                        // -12 The MPU fault handler
        BusFault_Handler,                         // -11 The bus fault handler
        UsageFault_Handler,                       // -10 The usage fault handler
        0,                                        //  -9 Reserved
        0,                                        //  -8 Reserved
        0,                                        //  -7 Reserved
        0,                                        //  -6 Reserved
        SVC_Handler,                              //  -5 SVCall handler
        DebugMon_Handler,                         //  -4 Debug monitor handler
        0,                                        //  -3 Reserved
        PendSV_Handler,                           //  -2 The PendSV handler
        SysTick_Handler,                          //  -1 The SysTick handler

        // ----------------------------------------------------------------------
        // DEVICE vectors
		UARTRX0_Handler   ,                       /*   0 UART 0 RX Handler            */
		UARTTX0_Handler   ,                       /*   1 UART 0 TX Handler            */
		UARTRX1_Handler   ,                       /*   2 UART 1 RX Handler            */
		UARTTX1_Handler   ,                       /*   3 UART 1 TX Handler            */
		UARTRX2_Handler   ,                       /*   4 UART 2 RX Handler            */
		UARTTX2_Handler   ,                       /*   5 UART 2 TX Handler            */
		PORT0_COMB_Handler,                       /*   6 GPIO Port 0 Combined Handler */
		PORT1_COMB_Handler,                       /*   7 GPIO Port 1 Combined Handler */
		TIMER0_Handler    ,                       /*   8 TIMER 0 handler              */
		TIMER1_Handler    ,                       /*   9 TIMER 1 handler              */
		DUALTIMER_HANDLER ,                       /*  10 Dual timer 2 handler         */
		0                 ,                       /*  11 Not used                     */
		UARTOVF0_Handler  ,                       /*  12 UART 0 Overflow Handler      */
		UARTOVF1_Handler  ,                       /*  13 UART 1 Overflow Handler      */
		UARTOVF2_Handler  ,                       /*  14 UART 2 Overflow Handler      */
		DMA_Handler       ,                       /*  15 DMA done + error Handler     */
		PORT0_0_Handler   ,                       /*  16 GPIO Port 0 pin 0 Handler    */
		PORT0_1_Handler   ,                       /*  17 GPIO Port 0 pin 1 Handler    */
		PORT0_2_Handler   ,                       /*  18 GPIO Port 0 pin 2 Handler    */
		PORT0_3_Handler   ,                       /*  19 GPIO Port 0 pin 3 Handler    */
		PORT0_4_Handler   ,                       /*  20 GPIO Port 0 pin 4 Handler    */
		PORT0_5_Handler   ,                       /*  21 GPIO Port 0 pin 5 Handler    */
		PORT0_6_Handler   ,                       /*  22 GPIO Port 0 pin 6 Handler    */
		PORT0_7_Handler   ,                       /*  23 GPIO Port 0 pin 7 Handler    */
		PORT0_8_Handler   ,                       /*  24 GPIO Port 0 pin 8 Handler    */
		PORT0_9_Handler   ,                       /*  25 GPIO Port 0 pin 9 Handler    */
		PORT0_10_Handler  ,                       /*  26 GPIO Port 0 pin 10 Handler   */
		PORT0_11_Handler  ,                       /*  27 GPIO Port 0 pin 11 Handler   */
		PORT0_12_Handler  ,                       /*  28 GPIO Port 0 pin 12 Handler   */
		PORT0_13_Handler  ,                       /*  29 GPIO Port 0 pin 13 Handler   */
		PORT0_14_Handler  ,                       /*  30 GPIO Port 0 pin 14 Handler   */
		PORT0_15_Handler  ,                       /*  31 GPIO Port 0 pin 15 Handler   */
		SPI_Handler       ,                       /*  32 SPI Handler                  */
		I2C_Handler       ,                       /*  33 I2C Handler                  */
		TRNG_Handler      ,                       /*  34 TRNG Handler                 */
		RTC_Handler                               /*  35 RTC Handler                  */
		                                          /* Interrupts 36 .. 223 are left out */
    };

// ----------------------------------------------------------------------------

// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.

void __attribute__ ((section(".after_vectors")))
Default_Handler(void)
{
  //while (1)
  //  {
  //  }
}

// ----------------------------------------------------------------------------

#ifdef __cplusplus
 }
#endif
