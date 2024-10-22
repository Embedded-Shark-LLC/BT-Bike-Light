cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(nordic)

target_include_directories(app PRIVATE src src/drivers)

# Get all source and header files
file(GLOB_RECURSE SOURCES src/*.c src/drivers/*.c)
file(GLOB_RECURSE HEADERS src/*.h src/drivers/*.h)

# Add sources and header files to project
target_sources(app PRIVATE ${SOURCES})
target_sources(app PRIVATE ${HEADERS})