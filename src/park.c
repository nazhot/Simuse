#include "park.h"
#include "attraction.h"
#include <stdbool.h>
#include <time.h>

struct Park {
    float tickSeconds;
    uint32_t lastTick;
    uint8_t hour;
    struct Attraction **attractionList;
    struct Person **personList;
    uint8_t numAttractions;
    uint32_t numPersons;
    uint32_t numGuestsInPark;
    uint32_t numGuestsIntoParkPerHour;
};


static struct Park park;
static bool initialized = false;


void park_initialize() {
    park = ( struct Park ) {0};
    //TODO: work on defaults here
    initialized = true;
}

static void park_updateAttractions() {
    for ( int i = 0; i < park.numAttractions; ++i ) {
        attraction_update( park.attractionList[i] );
    }
}

static void park_updatePersons() {

}

void park_run() {
    if ( !initialized ) return;
    while ( clock() * 1.0 / CLOCKS_PER_SEC - park.lastTick < park.tickSeconds );
    park_updateAttractions();
    park_updatePersons();
}
