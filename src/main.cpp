#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <fstream>

#include "graph.h"
#include "parser.h"
#include "dijkstra.h"
#include "astar.h"
#include "kdTree.h"
#include "interface.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

vector<pair<double, double>> lastPath;

string lowerCase(string text)
{
    for (char& c : text)
    {
        c = tolower(c);
    }
    return text;
}

Place* findPlaceByName(Graph& graph, string searchName)
{
    searchName = lowerCase(searchName);
    for (size_t i = 0; i < graph.places.size(); i++)
    {
        string placeName = lowerCase(graph.places[i].name);
        if (placeName.find(searchName) != string::npos)
        {
            return &graph.places[i];
        }
    }
    return nullptr;
}

bool tryParseCoordinates(const string& input, double& lat, double& lon) {
    if (input.find(',') != string::npos || (!input.empty() && (std::isdigit(input[0]) || input[0] == '-'))) {
        try {
            std::stringstream ss(input);
            string latStr, lonStr;
            
            if (std::getline(ss, latStr, ',')) {
                std::getline(ss, lonStr);
                lat = std::stod(latStr);
                lon = std::stod(lonStr);
                return true;
            }
        } catch (...) {
            return false;
        }
    }
    return false;
}

vector<Node*> findPossibleStartEndNodes(Graph& graph, double lat, double lon, int limit = 30)
{
    vector<pair<double, Node*>> possibleNodes;
    for (auto& pair : graph.nodes)
    {
        Node* node = &pair.second;
        if (node->edges.empty())
        {
            continue;
        }

        double dLat = node->lat - lat;
        double dLon = node->lon - lon;
        double dist = dLat * dLat + dLon * dLon;
        possibleNodes.push_back({dist, node});
    }

    sort(possibleNodes.begin(), possibleNodes.end());
    vector<Node*> result;
    for (size_t i = 0; i < possibleNodes.size() && i < static_cast<size_t>(limit); i++)
    {
        result.push_back(possibleNodes[i].second);
    }
    return result;
}

void exportRouteToCSV()
{
    if (lastPath.empty())
    {
        displayNoLastRoute();
        return;
    }

    ofstream file("../route.csv");
    if (!file.is_open())
    {
        displayError("Could not create route.csv");
        return;
    }

    file << "Name, Latitude, Longitude" << endl;

    for (size_t i = 0; i < lastPath.size(); i++)
    {
        string label;
        if (i == 0)
        {
            label = "Start";
        }
        else if (i == lastPath.size() - 1)
        {
            label = "End";
        }
        else
        {
            label = "Waypoint " + to_string(i);
        }
        file << label << "," << fixed << setprecision(7)
             << lastPath[i].first << "," << lastPath[i].second << "\n";
    }

    file.close();
    displayExportSuccess("route.csv", lastPath.size());
}

void exportRouteToKML() {
    if (lastPath.empty())
    {
        displayNoLastRoute();
        return;
    }

    ofstream kmlFile("../route.kml");
    if (!kmlFile.is_open())
    {
        displayError("Could not create route.kml");
        return;
    }

    // KML header
    kmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    kmlFile << "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n";
    kmlFile << "  <Document>\n";
    kmlFile << "    <name>Gainesville Routing Engine Path</name>\n";

    // Route styling
    kmlFile << "    <Style id=\"routeStyle\">\n";
    kmlFile << "      <LineStyle>\n";
    kmlFile << "        <color>ff0000ff</color>\n";
    kmlFile << "        <width>4</width>\n";
    kmlFile << "      </LineStyle>\n";
    kmlFile << "    </Style>\n";

    // Start node styling
    kmlFile << "    <Style id=\"startStyle\">\n";
    kmlFile << "      <IconStyle>\n";
    kmlFile << "        <scale>1.2</scale>\n"; 
    kmlFile << "        <Icon>\n";
    kmlFile << "          <href>http://maps.google.com/mapfiles/kml/paddle/red-circle.png</href>\n";
    kmlFile << "        </Icon>\n";
    kmlFile << "      </IconStyle>\n";
    kmlFile << "    </Style>\n";

    // End node styling
    kmlFile << "    <Style id=\"endStyle\">\n";
    kmlFile << "      <IconStyle>\n";
    kmlFile << "        <scale>1.2</scale>\n"; 
    kmlFile << "        <Icon>\n";
    kmlFile << "          <href>http://maps.google.com/mapfiles/kml/paddle/grn-circle.png</href>\n";
    kmlFile << "        </Icon>\n";
    kmlFile << "      </IconStyle>\n";
    kmlFile << "    </Style>\n";

    // Add start node marker
    kmlFile << "    <Placemark>\n";
    kmlFile << "      <name>Start Point</name>\n";
    kmlFile << "      <styleUrl>#startStyle</styleUrl>\n";
    kmlFile << "      <description>Origin Node</description>\n";
    kmlFile << "      <Point>\n";
    kmlFile << "        <coordinates>" << lastPath.front().second << "," << lastPath.front().first << ",0.0</coordinates>\n";
    kmlFile << "      </Point>\n";
    kmlFile << "    </Placemark>\n";

    // Add end node marker
    kmlFile << "    <Placemark>\n";
    kmlFile << "      <name>End Point</name>\n";
    kmlFile << "      <styleUrl>#endStyle</styleUrl>\n";
    kmlFile << "      <description>Destination Node</description>\n";
    kmlFile << "      <Point>\n";
    kmlFile << "        <coordinates>" << lastPath.back().second << "," << lastPath.back().first << ",0.0</coordinates>\n";
    kmlFile << "      </Point>\n";
    kmlFile << "    </Placemark>\n";

    // LineString geometry
    kmlFile << "    <Placemark>\n";
    kmlFile << "      <name>Shortest Path</name>\n";
    kmlFile << "      <styleUrl>#routeStyle</styleUrl>\n";
    kmlFile << "      <LineString>\n";
    kmlFile << "        <tessellate>1</tessellate>\n";
    kmlFile << "        <coordinates>\n";

    // Add coordinates (Longitude, Latitude, Altitude)
    for (const auto& coord : lastPath) {
        kmlFile << "          " << coord.second << "," << coord.first << ",0.0\n";
    }

    // Close structure
    kmlFile << "        </coordinates>\n";
    kmlFile << "      </LineString>\n";
    kmlFile << "    </Placemark>\n";
    kmlFile << "  </Document>\n";
    kmlFile << "</kml>\n";

    kmlFile.close();
    displayExportSuccess("route.kml", lastPath.size());
}

