#include "../AudioDesc.h"
#include "internal_object_type.h"

using namespace fragcore;

ALenum translate2ALFormat(AudioFormat format) {
	switch (format) {
	case eStero:
		return 0;
	default:
		return 0;
	}
}
