#include "ServerPacketTransmitter.h"
#include "Acceptor.h"
#include "RoomManager.h"
#include <memory>
#include "Daemon.h"
#include <shared/utils/Logger.h>

using namespace card;
namespace ba = boost::asio;
using ba::ip::tcp;
using boost::system::error_code;
using namespace std::chrono_literals;
using namespace std::string_literals;

int main() {
	initDaemon();
	initLogger("~/latest.log", "~/fatal.log");

	auto packetTransmitter = std::make_shared<ServerPacketTransmitter>();
	ba::io_context ioc;
	RoomManager roomManager(packetTransmitter);
	Acceptor acceptor(ioc, packetTransmitter, roomManager);

	ioc.run();
}