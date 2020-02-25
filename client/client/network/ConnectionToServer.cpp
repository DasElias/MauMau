#include "ConnectionToServer.h"
#include <shared/utils/Logger.h>

#include <thread>
#include <stdexcept>
#include <boost/bind.hpp>

namespace ba = boost::asio;
using ba::ip::tcp;
using boost::system::error_code;
using namespace std::chrono_literals;
using namespace std::string_literals;

namespace card {
	std::string const ConnectionToServer::HOST = "localhost";
	std::string const ConnectionToServer::PORT = "6767";

	ConnectionToServer::ConnectionToServer() :
			ioContext(),
			socket(ioContext),
			resolver(ioContext) {

		
	}
	void ConnectionToServer::start() {
		connectSocket();
		networkThread = std::make_unique<std::thread>([this] {
			try {
				ioContext.run();
			} catch(std::exception& e) {
				log(LogSeverity::FATAL, "An exception ("+std::string(e.what())+") has happened during ioContext.run().");
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
		tcp::resolver::query query(HOST, PORT);
		resolver.async_resolve(query, [this](const boost::system::error_code& ec, tcp::resolver::iterator endpointIterator) {
			if(!ec) {
				tcp::endpoint endpoint = *endpointIterator;
				socket.async_connect(endpoint, boost::bind(&ConnectionToServer::connectionCallback, this, boost::asio::placeholders::error, ++endpointIterator));
			} else {
				log(LogSeverity::ERR, "Error during trying to establish connection to server:");
				log(LogSeverity::ERR, ec.message() + "(" + std::to_string(ec.value()) + ")");
				throw boost::system::system_error(ec);
			}
		});
		
	}

	void ConnectionToServer::connectionCallback(const boost::system::error_code& ec, tcp::resolver::iterator endpointIterator) {
		if(! ec) {
			GeneralTCPTransmitter::startImpl();
		} else if(endpointIterator != tcp::resolver::iterator()) {
			// try next endpoint
			socket.close();
			tcp::endpoint endpoint = *endpointIterator;
			socket.async_connect(endpoint, boost::bind(&ConnectionToServer::connectionCallback, this, boost::asio::placeholders::error, ++endpointIterator));
		} else {
			log(LogSeverity::ERR, "Error during trying to establish connection to server:");
			log(LogSeverity::ERR, ec.message() + "(" + std::to_string(ec.value()) + ")");
			throw boost::system::system_error(ec);
		}
	}	
}