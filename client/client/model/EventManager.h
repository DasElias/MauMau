#pragma once
#include <algorithm>
#include <vector>
#include <functional>

#include "FunctionWrapper.h"
namespace card {
	template<class T>
	class EventManager {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::vector<FunctionWrapper<T>> eventHandlerList;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			EventManager() = default;
			EventManager(const EventManager&) = delete;
			EventManager& operator=(const EventManager&) = delete;


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			inline void addEventHandler(FunctionWrapper<T> eventHandler) {
				this->eventHandlerList.push_back(eventHandler);
			}
			inline void addEventHandler(std::function<void(T&)> eventHandler) {
				addEventHandler(FunctionWrapper<T>(eventHandler));
			}
			inline void removeEventHandler(FunctionWrapper<T> eventHandler) {
				eventHandlerList.erase(std::remove(eventHandlerList.begin(), eventHandlerList.end(), eventHandler), eventHandlerList.end());
			}
			inline void fireEvent(T& evt) {
				for(auto& handler : eventHandlerList) {
					handler(evt);
				}
			}
	};
}


