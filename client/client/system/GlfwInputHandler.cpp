#include "GlfwInputHandler.h"
#include <egui/model/events/ScrollEvent.h>

#include <map>
#include <stdexcept>
#include <shared/utils/Logger.h>

namespace card {
	namespace {
		static std::map<GLFWwindow*, GlfwInputHandler*> instanceMap;

		void mouseBtnCallback(GLFWwindow* window, int button, int action, int mods) {
			egui::MouseEvent evt = { button, action == GLFW_PRESS , int(egui::getCursorHandler().getCursorX()), int(egui::getCursorHandler().getCursorY())};
			instanceMap.at(window)->getMouseBtnEventManager().fireEvent(evt);
		}

		void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
			if (yoffset != 0) {
				egui::ScrollEvent evt = { float(yoffset) };
				instanceMap.at(window)->getScrollEventManager().fireEvent(evt);
			}
		}

		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			std::string keyName = instanceMap.at(window)->getKeyName(key, scancode);
			std::string keyChar = instanceMap.at(window)->getKeyChar(key, scancode);

			egui::KeyEventType type = egui::KeyEventType::RELEASE;
			if(action == GLFW_PRESS) type = egui::KeyEventType::PRESS;
			else if(action == GLFW_REPEAT) type = egui::KeyEventType::REPEAT; 
			egui::KeyEvent evt(key, keyName, keyChar, scancode, type);
			
