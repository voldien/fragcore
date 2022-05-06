#include "VideoTexture.h"
#include "VideoManager.h"
#include <Core/Ref.h>
using namespace fragcore;

VideoTexture::VideoTexture() {
	// VideoManager::getInstance()->registerVideo(Ref<VideoTexture>(this));
}

VideoTexture::~VideoTexture() {
	// VideoManager::getInstance()->unRegisterVideo(Ref<VideoTexture>(this));
}

bool VideoTexture::isPlaying() const { return false; }

void VideoTexture::play() {}

void VideoTexture::stop() {}

void VideoTexture::pause() {}

float VideoTexture::getCurrentPosition() const { return 0; }

float VideoTexture::getTotalLength() const { return 0; }

double VideoTexture::getDuration() const { return 0; }

double VideoTexture::getTime() const { return 0; }

bool VideoTexture::isReadyToPlay() const { return false; }

Texture *VideoTexture::getTexture() const { return (Texture *)this->texture.ptr(); }