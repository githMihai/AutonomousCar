# Project name and version
cmake_minimum_required(VERSION 3.0.0)
project(drive VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-fpermissive -O3")

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
    ./src/serialhandler/findfile.cpp
    ./src/socket/socket.cpp
    ./src/map/map.cpp
    ./src/map/node.cpp
    ./src/map/nodeset.cpp
    ./src/map/path.cpp
    ./src/map/edge.cpp
    ./src/map/edgeset.cpp
    ./src/map/vec2.cpp
    ./src/map/pathtracking.cpp
    ./src/map/heuristics.cpp
    ./src/imagemapping/point.cpp
    ./src/imagemapping/line.cpp
    ./src/imagemapping/imagecoord.cpp
    ./src/imagemapping/maprepresentation.cpp
    ./src/carcontrol/carcontrol.cpp
    ./src/image/image.cpp
    ./src/timer/timer.cpp

    ./src/testPoint.cpp
    ./src/testLine.cpp
    ./src/imagecoordTest.cpp
    ./src/mapreprTest.cpp
    ./src/moveTest.cpp
    ./src/gpsTest.cpp
    ./src/imageTest.cpp
    ./src/timerTest.cpp

    ./include/serialhandler/connexc.h
    # ./lib/serial/src/serial.cc
)
# ===================================

#  =============INCLUDES=============
include_directories( 
    ./include/gps
    ./include/serialhandler
    ./include/socket
    ./include/map
    ./include/imagemapping
    ./include/carcontrol
    ./include/observer
    ./include/image
    ./include/timer
    ./src
    ./lib/serial/include
    ${JSONCPPPATH}/include
    ${OpenCV_INCLUDE_DIRS}
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

set( OpenCV_DIR "/usr/lib/opencv" )

FIND_PACKAGE(Threads REQUIRED)  
FIND_PACKAGE(PkgConfig REQUIRED)
# FIND_PACKAGE(OpenCV REQUIRED COMPONENTS core imgproc highgui)
FIND_PACKAGE(OpenCV REQUIRED)

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
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
target_link_libraries(${PROJECT_NAME} stdc++fs)
# target_link_libraries( ${PROJECT_NAME} serial )
# ===================================