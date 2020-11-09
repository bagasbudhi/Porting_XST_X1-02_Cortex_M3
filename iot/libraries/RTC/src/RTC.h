// -----------------
//   RTC
// -----------------

#include <stdio.h>
#include <stdint.h>

typedef void(*voidFuncPtr)(void);

#define EPOCH_TIME_OFF      946684800  // This is 1st January 2000, 00:00:00 in epoch time
#define EPOCH_TIME_YEAR_OFF 100        // years since 1900

// Default date & time after reset
#define DEFAULT_YEAR    2000    // 2000..2063
#define DEFAULT_MONTH   1       // 1..12
#define DEFAULT_DAY     1       // 1..31
#define DEFAULT_HOUR    0       // 1..23
#define DEFAULT_MINUTE  0       // 0..59
#define DEFAULT_SECOND  0       // 0..59

class RTCXstIot {
public:
	enum Alarm_Match: uint8_t
	{
	  MATCH_OFF          = 0,  // Never
	  MATCH_SS           = 1,  // Every Minute
	  MATCH_MMSS         = 2,  // Every Hour
	  MATCH_HHMMSS       = 3,  // Every Day
	  MATCH_DDHHMMSS     = 4,  // Every Month
	  MATCH_MMDDHHMMSS   = 5,  // Every Year
	  MATCH_YYMMDDHHMMSS = 6   // Once, on a specific date and a specific time
	};

	RTCXstIot();
	void begin(void);
	void RTCRoutine(void);

	void enableAlarm(Alarm_Match match);
	void disableAlarm();

	void attachInterrupt(voidFuncPtr callback);
	void detachInterrupt();

	//void standbyMode();

	/* Get Functions */

	inline uint8_t getSeconds(){return time.seconds;}
	inline uint8_t getMinutes(){return time.minutes;}
	inline uint8_t getHours()  {return time.hours  ;}

	inline uint8_t  getDay()  {return time.day  ;}
	inline uint8_t  getMonth(){return time.month;}
	inline uint16_t getYear() {return time.year ;}

	inline uint8_t getAlarmSeconds(){return alarmTime.seconds;}
	inline uint8_t getAlarmMinutes(){return alarmTime.minutes;}
	inline uint8_t getAlarmHours()  {return alarmTime.hours  ;}

	inline uint8_t  getAlarmDay()  {return alarmTime.day  ;}
	inline uint8_t  getAlarmMonth(){return alarmTime.month;}
	inline uint16_t getAlarmYear() {return alarmTime.year ;}

	/* Set Functions */

	inline void setSeconds(uint8_t seconds){time.seconds = seconds;}
	inline void setMinutes(uint8_t minutes){time.minutes = minutes;}
	inline void setHours  (uint8_t hours)  {time.hours   = hours  ;}
	void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

	inline void setDay  (uint8_t  day  ){time.day   = day  ;}
	inline void setMonth(uint8_t  month){time.month = month;}
	inline void setYear (uint16_t year ){time.year  = year ;}
	void setDate(uint8_t day, uint8_t month, uint16_t year);

	inline void setAlarmSeconds(uint8_t seconds){alarmTime.seconds = seconds;}
	inline void setAlarmMinutes(uint8_t minutes){alarmTime.minutes = minutes;}
	inline void setAlarmHours  (uint8_t hours  ){alarmTime.hours   = hours  ;}
	void setAlarmTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

	inline void setAlarmDay  (uint8_t  day  ){alarmTime.day   = day  ;}
	inline void setAlarmMonth(uint8_t  month){alarmTime.month = month;}
	inline void setAlarmYear (uint16_t year ){alarmTime.year  = year ;}
	void setAlarmDate(uint8_t day, uint8_t month, uint16_t year);

	/* Epoch Functions (not implemented yet) */

	uint32_t getEpoch();
	uint32_t getY2kEpoch();
	void setEpoch(uint32_t ts);
	void setY2kEpoch(uint32_t ts);
	void setAlarmEpoch(uint32_t ts);

	//bool isConfigured() { return _configured; }

private:
	bool _enabled;
	struct time_s {
		uint16_t year;
		uint8_t month,
				day,
				hours,
				minutes,
				seconds;
	} time, alarmTime;
	Alarm_Match alarmConf;
	voidFuncPtr rtcCallback = NULL;
	bool isLeapYear(uint16_t year){ return ((year % 4) ? ((year % 100) ? ((year % 400) ? true : false ) : true ) : false );}
};
