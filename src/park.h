#ifndef PARK_H
#define PARK_H

#include <stdlib.h>
#include "attraction.h"
#include "guest.h"

typedef struct Park {
    uint timeOpen;
    uint currentTime;
    uint numAttractions;
    Attraction *attractions;
    uint numGuests;
    Guest *guests;
    uint **attractionWalkTimes;
} Park;

int guest_determineNextAttraction( Park *park, Guest *guest );

#endif
