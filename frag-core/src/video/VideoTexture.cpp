#include"Video/VideoTexture.h"
#include"Video/VideoManager.h"
#include"Audio/decoder/AudioDecoder.h"
#include"Core/Ref.h"
using namespace fragcore;

VideoTexture::VideoTexture() {
	//VideoManager::getInstance()->registerVideo(Ref<VideoTexture>(this));
}

VideoTexture::~VideoTexture() {
	//VideoManager::getInstance()->unRegisterVideo(Ref<VideoTexture>(this));
}

bool VideoTexture::isPlaying() const{

}

void VideoTexture::play() {
	
}

void VideoTexture::stop() {

}

void VideoTexture::pause() {

}

float VideoTexture::getCurrentPosition() const {

}

float VideoTexture::getTotalLength() const {

}

double VideoTexture::getDuration() const {

}

double VideoTexture::getTime() const {

}

bool VideoTexture::isReadyToPlay() const {

}

Texture *VideoTexture::getTexture() const {
	return (Texture*)this->texture.ptr();
}