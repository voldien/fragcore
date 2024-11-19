
#include "VKCompute.h"

using namespace fragcore;

// TODO relocate to its own file that handles all the compute of Vulkan.
bool VKCompute::supportCompute() noexcept { return true; }
void VKCompute::onInitialization() {}
/**/
void VKCompute::onDestruction() {}

const char *VKCompute::getVersion() const { return ""; }

CommandList *VKCompute::createCommandBuffer() { return nullptr; }
void VKCompute::submittCommand(Ref<CommandList> &list) {}
void VKCompute::execute(CommandList *list) {}