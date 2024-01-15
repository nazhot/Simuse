#include "person.h"


static void person_updateThirst( struct Person *person ) {

}
static void person_updateHunger( struct Person *person ) {

}
static void person_updateBladder( struct Person *person ) {

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
