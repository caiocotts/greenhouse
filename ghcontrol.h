/**  @brief Constants, structures, function prototypes for ghcontrol code
 *   @file ghcontrol.h
 */
#ifndef GHCONTROL_H
#define GHCONTROL_H
#include <stdint.h>
#include <time.h>

#define USHUMID 100
#define LSHUMID 0
#define SEARCHSTR "serial\t\t"
#define SYSINFOBUFSZ 512
#define GHUPDATE 2000
#define SENSORS 3
#define TEMPERATURE 0
#define HUMIDITY 1
#define PRESSURE 2
#define SIMULATE 1
#define USTEMP 50
#define LSTEMP -10
#define USPRESS 1016
#define LSPRESS 975
#define STEMP 25.0
#define SHUMID 55.0
#define ON 1
#define OFF 0
#define SIMTEMPERATURE 1
#define SIMHUMIDITY 1
#define SIMPRESSURE 1

struct readings
{
  time_t rtime;
  double temperature;
  double humidity;
  double pressure;
};
struct setpoints
{
  double temperature;
  double humidity;
};
struct controls
{
  int heater;
  int humidifier;
};

///@cond INTERNAL
int GhGetRandom(int range);
uint64_t GhGetSerial(void);
void GhDisplayHeader(const char *sname);
void GhDelay(int milliseconds);
void GhControllerInit(void);
void GhDisplayControls(struct controls ctrl);
void GhDisplayReadings(struct readings rdata);
void GhDisplayTargets(void);
struct controls GhSetControls(struct setpoints target, struct readings rdata);
struct setpoints GhSetTargets(void);
double GhGetHumidity(void);
double GhGetPressure(void);
double GhGetTemperature(void);
struct readings GhGetReadings(void);
///@endcond

#endif