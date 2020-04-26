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
	class ConnectionToShell : public GeneralTCPTransmitter {
		typedef std::function<void(std::string&)> receiveFunc;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			tcp::socket socket;
			receiveFunc onReceiveFunc;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ConnectionToShell(boost::asio::io_context& ioc, receiveFunc onReceiveFunc = [](std::string& jsonMsg){});

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void start();
			void send(std::string message);
			tcp::socket& getSocket() override;
			void close() override;

			void setOnReceiveFunc(receiveFunc func);


		private:
			void onReceive(std::string& msg);
		};
}