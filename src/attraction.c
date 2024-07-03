#include "attraction.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

Attraction attraction_create( const char *name, const uint popularity, const Vec2 position, 
                              const uint rideTime, const uint numCars, const uint guestsPerCar ) {
    Attraction attraction = { .name = {'\0'},
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
    if ( !strncpy( attraction.name, name, sizeof( attraction.name ) - 1 ) ){
        fprintf( stderr, "Cannot initialize attraction name: %s\n", name );
        exit( 1 );
    }
    return attraction;
}

void attraction_updateWaitTime( Attraction *attraction ) {
    attraction->currentWaitTime = attraction->currentWaitTimeConstant * attraction->guestsInLine;
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
    attraction_updateWaitTime( attraction );
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

void attraction_print( Attraction *attraction ) {
    printf( "--%s--\n", attraction->name );
    printf( "Popularity: %u\tX,Y: %i,%i\n", attraction->popularity, attraction->position.x, attraction->position.y );
    printf( "Num Cars: %u\tGuests per Car: %u\n", attraction->numCars, attraction->guestsPerCar );
    printf( "Ride Time: %u\n", attraction->rideTime );
}
