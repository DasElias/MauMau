#pragma once
#include "ServerPacketListenerCallback.h"
#include "cts/ClientToServerPacket.h"
#include <vector>
#include <map>

namespace card {
	class CTSPacketTransmitter {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::map<int, std::vector<ServerPacketListenerCallback>> permanentListeners;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			CTSPacketTransmitter();

		public:
			CTSPacketTransmitter(const CTSPacketTransmitter&) = delete;
			virtual ~CTSPacketTransmitter() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void sendPacketToServer(ClientToServerPacket& p) =0;
			void addListenerForServerPkt(int packetId, ServerPacketListenerCallback callback);
			void removeListenerForServerPkt(int packetId, ServerPacketListenerCallback callback);


		protected:
			void onPackageReceiveFromServer(Packet& p);
	};
}