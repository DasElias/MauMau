#pragma once
#include "../packet/GeneralTCPTransmitter.h"
#include <boost/asio.hpp>

namespace card {
    typedef std::function<void(std::string&)> receiveFunc;

	class GeneralTCPTransmitterWithKeepalive : public GeneralTCPTransmitter {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
        private:
            static boost::posix_time::time_duration const SEND_DELAY;
            static boost::posix_time::time_duration const RECEIVE_DELAY;
            static std::string const HEALTH_PACKET_CHAR;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            boost::asio::deadline_timer sendTimer;
            boost::asio::deadline_timer receiveTimer;
            bool wereTimersClosed = false;

            receiveFunc onReceiveFunc;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            GeneralTCPTransmitterWithKeepalive(boost::asio::io_context& ioContext, receiveFunc onReceiveFunc = [](std::string&){});
            GeneralTCPTransmitterWithKeepalive(const GeneralTCPTransmitterWithKeepalive&) = delete;

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void setOnReceiveFunc(receiveFunc func);
            void close() override;

        private:
            void onWrite() override final;
            void onReceive(std::string& msg) override final;

            void checkSendDeadline();
            void checkReceiveDeadline();

	};
}