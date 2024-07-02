#include "guest.h"
#include "park.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int numTimesRiddenAttraction( const Guest *guest, const uint attractionIndex ) {
    uint numTimes = 0;
    for ( uint i = 0; i < 256; ++i ) { //TODO: update 256 to be stored in a variable somewhere
        if ( guest->attractionsRiddenIndexes[i] == attractionIndex ) {
            ++numTimes;
        }
    } 
    return numTimes;
}


int guest_determineNextAttraction( const Park *park, const Guest *guest ) {
    uint index = 0;
    uint highestScore = 0;
    uint minWalkingTime = UINT_MAX;
    uint score;
    Attraction *currentAttraction = &park->attractions[guest->currentAttractionIndex];
    for ( uint i = 0; i < park->numAttractions; ++i ) {  
        uint attractionWalkTime = currentAttraction->attractionWalkTimes[i + 1];
        score = guest->attractionWeights[i] / 
                ( attractionWalkTime * numTimesRiddenAttraction( guest, i ) + 1);
        if ( score > highestScore ) {
            highestScore = score;
            index = i;
        }
        if ( attractionWalkTime < minWalkingTime ) {
            minWalkingTime = attractionWalkTime;
        }
    }
    if ( minWalkingTime >= ( park->timeOpen - park->currentTime ) ) {
        return 0;
    }
    return index + 1;

}

bool guest_decideToRideAttraction( const Park *park, const Guest *guest ) {
    return true;
}

Guest guest_create( const uint *attractionWeights, const uint numAttractions,
                    const uint enterTime, const uint exitTime ) {
    Guest guest = { .attractionWeights = {0},
                    .currentAttractionIndex = 0,
                    .currentStatus = ARRIVING,
                    .timeToAttraction = 0,
                    .linePosition = 0,
                    .carIndex = 0,
                    .enterTime = enterTime,
                    .exitTime = exitTime,
                    .totalTimeInLine = 0,
                    .numAttractionsRidden = 0,
                    .attractionsRiddenIndexes = {0} };
    return guest;
}
