#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <list>
#include "../utils/ReferenceOrValueWrapper.h"
using boost::asio::ip::tcp;

namespace card {
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
		protected:
			boost::asio::io_context& ioContext;

		private:
			boost::asio::streambuf rx;
			std::list<std::string> tx;
			bool wasConnectionEtablished = false;

			errorHandlingFunc onErrorFunc;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			GeneralTCPTransmitter(boost::asio::io_context& ioContext);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void send(std::string msg, bool atFront = false);
			virtual tcp::socket& getSocket() =0;
			void setOnErrorFunc(errorHandlingFunc callback);
			virtual void close() =0;

		protected:
			void startImpl();

		private:
			bool enqueue(std::string message, bool atFront);
			bool dequeue();
			void writeLoop();
			void readLoop();

			void filterDelimiterFromMsg(std::string& msg);

			virtual void onWrite();
			virtual void onReceive(std::string& msg) =0;
	};
}