
#include "Arduino.h"

// -----------------
//   SysTick
// -----------------

void delay(unsigned long ms){
	unsigned long startTime = millis();
	while((millis() - startTime) < ms);
}

// -----------------
//   UART
// -----------------

UART0 Serial;
UART1 Serial1;
UART2 Serial2;

UART::UART(CMSDK_UART_TypeDef* UART_arg)
{
	UART_dev = UART_arg;
}

void UART0::begin(uint32_t speed)
{
	UART_dev->BAUDDIV = (uint32_t) FOSC / speed;
	UART_dev->CTRL    = 0x03;
#ifdef XST_X1_01
	CMSDK_GPIO1->ALTFUNCSET = 3 << 0; //enable RX and TX
#else
	CMSDK_GPIO8->PULLUPSET = 3 << 0;
	CMSDK_GPIO8->ALTFUNCSET = 3 << 0; //enable RX and TX
	XSTIOT_CLKCON |= (1<<3);
#endif
}

void UART1::begin(uint32_t speed)
{
	UART_dev->BAUDDIV = (uint32_t) FOSC / speed;
	UART_dev->CTRL    = 0x03;
#ifdef XST_X1_01
	CMSDK_GPIO1->ALTFUNCSET = 3 << 2; //enable RX and TX
#else
	CMSDK_GPIO7->PULLUPSET = 3 << 2;
	CMSDK_GPIO7->ALTFUNCSET = 3 << 2; //enable RX and TX
	XSTIOT_CLKCON |= (1<<4);
#endif
}

void UART2::begin(uint32_t speed)
{
	UART_dev->BAUDDIV = (uint32_t) FOSC / speed;
	UART_dev->CTRL    = 0x03;
#ifdef XST_X1_01
	CMSDK_GPIO1->ALTFUNCSET = 3 << 4; //enable RX and TX
#else
	CMSDK_GPIO7->PULLUPSET = 3 << 4;
	CMSDK_GPIO7->ALTFUNCSET = 3 << 4; //enable RX and TX
	XSTIOT_CLKCON |= (1<<5);
#endif
}

void UART0::end(void)
{
	while ((UART_dev->STATE & 1)); // Wait if Transmit Holding register is full
	delayMicroseconds(UART_dev->BAUDDIV / (FOSC/1000000UL) * 11);
	UART_dev->CTRL    = 0x00;
#ifdef XST_X1_01
	CMSDK_GPIO1->ALTFUNCCLR = 3 << 0; //enable RX and TX
#else
	CMSDK_GPIO8->ALTFUNCCLR = 3 << 0; //enable RX and TX
#endif
}

void UART1::end(void)
{
	while ((UART_dev->STATE & 1)); // Wait if Transmit Holding register is full
	delayMicroseconds(UART_dev->BAUDDIV / (FOSC/1000000UL) * 11);
	UART_dev->CTRL    = 0x00;
#ifdef XST_X1_01
	CMSDK_GPIO1->ALTFUNCCLR = 3 << 2; //enable RX and TX
#else
	CMSDK_GPIO7->ALTFUNCCLR = 3 << 2; //enable RX and TX
#endif
}

void UART2::end(void)
{
	while ((UART_dev->STATE & 1)); // Wait if Transmit Holding register is full
	delayMicroseconds(UART_dev->BAUDDIV / (FOSC/1000000UL) * 11);
	UART_dev->CTRL    = 0x00;
#ifdef XST_X1_01
	CMSDK_GPIO1->ALTFUNCCLR = 3 << 4; //enable RX and TX
#else
	CMSDK_GPIO7->ALTFUNCCLR = 3 << 4; //enable RX and TX
#endif
}

size_t UART::print(char c)
{
	while ((UART_dev->STATE & 1)); // Wait if Transmit Holding register is full
	UART_dev->DATA = c;
	return (c);
}

size_t UART::println(char c)
{
	size_t n = print(c);
	n += print("\r\n");
	return n;
}

size_t UART::print(const char string[])
{
	size_t n = 0,
		   m = strlen(string);
	for (uint8_t i = 0; i<m; i++)
		n += print(string[i]);
	return n;
}

size_t UART::println(const char string[])
{
	size_t n = print(string);
	n += print("\r\n");
	return n;
}

size_t UART::print(long data, integerBase base){
	size_t returned = 0;
	if(base == DEC){
		if (data<0){
			returned += print('-');
			data = -data;
			returned += print((unsigned long)data,base);
			return returned;
		}else{
			return print((unsigned long)data,base);
		}
	}else{
		return print((unsigned long)data,base);
	}
}

size_t UART::println(long data, integerBase base)
{
	size_t n = print(data,base);
	n += print("\r\n");
	return n;
}

