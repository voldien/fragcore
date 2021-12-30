#include"GLQuery.h"
#include <GL/glew.h>

using namespace fragcore;

GLQueryObject::~GLQueryObject() {}

static GLenum getTarget(QueryObject::Target target) {
	switch (target) {
	case QueryObject::Samples:
		return GL_SAMPLES;
	case QueryObject::AnySamples:
		return GL_ANY_SAMPLES_PASSED;
	case QueryObject::TimePassed:
		return GL_TIME_ELAPSED;
	case QueryObject::eCurrentTime:
		return GL_TIMESTAMP;
	case QueryObject::PrimitiveGenerate:
		return GL_PRIMITIVES_GENERATED;
		return GL_ANY_SAMPLES_PASSED_CONSERVATIVE;
	default:
		throw RuntimeException("Invalid Query target");
	}
}

void GLQueryObject::begin(Target target, unsigned int index) {
	glBeginQueryIndexed(getTarget(target), index, this->query[index]);
}

void GLQueryObject::end(Target target, unsigned int index) {

	glEndQueryIndexed(getTarget(target), index);
}

long int GLQueryObject::getResult(int index) {

	GLuint64 finalResult;
	glGetQueryObjectui64v(this->query[index], GL_QUERY_RESULT, &finalResult);
	return (long int)finalResult;
}

static GLenum getConditionTarget(QueryObject::Condition condition) {

	switch (condition) {
	case QueryObject::Condition::eNoWait:
		return GL_QUERY_NO_WAIT;
	default:
		throw RuntimeException("Invalid query condition");
	}
	return GL_QUERY_BY_REGION_WAIT;
}

void GLQueryObject::beginConditionalRenderer(Condition target) {


	GLenum condition = getConditionTarget(target);
	glBeginConditionalRender(query[0], condition);
}

void GLQueryObject::endConditionalRenderer() { glEndConditionalRender(); }

intptr_t GLQueryObject::getNativePtr() const { return 0; }
