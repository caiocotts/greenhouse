/**  @brief Code for ghcontrol functions
 *   @file ghcontrol.c
 */
#include "ghcontrol.h"
#include "pisensehat.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**  @brief Delay program for a specific amount of time.
 *   @version 30MAR2021
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
 *   @version 30MAR2021
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
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @param range Holds a value representing the range of two values.
 *   @return
 */
int GhGetRandom(int range) { return rand() % range; }

/**  @brief Print a header with a specified username.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @param  sname Pointer to a string containing a username.
 *   @return void
 */
void GhDisplayHeader(const char *sname)
{
  fprintf(stdout, "%s' Greenhouse Controller\n\n\n", sname);
}

/**  @brief Generate a random seed based on the current time and call
 * GhDisplayHeader.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @return void
 */
void GhControllerInit(void)
{
  srand((unsigned)time(NULL));
  GhDisplayHeader("Caio Cotts");
#if SENSEHAT
  ShInit();
#endif
}

/**  @brief Display current time and sensor readings.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @param rdata   variable holding sensor readings.
 *   @return void
 */
void GhDisplayReadings(reading_s rdata)
{
  fprintf(stdout,
          "\nUnit:%Lx %s Readings\tT: %5.1lfC\tH: %5.1lf%%\tP: %6.1lfmb\n ",
          GhGetSerial(), ctime(&rdata.rtime), rdata.temperature, rdata.humidity,
          rdata.pressure);
}

/**  @brief Get current humidity measurements.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @return Humidity as a percentage.
 */
double GhGetHumidity(void)
{
#if SIMHUMIDITY
  return GhGetRandom(USHUMID - LSHUMID) + LSHUMID;
#else
  ht221sData_s ch = {0};
  ch = ShGetHT221SData();
  return ch.humidity;
#endif
}

/**  @brief Get current pressure measurements.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @return Pressure in millibars.
 */
double GhGetPressure(void)
{
#if SIMPRESSURE
  return GhGetRandom(USPRESS - LSPRESS) + LSPRESS;
#else
  lps25hData_s cp = {0};
  cp = ShGetLPS25HData();
  return cp.pressure;
#endif
}

/**  @brief Get current temperature measuremts.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @return  Temperature in celsius.
 */
double GhGetTemperature(void)
{

#if SIMTEMPERATURE
  return GhGetRandom(USTEMP - LSTEMP) + LSTEMP;
#else
  ht221sData_s ct = {0};
  ct = ShGetHT221SData();
  return ct.temperature;
#endif
}

/**  @brief Assign sensor values to readings variables.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @return Current sensor values.
 */
reading_s GhGetReadings(void)
{
  reading_s now = {0};

  now.rtime = time(NULL);
  now.temperature = GhGetTemperature();
  now.humidity = GhGetHumidity();
  now.pressure = GhGetPressure();
  return now;
}

/**  @brief Set heater and humidifier states to on or off based on set values.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @param target Target envirinmental values which the controller must
 * maintain.
 *   @param rdata Current sensor values
 *   @return a variable of type controls holding heater and humidifier states.
 */
