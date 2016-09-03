#ifndef N_CLAMP_ANGLES_OP_H
#define N_CLAMP_ANGLES_OP_H

#include <maya/MDataBlock.h>
#include <maya/MPlug.h>
#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>

class ClampAngleNode : public MPxNode
{
public:
    virtual MStatus         compute(const MPlug& plug, MDataBlock& data);
    static  void*           creator();
    static  MStatus         initialize();

public:
    static MTypeId          kNODE_ID;
    static MString          kNODE_NAME;

    static MObject          aInput;
    static MObject          aMin;
    static MObject          aMax;
    static MObject          aOutput;
};

#endif