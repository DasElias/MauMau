#pragma once
#include "AbstractGameFacade.h"
#include "../network/ClientPacketTransmitter.h"
#include "../network/ConnectionToServer.h"
#include <shared/packet/cts/RoomJoinRequest_CTSPacket.h>
#include <shared/packet/cts/RoomCreationRequest_CTSPacket.h>
#include <optional>
#include <string>

namespace card {
	class NetworkGameFacade : public AbstractGameFacade {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			boost::asio::io_context ioContext;
	
			Avatar avatar;
			bool isWaitingForResponse_field;
			std::optional<std::string> errorMsgInPlainText;
			std::string usernameOfLocalPlayer;
			std::shared_ptr<ConnectionToServer> conn;
			std::shared_ptr<ClientPacketTransmitter> packetTransmitter;
			std::unique_ptr<AbstractRoomLeaveHandler> roomLeaveHandler;
			std::unique_ptr<AbstractReturnBackToMenuHandler> gameEndHandler;
			std::unique_ptr<ProxyRoom> room;

			ServerPacketListenerCallback handler_enteringRoomSuccessReport;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			NetworkGameFacade(NetworkErrorHandler& errorHandler, std::unique_ptr<AbstractRoomLeaveHandler> roomLeaveHandler, std::unique_ptr<AbstractReturnBackToMenuHandler> gameEndHandler, std::string username, Avatar avatar);
			~NetworkGameFacade();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool hasErrorOccuredOnEstablishingConnection() const;
			bool isWaitingForResponse() const;
			std::optional<std::string> getErrorMsgOnEstablishingConnectionInPlainText();
			bool isGameRunning() const override;
			ProxyMauMauGame& getGame() override;
			bool isRoomAvailable() const;
			ProxyRoom& getRoom() override;

		protected:
			std::shared_ptr<CTSPacketTransmitter> getPacketTransmitter();

		private:
			void listener_enteringRoomSuccessReport(Packet& p);
			void setErrorMsgOnConnectionFail(boost::system::error_code ec);
			void setErrorMsgForSuccessReport(int statusCode);
	};
}