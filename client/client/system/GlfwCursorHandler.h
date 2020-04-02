#pragma once

#include <GLFW/glfw3.h>
#include <egui/input/CursorHandler.h>
#include <map>

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
			std::map<egui::CursorType, GLFWcursor*> cursors;

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
			void setCursorType(egui::CursorType type) override;

		private:
			GLFWcursor* createCursorIfNecessary(egui::CursorType type);
	};
}
