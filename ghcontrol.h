/**  @brief Constants, structures, function prototypes for ghcontrol code
 *   @file ghcontrol.h
 */
#ifndef GHCONTROL_H
#define GHCONTROL_H
#include "pisensehat.h"
#include <stdint.h>
#include <time.h>

#define SEARCHSTR "serial\t\t: "
#define SYSINFOBUFSZ 512
#define GHUPDATE 2000
#define SENSORS 3     // not used
#define TEMPERATURE 0 // not used
#define HUMIDITY 1    // not used
#define PRESSURE 2    // not used
#define SIMULATE 1    // not used
#define USTEMP 50
#define LSTEMP -10
#define USPRESS 1016
#define LSPRESS 975
#define STEMP 25.0
#define USHUMID 100
#define LSHUMID 0
#define SHUMID 55.0
#define ON 1
#define OFF 0
#define SIMTEMPERATURE 0
#define SIMHUMIDITY 0
#define SIMPRESSURE 0
#define CTIMESTRSZ 25
#define NUMBARS 8
#define NUMPTS 8.0
#define TBAR 7
#define HBAR 5
#define PBAR 3
#define SENSEHAT 1

typedef struct readings {
  time_t rtime;
  double temperature;
  double humidity;
  double pressure;
} reading_s;
typedef struct setpoints {
  double temperature;
  double humidity;
} setpoint_s;
typedef struct controls {
  int heater;
  int humidifier;
} control_s;

///@cond INTERNAL
int GhGetRandom(int range);
uint64_t GhGetSerial(void);
void GhDisplayHeader(const char *sname);
void GhDelay(int milliseconds);
void GhControllerInit(void);
void GhDisplayControls(control_s ctrl);
void GhDisplayReadings(reading_s rdata);
void GhDisplayTargets(setpoint_s spts);
control_s GhSetControls(setpoint_s target, reading_s rdata);
setpoint_s GhSetTargets(void);
double GhGetHumidity(void);
double GhGetPressure(void);
double GhGetTemperature(void);
reading_s GhGetReadings(void);
int GhLogData(char *fname, reading_s ghdata);
int GhSaveSetPoints(char *fname, setpoint_s spts);
setpoint_s GhRetrieveSetPoints(char *fname);
void GhDisplayAll(reading_s rd, setpoint_s sd);
///@endcond

#endif