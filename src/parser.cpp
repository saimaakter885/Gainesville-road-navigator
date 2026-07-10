#include "parser.h"
#include "distance.h"
#include "graph.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;
using json = nlohmann::json;

string Parser::getCoordinateString(double lat, double lon)
{
    ostringstream key;

    key << fixed << setprecision(7) << lat << "," << lon;
    return key.str();
}

long long Parser::getCoordinateId(double lat, double lon)
{
    string key = getCoordinateString(lat, lon);
    if (coordinateMap.find(key) == coordinateMap.end())
    {
        coordinateMap[key] = nextId;
        nextId++;
    }
    return coordinateMap[key];
}

bool Parser::parse(const string& filePath, Graph& graph)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cout << "Error: Cannot open file " << filePath << endl;
        return false;
    }

    json data;
    file >> data;
    if (!data.contains("features"))
    {
        cout << "Error: No features found in GeoJSON file" << endl;
        return false;
    }

    int edgeCount = 0;
    for (size_t i = 0; i < data["features"].size(); i++)
    {
        json feature = data["features"][i];

        if (!feature.contains("geometry"))
        {
            continue;
        }

        json geometry = feature["geometry"];

        if (!geometry.contains("type") || !geometry.contains("coordinates"))
        {
            continue;
        }

        string geometryType = geometry["type"];

        if (geometryType == "Point")
        {
            if (feature.contains("properties") &&
                feature["properties"].contains("name"))
            {
                string name = feature["properties"]["name"];
                double lon = geometry["coordinates"][0];
                double lat = geometry["coordinates"][1];

                graph.addPlace(name, lat, lon);
            }
            continue;
        }

        if (geometryType != "LineString")
        {
            continue;
        }

        json coordinates = geometry["coordinates"];
        if (coordinates.size() < 2)
        {
            continue;
        }

        for (size_t j = 0; j < coordinates.size(); j++)
        {
            double lon = coordinates[j][0];
            double lat = coordinates[j][1];

            long long id = getCoordinateId(lat, lon);
            graph.addNode(id, lat, lon);
        }

        for (size_t j = 0; j + 1 < coordinates.size(); j++)
        {
            double lon1 = coordinates[j][0];
            double lat1 = coordinates[j][1];
            double lon2 = coordinates[j + 1][0];
            double lat2 = coordinates[j + 1][1];

            long long id1 = getCoordinateId(lat1, lon1);
            long long id2 = getCoordinateId(lat2, lon2);

            double weight = haversineDistance(lat1, lon1, lat2, lon2);

            graph.addEdge(id1, id2, weight);
            edgeCount++;
        }
    }
    cout << "Edge count: " << edgeCount << endl;
    return true;
}
