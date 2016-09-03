#ifndef N_NODE_H
#define N_NODE_H

#define MAKE_INPUT_ATTR(fnAttr) \
    fnAttr.setKeyable(true); \
    fnAttr.setChannelBox(true); \
    fnAttr.setStorable(true); \
    fnAttr.setWritable(true); \

#define MAKE_OUTPUT_ATTR(fnAttr) \
    fnAttr.setKeyable(false); \
    fnAttr.setChannelBox(false); \
    fnAttr.setStorable(false); \
    fnAttr.setWritable(false);

#ifdef _DEBUG
    #define __CHECK_STATUS(status) CHECK_MSTATUS_AND_RETURN_IT(status);
#else
    #define __CHECK_STATUS(status) ;
#endif

#endif