#include "BlockingConnectionToServer.h"
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

using boost::lambda::bind;
using boost::lambda::var;
using boost::lambda::_1;

namespace card {
	BlockingConnectionToServer::BlockingConnectionToServer() :
			socket(ioContext),
			deadline(ioContext) {

		// no deadline is required until the first socket operation is started
		deadline.expires_at(boost::posix_time::pos_infin);

		checkDeadline();
		
	}
	void BlockingConnectionToServer::connect(std::string host, std::string port, boost::posix_time::time_duration timeout) {
		tcp::resolver::query query(host, port);
		tcp::resolver::iterator iter = tcp::resolver(ioContext).resolve(query);

		// set deadline
		deadline.expires_from_now(timeout);

		boost::system::error_code ec;
		for(; iter != tcp::resolver::iterator(); ++iter) {
			socket.close();

			ec = boost::asio::error::would_block;

			// start the asyncronous operation
			socket.async_connect(iter->endpoint(), var(ec) = _1);

			// block until the async operation has completed
			do {
				ioContext.run_one();
			} while(ec == boost::asio::error::would_block);

			// determine wheter a connection was established successfully
			if(! ec && socket.is_open()) return;
		}

		throw boost::system::system_error(ec ? ec : boost::asio::error::host_not_found);
	}
	std::string BlockingConnectionToServer::readLine(boost::posix_time::time_duration timeout) {
		// set deadline
		deadline.expires_from_now(timeout);

		boost::system::error_code ec = boost::asio::error::would_block;

		// start the asyncrounous operation
		boost::asio::async_read_until(socket, inputBuffer, '\n', var(ec) = _1);

		// block until the async operation has completed
		do {
			ioContext.run_one();
		} while(ec == boost::asio::error::would_block);

		if(ec) throw boost::system::system_error(ec);
		std::string line;
		std::istream is(&inputBuffer);
		std::getline(is, line);
		return line;

	}
	void BlockingConnectionToServer::writeLine(const std::string& msg, boost::posix_time::time_duration timeout) {
		std::string data = msg + "\n";

		// set deadline
		deadline.expires_from_now(timeout);

		boost::system::error_code ec = boost::asio::error::would_block;

		// start the asyncrounous operation
		boost::asio::async_write(socket, boost::asio::buffer(data), [&ec](boost::system::error_code e, std::size_t n) {
			ec = e;
		});

		// block until the async operation has completed
		do {
			ioContext.run_one();
		} while(ec == boost::asio::error::would_block);

		if(ec) throw boost::system::system_error(ec);
	}
	void BlockingConnectionToServer::checkDeadline() {
		// check whether the current deadline has passed
		if(deadline.expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
			// deadline has passed
			socket.close();

			// there is no longer an active deadline
			deadline.expires_at(boost::posix_time::pos_infin);
		}

		deadline.async_wait(bind(&BlockingConnectionToServer::checkDeadline, this));
	}
}