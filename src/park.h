#ifndef PARK_H
#define PARK_H

#include <inttypes.h>

/*
 * General idea with this is to keep the Park hidden to the user, since
 * there should only be one created. This is the first general idea while I get
 * it up and running, I don't see a huge need to have multiple Parks all at 
 * once, worst case I would just reset the current one during the same run
 * if I really wanted to have multiple things run together.
 */

void park_initialize();
void park_initializeRandomGuests( const uint32_t numGuests );
void park_initializeAttractionsFromFile( const char *filePath );
void park_setTickRate( const uint32_t nanoseconds );
void park_run();

#endif
