#include <Arduino.h>
#include <SPI.h>
#include <stdio.h>

#define SerialDev Serial

#define slave0Pin 10
#define slave1Pin 1
#define sckPin    (16+10)
#define SPIspeed  100000UL
#define SPImode   SPI_MODE3

void SPI_test(uint8_t slavePin, uint8_t dataOut, dataMode mode);

void setup(void){
	pinMode(slave0Pin,OUTPUT);
	pinMode(slave1Pin,OUTPUT);
	//pinMode(sckPin   ,OUTPUT);
	digitalWrite(slave0Pin,HIGH);
	digitalWrite(slave1Pin,HIGH);
	//digitalWrite(sckPin   ,HIGH);
	
	SerialDev.begin(115200);
	//CMSDK_UART0->CTRL |= CMSDK_UART_CTRL_HSTM_Msk;
	SerialDev.println("Multi-slave SPI test begin!");
}

void loop(void){
	static uint8_t dataOut = 130;

	SerialDev.print((unsigned char) dataOut);
	SerialDev.print(' ');
	SerialDev.print((unsigned char) ~dataOut);
	SerialDev.println();
	
	// Slave 0
	//digitalWrite(slave0Pin,LOW);
	SPI_test(slave0Pin,dataOut,SPImode);
	//digitalWrite(slave0Pin,HIGH);

	//delay(100);
	
	// Slave 1
	//digitalWrite(slave1Pin,LOW);
	//SPI_test(slave1Pin,~dataOut,SPImode);
	//digitalWrite(slave1Pin,HIGH);

	delay(100);
	SerialDev.println();
	dataOut++;

}

void SPI_test(uint8_t slavePin, uint8_t dataOut, dataMode mode){
	uint8_t dataIn1,dataIn2;

	SPI.beginTransaction(SPISettings(SPIspeed,MSBFIRST,mode));
	digitalWrite(slavePin,LOW);
	delay(1);
	SPI.transfer(dataOut);
	delay(1);
	dataIn1 = SPI.transfer(~dataOut);
	delay(1);
	dataIn2 = SPI.transfer(0x00);
	delay(1);
	digitalWrite(slavePin,HIGH);
	SPI.endTransaction();
	
	SerialDev.print((unsigned char) dataIn1);
	SerialDev.print(' ');
	SerialDev.print((unsigned char) dataIn2);
	SerialDev.println();
}
