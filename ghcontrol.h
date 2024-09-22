/** @breif GH control constants, structures, function prototypes
 *  @file ghcontrol.h
 */


#ifndef GHCONTROL_H
#define GHCONTROL_H
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "pisensehat.h"


// Constants
#define SEARCHSTR "serial\t\t:"
#define SYSINFOBUFSZ 512
#define GHUPDATE 2000
#define SENSORS 3
#define TEMPERATURE 0
#define HUMIDITY 1
#define PRESSURE 2
#define SIMTEMPERATURE 0
#define SIMHUMIDITY 0
#define SIMPRESSURE 0

//Alarm Constants
#define NALARMS 7
#define LOWERATEMP 10
#define UPPERATEMP 30
#define LOWERAHUMID 25
#define UPPERAHUMID 70
#define LOWERAPRESS 985
#define UPPERAPRESS 1016

//Sensehat Constants
#define NUMBARS 8
#define NUMPTS 8.0
#define TBAR 7
#define HBAR 5
#define PBAR 3
#define SENSEHAT 1


// Simulation Constants
#define SIMULATE 1
#define USTEMP 50
#define LSTEMP -50
#define USHUMID 100
#define LSHUMID 0
#define USPRESS 1016
#define LSPRESS 975
#define ON 1
#define OFF 0
#define CTIMESTRSZ 25


// Control Constants
#define STEMP 25.0
#define SHUMID 55.0


//Function Prototypes
///@cond INTERNAL 
uint64_t GhGetSerial(void);
struct setpoints GhSetTargets(void);
struct readings GhGetReadings(void);
struct controls GhSetControls(struct setpoints target, struct readings rdata);
struct setpoints GhRetrieveSetpoints(char * fname);
void GhSetAlarms(alarm_s calarm[NALARMS],alarmlimit_s alarmpt, reading_s rdata);
struct alarmlimit_s GhSetAlarmLimits(void);
void GhDisplayHeader(const char * sname);
int GhGetRandom(int range);
void GhDelay(int milliseconds);
void GhControllerInit(void);
void GhDisplayControls(struct controls ctrl);
void GhDisplayReadings(struct readings rdata);
double GhGetHumidity(void);
double GhGetPressure(void);
double GhGetTemperature(void);
void GhDisplayTargets(struct setpoints spts);
int GhLogData(char * fname, struct readings ghdata);
int GhSaveSetpoints(char * fname, struct setpoints spts);
void GhDisplayAll(struct readings rd, struct setpoints sd);
///@endcond 

//Structures
typedef struct readings
{
	time_t rtime;
	double temperature;
	double humidity;
	double pressure;
}reading_s;

typedef struct setpoints 
{
	double temperature;
	double humidity;
	double spts;
}setpoint_s;
typedef struct controls
{
	int heater;
	int humidifier;
}control_s;


// Enumerated Types
typedef enum { NOALARM,HTEMP,LTEMP,HHUMID,LHUMID,HPRESS,LPRESS } alarm_e;

typedef struct alarmlimits
{
double hight;
double lowt;
double highh;
double lowh;
double highp;
double lowp;
}alarmlimit_s;

typedef struct alarms
{
alarm_e code;
time_t atime;
double value;
}alarm_s;



#endif
