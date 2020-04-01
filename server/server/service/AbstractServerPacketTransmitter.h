#pragma once
#include "AbstractConnectionToClient.h"
#include <shared/packet/ParticipantOnServer.h>
#include <shared/packet/STCPacketTransmitter.h>

namespace card {
	class AbstractServerPacketTransmitter : public STCPacketTransmitter {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			AbstractServerPacketTransmitter() = default;
			virtual ~AbstractServerPacketTransmitter() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual bool wasParticipantRegistered(const std::shared_ptr<AbstractConnectionToClient>& conn) =0;
			virtual void registerParticipant(const std::shared_ptr<AbstractConnectionToClient>& conn, const std::shared_ptr<ParticipantOnServer>& newParticipant) =0;
			virtual void unregisterParticipant(std::shared_ptr<AbstractConnectionToClient> conn) =0;
	};
}