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
			bool isAiPlayer_field;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ParticipantOnClient(std::string username, Avatar avatar, bool isAiPlayer);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getUsername() const;
			Avatar getAvatar() const;
			bool isAiPlayer() const;
	};
}