#pragma once

#include <GLFW/glfw3.h>

#include <egui/input/DisplayHandler.h>

namespace card {
	class GlfwDisplayHandler : public egui::DisplayHandler {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			GLFWwindow* window;
			int width;
			int height;
			int framebufferWidth;
			int framebufferHeight;
			float contentScaleX;
			float contentScaleY;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			GlfwDisplayHandler(GLFWwindow* window, int startWidth, int startHeight);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void update() override;
			int getWidth() override;
			int getHeight() override;
			int getFramebufferWidth() override;
			int getFramebufferHeight() override;
			float getContentScaleX() override;
			float getContentScaleY() override;
	};

}
