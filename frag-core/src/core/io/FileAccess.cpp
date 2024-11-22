#include "IO/FileAccess.h"
using namespace fragcore;

FileAccess::FileAccess() = default;

FileAccess::~FileAccess() = default;

bool FileAccess::readable() const { return false; }
bool FileAccess::writeable() const { return false; }
bool FileAccess::executable() const { return false; }
