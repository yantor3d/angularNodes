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

#include "SoftClampAngle.h"
#include "node.h"

#include <math.h>

#include <maya/MAngle.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>

namespace illmill {
    const double s = sin(45 * (M_PI / 180.0));
    const double t = tan(22.5 * (M_PI / 180.0));

    const double softMax(const double &v, const double &mx, const double &sft) {
        const double r = sft / (s * t);
        if (v < (mx - ((s - t) * r)))
            return v;
        if (v > (mx + (r * t)))
            return mx;
        return mx + (r * (cos(asin(((mx - v) / r) + t)))) - r;
    }

    const double softMin(const double &v, const double &mn, const double &sft) {
        const double r = sft / (s * t);
        if (v > (mn + ((s - t) * r)))
            return v;
        if (v < (mn - (r * t)))
            return mn;
        return mn - (r * (cos(asin(((mn - v) / r) - t)))) + r;
    }

    const double softClamp(const double &v, const double &mn, const double &mnS, const double &mx, const double &mxS) {
        return (mx < mn) ? softMax(softMin(v, mx, mxS), mn, mnS)
                         : softMax(softMin(v, mn, mnS), mx, mxS);
    }
}

MObject SoftClampAngleNode::aInput;
MObject SoftClampAngleNode::aMin;
MObject SoftClampAngleNode::aMinSoft;
MObject SoftClampAngleNode::aMax;
MObject SoftClampAngleNode::aMaxSoft;
MObject SoftClampAngleNode::aOutput;

void* SoftClampAngleNode::creator()
{
    return new SoftClampAngleNode();
}

MStatus SoftClampAngleNode::initialize()
{
    MStatus status;
    MFnUnitAttribute u;
    MFnNumericAttribute n;

    aInput = u.create("input", "i", MFnUnitAttribute::kAngle, 0.0, &status); 
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(u);

    aMin = u.create("min", "mn", MFnUnitAttribute::kAngle, 0.0, &status); 
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(u);

    aMinSoft = n.create("minSoft", "mns", MFnNumericData::kDouble, 0.0, &status);
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(n);

    aMax = u.create("max", "mx", MFnUnitAttribute::kAngle, 0.0, &status);
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(u);

    aMaxSoft = n.create("maxSoft", "mxs", MFnNumericData::kDouble, 0.0, &status);
    __CHECK_STATUS(status);
    MAKE_INPUT_ATTR(n);

    aOutput = u.create("output", "o", MFnUnitAttribute::kAngle, 0.0, &status); 
    __CHECK_STATUS(status);
    MAKE_OUTPUT_ATTR(u);

    addAttribute(aInput);
    addAttribute(aMin);
    addAttribute(aMinSoft);
    addAttribute(aMax);
    addAttribute(aMaxSoft);
    addAttribute(aOutput);

    attributeAffects(aInput, aOutput);
    attributeAffects(aMin, aOutput);
    attributeAffects(aMinSoft, aOutput);
    attributeAffects(aMax, aOutput);
    attributeAffects(aMaxSoft, aOutput);

    return MS::kSuccess;
}

MStatus SoftClampAngleNode::compute(const MPlug& plug, MDataBlock& data)
{
    if (plug != aOutput) 
    {
        return MS::kUnknownParameter;
    }

    const double input = data.inputValue(aInput).asAngle().asDegrees();
    const double mn = data.inputValue(aMin).asAngle().asDegrees();
    const double mnS = data.inputValue(aMinSoft).asDouble();
    const double mx = data.inputValue(aMax).asAngle().asDegrees();
    const double mxS = data.inputValue(aMaxSoft).asDouble();

    const double result = illmill::softClamp(input, mn, mnS, mx, mxS);

    MDataHandle outputHandle = data.outputValue(aOutput);
    outputHandle.setMAngle(MAngle(result, MAngle::kDegrees));
    outputHandle.setClean();

    return MS::kSuccess;
}

























