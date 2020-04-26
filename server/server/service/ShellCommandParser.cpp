#include "ShellCommandParser.h"
#include "ThreadSynchronizer.h"

namespace card {
	ShellCommandParser::ShellCommandParser(const RoomManagerAccessorFromClient& roomManager, boost::asio::io_context& ioContext) :
			roomManager(roomManager),
			ioContext(ioContext) {
	}
	void ShellCommandParser::parse(ConnectionToShell& conn, std::string cmdMessage) {
		if(cmdMessage == "amountOfPlayers") conn.send(amountOfPlayers());
		else if(cmdMessage == "amountOfRooms") conn.send(amountOfRooms());
		else if(cmdMessage == "shutdown") conn.send(shutdown());
		else conn.send(help());
	}
	std::string ShellCommandParser::help() {
		return "AVAILABLE COMMANDS: amountOfPlayers, amountOfRooms, shutdown";
	}
	std::string ShellCommandParser::amountOfPlayers() {
		std::lock_guard lock(threadSyncronizer_getMutex());
		return std::to_string(roomManager.getAmountOfUsersInRooms());
	}
	std::string ShellCommandParser::amountOfRooms() {
		std::lock_guard lock(threadSyncronizer_getMutex());
		return std::to_string(roomManager.getAmountOfRooms());
	}
	std::string ShellCommandParser::shutdown() {
		ioContext.stop();
		return "Server was shut down";
	}
}