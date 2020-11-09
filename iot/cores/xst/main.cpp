
#include <Arduino.h>

#define PERIOD 1000UL
volatile unsigned long usTicks,msTicks;

int __attribute__ ((weak)) main(int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
	//FLASHCTRL->FC_CONTROL = (FLASHCTRL->FC_CONTROL & 0xFFFFFFF0) | 1;
	usTicks = 0UL;
	msTicks = 0UL;
	SysTick_Config((FOSC/1000000UL)*PERIOD);
	setup();
	while(1) loop();
}

__attribute__ ((long_call, section(".data.data_begin"))) unsigned long micros(void){
	unsigned long currentTick = SysTick->VAL;
	unsigned long msTime = millis();
	unsigned long elapsedCycle = (FOSC/1000000UL)*PERIOD - currentTick;
	unsigned long usTime = elapsedCycle / (FOSC/1000000UL);
	return (msTime * 1000UL + usTime);
}

__attribute__ ((long_call, section(".data.data_begin"))) void delayMicroseconds(unsigned long us){
	unsigned long startTick = SysTick->VAL;
	unsigned long delayTicks = us * (FOSC/1000000UL);
	unsigned long targetTick;
	if(delayTicks < startTick) {
		targetTick = startTick - delayTicks;
	} else if(delayTicks == startTick){
		targetTick = (FOSC/1000000UL)*PERIOD;
		while(SysTick->VAL < (((FOSC/1000000UL)*PERIOD) - 100UL));
	} else {
		targetTick = ((FOSC/1000000UL)*PERIOD) - (delayTicks - startTick);
		while(SysTick->VAL < (((FOSC/1000000UL)*PERIOD) - 100UL));
	}
	while(SysTick->VAL > targetTick);
}

extern "C" {
	__attribute__ ((long_call, section(".data.data_begin"))) void SysTick_Handler(void){
			msTicks++;
	}

	/* The prototype shows it is a naked function - in effect this is just an
	assembly function. */
	void HardFault_Handler(void) __attribute__((section(".after_vectors"),naked));
	/* The fault handler implementation calls a function called
	prvGetRegistersFromStack(). */
	void HardFault_Handler(void){
		__asm volatile
		(
			" tst lr, #4                                                \n"
			" ite eq                                                    \n"
			" mrseq r0, msp                                             \n"
			" mrsne r0, psp                                             \n"
			" ldr r1, [r0, #24]                                         \n"
			" ldr r2, handler2_address_const                            \n"
			" bx r2                                                     \n"
			" handler2_address_const: .word prvGetRegistersFromStack    \n"
		);
	}

	void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
	{
		/* These are volatile to try and prevent the compiler/linker optimizing them
		away as the variables never actually get used.  If the debugger won't show the
		values of the variables, make them global my moving their declaration outside
		of this function. */
		volatile __attribute__((unused)) uint32_t r0;
		volatile __attribute__((unused)) uint32_t r1;
		volatile __attribute__((unused)) uint32_t r2;
		volatile __attribute__((unused)) uint32_t r3;
		volatile __attribute__((unused)) uint32_t r12;
		volatile __attribute__((unused)) uint32_t lr; /* Link register. */
		volatile __attribute__((unused)) uint32_t pc; /* Program counter. */
		volatile __attribute__((unused)) uint32_t psr;/* Program status register. */

	    r0 = pulFaultStackAddress[ 0 ];
	    r1 = pulFaultStackAddress[ 1 ];
	    r2 = pulFaultStackAddress[ 2 ];
	    r3 = pulFaultStackAddress[ 3 ];

	    r12 = pulFaultStackAddress[ 4 ];
	    lr = pulFaultStackAddress[ 5 ];
	    pc = pulFaultStackAddress[ 6 ];
	    psr = pulFaultStackAddress[ 7 ];

	    /* When the following line is hit, the variables contain the register values. */
	    for( ;; );
	}
}
