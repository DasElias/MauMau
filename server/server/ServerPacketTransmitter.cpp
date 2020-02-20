#include "ServerPacketTransmitter.h"
#include <shared/packet/CTSPacketConstructorFromJson.h>
#include "ConnectionToClient.h"
#include "ThreadSynchronizer.h"

namespace card {
	void ServerPacketTransmitter::onReceive(std::string& msg, std::shared_ptr<ConnectionToClient> sender) {
		std::mutex& mutex = threadSyncronizer_getMutex();
		std::lock_guard<std::mutex> lockGuard(mutex);

		constructAndProcessCTSPacketFromJson(msg, [this, sender](ClientToServerPacket& packet) {
			optionalSuccessAnswerPacket answerPacket = std::nullopt;
			int packetType = packet.getPacketType();

			for(auto& handler : permanentListeners.getValuesOrEmptyVector(packetType)) {
				answerPacket = handler(packet, sender);
				if(answerPacket) goto packetFound;
			}
			if(participants.left.find(sender) != participants.left.end()) {
				auto senderAsParticipant = participants.left.at(sender);
				for(auto& handler : getRegisteredCallbacks(packet)) {
					answerPacket = handler(packet, senderAsParticipant);
					if(answerPacket) goto packetFound;
				}
			}		

			packetFound:
			if(!answerPacket) answerPacket = OperationSuccessful_STCAnswerPacket(false);
			this->sendPacketToClient(* answerPacket, sender);
		});
	}
	void ServerPacketTransmitter::sendPacketToClient(Packet& pkt, std::shared_ptr<ParticipantOnServer> receiver) {
		if(receiver->isRealPlayer()) {
			auto& conn = this->participants.right.at(receiver);
			sendPacketToClient(pkt, conn);
		}
		
	}
	void ServerPacketTransmitter::sendPacketToClients(Packet& pkt, std::vector<std::shared_ptr<ParticipantOnServer>> receivers) {
		for(auto& rec : receivers) {
			sendPacketToClient(pkt, rec);
		}
	}
	void ServerPacketTransmitter::sendPacketToClient(Packet& pkt, const std::shared_ptr<ConnectionToClient> conn) {
		std::string jsonMsg = pkt.getJson();
		conn->send(std::move(jsonMsg));
	}
	void ServerPacketTransmitter::addListenerForClientPkt(int packetId, ClientPacketListenerCallbackWithConnection callback) {
		permanentListeners.insertElement(packetId, callback);
	}
	void ServerPacketTransmitter::removeListenerForClientPkt(int packetId, ClientPacketListenerCallbackWithConnection callback) {
		permanentListeners.removeElement(packetId, callback);
	}
	bool ServerPacketTransmitter::wasParticipantRegistered(const std::shared_ptr<ConnectionToClient>& conn) {
		return participants.left.find(conn) != participants.left.end();
	}
	std::shared_ptr<ParticipantOnServer> ServerPacketTransmitter::getRegisteredParticipant(const std::shared_ptr<ConnectionToClient>& conn) {
		if(!wasParticipantRegistered(conn)) {
			throw std::runtime_error("This connection hasn't been registered before");
		}

		return participants.left.at(conn);
	}
	void ServerPacketTransmitter::registerParticipant(const std::shared_ptr<ConnectionToClient>& conn, const std::shared_ptr<ParticipantOnServer>& newParticipant) {
		if(!conn || !newParticipant) throw std::runtime_error("Can't register a participiant where he or the connection is null");
		if(participants.left.find(conn) != participants.left.end() || participants.right.find(newParticipant) != participants.right.end()) {
			throw std::runtime_error("The connection or the participant has already been registered.");
		}
		this->participants.insert(participantsBimapType::value_type(conn, newParticipant));
	}
	void ServerPacketTransmitter::unregisterParticipant(std::shared_ptr<ConnectionToClient> conn) {
		if(! conn) throw std::runtime_error("Connection is null");
		if(! wasParticipantRegistered(conn)) {
			throw std::runtime_error("This connection hasn't been registered before");
		}

		participants.left.erase(conn);
	}


}