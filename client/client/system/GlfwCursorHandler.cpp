#include "GlfwCursorHandler.h"

namespace card {
	GlfwCursorHandler::GlfwCursorHandler(GLFWwindow* window) :
			window(window),
			cursorX(0),
			cursorY(0),
			deltaCursorX(0),
			deltaCursorY(0) {
	}

	void GlfwCursorHandler::update() {
		double oldCursorX = cursorX;
		double oldCursorY = cursorY;

		glfwGetCursorPos(window, &cursorX, &cursorY);

		deltaCursorX = cursorX - oldCursorX;
		deltaCursorY = cursorY - oldCursorY;

	}

	float GlfwCursorHandler::getDeltaX() {
		return float(deltaCursorX);
	}

	float GlfwCursorHandler::getDeltaY() {
		return float(deltaCursorY);
	}

	float GlfwCursorHandler::getCursorX() {
		return float(cursorX);
	}

	float GlfwCursorHandler::getCursorY() {
		return float(cursorY);
	}

	void GlfwCursorHandler::setCursorPosition(float x, float y) {
		glfwSetCursorPos(window, x, y);
	}

}
