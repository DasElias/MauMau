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

	try {	
		auto packetTransmitter = std::make_shared<ServerPacketTransmitter>();
		ba::io_context ioc;
		RoomManager roomManager(packetTransmitter);
		Acceptor acceptor(ioc, packetTransmitter, roomManager);

		ioc.run();
	} catch(boost::system::system_error e) {
		log(LogSeverity::FATAL, std::string(e.what()) + "(" + std::to_string(e.code().value()) + ")");
	} catch(std::exception e) {
		log(LogSeverity::FATAL, e.what());
	} catch(...) {
		log(LogSeverity::FATAL, "An unknown exception has happened during network initializing.");
	}
	

}