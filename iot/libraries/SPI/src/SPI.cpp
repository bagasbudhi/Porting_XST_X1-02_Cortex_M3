
#include "SPI.h"

#ifndef FOSC
#define FOSC 20000000UL
#endif

SPIMaster SPI;
volatile bool SPIInterrupt;

SPISettings::SPISettings(uint32_t maxSpeed, dataOrder _order, dataMode _mode){
	divider = (maxSpeed >= FOSC/2   ) ? SPI_CLOCK_DIV2    :
			  (maxSpeed >= FOSC/4   ) ? SPI_CLOCK_DIV4    :
			  (maxSpeed >= FOSC/8   ) ? SPI_CLOCK_DIV8    :
			  (maxSpeed >= FOSC/16  ) ? SPI_CLOCK_DIV16   :
	          (maxSpeed >= FOSC/32  ) ? SPI_CLOCK_DIV32   :
			  (maxSpeed >= FOSC/64  ) ? SPI_CLOCK_DIV64   :
			  (maxSpeed >= FOSC/128 ) ? SPI_CLOCK_DIV128  :
			  (maxSpeed >= FOSC/256 ) ? SPI_CLOCK_DIV256  :
			  (maxSpeed >= FOSC/512 ) ? SPI_CLOCK_DIV512  :
			  (maxSpeed >= FOSC/1024) ? SPI_CLOCK_DIV1024 :
			  (maxSpeed >= FOSC/2048) ? SPI_CLOCK_DIV2048 :
			                            SPI_CLOCK_DIV4096 ;
	order   = _order;
	mode    = _mode;
}

void SPIMaster::beginTransaction(SPISettings settings){
	SPIInterrupt = false;
    
    XSTIOT_CLKCON |= (1UL<<12);
	XSTIOT_SPI->SPSR = 0xFF;
	NVIC_ClearPendingIRQ(SPI_IRQn);
	NVIC_EnableIRQ(SPI_IRQn);

	XSTIOT_SPI->SPCR = XSTIOT_SPI_SPIE_Msk | XSTIOT_SPI_SPE_Msk | XSTIOT_SPI_MSTR_Msk
						| ((uint8_t) settings.mode << XSTIOT_SPI_CPHA_Pos)
						| ((uint8_t) settings.divider & 0b11);
	XSTIOT_SPI->SPER = ((uint8_t) settings.divider >> 2  );

#ifdef XST_X1_01
	CMSDK_GPIO1->ALTFUNCSET = 0b111 << 8;
	CMSDK_GPIO1->OUTENABLESET = 0b11 << 9;
	CMSDK_GPIO1->OUTENABLECLR = 1 << 8;
	if(settings.mode < SPI_MODE2) CMSDK_GPIO1->UB_MASKED[(1<<(10-8))] = 0;
	else                          CMSDK_GPIO1->UB_MASKED[(1<<(10-8))] = (1<<10);
#else
	CMSDK_GPIO9->ALTFUNCSET = 0b111 << 3;
	CMSDK_GPIO9->OUTENABLESET = 0b101 << 3;
	CMSDK_GPIO9->OUTENABLECLR = 1 << 4;
	if(settings.mode < SPI_MODE2) CMSDK_GPIO9->LB_MASKED[(1<<5)] = 0;
	else                          CMSDK_GPIO9->LB_MASKED[(1<<5)] = 0xFF;
#endif
}

