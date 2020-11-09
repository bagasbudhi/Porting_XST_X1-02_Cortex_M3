
#include "DES.h"

DESClass DES;

DESClass::DESClass(void){
	decryptMode = false;
}

void DESClass::begin(bool _decryptMode){
	decryptMode = _decryptMode;
}

uint8_t DESClass::insertKey(const uint8_t key[]){
	uint8_t count = 0;

	XSTIOT_DES->DESCR = XSTIOT_DES_INSKEY_Msk | (decryptMode ? XSTIOT_DES_DECRYPT_Msk : 0);
	for(uint8_t i=0; i<8; i++){
		XSTIOT_DES->DESIN = key[i];
		count++;
	}

	return count;
}

uint8_t DESClass::insertData(const uint8_t data[]){
	uint8_t count = 0;

	XSTIOT_DES->DESCR = (decryptMode ? XSTIOT_DES_DECRYPT_Msk : 0);
	for(uint8_t i=0; i<8; i++){
		XSTIOT_DES->DESIN = data[i];
		count++;
	}

	return count;
}

void DESClass::processData(void){
	XSTIOT_DES->DESCR = XSTIOT_DES_START_Msk | (decryptMode ? XSTIOT_DES_DECRYPT_Msk : 0);
	while(!(XSTIOT_DES->DESSR & XSTIOT_DES_STATUS_Msk));
	while((XSTIOT_DES->DESSR & XSTIOT_DES_STATUS_Msk));
	for(uint8_t i=0;i<8;i++)
		processedData[i] = XSTIOT_DES->DESOUT;
}
