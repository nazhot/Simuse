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
    uint attractionWeights[256];
    uint currentAttractionIndex;
    GuestStatus currentStatus;
    uint timeToAttraction;
    uint linePosition;
    uint carIndex;
    uint enterTime;
    uint exitTime;
    uint totalTimeInLine;
    uint numAttractionsRidden;
    uint attractionsRiddenIndexes[256];
} Guest;

int guest_determineNextAttraction( const struct Park *park, const Guest *guest );
bool guest_decideToRideAttraction( const struct Park *park, const Guest *guest );
Guest guest_create( const uint *attractionWeights, const uint numAttractions, 
                    const uint enterTime,
                    uint exitTime );


#endif
