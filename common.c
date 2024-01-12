#include <stdbool.h>
#include "swephexp.h" 	/* this includes  "sweodef.h" */
#include "common.h"
#include "athens.h"

double VOID_OBSERVER[] = {36.0,1.0,0.0,0.0,0.0,0.0};
char EPHE_PATH[] = "./ephe/";

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

void printJulianDate(char * msg, double jd){
    int jday, jmon, jyear, jhour, jmin, jsec;
    double jut;
    swe_revjul(jd, SE_GREG_CAL, &jyear, &jmon, &jday, &jut);
    jhour = (int) floor(jut);
    jmin = (int) floor((jut - jhour) * 60);
    jsec = (int) floor(((jut - jhour) * 60 - jmin) * 60);
    printf("%s%02d/%02d/%d %02d:%02d:%02d \n", msg, jday, jmon, jyear, jhour, jmin, jsec);
}

void printDegree(char * msg, double deg_fl){
    int deg, min;
    double sec;
    deg = (int) floor(deg_fl);
    min = (int) floor((deg_fl - deg) * 60);
    sec = (((deg_fl - deg) * 60) - min) * 60;
    printf("%s%02d˚%02d´%10.7f´´\n", msg, deg, min, sec);
}

void swe_calc_wrap(double tjd_ut, int32 ipl, int32 iflag, double *xx, char *serr) {
  int32 iflgret = 0;
  iflgret = swe_calc_ut(tjd_ut, ipl, iflag, xx, serr);
  // if there is a problem, a negative value is returned and an error message is in serr.
  if (iflgret < 0) {
    printf("error: %s\n", serr);
    // continue;
  }
  if (iflgret != iflag) {
    printf("warning: iflgret != iflag. %d != %d - %s\n", iflgret, iflag, serr);
  }
}

double calc_next_sun_something(double tjd_ut, bool disc_center, int what_to_calc) {
  // Calculate the next sunset for athens
  // sets the "darr" array, as set by swe_heliacal_pheno_ut sweph function
  int return_code;
  int32 epheflag = SEFLG_SWIEPH;
  if(disc_center) {
    epheflag = epheflag | SE_BIT_DISC_CENTER;
  }
  char serr[AS_MAXCH];
  double tret;

  return_code = swe_rise_trans(tjd_ut, SE_SUN, NULL, epheflag, what_to_calc, ATHENS_GEOPOS, ATHENS_MEAN_ATPRESSURE,
                               ATHENS_MEAN_TEMPERATURE, &tret, serr);

  if (return_code == ERR) {
    // error action
    printf("%s\n", serr);
  }

  return tret;
}

double calc_next_sunrise(double tjd_ut, bool disc_center) {
  return calc_next_sun_something(tjd_ut, disc_center, SE_CALC_RISE);
}

double calc_next_sunset(double tjd_ut, bool disc_center) {
  return calc_next_sun_something(tjd_ut, disc_center, SE_CALC_SET);
}


double getAtticDayMidnight(double jd) {
  // Get the midnight of the attic day for jd given.
  // Since the new attic day begins at sunset, we calculate the next sunset, i.e. the end of the day to which given jd belongs.
  // Then we cut the fractional part and we are left with the midnight of the same (contemporary) date.

  return floor(calc_next_sunset(jd, true)) - 0.5;
}
