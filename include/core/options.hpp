#ifndef __OPTIONS_H__
#define __OPTIONS_H__

//Optimizations
#define CO_O_BADCPU 0
#define CO_O_LOWMEM 0
#define CO_O_ERRORS 1

//Basic implementations

#define CO_O_MULTI_THREADING 1
#if CO_O_MULTI_THREADING
#include <thread>
#include <atomic>
#include <mutex>
#endif

//Game region
#define CO_G_USE_3D 1

//This should be defined in the CMakeLists.txt file, but this is just incase.
#ifndef CO_G_SCRIPTING
#define CO_G_SCRIPTING 1

#warning "CO_G_SCRIPTING is not defined! Defaulting to 1"
#endif

//Are we using our packed filesystem? (one packed file for all files)
#define CO_FS_PACKED 0

//Can we load external files using dlls or shared objects? (Useful for older systems)
#define CO_FS_FIXEDCODE 0

#endif