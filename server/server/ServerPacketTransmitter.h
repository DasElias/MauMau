#pragma once

#include <shared/packet/STCPacketTransmitter.h>
#include <memory>
#include <map>
#include <unordered_map>
#include <boost/bimap.hpp>
#include "ClientPacketListenerCallbackWithConnection.h"

namespace card {
	class ServerPacketTransmitter : public STCPacketTransmitter {
		typedef boost::bimap<std::shared_ptr<ConnectionToClient>, std::shared_ptr<ParticipantOnServer>> participantsBimapType;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			participantsBimapType participants;
			MapWithMultipleValues<int, ClientPacketListenerCallbackWithConnection> permanentListeners;


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void onReceive(std::string& msg, std::shared_ptr<ConnectionToClient> sender);

			void sendPacketToClient(Packet& pkt, const std::shared_ptr<ConnectionToClient> conn);
			void sendPacketToClient(Packet& pkt, std::shared_ptr<ParticipantOnServer> receiver) override;
			void sendPacketToClients(Packet& pkt, std::vector<std::shared_ptr<ParticipantOnServer>> receivers) override;

			void addListenerForClientPkt(int packetId, ClientPacketListenerCallbackWithConnection callback);
			void removeListenerForClientPkt(int packetId, ClientPacketListenerCallbackWithConnection callback);

			bool wasParticipantRegistered(const std::shared_ptr<ConnectionToClient>& conn);
			std::shared_ptr<ParticipantOnServer> getRegisteredParticipant(const std::shared_ptr<ConnectionToClient>& conn);
			std::shared_ptr<ConnectionToClient> getConnectionOrNull(const std::shared_ptr<ParticipantOnServer>& participant);
			void registerParticipant(const std::shared_ptr<ConnectionToClient>& conn, const std::shared_ptr<ParticipantOnServer>& newParticipant);
			void unregisterParticipant(std::shared_ptr<ConnectionToClient> conn);
	};
}