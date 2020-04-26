#include "GeneralTCPTransmitter.h"
#include <thread>
#include <stdexcept>
#include <iostream>
#include "../utils/Logger.h"

namespace ba = boost::asio;
using ba::ip::tcp;
using boost::system::error_code;
using namespace std::chrono_literals;
using namespace std::string_literals;

#ifdef NDEBUG
#define FILTER_DELIMITER_IF_DEBUG(x) {}
#else
#define FILTER_DELIMITER_IF_DEBUG(x) filterDelimiterFromMsg(x)
#endif
	
namespace card {
	char const GeneralTCPTransmitter::DELIMITER = '\n';

	GeneralTCPTransmitter::GeneralTCPTransmitter(boost::asio::io_context& ioContext) :
			ioContext(ioContext),
			onErrorFunc([](error_code ec) {
				throw boost::system::system_error(ec);
			}) {
	}

	void GeneralTCPTransmitter::startImpl() {
		readLoop();
		wasConnectionEtablished = true;
		if(! tx.empty()) writeLoop();
	}
	void GeneralTCPTransmitter::send(std::string msg, bool atFront) {
		FILTER_DELIMITER_IF_DEBUG(msg);
		ba::post(ioContext, [=] {
			if(enqueue(msg + DELIMITER, atFront) && wasConnectionEtablished) {
				writeLoop();
			}
		});
	}
	void GeneralTCPTransmitter::setOnErrorFunc(errorHandlingFunc callback) {
		this->onErrorFunc = callback;
	}
	bool GeneralTCPTransmitter::enqueue(std::string message, bool atFront) {
		atFront &= !tx.empty();
		if(atFront) {
			tx.insert(std::next(std::begin(tx)), std::move(message));
		} else {
			tx.push_back(std::move(message));
		}
		return tx.size() == 1;
	}
	bool GeneralTCPTransmitter::dequeue() {
		assert(!tx.empty());
		tx.pop_front();
		return !tx.empty();
	}
	void GeneralTCPTransmitter::writeLoop() {
		ba::async_write(getSocket(), ba::buffer(tx.front()), [this, self = shared_from_this()](error_code ec, std::size_t n) {
			if(dequeue() && !ec) {
				onWrite();
				writeLoop();
			}
		});
	}
	void GeneralTCPTransmitter::readLoop() {
		ba::async_read_until(getSocket(), rx, DELIMITER, [this, self = shared_from_this()](error_code ec, std::size_t bytesTransferred) {
			if(! ec) {
				std::string json{
					boost::asio::buffers_begin(rx.data()),
					boost::asio::buffers_begin(rx.data()) + bytesTransferred - sizeof(DELIMITER)
				};
				rx.consume(bytesTransferred);

				onReceive(json);
				readLoop();
			} else {
				onErrorFunc(ec);
			}
		});
	}
	void card::GeneralTCPTransmitter::filterDelimiterFromMsg(std::string& msg) {
		auto sizeBefore = msg.size();
		msg.erase(std::remove(msg.begin(), msg.end(), DELIMITER), msg.end());
		auto sizeAfter = msg.size();

		if(sizeBefore != sizeAfter) {
			log(LogSeverity::WARNING, "Message sent by GeneralTCPTransmitter contains the delimiter char.");
		}
	}
	void card::GeneralTCPTransmitter::onWrite() {
		// do nothing
	}
}