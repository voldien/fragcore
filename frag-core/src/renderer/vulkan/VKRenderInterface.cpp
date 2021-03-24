#include"Renderer/vulkan/VKRenderInterface.h"

using namespace fragcore;

extern "C" IRenderer *createInternalRenderer(IConfig *config) { return new VKRenderInterface(config); }