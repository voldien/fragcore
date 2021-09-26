#include "Audio/AudioClip.h"
#include "Audio/AudioInterface.h"
#include "Audio/AudioListener.h"
#include "Audio/AudioPhysicalDevice.h"
#include "Audio/AudioSource.h"
#include "Audio/openal/internal_object_type.h"
#include "Core/Math.h"
#include "Core/SystemInfo.h"
#include "Core/TaskScheduler/TaskScheduler.h"
#include<fmt/core.h>
using namespace fragcore;
//
// void AudioInterface::initAllocate(InitAllocateTableInfo *table) {
//	table->interfacetype = eAudio;
//
//}

AudioInterface::AudioInterface(IConfig *config) : Module() {
	this->setName("OpenAL");
	// IConfig& configSettings;
	if (config == nullptr) {
		/*	TODO construct default configuration.	*/
	}

	/*	*/
	OpenALInterfaceObject *alDevice = (OpenALInterfaceObject *)malloc(sizeof(OpenALInterfaceObject));
	assert(alDevice);
	this->pdata = alDevice;
	memset(alDevice, 0, sizeof(OpenALInterfaceObject));

	// AL_SOURCE_TYPE
	// TODO add support
	// const char* device = config->get<const char*>("device");

	const ALchar *defaultDevice = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
	AudioPhysicalDevice audioPhysicalDevices;
	audioPhysicalDevices.setName(defaultDevice);
	setAudioDevice(audioPhysicalDevices);

	/*	Internal.	*/
	alDevice->scheduler = Ref<IScheduler>(new TaskScheduler(Math::clamp<int>(SystemInfo::getCPUCoreCount(), 1, 2), 48));
}

AudioInterface::~AudioInterface(void) {
	OpenALInterfaceObject *audio = (OpenALInterfaceObject *)this->pdata;

	/*	Release all the object associated.	*/
	delete *audio->scheduler;

	/*  Unbind and release context. */
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(audio->context);

	/*  Release device. */
	alcCloseDevice(audio->device);
	free(audio);
}

void AudioInterface::OnInitialization(void) {
	OpenALInterfaceObject *audio = (OpenALInterfaceObject *)this->pdata;

	/*	Allocate data.	*/
	audio->source.resize(32);
	audio->clips.resize(32);
	audio->reverbs.resize(32);
}

void AudioInterface::OnDestruction(void) {}

// TODO perhaps move to a helper header file and etc.
#include <AL/alext.h>
static inline ALenum to_al_format(short channels, short samples) {
	bool stereo = (channels > 1);
	//	AL_FORMAT_STER
	switch (samples) {
	case 32:
		if (stereo)
			return AL_FORMAT_STEREO_FLOAT32;
		else
			return AL_FORMAT_MONO_FLOAT32;
	case 16:
		if (stereo)
			return AL_FORMAT_STEREO16;
		else
			return AL_FORMAT_MONO16;
	case 8:
		if (stereo)
			return AL_FORMAT_STEREO8;
		else
			return AL_FORMAT_MONO8;
	default:
		throw RuntimeException("Invalid format. ");
	}
}

static const char *openAlErrorToString(int err) {
	switch (err) {
	case AL_NO_ERROR:
		return "AL_NO_ERROR";
	case AL_INVALID_ENUM:
		return "AL_INVALID_ENUM";
	case AL_INVALID_VALUE:
		return "AL_INVALID_VALUE";
	case AL_INVALID_OPERATION:
		return "AL_INVALID_OPERATION";
	case AL_OUT_OF_MEMORY:
		return "AL_OUT_OF_MEMORY";
	default:
		return "Unknown error code";
	}
}