size_t UART::print(unsigned long data, integerBase base){
	char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
	char *str = &buf[sizeof(buf) - 1];

	*str = '\0';
	do {
		char c = data % base;
		data /= base;
		*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while(data);

	return print(str);
}

size_t UART::println(unsigned long data, integerBase base)
{
	size_t n = print(data,base);
	n += print("\r\n");
	return n;
}

size_t UART::print(double number, int digits)
{
	size_t n = 0;

	if (isnan(number)){ return print("nan");}
	if (isinf(number)){ return print("inf");}
	if (number > 4294967040.0){ return print ("ovf");}  // constant determined empirically
	if (number <-4294967040.0){ return print ("ovf");}  // constant determined empirically

	// Handle negative numbers
	if (number < 0.0){
		n += print('-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;

	for (uint8_t i=0; i<digits; ++i){
		rounding /= 10.0;
	}

	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long)number;
	double remainder = number - (double)int_part;
	n += print(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
		n += print('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0){
		remainder *= 10.0;
		unsigned int toPrint = (unsigned int)(remainder);
		n += print(toPrint);
		remainder -= toPrint;
	}

	return n;
}

size_t UART::println(double number, int digits)
{
	size_t n = print(number,digits);
	n += print("\r\n");
	return n;
}

// -----------------
//   TRNG
// -----------------

ARMTZ_TRNG_TypeDef* trng = ARMTZ_TRNG;
int seed_index = 0;
int seed_sampling = 0xFF;

long random(long howbig)
{
	if (howbig == 0) {
		return 0;
	} else {
		trng->RND_SOURCE_ENABLE |= 0x00000001;
		trng->SAMPLE_CNT1 = seed_sampling;
		trng->TRNG_DEBUG_CONTROL |= 0xE; // set E to bypass all test
		while((trng->TRNG_VALID & 0x00000001) != 0x00000001){
			if(	(trng->RNG_ISR & 0x00000002) == 0x00000002 ||
				(trng->RNG_ISR & 0x00000004) == 0x00000004 ||
				(trng->RNG_ISR & 0x00000008) == 0x00000008){
				trng->TRNG_SW_RESET = 1;
				trng->RND_SOURCE_ENABLE |= 0x00000001;
				trng->SAMPLE_CNT1 = seed_sampling;
			}
		};
		long temp = (trng->EHR_DATA[seed_index]) % howbig;
		trng->RND_SOURCE_ENABLE &= ~0x00000001;
		return temp;
	}
}

long random_test()
{
	trng->RND_SOURCE_ENABLE |= 0x00000001;
	trng->SAMPLE_CNT1 = seed_sampling;
	trng->TRNG_DEBUG_CONTROL |= 0xE;
	while((trng->TRNG_VALID & 0x00000001) != 0x00000001){
		if(	(trng->RNG_ISR & 0x00000002) == 0x00000002 ||
			(trng->RNG_ISR & 0x00000004) == 0x00000004 ||
			(trng->RNG_ISR & 0x00000008) == 0x00000008){
			trng->TRNG_SW_RESET = 1;
			trng->RND_SOURCE_ENABLE |= 0x00000001;
			trng->SAMPLE_CNT1 = seed_sampling;
		}
	};
	long temp = (0x0ul|(trng->EHR_DATA[seed_index]));
	trng->RND_SOURCE_ENABLE &= ~0x00000001;
	return temp;
}

long random(long howsmall, long howbig)
{
	if (howsmall >= howbig) {
		return howsmall;
	} else {
		long diff = howbig - howsmall;
		long temp = random(diff) + howsmall;
		return temp;
	}
}

void randomSeed(unsigned long seed)
{
	seed_index = (int)(seed % 6);
	seed_sampling = (int)((seed % 0xDFF) + 0x1F);
}

// -----------------
//   GPIO
// -----------------

#ifdef XST_X1_01
void pinMode(uint8_t pin, gpioMode mode){
	CMSDK_GPIO_TypeDef* GPIO_dev;
	CMSDK_DUALTIMER_SINGLE_TypeDef* CMSDK_DUALTIMERx;

#ifdef XST_X1_01
	if(pin<16)      GPIO_dev = CMSDK_GPIO0;
	else            GPIO_dev = CMSDK_GPIO1;
#else
	if(pin<8)       GPIO_dev = CMSDK_GPIO0;
	else if(pin<16) GPIO_dev = CMSDK_GPIO1;
	else if(pin<24) GPIO_dev = CMSDK_GPIO2;
	else            GPIO_dev = CMSDK_GPIO3;
#endif
#ifdef XST_X1_01
	if(mode == INPUT) GPIO_dev->OUTENABLECLR = 1 << (pin & 0b1111);
	else              GPIO_dev->OUTENABLESET = 1 << (pin & 0b1111);
#else
	if(mode == INPUT) GPIO_dev->OUTENABLECLR = 1 << (pin & 0b0111);
	else GPIO_dev->OUTENABLESET = 1 << (pin & 0b0111);
#endif

	// PWM Settings
	if (pin == 4){
		GPIO_dev->ALTFUNCSET = 1 << (pin & 0b0111);
		CMSDK_DUALTIMERx = CMSDK_DUALTIMER1;

		/*Reset Timer Settings*/
		CMSDK_DUALTIMERx->TimerControl = TIMER_CTRL_DEFAULT |
										   (PRESCALE_1_1 & 0x3) << CMSDK_DUALTIMER_CTRL_PRESCALE_Pos |
										   CMSDK_DUALTIMER_CTRL_MODE_Msk;
		//PWM Enable
		if (mode != INPUT)
			CMSDK_DUALTIMERx->TimerControl |= 0x100; //enable pwm
		else
			CMSDK_DUALTIMERx->TimerControl &= ~0x100; //disable pwm

		CMSDK_DUALTIMERx->TimerLoad = PWM_16_MAX; //default load value

	}else if (pin == 5){
		GPIO_dev->ALTFUNCSET = 1 << (pin & 0b0111);
		CMSDK_DUALTIMERx = CMSDK_DUALTIMER2;

		/*Reset Timer Settings*/
		CMSDK_DUALTIMERx->TimerControl = TIMER_CTRL_DEFAULT |
										   (PRESCALE_1_1 & 0x3) << CMSDK_DUALTIMER_CTRL_PRESCALE_Pos |
										   CMSDK_DUALTIMER_CTRL_MODE_Msk;
		//PWM Enable
		if (mode != INPUT)
			CMSDK_DUALTIMERx->TimerControl |= 0x100; //enable pwm
		else
			CMSDK_DUALTIMERx->TimerControl &= ~0x100; //disable pwm

		CMSDK_DUALTIMERx->TimerLoad = PWM_16_MAX; //default load value

	}

}

gpioIntModeVal digitalRead (uint8_t pin){
	CMSDK_GPIO_TypeDef* GPIO_dev;

#ifdef XST_X1_01
	if(pin<16)      GPIO_dev = CMSDK_GPIO0;
	else            GPIO_dev = CMSDK_GPIO1;
#else
	if(pin<8)       GPIO_dev = CMSDK_GPIO0;
	else if(pin<16) GPIO_dev = CMSDK_GPIO1;
	else if(pin<24) GPIO_dev = CMSDK_GPIO2;
	else            GPIO_dev = CMSDK_GPIO3;
#endif

#ifdef XST_X1_01
	return ((GPIO_dev->DATA & 1<<(pin&0b1111))? HIGH : LOW);
#else
	return ((GPIO_dev->DATA & 1<<(pin&0b0111))? HIGH : LOW);
#endif
}

void digitalWrite(uint8_t pin, gpioIntModeVal val){
	CMSDK_GPIO_TypeDef* GPIO_dev;

#ifdef XST_X1_01
	if(pin<16)      GPIO_dev = CMSDK_GPIO0;
	else            GPIO_dev = CMSDK_GPIO1;
#else
	if(pin<8)       GPIO_dev = CMSDK_GPIO0;
	else if(pin<16) GPIO_dev = CMSDK_GPIO1;
	else if(pin<24) GPIO_dev = CMSDK_GPIO2;
	else            GPIO_dev = CMSDK_GPIO3;
#endif

#ifdef XST_X1_01
	pin %= 16;
	if(pin < 8) GPIO_dev->LB_MASKED[(1<< pin     )] = (val != LOW)<<pin;
	else        GPIO_dev->UB_MASKED[(1<<(pin % 8))] = (val != LOW)<<pin;
#else
	pin %= 8;
	GPIO_dev->LB_MASKED[(1<<pin)] = (val != LOW)<<pin;
#endif
}
#else
void pinMode (uint8_t pin, gpioMode mode){
	PinDescription pinDesc = boardPinTable[pin];
	uint32_t *modeSetReg;
	modeSetReg = (uint32_t*)pinDesc.pinPort + (mode << 2);
	*modeSetReg = pinDesc.pinMask;
}

gpioIntModeVal digitalRead (uint8_t pin){
	PinDescription pinDesc = boardPinTable[pin];
	uint32_t val = pinDesc.pinPort->DATA & pinDesc.pinMask;
	return ((gpioIntModeVal)(val > 0));
}

void digitalWrite(uint8_t pin, gpioIntModeVal val){
	PinDescription pinDesc = boardPinTable[pin];
	pinDesc.pinPort->LB_MASKED[pinDesc.pinMask] = (val == LOW) ? 0x00 : 0xFF;
}
#endif

// -------------------
//   GPIO Interrupts
// -------------------

// Interrupt Functions
voidFuncPtr gpioInterruptVect[] = {
	// 0
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	// 8
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
#ifndef XST_X1_01
	,
	// 16
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	// 24
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	// 32
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	// 40
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	// 48
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	// 56
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	// 64
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
#endif
};

void attachInterrupt(uint8_t pin, voidFuncPtr callback, gpioIntModeVal mode){
	IRQn_Type IRQn;
	CMSDK_GPIO_TypeDef* GPIO_dev;

#ifdef XST_X1_01
	if(pin<16){
		GPIO_dev = CMSDK_GPIO0;
		IRQn = (IRQn_Type)(pin+16);
		gpioInterruptVect[pin] = callback;
	}
	else{
		GPIO_dev = CMSDK_GPIO1;
		IRQn = PORT1_ALL_IRQn;
	}
	pin = pin & 0xF;

	switch(mode){
	case LOW:
		GPIO_dev->INTTYPECLR = 1<<pin;
		GPIO_dev->INTPOLCLR  = 1<<pin;
		break;
	case HIGH:
		GPIO_dev->INTTYPECLR = 1<<pin;
		GPIO_dev->INTPOLSET  = 1<<pin;
		break;
	case FALLING:
		GPIO_dev->INTTYPESET = 1<<pin;
		GPIO_dev->INTPOLCLR  = 1<<pin;
		break;
	default: // RISING
		GPIO_dev->INTTYPESET = 1<<pin;
		GPIO_dev->INTPOLSET  = 1<<pin;
		break;
	}
	GPIO_dev->INTENSET = 1 << pin;
	GPIO_dev->INTCLEAR = 1 << pin;
#else
	PinDescription thisPin = boardPinTable[pin];
	GPIO_dev = thisPin.pinPort;
	IRQn = thisPin.pinIRQn;
	gpioInterruptVect[pin] = callback;

	switch(mode){
	case LOW:
		GPIO_dev->INTTYPECLR = thisPin.pinMask;
		GPIO_dev->INTPOLCLR  = thisPin.pinMask;
		break;
	case HIGH:
		GPIO_dev->INTTYPECLR = thisPin.pinMask;
		GPIO_dev->INTPOLSET  = thisPin.pinMask;
		break;
	case FALLING:
		GPIO_dev->INTTYPESET = thisPin.pinMask;
		GPIO_dev->INTPOLCLR  = thisPin.pinMask;
		break;
	default: // RISING
		GPIO_dev->INTTYPESET = thisPin.pinMask;
		GPIO_dev->INTPOLSET  = thisPin.pinMask;
		break;
	}
	GPIO_dev->INTENSET = thisPin.pinMask;
	GPIO_dev->INTCLEAR = thisPin.pinMask;
#endif

	NVIC_ClearPendingIRQ(IRQn);
	NVIC_EnableIRQ(IRQn);
}

void detachInterrupt(uint8_t pin){
	IRQn_Type IRQn;
	CMSDK_GPIO_TypeDef* GPIO_dev;

#ifdef XST_X1_01
	if(pin<16){
		GPIO_dev = CMSDK_GPIO0;
		IRQn = (IRQn_Type)(pin+16);

		NVIC_DisableIRQ(IRQn);
		NVIC_ClearPendingIRQ(IRQn);
		gpioInterruptVect[pin] = NULL;
	}
	else GPIO_dev = CMSDK_GPIO1;

	GPIO_dev->INTENCLR = 1 << (pin & 0xF);

	if(pin>=16 && GPIO_dev->INTENSET == 0){
		NVIC_DisableIRQ     (PORT1_ALL_IRQn);
		NVIC_ClearPendingIRQ(PORT1_ALL_IRQn);
	}
#else
	PinDescription thisPin = boardPinTable[pin];
	GPIO_dev = thisPin.pinPort;
	IRQn = thisPin.pinIRQn;

	NVIC_DisableIRQ(IRQn);
	NVIC_ClearPendingIRQ(IRQn);
	GPIO_dev->INTENCLR = thisPin.pinMask;
	gpioInterruptVect[pin] = NULL;
#endif
}

extern "C" {
#ifdef XST_X1_01
void PORT0_0_Handler  (void){ CMSDK_GPIO0->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT0_0_IRQn ); if(gpioInterruptVect[ 0] != NULL) gpioInterruptVect[ 0]();  }
void PORT0_1_Handler  (void){ CMSDK_GPIO0->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT0_1_IRQn ); if(gpioInterruptVect[ 1] != NULL) gpioInterruptVect[ 1]();  }
void PORT0_2_Handler  (void){ CMSDK_GPIO0->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT0_2_IRQn ); if(gpioInterruptVect[ 2] != NULL) gpioInterruptVect[ 2]();  }
void PORT0_3_Handler  (void){ CMSDK_GPIO0->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT0_3_IRQn ); if(gpioInterruptVect[ 3] != NULL) gpioInterruptVect[ 3]();  }
void PORT0_4_Handler  (void){ CMSDK_GPIO0->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT0_4_IRQn ); if(gpioInterruptVect[ 4] != NULL) gpioInterruptVect[ 4]();  }
void PORT0_5_Handler  (void){ CMSDK_GPIO0->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT0_5_IRQn ); if(gpioInterruptVect[ 5] != NULL) gpioInterruptVect[ 5]();  }
void PORT0_6_Handler  (void){ CMSDK_GPIO0->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT0_6_IRQn ); if(gpioInterruptVect[ 6] != NULL) gpioInterruptVect[ 6]();  }
void PORT0_7_Handler  (void){ CMSDK_GPIO0->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT0_7_IRQn ); if(gpioInterruptVect[ 7] != NULL) gpioInterruptVect[ 7]();  }
void PORT0_8_Handler  (void){ CMSDK_GPIO0->INTCLEAR = 1<< 8; NVIC_ClearPendingIRQ(PORT0_8_IRQn ); if(gpioInterruptVect[ 8] != NULL) gpioInterruptVect[ 8](); }
void PORT0_9_Handler  (void){ CMSDK_GPIO0->INTCLEAR = 1<< 9; NVIC_ClearPendingIRQ(PORT0_9_IRQn ); if(gpioInterruptVect[ 9] != NULL) gpioInterruptVect[ 9](); }
void PORT0_10_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<<10; NVIC_ClearPendingIRQ(PORT0_10_IRQn); if(gpioInterruptVect[10] != NULL) gpioInterruptVect[10](); }
void PORT0_11_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<<11; NVIC_ClearPendingIRQ(PORT0_11_IRQn); if(gpioInterruptVect[11] != NULL) gpioInterruptVect[11](); }
void PORT0_12_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<<12; NVIC_ClearPendingIRQ(PORT0_12_IRQn); if(gpioInterruptVect[12] != NULL) gpioInterruptVect[12](); }
void PORT0_13_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<<13; NVIC_ClearPendingIRQ(PORT0_13_IRQn); if(gpioInterruptVect[13] != NULL) gpioInterruptVect[13](); }
void PORT0_14_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<<14; NVIC_ClearPendingIRQ(PORT0_14_IRQn); if(gpioInterruptVect[14] != NULL) gpioInterruptVect[14](); }
void PORT0_15_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<<15; NVIC_ClearPendingIRQ(PORT0_15_IRQn); if(gpioInterruptVect[15] != NULL) gpioInterruptVect[15](); }
#else
void __attribute__((section(".after_vectors"))) PORT0_0_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT0_0_IRQn ); if(gpioInterruptVect[54] != NULL) gpioInterruptVect[54]();  }
void __attribute__((section(".after_vectors"))) PORT0_1_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT0_1_IRQn ); if(gpioInterruptVect[55] != NULL) gpioInterruptVect[55]();  }
void __attribute__((section(".after_vectors"))) PORT0_2_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT0_2_IRQn ); if(gpioInterruptVect[56] != NULL) gpioInterruptVect[56]();  }
void __attribute__((section(".after_vectors"))) PORT0_3_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT0_3_IRQn ); if(gpioInterruptVect[57] != NULL) gpioInterruptVect[57]();  }
void __attribute__((section(".after_vectors"))) PORT0_4_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT0_4_IRQn ); if(gpioInterruptVect[58] != NULL) gpioInterruptVect[58]();  }
void __attribute__((section(".after_vectors"))) PORT0_5_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT0_5_IRQn ); if(gpioInterruptVect[59] != NULL) gpioInterruptVect[59]();  }
void __attribute__((section(".after_vectors"))) PORT0_6_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT0_6_IRQn ); if(gpioInterruptVect[60] != NULL) gpioInterruptVect[60]();  }
void __attribute__((section(".after_vectors"))) PORT0_7_Handler (void){ CMSDK_GPIO0->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT0_7_IRQn ); if(gpioInterruptVect[61] != NULL) gpioInterruptVect[61]();  }
void __attribute__((section(".after_vectors"))) PORT1_0_Handler (void){ CMSDK_GPIO1->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT1_0_IRQn ); if(gpioInterruptVect[62] != NULL) gpioInterruptVect[62](); }
void __attribute__((section(".after_vectors"))) PORT1_1_Handler (void){ CMSDK_GPIO1->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT1_1_IRQn ); if(gpioInterruptVect[63] != NULL) gpioInterruptVect[63](); }
void __attribute__((section(".after_vectors"))) PORT1_2_Handler (void){ CMSDK_GPIO1->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT1_2_IRQn ); if(gpioInterruptVect[64] != NULL) gpioInterruptVect[64](); }
void __attribute__((section(".after_vectors"))) PORT1_3_Handler (void){ CMSDK_GPIO1->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT1_3_IRQn ); if(gpioInterruptVect[65] != NULL) gpioInterruptVect[65](); }
void __attribute__((section(".after_vectors"))) PORT1_4_Handler (void){ CMSDK_GPIO1->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT1_4_IRQn ); if(gpioInterruptVect[66] != NULL) gpioInterruptVect[66](); }
void __attribute__((section(".after_vectors"))) PORT1_5_Handler (void){ CMSDK_GPIO1->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT1_5_IRQn ); if(gpioInterruptVect[67] != NULL) gpioInterruptVect[67](); }
void __attribute__((section(".after_vectors"))) PORT1_6_Handler (void){ CMSDK_GPIO1->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT1_6_IRQn ); /*if(gpioInterruptVect[  ] != NULL) gpioInterruptVect[  ]();*/ }
void __attribute__((section(".after_vectors"))) PORT1_7_Handler (void){ CMSDK_GPIO1->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT1_7_IRQn ); /*if(gpioInterruptVect[  ] != NULL) gpioInterruptVect[  ]();*/ }
void __attribute__((section(".after_vectors"))) PORT2_0_Handler (void){ CMSDK_GPIO2->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT2_0_IRQn ); if(gpioInterruptVect[38] != NULL) gpioInterruptVect[38](); }
void __attribute__((section(".after_vectors"))) PORT2_1_Handler (void){ CMSDK_GPIO2->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT2_1_IRQn ); if(gpioInterruptVect[39] != NULL) gpioInterruptVect[39](); }
void __attribute__((section(".after_vectors"))) PORT2_2_Handler (void){ CMSDK_GPIO2->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT2_2_IRQn ); if(gpioInterruptVect[40] != NULL) gpioInterruptVect[40](); }
void __attribute__((section(".after_vectors"))) PORT2_3_Handler (void){ CMSDK_GPIO2->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT2_3_IRQn ); if(gpioInterruptVect[41] != NULL) gpioInterruptVect[41](); }
void __attribute__((section(".after_vectors"))) PORT2_4_Handler (void){ CMSDK_GPIO2->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT2_4_IRQn ); if(gpioInterruptVect[42] != NULL) gpioInterruptVect[42](); }
void __attribute__((section(".after_vectors"))) PORT2_5_Handler (void){ CMSDK_GPIO2->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT2_5_IRQn ); if(gpioInterruptVect[43] != NULL) gpioInterruptVect[43](); }
void __attribute__((section(".after_vectors"))) PORT2_6_Handler (void){ CMSDK_GPIO2->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT2_6_IRQn ); if(gpioInterruptVect[44] != NULL) gpioInterruptVect[44](); }
void __attribute__((section(".after_vectors"))) PORT2_7_Handler (void){ CMSDK_GPIO2->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT2_7_IRQn ); if(gpioInterruptVect[45] != NULL) gpioInterruptVect[45](); }
void __attribute__((section(".after_vectors"))) PORT3_0_Handler (void){ CMSDK_GPIO3->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT3_0_IRQn ); if(gpioInterruptVect[48] != NULL) gpioInterruptVect[48](); }
void __attribute__((section(".after_vectors"))) PORT3_1_Handler (void){ CMSDK_GPIO3->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT3_1_IRQn ); if(gpioInterruptVect[49] != NULL) gpioInterruptVect[49](); }
void __attribute__((section(".after_vectors"))) PORT3_2_Handler (void){ CMSDK_GPIO3->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT3_2_IRQn ); if(gpioInterruptVect[50] != NULL) gpioInterruptVect[50](); }
void __attribute__((section(".after_vectors"))) PORT3_3_Handler (void){ CMSDK_GPIO3->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT3_3_IRQn ); if(gpioInterruptVect[51] != NULL) gpioInterruptVect[51](); }
void __attribute__((section(".after_vectors"))) PORT3_4_Handler (void){ CMSDK_GPIO3->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT3_4_IRQn ); if(gpioInterruptVect[52] != NULL) gpioInterruptVect[52](); }
void __attribute__((section(".after_vectors"))) PORT3_5_Handler (void){ CMSDK_GPIO3->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT3_5_IRQn ); if(gpioInterruptVect[68] != NULL) gpioInterruptVect[68](); }
void __attribute__((section(".after_vectors"))) PORT3_6_Handler (void){ CMSDK_GPIO3->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT3_6_IRQn ); if(gpioInterruptVect[69] != NULL) gpioInterruptVect[69](); }
void __attribute__((section(".after_vectors"))) PORT3_7_Handler (void){ CMSDK_GPIO3->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT3_7_IRQn ); if(gpioInterruptVect[53] != NULL) gpioInterruptVect[53](); }
void __attribute__((section(".after_vectors"))) PORT4_0_Handler (void){ CMSDK_GPIO4->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT4_0_IRQn ); /*if(gpioInterruptVect[  ] != NULL) gpioInterruptVect[  ]();*/ }
void __attribute__((section(".after_vectors"))) PORT4_1_Handler (void){ CMSDK_GPIO4->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT4_1_IRQn ); /*if(gpioInterruptVect[  ] != NULL) gpioInterruptVect[  ]();*/ }
void __attribute__((section(".after_vectors"))) PORT4_2_Handler (void){ CMSDK_GPIO4->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT4_2_IRQn ); /*if(gpioInterruptVect[  ] != NULL) gpioInterruptVect[  ]();*/ }
void __attribute__((section(".after_vectors"))) PORT4_3_Handler (void){ CMSDK_GPIO4->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT4_3_IRQn ); if(gpioInterruptVect[47] != NULL) gpioInterruptVect[47](); }
void __attribute__((section(".after_vectors"))) PORT4_4_Handler (void){ CMSDK_GPIO4->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT4_4_IRQn ); if(gpioInterruptVect[46] != NULL) gpioInterruptVect[46](); }
void __attribute__((section(".after_vectors"))) PORT4_5_Handler (void){ CMSDK_GPIO4->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT4_5_IRQn ); /*if(gpioInterruptVect[  ] != NULL) gpioInterruptVect[  ]();*/ }
void __attribute__((section(".after_vectors"))) PORT4_6_Handler (void){ CMSDK_GPIO4->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT4_6_IRQn ); /*if(gpioInterruptVect[  ] != NULL) gpioInterruptVect[  ]();*/ }
void __attribute__((section(".after_vectors"))) PORT4_7_Handler (void){ CMSDK_GPIO4->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT4_7_IRQn ); /*if(gpioInterruptVect[  ] != NULL) gpioInterruptVect[  ]();*/ }
void __attribute__((section(".after_vectors"))) PORT5_0_Handler (void){ CMSDK_GPIO5->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT5_0_IRQn ); if(gpioInterruptVect[37] != NULL) gpioInterruptVect[37](); }
void __attribute__((section(".after_vectors"))) PORT5_1_Handler (void){ CMSDK_GPIO5->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT5_1_IRQn ); if(gpioInterruptVect[35] != NULL) gpioInterruptVect[35](); }
void __attribute__((section(".after_vectors"))) PORT5_2_Handler (void){ CMSDK_GPIO5->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT5_2_IRQn ); if(gpioInterruptVect[33] != NULL) gpioInterruptVect[33](); }
void __attribute__((section(".after_vectors"))) PORT5_3_Handler (void){ CMSDK_GPIO5->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT5_3_IRQn ); if(gpioInterruptVect[31] != NULL) gpioInterruptVect[31](); }
void __attribute__((section(".after_vectors"))) PORT5_4_Handler (void){ CMSDK_GPIO5->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT5_4_IRQn ); if(gpioInterruptVect[29] != NULL) gpioInterruptVect[29](); }
void __attribute__((section(".after_vectors"))) PORT5_5_Handler (void){ CMSDK_GPIO5->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT5_5_IRQn ); if(gpioInterruptVect[27] != NULL) gpioInterruptVect[27](); }
void __attribute__((section(".after_vectors"))) PORT5_6_Handler (void){ CMSDK_GPIO5->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT5_6_IRQn ); if(gpioInterruptVect[25] != NULL) gpioInterruptVect[25](); }
void __attribute__((section(".after_vectors"))) PORT5_7_Handler (void){ CMSDK_GPIO5->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT5_7_IRQn ); if(gpioInterruptVect[23] != NULL) gpioInterruptVect[23](); }
void __attribute__((section(".after_vectors"))) PORT6_0_Handler (void){ CMSDK_GPIO6->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT6_0_IRQn ); if(gpioInterruptVect[36] != NULL) gpioInterruptVect[36](); }
void __attribute__((section(".after_vectors"))) PORT6_1_Handler (void){ CMSDK_GPIO6->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT6_1_IRQn ); if(gpioInterruptVect[34] != NULL) gpioInterruptVect[34](); }
void __attribute__((section(".after_vectors"))) PORT6_2_Handler (void){ CMSDK_GPIO6->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT6_2_IRQn ); if(gpioInterruptVect[32] != NULL) gpioInterruptVect[32](); }
void __attribute__((section(".after_vectors"))) PORT6_3_Handler (void){ CMSDK_GPIO6->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT6_3_IRQn ); if(gpioInterruptVect[30] != NULL) gpioInterruptVect[30](); }
void __attribute__((section(".after_vectors"))) PORT6_4_Handler (void){ CMSDK_GPIO6->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT6_4_IRQn ); if(gpioInterruptVect[28] != NULL) gpioInterruptVect[28](); }
void __attribute__((section(".after_vectors"))) PORT6_5_Handler (void){ CMSDK_GPIO6->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT6_5_IRQn ); if(gpioInterruptVect[26] != NULL) gpioInterruptVect[26](); }
void __attribute__((section(".after_vectors"))) PORT6_6_Handler (void){ CMSDK_GPIO6->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT6_6_IRQn ); if(gpioInterruptVect[24] != NULL) gpioInterruptVect[24](); }
void __attribute__((section(".after_vectors"))) PORT6_7_Handler (void){ CMSDK_GPIO6->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT6_7_IRQn ); if(gpioInterruptVect[22] != NULL) gpioInterruptVect[22](); }
void __attribute__((section(".after_vectors"))) PORT7_0_Handler (void){ CMSDK_GPIO7->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT7_0_IRQn ); if(gpioInterruptVect[21] != NULL) gpioInterruptVect[21](); }
void __attribute__((section(".after_vectors"))) PORT7_1_Handler (void){ CMSDK_GPIO7->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT7_1_IRQn ); if(gpioInterruptVect[20] != NULL) gpioInterruptVect[20](); }
void __attribute__((section(".after_vectors"))) PORT7_2_Handler (void){ CMSDK_GPIO7->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT7_2_IRQn ); if(gpioInterruptVect[19] != NULL) gpioInterruptVect[19](); }
void __attribute__((section(".after_vectors"))) PORT7_3_Handler (void){ CMSDK_GPIO7->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT7_3_IRQn ); if(gpioInterruptVect[18] != NULL) gpioInterruptVect[18](); }
void __attribute__((section(".after_vectors"))) PORT7_4_Handler (void){ CMSDK_GPIO7->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT7_4_IRQn ); if(gpioInterruptVect[17] != NULL) gpioInterruptVect[17](); }
void __attribute__((section(".after_vectors"))) PORT7_5_Handler (void){ CMSDK_GPIO7->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT7_5_IRQn ); if(gpioInterruptVect[16] != NULL) gpioInterruptVect[16](); }
void __attribute__((section(".after_vectors"))) PORT7_6_Handler (void){ CMSDK_GPIO7->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT7_6_IRQn ); if(gpioInterruptVect[15] != NULL) gpioInterruptVect[15](); }
void __attribute__((section(".after_vectors"))) PORT7_7_Handler (void){ CMSDK_GPIO7->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT7_7_IRQn ); if(gpioInterruptVect[14] != NULL) gpioInterruptVect[14](); }
void __attribute__((section(".after_vectors"))) PORT8_0_Handler (void){ CMSDK_GPIO8->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT8_0_IRQn ); if(gpioInterruptVect[ 0] != NULL) gpioInterruptVect[ 0](); }
void __attribute__((section(".after_vectors"))) PORT8_1_Handler (void){ CMSDK_GPIO8->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT8_1_IRQn ); if(gpioInterruptVect[ 1] != NULL) gpioInterruptVect[ 1](); }
void __attribute__((section(".after_vectors"))) PORT8_2_Handler (void){ CMSDK_GPIO8->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT8_2_IRQn ); if(gpioInterruptVect[ 2] != NULL) gpioInterruptVect[ 2](); }
void __attribute__((section(".after_vectors"))) PORT8_3_Handler (void){ CMSDK_GPIO8->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT8_3_IRQn ); if(gpioInterruptVect[ 3] != NULL) gpioInterruptVect[ 3](); }
void __attribute__((section(".after_vectors"))) PORT8_4_Handler (void){ CMSDK_GPIO8->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT8_4_IRQn ); if(gpioInterruptVect[ 4] != NULL) gpioInterruptVect[ 4](); }
void __attribute__((section(".after_vectors"))) PORT8_5_Handler (void){ CMSDK_GPIO8->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT8_5_IRQn ); if(gpioInterruptVect[ 5] != NULL) gpioInterruptVect[ 5](); }
void __attribute__((section(".after_vectors"))) PORT8_6_Handler (void){ CMSDK_GPIO8->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT8_6_IRQn ); if(gpioInterruptVect[ 6] != NULL) gpioInterruptVect[ 6](); }
void __attribute__((section(".after_vectors"))) PORT8_7_Handler (void){ CMSDK_GPIO8->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT8_7_IRQn ); if(gpioInterruptVect[ 7] != NULL) gpioInterruptVect[ 7](); }
void __attribute__((section(".after_vectors"))) PORT9_0_Handler (void){ CMSDK_GPIO9->INTCLEAR = 1<< 0; NVIC_ClearPendingIRQ(PORT9_0_IRQn ); if(gpioInterruptVect[ 8] != NULL) gpioInterruptVect[ 8](); }
void __attribute__((section(".after_vectors"))) PORT9_1_Handler (void){ CMSDK_GPIO9->INTCLEAR = 1<< 1; NVIC_ClearPendingIRQ(PORT9_1_IRQn ); if(gpioInterruptVect[ 9] != NULL) gpioInterruptVect[ 9](); }
void __attribute__((section(".after_vectors"))) PORT9_2_Handler (void){ CMSDK_GPIO9->INTCLEAR = 1<< 2; NVIC_ClearPendingIRQ(PORT9_2_IRQn ); if(gpioInterruptVect[10] != NULL) gpioInterruptVect[10](); }
void __attribute__((section(".after_vectors"))) PORT9_3_Handler (void){ CMSDK_GPIO9->INTCLEAR = 1<< 3; NVIC_ClearPendingIRQ(PORT9_3_IRQn ); if(gpioInterruptVect[11] != NULL) gpioInterruptVect[11](); }
void __attribute__((section(".after_vectors"))) PORT9_4_Handler (void){ CMSDK_GPIO9->INTCLEAR = 1<< 4; NVIC_ClearPendingIRQ(PORT9_4_IRQn ); if(gpioInterruptVect[12] != NULL) gpioInterruptVect[12](); }
void __attribute__((section(".after_vectors"))) PORT9_5_Handler (void){ CMSDK_GPIO9->INTCLEAR = 1<< 5; NVIC_ClearPendingIRQ(PORT9_5_IRQn ); if(gpioInterruptVect[13] != NULL) gpioInterruptVect[13](); }
void __attribute__((section(".after_vectors"))) PORT9_6_Handler (void){ CMSDK_GPIO9->INTCLEAR = 1<< 6; NVIC_ClearPendingIRQ(PORT9_6_IRQn ); if(gpioInterruptVect[70] != NULL) gpioInterruptVect[70](); }
void __attribute__((section(".after_vectors"))) PORT9_7_Handler (void){ CMSDK_GPIO9->INTCLEAR = 1<< 7; NVIC_ClearPendingIRQ(PORT9_7_IRQn ); if(gpioInterruptVect[71] != NULL) gpioInterruptVect[71](); }
#endif
}

