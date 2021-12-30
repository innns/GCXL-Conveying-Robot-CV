cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "DEBUG")
# set(CMAKE_BUILD_TYPE "RELEASE")
if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
  add_definitions(-DRELEASE)
endif()

project(junkcar)

find_package(Threads REQUIRED)
find_package(OpenCV 4.5.2 REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS} include include/UDP
                    ./serial_port/include)

# add_subdirectory(serial_port)

aux_source_directory(src INCLUDE_SRC)
# AUX_SOURCE_DIRECTORY(./ MAIN_SRC)
aux_source_directory(./serial_port/src SERIAL_SRC)

add_library(INCLUDE ${INCLUDE_SRC})
add_library(SERIAL ${SERIAL_SRC})

add_executable(main main.cpp)

target_link_libraries(main ${OpenCV_LIBS} INCLUDE SERIAL
                      ${CMAKE_THREAD_LIBS_INIT})

add_executable(test_detector ./test/test_detector.cpp)

target_link_libraries(test_detector ${OpenCV_LIBS} INCLUDE SERIAL
                      ${CMAKE_THREAD_LIBS_INIT})

# UDP
if(1)
  add_executable(udp_server test/UDP/test_udp_client.cpp)

  target_link_libraries(udp_server INCLUDE ${CMAKE_THREAD_LIBS_INIT})

  add_executable(udp_client ./test/UDP/test_udp_client.cpp)

  target_link_libraries(udp_client INCLUDE ${CMAKE_THREAD_LIBS_INIT})
endif()

add_executable(test_config ./test/test_config.cpp)

target_link_libraries(test_config ${OpenCV_LIBS} INCLUDE SERIAL
                      ${CMAKE_THREAD_LIBS_INIT})

add_executable(test_MT ./test/test_MT.cpp)
target_link_libraries(test_MT ${OpenCV_LIBS} INCLUDE SERIAL
                      ${CMAKE_THREAD_LIBS_INIT})

if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
  message(STATUS "使用了RELEASE")
else()
  message(STATUS "最终版本记得要RELEASE")
endif()

