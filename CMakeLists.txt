cmake_minimum_required(VERSION 3.22.1)

project(PhotoBatch)

# Collect all source files and header files in the project folder and subfolders
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
list(FILTER SOURCES EXCLUDE REGEX "CMakeCXXCompilerId.cpp")

# Add an executable target with the source files and header files
add_executable(PhotoBatch ${SOURCES} ${HEADERS})

add_subdirectory(Unittests)
target_link_libraries(PhotoBatch -lgtest)