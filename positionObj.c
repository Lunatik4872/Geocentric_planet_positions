#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "constants.h"

bool IsBixectil(int year){
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

float DaySinceJ2010(int year, int month, int day){
    float d = day + 364; 
    float daysInMonth[11] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

    for (unsigned i = 2011; i < year; i++) d += (IsBixectil(i)) ? 366 : 365;
    if(IsBixectil(year)) daysInMonth[1] = 29;
    for(unsigned i = 0; i < month-1; i++) d += daysInMonth[i];

    return d;
}

float* PosSun(int year, int month, int day){

    float d = DaySinceJ2010(year,month,day);

    float m = d/365.242191 + LONGITUDE_J2010 + LONGITUDE_PERIGEE; // average anomaly 
    float v = m + 2*EXENTRICITE_ST*sinf(m); // true anomaly
    float eclipticLongitude = v + LONGITUDE_PERIGEE;
    float normalizedEclipticLongitude = fmodf(eclipticLongitude, 2*PI);
    float angularSize = ANGLE0 * ((1+EXENTRICITE_ST*cos(v))/(1-EXENTRICITE_ST*EXENTRICITE_ST));

    float* result = (float*)malloc(3 * sizeof(float));
    result[0] = (normalizedEclipticLongitude*180)/PI;
    result[1] = 0;
    result[2] = (angularSize*180)/PI;

    return result;
}

float HigherPlanets(float eclipticRayProjection, float heliocentricEclipticLongitude, float longitudeEarth, float radiusEarth){
    return heliocentricEclipticLongitude+atanf((radiusEarth*sinf(heliocentricEclipticLongitude-longitudeEarth))/(eclipticRayProjection-radiusEarth*cosf(heliocentricEclipticLongitude-longitudeEarth)));
}

float LowerPlanets(float eclipticRayProjection, float heliocentricEclipticLongitude, float longitudeEarth, float radiusEarth){
    return PI+longitudeEarth+atanf((eclipticRayProjection*sinf(longitudeEarth-heliocentricEclipticLongitude))/(radiusEarth-eclipticRayProjection*cosf(longitudeEarth-heliocentricEclipticLongitude)));
}

float* PosPlanetGeocentric(int year, int month, int day, float Tp, float epsi, float w, float e, float a, float i, float omega, float angle0, float v0, bool lowPlanets){

    float d = DaySinceJ2010(year,month,day);

    float m = d/365.242191 + epsi + w; // average anomaly 
    float v = m + 2*e*sinf(m); // true anomaly

    float radius = (a*(1-e*e))/(1+e*cosf(v));
    float longitude = v + w;
    float heliocentricEclipticLatitude = asinf(sinf(longitude-omega)*sinf(i));

    float eclipticRayProjection = radius*cosf(heliocentricEclipticLatitude);
    float heliocentricEclipticLongitude = atanf((sinf(longitude-omega)*cosf(i))/(cosf(longitude-omega)))+omega;

    float mEarth = d/365.242191 + LONGITUDE_J2010_EA + LONGITUDE_PERIGEE_EA;
    float vEarth = mEarth + 2*EXENTRICITE_ST_EA*sinf(mEarth);
    float radiusEarth = (HALF_WIDE_AXIS_ORBIT_EA*(1-EXENTRICITE_ST_EA*EXENTRICITE_ST_EA))/(1+EXENTRICITE_ST_EA*cosf(vEarth));
    float longitudeEarth = vEarth + LONGITUDE_PERIGEE_EA;

    float geocentricEclipticLongitude = (lowPlanets) ? LowerPlanets(eclipticRayProjection,heliocentricEclipticLongitude,longitudeEarth,radiusEarth) : HigherPlanets(eclipticRayProjection,heliocentricEclipticLongitude,longitudeEarth,radiusEarth);
    float geocentricEclipticLatitude = atanf((eclipticRayProjection*tanf(heliocentricEclipticLatitude)*sinf(geocentricEclipticLongitude-heliocentricEclipticLongitude))/(radiusEarth*sinf(heliocentricEclipticLongitude-longitudeEarth)));
    float p = radiusEarth*radiusEarth+radius*radius-2*radiusEarth*radius*cosf(longitude-longitudeEarth)*cosf(heliocentricEclipticLatitude);
    float angularSize = angle0/sqrtf(p);
    float f = (1+cosf(geocentricEclipticLongitude-longitude))/2;
    float magnitude = v0+5*log10f((radius*sqrtf(p))/(sqrtf(f)));

    float* result = (float*)malloc(4 * sizeof(float));
    result[0] = (fmodf(geocentricEclipticLongitude, 2*PI)*180)/PI;
    result[1] = (fmodf(geocentricEclipticLatitude, 2*PI)*180)/PI;
    result[2] = (fmodf(angularSize, 2*PI)*180)/PI;
    result[3] = magnitude;

    return result;
}

int main(){
    float* sun = PosSun(2024,7,27);
    float* mercure = PosPlanetGeocentric(2024,7,27,TROPICAL_YEAR_ME,LONGITUDE_J2010_ME,LONGITUDE_PERIGEE_ME,EXENTRICITE_ST_ME,HALF_WIDE_AXIS_ORBIT_ME,ECLIPTIC_ORBIT_INCLINATION_ME,LONGITUDE_ASCENDING_NODE_ME,ANGLE0_ME,MAGNITUDE_1UA_ME,true);
    float* venus = PosPlanetGeocentric(2024,7,27,TROPICAL_YEAR_VE,LONGITUDE_J2010_VE,LONGITUDE_PERIGEE_VE,EXENTRICITE_ST_VE,HALF_WIDE_AXIS_ORBIT_VE,ECLIPTIC_ORBIT_INCLINATION_VE,LONGITUDE_ASCENDING_NODE_VE,ANGLE0_VE,MAGNITUDE_1UA_VE,true);
    float* march = PosPlanetGeocentric(2024,7,27,TROPICAL_YEAR_MA,LONGITUDE_J2010_MA,LONGITUDE_PERIGEE_MA,EXENTRICITE_ST_MA,HALF_WIDE_AXIS_ORBIT_MA,ECLIPTIC_ORBIT_INCLINATION_MA,LONGITUDE_ASCENDING_NODE_MA,ANGLE0_MA,MAGNITUDE_1UA_MA,false);
    float* jupiter = PosPlanetGeocentric(2024,7,27,TROPICAL_YEAR_JU,LONGITUDE_J2010_JU,LONGITUDE_PERIGEE_JU,EXENTRICITE_ST_JU,HALF_WIDE_AXIS_ORBIT_JU,ECLIPTIC_ORBIT_INCLINATION_JU,LONGITUDE_ASCENDING_NODE_JU,ANGLE0_JU,MAGNITUDE_1UA_JU,false);
    float* saturn = PosPlanetGeocentric(2024,7,27,TROPICAL_YEAR_SA,LONGITUDE_J2010_SA,LONGITUDE_PERIGEE_SA,EXENTRICITE_ST_SA,HALF_WIDE_AXIS_ORBIT_SA,ECLIPTIC_ORBIT_INCLINATION_SA,LONGITUDE_ASCENDING_NODE_SA,ANGLE0_SA,MAGNITUDE_1UA_SA,false);
    float* uranus = PosPlanetGeocentric(2024,7,27,TROPICAL_YEAR_UR,LONGITUDE_J2010_UR,LONGITUDE_PERIGEE_UR,EXENTRICITE_ST_UR,HALF_WIDE_AXIS_ORBIT_UR,ECLIPTIC_ORBIT_INCLINATION_UR,LONGITUDE_ASCENDING_NODE_UR,ANGLE0_UR,MAGNITUDE_1UA_UR,false);
    float* neptune = PosPlanetGeocentric(2024,7,27,TROPICAL_YEAR_NE,LONGITUDE_J2010_NE,LONGITUDE_PERIGEE_NE,EXENTRICITE_ST_NE,HALF_WIDE_AXIS_ORBIT_NE,ECLIPTIC_ORBIT_INCLINATION_NE,LONGITUDE_ASCENDING_NODE_NE,ANGLE0_NE,MAGNITUDE_1UA_NE,false);

    printf("Geocentric position :\n");
    printf("Sun :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n\n",sun[0],sun[1],sun[2]);
    printf("Mercure :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",mercure[0],mercure[1],mercure[2],mercure[3]);
    printf("Venus :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",venus[0],venus[1],venus[2],venus[3]);
    printf("Earth :\n   longitude : 0\n   latitude : 0\n   taille angulaire : 0\n   magnitude : 0\n\n");
    printf("March :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",march[0],march[1],march[2],march[3]);
    printf("Jupiter :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",jupiter[0],jupiter[1],jupiter[2],jupiter[3]);
    printf("Saturn :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",saturn[0],saturn[1],saturn[2],saturn[3]);
    printf("Uranus :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",uranus[0],uranus[1],uranus[2],uranus[3]);
    printf("Neptune :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",neptune[0],neptune[1],neptune[2],neptune[3]);

    free(sun);
    free(mercure);
    free(venus);
    free(march);
    free(jupiter);
    free(saturn);
    free(uranus);
    free(neptune);

    return 0;
}
