#include "AbstractServerPacketTransmitterDummy.h"

namespace card {
	bool AbstractServerPacketTransmitterDummy::wasParticipantRegistered(const std::shared_ptr<AbstractConnectionToClient>& conn) {
		return std::find(registeredConnections.begin(), registeredConnections.end(), conn) != registeredConnections.end();
	}
	void AbstractServerPacketTransmitterDummy::registerParticipant(const std::shared_ptr<AbstractConnectionToClient>& conn, const std::shared_ptr<ParticipantOnServer>& newParticipant) {
		if(wasParticipantRegistered(conn)) throw std::runtime_error("Tried to register a participant twice.");
		registeredConnections.push_back(conn);
	}
	void AbstractServerPacketTransmitterDummy::unregisterParticipant(std::shared_ptr<AbstractConnectionToClient> conn) {
		if(! wasParticipantRegistered(conn)) throw std::runtime_error("Tried to unregister a participant which isn't registered.");
		registeredConnections.erase(std::find(registeredConnections.begin(), registeredConnections.end(), conn));
	}
	void AbstractServerPacketTransmitterDummy::sendPacketToClient(Packet& pkt, std::shared_ptr<ParticipantOnServer> receiver) {
		// empty method body
	}
	void AbstractServerPacketTransmitterDummy::sendPacketToClients(Packet& pkt, std::vector<std::shared_ptr<ParticipantOnServer>> receivers) {
		// empty method body
	}
}