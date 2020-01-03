#pragma once
#include <shared/packet/CTSPacketTransmitter.h>
#include <boost/asio.hpp>
#include <shared/packet/GeneralTCPTransmitter.h>
#include "NetworkErrorHandler.h"

namespace card {
	class ClientPacketTransmitter : public CTSPacketTransmitter {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<GeneralTCPTransmitter> conn;
			NetworkErrorHandler& errorHandler;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ClientPacketTransmitter(std::shared_ptr<GeneralTCPTransmitter> conn, NetworkErrorHandler& errorHandler);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void sendPacketToServer(ClientToServerPacket& p);

		private:
			void onReceiveHandler(std::string& msg);
			void onErrorHandler(boost::system::error_code ec);


	};
}