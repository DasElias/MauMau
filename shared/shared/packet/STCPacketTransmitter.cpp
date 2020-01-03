#include "STCPacketTransmitter.h"
#include <algorithm>

namespace card {
	void STCPacketTransmitter::sendPacketToClientsButNotTo(Packet& pkt, std::vector<std::shared_ptr<ParticipantOnServer>> allReceivers, std::shared_ptr<ParticipantOnServer> excludedReceiver) {
		allReceivers.erase(std::remove(allReceivers.begin(), allReceivers.end(), excludedReceiver), allReceivers.end());
		this->sendPacketToClients(pkt, allReceivers);
	}

	void STCPacketTransmitter::addListenerForClientPkt(int packetId, ClientPacketListenerCallback callback) {
		permanentListeners.insertElement(packetId, callback);
	}
	void STCPacketTransmitter::removeListenerForClientPkt(int packetId, ClientPacketListenerCallback callback) {
		permanentListeners.removeElement(packetId, callback);
	}
	std::vector<ClientPacketListenerCallback> STCPacketTransmitter::getRegisteredCallbacks(ClientToServerPacket& p) {
		int packetType = p.getPacketType();
		return permanentListeners.getValuesOrEmptyVector(packetType);
	}
	
}