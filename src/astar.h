//
// Created by saima on 6/28/2026.
//

#ifndef GAINESVILLE_ROAD_NAVIGATOR_ASTAR_H
#define GAINESVILLE_ROAD_NAVIGATOR_ASTAR_H

#include "router.h"

class Astar : public Router {
public:
    vector<pair<double, double>> findPath(Node* start, Node* destination) override;
};

#endif //GAINESVILLE_ROAD_NAVIGATOR_ASTAR_H