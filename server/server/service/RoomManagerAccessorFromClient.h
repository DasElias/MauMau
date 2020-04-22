#pragma once
#include "ServerPacketTransmitter.h"
#include "RoomManager.h"

namespace card {
	class RoomManagerAccessorFromClient {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            RoomManager roomManager;
            std::shared_ptr<ServerPacketTransmitter> packetTransmitter;
            ClientPacketListenerCallbackWithConnection handler_onJoinRoom;
            ClientPacketListenerCallbackWithConnection handler_onCreateRoom;
            ClientPacketListenerCallbackWithConnection handler_onKickPlayer;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            RoomManagerAccessorFromClient(std::shared_ptr<ServerPacketTransmitter> packetTransmitter);
            ~RoomManagerAccessorFromClient();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void leave(std::shared_ptr<AbstractConnectionToClient> conn);
            std::size_t getAmountOfRooms() const;
            std::size_t getAmountOfUsersInRooms() const;


        private:
            optionalSuccessAnswerPacket listener_onJoinRoom(ClientToServerPacket& p, const std::shared_ptr<AbstractConnectionToClient>& conn);
            optionalSuccessAnswerPacket listener_onCreateRoom(ClientToServerPacket& p, const std::shared_ptr<AbstractConnectionToClient>& conn);
            optionalSuccessAnswerPacket listener_onKickPlayer(ClientToServerPacket& p, const std::shared_ptr<AbstractConnectionToClient>& conn);
	
            std::vector<std::string> getUsernamesOfOtherParticipants(ServerRoom& room, std::string joinedParticipant);
            std::vector<Avatar> getAvatarsOfOtherParticipants(ServerRoom& room, std::string joinedParticipant);
            std::vector<bool> areOtherParticipantsAiPlayers(ServerRoom& room, std::string joinedParticipant);
    };
}