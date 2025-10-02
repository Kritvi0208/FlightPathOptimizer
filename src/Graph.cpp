#include "Graph.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cctype>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// helper: strip surrounding quotes and trim (basic)
static string stripQuotes(const string& s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
        return s.substr(1, s.size() - 2);
    return s;
}

static string toUpper(const string& s) {
    string out = s;
    transform(out.begin(), out.end(), out.begin(), [](unsigned char c){ return std::toupper(c); });
    return out;
}

// haversine distance (km)
static double haversineKm(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0;
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    double a = sin(dLat/2.0)*sin(dLat/2.0) +
               cos(lat1*M_PI/180.0)*cos(lat2*M_PI/180.0)*sin(dLon/2.0)*sin(dLon/2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return R * c;
}

void Graph::addAirport(const Airport& airport) {
    if (airports.find(airport.getId()) == airports.end()) {
        airports[airport.getId()] = airport;
        string key = toUpper(stripQuotes(airport.getIata()));
        if (!key.empty() && key != "\\N") {
            iataToId[key] = airport.getId();
        }
    }
}

void Graph::addRoute(const Route& route) {
    routes.push_back(route);
    adjacency_list[route.getSourceAirportId()].emplace_back(route.getDestinationAirportId(), route);
}

void Graph::loadAirports(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening airports file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;
        while (getline(ss, field, ',')) fields.push_back(field);

        if (fields.size() < 8) continue;
        try {
            int id = stoi(fields[0]);
            string name = stripQuotes(fields[1]);
            string city = stripQuotes(fields[2]);
            string country = stripQuotes(fields[3]);
            string iata = stripQuotes(fields[4]);
            double latitude = stod(fields[6]);
            double longitude = stod(fields[7]);
            Airport a(id, name, city, country, iata, latitude, longitude);
            addAirport(a);
        } catch (...) {
            // ignore malformed line
        }
    }
    // no cout here; main prints counts so you don't get duplicates
}

void Graph::loadRoutes(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening routes file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;
        while (getline(ss, field, ',')) fields.push_back(field);

        if (fields.size() < 8) continue;
        try {
            string airline = stripQuotes(fields[0]);
            int airline_id = (fields[1] == "\\N" || fields[1].empty()) ? 0 : stoi(fields[1]);
            string source_airport = stripQuotes(fields[2]);
            int source_airport_id = (fields[3] == "\\N" || fields[3].empty()) ? 0 : stoi(fields[3]);
            string destination_airport = stripQuotes(fields[4]);
            int destination_airport_id = (fields[5] == "\\N" || fields[5].empty()) ? 0 : stoi(fields[5]);
            int stops = (fields[7] == "\\N" || fields[7].empty()) ? 0 : stoi(fields[7]);

            double time = 1.0;
            double cost = 100.0;
            double distance = 0.0;

            // compute distance if both airport ids exist
            auto itSrc = airports.find(source_airport_id);
            auto itDst = airports.find(destination_airport_id);
            if (itSrc != airports.end() && itDst != airports.end()) {
                distance = haversineKm(itSrc->second.getLatitude(),
                                       itSrc->second.getLongitude(),
                                       itDst->second.getLatitude(),
                                       itDst->second.getLongitude());
            }

            Route route(airline, airline_id, source_airport, source_airport_id,
                        destination_airport, destination_airport_id, stops, time, cost, distance);

            addRoute(route);
        } catch (...) {
            // ignore malformed line
        }
    }
    // no cout here; main prints counts
}

void Graph::computeTraffic() {
    airportTraffic.clear();

    for (const auto &route : routes) {
        if (route.getSourceAirportId() != 0)
            airportTraffic[route.getSourceAirportId()]++;
        if (route.getDestinationAirportId() != 0)
            airportTraffic[route.getDestinationAirportId()]++;
    }

    vector<pair<int,int>> tv(airportTraffic.begin(), airportTraffic.end());
    sort(tv.begin(), tv.end(), [](const pair<int,int>& a, const pair<int,int>& b){
        return a.second > b.second;
    });

    topBusiestAirports.clear();
    for (int i=0; i<5 && i < (int)tv.size(); ++i) topBusiestAirports.push_back(tv[i]);
}

vector<pair<int,int>> Graph::getTopBusiestAirports() const {
    return topBusiestAirports;
}

PathResult Graph::dijkstra(int source_id, int destination_id, const string& criteria) {
    unordered_map<int,double> dist;
    unordered_map<int,int> prev;
    for (const auto &p : airports) {
        dist[p.first] = numeric_limits<double>::infinity();
        prev[p.first] = -1;
    }

    if (airports.find(source_id) == airports.end() || airports.find(destination_id) == airports.end())
        return {{}, 0.0, 0};

    using PD = pair<double,int>;
    priority_queue<PD, vector<PD>, greater<PD>> pq;
    dist[source_id] = 0.0;
    pq.push({0.0, source_id});

    while (!pq.empty()) {
        int u = pq.top().second;
        double dcur = pq.top().first;
        pq.pop();
        if (dcur > dist[u]) continue;
        if (u == destination_id) break;

        auto it = adjacency_list.find(u);
        if (it == adjacency_list.end()) continue;

        for (const auto &nbr : it->second) {
            int v = nbr.first;
            const Route &r = nbr.second;
            double weight = 1.0;
            if (criteria == "distance") weight = r.getDistance();
            else if (criteria == "time") weight = r.getTime();
            else if (criteria == "cost") weight = r.getCost();
            else if (criteria == "stops") weight = r.getStops();

            double alt = dist[u] + weight;
            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                pq.push({alt, v});
            }
        }
    }

    vector<int> path;
    for (int at = destination_id; at != -1; at = prev[at]) {
        path.push_back(at);
        if (prev.find(at) == prev.end()) break; // safety
    }
    reverse(path.begin(), path.end());

    if (path.empty() || (path.size() == 1 && path[0] != source_id)) {
        return {{}, 0.0, 0};
    }

    PathResult res;
    res.path = path;
    res.hops = (int)path.size() - 1;
    res.totalDistance = dist[destination_id];
    if (!isfinite(res.totalDistance)) res.totalDistance = 0.0;
    return res;
}

const unordered_map<int,Airport>& Graph::getAirports() const { return airports; }
const vector<Route>& Graph::getRoutes() const { return routes; }

const Airport* Graph::getAirportByIATA(const string& iata) const {
    string key = toUpper(stripQuotes(iata));
    auto it = iataToId.find(key);
    if (it == iataToId.end()) return nullptr;
    auto ait = airports.find(it->second);
    if (ait == airports.end()) return nullptr;
    return &ait->second;
}
