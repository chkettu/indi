cmake_minimum_required(VERSION 2.4.7)
PROJECT(indi_lacerta_fbc C CXX)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake_modules/")
include(GNUInstallDirs)

set(INDI_LACERTA_FBC_VERSION_MAJOR 0)
set(INDI_LACERTA_FBC_VERSION_MINOR 1)

find_package(INDI REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/indi_lacerta_fbc.xml.cmake ${CMAKE_CURRENT_BINARY_DIR}/indi_lacerta_fbc.xml )

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${INDI_INCLUDE_DIR})

include(CMakeCommon)

########### Astromechanics Focuser ###########
set(indilacertafbc_SRCS
   ${CMAKE_CURRENT_SOURCE_DIR}/lacerta_fbc.cpp
   )

IF (UNITY_BUILD)
    ENABLE_UNITY_BUILD(indilacertafbc indilacertafbc_SRCS 10 cpp)
ENDIF ()

add_executable(indi_lacerta_fbc ${indilacertafbc_SRCS})

target_link_libraries(indi_lacerta_fbc ${INDI_DRIVER_LIBRARIES} pthread)

install(TARGETS indi_lacerta_fbc RUNTIME DESTINATION bin )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/indi_lacerta_fbc.xml DESTINATION ${INDI_DATA_DIR})


