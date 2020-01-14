#include "Logger.h"
#include <loguru/loguru.hpp>

namespace card {
	void initLogger(std::string logLatest, std::string logWarning) {
		loguru::add_file(logLatest.c_str(), loguru::Truncate, loguru::Verbosity_MAX);
		loguru::add_file(logWarning.c_str(), loguru::Append, loguru::Verbosity_WARNING);
	}

	void log(LogSeverity severity, std::string message) {
		switch(severity) {
			case LogSeverity::FATAL:
				LOG_F(FATAL, message.c_str());
				break;
			case LogSeverity::ERR:
				LOG_F(ERROR, message.c_str());
				break;
			case LogSeverity::WARNING:
				LOG_F(WARNING, message.c_str());
				break;
			default:
				LOG_F(INFO, message.c_str());
				break;
		}

	}

	void log(LogSeverity severity, int message) {
		log(severity, std::to_string(message));
	}

	void cleanUp() {
		// empty method body
	}
}