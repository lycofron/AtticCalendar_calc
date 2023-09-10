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

const double ATHENS_LONGITUDE = 23.7268;
const double ATHENS_LATITUDE = 37.9722;
const double ATHENS_ALTITUDE = 70;
double ATHENS_GEOPOS[3] = {ATHENS_LONGITUDE, ATHENS_LATITUDE, ATHENS_ALTITUDE};

const double ATHENS_MEAN_TEMPERATURE = 17.5;
const double ATHENS_MEAN_ATPRESSURE = 1013.25;
const double ATHENS_MEAN_HUMIDITY = 40.0;

double ATHENS_ATMOSPHERE[4] = {ATHENS_MEAN_ATPRESSURE, ATHENS_MEAN_TEMPERATURE, ATHENS_MEAN_HUMIDITY, 0};
double VOID_OBSERVER[] = {36,1,0,0,0,0};

const double summerSolsticePoint = 90.0;

void formatJulianDate(double jd, char * stringBuffer){
    // Format Julian date as follows:
    // Julian date as double, then day/month/year hour:min:sec
    // All columns have fixed width
    int jday, jmon, jyear, jhour, jmin, jsec;
    double jut;

    swe_revjul(jd, SE_GREG_CAL, &jyear, &jmon, &jday, &jut);
    jhour = (int) jut;
    jmin  = (int) ((jut - jhour) * 60);
    jsec  = (int) (((jut - jhour) * 60 - jmin) * 60);
    sprintf(stringBuffer, "% 16.6f %02d/%02d/%-5d %02d:%02d:%02d", jd, jday, jmon, jyear, jhour, jmin, jsec);
}

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

void calc_next_sunset(double tjd_ut, double *darr) {
  // Calculate the next sunset for athens
  // sets the "darr" array, as set by swe_heliacal_pheno_ut sweph function
  char serr[AS_MAXCH];
  int32 epheflag=SEFLG_SWIEPH | SE_BIT_DISC_CENTER;
  int return_code;
  char sun_name[AS_MAXCH];

  swe_get_planet_name(SE_SUN, sun_name);

  return_code=swe_heliacal_pheno_ut(tjd_ut, ATHENS_GEOPOS, ATHENS_ATMOSPHERE, VOID_OBSERVER, sun_name, SE_HELIACAL_SETTING, epheflag,
    darr, serr);

  if (return_code == ERR) {
    // error action
    printf("%s\n", serr);
  }
}

double getApparentSolsticeDate(double jd_initial){
  // The idea of Apparent Solstice is that, during solstice, the sun sets every day further to the north until it reaches the
  // farthest, and then returns to the south. Apparent Solstice is the day when it reaches its farthest point, and the attic day
  // that starts on that sunset (since in the attic calendar, days start at sunset) is the day of the solstice.

  double darr[50] = { 0 };
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
  for(jd=jd_initial - DAYS_AROUND_SOLSTICE; jd<=jd_initial+DAYS_AROUND_SOLSTICE; jd=jd+0.5){
    calc_next_sunset(jd, darr);
    jd=darr[22];
    consecutive_sunsets[i].sunset_time=darr[22];
    consecutive_sunsets[i].sun_azimuth=darr[5];
    // If we have already calculated 3 sunsets, start comparing
    if(i>=2) {
      if (consecutive_sunsets[i-1].sun_azimuth < consecutive_sunsets[i].sun_azimuth &&
          consecutive_sunsets[i-1].sun_azimuth < consecutive_sunsets[i-2].sun_azimuth) {
        return consecutive_sunsets[i].sunset_time;
      }
    }
    i++;
  }

  return 0.0;

}

int getAtticDayMidnight(double jd) {
  // Get the midnight of the attic day for jd given.
  // Since the new attic day begins at sunset, we calculate the next sunset, i.e. the end of the day to which given jd belongs.
  // Then we cut the fractional part and we are left with the midnight of the same (contemporary) date.
  double next_sunset_time;
  double darr[50] = { 0 };
  calc_next_sunset(jd, darr);
  next_sunset_time=darr[22];

  return (int) next_sunset_time;
}

int main()
{
    // Initialization
    swe_set_ephe_path("./ephe/");
    swe_set_topo(ATHENS_LONGITUDE, ATHENS_LATITUDE, ATHENS_ALTITUDE);

    /* Parameters */
    int startday = 1, startmon = 1, startyear = -3000;
    double startut = 0.0;

    int stopday = 31, stopmon = 12, stopyear = 2999;
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
    double current_jdt = startdt;
    while(current_jdt< stopjdt) {
      double true_solstice = getNextSolstice(current_jdt, false);
      int true_solstice_attic_day = getAtticDayMidnight(true_solstice);

      double true_solstice_athens = getNextSolstice(current_jdt, true);
      int true_solstice_athens_attic_day = getAtticDayMidnight(true_solstice);

      // Apparent solstice in Athens makes sense only if we calc by Athens topocentric solstice
      double apparent_solstice = getApparentSolsticeDate(true_solstice_athens);
      // Apparent Solstice is already a sunset. So we just need the next midnight
      int apparent_solstice_attic_day = (int) (apparent_solstice + 1.0);

      formatJulianDate(true_solstice, true_solstice_repr);
      formatJulianDate(true_solstice_attic_day, true_solstice_atticday_repr);
      formatJulianDate(true_solstice_athens, true_solstice_athens_repr);
      formatJulianDate(true_solstice_athens_attic_day, true_solstice_athens_atticday_repr);
      formatJulianDate(apparent_solstice, apparent_solstice_repr);
      formatJulianDate(apparent_solstice_attic_day, apparent_solstice_atticday_repr);
      printf("%s %s %s %s %s %s\n", true_solstice_repr, true_solstice_atticday_repr,
                                    true_solstice_athens_repr, true_solstice_athens_atticday_repr,
                                    apparent_solstice_repr, apparent_solstice_atticday_repr);
      current_jdt = current_jdt + 360;
    }

  return OK;
}
