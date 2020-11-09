
#ifndef Wire_h
#define Wire_h
#include <stdint.h>
#include <XST_IOT.h>

#ifndef FOSC
#define FOSC 20000000UL
#endif

#define BUFFER_LENGTH 32

class TwoWire {
public:
	TwoWire(void);
	void begin(void);
	void setClock(uint32_t clockFreq);
	void beginTransmission(uint8_t);
	uint8_t write(uint8_t);
	inline uint8_t write(char data){ return(write((uint8_t) data)); }
	inline uint8_t write(int  data){ return(write((uint8_t) data)); }
	uint8_t write(const uint8_t data[], uint8_t size);
	uint8_t write(const char data[]);
	uint8_t endTransmission(bool);
	inline uint8_t endTransmission(void){ return(endTransmission(true)); }

	uint8_t requestFrom(uint8_t,uint8_t,bool);
	inline uint8_t requestFrom(uint8_t _address, uint8_t dataNum){
		return (requestFrom(_address, dataNum, true));
	}
	inline uint8_t available(void){ return rxBufferLength; }
	uint8_t read(void);

private:
	bool clockIsSet;
	uint8_t address;

	uint8_t rxBuffer[BUFFER_LENGTH];
	uint8_t rxBufferIndex;
	uint8_t rxBufferLength;

	volatile uint8_t txBuffer[BUFFER_LENGTH];
	uint8_t txBufferLength;

	uint8_t sendByte(uint8_t data, bool start, bool stop);
	void receiveByte(bool ack, bool stop);
};

extern TwoWire Wire;

#endif
