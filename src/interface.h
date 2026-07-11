#pragma once
#include <string>
#include <vector>
#include <utility>

#include "graph.h"
#include "router.h"

namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string CYAN    = "\033[36m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string RED     = "\033[31m";
    const std::string BCYAN   = "\033[1;36m";
    const std::string BGREEN  = "\033[1;32m";
    const std::string BYELLOW = "\033[1;33m";
    const std::string BWHITE  = "\033[1;37m";
}

int displayMenu();
void displayGraphStats(const Graph& graph);
void displaySamplePlaces(const Graph& graph);
void displayRouteHeader(const std::string& startName, const std::string& endName, double distanceMeters);
void displayComparisonTable(Router& dijkstra, Router& astar, size_t dijkstraPathSize, size_t astarPathSize);
void displaySingleResult(Router& astar, size_t pathSize);
void displayError(const std::string& message);
void displaySuccess(const std::string& message);
void displayInfo(const std::string& message);
void displayNoRoute();
void displayNoLastRoute();
void displayExportSuccess(const std::string& filename, size_t pointCount);
void displayLoadingGraph();
void displayLoadSuccess(const Graph& graph);
