cmake_minimum_required(VERSION 3.16)
project(pool_game)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Define directories
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Automatically find all .cpp files in the src directory
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")

# Automatically find all .h files in the include directory (optional, for IDE support)
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.h")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Optional: Print discovered files (for debugging)
message(STATUS "Found source files: ${SOURCES}")
message(STATUS "Found header files: ${HEADERS}")

# Link to the actual SDL3 library.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Add include directory
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})


