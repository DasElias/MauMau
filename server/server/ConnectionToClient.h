#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <shared/packet/GeneralTCPTransmitter.h>

namespace ba = boost::asio;
using ba::ip::tcp;
using boost::system::error_code;
using namespace std::chrono_literals;
using namespace std::string_literals;

namespace card {
	class ConnectionToClient : public GeneralTCPTransmitter {
	// ----------------------------------------------------------------------
	// --------------------------------FIELDS--------------------------------
	// ----------------------------------------------------------------------
	private:
		boost::asio::io_context& ioc;
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
		boost::asio::io_context& getIoContext() override;
		tcp::socket& getSocket() override;
		void close() override;
	};
}