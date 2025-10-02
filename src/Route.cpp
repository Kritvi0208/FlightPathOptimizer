#include "Route.h"

// Constructor
Route::Route(const std::string& airline, int airline_id,
             const std::string& source_airport, int source_airport_id,
             const std::string& destination_airport, int destination_airport_id,
             int stops, double time, double cost, double distance)
    : airline(airline), airline_id(airline_id),
      source_airport(source_airport), source_airport_id(source_airport_id),
      destination_airport(destination_airport), destination_airport_id(destination_airport_id),
      stops(stops), time(time), cost(cost), distance(distance) {}

// Getters
std::string Route::getAirline() const { return airline; }
int Route::getAirlineId() const { return airline_id; }
std::string Route::getSourceAirport() const { return source_airport; }
int Route::getSourceAirportId() const { return source_airport_id; }
std::string Route::getDestinationAirport() const { return destination_airport; }
int Route::getDestinationAirportId() const { return destination_airport_id; }
int Route::getStops() const { return stops; }
double Route::getTime() const { return time; }
double Route::getCost() const { return cost; }
double Route::getDistance() const { return distance; }
