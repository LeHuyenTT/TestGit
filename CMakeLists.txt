cmake_minimum_required(VERSION 3.10)

project(AppTeacher LANGUAGES CXX)

# --- Qt packages ---
find_package(Qt5 COMPONENTS Core Quick Qml Sql Multimedia REQUIRED)

# --- Compiler setup ---
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# --- Paths ---
set(PKG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Pkg)
set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs)
#set(QML_RESOURCES ${PKG_PATH}/MVC/Views/qml.qrc)
set(OpenCV_DIR ${LIB_PATH}/cv2_bin/lib/cmake/opencv4)
set(OpenCV_INCLUDE_DIRS ${LIB_PATH}/cv2_bin/include)
set(ncnn_DIR ${LIB_PATH}/ncnn_bin/install/lib/cmake/ncnn)
set(PATH_LIB_SOCKET ${LIB_PATH}/SocketIO_bin/libsioclient.a)

# --- Dependencies ---
find_package(OpenCV REQUIRED)
set(ncnn_DIR "${CMAKE_SOURCE_DIR}/libs/ncnn_bin/lib/cmake/ncnn")
find_package(ncnn REQUIRED)
find_package(OpenSSL REQUIRED)

# --- Source files ---
file(GLOB CPP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${PKG_PATH}/*/*/src/*.cpp"
    "${PKG_PATH}/*/src/*.cpp"
)

file(GLOB HEADERS 
    "${PKG_PATH}/*/*/inc/*.h"
    "${PKG_PATH}/*/inc/*.h"
)

# --- Include directories ---
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Pkg
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/libs/ncnn_bin/include 
    ${LIB_PATH}/ncnn_bin/install/include/
    ${LIB_PATH}/SocketIO_bin/src
    ${OPENSSL_INCLUDE_DIR}
    ${PKG_PATH}/Protocols
    ${PKG_PATH}/MVC
    ${PKG_PATH}/Auth
    ${PKG_PATH}/ComputingServ
    ${PKG_PATH}/Devices
    ${PKG_PATH}/Middlewares
    ${PKG_PATH}/Utils
    ${PKG_PATH}/Booting
)

# --- Add executable ---
add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${HEADERS} ${QML_RESOURCES})

# --- Link libraries ---
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::Sql
    Qt5::Multimedia
    ncnn
    ${OpenCV_LIBS}
    ${PATH_LIB_SOCKET}
    ${OPENSSL_LIBRARIES}
)

# --- Compile definitions & flags ---
add_definitions(-DQT_NO_KEYWORDS)
target_compile_definitions(${PROJECT_NAME} PRIVATE QT_DEPRECATED_WARNINGS)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wno-deprecated-declarations -pthread -fopenmp)

# --- Resources ---
qt5_add_resources(QML_RESOURCES_ADDED ${QML_RESOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${QML_RESOURCES_ADDED})

# --- Install rule for Debug build ---
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_MODE)
    install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()
