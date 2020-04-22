#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace card {
	class BlockingConnectionToServer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            boost::asio::io_context ioContext;
            tcp::socket socket;
            boost::asio::deadline_timer deadline;
            boost::asio::streambuf inputBuffer;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            BlockingConnectionToServer();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void connect(std::string host, std::string port, boost::posix_time::time_duration timeout);
            std::string readLine(boost::posix_time::time_duration timeout);
            void writeLine(const std::string& msg, boost::posix_time::time_duration timeout);

        private:
            void checkDeadline();

	};
}