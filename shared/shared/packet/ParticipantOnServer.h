#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../model/Avatar.h"

namespace card {
	class ParticipantOnServer {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::string username;
			Avatar avatar;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ParticipantOnServer(std::string username, Avatar avatar);

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static std::vector<std::string> getVectorWithUsernames(const std::vector<std::shared_ptr<ParticipantOnServer>>& players);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual bool isRealPlayer() const;
			std::string getUsername() const;
			Avatar getAvatar() const;

	};
}