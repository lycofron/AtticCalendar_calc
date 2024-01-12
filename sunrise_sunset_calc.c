#include <stdbool.h>
#include "swephexp.h" 	/* this includes  "sweodef.h" */
#include "common.h"
#include "athens.h"

int main()
{
    // Initialization
    swe_set_ephe_path(EPHE_PATH);
    swe_set_topo(ATHENS_LONGITUDE, ATHENS_LATITUDE, ATHENS_ALTITUDE);

    /* Parameters */
    int startday = 6, startmon = 7, startyear = 2024;
    double startut = 0.0;

    int stopday = 27, stopmon = 6, stopyear = 2025;
    double stoput = 0.0;

    double startdt = swe_julday(startyear,startmon,startday,startut,SE_GREG_CAL);
    startdt = startdt + swe_deltat(startdt);
    double stopjdt = swe_julday(stopyear,stopmon,stopday,stoput,SE_GREG_CAL);
    stopjdt = stopjdt + swe_deltat(stopjdt);

    double current_jdt = startdt;
    double next_sunrise;
    double next_sunset;
    // char next_sunrise_repr[AS_MAXCH];
    // char next_sunset_repr[AS_MAXCH];

    while(current_jdt < stopjdt){
      next_sunset=calc_next_sunset(current_jdt, false);
      next_sunrise=calc_next_sunrise(next_sunset, false);
      current_jdt = next_sunrise;
      // formatJulianDate(next_sunset, next_sunset_repr);
      // formatJulianDate(next_sunrise, next_sunrise_repr);
      printf("%f\t%f\n", next_sunset, next_sunrise);
    }

}
