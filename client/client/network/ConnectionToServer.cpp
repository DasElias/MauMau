#include "ConnectionToServer.h"
#include <shared/utils/Logger.h>

#include <thread>
#include <stdexcept>

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
				log(LogSeverity::FATAL, "An unknown exception has happened during ioContext.run().");
			}
		});
	}
	boost::asio::io_context& ConnectionToServer::getIoContext() {
		return ioContext;
	}
	tcp::socket& ConnectionToServer::getSocket() {
		return socket;
	}
	void ConnectionToServer::close() {
		ioContext.post([this]() {
			socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
			socket.close();
		});
		networkThread->join();
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
			log(LogSeverity::ERR, "Error during trying to establish connection to server:");
			log(LogSeverity::ERR, error.message() + "(" + std::to_string(error.value()) + ")");
			throw boost::system::system_error(error);
		}
	}
	
}