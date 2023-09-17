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

double getNextApparentNewMoon(double jd){
  int32 iflgret = 0;
  double dret[50];
  char serr[AS_MAXCH];
  char moon_name[20];
  int32 iflag = SEFLG_SWIEPH | SEFLG_SPEED;

  swe_get_planet_name(SE_MOON, moon_name);
  iflgret = swe_heliacal_ut( jd, ATHENS_GEOPOS, ATHENS_ATMOSPHERE, VOID_OBSERVER, moon_name, SE_EVENING_FIRST, iflag, dret, serr);
  if (iflgret < 0) {
    printf("error: %s\n", serr);
    // continue;
  }

  return dret[0];

}

int main()
{
    // Initialization
    swe_set_ephe_path("./ephe/");
    double const SYNODIC_MONTH = 29.53;
    // swe_set_topo(ATHENS_LONGITUDE, ATHENS_LATITUDE, ATHENS_ALTITUDE);

    /* Parameters */
    int startday = 1, startmon = 1, startyear = -3000;
    double startut = 0.0;

    int stopday = 31, stopmon = 12, stopyear = 2998;
    double stoput = 0.0;

    double startdt = swe_julday(startyear,startmon,startday,startut,SE_GREG_CAL);
    startdt = startdt + swe_deltat(startdt);
    double stopjdt = swe_julday(stopyear,stopmon,stopday,stoput,SE_GREG_CAL);
    stopjdt = stopjdt + swe_deltat(stopjdt);

    double new_moon_date, new_moon_date_attic_day;
    char new_moon_date_repr[AS_MAXCH], new_moon_date_attic_day_repr[AS_MAXCH];

    double new_moon_athens_date, new_moon_athens_date_attic_day;
    char new_moon_athens_date_repr[AS_MAXCH], new_moon_athens_date_attic_day_repr[AS_MAXCH];

    double noumenia_date, noumenia_date_attic_day;
    char noumenia_date_repr[AS_MAXCH], noumenia_date_attic_day_repr[AS_MAXCH];

    double current_jdt = startdt;

    for(current_jdt = startdt; current_jdt< stopjdt; current_jdt = current_jdt + SYNODIC_MONTH) {
      new_moon_date = getClosestTrueNewMoon(current_jdt, false);
      new_moon_date_attic_day = getAtticDayMidnight(new_moon_date);
      new_moon_athens_date = getClosestTrueNewMoon(current_jdt, true);
      new_moon_athens_date_attic_day = getAtticDayMidnight(new_moon_athens_date);
      noumenia_date = getNextApparentNewMoon(new_moon_date);
      //No need for complex calculations. Noumenia is close after sunset, therefore, attic day is always following Midnight.
      noumenia_date_attic_day = floor(noumenia_date) + 0.5;

      formatJulianDate(new_moon_date, new_moon_date_repr);
      formatJulianDate(new_moon_date_attic_day, new_moon_date_attic_day_repr);
      formatJulianDate(new_moon_athens_date, new_moon_athens_date_repr);
      formatJulianDate(new_moon_athens_date_attic_day, new_moon_athens_date_attic_day_repr);
      formatJulianDate(noumenia_date, noumenia_date_repr);
      formatJulianDate(noumenia_date_attic_day, noumenia_date_attic_day_repr);
      printf("%s %s %s %s %s %s\n", new_moon_date_repr, new_moon_date_attic_day_repr, new_moon_athens_date_repr,
             new_moon_athens_date_attic_day_repr, noumenia_date_repr, noumenia_date_attic_day_repr);
    }

  return OK;
}