static void validateClipDesc(AudioClipDesc *desc) {
	if (desc == nullptr)
		throw InvalidPointerException("AudioClipDesc invalid pointer");
	if (desc->decoder.ptr() == nullptr) {
		throw InvalidArgumentException("");
	}

	if (desc->format < eMono || desc->format > eStero)
		throw InvalidArgumentException("");
	if (desc->datamode < AudioDataMode::LoadedInMemory || desc->datamode > AudioDataMode::DecompressOnLoad)
		throw InvalidArgumentException("");
	if (desc->source != nullptr && desc->size <= 0) {
		throw InvalidArgumentException("");
	}
}

AudioClip *AudioInterface::createAudioClip(AudioClipDesc *desc) {
	OpenALInterfaceObject *audio = (OpenALInterfaceObject *)this->pdata;
	validateClipDesc(desc);

	ALuint buffer;

	alGenBuffers((ALuint)1, &buffer);

	/*	TODO based on the loading type.	*/
	if (desc->datamode == AudioDataMode::LoadedInMemory) {
		long int size;
		/*	Load all data from the stream.	*/
		void *data = desc->decoder->getData(&size);
		/*	*/
		ALenum format = to_al_format(desc->format, desc->samples);
		alBufferData(buffer, format, data, size, desc->sampleRate);
		int err = alGetError();
		if (err != ALC_NO_ERROR)
			throw RuntimeException(fmt::format("Failed load memory to buffer {}", openAlErrorToString(err)));

		free(data);
		desc->decoder->deincreemnt();

	} else {
	}

	AudioClip *audioClip = new AudioClip();
	ALClip *source = new ALClip();

	/*	*/
	Ref<AudioDecoder> decoder;
	source->source = buffer;
	source->decoder = desc->decoder;
	source->mode = desc->datamode;
	audioClip->pdata = source;

	return audioClip;
}

void AudioInterface::deleteAudioClip(AudioClip *AudioClip) {
	OpenALInterfaceObject *audio = (OpenALInterfaceObject *)this->pdata;
	// ALClip *clip = (ALClip)AudioClip->getObject();

	alDeleteBuffers(1, nullptr);
	delete audio;
	delete AudioClip;
}

static void validateAudioSourceDesc(AudioSourceDesc *desc) {
	if (desc == nullptr)
		throw InvalidPointerException("AudioSourceDesc invalid pointer");
}

AudioSource *AudioInterface::createAudioSource(AudioSourceDesc *desc) {
	validateAudioSourceDesc(desc);

	ALuint source;
	alGenSources(1, &source);

	alSourcef(source, AL_PITCH, 1);
	// check for errors
	alSourcef(source, AL_GAIN, 1);
	// check for errors
	alSource3f(source, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(source, AL_LOOPING, AL_FALSE);

	AudioSource *audioSource = new AudioSource();
	ALSource *alSource = new ALSource();
	audioSource->pdata = alSource;
	alSource->source = source;

	return audioSource;
}

void AudioInterface::deleteAudioSource(AudioSource *audioSource) { delete audioSource; }

static void validateAudioReverbDesc(AudioReverbDesc *desc) {}

AudioReverb *AudioInterface::createAudioReverb(AudioReverbDesc *desc) {
	OpenALInterfaceObject *audio = (OpenALInterfaceObject *)this->pdata;
	validateAudioReverbDesc(desc);

	/*	Validate the system.	*/
	if (!audio->supportEffects)
		throw RuntimeException("");

	return nullptr;
}

void AudioInterface::deleteAudioReverb(AudioReverb *reverb) { delete reverb; }

AudioListener *AudioInterface::createAudioListener(AudioListenerDesc *desc) {

	OpenALInterfaceObject *audio = (OpenALInterfaceObject *)this->pdata;
	AudioListener *listener = new AudioListener();

	/*	*/
	listener->setPosition(desc->position);
	listener->setVelocity(Vector3::zero());
	listener->setOrientation(desc->rotation);

	return listener;
}

void AudioInterface::deleteAudioListener(AudioListener *listener) { delete listener; }

void AudioInterface::setAudioListener(AudioListener *listener) {
	OpenALInterfaceObject *audio = (OpenALInterfaceObject *)this->pdata;
	// audio->
}

AudioCapture *AudioInterface::createAudioCapture(void) { return nullptr; }
void AudioInterface::deleteAudioCapture(AudioCapture *capture) {}

std::vector<AudioPhysicalDevice> AudioInterface::getDevices(void) const {
	OpenALInterfaceObject *audio = (OpenALInterfaceObject *)this->pdata;
	std::vector<AudioPhysicalDevice> listDevices;

	const ALCchar *devices;
	const ALCchar *mices;
	if (alcIsExtensionPresent(nullptr, "ALC_enumeration_EXT") == AL_TRUE) {
		if (alcIsExtensionPresent(nullptr, "ALC_enumerate_all_EXT") == AL_FALSE)
			devices = (char *)alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
		else
			devices = (char *)alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);

		mices = (char *)alcGetString(nullptr, ALC_CAPTURE_DEVICE_SPECIFIER);
	}

	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	while (device && *device != '\0' && next && *next != '\0') {
		AudioPhysicalDevice audioPhysicalDevices;
		audioPhysicalDevices.setName(device);
		listDevices.push_back(audioPhysicalDevices);

		/*	*/
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}

	return listDevices;
}

