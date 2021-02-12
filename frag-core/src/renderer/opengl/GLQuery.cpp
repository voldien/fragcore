#include"Renderer/Query.h"
#include"Renderer/opengl/internal_object_type.h"
#include"Exception/InvalidArgumentException.h"
#include "Exception/RuntimeException.h"
#include<GL/glew.h>


using namespace fragcore;

QueryObject::~QueryObject(void) {

}

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

void QueryObject::begin(Target target, unsigned int index) {
	GLQuery * query = (GLQuery * )this->getObject();
	glBeginQueryIndexed(getTarget(target), index, query->query[index]);
}

void QueryObject::end(Target target, unsigned int index) {
	GLQuery * query = (GLQuery * )this->getObject();
	glEndQueryIndexed(getTarget(target), index);
}

long int QueryObject::getResult(int index){
    GLQuery * query = (GLQuery * )this->getObject();

    GLuint64 finalResult;
    glGetQueryObjectui64v(query->query[index], GL_QUERY_RESULT, &finalResult);
    return (long int)finalResult;
}

static GLenum getConditionTarget(QueryObject::Condition condition){

    switch(condition){
        case QueryObject::Condition::eNoWait:
            return GL_QUERY_NO_WAIT;
        default:
            throw RuntimeException("Invalid query condition");
    }
    return GL_QUERY_BY_REGION_WAIT;
}

void QueryObject::beginConditionalRenderer(Condition target) {
	GLQuery * query = (GLQuery * )this->getObject();

	GLenum condition = getConditionTarget(target);
	glBeginConditionalRender(query->query[0], condition);
}

void QueryObject::endConditionalRenderer(void) {
	glEndConditionalRender();
}


intptr_t QueryObject::getNativePtr(void) const {
	return 0;
}
