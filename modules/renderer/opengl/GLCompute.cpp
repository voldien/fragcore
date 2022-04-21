#include"../ICompute.h"

using namespace fragcore;

bool ICompute::supportCompute() noexcept { return true; }
void ICompute::OnInitialization() {}
void ICompute::OnDestruction() {}

const char *ICompute::getVersion() const { return ""; }

CommandList *ICompute::createCommandBuffer() { return nullptr; }

void ICompute::submittCommand(Ref<CommandList> &list)
{
    // Excecute compute based.
}

void ICompute::execute(CommandList *list)
{
    // Excecute compute based.
}