cmake_minimum_required(VERSION 3.16)

project(algorithm VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)
file(GLOB_RECURSE SOURCE_FILE "header/*.h" "header/*.hpp" "source/*.cpp")
file(GLOB_RECURSE LIB "lib/fmt/*.h" "lib/fmt/*.lib")
file(GLOB DEBUG_LIB "lib/fmt/*d.lib")
file(GLOB RELEASE_LIB "lib/fmt/*.lib")
file(GLOB IMAGES "image/*/*.*")
file(GLOB VALUES "values/*")
file(GLOB LAYOUTS "layout/*.qml")

include_directories("header")
include_directories("lib/fmt")
qt_standard_project_setup()

qt_add_executable(appalgorithm
    ${SOURCE_FILE}
    ${LIB}
)


foreach(filepath ${LAYOUTS})
    message(${filepath} "\t" ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath}) #.+/(.+)\\\\..*" "\\\\1
    list(APPEND QML_LAYOUTS ${filename})
endforeach(filepath)
list(REMOVE_DUPLICATES QML_LAYOUTS) #表示删除非第一个重名值

foreach(filepath ${IMAGES})
    message(${filepath} "\t" ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath}) #.+/(.+)\\\\..*" "\\\\1
    list(APPEND QML_IMAGES ${filename})
endforeach(filepath)
list(REMOVE_DUPLICATES QML_IMAGES)

foreach(filepath ${VALUES})
    message(${filepath} "\t" ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath}) #.+/(.+)\\\\..*" "\\\\1
    list(APPEND QML_VALUES ${filename})
endforeach(filepath)
list(REMOVE_DUPLICATES QML_VALUES)

list(APPEND QML_HEADERS
    "header/graph.h"
    "header/painteditem.hpp"
)

list(APPEND QML_SOURCES
    "source/graph.cpp"
)

qt_add_qml_module(appalgorithm
    URI algorithm
    VERSION 1.0
    QML_FILES ${QML_LAYOUTS}
    SOURCES ${QML_HEADERS} ${QML_SOURCES}
    RESOURCES ${QML_IMAGES} ${QML_VALUES}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appalgorithm PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appalgorithm
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appalgorithm
    PRIVATE Qt6::Quick
    debug ${DEBUG_LIB}
    optimized ${RELEASE_LIB}
)

include(GNUInstallDirs)
install(TARGETS appalgorithm
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
