
#include <Arduino.h>
#include <DES.h>

void setup(void){
	Serial2.begin(115200);
	Serial2.println("Hello!");
}

void loop(void){
	static uint8_t x = 0;
	char str[3];
	uint8_t key[8];
	uint8_t dataIn[8];
	uint8_t dataEnc[8];
	uint8_t dataDec[8];
	
	Serial2.println("Start!");
	
	for(uint8_t i=0; i<8; i++){
		key[i] = x+i;
		sprintf(str,"%02X ",key[i]);
		Serial2.print(str);
	}
	Serial2.println(" ");
	
	for(uint8_t i=0; i<8; i++){
		dataIn[i] = 255-x-i;
		sprintf(str,"%02X ",dataIn[i]);
		Serial2.print(str);
	}
	Serial2.println(" ");
	
	DES.begin(false);
	DES.insertKey(key);
	DES.insertData(dataIn);
	DES.processData();
	
	for(uint8_t i=0; i<8; i++){
		dataEnc[i] = DES.getProcessedData(i);
		sprintf(str,"%02X ",dataEnc[i]);
		Serial2.print(str);
	}
	Serial2.println(" ");
	
	DES.begin(true);
	DES.insertKey(key);
	DES.insertData(dataEnc);
	DES.processData();
	
	for(uint8_t i=0; i<8; i++){
		dataDec[i] = DES.getProcessedData(i);
		sprintf(str,"%02X ",dataDec[i]);
		Serial2.print(str);
	}

	uint8_t errorCode = 0;
	for(uint8_t i=0; i<8; i++){
		if(dataIn[i] != dataDec[i]) errorCode += 1<<i;
	}
	if(errorCode == 0) Serial2.print("OK!");
	else {
		sprintf(str,"%02X ",errorCode);
		Serial2.print(str);
	}
	Serial2.println(" ");
	
	for(volatile uint32_t i=0; i<0xE0000; i++);
	x++;
}