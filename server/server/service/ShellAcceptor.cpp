#include "ShellAcceptor.h"
#include <shared/utils/Logger.h>

namespace card {
	ShellAcceptor::ShellAcceptor(boost::asio::io_context& ioc, ShellCommandParser& commandParser) :
			ioc(ioc),
			acceptor(ioc, tcp::v4()),
			commandParser(commandParser) {

		acceptor.bind({{}, SERVER_PORT});
		acceptor.set_option(tcp::acceptor::reuse_address());
		acceptor.listen();

		acceptLoop();
	}

	void ShellAcceptor::acceptLoop() {
		std::shared_ptr<ConnectionToClient> session = std::make_shared<ConnectionToClient>(ioc);
		session->setOnReceiveFunc([this, session](std::string& jsonMsg) {
			commandParser.parse(*session, jsonMsg);
		});
		session->setOnErrorFunc([this, session](boost::system::error_code ec) {
			log(LogSeverity::INFO, "Shell client has disconnected with error message: " + ec.message());
		});
		acceptor.async_accept(session->getSocket(), [this, session](error_code ec) {
			session->start();
			if(!ec) acceptLoop();
		});
	}

}