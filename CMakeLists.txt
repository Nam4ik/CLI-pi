set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wextra -Werror -v")

project(termpi)

set(CMAKE_C_STANDART 99)
set(CC clang) 

find_package(PkgConfig REQUIRED) 
pkg_check_modules(MPFR REQUIRED mpfr) 

set(SOURCES
    main.c 
    config.c
)

add_executable(${PROJECT_NAME} ${MPFR_LIBRARIES})

