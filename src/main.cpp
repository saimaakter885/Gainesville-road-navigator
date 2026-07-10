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
        cout << "No route available to export. Please find a route first." << endl;
        return;
    }

    ofstream file("route.csv");
    if (!file.is_open())
    {
        cout << "Error: Could not create route.csv" << endl;
        return;
    }

    file << "Name, Latitude, Longitude" << endl;

    for (size_t i = 0; i < lastPath.size(); i++)
    {
        file << "Point " << i + 1 << "," << fixed << setprecision(7) << lastPath[i].first << "," << lastPath[i].second << endl;
    }

    file.close();
    cout << endl;
    cout << "Route exported successfully!" << endl;
    cout << "File saved as: route.csv" << endl;
}

void printMenu() {
    cout << endl;
    cout << "======================================" << endl;
    cout << "        Gainesville Road Navigator     " << endl;
    cout << "======================================" << endl;
    cout << "1. Find shortest path" << endl;
    cout << "2. Run Comparison (Dijkstra vs A*)" << endl;
    cout << "3. Show graph stats" << endl;
    cout << "4. Show sample places" << endl;
    cout << "5. Export last route to CSV" << endl;
    cout << "6. Exit" << endl;
    cout << "======================================" << endl;
    cout << "Enter choice: ";
}

void runRoute(Graph& graph, KDTree& kdTree, bool compareMode)
{
    string startName;
    string endName;

    cout << endl;
    cout << "Start place: ";
    getline(cin, startName);

    cout << "End place: ";
    getline(cin, endName);

    Place* startPlace = findPlaceByName(graph, startName);
    Place* endPlace = findPlaceByName(graph, endName);

    if (startPlace == nullptr)
    {
        cout << "Start place not found." << endl;
        return;
    }

    if (endPlace == nullptr)
    {
        cout << "End place not found." << endl;
        return;
    }

    cout << endl;
    cout << "Matched start: " << startPlace->name << endl;
    cout << "Matched end: " << endPlace->name << endl;

    Node* kdStart = kdTree.findNearestNode(startPlace->lat, startPlace->lon);
    Node* kdEnd = kdTree.findNearestNode(endPlace->lat, endPlace->lon);

    vector<Node*> startNodes = findPossibleStartEndNodes(graph, startPlace->lat, startPlace->lon);
    vector<Node*> endNodes = findPossibleStartEndNodes(graph, endPlace->lat, endPlace->lon);

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
        cout << "No route found between these places." << endl;
        return;
    }

    lastPath = bestAstarPath;

    cout << endl;
    cout << "           Route Comparison            " << endl;
    cout << "----------------------------------------" << endl;
    cout << startPlace->name << " ---> " << endPlace->name << endl;
    cout << fixed << setprecision(2);
    cout << "Distance: " << bestAstar.getTotalDistance() / 1000.0 << " km" << endl;
    cout << "----------------------------------------" << endl;

    if (compareMode)
    {
        cout << left;
        cout << setw(20) << "" << setw(20) << "Dijkstra"
                 << setw(20) << "A*" << endl;

        cout << setw(20) << "Nodes:" << setw(20) << bestDijkstra.getNodesVisited()
                << setw(20) << bestAstar.getNodesVisited()
                << endl;

        cout << setw(20) << "Time:" << setw(20) << (to_string(bestDijkstra.getExecutionTime()) + " us")
                    << setw(20) << (to_string(bestAstar.getExecutionTime()) + " us")
                    << endl;

        ostringstream dijkstraDistance, astarDistance;
        dijkstraDistance << fixed << setprecision(2) << bestDijkstra.getTotalDistance() / 1000.0 << " km";
        astarDistance << fixed << setprecision(2) << bestAstar.getTotalDistance() / 1000.0 << " km";

        cout << setw(20) << "Distance:" << setw(20) << dijkstraDistance.str()
                          << setw(20) << astarDistance.str()
                          << endl;

        cout << setw(20) << "Path size:" << setw(20) << bestDijkstraPath.size()
                          << setw(20) << bestAstarPath.size()
                          << endl;
        cout << right;
        cout << "--------------------------------------" << endl;

        if (bestAstar.getNodesVisited() < bestDijkstra.getNodesVisited())
        {
            double dijkstraNodes = static_cast<double>(bestDijkstra.getNodesVisited());
            double astarNodes = static_cast<double>(bestAstar.getNodesVisited());
            double percentage = dijkstraNodes > 0.0 ? ((dijkstraNodes - astarNodes) / dijkstraNodes) * 100.0 : 0.0;
            int timeDifference = bestDijkstra.getExecutionTime() - bestAstar.getExecutionTime();

            cout << fixed << setprecision(1)
                 << "A* explored " << percentage << "% fewer nodes and finished "
                 << timeDifference << " us faster." << endl;
        }
        else
        {
            cout << "Both algorithms found the route. A* did not explore fewer nodes for this case." << endl;
        }
    }
    else
    {
        cout << "Shortest path found using A*." << endl;
        cout << "Nodes visited: " << bestAstar.getNodesVisited() << endl;
        cout << "Time taken: " << bestAstar.getExecutionTime() << " us" << endl;
        cout << "Path distance: " << bestAstar.getTotalDistance() / 1000.0 << " km" << endl;
        cout << "Path size: " << bestAstarPath.size() << endl;
    }
}

int main()
{
    Graph graph;
    Parser parser;

    cout << "Loading Gainesville road network..." << endl;

    parser.parse("../data/gainesville.geojson", graph);

    KDTree kdTree;
    kdTree.build(graph.nodes);

    cout << "Data loaded successfully." << endl;

    int choice;

    while (true)
    {
        printMenu();

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "Invalid choice. Please enter a number from 1-6." << endl;
            continue;
        }

        cin.ignore(10000, '\n');

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
            graph.printGraphInfo();
        }
        else if (choice == 4)
        {
            graph.printSamplePlaces();
        }
        else if (choice == 5)
        {
            exportRouteToCSV();
        }
        else if (choice == 6)
        {
            cout << "Exiting Gainesville Road Navigator." << endl;
            break;
        }
        else
        {
            cout << "Invalid choice. Try again." << endl;
        }
    }
}