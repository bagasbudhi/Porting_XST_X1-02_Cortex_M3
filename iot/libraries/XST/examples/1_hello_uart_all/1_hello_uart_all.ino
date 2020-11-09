
#include <Arduino.h>

#define BAUDRATE 115200

#define PERIOD 500000UL

void setup(void)
{
  Serial.begin(BAUDRATE);
  Serial1.begin(BAUDRATE);
  Serial2.begin(BAUDRATE);
}

void loop(void)
{
	static unsigned long lastTime = 0UL;
	
	unsigned long usTime = micros();
	unsigned long msTime = millis();
	
	if((usTime - lastTime) >= PERIOD){
		Serial.print ("UART0: ");
		Serial1.print("UART1: ");
		Serial2.print("UART2: ");
		
		Serial.print (usTime);
		Serial1.print(usTime);
		Serial2.print(usTime);
		Serial.print (" us, ");
		Serial1.print(" us, ");
		Serial2.print(" us, ");
		
		Serial.print (msTime);
		Serial1.print(msTime);
		Serial2.print(msTime);
		Serial.println (" ms");
		Serial1.println(" ms");
		Serial2.println(" ms");
		
		lastTime += PERIOD;
	}
	//for(volatile uint32_t i=0; i<0x70000;i++);
}
