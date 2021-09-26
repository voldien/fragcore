#include <ogg/ogg.h>
#include <opus/opus.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "Core/IO/IO.h"
////#include "Exception/RuntimeException.h"
#include "Audio/decoder/WavDecoder.h"
#include "Audio/decoder/VorbisDecoder.h"
using namespace fragcore;


WavAudioDecoder::WavAudioDecoder(Ref<IO> &io) : AudioDecoder(io) {
 
}

WavAudioDecoder::~WavAudioDecoder(void) { }

void WavAudioDecoder::seek(long int microseconds) {

}

void *WavAudioDecoder::getData(long int *size) {}

void *WavAudioDecoder::getPage(int i) {}

AudioFormat WavAudioDecoder::getFormat(void) const {
	switch (this->channels) {
		case 1:
			return AudioFormat::eMono;
		default:
			break;
	}
	return AudioFormat::eMono;
}
unsigned int WavAudioDecoder::getSampleRate(void) const { return 0; }

unsigned int WavAudioDecoder::getNrChannels(void) const{
	
}
unsigned int WavAudioDecoder::getSampleBitResolution(void) const{

}

double WavAudioDecoder::getTotalTime(void) const { return this->length; }