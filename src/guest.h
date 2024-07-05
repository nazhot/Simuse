#ifndef GUEST_H
#define GUEST_H

#include <stdlib.h>
#include <stdbool.h>

struct Park;

typedef enum GuestStatus{
    ARRIVING,
    WALKING,
    IN_LINE,
    RIDING,
    LEAVING,
    GONE
} GuestStatus;

typedef struct Guest {
    float attractionWeights[256];
    uint currentAttractionIndex;
    GuestStatus currentStatus;
    uint timeToAttraction;
    uint linePosition;
    uint carIndex;
    uint enterTime;
    uint exitTime;
    uint totalTimeInLine;
    uint totalTimeWalking;
    uint totalTimeRiding;
    uint totalAttractionsSkipped;
    uint numAttractionsRidden;
    uint attractionsRiddenIndexes[256];
    uint numFailedAttractionsInARow;
} Guest;

void guest_determineNextAttraction( const struct Park *park, Guest *guest, const bool allowSameAttraction );
bool guest_decideToRideAttraction( const struct Park *park, Guest *guest );
Guest guest_create( const float *attractionWeights, const uint numAttractions, 
                    const uint enterTime,
                    uint exitTime );


#endif
