#pragma once
#include "../packet/ParticipantOnServer.h"

namespace card {
	class AiParticipant : public ParticipantOnServer {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			AiParticipant(std::string username, Avatar avatar);

		// ---------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool isRealPlayer() const override;
	};
}