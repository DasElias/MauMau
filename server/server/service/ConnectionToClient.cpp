#include "ConnectionToClient.h"
#include "ServerPacketTransmitter.h"

namespace card {
	ConnectionToClient::ConnectionToClient(boost::asio::io_context& ioc, receiveFunc onReceiveFunc) :
			GeneralTCPTransmitterWithKeepalive(ioc, onReceiveFunc),
			socket(ioc) {
	}
	void ConnectionToClient::start() {
		GeneralTCPTransmitter::startImpl();
	}
	void ConnectionToClient::send(std::string message) {
		GeneralTCPTransmitter::send(message);
	}
	tcp::socket& ConnectionToClient::getSocket() {
		return socket;
	}
	void ConnectionToClient::close() {
		GeneralTCPTransmitterWithKeepalive::close();
		ioContext.post([this]() {
			socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
			socket.close();
		});
	}
}