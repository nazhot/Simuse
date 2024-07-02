#ifndef ATTRACTION_H
#define ATTRACTION_H

#include <stdlib.h>
#include <stdbool.h>

//each unit of x/y is 4 feet, so that a distance of 1 is ~1 sec of travel time

typedef struct Vec2 {
    int x;
    int y;
} Vec2;

typedef struct Attraction {
    char *name;
    uint popularity;
    Vec2 position;
    uint guestsInFastLane;
    uint rideTime;
    uint numCars;
    uint guestsPerCar;
    uint guestsInLine;
    uint currentWaitTime;
    float currentWaitTimeConstant;
    uint lineFastLaneRatio;
    uint carArrivalTimes[256];
    uint carOccupancies[256];
    uint firstOpenCarIndex;
    uint numOpenCars;
    bool carOpen;
    uint attractionWalkTimes[256];
} Attraction;

Attraction attraction_create( char *name, uint popularity, Vec2 position, 
                              uint rideTime, uint numCars, uint guestsPerCar );
void attraction_updateOpenCars( Attraction *attraction );
uint attraction_loadOpenCar( Attraction *attraction );
void attraction_unloadReturnedCar( Attraction *attraction );
void attraction_updateArrivalTimes( Attraction *attraction );

#endif
