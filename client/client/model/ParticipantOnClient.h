#pragma once
#include <string>
#include <shared/model/Avatar.h>


namespace card {
	class ParticipantOnClient {
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
			ParticipantOnClient(std::string username, Avatar avatar);
			ParticipantOnClient(const ParticipantOnClient&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getUsername() const;
			Avatar getAvatar() const;
	};
}