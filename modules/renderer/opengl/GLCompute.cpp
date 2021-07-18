#include"Renderer/ICompute.h"

using namespace fragcore;

bool ICompute::supportCompute(void) noexcept { return true; }
void ICompute::OnInitialization(void) {}
void ICompute::OnDestruction(void) {}

const char *ICompute::getVersion(void) const { return ""; }

CommandList *ICompute::createCommandBuffer(void)
{

}

void ICompute::submittCommand(Ref<CommandList> &list)
{

}

void ICompute::execute(CommandList *list)
{

}