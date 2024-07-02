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
