
#include <string.h>
#include "Wire.h"

TwoWire Wire;

TwoWire::TwoWire(void){
	clockIsSet     = false;
	address        = 0;
	rxBufferIndex  = 0;
	rxBufferLength = 0;
	txBufferLength = 0;
}

volatile bool I2CInterrupt;

void TwoWire::setClock(uint32_t clockFreq){
	I2C0->PRER = FOSC / (5*clockFreq) - 1;
	clockIsSet = true;
}

void TwoWire::begin(void){
#ifdef XST_X1_01
	CMSDK_GPIO1->ALTFUNCSET = 0b11 << 6;
#else
	CMSDK_GPIO7->ALTFUNCSET = 0b11 << 0;
#endif

	if(!clockIsSet){
		I2C0->PRER = FOSC / (5*100000UL) - 1;
		clockIsSet = true;
	}

	I2C0->CR  = I2C_CR_IACK_Msk;
	I2CInterrupt = false;
	NVIC_ClearPendingIRQ(I2C_IRQn);
	NVIC_EnableIRQ(I2C_IRQn);

	I2C0->CTR = I2C_CTR_EN_Msk | I2C_CTR_IEN_Msk;
}

extern "C" void I2C_Handler(void){
	I2C0->CR  = I2C_CR_IACK_Msk;
	NVIC_ClearPendingIRQ(I2C_IRQn);
	I2CInterrupt = true;
}

void TwoWire::beginTransmission(uint8_t _address){
	address = _address;
	txBufferLength = 0;
}

uint8_t TwoWire::write(uint8_t data){
	if(txBufferLength < BUFFER_LENGTH){
		txBuffer[txBufferLength++] = data;
		return 1;
	}
	else
		return 0;
}

uint8_t TwoWire::write(const uint8_t data[], uint8_t size){
	uint8_t returned = 0;
	for(uint8_t i=0; i<size;i++)
		returned += write(data[i]);
	return returned;
}

uint8_t TwoWire::write(const char data[]){
	uint8_t returned = 0;
	for(uint8_t i=0; i<strlen(data);i++)
		returned += write((uint8_t)data[i]);
	return returned;
}

uint8_t TwoWire::endTransmission(bool stop){
	uint8_t returnVal = 0;
	begin();
	while(I2C0->SR & I2C_SR_Busy_Msk);
	if(sendByte(address << 1,true,false) == 0){
		for(uint8_t i=0;i<(txBufferLength-1);i++){
			if(sendByte(txBuffer[i],false,false) != 0) returnVal = 3;
		}
		if(sendByte(txBuffer[(txBufferLength-1)],false,stop) != 0) returnVal = 3;
	}
	else {
		returnVal = 2;
	}
	if(returnVal != 0) sendByte(0xFF,false,true);
	return returnVal;
}

uint8_t TwoWire::sendByte(uint8_t data, bool start, bool stop){
	I2C0->TXR = data;
	I2C0->CR = (start ? I2C_CR_STA_Msk : 0) | I2C_CR_WR_Msk | (stop ? I2C_CR_STO_Msk : 0);
	//while(I2C0->SR & I2C_SR_TIP_Msk);
	while(!I2CInterrupt);
	I2CInterrupt = false;
	return ((I2C0->SR & I2C_SR_RxACK_Msk) ? 1 : 0);
}

uint8_t TwoWire::requestFrom(uint8_t _address, uint8_t dataNum, bool stop){
	uint8_t readCount = 0;
	while(I2C0->SR & I2C_SR_Busy_Msk);
	if(sendByte((_address << 1 | 1),true,false) == 0){
		for(uint8_t i=0; i<(dataNum-1); i++){
			receiveByte(true,false);
			readCount++;
		}
		receiveByte(false,stop);
		readCount++;
	} else
		sendByte(0xFF,false,true);
	return readCount;
}

void TwoWire::receiveByte(bool ack, bool stop){
	I2C0->CR = (stop ? I2C_CR_STO_Msk : 0) | I2C_CR_RD_Msk | (ack ? 0 : I2C_CR_ACK_Msk);
	//while(I2C0->SR & I2C_SR_TIP_Msk);
	while(!I2CInterrupt);
	I2CInterrupt = false;
	rxBuffer[((rxBufferLength+rxBufferIndex)%32)] = I2C0->RXR;
	if(rxBufferLength < BUFFER_LENGTH) rxBufferLength++;
}

uint8_t TwoWire::read(void){
	uint8_t data = rxBuffer[(rxBufferIndex++)];
	rxBufferIndex %= 32;
	rxBufferLength--;
	return data;
}
