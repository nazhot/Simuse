#include <stdio.h>
#include <stdlib.h>
#include "attraction.h"
#include "guest.h"
#include "park.h"

int main( int argc, char *argv[] ) {
    uint numAttractions = 5;
    Attraction attractions[numAttractions];
    

    uint numGuests = 500;
    Guest guests[numGuests];
    for ( uint i = 0; i < numGuests; ++i ) {
        
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
                guest->currentAttractionIndex = guest_determineNextAttraction( &park, &guest );
                guest->currentStatus = WALKING;
                guest->timeToAttraction = park.attractionWalkTimes[0][guest->currentAttractionIndex];
                continue;
            }
            switch ( guest->currentStatus ) {
                case WALKING:
                    guest->timeToAttraction--;
                    if ( guest->timeToAttraction > 0 || !guest_decideToRideAttraction( guest, &park ) ) break;

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
        park.currentTime++;
    }

    return 0;
}
