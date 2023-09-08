#include <math.h>
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

double getNextSolstice(double jd) {
    int32 iflag = SEFLG_SPEED;
    double solsticejd;
    char serr[AS_MAXCH];

    solsticejd = swe_solcross_ut(summerSolsticePoint, jd, iflag, serr);
    if (solsticejd < jd) {
      printf("error: %s\n", serr);
    }

    return solsticejd;
}

void calc_solstices(double startjd, double endjd) {

    char label[] = "Summer solstice: ";

    double curjd = startjd;
    double solsticejd;

    while(curjd < endjd) {
        solsticejd = getNextSolstice(curjd);
        printJulianDate(label, solsticejd);
        curjd = solsticejd + 360.0;
    }
}

// double calc_next_sunset(double tjd_ut) {
//   char serr[AS_MAXCH];
//   int32 rsmi=SE_CALC_SET | SE_BIT_HINDU_RISING;
//   int32 epheflag=SEFLG_SWIEPH;
//   int return_code;
//   double tset;
//
//   return_code=swe_rise_trans(tjd_ut, SE_SUN, NULL, epheflag, rsmi, ATHENS_GEOPOS, ATHENS_MEAN_ATPRESSURE,
//     ATHENS_MEAN_TEMPERATURE, &tset, serr);
//   if (return_code == ERR) {
//     // error action
//     printf("%s\n", serr);
//   }
//
//   return tset;
//
// }

void calc_next_sunset(double tjd_ut, double *darr) {
  char serr[AS_MAXCH];
  int32 epheflag=SEFLG_SWIEPH | SE_BIT_HINDU_RISING;
  int return_code;
  char sun_name[AS_MAXCH];

  swe_get_planet_name(SE_SUN, sun_name);

  // ext_def(int32) swe_heliacal_pheno_ut(double tjd_ut, double *geopos, double *datm, double *dobs, char *ObjectName, int32 TypeEvent, int32 helflag, double *darr, char *serr);
  return_code=swe_heliacal_pheno_ut(tjd_ut, ATHENS_GEOPOS, ATHENS_ATMOSPHERE, VOID_OBSERVER, sun_name, SE_HELIACAL_SETTING, epheflag,
    darr, serr);

  if (return_code == ERR) {
    // error action
    printf("%s\n", serr);
  }
}

double getApparentSolsticeDate(double jd_initial){

  // char dateLabel[] = "Sun setting at:";
  // char degreeLabel[] = "\tAzimuth: ";
  double darr[50] = { 0 };
  const int DAYS_AROUND_SOLSTICE = 3;
  const int CONSECUTIVE_DAYS = DAYS_AROUND_SOLSTICE * 2 +1;
  struct time_azimuth {
    double sunset_time;
    double sun_azimuth;
  };
  struct time_azimuth consecutive_sunsets[CONSECUTIVE_DAYS];

  // printf("Longitude\tlatitude\tdistance\tspeed long.\tspeed lat.\n");
  double jd;
  int i = 0;
  for(jd=jd_initial - DAYS_AROUND_SOLSTICE; jd<=jd_initial+DAYS_AROUND_SOLSTICE; jd=jd+0.5){
    calc_next_sunset(jd, darr);
    jd=darr[22];
    consecutive_sunsets[i].sunset_time=darr[22];
    consecutive_sunsets[i].sun_azimuth=darr[5];
    i++;
  }

  for(i=1; i<CONSECUTIVE_DAYS-1; i++){
    if (consecutive_sunsets[i].sun_azimuth < consecutive_sunsets[i+1].sun_azimuth &&
    consecutive_sunsets[i].sun_azimuth < consecutive_sunsets[i+1].sun_azimuth) {
      return consecutive_sunsets[i].sunset_time;
    }
    // printJulianDate(dateLabel, consecutive_sunsets[i].sunset_time);
    // printDegree(degreeLabel, consecutive_sunsets[i].sun_azimuth);
  }

  return 0.0;

}

int main()
{
    // Initialization
    swe_set_ephe_path("./ephe/");
    swe_set_topo(ATHENS_LONGITUDE, ATHENS_LATITUDE, ATHENS_ALTITUDE);

    /* Parameters */
    int startday = 1, startmon = 1, startyear = 2020;
    double startut = 0.0;

    int stopday = 31, stopmon = 12, stopyear = 2030;
    double stoput = 0.0;

    double startdt = swe_julday(startyear,startmon,startday,startut,SE_GREG_CAL);
    startdt = startdt + swe_deltat(startdt);
    double stopjdt = swe_julday(stopyear,stopmon,stopday,stoput,SE_GREG_CAL);
    stopjdt = stopjdt + swe_deltat(stopjdt);

    /* Working-storage section */

    // double x2[6];

    // Get apparent solstice date
    char true_solstice_label[] = "\nTrue Solstice date:";
    char apparent_solstice_label[] = "Apparent Solstice date:";
    double current_jdt = startdt;
    while(current_jdt< stopjdt) {
      double true_solstice = getNextSolstice(current_jdt);
      double apparent_solstice = getApparentSolsticeDate(true_solstice);
      printJulianDate(true_solstice_label, true_solstice);
      printJulianDate(apparent_solstice_label, apparent_solstice);
      current_jdt = current_jdt + 360;
    }

  return OK;
}
