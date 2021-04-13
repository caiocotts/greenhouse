/**  @brief Main program that call and use all the functions and constants in
 * ghcontrol.c and .h
 *   @file ghc.c
 */
#include "ghcontrol.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  control_s ctrl = {0};
  setpoint_s sets = {0};
  reading_s creadings = {0};
  alarmlimit_s alimits = {0};
  alarm_s warn[NALARMS] = {0};
  sets = GhSetTargets();
  alimits = GhSetAlarmLimits();
  GhControllerInit();
  int logged;

  while (1)
  {
    creadings = GhGetReadings();
    logged = GhLogData("ghdata.txt", creadings);
    GhDisplayAll(creadings, sets);
    GhDisplayReadings(creadings);
    GhDisplayTargets(sets);
    ctrl = GhSetControls(sets, creadings);
    GhSetAlarms(warn, alimits, creadings);
    GhDisplayControls(ctrl);
    GhDisplayAlarms(warn);
    GhDelay(GHUPDATE);
  }

  return EXIT_FAILURE;
}