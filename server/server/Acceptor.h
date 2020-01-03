#pragma once
#include "ConnectionToClient.h"
#include <boost/asio.hpp>
#include "ServerPacketTransmitter.h"
#include "RoomManager.h"

namespace card {
	class Acceptor {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const SERVER_PORT = 6767;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			boost::asio::io_context& ioc;
			boost::asio::ip::tcp::acceptor acceptor;
			std::shared_ptr<ServerPacketTransmitter> packetTransmitter;
			RoomManager& roomManager;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Acceptor(boost::asio::io_context& ioc, std::shared_ptr<ServerPacketTransmitter> packetTransmitter, RoomManager& roomManager);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void acceptLoop();

	};
}