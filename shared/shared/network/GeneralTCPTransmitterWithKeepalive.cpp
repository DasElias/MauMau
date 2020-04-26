#include "GeneralTCPTransmitterWithKeepalive.h"	

namespace card {
	boost::posix_time::time_duration const GeneralTCPTransmitterWithKeepalive::SEND_DELAY = boost::posix_time::seconds(15);
	boost::posix_time::time_duration const GeneralTCPTransmitterWithKeepalive::RECEIVE_DELAY = boost::posix_time::seconds(25);
	std::string const GeneralTCPTransmitterWithKeepalive::HEALTH_PACKET_CHAR = "H";

	GeneralTCPTransmitterWithKeepalive::GeneralTCPTransmitterWithKeepalive(boost::asio::io_context& ioContext, receiveFunc onReceiveFunc) :
			GeneralTCPTransmitter(ioContext),
			sendTimer(ioContext),
			receiveTimer(ioContext),
			onReceiveFunc(onReceiveFunc) {

		// set initial deadlines
		sendTimer.expires_from_now(SEND_DELAY);
		receiveTimer.expires_from_now(RECEIVE_DELAY);

		checkSendDeadline();
		checkReceiveDeadline();
	}
	void GeneralTCPTransmitterWithKeepalive::setOnReceiveFunc(receiveFunc func) {
		this->onReceiveFunc = func;
	}
	void GeneralTCPTransmitterWithKeepalive::close() {
		wereTimersClosed = true;
		sendTimer.cancel();
		receiveTimer.cancel();
	}
	void GeneralTCPTransmitterWithKeepalive::onWrite() {
		sendTimer.expires_from_now(SEND_DELAY);
	}
	void GeneralTCPTransmitterWithKeepalive::onReceive(std::string& msg) {
		receiveTimer.expires_from_now(RECEIVE_DELAY);

		if(msg != HEALTH_PACKET_CHAR) {
			onReceiveFunc(msg);
		}
	}
	void GeneralTCPTransmitterWithKeepalive::checkSendDeadline() {
		if(wereTimersClosed) return;

		// check whether the current deadline has passed
		if(sendTimer.expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
			send(HEALTH_PACKET_CHAR);

			sendTimer.expires_from_now(SEND_DELAY);
		}

		sendTimer.async_wait(bind(&GeneralTCPTransmitterWithKeepalive::checkSendDeadline, this));
	}
	void GeneralTCPTransmitterWithKeepalive::checkReceiveDeadline() {
		if(wereTimersClosed) return;

		// check whether the current deadline has passed
		if(receiveTimer.expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
			// deadline has passed
			getSocket().close();

			// there is no longer an active deadline
			receiveTimer.expires_at(boost::posix_time::pos_infin);
		}

		receiveTimer.async_wait(bind(&GeneralTCPTransmitterWithKeepalive::checkReceiveDeadline, this));
	}
}