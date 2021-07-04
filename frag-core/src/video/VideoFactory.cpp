#include "Video/VideoFactory.h"
#include "Audio/AudioInterface.h"
#include "Audio/decoder/AudioDecoder.h"
#include "Core/IO/IO.h"
#include "Exception/RuntimeException.h"
#include <Exception/InvalidArgumentException.h>
#include <Renderer/RenderDesc.h>
#include <Utils/StringUtil.h>
#include <Utils/TextureUtil.h>
#include <fmt/core.h>
// TODO add support if libav does not support it.
// #include <theora/theora.h>
// #include<theora/theoradec.h>

using namespace fragcore;

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavcodec/vdpau.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55, 28, 1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif

extern void libAVComputeVideoTask(Task *task);

typedef struct fv_libav_video_header_t {

	/*  */
	struct AVFormatContext *pformatCtx;
	struct AVCodecContext *pVideoCtx;
	struct AVCodecContext *pAudioCtx;

	/*  */
	int videoStream;
	int audioStream;

	/*  */
	struct AVFrame *frame;
	struct AVFrame *frameoutput;
	struct SwsContext *sws_ctx;
	Ref<IO> io;
	Ref<VideoTexture> refVideo;
	Ref<Texture> texture;
	/*  TODO add audio reference object.    */

	unsigned int flag;
	double video_clock;
	double frame_timer;
	double frame_last_pts;
	double frame_last_delay;

} FVAVLibVideoHeader;

int ReadFunc(void *ptr, uint8_t *buf, int buf_size) {
	Ref<IO> *pStream = reinterpret_cast<Ref<IO> *>(ptr);
	unsigned long bytesRead = 0;
	bytesRead = (*pStream)->read(buf_size, buf);
	if (bytesRead != buf_size)
		return AVERROR_EOF;
	return bytesRead;
}

int64_t SeekFunc(void *ptr, int64_t pos, int whence) {
	Ref<IO> *pStream = reinterpret_cast<Ref<IO> *>(ptr);

	// Seek:
	unsigned long in = pos;
	if (whence == AVSEEK_SIZE)
		return (*pStream)->length();
	(*pStream)->seek(in, (IO::Seek)whence);
	// Return the new position:
	return (*pStream)->getPos();
}

static enum AVPixelFormat get_format(struct AVCodecContext *s, const enum AVPixelFormat *fmt) {

	/*  Iterate supported format the video can be encoded or decode with.   */
	const enum AVPixelFormat *pfmt = fmt;
	while (*pfmt != AV_PIX_FMT_NONE) {
		pfmt++;
	}
	return AV_PIX_FMT_YUV420P;
}

