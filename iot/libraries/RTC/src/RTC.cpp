// -----------------
//   RTC
// -----------------

#include "XST_IOT.h"
#include "RTC.h"

volatile uint8_t RTCTick = 0;

RTCXstIot::RTCXstIot()
{
	_enabled     = false         ;
	time.year    = DEFAULT_YEAR  ;
	time.month   = DEFAULT_MONTH ;
	time.day     = DEFAULT_DAY   ;
	time.hours   = DEFAULT_HOUR  ;
	time.minutes = DEFAULT_MINUTE;
	time.seconds = DEFAULT_SECOND;
	alarmConf    = MATCH_OFF;
}

void RTCXstIot::begin(void){
	NVIC_DisableIRQ(RTC_IRQn);
	RTC->RTCCR   = 0;

	RTC->RTCLR   = 0;
	RTC->RTCMR   = 32768;

	RTC->RTCICR  = 1;
	NVIC_ClearPendingIRQ(RTC_IRQn);

	RTC->RTCIMSC = 1;
	NVIC_EnableIRQ(RTC_IRQn);
	RTC->RTCCR   = 1;

	_enabled = true;
}

void RTCXstIot::RTCRoutine(void){
	bool incrementMonth = false,
		 RTCNextAlarm = false;

	if(_enabled && RTCTick){
		time.seconds += RTCTick;
		RTCTick = 0;

		if(time.seconds >= 60){
			time.minutes++;
			time.seconds = 0;
		}
		if(time.minutes >= 60){
			time.hours++;
			time.minutes = 0;
		}
		if(time.hours >= 24){
			time.day++;
			time.hours = 0;
		}
		if(time.day > 28){
			switch(time.month){
			case 2:
				if(isLeapYear(time.year)){
					   if(time.day > 29) incrementMonth = true;
				} else if(time.day > 28) incrementMonth = true;
				break;
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				if(time.day > 31) incrementMonth = true;
				break;
			default:
				if(time.day > 30) incrementMonth = true;
				break;
			}
			if(incrementMonth){
				time.month++;
				time.day = 1;
			}
		}
		if(time.month > 12){
			time.year++;
			time.month = 1;
		}

		if(time.seconds == alarmTime.seconds && alarmConf != MATCH_OFF){
			if(alarmConf >= MATCH_SS)           RTCNextAlarm = true;
			if(alarmConf >= MATCH_MMSS)         RTCNextAlarm = RTCNextAlarm && (time.minutes == alarmTime.minutes);
			if(alarmConf >= MATCH_HHMMSS)       RTCNextAlarm = RTCNextAlarm && (time.hours   == alarmTime.hours  );
			if(alarmConf >= MATCH_DDHHMMSS)     RTCNextAlarm = RTCNextAlarm && (time.day     == alarmTime.day    );
			if(alarmConf >= MATCH_MMDDHHMMSS)   RTCNextAlarm = RTCNextAlarm && (time.month   == alarmTime.month  );
			if(alarmConf >= MATCH_YYMMDDHHMMSS) RTCNextAlarm = RTCNextAlarm && (time.year    == alarmTime.year   );

			if(RTCNextAlarm && rtcCallback != NULL) rtcCallback();
			RTCNextAlarm = false;
		}
	}
}

extern "C" {
	void RTC_Handler(void){
		RTCTick++;
		RTC->RTCLR = 0;
		RTC->RTCICR = 1;
		NVIC_ClearPendingIRQ(RTC_IRQn);
	}
}

void RTCXstIot::enableAlarm(Alarm_Match match){
	alarmConf = match;
}

void RTCXstIot::disableAlarm(void){
	alarmConf = MATCH_OFF;
}

void RTCXstIot::attachInterrupt(voidFuncPtr callback){
	rtcCallback = callback;
}

void RTCXstIot::detachInterrupt(){
	rtcCallback = NULL;
}

void RTCXstIot::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds){
	setSeconds(seconds);
	setMinutes(minutes);
	setHours  (hours  );
}

void RTCXstIot::setDate(uint8_t day, uint8_t month, uint16_t year){
	setDay  (day  );
	setMonth(month);
	setYear (year );
}

void RTCXstIot::setAlarmTime(uint8_t hours, uint8_t minutes, uint8_t seconds){
	setAlarmSeconds(seconds);
	setAlarmMinutes(minutes);
	setAlarmHours  (hours  );
}

void RTCXstIot::setAlarmDate(uint8_t day, uint8_t month, uint16_t year){
	setAlarmDay  (day  );
	setAlarmMonth(month);
	setAlarmYear (year );
}
