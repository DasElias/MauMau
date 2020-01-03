#include "GlfwDisplayHandler.h"

namespace card {
	GlfwDisplayHandler::GlfwDisplayHandler(GLFWwindow* window, int startWidth, int startHeight) :
			window(window),
			width(startWidth),
			height(startHeight),
			framebufferWidth(startWidth),
			framebufferHeight(startHeight),
			contentScaleX(1),
			contentScaleY(1) {
	}

	void GlfwDisplayHandler::update() {
		glfwGetWindowSize(window, &width, &height);
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		glfwGetWindowContentScale(window, &contentScaleX, &contentScaleY);
	}

	int GlfwDisplayHandler::getWidth() {
		return width;
	}

	int GlfwDisplayHandler::getHeight() {
		return height;
	}

	int GlfwDisplayHandler::getFramebufferWidth() {
		return framebufferWidth;
	}

	int GlfwDisplayHandler::getFramebufferHeight() {
		return framebufferHeight;
	}

	float GlfwDisplayHandler::getContentScaleX() {
		return contentScaleX;
	}

	float GlfwDisplayHandler::getContentScaleY() {
		return contentScaleY;
	}

}

