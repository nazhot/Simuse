#define INI_HANDLER_LINENO 1
#include "attraction.h"
#include "importer.h"
#include "ini.h"
#include <stdlib.h>
#include <string.h>

typedef struct AttractionArray {
    Attraction *attractions;
    uint numAttractions;
} AttractionArray;

static int attractionImportHandler( void *user, const char *section, const char *name, const char *value, int lineno ) {
    static char lastSection[256] = "";

    AttractionArray *array = ( AttractionArray* ) user;

    //new section
    if ( strcmp( lastSection, section ) ) {
        strncpy( lastSection, section, 255 );
        array->attractions[array->numAttractions++] = attraction_create( lastSection, 0, ( Vec2 ) {0,0}, 0, 0, 0 );
    }

    Attraction *currentAttraction = &array->attractions[array->numAttractions - 1];

    if ( !strcmp( name, "Popularity" ) ) {
        currentAttraction->popularity = atoi( value );
        return 0;
    }

    if ( !strcmp( name, "X" ) ) {
        currentAttraction->position.x = atoi( value );
        return 0;
    }

    if ( !strcmp( name, "Y" ) ) {
        currentAttraction->position.y = atoi( value );
        return 0;
    }

    if ( !strcmp( name, "NumCars" ) ) {
        currentAttraction->numCars = atoi( value );
        return 0;
    }

    if ( !strcmp( name, "RideTime" ) ) {
        currentAttraction->rideTime = atoi( value );
        return 0;
    }

    if ( !strcmp( name, "GuestsPerCar" ) ) {
        currentAttraction->guestsPerCar = atoi( value );
        return 0;
    }

    return 0;
}


void importAttractionsFromFile( const char *fileName, Attraction *attractions, uint *numAttractions ) {
    AttractionArray array = { .numAttractions = 0,
                              .attractions = attractions };

    int attractionsError = ini_parse( fileName, attractionImportHandler, &array );
    ( void ) attractionsError;
    *numAttractions = array.numAttractions;
}
