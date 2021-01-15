cmake_minimum_required(VERSION 3.14)
project(RowServer)

set(CMAKE_CXX_STANDARD 14)

add_library(SSD1306 STATIC include/FileDescriptor.cxx
        include/OledHardware.cxx
        include/OledPixel.cxx
        include/OledFont8x8.cxx
        include/OledFont8x12.cxx
        include/OledFont8x16.cxx
        include/OledGraphics.cxx
        include/OledI2C.cxx)

include_directories(${PROJECT_SOURCE_DIR}/include)


link_directories(${PROJECT_SOURCE_DIR})
add_executable(RowServer  main.cpp)
add_executable(RowEncode  encoder.c)
add_executable(RowDistance distance.c)
add_executable(RowLight light.c)
add_executable(RowKom kompass.c)
add_executable(Rowled oled.c)


find_library(wiringPi_LIB wiringPi)
target_link_libraries(RowServer ${wiringPi_LIB})
find_library(wiringPi_LIBdev wiringPiDev)
target_link_libraries(RowServer ${wiringPi_LIBdev})
find_library(lpthreadLIB pthread)
target_link_libraries(RowServer ${lpthreadLIB})
target_link_libraries( RowServer SSD1306)



find_library(wiringPi_LIB wiringPi)
target_link_libraries(RowKom ${wiringPi_LIB})
find_library(wiringPi_LIBdev wiringPiDev)
target_link_libraries(RowKom ${wiringPi_LIBdev})
find_library(lpthreadLIB pthread)
target_link_libraries(RowKom ${lpthreadLIB})



find_library(wiringPi_LIB wiringPi)
target_link_libraries(RowEncode ${wiringPi_LIB})
find_library(wiringPi_LIBdev wiringPiDev)
target_link_libraries(RowEncode ${wiringPi_LIBdev})
find_library(lpthreadLIB pthread)
target_link_libraries(RowEncode ${lpthreadLIB})



find_library(wiringPi_LIB wiringPi)
target_link_libraries(RowDistance ${wiringPi_LIB})
find_library(wiringPi_LIBdev wiringPiDev)
target_link_libraries(RowDistance ${wiringPi_LIBdev})
find_library(lpthreadLIB pthread)
target_link_libraries(RowDistance ${lpthreadLIB})

find_library(wiringPi_LIB wiringPi)
target_link_libraries(RowLight ${wiringPi_LIB})
find_library(wiringPi_LIBdev wiringPiDev)
target_link_libraries(RowLight ${wiringPi_LIBdev})
find_library(lpthreadLIB pthread)
target_link_libraries(RowLight ${lpthreadLIB})


