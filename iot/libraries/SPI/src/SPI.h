
#include <stdint.h>
#include <XST_IOT.h>

typedef void(*voidFuncPtr)(void);
enum clockDiv: uint8_t {
	SPI_CLOCK_DIV2    = 0b0000,
	SPI_CLOCK_DIV4    = 0b0001,
	SPI_CLOCK_DIV8    = 0b0100,
	SPI_CLOCK_DIV16   = 0b0010,
	SPI_CLOCK_DIV32   = 0b0011,
	SPI_CLOCK_DIV64   = 0b0101,
	SPI_CLOCK_DIV128  = 0b0110,
	SPI_CLOCK_DIV256  = 0b0111,
	SPI_CLOCK_DIV512  = 0b1000,
	SPI_CLOCK_DIV1024 = 0b1001,
	SPI_CLOCK_DIV2048 = 0b1010,
	SPI_CLOCK_DIV4096 = 0b1011
};
enum dataMode: uint8_t {
	SPI_MODE0 = 0b00,
	SPI_MODE1 = 0b01,
	SPI_MODE2 = 0b10,
	SPI_MODE3 = 0b11
};
enum dataOrder: uint8_t {
	MSBFIRST = 0,
	LSBFIRST = 1
};

class SPISettings {
public:
	SPISettings(uint32_t maxSpeed, dataOrder _order, dataMode _mode);

private:
	clockDiv divider;
	dataOrder order;
	dataMode  mode;
	friend class SPIMaster;
};

class SPIMaster {
public:
	//SPIMaster();

	// old version
	inline void setClockDivider(clockDiv divider){
		XSTIOT_SPI->SPCR = (XSTIOT_SPI->SPCR & 0b11111100) | ((uint8_t) divider & 0b11);
		XSTIOT_SPI->SPER = (XSTIOT_SPI->SPER & 0b11111100) | ((uint8_t) divider >> 2  );
	}
	inline void setDataMode(dataMode mode){
		XSTIOT_SPI->SPCR = (XSTIOT_SPI->SPCR & 0b11110011) | ((uint8_t) mode << XSTIOT_SPI_CPHA_Pos);
		CMSDK_GPIO1->UB_MASKED[(1<<10)] = (mode >= SPI_MODE2)<<10;
	}
	// setBitOrder()
	//void begin(void);
	//void begin(uint8_t pin);
	void end(void);
	//void end(uint8_t pin);

	// new version
	void beginTransaction(SPISettings settings);
	//uint8_t beginTransaction(uint8_t pin, SPISettings settings);
	inline void endTransaction(void){end();}
	//uint8_t endTransaction(uint8_t pin);
	//usingInterrupt()

	uint8_t transfer(uint8_t val);
	void transfer(const uint8_t val[], uint8_t size);
	uint16_t transfer16(uint16_t val);

	void attachInterrupt(voidFuncPtr callback);
	void detachInterrupt(void);
};

extern SPIMaster SPI;
