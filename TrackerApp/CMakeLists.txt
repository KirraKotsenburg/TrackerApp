cmake_minimum_required(VERSION 3.16)

project(TrackerApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the path to your OpenCV build directory
set(OpenCV_DIR "D:/opencv/build")

# Find required Qt packages, including Multimedia
find_package(Qt6 6.7.1 REQUIRED COMPONENTS Core Gui Widgets Multimedia Quick SerialPort)
find_package(OpenCV REQUIRED PATHS ${OpenCV_DIR})

if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    link_directories(${OpenCV_LIB_DIR})
else()
    message(FATAL_ERROR "OpenCV not found")
endif()

# Standard Qt project setup
qt_standard_project_setup()

# Define the executable target
qt_add_executable(appTrackerApp
    main.cpp
)

# Define the QML module
qt_add_qml_module(appTrackerApp
    URI TrackerApp
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES model.h model.cpp
    SOURCES imageprovider.h imageprovider.cpp
    RESOURCES images/captured_frame.png
)

# Set target properties for macOS and Windows
set_target_properties(appTrackerApp PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Link the necessary Qt libraries
target_link_libraries(appTrackerApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Quick
    Qt6::SerialPort
    ${OpenCV_LIBS}
)

# Include directories for Qt
target_include_directories(appTrackerApp PRIVATE ${Qt6Multimedia_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS})

# Installation settings
install(TARGETS appTrackerApp
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Include GNUInstallDirs for standard installation directories
include(GNUInstallDirs)
install(TARGETS appTrackerApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

