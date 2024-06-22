#ifndef PARK_H
#define PARK_H

#include <stdlib.h>
#include "attraction.h"
#include "guest.h"

typedef struct Park {
    uint timeOpen;
    uint currentTime;
    Attraction *attractions;
    Guest *guests;
} Park;

#endif
