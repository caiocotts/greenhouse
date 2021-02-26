
#include "ghcontrol.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void GhDelay(int milliseconds) {
  long wait;
  clock_t now, start;

  wait = milliseconds * (CLOCKS_PER_SEC / 1000);
  start = clock();
  now = start;
  while ((now - start) < wait) {
    now = clock();
  }
}

uint64_t GhGetSerial(void) {

  static uint64_t serial = 0;
  FILE *fp;
  char buf[SYSINFOBUFSZ];
  char searchstring[] = SEARCHSTR;
  fp = fopen("/proc/cpuinfo", "r");
  if (fp != NULL) {
    while (fgets(buf, sizeof(buf), fp) != NULL) {
      if (!strncasecmp(searchstring, buf, strlen(searchstring))) {
        sscanf(buf + strlen(searchstring), "%Lx", &serial);
      }
    }
    fclose(fp);
  }
  if (serial == 0) {
    system("uname -a");
    system("ls --fu /usr/lib/codeblocks | grep -Po '\\.\\K[^ ]+'>stamp.txt");
    fp = fopen("stamp.txt", "r");
    if (fp != NULL) {
      while (fgets(buf, sizeof(buf), fp) != NULL) {
        sscanf(buf, "%lu", &serial);
      }
      fclose(fp);
    }
  }
  return serial;
}

int GhGetRandom(int range) { return rand() % range; }

void GhDisplayHeader(const char *sname) {
  fprintf(stdout, "%s' Greenhouse Controller\n\n\n", sname);
}

void GhControllerInit(void) {
  srand((unsigned)time(NULL));
  GhDisplayHeader("Caio Cotts");
}

void GhDisplayReadings(struct readings rdata) {

  fprintf(stdout,
          "\nUnit:%Lx %s Readings\tT: %5.1lfC\tH: %5.1lf%%\tP: %6.1lfmb\n ",
          GhGetSerial(), ctime(&rdata.rtime), rdata.temperature, rdata.humidity,
          rdata.temperature);
}

double GhGetHumidity(void) {
#if SIMHUMIDITY
  return GhGetRandom(USHUMID - LSHUMID) + LSHUMID;
#else
  return 55.0;
#endif
}

double GhGetPressure(void) {
#if SIMPRESSURE
  return GhGetRandom(USPRESS - LSPRESS) + LSPRESS;
#else
  return 1013.0;
#endif
}

double GhGetTemperature(void) {

#if SIMTEMPERATURE
  return GhGetRandom(USTEMP - LSTEMP) + LSTEMP;
#else
  return 20.0;
#endif
}

struct readings GhGetReadings(void) {
  struct readings now = {0};

  now.rtime = time(NULL);
  now.temperature = GhGetTemperature();
  now.humidity = GhGetHumidity();
  now.pressure = GhGetPressure();
  return now;
}

// void GhGetReadings(double readings[SENSORS]) {

//   readings[TEMPERATURE] = GhGetTemperature();
//   readings[HUMIDITY] = GhGetHumidity();
//   readings[PRESSURE] = GhGetPressure();
// }

struct controls GhSetControls(struct setpoints target, struct readings rdata) {
  struct controls cset = {0};

  if (rdata.temperature < STEMP) {
    cset.heater = ON;
  } else {
    cset.heater = OFF;
  }
  if (rdata.humidity < SHUMID) {
    cset.humidifier = ON;
  } else {
    cset.humidifier = OFF;
  }
  return cset;
}

struct setpoints GhSetTargets(void) {
  struct setpoints cpoints = {0};

  cpoints.temperature = STEMP;
  cpoints.humidity = SHUMID;
  return cpoints;
}

void GhDisplayTargets(void) {
  fprintf(stdout, "Targets\tT: %5.1lfC\tH: %5.1lf%%\n", STEMP, SHUMID);
}

void GhDisplayControls(struct controls ctrl) {
  fprintf(stdout, " Controls\tHeater: %i\tHumidifier: %i\n", ctrl.heater,
          ctrl.humidifier);
}