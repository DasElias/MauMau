#pragma once
#include <shared/packet/GeneralTCPTransmitter.h>

namespace card {
	class ConnectionToServer : public GeneralTCPTransmitter {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::string const HOST;
			static std::string const PORT;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			boost::asio::io_context ioContext;
			tcp::socket socket;

			std::unique_ptr<std::thread> networkThread;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ConnectionToServer();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void start();
			boost::asio::io_context& getIoContext() override;
			tcp::socket& getSocket() override;

		private:
			void connectSocket();

	};
}