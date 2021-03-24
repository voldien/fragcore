#include <Core/IO/GZFileIO.h>
#include "AudioInterfaceTest.h"
#include "Audio/decoder/AudioDecoderFactory.h"
#include <Audio/AudioInterface.h>
#include <gtest/gtest.h>
#include <FragCore.h>
using namespace fragcore;

TEST(AudioDecoder, AudioDecoderFactory_Invalid_Enum_Throw_Exception){
	Ref<IO> ref = Ref<IO>(SystemInfo::getStdOut());
	ASSERT_THROW(AudioDecoderFactory::createDecoder((AudioDecoderCodec)-1, ref), InvalidArgumentException);
}

TEST(AudioDecoder, AudioDecoderFactory_Invalid_IO_Throw_Exception) {
	Ref<IO> ref = SystemInfo::getStdIn();
	ASSERT_THROW(AudioDecoderFactory::createDecoder(AudioDecoderCodec::Wav, ref), InvalidArgumentException);
}

TEST(AudioDecoder, AudioDecoder_Create_Ogg_No_Throw) {
	Ref<IO> ref = SystemInfo::getStdOut();
	ASSERT_THROW(AudioDecoderFactory::createDecoder(AudioDecoderCodec::Ogg, ref), RuntimeException);
}

TEST(AudioDecoder, AudioDecoder_Create_Wav_No_Throw) {
	Ref<IO> ref = SystemInfo::getStdOut();
	ASSERT_THROW(AudioDecoderFactory::createDecoder(AudioDecoderCodec::Wav, ref), RuntimeException);
}

TEST(AudioDecoder, AudioDecoder_Create_MP3_No_Throw){
	Ref<IO> ref = SystemInfo::getStdOut();
	ASSERT_THROW(AudioDecoderFactory::createDecoder(AudioDecoderCodec::Mp3, ref), RuntimeException);
}

TEST(AudioDecoder, AudioDecoder)
{
	// std::vector<AudioFactory::AudioAPI>::const_iterator it = this->apis.cbegin();

	// for (; it != this->apis.end(); it++)
	// {
	// 	AudioFactory::AudioAPI api = (*it);

	// 	AudioInterface *audio;

	// 	ASSERT_TRUE(audio = createAudioInterface(api, NULL));
	// 	std::vector<AudioDecoderCodec> codes = {
	// 		AudioDecoderCodec::Ogg,
	// 		AudioDecoderCodec::Wav,
	// 		AudioDecoderCodec::Mp3};
	// 	std::vector<AudioDecoderCodec>::iterator it;
	// 	for (; it != codes.end(); it++)
	// 	{
	// 		AudioDecoderFactory::createDecoder(AudioDecoderCodec::Ogg);
	// 	}
	// }
}