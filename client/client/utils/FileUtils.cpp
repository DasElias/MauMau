#include "FileUtils.h"

#include <boost/filesystem.hpp>

using namespace boost;
namespace card {
	std::string getApplicationFolder() {
		std::string applFolder = std::string(getenv("APPDATA"));
		applFolder = applFolder.append("\\CardEngine");
		
		if(! boost::filesystem::exists(applFolder)) boost::filesystem::create_directory(applFolder);

		return applFolder;
	}

	void createFile(std::string path) {
		std::ofstream file{path};
	}
}