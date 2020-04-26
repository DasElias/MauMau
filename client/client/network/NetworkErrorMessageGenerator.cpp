#include "NetworkErrorMessageGenerator.h"

namespace card {
	std::string NetworkErrorMessageGenerator::getMsg(const boost::system::error_code& ec) {
		switch(ec.value()) {
			case boost::asio::error::connection_refused:
				return "Der Server ist zurzeit nicht verfügbar. Bitte versuche es später erneut.";
			case boost::asio::error::timed_out:
				return "Verbindung zum Server konnte nicht hergestellt werden. Bitte versuche es später erneut.";
			case boost::system::errc::no_link:
			case boost::asio::error::connection_aborted:
				return "Deine Internetverbindung ging verloren. Bitte versuche es später erneut.";
			default:
				return "Keine Internetverbindung. Bitte versuche es später erneut.";
		}
	}
}


