# This CMake is designed for WINDOWS

cmake_minimum_required(VERSION 2.6)
project(angularNodes)

set(MAYA_LIBRARIES OpenMaya OpenMayaAnim OpenMayaFX OpenMayaRender OpenMayaUI Foundation)

INCLUDE_DIRECTORIES($ENV{MAYA_INCLUDE_PATH})
LINK_DIRECTORIES($ENV{MAYA_LIBRARY_PATH})

set(SOURCE_FILES
    "src/n_angleBinaryOp.h"
    "src/n_angleMultiOp.h"
    "src/n_angleScalarOp.h"
    "src/n_angleUnaryOp.h"
    "src/n_clampAngle.h"
    "src/node.h"
    "src/n_angleBinaryOp.cpp"
    "src/n_angleMultiOp.cpp"
    "src/n_angleScalarOp.cpp"
    "src/n_angleUnaryOp.cpp"
    "src/n_clampAngle.cpp"
    "src/pluginMain.cpp"
)

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${MAYA_LIBRARIES})

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES 
    COMPILE_DEFINITIONS "REQUIRE_IOSTREAM;_BOOL;NT_PLUGIN"
    LINK_FLAGS "/export:initializePlugin /export:uninitializePlugin"
    PREFIX ""
    SUFFIX ".mll"
)
