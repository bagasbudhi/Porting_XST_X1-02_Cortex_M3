
#include <Arduino.h>

volatile unsigned int buf;
volatile bool dataAcquired;

void setup(void){
	Serial2.begin(115200);
	Serial2.println("Hello!");
	
	dataAcquired = false;
	CMSDK_GPIO3->ALTFUNCSET = 0b1111 << 0;
	XSTIOT_SPI->SPSR = 0xFF;
	NVIC_ClearPendingIRQ(SPI_IRQn);
	NVIC_EnableIRQ(SPI_IRQn);
	XSTIOT_SPI->SPCR = XSTIOT_SPI_SPIE_Msk
					 | XSTIOT_SPI_SPE_Msk;
	//XSTIOT_SPI->DIN = 0x00;
}

void loop(void){
	static uint8_t cnt = 0;
	
	//if(!(XSTIOT_SPI->SPSR & XSTIOT_SPI_RFEMPTY_Msk))
	//	dataAcquired = true;
	
	if(dataAcquired){
		cnt++;
		//XSTIOT_SPI->SPSR = 0xFF;
		//uint8_t rcv = XSTIOT_SPI->RFDOUT;
		//buf = rcv;
		//XSTIOT_SPI->DIN = rcv;
		Serial2.print(buf);
		Serial2.print(' ');
		if(cnt>=2){
			Serial2.println();
			cnt = 0;
		}
		dataAcquired = false;
	}
}

extern "C" __attribute__ ((long_call, section(".data.data_begin"))) void SPI_Handler(void){
	XSTIOT_SPI->SPSR = 0xFF;
	NVIC_ClearPendingIRQ(SPI_IRQn);
	uint8_t rcv = XSTIOT_SPI->RFDOUT;
	XSTIOT_SPI->DIN = rcv;
	buf = rcv;
	dataAcquired = true;
}
