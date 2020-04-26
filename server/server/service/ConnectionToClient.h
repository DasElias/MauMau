#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <shared/network/GeneralTCPTransmitterWithKeepalive.h>
#include "AbstractConnectionToClient.h"

namespace ba = boost::asio;
using ba::ip::tcp;
using boost::system::error_code;
using namespace std::chrono_literals;
using namespace std::string_literals;

namespace card {
	class ConnectionToClient : public GeneralTCPTransmitterWithKeepalive, public AbstractConnectionToClient {
	// ----------------------------------------------------------------------
	// --------------------------------FIELDS--------------------------------
	// ----------------------------------------------------------------------
	private:
		tcp::socket socket;

	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	public:
		ConnectionToClient(boost::asio::io_context& ioc, receiveFunc onReceiveFunc = [](std::string& jsonMsg){});

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	public:
		void start();
		void send(std::string message);
		tcp::socket& getSocket() override;
		void close() override;
	};
}