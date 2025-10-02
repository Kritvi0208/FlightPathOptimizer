#include <iostream>
#include <fstream>
// #include <unistd.h> for mac
#include <windows.h>
#include <thread>
#include <chrono>
#include <direct.h> // for _getcwd
#include <stdlib.h>
#include <algorithm>
#include <iomanip>
#include <vector>
#include "Graph.h"

using namespace std;

// Helper: find airport ID by IATA code
int getAirportIdByIata(const Graph &graph, const string &code)
{
    for (const auto &a : graph.getAirports())
    {
        if (a.second.getIata() == code)
            return a.first; // return airport ID
    }
    return -1; // not found
}

// Print airport details
void printAirportDetails(const Graph &graph, const string &code)
{
    for (const auto &a : graph.getAirports())
    {
        if (a.second.getIata() == code)
        {
            cout << "Airport Details:\n";
            cout << "ID: " << a.first << endl;
            cout << "Name: " << a.second.getName() << endl;
            cout << "City: " << a.second.getCity() << endl;
            cout << "Country: " << a.second.getCountry() << endl;
            cout << "IATA: " << a.second.getIata() << endl;
            cout << "Latitude: " << a.second.getLatitude() << endl;
            cout << "Longitude: " << a.second.getLongitude() << endl;
            return;
        }
    }
    cout << "Error: Airport with IATA code " << code << " not found!\n";
}

// Helper: count routes from an airport
int countRoutesFromAirport(const Graph &graph, int airportId)
{
    int count = 0;
    for (const auto &route : graph.getRoutes())
    {
        if (getAirportIdByIata(graph, route.getSourceAirport()) == airportId)
            count++;
    }
    return count;
}

int main()
{
    // Ensure UTF-8 console output (Windows)
    system("chcp 65001 > nul");

    // Show current directory
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL)
        cout << "Current working directory: " << cwd << endl;
    else
    {
        perror("getcwd() error");
        return 1;
    }

    Graph graph;

    // Load data
    cout << "Trying to load ../data/airports.dat" << endl;
    cout << "Trying to load ../data/routes.dat" << endl;

    graph.loadAirports("../data/airports.dat");
    graph.loadRoutes("../data/routes.dat");

    graph.computeTraffic();

    cout << "Loaded " << graph.getAirports().size() << " airports." << endl;
    cout << "Loaded " << graph.getRoutes().size() << " routes." << endl;

    // // Print some airports for verification
    // int limit = 5;
    // for (const auto &airport : graph.getAirports())
    // {
    //     cout << "Airport: " << airport.second.getName() << " (" << airport.second.getIata() << ")" << endl;
    //     if (--limit == 0)
    //         break;
    // }

    // Menu loop
    while (true)
    {
        cout << "\n===== Flight Route Optimization Menu =====\n";
        cout << "\n";
        cout << "Sample IATA Codes: "
             << "ATL(Atlanta), DXB(Dubai), LHR(London), CDG(Paris), "
             << "DEL(Delhi), LAX(Los Angeles), BOM(Mumbai), "
             << "ORD(Chicago), BLR(Bengaluru), HND(Tokyo)\n\n";
        cout << "1. Find shortest path between two airports\n";
        cout << "2. Show details of an airport (by IATA code)\n";
        cout << "3. Show top 5 busiest airports\n";
        cout << "4. Exit\n";
        cout << "\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            string srcCode, dstCode;
            cout << "Enter source airport IATA code: ";
            cin >> srcCode;
            cout << "Enter destination airport IATA code: ";
            cin >> dstCode;

            const Airport *srcAirport = graph.getAirportByIATA(srcCode);
            const Airport *dstAirport = graph.getAirportByIATA(dstCode);

            if (!srcAirport || !dstAirport)
            {
                cout << "Error: Invalid source or destination code!\n";
                continue;
            }

            PathResult res = graph.dijkstra(srcAirport->getId(), dstAirport->getId(), "distance");

            if (res.path.empty())
            {
                cout << "No path found between " << srcCode << " and " << dstCode << ".\n";
            }
            else
            {
                cout << "\nShortest path from " << srcCode << " to " << dstCode << ":\n\n";
                for (size_t i = 0; i < res.path.size(); i++)
                {
                    const Airport &ap = graph.getAirports().at(res.path[i]);
                    cout << ap.getName() << " (" << ap.getIata() << ")";
                    if (i != res.path.size() - 1)
                        cout << " -> ";
                }
                cout << "\n\nTotal distance: " << fixed << setprecision(2) << res.totalDistance << " km";
                cout << "\nNo. of hops: " << res.hops << endl;
            }
        }

            else if (choice == 2)
            {
                string code;
                cout << "Enter airport IATA code: ";
                cin >> code;
                cout << "\n";
                const Airport *ap = graph.getAirportByIATA(code);
                if (ap)
                {
                    cout << "Airport Details:\n";
                    cout << "ID: " << ap->getId() << endl;
                    cout << "Name: " << ap->getName() << endl;
                    cout << "City: " << ap->getCity() << endl;
                    cout << "Country: " << ap->getCountry() << endl;
                    cout << "IATA: " << ap->getIata() << endl;
                    cout << "Latitude: " << ap->getLatitude() << endl;
                    cout << "Longitude: " << ap->getLongitude() << endl;

                }
                else
                {
                    cout << "Error: Airport with IATA code " << code << " not found!\n";
                }
            }
            else if (choice == 3)
            {
                cout << "Top 5 Busiest Airports:\n";
                cout << "\n";
                for (const auto &p : graph.getTopBusiestAirports())
                {
                    int airportId = p.first;
                    int traffic = p.second;
                    if (graph.getAirports().count(airportId))
                    {
                        const Airport &airport = graph.getAirports().at(airportId);
                        cout << airport.getName() << " (" << airport.getIata() << ") - Connections: " << traffic << endl;
                    }
                }
            }

            else if (choice == 4)
            {
                cout << "Exiting program. Goodbye!\n";
                break;
            }
            else
            {
                cout << "Invalid choice, try again.\n";
            }
        }

        return 0;
    }
