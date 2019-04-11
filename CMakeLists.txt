# Project name and version
cmake_minimum_required(VERSION 3.0.0)
project(drive VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

# Enable testing
include(CTest)
enable_testing()

# =============SOURCES==============
set(SOURCE_FILES 
    ./src/main.cpp 
    ./src/gps/gpsconnection.cpp 
    ./src/gps/gpsdata.cpp 
    ./src/serialhandler/filehandler.cpp 
    ./src/serialhandler/messageconverter.cpp 
    ./src/serialhandler/serialhandler.cpp 
    ./src/socket/socket.cpp
    ./src/map/map.cpp
    ./src/map/node.cpp
    # ./lib/serial/src/serial.cc
)
# ===================================

#  =============INCLUDES=============
include_directories( 
    ./include/gps
    ./include/serialhandler
    ./include/socket
    ./include/map
    ./src
    ./lib/serial
    ${JSONCPPPATH}/include
)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/serial)
# ===================================

# ========FILES TO BE COMPILED=======
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )
# ===================================



# ========ADDED BY CMAKE TOOL========
set( CPACK_PROJECT_NAME ${PROJECT_NAME} )
set( CPACK_PROJECT_VERSION ${PROJECT_VERSION} )
include( CPack )
# ===================================



# =============LINKING===============
# Add link to pthread
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

FIND_PACKAGE(Threads REQUIRED)  
FIND_PACKAGE(PkgConfig REQUIRED)

pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

# pkg_check_modules(SERIAL serial)
# link_libraries(${SERIAL_LIBRARIES})

# Add libserial.so
# add_library(serial SHARED IMPORTED)
# set_target_properties(serial PROPERTIES
#     IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/serial/build/libserial.so"
#     INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/lib/serial/include"
# )

add_dependencies (${PROJECT_NAME} serial)

# Linking
target_link_libraries( ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} )
# target_link_libraries( ${PROJECT_NAME} serial )
target_link_libraries( ${PROJECT_NAME} ${JSONCPP_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME} serial )
# target_link_libraries( ${PROJECT_NAME} serial )
# ===================================