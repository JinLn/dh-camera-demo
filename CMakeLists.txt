cmake_minimum_required(VERSION 3.16)
project(dh_camera_demo)

set(DH_CAM_PATH /home/jinln/installProgram/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.2.2112.9071/Galaxy_camera)

add_compile_options(
        -std=c++2a
        -DELPP_THREAD_SAFE
        -DELPP_UNICODE
)

# Options for different build type.
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_options(
            # Complete debugger support.
            -O0
            # Default log file path.
            -DELPP_DEFAULT_LOG_FILE="../log/debug.log"
            # Enable stack backtrace log when the program crashes.
            -DELPP_FEATURE_CRASH_LOG
            # Enable performance tracking macro: TIMED_SCOPE, TIMED_FUNC, etc.
            -DELPP_FEATURE_PERFORMANCE_TRACKING
    )
elseif (CMAKE_BUILD_TYPE STREQUAL Release)
    add_compile_options(
            # Maximum speed.
            -O3
            -flto
            # Disable this flag when error occurs.
            -march=native
            -DELPP_DEFAULT_LOG_FILE="../log/release.log"
            # Disable LOG(DEBUG) to improve performance.
            -DELPP_DISABLE_DEBUG_LOGS
    )
endif ()

set(OpenCV_DIR /home/jinln/installProgram/opencv-4.5.4/build)
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${DH_CAM_PATH}/inc)
file(GLOB DH_LIBS ${DH_CAM_PATH}/lib/x86_64/*.so)

include_directories(/usr/include/x86_64-linux-gnu)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#[[add_library(${PROJECT_NAME} SHARED
        dh_camera.cpp
        dh_camera.h
        )]]

#file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/dh_camera.cpp)

file(GLOB LOGGING_SRC ${CMAKE_CURRENT_SOURCE_DIR}/easylogging++/*.c*)

add_executable(dh_camera_demo ${SRC} ${LOGGING_SRC} main.cpp)

target_link_libraries(dh_camera_demo
        ${DH_LIBS}
        ${OpenCV_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
        )

add_executable(dh_camera_double ${SRC} ${LOGGING_SRC} main2cam.cpp)

target_link_libraries(dh_camera_double
        ${DH_LIBS}
        ${OpenCV_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
        )
