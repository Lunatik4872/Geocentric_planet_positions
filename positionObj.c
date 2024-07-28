#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "constants.h"

struct Planet{
    float tropical_year;
    float longitude_j2010;
    float longitude_perigee;
    float exentricite_st;
    float half_wide_axis_orbit;
    float ecliptic_orbit_inclination;
    float longitude_ascending_node;
    float angle0;
    float magnitude_1ua;
    bool is_inner_planet;
};

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

    float m = d/365.242191 + LONGITUDE_J2010 - LONGITUDE_PERIGEE; // average anomaly 
    float v = m + 2 * EXENTRICITE_ST * sinf(m); // true anomaly
    float eclipticLongitude = v + LONGITUDE_PERIGEE;
    float normalizedEclipticLongitude = fmodf(eclipticLongitude, 2*PI);
    float angularSize = ANGLE0 * ((1+EXENTRICITE_ST*cos(v))/(1-EXENTRICITE_ST*EXENTRICITE_ST));

    float* result = (float*)malloc(3 * sizeof(float));
    result[0] = (normalizedEclipticLongitude*180)/PI;
    result[1] = 0;
    result[2] = angularSize;

    return result;
}

float CalculBV(float eV, float eB, float c){
    float mv = -2.5*log10f(eV)+c;
    float mb = -2.5*log10f(eB)+c;

    return mb-mv;
}

float ColorStar(float indiceColorBV){
    return 4600*(1/(0.92 * indiceColorBV + 1.7) + 1/(0.92 * indiceColorBV + 0.62));
}

float HigherPlanets(float eclipticRayProjection, float heliocentricEclipticLongitude, float longitudeEarth, float radiusEarth){
    return heliocentricEclipticLongitude+atanf((radiusEarth*sinf(heliocentricEclipticLongitude-longitudeEarth))/(eclipticRayProjection-radiusEarth*cosf(heliocentricEclipticLongitude-longitudeEarth)));
}

float LowerPlanets(float eclipticRayProjection, float heliocentricEclipticLongitude, float longitudeEarth, float radiusEarth){
    return PI+longitudeEarth+atanf((eclipticRayProjection*sinf(longitudeEarth-heliocentricEclipticLongitude))/(radiusEarth-eclipticRayProjection*cosf(longitudeEarth-heliocentricEclipticLongitude)));
}

float* PosPlanetGeocentric(int year, int month, int day, Planet *planet){

    float d = DaySinceJ2010(year,month,day);

    float m = 1/365.242191 * d/planet->tropical_year + planet->longitude_j2010 - planet->longitude_perigee; // average anomaly 
    float v = m + 2*planet->exentricite_st*sinf(m); // true anomaly

    float radius = (planet->half_wide_axis_orbit*(1-planet->exentricite_st*planet->exentricite_st))/(1+planet->exentricite_st*cosf(v));
    float longitude = v + planet->longitude_perigee;
    float heliocentricEclipticLatitude = asinf(sinf(longitude - planet->longitude_ascending_node) * sinf(planet->ecliptic_orbit_inclination));

    float eclipticRayProjection = radius*cosf(heliocentricEclipticLatitude);
    float heliocentricEclipticLongitude = atanf((sinf(longitude - planet->longitude_ascending_node) * cosf(planet->ecliptic_orbit_inclination)) / (cosf(longitude - planet->longitude_ascending_node))) + planet->longitude_ascending_node;

    float mEarth = 1/365.242191 * d/TROPICAL_YEAR_EA + LONGITUDE_J2010_EA - LONGITUDE_PERIGEE_EA;
    float vEarth = mEarth + 2 * EXENTRICITE_ST_EA * sinf(mEarth);
    float radiusEarth = (HALF_WIDE_AXIS_ORBIT_EA * (1 - EXENTRICITE_ST_EA * EXENTRICITE_ST_EA)) / (1 + EXENTRICITE_ST_EA * cosf(vEarth));
    float longitudeEarth = vEarth + LONGITUDE_PERIGEE_EA;

    float geocentricEclipticLongitude = (planet->is_inner_planet) ? LowerPlanets(eclipticRayProjection,heliocentricEclipticLongitude,longitudeEarth,radiusEarth) : HigherPlanets(eclipticRayProjection,heliocentricEclipticLongitude,longitudeEarth,radiusEarth);
    float geocentricEclipticLatitude = atanf((eclipticRayProjection * tanf(heliocentricEclipticLatitude) * sinf(geocentricEclipticLongitude - heliocentricEclipticLongitude)) / (radiusEarth * sinf(heliocentricEclipticLongitude - longitudeEarth)));
    float p = radiusEarth * radiusEarth + radius * radius - 2 * radiusEarth * radius * cosf(longitude-longitudeEarth) * cosf(heliocentricEclipticLatitude);
    float angularSize = planet->angle0 / sqrtf(p);
    float f = (1 + cosf(geocentricEclipticLongitude - longitude)) / 2;
    float magnitude = planet->magnitude_1ua + 5 * log10f((radius * sqrtf(p)) / (sqrtf(f)));

    float* result = (float*)malloc(4 * sizeof(float));
    result[0] = (fmodf(geocentricEclipticLongitude, 2*PI)*180)/PI;
    result[1] = (fmodf(geocentricEclipticLatitude, 2*PI)*180)/PI;
    result[2] = angularSize;
    result[3] = magnitude;

    return result;
}

