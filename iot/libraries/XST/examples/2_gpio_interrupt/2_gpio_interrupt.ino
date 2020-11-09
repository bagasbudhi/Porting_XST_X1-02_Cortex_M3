#define FOSC 20000000UL
#include <Arduino.h>

#define BAUDRATE 115200

volatile uint8_t p0int;

void setup(void)
{
    noInterrupts();
    
    Serial2.begin(BAUDRATE);
	Serial2.println("Hello from UART2!");
	
	pinMode(16+6,OUTPUT);
	pinMode(0,INPUT);
	pinMode(16+7,OUTPUT);
	
	p0int = 0;
	attachInterrupt(0,0,RISING);

	interrupts();
}

void loop(void)
{
  static uint32_t i = 0;
  
  //for(i=0;i<70000;i++);
  if(i==0x7000) digitalWrite(16+6,HIGH);
  //for(i=0;i<70000;i++);
  else if(i>=0xE000){
	  digitalWrite(16+6,LOW);
	  i = 0;
  }
  
  i++;
  
  if(digitalRead(0)) digitalWrite(16+7,HIGH);
  else               digitalWrite(16+7,LOW);
  
  if(p0int){
	  Serial2.println("Interrupt from P0[0]!");
	  p0int = 0;
  }
}

extern "C" {
	void PORT0_0_Handler(void){
		//noInterrupts();
		CMSDK_GPIO0->INTCLEAR = 1<<0;
		NVIC_ClearPendingIRQ(PORT0_0_IRQn);
		p0int = 1;
		//interrupts();
	}
 }
