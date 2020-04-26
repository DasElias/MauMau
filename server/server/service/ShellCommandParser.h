#pragma once
#include "ConnectionToShell.h"
#include "RoomManagerAccessorFromClient.h"


namespace card {
	class ShellCommandParser {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			const RoomManagerAccessorFromClient& roomManager;
			boost::asio::io_context& ioContext;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ShellCommandParser(const RoomManagerAccessorFromClient& roomManager, boost::asio::io_context& ioContext);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void parse(ConnectionToShell& conn, std::string cmdMessage);

		private:
			std::string help();
			std::string amountOfPlayers();
			std::string amountOfRooms();
			std::string shutdown();

	};
}