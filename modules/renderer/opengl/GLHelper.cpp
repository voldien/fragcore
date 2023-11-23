#include "GLHelper.h"
#include <GL/glew.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	void checkError() {
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			throw RuntimeException("glGetError indicated an error: {}", (const char *)glewGetErrorString(error));
		}
	}

	/**
	 * @brief Helper function for extracting error from OpenGL.
	 *
	 */
	void resetErrorFlag() {
		while (glGetError() != GL_NO_ERROR) {
		}
	}
} // namespace fragcore