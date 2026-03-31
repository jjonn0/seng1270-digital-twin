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

# Links the library to your program
target_link_libraries(VitalVitals ${wxWidgets_LIBRARIES})