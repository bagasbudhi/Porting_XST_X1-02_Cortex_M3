#define FOSC 20000000UL
#include <Arduino.h>
#include <stdio.h>
#include "RTC.h"

void rtcInterrupt(void);

RTCXstIot rtc;
volatile uint8_t rtcI;

void setup(void)
{
  Serial2.begin(115200);
  Serial2.println("Hello from UART2!");
  
  rtc.setTime(23,59,59);
  rtc.setDate(31,12,2019);
  rtcI = 0;
  
  rtc.setAlarmSeconds(1);
  rtc.enableAlarm(RTCXstIot::MATCH_SS);
  rtc.attachInterrupt(rtcInterrupt);
  
  rtc.begin();
}

void loop(void)
{
	char timeStr[8];
	char dateStr[10];
	rtc.RTCRoutine();
	
	if(rtcI){
		Serial2.print("Hi! Current time is ");
		sprintf(timeStr,"%02d:%02d:%02d",rtc.getHours(),rtc.getMinutes(),rtc.getSeconds());
		Serial2.println(timeStr);
		
		Serial2.print("Hi! Current date is ");
		sprintf(dateStr,"%d/%d/%d",rtc.getDay(),rtc.getMonth(),rtc.getYear());
		Serial2.println(dateStr);
		
		rtcI = 0;
	}
	
}

void rtcInterrupt(void){
	rtcI++;
}