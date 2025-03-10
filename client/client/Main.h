#pragma once
#include "state/StateManager.h"



namespace card {
	class Main {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const START_WIDTH = 1800;
			static int const START_HEIGHT = 800;
			static int const VSYNC = 1;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			GLFWwindow* p_window;
			StateManager stateManager;
			float fps = 0;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Main();
			~Main();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void setWindowIcon();
			void loop();
	};
}
