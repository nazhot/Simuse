#include <stdio.h>
#include <stdlib.h>
#include "attraction.h"
#include "guest.h"
#include "park.h"

int main( int argc, char *argv[] ) {
    uint numAttractions = 4;
    Attraction attractions[numAttractions];

    attractions[0] = attraction_create( "Attraction 1", 100, ( Vec2 ) { 600, 0 }, 120, 3, 10 );
    attractions[1] = attraction_create( "Attraction 2", 100, ( Vec2 ) { 600, 600 }, 120, 3, 10 );
    attractions[2] = attraction_create( "Attraction 3", 100, ( Vec2 ) { -600, 600 }, 120, 3, 10 );
    attractions[3] = attraction_create( "Attraction 4", 100, ( Vec2 ) { -600, 0 }, 120, 3, 10 );
    

    uint numGuests = 500;
    Guest guests[numGuests];
    for ( uint i = 0; i < numGuests; ++i ) {
        
    }

    //0 index is exit, ( 0,0 )
    uint attractionWalkTimes[numAttractions + 1][numAttractions + 1];
    attractionWalkTimes[0][0] = 0;
    for ( uint i = 0; i < numAttractions; ++i ) {
        attractionWalkTimes[0][i + 1] = abs( attractions[i].position.x ) + abs( attractions[i].position.y );
        attractionWalkTimes[i + 1][0] = abs( attractions[i].position.x ) + abs( attractions[i].position.y );
        for ( uint j = 0; j < numAttractions; ++j ) {
            attractionWalkTimes[i + 1][j + 1] = abs( attractions[i].position.x - attractions[j].position.x ) +
                                                abs( attractions[i].position.y - attractions[j].position.y );
        }
    }
    Park park = { .numGuests = numGuests,
                  .guests = guests,
                  .timeOpen = 43200, //half a day
                  .currentTime = 0,
                  .numAttractions = numAttractions,
                  .attractions = attractions };


    while ( park.currentTime < park.timeOpen ) {
        for ( uint i = 0; i < park.numGuests; ++i ) {
            Guest *guest = &park.guests[i];
            Attraction *currentAttraction = &park.attractions[guest->currentAttractionIndex];
            if ( park.currentTime == guest->enterTime ) {
                guest->currentAttractionIndex = guest_determineNextAttraction( &park, guest );
                guest->currentStatus = WALKING;
                guest->timeToAttraction = park.attractionWalkTimes[0][guest->currentAttractionIndex];
                continue;
            }
            switch ( guest->currentStatus ) {
                case WALKING:
                    guest->timeToAttraction--;
                    if ( guest->timeToAttraction > 0 || !guest_decideToRideAttraction( &park, guest ) ) break;

                    guest->currentStatus = IN_LINE;
                    guest->linePosition = currentAttraction->guestsInLine++;
                    break;
                case LEAVING:
                    guest->timeToAttraction--;
                    if ( guest->timeToAttraction == 0 ) guest->currentStatus = GONE;
                    break;
                case IN_LINE:
                    if ( currentAttraction->numOpenCars > 0 ) {
                        if ( guest->linePosition < park.attractions[guest->currentAttractionIndex].guestsPerCar ) {
                            guest->linePosition = 0;
                            guest->currentStatus = RIDING;
                            currentAttraction->carOccupancies[currentAttraction->firstOpenCarIndex]++;
                        } else {
                            guest->linePosition -= currentAttraction->guestsPerCar;
                            guest->totalTimeInLine++;
                        }
                    } else {
                        guest->totalTimeInLine++;
                    }
                default:
                    break;
            }
        }

        for ( uint i = 0; i < park.numAttractions; ++i ) {
            Attraction *attraction = &park.attractions[i];
            for ( uint j = 0; j < attraction->numCars; ++j ) {
                if ( attraction->carArrivalTimes[j] == 0 && attraction->carOccupancies[j] > 0 ) {
                    attraction->carArrivalTimes[j] = attraction->rideTime;
                    attraction->guestsInLine -= attraction->guestsPerCar;
                }  else if ( attraction->carArrivalTimes[j] > 0 ) {
                    attraction->carArrivalTimes[j]--;
                }
            }
            attraction->currentWaitTime = attraction->currentWaitTimeConstant * attraction->guestsInLine;
        }

        park.currentTime++;
    }

    return 0;
}
