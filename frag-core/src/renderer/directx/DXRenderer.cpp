#include"Renderer/IRenderer.h"


IRenderer* createInternalRenderer(void){
    return new IRenderer();
}