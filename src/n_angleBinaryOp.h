#ifndef N_ANGLE_BINARY_OP_H
#define N_ANGLE_BINARY_OP_H

#include <maya/MDataBlock.h>
#include <maya/MPlug.h>
#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>

class AngleBinaryOpNode : public MPxNode
{
public:
    virtual MStatus         compute(const MPlug& plug, MDataBlock& data);
    static  void*           creator();
    static  MStatus         initialize();

public:
    static MTypeId          kNODE_ID;
    static MString          kNODE_NAME;

    static MObject          aInput1;
    static MObject          aInput2;
    static MObject          aOperation;
    static MObject          aOutput;
};

#endif