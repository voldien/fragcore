#include "ALAudioReverb.h"
using namespace fragcore;

intptr_t OpenALAudioReverb::getNativePtr() const {

	return this->reverb;
}

OpenALAudioReverb::OpenALAudioReverb() {}

OpenALAudioReverb::~OpenALAudioReverb() {}
