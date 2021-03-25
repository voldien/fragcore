#include<Core/IO/GZFileIO.h>
#include <Exception/InvalidPointerException.h>
#include"AudioInterfaceTest.h"
#include"Audio/decoder/AudioDecoderFactory.h"
#include<Audio/AudioInterface.h>
#include<Audio/AudioListener.h>
#include<Audio/decoder/AudioDecoder.h>
#include<Audio/AudioSource.h>
#include<gtest/gtest.h>
#include <SDL2/SDL.h>
#include<FragCore.h>
using namespace fragcore;

void AudioInterfaceTest::TearDown()
{
	Test::TearDown();
	//delete this->config;
}

void AudioInterfaceTest::SetUp()
{
	Test::SetUp();
	this->apis = std::vector<AudioFactory::AudioAPI>({AudioFactory::OpenAL, AudioFactory::FMOD});
}

//TODO relocate to its own file for all about openal.
TEST_F(AudioInterfaceTest, AudioInterface_Create_AudioInterface_Enum_OpenAL_No_Throw){
	ASSERT_NO_THROW(AudioFactory::createAudioInterface(AudioFactory::OpenAL, NULL));
}

//TODO relocate to its own file for all about openal.
TEST_F(AudioInterfaceTest, AudioInterface_Delete_AudioInterface_Enum_OpenAL_No_Throw) {
	AudioInterface *interface = AudioFactory::createAudioInterface(AudioFactory::OpenAL, NULL);
	ASSERT_NO_THROW(delete interface);
}

TEST_F(AudioInterfaceTest, AudioInterface_Create_AudioInterface_Enum_OpenAL_Config_No_Throw){
	AudioInterface *interface = NULL;
	IConfig config;
	ASSERT_NO_THROW(interface = AudioFactory::createAudioInterface(AudioFactory::OpenAL, &config));
	delete interface;
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_Get_Devices_No_Throw)
{
	AudioInterface *interface = AudioFactory::createAudioInterface(AudioFactory::OpenAL, NULL); //TODO make part of the suite so it only implemented once.
	ASSERT_NO_THROW(interface->getAudioDevice());
	delete interface;
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_Get_Version_None_Null){
	AudioInterface *interface = AudioFactory::createAudioInterface(AudioFactory::OpenAL, NULL);
	//ASSERT_NE(interface->getVersion(), NULL);
	delete interface;
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_Get_Settings_No_Throw) {}


/*
 *	Audio Clips.
 */
TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_AudioClip_Null_Desc_Throw_Exception) {
	AudioInterface *interface = AudioFactory::createAudioInterface( AudioFactory::AudioAPI::OpenAL, NULL);
	ASSERT_THROW(interface->createAudioClip(NULL), InvalidPointerException);
	delete interface;
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_AudioClip_Uninitialized_Desc_Throw_Exception) {
	AudioInterface *interface = AudioFactory::createAudioInterface(AudioFactory::AudioAPI::OpenAL, NULL);
	AudioClipDesc desc = {0};
	ASSERT_THROW(interface->createAudioClip(&desc), InvalidArgumentException);
	delete interface;
}
TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_AudioCli_Invalid_Desc_Throw_Exception){
	AudioInterface *interface = AudioFactory::createAudioInterface(AudioFactory::AudioAPI::OpenAL, NULL);
	AudioClipDesc desc = {0};
	ASSERT_THROW(interface->createAudioClip(&desc), InvalidArgumentException);
	delete interface;
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_AudioClip_Create_No_Exception) {
	Ref<IO> ref = SystemInfo::getStdOut();
	AudioDecoder *decoder = AudioDecoderFactory::createDecoder(AudioDecoderCodec::Ogg, ref);
	AudioInterface *interface = AudioFactory::createAudioInterface(AudioFactory::AudioAPI::OpenAL, NULL);

	AudioClipDesc desc;
	desc.decoder = Ref<AudioDecoder>(decoder);
	desc.streaming = false;

	ASSERT_NO_THROW(interface->createAudioClip(&desc));
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_AudioClip_Stream_Create_No_Throw) {
}


/**
 *	Audio sources.
 */
TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_Create_AudioSource_NoThrow) {
	AudioInterface *audio;
	AudioFactory::AudioAPI api = AudioFactory::OpenAL;
	ASSERT_TRUE(audio = AudioFactory::createAudioInterface(api, NULL));

	AudioSourceDesc desc = {};
	desc.position = Vector3::zero();
	AudioSource* source = NULL;
	ASSERT_NO_THROW(source = audio->createAudioSource(&desc));

	audio->deleteAudioSource(source);
	delete audio;
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_Create_AudioSource_InitState) {
	AudioInterface *audio;
	AudioFactory::AudioAPI api = AudioFactory::OpenAL;
	ASSERT_TRUE(audio = AudioFactory::createAudioInterface(api, NULL));

	AudioSourceDesc desc = {};
	desc.position = Vector3::zero();
	AudioSource* source = NULL;
	source = audio->createAudioSource(&desc);

	ASSERT_FALSE(source->isPlaying());
	ASSERT_FALSE(source->isLooping());
	ASSERT_FLOAT_EQ(source->getVolume(), 1.0f);
	ASSERT_FLOAT_EQ(source->getDistance(), 1.0f);//TODO fix the correct value.
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_Audio_Source_Clip_No_Throw) {}

/**
 * Audio Listener.
 */
TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_Create_Audio_Listener_No_Throw) {
	AudioInterface *interface = AudioFactory::createAudioInterface(AudioFactory::OpenAL, NULL);

	AudioListenerDesc desc;
	AudioListener *list;
	
	ASSERT_NO_THROW(list = interface->createAudioListener(&desc));
	//ASSERT_NE(list, NULL);
	interface->deleteAudioListener(list);
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_Create_Audio_Listener_Correctly) {
	AudioInterface *interface = AudioFactory::createAudioInterface(AudioFactory::OpenAL, NULL);

	AudioListenerDesc desc;
	AudioListener *list;
	desc.position = Vector3::forward();
	desc.rotation = Quaternion::identity();
	list = interface->createAudioListener(&desc);
	ASSERT_EQ(list->getPosition(), desc.position);
	ASSERT_EQ(list->getOrientation(), desc.rotation);

	interface->deleteAudioListener(list);
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_AudioListener_Set_Volume_Correct) {
	AudioInterface* interface = AudioFactory::createAudioInterface(AudioFactory::OpenAL, NULL);

	AudioListenerDesc desc;
	AudioListener *list = interface->createAudioListener(&desc);
	list->setVolume(0.5f);
	ASSERT_FLOAT_EQ(list->getVolume(), 0.5f);

	/*	*/
	interface->deleteAudioListener(list);
	delete interface;
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_AudioListener_Set_) {
	AudioInterface* interface = AudioFactory::createAudioInterface(AudioFactory::OpenAL, NULL);

	AudioListenerDesc desc;
	AudioListener *list = interface->createAudioListener(&desc);
	list->setVolume(0.5f);
	ASSERT_FLOAT_EQ(list->getVolume(), 0.5f);

	interface->deleteAudioListener(list);
	delete interface;
}

TEST_F(AudioInterfaceTest, AudioInterface_OpenAL_AudioListener_InitVeclocity_Zero) {
	AudioInterface* interface = AudioFactory::createAudioInterface(AudioFactory::OpenAL, NULL);

	AudioListenerDesc desc;
	AudioListener *list = interface->createAudioListener(&desc);
	ASSERT_EQ(list->getVelocity(), Vector3::zero());
	interface->deleteAudioListener(list);
	delete interface;
}

