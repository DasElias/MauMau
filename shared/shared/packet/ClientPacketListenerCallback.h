#pragma once
#include <functional>
#include <optional>
#include "cts/ClientToServerPacket.h"
#include "stc/OperationSuccessful_STCAnswerPacket.h"
#include "ParticipantOnServer.h"
#include <memory>

namespace card {
	typedef std::optional<OperationSuccessful_STCAnswerPacket> optionalSuccessAnswerPacket;
	typedef std::function<optionalSuccessAnswerPacket(ClientToServerPacket&, const std::shared_ptr<ParticipantOnServer>&)> cplFunction;

	class ClientPacketListenerCallback {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int identity;
			cplFunction function;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ClientPacketListenerCallback(cplFunction function) :
				function(function) {
				static int id = 0;
				identity = id++;
			}

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			inline bool operator==(const ClientPacketListenerCallback& other) const {
				return identity == other.identity;
			}
			inline cplFunction getFunction() const {
				return function;
			}
			inline optionalSuccessAnswerPacket operator()(ClientToServerPacket& pkt, const std::shared_ptr<ParticipantOnServer>& sender) {
				return function(pkt, sender);
			};


	};
}


