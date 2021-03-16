/**  @brief Code for ghcontrol functions
 *   @file ghcontrol.c
 */
#include "ghcontrol.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**  @brief Delay program for a specific amount of time.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @param milliseconds Holds a value of time in milliseconds.
 *   @return void
*/
void GhDelay(int milliseconds)
{
  long wait;
  clock_t now, start;

  wait = milliseconds * (CLOCKS_PER_SEC / 1000);
  start = clock();
  now = start;
  while ((now - start) < wait)
  {
    now = clock();
  }
}

/**  @brief Get serial number of host computer.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @return Serial number as a long unsigned integer.
*/
uint64_t GhGetSerial(void)
{

  static uint64_t serial = 0;
  FILE *fp;
  char buf[SYSINFOBUFSZ];
  char searchstring[] = SEARCHSTR;
  fp = fopen("/proc/cpuinfo", "r");
  if (fp != NULL)
  {
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
      if (!strncasecmp(searchstring, buf, strlen(searchstring)))
      {
        sscanf(buf + strlen(searchstring), "%Lx", &serial);
      }
    }
    fclose(fp);
  }
  if (serial == 0)
  {
    system("uname -a");
    system("ls --fu /usr/lib/codeblocks | grep -Po '\\.\\K[^ ]+'>stamp.txt");
    fp = fopen("stamp.txt", "r");
    if (fp != NULL)
    {
      while (fgets(buf, sizeof(buf), fp) != NULL)
      {
        sscanf(buf, "%lu", &serial);
      }
      fclose(fp);
    }
  }
  return serial;
}

/**  @brief Get a random number based on current rand seed.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @param range Holds a value representing the range of two values.
 *   @return 
*/
int GhGetRandom(int range) { return rand() % range; }

/**  @brief Print a header with a specified username.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @param  sname Pointer to a string containing a username.
 *   @return void
*/
void GhDisplayHeader(const char *sname)
{
  fprintf(stdout, "%s' Greenhouse Controller\n\n\n", sname);
}

/**  @brief Generate a random seed based on the current time and call GhDisplayHeader.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @return void 
*/
void GhControllerInit(void)
{
  srand((unsigned)time(NULL));
  GhDisplayHeader("Caio Cotts");
}

/**  @brief Display current time and sensor readings.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @param rdata struct variable holding sensor readings.
 *   @return void
*/
void GhDisplayReadings(struct readings rdata)
{

  fprintf(stdout,
          "\nUnit:%Lx %s Readings\tT: %5.1lfC\tH: %5.1lf%%\tP: %6.1lfmb\n ",
          GhGetSerial(), ctime(&rdata.rtime), rdata.temperature, rdata.humidity,
          rdata.pressure);
}

/**  @brief Get current humidity measurements.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @return Humidity as a percentage. 
*/
double GhGetHumidity(void)
{
#if SIMHUMIDITY
  return GhGetRandom(USHUMID - LSHUMID) + LSHUMID;
#else
  return 55.0;
#endif
}

/**  @brief Get current pressure measurements.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @return Pressure in millibars.
*/
double GhGetPressure(void)
{
#if SIMPRESSURE
  return GhGetRandom(USPRESS - LSPRESS) + LSPRESS;
#else
  return 1013.0;
#endif
}

/**  @brief Get current temperature measuremts.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @return  Temperature in celsius.
*/
double GhGetTemperature(void)
{

#if SIMTEMPERATURE
  return GhGetRandom(USTEMP - LSTEMP) + LSTEMP;
#else
  return 20.0;
#endif
}

/**  @brief Assign sensor values to readings variables.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @return Current sensor values. 
*/
struct readings GhGetReadings(void)
{
  struct readings now = {0};

  now.rtime = time(NULL);
  now.temperature = GhGetTemperature();
  now.humidity = GhGetHumidity();
  now.pressure = GhGetPressure();
  return now;
}

/**  @brief Set heater and humidifier states to on or off based on set values.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @param target Target envirinmental values which the controller must maintain.
 *   @param rdata Current sensor values
 *   @return 
*/
struct controls GhSetControls(struct setpoints target, struct readings rdata)
{
  struct controls cset = {0};

  if (rdata.temperature < target.temperature)
  {
    cset.heater = ON;
  }
  else
  {
    cset.heater = OFF;
  }
  if (rdata.humidity < target.humidity)
  {
    cset.humidifier = ON;
  }
  else
  {
    cset.humidifier = OFF;
  }
  return cset;
}

/**  @brief Set target values for temperature and humidity.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @return a variable of type setpoints holding the environmental constants.
*/
struct setpoints GhSetTargets(void)
{
  struct setpoints cpoints = {0};

  cpoints.temperature = STEMP;
  cpoints.humidity = SHUMID;
  return cpoints;
}

/**  @brief Print environmental targets for temperature and humidity.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @return void 
*/
void GhDisplayTargets(void)
{
  fprintf(stdout, "Targets\tT: %5.1lfC\tH: %5.1lf%%\n", STEMP, SHUMID);
}

/**  @brief Print the current states of heater and humidifier.
 *   @version 25FEB2021
 *   @author Caio Cotts
 *   @param ctrl Holds the current states for heater and humidifier
 *   @return void
*/
void GhDisplayControls(struct controls ctrl)
{
  fprintf(stdout, " Controls\tHeater: %i\tHumidifier: %i\n", ctrl.heater,
          ctrl.humidifier);
}