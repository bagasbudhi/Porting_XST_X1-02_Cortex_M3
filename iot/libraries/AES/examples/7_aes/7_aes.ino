
#include <Arduino.h>
#include <AES.h>

void setup(void){
	Serial2.begin(115200);
	Serial2.println("Hello!");
}

void loop(void){
	static uint8_t x = 0;
	uint32_t key[8];
	uint32_t data[4];
	static uint32_t dataEnc[4];
	uint32_t dataDec[4];
	
	for(uint8_t i=0; i<8; i++){
		key[i] = 0xFFFFFFFF - (( i   *0x10 + x  ) << 24)
							- (((i+1)*0x10 + x+1) << 16)
							- (((i+2)*0x10 + x+2) << 8 )
							- (((i+3)*0x10 + x+3) << 0 );
	}
	
	for(uint8_t i=0; i<4; i++){
		data[i] = (( i   *0x10 + x  ) << 24)
				+ (((i+1)*0x10 + x+1) << 16)
				+ (((i+2)*0x10 + x+2) << 8 )
				+ (((i+3)*0x10 + x+3) << 0 );
	}
	
	AES.begin(true);
	AES.insertKey(key,AES_SIZE_128);
	AES.insertData(data);
	
	for(uint8_t i=0; i<4; i++){
		dataEnc[i] = AES.getProcessedData(i);
	}
	
	AES.begin(false);
	AES.insertKey(key,AES_SIZE_128);
	AES.insertData(dataEnc);
	
	uint8_t result = 0;
	for(uint8_t i=0; i<4; i++){
		dataDec[i] = AES.getProcessedData(i);
		if(dataDec[i] != data[i]) result |= (1<<i);
	}
	
	char str[8];
	if(result == 0) Serial2.println("OK!");
	else {		
		for(uint8_t i=0; i<8; i++){
			sprintf(str,"%08X",key[i]);
			Serial2.print("0x");
			Serial2.print(str);
			Serial2.print(" ");
		}	Serial2.println(" ");

		for(uint8_t i=0; i<8; i++){
			sprintf(str,"%08X",data[i]);
			Serial2.print("0x");
			Serial2.print(str);
			Serial2.print(" ");
		}	Serial2.println(" ");

		for(uint8_t i=0; i<8; i++){
			sprintf(str,"%08X",dataEnc[i]);
			Serial2.print("0x");
			Serial2.print(str);
			Serial2.print(" ");
		}	Serial2.println(" ");

		for(uint8_t i=0; i<8; i++){
			sprintf(str,"%08X",dataDec[i]);
			Serial2.print("0x");
			Serial2.print(str);
			Serial2.print(" ");
		}	Serial2.println(" ");

		sprintf(str,"%02X",result);
		Serial2.print("0x");
		Serial2.print(str);
		Serial2.println(" ");

		for(volatile uint32_t i=0; i<0xE0000; i++);
	}
	
	x++;
}
