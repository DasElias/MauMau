#pragma once
#include "ConnectionToClient.h"
#include <boost/asio.hpp>
#include "ShellCommandParser.h"

namespace card {
	class ShellAcceptor {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const SERVER_PORT = 6766;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			boost::asio::io_context& ioc;
			boost::asio::ip::tcp::acceptor acceptor;
			ShellCommandParser& commandParser;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ShellAcceptor(boost::asio::io_context& ioc, ShellCommandParser& commandParser);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void acceptLoop();

	};
}