#pragma once
#include <boost/asio.hpp>
#include <string>

namespace card {
	namespace NetworkErrorMessageGenerator {
		std::string getMsg(const boost::system::error_code& ec);
	}
}