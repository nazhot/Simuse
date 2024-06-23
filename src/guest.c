#include "guest.h"
#include "park.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int numTimesRiddenAttraction( Guest *guest, uint attractionIndex ) {
    uint numTimes = 0;
    for ( uint i = 0; i < 256; ++i ) { //TODO: update 256 to be stored in a variable somewhere
        if ( guest->attractionsRiddenIndexes[i] == attractionIndex ) {
            ++numTimes;
        }
    } 
    return numTimes;
}


int guest_determineNextAttraction( Park *park, Guest *guest ) {
    uint index = 0;
    uint highestScore = 0;
    uint minWalkingTime = UINT_MAX;
    uint score;
    for ( uint i = 0; i < park->numAttractions; ++i ) {  
        score = guest->attractionWeights[i] / 
                ( park->attractionWalkTimes[i][guest->currentAttractionIndex] * numTimesRiddenAttraction( guest, i ) );
        if ( score > highestScore ) {
            highestScore = score;
            index = i;
        }
        if ( park->attractionWalkTimes[i][guest->currentAttractionIndex] < minWalkingTime ) {
            minWalkingTime = park->attractionWalkTimes[i][guest->currentAttractionIndex];
        }
    }
    if ( minWalkingTime >= ( park->timeOpen - park->currentTime ) ) {
        return -1;
    }
    return index;

}

bool guest_decideToRideAttraction( Park *park, Guest *guest ) {
    return true;
}
