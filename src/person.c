#include "person.h"
#include <math.h>

#define WALKING_SPEED 5

static void person_updateThirst( struct Person *person ) {
    if ( person->thirst < 255 ) {
        person->thirst++;
    }
}
static void person_updateHunger( struct Person *person ) {
    if ( person->hunger < 255 ) {
        person->hunger++;
    }
}
static void person_updateBladder( struct Person *person ) {
    if ( person->bladder < 255 ) {
        person->bladder++;
    }
}

static void person_setDistanceToAttraction( struct Person *person ) {
    uint16_t xDiff = person->x > person->currentAttractionTarget->x ?
                     person->x - person->currentAttractionTarget->x :
                     person->currentAttractionTarget->x - person->x;
    uint16_t yDiff = person->y > person->currentAttractionTarget->y ?
                     person->y - person->currentAttractionTarget->y :
                     person->currentAttractionTarget->y - person->y;
    person->distanceToAttractionTarget =  ( int ) sqrt( xDiff * xDiff + yDiff * yDiff );
}

static void person_walkTowardsAttraction( struct Person *person ) {
     
}
static void person_checkForArrivalTime( struct Person *person ) { 

}

void person_update( struct Person *person ) {
    if ( person->inPark ) {
        person_updateThirst( person );
        person_updateHunger( person );
        person_updateBladder( person );
        if ( person->inAttraction ) {

        } else {
            person_walkTowardsAttraction( person );
        }
    } else {
        person_checkForArrivalTime( person ); 
    }
    
}
