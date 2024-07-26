#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "constants.h"

bool IsBixectil(int year){
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

float* PosSun(int year, int month, int day){
    // Nombre de jours écoulé depuis le 1er janvier 2010 //
    float d = day + 364; 
    float daysInMonth[11] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

    for (unsigned i = 2011; i < year; i++) d += (IsBixectil(i)) ? 366 : 365;
    if(IsBixectil(year)) daysInMonth[1] = 29;
    for(unsigned i = 0; i < month-1; i++) d += daysInMonth[i];
    ///////////////////////////////////////////////////////

    float m = d/365.242191 + LONGITUDE_J2010 + LONGITUDE_PERIGEE; // average anomaly 
    float v = m + 2*EXENTRICITE_ST*sin(m); // true anomaly
    float eclipticLongitude = v + LONGITUDE_PERIGEE;
    float normalizedEclipticLongitude = fmod(eclipticLongitude, 2*PI);
    float angularSize = ANGLE0 * ((1+EXENTRICITE_ST*cos(v))/(1-EXENTRICITE_ST*EXENTRICITE_ST));

    float* result = (float*)malloc(3 * sizeof(float));
    result[0] = (normalizedEclipticLongitude*180)/PI;
    result[1] = 0;
    result[2] = (angularSize*180)/PI;

    return result;
}

int main(){
    float* sun = PosSun(2024,7,26);
    printf("Sun :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n",sun[0],sun[1],sun[2]);
    free(sun);

    return 0;
}
