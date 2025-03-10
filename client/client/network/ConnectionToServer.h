#pragma once
#include <shared/network/GeneralTCPTransmitterWithKeepalive.h>

namespace card {
	class ConnectionToServer : public GeneralTCPTransmitterWithKeepalive {
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
			tcp::socket socket;
			tcp::resolver resolver;

			std::unique_ptr<std::thread> networkThread;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ConnectionToServer(boost::asio::io_context& ioContext);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void start(std::function<void(boost::system::system_error)> errorCallback);
			tcp::socket& getSocket() override;
			void close() override;

		private:
			void connectSocket();
			void connectionCallback(const boost::system::error_code& ec, tcp::resolver::iterator endpointIterator);

	};
}