control_s GhSetControls(setpoint_s target, reading_s rdata)
{
  control_s cset = {0};

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

/**  @brief Set target values for temperature and humidity if not already set.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @return a variable of type setpoints holding the environmental constants.
 */
setpoint_s GhSetTargets(void)
{
  setpoint_s cpoints = GhRetrieveSetPoints("setpoints.dat");
  if (cpoints.temperature == 0)
  {
    cpoints.temperature = STEMP;
    cpoints.humidity = SHUMID;
    GhSaveSetPoints("setpoints.dat", cpoints);
  }

  return cpoints;
}

/**  @brief Print environmental targets for temperature and humidity.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @param spts varialbe holding the environmental constants.
 *   @return void
 */
void GhDisplayTargets(setpoint_s spts)
{
  fprintf(stdout, "Targets\tT: %5.1lfC\tH: %5.1lf%%\n", spts.temperature,
          spts.humidity);
}

/**  @brief Print the current states of heater and humidifier.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @param ctrl Holds the current states for heater and humidifier
 *   @return void
 */
void GhDisplayControls(control_s ctrl)
{
  fprintf(stdout, " Controls\tHeater: %i\tHumidifier: %i\n", ctrl.heater,
          ctrl.humidifier);
}

/**  @brief Write output data into a file pointed to by fname.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @param fname points to a file which will hold output data.
 *   @param ghdata holds current time and sensor readings.
 *   @return 1 or 0 depending on the value returned by fopen
 */
int GhLogData(char *fname, reading_s ghdata)
{
  FILE *fp;
  char ltime[CTIMESTRSZ];
  fp = fopen(fname, "a");
  if (fp == NULL)
  {
    return 0;
  }
  strcpy(ltime, ctime(&ghdata.rtime));
  ltime[3] = ',';
  ltime[7] = ',';
  ltime[10] = ',';
  ltime[19] = ',';

  fprintf(fp, "\n%.24s,%5.1lf,%5.1lf,%6.1lf", ltime, ghdata.temperature,
          ghdata.humidity, ghdata.pressure);
  fclose(fp);
  return 1;
}

/**  @brief Write data from spts into a file pointed to by fname.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @param fname points to a file which will hold environmental constants.
 *   @param ghdata holds environmental constants.
 *   @return 1 or 0 depending on the value returned by fopen
 */
int GhSaveSetPoints(char *fname, setpoint_s spts)
{
  FILE *fp;
  fp = fopen(fname, "w");
  if (fp == NULL)
  {
    return 0;
  }
  fwrite(&spts, sizeof(setpoint_s), 1, fp);
  fclose(fp);
  return 1;
}

/**  @brief Read data from a file pointed to by fname and copy it into spts.
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @param fname points to a file which will hold environmental constants.
 *   @return variable holding environmental constants
 */
setpoint_s GhRetrieveSetPoints(char *fname)
{
  setpoint_s spts = {0};
  FILE *fp;
  fp = fopen(fname, "r");
  if (fp == NULL)
  {
    return spts;
  }
  fread(&spts, sizeof(setpoint_s), 1, fp);
  fclose(fp);
  return spts;
}

/**  @brief Display scaled sensor readings and targets on LED matrix
 *   @version 30MAR2021
 *   @author Caio Cotts
 *   @param rd current sensor readings
 *   @param sd current environmental constants
 *   @return void
 */
void GhDisplayAll(reading_s rd, setpoint_s sd)
{
  int rv, sv, avh, avl;
  fbpixel_s pxc = {0};
  ShClearMatrix();
  rv = (NUMPTS * (((rd.temperature - LSTEMP) / (USTEMP - LSTEMP)) + 0.05)) - 1.0;
  pxc.red = 0x00;
  pxc.green = 0xFF;
  pxc.blue = 0x00;
  ShSetVerticalBar(TBAR, pxc, rv);

  rv = (NUMPTS * (((rd.humidity - LSHUMID) / (USHUMID - LSHUMID)) + 0.05)) - 1.0;
  ShSetVerticalBar(HBAR, pxc, rv);

  rv = (NUMPTS * (((rd.pressure - LSPRESS) / (USPRESS - LSPRESS)) + 0.05)) - 1.0;
  ShSetVerticalBar(PBAR, pxc, rv);

  sv = (NUMPTS * (((sd.temperature - LSTEMP) / (USTEMP - LSTEMP)) + 0.05)) - 1.0;
  pxc.red = 0xF0;
  pxc.green = 0x0F;
  pxc.blue = 0xF0;
  ShSetPixel(TBAR, sv, pxc);

  sv = (NUMPTS * (((sd.humidity - LSHUMID) / (USHUMID - LSHUMID)) + 0.05)) - 1.0;
  ShSetPixel(HBAR, sv, pxc);
}