VideoTexture *VideoFactory::loadVideoTexture(Ref<IO> &ref, AudioClip **audio, IRenderer *renderer,
											 AudioInterface *audioInterface) {
	int result;
	FVAVLibVideoHeader header = {};

	if (!renderer)
		throw InvalidArgumentException("Requires rendering interface object.");

	const int iBufSize = 32 * 1024;
	uint8_t *pBuffer = new uint8_t[iBufSize];

	// Allocate the AVIOContext:
	AVIOContext *pIOCtx = avio_alloc_context(pBuffer, iBufSize, // internal Buffer and its size
											 0,					// bWriteable (1=true,0=false)
											 &ref,				// user data ; will be passed to our callback functions
											 ReadFunc,
											 0, // Write callback function (not used in this example)
											 SeekFunc);
	header.pformatCtx = avformat_alloc_context();
	header.pformatCtx->pb = pIOCtx;

	// Determining the input format:
	unsigned long ulReadBytes = 0;
	ulReadBytes = ref->read(iBufSize, pBuffer);
	ref->seek(0, IO::SET);

	// Now we set the ProbeData-structure for av_probe_input_format:
	AVProbeData probeData = {};
	probeData.buf = pBuffer;
	probeData.buf_size = ulReadBytes;
	probeData.filename = "";

	// Determine the input-format:
	header.pformatCtx->iformat = av_probe_input_format(&probeData, 1);

	result = avformat_open_input(&header.pformatCtx, "", 0, 0);
	if (result != 0) {
		char buf[AV_ERROR_MAX_STRING_SIZE];
		av_strerror(result, buf, sizeof(buf));
		throw RuntimeException(fmt::format("Failed to open input : %s", buf));
	}

	if ((result = avformat_find_stream_info(header.pformatCtx, nullptr)) < 0) {
		char buf[AV_ERROR_MAX_STRING_SIZE];
		av_strerror(result, buf, sizeof(buf));
		throw RuntimeException(fmt::format"Failed to retrieve info from stream info : %s", buf));
	}
	struct AVStream *video_st = nullptr;
	struct AVStream *audio_st = nullptr;

	/*	Get video codecs.	*/
	for (int x = 0; x < header.pformatCtx->nb_streams; x++) {
		AVStream *stream = header.pformatCtx->streams[x];

		/*  */
		if (stream->codecpar == nullptr)
			continue;

		switch (stream->codecpar->codec_type) {
		case AVMEDIA_TYPE_AUDIO:
			header.audioStream = x;
			audio_st = stream;
			break;
		case AVMEDIA_TYPE_SUBTITLE:
			break;
		case AVMEDIA_TYPE_VIDEO:
			header.videoStream = x;
			video_st = stream;
			break;
		}
	}

	/*  Get selected codec parameters. */
	if (!video_st)
		throw RuntimeException("Failed to find video stream.");

	if (audio && audioInterface && audio_st) {
		AVCodecParameters *pAudioCodecParam = audio_st->codecpar;

		/*  Create audio clip.  */
		AVCodec *audioCodec = avcodec_find_decoder(pAudioCodecParam->codec_id);
		header.pAudioCtx = avcodec_alloc_context3(audioCodec);
		if (!header.pAudioCtx)
			throw RuntimeException("Failed to create audio decode context");

		result = avcodec_parameters_to_context(header.pAudioCtx, pAudioCodecParam);
		if (result < 0) {
			char buf[AV_ERROR_MAX_STRING_SIZE];
			av_strerror(result, buf, sizeof(buf));
			throw RuntimeException(fmt::format"Failed to set codec parameters : %s", buf));
		}

		result = avcodec_open2(header.pAudioCtx, audioCodec, nullptr);
		if (result < 0) {
			char buf[AV_ERROR_MAX_STRING_SIZE];
			av_strerror(result, buf, sizeof(buf));
			throw RuntimeException(fmt::format"Failed to retrieve info from stream info : %s", buf));
		}
	}

	AVCodecParameters *pVideoCodecParam = video_st->codecpar;

	AVCodec *pVideoCodec = avcodec_find_decoder(pVideoCodecParam->codec_id);
	if (pVideoCodec == nullptr)
		throw RuntimeException("failed to find decoder");
	header.pVideoCtx = avcodec_alloc_context3(pVideoCodec);
	if (header.pVideoCtx == nullptr)
		throw RuntimeException("Failed to allocate video decoder context");

	// AV_PIX_FMT_FLAG_RGB
	/*  Modify the target pixel format. */
	// header.pVideoCtx->get_format = get_format;
	//	pVideoCodecParam->format = AV_PIX_FMT_BGR24;
	//	pVideoCodecParam->codec_tag = avcodec_pix_fmt_to_codec_tag(AV_PIX_FMT_BGR24);
	//	pVideoCodecParam->color_space = AVCOL_SPC_RGB;
	result = avcodec_parameters_to_context(header.pVideoCtx, pVideoCodecParam);
	if (result < 0) {
		char buf[AV_ERROR_MAX_STRING_SIZE];
		av_strerror(result, buf, sizeof(buf));
		throw RuntimeException(fmt::format"Failed to set codec parameters : %s", buf));
	}
	// av_find_best_pix_fmt_of_2
	// avcodec_default_get_format()

	if ((result = avcodec_open2(header.pVideoCtx, pVideoCodec, nullptr)) != 0) {
		char buf[AV_ERROR_MAX_STRING_SIZE];
		av_strerror(result, buf, sizeof(buf));
		throw RuntimeException(fmt::format"Failed to retrieve info from stream info : %s", buf));
	}

	header.frame = av_frame_alloc();
	header.frameoutput = av_frame_alloc();
	int m_bufferSize = av_image_get_buffer_size(AV_PIX_FMT_BGRA, header.pVideoCtx->width, header.pVideoCtx->height, 4);
	void *m_buffer = (uint8_t *)av_malloc(m_bufferSize);
	av_image_alloc(header.frameoutput->data, header.frameoutput->linesize, header.pVideoCtx->width,
				   header.pVideoCtx->height, AV_PIX_FMT_BGRA, 4);

	if (header.frame == nullptr)
		throw RuntimeException(fmt::format"Failed to allocate frame"));

	// m_bufferSize = avpicture_get_size(PIX_FMT_RGB24, width, height);
	// m_buffer = (uint8_t *)av_malloc(m_bufferSize);

	// m_bufferSize = avpicture_get_size(PIX_FMT_RGB24, width, height);
	// m_buffer = (uint8_t *)av_malloc(m_bufferSize);

	// AVPacket *pPacket = av_packet_alloc();
	header.sws_ctx = sws_getContext(header.pVideoCtx->width, header.pVideoCtx->height, header.pVideoCtx->pix_fmt,
									header.pVideoCtx->width, header.pVideoCtx->height, AV_PIX_FMT_BGRA, SWS_BICUBIC,
									nullptr, nullptr, nullptr);

	header.frame_timer = av_gettime() / 1000000.0;

	// TODO relocate.
	// TODO add audio decoder.
	struct AVPacket packet;
	int res;
	res = av_seek_frame(header.pformatCtx, header.videoStream, 60000, AVSEEK_FLAG_FRAME);
	while (true) {

		res = av_read_frame(header.pformatCtx, &packet);
		if (res == 0) {
			if (packet.stream_index == header.videoStream) {
				result = avcodec_send_packet(header.pVideoCtx, &packet);
				if (result < 0) {
					char buf[AV_ERROR_MAX_STRING_SIZE];
					av_strerror(result, buf, sizeof(buf));
					throw RuntimeException(fmt::format"Failed to send packet for decoding picture frame : %s", buf));
				}

				while (result >= 0) {
					result = avcodec_receive_frame(header.pVideoCtx, header.frame);
					if (result == AVERROR(EAGAIN) || result == AVERROR_EOF)
						break;
					if (result < 0) {
						char buf[AV_ERROR_MAX_STRING_SIZE];
						av_strerror(result, buf, sizeof(buf));
						throw RuntimeException(fmt::format" : %s", buf));
					}
					if (header.frame->format != AV_PIX_FMT_BGRA) {

						if (header.frame->format == AV_PIX_FMT_YUV420P) {
							header.frame->data[0] =
								header.frame->data[0] + header.frame->linesize[0] * (header.pVideoCtx->height - 1);
							header.frame->data[1] =
								header.frame->data[1] + header.frame->linesize[0] * header.pVideoCtx->height / 4 - 1;
							header.frame->data[2] =
								header.frame->data[2] + header.frame->linesize[0] * header.pVideoCtx->height / 4 - 1;

							header.frame->linesize[0] *= -1;
							header.frame->linesize[1] *= -1;
							header.frame->linesize[2] *= -1;
							sws_scale(header.sws_ctx, header.frame->data, header.frame->linesize, 0,
									  header.frame->height, header.frameoutput->data, header.frameoutput->linesize);
						}
					}
				}
			}
			if (packet.stream_index == header.audioStream) {
				result = avcodec_send_packet(header.pAudioCtx, &packet);
				if (result < 0) {
					char buf[AV_ERROR_MAX_STRING_SIZE];
					av_strerror(result, buf, sizeof(buf));
					throw RuntimeException(fmt::format"Failed to send packet for decoding audio frame : %s", buf));
				}

				while (result >= 0) {
					result = avcodec_receive_frame(header.pAudioCtx, header.frame);
					if (result == AVERROR(EAGAIN) || result == AVERROR_EOF)
						break;
					if (result < 0) {
						char buf[AV_ERROR_MAX_STRING_SIZE];
						av_strerror(result, buf, sizeof(buf));
						throw RuntimeException(fmt::format" : %s", buf));
					}

					av_get_channel_layout_nb_channels(header.frame->channel_layout);
					header.frame->format != AV_SAMPLE_FMT_S16P;
					header.frame->channel_layout;
					AudioClip *clip;
					clip->setData(header.frame->extended_data[0], header.frame->linesize[0], 0);
				}
			}

			printf("duration %f\n", (float)packet.duration);
		}
		// av_packet_free(&packet);
	}

	/*	Create Audio clip.	*/
	Ref<AudioDecoder> video_audio_decoder;
	AudioClipDesc clip_desc = {};
	clip_desc.decoder = video_audio_decoder;
	clip_desc.samples = header.pVideoCtx->sample_fmt;
	clip_desc.sampleRate = header.pVideoCtx->channels * header.pVideoCtx->sample_rate;
	clip_desc.format = (AudioFormat)header.pVideoCtx->channels;
	*audio = audioInterface->createAudioClip(&clip_desc);

	TextureDesc desc = {};

	/*  Texture attributes. */
	desc.width = header.frame->width;
	desc.height = header.frame->height;
	desc.depth = 1;
	// av_read_image_line2()
	desc.pixel = header.frameoutput->data[0];
	desc.pixelSize = 0;
	desc.compression = TextureDesc::eNoCompression;

	/*  Texture.    */
	desc.target = TextureDesc::eTexture2D;
	desc.pixelFormat = TextureFormat::BGRA32;
	desc.graphicFormat = GraphicFormat::R8G8B8A8_SRGB;
	desc.type = TextureDesc::eUnsignedByte;
	desc.numlevel = 4;
	desc.usemipmaps = 1;
	desc.srgb = 0;

	/*  sampler.    */
	desc.sampler.AddressU = SamplerDesc::eRepeat;
	desc.sampler.AddressV = SamplerDesc::eRepeat;
	desc.sampler.AddressW = SamplerDesc::eRepeat;
	desc.sampler.anisotropy = 8.0f;
	desc.sampler.minFilter = SamplerDesc::eLinear;
	desc.sampler.magFilter = SamplerDesc::eLinear;
	desc.sampler.mipmapFilter = SamplerDesc::eLinear;

	desc.marker.markerName = ref->getName().c_str();
	Texture *texture = renderer->createTexture(&desc);

	VideoTexture *videoTexture = nullptr;
	videoTexture->audioClip = Ref<AudioClip>(*audio);
	// videoTexture->decoder = video_audio_decoder;
	videoTexture->texture = Ref<Texture>(texture);
	videoTexture->taskcallback = libAVComputeVideoTask;

	header.io = ref;
	header.texture = Ref<Texture>(texture);
	//	header.refVideo = Ref<VideoTexture>(videoTexture);
	ref->increment();

	return videoTexture;
}

