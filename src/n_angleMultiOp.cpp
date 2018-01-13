/**
Copyright (c) 2016 Ryan Porter - arrayNodes
You may use, distribute, or modify this code under the terms of the MIT license.
*/

//-----------------------------------------------------------------------------
//  angleUnaryOp node
//
//  Performs a reducing operation with the input values
//      No Operation    - Returns zero.
//      Sum             - Returns the sum of the input values.
//      Difference      - Returns the difference of the input values. 
//      Product         - Returns the product of the input values.
//      Minimum         - Returns the smallest input value.
//      Maximum         - Returns the largest input value.
//-----------------------------------------------------------------------------

#include "n_angleMultiOp.h"
#include "node.h"

#include <vector>
#include <algorithm>
#include <numeric>

#include <maya/MAngle.h>
#include <maya/MDataHandle.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPlug.h>
#include <maya/MPxNode.h>

MObject AngleMultiOpNode::aInput;
MObject AngleMultiOpNode::aOperation;
MObject AngleMultiOpNode::aOutput;

const short NO_OP = 0;
const short SUM = 1;
const short DIFF = 2;
const short PRODUCT = 3;
const short MIN_ = 4;
const short MAX_ = 5;

void* AngleMultiOpNode::creator()
{
    return new AngleMultiOpNode();
}

MStatus AngleMultiOpNode::initialize()
{
    MStatus status;

    MFnUnitAttribute u;
    MFnEnumAttribute e;

    aInput = u.create("input", "i", MFnUnitAttribute::kAngle, 0.0, &status);
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(u);
    u.setArray(true);

    aOperation = e.create("operation", "op", SUM, &status);
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(e);

    e.addField("No Operation", NO_OP);
    e.addField("Sum", SUM);
    e.addField("Difference", DIFF);
    e.addField("Product", PRODUCT);
    e.addField("Minimum", MIN_);
    e.addField("Maximum", MAX_);

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

MStatus AngleMultiOpNode::compute(const MPlug& plug, MDataBlock& data)
{
    if (plug != aOutput) 
    {
        return MS::kUnknownParameter;
    }

    MArrayDataHandle inputArrayHandle = data.inputValue(aInput);
    
    unsigned numInputs = inputArrayHandle.elementCount();
    std::vector<double> inputs(numInputs);

    for (unsigned i = 0; i < numInputs; i++)
    {
        inputs[i] = inputArrayHandle.inputValue().asAngle().asDegrees();
        if (!inputArrayHandle.next()) continue;
    }

    short operation = data.inputValue(aOperation).asShort();

    double result = 0;

    switch (operation)
    {
        case SUM:
            result = std::accumulate(inputs.begin(), inputs.end(), 0.0, std::plus<double>());
            break;

        case DIFF:
            result = std::accumulate(inputs.begin(), inputs.end(), 0.0, std::minus<double>());
            break;

        case PRODUCT:
            result = std::accumulate(inputs.begin(), inputs.end(), 1.0, std::multiplies<double>());
            break;

        case MIN_:
            result = *std::min_element(inputs.begin(), inputs.end());
            break;

        case MAX_:
            result = *std::max_element(inputs.begin(), inputs.end());
            break;
    }

    MDataHandle output = data.outputValue(aOutput);
    output.setMAngle(MAngle(result, MAngle::kDegrees));
    output.setClean();

    return MS::kSuccess;
}