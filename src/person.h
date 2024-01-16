#ifndef PERSON_H
#define PERSON_H

#include "attraction.h"
#include <inttypes.h>
#include <stdbool.h>

enum PersonalityType {
    IMPATIENT,
    PATIENT
};


struct Person {
    enum PersonalityType personality;
    uint8_t expectedArrivalTime;
    uint8_t actualArrivalTime;
    uint8_t expectedLeaveTime;
    uint8_t actualLeaveTime;
    uint16_t x;
    uint16_t y;
    uint8_t hunger;
    uint8_t thirst;
    uint8_t bladder;
    uint8_t currentPatience;
    uint8_t defaultPatience;
    struct Attraction *currentAttractionTarget;
    uint32_t distanceToAttractionTarget;
    struct Attraction **favoriteRides;
    bool inAttraction;
    bool inPark;
    bool inLine;
};

void person_update( struct Person *person );

#endif
