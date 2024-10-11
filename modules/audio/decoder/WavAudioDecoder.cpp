#include <ogg/ogg.h>
#include <opus/opus.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "IO/IO.h"
#include "decoder/VorbisDecoder.h"
#include "decoder/WavDecoder.h"
using namespace fragcore;

WavAudioDecoder::WavAudioDecoder(Ref<IO> &io) : AudioDecoder(io) {}

WavAudioDecoder::~WavAudioDecoder() {}

void WavAudioDecoder::seek(long int microseconds) {}

void *WavAudioDecoder::getData(long int *size) { return nullptr; }

void *WavAudioDecoder::getPage(int i) { return nullptr; }

AudioFormat WavAudioDecoder::getFormat() const {
	switch (this->channels) {
	case 1:
		return AudioFormat::eMono;
	default:
		break;
	}
	return AudioFormat::eMono;
}
unsigned int WavAudioDecoder::getSampleRate() const { return 0; }

unsigned int WavAudioDecoder::getNrChannels() const { return 0; }
unsigned int WavAudioDecoder::getSampleBitResolution() const { return 0; }

double WavAudioDecoder::getTotalTime() const { return this->length; }