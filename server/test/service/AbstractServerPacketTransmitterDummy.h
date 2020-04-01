#pragma once
#include <service/AbstractServerPacketTransmitter.h>
#include <shared/packet/Packet.h>

namespace card {
	class AbstractServerPacketTransmitterDummy : public AbstractServerPacketTransmitter {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::vector<std::shared_ptr<AbstractConnectionToClient>> registeredConnections;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool wasParticipantRegistered(const std::shared_ptr<AbstractConnectionToClient>& conn) override;
			void registerParticipant(const std::shared_ptr<AbstractConnectionToClient>& conn, const std::shared_ptr<ParticipantOnServer>& newParticipant) override;
			void unregisterParticipant(std::shared_ptr<AbstractConnectionToClient> conn) override;

			void sendPacketToClient(Packet& pkt, std::shared_ptr<ParticipantOnServer> receiver) override;
			void sendPacketToClients(Packet& pkt, std::vector<std::shared_ptr<ParticipantOnServer>> receivers) override;
	};
}