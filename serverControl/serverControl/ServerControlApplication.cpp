#include "BlockingConnectionToServer.h"
#include <iostream>

using namespace card;

static std::string const HOST = "127.0.0.1";
static std::string const PORT = "6766";
static boost::posix_time::time_duration const TIMEOUT = boost::posix_time::seconds(10);

int main(int argc, const char* argv[]) {
	std::string argument = ((argc != 2) ? "help" : argv[1]);

	BlockingConnectionToServer conn;
	try {
		conn.connect(HOST, PORT, TIMEOUT);
	} catch(const boost::system::system_error& err) {
		std::cout << "Oops! Something went wrong! Couldn't establish connection to server." << std::endl;
		return -1;
	}

	try {
		conn.writeLine(argument, TIMEOUT);
		std::string response = conn.readLine(TIMEOUT);
		std::cout << response << std::endl;
	} catch(const boost::system::system_error& err) {
		std::cout << "Oops! Something went wrong!" << std::endl;
		return -1;
	}

	return 0;
}