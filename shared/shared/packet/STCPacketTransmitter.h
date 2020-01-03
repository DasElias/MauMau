#pragma once
#include "cts/ClientToServerPacket.h"
#include "ClientPacketListenerCallback.h"
#include "ParticipantOnServer.h"
#include "../utils/MapWithMultipleValues.h"

namespace card {
	class STCPacketTransmitter {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			MapWithMultipleValues<int, ClientPacketListenerCallback> permanentListeners;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			STCPacketTransmitter() = default;

		public:
			STCPacketTransmitter(const STCPacketTransmitter&) = delete;
			virtual ~STCPacketTransmitter() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void sendPacketToClient(Packet& pkt, std::shared_ptr<ParticipantOnServer> receiver) =0;
			virtual void sendPacketToClients(Packet& pkt, std::vector<std::shared_ptr<ParticipantOnServer>> receivers) =0;
			void sendPacketToClientsButNotTo(Packet& pkt, std::vector<std::shared_ptr<ParticipantOnServer>> allReceivers, std::shared_ptr<ParticipantOnServer> excludedReceiver);

			void addListenerForClientPkt(int packetId, ClientPacketListenerCallback callback);
			void removeListenerForClientPkt(int packetId, ClientPacketListenerCallback callback);

		protected:
			std::vector<ClientPacketListenerCallback> getRegisteredCallbacks(ClientToServerPacket& p);
	};
}