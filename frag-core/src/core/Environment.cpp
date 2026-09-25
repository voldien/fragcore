
#include "Core/Environment.h"
#include <libenvpp/env.hpp>

using namespace fragcore;

bool EnvManager::isEnvDefined(const char *variableName) noexcept { return std::getenv(variableName) != nullptr; }
const char *EnvManager::getEnValue(const char *variableName) noexcept { return std::getenv(variableName); }

void EnvManager::setEnvValue(const char *variableName, const char *value) noexcept {}