#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>

class Airport {
public:
    // Constructor
    Airport() = default;

    Airport(int id, const std::string& name, const std::string& city, const std::string& country,
            const std::string& iata, double latitude, double longitude);

    // Getters
    int getId() const;
    const std::string& getName() const;
    const std::string& getCity() const;
    const std::string& getCountry() const;
    const std::string& getIata() const;
    double getLatitude() const;
    double getLongitude() const;

private:
    int id;
    std::string name;
    std::string city;
    std::string country;
    std::string iata;
    double latitude;
    double longitude;
};

#endif // AIRPORT_H