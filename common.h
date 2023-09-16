#ifndef _ATTIC_CALENDAR_COMMON_INCLUDED
#define _ATTIC_CALENDAR_COMMON_INCLUDED

extern double VOID_OBSERVER[6];

void formatJulianDate(double jd, char * stringBuffer);
void calc_next_sunset(double tjd_ut, double *darr, bool disc_center);
int getAtticDayMidnight(double jd);

#endif
