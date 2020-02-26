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
			tcp::resolver resolver;

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
			void start(std::function<void(boost::system::system_error)> errorCallback);
			boost::asio::io_context& getIoContext() override;
			tcp::socket& getSocket() override;
			void close() override;

		private:
			void connectSocket();
			void connectionCallback(const boost::system::error_code& ec, tcp::resolver::iterator endpointIterator);

	};
}