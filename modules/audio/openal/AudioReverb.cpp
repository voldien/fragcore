#include"internal_object_type.h"
#include"../AudioReverb.h"
using namespace fragcore;

intptr_t AudioReverb::getNativePtr() const  {
	ALReverb *reverb= (ALReverb*) this->getObject();
	return reverb->reverb;
}

AudioReverb::AudioReverb() {

}

AudioReverb::~AudioReverb() {

}
