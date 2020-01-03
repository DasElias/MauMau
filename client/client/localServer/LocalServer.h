#pragma once
#include <shared/packet/STCPacketTransmitter.h>
#include <shared/server/ServerRoom.h>

namespace card {
	class LocalServer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const OPPONENTS_COUNT = 3;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<STCPacketTransmitter> packetTransmitter;
			std::shared_ptr<ParticipantOnServer> localParticipantOnServer;
			ServerRoom room;
		
			
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LocalServer(std::shared_ptr<STCPacketTransmitter> packetTransmitter, const std::shared_ptr<ParticipantOnServer>& localParticipantOnServer);
			LocalServer(const LocalServer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			ServerRoom& getRoom();

			void joinAiPlayer();
			std::vector<std::string> getOpponentUsernames();

		private:
			void initJoinAiPlayers();

	};
}