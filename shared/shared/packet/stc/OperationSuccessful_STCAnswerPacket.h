#pragma once
#include "../Packet.h"

namespace card {
	class OperationSuccessful_STCAnswerPacket : public Packet {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 300;

		private:
			static std::string const STATUS_CODE_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			bool wasSuccessful_field;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			OperationSuccessful_STCAnswerPacket(bool wasSuccessful);
			OperationSuccessful_STCAnswerPacket(const OperationSuccessful_STCAnswerPacket&) = default;
			OperationSuccessful_STCAnswerPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool wasSuccessful() const;
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}