cmake_minimum_required(VERSION 2.6)
project(angularNodes)

set(MAYA_LIBRARIES OpenMaya OpenMayaAnim OpenMayaFX OpenMayaRender OpenMayaUI Foundation)

INCLUDE_DIRECTORIES($ENV{MAYA_INCLUDE_PATH})
LINK_DIRECTORIES($ENV{MAYA_LIBRARY_PATH})

if (WIN32)
    set(MAYA_COMPILE_DEFINITIONS REQUIRE_IOSTREAM _BOOL NT_PLUGIN)
    set(MAYA_LINK_FLAGS "/export:initializePlugin /export:uninitializePlugin")
    set(MAYA_COMPILE_FLAGS "/MD")
    set(MAYA_EXTENSION ".mll")
elseif(UNIX AND NOT APPLE)
    set(MAYA_COMPILE_DEFINITIONS REQUIRE_IOSTREAM _BOOL LINUX _LINUX LINUX_64)
    set(MAYA_LINK_FLAGS "-shared -m64 -g -pthread -pipe -fPIC -Wno-deprecated -fno-gnu-keywords -Wl,-Bsymbolic")
    set(MAYA_COMPILE_FLAGS "-m64 -g -pthread -pipe -fPIC -Wno-deprecated -fno-gnu-keywords -Wl,-Bsymbolic")
    set(MAYA_EXTENSION ".so")
else()
    message(FATAL_ERROR, "This CMake file is configured for Windows or Linux")
endif()

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
    COMPILE_DEFINITIONS "${MAYA_COMPILE_DEFINITIONS}"
    COMPILE_FLAGS "${MAYA_COMPILE_FLAGS}"
    LINK_FLAGS "${MAYA_LINK_FLAGS}"
    PREFIX ""
    SUFFIX "${MAYA_EXTENSION}"
)