// -------------------
//   Analog I/O
// -------------------
void analogWrite(uint8_t pin, int val)
{
	if (val<0){
		val = 0;
	}else if (val>PWM_16_MAX){
		val = PWM_16_MAX;
	}

	if (pin==4 || pin ==5){
		pwmWrite((uint32_t)pin,(uint32_t)val);
	}
}

// PWM API
void pwmWrite(uint32_t pin, uint32_t on_cycle)
{
	CMSDK_DUALTIMER_SINGLE_TypeDef* CMSDK_DUALTIMERx;
	if (pin == 4)
	{
		CMSDK_DUALTIMERx = CMSDK_DUALTIMER1;
		CMSDK_DUALTIMERx->TimerComp = CMSDK_DUALTIMERx->TimerLoad - on_cycle; //dibalik karena down counting
		CMSDK_DUALTIMERx->TimerControl |= CMSDK_DUALTIMER_CTRL_EN_Msk; //enable PWM and timer
	}
	else if (pin == 5)
	{
		CMSDK_DUALTIMERx = CMSDK_DUALTIMER2;
		CMSDK_DUALTIMERx->TimerComp = CMSDK_DUALTIMERx->TimerLoad - on_cycle; //dibalik karena down counting
		CMSDK_DUALTIMERx->TimerControl |= CMSDK_DUALTIMER_CTRL_EN_Msk; //enable PWM and timer
	}
}

