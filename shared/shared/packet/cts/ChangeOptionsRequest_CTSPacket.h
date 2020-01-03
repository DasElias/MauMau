#pragma once
#include "ClientToServerPacket.h"
#include <map>
#include <string>

namespace card {
	class ChangeOptionsRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 402;

		private:
			static std::string const OPTIONS_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::map<std::string, int> options;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ChangeOptionsRequest_CTSPacket(std::map<std::string, int> options);
			ChangeOptionsRequest_CTSPacket(nlohmann::json jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::map<std::string, int> getOptions() const;
			

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}