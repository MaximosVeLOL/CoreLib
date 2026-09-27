# CMakeList.txt : CMake project for CoreLib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>")


set(CORE_USE_SCRIPTS ON)

add_subdirectory(vendored/glad)
add_subdirectory(vendored/glfw)

project ("CoreLib")




# Add source to this project's executable.
add_executable (CoreLib "source/test.cpp" "source/comp/input.cpp" "source/comp/renderer.cpp")

if(CORE_USE_SCRIPTS)
	add_subdirectory(vendored/angelscript/projects/cmake)
	#add_subdirectory(vendored/angelscript/)
	target_compile_definitions(CoreLib PRIVATE CO_G_SCRIPTING=1)

else()
	target_compile_definitions(CoreLib PRIVATE CO_G_SCRIPTING=0)
endif()

target_include_directories(CoreLib PUBLIC include)

target_link_libraries(CoreLib PRIVATE glad)
target_link_libraries(CoreLib PRIVATE glfw)
target_link_libraries(CoreLib PRIVATE angelscript)
	
	
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CoreLib PROPERTY CXX_STANDARD 23)
endif()



# TODO: Add tests and install targets if needed.
