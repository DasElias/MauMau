#include "ServerPacketTransmitter.h"
#include "Acceptor.h"
#include "RoomManager.h"
#include <memory>
#include "Daemon.h"
#include <shared/utils/Logger.h>
#include <shared/utils/ThreadUtils.h>
#include "ThreadSynchronizer.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	// windows
	#define LATEST_LOG_FLIE "C:\\latest.log"
	#define FATAL_LOG_FILE "C:\\fatal.log"
#else
	#define LATEST_LOG_FILE "~/latest.log"
	#define FATAL_LOG_FILE "~/fatal.log"
#endif

using namespace card;
namespace ba = boost::asio;
using ba::ip::tcp;
using boost::system::error_code;
using namespace std::chrono_literals;
using namespace std::string_literals;

void runThreadUtils() {
	while(true) {
		{
			std::mutex& mutex = threadSyncronizer_getMutex();
			std::lock_guard<std::mutex> lockGuard(mutex);

			threadUtils_update();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}	
}

int main() {
 	initDaemon();
	initLogger(LATEST_LOG_FLIE, FATAL_LOG_FILE);


	try {	
		auto packetTransmitter = std::make_shared<ServerPacketTransmitter>();
		ba::io_context ioc;
		RoomManager roomManager(packetTransmitter);
		Acceptor acceptor(ioc, packetTransmitter, roomManager);

		std::thread threadUtilsThread(&runThreadUtils);

		ioc.run();
	} catch(boost::system::system_error e) {
		log(LogSeverity::FATAL, std::string(e.what()) + "(" + std::to_string(e.code().value()) + ")");
	} catch(std::exception e) {
		log(LogSeverity::FATAL, e.what());
	} catch(...) {
		log(LogSeverity::FATAL, "An unknown exception has happened during network initializing.");
	}
	

}