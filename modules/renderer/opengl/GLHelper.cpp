#include "GLHelper.h"
#include <GL/glew.h>

namespace fragcore {

	void checkError() {
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			throw RuntimeException("glGetError indicated an error: {}", (const char *)glewGetErrorString(error));
		}
	}

	/*  Helper function for extracting error from OpenGL.   */
	void resetErrorFlag() {
		while (glGetError() != GL_NO_ERROR) {
		}
	}
} // namespace fragcore