/**  @brief Main program that call and use all the functions and constants in ghcontrol.c and .h
 *   @file ghc.c
 */
#include "ghcontrol.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  struct controls ctrl = {0};
  struct setpoints sets = {0};
  struct readings creadings = {0};
  sets = GhSetTargets();
  GhControllerInit();
  int logged;

  while (1)
  {
    creadings = GhGetReadings();
    logged = GhLogData("ghdata.txt", creadings);

    GhDisplayReadings(creadings);
    GhDisplayTargets(sets);
    ctrl = GhSetControls(sets, creadings);
    GhDisplayControls(ctrl);

    GhDelay(GHUPDATE);
  }

  return EXIT_FAILURE;
}