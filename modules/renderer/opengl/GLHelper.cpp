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
			throw RuntimeException("glGetError indicated an error: {} ({})", (const char *)glewGetErrorString(error),
								   error);
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

	bool validateExistingProgram() {
		int program;
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);
		glValidateProgram(program);
		fragcore::checkError();
		int status;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		return status == GL_TRUE;
	}

	std::string getProgramValidateString() {
		int program;
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);

		int status;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		if (!status) {
			GLchar errorLog[1024] = {0};
			glGetProgramInfoLog(program, 1024, nullptr, errorLog);
			return errorLog;
		}
		return "";
	}
} // namespace fragcore