#include "LocalPacketTransmitter.h"

namespace card {
	LocalPacketTransmitter::LocalPacketTransmitter(std::shared_ptr<ParticipantOnServer> localParticipantOnServer) :
			localParticipantOnServer(localParticipantOnServer) {
	}
	void LocalPacketTransmitter::sendPacketToServer(ClientToServerPacket& p) {
		optionalSuccessAnswerPacket answerPacket = std::nullopt;
		for(auto& handler : STCPacketTransmitter::getRegisteredCallbacks(p)) {
			answerPacket = handler(p, localParticipantOnServer);
			if(answerPacket) break;
		}

		if(!answerPacket) answerPacket = OperationSuccessful_STCAnswerPacket(false);

		this->sendPacketToClient(*answerPacket, localParticipantOnServer);
	}
	void LocalPacketTransmitter::sendPacketToClient(Packet& pkt, std::shared_ptr<ParticipantOnServer> receiver) {
		if(receiver == localParticipantOnServer) {
			CTSPacketTransmitter::onPackageReceiveFromServer(pkt);
		}
	}
	void LocalPacketTransmitter::sendPacketToClients(Packet& pkt, std::vector<std::shared_ptr<ParticipantOnServer>> receivers) {
		for(auto& r : receivers) {
			this->sendPacketToClient(pkt, r);
		}
	}
}