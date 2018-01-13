/**
Copyright (c) 2016 Ryan Porter - arrayNodes
You may use, distribute, or modify this code under the terms of the MIT license.
*/

//-----------------------------------------------------------------------------
// clampAngle node
//
// Restricts the value of an input angle to between a minimum and maximum value.
//-----------------------------------------------------------------------------

#define NOMINMAX
#define _USE_MATH_DEFINES

#include "n_clampAngle.h"
#include "node.h"

#include <math.h>
#include <algorithm>

#include <maya/MAngle.h>
#include <maya/MDataHandle.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPlug.h>
#include <maya/MPxNode.h>

MObject ClampAngleNode::aInput;
MObject ClampAngleNode::aMin;
MObject ClampAngleNode::aMax;
MObject ClampAngleNode::aOutput;

void* ClampAngleNode::creator()
{
    return new ClampAngleNode();
}

MStatus ClampAngleNode::initialize()
{
    MStatus status;
    MFnUnitAttribute u;
    
    aInput = u.create("input", "i", MFnUnitAttribute::kAngle, 0.0, &status); 
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(u);

    aMin = u.create("min", "mn", MFnUnitAttribute::kAngle, 0.0, &status); 
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(u);

    aMax = u.create("max", "mx", MFnUnitAttribute::kAngle, M_PI / 2.0, &status); 
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(u);

    aOutput = u.create("output", "o", MFnUnitAttribute::kAngle, 0.0, &status); 
    __CHECK_STATUS(status);
    MAKE_OUTPUT_ATTR(u);

    addAttribute(aInput);
    addAttribute(aMin);
    addAttribute(aMax);
    addAttribute(aOutput);

    attributeAffects(aInput, aOutput);
    attributeAffects(aMin, aOutput);
    attributeAffects(aMax, aOutput);

    return MS::kSuccess;
}

MStatus ClampAngleNode::compute(const MPlug& plug, MDataBlock& data)
{
    if (plug != aOutput) 
    {
        return MS::kUnknownParameter;
    }

    double input = data.inputValue(aInput).asAngle().asDegrees();
    double mn = data.inputValue(aMin).asAngle().asDegrees();
    double mx = data.inputValue(aMax).asAngle().asDegrees();

    double result = std::max(std::min(input, mx), mn);

    MDataHandle outputHandle = data.outputValue(aOutput);
    outputHandle.setMAngle(MAngle(result, MAngle::kDegrees));
    outputHandle.setClean();

    return MS::kSuccess;
}