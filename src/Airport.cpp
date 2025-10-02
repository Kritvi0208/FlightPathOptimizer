#include "Airport.h"

Airport::Airport(int id, const std::string& name, const std::string& city, const std::string& country,
                 const std::string& iata, double latitude, double longitude)
    : id(id), name(name), city(city), country(country), iata(iata), latitude(latitude), longitude(longitude) {}

int Airport::getId() const { return id; }
const std::string& Airport::getName() const { return name; }
const std::string& Airport::getCity() const { return city; }
const std::string& Airport::getCountry() const { return country; }
const std::string& Airport::getIata() const { return iata; }
double Airport::getLatitude() const { return latitude; }
double Airport::getLongitude() const { return longitude; }