void runRoute(Graph& graph, KDTree& kdTree, bool compareMode)
{
    string startInput, endInput;
    Place* startPlace;
    Place* endPlace;
    double startLat = 0.0, startLon = 0.0;
    double endLat = 0.0, endLon = 0.0;

    cout << endl;
    cout << "Start place name OR coordinates (lat, lon): ";
    getline(cin, startInput);

    cout << "End place name OR coordinates (lat, lon): ";
    getline(cin, endInput);

    bool coordinates = tryParseCoordinates(startInput, startLat, startLon);

    // Process start input
    if (coordinates) {
        displayInfo("Parsed start coordinates: " + to_string(startLat) + ", " + to_string(startLon));
    } else {
        startPlace = findPlaceByName(graph, startInput);
        if (startPlace == nullptr) {
            displayError("Start place not found.");
            return;
        }
        displayInfo("Matched start place: " + startPlace->name);
        startLat = startPlace->lat;
        startLon = startPlace->lon;
    }

    // Process end input
    bool endCoordinates = tryParseCoordinates(endInput, endLat, endLon);
    if (endCoordinates) {
        displayInfo("Parsed end coordinates: " + to_string(endLat) + ", " + to_string(endLon));
    } else {
        endPlace = findPlaceByName(graph, endInput);
        if (endPlace == nullptr) {
            displayError("End place not found.");
            return;
        }
        displayInfo("Matched end place: " + endPlace->name);
        endLat = endPlace->lat;
        endLon = endPlace->lon;
    }

    Node* kdStart = kdTree.findNearestNode(startLat, startLon);
    Node* kdEnd = kdTree.findNearestNode(endLat, endLon);

    vector<Node*> startNodes = findPossibleStartEndNodes(graph, startLat, startLon);
    vector<Node*> endNodes = findPossibleStartEndNodes(graph, endLat, endLon);

    if (kdStart != nullptr)
    {
        startNodes.insert(startNodes.begin(), kdStart);
    }

    if (kdEnd != nullptr)
    {
        endNodes.insert(endNodes.begin(), kdEnd);
    }

    Dijkstra bestDijkstra;
    Astar bestAstar;
    vector<pair<double, double>> bestDijkstraPath;
    vector<pair<double, double>> bestAstarPath;

    bool found = false;
    for (Node* startNode : startNodes)
    {
        for (Node* endNode : endNodes)
        {
            Dijkstra dijkstra;
            Astar astar;

            vector<pair<double, double>> dijkstraPath = dijkstra.findPath(startNode, endNode);
            vector<pair<double, double>> astarPath = astar.findPath(startNode, endNode);

            if (!dijkstraPath.empty() && !astarPath.empty())
            {
                bestDijkstra = dijkstra;
                bestAstar = astar;
                bestDijkstraPath = dijkstraPath;
                bestAstarPath = astarPath;
                found = true;
                break;
            }
        }
        if (found)
        {
            break;
        }
    }
    if (!found)
    {
        displayNoRoute();
        return;
    }

    lastPath = bestAstarPath;

    string startName = coordinates ? ("(" + to_string(startLat) + ", " + to_string(startLon) + ")") : startPlace->name;
    string endName = endCoordinates ? ("(" + to_string(endLat) + ", " + to_string(endLon) + ")") : endPlace->name;

    displayRouteHeader(startName, endName, bestAstar.getTotalDistance());

    if (compareMode)
    {
        displayComparisonTable(bestDijkstra, bestAstar, bestDijkstraPath.size(), bestAstarPath.size());
    }
    else
    {
        displaySingleResult(bestAstar, bestAstarPath.size());
    }
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode = 0;
    GetConsoleMode(consoleHandle, &consoleMode);
    SetConsoleMode(consoleHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    Graph graph;
    Parser parser;

    displayLoadingGraph();

    if (!parser.parse("../data/gainesville.geojson", graph)) {
        return -1;
    }

    KDTree kdTree;
    kdTree.build(graph.nodes);

    displayLoadSuccess(graph);

    while (true)
    {
        int choice = displayMenu();

        if (choice == 1)
        {
            runRoute(graph, kdTree, false);
        }
        else if (choice == 2)
        {
            runRoute(graph, kdTree, true);
        }
        else if (choice == 3)
        {
            displayGraphStats(graph);
        }
        else if (choice == 4)
        {
            displaySamplePlaces(graph);
        }
        else if (choice == 5)
        {
            exportRouteToCSV();
        }
        else if (choice == 6)
        {
            exportRouteToKML();
        }
        else if (choice == 7)
        {
            cout << "Exiting Gainesville Road Navigator." << endl;
            break;
        }
    }

    return 0;
}