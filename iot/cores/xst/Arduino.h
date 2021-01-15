
#ifndef Arduino_h
#define Arduino_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "XST_IOT.h"
#include "cmsis_gcc.h"

#ifndef FOSC
#ifdef XST_X1_01
#define FOSC 20000000UL
#else
#define FOSC 50000000UL
#endif
#endif

// -----------------
//   MATH
// -----------------
#include <math.h>
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

// -----------------
//   SysTick
// -----------------

extern volatile unsigned long msTicks;
unsigned long micros(void);
inline __attribute__((always_inline)) unsigned long millis(void){ return msTicks; }
void delay(unsigned long);
void delayMicroseconds(unsigned long);

// ---------------------
//   Default Functions
// ---------------------

void setup(void);
void loop(void);
typedef void(*voidFuncPtr)(void);
typedef bool boolean;
typedef uint8_t byte;
typedef unsigned int word;
#define Handler extern "C" void __attribute__ ((section(".after_vectors")))

// ----------------------------
//   Global Interrupt Control
// ----------------------------

#define interrupts()   __enable_irq()
#define noInterrupts() __disable_irq()

// -----------------
//   UART
// -----------------

typedef enum : uint8_t {
	BIN = 2,
	OCT = 8,
	DEC = 10,
	HEX = 16
} integerBase;

class UART {
protected:
	CMSDK_UART_TypeDef* UART_dev;
public:
	UART(CMSDK_UART_TypeDef*);
	//size_t print(const String &);
	size_t print(const char[]);
	size_t print(char);
	size_t print(long, integerBase = DEC);
	size_t print(unsigned long, integerBase = DEC);
	inline size_t print(unsigned char data, integerBase base = DEC){
        return print((unsigned long) data, base);
    }
	inline size_t print(int data, integerBase base = DEC){
		return print((long) data, base);
	}
	inline size_t print(unsigned int data, integerBase base = DEC){
		return print((unsigned long) data, base);
	}
	size_t print(double, int = 2);
	//size_t print(const Printable&);

	//size_t println(const String &s);
	inline size_t println(void) { return print("\r\n"); }
	size_t println(const char[]);
	size_t println(char);
	size_t println(long, integerBase = DEC);
	size_t println(unsigned long, integerBase = DEC);
	inline size_t println(unsigned char data, integerBase base = DEC){
		return println((unsigned long) data, base);
	}
	inline size_t println(int data, integerBase base = DEC){
		return println((long) data, base);
	}
	inline size_t println(unsigned int data, integerBase base = DEC){
		return println((unsigned long) data, base);
	}
	size_t println(double, int = 2);
	//size_t println(const Printable&);
};

class UART0 : public UART
{
public:
	UART0(void) : UART(CMSDK_UART0) {}
	void begin(uint32_t speed);
	void end(void);
};

class UART1 : public UART
{
public:
	UART1(void) : UART(CMSDK_UART1) {}
	void begin(uint32_t speed);
	void end(void);
};

class UART2 : public UART
{
public:
	UART2(void) : UART(CMSDK_UART2) {}
	void begin(uint32_t speed);
	void end(void);
};

extern UART0 Serial;
extern UART1 Serial1;
extern UART2 Serial2;

// -----------------
//   TRNG
// -----------------

extern ARMTZ_TRNG_TypeDef* trng;
extern int seed_index;
extern int seed_sampling;

long random(long);
long random(long, long);
void randomSeed(unsigned long);
long random_test();

// -----------------
//   GPIO
// -----------------

#ifndef XST_X1_01
typedef struct {
	CMSDK_GPIO_TypeDef* pinPort;
	uint8_t             pinMask;
	IRQn_Type           pinIRQn;
} PinDescription;
extern const PinDescription boardPinTable[];
#endif

//#define INPUT  0
//#define OUTPUT 1
typedef enum : uint8_t {
	INPUT  = 0,
	OUTPUT = 1
} gpioMode;

//#define LOW    0
//#define HIGH   1
typedef enum : uint8_t {
	LOW     = 0,
	HIGH    = 1,
	FALLING = 2,
	RISING  = 3
} gpioIntModeVal;

void           pinMode     (uint8_t pin, gpioMode       mode);
gpioIntModeVal digitalRead (uint8_t pin                     );
void           digitalWrite(uint8_t pin, gpioIntModeVal val );

// -------------------
//   GPIO Interrupts
// -------------------

//#define RISING  2
//#define FALLING 3

void attachInterrupt(uint8_t pin, voidFuncPtr callback, gpioIntModeVal mode);
void detachInterrupt(uint8_t pin);

// -------------------
//   Analog I/O
// -------------------

//int analogRead(uint8_t pin);
//void analogReference(uint8_t mode);

#define TIMER_CTRL_DEFAULT 0x20
#define TIMER_LOAD_DEFAULT 65535

#define PRESCALE_1_1   0
#define PRESCALE_1_16  1
#define PRESCALE_1_256 2

#define PWM_16_MAX 65535

void analogWrite(uint8_t pin, int val);

// PWM API
/**************************************************************************/
/*!
    @brief   Set PWM on cycle
    @param   pin pin number (4 or 5)
    @param   on_cycle on cycle, with default period cycle is 65535 (0 to period cycle)
*/
/**************************************************************************/
void pwmWrite(uint32_t pin, uint32_t on_cycle);

/**************************************************************************/
/*!
    @brief   Change PWM period
    @param   pin pin number (4 or 5)
    @param   load maximum counting value, 0 to 65535
*/
/**************************************************************************/
void pwmChangeLoad(uint32_t pin, uint32_t load);

/**************************************************************************/
/*!
    @brief   Function to convert ducy cycle percentage to on_cycle
    @param   pin pin number (4 or 5)
    @param   percent duty cycle percentage, 0 to 100
    @return  32-bit integer to be passed to on_cycle
*/
/**************************************************************************/
uint32_t pwmDutyCalc(uint32_t pin, double percent);

/**************************************************************************/
/*!
    @brief   Change dualtimer clock divider
    @param   pin pin number (4 or 5)
    @param   pwm clock divider (1/1, 1/16, or 1/256)
*/
/**************************************************************************/
void pwmChangeDiv(uint32_t pin, uint32_t prescaler);

// -------------------
//   Advanced I/O
// -------------------
extern bool update_tone;
void tone(uint8_t _pin, unsigned long frequency);
void noTone(uint8_t _pin);


#endif
