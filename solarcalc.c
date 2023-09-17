/***********************************************************************************************************************************

solarcalc.c

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

const double summerSolsticePoint = 90.0;

double getNextSolstice(double jd, bool topocentric_calc) {
    // Calculate the next Summer Solstice, starting from jd
    // If topocentric_calc is set, the calculation will be topocentric, for Athens
    int32 iflag = SEFLG_SPEED;
    if(topocentric_calc){
      iflag = SEFLG_TOPOCTR;
    }
    double solsticejd;
    char serr[AS_MAXCH];

    solsticejd = swe_solcross_ut(summerSolsticePoint, jd, iflag, serr);
    if (solsticejd < jd) {
      printf("error: %s\n", serr);
    }

    return solsticejd;
}


double get_sun_azimuth(double tjd){
  int32 iflag = SEFLG_SWIEPH | SEFLG_TOPOCTR;
  double xin[3], xaz[3];
  char serr[AS_MAXCH];
  swe_calc_wrap(tjd, SE_SUN, iflag, xin, serr);
  swe_azalt(tjd, SE_ECL2HOR, ATHENS_GEOPOS, ATHENS_MEAN_ATPRESSURE, ATHENS_MEAN_TEMPERATURE, xin, xaz);
  return xaz[0];
}


double getApparentSolsticeDate(double jd_initial){
  // The idea of Apparent Solstice is that, during solstice, the sun sets every day further to the north until it reaches the
  // farthest, and then returns to the south. Apparent Solstice is the day when it reaches its farthest point, and the attic day
  // that starts on that sunset (since in the attic calendar, days start at sunset) is the day of the solstice.

  // investigate a zone of that many days before and after solstice.
  // It would have been set to 2, but because of a bug (?) in sweph, we need to cancel out the first calculation. So it is set to 3.
  const int DAYS_AROUND_SOLSTICE = 3;
  const int CONSECUTIVE_DAYS = DAYS_AROUND_SOLSTICE * 2 +1;
  struct time_azimuth {
    double sunset_time;
    double sun_azimuth;
  };
  struct time_azimuth consecutive_sunsets[CONSECUTIVE_DAYS];

  double jd;
  int i = 0;

  // char dateLabel[] = "Sun setting at:";
  // char degreeLabel[] = "\tAzimuth: ";

  for(jd=jd_initial - DAYS_AROUND_SOLSTICE; jd<=jd_initial+DAYS_AROUND_SOLSTICE; jd=jd+0.5){
    jd = calc_next_sunset(jd, true);
    consecutive_sunsets[i].sunset_time = jd;
    consecutive_sunsets[i].sun_azimuth = get_sun_azimuth(jd);
    // printJulianDate(dateLabel, consecutive_sunsets[i].sunset_time);
    // printDegree(degreeLabel, consecutive_sunsets[i].sun_azimuth);
    // If we have already calculated 3 sunsets, start comparing
    if(i>=2) {
      if (consecutive_sunsets[i-1].sun_azimuth > consecutive_sunsets[i].sun_azimuth &&
          consecutive_sunsets[i-1].sun_azimuth > consecutive_sunsets[i-2].sun_azimuth) {
        return consecutive_sunsets[i].sunset_time;
      }
    }
    i++;
  }

  return 0.0;

}

int main()
{
    // Initialization
    swe_set_ephe_path(EPHE_PATH);
    swe_set_topo(ATHENS_LONGITUDE, ATHENS_LATITUDE, ATHENS_ALTITUDE);

    /* Parameters */
    int startday = 1, startmon = 1, startyear = -3000;
    double startut = 0.0;

    int stopday = 31, stopmon = 12, stopyear = 2998;
    double stoput = 0.0;

    double startdt = swe_julday(startyear,startmon,startday,startut,SE_GREG_CAL);
    startdt = startdt + swe_deltat(startdt);
    double stopjdt = swe_julday(stopyear,stopmon,stopday,stoput,SE_GREG_CAL);
    stopjdt = stopjdt + swe_deltat(stopjdt);

    char true_solstice_repr[AS_MAXCH];
    char true_solstice_athens_repr[AS_MAXCH];
    char apparent_solstice_repr[AS_MAXCH];
    char true_solstice_atticday_repr[AS_MAXCH];
    char true_solstice_athens_atticday_repr[AS_MAXCH];
    char apparent_solstice_atticday_repr[AS_MAXCH];

    printf("True Solstice (Universal)                                                  True Solstice (Athens)                                                    Apparent Solstice (Athens)\n");
    double current_jdt;

    double true_solstice;
    double true_solstice_athens;
    double apparent_solstice;

    double true_solstice_attic_day;
    double true_solstice_athens_attic_day;
    double apparent_solstice_attic_day;


    for(current_jdt = startdt; current_jdt< stopjdt; current_jdt = current_jdt + 360) {
      true_solstice = getNextSolstice(current_jdt, false);
      true_solstice_attic_day = getAtticDayMidnight(true_solstice);

      true_solstice_athens = getNextSolstice(current_jdt, true);
      true_solstice_athens_attic_day = getAtticDayMidnight(true_solstice);

      // Apparent solstice in Athens makes sense only if we calc by Athens topocentric solstice
      apparent_solstice = getApparentSolsticeDate(true_solstice_athens);
      // Apparent Solstice is already a sunset. So we just need the next midnight
      apparent_solstice_attic_day = floor(apparent_solstice) + 0.5;

      formatJulianDate(true_solstice, true_solstice_repr);
      formatJulianDate(true_solstice_attic_day, true_solstice_atticday_repr);
      formatJulianDate(true_solstice_athens, true_solstice_athens_repr);
      formatJulianDate(true_solstice_athens_attic_day, true_solstice_athens_atticday_repr);
      formatJulianDate(apparent_solstice, apparent_solstice_repr);
      formatJulianDate(apparent_solstice_attic_day, apparent_solstice_atticday_repr);
      printf("%s %s %s %s %s %s\n", true_solstice_repr, true_solstice_atticday_repr,
                                    true_solstice_athens_repr, true_solstice_athens_atticday_repr,
                                    apparent_solstice_repr, apparent_solstice_atticday_repr);
    }

  return OK;
}
