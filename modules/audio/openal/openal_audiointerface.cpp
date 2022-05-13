#include "../AudioPhysicalDevice.h"
#include "ALAudioClip.h"
#include "ALAudioListener.h"
#include "ALAudioSource.h"
#include "Core/Math.h"
#include "Core/SystemInfo.h"
#include "Core/TaskScheduler/TaskScheduler.h"
#include "OpenALAudioInterface.h"
#include "internal_object_type.h"
using namespace fragcore;

static void validateAudioSourceDesc(AudioSourceDesc *desc) {
	if (desc == nullptr)
		throw InvalidPointerException("AudioSourceDesc invalid pointer");
}

static void validateClipDesc(AudioClipDesc *desc) {
	if (desc == nullptr)
		throw InvalidPointerException("AudioClipDesc invalid pointer");
	// if (desc->decoder.ptr() == nullptr) {
	// 	throw InvalidArgumentException("Requires a valid decoder");
	// }

	if (desc->format < eMono || desc->format > eStero)
		throw InvalidArgumentException("");
	if (desc->datamode < AudioDataMode::LoadedInMemory || desc->datamode > AudioDataMode::DecompressOnLoad)
		throw InvalidArgumentException("");
	if (desc->source != nullptr && desc->size <= 0) {
		throw InvalidArgumentException("");
	}
}

OpenALAudioInterface::OpenALAudioInterface(IConfig *config) {
	this->setName("OpenAL");
	// IConfig& configSettings;
	if (config == nullptr) {
		/*	TODO construct default configuration.	*/
	}
	// AL_SOURCE_TYPE
	// TODO add support
	// const char* device = config->get<const char*>("device");

	const ALchar *defaultDevice = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
	current_device.setName(defaultDevice);

	/*	*/
	setAudioDevice(current_device);

	/*	Internal.	*/
	this->scheduler = Ref<IScheduler>(new TaskScheduler(Math::clamp<int>(SystemInfo::getCPUCoreCount(), 1, 2), 48));
}

OpenALAudioInterface::~OpenALAudioInterface() {

	// TODO check for error.
	/*  Unbind and release context. */
	alcMakeContextCurrent(nullptr);

	alcDestroyContext(this->context);

	// TODO check for error.
	/*  Release device. */
	alcCloseDevice(this->device);
}

void OpenALAudioInterface::OnInitialization() {

	// TODO allocate pool.
	/*	Allocate data.	*/
	// this->source.resize(32);
	// this->clips.resize(32);
	// this->reverbs.resize(32);
}

void OpenALAudioInterface::OnDestruction() {}

AudioClip *OpenALAudioInterface::createAudioClip(AudioClipDesc *desc) {
	validateClipDesc(desc);

	std::vector<ALuint> buffers(1);

	FAOPAL_VALIDATE(alGenBuffers((ALuint)1, &buffers[0]));

	/*	TODO based on the loading type.	*/
	if (desc->datamode == AudioDataMode::LoadedInMemory) {
		long int size;
		/*	Load all data from the stream.	*/
		void *data = desc->decoder->getData(&size);
		/*	*/
		ALenum format = to_al_format(desc->format, desc->samples);
		FAOPAL_VALIDATE(alBufferData(buffers[0], format, data, size, desc->sampleRate));

		free(data);
		desc->decoder->deincreemnt();

	} else {
		throw NotSupportedException();
	}

	return new OpenALAudioClip(*desc);
}

void OpenALAudioInterface::deleteAudioClip(AudioClip *AudioClip) {

	FAOPAL_VALIDATE(alDeleteBuffers(1, nullptr));
	delete AudioClip;
}

AudioSource *OpenALAudioInterface::createAudioSource(AudioSourceDesc *desc) {
	validateAudioSourceDesc(desc);

	// TODO add validation/error check.
	ALuint source;
	FAOPAL_VALIDATE(alGenSources(1, &source));

	FAOPAL_VALIDATE(alSourcef(source, AL_PITCH, 1));
	// check for errors
	FAOPAL_VALIDATE(alSourcef(source, AL_GAIN, 1));
	// check for errors
	FAOPAL_VALIDATE(alSource3f(source, AL_POSITION, 0, 0, 0));
	// check for errors
	FAOPAL_VALIDATE(alSource3f(source, AL_VELOCITY, 0, 0, 0));
	// check for errors
	FAOPAL_VALIDATE(alSourcei(source, AL_LOOPING, AL_FALSE));

	OpenALAudioSource *audioSource = new OpenALAudioSource(*desc, source);

	return audioSource;
}

