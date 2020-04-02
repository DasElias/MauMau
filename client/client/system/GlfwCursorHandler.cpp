#include "GlfwCursorHandler.h"
#include "GlfwCursorHandler.h"
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

	void GlfwCursorHandler::setCursorType(egui::CursorType type) {
		GLFWcursor* cursor = createCursorIfNecessary(type);
		glfwSetCursor(window, cursor);
	}

	GLFWcursor* GlfwCursorHandler::createCursorIfNecessary(egui::CursorType type) {
		if(cursors.find(type) == cursors.end()) {
			int offset = static_cast<int>(type);
			int shape = GLFW_ARROW_CURSOR + offset;
			GLFWcursor* c = glfwCreateStandardCursor(shape);
			cursors.insert(std::make_pair(type, c));
			return c;
		} else {
			return cursors.at(type);
		}
	}

}
