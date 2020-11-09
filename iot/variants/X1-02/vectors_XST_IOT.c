
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
//void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_COMB_Handler(void);
//void __attribute__ ((weak, alias ("Default_Handler"))) PORT1_COMB_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIMER0_Handler    (void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIMER1_Handler    (void);
void __attribute__ ((weak, alias ("Default_Handler"))) DUALTIMER_HANDLER (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTOVF0_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTOVF1_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) UARTOVF2_Handler  (void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA_Handler       (void);
void __attribute__ ((weak, alias ("Default_Handler"))) SPI_Handler       (void);
void __attribute__ ((weak, alias ("Default_Handler"))) I2C_Handler       (void);
void __attribute__ ((weak, alias ("Default_Handler"))) TRNG_Handler      (void);
void __attribute__ ((weak, alias ("Default_Handler"))) RTC_Handler       (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT0_7_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT1_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT1_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT1_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT1_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT1_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT1_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT1_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT1_7_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT2_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT2_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT2_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT2_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT2_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT2_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT2_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT2_7_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT3_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT3_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT3_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT3_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT3_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT3_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT3_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT3_7_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT4_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT4_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT4_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT4_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT4_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT4_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT4_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT4_7_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT5_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT5_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT5_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT5_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT5_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT5_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT5_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT5_7_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT6_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT6_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT6_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT6_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT6_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT6_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT6_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT6_7_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT7_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT7_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT7_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT7_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT7_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT7_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT7_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT7_7_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT8_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT8_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT8_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT8_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT8_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT8_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT8_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT8_7_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT9_0_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT9_1_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT9_2_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT9_3_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT9_4_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT9_5_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT9_6_Handler   (void);
void __attribute__ ((weak, alias ("Default_Handler"))) PORT9_7_Handler   (void);

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
		Default_Handler   ,                       /*   6 PWM 1 Handler                */
		Default_Handler   ,                       /*   7 PWM 2 Handler                */
		TIMER0_Handler    ,                       /*   8 TIMER 0 handler              */
		TIMER1_Handler    ,                       /*   9 TIMER 1 handler              */
		DUALTIMER_HANDLER ,                       /*  10 Dual timer 2 handler         */
		0                 ,                       /*  11 Not used                     */
		UARTOVF0_Handler  ,                       /*  12 UART 0 Overflow Handler      */
		UARTOVF1_Handler  ,                       /*  13 UART 1 Overflow Handler      */
		UARTOVF2_Handler  ,                       /*  14 UART 2 Overflow Handler      */
		DMA_Handler       ,                       /*  15 DMA done + error Handler     */
		Default_Handler   ,                       /*  16 UART 3 RX Handler            */
		Default_Handler   ,                       /*  17 UART 3 TX Handler            */
		Default_Handler   ,                       /*  18 UART 4 RX Handler            */
		Default_Handler   ,                       /*  19 UART 4 TX Handler            */
		Default_Handler   ,                       /*  20 UART 3 Overflow Handler      */
		Default_Handler   ,                       /*  21 UART 4 Overflow Handler      */
		Default_Handler   ,                       /*  22 PWM 3 Handler                */
		Default_Handler   ,                       /*  23 PWM 4 Handler                */
		Default_Handler   ,                       /*  24 Dual Timer 1 Handler         */
		0                 ,                       /*  25 Not used                     */
		0                 ,                       /*  26 Not used                     */
		0                 ,                       /*  27 Not used                     */
		0                 ,                       /*  28 Not used                     */
		0                 ,                       /*  29 Not used                     */
		0                 ,                       /*  30 Not used                     */
		0                 ,                       /*  31 Not used                     */
		SPI_Handler       ,                       /*  32 SPI Handler                  */
		I2C_Handler       ,                       /*  33 I2C Handler                  */
		TRNG_Handler      ,                       /*  34 TRNG Handler                 */
		RTC_Handler       ,                       /*  35 RTC Handler                  */
        Default_Handler   ,                       /*  36                              */
        Default_Handler   ,                       /*  37                              */
        Default_Handler   ,                       /*  38                              */
        Default_Handler   ,                       /*  39                              */
        PORT0_0_Handler   ,                       /*  40                              */
        PORT0_1_Handler   ,                       /*  41                              */
        PORT0_2_Handler   ,                       /*  42                              */
        PORT0_3_Handler   ,                       /*  43                              */
        PORT0_4_Handler   ,                       /*  44                              */
        PORT0_5_Handler   ,                       /*  45                              */
        PORT0_6_Handler   ,                       /*  46                              */
        PORT0_7_Handler   ,                       /*  47                              */
        PORT1_0_Handler   ,                       /*  48                              */
        PORT1_1_Handler   ,                       /*  49                              */
        PORT1_2_Handler   ,                       /*  50                              */
        PORT1_3_Handler   ,                       /*  51                              */
        PORT1_4_Handler   ,                       /*  52                              */
        PORT1_5_Handler   ,                       /*  53                              */
        PORT1_6_Handler   ,                       /*  54                              */
        PORT1_7_Handler   ,                       /*  55                              */
        PORT2_0_Handler   ,                       /*  56                              */
        PORT2_1_Handler   ,                       /*  57                              */
        PORT2_2_Handler   ,                       /*  58                              */
        PORT2_3_Handler   ,                       /*  59                              */
        PORT2_4_Handler   ,                       /*  60                              */
        PORT2_5_Handler   ,                       /*  61                              */
        PORT2_6_Handler   ,                       /*  62                              */
        PORT2_7_Handler   ,                       /*  63                              */
        PORT3_0_Handler   ,                       /*  64                              */
        PORT3_1_Handler   ,                       /*  65                              */
        PORT3_2_Handler   ,                       /*  66                              */
        PORT3_3_Handler   ,                       /*  67                              */
        PORT3_4_Handler   ,                       /*  68                              */
        PORT3_5_Handler   ,                       /*  69                              */
        PORT3_6_Handler   ,                       /*  70                              */
        PORT3_7_Handler   ,                       /*  71                              */
		PORT4_0_Handler   ,                       /*  72                              */
		PORT4_1_Handler   ,                       /*  73                              */
		PORT4_2_Handler   ,                       /*  74                              */
		PORT4_3_Handler   ,                       /*  75                              */
		PORT4_4_Handler   ,                       /*  76                              */
		PORT4_5_Handler   ,                       /*  77                              */
		PORT4_6_Handler   ,                       /*  78                              */
		PORT4_7_Handler   ,                       /*  79                              */
		PORT5_0_Handler   ,                       /*  80                              */
		PORT5_1_Handler   ,                       /*  81                              */
		PORT5_2_Handler   ,                       /*  82                              */
		PORT5_3_Handler   ,                       /*  83                              */
		PORT5_4_Handler   ,                       /*  84                              */
		PORT5_5_Handler   ,                       /*  85                              */
		PORT5_6_Handler   ,                       /*  86                              */
		PORT5_7_Handler   ,                       /*  87                              */
		PORT6_0_Handler   ,                       /*  88                              */
		PORT6_1_Handler   ,                       /*  89                              */
		PORT6_2_Handler   ,                       /*  90                              */
		PORT6_3_Handler   ,                       /*  91                              */
		PORT6_4_Handler   ,                       /*  92                              */
		PORT6_5_Handler   ,                       /*  93                              */
		PORT6_6_Handler   ,                       /*  94                              */
		PORT6_7_Handler   ,                       /*  95                              */
		PORT7_0_Handler   ,                       /*  96                              */
		PORT7_1_Handler   ,                       /*  97                              */
		PORT7_2_Handler   ,                       /*  98                              */
		PORT7_3_Handler   ,                       /*  99                              */
		PORT7_4_Handler   ,                       /*  100                             */
		PORT7_5_Handler   ,                       /*  101                             */
		PORT7_6_Handler   ,                       /*  102                             */
		PORT7_7_Handler   ,                       /*  103                             */
		PORT8_0_Handler   ,                       /*  104                             */
		PORT8_1_Handler   ,                       /*  105                             */
		PORT8_2_Handler   ,                       /*  106                             */
		PORT8_3_Handler   ,                       /*  107                             */
		PORT8_4_Handler   ,                       /*  108                             */
		PORT8_5_Handler   ,                       /*  109                             */
		PORT8_6_Handler   ,                       /*  110                             */
		PORT8_7_Handler   ,                       /*  111                             */
		PORT9_0_Handler   ,                       /*  112                             */
		PORT9_1_Handler   ,                       /*  113                             */
		PORT9_2_Handler   ,                       /*  114                             */
		PORT9_3_Handler   ,                       /*  115                             */
		PORT9_4_Handler   ,                       /*  116                             */
		PORT9_5_Handler   ,                       /*  117                             */
		PORT9_6_Handler   ,                       /*  118                             */
		PORT9_7_Handler   ,                       /*  119                             */
		Default_Handler   ,                       /*  120                             */
		Default_Handler   ,                       /*  121                             */
		Default_Handler   ,                       /*  122                             */
		Default_Handler   ,                       /*  123                             */
		Default_Handler   ,                       /*  124                             */
		Default_Handler   ,                       /*  125                             */
		Default_Handler   ,                       /*  126                             */
		Default_Handler   ,                       /*  127                             */
		Default_Handler   ,                       /*  128                             */
		Default_Handler                           /*  129                             */
		                                          /* Interrupts 130 .. 223 are left out */
    };

// ----------------------------------------------------------------------------

// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.

void __attribute__ ((section(".after_vectors")))
Default_Handler(void)
{
    while (1);
}

// ----------------------------------------------------------------------------

#ifdef __cplusplus
 }
#endif
