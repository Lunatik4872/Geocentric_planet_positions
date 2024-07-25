#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//Constant clacul sun position
#define LONGITUDE_J2010 4.8791937
#define LONGITUDE_PERIGEE 4.941244
#define EXENTRICITE_ST 0.0002916
#define PI 3.1415927
#define ANGLE0 0.0093047

bool IsBixectil(int year){
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

float* PosSun(int year, int month, int day){
    // Nombre de jours écoulé depuis le 1er janvier 2010 //
    float d = day + 364; 
    float daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    for (unsigned i = 2011; i < year; i++) d += (IsBixectil(i)) ? 366 : 365;
    if(IsBixectil(year)) daysInMonth[1] = 29;
    for(unsigned i = 0; i < month - 1; i++) d += daysInMonth[i];
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

    printf("longitude : %f\ntaille angulaire : %f\n",result[0],result[2]);
    return result;
}


int main(){
    PosSun(2024,7,25);
    return 0;
}