void OpenALAudioInterface::deleteAudioSource(AudioSource *audioSource) { delete audioSource; }

static void validateAudioReverbDesc(AudioReverbDesc *desc) {}

AudioReverb *OpenALAudioInterface::createAudioReverb(AudioReverbDesc *desc) {
	validateAudioReverbDesc(desc);

	/*	Validate the system.	*/
	if (!this->supportEffects)
		throw RuntimeException("");

	return nullptr;
}

void OpenALAudioInterface::deleteAudioReverb(AudioReverb *reverb) { delete reverb; }

AudioListener *OpenALAudioInterface::createAudioListener(AudioListenerDesc *desc) {

	OpenALAudioListener *listener = new OpenALAudioListener();

	/*	Set initial default state of the listener.	*/
	listener->setPosition(desc->position);
	listener->setVelocity(Vector3::Zero());
	listener->setOrientation(desc->rotation);

	return listener;
}

void OpenALAudioInterface::deleteAudioListener(AudioListener *listener) { delete listener; }

void OpenALAudioInterface::setAudioListener(AudioListener *listener) {
	// audio->
}

AudioCapture *OpenALAudioInterface::createAudioCapture() { return nullptr; }
void OpenALAudioInterface::deleteAudioCapture(AudioCapture *capture) {}

std::vector<AudioPhysicalDevice> OpenALAudioInterface::getDevices() const {
	std::vector<AudioPhysicalDevice> listDevices;

	const ALCchar *devices = nullptr;
	const ALCchar *mices = nullptr;
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
		std::string deviceName = std::string(device);
		audioPhysicalDevices.setName(deviceName);
		listDevices.push_back(audioPhysicalDevices);

		/*	*/
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}

	return listDevices;
}

void OpenALAudioInterface::setAudioDevice(const AudioPhysicalDevice &device) {
	ALint attribs[4] = {0};

	if (alcGetCurrentContext() != nullptr) {
		ALCcontext *context = alcGetCurrentContext();
		ALCdevice *curDevice = alcGetContextsDevice(context);

		// Same device.
		if (strcmp(alcGetString(curDevice, ALC_DEVICE_SPECIFIER), device.getName().c_str()) == 0) {
			return;
		}

		// Not same device. continue with selecting audio device.
	}

	if (alcGetCurrentContext() != nullptr) {
		/*  */
		if (!alcCloseDevice(this->device)) {
			throw RuntimeException("Failed to open audio device {}", device.getName().c_str());
		}
	}

	/*  */
	this->device = alcOpenDevice(device.getName().c_str());
	if (!this->device) {
		throw RuntimeException("Failed to open audio device {}", device.getName().c_str());
	}

	if (alcIsExtensionPresent(this->device, ALC_EXT_EFX_NAME)) {

		this->alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
		this->alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
		this->alIsEffect = (LPALISEFFECT)alGetProcAddress("alIsEffect");

		if (!(this->alGenEffects && this->alDeleteEffects && this->alIsEffect))
			this->supportEffects = false;

		/* Use Context creation hint to request 4 Auxiliary */ /* Sends per Source */
		attribs[0] = ALC_MAX_AUXILIARY_SENDS;
		attribs[1] = 4;

		this->supportEffects = true;
	}

	/*  Create context. */
	this->context = alcCreateContext(this->device, attribs);
	if (!this->context) {
		throw RuntimeException("Failed to create audio context for device {}", device.getName().c_str());
	}

	if (!alcMakeContextCurrent(this->context)) {
		throw RuntimeException("Failed to make current audio device {}", device.getName().c_str());
	}

	// alcGetIntegerv(audio->device, ALC_MAX_AUXILIARY_SENDS, 1, &iSends);
}

const char *OpenALAudioInterface::getVersion() const { return FV_STR_VERSION(1, 0, 0); }

const AudioPhysicalDevice &OpenALAudioInterface::getAudioDevice() const { return current_device; }

extern "C" AudioInterface *createInternalAudioInterface(IConfig *config) { return new OpenALAudioInterface(config); }
