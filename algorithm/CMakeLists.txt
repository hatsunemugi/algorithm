cmake_minimum_required(VERSION 3.5)

project(algorithm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS SVG REQUIRED)
set(TS_FILES algorithm_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        lcs.h
        lcs.cpp
        greedy.h
        greedy.cpp
        maxsum.h
        maxsum.cpp
        knapsack.h
        knapsack.cpp
        mycanvas.h
        mycanvas.cpp
        generate.h
        generate.cpp
        slidepage.h
        slidepage.cpp
        analyser.h
        analyser.cpp
        graph_view.h
        graph_view.cpp
        backtracking.h
        backtracking.cpp
        graph_implement.h
        graph_implement.cpp
        matrixchain.h
        matrixchain.cpp
        customWidgets.h
        customWidgets.cpp
        customScrollContainer.h
        customScrollContainer.cpp
        icons.qrc
        subwindow.h
        subwindow.cpp
        subwindow.ui
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(algorithm
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET algorithm APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(algorithm SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(algorithm
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(algorithm PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(algorithm PRIVATE Qt${QT_VERSION_MAJOR}::Svg)
set_target_properties(algorithm PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(algorithm)
endif()
