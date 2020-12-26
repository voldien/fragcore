#include "audio/decoder/AudioDecoderFactory.h"

#include "Exception/InvalidArgumentException.h"
#include "Exception/NotSupportedException.h"
#include "Utils/StringUtil.h"
#include "audio/decoder/VorbisDecoder.h"
#include "audio/decoder/WavDecoder.h"
using namespace fragcore;

AudioDecoder *AudioDecoderFactory::createDecoder(AudioDecoderCodec codec,
												 Ref<IO> &io) {
	// TODO determine if needed or if each decoder class handles it.
	if (!io->isReadable())
		throw InvalidArgumentException(
			fvformatf("IO Object: '%s' is not readable.", io->getName().c_str()));

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