#include "Core/IO/FileAccess.h"
using namespace fragcore;

FileAccess::FileAccess() {}

FileAccess::~FileAccess() {}

bool FileAccess::readable() const { return false; }
bool FileAccess::writeable() const { return false; }
bool FileAccess::executable() const { return false; }
