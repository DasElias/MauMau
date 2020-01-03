#include "ParticipantOnServer.h"

namespace card {
	ParticipantOnServer::ParticipantOnServer(std::string username) :
			username(username) {
	}
	std::vector<std::string> ParticipantOnServer::getVectorWithUsernames(const std::vector<std::shared_ptr<ParticipantOnServer>>& players) {
		std::vector<std::string> usernames;

		for(auto& p : players) {
			usernames.push_back(p->getUsername());
		}
		return usernames;
	}
	bool ParticipantOnServer::isRealPlayer() const {
		return true;
	}
	std::string ParticipantOnServer::getUsername() const {
		return username;
	}
}