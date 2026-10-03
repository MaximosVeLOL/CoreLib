#ifndef __OPTIONS_H__
#define __OPTIONS_H__

//Optimizations
#define CO_O_BADCPU 0
#define CO_O_LOWMEM 0
#define CO_O_ERRORS 1

//Basic implementations

#define CO_O_MULTI_THREADING 1

//Game region
#define CO_G_USE_3D 1

//This should be defined in the CMakeLists.txt file, but this is just incase.
#ifndef CO_G_SCRIPTING
#define CO_G_SCRIPTING 1

#warning "CO_G_SCRIPTING is not defined! Defaulting to 1"

#endif



#endif