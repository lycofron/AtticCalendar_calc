/***********************************************************************************************************************************

lunarcalc.c

The purpose of this program is to find all solstices in the years we are interested in.

In detail, the program will scan in the range between startud and stoput and will output, for each year julian dates and dates for:

* Universal solstice (ingression of Sun at Cancer)
* Midnight of attic day when topocentric solstice occurs.
* Topocentric solstice (Athens)
* Midnight of attic day when topocentric solstice occurs.
* Apparent solstice (topocentric - Athens)
* Midnight of attic day when topocentric apparent solstice occurs.

***********************************************************************************************************************************/
#include <math.h>
#include <stdbool.h>
#include "swephexp.h" 	/* this includes  "sweodef.h" */
#include "common.h"
#include "athens.h"

double getClosestTrueNewMoon(double jd, bool topocentric_calc){
  int32 iflag = SEFLG_SWIEPH | SEFLG_SPEED;
  if(topocentric_calc){
    iflag = iflag | SEFLG_TOPOCTR;
  }
  // Accuracy is 10 seconds
  const double min_jd_add = 10.0 / 60.0 / 60.0 / 24.0;
  double jd_add = 1.0;
  double sun_pos[6] = {0.0};
  double lun_pos[6] = {0.0};
  char serr[AS_MAXCH];
  double cur_jd = jd;

  while(fabs(jd_add) > min_jd_add) {
      swe_calc_wrap(cur_jd, SE_SUN, iflag, sun_pos, serr);
      swe_calc_wrap(cur_jd, SE_MOON, iflag, lun_pos, serr);
      // printf("Julian Date: %f\n", cur_jd);
      // printf("Sun :\t%11.7f\t%10.7f\t%10.7f\t%10.7f\n", sun_pos[0], sun_pos[1], sun_pos[2], sun_pos[3]);
      // printf("Moon:\t%11.7f\t%10.7f\t%10.7f\t%10.7f\n", lun_pos[0], lun_pos[1], lun_pos[2], lun_pos[3]);
      jd_add = swe_difdeg2n(sun_pos[0], lun_pos[0])/(lun_pos[3] - sun_pos[3]);
      cur_jd = cur_jd + jd_add;
  }
  return cur_jd;

}

int main()
{
    // Initialization
    swe_set_ephe_path("./ephe/");
    // swe_set_topo(ATHENS_LONGITUDE, ATHENS_LATITUDE, ATHENS_ALTITUDE);

    /* Parameters */
    int startday = 1, startmon = 1, startyear = 2023;
    double startut = 0.0;

    int stopday = 31, stopmon = 12, stopyear = 2024;
    double stoput = 0.0;

    double startdt = swe_julday(startyear,startmon,startday,startut,SE_GREG_CAL);
    startdt = startdt + swe_deltat(startdt);
    double stopjdt = swe_julday(stopyear,stopmon,stopday,stoput,SE_GREG_CAL);
    stopjdt = stopjdt + swe_deltat(stopjdt);

    double new_moon_date;
    char new_moon_date_repr[AS_MAXCH];

    bool do_init = true;
    double first_new_moon = 0.0;
    int new_moons_calced = 0;

    double current_jdt = startdt;

    while(current_jdt < stopjdt){
      new_moon_date = getClosestTrueNewMoon(current_jdt, false);
      formatJulianDate(new_moon_date, new_moon_date_repr);
      printf("%s\n", new_moon_date_repr);
      if (do_init) {
        first_new_moon = new_moon_date;
        current_jdt = current_jdt + 29.53;
        do_init = false;
      } else {
        new_moons_calced++;
        current_jdt = current_jdt + (new_moon_date - first_new_moon) / new_moons_calced;
        // printf("%f\n", (new_moon_date - first_new_moon) / new_moons_calced);
      }
    }




  return OK;
}
