
#include "AES.h"

AESClass AES;

uint8_t AESClass::insertKey(const uint32_t key[], aesKeySize_t keySize){
	uint8_t count = 0;

	XSTIOT_AES->AESCR |= ((uint8_t) keySize << XSTIOT_AES_KEYSIZE_Pos);
	for(uint8_t i=0; i<4; i++){
		XSTIOT_AES->KEY[i] = key[i];
		count++;
	}
	if(keySize >= AES_SIZE_192){
		XSTIOT_AES->KEY[4] = key[4];
		count++;
		XSTIOT_AES->KEY[5] = key[5];
		count++;
	}
	if(keySize >= AES_SIZE_256){
		XSTIOT_AES->KEY[6] = key[6];
		count++;
		XSTIOT_AES->KEY[7] = key[7];
		count++;
	}
	while(!(XSTIOT_AES->AESSR & XSTIOT_AES_INSKEY_Msk));
	return count;
}

uint8_t AESClass::insertData(const uint32_t data[]){
	uint8_t count = 0;

	for(uint8_t i=0; i<4; i++){
		XSTIOT_AES->DATA[i] = data[i];
		count++;
	}
	while(!(XSTIOT_AES->AESSR & XSTIOT_AES_START_Msk));
	return count;
}
