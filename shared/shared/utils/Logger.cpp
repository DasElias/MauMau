#include "Logger.h"

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>

namespace card {
	void initLogger(std::string logFolder) {
		static const std::string COMMON_FMT("[%TimeStamp%][%Severity%]:  %Message%");
		boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

		// Output message to console
		boost::log::add_console_log(
			std::cerr,
			boost::log::keywords::format = COMMON_FMT,
			boost::log::keywords::auto_flush = true
		);
		
		// prevent linker error
		boost::filesystem::current_path();

		// Output message to file
		boost::log::add_file_log(
			boost::log::keywords::target = logFolder,
			boost::log::keywords::file_name = "vclog_%3N.log",
			boost::log::keywords::rotation_size = 1 * 1024 * 1024,
			boost::log::keywords::max_size = 20 * 1024 * 1024,
			boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
			boost::log::keywords::format = COMMON_FMT,
			boost::log::keywords::auto_flush = true
		);
		
		boost::log::add_common_attributes();

		boost::log::core::get()->set_filter(
			boost::log::trivial::severity >= boost::log::trivial::info
		);
	}

	void log(LogSeverity severity, std::string message) {
		BOOST_LOG_TRIVIAL(error) << message;
	}

	void log(LogSeverity severity, int message) {
		log(severity, std::to_string(message));
	}

	void cleanUp() {
		// empty method body
	}
}