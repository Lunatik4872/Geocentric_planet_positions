#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "constants.h"
#include "positionObj.h"

int main(int argc, char **argv){
    Planet *mercure = creatPlanet(TROPICAL_YEAR_ME,LONGITUDE_J2010_ME,LONGITUDE_PERIGEE_ME,EXENTRICITE_ST_ME,HALF_WIDE_AXIS_ORBIT_ME,ECLIPTIC_ORBIT_INCLINATION_ME,LONGITUDE_ASCENDING_NODE_ME,ANGLE0_ME,MAGNITUDE_1UA_ME,true);
    Planet *venus = creatPlanet(TROPICAL_YEAR_VE,LONGITUDE_J2010_VE,LONGITUDE_PERIGEE_VE,EXENTRICITE_ST_VE,HALF_WIDE_AXIS_ORBIT_VE,ECLIPTIC_ORBIT_INCLINATION_VE,LONGITUDE_ASCENDING_NODE_VE,ANGLE0_VE,MAGNITUDE_1UA_VE,true);
    Planet *march = creatPlanet(TROPICAL_YEAR_MA,LONGITUDE_J2010_MA,LONGITUDE_PERIGEE_MA,EXENTRICITE_ST_MA,HALF_WIDE_AXIS_ORBIT_MA,ECLIPTIC_ORBIT_INCLINATION_MA,LONGITUDE_ASCENDING_NODE_MA,ANGLE0_MA,MAGNITUDE_1UA_MA,false);
    Planet *jupiter = creatPlanet(TROPICAL_YEAR_JU,LONGITUDE_J2010_JU,LONGITUDE_PERIGEE_JU,EXENTRICITE_ST_JU,HALF_WIDE_AXIS_ORBIT_JU,ECLIPTIC_ORBIT_INCLINATION_JU,LONGITUDE_ASCENDING_NODE_JU,ANGLE0_JU,MAGNITUDE_1UA_JU,false);
    Planet *saturn = creatPlanet(TROPICAL_YEAR_SA,LONGITUDE_J2010_SA,LONGITUDE_PERIGEE_SA,EXENTRICITE_ST_SA,HALF_WIDE_AXIS_ORBIT_SA,ECLIPTIC_ORBIT_INCLINATION_SA,LONGITUDE_ASCENDING_NODE_SA,ANGLE0_SA,MAGNITUDE_1UA_SA,false);
    Planet *uranus = creatPlanet(TROPICAL_YEAR_UR,LONGITUDE_J2010_UR,LONGITUDE_PERIGEE_UR,EXENTRICITE_ST_UR,HALF_WIDE_AXIS_ORBIT_UR,ECLIPTIC_ORBIT_INCLINATION_UR,LONGITUDE_ASCENDING_NODE_UR,ANGLE0_UR,MAGNITUDE_1UA_UR,false);
    Planet *neptune = creatPlanet(TROPICAL_YEAR_NE,LONGITUDE_J2010_NE,LONGITUDE_PERIGEE_NE,EXENTRICITE_ST_NE,HALF_WIDE_AXIS_ORBIT_NE,ECLIPTIC_ORBIT_INCLINATION_NE,LONGITUDE_ASCENDING_NODE_NE,ANGLE0_NE,MAGNITUDE_1UA_NE,false);

    int day = 29, month = 7, year = 2024;

    float* mercureGeo = PosPlanetGeocentric(year,month,day,mercure);
    float* venusGeo = PosPlanetGeocentric(year,month,day,venus);
    float* marchGeo = PosPlanetGeocentric(year,month,day,march);
    float* jupiterGeo = PosPlanetGeocentric(year,month,day,jupiter);
    float* saturnGeo = PosPlanetGeocentric(year,month,day,saturn);
    float* uranusGeo = PosPlanetGeocentric(year,month,day,uranus);
    float* neptuneGeo = PosPlanetGeocentric(year,month,day,neptune);

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