/**
Copyright (c) 2016 Ryan Porter - arrayNodes
You may use, distribute, or modify this code under the terms of the MIT license.
*/

//-----------------------------------------------------------------------------
//  angleScalarOp node
//
//  Performs a binary operation with an input value and a scalar.
//      No Operation    - Returns the input value.
//      Add             - Returns the sum of the input values.
//      Subtract        - Returns the difference between the input values. 
//      Multiply        - Returns the product of the input values.
//      Divide          - Returns the quotient of the input values.
//      Power           - Returns the exponent of the input values.
//-----------------------------------------------------------------------------

#include "n_angleScalarOp.h"
#include "node.h"

#include <math.h>

#include <maya/MAngle.h>
#include <maya/MDataHandle.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPlug.h>
#include <maya/MPxNode.h>

MObject AngleScalarOpNode::aInput;
MObject AngleScalarOpNode::aScalar;
MObject AngleScalarOpNode::aOperation;
MObject AngleScalarOpNode::aOutput;

const short NO_OP =     0;
const short ADD =       1;
const short SUBTRACT =  2;
const short MULTIPLY =  3;
const short DIVIDE =    4;
const short POWER =     5;

void* AngleScalarOpNode::creator()
{
    return new AngleScalarOpNode();
}

MStatus AngleScalarOpNode::initialize()
{
    MStatus status;

    MFnUnitAttribute u;
    MFnEnumAttribute e;

    aInput = u.create("input", "i", MFnUnitAttribute::kAngle, 0.0, &status);
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(u);

    aScalar = u.create("scalar", "sc", MFnUnitAttribute::kAngle, 0.0, &status);
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(u);

    aOperation = e.create("operation", "op", ADD, &status);
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(e);

    e.addField("No Operation", NO_OP);
    e.addField("Add", ADD);
    e.addField("Subtract", SUBTRACT);
    e.addField("Multiply", MULTIPLY);
    e.addField("Divide", DIVIDE);
    e.addField("Power", POWER);

    aOutput = u.create("output", "o", MFnUnitAttribute::kAngle, 0.0, &status);
    __CHECK_STATUS(status);
    MAKE_OUTPUT_ATTR(u)

    addAttribute(aInput);
    addAttribute(aScalar);
    addAttribute(aOperation);
    addAttribute(aOutput);

    attributeAffects(aInput, aOutput);
    attributeAffects(aScalar, aOutput);
    attributeAffects(aOperation, aOutput);

    return MS::kSuccess;
}

MStatus AngleScalarOpNode::compute(const MPlug& plug, MDataBlock& data)
{
    if (plug != aOutput)
    {
        return MS::kUnknownParameter;
    }

    double input1 = data.inputValue(aInput).asAngle().asDegrees();
    double input2 = data.inputValue(aScalar).asDouble();
    short operation = data.inputValue(aOperation).asShort();

    double result = input1;

    switch (operation)
    {
        case ADD:
            result = input1 + input2;
            break;

        case SUBTRACT:
            result = input1 - input2;
            break;

        case MULTIPLY:
            result = input1 * input2;
            break;

        case DIVIDE:
            result = input2 == 0.0 ? 100000.0 : input1 / input2;
            break;

        case POWER:
            result = std::pow(input1, input2);
            break;
    }

    MDataHandle output = data.outputValue(aOutput);
    output.setMAngle(MAngle(result, MAngle::kDegrees));
    output.setClean();

    return MS::kSuccess;
}
