#ifndef _TEST_RENDERINGINTERFACETEST_H_
#define _TEST_RENDERINGINTERFACETEST_H_ 1
#include "common.h"
using namespace fragcore;


//TODO 
#define SETRENDERAPISCOPE(obj, renderingApi)   \
    {                                               \
    switch (renderingApi) {                        \
        case RenderingFactory::OpenGL:{            \
            SCOPED_TRACE(obj->apiNames[0]);         \
        }break;                                     \
        case RenderingFactory::Vulkan:{\
            SCOPED_TRACE(obj->apiNames[1]);\
        }break;\
        case RenderingFactory::eOpenCL:{\
            SCOPED_TRACE(obj->apiNames[2]);\
        }break;\
        case RenderingFactory::DirectX:{\
            SCOPED_TRACE(obj->apiNames[3]);\
        }break;\
        default:\
            GTEST_FAIL();\
    }\
}\
\

#endif
