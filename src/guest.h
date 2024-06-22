#ifndef GUEST_H
#define GUEST_H

#include <stdlib.h>

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

#endif