void pwmChangeLoad(uint32_t pin, uint32_t load)
{
	CMSDK_DUALTIMER_SINGLE_TypeDef* CMSDK_DUALTIMERx;
	if (pin == 4){
		CMSDK_DUALTIMERx = CMSDK_DUALTIMER1;
		CMSDK_DUALTIMERx->TimerLoad = load;
	}else if (pin == 5){
		CMSDK_DUALTIMERx = CMSDK_DUALTIMER2;
		CMSDK_DUALTIMERx->TimerLoad = load;
	}
}

uint32_t pwmDutyCalc(uint32_t pin, double percent) {
	CMSDK_DUALTIMER_SINGLE_TypeDef* CMSDK_DUALTIMERx;

	//limit percentage value to avoid error
	if (percent >= 100)
	{
		percent = 100;
	}
	else if (percent <= 0)
	{
		percent = 0;
	}

	if (pin == 4){
		CMSDK_DUALTIMERx = CMSDK_DUALTIMER1;
		return (uint32_t) (CMSDK_DUALTIMERx->TimerLoad * percent / 100);
	}else if (pin == 5){
		CMSDK_DUALTIMERx = CMSDK_DUALTIMER2;
		return (uint32_t) (CMSDK_DUALTIMERx->TimerLoad * percent / 100);
	}

	return 0;
}

