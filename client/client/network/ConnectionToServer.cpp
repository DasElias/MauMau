#include "ConnectionToServer.h"
#include "ConnectionToServer.h"
#include "ConnectionToServer.h"

#include <thread>
#include <stdexcept>
#include <iostream>

namespace ba = boost::asio;
using ba::ip::tcp;
using boost::system::error_code;
using namespace std::chrono_literals;
using namespace std::string_literals;

namespace card {
	std::string const ConnectionToServer::HOST = "5.230.22.72";
	std::string const ConnectionToServer::PORT = "6767";

	ConnectionToServer::ConnectionToServer() :
			ioContext(),
			socket(ioContext) {

		connectSocket();
	}
	void ConnectionToServer::start() {
		GeneralTCPTransmitter::start();
		networkThread = std::make_unique<std::thread>([this] {
			try {
				ioContext.run();
			} catch(...) {
				std::cout << "Exception" << std::endl;
			}
		});
	}
	boost::asio::io_context& ConnectionToServer::getIoContext() {
		return ioContext;
	}
	tcp::socket& ConnectionToServer::getSocket() {
		return socket;
	}
	void ConnectionToServer::connectSocket() {
		tcp::resolver resolver(ioContext);
		tcp::resolver::query query(HOST, PORT);
		tcp::resolver::iterator iterator = resolver.resolve(query);
		tcp::resolver::iterator end;
		boost::system::error_code error = boost::asio::error::host_not_found;
		while(error && iterator != end) {
			socket.close();
			socket.connect(*iterator++, error);
		}
		if(error) {
			std::cout << error.message() << std::endl;
			throw boost::system::system_error(error);
		}
	}
	
}