			instanceMap.at(window)->getKeyEventManager().fireEvent(evt);
		}

		void charCallback(GLFWwindow* window, unsigned int codepoint) {
			egui::CharEnterEvent evt(codepoint);

			instanceMap.at(window)->getCharEnteredEventManager().fireEvent(evt);
		}
	}


	GlfwInputHandler::GlfwInputHandler(GLFWwindow* window) :
			window(window) {
		
		instanceMap.insert(std::make_pair(window, this));
		glfwSetScrollCallback(window, scrollCallback);
		glfwSetMouseButtonCallback(window, mouseBtnCallback);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetCharCallback(window, charCallback);
	}

	GlfwInputHandler::~GlfwInputHandler() {
		glfwSetScrollCallback(window, nullptr);
		glfwSetMouseButtonCallback(window, nullptr);
		
		instanceMap.erase(window);
	}



	bool GlfwInputHandler::isKeyDown(int key) {
		return ((key < 10) ? glfwGetMouseButton(window, key) : glfwGetKey(window, key)) == GLFW_PRESS;
	}

	void GlfwInputHandler::setCursorInputMode(int cursorMode) {
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);
	}

	std::string GlfwInputHandler::getKeyName(int key, int scanCode) {
		// filter non-printable keys
		switch(key) {
			case KEY_SPACE:
				return "SPACE";
			case KEY_ENTER:
				return "ENTER";
			case KEY_ESCAPE:
				return "ESCAPE";
			case KEY_TAB:
				return "TAB";
			case KEY_BACKSPACE:
				return "BACKSPACE";
			case KEY_INSERT:
				return "INSERT";
			case KEY_DELETE:
				return "DELETE";
			case KEY_RIGHT:
				return "RIGHT";
			case KEY_LEFT:
				return "LEFT";
			case KEY_DOWN:
				return "DOWN";
			case KEY_UP:
				return "UP";
			case KEY_PAGE_UP:
				return "PAGE_UP";
			case KEY_PAGE_DOWN:
				return "PAGE_DOWN";
			case KEY_HOME:
				return "HOME";
			case KEY_END:
				return "END";
			case KEY_CAPS_LOCK:
				return "CAPS_LOCK";
			case KEY_SCROLL_LOCK:
				return "SCROLL_LOCK";
			case KEY_NUM_LOCK:
				return "NUM_LOCK";
			case KEY_PRINT_SCREEN:
				return "PRINT_SCREEN";
			case KEY_PAUSE:
				return "PAUSE";
			case KEY_F1:
				return "F1";
			case KEY_F2:
				return "F2";
			case KEY_F3:
				return "F3";
			case KEY_F4:
				return "F4";
			case KEY_F5:
				return "F5";
			case KEY_F6:
				return "F6";
			case KEY_F7:
				return "F7";
			case KEY_F8:
				return "F8";
			case KEY_F9:
				return "F9";
			case KEY_F10:
				return "F10";
			case KEY_F11:
				return "F11";
			case KEY_F12:
				return "F12";
			case KEY_F13:
				return "F13";
			case KEY_F14:
				return "F14";
			case KEY_F15:
				return "F15";
			case KEY_F16:
				return "F16";
			case KEY_F17:
				return "F17";
			case KEY_F18:
				return "F18";
			case KEY_F19:
				return "F19";
			case KEY_F21:
				return "F21";
			case KEY_F22:
				return "F22";
			case KEY_F23:
				return "F23";
			case KEY_F24:
				return "F24";
			case KEY_F25:
				return "F25";
			case KEY_KP_1:
				return "KEYPAD 1";
			case KEY_KP_2:
				return "KEYPAD 2";
			case KEY_KP_3:
				return "KEYPAD 3";
			case KEY_KP_4:
				return "KEYPAD 4";
			case KEY_KP_5:
				return "KEYPAD 5";
			case KEY_KP_6:
				return "KEYPAD 6";
			case KEY_KP_7:
				return "KEYPAD 7";
			case KEY_KP_8:
				return "KEYPAD 8";
			case KEY_KP_9:
				return "KEYPAD 9";
			case KEY_KP_DECIMAL:
				return "KEYPAD DECIMAL";
			case KEY_KP_DIVIDE:
				return "KEYPAD DIVIDE";
			case KEY_KP_MULTIPLY:
				return "KEYPAD MULTIPLY";
			case KEY_KP_SUBTRACT:
				return "KEYPAD SUBTRACT";
			case KEY_KP_ADD:
				return "KEYPAD ADD";
			case KEY_KP_ENTER:
				return "KEYPAD ENTER";
			case KEY_KP_EQUAL:
				return "KEYPAD EQUAL";
			case KEY_LEFT_SHIFT:
				return "LEFT SHIFT";
			case KEY_LEFT_CONTROL:
				return "LEFT CTRL";
			case KEY_LEFT_ALT:
				return "LEFT ALT";
			case KEY_LEFT_SUPER:
				return "LEFT SUPER";
			case KEY_RIGHT_SHIFT:
				return "RIGHT SHIFT";
			case KEY_RIGHT_CONTROL:
				return "RIGHT CTRL";
			case KEY_RIGHT_ALT:
				return "RIGHT ALT";
			case KEY_RIGHT_SUPER:
				return "RIGHT SUPER";
			case KEY_MENU:
				return "MENU";
			case KEY_UNKNOWN:
				return "UNKNOWN KEY (" + std::to_string(key) + ")";
			default:
				{
					const char* stringPtr = glfwGetKeyName(key, scanCode);
					if(stringPtr == nullptr) return "UNKNOWN KEY! " + key;
					return std::string(stringPtr);
				}
		}
	}

	std::string GlfwInputHandler::getKeyChar(int key, int scanCode) {
		// filter non-printable keys
		switch(key) {
			case KEY_SPACE:
				return " ";
			case KEY_ENTER:
			case KEY_ESCAPE:
			case KEY_TAB:
			case KEY_BACKSPACE:
			case KEY_INSERT:
			case KEY_DELETE:
			case KEY_RIGHT:
			case KEY_LEFT:
			case KEY_DOWN:
			case KEY_UP:
			case KEY_PAGE_UP:
			case KEY_PAGE_DOWN:
			case KEY_HOME:
			case KEY_END:
			case KEY_CAPS_LOCK:
			case KEY_SCROLL_LOCK:
			case KEY_NUM_LOCK:
			case KEY_PRINT_SCREEN:
			case KEY_PAUSE:
			case KEY_F1:
			case KEY_F2:
			case KEY_F3:
			case KEY_F4:
			case KEY_F5:
			case KEY_F6:
			case KEY_F7:
			case KEY_F8:
			case KEY_F9:
			case KEY_F10:
			case KEY_F11:
			case KEY_F12:
			case KEY_F13:
			case KEY_F14:
			case KEY_F15:
			case KEY_F16:
			case KEY_F17:
			case KEY_F18:
			case KEY_F19:
			case KEY_F21:
			case KEY_F22:
			case KEY_F23:
			case KEY_F24:
			case KEY_F25:
			case KEY_KP_DECIMAL:
			case KEY_KP_DIVIDE:
			case KEY_KP_MULTIPLY:
			case KEY_KP_SUBTRACT:
			case KEY_KP_ADD:
			case KEY_KP_ENTER:
			case KEY_KP_EQUAL:
			case KEY_LEFT_SHIFT:
			case KEY_LEFT_CONTROL:
			case KEY_LEFT_ALT:
			case KEY_LEFT_SUPER:
			case KEY_RIGHT_SHIFT:
			case KEY_RIGHT_CONTROL:
			case KEY_RIGHT_ALT:
			case KEY_RIGHT_SUPER:
			case KEY_MENU:
				return "";
			case KEY_KP_1:
				return "1";
			case KEY_KP_2:
				return "2";
			case KEY_KP_3:
				return "3";
			case KEY_KP_4:
				return "4";
			case KEY_KP_5:
				return "5";
			case KEY_KP_6:
				return "6";
			case KEY_KP_7:
				return "7";
			case KEY_KP_8:
				return "8";
			case KEY_KP_9:
				return "9";		
			case KEY_UNKNOWN:
				return "";
			default:
				{
					const char* stringPtr = glfwGetKeyName(key, scanCode);
					if(stringPtr == nullptr) return "";
					return std::string(stringPtr);
				}
		}


	}

	egui::EventManager<egui::ScrollEvent>& GlfwInputHandler::getScrollEventManager() {
		return scrollEventManager;
	}

	egui::EventManager<egui::MouseEvent>& GlfwInputHandler::getMouseBtnEventManager() {
		return mouseBtnEventManager;
	}

	egui::EventManager<egui::KeyEvent>& GlfwInputHandler::getKeyEventManager() {
		return keyEventManager;
	}

	egui::EventManager<egui::CharEnterEvent>& GlfwInputHandler::getCharEnteredEventManager() {
		return charEnteredEventManager;
	}

}
