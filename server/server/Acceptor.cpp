#include "Acceptor.h"
#include "ServerPacketTransmitter.h"
#include <shared/utils/Logger.h>

namespace ba = boost::asio;
using ba::ip::tcp;
using boost::system::error_code;
using namespace std::chrono_literals;
using namespace std::string_literals;

namespace card {
	Acceptor::Acceptor(boost::asio::io_context& ioc, std::shared_ptr<ServerPacketTransmitter> packetTransmitter, RoomManager& roomManager) :
			ioc(ioc),
			acceptor(ioc, tcp::v4()),
			packetTransmitter(packetTransmitter),
			roomManager(roomManager) {

		acceptor.bind({{}, SERVER_PORT});
		acceptor.set_option(tcp::acceptor::reuse_address());
		acceptor.listen();

		acceptLoop();
	}
	void Acceptor::acceptLoop() {
		std::shared_ptr<ConnectionToClient> session = std::make_shared<ConnectionToClient>(ioc);
		session->setOnReceiveFunc([this, session](std::string& jsonMsg) {
			packetTransmitter->onReceive(jsonMsg, session);
		});
		session->setOnErrorFunc([this, session](boost::system::error_code ec) {
			roomManager.leave(session);
			log(LogSeverity::INFO, "Client has disconnected with error message: " + ec.message());
		});
		acceptor.async_accept(session->getSocket(), [this, session](error_code ec) {
			session->start();
			if(!ec) acceptLoop();
		});
	}
}