#pragma once
#include <shared/packet/STCPacketTransmitter.h>
#include <shared/server/ServerRoom.h>
#include <shared/model/Avatar.h>

namespace card {
	class LocalServer {
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
			LocalServer(std::shared_ptr<STCPacketTransmitter> packetTransmitter, std::size_t amountOfOpponents, const std::shared_ptr<ParticipantOnServer>& localParticipantOnServer, RoomOptions roomOptions);
			LocalServer(const LocalServer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			ServerRoom& getRoom();

			void joinAiPlayer();
			std::vector<std::string> getOpponentUsernames();
			std::vector<Avatar> getOpponentAvatars();

		private:
			void initJoinAiPlayers(std::size_t amountOfOpponents);

	};
}