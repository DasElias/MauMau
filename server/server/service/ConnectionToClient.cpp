#include "ConnectionToClient.h"
#include "ServerPacketTransmitter.h"

namespace card {
	ConnectionToClient::ConnectionToClient(boost::asio::io_context& ioc, receiveFunc onReceiveFunc) :
			GeneralTCPTransmitter(onReceiveFunc),
			ioc(ioc),
			socket(ioc) {
	}
	void ConnectionToClient::start() {
		GeneralTCPTransmitter::startImpl();
	}
	void ConnectionToClient::send(std::string message) {
		GeneralTCPTransmitter::send(message);
	}
	boost::asio::io_context& ConnectionToClient::getIoContext() {
		return ioc;
	}
	tcp::socket& ConnectionToClient::getSocket() {
		return socket;
	}
	void ConnectionToClient::close() {
		ioc.post([this]() {
			socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
			socket.close();
		});
	}
}