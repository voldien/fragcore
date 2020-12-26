#include"audio/openal/internal_object_type.h"
#include"audio/AudioReverb.h"
using namespace fragcore;

intptr_t AudioReverb::getNativePtr(void) const  {
	ALReverb *reverb= (ALReverb*) this->getObject();
	return reverb->reverb;
}

AudioReverb::AudioReverb(void) {

}

AudioReverb::~AudioReverb(void) {

}
