#include "distance.h"
#include <cmath>

double haversineDistance(double lat1, double lon1, double lat2, double lon2){
    const double earth_r = 6371000.0;

    double rad_lat1 = lat1 * M_PI / 180.0;
    double rad_lat2 = lat2 * M_PI / 180.0;
    double rad_lon1 = lon1 * M_PI / 180.0;
    double rad_lon2 = lon2 * M_PI / 180.0;

    double deltaLat = rad_lat2 - rad_lat1;
    double deltaLon = rad_lon2 - rad_lon1;

    double latLonDifference = sin(deltaLat / 2) * sin(deltaLat / 2) + cos(rad_lat1) * cos(rad_lat2) *
               sin(deltaLon / 2) * sin(deltaLon / 2);

    double angle_rad = 2 * atan2(sqrt(latLonDifference), sqrt(1 - latLonDifference));

    return earth_r * angle_rad;
}

