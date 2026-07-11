#include "interface.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;


static string formatWithCommas(long long n) {
    string s = to_string(n);
    int pos = static_cast<int>(s.length()) - 3;
    while (pos > 0) {
        s.insert(pos, ",");
        pos -= 3;
    }
    return s;
}

// string of dashes for box wdith
static string horizontalLine(int width) {
    string line = "";
    for (int i = 0; i < width; i++) {
        line += "-";
    }
    return line;
}

// Counts displayed characters not bytes because UTF-8 symbols (µ · →)
static int displayWidth(const string& text) {
    int width = 0;
    for (size_t i = 0; i < text.size(); i++) {
        unsigned char c = text[i];
        // skip  don't add column
        if ((c & 0xC0) != 0x80) {
            width++;
        }
    }
    return width;
}

int displayMenu() {
    int width = 48;
    string title = "GAINESVILLE ROAD NAVIGATOR";

    int titleLeftPad = (width - displayWidth(title)) / 2;
    if (titleLeftPad < 0) titleLeftPad = 0;
    int titleRightPad = width - displayWidth(title) - titleLeftPad;
    if (titleRightPad < 0) titleRightPad = 0;

    cout << "\n";
    cout << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET
         << string(titleLeftPad, ' ') << Color::BCYAN << title << Color::RESET << string(titleRightPad, ' ')
         << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";

    string option1 = "  [1]  Find shortest path";
    string option2 = "  [2]  Compare algorithms  (Dijkstra vs A*)";
    string option3 = "  [3]  Show graph statistics";
    string option4 = "  [4]  Show sample places";
    string option5 = "  [5]  Export last route to CSV";
    string option6 = "  [6]  Export last route to KML";
    string option7 = "  [7]  Exit";

    int pad1 = width - displayWidth(option1); if (pad1 < 0) pad1 = 0;
    int pad2 = width - displayWidth(option2); if (pad2 < 0) pad2 = 0;
    int pad3 = width - displayWidth(option3); if (pad3 < 0) pad3 = 0;
    int pad4 = width - displayWidth(option4); if (pad4 < 0) pad4 = 0;
    int pad5 = width - displayWidth(option5); if (pad5 < 0) pad5 = 0;
    int pad6 = width - displayWidth(option6); if (pad6 < 0) pad6 = 0;
    int pad7 = width - displayWidth(option7); if (pad7 < 0) pad7 = 0;

    cout << Color::CYAN << "|" << Color::RESET << "  [" << Color::YELLOW << "1" << Color::RESET << "]  Find shortest path"
         << string(pad1, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << "  [" << Color::YELLOW << "2" << Color::RESET << "]  Compare algorithms  (Dijkstra vs A*)"
         << string(pad2, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << "  [" << Color::YELLOW << "3" << Color::RESET << "]  Show graph statistics"
         << string(pad3, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << "  [" << Color::YELLOW << "4" << Color::RESET << "]  Show sample places"
         << string(pad4, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << "  [" << Color::YELLOW << "5" << Color::RESET << "]  Export last route to CSV"
         << string(pad5, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << "  [" << Color::YELLOW << "6" << Color::RESET << "]  Export last route to KML"
         << string(pad6, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << "  [" << Color::YELLOW << "7" << Color::RESET << "]  Exit"
         << string(pad7, ' ') << Color::CYAN << "|" << Color::RESET << "\n";

    cout << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";

    int choice;
    while (true) {
        cout << Color::BYELLOW << "  Select option (1-7): " << Color::RESET;

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            displayError("Invalid choice. Please enter a number from 1-7.");
            continue;
        }

        cin.ignore(10000, '\n');

        if (choice < 1 || choice > 7) {
            displayError("Invalid choice. Please enter a number from 1-7.");
            continue;
        }

        return choice;
    }
}

void displayGraphStats(const Graph& graph) {
    long long totalEdges = 0;
    for (const auto& entry : graph.nodes) {
        totalEdges += (long long)entry.second.edges.size();
    }
    totalEdges = totalEdges / 2;

    int width = 36;
    string title = "  GRAPH STATISTICS";
    string line1 = "  Region    Gainesville, FL";
    string line2 = "  Nodes     " + formatWithCommas((long long)graph.nodes.size());
    string line3 = "  Edges     " + formatWithCommas(totalEdges);

    int titlePad = width - displayWidth(title); if (titlePad < 0) titlePad = 0;
    int pad1 = width - displayWidth(line1); if (pad1 < 0) pad1 = 0;
    int pad2 = width - displayWidth(line2); if (pad2 < 0) pad2 = 0;
    int pad3 = width - displayWidth(line3); if (pad3 < 0) pad3 = 0;

    cout << "\n";
    cout << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << Color::BCYAN << title << Color::RESET
         << string(titlePad, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << line1 << string(pad1, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << line2 << string(pad2, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << line3 << string(pad3, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";
}

void displaySamplePlaces(const Graph& graph) {
    string title = "  SAMPLE NAMED PLACES";

    vector<string> lines;
    int count = 0;
    for (size_t i = 0; i < graph.places.size() && count < 20; i++) {
        lines.push_back("  · " + graph.places[i].name);
        count++;
    }

    // Box to biggest line
    int width = displayWidth(title);
    for (size_t i = 0; i < lines.size(); i++) {
        int lineWidth = displayWidth(lines[i]);
        if (lineWidth > width) {
            width = lineWidth;
        }
    }

    int titlePad = width - displayWidth(title); if (titlePad < 0) titlePad = 0;

    cout << "\n";
    cout << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";
    cout << Color::CYAN << "|" << Color::RESET << Color::BCYAN << title << Color::RESET
         << string(titlePad, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";

    for (size_t i = 0; i < lines.size(); i++) {
        int pad = width - displayWidth(lines[i]);
        if (pad < 0) pad = 0;
        cout << Color::CYAN << "|" << Color::RESET << lines[i] << string(pad, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    }

    cout << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";
}

void displayRouteHeader(const string& startName, const string& endName, double distanceMeters) {
    ostringstream distStream;
    distStream << fixed << setprecision(2) << (distanceMeters / 1000.0) << " km";
    string distanceText = distStream.str();

    string plainLine = "  " + startName + "  ->  " + endName + "       " + distanceText;

    cout << "\n  " << Color::BWHITE << startName << Color::RESET
         << "  " << Color::CYAN << "→" << Color::RESET << "  "
         << Color::BWHITE << endName << Color::RESET
         << "       " << Color::GREEN << distanceText << Color::RESET << "\n";

    cout << "  " << Color::CYAN << horizontalLine(displayWidth(plainLine)) << Color::RESET << "\n";
}

void displayComparisonTable(Router& dijkstra, Router& astar, size_t dijkstraPathSize, size_t astarPathSize) {
    int labelWidth = 22;
    int valueWidth = 17;
    string indent = "  ";

    cout << indent << Color::CYAN << "+" << horizontalLine(labelWidth) << "+"
         << horizontalLine(valueWidth) << "+" << horizontalLine(valueWidth) << "+" << Color::RESET << "\n";

    //column titles
    string dijkstraHeader = "Dijkstra";
    string astarHeader = "A*";
    int dHeaderPad = valueWidth - displayWidth(dijkstraHeader); if (dHeaderPad < 0) dHeaderPad = 0;
    int aHeaderPad = valueWidth - displayWidth(astarHeader); if (aHeaderPad < 0) aHeaderPad = 0;

    cout << indent << Color::CYAN << "|" << Color::RESET << string(labelWidth, ' ')
         << Color::CYAN << "|" << Color::RESET << string(dHeaderPad, ' ') << Color::BOLD << dijkstraHeader << Color::RESET
         << Color::CYAN << "|" << Color::RESET << string(aHeaderPad, ' ') << Color::BOLD << astarHeader << Color::RESET
         << Color::CYAN << "|" << Color::RESET << "\n";

    cout << indent << Color::CYAN << "+" << horizontalLine(labelWidth) << "+"
         << horizontalLine(valueWidth) << "+" << horizontalLine(valueWidth) << "+" << Color::RESET << "\n";

    //  Nodes explored row
    string label1 = "  Nodes explored";
    string dValue1 = formatWithCommas(dijkstra.getNodesVisited());
    string aValue1 = formatWithCommas(astar.getNodesVisited());
    int label1Pad = labelWidth - displayWidth(label1); if (label1Pad < 0) label1Pad = 0;
    int dValue1Pad = valueWidth - displayWidth(dValue1); if (dValue1Pad < 0) dValue1Pad = 0;
    int aValue1Pad = valueWidth - displayWidth(aValue1); if (aValue1Pad < 0) aValue1Pad = 0;

    cout << indent << Color::CYAN << "|" << Color::RESET << label1 << string(label1Pad, ' ')
         << Color::CYAN << "|" << Color::RESET << string(dValue1Pad, ' ') << dValue1
         << Color::CYAN << "|" << Color::RESET << string(aValue1Pad, ' ') << Color::BGREEN << aValue1 << Color::RESET
         << Color::CYAN << "|" << Color::RESET << "\n";

    // Time taken row
    string label2 = "  Time taken";
    string dValue2 = formatWithCommas(dijkstra.getExecutionTime()) + " µs";
    string aValue2 = formatWithCommas(astar.getExecutionTime()) + " µs";
    int label2Pad = labelWidth - displayWidth(label2); if (label2Pad < 0) label2Pad = 0;
    int dValue2Pad = valueWidth - displayWidth(dValue2); if (dValue2Pad < 0) dValue2Pad = 0;
    int aValue2Pad = valueWidth - displayWidth(aValue2); if (aValue2Pad < 0) aValue2Pad = 0;

    cout << indent << Color::CYAN << "|" << Color::RESET << label2 << string(label2Pad, ' ')
         << Color::CYAN << "|" << Color::RESET << string(dValue2Pad, ' ') << dValue2
         << Color::CYAN << "|" << Color::RESET << string(aValue2Pad, ' ') << Color::BGREEN << aValue2 << Color::RESET
         << Color::CYAN << "|" << Color::RESET << "\n";

    // Path distance row
    ostringstream dDistStream, aDistStream;
    dDistStream << fixed << setprecision(2) << dijkstra.getTotalDistance() / 1000.0 << " km";
    aDistStream << fixed << setprecision(2) << astar.getTotalDistance() / 1000.0 << " km";
    string label3 = "  Path distance";
    string dValue3 = dDistStream.str();
    string aValue3 = aDistStream.str();
    int label3Pad = labelWidth - displayWidth(label3); if (label3Pad < 0) label3Pad = 0;
    int dValue3Pad = valueWidth - displayWidth(dValue3); if (dValue3Pad < 0) dValue3Pad = 0;
    int aValue3Pad = valueWidth - displayWidth(aValue3); if (aValue3Pad < 0) aValue3Pad = 0;

    cout << indent << Color::CYAN << "|" << Color::RESET << label3 << string(label3Pad, ' ')
         << Color::CYAN << "|" << Color::RESET << string(dValue3Pad, ' ') << dValue3
         << Color::CYAN << "|" << Color::RESET << string(aValue3Pad, ' ') << Color::BGREEN << aValue3 << Color::RESET
         << Color::CYAN << "|" << Color::RESET << "\n";

    // Path size
    string label4 = "  Path size";
    string dValue4 = formatWithCommas((long long)dijkstraPathSize);
    string aValue4 = formatWithCommas((long long)astarPathSize);
    int label4Pad = labelWidth - displayWidth(label4); if (label4Pad < 0) label4Pad = 0;
    int dValue4Pad = valueWidth - displayWidth(dValue4); if (dValue4Pad < 0) dValue4Pad = 0;
    int aValue4Pad = valueWidth - displayWidth(aValue4); if (aValue4Pad < 0) aValue4Pad = 0;

    cout << indent << Color::CYAN << "|" << Color::RESET << label4 << string(label4Pad, ' ')
         << Color::CYAN << "|" << Color::RESET << string(dValue4Pad, ' ') << dValue4
         << Color::CYAN << "|" << Color::RESET << string(aValue4Pad, ' ') << Color::BGREEN << aValue4 << Color::RESET
         << Color::CYAN << "|" << Color::RESET << "\n";

    cout << indent << Color::CYAN << "+" << horizontalLine(labelWidth) << "+"
         << horizontalLine(valueWidth) << "+" << horizontalLine(valueWidth) << "+" << Color::RESET << "\n";

    // Summary
    double dijkstraNodes = (double)dijkstra.getNodesVisited();
    double astarNodes = (double)astar.getNodesVisited();
    long long dijkstraTime = dijkstra.getExecutionTime();
    long long astarTime = astar.getExecutionTime();

    if (astarNodes < dijkstraNodes) {
        double pct = 0.0;
        if (dijkstraNodes > 0.0) {
            pct = ((dijkstraNodes - astarNodes) / dijkstraNodes) * 100.0;
        }
        long long timeDiff = dijkstraTime - astarTime;

        cout << Color::BGREEN << Color::RESET
             << "A* explored " << fixed << setprecision(1) << pct
             << "% fewer nodes and finished "
             << timeDiff << " µs faster.\n";
    } else {
        cout << Color::YELLOW << "  · " << Color::RESET
             << "Both algorithms performed similarly on this route.\n";
    }
}

void displaySingleResult(Router& astar, size_t pathSize) {
    int width = 36;
    string indent = "  ";

    ostringstream distStream;
    distStream << fixed << setprecision(2) << astar.getTotalDistance() / 1000.0 << " km";

    string title = "  PATH FOUND  (A*)";
    string line1 = "  Nodes explored    " + formatWithCommas(astar.getNodesVisited());
    string line2 = "  Time taken        " + formatWithCommas(astar.getExecutionTime()) + " µs";
    string line3 = "  Path distance     " + distStream.str();
    string line4 = "  Path size         " + formatWithCommas((long long)pathSize);

    int titlePad = width - displayWidth(title); if (titlePad < 0) titlePad = 0;
    int pad1 = width - displayWidth(line1); if (pad1 < 0) pad1 = 0;
    int pad2 = width - displayWidth(line2); if (pad2 < 0) pad2 = 0;
    int pad3 = width - displayWidth(line3); if (pad3 < 0) pad3 = 0;
    int pad4 = width - displayWidth(line4); if (pad4 < 0) pad4 = 0;

    cout << "\n";
    cout << indent << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";
    cout << indent << Color::CYAN << "|" << Color::RESET << Color::BCYAN << title << Color::RESET
         << string(titlePad, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << indent << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";
    cout << indent << Color::CYAN << "|" << Color::RESET << line1 << string(pad1, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << indent << Color::CYAN << "|" << Color::RESET << line2 << string(pad2, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << indent << Color::CYAN << "|" << Color::RESET << line3 << string(pad3, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << indent << Color::CYAN << "|" << Color::RESET << line4 << string(pad4, ' ') << Color::CYAN << "|" << Color::RESET << "\n";
    cout << indent << Color::CYAN << "+" << horizontalLine(width) << "+" << Color::RESET << "\n";
}

void displayError(const string& message) {
    cout << Color::RED << "  [ERROR] " << Color::RESET << message << "\n";
}

void displaySuccess(const string& message) {
    cout << Color::BGREEN << Color::RESET << message << "\n";
}

void displayInfo(const string& message) {
    cout << Color::YELLOW << "  · " << Color::RESET << message << "\n";
}

void displayNoRoute() {
    displayError("No route found between these locations.");
    displayInfo("Try different start/end points or check the place names.");
}

void displayNoLastRoute() {
    displayError("No route available to export. Please find a route first.");
    displayInfo("Run option [1] or [2] first to find a route.");
}

void displayExportSuccess(const string& filename, size_t pointCount) {
    displaySuccess("Route exported: " + filename + "  (" + formatWithCommas((long long)pointCount) + " waypoints)");
    displayInfo("Import at: https://www.google.com/maps/d/");
}

void displayLoadingGraph() {
    cout << Color::YELLOW << "  Loading Gainesville road network..." << Color::RESET << "\n";
}

void displayLoadSuccess(const Graph& graph) {
    long long totalEdges = 0;
    for (const auto& entry : graph.nodes) {
        totalEdges += (long long)entry.second.edges.size();
    }
    totalEdges = totalEdges / 2;

    cout << Color::BGREEN << Color::RESET
         << "Graph loaded - " << formatWithCommas((long long)graph.nodes.size()) << " nodes  ·  "
         << formatWithCommas(totalEdges) << " edges\n";
}