/*void SPIMaster::begin(void){
	XSTIOT_SPI->SPCR |= XSTIOT_SPI_SPE_Msk | XSTIOT_SPI_MSTR_Msk;
#ifdef XST_X1_01
	CMSDK_GPIO1->ALTFUNCSET = 0b111 << 8;
	CMSDK_GPIO1->OUTENABLESET = 0b11 << 9;
	CMSDK_GPIO1->OUTENABLECLR = 1 << 8;
	if(settings.mode < SPI_MODE2) CMSDK_GPIO1->UB_MASKED[(1<<(10-8))] = 0;
	else                          CMSDK_GPIO1->UB_MASKED[(1<<(10-8))] = (1<<10);
#else
	CMSDK_GPIO9->ALTFUNCSET = 0b111 << 3;
	CMSDK_GPIO9->OUTENABLESET = 0b101 << 3;
	CMSDK_GPIO9->OUTENABLECLR = 1 << 4;
	if(settings.mode < SPI_MODE2) CMSDK_GPIO9->LB_MASKED[(1<<5)] = 0;
	else                          CMSDK_GPIO9->LB_MASKED[(1<<5)] = 0xFF;
#endif
}*/

void SPIMaster::end(void){
	//while(!(XSTIOT_SPI->SPSR & XSTIOT_SPI_WFEMPTY_Msk));
	while(!(XSTIOT_SPI->SPSR & XSTIOT_SPI_RFEMPTY_Msk))
		volatile uint8_t data __attribute__((unused)) = XSTIOT_SPI->DIN;

	XSTIOT_SPI->SPCR &= ~XSTIOT_SPI_SPE_Msk;
#ifdef XST_X1_01
	CMSDK_GPIO1->ALTFUNCCLR = 0b111 << 8;
#else
	CMSDK_GPIO9->ALTFUNCCLR = 0b111 << 3;
#endif
}

extern "C" void SPI_Handler(void){
	SPIInterrupt = true;
	NVIC_ClearPendingIRQ(SPI_IRQn);
	XSTIOT_SPI->SPSR = 0xFF;
}

uint8_t SPIMaster::transfer(uint8_t val){
	uint8_t data __attribute__((unused));

	while(!(XSTIOT_SPI->SPSR & XSTIOT_SPI_RFEMPTY_Msk))
		data = XSTIOT_SPI->DIN;

	XSTIOT_SPI->SPER  &= ~(0b11 << XSTIOT_SPI_ICNT_Pos);
	XSTIOT_SPI->SPSR   = XSTIOT_SPI_SPIF_Msk;
	XSTIOT_SPI->RFDOUT = val;
	//while ((XSTIOT_SPI->SPSR & (XSTIOT_SPI_SPIF_Msk | XSTIOT_SPI_WFEMPTY_Msk)) != (XSTIOT_SPI_SPIF_Msk | XSTIOT_SPI_WFEMPTY_Msk));
	while(XSTIOT_SPI->SPSR & XSTIOT_SPI_RFEMPTY_Msk);
	//while(!SPIInterrupt);
	SPIInterrupt = false;
	XSTIOT_SPI->SPSR   = XSTIOT_SPI_SPIF_Msk;

	return (XSTIOT_SPI->DIN);
}

uint16_t SPIMaster::transfer16(uint16_t val){
	uint16_t data /*__attribute__((unused))*/;

	while(!(XSTIOT_SPI->SPSR & XSTIOT_SPI_RFEMPTY_Msk))
		data = XSTIOT_SPI->DIN;

	XSTIOT_SPI->SPER   = (XSTIOT_SPI->SPER & ~(0b11 << XSTIOT_SPI_ICNT_Pos)) | (0b01 << XSTIOT_SPI_ICNT_Pos);
	XSTIOT_SPI->RFDOUT = (uint8_t)(val >> 8);
	XSTIOT_SPI->RFDOUT = (uint8_t) val;
	XSTIOT_SPI->SPSR   = XSTIOT_SPI_SPIF_Msk;
	//while (!(XSTIOT_SPI->SPSR & XSTIOT_SPI_SPIF_Msk));
	while(!SPIInterrupt);
	SPIInterrupt = false;
	XSTIOT_SPI->SPSR   = XSTIOT_SPI_SPIF_Msk;

	data = (uint16_t)XSTIOT_SPI->DIN << 8;
	return (data | XSTIOT_SPI->DIN);
}

void SPIMaster::transfer(const uint8_t val[], uint8_t size){
	for(uint8_t i=0;i<size;i++)
		transfer(val[i]);
}
