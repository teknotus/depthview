cmake_minimum_required(VERSION 2.8.11)

project(depthview)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

add_executable(depthview main.cpp cameracontrol.cpp controlswidget.cpp
               mainwindow.cpp cameradatafeed.cpp  videowidget.cpp)
target_link_libraries(depthview Qt5::Widgets)
