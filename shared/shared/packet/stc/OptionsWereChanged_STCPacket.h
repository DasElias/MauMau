#pragma once
#include "../Packet.h"
#include <map>
#include <string>
#include <nlohmann/json.hpp>

namespace card {
	class OptionsWereChanged_STCPacket : public Packet {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 106;

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
			OptionsWereChanged_STCPacket(std::map<std::string, int> options);
			OptionsWereChanged_STCPacket(nlohmann::json jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::map<std::string, int> getOptions() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}