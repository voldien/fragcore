#include "AudioDecoderFactory.h"

#include "VorbisDecoder.h"
#include "WavDecoder.h"
#include <fmt/format.h>
using namespace fragcore;

AudioDecoder *AudioDecoderFactory::createDecoder(AudioDecoderCodec codec, Ref<IO> &io) {
	// TODO determine if needed or if each decoder class handles it.
	if (!io->isReadable())
		throw InvalidArgumentException("IO Object: '{}' is not readable.", io->getName().c_str());

	switch (codec) {
	case AudioDecoderCodec::Ogg:
		return new VorbisAudioDecoder(io);
	case AudioDecoderCodec::Wav:
		return new WavAudioDecoder(io);
	case AudioDecoderCodec::Mp3:
		throw NotSupportedException();
	default:
		throw InvalidArgumentException("");
	}
}