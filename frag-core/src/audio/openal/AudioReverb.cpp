#include"Audio/openal/internal_object_type.h"
#include"Audio/AudioReverb.h"
using namespace fragcore;

intptr_t AudioReverb::getNativePtr(void) const  {
	ALReverb *reverb= (ALReverb*) this->getObject();
	return reverb->reverb;
}

AudioReverb::AudioReverb(void) {

}

AudioReverb::~AudioReverb(void) {

}
