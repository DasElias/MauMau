#pragma once
#include "ConnectionToClient.h"
#include "RoomManagerAccessorFromClient.h"


namespace card {
	class ShellCommandParser {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			const RoomManagerAccessorFromClient& roomManager;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ShellCommandParser(const RoomManagerAccessorFromClient& roomManager);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void parse(ConnectionToClient& conn, std::string cmdMessage);

		private:
			std::string help();
			std::string amountOfPlayers();
			std::string amountOfRooms();

	};
}