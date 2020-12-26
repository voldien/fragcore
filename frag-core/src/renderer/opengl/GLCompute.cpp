#include"Renderer/ICompute.h"

using namespace fragcore;

// TODO relocate to its own file that handles all the compute of Vulkan.
bool ICompute::supportCompute(void) { return true; }
void ICompute::OnInitialization(void) {}
void ICompute::OnDestruction(void) {}

const char *ICompute::getVersion(void) const { return ""; }