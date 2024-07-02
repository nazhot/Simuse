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
                            .attractionWalkTimes = {0} }; 
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

void attraction_unloadReturnedCar( Attraction *attraction ) {
    const uint numCars = attraction->numCars;
    for ( uint i = 0; i < numCars; ++i ) {
        if ( attraction->carArrivalTimes[i] == 0 && attraction->carOccupancies[i] > 0 ) {
            attraction->carOccupancies[i] = 0;
        }
    }
}
