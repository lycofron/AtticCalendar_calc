#ifndef _ATTIC_CALENDAR_COMMON_INCLUDED
#define _ATTIC_CALENDAR_COMMON_INCLUDED

extern char EPHE_PATH[];
extern double VOID_OBSERVER[6];

void formatJulianDate(double jd, char * stringBuffer);
void printJulianDate(char * msg, double jd);
void printDegree(char * msg, double deg_fl);
void swe_calc_wrap(double tjd_ut, int32 ipl, int32 iflag, double *xx, char *serr);
double calc_next_sunset(double tjd_ut, bool disc_center);
double calc_next_sunrise(double tjd_ut, bool disc_center);
double getAtticDayMidnight(double jd);

#endif
