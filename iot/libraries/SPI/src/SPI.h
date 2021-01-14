
#ifndef XSTIOT_SPI_H
#define XSTIOT_SPI_H

#include <stdlib.h>
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

//enum dataMode: uint8_t {
//	SPI_MODE0 = 0b00,
//	SPI_MODE1 = 0b01,
//	SPI_MODE2 = 0b10,
//	SPI_MODE3 = 0b11
//};
#define dataMode uint32_t
#define SPI_MODE0 0b00
#define SPI_MODE1 0b01
#define SPI_MODE2 0b10
#define SPI_MODE3 0b11

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
	friend class SPI_0;
	friend class SPI_1;
};

#define SPIClass SPI_0

class SPIMaster {
	protected:
	SPI_TypeDef* SPI_dev;
public:
	SPIMaster(SPI_TypeDef *);

	// old version
	inline void setClockDivider(clockDiv divider){
		SPI_dev->SPCR = (SPI_dev->SPCR & 0b11111100) | ((uint8_t) divider & 0b11);
		SPI_dev->SPER = (SPI_dev->SPER & 0b11111100) | ((uint8_t) divider >> 2  );
	}
	inline void setBitOrder(uint8_t bitOrder) {}
	
	//void begin(void);
	//void begin(uint8_t pin);
	//void end(uint8_t pin);

	// new version
	//uint8_t beginTransaction(uint8_t pin, SPISettings settings);
	//uint8_t endTransaction(uint8_t pin);
	//usingInterrupt()

	uint8_t transfer(uint8_t val);
	void transfer(const uint8_t val[], uint8_t size);
	uint16_t transfer16(uint16_t val);

	void attachInterrupt(voidFuncPtr callback);
	void detachInterrupt(void);
};

class SPI_0 : public SPIMaster
{
public:
	SPI_0(void) : SPIMaster(XSTIOT_SPI) {}
	inline void setDataMode(dataMode mode)
	{
		SPI_dev->SPCR = (SPI_dev->SPCR & 0b11110011) | ((uint8_t)mode << XSTIOT_SPI_CPHA_Pos);
		// CMSDK_GPIO1->UB_MASKED[(1<<10)] = (mode >= SPI_MODE2)<<10;
		CMSDK_GPIO9->LB_MASKED[(1 << 5)] = (mode >= SPI_MODE2) << 5;
	}
	void end(void);

	// new version
	void beginTransaction(SPISettings settings);
	inline void endTransaction(void) { end(); }
};

class SPI_1 : public SPIMaster
{
public:
	SPI_1(void) : SPIMaster(XSTIOT_SPI1) {}
	inline void setDataMode(dataMode mode)
	{
		SPI_dev->SPCR = (SPI_dev->SPCR & 0b11110011) | ((uint8_t)mode << XSTIOT_SPI_CPHA_Pos);
		// CMSDK_GPIO1->UB_MASKED[(1<<10)] = (mode >= SPI_MODE2)<<10;
		CMSDK_GPIO1->LB_MASKED[(1 << 3)] = (mode >= SPI_MODE2) << 3;
	}
	void end(void);

	// new version
	void beginTransaction(SPISettings settings);
	inline void endTransaction(void) { end(); }
};


extern SPI_0 SPI;
extern SPI_1 SPI1;

#endif
