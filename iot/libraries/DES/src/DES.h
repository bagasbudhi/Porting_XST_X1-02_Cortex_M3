
#include <stdint.h>
#include <XST_IOT.h>

class DESClass {
public:
	DESClass(void);
	void begin(bool);
	uint8_t insertKey(const uint8_t key[]);
	uint8_t insertData(const uint8_t data[]);
	void processData(void);
	inline uint8_t getProcessedData(uint8_t index){ return processedData[index]; }
private:
	bool decryptMode;
	uint8_t processedData[8];
};

extern DESClass DES;
