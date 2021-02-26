
#include "ghcontrol.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  time_t now;
  struct controls ctrl = {0};
  struct setpoints sets = {0};
  struct readings creadings = {0};
  sets = GhSetTargets();
  GhControllerInit();

  while (1) {
    now = time(NULL);
    creadings = GhGetReadings();

    GhDisplayReadings(creadings);
    GhDisplayTargets();
    ctrl = GhSetControls(sets, creadings);
    GhDisplayControls(ctrl);

    GhDelay(GHUPDATE);
  }

  // fprintf(stdout, "Press ENTER to continue...");
  // getchar();

  return EXIT_FAILURE;
}