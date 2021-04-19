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
  alarm_s *arecord;
  arecord = (alarm_s *)calloc(1, sizeof(alarm_s));
  if (arecord == NULL)
  {
    puts("Cannot allocate memory");
  }

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
    arecord = GhSetAlarms(arecord, alimits, creadings);
    GhDisplayControls(ctrl);
    GhDisplayAlarms(arecord);
    GhDelay(GHUPDATE);
  }

  return EXIT_FAILURE;
}