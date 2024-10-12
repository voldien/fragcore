#include <AudioClip.h>
#include <AudioFactory.h>
#include <AudioListener.h>
#include <AudioSource.h>
#include <FragCore.h>
#include <OpenALAudioInterface.h>
#include <decoder/AudioDecoder.h>
#include <decoder/VorbisDecoder.h>

using namespace fragcore;

class AudioPlaybackExample {
  public:
	AudioPlaybackExample(int argc, const char **argv) {

		if (argc < 2) {
			throw InvalidArgumentException("Required path to an audio file");
		}

		/*	*/
		Ref<AudioInterface> iaudio = Ref<AudioInterface>(new OpenALAudioInterface(nullptr));

		Ref<IScheduler> sch = Ref<IScheduler>(nullptr);
		FileSystem *fileSystem = FileSystem::createFileSystem(sch);

		/*	*/
		std::vector<AudioPhysicalDevice> devices = iaudio->getDevices();
		std::vector<AudioPhysicalDevice>::iterator device_it = devices.begin();

		/*	*/
		std::cout << "Physical Audio Devices" << std::endl;
		for (; device_it != devices.end(); device_it++) {
			std::cout << fmt::format("{}", (*device_it).getName()) << std::endl;
		}

		AudioListenerDesc list_desc = {.position = Vector3(0, 0, 0), .rotation = Quaternion::Identity()};

		Ref<AudioListener> listener = Ref<AudioListener>(iaudio->createAudioListener(&list_desc));
		listener->setVolume(1.0f);
		AudioSourceDesc source_desc = {};
		source_desc.position = Vector3::Zero();
		Ref<AudioSource> audioSource = Ref<AudioSource>(iaudio->createAudioSource(&source_desc));

		AudioClipDesc clip_desc = {};
		Ref<IO> file = Ref<IO>(fileSystem->openFile(argv[1], IO::READ));
		file->seek(0, IO::SET);

		Ref<AudioDecoder> decoder = Ref<AudioDecoder>(new VorbisAudioDecoder(file));
		printf("bits ! %d\n", decoder->getSampleBitResolution());
		clip_desc.decoder = decoder;
		clip_desc.samples = decoder->getSampleBitResolution();
		clip_desc.sampleRate = decoder->getSampleRate();
		clip_desc.format = AudioFormat::eStero;
		clip_desc.datamode = AudioDataMode::LoadedInMemory;

		Ref<AudioClip> clip = Ref<AudioClip>(iaudio->createAudioClip(&clip_desc));
		printf("Created the audio clip.\n");

		/*	*/
		audioSource->setClip(*clip);
		printf("Playing the sound.\n");
		audioSource->play();

		/*	Play the audio intill it ends.	*/
		usleep(10);
		while (audioSource->isPlaying()) {
			printf("pos sec: %f\n", audioSource->getPos());
			usleep(500);
			// audioSource->setVolume(std::abs(sin(audioSource->getPos())));
		}

		sleep(10);
		delete *iaudio;
	}
	~AudioPlaybackExample() {}
};

int main(int argc, const char **argv) {

	try {
		AudioPlaybackExample(argc, argv);
	} catch (const std::exception &ex) {
		std::cerr << cxxexcept::getStackMessage(ex) << std::endl;
		return EXIT_FAILURE;
	}
}