void AudioInterface::setAudioDevice(const AudioPhysicalDevice &device) {
	OpenALInterfaceObject *audio = (OpenALInterfaceObject *)this->pdata;
	ALint attribs[4] = {0};

	if (alcGetCurrentContext() != nullptr) {
		ALCcontext *context = alcGetCurrentContext();
		ALCdevice *curDevice = alcGetContextsDevice(context);

		// Same device.
		if (strcmp(alcGetString(curDevice, ALC_DEVICE_SPECIFIER), device.getName().c_str()) == 0)
			return;

		// Not same device. continue with selecting audio device.
	}

	if (alcGetCurrentContext() != nullptr) {
		/*  */
		if (!alcCloseDevice(audio->device))
			throw RuntimeException(fmt::format("Failed to open audio device {}", device.getName().c_str()));
	}

	/*  */
	audio->device = alcOpenDevice(device.getName().c_str());
	if (!audio->device)
		throw RuntimeException(fmt::format("Failed to open audio device {}", device.getName().c_str()));

	if (alcIsExtensionPresent(audio->device, ALC_EXT_EFX_NAME)) {

		audio->alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
		audio->alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
		audio->alIsEffect = (LPALISEFFECT)alGetProcAddress("alIsEffect");

		if (!(audio->alGenEffects && audio->alDeleteEffects && audio->alIsEffect))
			audio->supportEffects = false;

		/* Use Context creation hint to request 4 Auxiliary */ /* Sends per Source */
		attribs[0] = ALC_MAX_AUXILIARY_SENDS;
		attribs[1] = 4;

		audio->supportEffects = true;
	}

	/*  Create context. */
	audio->context = alcCreateContext(audio->device, attribs);
	if (!audio->context)
		throw RuntimeException(fmt::format("Failed to create audio context for device {}", device.getName().c_str()));

	if (!alcMakeContextCurrent(audio->context))
		throw RuntimeException(fmt::format("Failed to make current audio device {}", device.getName().c_str()));

	// alcGetIntegerv(audio->device, ALC_MAX_AUXILIARY_SENDS, 1, &iSends);
}

const char *AudioInterface::getVersion(void) const { return FV_STR_VERSION(1, 0, 0); }

const AudioPhysicalDevice &AudioInterface::getAudioDevice(void) const {
	ALCdevice *dv = alcGetContextsDevice(alcGetCurrentContext());
	AudioPhysicalDevice device = AudioPhysicalDevice();
	device.setName(alcGetString(dv, ALC_DEVICE_SPECIFIER));
	return device;
}

extern "C" AudioInterface *createInternalAudioInterface(IConfig *config) { return new AudioInterface(config); }
