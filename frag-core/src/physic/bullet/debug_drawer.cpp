#include <Core/Log.h>
#include"physic/bullet/internal_object_type.h"
#include"physic/bullet/debugDrawer.h"

using namespace fragcore;

DebugDrawer::DebugDrawer(IRenderer *renderer) {
	renderer->increment();
	this->renderer = renderer;

	/*  Create buffers. */

	/*  Create shaders. */
}


btIDebugDraw::DefaultColors DebugDrawer::getDefaultColors() const {
	return btIDebugDraw::getDefaultColors();
}

void DebugDrawer::setDefaultColors(const btIDebugDraw::DefaultColors &colors) {
	btIDebugDraw::setDefaultColors(colors);
}


void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {

}

void
DebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
                              const btVector3 &color) {

}

void DebugDrawer::reportErrorWarning(const char *warningString) {
	Log::log(Log::Warning, warningString);
}

void DebugDrawer::draw3dText(const btVector3 &location, const char *textString) {

}

void DebugDrawer::setDebugMode(int debugMode) {

}

int DebugDrawer::getDebugMode() const {
	return 0;
}

void DebugDrawer::clearLines() {


	btIDebugDraw::clearLines();
}

void DebugDrawer::flushLines() {
	btIDebugDraw::flushLines();


}