void pwmChangeDiv(uint32_t pin, uint32_t prescaler)
{
	CMSDK_DUALTIMER_SINGLE_TypeDef* CMSDK_DUALTIMERx;
	if (pin == 4){
		CMSDK_DUALTIMERx = CMSDK_DUALTIMER1;
		CMSDK_DUALTIMERx->TimerControl |= (prescaler & 0x3) << CMSDK_DUALTIMER_CTRL_PRESCALE_Pos;
	}else if (pin == 5){
		CMSDK_DUALTIMERx = CMSDK_DUALTIMER2;
		CMSDK_DUALTIMERx->TimerControl |= (prescaler & 0x3) << CMSDK_DUALTIMER_CTRL_PRESCALE_Pos;
	}
}

// -------------------
//   Advanced I/O
// -------------------
bool update_tone = true;
// frequency (in hertz) and duration (in milliseconds).
// max freq is 10 MHz, min freq is 305 Hz
void tone(uint8_t _pin, unsigned int frequency)
{
	if (_pin==4 || _pin==5){
		pinMode(_pin,OUTPUT);
		if (update_tone){
			if (frequency > 10000000){
				frequency = 10000000;
			}else if (frequency < 305){
				frequency = 305;
			}
			int load = FOSC/frequency;
			pwmChangeLoad(_pin,load);
			int comp = load/2;
			pwmWrite(_pin, comp);
		}
	}
}

void noTone(uint8_t _pin)
{
	CMSDK_DUALTIMER_SINGLE_TypeDef* CMSDK_DUALTIMERx;
	if (_pin==4 || _pin==5) {
		if (_pin==4) {
			CMSDK_DUALTIMERx = CMSDK_DUALTIMER1;
		}else if (_pin==5){
			CMSDK_DUALTIMERx = CMSDK_DUALTIMER2;
		}
		CMSDK_DUALTIMERx->TimerControl = TIMER_CTRL_DEFAULT;
		pwmWrite(_pin,0);
		update_tone = true;
	}
}
