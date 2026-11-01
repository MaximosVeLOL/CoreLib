# CMakeList.txt : CMake project for CoreLib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.28)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>")


set_property(GLOBAL PROPERTY CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(USE_SDL OFF)

set(CORE_USE_SCRIPTS OFF)

set(CORE_TESTING OFF)

if(DEFINED WIN32 OR DEFINED EMSCRIPTEN OR DEFINED ANDROID)
	set(USE_SDL ON)
endif()

#add_subdirectory(vendored/glad)
if(USE_SDL)
	add_subdirectory(vendored/SDL)
	add_subdirectory(vendored/SDL_ttf)
	#add_subdirectory(vendored/SDL_mixer)
endif()
project ("CoreLib")


if(EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif()



# Add source to this project's executable.
if(CORE_TESTING)
	add_executable (CoreLib "source/comp/input.cpp" "source/comp/renderer.cpp" "source/memory.cpp" "source/comp/asset.cpp")
else()
	add_library (CoreLib SHARED "source/comp/input.cpp" "source/comp/renderer.cpp" "source/memory.cpp" "source/comp/asset.cpp" "source/string.cpp" "source/utility.cpp" "source/filesystem.cpp" "source/module.cpp")
endif()
if(CORE_USE_SCRIPTS)
	add_subdirectory(vendored/angelscript/projects/cmake)
	#add_subdirectory(vendored/angelscript/)
	target_compile_definitions(CoreLib PRIVATE CO_G_SCRIPTING=1)

else()
	target_compile_definitions(CoreLib PRIVATE CO_G_SCRIPTING=0)
endif()

if(USE_SDL)
	target_compile_definitions(CoreLib PUBLIC CO_TARGET=0)
else()
	target_compile_definitions(CoreLib PUBLIC CO_TARGET=67)
endif()

target_include_directories(CoreLib PUBLIC include)

#target_link_libraries(CoreLib PRIVATE glad)
if(USE_SDL)
	target_link_libraries(CoreLib PRIVATE SDL3::SDL3)
	target_link_libraries(CoreLib PRIVATE SDL3_ttf::SDL3_ttf)
	#target_link_libraries(CoreLib PRIVATE SDL_mixer)
endif()
if(CORE_USE_SCRIPTS)
	target_link_libraries(CoreLib PRIVATE angelscript)
endif()
	
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CoreLib PROPERTY CXX_STANDARD 23)
endif()



# TODO: Add tests and install targets if needed.
