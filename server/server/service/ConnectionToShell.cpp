#include "ConnectionToShell.h"
#include "ServerPacketTransmitter.h"

namespace card {
	ConnectionToShell::ConnectionToShell(boost::asio::io_context& ioc, receiveFunc onReceiveFunc) :
			GeneralTCPTransmitter(ioc),
			onReceiveFunc(onReceiveFunc),
			socket(ioc) {
	}
	void ConnectionToShell::start() {
		GeneralTCPTransmitter::startImpl();
	}
	void ConnectionToShell::send(std::string message) {
		GeneralTCPTransmitter::send(message);
	}
	tcp::socket& ConnectionToShell::getSocket() {
		return socket;
	}
	void ConnectionToShell::close() {
		ioContext.post([this]() {
			socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
			socket.close();
		});
	}
	void ConnectionToShell::setOnReceiveFunc(receiveFunc func) {
		this->onReceiveFunc = func;
	}
	void ConnectionToShell::onReceive(std::string& msg) {
		this->onReceiveFunc(msg);
	}
}