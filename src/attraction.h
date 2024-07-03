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
    char name[256];
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
    uint numGuestsLastLoaded;
    uint carIndexLastLoaded;
} Attraction;

Attraction attraction_create( const char *name, const uint popularity, const Vec2 position, 
                              const uint rideTime, const uint numCars, const uint guestsPerCar );
void attraction_updateWaitTime( Attraction *attraction );
void attraction_updateOpenCars( Attraction *attraction );
void attraction_loadOpenCar( Attraction *attraction );
void attraction_unloadReturnedCar( Attraction *attraction );
void attraction_updateArrivalTimes( Attraction *attraction );
void attraction_print( Attraction *attraction );

#endif