// TODO determine where it shall be relocated and what class.
void libAVComputeVideoTask(Task *task) {
	VideoTexture *texture = (VideoTexture *)task->userData;
	FVAVLibVideoHeader header = {}; // TODO change to reference.
	// FVAVLibVideoHeader
	// TODO relocate.
	// TODO add audio decoder.
	struct AVPacket packet;
	int res;
	int result;
	res = av_seek_frame(header.pformatCtx, header.videoStream, 60000, AVSEEK_FLAG_FRAME);
	while (true) {

		res = av_read_frame(header.pformatCtx, &packet);
		if (res == 0) {
			if (packet.stream_index == header.videoStream) {
				result = avcodec_send_packet(header.pVideoCtx, &packet);
				if (result < 0) {
					char buf[AV_ERROR_MAX_STRING_SIZE];
					av_strerror(result, buf, sizeof(buf));
					throw RuntimeException(fmt::format"Failed to send packet for decoding picture frame : %s", buf));
				}

				while (result >= 0) {
					result = avcodec_receive_frame(header.pVideoCtx, header.frame);
					if (result == AVERROR(EAGAIN) || result == AVERROR_EOF)
						break;
					if (result < 0) {
						char buf[AV_ERROR_MAX_STRING_SIZE];
						av_strerror(result, buf, sizeof(buf));
						throw RuntimeException(fmt::format" : %s", buf));
					}
					if (header.frame->format != AV_PIX_FMT_BGRA) {

						if (header.frame->format == AV_PIX_FMT_YUV420P) {
							header.frame->data[0] =
								header.frame->data[0] + header.frame->linesize[0] * (header.pVideoCtx->height - 1);
							header.frame->data[1] =
								header.frame->data[1] + header.frame->linesize[0] * header.pVideoCtx->height / 4 - 1;
							header.frame->data[2] =
								header.frame->data[2] + header.frame->linesize[0] * header.pVideoCtx->height / 4 - 1;

							header.frame->linesize[0] *= -1;
							header.frame->linesize[1] *= -1;
							header.frame->linesize[2] *= -1;
							sws_scale(header.sws_ctx, header.frame->data, header.frame->linesize, 0,
									  header.frame->height, header.frameoutput->data, header.frameoutput->linesize);
						}
					}
				}
			}
			if (packet.stream_index == header.audioStream) {
				result = avcodec_send_packet(header.pAudioCtx, &packet);
				if (result < 0) {
					char buf[AV_ERROR_MAX_STRING_SIZE];
					av_strerror(result, buf, sizeof(buf));
					throw RuntimeException(fmt::format"Failed to send packet for decoding audio frame : %s", buf));
				}

				while (result >= 0) {
					result = avcodec_receive_frame(header.pAudioCtx, header.frame);
					if (result == AVERROR(EAGAIN) || result == AVERROR_EOF)
						break;
					if (result < 0) {
						char buf[AV_ERROR_MAX_STRING_SIZE];
						av_strerror(result, buf, sizeof(buf));
						throw RuntimeException(fmt::format" : %s", buf));
					}

					av_get_channel_layout_nb_channels(header.frame->channel_layout);
					header.frame->format != AV_SAMPLE_FMT_S16P;
					header.frame->channel_layout;
					AudioClip *clip;
					clip->setData(header.frame->extended_data[0], header.frame->linesize[0], 0);
				}
			}

			printf("duration %f\n", (float)packet.duration);
		}
		// av_packet_free(&packet);
	}
}

/*
av_seek_frame(header->pformatCtx, header->videoStream, header->video_clock * AV_TIME_BASE +
		av_q2d( header->pVideoCtx->time_base) , AVSEEK_FLAG_BACKWARD );
*/
