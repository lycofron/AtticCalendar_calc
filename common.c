#include <stdbool.h>
#include "swephexp.h" 	/* this includes  "sweodef.h" */
#include "common.h"
#include "athens.h"

double VOID_OBSERVER[] = {36.0,1.0,0.0,0.0,0.0,0.0};

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

void calc_next_sunset(double tjd_ut, double *darr, bool disc_center) {
  // Calculate the next sunset for athens
  // sets the "darr" array, as set by swe_heliacal_pheno_ut sweph function
  char serr[AS_MAXCH];
  int32 epheflag = SEFLG_SWIEPH;
  if(disc_center) {
    epheflag = epheflag | SE_BIT_DISC_CENTER;
  }
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

int getAtticDayMidnight(double jd) {
  // Get the midnight of the attic day for jd given.
  // Since the new attic day begins at sunset, we calculate the next sunset, i.e. the end of the day to which given jd belongs.
  // Then we cut the fractional part and we are left with the midnight of the same (contemporary) date.
  double next_sunset_time;
  double darr[50] = { 0 };
  calc_next_sunset(jd, darr, true);
  next_sunset_time=darr[22];

  return (int) next_sunset_time;
}
