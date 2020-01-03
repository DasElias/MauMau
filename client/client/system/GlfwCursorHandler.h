#pragma once

#include <GLFW/glfw3.h>
#include <egui/input/CursorHandler.h>


namespace card {
	class GlfwCursorHandler : public egui::CursorHandler {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			GlfwCursorHandler(GLFWwindow* window);

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			GLFWwindow* window;
			double cursorX;
			double cursorY;
			double deltaCursorX;
			double deltaCursorY;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void update() override;
			float getDeltaX() override;
			float getDeltaY() override;
			float getCursorX() override;
			float getCursorY() override;
			void setCursorPosition(float x, float y) override;
	};
}
