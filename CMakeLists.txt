cmake_minimum_required(VERSION 2.8)

project (FaceFilters)

set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt/5.9.1/lib/cmake/Qt5")

find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_CXX_CFLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")

include_directories (${OpenCV_INCLUDE_DIRS})

set(SRC_FILES 
src/FaceFilterApp.cpp
src/CameraPreview.cpp)

set(HDR_FILES
src/CameraPreview.h)

qt5_wrap_ui(UI_FILES src/CameraPreview.ui)

add_executable (FaceFilterApp 
${SRC_FILES}
${HDR_FILES}
${UI_FILES})

target_link_libraries(FaceFilterApp ${OpenCV_LIBS} Qt5::Core Qt5::Gui Qt5::Widgets)


