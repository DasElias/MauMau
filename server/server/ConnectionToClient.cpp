#include "ConnectionToClient.h"
#include "ServerPacketTransmitter.h"

namespace card {
	ConnectionToClient::ConnectionToClient(boost::asio::io_context& ioc, receiveFunc onReceiveFunc) :
			GeneralTCPTransmitter(onReceiveFunc),
			ioc(ioc),
			socket(ioc) {
	}
	boost::asio::io_context& ConnectionToClient::getIoContext() {
		return ioc;
	}
	tcp::socket& ConnectionToClient::getSocket() {
		return socket;
	}
}