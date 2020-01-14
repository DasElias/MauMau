#pragma once
#include <string>

namespace card {
	enum class LogSeverity {
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERR,
		FATAL
	};

	void initLogger(std::string logLatest, std::string logWarning);
	void log(LogSeverity severity, int message);
	void log(LogSeverity severity, std::string message);
	void cleanUp();
}