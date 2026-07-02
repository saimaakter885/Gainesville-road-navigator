#ifndef GAINESVILLE_ROAD_NAVIGATOR_PARSER_H
#define GAINESVILLE_ROAD_NAVIGATOR_PARSER_H

#include "graph.h"
#include <string>
#include <unordered_map>
using namespace std;

class Parser
{
private:
    unordered_map<string, long long> coordinateMap;
    long long nextId = 1;

    string getCoordinateString(double lat, double lon);
    long long getCoordinateId(double lat, double lon);

public:
    void parse(const string& filePath, Graph& graph);
};

#endif //GAINESVILLE_ROAD_NAVIGATOR_PARSER_H