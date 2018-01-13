/**
    Copyright (c) 2016 Ryan Porter
*/

/*
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

/**
angularNodes

Maya math nodes operate on double values. This means that Maya must create a 
unit conversion node when connecting to or from a doubleAngle value, such as 
the rotate attributes of a transform. This quickly pollutes a scene with these
unitConversion nodes, which have a negative measurable effect on rig performance.

These nodes operation on doubleAngle values as inputs and outputs, eliminating
the need for a unit conversion node in most cases.
*/

#include "n_angleBinaryOp.h"
#include "n_angleMultiOp.h"
#include "n_angleScalarOp.h"
#include "n_angleUnaryOp.h"
#include "n_clampAngle.h"

#include <maya/MFnPlugin.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

const char* kAUTHOR = "Ryan Porter";
const char* kVERSION = "1.0.1";
const char* kREQUIRED_API_VERSION = "Any";

MString ClampAngleNode::kNODE_NAME =        "clampAngle";
MString AngleBinaryOpNode::kNODE_NAME =     "angleBinaryOp";
MString AngleMultiOpNode::kNODE_NAME =      "angleMultiOp";
MString AngleScalarOpNode::kNODE_NAME =     "angleScalarOp";
MString AngleUnaryOpNode::kNODE_NAME =      "angleUnaryOp";

MTypeId AngleBinaryOpNode::kNODE_ID =       0x00126b12;
MTypeId AngleMultiOpNode::kNODE_ID =        0x00126b13;
MTypeId AngleScalarOpNode::kNODE_ID =       0x00126b14;
MTypeId AngleUnaryOpNode::kNODE_ID =        0x00126b15;
MTypeId ClampAngleNode::kNODE_ID =          0x00126b16;

#define REGISTER_NODE(NODE)                    \
    status = fnPlugin.registerNode(            \
        NODE::kNODE_NAME,                      \
        NODE::kNODE_ID,                        \
        NODE::creator,                         \
        NODE::initialize                       \
    );                                         \
    CHECK_MSTATUS_AND_RETURN_IT(status);       \

#define DEREGISTER_NODE(NODE)                  \
    status = fnPlugin.deregisterNode(          \
        NODE::kNODE_ID                         \
    );                                         \
    CHECK_MSTATUS_AND_RETURN_IT(status);       \

MStatus initializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin fnPlugin(obj, kAUTHOR, kVERSION, kREQUIRED_API_VERSION);

    REGISTER_NODE(AngleMultiOpNode);
    REGISTER_NODE(AngleBinaryOpNode);
    REGISTER_NODE(AngleScalarOpNode);
    REGISTER_NODE(AngleUnaryOpNode);
    REGISTER_NODE(ClampAngleNode);

    return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin fnPlugin(obj, kAUTHOR, kVERSION, kREQUIRED_API_VERSION);

    DEREGISTER_NODE(AngleMultiOpNode);
    DEREGISTER_NODE(AngleBinaryOpNode);
    DEREGISTER_NODE(AngleScalarOpNode);
    DEREGISTER_NODE(AngleUnaryOpNode);
    DEREGISTER_NODE(ClampAngleNode);

    return MS::kSuccess;
}
