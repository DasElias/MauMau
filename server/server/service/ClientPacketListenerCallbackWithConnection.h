#pragma once
#include <functional>
#include <optional>
#include <shared/packet/cts/ClientToServerPacket.h>
#include <shared/packet/stc/OperationSuccessful_STCAnswerPacket.h>
#include "ConnectionToClient.h"

namespace card {
	typedef std::optional<OperationSuccessful_STCAnswerPacket> optionalSuccessAnswerPacket;
	typedef std::function<optionalSuccessAnswerPacket(ClientToServerPacket&, const std::shared_ptr<AbstractConnectionToClient>&)> cplWithConnFunction;

	class ClientPacketListenerCallbackWithConnection {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int identity;
			cplWithConnFunction function;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ClientPacketListenerCallbackWithConnection(cplWithConnFunction function) :
				function(function) {
				static int id = 0;
				identity = id++;
			}

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			inline bool operator==(const ClientPacketListenerCallbackWithConnection& other) const {
				return identity == other.identity;
			}
			inline cplWithConnFunction getFunction() const {
				return function;
			}
			inline optionalSuccessAnswerPacket operator()(ClientToServerPacket& pkt, const std::shared_ptr<AbstractConnectionToClient>& conn) {
				return function(pkt, conn);
			};


	};
}


