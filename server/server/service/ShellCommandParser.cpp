#include "ShellCommandParser.h"
#include "ThreadSynchronizer.h"

namespace card {
	ShellCommandParser::ShellCommandParser(const RoomManagerAccessorFromClient& roomManager) :
			roomManager(roomManager) {
	}
	void ShellCommandParser::parse(ConnectionToClient& conn, std::string cmdMessage) {
		if(cmdMessage == "amountOfPlayers") conn.send(amountOfPlayers());
		else if(cmdMessage == "amountOfRooms") conn.send(amountOfRooms());
		else conn.send(help());
	}
	std::string ShellCommandParser::help() {
		return "AVAILABLE COMMANDS: amountOfPlayers, amountOfRooms";
	}
	std::string ShellCommandParser::amountOfPlayers() {
		std::lock_guard lock(threadSyncronizer_getMutex());
		return std::to_string(roomManager.getAmountOfUsersInRooms());
	}
	std::string ShellCommandParser::amountOfRooms() {
		std::lock_guard lock(threadSyncronizer_getMutex());
		return std::to_string(roomManager.getAmountOfRooms());
	}
}