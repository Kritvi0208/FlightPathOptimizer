#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>
#include <string>
#include "Airport.h"
#include "Route.h"
using namespace std;

// struct for returning path + distance + hops
struct PathResult {
    vector<int> path;
    double totalDistance;
    int hops;
};

class Graph {
private:
    unordered_map<int, int> airportTraffic;              // already exists
    vector<pair<int, int>> topBusiestAirports;           // top 5 busiest
    unordered_map<string, int> iataToId;                 // IATA -> ID map

public:
    unordered_map<int, Airport> airports;                // ID -> Airport
    vector<Route> routes;
    unordered_map<int, vector<pair<int, Route>>> adjacency_list;

    void addAirport(const Airport& airport);
    void addRoute(const Route& route);
    void loadAirports(const string& filename);
    void loadRoutes(const string& filename);

    // 🔄 Updated return type
    PathResult dijkstra(int source_id, int destination_id, const string& criteria);

    // Getter methods
    const unordered_map<int, Airport>& getAirports() const;
    const vector<Route>& getRoutes() const;

    void computeTraffic();  
    vector<pair<int, int>> getTopBusiestAirports() const;

    // lookup airport by IATA code
    const Airport* getAirportByIATA(const string& iata) const;
};

#endif // GRAPH_H
