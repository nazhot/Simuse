#ifndef ATTRACTION_H
#define ATTRACTION_H

#include <inttypes.h>
enum AttractionType {
    RIDE,
    FOOD_STAND
};

struct Attraction {
    enum AttractionType type;
    uint16_t x;
    uint16_t y;
    uint8_t thrillRating;
    uint8_t minutesPerRide;
    uint16_t peoplePerHour;
    uint16_t numPeopleRiding; 
};

#endif
