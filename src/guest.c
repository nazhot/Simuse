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


void guest_determineNextAttraction( const Park *park, Guest *guest, const bool allowSameAttraction ) {
    //index 0, aka exit, will always have a score of 0 because the attraction weight should be 0
    uint index = 0;
    uint highestScore = 0;
    uint minWalkingTime = UINT_MAX;
    uint score;
    Attraction *currentAttraction = &park->attractions[guest->currentAttractionIndex];
    float randomFloat = drand48();
    bool choiceFound = false;
    float cumulativeTotal = 0;
    for ( uint i = 1; i < park->numAttractions; ++i ) {  
        if ( !allowSameAttraction && i == guest->currentAttractionIndex ) continue;
        cumulativeTotal += guest->attractionWeights[i];
        if ( !choiceFound && randomFloat <= cumulativeTotal ) {
            index = i;
            choiceFound = true;
        }

        uint attractionWalkTime = currentAttraction->attractionWalkTimes[i + 1];
//        score = ( guest->attractionWeights[i] * guest->attractionWeights[i] ) / 
//                ( attractionWalkTime / 60 * numTimesRiddenAttraction( guest, i ) + 1);
//        if ( score > highestScore ) {
//            highestScore = score;
//            index = i;
//        }
        if ( attractionWalkTime < minWalkingTime ) {
            minWalkingTime = attractionWalkTime;
        }
    }
    if ( minWalkingTime >= ( park->timeOpen - park->currentTime ) ) { //time to leave
        guest->currentStatus = LEAVING;
        guest->currentAttractionIndex = 0;
        guest->timeToAttraction = currentAttraction->attractionWalkTimes[0];
        return;
    }
    if ( index == guest->currentAttractionIndex ) { //staying at same attraction
        guest->timeToAttraction = 0;
        guest->currentStatus = IN_LINE;
        guest->linePosition = currentAttraction->guestsInLine++;
        return;
    }
    guest->currentStatus = WALKING;
    guest->currentAttractionIndex = index;
    guest->timeToAttraction = currentAttraction->attractionWalkTimes[index];
}

bool guest_decideToRideAttraction( const Park *park, Guest *guest ) {
    int guestExpectedWait = ( 120 + 20 * guest->numFailedAttractionsInARow ) - guest->attractionWeights[guest->currentAttractionIndex];
    bool answer = park->attractions[guest->currentAttractionIndex].currentWaitTime <= guestExpectedWait;
    if ( answer ) {
        guest->numFailedAttractionsInARow = 0;
    } else { 
        ++guest->numFailedAttractionsInARow;
        ++guest->totalAttractionsSkipped;
    }
    return answer;
}

Guest guest_create( const float *attractionWeights, const uint numAttractions,
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
                    .totalTimeWalking = 0,
                    .totalTimeRiding = 0,
                    .totalAttractionsSkipped = 0,
                    .numAttractionsRidden = 0,
                    .attractionsRiddenIndexes = {0},
                    .numFailedAttractionsInARow = 0 };
    for ( uint i = 0; i < numAttractions; ++i ) {
        guest.attractionWeights[i] = attractionWeights[i];
    }
    return guest;
}
