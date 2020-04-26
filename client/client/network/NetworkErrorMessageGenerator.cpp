#include "NetworkErrorMessageGenerator.h"

namespace card {
	std::string NetworkErrorMessageGenerator::getMsg(const boost::system::error_code& ec) {
		switch(ec.value()) {
			case boost::asio::error::connection_refused:
				return "Der Server ist zurzeit nicht verf�gbar. Bitte versuche es sp�ter erneut.";
			case boost::asio::error::timed_out:
				return "Verbindung zum Server konnte nicht hergestellt werden. Bitte versuche es sp�ter erneut.";
			case boost::system::errc::no_link:
			case boost::asio::error::connection_aborted:
				return "Deine Internetverbindung ging verloren. Bitte versuche es sp�ter erneut.";
			default:
				return "Keine Internetverbindung. Bitte versuche es sp�ter erneut.";
		}
	}
}


