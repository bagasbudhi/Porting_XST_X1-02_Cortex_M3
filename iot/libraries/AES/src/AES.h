
#include <stdint.h>
#include <XST_IOT.h>

typedef enum : uint8_t {
	AES_SIZE_128 = 0,
	AES_SIZE_192 = 1,
	AES_SIZE_256 = 2
} aesKeySize_t;

class AESClass {
public:
	inline void begin(bool encryptMode){
		XSTIOT_AES->AESCR = encryptMode ? XSTIOT_AES_MODE_Msk : 0;
	}
	uint8_t insertKey(const uint32_t key[], aesKeySize_t keySize);
	uint8_t insertData(const uint32_t data[]);
	inline uint32_t getProcessedData(uint8_t index){ return XSTIOT_AES->DATA[index]; }
};

extern AESClass AES;
