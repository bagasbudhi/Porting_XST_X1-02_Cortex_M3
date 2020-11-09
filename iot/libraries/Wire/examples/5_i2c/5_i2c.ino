
#include <Arduino.h>
#include <Wire.h>

#define slaveAddress 2

void setup(void){
	Wire.setClock(100000);
	Wire.begin();
	Serial2.begin(115200);
	Serial2.println("Hello!");
}

void loop(void){
	static uint8_t x = 0;
	//char str[5];
	
	Wire.beginTransmission(slaveAddress);
	Wire.write("x is ");
	Wire.write(x);
	uint8_t returnVal = Wire.endTransmission();
	
	//sprintf(str,"%3d ",x);
	Serial2.print(x);
	Serial2.print(" [");
	Serial2.print((unsigned char)returnVal);
	Serial2.print("] ");
	x++;
	//for(volatile uint32_t i=0;i<0x70000;i++);
	delay(500);
	
	Wire.requestFrom(slaveAddress,1);
	while(Wire.available()){
		uint8_t c = Wire.read();
		//sprintf(str,"%3d",c);
		Serial2.print(c);
	}
	Serial2.println();
	//for(volatile uint32_t i=0;i<0x70000;i++);
	delay(500);
}