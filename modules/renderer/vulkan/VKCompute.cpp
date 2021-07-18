#include "Exception/InvalidArgumentException.h"
#include "Exception/RuntimeException.h"

#include "VKCommandList.h"
#include "internal_object_type.h"

using namespace fragcore;

// TODO relocate to its own file that handles all the compute of Vulkan.
bool ICompute::supportCompute(void) noexcept { return true; }
void ICompute::OnInitialization(void) {}
void ICompute::OnDestruction(void) {}

const char *ICompute::getVersion(void) const { return ""; }

CommandList *ICompute::createCommandBuffer(void) {}
void ICompute::submittCommand(Ref<CommandList> &list) {}
void ICompute::execute(CommandList *list) {}