#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "attraction.h"
#include "guest.h"
#include "importer.h"
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
    if ( argc != 2 ) {
        fprintf( stderr, "Requires (1) argument, file name of the attractions\n" );
        exit( 1 );
    }
    const char *fileName = argv[1];
    srand48( time( NULL ) );
    clock_t startTime = clock();
    uint numAttractions;
    uint timeOpen = 43200;
    Attraction attractions[256];

    importAttractionsFromFile( fileName, attractions, &numAttractions );
    for ( uint i = 0; i < numAttractions; ++i ) {
        attraction_print( &attractions[i] );
    }

/*
    attractions[0] = attraction_create( "EXIT", 0, ( Vec2 ) { 0, 0 }, 0, 0, 0 );
    attractions[1] = attraction_create( "Attraction 1", 100, ( Vec2 ) { 300, 0 }, 120, 4, 20 );
    attractions[2] = attraction_create( "Attraction 2", 100, ( Vec2 ) { 300, 300 }, 120, 3, 10 );
    attractions[3] = attraction_create( "Attraction 3", 100, ( Vec2 ) { -300, 300 }, 120, 3, 10 );
    attractions[4] = attraction_create( "Attraction 4", 100, ( Vec2 ) { -300, 0 }, 120, 3, 10 );
*/
    uint numGuests = 5000;
    Guest *guests = malloc( sizeof( Guest ) * numGuests );
    {
        uint attractionWeights[numAttractions];
        attractionWeights[0] = 0;
        uint enterTime;
        uint exitTime;
        for ( uint i = 0; i < numGuests; ++i ) {
            for ( uint j = 1; j < numAttractions; ++j ) {
                attractionWeights[j] = ( uint ) ( drand48() * 99 + 1 );
            }
            enterTime = ( uint ) ( drand48() * ( timeOpen - 10800 ) ); //10800 is 3 hours
            exitTime = enterTime + ( uint ) ( drand48() * ( timeOpen - enterTime - 10800 ) + 10800 );
            //exitTime = ( uint ) ( drand48() * ( timeOpen - enterTime ) + 10800 );
            guests[i] = guest_create( attractionWeights, numAttractions, enterTime, exitTime );
            printf( "%u:%u (%u)\n", enterTime, exitTime, exitTime - enterTime );
        }
    }

    //0 index is exit
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


    bool attractionLineJoined[numAttractions];
    while ( park.currentTime < park.timeOpen ) {
        for ( uint i = 0; i < numAttractions; ++i ) {
            attractionLineJoined[i] = false;
        }
        for ( uint i = 0; i < park.numGuests; ++i ) {
            Guest *guest = &park.guests[i];
            Attraction *currentAttraction = &park.attractions[guest->currentAttractionIndex];
            switch ( guest->currentStatus ) {
                case ARRIVING:
                    if ( park.currentTime < guest->enterTime ) break;
                    guest_determineNextAttraction( &park, guest, false );
                    if ( i == 0 ) {
                        LOG( "GUEST ENTERING: %u\n", park.currentTime );
                        LOG( "HEADING TO ATTRACTION: %s\n", park.attractions[guest->currentAttractionIndex].name );
                    }
                    break;
                case WALKING:
                    if ( guest->timeToAttraction == 0 ) {
                        if ( guest_decideToRideAttraction( &park, guest ) ) {
                            guest->currentStatus = IN_LINE;
                            guest->linePosition = currentAttraction->guestsInLine++;
                            if ( i == 0 ) {
                                LOG( "GUEST ARRIVED, CURRENTLY NUMBER %u IN LINE: %u\n", guest->linePosition, park.currentTime );
                            }
                            attractionLineJoined[guest->currentAttractionIndex] = true;
                        } else { 
                            guest_determineNextAttraction( &park, guest, false );
                            if ( i == 0 ) {
                                LOG( "GUEST DECIDED ON NEW ATTRACTION, %s: %u\n", park.attractions[guest->currentAttractionIndex].name, park.currentTime );
                            }
                        }
                        break;
                    }
                    ++guest->totalTimeWalking;
                    --guest->timeToAttraction;
                    break;
                case LEAVING:
                    if ( guest->timeToAttraction == 0 ) guest->currentStatus = GONE;
                    ++guest->totalTimeWalking;
                    --guest->timeToAttraction;
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
                            //LOG( "MOVED IN LINE FROM %u to %u: %u\n", guest->linePosition, guest->linePosition - currentAttraction->numGuestsLastLoaded, park.currentTime );
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
                    if ( currentAttraction->carOccupancies[guest->carIndex] ) {
                        ++guest->totalTimeRiding;
                        break;
                    }
                    //car unoccupied, just returned
                    guest_determineNextAttraction( &park, guest, true );
                    if ( i == 0 ) {
                        LOG( "GUEST FINISHED RIDE, NEXT ATTRACTION %s: %u\n", park.attractions[guest->currentAttractionIndex].name, park.currentTime );
                    }
                    break; 
                default:
                    break;
            }
        }

        for ( uint i = 1; i < park.numAttractions; ++i ) {
            Attraction *attraction = &park.attractions[i];
            if ( attractionLineJoined[i] ) {
                attraction_updateWaitTime( attraction );
            }
            attraction_updateArrivalTimes( attraction );
            attraction_updateOpenCars( attraction );
            attraction_loadOpenCar( attraction );
            attraction_unloadReturnedCar( attraction );
        }

        park.currentTime++;
    }

    uint totalRides = 0;
    uint totalTimeInPark = 0;
    uint attractionTotalRides[numAttractions];
    uint totalWaitTimes = 0;
    uint totalWalkTimes = 0;
    uint totalRideTimes = 0;
    uint totalAttractionsSkipped = 0;
    uint totalRideWeights[numAttractions];
    
    for ( uint i = 0; i < numAttractions; ++i ) {
        attractionTotalRides[i] = 0;
        totalRideWeights[i] = 0;
    }

    for ( uint i = 0; i < numGuests; ++i ) { 
        totalRides += guests[i].numAttractionsRidden;
        totalAttractionsSkipped += guests[i].totalAttractionsSkipped;
        for ( uint j = 0; j < guests[i].numAttractionsRidden; ++j ) {
            attractionTotalRides[guests[i].attractionsRiddenIndexes[j]]++;
        }
        for ( uint j = 1; j < numAttractions; ++j ) {
            totalRideWeights[j] += guests[i].attractionWeights[j];
        }
        totalTimeInPark += guests[i].exitTime - guests[i].enterTime;
        totalWaitTimes += guests[i].totalTimeInLine;
        totalWalkTimes += guests[i].totalTimeWalking;
        totalRideTimes += guests[i].totalTimeRiding;
    }

    printf( "Average Rides: %.2f\n", 1.0 * totalRides / numGuests );
    printf( "Average Rides Skipped: %.2f\n", 1.0 * totalAttractionsSkipped / numGuests );
    printf( "Average Time in Park: %.2f\n", 1.0 * totalTimeInPark / numGuests );
    printf( "Average Wait Time: %.2f\n", 1.0 * totalWaitTimes / numGuests );
    printf( "Average Walk Time: %.2f\n", 1.0 * totalWalkTimes / numGuests );
    printf( "Average Ride Time: %.2f\n", 1.0 * totalRideTimes / numGuests );
    for ( uint i = 1; i < numAttractions; ++i ) {
        printf( "Average Rides on %s: %.2f\n", park.attractions[i].name, 1.0 * attractionTotalRides[i] / numGuests );
        printf( "Average Weight: %.2f\n", 1.0 * totalRideWeights[i] / numGuests );
    }
    clock_t diffTime = clock() - startTime;
    int msec = diffTime * 1000 / CLOCKS_PER_SEC;
    printf( "Runtime: %d seconds %d milliseconds\n", msec / 1000, msec % 1000 );


    free( guests );

    return 0;
}
