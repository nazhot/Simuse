#ifndef ATTRACTION_H
#define ATTRACTION_H

#include <stdlib.h>

typedef struct Vec2 {
    int x;
    int y;
} Vec2;

typedef struct Attraction {
    uint popularity;
    char name[256];
    Vec2 position;
    uint guestsInFastLane;
    uint rideTime;
    uint numCars;
    uint guestsPerCar;
    uint guestsInLine;
    uint currentWaitTime;
    uint lineFastLaneRatio;
    uint carArrivalTimes[256];
    uint carOccupancies[256];
    uint firstOpenCarIndex;
    uint numOpenCars;
} Attraction;

#endif
