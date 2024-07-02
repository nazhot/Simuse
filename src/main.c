#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "attraction.h"
#include "guest.h"
#include "park.h"
#include <assert.h>

# define __ASSERT_VOID_CAST (void)

#ifdef DEBUG
#define LOG( ... ) printf( __VA_ARGS__ )
#else
#define LOG( ... ) (__ASSERT_VOID_CAST( 0 ))
#endif

/*
    Attraction[0] will always be expected to be the exit
*/

int main( int argc, char *argv[] ) {
    srand48( time( NULL ) );
    uint numAttractions = 5;
    uint timeOpen = 43200;
    Attraction attractions[numAttractions];

    attractions[0] = attraction_create( "EXIT", 0, ( Vec2 ) { 0, 0 }, 0, 0, 0 );
    attractions[1] = attraction_create( "Attraction 1", 100, ( Vec2 ) { 600, 0 }, 120, 3, 10 );
    attractions[2] = attraction_create( "Attraction 2", 100, ( Vec2 ) { 600, 600 }, 120, 3, 10 );
    attractions[3] = attraction_create( "Attraction 3", 100, ( Vec2 ) { -600, 600 }, 120, 3, 10 );
    attractions[4] = attraction_create( "Attraction 4", 100, ( Vec2 ) { -600, 0 }, 120, 3, 10 );

    uint numGuests = 5000;
    Guest *guests = malloc( sizeof( Guest ) * numGuests );
    {
        uint attractionWeights[numAttractions];
        attractionWeights[0] = 0;
        uint enterTime;
        uint exitTime;
        for ( uint i = 0; i < numGuests; ++i ) {
            for ( uint i = 1; i < numAttractions; ++i ) {
                attractionWeights[i] = ( uint ) drand48() * ( 99 ) + 1;
            }
            enterTime = ( uint ) drand48() * ( timeOpen - 10800 ); //10800 is 3 hours
            exitTime = ( uint ) drand48() * ( timeOpen - enterTime )+ 10800;
            guests[i] = guest_create( attractionWeights, numAttractions, enterTime, exitTime );
        }
    }

    //0 index is exit, ( 0,0 )
    for ( uint i = 0; i < numAttractions; ++i ) {
        for ( uint j = 0; j < numAttractions; ++j ) {
            attractions[i].attractionWalkTimes[j] = abs( attractions[i].position.x - attractions[j].position.x ) +
                                                        abs( attractions[i].position.y - attractions[j].position.y );
        }
    }
    Park park = { .numGuests = numGuests,
        .guests = guests,
        .timeOpen = timeOpen,
        .currentTime = 0,
        .numAttractions = numAttractions,
        .attractions = attractions };


    while ( park.currentTime < park.timeOpen ) {
        for ( uint i = 0; i < park.numGuests; ++i ) {
            Guest *guest = &park.guests[i];
            if ( park.currentTime == guest->enterTime ) {
                guest_determineNextAttraction( &park, guest );
                if ( i == 0 ) {
                    LOG( "GUEST ENTERING: %u\n", park.currentTime );
                    LOG( "HEADING TO ATTRACTION: %s\n", park.attractions[guest->currentAttractionIndex].name );
                }
                continue;
            }
            Attraction *currentAttraction = &park.attractions[guest->currentAttractionIndex];
            switch ( guest->currentStatus ) {
                case WALKING:
                    guest->timeToAttraction--;
                    if ( guest->timeToAttraction > 0 ) break;

                    guest->currentStatus = IN_LINE;
                    guest->linePosition = currentAttraction->guestsInLine++;
                    if ( i == 0 ) {
                        LOG( "GUEST ARRIVED, CURRENTLY NUMBER %u IN LINE: %u\n", guest->linePosition, park.currentTime );
                    }
                    break;
                case LEAVING:
                    guest->timeToAttraction--;
                    if ( guest->timeToAttraction == 0 ) guest->currentStatus = GONE;
                    if ( i == 0 && guest->currentStatus == GONE ) {
                        LOG( "GUEST LEFT AT %u\n", park.currentTime );
                    }
                    break;
                case IN_LINE:
                    if ( currentAttraction->numGuestsLastLoaded == 0 ) {
                        ++guest->totalTimeInLine;
                        break;
                    }
                    if ( guest->linePosition >= currentAttraction->numGuestsLastLoaded ) {
                        if ( i == 0 ) {
                            LOG( "MOVED IN LINE FROM %u to %u: %u\n", guest->linePosition, guest->linePosition - currentAttraction->numGuestsLastLoaded, park.currentTime );
                        }
                        guest->linePosition -= currentAttraction->numGuestsLastLoaded;
                        ++guest->totalTimeInLine;
                        break;
                    }

                    //guest loaded into car
                    guest->linePosition = 0;
                    guest->carIndex = currentAttraction->carIndexLastLoaded;
                    guest->currentStatus = RIDING;
                    guest->attractionsRiddenIndexes[guest->numAttractionsRidden++] = guest->currentAttractionIndex;
                    if ( i == 0 ) {
                        LOG( "STARTING RIDING: %u\n", park.currentTime );
                    }
                    break;
                case RIDING:
                    //car still occupied, means it's still going
                    if ( currentAttraction->carOccupancies[guest->carIndex] ) break;
                    //car unoccupied, just returned
                    guest_determineNextAttraction( &park, guest );
                    if ( i == 0 ) {
                        LOG( "GUEST FINISHED RIDE, NEXT ATTRACTION %s: %u\n", park.attractions[guest->currentAttractionIndex].name, park.currentTime );
                    }
                    if ( guest->currentAttractionIndex == 0 ) {
                        guest->currentStatus = LEAVING;
                        if ( i == 0 ) {
                            LOG( "GUEST LEAVING BEFORE %u exitTime: %u\n", guest->exitTime, park.currentTime );
                        }
                    }
                    break; 
                    
                default:
                    break;
            }
        }

        for ( uint i = 1; i < park.numAttractions; ++i ) {
            Attraction *attraction = &park.attractions[i];
            if ( i == 1 ) {
                LOG( "Guests in line: %u\n", attraction->guestsInLine );
            }
            attraction_updateArrivalTimes( attraction );
            attraction_updateOpenCars( attraction );
            attraction_loadOpenCar( attraction );
            attraction_unloadReturnedCar( attraction );
        }

        park.currentTime++;
    }

    return 0;
}
