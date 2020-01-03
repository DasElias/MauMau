#pragma once
#include <shared/packet/CTSPacketTransmitter.h>
#include <shared/packet/STCPacketTransmitter.h>
#include <memory>

namespace card {
	class LocalPacketTransmitter : public CTSPacketTransmitter, public STCPacketTransmitter {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<ParticipantOnServer> localParticipantOnServer;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LocalPacketTransmitter(std::shared_ptr<ParticipantOnServer> localParticipantOnServer);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void sendPacketToServer(ClientToServerPacket& p) override;
			void sendPacketToClient(Packet& pkt, std::shared_ptr<ParticipantOnServer> receiver) override;
			void sendPacketToClients(Packet& pkt, std::vector<std::shared_ptr<ParticipantOnServer>> receivers) override;
	};
}