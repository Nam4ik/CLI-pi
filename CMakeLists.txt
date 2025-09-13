cmake_minimum_required(VERSION 3.5)

project(termpi)

set(CMAKE_C_STANDARD 99)
set(CC clang)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MPFR REQUIRED mpfr)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wextra -Werror -v")

set(SOURCES
    src/main.c 
    src/config.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${MPFR_LIBRARIES} m)

include_directories(${MPFR_INCLUDE_DIRS})

