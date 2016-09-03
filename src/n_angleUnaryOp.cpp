/**
Copyright (c) 2016 Ryan Porter - arrayNodes
You may use, distribute, or modify this code under the terms of the MIT license.
*/

//-----------------------------------------------------------------------------
//  angleUnaryOp node
//
//  Performs a unary operation on an input value
//      No Operation    - Returns the input value
//      Negate          - Returns the input value with the sign inverted
//      Sign            - Returns +1.0 if the input value is positive, otherwise -1.0
//      Inverse         - Returns the reciprocal value if the input.
//      Floor           - Returns the value rounded down to the nearest whole number.
//      Ceiling         - Returns the value rounded up to the nearest whole number.
//-----------------------------------------------------------------------------

#include "n_angleUnaryOp.h"
#include "node.h"

#include <algorithm>

#include <maya/MAngle.h>
#include <maya/MDataHandle.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPlug.h>
#include <maya/MPxNode.h>

const short NO_OP =             0;
const short SIGN =              1;
const short NEGATE =            2;
const short RECIPROCAL =        3;
const short FLOOR =             4;
const short CEILING =           5;
const short ABSOLUTE_VALUE =    6;

MObject AngleUnaryOpNode::aInput;
MObject AngleUnaryOpNode::aOperation;
MObject AngleUnaryOpNode::aOutput;

void* AngleUnaryOpNode::creator()
{
    return new AngleUnaryOpNode();
}

MStatus AngleUnaryOpNode::initialize()
{
    MStatus status;

    MFnUnitAttribute u;
    MFnEnumAttribute e;

    aInput = u.create("input", "i", MFnUnitAttribute::kAngle, 0.0, &status);
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(u);

    aOperation = e.create("operation", "op", SIGN, &status);
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(e);

    e.addField("No Operation", NO_OP);
    e.addField("Sign", SIGN);
    e.addField("Negate", NEGATE);
    e.addField("Reciprocal", RECIPROCAL);
    e.addField("Floor", FLOOR);
    e.addField("Ceiling", CEILING);
    e.addField("Absolute Value", ABSOLUTE_VALUE);

    aOutput = u.create("output", "o", MFnUnitAttribute::kAngle, 0.0, &status);
    __CHECK_STATUS(status);
    MAKE_OUTPUT_ATTR(u)

    addAttribute(aInput);
    addAttribute(aOperation);
    addAttribute(aOutput);

    attributeAffects(aInput, aOutput);
    attributeAffects(aOperation, aOutput);

    return MS::kSuccess;
}

MStatus AngleUnaryOpNode::compute(const MPlug& plug, MDataBlock& data)
{
    if (plug != aOutput) 
    {
        return MS::kUnknownParameter;
    }

    double input = data.inputValue(aInput).asAngle().asDegrees();
    short operation = data.inputValue(aOperation).asShort();

    double result = input;

    switch (operation)
    {
        case SIGN: 
            result = input >= 0.0 ? 1.0 : -1.0;
            break;

        case ABSOLUTE_VALUE: 
            result = abs(input);
            break;

        case NEGATE: 
            result = -input; 
            break;

        case RECIPROCAL: 
            result = input == 0.0 ? FLT_MAX : 1.0 / input; 
            break;

        case FLOOR: 
            result = floor(input); 
            break;

        case CEILING: 
            result = ceil(input); 
            break;
    }

    MDataHandle output = data.outputValue(aOutput);
    output.setMAngle(MAngle(result, MAngle::kDegrees));
    output.setClean();

    return MS::kSuccess;
}