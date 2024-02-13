cmake_minimum_required (VERSION 3.8)

project ("PhotoBatch")

# Add an executable target with the source files and header files
add_executable(PhotoBatch
	"PhotoBatch.cpp"
	"ArgumentParser.h"
	"ArgumentParser.cpp"
	"Utils.h"
	"Utils.cpp" 
	"Mode.h"
	"Mode.cpp"
	"RenameMode.h"
	"RenameMode.cpp"
	"ConvertMode.h"
	"ConvertMode.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PhotoBatch PROPERTY CXX_STANDARD 20)
endif()
set(FETCHCONTENT_QUIET OFF CACHE BOOL "Desabilita o modo quiet to FetchContent" FORCE)

include(FetchContent)

add_subdirectory(Unittests)
add_subdirectory(External)

target_link_libraries(PhotoBatch PRIVATE stb)