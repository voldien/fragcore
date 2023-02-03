
#include "VKCommandList.h"
#include "internal_object_type.h"

using namespace fragcore;

// TODO relocate to its own file that handles all the compute of Vulkan.
bool ICompute::supportCompute() noexcept { return true; }
void ICompute::OnInitialization() {}
/**/
void ICompute::OnDestruction() {}

const char *ICompute::getVersion() const { return ""; }

CommandList *ICompute::createCommandBuffer() {
    return nullptr;
}
void ICompute::submittCommand(Ref<CommandList> &list) {}
void ICompute::execute(CommandList *list) {}