Planet* creatPlanet(float tropical_year, float longitude_j2010, float longitude_perigee, float exentricite_st, float half_wide_axis_orbit, float ecliptic_orbit_inclination, float longitude_ascending_node, float angle0, float magnitude_1ua, bool is_inner_planet){
    Planet* new_planet = malloc(sizeof(Planet));
    if(new_planet == NULL) {
        return NULL;
    }

    new_planet->tropical_year = tropical_year;
    new_planet->longitude_j2010 = longitude_j2010;
    new_planet->longitude_perigee = longitude_perigee;
    new_planet->exentricite_st = exentricite_st;
    new_planet->half_wide_axis_orbit = half_wide_axis_orbit;
    new_planet->ecliptic_orbit_inclination = ecliptic_orbit_inclination;
    new_planet->longitude_ascending_node = longitude_ascending_node;
    new_planet->angle0 = angle0;
    new_planet->magnitude_1ua = magnitude_1ua;
    new_planet->is_inner_planet = is_inner_planet;

    return new_planet;
}

void destroyPlanet(Planet *planet){
    free(planet);
}

int main(){

    Planet *mercure = creatPlanet(TROPICAL_YEAR_ME,LONGITUDE_J2010_ME,LONGITUDE_PERIGEE_ME,EXENTRICITE_ST_ME,HALF_WIDE_AXIS_ORBIT_ME,ECLIPTIC_ORBIT_INCLINATION_ME,LONGITUDE_ASCENDING_NODE_ME,ANGLE0_ME,MAGNITUDE_1UA_ME,true);
    Planet *venus = creatPlanet(TROPICAL_YEAR_VE,LONGITUDE_J2010_VE,LONGITUDE_PERIGEE_VE,EXENTRICITE_ST_VE,HALF_WIDE_AXIS_ORBIT_VE,ECLIPTIC_ORBIT_INCLINATION_VE,LONGITUDE_ASCENDING_NODE_VE,ANGLE0_VE,MAGNITUDE_1UA_VE,true);
    Planet *march = creatPlanet(TROPICAL_YEAR_MA,LONGITUDE_J2010_MA,LONGITUDE_PERIGEE_MA,EXENTRICITE_ST_MA,HALF_WIDE_AXIS_ORBIT_MA,ECLIPTIC_ORBIT_INCLINATION_MA,LONGITUDE_ASCENDING_NODE_MA,ANGLE0_MA,MAGNITUDE_1UA_MA,false);
    Planet *jupiter = creatPlanet(TROPICAL_YEAR_JU,LONGITUDE_J2010_JU,LONGITUDE_PERIGEE_JU,EXENTRICITE_ST_JU,HALF_WIDE_AXIS_ORBIT_JU,ECLIPTIC_ORBIT_INCLINATION_JU,LONGITUDE_ASCENDING_NODE_JU,ANGLE0_JU,MAGNITUDE_1UA_JU,false);
    Planet *saturn = creatPlanet(TROPICAL_YEAR_SA,LONGITUDE_J2010_SA,LONGITUDE_PERIGEE_SA,EXENTRICITE_ST_SA,HALF_WIDE_AXIS_ORBIT_SA,ECLIPTIC_ORBIT_INCLINATION_SA,LONGITUDE_ASCENDING_NODE_SA,ANGLE0_SA,MAGNITUDE_1UA_SA,false);
    Planet *uranus = creatPlanet(TROPICAL_YEAR_UR,LONGITUDE_J2010_UR,LONGITUDE_PERIGEE_UR,EXENTRICITE_ST_UR,HALF_WIDE_AXIS_ORBIT_UR,ECLIPTIC_ORBIT_INCLINATION_UR,LONGITUDE_ASCENDING_NODE_UR,ANGLE0_UR,MAGNITUDE_1UA_UR,false);
    Planet *neptune = creatPlanet(TROPICAL_YEAR_NE,LONGITUDE_J2010_NE,LONGITUDE_PERIGEE_NE,EXENTRICITE_ST_NE,HALF_WIDE_AXIS_ORBIT_NE,ECLIPTIC_ORBIT_INCLINATION_NE,LONGITUDE_ASCENDING_NODE_NE,ANGLE0_NE,MAGNITUDE_1UA_NE,false);

    float* mercureGeo = PosPlanetGeocentric(2024,7,27,mercure);
    float* venusGeo = PosPlanetGeocentric(2024,7,27,venus);
    float* marchGeo = PosPlanetGeocentric(2024,7,27,march);
    float* jupiterGeo = PosPlanetGeocentric(2024,7,27,jupiter);
    float* saturnGeo = PosPlanetGeocentric(2024,7,27,saturn);
    float* uranusGeo = PosPlanetGeocentric(2024,7,27,uranus);
    float* neptuneGeo = PosPlanetGeocentric(2024,7,27,neptune);

    printf("Geocentric position :\n");
    printf("Mercure :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",mercureGeo[0],mercureGeo[1],mercureGeo[2],mercureGeo[3]);
    printf("Venus :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",venusGeo[0],venusGeo[1],venusGeo[2],venusGeo[3]);
    printf("Earth :\n   longitude : 0\n   latitude : 0\n   taille angulaire : 0\n   magnitude : 0\n\n");
    printf("March :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",marchGeo[0],marchGeo[1],marchGeo[2],marchGeo[3]);
    printf("Jupiter :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",jupiterGeo[0],jupiterGeo[1],jupiterGeo[2],jupiterGeo[3]);
    printf("Saturn :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",saturnGeo[0],saturnGeo[1],saturnGeo[2],saturnGeo[3]);
    printf("Uranus :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",uranusGeo[0],uranusGeo[1],uranusGeo[2],uranusGeo[3]);
    printf("Neptune :\n   longitude : %f\n   latitude : %f\n   taille angulaire : %f\n   magnitude : %f\n\n",neptuneGeo[0],neptuneGeo[1],neptuneGeo[2],neptuneGeo[3]);

    float bvSun = CalculBV(550,300,-26.832);
    float t = ColorStar(bvSun);
    printf("Sun (%f %f) is : ",t,bvSun);
    if(t<=1500) printf("RED !\n");
    else if(t<=5500) printf("ORANGE !\n");
    else if(t<=7500) printf("WHITE !\n");
    else printf("BLUE !\n");

    free(mercureGeo);
    free(venusGeo);
    free(marchGeo);
    free(jupiterGeo);
    free(saturnGeo);
    free(uranusGeo);
    free(neptuneGeo);

    destroyPlanet(mercure);
    destroyPlanet(venus);
    destroyPlanet(march);
    destroyPlanet(jupiter);
    destroyPlanet(saturn);
    destroyPlanet(uranus);
    destroyPlanet(neptune);
    
    return 0;
}
