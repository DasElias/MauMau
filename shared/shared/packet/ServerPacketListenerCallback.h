#pragma once

#include <functional>
#include "Packet.h"

namespace card {
	class ServerPacketListenerCallback {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int identity;
			std::function<void(Packet&)> function;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ServerPacketListenerCallback(std::function<void(Packet&)> function) :
				function(function) {
				static int id = 0;
				identity = id++;
			};

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			inline bool operator==(const ServerPacketListenerCallback& other) const {
				return identity == other.identity;
			};
			inline void operator()(Packet& event) {
				function(event);
			};

	};
}


