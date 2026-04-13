cmake_minimum_required(VERSION 3.10)
project(VitalVitals)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        App.cpp
        MainFrame.cpp
)
# This finds the wxWidgets library you just installed
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(VitalVitals ${SOURCES})


find_path(CRYPTOPP_INCLUDE_DIR cryptlib.h PATH_SUFFIXES cryptopp)
find_library(CRYPTOPP_LIBRARY NAMES cryptopp)
target_include_directories(VitalVitals PRIVATE ${CRYPTOPP_INCLUDE_DIR})

target_link_libraries(VitalVitals
        ${wxWidgets_LIBRARIES}
        ${CRYPTOPP_LIBRARY})
