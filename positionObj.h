#ifndef POSITIONOBJ_H
#define POSITIONOBJ_H

typedef struct Planet Planet;

bool IsBixectil(int year);
float DaySinceJ2010(int year, int month, int day);
float* PosSun(int year, int month, int day);
float CalculBV(float eV, float eB, float c);
float ColorStar(float indiceColorBV);
float* PosPlanetGeocentric(int year, int month, int day, Planet *planet);
Planet* creatPlanet(float tropical_year, float longitude_j2010, float longitude_perigee, float exentricite_st, float half_wide_axis_orbit, float ecliptic_orbit_inclination, float longitude_ascending_node, float angle0, float magnitude_1ua, bool is_inner_planet);
void destroyPlanet(Planet *planet);

#endif