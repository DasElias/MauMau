#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <list>
#include "../utils/ReferenceOrValueWrapper.h"
using boost::asio::ip::tcp;

namespace card {
	typedef std::function<void(std::string&)> receiveFunc;
	typedef std::function<void(boost::system::error_code)> errorHandlingFunc;

	class GeneralTCPTransmitter : public std::enable_shared_from_this<GeneralTCPTransmitter> {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static char const DELIMITER;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			boost::asio::streambuf rx;
			std::list<std::string> tx;

			receiveFunc onReceiveFunc;
			errorHandlingFunc onErrorFunc;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			GeneralTCPTransmitter(receiveFunc onReceiveFunc = [](std::string&) {});

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void start();
			void send(std::string msg, bool atFront = false);
			virtual boost::asio::io_context& getIoContext() =0;
			virtual tcp::socket& getSocket() =0;
			void setOnReceiveFunc(receiveFunc callback);
			void setOnErrorFunc(errorHandlingFunc callback);

		private:
			bool enqueue(std::string message, bool atFront);
			bool dequeue();
			void writeLoop();
			void readLoop();

			void filterDelimiterFromMsg(std::string& msg);

	};
}