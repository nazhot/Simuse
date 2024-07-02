#include "attraction.h"
#include <stdbool.h>

Attraction attraction_create( char *name, uint popularity, Vec2 position, 
                              uint rideTime, uint numCars, uint guestsPerCar ) {
    return ( Attraction ) { .name = name,
                            .popularity = popularity,
                            .position = position,
                            .rideTime = rideTime,
                            .numCars = numCars,
                            .guestsPerCar = guestsPerCar,
                            .guestsInFastLane = 0,
                            .guestsInLine = 0,
                            .currentWaitTime = 0,
                            .currentWaitTimeConstant = ( 1.0 * rideTime / numCars ) / guestsPerCar,
                            .lineFastLaneRatio = 0,
                            .carArrivalTimes = {0},
                            .carOccupancies = {0},
                            .numOpenCars = numCars,
                            .carOpen = true,
                            .attractionWalkTimes = {0},
                            .numGuestsLastLoaded = 0,
                            .carIndexLastLoaded = 0 };
}

void attraction_updateOpenCars( Attraction *attraction ) {
    const uint numCars = attraction->numCars;
    for ( uint i = 0; i < numCars; ++i ) {
        if ( attraction->carArrivalTimes[i] == 0 && attraction->carOccupancies[i] == 0 ) {
            attraction->carOpen = true;
            attraction->firstOpenCarIndex = i;
            return;
        }
    }
    attraction->carOpen = false;
}

void attraction_loadOpenCar( Attraction *attraction ) {
    attraction->numGuestsLastLoaded = 0;
    if ( !attraction->carOpen ) return;
    uint numToLoad = attraction->guestsInLine > attraction->guestsPerCar ? attraction->guestsPerCar : attraction->guestsInLine;
    attraction->carOccupancies[attraction->firstOpenCarIndex] = numToLoad;
    attraction->carArrivalTimes[attraction->firstOpenCarIndex] = attraction->rideTime;
    attraction->numGuestsLastLoaded = numToLoad;
    attraction->carIndexLastLoaded = attraction->firstOpenCarIndex;
    attraction->guestsInLine -= numToLoad;
}

void attraction_unloadReturnedCar( Attraction *attraction ) {
    const uint numCars = attraction->numCars;
    for ( uint i = 0; i < numCars; ++i ) {
        if ( attraction->carArrivalTimes[i] == 0 && attraction->carOccupancies[i] > 0 ) {
            attraction->carOccupancies[i] = 0;
        }
    }
}

void attraction_updateArrivalTimes( Attraction *attraction ) {
    const uint numCars = attraction->numCars;
    for ( uint i = 0; i < numCars; ++i ) {
        if ( attraction->carArrivalTimes[i] > 0 ) {
            --attraction->carArrivalTimes[i];
        }
    }
}
