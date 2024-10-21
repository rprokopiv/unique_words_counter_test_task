cmake_minimum_required(VERSION 3.10)  # Minimum required version of CMake
project(UniqueWordsCounter VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify source files
set(SOURCES
    main.cpp
    FileDataProvider.cpp
)

# Automatically find all header files in the include directory
file(GLOB HEADERS *.h)

# Add the executable target
add_executable(UniqueWordsCounter ${SOURCES} ${HEADERS})

# Include directories for headers
target_include_directories(UniqueWordsCounter PRIVATE include)
