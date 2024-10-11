
#include "Core/Environment.h"

using namespace fragcore;

bool EnvManager::isEnvDefined(const char *variableName) { return std::getenv(variableName) != nullptr; }
const char *EnvManager::getEnValue(const char *variableName) { return std::getenv(variableName); }
